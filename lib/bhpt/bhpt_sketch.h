/** Author and Copyright 2020 Johannes Bernhard Steffens
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

#ifndef BHPT_SKETCH_H
#define BHPT_SKETCH_H

/**********************************************************************************************************************/

/** Global Interface Sketch of this Library*/

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_std.h"
#include "bhpt.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bhpt, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** The adaptor defines a method for updating adaptive holors from accumulated gradients
 *  It is assigned to an adaptive, which prepares (resets) its adapt function.
 */
group :adaptor =
{
    signature void get_min_max( const, f3_t* min, f3_t* max );
    signature void zro_grad( mutable );
    signature void acc_grad( mutable, const @* src );

    signature void acc_stats( const, bhvm_stats_s* axon, bhvm_stats_s* grad );

    /// Makes axon values of o reference axon values of src; run adaptive_a_rebind_holors afterwards
    signature void rebind_axon( mutable, @* src );

    /// The node weakly references adaptive axon and gradient holors.
    stamp :node  = aware :
    {
        private bhvm_holor_s* axon;
        private bhvm_holor_s* grad;
        func : .get_min_max;
        func : .acc_stats;
        func : .zro_grad  = { o.grad.v.zro(); };
        func : .acc_grad  =   { assert( o.grad.s.is_equal( src.grad.s.1 ) ); o.grad.v.acc( src.grad.v.1 ); };
        func : .rebind_axon = { assert( o.axon.s.is_equal( src.axon.s.1 ) ); o.axon.v.weak( src.axon.v.1 ); };
    };

    /** The probe is obtained via function get_adaptor_probe.
     *  It is valid after obtaining until a modification
     *  of the underlying adaptive that can change holor bindings.
     *  If the probe is used to change holor bindings,
     *  adaptive_a_rebind_holors must be called.
     */
    stamp :probe = aware bcore_array
    {
        :node_s [];
        func : .get_min_max;
        func : .acc_stats = { BFOR_EACH( i, o ) o.[ i ].acc_stats( axon, grad ); };
        func : .zro_grad  = { BFOR_EACH( i, o ) o.[ i ].zro_grad(); };
        func : .acc_grad     = { assert( o.size == src.size ); BFOR_EACH( i, o ) o.[ i ].acc_grad( src.[ i ].1 ); };
        func : .rebind_axon  = { assert( o.size == src.size ); BFOR_EACH( i, o ) o.[ i ].rebind_axon( src.[ i ].1 ); };
    };

    feature void reset( mutable ); // resets all moments
    feature void adapt( mutable, const :node_s* node );

    stamp :adl = aware bcore_array { aware : => []; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** The adaptive is the trainable unit.
 *  It defines all interface functions for inference, gradient computation,
 *  gradient accumulation, gradient interchange, adaptive update and adaptive
 *  interchange.
 */
group :adaptive =
{
    /// retrieves entry-holor-format (shape & type); returns format
    feature strict bhvm_holor_s* get_format_en( const, bhvm_holor_s* format );

    /// retrieves exit-holor-format (shape & type); returns format
    feature strict bhvm_holor_s* get_format_ex( const, bhvm_holor_s* format );

    /// axon-pass
    feature void axon_pass( mutable, const bhvm_holor_s* ax_en, bhvm_holor_s* ax_ex ) = {};

    /// dendrite-pass (ag_en may be NULL)
    feature void dendrite_pass( mutable, const bhvm_holor_s* ag_ex, bhvm_holor_s* ag_en ) = {};

    /// resets cyclic variables
    feature void cyclic_reset( mutable ) = {};

    /** Obtains a holor-probe for accumulative gradients; returns probe
     *  The probe is to be deemed invalid after the adaptive has been modified
     *  in a way that effects holor binding. If a probe is used to change holor bindings
     *  rebind_holors must be executed afterwards.
     *  This function should execute fast.
     */
    feature ::adaptor_probe_s* get_adaptor_probe( const, ::adaptor_probe_s* probe ) = { return probe; };

    /** Explicitly rebinds internal holorbase.
     *  This can be necessary when holors of a probe have been externally reallocated.
     *  Use with care!
     */
    feature void rebind_holors( mutable );

    /// outputs current status information to sink
    feature void status_to_sink( const, sz_t verbosity, bcore_sink* sink ) = { if( verbosity > 0 ) bcore_txt_ml_a_to_sink( o, sink ); };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** The builder creates a new (untrained) adaptive.
 */
group :builder =
{
    /// defines entry-holor format (shape & type)
    feature strict void set_format_en( mutable, const bhvm_holor_s* format );

    /// defines exit-holor (shape & type)
    feature strict void set_format_ex( mutable, const bhvm_holor_s* format );

    /// creates adaptive releasing ownership
    feature strict ::adaptive* create_adaptive( const );
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** The tutor defines an adaptive and a training task.
 *  It provides routines for adaptive generation, priming and testing.
 *  Priming and testing may mutate the tutor in a concurrent fashion.
 */
group :tutor =
{
    /// creates adaptive via builder
    feature strict ::adaptive* create_adaptive( const );

    /// creates adaptor for adaptive
    feature strict ::adaptor* create_adaptor( const );

    /// resets the training session
    feature void reset( mutable ) = {};

    /// primes a specified adaptive (must be concurrent for tutor)
    feature void prime( mutable, ::adaptive* adaptive ) = {};

    /** Tests a specified adaptive (must be concurrent for tutor) and writes result to log;
     *  Testing may mutate the tutor, if necessary and done in a thread safe manner.
     *  Mutation is not desirable, though, and should never affect the reliability or interpretability
     *  of test results.
     */
    feature void test( mutable, const ::adaptive* adaptive, sz_t verbosity, bcore_sink* log ) = {};

    /// outputs current status information to sink
    feature void status_to_sink( const, sz_t verbosity, bcore_sink* sink ) = { if( verbosity > 0 ) bcore_txt_ml_a_to_sink( o, sink ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t bhpt_sketch_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHPT_SKETCH_H
