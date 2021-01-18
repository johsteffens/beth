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
    if( path[ 0 ] ) bcore_bin_ml_a_to_file( o.state, path );
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) (void test( m@* o )) =
{
    if( o.verbosity > 0 ) o.log.push_fa( "#pl10 {#<sz_t>}: ", o.state.cycle_number );
    o.tutor.test( o.state.adaptive, o.verbosity, o.log );
    if( o.verbosity > 0 )
    {
        m $* probe = o.state.adaptive.get_adaptor_probe( bhpt_adaptor_probe_s!^ );
        m $* stats_axon = bhvm_stats_s!^;
        probe.acc_stats( stats_axon, NULL );
        o.log.push_fa( ", axon{ " );
        stats_axon.to_sink( o.log );
        o.log.push_fa( " }" );
        o.log.push_fa( ", grad{ " );
        o.stats_grad.to_sink( o.log );
        o.log.push_fa( " }\n" );
        o.stats_grad.clear();
    }
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

func (:s) bcore_main.main = (try)
{
    o.main_frame = frame;

    bl_t reset = false;

    if( !o.log   ) o.log   = bcore_fork( BCORE_STDOUT );

    o.state!;

    c bcore_arr_st_s* args = frame.args;

    if( args )
    {
        foreach( c st_s* arg in args ) if( arg.equal_sc( "-reset" ) ) reset = true;

        if( args.size > 1 )
        {
            o.state_path.push_fa( "#<sc_t>.state", args.[ 1 ].sc );
        }
    }

    if( !reset )
    {
        sc_t path = o.state_path.sc;
        if( path[ 0 ] && bcore_file_exists( path ) )
        {
            o.state =< bhpt_frame_state_s!;
            bcore_bin_ml_a_from_file( o.state, path );
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

/**********************************************************************************************************************/
