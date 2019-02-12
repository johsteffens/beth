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

#ifndef BMATH_CNN_H
#define BMATH_CNN_H

#include "bcore_std.h"

/**********************************************************************************************************************/

/// activation function
BCORE_DECLARE_OBJECT( bmath_cnn_act_s )
{
    st_s st_activation;
    st_s st_derivative;
    bmath_fp_f3_unary fp_activation;
    bmath_fp_f3_unary fp_derivative;
};

/// sets function pointers
void bmath_cnn_act_s_setup( bmath_cnn_act_s* o );

/// frequently used activation functions
bmath_cnn_act_s bmath_cnn_act_tanh();
bmath_cnn_act_s bmath_cnn_act_relu();
bmath_cnn_act_s bmath_cnn_act_softplus();
bmath_cnn_act_s bmath_cnn_act_leaky_relu();

/**********************************************************************************************************************/

/**********************************************************************************************************************/

/** Simple scalable 'All-Convolutional' CNN: vector -> vector
 *  (Inspired by Paper "Striving for Simplicity: The All Convolutional Net" (https://arxiv.org/abs/1412.6806)
 *
 *  Uses fast fast matrix-matrix-multiplication for query and learning.
 *
 *  Set/change architecture parameters. Then run query/learn as needed.
 *  Call reset if parameters need change.
 */
BCORE_DECLARE_OBJECT( bmath_cnn_s )
{
    aware_t _;

    /// === architecture parameters ================================
    sz_t input_size;             // input vector size
    sz_t input_step;             // (default 1) input vector stepping
    sz_t input_convolution_size; // (default 2) first layer convolution
    sz_t input_kernels;          // (default 8) kernels on input layer
    sz_t output_kernels;         // (default 1) kernels on output layer
    f3_t kernels_rate;           // (default 0) rate at which number of kernels increase per layer (negative: decrease); last layer excluded
    sz_t reduction_step;         // (default 2) desired dimensionality reduction stepping when possible for full coverage
    sz_t convolution_size;       // (default 2) dimensionality convolution size
    bmath_cnn_act_s act_mid;     // (default: softplus) middle activation function
    bmath_cnn_act_s act_out;     // (default: tanh) output activation function
    u2_t random_state;           // (default: 1234) random state variable (for random initialization)
    /// ==============================================================

    /// === internal data ============================================
    bmath_arr_mf3_s   arr_w;  // weight matrix

    bmath_arr_mf3_s   arr_a;  // input  matrix  (weak map to buf)
    bmath_arr_mf3_s   arr_b;  // output matrix  (weak map to buf)
    bmath_vf3_s       buf_ab; // data buffer for a and b matrix

    bmath_arr_mf3_s   arr_ga;  // gradient input  matrix  (weak map to gbuf)
    bmath_arr_mf3_s   arr_gb;  // gradient output matrix  (weak map to gbuf)
    bmath_vf3_s       buf_gab; // data buffer for ga and gb matrix

    bcore_arr_fp_s    arr_fp_activation; // activation functions
    bcore_arr_fp_s    arr_fp_derivative; // derivative functions

    bmath_mf3_s       learn_gw; // grad w used in a lean step

    bmath_vf3_s       in;   // input vector weak map to arr_a[ 0 ]
    bmath_vf3_s       out;  // output vector weak map to arr_b[ layers-1 ]
    bmath_vf3_s       gout; // output vector weak map to arr_gb[ layers-1 ]

    /// ==============================================================
};

/** Sets up network from architecture parameters.
 *  If network is untrained, weights are randomly initialized.
 *  This function is called by bmath_cnn_s_query if necessary.
 *  After calling setup architecture parameters should not be changed.
 *  Call bmath_cnn_s_reset if architecture parameters are to be changed.
 */
void bmath_cnn_s_setup( bmath_cnn_s* o );

/// Resets network to the untrained state
void bmath_cnn_s_reset( bmath_cnn_s* o );

/// Outputs architecture to text-sink.
void bmath_cnn_s_arc_to_sink( const bmath_cnn_s* o, bcore_sink* sink );

/// Output kernels > 1: Query (inference); returns output activation. (out can be NULL)
void bmath_cnn_s_query( bmath_cnn_s* o, const bmath_vf3_s* in, bmath_vf3_s* out );

/// Output kernels > 1: Learn step; if out is != NULL it is filled with the result of bmath_cnn_s_query
void bmath_cnn_s_learn( bmath_cnn_s* o, const bmath_vf3_s* in, const bmath_vf3_s* target, f3_t step, bmath_vf3_s* out );

/// weight decay step
void bmath_cnn_s_decay( bmath_cnn_s* o, f3_t step );

/// Output kernels == 1: Query (inference); returns output activation.
f3_t bmath_cnn_s_query_1( bmath_cnn_s* o, const bmath_vf3_s* in );

/// Output kernels == 1: Learn step; returns output activation of query.
f3_t bmath_cnn_s_learn_1( bmath_cnn_s* o, const bmath_vf3_s* in, f3_t target, f3_t step );

/**********************************************************************************************************************/

vd_t bmath_cnn_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_CNN_H
