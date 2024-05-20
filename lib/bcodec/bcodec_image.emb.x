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

    st_s^ format_name;
    bcore_source_string_s_create_from_string( first_three_lines )^.parse_fa( "#name #<sz_t*> #<sz_t*> 255\n", format_name.1, cols.1, rows.1 );

    if( format_name.equal_sc( "P6" ) ) // color map
    {
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
    else if( format_name.equal_sc( "P5" ) ) // grey map
    {
        o.set_size( rows, cols );
        for( sz_t i = 0; i < o.rows; i++ )
        {
            for( sz_t j = 0; j < o.cols; j++ )
            {
                u0_t v;
                source.get_data( v.1, 1 );
                o.set_argb( i, j, 0, v, v, v );
            }
        }
    }
    else
    {
        ERR_fa( "Unhandled PNM format '#<sc_t>'", format_name.sc );
    }

}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) copy_typed
{
    switch( type )
    {
        case     :yuyv_s~: o.from_yuyv( src.cast( :yuyv_s* ) );
        case bmath_mf2_s~: o.from_mf2( src.cast( bmath_mf2_s* ) );
        default: return bcore_error_push_fa( TYPEOF_conversion_error, "Cannot convert from '#<sc_t>'\n", bnameof( type ) );
    }
    return 0;
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

func (:bgra_s) to_bgr_mf2
{
    mat.set_size( o.rows, o.cols * 3 );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        m f2_t* m_row = mat.[ i * mat.stride ];
        c u0_t* o_row = o .[ i * o.bytes_per_row ];
        for( sz_t j = 0; j < o.cols; j++ )
        {
            m_row[ j * 3 + 0 ] = o_row[ j * 4 + 0 ];
            m_row[ j * 3 + 1 ] = o_row[ j * 4 + 1 ];
            m_row[ j * 3 + 2 ] = o_row[ j * 4 + 2 ];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) from_bgr_mf2
{
    ASSERT( mat.cols % 3 == 0 );
    o.set_size( mat.rows, mat.cols / 3 );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        c f2_t* m_row = mat.[ i * mat.stride ];
        m u0_t* o_row = o  .[ i * o.bytes_per_row ];
        for( sz_t j = 0; j < o.cols; j++ )
        {
            o_row[ j * 4 + 0 ] = f2_rs2( f2_max( 0, f2_min( 255, m_row[ j * 3 + 0 ] ) ) );
            o_row[ j * 4 + 1 ] = f2_rs2( f2_max( 0, f2_min( 255, m_row[ j * 3 + 1 ] ) ) );
            o_row[ j * 4 + 2 ] = f2_rs2( f2_max( 0, f2_min( 255, m_row[ j * 3 + 2 ] ) ) );
            o_row[ j * 4 + 3 ] = 0;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) to_yuv_mf2
{
    mat.set_size( o.rows, o.cols * 3 );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        m f2_t* m_row = mat.[ i * mat.stride ];
        c u0_t* o_row = o .[ i * o.bytes_per_row ];
        for( sz_t j = 0; j < o.cols; j++ )
        {
            f3_t b = o_row[ j * 4 + 0 ];
            f3_t g = o_row[ j * 4 + 1 ];
            f3_t r = o_row[ j * 4 + 2 ];
            m_row[ j * 3 + 0 ] = :y_from_rgb( r, g, b );
            m_row[ j * 3 + 1 ] = :u_from_rgb( r, g, b );
            m_row[ j * 3 + 2 ] = :v_from_rgb( r, g, b );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) from_yuv_mf2
{
    ASSERT( mat.cols % 3 == 0 );
    o.set_size( mat.rows, mat.cols / 3 );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        c f2_t* m_row = mat.[ i * mat.stride ];
        m u0_t* o_row = o  .[ i * o.bytes_per_row ];
        for( sz_t j = 0; j < o.cols; j++ )
        {
            f3_t y = m_row[ j * 3 + 0 ];
            f3_t u = m_row[ j * 3 + 1 ];
            f3_t v = m_row[ j * 3 + 2 ];

            o_row[ j * 4 + 0 ] = f3_rs2( f3_max( 0, f3_min( 255, :b_from_yuv( y, u, v ) ) ) );
            o_row[ j * 4 + 1 ] = f3_rs2( f3_max( 0, f3_min( 255, :g_from_yuv( y, u, v ) ) ) );
            o_row[ j * 4 + 2 ] = f3_rs2( f3_max( 0, f3_min( 255, :r_from_yuv( y, u, v ) ) ) );
            o_row[ j * 4 + 3 ] = 0;
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

func (:bgra_s) y_to_mf2
{
    mat.set_size( o.rows, o.cols );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        m f2_t* m_row = mat.[ i * mat.stride ];
        c u0_t* o_row = o .[ i * o.bytes_per_row ];
        for( sz_t j = 0; j < o.cols; j++ )
        {
            m_row[ j ] = :y_from_rgb( o_row[ j * 4 + 0 ], o_row[ j * 4 + 1 ], o_row[ j * 4 + 2 ] );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) y_from_mf2
{
    o.set_size( mat.rows, mat.cols );
    for( sz_t i = 0; i < o.rows; i++ )
    {
        for( sz_t j = 0; j < o.cols; j++ )
        {
            s2_t v = f2_rs2( f2_max( 0, f2_min( 255, mat.[ i * mat.stride + j ] ) ) );
            o.[ i * o.bytes_per_row + j * 4 + 0 ] = v;
            o.[ i * o.bytes_per_row + j * 4 + 1 ] = v;
            o.[ i * o.bytes_per_row + j * 4 + 2 ] = v;
            o.[ i * o.bytes_per_row + j * 4 + 3 ] = 0;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// If patch is outside the image region, the image is extended by duplicating border pixels.
func (:bgra_s) patch_to_f2_bgr
{
    if( o.rows * o.cols == 0 ) = o;
    for( sz_t i = row; i < rows; i++ )
    {
        for( sz_t j = col; j < cols; j++ )
        {
            u0_t* src = o.c_get_pixel( i, j );
            bgr[ 0 ] = src ? src[ 0 ] : 0;
            bgr[ 1 ] = src ? src[ 1 ] : 0;
            bgr[ 2 ] = src ? src[ 2 ] : 0;
            bgr += 3;
        }
    }
    = o;
}

//----------------------------------------------------------------------------------------------------------------------

/// Pixels outside the image area are ignored.
func (:bgra_s) patch_from_f2_bgr
{
    for( sz_t i = row; i < rows; i++ )
    {
        if( i >= 0 && i < o.rows )
        {
            for( sz_t j = col; j < cols; j++ )
            {
                if( j >= 0 && j < o.cols )
                {
                    m u0_t* dst = o.m_get_pixel( i, j );
                    dst[ 0 ] = bgr[ 0 ];
                    dst[ 1 ] = bgr[ 1 ];
                    dst[ 2 ] = bgr[ 2 ];
                    bgr += 3;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) sdev_equ
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
    = sum;
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) sdev_zro
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
    = sum;
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) sdev_equ_bgr
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
                +  s2_sqr( ( s2_t )po[ 2 ] - ( s2_t )pb[ 2 ] );
        }
    }
    = sum;
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) sdev_zro_bgr
{
    f3_t sum = 0;
    for( sz_t i = 0; i < o.rows; i++ )
    {
        for( sz_t j = 0; j < o.cols; j++ )
        {
            u0_t* po = o.c_get_pixel( i, j );
            sum += s2_sqr( ( s2_t )po[ 0 ] )
                +  s2_sqr( ( s2_t )po[ 1 ] )
                +  s2_sqr( ( s2_t )po[ 2 ] );
        }
    }
    = sum;
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) gen_striped
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

group :gen
{
    func o rescale( m bmath_mf2_s* o, f3_t min, f3_t max )
    {
        f3_t m_min = o.min();
        f3_t m_max = o.max();
        f3_t inv_m_range = ( m_max > m_min ) ? 1.0 / ( m_max - m_min ) : 0;
        f3_t range = max - min;
        f3_t fac = inv_m_range * range;

        for( sz_t i = 0; i < o.rows; i++ )
        {
            m f2_t* row = o.data + i * o.stride;
            for( sz_t j = 0; j < o.cols; j++ ) row[ j ] = ( ( row[ j ] - m_min ) * fac ) + min;
        }
    }

    func (:white_s) :.f
    {
        if ( !o.prsg ) o.prsg =< bcore_prsg_lcg_u3_00_s!;
        o.mat!.set_size( rows, cols );
        for( sz_t i = 0; i < rows; i++ )
        {
            for( sz_t j = 0; j < cols; j++ ) o.mat.[ i * o.mat.stride + j ] = o.prsg.gen_f3( min, max );
        }
        = o.mat;
    }

    func (:red_s) :.f
    {
        if ( !o.prsg ) o.prsg =< bcore_prsg_lcg_u3_00_s!;
        o.mat!.set_size( rows, cols );
        :white_s!( o.mat.fork(), o.prsg.fork() )^.f( rows, cols, -1, 1 );

        for( sz_t i = 0; i < rows; i++ )
        {
            m f2_t* row1 = o.mat.data + i * o.mat.stride;
            for( sz_t j = 1; j < cols; j++ ) row1[ j ] = ( row1[ j ] + row1[ j - 1 ] ) * ( 1.0 - o.attn );
        }

        for( sz_t i = 1; i < rows; i++ )
        {
            m f2_t* row0 = o.mat.data + ( i - 1 ) * o.mat.stride;
            m f2_t* row1 = o.mat.data + i * o.mat.stride;
            for( sz_t j = 0; j < cols; j++ ) row1[ j ] = ( row1[ j ] + row0[ j ] ) * ( 1.0 - o.attn );
        }

        = o.mat.:rescale( min, max );
    }

    func (:jupiter_s) :.f
    {
        if ( !o.prsg ) o.prsg =< bcore_prsg_lcg_u3_00_s!;
        o.mat!.set_size( rows, cols ).zro();
        for( sz_t i = 0; i < rows; i++ )
        {
            for( sz_t j = 0; j < cols; j++ )
            {
                f3_t v = o.prsg.gen_f3( -1, 1 );
                if( j > 0 ) v += o.mat.[ i * o.mat.stride + ( j - 1 ) ] * 0.5;
                if( i > 0 ) v += o.mat.[ ( i - 1 ) * o.mat.stride + j ] * 0.5;
                o.mat.[ i * o.mat.stride + j ] += v;
            }
        }
        = o.mat.:rescale( min, max );
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bgra_s) gen
{
    o.b_from_mf2( gen.f( rows, cols, 0, 255 ) );
    o.g_from_mf2( gen.f( rows, cols, 0, 255 ) );
    o.r_from_mf2( gen.f( rows, cols, 0, 255 ) );
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

func (:yuyv_s) copy_typed
{
    switch( type )
    {
        case :bgra_s~: o.from_bgra( src.cast( :bgra_s* ) );
        case bcore_img_u2_s~: o.from_argb( src.cast( bcore_img_u2_s* ) );
        default: return bcore_error_push_fa( TYPEOF_conversion_error, "Cannot convert from '#<sc_t>'\n", bnameof( type ) );
    }
    return 0;
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
    :bgra_s^ i1.gen( 128, 256, :gen_jupiter_s!^ );

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

        //i1.pnm_to_file( "temp/bcodec_image_bgra_s_selftest.pnm" );
        //bgra.pnm_to_file( "temp/bcodec_image_bgra_s_selftest_2.pnm" );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

