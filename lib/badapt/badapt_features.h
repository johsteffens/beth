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

#ifndef BADAPT_FEATURES_H
#define BADAPT_FEATURES_H

#include "bmath_std.h"
#include "badapt_precoded.h"

/**********************************************************************************************************************/

/// loss function
BETH_PRECODE( badapt_loss )
#ifdef BETH_PRECODE_SECTION
    feature strict 'pa' f3_t loss(    const, const bmath_vf3_s* out, const bmath_vf3_s* target );                    // loss function
    feature strict 'pa' f3_t loss_f3( const, f3_t               out, f3_t               target );                    // loss function on scalars
    feature strict 'pa' void bgrad(   const, const bmath_vf3_s* out, const bmath_vf3_s* target, bmath_vf3_s* grad ); // computes loss minimizing backward gradient of x
#endif // BETH_PRECODE_SECTION

/**********************************************************************************************************************/

/// activation function
BETH_PRECODE( badapt_activation )
#ifdef BETH_PRECODE_SECTION
    feature strict 'pa' f3_t fx( const, f3_t x ); // y  = f( x )
    feature strict 'pa' f3_t dy( const, f3_t y ); // dy = d( y ) (derivative applied on y)
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

vd_t badapt_features_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_FEATURES_H

