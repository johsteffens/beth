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

typedef struct bmath_group bmath_group;

/**********************************************************************************************************************/

/// operation - features
typedef void (*bmath_fp_add )( const bmath_group* o, const bmath_group* op, bmath_group* res ); // res = o + op
typedef void (*bmath_fp_sub )( const bmath_group* o, const bmath_group* op, bmath_group* res ); // res = o - op
typedef void (*bmath_fp_neg )( const bmath_group* o, bmath_group* res );     // res = -o
typedef void (*bmath_fp_zro )(       bmath_group* o );                       // o = 0 (null_element)
typedef void (*bmath_fp_cpy )( const bmath_group* o, bmath_group* res );     // o = op; note that this is not always a full copy (for vectors it may be a projection)

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

BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAP1( bmath_group, add, const bmath_group*, op, bmath_group*, res )
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP1( bmath_group, zro )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAP1( bmath_group, neg, bmath_group*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_group, sub, const bmath_group*, op, bmath_group*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAPX( bmath_group, cpy, bmath_group*, res )

/**********************************************************************************************************************/
// bmath_ring_s

typedef struct bmath_ring bmath_ring;

typedef void (*bmath_fp_mul )( const bmath_ring* o, const bmath_ring* op, bmath_ring* res ); // res = o * op
typedef void (*bmath_fp_div )( const bmath_ring* o, const bmath_ring* op, bmath_ring* res ); // res = o / op
typedef void (*bmath_fp_inv )( const bmath_ring* o, bmath_ring* res );            // res = 1 / o
typedef void (*bmath_fp_one )( bmath_ring* o );                                   // o = 1 (one_element)

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

BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAP1( bmath_ring, add, const bmath_ring*, op, bmath_ring*, res )
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP1( bmath_ring, zro )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAP1( bmath_ring, neg, bmath_ring*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_ring, sub, const bmath_ring*, op, bmath_ring*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAPX( bmath_ring, cpy, bmath_ring*, res )

BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAP1( bmath_ring, mul, const bmath_ring*, op, bmath_ring*, res )
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAPX( bmath_ring, one )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAP1( bmath_ring, inv, bmath_ring*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAP1( bmath_ring, div, const bmath_ring*, op, bmath_ring*, res )

/**********************************************************************************************************************/

vd_t bmath_spect_algebraic_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_ALGEBRAIC_H

