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

#ifndef BMATH_LEAF_H
#define BMATH_LEAF_H

/// functionality related to elementary types

#include "bcore_signal.h"
#include "bcore_leaf.h"
#include "bmath_hwflags.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/
// f3_t

/// math constants
static inline f3_t f3_pi(   void ) { return 3.1415926535897932384626434; } // PI
static inline f3_t f3_pi_2( void ) { return 1.5707963267948966192313217; } // PI / 2
static inline f3_t f3_tau(  void ) { return 6.2831853071795864769252868; } // 2 * PI

static inline bl_t f3_is_nan( f3_t v ) { return v != v; } // nan compares unequal to itself

/// functions
static inline f3_t f3_exp( f3_t v ) { return exp( v ); }
static inline f3_t f3_log( f3_t v ) { return v > 0 ? log( v ) : -f3_lim_max; }
static inline f3_t f3_pow( f3_t a, f3_t b ) { return ( a > 0 ) ? pow( a, b ) : 0; }
static inline f3_t f3_erf( f3_t a ) { return erf( a ); }

/// random generator (range -1, 1)
static inline f3_t f3_rnd_sym(  u3_t* rv ) { return ( *rv = bcore_lcg01_u3( *rv ) ) * ( 2.0 / 0xFFFFFFFFFFFFFFFFull ) - 1.0; }

/// random generator (range 0, 1)
static inline f3_t f3_rnd_pos(  u3_t* rv ) { return ( *rv = bcore_lcg01_u3( *rv ) ) * ( 1.0 / 0xFFFFFFFFFFFFFFFFull ); }

u2_t bmath_u2_argb_from_f3( vd_t, f3_t v ); // fist argument ignored (can be NULL)

typedef f3_t (*bmath_fp_f3_ar0 )( void           ); // nullary function   -> f3
typedef f3_t (*bmath_fp_f3_ar1 )( f3_t a         ); // unary function  f3 -> f3
typedef f3_t (*bmath_fp_f3_ar2 )( f3_t a, f3_t b ); // binary function (f3,f3) -> f3

typedef f3_t (*bmath_fp_f3_op_ar0 )( vc_t op );                 // arity 0 operator
typedef f3_t (*bmath_fp_f3_op_ar1 )( vc_t op, f3_t a );         // arity 1 operator
typedef f3_t (*bmath_fp_f3_op_ar2 )( vc_t op, f3_t a, f3_t b ); // arity 2 operator

/**********************************************************************************************************************/
// f2_t

/// math constants
static inline f2_t f2_pi(   void ) { return 3.1415926535897932384626434; } // PI
static inline f2_t f2_pi_2( void ) { return 1.5707963267948966192313217; } // PI / 2
static inline f2_t f2_tau(  void ) { return 6.2831853071795864769252868; } // 2 * PI

static inline bl_t f2_is_nan( f2_t v ) { return v != v; } // nan compares unequal to itself

/// functions
static inline f2_t f2_exp( f2_t v ) { return expf( v ); }
static inline f2_t f2_log( f2_t v ) { return v > 0 ? logf( v ) : -f2_lim_max; }
static inline f2_t f2_pow( f2_t a, f2_t b ) { return ( a > 0 ) ? powf( a, b ) : 0; }
static inline f2_t f2_erf( f2_t a ) { return erff( a ); }

/// random generator (range -1, 1)
static inline f2_t f2_rnd_sym(  u3_t* rv ) { return ( *rv = bcore_lcg01_u3( *rv ) ) * ( 2.0 / 0xFFFFFFFFFFFFFFFFull ) - 1.0; }

/// random generator (range 0, 1)
static inline f2_t f2_rnd_pos(  u3_t* rv ) { return ( *rv = bcore_lcg01_u3( *rv ) ) * ( 1.0 / 0xFFFFFFFFFFFFFFFFull ); }

u2_t bmath_u2_argb_from_f2( vd_t, f2_t v ); // fist argument ignored (can be NULL)

typedef f2_t (*bmath_fp_f2_ar0 )( void           ); // nullary function   -> f2
typedef f2_t (*bmath_fp_f2_ar1 )( f2_t a         ); // unary function  f2 -> f2
typedef f2_t (*bmath_fp_f2_ar2 )( f2_t a, f2_t b ); // binary function (f2,f2) -> f2

typedef f2_t (*bmath_fp_f2_op_ar0 )( vc_t op );                 // arity 0 operator
typedef f2_t (*bmath_fp_f2_op_ar1 )( vc_t op, f2_t a );         // arity 1 operator
typedef f2_t (*bmath_fp_f2_op_ar2 )( vc_t op, f2_t a, f2_t b ); // arity 2 operator

/**********************************************************************************************************************/

vd_t bmath_leaf_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_LEAF_H

