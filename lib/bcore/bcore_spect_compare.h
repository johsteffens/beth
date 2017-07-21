/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_SPECT_COMPARE_H
#define BCORE_SPECT_COMPARE_H

/** Generic comparison of two objects of the same type.
 *  Determines equality and induces a canonic order of instances.
 */

#include "bcore_flect.h"
#include "bcore_features.h"
#include "bcore_spect_via.h"

/// optional features:
typedef u2_t (*bcore_compare_fp)( vc_t obj1, vc_t obj2 );  // also supported as (ap_t, "compare")

typedef struct bcore_compare_s bcore_compare_s;
typedef struct bcore_compare_s
{
    aware_t p_type;  // type of perspective
    tp_t    o_type;  // type of object
    const bcore_via_s* via;
    fp_t fp_compare; // bcore_compare_fp or ap_t
    u2_t ( *compare )( const bcore_compare_s* p, vc_t obj1, vc_t obj2 );
} bcore_compare_s;

const bcore_compare_s* bcore_compare_s_get_typed( tp_t type );

u2_t bcore_compare_spect( const bcore_compare_s* p, vc_t obj1, vc_t obj2 );
u2_t bcore_compare_typed( tp_t type,                vc_t obj1, vc_t obj2 );
u2_t bcore_compare_aware(                           vc_t obj1, vc_t obj2 );

#endif // BCORE_SPECT_COMPARE_H
