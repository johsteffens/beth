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
    bhpt_hprobe_s* hprobe_adaptive = BLM_CREATE( bhpt_hprobe_s );
    bhpt_hprobe_s* hprobe_accugrad = BLM_CREATE( bhpt_hprobe_s );

    bhpt_adaptive_a_get_hprobe_adaptive( o->state->adaptive, hprobe_adaptive );
    bhpt_adaptive_a_get_hprobe_accugrad( o->state->adaptive, hprobe_accugrad );

    ASSERT( o->adaptor_adl->size == hprobe_adaptive->size );
    ASSERT( o->adaptor_adl->size == hprobe_accugrad->size );

    BFOR_EACH( i, o->adaptor_adl )
    {
        bhpt_adaptor_a_adapt( o->adaptor_adl->data[ i ], hprobe_accugrad->data[ i ], hprobe_adaptive->data[ i ] );
        bhvm_value_s_zro( &hprobe_accugrad->data[ i ]->v );
    }

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bhpt_frame_s_run( bhpt_frame_s* o )
{
    bhpt_frame_state_s* state = o->state;

    while( state->cycle_number < o->cycle_finish )
    {
        bhpt_tutor_a_prime( o->tutor, state->adaptive );
        state->cycle_number++;

        if( state->cycle_number - state->last_cycle_adapt >= o->cycle_adapt )
        {
            state->last_cycle_adapt = state->cycle_number;
            bhpt_frame_s_adapt( o );
        }

        if( state->cycle_number - state->last_cycle_test >= o->cycle_test )
        {
            state->last_cycle_test = state->cycle_number;
            bhpt_tutor_a_test( o->tutor, state->adaptive, o->verbosity, o->log );
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

    if( reset )
    {
        bhpt_frame_state_s_attach( &o->state, bhpt_frame_state_s_create() );
    }
    else
    {
        sc_t path = o->state_path.sc;
        if( path[ 0 ] && bcore_file_exists( path ) )
        {
            bhpt_frame_state_s_attach( &o->state, bhpt_frame_state_s_create() );
            bcore_bin_ml_a_from_file( o->state, path );
            bcore_sink_a_push_fa( o->log, "State recovered from '#<sc_t>'\n", path );
        }
    }

    assert( o->builder );
    assert( o->tutor );

    if( !o->state->adaptive ) o->state->adaptive = bhpt_tutor_a_create_adaptive( o->tutor, o->builder );

    // setting up adaptors
    {
        BLM_INIT();
        bhpt_adaptor_adl_s_attach( &o->adaptor_adl, bhpt_adaptor_adl_s_create() );
        bhpt_hprobe_s* hprobe = BLM_CREATE( bhpt_hprobe_s );
        bhpt_adaptive_a_get_hprobe_adaptive( o->state->adaptive, hprobe );
        bhpt_adaptor_adl_s_set_size( o->adaptor_adl, hprobe->size );
        BFOR_EACH( i, o->adaptor_adl ) o->adaptor_adl->data[ i ] = bhpt_adaptor_a_clone( o->adaptor );
        BLM_DOWN();
    }

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

