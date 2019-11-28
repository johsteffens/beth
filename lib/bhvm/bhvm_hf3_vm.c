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

#include "bhvm_hf3_vm.h"

#ifdef TYPEOF_bhvm_hf3_vm

/**********************************************************************************************************************/
// vm_op

void bhvm_hf3_vm_op_set_args( bhvm_hf3_vm_op* o, sc_t arr_sig, const bcore_arr_sz_s* arr_idx )
{
    for( sz_t i = 0; i < arr_idx->size; i++ )
    {
        if( !arr_sig ) break;
        if( arr_sig[ i ] == 0 ) break;
        bhvm_hf3_vm_op_a_set_arg( o, arr_sig[ i ], arr_idx->data[ i ] );
    }
}

/**********************************************************************************************************************/
/// op_s

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_prop_s_setup_d( bhvm_hf3_vm_prop_s* o, bhvm_hf3_vm_op* op )
{
    if( op )
    {
        bcore_inst_a_attach( (bcore_inst**)&o->op, (bcore_inst*)op );
        o->p = (bhvm_hf3_vm_s*)bhvm_hf3_vm_s_get_aware( o->op );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// proc_s

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_proc_s_push_op_d( bhvm_hf3_vm_proc_s* o, bhvm_hf3_vm_op* op )
{
    if( op )
    {
        ASSERT( *(aware_t*)op != TYPEOF_bhvm_hf3_vm_prop_s );
        bhvm_hf3_vm_prop_s* vm_op = bhvm_hf3_vm_proc_s_push( o );
        bhvm_hf3_vm_prop_s_setup_d( vm_op, op );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// frame_s

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_check_integrity( bhvm_hf3_vm_frame_s* o )
{
    for( sz_t i = 0; i < o->arr_holor.size; i++ )
    {
        bhvm_hf3_s_check_integrity( &o->arr_holor.data[ i ].h );
    }

    for( sz_t i = 0; i < o->lib_mcode.arr.size; i++ )
    {
        bhvm_hf3_vm_mcode_s* mcode = o->lib_mcode.arr.data[ i ];
        for( sz_t j = 0; j < mcode->size; j++ )
        {
            bhvm_hf3_vm_mop_s* mop = &mcode->data[ j ];
            ASSERT( mop->op );
            sz_t arity = bhvm_hf3_vm_op_a_get_arity( mop->op );
            sz_t* a = bcore_u_alloc( sizeof( sz_t ), NULL, arity + 1, NULL );
            bhvm_hf3_vm_op_a_get_indices( mop->op, a );
            for( sz_t k = 0; k <= arity; k++ )
            {
                sz_t idx = a[ k ];
                if( idx < 0 || idx >= o->arr_holor.size )
                {
                    ERR_fa
                    (
                        "In procedure #<sz_t>, operation #<sz_t>:\n"
                        "Index #<sz_t> of operator #<sc_t> has value #<sz_t>, which is out of range.",
                        i, j, k, ifnameof( mop->op->_ ), idx
                    );
                }
            }
            bcore_free( a );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

tp_t bhvm_hf3_vm_frame_s_entypeof( bhvm_hf3_vm_frame_s* o, sc_t name )
{
    return bcore_hmap_name_s_set_sc( &o->map_name, name );
}

// ---------------------------------------------------------------------------------------------------------------------

sc_t bhvm_hf3_vm_frame_s_nameof( const bhvm_hf3_vm_frame_s* o, tp_t type )
{
    return bcore_hmap_name_s_get_sc( &o->map_name, type );
}

// ---------------------------------------------------------------------------------------------------------------------

sc_t bhvm_hf3_vm_frame_s_ifnameof( const bhvm_hf3_vm_frame_s* o, tp_t type )
{
    sc_t name = bcore_hmap_name_s_get_sc( &o->map_name, type );
    return name ? name : "";
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_setup( bhvm_hf3_vm_frame_s* o )
{
    if( !o->mcode_setup ) return;
    if( !bhvm_hf3_vm_frame_s_mcode_exists( o, o->mcode_setup ) ) return;
    bhvm_hf3_vm_frame_s_mcode_run( o, o->mcode_setup );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_shelve( bhvm_hf3_vm_frame_s* o )
{
    if( !o->mcode_shelve ) return;
    if( !bhvm_hf3_vm_frame_s_mcode_exists( o, o->mcode_shelve ) ) return;
    bhvm_hf3_vm_frame_s_mcode_run( o, o->mcode_shelve );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_clear( bhvm_hf3_vm_frame_s* o )
{
    bhvm_hf3_vm_arr_holor_s_clear( &o->arr_holor );
    bhvm_hf3_vm_lib_mcode_s_clear( &o->lib_mcode );
    bcore_hmap_name_s_clear( &o->map_name );
    o->mcode_setup = 0;
    o->mcode_shelve = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

// Microcode

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_mcode_to_sink( const bhvm_hf3_vm_frame_s* o, tp_t name, bcore_sink* sink )
{
    if( !bhvm_hf3_vm_frame_s_mcode_exists( o, name ) ) return;
    bhvm_hf3_vm_mcode_s* vm_mcode = bhvm_hf3_vm_frame_s_mcode_get( ( bhvm_hf3_vm_frame_s* )o, name );

    BLM_INIT();
    bcore_arr_sz_s* arr_sz = BLM_CREATE( bcore_arr_sz_s );
    st_s* st1 = BLM_CREATE( st_s );
    st_s* st2 = BLM_CREATE( st_s );

    bcore_sink_a_push_fa( sink, "MCODE #<sc_t>:\n", bhvm_hf3_vm_frame_s_ifnameof( o, name ) );

    BFOR_EACH( i, vm_mcode )
    {
        bhvm_hf3_vm_mop_s* mop = &vm_mcode->data[ i ];
        assert( mop->op );
        const bhvm_hf3_vm_op* op = ( bhvm_hf3_vm_op* )mop->op;
        bcore_arr_sz_s_set_size( arr_sz, bhvm_hf3_vm_op_a_get_arity( op ) + 1 );
        bhvm_hf3_vm_op_a_get_indices( op, arr_sz->data );

        st_s_copy_sc( st1, ifnameof( op->_ ) );
        st_s_replace_sc_sc( st1, "bhvm_hf3_vm_op_", "" );
        if( st1->size >= 2 && sc_t_equ( st1->sc + st1->size - 2, "_s" ) ) st_s_pop_n( st1, 2 );

        bcore_sink_a_push_fa( sink, "#p32'.'{#<sc_t> } ", st1->sc );
        BFOR_EACH( i, arr_sz )
        {
            st_s_clear( st2 );
            sz_t index = arr_sz->data[ i ];
            bhvm_hf3_vm_holor_s* holor = &o->arr_holor.data[ index ];
            sc_t sc_name = bhvm_hf3_vm_frame_s_nameof( o, holor->name );
            if( sc_name ) st_s_push_fa( st2, "(#<sc_t>)", sc_name );
            st_s_push_fa( st2, "#<sz_t>", index );
            bcore_sink_a_push_fa( sink, " #pl8' '{#<sc_t>}", st2->sc );
        }
        bcore_sink_a_push_fa( sink, "\n" );
    }

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_mcode_to_stdout( const bhvm_hf3_vm_frame_s* o, tp_t name )
{
    bhvm_hf3_vm_frame_s_mcode_to_sink( o, name, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

// Holors

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_holors_set_size( bhvm_hf3_vm_frame_s* o, sz_t size )
{
    bhvm_hf3_vm_arr_holor_s_set_size( &o->arr_holor, size );
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_holors_push( bhvm_hf3_vm_frame_s* o )
{
    return bhvm_hf3_vm_arr_holor_s_push( &o->arr_holor );
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_holors_get_by_index( bhvm_hf3_vm_frame_s* o, sz_t index )
{
    ASSERT( index >= 0 && index < o->arr_holor.size );
    return &o->arr_holor.data[ index ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_alloc_holors_of_type( bhvm_hf3_vm_frame_s* o, tp_t type )
{
    for( sz_t i = 0; i < o->arr_holor.size; i++ )
    {
        bhvm_hf3_vm_holor_s* holor = &o->arr_holor.data[ i ];
        if( holor->type == type )
        {
            bhvm_hf3_s_fit_v_size( &holor->h );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_dealloc_holors_of_type( bhvm_hf3_vm_frame_s* o, tp_t type )
{
    for( sz_t i = 0; i < o->arr_holor.size; i++ )
    {
        bhvm_hf3_vm_holor_s* holor = &o->arr_holor.data[ i ];
        if( holor->type == type )
        {
            bhvm_hf3_s_clear_v_data( &holor->h );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// input/output holors

void bhvm_hf3_vm_frame_s_input_push( bhvm_hf3_vm_frame_s* o, sz_t idx_val )
{
    bcore_arr_sz_s_push( &o->input, idx_val );
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_input_get_holor( bhvm_hf3_vm_frame_s* o, sz_t index )
{
    ASSERT( index >= 0 && index < o->input.size );
    sz_t idx_val = o->input.data[ index ];
    ASSERT( idx_val >= 0 && idx_val < o->arr_holor.size );
    return &o->arr_holor.data[ idx_val ];
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_input_get_paired_holor( bhvm_hf3_vm_frame_s* o, sz_t index )
{
    bhvm_hf3_vm_holor_s* h = bhvm_hf3_vm_frame_s_input_get_holor( o, index );
    if( h->idx_paired < 0 ) return NULL;
    return bhvm_hf3_vm_frame_s_holors_get_by_index( o, h->idx_paired );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_output_push( bhvm_hf3_vm_frame_s* o, sz_t idx_val )
{
    bcore_arr_sz_s_push( &o->output, idx_val );
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_output_get_holor( bhvm_hf3_vm_frame_s* o, sz_t index )
{
    ASSERT( index >= 0 && index < o->output.size );
    sz_t idx_val = o->output.data[ index ];
    ASSERT( idx_val >= 0 && idx_val < o->arr_holor.size );
    return &o->arr_holor.data[ idx_val ];
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_hf3_vm_holor_s* bhvm_hf3_vm_frame_s_output_get_paired_holor( bhvm_hf3_vm_frame_s* o, sz_t index )
{
    bhvm_hf3_vm_holor_s* h = bhvm_hf3_vm_frame_s_output_get_holor( o, index );
    if( h->idx_paired < 0 ) return NULL;
    return bhvm_hf3_vm_frame_s_holors_get_by_index( o, h->idx_paired );
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_hf3_s* bhvm_hf3_vm_frame_s_input_get( bhvm_hf3_vm_frame_s* o, sz_t index )
{
    return &bhvm_hf3_vm_frame_s_input_get_holor( o, index )->h;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_hf3_s* bhvm_hf3_vm_frame_s_output_get( bhvm_hf3_vm_frame_s* o, sz_t index )
{
    return &bhvm_hf3_vm_frame_s_output_get_holor( o, index )->h;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_hf3_s* bhvm_hf3_vm_frame_s_input_get_paired( bhvm_hf3_vm_frame_s* o, sz_t index )
{
    bhvm_hf3_vm_holor_s* vmh = bhvm_hf3_vm_frame_s_input_get_paired_holor( o, index );
    return vmh ? &vmh->h : NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

bhvm_hf3_s* bhvm_hf3_vm_frame_s_output_get_paired( bhvm_hf3_vm_frame_s* o, sz_t index )
{
    bhvm_hf3_vm_holor_s* vmh = bhvm_hf3_vm_frame_s_output_get_paired_holor( o, index );
    return vmh ? &vmh->h : NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_input_set( bhvm_hf3_vm_frame_s* o, sz_t index, bhvm_hf3_s* h )
{
    bhvm_hf3_s_copy( bhvm_hf3_vm_frame_s_input_get( o, index ), h );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_input_set_all( bhvm_hf3_vm_frame_s* o, bhvm_hf3_adl_s* h )
{
    ASSERT( o->input.size == h->size );
    BFOR_EACH( i, h ) bhvm_hf3_vm_frame_s_input_set( o, i, h->data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_output_get_all( bhvm_hf3_vm_frame_s* o, bhvm_hf3_adl_s* h )
{
    bhvm_hf3_adl_s_set_size( h, o->output.size );
    BFOR_EACH( i, h ) bhvm_hf3_s_attach( &h->data[ i ], bhvm_hf3_s_clone( bhvm_hf3_vm_frame_s_output_get( o, i ) ) );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_output_set_paired( bhvm_hf3_vm_frame_s* o, sz_t index, bhvm_hf3_s* h )
{
    bhvm_hf3_s* ph = bhvm_hf3_vm_frame_s_output_get_paired( o, index );
    if( ph ) bhvm_hf3_s_copy( ph, h );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_hf3_vm_frame_s_output_set_paired_all( bhvm_hf3_vm_frame_s* o, bhvm_hf3_adl_s* h )
{
    ASSERT( o->output.size == h->size );
    BFOR_EACH( i, h ) bhvm_hf3_vm_frame_s_output_set_paired( o, i, h->data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhvm_hf3_vm

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bhvm_hf3_vm_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_hf3_vm" ) ) )
    {
        case TYPEOF_init1:
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
            bcore_plant_compile( "bhvm_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

