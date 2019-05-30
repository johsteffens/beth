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

/**********************************************************************************************************************/

/// activation function
BETH_PRECODE( badapt_activation )
#ifdef BETH_PRECODE_SECTION
    feature strict 'pa' f3_t fx( const, f3_t x ); // y  = f( x )
    feature strict 'pa' f3_t dy( const, f3_t y ); // dy = d( y ) (derivative applied on y)

    // (logistic function)
    self :lgst_s       = aware badapt_activation { func :fx; func :dy; }; // f( x ) = 1.0 / ( 1.0 + exp( -x ) )
    self :lgst_hard_s  = aware badapt_activation { func :fx; func :dy; }; // f( x ) = ( x < -2 ) ? 0 : ( x > 2 ) ? 1 : 0.25 * ( x + 2 )
    self :lgst_leaky_s = aware badapt_activation { func :fx; func :dy; }; // f( x ) = ( x < -2 ) ? 0.01 * ( x + 2 ) : ( x > 2 ) ? 1 + 0.01 * ( x - 2 ) : 0.25 * ( x + 2 )

    // tanh
    self :tanh_s       = aware badapt_activation { func :fx; func :dy; }; // f(x) = tanh(x)
    self :tanh_hard_s  = aware badapt_activation { func :fx; func :dy; }; // f(x) = ( x < -1.0 ) ? -1.0 : ( x > 1.0 ) ? 1.0 : x
    self :tanh_leaky_s = aware badapt_activation { func :fx; func :dy; }; // f(x) = ( x < -1.0 ) ? -1.0 + 0.01 * ( x + 1.0 ) : ( x > 1.0 ) ? 1.0 + 0.01 * ( x - 1.0 ) : x

    // softplus function
    self :softplus_s   = aware badapt_activation { func :fx; func :dy; }; // f(x) = log( 1.0 + exp( x ) )
    self :relu_s       = aware badapt_activation { func :fx; func :dy; }; // f(x) = x > 0 ? x : 0
    self :leaky_relu_s = aware badapt_activation { func :fx; func :dy; }; // f(x) = x > 0 ? x : 0.01*x

#endif // BETH_PRECODE_SECTION

/**********************************************************************************************************************/

/// activator: (adaptive) activation applied to a vector
BETH_PRECODE( badapt_activator )
#ifdef BETH_PRECODE_SECTION

    /// resets trainable components with given seed
    feature strict 'a' void reset( mutable );

    /// should be called once before use
    feature strict 'a' void setup( mutable );

    /// vector size
    feature        'a' sz_t get_size( const );
    feature        'a' void set_size( mutable, sz_t size );

    /// activation function
    feature        'a' const badapt_activation* get_activation( const );
    feature        'a' void set_activation( mutable, const badapt_activation* activation );

    /// fast concurrent inference
    feature strict 'a' void infer( const, const bmath_vf3_s* in, bmath_vf3_s* out );

    /// fast concurrent gradient backpropagation (no changing of state)
    /// grad_in and grad_out may refer to the same object
    feature strict 'a' void bgrad( const, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out );

    /// adaptation step after last minfer for given gradient; grad_in can be NULL
    /// grad_in and grad_out may refer to the same object
    feature strict 'a' void adapt( mutable, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out, f3_t step );

#endif // BETH_PRECODE_SECTION

/**********************************************************************************************************************/

/// activation function
BETH_PRECODE( badapt_activation_objects )
#ifdef BETH_PRECODE_SECTION


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
    func : setup;
    func : reset;
    func : infer;
    func : bgrad;
    func : adapt;
    func : set_activation;
    func : get_activation;
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
    func : setup;
    func : reset;
    func : infer;
    func : bgrad;
    func : adapt;
    func : set_activation;
    func : get_activation;
};

#endif // BETH_PRECODE_SECTION

badapt_activator* badapt_activator_create_from_types( tp_t tp_activator, tp_t tp_activation );
badapt_activator* badapt_activator_create_from_names( sc_t sc_activator, sc_t sc_activation );
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

