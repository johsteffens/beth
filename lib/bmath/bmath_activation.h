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

#ifndef BMATH_ACTIVATION_H
#define BMATH_ACTIVATION_H

/// Activation interface and functions

#include "bcore_spect_sink.h"
#include "bmath_mf3.h"
#include "bmath_vector.h"
#include "bmath_precoded.h"

/**********************************************************************************************************************/

BETH_PRECODE( bmath_activation )
/*
    // forward activation function y = f(x)
    feature 'par' f3_t fx( const, f3_t x );

    // derivative on y
    feature 'par' f3_t dy( const, f3_t y );
*/

/**********************************************************************************************************************/

BETH_PRECODE( bmath_activation_objects )
/*
    self bmath_activation_tanh_s       = bmath_activation { aware_t _; func bmath_activation : fx; func bmath_activation : dy; };
    self bmath_activation_relu_s       = bmath_activation { aware_t _; func bmath_activation : fx; func bmath_activation : dy; };
    self bmath_activation_leaky_relu_s = bmath_activation { aware_t _; func bmath_activation : fx; func bmath_activation : dy; };
    self bmath_activation_softplus_s   = bmath_activation { aware_t _; func bmath_activation : fx; func bmath_activation : dy; }; // smooth approximation of relu
*/

/**********************************************************************************************************************/

vd_t bmath_activation_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_ACTIVATION_H

