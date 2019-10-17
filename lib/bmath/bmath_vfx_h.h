/** Copyright 2019 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

/** Vector Template.
 *
 *  Include in the final header file after object definition
 *  with macro BFPREC_LEVEL (temporarily) defined.
 *
 *  Example:
 *
 *  #define BFPREC_LEVEL 2 // defines all functions for f2_t
 *  #include "bmath_vfx_h.h"
 *
 *  #define BFPREC_LEVEL 3 // defines all functions for f3_t
 *  #include "bmath_vfx_h.h"
 */

/**********************************************************************************************************************/
// Template macros

#ifndef BFPREC_LEVEL
    #error "Macro 'BFPREC_LEVEL' is not defined."
#endif // fx_t

#if BFPREC_LEVEL == 2

    #define fx_t                    f2_t
    #define bmath_vfx_s             bmath_vf2_s
    #define bmath_vfx_(      name ) bmath_vf2_##name
    #define bmath_fx_t_vec_( name ) bmath_f2_t_vec_##name
    #define bmath_fp_fx_(    name ) bmath_fp_f2_##name

#elif BFPREC_LEVEL == 3

    #define fx_t                    f3_t
    #define bmath_vfx_s             bmath_vf3_s
    #define bmath_vfx_(      name ) bmath_vf3_##name
    #define bmath_fx_t_vec_( name ) bmath_f3_t_vec_##name
    #define bmath_fp_fx_(    name ) bmath_fp_f3_##name

#else

    #error "Macro BFPREC_LEVEL has wrong value! (Only '2' or '3' are supported)"

#endif // BFPREC_LEVEL

/**********************************************************************************************************************/

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
#include "bmath_pmt.h"

/**********************************************************************************************************************/
// low level vector functions (partially AVX optimized)

fx_t bmath_fx_t_vec_(sum)(          const fx_t* v1, sz_t size ); // sum_i v1[i]
fx_t bmath_fx_t_vec_(sum_esp)(      const fx_t* v1, sz_t size ); // sum_i v1[i]
fx_t bmath_fx_t_vec_(mul_vec)(      const fx_t* v1, const fx_t* v2, sz_t size ); // sum_i v1[i]*v2[i]
fx_t bmath_fx_t_vec_(mul_vec_esp)(  const fx_t* v1, const fx_t* v2, sz_t size ); // sum_i v1[i]*v2[i]
fx_t bmath_fx_t_vec_(mul3_vec)(     const fx_t* v1, const fx_t* v2, const fx_t* v3, sz_t size ); // sum_i v1[i]*v2[i]*v3[i]
fx_t bmath_fx_t_vec_(mul3_vec_esp)( const fx_t* v1, const fx_t* v2, const fx_t* v3, sz_t size ); // sum_i v1[i]*v2[i]*v3[i]

/// with stride for both vectors
fx_t bmath_fx_t_vec_(mul_vec_stride)(     const fx_t* v1, sz_t stride1, const fx_t* v2, sz_t stride2, sz_t size ); // sum_i v1[i]*v2[i]
fx_t bmath_fx_t_vec_(mul_vec_esp_stride)( const fx_t* v1, sz_t stride1, const fx_t* v2, sz_t stride2, sz_t size ); // sum_i v1[i]*v2[i]

void bmath_fx_t_vec_(mul_scl)(     const fx_t* a, fx_t b,                fx_t* r, sz_t size ); // a * b + c -> r
void bmath_fx_t_vec_(mul_scl_add)( const fx_t* a, fx_t b, const fx_t* c, fx_t* r, sz_t size ); // a * b + c -> r

/**********************************************************************************************************************/
// vector object functions

static inline void bmath_vfx_(s_init_weak)( bmath_vfx_s* o, fx_t* data, sz_t size ) { bmath_vfx_(s_init)( o ); o->data = data; o->size = size; }
static inline bmath_vfx_s bmath_vfx_(init_weak)( fx_t* data, sz_t size ) { bmath_vfx_s o; bmath_vfx_(s_init_weak)( &o, data, size ); return o; }

void bmath_vfx_(s_move)( bmath_vfx_s* o, bmath_vfx_s* src );
void bmath_vfx_(s_clear)( bmath_vfx_s* o );

bmath_vfx_s* bmath_vfx_(s_create_set_size)( uz_t size );
bmath_vfx_s* bmath_vfx_(s_create_fill)( uz_t size, fx_t val );

void bmath_vfx_(s_set_size)(    bmath_vfx_s* o, uz_t size );
void bmath_vfx_(s_set_space)(   bmath_vfx_s* o, uz_t space );
void bmath_vfx_(s_set_size_to)( const bmath_vfx_s* o, bmath_vfx_s* res );
void bmath_vfx_(s_fill)(        bmath_vfx_s* o, fx_t val );

/// copies external data into vector
void bmath_vfx_(s_set_data)( bmath_vfx_s* o, const fx_t* data, sz_t size );

/** Sets all vector elements to random values.
 *  Random generator:
 *    Parameters density, min, max, p_rval apply to the random generator.
 *      rval: Pointer to running variable of random generator.
 *            If NULL, an internal fixed random seed is used.
 *
 *     density (range [0.0, 1.0]) specifies the rate at which the random generator
 *     creates a non-zero value.
 */
void bmath_vfx_(s_set_random)( bmath_vfx_s* o, fx_t density, fx_t min, fx_t max, u2_t* p_rval );

/// swaps vectors by swapping data references (no copying of memory)
void bmath_vfx_(s_swapr)( bmath_vfx_s* o, bmath_vfx_s* v );

void bmath_vfx_(s_push)( bmath_vfx_s* o, fx_t val );
void bmath_vfx_(s_push_vfx)( bmath_vfx_s* o, const bmath_vfx_s* vec );

bl_t bmath_vfx_(s_is_near_equ)( const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t max_dev );
bl_t bmath_vfx_(s_is_near_zro)( const bmath_vfx_s* o, fx_t max_dev );

static inline bl_t bmath_vfx_(s_is_equ)( const bmath_vfx_s* o, const bmath_vfx_s* op ) { return bmath_vfx_(s_is_near_equ)( o, op, 0 ); }
static inline bl_t bmath_vfx_(s_is_zro)( const bmath_vfx_s* o ) { return bmath_vfx_(s_is_near_zro)( o, 0 ); }

/// vector is nan if at least one element is nan
bl_t bmath_vfx_(s_is_nan)( const bmath_vfx_s* o );

void bmath_vfx_(s_zro)( bmath_vfx_s* o );
void bmath_vfx_(s_neg)( const bmath_vfx_s* o, bmath_vfx_s* res );
void bmath_vfx_(s_cpy)( const bmath_vfx_s* o, bmath_vfx_s* res );
void bmath_vfx_(s_cat)( const bmath_vfx_s* a, const bmath_vfx_s* b,  bmath_vfx_s* r );
void bmath_vfx_(s_add)( const bmath_vfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );
void bmath_vfx_(s_sub)( const bmath_vfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );

/// adds the same scalar to all components
void bmath_vfx_(s_add_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res );
void bmath_vfx_(s_sub_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res );
void bmath_vfx_(s_add_scl_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res );
void bmath_vfx_(s_sub_scl_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res );

/// hadamard product: a o b [+ c] -> r  (elementwise)
void bmath_vfx_(s_mul_hdm)(     const bmath_vfx_s* a, const bmath_vfx_s* b,                       bmath_vfx_s* r );
void bmath_vfx_(s_mul_hdm_add)( const bmath_vfx_s* a, const bmath_vfx_s* b, const bmath_vfx_s* c, bmath_vfx_s* r );

void bmath_vfx_(s_pmt_mul)(     const bmath_vfx_s* o, const bmath_pmt_s* p, bmath_vfx_s* res ); // y = P   * x ; y_p[k] = x_k
void bmath_vfx_(s_pmt_htp_mul)( const bmath_vfx_s* o, const bmath_pmt_s* p, bmath_vfx_s* res ); // y = P^T * x ; y_k = x_p[k]

void bmath_vfx_(s_mul_scl_fx)(     const bmath_vfx_s* o, fx_t s,                       bmath_vfx_s* r ); // o * b     --> r
void bmath_vfx_(s_mul_scl_fx_add)( const bmath_vfx_s* o, fx_t s, const bmath_vfx_s* a, bmath_vfx_s* r ); // o * b + a --> r

void bmath_vfx_(s_mul_scl)(    const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res  );
void bmath_vfx_(s_mul_fx)(     const bmath_vfx_s* o, fx_t scl2, bmath_vfx_s* res ); // same as bmath_vfx_(s_mul_scl_fx
fx_t bmath_vfx_(s_fx_mul_vec)( const bmath_vfx_s* o, const bmath_vfx_s* vec2 );
fx_t bmath_vfx_(s_fx_sqr)(     const bmath_vfx_s* o );
fx_t bmath_vfx_(s_fx_sub_sqr)( const bmath_vfx_s* o, const bmath_vfx_s* vec2 );
fx_t bmath_vfx_(s_fx_max)(     const bmath_vfx_s* o );
fx_t bmath_vfx_(s_fx_min)(     const bmath_vfx_s* o );
fx_t bmath_vfx_(s_fx_sum)(     const bmath_vfx_s* o );
fx_t bmath_vfx_(s_fx_avg)(     const bmath_vfx_s* o );
fx_t bmath_vfx_(s_fx_var)(     const bmath_vfx_s* o );
fx_t bmath_vfx_(s_fx_dev)(     const bmath_vfx_s* o );
void bmath_vfx_(s_mul_vec)(    const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t* res ); // dot product
void bmath_vfx_(s_sqr)(        const bmath_vfx_s* o, fx_t* res  );
void bmath_vfx_(s_sub_sqr)(    const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t* res );
void bmath_vfx_(s_sum)(        const bmath_vfx_s* o, fx_t* res  );
void bmath_vfx_(s_avg)(        const bmath_vfx_s* o, fx_t* res  );
void bmath_vfx_(s_var)(        const bmath_vfx_s* o, fx_t* res  );
void bmath_vfx_(s_dev)(        const bmath_vfx_s* o, fx_t* res  );

void bmath_vfx_(s_set_sqr)( bmath_vfx_s* o, fx_t val ); // multiplies a factor to all components such that bmath_vfx_(s_fx_sqr returns val
void bmath_vfx_(s_set_sum)( bmath_vfx_s* o, fx_t val ); // adds a value to all components such that bmath_vfx_(s_fx_sum returns val
void bmath_vfx_(s_set_avg)( bmath_vfx_s* o, fx_t val ); // adds a value to all components such that bmath_vfx_(s_fx_avg returns val

static inline
void bmath_vfx_(s_set_fx)( bmath_vfx_s* o, uz_t index, fx_t v )
{
    assert( index < o->size );
    o->data[ index ] = v;
}

static inline
fx_t bmath_vfx_(s_get_fx)( const bmath_vfx_s* o, uz_t index )
{
    assert( index < o->size );
    return o->data[ index ];
}

static inline
bmath_vfx_s bmath_vfx_(s_get_weak_sub_vec)( const bmath_vfx_s* o, uz_t index, uz_t size )
{
    assert( o->size >= index + size );
    return ( bmath_vfx_s ) { .size = size, .space = 0, .data = o->data + index };
}

fx_t bmath_vfx_(s_max)(     const bmath_vfx_s* o );
fx_t bmath_vfx_(s_min)(     const bmath_vfx_s* o );
uz_t bmath_vfx_(s_idx_max)( const bmath_vfx_s* o );
uz_t bmath_vfx_(s_idx_min)( const bmath_vfx_s* o );

//----------------------------------------------------------------------------------------------------------------------
// vector * unary_map --> vector

void bmath_vfx_(s_eop_map)(     const bmath_vfx_s* o, bmath_fp_fx_(ar1) b, bmath_vfx_s* r ); // r_i = b( o_i )
void bmath_vfx_(s_eop_map_mul)( const bmath_vfx_s* o, bmath_fp_fx_(ar1) b, const bmath_vfx_s* c, bmath_vfx_s* r ); // r_i = b( o_i ) * c_i

//----------------------------------------------------------------------------------------------------------------------
// For easy inspection
void bmath_vfx_(s_to_sink)( const bmath_vfx_s* o, bcore_sink* sink );
static inline void bmath_vfx_(s_to_string)( const bmath_vfx_s* o, st_s* string ) { bmath_vfx_(s_to_sink)( o, ( bcore_sink* )string ); }
static inline void bmath_vfx_(s_to_stdout)( const bmath_vfx_s* o               ) { bmath_vfx_(s_to_sink)( o, BCORE_STDOUT ); }

void bmath_vfx_(s_stat_to_sink)( const bmath_vfx_s* o, bcore_sink* sink ); // vector statistics
static inline void bmath_vfx_(s_stat_to_string)( const bmath_vfx_s* o, st_s* string ) { bmath_vfx_(s_stat_to_sink)( o, ( bcore_sink* )string ); }
static inline void bmath_vfx_(s_stat_to_stdout)( const bmath_vfx_s* o               ) { bmath_vfx_(s_stat_to_sink)( o, BCORE_STDOUT ); }

/**********************************************************************************************************************/
// backward compatibility

#if BFPREC_LEVEL == 3

#define bmath_vf3_s_f3_sub_sqr     bmath_vf3_s_fx_sub_sqr
#define bmath_vf3_s_mul_scl_f3_add bmath_vf3_s_mul_scl_fx_add
#define bmath_vf3_s_mul_scl_f3     bmath_vf3_s_mul_scl_fx
#define bmath_vf3_s_mul_f3         bmath_vf3_s_mul_fx
#define bmath_vf3_s_f3_mul_vec     bmath_vf3_s_fx_mul_vec
#define bmath_vf3_s_mul_f3         bmath_vf3_s_mul_fx
#define bmath_vf3_s_get_f3         bmath_vf3_s_get_fx
#define bmath_vf3_s_f3_sqr         bmath_vf3_s_fx_sqr
#define bmath_vf3_s_f3_avg         bmath_vf3_s_fx_avg
#define bmath_vf3_s_set_f3         bmath_vf3_s_set_fx
#define bmath_vf3_s_get_f3         bmath_vf3_s_get_fx

#endif // BFPREC_LEVEL

/**********************************************************************************************************************/
// undefine macros

#undef BFPREC_LEVEL
#undef fx_t
#undef bmath_vfx_s
#undef bmath_vfx_
#undef bmath_fx_t_vec_
#undef bmath_fp_fx_
