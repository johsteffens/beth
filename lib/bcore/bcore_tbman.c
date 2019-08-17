/** Copyright 2017 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/** Token Block Memory Manager.
 *  An adapted, simplified and stand-alone version of this memory manager has
 *  been released on https://github.com/johsteffens/tbman under a public license.
 */

#include "bcore_tbman.h"
#include "bcore_control.h"
#include "bcore_btree.h"
#include "bcore_st.h"
#include "bcore_threads.h"
#include "bcore_name_manager.h"
#include "bcore_signal.h"
#include <time.h>
#include <math.h>

/**********************************************************************************************************************/
// default parameters

static const uz_t default_pool_size       = 0x10000;
static const uz_t default_min_block_size  = 8;
static const uz_t default_max_block_size  = 1024 * 16;

static const uz_t default_stepping_method = 1;
static const bool   default_full_align      = true;

/// Minimum alignment of memory blocks
#define TBMAN_ALIGN 0x100

/**********************************************************************************************************************/
// forward declarations

typedef struct block_manager_s block_manager_s;
typedef struct down_manager_s  down_manager_s;
typedef void (*fp_lost_alignment)( vd_t );

/**********************************************************************************************************************/
// shutdown-management

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

typedef struct down_arg_s
{
    down_s _; // pointer to _down_arg
    fp_down_arg fp; // external down function
    vc_t arg;  // argument for external function
} down_arg_s;

// ---------------------------------------------------------------------------------------------------------------------

static void _down_arg( const down_arg_s* o, vd_t obj ) { o->fp( o->arg, obj ); }
static down_arg_s* down_arg_s_create( down_manager_s* o, fp_down_arg fp, vc_t arg );

// ---------------------------------------------------------------------------------------------------------------------

typedef struct down_obj_arr_s
{
    down_s _; // internal down function
    fp_down_obj fp;   // external down function
    uz_t size; // number of active array elements
    uz_t step; // stepping between array elements in bytes
} down_obj_arr_s;

// ---------------------------------------------------------------------------------------------------------------------

static void _down_obj_arr( const down_obj_arr_s* o, vd_t obj )
{
    for( uz_t i = o->size; i > 0; i-- )
    {
        o->fp( obj );
        obj = ( u0_t* )obj + o->step;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static down_obj_arr_s* down_obj_arr_s_create( down_manager_s* o, fp_down_obj fp, uz_t size, uz_t step );

// ---------------------------------------------------------------------------------------------------------------------

typedef struct down_arg_arr_s
{
    down_s _; // internal down function
    fp_down_arg fp; // external down function
    vc_t arg;  // argument for external function
    uz_t size; // number of active array elements
    uz_t step; // stepping between array elements in bytes
} down_arg_arr_s;

// ---------------------------------------------------------------------------------------------------------------------

static void _down_arg_arr( const down_arg_arr_s* o, vd_t obj )
{
    for( uz_t i = o->size; i > 0; i-- )
    {
        o->fp( o->arg, obj );
        obj = ( u0_t* )obj + o->step;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static down_arg_arr_s* down_arg_arr_s_create( down_manager_s* o, fp_down_arg fp, vc_t arg, uz_t size, uz_t step );

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_external_b_alloc( vd_t current_ptr, uz_t requested_bytes, uz_t* granted_bytes )
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

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_external_bn_alloc( vd_t current_ptr, uz_t current_bytes, uz_t requested_bytes, uz_t* granted_bytes )
{
    return bcore_tbman_external_b_alloc( current_ptr, requested_bytes, granted_bytes );
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_external_alloc( vd_t ptr, uz_t size )
{
    return bcore_tbman_external_b_alloc( ptr, size, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_external_un_alloc( uz_t unit_bytes, vd_t current_ptr, uz_t current_units, uz_t requested_units, uz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_external_bn_alloc( current_ptr, unit_bytes * current_units, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_external_u_alloc( uz_t unit_bytes, vd_t current_ptr, uz_t requested_units, uz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_external_b_alloc( current_ptr, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

// ---------------------------------------------------------------------------------------------------------------------

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
    uz_t pool_size;   // size of pool in bytes
    uz_t block_size;  // size of block in bytes
    u1_t stack_size;  // size of token-stack ( == pool_size / block_size )
    u1_t stack_index; // index into token-stack

    /** aligned
     *  The memory-pool is considered aligned when the integer-evaluation of its address
     *  is a multiple of pool_size, which means that the pool address can be obtained
     *  from any pointer inside the pool by a mere integer division.
     */
    bl_t aligned;

    bcore_mutex_s* mutex;  // governing mutex
    block_manager_s* parent;
    uz_t parent_index;

    uz_t* rc_count_arr; // array of reference counters
    vd_t* rc_down_arr;  // array of shut down methods
    down_manager_s* down_manager;  // manager of down objects

    u1_t  token_stack[]; // stack of block-tokens (part of pool)
} token_manager_s;

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_init( token_manager_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_down( token_manager_s* o )
{
    if( o->rc_count_arr ) free( o->rc_count_arr );
    if( o->rc_down_arr  ) free( o->rc_down_arr  );
}

// ---------------------------------------------------------------------------------------------------------------------

static token_manager_s* token_manager_s_create
    (
        uz_t pool_size,
        uz_t block_size,
        bl_t align,
        down_manager_s* down_manager,
        bcore_mutex_s* mutex
    )
{
    if( ( pool_size & ( pool_size - 1 ) ) != 0 ) ERR( "pool_size %zu is not a power of two", pool_size );
    uz_t stack_size = pool_size / block_size;
    if( stack_size > 0x10000 ) ERR( "stack_size %zu exceeds 0x10000", stack_size );
    uz_t reserved_size   = sizeof( token_manager_s ) + sizeof( u1_t ) * stack_size;
    uz_t reserved_blocks = reserved_size / block_size + ( ( reserved_size % block_size ) > 0 );
    if( stack_size < ( reserved_blocks + 1 ) ) ERR( "pool_size %zu is too small", pool_size );

    token_manager_s* o;
    if( align )
    {
        /* We align to the entire pool_size (not just block_size).
         * This is to enable quick O(1) location of the token manager from any memory instance
         * by computing the next lower alignment point form a given address.
         */
        o = aligned_alloc( pool_size, pool_size );
        if( !o ) ERR( "Failed aligned allocating %zu bytes", pool_size );
    }
    else
    {
        o = aligned_alloc( TBMAN_ALIGN, pool_size );
        if( !o ) ERR( "Failed allocating %zu bytes", pool_size );
    }

    token_manager_s_init( o );

    /* We explicitly check alignment allowing for unexpected behavior of aligned_alloc.
     * If alignment (according to tbman's definition) fails, o->aligned is set to 'false' and
     * tbman falls back to a slightly slower btree-search to locate the token manager.
     */
    o->aligned = ( ( intptr_t )o & ( intptr_t )( pool_size - 1 ) ) == 0;
    o->pool_size = pool_size;
    o->block_size = block_size;
    o->stack_size = stack_size;
    o->stack_index = 0;
    for( uz_t i = 0; i < o->stack_size; i++ ) o->token_stack[ i ] = ( i + reserved_blocks ) < stack_size ? ( i + reserved_blocks ) : 0;
    o->down_manager = down_manager;
    o->mutex = mutex;
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_discard( token_manager_s* o )
{
    if( !o ) return;
    token_manager_s_down( o );
    free( o );
}

// ---------------------------------------------------------------------------------------------------------------------

static bl_t token_manager_s_is_full( token_manager_s* o )
{
    return o->token_stack[ o->stack_index ] == 0;
}

// ---------------------------------------------------------------------------------------------------------------------

static bl_t token_manager_s_is_empty( token_manager_s* o )
{
    return o->stack_index == 0;
}

// ---------------------------------------------------------------------------------------------------------------------

static vd_t token_manager_s_alloc( token_manager_s* o )
{
    assert( !token_manager_s_is_full( o ) );
    vd_t ret = ( u0_t* )o + o->token_stack[ o->stack_index ] * o->block_size;
    assert( ( u0_t* )ret >= ( u0_t* )o + sizeof( token_manager_s ) );
    o->stack_index++;
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

// forward declarations (implementation below)
static void block_manager_s_full_to_free( struct block_manager_s* o, token_manager_s* child );
static void block_manager_s_free_to_empty( struct block_manager_s* o, token_manager_s* child );

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_free_token( token_manager_s* o, u1_t token )
{
    #ifdef RTCHECKS
        if( token * o->block_size < sizeof( token_manager_s ) ) ERR( "Attempt to free reserved memory." );
        for( uz_t i = o->stack_index; i < o->stack_size; i++ ) if( o->token_stack[ i ] == token ) ERR( "Attempt to free memory that is declared free." );
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
                bcore_mutex_s_unlock( o->mutex );
                ( ( down_s* )down )->_( down, ( u0_t* )o + token * o->block_size );
                bcore_mutex_s_lock( o->mutex );
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

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_free( token_manager_s* o, vd_t ptr )
{
    #ifdef RTCHECKS
        if( o->stack_index == 0 ) ERR( "Block manager is empty." );
        if( ( uz_t )( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) > o->pool_size ) ERR( "Attempt to free memory outside pool." );
    #endif
    token_manager_s_free_token( o, ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size );
}

// ---------------------------------------------------------------------------------------------------------------------

/// Finds the root address of given memory instance; returns NULL in case ptr is outside token_manager's memory space
static vd_t token_manager_s_root( token_manager_s* o, vc_t ptr )
{
    u3_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;
    return ( token < o->stack_size ) ? ( u0_t* )o + token * o->block_size : NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_create_rc_count_arr( token_manager_s* o )
{
    uz_t alloc_size = sizeof( uz_t ) * o->stack_size;
    o->rc_count_arr = malloc( alloc_size );
    memset( o->rc_count_arr, 0, alloc_size );
}

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_create_rc_down_arr( token_manager_s* o )
{
    uz_t alloc_size = sizeof( vd_t ) * o->stack_size;
    o->rc_down_arr = malloc( alloc_size );
    memset( o->rc_down_arr, 0, alloc_size );
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t token_manager_s_references( token_manager_s* o, vc_t ptr )
{
    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;
    if( !o->rc_count_arr ) return 1;
    return 1 + o->rc_count_arr[ token ];
}

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_fork( token_manager_s* o, vd_t ptr )
{
    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;

    #ifdef RTCHECKS
        if( token * o->block_size < sizeof( token_manager_s ) ) ERR( "Attempt to fork reserved memory." );
        for( uz_t i = o->stack_index; i < o->stack_size; i++ ) if( o->token_stack[ i ] == token ) ERR( "Attempt to fork memory that is declared free." );
    #endif // RTCHECKS

    if( !o->rc_count_arr ) token_manager_s_create_rc_count_arr( o );
    o->rc_count_arr[ token ]++;
    assert( o->rc_count_arr[ token ] ); // overflow check
}

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_release( token_manager_s* o, vd_t ptr )
{
    token_manager_s_free( o, ptr );
}

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_release_obj( token_manager_s* o, fp_down_obj down, vd_t ptr )
{
    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;
    if( !o->rc_count_arr )
    {
        bcore_mutex_s_unlock( o->mutex );
        down( ptr );
        bcore_mutex_s_lock( o->mutex );
        token_manager_s_free_token( o, token );
        return;
    }

    if( o->rc_count_arr[ token ] )
    {
        o->rc_count_arr[ token ]--;
        if( !o->rc_down_arr ) token_manager_s_create_rc_down_arr( o );
        if( !o->rc_down_arr[ token ] ) o->rc_down_arr[ token ] = down_obj_s_create( o->down_manager, down );
        return;
    }

    if( !o->rc_down_arr || !o->rc_down_arr[ token ] )
    {
        bcore_mutex_s_unlock( o->mutex );
        down( ptr );
        bcore_mutex_s_lock( o->mutex );
    }
    token_manager_s_free_token( o, token );
}

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_release_arg( token_manager_s* o, fp_down_arg down, vc_t arg, vd_t ptr )
{
    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;
    if( !o->rc_count_arr )
    {
        bcore_mutex_s_unlock( o->mutex );
        down( arg, ptr );
        bcore_mutex_s_lock( o->mutex );
        token_manager_s_free_token( o, token );
        return;
    }

    if( o->rc_count_arr[ token ] )
    {
        o->rc_count_arr[ token ]--;
        if( !o->rc_down_arr ) token_manager_s_create_rc_down_arr( o );
        if( !o->rc_down_arr[ token ] ) o->rc_down_arr[ token ] = down_arg_s_create( o->down_manager, down, arg );
        return;
    }

    if( !o->rc_down_arr || !o->rc_down_arr[ token ] )
    {
        bcore_mutex_s_unlock( o->mutex );
        down( arg, ptr );
        bcore_mutex_s_lock( o->mutex );
    }

    token_manager_s_free_token( o, token );
}

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_release_obj_arr( token_manager_s* o, fp_down_obj down, vd_t ptr, uz_t size, uz_t step )
{
    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;
    if( !o->rc_count_arr )
    {
        bcore_mutex_s_unlock( o->mutex );
        for( uz_t i = 0; i < size; i++ ) down( ( u0_t* )ptr + i * step );
        bcore_mutex_s_lock( o->mutex );
        token_manager_s_free_token( o, token );
        return;
    }

    if( o->rc_count_arr[ token ] )
    {
        o->rc_count_arr[ token ]--;
        if( !o->rc_down_arr ) token_manager_s_create_rc_down_arr( o );
        if( !o->rc_down_arr[ token ] ) o->rc_down_arr[ token ] = down_obj_arr_s_create( o->down_manager, down, size, step );
        return;
    }

    if( !o->rc_down_arr || !o->rc_down_arr[ token ] )
    {
        bcore_mutex_s_unlock( o->mutex );
        for( uz_t i = 0; i < size; i++ ) down( ( u0_t* )ptr + i * step );
        bcore_mutex_s_lock( o->mutex );
    }
    token_manager_s_free_token( o, token );
}

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_release_arg_arr( token_manager_s* o, fp_down_arg down, vc_t arg, vd_t ptr, uz_t size, uz_t step )
{
    u1_t token = ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )o ) ) / o->block_size;
    if( !o->rc_count_arr )
    {
        bcore_mutex_s_unlock( o->mutex );
        for( uz_t i = 0; i < size; i++ ) down( arg, ( u0_t* )ptr + i * step );
        bcore_mutex_s_lock( o->mutex );
        token_manager_s_free_token( o, token );
        return;
    }

    if( o->rc_count_arr[ token ] )
    {
        o->rc_count_arr[ token ]--;
        if( !o->rc_down_arr ) token_manager_s_create_rc_down_arr( o );
        if( !o->rc_down_arr[ token ] ) o->rc_down_arr[ token ] = down_arg_arr_s_create( o->down_manager, down, arg, size, step );
        return;
    }

    if( !o->rc_down_arr || !o->rc_down_arr[ token ] )
    {
        bcore_mutex_s_unlock( o->mutex );
        for( uz_t i = 0; i < size; i++ ) down( arg, ( u0_t* )ptr + i * step );
        bcore_mutex_s_lock( o->mutex );
    }

    token_manager_s_free_token( o, token );
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t token_manager_s_total_alloc( const token_manager_s* o )
{
    return o->block_size * o->stack_index;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t token_manager_s_total_instances( const token_manager_s* o )
{
    return o->stack_index;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t token_manager_s_total_references( const token_manager_s* o )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->stack_index; i++ )
    {
        uz_t token = o->token_stack[ i ];
        count++;
        if( o->rc_count_arr ) count += o->rc_count_arr[ token ];
    }
    return count;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t token_manager_s_total_space( const token_manager_s* o )
{
    return o->pool_size + o->stack_size * sizeof( u1_t );
}

// ---------------------------------------------------------------------------------------------------------------------

static void token_manager_s_for_each_instance( token_manager_s* o, void (*cb)( vd_t arg, vd_t ptr, uz_t space ), vd_t arg )
{
    if( !cb ) return;
    for( uz_t i = 0; i < o->stack_index; i++ )
    {
        uz_t token = o->token_stack[ i ];
        cb( arg, ( u0_t* )o + token * o->block_size, o->block_size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static st_s* token_manager_s_status( const token_manager_s* o, int detail_level )
{
    st_s* str = st_s_create();
    if( detail_level <= 0 ) return str;
    st_s_pushf( str, "    pool_size:   %zu\n",  o->pool_size );
    st_s_pushf( str, "    block_size:  %zu\n",  o->block_size );
    st_s_pushf( str, "    stack_size:  %zu\n",  o->stack_size );
    st_s_pushf( str, "    aligned:     %s\n",   o->aligned ? "true" : "false" );
    st_s_pushf( str, "    stack_index: %zu\n",  ( uz_t )o->stack_index );
    st_s_pushf( str, "    total alloc: %zu\n", token_manager_s_total_alloc( o ) );
    st_s_pushf( str, "    total space: %zu\n", token_manager_s_total_space( o ) );
    return str;
}

// ---------------------------------------------------------------------------------------------------------------------

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
    uz_t pool_size;  // pool size of all token-managers
    uz_t block_size; // block size of all token-managers
    bl_t align;      // attempt to align token_managers to pool_size
    token_manager_s** data;
    uz_t size, space;
    uz_t free_index;       // entries equal or above free_index have space for allocation
    f3_t sweep_hysteresis; // if ( empty token-managers ) / ( used token-managers ) < sweep_hysteresis, empty token-managers are discarded
    bl_t aligned;          // all token managers are aligned to pool_size
    vd_t parent;           // parent
    fp_lost_alignment lost_alignment_fp; // callback for lost alignment
    bcore_btree_vd_s* internal_btree;
    down_manager_s* down_manager;
    bcore_mutex_s* mutex;  // governing mutex
} block_manager_s;

// ---------------------------------------------------------------------------------------------------------------------

static void block_manager_s_init
    (
        block_manager_s* o,
        vd_t parent,
        uz_t pool_size,
        uz_t block_size,
        bl_t align,
        fp_lost_alignment lost_alignment_fp,
        bcore_btree_vd_s* internal_btree,
        down_manager_s* down_manager,
        bcore_mutex_s* mutex
    )
{
    bcore_memzero( o, sizeof( *o ) );
    o->parent = parent;
    o->aligned = true;
    o->sweep_hysteresis = 0.125;
    o->pool_size = pool_size;
    o->block_size = block_size;
    o->align = align;
    o->lost_alignment_fp = lost_alignment_fp;
    o->internal_btree = internal_btree;
    o->down_manager = down_manager;
    o->mutex = mutex;
}

// ---------------------------------------------------------------------------------------------------------------------

static void block_manager_s_down( block_manager_s* o )
{
    if( o->data )
    {
        for( uz_t i = 0; i < o->size; i++ ) token_manager_s_discard( o->data[ i ] );
        free( o->data );
        o->data = NULL;
        o->size = o->space = 0;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static block_manager_s* block_manager_s_create
    (
        vd_t parent,
        uz_t pool_size,
        uz_t block_size,
        bl_t align,
        fp_lost_alignment lost_alignment_fp,
        bcore_btree_vd_s* internal_btree,
        down_manager_s* down_manager,
        bcore_mutex_s* mutex
    )
{
    block_manager_s* o = malloc( sizeof( block_manager_s ) );
    if( !o ) ERR( "Failed allocating %zu bytes", sizeof( block_manager_s ) );
    block_manager_s_init( o, parent, pool_size, block_size, align, lost_alignment_fp, internal_btree, down_manager, mutex );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

static void block_manager_s_discard( block_manager_s* o )
{
    if( !o ) return;
    block_manager_s_down( o );
    free( o );
}

// ---------------------------------------------------------------------------------------------------------------------

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
        if( o->lost_alignment_fp ) o->lost_alignment_fp( o->parent );
    }
    if( bcore_btree_vd_s_set( o->internal_btree, o->data[ o->size ] ) != 1 ) ERR( "Failed registering block address." );
    o->size++;
}

// ---------------------------------------------------------------------------------------------------------------------

static vd_t block_manager_s_alloc( block_manager_s* o )
{
    if( o->free_index == o->size ) block_manager_s_acquire_token_manager( o );
    token_manager_s* child = o->data[ o->free_index ];
    vd_t ret = token_manager_s_alloc( child );
    if( token_manager_s_is_full( child ) ) o->free_index++;
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

// A child reports turning full --> free
static void block_manager_s_full_to_free( block_manager_s* o, token_manager_s* child )
{
    assert( o->free_index > 0 );

    o->free_index--;

    // swap child with current free position
    uz_t child_index = child->parent_index;
    uz_t swapc_index = o->free_index;

    token_manager_s* swapc = o->data[ swapc_index ];
    o->data[ swapc_index ] = child;
    o->data[ child_index ] = swapc;
    child->parent_index = swapc_index;
    swapc->parent_index = child_index;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t block_manager_s_empty_tail( const block_manager_s* o )
{
    if( o->size == 0 ) return 0;
    uz_t empty_index = o->size;
    while( empty_index > 0 && token_manager_s_is_empty( o->data[ empty_index - 1 ] ) ) empty_index--;
    return o->size - empty_index;
}

// ---------------------------------------------------------------------------------------------------------------------

// A child reports turning free --> empty
static void block_manager_s_free_to_empty( block_manager_s* o, token_manager_s* child )
{
    // move empty manager to tail (if not already there)
    uz_t child_index = child->parent_index;
    uz_t empty_tail  = block_manager_s_empty_tail( o );
    if( empty_tail < o->size )
    {
        uz_t swapc_index = o->size - empty_tail - 1;
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

// ---------------------------------------------------------------------------------------------------------------------

static uz_t block_manager_s_total_alloc( const block_manager_s* o )
{
    uz_t sum = 0;
    for( uz_t i = 0; i < o->size; i++ )
    {
        sum += token_manager_s_total_alloc( o->data[ i ] );
    }
    return sum;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t block_manager_s_total_instances( const block_manager_s* o )
{
    uz_t sum = 0;
    for( uz_t i = 0; i < o->size; i++ )
    {
        sum += token_manager_s_total_instances( o->data[ i ] );
    }
    return sum;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t block_manager_s_total_references( const block_manager_s* o )
{
    uz_t sum = 0;
    for( uz_t i = 0; i < o->size; i++ )
    {
        sum += token_manager_s_total_references( o->data[ i ] );
    }
    return sum;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t block_manager_s_total_space( const block_manager_s* o )
{
    uz_t sum = 0;
    for( uz_t i = 0; i < o->size; i++ )
    {
        sum += token_manager_s_total_space( o->data[ i ] );
    }
    return sum;
}

// ---------------------------------------------------------------------------------------------------------------------

static void block_manager_s_for_each_instance( block_manager_s* o, void (*cb)( vd_t arg, vd_t ptr, uz_t space ), vd_t arg )
{
    for( uz_t i = 0; i < o->size; i++ ) token_manager_s_for_each_instance( o->data[ i ], cb, arg );
}

// ---------------------------------------------------------------------------------------------------------------------

static st_s* block_manager_s_status( const block_manager_s* o, int detail_level )
{
    st_s* str = st_s_create();
    if( detail_level <= 0 ) return str;
    st_s_pushf( str, "  pool_size:        %zu\n", o->pool_size );
    st_s_pushf( str, "  block_size:       %zu\n", o->block_size );
    st_s_pushf( str, "  sweep_hysteresis: %g\n",  o->sweep_hysteresis );
    st_s_pushf( str, "  aligned:          %s\n",  o->aligned ? "true" : "false" );
    st_s_pushf( str, "  token_managers:   %zu\n", o->size );
    st_s_pushf( str, "      full:         %zu\n", o->free_index );
    st_s_pushf( str, "      empty:        %zu\n", block_manager_s_empty_tail( o ) );
    st_s_pushf( str, "  total alloc:      %zu\n", block_manager_s_total_alloc( o ) );
    st_s_pushf( str, "  total space:      %zu\n", block_manager_s_total_space( o ) );
    if( detail_level > 1 )
    {
        for( uz_t i = 0; i < o->size; i++ )
        {
            st_s_pushf( str, "\nblock manager %zu:\n", i );
            st_s_push_st_d( str, token_manager_s_status( o->data[ i ], detail_level - 1 ) );
        }
    }
    return str;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/** External-Control
 *  Control framework for external storage
 */

// ---------------------------------------------------------------------------------------------------------------------

typedef struct ext_s // external item
{
    uz_t size;     // allocated size
    uz_t rc_count; // reference counters
    vd_t rc_down;  // shut down method
} ext_s;

// ---------------------------------------------------------------------------------------------------------------------

static void ext_s_init( ext_s* o ) { bcore_memzero( o, sizeof( ext_s ) ); }
static void ext_s_down( ext_s* o ) {}

// ---------------------------------------------------------------------------------------------------------------------

typedef struct external_manager_s
{
    block_manager_s   bm_ext;
    bcore_btree_vd_s* tm_btree; // btree of token managers
    bcore_btree_pp_s* ex_btree; // btree of external references
    uz_t              pool_size;
    bl_t              aligned;
    down_manager_s*   down_manager;  // manager of down objects
    bcore_mutex_s*    mutex;    // governing mutex
} external_manager_s;

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_lost_alignment( external_manager_s* o )
{
    o->aligned = false;
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_init
    (
        external_manager_s* o,
        uz_t pool_size,
        bl_t full_align,
        down_manager_s* down_manager,
        bcore_mutex_s* mutex
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
            ( fp_lost_alignment )external_manager_s_lost_alignment,
            o->tm_btree,
            NULL,
            mutex
        );
    o->aligned = true;
    o->down_manager = down_manager;
    o->mutex = mutex;
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_down( external_manager_s* o )
{
    block_manager_s_down( &o->bm_ext );
    bcore_btree_vd_s_discard( o->tm_btree );
    bcore_btree_pp_s_discard( o->ex_btree );
}

// ---------------------------------------------------------------------------------------------------------------------

static ext_s* ext_s_create( external_manager_s* o )
{
    ext_s* ext = block_manager_s_alloc( &o->bm_ext );
    ext_s_init( ext );
    return ext;
}

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

static vd_t external_manager_s_malloc( external_manager_s* o, uz_t requested_bytes, uz_t* granted_bytes )
{
    vd_t ptr = aligned_alloc( TBMAN_ALIGN, requested_bytes );
    if( !ptr ) ERR( "Failed allocating %zu bytes.", requested_bytes );
    if( granted_bytes ) *granted_bytes = requested_bytes;
    ext_s* ext = ext_s_create( o );
    ext->size = requested_bytes;
    if( bcore_btree_pp_s_set( o->ex_btree, ptr, ext ) != 1 ) ERR( "Registering new address failed" );
    return ptr;
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_free_ext( external_manager_s* o, vd_t ptr, ext_s* ext )
{
    // reference control
    if( ext->rc_count )
    {
        ext->rc_count--;
        return;
    }

    if( ext->rc_down )
    {
        down_s* down = ext->rc_down;
        bcore_mutex_s_unlock( o->mutex );
        down->_( down, ptr );
        bcore_mutex_s_lock( o->mutex );
        assert( o->down_manager );
        down_s_discard( o->down_manager, down );
        ext->rc_down = NULL;
    }

    bcore_btree_pp_s_remove( o->ex_btree, ptr );
    ext_s_discard( o, ext );
    free( ptr );
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_free( external_manager_s* o, vd_t ptr )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Attempt to free invalid memory" );
    external_manager_s_free_ext( o, ptr, *ext_p );
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_get_instance( external_manager_s* o, vc_t ptr, vd_t* root, uz_t* granted_space )
{
    bcore_btree_pp_kv_s* kv = bcore_btree_pp_s_largest_below_equal( o->ex_btree, ( vd_t )ptr );
    if( !kv ) ERR( "Object has no root address in dynamic memory address in dynamic memory." );
    ext_s* ext = kv->val;
    if( ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )kv->key ) ) >= ext->size ) ERR( "Object has no root address in dynamic memory." );

    if( root ) *root = kv->key;
    if( granted_space ) *granted_space = ext->size;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t external_manager_s_granted_bytes( external_manager_s* o, vd_t ptr )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Could not retrieve current external memory" );
    return ( ( ext_s* )*ext_p )->size;
}

// ---------------------------------------------------------------------------------------------------------------------

static void ext_sum( vd_t val, bcore_btree_pp_kv_s kv ) { *(uz_t*)val += ( ( ext_s* )kv.val )->size; }

// ---------------------------------------------------------------------------------------------------------------------

static uz_t external_manager_s_total_alloc( const external_manager_s* o )
{
    uz_t size = 0;
    bcore_btree_pp_s_run( o->ex_btree, ext_sum, &size );
    return size;
}

// ---------------------------------------------------------------------------------------------------------------------

static void ext_count( vd_t val, bcore_btree_pp_kv_s kv ) { *(uz_t*)val += 1; }

// ---------------------------------------------------------------------------------------------------------------------

static uz_t external_manager_s_total_instances( const external_manager_s* o )
{
    uz_t size = 0;
    bcore_btree_pp_s_run( o->ex_btree, ext_count, &size );
    return size;
}

// ---------------------------------------------------------------------------------------------------------------------

static void ext_rc_ref_count( vd_t val, bcore_btree_pp_kv_s kv )
{
    *(uz_t*)val += 1 + ( ( ext_s* )kv.val )->rc_count;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t external_manager_s_total_references( const external_manager_s* o )
{
    uz_t size = 0;
    bcore_btree_pp_s_run( o->ex_btree, ext_rc_ref_count, &size );
    return size;
}

// ---------------------------------------------------------------------------------------------------------------------

typedef struct ext_for_instance_arg
{
    void (*cb)( vd_t arg, vd_t ptr, uz_t space );
    vd_t arg;
} ext_for_instance_arg;

// ---------------------------------------------------------------------------------------------------------------------

static void ext_for_instance( vd_t val, bcore_btree_pp_kv_s kv )
{
    ext_for_instance_arg* iarg = val;
    iarg->cb( iarg->arg, kv.key, ( ( ext_s* )kv.val )->size );
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_for_each_instance( external_manager_s* o, void (*cb)( vd_t arg, vd_t ptr, uz_t space ), vd_t arg )
{
    ext_for_instance_arg iarg = { .cb = cb, .arg = arg };
    bcore_btree_pp_s_run( o->ex_btree, ext_for_instance, &iarg );
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t external_manager_s_references( external_manager_s* o, vc_t ptr )
{
    bcore_btree_pp_kv_s* kv = bcore_btree_pp_s_largest_below_equal( o->ex_btree, ( vd_t )ptr );
    if( !kv ) return 0;
    ext_s* ext = kv->val;
    if( ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )kv->key ) ) >= ext->size ) return 0;
    return 1 + ext->rc_count;
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_fork( external_manager_s* o, vd_t ptr )
{
    bcore_btree_pp_kv_s* kv = bcore_btree_pp_s_largest_below_equal( o->ex_btree, ptr );
    if( !kv ) ERR( "Object has no root address in dynamic memory." );
    ext_s* ext = kv->val;
    if( ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )kv->key ) ) >= ext->size ) ERR( "Object has no root address in dynamic memory." );
    ext->rc_count++;
    assert( ext->rc_count ); // overflow check
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_release( external_manager_s* o, vd_t ptr )
{
    bcore_btree_pp_kv_s* kv = bcore_btree_pp_s_largest_below_equal( o->ex_btree, ptr );
    if( !kv ) ERR( "Object has no root address in dynamic memory." );
    ext_s* ext = kv->val;
    if( ( ( ptrdiff_t )( ( u0_t* )ptr - ( u0_t* )kv->key ) ) >= ext->size ) ERR( "Object has no root address in dynamic memory." );
    external_manager_s_free_ext( o, kv->key, ext );
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_release_obj( external_manager_s* o, fp_down_obj down, vd_t ptr )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Object is not root." );
    ext_s* ext = *ext_p;
    if( ext->rc_count )
    {
        ext->rc_count--;
        if( !ext->rc_down ) ext->rc_down = down_obj_s_create( o->down_manager, down );
    }
    else
    {
        if( !ext->rc_down )
        {
            bcore_mutex_s_unlock( o->mutex );
            down( ptr );
            bcore_mutex_s_lock( o->mutex );
        }
        external_manager_s_free_ext( o, ptr, ext );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_release_arg( external_manager_s* o, fp_down_arg down, vc_t arg, vd_t ptr )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Object is not root." );
    ext_s* ext = *ext_p;
    if( ext->rc_count )
    {
        ext->rc_count--;
        assert( o->down_manager );
        if( !ext->rc_down ) ext->rc_down = down_arg_s_create( o->down_manager, down, arg );
    }
    else
    {
        if( !ext->rc_down )
        {
            bcore_mutex_s_unlock( o->mutex );
            down( arg, ptr );
            bcore_mutex_s_lock( o->mutex );
        }
        external_manager_s_free_ext( o, ptr, ext );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_release_obj_arr( external_manager_s* o, fp_down_obj down, vd_t ptr, uz_t size, uz_t step )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Object is not root." );
    ext_s* ext = *ext_p;
    if( ext->rc_count )
    {
        ext->rc_count--;
        assert( o->down_manager );
        if( !ext->rc_down ) ext->rc_down = down_obj_arr_s_create( o->down_manager, down, size, step );
    }
    else
    {
        if( !ext->rc_down )
        {
            bcore_mutex_s_unlock( o->mutex );
            for( uz_t i = 0; i < size; i++ ) down( ( u0_t* )ptr + i * step );
            bcore_mutex_s_lock( o->mutex );
        }
        external_manager_s_free_ext( o, ptr, ext );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static void external_manager_s_release_arg_arr( external_manager_s* o, fp_down_arg down, vc_t arg, vd_t ptr, uz_t size, uz_t step )
{
    vd_t* ext_p = bcore_btree_pp_s_val( o->ex_btree, ptr );
    if( !ext_p ) ERR( "Object is not root." );
    ext_s* ext = *ext_p;
    if( ext->rc_count )
    {
        ext->rc_count--;
        assert( o->down_manager );
        if( !ext->rc_down ) ext->rc_down = down_arg_arr_s_create( o->down_manager, down, arg, size, step );
    }
    else
    {
        if( !ext->rc_down )
        {
            bcore_mutex_s_unlock( o->mutex );
            for( uz_t i = 0; i < size; i++ ) down( arg, ( u0_t* )ptr + i * step );
            bcore_mutex_s_lock( o->mutex );
        }
        external_manager_s_free_ext( o, ptr, ext );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/// Manager of down-objects

// ---------------------------------------------------------------------------------------------------------------------

typedef struct down_manager_s
{
    block_manager_s   bm_down_obj;
    block_manager_s   bm_down_arg;
    block_manager_s   bm_down_obj_arr;
    block_manager_s   bm_down_arg_arr;
    bcore_btree_vd_s* tm_btree; // btree of token managers
    uz_t              pool_size;
    bl_t              aligned;
    bcore_mutex_s*    mutex;    // governing mutex
} down_manager_s;

// ---------------------------------------------------------------------------------------------------------------------

static void down_manager_s_lost_alignment( external_manager_s* o )
{
    o->aligned = false;
}

// ---------------------------------------------------------------------------------------------------------------------

static void down_manager_s_init( down_manager_s* o, uz_t pool_size, bl_t full_align, bcore_mutex_s* mutex )
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
        ( fp_lost_alignment )down_manager_s_lost_alignment,
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
        ( fp_lost_alignment )down_manager_s_lost_alignment,
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
        ( fp_lost_alignment )down_manager_s_lost_alignment,
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
        ( fp_lost_alignment )down_manager_s_lost_alignment,
        o->tm_btree,
        NULL,
        mutex
    );

    o->aligned = true;
}

// ---------------------------------------------------------------------------------------------------------------------

static void down_manager_s_down( down_manager_s* o )
{
    block_manager_s_down( &o->bm_down_obj );
    block_manager_s_down( &o->bm_down_arg );
    block_manager_s_down( &o->bm_down_obj_arr );
    block_manager_s_down( &o->bm_down_arg_arr );
    bcore_btree_vd_s_discard( o->tm_btree );
}

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

static down_obj_s* down_obj_s_create( down_manager_s* o, fp_down_obj fp )
{
    down_obj_s* down = block_manager_s_alloc( &o->bm_down_obj );
    down->_._ = ( fp_down )_down_obj;
    down->fp = fp;
    return down;
}

// ---------------------------------------------------------------------------------------------------------------------

static down_arg_s* down_arg_s_create( down_manager_s* o, fp_down_arg fp, vc_t arg )
{
    down_arg_s* down = block_manager_s_alloc( &o->bm_down_arg );
    down->_._ = ( fp_down )_down_arg;
    down->fp = fp;
    down->arg = arg;
    return down;
}

// ---------------------------------------------------------------------------------------------------------------------

static down_obj_arr_s* down_obj_arr_s_create( down_manager_s* o, fp_down_obj fp, uz_t size, uz_t step )
{
    down_obj_arr_s* down = block_manager_s_alloc( &o->bm_down_obj_arr );
    down->_._ = ( fp_down )_down_obj_arr;
    down->fp = fp;
    down->size = size;
    down->step = step;
    return down;
}

// ---------------------------------------------------------------------------------------------------------------------

static down_arg_arr_s* down_arg_arr_s_create( down_manager_s* o, fp_down_arg fp, vc_t arg, uz_t size, uz_t step )
{
    down_arg_arr_s* down = block_manager_s_alloc( &o->bm_down_arg_arr );
    down->_._ = ( fp_down )_down_arg_arr;
    down->fp = fp;
    down->arg = arg;
    down->size = size;
    down->step = step;
    return down;

}

// ---------------------------------------------------------------------------------------------------------------------

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
 *       (O(log(n)) - where 'n' is the current amount of token managers.)
 *
 */
typedef struct bcore_tbman_s
{
    block_manager_s** data; // block managers are sorted by increasing block size
    uz_t size;
    uz_t pool_size;               // pool size for all token managers
    uz_t min_block_size;
    uz_t max_block_size;
    bl_t aligned;                 // all token managers are aligned
    uz_t* block_size_array;       // copy of block size values (for fast access)
    bcore_btree_vd_s* internal_btree;
    external_manager_s  external_manager;
    down_manager_s down_manager;
    bcore_mutex_s mutex;
} bcore_tbman_s;

// ---------------------------------------------------------------------------------------------------------------------

static inline void tbman_s_lock( bcore_tbman_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
}

// ---------------------------------------------------------------------------------------------------------------------

static inline void tbman_s_unlock( bcore_tbman_s* o )
{
    bcore_mutex_s_unlock( &o->mutex );
}

// ---------------------------------------------------------------------------------------------------------------------

static void tbman_s_lost_alignment( struct bcore_tbman_s* o )
{
    o->aligned = false;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_s_init( bcore_tbman_s* o, uz_t pool_size, uz_t min_block_size, uz_t max_block_size, uz_t stepping_method, bl_t full_align )
{
    bcore_memzero( o, sizeof( *o ) );
    bcore_mutex_s_init( &o->mutex );

    o->internal_btree = bcore_btree_vd_s_create( bcore_tbman_external_alloc );

    /// The following three values are configurable parameters of memory manager
    o->pool_size      = pool_size;
    o->min_block_size = min_block_size;
    o->max_block_size = max_block_size;

    uz_t mask_bxp  = stepping_method;
    uz_t size_mask = ( 1 << mask_bxp ) - 1;
    uz_t size_inc  = o->min_block_size;
    while( ( size_mask < o->min_block_size ) || ( ( size_mask << 1 ) & o->min_block_size ) != 0 ) size_mask <<= 1;

    uz_t space = 0;

    for( uz_t block_size = o->min_block_size; block_size <= o->max_block_size; block_size += size_inc )
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
                ( fp_lost_alignment )tbman_s_lost_alignment,
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

    o->block_size_array = malloc( o->size * sizeof( uz_t ) );
    if( !o->block_size_array ) ERR( "Failed allocating %zu bytes", o->size * sizeof( uz_t ) );

    o->aligned = true;
    for( uz_t i = 0; i < o->size; i++ )
    {
        o->aligned = o->aligned && o->data[ i ]->aligned;
        o->block_size_array[ i ] = o->data[ i ]->block_size;
    }

    external_manager_s_init( &o->external_manager, pool_size, full_align, &o->down_manager, &o->mutex );
    down_manager_s_init( &o->down_manager, pool_size, full_align, &o->mutex );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_s_down( bcore_tbman_s* o )
{
    tbman_s_lock( o );
    external_manager_s_down( &o->external_manager );
    down_manager_s_down( &o->down_manager );

    if( o->data )
    {
        for( uz_t i = 0; i < o->size; i++ ) block_manager_s_discard( o->data[ i ] );
        free( o->data );
    }

    bcore_btree_vd_s_discard( o->internal_btree );

    if( o->block_size_array ) free( o->block_size_array );

    tbman_s_unlock( o );
    bcore_mutex_s_down( &o->mutex );
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_tbman_s* bcore_tbman_s_create( uz_t pool_size, uz_t min_block_size, uz_t max_block_size, uz_t stepping_method, bl_t full_align )
{
    bcore_tbman_s* o = malloc( sizeof( bcore_tbman_s ) );
    if( !o ) ERR( "Failed allocating %zu bytes", sizeof( bcore_tbman_s ) );
    bcore_tbman_s_init( o, pool_size, min_block_size, max_block_size, stepping_method, full_align );
    return o;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_tbman_s* bcore_tbman_s_create_default()
{
    return bcore_tbman_s_create
    (
        default_pool_size,
        default_min_block_size,
        default_max_block_size,
        default_stepping_method,
        default_full_align
    );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_s_discard( bcore_tbman_s* o )
{
    if( !o ) return;
    bcore_tbman_s_down( o );
    free( o );
}

// ---------------------------------------------------------------------------------------------------------------------

static vd_t tbman_s_mem_alloc( bcore_tbman_s* o, uz_t requested_bytes, uz_t* granted_bytes )
{
    block_manager_s* block_manager = NULL;
    for( uz_t i = 0; i < o->size; i++ )
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

// ---------------------------------------------------------------------------------------------------------------------

static void tbman_s_mem_free( bcore_tbman_s* o, vd_t current_ptr, const uz_t* current_bytes )
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

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

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

// ---------------------------------------------------------------------------------------------------------------------

static void tbman_s_release_obj_arr( bcore_tbman_s* o, fp_down_obj down, vd_t ptr, uz_t size, uz_t step )
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

// ---------------------------------------------------------------------------------------------------------------------

static void tbman_s_release_arg_arr( bcore_tbman_s* o, fp_down_arg down, vc_t arg, vd_t ptr, uz_t size, uz_t step )
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

// ---------------------------------------------------------------------------------------------------------------------

static uz_t tbman_s_references( bcore_tbman_s* o, vc_t ptr )
{
    vd_t block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, ( vd_t )ptr );
    if( block_ptr && ( ( ptrdiff_t )( (u0_t*)ptr - (u0_t*)block_ptr ) < o->pool_size ) )
    {
        return token_manager_s_references( block_ptr, ptr );
    }
    else
    {
        return external_manager_s_references( &o->external_manager, ptr );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static vd_t tbman_s_mem_realloc( bcore_tbman_s* o, vd_t current_ptr, const uz_t* current_bytes, uz_t requested_bytes, uz_t* granted_bytes )
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
            vd_t reserved_ptr = tbman_s_mem_alloc( o, requested_bytes, granted_bytes );
            bcore_memcpy( reserved_ptr, current_ptr, token_manager->block_size );
            token_manager_s_free( token_manager, current_ptr );
            return reserved_ptr;
        }
        else // size reduction
        {
            block_manager_s* block_manager = NULL;
            for( uz_t i = 0; i < o->size; i++ )
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
            vd_t reserved_ptr = tbman_s_mem_alloc( o, requested_bytes, granted_bytes );
            bcore_memcpy( reserved_ptr, current_ptr, requested_bytes );
            external_manager_s_free( &o->external_manager, current_ptr );
            return reserved_ptr;
        }
        else // neither old nor new size handled by this manager
        {

            uz_t current_ext_bytes = external_manager_s_granted_bytes( &o->external_manager, current_ptr );

            // if requested bytes is less but not significantly less than current bytes, keep current memory
            if( ( requested_bytes < current_ext_bytes ) && ( requested_bytes >= ( current_ext_bytes >> 1 ) ) )
            {
                if( granted_bytes ) *granted_bytes = current_ext_bytes;
                return current_ptr;
            }

            vd_t reserved_ptr = external_manager_s_malloc( &o->external_manager, requested_bytes, granted_bytes );
            uz_t copy_bytes = ( requested_bytes < current_ext_bytes ) ? requested_bytes : current_ext_bytes;
            bcore_memcpy( reserved_ptr, current_ptr, copy_bytes );
            external_manager_s_free( &o->external_manager, current_ptr );
            return reserved_ptr;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_s_fork( bcore_tbman_s* o, vd_t ptr )
{
    if( !ptr ) return NULL;
    tbman_s_lock( o );
    tbman_s_fork( o, ptr );
    tbman_s_unlock( o );
    return ptr;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_s_release( bcore_tbman_s* o, vd_t ptr )
{
    if( !ptr ) return;
    tbman_s_lock( o );
    tbman_s_release( o, ptr );
    tbman_s_unlock( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_s_release_obj( bcore_tbman_s* o, fp_down_obj down, vd_t ptr )
{
    if( !ptr ) return;
    tbman_s_lock( o );
    tbman_s_release_obj( o, down, ptr );
    tbman_s_unlock( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_s_release_arg( bcore_tbman_s* o, fp_down_arg down, vc_t arg, vd_t ptr )
{
    if( !ptr ) return;
    tbman_s_lock( o );
    tbman_s_release_arg( o, down, arg, ptr );
    tbman_s_unlock( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_s_release_obj_arr( bcore_tbman_s* o, fp_down_obj down, vd_t ptr, uz_t size, uz_t step )
{
    if( !ptr ) return;
    tbman_s_lock( o );
    tbman_s_release_obj_arr( o, down, ptr, size, step );
    tbman_s_unlock( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_s_release_arg_arr( bcore_tbman_s* o, fp_down_arg down, vc_t arg, vd_t ptr, uz_t size, uz_t step )
{
    if( !ptr ) return;
    tbman_s_lock( o );
    tbman_s_release_arg_arr( o, down, arg, ptr, size, step );
    tbman_s_unlock( o );
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_tbman_s_references( bcore_tbman_s* o, vc_t ptr )
{
    if( !ptr ) return 0;
    tbman_s_lock( o );
    uz_t n = tbman_s_references( o, ptr );
    tbman_s_unlock( o );
    return n;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_s_b_alloc( bcore_tbman_s* o, vd_t current_ptr, uz_t requested_bytes, uz_t* granted_bytes )
{
    tbman_s_lock( o );
    vd_t ret = NULL;
    if( requested_bytes == 0 )
    {
        if( current_ptr )
        {
            tbman_s_mem_free( o, current_ptr, NULL );
        }
        if( granted_bytes ) *granted_bytes = 0;
    }
    else
    {
        if( current_ptr )
        {
            ret = tbman_s_mem_realloc( o, current_ptr, NULL, requested_bytes, granted_bytes );
        }
        else
        {
            ret = tbman_s_mem_alloc( o, requested_bytes, granted_bytes );
        }
    }
    tbman_s_unlock( o );
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_s_bn_alloc( bcore_tbman_s* o, vd_t current_ptr, uz_t current_bytes, uz_t requested_bytes, uz_t* granted_bytes )
{
    tbman_s_lock( o );
    vd_t ret = NULL;
    if( requested_bytes == 0 )
    {
        if( current_bytes ) // 0 means current_ptr may not be used for free or realloc
        {
            tbman_s_mem_free( o, current_ptr, &current_bytes );
        }
        if( granted_bytes ) *granted_bytes = 0;
    }
    else
    {
        if( current_bytes ) // 0 means current_ptr may not be used for free or realloc
        {
            ret = tbman_s_mem_realloc( o, current_ptr, &current_bytes, requested_bytes, granted_bytes );
        }
        else
        {
            ret = tbman_s_mem_alloc( o, requested_bytes, granted_bytes );
        }
    }
    tbman_s_unlock( o );
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_s_u_alloc( bcore_tbman_s* o, uz_t unit_bytes, vd_t current_ptr, uz_t requested_units, uz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_s_b_alloc( o, current_ptr, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_s_un_alloc( bcore_tbman_s* o, uz_t unit_bytes, vd_t current_ptr, uz_t current_units, uz_t requested_units, uz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_s_bn_alloc( o, current_ptr, unit_bytes * current_units, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_s_get_instance( bcore_tbman_s* o, vc_t ptr, vd_t* root, uz_t* granted_space )
{
    tbman_s_lock( o );
    vd_t block_ptr = bcore_btree_vd_s_largest_below_equal( o->internal_btree, ( vd_t )ptr );
    if( block_ptr && ( ( ptrdiff_t )( (u0_t*)ptr - (u0_t*)block_ptr ) < o->pool_size ) )
    {
        token_manager_s* token_manager = block_ptr;
        if( root ) *root = token_manager_s_root( token_manager, ptr );
        if( granted_space ) *granted_space = token_manager->block_size;
    }
    else
    {
        external_manager_s_get_instance( &o->external_manager, ptr, root, granted_space );
    }
    tbman_s_unlock( o );
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t tbman_s_internal_total_alloc( const bcore_tbman_s* o )
{
    uz_t sum = 0;
    for( uz_t i = 0; i < o->size; i++ )
    {
        sum += block_manager_s_total_alloc( o->data[ i ] );
    }
    return sum;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t tbman_s_internal_total_instances( const bcore_tbman_s* o )
{
    uz_t sum = 0;
    for( uz_t i = 0; i < o->size; i++ )
    {
        sum += block_manager_s_total_instances( o->data[ i ] );
    }
    return sum;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t tbman_s_internal_total_references( const bcore_tbman_s* o )
{
    uz_t sum = 0;
    for( uz_t i = 0; i < o->size; i++ )
    {
        sum += block_manager_s_total_references( o->data[ i ] );
    }
    return sum;
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t tbman_s_total_alloc( const bcore_tbman_s* o )
{
    return external_manager_s_total_alloc( &o->external_manager ) + tbman_s_internal_total_alloc( o );
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t tbman_s_total_instances( const bcore_tbman_s* o )
{
    return external_manager_s_total_instances( &o->external_manager ) + tbman_s_internal_total_instances( o );
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t tbman_s_total_references( const bcore_tbman_s* o )
{
    return external_manager_s_total_references( &o->external_manager ) + tbman_s_internal_total_references( o );
}

// ---------------------------------------------------------------------------------------------------------------------

static uz_t tbman_s_total_space( const bcore_tbman_s* o )
{
    uz_t sum = 0;
    for( uz_t i = 0; i < o->size; i++ )
    {
        sum += block_manager_s_total_space( o->data[ i ] );
    }
    sum += block_manager_s_total_space( &o->external_manager.bm_ext );
    return sum;
}

// ---------------------------------------------------------------------------------------------------------------------

static void tbman_s_for_each_instance( bcore_tbman_s* o, void (*cb)( vd_t arg, vd_t ptr, uz_t space ), vd_t arg )
{
    for( uz_t i = 0; i < o->size; i++ )
    {
        block_manager_s_for_each_instance( o->data[ i ], cb, arg );
    }
    external_manager_s_for_each_instance( &o->external_manager, cb, arg );
}

/**********************************************************************************************************************/
/**********************************************************************************************************************/

/**********************************************************************************************************************/
// Interface

// ---------------------------------------------------------------------------------------------------------------------

static bcore_tbman_s* tbman_s_g = NULL;

static void create_tbman()
{
    tbman_s_g = bcore_tbman_s_create_default();
}

// ---------------------------------------------------------------------------------------------------------------------

static void discard_tbman()
{
    bcore_tbman_s_discard( tbman_s_g );
    tbman_s_g = NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

static void tbman_open()
{
    static bcore_once_s flag = bcore_once_init;
    bcore_once_s_run( &flag, create_tbman );
}

// ---------------------------------------------------------------------------------------------------------------------

static void tbman_close()
{
    discard_tbman();
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_fork( vd_t ptr )
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_fork( tbman_s_g, ptr );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_release( vd_t ptr )
{
    assert( tbman_s_g != NULL );
    bcore_tbman_s_release( tbman_s_g, ptr );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_release_obj( fp_down_obj down, vd_t ptr )
{
    assert( tbman_s_g != NULL );
    bcore_tbman_s_release_obj( tbman_s_g, down, ptr );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_release_arg( fp_down_arg down, vc_t arg, vd_t ptr )
{
    assert( tbman_s_g != NULL );
    bcore_tbman_s_release_arg( tbman_s_g, down, arg, ptr );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_release_obj_arr( fp_down_obj down, vd_t ptr, uz_t size, uz_t step )
{
    assert( tbman_s_g != NULL );
    bcore_tbman_s_release_obj_arr( tbman_s_g, down, ptr, size, step );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_release_arg_arr( fp_down_arg down, vc_t arg, vd_t ptr, uz_t size, uz_t step )
{
    assert( tbman_s_g != NULL );
    bcore_tbman_s_release_arg_arr( tbman_s_g, down, arg, ptr, size, step );
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_tbman_references( vc_t ptr )
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_references( tbman_s_g, ptr );
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_b_alloc( vd_t current_ptr, uz_t requested_bytes, uz_t* granted_bytes )
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_b_alloc( tbman_s_g, current_ptr, requested_bytes, granted_bytes );
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_bn_alloc( vd_t current_ptr, uz_t current_bytes, uz_t requested_bytes, uz_t* granted_bytes )
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_bn_alloc( tbman_s_g, current_ptr, current_bytes, requested_bytes, granted_bytes );
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_u_alloc( uz_t unit_bytes, vd_t current_ptr, uz_t requested_units, uz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_b_alloc( current_ptr, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_un_alloc( uz_t unit_bytes, vd_t current_ptr, uz_t current_units, uz_t requested_units, uz_t* reserved_units )
{
    vd_t reserved_ptr = bcore_tbman_bn_alloc( current_ptr, unit_bytes * current_units, unit_bytes * requested_units, reserved_units );
    if( reserved_units ) *reserved_units /= ( unit_bytes > 0 ? unit_bytes : 1 );
    return reserved_ptr;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_get_instance( vc_t ptr, vd_t* root, uz_t* granted_space )
{
    bcore_tbman_s_get_instance( tbman_s_g, ptr, root, granted_space );
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_tbman_s_granted_space( bcore_tbman_s* o, vc_t ptr )
{
    uz_t space = 0;
    bcore_tbman_s_get_instance( o, ptr, NULL, &space );
    return space;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_tbman_s_total_granted_space( bcore_tbman_s* o )
{
    tbman_s_lock( o );
    uz_t space = tbman_s_total_alloc( o );
    tbman_s_unlock( o );
    return space;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_tbman_s_total_instances( bcore_tbman_s* o )
{
    tbman_s_lock( o );
    uz_t size = tbman_s_total_instances( o );
    tbman_s_unlock( o );
    return size;
}

// ---------------------------------------------------------------------------------------------------------------------

uz_t bcore_tbman_s_total_references( bcore_tbman_s* o )
{
    tbman_s_lock( o );
    uz_t size = tbman_s_total_references( o );
    tbman_s_unlock( o );
    return size;
}

// ---------------------------------------------------------------------------------------------------------------------

typedef struct bcore_tbman_mnode { vd_t p; uz_t s; } bcore_tbman_mnode;
typedef struct bcore_tbman_mnode_arr { bcore_tbman_mnode* data; uz_t size; uz_t space; } bcore_tbman_mnode_arr;

static void for_each_instance_collect_callback( vd_t arg, vd_t ptr, uz_t space )
{
    assert( arg );
    bcore_tbman_mnode_arr* arr = arg;
    assert( arr->size < arr->space );
    arr->data[ arr->size ] = ( bcore_tbman_mnode ){ .p = ptr, .s = space };
    arr->size++;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_tbman_s_for_each_instance( bcore_tbman_s* o, void (*cb)( vd_t arg, vd_t ptr, uz_t space ), vd_t arg )
{
    if( !cb ) return;
    sz_t size = bcore_tbman_s_total_instances( o );
    if( !size ) return;

    bcore_tbman_mnode_arr arr;
    arr.data  = malloc( sizeof( bcore_tbman_mnode ) * size );
    arr.space = size;
    arr.size  = 0;

    tbman_s_lock( o );
    tbman_s_for_each_instance( o, for_each_instance_collect_callback, &arr );
    tbman_s_unlock( o );

    assert( arr.size == arr.space );

    for( sz_t i = 0; i < size; i++ ) cb( arg, arr.data[ i ].p, arr.data[ i ].s );

    free( arr.data );
}

uz_t bcore_tbman_granted_space( vc_t ptr )
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_granted_space( tbman_s_g, ptr );
}

uz_t bcore_tbman_total_granted_space()
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_total_granted_space( tbman_s_g );
}

uz_t bcore_tbman_total_instances()
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_total_instances( tbman_s_g );
}

uz_t bcore_tbman_total_references()
{
    assert( tbman_s_g != NULL );
    return bcore_tbman_s_total_references( tbman_s_g );
}

void bcore_tbman_for_each_instance( void (*cb)( vd_t arg, vd_t ptr, uz_t space ), vd_t arg )
{
    assert( tbman_s_g != NULL );
    bcore_tbman_s_for_each_instance( tbman_s_g, cb, arg );
}

// not thread-safe
st_s* bcore_tbman_s_status( bcore_tbman_s* o, int detail_level )
{
    if( detail_level <= 0 ) return NULL;

    st_s* str = st_s_create();
    st_s_pushf( str, "main manager\n" );
    st_s_pushf( str, "  pool_size ........... %zu\n", o->pool_size );
    st_s_pushf( str, "  block managers ...... %zu\n", o->size );
    st_s_pushf( str, "  token managers ...... %zu\n", bcore_btree_vd_s_count( o->internal_btree, NULL, NULL ) );
    st_s_pushf( str, "  external allocs ..... %zu\n", bcore_btree_pp_s_count( o->external_manager.ex_btree, NULL, NULL ) );
    st_s_pushf( str, "  internal_btree depth  %zu\n", bcore_btree_vd_s_depth( o->internal_btree ) );
    st_s_pushf( str, "  external_btree depth  %zu\n", bcore_btree_pp_s_depth( o->external_manager.ex_btree ) );
    st_s_pushf( str, "  min_block_size ...... %zu\n", o->size > 0 ? o->data[ 0 ]->block_size : 0 );
    st_s_pushf( str, "  max_block_size ...... %zu\n", o->size > 0 ? o->data[ o->size - 1 ]->block_size : 0 );
    st_s_pushf( str, "  aligned ............. %s\n",  o->aligned ? "true" : "false" );

    st_s_pushf( str, "external manager\n" );
    st_s_pushf( str, "  pool_size ........... %zu\n", o->external_manager.pool_size );
    st_s_pushf( str, "  token managers ...... %zu\n", bcore_btree_vd_s_count( o->external_manager.tm_btree, NULL, NULL ) );
    st_s_pushf( str, "  aligned ............. %s\n",  o->external_manager.aligned ? "true" : "false" );
    st_s_pushf( str, "  item size ........... %zu\n", sizeof( ext_s ) );
    st_s_pushf( str, "  items ............... %zu\n", block_manager_s_total_alloc( &o->external_manager.bm_ext ) / sizeof( ext_s ) );
    st_s_pushf( str, "  alloc ............... %zu\n", block_manager_s_total_alloc( &o->external_manager.bm_ext ) );
    st_s_pushf( str, "  space ............... %zu\n", block_manager_s_total_space( &o->external_manager.bm_ext ) );

    st_s_pushf( str, "down manager\n" );
    st_s_pushf( str, "  pool_size ........... %zu\n", o->down_manager.pool_size );
    st_s_pushf( str, "  token managers ...... %zu\n", bcore_btree_vd_s_count( o->down_manager.tm_btree, NULL, NULL ) );
    st_s_pushf( str, "  aligned ............. %s\n",  o->down_manager.aligned ? "true" : "false" );
    st_s_pushf( str, "  down_obj_s size ..... %zu\n", sizeof( down_obj_s ) );
    st_s_pushf( str, "  down_obj_s items .... %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_obj ) / sizeof( down_obj_s ) );
    st_s_pushf( str, "  down_obj_s alloc .... %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_obj ) );
    st_s_pushf( str, "  down_obj_s space .... %zu\n", block_manager_s_total_space( &o->down_manager.bm_down_obj ) );
    st_s_pushf( str, "  down_arg_s size ..... %zu\n", sizeof( down_arg_s ) );
    st_s_pushf( str, "  down_arg_s items .... %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_arg ) / sizeof( down_arg_s ) );
    st_s_pushf( str, "  down_arg_s alloc .... %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_arg ) );
    st_s_pushf( str, "  down_arg_s space .... %zu\n", block_manager_s_total_space( &o->down_manager.bm_down_arg ) );
    st_s_pushf( str, "  down_obj_arr_s size   %zu\n", sizeof( down_obj_arr_s ) );
    st_s_pushf( str, "  down_obj_arr_s items  %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_obj_arr ) / sizeof( down_obj_arr_s ) );
    st_s_pushf( str, "  down_obj_arr_s alloc  %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_obj_arr ) );
    st_s_pushf( str, "  down_obj_arr_s space  %zu\n", block_manager_s_total_space( &o->down_manager.bm_down_obj_arr ) );
    st_s_pushf( str, "  down_arg_arr_s size   %zu\n", sizeof( down_arg_arr_s ) );
    st_s_pushf( str, "  down_arg_arr_s items  %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_arg_arr ) / sizeof( down_arg_arr_s ) );
    st_s_pushf( str, "  down_arg_arr_s alloc  %zu\n", block_manager_s_total_alloc( &o->down_manager.bm_down_arg_arr ) );
    st_s_pushf( str, "  down_arg_arr_s space  %zu\n", block_manager_s_total_space( &o->down_manager.bm_down_arg_arr ) );

    st_s_pushf( str, "overall allocations\n" );
    st_s_pushf( str, "  total bytes granted . %zu\n", tbman_s_total_alloc( o ) );
    st_s_pushf( str, "    external .......... %zu\n", external_manager_s_total_alloc( &o->external_manager ) );
    st_s_pushf( str, "    internal .......... %zu\n", tbman_s_internal_total_alloc( o ) );
    st_s_pushf( str, "  internal used ....... %zu\n", tbman_s_total_space( o ) );
    st_s_pushf( str, "  total instances ..... %zu\n", tbman_s_total_instances( o ) );
    st_s_pushf( str, "    external .......... %zu\n", external_manager_s_total_instances( &o->external_manager ) );
    st_s_pushf( str, "    internal .......... %zu\n", tbman_s_internal_total_instances( o ) );
    st_s_pushf( str, "  total references .... %zu\n", tbman_s_total_references( o ) );
    st_s_pushf( str, "    external .......... %zu\n", external_manager_s_total_references( &o->external_manager ) );
    st_s_pushf( str, "    internal .......... %zu\n", tbman_s_internal_total_references( o ) );
    if( detail_level > 1 )
    {
        for( uz_t i = 0; i < o->size; i++ )
        {
            st_s_pushf( str, "\nblock manager %zu:\n", i );
            st_s_push_st_d( str, block_manager_s_status( o->data[ i ], detail_level - 1 ) );
        }
    }

    return str;
}

#include <stdio.h>
static void instance_diagnostics( vd_t arg, vd_t ptr, uz_t space )
{
    printf( "ptr = %p, space = %5zu", ptr, space );
    if( bcore_name_try_name( ( ( tp_t* )ptr )[ 0 ] ) )
    {
        printf( ", aware of = '%s'", nameof( ( ( tp_t* )ptr )[ 0 ] ) );
        if( bcore_name_try_name( ( ( tp_t* )ptr )[ 1 ] ) )
        {
            printf( ", spect of = '%s'", nameof( ( ( tp_t* )ptr )[ 1 ] ) );
        }
    }
    printf( "\n" );
}

void bcore_tbman_s_instance_disgnostics( bcore_tbman_s* o )
{
    bcore_tbman_s_for_each_instance( o, instance_diagnostics, NULL );
}

void bcore_tbman_instance_disgnostics()
{
    bcore_tbman_for_each_instance( instance_diagnostics, NULL );
}

/**********************************************************************************************************************/
// Testing and evaluation

#include "bcore_spect_inst.h"

typedef vd_t (*fp_alloc)( vd_t current_ptr, uz_t current_bytes, uz_t requested_bytes, uz_t* granted_bytes );

// ---------------------------------------------------------------------------------------------------------------------

static st_s* tbman_alloc_challenge
(
    fp_alloc alloc,
    uz_t table_size,
    uz_t cycles,
    uz_t max_alloc,
    u2_t seed,
    bl_t cleanup,
    bl_t verbose
)
{
    st_s* str = st_s_create();

    vd_t* data_table = malloc( table_size * sizeof( vd_t ) );
    uz_t* size_table = malloc( table_size * sizeof( uz_t ) );
    for( uz_t i = 0; i < table_size; i++ ) data_table[ i ] = NULL;
    for( uz_t i = 0; i < table_size; i++ ) size_table[ i ] = 0;

    u2_t rval = seed;
    uz_t alloc_attempts = 0;
    uz_t realloc_attempts = 0;
    uz_t free_attempts = 0;
    uz_t alloc_failures = 0;
    uz_t realloc_failures = 0;
    uz_t free_failures = 0;

    for( uz_t j = 0; j < cycles; j++ )
    {
        for( uz_t i = 0; i < table_size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            uz_t idx = rval % table_size;

            // verify table content
            if( size_table[ idx ] > 0 )
            {
                u2_t rv = bcore_xsg_u2( idx + 1 );
                uint8_t* data = data_table[ idx ];
                uz_t sz = size_table[ idx ];
                for( uz_t i = 0; i < sz; i++ )
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
                uz_t size = pow( ( double )max_alloc, ( rval * pow( 2.0, -32 ) ) );
                data_table[ idx ] = alloc( data_table[ idx ], 0, size, &size_table[ idx ] );
                alloc_attempts++;
                alloc_failures += ( size > 0 ) && ( data_table[ idx ] == NULL );

                // set new content
                if( size_table[ idx ] > 0 )
                {
                    u2_t rv = bcore_xsg_u2( idx + 1 );
                    uint8_t* data = data_table[ idx ];
                    uz_t sz = size_table[ idx ];
                    for( uz_t i = 0; i < sz; i++ ) data[ i ] = ( ( rv = bcore_xsg_u2( rv ) ) & 255 );
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
                    uz_t size = pow( ( double )max_alloc, rval * pow( 2.0, -32 ) );

                    uz_t new_size = 0;

                    data_table[ idx ] = alloc( data_table[ idx ], size_table[ idx ], size, &new_size ); // realloc

                    // verify old table content (only when size > sz - stdlib realloc does not seem to retain data otherwise)
                    if( size > size_table[ idx ] )
                    {
                        if( data_table[ idx ] != NULL && size_table[ idx ] > 0 )
                        {
                            u2_t rv = bcore_xsg_u2( idx + 1 );
                            uint8_t* data = data_table[ idx ];
                            uz_t sz = size_table[ idx ];
                            for( uz_t i = 0; i < sz; i++ )
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
                        uz_t sz = size_table[ idx ];
                        for( uz_t i = 0; i < sz; i++ ) data[ i ] = ( ( rv = bcore_xsg_u2( rv ) ) & 255 );
                    }
                }
            }
        }
    }

    uz_t allocated_table_size = 0;
    for( uz_t i = 0; i < table_size; i++ ) allocated_table_size += ( data_table[ i ] != NULL );

    if( verbose )
    {
        st_s_pushf( str, "cycles ............... %zu\n", cycles );
        st_s_pushf( str, "max alloc ............ %zu\n", max_alloc );
        st_s_pushf( str, "Instances\n" );
        st_s_pushf( str, "  total .............. %zu\n", table_size );
        st_s_pushf( str, "  allocated .......... %zu\n", allocated_table_size );
        st_s_pushf( str, "Alloc\n" );
        st_s_pushf( str, "  attempts  .......... %zu\n", alloc_attempts );
        st_s_pushf( str, "  failures  .......... %zu\n", alloc_failures );
        st_s_pushf( str, "Realloc\n" );
        st_s_pushf( str, "  attempts  .......... %zu\n", realloc_attempts );
        st_s_pushf( str, "  failures  .......... %zu\n", realloc_failures );
        st_s_pushf( str, "Free\n" );
        st_s_pushf( str, "  attempts  .......... %zu\n", free_attempts );
        st_s_pushf( str, "  failures  .......... %zu\n", free_failures );
    }

    // speed test
    {
        clock_t time = clock();
        for( uz_t j = 0; j < cycles; j++ )
        {
            for( uz_t i = 0; i < table_size; i++ )
            {
                rval = bcore_xsg_u2( rval );
                uz_t idx = rval % table_size;
                if( data_table[ idx ] == NULL )
                {
                    uz_t size = pow( ( double )max_alloc, rval * pow( 2.0, -32 ) );
                    data_table[ idx ] = alloc( data_table[ idx ], 0, size, &size_table[ idx ] );
                }
                else
                {
                    data_table[ idx ] = alloc( data_table[ idx ], size_table[ idx ], 0, &size_table[ idx ] ); // free
                }
            }
        }
        time = clock() - time;
        st_s_pushf( str, "speed test alloc-free: %5.3gs\n", ( double )time / ( CLOCKS_PER_SEC  * cycles  * table_size ) );
    }
    // speed test
    {
        clock_t time = clock();
        for( uz_t j = 0; j < cycles; j++ )
        {
            for( uz_t i = 0; i < table_size; i++ )
            {
                rval = bcore_xsg_u2( rval );
                uz_t idx = rval % table_size;
                rval = bcore_xsg_u2( rval );
                uz_t size = pow( ( double )max_alloc, rval * pow( 2.0, -32 ) );
                data_table[ idx ] = alloc( data_table[ idx ], size_table[ idx ], size, &size_table[ idx ] ); // realloc
            }
        }
        time = clock() - time;
        st_s_pushf( str, "speed test realloc:    %5.3gs\n", ( double )time / ( CLOCKS_PER_SEC  * cycles  * table_size ) );
    }

    // cleanup
    if( cleanup ) for( uz_t i = 0; i < table_size; i++ ) data_table[ i ] = alloc( data_table[ i ], size_table[ i ], 0, NULL );

    free( size_table );
    free( data_table );
    return str;
}

// ---------------------------------------------------------------------------------------------------------------------

static void tbman_s_quicktest( void )
{
    uz_t table_size = 200;
    uz_t cycles     = 1;
    uz_t max_alloc  = 4096;
    uz_t seed       = 5479;
    st_s_discard( tbman_alloc_challenge( bcore_tbman_bn_alloc, table_size, cycles, max_alloc, seed, true, false ) );
}

// ---------------------------------------------------------------------------------------------------------------------

/// testing reference control
static st_s* tbman_s_rctest( void )
{
    st_s* log = st_s_createf( "== tbman_s_rctest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    typedef struct myclass
    {
        st_s  name1;
        st_s* name2;
        u3_t  v1;
        u3_t* v2;
    } myclass;

    tp_t t_myclass = bcore_flect_type_parse_sc( "{ st_s name1; st_s* name2; u3_t v1; u3_t* v2; }" );
    bcore_inst_t_discard( t_myclass, bcore_inst_t_create( t_myclass ) ); // cycle once to get all perspectives alive

    myclass* obj1 = bcore_inst_t_create( t_myclass );
    myclass* obj2 = bcore_inst_t_create( t_myclass );

    uz_t arr_size = 100000;
    myclass* obj_arr = bcore_tbman_malloc( sizeof( myclass ) * arr_size );
    for( uz_t i = 0; i < arr_size; i++ )
    {
        bcore_inst_t_init( t_myclass, (bcore_inst*)&obj_arr[ i ] );
        st_s_pushf( &obj_arr[ i ].name1, "Hi!" );
        obj_arr[ i ].name2 = st_s_createf( "Ho!" );
    }

    obj_arr = bcore_tbman_fork( obj_arr );
    bcore_tbman_release_arg_arr( ( fp_down_arg )bcore_inst_p_down, bcore_inst_s_get_typed( t_myclass ), obj_arr, arr_size, sizeof( myclass ) );

    vd_t* ref_arr1 = bcore_tbman_u_alloc( sizeof( vd_t ), NULL, arr_size, NULL );
    vd_t* ref_arr2 = bcore_tbman_u_alloc( sizeof( vd_t ), NULL, arr_size, NULL );

    {
        clock_t time = clock();
        for( uz_t i = 0; i < arr_size; i++ )
        {
            ref_arr1[ i ] = bcore_tbman_fork( &obj_arr[ i ].name1 );
        }
        time = clock() - time;
        st_s_pushf( log, "fork 1 ..... %zuns\n", ( uz_t )( ( ( double )time * 1E9 ) / ( CLOCKS_PER_SEC  * arr_size ) ) );
    }

    {
        clock_t time = clock();
        for( uz_t i = 0; i < arr_size; i++ )
        {
            ref_arr2[ i ] = bcore_tbman_fork( &obj_arr[ i ].name1 );
        }
        time = clock() - time;
        st_s_pushf( log, "fork 2 ..... %zuns\n", ( uz_t )( ( ( double )time * 1E9 ) / ( CLOCKS_PER_SEC  * arr_size ) ) );
    }

    st_s* obj2_name1 = bcore_tbman_fork( &obj2->name1 );

    st_s_pushf( obj2_name1, "Hi!" );

    bcore_tbman_release_arg( ( fp_down_arg )bcore_inst_p_down, bcore_inst_s_get_typed( t_myclass ), obj1 );
    bcore_tbman_release_arg( ( fp_down_arg )bcore_inst_p_down, bcore_inst_s_get_typed( t_myclass ), obj2 );

    bcore_tbman_release( obj2_name1 );

    st_s_pushf( log, "obj_arr references %zu\n", bcore_tbman_references( obj_arr ) );

    {
        clock_t time = clock();
        for( uz_t i = 0; i < arr_size; i++ )
        {
            bcore_tbman_release( ref_arr1[ i ] );
        }
        time = clock() - time;
        st_s_pushf( log, "release 1 .. %zuns\n", ( uz_t )( ( ( double )time * 1E9 ) / ( CLOCKS_PER_SEC  * arr_size ) ) );
    }

    {
        clock_t time = clock();
        for( uz_t i = 0; i < arr_size; i++ )
        {
            bcore_tbman_release( ref_arr2[ i ] );
        }
        time = clock() - time;
        st_s_pushf( log, "release 2 .. %zuns\n", ( uz_t )( ( ( double )time * 1E9 ) / ( CLOCKS_PER_SEC  * arr_size ) ) );
    }

    st_s_pushf( log, "obj_arr references %zu\n", bcore_tbman_references( obj_arr ) );

    bcore_tbman_release( obj_arr );

    st_s_pushf( log, "obj_arr references %zu\n", bcore_tbman_references( obj_arr ) );

    bcore_tbman_release( ref_arr1 );
    bcore_tbman_release( ref_arr2 );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );

    st_s_push_st_d( log, bcore_tbman_s_status( tbman_s_g, 1 ) );
    return log;
}

// ---------------------------------------------------------------------------------------------------------------------

static st_s* tbman_s_memtest( void )
{
    st_s* log = st_s_createf( "== tbman_s_memtest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    uz_t table_size = 100000;
    uz_t cycles     = 5;
    uz_t max_alloc  = 65536;
    uz_t seed       = 1237;

    {
        st_s_pushf( log, "\nbcore_internal_alloc:\n");
        st_s_push_st_d( log, tbman_alloc_challenge( bcore_tbman_bn_alloc, table_size, cycles, max_alloc, seed, true, false ) );
        st_s_push_st_d( log, bcore_tbman_s_status( tbman_s_g, 1 ) );
//        st_s_push_st_d( log, bcore_btree_vd_s_status( m->internal_btree ) );

    }

    {
        st_s_pushf( log, "\nbcore_external_alloc:\n");
        st_s_push_st_d( log, tbman_alloc_challenge( bcore_tbman_external_bn_alloc, table_size, cycles, max_alloc, seed, true, false ) );
    }

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t tbman_s_thread_func( vd_t arg )
{
    st_s* log = st_s_create();
    uz_t table_size = 10000;
    uz_t cycles     = 5;
    uz_t max_alloc  = 65536;
    uz_t seed       = 1237;
    st_s_push_st_d( log, tbman_alloc_challenge( bcore_tbman_bn_alloc, table_size, cycles, max_alloc, seed, true, false ) );
//    st_s_push_st_d( log, tbman_alloc_challenge( bcore_tbman_external_bn_alloc, table_size, cycles, max_alloc, seed, true, false ) );
    return log;
}

// ---------------------------------------------------------------------------------------------------------------------

static st_s* tbman_s_thread_test( void )
{
    st_s* log = st_s_create();

    bcore_thread_s th = bcore_thread_call( tbman_s_thread_func, NULL );

    {
        st_s* log_l = bcore_thread_join( th );
        st_s_push_fa( log, "single thread:\n#<st_s*>\n", log_l );
        st_s_discard( log_l );
    }

    bcore_thread_s th1 = bcore_thread_call( tbman_s_thread_func, NULL );
    bcore_thread_s th2 = bcore_thread_call( tbman_s_thread_func, NULL );
    bcore_thread_s th3 = bcore_thread_call( tbman_s_thread_func, NULL );
    bcore_thread_s th4 = bcore_thread_call( tbman_s_thread_func, NULL );


    {
        st_s* log_l = bcore_thread_join( th1 );
        st_s_push_fa( log, "thread1:\n#<st_s*>\n", log_l );
        st_s_discard( log_l );
    }

    {
        st_s* log_l = bcore_thread_join( th2 );
        st_s_push_fa( log, "thread2:\n#<st_s*>\n", log_l );
        st_s_discard( log_l );
    }

    {
        st_s* log_l = bcore_thread_join( th3 );
        st_s_push_fa( log, "thread3:\n#<st_s*>\n", log_l );
        st_s_discard( log_l );
    }

    {
        st_s* log_l = bcore_thread_join( th4 );
        st_s_push_fa( log, "thread4:\n#<st_s*>\n", log_l );
        st_s_discard( log_l );
    }

    return log;
}

// ---------------------------------------------------------------------------------------------------------------------

static void tbman_s_instance_test( void )
{
    for( uz_t size = 1; size < 10000000; size *= 2 )
    {
        uz_t space = 0;
        vd_t ptr = bcore_tbman_bn_alloc( NULL, 0, size, &space );
        uz_t instance_space = 0;
        vd_t instance_root = NULL;
        bcore_tbman_get_instance( ( u0_t* )ptr + ( size >> 1 ), &instance_root, &instance_space );
        ASSERT( instance_root == ptr );
        ASSERT( instance_space == space );
        bcore_tbman_free( ptr );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

typedef struct diagnostic_s { bcore_tbman_s* man; vd_t* ptr_arr; uz_t* spc_arr; sz_t size; } diagnostic_s;

static void tbman_s_diagnostic_test_callback( vd_t arg, vd_t ptr, uz_t space )
{
    diagnostic_s* d = arg;
    bl_t found = false;
    for( sz_t i = 0; i < d->size; i++ )
    {
        if( ptr == d->ptr_arr[ i ] )
        {
            found = true;
            ASSERT( space == d->spc_arr[ i ] );
        }
    }
    ASSERT( found );
    bcore_tbman_s_b_alloc( d->man, ptr, 0, NULL );
}

static void tbman_s_diagnostic_test( void )
{
    diagnostic_s diag;
    diag.man     = bcore_tbman_s_create_default();
    diag.size    = 1000;
    diag.ptr_arr = bcore_malloc( sizeof( vd_t ) * diag.size );
    diag.spc_arr = bcore_malloc( sizeof( sz_t ) * diag.size );

    u2_t rval = 1234;

    for( sz_t i = 0; i < diag.size; i++ )
    {
        rval = bcore_xsg_u2( rval );
        sz_t size = rval % 20000;
        diag.ptr_arr[ i ] = bcore_tbman_s_b_alloc( diag.man, NULL, size, &diag.spc_arr[ i ] );
    }

    ASSERT( bcore_tbman_s_total_instances( diag.man ) == diag.size );

    // the callback function frees memory
    bcore_tbman_s_for_each_instance( diag.man, tbman_s_diagnostic_test_callback, &diag );

    ASSERT( bcore_tbman_s_total_granted_space( diag.man ) == 0 );
    ASSERT( bcore_tbman_s_total_instances(     diag.man ) == 0 );

    diag.ptr_arr = bcore_free( diag.ptr_arr );
    diag.spc_arr = bcore_free( diag.spc_arr );

    bcore_tbman_s_discard( diag.man );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// signal

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_tbman_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_tbman" ) ) )
    {
        case TYPEOF_init0:
        {
            tbman_open();
        }
        break;

        case TYPEOF_init1:
        {
            tbman_s_quicktest(); // system critical test
        }
        break;

        case TYPEOF_down0:
        {
            uz_t space = bcore_tbman_total_granted_space();
            if( space > 0 )
            {
                uz_t instances = bcore_tbman_total_instances();
                uz_t references = bcore_tbman_total_references();
                st_s_print_d( bcore_tbman_s_status( tbman_s_g, 1 ) );
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
        break;

        case TYPEOF_selftest:
        {
            st_s* log = st_s_create();
            tbman_s_diagnostic_test();
            tbman_s_instance_test();
            st_s_push_st_d( log, tbman_s_thread_test() );
            st_s_push_st_d( log, tbman_s_rctest() );
            st_s_push_st_d( log, tbman_s_memtest() );
            return log;
        }
        break;

        default: break;
    }
    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
