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

#ifndef BADAPT_ACTIVATOR_H
#define BADAPT_ACTIVATOR_H

#include "bmath_std.h"
#include "badapt_precoded.h"
#include "badapt_features.h"

/**********************************************************************************************************************/

/// activation function
BETH_PRECODE( badapt_activation_objects )
#ifdef BETH_PRECODE_SECTION

self badapt_activation_tanh_s = badapt_activation
{
    aware_t _;
    func badapt_activation : fx;
    func badapt_activation : dy;
};

self badapt_activation_relu_s = badapt_activation
{
    aware_t _;
    func badapt_activation : fx;
    func badapt_activation : dy;
};

self badapt_activation_leaky_relu_s = badapt_activation
{
    aware_t _;
    func badapt_activation : fx;
    func badapt_activation : dy;
};

self badapt_activation_softplus_s = badapt_activation
{
    aware_t _;
    func badapt_activation : fx;
    func badapt_activation : dy;
};

#endif // BETH_PRECODE_SECTION

/**********************************************************************************************************************/

/// activation function
BETH_PRECODE( badapt_activator_objects )
#ifdef BETH_PRECODE_SECTION

/// specifies which activator is used for which layer; negative layer number means relative to last layer + 1
self badapt_layer_activator_s = bcore_inst
{
    aware_t _;
    sz_t layer;
    aware badapt_activator => activator;
};

self badapt_arr_activator_s       = bcore_array { aware_t _; aware badapt_activator   => [] arr; };
self badapt_arr_layer_activator_s = bcore_array { aware_t _; badapt_layer_activator_s    [] arr; };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Activator without bias.
 */
self badapt_activator_plain_s = badapt_activator
{
    aware_t _;
    aware badapt_activation => activation;
    func badapt_activator : setup;
    func badapt_activator : reset;
    func badapt_activator : infer;
    func badapt_activator : bgrad;
    func badapt_activator : adapt;
    func badapt_activator : set_activation;
    func badapt_activator : get_activation;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Activator with bias.
 *  Bias is not randomized but initialized zero (common practice).
 */
self badapt_activator_bias_s = badapt_activator
{
    aware_t _;
    aware badapt_activation => activation;
    f3_t [] arr_bias;
    func badapt_activator : setup;
    func badapt_activator : reset;
    func badapt_activator : infer;
    func badapt_activator : bgrad;
    func badapt_activator : adapt;
    func badapt_activator : set_activation;
    func badapt_activator : get_activation;
};

#endif // BETH_PRECODE_SECTION

badapt_activator* badapt_activator_create_from_types( tp_t tp_activator, tp_t tp_activation );
badapt_layer_activator_s* badapt_layer_activator_s_create_from_types( sz_t layer, tp_t tp_activator, tp_t tp_activation );
void badapt_arr_layer_activator_s_push_from_types( badapt_arr_layer_activator_s* o, sz_t layer, tp_t tp_activator, tp_t tp_activation );
void badapt_arr_layer_activator_s_push_from_names( badapt_arr_layer_activator_s* o, sz_t layer, sc_t activator, sc_t tp_activation ); //sc: only specifying part (e.g. "plain", "tanh")
const badapt_activator* badapt_arr_layer_activator_s_get_activator( const badapt_arr_layer_activator_s* o, sz_t index, sz_t layers ); // returns activator for given layer

/// constructs array from layer-activators
void badapt_arr_activator_s_setup_from_arr_layer_activator( badapt_arr_activator_s* o, const badapt_arr_layer_activator_s* arr, sz_t layers );

badapt_activator_plain_s* badapt_activator_plain_s_create_activation( const badapt_activation* activation );
badapt_activator_bias_s*  badapt_activator_bias_s_create_activation(  const badapt_activation* activation );

/**********************************************************************************************************************/

vd_t badapt_activator_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_ACTIVATOR_H

