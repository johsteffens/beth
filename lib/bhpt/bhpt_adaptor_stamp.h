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

 /// Collection of bhpt_adaptor stamps

#ifndef BHPT_ADAPTOR_STAMP_H
#define BHPT_ADAPTOR_STAMP_H

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_std.h"
#include "bhpt_sketch.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhpt_adaptor_stamp

PLANT_GROUP( bhpt_adaptor_stamp, bhpt_adaptor )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// Adding gradient*epsilon to adaptive
stamp bhpt_adaptor_epsilon = aware bhpt_adaptor
{
    f3_t epsilon;
    func bhpt_adaptor : reset = {};
    func bhpt_adaptor : adapt =
    {
        assert( bhvm_shape_s_is_equal( &accugrad->s, &adaptive->s ) );
        bhvm_value_s_mul_scl_f3_acc( &accugrad->v, o->epsilon, &adaptive->v );
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// List of adaptors applied in sequence
stamp bhpt_adaptor_list = aware bhpt_adaptor
{
    bhpt_adaptor_adl_s adl;
    func bhpt_adaptor : reset = { BFOR_EACH( i, &o->adl ) bhpt_adaptor_a_reset( o->adl.data[ i ] ); };
    func bhpt_adaptor : adapt = { BFOR_EACH( i, &o->adl ) bhpt_adaptor_a_adapt( o->adl.data[ i ], accugrad, adaptive ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhpt_adaptor_stamp

/**********************************************************************************************************************/

vd_t bhpt_adaptor_stamp_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHPT_ADAPTOR_STAMP_H
