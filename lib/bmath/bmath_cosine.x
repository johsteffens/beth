/** Author and Copyright 2022 Johannes Bernhard Steffens
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

//----------------------------------------------------------------------------------------------------------------------

group :dct
{

    //------------------------------------------------------------------------------------------------------------------

    func (:f3_s) f3_t mul_r( bmath_cf3_s* a, bmath_cf3_s* b ) = a.r * b.r - a.i * b.i;
    func (:f2_s) f2_t mul_r( bmath_cf2_s* a, bmath_cf2_s* b ) = a.r * b.r - a.i * b.i;

    //------------------------------------------------------------------------------------------------------------------

    // DCT2 direct implementation (slow)
    func (:f3_s) df
    {
        f3_t inv_size = size > 0 ? 1.0 / size : 0;
        for( sz_t k = 0; k < size; k++ )
        {
            dst[ k ] = 0;
            for( sz_t n = 0; n < size; n++ ) dst[ k ] += src[ n ] * cos( ( f3_pi() * ( n + 0.5 ) * k ) * inv_size );
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f3_s) dfv
    {
        dst.set_size( src.size );
        o.df( src.data, dst.data, src.size );
    }

    //------------------------------------------------------------------------------------------------------------------

    // inverse of DCT2 direct implementation (slow)  (DCT3 with factor 2/N)
    func (:f3_s) di
    {
        f3_t inv_size = size > 0 ? 1.0 / size : 0;
        for( sz_t k = 0; k < size; k++ )
        {
            dst[ k ] = 0.5 * src[ 0 ];
            for( sz_t n = 1; n < size; n++ ) dst[ k ] += src[ n ] * cos( ( f3_pi() * n * ( k + 0.5 ) ) * inv_size );
            dst[ k ] *= 2.0 * inv_size;
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f3_s) div
    {
        dst.set_size( src.size );
        o.di( src.data, dst.data, src.size );
    }

    //------------------------------------------------------------------------------------------------------------------

    /** DCT Fast Forward Transformation (via FFT)
     *  Transforms size source-samples into size destination samples
     *  src, dst can be the same pointer
     */
    func (:f3_s) ff
    {
        ASSERT( ( ( size - 1 ) & size ) == 0 );
        sz_t n = size * 2;
        o.buf_!.set_size( n * 2 );
        m bmath_cf3_s* dc1 = o.buf_.data + n * 0;
        m bmath_cf3_s* dc2 = o.buf_.data + n * 1;
        for( sz_t i = 0; i < size; i++ ) dc1[ i ]._( src[ i ], 0 );
        for( sz_t i = size; i < n; i++ ) dc1[ i ].zro();
        bmath_fourier_f3_fft_buf( dc1, dc2, n, dc1 );
        bmath_cf3_s^ w0.urt(  0, 2 * n );
        bmath_cf3_s^ w1.urt( -1, 2 * n );
        for( sz_t i = 0; i < size; i++ ) { dst[ i ] = o.mul_r( dc2[ i ], w0 ); w1.mul( w0, w0 ); }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f3_s) ffv
    {
        dst.set_size( src.size );
        o.ff( src.data, dst.data, src.size );
    }

    //------------------------------------------------------------------------------------------------------------------

    /** DCT2 Fast Inverse Transformation (via FFT)  (DCT3 with factor 2/N)
     *  Transforms size source-samples into size destination samples
     *  src, dst can be the same pointer
     */
    func (:f3_s) fi
    {
        ASSERT( ( ( size - 1 ) & size ) == 0 );
        sz_t n = size * 2;
        o.buf_!.set_size( n * 2 );
        m bmath_cf3_s* dc1 = o.buf_.data + n * 0;
        m bmath_cf3_s* dc2 = o.buf_.data + n * 1;
        bmath_cf3_s^ w0.urt(  0, 2 * n );
        bmath_cf3_s^ w1.urt( -1, 2 * n );
        for( sz_t i = 0; i < size; i++ ) { w0.mul_f3( src[ i ], dc1[ i ] ); w1.mul( w0, w0 ); }
        for( sz_t i = size; i < n; i++ ) dc1[ i ].zro();
        bmath_fourier_f3_fft_buf( dc1, dc2, n, dc1 );
        f3_t inv_size = size > 0 ? 1.0 / size : 0;
        f3_t offs = size > 0 ? 0.5 * src[ 0 ] : 0;
        for( sz_t i = 0; i < size; i++ ) dst[ i ] = ( dc2[ i ].r - offs ) * 2 * inv_size;
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f3_s) fiv
    {
        dst.set_size( src.size );
        o.fi( src.data, dst.data, src.size );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f3_s) void selftest()
    {
        sz_t n = 1024;
        bmath_vf3_s^ v1.set_size( n ).set_random( 1.0, -1.0, 1.0, NULL );
        bmath_vf3_s^ v2;
        bmath_vf3_s^ v3;

        :f3_s^ dft;

        dft.dfv( v1, v2 );
        dft.div( v2, v3 );

        {
            f3_t dev = 0;
            for( sz_t i = 0; i < n; i++ ) dev += f3_sqr( v1.[ i ] - v3.[ i ] );
            ASSERT( dev < 1E-15 );
        }

        bmath_vf3_s^ v4;
        bmath_vf3_s^ v5;

        dft.ffv( v1, v4 );

        {
            f3_t dev = 0;
            for( sz_t i = 0; i < n; i++ ) dev += f3_sqr( v2.[ i ] - v4.[ i ] );
            ASSERT( dev < 1E-15 );
        }

        dft.fiv( v4, v5 );

        {
            f3_t dev = 0;
            for( sz_t i = 0; i < n; i++ ) dev += f3_sqr( v1.[ i ] - v5.[ i ] );
            ASSERT( dev < 1E-15 );
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    // DCT2 direct implementation (slow)
    func (:f2_s) df
    {
        f3_t inv_size = size > 0 ? 1.0 / size : 0;
        for( sz_t k = 0; k < size; k++ )
        {
            f3_t sum = 0;
            for( sz_t n = 0; n < size; n++ ) sum += src[ n ] * cos( ( f3_pi() * ( n + 0.5 ) * k ) * inv_size );
            dst[ k ] = sum;
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f2_s) dfv
    {
        dst.set_size( src.size );
        o.df( src.data, dst.data, src.size );
    }

    //------------------------------------------------------------------------------------------------------------------

    // inverse of DCT2 direct implementation (slow)  (DCT3 with factor 2/N)
    func (:f2_s) di
    {
        f3_t inv_size = size > 0 ? 1.0 / size : 0;
        for( sz_t k = 0; k < size; k++ )
        {
            f3_t sum = 0.5 * src[ 0 ];
            for( sz_t n = 1; n < size; n++ ) sum += src[ n ] * cos( ( f3_pi() * n * ( k + 0.5 ) ) * inv_size );
            dst[ k ] = sum * 2.0 * inv_size;
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f2_s) div
    {
        dst.set_size( src.size );
        o.di( src.data, dst.data, src.size );
    }

    //------------------------------------------------------------------------------------------------------------------

    /** DCT Fast Forward Transformation (via FFT)
     *  Transforms size source-samples into size destination samples
     *  src, dst can be the same pointer
     */
    func (:f2_s) ff
    {
        ASSERT( ( ( size - 1 ) & size ) == 0 );
        sz_t n = size * 2;
        o.buf_!.set_size( n * 2 );
        m bmath_cf2_s* dc1 = o.buf_.data + n * 0;
        m bmath_cf2_s* dc2 = o.buf_.data + n * 1;
        for( sz_t i = 0; i < size; i++ ) dc1[ i ]._( src[ i ], 0 );
        for( sz_t i = size; i < n; i++ ) dc1[ i ].zro();
        bmath_fourier_f2_fft_buf( dc1, dc2, n, dc1 );
        bmath_cf2_s^ w0.urt(  0, 2 * n );
        bmath_cf2_s^ w1.urt( -1, 2 * n );
        for( sz_t i = 0; i < size; i++ ) { dst[ i ] = o.mul_r( dc2[ i ], w0 ); w1.mul( w0, w0 ); }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f2_s) ffv
    {
        dst.set_size( src.size );
        o.ff( src.data, dst.data, src.size );
    }

    //------------------------------------------------------------------------------------------------------------------

    /** DCT2 Fast Inverse Transformation (via FFT)  (DCT3 with factor 2/N)
     *  Transforms size source-samples into size destination samples
     *  src, dst can be the same pointer
     */
    func (:f2_s) fi
    {
        ASSERT( ( ( size - 1 ) & size ) == 0 );
        sz_t n = size * 2;
        o.buf_!.set_size( n * 2 );
        m bmath_cf2_s* dc1 = o.buf_.data + n * 0;
        m bmath_cf2_s* dc2 = o.buf_.data + n * 1;
        bmath_cf2_s^ w0.urt(  0, 2 * n );
        bmath_cf2_s^ w1.urt( -1, 2 * n );
        for( sz_t i = 0; i < size; i++ ) { w0.mul_f2( src[ i ], dc1[ i ] ); w1.mul( w0, w0 ); }
        for( sz_t i = size; i < n; i++ ) dc1[ i ].zro();
        bmath_fourier_f2_fft_buf( dc1, dc2, n, dc1 );
        f2_t inv_size = size > 0 ? 1.0 / size : 0;
        f2_t offs = size > 0 ? 0.5 * src[ 0 ] : 0;
        for( sz_t i = 0; i < size; i++ ) dst[ i ] = ( dc2[ i ].r - offs ) * 2 * inv_size;
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f2_s) fiv
    {
        dst.set_size( src.size );
        o.fi( src.data, dst.data, src.size );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f2_s) void selftest()
    {
        sz_t n = 1024;
        bmath_vf2_s^ v1.set_size( n ).set_random( 1.0, -1.0, 1.0, NULL );
        bmath_vf2_s^ v2;
        bmath_vf2_s^ v3;

        :f2_s^ dft;

        dft.dfv( v1, v2 );
        dft.div( v2, v3 );

        {
            f2_t dev = 0;
            for( sz_t i = 0; i < n; i++ ) dev += f2_sqr( v1.[ i ] - v3.[ i ] );
            ASSERT( dev < 1E-12 );
        }

        bmath_vf2_s^ v4;
        bmath_vf2_s^ v5;

        dft.ffv( v1, v4 );

        {
            f2_t dev = 0;
            for( sz_t i = 0; i < n; i++ ) dev += f2_sqr( v2.[ i ] - v4.[ i ] );
            ASSERT( dev < 1E-4 );
        }

        dft.fiv( v4, v5 );

        {
            f2_t dev = 0;
            for( sz_t i = 0; i < n; i++ ) dev += f2_sqr( v1.[ i ] - v5.[ i ] );
            ASSERT( dev < 1E-6 );
        }
    }

    //------------------------------------------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :mdct
{
    /// multiplies two complex numbers and returns the real part
    func (:f3_s) f3_t mul_r( bmath_cf3_s* a, bmath_cf3_s* b ) = a.r * b.r - a.i * b.i;
    func (:f2_s) f3_t mul_r( bmath_cf2_s* a, bmath_cf2_s* b ) = a.r * b.r - a.i * b.i;

    //------------------------------------------------------------------------------------------------------------------

    func (:f3_s) ff
    {
        ASSERT( ( ( src_size - 1 ) & src_size ) == 0 );
        if( !o.buf_ ) o.buf_ = bmath_vcf3_s!;
        o.buf_.set_size( src_size * 2 );
        m bmath_cf3_s* dc1 = o.buf_.data + src_size * 0;
        m bmath_cf3_s* dc2 = o.buf_.data + src_size * 1;

        bmath_cf3_s^ w0;
        bmath_cf3_s^ w1;

        sz_t n = src_size >> 1;

        w0.urt(  0, 4 * n );
        w1.urt( -1, 4 * n );
        for( sz_t i = 0; i < src_size; i++ ) { w0.mul_f3( src[ i ], dc1[ i ] ); w1.mul( w0, w0 ); }

        bmath_fourier_f3_fft_buf( dc1, dc2, src_size, dc1 );

        w0.urt( -1 * ( 1 + n ), 8 * n );
        w1.urt( -2 * ( 1 + n ), 8 * n );
        for( sz_t i = 0; i < n; i++ ) { dst[ i ] = o.mul_r( dc2[ i ], w0 ); w1.mul( w0, w0 ); }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f3_s) fi
    {
        ASSERT( ( ( dst_size - 1 ) & dst_size ) == 0 );
        if( !o.buf_ ) o.buf_ = bmath_vcf3_s!;
        o.buf_.set_size( dst_size * 2 );
        m bmath_cf3_s* dc1 = o.buf_.data + dst_size * 0;
        m bmath_cf3_s* dc2 = o.buf_.data + dst_size * 1;

        sz_t n = dst_size >> 1;

        bmath_cf3_s^ w0;
        bmath_cf3_s^ w1;

        w0.urt( -1 * ( 1 + n ), 8 * n );
        w1.urt( -2 * ( 1 + n ), 8 * n );
        for( sz_t i = 0; i < n; i++ ) { w0.mul_f3( src[ i ], dc1[ i ] ); w1.mul( w0, w0 ); }
        for( sz_t i = n; i < dst_size; i++ ) dc1[ i ].zro();

        bmath_fourier_f3_fft_buf( dc1, dc2, dst_size, dc1 );

        w0.urt(  0, 4 * n ).mul_f3( 1.0 / n, w0 );
        w1.urt( -1, 4 * n );
        for( sz_t i = 0; i < dst_size; i++ ) { dst[ i ] = o.mul_r( dc2[ i ], w0 ); w1.mul( w0, w0 ); }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f3_s) ffv
    {
        if( dst != src ) dst.set_size( src.size >> 1 );
        o.ff( src.data, dst.data, src.size );
        if( dst == src ) dst.set_size( src.size >> 1 );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f3_s) fiv
    {
        dst.set_size( src.size << 1 ); // in case dst == src space is increased but data retained
        o.fi( src.data, dst.data, dst.size );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f3_s) void selftest()
    {
        sz_t n = 1024;
        bmath_vf3_s^ v1.set_size( n ).set_random( 1.0, -1.0, 1.0, NULL );
        bmath_vf3_s^ v2;
        bmath_vf3_s^ v3;

        :f3_s^ mdct;

        mdct.fiv( mdct.ffv( v1, v2 ), v3 );
        ASSERT( v2.size == n / 2 );

        f3_t dev = 0;
        for( sz_t i = 0; i < ( n >> 1 ); i++ ) dev += f3_sqr( ( v1.[ i ] - v1.[     ( n >> 1 ) - i - 1 ] ) * 0.5 - v3.[ i ] );
        for( sz_t i = ( n >> 1 ); i < n; i++ ) dev += f3_sqr( ( v1.[ i ] + v1.[ n + ( n >> 1 ) - i - 1 ] ) * 0.5 - v3.[ i ] );

        ASSERT( dev < 1E-15 );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f2_s) ff
    {
        ASSERT( ( ( src_size - 1 ) & src_size ) == 0 );
        if( !o.buf_ ) o.buf_ = bmath_vcf2_s!;
        o.buf_.set_size( src_size * 2 );
        m bmath_cf2_s* dc1 = o.buf_.data + src_size * 0;
        m bmath_cf2_s* dc2 = o.buf_.data + src_size * 1;

        bmath_cf2_s^ w0;
        bmath_cf2_s^ w1;

        sz_t n = src_size >> 1;

        w0.urt(  0, 4 * n );
        w1.urt( -1, 4 * n );
        for( sz_t i = 0; i < src_size; i++ ) { w0.mul_f2( src[ i ], dc1[ i ] ); w1.mul( w0, w0 ); }

        bmath_fourier_f2_fft_buf( dc1, dc2, src_size, dc1 );

        w0.urt( -1 * ( 1 + n ), 8 * n );
        w1.urt( -2 * ( 1 + n ), 8 * n );
        for( sz_t i = 0; i < n; i++ ) { dst[ i ] = o.mul_r( dc2[ i ], w0 ); w1.mul( w0, w0 ); }
    };

    //------------------------------------------------------------------------------------------------------------------

    func (:f2_s) fi
    {
        ASSERT( ( ( dst_size - 1 ) & dst_size ) == 0 );
        if( !o.buf_ ) o.buf_ = bmath_vcf2_s!;
        o.buf_.set_size( dst_size * 2 );
        m bmath_cf2_s* dc1 = o.buf_.data + dst_size * 0;
        m bmath_cf2_s* dc2 = o.buf_.data + dst_size * 1;

        sz_t n = dst_size >> 1;

        bmath_cf2_s^ w0;
        bmath_cf2_s^ w1;

        w0.urt( -1 * ( 1 + n ), 8 * n );
        w1.urt( -2 * ( 1 + n ), 8 * n );
        for( sz_t i = 0; i < n; i++ ) { w0.mul_f2( src[ i ], dc1[ i ] ); w1.mul( w0, w0 ); }
        for( sz_t i = n; i < dst_size; i++ ) dc1[ i ].zro();

        bmath_fourier_f2_fft_buf( dc1, dc2, dst_size, dc1 );

        w0.urt(  0, 4 * n ).mul_f2( 1.0 / n, w0 );
        w1.urt( -1, 4 * n );
        for( sz_t i = 0; i < dst_size; i++ ) { dst[ i ] = o.mul_r( dc2[ i ], w0 ); w1.mul( w0, w0 ); }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f2_s) ffv
    {
        if( dst != src ) dst.set_size( src.size >> 1 );
        o.ff( src.data, dst.data, src.size );
        if( dst == src ) dst.set_size( src.size >> 1 );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f2_s) fiv
    {
        dst.set_size( src.size << 1 ); // in case dst == src space is increased but data retained
        o.fi( src.data, dst.data, dst.size );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:f2_s) void selftest()
    {
        sz_t n = 1024;
        bmath_vf2_s^ v1.set_size( n ).set_random( 1.0, -1.0, 1.0, NULL );
        bmath_vf2_s^ v2;
        bmath_vf2_s^ v3;

        :f2_s^ mdct;

        mdct.fiv( mdct.ffv( v1, v2 ), v3 );
        ASSERT( v2.size == n / 2 );

        f2_t dev = 0;
        for( sz_t i = 0; i < ( n >> 1 ); i++ ) dev += f2_sqr( ( v1.[ i ] - v1.[     ( n >> 1 ) - i - 1 ] ) * 0.5 - v3.[ i ] );
        for( sz_t i = ( n >> 1 ); i < n; i++ ) dev += f2_sqr( ( v1.[ i ] + v1.[ n + ( n >> 1 ) - i - 1 ] ) * 0.5 - v3.[ i ] );

        ASSERT( dev < 1E-6 );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func void selftest()
{
    :dct_f3_s_selftest();
    :dct_f2_s_selftest();

    :mdct_f3_s_selftest();
    :mdct_f2_s_selftest();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

