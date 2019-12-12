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

#ifndef BHVM_HF3_VM_H
#define BHVM_HF3_VM_H

/**********************************************************************************************************************/

/** Virtual Machine representing the implementation of an operator-graph on Holor hf3.
 */

/**********************************************************************************************************************/

#include "bhvm_planted_hf3.h"
#include "bhvm_hf3.h"
#include "bhvm_hf3_op.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm_hf3_vm

PLANT_GROUP( bhvm_hf3_vm, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// holor

stamp :hparam = aware :
{
    tp_t name;
    tp_t type;
    sz_t idx_paired = -1; // index to holor paired with this one (e.g. axon <=> axon-gradient)
};

stamp :holor = aware :
{
    :hparam_s p;
    bhvm_hf3_s h;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :arr_holor = aware bcore_array { :holor_s []; };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'ap' void run( const, :holor_s* ah );

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Microcode Operator Set

group :op =
{
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    feature 'a' sz_t get_arity( const );
    feature 'a' void set_indices( mutable, sz_t* a ); // a represents an array of arity + 1 elements
    feature 'a' void get_indices( const,   sz_t* a ); // a represents an array of arity + 1 elements

    feature 'a' sc_t sig( const ); // argument signature of operator (e.g. "aby")

    /** Sets arguments from index data according to signature
      * Identifiers:
      *   a ... j : a-pass input channels
      *   y       : a-pass output channel
      *   v       : d-pass gradient (at output channel)
      *   u       : d-pass gradient (result at selected input channel)
      */
    feature 'a' void set_arg( mutable, char id, sz_t idx );

    /// Extendable signature to create object, assign arguments and additional parameters
    signature :* csetup( mutable );

    /// nullary operators
    group :ar0 =
    {
        signature :: :csetup csetup( sz_t idx_a );

        extending stump :_ = aware :
        {
            sz_t a = -1;
            func :: :get_arity = { return 0; };
            func :: :set_indices = { o->a = a[0]; };
            func :: :get_indices = { a[0] = o->a; };
            func  : :csetup = { if( !o ) o = @_create(); o->a = idx_a; return (::*)o; };
        };

        func :: :set_arg =
        {
            if( id == @_sig( o )[0] ) o->a = idx;
        };

        func :: :sig = { return "y"; }; // default signature
        stamp :nul       = { func ::: :run = {                                         }; }; // no operation
        stamp :zro       = { func ::: :run = { bhvm_hf3_s_zro(        &ah[ o->a ].h ); }; }; // sets operand zero
        stamp :zro_nc    = { func ::: :run = { if( ah[ o->a ].h.v_space > 0 ) bhvm_hf3_s_zro( &ah[ o->a ].h ); }; }; // sets operand zero
        stamp :determine = { func ::: :run = { bhvm_hf3_s_fit_v_size( &ah[ o->a ].h ); }; }; // makes operand determined
        stamp :vacate    = { func ::: :run = { bhvm_hf3_s_set_vacant( &ah[ o->a ].h ); }; }; // makes operand vacant
        stamp :clear     = { func ::: :run = { bhvm_hf3_s_clear(      &ah[ o->a ].h ); }; }; // clears operand

        // randomizes a determined, rseed and index 'a' are used as seed
        signature : : csetup csetup_randomize( u2_t rseed, f3_t density, f3_t min, f3_t max );

        stamp :randomize =
        {
            u2_t rseed = 1234;
            f3_t min = -0.5;
            f3_t max =  0.5;
            f3_t density = 1.0;
            func : :csetup_randomize =
            {
                if( !o ) o = :randomize_s_create();
                o->a = idx_a;
                o->rseed   = rseed;
                o->density = density;
                o->min     = min;
                o->max     = max;
                return (::*)o;
            };

            func ::: :run = { u2_t rval = o->rseed + o->a; bhvm_hf3_s_set_random( &ah[ o->a ].h, o->density, o->min, o->max, &rval ); };
        };

        // dendrite-pass
        group :dp =
        {
            func ::: :sig = { return "u"; };
            stamp :ca_floor = { func :::: :run = { bhvm_hf3_op_ar0_dp_ca_floor_s_f( &ah[o->a].h ); }; };
            stamp :ca_ceil  = { func :::: :run = { bhvm_hf3_op_ar0_dp_ca_ceil_s_f ( &ah[o->a].h ); }; };
        };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// unary operators
    group :ar1 =
    {
        signature :: :csetup csetup( sz_t idx_a, sz_t idx_b );

        extending stump :_ = aware :
        {
            sz_t a = -1;
            sz_t b = -1;
            func :: :get_arity   = { return 1; };
            func :: :set_indices = { o->a = a[0]; o->b = a[1]; };
            func :: :get_indices = { a[0] = o->a; a[1] = o->b; };
            func  : :csetup      = { if( !o ) o = @_create(); o->a = idx_a; o->b = idx_b; return (::*)o; };
        };

        func :: :set_arg =
        {
            if( id == @_sig( o )[ 0 ] ) o->a = idx;
            if( id == @_sig( o )[ 1 ] ) o->b = idx;
        };

        func :: :sig = { return "ay"; }; // default signature
        stamp :unary = { bmath_fp_f3_ar1 unary; func ::: :run = { bhvm_hf3_s_fp_f3_ar1( &ah[o->a].h, o->unary, &ah[o->b].h ); }; };

        stamp :cpy        = { func ::: :run = { bhvm_hf3_op_ar1_cpy_s_f  ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :neg        = { func ::: :run = { bhvm_hf3_op_ar1_neg_s_f  ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :floor      = { func ::: :run = { bhvm_hf3_op_ar1_floor_s_f( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :ceil       = { func ::: :run = { bhvm_hf3_op_ar1_ceil_s_f ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :exp        = { func ::: :run = { bhvm_hf3_op_ar1_exp_s_f  ( &ah[o->a].h, &ah[o->b].h ); }; };

        /// specific copy operators
        stamp :cpy_ay     = { func :: :sig = { return "ay"; }; func ::: :run = { bhvm_hf3_op_ar1_cpy_s_f( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :cpy_by     = { func :: :sig = { return "by"; }; func ::: :run = { bhvm_hf3_op_ar1_cpy_s_f( &ah[o->a].h, &ah[o->b].h ); }; };

        // logistic
        stamp :lgst       = { func ::: :run = { bhvm_hf3_op_ar1_lgst_s_f      ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :lgst_hard  = { func ::: :run = { bhvm_hf3_op_ar1_lgst_hard_s_f ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :lgst_leaky = { func ::: :run = { bhvm_hf3_op_ar1_lgst_leaky_s_f( &ah[o->a].h, &ah[o->b].h ); }; };

        // tanh
        stamp :tanh       = { func ::: :run = { bhvm_hf3_op_ar1_tanh_s_f      ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :tanh_hard  = { func ::: :run = { bhvm_hf3_op_ar1_tanh_hard_s_f ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :tanh_leaky = { func ::: :run = { bhvm_hf3_op_ar1_tanh_leaky_s_f( &ah[o->a].h, &ah[o->b].h ); }; };

        // softplus
        stamp :softplus   = { func ::: :run = { bhvm_hf3_op_ar1_softplus_s_f  ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :relu       = { func ::: :run = { bhvm_hf3_op_ar1_relu_s_f      ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :relu_leaky = { func ::: :run = { bhvm_hf3_op_ar1_relu_leaky_s_f( &ah[o->a].h, &ah[o->b].h ); }; };

        // Cast operators make the target weakly reference source data.
        // Value data are always referenced; shape might be referenced.
        stamp :cast_htp   = { func :::  :run = { bhvm_hf3_op_ar1_cast_htp_s_f( &ah[o->a].h, &ah[o->b].h ); }; };

        // dendrite-pass
        group :dp =
        {
            func ::: :sig = { return "vu"; };
            stamp :ca_cpy = { func :::: :run = { bhvm_hf3_op_ar1_dp_ca_cpy_s_f( &ah[o->a].h, &ah[o->b].h ); }; };
            stamp :ca_neg = { func :::: :run = { bhvm_hf3_op_ar1_dp_ca_neg_s_f( &ah[o->a].h, &ah[o->b].h ); }; };
            stamp :ca_add = { func :::: :run = { bhvm_hf3_op_ar1_dp_ca_add_s_f( &ah[o->a].h, &ah[o->b].h ); }; };
            stamp :cb_add = { func :::: :run = { bhvm_hf3_op_ar1_dp_cb_add_s_f( &ah[o->a].h, &ah[o->b].h ); }; };
            stamp :ca_sub = { func :::: :run = { bhvm_hf3_op_ar1_dp_ca_sub_s_f( &ah[o->a].h, &ah[o->b].h ); }; };
            stamp :cb_sub = { func :::: :run = { bhvm_hf3_op_ar1_dp_cb_sub_s_f( &ah[o->a].h, &ah[o->b].h ); }; };

            /** Note: The dendrite-pass cast operator makes the the axon gradient reference the dendrite gradient (never vice versa).
             *  This is important because the dendrite node may have multiple other independent down-links.
             */
            stamp :ca_cast_htp =
            {
                func  ::: :sig = { return bhvm_hf3_op_ar1_dp_ca_cast_htp_s_sig(); }; // signature would be "uv"
                func :::: :run = {        bhvm_hf3_op_ar1_dp_ca_cast_htp_s_f( &ah[o->a].h, &ah[o->b].h ); };
            };

        };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// binary operators
    group :ar2 =
    {
        signature   :: :csetup csetup( sz_t idx_a, sz_t idx_b, sz_t idx_c );

        extending stump :_ = aware :
        {
            sz_t a = -1;
            sz_t b = -1;
            sz_t c = -1;
            func :: :get_arity = { return 2; };
            func :: :set_indices = { o->a = a[0]; o->b = a[1]; o->c = a[2]; };
            func :: :get_indices = { a[0] = o->a; a[1] = o->b; a[2] = o->c; };
            func  : :csetup   = { if( !o ) o = @_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (::*)o; };
        };

        func :: :set_arg =
        {
            if( id == @_sig( o )[ 0 ] ) o->a = idx;
            if( id == @_sig( o )[ 1 ] ) o->b = idx;
            if( id == @_sig( o )[ 2 ] ) o->c = idx;
        };

        func :: :sig = { return "aby"; }; // default signature
        stamp :add     = { func ::: :run = { bhvm_hf3_op_ar2_add_s_f(     &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; }; // a + b -> c
        stamp :sub     = { func ::: :run = { bhvm_hf3_op_ar2_sub_s_f(     &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; }; // a - b -> c
        stamp :hmul    = { func ::: :run = { bhvm_hf3_op_ar2_hmul_s_f(    &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; }; // a * b -> c
        stamp :hdiv    = { func ::: :run = { bhvm_hf3_op_ar2_hdiv_s_f(    &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; }; // a / b -> c
        stamp :bmul    = { func ::: :run = { bhvm_hf3_op_ar2_bmul_s_f(    &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; }; // a ** b -> c
        stamp :mul_scl = { func ::: :run = { bhvm_hf3_op_ar2_mul_scl_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; }; // a * s(b) -> c
        stamp :scl_mul = { func ::: :run = { bhvm_hf3_op_ar2_scl_mul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; }; // s(a) * b -> c

        // dendrite-pass
        group :dp =
        {
            func ::: :sig = { return "yvu"; };

            // exp
            stamp :ca_exp        = { func :::: :run = { bhvm_hf3_op_ar2_dp_ca_exp_s_f(        &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; };

            // logistic
            stamp :ca_lgst       = { func :::: :run = { bhvm_hf3_op_ar2_dp_ca_lgst_s_f(       &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; };
            stamp :ca_lgst_hard  = { func :::: :run = { bhvm_hf3_op_ar2_dp_ca_lgst_hard_s_f(  &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; };
            stamp :ca_lgst_leaky = { func :::: :run = { bhvm_hf3_op_ar2_dp_ca_lgst_leaky_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; };

            // tanh
            stamp :ca_tanh       = { func :::: :run = { bhvm_hf3_op_ar2_dp_ca_tanh_s_f(       &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; };
            stamp :ca_tanh_hard  = { func :::: :run = { bhvm_hf3_op_ar2_dp_ca_tanh_hard_s_f(  &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; };
            stamp :ca_tanh_leaky = { func :::: :run = { bhvm_hf3_op_ar2_dp_ca_tanh_leaky_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; };

            // softplus
            stamp :ca_softplus   = { func :::: :run = { bhvm_hf3_op_ar2_dp_ca_softplus_s_f(   &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; };
            stamp :ca_relu       = { func :::: :run = { bhvm_hf3_op_ar2_dp_ca_relu_s_f(       &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; };
            stamp :ca_relu_leaky = { func :::: :run = { bhvm_hf3_op_ar2_dp_ca_relu_leaky_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); }; };

            /// explicit dp-signature ...

            stamp :ca_hmul =
            {
                func  ::: :sig = { return bhvm_hf3_op_ar2_dp_ca_hmul_s_sig(); };
                func :::: :run = {        bhvm_hf3_op_ar2_dp_ca_hmul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); };
            };

            stamp :cb_hmul =
            {
                func  ::: :sig = { return bhvm_hf3_op_ar2_dp_cb_hmul_s_sig(); };
                func :::: :run = {        bhvm_hf3_op_ar2_dp_cb_hmul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); };
            };

            stamp :ca_bmul =
            {
                func  ::: :sig = { return bhvm_hf3_op_ar2_dp_ca_bmul_s_sig(); };
                func :::: :run = {        bhvm_hf3_op_ar2_dp_ca_bmul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); };
            };

            stamp :cb_bmul =
            {
                func  ::: :sig = { return bhvm_hf3_op_ar2_dp_cb_bmul_s_sig(); };
                func :::: :run = {        bhvm_hf3_op_ar2_dp_cb_bmul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); };
            };

            stamp :ca_mul_scl =
            {
                func  ::: :sig = { return bhvm_hf3_op_ar2_dp_ca_mul_scl_s_sig(); };
                func :::: :run = {        bhvm_hf3_op_ar2_dp_ca_mul_scl_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); };
            };

            stamp :cb_mul_scl =
            {
                func  ::: :sig = { return bhvm_hf3_op_ar2_dp_cb_mul_scl_s_sig(); };
                func :::: :run = {        bhvm_hf3_op_ar2_dp_cb_mul_scl_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); };
            };

            stamp :ca_scl_mul =
            {
                func  ::: :sig = { return bhvm_hf3_op_ar2_dp_ca_scl_mul_s_sig(); };
                func :::: :run = {        bhvm_hf3_op_ar2_dp_ca_scl_mul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); };
            };

            stamp :cb_scl_mul =
            {
                func  ::: :sig = { return bhvm_hf3_op_ar2_dp_cb_scl_mul_s_sig(); };
                func :::: :run = {        bhvm_hf3_op_ar2_dp_cb_scl_mul_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h ); };
            };

        };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// ternary operators
    group :ar3 =
    {
        signature   :: :csetup csetup( sz_t idx_a, sz_t idx_b, sz_t idx_c, sz_t idx_d );

        extending stump :_ = aware :
        {
            sz_t a = -1;
            sz_t b = -1;
            sz_t c = -1;
            sz_t d = -1;
            func :: :get_arity = { return 3; };
            func :: :set_indices = { o->a = a[0]; o->b = a[1]; o->c = a[2]; o->d = a[3]; };
            func :: :get_indices = { a[0] = o->a; a[1] = o->b; a[2] = o->c; a[3] = o->d; };
            func  : :csetup   = { if( !o ) o = @_create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; o->d = idx_d; return (::*)o; };
        };

        func :: :set_arg =
        {
            if( id == @_sig( o )[ 0 ] ) o->a = idx;
            if( id == @_sig( o )[ 1 ] ) o->b = idx;
            if( id == @_sig( o )[ 2 ] ) o->c = idx;
            if( id == @_sig( o )[ 3 ] ) o->d = idx;
        };

        func :: :sig = { return "abcy"; };  // default signature

        /// dendrite-pass
        group :dp =
        {
            stamp :ca_hdiv =
            {
                func  ::: :sig = { return bhvm_hf3_op_ar3_dp_ca_hdiv_s_sig(); };
                func :::: :run = {        bhvm_hf3_op_ar3_dp_ca_hdiv_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h, &ah[o->d].h ); };
            };

            stamp :cb_hdiv =
            {
                func  ::: :sig = { return bhvm_hf3_op_ar3_dp_cb_hdiv_s_sig(); };
                func :::: :run = {        bhvm_hf3_op_ar3_dp_cb_hdiv_s_f( &ah[o->a].h, &ah[o->b].h, &ah[o->c].h, &ah[o->d].h ); };
            };
        };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Microcode

feature 'ap' void mrun( const, :holor_s* ah ); // microcode operation

signature void clear( mutable );

// microcode operator with preset perspective
stamp :mop = aware :
{
    aware :op => op;
    private :s* p; // bhvm_hf3_vm perspective of op

    func bcore_inst_call : copy_x  = { o->p = o->op ? (:s*):s_get_aware( o->op ) : NULL; };
    func bcore_via_call  : mutated = { @_copy_x( o ); };
    func bhvm_hf3_vm     : mrun = { assert( o->p ); assert( o->p->run ); o->p->run( (vc_t)o->op, ah ); };
};

signature void op_push_d( mutable,       :op* op );
signature void op_push_c( mutable, const :op* op );

// microcode
stamp :mcode = aware bcore_array
{
    tp_t name;
    :mop_s [];
    func bhvm_hf3_vm : mrun = { BFOR_EACH( i, o ) :mop_s_mrun( &o->data[ i ], ah ); };

    func : :op_push_d =
    {
        assert( op );
        :mop_s* mop = @_push( o );
        mop->op = op;
        mop->p = (:s*):s_get_aware( mop->op );
        assert( mop->p );
    };

    func : :op_push_c =
    {
        @_op_push_d( o, :op_a_clone( op ) );
    };
};

// microcode array
stamp :arr_mcode = aware bcore_array { :mcode_s => []; };

// mcode library functions
signature      bl_t mcode_exists      (   const, tp_t name );
signature :mcode_s* mcode_get         ( mutable, tp_t name ); // retrieves mcode if existing or returns NULL
signature :mcode_s* mcode_get_or_new  ( mutable, tp_t name ); // creates mcode if not yet existing otherwise same as get
signature :mcode_s* mcode_reset       ( mutable, tp_t name ); // creates mcode if not yet existing or sets its size to zero
signature      void mcode_op_push_d   ( mutable, tp_t name,       :op* op );
signature      void mcode_op_push_c   ( mutable, tp_t name, const :op* op );
signature      void mcode_push        ( mutable, tp_t name, tp_t src_name ); // appends mcode of src_name
signature      void mcode_push_reverse( mutable, tp_t name, tp_t src_name ); // appends mcode of src_name in reverse order
signature      void mcode_remove      ( mutable, tp_t name );                // removes mcode if existing
signature      void mcode_mrun        ( mutable, tp_t name, :holor_s* ah );

// microcode library
stamp :lib_mcode = aware :
{
    :arr_mcode_s arr;
    bcore_hmap_tpuz_s map; // name-index map

    func : :clear = { :arr_mcode_s_clear( &o->arr ); bcore_hmap_tpuz_s_clear( &o->map ); };

    func : :mcode_exists = { return bcore_hmap_tpuz_s_exists( &o->map, name ); };

    func : :mcode_get =
    {
        uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
        if( !pidx ) return NULL;
        return o->arr.data[ *pidx ];
    };

    func : :mcode_get_or_new =
    {
        if( bcore_hmap_tpuz_s_exists( &o->map, name ) ) return @_mcode_get( o, name );
        bcore_hmap_tpuz_s_set( &o->map, name, o->arr.size );
        :mcode_s* mcode = :arr_mcode_s_push( &o->arr );
        mcode->name = name;
        return mcode;
    };

    func : :mcode_reset =
    {
        :mcode_s* mcode = @_mcode_get_or_new( o, name );
        :mcode_s_clear( mcode );
        return mcode;
    };

    func : :mcode_op_push_c = { :mcode_s_op_push_c( @_mcode_get_or_new( o, name ), op ); };
    func : :mcode_op_push_d = { :mcode_s_op_push_d( @_mcode_get_or_new( o, name ), op ); };

    func : :mcode_push =
    {
        :mcode_s* src = @_mcode_get( o, src_name );
        if( !src ) return;
        :mcode_s* dst = @_mcode_get_or_new( o, name );
        BFOR_EACH( i, src ) :mcode_s_op_push_c( dst, src->data[ i ].op );
    };

    func : :mcode_push_reverse =
    {
        :mcode_s* src = @_mcode_get( o, src_name );
        if( !src ) return;
        :mcode_s* dst = @_mcode_get_or_new( o, name );
        for( sz_t i = src->size - 1; i >= 0; i-- ) :mcode_s_op_push_c( dst, src->data[ i ].op );
    };

    func : :mcode_remove =
    {
        uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
        if( !pidx ) return;
        sz_t idx = *pidx;
        :mcode_s_discard( o->arr.data[ idx ] );
        o->arr.size--;
        o->arr.data[ idx ] = o->arr.data[ o->arr.size ];
        o->arr.data[ o->arr.size ] = NULL;
        if( idx < o->arr.size ) bcore_hmap_tpuz_s_set( &o->map, o->arr.data[ idx ]->name, idx );
    };

    func : :mcode_mrun = { :mcode_s* mc = @_mcode_get( o, name ); if( mc ) :mcode_s_mrun( mc, ah ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Privileged (frame) instruction code

forward :thread_s;
forward :pi;

feature 'ap' void trun( const, :thread_s* t );

// privileged instruction with preset perspective
stamp :fpi = aware :
{
    aware :pi => pi;
    private :s* p; // bhvm_hf3_vm perspective of pi

    func bcore_inst_call : copy_x  = { o->p = o->pi ? (:s*):s_get_aware( o->pi ) : NULL; };
    func bcore_via_call  : mutated = { @_copy_x( o ); };
    func bhvm_hf3_vm     : trun = { assert( o->p ); assert( o->p->trun ); o->p->trun( (vc_t)o->pi, t ); };
};

signature void pi_push_d( mutable,       :pi* pi );
signature void pi_push_c( mutable, const :pi* pi );

// frame-code
stamp :fcode = aware bcore_array
{
    tp_t name;
    :fpi_s [];

    func : :pi_push_d =
    {
        assert( pi );
        :fpi_s* fpi = @_push( o );
        fpi->pi = pi;
        fpi->p = (:s*):s_get_aware( fpi->pi );
        assert( fpi->p );
    };

    func : :pi_push_c =
    {
        @_pi_push_d( o, :pi_a_clone( pi ) );
    };

    func : :trun =
    {
        while( t->ip >= 0 && t->ip < t->fcode->size && t->sp <= t->stack.size )
        {
            :fpi_s_trun( &o->data[ t->ip ], t );
        }
    };
};

// frame-code array
stamp :arr_fcode = aware bcore_array { :fcode_s => []; };

// frame-thread
stamp :thread = aware :
{
    private :frame_s* frame;
    private :fcode_s* fcode;

    sz_t ip; // instruction pointer
    sz_t sp; // stack pointer
    bcore_arr_sz_s stack;
};

// fcode library functions
signature      bl_t fcode_exists      (   const, tp_t name );
signature :fcode_s* fcode_get         ( mutable, tp_t name ); // retrieves fcode if existing or returns NULL
signature :fcode_s* fcode_get_or_new  ( mutable, tp_t name ); // creates fcode if not yet existing otherwise same as get
signature :fcode_s* fcode_reset       ( mutable, tp_t name ); // creates fcode if not yet existing or sets its size to zero
signature      void fcode_pi_push_d   ( mutable, tp_t name,       :pi* pi );
signature      void fcode_pi_push_c   ( mutable, tp_t name, const :pi* pi );
signature      void fcode_push        ( mutable, tp_t name, tp_t src_name ); // appends fcode of src_name
signature      void fcode_push_reverse( mutable, tp_t name, tp_t src_name ); // appends fcode of src_name in reverse order
signature      void fcode_remove      ( mutable, tp_t name );                // removes fcode if existing
signature      void fcode_trun        ( mutable, tp_t name, :thread_s* t );

// microcode library
stamp :lib_fcode = aware :
{
    :arr_fcode_s arr;
    bcore_hmap_tpuz_s map; // name-index map

    func : :clear = { :arr_fcode_s_clear( &o->arr ); bcore_hmap_tpuz_s_clear( &o->map ); };

    func : :fcode_exists = { return bcore_hmap_tpuz_s_exists( &o->map, name ); };

    func : :fcode_get =
    {
        uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
        if( !pidx ) return NULL;
        return o->arr.data[ *pidx ];
    };

    func : :fcode_get_or_new =
    {
        if( bcore_hmap_tpuz_s_exists( &o->map, name ) ) return @_fcode_get( o, name );
        bcore_hmap_tpuz_s_set( &o->map, name, o->arr.size );
        :fcode_s* fcode = :arr_fcode_s_push( &o->arr );
        fcode->name = name;
        return fcode;
    };

    func : :fcode_reset =
    {
        :fcode_s* fcode = @_fcode_get_or_new( o, name );
        :fcode_s_clear( fcode );
        return fcode;
    };

    func : :fcode_pi_push_c = { :fcode_s_pi_push_c( @_fcode_get_or_new( o, name ), pi ); };
    func : :fcode_pi_push_d = { :fcode_s_pi_push_d( @_fcode_get_or_new( o, name ), pi ); };

    func : :fcode_push =
    {
        :fcode_s* src = @_fcode_get( o, src_name );
        if( !src ) return;
        :fcode_s* dst = @_fcode_get_or_new( o, name );
        BFOR_EACH( i, src ) :fcode_s_pi_push_c( dst, src->data[ i ].pi );
    };

    func : :fcode_remove =
    {
        uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
        if( !pidx ) return;
        sz_t idx = *pidx;
        :fcode_s_discard( o->arr.data[ idx ] );
        o->arr.size--;
        o->arr.data[ idx ] = o->arr.data[ o->arr.size ];
        o->arr.data[ o->arr.size ] = NULL;
        if( idx < o->arr.size ) bcore_hmap_tpuz_s_set( &o->map, o->arr.data[ idx ]->name, idx );
    };

    func : :fcode_trun = { :fcode_s* fc = @_fcode_get( o, name ); if( fc ) :fcode_s_trun( fc, t ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Frame

signature void setup( mutable );
signature void mcode_run( mutable, tp_t name );

stamp :frame = aware :
{
    :arr_holor_s arr_holor; // array of holors
    :lib_mcode_s lib_mcode;
    :lib_fcode_s lib_fcode;
    bcore_hmap_name_s map_name;  // map of names used

    tp_t mcode_setup  = 0;
    tp_t mcode_shelve = 0;

    /** Optional interface for the frame:
     *  input, output index-reference corresponding holors
     *  They are setup by vm-builders
     */
    bcore_arr_sz_s input;
    bcore_arr_sz_s output;

    func :              : setup;
    func bcore_via_call : mutated = { :frame_s_setup( o ); };
    func bcore_via_call : shelve;

    /// mcode library functions
    func : :mcode_exists       = { return :lib_mcode_s_mcode_exists( &o->lib_mcode, name ); };
    func : :mcode_get          = { return :lib_mcode_s_mcode_get(    &o->lib_mcode, name ); };
    func : :mcode_get_or_new   = { return :lib_mcode_s_mcode_get_or_new( &o->lib_mcode, name ); };
    func : :mcode_reset        = { return :lib_mcode_s_mcode_reset(  &o->lib_mcode, name ); };
    func : :mcode_op_push_d    = { :lib_mcode_s_mcode_op_push_d(     &o->lib_mcode, name, op ); };
    func : :mcode_op_push_c    = { :lib_mcode_s_mcode_op_push_c(     &o->lib_mcode, name, op ); };
    func : :mcode_push         = { :lib_mcode_s_mcode_push(          &o->lib_mcode, name, src_name ); };
    func : :mcode_push_reverse = { :lib_mcode_s_mcode_push_reverse(  &o->lib_mcode, name, src_name ); };
    func : :mcode_remove       = { :lib_mcode_s_mcode_remove(        &o->lib_mcode, name ); };
    func : :mcode_run          = { :lib_mcode_s_mcode_mrun(          &o->lib_mcode, name, o->arr_holor.data ); };

    /// fcode library functions
    func : :fcode_exists       = { return :lib_fcode_s_fcode_exists( &o->lib_fcode, name ); };
    func : :fcode_get          = { return :lib_fcode_s_fcode_get(    &o->lib_fcode, name ); };
    func : :fcode_get_or_new   = { return :lib_fcode_s_fcode_get_or_new( &o->lib_fcode, name ); };
    func : :fcode_reset        = { return :lib_fcode_s_fcode_reset(  &o->lib_fcode, name ); };
    func : :fcode_pi_push_d    = { :lib_fcode_s_fcode_pi_push_d(     &o->lib_fcode, name, pi ); };
    func : :fcode_pi_push_c    = { :lib_fcode_s_fcode_pi_push_c(     &o->lib_fcode, name, pi ); };
    func : :fcode_push         = { :lib_fcode_s_fcode_push(          &o->lib_fcode, name, src_name ); };
    func : :fcode_remove       = { :lib_fcode_s_fcode_remove(        &o->lib_fcode, name ); };
    //func : :fcode_run          = { :lib_fcode_s_fcode_mrun(          &o->lib_fcode, name, o->arr_holor.data ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Privileged Instruction Set

group :pi =
{
    feature 'a' tp_t get_class( const );

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

};

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ---------------------------------------------------------------------------------------------------------------------
// vm_op

/// sets arguments for operator matching arr_sig to operator signature
void bhvm_hf3_vm_op_set_args( bhvm_hf3_vm_op* o, sc_t arr_sig, const bcore_arr_sz_s* arr_idx );

// ---------------------------------------------------------------------------------------------------------------------
// frame_s

/// error if frame integrity is faulty;
void bhvm_hf3_vm_frame_s_check_integrity( bhvm_hf3_vm_frame_s* o );

/// enrolls name (if not existing) and retuns type
tp_t bhvm_hf3_vm_frame_s_entypeof( bhvm_hf3_vm_frame_s* o, sc_t name );

/// returns name of type or NULL if not existing
sc_t bhvm_hf3_vm_frame_s_nameof( const bhvm_hf3_vm_frame_s* o, tp_t type );

/// returns name of type or "" if not existing
sc_t bhvm_hf3_vm_frame_s_ifnameof( const bhvm_hf3_vm_frame_s* o, tp_t type );

/// runs procedure 'setup'
void bhvm_hf3_vm_frame_s_setup( bhvm_hf3_vm_frame_s* o );

/// runs procedure 'shelve'
void bhvm_hf3_vm_frame_s_shelve( bhvm_hf3_vm_frame_s* o );

/// clears all content
void bhvm_hf3_vm_frame_s_clear( bhvm_hf3_vm_frame_s* o );

// ---------------------------------------------------------------------------------------------------------------------
// Microcode

/// inspection / debugging
void bhvm_hf3_vm_frame_s_mcode_to_sink(   const bhvm_hf3_vm_frame_s* o, tp_t name, bcore_sink* sink );
void bhvm_hf3_vm_frame_s_mcode_to_stdout( const bhvm_hf3_vm_frame_s* o, tp_t name );

// ---------------------------------------------------------------------------------------------------------------------
// Holors

void bhvm_hf3_vm_frame_s_holors_set_size( bhvm_hf3_vm_frame_s* o, sz_t size );
bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_holors_push( bhvm_hf3_vm_frame_s* o );
bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_holors_get_by_index( bhvm_hf3_vm_frame_s* o, sz_t index );

/// retrieves holor index; returns -1 if holor does not exists
sz_t bhvm_hf3_vm_frame_s_holors_get_index_by_name( bhvm_hf3_vm_frame_s* o, tp_t name );

/// allocates v_data (fit_v_size) of holors of given type
void bhvm_hf3_vm_frame_s_alloc_holors_of_type( bhvm_hf3_vm_frame_s* o, tp_t type );

/// de-allocates v_data of holors of given type
void bhvm_hf3_vm_frame_s_dealloc_holors_of_type( bhvm_hf3_vm_frame_s* o, tp_t type );

// ---------------------------------------------------------------------------------------------------------------------
// Holors for frame-input and frame-output

void bhvm_hf3_vm_frame_s_input_push(  bhvm_hf3_vm_frame_s* o, sz_t index );
void bhvm_hf3_vm_frame_s_output_push( bhvm_hf3_vm_frame_s* o, sz_t index );

bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_input_get_holor(  bhvm_hf3_vm_frame_s* o, sz_t index );
bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_output_get_holor( bhvm_hf3_vm_frame_s* o, sz_t index );
bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_input_get_paired_holor(  bhvm_hf3_vm_frame_s* o, sz_t index ); // returns NULL if holor has no pair
bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_output_get_paired_holor( bhvm_hf3_vm_frame_s* o, sz_t index ); // returns NULL if holor has no pair

bhvm_hf3_s* bhvm_hf3_vm_frame_s_input_get(         bhvm_hf3_vm_frame_s* o, sz_t index );
bhvm_hf3_s* bhvm_hf3_vm_frame_s_output_get(        bhvm_hf3_vm_frame_s* o, sz_t index );
bhvm_hf3_s* bhvm_hf3_vm_frame_s_input_get_paired(  bhvm_hf3_vm_frame_s* o, sz_t index ); // returns NULL if holor has no pair
bhvm_hf3_s* bhvm_hf3_vm_frame_s_output_get_paired( bhvm_hf3_vm_frame_s* o, sz_t index ); // returns NULL if holor has no pair

void bhvm_hf3_vm_frame_s_input_set( bhvm_hf3_vm_frame_s* o, sz_t index, bhvm_hf3_s* h ); // copies h

void bhvm_hf3_vm_frame_s_input_set_all(  bhvm_hf3_vm_frame_s* o, bhvm_hf3_adl_s* h ); // copies holors
void bhvm_hf3_vm_frame_s_output_get_all( bhvm_hf3_vm_frame_s* o, bhvm_hf3_adl_s* h ); // copies holors

/// sets paired holor for existing pairs.
void bhvm_hf3_vm_frame_s_output_set_paired_all( bhvm_hf3_vm_frame_s* o, bhvm_hf3_adl_s* h );

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// prototypes, inline
static inline bhvm_hf3_vm_op_ar1_unary_s* bhvm_hf3_vm_op_ar1_unary_s_create_unary( bmath_fp_f3_ar1 unary )
{
    bhvm_hf3_vm_op_ar1_unary_s* o = bhvm_hf3_vm_op_ar1_unary_s_create();
    o->unary = unary;
    return o;
}

#endif // TYPEOF_bhvm_hf3_vm

/**********************************************************************************************************************/

vd_t bhvm_hf3_vm_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_HF3_VM_H



