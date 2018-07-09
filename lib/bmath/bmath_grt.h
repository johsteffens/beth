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

/** Givens rotation (G)
 *  The rotation is specified by sine (s) and cosine (c) components but
 *  without defining rows, columns indices (j,k), which will be supplied
 *  when needed at code level.
 *
 *  Applied to a vector v: v' = G * v with v = (a,b) is a butterfly-operation:
 *    a' = c * a + s * b
 *    b' = c * b - s * a
 *
 *  A complete rotation matrix Gkl (k < l) is deemed to have the following shape
 *  gkk = gll = c, gkl = -s, glk = s; All other elements: gij = ((i==j)?1:0)
 *
 *  The  left-side rotation G * A modifies rows    k and l of A.
 *  The right-side rotation A * G modifies columns k and l of A.
 */

#ifndef BMATH_GRT_H
#define BMATH_GRT_H

#include "bcore_std.h"
#include "bmath_leaf.h"

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_grt_f3_s ) { f3_t c; f3_t s; };

//---------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate a in vector (a,b).
static inline void bmath_grt_f3_s_init_to_annihilate_a( bmath_grt_f3_s* o, f3_t a, f3_t b )
{
    a = ( b > 0 ) ? a : -a;
    b = ( b > 0 ) ? b : -b;
    f3_t r = hypot( a, b );
    o->c = ( r > f3_lim_min ) ?  b / r : 1;
    o->s = ( r > f3_lim_min ) ? -a / r : 0;
}

//---------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate b in vector (a,b).
static inline void bmath_grt_f3_s_init_to_annihilate_b( bmath_grt_f3_s* o, f3_t a, f3_t b )
{
    b = ( a > 0 ) ? b : -b;
    a = ( a > 0 ) ? a : -a;
    f3_t r = hypot( b, a );
    o->c = ( r > f3_lim_min ) ? a / r : 1;
    o->s = ( r > f3_lim_min ) ? b / r : 0;
}

//---------------------------------------------------------------------------------------------------------------------

/// Applies specific rotation to vector (a,b) assuming it annihilates a.
static inline void bmath_grt_f3_s_annihilate_a( bmath_grt_f3_s* o, f3_t* a, f3_t* b )
{
    *b = o->c * *b - o->s * *a;
    *a = 0;
}

//---------------------------------------------------------------------------------------------------------------------

/// Applies specific rotation to vector (a,b) assuming it annihilates b.
static inline void bmath_grt_f3_s_annihilate_b( bmath_grt_f3_s* o, f3_t* a, f3_t* b )
{
    *a = o->c * *a + o->s * *b;
    *b = 0;
}

//---------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate a in vector (a,b) and execute the rotation.
static inline void bmath_grt_f3_s_init_and_annihilate_a( bmath_grt_f3_s* o, f3_t* a, f3_t* b )
{
    bmath_grt_f3_s_init_to_annihilate_a( o, *a, *b );
    bmath_grt_f3_s_annihilate_a( o, a, b );
}

//---------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate b in vector (a,b) and execute the rotation.
static inline void bmath_grt_f3_s_init_and_annihilate_b( bmath_grt_f3_s* o, f3_t* a, f3_t* b )
{
    bmath_grt_f3_s_init_to_annihilate_b( o, *a, *b );
    bmath_grt_f3_s_annihilate_b( o, a, b );
}

//---------------------------------------------------------------------------------------------------------------------

/// Applies general rotation to vector (a,b) (butterfly operation).
static inline void bmath_grt_f3_s_rotate( const bmath_grt_f3_s* o, f3_t* a, f3_t* b )
{
    f3_t a0 = *a;
    *a = o->c * a0 + o->s * *b;
    *b = o->c * *b - o->s * a0;
}

//---------------------------------------------------------------------------------------------------------------------

/// Rotates two matrix rows (section)
static inline void bmath_grt_f3_s_rotate_row( const bmath_grt_f3_s* o, f3_t* a, f3_t* b, sz_t start, sz_t end )
{
    for( sz_t i = start; i < end; i++ ) bmath_grt_f3_s_rotate( o, a + i, b + i );
}

//---------------------------------------------------------------------------------------------------------------------

/// Rotates two matrix columns (section).
static inline void bmath_grt_f3_s_rotate_col( const bmath_grt_f3_s* o, f3_t* a, f3_t* b, sz_t stride, sz_t start, sz_t end )
{
    for( sz_t i = start; i < end; i++ ) bmath_grt_f3_s_rotate( o, a + stride * i, b + stride * i );
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_arr_grt_f3_s ) { aware_t _; BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_grt_f3_s, ); };

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_grt_f3_s_set_space( bmath_arr_grt_f3_s* o, sz_t space );
void bmath_arr_grt_f3_s_set_size(  bmath_arr_grt_f3_s* o, sz_t size );
void bmath_arr_grt_f3_s_zro(       bmath_arr_grt_f3_s* o );
void bmath_arr_grt_f3_s_push(      bmath_arr_grt_f3_s* o, bmath_grt_f3_s grt );

//---------------------------------------------------------------------------------------------------------------------

static inline void bmath_arr_grt_f3_s_rotate_fwd( bmath_arr_grt_f3_s* o, sz_t start, f3_t* row, sz_t size )
{
    const bmath_grt_f3_s* grt = &o->data[ start ];
    for( sz_t i = 0; i < size; i++ ) bmath_grt_f3_s_rotate( grt + i, row + i - 1, row + i );
}

//---------------------------------------------------------------------------------------------------------------------

static inline void bmath_arr_grt_f3_s_rotate_bwd( bmath_arr_grt_f3_s* o, f3_t* row, sz_t start, sz_t size )
{
    const bmath_grt_f3_s* grt = &o->data[ start ];
    f3_t* a = row + start - 1;
    for( sz_t i = 0; i < size; i++ ) bmath_grt_f3_s_rotate( grt - i, a - i, a - i + 1 );
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_grt_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_GRT_H
