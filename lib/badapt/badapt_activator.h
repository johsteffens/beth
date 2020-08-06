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

#ifndef BADAPT_ACTIVATOR_H
#define BADAPT_ACTIVATOR_H

#include "bmath_std.h"
#include "badapt_xoila_out.h"

/**********************************************************************************************************************/

/// activation function
XOILA_DEFINE_GROUP( badapt_activation, bcore_inst )
#ifdef XOILA_SECTION
    feature strict 'pa' f3_t fx( const, f3_t x ); // y  = f( x )
    feature strict 'pa' f3_t dy( const, f3_t y ); // dy = d( y ) (derivative applied on y)

    // ======= (trivial activations) ============
    stamp :zero   = aware : { func : :fx = { return 0.0; }; func : :dy = { return 0.0; }; };
    stamp :one    = aware : { func : :fx = { return 1.0; }; func : :dy = { return 0.0; }; };
    stamp :linear = aware : { func : :fx = { return   x; }; func : :dy = { return 1.0; }; };

    // ======= (logistic function) ============
    stamp :sigm = aware :
    {
        func : :fx = { return ( x > -700 ) ? ( 1.0 / ( 1.0 + exp( -x ) ) ) : 0; };
        func : :dy = { return y * ( 1.0 - y ); };
    };

    stamp :sigm_hard = aware :
    {
        func : :fx = { return ( x < -2.0 ) ? 0.0 : ( x > 2.0 ) ? 1.0 : 0.25 * ( x + 2.0 ); };
        func : :dy = { return ( y <  0.0 ) ? 0.0 : ( y > 1.0 ) ? 0.0 : 0.25; };
    };

    stamp :sigm_leaky = aware :
    {
        func : :fx = { return ( x < -2.0 ) ? 0.01 * ( x + 2.0 ) : ( x > 2.0 ) ? 1.0 + 0.01 * ( x - 2.0 ) : 0.25 * ( x + 2.0 ); };
        func : :dy = { return ( y <  0.0 ) ? 0.01 : ( y > 1.0 ) ? 0.01 : 0.25; };
    };

    // ======= (tanh) =========================
    stamp :tanh = aware :
    {
        func : :fx = { return ( x < 350 ) ? ( 1.0 - ( 2.0 / ( exp( 2.0 * x ) + 1.0 ) ) ) : 1.0; };
        func : :dy = { return 1.0 - f3_sqr( y ); };
    };

    stamp :tanh_hard = aware :
    {
        func : :fx = { return ( x < -1.0 ) ? -1.0 : ( x > 1.0 ) ? 1.0 : x; };
        func : :dy = { return ( y < -1.0 ) ?  0.0 : ( y > 1.0 ) ? 0.0 : 1.0; };
    };

    stamp :tanh_leaky = aware :
    {
        func : :fx = { return ( x < -1.0 ) ? -1.0 + 0.01 * ( x + 1.0 ) : ( x > 1.0 ) ? 1.0 + 0.01 * ( x - 1.0 ) : x; };
        func : :dy = { return ( y < -1.0 ) ?  0.01 : ( y > 1.0 ) ? 0.01 : 1.0; };
    };

    // ======= (softplus function) ============
    stamp :softplus = aware :
    {
        func : :fx = { return ( x < 700 ) ? log( 1.0 + exp( x ) ) : x; };
        func : :dy = { f3_t u = exp( y ); return ( u - 1.0 ) / u; };
    };

    stamp :relu = aware :
    {
        func : :fx = { return x > 0 ? x : 0; };
        func : :dy = { return y > 0 ? 1 : 0; };
    };

    stamp :leaky_relu = aware :
    {
        func : :fx = { return x > 0 ? x : x * 0.01; };
        func : :dy = { return y > 0 ? 1 : 0.01; };
    };

#endif // XOILA_SECTION

/**********************************************************************************************************************/

/// activator: (adaptive) activation applied to a vector
XOILA_DEFINE_GROUP( badapt_activator, bcore_inst )
#ifdef XOILA_SECTION

    /// activation function
    feature 'a' const badapt_activation* get_activation( const )                    = { return NULL; };
    feature 'a' void set_activation( mutable, const badapt_activation* activation ) = {};

    /// fast concurrent inference
    feature strict 'a' void infer( const, const bmath_vf3_s* in, bmath_vf3_s* out );

    /// fast concurrent gradient backpropagation (no changing of state)
    /// grad_in and grad_out may refer to the same object
    feature strict 'a' void bgrad( const, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out, const bmath_vf3_s* out );

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// Activator without bias.
    stamp :plain = aware :
    {
        aware badapt_activation => activation;

        func : :infer =
        {
            assert( in->size == out->size );
            const badapt_activation_s* activation_p = badapt_activation_s_get_aware( o->activation );
            for( sz_t i = 0; i < out->size; i++ )
            {
                out->data[ i ] = badapt_activation_p_fx( activation_p, o->activation, in->data[ i ] );
            }
        };

        func : :bgrad =
        {
            assert( grad_in->size == grad_out->size );
            assert( grad_in->size ==      out->size );
            const badapt_activation_s* activation_p = badapt_activation_s_get_aware( o->activation );
            for( sz_t i = 0; i < out->size; i++ )
            {
                grad_in->data[ i ] = badapt_activation_p_dy( activation_p, o->activation, out->data[ i ] ) * grad_out->data[ i ];
            }
        };

        func : :set_activation = { badapt_activation_a_replicate( &o->activation, activation ); };
        func : :get_activation = { return o->activation; };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// softmax activator.
    stamp :softmax = aware :
    {
        func : :infer =
        {
            f3_t max = bmath_vf3_s_max( in );
            f3_t sum = 0;
            for( sz_t i = 0; i < out->size; i++ )
            {
                f3_t v = exp( in->data[ i ] - max );
                sum += v;
                out->data[ i ] = v;
            }
            bmath_vf3_s_mul_f3( out, 1.0 / sum, out );
        };

        func : :bgrad =
        {
            f3_t dpd = bmath_vf3_s_f3_mul_vec( grad_out, out );
            for( sz_t i = 0; i < grad_in->size; i++ )
            {
                grad_in->data[ i ] = out->data[ i ] * ( grad_out->data[ i ] - dpd );
            }
        };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// specifies which activator is used for which layer; negative layer number means relative to last layer + 1
    stamp badapt_layer_activator     = aware bcore_inst  { sz_t layer; aware badapt_activator => activator; };
    stamp badapt_arr_layer_activator = aware bcore_array { badapt_layer_activator_s    [] arr; };
    stamp badapt_arr_activator       = aware bcore_array { aware badapt_activator   => [] arr; };

#endif // XOILA_SECTION

/**********************************************************************************************************************/

badapt_activator* badapt_activator_create_from_types( tp_t tp_activator, tp_t tp_activation );
badapt_activator* badapt_activator_create_from_names( sc_t sc_activator, sc_t sc_activation );
badapt_layer_activator_s* badapt_layer_activator_s_create_from_types( sz_t layer, tp_t tp_activator, tp_t tp_activation );
void badapt_arr_layer_activator_s_push_from_types( badapt_arr_layer_activator_s* o, sz_t layer, tp_t tp_activator, tp_t tp_activation );
void badapt_arr_layer_activator_s_push_from_names( badapt_arr_layer_activator_s* o, sz_t layer, sc_t activator, sc_t tp_activation ); //sc: only specifying part (e.g. "plain", "tanh")
const badapt_activator* badapt_arr_layer_activator_s_get_activator( const badapt_arr_layer_activator_s* o, sz_t index, sz_t layers ); // returns activator for given layer

/// constructs array from layer-activators
void badapt_arr_activator_s_setup_from_arr_layer_activator( badapt_arr_activator_s* o, const badapt_arr_layer_activator_s* arr, sz_t layers );

badapt_activator_plain_s* badapt_activator_plain_s_create_activation( const badapt_activation* activation );

/**********************************************************************************************************************/

vd_t badapt_activator_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_ACTIVATOR_H

