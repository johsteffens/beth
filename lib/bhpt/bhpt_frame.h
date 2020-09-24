/** Author and Copyright 2020 Johannes Bernhard Steffens
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

#ifndef BHPT_FRAME_H
#define BHPT_FRAME_H

/**********************************************************************************************************************/

/** Global Interface Sketch of this Library*/

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_std.h"
#include "bhvm_std.h"
#include "bhpt_sketch.h"
#include "bhpt_xoila_out.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bhpt_frame, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

group :thread =
{
    /// shared object in a thread base
    stamp :share = aware :
    {
        aware bhpt_tutor    -> tutor;
        aware bhpt_adaptive -> adaptive;

        bcore_condition_s    => condition_item;
        bcore_mutex_s        => mutex; // mutex for share

        sz_t finished_count  = 0;      // number is incremented by item when a task was finished
    };

    signature vd_t loop(       mutable ); // thread function
    signature void loop_enter( mutable );
    signature void loop_exit(  mutable );
    signature void wait_while_locked( mutable ); // lock-unlock in sequence (used to test if the item is unlocked and waits otherwise)


    stamp :item = aware :
    {
        bl_t running = false;
        sz_t prime_cycles = 0; // number of prime cycles yet to be executed
        bcore_thread_s       => thread;
        bcore_mutex_s        => mutex;
        :share_s             -> share;
        aware bhpt_adaptive  => adaptive; // local adaptive

        func bcore_inst_call : down_e = { ASSERT( !o->running ); };

        func : :loop;
        func : :loop_enter;
        func : :loop_exit;
        func : :wait_while_locked = { bcore_mutex_s_lock( o->mutex ); bcore_mutex_s_unlock( o->mutex ); };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    stamp :ads = aware bcore_array { :item_s []; };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    signature void tsetup( mutable, sz_t threads, bhpt_adaptive* adaptive, bhpt_tutor* tutor ); // thread function
    signature void tdown( mutable ); // thread function
    signature void run( mutable, sz_t cycles_per_thread );

    stamp :base = aware :
    {
        :ads_s ads;
        :share_s => share;

        func : :tsetup;
        func : :tdown;
        func : :run;

        func bcore_inst_call : down_e = { @_tdown( o ); };
        func bcore_inst_call : copy_e = { @_tdown( o ); };
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :state = aware :
{
    sz_t cycle_number;
    sz_t last_cycle_adapt;
    sz_t last_cycle_test;
    sz_t last_cycle_backup;
    aware bhpt_adaptive => adaptive;
    bhpt_adaptor_adl_s => adaptor_adl;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// Adaptive framework
stamp : = aware bcore_main
{
    /// ========= parameters ================
    aware bhpt_tutor   => tutor;
    hidden :thread_base_s => thread_base;

    private bcore_main_frame_s* main_frame;

    // A single single cycle represents an adaptive primimg

    // number of threads to run a minibatch (0 means single threaded)
    sz_t threads = 1;

    /** Requested cycle number at which the network is adapted.
     *  The actual number is the nearest within [1, cycle_adapt]
     *  divisible by max( threads, 1 )
     */
    sz_t cycle_adapt  = 1;

    // cycle number at which the network is tested
    sz_t cycle_test   = 1000;

    // cycle number at which a new backup is generated
    sz_t cycle_backup = 1000;

    // cycle number at which training ends
    sz_t cycle_finish = 1000000;

    // log verbosity
    sz_t verbosity = 1;

    /// ========= end parameters  ==========

    /// current state of frame
    hidden :state_s => state;

    /// statistics
    hidden bhvm_stats_s stats_grad;

    st_s state_path; // backup file for state

    hidden aware bcore_sink -> log;

    func bcore_main : main;

    func bcore_main : exit_required = { return bcore_main_frame_s_exit_required( o->main_frame ); };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t bhpt_frame_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHPT_FRAME_H
