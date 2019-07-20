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

feature 'ap' void run( const, :arr_holor_s* arr_holor );

// operator with preset perspective
stamp :op = aware :
{
    aware : => op;
    private :s* p; // bmath_hf3_vm perspective of op

    func bcore_inst_call : copy_x  = { o->p = (:s*):s_get_aware( o->op ); };
    func bcore_via_call  : mutated = { :op_s_copy_x( o ); };

    func bmath_hf3_vm : run = { :p_run( o->p, o->op, arr_holor ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// procedure
stamp :proc = aware bcore_array
{
    tp_t name;
    :op_s [];
    func bmath_hf3_vm : run = { for( sz_t i = 0; i < o->size; i++ ) :op_s_run( &o->data[ i ], arr_holor ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :arr_proc = aware bcore_array { :proc_s []; };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// Operators

feature 'a' void set_arg( mutable, char id, sz_t idx ) = { ERR_fa( "Not implemented in #<sc_t>", ifnameof( *(aware_t*)o ) ); };

body set_arg_a =
{
    if     ( id == 'a' ) o->a = idx;
    else ERR_fa( "Operator '#<sc_t>': Invalid arg id '#<char>'.", ifnameof( *(aware_t*)o ), id );
};

body set_arg_ab =
{
    if     ( id == 'a' ) o->a = idx;
    else if( id == 'b' ) o->b = idx;
    else ERR_fa( "Operator '#<sc_t>': Invalid arg id '#<char>'.", ifnameof( *(aware_t*)o ), id );
};

body set_arg_abc =
{
    if     ( id == 'a' ) o->a = idx;
    else if( id == 'b' ) o->b = idx;
    else if( id == 'c' ) o->c = idx;
    else ERR_fa( "Operator '#<sc_t>': Invalid arg id '#<char>'.", ifnameof( *(aware_t*)o ), id );
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// a -> b
stamp :op_linear = aware :
{
    sz_t a; sz_t b;
    func : : run = { bmath_hf3_s_cpy( &arr_holor->data[ o->a ].hf3, &arr_holor->data[ o->b ].hf3 ); };
    func : : set_arg = :set_arg_ab;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// tanh(a) -> b
stamp :op_tanh = aware :
{
    sz_t a; sz_t b;
    func : : run = { bmath_hf3_s_tanh( &arr_holor->data[ o->a ].hf3, &arr_holor->data[ o->b ].hf3 ); };
    func : : set_arg = :set_arg_ab;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// a + b -> c
stamp :op_add = aware :
{
    sz_t a; sz_t b; sz_t c;
    func : : run = { bmath_hf3_s_add( &arr_holor->data[ o->a ].hf3, &arr_holor->data[ o->b ].hf3, &arr_holor->data[ o->c ].hf3 ); };
    func : : set_arg = :set_arg_abc;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// a - b -> c
stamp :op_sub = aware :
{
    sz_t a; sz_t b; sz_t c;
    func bmath_hf3_vm : run = { bmath_hf3_s_sub( &arr_holor->data[ o->a ].hf3, &arr_holor->data[ o->b ].hf3, &arr_holor->data[ o->c ].hf3 ); };
    func : : set_arg = :set_arg_abc;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// a * b -> c
stamp :op_kmul = aware :
{
    sz_t k = 1;
    sz_t a; sz_t b; sz_t c;
    func bmath_hf3_vm : run = { bmath_hf3_s_kmul( &arr_holor->data[ o->a ].hf3, o->k, &arr_holor->data[ o->b ].hf3, &arr_holor->data[ o->c ].hf3 ); };
    func : : set_arg = :set_arg_abc;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// a <*> b -> c
stamp :op_hmul = aware :
{
    sz_t a; sz_t b; sz_t c;
    func bmath_hf3_vm : run = { bmath_hf3_s_hmul( &arr_holor->data[ o->a ].hf3, &arr_holor->data[ o->b ].hf3, &arr_holor->data[ o->c ].hf3 ); };
    func : : set_arg = :set_arg_abc;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :frame = aware :
{
    :arr_holor_s arr_holor; // array of holors
    :arr_proc_s  arr_proc;  // array of principal functions

    bcore_hmap_tpuz_s map_proc;  // associates a name with a procedure
    bcore_hmap_tpuz_s map_holor; // associates a name with a holor
    bcore_hmap_name_s map_name;  // map of names used
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ---------------------------------------------------------------------------------------------------------------------
// proc_s

/// do not push bmath_hf3_vm_op_s here
void bmath_hf3_vm_proc_s_push_op_d( bmath_hf3_vm_proc_s* o, bmath_hf3_vm* op );

// ---------------------------------------------------------------------------------------------------------------------
// frame_s

/// clears all content
void bmath_hf3_vm_frame_s_clear( bmath_hf3_vm_frame_s* o );

/// runs a procedure
void bmath_hf3_vm_frame_s_run_proc( bmath_hf3_vm_frame_s* o, tp_t name );

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



