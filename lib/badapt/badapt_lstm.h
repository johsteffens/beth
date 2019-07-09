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

/** Long Short Term Memory Recurrent Network with forget gate.
 *  Principal idea described 1999 by Gers/Schmidhuber/Cummins in "Learning to Forget: Continual Prediction with LSTM"
 *
 *    Matrices:
 *    w_ij: maps v_j to channel i
 *    transposed: w_ji = w_ij^t
 *
 *    Bias: b_i
 *
 *    Operators:
 *    * - linear multiplication
 *    o - hadamard product
 *    x - outer product of vectors
 *    : - catenation
 *
 *    Activators
 *    a - regular or sigmoid
 *
 *    ------------------------------
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
 *
 *    ------------------------------
 *
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
 *
 */

#ifndef BADAPT_LSTM_H
#define BADAPT_LSTM_H

#include "bcore_std.h"
#include "badapt_activator.h"
#include "badapt_adaptive.h"
#include "badapt_planted.h"

/**********************************************************************************************************************/

BETH_PRECODE( badapt_lstm )
#ifdef BETH_PRECODE_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :layer = bcore_inst
{
    bmath_vf3_s v_x;
    bmath_vf3_s v_f;
    bmath_vf3_s v_i;
    bmath_vf3_s v_o;
    bmath_vf3_s v_q;
    bmath_vf3_s v_c;
    bmath_vf3_s v_d;
    bmath_vf3_s v_h;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :arr_layer = aware bcore_array{ :layer_s => [] arr; };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp : = aware badapt_adaptive
{
    // === architecture parameters ================================
    sz_t size_input;
    sz_t size_hidden;
    sz_t size_output;
    sz_t size_unfolded;

    badapt_dynamics_std_s dynamics;

    // recurrent weights
    bmath_mf3_s w_fx;
    bmath_mf3_s w_fh;
    bmath_mf3_s w_ix;
    bmath_mf3_s w_ih;
    bmath_mf3_s w_ox;
    bmath_mf3_s w_oh;
    bmath_mf3_s w_qx;
    bmath_mf3_s w_qh;

    // output weights
    bmath_mf3_s w_rh;

    // recurrent bias
    bmath_vf3_s b_f;
    bmath_vf3_s b_i;
    bmath_vf3_s b_o;
    bmath_vf3_s b_q;

    // output bias
    bmath_vf3_s b_r;

    aware badapt_activator => a_f;
    aware badapt_activator => a_i;
    aware badapt_activator => a_o;
    aware badapt_activator => a_q;
    aware badapt_activator => a_d;
    aware badapt_activator => a_r;

    // === runtime data =============================================
    hidden bmath_vf3_s v_r; // output vector

    // gradients
    hidden bmath_vf3_s d_v_r;

    hidden bmath_vf3_s d_v_f;
    hidden bmath_vf3_s d_v_i;
    hidden bmath_vf3_s d_v_o;
    hidden bmath_vf3_s d_v_q;
    hidden bmath_vf3_s d_v_c;
    hidden bmath_vf3_s d_v_d;
    hidden bmath_vf3_s d_v_h;

    hidden bmath_mf3_s d_w_fx;
    hidden bmath_mf3_s d_w_fh;
    hidden bmath_mf3_s d_w_ix;
    hidden bmath_mf3_s d_w_ih;
    hidden bmath_mf3_s d_w_ox;
    hidden bmath_mf3_s d_w_oh;
    hidden bmath_mf3_s d_w_qx;
    hidden bmath_mf3_s d_w_qh;

    hidden bmath_vf3_s d_b_f;
    hidden bmath_vf3_s d_b_i;
    hidden bmath_vf3_s d_b_o;
    hidden bmath_vf3_s d_b_q;

    hidden badapt_lstm_arr_layer_s arr_layer;
    // ==============================================================

    // === adaptive functions =======================================
    func : get_in_size      = { return o->size_input; };
    func : get_out_size     = { return o->size_output; };
    func : get_dynamics_std = { badapt_dynamics_std_s_copy( dynamics, &o->dynamics ); };
    func : set_dynamics_std = { badapt_dynamics_std_s_copy( &o->dynamics, dynamics ); };

    func : arc_to_sink;
    func : minfer;
    func : bgrad_adapt;
    func : reset;
    func : get_weights_min_max;
    // ==============================================================
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :builder = aware badapt_builder
{
    sz_t size_input;
    sz_t size_hidden = 8;
    sz_t size_output = 1;
    sz_t size_unfolded = 2;  // number of unfolded time steps during learning

    badapt_dynamics_std_s dynamics;

    u2_t random_seed = 1234; // random seed variable (for random initialization)
    f3_t random_range = 0.5;

    // activators have a default initialization
    aware badapt_activator => a_f; // sigmoid
    aware badapt_activator => a_i; // sigmoid
    aware badapt_activator => a_o; // sigmoid
    aware badapt_activator => a_q; // tanh
    aware badapt_activator => a_d; // tanh (or linear)
    aware badapt_activator => a_r; // sigmoid

    // === builder functions =======================================

    /// constructor
    func bcore_inst_call : init_x;

    /// input vector size
    func : get_in_size = { return o->size_input; };
    func : set_in_size = { o->size_input = size; };

    /// output vector size
    func : get_out_size = { return o->size_output; };
    func : set_out_size = { o->size_output = size; };

    /// builds adaptive ready to be trained; passes ownership
    func : build;

    // ==============================================================
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // BETH_PRECODE_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t badapt_lstm_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BADAPT_LSTM_H
