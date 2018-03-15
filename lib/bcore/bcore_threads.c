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

void bcore_mutex_init( bcore_mutex_t* o )
{
    int ern = pthread_mutex_init( o, NULL );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_mutex_down( bcore_mutex_t* o )
{
    int ern = pthread_mutex_destroy( o );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_mutex_lock( bcore_mutex_t* o )
{
    int ern = pthread_mutex_lock( o );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_mutex_unlock( bcore_mutex_t* o )
{
    int ern = pthread_mutex_unlock( o );
    if( ern ) ERR( "function returned error %i", ern );
}

void bcore_once( bcore_once_t* flag, void (*func)() )
{
    int ern = pthread_once( flag, func );
    if( ern ) ERR( "function returned error %i", ern );
}

vd_t bcore_threads_signal_handler( const bcore_signal_s* o )
{
    return NULL;
}


