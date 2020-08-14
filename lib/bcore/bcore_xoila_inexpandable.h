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

#ifndef BCORE_XOILA_INEXPANDABLE_H
#define BCORE_XOILA_INEXPANDABLE_H

#include "bcore_std.h"
#include "bcore_xoila_out.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_fp
XOILA_DEFINE_GROUP( bcore_inexpandable, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

expandable = false;

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
type st_s;
type sr_s;

// perspectives
type bcore_inst;
type bcore_array;
type bcore_via;
type bcore_source;
type bcore_sink;

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bcore_fp

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_fp
XOILA_DEFINE_GROUP( bcore_fp, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

expandable = false;

feature '' void copy_typed( mutable, tp_t type, vc_t src );

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bcore_fp

/**********************************************************************************************************************/

vd_t bcore_xoila_inexpandable_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BCORE_XOILA_INEXPANDABLE_H
