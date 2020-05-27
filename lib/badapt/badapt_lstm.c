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
#include "badapt_lstm.h"
#include "badapt_trainer.h"

#ifdef TYPEOF_badapt_lstm_s

/**********************************************************************************************************************/
// badapt_lstm_layer_s

void badapt_lstm_layer_s_setup( badapt_lstm_layer_s* o, sz_t size_input, sz_t size_hidden )
{
    bmath_vf3_s_set_size( &o->v_x, size_input );
    bmath_vf3_s_set_size( &o->v_f, size_hidden );
    bmath_vf3_s_set_size( &o->v_i, size_hidden );
    bmath_vf3_s_set_size( &o->v_o, size_hidden );
    bmath_vf3_s_set_size( &o->v_q, size_hidden );
    bmath_vf3_s_set_size( &o->v_c, size_hidden );
    bmath_vf3_s_set_size( &o->v_d, size_hidden );
    bmath_vf3_s_set_size( &o->v_h, size_hidden );

    bmath_vf3_s_zro( &o->v_x );
    bmath_vf3_s_zro( &o->v_f );
    bmath_vf3_s_zro( &o->v_i );
    bmath_vf3_s_zro( &o->v_o );
    bmath_vf3_s_zro( &o->v_q );
    bmath_vf3_s_zro( &o->v_c );
    bmath_vf3_s_zro( &o->v_d );
    bmath_vf3_s_zro( &o->v_h );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_lstm_arr_layer_s_setup( badapt_lstm_arr_layer_s* o, sz_t size_arr, sz_t size_input, sz_t size_hidden )
{
    badapt_lstm_arr_layer_s_set_size( o, size_arr );
    for( sz_t i = 0; i < o->arr_size; i++ )
    {
        if( !o->arr_data[ i ] ) o->arr_data[ i ] = badapt_lstm_layer_s_create();
        badapt_lstm_layer_s_setup( o->arr_data[ i ], size_input, size_hidden );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_lstm_arr_layer_s_rotate( badapt_lstm_arr_layer_s* o )
{
    if( o->arr_size <= 1 ) return;

    badapt_lstm_layer_s* last_layer = o->arr_data[ o->arr_size - 1 ];

    for( sz_t i = o->arr_size - 1; i > 0 ; i-- ) o->arr_data[ i ] = o->arr_data[ i - 1 ];

    o->arr_data[ 0 ] = last_layer;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// badapt_lstm_s

void badapt_lstm_s_arc_to_sink( const badapt_lstm_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#<sc_t>\n",  ifnameof( *(aware_t*)o ) );
    bcore_sink_a_push_fa( sink, "input:    #<sz_t>\n", o->size_input );
    bcore_sink_a_push_fa( sink, "hidden:   #<sz_t>\n", o->size_hidden );
    bcore_sink_a_push_fa( sink, "output:   #<sz_t>\n", o->size_output );
    bcore_sink_a_push_fa( sink, "unfolded: #<sz_t>\n", o->size_unfolded );

    tp_t t_f = badapt_activator_a_get_activation( o->a_f ) ? *( aware_t* )badapt_activator_a_get_activation( o->a_f ) : 0;
    tp_t t_i = badapt_activator_a_get_activation( o->a_i ) ? *( aware_t* )badapt_activator_a_get_activation( o->a_i ) : 0;
    tp_t t_o = badapt_activator_a_get_activation( o->a_o ) ? *( aware_t* )badapt_activator_a_get_activation( o->a_o ) : 0;
    tp_t t_q = badapt_activator_a_get_activation( o->a_q ) ? *( aware_t* )badapt_activator_a_get_activation( o->a_q ) : 0;
    tp_t t_d = badapt_activator_a_get_activation( o->a_d ) ? *( aware_t* )badapt_activator_a_get_activation( o->a_d ) : 0;
    tp_t t_r = badapt_activator_a_get_activation( o->a_r ) ? *( aware_t* )badapt_activator_a_get_activation( o->a_r ) : 0;

    bcore_sink_a_push_fa( sink, "a_f: #<sc_t> (#<sc_t>)\n", ifnameof( *( aware_t* )o->a_f ), ifnameof( t_f ) );
    bcore_sink_a_push_fa( sink, "a_i: #<sc_t> (#<sc_t>)\n", ifnameof( *( aware_t* )o->a_i ), ifnameof( t_i ) );
    bcore_sink_a_push_fa( sink, "a_o: #<sc_t> (#<sc_t>)\n", ifnameof( *( aware_t* )o->a_o ), ifnameof( t_o ) );
    bcore_sink_a_push_fa( sink, "a_q: #<sc_t> (#<sc_t>)\n", ifnameof( *( aware_t* )o->a_q ), ifnameof( t_q ) );
    bcore_sink_a_push_fa( sink, "a_d: #<sc_t> (#<sc_t>)\n", ifnameof( *( aware_t* )o->a_d ), ifnameof( t_d ) );
    bcore_sink_a_push_fa( sink, "a_r: #<sc_t> (#<sc_t>)\n", ifnameof( *( aware_t* )o->a_r ), ifnameof( t_r ) );
}

// ---------------------------------------------------------------------------------------------------------------------

/**
 *    Recurrent inference:
 *
 *    // Input/Output
 *     v_x - input
 *    pv_h - hidden     of previous layer
 *    pv_c - cell-state of previous layer
 *     v_h - hidden     (recurrent pass-on & output)
 *     v_c - cell-state (recurrent pass-on)
 *
 *    v_f = a_f( w_f * v_x : pv_h + b_f )
 *    v_i = a_i( w_i * v_x : pv_h + b_i )
 *    v_o = a_o( w_o * v_x : pv_h + b_o )
 *    v_q = a_q( w_q * v_x : pv_h + b_q )
 *    v_c = v_f o pv_c + v_i o v_q
 *    v_d = a_d( v_c )
 *    v_h = v_o o v_d
 *
 *    Output inference:
 *    v_r = a_r( w_rh * v_h + b_r )
 */
void badapt_lstm_s_minfer( badapt_lstm_s* o, const bmath_vf3_s* in, bmath_vf3_s* out )
{
    if( o->arr_layer.arr_size != o->size_unfolded ) badapt_lstm_s_reset( o );

    ASSERT( o->arr_layer.arr_size >= 2 );

    badapt_lstm_arr_layer_s_rotate( &o->arr_layer );
    badapt_lstm_layer_s* l = o->arr_layer.arr_data[ 0 ]; // this layer
    badapt_lstm_layer_s* p = o->arr_layer.arr_data[ 1 ]; // previous layer

    // input & recurrent cycle
    bmath_vf3_s_cpy( in, &l->v_x );

    // v_f = a_f( w_f * v_x : pv_h + b_f )
    bmath_mf3_s_mul_vec(     &o->w_fx, &l->v_x, &l->v_f );
    bmath_mf3_s_mul_vec_add( &o->w_fh, &p->v_h, &l->v_f, &l->v_f );
    bmath_vf3_s_add(         &l->v_f,  &o->b_f, &l->v_f );
    badapt_activator_a_infer( o->a_f , &l->v_f, &l->v_f );

    // v_i = a_i( w_i * v_x : pv_h + b_i )
    bmath_mf3_s_mul_vec(     &o->w_ix, &l->v_x, &l->v_i );
    bmath_mf3_s_mul_vec_add( &o->w_ih, &p->v_h, &l->v_i, &l->v_i );
    bmath_vf3_s_add(         &l->v_i,  &o->b_i, &l->v_i );
    badapt_activator_a_infer( o->a_i , &l->v_i, &l->v_i );

    // v_o = a_o( w_o * v_x : pv_h + b_o )
    bmath_mf3_s_mul_vec(     &o->w_ox, &l->v_x, &l->v_o );
    bmath_mf3_s_mul_vec_add( &o->w_oh, &p->v_h, &l->v_o, &l->v_o );
    bmath_vf3_s_add(         &l->v_o,  &o->b_o, &l->v_o );
    badapt_activator_a_infer( o->a_o , &l->v_o, &l->v_o );

    // v_q = a_q( w_q * v_x : pv_h + b_q )
    bmath_mf3_s_mul_vec(     &o->w_qx, &l->v_x, &l->v_q );
    bmath_mf3_s_mul_vec_add( &o->w_qh, &p->v_h, &l->v_q, &l->v_q );
    bmath_vf3_s_add(         &l->v_q,  &o->b_q, &l->v_q );
    badapt_activator_a_infer( o->a_q , &l->v_q, &l->v_q );

    // v_c = v_f o pv_c + v_i o v_q
    bmath_vf3_s_mul_hdm(     &l->v_f, &p->v_c, &l->v_c );
    bmath_vf3_s_mul_hdm_add( &l->v_i, &l->v_q, &l->v_c, &l->v_c );

    // v_d = a_d( v_c )
    badapt_activator_a_infer( o->a_d, &l->v_c, &l->v_d );

    // v_h = v_o o v_d
    bmath_vf3_s_mul_hdm(     &l->v_o, &l->v_d, &l->v_h );

    // v_r = a_r( w_rh * v_h + b_r )
    bmath_mf3_s_mul_vec(     &o->w_rh, &l->v_h, &o->v_r );
    bmath_vf3_s_add(         &o->v_r , &o->b_r, &o->v_r );
    badapt_activator_a_infer( o->a_r , &o->v_r, &o->v_r );

    if( out ) bmath_vf3_s_cpy( &o->v_r, out );
}

// ---------------------------------------------------------------------------------------------------------------------
/**
 *    Output gradients
 *    given: d_v_r
 *
 *    d_v_r = ( dy_a_r( v_r ) o d_v_r )
 *    d_v_h = w_hr * d_v_r;
 *    d_v_c = 0
 *
 *    d_b_r  += d_v_r
 *    d_w_rh += d_v_r x v_h
 *
 *    Recurrent gradients
 *    input grads: d_v_h, d_v_c
 *
 *    d_v_d = v_o o d_v_h
 *    d_v_o = v_d o d_v_h
 *    d_v_c = d_v_c + dy_a_d( v_d ) o d_v_d
 *    d_v_q =  v_i o d_v_c
 *    d_v_i =  v_q o d_v_c
 *    d_v_f = pv_c o d_v_c
 *    d_v_c =  v_f o d_v_c
 *
 *    d_v_f = dy_a_f( v_f ) o d_v_f
 *    d_v_i = dy_a_i( v_i ) o d_v_i
 *    d_v_o = dy_a_o( v_o ) o d_v_o
 *    d_v_q = dy_a_q( v_q ) o d_v_q
 *
 *    d_v_h = w_hf * d_v_f + w_hi * d_v_i + w_ho * d_v_o + w_hq * d_v_q
 *    d_v_x = w_xf * d_v_f + w_xi * d_v_i + w_xo * d_v_o + w_xq * d_v_q // d_v_x only needed if gradient is returned
 *
 *    d_b_f += d_v_f
 *    d_b_i += d_v_i
 *    d_b_o += d_v_o
 *    d_b_q += d_v_q
 *
 *    d_w_fx += d_v_f x v_x
 *    d_w_ix += d_v_i x v_x
 *    d_w_ox += d_v_o x v_x
 *    d_w_qx += d_v_q x v_x
 *
 *    d_w_fh += d_v_f x pv_h
 *    d_w_ih += d_v_i x pv_h
 *    d_w_oh += d_v_o x pv_h
 *    d_w_qh += d_v_q x pv_h
 */
void badapt_lstm_s_bgrad_adapt( badapt_lstm_s* o, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out )
{
    bmath_vf3_s_cpy( grad_out, &o->d_v_r );

    const badapt_lstm_layer_s* l = o->arr_layer.arr_data[ 0 ];
    const badapt_lstm_layer_s* p = NULL;

    // d_v_r = dy_a_r( v_r ) o d_v_r
    badapt_activator_a_bgrad( o->a_r , &o->d_v_r, &o->d_v_r, &o->v_r );

    // d_b_r  += d_v_r --> apply to b_r
    bmath_vf3_s_mul_scl_f3_add( &o->d_v_r, o->dynamics.epsilon, &o->b_r, &o->b_r );

    // d_v_h = w_hr * d_v_r
    bmath_mf3_s_htp_mul_vec( &o->w_rh, &o->d_v_r, &o->d_v_h );

    // d_w_rh += d_v_r x v_h  --> apply to w_rh
    badapt_dynamics_std_s_weights_adapt( &o->dynamics, &l->v_h, &o->w_rh, &o->d_v_r, 1.0 );

    // d_v_c = 0
    bmath_vf3_s_zro( &o->d_v_c );

    for( sz_t i = 0; i < o->size_unfolded; i++ )
    {
        l = o->arr_layer.arr_data[ i ];
        p = ( i < o->size_unfolded - 1 ) ? o->arr_layer.arr_data[ i + 1 ] : NULL;

        // d_v_d  = v_o o d_v_h
        bmath_vf3_s_mul_hdm( &l->v_o, &o->d_v_h, &o->d_v_d );

        // d_v_o  = v_d o d_v_h
        bmath_vf3_s_mul_hdm( &l->v_d, &o->d_v_h, &o->d_v_o );

        // d_v_c  = d_v_c + dy_a_d( v_d ) o d_v_d
        badapt_activator_a_bgrad( o->a_d, &o->d_v_d, &o->d_v_d, &l->v_d );
        bmath_vf3_s_add( &o->d_v_c, &o->d_v_d, &o->d_v_c );

        // d_v_q  =  v_i o d_v_c
        bmath_vf3_s_mul_hdm( &l->v_i, &o->d_v_c, &o->d_v_q );

        // d_v_i  =  v_q o d_v_c
        bmath_vf3_s_mul_hdm( &l->v_q, &o->d_v_c, &o->d_v_i );

        // d_v_f  = pv_c o d_v_c
        if( p )
        {
            bmath_vf3_s_mul_hdm( &p->v_c, &o->d_v_c, &o->d_v_f );
        }
        else
        {
            bmath_vf3_s_zro( &o->d_v_f );
        }

        // d_v_c  =  v_f o d_v_c
        bmath_vf3_s_mul_hdm( &l->v_f, &o->d_v_c, &o->d_v_c );

        // d_v_f = dy_a_f( v_f ) o d_v_f
        badapt_activator_a_bgrad( o->a_f, &o->d_v_f, &o->d_v_f, &l->v_f );

        // d_v_i = dy_a_i( v_i ) o d_v_i
        badapt_activator_a_bgrad( o->a_i, &o->d_v_i, &o->d_v_i, &l->v_i );

        // d_v_o = dy_a_o( v_o ) o d_v_o
        badapt_activator_a_bgrad( o->a_o, &o->d_v_o, &o->d_v_o, &l->v_o );

        // d_v_q = dy_a_q( v_q ) o d_v_q
        badapt_activator_a_bgrad( o->a_q, &o->d_v_q, &o->d_v_q, &l->v_q );

        // d_v_h = w_hf * d_v_f + w_hi * d_v_i + w_ho * d_v_o + w_hq * d_v_q
        bmath_mf3_s_htp_mul_vec(     &o->w_fh, &o->d_v_f,            &o->d_v_h );
        bmath_mf3_s_htp_mul_vec_add( &o->w_ih, &o->d_v_i, &o->d_v_h, &o->d_v_h );
        bmath_mf3_s_htp_mul_vec_add( &o->w_oh, &o->d_v_o, &o->d_v_h, &o->d_v_h );
        bmath_mf3_s_htp_mul_vec_add( &o->w_qh, &o->d_v_q, &o->d_v_h, &o->d_v_h );

        // d_v_x = w_xf * d_v_f + w_xi * d_v_i + w_xo * d_v_o + w_xq * d_v_q // d_v_x only needed if gradient is returned
        if( grad_in && i == 0 ) // d_v_x == grad_in
        {
            bmath_mf3_s_htp_mul_vec(     &o->w_fx, &o->d_v_f,          grad_in );
            bmath_mf3_s_htp_mul_vec_add( &o->w_ix, &o->d_v_i, grad_in, grad_in );
            bmath_mf3_s_htp_mul_vec_add( &o->w_ox, &o->d_v_o, grad_in, grad_in );
            bmath_mf3_s_htp_mul_vec_add( &o->w_qx, &o->d_v_q, grad_in, grad_in );
        }

        // d_b_f += d_v_f
        bmath_vf3_s_add( &o->d_v_f, &o->d_b_f, &o->d_b_f );

        // d_b_i += d_v_i
        bmath_vf3_s_add( &o->d_v_i, &o->d_b_i, &o->d_b_i );

        // d_b_o += d_v_o
        bmath_vf3_s_add( &o->d_v_o, &o->d_b_o, &o->d_b_o );

        // d_b_q += d_v_q
        bmath_vf3_s_add( &o->d_v_q, &o->d_b_q, &o->d_b_q );


        // d_w_fx += d_v_f x v_x
        bmath_mf3_s_opd_add( &o->d_w_fx, &o->d_v_f, &l->v_x, &o->d_w_fx );

        // d_w_ix += d_v_i x v_x
        bmath_mf3_s_opd_add( &o->d_w_ix, &o->d_v_i, &l->v_x, &o->d_w_ix );

        // d_w_ox += d_v_o x v_x
        bmath_mf3_s_opd_add( &o->d_w_ox, &o->d_v_o, &l->v_x, &o->d_w_ox );

        // d_w_qx += d_v_q x v_x
        bmath_mf3_s_opd_add( &o->d_w_qx, &o->d_v_q, &l->v_x, &o->d_w_qx );

        if( p )
        {
            // d_w_fh += d_v_f x pv_h
            bmath_mf3_s_opd_add( &o->d_w_fh, &o->d_v_f, &p->v_h, &o->d_w_fh );

            // d_w_ih += d_v_i x pv_h
            bmath_mf3_s_opd_add( &o->d_w_ih, &o->d_v_i, &p->v_h, &o->d_w_ih );

            // d_w_oh += d_v_o x pv_h
            bmath_mf3_s_opd_add( &o->d_w_oh, &o->d_v_o, &p->v_h, &o->d_w_oh );

            // d_w_qh += d_v_q x pv_h
            bmath_mf3_s_opd_add( &o->d_w_qh, &o->d_v_q, &p->v_h, &o->d_w_qh );
        }
    }

    // apply gradients
    bmath_mf3_s_mul_scl_fx_add( &o->d_w_fx, o->dynamics.epsilon, &o->w_fx, &o->w_fx );
    bmath_mf3_s_mul_scl_fx_add( &o->d_w_ix, o->dynamics.epsilon, &o->w_ix, &o->w_ix );
    bmath_mf3_s_mul_scl_fx_add( &o->d_w_ox, o->dynamics.epsilon, &o->w_ox, &o->w_ox );
    bmath_mf3_s_mul_scl_fx_add( &o->d_w_qx, o->dynamics.epsilon, &o->w_qx, &o->w_qx );
    bmath_mf3_s_mul_scl_fx_add( &o->d_w_fh, o->dynamics.epsilon, &o->w_fh, &o->w_fh );
    bmath_mf3_s_mul_scl_fx_add( &o->d_w_ih, o->dynamics.epsilon, &o->w_ih, &o->w_ih );
    bmath_mf3_s_mul_scl_fx_add( &o->d_w_oh, o->dynamics.epsilon, &o->w_oh, &o->w_oh );
    bmath_mf3_s_mul_scl_fx_add( &o->d_w_qh, o->dynamics.epsilon, &o->w_qh, &o->w_qh );

    bmath_vf3_s_mul_scl_f3_add( &o->d_b_f, o->dynamics.epsilon, &o->b_f, &o->b_f );
    bmath_vf3_s_mul_scl_f3_add( &o->d_b_i, o->dynamics.epsilon, &o->b_i, &o->b_i );
    bmath_vf3_s_mul_scl_f3_add( &o->d_b_o, o->dynamics.epsilon, &o->b_o, &o->b_o );
    bmath_vf3_s_mul_scl_f3_add( &o->d_b_q, o->dynamics.epsilon, &o->b_q, &o->b_q );

    if( o->dynamics.lambda_l2 > 0 )
    {
        f3_t factor = ( 1.0 - o->dynamics.epsilon * o->dynamics.lambda_l2 );
        bmath_mf3_s_mul_scl_fx( &o->w_fx, factor, &o->w_fx );
        bmath_mf3_s_mul_scl_fx( &o->w_ix, factor, &o->w_ix );
        bmath_mf3_s_mul_scl_fx( &o->w_ox, factor, &o->w_ox );
        bmath_mf3_s_mul_scl_fx( &o->w_qx, factor, &o->w_qx );
        bmath_mf3_s_mul_scl_fx( &o->w_fh, factor, &o->w_fh );
        bmath_mf3_s_mul_scl_fx( &o->w_ih, factor, &o->w_ih );
        bmath_mf3_s_mul_scl_fx( &o->w_oh, factor, &o->w_oh );
        bmath_mf3_s_mul_scl_fx( &o->w_qh, factor, &o->w_qh );

        bmath_vf3_s_mul_scl_f3( &o->b_f, factor, &o->b_f );
        bmath_vf3_s_mul_scl_f3( &o->b_i, factor, &o->b_i );
        bmath_vf3_s_mul_scl_f3( &o->b_o, factor, &o->b_o );
        bmath_vf3_s_mul_scl_f3( &o->b_q, factor, &o->b_q );
    }

    bmath_mf3_s_zro( &o->d_w_fx );
    bmath_mf3_s_zro( &o->d_w_ix );
    bmath_mf3_s_zro( &o->d_w_ox );
    bmath_mf3_s_zro( &o->d_w_qx );
    bmath_mf3_s_zro( &o->d_w_fh );
    bmath_mf3_s_zro( &o->d_w_ih );
    bmath_mf3_s_zro( &o->d_w_oh );
    bmath_mf3_s_zro( &o->d_w_qh );

    bmath_vf3_s_zro( &o->d_b_f );
    bmath_vf3_s_zro( &o->d_b_i );
    bmath_vf3_s_zro( &o->d_b_o );
    bmath_vf3_s_zro( &o->d_b_q );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_lstm_s_reset( badapt_lstm_s* o )
{
    bmath_vf3_s_set_size( &o->d_b_f, o->size_hidden );
    bmath_vf3_s_set_size( &o->d_b_i, o->size_hidden );
    bmath_vf3_s_set_size( &o->d_b_o, o->size_hidden );
    bmath_vf3_s_set_size( &o->d_b_q, o->size_hidden );

    bmath_mf3_s_set_size( &o->d_w_fx, o->size_hidden, o->size_input );
    bmath_mf3_s_set_size( &o->d_w_ix, o->size_hidden, o->size_input );
    bmath_mf3_s_set_size( &o->d_w_ox, o->size_hidden, o->size_input );
    bmath_mf3_s_set_size( &o->d_w_qx, o->size_hidden, o->size_input );
    bmath_mf3_s_set_size( &o->d_w_fh, o->size_hidden, o->size_hidden );
    bmath_mf3_s_set_size( &o->d_w_ih, o->size_hidden, o->size_hidden );
    bmath_mf3_s_set_size( &o->d_w_oh, o->size_hidden, o->size_hidden );
    bmath_mf3_s_set_size( &o->d_w_qh, o->size_hidden, o->size_hidden );

    bmath_vf3_s_set_size( &o->v_r,   o->size_output );
    bmath_vf3_s_set_size( &o->d_v_r, o->size_output );
    bmath_vf3_s_set_size( &o->d_v_f, o->size_hidden );
    bmath_vf3_s_set_size( &o->d_v_i, o->size_hidden );
    bmath_vf3_s_set_size( &o->d_v_o, o->size_hidden );
    bmath_vf3_s_set_size( &o->d_v_q, o->size_hidden );
    bmath_vf3_s_set_size( &o->d_v_c, o->size_hidden );
    bmath_vf3_s_set_size( &o->d_v_d, o->size_hidden );
    bmath_vf3_s_set_size( &o->d_v_h, o->size_hidden );

    bmath_mf3_s_zro( &o->d_w_fx );
    bmath_mf3_s_zro( &o->d_w_ix );
    bmath_mf3_s_zro( &o->d_w_ox );
    bmath_mf3_s_zro( &o->d_w_qx );
    bmath_mf3_s_zro( &o->d_w_fh );
    bmath_mf3_s_zro( &o->d_w_ih );
    bmath_mf3_s_zro( &o->d_w_oh );
    bmath_mf3_s_zro( &o->d_w_qh );

    bmath_vf3_s_zro( &o->v_r   );
    bmath_vf3_s_zro( &o->d_v_r );
    bmath_vf3_s_zro( &o->d_v_f );
    bmath_vf3_s_zro( &o->d_v_i );
    bmath_vf3_s_zro( &o->d_v_o );
    bmath_vf3_s_zro( &o->d_v_q );
    bmath_vf3_s_zro( &o->d_v_c );
    bmath_vf3_s_zro( &o->d_v_d );
    bmath_vf3_s_zro( &o->d_v_h );

    bmath_vf3_s_zro( &o->d_b_f );
    bmath_vf3_s_zro( &o->d_b_i );
    bmath_vf3_s_zro( &o->d_b_o );
    bmath_vf3_s_zro( &o->d_b_q );

    badapt_lstm_arr_layer_s_setup( &o->arr_layer, o->size_unfolded, o->size_input, o->size_hidden );
}

// ---------------------------------------------------------------------------------------------------------------------

void badapt_lstm_s_get_weights_min_max( const badapt_lstm_s* o, f3_t* arg_min, f3_t* arg_max )
{
    f3_t max =         bmath_mf3_s_fx_max( &o->w_fx );
    max = f3_max( max, bmath_mf3_s_fx_max( &o->w_ix ) );
    max = f3_max( max, bmath_mf3_s_fx_max( &o->w_ox ) );
    max = f3_max( max, bmath_mf3_s_fx_max( &o->w_qx ) );
    max = f3_max( max, bmath_mf3_s_fx_max( &o->w_fh ) );
    max = f3_max( max, bmath_mf3_s_fx_max( &o->w_ih ) );
    max = f3_max( max, bmath_mf3_s_fx_max( &o->w_oh ) );
    max = f3_max( max, bmath_mf3_s_fx_max( &o->w_qh ) );
    max = f3_max( max, bmath_mf3_s_fx_max( &o->w_rh ) );

    f3_t min =         bmath_mf3_s_fx_min( &o->w_fx );
    min = f3_min( min, bmath_mf3_s_fx_min( &o->w_ix ) );
    min = f3_min( min, bmath_mf3_s_fx_min( &o->w_ox ) );
    min = f3_min( min, bmath_mf3_s_fx_min( &o->w_qx ) );
    min = f3_min( min, bmath_mf3_s_fx_min( &o->w_fh ) );
    min = f3_min( min, bmath_mf3_s_fx_min( &o->w_ih ) );
    min = f3_min( min, bmath_mf3_s_fx_min( &o->w_oh ) );
    min = f3_min( min, bmath_mf3_s_fx_min( &o->w_qh ) );
    min = f3_min( min, bmath_mf3_s_fx_min( &o->w_rh ) );

    if( arg_max ) *arg_max = max;
    if( arg_min ) *arg_min = min;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// badapt_lstm_builder_s

// ---------------------------------------------------------------------------------------------------------------------

void badapt_lstm_builder_s_init_x( badapt_lstm_builder_s* o )
{
    o->a_f = badapt_activator_create_from_names( "plain", "sigm" );
    o->a_i = badapt_activator_create_from_names( "plain", "sigm" );
    o->a_o = badapt_activator_create_from_names( "plain", "sigm" );
    o->a_q = badapt_activator_create_from_names( "plain", "tanh" );
    o->a_d = badapt_activator_create_from_names( "plain", "tanh" );
//    o->a_r = badapt_activator_create_from_names( "plain", "sigm" );
    o->a_r = badapt_activator_create_from_names( "softmax", NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

/// sets activator to simulate an elman recurrent network
void badapt_lstm_builder_s_set_ern( badapt_lstm_builder_s* o )
{
    badapt_activator_a_discard( o->a_f ); o->a_f = badapt_activator_create_from_names( "plain", "zero" );
    badapt_activator_a_discard( o->a_i ); o->a_i = badapt_activator_create_from_names( "plain", "zero" );
    badapt_activator_a_discard( o->a_o ); o->a_o = badapt_activator_create_from_names( "plain", "tanh" );
    badapt_activator_a_discard( o->a_q ); o->a_q = badapt_activator_create_from_names( "plain", "zero" );
    badapt_activator_a_discard( o->a_d ); o->a_d = badapt_activator_create_from_names( "plain", "one"  );
    badapt_activator_a_discard( o->a_r ); o->a_r = badapt_activator_create_from_names( "plain", "sigm" );
}

// ---------------------------------------------------------------------------------------------------------------------

badapt_adaptive* badapt_lstm_builder_s_build( const badapt_lstm_builder_s* o )
{
    ASSERT( o->size_input    > 0 );
    ASSERT( o->size_output   > 0 );
    ASSERT( o->size_hidden   > 0 );
    ASSERT( o->size_unfolded > 1 );

    BCORE_LIFE_INIT();

    u2_t random_state = o->random_seed;
    badapt_lstm_s* lstm = badapt_lstm_s_create();

    lstm->size_input    = o->size_input;
    lstm->size_hidden   = o->size_hidden;
    lstm->size_output   = o->size_output;
    lstm->size_unfolded = o->size_unfolded;

    badapt_dynamics_std_s_copy( &lstm->dynamics, &o->dynamics );

    lstm->a_f = badapt_activator_a_clone( o->a_f );
    lstm->a_i = badapt_activator_a_clone( o->a_i );
    lstm->a_o = badapt_activator_a_clone( o->a_o );
    lstm->a_q = badapt_activator_a_clone( o->a_q );
    lstm->a_d = badapt_activator_a_clone( o->a_d );
    lstm->a_r = badapt_activator_a_clone( o->a_r );

    bmath_mf3_s_set_size( &lstm->w_fx, o->size_hidden, o->size_input );
    bmath_mf3_s_set_size( &lstm->w_ix, o->size_hidden, o->size_input );
    bmath_mf3_s_set_size( &lstm->w_ox, o->size_hidden, o->size_input );
    bmath_mf3_s_set_size( &lstm->w_qx, o->size_hidden, o->size_input );
    bmath_mf3_s_set_size( &lstm->w_fh, o->size_hidden, o->size_hidden );
    bmath_mf3_s_set_size( &lstm->w_ih, o->size_hidden, o->size_hidden );
    bmath_mf3_s_set_size( &lstm->w_oh, o->size_hidden, o->size_hidden );
    bmath_mf3_s_set_size( &lstm->w_qh, o->size_hidden, o->size_hidden );
    bmath_mf3_s_set_size( &lstm->w_rh, o->size_output, o->size_hidden );

    bmath_vf3_s_set_size( &lstm->b_f, o->size_hidden );
    bmath_vf3_s_set_size( &lstm->b_i, o->size_hidden );
    bmath_vf3_s_set_size( &lstm->b_o, o->size_hidden );
    bmath_vf3_s_set_size( &lstm->b_q, o->size_hidden );
    bmath_vf3_s_set_size( &lstm->b_r, o->size_output );

    f3_t limit_rec = 1.0 / f3_srt( o->size_hidden );
    f3_t limit_fwd = 0.5;

    bmath_mf3_s_set_random( &lstm->w_ox, false, false, 0, 1.0, -limit_fwd, limit_fwd, &random_state );
    bmath_mf3_s_set_random( &lstm->w_oh, false, false, 0, 1.0, -limit_rec, limit_rec, &random_state );
    bmath_mf3_s_set_random( &lstm->w_rh, false, false, 0, 1.0, -limit_fwd, limit_fwd, &random_state );

    bmath_mf3_s_set_random( &lstm->w_fx, false, false, 0, 1.0, -limit_fwd, limit_fwd, &random_state );
    bmath_mf3_s_set_random( &lstm->w_ix, false, false, 0, 1.0, -limit_fwd, limit_fwd, &random_state );
    bmath_mf3_s_set_random( &lstm->w_qx, false, false, 0, 1.0, -limit_fwd, limit_fwd, &random_state );

    bmath_mf3_s_set_random( &lstm->w_fh, false, false, 0, 1.0, -limit_rec, limit_rec, &random_state );
    bmath_mf3_s_set_random( &lstm->w_ih, false, false, 0, 1.0, -limit_rec, limit_rec, &random_state );
    bmath_mf3_s_set_random( &lstm->w_qh, false, false, 0, 1.0, -limit_rec, limit_rec, &random_state );

    BCORE_LIFE_RETURNV( badapt_adaptive*, ( badapt_adaptive* )lstm );
}

#endif // TYPEOF_badapt_lstm_s

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t badapt_lstm_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "badapt_lstm" ) ) )
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


