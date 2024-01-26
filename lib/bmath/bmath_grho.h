/** Author and Copyright 2023 Johannes Bernhard Steffens
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

#ifndef BMATH_GRHO_H
#define BMATH_GRHO_H

#include "bcore_std.h"
#include "bmath_hwflags.h"
#include "bmath_leaf.h"

/** Givens rotation (G) using a compact storage format (single float per rotation)
 *  (Variant of Steward 1976: The Economical Storage of Plane Rotations)
 *
 *  The givens rotation is specified by sine (s) and cosine (c).
 *
 *  Applied to a vector v: v' = G * v with v = (a,b) is a 'butterfly-operation':
 *    a' = c * a + s * b
 *    b' = c * b - s * a
 *
 *  A complete rotation matrix Gkl (k < l) is deemed to have the following shape
 *  gkk = gll = c, gkl = -s, glk = s; All other elements: gij = ((i==j)?1:0)
 *
 *  The  left-multiplication G * A modifies rows    k and l of A.
 *  The right-multiplication A * G modifies columns k and l of A.
 *
 *  The compact storage format uses a single floating point value 'rho' to represent the entire rotation.
 *  This allows storing rotations temporarily in-place on matrix operations.
 *
 *  rho is defined as follows:
 *        (0.5 * sign( cos ) * sin)  for abs( sin ) <  abs( cos )
 *        (2.0 * sign( sin ) / cos)  for abs( sin ) >= abs( cos )
 *        sign( sin )                for cos == 0
 *
 *  Exact reconstruction only possible for cos >= 0, otherwise reconstruction can differ by factor -1.
 *  In practise cos >= 0 can always be granted.
 *
 *  rho is invariant to mirroring across both rotation axes.
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// rho from givens rotation
static inline f3_t bmath_grho_from_grt( f3_t c, f3_t s )
{
    f3_t abs_c = f3_abs( c );
    return f3_abs( s ) < abs_c ?
                0.5 * f3_sig( c ) * s :
                   abs_c > f3_lim_min ?
                       2.0 * f3_sig( s ) / c :
                           f3_sig( s );
}

//----------------------------------------------------------------------------------------------------------------------

/// rho to givens rotation
static inline void bmath_grho_to_grt( f3_t r, f3_t* c, f3_t* s )
{
    f3_t abs_r = f3_abs( r );
    if( abs_r < 0.5 )
    {
        *s = 2.0 * r;
        *c = f3_srt( 1.0 - *s * *s );
    }
    else if( abs_r > 2.0 )
    {
        *c = 2.0 / abs_r;
        *s = f3_sig( r ) * f3_srt( 1.0 - *c * *c );
    }
    else
    {
        *s = f3_sig( r );
        *c = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate a in vector (a,b).
static inline f3_t bmath_grho_init_to_annihilate_a( f3_t a, f3_t b )
{
    a = ( b < 0 ) ? -a : a;
    b = ( b < 0 ) ? -b : b; // b >= 0
    f3_t r = hypot( a, b );

    return f3_abs( a ) < b ?
                0.5 * ( ( r > f3_lim_min ) ? -a / r : 0 ) :
                   b > f3_lim_min ?
                       2.0 * f3_sig( -a ) * r / b :
                           f3_sig( -a );
}

//----------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate b in vector (a,b). (cos >= 0)
static inline f3_t bmath_grho_init_to_annihilate_b( f3_t a, f3_t b )
{
    b = ( a < 0 ) ? -b : b;
    a = ( a < 0 ) ? -a : a; // a >= 0
    f3_t r = hypot( b, a );

    return f3_abs( b ) < a ?
                0.5 * ( ( r > f3_lim_min ) ? b / r : 0 ) :
                   a > f3_lim_min ?
                       2.0 * f3_sig( b ) * r / a :
                           f3_sig( b );
}

//----------------------------------------------------------------------------------------------------------------------

// Applies specific rotation to vector (a,b) assuming it annihilates a.
// Returns rotated b (rotated a is zero)
static inline f3_t bmath_grho_annihilate_a( f3_t rho, f3_t a, f3_t b )
{
    f3_t c, s;
    bmath_grho_to_grt( rho, &c, &s );
    return c * b - s * a;
}

//----------------------------------------------------------------------------------------------------------------------

// Applies specific rotation to vector (a,b) assuming it annihilates b.
// Returns rotated a (rotated b is zero)
static inline f3_t bmath_grho_annihilate_b( f3_t rho, f3_t a, f3_t b )
{
    f3_t c, s;
    bmath_grho_to_grt( rho, &c, &s );
    return c * a + s * b;
}

//----------------------------------------------------------------------------------------------------------------------

/// Applies general rotation to vector (a,b) (butterfly operation).
static inline void bmath_grho_cs_rotate_f3( f3_t c, f3_t s, f3_t* a, f3_t* b )
{
    f3_t a0 = *a;
    *a = c * a0 + s * *b;
    *b = c * *b - s * a0;
}

//----------------------------------------------------------------------------------------------------------------------

/// Applies general rotation to vector (a,b) (butterfly operation).
static inline void bmath_grho_cs_rotate_f2( f3_t c, f3_t s, f2_t* a, f2_t* b )
{
    f3_t a0 = *a;
    *a = c * a0 + s * *b;
    *b = c * *b - s * a0;
}

//----------------------------------------------------------------------------------------------------------------------

/// Applies general rotation to vector (a,b) (butterfly operation).
static inline void bmath_grho_rotate_f3( f3_t rho, f3_t* a, f3_t* b )
{
    f3_t c, s;
    bmath_grho_to_grt( rho, &c, &s );
    bmath_grho_cs_rotate_f3( c, s, a, b );
}

//----------------------------------------------------------------------------------------------------------------------

/// Applies general rotation to vector (a,b) (butterfly operation).
static inline void bmath_grho_rotate_f2( f3_t rho, f2_t* a, f2_t* b )
{
    f3_t c, s;
    bmath_grho_to_grt( rho, &c, &s );
    bmath_grho_cs_rotate_f2( c, s, a, b );
}

//----------------------------------------------------------------------------------------------------------------------

/// Rotates two matrix rows (section)
static inline void bmath_grho_row_rotate_f3( f3_t rho, f3_t* a, f3_t* b, uz_t start, uz_t end )
{
    if( rho == 0 ) return;
    f3_t c, s;
    bmath_grho_to_grt( rho, &c, &s );
    for( uz_t i = start; i < end; i++ ) bmath_grho_cs_rotate_f3( c, s, a + i, b + i );
}

//----------------------------------------------------------------------------------------------------------------------

/// Rotates two matrix rows (section)
static inline void bmath_grho_row_rotate_f2( f3_t rho, f2_t* a, f2_t* b, uz_t start, uz_t end )
{
    if( rho == 0 ) return;
    f3_t c, s;
    bmath_grho_to_grt( rho, &c, &s );
    for( uz_t i = start; i < end; i++ ) bmath_grho_cs_rotate_f2( c, s, a + i, b + i );
}

//----------------------------------------------------------------------------------------------------------------------

/// Rotates two matrix columns (section).
static inline void bmath_grho_col_rotate_f3( f3_t rho, f3_t* a, f3_t* b, uz_t stride, uz_t start, uz_t end )
{
    if( rho == 0 ) return;
    f3_t c, s;
    bmath_grho_to_grt( rho, &c, &s );
    for( uz_t i = start; i < end; i++ ) bmath_grho_cs_rotate_f3( c, s, a + stride * i, b + stride * i );
}

//----------------------------------------------------------------------------------------------------------------------

/// Rotates two matrix columns (section).
static inline void bmath_grho_col_rotate_f2( f3_t rho, f2_t* a, f2_t* b, uz_t stride, uz_t start, uz_t end )
{
    if( rho == 0 ) return;
    f3_t c, s;
    bmath_grho_to_grt( rho, &c, &s );
    for( uz_t i = start; i < end; i++ ) bmath_grho_cs_rotate_f2( c, s, a + stride * i, b + stride * i );
}

//----------------------------------------------------------------------------------------------------------------------

/// Swipes matrix row
static inline void bmath_grho_row_swipe_f3( const f3_t* rho_arr, f3_t* row, uz_t start, uz_t end )
{
    for( uz_t i = start; i < end; i++ ) bmath_grho_rotate_f3( rho_arr[ i ], row + i, row + i + 1 );
}

//----------------------------------------------------------------------------------------------------------------------

/// Swipes matrix row
static inline void bmath_grho_row_swipe_f2( const f3_t* rho_arr, f2_t* row, uz_t start, uz_t end )
{
    for( uz_t i = start; i < end; i++ ) bmath_grho_rotate_f2( rho_arr[ i ], row + i, row + i + 1 );
}

//----------------------------------------------------------------------------------------------------------------------

/// Swipes matrix row in reverse
static inline void bmath_grho_row_swipe_reverse_f3( const f3_t* rho_arr, f3_t* row, uz_t start, uz_t end )
{
    for( uz_t i = end; i > start; i-- ) bmath_grho_rotate_f3( rho_arr[ i ], row + i - 1, row + i );
}

//----------------------------------------------------------------------------------------------------------------------

/// Swipes matrix row in reverse
static inline void bmath_grho_row_swipe_reverse_f2( const f3_t* rho_arr, f2_t* row, uz_t start, uz_t end )
{
    for( uz_t i = end; i > start; i-- ) bmath_grho_rotate_f2( rho_arr[ i ], row + i - 1, row + i );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif  // BMATH_GRHO_H
