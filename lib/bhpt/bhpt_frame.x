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
    sz_t cycle;
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

    /// ========= semi parameters  =========

    /// current state of frame
    :state_s => state;

    /// ========= end parameters  ==========

    /// statistics
    hidden bhvm_stats_s stats_grad;

    st_s state_path; // backup file for state

    hidden aware bcore_sink -> log;

    hidden bl_t flag_exit_required;
    hidden bl_t flag_suspend_requested;
    hidden bl_t flag_interrupt_requested;
    hidden bcore_mutex_s flag_mutex;

    func (void run_training_single_threaded( m@* o ));
    func (void run_training_multi_threaded( m@* o ));
    func (void run_training( m@* o ));
    func (bl_t exit_training( m@* o )) = { o.flag_mutex.create_lock()^; return o.flag_suspend_requested || o.flag_interrupt_requested || o.flag_exit_required; };

    func bcore_main.main;
    func bcore_shell.op_group = { return :op~; };

    func bcore_main.on_suspend     = { o.flag_mutex.create_lock()^; o.flag_suspend_requested   = true; return true; };
    func bcore_main.on_interrupt   = { o.flag_mutex.create_lock()^; o.flag_interrupt_requested = true; o.flag_exit_required = true; return true; };
    func bcore_main.on_termination = { o.flag_mutex.create_lock()^; o.flag_exit_required       = true; return true; };

    func (void clear_flags( m @* o )) = { o.flag_mutex.create_lock()^; o.flag_suspend_requested = o.flag_interrupt_requested = o.flag_exit_required = false; };

    func ( bl_t exit_required( @* o ) ) = { o.cast( m $* ).flag_mutex.create_lock()^; return o.flag_exit_required; };
};

// ---------------------------------------------------------------------------------------------------------------------

func (:thread_item_s) :.loop =
{
    m $* probe_item  = bhpt_adaptor_probe_s!^;
    m $* probe_share = bhpt_adaptor_probe_s!^;

    o.mutex.create_lock()^;

    while( o.running )
    {
        if( o.prime_cycles > 0 )
        {
            o.adaptive.get_adaptor_probe( probe_item );
            probe_item.zro_grad();

            for( sz_t i = 0; i < o.prime_cycles; i++ ) o.share.tutor.prime( o.adaptive );

            o.adaptive.get_adaptor_probe( probe_item );

            {
                o.share.mutex.create_lock()^;
                o.share.adaptive.get_adaptor_probe( probe_share );
                probe_share.acc_grad( probe_item );
                o.share.finished_count++;
            }

            o.prime_cycles = 0;
        }

        o.share.condition_item.sleep( o.mutex );
    }

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
        bcore_lock_s^ _.set( e.mutex );
        e.prime_cycles = cycles_per_thread;
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
    sz_t cycle;
    aware bhpt_test_result => test_result;
    bhvm_stats_s => stats_axon;
    bhvm_stats_s => stats_grad;

    func bhpt_test_result.to_sink =
    {
        if( verbosity == 0 ) return o;
        sink.push_fa( "#pl10 {#<sz_t>}: ", o.cycle );

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
    test_result.cycle = o.state.cycle;
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

func (:s) run_training_single_threaded =
{
    m bhpt_frame_state_s* state = o.state;

    sz_t cycle_adapt = sz_max( o.cycle_adapt, 1 );

    while( state.cycle < o.cycle_finish && !o.exit_training() )
    {
        if( state.cycle == 0 || state.cycle - state.last_cycle_test >= o.cycle_test )
        {
            state.last_cycle_test = state.cycle;
            o.test();
        }

        o.tutor.prime( state.adaptive );
        state.cycle++;

        if( state.cycle - state.last_cycle_adapt >= cycle_adapt )
        {
            state.last_cycle_adapt = state.cycle;
            o.adapt();
        }

        if( state.cycle - state.last_cycle_backup >= o.cycle_backup )
        {
            state.last_cycle_backup = state.cycle;
            o.backup();
        }
    }
};

// ---------------------------------------------------------------------------------------------------------------------

/// multi threaded run
func (:s) run_training_multi_threaded =
{
    m bhpt_frame_state_s* state = o.state;
    o.thread_base!.tsetup( o.threads, state.adaptive, o.tutor );

    sz_t prime_cycles_per_thread = sz_max( o->cycle_adapt / sz_max( o.threads, 1 ), 1 );
    sz_t cycle_adapt = o.threads * prime_cycles_per_thread;

    while( state.cycle < o.cycle_finish && !o.exit_training() )
    {
        if( state.cycle == 0 || state.cycle - state.last_cycle_test >= o.cycle_test )
        {
            state.last_cycle_test = state.cycle;
            o.test();
        }

        o.thread_base.run( prime_cycles_per_thread );

        state.cycle += cycle_adapt;
        o.adapt();
        state.last_cycle_adapt = state.cycle;

        if( state.cycle - state.last_cycle_backup >= o.cycle_backup )
        {
            state.last_cycle_backup = state.cycle;
            o.backup();
        }
    }

    o.thread_base =< NULL;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) run_training =
{
    o.log.push_fa( "Running training... (C-z: interactive mode; C-c: save and quit)\n", o->state->cycle );
    o.clear_flags();
    if( o.threads < 1 )
    {
        o.run_training_single_threaded();
    }
    else
    {
        o.run_training_multi_threaded();
    }
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

    bl_t flag_reset = false;
    bl_t flag_start = false;

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
                flag_reset = true;
            }
            else if( arg.equal_sc( "-start" ) || arg.equal_sc( "-continue" ) )
            {
                flag_start = true;
            }
            else
            {
                return bcore_error_push_fa( TYPEOF_general_error, "Invalid argument '#<sc_t>'", arg.sc );
            }
        }
    }

    if( !flag_reset )
    {
        sc_t path = o.state_path.sc;
        if( path[ 0 ] && bcore_file_exists( path ) )
        {
            o.state =< bhpt_frame_state_s!.from_file_bin_ml( path );
            o.log.push_fa( "State recovered from '#<sc_t>'\n", path );
        }
        else
        {
            flag_reset = true;
        }
    }

    assert( o.tutor );

    if( flag_reset )
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

    if( flag_start )
    {
        o.run_training();
    }

    if( !o.exit_required() )
    {
        o.cast( m bcore_shell* ).loop( frame, frame.source, frame.sink, NULL );
    }

    o.log.push_fa( "\nSaving state at cycle #<sz_t>\n", o->state->cycle );
    o.backup();
    o.log.push_fa( "Exiting cleanly.\n" );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:state_s) (er_t table_to_sink(:state_s* o, bcore_hmap_name_s* hmap_name, x_via_path_adl_s* path_adl, m bcore_sink* sink )) =
{
    foreach( $* path in path_adl )
    {
        if( __i > 0 ) sink.push_sc( "," );
        path.to_sink( sink );
    }

    sink.push_sc( "\n" );

    foreach( $* test_result in o.test_result_adl )
    {
        foreach( $* path in path_adl )
        {
            if( __i > 0 ) sink.push_sc( "," );

            tp_t type = path.type_in( test_result );

            if( type == 0 )
            {
                st_s^ msg;
                msg.push_sc( "Path '" );
                path.to_sink( msg );
                msg.push_fa( "' not found in '#<sc_t>'.\n", bnameof( test_result._ ) );
                return bcore_error_push_sc( TYPEOF_general_error, msg.sc );
            }

            if( !bcore_tp_is_numeric( type ) )
            {
                st_s^ msg;
                msg.push_sc( "Path '" );
                path.to_sink( msg );
                msg.push_fa( "': '<#sc_t>' is not a numeric type.", bnameof( type ) );
                return bcore_error_push_sc( TYPEOF_general_error, msg.sc );
            }

            f3_t val = sr_to_f3( path.get_sr_in( test_result ) );
            sink.push_fa( "#<f3_t>", val );
        }
        sink.push_sc( "\n" );
    }
    return 0;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:state_s) (void help_to_sink( m bcore_sink* sink )) =
{
    sink.push_fa( "-help:  Prints this help information.\n" );
    sink.push_fa( "-csv name1 name2 ... : creates comma-separated-values table of selected numeric array items.\n" );
    sink.push_fa( "   Example: -csv cycle, test_result.error, test_result.bias \n" );
    sink.push_fa( "-adaptive '<command string>': Passes a command string to the adaptive.\n" );
    sink.push_fa( "   Example: -csv cycle, test_result.error, test_result.bias \n" );
};

// ---------------------------------------------------------------------------------------------------------------------

func (:state_s) bcore_main.main = (try)
{
    bcore_arr_st_s* args = frame.args;
    if( o.test_result_adl )
    {
        if( args.size > 2 )
        {
            st_s* cmd = args.[ 2 ];
            if( cmd.equal_sc( "-help" ) )
            {
                o.help_to_sink( BCORE_STDOUT );
                return 0;
            }
            else if( cmd.equal_sc( "-csv" ) )
            {
                sz_t idx = 3;
                if( idx == args.size ) return bcore_error_push_fa( TYPEOF_general_error, "Element name expected." );
                x_via_path_adl_s^ path_adl;
                while( idx < args.size )
                {
                    path_adl.push_d( x_via_path_s!.parse_sc( args.[ idx++ ].sc ) );
                }

                bcore_hmap_name_s^ hmap_name;
                bcore_arr_tp_s^ arr_tp;
                for( ; idx < args.size; idx++ ) arr_tp.push( hmap_name.set_sc( args.[idx].sc ) );
                o.table_to_sink( hmap_name, path_adl, BCORE_STDOUT ).try();
                return 0;
            }
            else if( cmd.equal_sc( "-adaptive" ) )
            {
                if( args.size > 3 )
                {
                    st_s* sub_cmd = args.[ 3 ];
                    o.adaptive?.run_command( sub_cmd.sc );
                }
            }
            else
            {
                return bcore_error_push_fa( TYPEOF_general_error, "Invalid command '#<sc_t>'", cmd.sc );
            }
        }
        else
        {
            o.test_result_adl.to_sink( 1, BCORE_STDOUT );
            bcore_sink_a_push_fa( BCORE_STDOUT, "Current cycle number: #<sz_t>\n", o.cycle );
        }
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

group :op = retrievable
{
    stamp :run_s =
    {
        func bcore_shell_op.key = { return "run"; };
        func bcore_shell_op.info = { return "starts/continues training; C-z: interactive exit; C-c: terminating exit"; };
        func bcore_shell_op.run =
        {
            m$* frame = obj.cast( m ::s* );
            frame.run_training();
            if( frame.exit_required() ) control.request_exit_loop();
        };
    };

    stamp :safe_s =
    {
        func bcore_shell_op.key = { return "safe"; };
        func bcore_shell_op.info = { return "saves current status"; };
        func bcore_shell_op.run =
        {
            m$* frame = obj.cast( m ::s* );
            sink.push_fa( "Saving state at cycle #<sz_t>\n", frame.state.cycle );
            frame.backup();
        };
    };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
