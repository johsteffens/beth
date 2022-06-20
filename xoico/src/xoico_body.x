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

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

signature er_t parse_expression      ( m @* o, c xoico_host* host, m x_source* source );
signature er_t parse_single_statement( m @* o, c xoico_host* host, m x_source* source );
signature er_t finalize( m @* o, c xoico_host* host );
signature er_t expand( c @* o, c xoico_host* host, c xoico_signature_s* signature, sz_t indent, m x_sink* sink );

//----------------------------------------------------------------------------------------------------------------------

stamp :code_s = aware :
{
    bl_t single_line;
    tp_t hash_source;

    x_source_point_s source_point;

    func xoico.parse;
    func     :.parse_single_statement;
    func xoico.get_hash;
    func xoico.get_source_point { return o.source_point; };
};

//----------------------------------------------------------------------------------------------------------------------

stamp :s = aware :
{
    tp_t name;
    tp_t global_name;

    :code_s => code;

    bl_t go_inline;

    x_source_point_s source_point;

    func xoico.get_hash;
    func xoico.get_global_name_tp { return o.global_name; };
    func     :.parse_expression;
    func     xoico.parse;
    func     :.finalize { return 0; };
    func     :.expand;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:code_s) xoico.parse
{
    tp_t hash = bcore_tp_init();

    o.source_point.setup_from_source( source );

    if( source.parse_bl( " #?'('" ) )
    {
        while( !source.eos() && source.inspect_char() != ')' ) hash = bcore_tp_fold_u0( hash, source.get_char() );
        source.parse_fa( ")" );
    }

    source.parse_fa( " {" );

    sz_t nest_count = 1;
    bl_t exit_loop = false;
    o.single_line = true;

    while( source.parse_bl( "#?' '" ) ); // skip leading spaces

    while( !source.eos() && !exit_loop )
    {
        u0_t c =  source.get_u0();
        switch( c )
        {
            case '{':
            {
                nest_count++;
            }
            break;

            case '}':
            {
                nest_count--;
                if( !nest_count )
                {
                    c = 0;
                    exit_loop = true;
                }
            }
            break;

            case '"': // string literal
            {
                hash = bcore_tp_fold_u0( hash, c );
                while( !source.eos() && ((c = source.get_char()) != '"') )
                {
                    hash = bcore_tp_fold_u0( hash, c );
                    if( c == '\\' ) hash = bcore_tp_fold_u0( hash, source.get_u0() );
                    if( c == '\n' ) return source.parse_error_fa( "Newline in string literal." );
                }
                c = 0;
            }
            break;

            case '\'': // char literal
            {
                hash = bcore_tp_fold_u0( hash, c );
                while( !source.eos() && ((c = source.get_char()) != '\'') )
                {
                    hash = bcore_tp_fold_u0( hash, c );
                    if( c == '\\' ) hash = bcore_tp_fold_u0( hash, source.get_u0() );
                    if( c == '\n' ) return source.parse_error_fa( "Newline in char literal." );
                }
                c = 0;
            }
            break;

            case '/': // comment
            {
                hash = bcore_tp_fold_u0( hash, c );
                if( source.inspect_char() == '/' )
                {
                    while( !source.eos() )
                    {
                        c = source.get_char();
                        hash = bcore_tp_fold_u0( hash, c );
                        if( c == '\n' )
                        {
                            o.single_line = false;
                            break;
                        }
                    }
                }
                else if( source.inspect_char() == '*' )
                {
                    while( !source.eos() )
                    {
                        if( source.parse_bl( "#?'*/'" ) )
                        {
                            hash = bcore_tp_fold_sc( hash, "*/" );
                            break;
                        }
                        else
                        {
                            c = source.get_char();
                            if( c == '\n' ) o.single_line = false;
                            hash = bcore_tp_fold_u0( hash, c );
                        }
                    }
                }
                c = 0;
            }
            break;

            case '\n' :
            {
                o.single_line = false;
                break;
            }

            default: break;
        }
        hash = bcore_tp_fold_u0( hash, c );
    }

    o.hash_source = hash;
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:code_s) :.parse_single_statement
{
    tp_t hash = bcore_tp_init();

    o.source_point.setup_from_source( source );

    sz_t nest_count = 0;
    bl_t exit_loop = false;
    o.single_line = true;

    while( source.parse_bl( "#?' '" ) ); // skip leading spaces

    while( !source.eos() && !exit_loop )
    {
        u0_t c =  source.get_u0();
        switch( c )
        {
            case '{':
            case '}':
                return source.parse_error_fa( "Braces are not allowed in single statement." );
            break;

            case '(':
            {
                nest_count++;
            }
            break;

            case ')':
            {
                if( nest_count == 0 )
                {
                    return source.parse_error_fa( "Unmatched closing bracket ')'." );
                }
                nest_count--;
            }
            break;

            case ';':
            {
                if( nest_count == 0 ) exit_loop = true;
            }
            break;

            case '"': // string literal
            {
                hash = bcore_tp_fold_u0( hash, c );
                while( !source.eos() && ((c = source.get_char()) != '"') )
                {
                    hash = bcore_tp_fold_u0( hash, c );
                    if( c == '\\' ) hash = bcore_tp_fold_u0( hash, source.get_u0() );
                    if( c == '\n' ) return source.parse_error_fa( "Newline in string literal." );
                }
                c = 0;
            }
            break;

            case '\'': // char literal
            {
                hash = bcore_tp_fold_u0( hash, c );
                while( !source.eos() && ((c = source.get_char()) != '\'') )
                {
                    hash = bcore_tp_fold_u0( hash, c );
                    if( c == '\\' ) hash = bcore_tp_fold_u0( hash, source.get_u0() );
                    if( c == '\n' ) return source.parse_error_fa( "Newline in char literal." );
                }
                c = 0;
            }
            break;

            case '/': // comment
            {
                hash = bcore_tp_fold_u0( hash, c );
                if( source.inspect_char() == '/' )
                {
                    while( !source.eos() )
                    {
                        c = source.get_char();
                        hash = bcore_tp_fold_u0( hash, c );
                        if( c == '\n' )
                        {
                            o.single_line = false;
                            break;
                        }
                    }
                }
                else if( source.inspect_char() == '*' )
                {
                    while( !source.eos() )
                    {
                        if( source.parse_bl( "#?'*/'" ) )
                        {
                            hash = bcore_tp_fold_sc( hash, "*/" );
                            break;
                        }
                        else
                        {
                            c = source.get_char();
                            if( c == '\n' ) o.single_line = false;
                            hash = bcore_tp_fold_u0( hash, c );
                        }
                    }
                }
                c = 0;
            }
            break;

            case '\n' :
            {
                o.single_line = false;
                break;
            }

            default: break;
        }
        hash = bcore_tp_fold_u0( hash, c );
    }

    o.hash_source = hash;
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:code_s) xoico.get_hash
{
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o._ );
    hash = bcore_tp_fold_tp( hash, o.hash_source );
    return hash;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.get_hash
{
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o._ );
    hash = bcore_tp_fold_tp( hash, o.name );
    if( o.code ) hash = bcore_tp_fold_tp( hash, o.code.get_hash() );
    hash = bcore_tp_fold_bl( hash, o.go_inline );
    return hash;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.parse_expression
{
    if( source.parse_bl( " #=?'{'" ) )
    {
        o.code =< xoico_body_code_s!;
        o.code.parse( host, source );
        o.go_inline = o.code.single_line;
    }
    else
    {
        s3_t index = source.get_index();

        m $* compiler = host.compiler();

        st_s^ st_name;
        host.parse_name_st( source, st_name );
        tp_t tp_name = btypeof( st_name.sc );

        // if name_buf refers to another body
        if( compiler.is_body( tp_name ) )
        {
            c $* body = compiler.get_body( tp_name );
            o.code =< body.code.clone();
            o.go_inline = body.go_inline;
        }
        else // at this point we assume a single statement body
        {
            source.set_index( index );
            o.code =< xoico_body_code_s!;
            o.code.parse_single_statement( host, source );
            o.go_inline = o.code.single_line;
        }
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.parse
{
    o.source_point.setup_from_source( source );

    if( source.parse_bl( " #=?'='" ) )
    {
        s3_t index = source.get_index();
        if( source.parse_bl( "= #?'{'" ) )
        {
            return source.parse_error_fa( "Explicit block assignment '=' is deprecated. Remove assignment operator.\n" );
        }
        else // assignment can be used as completion in a single-statement body
        {
            source.set_index( index );
        }
    }

    o.parse_expression( host, source );

    m $* compiler = host.compiler();
    o.global_name = host.entypeof( st_s_create_fa( "#<sc_t>_#<sc_t>", compiler.nameof( host.obj_type() ), compiler.nameof( o.name ) )^^.sc );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.expand
{
    c st_s* final_code = NULL;
    m st_s* st_out = st_s!^^;

    m xoico_cengine* cengine = host.cengine();
    ASSERT( cengine );

    if( o.code )
    {
        cengine.translate( host, o, signature, ( x_sink* )st_out );
        final_code = st_out;
    }

    if( o.go_inline )
    {
        sink.push_fa( "{#<sc_t>}", final_code.sc );
    }
    else
    {
        sink.push_fa( "{\n#rn{ }", indent + 4 );
        for( sz_t i = 0; i < final_code.size; i++ )
        {
            char c = final_code.sc[ i ];
            sink.push_char( c );
            if( c == '\n' ) sink.push_fa( "#rn{ }", indent + 4 );
        }
        sink.push_fa( "\n#rn{ }}", indent );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
