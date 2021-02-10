/** Author and Copyright 2020 Johannes Bernhard Steffens
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

/** Beth via-group. Provides access via reflection. */

#ifndef BCORE_X_VIA_H
#define BCORE_X_VIA_H

#include "bcore_txt_ml.h"
#include "bcore_bin_ml.h"
#include "bcore_file.h"
#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_via, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// returns number of elements available
func (sz_t   size(         c aware @* o )) = { return bcore_via_a_get_size(    o.cast( c bcore_via* ) ); };
func (sz_t t_size( tp_t t, c obliv @* o )) = { return bcore_via_t_get_size( t, o.cast( c bcore_via* ) ); };

/// returns name of indexed element
func (tp_t   name(         c aware @* o, sz_t index )) = { return bcore_via_a_iget_name(    o.cast( c bcore_via* ), index ); };
func (tp_t t_name( tp_t t, c obliv @* o, sz_t index )) = { return bcore_via_t_iget_name( t, o.cast( c bcore_via* ), index ); };

/// checks if given name exists
func (bl_t   exists(         c aware @* o, tp_t name )) = { return bcore_via_a_nexists(    o.cast( c bcore_via* ), name ); };
func (bl_t t_exists( tp_t t, c obliv @* o, tp_t name )) = { return bcore_via_t_nexists( t, o.cast( c bcore_via* ), name ); };

/// returns type of element
func (tp_t   type(         c aware @* o, tp_t name )) = { return bcore_via_a_nget_type(    o.cast( c bcore_via* ), name ); };
func (tp_t t_type( tp_t t, c obliv @* o, tp_t name )) = { return bcore_via_t_nget_type( t, o.cast( c bcore_via* ), name ); };

/// returns sr_NULL in case of no match
func (sr_s   m_get_sr(         m aware @* o, tp_t name )) = { return :t_m_get_sr( o._, o, name ); };
func (sr_s t_m_get_sr( tp_t t, m obliv @* o, tp_t name ));

/// returns sr_NULL in case of no match
func (sr_s   c_get_sr(         c aware @* o, tp_t name )) = { return :t_c_get_sr( o._, o, name ); };
func (sr_s t_c_get_sr( tp_t t, c obliv @* o, tp_t name ));

/// selects element in o and returns a reference to it; returns NULL on no-match and for shells.
func (c x_inst* c_get( c aware @* o, tp_t name )) = { sr_s sr = o.c_get_sr( name ); if( sr.is_strong() ) sr.clear(); return sr.o; };

/// selects element in o and returns a reference to it; returns NULL on no-match and for shells.
func (m x_inst* m_get( m aware @* o, tp_t name )) = { sr_s sr = o.m_get_sr( name ); if( sr.is_strong() ) sr.clear(); return sr.o; };

/// sets element in o and returns a smart reference to it; returns sr_null() for shells.
func (sr_s   set_sr(         m aware @* o, tp_t name, sr_s sr_src )) = { return :t_set_sr( o._, o, name, sr_src ); };
func (sr_s t_set_sr( tp_t t, m obliv @* o, tp_t name, sr_s sr_src ));

/// sets element in o and returns a reference to it; returns NULL for shells.
func (m x_inst* set_c( m aware @* o, tp_t name, c x_inst* src )) = { return o.set_sr( name, sr_awc( src ) ).o; };

/// sets element in o and returns a reference to it; returns NULL for shells.
func (m x_inst* set_d( m aware @* o, tp_t name, d x_inst* src )) = { return o.set_sr( name, sr_asd( src ) ).o; };

//----------------------------------------------------------------------------------------------------------------------

name :path_s_array_index;
name sr_s;

stamp :path_s = aware x_inst
{
    tp_t [];

    func (   o clear( m@* o )) = { o.cast( m x_array* ).clear(); return o; };
    func (void push(  m@* o, tp_t tp )) = { o.cast( m x_array* ).push_c( tp.cast( c x_inst* ) ); };
    func (void push_index(  m@* o, sz_t index )) =
    {
        o.push( TYPEOF_:path_s_array_index );
        o.push( index );
    };

    /// parses format: <name>.<name>. ....
    /// parses format: <name>.[<index_literal>]. ...
    func ( o parse(    m@* o, m bcore_source* source ));
    func ( o parse_sc( m@* o, sc_t sc )) = { return o.parse( bcore_source_string_s_create_sc( sc )^ ); };

    /// returns sr_null in case path is not found in inst
    func (sr_s get_sr_in  ( c@* o,         c aware x_inst* inst )) = { return o.get_sr_in_t( inst._, inst ); };
    func (sr_s get_sr_in_t( c@* o, tp_t t, c obliv x_inst* inst ));

    func (bl_t exists_in  ( c@* o,         c aware x_inst* inst )) = { return o.exists_in_t( inst._, inst ); };
    func (bl_t exists_in_t( c@* o, tp_t t, c obliv x_inst* inst )) = { sr_s^ sr; return ( sr = o.get_sr_in_t( t, inst ) ).o != NULL; };

    /// returns NULL if not found or element is a shell
    func (c x_inst* c_get_in( c@* o, c obliv x_inst* inst )) = { sr_s sr = o.get_sr_in( inst ); if( sr.is_strong() ) sr.clear(); return sr.o.cast( c x_inst* ); };
    func (m x_inst* m_get_in( c@* o, m obliv x_inst* inst )) = { sr_s sr = o.get_sr_in( inst ); if( sr.is_strong() ) sr.clear(); return sr.o.cast( m x_inst* ); };
};

//----------------------------------------------------------------------------------------------------------------------

func (:path_s) parse =
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

    return o;
};

//----------------------------------------------------------------------------------------------------------------------

/// returns sr_null in case path is not found in inst
func (:path_s) get_sr_in_t =
{
    sr_s sr0 = sr_twc( t, inst );
    if( !sr0.o ) return sr_null();

    const bcore_via_s* p = NULL;
    sz_t index = -1;

    bl_t found = true;

    for( sz_t i = 0; i < o.size; i++ )
    {
        c bcore_via* via = cast( sr0.o, c bcore_via* );
        p = p ? bcore_via_p_iget_via( p, via, index ) : bcore_via_s_get_typed( sr_s_type( &sr0 ) );
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
        if( !sr0.o ) return sr_null();
    }

    if( !found )
    {
        sr_down( sr0 );
        sr0 = sr_null();
    }

    return sr0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func :.t_m_get_sr =
{
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, o.cast( c bcore_via* ), name );
    if( index < 0 ) return sr_null();
    return bcore_via_p_iget( p, o.cast(c bcore_via* ), index );
};

//----------------------------------------------------------------------------------------------------------------------

/// sets element in o and returns a smart reference to it; returns sr_null() for shells.
func :.t_set_sr =
{
    const bcore_via_s* p = bcore_via_s_get_typed( t );
    sz_t index = bcore_via_p_nget_index( p, o.cast( c bcore_via* ), name );
    if( index < 0 ) ERR_fa( "'#<sc_t>' has no element of name '#<sc_t>'", bifnameof( p->header.o_type ), bifnameof( name ) );
    bcore_via_p_iset( p, o.cast( m bcore_via* ), index, sr_src );
    sr_s sr = bcore_via_p_iget( p, o.cast(c bcore_via* ), index );
    if( sr.is_strong() ) sr.clear();
    return sr;
};

//----------------------------------------------------------------------------------------------------------------------

/// returns sr_NULL in case of no match
func :.t_c_get_sr =
{
    sr_s sr = :t_m_get_sr( t, o.cast( m@* ), name );
    if( sr.is_weak() ) sr.set_const( true );
    return sr;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (void selftest() ) =
{
    bcore_prsg_lcg_u3_00_s^ lcg;

    ASSERT( lcg.cast( c x_via* ).exists( btypeof( "state" ) ) );

    u3_t state = lcg.cast( c x_via* ).c_get( btypeof( "state" ) ).cast( c u3_t* );
    ASSERT( state == 16437 );

    x_via_set_sr( lcg, btypeof( "state" ), sr_u3( 12345 ) );
    state = x_via_c_get( lcg, btypeof( "state" ) ).cast( c u3_t* ).0;
    ASSERT( state == 12345 );

    c x_inst* inst = x_via_path_s!^.parse_sc( "state" ).c_get_in( lcg );
    ASSERT( inst );
    ASSERT( inst.cast( c u3_t* ).0 == 12345 );

    c x_inst* zoo = cast( bcore_spect_via_create_zoo( 2 ).o, d x_inst* )^;

    //zoo.to_sink_txt_ml( x_inst_stdout() );

    //x_via_path_s!^.parse_sc( "[0]" ).c_get_in( zoo )?.to_sink_txt_ml( x_inst_stdout() );

    sr_s sr = x_via_path_s!^.parse_sc( "[0].[1].vdata.data2.[0]" ).get_sr_in_t( zoo._, zoo );

    if( sr.o ) bcore_txt_ml_x_to_sink( sr, x_inst_stdout() );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_VIA_H