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

#ifndef BADAPT_adaptive_H
#define BADAPT_adaptive_H

#include "bmath_std.h"
#include "badapt_xoila_out.h"
#include "badapt_loss.h"

/**********************************************************************************************************************/

/// training dynamics
XOILA_DEFINE_GROUP( badapt_dynamics, bcore_inst )
#ifdef XOILA_SECTION

/// updates weights if forward map is: in * w = out;
feature strict void weights_adapt( const, const bmath_vf3_s* in, bmath_mf3_s* w, const bmath_vf3_s* grad_out, f3_t epsilon_factor );

stamp :std = aware :
{
    f3_t epsilon;   // learning rate
    f3_t lambda_l1; // l1-regularization
    f3_t lambda_l2; // l2-regularization
    func badapt_dynamics . weights_adapt;
};

#endif // XOILA_SECTION

/**********************************************************************************************************************/

/// adaptive unit
XOILA_DEFINE_GROUP( badapt_adaptive, bcore_inst )
#ifdef XOILA_SECTION

    // ===== required features =====

    /// input vector size
    feature strict sz_t get_in_size( const );

    /// output vector size
    feature strict sz_t get_out_size( const );

    /// fast concurrent inference (can be omitted e.g. for recurrent networks)
    feature        void infer( const, const bmath_vf3_s* in, bmath_vf3_s* out );

    /// mutable inference (used for training and recurrent nets)
    feature strict void minfer( mutable, const bmath_vf3_s* in, bmath_vf3_s* out );

    /// fast concurrent gradient backpropagation (no changing of state)
    feature        void bgrad( const, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out );

    /// gradient backpropagation with adaptation; relates to after last call to minfer for given gradient; grad_in can be NULL
    feature strict void bgrad_adapt( mutable, bmath_vf3_s* grad_in, const bmath_vf3_s* grad_out );

    // ===== optional features =====

    /// resets network to a defined internal state; (e.g. for recurrent networks )
    feature        void reset( mutable );

    /// retrieves min and max of weights
    feature        void get_weights_min_max( const, f3_t* min, f3_t* max );

    feature void get_dynamics_std( const,         badapt_dynamics_std_s* dynamics );
    feature void set_dynamics_std( mutable, const badapt_dynamics_std_s* dynamics );

    // ===== optional features with default implementation =====

    /// outputs architecture to text sink (for easy inspection)
    feature void arc_to_sink( const, bcore_sink* sink ) = { bcore_txt_ml_a_to_sink( o, sink ); };

    /// inference for scalar output
    feature f3_t infer_f3( const, const bmath_vf3_s* in ) =
    {
        bmath_vf3_s v_out;
        f3_t out = 0;
        bmath_vf3_s_init_weak( &v_out, &out, 1 );
        badapt_adaptive_a_infer( o, in, &v_out );
        return out;
    };

    /// full adaption cycle based on loss function; adapt_loss_f3 returns estimates result
    feature void adapt_loss( mutable, const badapt_loss* loss, const bmath_vf3_s* in, const bmath_vf3_s* target, bmath_vf3_s* out ) =
    {
        ASSERT( out != NULL );
        bmath_vf3_s* grad = bmath_vf3_s_create();
        bmath_vf3_s_set_size( grad, out->size );
        badapt_adaptive_a_minfer( o, in, out );
        badapt_loss_a_bgrad( loss, out, target, grad );
        badapt_adaptive_a_bgrad_adapt( o, NULL, grad );
        bmath_vf3_s_discard( grad );
    };

    feature f3_t adapt_loss_f3( mutable, const badapt_loss* loss, const bmath_vf3_s* in, f3_t target ) =
    {
        bmath_vf3_s v_target, v_out;
        f3_t out = 0;
        bmath_vf3_s_init_weak( &v_target, &target, 1 );
        bmath_vf3_s_init_weak( &v_out, &out, 1 );
        badapt_adaptive_a_adapt_loss( o, loss, in, &v_target, &v_out );
        return out;
    };

#endif // XOILA_SECTION

/**********************************************************************************************************************/

/// builder: constructs the adaptive architecture
XOILA_DEFINE_GROUP( badapt_builder, bcore_inst )
#ifdef XOILA_SECTION

    /// input vector size
    feature strict sz_t get_in_size( const );
    feature strict void set_in_size( mutable, sz_t size );

    /// output vector size
    feature strict sz_t get_out_size( const );
    feature strict void set_out_size( mutable, sz_t size );

    /// builds adaptive ready to be trained; passes ownership
    feature strict badapt_adaptive* build( const );

#endif // XOILA_SECTION

/**********************************************************************************************************************/

vd_t badapt_adaptive_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_adaptive_H

