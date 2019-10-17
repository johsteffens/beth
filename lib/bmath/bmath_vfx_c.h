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
 *  Include in the final *.c file
 *  with macro fx_t (temporarily) defined.
 *
 *  Example:
 *
 *  #define fx_t f2_t // will be undefined in header below
 *  #include "bmath_vfx_c.h"
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

    #define __m256x          __m256
    #define _mm256_add_px    _mm256_add_ps
    #define _mm256_mul_px    _mm256_mul_ps
    #define _mm256_loadu_px  _mm256_loadu_ps
    #define _mm256_storeu_px _mm256_storeu_ps
    #define _mm256_fmadd_px  _mm256_fmadd_ps
    #define _mm256_set1_px   _mm256_set1_ps
    #define P256_SIZE       8
    #define P256_ZERO       { 0, 0, 0, 0, 0, 0, 0, 0 }
    #define P256_HSUM(v)    v[0] + v[1] + v[2] + v[3] + v[4] + v[5] + v[6] + v[7];

#elif BFPREC_LEVEL == 3

    #define fx_t                    f3_t
    #define bmath_vfx_s             bmath_vf3_s
    #define bmath_vfx_(      name ) bmath_vf3_##name
    #define bmath_fx_t_vec_( name ) bmath_f3_t_vec_##name
    #define bmath_fp_fx_(    name ) bmath_fp_f3_##name

    #define __m256x          __m256d
    #define _mm256_add_px    _mm256_add_pd
    #define _mm256_mul_px    _mm256_mul_pd
    #define _mm256_loadu_px  _mm256_loadu_pd
    #define _mm256_storeu_px _mm256_storeu_pd
    #define _mm256_fmadd_px  _mm256_fmadd_pd
    #define _mm256_set1_px   _mm256_set1_pd
    #define P256_SIZE       4
    #define P256_ZERO       { 0, 0, 0, 0 }
    #define P256_HSUM(v)    v[0] + v[1] + v[2] + v[3];

#else

    #error "Macro BFPREC_LEVEL has wrong value! (Only '2' or '3' are supported)"

#endif // BFPREC_LEVEL

/**********************************************************************************************************************/
// low level vector functions (partially AVX optimized)

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_fx_t_vec_(sum)( const fx_t* v1, sz_t size )
{
#ifdef BMATH_AVX
    __m256x sum_p = P256_ZERO;
    sz_t i = 0;
    for( ; i <= size - P256_SIZE; i += P256_SIZE )
    {
        sum_p = _mm256_add_px( _mm256_loadu_px( v1 + i ), sum_p );
    }
#else
    fx_t sum_p[ P256_SIZE ] = P256_ZERO;
    sz_t i;
    for( i = 0; i <= size - P256_SIZE; i += P256_SIZE )
    {
        sum_p4[ 0 ] += v1[ i + 0 ];
        sum_p4[ 1 ] += v1[ i + 1 ];
        sum_p4[ 2 ] += v1[ i + 2 ];
        sum_p4[ 3 ] += v1[ i + 3 ];
        #if P256_SIZE == 8
            sum_p4[ 4 ] += v1[ i + 4 ];
            sum_p4[ 5 ] += v1[ i + 5 ];
            sum_p4[ 6 ] += v1[ i + 6 ];
            sum_p4[ 7 ] += v1[ i + 7 ];
        #endif // P256_SIZE
    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) sum_p[ 0 ] += v1[ i ];
    return P256_HSUM( sum_p );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_fx_t_vec_(sum_esp)( const fx_t* v1, sz_t size )
{
    if( size <= 32 ) return bmath_fx_t_vec_(sum)( v1, size );
    sz_t mid = size >> 1;
    return bmath_fx_t_vec_(sum_esp)( v1, mid ) + bmath_fx_t_vec_(sum_esp)( v1 + mid, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_fx_t_vec_(mul_vec)( const fx_t* v1, const fx_t* v2, sz_t size )
{
#ifdef BMATH_AVX
    __m256x sum_p = P256_ZERO;
    sz_t i = 0;
    for( ; i <= size - P256_SIZE; i += P256_SIZE )
    {
        #ifdef BMATH_AVX2_FMA
            sum_p = _mm256_fmadd_px( _mm256_loadu_px( v1 + i ), _mm256_loadu_px( v2 + i ), sum_p );
        #else
            sum_p = _mm256_add_px( _mm256_mul_px( _mm256_loadu_px( v1 + i ), _mm256_loadu_px( v2 + i ) ), sum_p );
        #endif // BMATH_AVX2_FMA
    }
#else
    fx_t sum_p[ P256_SIZE ] = P256_ZERO;
    sz_t i;
    for( i = 0; i <= size - P256_SIZE; i += P256_SIZE )
    {
        sum_p4[ 0 ] += v1[ i + 0 ] * v2[ i + 0 ];
        sum_p4[ 1 ] += v1[ i + 1 ] * v2[ i + 1 ];
        sum_p4[ 2 ] += v1[ i + 2 ] * v2[ i + 2 ];
        sum_p4[ 3 ] += v1[ i + 3 ] * v2[ i + 3 ];
        #if P256_SIZE == 8
            sum_p4[ 4 ] += v1[ i + 4 ] * v2[ i + 4 ];
            sum_p4[ 5 ] += v1[ i + 5 ] * v2[ i + 5 ];
            sum_p4[ 6 ] += v1[ i + 6 ] * v2[ i + 6 ];
            sum_p4[ 7 ] += v1[ i + 7 ] * v2[ i + 7 ];
        #endif // P256_SIZE
    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) sum_p[ 0 ] += v1[ i ] * v2[ i ];
    return P256_HSUM( sum_p );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_fx_t_vec_(mul_vec_stride)( const fx_t* v1, sz_t stride1, const fx_t* v2, sz_t stride2, sz_t size )
{
    fx_t sum_p4[ 4 ] = { 0, 0, 0, 0 };
    sz_t i;
    for( i = 0; i <= size - 4; i += 4 )
    {
        sum_p4[ 0 ] += v1[ ( i + 0 ) * stride1 ] * v2[ ( i + 0 ) * stride2 ];
        sum_p4[ 1 ] += v1[ ( i + 1 ) * stride1 ] * v2[ ( i + 1 ) * stride2 ];
        sum_p4[ 2 ] += v1[ ( i + 2 ) * stride1 ] * v2[ ( i + 2 ) * stride2 ];
        sum_p4[ 3 ] += v1[ ( i + 3 ) * stride1 ] * v2[ ( i + 3 ) * stride2 ];
    }
    for( ; i < size; i++ ) sum_p4[ 0 ] += v1[ i * stride1 ] * v2[ i * stride2 ];
    return sum_p4[ 0 ] + sum_p4[ 1 ] + sum_p4[ 2 ] + sum_p4[ 3 ];
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_fx_t_vec_(mul_vec_esp)( const fx_t* v1, const fx_t* v2, sz_t size )
{
    if( size <= 32 ) return bmath_fx_t_vec_(mul_vec)( v1, v2, size );
    sz_t mid = size >> 1;
    return bmath_fx_t_vec_(mul_vec_esp)( v1, v2, mid ) + bmath_fx_t_vec_(mul_vec_esp)( v1 + mid, v2 + mid, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_fx_t_vec_(mul_vec_esp_stride)( const fx_t* v1, sz_t stride1, const fx_t* v2, sz_t stride2, sz_t size )
{
    if( size <= 32 ) return bmath_fx_t_vec_(mul_vec_stride)( v1, stride1, v2, stride2, size );
    sz_t mid = size >> 1;
    return bmath_fx_t_vec_(mul_vec_esp_stride)( v1, stride1, v2, stride2, mid ) + bmath_fx_t_vec_(mul_vec_esp_stride)( v1 + mid * stride1, stride1, v2 + mid * stride2, stride2, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_fx_t_vec_(mul3_vec)( const fx_t* v1, const fx_t* v2, const fx_t* v3, sz_t size )
{
#ifdef BMATH_AVX
    __m256x sum_p = P256_ZERO;
    sz_t i = 0;
    for( ; i <= size - P256_SIZE; i += P256_SIZE )
    {
        #ifdef BMATH_AVX2_FMA
            sum_p = _mm256_fmadd_px( _mm256_mul_px( _mm256_loadu_px( v1 + i ), _mm256_loadu_px( v2 + i ) ), _mm256_loadu_px( v3 + i ), sum_p );
        #else
            sum_p = _mm256_add_px( _mm256_mul_px( _mm256_mul_px( _mm256_loadu_px( v1 + i ), _mm256_loadu_px( v2 + i ) ), _mm256_loadu_px( v3 + i ) ), sum_p );
        #endif // BMATH_AVX2_FMA
    }
#else
    fx_t sum_p4[ P256_SIZE ] = P256_ZERO;
    sz_t i;
    for( i = 0; i <= size - P256_SIZE; i += P256_SIZE )
    {
        sum_p4[ 0 ] += v1[ i + 0 ] * v2[ i + 0 ] * v3[ i + 0 ];
        sum_p4[ 1 ] += v1[ i + 1 ] * v2[ i + 1 ] * v3[ i + 1 ];
        sum_p4[ 2 ] += v1[ i + 2 ] * v2[ i + 2 ] * v3[ i + 2 ];
        sum_p4[ 3 ] += v1[ i + 3 ] * v2[ i + 3 ] * v3[ i + 3 ];
        #if P256_SIZE == 8
            sum_p4[ 4 ] += v1[ i + 4 ] * v2[ i + 4 ] * v3[ i + 4 ];
            sum_p4[ 5 ] += v1[ i + 5 ] * v2[ i + 5 ] * v3[ i + 5 ];
            sum_p4[ 6 ] += v1[ i + 6 ] * v2[ i + 6 ] * v3[ i + 6 ];
            sum_p4[ 7 ] += v1[ i + 7 ] * v2[ i + 7 ] * v3[ i + 7 ];
        #endif // P256_SIZE
    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) sum_p[ 0 ] += v1[ i ] * v2[ i ] * v3[ i ];
    return P256_HSUM( sum_p );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_fx_t_vec_(mul3_vec_esp)( const fx_t* v1, const fx_t* v2, const fx_t* v3, sz_t size )
{
    if( size <= 32 ) return bmath_fx_t_vec_(mul3_vec)( v1, v2, v3, size );
    sz_t mid = size >> 1;
    return bmath_fx_t_vec_(mul3_vec_esp)( v1, v2, v3, mid ) + bmath_fx_t_vec_(mul3_vec_esp)( v1 + mid, v2 + mid, v3 + mid, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

// a * b -> r
void bmath_fx_t_vec_(mul_scl)( const fx_t* a, fx_t b, fx_t* r, sz_t size )
{
    sz_t i = 0;
#ifdef BMATH_AVX
    __m256x b_p = _mm256_set1_px( b );
    for( ; i <= size - P256_SIZE; i += P256_SIZE )
    {
        _mm256_storeu_px( r + i, _mm256_mul_px( _mm256_loadu_px( a + i ), b_p ) );
    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) r[ i ] = a[ i ] * b;
}

//----------------------------------------------------------------------------------------------------------------------

// a * b + c -> r
void bmath_fx_t_vec_(mul_scl_add)( const fx_t* a, fx_t b, const fx_t* c, fx_t* r, sz_t size )
{
    sz_t i = 0;
#ifdef BMATH_AVX
    __m256x b_p = _mm256_set1_px( b );
    for( ; i <= size - P256_SIZE; i += P256_SIZE )
    {
        #ifdef BMATH_AVX2_FMA
            _mm256_storeu_px( r + i, _mm256_fmadd_px( _mm256_loadu_px( a + i ), b_p , _mm256_loadu_px( c + i ) ) );
        #else
            _mm256_storeu_px( r + i, _mm256_add_px( _mm256_mul_px( _mm256_loadu_px( a + i ), b_p ), _mm256_loadu_px( c + i ) ) );
        #endif // BMATH_AVX2_FMA

    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) r[ i ] = a[ i ] * b + c[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bmath_vfx_s

void bmath_vfx_(s_move)( bmath_vfx_s* o, bmath_vfx_s* src )
{
    if( o == src ) return;
    *o = *src;
    src->space = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_clear)( bmath_vfx_s* o )
{
    bcore_array_a_set_space( (bcore_array*)o, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_set_size)( bmath_vfx_s* o, uz_t size )
{
    bcore_array_a_set_size( (bcore_array*)o, size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_set_space)( bmath_vfx_s* o, uz_t space )
{
    bcore_array_a_set_space( (bcore_array*)o, space );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_set_size_to)( const bmath_vfx_s* o, bmath_vfx_s* res )
{
    bmath_vfx_(s_set_size)( res, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_fill)( bmath_vfx_s* o, fx_t val )
{
    for( uz_t i = 0; i < o->size; i++ ) o->data[ i ] = val;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_set_data)( bmath_vfx_s* o, const fx_t* data, sz_t size )
{
    bmath_vfx_(s_set_size)( o, size );
    bcore_u_memcpy( sizeof( fx_t ), o->data, data, size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_set_random)( bmath_vfx_s* o, fx_t density, fx_t min, fx_t max, u2_t* p_rval )
{
    u2_t rval = p_rval ? *p_rval : 12345;
    fx_t range = max - min;
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( f3_xsg1_pos( &rval ) < density )
        {
            o->data[ i ] = ( range * f3_xsg1_pos( &rval ) ) + min;
        }
        else
        {
            o->data[ i ] = 0;
        }
    }
    if( p_rval ) *p_rval = rval;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_push)( bmath_vfx_s* o, fx_t val )
{
    if( o->size < o->space )
    {
        o->data[ o->size++ ] = val;
    }
    else
    {
        bcore_array_a_push_f3( (bcore_array*)o, val );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_push_vfx)( bmath_vfx_s* o, const bmath_vfx_s* vec )
{
    for( uz_t i = 0; i < vec->size; i++ ) bmath_vfx_(s_push)( o, vec->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bmath_vfx_(s_is_near_equ)( const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t max_dev )
{
    if( o->size != op->size ) return false;
    for( uz_t i = 0   ; i < o->size;  i++ ) if( f3_abs( o->data[ i ] - op->data[ i ] ) > max_dev ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bmath_vfx_(s_is_near_zro)( const bmath_vfx_s* o, fx_t max_dev )
{
    for( uz_t i = 0; i < o->size; i++ ) if( f3_abs( o->data[ i ] )  > max_dev ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bmath_vfx_(s_is_nan)( const bmath_vfx_s* o )
{
    for( uz_t i = 0; i < o->size; i++ ) if( f3_is_nan( o->data[ i ] ) ) return true;
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vfx_s* bmath_vfx_(s_create_set_size)( uz_t size )
{
    bmath_vfx_s* o = bmath_vfx_(s_create)();
    bmath_vfx_(s_set_size)( o, size );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vfx_s* bmath_vfx_(s_create_fill)( uz_t size, fx_t val )
{
    bmath_vfx_s* o = bmath_vfx_(s_create_set_size)( size );
    bmath_vfx_(s_fill)( o, val );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_zro)( bmath_vfx_s* o )
{
    bcore_u_memzero( sizeof( fx_t ), o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_neg)( const bmath_vfx_s* o, bmath_vfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = -o->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_cpy)( const bmath_vfx_s* o, bmath_vfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_cat)( const bmath_vfx_s* a, const bmath_vfx_s* b, bmath_vfx_s* r )
{
    ASSERT( r->size == a->size + b->size );
    for( uz_t i = 0; i < a->size; i++ ) r->data[           i ] = a->data[ i ];
    for( uz_t i = 0; i < b->size; i++ ) r->data[ a->size + i ] = b->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_add)( const bmath_vfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    ASSERT( o->size == op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] + op->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_sub)( const bmath_vfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    ASSERT( o->size == op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] - op->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_add_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] + *op;
}


//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_sub_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] - *op;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_add_scl_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res )
{
    bmath_vfx_(s_add_scl)( o, &op, res );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_sub_scl_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res )
{
    bmath_vfx_(s_sub_scl)( o, &op, res );
}

//----------------------------------------------------------------------------------------------------------------------

/// hadamard product: a o b [+ c] -> r  (elementwise)
void bmath_vfx_(s_mul_hdm)( const bmath_vfx_s* a, const bmath_vfx_s* b, bmath_vfx_s* r )
{
    ASSERT( a->size == b->size );
    ASSERT( a->size == r->size );
    for( uz_t i = 0; i < a->size; i++ ) r->data[ i ] = a->data[ i ] * b->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_mul_hdm_add)( const bmath_vfx_s* a, const bmath_vfx_s* b, const bmath_vfx_s* c, bmath_vfx_s* r )
{
    ASSERT( a->size == b->size );
    ASSERT( a->size == c->size );
    ASSERT( a->size == r->size );
    for( uz_t i = 0; i < a->size; i++ ) r->data[ i ] = a->data[ i ] * b->data[ i ] + c->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_pmt_mul)( const bmath_vfx_s* o, const bmath_pmt_s* p, bmath_vfx_s* res )
{
    if( o == res )
    {
        bmath_vfx_s* buf = bmath_vfx_(s_create)();
        bmath_vfx_(s_set_size)( buf, o->size );
        bmath_vfx_(s_pmt_mul)( o, p, buf );
        bmath_vfx_(s_cpy)( buf, res );
        bmath_vfx_(s_discard)( buf );
        return;
    }

    ASSERT( o->size == p->size );
    ASSERT( o->size == res->size );

    for( uz_t i = 0; i < p->size; i++ )
    {
        assert( p->data[ i ] < o->size );
        res->data[ p->data[ i ] ] = o->data[ i ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_pmt_htp_mul)( const bmath_vfx_s* o, const bmath_pmt_s* p, bmath_vfx_s* res )
{
    if( o == res )
    {
        bmath_vfx_s* buf = bmath_vfx_(s_create)();
        bmath_vfx_(s_set_size)( buf, o->size );
        bmath_vfx_(s_pmt_htp_mul)( o, p, buf );
        bmath_vfx_(s_cpy)( buf, res );
        bmath_vfx_(s_discard)( buf );
        return;
    }

    ASSERT( o->size == p->size );
    ASSERT( o->size == res->size );

    for( uz_t i = 0; i < p->size; i++ )
    {
        assert( p->data[ i ] < o->size );
        res->data[ i ] = o->data[ p->data[ i ] ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_mul_scl)( const bmath_vfx_s* o, const fx_t* op, bmath_vfx_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] * *op;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_mul_scl_fx)(  const bmath_vfx_s* o, fx_t s, bmath_vfx_s* r )
{
    ASSERT( o->size == r->size );
    for( uz_t i = 0; i < o->size; i++ ) r->data[ i ] = o->data[ i ] * s;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_mul_scl_fx_add)( const bmath_vfx_s* o, fx_t s, const bmath_vfx_s* a, bmath_vfx_s* r )
{
    ASSERT( o->size == r->size );
    ASSERT( o->size == a->size );
    for( uz_t i = 0; i < o->size; i++ ) r->data[ i ] = o->data[ i ] * s + a->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_mul_fx)( const bmath_vfx_s* o, fx_t op, bmath_vfx_s* res )
{
    bmath_vfx_(s_mul_scl_fx)( o, op, res );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_vfx_(s_fx_mul_vec)( const bmath_vfx_s* o, const bmath_vfx_s* vec2 )
{
    ASSERT( o->size == vec2->size );
    return bmath_fx_t_vec_(mul_vec)( o->data, vec2->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

static fx_t fx_s_sqr( const fx_t* v1, uz_t size )
{
    switch( size )
    {
        case 0: return 0;
        case 1: return v1[0]*v1[0];
        case 2: return v1[0]*v1[0]+v1[1]*v1[1];
        case 3: return v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2];
        case 4: return v1[0]*v1[0]+v1[1]*v1[1]+v1[2]*v1[2]+v1[3]*v1[3];
        default: break;
    }
    uz_t sz1 = size >> 1;
    return fx_s_sqr( v1, sz1 ) + fx_s_sqr( v1 + sz1, size - sz1 );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_vfx_(s_fx_sqr)( const bmath_vfx_s* o )
{
    return fx_s_sqr( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

static fx_t fx_s_sqr_sub( const fx_t* v1, const fx_t* v2, uz_t size )
{
    switch( size )
    {
        case 0: return 0;
        case 1: return f3_sqr(v1[0]-v2[0]);
        case 2: return f3_sqr(v1[0]-v2[0])+f3_sqr(v1[1]-v2[1]);
        case 3: return f3_sqr(v1[0]-v2[0])+f3_sqr(v1[1]-v2[1])+f3_sqr(v1[2]-v2[2]);
        case 4: return f3_sqr(v1[0]-v2[0])+f3_sqr(v1[1]-v2[1])+f3_sqr(v1[2]-v2[2])+f3_sqr(v1[3]-v2[3]);
        default: break;
    }
    uz_t sz1 = size >> 1;
    return fx_s_sqr_sub( v1, v2, sz1 ) + fx_s_sqr_sub( v1 + sz1, v2 + sz1, size - sz1 );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_vfx_(s_fx_sub_sqr)( const bmath_vfx_s* o, const bmath_vfx_s* vec2 )
{
    ASSERT( o->size == vec2->size );
    return fx_s_sqr_sub( o->data, vec2->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

static fx_t fx_max( fx_t a, fx_t b ) { return ( a > b ) ? a : b; }
static fx_t fx_min( fx_t a, fx_t b ) { return ( a < b ) ? a : b; }

static fx_t fx_s_fx_max( const fx_t* v1, uz_t size )
{
    fx_t max = ( size > 0 ) ? v1[ 0 ] : 0;
    for( uz_t i = 1; i < size; i++ ) max = fx_max( max, v1[ i ] );
    return max;
}

//----------------------------------------------------------------------------------------------------------------------

static fx_t fx_s_fx_min( const fx_t* v1, uz_t size )
{
    fx_t min = ( size > 0 ) ? v1[ 0 ] : 0;
    for( uz_t i = 1; i < size; i++ ) min = fx_min( min, v1[ i ] );
    return min;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_vfx_(s_fx_sum)( const bmath_vfx_s* o )
{
    return bmath_fx_t_vec_(sum)( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_vfx_(s_fx_avg)( const bmath_vfx_s* o )
{
    return o->size > 0 ? bmath_vfx_(s_fx_sum)( o ) / o->size : 0;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_vfx_(s_fx_var)( const bmath_vfx_s* o )
{
    fx_t e_v  = bmath_vfx_(s_fx_sum)( o ) / ( ( o->size > 1 ) ? o->size : 1 );
    fx_t e_vv = bmath_vfx_(s_fx_sqr)( o ) / ( ( o->size > 1 ) ? o->size : 1 );
    return f3_max( 0.0, e_vv - f3_sqr( e_v ) );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_vfx_(s_fx_dev)( const bmath_vfx_s* o )
{
    return f3_srt( bmath_vfx_(s_fx_var)( o ) );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_vfx_(s_fx_max)( const bmath_vfx_s* o )
{
    return fx_s_fx_max( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_vfx_(s_fx_min)( const bmath_vfx_s* o )
{
    return fx_s_fx_min( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_mul_vec)( const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t* res )
{
    *res = bmath_vfx_(s_fx_mul_vec)( o, op );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_sqr)( const bmath_vfx_s* o, fx_t* res )
{
    *res = bmath_vfx_(s_fx_sqr)( o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_sub_sqr)( const bmath_vfx_s* o, const bmath_vfx_s* op, fx_t* res )
{
    *res = bmath_vfx_(s_fx_sub_sqr)( o, op );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_sum)( const bmath_vfx_s* o, fx_t* res )
{
    *res = bmath_vfx_(s_fx_sum)( o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_avg)( const bmath_vfx_s* o, fx_t* res )
{
    *res = bmath_vfx_(s_fx_avg)( o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_var)( const bmath_vfx_s* o, fx_t* res )
{
    *res = bmath_vfx_(s_fx_var)( o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_dev)( const bmath_vfx_s* o, fx_t* res )
{
    *res = bmath_vfx_(s_fx_dev)( o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_set_sqr)( bmath_vfx_s* o, fx_t val )
{
    ASSERT( val >= 0 );
    fx_t cur = bmath_vfx_(s_fx_sqr)( o );
    fx_t fac = ( cur > 0 ) ? sqrt( val / cur ) : 0;
    bmath_vfx_(s_mul_fx)( o, fac, o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_set_sum)( bmath_vfx_s* o, fx_t val )
{
    fx_t cur = bmath_vfx_(s_fx_sum)( o );
    fx_t add = o->size > 0 ? ( val - cur ) / o->size : 0;
    for( uz_t i = 0; i < o->size; i++ ) o->data[ i ] += add;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_set_avg)( bmath_vfx_s* o, fx_t val )
{
    fx_t cur = bmath_vfx_(s_fx_avg)( o );
    fx_t add = val - cur;
    for( uz_t i = 0; i < o->size; i++ ) o->data[ i ] += add;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_vfx_(s_max)( const bmath_vfx_s* o )
{
    fx_t v = o->size > 0 ? o->data[ 0 ] : 0;
    for( uz_t i = 1; i < o->size; i++ ) v = fx_max( v, o->data[ i ] );
    return v;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t bmath_vfx_(s_min)( const bmath_vfx_s* o )
{
    fx_t v = o->size > 0 ? o->data[ 0 ] : 0;
    for( uz_t i = 1; i < o->size; i++ ) v = fx_min( v, o->data[ i ] );
    return v;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bmath_vfx_(s_idx_max)( const bmath_vfx_s* o )
{
    fx_t v = o->size > 0 ? o->data[ 0 ] : 0;
    uz_t i = 0;
    for( uz_t j = 1; j < o->size; j++ )
    {
        i = o->data[ j ] > v ? j : i;
        v = o->data[ j ] > v ? o->data[ j ] : v;
    }
    return i;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bmath_vfx_(s_idx_min)( const bmath_vfx_s* o )
{
    fx_t v = o->size > 0 ? o->data[ 0 ] : 0;
    uz_t i = 0;
    for( uz_t j = 1; j < o->size; j++ )
    {
        i = o->data[ j ] < v ? j : i;
        v = o->data[ j ] < v ? o->data[ j ] : v;
    }
    return i;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_eop_map)( const bmath_vfx_s* o, bmath_fp_fx_(ar1) b, bmath_vfx_s* r )
{
    ASSERT( o->size == r->size );
    for( sz_t j = 0; j < o->size; j++ ) r->data[ j ] = b( o->data[ j ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_eop_map_mul)( const bmath_vfx_s* o, bmath_fp_fx_(ar1) b, const bmath_vfx_s* c, bmath_vfx_s* r )
{
    ASSERT( o->size == r->size );
    ASSERT( o->size == c->size );
    for( sz_t j = 0; j < o->size; j++ ) r->data[ j ] = b( o->data[ j ] ) * c->data[ j ];
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_to_sink)( const bmath_vfx_s* o, bcore_sink* sink )
{
    const fx_t* v = o->data;
    bcore_sink_a_push_fa( sink, "(#<uz_t>)\n", o->size );
    for( uz_t i = 0; i < o->size; i++ ) bcore_sink_a_push_fa( sink, "#<f3_t>\n", v[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vfx_(s_stat_to_sink)( const bmath_vfx_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "(#<uz_t>)\n", o->size );
    bcore_sink_a_push_fa( sink, "  sum .. #<f3_t>\n", bmath_vfx_(s_fx_sum)( o ) );
    bcore_sink_a_push_fa( sink, "  avg .. #<f3_t>\n", bmath_vfx_(s_fx_avg)( o ) );
    bcore_sink_a_push_fa( sink, "  max .. #<f3_t>\n", bmath_vfx_(s_fx_max)( o ) );
    bcore_sink_a_push_fa( sink, "  min .. #<f3_t>\n", bmath_vfx_(s_fx_min)( o ) );
    bcore_sink_a_push_fa( sink, "  dev .. #<f3_t>\n", bmath_vfx_(s_fx_dev)( o ) );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_spect_vector.h"

static void bmath_vfx_(selftest)( void )
{
    BLM_INIT();

    {
        uz_t size = 100;
        bmath_vfx_s* v1 = BLM_A_PUSH( bmath_vfx_(s_create_set_size)( size ) );
        for( uz_t i = 0; i < size; i++ ) v1->data[ i ] = i + 1;
        f3_t sqr_sum = 0;
        for( uz_t i = 0; i < size; i++ ) sqr_sum += f3_sqr( v1->data[ i ] );
        ASSERT( bmath_vfx_(s_fx_sum)( v1 ) == ( size * ( size + 1 ) ) / 2 );
        ASSERT( bmath_vfx_(s_fx_avg)( v1 ) == ( fx_t )( size * ( size + 1 ) ) / ( 2 * size ) );
        ASSERT( bmath_vfx_(s_fx_sqr)( v1 ) == sqr_sum );
        ASSERT( bmath_vfx_(s_fx_sqr)( v1 ) == bmath_vfx_(s_fx_mul_vec)( v1, v1 ) );

        bmath_vfx_s* v2 = BLM_A_PUSH( bmath_vfx_(s_create_set_size)( size ) );
        bmath_vfx_(s_cpy)( v1, v2 );
        ASSERT( bmath_vfx_(s_fx_sum)( v2 ) ==  bmath_vfx_(s_fx_sum)( v1 ) );
        bmath_vfx_(s_neg)( v1, v2 );
        ASSERT( bmath_vfx_(s_fx_sum)( v2 ) == -bmath_vfx_(s_fx_sum)( v1 ) );
        bmath_vfx_(s_mul_fx)( v1, 2.0, v2 );
        ASSERT( bmath_vfx_(s_fx_sum)(     v2     ) == 2.0 * bmath_vfx_(s_fx_sum)( v1 ) );
        ASSERT( bmath_vfx_(s_fx_mul_vec)( v1, v2 ) == 2.0 * bmath_vfx_(s_fx_sqr)( v1 ) );

        fx_t v = 0;
        bmath_vector_a_mul_vec( ( const bmath_vector* )v1, ( const bmath_vector* )v2, ( bmath_ring* )&v );
        ASSERT( bmath_vfx_(s_fx_mul_vec)( v1, v2 ) == v );
    }

    BLM_DOWN();
}

/**********************************************************************************************************************/

