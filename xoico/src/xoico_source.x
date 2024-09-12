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

/// parse function for different embedding methods
signature er_t parse( m @* o, c xoico_host* host, m x_source* source, sc_t group_name, sc_t trait_name, tp_t embed_method );


/// retrieves group if already defined (e.g. for extending); sets arg_group.1 NULL if not defined
signature er_t get_group_if_preexsting( m @* o, xoico_host* host, m x_source* source, sc_t group_name, sc_t trait_name, m xoico_group_s.2 group );

//----------------------------------------------------------------------------------------------------------------------

stamp :s = aware :
{
    st_s full_path; // full file path (including extension)
    st_s name; // file name excluding directory and extension
    st_s ext;  // file extension ( "h" or "x" )
    xoico_group_s => [];

    // source file represents plain data to be embedded in target
    bcore_arr_u0_s => plain_data;
    bl_t embed_plain_data_as_string_function; // embed plain data as string function

    hidden aware xoico_target_s* target;

    func xoico.expand_setup
    {
        foreach( m $* e in o ) e.expand_setup( o );
        return 0;
    };

    func m xoico_group_s* push_d( m @* o, d xoico_group_s* group )
    {
        return o.cast( m x_array* ).push_d( group );
    };

    func xoico.get_hash
    {
        tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
        foreach( m xoico_group_s* e in o ) hash = bcore_tp_fold_tp( hash, e.get_hash() );
        return hash;
    };

    func :.parse;

    func xoico.finalize
    {
        foreach( m $* e in o ) e.finalize( o );
        return 0;
    };

    func xoico.expand_declaration
    {
        if( o.size > 0 )
        {
            sink.push_fa( "\n" );
            sink.push_fa( "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
            sink.push_fa( "#rn{ }// source: #<sc_t>.#<sc_t>\n", indent, o.name.sc, o.ext.sc );
            foreach( m $* e in o ) e.expand_declaration( indent, sink );
        }
        return 0;
    };

    func xoico.expand_definition
    {
        if( o.size > 0 || o.ext.equal_sc( "h" ) )
        {
            sink.push_fa( "\n" );
            sink.push_fa( "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
            sink.push_fa( "#rn{ }// source: #<sc_t>.#<sc_t>\n", indent, o.name.sc, o.ext.sc );

            if( o.ext.equal_sc( "h" ) )
            {
                sink.push_fa( "#rn{ }##include \"#<sc_t>.#<sc_t>\"\n", indent, o.name.sc, o.ext.sc );
            }
            foreach( m $* e in o ) e.expand_definition( indent, sink );
        }

        if( o.embed_plain_data_as_string_function )
        {
            sink.push_fa( "\n" );
            sink.push_fa( "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
            sink.push_fa( "#rn{ }// source: #<sc_t>.#<sc_t>\n", indent, o.name.sc, o.ext.sc );
            sink.push_fa( "#rn{ }// Embedded as string function.\n", indent );

            st_s^ func_global_name.push_fa( "#<sc_t>_#<sc_t>", o.name.sc, o.ext.sc );
            func_global_name.replace_sc_sc( ".", "_" );

            sink.push_fa( "#rn{ }st_s* #<sc_t>( void )\n", indent, func_global_name.sc );
            sink.push_fa( "#rn{ }{\n"                            , indent );

            sink.push_fa( "#rn{ }    sc_t sc[] =\n", indent );
            sink.push_fa( "#rn{ }    {\n" , indent );
            sink.push_fa( "#rn{ }        \"" , indent );

            sz_t lines = 0;

            for( sz_t i = 0; i < o.plain_data.size; i++ )
            {
                u0_t c = o.plain_data.[ i ];
                switch( c )
                {
                    case '\\': sink.push_sc( "\\\\" ); break;
                    case '"' : sink.push_sc( "\\\"" ); break;
                    case '\n':
                    {
                        if( i + 1 < o.plain_data.size )
                        {
                            sink.push_fa( "\\n\",\n" );
                            sink.push_fa( "#rn{ }        \"" , indent );
                            lines++;
                        }
                    }
                    break;

                    default: sink.push_char( c );  break;
                }
            }
            sink.push_fa( "\\n\"\n" );
            sink.push_fa( "#rn{ }    };\n", indent );
            lines++;

            sink.push_fa( "#rn{ }    st_s* st = st_s_create();\n", indent );
            sink.push_fa( "#rn{ }    for( sz_t i = 0; i < #<sz_t>; i++ )\n", indent, lines );
            sink.push_fa( "#rn{ }    {\n", indent );
            sink.push_fa( "#rn{ }        st_s_push_sc( st, sc[ i ] );\n", indent );
            sink.push_fa( "#rn{ }    }\n", indent );
            sink.push_fa( "#rn{ }    return st;\n", indent, func_global_name.sc );
            sink.push_fa( "#rn{ }}\n", indent, func_global_name.sc );
        }

        return 0;
    };

    func xoico.expand_init1
    {
        if( o.size > 0 )
        {
            sink.push_fa( "\n" );
            sink.push_fa( "#rn{ }// #rn{-}\n", indent, sz_max( 0, 80 - indent ) );
            sink.push_fa( "#rn{ }// source: #<sc_t>.#<sc_t>\n", indent, o.name.sc, o.ext.sc );
            foreach( m $* e in o ) e.expand_init1( indent, sink );
        }
        return 0;
    };

    func xoico.expand_down1
    {
        if( o.size > 0 )
        {
            // there is hardly any code in the down1-handler, so for the moment we need omit commenting the sources
//            sink.push_fa( "\n" );
//            sink.push_fa( "#rn{ }// #rn{-}\n", indent, sz_max( 0, 80 - indent ) );
//            sink.push_fa( "#rn{ }// source: #<sc_t>.#<sc_t>\n", indent, o.name.sc, o.ext.sc );
            foreach( m $* e in o ) e.expand_down1( indent, sink );
        }
        return 0;
    };

    func xoico.expand_manifesto
    {
        foreach( m $* e in o ) e.expand_manifesto( host, indent, sink );
        return 0;
    };

    func xoico_group.explicit_embeddings_push
    {
        foreach( m $* group in o ) group.explicit_embeddings_push( arr );
    };

    func xoico_host.compiler
    {
        return o.target.compiler;
    };

    func :.get_group_if_preexsting;
};

//----------------------------------------------------------------------------------------------------------------------

/// Returns NULL in case of no match;
func (:s) get_group_if_preexsting
{
    m xoico_compiler_s* compiler = host.compiler();
    if( compiler.is_group( host.entypeof( group_name ) ) )
    {
        group.1 = compiler.get_group( host.entypeof( group_name ) );

        // Extending a preexisting group is only allowed inside the same target
        if( group.xoico_source.target != o.target )
        {
            return source.parse_error_fa( "Group '#<sc_t>' was defined in target '#<sc_t>'. It cannot be extended in a different target.", group_name, group.xoico_source.target.name.sc );
        }

        if( group.trait_name != compiler.entypeof( trait_name ) )
        {
            return source.parse_error_fa( "Extending group '#<sc_t>' of trait '#<sc_t>' with a different trait '#<sc_t>'.", group_name, host.nameof( group.trait_name ), trait_name );
        }

    }
    else
    {
        group.1 = NULL;
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) parse
{
    m $* compiler = o.target.compiler;

    if( embed_method == TYPEOF_as_header )
    {
        while( !source.eos() )
        {
            if( source.parse_bl( " #?w'XOILA_DEFINE_GROUP'" ) )
            {
                st_s^ st_trait_name;
                st_s^ st_group_name;
                source.parse_fa( " ( #name , #name", st_group_name.1, st_trait_name.1 );
                m xoico_group_s* group = NULL;
                o.get_group_if_preexsting( host, source, st_group_name.sc, st_trait_name.sc, group.2 );
                if( !group )
                {
                    group = o.push_d( xoico_group_s! );
                    group.xoico_source = o;
                    group.compiler = compiler;
                    group.set_name_sc( host, st_group_name.sc );
                    group.trait_name = compiler.entypeof( st_trait_name.sc );
                }

                if( source.parse_bl( " #=?','" ) )
                {
                    while( source.parse_bl( " #?','" ) )
                    {
                        m st_s* embed_file = st_s!^;
                        source.parse_fa( " #string", embed_file );
                        d x_source* embed_source = NULL;
                        xoico_embed_file_open( source, embed_file.sc, embed_source.2 );
                        embed_source^^;
                        group.explicit_embeddings.push_st( embed_file );
                        group.parse( o, false, embed_source );
                    }
                    source.parse_fa( " )" );
                }
                else
                {
                    source.parse_fa( " )" );
                    group.parse( o, false, source );
                }
                o.target.compiler.register_group( group );
            }
            else
            {
                source.get_u0();
            }
        }
    }
    else
    {
        m xoico_group_s* group = NULL;

        o.get_group_if_preexsting( host, source, group_name, trait_name, group.2 );
        if( group )
        {
            if( group.xoico_source != o )
            {
                group.explicit_embeddings.push_st( st_s_create_fa( "#<sc_t>.#<sc_t>", o.name.sc, o.ext.sc )^ );
            }
        }
        else
        {
            group = o.push_d( xoico_group_s! );
            group.xoico_source = o;
            group.compiler = compiler;
            group.set_name_sc( host, group_name );
            group.trait_name = compiler.entypeof( trait_name );
            group.is_manifesto = true;
        }

        if( embed_method == TYPEOF_as_group )
        {
            group.parse( o, false, source );
        }
        else
        {
            group.parse_embed( o, source, embed_method );
        }

        compiler.register_group( group );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
//
