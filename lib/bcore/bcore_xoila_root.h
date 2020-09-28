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

/** Beth inexpandable group.
 *  This group does not generate effective plant-code but is otherwise accessible within the plant framework.
 *  It manages components, which have a low-level implementation: E.g. beth-system features.
 */

#ifndef BCORE_XOILA_ROOT_H
#define BCORE_XOILA_ROOT_H

#include "bcore_std.h"
#include "bcore_xoila_out.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_xoila_root
XOILA_DEFINE_GROUP( bcore_xoila_root, bcore_inst )

#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

set inexpandable;

// External types

// primitives
type u0_t;
type u1_t;
type u2_t;
type u3_t;
type s0_t;
type s1_t;
type s2_t;
type s3_t;
type f2_t;
type f3_t;
type uz_t;
type sz_t;
type tp_t;
type aware_t;
type sc_t;
type sd_t;

// objects
type sr_s;

/// st_s
group st = bcore_inst
{
    stamp : = bcore_inst
    {
        sd_t data;
        sc_t sc;
        uz_t size;
        uz_t space;
    };
};

group bcore_inst   = bcore_inst {};
group bcore_array  = bcore_inst {};
group bcore_via    = bcore_inst {};
group bcore_source = bcore_inst {};
group bcore_sink   = bcore_inst {};

group bcore_fp = bcore_inst
{
    feature '' void copy_typed( mutable, tp_t type, vc_t src );
};

/// default functions in a stamp
group bcore_stamp_funcs = bcore_inst
{
    signature void init( mutable );
    signature void down( mutable );
    signature void copy( mutable, const @* src );

    signature   @* create(  plain );
    signature void discard( mutable );
    signature   @* clone(   const );
};

embed "bcore_arr.x";

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bcore_xoila_root

/**********************************************************************************************************************/

#endif  // BCORE_XOILA_ROOT_H
