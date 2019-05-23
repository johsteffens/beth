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

#ifndef BADAPT_adaptive_H
#define BADAPT_adaptive_H

#include "bmath_std.h"
#include "badapt_precoded.h"
#include "badapt_features.h"

/**********************************************************************************************************************/

/// training dynamics
BETH_PRECODE( badapt_adaptive_objects )
#ifdef BETH_PRECODE_SECTION

self badapt_dynamics_s = bcore_inst
{
    aware_t _;
    f3_t epsilon;   // learning rate
    f3_t lambda_l1; // l1-regularization
    f3_t lambda_l2; // l2-regularization
};

#endif // BETH_PRECODE_SECTION

/**********************************************************************************************************************/

/// adaptive unit
BETH_PRECODE( badapt_adaptive )
#ifdef BETH_PRECODE_SECTION

    // ===== required features =====

    /// input vector size
    feature strict 'a' sz_t get_in_size( const );

    /// output vector size
    feature strict 'a' sz_t get_out_size( const );

    /// fast concurrent inference
    feature strict 'a' void infer( const, const bmath_vf3_s* in, bmath_vf3_s* out );

    /// mutable inference (used for training)
    feature strict 'a' void minfer( mutable, const bmath_vf3_s* in, bmath_vf3_s* out );

    /// fast concurrent gradient backpropagation (no changing of state)
    feature strict 'a' void bgrad( const, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out );

    /// gradient backpropagation with adaptation; relates to after last call to minfer for given gradient; grad_in can be NULL
    feature strict 'a' void bgrad_adapt( mutable, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out );

    // ===== optional features =====

    feature 'a' void get_dynamics( const,         badapt_dynamics_s* dynamics );
    feature 'a' void set_dynamics( mutable, const badapt_dynamics_s* dynamics );

    // ===== optional features with default implementation =====

    /// outputs architecture to text sink (for easy inspection)
    feature 'a' void arc_to_sink( const, bcore_sink* sink ) = arc_to_sink_fallback;

    /// inference for scalar output
    feature 'a' f3_t infer_f3( const, const bmath_vf3_s* in ) = infer_f3_fallback;

    /// full adaption cycle based on loss function; adapt_loss_f3 returns estimates result
    feature 'a' void adapt_loss(    mutable, const badapt_loss* loss, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out ) = adapt_loss_fallback;
    feature 'a' f3_t adapt_loss_f3( mutable, const badapt_loss* loss, const bmath_vf3_s* in, f3_t target )                                 = adapt_loss_f3_fallback;

#endif // BETH_PRECODE_SECTION

/**********************************************************************************************************************/

/// builder: constructs the adaptive architecture
BETH_PRECODE( badapt_builder )
#ifdef BETH_PRECODE_SECTION

    /// input vector size
    feature strict 'a' sz_t get_in_size( const );
    feature strict 'a' void set_in_size( mutable, sz_t size );

    /// output vector size
    feature strict 'a' sz_t get_out_size( const );
    feature strict 'a' void set_out_size( mutable, sz_t size );

    /// builds adaptive ready to be trained; passes ownership
    feature strict 'a' badapt_adaptive* build( const );

#endif // BETH_PRECODE_SECTION

/**********************************************************************************************************************/

vd_t badapt_adaptive_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_adaptive_H

