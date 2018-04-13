/** Copyright 2017 Johannes Bernhard Steffens
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

#ifndef BCORE_SPECT_COMPARE_H
#define BCORE_SPECT_COMPARE_H

/** Generic comparison of two objects of the same type.
 *  Determines equality and induces a canonic order of instances.
 *  The comparison function returns...
 *      ==0 when both objects are equal.
 *       >0 when object1 is ordered before object2
 *       <0 when object1 is ordered after  object2
 */

#include "bcore_spect.h"
#include "bcore_features.h"
#include "bcore_spect_via.h"

/// optional features:
typedef s2_t (*bcore_fp_compare)( vc_t obj1, vc_t obj2 );  // also supported as (ap_t, "compare")

typedef struct bcore_compare_s bcore_compare_s;
typedef struct bcore_compare_s
{
    aware_t p_type;  // type of perspective
    tp_t    o_type;  // type of object
    const bcore_via_s* via;
    fp_t fp_compare; // bcore_fp_compare or ap_t
    s2_t ( *compare )( const bcore_compare_s* p, vc_t obj1, vc_t obj2 ); // internal compare function
} bcore_compare_s;

BCORE_DEFINE_INLINE_SPECT_GET_TYPED_CACHED( bcore_compare_s )
BCORE_DEFINE_INLINE_SPECT_GET_AWARE( bcore_compare_s )

s2_t bcore_compare_spect(   const bcore_compare_s* p, vc_t obj1, vc_t obj2 );
s2_t bcore_compare_typed(   tp_t type,                vc_t obj1, vc_t obj2 );
s2_t bcore_compare_bityped( tp_t type1, vc_t obj1, tp_t type2, vc_t obj2 );
s2_t bcore_compare_sr( sr_s obj1, sr_s obj2 );
s2_t bcore_compare_q_sr( const sr_s* obj1, const sr_s* obj2 );

bool bcore_equal_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 );
bool bcore_equal_typed( tp_t type,                vc_t obj1, vc_t obj2 );
bool bcore_equal_bityped( tp_t type1, vc_t obj1, tp_t type2, vc_t obj2 );

/// Aware operations can handle objects of different type by imposing a type order.
s2_t bcore_compare_aware( vc_t obj1, vc_t obj2 );
bool bcore_equal_aware(   vc_t obj1, vc_t obj2 );

/// diff: in case values differ, a string is created describing the nature of difference
st_s* bcore_diff_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 );
st_s* bcore_diff_typed( tp_t type,                vc_t obj1, vc_t obj2 );
st_s* bcore_diff_bityped( tp_t type1, vc_t obj1, tp_t type2, vc_t obj2 );
st_s* bcore_diff_aware( vc_t obj1, vc_t obj2 );
st_s* bcore_diff_sr( sr_s obj1, sr_s obj2 );
st_s* bcore_diff_q_sr( const sr_s* obj1, const sr_s* obj2 );

vd_t bcore_spect_compare_signal_handler( const bcore_signal_s* o );

#endif // BCORE_SPECT_COMPARE_H
