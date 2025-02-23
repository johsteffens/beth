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

/**
 *  X Threads. Replaces bcore_threads.
 */

#ifndef BCORE_X_THREADS_H
#define BCORE_X_THREADS_H

#include <pthread.h>
#include <sys/types.h>
#include "bcore_types.h"
#include "bcore_feature.h"
#include "bcore_x_inst.h"
#include "bcore.xo.h"

// prototype for getting thread id
pid_t gettid( void );

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x, x_inst )

#ifdef XOILA_SECTION

forward :mutex_s;
identifier gettid;

//----------------------------------------------------------------------------------------------------------------------

// locks a mutex during lifetime
stamp :lock_s
{
    private :mutex_s * mutex;

    func o _( m@* o, m :mutex_s* mutex ) = o.set( mutex );

    func o set( m@* o, m :mutex_s* mutex )
    {
        o.release();
        o.mutex = mutex;
        o.mutex.lock();
    }

    func o release( m@* o )
    {
        if( o.mutex ) o.mutex.unlock();
        o.mutex = NULL;
    }

    func bcore_inst_call.down_e o.release();
}

//----------------------------------------------------------------------------------------------------------------------

// unlocks a mutex during lifetime
stamp :unlock_s
{
    private :mutex_s * mutex;

    func o _( m@* o, m :mutex_s* mutex ) = o.set( mutex );

    func o set( m@* o, m :mutex_s* mutex )
    {
        o.release();
        o.mutex = mutex;
        o.mutex.unlock();
    }

    func o release( m@* o )
    {
        if( o.mutex ) o.mutex.lock();
        o.mutex = NULL;
    }

    func bcore_inst_call.down_e o.release();
}

//----------------------------------------------------------------------------------------------------------------------

stamp :mutex_s
{
    private pthread_mutex_t _mutex; // do not access directly
    func bcore_inst_call.init_x;
    func bcore_inst_call.down_e;
    func void lock  (m@*o);
    func void unlock(m@*o);
    func d :lock_s*   create_lock(   m@* o ) = :lock_s!.set( o );
    func d :unlock_s* create_unlock( m@* o ) = :unlock_s!.set( o );
}

//----------------------------------------------------------------------------------------------------------------------

stamp :condition_s
{
    private pthread_cond_t _cond; // do not access directly
    func bcore_inst_call.init_x;
    func bcore_inst_call.down_e;

    /** sleep: Suspends calling thread until woken from another thread.
     *  This function binds the condition to the mutex until return.
     *  The function exits only when receiving a 'wakeup' signal.
     *  A wakeup signal can be triggered by one of the wake functions below.
     *  The mutex passed as argument must be locked.
     *  The function unlocks the mutex upon entering and re-locks upon exiting.
     *
     *  Beware of spurious wakeups:
     *     A wakeup signal can also spuriously occur (e.g. on specific system events) without
     *     an explicit call to a wake function below.
     *
     *  This function properly employs a memory barrier.
     */
    func void sleep( m@* o, m x_mutex_s* mutex );

    /// wakes at least one thread if sleeping on this condition
    func void wake_one( m@* o );

    /// wakes all threads sleeping on this condition
    func void wake_all( m@* o );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// thread

//----------------------------------------------------------------------------------------------------------------------

group :thread
{
    group :result {};

    /// Overload one of these functions in the thread-functor:
    feature (TResult) :result* m_thread_func( m@* o );
    feature (TResult) :result* c_thread_func( c@* o );

    /** Thread Controller.
     *  Joins automatically on shut-down.
     */
    stamp :s =
    {
        private pthread_t _thread; // do not access directly
        bl_t _join;   // true in case thread needs joining
        func bcore_inst_call.down_e;

        /** Calls obj.m_thread_func() or obj.c_thread_func() in a new joinable thread
         *  If o is unjoined, it is joined first.
         */
        func o call_m_thread_func( m@* o, m aware :* obj );

        /** Calls obj.c_thread_func() in a new joinable thread
         *  If o is unjoined, it is joined first.
         */
        func o call_c_thread_func( m@* o, c aware :* obj );

        /** Waits for obj.m_thread_func or obj.m_thread_func to finish and returns its result (reentrant).
         *  In case the thread has already been joined (or is not joinable), the function
         *  returns NULL immediately.
         *  Synchronizes memory (employs a memory barrier).
         */
        func c (TResult) :result* join( m@* o );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// other thread related functions

//----------------------------------------------------------------------------------------------------------------------

/// suspends current thread for a period given in ...
func void threads_sleep_ns( u3_t ns ); /// ... nanoseconds
func void threads_sleep_us( u3_t us ); /// ... microseconds
func void threads_sleep_ms( u3_t ms ); /// ... milliseconds

//----------------------------------------------------------------------------------------------------------------------

/// returns the id of the current thread (fast)
func tp_t threads_get_id() = gettid();

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

vd_t bcore_x_threads_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_X_THREADS_H
