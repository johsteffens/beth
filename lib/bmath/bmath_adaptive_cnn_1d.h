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

#ifndef BMATH_ADAPTIVE_CNN_1D_H
#define BMATH_ADAPTIVE_CNN_1D_H

#include "bcore_std.h"
#include "bmath_adaptive.h"
#include "bmath_activation.h"
#include "bmath_precoded.h"

/**********************************************************************************************************************/
#ifdef TYPEOF_bmath_adaptive_cnn_1d

/** Simple scalable 'All-Convolutional' CNN: vector -> vector
 *  (Inspired by Paper "Striving for Simplicity: The All Convolutional Net" (https://arxiv.org/abs/1412.6806)
 *
 *  Uses fast matrix-matrix-multiplication for query and learning.
 *
 *  Set/change architecture parameters. Then run query/learn as needed.
 *  Call reset if parameters need change.
 */
BETH_PRECODE( bmath_adaptive_cnn_1d )
/*
    self bmath_adaptive_cnn_1d_s = bmath_adaptive
    {
        aware_t _;

        // === architecture parameters ================================
        sz_t input_size                ; // input vector size
        sz_t input_step             = 1; // input vector stepping
        sz_t input_convolution_size = 2; // first layer convolution
        sz_t input_kernels          = 8; // kernels on input layer
        sz_t output_kernels         = 1; // kernels on output layer
        f3_t kernels_rate           = 0; // rate at which number kernels increase per layer (negative: decrease)
        sz_t reduction_step         = 2; // dimensionality reduction stepping
        sz_t convolution_size       = 2; // dimensionality convolution size
        f3_t adapt_step             = 0.0001; // learning rate
        f3_t decay_step             = 0; // weight decay rate
        sr_s act_mid                   ; // middle activation function
        sr_s act_out                   ; // output activation function
        u2_t random_state = 1234       ; // random state variable (for random initialization)
        // ==============================================================

        // === runtime data ============================================
        bmath_arr_mf3_s arr_w;         // weight matrix
        hidden bmath_arr_mf3_s arr_a;  // input  matrix  (weak map to buf)
        hidden bmath_arr_mf3_s arr_b;  // output matrix  (weak map to buf)
        hidden bmath_vf3_s     buf_ab; // data buffer for input and output
        hidden bmath_arr_mf3_s arr_ga; // grad input  matrix  (weak map to gbuf)
        hidden bmath_arr_mf3_s arr_gb; // grad output matrix  (weak map to gbuf)
        hidden bmath_vf3_s     buf_gab;
        hidden bcore_arr_sr_s  arr_activation; // activation functions
        hidden bmath_vf3_s     in;
        hidden bmath_vf3_s     out;
        hidden bmath_vf3_s    gout;
        // ==============================================================

        // === functions ================================================
        func bmath_adaptive: get_in_size;
        func bmath_adaptive: set_in_size;
        func bmath_adaptive: get_out_size;
        func bmath_adaptive: set_out_size;
        func bmath_adaptive: get_step;
        func bmath_adaptive: set_step;
        func bmath_adaptive: get_decay;
        func bmath_adaptive: set_decay;
        func bmath_adaptive: setup;
        func bmath_adaptive: set_untrained;
        func bmath_adaptive: arc_to_sink;
        func bmath_adaptive: query;
        func bmath_adaptive: adapt;
        // ==============================================================
    }
*/

#endif // TYPEOF_bmath_adaptive_cnn

/**********************************************************************************************************************/

vd_t bmath_adaptive_cnn_1d_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_ADAPTIVE_CNN_1D_H
