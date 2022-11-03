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

    func m u0_t* m_get_pixel( m@* o, sz_t row, sz_t col ) { assert( row >= 0 && row < o.rows ); assert( col >= 0 && col < o.cols ); = o.data + row * o.bytes_per_row + col * 4; };
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

    func o from_yuyv( m@* o, c :yuyv_s* img );

    ///------------------------------------------------------------------------------------------------------------------
    /// Frobenius norm

    /** fdev = ||f(o) - x||
     *  '|| ... ||' = Frobenius norm  ( sqrt(sum over squares) )
     *  Note: By this definition fdev_zro is the Frobenius norm of o.
     */
    func f3_t fdev_equ( @* o, @* b ); // deviation from equality
    func f3_t fdev_zro( @* o );       // deviation from zero


    ///------------------------------------------------------------------------------------------------------------------
    /// Matrix I/O

    /// converts all values to/from matrix (mat.cols == o.cols * 4 )
    func mat to_mf2( c @* o, m bmath_mf2_s* mat );
    func o from_mf2( m @* o, c bmath_mf2_s* mat );

    /// offs of {0,1,2,3} maps to {b,g,r,a}
    func mat offs_to_mf2( c @* o, sz_t offs, m bmath_mf2_s* mat );
    func o offs_from_mf2( m @* o, sz_t offs, c bmath_mf2_s* mat );

    func mat b_to_mf2( c @* o, m bmath_mf2_s* mat ) = o.offs_to_mf2( 0, mat );
    func mat g_to_mf2( c @* o, m bmath_mf2_s* mat ) = o.offs_to_mf2( 1, mat );
    func mat r_to_mf2( c @* o, m bmath_mf2_s* mat ) = o.offs_to_mf2( 2, mat );
    func mat a_to_mf2( c @* o, m bmath_mf2_s* mat ) = o.offs_to_mf2( 3, mat );
    func o b_from_mf2( m @* o, c bmath_mf2_s* mat ) = o.offs_from_mf2( 0, mat );
    func o g_from_mf2( m @* o, c bmath_mf2_s* mat ) = o.offs_from_mf2( 1, mat );
    func o r_from_mf2( m @* o, c bmath_mf2_s* mat ) = o.offs_from_mf2( 2, mat );
    func o a_from_mf2( m @* o, c bmath_mf2_s* mat ) = o.offs_from_mf2( 3, mat );

    ///------------------------------------------------------------------------------------------------------------------
    /// generating images...

    /// striped image: horizontal: blue, vertical: green, diagonal: red; period: period-size in pixels (-1: defaults to 64)
    func o generate_striped( m@* o, sz_t rows, sz_t cols, sz_t period );

    /// white noise
    func o generate_random_white( m@* o, sz_t rows, sz_t cols, m bcore_prsg* prsg );

    /// brownian noise
    func o generate_random_brown( m@* o, sz_t rows, sz_t cols, m bcore_prsg* prsg );
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
    func o from_bgra( m@* o, c :bgra_s* img );

    func img to_argb( c@* o, m bcore_img_u2_s* img );
    func o from_argb( m@* o, c bcore_img_u2_s* img );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

embed "bcodec_image.emb.x";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

