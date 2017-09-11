/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_memory_manager.h"
#include "bcore_control.h"
#include "bcore_btree.h"
#include "bcore_string.h"
#include "bcore_threads.h"
#include "bcore_name_manager.h"
#include <time.h>
#include <math.h>

/**********************************************************************************************************************/

void* bcore_external_b_alloc( void* current_ptr, sz_t requested_bytes, sz_t* granted_bytes )
{
	if( requested_bytes == 0 )
	{
		if( current_ptr ) free( current_ptr );
		current_ptr = NULL;
		if( granted_bytes ) *granted_bytes = 0;
	}
	else
	{
        if( current_ptr )
        {
            current_ptr = realloc( current_ptr, requested_bytes );
        }
        else
        {
            current_ptr = malloc( requested_bytes );
        }
        if( !current_ptr ) ERR( "Failed allocating %lu bytes", requested_bytes );
        if( granted_bytes ) *granted_bytes = requested_bytes;
    }
	return current_ptr;
}

void* bcore_external_bn_alloc( void* current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes )
{
    return bcore_external_b_alloc( current_ptr, requested_bytes, granted_bytes );
}

void* bcore_external_alloc( void* ptr, sz_t size )
{
    return bcore_external_b_alloc( ptr, size, NULL );
}

void* bcore_external_un_alloc( sz_t unit_bytes, void* current_ptr, sz_t current_units, sz_t requested_units, sz_t* reserved_units )
{
    void* reserved_ptr = bcore_external_bn_alloc( current_ptr, unit_bytes * current_units, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

void* bcore_external_u_alloc( sz_t unit_bytes, void* current_ptr, sz_t requested_units, sz_t* reserved_units )
{
    void* reserved_ptr = bcore_external_b_alloc( current_ptr, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/** Token-Manager
 *
 *  Fragmentation-free and fast (O(1)) pool based dynamic management using fixed sized blocks.
 *  A free block is identified by a token representing its address. Tokens are managed in a stack.
 *  An alloc-request consumes the top token from stack. A free-request pushes the token back onto the stack.
 *
 *  The instance token_manager_s occupies the memory-pool; being its header. This supports efficient (O(log(n))
 *  determination of the correct token-manager by the memory-manager (s. algorithm below).
 *
 *  Token managers can be run in full-alignment-mode in which they are aligned to pool_size, which is
 *  a power of two. This allows O(1) lookup of the pool manager from any of its managed allocations.
 *
 */
typedef struct token_manager_s
{
    sz_t pool_size;
    sz_t block_size;
    u1_t stack_size;  // size of token-stack
    u1_t stack_index; // index into token-stack

    /** aligned
     *  The memory-pool is considered aligned when the integer-evaluation of its address
     *  is a multiple of pool_size, which means that the pool address can be obtained by
     *  from any pointer inside the pool by a mere integer division.
     */
    bool aligned;

    struct block_manager_s* parent;
    sz_t parent_index;
    u1_t token_stack[]; // stack of block-tokens (part of pool)
} token_manager_s;


static void token_manager_s_init( token_manager_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

static void token_manager_s_down( token_manager_s* o )
{
}

static token_manager_s* token_manager_s_create( sz_t pool_size, sz_t block_size, bool align )
{
    if( ( pool_size & ( pool_size - 1 ) ) != 0 ) ERR( "pool_size %lu is not a power of two", pool_size );
    sz_t stack_size = pool_size / block_size;
    if( stack_size > 0x10000 ) ERR( "stack_size exceeds %lu", stack_size );
    sz_t reserved_size   = sizeof( token_manager_s ) + sizeof( u1_t ) * stack_size;
    sz_t reserved_blocks = reserved_size / block_size + ( ( reserved_size % block_size ) > 0 );
    if( stack_size < ( reserved_blocks + 1 ) ) ERR( "pool_size %lu is too small", pool_size );

    token_manager_s* o;
    if( align )
    {
        o = aligned_alloc( pool_size, pool_size );
        if( !o ) ERR( "Failed aligned allocating %lu bytes", pool_size );
    }
    else
    {
        o = aligned_alloc( BCORE_MEMORY_MANAGER_ALIGN, pool_size );
        if( !o ) ERR( "Failed allocating %lu bytes", pool_size );
    }

    token_manager_s_init( o );
    o->aligned = ( ( intptr_t )o & ( intptr_t )( pool_size - 1 ) ) == 0;
    o->pool_size = pool_size;
    o->block_size = block_size;
    o->stack_size = stack_size;
    o->stack_index = 0;
    for( sz_t i = 0; i < o->stack_size; i++ ) o->token_stack[ i ] = ( i + reserved_blocks ) < stack_size ? ( i + reserved_blocks ) : 0;
    return o;
}

static void token_manager_s_discard( token_manager_s* o )
{
    if( !o ) return;
    token_manager_s_down( o );
    free( o );
}

static bool token_manager_s_is_full( token_manager_s* o )
{
    return o->token_stack[ o->stack_index ] == 0;
}

static bool token_manager_s_is_empty( token_manager_s* o )
{
    return o->stack_index == 0;
}

static void* token_manager_s_alloc( token_manager_s* o )
{
    assert( !token_manager_s_is_full( o ) );
    void* ret = ( u0_t* )o + o->token_stack[ o->stack_index ] * o->block_size;
    assert( ( u0_t* )ret >= ( u0_t* )o + sizeof( token_manager_s ) );
    o->stack_index++;
    return ret;
}

// forward declarations (implementation below)
static void block_manager_s_full_to_free( struct block_manager_s* o, token_manager_s* child );
static void block_manager_s_free_to_empty( struct block_manager_s* o, token_manager_s* child );

static void token_manager_s_free( token_manager_s* o, void* ptr )
{
    #ifdef RTCHECKS
        if( o->stack_index == 0 ) ERR( "Block manager is empty." );
        if( ( sz_t )( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) > o->pool_size ) ERR( "Attempt to free memory outside pool." );
    #endif

    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;

    #ifdef RTCHECKS
        if( token * o->block_size < sizeof( token_manager_s ) ) ERR( "Attempt to free reserved memory." );
        for( sz_t i = o->stack_index; i < o->stack_size; i++ ) if( o->token_stack[ i ] == token ) ERR( "Attempt to free memory that is declared free." );
    #endif // RTCHECKS

    if( o->token_stack[ o->stack_index ] == 0 ) block_manager_s_full_to_free( o->parent, o );

    o->stack_index--;
    o->token_stack[ o->stack_index ] = token;

    if( o->stack_index == 0 ) block_manager_s_free_to_empty( o->parent, o );
}

static sz_t token_manager_s_total_alloc( const token_manager_s* o )
{
    return o->block_size * o->stack_index;
}

static sz_t token_manager_s_total_space( const token_manager_s* o )
{
    return o->pool_size + o->stack_size * sizeof( u1_t );
}

static bcore_string_s* token_manager_s_status( const token_manager_s* o, int detail_level )
{
    bcore_string_s* str = bcore_string_s_create();
    if( detail_level <= 0 ) return str;
    bcore_string_s_pushf( str, "    pool_size:   %lu\n",  o->pool_size );
    bcore_string_s_pushf( str, "    block_size:  %lu\n",  o->block_size );
    bcore_string_s_pushf( str, "    stack_size:  %lu\n",  o->stack_size );
    bcore_string_s_pushf( str, "    aligned:     %s\n",   o->aligned ? "true" : "false" );
    bcore_string_s_pushf( str, "    stack_index: %hhu\n", o->stack_index );
    bcore_string_s_pushf( str, "    total alloc: %lu\n", token_manager_s_total_alloc( o ) );
    bcore_string_s_pushf( str, "    total space: %lu\n", token_manager_s_total_space( o ) );
    return str;
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/** Block-Manager
 *
 *  Contains an array of token-managers, each of the same block-size.
 *  A token-manager has one of three stats: 'full', 'free' and 'empty'.
 *  A  'full'  token-manager has no space left for allocation
 *  A  'free'  token-manager has (some) space available for allocation.
 *  An 'empty' token-manager has all space available for allocation.
 *  Token managers are linearly arranged by state in the order: full, free, empty.
 *  An index (free_index) points to the full-free border.
 *
 *  Alloc request: O(1)
 *    - redirected to the free_indexe(d) token-manager.
 *    - if that token-manager becomes 'full', free_index is incremented
 *    - if all token-managers are full, a new token-manager is appended at the next alloc request
 *
 *  Free request:
 *    - block_manager_s does not directly receive free requests. Instead the parent-manager directly invokes the
 *      the corresponding token manager.
 *    - If a token-manager turns from full to free, it reports to the block manager, which swaps its position
 *      with the last full token_manager and decrements free_index.
 *    - If a token-manager turns from free to empty, it reports to the block manager, which swaps its position
 *      with the last free token_manager. When enough empty token-managers accumulated (sweep_hysteresis), they
 *      are discarded (memory returned to the system).
 *
 */
typedef struct block_manager_s
{
    sz_t pool_size;  // pool size of all token-managers
    sz_t block_size; // block size of all token-managers
    bool align;      // attempt to align token_managers to pool_size
    token_manager_s** data;
    sz_t size, space;
    sz_t free_index;       // entries equal or above free_index have space for allocation
    f3_t sweep_hysteresis; // if ( empty token-managers ) / ( used token-managers ) < sweep_hysteresis, empty token-managers are discarded
    bool aligned;          // all token managers are aligned to pool_size
    struct bcore_memory_manager_s* parent;
    bcore_btree_vd_s* internal_btree;
} block_manager_s;

static void block_manager_s_init( block_manager_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->aligned = true;
    o->sweep_hysteresis = 0.125;
}

static void block_manager_s_down( block_manager_s* o )
{
    if( o->data )
    {
        for( sz_t i = 0; i < o->size; i++ ) token_manager_s_discard( o->data[ i ] );
        free( o->data );
        o->data = NULL;
        o->size = o->space = 0;
    }
}

static block_manager_s* block_manager_s_create( sz_t pool_size, sz_t block_size, bool align )
{
    block_manager_s* o = malloc( sizeof( block_manager_s ) );
    if( !o ) ERR( "Failed allocating %lu bytes", sizeof( block_manager_s ) );
    block_manager_s_init( o );
    o->pool_size = pool_size;
    o->block_size = block_size;
    o->align = align;
    return o;
}

static void block_manager_s_discard( block_manager_s* o )
{
    if( !o ) return;
    block_manager_s_down( o );
    free( o );
}

void bcore_memory_manager_s_lost_alignment( struct bcore_memory_manager_s* o, const block_manager_s* child );

static void* block_manager_s_alloc( block_manager_s* o )
{
    if( o->free_index == o->size )
    {
        if( o->size == o->space )
        {
            o->space = ( o->space > 0 ) ? o->space * 2 : 1;

            if( o->data )
            {
                o->data = realloc( o->data, sizeof( token_manager_s* ) * o->space );
            }
            else
            {
                o->data = malloc( sizeof( token_manager_s* ) * o->space );
            }

            if( !o->data ) ERR( "Failed allocating %lu bytes", sizeof( token_manager_s* ) * o->space );
        }
        o->data[ o->size ] = token_manager_s_create( o->pool_size, o->block_size, o->align );
        o->data[ o->size ]->parent_index = o->size;
        o->data[ o->size ]->parent = o;
        if( o->aligned && !o->data[ o->size ]->aligned )
        {
            o->aligned = false;
            bcore_memory_manager_s_lost_alignment( o->parent, o );
        }
        if( bcore_btree_vd_s_set( o->internal_btree, o->data[ o->size ] ) != 1 ) ERR( "Failed registering block address." );
        o->size++;
    }
    token_manager_s* child = o->data[ o->free_index ];
    void* ret = token_manager_s_alloc( child );
    if( token_manager_s_is_full( child ) ) o->free_index++;
    return ret;
}

// A child reports turning full --> free
static void block_manager_s_full_to_free( block_manager_s* o, token_manager_s* child )
{
    assert( o->free_index > 0 );

    o->free_index--;

    // swap child with current free position
    sz_t child_index = child->parent_index;
    sz_t swapc_index = o->free_index;

    token_manager_s* swapc = o->data[ swapc_index ];
    o->data[ swapc_index ] = child;
    o->data[ child_index ] = swapc;
    child->parent_index = swapc_index;
    swapc->parent_index = child_index;
}

static sz_t block_manager_s_empty_tail( const block_manager_s* o )
{
    if( o->size == 0 ) return 0;
    sz_t empty_index = o->size;
    while( empty_index > 0 && token_manager_s_is_empty( o->data[ empty_index - 1 ] ) ) empty_index--;
    return o->size - empty_index;
}

// A child reports turning free --> empty
static void block_manager_s_free_to_empty( block_manager_s* o, token_manager_s* child )
{
    // move empty manager to tail (if not already there)
    sz_t child_index = child->parent_index;
    sz_t empty_tail  = block_manager_s_empty_tail( o );
    if( empty_tail < o->size )
    {
        sz_t swapc_index = o->size - empty_tail - 1;
        if( child_index < swapc_index )
        {
            token_manager_s* swapc = o->data[ swapc_index ];
            o->data[ child_index ] = swapc;
            o->data[ swapc_index ] = child;
            child->parent_index = swapc_index;
            swapc->parent_index = child_index;
            empty_tail++;
        }
    }

    if( empty_tail > ( o->size - empty_tail ) * o->sweep_hysteresis ) // discard empty managers when enough accumulated
    {
        while( o->size > 0 && token_manager_s_is_empty( o->data[ o->size - 1 ] ) )
        {
            o->size--;

            if( bcore_btree_vd_s_remove( o->internal_btree, o->data[ o->size ] ) != 1 ) ERR( "Failed removing block address." );

            #ifdef RTCHECKS
                if( bcore_btree_vd_s_exists( o->internal_btree, o->data[ o->size ] ) )      ERR( "Removed block address still exists" );
            #endif

            token_manager_s_discard( o->data[ o->size ] );
            o->data[ o->size ] = NULL;
        }
    }
}

static sz_t block_manager_s_total_alloc( const block_manager_s* o )
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        sum += token_manager_s_total_alloc( o->data[ i ] );
    }
    return sum;
}

static sz_t block_manager_s_total_space( const block_manager_s* o )
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        sum += token_manager_s_total_space( o->data[ i ] );
    }
    return sum;
}

static bcore_string_s* block_manager_s_status( const block_manager_s* o, int detail_level )
{
    bcore_string_s* str = bcore_string_s_create();
    if( detail_level <= 0 ) return str;
    bcore_string_s_pushf( str, "  pool_size:        %lu\n", o->pool_size );
    bcore_string_s_pushf( str, "  block_size:       %lu\n", o->block_size );
    bcore_string_s_pushf( str, "  sweep_hysteresis: %g\n",  o->sweep_hysteresis );
    bcore_string_s_pushf( str, "  aligned:          %s\n",  o->aligned ? "true" : "false" );
    bcore_string_s_pushf( str, "  token_managers:   %lu\n", o->size );
    bcore_string_s_pushf( str, "      full:         %lu\n", o->free_index );
    bcore_string_s_pushf( str, "      empty:        %lu\n", block_manager_s_empty_tail( o ) );
    bcore_string_s_pushf( str, "  total alloc:      %lu\n", block_manager_s_total_alloc( o ) );
    bcore_string_s_pushf( str, "  total space:      %lu\n", block_manager_s_total_space( o ) );
    if( detail_level > 1 )
    {
        for( sz_t i = 0; i < o->size; i++ )
        {
            bcore_string_s_pushf( str, "\nblock manager %lu:\n", i );
            bcore_string_s_push_string_d( str, token_manager_s_status( o->data[ i ], detail_level - 1 ) );
        }
    }
    return str;
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/** Memory-Manager
 *
 *  Contains a fixed-size array of block-managers with exponentially increasing block_size.
 *  (E.g. via size-doubling, but other arrangements are also possible)
 *
 *  Alloc request:
 *     - directed to the block-manager with the smallest fitting bock-size
 *     - if the largest block size is yet too small, the request is passed on to the OS (-->aligned_alloc)
 *       --> O(1) for size requests equal or below largest block size assuming alloc and free requests are statistically
 *           balanced such the overall memory in use is not dramatically varying.
 *
 *  Free request:
 *     - If the previously allocated size is available and all token managers are aligned
 *       the address of the token manager is directly calculated from the allocated address. (O(1))
 *     - Otherwise: The corresponding token-manager is determined via internal_btree from the memory-address
 *       (Olog(n) - where 'n' is the current amount of allocations in circulation.)
 *
 */
typedef struct bcore_memory_manager_s
{
    block_manager_s** data; // block managers are sorted by increasing block size
    sz_t size;
    sz_t pool_size;               // pool size for all token managers
    sz_t min_block_size;
    sz_t max_block_size;
    bool aligned;                 // all token managers are aligned
    sz_t* block_size_array;       // copy of block size values (for fast access)
    bcore_btree_vd_s* internal_btree;
    bcore_btree_ps_s* external_btree;
    bcore_mutex_t mutex;
} bcore_memory_manager_s;

void bcore_memory_manager_s_init( bcore_memory_manager_s* o, sz_t pool_size, sz_t min_block_size, sz_t max_block_size, sz_t stepping_method, bool full_align )
{
    bcore_memzero( o, sizeof( *o ) );
    bcore_mutex_init( &o->mutex );

    o->internal_btree = bcore_btree_vd_s_create( bcore_external_alloc );
    o->external_btree = bcore_btree_ps_s_create( bcore_external_alloc );

    /// The following three values are configurable parameters of memory manager
    o->pool_size      = pool_size;
    o->min_block_size = min_block_size;
    o->max_block_size = max_block_size;

    sz_t mask_bxp  = stepping_method;
    sz_t size_mask = ( 1 << mask_bxp ) - 1;
    sz_t size_inc  = o->min_block_size;
    while( ( size_mask < o->min_block_size ) || ( ( size_mask << 1 ) & o->min_block_size ) != 0 ) size_mask <<= 1;

    sz_t space = 0;

    for( sz_t block_size = o->min_block_size; block_size <= o->max_block_size; block_size += size_inc )
    {
        if( o->size == space )
        {
            space = space > 0 ? space * 2 : 16;

            if( o->data )
            {
                o->data = realloc( o->data, sizeof( block_manager_s* ) * space );
            }
            else
            {
                o->data = malloc( sizeof( block_manager_s* ) * space );
            }

            if( !o->data ) ERR( "Failed allocating %lu bytes", sizeof( block_manager_s* ) * space );
        }
        o->data[ o->size ] = block_manager_s_create( o->pool_size, block_size, full_align );
        o->data[ o->size ]->internal_btree = o->internal_btree;
        o->data[ o->size ]->parent = o;
        o->size++;

        if( block_size > size_mask )
        {
            size_mask <<= 1;
            size_inc  <<= 1;
        }
    }

    o->block_size_array = malloc( o->size * sizeof( sz_t ) );
    if( !o->block_size_array ) ERR( "Failed allocating %lu bytes", o->size * sizeof( sz_t ) );

    o->aligned = true;
    for( sz_t i = 0; i < o->size; i++ )
    {
        o->aligned = o->aligned && o->data[ i ]->aligned;
        o->block_size_array[ i ] = o->data[ i ]->block_size;
    }
}

void bcore_memory_manager_s_down( bcore_memory_manager_s* o )
{
    bcore_mutex_lock( &o->mutex );
    if( o->data )
    {
        for( sz_t i = 0; i < o->size; i++ ) block_manager_s_discard( o->data[ i ] );
        free( o->data );
    }

    bcore_btree_vd_s_discard( o->internal_btree );
    bcore_btree_ps_s_discard( o->external_btree );

    bcore_mutex_unlock( &o->mutex );
    bcore_mutex_down( &o->mutex );
}

bcore_memory_manager_s* bcore_memory_manager_s_create( sz_t pool_size, sz_t min_block_size, sz_t max_block_size, sz_t stepping_method, bool full_align )
{
    bcore_memory_manager_s* o = malloc( sizeof( bcore_memory_manager_s ) );
    if( !o ) ERR( "Failed allocating %lu bytes", sizeof( bcore_memory_manager_s ) );
    bcore_memory_manager_s_init( o, pool_size, min_block_size, max_block_size, stepping_method, full_align );
    return o;
}

void bcore_memory_manager_s_discard( bcore_memory_manager_s* o )
{
    if( !o ) return;
    bcore_memory_manager_s_down( o );
    free( o );
}

void bcore_memory_manager_s_lost_alignment( struct bcore_memory_manager_s* o, const block_manager_s* child )
{
    o->aligned = false;
}

static void* bcore_memory_manager_s_malloc( bcore_memory_manager_s* o, sz_t requested_bytes, sz_t* granted_bytes )
{
    block_manager_s* block_manager = NULL;
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( requested_bytes <= o->block_size_array[ i ] )
        {
            block_manager = o->data[ i ];
            break;
        }
    }

    void* reserved_ptr = NULL;
    if( block_manager )
    {
        reserved_ptr = block_manager_s_alloc( block_manager );
        if( granted_bytes ) *granted_bytes = block_manager->block_size;
    }
    else
    {
        reserved_ptr = aligned_alloc( BCORE_MEMORY_MANAGER_ALIGN, requested_bytes );
        if( !reserved_ptr ) ERR( "Failed allocating %lu bytes.", requested_bytes );
        if( granted_bytes ) *granted_bytes = requested_bytes;
        if( bcore_btree_ps_s_set( o->external_btree, reserved_ptr, requested_bytes ) != 1 ) ERR( "Registering new address failed" );
    }

    return reserved_ptr;
}

static void bcore_memory_manager_s_free( bcore_memory_manager_s* o, void* current_ptr, const sz_t* current_bytes )
{
    if( current_bytes && *current_bytes <= o->max_block_size && o->aligned )
    {
        token_manager_s* token_manager = ( token_manager_s* )( ( intptr_t )current_ptr & ~( intptr_t )( o->pool_size - 1 ) );
        token_manager_s_free( token_manager, current_ptr );
    }
    else
    {
        void* block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, current_ptr );
        if( block_ptr && ( ( (u0_t*)current_ptr - (u0_t*)block_ptr ) < o->pool_size ) )
        {
            token_manager_s_free( block_ptr, current_ptr );
        }
        else
        {
            if( bcore_btree_ps_s_remove( o->external_btree, current_ptr ) != 1 ) ERR( "Attempt to free invalid memory" );
            free( current_ptr );
        }
    }
}

static void* bcore_memory_manager_s_realloc( bcore_memory_manager_s* o, void* current_ptr, const sz_t* current_bytes, sz_t requested_bytes, sz_t* granted_bytes )
{
    token_manager_s* token_manager = NULL;
    if( current_bytes && *current_bytes <= o->max_block_size && o->aligned )
    {
        token_manager = ( token_manager_s* )( ( intptr_t )current_ptr & ~( intptr_t )( o->pool_size - 1 ) );
    }
    else
    {
        void* block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, current_ptr );
        if( block_ptr && ( ( (u0_t*)current_ptr - (u0_t*)block_ptr ) < o->pool_size ) ) token_manager = block_ptr;
    }

    if( token_manager )
    {
        if( requested_bytes > token_manager->block_size )
        {
            void* reserved_ptr = bcore_memory_manager_s_malloc( o, requested_bytes, granted_bytes );
            bcore_memcpy( reserved_ptr, current_ptr, token_manager->block_size );
            token_manager_s_free( token_manager, current_ptr );
            return reserved_ptr;
        }
        else // size reduction
        {
            block_manager_s* block_manager = NULL;
            for( sz_t i = 0; i < o->size; i++ )
            {
                if( requested_bytes <= o->block_size_array[ i ] )
                {
                    block_manager = o->data[ i ];
                    break;
                }
            }

            if( block_manager->block_size != token_manager->block_size )
            {
                void* reserved_ptr = block_manager_s_alloc( block_manager );
                bcore_memcpy( reserved_ptr, current_ptr, requested_bytes );
                token_manager_s_free( token_manager, current_ptr );
                if( granted_bytes ) *granted_bytes = block_manager->block_size;
                return reserved_ptr;
            }
            else
            {
                // same block-size: keep current location
                if( granted_bytes ) *granted_bytes = token_manager->block_size;
                return current_ptr;
            }
        }
    }
    else
    {
        if( requested_bytes <= o->max_block_size ) // new size fits into manager, old size was outside manager
        {
            void* reserved_ptr = bcore_memory_manager_s_malloc( o, requested_bytes, granted_bytes );
            bcore_memcpy( reserved_ptr, current_ptr, requested_bytes );
            if( bcore_btree_ps_s_remove( o->external_btree, current_ptr ) != 1 ) ERR( "Attempt to free invalid memory" );
            free( current_ptr );
            return reserved_ptr;
        }
        else // neither old nor new size handled by this manager
        {
            sz_t* p_current_bytes = bcore_btree_ps_s_val( o->external_btree, current_ptr );
            if( !p_current_bytes ) ERR( "Could not retrieve current external memory" );
            sz_t current_ext_bytes = *p_current_bytes;

            // is requested bytes is less but not significantly less than current bytes, keep current memory
            if( ( requested_bytes < current_ext_bytes ) && ( requested_bytes >= ( current_ext_bytes >> 1 ) ) )
            {
                if( granted_bytes ) *granted_bytes = current_ext_bytes;
                return current_ptr;
            }

            void* reserved_ptr = aligned_alloc( BCORE_MEMORY_MANAGER_ALIGN, requested_bytes );
            if( !reserved_ptr ) ERR( "Failed allocating %lu bytes.", requested_bytes );
            if( granted_bytes ) *granted_bytes = requested_bytes;
            if( bcore_btree_ps_s_set( o->external_btree, reserved_ptr, requested_bytes ) != 1 ) ERR( "Registering new address failed" );

            sz_t copy_bytes = ( requested_bytes < current_ext_bytes ) ? requested_bytes : current_ext_bytes;
            bcore_memcpy( reserved_ptr, current_ptr, copy_bytes );

            if( bcore_btree_ps_s_remove( o->external_btree, current_ptr ) != 1 ) ERR( "Attempt to free invalid memory" );
            free( current_ptr );
            return reserved_ptr;
        }
    }
}

void* bcore_memory_manager_s_b_alloc( bcore_memory_manager_s* o, void* current_ptr, sz_t requested_bytes, sz_t* granted_bytes )
{
    bcore_mutex_lock( &o->mutex );
    void* ret = NULL;
	if( requested_bytes == 0 )
	{
		if( current_ptr )
        {
            bcore_memory_manager_s_free( o, current_ptr, NULL );
        }
        if( granted_bytes ) *granted_bytes = 0;
	}
	else
	{
        if( current_ptr )
        {
            ret = bcore_memory_manager_s_realloc( o, current_ptr, NULL, requested_bytes, granted_bytes );
        }
        else
        {
            ret = bcore_memory_manager_s_malloc( o, requested_bytes, granted_bytes );
        }
    }
    bcore_mutex_unlock( &o->mutex );
	return ret;
}

void* bcore_memory_manager_s_bn_alloc( bcore_memory_manager_s* o, void* current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes )
{
    bcore_mutex_lock( &o->mutex );
    void* ret = NULL;
	if( requested_bytes == 0 )
	{
		if( current_bytes ) // 0 means current_ptr may not be used for free or realloc
        {
            bcore_memory_manager_s_free( o, current_ptr, &current_bytes );
        }
        if( granted_bytes ) *granted_bytes = 0;
	}
	else
	{
        if( current_bytes ) // 0 means current_ptr may not be used for free or realloc
        {
            ret = bcore_memory_manager_s_realloc( o, current_ptr, &current_bytes, requested_bytes, granted_bytes );
        }
        else
        {
            ret = bcore_memory_manager_s_malloc( o, requested_bytes, granted_bytes );
        }
    }
    bcore_mutex_unlock( &o->mutex );
	return ret;
}

void* bcore_memory_manager_s_u_alloc( bcore_memory_manager_s* o, sz_t unit_bytes, void* current_ptr, sz_t requested_units, sz_t* reserved_units )
{
    void* reserved_ptr = bcore_memory_manager_s_b_alloc( o, current_ptr, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

void* bcore_memory_manager_s_un_alloc( bcore_memory_manager_s* o, sz_t unit_bytes, void* current_ptr, sz_t current_units, sz_t requested_units, sz_t* reserved_units )
{
    void* reserved_ptr = bcore_memory_manager_s_bn_alloc( o, current_ptr, unit_bytes * current_units, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

static sz_t bcore_memory_manager_s_total_external_alloc( const bcore_memory_manager_s* o )
{
    return bcore_btree_ps_s_sum( o->external_btree, NULL, NULL );
}

static sz_t bcore_memory_manager_s_total_internal_alloc( const bcore_memory_manager_s* o )
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        sum += block_manager_s_total_alloc( o->data[ i ] );
    }
    return sum;
}

static sz_t bcore_memory_manager_s_total_alloc( const bcore_memory_manager_s* o )
{
    return bcore_memory_manager_s_total_external_alloc( o )
         + bcore_memory_manager_s_total_internal_alloc( o );
}

static sz_t bcore_memory_manager_s_total_space( const bcore_memory_manager_s* o )
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        sum += block_manager_s_total_space( o->data[ i ] );
    }
    return sum;
}

/**********************************************************************************************************************/
// Global manager

bcore_memory_manager_s* bcore_memory_manager_s_g = NULL;

void bcore_create_memory_manager()
{
    const sz_t pool_size       = 0x10000;
    const sz_t min_block_size  = 8;
    const sz_t max_block_size  = 4096;
    const sz_t stepping_method = 1;
    const bool full_align = true;
    bcore_memory_manager_s_g = bcore_memory_manager_s_create( pool_size, min_block_size, max_block_size, stepping_method, full_align );
}

void bcore_discard_memory_manager()
{
    bcore_memory_manager_s_discard( bcore_memory_manager_s_g );
    bcore_memory_manager_s_g = NULL;
}

static void memory_manager_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, bcore_create_memory_manager );
}

static void memory_manager_close()
{
    void bcore_discard_memory_manager();
}

void* bcore_memory_manager_b_alloc( void* current_ptr, sz_t requested_bytes, sz_t* granted_bytes )
{
    assert( bcore_memory_manager_s_g != NULL );
    return bcore_memory_manager_s_b_alloc( bcore_memory_manager_s_g, current_ptr, requested_bytes, granted_bytes );
}

void* bcore_memory_manager_bn_alloc( void* current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes )
{
    assert( bcore_memory_manager_s_g != NULL );
    return bcore_memory_manager_s_bn_alloc( bcore_memory_manager_s_g, current_ptr, current_bytes, requested_bytes, granted_bytes );
}

void* bcore_memory_manager_u_alloc( sz_t unit_bytes, void* current_ptr, sz_t requested_units, sz_t* reserved_units )
{
    void* reserved_ptr = bcore_memory_manager_b_alloc( current_ptr, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

void* bcore_memory_manager_un_alloc( sz_t unit_bytes, void* current_ptr, sz_t current_units, sz_t requested_units, sz_t* reserved_units )
{
    void* reserved_ptr = bcore_memory_manager_bn_alloc( current_ptr, unit_bytes * current_units, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

sz_t bcore_memory_manager_s_granted_space( bcore_memory_manager_s* o )
{
    bcore_mutex_lock( &o->mutex );
    sz_t space = bcore_memory_manager_s_total_alloc( o );
    bcore_mutex_unlock( &o->mutex );
    return space;
}

sz_t bcore_memory_manager_granted_space()
{
    assert( bcore_memory_manager_s_g != NULL );
    return bcore_memory_manager_s_granted_space( bcore_memory_manager_s_g );
}

// not thread-safe
bcore_string_s* bcore_memory_manager_s_status( bcore_memory_manager_s* o, int detail_level )
{
    if( detail_level <= 0 ) return NULL;

    bcore_string_s* str = bcore_string_s_create();
    bcore_string_s_pushf( str, "pool_size:              %lu\n", o->pool_size );
    bcore_string_s_pushf( str, "block managers:         %lu\n", o->size );
    bcore_string_s_pushf( str, "token managers:         %lu\n", bcore_btree_vd_s_count( o->internal_btree, NULL, NULL ) );
    bcore_string_s_pushf( str, "external allocs:        %lu\n", bcore_btree_ps_s_count( o->external_btree, NULL, NULL ) );
    bcore_string_s_pushf( str, "internal_btree depth:   %lu\n", bcore_btree_vd_s_depth( o->internal_btree ) );
    bcore_string_s_pushf( str, "external_btree depth:   %lu\n", bcore_btree_ps_s_depth( o->external_btree ) );
    bcore_string_s_pushf( str, "min_block_size:         %lu\n", o->size > 0 ? o->data[ 0 ]->block_size : 0 );
    bcore_string_s_pushf( str, "max_block_size:         %lu\n", o->size > 0 ? o->data[ o->size - 1 ]->block_size : 0 );
    bcore_string_s_pushf( str, "aligned:                %s\n",  o->aligned ? "true" : "false" );
    bcore_string_s_pushf( str, "total external granted: %lu\n", bcore_memory_manager_s_total_external_alloc( o ) );
    bcore_string_s_pushf( str, "total internal granted: %lu\n", bcore_memory_manager_s_total_internal_alloc( o ) );
    bcore_string_s_pushf( str, "total internal used:    %lu\n", bcore_memory_manager_s_total_space( o ) );
    if( detail_level > 1 )
    {
        for( sz_t i = 0; i < o->size; i++ )
        {
            bcore_string_s_pushf( str, "\nblock manager %lu:\n", i );
            bcore_string_s_push_string_d( str, block_manager_s_status( o->data[ i ], detail_level - 1 ) );
        }
    }

    return str;
}

/**********************************************************************************************************************/
// Testing and evaluation

static bcore_string_s* bcore_memory_alloc_challenge( void* (*alloc)( void* current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes ),
                                                     sz_t table_size, sz_t cycles, sz_t max_alloc, u2_t seed, bool cleanup, bool verbose )
{
    bcore_string_s* str = bcore_string_s_create();

    void** data_table = malloc( table_size * sizeof( void* ) );
    sz_t*  size_table = malloc( table_size * sizeof( sz_t ) );
    for( sz_t i = 0; i < table_size; i++ ) data_table[ i ] = NULL;
    for( sz_t i = 0; i < table_size; i++ ) size_table[ i ] = 0;

    u2_t rval = seed;
    sz_t alloc_attempts = 0;
    sz_t realloc_attempts = 0;
    sz_t free_attempts = 0;
    sz_t alloc_failures = 0;
    sz_t realloc_failures = 0;
    sz_t free_failures = 0;

    for( sz_t j = 0; j < cycles; j++ )
    {
        for( sz_t i = 0; i < table_size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            sz_t idx = rval % table_size;

            // verify table content
            if( size_table[ idx ] > 0 )
            {
                u2_t rv = bcore_xsg_u2( idx + 1 );
                uint8_t* data = data_table[ idx ];
                sz_t sz = size_table[ idx ];
                for( sz_t i = 0; i < sz; i++ )
                {
                    rv = bcore_xsg_u2( rv );
                    if( data[ i ] != ( rv & 255 ) )
                    {
                        ERR( "data failure [%u vs %u].", ( u2_t )data[ i ], ( u2_t )( rv & 255 ) );
                    }
                }
            }

            if( data_table[ idx ] == NULL )
            {
                rval = bcore_xsg_u2( rval );
                sz_t size = pow( ( double )max_alloc, ( rval * pow( 2.0, -32 ) ) );
                data_table[ idx ] = alloc( data_table[ idx ], 0, size, &size_table[ idx ] );
                alloc_attempts++;
                alloc_failures += ( size > 0 ) && ( data_table[ idx ] == NULL );

                // set new content
                if( size_table[ idx ] > 0 )
                {
                    u2_t rv = bcore_xsg_u2( idx + 1 );
                    uint8_t* data = data_table[ idx ];
                    sz_t sz = size_table[ idx ];
                    for( sz_t i = 0; i < sz; i++ ) data[ i ] = ( ( rv = bcore_xsg_u2( rv ) ) & 255 );
                }
            }
            else
            {
                rval = bcore_xsg_u2( rval );
                if( rval & 32 )
                {
                    data_table[ idx ] = alloc( data_table[ idx ], size_table[ idx ], 0, &size_table[ idx ] ); // free
                    free_attempts++;
                    free_failures += ( data_table[ idx ] != NULL );
                }
                else
                {
                    rval = bcore_xsg_u2( rval );
                    sz_t size = pow( ( double )max_alloc, rval * pow( 2.0, -32 ) );

                    sz_t new_size = 0;

                    data_table[ idx ] = alloc( data_table[ idx ], size_table[ idx ], size, &new_size ); // realloc

                    // verify old table content (only when size > sz - stdlib realloc does not seem to retain data otherwise)
                    if( size > size_table[ idx ] )
                    {
                        if( data_table[ idx ] != NULL && size_table[ idx ] > 0 )
                        {
                            u2_t rv = bcore_xsg_u2( idx + 1 );
                            uint8_t* data = data_table[ idx ];
                            sz_t sz = size_table[ idx ];
                            for( sz_t i = 0; i < sz; i++ )
                            {
                                rv = bcore_xsg_u2( rv );
                                if( data[ i ] != ( rv & 255 ) )
                                {
                                    ERR( "data failure [%u vs %u].", ( u2_t )data[ i ], ( u2_t )( rv & 255 ) );
                                }
                            }
                        }
                    }

                    size_table[ idx ] = new_size; //( data_table[ idx ] != NULL ) ? size : 0;
                    realloc_attempts++;
                    realloc_failures += ( size > 0 ) && ( data_table[ idx ] == NULL );

                    // set new content
                    if( size_table[ idx ] > 0 )
                    {
                        u2_t rv = bcore_xsg_u2( idx + 1 );
                        uint8_t* data = data_table[ idx ];
                        sz_t sz = size_table[ idx ];
                        for( sz_t i = 0; i < sz; i++ ) data[ i ] = ( ( rv = bcore_xsg_u2( rv ) ) & 255 );
                    }
                }
            }
        }
    }

    sz_t allocated_table_size = 0;
    for( sz_t i = 0; i < table_size; i++ ) allocated_table_size += ( data_table[ i ] != NULL );

    if( verbose )
    {
        bcore_string_s_pushf( str, "cycles ............... %lu\n", cycles );
        bcore_string_s_pushf( str, "max alloc ............ %lu\n", max_alloc );
        bcore_string_s_pushf( str, "Instances\n" );
        bcore_string_s_pushf( str, "  total .............. %lu\n", table_size );
        bcore_string_s_pushf( str, "  allocated .......... %lu\n", allocated_table_size );
        bcore_string_s_pushf( str, "Alloc\n" );
        bcore_string_s_pushf( str, "  attempts  .......... %lu\n", alloc_attempts );
        bcore_string_s_pushf( str, "  failures  .......... %lu\n", alloc_failures );
        bcore_string_s_pushf( str, "Realloc\n" );
        bcore_string_s_pushf( str, "  attempts  .......... %lu\n", realloc_attempts );
        bcore_string_s_pushf( str, "  failures  .......... %lu\n", realloc_failures );
        bcore_string_s_pushf( str, "Free\n" );
        bcore_string_s_pushf( str, "  attempts  .......... %lu\n", free_attempts );
        bcore_string_s_pushf( str, "  failures  .......... %lu\n", free_failures );
    }


    // speed test
    {
        clock_t time = clock();
        for( sz_t j = 0; j < cycles; j++ )
        {
            for( sz_t i = 0; i < table_size; i++ )
            {
                rval = bcore_xsg_u2( rval );
                sz_t idx = rval % table_size;
                if( data_table[ idx ] == NULL )
                {
                    sz_t size = pow( ( double )max_alloc, rval * pow( 2.0, -32 ) );
                    data_table[ idx ] = alloc( data_table[ idx ], 0, size, &size_table[ idx ] );
                }
                else
                {
                    data_table[ idx ] = alloc( data_table[ idx ], size_table[ idx ], 0, &size_table[ idx ] ); // free
                }
            }
        }
        time = clock() - time;
        bcore_string_s_pushf( str, "speed test alloc-free: %5.3gs\n", ( double )time / ( CLOCKS_PER_SEC  * cycles  * table_size ) );
    }
    // speed test
    {
        clock_t time = clock();
        for( sz_t j = 0; j < cycles; j++ )
        {
            for( sz_t i = 0; i < table_size; i++ )
            {
                rval = bcore_xsg_u2( rval );
                sz_t idx = rval % table_size;
                rval = bcore_xsg_u2( rval );
                sz_t size = pow( ( double )max_alloc, rval * pow( 2.0, -32 ) );
                data_table[ idx ] = alloc( data_table[ idx ], size_table[ idx ], size, &size_table[ idx ] ); // realloc
            }
        }
        time = clock() - time;
        bcore_string_s_pushf( str, "speed test realloc:    %5.3gs\n", ( double )time / ( CLOCKS_PER_SEC  * cycles  * table_size ) );
    }

    // cleanup
    if( cleanup ) for( sz_t i = 0; i < table_size; i++ ) data_table[ i ] = alloc( data_table[ i ], size_table[ i ], 0, NULL );

    free( size_table );
    free( data_table );
    return str;
}

static void memory_manager_s_quicktest( void )
{
    sz_t table_size = 200;
    sz_t cycles     = 1;
    sz_t max_alloc  = 4096;
    sz_t seed       = 5479;
    bcore_string_s_discard( bcore_memory_alloc_challenge( bcore_memory_manager_bn_alloc, table_size, cycles, max_alloc, seed, true, false ) );
}

static bcore_string_s* memory_manager_s_selftest( void )
{
    bcore_string_s* str = bcore_string_s_create();

    sz_t table_size = 100000;
    sz_t cycles     = 5;
    sz_t max_alloc  = 65536;
    sz_t seed       = 1237;

    {
        bcore_string_s_pushf( str, "\nbcore_internal_alloc:\n");
        bcore_string_s_push_string_d( str, bcore_memory_alloc_challenge( bcore_memory_manager_bn_alloc, table_size, cycles, max_alloc, seed, true, false ) );
        bcore_string_s_push_string_d( str, bcore_memory_manager_s_status( bcore_memory_manager_s_g, 1 ) );
//        bcore_string_s_push_string_d( str, bcore_btree_vd_s_status( m->internal_btree ) );

    }

    {
        bcore_string_s_pushf( str, "\nbcore_external_alloc:\n");
        bcore_string_s_push_string_d( str, bcore_memory_alloc_challenge( bcore_external_bn_alloc, table_size, cycles, max_alloc, seed, true, false ) );
    }

    return str;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_memory_manager_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_memory_manager" ) ) return NULL;
    if( signal == typeof( "init0" ) )
    {
        memory_manager_open();
    }
    else if( signal == typeof( "init1" ) )
    {
        memory_manager_s_quicktest();
    }
    else if( signal == typeof( "down0" ) )
    {
        sz_t space = bcore_memory_manager_granted_space();
        if( space > 0 ) ERR( "Leaking memory: %zu bytes", space );
        memory_manager_close();
    }
    else if( signal == typeof( "selftest" ) )
    {
        bcore_string_s_print_d( memory_manager_s_selftest() );
    }

    return NULL;
}

