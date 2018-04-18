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
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/
// f3_t

static inline f3_t f3_sqr( f3_t v ) { return v * v; }
static inline f3_t f3_srt( f3_t v ) { return sqrt( v ); }
static inline f3_t f3_min( f3_t a, f3_t b ) { return a < b ? a : b; }
static inline f3_t f3_max( f3_t a, f3_t b ) { return a > b ? a : b; }
static inline f3_t f3_abs( f3_t v ) { return v  < 0 ? -v : v; }
static inline f3_t f3_inv( f3_t v ) { return v != 0 ? 1.0 / v : 0; }

static inline f3_t f3_pi()   { return 3.1415926535897932384626434; }
static inline f3_t f3_pi_2() { return 1.5707963267948966192313217; }
static inline f3_t f3_tau()  { return 6.2831853071795864769252868; }

/// random generator (range -1, 1)
static inline f3_t f3_xsg1_sym( u2_t* rv ) { return ( *rv = bcore_xsg1_u2( *rv ) ) * ( 2.0 / 0xFFFFFFFFu ) - 1.0; }
static inline f3_t f3_xsg2_sym( u2_t* rv ) { return ( *rv = bcore_xsg2_u2( *rv ) ) * ( 2.0 / 0xFFFFFFFFu ) - 1.0; }

/// random generator (range 0, 1)
static inline f3_t f3_xsg1_pos( u2_t* rv ) { return ( *rv = bcore_xsg1_u2( *rv ) ) * ( 1.0 / 0xFFFFFFFFu ); }
static inline f3_t f3_xsg2_pos( u2_t* rv ) { return ( *rv = bcore_xsg2_u2( *rv ) ) * ( 1.0 / 0xFFFFFFFFu ); }

/**********************************************************************************************************************/
// sz_t

static inline sz_t sz_sqr( sz_t v ) { return v * v; }
static inline sz_t sz_min( sz_t a, sz_t b ) { return a < b ? a : b; }
static inline sz_t sz_max( sz_t a, sz_t b ) { return a > b ? a : b; }

/**********************************************************************************************************************/

vd_t bmath_leaf_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_LEAF_H

