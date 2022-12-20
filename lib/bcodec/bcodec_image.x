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

include "bcore_std.h";
include "bmath_std.h";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/**
RGB - YUV Conversion

R = Y               + 1.59766 * V
G = Y - 0.39063 * U - 0.81250 * V
B = Y + 2.01563 * U

------

Y = +0.29874 * R +0.58742 * G +0.11384 * B
U = -0.14821 * R -0.29143 * G +0.43964 * B
V = +0.43893 * R -0.36768 * G -0.07126 * B
*/

func f3_t y_from_rgb( f3_t r, f3_t g, f3_t b ) = +0.29874 * r +0.58742 * g +0.11384 * b;
func f3_t u_from_rgb( f3_t r, f3_t g, f3_t b ) = -0.14821 * r -0.29143 * g +0.43964 * b;
func f3_t v_from_rgb( f3_t r, f3_t g, f3_t b ) = +0.43893 * r -0.36768 * g -0.07126 * b;

func f3_t r_from_yuv( f3_t y, f3_t u, f3_t v ) = y               + 1.59766 * v;
func f3_t g_from_yuv( f3_t y, f3_t u, f3_t v ) = y - 0.39063 * u - 0.81250 * v;
func f3_t b_from_yuv( f3_t y, f3_t u, f3_t v ) = y + 2.01563 * u;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** BGRA8888 image encoding (true color with alpha value) in 4 bytes per pixel.
 *  Common notations: BGRA8888 (little-endian), ARGB32 (big-endian)
 *  Compatible with bcore_img_s with bytes_per_row == stride * 4.
 *
 *  Let u0_t* p be a pointer to a pixel in the image:
 *  u0_t blue  = p[ 0 ];
 *  u0_t green = p[ 1 ];
 *  u0_t red   = p[ 2 ];
 *  u0_t alpha = p[ 3 ];
 *
 */
stamp :bgra_s x_array
{
    sz_t rows; // height of image
    sz_t cols; // width of image cols (always even)
    sz_t bytes_per_row; // cols * 4 on tightest packing. Can be lager when the data area is mapped to an external ABI.
    u0_t [];   // packed pixel data in BGRA format

    /// if bytes_per_row == -1, it is internally set to cols * 4
    func o set_size_bytes_per_row( m@* o, sz_t rows, sz_t cols, sz_t bytes_per_row );
    func o set_size( m@* o, sz_t rows, sz_t cols ) = o.set_size_bytes_per_row( rows, cols, -1 );
    func o zro( m@* o ) { bcore_memset( o.data, 0, o.size ); }

    func m u0_t* m_get_pixel( m@* o, sz_t row, sz_t col )
    {
        sz_t i = sz_max( 0, sz_min( o.rows - 1, row ) );
        sz_t j = sz_max( 0, sz_min( o.cols - 1, col ) );
        = ( o.rows * o.cols > 0 ) ? o.data + i * o.bytes_per_row + j * 4 : NULL;
    };

    /// if row, col is out of range, the nearest border pixel is returned
    func c u0_t* c_get_pixel( c@* o, sz_t row, sz_t col ) = o.cast( m$* ).m_get_pixel( row, col );

    func o set_pixel( m@* o, sz_t row, sz_t col, u0_t* data ) bcore_memcpy( o.m_get_pixel( row, col ), data, 4 );
    func o set_bgra(  m@* o, sz_t row, sz_t col, u0_t b, u0_t g, u0_t r, u0_t a ) { m u0_t* p = o.m_get_pixel( row, col ); p[ 0 ] = b; p[ 1 ] = g; p[ 2 ] = r; p[ 3 ] = a; }
    func o set_bgr(   m@* o, sz_t row, sz_t col, u0_t b, u0_t g, u0_t r         ) = o.set_bgra( row, col, b, g, r, 0 );
    func o set_argb(  m@* o, sz_t row, sz_t col, u0_t a, u0_t r, u0_t g, u0_t b ) = o.set_bgra( row, col, b, g, r, a );
    func o set_rgb(   m@* o, sz_t row, sz_t col,         u0_t r, u0_t g, u0_t b ) = o.set_bgra( row, col, b, g, r, 0 );

    /// pnm file format
    func o pnm_to_sink    ( c@* o, m x_sink*   sink   );
    func o pnm_from_source( m@* o, m x_source* source );
    func o pnm_to_file    ( c@* o, sc_t file ) = o.pnm_to_sink    (   x_sink_check_create_from_file( file )^ );
    func o pnm_from_file  ( m@* o, sc_t file ) = o.pnm_from_source( x_source_check_create_from_file( file )^ );

    func bcore_fp.copy_typed;
    func o copy_t( m@* o, x_inst* src, tp_t type ) o.copy_typed( type, src );
    func o copy_a( m@* o,     c  x_inst* src ) = o.copy_t( src, src._ );
    func o from_image( m@* o, c :*       img ) = o.copy_a( img );
    func o from_yuyv(  m@* o, c :yuyv_s* img );

    ///------------------------------------------------------------------------------------------------------------------
    // Note: Default deviations below can include the alpha channel.
    //       Choose ...bgr to limit deviations to color chanels

    /// square sum deviation
    func f3_t sdev_equ    ( @* o, @* b ); // deviation from equality
    func f3_t sdev_equ_bgr( @* o, @* b ); // deviation from equality
    func f3_t sdev_zro    ( @* o );       // deviation from zero
    func f3_t sdev_zro_bgr( @* o );       // deviation from zero

    /// Frobenius norm
    /** fdev = ||f(o) - x||
     *  '|| ... ||' = Frobenius norm  ( sqrt(sum over squares) )
     *  Note: By this definition fdev_zro is the Frobenius norm of o.
     */
    func f3_t fdev_equ    ( @* o, @* b ) = f3_srt( o.sdev_equ(b) );
    func f3_t fdev_equ_bgr( @* o, @* b ) = f3_srt( o.sdev_equ_bgr(b) );
    func f3_t fdev_zro    ( @* o )       = f3_srt( o.sdev_zro( ) );
    func f3_t fdev_zro_bgr( @* o )       = f3_srt( o.sdev_zro_bgr( ) );

    func s3_t     pixels    ( @*o ) = ( s3_t )o.rows * o.cols;
    func s3_t     values    ( @*o ) = o.pixels() * 4;
    func s3_t     values_bgr( @*o ) = o.pixels() * 3;
    func f3_t inv_values    ( @*o ) = ( ( o.values    () > 0 ) ? 1.0 / o.values    () : 0 );
    func f3_t inv_values_bgr( @*o ) = ( ( o.values_bgr() > 0 ) ? 1.0 / o.values_bgr() : 0 );

    /// RMS norm
    func f3_t rdev_equ    ( @* o, @* b ) = f3_srt( o.sdev_equ    (b) * o.inv_values    () );
    func f3_t rdev_equ_bgr( @* o, @* b ) = f3_srt( o.sdev_equ_bgr(b) * o.inv_values_bgr() );
    func f3_t rdev_zro    ( @* o )       = f3_srt( o.sdev_zro    ( ) * o.inv_values    () );
    func f3_t rdev_zro_bgr( @* o )       = f3_srt( o.sdev_zro_bgr( ) * o.inv_values_bgr() );

    ///------------------------------------------------------------------------------------------------------------------
    /// Vector/Matrix I/O

    /// converts all values to/from matrix (mat.cols == o.cols * 4 )
    func mat to_mf2( c @* o, m bmath_mf2_s* mat );
    func o from_mf2( m @* o, c bmath_mf2_s* mat );

    /// converts bgr values to/from matrix (mat.cols == o.cols * 3 )
    func mat to_bgr_mf2( c @* o, m bmath_mf2_s* mat );
    func o from_bgr_mf2( m @* o, c bmath_mf2_s* mat );

    /// converts yuv values to/from matrix (mat.cols == o.cols * 3 )
    func mat to_yuv_mf2( c @* o, m bmath_mf2_s* mat );
    func o from_yuv_mf2( m @* o, c bmath_mf2_s* mat );

    /// offs of {0,1,2,3} maps to {b,g,r,a}
    func mat offs_to_mf2( c @* o, sz_t offs, m bmath_mf2_s* mat );
    func o offs_from_mf2( m @* o, sz_t offs, c bmath_mf2_s* mat );

    func mat b_to_mf2( c @* o, m bmath_mf2_s* mat ) = o.offs_to_mf2( 0, mat );
    func mat g_to_mf2( c @* o, m bmath_mf2_s* mat ) = o.offs_to_mf2( 1, mat );
    func mat r_to_mf2( c @* o, m bmath_mf2_s* mat ) = o.offs_to_mf2( 2, mat );
    func mat a_to_mf2( c @* o, m bmath_mf2_s* mat ) = o.offs_to_mf2( 3, mat );
    func mat y_to_mf2( c @* o, m bmath_mf2_s* mat );
    func o b_from_mf2( m @* o, c bmath_mf2_s* mat ) = o.offs_from_mf2( 0, mat );
    func o g_from_mf2( m @* o, c bmath_mf2_s* mat ) = o.offs_from_mf2( 1, mat );
    func o r_from_mf2( m @* o, c bmath_mf2_s* mat ) = o.offs_from_mf2( 2, mat );
    func o a_from_mf2( m @* o, c bmath_mf2_s* mat ) = o.offs_from_mf2( 3, mat );
    func o y_from_mf2( m @* o, c bmath_mf2_s* mat );

    /// If patch is outside the image region, the image is extended by duplicating border pixels.
    func o patch_to_f2_bgr( c@* o, sz_t row, sz_t col, sz_t rows, sz_t cols, m f2_t* bgr );
    func bgr patch_to_vf2_bgr( c@* o, sz_t row, sz_t col, sz_t rows, sz_t cols, m bmath_vf2_s* bgr )
    {
        bgr.set_size( rows * cols * 3 );
        o.patch_to_f2_bgr( row, col, rows, cols, bgr.data );
    }

    /// Pixels outside the image area are ignored.
    func o patch_from_f2_bgr ( m@* o, sz_t row, sz_t col, sz_t rows, sz_t cols, c f2_t* bgr );
    func o patch_from_vf2_bgr( m@* o, sz_t row, sz_t col, sz_t rows, sz_t cols, c bmath_vf2_s* bgr )
    {
        ASSERT( bgr.size <= rows * cols * 3 );
        o.patch_from_f2_bgr( row, col, rows, cols, bgr.data );
    }

    ///------------------------------------------------------------------------------------------------------------------
    /// generating images...

    /// striped image: horizontal: blue, vertical: green, diagonal: red; period: period-size in pixels (-1: defaults to 64)
    func o gen_striped( m@* o, sz_t rows, sz_t cols, sz_t period );

    /// using a gen-object; Example: :bgra_s!.gen( 128, 256, :gen_jupiter_s!^ );
    func o gen( m@* o, sz_t rows, sz_t cols, m :gen* gen );
}

//----------------------------------------------------------------------------------------------------------------------

/// Generator used by image-function 'gen'
group :gen
{
    feature m bmath_mf2_s* f( m@*o, sz_t rows, sz_t cols, f3_t min, f3_t max );

    // white noise
    stamp :white_s   { $ bmath_mf2_s => mat; $ bcore_prsg => prsg; }

    // red noise (approximation)
    stamp :red_s     { $ bmath_mf2_s => mat; $ bcore_prsg => prsg; $ f3_t attn = 0.05; }

    // diagonal striped noise (similarity to jupiter bands)
    stamp :jupiter_s { $ bmath_mf2_s => mat; $ bcore_prsg => prsg; }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** YUYV image encoding typically used in video capturing
 *  Chrominance (UV) is encoded at half (x-) resolution.
 *  Two adjaced pixels share the same chrominance but encode individual luminance.
 *  Data is encoded in YUYV pattern (4 bytes per 2 pixel).
 */
stamp :yuyv_s x_array
{
    sz_t rows; // height of image
    sz_t cols; // width of image cols (always even)
    sz_t bytes_per_row; // cols * 2 on tightest packing. Can be lager when the data area is mapped to an external ABI.
    u0_t [];   // packed pixel data in YUYV format

    /// cols must be even; if bytes_per_row == -1, it is set to cols * 2
    func o set_size_bytes_per_row( m@* o, sz_t rows, sz_t cols, sz_t bytes_per_row );
    func o set_size( m@* o, sz_t rows, sz_t cols ) = o.set_size_bytes_per_row( rows, cols, -1 );

    func bcore_fp.copy_typed;
    func o copy_t( m@* o, x_inst* src, tp_t type ) o.copy_typed( type, src );
    func o copy_a( m@* o,     c  x_inst* src ) = o.copy_t( src, src._ );
    func o from_image( m@* o, c :*       img ) = o.copy_a( img );
    func o from_bgra(  m@* o, c :bgra_s* img );

    func img to_argb( c@* o, m bcore_img_u2_s* img );
    func o from_argb( m@* o, c bcore_img_u2_s* img );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

embed "bcodec_image.emb.x";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

