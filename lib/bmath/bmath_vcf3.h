/** Author and Copyright 2018 Johannes Bernhard Steffens
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

#ifndef BMATH_VCF3_H
#define BMATH_VCF3_H

/**********************************************************************************************************************/

/// Collection of vectors.

/** Nomenclature
 *  add, zro, neg, sub, mul, mul_vec, sqr, sqr_sub - according to spect_vector
 *  mul_hdm: C = A o B  (hadamard product == elementwise product)
 *  cnj: conjugation
 *  cpy: copy
 *  cat: catenation
 *  sum: sum of elements
 *  avg: average
 *  var: (std-)variance
 *  dev: (std-)deviation
 *  dft: discrete fourier transform  (fft used when size is power of 2)
 *  ift: inverse dft
 *  pmt: permutation
 */
/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_complex.h"
#include "bmath_vf3.h"

/**********************************************************************************************************************/
// dynamic size vector of bmath_cf3_s

BCORE_DECLARE_OBJECT( bmath_vcf3_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_cf3_s, );
};

void bmath_vcf3_s_move( bmath_vcf3_s* o, bmath_vcf3_s* src );

void bmath_vcf3_s_set_size( bmath_vcf3_s* o, uz_t size );
void bmath_vcf3_s_push(     bmath_vcf3_s* o, bmath_cf3_s cf3 );
void bmath_vcf3_s_push_ri(  bmath_vcf3_s* o, f3_t r, f3_t i );

bmath_vcf3_s* bmath_vcf3_s_create_size( uz_t size );
bmath_vcf3_s* bmath_vcf3_s_create_fill( bmath_cf3_s val, uz_t size );

void bmath_vcf3_s_zro(           bmath_vcf3_s* o );
void bmath_vcf3_s_neg(     const bmath_vcf3_s* o, bmath_vcf3_s* res );
void bmath_vcf3_s_cnj(     const bmath_vcf3_s* o, bmath_vcf3_s* res );
void bmath_vcf3_s_cpy(     const bmath_vcf3_s* o, bmath_vcf3_s* res );
void bmath_vcf3_s_add(     const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_vcf3_s* res );
void bmath_vcf3_s_sub(     const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_vcf3_s* res );
void bmath_vcf3_s_mul_scl( const bmath_vcf3_s* o, const bmath_cf3_s*  op, bmath_vcf3_s* res );
void bmath_vcf3_s_mul_cf3( const bmath_vcf3_s* o, const bmath_cf3_s   op, bmath_vcf3_s* res );
void bmath_vcf3_s_mul_f3(  const bmath_vcf3_s* o,              f3_t   op, bmath_vcf3_s* res );
void bmath_vcf3_s_dft(     const bmath_vcf3_s* o, bmath_vcf3_s* res );
void bmath_vcf3_s_ift(     const bmath_vcf3_s* o, bmath_vcf3_s* res );
void bmath_vcf3_s_mul_vec( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_cf3_s* res );
void bmath_vcf3_s_sqr(     const bmath_vcf3_s* o, bmath_cf3_s* res );
void bmath_vcf3_s_sub_sqr( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_cf3_s* res );
void bmath_vcf3_s_sum(     const bmath_vcf3_s* o, bmath_cf3_s* res );
void bmath_vcf3_s_avg(     const bmath_vcf3_s* o, bmath_cf3_s* res );

static inline
void bmath_vcf3_s_set_cf3( bmath_vcf3_s* o, uz_t index, bmath_cf3_s v )
{
    assert( index < o->size );
    o->data[ index ] = v;
}

static inline
bmath_cf3_s bmath_vcf3_s_get_cf3( const bmath_vcf3_s* o, uz_t index )
{
    assert( index < o->size );
    return o->data[ index ];
}

/// For easy inspection
void bmath_vcf3_s_to_stdout( const bmath_vcf3_s* o );

/**********************************************************************************************************************/

vd_t bmath_vcf3_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_VCF3_H
