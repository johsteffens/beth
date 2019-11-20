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

#include "bhvm_hf3.h"
#include "bhvm_hf3_op.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm_hf3_vm

PLANT_GROUP( bhvm_hf3_vm, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// holor
stamp :holor = aware :
{
    tp_t name;
    tp_t type;
    sz_t idx_paired = -1; // index to holor paired with this one (e.g. axon <=> axon-gradient)
    bhvm_hf3_s h;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :arr_holor = aware bcore_array { :holor_s []; };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'ap' void run( const, :holor_s* ah );

// procedural operator with preset perspective
stamp :prop = aware :
{
    aware :op => op;
    private :s* p; // bhvm_hf3_vm perspective of op

    func bcore_inst_call : copy_x  = { o->p = o->op ? (:s*):s_get_aware( o->op ) : NULL; };
    func bcore_via_call  : mutated = { :prop_s_copy_x( o ); };
    func bhvm_hf3_vm     : run = { assert( o->p && o->p->run ); o->p->run( (vc_t)o->op, ah ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// procedure
stamp :proc = aware bcore_array
{
    tp_t name;
    :prop_s [];
    bcore_arr_sz_s in;  // indices for input holors (if any)
    bcore_arr_sz_s out; // indices for output holors (if any)
    func bhvm_hf3_vm : run =
    {
        for( sz_t i = 0; i < o->size; i++ ) :prop_s_run( &o->data[ i ], ah );
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// library: array of procedures
stamp :library = aware bcore_array
{
    :proc_s => [];
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Operators

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
            func  : :csetup = { if( !o ) o = @create(); o->a = idx_a; return (::*)o; };
        };

        func :: :set_arg =
        {
            if( id == @sig( o )[0] ) o->a = idx;
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
            func  : :csetup      = { if( !o ) o = @create(); o->a = idx_a; o->b = idx_b; return (::*)o; };
        };

        func :: :set_arg =
        {
            if( id == @sig( o )[ 0 ] ) o->a = idx;
            if( id == @sig( o )[ 1 ] ) o->b = idx;
        };

        func :: :sig = { return "ay"; }; // default signature
        stamp :unary = { bmath_fp_f3_ar1 unary; func ::: :run = { bhvm_hf3_s_fp_f3_ar1( &ah[o->a].h, o->unary, &ah[o->b].h ); }; };

        stamp :cpy        = { func ::: :run = { bhvm_hf3_op_ar1_cpy_s_f  ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :neg        = { func ::: :run = { bhvm_hf3_op_ar1_neg_s_f  ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :floor      = { func ::: :run = { bhvm_hf3_op_ar1_floor_s_f( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :ceil       = { func ::: :run = { bhvm_hf3_op_ar1_ceil_s_f ( &ah[o->a].h, &ah[o->b].h ); }; };
        stamp :exp        = { func ::: :run = { bhvm_hf3_op_ar1_exp_s_f  ( &ah[o->a].h, &ah[o->b].h ); }; };

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
            func  : :csetup   = { if( !o ) o = @create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; return (::*)o; };
        };

        func :: :set_arg =
        {
            if( id == @sig( o )[ 0 ] ) o->a = idx;
            if( id == @sig( o )[ 1 ] ) o->b = idx;
            if( id == @sig( o )[ 2 ] ) o->c = idx;
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
            func  : :csetup   = { if( !o ) o = @create(); o->a = idx_a; o->b = idx_b; o->c = idx_c; o->d = idx_d; return (::*)o; };
        };

        func :: :set_arg =
        {
            if( id == @sig( o )[ 0 ] ) o->a = idx;
            if( id == @sig( o )[ 1 ] ) o->b = idx;
            if( id == @sig( o )[ 2 ] ) o->c = idx;
            if( id == @sig( o )[ 3 ] ) o->d = idx;
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

signature void setup( mutable );

stamp :frame = aware :
{
    :arr_holor_s arr_holor; // array of holors
    :library_s   library;   // array of principal functions

    tp_t proc_setup  = 0;
    tp_t proc_shelve = 0;

    bcore_hmap_tpuz_s map_proc;  // associates a name with a procedure
    bcore_hmap_name_s map_name;  // map of names used

    /** Optional interface for the frame:
     *  input, output index-reference corresponding holors
     *  They are setup by vm-builders
     */
    bcore_arr_sz_s input;
    bcore_arr_sz_s output;

    func :              : setup;
    func bcore_via_call : mutated = { :frame_s_setup( o ); };
    func bcore_via_call : shelve;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ---------------------------------------------------------------------------------------------------------------------
// vm_op

/// sets arguments for operator matching arr_sig to operator signature
void bhvm_hf3_vm_op_set_args( bhvm_hf3_vm_op* o, sc_t arr_sig, const bcore_arr_sz_s* arr_idx );

// ---------------------------------------------------------------------------------------------------------------------
// proc_s

void bhvm_hf3_vm_proc_s_push_op_d( bhvm_hf3_vm_proc_s* o, bhvm_hf3_vm_op* op );

// ---------------------------------------------------------------------------------------------------------------------
// frame_s

/// error if frame integrity is faulty;
void bhvm_hf3_vm_frame_s_check_integrity( bhvm_hf3_vm_frame_s* o );

/// enrolls name (if not existing) and retuns type
tp_t bhvm_hf3_vm_frame_s_entypeof( bhvm_hf3_vm_frame_s* o, sc_t name );

/// returns name of type or "" if not existing
sc_t bhvm_hf3_vm_frame_s_ifnameof( bhvm_hf3_vm_frame_s* o, tp_t type );

/// runs procedure 'setup'
void bhvm_hf3_vm_frame_s_setup( bhvm_hf3_vm_frame_s* o );

/// runs procedure 'shelve'
void bhvm_hf3_vm_frame_s_shelve( bhvm_hf3_vm_frame_s* o );

/// clears all content
void bhvm_hf3_vm_frame_s_clear( bhvm_hf3_vm_frame_s* o );

// Procedures

/// checks if procedure exists
bl_t bhvm_hf3_vm_frame_s_proc_exists( const bhvm_hf3_vm_frame_s* o, tp_t name );

/// adds a new procedure if not already existing
bhvm_hf3_vm_proc_s* bhvm_hf3_vm_frame_s_proc_push( bhvm_hf3_vm_frame_s* o, tp_t name );

/// adds a new procedure if not already existing; if it exists it is set to size 0
bhvm_hf3_vm_proc_s* bhvm_hf3_vm_frame_s_proc_reset( bhvm_hf3_vm_frame_s* o, tp_t name );

/// retrieves a procedure; returns NULL if not existing
bhvm_hf3_vm_proc_s* bhvm_hf3_vm_frame_s_proc_get( const bhvm_hf3_vm_frame_s* o, tp_t name );

/// retrieves a procedure or adds it if not existing
bhvm_hf3_vm_proc_s* bhvm_hf3_vm_frame_s_proc_get_or_push( bhvm_hf3_vm_frame_s* o, tp_t name );

/// runs a procedure
void bhvm_hf3_vm_frame_s_proc_run( bhvm_hf3_vm_frame_s* o, tp_t name );

/// removes a procedure if existing
void bhvm_hf3_vm_frame_s_proc_remove( bhvm_hf3_vm_frame_s* o, tp_t name );

/// pushes operation to given procedure; creates procedure if not existing
void bhvm_hf3_vm_frame_s_proc_push_op_d( bhvm_hf3_vm_frame_s* o, tp_t proc, bhvm_hf3_vm_op* op );
void bhvm_hf3_vm_frame_s_proc_push_op_c( bhvm_hf3_vm_frame_s* o, tp_t proc, const bhvm_hf3_vm_op* op ); // copies op

/// appends all operations of src_proc to proc; if src_proc does not exists, nothing happens
void bhvm_hf3_vm_frame_s_proc_append_proc(         bhvm_hf3_vm_frame_s* o, tp_t proc, tp_t src_proc );
void bhvm_hf3_vm_frame_s_proc_append_proc_reverse( bhvm_hf3_vm_frame_s* o, tp_t proc, tp_t src_proc ); // appends in reverse order

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
// input/output holors

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



