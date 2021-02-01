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

/**********************************************************************************************************************/

/// Adaptive Framework

include 'h' "bmath_std.h";
include 'h' "bhvm_std.h";

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

group :thread =
{
    /// shared object in a thread base
    stamp :share_s = aware :
    {
        aware bhpt_tutor    -> tutor;
        aware bhpt_adaptive -> adaptive;

        bcore_condition_s    => condition_item;
        bcore_mutex_s        => mutex; // mutex for share

        sz_t finished_count  = 0;      // number is incremented by item when a task was finished
    };

    signature vd_t loop(       m @* o ); // thread function
    signature void loop_enter( m @* o );
    signature void loop_exit(  m @* o );
    signature void wait_while_locked( m @* o ); // lock-unlock in sequence (used to test if the item is unlocked and waits otherwise)

    stamp :item_s = aware :
    {
        bl_t running = false;
        sz_t prime_cycles = 0; // number of prime cycles yet to be executed
        bcore_thread_s       => thread;
        bcore_mutex_s        => mutex;
        :share_s             -> share;
        aware bhpt_adaptive  => adaptive; // local adaptive

        func bcore_inst_call.down_e = { ASSERT( !o.running ); };

        func :.loop;
        func :.loop_enter;
        func :.loop_exit;
        func :.wait_while_locked = { o.mutex.lock(); o.mutex.unlock(); };
    };

    // -----------------------------------------------------------------------------------------------------------------

    stamp :ads_s = aware x_array
    {
        :item_s [];
        wrap x_array.set_size;
        wrap x_array.clear;
    };

    // -----------------------------------------------------------------------------------------------------------------

    signature void tsetup( m @* o, sz_t threads, m bhpt_adaptive* adaptive, m bhpt_tutor* tutor ); // thread function
    signature void tdown( m @* o ); // thread function
    signature void run( m @* o, sz_t cycles_per_thread );

    stamp :base_s = aware :
    {
        :ads_s ads;
        :share_s => share;

        func :.tsetup;
        func :.tdown;
        func :.run;

        func bcore_inst_call.down_e = { o.tdown(); };
        func bcore_inst_call.copy_e = { o.tdown(); };
    };
};

// ---------------------------------------------------------------------------------------------------------------------

stamp :state_s = aware :
{
    sz_t cycle_number;
    sz_t last_cycle_adapt;
    sz_t last_cycle_test;
    sz_t last_cycle_backup;
    aware bhpt_adaptive => adaptive;
    bhpt_adaptor_adl_s => adaptor_adl;
    bhpt_test_result_adl_s => test_result_adl;

    func bcore_main.main;
};

// ---------------------------------------------------------------------------------------------------------------------

/// Adaptive framework
stamp :s = aware bcore_main
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

    func bcore_main.main;

    func bcore_main.exit_required = { return o.main_frame.exit_required(); };

};

// ---------------------------------------------------------------------------------------------------------------------

func (:thread_item_s) :.loop =
{
    m $* probe_item  = bhpt_adaptor_probe_s!^;
    m $* probe_share = bhpt_adaptor_probe_s!^;

    o.mutex.lock();

    while( o.running )
    {
        if( o.prime_cycles > 0 )
        {
            o.adaptive.get_adaptor_probe( probe_item );
            probe_item.zro_grad();

            for( sz_t i = 0; i < o.prime_cycles; i++ ) o.share.tutor.prime( o.adaptive );

            o.adaptive.get_adaptor_probe( probe_item );
            o.share.mutex.lock();
            o.share.adaptive.get_adaptor_probe( probe_share );
            probe_share.acc_grad( probe_item );
            o.share.finished_count++;
            o.share.mutex.unlock();
            o.prime_cycles = 0;
        }

        o.share.condition_item.sleep( o.mutex );
    }

    o.mutex.unlock();
    return NULL;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:thread_item_s) :.loop_enter =
{
    o.loop_exit(); // exit if not yet exited

    o.thread!;
    o.mutex!;
    ASSERT( o.share.condition_item );

    o.mutex.lock();
    o.running = true;
    o.mutex.unlock();

    verbatim_C
    {
        bcore_thread_s_call( o->thread, ( bcore_fp_thread )bhpt_frame_thread_item_s_loop, o );
    }
};

// ---------------------------------------------------------------------------------------------------------------------

func (:thread_item_s) :.loop_exit =
{
    if( !o.mutex ) return;
    o.mutex.lock();
    o.running = false;
    o.mutex.unlock();
};

// ---------------------------------------------------------------------------------------------------------------------

func (:thread_base_s) :.tsetup =
{
    if( o.ads.size > 0 ) o.tdown();
    o.share =< bhpt_frame_thread_share_s!;
    o.share.condition_item =< bcore_condition_s!;
    o.share.mutex =< bcore_mutex_s!;
    o.share.adaptive =< adaptive.fork();
    o.share.tutor =< tutor.fork();
    o.ads.set_size( threads );

    m $* probe_item  = bhpt_adaptor_probe_s!^;
    m $* probe_share = adaptive.get_adaptor_probe( bhpt_adaptor_probe_s!^ );

    foreach( m $* e in o.ads )
    {
        e.adaptive = adaptive.clone();
        e.adaptive.get_adaptor_probe( probe_item );
        probe_item.rebind_axon( probe_share );
        e.adaptive.rebind_holors();
        e.share = o.share.fork();
    }

    foreach( m $* e in o.ads ) e.loop_enter();
};

// ---------------------------------------------------------------------------------------------------------------------

func (:thread_base_s) :.tdown =
{
    if( !o.share ) return;
    foreach( m $* e in o.ads ) e.loop_exit();
    o.share.condition_item.wake_all();
    o.ads.clear();
    o.share.condition_item =< NULL;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:thread_base_s) :.run =
{
    o.share.mutex.lock();
    o.share.finished_count = 0;
    o.share.mutex.unlock();

    foreach( m $* e in o.ads )
    {
        e.mutex.lock();
        e.prime_cycles = cycles_per_thread;
        e.mutex.unlock();
    }

    o.share.condition_item.wake_all();

    for( bl_t busy = true; busy; )
    {
        bcore_sleep_us( 10 );

        /// obtaining item-locks reduces idling cycles
        foreach( m$* e in o.ads ) e.wait_while_locked();

        o.share.mutex.lock();
        busy = o.share.finished_count < o.ads.size;
        o.share.mutex.unlock();
    }
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) (void adapt( m @* o )) =
{
    m bhpt_adaptor_adl_s*   adaptor_adl = o.state.adaptor_adl;
    m bhpt_adaptive*        adaptive = o.state.adaptive;
    m bhpt_adaptor_probe_s* probe = adaptive.get_adaptor_probe( bhpt_adaptor_probe_s!^ );

    ASSERT( probe.size == adaptor_adl.size );

    m bhvm_stats_s* stats_grad = bhvm_stats_s!^;
    probe.acc_stats( NULL, stats_grad );
    o.stats_grad.acc_stats( stats_grad );

    foreach( m$* e in adaptor_adl ) e.adapt( probe.[ __i ] );
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) (void backup( m@* o )) =
{
    sc_t path = o.state_path.sc;
    if( path[ 0 ] ) o.state.to_file_bin_ml( path );
};

// ---------------------------------------------------------------------------------------------------------------------

stamp :test_result_s = aware bhpt_test_result
{
    sz_t cycle_number;
    aware bhpt_test_result => test_result;
    bhvm_stats_s => stats_axon;
    bhvm_stats_s => stats_grad;

    func bhpt_test_result.to_sink =
    {
        if( verbosity == 0 ) return o;
        sink.push_fa( "#pl10 {#<sz_t>}: ", o.cycle_number );

        if( o.test_result )
        {
            o.test_result.to_sink( verbosity, sink );
        }

        if( o.stats_axon )
        {
            sink.push_fa( ", axon{ " );
            o.stats_axon.to_sink( sink );
            sink.push_fa( " }" );
        }

        if( o.stats_grad )
        {
            sink.push_fa( ", grad{ " );
            o.stats_grad.to_sink( sink );
            sink.push_fa( " }" );
        }

        return o;
    };
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) (void test( m@* o )) =
{
    :test_result_s^ test_result;
    test_result.cycle_number = o.state.cycle_number;
    test_result.test_result =< o.tutor.test( o.state.adaptive );
    o.state.adaptive.get_adaptor_probe( bhpt_adaptor_probe_s!^ ).acc_stats( test_result.stats_axon!, NULL );
    test_result.stats_grad =< o.stats_grad.clone();

    if( o.verbosity > 0 )
    {
        test_result.to_sink( o.verbosity, o.log );
        o.log.push_fa( " }\n" );
    }

    o.state.test_result_adl!.push_c( test_result );

    o.stats_grad.clear();
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) (void run_single_threaded( m@* o )) =
{
    m bhpt_frame_state_s* state = o.state;

    sz_t cycle_adapt = sz_max( o.cycle_adapt, 1 );

    while( state.cycle_number < o.cycle_finish && !o.exit_required() )
    {
        if( state.cycle_number == 0 || state.cycle_number - state.last_cycle_test >= o.cycle_test )
        {
            state.last_cycle_test = state.cycle_number;
            o.test();
        }

        o.tutor.prime( state.adaptive );
        state.cycle_number++;

        if( state.cycle_number - state.last_cycle_adapt >= cycle_adapt )
        {
            state.last_cycle_adapt = state.cycle_number;
            o.adapt();
        }

        if( state.cycle_number - state.last_cycle_backup >= o.cycle_backup )
        {
            state.last_cycle_backup = state.cycle_number;
            o.backup();
        }
    }
};

// ---------------------------------------------------------------------------------------------------------------------

/// multi threaded run
func (:s) (void run_multi_threaded( m@* o )) =
{
    m bhpt_frame_state_s* state = o.state;
    o.thread_base!.tsetup( o.threads, state.adaptive, o.tutor );

    sz_t prime_cycles_per_thread = sz_max( o->cycle_adapt / sz_max( o.threads, 1 ), 1 );
    sz_t cycle_adapt = o.threads * prime_cycles_per_thread;

    while( state.cycle_number < o.cycle_finish && !o.exit_required() )
    {
        if( state.cycle_number == 0 || state.cycle_number - state.last_cycle_test >= o.cycle_test )
        {
            state.last_cycle_test = state.cycle_number;
            o.test();
        }

        o.thread_base.run( prime_cycles_per_thread );

        state.cycle_number += cycle_adapt;
        o.adapt();
        state.last_cycle_adapt = state.cycle_number;

        if( state.cycle_number - state.last_cycle_backup >= o.cycle_backup )
        {
            state.last_cycle_backup = state.cycle_number;
            o.backup();
        }
    }

    o.thread_base =< NULL;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) (void help_to_sink( m bcore_sink* sink )) =
{
    sink.push_fa( "-help:  Prints this help information.\n" );
    sink.push_fa( "-reset: Resets training. Discards previous training results.\n" );
    sink.push_fa( "-continue: (Default) Continues training from last saved training state.\n" );
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) bcore_main.main = (try)
{
    o.main_frame = frame;

    bl_t reset = false;

    if( !o.log ) o.log = bcore_fork( BCORE_STDOUT );

    c bcore_arr_st_s* args = frame.args;

    if( args )
    {
        foreach( c st_s* arg in args )
        {
            if( __i < 2 )
            {
                if( __i == 1 ) o.state_path.push_fa( "#<sc_t>.state", arg.sc );
            }
            else if( arg.equal_sc( "-help" ) )
            {
                o.help_to_sink( BCORE_STDOUT );
                return 0;
            }
            else if( arg.equal_sc( "-reset" ) )
            {
                reset = true;
            }
            else if( arg.equal_sc( "-continue" ) )
            {
                reset = false;
            }
            else
            {
                return bcore_error_push_fa( TYPEOF_general_error, "Invalid argument '#<sc_t>'", arg.sc );
            }
        }
    }

    if( !reset )
    {
        sc_t path = o.state_path.sc;
        if( path[ 0 ] && bcore_file_exists( path ) )
        {
            o.state =< bhpt_frame_state_s!.from_file_bin_ml( path );
            o.log.push_fa( "State recovered from '#<sc_t>'\n", path );
        }
        else
        {
            reset = true;
        }
    }

    assert( o.tutor );

    if( reset )
    {
        o.state =< bhpt_frame_state_s!;
        o.state.adaptive = o.tutor.create_adaptive();

        // setting up adaptors
        o.state.adaptor_adl =< bhpt_adaptor_adl_s!;
        m $* probe = bhpt_adaptor_probe_s!^;
        o.state.adaptive.get_adaptor_probe( probe );
        o.state.adaptor_adl.set_size( probe.size );
        foreach( m $.2 e in o.state.adaptor_adl ) e.1 = o.tutor.create_adaptor();
    }

    assert( o.state );
    assert( o.state.adaptive );

    o.tutor.status_to_sink( o.verbosity, o.log );
    o.log.push_fa( "\n" );
    o.state.adaptive.status_to_sink( o.verbosity, o.log );
    o.log.push_fa( "\n" );

    if( o.threads < 1 )
    {
        o.run_single_threaded();
    }
    else
    {
        o.run_multi_threaded();
    }

    if( o.exit_required() )
    {
        o.log.push_fa( "\nSaving state at cycle #<sz_t>\n", o->state->cycle_number );
        o.backup();
        o.log.push_fa( "Exiting cleanly.\n" );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:state_s) (void help_to_sink( m bcore_sink* sink )) =
{
    sink.push_fa( "-help:  Prints this help information.\n" );
    sink.push_fa( "-table [-h] name [...]: creates a comma separated utf8-table of numeric array items. -h: outputs header line.\n" );
};

// ---------------------------------------------------------------------------------------------------------------------

func (:state_s) (void table_to_sink( c bcore_hmap_name_s* hmap_name, c bcore_arr_tp_s* arr_tp, bl_t header, m bcore_sink* sink )) =
{

};

// ---------------------------------------------------------------------------------------------------------------------

func (:state_s) bcore_main.main =
{
    c bcore_arr_st_s* args = frame.args;
    if( /*frame.args.size == 0 && */ o.test_result_adl )
    {
        if( args.size > 2 )
        {
            if( args.[ 2 ].equal_sc( "-help" ) )
            {
                o.help_to_sink( BCORE_STDOUT );
                return 0;
            }
            else if( args.[ 2 ].equal_sc( "-table" ) )
            {
                bl_t header = false;
                sz_t idx = 2;
                if( idx < args.size && args.[ idx ].equal_sc( "-h" ) ) { idx++; header = true; }
                if( args.size == idx ) return bcore_error_push_fa( TYPEOF_general_error, "Element name expected." );
                bcore_hmap_name_s^ hmap_name;
                bcore_arr_tp_s^ arr_tp;
                for( ; idx < args.size; idx++ ) arr_tp.push( hmap_name.set_sc( args.[idx].sc ) );
                o.table_to_sink( hmap_name, arr_tp, header, BCORE_STDOUT );
                return 0;
            }
        }


        o.test_result_adl.to_sink( 1, BCORE_STDOUT );
        bcore_sink_a_push_fa( BCORE_STDOUT, "Current cycle number: #<sz_t>\n", o.cycle_number );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
