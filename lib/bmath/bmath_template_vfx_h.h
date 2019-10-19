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

/**********************************************************************************************************************/


/** Vector Template.
 *
 *  Include in the final header file after object definition
 *  with macro BMATH_TEMPLATE_FX_PREC (temporarily) defined.
 *
 *  Example:
 *
 *  #define BMATH_TEMPLATE_FX_PREC 2 // defines all functions for f2_t
 *  #include "bmath_template_vfx_h.h"
 *
 *  #define BMATH_TEMPLATE_FX_PREC 3 // defines all functions for f3_t
 *  #include "bmath_template_vfx_h.h"
 */

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

// NOTE: Do not include non-templates (template definitions could be undone/changed)
#include "bmath_template_fx_begin.h"

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

static inline void bmath_vfx_s_(init_weak)( bmath_vfx_s* o, fx_t* data, sz_t size ) { bmath_vfx_s_(init)( o ); o->data = data; o->size = size; }
static inline bmath_vfx_s bmath_vfx_(init_weak)( fx_t* data, sz_t size ) { bmath_vfx_s o; bmath_vfx_s_(init_weak)( &o, data, size ); return o; }

void bmath_vfx_s_(move)( bmath_vfx_s* o, bmath_vfx_s* src );
void bmath_vfx_s_(clear)( bmath_vfx_s* o );

bmath_vfx_s* bmath_vfx_s_(create_set_size)( uz_t size );
bmath_vfx_s* bmath_vfx_s_(create_fill)( uz_t size, fx_t val );

void bmath_vfx_s_(set_size)(    bmath_vfx_s* o, uz_t size );
void bmath_vfx_s_(set_space)(   bmath_vfx_s* o, uz_t space );
void bmath_vfx_s_(set_size_to)( const bmath_vfx_s* o, bmath_vfx_s* res );
void bmath_vfx_s_(fill)(        bmath_vfx_s* o, fx_t val );

/// copies external data into vector
void bmath_vfx_s_(set_data)( bmath_vfx_s* o, const fx_t* data, sz_t size );

/** Sets all vector elements to random values.
 *  Random generator:
 *    Parameters density, min, max, p_rval apply to the random generator.
 *      rval: Pointer to running variable of random generator.
 *            If NULL, an internal fixed random seed is used.
 *
 *     density (range [0.0, 1.0]) specifies the rate at which the random generator
 *     creates a non-zero value.
 */
void bmath_vfx_s_(set_random)( bmath_vfx_s* o, fx_t density, fx_t min, fx_t max, u2_t* p_rval );

/// swaps vectors by swapping data references (no copying of memory)
void bmath_vfx_s_(swapr)( bmath_vfx_s* o, bmath_vfx_s* v );

void bmath_vfx_s_(push)( bmath_vfx_s* o, fx_t val );
void bmath_vfx_s_(push_vfx)( bmath_vfx_s* o, const bmath_vfx_s* vec );

bl_t bmath_vfx_s_(is_near_equ)( const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t max_dev );
bl_t bmath_vfx_s_(is_near_zro)( const bmath_vfx_s* o, fx_t max_dev );

static inline bl_t bmath_vfx_s_(is_equ)( const bmath_vfx_s* o, const bmath_vfx_s* op ) { return bmath_vfx_s_(is_near_equ)( o, op, 0 ); }
static inline bl_t bmath_vfx_s_(is_zro)( const bmath_vfx_s* o ) { return bmath_vfx_s_(is_near_zro)( o, 0 ); }

/// vector is nan if at least one element is nan
bl_t bmath_vfx_s_(is_nan)( const bmath_vfx_s* o );

void bmath_vfx_s_(zro)( bmath_vfx_s* o );
void bmath_vfx_s_(neg)( const bmath_vfx_s* o, bmath_vfx_s* res );
void bmath_vfx_s_(cpy)( const bmath_vfx_s* o, bmath_vfx_s* res );
void bmath_vfx_s_(cat)( const bmath_vfx_s* a, const bmath_vfx_s* b,  bmath_vfx_s* r );
void bmath_vfx_s_(add)( const bmath_vfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );
void bmath_vfx_s_(sub)( const bmath_vfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );

/// adds the same scalar to all components
void bmath_vfx_s_(add_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res );
void bmath_vfx_s_(sub_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res );
void bmath_vfx_s_(add_scl_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res );
void bmath_vfx_s_(sub_scl_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res );

/// hadamard product: a o b [+ c] -> r  (elementwise)
void bmath_vfx_s_(mul_hdm)(     const bmath_vfx_s* a, const bmath_vfx_s* b,                       bmath_vfx_s* r );
void bmath_vfx_s_(mul_hdm_add)( const bmath_vfx_s* a, const bmath_vfx_s* b, const bmath_vfx_s* c, bmath_vfx_s* r );

void bmath_vfx_s_(pmt_mul)(     const bmath_vfx_s* o, const bmath_pmt_s* p, bmath_vfx_s* res ); // y = P   * x ; y_p[k] = x_k
void bmath_vfx_s_(pmt_htp_mul)( const bmath_vfx_s* o, const bmath_pmt_s* p, bmath_vfx_s* res ); // y = P^T * x ; y_k = x_p[k]

void bmath_vfx_s_(mul_scl_fx)(     const bmath_vfx_s* o, fx_t s,                       bmath_vfx_s* r ); // o * b     --> r
void bmath_vfx_s_(mul_scl_fx_add)( const bmath_vfx_s* o, fx_t s, const bmath_vfx_s* a, bmath_vfx_s* r ); // o * b + a --> r

void bmath_vfx_s_(mul_scl)(    const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res  );
void bmath_vfx_s_(mul_fx)(     const bmath_vfx_s* o, fx_t scl2, bmath_vfx_s* res ); // same as bmath_vfx_s_(mul_scl_fx
fx_t bmath_vfx_s_(fx_mul_vec)( const bmath_vfx_s* o, const bmath_vfx_s* vec2 );
fx_t bmath_vfx_s_(fx_sqr)(     const bmath_vfx_s* o );
fx_t bmath_vfx_s_(fx_sub_sqr)( const bmath_vfx_s* o, const bmath_vfx_s* vec2 );
fx_t bmath_vfx_s_(fx_max)(     const bmath_vfx_s* o );
fx_t bmath_vfx_s_(fx_min)(     const bmath_vfx_s* o );
fx_t bmath_vfx_s_(fx_sum)(     const bmath_vfx_s* o );
fx_t bmath_vfx_s_(fx_avg)(     const bmath_vfx_s* o );
fx_t bmath_vfx_s_(fx_var)(     const bmath_vfx_s* o );
fx_t bmath_vfx_s_(fx_dev)(     const bmath_vfx_s* o );
void bmath_vfx_s_(mul_vec)(    const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t* res ); // dot product
void bmath_vfx_s_(sqr)(        const bmath_vfx_s* o, fx_t* res  );
void bmath_vfx_s_(sub_sqr)(    const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t* res );
void bmath_vfx_s_(sum)(        const bmath_vfx_s* o, fx_t* res  );
void bmath_vfx_s_(avg)(        const bmath_vfx_s* o, fx_t* res  );
void bmath_vfx_s_(var)(        const bmath_vfx_s* o, fx_t* res  );
void bmath_vfx_s_(dev)(        const bmath_vfx_s* o, fx_t* res  );

void bmath_vfx_s_(set_sqr)( bmath_vfx_s* o, fx_t val ); // multiplies a factor to all components such that bmath_vfx_s_(fx_sqr returns val
void bmath_vfx_s_(set_sum)( bmath_vfx_s* o, fx_t val ); // adds a value to all components such that bmath_vfx_s_(fx_sum returns val
void bmath_vfx_s_(set_avg)( bmath_vfx_s* o, fx_t val ); // adds a value to all components such that bmath_vfx_s_(fx_avg returns val

static inline
void bmath_vfx_s_(set_fx)( bmath_vfx_s* o, uz_t index, fx_t v )
{
    assert( index < o->size );
    o->data[ index ] = v;
}

static inline
fx_t bmath_vfx_s_(get_fx)( const bmath_vfx_s* o, uz_t index )
{
    assert( index < o->size );
    return o->data[ index ];
}

static inline
bmath_vfx_s bmath_vfx_s_(get_weak_sub_vec)( const bmath_vfx_s* o, uz_t index, uz_t size )
{
    assert( o->size >= index + size );
    return ( bmath_vfx_s ) { .size = size, .space = 0, .data = o->data + index };
}

fx_t bmath_vfx_s_(max)(     const bmath_vfx_s* o );
fx_t bmath_vfx_s_(min)(     const bmath_vfx_s* o );
uz_t bmath_vfx_s_(idx_max)( const bmath_vfx_s* o );
uz_t bmath_vfx_s_(idx_min)( const bmath_vfx_s* o );

//----------------------------------------------------------------------------------------------------------------------
// vector * unary_map --> vector

void bmath_vfx_s_(eop_map)(     const bmath_vfx_s* o, bmath_fp_fx_(ar1) b, bmath_vfx_s* r ); // r_i = b( o_i )
void bmath_vfx_s_(eop_map_mul)( const bmath_vfx_s* o, bmath_fp_fx_(ar1) b, const bmath_vfx_s* c, bmath_vfx_s* r ); // r_i = b( o_i ) * c_i

//----------------------------------------------------------------------------------------------------------------------
// For easy inspection
void bmath_vfx_s_(to_sink)( const bmath_vfx_s* o, bcore_sink* sink );
static inline void bmath_vfx_s_(to_string)( const bmath_vfx_s* o, st_s* string ) { bmath_vfx_s_(to_sink)( o, ( bcore_sink* )string ); }
static inline void bmath_vfx_s_(to_stdout)( const bmath_vfx_s* o               ) { bmath_vfx_s_(to_sink)( o, BCORE_STDOUT ); }

void bmath_vfx_s_(stat_to_sink)( const bmath_vfx_s* o, bcore_sink* sink ); // vector statistics
static inline void bmath_vfx_s_(stat_to_string)( const bmath_vfx_s* o, st_s* string ) { bmath_vfx_s_(stat_to_sink)( o, ( bcore_sink* )string ); }
static inline void bmath_vfx_s_(stat_to_stdout)( const bmath_vfx_s* o               ) { bmath_vfx_s_(stat_to_sink)( o, BCORE_STDOUT ); }

/**********************************************************************************************************************/
// backward compatibility

#if BMATH_TEMPLATE_FX_PREC == 3

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

#endif // BMATH_TEMPLATE_FX_PREC

/**********************************************************************************************************************/
// closing template

#include "bmath_template_fx_end.h"
