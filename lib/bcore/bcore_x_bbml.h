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

BTML: Beth binary markup language .

Version handling without version declaration:

New elements:
An incomplete set of members is handled by setting unspecified elements to their respective default.

Retired elements:
An unrecognized element name is ignored by the parser. (The object of that element might be temporarily created and parsed)

*/

#ifndef BCORE_X_BBML_H
#define BCORE_X_BBML_H

#include "bcore_sr.h"
#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_bbml, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/
/// interface

//----------------------------------------------------------------------------------------------------------------------

/** Reads object from source.
 *  If o implements copy_typed, automatic type conversion is used.
 *  Allows recovering from parse errors and conversion errors.
 *  Checks error-stack after copy_typed.
 *  In case of a parse error o is not being changed.
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
 *  If type is != NULL Sets type.0 to object's type.
 */
func (d obliv @* create_from_source_t( m bcore_source* source, m tp_t* type ));
func (d obliv @* create_from_st_t( c st_s* st, m tp_t* type )) = { return :create_from_source_t( bcore_source_string_s_create_from_string( st )^, type ); };
func (d obliv @* create_from_sc_t(   sc_t  sc, m tp_t* type )) = { return :create_from_source_t( bcore_source_string_s_create_from_sc( sc )^, type ); };
func (d aware @* create_from_source( m bcore_source* source ));
func (d aware @* create_from_st( c st_s* st )) = { return :create_from_source( bcore_source_string_s_create_from_string( st )^ ); };
func (d aware @* create_from_sc(   sc_t  sc )) = { return :create_from_source( bcore_source_string_s_create_from_sc( sc )^ ); };

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

/** Overload these features for objects that define their own markup syntax.
 *  Note: Always overload both features with compatible syntax to ensure I/O consistency.
 */
feature 'at' er_t feature_body_from_source( m@* o, m bcore_source* source );
feature 'at' void feature_body_to_sink(     c@* o, m bcore_sink* sink );

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

func create_from_source_t =
{
    sr_s sr = sr_null();
    :parse_create_object( source, sr.1 );
    if( sr.o && type ) type.0 = sr_s_o_type( sr );
    return sr.o.cast( d @* ); // sr.o is NULL in case of error
};

//----------------------------------------------------------------------------------------------------------------------

func create_from_source =
{
    tp_t t = 0;
    d @* o = :create_from_source_t( source, t.1 );
    if( t )
    {
        ASSERT( x_stamp_t_is_aware( t ) );
    }
    return o;
};

//----------------------------------------------------------------------------------------------------------------------

func t_to_sink =
{
    o.t_translate_recursive( t, 0, true, sink );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (tp_t parse_type( m bcore_source* source )) = { tp_t type = 0; source.get_data( type.1, sizeof( type ) ); return type; };
func (bl_t parse_flag( m bcore_source* source )) = { u0_t flag = 0; source.get_data( flag.1, sizeof( flag ) ); return flag; };
func (sz_t parse_size( m bcore_source* source )) = { sz_t size = 0; source.get_data( size.1, sizeof( size ) ); return size; };

//----------------------------------------------------------------------------------------------------------------------

func appears_valid =
{
    sz_t index = source.get_index();
    tp_t type = :parse_type( source );
    bl_t valid = bcore_flect_exists( type );
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
    tp_t type = :parse_type( source );
    bl_t flag = :parse_flag( source );

    if( type )
    {
        if( bcore_flect_exists( type ) )
        {
            if( flag )
            {
                m x_inst* inst = x_inst_t_create( type ).t_scope( type );
                :t_parse_body( inst, type, source );
                obj.0 = sr_tsd( type, inst.fork() );
            }
            else // no instance
            {
                obj.p = bcore_inst_s_get_typed( type );
            }
        }
        else
        {
            return bcore_error_push_fa( TYPEOF_parse_error, "Type '#<sc_t>' has no reflection.", bnameof( type ) );
        }
    }

    return er;
};

//----------------------------------------------------------------------------------------------------------------------

func (er_t t_parse_body( m @* o, tp_t t, m bcore_source* source )) = (try)
{
    m x_stamp* stamp = o;
    if( o.t_defines_feature_body_from_source( t ) )
    {
        o.t_feature_body_from_source( t, source );
    }
    else if( t == st_s~ )
    {
        m st_s* st = o.cast( m st_s* );
        st.clear();
        char c = 0;
        while( (c = source.get_char()) != 0 ) st.push_char( c );
    }
    else if( stamp.t_is_leaf( t ) )
    {
        switch( t )
        {
            case TYPEOF_aware_t : source.get_data( o, sizeof( aware_t ) ); break;
            case TYPEOF_bl_t    : source.get_data( o, sizeof( bl_t ) ); break;
            case TYPEOF_f2_t    : source.get_data( o, sizeof( f2_t ) ); break;
            case TYPEOF_f3_t    : source.get_data( o, sizeof( f3_t ) ); break;
            case TYPEOF_s0_t    : source.get_data( o, sizeof( s0_t ) ); break;
            case TYPEOF_s1_t    : source.get_data( o, sizeof( s1_t ) ); break;
            case TYPEOF_s2_t    : source.get_data( o, sizeof( s2_t ) ); break;
            case TYPEOF_s3_t    : source.get_data( o, sizeof( s3_t ) ); break;
            case TYPEOF_sc_t    : break; // c-style constant strings are skipped over
            case TYPEOF_sz_t    : source.get_data( o, sizeof( sz_t ) ); break;
            case TYPEOF_uz_t    : source.get_data( o, sizeof( uz_t ) ); break;
            case TYPEOF_tp_t    : source.get_data( o, sizeof( tp_t ) ); break;
            case TYPEOF_u0_t    : source.get_data( o, sizeof( u0_t ) ); break;
            case TYPEOF_u1_t    : source.get_data( o, sizeof( u1_t ) ); break;
            case TYPEOF_u2_t    : source.get_data( o, sizeof( u2_t ) ); break;
            case TYPEOF_u3_t    : source.get_data( o, sizeof( u3_t ) ); break;
            default: ERR_fa( "Cannot convert type '#<sc_t>' from a binary stream.", bnameof( t ) );
        }
    }
    else if( stamp.t_is_pure_array( t ) )
    {
        m x_array* arr = stamp;
        if( arr.t_is_mutable_mono_typed( t ) ) arr.t_set_gtype( t, :parse_type( source ) );

        sz_t size = :parse_size( source );

        if( arr.t_is_fixed( t ) )
        {
            sz_t arr_size = arr.t_size( t );

            for( sz_t i = 0; i < size; i++ )
            {
                sr_s sr = sr_null();
                :parse_create_object( source, sr.1 );
                if( i < arr_size )
                {
                    arr.t_set_sr( t, i, sr );
                }
                else
                {
                    sr_down( sr );
                }
            }
        }
        else
        {
            arr.t_set_size( t, size );
            for( sz_t i = 0; i < size; i++ )
            {
                sr_s sr = sr_null();
                :parse_create_object( source, sr.1 );
                arr.t_set_sr( t, i, sr );
            }
        }
    }
    else
    {
        while( !source.eos() )
        {
            // non existing member variables are parsed but not assigned
            tp_t tp_name = :parse_type( source );
            if( !tp_name ) break;

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

func (void push_type( m bcore_sink* sink, tp_t type )) = { sink.push_data( type.1, sizeof( type ) ); };
func (void push_flag( m bcore_sink* sink, bl_t flag )) = { u0_t v = flag ? 1 : 0;  sink.push_data( v.1, 1 ); };
func (void push_size( m bcore_sink* sink, sz_t size )) = { sink.push_data( size.1, sizeof( size ) ); };

func (void t_translate_recursive( @* o, tp_t t, tp_t name, bl_t shelve, m bcore_sink* sink )) =
{
    // shelving obj_l
    if( o && shelve && bcore_via_call_t_defines_shelve( t ) )
    {
        d @* o_clone = o.cast( x_inst* ).t_clone( t ); // no scoping (o_clone is obliv)
        o_clone.cast( m x_stamp* ).t_shelve( t );
        o_clone.t_translate_recursive( t, name, false, sink );
        o_clone.cast( d x_inst* ).discard();
        return;
    }

    if( name ) :push_type( sink, name );

    :push_type( sink, t );
    :push_flag( sink, o != NULL );

    if( o )
    {
        if( o.t_defines_feature_body_to_sink( t ) )
        {
            o.t_feature_body_to_sink( t, sink );
        }
        else if( t == TYPEOF_st_s ) // strings
        {
            st_s* string = o.cast( st_s* );
            sink.push_data( ( vc_t )string.data, string.size + 1 ); // push string including terminating 0
        }
        else if( x_stamp_t_is_leaf( t ) )
        {
            switch( t )
            {
                case TYPEOF_aware_t : sink.push_data( o, sizeof( aware_t ) ); break;
                case TYPEOF_bl_t    : sink.push_data( o, sizeof( bl_t ) ); break;
                case TYPEOF_f2_t    : sink.push_data( o, sizeof( f2_t ) ); break;
                case TYPEOF_f3_t    : sink.push_data( o, sizeof( f3_t ) ); break;
                case TYPEOF_s0_t    : sink.push_data( o, sizeof( s0_t ) ); break;
                case TYPEOF_s1_t    : sink.push_data( o, sizeof( s1_t ) ); break;
                case TYPEOF_s2_t    : sink.push_data( o, sizeof( s2_t ) ); break;
                case TYPEOF_s3_t    : sink.push_data( o, sizeof( s3_t ) ); break;
                case TYPEOF_sc_t    : break; // c-style constant strings are skipped over
                case TYPEOF_sz_t    : sink.push_data( o, sizeof( sz_t ) ); break;
                case TYPEOF_uz_t    : sink.push_data( o, sizeof( uz_t ) ); break;
                case TYPEOF_tp_t    : sink.push_data( o, sizeof( tp_t ) ); break;
                case TYPEOF_u0_t    : sink.push_data( o, sizeof( u0_t ) ); break;
                case TYPEOF_u1_t    : sink.push_data( o, sizeof( u1_t ) ); break;
                case TYPEOF_u2_t    : sink.push_data( o, sizeof( u2_t ) ); break;
                case TYPEOF_u3_t    : sink.push_data( o, sizeof( u3_t ) ); break;
                default: ERR_fa( "Cannot convert type '#<sc_t>' into a binary stream.", bnameof( t ) );
            }
        }
        else
        {
            if( x_stamp_t_is_pure_array( t ) )
            {
                x_array* arr = o;
                if( arr.t_is_mutable_mono_typed( t ) ) :push_type( sink, arr.t_get_mono_type( t ) );
                sz_t size = arr.t_size( t );
                :push_size( sink, size );
                for( sz_t i = 0; i < size; i++ )
                {
                    sr_s sr = arr.t_c_get_sr( t, i );
                    :t_translate_recursive( sr.o, sr.o_type(), 0, true, sink );
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
                    :t_translate_recursive( sr.o, sr.o_type(), name, true, sink );
                    sr_down( sr );
                }

                /// we push a terminating '0' to indicate the end of the parameter list
                :push_type( sink, 0 );
            }
        }
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

#endif  // BCORE_X_BBML_H
