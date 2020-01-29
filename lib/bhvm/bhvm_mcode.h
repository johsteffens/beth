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
#include "bhvm_planted.h"
#include "bhvm_vop.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm_mcode

PLANT_GROUP( bhvm_mcode, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// microcode operation with preset perspective
stamp :op = aware :
{
    aware bhvm_vop => vop;
    private bhvm_vop_s* p; // perspective of vop

    func bcore_inst_call : copy_x  = { o->p = o->vop ? ( bhvm_vop_s* )bhvm_vop_s_get_aware( o->vop ) : NULL; };
    func bcore_via_call  : mutated = { @_copy_x( o ); };
    func bhvm_vop        : run = { assert( o->p ); assert( o->p->run ); o->p->run( (vc_t)o->vop, ah ); };
};

signature void vop_push_d( mutable,       bhvm_vop* vop );
signature void vop_push_c( mutable, const bhvm_vop* vop );

group :hmeta =
{
    // dummy feature to enforce self-awareness testing for meta types
    feature 'a' void do_nothing( const );

    stamp :adl = aware bcore_array { aware : => []; };
};

/// returns index to pushed holor;
signature sz_t push_hm ( mutable, const bhvm_holor_s* h, const :hmeta* m );
signature :push_hm push_hmc( char c, bhvm_vop_arr_ci_s* arr_ci );

group :hbase =
{
    signature @* set_size( mutable, sz_t size );
    signature sz_t get_size( const );

    stamp : = aware :
    {
        bhvm_holor_ads_s holor_ads;
           ::hmeta_adl_s hmeta_adl;

        func  : :set_size =
        {
            bhvm_holor_ads_s_set_size( &o->holor_ads, size  );
            ::hmeta_adl_s_set_size( &o->hmeta_adl, size  );
            return o;
        };

        func  : :get_size = { return o->holor_ads.size; };

        func :: :push_hm  =
        {
            sz_t idx = o->holor_ads.size;
            ::hmeta_adl_s_push_c( &o->hmeta_adl, m );
            bhvm_holor_ads_s_push_c( &o->holor_ads, h );
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
    };
};

stamp :track = aware bcore_array
{
    tp_t name;
    :op_s [];
    func bhvm_vop : run = { BFOR_EACH( i, o ) :op_s_run( &o->data[ i ], ah ); };

    func : :vop_push_d =
    {
        assert( vop );
        :op_s* op = @_push( o );
        op->vop = vop;
        op->p = ( bhvm_vop_s* )bhvm_vop_s_get_aware( op->vop );
        assert( op->p );
    };

    func : :vop_push_c =
    {
        @_vop_push_d( o, bhvm_vop_a_clone( vop ) );
    };
};

stamp :track_ads = aware bcore_array { :track_s => []; };

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
signature      void track_push_reverse( mutable, tp_t name, tp_t src_name ); // appends track of src_name in reverse order
signature      void track_remove      ( mutable, tp_t name );                // removes track if existing
signature      void track_run_ah      (   const, tp_t name, bhvm_holor_s* ah );
signature      void track_run         (   const, tp_t name );

signature :track_vop_push_d track_vop_set_args_push_d( const bhvm_vop_arr_ci_s* arr_ci );

// track library
stamp :lib = aware :
{
    :track_ads_s      arr;
    bcore_hmap_tpuz_s map; // name-index map

    func : :clear = { :track_ads_s_clear( &o->arr ); bcore_hmap_tpuz_s_clear( &o->map ); };

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
        :track_s* track = :track_ads_s_push( &o->arr );
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

    func : :track_push_reverse =
    {
        :track_s* src = @_track_get( o, src_name );
        if( !src ) return;
        :track_s* dst = @_track_get_or_new( o, name );
        for( sz_t i = src->size - 1; i >= 0; i-- ) :track_s_vop_push_c( dst, src->data[ i ].vop );
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
    :lib_s   => lib;
    :hbase_s => hbase;

    func : :track_get = { if( !o->lib ) return NULL; return :lib_s_track_get( o->lib, name ); };

    func : :track_vop_push_c = { if( !o->lib ) o->lib = :lib_s_create(); :lib_s_track_vop_push_c( o->lib, name, vop ); };
    func : :track_vop_push_d = { if( !o->lib ) o->lib = :lib_s_create(); :lib_s_track_vop_push_d( o->lib, name, vop ); };

    func : :track_vop_set_args_push_d = { if( !o->lib ) o->lib = :lib_s_create(); :lib_s_track_vop_set_args_push_d( o->lib, name, vop, arr_ci ); };

    func : :push_hm  = { if( !o->hbase ) o->hbase = :hbase_s_create(); return :hbase_s_push_hm(  o->hbase, h, m            ); };
    func : :push_hmc = { if( !o->hbase ) o->hbase = :hbase_s_create(); return :hbase_s_push_hmc( o->hbase, h, m, c, arr_ci ); };

    func : :track_run = { if( !o->lib ) return; :lib_s_track_run_ah( o->lib, name, o->hbase->holor_ads.data ); };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void bhvm_mcode_frame_s_check_integrity( const bhvm_mcode_frame_s* o );

#endif // TYPEOF_bhvm_mcode

/**********************************************************************************************************************/

vd_t bhvm_mcode_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_MCODE_H
