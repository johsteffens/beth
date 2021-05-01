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

#ifndef BCORE_SHELL_H
#define BCORE_SHELL_H

#include "bcore_spect_source.h"
#include "bcore_sources.h"
#include "bcore_file.h"
#include "bcore_x_inst.h"
#include "bcore_x_source.h"
#include "bcore_x_sink.h"
#include "bcore_hmap.h"
#include "bcore_hmap_name.h"
#include "bcore_hmap_tp_st.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bcore_shell, x_inst )

#ifdef XOILA_SECTION

forward x_inst;
forward bcore_main_frame_s;
forward :control_s;

//----------------------------------------------------------------------------------------------------------------------

group :op =
{
    /// Implement 'key' (simple string) or 'get_key' (formatted string)
    feature 'a' sc_t key(  @* o );
    feature 'a' void get_key(  @* o, m st_s* key ) = { key.copy_sc( o.key() ); };

    /// Implement 'info' (simple string) or 'get_info' (formatted string)
    feature 'a' sc_t info( @* o );
    feature 'a' void get_info( @* o, m st_s* info ) = { info.copy_sc( o.info() ); };

    feature 'a' void run
    (
        m @* o,
        m ::* obj,
        bcore_main_frame_s* main_frame,
        m x_source* source,
        m bcore_sink* sink,
        m bcore_shell_control_s* control
    );

    func (bl_t key_match( @* o, sc_t key )) =
    {
        st_s^ st_key;
        o.get_key( st_key.1 );
        sz_t index = 0;
        while( index < st_key.size )
        {
            st_s^ name;
            char c = 0;
            while( index < st_key.size )
            {
                c = st_key.[ index++ ];
                if( c == ' ' || c== ',' ) break;
                name.push_char( c );
            }
            if( name.size == 0 ) return false;
            if( name.equal_sc( key ) ) return true;
        }
        return false;
    };

    /** Tests for keyword match.
     *  false: No match. Nothing consumed. Go try next command.
     *  true:  Match. Keyword consumed. Continue with 'parse_param'.
     */
    func (bl_t parse_match( m @* o, m x_source* source )) =
    {
        st_s^ key;
        source.parse_fa( "#=until' '", key.1 );
        if( key.size == 0 ) return false;
        if( !o.key_match( key.sc ) ) return false;
        source.parse_fa( "#-until' '" );
        return true;
    };

    /** Parses parameters of operator.
     *  false: An error occurred. Discard this operation and continue with next command.
     *  true:  Parsing successful. Continue with 'run'
     */
    func (bl_t parse_param( m @* o, m x_source* source, m bcore_sink* sink )) =
    {
        sz_t direct_index = 0;
        source.parse_fa( "#skip' \t'" );
        while( !source.eos() && !source.parse_bl( "#?'\n'" ) )
        {
            m x_stamp* stamp = o;

            /// addressing parameter via name
            if( source.parse_bl( "#?([0]=='-'&&(([1]>='A'&&[1]<='Z')||([1]>='a'&&[1]<='z')))" ) )
            {
                st_s^ name;
                source.parse_fa( "-#name", &name );
                tp_t tp_name = btypeof( name.sc );
                sz_t index = 0;
                for( index = 0; index < stamp.size(); index++ ) if( stamp.name( index ) == tp_name ) break;
                if( index == stamp.size() )
                {
                    sink.push_fa( "Invalid parameter '#<sc_t>'\n", name.sc );
                    return false;
                }
                source.parse_fa( "#skip' '" );

                tp_t type = stamp.type( tp_name );

                switch( type )
                {
                    case bl_t~:
                    {
                        if( source.parse_bl( "#?'true'" ) )
                        {
                            stamp.set_sr( tp_name, sr_bl( true ) );
                        }
                        else if( source.parse_bl( "#?'false'" ) )
                        {
                            stamp.set_sr( tp_name, sr_bl( false ) );
                        }
                        else
                        {
                            stamp.set_sr( tp_name, sr_bl( true ) );
                        }
                    }
                    break;

                    case sz_t~:
                    case uz_t~:
                    case f3_t~:
                    {
                        f3_t v_f3 = 0;
                        source.parse_fa( "#<f3_t*>", &v_f3 );
                        stamp.set_sr( tp_name, sr_f3( v_f3 ) );
                    }
                    break;

                    case st_s~:
                    {
                        st_s^ st;
                        if( source.parse_bl( "#=?'\"'" ) )
                        {
                            source.parse_fa( "#string", st.1 );
                        }
                        else
                        {
                            source.parse_fa( "#name", st.1 );
                        }
                        stamp.set_sr( tp_name, sr_awc( st ) );
                    }
                    break;

                    default:
                    {
                        sink.push_fa( "Parameter '#<sc_t>' is of unhandled type '#<sc_t>'.\n", name.sc, bnameof( type ) );
                        return false;
                    }
                }
            }
            else // addressing via index counter
            {
                if( direct_index >= stamp.size() )
                {
                    sink.push_fa( "Passing excess argument. This command has only #<sz_t> arguments.\n", stamp.size() );
                    return false;
                }

                tp_t type = stamp.type_i( direct_index );
                m x_inst* inst = x_inst_t_create( type ).t_scope( type, scope_local );
                if( x_btml_t_body_from_source( inst, type, source ) )
                {
                    bcore_error_pop_all_to_sink( sink );
                    return false;
                }

                stamp.set_sr_i( direct_index, sr_tsd( type, inst.fork() ) );
                direct_index++;
            }
            source.parse_fa( "#skip' \t'" );
        }

        return true;
    };

    /// Sends a description of operators argument signature to sink.
    func (void arg_signature_to_sink( m @* o, m bcore_sink* sink )) =
    {
        m x_stamp* v = o;
        for( sz_t i = 0; i < v.size(); i++ )
        {
            tp_t name = v.name( i );
            tp_t type = v.type( name );
            switch( type )
            {
                case f3_t~: case sz_t~: case uz_t~: case s3_t~: case u3_t~:
                {
                    sink.push_fa( "[<#<sc_t>>(number)] ", bnameof( name ) );
                }
                break;

                case st_s~:
                {
                    sink.push_fa( "[<#<sc_t>>(text)] ", bnameof( name ) );
                }
                break;

                case bl_t~:
                {
                    sink.push_fa( "[-#<sc_t>] ", bnameof( name ) );
                }
                break;

                default:
                {
                    sink.push_fa( "[<#<sc_t>>(#<sc_t> via BTML)] ", bnameof( name ), bnameof( type ) );
                }
                break;
            }
        }
    };

    //----------------------------------------------------------------------------------------------------------------------

};

//----------------------------------------------------------------------------------------------------------------------

group :control =
{
    /// request to exit loop asap
    signature void request_exit_loop( m @* o );
    signature void request_exit_all( m @* o );
    signature bl_t exit_loop( @* o );
    signature o reset( m @* o );
    signature d @* spawn( m @* o );

    stamp :s =
    {
        hidden :s* parent;
        aware x_sink   -> sink;
        aware x_source -> source;

        bcore_hmap_name_s  hmap_name;
        bcore_hmap_tp_st_s hmap_alias;

        bl_t exit_loop;
        st_s path;
        func :.reset             = { o.exit_loop = false; return o; };
        func :.request_exit_loop = { o.exit_loop = true; };
        func :.request_exit_all  = { o.exit_loop = true; if( o.parent ) o.parent.request_exit_all(); };
        func :.exit_loop         = { return o.exit_loop; };
        func :.spawn             = { d @* r = o.clone(); r.parent = o; return r; };
    };
};

//----------------------------------------------------------------------------------------------------------------------

/// Overload one of features below to register groups with operators.

// Overload this to always include default operators
feature tp_t op_group( @* o ) = { return :op_default~; };

// Overload this to handle multiple groups (if default group is wanted, it must be explicitly added to list)
feature void push_op_groups( @* o, m bcore_arr_tp_s* list ) =
{
    list.push( o.op_group() );
    if( !list.exists( :op_default~ ) ) list.push( :op_default~ );
};

//----------------------------------------------------------------------------------------------------------------------

func (d bcore_arr_tp_s* get_op_stamps( @* o ) ) =
{
    bcore_arr_tp_s^ op_groups;
    o.push_op_groups( op_groups );
    d $* op_stamps = bcore_arr_tp_s!;
    foreach( tp_t t in op_groups ) op_stamps.push_arr( x_group_get_arr_traitline_stamps( t ) );
    return op_stamps;
};

//----------------------------------------------------------------------------------------------------------------------

func (void help_to_sink( m @* o, :control_s* control, m bcore_sink* sink )) =
{
    bcore_arr_tp_s^ op_groups;
    o.push_op_groups( op_groups );

    foreach( tp_t tp_group in op_groups )
    {
        sink.push_fa( "#p80'*'{******** #<sc_t> }\n", bnameof( tp_group ) );
        bcore_arr_st_s^ table;
        foreach( tp_t t in x_group_get_arr_traitline_stamps( tp_group ) )
        {
            m :op* op = x_inst_t_create( t ).t_scope( t );
            st_s^ key;
            op.get_key( key );
            key.replace_char_char( ' ', ',' );
            st_s^ sign;
            sign.push_fa( "#<sc_t> ", key.sc );
            op.arg_signature_to_sink( sign );
            st_s^ info;
            op.get_info( info );
            table.push_fa( "#<st_s*>", sign.1 );
            table.push_fa( ": #<st_s*>", info.1 );
        }
        table.table_to_sink( -1, 2, sink );
    }

    if( control.hmap_alias.keys() > 0 )
    {
        sink.push_fa( "#p80'*'{******** #<sc_t> }\n", "ALIAS" );
        bcore_arr_st_s^ table;
        for( sz_t i = 0; i < control.hmap_alias.size(); i++ )
        {
            tp_t key = control.hmap_alias.idx_key( i );
            if( key )
            {
                st_s* expression = control.hmap_alias.idx_val( i );
                table.push_fa( "#<st_s*>", control.hmap_name.get( key ) );
                table.push_fa( ": #<st_s*>", expression );
            }
        }
        table.table_to_sink( -1, 2, sink );
    }

};

//----------------------------------------------------------------------------------------------------------------------

signature void loop
(
    m @* o,
    bcore_main_frame_s* frame,
    m bcore_shell_control_s* control
);

func loop =
{
    ASSERT( control );
    while( !control.source.eos() && !control.exit_loop() )
    {
        control.sink.push_fa( "\n#<sc_t>#<sc_t>(#<sc_t>)> ", control.path.sc, control.path.size ? " " : "", bnameof( o._ ) ).flush();
        control.reset();

        st_s^ expression;
        control.source.parse_fa( " #until'\n'#skip'\n'", expression.1 );
        if( control.hmap_alias.exists( btypeof( expression.sc ) ) )
        {
            expression.copy( control.hmap_alias.get( btypeof( expression.sc ) ) );
        }

        m$* line_source = x_source_create_from_st( expression )^;

        if( expression.size > 0 )
        {
            bl_t found = false;

            foreach( tp_t t in o.get_op_stamps()^ )
            {
                m :op* op = x_inst_t_create( t ).t_scope( t );
                if( op.parse_match( line_source ) )
                {
                    if( op.parse_param( line_source, control.sink ) )
                    {
                        op.run( o, frame, control.source, control.sink, control );
                    }
                    found = true;
                    break;
                }
            }

            if( !found )
            {
                control.sink.push_fa( "Syntax error.\n" );
                o.help_to_sink( control, control.sink );
            }
        }
    }

    control.exit_loop = false;
};

//----------------------------------------------------------------------------------------------------------------------

group :op_default = retrievable
{
    stamp :help_s =
    {
        func ::op.key = { return "?,help"; };
        func ::op.info = { return "Lists available commands"; };
        func ::op.run = { obj.help_to_sink( control, sink ); };
    };

    stamp :ls_s =
    {
        st_s path;

        func ::op.get_key = { key.push_fa( "ls" ); };
        func ::op.get_info = { info.push_fa( "Lists all visible members" ); };
        func ::op.run =
        {
            x_stamp_path_s^ path.parse( x_source_create_from_sc( o.path.sc )^ );
            sr_s sr = path.get_sr_in( obj );

            if( sr.o )
            {
                tp_t t = sr_s_type( sr );
                x_stamp* v = sr.o;
                sz_t size = x_stamp_t_size( t );
                bcore_arr_st_s^ table;
                for( sz_t i = 0; i < size; i++ )
                {
                    tp_t name = x_stamp_t_name( t, i );
                    tp_t type = x_stamp_t_type( v, t, name );
                    table.push_fa( "#<sc_t>", bnameof( name ) );
                    table.push_fa( ": #<sc_t>", bnameof( type ) );
                    m st_s* st = table.push();

                    switch( type )
                    {
                        case u0_t~: case u1_t~: case u2_t~: case u3_t~:
                        case s0_t~: case s1_t~: case s2_t~: case s3_t~:
                        case bl_t~: case uz_t~: case sz_t~: case tp_t~:
                        {
                            sr_s^ sr;
                            sr = x_stamp_t_c_get_sr( v, t, name );
                            if( sr.o )
                            {
                                st.push_sc( ": " );
                                st.push_st_d( st_s_create_typed( type, sr.o ) );
                            }
                        }
                        break;

                        default: break;
                    }
                }
                table.table_to_sink( size, -1, sink );
            }
            else
            {
                sink.push_fa( "Path not found.\n" );
            }
        };
    };

    stamp :enter_s =
    {
        st_s path;
        func ::op.key = { return "n,enter"; };
        func ::op.info = { return "Enters object"; };
        func ::op.run =
        {
            x_stamp_path_s* path = x_stamp_path_s!^.parse( x_source_create_from_st( o.path )^ );
            if( path.size == 0 )
            {
                sink.push_fa( "Path '#<sc_t>' not found.\n", o.path.sc );
                return;
            }
            sr_s^ sr;
            sr = path.get_sr_in( obj );
            mutable bcore_shell* shell_o = sr.o;
            tp_t shell_t = sr.type();

            if( shell_o )
            {
                if( !x_stamp_t_is_aware( shell_t ) )
                {
                    sink.push_fa( "Selected object '#<sc_t>' is oblivious. Cannot enter.\n", bnameof( shell_t ) );
                    return;
                }

                m $* control_child = control.spawn()^;
                if( control_child.path.size ) control_child.path.push_sc( "|" );
                control_child.path.push_st( o.path );
                control_child.source =< source.fork();
                control_child.sink =< sink.fork();
                shell_o.loop( main_frame, control_child );
            }
            else
            {
                sink.push_fa( "Path '#<sc_t>' not found.\n", o.path.sc );
            }
        };
    };

    stamp :exit_s =
    {
        bl_t a = false;
        func ::op.key = { return "x,exit"; };
        func ::op.info = { return "Exits object; -a: exits all parent objects"; };
        func ::op.run = { if( o.a ) control.request_exit_all(); else control.request_exit_loop(); };
    };

    stamp :get_s =
    {
        st_s path;
        func ::op.key = { return "get"; };
        func ::op.info = { return "Outputs object as btml"; };
        func ::op.run =
        {
            sr_s sr = x_stamp_path_s!^.parse( x_source_create_from_st( o.path )^ ).get_sr_in( obj );
            if( sr.o )
            {
                x_btml_t_to_sink( sr.o, sr_s_type( sr ), sink );
            }
            else
            {
                sink.push_fa( "Path '#<sc_t>' not found.\n", o.path.sc );
            }
        };
    };

    stamp :set_s =
    {
        st_s path;
        st_s source;
        func ::op.key = { return "set"; };
        func ::op.info = { return "Sets object from btml"; };
        func ::op.run =
        {
            sr_s sr = x_stamp_path_s!^.parse( x_source_create_from_st( o.path )^ ).get_sr_in( obj );
            if( sr.o )
            {
                if( o.source.size == 0 )
                {
                    sink.push_fa( "No value specified.\n" );
                    return;
                }
                tp_t type = 0;
                m $* val = x_btml_create_from_st_t( o.source, type.1 ).t_scope( type );
                if( type )
                {
                    sr.o.t_copy_typed( sr_s_type( sr ), type, val );
                }
                else
                {
                    bcore_error_pop_all_to_sink( sink );
                    sink.push_fa( "Could not assign value.\n" );
                }
            }
            else
            {
                sink.push_fa( "Path '#<sc_t>' not found.\n", o.path.sc );
            }
        };
    };

    stamp :alias_s =
    {
        st_s key;
        st_s expression;
        func ::op.key  = { return "alias"; };
        func ::op.info = { return "Defines an alias."; };
        func ::op.run  =
        {
            if( o.key.size == 0 )
            {
                sink.push_fa( "No key defined.\n" );
                return;
            }
            tp_t tp_key = control.hmap_name.set_st_c( o.key );
            if( o.expression.size > 0 )
            {
                control.hmap_alias.set( tp_key, o.expression );
            }
            else
            {
                control.hmap_alias.remove( tp_key );
            }
        };
    };

    stamp :stdin_s =
    {
        func ::op.key = { return "stdin"; };
        func ::op.info = { return "Sets shell-control source to stdin"; };
        func ::op.run = { control.source =< x_source_stdin().fork(); };
    };

    stamp :stdout_s =
    {
        func ::op.key = { return "stdout"; };
        func ::op.info = { return "Sets shell-control sink to stdout"; };
        func ::op.run = { control.sink =< x_sink_stdout().fork(); };
    };

};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif // BCORE_SHELL_H



