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

/**********************************************************************************************************************/

/** Global Interface Sketch of this Library*/

/**********************************************************************************************************************/

include "bmath_std.h";
include "bhvm_std.h";

/**********************************************************************************************************************/

/** The adaptor defines a method for updating adaptive holors from accumulated gradients
 *  It is assigned to an adaptive, which prepares (resets) its adapt function.
 */
group :adaptor =
{
    signature void acc_min_max( c @* o, m f3_t* min, m f3_t* max ); // updates min.0, max.0 only if new value exceeds range
    signature void zro_grad( m @* o );
    signature void acc_grad( m @* o, c @* src );

    signature void acc_stats( c @* o, m bhvm_stats_s* axon, m bhvm_stats_s* grad );

    /// Makes axon values of o reference axon values of src; run adaptive_a_rebind_holors afterwards
    signature void rebind_axon( m @* o, m @* src );

    /// The node weakly references adaptive axon and gradient holors.
    stamp :node_s  = aware :
    {
        private bhvm_holor_s* axon;
        private bhvm_holor_s* grad;

        func : .zro_grad  { o.grad.v.zro(); };
        func : .acc_grad  { assert( o.grad.s.is_equal( src.grad.s.1 ) ); o.grad.v.acc( src.grad.v.1 ); };
        func : .rebind_axon { assert( o.axon.s.is_equal( src.axon.s.1 ) ); o.axon.v.weak( src.axon.v.1 ); };

        func : .acc_min_max
        {
            if( o.axon )
            {
                if( min.1 ) min.0 = f3_min( min.0, o.axon.v.get_min_f3() );
                if( max.1 ) max.0 = f3_max( max.0, o.axon.v.get_max_f3() );
            }
        };

        func : .acc_stats
        {
            if( axon && o.axon ) o.axon.v.stats_acc( axon );
            if( grad && o.grad ) o.grad.v.stats_acc( grad );
        };
    };

    /** The probe is obtained via function get_adaptor_probe.
     *  It is valid after obtaining until a modification
     *  of the underlying adaptive that can change holor bindings.
     *  If the probe is used to change holor bindings,
     *  adaptive_a_rebind_holors must be called.
     */
    stamp :probe_s = aware x_array
    {
        :node_s [];
        func : .acc_min_max { foreach( c$* e in o ) e.acc_min_max( min.1, max.1 ); };
        func : .acc_stats   { foreach( c$* e in o ) e.acc_stats( axon, grad ); };
        func : .zro_grad    { foreach( m$* e in o ) e.zro_grad(); };
        func : .acc_grad    { assert( o.size == src.size ); foreach( m$* e in o ) e.acc_grad( src.[ __i ].1 ); };
        func : .rebind_axon { assert( o.size == src.size ); foreach( m$* e in o ) e.rebind_axon( src.[ __i ].1 ); };
        wrap x_array.set_size;
    };

    feature void reset( m @* o ); // resets all moments
    feature void adapt( m @* o, c :node_s* node );

    stamp :adl_s = aware x_array
    {
        aware : => [];
        wrap x_array.set_size;
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/** The adaptive is the trainable unit.
 *  It defines all interface functions for inference, gradient computation,
 *  gradient accumulation, gradient interchange, adaptive update and adaptive
 *  interchange.
 */
group :adaptive =
{
    /// retrieves entry-holor-format (shape & type); returns format
    feature strict m bhvm_holor_s* get_format_en( c @* o, m bhvm_holor_s* format );

    /// retrieves exit-holor-format (shape & type); returns format
    feature strict m bhvm_holor_s* get_format_ex( c @* o, m bhvm_holor_s* format );

    /// axon-pass
    feature void axon_pass( m @* o, c bhvm_holor_s* ax_en, m bhvm_holor_s* ax_ex ) {};

    /// dendrite-pass (ag_en may be NULL)
    feature void dendrite_pass( m @* o, c bhvm_holor_s* ag_ex, m bhvm_holor_s* ag_en ) {};

    /// resets cyclic variables
    feature void cyclic_reset( m @* o ) {};

    /** Obtains a holor-probe for accumulative gradients; returns probe
     *  The probe is to be deemed invalid after the adaptive has been modified
     *  in a way that effects holor binding. If a probe is used to change holor bindings
     *  rebind_holors must be executed afterwards.
     *  This function should execute fast.
     */
    feature m ::adaptor_probe_s* get_adaptor_probe( c @* o, m ::adaptor_probe_s* probe ) { return probe; };

    /** Explicitly rebinds internal holorbase.
     *  This can be necessary when holors of a probe have been externally reallocated.
     *  Use with care!
     */
    feature void rebind_holors( m @* o );

    /// outputs current status information to sink
    feature void status_to_sink( c @* o, sz_t verbosity, m bcore_sink* sink ) { if( verbosity > 0 ) bcore_txt_ml_a_to_sink( o, sink ); };

    /// command-line interface TODO: implement bcore_main feature instead
    feature er_t run_command( m@* o, sc_t command )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "Command '#<sc_t>': run_command is not implemented in '#<sc_t>'.", command, bnameof( o._ ) );
    };

};

// ---------------------------------------------------------------------------------------------------------------------

/** The builder creates a new (untrained) adaptive.
 */
group :builder =
{
    /// defines entry-holor format (shape & type)
    feature strict void set_format_en( m @* o, c bhvm_holor_s* format );

    /// defines exit-holor (shape & type)
    feature strict void set_format_ex( m @* o, c bhvm_holor_s* format );

    /// creates adaptive releasing ownership
    feature strict d ::adaptive* create_adaptive( c @* o );
};

// ---------------------------------------------------------------------------------------------------------------------

/** Test result is an object containing results of a adaptive test.
 *  Multiple test can be stored in a database to monitor the training progress.
 */
group :test_result =
{
    feature o to_sink( c @* o, sz_t verbosity, m bcore_sink* sink );

    stamp :adl_s = aware x_array
    {
        aware : => [];
        func :.to_sink { foreach( c$* e in o ) { e.to_sink( verbosity, sink ); sink.push_sc( "\n" ); } return o; };
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/** The tutor defines an adaptive and a training task.
 *  It provides routines for adaptive generation, priming and testing.
 *  Priming and testing may mutate the tutor in a concurrent fashion.
 */
group :tutor =
{
    /// creates adaptive (typically from internal builder)
    feature strict d ::adaptive* create_adaptive( c @* o );

    /// creates adaptor for adaptive
    feature strict d ::adaptor* create_adaptor( c @* o );

    /// resets the training session
    feature void reset( m @* o ) {};

    /// primes a specified adaptive (must be concurrent for tutor)
    feature void prime( m @* o, m ::adaptive* adaptive ) {};

    /** Tests a specified adaptive (must be concurrent for tutor) and creates a test result;
     *  Testing may mutate the tutor if necessary and done in a thread safe manner.
     *  Mutation is not desirable. It should never affect the reliability or interpretability
     *  of test results.
     *  Simple use: o.test( <adaptive> )^.to_sink( <verbosity>, <sink> );
     */
    feature d ::test_result* test( m @* o, c ::adaptive* adaptive ) { return NULL; };

    /// outputs current status information to sink
    feature void status_to_sink( c @* o, sz_t verbosity, m bcore_sink* sink ) { if( verbosity > 0 ) o.cast( x_btml* ).to_sink( sink ); };
};

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

