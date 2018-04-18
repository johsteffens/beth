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

#include "bmath_fourier.h"
#include "bmath_spect_vector.h"

/**********************************************************************************************************************/

void bmath_fourier_dft_f3( bmath_cf3_s* dst, const bmath_cf3_s* src, sz_t size )
{
    if( size == 0 ) return;
    if( dst == src )
    {
        bmath_cf3_s* src_l = bcore_malloc( sizeof( bmath_cf3_s ) * size );
        bcore_memcpy( src_l, src, sizeof( bmath_cf3_s ) * size );
        bmath_fourier_dft_f3( dst, src_l, size );
        bcore_free( src_l );
        return;
    }

    bmath_cf3_s ws0 = bmath_cf3_init_urt( -1, size );
    bmath_cf3_s ws1 = bmath_cf3_one();

    for( sz_t j = 0; j < size; j++ )
    {
        bmath_cf3_s w   = bmath_cf3_one();
        bmath_cf3_s sum = bmath_cf3_zro();
        for( sz_t i = 0; i < size; i++ )
        {
            bmath_cf3_s_add_mul( &sum, &sum, &src[ i ], &w );
            bmath_cf3_s_mul( &w, &w, &ws1 );
        }
        dst[ j ] = sum;
        bmath_cf3_s_mul( &ws1, &ws1, &ws0 );
    }
}

/**********************************************************************************************************************/

// for n0 > 4: buf must be preallocated to size n0;
// buf == src is allowed
void bmath_fourier_fft_f3_buf( bmath_cf3_s* dst, const bmath_cf3_s* src, sz_t n0, bmath_cf3_s* buf )
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

    sz_t n1 = n0 >> 1;

    bmath_cf3_s ws0 = bmath_cf3_init_urt( -1, n0 );
    bmath_cf3_s w0  = bmath_cf3_one();
    for( sz_t i = 0; i < n1; i++ )
    {
        bmath_cf3_s_add( &dst[ i ], &src[ i ], &src[ i + n1 ] );
        bmath_cf3_s_mul_sub( &buf[ i ], &w0, &src[ i ], &src[ i + n1 ] );
        bmath_cf3_s_mul( &w0, &w0, &ws0 );
    }
    bmath_fourier_fft_f3_buf( dst + n1, buf, n1, buf + n1 );
    bmath_fourier_fft_f3_buf( buf,      dst, n1, buf + n1 );

    for( sz_t i = 0; i < n1; i++ )
    {
        dst[ 2 * i     ] = buf[ i ];
        dst[ 2 * i + 1 ] = dst[ i + n1 ];
    }
}

void bmath_fourier_fft_f3( bmath_cf3_s* dst, const bmath_cf3_s* src, sz_t n0 )
{
    if( n0 <= 2 )
    {
        if( n0 == 0 ) return;

        if( n0 == 1 )
        {
            dst[ 0 ] = src[ 0 ];
            return;
        }

        dst[ 0 ].v[ 0 ] = src[ 0 ].v[ 0 ] + src[ 1 ].v[ 0 ];
        dst[ 0 ].v[ 1 ] = src[ 0 ].v[ 1 ] + src[ 1 ].v[ 1 ];
        dst[ 1 ].v[ 0 ] = src[ 0 ].v[ 0 ] - src[ 1 ].v[ 0 ];
        dst[ 1 ].v[ 1 ] = src[ 0 ].v[ 1 ] - src[ 1 ].v[ 1 ];
        return;
    }

    ASSERT( ( n0 & ( n0 - 1 ) ) == 0 ); // size must be power of two
    bmath_cf3_s* buf = bcore_malloc( sizeof( bmath_cf3_s ) * n0 );
    if( src == dst )
    {
        bcore_memcpy( buf, src, sizeof( bmath_cf3_s ) * n0 );
        bmath_fourier_fft_f3_buf( dst, buf, n0, buf );
    }
    else
    {
        bmath_fourier_fft_f3_buf( dst, src, n0, buf );
    }
    bcore_free( buf );
}

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    st_s* log = st_s_createf( "== bmath_fourier_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    typedef struct complex_vec_s { aware_t _; bmath_cf3_s* data; sz_t size; sz_t space; } complex_vec_s;
    tp_t TYPEOF_complex_vec_s = bcore_flect_type_parse_sc( "{ aware_t _; bmath_cf3_s [] arr; }" );

    /// data test
    bcore_life_s* l = bcore_life_s_create();
    {
        sz_t size = 128;

        bmath_cf3_s z;

        complex_vec_s* vec1 = bcore_life_s_push_aware( l, bcore_inst_typed_create( TYPEOF_complex_vec_s ) );
        complex_vec_s* vec2 = bcore_life_s_push_aware( l, bcore_inst_typed_create( TYPEOF_complex_vec_s ) );
        complex_vec_s* vec3 = bcore_life_s_push_aware( l, bcore_inst_typed_create( TYPEOF_complex_vec_s ) );
        bcore_array_aware_set_size( vec1, size );
        bcore_array_aware_set_size( vec2, size );
        bcore_array_aware_set_size( vec3, size );

        u2_t rval = 1234;

        for( sz_t i = 0; i < size; i++ )
        {
            f3_t re = f3_xsg1_sym( &rval );
            f3_t im = f3_xsg1_sym( &rval );
            vec1->data[ i ] = bmath_cf3_init( re, im );
        }

    //    bcore_txt_ml_to_stdout( sr_awc( vec1 ) );

        /// vec2 = DFT( vec1 )
        bmath_fourier_dft_f3( vec2->data, vec1->data, size );
        /// vec3 = FFT( vec1 )
        bmath_fourier_fft_f3( vec3->data, vec1->data, size );

    //    bcore_txt_ml_to_stdout( sr_awc( vec2 ) );
    //    bcore_txt_ml_to_stdout( sr_awc( vec3 ) );
        bmath_vector_aware_sqr_sub( &z, vec2, vec3 );
        ASSERT( bmath_cf3_mag( z ) < 1e-14 );

        /// vec2 = inverse FFT( vec1 )
        bcore_array_aware_do( vec2, 0, -1, ( fp_t )bmath_cf3_s_self_cnj );
        bmath_fourier_fft_f3( vec2->data, vec2->data, size );
        bcore_array_aware_do( vec2, 0, -1, ( fp_t )bmath_cf3_s_self_cnj );
        bmath_cf3_s f = bmath_cf3_init( 1.0 / size, 0 );
        bmath_vector_aware_mul( vec2, vec2, &f );

        /// compare vec1, vec 2
        bmath_vector_aware_sqr_sub( &z, vec1, vec2 );
    //    printf( "%g\n", bmath_cf3_mag( z ) );
        ASSERT( bmath_cf3_mag( z ) < 1e-14 );
    }

    /// speed test
    {
        sz_t size = 1 << 20;

        complex_vec_s* vec1 = bcore_life_s_push_aware( l, bcore_inst_typed_create( TYPEOF_complex_vec_s ) );
        complex_vec_s* vec2 = bcore_life_s_push_aware( l, bcore_inst_typed_create( TYPEOF_complex_vec_s ) );
        bcore_array_aware_set_size( vec1, size );
        bcore_array_aware_set_size( vec2, size );

        u2_t rval = 1234;

        for( sz_t i = 0; i < size; i++ )
        {
            f3_t re = f3_xsg1_sym( &rval );
            f3_t im = f3_xsg1_sym( &rval );
            vec1->data[ i ] = bmath_cf3_init( re, im );
        }

        clock_t time;

        time = clock();
        bmath_fourier_fft_f3( vec2->data, vec1->data, size );
        time = clock() - time;
        st_s_push_fa( log, "fft ... #<sz_t>ms\n", ( sz_t ) ( ( 1E3 * ( double )( time )/CLOCKS_PER_SEC ) ) );

    }

    bcore_life_s_discard( l );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/

vd_t bmath_fourier_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_fourier" ) ) )
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

