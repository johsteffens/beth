/** Copyright 2019 Johannes Bernhard Steffens
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

#include "badapt_training.h"

/**********************************************************************************************************************/
// badapt_supplier

//----------------------------------------------------------------------------------------------------------------------

void badapt_supplier_setup_builder_default( const badapt_supplier* o, badapt_builder* builder )
{
    badapt_builder_a_set_in_size ( builder, badapt_supplier_a_get_in_size( o ) );
    badapt_builder_a_set_out_size( builder, badapt_supplier_a_get_out_size( o ) );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// badapt_training

//----------------------------------------------------------------------------------------------------------------------

bl_t badapt_guide_std_s_callback( const badapt_guide_std_s* o, badapt_training_state* training_state )
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( badapt_dynamics_std_s, dynamics );

    ASSERT( badapt_training_state_a_defines_get_adaptive( training_state ) );
    ASSERT( badapt_training_state_a_defines_get_progress( training_state ) );
    badapt_adaptive*   adaptive = badapt_training_state_a_get_adaptive( training_state );
    badapt_progress_s* progress = badapt_training_state_a_get_progress( training_state );


    ASSERT( *(aware_t*)training_state == TYPEOF_badapt_training_state_std_s );
//    badapt_training_state_std_s* state = ( badapt_training_state_std_s* )training_state;

    badapt_adaptive_a_get_dynamics_std( adaptive, dynamics );

    if( o->log )
    {
        bcore_sink_a_pushf( o->log, "% 6zi: err%6.3f|improved %6.3f|bias %6.3f|log(epsilon) %5.3f\n", progress->iteration, progress->error, progress->improved, progress->bias, log( dynamics->epsilon ) );
    }

    dynamics->epsilon *= o->annealing_factor;
    badapt_adaptive_a_set_dynamics_std( adaptive, dynamics );

    BCORE_LIFE_RETURN( true );
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// badapt_training_state_std_s

//----------------------------------------------------------------------------------------------------------------------

void badapt_training_state_std_s_set_adaptive( badapt_training_state_std_s* o, badapt_adaptive* adaptive )
{
    badapt_adaptive_a_replicate( &o->adaptive, adaptive );
}

//----------------------------------------------------------------------------------------------------------------------

badapt_adaptive* badapt_training_state_std_s_get_adaptive( const badapt_training_state_std_s* o )
{
    return o->adaptive;
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_training_state_std_s_set_supplier( badapt_training_state_std_s* o, badapt_supplier* supplier )
{
    badapt_supplier_a_replicate( &o->supplier, supplier );
}

//----------------------------------------------------------------------------------------------------------------------

badapt_supplier* badapt_training_state_std_s_get_supplier( const badapt_training_state_std_s* o )
{
    return o->supplier;
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_training_state_std_s_set_progress( badapt_training_state_std_s* o, badapt_progress_s* progress )
{
    badapt_progress_s_copy( &o->progress, progress );
}

//----------------------------------------------------------------------------------------------------------------------

badapt_progress_s* badapt_training_state_std_s_get_progress( const badapt_training_state_std_s* o )
{
    return ( badapt_progress_s* )&o->progress;
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_training_state_std_s_set_guide( badapt_training_state_std_s* o, badapt_guide* guide )
{
    badapt_guide_a_replicate( &o->guide, guide );
}

//----------------------------------------------------------------------------------------------------------------------

badapt_guide* badapt_training_state_std_s_get_guide( const badapt_training_state_std_s* o )
{
    return ( badapt_guide* )o->guide;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// badapt_guide_std_s

void badapt_guide_std_s_init_x( badapt_guide_std_s* o )
{
    o->log = bcore_fork( BCORE_STDOUT );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t badapt_training_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_training" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            return NULL;
        }
        break;

        case TYPEOF_precoder:
        {
            bcore_precoder_compile( "badapt_precoded", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

