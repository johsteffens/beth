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
#include "bhvm.xo.h"
#include "bhvm_vop.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm_mcode

XOILA_DEFINE_GROUP( bhvm_mcode, bcore_inst )
#ifdef XOILA_SECTION

// ---------------------------------------------------------------------------------------------------------------------

// microcode operation with preset perspective
stamp :op_s = aware :
{
    aware bhvm_vop => vop;
    private bhvm_vop_spect_s* p; // perspective of vop

    func bcore_inst_call.copy_x  = (verbatim_C) { o->p = o->vop ? ( bhvm_vop_spect_s* )bhvm_vop_spect_s_get_aware( o->vop ) : NULL; };
    func bcore_via_call.mutated = { o.copy_x( NULL ); };
    func bhvm_vop.run = (verbatim_C) { assert( o->p ); assert( o->p->run ); o->p->run( (vc_t)o->vop, ah ); };
};

signature sz_t vop_push_d( m @* o, m bhvm_vop* vop );
signature sz_t vop_push_c( m @* o, c bhvm_vop* vop );

name pclass_ax0; // main axon holor
name pclass_ag0; // main axon gradient
name pclass_ax1; // alternate axon holor
name pclass_ag1; // alternate axon gradient

signature sz_t get_pclass_idx( c @* o, tp_t pclass );

// ---------------------------------------------------------------------------------------------------------------------

/// mcode node (indexing for a given hbase)
stamp :node_s = aware :
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

    func :.get_pclass_idx =
    {
        switch( pclass )
        {
            case TYPEOF_pclass_ax0: return o.ax0;
            case TYPEOF_pclass_ag0: return o.ag0;
            case TYPEOF_pclass_ax1: return o.ax1;
            case TYPEOF_pclass_ag1: return o.ag1;
            default: break;
        }
        return -1;
    };
};

// ---------------------------------------------------------------------------------------------------------------------

signature m :node_s* push_node( m @* o );

stamp :nbase_s = aware x_array
{
    :node_s => [];
    func :.push_node =
    {
        sz_t nidx = o.size;
        :node_s* node = o.push();
        node.nidx = nidx;
        return node;
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/// Holor meta data
group :hmeta =
{
    feature tp_t get_name( c @* o ) = { return 0; };
    feature tp_t get_pclass( c @* o ) = { return 0; };

    feature m ::node_s* get_node( c @* o ) = { return NULL; };
    feature void        set_node( m @* o, m ::node_s* node ) = {};

    feature bl_t is_rollable( c @* o ) = { return false; }; // unrolling: holor need not be duplicated (e.g. const or adaptive)
    feature bl_t is_active( c @* o )   = { return true;  }; // holor is active

    feature m bcore_inst* get_custom( c @* o )                           = { return NULL; }; // retrieves custom data (if available)
    feature m bcore_inst* set_custom( m @* o, const bcore_inst* custom ) = { return NULL; }; // sets custom data and returns custom copy (if supported)

    stamp :adl_s = aware x_array { aware : => []; };
};

// ---------------------------------------------------------------------------------------------------------------------

/// returns index to pushed holor;
signature sz_t push_hm ( m @* o, const bhvm_holor_s* h, const :hmeta* m );
signature sz_t push_hmc( m @* o, const bhvm_holor_s* h, const :hmeta* m, char c, m bhvm_vop_arr_ci_s* arr_ci );
signature sz_t push_copy_from_index( m @* o, sz_t index );

group :hbase =
{
    signature m @* set_size( m @* o, sz_t size );
    signature sz_t get_size( c @* o );

    signature m bhvm_holor_s* get_holor( c @* o, sz_t index );
    signature m ::hmeta*      get_hmeta( c @* o, sz_t index );

    stamp :s = aware :
    {
        bhvm_holor_adl_s holor_adl;
           ::hmeta_adl_s hmeta_adl;

        func :.set_size =
        {
            o.holor_adl.set_size( size );
            o.hmeta_adl.set_size( size );
            return o;
        };

        /// if(copy_size_limit >=0) holors beyond the limit are not copied or streamed
        sz_t copy_size_limit = -1;
        func bcore_via_call.mutated = { if( o.copy_size_limit >= 0 ) o.set_size( o.copy_size_limit ); };
        func bcore_inst_call.copy_x = { if( o.copy_size_limit >= 0 ) o.set_size( o.copy_size_limit ); };

        func  :.get_size = { return o.holor_adl.size; };

        func ::.push_hm  =
        {
            sz_t idx = o.holor_adl.size;
            o.hmeta_adl.push_c( m );
            o.holor_adl.push_c( h );
            return idx;
        };

        func ::.push_hmc  =
        {
            bhvm_vop_ci_s ci;
            bhvm_vop_ci_s_init( ci.1 );
            ci.c = c;
            ci.i = o.push_hm( h, m );
            arr_ci.push_c( ci.1 );
            return ci.i;
        };

        func ::.push_copy_from_index =
        {
            assert( index >= 0 && index < o.holor_adl.size );
            sz_t ret = o.holor_adl.size;
            o.hmeta_adl.push_c( o->hmeta_adl.[ index ] );
            o.holor_adl.push_c( o->holor_adl.[ index ] );
            return ret;
        };

        func :.get_holor = { assert( index >= 0 && index < o.holor_adl.size ); return o.holor_adl.[ index ]; };
        func :.get_hmeta = { assert( index >= 0 && index < o.hmeta_adl.size ); return o.hmeta_adl.[ index ]; };
    };
};

// ---------------------------------------------------------------------------------------------------------------------

signature void run_section( c @* o, sz_t start, sz_t size, m bhvm_holor_s** ah );

stamp :track_s = aware x_array
{
    tp_t name;
    :op_s [];

    func bhvm_vop . run = { for( sz_t i = 0; i < o.size; i++ ) o.[i].run( ah ); };

    func :. run_section =
    {
        assert( start >= 0 && start < o->size - size );
        for( sz_t i = 0; i < size; i++ ) o.[ i + start ].run( ah );
    };

    func :.vop_push_d =
    {
        assert( vop );
        :op_s* op = o.push();
        op.vop = vop;
        op.p = ( bhvm_vop_spect_s* )bhvm_vop_spect_s_get_aware( op->vop );
        assert( op.p );
        return o.size - 1;
    };

    func :.vop_push_c = { return o.vop_push_d( vop.clone() ); };

    func :.push_copy_from_index =
    {
        assert( index >= 0 && index < o->size );
        return o.vop_push_d( o.[ index ].vop.clone() );
    };

    /// fills index_arr with all holor references; each index occurring only once
    func (void get_index_arr( c @* o, m bcore_arr_sz_s* index_arr ));

    /// replaces all indices where index map yields an index >= 0
    func (void replace_index_via_map( m @* o, m bcore_arr_sz_s* index_map ));

    /**
     *  Replaces all matching inputs until (including) cyclic_idx occurs as output.
     *  Subsequent operators remain unchanged.
     */
    func (void cyclic_split_replace( m @* o, sz_t idx, sz_t new_idx ));

    /// remove all operations there the output index is not mapped
    func( void remove_unmapped_output( m @* o, m bcore_arr_sz_s* index_map ));

};

// ---------------------------------------------------------------------------------------------------------------------

stamp :track_adl_s = aware x_array
{
    :track_s => [];
    wrap x_array.push_c;
};

// ---------------------------------------------------------------------------------------------------------------------

// track library functions
signature        void clear             ( m @* o            );
signature        bl_t track_exists      ( c @* o, tp_t name );
signature m :track_s* track_get         ( m @* o, tp_t name ); // retrieves track if existing or returns NULL
signature m :track_s* track_get_or_new  ( m @* o, tp_t name ); // creates track if not yet existing otherwise same as get
signature m :track_s* track_reset       ( m @* o, tp_t name ); // creates track if not yet existing or sets its size to zero
signature        void track_vop_push_d  ( m @* o, tp_t name, m bhvm_vop* vop );
signature        void track_vop_push_c  ( m @* o, tp_t name, c bhvm_vop* vop );
signature        void track_push        ( m @* o, tp_t name, tp_t src_name ); // appends track of src_name
signature        void track_remove      ( m @* o, tp_t name );                // removes track if existing
signature        void track_run_ah      ( c @* o, tp_t name, m bhvm_holor_s** ah );
signature        void track_run         ( c @* o, tp_t name );

signature void track_vop_set_args_push_d( m @* o, tp_t name, m bhvm_vop* vop, c bhvm_vop_arr_ci_s* arr_ci );

// track library
stamp :lib_s = aware :
{
    :track_adl_s      arr;
    bcore_hmap_tpuz_s map; // name-index map

    func :.clear = { o.arr.clear(); o.map.clear(); };
    func :.track_exists = { return o.map.exists( name ); };

    func :.track_get =
    {
        uz_t* pidx = o.map.get( name );
        return pidx ? o.arr.[ pidx.0 ] : NULL;
    };

    func :.track_get_or_new =
    {
        if( o.map.exists( name ) ) return o.track_get( name );
        o.map.set( name, o.arr.size );
        :track_s* track = o.arr.push();
        track.name = name;
        return track;
    };

    func :.track_reset =
    {
        :track_s* track = o.track_get_or_new( name );
        track.clear();
        return track;
    };

    func :.track_vop_push_c = { o.track_get_or_new( name ).vop_push_c( vop ); };
    func :.track_vop_push_d = { o.track_get_or_new( name ).vop_push_d( vop ); };

    func :.track_vop_set_args_push_d =
    {
        vop.set_args( arr_ci );
        o.track_get_or_new( name ).vop_push_d( vop );
    };

    func :.track_push =
    {
        :track_s* src = o.track_get( src_name );
        if( !src ) return;
        :track_s* dst = o.track_get_or_new( name );
        foreach( $* e in src ) dst.vop_push_c( e.vop );
    };

    func :.track_remove =
    {
        uz_t* pidx = o.map.get( name );
        if( !pidx ) return;
        sz_t idx = pidx.0;
        o.arr.[ idx ].discard();
        o.arr.size--;
        o.arr.[ idx ] = o.arr.[ o->arr.size ];
        o.arr.[ o.arr.size ] = NULL;
        if( idx < o.arr.size ) o.map.set( o.arr.[ idx ].name, cast( idx, uz_t ) );
    };

    func :.track_run_ah =
    {
        :track_s* t = cast( o, @* ).track_get( name );
        if( t ) t.run( ah );
    };

};

// ---------------------------------------------------------------------------------------------------------------------

stamp :frame_s = aware :
{
    :lib_s    => lib;
    :hbase_s  => hbase;
    :nbase_s  => nbase;

    func bcore_via_call . mutated;
    func bcore_inst_call . copy_x;

    func :.track_get = { return ( o.lib ) ? o.lib.track_get( name ) : NULL; };

    func :.track_vop_push_c = { if( !o.lib ) o.lib = :lib_s_create(); o.lib.track_vop_push_c( name, vop ); };
    func :.track_vop_push_d = { if( !o.lib ) o.lib = :lib_s_create(); o.lib.track_vop_push_d( name, vop ); };

    func :.track_vop_set_args_push_d = { if( !o.lib ) o.lib = :lib_s_create(); o.lib.track_vop_set_args_push_d( name, vop, arr_ci ); };

    func :.push_hm   = { if( !o->hbase ) o->hbase = :hbase_s_create(); return o.hbase.push_hm(  h, m            ); };
    func :.push_hmc  = { if( !o->hbase ) o->hbase = :hbase_s_create(); return o.hbase.push_hmc( h, m, c, arr_ci ); };
    func :.push_node = { if( !o->nbase ) o->nbase = :nbase_s_create(); return o.nbase.push_node(); };

    func :.track_run = { if( !o->lib ) return; o.lib.track_run_ah( name, o.hbase.holor_adl.data ); };

    func (void check_integrity( c @* o ));
};

// ---------------------------------------------------------------------------------------------------------------------

#endif // XOILA_SECTION

#endif // TYPEOF_bhvm_mcode

/**********************************************************************************************************************/

vd_t bhvm_mcode_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_MCODE_H
