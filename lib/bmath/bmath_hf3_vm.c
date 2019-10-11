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
// vm_op

void bmath_hf3_vm_op_set_args( bmath_hf3_vm_op* o, sc_t arr_sig, const bcore_arr_sz_s* arr_idx )
{
    for( sz_t i = 0; i < arr_idx->size; i++ )
    {
        if( !arr_sig ) break;
        if( arr_sig[ i ] == 0 ) break;
        bmath_hf3_vm_op_a_set_arg( o, arr_sig[ i ], arr_idx->data[ i ] );
    }
}

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

void bmath_hf3_vm_frame_s_check_integrity( bmath_hf3_vm_frame_s* o )
{
    for( sz_t i = 0; i < o->library.size; i++ )
    {
        bmath_hf3_vm_proc_s* proc = o->library.data[ i ];
        for( sz_t j = 0; j < proc->size; j++ )
        {
            bmath_hf3_vm_prop_s* prop = &proc->data[ j ];
            ASSERT( prop->op );
            sz_t arity = bmath_hf3_vm_op_a_get_arity( prop->op );
            sz_t* a = bcore_u_alloc( sizeof( sz_t ), NULL, arity + 1, NULL );
            bmath_hf3_vm_op_a_get_indices( prop->op, a );
            for( sz_t k = 0; k <= arity; k++ )
            {
                sz_t idx = a[ k ];
                if( idx < 0 || idx >= o->arr_holor.size )
                {
                    ERR_fa
                    (
                        "In procedure #<sz_t>, operation #<sz_t>:\n"
                        "Index #<sz_t> of operator #<sc_t> has value #<sz_t>, which is out of range.",
                        i, j, k, ifnameof( prop->op->_ ), idx
                    );
                }
            }
            bcore_free( a );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

tp_t bmath_hf3_vm_frame_s_entypeof( bmath_hf3_vm_frame_s* o, sc_t name )
{
    return bcore_hmap_name_s_set_sc( &o->map_name, name );
}

// ---------------------------------------------------------------------------------------------------------------------

sc_t bmath_hf3_vm_frame_s_ifnameof( bmath_hf3_vm_frame_s* o, tp_t type )
{
    sc_t name = bcore_hmap_name_s_get_sc( &o->map_name, type );
    return name ? name : "";
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_setup( bmath_hf3_vm_frame_s* o )
{
    if( !o->proc_setup ) return;
    if( !bmath_hf3_vm_frame_s_proc_exists( o, o->proc_setup ) ) return;
    bmath_hf3_vm_frame_s_proc_run( o, o->proc_setup );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_shelve( bmath_hf3_vm_frame_s* o )
{
    if( !o->proc_shelve ) return;
    if( !bmath_hf3_vm_frame_s_proc_exists( o, o->proc_shelve ) ) return;
    bmath_hf3_vm_frame_s_proc_run( o, o->proc_shelve );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_clear( bmath_hf3_vm_frame_s* o )
{
    bmath_hf3_vm_arr_holor_s_clear( &o->arr_holor );
    bmath_hf3_vm_library_s_clear( &o->library );
    bcore_hmap_tpuz_s_clear( &o->map_proc );
    bcore_hmap_name_s_clear( &o->map_name );
    o->proc_setup = 0;
    o->proc_shelve = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

// Procedures

// ---------------------------------------------------------------------------------------------------------------------

bl_t bmath_hf3_vm_frame_s_proc_exists( const bmath_hf3_vm_frame_s* o, tp_t name )
{
    return bcore_hmap_tpuz_s_exists( &o->map_proc, name );
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_vm_proc_s* bmath_hf3_vm_frame_s_proc_push( bmath_hf3_vm_frame_s* o, tp_t name )
{
    ASSERT( !bmath_hf3_vm_frame_s_proc_exists( o, name ) );
    bmath_hf3_vm_proc_s* proc = bmath_hf3_vm_library_s_push( &o->library );
    proc->name = name;
    bcore_hmap_tpuz_s_set( &o->map_proc, name, o->library.size - 1 );
    return proc;
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_vm_proc_s* bmath_hf3_vm_frame_s_proc_reset( bmath_hf3_vm_frame_s* o, tp_t name )
{
    if( !bmath_hf3_vm_frame_s_proc_exists( o, name ) )
    {
        return bmath_hf3_vm_frame_s_proc_push( o, name );
    }
    else
    {
        bmath_hf3_vm_proc_s* proc = bmath_hf3_vm_frame_s_proc_get( o, name );
        bmath_hf3_vm_proc_s_clear( proc );
        return proc;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_vm_proc_s* bmath_hf3_vm_frame_s_proc_get( const bmath_hf3_vm_frame_s* o, tp_t name )
{
    uz_t* p_idx = bcore_hmap_tpuz_s_get( &o->map_proc, name );
    if( !p_idx ) return NULL;
    sz_t idx = *p_idx;
    bmath_hf3_vm_proc_s* proc = o->library.data[ idx ];
    return proc;
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_vm_proc_s* bmath_hf3_vm_frame_s_proc_get_or_push( bmath_hf3_vm_frame_s* o, tp_t name )
{
    bmath_hf3_vm_proc_s* proc = bmath_hf3_vm_frame_s_proc_get( o, name );
    if( !proc ) proc = bmath_hf3_vm_frame_s_proc_push( o, name );
    return proc;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_proc_run( bmath_hf3_vm_frame_s* o, tp_t name )
{
    if( !bmath_hf3_vm_frame_s_proc_exists( o, name ) )
    {
        ERR_fa( "Procedure '#<sc_t>' does not exist.", bcore_hmap_name_s_get_sc( &o->map_name, name ) );
    }
    sz_t idx = *bcore_hmap_tpuz_s_get( &o->map_proc, name );
    bmath_hf3_vm_proc_s* proc = o->library.data[ idx ];
    bmath_hf3_vm_proc_s_run( proc, o->arr_holor.data );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_proc_remove( bmath_hf3_vm_frame_s* o, tp_t name )
{
    uz_t* p_idx = bcore_hmap_tpuz_s_get( &o->map_proc, name );
    if( !p_idx ) return;
    sz_t idx = *p_idx;
    bmath_hf3_vm_proc_s_detach( &o->library.data[ idx ] );
    sz_t last_idx = o->library.size - 1;
    if( idx != last_idx )
    {
        bmath_hf3_vm_proc_s* last_proc = o->library.data[ last_idx ];
        o->library.data[ idx ] = last_proc;
        o->library.data[ last_idx ] = NULL;
        bcore_hmap_tpuz_s_set( &o->map_proc, last_proc->name, idx );
    }
    bcore_array_a_pop( ( bcore_array* )&o->library );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_proc_push_op_d( bmath_hf3_vm_frame_s* o, tp_t tp_proc, bmath_hf3_vm_op* op )
{
    if( !bcore_hmap_tpuz_s_exists( &o->map_proc, tp_proc ) )
    {
        bcore_hmap_tpuz_s_set( &o->map_proc, tp_proc, o->library.size );
        bmath_hf3_vm_library_s_push( &o->library );
    }

    sz_t idx = *bcore_hmap_tpuz_s_get( &o->map_proc, tp_proc );
    bmath_hf3_vm_proc_s* proc = o->library.data[ idx ];
    bmath_hf3_vm_proc_s_push_op_d( proc, op );
}

// ---------------------------------------------------------------------------------------------------------------------

// Holors

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_holors_set_size( bmath_hf3_vm_frame_s* o, sz_t size )
{
    bmath_hf3_vm_arr_holor_s_set_size( &o->arr_holor, size );
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_holors_push( bmath_hf3_vm_frame_s* o )
{
    return bmath_hf3_vm_arr_holor_s_push( &o->arr_holor );
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_holors_get_by_index( bmath_hf3_vm_frame_s* o, sz_t index )
{
    ASSERT( index >= 0 && index < o->arr_holor.size );
    return &o->arr_holor.data[ index ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_alloc_holors_of_type( bmath_hf3_vm_frame_s* o, tp_t type )
{
    for( sz_t i = 0; i < o->arr_holor.size; i++ )
    {
        bmath_hf3_vm_holor_s* holor = &o->arr_holor.data[ i ];
        if( holor->type == type )
        {
            bmath_hf3_s_fit_v_size( &holor->h );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_dealloc_holors_of_type( bmath_hf3_vm_frame_s* o, tp_t type )
{
    for( sz_t i = 0; i < o->arr_holor.size; i++ )
    {
        bmath_hf3_vm_holor_s* holor = &o->arr_holor.data[ i ];
        if( holor->type == type )
        {
            bmath_hf3_s_clear_v_data( &holor->h );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// input/output holors

void bmath_hf3_vm_frame_s_input_push( bmath_hf3_vm_frame_s* o, sz_t idx_val )
{
    bcore_arr_sz_s_push( &o->input, idx_val );
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_input_get_holor( bmath_hf3_vm_frame_s* o, sz_t index )
{
    ASSERT( index >= 0 && index < o->input.size );
    sz_t idx_val = o->input.data[ index ];
    ASSERT( idx_val >= 0 && idx_val < o->arr_holor.size );
    return &o->arr_holor.data[ idx_val ];
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_input_get_paired_holor( bmath_hf3_vm_frame_s* o, sz_t index )
{
    bmath_hf3_vm_holor_s* h = bmath_hf3_vm_frame_s_input_get_holor( o, index );
    if( h->idx_paired < 0 ) return NULL;
    return bmath_hf3_vm_frame_s_holors_get_by_index( o, h->idx_paired );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_output_push( bmath_hf3_vm_frame_s* o, sz_t idx_val )
{
    bcore_arr_sz_s_push( &o->output, idx_val );
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_output_get_holor( bmath_hf3_vm_frame_s* o, sz_t index )
{
    ASSERT( index >= 0 && index < o->output.size );
    sz_t idx_val = o->output.data[ index ];
    ASSERT( idx_val >= 0 && idx_val < o->arr_holor.size );
    return &o->arr_holor.data[ idx_val ];
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_vm_holor_s* bmath_hf3_vm_frame_s_output_get_paired_holor( bmath_hf3_vm_frame_s* o, sz_t index )
{
    bmath_hf3_vm_holor_s* h = bmath_hf3_vm_frame_s_output_get_holor( o, index );
    if( h->idx_paired < 0 ) return NULL;
    return bmath_hf3_vm_frame_s_holors_get_by_index( o, h->idx_paired );
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_s* bmath_hf3_vm_frame_s_input_get( bmath_hf3_vm_frame_s* o, sz_t index )
{
    return &bmath_hf3_vm_frame_s_input_get_holor( o, index )->h;
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_s* bmath_hf3_vm_frame_s_output_get( bmath_hf3_vm_frame_s* o, sz_t index )
{
    return &bmath_hf3_vm_frame_s_output_get_holor( o, index )->h;
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_s* bmath_hf3_vm_frame_s_input_get_paired( bmath_hf3_vm_frame_s* o, sz_t index )
{
    bmath_hf3_vm_holor_s* vmh = bmath_hf3_vm_frame_s_input_get_paired_holor( o, index );
    return vmh ? &vmh->h : NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_hf3_s* bmath_hf3_vm_frame_s_output_get_paired( bmath_hf3_vm_frame_s* o, sz_t index )
{
    bmath_hf3_vm_holor_s* vmh = bmath_hf3_vm_frame_s_output_get_paired_holor( o, index );
    return vmh ? &vmh->h : NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_input_set( bmath_hf3_vm_frame_s* o, sz_t index, bmath_hf3_s* h )
{
    bmath_hf3_s_copy( bmath_hf3_vm_frame_s_input_get( o, index ), h );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_input_set_all( bmath_hf3_vm_frame_s* o, bmath_hf3_adl_s* h )
{
    ASSERT( o->input.size == h->size );
    BFOR_EACH( i, h ) bmath_hf3_vm_frame_s_input_set( o, i, h->data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_output_get_all( bmath_hf3_vm_frame_s* o, bmath_hf3_adl_s* h )
{
    bmath_hf3_adl_s_set_size( h, o->output.size );
    BFOR_EACH( i, h ) bmath_hf3_s_attach( &h->data[ i ], bmath_hf3_s_clone( bmath_hf3_vm_frame_s_output_get( o, i ) ) );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_output_set_paired( bmath_hf3_vm_frame_s* o, sz_t index, bmath_hf3_s* h )
{
    bmath_hf3_s* ph = bmath_hf3_vm_frame_s_output_get_paired( o, index );
    if( ph ) bmath_hf3_s_copy( ph, h );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_vm_frame_s_output_set_paired_all( bmath_hf3_vm_frame_s* o, bmath_hf3_adl_s* h )
{
    ASSERT( o->output.size == h->size );
    BFOR_EACH( i, h ) bmath_hf3_vm_frame_s_output_set_paired( o, i, h->data[ i ] );
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

