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

#ifndef BMATH_ADAPTIVE_MLP_H
#define BMATH_ADAPTIVE_MLP_H

#include "bcore_std.h"
#include "bmath_adaptive.h"
#include "bmath_activation.h"
#include "bmath_precoded.h"

/**********************************************************************************************************************/
#ifdef TYPEOF_bmath_adaptive_mlp

/** Simple scalable multi layer perceptron vector -> vector
 *  Set/change architecture parameters. Then run query/learn as needed.
 *  Call reset if parameters need change.
 */
BETH_PRECODE( bmath_adaptive_mlp )
/*
    self bmath_adaptive_mlp_s = bmath_adaptive
    {
        aware_t _;

        // === architecture parameters ================================
        sz_t input_size;             // input vector size
        sz_t input_kernels    = 8;   // (default 8) kernels on input layer
        sz_t output_kernels   = 1;   // (default 1) kernels on output layer
        sz_t layers           = 2;   // (default 2) number of layers
        f3_t kernels_rate     = 0;   // (default 0) rate at which number of kernels increase per layer (negative: decrease); last layer excluded
        f3_t adapt_step       = 0.0001; // learning rate
        f3_t decay_step       = 0;   // weight decay rate
        sr_s act_mid;                // (default: softplus) middle activation function
        sr_s act_out;                // (default: tanh) output activation function
        u2_t random_state     =1234; // (default: 1234) random state variable (for random initialization)
        // ==============================================================

        // === runtime data =============================================
               bmath_arr_mf3_s   arr_w;  // weight matrix
        hidden bmath_arr_vf3_s   arr_a;  // input  vector (weak map to buf)
        hidden bmath_arr_vf3_s   arr_b;  // output vector (weak map to buf)
        hidden bmath_vf3_s       buf_ab; // data buffer for a and b vector
        hidden bmath_arr_vf3_s   arr_ga;  // gradient input  vector (weak map to gbuf)
        hidden bmath_arr_vf3_s   arr_gb;  // gradient output vector (weak map to gbuf)
        hidden bmath_vf3_s       buf_gab; // data buffer for ga and gb matrix
        hidden bcore_arr_sr_s    arr_activation; // activation functions
        hidden bmath_vf3_s       in;   // input vector weak map to arr_a[ 0 ]
        hidden bmath_vf3_s       out;  // output vector weak map to arr_b[ layers-1 ]
        hidden bmath_vf3_s       gout; // output vector weak map to arr_gb[ layers-1 ]
        /// ==============================================================

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

#endif // TYPEOF_bmath_adaptive_mlp

/**********************************************************************************************************************/

vd_t bmath_adaptive_mlp_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_ADAPTIVE_MLP_H
