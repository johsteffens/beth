/** Author and Copyright 2021 Johannes Bernhard Steffens
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

/** Beth stamp-group. Provides access from reflection (using bcore_via perspective). */

#ifndef BCORE_X_STAMP_H
#define BCORE_X_STAMP_H

#include "bcore_txt_ml.h"
#include "bcore_bin_ml.h"
#include "bcore_file.h"
#include "bcore_x_array.h"
#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_stamp, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/
/// Navigating inside a stamp

//----------------------------------------------------------------------------------------------------------------------

/// checks if object is a stamp (meaning that it has a reflection and can be created via x_inst_create)
func bl_t   is_stamp( c aware @* o ) = x_inst_has_reflection( o._ );
func bl_t t_is_stamp( tp_t t )       = x_inst_has_reflection( t );

/// returns number of elements available
func sz_t   size( c aware @* o ) = bcore_via_a_get_size( o.cast( c bcore_via* ) );
func sz_t t_size( tp_t t ) = bcore_via_t_get_size( t, NULL );

/// checks if object is a leaf-type (note that leaf types are usually not self-aware)
func bl_t   is_leaf( c aware @* o ) = bcore_via_a_is_leaf( o );
func bl_t t_is_leaf( tp_t t )       = bcore_via_t_is_leaf( t, NULL );

/// checks if object is aware
func bl_t   is_aware( c aware @* o ) = bcore_via_a_is_aware( o.cast( c bcore_via* ) );
func bl_t t_is_aware( tp_t t )       = bcore_via_t_is_aware( t, NULL );

/// checks if object is an array
func bl_t   is_array( c aware @* o ) = bcore_spect_trait_supported( TYPEOF_bcore_array, o._ );
func bl_t t_is_array( tp_t t )       = bcore_spect_trait_supported( TYPEOF_bcore_array, t );

/// checks if object is an array without additional elements (pure arrays are not leafs)
func bl_t   is_pure_array( c aware @* o ) = bcore_via_a_is_pure_array( o.cast( c bcore_via* ) );
func bl_t t_is_pure_array( tp_t t )       = bcore_via_t_is_pure_array( t, NULL );

/// returns name of indexed element
func tp_t   name( c aware @* o, sz_t index ) = bcore_via_a_iget_name( o.cast( c bcore_via* ), index );
func tp_t t_name( tp_t t,       sz_t index ) = bcore_via_t_iget_name( t, NULL, index );

/// returns index of named element
func tp_t   index( c aware @* o, tp_t name ) = bcore_via_a_nget_index( o.cast( c bcore_via* ), name );
func tp_t t_index( tp_t t,       tp_t name ) = bcore_via_t_nget_index( t, NULL, name );

/// checks if given name exists
func bl_t   exists( c aware @* o, tp_t name ) = bcore_via_a_nexists( o.cast( c bcore_via* ), name );
func bl_t t_exists( tp_t t,       tp_t name ) = bcore_via_t_nexists( t, NULL, name );

/// checks if element is static (type need not be recorded)
func bl_t   is_static_i( aware @* o, sz_t index ) = bcore_via_a_iis_static( o.cast( c bcore_via* ), index );
func bl_t t_is_static_i( tp_t t,     sz_t index ) = bcore_via_t_iis_static( t, NULL, index );
func bl_t   is_static  ( aware @* o, tp_t name )  = bcore_via_a_nis_static( o.cast( c bcore_via* ), name );
func bl_t t_is_static  ( tp_t t,     tp_t name )  = bcore_via_t_nis_static( t, NULL, name );

/// checks if element is a link (means that it can be NULL); (Note: An array is a distinct static object -> not a link)
func bl_t   is_link_i( aware @* o, sz_t index ) = bcore_via_a_iis_link( o.cast( c bcore_via* ), index );
func bl_t t_is_link_i( tp_t t,     sz_t index ) = bcore_via_t_iis_link( t, NULL, index );
func bl_t   is_link  ( aware @* o, tp_t name )  = bcore_via_a_nis_link( o.cast( c bcore_via* ), name );
func bl_t t_is_link  ( tp_t t,     tp_t name )  = bcore_via_t_nis_link( t, NULL, name );

func bl_t   is_static_link_i( aware @* o, sz_t index ) = bcore_via_a_iis_static_link( o.cast( c bcore_via* ), index );
func bl_t t_is_static_link_i( tp_t t,     sz_t index ) = bcore_via_t_iis_static_link( t, NULL, index );
func bl_t   is_static_link  ( aware @* o, tp_t name )  = bcore_via_a_nis_static_link( o.cast( c bcore_via* ), name );
func bl_t t_is_static_link  ( tp_t t,     tp_t name )  = bcore_via_t_nis_static_link( t, NULL, name );

func bl_t   is_typed_link_i( aware @* o, sz_t index ) = bcore_via_a_iis_typed_link( o.cast( c bcore_via* ), index );
func bl_t t_is_typed_link_i( tp_t t,     sz_t index ) = bcore_via_t_iis_typed_link( t, NULL, index );
func bl_t   is_typed_link  ( aware @* o, tp_t name )  = bcore_via_a_nis_typed_link( o.cast( c bcore_via* ), name );
func bl_t t_is_typed_link  ( tp_t t,     tp_t name )  = bcore_via_t_nis_typed_link( t, NULL, name );

func bl_t   is_aware_link_i( aware @* o, sz_t index ) = bcore_via_a_iis_aware_link( o.cast( c bcore_via* ), index );
func bl_t t_is_aware_link_i( tp_t t,     sz_t index ) = bcore_via_t_iis_aware_link( t, NULL, index );
func bl_t   is_aware_link  ( aware @* o, tp_t name )  = bcore_via_a_nis_aware_link( o.cast( c bcore_via* ), name );
func bl_t t_is_aware_link  ( tp_t t,     tp_t name )  = bcore_via_t_nis_aware_link( t, NULL, name );

/// checks if element is an array
func bl_t   is_array_i( aware @* o, sz_t index ) = bcore_via_a_iis_array( o.cast( c bcore_via* ), index );
func bl_t t_is_array_i( tp_t t,     sz_t index ) = bcore_via_t_iis_array( t, NULL, index );
func bl_t   is_array_n( aware @* o, tp_t name )  = bcore_via_a_nis_array( o.cast( c bcore_via* ), name );
func bl_t t_is_array_n( tp_t t,     tp_t name )  = bcore_via_t_nis_array( t, NULL, name );

/// returns type of element (note that the type might be defined at runtime, hence the object o is always required)
func tp_t   type  ( aware @* o,         tp_t name  ) = bcore_via_a_nget_type(    o.cast( c bcore_via* ), name );
func tp_t t_type  ( obliv @* o, tp_t t, tp_t name  ) = bcore_via_t_nget_type( t, o.cast( c bcore_via* ), name );
func tp_t   type_i( aware @* o,         sz_t index ) = bcore_via_a_iget_type(    o.cast( c bcore_via* ), index );
func tp_t t_type_i( obliv @* o, tp_t t, sz_t index ) = bcore_via_t_iget_type( t, o.cast( c bcore_via* ), index );

/// returns sr_NULL in case of no match
func sr_s t_m_get_sr( m obliv @* o,   tp_t t, tp_t name );
func sr_s   m_get_sr( m aware @* o,           tp_t name ) = o.t_m_get_sr( o._, name );
func sr_s t_m_get_sr_i( m obliv @* o, tp_t t, sz_t index );
func sr_s   m_get_sr_i( m aware @* o,         sz_t index ) = o.t_m_get_sr_i( o._, index );

/// returns sr_NULL in case of no match
func sr_s t_c_get_sr  ( c obliv @* o, tp_t t, tp_t name );
func sr_s   c_get_sr  ( c aware @* o,         tp_t name ) = o.t_c_get_sr( o._, name );
func sr_s t_c_get_sr_i( c obliv @* o, tp_t t, sz_t index );
func sr_s   c_get_sr_i( c aware @* o,         sz_t index ) = o.t_c_get_sr_i( o._, index );

/// selects element in o and returns a weak reference to it; returns NULL on no-match and for shells.
func c x_inst* t_c_get(   c obliv @* o, tp_t t, tp_t name ) { sr_s sr = o.t_c_get_sr( t, name ); if( sr.is_strong() ) sr.clear(); = sr.o; }
func m x_inst* t_m_get(   m obliv @* o, tp_t t, tp_t name ) { sr_s sr = o.t_m_get_sr( t, name ); if( sr.is_strong() ) sr.clear(); = sr.o; }
func c x_inst* t_c_get_i( c obliv @* o, tp_t t, sz_t index ) { sr_s sr = o.t_c_get_sr_i( t, index ); if( sr.is_strong() ) sr.clear(); = sr.o; }
func m x_inst* t_m_get_i( m obliv @* o, tp_t t, sz_t index ) { sr_s sr = o.t_m_get_sr_i( t, index ); if( sr.is_strong() ) sr.clear(); = sr.o; }

func c x_inst* c_get(   c aware @* o, tp_t name ) { sr_s sr = o.c_get_sr( name ); if( sr.is_strong() ) sr.clear(); = sr.o; }
func m x_inst* m_get(   m aware @* o, tp_t name ) { sr_s sr = o.m_get_sr( name ); if( sr.is_strong() ) sr.clear(); = sr.o; }
func c x_inst* c_get_i( c aware @* o, sz_t index ) { sr_s sr = o.c_get_sr_i( index ); if( sr.is_strong() ) sr.clear(); = sr.o; }
func m x_inst* m_get_i( m aware @* o, sz_t index ) { sr_s sr = o.m_get_sr_i( index ); if( sr.is_strong() ) sr.clear(); = sr.o; }

func f3_t get_f3( c aware @* o, tp_t name ) { sr_s sr = o.c_get_sr( name ); f3_t v = sr.to_f3(); if( sr.is_strong() ) sr.clear(); = v; }
func s3_t get_s3( c aware @* o, tp_t name ) { sr_s sr = o.c_get_sr( name ); s3_t v = sr.to_f3(); if( sr.is_strong() ) sr.clear(); = v; }
func u3_t get_u3( c aware @* o, tp_t name ) { sr_s sr = o.c_get_sr( name ); u3_t v = sr.to_f3(); if( sr.is_strong() ) sr.clear(); = v; }
func tp_t get_tp( c aware @* o, tp_t name ) { sr_s sr = o.c_get_sr( name ); tp_t v = sr.to_f3(); if( sr.is_strong() ) sr.clear(); = v; }
func bl_t get_bl( c aware @* o, tp_t name ) { sr_s sr = o.c_get_sr( name ); bl_t v = sr.to_f3(); if( sr.is_strong() ) sr.clear(); = v; }

/// sets element in o. (no effect in case of invalid index or name); returns recoverable rror in case of an internal assignment or conversion error
func er_t t_set_sr  ( m obliv @* o, tp_t t, tp_t name, sr_s sr_src );
func er_t   set_sr  ( m aware @* o,         tp_t name, sr_s sr_src ) = o.t_set_sr( o._, name, sr_src );
func er_t t_set_sr_i( m obliv @* o, tp_t t, sz_t index, sr_s sr_src );
func er_t   set_sr_i( m aware @* o,         sz_t index, sr_s sr_src ) = o.t_set_sr_i( o._, index, sr_src );

/// sets element in o and returns a weak smart reference to it in case the element is not a shell; returns sr_null() for invalid index.
func sr_s t_set_sr_ret  ( m obliv @* o, tp_t t, tp_t name, sr_s sr_src );
func sr_s   set_sr_ret  ( m aware @* o,         tp_t name, sr_s sr_src ) = o.t_set_sr_ret( o._, name, sr_src );
func sr_s t_set_sr_ret_i( m obliv @* o, tp_t t, sz_t index, sr_s sr_src );
func sr_s   set_sr_ret_i( m aware @* o,         sz_t index, sr_s sr_src ) = o.t_set_sr_ret_i( o._, index, sr_src );

/// sets element in o and returns a reference to it; returns NULL for shells.
func m x_inst* set_c( m aware @* o, tp_t name, c aware x_inst* src ) = o.set_sr_ret( name, sr_awc( src ) ).o;
func m x_inst* set_d( m aware @* o, tp_t name, d aware x_inst* src ) = o.set_sr_ret( name, sr_asm( src ) ).o;
func m x_inst* set_t_c( m aware @* o, tp_t name, tp_t src_type, c obliv x_inst* src ) = o.set_sr_ret( name, sr_twc( src_type, src ) ).o;
func m x_inst* set_t_d( m aware @* o, tp_t name, tp_t src_type, d obliv x_inst* src ) = o.set_sr_ret( name, sr_tsm( src_type, src ) ).o;

func er_t set_f3( m aware @* o, tp_t name, f3_t  val ) = o.set_sr( name, sr_twc( f3_t~, val.1 ) );
func er_t set_s3( m aware @* o, tp_t name, s3_t  val ) = o.set_sr( name, sr_twc( s3_t~, val.1 ) );
func er_t set_u3( m aware @* o, tp_t name, u3_t  val ) = o.set_sr( name, sr_twc( u3_t~, val.1 ) );
func er_t set_tp( m aware @* o, tp_t name, tp_t  val ) = o.set_sr( name, sr_twc( tp_t~, val.1 ) );
func er_t set_bl( m aware @* o, tp_t name, bl_t  val ) = o.set_sr( name, sr_twc( bl_t~, val.1 ) );
func er_t set_st( m aware @* o, tp_t name, st_s* val ) = o.set_sr( name, sr_twc( st_s~, val.1 ) );

/// tells stamp that it was mutated (stamp must overload bcore_via_call:mutated to receive this signal)
func void t_mutated( m obliv @* o, tp_t t ) if( bcore_via_call_t_defines_mutated( t ) ) o.cast( m bcore_via_call* ).t_mutated( t );
func void   mutated( m aware @* o         ) o.cast( m bcore_via_call* ).mutated();

/// tells stamp that it should 'shelve' itself; see bcore_via_call:shelve for details (stamp must overload bcore_via_call:shelve to receive this signal)
func void t_shelve( m obliv @* o, tp_t t ) if( bcore_via_call_t_defines_shelve( t ) ) o.cast( m bcore_via_call* ).t_shelve( t );
func void   shelve( m aware @* o         ) o.cast( m bcore_via_call* ).shelve();

/// tells stamp about a source that was used for mutation (stamp must overload bcore_via_call:source to receive this signal)
func void t_source( m obliv @* o, tp_t t, m x_source* source ) if( bcore_via_call_t_defines_source( t ) ) o.cast( m bcore_via_call* ).t_source( t, source );
func void   source( m aware @* o        , m x_source* source ) o.cast( m bcore_via_call* ).source( source );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/** Path specifies an array of types addressing a specific sub-element in the object-tree.
 *  The empty path is a valid address referring to the root object.
 */

//----------------------------------------------------------------------------------------------------------------------

name :path_s_array_index;
name sr_s;

stamp :path_s = aware x_inst
{
    tp_t [];

    func o clear( m@* o ) o.cast( m x_array* ).clear();
    func void push(  m@* o, tp_t tp ) o.cast( m x_array* ).push_c( tp.cast( c x_inst* ) );
    func void push_index(  m@* o, sz_t index )
    {
        o.push( TYPEOF_:path_s_array_index );
        o.push( index );
    };

    /// parses format: <name>.<name>. .... or <name>.[<index_literal>]. ...
    func o parse(    m@* o, m x_source* source );
    func o parse_sc( m@* o, sc_t sc ) = o.parse( x_source_create_from_sc( sc )^ );

    func o to_sink( c@* o, m bcore_sink* sink );

    /// checks if given name exists
    func bl_t exists_in  ( c@* o,         c aware x_inst* inst ) = o.exists_in_t( inst._, inst );
    func bl_t exists_in_t( c@* o, tp_t t, c obliv x_inst* inst ) { sr_s^ sr; sr = o.get_sr_in_t( t, inst ); = sr.o != NULL; }

    /// returns type of element; returns 0 if not existing
    func tp_t type_in  ( c@* o,         c aware x_inst* inst ) = o.type_in_t( inst._, inst );
    func tp_t type_in_t( c@* o, tp_t t, c obliv x_inst* inst ) { sr_s^ sr; sr = o.get_sr_in_t( t, inst ); = sr.type(); }

    /// returns sr_null in case path is not found in inst
    func sr_s get_sr_in  ( c@* o,         c aware x_inst* inst ) = o.get_sr_in_t( inst._, inst );
    func sr_s get_sr_in_t( c@* o, tp_t t, c obliv x_inst* inst );

    /// returns NULL if not found or element is a shell
    func c x_inst* c_get_in( c@* o, c obliv x_inst* inst ) { sr_s sr = o.get_sr_in( inst ); if( sr.is_strong() ) sr.clear(); = sr.o.cast( c x_inst* ); }
    func m x_inst* m_get_in( c@* o, m obliv x_inst* inst ) { sr_s sr = o.get_sr_in( inst ); if( sr.is_strong() ) sr.clear(); = sr.o.cast( m x_inst* ); }

    /// sets element in inst; no effect on invalid path.
    func void set_sr_in  ( c@* o,         m aware x_inst* inst, sr_s sr_src ) o.set_sr_in_t( inst._, inst, sr_src );
    func void set_sr_in_t( c@* o, tp_t t, m obliv x_inst* inst, sr_s sr_src ) o.set_sr_ret_in_t( inst._, inst, sr_src );

    /// sets element in inst and returns a smart reference to it in case object is not a shell; returns sr_null() on invalid path.
    func sr_s set_sr_ret_in  ( c@* o,         m aware x_inst* inst, sr_s sr_src ) = o.set_sr_ret_in_t( inst._, inst, sr_src );
    func sr_s set_sr_ret_in_t( c@* o, tp_t t, m obliv x_inst* inst, sr_s sr_src );

    /// sets element in o and returns a reference to it; returns NULL for shells.
    func m x_inst* set_c_in( c@* o, m aware x_inst* inst, c x_inst* src ) = o.set_sr_ret_in( inst, sr_awc( src ) ).o;
    func m x_inst* set_d_in( c@* o, m aware x_inst* inst, d x_inst* src ) = o.set_sr_ret_in( inst, sr_asm( src ) ).o;
}

//----------------------------------------------------------------------------------------------------------------------

stamp :path_adl_s = aware x_array { :path_s => []; }

//----------------------------------------------------------------------------------------------------------------------

func (:path_s) to_sink
{
    for( sz_t i = 0; i < o.size; i++ )
    {
        if( i > 0 ) sink.push_fa( ".", o.[ i ] );
        if( o.[ i ] == TYPEOF_:path_s_array_index )
        {
            i++;
            sink.push_fa( "[#<tp_t>]", o.[ i ] );
        }
        else
        {
            sink.push_fa( "#<sc_t>", bnameof( o.[ i ] ) );
        }
    }
    = o;
}

//----------------------------------------------------------------------------------------------------------------------

func (:path_s) parse
{
    o.clear();
    st_s^ name;

    while( !source.eos() )
    {
        name.clear();
        if( source.parse_bl( " #?'['" ) )
        {
            uz_t index = 0;
            source.parse_fa( " #<uz_t*>", &index );
            source.parse_fa( " ]" );
            o.push_index( index );
        }
        else
        {
            source.parse_fa( " #name", name.1 );
            if( name.size == 0 ) break;
            o.push( btypeof( name.sc ) );
        }
        if( !source.parse_bl( " #?'.'" ) ) break;
    }
    = o;
}

//----------------------------------------------------------------------------------------------------------------------

/// returns sr_null in case path is not found in inst
func (:path_s) get_sr_in_t
{
    sr_s sr0 = sr_twc( t, inst );
    if( !sr0.o ) = sr_null();

    const bcore_via_s* p = NULL;
    sz_t index = -1;

    bl_t found = true;

    for( sz_t i = 0; i < o.size; i++ )
    {
        c bcore_via* via = cast( sr0.o, c bcore_via* );
        p = ( p && index >= 0 ) ? bcore_via_p_iget_via( p, via, index ) : NULL;
        if( !p ) p = bcore_via_s_get_typed( sr_s_type( &sr0 ) );
        if( !p ) { found = false; break; }
        sr_s sr1 = sr_null();

        if( o.[ i ] == TYPEOF_:path_s_array_index )
        {
            if( !bcore_spect_trait_supported( TYPEOF_bcore_array, sr_s_type( &sr0 ) ) ) { found = false; break; }
            const bcore_array_s* p_array = bcore_array_s_get_typed( sr_s_type( &sr0 ) );
            const bcore_array* array = ( const bcore_array* )via;
            i++;
            uz_t arr_index = o.[ i ];
            uz_t arr_size = bcore_array_p_get_size( p_array, array );
            if( arr_index >= arr_size ) { found = false; break; }
            sr1 = bcore_array_p_get( p_array, array, arr_index );
            p = NULL;
        }
        else
        {
            index = bcore_via_p_nget_index( p, via, o.[ i ] );
            if( index < 0 ) { found = false; break; }
            sr1 = bcore_via_p_iget( p, via, index );
        }
        sr_down( sr0 );
        sr0 = sr1;
        if( !sr0.o ) = sr_null();
    }

    if( !found ) sr0.clear();

    = sr0;
}

//----------------------------------------------------------------------------------------------------------------------

/// returns sr_null in case path is not found in inst
func (:path_s) set_sr_ret_in_t
{
    sr_s sr0 = sr_twc( t, inst );

    sr_s^ src_hook;
    src_hook = sr_src;
    sr_src.set_strong( false );

    if( !sr0.o ) = sr_null();

    if( o.size == 0 && sr_src.o ) bcore_inst_x_copy_typed( sr0, sr_src.type(), sr_src.o );

    const bcore_via_s* p = NULL;
    sz_t index = -1;

    bl_t found = true;

    for( sz_t i = 0; i < o.size; i++ )
    {
        m bcore_via* via = cast( sr0.o, m bcore_via* );
        p = ( p && index >= 0 ) ? bcore_via_p_iget_via( p, via, index ) : NULL;
        if( !p ) p = bcore_via_s_get_typed( sr_s_type( &sr0 ) );
        if( !p ) { found = false; break; }

        sr_s sr1 = sr_null();

        if( o.[ i ] == TYPEOF_:path_s_array_index )
        {
            if( !bcore_spect_trait_supported( TYPEOF_bcore_array, sr_s_type( &sr0 ) ) ) { found = false; break; }
            const bcore_array_s* p_array = bcore_array_s_get_typed( sr_s_type( &sr0 ) );
            m bcore_array* array = via;
            i++;
            uz_t arr_index = o.[ i ];
            uz_t arr_size = bcore_array_p_get_size( p_array, array );
            if( arr_index >= arr_size ) { found = false; break; }

            if( i + 1 == o.size ) bcore_array_p_set( p_array, array, arr_index, sr_src );

            sr1 = bcore_array_p_get( p_array, array, arr_index );
            p = NULL;
        }
        else
        {
            index = bcore_via_p_nget_index( p, via, o.[ i ] );
            if( index < 0 ) { found = false; break; }

            if( i + 1 == o.size ) bcore_via_p_iset( p, via, index, sr_src );

            sr1 = bcore_via_p_iget( p, via, index );
        }
        sr_down( sr0 );
        sr0 = sr1;
        if( !sr0.o ) = sr_null();
    }

    if( !found ) sr0.clear();

    = sr0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func :.t_m_get_sr
{
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, o.cast( c bcore_via* ), name );
    if( index < 0 ) = sr_null();
    = bcore_via_p_iget( p, o.cast(c bcore_via* ), index );
}

//----------------------------------------------------------------------------------------------------------------------

func :.t_m_get_sr_i
{
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    if( index < 0 || index >= bcore_via_p_get_size( p, o.cast( bcore_via* ) ) ) = sr_null();
    = bcore_via_p_iget( p, o.cast(c bcore_via* ), index );
}

//----------------------------------------------------------------------------------------------------------------------

/** Sets element in o
 *  No effect in case of invalid address.
 *  Returns recoverable error in case there was an internal conversion or assignment error.
 */
func :.t_set_sr
{
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, o.cast( c bcore_via* ), name );
    if( index < 0 )
    {
        sr_src.down();
        = 0;
    }
    = bcore_via_p_iset( p, o.cast( m bcore_via* ), index, sr_src );
}

//----------------------------------------------------------------------------------------------------------------------

/** Sets element in o and returns a weak smart reference to it;
 *  No effect in case of invalid address.
 *  Returns sr_null() in case of a shell.
 */
func :.t_set_sr_ret
{
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, o.cast( c bcore_via* ), name );
    if( index < 0 )
    {
        sr_src.down();
        = sr_null();
    }
    bcore_via_p_iset( p, o.cast( m bcore_via* ), index, sr_src );
    sr_s sr = bcore_via_p_iget( p, o.cast( c bcore_via* ), index );
    if( sr.is_strong() ) sr.clear();
    = sr;
}

//----------------------------------------------------------------------------------------------------------------------

/** Sets element in o.
 *  No effect in case of invalid address.
 *  Returns recoverable error in case there was an internal conversion or assignment error.
 */
func :.t_set_sr_i
{
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    if( index < 0 || index >= bcore_via_p_get_size( p, o.cast( bcore_via* ) ) )
    {
        sr_src.down();
        = 0;
    }
    = bcore_via_p_iset( p, o.cast( m bcore_via* ), index, sr_src );
}

//----------------------------------------------------------------------------------------------------------------------

/** Sets element in o and returns a weak smart reference to it;
 *  No effect in case of invalid address.
 *  Returns sr_null() in case of a shell.
 */
func :.t_set_sr_ret_i
{
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    if( index < 0 || index >= bcore_via_p_get_size( p, o.cast( bcore_via* ) ) )
    {
        sr_src.down();
        = sr_null();
    }
    bcore_via_p_iset( p, o.cast( m bcore_via* ), index, sr_src );
    sr_s sr = bcore_via_p_iget( p, o.cast( c bcore_via* ), index );
    if( sr.is_strong() ) sr.clear();
    = sr;
}

//----------------------------------------------------------------------------------------------------------------------

/// returns sr_NULL in case of no match
func :.t_c_get_sr
{
    sr_s sr = o.cast( m@* ).t_m_get_sr( t, name );
    if( sr.is_weak() ) sr.set_const( true );
    = sr;
}

//----------------------------------------------------------------------------------------------------------------------

/// returns sr_NULL in case of no match
func :.t_c_get_sr_i
{
    sr_s sr = o.cast( m@* ).t_m_get_sr_i( t, index );
    if( sr.is_weak() ) sr.set_const( true );
    = sr;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func void selftest()
{
    bcore_prsg_lcg_u3_00_s^ lcg;

    ASSERT( lcg.cast( c x_stamp* ).exists( btypeof( "state" ) ) );

    u3_t state = lcg.cast( c x_stamp* ).c_get( btypeof( "state" ) ).cast( c u3_t* );
    ASSERT( state == 16437 );

    x_stamp_set_sr_ret( lcg, btypeof( "state" ), sr_u3( 12345 ) );
    state = x_stamp_c_get( lcg, btypeof( "state" ) ).cast( c u3_t* ).0;
    ASSERT( state == 12345 );

    c x_inst* inst = x_stamp_path_s!^.parse_sc( "state" ).c_get_in( lcg );
    ASSERT( inst );
    ASSERT( inst.cast( c u3_t* ).0 == 12345 );

    m x_inst* zoo = cast( bcore_spect_via_create_zoo( 2 ).o, d x_inst* )^;

    sr_s sr = sr_null();

    sr = x_stamp_path_s!^.parse_sc( "[0].[1].vdata.data2.[1]" ).get_sr_in( zoo );
    ASSERT( sr.type() == TYPEOF_u3_t );
    ASSERT( sr.to_u3() == 0 );

    sr = x_stamp_path_s!^.parse_sc( "[0].[1].vdata.data2.[1]" ).set_sr_ret_in( zoo, sr_u3( 20 ) );
    ASSERT( sr.type() == TYPEOF_u3_t );
    ASSERT( sr.to_u3() == 20 );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_STAMP_H
