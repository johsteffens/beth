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

#include "bmath_grt_f3.h"

BCORE_DEFINE_OBJECT_FLAT( bcore_inst, bmath_grt_f3_s ) "{ f3_t c; f3_t s; }";

//----------------------------------------------------------------------------------------------------------------------

void bmath_grt_f3_s_to_stdout( const bmath_grt_f3_s* o )
{
    bcore_msg_fa( "c=#<f3_t> s=#<f3_t>\n", o->c, o->s );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_arr_grt_f3_s ) "{ aware_t _; bmath_grt_f3_s [] arr; }";

//----------------------------------------------------------------------------------------------------------------------

bmath_arr_grt_f3_s* bmath_arr_grt_f3_s_create_size( uz_t size )
{
    bmath_arr_grt_f3_s* o = bmath_arr_grt_f3_s_create();
    bmath_arr_grt_f3_s_set_size( o, size );
    bmath_arr_grt_f3_s_zro( o );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_arr_grt_f3_s bmath_arr_grt_f3_of_size( uz_t size )
{
    bmath_arr_grt_f3_s o;
    bmath_arr_grt_f3_s_init( &o );
    bmath_arr_grt_f3_s_set_size( &o, size );
    bmath_arr_grt_f3_s_zro( &o );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// simd optimized code

void bmath_simd_f3_row_rotate_default( f3_t* v1, f3_t* v2, sz_t size, const bmath_grt_f3_s* grt )
{
    for( sz_t i = 0; i < size; i++ )
    {
        f3_t b = v2[ i ];
        v2[ i ] = b * grt->c - v1[ i ] * grt->s;
        v1[ i ] = b * grt->s + v1[ i ] * grt->c;
    }
}

#ifdef BMATH_AVX

void bmath_simd_f3_row_rotate_avx( f3_t* v1, f3_t* v2, sz_t size, const bmath_grt_f3_s* grt )
{
    __m256d s_p4 = { grt->s, grt->s, grt->s, grt->s };
    __m256d c_p4 = { grt->c, grt->c, grt->c, grt->c };

    sz_t i;
    for( i = 0; i <= size - 4; i += 4 )
    {
        __m256d a_p4 = _mm256_loadu_pd( v1 + i );
        __m256d b_p4 = _mm256_loadu_pd( v2 + i );

#ifdef BMATH_AVX2_FMA
        _mm256_storeu_pd( v1 + i, _mm256_fmadd_pd( a_p4, c_p4, _mm256_mul_pd( b_p4, s_p4 ) ) );
        _mm256_storeu_pd( v2 + i, _mm256_fmsub_pd( b_p4, c_p4, _mm256_mul_pd( a_p4, s_p4 ) ) );
#else
        _mm256_storeu_pd( v1 + i, _mm256_add_pd( _mm256_mul_pd( a_p4, c_p4 ), _mm256_mul_pd( b_p4, s_p4 ) ) );
        _mm256_storeu_pd( v2 + i, _mm256_sub_pd( _mm256_mul_pd( b_p4, c_p4 ), _mm256_mul_pd( a_p4, s_p4 ) ) );
#endif // BMATH_AVX2_FMA

    }

    for( ; i < size; i++ )
    {
        f3_t b = v2[ i ];
        v2[ i ] = b * grt->c - v1[ i ] * grt->s;
        v1[ i ] = b * grt->s + v1[ i ] * grt->c;
    }
}

#endif // BMATH_AVX

void bmath_simd_f3_row_rotate( f3_t* v1, f3_t* v2, sz_t size, const bmath_grt_f3_s* grt )
{
#ifdef BMATH_AVX
    bmath_simd_f3_row_rotate_avx( v1, v2, size, grt );
#else
    bmath_simd_f3_row_rotate_default( v1, v2, size, grt );
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_simd_f3_col_rotate_default( f3_t* v1, f3_t* v2, sz_t stride, sz_t size, const bmath_grt_f3_s* grt )
{
    for( sz_t i = 0; i < size; i++ )
    {
        f3_t b = v2[ i * stride ];
        v2[ i * stride ] = b * grt->c - v1[ i * stride ] * grt->s;
        v1[ i * stride ] = b * grt->s + v1[ i * stride ] * grt->c;
    }
}

//----------------------------------------------------------------------------------------------------------------------

#ifdef BMATH_AVX

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
#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

void bmath_simd_f3_col_rotate( f3_t* v1, f3_t* v2, sz_t stride, sz_t size, const bmath_grt_f3_s* grt )
{
#ifdef BMATH_AVX
    bmath_simd_f3_col_rotate_avx( v1, v2, stride, size, grt );
#else
    bmath_simd_f3_col_rotate_default( v1, v2, stride, size, grt );
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

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

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_grt_f3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_grt" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_grt_f3_s );
            BCORE_REGISTER_OBJECT( bmath_arr_grt_f3_s );
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

