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

static inline f3_t f3_ident( f3_t v ) { return  v; }
static inline f3_t f3_neg  ( f3_t v ) { return -v; }
static inline f3_t f3_sqr  ( f3_t v ) { return v * v; }
static inline f3_t f3_srt  ( f3_t v ) { return sqrt( v ); }
static inline f3_t f3_min  ( f3_t a, f3_t b ) { return a < b ? a : b; }
static inline f3_t f3_max  ( f3_t a, f3_t b ) { return a > b ? a : b; }
static inline f3_t f3_sig  ( f3_t v ) { return v  < 0 ? -1 : 1; }
static inline f3_t f3_abs  ( f3_t v ) { return v  < 0 ? -v : v; }
static inline f3_t f3_inv  ( f3_t v ) { return v != 0 ? 1.0 / v : f3_lim_inf; }
static inline s2_t f3_rs2  ( f3_t v ) { return  lrint( v ); }
static inline s3_t f3_rs3  ( f3_t v ) { return llrint( v ); }
static inline void f3_t_swap( f3_t* v1, f3_t* v2 ) { f3_t t = *v1; *v1 = *v2; *v2 = t;  }

/// math constants
static inline f3_t f3_pi(   void ) { return 3.1415926535897932384626434; } // PI
static inline f3_t f3_pi_2( void ) { return 1.5707963267948966192313217; } // PI / 2
static inline f3_t f3_tau(  void ) { return 6.2831853071795864769252868; } // 2 * PI
static inline f3_t f3_srt2( void ) { return 1.4142135623730950488016887; } // squareroot( 2 )

static inline bl_t f3_is_nan( f3_t v ) { return v != v; } // nan compares unequal to itself

/// extended math functions
static inline f3_t f3_exp  ( f3_t v ) { return exp( v ); }
static inline f3_t f3_log  ( f3_t v ) { return v > 0 ? log  ( v ) : -f3_lim_max; }
static inline f3_t f3_log2 ( f3_t v ) { return v > 0 ? log2 ( v ) : -f3_lim_max; }
static inline f3_t f3_log10( f3_t v ) { return v > 0 ? log10( v ) : -f3_lim_max; }
static inline f3_t f3_pow  ( f3_t a, f3_t b ) { return ( a > 0 ) ? pow( a, b ) : 0; }
static inline f3_t f3_erf  ( f3_t a ) { return erf( a ); }
static inline f3_t f3_sin  ( f3_t a ) { return sin( a ); }
static inline f3_t f3_cos  ( f3_t a ) { return cos( a ); }
static inline f3_t f3_tan  ( f3_t a ) { return tan( a ); }
static inline f3_t f3_tanh ( f3_t a ) { return tanh( a ); }
static inline f3_t f3_ferm ( f3_t a ) { return 1.0 / ( 1.0 + exp( -a ) );    } // fermi function (2ferm(2x)-1 == tanh(x))
static inline f3_t f3_sign ( f3_t a ) { return a < 0 ? -1 : a > 0 ? 1 : 0  ; } // sign function
static inline f3_t f3_ustp ( f3_t a ) { return a < 0 ?  0 : a > 0 ? 1 : 0.5; } // unit step function (heavyside function)
static inline f3_t f3_sqrt ( f3_t a ) { return sqrt( a ); }
static inline f3_t f3_ceil ( f3_t a ) { return ceil( a ); }
static inline f3_t f3_floor( f3_t a ) { return floor( a ); }

/// random generator (range -1, 1)
static inline f3_t f3_rnd_sym(  u3_t* rv ) { return ( *rv = bcore_lcg01_u3( *rv ) ) * ( 2.0 / 0xFFFFFFFFFFFFFFFFull ) - 1.0; }

/// random generator (range 0, 1)
static inline f3_t f3_rnd_pos(  u3_t* rv ) { return ( *rv = bcore_lcg01_u3( *rv ) ) * ( 1.0 / 0xFFFFFFFFFFFFFFFFull ); }


/**********************************************************************************************************************/
// f2_t

/// limits
static const f2_t f2_lim_min = FLT_MIN;     // minimum normalized representable positive number
static const f2_t f2_lim_max = FLT_MAX;     // maximum normalized representable positive number
static const f2_t f2_lim_eps = FLT_EPSILON; // minimum representable positive difference to 1.0
static const f2_t f2_lim_inf = INFINITY;    // floating point representation of infinity

static inline f2_t f2_ident( f2_t v ) { return  v; }
static inline f2_t f2_neg  ( f2_t v ) { return -v; }
static inline f2_t f2_sqr  ( f2_t v ) { return v * v; }
static inline f2_t f2_srt  ( f2_t v ) { return sqrt( v ); }
static inline f2_t f2_min  ( f2_t a, f2_t b ) { return a < b ? a : b; }
static inline f2_t f2_max  ( f2_t a, f2_t b ) { return a > b ? a : b; }
static inline f2_t f2_sig  ( f2_t v ) { return v  < 0 ? -1 : 1; }
static inline f2_t f2_abs  ( f2_t v ) { return v  < 0 ? -v : v; }
static inline f2_t f2_inv  ( f2_t v ) { return v != 0 ? 1.0 / v : f2_lim_inf; }
static inline s2_t f2_rs2  ( f2_t v ) { return  lrintf( v ); }
static inline s3_t f2_rs3  ( f2_t v ) { return llrintf( v ); }
static inline void f2_t_swap( f2_t* v1, f2_t* v2 ) { f2_t t = *v1; *v1 = *v2; *v2 = t;  }

/// math constants
static inline f2_t f2_pi(   void ) { return 3.1415926535897932384626434; } // PI
static inline f2_t f2_pi_2( void ) { return 1.5707963267948966192313217; } // PI / 2
static inline f2_t f2_tau(  void ) { return 6.2831853071795864769252868; } // 2 * PI
static inline f2_t f2_srt2( void ) { return 1.4142135623730950488016887; } // squareroot( 2 )

static inline bl_t f2_is_nan( f2_t v ) { return v != v; } // nan compares unequal to itself

/// extended math functions
static inline f2_t f2_exp  ( f2_t v ) { return expf( v ); }
static inline f2_t f2_log  ( f2_t v ) { return v > 0 ? logf  ( v ) : -f2_lim_max; }
static inline f2_t f2_log2 ( f2_t v ) { return v > 0 ? log2f ( v ) : -f2_lim_max; }
static inline f2_t f2_log10( f2_t v ) { return v > 0 ? log10f( v ) : -f2_lim_max; }
static inline f2_t f2_pow  ( f2_t a, f2_t b ) { return ( a > 0 ) ? powf( a, b ) : 0; }
static inline f2_t f2_erf  ( f2_t a ) { return erff( a ); }
static inline f2_t f2_sin  ( f2_t a ) { return sinf( a ); }
static inline f2_t f2_cos  ( f2_t a ) { return cosf( a ); }
static inline f2_t f2_tan  ( f2_t a ) { return tanf( a ); }
static inline f2_t f2_tanh ( f2_t a ) { return tanhf( a ); }
static inline f2_t f2_ferm ( f2_t a ) { return 1.0 / ( 1.0 + exp( -a ) );    } // fermi function (2ferm(2x)-1 == tanh(x))
static inline f2_t f2_sign ( f2_t a ) { return a < 0 ? -1 : a > 0 ? 1 : 0  ; } // sign function
static inline f2_t f2_ustp ( f2_t a ) { return a < 0 ?  0 : a > 0 ? 1 : 0.5; } // unit step function (heavyside function)
static inline f2_t f2_sqrt ( f2_t a ) { return sqrt( a ); }
static inline f2_t f2_ceil ( f2_t a ) { return ceilf( a ); }
static inline f2_t f2_floor( f2_t a ) { return floorf( a ); }

/// random generator (range -1, 1)
static inline f2_t f2_rnd_sym(  u3_t* rv ) { return ( *rv = bcore_lcg01_u3( *rv ) ) * ( 2.0 / 0xFFFFFFFFFFFFFFFFull ) - 1.0; }

/// random generator (range 0, 1)
static inline f2_t f2_rnd_pos(  u3_t* rv ) { return ( *rv = bcore_lcg01_u3( *rv ) ) * ( 1.0 / 0xFFFFFFFFFFFFFFFFull ); }

/**********************************************************************************************************************/
// sz_t

static inline sz_t sz_sqr( sz_t v ) { return v * v; }
static inline sz_t sz_min( sz_t a, sz_t b ) { return a < b ? a : b; }
static inline sz_t sz_max( sz_t a, sz_t b ) { return a > b ? a : b; }
static inline sz_t sz_sig( sz_t v ) { return v < 0 ? -1 : 1; }
static inline sz_t sz_abs( sz_t v ) { return v < 0 ? -v : v; }
static inline void sz_t_swap( sz_t* v1, sz_t* v2 ) { sz_t t = *v1; *v1 = *v2; *v2 = t; }

// greatest common divisor
static inline sz_t sz_gcd( sz_t a, sz_t b ) { return ( a == 0 ) ? b : sz_gcd( b % a, a ); }

/**********************************************************************************************************************/
// uz_t

static inline uz_t uz_sqr( uz_t v ) { return v * v; }
static inline uz_t uz_min( uz_t a, uz_t b ) { return a < b ? a : b; }
static inline uz_t uz_max( uz_t a, uz_t b ) { return a > b ? a : b; }
static inline void uz_t_swap( uz_t* v1, uz_t* v2 ) { uz_t t = *v1; *v1 = *v2; *v2 = t; }

// greatest common divisor
static inline uz_t uz_gcd( uz_t a, uz_t b ) { return ( a == 0 ) ? b : uz_gcd( b % a, a ); }

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
static inline s3_t s3_sqr( s3_t v ) { return v * v; }
static inline void s3_t_swap( s3_t* v1, s3_t* v2 ) { s3_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/
// s2_t

static inline s2_t s2_min( s2_t a, s2_t b ) { return a < b ? a : b; }
static inline s2_t s2_max( s2_t a, s2_t b ) { return a > b ? a : b; }
static inline s2_t s2_sig( s2_t v ) { return v < 0 ? -1 : 1; }
static inline s2_t s2_abs( s2_t v ) { return v < 0 ? -v : v; }
static inline s2_t s2_sqr( s3_t v ) { return v * v; }
static inline void s2_t_swap( s2_t* v1, s2_t* v2 ) { s2_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/
// s1_t

static inline s1_t s1_min( s1_t a, s1_t b ) { return a < b ? a : b; }
static inline s1_t s1_max( s1_t a, s1_t b ) { return a > b ? a : b; }
static inline s1_t s1_sig( s1_t v ) { return v < 0 ? -1 : 1; }
static inline s1_t s1_abs( s1_t v ) { return v < 0 ? -v : v; }
static inline s1_t s1_sqr( s1_t v ) { return v * v; }
static inline void s1_t_swap( s1_t* v1, s1_t* v2 ) { s1_t t = *v1; *v1 = *v2; *v2 = t; }

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
// u1_t

static inline u1_t u1_min( u1_t a, u1_t b ) { return a < b ? a : b; }
static inline u1_t u1_max( u1_t a, u1_t b ) { return a > b ? a : b; }
static inline void u1_t_swap( u1_t* v1, u1_t* v2 ) { u1_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/
// vd_t

static inline void vd_t_swap( vd_t* v1, vd_t* v2 ) { vd_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/
// vc_t

static inline void vc_t_swap( vc_t* v1, vc_t* v2 ) { vc_t t = *v1; *v1 = *v2; *v2 = t; }

/**********************************************************************************************************************/

vd_t bcore_leaf_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_LEAF_H

