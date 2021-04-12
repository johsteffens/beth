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

/**

BTML: Beth text markup language .

BTML has a generic syntax for stamps.
Syntax
<type>
   member1: object-data
   member2: object-data
   ...
</>

Example:

stamp myobject =
{
    st_s string;
    tp_t id;
};

... translates to ...

<myobject>
    string:<st_s>"Some string text"</>
    id:<tp_t>1234</>
</>

Version handling without version declaration:

New elements:
An incomplete set of members is handled by setting unspecified elements to their respective default.

Retired elements:
An unrecognized element name is ignored by the parser. (The object of that element might be temporarily created and parsed)

*/

#ifndef BCORE_BTML_H
#define BCORE_BTML_H

#include "bcore_sr.h"
#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_btml, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/
/// interface

//----------------------------------------------------------------------------------------------------------------------

/** Reads object from source.
 *  If o implements copy_typed, automatic type conversion is used.
 *  Allows recovering from parse errors and conversion errors.
 *  Checks error-stack after copy_typed.
 *  In case of a parse error o is not being changed.
 *
 *  Supports 'brief' <#file> inclusion syntax:
 *     Example: <#file> "data/my_file.txt" </>
 *     The file contains the entire object: <type> <body> </>
 *     The inclusion can be used at any place where an object is expected
 */
func (er_t t_from_source( m@* o, tp_t t, m bcore_source* source ));
func (er_t   from_source( m@* o,         m bcore_source* source )) = { return o.t_from_source( o._, source ); };
func (er_t t_from_file  ( m@* o, tp_t t,   sc_t file )) = { return o.t_from_source( o._, bcore_file_open_source( file )^ ); };
func (er_t   from_file  ( m@* o,           sc_t file )) = { return o.t_from_file( o._, file ); };
func (er_t t_from_st    ( m@* o, tp_t t, c st_s* st  )) = { return o.t_from_source( o._, bcore_source_string_s_create_from_string( st )^ ); };
func (er_t   from_st    ( m@* o,         c st_s* st  )) = { return o.t_from_st( o._, st ); };
func (er_t t_from_sc    ( m@* o, tp_t t,   sc_t  sc  )) = { return o.t_from_source( o._, bcore_source_string_s_create_from_sc( sc )^ ); };
func (er_t   from_sc    ( m@* o,           sc_t  sc  )) = { return o.t_from_sc( o._, sc ); };

/** Reads and creates object from source.
 *  Returns NULL in case of parse error (check error-stack).
 */
func (d @* create_from_source( m bcore_source* source ));
func (d @* create_from_st( c st_s* st )) = { return :create_from_source( bcore_source_string_s_create_from_string( st )^ ); };
func (d @* create_from_sc(   sc_t  sc )) = { return :create_from_source( bcore_source_string_s_create_from_sc( sc )^ ); };

/// Tests initial source content for validity. Restores index.
func (bl_t appears_valid( m bcore_source* source ));

/// Writes object to sink.
func (void t_to_sink( c@* o, tp_t t, m bcore_sink* sink ));
func (void   to_sink( c@* o,         m bcore_sink* sink )) = { o.t_to_sink( o._, sink ); };
func (void t_to_file( c@* o, tp_t t, sc_t file )) = { o.t_to_sink( o._, bcore_file_open_sink( file )^ ); };
func (void   to_file( c@* o,         sc_t file )) = { o.t_to_file( o._, file ); };

/// runs a transfer-test (write-read-compare) on given object; in case of a mismatch a descriptive error is created.
func (void t_test_transfer( @* o, tp_t t ));
func (void   test_transfer( @* o )) = { o.t_test_transfer( o._ ); };

/// Overload these features for objects that define their own txt_ml syntax
feature 'at' er_t body_from_source( m@* o, m bcore_source* source );
feature 'at' void body_to_sink(     c@* o, m bcore_sink* sink );

//----------------------------------------------------------------------------------------------------------------------

func t_from_source = (try)
{
    sr_s sr = sr_null();
    :parse_create_object( source, sr.1 );
    x_inst_t_copy_typed( o, t, sr_s_o_type( sr.1 ), sr.o );
    sr_s_down( sr );
    return bcore_error_last();
};

//----------------------------------------------------------------------------------------------------------------------

func create_from_source =
{
    sr_s sr = sr_null();
    :parse_create_object( source, sr.1 );
    return sr.o.cast( d @* ); // sr.o is NULL in case of error
};

//----------------------------------------------------------------------------------------------------------------------

func t_to_sink =
{
    o.t_translate_recursive( o._, 0, true, sink, 0 );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (sc_t name_of( tp_t type, m st_s* buf )) =
{
    sc_t n = bcore_name_try_name( type );
    if( n ) return n;
    buf.copy_typed( tp_t~, type.1 );
    return buf.sc;
};

// ---------------------------------------------------------------------------------------------------------------------

func (tp_t type_of( const st_s* name )) =
{
    tp_t tp = 0;
    if( name.size == 0 )
    {
        return 0;
    }
    else if( name.[ 0 ] >= '0' && name.[ 0 ] <= '9' )
    {
        name.parse_fa( 0, -1, "#tp_t", &tp );
    }
    else
    {
        tp = btypeof( name.sc );
    }
    return tp;
};

//----------------------------------------------------------------------------------------------------------------------

func appears_valid =
{
    bl_t valid = false;
    sz_t index = source.get_index();
    m st_s* type_string = st_s!^;
    if( source.parse_bl( " #?'<'" ) ) // type specifier
    {
        if( source.parse_em_fa( "#until'>'>", type_string ) )
        {
            bcore_error_remove_last();
        }
        else
        {
            tp_t type = :type_of( type_string );
            if( bcore_flect_exists( type ) )      valid = true;
            else if( type == btypeof( "#file" ) ) valid = true;
        }
    }

    source.set_index( index );
    return valid;
};

//----------------------------------------------------------------------------------------------------------------------

/** On entering obj should be sr_null
 *  In case of error obj need not be discarded
 */
func (er_t parse_create_object( m bcore_source* source, m sr_s* obj )) = (try)
{
    er_t er = 0;
    m st_s* type_string = st_s!^;
    source.parse_em_fa( " " ); // take whitespaces

    if( source.parse_bl( "#?'<'" ) ) // type specifier
    {
        source.parse_em_fa( "#until'>'>", type_string );
        tp_t type = :type_of( type_string );
        if( type )
        {
            if( bcore_flect_exists( type ) )
            {
                if( source.parse_bl( " #?'NULL'" ) ) // no instance
                {
                    source.parse_em_fa( " </>" );
                    obj.p = bcore_inst_s_get_typed( type );
                }
                else
                {
                    m x_inst* inst = x_inst_t_create( type )^;
                    :t_parse_body( inst, type, source );
                    obj.0 = sr_tsd( type, inst.fork() );
                }
            }
            else if( type == btypeof( "#file" ) )
            {
                m st_s* file = st_s!^;
                source.parse_em_fa( " #string </>", file );

                if( file.[ 0 ] != '/' ) // make path relative to current file path
                {
                    m st_s* cur_file = st_s_create_sc( source.get_file() )^;
                    if( cur_file.size > 0 )
                    {
                        sz_t idx = cur_file.find_char( cur_file.size, 0, '/' );
                        if( idx < cur_file.size )
                        {
                            cur_file.[ idx ] = 0;
                            file.copy( st_s_create_fa( "#<sc_t>/#<sc_t>", cur_file.sc, file.sc )^ );
                        }
                    }
                }

                if( bcore_file_exists( file.sc ) )
                {
                    :parse_create_object( bcore_file_open_source( file.sc )^, obj );
                }
                else
                {
                    return source.parse_error_fa( "File '#<sc_t>' not found.", file.sc );
                }
            }
            else
            {
                return source.parse_error_fa( "Type '#<sc_t>' has no reflection.", type_string.sc );
            }
        }
        else
        {
            source.parse_em_fa( " </>" );
        }
    }

    // simplified encoding for certain leaf types ...

    // string
    else if( source.parse_bl( "#=?'\"'" ) )
    {
        m st_s* st = st_s!^;
        source.parse_em_fa( " #string", st );
        obj.0 = sr_tsd( st_s~, st.fork() );
    }
    // number
    else if( source.parse_bl( "#?(([0]>='0'&&[0]<='9')||[0]=='+'||[0]=='-')" ) )
    {
        f3_t v;
        source.parse_em_fa( "#<f3_t*>", &v );
        obj.0 = sr_f3( v );
    }
    // boolean
    else if( source.parse_bl( "#?w'true'" ) )
    {
        obj.0 = sr_bl( true );
    }
    // boolean
    else if( source.parse_bl( "#?w'false'" ) )
    {
        obj.0 = sr_bl( false );
    }
    else
    {
        return source.parse_error_fa( "Type expected." );
    }

    return er;
};

//----------------------------------------------------------------------------------------------------------------------

func (er_t t_parse_body( m @* o, tp_t t, m bcore_source* source )) = (try)
{
    m x_stamp* stamp = o;
    if( :t_defines_body_from_source( t ) )
    {
        :t_body_from_source( t, o, source );
        source.parse_em_fa( " </>" );
    }
    else if( t == st_s~ )
    {
        source.parse_em_fa( " #string", o.cast( m st_s* ) );
        source.parse_em_fa( " </>" );
    }
    else if( stamp.t_is_leaf( t ) )
    {
        source.parse_em_fa( st_s_create_fa( " ##<#<sc_t>*>", :name_of( t, st_s!^ ) )^.sc, o );
        source.parse_em_fa( " </>" );
    }
    else if( stamp.t_is_pure_array( t ) )
    {
        m x_array* arr = stamp;
        if( arr.t_is_mutable_mono_typed( t ) )
        {
            tp_t type = 0;
            if( source.parse_bl( "#?'type:'" ) )
            {
                sr_s sr = sr_null();
                :parse_create_object( source, sr.1 );
                type = sr_to_tp( sr );
            }
            arr.t_set_gtype( t, type );
        }

        if( arr.t_is_fixed( t ) )
        {
            sz_t arr_size = arr.t_size( t );
            sz_t arr_count = 0;
            while( !source.parse_bl( " #?'</>'" ) )
            {
                sr_s sr = sr_null();
                :parse_create_object( source, sr.1 );
                if( arr_count < arr_size )
                {
                    arr.t_set_sr( t, arr_count, sr );
                }
                else
                {
                    sr_down( sr );
                }
                arr_count++;
            }
        }
        else
        {
            arr.t_set_size( t, 0 );
            while( !source.parse_bl( " #?'</>'" ) )
            {
                sr_s sr = sr_null();
                :parse_create_object( source, sr.1 );
                arr.t_push_sr( t, sr );
            }
        }
    }
    else
    {
        while( !source.parse_bl( " #?'</>'" ) )
        {
            // non existing member variables are parsed but not assigned
            m st_s* name = st_s!^;
            source.parse_fa( " #name :", name );
            tp_t tp_name = btypeof( name.sc );
            sr_s sr = sr_null();
            :parse_create_object( source, sr.1 );
            if( stamp.t_exists( t, tp_name ) )
            {
                stamp.t_set_sr( t, tp_name, sr );
            }
            else
            {
                sr_down( sr );
            }
        }
    }

    stamp.t_source( t, source );
    stamp.t_mutated( t );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (void t_translate_recursive( @* o, tp_t t, tp_t name, bl_t shelve, m bcore_sink* sink, sz_t depth )) =
{
    sz_t indent = 4 * depth;
    m st_s* buf = st_s!^;

    // shelving obj_l
    if( o && shelve && bcore_via_call_t_defines_shelve( t ) )
    {
        m@* o_clone = o.cast( x_inst* ).t_clone( t )^;
        o_clone.cast( m x_stamp* ).t_shelve( t );
        o_clone.t_translate_recursive( t, name, false, sink, depth );
        return;
    }

    sink.push_fa( "#rn{ }", indent );
    if( name ) sink.push_fa( "#<sc_t>:", :name_of( name, buf ) );

    if( !o ) // o == NULL ...
    {
        if( t )
        {
            // Explicit NULL specifies that the object is not instantiated.
            // This is to differentiate from the case where the object was instantiated
            // without additional parameters.
            sink.push_fa( "<#<sc_t>>NULL</>\n", :name_of( t, buf ) );
        }
        else
        {
            sink.push_fa( "<></>\n" );
        }
    }
    else // o != NULL ...
    {
        sink.push_fa( "<#<sc_t>>", :name_of( t, buf ) );

        if( :t_defines_body_to_sink( t ) )
        {
            :t_body_to_sink( t, o, sink );
        }
        else if( t == TYPEOF_st_s ) // strings
        {
            m st_s* string = o.cast( st_s* ).clone()^;
            string.replace_char_sc( '\"', "\\\"" );
            sink.push_fa( "\"#<sc_t>\"", string.sc );
        }
        else if( x_stamp_t_is_leaf( t ) )
        {
            sink.push_string_d( st_s_create_typed( t, o ) );
        }
        else
        {
            sink.push_char( '\n' );
            if( x_stamp_t_is_pure_array( t ) )
            {
                x_array* arr = o;
                if( arr.t_is_mutable_mono_typed( t ) )
                {
                    tp_t mono_type = arr.t_get_mono_type( t );
                    :t_translate_recursive( mono_type.cast( @* ), tp_t~, TYPEOF_type, false, sink, depth + 1 );
                }
                sz_t size = arr.t_size( t );
                for( sz_t i = 0; i < size; i++ )
                {
                    sr_s sr = arr.t_c_get_sr( t, i );
                    :t_translate_recursive( sr.o, sr.o_type(), 0, true, sink, depth + 1 );
                    sr_down( sr );
                }
            }
            else
            {
                x_stamp* stamp = o;
                sz_t size = stamp.t_size( t );
                for( sz_t i = 0; i < size; i++ )
                {
                    sr_s sr = stamp.t_c_get_sr_i( t, i );
                    tp_t name = stamp.t_name( t, i );
                    :t_translate_recursive( sr.o, sr.o_type(), name, true, sink, depth + 1 );
                    sr_down( sr );
                }
            }
            sink.push_fa( "#rn{ }", indent );
        }
        sink.push_sc( "</>\n" );
    }
};

//----------------------------------------------------------------------------------------------------------------------

func t_test_transfer =
{
    d st_s* string = st_s!;
    o.t_to_sink( t, string );

    m bcore_source* source = bcore_source_string_s_create_from_string_d( string )^;

    d @* o2 = x_inst_t_create( t );
    if( o2.t_from_source( t, source ) )
    {
        bcore_error_pop_all_to_stderr();
        ERR_fa( "\n" );
    }

    s2_t c = bcore_compare_bityped( t, o, t, o2 );
    if( c != 0 )
    {
        st_s* diff = cast( bcore_diff_bityped( t, o, t, o2 ), d st_s* )^;
        if( diff )
        {
            ERR( "Comparison returned '%"PRIi32"':\n%s\n", c, diff->sc );
        }
        else
        {
            ERR( "Comparison returned '%"PRIi32"' but diff returned 'NULL'\n", c );
        }
    }

    o2.t_discard( t );
};

//----------------------------------------------------------------------------------------------------------------------

func (void selftest()) =
{
    sr_s^ zoo; zoo = bcore_spect_via_create_zoo( 1000 );

    clock_t time = clock();
    zoo.o.cast( @* ).t_test_transfer( sr_s_o_type( zoo ) );
    time = clock() - time;

    bcore_msg( "x_bbml transfer %5.3fs\n", ( double )time/CLOCKS_PER_SEC );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_BTML_H
