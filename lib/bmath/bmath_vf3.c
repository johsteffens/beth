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

#include <stdio.h>

#include "bmath_vf3.h"
#include "bmath_spect_vector.h"

/**********************************************************************************************************************/
// low level vector functions

f3_t bmath_f3_t_vec_sum( const f3_t* v1, sz_t size )
{
#ifdef BMATH_AVX
    __m256d sum_p4 = { 0, 0, 0, 0 };
    sz_t i = 0;
    for( ; i <= size - 4; i += 4 )
    {
        sum_p4 = _mm256_add_pd( _mm256_loadu_pd( v1 + i ), sum_p4 );
    }
    for( ; i < size; i++ ) sum_p4[ 0 ] += v1[ i ];
    return sum_p4[ 0 ] + sum_p4[ 1 ] + sum_p4[ 2 ] + sum_p4[ 3 ];
#else
    f3_t sum_p4[ 4 ] = { 0, 0, 0, 0 };
    sz_t i;
    for( i = 0; i <= size - 4; i += 4 )
    {
        sum_p4[ 0 ] += v1[ i + 0 ];
        sum_p4[ 1 ] += v1[ i + 1 ];
        sum_p4[ 2 ] += v1[ i + 2 ];
        sum_p4[ 3 ] += v1[ i + 3 ];
    }
    for( ; i < size; i++ ) sum_p4[ 0 ] += v1[ i ];
    return sum_p4[ 0 ] + sum_p4[ 1 ] + sum_p4[ 2 ] + sum_p4[ 3 ];
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_f3_t_vec_sum_esp( const f3_t* v1, sz_t size )
{
    if( size <= 32 ) return bmath_f3_t_vec_sum( v1, size );
    sz_t mid = size >> 1;
    return bmath_f3_t_vec_sum_esp( v1, mid ) + bmath_f3_t_vec_sum_esp( v1 + mid, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_f3_t_vec_mul_vec( const f3_t* v1, const f3_t* v2, sz_t size )
{
#ifdef BMATH_AVX
    __m256d sum_p4 = { 0, 0, 0, 0 };
    sz_t i = 0;
    for( ; i <= size - 4; i += 4 )
    {
        #ifdef BMATH_AVX2_FMA
            sum_p4 = _mm256_fmadd_pd( _mm256_loadu_pd( v1 + i ), _mm256_loadu_pd( v2 + i ), sum_p4 );
        #else
            sum_p4 = _mm256_add_pd( _mm256_mul_pd( _mm256_loadu_pd( v1 + i ), _mm256_loadu_pd( v2 + i ) ), sum_p4 );
        #endif // BMATH_AVX2_FMA
    }
    for( ; i < size; i++ ) sum_p4[ 0 ] += v1[ i ] * v2[ i ];
    return sum_p4[ 0 ] + sum_p4[ 1 ] + sum_p4[ 2 ] + sum_p4[ 3 ];
#else
    f3_t sum_p4[ 4 ] = { 0, 0, 0, 0 };
    sz_t i;
    for( i = 0; i <= size - 4; i += 4 )
    {
        sum_p4[ 0 ] += v1[ i + 0 ] * v2[ i + 0 ];
        sum_p4[ 1 ] += v1[ i + 1 ] * v2[ i + 1 ];
        sum_p4[ 2 ] += v1[ i + 2 ] * v2[ i + 2 ];
        sum_p4[ 3 ] += v1[ i + 3 ] * v2[ i + 3 ];
    }
    for( ; i < size; i++ ) sum_p4[ 0 ] += v1[ i ] * v2[ i ];
    return sum_p4[ 0 ] + sum_p4[ 1 ] + sum_p4[ 2 ] + sum_p4[ 3 ];
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_f3_t_vec_mul_vec_stride( const f3_t* v1, sz_t stride1, const f3_t* v2, sz_t stride2, sz_t size )
{
    f3_t sum_p4[ 4 ] = { 0, 0, 0, 0 };
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

f3_t bmath_f3_t_vec_mul_vec_esp( const f3_t* v1, const f3_t* v2, sz_t size )
{
    if( size <= 32 ) return bmath_f3_t_vec_mul_vec( v1, v2, size );
    sz_t mid = size >> 1;
    return bmath_f3_t_vec_mul_vec_esp( v1, v2, mid ) + bmath_f3_t_vec_mul_vec_esp( v1 + mid, v2 + mid, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_f3_t_vec_mul_vec_esp_stride( const f3_t* v1, sz_t stride1, const f3_t* v2, sz_t stride2, sz_t size )
{
    if( size <= 32 ) return bmath_f3_t_vec_mul_vec_stride( v1, stride1, v2, stride2, size );
    sz_t mid = size >> 1;
    return bmath_f3_t_vec_mul_vec_esp_stride( v1, stride1, v2, stride2, mid ) + bmath_f3_t_vec_mul_vec_esp_stride( v1 + mid * stride1, stride1, v2 + mid * stride2, stride2, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_f3_t_vec_mul3_vec( const f3_t* v1, const f3_t* v2, const f3_t* v3, sz_t size )
{
#ifdef BMATH_AVX
    __m256d sum_p4 = { 0, 0, 0, 0 };
    sz_t i = 0;
    for( ; i <= size - 4; i += 4 )
    {
        #ifdef BMATH_AVX2_FMA
            sum_p4 = _mm256_fmadd_pd( _mm256_mul_pd( _mm256_loadu_pd( v1 + i ), _mm256_loadu_pd( v2 + i ) ), _mm256_loadu_pd( v3 + i ), sum_p4 );
        #else
            sum_p4 = _mm256_add_pd( _mm256_mul_pd( _mm256_mul_pd( _mm256_loadu_pd( v1 + i ), _mm256_loadu_pd( v2 + i ) ), _mm256_loadu_pd( v3 + i ) ), sum_p4 );
        #endif // BMATH_AVX2_FMA
    }
    for( ; i < size; i++ ) sum_p4[ 0 ] += v1[ i ] * v2[ i ] * v3[ i ];
    return sum_p4[ 0 ] + sum_p4[ 1 ] + sum_p4[ 2 ] + sum_p4[ 3 ];
#else
    f3_t sum_p4[ 4 ] = { 0, 0, 0, 0 };
    sz_t i;
    for( i = 0; i <= size - 4; i += 4 )
    {
        sum_p4[ 0 ] += v1[ i + 0 ] * v2[ i + 0 ] * v3[ i + 0 ];
        sum_p4[ 1 ] += v1[ i + 1 ] * v2[ i + 1 ] * v3[ i + 1 ];
        sum_p4[ 2 ] += v1[ i + 2 ] * v2[ i + 2 ] * v3[ i + 2 ];
        sum_p4[ 3 ] += v1[ i + 3 ] * v2[ i + 3 ] * v3[ i + 3 ];
    }
    for( ; i < size; i++ ) sum_p4[ 0 ] += v1[ i ] * v2[ i ] * v3[ i ];
    return sum_p4[ 0 ] + sum_p4[ 1 ] + sum_p4[ 2 ] + sum_p4[ 3 ];
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_f3_t_vec_mul3_vec_esp( const f3_t* v1, const f3_t* v2, const f3_t* v3, sz_t size )
{
    if( size <= 32 ) return bmath_f3_t_vec_mul3_vec( v1, v2, v3, size );
    sz_t mid = size >> 1;
    return bmath_f3_t_vec_mul3_vec_esp( v1, v2, v3, mid ) + bmath_f3_t_vec_mul3_vec_esp( v1 + mid, v2 + mid, v3 + mid, size - mid );
}

//----------------------------------------------------------------------------------------------------------------------

// a * b -> r
void bmath_f3_t_vec_mul_scl( const f3_t* a, f3_t b, f3_t* r, sz_t size )
{
    sz_t i = 0;
#ifdef BMATH_AVX
    __m256d b_p4 = _mm256_set1_pd( b );
    for( ; i <= size - 4; i += 4 )
    {
        _mm256_storeu_pd( r + i, _mm256_mul_pd( _mm256_loadu_pd( a + i ), b_p4 ) );
    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) r[ i ] = a[ i ] * b;
}

//----------------------------------------------------------------------------------------------------------------------

// a * b + c -> r
void bmath_f3_t_vec_mul_scl_add( const f3_t* a, f3_t b, const f3_t* c, f3_t* r, sz_t size )
{
    sz_t i = 0;
#ifdef BMATH_AVX
    __m256d b_p4 = _mm256_set1_pd( b );
    for( ; i <= size - 4; i += 4 )
    {
        #ifdef BMATH_AVX2_FMA
            _mm256_storeu_pd( r + i, _mm256_fmadd_pd( _mm256_loadu_pd( a + i ), b_p4 , _mm256_loadu_pd( c + i ) ) );
        #else
            _mm256_storeu_pd( r + i, _mm256_add_pd( _mm256_mul_pd( _mm256_loadu_pd( a + i ), b_p4 ), _mm256_loadu_pd( c + i ) ) );
        #endif // BMATH_AVX2_FMA

    }
#endif // BMATH_AVX
    for( ; i < size; i++ ) r[ i ] = a[ i ] * b + c[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bmath_vf3_s

BCORE_DEFINE_OBJECT_INST( bmath_vector, bmath_vf3_s )
"{"
    "aware_t _;"
    "f3_t [];"
    "func bmath_fp:add;"
    "func bmath_fp:zro;"
    "func bmath_fp:neg;"
    "func bmath_fp:sub;"
    "func bmath_fp:cpy;"
    "func bmath_fp_vector:mul_scl = bmath_vf3_s_mul_scl;"
    "func bmath_fp_vector:mul_vec = bmath_vf3_s_mul_vec;"
"}";

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_move( bmath_vf3_s* o, bmath_vf3_s* src )
{
    if( o == src ) return;
    *o = *src;
    src->space = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_clear( bmath_vf3_s* o )
{
    bcore_array_a_set_space( (bcore_array*)o, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_size( bmath_vf3_s* o, uz_t size )
{
    bcore_array_a_set_size( (bcore_array*)o, size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_space( bmath_vf3_s* o, uz_t space )
{
    bcore_array_a_set_space( (bcore_array*)o, space );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_size_to( const bmath_vf3_s* o, bmath_vf3_s* res )
{
    bmath_vf3_s_set_size( res, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_fill( bmath_vf3_s* o, f3_t val )
{
    for( uz_t i = 0; i < o->size; i++ ) o->data[ i ] = val;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_data( bmath_vf3_s* o, const f3_t* data, sz_t size )
{
    bmath_vf3_s_set_size( o, size );
    bcore_u_memcpy( sizeof( f3_t ), o->data, data, size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_random( bmath_vf3_s* o, f3_t density, f3_t min, f3_t max, u2_t* p_rval )
{
    u2_t rval = p_rval ? *p_rval : 12345;
    f3_t range = max - min;
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

void bmath_vf3_s_push( bmath_vf3_s* o, f3_t val )
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

void bmath_vf3_s_push_vf3( bmath_vf3_s* o, const bmath_vf3_s* vec )
{
    for( uz_t i = 0; i < vec->size; i++ ) bmath_vf3_s_push( o, vec->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bmath_vf3_s_is_near_equ( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t max_dev )
{
    if( o->size != op->size ) return false;
    for( uz_t i = 0   ; i < o->size;  i++ ) if( f3_abs( o->data[ i ] - op->data[ i ] ) > max_dev ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bmath_vf3_s_is_near_zro( const bmath_vf3_s* o, f3_t max_dev )
{
    for( uz_t i = 0; i < o->size; i++ ) if( f3_abs( o->data[ i ] )  > max_dev ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bmath_vf3_s_is_nan( const bmath_vf3_s* o )
{
    for( uz_t i = 0; i < o->size; i++ ) if( f3_is_nan( o->data[ i ] ) ) return true;
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vf3_s* bmath_vf3_s_create_set_size( uz_t size )
{
    bmath_vf3_s* o = bmath_vf3_s_create();
    bmath_vf3_s_set_size( o, size );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vf3_s* bmath_vf3_s_create_fill( uz_t size, f3_t val )
{
    bmath_vf3_s* o = bmath_vf3_s_create_set_size( size );
    bmath_vf3_s_fill( o, val );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_zro( bmath_vf3_s* o )
{
    bcore_u_memzero( sizeof( f3_t ), o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_neg( const bmath_vf3_s* o, bmath_vf3_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0   ; i < o->size; i++ ) res->data[ i ] = -o->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_cpy( const bmath_vf3_s* o, bmath_vf3_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_cat( const bmath_vf3_s* a, const bmath_vf3_s* b, bmath_vf3_s* r )
{
    ASSERT( r->size == a->size + b->size );
    for( uz_t i = 0; i < a->size; i++ ) r->data[           i ] = a->data[ i ];
    for( uz_t i = 0; i < b->size; i++ ) r->data[ a->size + i ] = b->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_add( const bmath_vf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    ASSERT( o->size == op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] + op->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_sub( const bmath_vf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    ASSERT( o->size == op->size );
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] - op->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_add_scl( const bmath_vf3_s* o, const f3_t* op, bmath_vf3_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] + *op;
}


//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_sub_scl( const bmath_vf3_s* o, const f3_t* op, bmath_vf3_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] - *op;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_add_scl_f3( const bmath_vf3_s* o, f3_t op, bmath_vf3_s* res )
{
    bmath_vf3_s_add_scl( o, &op, res );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_sub_scl_f3( const bmath_vf3_s* o, f3_t op, bmath_vf3_s* res )
{
    bmath_vf3_s_sub_scl( o, &op, res );
}

//----------------------------------------------------------------------------------------------------------------------

/// hadamard product: a o b [+ c] -> r  (elementwise)
void bmath_vf3_s_mul_hdm( const bmath_vf3_s* a, const bmath_vf3_s* b, bmath_vf3_s* r )
{
    ASSERT( a->size == b->size );
    ASSERT( a->size == r->size );
    for( uz_t i = 0; i < a->size; i++ ) r->data[ i ] = a->data[ i ] * b->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_mul_hdm_add( const bmath_vf3_s* a, const bmath_vf3_s* b, const bmath_vf3_s* c, bmath_vf3_s* r )
{
    ASSERT( a->size == b->size );
    ASSERT( a->size == c->size );
    ASSERT( a->size == r->size );
    for( uz_t i = 0; i < a->size; i++ ) r->data[ i ] = a->data[ i ] * b->data[ i ] + c->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_pmt_mul( const bmath_vf3_s* o, const bmath_pmt_s* p, bmath_vf3_s* res )
{
    if( o == res )
    {
        bmath_vf3_s* buf = bmath_vf3_s_create();
        bmath_vf3_s_set_size( buf, o->size );
        bmath_vf3_s_pmt_mul( o, p, buf );
        bmath_vf3_s_cpy( buf, res );
        bmath_vf3_s_discard( buf );
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

void bmath_vf3_s_pmt_htp_mul( const bmath_vf3_s* o, const bmath_pmt_s* p, bmath_vf3_s* res )
{
    if( o == res )
    {
        bmath_vf3_s* buf = bmath_vf3_s_create();
        bmath_vf3_s_set_size( buf, o->size );
        bmath_vf3_s_pmt_htp_mul( o, p, buf );
        bmath_vf3_s_cpy( buf, res );
        bmath_vf3_s_discard( buf );
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

void bmath_vf3_s_mul_scl( const bmath_vf3_s* o, const f3_t* op, bmath_vf3_s* res )
{
    ASSERT( o->size == res->size );
    for( uz_t i = 0; i < o->size; i++ ) res->data[ i ] = o->data[ i ] * *op;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_mul_scl_f3(  const bmath_vf3_s* o, f3_t s, bmath_vf3_s* r )
{
    ASSERT( o->size == r->size );
    for( uz_t i = 0; i < o->size; i++ ) r->data[ i ] = o->data[ i ] * s;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_mul_scl_f3_add( const bmath_vf3_s* o, f3_t s, const bmath_vf3_s* a, bmath_vf3_s* r )
{
    ASSERT( o->size == r->size );
    ASSERT( o->size == a->size );
    for( uz_t i = 0; i < o->size; i++ ) r->data[ i ] = o->data[ i ] * s + a->data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_mul_f3(  const bmath_vf3_s* o, f3_t op, bmath_vf3_s* res )
{
    bmath_vf3_s_mul_scl_f3( o, op, res );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_mul_vec( const bmath_vf3_s* o, const bmath_vf3_s* vec2 )
{
    ASSERT( o->size == vec2->size );
    return bmath_f3_t_vec_mul_vec( o->data, vec2->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_sqr( const f3_t* v1, uz_t size )
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
    return f3_s_sqr( v1, sz1 ) + f3_s_sqr( v1 + sz1, size - sz1 );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_sqr( const bmath_vf3_s* o )
{
    return f3_s_sqr( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_sqr_sub( const f3_t* v1, const f3_t* v2, uz_t size )
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
    return f3_s_sqr_sub( v1, v2, sz1 ) + f3_s_sqr_sub( v1 + sz1, v2 + sz1, size - sz1 );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_sub_sqr( const bmath_vf3_s* o, const bmath_vf3_s* vec2 )
{
    ASSERT( o->size == vec2->size );
    return f3_s_sqr_sub( o->data, vec2->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_f3_max( const f3_t* v1, uz_t size )
{
    f3_t max = ( size > 0 ) ? v1[ 0 ] : 0;
    for( uz_t i = 1; i < size; i++ ) max = f3_max( max, v1[ i ] );
    return max;
}

//----------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_f3_min( const f3_t* v1, uz_t size )
{
    f3_t min = ( size > 0 ) ? v1[ 0 ] : 0;
    for( uz_t i = 1; i < size; i++ ) min = f3_min( min, v1[ i ] );
    return min;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_sum( const bmath_vf3_s* o )
{
    return bmath_f3_t_vec_sum( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_avg( const bmath_vf3_s* o )
{
    return o->size > 0 ? bmath_vf3_s_f3_sum( o ) / o->size : 0;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_var( const bmath_vf3_s* o )
{
    f3_t e_v  = bmath_vf3_s_f3_sum( o ) / ( ( o->size > 1 ) ? o->size : 1 );
    f3_t e_vv = bmath_vf3_s_f3_sqr( o ) / ( ( o->size > 1 ) ? o->size : 1 );
    return f3_max( 0.0, e_vv - f3_sqr( e_v ) );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_dev( const bmath_vf3_s* o )
{
    return f3_srt( bmath_vf3_s_f3_var( o ) );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_max( const bmath_vf3_s* o )
{
    return f3_s_f3_max( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_f3_min( const bmath_vf3_s* o )
{
    return f3_s_f3_min( o->data, o->size );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_mul_vec( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t* res )
{
    *res = bmath_vf3_s_f3_mul_vec( o, op );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_sqr( const bmath_vf3_s* o, f3_t* res )
{
    *res = bmath_vf3_s_f3_sqr( o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_sub_sqr( const bmath_vf3_s* o, const bmath_vf3_s* op, f3_t* res )
{
    *res = bmath_vf3_s_f3_sub_sqr( o, op );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_sum( const bmath_vf3_s* o, f3_t* res )
{
    *res = bmath_vf3_s_f3_sum( o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_avg( const bmath_vf3_s* o, f3_t* res )
{
    *res = bmath_vf3_s_f3_avg( o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_var( const bmath_vf3_s* o, f3_t* res )
{
    *res = bmath_vf3_s_f3_var( o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_dev( const bmath_vf3_s* o, f3_t* res )
{
    *res = bmath_vf3_s_f3_dev( o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_sqr( bmath_vf3_s* o, f3_t val )
{
    ASSERT( val >= 0 );
    f3_t cur = bmath_vf3_s_f3_sqr( o );
    f3_t fac = ( cur > 0 ) ? sqrt( val / cur ) : 0;
    bmath_vf3_s_mul_f3( o, fac, o );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_sum( bmath_vf3_s* o, f3_t val )
{
    f3_t cur = bmath_vf3_s_f3_sum( o );
    f3_t add = o->size > 0 ? ( val - cur ) / o->size : 0;
    for( uz_t i = 0; i < o->size; i++ ) o->data[ i ] += add;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_set_avg( bmath_vf3_s* o, f3_t val )
{
    f3_t cur = bmath_vf3_s_f3_avg( o );
    f3_t add = val - cur;
    for( uz_t i = 0; i < o->size; i++ ) o->data[ i ] += add;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_max( const bmath_vf3_s* o )
{
    f3_t v = o->size > 0 ? o->data[ 0 ] : 0;
    for( uz_t i = 1; i < o->size; i++ ) v = f3_max( v, o->data[ i ] );
    return v;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_vf3_s_min( const bmath_vf3_s* o )
{
    f3_t v = o->size > 0 ? o->data[ 0 ] : 0;
    for( uz_t i = 1; i < o->size; i++ ) v = f3_min( v, o->data[ i ] );
    return v;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bmath_vf3_s_idx_max( const bmath_vf3_s* o )
{
    f3_t v = o->size > 0 ? o->data[ 0 ] : 0;
    uz_t i = 0;
    for( uz_t j = 1; j < o->size; j++ )
    {
        i = o->data[ j ] > v ? j : i;
        v = o->data[ j ] > v ? o->data[ j ] : v;
    }
    return i;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bmath_vf3_s_idx_min( const bmath_vf3_s* o )
{
    f3_t v = o->size > 0 ? o->data[ 0 ] : 0;
    uz_t i = 0;
    for( uz_t j = 1; j < o->size; j++ )
    {
        i = o->data[ j ] < v ? j : i;
        v = o->data[ j ] < v ? o->data[ j ] : v;
    }
    return i;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_eop_map( const bmath_vf3_s* o, bmath_fp_f3_ar1 b, bmath_vf3_s* r )
{
    ASSERT( o->size == r->size );
    for( sz_t j = 0; j < o->size; j++ ) r->data[ j ] = b( o->data[ j ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_eop_map_mul( const bmath_vf3_s* o, bmath_fp_f3_ar1 b, const bmath_vf3_s* c, bmath_vf3_s* r )
{
    ASSERT( o->size == r->size );
    ASSERT( o->size == c->size );
    for( sz_t j = 0; j < o->size; j++ ) r->data[ j ] = b( o->data[ j ] ) * c->data[ j ];
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

void bmath_vf3_s_to_sink( const bmath_vf3_s* o, bcore_sink* sink )
{
    const f3_t* v = o->data;
    bcore_sink_a_push_fa( sink, "(#<uz_t>)\n", o->size );
    for( uz_t i = 0; i < o->size; i++ ) bcore_sink_a_push_fa( sink, "#<f3_t>\n", v[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_vf3_s_stat_to_sink( const bmath_vf3_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "(#<uz_t>)\n", o->size );
    bcore_sink_a_push_fa( sink, "  sum .. #<f3_t>\n", bmath_vf3_s_f3_sum( o ) );
    bcore_sink_a_push_fa( sink, "  avg .. #<f3_t>\n", bmath_vf3_s_f3_avg( o ) );
    bcore_sink_a_push_fa( sink, "  max .. #<f3_t>\n", bmath_vf3_s_f3_max( o ) );
    bcore_sink_a_push_fa( sink, "  min .. #<f3_t>\n", bmath_vf3_s_f3_min( o ) );
    bcore_sink_a_push_fa( sink, "  dev .. #<f3_t>\n", bmath_vf3_s_f3_dev( o ) );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();

    /// vf3
    {
        uz_t size = 100;
        bmath_vf3_s* v1 = bcore_life_s_push_aware( l, bmath_vf3_s_create_set_size( size ) );
        for( uz_t i = 0; i < size; i++ ) v1->data[ i ] = i + 1;
        f3_t sqr_sum = 0;
        for( uz_t i = 0; i < size; i++ ) sqr_sum += f3_sqr( v1->data[ i ] );
        ASSERT( bmath_vf3_s_f3_sum( v1 ) == ( size * ( size + 1 ) ) / 2 );
        ASSERT( bmath_vf3_s_f3_avg( v1 ) == ( f3_t )( size * ( size + 1 ) ) / ( 2 * size ) );
        ASSERT( bmath_vf3_s_f3_sqr( v1 ) == sqr_sum );
        ASSERT( bmath_vf3_s_f3_sqr( v1 ) == bmath_vf3_s_f3_mul_vec( v1, v1 ) );

        bmath_vf3_s* v2 = bcore_life_s_push_aware( l, bmath_vf3_s_create_set_size( size ) );
        bmath_vf3_s_cpy( v1, v2 );
        ASSERT( bmath_vf3_s_f3_sum( v2 ) ==   bmath_vf3_s_f3_sum( v1 ) );
        bmath_vf3_s_neg( v1, v2 );
        ASSERT( bmath_vf3_s_f3_sum( v2 ) == - bmath_vf3_s_f3_sum( v1 ) );
        bmath_vf3_s_mul_f3( v1, 2.0, v2 );
        ASSERT( bmath_vf3_s_f3_sum(     v2     ) == 2.0 * bmath_vf3_s_f3_sum( v1 ) );
        ASSERT( bmath_vf3_s_f3_mul_vec( v1, v2 ) == 2.0 * bmath_vf3_s_f3_sqr( v1 ) );

        f3_t v = 0;
        bmath_vector_a_mul_vec( ( const bmath_vector* )v1, ( const bmath_vector* )v2, ( bmath_ring* )&v );
        ASSERT( bmath_vf3_s_f3_mul_vec( v1, v2 ) == v );
    }

    bcore_life_s_discard( l );

    return NULL;
}

/**********************************************************************************************************************/

vd_t bmath_vf3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_vf3" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FFUNC( bmath_fp_add,            bmath_vf3_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro,            bmath_vf3_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg,            bmath_vf3_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub,            bmath_vf3_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_cpy,            bmath_vf3_s_cpy );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_vec, bmath_vf3_s_mul_vec );
            BCORE_REGISTER_FFUNC( bmath_fp_vector_mul_scl, bmath_vf3_s_mul_scl );
            BCORE_REGISTER_OBJECT( bmath_vf3_s );
        }
        break;

        case TYPEOF_selftest:
        {
            return selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

