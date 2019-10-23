/** Author and Copyright 2019 Johannes Bernhard Steffens
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
 */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'ap' sz_t get_arity( const );

/// a represents an array of size arity
feature 'ap' void aof( const, const bmath_hf3_s** a, bmath_hf3_s* r );

/// operator signature (nontrivial for d-pass functions)
signature    sc_t  sig( plain );
feature 'ap' sc_t osig( const );

/// nullary operators
group :ar0 =
{
    extending stump verbatim :_ = aware : {};

    signature    void  f( plain, bmath_hf3_s* r );
    feature 'ap' void of( const, bmath_hf3_s* r );

    func :: :get_arity = { return 0; };
    func  : :  f = { ERR_fa( "Function 'f' is not available. Need to call 'of' instead?" ); };
    func  : : of = { @f( r ); };
    func :: :aof = { @of( o, r ); };

    stamp :zro     = {                   func : : f = { bmath_hf3_s_zro(       r ); }; };
    stamp :literal = { bmath_hf3_s -> h; func : :of = { bmath_hf3_s_cpy( o->h, r ); }; };

    /// dendride-pass
    group :dp =
    {
        extending stump verbatim :_ = aware : {};

        func ::: :sig = { return "u"; };
        stamp :ca_floor = { func :: :f = { bmath_hf3_s_zro( r ); }; };
        stamp :ca_ceil  = { func :: :f = { bmath_hf3_s_zro( r ); }; };
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// unary operators
group :ar1 =
{
    extending stump verbatim :_ = aware : {};

    signature void f( plain, const bmath_hf3_s* a, bmath_hf3_s* r );

    feature 'ap' void of( const, const bmath_hf3_s* a, bmath_hf3_s* r );

    func :: :get_arity = { return 1; };

    func  : :  f = { ERR_fa( "Function 'f' is not available. Need to call 'of' instead?" ); };
    func  : : of = {  @f(    a   , r ); };
    func :: :aof = { @of( o, a[0], r ); };

    stamp :cpy        = { func : :f = { bmath_hf3_s_cpy( a, r ); }; };
    stamp :neg        = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_neg_s_fx, r ); }; };
    stamp :floor      = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, floor, r ); }; };
    stamp :ceil       = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, ceil,  r ); }; };
    stamp :exp        = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, exp,   r ); }; };

    // logistic
    stamp :lgst       = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_s_fx,       r ); }; };
    stamp :lgst_hard  = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_hard_s_fx,  r ); }; };
    stamp :lgst_leaky = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_leaky_s_fx, r ); }; };

    // tanh
    stamp :tanh       = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_s_fx,       r ); }; };
    stamp :tanh_hard  = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_hard_s_fx,  r ); }; };
    stamp :tanh_leaky = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_leaky_s_fx, r ); }; };

    // softplus
    stamp :softplus   = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_softplus_s_fx,   r ); }; };
    stamp :relu       = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_relu_s_fx,       r ); }; };
    stamp :relu_leaky = { func : :f = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_relu_leaky_s_fx, r ); }; };

    /// dendride-pass
    group :dp =
    {
        extending stump verbatim :_ = aware : {};

        func ::: :sig = { return "vu"; };
        stamp :ca_cpy = { func :: :f = { bmath_hf3_s_add( r, a, r ); }; };
        stamp :ca_neg = { func :: :f = { bmath_hf3_s_sub( r, a, r ); }; };

        stamp :ca_add = { func :: :f = { bmath_hf3_s_add( r, a, r ); }; };
        stamp :cb_add = { func :: :f = { bmath_hf3_s_add( r, a, r ); }; };
        stamp :ca_sub = { func :: :f = { bmath_hf3_s_add( r, a, r ); }; };
        stamp :cb_sub = { func :: :f = { bmath_hf3_s_sub( r, a, r ); }; };
    };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// binary operators
group :ar2 =
{
    extending stump verbatim :_ = aware : {};

    signature    void f(  plain, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );
    feature 'ap' void of( const, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );

    func :: :get_arity = { return 2; };
    func  : :  f = { ERR_fa( "Function 'f' is not available. Need to call 'of' instead?" ); };
    func  : : of = {  @f( a, b, r ); };
    func :: :aof = { @of( o, a[0], a[1], r ); };

    stamp :add  = { func : :f = { bmath_hf3_s_add ( a, b, r ); }; };
    stamp :sub  = { func : :f = { bmath_hf3_s_sub ( a, b, r ); }; };
    stamp :hmul = { func : :f = { bmath_hf3_s_hmul( a, b, r ); }; };
    stamp :hdiv = { func : :f = { bmath_hf3_s_fp_f3_ar2( a, b, bmath_f3_op_ar2_div_s_fx, r ); }; };

    /// bmul quartet
    stamp :bmul         = { func : :f = { bmath_hf3_s_bmul        ( a, b,         r ); }; }; // a ** b -> c
    stamp :bmul_htp     = { func : :f = { bmath_hf3_s_bmul_htp    ( a, b,         r ); }; }; // a *^ b -> c
    stamp :htp_bmul     = { func : :f = { bmath_hf3_s_htp_bmul    ( a, b,         r ); }; }; // a ^* b -> c
    stamp :htp_bmul_htp = { func : :f = { bmath_hf3_s_htp_bmul_htp( a, b,         r ); }; }; // a ^*^ b -> c

    /// h * scalar
    stamp :mul_scl      = { func : :f = { bmath_hf3_s_mul_scl     ( a, b->v_data, r ); }; }; // a * s(b) -> c
    stamp :scl_mul      = { func : :f = { bmath_hf3_s_mul_scl     ( b, a->v_data, r ); }; }; // s(a) * b -> c

    /// dendride-pass
    group :dp =
    {
        extending stump verbatim :_ = aware : {};

        /// dp-yvu functions ...
        func ::: :sig = { return "yvu"; };

        stamp :ca_exp        = { func :: :f = { bmath_hf3_s_hmul_add( a, b, r, r ); }; };

        // logistic
        stamp :ca_lgst       = { func :: :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_lgst_s_gy,       b, r ); }; };
        stamp :ca_lgst_hard  = { func :: :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_lgst_hard_s_gy,  b, r ); }; };
        stamp :ca_lgst_leaky = { func :: :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_lgst_leaky_s_gy, b, r ); }; };

        // tanh
        stamp :ca_tanh       = { func :: :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_tanh_s_gy,       b, r ); }; };
        stamp :ca_tanh_hard  = { func :: :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_tanh_hard_s_gy,  b, r ); }; };
        stamp :ca_tanh_leaky = { func :: :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_tanh_leaky_s_gy, b, r ); }; };

        // softplus
        stamp :ca_softplus   = { func :: :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_softplus_s_gy,   b, r ); }; };
        stamp :ca_relu       = { func :: :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_relu_s_gy,       b, r ); }; };
        stamp :ca_relu_leaky = { func :: :f = { bmath_hf3_s_fp_f3_ar1_madd( a, bmath_f3_op_ar1_relu_leaky_s_gy, b, r ); }; };

        /// explicit dp-signature ...
        // hmul
        stamp :ca_hmul = { func ::: :sig = { return "vbu"; }; func :: :f = { bmath_hf3_s_hmul_add( a, b, r, r ); }; };
        stamp :cb_hmul = { func ::: :sig = { return "avu"; }; func :: :f = { bmath_hf3_s_hmul_add( a, b, r, r ); }; };

        /// bmul quartet
        stamp :ca_bmul         = { func ::: :sig = { return "vbu"; }; func :: :f = { bmath_hf3_s_bmul_htp_add    ( a, b, r, r ); }; };
        stamp :cb_bmul         = { func ::: :sig = { return "avu"; }; func :: :f = { bmath_hf3_s_htp_bmul_add    ( a, b, r, r ); }; };
        stamp :ca_bmul_htp     = { func ::: :sig = { return "vbu"; }; func :: :f = { bmath_hf3_s_bmul_add        ( a, b, r, r ); }; };
        stamp :cb_bmul_htp     = { func ::: :sig = { return "vau"; }; func :: :f = { bmath_hf3_s_htp_bmul_add    ( a, b, r, r ); }; };
        stamp :ca_htp_bmul     = { func ::: :sig = { return "bvu"; }; func :: :f = { bmath_hf3_s_bmul_htp_add    ( a, b, r, r ); }; };
        stamp :cb_htp_bmul     = { func ::: :sig = { return "avu"; }; func :: :f = { bmath_hf3_s_bmul_add        ( a, b, r, r ); }; };
        stamp :ca_htp_bmul_htp = { func ::: :sig = { return "bvu"; }; func :: :f = { bmath_hf3_s_htp_bmul_htp_add( a, b, r, r ); }; };
        stamp :cb_htp_bmul_htp = { func ::: :sig = { return "vau"; }; func :: :f = { bmath_hf3_s_htp_bmul_htp_add( a, b, r, r ); }; };

        /// h * scalar
        stamp :ca_mul_scl      = { func ::: :sig = { return "vbu"; }; func :: :f = { bmath_hf3_s_mul_scl_f3_add( a, b->v_data[ 0 ], r, r ); }; };
        stamp :cb_mul_scl      = { func ::: :sig = { return "avu"; }; func :: :f = { r->v_data[ 0 ] += bmath_hf3_s_f3_vec_mul_vec( a, b ); }; };

        /// scalar * h
        stamp :ca_scl_mul      = { func ::: :sig = { return "vbu"; }; func :: :f = { r->v_data[ 0 ] += bmath_hf3_s_f3_vec_mul_vec( a, b ); }; };
        stamp :cb_scl_mul      = { func ::: :sig = { return "avu"; }; func :: :f = { bmath_hf3_s_mul_scl_f3_add( b, a->v_data[ 0 ], r, r ); }; };
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// ternary operators
group :ar3 =
{
    extending stump verbatim :_ = aware : {};

    signature    void f(  plain, const bmath_hf3_s* a, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r );
    feature 'ap' void of( const, const bmath_hf3_s* a, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r );

    func :: :get_arity = { return 3; };
    func  : :  f = { ERR_fa( "Function 'f' is not available. Need to call 'of' instead?" ); };
    func  : : of = {  @f( a, b, c, r ); };
    func :: :aof = { @of( o, a[0], a[1], a[2], r ); };

    /// dendride-pass
    group :dp =
    {
        extending stump verbatim :_ = aware : {};

        // hdiv
        func ::: :sig = { return "abvu"; };
        stamp :ca_hdiv = { func :: :f = { bmath_hf3_s_fp_f3_ar2_madd( a, b, bmath_f3_op_ar2_div_s_gxa, c, r ); }; };
        stamp :cb_hdiv = { func :: :f = { bmath_hf3_s_fp_f3_ar2_madd( a, b, bmath_f3_op_ar2_div_s_gxb, c, r ); }; };
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t bmath_hf3_op_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_HF3_OP_H



