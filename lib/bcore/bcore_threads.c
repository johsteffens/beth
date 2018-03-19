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

#include "bcore_threads.h"
#include "bcore_control.h"
#include "bcore_flect.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/
// mutex

void bcore_mutex_s_init( bcore_mutex_s* o )
{
    int ern = pthread_mutex_init( &o->_mutex, NULL );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_mutex_s_down( bcore_mutex_s* o )
{
    int ern = pthread_mutex_destroy( &o->_mutex );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_mutex_s_copy( bcore_mutex_s* o, const bcore_mutex_s* src )
{
    ERR( "bcore_mutex_s cannot be copied" );
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

static bcore_flect_self_s* bcore_mutex_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_mutex_s = { ... }", sizeof( bcore_mutex_s ) );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_mutex_s_init, "bcore_fp_init", "init" );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_mutex_s_down, "bcore_fp_down", "down" );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_mutex_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

/**********************************************************************************************************************/
// once

void bcore_once_s_run( bcore_once_s* flag, void (*func)() )
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
    ERR( "bcore_condition_s cannot be copied" );
}

BCORE_DEFINE_FUNCTIONS_CDC( bcore_condition_s )

void bcore_condition_s_wait( bcore_condition_s* o, bcore_mutex_s* mutex )
{
    int ern = pthread_cond_wait( &o->_cond, &mutex->_mutex );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_condition_s_trigger_one( bcore_condition_s* o )
{
    int ern = pthread_cond_signal( &o->_cond );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_condition_s_trigger_all( bcore_condition_s* o )
{
    int ern = pthread_cond_broadcast( &o->_cond );
    if( ern ) ERR( "function returned error %i", ern );
}

static bcore_flect_self_s* bcore_condition_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_condition_s = { ... }", sizeof( bcore_condition_s ) );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_condition_s_init, "bcore_fp_init", "init" );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_condition_s_down, "bcore_fp_down", "down" );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_condition_s_copy, "bcore_fp_copy", "copy" );
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
    // nothing to do
}

void bcore_thread_s_copy( bcore_thread_s* o, const bcore_thread_s* src )
{
    bcore_memcpy( o, src, sizeof( *o ) );
}

BCORE_DEFINE_FUNCTIONS_CDC( bcore_thread_s )

static bcore_flect_self_s* bcore_thread_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bcore_thread_s = { ... }", sizeof( bcore_thread_s ) );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_thread_s_init, "bcore_fp_init", "init" );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_thread_s_down, "bcore_fp_down", "down" );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_thread_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

void bcore_thread_s_call( bcore_thread_s* o, vd_t ( *func)( vd_t ), vd_t arg )
{
    int ern = pthread_create( &o->_thread, NULL, func, arg );
    if( ern ) ERR( "function returned error %i", ern );
}

bcore_thread_s bcore_thread_call( vd_t ( *func )( vd_t ), vd_t arg )
{
    bcore_thread_s thread;
    bcore_thread_s_init( &thread );
    bcore_thread_s_call( &thread, func, arg );
    return thread;
}

vd_t bcore_thread_s_join( bcore_thread_s* o )
{
    vd_t ret = NULL;
    int ern = pthread_join( o->_thread, &ret );
    if( ern ) ERR( "function returned error %i", ern );
    return ret;
}

vd_t bcore_thread_join( bcore_thread_s o )
{
    vd_t ret = NULL;
    int ern = pthread_join( o._thread, &ret );
    if( ern ) ERR( "function returned error %i", ern );
    return ret;
}

/**********************************************************************************************************************/

vd_t bcore_threads_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_threads" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FLECT( bcore_mutex_s );
            BCORE_REGISTER_FLECT( bcore_condition_s );
            BCORE_REGISTER_FLECT( bcore_thread_s );
        }
        break;

        default: break;
    }

    return NULL;
}

