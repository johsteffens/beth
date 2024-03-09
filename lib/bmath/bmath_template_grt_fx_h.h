/** Author and Copyright 2018 Johannes Bernhard Steffens
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

/** Givens rotation (G)
 *  The rotation is specified by sine (s) and cosine (c) components but
 *  without defining rows, columns indices (j,k), which will be supplied
 *  when needed at code level.
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
 *  Where possible o->c >= 0. This particularly applies to
 *  annihilating-initialization and rho-initialization.
 *
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

/** Returns 'rho' - a stable single float representation of a givens rotation.
 *  (Variant of Steward 1976: The Economical Storage of Plane Rotations)
 *
 *  Exact reconstruction possible for o->c >= 0, otherwise reconstruction can differ by factor -1.
 *
 *  Inverse function: bmath_grt_fx_s_init_from_rho
 *
 *  rho is defined as follows:
 *        (0.5 * sign( cos ) * sin)  for abs( sin ) <  abs( cos )
 *        (2.0 * sign( sin ) / cos)  for abs( sin ) >= abs( cos )
 *        sign( sin )                for cos == 0
 *
 *  rho is invariant to mirroring across both rotation axes.
 */
static inline f3_t BCATU(bmath_grt_fx_s,rho)( const bmath_grt_fx_s* o )
{
    f3_t abs_c = f3_abs( o->c );
    return f3_abs( o->s ) < abs_c ?
                0.5 * f3_sig( o->c ) * o->s :
                   abs_c > f3_lim_min ?
                       2.0 * f3_sig( o->s ) / o->c :
                           f3_sig( o->s );
}

//----------------------------------------------------------------------------------------------------------------------

/// Reconstructs rotation from rho obtained by bmath_grt_fx_s_rho
static inline void BCATU(bmath_grt_fx_s,init_from_rho)( bmath_grt_fx_s* o, f3_t r )
{
    f3_t abs_r = f3_abs( r );
    if( abs_r < 0.5 )
    {
        f3_t s = 2.0 * r;
        o->c = f3_srt( 1.0 - s * s );
        o->s = s;
    }
    else if( abs_r > 2.0 )
    {
        f3_t c = 2.0 / abs_r;
        o->s = f3_sig( r ) * f3_srt( 1.0 - c * c );
        o->c = c;
    }
    else
    {
        o->s = f3_sig( r );
        o->c = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate a in vector (a,b).
static inline void BCATU(bmath_grt_fx_s,init_to_annihilate_a)( bmath_grt_fx_s* o, f3_t a, f3_t b )
{
    a = ( b < 0 ) ? -a : a;
    b = ( b < 0 ) ? -b : b;
    f3_t r = hypot( a, b );
    o->c = ( r > f3_lim_min ) ?  b / r : 1;
    o->s = ( r > f3_lim_min ) ? -a / r : 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate b in vector (a,b). (cos >= 0)
static inline void BCATU(bmath_grt_fx_s,init_to_annihilate_b)( bmath_grt_fx_s* o, f3_t a, f3_t b )
{
    b = ( a < 0 ) ? -b : b;
    a = ( a < 0 ) ? -a : a;
    f3_t r = hypot( b, a );
    o->c = ( r > f3_lim_min ) ? a / r : 1;
    o->s = ( r > f3_lim_min ) ? b / r : 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// Applies specific rotation to vector (a,b) assuming it annihilates a.
static inline void BCATU(bmath_grt_fx_s,annihilate_a)( bmath_grt_fx_s* o, fx_t* a, fx_t* b )
{
    //*b = o->c * *b - o->s * *a;

    *b = BCATU( fx, sig )( *b ) * hypot( *b, *a );
    *a = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// Applies specific rotation to vector (a,b) assuming it annihilates b.
static inline void BCATU(bmath_grt_fx_s,annihilate_b)( bmath_grt_fx_s* o, fx_t* a, fx_t* b )
{
    //*a = o->c * *a + o->s * *b;
    *a = BCATU( fx, sig )( *a ) * hypot( *a, *b );
    *b = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate a in vector (a,b) and execute the rotation. (cos >= 0)
static inline void BCATU(bmath_grt_fx_s,init_and_annihilate_a)( bmath_grt_fx_s* o, fx_t* a, fx_t* b )
{
    BCATU(bmath_grt_fx_s,init_to_annihilate_a)( o, *a, *b );
    BCATU(bmath_grt_fx_s,annihilate_a)( o, a, b );
}

//----------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate b in vector (a,b) and execute the rotation. (cos >= 0)
static inline void BCATU(bmath_grt_fx_s,init_and_annihilate_b)( bmath_grt_fx_s* o, fx_t* a, fx_t* b )
{
    BCATU(bmath_grt_fx_s,init_to_annihilate_b)( o, *a, *b );
    BCATU(bmath_grt_fx_s,annihilate_b)( o, a, b );
}

//----------------------------------------------------------------------------------------------------------------------

/// Sets up rotation in order to annihilate b in vector (a,b), executes the rotation and stores rho on *b
static inline void BCATU(bmath_grt_fx_s,init_and_annihilate_b_set_rho)( bmath_grt_fx_s* o, fx_t* a, fx_t* b )
{
    BCATU(bmath_grt_fx_s,init_to_annihilate_b)( o, *a, *b );
    //*a = o->c * *a + o->s * *b;
    *a = BCATU( fx, sig )( *a ) * hypot( *a, *b );
    *b = BCATU(bmath_grt_fx_s,rho)( o );
}

//----------------------------------------------------------------------------------------------------------------------

/// Applies general rotation to vector (a,b) (butterfly operation).
static inline void BCATU(bmath_grt_fx_s,rotate)( const bmath_grt_fx_s* o, fx_t* a, fx_t* b )
{
    fx_t a0 = *a;
    *a = o->c * a0 + o->s * *b;
    *b = o->c * *b - o->s * a0;
}

//----------------------------------------------------------------------------------------------------------------------

/// Rotates two matrix rows (section)
static inline void BCATU(bmath_grt_fx_s,row_rotate)( const bmath_grt_fx_s* o, fx_t* a, fx_t* b, sz_t start, sz_t end )
{
    if( o->s == 0 ) return;
    for( sz_t i = start; i < end; i++ ) BCATU(bmath_grt_fx_s,rotate)( o, a + i, b + i );
}

//----------------------------------------------------------------------------------------------------------------------

/// Rotates two matrix columns (section).
static inline void BCATU(bmath_grt_fx_s,col_rotate)( const bmath_grt_fx_s* o, fx_t* a, fx_t* b, sz_t stride, sz_t start, sz_t end )
{
    if( o->s == 0 ) return;
    for( sz_t i = start; i < end; i++ ) BCATU(bmath_grt_fx_s,rotate)( o, a + stride * i, b + stride * i );
}

//----------------------------------------------------------------------------------------------------------------------

/// Swipes matrix row
static inline void BCATU(bmath_grt_fx_s,row_swipe)( const bmath_grt_fx_s* arr, fx_t* row, sz_t start, sz_t end )
{
    for( sz_t i = start; i < end; i++ ) BCATU(bmath_grt_fx_s,rotate)( arr + i, row + i, row + i + 1 );
}

//----------------------------------------------------------------------------------------------------------------------

/// Swipes matrix row in reverse
static inline void BCATU(bmath_grt_fx_s,row_swipe_reverse)( const bmath_grt_fx_s* arr, fx_t* row, sz_t start, sz_t end )
{
    for( sz_t i = end; i > start; i-- ) BCATU(bmath_grt_fx_s,rotate)( arr + i, row + i - 1, row + i );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_grt_fx_s,to_stdout)( const bmath_grt_fx_s* o );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_arr_grt_fx_s ) { aware_t _; BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_grt_fx_s, ); };

static inline void BCATU(bmath_arr_grt,fx,s,set_size)( bmath_arr_grt_fx_s* o, sz_t size ) { bcore_array_a_set_size( ( bcore_array* )o, size ); }
static inline void BCATU(bmath_arr_grt,fx,s,zro)( bmath_arr_grt_fx_s* o ) { bcore_u_memzero( sizeof( bmath_grt_fx_s ), o->data, o->size ); }

/// creates array of given size; explicitly zeros all elements
bmath_arr_grt_fx_s* BCATU(bmath_arr_grt,fx,s,create_size)( sz_t size );
bmath_arr_grt_fx_s  BCATU(bmath_arr_grt,fx,of_size)( sz_t size );

/// returns number of identity rotations (sine == 0)
static inline sz_t BCATU(bmath_arr_grt,fx,s,count_identity)( const bmath_arr_grt_fx_s* o, sz_t start, sz_t end )
{
    sz_t count = 0;
    for( sz_t i = start; i < end; i++ ) count += ( o->data[ i ].s == 0 );
    return count;
}

//----------------------------------------------------------------------------------------------------------------------

static inline fx_t BCATU(bmath_arr_grt,fx,s,density)( const bmath_arr_grt_fx_s* o, sz_t start, sz_t end )
{
    sz_t count = BCATU(bmath_arr_grt,fx,s,count_identity)( o, start, end );
    return ( end > start ) ? ( fx_t )( end - start - count ) / ( end - start ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// givens row rotation (simd optimized)
void BCATU(bmath_simd,fx,row_rotate)( fx_t* v1, fx_t* v2, sz_t size, const bmath_grt_fx_s* grt );

/// givens column rotation (simd optimized)
void BCATU(bmath_simd,fx,col_rotate)( fx_t* v1, fx_t* v2, sz_t stride, sz_t size, const bmath_grt_fx_s* grt );

/// see bmath_mfx_s_drow_swipe_rev (simd optimized)
void BCATU(bmath_simd,fx,drow_swipe_rev)( fx_t* row, const bmath_grt_fx_s* grt, sz_t size );

/// bmath_simd_fx_drow_swipe_rev for 4 successive rows (simd optimized)
void BCATU(bmath_simd,fx,4drow_swipe_rev)( fx_t* row, sz_t stride, const bmath_grt_fx_s* grt, sz_t size );

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

