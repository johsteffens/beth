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

#ifndef BMATH_LEAF_H
#define BMATH_LEAF_H

/// functionality related to elementary types

#include "bcore_signal.h"
#include "bcore_leaf.h"
#include "bmath_quicktypes.h"

#include <float.h>

/**********************************************************************************************************************/
// f3_t

/// limits
static const f3_t f3_lim_min = DBL_MIN;     // minimum normalized representable positive number
static const f3_t f3_lim_max = DBL_MAX;     // maximum normalized representable positive number
static const f3_t f3_lim_eps = DBL_EPSILON; // minimum representable positive difference to 1.0

/// math constants
static inline f3_t f3_pi( void )   { return 3.1415926535897932384626434; }
static inline f3_t f3_pi_2( void ) { return 1.5707963267948966192313217; }
static inline f3_t f3_tau( void )  { return 6.2831853071795864769252868; }

/// random generator (range -1, 1)
static inline f3_t f3_xsg1_sym( u2_t* rv ) { return ( *rv = bcore_xsg1_u2( *rv ) ) * ( 2.0 / 0xFFFFFFFFu ) - 1.0; }
static inline f3_t f3_xsg2_sym( u2_t* rv ) { return ( *rv = bcore_xsg2_u2( *rv ) ) * ( 2.0 / 0xFFFFFFFFu ) - 1.0; }

/// random generator (range 0, 1)
static inline f3_t f3_xsg1_pos( u2_t* rv ) { return ( *rv = bcore_xsg1_u2( *rv ) ) * ( 1.0 / 0xFFFFFFFFu ); }
static inline f3_t f3_xsg2_pos( u2_t* rv ) { return ( *rv = bcore_xsg2_u2( *rv ) ) * ( 1.0 / 0xFFFFFFFFu ); }

/// Solves: sin(arc) * b = cos(arc) * a; cos >= 0  (deprecated -> use grt_s functions)
static inline
void f3_arc_to_sin_cos( f3_t a, f3_t b, f3_t* sin_arc, f3_t* cos_arc )
{
    // ensure cos_arc is never negative
    a = ( b > 0 ) ? a : -a;
    b = ( b > 0 ) ? b : -b;
    f3_t r = hypot( a, b );
    *sin_arc = ( r > f3_lim_min ) ? a / r : 0;
    *cos_arc = ( r > f3_lim_min ) ? b / r : 1;

}

/**********************************************************************************************************************/

vd_t bmath_leaf_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_LEAF_H

