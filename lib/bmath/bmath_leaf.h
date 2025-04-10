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

u2_t bmath_u2_argb_from_f3( vd_t, f3_t v ); // fist argument ignored (can be NULL)

typedef f3_t (*bmath_fp_f3_ar0 )( void                   ); // nullary function   -> f3
typedef f3_t (*bmath_fp_f3_ar1 )( f3_t a                 ); // unary function  f3 -> f3
typedef f3_t (*bmath_fp_f3_ar2 )( f3_t a, f3_t b         ); // binary function (f3,f3) -> f3
typedef f3_t (*bmath_fp_f3_ar3 )( f3_t a, f3_t b, f3_t c ); // ternary function (f3,f3) -> f3

typedef f3_t (*bmath_fp_f3_op_ar0 )( vc_t op );                 // arity 0 operator
typedef f3_t (*bmath_fp_f3_op_ar1 )( vc_t op, f3_t a );         // arity 1 operator
typedef f3_t (*bmath_fp_f3_op_ar2 )( vc_t op, f3_t a, f3_t b ); // arity 2 operator

/**********************************************************************************************************************/
// f2_t

u2_t bmath_u2_argb_from_f2( vd_t, f2_t v ); // fist argument ignored (can be NULL)

typedef f2_t (*bmath_fp_f2_ar0 )( void                   ); // nullary function   -> f2
typedef f2_t (*bmath_fp_f2_ar1 )( f2_t a                 ); // unary function  f2 -> f2
typedef f2_t (*bmath_fp_f2_ar2 )( f2_t a, f2_t b         ); // binary function (f2,f2) -> f2
typedef f2_t (*bmath_fp_f2_ar3 )( f2_t a, f2_t b, f2_t c ); // ternary function (f2,f2,f2) -> f2

typedef f2_t (*bmath_fp_f2_op_ar0 )( vc_t op );                 // arity 0 operator
typedef f2_t (*bmath_fp_f2_op_ar1 )( vc_t op, f2_t a );         // arity 1 operator
typedef f2_t (*bmath_fp_f2_op_ar2 )( vc_t op, f2_t a, f2_t b ); // arity 2 operator

/**********************************************************************************************************************/

vd_t bmath_leaf_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_LEAF_H

