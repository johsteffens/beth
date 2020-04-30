/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_threads.h"
#include "bcore_control.h"
#include "bcore_flect.h"
#include "bcore_signal.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"

#include <errno.h>
#include <time.h>

/**********************************************************************************************************************/
// mutex

void bcore_mutex_s_init( bcore_mutex_s* o )
{
    bcore_memzero( o, sizeof( *o  ) );
    int ern = pthread_mutex_init( &o->_mutex, NULL );
    if( ern == EAGAIN ) ERR( "Resources exceeded." );
    if( ern == ENOMEM ) ERR( "Memory exhaused." );
    if( ern == EPERM  ) ERR( "Unpriviledged call." );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_mutex_s_down( bcore_mutex_s* o )
{
    int ern = pthread_mutex_destroy( &o->_mutex );
    if( ern == EBUSY  ) ERR( "Mutex is locked." );
    if( ern == EINVAL ) ERR( "Invalid mutex." );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_mutex_s_copy( bcore_mutex_s* o, const bcore_mutex_s* src )
{
    // nothing to do
}

void bcore_mutex_s_attach( bcore_mutex_s** o, bcore_mutex_s* src )
{
    if( !o ) return;
    bcore_mutex_s_discard( *o );
    *o = src;
}

void bcore_mutex_s_detach( bcore_mutex_s** o )
{
    if( o )
    {
        bcore_mutex_s_discard( *o );
        *o = NULL;
    }
}

BCORE_DEFINE_FUNCTIONS_CDC( bcore_mutex_s )

void bcore_mutex_s_lock( bcore_mutex_s* o )
{
    int ern = pthread_mutex_lock( &o->_mutex );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_mutex_s_unlock( bcore_mutex_s* o )
{
    int ern = pthread_mutex_unlock( &o->_mutex );
    if( ern ) ERR( "function returned error %i", ern );
}

static bcore_self_s* bcore_mutex_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_mutex_s = { ... }", bcore_mutex_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_mutex_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_mutex_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_mutex_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

/**********************************************************************************************************************/
// once

void bcore_once_s_run( bcore_once_s* flag, void (*func)( void ) )
{
    int ern = pthread_once( flag, func );
    if( ern ) ERR( "function returned error %i", ern );
}

/**********************************************************************************************************************/
// condition

void bcore_condition_s_init( bcore_condition_s* o )
{
    int ern = pthread_cond_init( &o->_cond, NULL );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_condition_s_down( bcore_condition_s* o )
{
    int ern = pthread_cond_destroy( &o->_cond );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_condition_s_copy( bcore_condition_s* o, const bcore_condition_s* src )
{
    // nothing to do
}

void bcore_condition_s_attach( bcore_condition_s** o, bcore_condition_s* src )
{
    if( !o ) return;
    bcore_condition_s_discard( *o );
    *o = src;
}

void bcore_condition_s_detach( bcore_condition_s** o )
{
    if( o )
    {
        bcore_condition_s_discard( *o );
        *o = NULL;
    }
}

BCORE_DEFINE_FUNCTIONS_CDC( bcore_condition_s )

void bcore_condition_s_sleep( bcore_condition_s* o, bcore_mutex_s* mutex )
{
    int ern = pthread_cond_wait( &o->_cond, &mutex->_mutex );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_condition_s_wake_one( bcore_condition_s* o )
{
    int ern = pthread_cond_signal( &o->_cond );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_condition_s_wake_all( bcore_condition_s* o )
{
    int ern = pthread_cond_broadcast( &o->_cond );
    if( ern ) ERR( "function returned error %i", ern );
}

static bcore_self_s* bcore_condition_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_condition_s = { ... }", bcore_condition_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_condition_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_condition_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_condition_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

/**********************************************************************************************************************/
// thread

void bcore_thread_s_init( bcore_thread_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void bcore_thread_s_down( bcore_thread_s* o )
{
    bcore_thread_s_join( o );
}

void bcore_thread_s_copy( bcore_thread_s* o, const bcore_thread_s* src )
{
    // nothing to do
}

void bcore_thread_s_attach( bcore_thread_s** o, bcore_thread_s* src )
{
    if( !o ) return;
    bcore_thread_s_discard( *o );
    *o = src;
}

void bcore_thread_s_detach( bcore_thread_s** o )
{
    if( o )
    {
        bcore_thread_s_discard( *o );
        *o = NULL;
    }
}

BCORE_DEFINE_FUNCTIONS_CDC( bcore_thread_s )

static bcore_self_s* bcore_thread_s_create_self( void )
{
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( "bcore_thread_s = { ... }", bcore_thread_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_thread_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_thread_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_thread_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

void bcore_thread_s_call( bcore_thread_s* o, bcore_fp_thread func, vd_t arg )
{
    bcore_thread_s_join( o );
    int ern = pthread_create( &o->_thread, NULL, func, arg );
    if( ern ) ERR( "function returned error %i", ern );
    o->_join = true;
}

bcore_thread_s bcore_thread_call( bcore_fp_thread func, vd_t arg )
{
    bcore_thread_s thread;
    bcore_thread_s_init( &thread );
    bcore_thread_s_call( &thread, func, arg );
    return thread;
}

vd_t bcore_thread_s_join( bcore_thread_s* o )
{
    vd_t ret = NULL;
    if( o->_join )
    {
        int ern = pthread_join( o->_thread, &ret );
        if( ern == EINVAL )  ERR( "No joinable thread" );
        if( ern == ESRCH )   ERR( "Invalid thread id" );
        if( ern == EDEADLK ) ERR( "Deadlock detected." );
        if( ern ) ERR( "function returned error %i", ern );
        o->_join = false;
    }
    return ret;
}

vd_t bcore_thread_join( bcore_thread_s o )
{
    return bcore_thread_s_join( &o );
}

/**********************************************************************************************************************/
/// bcore_thread_arr_s

typedef struct bcore_thread_arr_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            bcore_thread_s* data;
            uz_t size, space;
        };
    };
} bcore_thread_arr_s;

BCORE_DEFINE_FUNCTIONS_SELF_OBJECT_INST( bcore_thread_arr_s, "bcore_thread_arr_s = bcore_inst { aware_t _; bcore_thread_s [] arr; }" )

void bcore_thread_arr_s_attach( bcore_thread_arr_s** o, bcore_thread_arr_s* src )
{
    if( !o ) return;
    bcore_thread_arr_s_discard( *o );
    *o = src;
}

void bcore_thread_arr_s_detach( bcore_thread_arr_s** o )
{
    if( o )
    {
        bcore_thread_arr_s_discard( *o );
        *o = NULL;
    }
}

uz_t bcore_thread_arr_s_get_size( const bcore_thread_arr_s* o )
{
    return o->size;
}

bcore_thread_s* bcore_thread_arr_s_get_thread( bcore_thread_arr_s* o, uz_t index )
{
    assert( index < o->size );
    return &o->data[ index ];
}

uz_t bcore_thread_arr_s_push_call( bcore_thread_arr_s* o, bcore_fp_thread func, vd_t arg )
{
    bcore_thread_s* thread = bcore_thread_s_create();
    bcore_thread_s_call( thread, func, arg );
    bcore_array_a_push( (bcore_array*)o, sr_tsd( TYPEOF_bcore_thread_s, thread ) );
    return o->size - 1;
}

vd_t bcore_thread_arr_s_join_pop( bcore_thread_arr_s* o )
{
    if( o->size == 0 ) return NULL;
    bcore_thread_s* thread = bcore_thread_arr_s_get_thread( o, o->size - 1 );
    vd_t ret = bcore_thread_s_join( thread );
    bcore_array_a_pop( (bcore_array*)o );
    return ret;
}

/**********************************************************************************************************************/

void bcore_sleep_ns( u3_t ns )
{
    struct timespec ts = { 0 };
    ts.tv_sec  = ns / 1000000000;
    ts.tv_nsec = ns % 1000000000;

    // it seems the nanosleep prototype is not necessarily defined in <time.h>
    // so we do it here.
    int nanosleep( const struct timespec* requested, struct timespec* remaining );

    nanosleep( &ts, NULL );
}

void bcore_sleep_us( u3_t us )
{
    bcore_sleep_ns( us * 1000 );
}

void bcore_sleep_ms( u3_t ms )
{
    bcore_sleep_ns( ms * 1000000 );
}

/**********************************************************************************************************************/

vd_t bcore_threads_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_threads" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bcore_mutex_s );
            BCORE_REGISTER_OBJECT( bcore_condition_s );
            BCORE_REGISTER_OBJECT( bcore_thread_s );
            BCORE_REGISTER_OBJECT( bcore_thread_arr_s );
        }
        break;

        default: break;
    }

    return NULL;
}

