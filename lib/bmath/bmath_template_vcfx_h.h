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

/** Template: Complex Vector */

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

/**********************************************************************************************************************/
/// checks, deviations

/** Near-state means: For each vector element the absolute difference
 *  to the specified state is less or equal max_dev.
 *  Hence, max_dev == 0 tests for the exact state (s. below).
 */
bl_t BCATU(bmath_vcfx_s,is_near_equ)( const bmath_vcfx_s* o, const bmath_vcfx_s* op, fx_t max_dev ); // equality
bl_t BCATU(bmath_vcfx_s,is_near_zro)( const bmath_vcfx_s* o, fx_t max_dev ); // zero

static inline bl_t BCATU(bmath_vcfx_s,is_equ)( const bmath_vcfx_s* o, const bmath_vcfx_s* op ) { return BCATU(bmath_vcfx_s,is_near_equ)( o, op, 0 ); }
static inline bl_t BCATU(bmath_vcfx_s,is_zro)( const bmath_vcfx_s* o ) { return BCATU(bmath_vcfx_s,is_near_zro)( o, 0 ); }

/// Vector is nan if at least one element is nan
bl_t BCATU(bmath_vcfx_s,is_nan)( const bmath_vcfx_s* o );

/// total sum of squares
fx_t BCATU(bmath_vcfx_s,tss)( const bmath_vcfx_s* o );

/// Generalized frobenius norm (sqrt(sum over all components: sqr(difference))
fx_t BCATU(bmath_vcfx_s,fdev)( const bmath_vcfx_s* o, const bmath_vcfx_s* op );

/**********************************************************************************************************************/

void BCATU(bmath_vcfx_s,move)( bmath_vcfx_s* o, bmath_vcfx_s* src );
void BCATU(bmath_vcfx_s,clear)( bmath_vcfx_s* o );

void BCATU(bmath_vcfx_s,set_size)( bmath_vcfx_s* o, uz_t size );
void BCATU(bmath_vcfx_s,push)(     bmath_vcfx_s* o, bmath_cfx_s cfx );
void BCATU(bmath_vcfx_s,push_ri)(  bmath_vcfx_s* o, fx_t r, fx_t i );

static inline void BCATU(bmath_vcfx_s,push_r)( bmath_vcfx_s* o, fx_t r ) { BCATU(bmath_vcfx_s,push_ri)( o, r, 0 ); }
static inline void BCATU(bmath_vcfx_s,push_i)( bmath_vcfx_s* o, fx_t i ) { BCATU(bmath_vcfx_s,push_ri)( o, 0, i ); }
static inline void BCATU(bmath_vcfx_s,set_r)( bmath_vcfx_s* o, uz_t index, fx_t r ) { assert( index < o->size ); o->data[ index ].v[ 0 ] = r; }
static inline void BCATU(bmath_vcfx_s,set_i)( bmath_vcfx_s* o, uz_t index, fx_t i ) { assert( index < o->size ); o->data[ index ].v[ 1 ] = i; }

/// sets real values from vector
static inline void BCATU(bmath_vcfx_s,set_vr)( bmath_vcfx_s* o, const bmath_vfx_s* vec ) { uz_t sz = uz_min( o->size, vec->size ); for( uz_t i = 0; i < sz; i++ ) o->data[ i ].v[ 0 ] = vec->data[ i ]; }

/// sets imag values from vector
static inline void BCATU(bmath_vcfx_s,set_vi)( bmath_vcfx_s* o, const bmath_vfx_s* vec ) { uz_t sz = uz_min( o->size, vec->size ); for( uz_t i = 0; i < sz; i++ ) o->data[ i ].v[ 1 ] = vec->data[ i ]; }

bmath_vcfx_s* BCATU(bmath_vcfx_s,create_size)( uz_t size );
bmath_vcfx_s* BCATU(bmath_vcfx_s,create_fill)( bmath_cfx_s val, uz_t size );

/** Sets all vector elements to random values.
 *  Random generator:
 *    Parameters density, min, max, p_rval apply to the random generator.
 *      rval: Pointer to running variable of random generator.
 *            If NULL, an internal fixed random seed is used.
 *
 *     density (range [0.0, 1.0]) specifies the rate at which the random generator
 *     creates a non-zero value.
 */
void BCATU(bmath_vcfx_s,set_random)(    bmath_vcfx_s* o, fx_t density, fx_t min, fx_t max, bcore_prsg* prsg );
void BCATU(bmath_vcfx_s,set_random_u3)( bmath_vcfx_s* o, fx_t density, fx_t min, fx_t max, u3_t* p_rval );

void BCATU(bmath_vcfx_s,zro)(           bmath_vcfx_s* o );
void BCATU(bmath_vcfx_s,neg)(     const bmath_vcfx_s* o, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,cnj)(     const bmath_vcfx_s* o, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,cpy)(     const bmath_vcfx_s* o, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,add)(     const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,sub)(     const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,mul_scl)( const bmath_vcfx_s* o, const bmath_cfx_s*  op, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,mul_cfx)( const bmath_vcfx_s* o, const bmath_cfx_s   op, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,mul_fx)(  const bmath_vcfx_s* o,              fx_t   op, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,dft)(     const bmath_vcfx_s* o, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,ift)(     const bmath_vcfx_s* o, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,mul_vec)( const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_cfx_s* res );
void BCATU(bmath_vcfx_s,sqr)(     const bmath_vcfx_s* o, bmath_cfx_s* res );
void BCATU(bmath_vcfx_s,sub_sqr)( const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_cfx_s* res );
void BCATU(bmath_vcfx_s,sum)(     const bmath_vcfx_s* o, bmath_cfx_s* res );
void BCATU(bmath_vcfx_s,avg)(     const bmath_vcfx_s* o, bmath_cfx_s* res );

static inline
void BCATU(bmath_vcfx_s,set_cfx)( bmath_vcfx_s* o, uz_t index, bmath_cfx_s v )
{
    assert( index < o->size );
    o->data[ index ] = v;
}

static inline
bmath_cfx_s BCATU(bmath_vcfx_s,get_cfx)( const bmath_vcfx_s* o, uz_t index )
{
    assert( index < o->size );
    return o->data[ index ];
}

/// For easy inspection
void BCATU(bmath_vcfx_s,to_stdout)( const bmath_vcfx_s* o );

/**********************************************************************************************************************/
// type conversion

void BCATU(bmath_vcfx_s,copy_typed)( bmath_vcfx_s* o, tp_t type, vc_t src );

static inline void BCATU(bmath_vcfx_s,copy_t)( bmath_vcfx_s* o, tp_t type, vc_t src )
{
    BCATU(bmath_vcfx_s,copy_typed)( o, type, src );
}

static inline void BCATU(bmath_vcfx_s,copy_a)( bmath_vcfx_s* o, vc_t src )
{
    if( src )
    {
        BCATU(bmath_vcfx_s,copy_t)( o, *(aware_t*)src, src );
    }
    else
    {
        BCATU(bmath_vcfx_s,clear)( o );
    }
}

static inline bmath_vcfx_s* BCATU(bmath_vcfx_s,clone_t)( tp_t type, vc_t src )
{
    if( !src ) return NULL;
    bmath_vcfx_s* o = BCATU(bmath_vcfx_s,create)();
    BCATU(bmath_vcfx_s,copy_t)( o, type, src );
    return o;
}

static inline bmath_vcfx_s* BCATU(bmath_vcfx_s,clone_a)( vc_t src )
{
    if( !src ) return NULL;
    bmath_vcfx_s* o = BCATU(bmath_vcfx_s,create)();
    BCATU(bmath_vcfx_s,copy_a)( o, src );
    return o;
}

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

