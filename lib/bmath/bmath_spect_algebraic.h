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

typedef void (*bmath_fp_cpt )( vc_t o,                    tp_t tres, vd_t res ); // typed copy: canonic interface for copy with type conversion
typedef void (*bmath_fp_mlt )( vc_t o, tp_t top, vc_t op, tp_t tres, vd_t res ); // typed multiplication: canonic interface for mappings associated with multiplication

/// operation - features
typedef bl_t (*bmath_fp_is_zro )( const bmath_group* o );          // checks zeroness
typedef bl_t (*bmath_fp_is_equ )( const bmath_group* o, vc_t op ); // checks equality

typedef void (*bmath_fp_add )( const bmath_group* o, vc_t op, vd_t res ); // res = o + op
typedef void (*bmath_fp_sub )( const bmath_group* o, vc_t op, vd_t res ); // res = o - op
typedef void (*bmath_fp_neg )( const bmath_group* o, vd_t res );          // res = -o
typedef void (*bmath_fp_zro )(       bmath_group* o );                    // o = 0 (null_element)
typedef void (*bmath_fp_cpy )( const bmath_group* o, vd_t res );          // o = op; note that this is not always a full copy (for vectors it may be a projection)

/**********************************************************************************************************************/
// bmath_group_s (usually abelian)

BCORE_DECLARE_SPECT( bmath_group_s )
{
    bcore_spect_header_s header;
    const bcore_inst_s* spect_inst;

    bmath_fp_is_equ is_equ; // mandatory
    bmath_fp_is_zro is_zro; // mandatory

    bmath_fp_add add; // mandatory
    bmath_fp_zro zro; // mandatory
    bmath_fp_neg neg; // mandatory
    bmath_fp_sub sub; // optional
    bmath_fp_cpy cpy; // optional
    bmath_fp_mlt mlt; // optional
};

BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP1( bmath_group, is_equ, bl_t, const bmath_group*, op )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP1( bmath_group, is_zro, bl_t )

BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAP1( bmath_group, add, const bmath_group*, op, bmath_group*, res )
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP1( bmath_group, zro )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAP1( bmath_group, neg, bmath_group*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_group, sub, const bmath_group*, op, bmath_group*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAPX( bmath_group, cpy, bmath_group*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG4_MAP1( bmath_group, mlt, tp_t, top, vc_t, op, tp_t, tres, vd_t, res )

/**********************************************************************************************************************/
// bmath_ring_s

typedef struct bmath_ring bmath_ring;

typedef bl_t (*bmath_fp_is_one )( const bmath_ring* o );          // checks oneness
typedef void (*bmath_fp_mul )( const bmath_ring* o, vc_t op, vd_t res ); // res = o * op
typedef void (*bmath_fp_div )( const bmath_ring* o, vc_t op, vd_t res ); // res = o / op
typedef void (*bmath_fp_inv )( const bmath_ring* o, vd_t res );          // res = 1 / o
typedef void (*bmath_fp_one )(       bmath_ring* o );                    // o = 1 (one_element)

BCORE_DECLARE_SPECT( bmath_ring_s )
{
    bcore_spect_header_s header;
    const bcore_inst_s* spect_inst;

    bmath_fp_is_equ is_equ; // mandatory
    bmath_fp_is_zro is_zro; // mandatory
    bmath_fp_is_zro is_one; // mandatory

    bmath_fp_add add; // mandatory
    bmath_fp_zro zro; // mandatory
    bmath_fp_neg neg; // mandatory
    bmath_fp_sub sub; // optional
    bmath_fp_cpy cpy; // optional
    bmath_fp_mlt mlt; // optional

    bmath_fp_mul mul; // mandatory
    bmath_fp_one one; // mandatory
    bmath_fp_inv inv; // optional
    bmath_fp_div div; // optional
};

BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP1( bmath_ring, is_equ, bl_t, const bmath_group*, op )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP1( bmath_ring, is_zro, bl_t )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP1( bmath_ring, is_one, bl_t )

BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAP1( bmath_ring, add, const bmath_ring*, op, bmath_ring*, res )
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP1( bmath_ring, zro )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAP1( bmath_ring, neg, bmath_ring*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bmath_ring, sub, const bmath_ring*, op, bmath_ring*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAPX( bmath_ring, cpy, bmath_ring*, res )

BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAP1( bmath_ring, mul, const bmath_ring*, op, bmath_ring*, res )
BCORE_FUNC_SPECT_CONST0_RET0_ARG0_MAP1( bmath_ring, one )
BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAP1( bmath_ring, inv, bmath_ring*, res )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAP1( bmath_ring, div, const bmath_ring*, op, bmath_ring*, res )

BCORE_FUNC_SPECT_CONST1_RET0_ARG4_MAPX( bmath_ring, mlt, tp_t, top, vc_t, op, tp_t, tres, vd_t, res )

/**********************************************************************************************************************/

vd_t bmath_spect_algebraic_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_ALGEBRAIC_H

