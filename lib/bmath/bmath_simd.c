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

#include "bmath_simd.h"
#include "bmath_grt.h"
#include "bmath_vector.h"

//#define BMATH_USE_AVX
//#define BMATH_NO_SIMD

#ifndef BMATH_NO_SIMD

#ifdef __AVX__
    #define BMATH_USE_AVX
#endif

#ifdef __AVX2__
    #define BMATH_USE_AVX2
#endif

#endif  // BMATH_NO_SIMD

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_simd_f3_mul_vec_default( const f3_t* v1, const f3_t* v2, sz_t size )
{
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
}

#ifdef BMATH_USE_AVX
f3_t bmath_simd_f3_mul_vec_avx( const f3_t* v1, const f3_t* v2, sz_t size )
{
    __m256d sum_p4 = { 0, 0, 0, 0 };
    sz_t i;
    for( i = 0; i <= size - 4; i += 4 )
    {
        sum_p4 = _mm256_add_pd( sum_p4, _mm256_mul_pd( _mm256_loadu_pd( v1 + i ), _mm256_loadu_pd( v2 + i ) ) );

//        sum_p4 = _mm256_fmadd_pd( _mm256_loadu_pd( v1 + i ), _mm256_loadu_pd( v2 + i ), sum_p4 );
    }
    for( ; i < size; i++ ) sum_p4[ 0 ] += v1[ i ] * v2[ i ];
    return sum_p4[ 0 ] + sum_p4[ 1 ] + sum_p4[ 2 ] + sum_p4[ 3 ];
}
#endif // BMATH_USE_AVX

f3_t bmath_simd_f3_mul_vec( const f3_t* v1, const f3_t* v2, sz_t size )
{
#ifdef BMATH_USE_AVX
    return bmath_simd_f3_mul_vec_avx( v1, v2, size );
#else
    return bmath_simd_f3_mul_vec_default( v1, v2, size );
#endif // BMATH_USE_AVX

}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_simd_f3_mul_vec3_default( const f3_t* v1, const f3_t* v2, const f3_t* v3, sz_t size )
{
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
}

#ifdef BMATH_USE_AVX
f3_t bmath_simd_f3_mul_vec3_avx( const f3_t* v1, const f3_t* v2, const f3_t* v3, sz_t size )
{
    __m256d sum_p4 = { 0, 0, 0, 0 };
    sz_t i;
    for( i = 0; i <= size - 4; i += 4 )
    {
        sum_p4 = _mm256_add_pd( sum_p4,
                               _mm256_mul_pd( _mm256_mul_pd( _mm256_loadu_pd( v1 + i ), _mm256_loadu_pd( v2 + i ) ),
                                              _mm256_loadu_pd( v3 + i ) ) );

//        sum_p4 = _mm256_fmadd_pd( _mm256_mul_pd( _mm256_loadu_pd( v1 + i ), _mm256_loadu_pd( v2 + i ) ), _mm256_loadu_pd( v3 + i ), sum_p4 );
    }
    for( ; i < size; i++ ) sum_p4[ 0 ] += v1[ i ] * v2[ i ] * v3[ i ];
    return sum_p4[ 0 ] + sum_p4[ 1 ] + sum_p4[ 2 ] + sum_p4[ 3 ];
}
#endif // BMATH_USE_AVX

f3_t bmath_simd_f3_mul_vec3( const f3_t* v1, const f3_t* v2, const f3_t* v3, sz_t size )
{
#ifdef BMATH_USE_AVX
    return bmath_simd_f3_mul_vec3_avx( v1, v2, v3, size );
#else
    return bmath_simd_f3_mul_vec3_default( v1, v2, v3, size );
#endif // BMATH_USE_AVX

}

//---------------------------------------------------------------------------------------------------------------------

void bmath_simd_f3_row_rotate_default( f3_t* v1, f3_t* v2, sz_t size, const bmath_grt_f3_s* grt )
{
    for( sz_t i = 0; i < size; i++ )
    {
        f3_t b = v2[ i ];
        v2[ i ] = b * grt->c - v1[ i ] * grt->s;
        v1[ i ] = b * grt->s + v1[ i ] * grt->c;
    }
}

#ifdef BMATH_USE_AVX

void bmath_simd_f3_row_rotate_avx( f3_t* v1, f3_t* v2, sz_t size, const bmath_grt_f3_s* grt )
{
    __m256d s_p4 = { grt->s, grt->s, grt->s, grt->s };
    __m256d c_p4 = { grt->c, grt->c, grt->c, grt->c };

    sz_t i;
    for( i = 0; i <= size - 4; i += 4 )
    {
        __m256d a_p4 = _mm256_loadu_pd( v1 + i );
        __m256d b_p4 = _mm256_loadu_pd( v2 + i );
        _mm256_storeu_pd( v1 + i, _mm256_add_pd( _mm256_mul_pd( a_p4, c_p4 ), _mm256_mul_pd( b_p4, s_p4 ) ) );
        _mm256_storeu_pd( v2 + i, _mm256_sub_pd( _mm256_mul_pd( b_p4, c_p4 ), _mm256_mul_pd( a_p4, s_p4 ) ) );

//        _mm256_storeu_pd( v1 + i, _mm256_fmadd_pd( a_p4, c_p4, _mm256_mul_pd( b_p4, s_p4 ) ) );
//        _mm256_storeu_pd( v2 + i, _mm256_fmsub_pd( b_p4, c_p4, _mm256_mul_pd( a_p4, s_p4 ) ) );
    }

    for( ; i < size; i++ )
    {
        f3_t b = v2[ i ];
        v2[ i ] = b * grt->c - v1[ i ] * grt->s;
        v1[ i ] = b * grt->s + v1[ i ] * grt->c;
    }
}

#endif // BMATH_USE_AVX

void bmath_simd_f3_row_rotate( f3_t* v1, f3_t* v2, sz_t size, const bmath_grt_f3_s* grt )
{
#ifdef BMATH_USE_AVX
    bmath_simd_f3_row_rotate_avx( v1, v2, size, grt );
#else
    bmath_simd_f3_row_rotate_default( v1, v2, size, grt );
#endif // BMATH_USE_AVX
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_simd_f3_col_rotate_default( f3_t* v1, f3_t* v2, sz_t stride, sz_t size, const bmath_grt_f3_s* grt )
{
    for( sz_t i = 0; i < size; i++ )
    {
        f3_t b = v2[ i * stride ];
        v2[ i * stride ] = b * grt->c - v1[ i * stride ] * grt->s;
        v1[ i * stride ] = b * grt->s + v1[ i * stride ] * grt->c;
    }
}

//---------------------------------------------------------------------------------------------------------------------

#ifdef BMATH_USE_AVX

void bmath_simd_f3_col_rotate_avx( f3_t* v1, f3_t* v2, sz_t stride, sz_t size, const bmath_grt_f3_s* grt )
{
    __m256d s_p4 = { grt->s, grt->s, grt->s, grt->s };
    __m256d c_p4 = { grt->c, grt->c, grt->c, grt->c };

    sz_t i;
    for( i = 0; i <= size - 4; i += 4 )
    {
        __m256d a1_p4 = { v1[ ( i + 0 ) * stride ], v1[ ( i + 1 ) * stride ], v1[ ( i + 2 ) * stride ], v1[ ( i + 3 ) * stride ] };
        __m256d b1_p4 = { v2[ ( i + 0 ) * stride ], v2[ ( i + 1 ) * stride ], v2[ ( i + 2 ) * stride ], v2[ ( i + 3 ) * stride ] };
        __m256d a2_p4 = _mm256_add_pd( _mm256_mul_pd( a1_p4, c_p4 ), _mm256_mul_pd( b1_p4, s_p4 ) );
        __m256d b2_p4 = _mm256_sub_pd( _mm256_mul_pd( b1_p4, c_p4 ), _mm256_mul_pd( a1_p4, s_p4 ) );
        v1[ ( i + 0 ) * stride ] = a2_p4[ 0 ];
        v1[ ( i + 1 ) * stride ] = a2_p4[ 1 ];
        v1[ ( i + 2 ) * stride ] = a2_p4[ 2 ];
        v1[ ( i + 3 ) * stride ] = a2_p4[ 3 ];

        v2[ ( i + 0 ) * stride ] = b2_p4[ 0 ];
        v2[ ( i + 1 ) * stride ] = b2_p4[ 1 ];
        v2[ ( i + 2 ) * stride ] = b2_p4[ 2 ];
        v2[ ( i + 3 ) * stride ] = b2_p4[ 3 ];
    }

    for( ; i < size; i++ )
    {
        f3_t b = v2[ i * stride ];
        v2[ i * stride ] = b * grt->c - v1[ i * stride ] * grt->s;
        v1[ i * stride ] = b * grt->s + v1[ i * stride ] * grt->c;
    }
}
#endif // BMATH_USE_AVX

//---------------------------------------------------------------------------------------------------------------------

void bmath_simd_f3_col_rotate( f3_t* v1, f3_t* v2, sz_t stride, sz_t size, const bmath_grt_f3_s* grt )
{
#ifdef BMATH_USE_AVX
    bmath_simd_f3_col_rotate_avx( v1, v2, stride, size, grt );
#else
    bmath_simd_f3_col_rotate_default( v1, v2, stride, size, grt );
#endif // BMATH_USE_AVX
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_simd_f3_drow_swipe_rev( f3_t* row, const bmath_grt_f3_s* grt, sz_t size )
{
    f3_t v = row[ 0 ];
    sz_t i;
    for( i = size; i > 4; i -= 4 )
    {
        bmath_grt_f3_s_rotate( grt + i - 1, &v, row + i - 0 );
        bmath_grt_f3_s_rotate( grt + i - 2, &v, row + i - 1 );
        bmath_grt_f3_s_rotate( grt + i - 3, &v, row + i - 2 );
        bmath_grt_f3_s_rotate( grt + i - 4, &v, row + i - 3 );
    }

    for( ; i > 0; i-- ) bmath_grt_f3_s_rotate( grt + i - 1, &v, row + i );

    row[ 0 ] = v;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_simd_f3_4drow_swipe_rev( f3_t* row, sz_t stride, const bmath_grt_f3_s* grt, sz_t size )
{
    f3_t a[ 4 ];

    a[ 0 ] = row[ stride * 0 ];
    a[ 1 ] = row[ stride * 1 ];
    a[ 2 ] = row[ stride * 2 ];
    a[ 3 ] = row[ stride * 3 ];

    for( sz_t i = size; i > 0; i-- )
    {
        bmath_grt_f3_s_rotate( grt + i - 1, a + 0, row + stride * 0 + i );
        bmath_grt_f3_s_rotate( grt + i - 1, a + 1, row + stride * 1 + i );
        bmath_grt_f3_s_rotate( grt + i - 1, a + 2, row + stride * 2 + i );
        bmath_grt_f3_s_rotate( grt + i - 1, a + 3, row + stride * 3 + i );
    }

    row[ stride * 0 ] = a[ 0 ];
    row[ stride * 1 ] = a[ 1 ];
    row[ stride * 2 ] = a[ 2 ];
    row[ stride * 3 ] = a[ 3 ];
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//---------------------------------------------------------------------------------------------------------------------

static vd_t selftest( void )
{
#ifdef BMATH_USE_AVX
    BCORE_LIFE_INIT();

    BCORE_LIFE_CREATE( bmath_vf3_s, v1 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v2 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v3 );

    sz_t n = 1000;

    bmath_vf3_s_set_size( v1, n );
    bmath_vf3_s_set_size( v2, n );
    bmath_vf3_s_set_size( v3, n );

    u2_t rval = 1234;

    bmath_vf3_s_set_random( v1, 1.0, -1.0, 1.0, &rval );
    bmath_vf3_s_set_random( v2, 1.0, -1.0, 1.0, &rval );
    bmath_vf3_s_set_random( v3, 1.0, -1.0, 1.0, &rval );
    f3_t p0, p1;
    p0 = bmath_simd_f3_mul_vec_default( v1->data, v2->data, v1->size );
    p1 = bmath_simd_f3_mul_vec_avx(     v1->data, v2->data, v1->size );
    ASSERT( p0 == p1 );

    p0 = bmath_simd_f3_mul_vec3_default( v1->data, v2->data, v3->data, v1->size );
    p1 = bmath_simd_f3_mul_vec3_avx(     v1->data, v2->data, v3->data, v1->size );
    ASSERT( p0 == p1 );

    BCORE_LIFE_DOWN();
#endif  // BMATH_USE_AVX
    return NULL;
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_simd_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_simd" ) ) )
    {
        case TYPEOF_init1:
        {
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
