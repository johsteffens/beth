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

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/
/// bmath_grt_fx_s

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_grt_fx_s,to_stdout)( const bmath_grt_fx_s* o )
{
    bcore_msg_fa( "c=#<"BSTR(fx_t)"> s=#<"BSTR(fx_t)">\n", o->c, o->s );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// bmath_arr_grt_fx_s

//----------------------------------------------------------------------------------------------------------------------

bmath_arr_grt_fx_s* BCATU(bmath_arr_grt_fx_s,create_size)( uz_t size )
{
    bmath_arr_grt_fx_s* o = BCATU(bmath_arr_grt_fx_s,create)();
    BCATU(bmath_arr_grt_fx_s,set_size)( o, size );
    BCATU(bmath_arr_grt_fx_s,zro)( o );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_arr_grt_fx_s BCATU(bmath_arr_grt,fx,of_size)( uz_t size )
{
    bmath_arr_grt_fx_s o;
    BCATU(bmath_arr_grt_fx_s,init)( &o );
    BCATU(bmath_arr_grt_fx_s,set_size)( &o, size );
    BCATU(bmath_arr_grt_fx_s,zro)( &o );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// simd optimized code

void BCATU(bmath_simd,fx,row_rotate_default)( fx_t* v1, fx_t* v2, sz_t size, const bmath_grt_fx_s* grt )
{
    for( sz_t i = 0; i < size; i++ )
    {
        fx_t b = v2[ i ];
        v2[ i ] = b * grt->c - v1[ i ] * grt->s;
        v1[ i ] = b * grt->s + v1[ i ] * grt->c;
    }
}

#if (defined BMATH_AVX) && (BMATH_TEMPLATE_FX_PREC == 3)

void BCATU(bmath_simd,fx,row_rotate_avx)( fx_t* v1, fx_t* v2, sz_t size, const bmath_grt_fx_s* grt )
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
        fx_t b = v2[ i ];
        v2[ i ] = b * grt->c - v1[ i ] * grt->s;
        v1[ i ] = b * grt->s + v1[ i ] * grt->c;
    }
}

#endif // BMATH_AVX

void BCATU(bmath_simd,fx,row_rotate)( fx_t* v1, fx_t* v2, sz_t size, const bmath_grt_fx_s* grt )
{
#if (defined BMATH_AVX) && (BMATH_TEMPLATE_FX_PREC == 3)
    BCATU(bmath_simd,fx,row_rotate_avx)( v1, v2, size, grt );
#else
    BCATU(bmath_simd,fx,row_rotate_default)( v1, v2, size, grt );
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_simd,fx,col_rotate_default)( fx_t* v1, fx_t* v2, sz_t stride, sz_t size, const bmath_grt_fx_s* grt )
{
    for( sz_t i = 0; i < size; i++ )
    {
        fx_t b = v2[ i * stride ];
        v2[ i * stride ] = b * grt->c - v1[ i * stride ] * grt->s;
        v1[ i * stride ] = b * grt->s + v1[ i * stride ] * grt->c;
    }
}

//----------------------------------------------------------------------------------------------------------------------

#if (defined BMATH_AVX) && (BMATH_TEMPLATE_FX_PREC == 3)

void BCATU(bmath_simd,fx,col_rotate_avx)( fx_t* v1, fx_t* v2, sz_t stride, sz_t size, const bmath_grt_fx_s* grt )
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
        fx_t b = v2[ i * stride ];
        v2[ i * stride ] = b * grt->c - v1[ i * stride ] * grt->s;
        v1[ i * stride ] = b * grt->s + v1[ i * stride ] * grt->c;
    }
}
#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_simd,fx,col_rotate)( fx_t* v1, fx_t* v2, sz_t stride, sz_t size, const bmath_grt_fx_s* grt )
{
#if (defined BMATH_AVX) && (BMATH_TEMPLATE_FX_PREC == 3)
    BCATU(bmath_simd,fx,col_rotate_avx)( v1, v2, stride, size, grt );
#else
    BCATU(bmath_simd,fx,col_rotate_default)( v1, v2, stride, size, grt );
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_simd,fx,drow_swipe_rev)( fx_t* row, const bmath_grt_fx_s* grt, sz_t size )
{
    fx_t v = row[ 0 ];
    sz_t i;
    for( i = size; i > 4; i -= 4 )
    {
        BCATU(bmath_grt_fx_s,rotate)( grt + i - 1, &v, row + i - 0 );
        BCATU(bmath_grt_fx_s,rotate)( grt + i - 2, &v, row + i - 1 );
        BCATU(bmath_grt_fx_s,rotate)( grt + i - 3, &v, row + i - 2 );
        BCATU(bmath_grt_fx_s,rotate)( grt + i - 4, &v, row + i - 3 );
    }

    for( ; i > 0; i-- ) BCATU(bmath_grt_fx_s,rotate)( grt + i - 1, &v, row + i );

    row[ 0 ] = v;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_simd,fx,4drow_swipe_rev)( fx_t* row, sz_t stride, const bmath_grt_fx_s* grt, sz_t size )
{
    fx_t a[ 4 ];

    a[ 0 ] = row[ stride * 0 ];
    a[ 1 ] = row[ stride * 1 ];
    a[ 2 ] = row[ stride * 2 ];
    a[ 3 ] = row[ stride * 3 ];

    for( sz_t i = size; i > 0; i-- )
    {
        BCATU(bmath_grt_fx_s,rotate)( grt + i - 1, a + 0, row + stride * 0 + i );
        BCATU(bmath_grt_fx_s,rotate)( grt + i - 1, a + 1, row + stride * 1 + i );
        BCATU(bmath_grt_fx_s,rotate)( grt + i - 1, a + 2, row + stride * 2 + i );
        BCATU(bmath_grt_fx_s,rotate)( grt + i - 1, a + 3, row + stride * 3 + i );
    }

    row[ stride * 0 ] = a[ 0 ];
    row[ stride * 1 ] = a[ 1 ];
    row[ stride * 2 ] = a[ 2 ];
    row[ stride * 3 ] = a[ 3 ];
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

