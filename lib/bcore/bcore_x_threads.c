/** Author and Copyright 2021 Johannes Bernhard Steffens
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

#include "bcore_x_threads.h"
#include <errno.h>

/**********************************************************************************************************************/
// mutex

//----------------------------------------------------------------------------------------------------------------------

void x_mutex_s_init_x( x_mutex_s* o )
{
    int ern = pthread_mutex_init( &o->_mutex, NULL );
    if( ern == EAGAIN ) ERR( "Resources exceeded." );
    if( ern == ENOMEM ) ERR( "Memory exhausted." );
    if( ern == EPERM  ) ERR( "Unprivileged call." );
    if( ern ) ERR( "function returned error %i", ern );
}

//----------------------------------------------------------------------------------------------------------------------

void x_mutex_s_down_e( x_mutex_s* o )
{
    int ern = pthread_mutex_destroy( &o->_mutex );
    if( ern == EBUSY  ) ERR( "Mutex is locked." );
    if( ern == EINVAL ) ERR( "Invalid mutex." );
    if( ern ) ERR( "function returned error %i", ern );
}

//----------------------------------------------------------------------------------------------------------------------

void x_mutex_s_lock( x_mutex_s* o )
{
    int ern = pthread_mutex_lock( &o->_mutex );
    if( ern ) ERR( "function returned error %i", ern );
}

//----------------------------------------------------------------------------------------------------------------------

void x_mutex_s_unlock( x_mutex_s* o )
{
    int ern = pthread_mutex_unlock( &o->_mutex );
    if( ern ) ERR( "function returned error %i", ern );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// condition

//----------------------------------------------------------------------------------------------------------------------

void x_condition_s_init_x( x_condition_s* o )
{
    int ern = pthread_cond_init( &o->_cond, NULL );
    if( ern ) ERR( "function returned error %i", ern );
}

//----------------------------------------------------------------------------------------------------------------------

void x_condition_s_down_e( x_condition_s* o )
{
    int ern = pthread_cond_destroy( &o->_cond );
    if( ern ) ERR( "function returned error %i", ern );
}

//----------------------------------------------------------------------------------------------------------------------

void x_condition_s_sleep( x_condition_s* o, x_mutex_s* mutex )
{
    int ern = pthread_cond_wait( &o->_cond, &mutex->_mutex );
    if( ern ) ERR( "function returned error %i", ern );
}

//----------------------------------------------------------------------------------------------------------------------

void x_condition_s_wake_one( x_condition_s* o )
{
    int ern = pthread_cond_signal( &o->_cond );
    if( ern ) ERR( "function returned error %i", ern );
}

//----------------------------------------------------------------------------------------------------------------------

void x_condition_s_wake_all( x_condition_s* o )
{
    int ern = pthread_cond_broadcast( &o->_cond );
    if( ern ) ERR( "function returned error %i", ern );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// thread

//----------------------------------------------------------------------------------------------------------------------

void x_thread_s_down_e( x_thread_s* o )
{
    x_thread_s_join( o );
}

//----------------------------------------------------------------------------------------------------------------------

static vd_t x_m_thread_func( vd_t thread_obj )
{
    if( x_thread_defines_m_thread_func( thread_obj ) )
    {
        return ( vd_t )x_thread_a_m_thread_func( thread_obj );
    }
    else if( x_thread_defines_c_thread_func( thread_obj ) )
    {
        return ( vd_t )x_thread_a_c_thread_func( thread_obj );
    }
    else
    {
        return NULL;
    }
}

//----------------------------------------------------------------------------------------------------------------------

static vd_t x_c_thread_func( vd_t thread_obj )
{
    if( x_thread_defines_c_thread_func( thread_obj ) )
    {
        return ( vd_t )x_thread_a_c_thread_func( thread_obj );
    }
    else
    {
        return NULL;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void x_thread_s_call_m_thread_func( x_thread_s* o, x_thread* thread_obj )
{
    x_thread_s_join( o );
    int ern = pthread_create( &o->_thread, NULL, x_m_thread_func, thread_obj );
    if( ern ) ERR( "function returned error %i", ern );
    o->_join = true;
}

//----------------------------------------------------------------------------------------------------------------------

void x_thread_s_call_c_thread_func( x_thread_s* o, const x_thread* thread_obj )
{
    x_thread_s_join( o );
    int ern = pthread_create( &o->_thread, NULL, x_c_thread_func, ( vd_t )thread_obj );
    if( ern ) ERR( "function returned error %i", ern );
    o->_join = true;
}

//----------------------------------------------------------------------------------------------------------------------

const x_inst* x_thread_s_join( x_thread_s* o )
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

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_x_threads_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_x_threads" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_TYPE( root, pthread_mutex_t );
            BCORE_REGISTER_TYPE( root, pthread_cond_t );
            BCORE_REGISTER_TYPE( root, pthread_t );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

