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

#ifndef BHVM_MCODE_H
#define BHVM_MCODE_H

/**********************************************************************************************************************/

/** V-Machine Microcode
 */

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_xoila_out.h"
#include "bhvm_vop.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm_mcode

XOILA_DEFINE_GROUP( bhvm_mcode, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// microcode operation with preset perspective
stamp :op = aware :
{
    aware bhvm_vop => vop;
    private bhvm_vop_spect_s* p; // perspective of vop

    func bcore_inst_call : copy_x  = { o->p = o->vop ? ( bhvm_vop_spect_s* )bhvm_vop_spect_s_get_aware( o->vop ) : NULL; };
    func bcore_via_call  : mutated = { @_copy_x( o, NULL ); };
    func bhvm_vop        : run = { assert( o->p ); assert( o->p->run ); o->p->run( (vc_t)o->vop, ah ); };
};

signature sz_t vop_push_d( mutable,       bhvm_vop* vop );
signature sz_t vop_push_c( mutable, const bhvm_vop* vop );

name pclass_ax0; // main axon holor
name pclass_ag0; // main axon gradient
name pclass_ax1; // alternate axon holor
name pclass_ag1; // alternate axon gradient

signature sz_t get_pclass_idx( const, tp_t pclass );

/// mcode node (indexing for a given hbase)
stamp :node = aware :
{
    /// Index into nbase holding this node
    sz_t nidx = -1;

    /** Main axon holor index.
     *  Indicates the location of the axon-holor for the node in the mcode framework.
     *  -1 means: Holor is not specified
     */
    sz_t ax0 = -1;

    /** Main axon-gradient index.
     *  Indicates the location of the gradient holor for the node in the mcode framework.
     *  -1 means: Node has no gradient or gradient is not specified;
     */
    sz_t ag0 = -1;

    /** Auxiliary axon holor index (context dependent)
     *  - Used on cyclic node to resolve update.
     */
    sz_t ax1 = -1;

    /** Auxiliary axon-gradient index (context dependent)
     *  - Used on cyclic nodes to resolve dp track
     */
    sz_t ag1 = -1;

    /// node is a parameter
    bl_t param;

    /// node is adaptive
    bl_t adaptive;

    /// node is cyclic
    bl_t cyclic;

    func : :get_pclass_idx =
    {
        switch( pclass )
        {
            case TYPEOF_pclass_ax0: return o->ax0;
            case TYPEOF_pclass_ag0: return o->ag0;
            case TYPEOF_pclass_ax1: return o->ax1;
            case TYPEOF_pclass_ag1: return o->ag1;
            default: break;
        }
        return -1;
    };
};

signature :node_s* push_node( mutable );

stamp :nbase = aware bcore_array
{
    :node_s => [];
    func : :push_node =
    {
        sz_t nidx = o->size;
        bcore_array_a_push( ( bcore_array* )o, sr_asd( :node_s_create() ) );
        :node_s* node = o->data[ nidx ];
        node->nidx = nidx;
        return node;
    };
};

/// Holor meta data
group :hmeta =
{
    feature 'a' tp_t get_name( const ) = { return 0; };
    feature 'a' tp_t get_pclass( const )  = { return 0; };

    feature 'a' ::node_s* get_node( const ) = { return NULL; };
    feature 'a' void      set_node( mutable, ::node_s* node ) = {};

    feature 'a' bl_t is_rollable( const )  = { return false; }; // unrolling: holor need not be duplicated (e.g. const or adaptive)
    feature 'a' bl_t is_active( const )    = { return true;  }; // holor is active

    feature 'a' bcore_inst* get_custom( const )                             = { return NULL; }; // retrieves custom data (if available)
    feature 'a' bcore_inst* set_custom( mutable, const bcore_inst* custom ) = { return NULL; }; // sets custom data and returns custom copy (if supported)

    stamp :adl = aware bcore_array { aware : => []; };
};

/// returns index to pushed holor;
signature sz_t push_hm ( mutable, const bhvm_holor_s* h, const :hmeta* m );
signature sz_t push_hmc( mutable, const bhvm_holor_s* h, const :hmeta* m, char c, bhvm_vop_arr_ci_s* arr_ci );
signature sz_t push_copy_from_index( mutable, sz_t index );

group :hbase =
{
    signature @* set_size( mutable, sz_t size );
    signature sz_t get_size( const );

    signature  bhvm_holor_s* get_holor( const, sz_t index );
    signature  ::hmeta*      get_hmeta( const, sz_t index );

    stamp : = aware :
    {
        bhvm_holor_adl_s holor_adl;
           ::hmeta_adl_s hmeta_adl;

        func  : :set_size =
        {
            bhvm_holor_adl_s_set_size( &o->holor_adl, size );
               ::hmeta_adl_s_set_size( &o->hmeta_adl, size );
            return o;
        };

        /// if(copy_size_limit >=0) holors beyond the limit are not copied or streamed
        sz_t copy_size_limit = -1;
        func bcore_via_call : mutated = { if( o->copy_size_limit >= 0 ) @_set_size( o, o->copy_size_limit ); };
        func bcore_inst_call : copy_x = { if( o->copy_size_limit >= 0 ) @_set_size( o, o->copy_size_limit ); };

        func  : :get_size = { return o->holor_adl.size; };

        func :: :push_hm  =
        {
            sz_t idx = o->holor_adl.size;
               ::hmeta_adl_s_push_c( &o->hmeta_adl, m );
            bhvm_holor_adl_s_push_c( &o->holor_adl, h );
            return idx;
        };

        func :: :push_hmc  =
        {
            bhvm_vop_ci_s ci;
            bhvm_vop_ci_s_init( &ci );
            ci.c = c;
            ci.i = @_push_hm( o, h, m );
            *bhvm_vop_arr_ci_s_push( arr_ci ) = ci;
            return ci.i;
        };

        func :: :push_copy_from_index =
        {
            assert( index >= 0 && index < o->holor_adl.size );

            sz_t ret = o->holor_adl.size;
               ::hmeta_adl_s_push_c( &o->hmeta_adl, o->hmeta_adl.data[ index ] );
            bhvm_holor_adl_s_push_c( &o->holor_adl, o->holor_adl.data[ index ] );
            return ret;
        };

        func : :get_holor = { assert( index >= 0 && index < o->holor_adl.size ); return o->holor_adl.data[ index ]; };
        func : :get_hmeta = { assert( index >= 0 && index < o->hmeta_adl.size ); return o->hmeta_adl.data[ index ]; };
    };
};

signature void run_section( const, sz_t start, sz_t size, bhvm_holor_s** ah );

stamp :track = aware bcore_array
{
    tp_t name;
    :op_s [];

    func bhvm_vop : run =
    {
        BFOR_EACH( i, o ) :op_s_run( &o->data[ i ], ah );
    };

    func : : run_section =
    {
        assert( start >= 0 && start < o->size - size );
        for( sz_t i = 0; i < size; i++ ) :op_s_run( &o->data[ i + start ], ah );
    };

    func : :vop_push_d =
    {
        assert( vop );
        :op_s* op = @_push( o );
        op->vop = vop;
        op->p = ( bhvm_vop_spect_s* )bhvm_vop_spect_s_get_aware( op->vop );
        assert( op->p );
        return o->size - 1;
    };

    func : :vop_push_c =
    {
        return @_vop_push_d( o, bhvm_vop_a_clone( vop ) );
    };

    func : :push_copy_from_index =
    {
        assert( index >= 0 && index < o->size );
        return @_vop_push_d( o, bhvm_vop_a_clone( o->data[ index ].vop ) );
    };
};

stamp :track_adl = aware bcore_array { :track_s => []; };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// track library functions
signature      void clear             ( mutable            );
signature      bl_t track_exists      (   const, tp_t name );
signature :track_s* track_get         ( mutable, tp_t name ); // retrieves track if existing or returns NULL
signature :track_s* track_get_or_new  ( mutable, tp_t name ); // creates track if not yet existing otherwise same as get
signature :track_s* track_reset       ( mutable, tp_t name ); // creates track if not yet existing or sets its size to zero
signature      void track_vop_push_d  ( mutable, tp_t name,       bhvm_vop* vop );
signature      void track_vop_push_c  ( mutable, tp_t name, const bhvm_vop* vop );
signature      void track_push        ( mutable, tp_t name, tp_t src_name ); // appends track of src_name
signature      void track_remove      ( mutable, tp_t name );                // removes track if existing
signature      void track_run_ah      (   const, tp_t name, bhvm_holor_s** ah );
signature      void track_run         (   const, tp_t name );

signature void track_vop_set_args_push_d( mutable, tp_t name, bhvm_vop* vop, const bhvm_vop_arr_ci_s* arr_ci );

// track library
stamp :lib = aware :
{
    :track_adl_s      arr;
    bcore_hmap_tpuz_s map; // name-index map

    func : :clear = { :track_adl_s_clear( &o->arr ); bcore_hmap_tpuz_s_clear( &o->map ); };

    func : :track_exists = { return bcore_hmap_tpuz_s_exists( &o->map, name ); };

    func : :track_get =
    {
        uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
        if( !pidx ) return NULL;
        return o->arr.data[ *pidx ];
    };

    func : :track_get_or_new =
    {
        if( bcore_hmap_tpuz_s_exists( &o->map, name ) ) return @_track_get( o, name );
        bcore_hmap_tpuz_s_set( &o->map, name, o->arr.size );
        :track_s* track = :track_adl_s_push( &o->arr );
        track->name = name;
        return track;
    };

    func : :track_reset =
    {
        :track_s* track = @_track_get_or_new( o, name );
        :track_s_clear( track );
        return track;
    };

    func : :track_vop_push_c = { :track_s_vop_push_c( @_track_get_or_new( o, name ), vop ); };
    func : :track_vop_push_d = { :track_s_vop_push_d( @_track_get_or_new( o, name ), vop ); };

    func : :track_vop_set_args_push_d =
    {
        bhvm_vop_a_set_args( vop, arr_ci );
        :track_s_vop_push_d( @_track_get_or_new( o, name ), vop );
    };

    func : :track_push =
    {
        :track_s* src = @_track_get( o, src_name );
        if( !src ) return;
        :track_s* dst = @_track_get_or_new( o, name );
        BFOR_EACH( i, src ) :track_s_vop_push_c( dst, src->data[ i ].vop );
    };

    func : :track_remove =
    {
        uz_t* pidx = bcore_hmap_tpuz_s_get( &o->map, name );
        if( !pidx ) return;
        sz_t idx = *pidx;
        :track_s_discard( o->arr.data[ idx ] );
        o->arr.size--;
        o->arr.data[ idx ] = o->arr.data[ o->arr.size ];
        o->arr.data[ o->arr.size ] = NULL;
        if( idx < o->arr.size ) bcore_hmap_tpuz_s_set( &o->map, o->arr.data[ idx ]->name, idx );
    };

    func : :track_run_ah = { :track_s* t = @_track_get( (@*)o, name ); if( t ) :track_s_run( t, ah ); };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :frame = aware :
{
    :lib_s    => lib;
    :hbase_s  => hbase;
    :nbase_s  => nbase;

    func bcore_via_call : mutated;
    func bcore_inst_call : copy_x;

    func : :track_get = { if( !o->lib ) return NULL; return :lib_s_track_get( o->lib, name ); };

    func : :track_vop_push_c = { if( !o->lib ) o->lib = :lib_s_create(); :lib_s_track_vop_push_c( o->lib, name, vop ); };
    func : :track_vop_push_d = { if( !o->lib ) o->lib = :lib_s_create(); :lib_s_track_vop_push_d( o->lib, name, vop ); };

    func : :track_vop_set_args_push_d = { if( !o->lib ) o->lib = :lib_s_create(); :lib_s_track_vop_set_args_push_d( o->lib, name, vop, arr_ci ); };

    func : :push_hm   = { if( !o->hbase ) o->hbase = :hbase_s_create(); return :hbase_s_push_hm(   o->hbase, h, m            ); };
    func : :push_hmc  = { if( !o->hbase ) o->hbase = :hbase_s_create(); return :hbase_s_push_hmc(  o->hbase, h, m, c, arr_ci ); };
    func : :push_node = { if( !o->nbase ) o->nbase = :nbase_s_create(); return :nbase_s_push_node( o->nbase                  ); };

    func : :track_run = { if( !o->lib ) return; :lib_s_track_run_ah( o->lib, name, o->hbase->holor_adl.data ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// fills index_arr with all holor references; each index occurring only once
void bhvm_mcode_track_s_get_index_arr( const bhvm_mcode_track_s* o, bcore_arr_sz_s* index_arr );

/// replaces all indices where index map yields an index >= 0
void bhvm_mcode_track_s_replace_index_via_map( bhvm_mcode_track_s* o, bcore_arr_sz_s* index_map );

/**
 *  Replaces all matching inputs until (including) cyclic_idx occurs as output.
 *  Subsequent operators remain unchanged.
 */
void bhvm_mcode_track_s_cyclic_split_replace( bhvm_mcode_track_s* o, sz_t idx, sz_t new_idx );

/// remove all operations there the output index is not mapped
void bhvm_mcode_track_s_remove_unmapped_output( bhvm_mcode_track_s* o, bcore_arr_sz_s* index_map );

void bhvm_mcode_frame_s_check_integrity( const bhvm_mcode_frame_s* o );

#endif // TYPEOF_bhvm_mcode

/**********************************************************************************************************************/

vd_t bhvm_mcode_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_MCODE_H
