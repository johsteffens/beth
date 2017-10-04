/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

/** Token Block Memory Manager.
 *  An adapted, simplified and stand-alone version of this memory manager has
 *  been released on https://github.com/johsteffens/tbman under a public license.
 */

#include "bcore_tbman.h"
#include "bcore_control.h"
#include "bcore_btree.h"
#include "bcore_string.h"
#include "bcore_threads.h"
#include "bcore_name_manager.h"
#include "bcore_quicktypes.h"
#include <time.h>
#include <math.h>

/**********************************************************************************************************************/
// default parameters

static const size_t default_pool_size       = 0x10000;
static const size_t default_min_block_size  = 8;
static const size_t default_max_block_size  = 1024 * 16;

static const size_t default_stepping_method = 1;
static const bool   default_full_align      = true;

/// Minimum alignment of memory blocks
#define TBMAN_ALIGN 0x100

/**********************************************************************************************************************/
// forward declarations

typedef struct block_manager_s block_manager_s;
typedef struct down_manager_s  down_manager_s;

/**********************************************************************************************************************/
// shutdown-management

/// shuts down an object (obj), o is the down structure
typedef void (*fp_down )(      vc_t o, vd_t obj );

/// first element of down structures below
typedef struct down_s { fp_down _; } down_s;

static void down_s_discard( down_manager_s* o, down_s* obj );

typedef struct down_obj_s
{
    down_s _; // pointer to _down_obj
    fp_down_obj fp;  // external down function
} down_obj_s;

static void _down_obj( const down_obj_s* o, vd_t obj ) { o->fp( obj ); }
static down_obj_s* down_obj_s_create( down_manager_s* o, fp_down_obj fp );

typedef struct down_arg_s
{
    down_s _; // pointer to _down_arg
    fp_down_arg fp; // external down function
    vc_t arg;  // argument for external function
} down_arg_s;

static void _down_arg( const down_arg_s* o, vd_t obj ) { o->fp( o->arg, obj ); }
static down_arg_s* down_arg_s_create( down_manager_s* o, fp_down_arg fp, vc_t arg );

typedef struct down_obj_arr_s
{
    down_s _; // internal down function
    fp_t fp;   // external down function
    sz_t size; // number of active array elements
    sz_t step; // stepping between array elements in bytes
} down_obj_arr_s;

static void _down_obj_arr( const down_obj_arr_s* o, vd_t obj )
{
    for( sz_t i = o->size; i > 0; i-- )
    {
        o->fp( obj );
        obj = ( u0_t* )obj + o->step;
    }
}

static down_obj_arr_s* down_obj_arr_s_create( down_manager_s* o, fp_down_obj fp, sz_t size, sz_t step );

typedef struct down_arg_arr_s
{
    down_s _; // internal down function
    fp_t fp;   // external down function
    vc_t arg;  // argument for external function
    sz_t size; // number of active array elements
    sz_t step; // stepping between array elements in bytes
} down_arg_arr_s;

static void _down_arg_arr( const down_arg_arr_s* o, vd_t obj )
{
    for( sz_t i = o->size; i > 0; i-- )
    {
        o->fp( o->arg, obj );
        obj = ( u0_t* )obj + o->step;
    }
}

static down_arg_arr_s* down_arg_arr_s_create( down_manager_s* o, fp_down_arg fp, vc_t arg, sz_t size, sz_t step );

/**********************************************************************************************************************/

vd_t bcore_tbman_external_b_alloc( vd_t current_ptr, sz_t requested_bytes, sz_t* granted_bytes )
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
        if( !current_ptr ) ERR( "Failed allocating %zu bytes", requested_bytes );
        if( granted_bytes ) *granted_bytes = requested_bytes;
    }
    return current_ptr;
}

vd_t bcore_tbman_external_bn_alloc( vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes )
{
    return bcore_tbman_external_b_alloc( current_ptr, requested_bytes, granted_bytes );
}

vd_t bcore_tbman_external_alloc( vd_t ptr, sz_t size )
{
    return bcore_tbman_external_b_alloc( ptr, size, NULL );
}

vd_t bcore_tbman_external_un_alloc( sz_t unit_bytes, vd_t current_ptr, sz_t current_units, sz_t requested_units, sz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_external_bn_alloc( current_ptr, unit_bytes * current_units, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

vd_t bcore_tbman_external_u_alloc( sz_t unit_bytes, vd_t current_ptr, sz_t requested_units, sz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_external_b_alloc( current_ptr, unit_bytes * requested_units, reserved_units );
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
    sz_t pool_size;   // size of pool in bytes
    sz_t block_size;  // size of block in bytes
    u1_t stack_size;  // size of token-stack ( == pool_size / block_size )
    u1_t stack_index; // index into token-stack

    /** aligned
     *  The memory-pool is considered aligned when the integer-evaluation of its address
     *  is a multiple of pool_size, which means that the pool address can be obtained
     *  from any pointer inside the pool by a mere integer division.
     */
    bl_t aligned;

    bcore_mutex_t* mutex;  // governing mutex
    block_manager_s* parent;
    sz_t parent_index;

    sz_t* rc_count_arr; // array of reference counters
    vd_t* rc_down_arr;  // array of shut down methods
    down_manager_s* down_manager;  // manager of down objects

    u1_t  token_stack[]; // stack of block-tokens (part of pool)
} token_manager_s;


static void token_manager_s_init( token_manager_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

static void token_manager_s_down( token_manager_s* o )
{
    if( o->rc_count_arr ) free( o->rc_count_arr );
    if( o->rc_down_arr  ) free( o->rc_down_arr  );
}

static token_manager_s* token_manager_s_create
    (
        sz_t pool_size,
        sz_t block_size,
        bl_t align,
        down_manager_s* down_manager,
        bcore_mutex_t* mutex
    )
{
    if( ( pool_size & ( pool_size - 1 ) ) != 0 ) ERR( "pool_size %zu is not a power of two", pool_size );
    sz_t stack_size = pool_size / block_size;
    if( stack_size > 0x10000 ) ERR( "stack_size %zu exceeds 0x10000", stack_size );
    sz_t reserved_size   = sizeof( token_manager_s ) + sizeof( u1_t ) * stack_size;
    sz_t reserved_blocks = reserved_size / block_size + ( ( reserved_size % block_size ) > 0 );
    if( stack_size < ( reserved_blocks + 1 ) ) ERR( "pool_size %zu is too small", pool_size );

    token_manager_s* o;
    if( align )
    {
        o = aligned_alloc( pool_size, pool_size );
        if( !o ) ERR( "Failed aligned allocating %zu bytes", pool_size );
    }
    else
    {
        o = aligned_alloc( TBMAN_ALIGN, pool_size );
        if( !o ) ERR( "Failed allocating %zu bytes", pool_size );
    }

    token_manager_s_init( o );
    o->aligned = ( ( intptr_t )o & ( intptr_t )( pool_size - 1 ) ) == 0;
    o->pool_size = pool_size;
    o->block_size = block_size;
    o->stack_size = stack_size;
    o->stack_index = 0;
    for( sz_t i = 0; i < o->stack_size; i++ ) o->token_stack[ i ] = ( i + reserved_blocks ) < stack_size ? ( i + reserved_blocks ) : 0;
    o->down_manager = down_manager;
    o->mutex = mutex;
    return o;
}

static void token_manager_s_discard( token_manager_s* o )
{
    if( !o ) return;
    token_manager_s_down( o );
    free( o );
}

static bl_t token_manager_s_is_full( token_manager_s* o )
{
    return o->token_stack[ o->stack_index ] == 0;
}

static bl_t token_manager_s_is_empty( token_manager_s* o )
{
    return o->stack_index == 0;
}

static vd_t token_manager_s_alloc( token_manager_s* o )
{
    assert( !token_manager_s_is_full( o ) );
    vd_t ret = ( u0_t* )o + o->token_stack[ o->stack_index ] * o->block_size;
    assert( ( u0_t* )ret >= ( u0_t* )o + sizeof( token_manager_s ) );
    o->stack_index++;
    return ret;
}

// forward declarations (implementation below)
static void block_manager_s_full_to_free( struct block_manager_s* o, token_manager_s* child );
static void block_manager_s_free_to_empty( struct block_manager_s* o, token_manager_s* child );

static void token_manager_s_free( token_manager_s* o, vd_t ptr )
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

    // reference control
    if( o->rc_count_arr )
    {
        if( o->rc_count_arr[ token ] )
        {
            o->rc_count_arr[ token ]--;
            return;
        }

        if( o->rc_down_arr )
        {
            vd_t down = o->rc_down_arr[ token ];
            if( down )
            {
                bcore_mutex_unlock( o->mutex );
                ( ( down_s* )down )->_( down, ( u0_t* )o + token * o->block_size );
                bcore_mutex_lock( o->mutex );
                assert( o->down_manager );
                down_s_discard( o->down_manager, down );
                o->rc_down_arr[ token ] = NULL;
            }
        }
    }

    if( o->token_stack[ o->stack_index ] == 0 ) block_manager_s_full_to_free( o->parent, o );
    o->stack_index--;
    o->token_stack[ o->stack_index ] = token;
    if( o->stack_index == 0 ) block_manager_s_free_to_empty( o->parent, o );
}

static void token_manager_s_create_rc_count_arr( token_manager_s* o )
{
    sz_t alloc_size = sizeof( sz_t ) * o->stack_size;
    o->rc_count_arr = malloc( alloc_size );
    memset( o->rc_count_arr, 0, alloc_size );
}

static void token_manager_s_create_rc_down_arr( token_manager_s* o )
{
    sz_t alloc_size = sizeof( vd_t ) * o->stack_size;
    o->rc_down_arr = malloc( alloc_size );
    memset( o->rc_down_arr, 0, alloc_size );
}

static void token_manager_s_fork( token_manager_s* o, vd_t ptr )
{
    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;
    if( !o->rc_count_arr ) token_manager_s_create_rc_count_arr( o );
    o->rc_count_arr[ token ]++;
}

static void token_manager_s_release( token_manager_s* o, vd_t ptr )
{
    token_manager_s_free( o, ptr );
}

static void token_manager_s_release_obj( token_manager_s* o, fp_down_obj down, vd_t ptr )
{
    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;
    if( !o->rc_count_arr )
    {
        bcore_mutex_unlock( o->mutex );
        down( ptr );
        bcore_mutex_lock( o->mutex );
        token_manager_s_free( o, ptr );
        return;
    }

    if( o->rc_count_arr[ token ] )
    {
        o->rc_count_arr[ token ]--;
        if( !o->rc_down_arr ) token_manager_s_create_rc_down_arr( o );
        if( o->rc_down_arr[ token ] ) ERR( "Shutdown function has already been defined." );
        assert( o->down_manager );
        o->rc_down_arr[ token ] = down_obj_s_create( o->down_manager, down );
        return;
    }

    if( o->rc_down_arr && o->rc_down_arr[ token ] ) ERR( "Shutdown function has already been defined." );
    bcore_mutex_unlock( o->mutex );
    down( ptr );
    bcore_mutex_lock( o->mutex );
    token_manager_s_free( o, ptr );
}

static void token_manager_s_release_arg( token_manager_s* o, fp_down_arg down, vc_t arg, vd_t ptr )
{
    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;
    if( !o->rc_count_arr )
    {
        bcore_mutex_unlock( o->mutex );
        down( arg, ptr );
        bcore_mutex_lock( o->mutex );
        token_manager_s_free( o, ptr );
        return;
    }

    if( o->rc_count_arr[ token ] )
    {
        o->rc_count_arr[ token ]--;
        if( !o->rc_down_arr ) token_manager_s_create_rc_down_arr( o );
        if( o->rc_down_arr[ token ] ) ERR( "Shutdown function has already been defined." );
        assert( o->down_manager );
        o->rc_down_arr[ token ] = down_arg_s_create( o->down_manager, down, arg );
        return;
    }

    if( o->rc_down_arr && o->rc_down_arr[ token ] ) ERR( "Shutdown function has already been defined." );
    bcore_mutex_unlock( o->mutex );
    down( arg, ptr );
    bcore_mutex_lock( o->mutex );
    token_manager_s_free( o, ptr );
}

static void token_manager_s_release_obj_arr( token_manager_s* o, fp_down_obj down, vd_t ptr, sz_t size, sz_t step )
{
    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;
    if( !o->rc_count_arr )
    {
        bcore_mutex_unlock( o->mutex );
        for( sz_t i = 0; i < size; i++ ) down( ( u0_t* )ptr + i * step );
        bcore_mutex_lock( o->mutex );
        token_manager_s_free( o, ptr );
        return;
    }

    if( o->rc_count_arr[ token ] )
    {
        o->rc_count_arr[ token ]--;
        if( !o->rc_down_arr ) token_manager_s_create_rc_down_arr( o );
        if( o->rc_down_arr[ token ] ) ERR( "Shutdown function has already been defined." );
        assert( o->down_manager );
        o->rc_down_arr[ token ] = down_obj_arr_s_create( o->down_manager, down, size, step );
        return;
    }

    if( o->rc_down_arr && o->rc_down_arr[ token ] ) ERR( "Shutdown function has already been defined." );
    bcore_mutex_unlock( o->mutex );
    for( sz_t i = 0; i < size; i++ ) down( ( u0_t* )ptr + i * step );
    bcore_mutex_lock( o->mutex );
    token_manager_s_free( o, ptr );
}

static void token_manager_s_release_arg_arr( token_manager_s* o, fp_down_arg down, vc_t arg, vd_t ptr, sz_t size, sz_t step )
{
    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;
    if( !o->rc_count_arr )
    {
        bcore_mutex_unlock( o->mutex );
        for( sz_t i = 0; i < size; i++ ) down( arg, ( u0_t* )ptr + i * step );
        bcore_mutex_lock( o->mutex );
        token_manager_s_free( o, ptr );
        return;
    }

    if( o->rc_count_arr[ token ] )
    {
        o->rc_count_arr[ token ]--;
        if( !o->rc_down_arr ) token_manager_s_create_rc_down_arr( o );
        if( o->rc_down_arr[ token ] ) ERR( "Shutdown function has already been defined." );
        assert( o->down_manager );
        o->rc_down_arr[ token ] = down_arg_arr_s_create( o->down_manager, down, arg, size, step );
        return;
    }

    if( o->rc_down_arr && o->rc_down_arr[ token ] ) ERR( "Shutdown function has already been defined." );
    bcore_mutex_unlock( o->mutex );
    for( sz_t i = 0; i < size; i++ ) down( arg, ( u0_t* )ptr + i * step );
    bcore_mutex_lock( o->mutex );
    token_manager_s_free( o, ptr );
}

static sz_t token_manager_s_total_alloc( const token_manager_s* o )
{
    return o->block_size * o->stack_index;
}

static sz_t token_manager_s_instances( const token_manager_s* o )
{
    return o->stack_index;
}

static sz_t token_manager_s_references( const token_manager_s* o )
{
    sz_t count = 0;
    for( sz_t i = 0; i < o->stack_index; i++ )
    {
        sz_t token = o->token_stack[ i ];
        count++;
        if( o->rc_count_arr ) count += o->rc_count_arr[ token ];
    }
    return count;
}

static sz_t token_manager_s_total_space( const token_manager_s* o )
{
    return o->pool_size + o->stack_size * sizeof( u1_t );
}

static bcore_string_s* token_manager_s_status( const token_manager_s* o, int detail_level )
{
    bcore_string_s* str = bcore_string_s_create();
    if( detail_level <= 0 ) return str;
    bcore_string_s_pushf( str, "    pool_size:   %zu\n",  o->pool_size );
    bcore_string_s_pushf( str, "    block_size:  %zu\n",  o->block_size );
    bcore_string_s_pushf( str, "    stack_size:  %zu\n",  o->stack_size );
    bcore_string_s_pushf( str, "    aligned:     %s\n",   o->aligned ? "true" : "false" );
    bcore_string_s_pushf( str, "    stack_index: %zu\n",  ( sz_t )o->stack_index );
    bcore_string_s_pushf( str, "    total alloc: %zu\n", token_manager_s_total_alloc( o ) );
    bcore_string_s_pushf( str, "    total space: %zu\n", token_manager_s_total_space( o ) );
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
    bl_t align;      // attempt to align token_managers to pool_size
    token_manager_s** data;
    sz_t size, space;
    sz_t free_index;       // entries equal or above free_index have space for allocation
    f3_t sweep_hysteresis; // if ( empty token-managers ) / ( used token-managers ) < sweep_hysteresis, empty token-managers are discarded
    bl_t aligned;          // all token managers are aligned to pool_size
    vd_t parent;           // parent
    fp_t fp_lost_alignment; // callback for lost alignment
    bcore_btree_vd_s* internal_btree;
    down_manager_s* down_manager;
    bcore_mutex_t* mutex;  // governing mutex
} block_manager_s;

static void block_manager_s_init
    (
        block_manager_s* o,
        vd_t parent,
        sz_t pool_size,
        sz_t block_size,
        bl_t align,
        fp_t fp_lost_alignment,
        bcore_btree_vd_s* internal_btree,
        down_manager_s* down_manager,
        bcore_mutex_t* mutex
    )
{
    bcore_memzero( o, sizeof( *o ) );
    o->parent = parent;
    o->aligned = true;
    o->sweep_hysteresis = 0.125;
    o->pool_size = pool_size;
    o->block_size = block_size;
    o->align = align;
    o->fp_lost_alignment = fp_lost_alignment;
    o->internal_btree = internal_btree;
    o->down_manager = down_manager;
    o->mutex = mutex;
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

static block_manager_s* block_manager_s_create
    (
        vd_t parent,
        sz_t pool_size,
        sz_t block_size,
        bl_t align,
        fp_t fp_lost_alignment,
        bcore_btree_vd_s* internal_btree,
        down_manager_s* down_manager,
        bcore_mutex_t* mutex
    )
{
    block_manager_s* o = malloc( sizeof( block_manager_s ) );
    if( !o ) ERR( "Failed allocating %zu bytes", sizeof( block_manager_s ) );
    block_manager_s_init( o, parent, pool_size, block_size, align, fp_lost_alignment, internal_btree, down_manager, mutex );
    return o;
}

static void block_manager_s_discard( block_manager_s* o )
{
    if( !o ) return;
    block_manager_s_down( o );
    free( o );
}

static void block_manager_s_acquire_token_manager( block_manager_s* o )
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

        if( !o->data ) ERR( "Failed allocating %zu bytes", sizeof( token_manager_s* ) * o->space );
    }
    o->data[ o->size ] = token_manager_s_create( o->pool_size, o->block_size, o->align, o->down_manager, o->mutex );
    o->data[ o->size ]->parent_index = o->size;
    o->data[ o->size ]->parent = o;
    if( o->aligned && !o->data[ o->size ]->aligned )
    {
        o->aligned = false;
        if( o->fp_lost_alignment ) o->fp_lost_alignment( o->parent );
    }
    if( bcore_btree_vd_s_set( o->internal_btree, o->data[ o->size ] ) != 1 ) ERR( "Failed registering block address." );
    o->size++;
}

static vd_t block_manager_s_alloc( block_manager_s* o )
{
    if( o->free_index == o->size ) block_manager_s_acquire_token_manager( o );
    token_manager_s* child = o->data[ o->free_index ];
    vd_t ret = token_manager_s_alloc( child );
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

static sz_t block_manager_s_instances( const block_manager_s* o )
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        sum += token_manager_s_instances( o->data[ i ] );
    }
    return sum;
}

static sz_t block_manager_s_references( const block_manager_s* o )
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        sum += token_manager_s_references( o->data[ i ] );
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
    bcore_string_s_pushf( str, "  pool_size:        %zu\n", o->pool_size );
    bcore_string_s_pushf( str, "  block_size:       %zu\n", o->block_size );
    bcore_string_s_pushf( str, "  sweep_hysteresis: %g\n",  o->sweep_hysteresis );
    bcore_string_s_pushf( str, "  aligned:          %s\n",  o->aligned ? "true" : "false" );
    bcore_string_s_pushf( str, "  token_managers:   %zu\n", o->size );
    bcore_string_s_pushf( str, "      full:         %zu\n", o->free_index );
    bcore_string_s_pushf( str, "      empty:        %zu\n", block_manager_s_empty_tail( o ) );
    bcore_string_s_pushf( str, "  total alloc:      %zu\n", block_manager_s_total_alloc( o ) );
    bcore_string_s_pushf( str, "  total space:      %zu\n", block_manager_s_total_space( o ) );
    if( detail_level > 1 )
    {
        for( sz_t i = 0; i < o->size; i++ )
        {
            bcore_string_s_pushf( str, "\nblock manager %zu:\n", i );
            bcore_string_s_push_string_d( str, token_manager_s_status( o->data[ i ], detail_level - 1 ) );
        }
    }
    return str;
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/** External-Control
 *  Control framework for external storage
 */

typedef struct ext_s // external item
{
    sz_t size;     // allocated size
    sz_t rc_count; // reference counters
    vd_t rc_down;  // shut down method
} ext_s;

static void ext_s_init( ext_s* o ) { bcore_memzero( o, sizeof( ext_s ) ); }
static void ext_s_down( ext_s* o ) {}

typedef struct external_manager_s
{
    block_manager_s   bm_ext;
    bcore_btree_vd_s* tm_btree; // btree of token managers
    bcore_btree_pp_s* ex_btree; // btree of external references
    sz_t              pool_size;
    bl_t              aligned;
    down_manager_s*   down_manager;  // manager of down objects
    bcore_mutex_t*    mutex;    // governing mutex
} external_manager_s;


static void external_manager_s_lost_alignment( external_manager_s* o )
{
    o->aligned = false;
}

static void external_manager_s_init
    (
        external_manager_s* o,
        sz_t pool_size,
        bl_t full_align,
        down_manager_s* down_manager,
        bcore_mutex_t* mutex
    )
{
    bcore_memzero( o, sizeof( *o ) );
    o->pool_size = pool_size;
    o->tm_btree = bcore_btree_vd_s_create( bcore_tbman_external_alloc );
    o->ex_btree = bcore_btree_pp_s_create( bcore_tbman_external_alloc );
    block_manager_s_init
        (
            &o->bm_ext,
            o,
            pool_size,
            sizeof( ext_s ),
            full_align,
            external_manager_s_lost_alignment,
            o->tm_btree,
            NULL,
            mutex
        );
    o->aligned = true;
    o->down_manager = down_manager;
    o->mutex = mutex;
}

static void external_manager_s_down( external_manager_s* o )
{
    block_manager_s_down( &o->bm_ext );
    bcore_btree_vd_s_discard( o->tm_btree );
    bcore_btree_pp_s_discard( o->ex_btree );
}

static ext_s* ext_s_create( external_manager_s* o )
{
    ext_s* ext = block_manager_s_alloc( &o->bm_ext );
    ext_s_init( ext );
    return ext;
}

static void ext_s_discard( external_manager_s* o, ext_s* ext )
{
    ext_s_down( ext );
    if( o->aligned )
    {
        token_manager_s* token_manager = ( token_manager_s* )( ( intptr_t )ext & ~( intptr_t )( o->pool_size - 1 ) );
        token_manager_s_free( token_manager, ext );
    }
    else
    {
        token_manager_s_free( bcore_btree_vd_s_largest_below_equal( o->tm_btree, ext ), ext );
    }
}

static vd_t external_manager_s_malloc( external_manager_s* o, sz_t requested_bytes, sz_t* granted_bytes )
{
    vd_t ptr = aligned_alloc( TBMAN_ALIGN, requested_bytes );
    if( !ptr ) ERR( "Failed allocating %zu bytes.", requested_bytes );
    if( granted_bytes ) *granted_bytes = requested_bytes;
    ext_s* ext = ext_s_create( o );
    ext->size = requested_bytes;
    if( bcore_btree_pp_s_set( o->ex_btree, ptr, ext ) != 1 ) ERR( "Registering new address failed" );
    return ptr;
}

static void external_manager_s_free( external_manager_s* o, vd_t ptr )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Attempt to free invalid memory" );
    ext_s* ext = *ext_p;

    // reference control
    if( ext->rc_count )
    {
        ext->rc_count--;
        return;
    }

    if( ext->rc_down )
    {
        down_s* down = ext->rc_down;
        bcore_mutex_unlock( o->mutex );
        down->_( down, ptr );
        bcore_mutex_lock( o->mutex );
        assert( o->down_manager );
        down_s_discard( o->down_manager, down );
        ext->rc_down = NULL;
    }

    bcore_btree_pp_s_remove( o->ex_btree, ptr );
    ext_s_discard( o, ext );
    free( ptr );
}

static sz_t external_manager_s_granted_bytes( external_manager_s* o, vd_t ptr )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Could not retrieve current external memory" );
    return ( ( ext_s* )*ext_p )->size;
}

static void ext_sum( vd_t val, bcore_btree_pp_kv_s kv ) { *(sz_t*)val += ( ( ext_s* )kv.val )->size; }

static sz_t external_manager_s_total_alloc( const external_manager_s* o )
{
    sz_t size = 0;
    bcore_btree_pp_s_run( o->ex_btree, ext_sum, &size );
    return size;
}

static void ext_count( vd_t val, bcore_btree_pp_kv_s kv ) { *(sz_t*)val += 1; }

static sz_t external_manager_s_instances( const external_manager_s* o )
{
    sz_t size = 0;
    bcore_btree_pp_s_run( o->ex_btree, ext_count, &size );
    return size;
}

static void ext_rc_ref_count( vd_t val, bcore_btree_pp_kv_s kv )
{
    *(sz_t*)val += 1 + ( ( ext_s* )kv.val )->rc_count;
}

static sz_t external_manager_s_references( const external_manager_s* o )
{
    sz_t size = 0;
    bcore_btree_pp_s_run( o->ex_btree, ext_rc_ref_count, &size );
    return size;
}

static void external_manager_s_fork( external_manager_s* o, vd_t ptr )
{
    bcore_btree_pp_kv_s* kv = bcore_btree_pp_s_largest_below_equal( o->ex_btree, ptr );
    if( !kv ) ERR( "Object has no root." );
    ext_s* ext = kv->val;
    if( ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )kv->key ) ) > ext->size ) ERR( "Object has no root." );
    ext->rc_count++;
}

static void external_manager_s_release( external_manager_s* o, vd_t ptr )
{
    bcore_btree_pp_kv_s* kv = bcore_btree_pp_s_largest_below_equal( o->ex_btree, ptr );
    if( !kv ) ERR( "Object has no root." );
    ext_s* ext = kv->val;
    if( ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )kv->key ) ) > ext->size ) ERR( "Object has no root." );

    external_manager_s_free( o, kv->key );
}

static void external_manager_s_release_obj( external_manager_s* o, fp_down_obj down, vd_t ptr )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Object is not root." );
    ext_s* ext = *ext_p;
    if( ext->rc_down ) ERR( "Shutdown function has already been defined." );
    if( ext->rc_count )
    {
        ext->rc_count--;
        assert( o->down_manager );
        ext->rc_down = down_obj_s_create( o->down_manager, down );
    }
    else
    {
        bcore_mutex_unlock( o->mutex );
        down( ptr );
        bcore_mutex_lock( o->mutex );
        external_manager_s_free( o, ptr );
    }
}

static void external_manager_s_release_arg( external_manager_s* o, fp_down_arg down, vc_t arg, vd_t ptr )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Object is not root." );
    ext_s* ext = *ext_p;
    if( ext->rc_down ) ERR( "Shutdown function has already been defined." );
    if( ext->rc_count )
    {
        ext->rc_count--;
        assert( o->down_manager );
        ext->rc_down = down_arg_s_create( o->down_manager, down, arg );
    }
    else
    {
        bcore_mutex_unlock( o->mutex );
        down( arg, ptr );
        bcore_mutex_lock( o->mutex );
        external_manager_s_free( o, ptr );
    }
}

static void external_manager_s_release_obj_arr( external_manager_s* o, fp_down_obj down, vd_t ptr, sz_t size, sz_t step )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Object is not root." );
    ext_s* ext = *ext_p;
    if( ext->rc_down ) ERR( "Shutdown function has already been defined." );
    if( ext->rc_count )
    {
        ext->rc_count--;
        assert( o->down_manager );
        ext->rc_down = down_obj_arr_s_create( o->down_manager, down, size, step );
    }
    else
    {
        bcore_mutex_unlock( o->mutex );
        for( sz_t i = 0; i < size; i++ ) down( ( u0_t* )ptr + i * step );
        bcore_mutex_lock( o->mutex );
        external_manager_s_free( o, ptr );
    }
}

static void external_manager_s_release_arg_arr( external_manager_s* o, fp_down_arg down, vc_t arg, vd_t ptr, sz_t size, sz_t step )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Object is not root." );
    ext_s* ext = *ext_p;
    if( ext->rc_down ) ERR( "Shutdown function has already been defined." );
    if( ext->rc_count )
    {
        ext->rc_count--;
        assert( o->down_manager );
        ext->rc_down = down_arg_arr_s_create( o->down_manager, down, arg, size, step );
    }
    else
    {
        bcore_mutex_unlock( o->mutex );
        for( sz_t i = 0; i < size; i++ ) down( arg, ( u0_t* )ptr + i * step );
        bcore_mutex_lock( o->mutex );
        external_manager_s_free( o, ptr );
    }
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/// Manager of down-objects

typedef struct down_manager_s
{
    block_manager_s   bm_down_obj;
    block_manager_s   bm_down_arg;
    block_manager_s   bm_down_obj_arr;
    block_manager_s   bm_down_arg_arr;
    bcore_btree_vd_s* tm_btree; // btree of token managers
    sz_t              pool_size;
    bl_t              aligned;
    bcore_mutex_t*    mutex;    // governing mutex
} down_manager_s;

static void down_manager_s_lost_alignment( external_manager_s* o )
{
    o->aligned = false;
}

static void down_manager_s_init( down_manager_s* o, sz_t pool_size, bl_t full_align, bcore_mutex_t* mutex )
{
    bcore_memzero( o, sizeof( *o ) );
    o->pool_size = pool_size;
    o->tm_btree = bcore_btree_vd_s_create( bcore_tbman_external_alloc );

    block_manager_s_init
    (
        &o->bm_down_obj,
        o,
        pool_size,
        sizeof( down_obj_s ),
        full_align,
        down_manager_s_lost_alignment,
        o->tm_btree,
        NULL,
        mutex
    );

    block_manager_s_init
    (
        &o->bm_down_arg,
        o,
        pool_size,
        sizeof( down_arg_s ),
        full_align,
        down_manager_s_lost_alignment,
        o->tm_btree,
        NULL,
        mutex
    );

    block_manager_s_init
    (
        &o->bm_down_obj_arr,
        o,
        pool_size,
        sizeof( down_obj_arr_s ),
        full_align,
        down_manager_s_lost_alignment,
        o->tm_btree,
        NULL,
        mutex
    );

    block_manager_s_init
    (
        &o->bm_down_arg_arr,
        o,
        pool_size,
        sizeof( down_arg_arr_s ),
        full_align,
        down_manager_s_lost_alignment,
        o->tm_btree,
        NULL,
        mutex
    );

    o->aligned = true;
}

static void down_manager_s_down( down_manager_s* o )
{
    block_manager_s_down( &o->bm_down_obj );
    block_manager_s_down( &o->bm_down_arg );
    block_manager_s_down( &o->bm_down_obj_arr );
    block_manager_s_down( &o->bm_down_arg_arr );
    bcore_btree_vd_s_discard( o->tm_btree );
}

static void down_s_discard( down_manager_s* o, down_s* down )
{
    if( o->aligned )
    {
        token_manager_s* token_manager = ( token_manager_s* )( ( intptr_t )down & ~( intptr_t )( o->pool_size - 1 ) );
        token_manager_s_free( token_manager, down );
    }
    else
    {
        token_manager_s_free( bcore_btree_vd_s_largest_below_equal( o->tm_btree, down ), down );
    }
}

static down_obj_s* down_obj_s_create( down_manager_s* o, fp_down_obj fp )
{
    down_obj_s* down = block_manager_s_alloc( &o->bm_down_obj );
    down->_._ = ( fp_down )_down_obj;
    down->fp = fp;
    return down;
}

static down_arg_s* down_arg_s_create( down_manager_s* o, fp_down_arg fp, vc_t arg )
{
    down_arg_s* down = block_manager_s_alloc( &o->bm_down_arg );
    down->_._ = ( fp_down )_down_arg;
    down->fp = fp;
    down->arg = arg;
    return down;
}

static down_obj_arr_s* down_obj_arr_s_create( down_manager_s* o, fp_down_obj fp, sz_t size, sz_t step )
{
    down_obj_arr_s* down = block_manager_s_alloc( &o->bm_down_obj_arr );
    down->_._ = ( fp_down )_down_obj_arr;
    down->fp = fp;
    down->size = size;
    down->step = step;
    return down;
}

static down_arg_arr_s* down_arg_arr_s_create( down_manager_s* o, fp_down_arg fp, vc_t arg, sz_t size, sz_t step )
{
    down_arg_arr_s* down = block_manager_s_alloc( &o->bm_down_arg_arr );
    down->_._ = ( fp_down )_down_arg_arr;
    down->fp = fp;
    down->arg = arg;
    down->size = size;
    down->step = step;
    return down;

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
typedef struct bcore_tbman_s
{
    block_manager_s** data; // block managers are sorted by increasing block size
    sz_t size;
    sz_t pool_size;               // pool size for all token managers
    sz_t min_block_size;
    sz_t max_block_size;
    bl_t aligned;                 // all token managers are aligned
    sz_t* block_size_array;       // copy of block size values (for fast access)
    bcore_btree_vd_s* internal_btree;
    external_manager_s  external_manager;
    down_manager_s down_manager;
    bcore_mutex_t mutex;
} bcore_tbman_s;

static inline void tbman_s_lock( bcore_tbman_s* o )
{
    bcore_mutex_lock( &o->mutex );
}

static inline void tbman_s_unlock( bcore_tbman_s* o )
{
    bcore_mutex_unlock( &o->mutex );
}

static void tbman_s_lost_alignment( struct bcore_tbman_s* o )
{
    o->aligned = false;
}

void bcore_tbman_s_init( bcore_tbman_s* o, sz_t pool_size, sz_t min_block_size, sz_t max_block_size, sz_t stepping_method, bl_t full_align )
{
    bcore_memzero( o, sizeof( *o ) );
    bcore_mutex_init( &o->mutex );

    o->internal_btree = bcore_btree_vd_s_create( bcore_tbman_external_alloc );

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

            if( !o->data ) ERR( "Failed allocating %zu bytes", sizeof( block_manager_s* ) * space );
        }

        o->data[ o->size ] = block_manager_s_create
            (
                o,
                o->pool_size,
                block_size,
                full_align,
                tbman_s_lost_alignment,
                o->internal_btree,
                &o->down_manager,
                &o->mutex
            );

        o->size++;

        if( block_size > size_mask )
        {
            size_mask <<= 1;
            size_inc  <<= 1;
        }
    }

    o->block_size_array = malloc( o->size * sizeof( sz_t ) );
    if( !o->block_size_array ) ERR( "Failed allocating %zu bytes", o->size * sizeof( sz_t ) );

    o->aligned = true;
    for( sz_t i = 0; i < o->size; i++ )
    {
        o->aligned = o->aligned && o->data[ i ]->aligned;
        o->block_size_array[ i ] = o->data[ i ]->block_size;
    }

    external_manager_s_init( &o->external_manager, pool_size, full_align, &o->down_manager, &o->mutex );
    down_manager_s_init( &o->down_manager, pool_size, full_align, &o->mutex );
}

void bcore_tbman_s_down( bcore_tbman_s* o )
{
    tbman_s_lock( o );
    external_manager_s_down( &o->external_manager );
    down_manager_s_down( &o->down_manager );

    if( o->data )
    {
        for( sz_t i = 0; i < o->size; i++ ) block_manager_s_discard( o->data[ i ] );
        free( o->data );
    }

    bcore_btree_vd_s_discard( o->internal_btree );

    tbman_s_unlock( o );
    bcore_mutex_down( &o->mutex );
}

bcore_tbman_s* bcore_tbman_s_create( sz_t pool_size, sz_t min_block_size, sz_t max_block_size, sz_t stepping_method, bl_t full_align )
{
    bcore_tbman_s* o = malloc( sizeof( bcore_tbman_s ) );
    if( !o ) ERR( "Failed allocating %zu bytes", sizeof( bcore_tbman_s ) );
    bcore_tbman_s_init( o, pool_size, min_block_size, max_block_size, stepping_method, full_align );
    return o;
}

void bcore_tbman_s_discard( bcore_tbman_s* o )
{
    if( !o ) return;
    bcore_tbman_s_down( o );
    free( o );
}

static vd_t tbman_s_malloc( bcore_tbman_s* o, sz_t requested_bytes, sz_t* granted_bytes )
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

    vd_t reserved_ptr = NULL;
    if( block_manager )
    {
        reserved_ptr = block_manager_s_alloc( block_manager );
        if( granted_bytes ) *granted_bytes = block_manager->block_size;
    }
    else
    {
        reserved_ptr = external_manager_s_malloc( &o->external_manager, requested_bytes, granted_bytes );
    }

    return reserved_ptr;
}

static void tbman_s_free( bcore_tbman_s* o, vd_t current_ptr, const sz_t* current_bytes )
{
    if( current_bytes && *current_bytes <= o->max_block_size && o->aligned )
    {
        token_manager_s* token_manager = ( token_manager_s* )( ( intptr_t )current_ptr & ~( intptr_t )( o->pool_size - 1 ) );
        token_manager_s_free( token_manager, current_ptr );
    }
    else
    {
        vd_t block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, current_ptr );
        if( block_ptr && ( ( ptrdiff_t )( (u0_t*)current_ptr - (u0_t*)block_ptr ) < o->pool_size ) )
        {
            token_manager_s_free( block_ptr, current_ptr );
        }
        else
        {
            external_manager_s_free( &o->external_manager, current_ptr );
        }
    }
}

static void tbman_s_fork( bcore_tbman_s* o, vd_t ptr )
{
    vd_t block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, ptr );
    if( block_ptr && ( ( ptrdiff_t )( (u0_t*)ptr - (u0_t*)block_ptr ) < o->pool_size ) )
    {
        token_manager_s_fork( block_ptr, ptr );
    }
    else
    {
        external_manager_s_fork( &o->external_manager, ptr );
    }
}

static void tbman_s_release( bcore_tbman_s* o, vd_t ptr )
{
    vd_t block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, ptr );
    if( block_ptr && ( ( ptrdiff_t )( (u0_t*)ptr - (u0_t*)block_ptr ) < o->pool_size ) )
    {
        token_manager_s_release( block_ptr, ptr );
    }
    else
    {
        external_manager_s_release( &o->external_manager, ptr );
    }
}

static void tbman_s_release_obj( bcore_tbman_s* o, fp_down_obj down, vd_t ptr )
{
    if( down )
    {
        vd_t block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, ptr );
        if( block_ptr && ( ( ptrdiff_t )( (u0_t*)ptr - (u0_t*)block_ptr ) < o->pool_size ) )
        {
            token_manager_s_release_obj( block_ptr, down, ptr );
        }
        else
        {
            external_manager_s_release_obj( &o->external_manager, down, ptr );
        }
    }
    else
    {
        tbman_s_release( o, ptr );
    }
}

static void tbman_s_release_arg( bcore_tbman_s* o, fp_down_arg down, vc_t arg, vd_t ptr )
{
    if( down )
    {
        vd_t block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, ptr );
        if( block_ptr && ( ( ptrdiff_t )( (u0_t*)ptr - (u0_t*)block_ptr ) < o->pool_size ) )
        {
            token_manager_s_release_arg( block_ptr, down, arg, ptr );
        }
        else
        {
            external_manager_s_release_arg( &o->external_manager, down, arg, ptr );
        }
    }
    else
    {
        tbman_s_release( o, ptr );
    }
}

static void tbman_s_release_obj_arr( bcore_tbman_s* o, fp_down_obj down, vd_t ptr, sz_t size, sz_t step )
{
    if( down )
    {
        vd_t block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, ptr );
        if( block_ptr && ( ( ptrdiff_t )( (u0_t*)ptr - (u0_t*)block_ptr ) < o->pool_size ) )
        {
            token_manager_s_release_obj_arr( block_ptr, down, ptr, size, step );
        }
        else
        {
            external_manager_s_release_obj_arr( &o->external_manager, down, ptr, size, step );
        }
    }
    else
    {
        tbman_s_release( o, ptr );
    }
}

static void tbman_s_release_arg_arr( bcore_tbman_s* o, fp_down_arg down, vc_t arg, vd_t ptr, sz_t size, sz_t step )
{
    if( down )
    {
        vd_t block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, ptr );
        if( block_ptr && ( ( ptrdiff_t )( (u0_t*)ptr - (u0_t*)block_ptr ) < o->pool_size ) )
        {
            token_manager_s_release_arg_arr( block_ptr, down, arg, ptr, size, step );
        }
        else
        {
            external_manager_s_release_arg_arr( &o->external_manager, down, arg, ptr, size, step );
        }
    }
    else
    {
        tbman_s_release( o, ptr );
    }
}

static vd_t tbman_s_realloc( bcore_tbman_s* o, vd_t current_ptr, const sz_t* current_bytes, sz_t requested_bytes, sz_t* granted_bytes )
{
    token_manager_s* token_manager = NULL;
    if( current_bytes && *current_bytes <= o->max_block_size && o->aligned )
    {
        token_manager = ( token_manager_s* )( ( intptr_t )current_ptr & ~( intptr_t )( o->pool_size - 1 ) );
    }
    else
    {
        vd_t block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, current_ptr );
        if( block_ptr && ( ( (u0_t*)current_ptr - (u0_t*)block_ptr ) < o->pool_size ) ) token_manager = block_ptr;
    }

    if( token_manager )
    {
        if( requested_bytes > token_manager->block_size )
        {
            vd_t reserved_ptr = tbman_s_malloc( o, requested_bytes, granted_bytes );
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
                vd_t reserved_ptr = block_manager_s_alloc( block_manager );
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
            vd_t reserved_ptr = tbman_s_malloc( o, requested_bytes, granted_bytes );
            bcore_memcpy( reserved_ptr, current_ptr, requested_bytes );
            external_manager_s_free( &o->external_manager, current_ptr );
            return reserved_ptr;
        }
        else // neither old nor new size handled by this manager
        {

            sz_t current_ext_bytes = external_manager_s_granted_bytes( &o->external_manager, current_ptr );

            // if requested bytes is less but not significantly less than current bytes, keep current memory
            if( ( requested_bytes < current_ext_bytes ) && ( requested_bytes >= ( current_ext_bytes >> 1 ) ) )
            {
                if( granted_bytes ) *granted_bytes = current_ext_bytes;
                return current_ptr;
            }

            vd_t reserved_ptr = external_manager_s_malloc( &o->external_manager, requested_bytes, granted_bytes );
            sz_t copy_bytes = ( requested_bytes < current_ext_bytes ) ? requested_bytes : current_ext_bytes;
            bcore_memcpy( reserved_ptr, current_ptr, copy_bytes );
            external_manager_s_free( &o->external_manager, current_ptr );
            return reserved_ptr;
        }
    }
}

vd_t bcore_tbman_s_fork( bcore_tbman_s* o, vd_t ptr )
{
    tbman_s_lock( o );
    tbman_s_fork( o, ptr );
    tbman_s_unlock( o );
    return ptr;
}

void bcore_tbman_s_release( bcore_tbman_s* o, vd_t ptr )
{
    tbman_s_lock( o );
    tbman_s_release( o, ptr );
    tbman_s_unlock( o );
}

void bcore_tbman_s_release_obj( bcore_tbman_s* o, fp_down_obj down, vd_t ptr )
{
    tbman_s_lock( o );
    tbman_s_release_obj( o, down, ptr );
    tbman_s_unlock( o );
}

void bcore_tbman_s_release_arg( bcore_tbman_s* o, fp_down_arg down, vc_t arg, vd_t ptr )
{
    tbman_s_lock( o );
    tbman_s_release_arg( o, down, arg, ptr );
    tbman_s_unlock( o );
}

void bcore_tbman_s_release_obj_arr( bcore_tbman_s* o, fp_down_obj down, vd_t ptr, sz_t size, sz_t step )
{
    tbman_s_lock( o );
    tbman_s_release_obj_arr( o, down, ptr, size, step );
    tbman_s_unlock( o );
}

void bcore_tbman_s_release_arg_arr( bcore_tbman_s* o, fp_down_arg down, vc_t arg, vd_t ptr, sz_t size, sz_t step )
{
    tbman_s_lock( o );
    tbman_s_release_arg_arr( o, down, arg, ptr, size, step );
    tbman_s_unlock( o );
}

vd_t bcore_tbman_s_b_alloc( bcore_tbman_s* o, vd_t current_ptr, sz_t requested_bytes, sz_t* granted_bytes )
{
    tbman_s_lock( o );
    vd_t ret = NULL;
    if( requested_bytes == 0 )
    {
        if( current_ptr )
        {
            tbman_s_free( o, current_ptr, NULL );
        }
        if( granted_bytes ) *granted_bytes = 0;
    }
    else
    {
        if( current_ptr )
        {
            ret = tbman_s_realloc( o, current_ptr, NULL, requested_bytes, granted_bytes );
        }
        else
        {
            ret = tbman_s_malloc( o, requested_bytes, granted_bytes );
        }
    }
    tbman_s_unlock( o );
    return ret;
}

vd_t bcore_tbman_s_bn_alloc( bcore_tbman_s* o, vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes )
{
    tbman_s_lock( o );
    vd_t ret = NULL;
    if( requested_bytes == 0 )
    {
        if( current_bytes ) // 0 means current_ptr may not be used for free or realloc
        {
            tbman_s_free( o, current_ptr, &current_bytes );
        }
        if( granted_bytes ) *granted_bytes = 0;
    }
    else
    {
        if( current_bytes ) // 0 means current_ptr may not be used for free or realloc
        {
            ret = tbman_s_realloc( o, current_ptr, &current_bytes, requested_bytes, granted_bytes );
        }
        else
        {
            ret = tbman_s_malloc( o, requested_bytes, granted_bytes );
        }
    }
    tbman_s_unlock( o );
    return ret;
}

vd_t bcore_tbman_s_u_alloc( bcore_tbman_s* o, sz_t unit_bytes, vd_t current_ptr, sz_t requested_units, sz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_s_b_alloc( o, current_ptr, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

vd_t bcore_tbman_s_un_alloc( bcore_tbman_s* o, sz_t unit_bytes, vd_t current_ptr, sz_t current_units, sz_t requested_units, sz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_s_bn_alloc( o, current_ptr, unit_bytes * current_units, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

static sz_t tbman_s_total_internal_alloc( const bcore_tbman_s* o )
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        sum += block_manager_s_total_alloc( o->data[ i ] );
    }
    return sum;
}

static sz_t tbman_s_internal_instances( const bcore_tbman_s* o )
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        sum += block_manager_s_instances( o->data[ i ] );
    }
    return sum;
}

static sz_t tbman_s_internal_references( const bcore_tbman_s* o )
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        sum += block_manager_s_references( o->data[ i ] );
    }
    return sum;
}

static sz_t tbman_s_total_alloc( const bcore_tbman_s* o )
{
    return external_manager_s_total_alloc( &o->external_manager ) + tbman_s_total_internal_alloc( o );
}

static sz_t tbman_s_instances( const bcore_tbman_s* o )
{
    return external_manager_s_instances( &o->external_manager ) + tbman_s_internal_instances( o );
}

static sz_t tbman_s_references( const bcore_tbman_s* o )
{
    return external_manager_s_references( &o->external_manager ) + tbman_s_internal_references( o );
}

static sz_t tbman_s_total_space( const bcore_tbman_s* o )
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        sum += block_manager_s_total_space( o->data[ i ] );
    }
    sum += block_manager_s_total_space( &o->external_manager.bm_ext );
    return sum;
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/


/**********************************************************************************************************************/
// Global manager

static bcore_tbman_s* tbman_s_g = NULL;

static void create_tbman()
{
    tbman_s_g = bcore_tbman_s_create
    (
        default_pool_size,
        default_min_block_size,
        default_max_block_size,
        default_stepping_method,
        default_full_align
    );
}

static void discard_tbman()
{
    bcore_tbman_s_discard( tbman_s_g );
    tbman_s_g = NULL;
}

static void tbman_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, create_tbman );
}

static void tbman_close()
{
    discard_tbman();
}

vd_t bcore_tbman_fork( vd_t ptr )
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_fork( tbman_s_g, ptr );
}

void bcore_tbman_release( vd_t ptr )
{
    assert( tbman_s_g != NULL );
    bcore_tbman_s_release( tbman_s_g, ptr );
}

void bcore_tbman_release_obj( fp_down_obj down, vd_t ptr )
{
    assert( tbman_s_g != NULL );
    bcore_tbman_s_release_obj( tbman_s_g, down, ptr );
}

void bcore_tbman_release_arg( fp_down_arg down, vc_t arg, vd_t ptr )
{
    assert( tbman_s_g != NULL );
    bcore_tbman_s_release_arg( tbman_s_g, down, arg, ptr );
}

void bcore_tbman_release_obj_arr( fp_down_obj down, vd_t ptr, sz_t size, sz_t step )
{
    assert( tbman_s_g != NULL );
    bcore_tbman_s_release_obj_arr( tbman_s_g, down, ptr, size, step );
}

void bcore_tbman_release_arg_arr( fp_down_arg down, vc_t arg, vd_t ptr, sz_t size, sz_t step )
{
    assert( tbman_s_g != NULL );
    bcore_tbman_s_release_arg_arr( tbman_s_g, down, arg, ptr, size, step );
}

vd_t bcore_tbman_b_alloc( vd_t current_ptr, sz_t requested_bytes, sz_t* granted_bytes )
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_b_alloc( tbman_s_g, current_ptr, requested_bytes, granted_bytes );
}

vd_t bcore_tbman_bn_alloc( vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes )
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_bn_alloc( tbman_s_g, current_ptr, current_bytes, requested_bytes, granted_bytes );
}

vd_t bcore_tbman_u_alloc( sz_t unit_bytes, vd_t current_ptr, sz_t requested_units, sz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_b_alloc( current_ptr, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

vd_t bcore_tbman_un_alloc( sz_t unit_bytes, vd_t current_ptr, sz_t current_units, sz_t requested_units, sz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_bn_alloc( current_ptr, unit_bytes * current_units, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

sz_t bcore_tbman_s_granted_space( bcore_tbman_s* o )
{
    tbman_s_lock( o );
    sz_t space = tbman_s_total_alloc( o );
    tbman_s_unlock( o );
    return space;
}

sz_t bcore_tbman_s_instances( bcore_tbman_s* o )
{
    tbman_s_lock( o );
    sz_t space = tbman_s_instances( o );
    tbman_s_unlock( o );
    return space;
}

sz_t bcore_tbman_s_references( bcore_tbman_s* o )
{
    tbman_s_lock( o );
    sz_t space = tbman_s_references( o );
    tbman_s_unlock( o );
    return space;
}

sz_t bcore_tbman_granted_space()
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_granted_space( tbman_s_g );
}

sz_t bcore_tbman_instances()
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_instances( tbman_s_g );
}

sz_t bcore_tbman_references()
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_references( tbman_s_g );
}

// not thread-safe
bcore_string_s* bcore_tbman_s_status( bcore_tbman_s* o, int detail_level )
{
    if( detail_level <= 0 ) return NULL;

    bcore_string_s* str = bcore_string_s_create();
    bcore_string_s_pushf( str, "main manager\n" );
    bcore_string_s_pushf( str, "  pool_size ........... %zu\n", o->pool_size );
    bcore_string_s_pushf( str, "  block managers ...... %zu\n", o->size );
    bcore_string_s_pushf( str, "  token managers ...... %zu\n", bcore_btree_vd_s_count( o->internal_btree, NULL, NULL ) );
    bcore_string_s_pushf( str, "  external allocs ..... %zu\n", bcore_btree_pp_s_count( o->external_manager.ex_btree, NULL, NULL ) );
    bcore_string_s_pushf( str, "  internal_btree depth  %zu\n", bcore_btree_vd_s_depth( o->internal_btree ) );
    bcore_string_s_pushf( str, "  external_btree depth  %zu\n", bcore_btree_pp_s_depth( o->external_manager.ex_btree ) );
    bcore_string_s_pushf( str, "  min_block_size ...... %zu\n", o->size > 0 ? o->data[ 0 ]->block_size : 0 );
    bcore_string_s_pushf( str, "  max_block_size ...... %zu\n", o->size > 0 ? o->data[ o->size - 1 ]->block_size : 0 );
    bcore_string_s_pushf( str, "  aligned ............. %s\n",  o->aligned ? "true" : "false" );

    bcore_string_s_pushf( str, "external manager\n" );
    bcore_string_s_pushf( str, "  pool_size ........... %zu\n", o->external_manager.pool_size );
    bcore_string_s_pushf( str, "  token managers ...... %zu\n", bcore_btree_vd_s_count( o->external_manager.tm_btree, NULL, NULL ) );
    bcore_string_s_pushf( str, "  aligned ............. %s\n",  o->external_manager.aligned ? "true" : "false" );
    bcore_string_s_pushf( str, "  item size ........... %zu\n", sizeof( ext_s ) );
    bcore_string_s_pushf( str, "  items ............... %zu\n", block_manager_s_total_alloc( &o->external_manager.bm_ext ) / sizeof( ext_s ) );
    bcore_string_s_pushf( str, "  alloc ............... %zu\n", block_manager_s_total_alloc( &o->external_manager.bm_ext ) );
    bcore_string_s_pushf( str, "  space ............... %zu\n", block_manager_s_total_space( &o->external_manager.bm_ext ) );

    bcore_string_s_pushf( str, "down manager\n" );
    bcore_string_s_pushf( str, "  pool_size ........... %zu\n", o->down_manager.pool_size );
    bcore_string_s_pushf( str, "  token managers ...... %zu\n", bcore_btree_vd_s_count( o->down_manager.tm_btree, NULL, NULL ) );
    bcore_string_s_pushf( str, "  aligned ............. %s\n",  o->down_manager.aligned ? "true" : "false" );
    bcore_string_s_pushf( str, "  down_obj_s size ..... %zu\n", sizeof( down_obj_s ) );
    bcore_string_s_pushf( str, "  down_obj_s items .... %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_obj ) / sizeof( down_obj_s ) );
    bcore_string_s_pushf( str, "  down_obj_s alloc .... %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_obj ) );
    bcore_string_s_pushf( str, "  down_obj_s space .... %zu\n", block_manager_s_total_space( &o->down_manager.bm_down_obj ) );
    bcore_string_s_pushf( str, "  down_arg_s size ..... %zu\n", sizeof( down_arg_s ) );
    bcore_string_s_pushf( str, "  down_arg_s items .... %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_arg ) / sizeof( down_arg_s ) );
    bcore_string_s_pushf( str, "  down_arg_s alloc .... %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_arg ) );
    bcore_string_s_pushf( str, "  down_arg_s space .... %zu\n", block_manager_s_total_space( &o->down_manager.bm_down_arg ) );
    bcore_string_s_pushf( str, "  down_obj_arr_s size   %zu\n", sizeof( down_obj_arr_s ) );
    bcore_string_s_pushf( str, "  down_obj_arr_s items  %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_obj_arr ) / sizeof( down_obj_arr_s ) );
    bcore_string_s_pushf( str, "  down_obj_arr_s alloc  %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_obj_arr ) );
    bcore_string_s_pushf( str, "  down_obj_arr_s space  %zu\n", block_manager_s_total_space( &o->down_manager.bm_down_obj_arr ) );
    bcore_string_s_pushf( str, "  down_arg_arr_s size   %zu\n", sizeof( down_arg_arr_s ) );
    bcore_string_s_pushf( str, "  down_arg_arr_s items  %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_arg_arr ) / sizeof( down_arg_arr_s ) );
    bcore_string_s_pushf( str, "  down_arg_arr_s alloc  %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_arg_arr ) );
    bcore_string_s_pushf( str, "  down_arg_arr_s space  %zu\n", block_manager_s_total_space( &o->down_manager.bm_down_arg_arr ) );

    bcore_string_s_pushf( str, "overall allocations\n" );
    bcore_string_s_pushf( str, "  total bytes granted . %zu\n", tbman_s_total_alloc( o ) );
    bcore_string_s_pushf( str, "    external .......... %zu\n", external_manager_s_total_alloc( &o->external_manager ) );
    bcore_string_s_pushf( str, "    internal .......... %zu\n", tbman_s_total_internal_alloc( o ) );
    bcore_string_s_pushf( str, "  internal used ....... %zu\n", tbman_s_total_space( o ) );
    bcore_string_s_pushf( str, "  total instances ..... %zu\n", tbman_s_instances( o ) );
    bcore_string_s_pushf( str, "    external .......... %zu\n", external_manager_s_instances( &o->external_manager ) );
    bcore_string_s_pushf( str, "    internal .......... %zu\n", tbman_s_internal_instances( o ) );
    bcore_string_s_pushf( str, "  total references .... %zu\n", tbman_s_references( o ) );
    bcore_string_s_pushf( str, "    external .......... %zu\n", external_manager_s_references( &o->external_manager ) );
    bcore_string_s_pushf( str, "    internal .......... %zu\n", tbman_s_internal_references( o ) );
    if( detail_level > 1 )
    {
        for( sz_t i = 0; i < o->size; i++ )
        {
            bcore_string_s_pushf( str, "\nblock manager %zu:\n", i );
            bcore_string_s_push_string_d( str, block_manager_s_status( o->data[ i ], detail_level - 1 ) );
        }
    }

    return str;
}

/**********************************************************************************************************************/
// Testing and evaluation

#include "bcore_quicktypes.h"
#include "bcore_spect_inst.h"

typedef vd_t (*fp_alloc)( vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes );

static bcore_string_s* tbman_alloc_challenge
(
    fp_alloc alloc,
    sz_t table_size,
    sz_t cycles,
    sz_t max_alloc,
    u2_t seed,
    bl_t cleanup,
    bl_t verbose
)
{
    bcore_string_s* str = bcore_string_s_create();

    vd_t* data_table = malloc( table_size * sizeof( vd_t ) );
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
        bcore_string_s_pushf( str, "cycles ............... %zu\n", cycles );
        bcore_string_s_pushf( str, "max alloc ............ %zu\n", max_alloc );
        bcore_string_s_pushf( str, "Instances\n" );
        bcore_string_s_pushf( str, "  total .............. %zu\n", table_size );
        bcore_string_s_pushf( str, "  allocated .......... %zu\n", allocated_table_size );
        bcore_string_s_pushf( str, "Alloc\n" );
        bcore_string_s_pushf( str, "  attempts  .......... %zu\n", alloc_attempts );
        bcore_string_s_pushf( str, "  failures  .......... %zu\n", alloc_failures );
        bcore_string_s_pushf( str, "Realloc\n" );
        bcore_string_s_pushf( str, "  attempts  .......... %zu\n", realloc_attempts );
        bcore_string_s_pushf( str, "  failures  .......... %zu\n", realloc_failures );
        bcore_string_s_pushf( str, "Free\n" );
        bcore_string_s_pushf( str, "  attempts  .......... %zu\n", free_attempts );
        bcore_string_s_pushf( str, "  failures  .......... %zu\n", free_failures );
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

static void tbman_s_quicktest( void )
{
    sz_t table_size = 200;
    sz_t cycles     = 1;
    sz_t max_alloc  = 4096;
    sz_t seed       = 5479;
    bcore_string_s_discard( tbman_alloc_challenge( bcore_tbman_bn_alloc, table_size, cycles, max_alloc, seed, true, false ) );
}

/// testing reference control
static bcore_string_s* tbman_s_rctest( void )
{
    bcore_string_s* log = bcore_string_s_createf( "== tbman_s_rctest " );
    bcore_string_s_push_char_n( log, '=', 120 - log->size );
    bcore_string_s_push_char( log, '\n' );

    typedef struct myclass
    {
        bcore_string_s  name1;
        bcore_string_s* name2;
        u3_t  v1;
        u3_t* v2;
    } myclass;

    tp_t t_myclass = bcore_flect_type_parse_sc( "{ bcore_string_s name1; bcore_string_s* name2; u3_t v1; u3_t* v2; }" );
    bcore_inst_typed_discard( t_myclass, bcore_inst_typed_create( t_myclass ) ); // cycle once to get all perspectives alive

    myclass* obj1 = bcore_inst_typed_create( t_myclass );
    myclass* obj2 = bcore_inst_typed_create( t_myclass );

    sz_t arr_size = 100000;
    myclass* obj_arr = bcore_tbman_malloc( sizeof( myclass ) * arr_size );
    for( sz_t i = 0; i < arr_size; i++ )
    {
        bcore_inst_typed_init( t_myclass, &obj_arr[ i ] );
        bcore_string_s_pushf( &obj_arr[ i ].name1, "Hi!" );
        obj_arr[ i ].name2 = bcore_string_s_createf( "Ho!" );
    }

    obj_arr = bcore_tbman_fork( obj_arr );
    bcore_tbman_release_arg_arr( ( fp_t )bcore_inst_spect_down, bcore_inst_s_get_typed( t_myclass ), obj_arr, arr_size, sizeof( myclass ) );

    vd_t* ref_arr = bcore_tbman_u_alloc( sizeof( vd_t ), NULL, arr_size, NULL );

    {
        clock_t time = clock();
        for( sz_t i = 0; i < arr_size; i++ )
        {
            ref_arr[ i ] = bcore_tbman_fork( &obj_arr[ i ].name1 );
        }
        time = clock() - time;
        bcore_string_s_pushf( log, "fork ....... %zuns\n", ( sz_t )( ( ( double )time * 1E9 ) / ( CLOCKS_PER_SEC  * arr_size ) ) );
    }

    bcore_string_s* obj2_name1 = bcore_tbman_fork( &obj2->name1 );

    bcore_string_s_pushf( obj2_name1, "Hi!" );

    bcore_tbman_release_arg( ( fp_t )bcore_inst_spect_down, bcore_inst_s_get_typed( t_myclass ), obj1 );
    bcore_tbman_release_arg( ( fp_t )bcore_inst_spect_down, bcore_inst_s_get_typed( t_myclass ), obj2 );

    bcore_tbman_release( obj2_name1 );

    bcore_tbman_release( obj_arr );

    {
        clock_t time = clock();
        for( sz_t i = 0; i < arr_size; i++ )
        {
            bcore_tbman_release( ref_arr[ i ] );
            ref_arr[ i ] = NULL;
        }
        time = clock() - time;
        bcore_string_s_pushf( log, "release..... %zuns\n", ( sz_t )( ( ( double )time * 1E9 ) / ( CLOCKS_PER_SEC  * arr_size ) ) );
    }

    bcore_tbman_release( ref_arr );

    bcore_string_s_push_char_n( log, '=', 120 );
    bcore_string_s_push_char( log, '\n' );

    bcore_string_s_push_string_d( log, bcore_tbman_s_status( tbman_s_g, 1 ) );
    return log;
}

static bcore_string_s* tbman_s_memtest( void )
{
    bcore_string_s* log = bcore_string_s_createf( "== tbman_s_memtest " );
    bcore_string_s_push_char_n( log, '=', 120 - log->size );
    bcore_string_s_push_char( log, '\n' );

    sz_t table_size = 100000;
    sz_t cycles     = 5;
    sz_t max_alloc  = 65536;
    sz_t seed       = 1237;

    {
        bcore_string_s_pushf( log, "\nbcore_internal_alloc:\n");
        bcore_string_s_push_string_d( log, tbman_alloc_challenge( bcore_tbman_bn_alloc, table_size, cycles, max_alloc, seed, true, false ) );
        bcore_string_s_push_string_d( log, bcore_tbman_s_status( tbman_s_g, 1 ) );
//        bcore_string_s_push_string_d( log, bcore_btree_vd_s_status( m->internal_btree ) );

    }

    {
        bcore_string_s_pushf( log, "\nbcore_external_alloc:\n");
        bcore_string_s_push_string_d( log, tbman_alloc_challenge( bcore_tbman_external_bn_alloc, table_size, cycles, max_alloc, seed, true, false ) );
    }

    bcore_string_s_push_char_n( log, '=', 120 );
    bcore_string_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_tbman_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_tbman" ) ) return NULL;
    if( signal == typeof( "init0" ) )
    {
        tbman_open();
    }
    else if( signal == typeof( "init1" ) )
    {
        tbman_s_quicktest(); // system critical test
    }
    else if( signal == typeof( "down0" ) )
    {
        sz_t space = bcore_tbman_granted_space();
        if( space > 0 )
        {
            sz_t instances = bcore_tbman_instances();
            sz_t references = bcore_tbman_references();
            ERR
            (
                "Leaking memory .... %zu bytes\n"
                "     instances .... %zu\n"
                "     references ... %zu\n",
                space,
                instances,
                references
            );
        }
        tbman_close();
    }
    else if( signal == typeof( "selftest" ) )
    {
        bcore_string_s_print_d( tbman_s_rctest() );
//        bcore_string_s_print_d( tbman_s_memtest() );
    }

    return NULL;
}

