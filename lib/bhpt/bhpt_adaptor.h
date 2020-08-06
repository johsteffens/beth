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

#ifndef BHPT_ADAPTOR_H
#define BHPT_ADAPTOR_H

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_std.h"
#include "bhpt_sketch.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhpt_adaptor_stamp

XOILA_DEFINE_GROUP( bhpt_adaptor_stamp, bhpt_adaptor )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// Adding gradient*epsilon to adaptive
stamp bhpt_adaptor_epsilon = aware bhpt_adaptor
{
    f3_t epsilon;
    func bhpt_adaptor : reset = {};
    func bhpt_adaptor : adapt =
    {
        assert( bhvm_shape_s_is_equal( &node->axon->s, &node->grad->s ) );
        bhvm_value_s_mul_scl_f3_acc( &node->grad->v, o->epsilon, &node->axon->v );
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// Applying l2-regularization (weight-decay) to weights: w -= w * lambda;
stamp bhpt_adaptor_reg_l2 = aware bhpt_adaptor
{
    f3_t lambda;
    func bhpt_adaptor : reset = {};
    func bhpt_adaptor : adapt = { bhvm_value_s_mul_scl_f3_acc( &node->axon->v, -o->lambda, &node->axon->v ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp bhpt_adaptor_zro_grad = aware bhpt_adaptor
{
    func bhpt_adaptor : adapt = { bhvm_value_s_zro( &node->grad->v ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// List of adaptors applied in sequence
stamp bhpt_adaptor_list = aware bcore_array
{
    aware bhpt_adaptor => [];

    func bhpt_adaptor : reset =
    {
        BFOR_SIZE( i, o->size ) bhpt_adaptor_a_reset( o->data[ i ] );
    };

    func bhpt_adaptor : adapt =
    {
        BFOR_SIZE( i, o->size ) bhpt_adaptor_a_adapt( o->data[ i ], node );
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhpt_adaptor_stamp

/**********************************************************************************************************************/

vd_t bhpt_adaptor_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHPT_ADAPTOR_H
