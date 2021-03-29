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

/**********************************************************************************************************************/

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
        m bcore_source* source,
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
    func (bl_t parse_match( m @* o, m bcore_source* source )) =
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
    func (bl_t parse_param( m @* o, m bcore_source* source, m bcore_sink* sink )) =
    {
        while( !source.eos() && !source.parse_bl( "#?'\n'" ) )
        {
            source.parse_fa( "#skip' '" );
            m x_via* v = o;
            if( source.parse_bl( "#?([0]=='-'&&(([1]>='A'&&[1]<='Z')||([1]>='a'&&[1]<='z')))" ) )
            {
                st_s^ name;
                source.parse_fa( "-#name", &name );
                tp_t tp_name = btypeof( name.sc );
                sz_t index = 0;
                for( index = 0; index < v.size(); index++ ) if( v.name( index ) == tp_name ) break;
                if( index == v.size() )
                {
                    sink.push_fa( "Invalid parameter '#<sc_t>'\n", name.sc );
                    return false;
                }
                source.parse_fa( "#skip' '" );

                tp_t type = v.type( tp_name );

                switch( type )
                {
                    case bl_t~:
                    {
                        if( source.parse_bl( "#?'true'" ) )
                        {
                            v.set_sr( tp_name, sr_bl( true ) );
                        }
                        else if( source.parse_bl( "#?'false'" ) )
                        {
                            v.set_sr( tp_name, sr_bl( false ) );
                        }
                        else
                        {
                            v.set_sr( tp_name, sr_bl( true ) );
                        }
                    }
                    break;

                    case sz_t~:
                    case uz_t~:
                    case f3_t~:
                    {
                        f3_t v_f3 = 0;
                        source.parse_fa( "#<f3_t*>", &v_f3 );
                        v.set_sr( tp_name, sr_f3( v_f3 ) );
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
                        v.set_sr( tp_name, sr_awc( st ) );
                    }
                    break;

                    default:
                    {
                        sink.push_fa( "Parameter '#<sc_t>' is of unhandled type '#<sc_t>'.\n", name.sc, bnameof( type ) );
                        return false;
                    }
                }
            }
            else
            {
                bl_t success = false;

                /// a numeric is passed to the first numeric type
                if( source.parse_bl( "#?(([0]=='-'&&([1]>='0'||[1]<='9'))||([0]>='0'&&[0]<='9'))" ) )
                {
                    f3_t num;
                    source.parse_fa( "#<f3_t*>", num.1 );

                    for( sz_t i = 0; i < v.size(); i++ )
                    {
                        switch( v.type( v.name( i ) ) )
                        {
                            case f3_t~: case sz_t~: case uz_t~: case s3_t~: case u3_t~:
                            {
                                v.set_sr( v.name( i ), sr_f3( num ) );
                                success = true;
                            }
                            break;

                            default: break;
                        }
                    }
                }

                /// an identifier is passed to the first st_s element
                else if( source.parse_bl( "#?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z'))" ) )
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

                    for( sz_t i = 0; i < v.size(); i++ )
                    {
                        if( v.type( v.name( i ) ) == st_s~ )
                        {
                            v.set_sr( v.name( i ), sr_awc( st ) );
                            success = true;
                            break;
                        }
                    }
                }

                if( success ) return true;

                source.parse_msg_to_sink_fa( sink, "Unhandled Parameter" );
                return false;
            }
        }
        return true;
    };

    /// Sends a description of operators argument signature to sink.
    func (void arg_signature_to_sink( m @* o, m bcore_sink* sink )) =
    {
        m x_via* v = o;
        bl_t direct_num = false;
        bl_t direct_str = false;
        for( sz_t i = 0; i < v.size(); i++ )
        {
            tp_t name = v.name( i );
            tp_t type = v.type( name );
            switch( type )
            {
                case f3_t~: case sz_t~: case uz_t~: case s3_t~: case u3_t~:
                {
                    if( !direct_num )
                    {
                        sink.push_fa( "[<#<sc_t>>(number)] ", bnameof( name ) );
                        direct_num = true;
                    }
                    else
                    {
                        sink.push_fa( "[-#<sc_t> <number>] ", bnameof( name ) );
                    }
                }
                break;

                case st_s~:
                {
                    if( !direct_str )
                    {
                        sink.push_fa( "[<#<sc_t>>(text)] ", bnameof( name ) );
                        direct_str = true;
                    }
                    else
                    {
                        sink.push_fa( "[-#<sc_t> <text>] ", bnameof( name ) );
                    }
                }
                break;

                case bl_t~:
                {
                    sink.push_fa( "[-#<sc_t>] ", bnameof( name ) );
                }
                break;

                default: break;
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

    stamp :s =
    {
        bl_t exit_loop;
        bl_t exit_all;
        func :.reset             = { o.exit_loop = false; return o; };
        func :.request_exit_loop = { o.exit_loop = true; };
        func :.request_exit_all  = { o.exit_all = true; };
        func :.exit_loop         = { return o.exit_loop || o.exit_all; };
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

func (void help_to_sink( m @* o, m bcore_sink* sink )) =
{
    bcore_arr_st_s^ arr_sign;
    bcore_arr_st_s^ arr_info;
    sz_t sz_sign = 0;
    foreach( tp_t t in o.get_op_stamps()^ )
    {
        m :op* op = x_inst_t_create( t )^;
        st_s^ key;
        op.get_key( key );
        key.replace_char_char( ' ', ',' );
        st_s^ sign;
        sign.push_fa( "#<sc_t> ", key.sc );
        op.arg_signature_to_sink( sign );
        st_s^ info;
        op.get_info( info );
        arr_sign.push_st( sign );
        arr_info.push_st( info );
        sz_sign = sz_max( sz_sign, sign.size );
    }

    foreach( $* sign in arr_sign )
    {
        sink.push_fa( "#pn' '{#<sc_t>} : #<sc_t>\n", sz_sign, sign.sc, arr_info.[ __i ].sc );
    }
};

//----------------------------------------------------------------------------------------------------------------------

signature void loop
(
    m @* o,
    bcore_main_frame_s* frame,
    m bcore_source* source,
    m bcore_sink* sink,
    m bcore_shell_control_s* control,
    st_s* prompt
);

func loop =
{
    if( !control ) control = :control_s!.scope( control );
    while( !source.eos() && !control.exit_loop() )
    {
        sink.push_fa( "\n#<sc_t>(#<sc_t>)> ", prompt ? prompt.sc : "", bnameof( o._ ) ).flush();
        control.reset();

        st_s^ line;
        source.parse_fa( " #until'\n'#skip'\n'", line.1 );
        bcore_source_string_s^ line_source.setup_from_string( line );

        if( line.size > 0 )
        {
            bl_t found = false;

            foreach( tp_t t in o.get_op_stamps()^ )
            {
                m :op* op = x_inst_t_create( t )^;
                if( op.parse_match( line_source ) )
                {
                    if( op.parse_param( line_source, sink ) )
                    {
                        op.run( o, frame, source, sink, control );
                    }
                    found = true;
                    break;
                }
            }

            if( !found )
            {
                sink.push_fa( "Syntax error.\n" );
                o.help_to_sink( sink );
            }
        }
    }
};

//----------------------------------------------------------------------------------------------------------------------

group :op_default = retrievable
{
    stamp :enter_s =
    {
        st_s path;
        func ::op.key = { return "n,enter"; };
        func ::op.info = { return "enters object"; };
        func ::op.run =
        {
            sr_s sr = x_via_path_s!^.parse( bcore_source_string_s_create_from_string( o.path )^ ).get_sr_in( obj );
            if( sr.o )
            {
                tp_t t = sr_s_type( sr );
                if( !x_via_t_is_aware( t ) )
                {
                    sink.push_fa( "Selected object '#<sc_t>' is oblivious. Cannot enter.\n", bnameof( t ) );
                    return;
                }

                mutable bcore_shell* shell_o = sr.o;
                shell_o.loop( main_frame, source, sink, control, o.path );
            }
            else
            {
                sink.push_fa( "Path not found.\n" );
            }
        };
    };

    stamp :exit_s =
    {
        bl_t a;
        func ::op.key = { return "x,exit"; };
        func ::op.info = { return "exits object; -a: exits all parent objects"; };
        func ::op.run = { if( o.a ) control.request_exit_all(); else control.request_exit_loop(); };
    };

    stamp :help_s =
    {
        func ::op.key = { return "?,help"; };
        func ::op.info = { return "lists available commands"; };
        func ::op.run = { obj.help_to_sink( sink ); };
    };

    stamp :ls_s =
    {
        bl_t l = false;
        st_s path;

        func ::op.get_key = { key.push_fa( "ls" ); };
        func ::op.get_info = { info.push_fa( "lists all visible members; -l: txt_ml to stdout" ); };
        func ::op.run =
        {
            bl_t extensive = o.l;

            x_via_path_s^ path.parse( bcore_source_string_s_create_sc( o.path.sc )^ );
            sr_s sr = path.get_sr_in( obj );

            if( sr.o )
            {
                if( extensive )
                {
                    x_inst_t_to_sink_txt_ml( sr_s_type( sr ), sr.o, sink );
                }
                else
                {
                    tp_t t = sr_s_type( sr );
                    x_via* v = sr.o;
                    sz_t size = x_via_t_size( t );
                    for( sz_t i = 0; i < size; i++ )
                    {
                        tp_t name = x_via_t_name( t, i );
                        tp_t type = x_via_t_type( t, v, name );
                        sink.push_fa( "#<sc_t>: #<sc_t>\n", bnameof( name ), bnameof( type ) );
                    }
                }
            }
            else
            {
                sink.push_fa( "Path not found.\n" );
            }
        };
    };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


