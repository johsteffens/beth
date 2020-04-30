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

#include "bhpt_frame.h"

#ifdef TYPEOF_bhpt

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bhpt_frame_thread_item_s_loop( bhpt_frame_thread_item_s* o )
{
    BLM_INIT();
    bhpt_adaptor_probe_s* probe_item  = BLM_CREATE( bhpt_adaptor_probe_s );
    bhpt_adaptor_probe_s* probe_share = BLM_CREATE( bhpt_adaptor_probe_s );

    bcore_mutex_s_lock( o->mutex );

    while( o->running )
    {
        if( o->prime_cycles > 0 )
        {
            bhpt_adaptive_a_get_adaptor_probe( o->adaptive, probe_item );

            bhpt_adaptor_probe_s_zro_grad( probe_item );
            BFOR_SIZE( i, o->prime_cycles ) bhpt_tutor_a_prime( o->share->tutor, o->adaptive );

            bhpt_adaptive_a_get_adaptor_probe( o->adaptive, probe_item );

            bcore_mutex_s_lock( o->share->mutex );
            bhpt_adaptive_a_get_adaptor_probe( o->share->adaptive, probe_share );
            bhpt_adaptor_probe_s_acc_grad( probe_share, probe_item );
            o->share->finished_count++;
            bcore_mutex_s_unlock( o->share->mutex );

            o->prime_cycles = 0;
        }

        bcore_condition_s_sleep( o->share->condition_item, o->mutex );
    }

    bcore_mutex_s_unlock( o->mutex );
    BLM_RETURNV( vd_t, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_thread_item_s_loop_enter( bhpt_frame_thread_item_s* o )
{
    bhpt_frame_thread_item_s_loop_exit( o );
    if( !o->thread ) o->thread = bcore_thread_s_create();
    if( !o->mutex  ) o->mutex  = bcore_mutex_s_create();
    ASSERT( o->share->condition_item );

    bcore_mutex_s_lock( o->mutex );
    o->running = true;
    bcore_mutex_s_unlock( o->mutex );

    bcore_thread_s_call( o->thread, ( bcore_fp_thread )bhpt_frame_thread_item_s_loop, o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_thread_item_s_loop_exit( bhpt_frame_thread_item_s* o )
{
    if( !o->mutex ) return;
    bcore_mutex_s_lock( o->mutex );
    o->running = false;
    bcore_mutex_s_unlock( o->mutex );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_thread_base_s_tsetup( bhpt_frame_thread_base_s* o, sz_t threads, bhpt_adaptive* adaptive, bhpt_tutor* tutor )
{
    BLM_INIT();
    if( o->ads.size > 0 ) bhpt_frame_thread_base_s_tdown( o );
    bhpt_frame_thread_share_s_attach( &o->share, bhpt_frame_thread_share_s_create() );
    bhpt_frame_thread_share_s* share = o->share;
    bcore_condition_s_attach( &share->condition_item, bcore_condition_s_create() );
    bcore_mutex_s_attach( &share->mutex, bcore_mutex_s_create() );
    bhpt_adaptive_a_attach( &share->adaptive, bcore_fork( adaptive ) );

    bhpt_tutor_a_attach( &share->tutor, bcore_fork( tutor ) );
    bhpt_frame_thread_ads_s_set_size( &o->ads, threads );

    bhpt_adaptor_probe_s* probe_item  = BLM_CREATE( bhpt_adaptor_probe_s );
    bhpt_adaptor_probe_s* probe_share = bhpt_adaptive_a_get_adaptor_probe( adaptive, BLM_CREATE( bhpt_adaptor_probe_s ) );

    BFOR_EACH( i, &o->ads )
    {
        bhpt_frame_thread_item_s* item = &o->ads.data[ i ];
        item->adaptive = bhpt_adaptive_a_clone( adaptive );

        bhpt_adaptive_a_get_adaptor_probe( item->adaptive, probe_item );
        bhpt_adaptor_probe_s_rebind_axon( probe_item, probe_share );
        bhpt_adaptive_a_rebind_holors( item->adaptive );

        item->share    = bcore_fork( share );
    }

    BFOR_EACH( i, &o->ads ) bhpt_frame_thread_item_s_loop_enter( &o->ads.data[ i ] );
    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_thread_base_s_tdown( bhpt_frame_thread_base_s* o )
{
    if( !o->share ) return;
    BFOR_EACH( i, &o->ads )
    {
        bhpt_frame_thread_item_s* item = &o->ads.data[ i ];
        bhpt_frame_thread_item_s_loop_exit( item );
    }
    bcore_condition_s_wake_all( o->share->condition_item );

    bhpt_frame_thread_ads_s_clear( &o->ads );
    bcore_condition_s_detach( &o->share->condition_item );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_thread_base_s_run( bhpt_frame_thread_base_s* o, sz_t cycles_per_thread )
{
    bcore_mutex_s_lock( o->share->mutex );

    o->share->finished_count = 0;
    bcore_mutex_s_unlock( o->share->mutex );

    BFOR_EACH( i, &o->ads )
    {
        bhpt_frame_thread_item_s* item = &o->ads.data[ i ];
        bcore_mutex_s_lock( item->mutex );
        item->prime_cycles = cycles_per_thread;
        bcore_mutex_s_unlock( item->mutex );
    }

    bcore_condition_s_wake_all( o->share->condition_item );

    for( bl_t busy = true; busy; )
    {
        bcore_sleep_us( 10 );

        /// obtaining item-locks reduces idling cycles
        BFOR_EACH( i, &o->ads ) bhpt_frame_thread_item_s_wait_while_locked( &o->ads.data[ i ] );

        bcore_mutex_s_lock( o->share->mutex );
        busy = o->share->finished_count < o->ads.size;
        bcore_mutex_s_unlock( o->share->mutex );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_s_adapt( bhpt_frame_s* o )
{
    BLM_INIT();
    bhpt_adaptor_adl_s* adaptor_adl = o->state->adaptor_adl;
    bhpt_adaptive*      adaptive = o->state->adaptive;
    bhpt_adaptor_probe_s* probe = bhpt_adaptive_a_get_adaptor_probe( adaptive, BLM_CREATE( bhpt_adaptor_probe_s ) );

    ASSERT( probe->size == adaptor_adl->size );

    BFOR_EACH( i, adaptor_adl )
    {
        bhpt_adaptor_a_adapt( adaptor_adl->data[ i ], &probe->data[ i ] );
    }

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_s_backup( bhpt_frame_s* o )
{
    sc_t path = o->state_path.sc;
    if( path[ 0 ] ) bcore_bin_ml_a_to_file( o->state, path );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_s_test( bhpt_frame_s* o )
{
    bhpt_frame_state_s* state = o->state;
    if( o->verbosity > 0 ) bcore_sink_a_push_fa( o->log, "#pl10 {#<sz_t>}: ", state->cycle_number );
    bhpt_tutor_a_test( o->tutor, state->adaptive, o->verbosity, o->log );
    if( o->verbosity > 0 )
    {
        BLM_INIT();
        bhpt_adaptor_probe_s* probe = bhpt_adaptive_a_get_adaptor_probe( state->adaptive, BLM_CREATE( bhpt_adaptor_probe_s ) );
        f3_t min = 0, max = 0;
        bhpt_adaptor_probe_s_get_min_max( probe, &min, &max );
        bcore_sink_a_pushf( o->log, ", min: %6.3g, max: %6.3g", min, max );
        bcore_sink_a_push_fa( o->log, "\n" );
        BLM_DOWN();
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/// single threaded run
void bhpt_frame_s_run_single_threaded( bhpt_frame_s* o )
{
    bhpt_frame_state_s* state = o->state;

    sz_t cycle_adapt = sz_max( o->cycle_adapt, 1 );

    while( state->cycle_number < o->cycle_finish )
    {
        if( state->cycle_number == 0 || state->cycle_number - state->last_cycle_test >= o->cycle_test )
        {
            state->last_cycle_test = state->cycle_number;
            bhpt_frame_s_test( o );
        }

        bhpt_tutor_a_prime( o->tutor, state->adaptive );
        state->cycle_number++;

        if( state->cycle_number - state->last_cycle_adapt >= cycle_adapt )
        {
            state->last_cycle_adapt = state->cycle_number;
            bhpt_frame_s_adapt( o );
        }

        if( state->cycle_number - state->last_cycle_backup >= o->cycle_backup )
        {
            state->last_cycle_backup = state->cycle_number;
            bhpt_frame_s_backup( o );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/// multi threaded run
void bhpt_frame_s_run_multi_threaded( bhpt_frame_s* o )
{
    BLM_INIT();
    bhpt_frame_state_s* state = o->state;

    if( !o->thread_base ) o->thread_base = bhpt_frame_thread_base_s_create();
    bhpt_frame_thread_base_s_tsetup( o->thread_base, o->threads, state->adaptive, o->tutor );

    sz_t prime_cycles_per_thread = sz_max( o->cycle_adapt / sz_max( o->threads, 1 ), 1 );
    sz_t cycle_adapt = o->threads * prime_cycles_per_thread;

    while( state->cycle_number < o->cycle_finish )
    {
        if( state->cycle_number == 0 || state->cycle_number - state->last_cycle_test >= o->cycle_test )
        {
            state->last_cycle_test = state->cycle_number;
            bhpt_frame_s_test( o );
        }

        bhpt_frame_thread_base_s_run( o->thread_base, prime_cycles_per_thread );


        state->cycle_number += cycle_adapt;
        bhpt_frame_s_adapt( o );
        state->last_cycle_adapt = state->cycle_number;

        if( state->cycle_number - state->last_cycle_backup >= o->cycle_backup )
        {
            state->last_cycle_backup = state->cycle_number;
            bhpt_frame_s_backup( o );
        }
    }

    bhpt_frame_thread_base_s_detach( &o->thread_base );
    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

s2_t bhpt_frame_s_main( bhpt_frame_s* o, const bcore_arr_st_s* args )
{
    BLM_INIT();

    bl_t reset = false;

    if( !o->log   ) o->log   = bcore_fork( BCORE_STDOUT );
    if( !o->state ) o->state = bhpt_frame_state_s_create();

    if( args )
    {
        for( sz_t i = 0; i < args->size; i++ )
        {
            if( st_s_equal_sc( args->data[ i ], "-reset" ) ) reset = true;
        }

        if( args->size > 1 )
        {
            st_s_push_fa( &o->state_path, "#<sc_t>.state", args->data[ 1 ]->sc );
        }
    }

    if( !reset )
    {
        sc_t path = o->state_path.sc;
        if( path[ 0 ] && bcore_file_exists( path ) )
        {
            bhpt_frame_state_s_attach( &o->state, bhpt_frame_state_s_create() );
            bcore_bin_ml_a_from_file( o->state, path );
            bcore_sink_a_push_fa( o->log, "State recovered from '#<sc_t>'\n", path );
        }
        else
        {
            reset = true;
        }
    }

    if( reset )
    {
        BLM_INIT();
        bhpt_frame_state_s_attach( &o->state, bhpt_frame_state_s_create() );
        o->state->adaptive = bhpt_tutor_a_create_adaptive( o->tutor, o->builder );

        // setting up adaptors
        bhpt_adaptor_adl_s_attach( &o->state->adaptor_adl, bhpt_adaptor_adl_s_create() );
        bhpt_adaptor_probe_s* probe = BLM_CREATE( bhpt_adaptor_probe_s );
        bhpt_adaptive_a_get_adaptor_probe( o->state->adaptive, probe );
        bhpt_adaptor_adl_s_set_size( o->state->adaptor_adl, probe->size );
        BFOR_EACH( i, o->state->adaptor_adl ) o->state->adaptor_adl->data[ i ] = bhpt_adaptor_a_clone( o->adaptor );

        BLM_DOWN();
    }

    assert( o->builder );
    assert( o->tutor );
    assert( o->state );
    assert( o->state->adaptive );

    bhpt_tutor_a_status_to_sink( o->tutor, o->verbosity, o->log );
    bcore_sink_a_push_fa( o->log, "\n" );
    bhpt_adaptive_a_status_to_sink( o->state->adaptive, o->verbosity, o->log );
    bcore_sink_a_push_fa( o->log, "\n" );

    if( o->threads < 1 )
    {
        bhpt_frame_s_run_single_threaded( o );
    }
    else
    {
        bhpt_frame_s_run_multi_threaded( o );
    }

    BLM_RETURNV( s2_t, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhpt_frame

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bhpt_frame_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhpt_frame" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_get_quicktypes:
        {
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        case TYPEOF_plant:
        {
            bcore_plant_compile( "bhpt_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

