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

#ifndef BCORE_LEAF_H
#define BCORE_LEAF_H

/// Basic functionality related to leaf-types

#include "bcore_signal.h"
#include "bcore_types.h"
#include <float.h>
#include <math.h>

/** Nomenclature:
 *  sqr: x^2;     square
 *  srt: x^(0.5); square root
 *  abs: |x|; magnitude
 *  inv: 1/x; inverse for x != 0 (returns infinity in case x == 0)
 *  sig: sign( x ); 1 or -1
 */

/**********************************************************************************************************************/
// f3_t

/// limits
static const f3_t f3_lim_min = DBL_MIN;     // minimum normalized representable positive number
static const f3_t f3_lim_max = DBL_MAX;     // maximum normalized representable positive number
static const f3_t f3_lim_eps = DBL_EPSILON; // minimum representable positive difference to 1.0
static const f3_t f3_lim_inf = INFINITY;    // floating point representation of infinity

static inline f3_t f3_sqr( f3_t v ) { return v * v; }
static inline f3_t f3_srt( f3_t v ) { return sqrt( v ); }
static inline f3_t f3_min( f3_t a, f3_t b ) { return a < b ? a : b; }
static inline f3_t f3_max( f3_t a, f3_t b ) { return a > b ? a : b; }
static inline f3_t f3_sig( f3_t v ) { return v  < 0 ? -1 : 1; }
static inline f3_t f3_abs( f3_t v ) { return v  < 0 ? -v : v; }
static inline f3_t f3_inv( f3_t v ) { return v != 0 ? 1.0 / v : f3_lim_inf; }
static inline void f3_t_swap( f3_t* v1, f3_t* v2 ) { f3_t t = *v1; *v1 = *v2; *v2 = t;  }

/**********************************************************************************************************************/
// f2_t

/// limits
static const f2_t f2_lim_min = FLT_MIN;     // minimum normalized representable positive number
static const f2_t f2_lim_max = FLT_MAX;     // maximum normalized representable positive number
static const f2_t f2_lim_eps = FLT_EPSILON; // minimum representable positive difference to 1.0
static const f2_t f2_lim_inf = INFINITY;    // floating point representation of infinity

static inline f2_t f2_sqr( f2_t v ) { return v * v; }
static inline f2_t f2_srt( f2_t v ) { return sqrt( v ); }
static inline f2_t f2_min( f2_t a, f2_t b ) { return a < b ? a : b; }
static inline f2_t f2_max( f2_t a, f2_t b ) { return a > b ? a : b; }
static inline f2_t f2_sig( f2_t v ) { return v  < 0 ? -1 : 1; }
static inline f2_t f2_abs( f2_t v ) { return v  < 0 ? -v : v; }
static inline f2_t f2_inv( f2_t v ) { return v != 0 ? 1.0 / v : f2_lim_inf; }
static inline void f2_t_swap( f2_t* v1, f2_t* v2 ) { f2_t t = *v1; *v1 = *v2; *v2 = t;  }

/**********************************************************************************************************************/
// sz_t

static inline sz_t sz_sqr( sz_t v ) { return v * v; }
static inline sz_t sz_min( sz_t a, sz_t b ) { return a < b ? a : b; }
static inline sz_t sz_max( sz_t a, sz_t b ) { return a > b ? a : b; }
static inline sz_t sz_sig( sz_t v ) { return v < 0 ? -1 : 1; }
static inline sz_t sz_abs( sz_t v ) { return v < 0 ? -v : v; }
static inline void sz_t_swap( sz_t* v1, sz_t* v2 ) { sz_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/
// uz_t

static inline uz_t uz_sqr( uz_t v ) { return v * v; }
static inline uz_t uz_min( uz_t a, uz_t b ) { return a < b ? a : b; }
static inline uz_t uz_max( uz_t a, uz_t b ) { return a > b ? a : b; }
static inline void uz_t_swap( uz_t* v1, uz_t* v2 ) { uz_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/
// tp_t

static inline tp_t tp_min( tp_t a, tp_t b ) { return a < b ? a : b; }
static inline tp_t tp_max( tp_t a, tp_t b ) { return a > b ? a : b; }
static inline void tp_t_swap( tp_t* v1, tp_t* v2 ) { tp_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/
// s3_t

static inline s3_t s3_min( s3_t a, s3_t b ) { return a < b ? a : b; }
static inline s3_t s3_max( s3_t a, s3_t b ) { return a > b ? a : b; }
static inline s3_t s3_sig( s3_t v ) { return v < 0 ? -1 : 1; }
static inline s3_t s3_abs( s3_t v ) { return v < 0 ? -v : v; }
static inline void s3_t_swap( s3_t* v1, s3_t* v2 ) { s3_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/
// s2_t

static inline s2_t s2_min( s2_t a, s2_t b ) { return a < b ? a : b; }
static inline s2_t s2_max( s2_t a, s2_t b ) { return a > b ? a : b; }
static inline s2_t s2_sig( s2_t v ) { return v < 0 ? -1 : 1; }
static inline s2_t s2_abs( s2_t v ) { return v < 0 ? -v : v; }
static inline void s2_t_swap( s2_t* v1, s2_t* v2 ) { s2_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/
// u3_t

static inline u3_t u3_min( u3_t a, u3_t b ) { return a < b ? a : b; }
static inline u3_t u3_max( u3_t a, u3_t b ) { return a > b ? a : b; }
static inline void u3_t_swap( u3_t* v1, u3_t* v2 ) { u3_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/
// u2_t

static inline u2_t u2_min( u2_t a, u2_t b ) { return a < b ? a : b; }
static inline u2_t u2_max( u2_t a, u2_t b ) { return a > b ? a : b; }
static inline void u2_t_swap( u2_t* v1, u2_t* v2 ) { u2_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/

vd_t bcore_leaf_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_LEAF_H

