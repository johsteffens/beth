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

/** Operators on holors.
 *
 *  Dendride-Pass:
 *  Dendrite-Pass (DP) Functors  (Updates the gradient of a specified input channel)
 *  Nomenclature <name>_<ch>_<arg_sig>
 *  name: Name of the axon-pass functor from which the dp is computed
 *  ch:   Specified Input channel (a, b, c , ... )
 *  arg_sig: Signature of require arguments in given order.
 *  Example: lgst_dpa_yg
 *     Dendride-pass of functor 'lgst' through channel 'a' requiring 'y' (the output of lgst)
 *     and the (pre-)gradient as input.
 */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'ap' sz_t get_arity( const );

/// a represents an array of size arity
feature 'ap' void aof( const, const bmath_hf3_s** a, bmath_hf3_s* r );

/// nullary operators
group :ar0 =
{
    signature    void  f( plain, bmath_hf3_s* r );
    feature 'ap' void of( const, bmath_hf3_s* r );

    func :: :get_arity = { return 0; };
    func :    :f = { ERR_fa( "Not available." ); };
    func :   :of = { @f( r ); };
    func :: :aof = { @of( o, r ); };

    stamp :zero    = aware : {                   func : : f = { bmath_hf3_s_zro(       r ); }; };
    stamp :literal = aware : { bmath_hf3_s -> h; func : :of = { bmath_hf3_s_cpy( o->h, r ); }; };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// unary operators
group :ar1 =
{
    signature void f( plain, const bmath_hf3_s* a, bmath_hf3_s* r );

    feature 'ap' void of( const, const bmath_hf3_s* a, bmath_hf3_s* r );

    func :: :get_arity = { return 1; };

    func  : :  f = { ERR_fa( "Not available." ); };
    func  : : of = {  @f(    a   , r ); };
    func :: :aof = { @of( o, a[0], r ); };

    stamp :identity   = aware : { func : :f = { bmath_hf3_s_cpy( a, r ); }; };

    // logistic
    stamp :lgst       = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_s_fx,       r ); }; };
    stamp :lgst_hard  = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_hard_s_fx,  r ); }; };
    stamp :lgst_leaky = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_leaky_s_fx, r ); }; };

    // tanh
    stamp :tanh       = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_s_fx,       r ); }; };
    stamp :tanh_hard  = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_hard_s_fx,  r ); }; };
    stamp :tanh_leaky = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_leaky_s_fx, r ); }; };

    // softplus
    stamp :softplus   = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_softplus_s_fx,   r ); }; };
    stamp :relu       = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_relu_s_fx,       r ); }; };
    stamp :relu_leaky = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_relu_leaky_s_fx, r ); }; };

    /// ======= Dendride-Pass ============

    stamp :add_dpa_g  = aware : { func : :f = { bmath_hf3_s_add( r, a, r ); }; };
    stamp :add_dpb_g  = aware : { func : :f = { bmath_hf3_s_add( r, a, r ); }; };
    stamp :sub_dpa_g  = aware : { func : :f = { bmath_hf3_s_add( r, a, r ); }; };
    stamp :sub_dpb_g  = aware : { func : :f = { bmath_hf3_s_sub( r, a, r ); }; };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// binary operators
group :ar2 =
{
    signature    void f(  plain, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );
    feature 'ap' void of( const, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );

    func :: :get_arity = { return 2; };
    func  : :  f = { ERR_fa( "Not available." ); };
    func  : : of = {  @f( a, b, r ); };
    func :: :aof = { @of( o, a[0], a[1], r ); };

    stamp :add  = aware : { func : :f  = { bmath_hf3_s_add ( a, b, r ); }; };
    stamp :sub  = aware : { func : :f  = { bmath_hf3_s_add ( a, b, r ); }; };
    stamp :hmul = aware : { func : :f  = { bmath_hf3_s_hmul( a, b, r ); }; };
    stamp :hdiv = aware : { func : :f  = { bmath_hf3_s_fp_f3_ar2( a, b, bmath_f3_op_ar2_div_s_fx, r ); }; };

    /// ======= Dendride-Pass ============

    // logistic
    stamp :lgst_dpa_yg       = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_lgst_s_gy,       b, r ); }; };
    stamp :lgst_hard_dpa_yg  = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_lgst_hard_s_gy,  b, r ); }; };
    stamp :lgst_leaky_dpa_yg = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_lgst_leaky_s_gy, b, r ); }; };

    // tanh
    stamp :tanh_dpa_yg       = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_tanh_s_gy,       b, r ); }; };
    stamp :tanh_hard_dpa_yg  = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_tanh_hard_s_gy,  b, r ); }; };
    stamp :tanh_leaky_dpa_yg = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_tanh_leaky_s_gy, b, r ); }; };

    // softplus
    stamp :softplus_dpa_yg   = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_softplus_s_gy,   b, r ); }; };
    stamp :relu_dpa_yg       = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_relu_s_gy,       b, r ); }; };
    stamp :relu_leaky_dpa_yg = aware : { func : :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_relu_leaky_s_gy, b, r ); }; };

    // hmul
    stamp :hmul_dpa_bg       = aware : { func : :f = { bmath_hf3_s_hmul_add( a, b, r, r ); }; };
    stamp :hmul_dpb_ag       = aware : { func : :f = { bmath_hf3_s_hmul_add( a, b, r, r ); }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// ternary operators
group :ar3 =
{
    signature    void f(  plain, const bmath_hf3_s* a, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r );
    feature 'ap' void of( const, const bmath_hf3_s* a, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r );

    func :: :get_arity = { return 3; };
    func  : :  f = { ERR_fa( "Not available." ); };
    func  : : of = {  @f( a, b, c, r ); };
    func :: :aof = { @of( o, a[0], a[1], a[2], r ); };

    /// ======= Dendride-Pass ============

    // hdiv
    stamp :hdiv_dpa_abg = aware : { func : :f = { bmath_hf3_s_fp_f3_ar2_madd( a, b, bmath_f3_op_ar2_div_s_gxa, c, r ); }; };
    stamp :hdiv_dpb_abg = aware : { func : :f = { bmath_hf3_s_fp_f3_ar2_madd( a, b, bmath_f3_op_ar2_div_s_gxb, c, r ); }; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t bmath_hf3_op_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_HF3_OP_H



