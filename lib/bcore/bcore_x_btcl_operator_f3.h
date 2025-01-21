/** Author and Copyright 2025 Johannes Bernhard Steffens
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

#ifndef BCORE_BTCL_X_OPERATOR_F3_H
#define BCORE_BTCL_X_OPERATOR_F3_H

#include "bcore_leaf.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

typedef f3_t (*x_btcl_operator_f3_fp_f3_ar0 )( void                   ); // nullary function   -> f3
typedef f3_t (*x_btcl_operator_f3_fp_f3_ar1 )( f3_t a                 ); // unary function  f3 -> f3
typedef f3_t (*x_btcl_operator_f3_fp_f3_ar2 )( f3_t a, f3_t b         ); // binary function (f3,f3) -> f3
typedef f3_t (*x_btcl_operator_f3_fp_f3_ar3 )( f3_t a, f3_t b, f3_t c ); // ternary function (f3,f3,f3) -> f3

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// unary operators

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// binary operators

//----------------------------------------------------------------------------------------------------------------------

static inline f3_t x_btcl_operator_f3_pow_get          ( f3_t a, f3_t b ) { return f3_pow( a, b ); }
static inline f3_t x_btcl_operator_f3_div_get          ( f3_t a, f3_t b ) { return ( b != 0 ) ? a / b : 0; }
static inline f3_t x_btcl_operator_f3_mul_get          ( f3_t a, f3_t b ) { return a * b; }
static inline f3_t x_btcl_operator_f3_sub_get          ( f3_t a, f3_t b ) { return a - b; }
static inline f3_t x_btcl_operator_f3_add_get          ( f3_t a, f3_t b ) { return a + b; }
static inline f3_t x_btcl_operator_f3_equal_get        ( f3_t a, f3_t b ) { return ( a == b ); }
static inline f3_t x_btcl_operator_f3_unequal_get      ( f3_t a, f3_t b ) { return ( a != b ); }
static inline f3_t x_btcl_operator_f3_larger_equal_get ( f3_t a, f3_t b ) { return ( a >= b ); }
static inline f3_t x_btcl_operator_f3_larger_get       ( f3_t a, f3_t b ) { return ( a >  b ); }
static inline f3_t x_btcl_operator_f3_smaller_equal_get( f3_t a, f3_t b ) { return ( a <= b ); }
static inline f3_t x_btcl_operator_f3_smaller_get      ( f3_t a, f3_t b ) { return ( a <  b ); }
static inline f3_t x_btcl_operator_f3_and_get          ( f3_t a, f3_t b ) { return ( a && b ); }
static inline f3_t x_btcl_operator_f3_or_get           ( f3_t a, f3_t b ) { return ( a || b ); }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// ternary operators

//----------------------------------------------------------------------------------------------------------------------

static inline f3_t x_btcl_operator_f3_conditional_get( f3_t a, f3_t b, f3_t c ) { return ( a ) ? b : c; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif  // BCORE_BTCL_X_OPERATOR_F3_H
