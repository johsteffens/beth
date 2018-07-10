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
 *  add, zro, neg, sub, mul, mul_vec, sqr, sqr_sub - according to spect_vector
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

BCORE_DECLARE_OBJECT( bmath_vf3_s )
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

void bmath_vf3_s_move( bmath_vf3_s* o, bmath_vf3_s* src );
void bmath_vf3_s_clear( bmath_vf3_s* o );

bmath_vf3_s* bmath_vf3_s_create_set_size( sz_t size );
bmath_vf3_s* bmath_vf3_s_create_fill( sz_t size, f3_t val );
bmath_vf3_s* bmath_vf3_s_create_fill_random( sz_t size, f3_t min, f3_t max, u2_t* rval );

void bmath_vf3_s_set_size(    bmath_vf3_s* o, sz_t size );
void bmath_vf3_s_set_size_to( const bmath_vf3_s* o, bmath_vf3_s* res );
void bmath_vf3_s_fill(        bmath_vf3_s* o, f3_t val );
void bmath_vf3_s_fill_random( bmath_vf3_s* o, f3_t min, f3_t max, u2_t* rval );

void bmath_vf3_s_push( bmath_vf3_s* o, f3_t val );
void bmath_vf3_s_push_vf3( bmath_vf3_s* o, const bmath_vf3_s* vec );

bl_t bmath_vf3_s_is_equ( const bmath_vf3_s* o, const bmath_vf3_s* op );
bl_t bmath_vf3_s_is_zro( const bmath_vf3_s* o );
bl_t bmath_vf3_s_is_near_equ( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t max_dev );
bl_t bmath_vf3_s_is_near_zro( const bmath_vf3_s* o, f3_t max_dev );

void bmath_vf3_s_zro(     bmath_vf3_s* o );
void bmath_vf3_s_neg(     const bmath_vf3_s* o, bmath_vf3_s* res );
void bmath_vf3_s_cpy(     const bmath_vf3_s* o, bmath_vf3_s* res );
void bmath_vf3_s_add(     const bmath_vf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_vf3_s_sub(     const bmath_vf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_vf3_s_mul_scl( const bmath_vf3_s* o, const f3_t* op,        bmath_vf3_s* res  );
void bmath_vf3_s_mul_f3(  const bmath_vf3_s* o, f3_t scl2, bmath_vf3_s* res );
f3_t bmath_vf3_s_f3_mul_vec( const bmath_vf3_s* o, const bmath_vf3_s* vec2 );
f3_t bmath_vf3_s_f3_sqr(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_sub_sqr( const bmath_vf3_s* o, const bmath_vf3_s* vec2 );
f3_t bmath_vf3_s_f3_sum(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_avg(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_var(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_dev(     const bmath_vf3_s* o );
void bmath_vf3_s_mul_vec( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t* res );
void bmath_vf3_s_sqr(     const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_sub_sqr( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t* res );
void bmath_vf3_s_sum(     const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_avg(     const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_var(     const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_dev(     const bmath_vf3_s* o, f3_t* res  );

void bmath_vf3_s_set_sqr( bmath_vf3_s* o, f3_t val ); // multiplies a factor to all components such that bmath_vf3_s_f3_sqr returns val
void bmath_vf3_s_set_sum( bmath_vf3_s* o, f3_t val ); // adds a value to all components such that bmath_vf3_s_f3_sum returns val
void bmath_vf3_s_set_avg( bmath_vf3_s* o, f3_t val ); // adds a value to all components such that bmath_vf3_s_f3_avg returns val

static inline
void bmath_vf3_s_set_f3( bmath_vf3_s* o, sz_t index, f3_t v )
{
    assert( index < o->size );
    o->data[ index ] = v;
}

static inline
f3_t bmath_vf3_s_get_f3( const bmath_vf3_s* o, sz_t index )
{
    assert( index < o->size );
    return o->data[ index ];
}

f3_t bmath_vf3_s_max(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_min(     const bmath_vf3_s* o );
sz_t bmath_vf3_s_idx_max( const bmath_vf3_s* o );
sz_t bmath_vf3_s_idx_min( const bmath_vf3_s* o );

/// For easy inspection
void bmath_vf3_s_to_stdout( const bmath_vf3_s* o );
void bmath_vf3_s_stat_to_stdout( const bmath_vf3_s* o ); // vector statistics

/**********************************************************************************************************************/
// dynamic size vector of bmath_cf3_s

BCORE_DECLARE_OBJECT( bmath_vcf3_s )
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

void bmath_vcf3_s_move( bmath_vcf3_s* o, bmath_vcf3_s* src );

void bmath_vcf3_s_set_size( bmath_vcf3_s* o, sz_t size );
bmath_vcf3_s* bmath_vcf3_s_create_size( sz_t size );
bmath_vcf3_s* bmath_vcf3_s_create_fill( bmath_cf3_s val, sz_t size );

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
void bmath_vcf3_s_set_cf3( bmath_vcf3_s* o, sz_t index, bmath_cf3_s v )
{
    assert( index < o->size );
    o->data[ index ] = v;
}

static inline
bmath_cf3_s bmath_vcf3_s_get_cf3( const bmath_vcf3_s* o, sz_t index )
{
    assert( index < o->size );
    return o->data[ index ];
}

/// For easy inspection
void bmath_vcf3_s_to_stdout( const bmath_vcf3_s* o );

/**********************************************************************************************************************/
// array of vf3_s

BCORE_DECLARE_OBJECT( bmath_arr_vf3_s )
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            bmath_vf3_s* data;
            sz_t  size;
            sz_t  space;
        };
    };
};

void bmath_arr_vf3_s_clear(      bmath_arr_vf3_s* o ); // sets size to zero
void bmath_arr_vf3_s_set_space(  bmath_arr_vf3_s* o, sz_t space );
void bmath_arr_vf3_s_set_size(   bmath_arr_vf3_s* o, sz_t size );
void bmath_arr_vf3_s_push(       bmath_arr_vf3_s* o, const bmath_vf3_s* vec );
void bmath_arr_vf3_s_push_arr(   bmath_arr_vf3_s* o, const bmath_arr_vf3_s* arr );
void bmath_arr_vf3_s_pop(        bmath_arr_vf3_s* o );
void bmath_arr_vf3_s_reorder(    bmath_arr_vf3_s* o, const bcore_arr_sz_s* order );

/** Following functions apply a bmath_vf3_s operation on consecutive elements indexed start, ..., end-1.
 *  start, end are automatically limited to o->size;
 *  Where res is in use, the same index is applied to o and res.
 *     Requires bounded( end ) <= res->size (tested).
 *     o == res allowed (except where stated otherwise)
 *  end == -1 assumes end == o->size
 */

void bmath_arr_vf3_s_on_section_set_size(      bmath_arr_vf3_s* o, sz_t start, sz_t end, sz_t size );
void bmath_arr_vf3_s_on_section_fill(          bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t val );
void bmath_arr_vf3_s_on_section_fill_random(   bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t min, f3_t max, u2_t* rval );
void bmath_arr_vf3_s_on_section_zro(           bmath_arr_vf3_s* o, sz_t start, sz_t end );
void bmath_arr_vf3_s_on_section_neg(     const bmath_arr_vf3_s* o, sz_t start, sz_t end,                        bmath_arr_vf3_s* res );
void bmath_arr_vf3_s_on_section_cpy(     const bmath_arr_vf3_s* o, sz_t start, sz_t end,                        bmath_arr_vf3_s* res );
void bmath_arr_vf3_s_on_section_add(     const bmath_arr_vf3_s* o, sz_t start, sz_t end, const bmath_vf3_s* op, bmath_arr_vf3_s* res );
void bmath_arr_vf3_s_on_section_sub(     const bmath_arr_vf3_s* o, sz_t start, sz_t end, const bmath_vf3_s* op, bmath_arr_vf3_s* res );
void bmath_arr_vf3_s_on_section_mul_f3(  const bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t op,               bmath_arr_vf3_s* res );
void bmath_arr_vf3_s_on_section_set_sqr(       bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t val );
void bmath_arr_vf3_s_on_section_set_sum(       bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t val );
void bmath_arr_vf3_s_on_section_set_avg(       bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t val );
f3_t bmath_arr_vf3_s_on_section_f3_sum_sprec( const bmath_arr_vf3_s* o, sz_t start, sz_t end, sz_t index ); // stochastically precise summing of indexed element

// stochastically precise summing of co-product (vi - ai) * (vj - aj)
f3_t bmath_arr_vf3_s_on_section_f3_sum_coprd_sprec( const bmath_arr_vf3_s* o, sz_t start, sz_t end, f3_t ai, f3_t aj, sz_t i, sz_t j );

void bmath_arr_vf3_s_on_section_get_sum_sprc( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_vf3_s* res ); // stochastically precise summing
void bmath_arr_vf3_s_on_section_get_avg_sprc( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_vf3_s* res ); // stochastically precise averaging
void bmath_arr_vf3_s_on_section_get_sum_fast( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_vf3_s* res ); // fast summing   (loss of accuracy possible)
void bmath_arr_vf3_s_on_section_get_avg_fast( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_vf3_s* res ); // fast averaging (loss of accuracy possible)

static inline void bmath_arr_vf3_s_on_section_get_sum( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_vf3_s* res ) { bmath_arr_vf3_s_on_section_get_sum_sprc( o, start, end, res ); }
static inline void bmath_arr_vf3_s_on_section_get_avg( const bmath_arr_vf3_s* o, sz_t start, sz_t end, bmath_vf3_s* res ) { bmath_arr_vf3_s_on_section_get_avg_sprc( o, start, end, res ); }

/**********************************************************************************************************************/

vd_t bmath_vector_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_VECTOR_H
