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
            BCATU(bmath_cfx_s,add_mul)( &sum, &src[ i ], &w, &sum );
            BCATU(bmath_cfx_s,mul)( &w, &ws1, &w );
        }
        dst[ j ] = sum;
        BCATU(bmath_cfx_s,mul)( &ws1, &ws0, &ws1 );
    }
}

/**********************************************************************************************************************/

/** Efficient flavor of the recursive Cooley Turkey fft algorithm.
 *    for n0 > 4: buf must be preallocated to size n0.
 *    buf == src is allowed.
 */
void BCATU(bmath_fourier,fx,rct_fft)( const bmath_cfx_s* src, bmath_cfx_s* dst, uz_t n0, bmath_cfx_s* buf )
{
    if( n0 == 4 )
    {
        // attempt to further disentangle appears not to speed up on gcc -O3
        dst[ 0 ].v[ 0 ] = src[ 0 ].v[ 0 ] + src[ 1 ].v[ 0 ] + src[ 2 ].v[ 0 ] + src[ 3 ].v[ 0 ];
        dst[ 0 ].v[ 1 ] = src[ 0 ].v[ 1 ] + src[ 1 ].v[ 1 ] + src[ 2 ].v[ 1 ] + src[ 3 ].v[ 1 ];
        dst[ 2 ].v[ 0 ] = src[ 0 ].v[ 0 ] - src[ 1 ].v[ 0 ] + src[ 2 ].v[ 0 ] - src[ 3 ].v[ 0 ];
        dst[ 2 ].v[ 1 ] = src[ 0 ].v[ 1 ] - src[ 1 ].v[ 1 ] + src[ 2 ].v[ 1 ] - src[ 3 ].v[ 1 ];
        dst[ 1 ].v[ 0 ] = src[ 0 ].v[ 0 ] + src[ 1 ].v[ 1 ] - src[ 2 ].v[ 0 ] - src[ 3 ].v[ 1 ];
        dst[ 1 ].v[ 1 ] = src[ 0 ].v[ 1 ] - src[ 1 ].v[ 0 ] - src[ 2 ].v[ 1 ] + src[ 3 ].v[ 0 ];
        dst[ 3 ].v[ 0 ] = src[ 0 ].v[ 0 ] - src[ 1 ].v[ 1 ] - src[ 2 ].v[ 0 ] + src[ 3 ].v[ 1 ];
        dst[ 3 ].v[ 1 ] = src[ 0 ].v[ 1 ] + src[ 1 ].v[ 0 ] - src[ 2 ].v[ 1 ] - src[ 3 ].v[ 0 ];
        return;
    }

    uz_t n1 = n0 >> 1;

    bmath_cfx_s ws0 = BCATU(bmath,cfx,init_urt)( -1, n0 );
    bmath_cfx_s w0  = BCATU(bmath,cfx,one)();

    for( uz_t i = 0; i < n1; i++ )
    {
        BCATU(bmath_cfx_s,add)( &src[ i ], &src[ i + n1 ], &dst[ i ] );
        BCATU(bmath_cfx_s,mul_sub)( &w0, &src[ i ], &src[ i + n1 ], &buf[ i ] );
        BCATU(bmath_cfx_s,mul)( &w0, &ws0, &w0 );
    }

    BCATU(bmath_fourier,fx,rct_fft)( buf, dst + n1, n1, buf + n1 );
    BCATU(bmath_fourier,fx,rct_fft)( dst, buf,      n1, buf + n1 );

    for( uz_t i = 0; i < n1; i++ )
    {
        dst[ 2 * i     ] = buf[ i ];
        dst[ 2 * i + 1 ] = dst[ i + n1 ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

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
        BCATU(bmath_fourier,fx,rct_fft)( buf_l, dst, size, buf_l );
    }
    else
    {
        BCATU(bmath_fourier,fx,rct_fft)( src, dst, size, buf_l );
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
    st_s* log = st_s_createf( "== bmath_fourier_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    typedef struct complex_vec_s { aware_t _; bmath_cfx_s* data; uz_t size; uz_t space; } complex_vec_s;

    #if BMATH_TEMPLATE_FX_PREC == 2
        tp_t TYPEOF_complex_vec_s = bcore_flect_type_parse_sc( "{ aware_t _; bmath_cf2_s [] arr; }" );
    #else
        tp_t TYPEOF_complex_vec_s = bcore_flect_type_parse_sc( "{ aware_t _; bmath_cf3_s [] arr; }" );
    #endif // BMATH_TEMPLATE_FX_PREC

    /// data test
    bcore_life_s* l = bcore_life_s_create();
    {
        uz_t size = 128;

        bmath_cfx_s z;

        complex_vec_s* vec1 = bcore_life_s_push_aware( l, bcore_inst_t_create( TYPEOF_complex_vec_s ) );
        complex_vec_s* vec2 = bcore_life_s_push_aware( l, bcore_inst_t_create( TYPEOF_complex_vec_s ) );
        complex_vec_s* vec3 = bcore_life_s_push_aware( l, bcore_inst_t_create( TYPEOF_complex_vec_s ) );
        bcore_array_a_set_size( (bcore_array*)vec1, size );
        bcore_array_a_set_size( (bcore_array*)vec2, size );
        bcore_array_a_set_size( (bcore_array*)vec3, size );

        u2_t rval = 1234;

        for( uz_t i = 0; i < size; i++ )
        {
            fx_t re = BCATU(fx,xsg1_sym)( &rval );
            fx_t im = BCATU(fx,xsg1_sym)( &rval );
            vec1->data[ i ] = BCATU(bmath,cfx,init)( re, im );
        }

        /// vec2 = DFT( vec1 )
        BCATU(bmath_fourier,fx,dft)( vec1->data, vec2->data, size );
        /// vec3 = FFT( vec1 )
        BCATU(bmath_fourier,fx,fft)( vec1->data, vec3->data, size );

        bmath_vector_a_sub_sqr( ( const bmath_vector* )vec2, ( const bmath_vector* )vec3, ( bmath_ring* )&z );
        ASSERT( BCATU(bmath,cfx,mag)( z ) < 1e-14 );

        /// vec2 = inverse FFT( vec1 )
        bcore_array_a_do( (bcore_array*)vec2, 0, -1, ( fp_t )BCATU(bmath_cfx_s,self_cnj) );
        BCATU(bmath_fourier,fx,fft)( vec2->data, vec2->data, size );
        bcore_array_a_do( (bcore_array*)vec2, 0, -1, ( fp_t )BCATU(bmath_cfx_s,self_cnj) );
        bmath_cfx_s f = BCATU(bmath,cfx,init)( 1.0 / size, 0 );
        bmath_vector_a_mul_scl( ( const bmath_vector* )vec2, ( const bmath_ring* )&f, ( bmath_vector* )vec2 );

        /// compare vec1, vec 2
        bmath_vector_a_sub_sqr( ( const bmath_vector* )vec1, ( const bmath_vector* )vec2, ( bmath_ring* )&z );
        ASSERT( BCATU(bmath,cfx,mag)( z ) < 1e-14 );
    }

    /// speed test
    {
        uz_t size = 1 << 20;

        complex_vec_s* vec1 = bcore_life_s_push_aware( l, bcore_inst_t_create( TYPEOF_complex_vec_s ) );
        complex_vec_s* vec2 = bcore_life_s_push_aware( l, bcore_inst_t_create( TYPEOF_complex_vec_s ) );
        bcore_array_a_set_size( (bcore_array*)vec1, size );
        bcore_array_a_set_size( (bcore_array*)vec2, size );

        u2_t rval = 1234;

        for( uz_t i = 0; i < size; i++ )
        {
            fx_t re = BCATU(fx,xsg1_sym)( &rval );
            fx_t im = BCATU(fx,xsg1_sym)( &rval );
            vec1->data[ i ] = BCATU(bmath,cfx,init)( re, im );
        }

        clock_t time;

        time = clock();
        BCATU(bmath_fourier,fx,fft)( vec1->data, vec2->data, size );
        time = clock() - time;
        st_s_push_fa( log, "fft ... #<uz_t>ms\n", ( uz_t ) ( ( 1E3 * ( double )( time )/CLOCKS_PER_SEC ) ) );

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

