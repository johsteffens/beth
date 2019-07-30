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

#include "bmath_hf3_vm.h"

/**********************************************************************************************************************/
/// op_s

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_prop_s_setup_d( bmath_hf3_vm_prop_s* o, bmath_hf3_vm_op* op )
{
    if( op )
    {
        bcore_inst_a_attach( (bcore_inst**)&o->op, (bcore_inst*)op );
        o->p = (bmath_hf3_vm_s*)bmath_hf3_vm_s_get_aware( o->op );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// proc_s

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_proc_s_push_op_d( bmath_hf3_vm_proc_s* o, bmath_hf3_vm_op* op )
{
    if( op )
    {
        ASSERT( *(aware_t*)op != TYPEOF_bmath_hf3_vm_prop_s );
        bmath_hf3_vm_prop_s* vm_op = bmath_hf3_vm_proc_s_push( o );
        bmath_hf3_vm_prop_s_setup_d( vm_op, op );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// frame_s

// ---------------------------------------------------------------------------------------------------------------------

tp_t bmath_hf3_vm_frame_s_entypeof( bmath_hf3_vm_frame_s* o, sc_t name )
{
    return bcore_hmap_name_s_set_sc( &o->map_name, name );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_setup( bmath_hf3_vm_frame_s* o )
{
    if( !o->proc_setup ) return;
    if( !bmath_hf3_vm_frame_s_proc_exists( o, o->proc_setup ) ) return;
    bmath_hf3_vm_frame_s_run_proc( o, o->proc_setup );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_shelve( bmath_hf3_vm_frame_s* o )
{
    if( !o->proc_shelve ) return;
    if( !bmath_hf3_vm_frame_s_proc_exists( o, o->proc_shelve ) ) return;
    bmath_hf3_vm_frame_s_run_proc( o, o->proc_shelve );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_clear( bmath_hf3_vm_frame_s* o )
{
    bmath_hf3_vm_arr_holor_s_clear( &o->arr_holor );
    bmath_hf3_vm_library_s_clear( &o->library );
    bcore_hmap_tpuz_s_clear( &o->map_proc );
    bcore_hmap_tpuz_s_clear( &o->map_holor );
    bcore_hmap_name_s_clear( &o->map_name );
    o->proc_setup = 0;
    o->proc_shelve = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bmath_hf3_vm_frame_s_proc_exists( const bmath_hf3_vm_frame_s* o, tp_t name )
{
    return bcore_hmap_tpuz_s_exists( &o->map_proc, name );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_run_proc( bmath_hf3_vm_frame_s* o, tp_t name )
{
    if( !bcore_hmap_tpuz_s_exists( &o->map_proc, name ) )
    {
        ERR_fa( "'#<sc_t>' does not represent a procedure.", bcore_hmap_name_s_get_sc( &o->map_name, name ) );
    }
    sz_t idx = *bcore_hmap_tpuz_s_get( &o->map_proc, name );
    bmath_hf3_vm_proc_s* proc = &o->library.data[ idx ];
    bmath_hf3_vm_proc_s_run( proc, o->arr_holor.data );
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t bmath_hf3_vm_frame_s_get_holor_index( bmath_hf3_vm_frame_s* o, tp_t name )
{
    if( !bcore_hmap_tpuz_s_exists( &o->map_holor, name ) ) return -1;
    return *bcore_hmap_tpuz_s_get( &o->map_holor, name );
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_get_holor( bmath_hf3_vm_frame_s* o, tp_t name )
{
    sz_t idx = bmath_hf3_vm_frame_s_get_holor_index( o, name );
    return idx >= 0 ? &o->arr_holor.data[ idx ] : NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_alloc_holors( bmath_hf3_vm_frame_s* o, tp_t type )
{
    for( sz_t i = 0; i < o->arr_holor.size; i++ )
    {
        bmath_hf3_vm_holor_s* holor = &o->arr_holor.data[ i ];
        if( holor->type == type )
        {
            bmath_hf3_s_fit_v_size( &holor->hf3 );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_dealloc_holors( bmath_hf3_vm_frame_s* o, tp_t type )
{
    for( sz_t i = 0; i < o->arr_holor.size; i++ )
    {
        bmath_hf3_vm_holor_s* holor = &o->arr_holor.data[ i ];
        if( holor->type == type )
        {
            bmath_hf3_s_clear_v_data( &holor->hf3 );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_push_op_d( bmath_hf3_vm_frame_s* o, tp_t tp_proc, bmath_hf3_vm_op* op )
{
    if( !bcore_hmap_tpuz_s_exists( &o->map_proc, tp_proc ) )
    {
        bcore_hmap_tpuz_s_set( &o->map_proc, tp_proc, o->library.size );
        bmath_hf3_vm_library_s_push( &o->library );
    }

    sz_t idx = *bcore_hmap_tpuz_s_get( &o->map_proc, tp_proc );
    bmath_hf3_vm_proc_s* proc = &o->library.data[ idx ];
    bmath_hf3_vm_proc_s_push_op_d( proc, op );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bmath_hf3_vm_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_hf3_vm" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_get_quicktypes:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            return NULL;
        }
        break;

        case TYPEOF_plant:
        {
            bcore_plant_compile( "bmath_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

