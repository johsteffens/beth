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

/** Image objects compatible with bcore_matrix_s and bcore_array_s
 *  Data storage: Row-Major
 *
 *  Note:
 *    - Do not use bcore_array_s for size manipulation
 *    - bcore_array_s can access beyond image scope in case stride > cols
 *
 *  Image pixel encoding formats
 *    TYPEOF_bcore_img_u2_argb:
 *      ARGB32, BGRA8888 (little-endian)
 *      u2 = ((u2_t)a << 24) | ((u2_t)r << 16) | ((u2_t)g << 8) | b
 */

group bcore_img = x_inst
{
    // pixel conversion
    func void u2_pixel_to_rgb  ( tp_t format, u2_t v, u0_t* r, u0_t* g, u0_t* b );
    func u2_t u2_pixel_from_rgb( tp_t format, u0_t r, u0_t g, u0_t b );
    func void u2_pixel_to_yuv  ( tp_t format, u2_t p, s2_t* y, s2_t* u, s2_t* v ); // YUV values are scaled by factor 1000
    func u2_t u2_pixel_from_yuv( tp_t format, s2_t y, s2_t u, s2_t v );            // YUV values are scaled by factor 1000

    stamp :u2_s
    {
        tp_t format; // pixel color space
        uz_t rows;
        uz_t cols;
        uz_t stride;
        u2_t [];

        func void set_size( m@*o, uz_t rows, uz_t cols );

        // pnm format
        func o pnm_to_sink(     c@* o, x_sink* dst );
        func o pnm_to_file(     c@* o, sc_t file );
        func o pnm_from_source( m@* o, x_source* src );
        func o pnm_from_file(   m@* o, sc_t file );

        // pixel access
        func void set_pixel( m@* o, uz_t row, uz_t col, u2_t v );
        func u2_t get_pixel( c@* o, uz_t row, uz_t col );
        func void set_rgb( m@* o, uz_t row, uz_t col, u0_t  r, u0_t  g, u0_t  b );
        func void get_rgb( c@* o, uz_t row, uz_t col, u0_t* r, u0_t* g, u0_t* b );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

