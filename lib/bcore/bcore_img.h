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

#ifndef BCORE_IMG_H
#define BCORE_IMG_H

#include "bcore_flect.h"
#include "bcore_features.h"
#include "bcore_sr.h"
#include "bcore_spect_sink.h"
#include "bcore_spect_source.h"

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

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bcore_img_u2_s )
{
    aware_t _;
    tp_t format; // pixel color space
    uz_t rows;
    uz_t cols;
    uz_t stride;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( u2_t, );
};

/** Allocates image and initializes pixels with 0
 *  if o->format == 0: o->format = TYPEOF_bcore_img_u2_argb
 */
void bcore_img_u2_s_set_size( bcore_img_u2_s* o, uz_t rows, uz_t cols );

// ---------------------------------------------------------------------------------------------------------------------
// pnm format
void bcore_img_u2_s_pnm_to_sink( const bcore_img_u2_s* o, bcore_sink* dst );
void bcore_img_u2_s_pnm_to_file( const bcore_img_u2_s* o, sc_t file );
void bcore_img_u2_s_pnm_from_source(   bcore_img_u2_s* o, bcore_source* src );
void bcore_img_u2_s_pnm_from_file(     bcore_img_u2_s* o, sc_t file );

// ---------------------------------------------------------------------------------------------------------------------
// pixel access

static inline u2_t bcore_img_u2_pixel_from_rgb( tp_t format, u0_t r, u0_t g, u0_t b )
{
    switch( format )
    {
        case TYPEOF_bcore_img_u2_argb: return (u2_t)r << 16 | (u2_t)g << 8 | b;
        default: ERR_fa( "Invalid format '#<sc_t>'", ifnameof( format ) );
    }
    return 0;
}

static inline void bcore_img_u2_pixel_to_rgb( tp_t format, u2_t v, u0_t* r, u0_t* g, u0_t* b )
{
    switch( format )
    {
        case TYPEOF_bcore_img_u2_argb:
        {
            if( b ) *b = v;
            if( g ) *g = v >> 8;
            if( r ) *r = v >> 16;
        }
        break;

        default: ERR_fa( "Invalid format '#<sc_t>'", ifnameof( format ) );
    }
}

static inline void bcore_img_u2_s_set_pixel( bcore_img_u2_s* o, uz_t row, uz_t col, u2_t v )
{
    assert( row < o->rows );
    assert( col < o->cols );
    o->data[ row * o->stride + col ] = v;
}

static inline u2_t bcore_img_u2_s_get_pixel( const bcore_img_u2_s* o, uz_t row, uz_t col )
{
    assert( row < o->rows );
    assert( col < o->cols );
    return o->data[ row * o->stride + col ];
}

static inline void bcore_img_u2_s_set_rgb( bcore_img_u2_s* o, uz_t row, uz_t col, u0_t r, u0_t g, u0_t b )
{
    bcore_img_u2_s_set_pixel( o, row, col, bcore_img_u2_pixel_from_rgb( o->format, r, g, b ) );
}

static inline void bcore_img_u2_s_get_rgb( const bcore_img_u2_s* o, uz_t row, uz_t col, u0_t* r, u0_t* g, u0_t* b )
{
    bcore_img_u2_pixel_to_rgb( o->format, bcore_img_u2_s_get_pixel( o, row, col ), r, g, b );
}

/**********************************************************************************************************************/

vd_t bcore_img_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_IMG_H
