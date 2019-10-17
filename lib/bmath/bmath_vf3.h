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

#ifndef BMATH_VF3_H
#define BMATH_VF3_H

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
#include "bmath_hwflags.h"
#include "bmath_spect_algebraic.h"
#include "bmath_complex.h"
#include "bmath_pmt.h"

/**********************************************************************************************************************/
// low level vector functions (partially AVX optimized)

f3_t bmath_f3_t_vec_sum(          const f3_t* v1, sz_t size ); // sum_i v1[i]
f3_t bmath_f3_t_vec_sum_esp(      const f3_t* v1, sz_t size ); // sum_i v1[i]
f3_t bmath_f3_t_vec_mul_vec(      const f3_t* v1, const f3_t* v2, sz_t size ); // sum_i v1[i]*v2[i]
f3_t bmath_f3_t_vec_mul_vec_esp(  const f3_t* v1, const f3_t* v2, sz_t size ); // sum_i v1[i]*v2[i]
f3_t bmath_f3_t_vec_mul3_vec(     const f3_t* v1, const f3_t* v2, const f3_t* v3, sz_t size ); // sum_i v1[i]*v2[i]*v3[i]
f3_t bmath_f3_t_vec_mul3_vec_esp( const f3_t* v1, const f3_t* v2, const f3_t* v3, sz_t size ); // sum_i v1[i]*v2[i]*v3[i]

/// with stride for both vectors
f3_t bmath_f3_t_vec_mul_vec_stride(     const f3_t* v1, sz_t stride1, const f3_t* v2, sz_t stride2, sz_t size ); // sum_i v1[i]*v2[i]
f3_t bmath_f3_t_vec_mul_vec_esp_stride( const f3_t* v1, sz_t stride1, const f3_t* v2, sz_t stride2, sz_t size ); // sum_i v1[i]*v2[i]

void bmath_f3_t_vec_mul_scl(     const f3_t* a, f3_t b,                f3_t* r, sz_t size ); // a * b + c -> r
void bmath_f3_t_vec_mul_scl_add( const f3_t* a, f3_t b, const f3_t* c, f3_t* r, sz_t size ); // a * b + c -> r

/**********************************************************************************************************************/
// dynamic size vector of f3_t

BCORE_DECLARE_OBJECT( bmath_vf3_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, );
};

static inline void bmath_vf3_s_init_weak( bmath_vf3_s* o, f3_t* data, sz_t size ) { bmath_vf3_s_init( o ); o->data = data; o->size = size; }
static inline bmath_vf3_s bmath_vf3_init_weak( f3_t* data, sz_t size ) { bmath_vf3_s o; bmath_vf3_s_init_weak( &o, data, size ); return o; }

void bmath_vf3_s_move( bmath_vf3_s* o, bmath_vf3_s* src );
void bmath_vf3_s_clear( bmath_vf3_s* o );

bmath_vf3_s* bmath_vf3_s_create_set_size( uz_t size );
bmath_vf3_s* bmath_vf3_s_create_fill( uz_t size, f3_t val );

void bmath_vf3_s_set_size(    bmath_vf3_s* o, uz_t size );
void bmath_vf3_s_set_space(   bmath_vf3_s* o, uz_t space );
void bmath_vf3_s_set_size_to( const bmath_vf3_s* o, bmath_vf3_s* res );
void bmath_vf3_s_fill(        bmath_vf3_s* o, f3_t val );

/// copies external data into vector
void bmath_vf3_s_set_data( bmath_vf3_s* o, const f3_t* data, sz_t size );

/** Sets all vector elements to random values.
 *  Random generator:
 *    Parameters density, min, max, p_rval apply to the random generator.
 *      rval: Pointer to running variable of random generator.
 *            If NULL, an internal fixed random seed is used.
 *
 *     density (range [0.0, 1.0]) specifies the rate at which the random generator
 *     creates a non-zero value.
 */
void bmath_vf3_s_set_random( bmath_vf3_s* o, f3_t density, f3_t min, f3_t max, u2_t* p_rval );

/// swaps vectors by swapping data references (no copying of memory)
void bmath_vf3_s_swapr( bmath_vf3_s* o, bmath_vf3_s* v );

void bmath_vf3_s_push( bmath_vf3_s* o, f3_t val );
void bmath_vf3_s_push_vf3( bmath_vf3_s* o, const bmath_vf3_s* vec );

bl_t bmath_vf3_s_is_near_equ( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t max_dev );
bl_t bmath_vf3_s_is_near_zro( const bmath_vf3_s* o, f3_t max_dev );

static inline bl_t bmath_vf3_s_is_equ( const bmath_vf3_s* o, const bmath_vf3_s* op ) { return bmath_vf3_s_is_near_equ( o, op, 0 ); }
static inline bl_t bmath_vf3_s_is_zro( const bmath_vf3_s* o ) { return bmath_vf3_s_is_near_zro( o, 0 ); }

/// vector is nan if at least one element is nan
bl_t bmath_vf3_s_is_nan( const bmath_vf3_s* o );

void bmath_vf3_s_zro( bmath_vf3_s* o );
void bmath_vf3_s_neg( const bmath_vf3_s* o, bmath_vf3_s* res );
void bmath_vf3_s_cpy( const bmath_vf3_s* o, bmath_vf3_s* res );
void bmath_vf3_s_cat( const bmath_vf3_s* a, const bmath_vf3_s* b,  bmath_vf3_s* r );
void bmath_vf3_s_add( const bmath_vf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );
void bmath_vf3_s_sub( const bmath_vf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res );

/// adds the same scalar to all components
void bmath_vf3_s_add_scl( const bmath_vf3_s* o, const f3_t* op, bmath_vf3_s* res );
void bmath_vf3_s_sub_scl( const bmath_vf3_s* o, const f3_t* op, bmath_vf3_s* res );
void bmath_vf3_s_add_scl_f3( const bmath_vf3_s* o, f3_t op, bmath_vf3_s* res );
void bmath_vf3_s_sub_scl_f3( const bmath_vf3_s* o, f3_t op, bmath_vf3_s* res );

/// hadamard product: a o b [+ c] -> r  (elementwise)
void bmath_vf3_s_mul_hdm(     const bmath_vf3_s* a, const bmath_vf3_s* b,                       bmath_vf3_s* r );
void bmath_vf3_s_mul_hdm_add( const bmath_vf3_s* a, const bmath_vf3_s* b, const bmath_vf3_s* c, bmath_vf3_s* r );

void bmath_vf3_s_pmt_mul(     const bmath_vf3_s* o, const bmath_pmt_s* p, bmath_vf3_s* res ); // y = P   * x ; y_p[k] = x_k
void bmath_vf3_s_pmt_htp_mul( const bmath_vf3_s* o, const bmath_pmt_s* p, bmath_vf3_s* res ); // y = P^T * x ; y_k = x_p[k]

void bmath_vf3_s_mul_scl_f3(     const bmath_vf3_s* o, f3_t s,                       bmath_vf3_s* r ); // o * b     --> r
void bmath_vf3_s_mul_scl_f3_add( const bmath_vf3_s* o, f3_t s, const bmath_vf3_s* a, bmath_vf3_s* r ); // o * b + a --> r

void bmath_vf3_s_mul_scl(    const bmath_vf3_s* o, const f3_t* op,        bmath_vf3_s* res  );
void bmath_vf3_s_mul_f3(     const bmath_vf3_s* o, f3_t scl2, bmath_vf3_s* res ); // same as bmath_vf3_s_mul_scl_f3
f3_t bmath_vf3_s_f3_mul_vec( const bmath_vf3_s* o, const bmath_vf3_s* vec2 );
f3_t bmath_vf3_s_f3_sqr(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_sub_sqr( const bmath_vf3_s* o, const bmath_vf3_s* vec2 );
f3_t bmath_vf3_s_f3_max(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_min(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_sum(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_avg(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_var(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_f3_dev(     const bmath_vf3_s* o );
void bmath_vf3_s_mul_vec(    const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t* res ); // dot product
void bmath_vf3_s_sqr(        const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_sub_sqr(    const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t* res );
void bmath_vf3_s_sum(        const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_avg(        const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_var(        const bmath_vf3_s* o, f3_t* res  );
void bmath_vf3_s_dev(        const bmath_vf3_s* o, f3_t* res  );

void bmath_vf3_s_set_sqr( bmath_vf3_s* o, f3_t val ); // multiplies a factor to all components such that bmath_vf3_s_f3_sqr returns val
void bmath_vf3_s_set_sum( bmath_vf3_s* o, f3_t val ); // adds a value to all components such that bmath_vf3_s_f3_sum returns val
void bmath_vf3_s_set_avg( bmath_vf3_s* o, f3_t val ); // adds a value to all components such that bmath_vf3_s_f3_avg returns val

static inline
void bmath_vf3_s_set_f3( bmath_vf3_s* o, uz_t index, f3_t v )
{
    assert( index < o->size );
    o->data[ index ] = v;
}

static inline
f3_t bmath_vf3_s_get_f3( const bmath_vf3_s* o, uz_t index )
{
    assert( index < o->size );
    return o->data[ index ];
}

static inline
bmath_vf3_s bmath_vf3_s_get_weak_sub_vec( const bmath_vf3_s* o, uz_t index, uz_t size )
{
    assert( o->size >= index + size );
    return ( bmath_vf3_s ) { .size = size, .space = 0, .data = o->data + index };
}

f3_t bmath_vf3_s_max(     const bmath_vf3_s* o );
f3_t bmath_vf3_s_min(     const bmath_vf3_s* o );
uz_t bmath_vf3_s_idx_max( const bmath_vf3_s* o );
uz_t bmath_vf3_s_idx_min( const bmath_vf3_s* o );

//----------------------------------------------------------------------------------------------------------------------
// vector * unary_map --> vector

void bmath_vf3_s_eop_map(     const bmath_vf3_s* o, bmath_fp_f3_ar1 b, bmath_vf3_s* r ); // r_i = b( o_i )
void bmath_vf3_s_eop_map_mul( const bmath_vf3_s* o, bmath_fp_f3_ar1 b, const bmath_vf3_s* c, bmath_vf3_s* r ); // r_i = b( o_i ) * c_i

//----------------------------------------------------------------------------------------------------------------------
// For easy inspection
void bmath_vf3_s_to_sink( const bmath_vf3_s* o, bcore_sink* sink );
static inline void bmath_vf3_s_to_string( const bmath_vf3_s* o, st_s* string ) { bmath_vf3_s_to_sink( o, ( bcore_sink* )string ); }
static inline void bmath_vf3_s_to_stdout( const bmath_vf3_s* o               ) { bmath_vf3_s_to_sink( o, BCORE_STDOUT ); }

void bmath_vf3_s_stat_to_sink( const bmath_vf3_s* o, bcore_sink* sink ); // vector statistics
static inline void bmath_vf3_s_stat_to_string( const bmath_vf3_s* o, st_s* string ) { bmath_vf3_s_stat_to_sink( o, ( bcore_sink* )string ); }
static inline void bmath_vf3_s_stat_to_stdout( const bmath_vf3_s* o               ) { bmath_vf3_s_stat_to_sink( o, BCORE_STDOUT ); }

/**********************************************************************************************************************/

vd_t bmath_vf3_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_VF3_H
