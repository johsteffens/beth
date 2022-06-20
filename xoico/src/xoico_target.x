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

signature er_t parse_from_path( m @* o, sc_t source_path, sc_t group_name /* can be NULL */, sc_t trait_name /* can be NULL */ );
signature bl_t to_be_modified( c @* o );
signature er_t expand_phase1( m @* o, m bl_t* p_modified );
signature er_t expand_phase2( m @* o, m bl_t* p_modified );
signature bl_t is_cyclic( m @* o ); // mutable because flag is used for cyclic test
signature er_t set_dependencies(  m @* o, c bcore_arr_sz_s* dependencies );
signature er_t set_main_function( m @* o, c xoico_func_s* func );

stamp :s = aware :
{
    st_s name; // target name (e.g. "bcore")
    st_s ext;  // target extension (e.g. "xo")

    /// parameters

    /** copyright_and_licence_terms: If specified, the associated *.xo.* files carry this text
     *  in the appropriate location for the license preamble.
     */
    st_s => copyright_and_license_terms;

    /// prevents body update if body signature is unchanged
    bl_t update_target_on_body_signature = true;

    st_s include_path; // (local) path used in generated '#include' directives
    st_s output_path; // full path excluding extension *.h *.c *.state or output files
    xoico_source_s => [];

    st_s signal_handler_name;   // name of governing signal handler
    bl_t define_signal_handler; // implements <name>_general_signal_handler in xo.c

    bcore_arr_sz_s dependencies; // index array to dependent targets
    bl_t flag; // general purpose flag
    bl_t modified;    // target is to be modified
    bl_t readonly;    // target is readonly (affects writing in phase2)
    st_s => target_h; // target header file
    st_s => target_c; // target c file
    st_s => target_state; // target state file

    /** The body signature is a signature of the effective c-code (and certain comments)
     *  It is used to prevent updating *.xo files when the effective code has not changed.
     */
    tp_t body_signature_h; // body signature for output file
    tp_t body_signature_c; // body signature for output file

    tp_t pre_hash;

    /// Optional cengine that is to be used in all bodies of this target
    aware xoico_cengine -> cengine;

    hidden aware xoico_compiler_s* compiler;
    hidden xoico_func_s* main_function;

    func :.parse_from_path;
    func :.to_be_modified;

    func xoico.finalize
    {
        foreach( m $* e in o ) e.finalize( o );
        return 0;
    };

    func xoico.expand_setup
    {
        foreach( m $* e in o ) e.expand_setup( o );
        return 0;
    };

    func :.expand_phase1;
    func :.expand_phase2;
    func :.is_cyclic;
    func :.set_dependencies;
    func :.set_main_function
    {
        if( o.compiler.has_main_function ) return func.source_point.parse_error_fa( "A main function was already declared." );
        o.compiler.has_main_function = true;
        o.main_function = func.cast( m$* );
        return 0;
    };

    func void push_d( m @* o, d xoico_source_s* source )
    {
        o.cast( m x_array* ).push_d( source );
    };

    func xoico_group.explicit_embeddings_push
    {
        foreach( m $* source in o ) source.explicit_embeddings_push( arr );
    };

    func xoico_host.compiler { return o.compiler; };
    func xoico_host.cengine { return o.cengine; };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.parse_from_path
{
    m st_s* source_name        = bcore_file_strip_extension( bcore_file_name( source_path ) )^^;
    m st_s* source_folder_path = bcore_file_folder_path( source_path )^^;
    m st_s* source_path_n      = st_s_create_fa( "#<sc_t>/#<sc_t>", source_folder_path.sc, source_name.sc )^^;

    bl_t source_exists = false;

    foreach( m $* e in o )
    {
        if( source_path_n.equal_st( e.path ) )
        {
            source_exists = true;
            break;
        }
    }

    if( !source_exists )
    {
        m xoico_source_s* xsource = xoico_source_s!^^;
        xsource.target = o;

        xsource.name.copy_sc( source_name.sc );
        xsource.path.copy( source_path_n );
        xsource.ext.copy_sc( bcore_file_extension( source_path ) );

        if( bcore_file_exists( source_path ) )
        {
            if( group_name )
            {
                xsource.parse_x( o, bcore_file_open_source( source_path )^^, group_name, trait_name );
            }
            else
            {
                xsource.parse_h( o, bcore_file_open_source( source_path )^^ );
            }
        }
        else
        {
            return bcore_error_push_fa( general_error~, "Could not open '#<sc_t>'.", source_path );
        }

        o.push_d( xsource.fork() );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) tp_t get_hash( c @* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_tp( hash, o.pre_hash );
    hash = bcore_tp_fold_tp( hash, o.compiler.target_pre_hash );
    hash = bcore_tp_fold_tp( hash, o._ );
    hash = bcore_tp_fold_sc( hash, o.name.sc );
    hash = bcore_tp_fold_sc( hash, o.ext.sc );
    hash = bcore_tp_fold_bl( hash, o.define_signal_handler );

    if( o.cengine ) hash = bcore_tp_fold_tp( hash, o.cengine.get_hash() );
    if( o.copyright_and_license_terms )  hash = bcore_tp_fold_sc( hash, o.copyright_and_license_terms.sc );

    foreach( m $* e in o ) hash = bcore_tp_fold_tp( hash, e.get_hash() );

    if( o.dependencies.size > 0 )
    {
        m $* arr_tp = bcore_arr_tp_s!^^;

        foreach( $ target_idx in o.dependencies ) arr_tp.push( o.compiler.[ target_idx ].get_hash() );

        // we sort to make the accrued hash independent of the order of dependencies
        arr_tp.sort( 1 );

        foreach( $ tp in arr_tp ) hash = bcore_tp_fold_tp( hash, tp );
    }

    return hash;
};

//----------------------------------------------------------------------------------------------------------------------

/// returns true if target's dependencies are cyclic
func (:s) bl_t is_cyclic_recursive( m @* o )
{
    if( o.flag ) return true;
    o.flag = true;
    foreach( sz_t target_idx in o.dependencies ) if( o.compiler.[ target_idx ].is_cyclic_recursive() ) return true;
    o.flag = false;
    return false;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.is_cyclic
{
    o.compiler.clear_flags();
    bl_t cyclic = o.is_cyclic_recursive();
    o.compiler.clear_flags();
    return cyclic;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.set_dependencies
{
    sz_t targets = o.compiler.size;

    /// sort, remove duplicates, copy
    m bcore_arr_sz_s* dst = o.dependencies;
    dst.set_size( 0 );
    m bcore_arr_sz_s* src = dependencies.clone()^^.sort( 1 );
    for( sz_t i = 0; i < src.size; i++ )
    {
        if( i == 0 || src.[ i ] != src.[ i - 1 ] )
        {
            sz_t idx = src.[ i ];
            ASSERT( idx >= 0 && idx < targets );
            dst.push( idx );
        }
    }

    if( o.is_cyclic() )
    {
        return bcore_error_push_fa( general_error~, "Cyclic dependencies found in target '#<sc_t>'.", o.name.sc );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_update_time( c @* o, sz_t indent, m x_sink* sink )
{
    m bcore_cday_utc_s* time = bcore_cday_utc_s!^^;
    bcore_cday_utc_s_from_system( time );
    sink.push_fa( "//  Last update: " ); bcore_cday_utc_s_to_sink( time, ( bcore_sink* )sink ); sink.push_fa( "\n" );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_heading( c @* o, sz_t indent, m x_sink* sink )
{
    sink.push_fa( "/** This file was generated from xoila source code.\n" );
    sink.push_fa( " *  Compiling Agent : XOICO (C) 2020 ... 2022 J.B.Steffens\n" );
    sink.push_fa( " *  Note that any changes of this file can be erased or overwritten by XOICO.\n" );
    sink.push_fa( " *\n" );

    if( o.copyright_and_license_terms )
    {
        st_s^ st.copy( o.copyright_and_license_terms );
        st.replace_sc_sc( "\n", " \n *  " );
        sink.push_fa( " *  #<st_s*>\n", st.1 );
    }
    else
    {
        sink.push_fa( " *  Copyright and License of this File:\n" );
        sink.push_fa( " *\n" );
        sink.push_fa( " *  Unless explicitly stated otherwise in governing license terms, this file inherits the\n" );
        sink.push_fa( " *  copyright and license terms of the immediate source code from which it was compiled.\n" );
        sink.push_fa( " *\n" );
    }

    sink.push_fa( " *  The immediate source code is distributed across following files:\n" );

    foreach( m $* e in o ) sink.push_fa( " *  #<sc_t>.#<sc_t>\n", e.name.sc, e.ext.sc );

    {
        m $* arr = bcore_arr_st_s!^^;
        o.explicit_embeddings_push( arr );
        arr.sort( 1 );
        if( arr.size > 0 )
        {
            m st_s* prev_file = NULL;
            foreach( m st_s* file in arr )
            {
                if( !file.equal_st( prev_file ) ) sink.push_fa( " *  #<sc_t>\n", file.sc );
                prev_file = file;
            }
        }
    }


    sink.push_fa( " *\n" );
    sink.push_fa( " */\n" );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_h( c @* o, sz_t indent, m x_sink* sink, mutable tp_t* body_signature )
{
    o.expand_update_time( indent, sink );

    st_s^ sink_buf;

    o.expand_heading( indent, sink );
    sink_buf.push_fa( "\n" );
    sink_buf.push_fa( "#rn{ }##ifndef __#<sc_t>_#<sc_t>_H\n", indent, o.name.sc, o.ext.sc );
    sink_buf.push_fa( "#rn{ }##define __#<sc_t>_#<sc_t>_H\n", indent, o.name.sc, o.ext.sc );

    sink_buf.push_fa( "\n" );
    sink_buf.push_fa( "#rn{ }##include \"bcore_control.h\"\n", indent );
    sink_buf.push_fa( "#rn{ }##include \"bcore_xoila.h\"\n", indent );

    /// include generated headers this target depends on
    foreach( sz_t target_idx in o.dependencies )
    {
        c xoico_target_s* target = o.compiler.data[ target_idx ];
        sink_buf.push_fa( "#rn{ }##include \"#<sc_t>.h\"\n", indent, target.include_path.sc );
    }

    sink_buf.push_fa( "\n" );

    sink_buf.push_fa( "#rn{ }##define TYPEOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, o.name.sc, btypeof( o.name.sc ) );

    foreach( m $* e in o ) e.expand_declaration( o, indent, sink_buf );

    sink_buf.push_fa( "\n" );
    sink_buf.push_fa( "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    sink_buf.push_fa( "\n" );
    sink_buf.push_fa( "#rn{ }vd_t #<sc_t>_#<sc_t>_signal_handler( const bcore_signal_s* o );\n", indent, o.name.sc, o.ext.sc );

    sink_buf.push_fa( "\n" );
    sink_buf.push_fa( "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    sink_buf.push_fa( "// Manifesto\n" );
    sink_buf.push_fa( "\n" );
    foreach( m $* e in o ) e.expand_manifesto( o, indent, sink_buf );

    sink_buf.push_fa( "\n" );
    sink_buf.push_fa( "#rn{ }##endif // __#<sc_t>_#<sc_t>_H\n", indent, o.name.sc, o.ext.sc );

    sink.push_data( ( vc_t )sink_buf.sc, sink_buf.size );

    tp_t body_hash = bcore_tp_fold_sc( bcore_tp_init(), sink_buf.sc );
    sink.push_fa( "// XOICO_BODY_SIGNATURE 0x#pl16'0'{#X<tp_t>}\n", body_hash );
    if( body_signature ) body_signature.0 = body_hash;

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_init1( c @* o, sz_t indent, m x_sink* sink )
{
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_c( c @* o, sz_t indent, m x_sink* sink, mutable tp_t* body_signature )
{
    st_s^ sink_buf;
    tp_t body_hash = bcore_tp_init();

    o.expand_update_time( indent, sink );

    o.expand_heading( indent, sink_buf );

    sink_buf.push_fa( "\n" );
    sink_buf.push_fa( "#rn{ }##include \"#<sc_t>.h\"\n", indent, o.include_path.sc );
    sink_buf.push_fa( "#rn{ }##include \"bcore_spect.h\"\n", indent );
    sink_buf.push_fa( "#rn{ }##include \"bcore_spect_inst.h\"\n", indent );
    sink_buf.push_fa( "#rn{ }##include \"bcore_sr.h\"\n", indent );
    sink_buf.push_fa( "#rn{ }##include \"bcore_const_manager.h\"\n", indent );
    sink_buf.push_fa( "\n" );

    body_hash = bcore_tp_fold_sc( body_hash, sink_buf.sc );
    sink.push_data( ( vc_t )sink_buf.sc, sink_buf.size );
    sink_buf.clear();

    sink.push_fa( "#rn{ }// To force a rebuild of this target by xoico, reset the hash key value below to 0.\n", indent );
    sink.push_fa( "#rn{ }// HKEYOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, o.name.sc, o.get_hash() );

    /// definition section
    foreach( m $* e in o ) e.expand_definition( o, indent, sink_buf );

    /// signal section
    sink_buf.push_fa( "\n" );
    sink_buf.push_fa( "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    sink_buf.push_fa( "\n" );

    /// prototypes of signal handlers this target depends on
    foreach( sz_t target_idx in o.dependencies )
    {
        c xoico_target_s* target = o.compiler.data[ target_idx ];
        if( target.signal_handler_name.size > 0 )
        {
            sink_buf.push_fa( "#rn{ }vd_t #<sc_t>( const bcore_signal_s* o );\n", indent, target.signal_handler_name.sc );
        }
    }

    sink_buf.push_fa( "\n" );
    sink_buf.push_fa( "#rn{ }vd_t #<sc_t>_#<sc_t>_signal_handler( const bcore_signal_s* o )\n", indent, o.name.sc, o.ext.sc );
    sink_buf.push_fa( "#rn{ }{\n", indent );
    sink_buf.push_fa( "#rn{ }    switch( bcore_signal_s_handle_type( o, typeof( \"#<sc_t>_#<sc_t>\" ) ) )\n", indent, o.name.sc, o.ext.sc );
    sink_buf.push_fa( "#rn{ }    {\n", indent );

    sink_buf.push_fa( "#rn{ }        case TYPEOF_init1:\n", indent );
    sink_buf.push_fa( "#rn{ }        {\n", indent );
    o.expand_init1( indent + 12, sink_buf );
    foreach( m $* e in o ) e.expand_init1( o, indent + 12, sink_buf );
    sink_buf.push_fa( "#rn{ }        }\n", indent );
    sink_buf.push_fa( "#rn{ }        break;\n", indent );

    if( o.dependencies.size > 0 )
    {
        sink_buf.push_fa( "#rn{ }        case TYPEOF_push_dependencies:\n", indent );
        sink_buf.push_fa( "#rn{ }        {\n", indent );
        sink_buf.push_fa( "#rn{ }            ASSERT( o->object && ( *( aware_t* )o->object ) == TYPEOF_bcore_arr_fp_s );\n", indent );
        sink_buf.push_fa( "#rn{ }            bcore_arr_fp_s* arr_fp = o->object;\n", indent );
        foreach( sz_t target_idx in o.dependencies )
        {
            c xoico_target_s* target = o.compiler.[ target_idx ];
            if( target.signal_handler_name.size > 0 )
            {
                sink_buf.push_fa( "#rn{ }            bcore_arr_fp_s_push( arr_fp, ( fp_t )#<sc_t> );\n", indent, target.signal_handler_name.sc );
            }
        }
        sink_buf.push_fa( "#rn{ }        }\n", indent );
        sink_buf.push_fa( "#rn{ }        break;\n", indent );
    }

    sink_buf.push_fa( "#rn{ }        default: break;\n", indent );
    sink_buf.push_fa( "#rn{ }    }\n", indent );
    sink_buf.push_fa( "#rn{ }    return NULL;\n", indent );
    sink_buf.push_fa( "#rn{ }}\n", indent );

    if( o.define_signal_handler )
    {
        sink_buf.push_fa( "#rn{ }BETH_SIGNAL_DEFINE( #<sc_t> )\n", indent, o.name.sc );
    }

    if( o.main_function )
    {
        bl_t c_style = ( o.main_function.signature.args.size == 2 );

        sink_buf.push_fa( "\n" );
        sink_buf.push_fa( "#rn{ }int main( int argc, char** argv )\n", indent );
        sink_buf.push_fa( "#rn{ }{\n", indent );
        sink_buf.push_fa( "#rn{ }    BETH_USE( #<sc_t> );\n", indent, o.name.sc );


        if( c_style )
        {
            sink_buf.push_fa( "#rn{ }    int retv = #<sc_t>( argc, argv );\n", indent, o.compiler.nameof( o.main_function.global_name ) );
        }
        else
        {
            sink_buf.push_fa( "#rn{ }    bcore_arr_st_s* args = bcore_arr_st_s_create();\n", indent );
            sink_buf.push_fa( "#rn{ }    for( sz_t i = 0; i < argc; i++ ) bcore_arr_st_s_push_sc( args, argv[ i ] );\n", indent );
            sink_buf.push_fa( "#rn{ }    int retv = #<sc_t>( args );\n", indent, o.compiler.nameof( o.main_function.global_name ) );
            sink_buf.push_fa( "#rn{ }    bcore_arr_st_s_discard( args );\n", indent );
        }

        sink_buf.push_fa( "#rn{ }    BETH_CLOSEV( 0 );\n", indent );
        sink_buf.push_fa( "#rn{ }    return retv;\n", indent );
        sink_buf.push_fa( "#rn{ }}\n", indent );
    }

    body_hash = bcore_tp_fold_sc( body_hash, sink_buf.sc );
    sink.push_data( ( vc_t )sink_buf.sc, sink_buf.size );
    sink_buf.clear();

    sink.push_fa( "// XOICO_BODY_SIGNATURE 0x#pl16'0'{#X<tp_t>}\n", body_hash );
    if( body_signature ) body_signature.0 = body_hash;

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_state( c @* o, m x_sink* sink )
{
    sink.push_fa( "HKEYOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}\n", o.name.sc, o.get_hash() );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.to_be_modified
{
    if( o.compiler.always_expand ) return true;

    bl_t to_be_modified = true;

    tp_t target_hash = o.get_hash();

    m st_s* file = st_s_create_fa( "#<sc_t>.state", o.output_path.sc )^^;
    if( bcore_file_exists( file.sc ) )
    {
        m st_s* key_defined = st_s_create_fa( "##?w'HKEYOF_#<sc_t>'", o.name.sc )^^;
        m x_source* source = bcore_file_open_source( file.sc )^^;
        while( !source.eos() )
        {
            if( source.inspect_char() == 'H' )
            {
                if( source.parse_bl( key_defined.sc ) )
                {
                    tp_t key_val = 0;
                    source.parse_fa( " #<tp_t*>", &key_val );
                    to_be_modified = ( key_val != target_hash );
                    break;
                }
            }
            else
            {
                source.get_char();
            }
        }
    }

    return to_be_modified;
};

//----------------------------------------------------------------------------------------------------------------------

/// expands all text files in memory
func (:s) :.expand_phase1
{
    o.target_h =< NULL;
    o.target_c =< NULL;
    o.target_state =< NULL;
    o.modified = false;

    if( o.to_be_modified() )
    {
        o.target_h = st_s!;
        o.target_c = st_s!;
        o.target_state = st_s!;
        if( !o.compiler.dry_run )
        {
            o.expand_h( 0, o.target_h, o.body_signature_h.1 );
            o.expand_c( 0, o.target_c, o.body_signature_c.1 );
            o.expand_state( o.target_state );
        }
        else
        {
            m st_s* buf = st_s!^^;
            o.expand_h( 0, buf, o.body_signature_h.1 );
            o.expand_c( 0, buf, o.body_signature_c.1 );
            o.expand_state( buf );
        }
        o.modified = true;
    }

    if( p_modified ) p_modified.0 = o.modified;

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func er_t write_with_signature( sc_t file, c st_s* data )
{
    tp_t hash = bcore_tp_fold_sc( bcore_tp_init(), data.sc );
    m x_sink* sink = bcore_file_open_sink( file )^^;
    sink.push_data( ( vc_t )data.data, data.size );
    sink.push_fa( "// XOICO_FILE_SIGNATURE 0x#pl16'0'{#X<tp_t>}\n", hash );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/// returns true if a file was modified
func (:s) :.expand_phase2
{
    if( !o.modified )
    {
        if( p_modified.1 ) p_modified.0 = false;
        return 0;
    }

    ASSERT( o.target_h );
    ASSERT( o.target_c );

    m st_s* file_h = st_s_create_fa( "#<sc_t>.h", o.output_path.sc )^;
    m st_s* file_c = st_s_create_fa( "#<sc_t>.c", o.output_path.sc )^;
    m st_s* file_state = st_s_create_fa( "#<sc_t>.state", o.output_path.sc )^;

    if( o.readonly )
    {
//        bcore_msg_fa( "Affected: #<sc_t>\n", file_h.sc );
//        bcore_msg_fa( "Affected: #<sc_t>\n", file_c.sc );
        if( p_modified.1 ) p_modified.0 = false;
    }
    else
    {
        {
            bl_t clear_to_overwrite = false;
            tp_t body_signature = o.update_target_on_body_signature ? o.body_signature_h : 0;
            o.compiler.check_overwrite( file_h.sc, body_signature, clear_to_overwrite.1 );
            if( clear_to_overwrite )
            {
                bcore_msg_fa( "Writing: #<sc_t>\n", file_h.sc );
                xoico_target_write_with_signature( file_h.sc, o.target_h );
                if( p_modified.1 ) p_modified.0 = true;
            }
        }

        {
            bl_t clear_to_overwrite = false;
            tp_t body_signature = o.update_target_on_body_signature ? o.body_signature_c : 0;
            o.compiler.check_overwrite( file_c.sc, body_signature, clear_to_overwrite.1 );
            if( clear_to_overwrite )
            {
                bcore_msg_fa( "Writing: #<sc_t>\n", file_c.sc );
                xoico_target_write_with_signature( file_c.sc, o.target_c );
                if( p_modified.1 ) p_modified.0 = true;
            }
        }

        {
            bl_t clear_to_overwrite = false;
            o.compiler.check_overwrite( file_state.sc, 0, clear_to_overwrite.1 );
            if( clear_to_overwrite )
            {
                bcore_msg_fa( "Writing: #<sc_t>\n", file_state.sc );
                xoico_target_write_with_signature( file_state.sc, o.target_state );
                if( p_modified.1 ) p_modified.0 = true;
            }
        }

    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

