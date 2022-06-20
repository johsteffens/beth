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

name x_inst_main;
name x_inst_main_c;

signature m xoico* /*item*/ push_item_d( m @* o, d xoico* item );
signature er_t parse_name_recursive( c @* o, m x_source* source, m st_s* name );
signature er_t expand_declaration(   c @* o, sz_t indent, m x_sink* sink );
signature er_t expand_definition(    c @* o, sz_t indent, m x_sink* sink );
signature er_t expand_init1(         c @* o, sz_t indent, m x_sink* sink );
signature void explicit_embeddings_push( c @* o, m bcore_arr_st_s* arr );
signature er_t parse ( m @* o, c xoico_host* host, bl_t parse_block, m x_source* source );

signature m xoico_source_s*   get_source( c @* o );
signature m xoico_target_s*   get_target( c @* o );
signature m xoico_compiler_s* get_compiler( c @* o );

signature c xoico_feature_s* get_feature( c @* o, tp_t name ); // returns NULL in case name is not a member function
signature c    xoico_func_s*    get_func( c @* o, tp_t name ); // returns NULL in case name is not a member function

/// returns function by name in trait-line or NULL
signature c xoico_feature_s* get_traitline_feature_from_name( c @* o, tp_t name );
signature c    xoico_func_s*    get_traitline_func_from_name( c @* o, tp_t name );

/// returns member-function by name in trait-line or NULL
signature c xoico_func_s* get_traitline_member_func_from_name( c @* o, tp_t name );

/// source stack to handle includes
stamp :source_stack_s = aware x_array { aware x_source -> []; };

stamp :s = aware :
{
    aware xoico => []; // group elements

    private @* lexical_parent; // lexical parent (for namespace)

    bcore_arr_st_s includes_in_declaration;
    bcore_arr_st_s includes_in_definition;

    /** List of files explicitly included by embed directive.
     *  Used to list all sources contributing to this target in
     *  copyright and license info.
     */
    bcore_arr_st_s explicit_embeddings;

    st_s st_name; // global name
    tp_t tp_name; // global name

    func void set_name_sc( m @* o, xoico_host* host, sc_t name )
    {
        o.st_name.copy_sc( name );
        o.tp_name = host.entypeof( name );
    };

    tp_t trait_name = bcore_inst; // trait name
    tp_t pre_hash;

    /** Beta values > 0 represent experimental or transitional states in development
     *  They can be specified using the set directive: e.g. set beta = 1;
     */
    tp_t beta = 0;

    // 'expandable' is set 'false' for groups that is not intended to be expanded into actual code
    // but may contain information referenced in other groups (e.g. global features)
    bl_t expandable = true;

    // expands group inside *.xo.h during expand_manifesto; typically done when the group wraps a *.x source
    bl_t is_manifesto;

    // provides functionality to retrieve stamps which have this group in their trait-line
    bl_t is_retrievable;

    // defined when group is retrievable; contains a list of all stamps that have this group in their traitline
    bcore_arr_tp_s => retrievable_stamps;

    /** Activates using the short perspective type name.
     *  Normally the perspective type of a group is '<group_name>_spect_s'
     *  The short version is '<group_name>_s' can clash with stamp names.
     *  It should only be used for mapping low level perspectives into the xoila framework.
     *  (e.g. in inexpandable groups)
     */
    bl_t short_spect_name;

    xoico_transient_map_s transient_map;

    private xoico_stamp_s* extending_stamp; // !=NULL: extends this stamp on subsequent stamps

    xoico_funcs_s funcs;    // functions defined inside the group

    private aware xoico_source_s* xoico_source;
    hidden aware  xoico_compiler_s* compiler;

    x_source_point_s => source_point;

    hidden bcore_hmap_tpvd_s hmap_feature;
    hidden bcore_hmap_tpvd_s hmap_func;

    func :.parse;

    func xoico.get_hash;
    func xoico.get_global_name_tp { return o.tp_name; };
    func xoico.finalize;
    func xoico.expand_setup
    {
        foreach( m $* e in o ) try( e.expand_setup( o ) );
        return 0;
    };

    func :.parse_name_recursive;
    func :.expand_declaration;
    func :.expand_definition;
    func :.expand_init1;

    func xoico.expand_manifesto
    {
        if( !o.expandable || !o.is_manifesto ) return 0;
        sink.push_fa( "#rn{ }BETH_EXPAND_GROUP_#<sc_t>\n", indent, o.st_name.sc );
        return 0;
    };

    func :.push_item_d { return o.cast( m x_array* ).push_d( item ); };

    func c @* get_trait_group( c @* o )
    {
        return ( o.trait_name != o.tp_name ) ? o.compiler.get_group( o.trait_name ) : NULL;
    };

    func :.get_traitline_feature_from_name
    {
        if( !o ) return NULL;
        c xoico_feature_s** p_feature = ( const xoico_feature_s** )o.hmap_feature.get( name );
        return p_feature ? *p_feature : o.get_trait_group().get_traitline_feature_from_name( name );
    };

    func :.get_traitline_func_from_name
    {
        if( !o ) return NULL;
        c xoico_func_s** p_func = ( const xoico_func_s** )o.hmap_func.get( name );
        return p_func ? *p_func : o.get_trait_group().get_traitline_func_from_name( name );
    };

    func :.get_traitline_member_func_from_name
    {
        if( !o ) return NULL;
        c xoico_func_s** p_func = ( const xoico_func_s** )o.hmap_func.get( name );
        if( p_func && p_func.as_member() ) return p_func.1;
        return o.get_trait_group().get_traitline_member_func_from_name( name );
    };

    func :.get_feature
    {
        c xoico_feature_s** p_feature = ( const xoico_feature_s** )o.hmap_feature.get( name );
        return p_feature ? *p_feature : NULL;
    };

    func :.get_func
    {
        c xoico_func_s** p_func = ( const xoico_func_s** )o.hmap_func.get( name );
        return p_func ? *p_func : NULL;
    };

    func :.explicit_embeddings_push { foreach( m st_s* st in o.explicit_embeddings ) arr.push_st( st ); };

    func xoico_host.parse_name_st;
    func xoico_host.parse_name_tp;

    func xoico_host.compiler { return o.compiler; };
    func xoico_host.cengine
    {
        return o.xoico_source.target.cengine;
    };

    func xoico_host.obj_type { return o.tp_name; };
    func xoico_host.create_spect_name;
    func xoico_host.transient_map { return o.transient_map; };

    func xoico.get_source_point { return o.source_point; };
};

//----------------------------------------------------------------------------------------------------------------------

stamp xoico_nested_group_s = aware :
{
    hidden xoico_group_s* group; // group object;
    func xoico.get_hash
    {
        return o.group ? o.group.get_hash() : 0;
    };

    func xoico.expand_forward
    {
        sink.push_fa( " \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o.group.st_name.sc );
        return 0;
    };

    func xoico.expand_indef_declaration
    {
        sink.push_fa( " \\\n#rn{ }  BETH_EXPAND_GROUP_#<sc_t>", indent, o.group.st_name.sc );
        return 0;
    };

    func xoico.get_source_point { return o.group.source_point; };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.get_hash
{
    tp_t hash = o.pre_hash;
    hash = bcore_tp_fold_tp( hash, o.tp_name );
    hash = bcore_tp_fold_tp( hash, o.trait_name );
    hash = bcore_tp_fold_bl( hash, o.is_retrievable );
    hash = bcore_tp_fold_bl( hash, o.expandable );
    hash = bcore_tp_fold_bl( hash, o.short_spect_name );
    hash = bcore_tp_fold_tp( hash, o.beta );
    hash = bcore_tp_fold_tp( hash, o.funcs.get_hash() );
    foreach( m $* e in o ) hash = bcore_tp_fold_tp( hash, e.get_hash() );
    foreach( m $* e in o.includes_in_declaration ) hash = bcore_tp_fold_sc( hash, e.sc );
    foreach( m $* e in o.includes_in_definition  ) hash = bcore_tp_fold_sc( hash, e.sc );
    foreach( m $* e in o.explicit_embeddings     ) hash = bcore_tp_fold_sc( hash, e.sc );
    return hash;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico_host.create_spect_name
{
    if( o.short_spect_name )
    {
        return st_s_create_fa( "#<sc_t>_s", o.st_name.sc );
    }
    else
    {
        return st_s_create_fa( "#<sc_t>_spect_s", o.st_name.sc );
    }
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.parse_name_recursive
{
    if( source.parse_bl( "#?':'" ) )
    {
        if( o.lexical_parent )
        {
            o.lexical_parent.parse_name_recursive( source, name );
        }
        else
        {
            return source.parse_error_fa( "Too many colons: Namespace depth exceeded." );
        }
    }
    else
    {
        name.copy( o.st_name );
        m st_s* s = st_s!^;
        source.parse_fa( "#name", s );
        if( s.size > 0 ) name.push_fa( "_#<sc_t>", s.sc );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico_host.parse_name_st
{
    if( source.parse_bl( " #?':'" ) )
    {
        o.parse_name_recursive( source, name );
    }
    else
    {
        source.parse_fa( " #name", name );
    }

    if( name.size > 0 ) o.compiler.entypeof( name.sc );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico_host.parse_name_tp
{
    m $* s = st_s!^;

    if( source.parse_bl( " #?':'" ) )
    {
        o.parse_name_recursive( source, s );
    }
    else
    {
        source.parse_fa( " #name", s );
    }

    if( s.size == 0 ) source.parse_error_fa( "Identifier expected." );

    if( name.1 ) name.0 = o.compiler.entypeof( s.sc );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t push_default_feature_from_sc( m @* o, sc_t sc )
{
    m $* compiler = o.compiler;
    m $* feature = xoico_feature_s!^;
    feature.expandable = false;
    feature.parse( o, x_source_create_from_sc( sc )^ );

    if( !compiler.is_item( feature.get_global_name_tp() ) )
    {
        foreach( m $* func in feature.funcs_return_to_group ) o.funcs.push_d( func.fork() );
        feature.funcs_return_to_group.clear();
        compiler.register_item( feature );
        o.hmap_feature.set( feature.signature.name, ( vd_t )feature );
        o.push_item_d( feature.fork() );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t push_default_func_from_sc( m @* o, sc_t sc )
{
    m $* func = xoico_func_s!^;
    func.expandable = false;
    func.parse( o, x_source_create_from_sc( sc )^ );
    o.push_func_d( func.fork() );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t parse_func( m @* o, m x_source* source )
{
    m $* func = xoico_func_s!^;
    func.parse( o, source );
    o.push_func_d( func.fork() );
    if( func.signature_global_name == x_inst_main~ || func.signature_global_name == x_inst_main_c~ )
    {
        o.xoico_source.target.set_main_function( func );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t push_func_d( m @* o, d xoico_func_s* func )
{
    sz_t idx = o.funcs.get_index_from_name( func.name );

    m $* compiler = o.compiler;

    if( idx >= 0 )
    {
        m xoico_func_s* prex_func = o.funcs.[ idx ];
        if( ( prex_func.signature_global_name == func.signature_global_name ) )
        {
            if( !func.body )
            {
                return func.source_point.parse_error_fa( "Function '#<sc_t>' has already been declared.", compiler.nameof( func.name ) );
            }
            else if( prex_func.body )
            {
                return func.source_point.parse_error_fa( "Function '#<sc_t>' has already a body.", compiler.nameof( func.name ) );
            }
            else
            {
                o.funcs.replace_d( idx, func );
            }
        }
        else
        {
            return func.source_point.parse_error_fa( "Function '#<sc_t>' has already been declared.", compiler.nameof( func.name ) );
        }
    }
    else
    {
        o.funcs.push_d( func );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.parse
{
    m $* compiler = o.compiler;
    m $* stack = xoico_group_source_stack_s!^;
    stack.push_d( source.fork() );

    sc_t group_termination = NULL;

    if( !o.source_point )
    {
        o.source_point!.setup_from_source( source );
        o.pre_hash = bcore_tp_init();
        if( !o.tp_name ) o.tp_name = compiler.entypeof( o.st_name.sc );
    }

    if( parse_block ) // this group is nested in another group, the group body is enclosed in { ... }
    {
        source.parse_fa( " {" );
        group_termination = " #?'}'";
    }
    else if( source.parse_bl( " #?'#ifdef XOILA_SECTION'" ) ) // this group is root
    {
        group_termination = " #?'#endif'";
    }

    bl_t extend_stump = false;

    while
    (
        stack.size >= 2 ||
        ( group_termination ? !source.parse_bl( group_termination ) : !source.eos() )
    )
    {
        if( source.eos() )
        {
            if( stack.size >= 2 )
            {
                stack.[ stack.size - 1 ] =< NULL;
                source = stack.[ stack.size - 2 ];
                stack.set_size( stack.size - 1 );
            }
            else if( group_termination )
            {
                return source.parse_error_fa( "Unexpected end of file." );
            }
            else
            {
                break;
            }
        }
        else if( source.parse_bl( " #?w'stamp' " ) )
        {
            m $* stamp = xoico_stamp_s!^;
            stamp.group = o;
            stamp.parse( o, source );
            stamp.push_default_funcs();
            compiler.register_item( o.push_item_d( stamp.fork() ) );
        }

        /// stumps are inexpandable stamps. They can be used as template.
        else if( source.parse_bl( " #?w'stump' " ) )
        {
            m $* stump = xoico_stamp_s!^;
            stump.group = o;
            stump.parse( o, source );
            //stump.make_funcs_overloadable();
            compiler.register_item( stump );
            compiler.life_a_push( stump.fork() );
            if( extend_stump )
            {
                o.extending_stamp = stump;
                extend_stump = false;
            }
        }
        else if( source.parse_bl( " #?w'signature' " ) )
        {
            m $* signature = xoico_signature_s!^;
            signature.parse( o, source );
            source.parse_fa( " ; " );
            compiler.register_item( o.push_item_d( signature.fork() ) );
        }
        else if( source.parse_bl( " #?w'body' " ) )
        {
            m $* body = xoico_body_s!^;
            st_s^ body_name;
            o.parse_name_st( source, body_name );
            if( body_name.size == 0 ) return source.parse_error_fa( "Body name expected." );
            body.name = host.entypeof( body_name.sc );

            body.parse( o, source );

            source.parse_fa( " #-?';' " ); // closing semicolon is optional
            compiler.register_item( o.push_item_d( body.fork() ) );
        }
        else if( source.parse_bl( " #?w'feature' " ) )
        {
            m $* feature = xoico_feature_s!^;
            feature.group = o;
            feature.parse( o, source );
            compiler.register_item( o.push_item_d( feature.fork() ) );
            o.hmap_feature.set( feature.signature.name, ( vd_t )feature );
            foreach( m $* func in feature.funcs_return_to_group ) o.funcs.push_d( func.fork() );
            feature.funcs_return_to_group.clear();
        }
        else if( source.parse_bl( " #?w'func' " ) )
        {
            bl_t is_group_function = true;
            if( source.parse_bl( " #=?'('" ) )
            {
                sz_t index = source.get_index();
                source.parse_fa( "(" );
                m $* stamp_name = st_s!^;
                o.parse_name_st( source, stamp_name );
                if( source.parse_bl( " #?')'" ) )
                {
                    if( !stamp_name.ends_in_sc( "_s" ) ) return source.parse_error_fa( "Stamp name '#<sc_t>' must end in '_s'.", stamp_name.sc );
                    tp_t tp_stamp_name = btypeof( stamp_name.sc );
                    if( !compiler.is_stamp( tp_stamp_name ) )
                    {
                        return source.parse_error_fa( "Cannot associate type name '#<sc_t>' with a stamp.", stamp_name.sc );
                    }

                    m xoico_stamp_s* stamp = compiler.get_stamp( tp_stamp_name );
                    stamp.parse_func( source );
                    o.pre_hash = bcore_tp_fold_tp( o.pre_hash, stamp.get_hash() );
                    is_group_function = false;
                }
                else
                {
                    source.set_index( index );
                    is_group_function = true;
                }
            }

            if( is_group_function )
            {
                o.parse_func( source );
            }
        }
        else if( source.parse_bl( " #?w'name' " ) )
        {
            m $* name = xoico_name_s!^;
            name.parse( o, source );
            o.push_item_d( name.fork() );
            o.compiler.hmap_declared_name.set( name.name );
        }
        else if( source.parse_bl( " #?w'type' " ) )
        {
            xoico_name_s^ name.parse( o, source );
            compiler.register_external_type( name.name );
            o.pre_hash = bcore_tp_fold_tp( o.pre_hash, name.get_hash() );
        }
        else if( source.parse_bl( " #?w'identifier' " ) )
        {
            xoico_name_s^ name.parse( o, source );
            compiler.register_external_identifier( name.name );
            o.pre_hash = bcore_tp_fold_tp( o.pre_hash, name.get_hash() );
        }
        else if( source.parse_bl( " #?w'forward' " ) )
        {
            m $* forward = xoico_forward_s!^;
            forward.group = o;
            forward.parse( o, source );
            o.push_item_d( forward.fork() );
        }
        else if( source.parse_bl( " #?w'extending'" ) )
        {
            o.extending_stamp = NULL;
            if( source.parse_bl( " #=?';'" ) )
            {
                // just reset o->extending - nothing else
            }
            else if( source.parse_bl( " #=?w'stump'" ) )
            {
                extend_stump = true;
            }
            else
            {
                m $* templ_name = st_s!^;
                o.parse_name_st( source, templ_name );
                if( !templ_name.ends_in_sc( "_s" ) ) return source.parse_error_fa( "Stamp name '#<sc_t>' must end in '_s'.", templ_name.sc );
                c xoico* item = compiler.get_const_item( btypeof( templ_name.sc ) );
                if( !item ) return source.parse_error_fa( "Template #<sc_t> not found.", templ_name.sc );
                if( item._ != xoico_stamp_s~ ) return source.parse_error_fa( "Template #<sc_t> is no stamp.", templ_name.sc );
                o.extending_stamp = item.cast( m xoico_stamp_s* );
                source.parse_fa( " ;" );
            }
        }
        else if( source.parse_bl( " #?w'group' " ) )
        {
            st_s^ st_group_name;
            o.parse_name_st( source, st_group_name );
            bl_t retrievable = false;

            // using assignment symbol is optional
            if( source.parse_bl( " #?'=' " ) )
            {
                // nothing (error in case use of assignment becomes illegal)
            }

            // flags
            if( source.parse_bl( " #?w'retrievable' " ) ) retrievable = true;

            m$* transient_map = o.transient_map.clone()^;
            tp_t tp_trait_name = o.tp_name;
            m $* st_trait_name = st_s!^;

            // first test for 'trans' used instead of trait_name
            if( source.parse_bl( " #?w'trans' " ) )
            {
                transient_map.parse_update( o, source );
            }
            else
            {
                o.parse_name_st( source, st_trait_name );
                if( st_trait_name.size > 0 )
                {
                    tp_trait_name = compiler.entypeof( st_trait_name.sc );
                }
                else
                {
                    st_trait_name.copy_sc( compiler.nameof( tp_trait_name ) );
                }

                // second test for trans in case it occurs after trait_name
                if( source.parse_bl( " #?w'trans' " ) ) transient_map.parse_update( o, source );
            }

            m xoico_group_s* group = NULL;
            o.xoico_source.get_group_if_preexsting( host, source, st_group_name.sc, st_trait_name.sc, group.2 );

            if( !group )
            {
                group = o.xoico_source.push_d( xoico_group_s! );
                group.lexical_parent = o;
                group.xoico_source = o.xoico_source;
                group.compiler = o.compiler;
                group.extending_stamp = o.extending_stamp;
                group.expandable = o.expandable;
                group.set_name_sc( host, st_group_name.sc );
                group.is_retrievable = retrievable;
                group.trait_name = tp_trait_name;
                group.transient_map.copy( transient_map );
                group.parse( o, true, source );
                compiler.register_group( group );
                m xoico_nested_group_s* nested_group = xoico_nested_group_s!^;
                nested_group.group = group;
                o.push_item_d( nested_group.fork() );
            }
            else
            {
                group.transient_map.update( transient_map );
                group.parse( o, true, source );
            }

            source.parse_fa( " #-?';' " ); // closing semicolon is optional
        }
        else if( source.parse_bl( " #?w'set' " ) )
        {
            if     ( source.parse_bl( " #?w'retrievable' "      ) ) o.is_retrievable = true;
            else if( source.parse_bl( " #?w'inexpandable' "     ) ) o.expandable = false;
            else if( source.parse_bl( " #?w'short_spect_name' " ) ) o.short_spect_name = true;
            else if( source.parse_bl( " #?w'beta' "             ) ) source.parse_fa( " = #<tp_t*>", o.beta.1 );
            source.parse_fa( " ;" );
        }
        else if( source.parse_bl( " #?w'embed' " ) )
        {
            m st_s* folder = bcore_file_folder_path( source.get_file() )^;
            if( folder.size == 0 ) folder.push_char( '.' );
            m st_s* embed_file = st_s!^;
            source.parse_fa( " #string" , embed_file );
            source.parse_fa( " ;" );
            o.explicit_embeddings.push_st( embed_file );
            d x_source* embed_source = NULL;
            try( xoico_embed_file_open( source, embed_file.sc, embed_source.2 ) );

            // check for cyclic inclusions
            foreach( m $* e in stack )
            {
                sc_t path = embed_source.get_file();
                if( sc_t_equal( path, e.get_file() ) )
                {
                    return source.parse_error_fa( "Xoico: Cyclic inclusion." );
                }
            }

            stack.push_d( embed_source );
            source = embed_source;
        }
        else if( source.parse_bl( " #?w'include' " ) )
        {
            bl_t in_definition = false;
            if( source.parse_bl( " #?([0]==''')" ) )
            {
                char c = 0;
                source.parse_fa( "'#<char*>'", &c );
                if     ( c == 'c' ) in_definition = true;
                else if( c == 'h' ) in_definition = false;
                else return source.parse_error_fa( "include: 'c' or 'h' expected." );
            }
            else if( source.parse_bl( " #?w'deferred' " ) )
            {
                return source.parse_error_fa( "Syntax \"include deferred\" is deprecated. Use \"include 'c'\" instead." );
            }

            m st_s* include_file = st_s!^;

            if( source.parse_bl( " #?'\"'" ) )
            {
                include_file.push_char( '"' );
                source.parse_fa( "#:until'\"'\"", include_file );
                include_file.push_char( '"' );
            }
            else if( source.parse_bl( " #?'<'" ) )
            {
                include_file.push_char( '<' );
                source.parse_fa( "#:until'>'>", include_file );
                include_file.push_char( '>' );
            }
            else
            {
                return source.parse_error_fa( "'include <filepath>' or 'include \"filepath\"' expected." );
            }
            source.parse_fa( " ;" );

            if( in_definition )
            {
                o.includes_in_definition.push_st( include_file );
            }
            else
            {
                o.includes_in_declaration.push_st( include_file );
            }
        }
        else if( source.parse_bl( " #?w'trans' " ) )
        {
            o.transient_map.parse_update( o, source );
            source.parse_fa( " ;" );
        }
        else
        {
            return source.parse_error_fa( "Xoico: syntax error." );
        }

        source.parse_fa( " " );// consume whitespaces
    }

    if( stack.size > 1 )
    {
        return source.parse_error_fa( "Xoico: Unexpected end of group reached." );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.finalize
{
    /// default features
    o.push_default_feature_from_sc( "d @* clone( c @* o );" );
    o.push_default_feature_from_sc( "void copy( m @* o, c @* src );" );
    o.push_default_feature_from_sc( "void discard( m @* o );" );
    o.push_default_func_from_sc(    "d obliv @* t_create( tp_t t );" );

    // check validity of trait name
    if( !host.compiler().is_group( o.trait_name ) )
    {
        return o.source_point.parse_error_fa( "In group '#<sc_t>': Trait name '#<sc_t>' is not a group.", o.st_name.sc, host.nameof( o.trait_name ) );
    }

    foreach( m $* e in o ) e.finalize( o );
    foreach( m $* func in o.funcs )
    {
        func.finalize( o );
        o.compiler.register_func( func );
        o.hmap_func.set( func.name, ( vd_t )func );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_forward( c @* o, sz_t indent, m x_sink* sink )
{
    if( !o.expandable ) return 0;
    sink.push_fa( " \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o.st_name.sc );
    foreach( m $* e in o ) e.expand_forward( o, indent, sink );
    foreach( m $* func in o.funcs ) func.expand_forward( o, indent, sink );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_spect_declaration( c @* o, sz_t indent, m x_sink* sink )
{
    if( !o.expandable ) return 0;
    if( o.short_spect_name )
    {
        sink.push_fa( " \\\n#rn{ }BCORE_DECLARE_SPECT( #<sc_t> )", indent, o.st_name.sc );
    }
    else
    {
        sink.push_fa( " \\\n#rn{ }XOILA_DECLARE_SPECT( #<sc_t> )", indent, o.st_name.sc );
    }

    sink.push_fa( " \\\n#rn{ }{", indent );
    sink.push_fa( " \\\n#rn{ }    bcore_spect_header_s header;", indent );
    foreach( m $* e in o ) e.expand_spect_declaration( o, indent + 4, sink );
    sink.push_fa( " \\\n#rn{ }};", indent );

    sink.push_fa
    (
        " \\\n#rn{ }"
        "BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( #<sc_t> )",
        indent,
        o->st_name.sc
    );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.expand_declaration
{
    if( !o.expandable ) return 0;

    sink.push_fa( "\n" );
    sink.push_fa( "#rn{ }//#rn{-}\n", indent, sz_max( 0, 118 - indent ) );
    sink.push_fa("#rn{ }// group: #<sc_t>", indent, o->st_name.sc );
    if( o.explicit_embeddings.size > 0 )
    {
        sink.push_fa("; embeds:" );
        foreach( m st_s* st in o.explicit_embeddings ) sink.push_fa(" #<st_s*>", st );
    }
    sink.push_fa("\n" );

    foreach( m $* e in o->includes_in_declaration ) sink.push_fa( "##include #<sc_t>\n", e.sc );

    sink.push_fa( "\n" );
    sink.push_fa( "#rn{ }##define TYPEOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, o.st_name.sc, btypeof( o.st_name.sc ) );

    m st_s* st_spect_name = xoico_group_s_create_spect_name( o )^;
    sc_t  sc_spect_name = st_spect_name->sc;

    sink.push_fa( "#rn{ }##define TYPEOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, sc_spect_name, btypeof( sc_spect_name ) );

    foreach( m $* e in o ) e.expand_declaration( o, indent, sink );
    sink.push_fa( "#rn{ }##define BETH_EXPAND_GROUP_#<sc_t>", indent, o.st_name.sc );

    o.expand_forward( indent + 2, sink );

    foreach( m $* e in o ) e.expand_indef_typedef( o, indent, sink );
    o.expand_spect_declaration( indent + 2, sink );

    foreach( m $* e in o ) e.expand_indef_declaration( o, indent, sink );
    foreach( m $* func in o->funcs ) func.expand_declaration( o, indent + 2, sink );
    sink.push_fa( "\n" );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_spect_definition( c @* o, sz_t indent, m x_sink* sink )
{
    m $* compiler = o.compiler;
    if( !o.expandable ) return 0;
    sink.push_fa( "\n" );
    if( o.short_spect_name )
    {
        sink.push_fa( "#rn{ }BCORE_DEFINE_SPECT( #<sc_t>, #<sc_t> )\n", indent, compiler.nameof( o.trait_name ), o.st_name.sc );
    }
    else
    {
        sink.push_fa( "#rn{ }XOILA_DEFINE_SPECT( #<sc_t>, #<sc_t> )\n", indent, compiler.nameof( o.trait_name ), o.st_name.sc );
    }

    sink.push_fa( "#rn{ }\"{\"\n", indent );
    sink.push_fa( "#rn{ }    \"bcore_spect_header_s header;\"\n", indent );
    foreach( m $* e in o ) e.expand_spect_definition( o, indent + 4, sink );
    sink.push_fa( "#rn{ }\"}\";\n", indent );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.expand_definition
{
    if( !o.expandable ) return 0;
    sink.push_fa( "\n" );
    sink.push_fa( "#rn{ }//#rn{-}\n", indent, sz_max( 0, 118 - indent ) );
    sink.push_fa( "#rn{ }// group: #<sc_t>", indent, o.st_name.sc );
    if( o.explicit_embeddings.size > 0 )
    {
        sink.push_fa("; embeds:" );
        foreach( m st_s* st in o.explicit_embeddings ) sink.push_fa(" #<st_s*>", st );
    }
    sink.push_fa("\n" );

    foreach( m $* e in o.includes_in_definition ) sink.push_fa( "##include #<sc_t>\n", e.sc );

    // non-features
    foreach( m $* e in o; e._ != xoico_feature_s~ ) e.expand_definition( o, indent, sink );

    o.expand_spect_definition( indent, sink );

    // just-features
    foreach( m $* e in o; e._ == xoico_feature_s~ ) e.expand_definition( o, indent, sink );

    foreach( m $* func in o->funcs ) func.expand_definition( o, indent, sink );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.expand_init1
{
    if( !o.expandable ) return 0;
    sink.push_fa( "\n" );
    sink.push_fa( "#rn{ }// group: #<sc_t>\n", indent, o->st_name.sc );
    foreach( m $* e in o ) e.expand_init1( o, indent, sink );

    if( o.short_spect_name )
    {
        sink.push_fa( "#rn{ }BCORE_REGISTER_SPECT( #<sc_t> );\n", indent, o.st_name.sc );
    }
    else
    {
        sink.push_fa( "#rn{ }XOILA_REGISTER_SPECT( #<sc_t> );\n", indent, o.st_name.sc );
    }

    if( o.is_retrievable )
    {
        /// deprecated
//        foreach( m $* e in o; e._ == xoico_stamp_s~ )
//        {
//            sink.push_fa( "#rn{ }bcore_inst_s_get_typed( TYPEOF_#<sc_t> );\n", indent, e.cast( m xoico_stamp_s* ).st_name.sc );
//        }

        sink.push_fa( "#rn{ }{\n", indent );
        sink.push_fa( "#rn{ }   bcore_arr_tp_s* arr = bcore_arr_tp_s_create();\n", indent );
        foreach( tp_t type in o.retrievable_stamps )
        {
            sink.push_fa( "#rn{ }   bcore_arr_tp_s_push( arr, TYPEOF_#<sc_t> );\n", indent, o.compiler.nameof( type ) );
        }
        sink.push_fa( "#rn{ }   bcore_xoila_set_arr_traitline_stamps_d( TYPEOF_#<sc_t>, arr );\n", indent, o.compiler.nameof( o.tp_name ) );
        sink.push_fa( "#rn{ }}\n", indent );

    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

