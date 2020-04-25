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

vd_t bhpt_frame_thread_s_tloop( bhpt_frame_thread_s* o )
{
    bcore_mutex_s_lock( o->mutex );

    while( o->running )
    {
        if( o->tutor && o->adaptive )
        {
            while( o->prime_cycles > 0 )
            {
                bhpt_tutor_a_prime( o->tutor, o->adaptive );
                o->prime_cycles--;
            }
        }
        bcore_condition_s_sleep( o->condition, o->mutex );
    }

    bcore_mutex_s_unlock( o->mutex );
    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_thread_s_tloop_enter( bhpt_frame_thread_s* o )
{
    bhpt_frame_thread_s_tloop_exit( o );
    if( !o->thread ) o->thread = bcore_thread_s_create();
    if( !o->mutex  ) o->mutex  = bcore_mutex_s_create();
    if( !o->probe  ) o->probe  = bhpt_adaptor_probe_s_create();
    ASSERT( o->condition );

    bcore_mutex_s_lock( o->mutex );
    o->running = true;
    bhpt_adaptive_a_get_adaptor_probe( o->adaptive, o->probe );
    bcore_mutex_s_unlock( o->mutex );

    bcore_thread_s_call( o->thread, ( bcore_fp_thread )bhpt_frame_thread_s_tloop, o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_thread_s_tloop_exit( bhpt_frame_thread_s* o )
{
    if( !o->running   ) return;
    if( !o->thread    ) return;
    if( !o->mutex     ) return;
    if( !o->condition ) return;
    bcore_mutex_s_lock( o->mutex );
    o->running = false;
    bcore_mutex_s_unlock( o->mutex );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_thread_base_s_tsetup( bhpt_frame_thread_base_s* o, sz_t threads, bhpt_adaptive* adaptive, bhpt_tutor* tutor )
{
    BLM_INIT();
    if( o->thread_ads.size > 0 ) bhpt_frame_thread_base_s_tdown( o );
    bcore_condition_s_attach( &o->condition, bcore_condition_s_create() );
    bhpt_frame_thread_ads_s_set_size( &o->thread_ads, threads );

    bhpt_adaptor_probe_s* probe0 = bhpt_adaptive_a_get_adaptor_probe( adaptive, BLM_CREATE( bhpt_adaptor_probe_s ) );
    bhpt_adaptor_probe_s* probe1 = BLM_CREATE( bhpt_adaptor_probe_s );

    BFOR_EACH( i, &o->thread_ads )
    {
        bhpt_frame_thread_s* thread = &o->thread_ads.data[ i ];
        thread->condition = bcore_fork( o->condition );
        thread->tutor     = bcore_fork( tutor );
        thread->adaptive  = bhpt_adaptive_a_clone( adaptive );

        // weakly redirect adadaptive values
        bhpt_adaptive_a_get_adaptor_probe( thread->adaptive, probe1 );
        BFOR_EACH( i, probe0 )
        {
            bhpt_adaptor_node_s* node0 = &probe0->data[ i ];
            bhpt_adaptor_node_s* node1 = &probe1->data[ i ];
            bhvm_value_s_weak( &node1->axon->v, &node0->axon->v );
        }
    }

    BFOR_EACH( i, &o->thread_ads ) bhpt_frame_thread_s_tloop_enter( &o->thread_ads.data[ i ] );
    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_thread_base_s_tdown( bhpt_frame_thread_base_s* o )
{
    if( !o->condition ) return;
    BFOR_EACH( i, &o->thread_ads )
    {
        bhpt_frame_thread_s* thread = &o->thread_ads.data[ i ];
        bhpt_frame_thread_s_tloop_exit( thread );
    }
    bcore_condition_s_wake_all( o->condition );
    bhpt_frame_thread_ads_s_clear( &o->thread_ads );
    bcore_condition_s_detach( &o->condition );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_thread_base_s_run( bhpt_frame_thread_base_s* o, bhpt_adaptor_probe_s* adaptive_probe, sz_t cycles_per_thread )
{
    BFOR_EACH( i, &o->thread_ads )
    {
        bhpt_frame_thread_s* thread = &o->thread_ads.data[ i ];
        bcore_mutex_s_lock( thread->mutex );
        BFOR_EACH( i, thread->probe ) bhvm_value_s_zro( &thread->probe->data[ i ].grad->v );
        thread->prime_cycles = cycles_per_thread;
        thread->flag = true;
        bcore_mutex_s_unlock( thread->mutex );
    }

    bcore_condition_s_wake_all( o->condition );
    BFOR_EACH( i, adaptive_probe ) bhvm_value_s_zro( &adaptive_probe->data[ i ].grad->v );

    for( sz_t busy_count = o->thread_ads.size; busy_count > 0; )
    {
        bcore_sleep_us( 10 );
        busy_count = 0;
        BFOR_EACH( i, &o->thread_ads )
        {
            bhpt_frame_thread_s* thread = &o->thread_ads.data[ i ];
            if( thread->flag )
            {
                bcore_mutex_s_lock( thread->mutex );
                if( thread->prime_cycles == 0 )
                {
                    thread->flag = false;
                    BFOR_EACH( i, adaptive_probe ) bhvm_holor_s_acc( adaptive_probe->data[ i ].grad, thread->probe->data[ i ].grad );
                }
                bcore_mutex_s_unlock( thread->mutex );
            }
            busy_count += thread->flag;
        }
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

    sz_t cycle_adapt = sz_max( o->threads, 1 ) * o->prime_cycles_per_thread;

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
            sc_t path = o->state_path.sc;
            if( path[ 0 ] )
            {
                bcore_bin_ml_a_to_file( o->state, path );
            }
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

    sz_t cycle_adapt = o->threads * o->prime_cycles_per_thread;

    bhpt_adaptor_probe_s* adaptive_probe = bhpt_adaptive_a_get_adaptor_probe( state->adaptive, BLM_CREATE( bhpt_adaptor_probe_s ) );

    while( state->cycle_number < o->cycle_finish )
    {
        if( state->cycle_number == 0 || state->cycle_number - state->last_cycle_test >= o->cycle_test )
        {
            state->last_cycle_test = state->cycle_number;
            bhpt_frame_s_test( o );
        }

        bhpt_frame_thread_base_s_run( o->thread_base, adaptive_probe, o->prime_cycles_per_thread );
        state->cycle_number += cycle_adapt;
        bhpt_frame_s_adapt( o );

        if( state->cycle_number - state->last_cycle_backup >= o->cycle_backup )
        {
            state->last_cycle_backup = state->cycle_number;
            sc_t path = o->state_path.sc;
            if( path[ 0 ] )
            {
                bcore_bin_ml_a_to_file( o->state, path );
            }
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

    if( o->threads <= 1 )
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

