/** Author and Copyright 2019 Johannes Bernhard Steffens
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

#include "badapt_trainer.h"

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// badapt_trainer_batch_s

//----------------------------------------------------------------------------------------------------------------------

badapt_training_state* badapt_trainer_batch_s_create_state( const badapt_trainer_batch_s* o )
{
    badapt_training_state_std_s* state = badapt_training_state_std_s_create();
    return ( badapt_training_state* )state;
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_trainer_batch_s_run( const badapt_trainer_batch_s* o, badapt_training_state* training_state )
{
    ASSERT( *(aware_t*)training_state == TYPEOF_badapt_training_state_std_s );

    ASSERT( badapt_training_state_a_defines_set_supplier( training_state ) );
    ASSERT( badapt_training_state_a_defines_get_adaptive( training_state ) );
    ASSERT( badapt_training_state_a_defines_get_progress( training_state ) );
    ASSERT( badapt_training_state_a_defines_get_guide( training_state ) );

    badapt_supplier*   supplier = badapt_training_state_a_get_supplier( training_state );
    badapt_adaptive*   adaptive = badapt_training_state_a_get_adaptive( training_state );
    badapt_progress_s* progress = badapt_training_state_a_get_progress( training_state );
    badapt_guide*      guide    = badapt_training_state_a_get_guide(    training_state );

    ASSERT( supplier != NULL );

    const badapt_loss* loss = o->loss;
    if( !loss && badapt_supplier_a_defines_preferred_loss( supplier ) )
    {
        loss = badapt_supplier_a_preferred_loss( supplier );
    }

    ASSERT( loss != NULL );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( badapt_arr_sample_batch_s, buffer_valid );
    BCORE_LIFE_CREATE( badapt_arr_sample_batch_s, buffer_batch );
    BCORE_LIFE_CREATE( bmath_vf3_s, out );

    ASSERT( badapt_adaptive_a_get_in_size(  adaptive ) == badapt_supplier_a_get_in_size(  supplier ) );
    ASSERT( badapt_adaptive_a_get_out_size( adaptive ) == badapt_supplier_a_get_out_size( supplier ) );

    bmath_vf3_s_set_size( out, badapt_adaptive_a_get_out_size( adaptive ) );

    bcore_array_a_set_size( ( bcore_array* )buffer_valid, 0 );

    badapt_arr_sample_batch_s_set_size( buffer_valid, o->valid_size );
    for( sz_t i = 0; i < o->valid_size; i++ )
    {
        badapt_supplier_a_fetch_sample_vio( supplier, &buffer_valid->arr_data[ i ].in, &buffer_valid->arr_data[ i ].out );
    }

    f3_t val_last_error = 0;
    f3_t improved = 0;
    f3_t bias = 0;

    for( ; progress->iteration <= o->max_iterations; progress->iteration++ )
    {
        f3_t val_error = 0;
        f3_t val_weight = 0;
        f3_t trn_error = 0;
        f3_t trn_weight = 0;

        {
            bl_t use_infer = badapt_adaptive_a_defines_infer( adaptive );
            badapt_adaptive* adaptive_val = use_infer ? bcore_fork( adaptive ) : badapt_adaptive_a_clone( adaptive );

            for( sz_t i = 0; i < o->valid_size; i++ )
            {
                const badapt_sample_batch_s* sample = &buffer_valid->arr_data[ i ];
                if( use_infer )
                {
                    badapt_adaptive_a_infer( adaptive_val, &sample->in, out );
                }
                else
                {
                    badapt_adaptive_a_minfer( adaptive_val, &sample->in, out );
                }
                f3_t val_loss = badapt_loss_a_loss( loss, out, &sample->out );
                val_error  += val_loss / sample->out.size;
                val_weight += 1.0;
            }
            badapt_adaptive_a_discard( adaptive_val );
        }

        if( val_weight > 0 ) val_error /= val_weight;
        if( val_error > 0 && val_last_error > 0 ) improved = log( val_last_error ) - log( val_error );
        val_last_error = val_error;

        if( guide )
        {
            progress->error    = val_error;
            progress->improved = improved;
            progress->bias     = bias;
            if( !badapt_guide_a_callback( guide, training_state ) ) break;
        }
        badapt_training_state_a_backup( training_state );

        if( progress->iteration == o->max_iterations ) break;

        for( sz_t fetch_cycle = 0; fetch_cycle < o->fetch_cycles_per_iteration; fetch_cycle++ )
        {
            bcore_array_a_set_size( ( bcore_array* )buffer_batch, 0 );

            badapt_arr_sample_batch_s_set_size( buffer_batch, o->batch_size );
            for( sz_t i = 0; i < o->batch_size; i++ )
            {
                badapt_supplier_a_fetch_sample_vio( supplier, &buffer_batch->arr_data[ i ].in, &buffer_batch->arr_data[ i ].out );
            }

            for( sz_t batch_cycle = 0; batch_cycle < o->batch_cycles_per_fetch; batch_cycle++ )
            {
                for( sz_t i = 0; i < o->batch_size; i++ )
                {
                    const badapt_sample_batch_s* sample = &buffer_batch->arr_data[ i ];
                    badapt_adaptive_a_adapt_loss( adaptive, loss, &sample->in, &sample->out, out );
                    f3_t val_loss = badapt_loss_a_loss( loss, out, &sample->out );

                    trn_error  += val_loss / sample->out.size;
                    trn_weight += 1.0;
                }
            }
        }

        if( trn_weight > 0 ) trn_error /= trn_weight;
        if( val_error > 0 && trn_error > 0 ) bias = log( val_error ) - log( trn_error );
    }

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// badapt_trainer_main_s

s2_t badapt_trainer_main_s_main( badapt_trainer_main_s* o, const bcore_arr_st_s* args )
{
    BCORE_LIFE_INIT();

    bl_t reset = false;
    BCORE_LIFE_CREATE( st_s, backup_path );

    if( args )
    {
        for( sz_t i = 0; i < args->size; i++ )
        {
            if( st_s_equal_sc( args->data[ i ], "-reset" ) ) reset = true;
        }

        if( args->size > 1 )
        {
            st_s_push_fa( backup_path, "#<sc_t>.state", args->data[ 1 ]->sc );
        }
    }

    badapt_training_state* state = BCORE_LIFE_A_PUSH( badapt_trainer_a_create_state( o->trainer ) );
    if( badapt_training_state_a_defines_set_backup_path( state ) ) badapt_training_state_a_set_backup_path( state, backup_path->sc );
    if( !reset && badapt_training_state_a_recover( state ) )
    {
        bcore_msg_fa( "state recovered from '#<sc_t>'\n", badapt_training_state_a_get_backup_path( state ) );
    }
    else
    {
        badapt_supplier_a_setup_builder( o->problem, o->builder );
        badapt_training_state_a_set_adaptive( state, BCORE_LIFE_A_PUSH( badapt_builder_a_build( o->builder ) ) );
    }

    badapt_training_state_a_set_supplier( state, o->problem );
    badapt_training_state_a_set_guide(    state, o->guide );

    badapt_adaptive_a_arc_to_sink( badapt_training_state_a_get_adaptive( state ), BCORE_STDOUT );
    badapt_trainer_a_run( o->trainer, state );
    BCORE_LIFE_RETURNV( s2_t, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t badapt_trainer_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_trainer" ) ) )
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

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

