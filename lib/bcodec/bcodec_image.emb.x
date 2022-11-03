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

/** Image formats used by codecs & media */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

identifier TYPEOF_bcore_img_u2_argb;

func u0_t clamp16 ( s2_t v ) { = s2_min( 255, s2_max( 0, v >> 8 ) ); }
func u0_t rclamp16( s2_t v ) { = s2_min( 255, s2_max( 0, ( v + 128 ) >> 8 ) ); }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) set_size_bytes_per_row
{
    bytes_per_row = ( bytes_per_row < 0 ) ? cols * 4 : bytes_per_row;
    o.cast( m x_array* ).set_size( bytes_per_row * rows );
    o.bytes_per_row = bytes_per_row;
    o.rows = rows;
    o.cols = cols;
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) pnm_to_sink
{
    sink.push_fa( "P6\n#<sz_t> #<sz_t>\n255\n", o.cols, o.rows );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        for( sz_t j = 0; j < o.cols; j++ )
        {
            u0_t* bgra = o.c_get_pixel( i, j );
            u0_t rgb[ 3 ] = { bgra[ 2 ], bgra[ 1 ], bgra[ 0 ] };
            sink.push_data( rgb, 3 );
        }
    }
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) pnm_from_source
{
    // We avoid source-parse functions so that source can stay a mere data buffer.
    // Instead we scan the first text lines into a string:
    st_s^ first_three_lines;
    sz_t count = 0;
    char c;
    while( !source.eos() && count < 3 )
    {
        first_three_lines.push_char( ( c = source.get_char() ) );
        count += ( c == '\n' );
    }

    sz_t rows = 0; sz_t cols = 0;
    bcore_source_string_s_create_from_string( first_three_lines )^.parse_fa( "P6 #<sz_t*> #<sz_t*> 255\n", cols.1, rows.1 );
    o.set_size( rows, cols );

    for( sz_t i = 0; i < o.rows; i++ )
    {
        for( sz_t j = 0; j < o.cols; j++ )
        {
            u0_t rgb[ 3 ];
            source.get_data( rgb, 3 );
            o.set_argb( i, j, 0, rgb[ 0 ], rgb[ 1 ], rgb[ 2 ] );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) from_yuyv
{
    assert( ( img.cols & 1 ) == 0 );
    o.set_size( img.rows, img.cols );

    for( sz_t j = 0; j < img.rows; j++ )
    {
        c u0_t* i_row = img.data + img.bytes_per_row * j;
        m u0_t* o_row =   o.data +   o.bytes_per_row * j;
        for( sz_t i = 0; i < img.cols; i += 2 )
        {
            u2_t y1 = ( ( ( s2_t )i_row[ i * 2     ] -  16 ) * 298 );
            s2_t u  =     ( s2_t )i_row[ i * 2 + 1 ] - 128;
            u2_t y2 = ( ( ( s2_t )i_row[ i * 2 + 2 ] -  16 ) * 298 );
            s2_t v  =     ( s2_t )i_row[ i * 2 + 3 ] - 128;

            s2_t rp =             409 * v;
            s2_t gp = - 100 * u - 208 * v;
            s2_t bp =   516 * u;

            o_row[ i * 4     ] = :rclamp16( y1 + bp );
            o_row[ i * 4 + 1 ] = :rclamp16( y1 + gp );
            o_row[ i * 4 + 2 ] = :rclamp16( y1 + rp );
            o_row[ i * 4 + 3 ] = 0;

            o_row[ i * 4 + 4 ] = :rclamp16( y2 + bp );
            o_row[ i * 4 + 5 ] = :rclamp16( y2 + gp );
            o_row[ i * 4 + 6 ] = :rclamp16( y2 + rp );
            o_row[ i * 4 + 7 ] = 0;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) to_mf2
{
    mat.set_size( o.rows, o.cols * 4 );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        m f2_t* m_row = mat.[ i * mat.stride ];
        c u0_t* o_row = o .[ i * o.bytes_per_row ];
        for( sz_t j = 0; j < o.cols; j++ )
        {
            m_row[ j * 4 + 0 ] = o_row[ j * 4 + 0 ];
            m_row[ j * 4 + 1 ] = o_row[ j * 4 + 1 ];
            m_row[ j * 4 + 2 ] = o_row[ j * 4 + 2 ];
            m_row[ j * 4 + 3 ] = o_row[ j * 4 + 3 ];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) from_mf2
{
    ASSERT( mat.cols % 4 == 0 );
    o.set_size( mat.rows, mat.cols / 4 );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        c f2_t* m_row = mat.[ i * mat.stride ];
        m u0_t* o_row = o  .[ i * o.bytes_per_row ];
        for( sz_t j = 0; j < o.cols; j++ )
        {
            o_row[ j * 4 + 0 ] = f2_rs2( f2_max( 0, f2_min( 255, m_row[ j * 4 + 0 ] ) ) );
            o_row[ j * 4 + 1 ] = f2_rs2( f2_max( 0, f2_min( 255, m_row[ j * 4 + 1 ] ) ) );
            o_row[ j * 4 + 2 ] = f2_rs2( f2_max( 0, f2_min( 255, m_row[ j * 4 + 2 ] ) ) );
            o_row[ j * 4 + 3 ] = f2_rs2( f2_max( 0, f2_min( 255, m_row[ j * 4 + 3 ] ) ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) offs_to_mf2
{
    ASSERT( offs >= 0 && offs < 4 );
    mat.set_size( o.rows, o.cols );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        for( sz_t j = 0; j < o.cols; j++ )
        {
            mat.[ i * mat.stride + j ] = o.[ i * o.bytes_per_row + j * 4 + offs ];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) offs_from_mf2
{
    ASSERT( offs >= 0 && offs < 4 );
    o.set_size( mat.rows, mat.cols );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        for( sz_t j = 0; j < o.cols; j++ )
        {
            o.[ i * o.bytes_per_row + j * 4 + offs ] = f2_rs2( f2_max( 0, f2_min( 255, mat.[ i * mat.stride + j ] ) ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) fdev_equ
{
    ASSERT( o.rows == b.rows );
    ASSERT( o.cols == b.cols );
    f3_t sum = 0;
    for( sz_t i = 0; i < o.rows; i++ )
    {
        for( sz_t j = 0; j < o.cols; j++ )
        {
            u0_t* po = o.c_get_pixel( i, j );
            u0_t* pb = b.c_get_pixel( i, j );

            sum += s2_sqr( ( s2_t )po[ 0 ] - ( s2_t )pb[ 0 ] )
                +  s2_sqr( ( s2_t )po[ 1 ] - ( s2_t )pb[ 1 ] )
                +  s2_sqr( ( s2_t )po[ 2 ] - ( s2_t )pb[ 2 ] )
                +  s2_sqr( ( s2_t )po[ 3 ] - ( s2_t )pb[ 3 ] );
        }
    }
    = f3_srt( sum );
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) fdev_zro
{
    f3_t sum = 0;
    for( sz_t i = 0; i < o.rows; i++ )
    {
        for( sz_t j = 0; j < o.cols; j++ )
        {
            u0_t* po = o.c_get_pixel( i, j );
            sum += s2_sqr( ( s2_t )po[ 0 ] )
                +  s2_sqr( ( s2_t )po[ 1 ] )
                +  s2_sqr( ( s2_t )po[ 2 ] )
                +  s2_sqr( ( s2_t )po[ 3 ] );
        }
    }
    = f3_srt( sum );
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) generate_striped
{
    sz_t p = period > 0 ? period : 64;
    sz_t h = p >> 1;

    o.set_size( rows, cols );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        for( sz_t j = 0; j < o.cols; j++ )
        {
            o.set_bgr( i, j,
                ( i       % p >= h ) ? 200 : 0,
                ( j       % p >= h ) ? 200 : 0,
              ( ( i + j ) % p >= h ) ? 200 : 0 );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func o set_random_white( m bmath_mf2_s* o, sz_t rows, sz_t cols, m bcore_prsg* prsg, f3_t min, f3_t max )
{
    o.set_size( rows, cols );
    for( sz_t i = 0; i < rows; i++ )
    {
        for( sz_t j = 0; j < cols; j++ )
        {
            o.[ i * o.stride + j ] = prsg.gen_f3( 0, 255 );
        }
    }

    = o;
}

func (:bgra_s) generate_random_white
{
    if( !prsg )
    {
        prsg = bcore_prsg_lcg_u3_00_s!^^;
        prsg.set_state_u3( 1234 );
    }
    bmath_mf2_s^ mat;
    o.b_from_mf2( :set_random_white( mat, rows, cols, prsg, 0, 255 ) );
    o.g_from_mf2( :set_random_white( mat, rows, cols, prsg, 0, 255 ) );
    o.r_from_mf2( :set_random_white( mat, rows, cols, prsg, 0, 255 ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) generate_random_brown
{
    if( !prsg )
    {
        prsg = bcore_prsg_lcg_u3_00_s!^^;
        prsg.set_state_u3( 1234 );
    }
    u3_t state = prsg.state_u3();

    o.set_size( rows, cols );

    f3_t min = 0; f3_t max = 0;
    f3_t f = 0;

    f3_t attenuation = 0.95;

    for( sz_t c = 0; c < 2; c++ )
    {
        bmath_mf3_s^ m1.set_size( o.cols, 3 ).zro();
        bmath_mf3_s^ m2.set_size( o.cols, 3 ).zro();
        prsg.set_state_u3( state );

        for( sz_t i = 0; i < o.rows; i++ )
        {
            for( sz_t j = 0; j < o.cols; j++ )
            {
                for( sz_t k = 0; k < 3; k++ )
                {
                    m1.[ j * 3 + k ] = ( ( ( j > 0 ) ? m1.[ ( j - 1 ) * 3 + k ] : 0 ) + prsg.gen_f3( -1.0, 1.0 ) ) * attenuation;
                    m2.[ j * 3 + k ] = ( m2.[ j * 3 + k ] + m1.[ j * 3 + k ] ) * attenuation;
                }

                if( c == 1 )
                {
                    o.set_bgr
                    (
                        i, j,
                        f3_rs3( ( m2.[ j * 3 + 0 ] - min ) * f ),
                        f3_rs3( ( m2.[ j * 3 + 1 ] - min ) * f ),
                        f3_rs3( ( m2.[ j * 3 + 2 ] - min ) * f )
                    );
                }
            }

            if( c == 0 )
            {
                min = f3_min( min, m2.min() );
                max = f3_max( max, m2.max() );
            }
        }
        f = ( max > min ) ? 255.0 / ( max - min ) : 0;
    }

    bcore_msg_fa( "[#<f3_t>,#<f3_t>]\n", min, max );

}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:yuyv_s) set_size_bytes_per_row
{
    ASSERT( ( cols & 1 ) == 0 );
    bytes_per_row = ( bytes_per_row < 0 ) ? cols * 2 : bytes_per_row;
    o.cast( m x_array* ).set_size( bytes_per_row * rows );
    o.bytes_per_row = bytes_per_row;
    o.rows = rows;
    o.cols = cols;
}

//----------------------------------------------------------------------------------------------------------------------

/**
RGB from YUV:
Y = ( YP - 16 ) * 1.16407

R = Y               + 1.59766 * V
G = Y - 0.39063 * U - 0.81250 * V
B = Y + 2.01563 * U

------

Y = +0.29874 * R +0.58742 * G +0.11384 * B
U = -0.14821 * R -0.29143 * G +0.43964 * B
V = +0.43893 * R -0.36768 * G -0.07126 * B

*/

func (:yuyv_s) to_argb
{
    assert( ( o.cols & 1 ) == 0 );
    img.format = TYPEOF_bcore_img_u2_argb;
    img.set_size( o.rows, o.cols );

    for( sz_t j = 0; j < img.rows; j++ )
    {
        u0_t* yp = o.data + o.bytes_per_row * j;

        m u2_t* u2_row = img.data + img.stride * j;
        for( sz_t i = 0; i < img.cols; i += 2 )
        {
            u2_t y1 = ( ( ( s2_t )yp[ i * 2     ] - 16 ) * 298 );
            s2_t u  =     ( s2_t )yp[ i * 2 + 1 ] - 128;
            u2_t y2 = ( ( ( s2_t )yp[ i * 2 + 2 ] - 16 ) * 298 );
            s2_t v  =     ( s2_t )yp[ i * 2 + 3 ] - 128;

            s2_t rp =             409 * v;
            s2_t gp = - 100 * u - 208 * v;
            s2_t bp =   516 * u;

            u2_row[ i     ] = bcore_img_u2_pixel_from_rgb( img.format, :rclamp16( y1 + rp ), :rclamp16( y1 + gp ), :rclamp16( y1 + bp ) );
            u2_row[ i + 1 ] = bcore_img_u2_pixel_from_rgb( img.format, :rclamp16( y2 + rp ), :rclamp16( y2 + gp ), :rclamp16( y2 + bp ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:yuyv_s) from_bgra
{
//Y = +0.29874 * R +0.58742 * G +0.11384 * B
//U = -0.14821 * R -0.29143 * G +0.43964 * B
//V = +0.43893 * R -0.36768 * G -0.07126 * B

    assert( ( img.cols & 1 ) == 0 );
    o.set_size( img.rows, img.cols );

    for( sz_t j = 0; j < img.rows; j++ )
    {
        m u0_t* o_row =   o.data +   o.bytes_per_row * j;
        c u0_t* i_row = img.data + img.bytes_per_row * j;
        for( sz_t i = 0; i < img.cols; i += 2 )
        {
            u0_t b1 = i_row[ i * 4     ];
            u0_t g1 = i_row[ i * 4 + 1 ];
            u0_t r1 = i_row[ i * 4 + 2 ];
            u0_t b2 = i_row[ i * 4 + 4 ];
            u0_t g2 = i_row[ i * 4 + 5 ];
            u0_t r2 = i_row[ i * 4 + 6 ];

            s2_t y1 = ( s2_t )r1 * 76 + ( s2_t )g1 * 150 + ( s2_t )b1 * 29;
            s2_t y2 = ( s2_t )r2 * 76 + ( s2_t )g2 * 150 + ( s2_t )b2 * 29;

            s2_t r = ( s2_t )r1 + ( s2_t )r2;
            s2_t g = ( s2_t )g1 + ( s2_t )g2;
            s2_t b = ( s2_t )b1 + ( s2_t )b2;

            s2_t u = ( r * -38 - g * 75 + b * 113 + 1 ) >> 1;
            s2_t v = ( r * 112 - g * 94 - b * 18  + 1 ) >> 1;

            o_row[ i * 2     ] = :rclamp16( ( ( y1 * 220 + 128 ) >> 8 ) + ( 16 << 8 ) );
            o_row[ i * 2 + 1 ] = :rclamp16( u + ( 128 << 8 ) );
            o_row[ i * 2 + 2 ] = :rclamp16( ( ( y2 * 220 + 128 ) >> 8 ) + ( 16 << 8 ) );
            o_row[ i * 2 + 3 ] = :rclamp16( v + ( 128 << 8 ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:yuyv_s) from_argb
{
//Y = +0.29874 * R +0.58742 * G +0.11384 * B
//U = -0.14821 * R -0.29143 * G +0.43964 * B
//V = +0.43893 * R -0.36768 * G -0.07126 * B

    assert( ( img.cols & 1 ) == 0 );
    assert( img.format == TYPEOF_bcore_img_u2_argb );
    o.set_size( img.rows, img.cols );

    u0_t r1 = 0; u0_t g1 = 0; u0_t b1 = 0;
    u0_t r2 = 0; u0_t g2 = 0; u0_t b2 = 0;

    for( sz_t j = 0; j < img.rows; j++ )
    {
        m u0_t* yp = o.data + o.bytes_per_row * j;

        m u2_t* u2_row = img.data + img.stride * j;
        for( sz_t i = 0; i < img.cols; i += 2 )
        {
            bcore_img_u2_pixel_to_rgb( img.format, u2_row[ i     ], r1, g1, b1 );
            bcore_img_u2_pixel_to_rgb( img.format, u2_row[ i + 1 ], r2, g2, b2 );

            s2_t y1 = ( s2_t )r1 * 76 + ( s2_t )g1 * 150 + ( s2_t )b1 * 29;
            s2_t y2 = ( s2_t )r2 * 76 + ( s2_t )g2 * 150 + ( s2_t )b2 * 29;

            s2_t r = ( s2_t )r1 + ( s2_t )r2;
            s2_t g = ( s2_t )g1 + ( s2_t )g2;
            s2_t b = ( s2_t )b1 + ( s2_t )b2;

            s2_t u = ( r * -38 - g * 75 + b * 113 + 1 ) >> 1;
            s2_t v = ( r * 112 - g * 94 - b * 18  + 1 ) >> 1;

            yp[ i * 2     ] = :rclamp16( ( ( y1 * 220 + 128 ) >> 8 ) + ( 16 << 8 ) );
            yp[ i * 2 + 1 ] = :rclamp16( u + ( 128 << 8 ) );
            yp[ i * 2 + 2 ] = :rclamp16( ( ( y2 * 220 + 128 ) >> 8 ) + ( 16 << 8 ) );
            yp[ i * 2 + 3 ] = :rclamp16( v + ( 128 << 8 ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) void selftest()
{
    //:bgra_s^ i1.generate_striped( 128, 256, 16 );
    :bgra_s^ i1.generate_random_white( 128, 256, NULL );
    //:bgra_s^ i1.generate_random_brown( 128, 256, NULL );
    //:bgra_s^ i1.generate_random_brown( 400, 500, NULL );

    /// test pnm codec
    {
        :bgra_s^ i2;
        m bcore_sink_buffer_s* sink = bcore_sink_buffer_s!^;
        i1.pnm_to_sink( sink );
        i2.pnm_from_source( bcore_source_buffer_s_create_from_data( sink.data, sink.size )^ );
        ASSERT( bcore_compare_aware( i1.1, i2.1 ) == 0 );
    }

    /// test yuyv conversion
    {
        :yuyv_s^ yuyv.from_bgra( i1 );
        :bgra_s^ bgra.from_yuyv( yuyv );

        f3_t dev = bgra.fdev_equ( i1 ) / ( i1.rows * i1.cols );
        ASSERT( dev < 0.5 );

         i1.pnm_to_file( "temp/bcodec_image_bgra_s_selftest.pnm" );
         bgra.pnm_to_file( "temp/bcodec_image_bgra_s_selftest_2.pnm" );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

