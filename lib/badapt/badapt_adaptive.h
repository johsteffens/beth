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
#include "badapt.xo.h"
#include "badapt_loss.h"

/**********************************************************************************************************************/

/// training dynamics
XOILA_DEFINE_GROUP( badapt_dynamics, bcore_inst )
#ifdef XOILA_SECTION

/// updates weights if forward map is: in * w = out;
feature strict void weights_adapt( c @* o, c bmath_vf3_s* in, m bmath_mf3_s* w, c bmath_vf3_s* grad_out, f3_t epsilon_factor );

stamp :std_s = aware :
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
    feature strict sz_t get_in_size( c @* o );

    /// output vector size
    feature strict sz_t get_out_size( c @* o );

    /// fast concurrent inference (can be omitted e.g. for recurrent networks)
    feature        void infer( c @* o, c bmath_vf3_s* in, m bmath_vf3_s* out );

    /// mutable inference (used for training and recurrent nets)
    feature strict void minfer( m @* o, c bmath_vf3_s* in, m bmath_vf3_s* out );

    /// fast concurrent gradient backpropagation (no changing of state)
    feature        void bgrad( c @* o, m bmath_vf3_s* grad_in, c bmath_vf3_s* grad_out );

    /// gradient backpropagation with adaptation; relates to after last call to minfer for given gradient; grad_in can be NULL
    feature strict void bgrad_adapt( m @* o, m bmath_vf3_s* grad_in, c bmath_vf3_s* grad_out );

    // ===== optional features =====

    /// resets network to a defined internal state; (e.g. for recurrent networks )
    feature        void reset( m @* o );

    /// retrieves min and max of weights
    feature        void get_weights_min_max( c @* o, m f3_t* min, m f3_t* max );

    feature void get_dynamics_std( c @* o, m badapt_dynamics_std_s* dynamics );
    feature void set_dynamics_std( m @* o, c badapt_dynamics_std_s* dynamics );

    // ===== optional features with default implementation =====

    /// outputs architecture to text sink (for easy inspection)
    feature void arc_to_sink( c @* o, m bcore_sink* sink ) = { bcore_txt_ml_a_to_sink( o, sink ); };

    /// inference for scalar output
    feature f3_t infer_f3( c @* o, c bmath_vf3_s* in ) =
    {
        bmath_vf3_s v_out;
        f3_t out = 0;
        bmath_vf3_s_init_weak( v_out.1, out.1, 1 );
        badapt_adaptive_a_infer( o, in, v_out.1 );
        return out;
    };

    /// full adaption cycle based on loss function; adapt_loss_f3 returns estimates result
    feature void adapt_loss( m @* o, c badapt_loss* loss, c bmath_vf3_s* in, c bmath_vf3_s* target, m bmath_vf3_s* out ) =
    {
        ASSERT( out != NULL );
        d bmath_vf3_s* grad = bmath_vf3_s_create();
        bmath_vf3_s_set_size( grad, out->size );
        badapt_adaptive_a_minfer( o, in, out );
        badapt_loss_a_bgrad( loss, out, target, grad );
        badapt_adaptive_a_bgrad_adapt( o, NULL, grad );
        bmath_vf3_s_discard( grad );
    };

    feature f3_t adapt_loss_f3( m @* o, c badapt_loss* loss, c bmath_vf3_s* in, f3_t target ) =
    {
        bmath_vf3_s v_target;
        bmath_vf3_s v_out;
        f3_t out = 0;
        bmath_vf3_s_init_weak( v_target.1, target.1, 1 );
        bmath_vf3_s_init_weak( v_out.1, out.1, 1 );
        badapt_adaptive_a_adapt_loss( o, loss, in, v_target.1, v_out.1 );
        return out;
    };

#endif // XOILA_SECTION

/**********************************************************************************************************************/

/// builder: constructs the adaptive architecture
XOILA_DEFINE_GROUP( badapt_builder, bcore_inst )
#ifdef XOILA_SECTION

    /// input vector size
    feature strict sz_t get_in_size( c @* o );
    feature strict void set_in_size( m @* o, sz_t size );

    /// output vector size
    feature strict sz_t get_out_size( c @* o );
    feature strict void set_out_size( m @* o, sz_t size );

    /// builds adaptive ready to be trained; passes ownership
    feature strict m badapt_adaptive* build( c @* o );

#endif // XOILA_SECTION

/**********************************************************************************************************************/

vd_t badapt_adaptive_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_adaptive_H

