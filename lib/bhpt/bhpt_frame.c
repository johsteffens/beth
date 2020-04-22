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

void bhpt_frame_s_run( bhpt_frame_s* o )
{
    bhpt_frame_state_s* state = o->state;

    while( state->cycle_number < o->cycle_finish )
    {
        if( state->cycle_number == 0 || state->cycle_number - state->last_cycle_test >= o->cycle_test )
        {
            state->last_cycle_test = state->cycle_number;
            bhpt_frame_s_test( o );
        }

        bhpt_tutor_a_prime( o->tutor, state->adaptive );
        state->cycle_number++;

        if( state->cycle_number - state->last_cycle_adapt >= o->cycle_adapt )
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

    bhpt_adaptive_a_status_to_sink( o->state->adaptive, o->verbosity, o->log );

    bhpt_frame_s_run( o );

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

