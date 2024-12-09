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

BTML: Beth text markup language.

Usage: See ../../doc/bcore/btml.md

*/

#ifndef BCORE_X_BTML_H
#define BCORE_X_BTML_H

#include "bcore_sr.h"
#include "bcore.xo.h"
#include "bcore_x_source.h"
#include "bcore_x_sink.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_btml, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/
/// interface

//----------------------------------------------------------------------------------------------------------------------

/** Reads full object from source.
 *  If o implements copy_typed, automatic type conversion is used.
 *  Allows recovering from parse errors and conversion errors.
 *  Checks error-stack after copy_typed.
 *  In case of a parse error o is not being changed.
 */
func er_t t_from_source( m@* o, tp_t t, m x_source* source );
func er_t   from_source( m@* o,         m x_source* source ) = o.t_from_source( o._, source );
func er_t t_from_file  ( m@* o, tp_t t,   sc_t file ) = o.t_from_source( t, x_source_check_create_from_file( file )^ );
func er_t   from_file  ( m@* o,           sc_t file ) = o.t_from_file( o._, file );
func er_t t_from_st    ( m@* o, tp_t t, c st_s* st  ) = o.t_from_source( t, x_source_create_from_st( st )^ );
func er_t   from_st    ( m@* o,         c st_s* st  ) = o.t_from_st( o._, st );
func er_t t_from_sc    ( m@* o, tp_t t,   sc_t  sc  ) = o.t_from_source( t, x_source_create_from_sc( sc )^ );
func er_t   from_sc    ( m@* o,           sc_t  sc  ) = o.t_from_sc( o._, sc );

/** Reads only object body from source (excluding <...>, </> enclosures)
 *  Allows recovering from parse errors and conversion errors.
 *  In case of a parse error o might have been changed.
 */
func er_t t_body_from_source( m@* o, tp_t t, m x_source* source ) = o.t_parse_body( t, source );
func er_t   body_from_source( m@* o,         m x_source* source ) = o.t_body_from_source( o._, source );
func er_t t_body_from_file  ( m@* o, tp_t t,   sc_t file ) = o.t_body_from_source( t, x_source_check_create_from_file( file )^ );
func er_t   body_from_file  ( m@* o,           sc_t file ) = o.t_body_from_file( o._, file );
func er_t t_body_from_st    ( m@* o, tp_t t, c st_s* st  ) = o.t_body_from_source( t, x_source_create_from_st( st )^ );
func er_t   body_from_st    ( m@* o,         c st_s* st  ) = o.t_body_from_st( o._, st );
func er_t t_body_from_sc    ( m@* o, tp_t t,   sc_t  sc  ) = o.t_body_from_source( t, x_source_create_from_sc( sc )^ );
func er_t   body_from_sc    ( m@* o,           sc_t  sc  ) = o.t_body_from_sc( o._, sc );

/** Reads and creates object from source.
 *  Returns NULL in case of parse error (check error-stack).
 *  If type is != NULL Sets type.0 to object's type.
 */
func d obliv @* create_from_source_t( m x_source* source, m tp_t* type );
func d obliv @* create_from_st_t( c st_s* st, m tp_t* type ) = :create_from_source_t( x_source_create_from_st( st )^, type );
func d obliv @* create_from_sc_t(   sc_t  sc, m tp_t* type ) = :create_from_source_t( x_source_create_from_sc( sc )^, type );
func d aware @* create_from_source( m x_source* source );
func d aware @* create_from_st( c st_s* st )  = :create_from_source( x_source_create_from_st( st )^ );
func d aware @* create_from_sc(   sc_t  sc )  = :create_from_source( x_source_create_from_sc( sc )^ );
func d aware @* create_from_file( sc_t file ) = :create_from_source( x_source_create_from_file( file )^ );

/// Tests initial source content for validity. Restores index.
func bl_t appears_valid( m x_source* source );

/// Tests initial source content if it represents an object of given type. Restores index.
func bl_t t_appears_valid( tp_t type, m x_source* source );

/// Writes object to sink.
func sink t_to_sink( c@* o, tp_t t, m x_sink* sink );
func sink   to_sink( c@* o,         m x_sink* sink ) = o.t_to_sink( o._, sink );
func void t_to_file( c@* o, tp_t t, sc_t file ) o.t_to_sink( t, bcore_file_open_sink( file )^ );
func void   to_file( c@* o,         sc_t file ) o.t_to_file( o._, file );
func void t_to_stdout( c@* o, tp_t t ) o.t_to_sink( t, x_sink_stdout() );
func void   to_stdout( c@* o,        ) o.t_to_stdout( o._ );

/// runs a transfer-test (write-read-compare) on given object; in case of a mismatch a descriptive error is created. Returns file/stream size involved.
func sz_t t_test_transfer( @* o, tp_t t );
func sz_t   test_transfer( @* o ) = o.t_test_transfer( o._ );

/** Overload these features for objects that define their own markup syntax.
 *  Note: Always overload both features with compatible syntax to ensure I/O consistency.
 */
feature 'at' er_t btml_body_from_source( m@* o, m x_source* source );
feature 'at' void btml_body_to_sink(     c@* o, m x_sink* sink );

//----------------------------------------------------------------------------------------------------------------------

func t_from_source
{
    sr_s sr = sr_null();
    :parse_create_object( source, NULL, 0, sr.1 );
    x_inst_t_copy_typed( o, t, sr_s_o_type( sr.1 ), sr.o );
    sr_s_down( sr );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func create_from_source_t
{
    if( !source )
    {
        type.0 = 0;
        = NULL;
    }
    sr_s sr = sr_null();
    if( :parse_create_object( source, NULL, 0, sr.1 ) ) = NULL;
    if( sr.o && type ) type.0 = sr_s_o_type( sr );
    = sr.o.cast( d @* ); // sr.o is NULL in case of error
}

//----------------------------------------------------------------------------------------------------------------------

func create_from_source
{
    tp_t t = 0;
    d @* o = :create_from_source_t( source, t.1 );
    if( t )
    {
        ASSERT( x_stamp_t_is_aware( t ) );
    }
    = o;
}

//----------------------------------------------------------------------------------------------------------------------

func t_to_sink
{
    o.t_translate_recursive( t, 0, true, sink, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// implementation

//----------------------------------------------------------------------------------------------------------------------

func bl_t is_identifier( m x_source* source ) = source.parse_bl( " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" );

//----------------------------------------------------------------------------------------------------------------------

func d st_s* parse_name( m x_source* source )
{
    m st_s* st_name = st_s!^;
    source.parse_fa( " #name", st_name );
    = st_name.fork();
}

//----------------------------------------------------------------------------------------------------------------------

func sc_t name_of( tp_t type, m st_s* buf )
{
    sc_t n = bcore_name_try_name( type );
    if( n ) = n;
    buf.copy_typed( tp_t~, type.1 );
    = buf.sc;
}

// ---------------------------------------------------------------------------------------------------------------------

func tp_t type_of( const st_s* name )
{
    tp_t tp = 0;
    if( name.size == 0 )
    {
        = 0;
    }
    else if( name.[ 0 ] >= '0' && name.[ 0 ] <= '9' )
    {
        name.parse_fa( 0, -1, "#tp_t", &tp );
    }
    else
    {
        tp = btypeof( name.sc );
    }
    = tp;
}

//----------------------------------------------------------------------------------------------------------------------

func appears_valid
{
    bl_t valid = false;
    sz_t index = source.get_index();
    m st_s* type_string = st_s!^;
    if( source.parse_bl( " #?'<'" ) ) // type specifier
    {
        if( source.parse_fa( "#until'>'>", type_string ) )
        {
            bcore_error_remove_last();
        }
        else
        {
            if( type_string.size > 0 )
            {
                if( type_string.[ type_string.size - 1 ] == '/' ) type_string.pop_char();
            }
            tp_t type = :type_of( type_string );
            if( bcore_flect_exists( type ) )      valid = true;
            else if( type == btypeof( "#file" ) ) valid = true;
            else if( type == btypeof( "#path" ) ) valid = true;
        }
    }

    source.set_index( index );
    = valid;
}

//----------------------------------------------------------------------------------------------------------------------

func t_appears_valid
{
    bl_t valid = false;
    sz_t index = source.get_index();
    m st_s* type_string = st_s!^;
    if( source.parse_bl( " #?'<'" ) ) // type specifier
    {
        if( source.parse_fa( "#until'>'>", type_string ) )
        {
            bcore_error_remove_last();
        }
        else
        {
            if( :type_of( type_string ) == type ) valid = true;
        }
    }

    source.set_index( index );
    = valid;
}

//----------------------------------------------------------------------------------------------------------------------

/** On entering, obj should be sr_null
 *  In case of error obj need not be discarded
 *  if default_obj is defined, obj copies from default_obj before parsing the body
 *  default_tp != 0 takes the place of an empty type specifier (syntax: <>)
 */
func er_t parse_create_object( m x_source* source, sr_s* default_obj, tp_t default_tp, m sr_s* obj )
{
    er_t er = 0;
    m st_s* type_string = st_s!^;
    source.parse_fa( " " ); // take whitespaces

    if( source.parse_bl( "#?'<'" ) ) // type specifier
    {
        source.parse_fa( "#until'>'>", type_string );
        bl_t compact = false; // compact version of <mystamp_s></> is <mystamp_s/>
        tp_t type = 0;
        if( type_string.size > 0 && type_string.[ type_string.size - 1 ] =='/' )
        {
            type_string.pop_char();
            compact = true;
        }

        if( type_string.size == 1 && type_string.[ 0 ] =='*' )
        {
            if( default_tp == 0 )
            {
                = source.parse_error_fa( "Context does not provide an inheritable type." );
            }
            type = default_tp;
        }
        else
        {
            type = :type_of( type_string );
        }

        if( type )
        {
            if( bcore_flect_exists( type ) )
            {
                if( source.parse_bl( " #?'NULL'" ) ) // no instance
                {
                    if( !compact) source.parse_fa( " </>" );
                    obj.p = bcore_inst_s_get_typed( type );
                }
                else
                {
                    m x_inst* inst = x_inst_t_create( type ).t_scope( type );
                    if( default_obj.type() == type ) x_inst_t_copy( inst, type, default_obj.o );
                    if( !compact)
                    {
                        :t_parse_body( inst, type, source );
                        source.parse_fa( " </>" );
                    }
                    else
                    {
                        inst.cast( m x_stamp* ).t_source( type, source );
                    }
                    obj.0 = sr_tsm( type, inst.fork() );
                }
            }
            else
            {
                tp_t typeof_file = btypeof( "#file" );
                tp_t typeof_path = btypeof( "#path" );
                if( type == typeof_file || type == typeof_path )
                {
                    m st_s* path = st_s!^;
                    source.parse_fa( " #string #-?'</>'", path );

                    if( path.[ 0 ] != '/' ) // make path relative to current file path
                    {
                        m st_s* cur_file = st_s_create_sc( source.get_file() )^;
                        if( cur_file.size > 0 )
                        {
                            sz_t idx = cur_file.find_char( cur_file.size, 0, '/' );
                            if( idx < cur_file.size )
                            {
                                cur_file.[ idx ] = 0;
                                path.copy( st_s_create_fa( "#<sc_t>/#<sc_t>", cur_file.sc, path.sc )^ );
                            }
                        }
                    }

                    if( type == typeof_file )
                    {
                        if( bcore_file_exists( path.sc ) )
                        {
                            :parse_create_object( bcore_file_open_source( path.sc )^, NULL, 0, obj );
                        }
                        else
                        {
                            = source.parse_error_fa( "File '#<sc_t>' not found.", path.sc );
                        }
                    }
                    else
                    {
                        obj.0 = sr_asm( path.fork() );
                    }
                }
                else
                {
                    = source.parse_error_fa( "Type '#<sc_t>' has no reflection.", type_string.sc );
                }
            }
        }
        else
        {
            source.parse_fa( " </>" );
        }
    }

    // simplified encoding for certain leaf types ...

    // string
    else if( source.parse_bl( "#=?'\"'" ) )
    {
        m st_s* st = st_s!^;
        source.parse_fa( " #string", st );
        obj.0 = sr_tsm( st_s~, st.fork() );
    }
    // number
    else if( source.parse_bl( "#?(([0]>='0'&&[0]<='9')||[0]=='+'||[0]=='-')" ) )
    {
        f3_t v;
        source.parse_fa( "#<f3_t*>", &v );
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
        :skip_body( source );
        source.parse_fa( " </>" );
    }

    = er;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t t_parse_body( m @* o, tp_t t, m x_source* source )
{
    sr_s default_element = sr_null();
    sr_s* default_sr = NULL;

    if( source.parse_bl( "#?w'#default:'" ) )
    {
        :parse_create_object( source, NULL, 0, default_element.1 );
        default_sr = default_element.1;
    }

    m x_stamp* stamp = o;
    if( o.t_defines_btml_body_from_source( t ) )
    {
        o.t_btml_body_from_source( t, source );
    }

    /// supported string formats: '"..."' or any text terminated by whitespace or '</>'
    else if( t == st_s~ )
    {
        m st_s* st = o.cast( m st_s* );
        if( source.parse_bl( " #=?'\"'" ) )
        {
            source.parse_fa( " #string", st );
        }
        else
        {
            st.clear();
            while( !source.eos() )
            {
                char c = source.inspect_char();
                if( c == ' ' || c == '\t' || c == '\r' || c == '\n' ) break;
                if( c == '<' && source.parse_bl( " #=?'</>'" ) ) break;
                st.push_char( source.get_char() );
            }
        }
    }
    else if( stamp.t_is_leaf( t ) )
    {
        switch( t )
        {
            case tp_t~:
            case er_t~:
            case aware_t~:
            {
                if( :is_identifier( source ) )
                {
                    o.cast( m tp_t* ).0 = btypeof( :parse_name( source )^.sc );
                }
                else
                {
                    source.parse_fa( st_s_create_fa( " ##<#<sc_t>*>", :name_of( t, st_s!^ ) )^.sc, o );
                }
            }
            break;

            default:
            {
                source.parse_fa( st_s_create_fa( " ##<#<sc_t>*>", :name_of( t, st_s!^ ) )^.sc, o );
            }
            break;
        }
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
                :parse_create_object( source, default_sr, 0, sr.1 );
                type = sr_to_tp( sr );
            }
            arr.t_set_gtype( t, type );
        }

        tp_t mono_type = arr.t_get_mono_type( t ); // mono type is 0 in case array has no mono type

        if( arr.t_is_fixed( t ) )
        {
            sz_t arr_size = arr.t_size( t );
            sz_t arr_count = 0;

            while( !source.parse_bl( " #=?'</>'" ) )
            {
                sr_s sr = sr_null();
                :parse_create_object( source, default_sr, mono_type, sr.1 );
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
            while( !source.parse_bl( " #=?'</>'" ) )
            {
                sr_s sr = sr_null();
                :parse_create_object( source, default_sr, mono_type, sr.1 );
                arr.t_push_sr( t, sr );
            }
        }
    }
    else
    {
        while( !source.parse_bl( " #=?'</>'" ) )
        {
            m st_s* name = st_s!^;
            source.parse_fa( " #name :", name );
            tp_t tp_name = btypeof( name.sc );
            sr_s sr = sr_null();
            // non existing member variables are parsed but not assigned
            if( stamp.t_exists( t, tp_name ) )
            {
                :parse_create_object( source, default_sr, stamp.t_type( t, tp_name ), sr.1 );
                stamp.t_set_sr( t, tp_name, sr );
            }
            else
            {
                :parse_create_object( source, default_sr, 0, sr.1 ); // skip object and remove it
                sr_down( sr );
            }
        }
    }

    stamp.t_source( t, source );
    stamp.t_mutated( t );

    sr_down( default_element );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t skip_body( m x_source* source )
{
    while( !source.eos() )
    {
        if( source.parse_bl( " #=?'</>'" ) ) break;
        if( source.parse_bl( "#?'<'" ) )
        {
            source.parse_fa( "#-until'>'>" );
            :skip_body( source );
            source.parse_fa( "</>" );
        }
        else if( source.parse_bl( "#=?'\"'" ) ) // strings
        {
            source.parse_fa( "#-string" );
        }
        else
        {
            source.get_char();
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func void t_translate_recursive( @* o, tp_t t, tp_t name, bl_t shelve, m x_sink* sink, sz_t depth )
{
    sz_t indent = 4 * depth;
    m st_s* buf = st_s!^;

    // shelving obj_l
    if( o && shelve && bcore_via_call_t_defines_shelve( t ) )
    {
        d @* o_clone = o.cast( x_inst* ).t_clone( t ); // o_clone is obliv
        o_clone.cast( m x_stamp* ).t_shelve( t );
        o_clone.t_translate_recursive( t, name, false, sink, depth );
        o_clone.cast( d x_inst* ).t_discard( t );
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

        if( o.t_defines_btml_body_to_sink( t ) )
        {
            o.t_btml_body_to_sink( t, sink );
        }
        else if( t == TYPEOF_st_s ) // strings
        {
            m st_s* string = o.cast( st_s* ).clone()^;
            string.replace_char_sc( '\"', "\\\"" );
            sink.push_fa( "\"#<sc_t>\"", string.sc );
        }
        else if( x_stamp_t_is_leaf( t ) )
        {
            switch( t )
            {
                case tp_t~:
                case er_t~:
                case aware_t~:
                {
                    sc_t sc_name = bnameof( *( tp_t* )o );
                    if( sc_name )
                    {
                        sink.push_fa( "#<sc_t>", sc_name );
                    }
                    else
                    {
                        sink.push_st_d( st_s_create_typed( t, o ) );
                    }
                }
                break;

                default:
                {
                    sink.push_st_d( st_s_create_typed( t, o ) );
                }
                break;
            }
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
}

//----------------------------------------------------------------------------------------------------------------------

func t_test_transfer
{
    d st_s* string = st_s!;
    o.t_to_sink( t, string );
    sz_t file_size = string.size;
    m x_source* source = x_source_create_from_st_d( string )^;

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
    = file_size;
}

//----------------------------------------------------------------------------------------------------------------------

func void selftest()
{
    sr_s^ zoo; zoo = bcore_spect_via_create_zoo( 1000 );

    clock_t time = clock();
    sz_t size = zoo.o.cast( @* ).t_test_transfer( sr_s_o_type( zoo ) );
    time = clock() - time;

    bcore_msg_fa( "x_btml transfer #<f3_t>s; size: #<sz_t> bytes\n", ( f3_t )time/CLOCKS_PER_SEC, size );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_BTML_H
