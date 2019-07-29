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
    bmath_hf3_s hf3;
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

    func bcore_inst_call : copy_x  = { o->p = (:s*):s_get_aware( o->op ); };
    func bcore_via_call  : mutated = { :prop_s_copy_x( o ); };

    func bmath_hf3_vm : run = { :p_run( o->p, (vc_t)o->op, hbase ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// procedure
stamp :proc = aware bcore_array
{
    tp_t name;
    :prop_s [];
    bcore_arr_sz_s in;  // indices for input holors (if any)
    bcore_arr_sz_s out; // indices for output holors (if any)
    func bmath_hf3_vm : run = { for( sz_t i = 0; i < o->size; i++ ) :prop_s_run( &o->data[ i ], hbase ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// library: array of procedures
stamp :library = aware bcore_array
{
    :proc_s [];
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Operators

group :op =
{

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// arity 0 operators
    group :ar0 =
    {
        feature 'a' void set_args( mutable, sz_t idx_a );
        body body_set_args = { o->a = idx_a; };

        stamp ::set_determined = aware :
        {
            sz_t a;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_fit_v_size( &hbase[ o->a ].hf3 ); };
        };

        stamp ::set_vacant = aware :
        {
            sz_t a;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_set_vacant( &hbase[ o->a ].hf3 ); };
        };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// arity 1 operators
    group :ar1 =
    {
        feature 'a' void set_args( mutable, sz_t idx_a, sz_t idx_b );
        body body_set_args = { o->a = idx_a; o->b = idx_b; };

        /// a -> b
        stamp ::linear = aware :
        {
            sz_t a; sz_t b;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_cpy( &hbase[ o->a ].hf3, &hbase[ o->b ].hf3 ); };
        };

        /// tanh(a) -> b
        stamp ::tanh = aware :
        {
            sz_t a; sz_t b;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_tanh( &hbase[ o->a ].hf3, &hbase[ o->b ].hf3 ); };
        };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// arity 2 operators
    group :ar2 =
    {
        feature 'a' void set_args( mutable, sz_t idx_a, sz_t idx_b, sz_t idx_c );
        body body_set_args = { o->a = idx_a; o->b = idx_b; o->c = idx_c; };

        /// a + b -> c
        stamp ::add = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_add( &hbase[ o->a ].hf3, &hbase[ o->b ].hf3, &hbase[ o->c ].hf3 ); };
        };

        /// a - b -> c
        stamp ::sub = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_sub( &hbase[ o->a ].hf3, &hbase[ o->b ].hf3, &hbase[ o->c ].hf3 ); };
        };

        /// a <*> b -> c
        stamp ::bmul = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_bmul( &hbase[ o->a ].hf3, &hbase[ o->b ].hf3, &hbase[ o->c ].hf3 ); };
        };

        /// a <*t> b -> c
        stamp ::bmul_htp = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_bmul_htp( &hbase[ o->a ].hf3, &hbase[ o->b ].hf3, &hbase[ o->c ].hf3 ); };
        };

        /// a <t*> b -> c
        stamp ::htp_bmul = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_htp_bmul( &hbase[ o->a ].hf3, &hbase[ o->b ].hf3, &hbase[ o->c ].hf3 ); };
        };

        /// a <t*t> b -> c
        stamp ::htp_bmul_htp = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_htp_bmul_htp( &hbase[ o->a ].hf3, &hbase[ o->b ].hf3, &hbase[ o->c ].hf3 ); };
        };

        /// a * b -> c
        stamp ::hmul = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_hmul( &hbase[ o->a ].hf3, &hbase[ o->b ].hf3, &hbase[ o->c ].hf3 ); };
        };

        /// a * s(b) -> c
        stamp ::mul_scl = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_mul_scl( &hbase[ o->a ].hf3, hbase[ o->b ].hf3.v_data, &hbase[ o->c ].hf3 ); };
        };

        /// s(a) * b -> c
        stamp ::scl_mul = aware :
        {
            sz_t a; sz_t b; sz_t c;
            func   : :set_args = :body_set_args;
            func ::: :run = { bmath_hf3_s_mul_scl( &hbase[ o->b ].hf3, hbase[ o->a ].hf3.v_data, &hbase[ o->c ].hf3 ); };
        };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :frame = aware :
{
    :arr_holor_s arr_holor; // array of holors
    :library_s   library;  // array of principal functions

    bcore_hmap_tpuz_s map_proc;  // associates a name with a procedure
    bcore_hmap_tpuz_s map_holor; // associates a name with a holor
    bcore_hmap_name_s map_name;  // map of names used
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ---------------------------------------------------------------------------------------------------------------------
// proc_s

/// (!)do not push bmath_hf3_vm_op_s here
void bmath_hf3_vm_proc_s_push_op_d( bmath_hf3_vm_proc_s* o, bmath_hf3_vm_op* op );

// ---------------------------------------------------------------------------------------------------------------------
// frame_s

/// clears all content
void bmath_hf3_vm_frame_s_clear( bmath_hf3_vm_frame_s* o );

/// runs a procedure
void bmath_hf3_vm_frame_s_run_proc( bmath_hf3_vm_frame_s* o, tp_t name );

/// retrieves holor index; returns -1 if holor does not exists
sz_t bmath_hf3_vm_frame_s_get_holor_index( bmath_hf3_vm_frame_s* o, tp_t name );

/// retrieves holor; returns NULL if holor does not exists
bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_get_holor( bmath_hf3_vm_frame_s* o, tp_t name );

/// allocates v_data (fit_v_size) of holors of given type
void bmath_hf3_vm_frame_s_alloc_holors( bmath_hf3_vm_frame_s* o, tp_t type );

/// de-allocates v_data of holors of given type
void bmath_hf3_vm_frame_s_dealloc_holors( bmath_hf3_vm_frame_s* o, tp_t type );

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_hf3_vm_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_HF3_VM_H



