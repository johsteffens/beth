/** Copyright 2019 Johannes Bernhard Steffens
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

#ifndef BMATH_F3_OP_H
#define BMATH_F3_OP_H

/**********************************************************************************************************************/

/// Objects (operators) related to f3_t

/**********************************************************************************************************************/

#include "bmath_planted.h"
#include "bmath_leaf.h"

/**********************************************************************************************************************/

PLANT_GROUP( bmath_f3_op, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Operators on scalars
 *  Principal functions
 *     [o]fx  - returns the operator's result given arguments 'x' according to arity (x is only given for arity >= 1)
 *     [o]gxi - returns the gradient for argument 'i' given arguments 'x' (i is only given for arity >= 2)
 *     [o]gyi - returns the gradient for argument 'i' given arguments 'x' but the i-th argument swapped for y=fx
 *     Prefix o is used for generic :op level functions
 */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'ap' sz_t get_arity( const );

/// a represents an array of size arity
feature 'ap' f3_t ofx(  const, const f3_t* a );
feature 'ap' f3_t ogxi( const, const f3_t* a, sz_t i );
feature 'ap' f3_t ogyi( const, const f3_t* a, sz_t i );

/// nullary operators
group :ar0 =
{
    feature 'a' f3_t f( const );

    func :: :get_arity = { return 0; };
    func :: :ofx  = { return @f( o ); };

    stamp :zero    = aware : {         func : :f = { return  0.0; }; };
    stamp :one     = aware : {         func : :f = { return  1.0; }; };
    stamp :literal = aware : { f3_t v; func : :f = { return o->v; }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// unary operators
group :ar1 =
{
    feature 'a' f3_t fx( const, f3_t a );
    feature 'a' f3_t gy( const, f3_t y );

    func :: :get_arity = { return 1; };
    func :: :ofx  = { return @fx( o, a[0] ); };
    func :: :ogyi = { return @gy( o, a[0] ); };

    // ======= (elementary functions) ============

    stamp :zero     = aware : { func : :fx = { return       0.0; }; func : :gy = { return  0.0; }; };
    stamp :one      = aware : { func : :fx = { return       1.0; }; func : :gy = { return  0.0; }; };
    stamp :identity = aware : { func : :fx = { return        a ; }; func : :gy = { return  1.0; }; };
    stamp :exp      = aware : { func : :fx = { return    exp(a); }; func : :gy = { return    y; }; };
    stamp :inv      = aware : { func : :fx = { return f3_inv(a); }; func : :gy = { return -y*y; }; };

    // ======= (logistic function) ============

    stamp :lgst = aware :
    {
        func : :fx = { return ( a > -700 ) ? ( 1.0 / ( 1.0 + exp( -a ) ) ) : 0; };
        func : :gy = { return y * ( 1.0 - y ); };
    };

    stamp :lgst_hard = aware : // lgst approximation
    {
        func : :fx = { return ( a < -2.0 ) ? 0.0 : ( a > 2.0 ) ? 1.0 : 0.25 * ( a + 2.0 ); };
        func : :gy = { return ( y <  0.0 ) ? 0.0 : ( y > 1.0 ) ? 0.0 : 0.25; };
    };

    stamp :lgst_leaky = aware : // lgst approximation
    {
        func : :fx = { return ( a < -2.0 ) ? 0.01 * ( a + 2.0 ) : ( a > 2.0 ) ? 1.0 + 0.01 * ( a - 2.0 ) : 0.25 * ( a + 2.0 ); };
        func : :gy = { return ( y <  0.0 ) ? 0.01 : ( y > 1.0 ) ? 0.01 : 0.25; };
    };

    // ======= (tanh) =========================

    stamp :tanh = aware :
    {
        func : :fx = { return ( a < 350 ) ? ( 1.0 - ( 2.0 / ( exp( 2.0 * a ) + 1.0 ) ) ) : 1.0; };
        func : :gy = { return 1.0 - f3_sqr( y ); };
    };

    stamp :tanh_hard = aware : // tanh approximation
    {
        func : :fx = { return ( a < -1.0 ) ? -1.0 : ( a > 1.0 ) ? 1.0 : a; };
        func : :gy = { return ( y < -1.0 ) ?  0.0 : ( y > 1.0 ) ? 0.0 : 1.0; };
    };

    stamp :tanh_leaky = aware : // tanh approximation
    {
        func : :fx = { return ( a < -1.0 ) ? -1.0 + 0.01 * ( a + 1.0 ) : ( a > 1.0 ) ? 1.0 + 0.01 * ( a - 1.0 ) : a; };
        func : :gy = { return ( y < -1.0 ) ?  0.01 : ( y > 1.0 ) ? 0.01 : 1.0; };
    };

    // ======= (softplus function) ============

    stamp :softplus = aware :
    {
        func : :fx = { return ( a < 700 ) ? log( 1.0 + exp( a ) ) : a; };
        func : :gy = { f3_t u = exp( y ); return ( u - 1.0 ) / u; };
    };

    stamp :relu = aware : // relu: hard approximation of softplus
    {
        func : :fx = { return a > 0 ? a : 0; };
        func : :gy = { return y > 0 ? 1 : 0; };
    };

    stamp :relu_leaky = aware :
    {
        func : :fx = { return a > 0 ? a : a * 0.01; };
        func : :gy = { return y > 0 ? 1 : 0.01; };
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// binary operators
group :ar2 =
{
    feature 'a' f3_t fx ( const, f3_t a, f3_t b );
    feature 'a' f3_t gxa( const, f3_t a, f3_t b );
    feature 'a' f3_t gxb( const, f3_t a, f3_t b );

    func :: :get_arity = { return 2; };
    func :: :ofx  = { return @fx( o, a[0], a[1] ); };
    func :: :ogxi = { return ( i == 0 ) ? @gxa( o, a[0], a[1] ) : @gxb( o, a[0], a[1] ); };

    stamp :add = aware :
    {
        func : :fx  = { return a + b; };
        func : :gxa = { return 1; };
        func : :gxb = { return 1; };
    };

    stamp :sub = aware :
    {
        func : :fx  = { return a - b; };
        func : :gxa = { return  1; };
        func : :gxb = { return -1; };
    };

    stamp :mul = aware :
    {
        func : :fx  = { return a * b; };
        func : :gxa = { return b; };
        func : :gxb = { return a; };
    };

    stamp :div = aware :
    {
        func : :fx  = { return b != 0 ?   a / b         :  a * f3_lim_inf; };
        func : :gxa = { return b != 0 ? 1.0 / b         :      f3_lim_inf; };
        func : :gxb = { return b != 0 ?  -a / ( b * b ) : -a * f3_lim_inf; };
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t bmath_f3_op_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_F3_OP_H



