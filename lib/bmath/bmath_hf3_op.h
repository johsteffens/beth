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

#ifndef BMATH_HF3_OP_H
#define BMATH_HF3_OP_H

/**********************************************************************************************************************/

/// Objects (operators) related to f3_t

/**********************************************************************************************************************/

#include "bmath_planted.h"
#include "bmath_hf3.h"

/**********************************************************************************************************************/

PLANT_GROUP( bmath_hf3_op, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Operators on scalars
 *  Principal functions
 *     [o]fx  - returns the operator's result given arguments 'x' according to arity (x is only given for arity >= 1)
 *     [o]gxi - returns the gradient for argument 'i' given arguments 'x' (i is only given for arity >= 2)
 *     [o]gyi - returns the gradient for argument 'i' given arguments 'x' but the i-th argument swapped for y=fx
 *     Prefix o is used for generic :op level functions
 */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'ap' sz_t get_arity( const );

/// a represents an array of size arity
feature 'ap' void ofx( const,         const bmath_hf3_s** a, bmath_hf3_s* r );
feature 'ap' void ogx( const, sz_t i, const bmath_hf3_s** a, bmath_hf3_s* r );
feature 'ap' void ogy( const, sz_t i, const bmath_hf3_s** a, bmath_hf3_s* r );

/// nullary operators
group :ar0 =
{
    func :: :get_arity = { return 0; };
    func :: :ofx = { @f( o, r ); };

    feature 'a' void f( const, bmath_hf3_s* r );

    stamp :zero    = aware : {                   func : :f = { bmath_hf3_s_zro( r ); }; };
    stamp :literal = aware : { bmath_hf3_s -> h; func : :f = { bmath_hf3_s_cpy( o->h, r ); }; };

    stamp :f3      = aware :
    {
        bmath_f3_op_ar0 -> op;
        func : :f = { bmath_hf3_s_fp_f3_op_ar0( r, ( bmath_fp_f3_op_ar0 )bmath_f3_op_ar0_s_get_aware( o->op )->f, o->op ); };
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// unary operators
group :ar1 =
{
    func :: :get_arity = { return 1; };
    func :: :ofx = { @fx( o, a[0], r ); };
    func :: :ogy = { @gy( o, a[0], r ); };

    feature 'a' void fx( const, const bmath_hf3_s* a, bmath_hf3_s* r );
    feature 'a' void gy( const, const bmath_hf3_s* y, bmath_hf3_s* r );

    stamp :identity = aware :
    {
        func : :fx = { bmath_hf3_s_cpy( a, r ); };
        func : :gy = { bmath_hf3_s_set_f3( r, 1.0 ); };
    };

    stamp :f3 = aware :
    {
        bmath_f3_op_ar1 -> op;
        func : :fx = { bmath_hf3_s_fp_f3_op_ar1( a, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_s_get_aware( o->op )->fx, o->op, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_op_ar1( y, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_s_get_aware( o->op )->gy, o->op, r ); };
    };

    // ======= (logistic function) ============

    stamp :lgst = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_op_ar1( a, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_lgst_s_fx, NULL, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_op_ar1( y, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_lgst_s_gy, NULL, r ); };
    };

    stamp :lgst_hard = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_op_ar1( a, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_lgst_hard_s_fx, NULL, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_op_ar1( y, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_lgst_hard_s_gy, NULL, r ); };
    };

    stamp :lgst_leaky = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_op_ar1( a, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_lgst_leaky_s_fx, NULL, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_op_ar1( y, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_lgst_leaky_s_gy, NULL, r ); };
    };

    // ======= (tanh) =========================

    stamp :tanh = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_op_ar1( a, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_tanh_s_fx, NULL, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_op_ar1( y, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_tanh_s_gy, NULL, r ); };
    };

    stamp :tanh_hard = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_op_ar1( a, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_tanh_hard_s_fx, NULL, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_op_ar1( y, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_tanh_hard_s_gy, NULL, r ); };
    };

    stamp :tanh_leaky = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_op_ar1( a, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_tanh_leaky_s_fx, NULL, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_op_ar1( y, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_tanh_leaky_s_gy, NULL, r ); };
    };

    // ======= (softplus function) ============

    stamp :softplus = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_op_ar1( a, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_softplus_s_fx, NULL, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_op_ar1( y, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_softplus_s_gy, NULL, r ); };
    };

    stamp :relu = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_op_ar1( a, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_relu_s_fx, NULL, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_op_ar1( y, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_relu_s_gy, NULL, r ); };
    };

    stamp :relu_leaky = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_op_ar1( a, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_relu_leaky_s_fx, NULL, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_op_ar1( y, ( bmath_fp_f3_op_ar1 )bmath_f3_op_ar1_relu_leaky_s_gy, NULL, r ); };
    };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// binary operators
group :ar2 =
{
    func :: :get_arity = { return 2; };
    func :: :ofx = { @fx( o, a[0], a[1], r ); };
    func :: :ogx = { if( i == 0 ) { @gxa( o, a[0], a[1], r ); } else { @gxb( o, a[0], a[1], r ); } };

    feature 'a' void fx(  const, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );
    feature 'a' void gxa( const, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );
    feature 'a' void gxb( const, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );

    stamp :add = aware :
    {
        func : :fx  = { bmath_hf3_s_add( a, b, r ); };
        func : :gxa = { bmath_hf3_s_set_f3( r, 1.0 ); };
        func : :gxb = { bmath_hf3_s_set_f3( r, 1.0 ); };
    };

    stamp :sub = aware :
    {
        func : :fx  = { bmath_hf3_s_add( a, b, r ); };
        func : :gxa = { bmath_hf3_s_set_f3( r,  1.0 ); };
        func : :gxb = { bmath_hf3_s_set_f3( r, -1.0 ); };
    };

    stamp :hmul = aware :
    {
        func : :fx  = { bmath_hf3_s_hmul( a, b, r ); };
        func : :gxa = { bmath_hf3_s_cpy( b, r ); };
        func : :gxb = { bmath_hf3_s_cpy( a, r ); };
    };

    stamp :div = aware :
    {
        func : :fx  = { bmath_hf3_s_fp_f3_op_ar2( a, b, ( bmath_fp_f3_op_ar2 )bmath_f3_op_ar2_div_s_fx,  NULL, r ); };
        func : :gxa = { bmath_hf3_s_fp_f3_op_ar2( a, b, ( bmath_fp_f3_op_ar2 )bmath_f3_op_ar2_div_s_gxa, NULL, r ); };
        func : :gxb = { bmath_hf3_s_fp_f3_op_ar2( a, b, ( bmath_fp_f3_op_ar2 )bmath_f3_op_ar2_div_s_gxb, NULL, r ); };
    };
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t bmath_hf3_op_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_HF3_OP_H



