/** Copyright 2018 Johannes Bernhard Steffens
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

#ifndef BMATH_SPECT_ALGEBRAIC_H
#define BMATH_SPECT_ALGEBRAIC_H

/// Algebraic perspectives: Group and Ring.

#include "bcore_std.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/

/// operation - features
typedef void (*bmath_fp_add )( vd_t o, vc_t op1, vc_t op2 ); // o = op1 + op2
typedef void (*bmath_fp_sub )( vd_t o, vc_t op1, vc_t op2 ); // o = op1 - op2
typedef void (*bmath_fp_neg )( vd_t o, vc_t op ) ;           // o = -op
typedef void (*bmath_fp_zro )( vd_t o );                     // o = 0 (null_element)
typedef void (*bmath_fp_mul )( vd_t o, vc_t op1, vc_t op2 ); // o = op1 * op2
typedef void (*bmath_fp_div )( vd_t o, vc_t op1, vc_t op2 ); // o = op1 / op2
typedef void (*bmath_fp_inv )( vd_t o, vc_t op );            // o = 1 / op
typedef void (*bmath_fp_one )( vd_t o );                     // o = 1 (one_element)
typedef void (*bmath_fp_cpy )( vd_t o, vc_t op );            // o = op; note that this is not always a full copy (for vectors it may be a projection)

/**********************************************************************************************************************/
// bmath_group_s (usually abelian)

BCORE_DECLARE_SPECT( bmath_group_s )
{
    bcore_spect_header_s header;
    const bcore_inst_s* spect_inst;
    bmath_fp_add add; // mandatory
    bmath_fp_zro zro; // mandatory
    bmath_fp_neg neg; // mandatory
    bmath_fp_sub sub; // optional
    bmath_fp_cpy cpy; // optional
};

BCORE_DECLARE_SPECT_MAPF_V2( bmath_group, add, vd_t, vc_t, op1, vc_t, op2 )
BCORE_DECLARE_SPECT_MAPF_V0( bmath_group, zro, vd_t )
BCORE_DECLARE_SPECT_MAPF_V1( bmath_group, neg, vd_t, vc_t, op1 )
BCORE_DECLARE_SPECT_MAPD_V2( bmath_group, sub, vd_t, vc_t, op1, vc_t, op2 )
BCORE_DECLARE_SPECT_MAPD_V1( bmath_group, cpy, vd_t, vc_t, op1 )

/**********************************************************************************************************************/
// bmath_ring_s

BCORE_DECLARE_SPECT( bmath_ring_s )
{
    bcore_spect_header_s header;
    const bcore_inst_s* spect_inst;

    bmath_fp_add add; // mandatory
    bmath_fp_zro zro; // mandatory
    bmath_fp_neg neg; // mandatory
    bmath_fp_sub sub; // optional
    bmath_fp_cpy cpy; // optional

    bmath_fp_mul mul; // mandatory
    bmath_fp_one one; // optional
    bmath_fp_inv inv; // optional
    bmath_fp_div div; // optional
};

BCORE_DECLARE_SPECT_MAPF_V2( bmath_ring, add, vd_t, vc_t, op1, vc_t, op2 )
BCORE_DECLARE_SPECT_MAPF_V0( bmath_ring, zro, vd_t )
BCORE_DECLARE_SPECT_MAPF_V1( bmath_ring, neg, vd_t, vc_t, op1 )
BCORE_DECLARE_SPECT_MAPD_V2( bmath_ring, sub, vd_t, vc_t, op1, vc_t, op2 )
BCORE_DECLARE_SPECT_MAPD_V1( bmath_ring, cpy, vd_t, vc_t, op1 )

BCORE_DECLARE_SPECT_MAPF_V2( bmath_ring, mul, vd_t, vc_t, op1, vc_t, op2 )
BCORE_DECLARE_SPECT_MAPF_V0( bmath_ring, one, vd_t )
BCORE_DECLARE_SPECT_MAPF_V1( bmath_ring, inv, vd_t, vc_t, op1 )
BCORE_DECLARE_SPECT_MAPD_V2( bmath_ring, div, vd_t, vc_t, op1, vc_t, op2 )

/**********************************************************************************************************************/

vd_t bmath_spect_algebraic_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_ALGEBRAIC_H

