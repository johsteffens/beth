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

#ifndef BMATH_ALGEBRAIC_SPECS_H
#define BMATH_ALGEBRAIC_SPECS_H

/// Collection of algebraic perspectives.

#include "bcore_std.h"

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

/**********************************************************************************************************************/

typedef struct bmath_abelian_group_s
{
    bcore_spect_header_s header;

    bmath_fp_add fp_add; // mandatory
    bmath_fp_zro fp_zro; // mandatory
    bmath_fp_neg fp_neg; // mandatory
    bmath_fp_sub fp_sub; // optional

} bmath_abelian_group_s;

const bmath_abelian_group_s* bmath_abelian_group_s_get_typed( tp_t type );
const bmath_abelian_group_s* bmath_abelian_group_s_get_aware( vc_t o );

void bmath_abelian_group_spect_add( const bmath_abelian_group_s* p, vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_spect_sub( const bmath_abelian_group_s* p, vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_spect_neg( const bmath_abelian_group_s* p, vd_t o, vc_t op );
void bmath_abelian_group_spect_zro( const bmath_abelian_group_s* p, vd_t o );

void bmath_abelian_group_typed_add( tp_t tp, vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_typed_sub( tp_t tp, vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_typed_neg( tp_t tp, vd_t o, vc_t op );
void bmath_abelian_group_typed_zro( tp_t tp, vd_t o );

void bmath_abelian_group_aware_add( vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_aware_sub( vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_aware_neg( vd_t o, vc_t op );
void bmath_abelian_group_aware_zro( vd_t o );

void bmath_abelian_group_q_add( const sr_s* o, vc_t op1, vc_t op2 );
void bmath_abelian_group_q_sub( const sr_s* o, vc_t op1, vc_t op2 );
void bmath_abelian_group_q_neg( const sr_s* o, vc_t op );
void bmath_abelian_group_q_zro( const sr_s* o );

/**********************************************************************************************************************/

vd_t bmath_algebraic_specs_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_ALGEBRAIC_SPECS_H
