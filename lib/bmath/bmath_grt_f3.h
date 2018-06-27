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

/** This header file contains code around givens rotations with f3 precision.
 *
 *  Identifiers are deemed to be used within a bmath_f3 name space.
 *  Therefore this header can only be included once per translation unit and
 *  may not be shared outside its designated name space.
 */

#ifdef BMATH_GRT_F3_H
    #error "This header file can not be shared. Include it only in *.c files where needed."
#endif

#ifdef BMATH_GRT_F2_H
    #error "This header file can not be mixed with bmath_grt_f2.h"
#endif

#define BMATH_GRT_F3_H

/**********************************************************************************************************************/

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
typedef struct grt_s { f3_t c; f3_t s; } grt_s;

//---------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate b in vector (a,b).
static inline void grt_s_init_to_annihilate_b( grt_s* o, f3_t a, f3_t b )
{
    b = ( a > 0 ) ? b : -b;
    a = ( a > 0 ) ? a : -a;
    f3_t r = hypot( b, a );
    o->c = ( r > f3_lim_min ) ? a / r : 1;
    o->s = ( r > f3_lim_min ) ? b / r : 0;
}

//---------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate b in vector (a,b) and execute the rotation.
static inline void grt_s_init_and_annihilate_b( grt_s* o, f3_t* a, f3_t* b )
{
    grt_s_init_to_annihilate_b( o, *a, *b );
    *a = o->c * *a + o->s * *b;
    *b = 0;
}

//---------------------------------------------------------------------------------------------------------------------

/// Applies specific rotation to vector (a,b) assuming it annihilates b.
static inline void grt_s_annihilate_b( grt_s* o, f3_t* a, f3_t* b )
{
    *a = o->c * *a + o->s * *b;
    *b = 0;
}

//---------------------------------------------------------------------------------------------------------------------

/// Applies general rotation to vector (a,b) (butterfly operation).
static inline void grt_s_rotate( const grt_s* o, f3_t* a, f3_t* b )
{
    f3_t a0 = *a;
    *a = o->c * a0 + o->s * *b;
    *b = o->c * *b - o->s * a0;
}

//---------------------------------------------------------------------------------------------------------------------

/// Rotates two matrix rows (section)
static inline void grt_s_rotate_row( const grt_s* o, f3_t* a, f3_t* b, sz_t start, sz_t end )
{
    for( sz_t i = start; i < end; i++ ) grt_s_rotate( o, a + i, b + i );
}

//---------------------------------------------------------------------------------------------------------------------

/// Rotates two matrix columns (section).
static inline void grt_s_rotate_col( const grt_s* o, f3_t* a, f3_t* b, sz_t stride, sz_t start, sz_t end )
{
    for( sz_t i = start; i < end; i++ ) grt_s_rotate( o, a + stride * i, b + stride * i );
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

