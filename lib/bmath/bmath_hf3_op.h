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
 *  Generic naming scheme applied here is consistent with bmath_f3_op.
 */
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'ap' sz_t get_arity( const );

/// a represents an array of size arity
feature 'ap' void aofx( const,          const bmath_hf3_s** a, bmath_hf3_s* r );
feature 'ap' void aogx( const, sz_t ch, const bmath_hf3_s** a, bmath_hf3_s* r );
feature 'ap' void aogy( const, sz_t ch, const bmath_hf3_s** a, bmath_hf3_s* r );

/// nullary operators
group :ar0 =
{
    signature    void  f( plain, bmath_hf3_s* r );
    feature 'ap' void of( const, bmath_hf3_s* r );

    func :: :get_arity = { return 0; };
    func :  :f    = { ERR_fa( "Not available." ); };
    func :  :of   = { @f( r ); };
    func :: :aofx = { @of( o, r ); };

    stamp :zero    = aware : {                   func : : f = { bmath_hf3_s_zro(       r ); }; };
    stamp :literal = aware : { bmath_hf3_s -> h; func : :of = { bmath_hf3_s_cpy( o->h, r ); }; };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// unary operators
group :ar1 =
{
    signature void fx( plain, const bmath_hf3_s* a, bmath_hf3_s* r );
    signature void gy( plain, const bmath_hf3_s* y, bmath_hf3_s* r );

    feature 'ap' void ofx( const, const bmath_hf3_s* a, bmath_hf3_s* r );
    feature 'ap' void ogy( const, const bmath_hf3_s* y, bmath_hf3_s* r );

    func :: :get_arity = { return 1; };

    func  : :  fx = { ERR_fa( "Not available." ); };
    func  : :  gy = { ERR_fa( "Not available." ); };
    func  : : ofx = {  @fx(    a   , r ); };
    func  : : ogy = {  @gy(    y   , r ); };
    func :: :aofx = { @ofx( o, a[0], r ); };
    func :: :aogy = { @ogy( o, a[0], r ); };

    stamp :identity = aware :
    {
        func : :fx = { bmath_hf3_s_cpy( a, r ); };
        func : :gy = { bmath_hf3_s_set_f3( r, 1.0 ); };
    };

    // ======= (logistic function) ============

    stamp :lgst = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_s_fx, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_ar1( y, bmath_f3_op_ar1_lgst_s_gy, r ); };
    };

    stamp :lgst_hard = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_hard_s_fx, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_ar1( y, bmath_f3_op_ar1_lgst_hard_s_gy, r ); };
    };

    stamp :lgst_leaky = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_lgst_leaky_s_fx, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_ar1( y, bmath_f3_op_ar1_lgst_leaky_s_gy, r ); };
    };

    // ======= (tanh) =========================

    stamp :tanh = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_s_fx, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_ar1( y, bmath_f3_op_ar1_tanh_s_gy, r ); };
    };

    stamp :tanh_hard = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_hard_s_fx, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_ar1( y, bmath_f3_op_ar1_tanh_hard_s_gy, r ); };
    };

    stamp :tanh_leaky = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_tanh_leaky_s_fx, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_ar1( y, bmath_f3_op_ar1_tanh_leaky_s_gy, r ); };
    };

    // ======= (softplus function) ============

    stamp :softplus = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_softplus_s_fx, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_ar1( y, bmath_f3_op_ar1_softplus_s_gy, r ); };
    };

    stamp :relu = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_relu_s_fx, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_ar1( y, bmath_f3_op_ar1_relu_s_gy, r ); };
    };

    stamp :relu_leaky = aware :
    {
        func : :fx = { bmath_hf3_s_fp_f3_ar1( a, bmath_f3_op_ar1_relu_leaky_s_fx, r ); };
        func : :gy = { bmath_hf3_s_fp_f3_ar1( y, bmath_f3_op_ar1_relu_leaky_s_gy, r ); };
    };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// binary operators
group :ar2 =
{
    signature    void fx(   plain, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );
    signature    void gxa(  plain, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );
    signature    void gxb(  plain, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );

    feature 'ap' void ofx(  const, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );
    feature 'ap' void ogxa( const, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );
    feature 'ap' void ogxb( const, const bmath_hf3_s* a, const bmath_hf3_s* b, bmath_hf3_s* r );

    func :: :get_arity = { return 2; };
    func  : :  fx  = { ERR_fa( "Not available." ); };
    func  : :  gxa = { ERR_fa( "Not available." ); };
    func  : :  gxb = { ERR_fa( "Not available." ); };

    func  : : ofx  = {  @fx(  a, b, r ); };
    func  : : ogxa = {  @gxa( a, b, r ); };
    func  : : ogxb = {  @gxb( a, b, r ); };

    func :: :aofx = { @ofx( o, a[0], a[1], r ); };
    func :: :aogx = { if( ch == 0 ) { @ogxa( o, a[0], a[1], r ); } else { @ogxb( o, a[0], a[1], r ); } };

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
        func : :fx  = { bmath_hf3_s_fp_f3_ar2( a, b, bmath_f3_op_ar2_div_s_fx,  r ); };
        func : :gxa = { bmath_hf3_s_fp_f3_ar2( a, b, bmath_f3_op_ar2_div_s_gxa, r ); };
        func : :gxb = { bmath_hf3_s_fp_f3_ar2( a, b, bmath_f3_op_ar2_div_s_gxb, r ); };
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t bmath_hf3_op_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_HF3_OP_H



