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

/** Framework of n-ari operators spelled out on scalars and providing a structure for
 *  other operands.
 *
 *  There are two kinds of principal functions
 *    f - general forward mapping. Called fx for arity > 0.
 *    g - gradient of a specific argument by partial derivative for arity > 0.
 *
 *  Functors (functions taking the operator instance as first argument) have prefix 'o'.
 *  Of these the virtual f3_op-functors have additional prefix 'a' because they need different arguments.
 *  Gradient functions 'g' have two kinds of (first) postfix:
 *    x - using all input operands of 'f'
 *    y - using the output of 'f' for the argument channel for which the gradient is computed. The rest is the same as 'x'
 *  A second postfix indicates the the f-argument for which the gradient is computed.
 *  Example: gxb: compute the gradient of argument 'b' of function 'f'
 *
 */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'ap' sz_t get_arity( const );

/// a represents an array of size arity
feature 'ap' f3_t aofx( const, const f3_t* a );
feature 'ap' f3_t aogx( const, const f3_t* a, sz_t ch );  // ch: argument channel number [0, ..., arity -1]
feature 'ap' f3_t aogy( const, const f3_t* a, sz_t ch );

/// nullary operators
group :ar0 =
{
    signature    f3_t  f( plain );
    feature 'ap' f3_t of( const );

    func :: :get_arity = { return 0; };
    func :  :f    = { ERR_fa( "Not available." ); return 0; };
    func :  :of   = { return @f(); };
    func :: :aofx = { return @of( o ); };

    stamp :zero    = aware : {         func : :f  = { return  0.0; }; };
    stamp :one     = aware : {         func : :f  = { return  1.0; }; };
    stamp :literal = aware : { f3_t v; func : :of = { return o->v; }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// unary operators
group :ar1 =
{
    signature f3_t fx( plain, f3_t a );
    signature f3_t gy( plain, f3_t y );

    feature 'ap' f3_t ofx( const, f3_t a );
    feature 'ap' f3_t ogy( const, f3_t y );

    func :: :get_arity = { return 1; };
    func  : :  fx = { ERR_fa( "Not available." ); return 0; };
    func  : :  gy = { ERR_fa( "Not available." ); return 0; };
    func  : : ofx = { return @fx( a ); };
    func  : : ogy = { return @gy( y ); };
    func :: :aofx = { return @ofx( o, a[0] ); };
    func :: :aogy = { return @ogy( o, a[0] ); };

    // ======= (elementary functions) ============

    stamp :zero     = aware : { func : :fx = { return       0.0; }; func : :gy = { return  0.0; }; };
    stamp :one      = aware : { func : :fx = { return       1.0; }; func : :gy = { return  0.0; }; };
    stamp :identity = aware : { func : :fx = { return        a ; }; func : :gy = { return  1.0; }; };
    stamp :neg      = aware : { func : :fx = { return       -a ; }; func : :gy = { return -1.0; }; };
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
    signature f3_t fx ( plain, f3_t a, f3_t b );
    signature f3_t gxa( plain, f3_t a, f3_t b );
    signature f3_t gxb( plain, f3_t a, f3_t b );

    feature 'ap' f3_t ofx ( const, f3_t a, f3_t b );
    feature 'ap' f3_t ogxa( const, f3_t a, f3_t b );
    feature 'ap' f3_t ogxb( const, f3_t a, f3_t b );

    func  : :fx  = { ERR_fa( "Not available." ); return 0; };
    func  : :gxa = { ERR_fa( "Not available." ); return 0; };
    func  : :gxb = { ERR_fa( "Not available." ); return 0; };

    func  : :ofx  = { return @fx(  a, b ); };
    func  : :ogxa = { return @gxa( a, b ); };
    func  : :ogxb = { return @gxb( a, b ); };

    func :: :get_arity = { return 2; };
    func :: :aofx = { return @ofx( o, a[0], a[1] ); };
    func :: :aogx = { return ( ch == 0 ) ? @ogxa( o, a[0], a[1] ) : @ogxb( o, a[0], a[1] ); };

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



