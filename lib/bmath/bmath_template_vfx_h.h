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

/**********************************************************************************************************************/

/** Template: Vector */

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
 *  tss: total sum of squares (also vector-norm)
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/
// low level vector functions (partially AVX optimized)

fx_t BCATU(bmath,fx,t_vec,sum)(          const fx_t* v1, sz_t size ); // sum_i v1[i]
fx_t BCATU(bmath,fx,t_vec,sum_esp)(      const fx_t* v1, sz_t size ); // sum_i v1[i]
fx_t BCATU(bmath,fx,t_vec,mul_vec)(      const fx_t* v1, const fx_t* v2, sz_t size ); // sum_i v1[i]*v2[i]
fx_t BCATU(bmath,fx,t_vec,mul_vec_esp)(  const fx_t* v1, const fx_t* v2, sz_t size ); // sum_i v1[i]*v2[i]
fx_t BCATU(bmath,fx,t_vec,mul3_vec)(     const fx_t* v1, const fx_t* v2, const fx_t* v3, sz_t size ); // sum_i v1[i]*v2[i]*v3[i]
fx_t BCATU(bmath,fx,t_vec,mul3_vec_esp)( const fx_t* v1, const fx_t* v2, const fx_t* v3, sz_t size ); // sum_i v1[i]*v2[i]*v3[i]

/// with stride for both vectors
fx_t BCATU(bmath,fx,t_vec,mul_vec_stride)(     const fx_t* v1, sz_t stride1, const fx_t* v2, sz_t stride2, sz_t size ); // sum_i v1[i]*v2[i]
fx_t BCATU(bmath,fx,t_vec,mul_vec_esp_stride)( const fx_t* v1, sz_t stride1, const fx_t* v2, sz_t stride2, sz_t size ); // sum_i v1[i]*v2[i]

void BCATU(bmath,fx,t_vec,mul_scl)(     const fx_t* a, fx_t b,                fx_t* r, sz_t size ); // a * b + c -> r
void BCATU(bmath,fx,t_vec,mul_scl_add)( const fx_t* a, fx_t b, const fx_t* c, fx_t* r, sz_t size ); // a * b + c -> r

/**********************************************************************************************************************/
// vector object functions

static inline void BCATU(bmath_vfx_s,init_weak)( bmath_vfx_s* o, fx_t* data, sz_t size ) { BCATU(bmath_vfx_s,init)( o ); o->data = data; o->size = size; }
static inline bmath_vfx_s BCATU(bmath,vfx,init_weak)( fx_t* data, sz_t size ) { bmath_vfx_s o; BCATU(bmath_vfx_s,init_weak)( &o, data, size ); return o; }

void BCATU(bmath_vfx_s,move)( bmath_vfx_s* o, bmath_vfx_s* src );
void BCATU(bmath_vfx_s,clear)( bmath_vfx_s* o );

bmath_vfx_s* BCATU(bmath_vfx_s,create_set_size)( uz_t size );
bmath_vfx_s* BCATU(bmath_vfx_s,create_fill)( uz_t size, fx_t val );

void BCATU(bmath_vfx_s,set_size)(    bmath_vfx_s* o, uz_t size );
void BCATU(bmath_vfx_s,set_space)(   bmath_vfx_s* o, uz_t space );
void BCATU(bmath_vfx_s,set_size_to)( const bmath_vfx_s* o, bmath_vfx_s* res );
void BCATU(bmath_vfx_s,fill)(        bmath_vfx_s* o, fx_t val );

/// copies external data into vector
void BCATU(bmath_vfx_s,set_data)( bmath_vfx_s* o, const fx_t* data, sz_t size );

/** Sets all vector elements to random values.
 *  Random generator:
 *    Parameters density, min, max, p_rval apply to the random generator.
 *      rval: Pointer to running variable of random generator.
 *            If NULL, an internal fixed random seed is used.
 *
 *     density (range [0.0, 1.0]) specifies the rate at which the random generator
 *     creates a non-zero value.
 */
void BCATU(bmath_vfx_s,set_random)( bmath_vfx_s* o, fx_t density, fx_t min, fx_t max, u2_t* p_rval );

/// swaps vectors by swapping data references (no copying of memory)
void BCATU(bmath_vfx_s,swapr)( bmath_vfx_s* o, bmath_vfx_s* v );

void BCATU(bmath_vfx_s,push)( bmath_vfx_s* o, fx_t val );
void BCATU(bmath_vfx_s,push_vfx)( bmath_vfx_s* o, const bmath_vfx_s* vec );

bl_t BCATU(bmath_vfx_s,is_near_equ)( const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t max_dev );
bl_t BCATU(bmath_vfx_s,is_near_zro)( const bmath_vfx_s* o, fx_t max_dev );

static inline bl_t BCATU(bmath_vfx_s,is_equ)( const bmath_vfx_s* o, const bmath_vfx_s* op ) { return BCATU(bmath_vfx_s,is_near_equ)( o, op, 0 ); }
static inline bl_t BCATU(bmath_vfx_s,is_zro)( const bmath_vfx_s* o ) { return BCATU(bmath_vfx_s,is_near_zro)( o, 0 ); }

/// sqrt(sum over squared differences)
fx_t BCATU(bmath_vfx_s,fdev)( const bmath_vfx_s* o, const bmath_vfx_s* op );

/// vector is nan if at least one element is nan
bl_t BCATU(bmath_vfx_s,is_nan)( const bmath_vfx_s* o );

/// total sum of squares
fx_t BCATU(bmath_vfx_s,tss)( const bmath_vfx_s* o );

/** fdev = ||f(o) - x||
 *  '|| ... ||' = Frobenius norm  ( sqrt(sum over squares) )
 */
fx_t BCATU(bmath_vfx_s,fdev_equ)( const bmath_vfx_s* a, const bmath_vfx_s* b );
fx_t BCATU(bmath_vfx_s,fdev_zro)( const bmath_vfx_s* o );

void BCATU(bmath_vfx_s,zro)( bmath_vfx_s* o );
void BCATU(bmath_vfx_s,neg)( const bmath_vfx_s* o, bmath_vfx_s* res );
void BCATU(bmath_vfx_s,cpy)( const bmath_vfx_s* o, bmath_vfx_s* res );
void BCATU(bmath_vfx_s,cat)( const bmath_vfx_s* a, const bmath_vfx_s* b,  bmath_vfx_s* r );
void BCATU(bmath_vfx_s,add)( const bmath_vfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );
void BCATU(bmath_vfx_s,sub)( const bmath_vfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res );

/// adds the same scalar to all components
void BCATU(bmath_vfx_s,add_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res );
void BCATU(bmath_vfx_s,sub_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res );
void BCATU(bmath_vfx_s,add_scl_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res );
void BCATU(bmath_vfx_s,sub_scl_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res );

/// hadamard product: a o b [+ c] -> r  (elementwise)
void BCATU(bmath_vfx_s,mul_hdm)(     const bmath_vfx_s* a, const bmath_vfx_s* b,                       bmath_vfx_s* r );
void BCATU(bmath_vfx_s,mul_hdm_add)( const bmath_vfx_s* a, const bmath_vfx_s* b, const bmath_vfx_s* c, bmath_vfx_s* r );

void BCATU(bmath_vfx_s,pmt_mul)(     const bmath_vfx_s* o, const bmath_pmt_s* p, bmath_vfx_s* res ); // y = P   * x ; y_p[k] = x_k
void BCATU(bmath_vfx_s,pmt_htp_mul)( const bmath_vfx_s* o, const bmath_pmt_s* p, bmath_vfx_s* res ); // y = P^T * x ; y_k = x_p[k]

void BCATU(bmath_vfx_s,mul_scl_fx)(     const bmath_vfx_s* o, fx_t s,                       bmath_vfx_s* r ); // o * b     --> r
void BCATU(bmath_vfx_s,mul_scl_fx_add)( const bmath_vfx_s* o, fx_t s, const bmath_vfx_s* a, bmath_vfx_s* r ); // o * b + a --> r

void BCATU(bmath_vfx_s,mul_scl)(    const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res  );
void BCATU(bmath_vfx_s,mul_fx)(     const bmath_vfx_s* o, fx_t scl2, bmath_vfx_s* res ); // same as BCATU(bmath_vfx_s,mul_scl_fx
fx_t BCATU(bmath_vfx_s,fx_mul_vec)( const bmath_vfx_s* o, const bmath_vfx_s* vec2 );
fx_t BCATU(bmath_vfx_s,fx_sqr)(     const bmath_vfx_s* o );
fx_t BCATU(bmath_vfx_s,fx_sub_sqr)( const bmath_vfx_s* o, const bmath_vfx_s* vec2 );
fx_t BCATU(bmath_vfx_s,fx_max)(     const bmath_vfx_s* o );
fx_t BCATU(bmath_vfx_s,fx_min)(     const bmath_vfx_s* o );
fx_t BCATU(bmath_vfx_s,fx_sum)(     const bmath_vfx_s* o );
fx_t BCATU(bmath_vfx_s,fx_avg)(     const bmath_vfx_s* o );
fx_t BCATU(bmath_vfx_s,fx_var)(     const bmath_vfx_s* o );
fx_t BCATU(bmath_vfx_s,fx_dev)(     const bmath_vfx_s* o );
void BCATU(bmath_vfx_s,mul_vec)(    const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t* res ); // dot product
void BCATU(bmath_vfx_s,sqr)(        const bmath_vfx_s* o, fx_t* res  );
void BCATU(bmath_vfx_s,sub_sqr)(    const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t* res );
void BCATU(bmath_vfx_s,sum)(        const bmath_vfx_s* o, fx_t* res  );
void BCATU(bmath_vfx_s,avg)(        const bmath_vfx_s* o, fx_t* res  );
void BCATU(bmath_vfx_s,var)(        const bmath_vfx_s* o, fx_t* res  );
void BCATU(bmath_vfx_s,dev)(        const bmath_vfx_s* o, fx_t* res  );

void BCATU(bmath_vfx_s,set_sqr)( bmath_vfx_s* o, fx_t val ); // multiplies a factor to all components such that BCATU(bmath_vfx_s,fx_sqr returns val
void BCATU(bmath_vfx_s,set_sum)( bmath_vfx_s* o, fx_t val ); // adds a value to all components such that BCATU(bmath_vfx_s,fx_sum returns val
void BCATU(bmath_vfx_s,set_avg)( bmath_vfx_s* o, fx_t val ); // adds a value to all components such that BCATU(bmath_vfx_s,fx_avg returns val

static inline
void BCATU(bmath_vfx_s,set_fx)( bmath_vfx_s* o, uz_t index, fx_t v )
{
    assert( index < o->size );
    o->data[ index ] = v;
}

static inline
fx_t BCATU(bmath_vfx_s,get_fx)( const bmath_vfx_s* o, uz_t index )
{
    assert( index < o->size );
    return o->data[ index ];
}

static inline
bmath_vfx_s BCATU(bmath_vfx_s,get_weak_sub_vec)( const bmath_vfx_s* o, uz_t index, uz_t size )
{
    assert( o->size >= index + size );
    return ( bmath_vfx_s ) { .size = size, .space = 0, .data = o->data + index };
}

fx_t BCATU(bmath_vfx_s,max)(     const bmath_vfx_s* o );
fx_t BCATU(bmath_vfx_s,min)(     const bmath_vfx_s* o );
uz_t BCATU(bmath_vfx_s,idx_max)( const bmath_vfx_s* o );
uz_t BCATU(bmath_vfx_s,idx_min)( const bmath_vfx_s* o );

//----------------------------------------------------------------------------------------------------------------------
// vector * unary_map --> vector

void BCATU(bmath_vfx_s,eop_map)(     const bmath_vfx_s* o, BCATU(bmath_fp,fx,ar1) b, bmath_vfx_s* r ); // r_i = b( o_i )
void BCATU(bmath_vfx_s,eop_map_mul)( const bmath_vfx_s* o, BCATU(bmath_fp,fx,ar1) b, const bmath_vfx_s* c, bmath_vfx_s* r ); // r_i = b( o_i ) * c_i

//----------------------------------------------------------------------------------------------------------------------
// For easy inspection
void BCATU(bmath_vfx_s,to_sink)( const bmath_vfx_s* o, bcore_sink* sink );
static inline void BCATU(bmath_vfx_s,to_string)( const bmath_vfx_s* o, st_s* string ) { BCATU(bmath_vfx_s,to_sink)( o, ( bcore_sink* )string ); }
static inline void BCATU(bmath_vfx_s,to_stdout)( const bmath_vfx_s* o               ) { BCATU(bmath_vfx_s,to_sink)( o, BCORE_STDOUT ); }

void BCATU(bmath_vfx_s,stat_to_sink)( const bmath_vfx_s* o, bcore_sink* sink ); // vector statistics
static inline void BCATU(bmath_vfx_s,stat_to_string)( const bmath_vfx_s* o, st_s* string ) { BCATU(bmath_vfx_s,stat_to_sink)( o, ( bcore_sink* )string ); }
static inline void BCATU(bmath_vfx_s,stat_to_stdout)( const bmath_vfx_s* o               ) { BCATU(bmath_vfx_s,stat_to_sink)( o, BCORE_STDOUT ); }

/**********************************************************************************************************************/
// type conversion

void BCATU(bmath_vfx_s,copy_typed)( bmath_vfx_s* o, tp_t type, vc_t src );

static inline void BCATU(bmath_vfx_s,copy_t)( bmath_vfx_s* o, tp_t type, vc_t src )
{
    BCATU(bmath_vfx_s,copy_typed)( o, type, src );
}

static inline void BCATU(bmath_vfx_s,copy_a)( bmath_vfx_s* o, vc_t src )
{
    if( src )
    {
        BCATU(bmath_vfx_s,copy_t)( o, *(aware_t*)src, src );
    }
    else
    {
        BCATU(bmath_vfx_s,clear)( o );
    }
}

static inline bmath_vfx_s* BCATU(bmath_vfx_s,clone_t)( tp_t type, vc_t src )
{
    if( !src ) return NULL;
    bmath_vfx_s* o = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,copy_t)( o, type, src );
    return o;
}

static inline bmath_vfx_s* BCATU(bmath_vfx_s,clone_a)( vc_t src )
{
    if( !src ) return NULL;
    bmath_vfx_s* o = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,copy_a)( o, src );
    return o;
}

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

#include "bmath_template_fx_end.h"
