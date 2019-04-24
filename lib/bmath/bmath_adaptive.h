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

#ifndef BMATH_ADAPTIVE_H
#define BMATH_ADAPTIVE_H

/// An adaptive object is trainable (e.g. neural network)

#include "bcore_spect_sink.h"
#include "bmath_mf3.h"
#include "bmath_vector.h"
#include "bmath_precoded.h"

/**********************************************************************************************************************/
/// features

BETH_PRECODE( bmath_adaptive )
/*
    // input vector size
    feature sz_t get_in_size( const );
    feature void set_in_size( mutable, sz_t size );

    // output vector size
    feature sz_t get_out_size( const );
    feature void set_out_size( mutable, sz_t size );

    // adaption step
    feature f3_t get_step( const );
    feature void set_step( mutable, f3_t val );

    // adaption decay (L2 - regularization)
    feature f3_t get_decay( const );
    feature void set_decay( mutable, f3_t val );

    // setup of the internal architecture (usually, this function needs not be explicitly called)
    feature void setup( mutable, bl_t training );

    // resets adaptive to the untrained state
    feature void set_untrained( mutable );

    // outputs architecture to text sink (for easy inspection)
    feature void arc_to_sink( const, bcore_sink* sink );

    // query == inference
    // Note that querying can change the internal state of the adaptive.
    // It might not be reentrant (e.g. recurrent network) and should not be assumed concurrent unless explicitly stated.
    feature void query( mutable, const bmath_vf3_s* in, bmath_vf3_s* out );

    // adaptation step (out can be NULL)
    feature void adapt( mutable, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out );
*/

/// simplified query/adapt for output size == 1; returns output activation.
f3_t bmath_adaptive_a_query_1( bmath_adaptive* o, const bmath_vf3_s* in );
f3_t bmath_adaptive_a_adapt_1( bmath_adaptive* o, const bmath_vf3_s* in, f3_t target );

/// generic test for an adaptive
void bmath_adaptive_a_test( const bmath_adaptive* o );

/**********************************************************************************************************************/

vd_t bmath_adaptive_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_ADAPTIVE_H

