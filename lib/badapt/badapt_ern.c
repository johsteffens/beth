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

#include "bcore_std.h"
#include "bmath_std.h"
#include "bmath_plot.h"
#include "badapt_ern.h"
#include "badapt_trainer.h"

/**********************************************************************************************************************/
// badapt_ern_s

// ---------------------------------------------------------------------------------------------------------------------
#ifdef TYPEOF_badapt_ern_s
/**********************************************************************************************************************/
// badapt_ern_layer_s

void badapt_ern_layer_s_setup( badapt_ern_layer_s* o, sz_t size_input, sz_t size_hidden )
{
    bmath_vf3_s_set_size( &o->v_x, size_input );
    bmath_vf3_s_set_size( &o->v_c, size_hidden );
    bmath_vf3_s_set_size( &o->v_h, size_hidden );
    bmath_vf3_s_zro( &o->v_x );
    bmath_vf3_s_zro( &o->v_c );
    bmath_vf3_s_zro( &o->v_h  );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_ern_arr_layer_s_setup( badapt_ern_arr_layer_s* o, sz_t size_arr, sz_t size_input, sz_t size_hidden )
{
    badapt_ern_arr_layer_s_set_size( o, size_arr );
    for( sz_t i = 0; i < o->arr_size; i++ )
    {
        if( !o->arr_data[ i ] ) o->arr_data[ i ] = badapt_ern_layer_s_create();
        badapt_ern_layer_s_setup( o->arr_data[ i ], size_input, size_hidden );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_ern_arr_layer_s_rotate( badapt_ern_arr_layer_s* o )
{
    if( o->arr_size <= 1 ) return;

    badapt_ern_layer_s* last_layer = o->arr_data[ o->arr_size - 1 ];

    for( sz_t i = o->arr_size - 1; i > 0 ; i-- ) o->arr_data[ i ] = o->arr_data[ i - 1 ];

    o->arr_data[ 0 ] = last_layer;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// badapt_ern_s

// ---------------------------------------------------------------------------------------------------------------------

void badapt_ern_s_arc_to_sink( const badapt_ern_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#<sc_t>\n",  ifnameof( *(aware_t*)o ) );
    bcore_sink_a_push_fa( sink, "input:    #<sz_t>\n", o->size_input );
    bcore_sink_a_push_fa( sink, "hidden:   #<sz_t>\n", o->w_hx.rows );
    bcore_sink_a_push_fa( sink, "output:   #<sz_t>\n", o->size_output );
    bcore_sink_a_push_fa( sink, "unfolded: #<sz_t>\n", o->size_unfolded );
    bcore_sink_a_push_fa( sink, "a_h: #<sc_t> (#<sc_t>)\n", ifnameof( *( aware_t* )o->a_h ), ifnameof( *( aware_t* )badapt_activator_a_get_activation( o->a_h ) ) );
    bcore_sink_a_push_fa( sink, "a_o: #<sc_t> (#<sc_t>)\n", ifnameof( *( aware_t* )o->a_o ), ifnameof( *( aware_t* )badapt_activator_a_get_activation( o->a_o ) ) );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_ern_s_minfer( badapt_ern_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    if( o->arr_layer.arr_size != o->size_unfolded )
    {
        badapt_ern_arr_layer_s_setup( &o->arr_layer, o->size_unfolded, o->size_input, o->size_hidden );
    }
    bmath_vf3_s_set_size( &o->v_o , o->w_oh.rows );

    badapt_ern_arr_layer_s_rotate( &o->arr_layer );
    badapt_ern_layer_s* prev_layer = o->arr_layer.arr_data[ 1 % o->arr_layer.arr_size ];
    badapt_ern_layer_s* this_layer = o->arr_layer.arr_data[ 0 ];

    bmath_vf3_s_cpy(               in, &this_layer->v_x );
    bmath_vf3_s_cpy( &prev_layer->v_h, &this_layer->v_c );

    bmath_mf3_s_mul_vec(      &o->w_hx, &this_layer->v_x, &this_layer->v_h );
    bmath_mf3_s_mul_vec_add(  &o->w_hc, &this_layer->v_c, &this_layer->v_h, &this_layer->v_h );

    bmath_vf3_s_add( &o->b_h, &this_layer->v_h, &this_layer->v_h );
    badapt_activator_a_infer(  o->a_h, &this_layer->v_h,  &this_layer->v_h  );

    bmath_mf3_s_mul_vec     ( &o->w_oh, &this_layer->v_h,  &o->v_o );

    bmath_vf3_s_add( &o->b_o, &o->v_o, &o->v_o );
    badapt_activator_a_infer(  o->a_o, &o->v_o,  &o->v_o );

    if( out ) bmath_vf3_s_cpy( &o->v_o, out );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_ern_s_bgrad_adapt( badapt_ern_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out )
{
    badapt_ern_layer_s* layer0 = o->arr_layer.arr_data[ 0 ];
    bmath_vf3_s_set_size( &o->d_v_o,      o->v_o.size );
    bmath_vf3_s_set_size( &o->d_v_h, layer0->v_h.size );
    bmath_vf3_s_set_size( &o->d_v_c, layer0->v_c.size );
    bmath_vf3_s_set_size( &o->d_b_h, layer0->v_h.size );

    bmath_vf3_s_cpy( grad_out, &o->d_v_o );

    badapt_activator_a_bgrad( o->a_o, &o->d_v_o, &o->d_v_o, &o->v_o );
    bmath_vf3_s_mul_scl_f3_add( &o->d_v_o, o->dynamics.epsilon, &o->b_o, &o->b_o );

    bmath_mf3_s_htp_mul_vec( &o->w_oh, &o->d_v_o, &o->d_v_h );      // W^T * GO -> GH
    badapt_dynamics_std_s_weights_adapt( &o->dynamics, &layer0->v_h, &o->w_oh, &o->d_v_o, 1.0 );

    if( grad_in ) bmath_mf3_s_htp_mul_vec( &o->w_hx, &o->d_v_h, grad_in ); // W^T * GH -> grad_in

    for( sz_t i = 0; i < o->size_unfolded; i++ )
    {
        badapt_ern_layer_s* layer = o->arr_layer.arr_data[ i ];
        badapt_activator_a_bgrad( o->a_h, &o->d_v_h, &o->d_v_h, &layer->v_h );
        bmath_vf3_s_add( &o->d_v_h, &o->d_b_h, &o->d_b_h );

        bmath_mf3_s_htp_mul_vec( &o->w_hc, &o->d_v_h, &o->d_v_c ); // W^T * GH -> GC

        bmath_mf3_s_opd_add( &o->d_w_hx, &o->d_v_h, &layer->v_x, &o->d_w_hx );
        bmath_mf3_s_opd_add( &o->d_w_hc, &o->d_v_h, &layer->v_c, &o->d_w_hc );

        bmath_vf3_s_copy( &o->d_v_h, &o->d_v_c ); // GC -> GH
    }

    bmath_vf3_s_mul_scl_f3_add( &o->d_b_h,  o->dynamics.epsilon, &o->b_h,  &o->b_h );
    bmath_mf3_s_mul_scl_fx_add( &o->d_w_hx, o->dynamics.epsilon, &o->w_hx, &o->w_hx );
    bmath_mf3_s_mul_scl_fx_add( &o->d_w_hc, o->dynamics.epsilon, &o->w_hc, &o->w_hc );
    bmath_mf3_s_mul_scl_fx( &o->w_hx, ( 1.0 - o->dynamics.epsilon * o->dynamics.lambda_l2 ), &o->w_hx );
    bmath_mf3_s_mul_scl_fx( &o->w_hc, ( 1.0 - o->dynamics.epsilon * o->dynamics.lambda_l2 ), &o->w_hc );
    bmath_mf3_s_zro( &o->d_w_hx );
    bmath_mf3_s_zro( &o->d_w_hc );
    bmath_vf3_s_zro( &o->d_b_h );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_ern_s_get_weights_min_max( const badapt_ern_s* o, f3_t* arg_min, f3_t* arg_max )
{
    f3_t max =         bmath_mf3_s_fx_max( &o->w_hx );
    max = f3_max( max, bmath_mf3_s_fx_max( &o->w_hc ) );
    max = f3_max( max, bmath_mf3_s_fx_max( &o->w_oh ) );

    f3_t min =         bmath_mf3_s_fx_min( &o->w_hx );
    min = f3_min( min, bmath_mf3_s_fx_min( &o->w_hc ) );
    min = f3_min( min, bmath_mf3_s_fx_min( &o->w_oh ) );

    if( arg_max ) *arg_max = max;
    if( arg_min ) *arg_min = min;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// badapt_ern_builder_s

// ---------------------------------------------------------------------------------------------------------------------

badapt_adaptive* badapt_ern_builder_s_build( const badapt_ern_builder_s* o )
{
    ASSERT( o->size_input > 0 );
    ASSERT( o->size_output > 0 );

    BCORE_LIFE_INIT();

    u2_t random_state = o->random_seed;
    badapt_ern_s* ern = badapt_ern_s_create();

    ern->size_input  = o->size_input;
    ern->size_hidden = o->size_hidden;
    ern->size_output = o->size_output;
    ern->size_unfolded = o->size_unfolded;

    badapt_dynamics_std_s_copy( &ern->dynamics, &o->dynamics );

    bmath_mf3_s_set_size( &ern->w_hx, o->size_hidden, o->size_input );
    bmath_mf3_s_set_size( &ern->w_hc, o->size_hidden, o->size_hidden );
    bmath_mf3_s_set_size( &ern->w_oh, o->size_output, o->size_hidden );
    bmath_mf3_s_set_random( &ern->w_hx, false, false, 0, 1.0, -0.5, 0.5, &random_state );

    f3_t v_limit = 1.0 / f3_srt( o->size_hidden );
    bmath_mf3_s_set_random( &ern->w_hc, false, false, 0, 1.0, -v_limit, v_limit, &random_state );
    bmath_mf3_s_set_random( &ern->w_oh, false, false, 0, 1.0, -0.5, 0.5, &random_state );

    bmath_mf3_s_set_size( &ern->d_w_hx, ern->w_hx.rows, ern->w_hx.cols );
    bmath_mf3_s_set_size( &ern->d_w_hc, ern->w_hc.rows, ern->w_hc.cols );
    bmath_mf3_s_zro( &ern->d_w_hx );
    bmath_mf3_s_zro( &ern->d_w_hc );
    bmath_vf3_s_set_size( &ern->b_h, o->size_hidden );
    bmath_vf3_s_set_size( &ern->b_o, o->size_output );
    bmath_vf3_s_zro( &ern->b_h );
    bmath_vf3_s_zro( &ern->b_o );

    ern->a_h = badapt_activator_a_clone( o->a_h );
    ern->a_o = badapt_activator_a_clone( o->a_o );

    BCORE_LIFE_RETURNV( badapt_adaptive*, ( badapt_adaptive* )ern );
}

// ---------------------------------------------------------------------------------------------------------------------

#endif

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t badapt_ern_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_ern" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


