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

/// Threads handling (based on pthreads)

#ifndef BCORE_THREADS_H
#define BCORE_THREADS_H

#include <pthread.h>
#include "bcore_types.h"
#include "bcore_feature.h"

/**********************************************************************************************************************/
// mutex

typedef struct bcore_mutex_s
{
    pthread_mutex_t _mutex; // do not access directly
} bcore_mutex_s;

void           bcore_mutex_s_init( bcore_mutex_s* o );
void           bcore_mutex_s_down( bcore_mutex_s* o );
void           bcore_mutex_s_copy( bcore_mutex_s* o, const bcore_mutex_s* src ); // does not do anything

bcore_mutex_s* bcore_mutex_s_create();
void           bcore_mutex_s_discard( bcore_mutex_s* o );
bcore_mutex_s* bcore_mutex_s_clone( const bcore_mutex_s* src ); // clone behaves like create (src is not evaluated)

void bcore_mutex_s_lock(   bcore_mutex_s* o );
void bcore_mutex_s_unlock( bcore_mutex_s* o );

/**********************************************************************************************************************/
// once

typedef pthread_once_t bcore_once_s;
#define bcore_once_init PTHREAD_ONCE_INIT
void bcore_once_s_run( bcore_once_s* flag, void (*func)( void ) );

/**********************************************************************************************************************/
// condition

typedef struct bcore_condition_s
{
    pthread_cond_t _cond; // do not access directly
} bcore_condition_s;

void               bcore_condition_s_init( bcore_condition_s* o );
void               bcore_condition_s_down( bcore_condition_s* o );
void               bcore_condition_s_copy( bcore_condition_s* o, const bcore_condition_s* src ); // does not do anything
bcore_condition_s* bcore_condition_s_create();
void               bcore_condition_s_discard( bcore_condition_s* o );
bcore_condition_s* bcore_condition_s_clone( const bcore_condition_s* src ); // clone behaves like create (src is not evaluated)

/** Sleeps until woken from another thread.
 *  A mutex-lock must have been obtained before. The mutex is unlocked
 *  by during sleep and re-locked when woken up.
 *  Beware of spurious wakeups: A wakeup may happen spuriously without
 *  actual cause from wake-trigger send to this condition.
 */
void bcore_condition_s_sleep( bcore_condition_s* o, bcore_mutex_s* mutex );

/// wakes at least one thread if sleeping on this condition
void bcore_condition_s_wake_one( bcore_condition_s* o );

/// wakes all threads sleeping on this condition
void bcore_condition_s_wake_all( bcore_condition_s* o );

/**********************************************************************************************************************/
// thread

typedef vd_t ( *bcore_fp_thread )( vd_t );

#define TYPEOF_bcore_thread_s typeof( "bcore_thread_s" )
typedef struct bcore_thread_s
{
    pthread_t _thread; // do not access directly
    bl_t      _join;   // true in case thread needs joining
} bcore_thread_s;

typedef struct bcore_thread_s bcore_pthread_s;

void            bcore_thread_s_init( bcore_thread_s* o );
void            bcore_thread_s_down( bcore_thread_s* o ); // joins automatically if required
void            bcore_thread_s_copy( bcore_thread_s* o, const bcore_thread_s* src ); // does not do anything

bcore_thread_s* bcore_thread_s_create();
void            bcore_thread_s_discard(     bcore_thread_s* o );
bcore_thread_s* bcore_thread_s_clone( const bcore_thread_s* src ); // clone behaves like create (src is not evaluated)

/** Calls func( arg ) in a new joinable thread
 *  If o is unjoined, it is joined first.
 */
void bcore_thread_s_call( bcore_thread_s* o, bcore_fp_thread func, vd_t arg );

/** Waits for func( arg ) to finish and returns its return value (reentrant)
 *  In case the thread has already been joined (or is not joinable), the function
 *  returns NULL immediately.
 */
vd_t bcore_thread_s_join( bcore_thread_s* o );

/// calls func( arg ) in a new thread, which is returned
bcore_thread_s bcore_thread_call( bcore_fp_thread func, vd_t arg );

/// waits for func( arg ) to finish and returns its return value
vd_t bcore_thread_join( bcore_thread_s o );

/**********************************************************************************************************************/
// thread array

#define TYPEOF_bcore_thread_arr_s typeof( "bcore_thread_arr_s" )
typedef struct bcore_thread_arr_s bcore_thread_arr_s;

void bcore_thread_arr_s_init( bcore_thread_arr_s* o );
void bcore_thread_arr_s_down( bcore_thread_arr_s* o );
void bcore_thread_arr_s_copy( bcore_thread_arr_s* o, const bcore_thread_arr_s* src );
bcore_thread_arr_s* bcore_thread_arr_s_create();
void                bcore_thread_arr_s_discard(     bcore_thread_arr_s* o );
bcore_thread_arr_s* bcore_thread_arr_s_clone( const bcore_thread_arr_s* o );

uz_t            bcore_thread_arr_s_get_size( const bcore_thread_arr_s* o );
bcore_thread_s* bcore_thread_arr_s_get_thread( bcore_thread_arr_s* o, uz_t index );

/// pushes a new thread; returns index of pushed thread
uz_t bcore_thread_arr_s_push_call( bcore_thread_arr_s* o, bcore_fp_thread func, vd_t arg );

/// joins last thread and removes it. returns join-result
vd_t bcore_thread_arr_s_join_pop( bcore_thread_arr_s* o );

/**********************************************************************************************************************/
// other thread related functions

/// suspends current thread for period ns given in ...
void bcore_sleep_ns( uz_t ns ); /// ... nanoseconds
void bcore_sleep_us( uz_t us ); /// ... microseconds
void bcore_sleep_ms( uz_t ms ); /// ... milliseconds

/**********************************************************************************************************************/


vd_t bcore_threads_signal_handler( const bcore_signal_s* o );

#endif // BCORE_THREADS_H

