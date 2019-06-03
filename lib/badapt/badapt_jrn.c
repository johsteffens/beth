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

#include "bcore_std.h"
#include "bmath_std.h"
#include "bmath_plot.h"
#include "badapt_jrn.h"
#include "badapt_trainer.h"
#include "badapt_problem.h"

/**********************************************************************************************************************/
// badapt_jrn_s

// ---------------------------------------------------------------------------------------------------------------------
#ifdef TYPEOF_badapt_jrn_s
sz_t badapt_jrn_s_get_in_size(  const badapt_jrn_s* o )
{
    return o->size_input;
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t badapt_jrn_s_get_out_size( const badapt_jrn_s* o )
{
    return o->size_output;
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_jrn_s_get_dynamics_std( const badapt_jrn_s* o, badapt_dynamics_std_s* dynamics )
{
    badapt_dynamics_std_s_copy( dynamics, &o->dynamics );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_jrn_s_set_dynamics_std( badapt_jrn_s* o, const badapt_dynamics_std_s* dynamics )
{
    badapt_dynamics_std_s_copy( &o->dynamics, dynamics );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_jrn_s_arc_to_sink( const badapt_jrn_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#<sc_t>\n",  ifnameof( *(aware_t*)o ) );
    bcore_sink_a_push_fa( sink, "size input:  #<sz_t>\n", o->size_input );
    bcore_sink_a_push_fa( sink, "size hidden: #<sz_t>\n", o->wgt_input.rows );
    bcore_sink_a_push_fa( sink, "size output: #<sz_t>\n", o->size_output );
    bcore_sink_a_push_fa( sink, "context epsilon factor: #<f3_t>\n", o->context_epsilon_factor );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_jrn_s_minfer( badapt_jrn_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    ASSERT( in->size == o->size_input );

    bmath_vf3_s_copy( &o->vec_input, in );
    bmath_vf3_s_swapr( &o->vec_output, &o->vec_context );
    bmath_vf3_s_set_size( &o->vec_hidden, o->wgt_input.rows );

    if( o->vec_context.size > 0 )
    {
        bmath_mf3_s_mul_vec(     &o->wgt_context, &o->vec_context, &o->vec_hidden );
        bmath_mf3_s_mul_vec_add( &o->wgt_input, &o->vec_input, &o->vec_hidden, &o->vec_hidden );
    }
    else
    {
        bmath_mf3_s_mul_vec( &o->wgt_input, &o->vec_input, &o->vec_hidden );
    }

    badapt_activator_a_infer( o->activator_hidden, &o->vec_hidden, &o->vec_hidden );

    bmath_vf3_s_set_size( &o->vec_output, o->wgt_hidden.rows );
    bmath_mf3_s_mul_vec( &o->wgt_hidden, &o->vec_hidden, &o->vec_output );
    badapt_activator_a_infer( o->activator_output, &o->vec_output, &o->vec_output );

    if( out ) bmath_vf3_s_cpy( &o->vec_output, out );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_jrn_s_bgrad( const badapt_jrn_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out )
{
    bmath_vf3_s* grad_output = bmath_vf3_s_create_set_size( o->wgt_hidden.rows );
    bmath_vf3_s* grad_hidden = bmath_vf3_s_create_set_size( o->wgt_input.rows );

    bmath_vf3_s_cpy( grad_out, grad_output );

    badapt_activator_a_adapt( o->activator_output, grad_output, grad_output, &o->vec_output, o->dynamics.epsilon );
    bmath_mf3_s_htp_mul_vec( &o->wgt_hidden, grad_output, grad_hidden );          // GH <- W^T * GO
    badapt_activator_a_adapt( o->activator_hidden, grad_hidden, grad_hidden, &o->vec_hidden, o->dynamics.epsilon );
    if( grad_in ) bmath_mf3_s_htp_mul_vec( &o->wgt_input, grad_hidden, grad_in ); // grad_in <- W^T * GH

    bmath_vf3_s_discard( grad_hidden );
    bmath_vf3_s_discard( grad_output );
}

// ---------------------------------------------------------------------------------------------------------------------

static void badapt_jrn_s_update_wgt( const badapt_jrn_s* o, const bmath_vf3_s* vec_in, bmath_mf3_s* w, const bmath_vf3_s* grad_out, f3_t epsilon )
{
    assert( w->rows == grad_out->size );
    assert( w->cols == vec_in->size );

    f3_t l2_reg_factor = ( 1.0 - o->dynamics.lambda_l2  * epsilon );
    f3_t l1_reg_offset = o->dynamics.lambda_l1 * epsilon;

    for( sz_t i = 0; i < w->rows; i++ )
    {
        f3_t* wr = w->data + i * w->stride;
        f3_t gi = epsilon * grad_out->data[ i ];
        for( sz_t j = 0; j < w->cols; j++ ) wr[ j ] = ( wr[ j ] + vec_in->data[ j ] * gi ) * l2_reg_factor;
        if( l1_reg_offset > 0 )
        {
            for( sz_t j = 0; j < w->cols; j++ ) wr[ j ] += ( wr[ j ] > 0 ) ? -l1_reg_offset : l1_reg_offset;
        }
    }
    assert( !bmath_mf3_s_is_nan( w ) );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_jrn_s_bgrad_adapt( badapt_jrn_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out )
{
    bmath_vf3_s* grad_output = bmath_vf3_s_create_set_size( o->wgt_hidden.rows );
    bmath_vf3_s* grad_hidden = bmath_vf3_s_create_set_size( o->wgt_input.rows );

    bmath_vf3_s_cpy( grad_out, grad_output );

    badapt_activator_a_adapt( o->activator_output, grad_output, grad_output, &o->vec_output, o->dynamics.epsilon );
    bmath_mf3_s_htp_mul_vec( &o->wgt_hidden, grad_output, grad_hidden );          // GH <- W^T * GO
    badapt_jrn_s_update_wgt( o, &o->vec_hidden, &o->wgt_hidden, grad_output, o->dynamics.epsilon );
    badapt_activator_a_adapt( o->activator_hidden, grad_hidden, grad_hidden, &o->vec_hidden, o->dynamics.epsilon );
    if( grad_in ) bmath_mf3_s_htp_mul_vec( &o->wgt_input, grad_hidden, grad_in ); // grad_in <- W^T * GH
    badapt_jrn_s_update_wgt( o, &o->vec_input, &o->wgt_input, grad_hidden, o->dynamics.epsilon );
    if( o->vec_context.size > 0 )
    {
        badapt_jrn_s_update_wgt( o, &o->vec_context, &o->wgt_context, grad_hidden, o->dynamics.epsilon * o->context_epsilon_factor );
    }

    bmath_vf3_s_discard( grad_hidden );
    bmath_vf3_s_discard( grad_output );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// badapt_builder_jrn_s

// ---------------------------------------------------------------------------------------------------------------------

sz_t badapt_builder_jrn_s_get_in_size( const badapt_builder_jrn_s* o )
{
    return o->size_input;
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_builder_jrn_s_set_in_size( badapt_builder_jrn_s* o, sz_t size )
{
    o->size_input = size;
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t badapt_builder_jrn_s_get_out_size( const badapt_builder_jrn_s* o )
{
    return o->size_output;
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_builder_jrn_s_set_out_size( badapt_builder_jrn_s* o, sz_t size )
{
    o->size_output = size;
}

// ---------------------------------------------------------------------------------------------------------------------

badapt_adaptive* badapt_builder_jrn_s_build( const badapt_builder_jrn_s* o )
{
    ASSERT( o->size_input > 0 );
    ASSERT( o->size_output > 0 );

    BCORE_LIFE_INIT();

    u2_t random_state = o->random_seed;
    badapt_jrn_s* ern = badapt_jrn_s_create();

    ern->size_input  = o->size_input;
    ern->size_output = o->size_output;
    ern->context_epsilon_factor = o->context_epsilon_factor;

    badapt_dynamics_std_s_copy( &ern->dynamics, &o->dynamics );

    bmath_mf3_s_set_size( &ern->wgt_input,   o->size_hidden, o->size_input );
    bmath_mf3_s_set_size( &ern->wgt_context, o->size_hidden, o->size_output );
    bmath_mf3_s_set_size( &ern->wgt_hidden,  o->size_output, o->size_hidden );
    bmath_mf3_s_set_random( &ern->wgt_input,   false, false, 0, 1.0, -0.5, 0.5, &random_state );
    bmath_mf3_s_set_random( &ern->wgt_context, false, false, 0, 1.0, -0.1, 0.1, &random_state );
    bmath_mf3_s_set_random( &ern->wgt_hidden,  false, false, 0, 1.0, -0.5, 0.5, &random_state );

    ern->activator_hidden = badapt_activator_a_clone( o->activator_hidden );
    ern->activator_output = badapt_activator_a_clone( o->activator_output );

    BCORE_LIFE_RETURN( ( badapt_adaptive* )ern );
}

// ---------------------------------------------------------------------------------------------------------------------

#endif

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t badapt_jrn_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_jrn" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
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

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


