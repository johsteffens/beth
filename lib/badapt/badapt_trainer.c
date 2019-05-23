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

#include "badapt_trainer.h"

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// badapt_trainer_batch_s

//----------------------------------------------------------------------------------------------------------------------

badapt_training_state* badapt_trainer_batch_s_create_state( const badapt_trainer_batch_s* o )
{
    badapt_training_state_std_s* state = badapt_training_state_std_s_create();
    state->log = bcore_inst_a_clone( ( bcore_inst* )BCORE_STDOUT );
    return ( badapt_training_state* )state;
}

//----------------------------------------------------------------------------------------------------------------------

void badapt_trainer_batch_s_run( const badapt_trainer_batch_s* o, badapt_training_state* training_state )
{
    ASSERT( *(aware_t*)training_state == TYPEOF_badapt_training_state_std_s );
    badapt_training_state_std_s* state = ( badapt_training_state_std_s* )training_state;

    ASSERT( state->supplier != NULL );

    const badapt_loss* loss = o->loss;
    if( !loss && badapt_supplier_a_defines_preferred_loss( state->supplier ) )
    {
        loss = badapt_supplier_a_preferred_loss( state->supplier );
    }

    ASSERT( loss != NULL );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( badapt_arr_sample_batch_s, buffer_valid );
    BCORE_LIFE_CREATE( badapt_arr_sample_batch_s, buffer_batch );
    BCORE_LIFE_CREATE( bmath_vf3_s, out );

    ASSERT( badapt_adaptive_a_get_in_size( state->adaptive ) == badapt_supplier_a_get_in_size( state->supplier ) );
    ASSERT( badapt_adaptive_a_get_out_size( state->adaptive ) == badapt_supplier_a_get_out_size( state->supplier ) );


    bmath_vf3_s_set_size( out, badapt_adaptive_a_get_out_size( state->adaptive ) );

    bcore_array_a_set_size( ( bcore_array* )buffer_valid, 0 );

    badapt_arr_sample_batch_s_set_size( buffer_valid, o->valid_size );
    for( sz_t i = 0; i < o->valid_size; i++ )
    {
        badapt_supplier_a_fetch_sample_vio( state->supplier, &buffer_valid->arr_data[ i ].in, &buffer_valid->arr_data[ i ].out );
    }

    f3_t val_last_error = 0;

    for( ; state->iteration < o->max_iterations; state->iteration++ )
    {
        f3_t trn_error = 0;
        f3_t trn_weight = 0;
        for( sz_t fetch_cycle = 0; fetch_cycle < o->fetch_cycles_per_iteration; fetch_cycle++ )
        {
            bcore_array_a_set_size( ( bcore_array* )buffer_batch, 0 );

            badapt_arr_sample_batch_s_set_size( buffer_batch, o->batch_size );
            for( sz_t i = 0; i < o->batch_size; i++ )
            {
                badapt_supplier_a_fetch_sample_vio( state->supplier, &buffer_batch->arr_data[ i ].in, &buffer_batch->arr_data[ i ].out );
            }

            for( sz_t batch_cycle = 0; batch_cycle < o->batch_cycles_per_fetch; batch_cycle++ )
            {
                for( sz_t i = 0; i < o->batch_size; i++ )
                {
                    const badapt_sample_batch_s* sample = &buffer_batch->arr_data[ i ];
                    badapt_adaptive_a_adapt_loss( state->adaptive, loss, &sample->in, &sample->out, out );
                    f3_t val_loss = badapt_loss_a_loss( loss, out, &sample->out );

                    trn_error  += val_loss / sample->out.size;
                    trn_weight += 1.0;
                }
            }
        }

        f3_t val_error = 0;
        f3_t val_weight = 0;

        for( sz_t i = 0; i < o->valid_size; i++ )
        {
            const badapt_sample_batch_s* sample = &buffer_valid->arr_data[ i ];
            badapt_adaptive_a_infer( state->adaptive, &sample->in, out );
            f3_t val_loss = badapt_loss_a_loss( loss, out, &sample->out );
            val_error  += val_loss / sample->out.size;
            val_weight += 1.0;
        }

        if( trn_weight > 0 ) trn_error /= trn_weight;
        if( val_weight > 0 ) val_error /= val_weight;

        f3_t progress = 0;
        f3_t bias = 0;

        if( val_error > 0 &&      trn_error > 0 ) bias = log( val_error ) - log( trn_error );
        if( val_error > 0 && val_last_error > 0 ) progress = log( val_last_error ) - log( val_error );

        val_last_error = val_error;

        state->error    = val_error;
        state->progress = progress;
        state->bias     = bias;

        if( state->guide )
        {
            if( !badapt_guide_a_callback( state->guide, training_state ) ) break;
        }
    }

    BCORE_LIFE_DOWN();
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

