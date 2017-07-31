/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SPECT_COMPARE_H
#define BCORE_SPECT_COMPARE_H

/** Generic comparison of two objects of the same type.
 *  Determines equality and induces a canonic order of instances.
 *  The comparison function returns...
 *      ==0 when both objects are equal.
 *       >0 when object1 is ordered before object2
 *       <0 when object1 is ordered after  object2
 */

#include "bcore_flect.h"
#include "bcore_features.h"
#include "bcore_spect_via.h"

/// optional features:
typedef s2_t (*bcore_compare_fp)( vc_t obj1, vc_t obj2 );  // also supported as (ap_t, "compare")

typedef struct bcore_compare_s bcore_compare_s;
typedef struct bcore_compare_s
{
    aware_t p_type;  // type of perspective
    tp_t    o_type;  // type of object
    const bcore_via_s* via;
    fp_t fp_compare; // bcore_compare_fp or ap_t
    s2_t ( *compare )( const bcore_compare_s* p, vc_t obj1, vc_t obj2 );
} bcore_compare_s;

bcore_flect_self_s* bcore_compare_s_create_self( void );
const bcore_compare_s* bcore_compare_s_get_typed( tp_t type );

s2_t bcore_compare_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 );
s2_t bcore_compare_typed( tp_t type,                vc_t obj1, vc_t obj2 );

bool bcore_equal_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 );
bool bcore_equal_typed( tp_t type,                vc_t obj1, vc_t obj2 );

/// Aware operations can handle objects of different type by imposing a type order.
s2_t bcore_compare_aware( vc_t obj1, vc_t obj2 );
bool bcore_equal_aware(   vc_t obj1, vc_t obj2 );

/**********************************************************************************************************************/
// testing, debugging

bcore_string_s* bcore_spect_compare_selftest( void );

#endif // BCORE_SPECT_COMPARE_H
