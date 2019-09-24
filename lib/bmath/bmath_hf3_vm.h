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

#ifndef BMATH_HF3_VM_H
#define BMATH_HF3_VM_H

/**********************************************************************************************************************/

/** Virtual Machine representing the implementation of an operator-graph on Holor hf3.
 */

/**********************************************************************************************************************/

#include "bmath_hf3.h"

/**********************************************************************************************************************/

PLANT_GROUP( bmath_hf3_vm, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// holor
stamp :holor = aware :
{
    tp_t name;
    tp_t type;
    bmath_hf3_s h;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :arr_holor = aware bcore_array { :holor_s []; };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'ap' void run( const, :holor_s* hbase );

// procedural operator with preset perspective
stamp :prop = aware :
{
    aware :op => op;
    private :s* p; // bmath_hf3_vm perspective of op

    func bcore_inst_call : copy_x  = { o->p = o->op ? (:s*):s_get_aware( o->op ) : NULL; };
    func bcore_via_call  : mutated = { :prop_s_copy_x( o ); };

    func bmath_hf3_vm : run = { assert( o->p && o->p->run ); o->p->run( (vc_t)o->op, hbase ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// procedure
stamp :proc = aware bcore_array
{
    tp_t name;
    :prop_s [];
    bcore_arr_sz_s in;  // indices for input holors (if any)
    bcore_arr_sz_s out; // indices for output holors (if any)
    func bmath_hf3_vm : run =
    {
        for( sz_t i = 0; i < o->size; i++ ) :prop_s_run( &o->data[ i ], hbase );
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

    /// Extendable signature to create object, assign arguments and additional parameters
    signature :* csetup( mutable );

    /// (deprecated) Extendable signature for assigning arguments
    signature :* set_args( mutable );

    /// arity 0 operators
    group :ar0 =
    {
        func        :: :get_arity = { return 0; };
        signature   :: :csetup   csetup(   sz_t idx_a );
        feature 'a' :: :set_args set_args( sz_t idx_a );

        func :: :set_indices = { o->a = a[0]; };
        func :: :get_indices = { a[0] = o->a; };
        func  : :set_args = { o->a = idx_a; return (::*)o; };
        func  : :csetup = { if( !o ) o = @create(); o->a = idx_a; return (::*)o; };

        stamp :determine = aware :
        {
            sz_t a;
            func ::: :run = { bmath_hf3_s_fit_v_size( &hbase[ o->a ].h ); };
        };

        stamp :vacate = aware :
        {
            sz_t a;
            func ::: :run = { bmath_hf3_s_set_vacant( &hbase[ o->a ].h ); };
        };

        signature : : csetup csetup_randomize( u2_t rseed );
        stamp :randomize = aware :
        {
            sz_t a;
            u2_t rseed = 1234;
            func   : :csetup_randomize = { if( !o ) o = :randomize_s_create(); o->a = idx_a; o->rseed = rseed; return (::*)o; };
            func ::: :run = { u2_t rval = o->rseed + o->a; bmath_hf3_s_set_random( &hbase[ o->a ].h, 1.0, -1.0, 1.0, &rval ); };
        };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// arity 1 operators
    group :ar1 =
    {
        func        :: :get_arity = { return 1; };
        signature   :: :csetup   csetup(   sz_t idx_a, sz_t idx_b );
        feature 'a' :: :set_args set_args( sz_t idx_a, sz_t idx_b );

        func :: :set_indices = { o->a = a[0]; o->b = a[1]; };
        func :: :get_indices = { a[0] = o->a; a[1] = o->b; };
        func  : :set_args = { o->a = idx_a; o->b = idx_b; return (::*)o; };
        func  : :csetup   = { if( !o ) o = @create(); o->a = idx_a; o->b = idx_b; return (::*)o; };

        /// deprecated: use cpy  (term 'identity' should be used in higher abstractions)
        stamp :identity = aware :
        {
            sz_t a; sz_t b;
            func ::: :run = { bmath_hf3_s_cpy( &hbase[ o->a ].h, &hbase[ o->b ].h ); };
        };

        /// a -> b
        stamp :cpy = aware :
        {
            sz_t a; sz_t b;
            func ::: :run = { bmath_hf3_s_cpy( &hbase[ o->a ].h, &hbase[ o->b ].h ); };
        };

        /// tanh(a) -> b
        stamp :tanh = aware :
        {
            sz_t a; sz_t b;
            func ::: :run = { bmath_hf3_s_tanh( &hbase[ o->a ].h, &hbase[ o->b ].h ); };
        };

        /// unary(a) -> b
        stamp :unary = aware :
        {
            sz_t a; sz_t b;
            bmath_fp_f3_ar1 unary;
            func ::: :run = { bmath_hf3_s_fp_f3_ar1( &hbase[ o->a ].h, o->unary, &hbase[ o->b ].h ); };
        };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// arity 2 operators
    group :ar2 =
    {
        func        :: :get_arity = { return 2; };
        signature   :: :csetup   csetup(   sz_t idx_a, sz_t idx_b, sz_t idx_c );
        feature 'a' :: :set_args set_args( sz_t idx_a, sz_t idx_b, sz_t idx_c );

        func :: :set_indices = { o->a = a[0]; o->b = a[1]; o->c = a[2]; };
        func :: :get_indices = { a[0] = o->a; a[1] = o->b; a[2] = o->c; };
        func  : :set_args = { o->a = idx_a; o->b = idx_b; o->c = idx_c; return (::*)o; };
        func  : :csetup   = { if( !o ) o = @create(); @set_args( o, idx_a, idx_b, idx_c); return (::*)o; };

        /// a + b -> c
        stamp :add = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func ::: :run = { bmath_hf3_s_add( &hbase[ o->a ].h, &hbase[ o->b ].h, &hbase[ o->c ].h ); };
        };

        /// a - b -> c
        stamp :sub = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func ::: :run = { bmath_hf3_s_sub( &hbase[ o->a ].h, &hbase[ o->b ].h, &hbase[ o->c ].h ); };
        };

        /// a ** b -> c
        stamp :bmul = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func ::: :run =
            {
                bmath_hf3_s_bmul( &hbase[ o->a ].h, &hbase[ o->b ].h, &hbase[ o->c ].h );
            };
        };

        /// a *^ b -> c
        stamp :bmul_htp = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func ::: :run = { bmath_hf3_s_bmul_htp( &hbase[ o->a ].h, &hbase[ o->b ].h, &hbase[ o->c ].h ); };
        };

        /// a ^* b -> c
        stamp :htp_bmul = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func ::: :run = { bmath_hf3_s_htp_bmul( &hbase[ o->a ].h, &hbase[ o->b ].h, &hbase[ o->c ].h ); };
        };

        /// a ^*^ b -> c
        stamp :htp_bmul_htp = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func ::: :run = { bmath_hf3_s_htp_bmul_htp( &hbase[ o->a ].h, &hbase[ o->b ].h, &hbase[ o->c ].h ); };
        };

        /// a * b -> c
        stamp :hmul = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func ::: :run = { bmath_hf3_s_hmul( &hbase[ o->a ].h, &hbase[ o->b ].h, &hbase[ o->c ].h ); };
        };

        /// a * s(b) -> c
        stamp :mul_scl = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func ::: :run = { bmath_hf3_s_mul_scl( &hbase[ o->a ].h, hbase[ o->b ].h.v_data, &hbase[ o->c ].h ); };
        };

        /// s(a) * b -> c
        stamp :scl_mul = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func ::: :run = { bmath_hf3_s_mul_scl( &hbase[ o->b ].h, hbase[ o->a ].h.v_data, &hbase[ o->c ].h ); };
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

    tp_t proc_setup = 0;
    tp_t proc_shelve = 0;

    bcore_hmap_tpuz_s map_proc;  // associates a name with a procedure
    bcore_hmap_tpuz_s map_holor; // associates a name with a holor
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
// proc_s

void bmath_hf3_vm_proc_s_push_op_d( bmath_hf3_vm_proc_s* o, bmath_hf3_vm_op* op );

// ---------------------------------------------------------------------------------------------------------------------
// frame_s

/// error if frame integrity is faulty;
void bmath_hf3_vm_frame_s_check_integrity( bmath_hf3_vm_frame_s* o );

/// enrolls name (if not existing) and retuns type
tp_t bmath_hf3_vm_frame_s_entypeof( bmath_hf3_vm_frame_s* o, sc_t name );

/// runs procedure 'setup'
void bmath_hf3_vm_frame_s_setup( bmath_hf3_vm_frame_s* o );

/// runs procedure 'shelve'
void bmath_hf3_vm_frame_s_shelve( bmath_hf3_vm_frame_s* o );

/// clears all content
void bmath_hf3_vm_frame_s_clear( bmath_hf3_vm_frame_s* o );

// Procedures

/// checks if procedure exists
bl_t bmath_hf3_vm_frame_s_proc_exists( const bmath_hf3_vm_frame_s* o, tp_t name );

/// adds a new procedure if not already existing
bmath_hf3_vm_proc_s* bmath_hf3_vm_frame_s_proc_push( bmath_hf3_vm_frame_s* o, tp_t name );

/// adds a new procedure if not already existing; if it exists it is set to size 0
bmath_hf3_vm_proc_s* bmath_hf3_vm_frame_s_proc_reset( bmath_hf3_vm_frame_s* o, tp_t name );

/// retrieves a procedure
bmath_hf3_vm_proc_s* bmath_hf3_vm_frame_s_proc_get( const bmath_hf3_vm_frame_s* o, tp_t name );

/// retrieves a procedure or adds it if not existing
bmath_hf3_vm_proc_s* bmath_hf3_vm_frame_s_proc_get_or_push( bmath_hf3_vm_frame_s* o, tp_t name );

/// runs a procedure
void bmath_hf3_vm_frame_s_proc_run( bmath_hf3_vm_frame_s* o, tp_t name );

/// removes a procedure if existing
void bmath_hf3_vm_frame_s_proc_remove( bmath_hf3_vm_frame_s* o, tp_t name );

/// pushes operation to given procedure; creates procedure if not existing
void bmath_hf3_vm_frame_s_proc_push_op_d( bmath_hf3_vm_frame_s* o, tp_t proc, bmath_hf3_vm_op* op );

// Holors

void bmath_hf3_vm_frame_s_holors_set_size( bmath_hf3_vm_frame_s* o, sz_t size );
void bmath_hf3_vm_frame_s_holors_setup_name_map( bmath_hf3_vm_frame_s* o );
bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_holors_get_by_index(      bmath_hf3_vm_frame_s* o, sz_t index );

/// retrieves holor index; returns -1 if holor does not exists
sz_t bmath_hf3_vm_frame_s_holors_get_index_by_name( bmath_hf3_vm_frame_s* o, tp_t name );

/// retrieves holor by name; returns NULL if holor does not exists
bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_holors_get_by_name( bmath_hf3_vm_frame_s* o, tp_t name );

/// allocates v_data (fit_v_size) of holors of given type
void bmath_hf3_vm_frame_s_alloc_holors_of_type( bmath_hf3_vm_frame_s* o, tp_t type );

/// de-allocates v_data of holors of given type
void bmath_hf3_vm_frame_s_dealloc_holors_of_type( bmath_hf3_vm_frame_s* o, tp_t type );

// ---------------------------------------------------------------------------------------------------------------------
// input/output holors

void                  bmath_hf3_vm_frame_s_input_push(      bmath_hf3_vm_frame_s* o, sz_t index );
bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_input_get_holor( bmath_hf3_vm_frame_s* o, sz_t index );

void                  bmath_hf3_vm_frame_s_output_push(      bmath_hf3_vm_frame_s* o, sz_t index );
bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_output_get_holor( bmath_hf3_vm_frame_s* o, sz_t index );

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// prototypes, inline
static inline bmath_hf3_vm_op_ar1_unary_s* bmath_hf3_vm_op_ar1_unary_s_create_unary( bmath_fp_f3_ar1 unary )
{
    bmath_hf3_vm_op_ar1_unary_s* o = bmath_hf3_vm_op_ar1_unary_s_create();
    o->unary = unary;
    return o;
}

/**********************************************************************************************************************/

vd_t bmath_hf3_vm_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_HF3_VM_H



