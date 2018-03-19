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

/** threads.h, threads.c principally pthreads-functionality according to
 *  beth coding convention.
 *  Some pthread functionality may also be altered or hidden in view of future
 *  adaptation to platforms without pthread support.
 */

#ifndef BCORE_THREADS_H
#define BCORE_THREADS_H

#include <pthread.h>
#include "bcore_types.h"
#include "bcore_features.h"

/**********************************************************************************************************************/
// mutex

typedef struct bcore_mutex_s
{
    pthread_mutex_t _mutex; // do not access directly
} bcore_mutex_s;

void           bcore_mutex_s_init( bcore_mutex_s* o );
void           bcore_mutex_s_down( bcore_mutex_s* o );
bcore_mutex_s* bcore_mutex_s_create();
void           bcore_mutex_s_discard( bcore_mutex_s* o );

void bcore_mutex_s_lock(   bcore_mutex_s* o );
void bcore_mutex_s_unlock( bcore_mutex_s* o );

/**********************************************************************************************************************/
// once

typedef pthread_once_t bcore_once_s;
#define bcore_once_init PTHREAD_ONCE_INIT
void bcore_once_s_run( bcore_once_s* flag, void (*func)() );

/**********************************************************************************************************************/
// condition

typedef struct bcore_condition_s
{
    pthread_cond_t _cond; // do not access directly
} bcore_condition_s;

void               bcore_condition_s_init( bcore_condition_s* o );
void               bcore_condition_s_down( bcore_condition_s* o );
bcore_condition_s* bcore_condition_s_create();
void               bcore_condition_s_discard( bcore_condition_s* o );

/// waits for trigger from another thread (via signal_one or signal_all); (a mutex-lock must have been obtained before)
void bcore_condition_s_wait( bcore_condition_s* o, bcore_mutex_s* mutex );

/// triggers at least one waiting thread
void bcore_condition_s_trigger_one( bcore_condition_s* o );

/// triggers all waiting threads
void bcore_condition_s_trigger_all( bcore_condition_s* o );

/**********************************************************************************************************************/
// thread

typedef struct bcore_thread_s
{
    pthread_t _thread; // do not access directly
} bcore_thread_s;

typedef struct bcore_thread_s bcore_pthread_s;

void            bcore_thread_s_init( bcore_thread_s* o );
void            bcore_thread_s_down( bcore_thread_s* o );
void            bcore_thread_s_copy( bcore_thread_s* o, const bcore_thread_s* src );

bcore_thread_s* bcore_thread_s_create();
void            bcore_thread_s_discard(     bcore_thread_s* o );
bcore_thread_s* bcore_thread_s_clone( const bcore_thread_s* o );

/// calls func( arg ) in a new thread
void bcore_thread_s_call( bcore_thread_s* o, vd_t ( *func )( vd_t ), vd_t arg );

/// waits for func( arg ) to finish and returns its return value
vd_t bcore_thread_s_join( bcore_thread_s* o );

/// calls func( arg ) in a new thread, which is returned (return value can be discarded or reused without shutting down or re-init)
bcore_thread_s bcore_thread_call( vd_t ( *func )( vd_t ), vd_t arg );

/// waits for func( arg ) to finish and returns its return value; (o can be discarded or reused without shutting down or re-init)
vd_t bcore_thread_join( bcore_thread_s o );

/**********************************************************************************************************************/

vd_t bcore_threads_signal_handler( const bcore_signal_s* o );

#endif // BCORE_THREADS_H

