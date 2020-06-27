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

/** Discrete Fourier Transform.
 *  DFT: Canonic implementation: F_k = Sum_j( f_j * w^jk ); w := e^-i2pi/n
 *  FFT: Fast transform (on power 2 sizes) (numerically equivalent to DFT)
 *  To obtain the inverse transform:
 *      - Conjugate src before FT.
 *      - Conjugate dst after FT and scale with 1.0 / size.
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_fourier,fx,dft)( const bmath_cfx_s* src, bmath_cfx_s* dst, uz_t size )
{
    if( size == 0 ) return;
    if( dst == src )
    {
        bmath_cfx_s* src_l = bcore_malloc( sizeof( bmath_cfx_s ) * size );
        bcore_memcpy( src_l, src, sizeof( bmath_cfx_s ) * size );
        BCATU(bmath_fourier,fx,dft)( src_l, dst, size );
        bcore_free( src_l );
        return;
    }

    bmath_cfx_s ws0 = BCATU(bmath,cfx,init_urt)( -1, size );
    bmath_cfx_s ws1 = BCATU(bmath,cfx,one)();

    for( uz_t j = 0; j < size; j++ )
    {
        bmath_cfx_s w   = BCATU(bmath,cfx,one)();
        bmath_cfx_s sum = BCATU(bmath,cfx,zro)();
        for( uz_t i = 0; i < size; i++ )
        {
            BCATU(bmath_cfx_s,add_prod)( &sum, &src[ i ], &w, &sum );
            BCATU(bmath_cfx_s,mul)( &w, &ws1, &w );
        }
        dst[ j ] = sum;
        BCATU(bmath_cfx_s,mul)( &ws1, &ws0, &ws1 );
    }
}

/**********************************************************************************************************************/
// using arrays of bmath_cfx_s

//----------------------------------------------------------------------------------------------------------------------

static void fft_4( const bmath_cfx_s* src, bmath_cfx_s* dst )
{
    #ifdef BMATH_AVX
        #if ( BMATH_TEMPLATE_FX_PREC == 2 )
            __m128 dst0_m5 = _mm_add_ps( _mm_setr_ps( src[ 0 ].v[ 0 ], src[ 0 ].v[ 1 ],  src[ 0 ].v[ 0 ],  src[ 0 ].v[ 1 ] ),
                                         _mm_setr_ps( src[ 2 ].v[ 0 ], src[ 2 ].v[ 1 ], -src[ 2 ].v[ 0 ], -src[ 2 ].v[ 1 ] ) );
            __m128 dst1_m5 = _mm_add_ps( _mm_setr_ps( src[ 1 ].v[ 0 ], src[ 1 ].v[ 1 ],  src[ 1 ].v[ 1 ], -src[ 1 ].v[ 0 ] ),
                                         _mm_setr_ps( src[ 3 ].v[ 0 ], src[ 3 ].v[ 1 ], -src[ 3 ].v[ 1 ], +src[ 3 ].v[ 0 ] ) );
            _mm_storeu_ps( ( ( fx_t* )dst ),     _mm_add_ps( dst0_m5, dst1_m5 ) );
            _mm_storeu_ps( ( ( fx_t* )dst ) + 4, _mm_sub_ps( dst0_m5, dst1_m5 ) );
        #else
            __m256d dst0_m5 = M5_ADD( _mm256_setr_pd( src[ 0 ].v[ 0 ], src[ 0 ].v[ 1 ],  src[ 0 ].v[ 0 ],  src[ 0 ].v[ 1 ] ),
                                      _mm256_setr_pd( src[ 2 ].v[ 0 ], src[ 2 ].v[ 1 ], -src[ 2 ].v[ 0 ], -src[ 2 ].v[ 1 ] ) );
            __m256d dst1_m5 = M5_ADD( _mm256_setr_pd( src[ 1 ].v[ 0 ], src[ 1 ].v[ 1 ],  src[ 1 ].v[ 1 ], -src[ 1 ].v[ 0 ] ),
                                      _mm256_setr_pd( src[ 3 ].v[ 0 ], src[ 3 ].v[ 1 ], -src[ 3 ].v[ 1 ], +src[ 3 ].v[ 0 ] ) );
            M5_STOR( ( ( fx_t* )dst ),     M5_ADD( dst0_m5, dst1_m5 ) );
            M5_STOR( ( ( fx_t* )dst ) + 4, M5_SUB( dst0_m5, dst1_m5 ) );

        #endif
    #else
        dst[ 0 ].v[ 0 ] = src[ 0 ].v[ 0 ] + src[ 1 ].v[ 0 ] + src[ 2 ].v[ 0 ] + src[ 3 ].v[ 0 ];
        dst[ 0 ].v[ 1 ] = src[ 0 ].v[ 1 ] + src[ 1 ].v[ 1 ] + src[ 2 ].v[ 1 ] + src[ 3 ].v[ 1 ];
        dst[ 1 ].v[ 0 ] = src[ 0 ].v[ 0 ] + src[ 1 ].v[ 1 ] - src[ 2 ].v[ 0 ] - src[ 3 ].v[ 1 ];
        dst[ 1 ].v[ 1 ] = src[ 0 ].v[ 1 ] - src[ 1 ].v[ 0 ] - src[ 2 ].v[ 1 ] + src[ 3 ].v[ 0 ];
        dst[ 2 ].v[ 0 ] = src[ 0 ].v[ 0 ] - src[ 1 ].v[ 0 ] + src[ 2 ].v[ 0 ] - src[ 3 ].v[ 0 ];
        dst[ 2 ].v[ 1 ] = src[ 0 ].v[ 1 ] - src[ 1 ].v[ 1 ] + src[ 2 ].v[ 1 ] - src[ 3 ].v[ 1 ];
        dst[ 3 ].v[ 0 ] = src[ 0 ].v[ 0 ] - src[ 1 ].v[ 1 ] - src[ 2 ].v[ 0 ] + src[ 3 ].v[ 1 ];
        dst[ 3 ].v[ 1 ] = src[ 0 ].v[ 1 ] + src[ 1 ].v[ 0 ] - src[ 2 ].v[ 1 ] - src[ 3 ].v[ 0 ];
        // further disentangling shows no speed advantage on gcc -O3
    #endif
}

//----------------------------------------------------------------------------------------------------------------------

/** Efficient flavor of the recursive Cooley Tukey fft algorithm. SIMD optimized.
 *    for n0 >= 4: buf must be preallocated to size n0.
 *    buf == src is allowed.
 */
static void recursive_fft( const bmath_cfx_s* src, bmath_cfx_s* dst, uz_t n0, bmath_cfx_s* buf )
{
    uz_t n1 = n0 >> 1;

    bmath_cfx_s ws0 = BCATU(bmath,cfx,init_urt)( -1, n0 );
    bmath_cfx_s w0  = BCATU(bmath,cfx,one)();

    #ifdef BMATH_AVX2

        assert( n1 >= 4 && ( n1 & 3 ) == 0 );

        #if ( BMATH_TEMPLATE_FX_PREC == 2 )
            #define BMATH_COND_NO_SIMD ( n1 == 4 )
        #else
            #define BMATH_COND_NO_SIMD false
        #endif

        // reasonable compiler optimization will skip this branch entirely for FX_PREC == 3
        if( BMATH_COND_NO_SIMD )
        {
            for( sz_t i = 0; i < 4; i++ )
            {
                BCATU(bmath_cfx_s,add)( &src[ i ], &src[ i + n1 ], &dst[ i ] );
                BCATU(bmath_cfx_s,mul_diff)( &w0, &src[ i ], &src[ i + n1 ], &buf[ i ] );
                BCATU(bmath_cfx_s,mul)( &w0, &ws0, &w0 );
            }
        }
        else
        {
            #if ( BMATH_TEMPLATE_FX_PREC == 2 )
                __m256i mask_eve = _mm256_setr_epi32( -1, 0, -1, 0, -1, 0, -1, 0 );
                __m256i mask_odd = _mm256_setr_epi32( 0, -1, 0, -1, 0, -1, 0, -1 );
                __m256i idx_r    = _mm256_setr_epi32( 0, 8, 2, 10, 4, 12, 6, 14 );
                __m256i idx_i    = _mm256_setr_epi32( 1, 9, 3, 11, 5, 13, 7, 15 );
            #else
                __m128i idx_r = _mm_setr_epi32( 0, 4, 2, 6 );
                __m128i idx_i = _mm_setr_epi32( 1, 5, 3, 7 );
            #endif

            M5_T w0_r;
            M5_T w0_i;

            {
                bmath_cfx_s wb[ P5_SIZE ];
                for( sz_t i = 0; i < P5_SIZE; i++ )
                {
                    wb[ i ] = w0;
                    BCATU(bmath_cfx_s,mul)( &w0, &ws0, &w0 );
                }
                w0_r = M5_GATHER( ( fx_t* )wb, idx_r, sizeof( fx_t ) );
                w0_i = M5_GATHER( ( fx_t* )wb, idx_i, sizeof( fx_t ) );
            }

            M5_T ws0_r = M5_SET_ALL( w0.v[ 0 ] );
            M5_T ws0_i = M5_SET_ALL( w0.v[ 1 ] );

            for( sz_t i = 0; i < n1; i += P5_SIZE )
            {
                M5_T src_0_r = M5_GATHER( ( fx_t* )&src[ i      ], idx_r, sizeof( fx_t ) );
                M5_T src_0_i = M5_GATHER( ( fx_t* )&src[ i      ], idx_i, sizeof( fx_t ) );
                M5_T src_n_r = M5_GATHER( ( fx_t* )&src[ i + n1 ], idx_r, sizeof( fx_t ) );
                M5_T src_n_i = M5_GATHER( ( fx_t* )&src[ i + n1 ], idx_i, sizeof( fx_t ) );
                M5_T dst_r   = M5_ADD( src_0_r, src_n_r );
                M5_T dst_i   = M5_ADD( src_0_i, src_n_i );

                #if ( BMATH_TEMPLATE_FX_PREC == 2 )
                    _mm256_maskstore_ps( ( ( fx_t* )dst ) + i * 2,     mask_eve, dst_r );
                    _mm256_maskstore_ps( ( ( fx_t* )dst ) + i * 2 + 1, mask_eve, dst_i );
                    _mm256_maskstore_ps( ( ( fx_t* )dst ) + i * 2 + 7, mask_odd, dst_r );
                    _mm256_maskstore_ps( ( ( fx_t* )dst ) + i * 2 + 8, mask_odd, dst_i );
                #else
                    M5_STOR( ( fx_t* )&dst[ i     ], _mm256_shuffle_pd( dst_r, dst_i, 0x0 ) );
                    M5_STOR( ( fx_t* )&dst[ i + 2 ], _mm256_shuffle_pd( dst_r, dst_i, 0xF ) );
                #endif

                M5_T tmp_r = M5_SUB( src_0_r, src_n_r );
                M5_T tmp_i = M5_SUB( src_0_i, src_n_i );

                dst_r = M5_MUL_SUB( tmp_r, w0_r, M5_MUL( tmp_i, w0_i ) );
                dst_i = M5_MUL_ADD( tmp_r, w0_i, M5_MUL( tmp_i, w0_r ) );

                #if ( BMATH_TEMPLATE_FX_PREC == 2 )
                    _mm256_maskstore_ps( ( ( fx_t* )buf ) + i * 2,     mask_eve, dst_r );
                    _mm256_maskstore_ps( ( ( fx_t* )buf ) + i * 2 + 1, mask_eve, dst_i );
                    _mm256_maskstore_ps( ( ( fx_t* )buf ) + i * 2 + 7, mask_odd, dst_r );
                    _mm256_maskstore_ps( ( ( fx_t* )buf ) + i * 2 + 8, mask_odd, dst_i );
                #else
                    M5_STOR( ( fx_t* )&buf[ i     ], _mm256_shuffle_pd( dst_r, dst_i, 0x0 ) );
                    M5_STOR( ( fx_t* )&buf[ i + 2 ], _mm256_shuffle_pd( dst_r, dst_i, 0xF ) );
                #endif

                tmp_r = M5_MUL_SUB( ws0_r, w0_r, M5_MUL( ws0_i, w0_i ) );
                tmp_i = M5_MUL_ADD( ws0_r, w0_i, M5_MUL( ws0_i, w0_r ) );

                w0_r = tmp_r;
                w0_i = tmp_i;
            }
        }

    #else // no simd

        for( sz_t i = 0; i < n1; i++ )
        {
            BCATU(bmath_cfx_s,add)( &src[ i ], &src[ i + n1 ], &dst[ i ] );
            BCATU(bmath_cfx_s,mul_diff)( &w0, &src[ i ], &src[ i + n1 ], &buf[ i ] );
            BCATU(bmath_cfx_s,mul)( &w0, &ws0, &w0 );
        }

    #endif

    if( n1 == 4 )
    {
        fft_4( buf, dst + n1 );
        fft_4( dst, buf      );
    }
    else
    {
        recursive_fft( buf, dst + n1, n1, buf + n1 );
        recursive_fft( dst, buf,      n1, buf + n1 );
    }

    for( uz_t i = 0; i < n1; i++ )
    {
        dst[ 2 * i     ] = buf[ i ];
        dst[ 2 * i + 1 ] = dst[ i + n1 ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** Note on splitting complex data:
 *  Splitting complex array into separate arrays or real and imaginary values is sometimes recommended because it needs
 *  fewer SIMD operations (no gathering, unpacking). I found experimentally, though, that extra cost due to splitting, merging
 *  and cache misses leaves no net benefit altogether.
 */
vd_t BCATU(bmath_fourier,fx,fft_buf)( const bmath_cfx_s* src, bmath_cfx_s* dst, uz_t size, vd_t buf )
{
    if( size <= 2 )
    {
        if( size == 0 ) return NULL;

        if( size == 1 )
        {
            dst[ 0 ] = src[ 0 ];
            return NULL;
        }

        dst[ 0 ].v[ 0 ] = src[ 0 ].v[ 0 ] + src[ 1 ].v[ 0 ];
        dst[ 0 ].v[ 1 ] = src[ 0 ].v[ 1 ] + src[ 1 ].v[ 1 ];
        dst[ 1 ].v[ 0 ] = src[ 0 ].v[ 0 ] - src[ 1 ].v[ 0 ];
        dst[ 1 ].v[ 1 ] = src[ 0 ].v[ 1 ] - src[ 1 ].v[ 1 ];
        return NULL;
    }

    ASSERT( ( size & ( size - 1 ) ) == 0 ); // size must be power of two
    bmath_cfx_s* buf_l = ( buf ) ? buf : bcore_malloc( sizeof( bmath_cfx_s ) * size );

    if( src == dst )
    {
        bcore_memcpy( buf_l, src, sizeof( bmath_cfx_s ) * size );
        if( size == 4 )
        {
            fft_4( buf_l, dst );
        }
        else
        {
            recursive_fft( buf_l, dst, size, buf_l );
        }
    }
    else
    {
        if( size == 4 )
        {
            fft_4( src, dst );
        }
        else
        {
            recursive_fft( src, dst, size, buf_l );
        }
    }

    return buf_l;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_fourier,fx,fft)( const bmath_cfx_s* src, bmath_cfx_s* dst, uz_t size )
{
    bcore_free( BCATU(bmath_fourier,fx,fft_buf)( src, dst, size, NULL ) );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static vd_t fourier_selftest( void )
{
    st_s* log = st_s_create_fa( "== Fourier_selftest: '#<sc_t>' ", BSTR(bmath_cfx_s) );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    typedef struct complex_vec_s { aware_t _; bmath_cfx_s* data; uz_t size; uz_t space; } complex_vec_s;

    tp_t TYPEOF_complex_vec_s = bcore_flect_type_parse_sc( "{ aware_t _; "BSTR(bmath_cfx_s)" [] arr; }" );

    /// data test
    bcore_life_s* l = bcore_life_s_create();
    {
        sz_t size = 1 << 7;

        bmath_cfx_s z;

        complex_vec_s* vec1 = bcore_life_s_push_aware( l, bcore_inst_t_create( TYPEOF_complex_vec_s ) );
        complex_vec_s* vec2 = bcore_life_s_push_aware( l, bcore_inst_t_create( TYPEOF_complex_vec_s ) );
        complex_vec_s* vec3 = bcore_life_s_push_aware( l, bcore_inst_t_create( TYPEOF_complex_vec_s ) );
        bcore_array_a_set_size( (bcore_array*)vec1, size );
        bcore_array_a_set_size( (bcore_array*)vec2, size );
        bcore_array_a_set_size( (bcore_array*)vec3, size );

        u3_t rval = 1234;

        for( sz_t i = 0; i < size; i++ )
        {
            fx_t re = BCATU(fx,rnd_sym)( &rval );
            fx_t im = BCATU(fx,rnd_sym)( &rval );
            vec1->data[ i ] = BCATU(bmath,cfx,init)( re, im );
        }

        /// vec2 = DFT( vec1 )
        BCATU(bmath_fourier,fx,dft)( vec1->data, vec2->data, size );
        /// vec3 = FFT( vec1 )
        BCATU(bmath_fourier,fx,fft)( vec1->data, vec3->data, size );

        bmath_vector_a_sub_sqr( ( const bmath_vector* )vec2, ( const bmath_vector* )vec3, ( bmath_ring* )&z );
        ASSERT( BCATU(bmath,cfx,mag)( z ) < ( ( sizeof( fx_t ) == 4 ) ? 1e-5 : 1e-14 ) );

        /// vec2 = inverse FFT( vec1 )
        bcore_array_a_do( (bcore_array*)vec2, 0, -1, ( fp_t )BCATU(bmath_cfx_s,self_cnj) );
        BCATU(bmath_fourier,fx,fft)( vec2->data, vec2->data, size );
        bcore_array_a_do( (bcore_array*)vec2, 0, -1, ( fp_t )BCATU(bmath_cfx_s,self_cnj) );
        bmath_cfx_s f = BCATU(bmath,cfx,init)( 1.0 / size, 0 );
        bmath_vector_a_mul_scl( ( const bmath_vector* )vec2, ( const bmath_ring* )&f, ( bmath_vector* )vec2 );

        /// compare vec1, vec 2
        bmath_vector_a_sub_sqr( ( const bmath_vector* )vec1, ( const bmath_vector* )vec2, ( bmath_ring* )&z );
        ASSERT( BCATU(bmath,cfx,mag)( z ) < ( ( sizeof( fx_t ) == 4 ) ? 1e-7 : 1e-14 ) );
    }

    /// speed test
    {
        sz_t size = 1 << 20;

        complex_vec_s* vec1 = bcore_life_s_push_aware( l, bcore_inst_t_create( TYPEOF_complex_vec_s ) );
        complex_vec_s* vec2 = bcore_life_s_push_aware( l, bcore_inst_t_create( TYPEOF_complex_vec_s ) );
        bcore_array_a_set_size( (bcore_array*)vec1, size );
        bcore_array_a_set_size( (bcore_array*)vec2, size );

        u3_t rval = 1234;

        for( sz_t i = 0; i < size; i++ )
        {
            fx_t re = BCATU(fx,rnd_sym)( &rval );
            fx_t im = BCATU(fx,rnd_sym)( &rval );
            vec1->data[ i ] = BCATU(bmath,cfx,init)( re, im );
        }

        clock_t time;

        time = clock();
        BCATU(bmath_fourier,fx,fft)( vec1->data, vec2->data, size );
        time = clock() - time;
        st_s_push_fa( log, "fft ... #<uz_t>ms\n", ( sz_t ) ( ( 1E3 * ( double )( time )/CLOCKS_PER_SEC ) ) );

    }

    bcore_life_s_discard( l );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

