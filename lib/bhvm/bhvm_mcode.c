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

#include "bhvm_mcode.h"

#ifdef TYPEOF_bhvm_mcode

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_mcode_track_s_get_index_arr( const bhvm_mcode_track_s* o, bcore_arr_sz_s* index_arr )
{
    BLM_INIT();
    sz_t max_index = -1;
    bcore_arr_sz_s_clear( index_arr );

    BFOR_EACH( i, o )
    {
        const bhvm_vop* vop = o->data[ i ].vop;
        BFOR_SIZE( i, bhvm_vop_a_arity( vop ) + 1 ) max_index = sz_max( max_index, bhvm_vop_a_get_index( vop, i ) );
    }

    if( max_index < 0 ) BLM_RETURN();

    bcore_arr_bl_s* flag_arr = BLM_CREATE( bcore_arr_bl_s );
    bcore_arr_bl_s_fill( flag_arr, max_index + 1, false );

    BFOR_EACH( i, o )
    {
        const bhvm_vop* vop = o->data[ i ].vop;
        BFOR_SIZE( i, bhvm_vop_a_arity( vop ) + 1 ) flag_arr->data[ bhvm_vop_a_get_index( vop, i ) ] = true;
    }

    BFOR_EACH( i, flag_arr ) if( flag_arr->data[ i ] ) bcore_arr_sz_s_push( index_arr, i );

    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

static void track_s_replace_index_via_map( bhvm_mcode_track_s* o, bcore_arr_sz_s* index_map, bl_t include_output )
{
    BFOR_EACH( i, o )
    {
        bhvm_vop* vop = o->data[ i ].vop;
        BFOR_SIZE( i, bhvm_vop_a_arity( vop ) + ( include_output ? 1 : 0 ) )
        {
            sz_t old_index = bhvm_vop_a_get_index( vop, i );
            assert( old_index >= 0 );
            sz_t new_index = ( old_index < index_map->size ) ? index_map->data[ old_index ] : -1;
            if( new_index >= 0 ) bhvm_vop_a_set_index( vop, i, new_index );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_mcode_track_s_replace_index_via_map( bhvm_mcode_track_s* o, bcore_arr_sz_s* index_map )
{
    track_s_replace_index_via_map( o, index_map, true );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_mcode_track_s_cyclic_split_replace( bhvm_mcode_track_s* o, sz_t idx, sz_t new_idx )
{
    BFOR_EACH( i, o )
    {
        bhvm_vop* vop = o->data[ i ].vop;
        sz_t arity = bhvm_vop_a_arity( vop );
        BFOR_SIZE( i, arity ) if( bhvm_vop_a_get_index( vop, i ) == idx ) bhvm_vop_a_set_index( vop, i, new_idx );
        if( bhvm_vop_a_get_index( vop, arity ) == idx ) break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_mcode_track_s_remove_unmapped_output( bhvm_mcode_track_s* o, bcore_arr_sz_s* index_map )
{
    sz_t k = 0;
    BFOR_EACH( i, o )
    {
        bhvm_vop* vop = o->data[ i ].vop;
        sz_t arity = bhvm_vop_a_arity( vop );
        sz_t old_index = bhvm_vop_a_get_index( vop, arity );
        sz_t new_index = index_map->data[ old_index ];
        if( new_index >= 0 )
        {
            if( k < i ) bhvm_mcode_op_s_copy( &o->data[ k ], &o->data[ i ] );
            k++;
        }
    }

    bhvm_mcode_track_s_set_size( o, k );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_mcode_track_s_check_index( const bhvm_mcode_track_s* o, sz_t hbase_size )
{
    BFOR_EACH( i, o )
    {
        const bhvm_vop* vop = o->data[ i ].vop;
        sz_t arity = bhvm_vop_a_arity( vop );
        for( sz_t i = 0; i <= arity; i++ )
        {
            sz_t idx = bhvm_vop_a_get_index( vop, i );
            if( idx <  0 || idx >= hbase_size )
            {
                ERR_fa( "vop '#<sc_t>' index #<sz_t> of value #<sz_t> is out of range [0, #<sz_t>].\n", ifnameof( vop->_ ), i, idx, hbase_size - 1 );
            }
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_mcode_frame_s_mutated( bhvm_mcode_frame_s* o )
{
    /// restore mnode in holors
    BFOR_EACH( i, o->nbase )
    {
        bhvm_mcode_node_s* node = &o->nbase->data[ i ];
        if( node->ax0 >= 0 ) bhvm_mcode_hmeta_a_set_node( bhvm_mcode_hbase_s_get_hmeta( o->hbase, node->ax0 ), node );
        if( node->ax1 >= 0 ) bhvm_mcode_hmeta_a_set_node( bhvm_mcode_hbase_s_get_hmeta( o->hbase, node->ax1 ), node );
        if( node->ag0 >= 0 ) bhvm_mcode_hmeta_a_set_node( bhvm_mcode_hbase_s_get_hmeta( o->hbase, node->ag0 ), node );
        if( node->ag1 >= 0 ) bhvm_mcode_hmeta_a_set_node( bhvm_mcode_hbase_s_get_hmeta( o->hbase, node->ag1 ), node );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_mcode_frame_s_check_integrity( const bhvm_mcode_frame_s* o )
{
    if( !o->lib ) return;
    if( o->lib && !o->hbase ) ERR_fa( "hbase missing" );

    BFOR_EACH( i, &o->lib->arr )
    {
        const bhvm_mcode_track_s* track = o->lib->arr.data[ i ];
        bhvm_mcode_track_s_check_index( track, o->hbase->holor_ads.size );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhvm_mcode

vd_t bhvm_mcode_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_mcode" ) ) )
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

