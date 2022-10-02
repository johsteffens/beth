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

/** YUYV image encoding typically used in video capturing
 *  Chrominance (UV) is encoded at half (x-) resolution.
 *  Two adjaced pixels share the same chrominance but encode individual luminance.
 *  Data is encoded in YUYV pattern (4 bytes per 2 pixel).
 */
stamp :yuyv_s x_array
{
    sz_t width;  // number of image cols (always even)
    sz_t height; // number of image rows
    sz_t bytes_per_line;
    u0_t [];

    func void convert_to_argb( @* o, m bcore_img_u2_s* img );
}

//----------------------------------------------------------------------------------------------------------------------

identifier TYPEOF_bcore_img_u2_argb;

func u0_t clamp16( s2_t v ) { = s2_min( 255, s2_max( 0, v >> 8 ) ); }

func (:yuyv_s) convert_to_argb
{
    assert( ( o.width & 1 ) == 0 );
    img.format = TYPEOF_bcore_img_u2_argb;
    img.set_size( o.height, o.width );

    for( sz_t j = 0; j < img.rows; j++ )
    {
        u0_t* yp = o.data + o.bytes_per_line * j;

        m u2_t* u2_row = img.data + img.stride * j;
        for( sz_t i = 0; i < img.cols; i += 2 )
        {
            u2_t y1 = ( ( ( s2_t )yp[ i * 2     ] - 16 ) * 298 ) + 128;
            s2_t u  =     ( s2_t )yp[ i * 2 + 1 ] - 128;
            u2_t y2 = ( ( ( s2_t )yp[ i * 2 + 2 ] - 16 ) * 298 ) + 128;
            s2_t v  =     ( s2_t )yp[ i * 2 + 3 ] - 128;

            s2_t rp =             409 * v;
            s2_t gp = - 100 * u - 208 * v;
            s2_t bp =   516 * u;

            u2_row[ i     ] = bcore_img_u2_pixel_from_rgb( img.format, :clamp16( y1 + rp ), :clamp16( y1 + gp ), :clamp16( y1 + bp ) );
            u2_row[ i + 1 ] = bcore_img_u2_pixel_from_rgb( img.format, :clamp16( y2 + rp ), :clamp16( y2 + gp ), :clamp16( y2 + bp ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
