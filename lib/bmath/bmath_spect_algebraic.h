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

/// Collection of algebraic perspectives.

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

/**********************************************************************************************************************/
// bmath_abelian_group_s
typedef struct bmath_abelian_group_s
{
    bcore_spect_header_s header;
    const bcore_inst_s* spect_inst;
    bmath_fp_add fp_add; // mandatory
    bmath_fp_zro fp_zro; // mandatory
    bmath_fp_neg fp_neg; // mandatory
    bmath_fp_sub fp_sub; // optional

} bmath_abelian_group_s;

BCORE_DEFINE_SPECT_GET_TYPED( bmath_abelian_group_s )
BCORE_DEFINE_SPECT_GET_AWARE( bmath_abelian_group_s )

void bmath_abelian_group_spect_add( const bmath_abelian_group_s* p, vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_spect_zro( const bmath_abelian_group_s* p, vd_t o );
void bmath_abelian_group_spect_neg( const bmath_abelian_group_s* p, vd_t o, vc_t op );
void bmath_abelian_group_spect_sub( const bmath_abelian_group_s* p, vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_spect_cpy( const bmath_abelian_group_s* p, vd_t o, vc_t op ); // copy

void bmath_abelian_group_typed_add( tp_t t, vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_typed_zro( tp_t t, vd_t o );
void bmath_abelian_group_typed_neg( tp_t t, vd_t o, vc_t op );
void bmath_abelian_group_typed_sub( tp_t t, vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_typed_cpy( tp_t t, vd_t o, vc_t op );

void bmath_abelian_group_aware_add( vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_aware_zro( vd_t o );
void bmath_abelian_group_aware_neg( vd_t o, vc_t op );
void bmath_abelian_group_aware_sub( vd_t o, vc_t op1, vc_t op2 );
void bmath_abelian_group_aware_cpy( vd_t o, vc_t op );

void bmath_abelian_group_add( sr_s o, vc_t op1, vc_t op2 );
void bmath_abelian_group_zro( sr_s o );
void bmath_abelian_group_neg( sr_s o, vc_t op );
void bmath_abelian_group_sub( sr_s o, vc_t op1, vc_t op2 );
void bmath_abelian_group_cpy( sr_s o, vc_t op );

void bmath_abelian_group_q_add( const sr_s* o, vc_t op1, vc_t op2 );
void bmath_abelian_group_q_zro( const sr_s* o );
void bmath_abelian_group_q_neg( const sr_s* o, vc_t op );
void bmath_abelian_group_q_sub( const sr_s* o, vc_t op1, vc_t op2 );
void bmath_abelian_group_q_cpy( const sr_s* o, vc_t op );

/**********************************************************************************************************************/

// bmath_ring_s
typedef struct bmath_ring_s
{
    bcore_spect_header_s header;
    const bcore_inst_s* spect_inst;

    bmath_fp_add fp_add; // mandatory
    bmath_fp_zro fp_zro; // mandatory
    bmath_fp_neg fp_neg; // mandatory
    bmath_fp_sub fp_sub; // optional

    bmath_fp_mul fp_mul; // mandatory
    bmath_fp_one fp_one; // optional
    bmath_fp_inv fp_inv; // optional
    bmath_fp_div fp_div; // optional

} bmath_ring_s;

BCORE_DEFINE_SPECT_GET_TYPED( bmath_ring_s )
BCORE_DEFINE_SPECT_GET_AWARE( bmath_ring_s )

void bmath_ring_spect_add( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 );
void bmath_ring_spect_zro( const bmath_ring_s* p, vd_t o );
void bmath_ring_spect_neg( const bmath_ring_s* p, vd_t o, vc_t op );
void bmath_ring_spect_sub( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 );
void bmath_ring_spect_cpy( const bmath_ring_s* p, vd_t o, vc_t op ); // copy
void bmath_ring_spect_mul( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 );
void bmath_ring_spect_one( const bmath_ring_s* p, vd_t o );
void bmath_ring_spect_inv( const bmath_ring_s* p, vd_t o, vc_t op );
void bmath_ring_spect_div( const bmath_ring_s* p, vd_t o, vc_t op1, vc_t op2 );

void bmath_ring_typed_add( tp_t t, vd_t o, vc_t op1, vc_t op2 );
void bmath_ring_typed_zro( tp_t t, vd_t o );
void bmath_ring_typed_neg( tp_t t, vd_t o, vc_t op );
void bmath_ring_typed_sub( tp_t t, vd_t o, vc_t op1, vc_t op2 );
void bmath_ring_typed_cpy( tp_t t, vd_t o, vc_t op );
void bmath_ring_typed_mul( tp_t t, vd_t o, vc_t op1, vc_t op2 );
void bmath_ring_typed_one( tp_t t, vd_t o );
void bmath_ring_typed_inv( tp_t t, vd_t o, vc_t op );
void bmath_ring_typed_div( tp_t t, vd_t o, vc_t op1, vc_t op2 );

void bmath_ring_aware_add( vd_t o, vc_t op1, vc_t op2 );
void bmath_ring_aware_zro( vd_t o );
void bmath_ring_aware_neg( vd_t o, vc_t op );
void bmath_ring_aware_sub( vd_t o, vc_t op1, vc_t op2 );
void bmath_ring_aware_cpy( vd_t o, vc_t op );
void bmath_ring_aware_mul( vd_t o, vc_t op1, vc_t op2 );
void bmath_ring_aware_one( vd_t o );
void bmath_ring_aware_inv( vd_t o, vc_t op );
void bmath_ring_aware_div( vd_t o, vc_t op1, vc_t op2 );

void bmath_ring_add( sr_s o, vc_t op1, vc_t op2 );
void bmath_ring_zro( sr_s o );
void bmath_ring_neg( sr_s o, vc_t op );
void bmath_ring_sub( sr_s o, vc_t op1, vc_t op2 );
void bmath_ring_cpy( sr_s o, vc_t op );
void bmath_ring_mul( sr_s o, vc_t op1, vc_t op2 );
void bmath_ring_one( sr_s o );
void bmath_ring_inv( sr_s o, vc_t op );
void bmath_ring_div( sr_s o, vc_t op1, vc_t op2 );

void bmath_ring_q_add( const sr_s* o, vc_t op1, vc_t op2 );
void bmath_ring_q_zro( const sr_s* o );
void bmath_ring_q_neg( const sr_s* o, vc_t op );
void bmath_ring_q_sub( const sr_s* o, vc_t op1, vc_t op2 );
void bmath_ring_q_cpy( const sr_s* o, vc_t op );
void bmath_ring_q_mul( const sr_s* o, vc_t op1, vc_t op2 );
void bmath_ring_q_one( const sr_s* o );
void bmath_ring_q_inv( const sr_s* o, vc_t op );
void bmath_ring_q_div( const sr_s* o, vc_t op1, vc_t op2 );

/**********************************************************************************************************************/

vd_t bmath_spect_algebraic_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_ALGEBRAIC_H

