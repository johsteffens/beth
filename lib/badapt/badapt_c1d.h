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

/** 1D Convolutional Neural Network
 */

#ifndef BADAPT_C1D_H
#define BADAPT_C1D_H

#include "bcore_std.h"
#include "badapt_activator.h"
#include "badapt_adaptive.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( badapt_c1d, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :layer_s = aware bcore_inst
{
    sz_t input_size;  // input vector size
    sz_t stride;      // convolution step size (stride of spliced matrix)
    sz_t steps;       // number of convolution steps
    sz_t kernel_size; // size of single kernel (rows of kernel matrix)
    sz_t kernels;     // number of kernels     (cols of kernel matrix)

    bmath_mf3_s                w; // weights
    bmath_vf3_s                b; // bias
    aware  badapt_activator => a; // activator
    hidden bmath_vf3_s         o; // output
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :arr_layer_s = aware x_array { :layer_s [] arr; };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp badapt_c1d_s = aware badapt_adaptive
{
    // === architecture parameters ================================

    badapt_dynamics_std_s  dynamics;
    :arr_layer_s           arr_layer;
    sz_t                   max_buffer_size;

    // === runtime data =============================================

    hidden bmath_vf3_s in;

    // ==============================================================

    // === adaptive functions =======================================
    func badapt_adaptive . get_in_size      { return o->arr_layer.arr_size > 0 ? o->arr_layer.arr_data[ 0 ].input_size : 0; };
    func badapt_adaptive . get_out_size     { return o->arr_layer.arr_size > 0 ? o->arr_layer.arr_data[ o->arr_layer.arr_size - 1 ].kernels : 0; };
    func badapt_adaptive . get_dynamics_std { dynamics.copy( o.dynamics ); };
    func badapt_adaptive . set_dynamics_std { o.dynamics.copy( dynamics ); };

    func badapt_adaptive . arc_to_sink;
    func badapt_adaptive . infer;
    func badapt_adaptive . minfer;

    func badapt_adaptive . bgrad;
    func badapt_adaptive . bgrad_adapt;
    // ==============================================================
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// Builder creating a funnel structure of kernels
stamp badapt_builder_c1d_funnel_s = aware badapt_builder
{
    sz_t input_size                ; // input vector size
    sz_t input_step             = 1; // input vector stepping
    sz_t input_convolution_size = 2; // first layer convolution
    sz_t input_kernels          = 8; // kernels on input layer
    sz_t output_kernels         = 1; // kernels on output layer
    f3_t kernels_rate           = 0; // rate at which number kernels increase per layer (negative: decrease)
    sz_t reduction_step         = 2; // dimensionality reduction stepping
    sz_t convolution_size       = 2; // dimensionality convolution size
    u2_t random_seed         = 1234; // random seed variable (for random initialization)

    badapt_dynamics_std_s dynamics;

    badapt_arr_layer_activator_s arr_layer_activator;

    // === builder functions =======================================

    /// input vector size
    func ^ . get_in_size { return o->input_size; };
    func ^ . set_in_size { o->input_size = size; };

    /// output vector size
    func ^ . get_out_size { return o->output_kernels; };
    func ^ . set_out_size { o->output_kernels = size; };

    /// builds adaptive ready to be trained; passes ownership
    func ^ . build;

    // ==============================================================
};

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void badapt_c1d_s_test_sine_random();
void badapt_c1d_s_test_binary_add();
void badapt_c1d_s_test_binary_mul();
void badapt_c1d_s_test_binary_xsg3();
void badapt_c1d_s_test_binary_hash();
void badapt_c1d_s_test_polynom();

/**********************************************************************************************************************/

vd_t badapt_c1d_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BADAPT_C1D_H
