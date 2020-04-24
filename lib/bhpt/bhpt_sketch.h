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
#include "bhpt_planted.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhpt

PLANT_GROUP( bhpt, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** The adaptor defines a method for updating adaptive holors from accumulated gradients
 *  It is assigned to an adaptive, which prepares (resets) its adapt function.
 */
group :adaptor =
{
    signature void get_min_max( const, f3_t* min, f3_t* max );

    stamp :node  = aware :
    {
        bhvm_holor_s -> axon;
        bhvm_holor_s -> grad;
        func : :get_min_max =
        {
            if( o->axon )
            {
                if( min ) *min = bhvm_value_s_get_min_f3( &o->axon->v );
                if( max ) *max = bhvm_value_s_get_max_f3( &o->axon->v );
            }
        };
    };

    stamp :probe = aware bcore_array
    {
        :node_s [];
        func : :get_min_max =
        {
            f3_t min_l = 0, max_l = 0;
            BFOR_EACH( i, o )
            {
                :node_s_get_min_max( &o->data[ i ], &min_l, &max_l );
                if( min ) *min = ( i > 0 ) ? f3_min( *min, min_l ) : min_l;
                if( max ) *max = ( i > 0 ) ? f3_max( *max, max_l ) : max_l;
            }
        };
    };

    feature 'a' void reset( mutable ); // resets all moments
    feature 'a' void adapt( mutable, const :node_s* node );

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
    feature strict 'a' bhvm_holor_s* get_format_en( const, bhvm_holor_s* format );

    /// retrieves exit-holor-format (shape & type); returns format
    feature strict 'a' bhvm_holor_s* get_format_ex( const, bhvm_holor_s* format );

    /// axon-pass
    feature 'a' void axon_pass( mutable, const bhvm_holor_s* ax_en, bhvm_holor_s* ax_ex ) = {};

    /// dendrite-pass (ag_en may be NULL)
    feature 'a' void dendrite_pass( mutable, const bhvm_holor_s* ag_ex, bhvm_holor_s* ag_en ) = {};

    /// resets cyclic variables
    feature 'a' void cyclic_reset( mutable ) = {};

    /// obtains a holor-probe for accumulative gradients; returns probe
    feature 'a' ::adaptor_probe_s* get_adaptor_probe( const, ::adaptor_probe_s* probe ) = { return probe; };

    /// outputs current status information to sink
    feature 'a' void status_to_sink( const, sz_t verbosity, bcore_sink* sink ) = { if( verbosity > 0 ) bcore_txt_ml_a_to_sink( o, sink ); };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** The builder creates a new (untrained) adaptive.
 */
group :builder =
{
    /// defines entry-holor format (shape & type)
    feature strict 'a' void set_format_en( mutable, const bhvm_holor_s* format );

    /// defines exit-holor (shape & type)
    feature strict 'a' void set_format_ex( mutable, const bhvm_holor_s* format );

    /// creates adaptive releasing ownership
    feature strict 'a' ::adaptive* create_adaptive( const );
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** The tutor defines a training task.
 *  It primes and tests adaptives (thread safe).
 */
group :tutor =
{
    /// creates adaptive via builder releasing ownership
    feature strict 'a' ::adaptive* create_adaptive( const, const ::builder* builder );

    /// resets the training session
    feature 'a' void reset( mutable ) = {};

    /// primes a specified adaptive (concurrent for tutor)
    feature 'a' void prime( mutable, ::adaptive* adaptive ) = {};

    /// tests a specified adaptive (concurrent for tutor) and writes result to log
    feature 'a' void test( const, const ::adaptive* adaptive, sz_t verbosity, bcore_sink* log ) = {};

    /// outputs current status information to sink
    feature 'a' void status_to_sink( const, sz_t verbosity, bcore_sink* sink ) = { if( verbosity > 0 ) bcore_txt_ml_a_to_sink( o, sink ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhpt

/**********************************************************************************************************************/

vd_t bhpt_sketch_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHPT_SKETCH_H
