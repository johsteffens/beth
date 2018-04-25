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

#ifndef BMATH_VECTOR_H
#define BMATH_VECTOR_H

/**********************************************************************************************************************/

/// Collection of vectors.

/** Nomenclature
 *  add, zro, neg, sub, mul, dot_prd, sqr, sqr_sub - according to spect_vector
 *  cnj: conjugation
 *  cpy: projecting or injecting copy
 *  sum: sum of elements
 *  avg: average
 *  var: (std-)variance
 *  dev: (std-)deviation
 *  dft: discrete fourier transform  (fft used when size is power of 2)
 *  ift: inverse dft
 */
/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_spect_algebraic.h"
#include "bmath_complex.h"

/**********************************************************************************************************************/
// dynamic size vector of f3_t

BCORE_DECLARE_OBJECT_( bmath_vf3_s )
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            f3_t* data;
            sz_t  size;
            sz_t  space;
        };
    };
};

bmath_vf3_s* bmath_vf3_s_create_size( sz_t size );
bmath_vf3_s* bmath_vf3_s_create_fill( f3_t val, sz_t size );

void bmath_vf3_s_zro(     bmath_vf3_s* o );
void bmath_vf3_s_neg(     const bmath_vf3_s* o, bmath_vf3_s* res );
void bmath_vf3_s_cpy(     const bmath_vf3_s* o, bmath_vf3_s* res );
void bmath_vf3_s_add(     const bmath_vf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_vf3_s_sub(     const bmath_vf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_vf3_s_mul(     const bmath_vf3_s* o, const f3_t* op,        bmath_vf3_s* res  );
void bmath_vf3_s_mul_f3(  const bmath_vf3_s* o, f3_t scl2, bmath_vf3_s* res );
f3_t bmath_vf3_s_f3_dot_prd( const bmath_vf3_s* o, const bmath_vf3_s* vec2 );
f3_t bmath_vf3_s_f3_sqr(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_sub_sqr( const bmath_vf3_s* o, const bmath_vf3_s* vec2 );
f3_t bmath_vf3_s_f3_sum(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_avg(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_var(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_dev(     const bmath_vf3_s* o );
void bmath_vf3_s_dot_prd( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t* res );
void bmath_vf3_s_sqr(     const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_sub_sqr( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t* res );
void bmath_vf3_s_sum(     const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_avg(     const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_var(     const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_dev(     const bmath_vf3_s* o, f3_t* res  );

/**********************************************************************************************************************/
// dynamic size vector of bmath_cf3_s

BCORE_DECLARE_OBJECT_( bmath_vcf3_s )
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            bmath_cf3_s* data;
            sz_t  size;
            sz_t  space;
        };
    };
};

bmath_vcf3_s* bmath_vcf3_s_create_size( sz_t size );
bmath_vcf3_s* bmath_vcf3_s_create_fill( bmath_cf3_s val, sz_t size );

void bmath_vcf3_s_zro(     bmath_vcf3_s* o );
void bmath_vcf3_s_neg(     const bmath_vcf3_s* o, bmath_vcf3_s* res );
void bmath_vcf3_s_cnj(     const bmath_vcf3_s* o, bmath_vcf3_s* res );
void bmath_vcf3_s_cpy(     const bmath_vcf3_s* o, bmath_vcf3_s* res );
void bmath_vcf3_s_add(     const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_vcf3_s* res );
void bmath_vcf3_s_sub(     const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_vcf3_s* res );
void bmath_vcf3_s_mul(     const bmath_vcf3_s* o, const bmath_cf3_s*  op, bmath_vcf3_s* res );
void bmath_vcf3_s_mul_cf3( const bmath_vcf3_s* o, const bmath_cf3_s   op, bmath_vcf3_s* res );
void bmath_vcf3_s_mul_f3(  const bmath_vcf3_s* o,              f3_t   op, bmath_vcf3_s* res );
void bmath_vcf3_s_dft(     const bmath_vcf3_s* o, bmath_vcf3_s* res );
void bmath_vcf3_s_ift(     const bmath_vcf3_s* o, bmath_vcf3_s* res );
void bmath_vcf3_s_dot_prd( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_cf3_s* res );
void bmath_vcf3_s_sqr(     const bmath_vcf3_s* o, bmath_cf3_s* res );
void bmath_vcf3_s_sub_sqr( const bmath_vcf3_s* o, const bmath_vcf3_s* op, bmath_cf3_s* res );
void bmath_vcf3_s_sum(     const bmath_vcf3_s* o, bmath_cf3_s* res );
void bmath_vcf3_s_avg(     const bmath_vcf3_s* o, bmath_cf3_s* res );

/**********************************************************************************************************************/

vd_t bmath_vector_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_VECTOR_H
