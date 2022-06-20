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

signature er_t parse_func( m @* o, m x_source* source );
signature er_t parse_wrap( m @* o, m x_source* source );
signature er_t make_funcs_overloadable( m @* o );
signature er_t push_default_funcs( m @* o );
signature er_t push_compact_initializer_func( m @* o );
signature c xoico_func_s* get_func_from_name( c @* o, tp_t name ); // returns NULL if not found
signature c xoico_feature_s* get_traitline_feature_from_name( c @* o, tp_t name ); // returns NULL if not found
signature c xoico_func_s* get_traitline_func_from_name( c @* o, tp_t name ); // returns NULL if not found
signature c xoico_func_s* get_traitline_member_func_from_name( c @* o, tp_t name ); // returns NULL if not found

stamp :s = aware :
{
    st_s st_name;
    tp_t tp_name; // typeof( st_name )
    tp_t trait_name;
    bl_t is_aware = true;
    bl_t has_compact_initializer = false;

    :arr_self_item_s => arr_self_item;

    st_s => self_source;

    bcore_self_s => self; // created in expand_setup
    xoico_funcs_s funcs;
    xoico_wraps_s wraps;

    xoico_transient_map_s transient_map;

    private aware xoico_group_s* group;
    x_source_point_s source_point;

    func xoico.get_hash
    {
        tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o->_ );
        hash = bcore_tp_fold_sc( hash, o.st_name.sc );
        hash = bcore_tp_fold_sc( hash, o.self_source ? o.self_source.sc : "" );
        hash = bcore_tp_fold_tp( hash, o.funcs.get_hash() );
        return hash;
    };

    func xoico.get_global_name_tp { return o.tp_name; };
    func xoico.finalize;

    func xoico.expand_setup { return 0; };
    func xoico.expand_declaration;
    func xoico.expand_forward
    {
        sink.push_fa( " \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o.st_name.sc );
        return 0;
    };

    func xoico.expand_indef_declaration
    {
        sink.push_fa( " \\\n#rn{ }  BETH_EXPAND_ITEM_#<sc_t>", indent, o.st_name.sc );
        return 0;
    };

    func xoico.expand_definition;
    func xoico.expand_init1;

    func xoico.parse;
    func :.parse_func;
    func :.parse_wrap;

    func :.make_funcs_overloadable
    {
        foreach( m $* func in o.funcs ) func->overloadable = true;
        return 0;
    };

    func :.get_func_from_name { return o.funcs.get_func_from_name( name ); };

    func :.get_traitline_func_from_name
    {
        c $* func = o.funcs.get_func_from_name( name );
        if( !func )
        {
            func = o.group.compiler.get_group( o.trait_name ).get_traitline_func_from_name( name );
        }
        return func;
    };

    func :.get_traitline_feature_from_name
    {
        return o.group.compiler.get_group( o.trait_name ).get_traitline_feature_from_name( name );
    };

    func :.get_traitline_member_func_from_name
    {
        c $* func = o.funcs.get_func_from_name( name );
        if( !func )
        {
            func = o.group.compiler.get_group( o.trait_name ).get_traitline_member_func_from_name( name );
        }
        return func;
    };

    func :.push_default_funcs;

    func xoico_host.parse_name_st { return o.group.parse_name_st( source, name ); };
    func xoico_host.compiler { return o.group.compiler; };
    func xoico_host.cengine { return o.group.cengine(); };
    func xoico_host.obj_type { return o.tp_name; };
    func xoico_host.transient_map { return o.transient_map; };

    func xoico.get_source_point { return o.source_point; };
};

//----------------------------------------------------------------------------------------------------------------------

stamp :self_item_s =
{
    x_source_point_s source_point;
    st_s st;
    bl_t arg_of_initializer; // this item is argument of the compact initializer
    bl_t copy_from_initializer; // this shall be copied (cloned) from the the compact initializer

    func er_t to_bcore_self_item( @* o, bcore_self_s* self, m bcore_self_item_s* item )
    {
        er_t er = bcore_self_item_s_parse_src( item, sr_awc( x_source_create_from_st( o.st )^ ), self, false );

        if( er )
        {
            m $* msg = st_s!^;
            bcore_error_pop_st( er.1, msg );
            return o.source_point.parse_error_fa( "#<sc_t>\n", msg.sc );
        }
        return 0;
    };
};

stamp :arr_self_item_s = x_array { :self_item_s []; };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// removes comments, excessive whitespaces; trailing whitespaces; keeps strings but replaces '"' with '\"'
func d st_s* create_embedded_string( c st_s* s )
{
    d st_s* out = st_s!;
    for( sz_t i = 0; i < s.size; i++ )
    {
        if( s.[ i ] == '/' && s.[ i + 1 ] == '/' )
        {
            while( i < s.size && s.[ i ] != '\n' ) i++;
        }
        else if( s.[ i ] == '/' && s.[ i + 1 ] == '*' )
        {
            while( i < s.size )
            {
                if( s.[ i ] == '*' && s.[ i + 1 ] == '/' )
                {
                    if( out.[ out.size - 1 ] != ' ' ) out.push_char( ' ' );
                    i += 2;
                    break;
                }
                i++;
            }
        }
        else if( s.[ i ] == '"' )
        {
            out.push_char( '\\' );
            out.push_char( '"' );
            i++;
            while( i < s.size && s.[ i ] != '"' ) out.push_char( s.[ i++ ] );
            if( s.[ i ] == '"' )
            {
                out.push_char( '\\' );
                out.push_char( '"' );
            }
        }
        else if( s.[ i ] == '\n' )
        {
            if( out.[ out.size - 1 ] != ' ' ) out.push_char( ' ' );
        }
        else if( s.[ i ] == '\t' )
        {
            if( out.[ out.size - 1 ] != ' ' ) out.push_char( ' ' );
        }
        else if( s.[ i ] == ' ' )
        {
            if( out.[ out.size - 1 ] != ' ' ) out.push_char( ' ' );
        }
        else
        {
            out.push_char( s.[ i ] );
        }
    }
    if( out.size > 0 && out.[ out.size - 1 ] == ' ' )
    {
        out.[ out.size - 1 ] = 0;
        out.size--;
    }
    return out;
};

//----------------------------------------------------------------------------------------------------------------------

/** Creates a structured multiline string for direct code embedding
 *  from an embedded string
 */
func d st_s* create_structured_multiline_string( c sc_t s, sz_t indent )
{
    d st_s* out = st_s!;
    sz_t ind = indent;
    bl_t newline = true;
    for( sz_t i = 0; s[ i ] != 0; i++ )
    {
        char c = s[ i ];
        if( c == ';' )
        {
            if( newline ) out.push_fa( "#rn{ }\"", ind );
            out.push_char( c );
            out.push_fa( "\"\n" );
            newline = true;
        }
        else if( c == '{' )
        {
            if( newline )
            {
                out.push_fa( "#rn{ }\"{\"", ind );
            }
            else
            {
                out.push_fa( "\"\n#rn{ }\"{\"", ind );
            }
            if( s[ i + 1 ] != 0 ) out.push_char( '\n' );
            ind += 4;
            newline = true;

        }
        else if( c == '}' )
        {
            ind -= 4;
            if( newline )
            {
                out.push_fa( "#rn{ }\"}\"", ind );
            }
            else
            {
                out.push_fa( "\"\n#rn{ }\"}\"", ind );
            }
            if( s[ i + 1 ] != 0 ) out.push_char( '\n' );
            newline = true;
        }
        else if( c == ' ' )
        {
            if( !newline )
            {
                if( s[ i + 1 ] != ';' && s[ i + 1 ] != '{' && s[ i + 1 ] != '}' && s[ i + 1 ] != 0 )
                {
                    out.push_char( c );
                }
            }
        }
        else
        {
            if( newline )
            {
                out.push_fa( "#rn{ }\"", ind );
                newline = false;
            }
            out.push_char( c );
        }
    }

    if( !newline ) out.push_char( '"' );
    return out;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) sc_t get_rel_name_sc( c @* o )
{
    sc_t group_name = o.group.st_name.sc;
    sc_t stamp_name = o.st_name.sc;

    sz_t i = 0;
    while( group_name[ i ] == stamp_name[ i ] && group_name[ i ] != 0 ) i++;
    if( group_name[ i ] == 0 && stamp_name[ i ] == '_' ) i++;

    return stamp_name + i;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.parse_func
{
    m $* compiler = o.group.compiler;
    m $* func = xoico_func_s!^;
    func.parse( o, source );

    sz_t idx = o.funcs.get_index_from_name( func->name );

    if( idx >= 0 )
    {
        m xoico_func_s* prex_func = o.funcs.[ idx ];
        if( ( prex_func.signature_global_name == func.signature_global_name ) )
        {
            if( !func.body )
            {
                return source.parse_error_fa( "Function '#<sc_t>' has already been declared.", compiler.nameof( func.name ) );
            }
            else if( prex_func.body )
            {
                return func.source_point.parse_error_fa( "Function '#<sc_t>' has already a body.", compiler.nameof( func.name ) );
            }
            else
            {
                o.funcs.replace_d( idx, func.fork() );
            }
        }
        else if( prex_func.overloadable )
        {
            o.funcs.replace_d( idx, func.fork() );
        }
        else
        {
            return func.source_point.parse_error_fa( "Function '#<sc_t>' has already been declared and is not overloadable.", compiler.nameof( func.name ) );
        }
    }
    else
    {
        o.funcs.push_d( func.fork() );
    }

    if( func.signature_global_name == x_inst_main~ ) o.group.xoico_source.target.set_main_function( func );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.parse_wrap
{
    m $* wrap = xoico_wrap_s!^;
    wrap.parse( o, source );
    o.wraps.push_d( wrap.fork() );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t parse_extend( m @* o, m x_source* source )
{
    m $* buf = st_s!^;
    m $* self = bcore_self_s!^;
    self.type = o.tp_name;
    self.trait = o.trait_name;

    if( source.parse_bl( " #?'{'" ) )
    {
        while( !source.eos() && !source.parse_bl( " #?'}'" ) )
        {
            if( source.parse_bl( " #?w'func'" ) )
            {
                o.parse_func( source );
            }
            else if( source.parse_bl( " #?w'wrap'" ) )
            {
                o.parse_wrap( source );
            }
            else
            {
                bl_t exit = false;

                buf.clear();
                bl_t arg_of_initializer = false;

                while( !exit && !source.eos() )
                {
                    u0_t c =  source.get_u0();
                    switch( c )
                    {
                        case ':':
                        {
                            m st_s* name = st_s!^;
                            o.group.parse_name_recursive( source, name );
                            buf.push_st( name );
                        }
                        break;

                        case '$':
                        {
                            arg_of_initializer = true;
                        }
                        break;

                        case '@':
                        {
                            buf.push_st( o.st_name );
                        }
                        break;

                        case ';':
                        {
                            buf.push_char( c );
                            exit = true;
                        }
                        break;

                        case '"': // string literal
                        {
                            buf.push_char( c );
                            while( !source.eos() && ((c = source.get_char()) != '"') )
                            {
                                buf.push_char( c );
                                if( c == '\\' ) buf.push_char( source.get_char() );
                                if( c == '\n' ) return source.parse_error_fa( "Newline in string literal." );
                            }
                            if( source.eos() ) return source.parse_error_fa( "End of file in string literal." );
                            buf.push_char( c );
                        }
                        break;

                        default:
                        {
                            buf.push_char( c );
                        }
                        break;
                    }
                }

                if( arg_of_initializer ) o.has_compact_initializer = true;

                m$* self_item = :self_item_s!^;
                self_item.arg_of_initializer = arg_of_initializer;
                self_item.st.copy( buf );
                self_item.source_point.setup_from_source( source );
                o.arr_self_item!.push_d( self_item.fork() );
            }
        }
    }
    else
    {
        if( source.parse_bl( " #?w'func'" ) )
        {
            o.parse_func( source );
        }
        else  // a non-block must close with semicolon
        {
            source.parse_fa( " ;" );
        }
    }

    source.parse_fa( " #-?';' " ); // closing semicolon is optional

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t push_default_func_from_sc( m @* o, sc_t sc )
{
    m $* compiler = o.group.compiler;
    m $* func = xoico_func_s!^;
    func.overloadable = false;
    func.expandable = false;

    func.parse_sc( o, sc );

    sz_t idx = o.funcs.get_index_from_signature_global_name( func.signature_global_name );

    if( idx >= 0 )
    {
        return o.source_point.parse_error_fa( "Function '#<sc_t>' conflicts with a default function for this stamp.", compiler.nameof( func.name ) );
    }
    else
    {
        o.funcs.push_d( func.fork() );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.push_default_funcs
{
    o.push_default_func_from_sc( "bcore_stamp_funcs.init;" );
    o.push_default_func_from_sc( "bcore_stamp_funcs.down;" );
    o.push_default_func_from_sc( "bcore_stamp_funcs.copy;" );
    o.push_default_func_from_sc( "bcore_stamp_funcs.create;" );
    o.push_default_func_from_sc( "bcore_stamp_funcs.discard;" );
    o.push_default_func_from_sc( "bcore_stamp_funcs.clone;" );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.push_compact_initializer_func
{
    m $* compiler = o.group.compiler;

    st_s^ sig;
    st_s^ body;
    sig.push_fa( "o _(m@* o" );

    m $* self = bcore_self_s!^; self.type = o.tp_name; self.trait = o.trait_name;

    sc_t err_prefix = "Cannot setup member for compact initialization";

    foreach( $* self_item in o.arr_self_item )
    {
        if( self_item.arg_of_initializer )
        {
            m $* item = bcore_self_item_s!^;
            self_item.to_bcore_self_item( self, item );

            if( !item.type ) return self_item.source_point.parse_error_fa( "#<sc_t>: Item has no type.", err_prefix );
            if( !item.name ) return self_item.source_point.parse_error_fa( "#<sc_t>: Item has no name.", err_prefix );
            if( bcore_flect_caps_is_array( item.caps ) ) return self_item.source_point.parse_error_fa( "#<sc_t>: Item is an array.", err_prefix );
            if( bcore_flect_caps_is_typed( item.caps ) ) return self_item.source_point.parse_error_fa( "#<sc_t>: Item is typed.", err_prefix );
            sz_t indirection = bcore_flect_caps_get_indirection( item.caps );
            bl_t is_leaf = bcore_type_is_leaf( item.type );
            bl_t is_pointer = ( item.caps == BCORE_CAPS_POINTER );

            sc_t sc_type = bnameof( item.type );
            sc_t sc_name = bnameof( item.name );

            if( indirection == 0 )
            {
                if( is_leaf )
                {
                    sig.push_fa( ", #<sc_t> #<sc_t>", sc_type, sc_name );
                    body.push_fa( "    o.#<sc_t> = #<sc_t>;\n", sc_name, sc_name );
                }
                else
                {
                    sig.push_fa( ", c #<sc_t>* #<sc_t>", sc_type, sc_name );
                    body.push_fa( "    o.#<sc_t>.copy( #<sc_t> );\n", sc_name, sc_name );
                }
            }
            else
            {
                if( is_leaf || is_pointer )
                {
                    sig.push_fa( ", m #<sc_t>* #<sc_t>", sc_type, sc_name );
                    body.push_fa( "    o.#<sc_t> = #<sc_t>;\n", sc_name, sc_name );
                }
                else if( self_item.copy_from_initializer )
                {
                    sig.push_fa( ", c #<sc_t>* #<sc_t>", sc_type, sc_name );
                    body.push_fa( "    o.#<sc_t> =< #<sc_t>.clone();\n", sc_name, sc_name );
                }
                else
                {
                    sig.push_fa( ", d #<sc_t>* #<sc_t>", sc_type, sc_name );
                    body.push_fa( "    o.#<sc_t> =< #<sc_t>;\n", sc_name, sc_name );
                }
            }
        }
    }
    sig.push_fa( ")" );

    st_s^ st_func.push_fa
    (
        "#<sc_t>\n"
        "{\n"
        "#<sc_t>"
        "    return o;\n"
        "};",
        sig.sc,
        body.sc
    );

    sz_t idx = o.funcs.get_index_from_name( compiler.entypeof( "_" ) );
    if( idx >= 0 )
    {
        return o.funcs.[ idx ].source_point.parse_error_fa
        (
            "Explicit compact function declaration not possible because this stamp has one or more member elements marked for compact initialization."
        );
    }

    m $* func = xoico_func_s!^;
    func.parse_sc( o, st_func.sc );
    o.funcs.push_d( func.fork() );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.parse
{
    m $* compiler = o.group.compiler;
    bl_t verbatim = source.parse_bl( " #?w'verbatim'" );

    m $* st_stamp_name = st_s!^;

    o.source_point.setup_from_source( source );

    o.transient_map.copy( o.group.transient_map );

    o.group.parse_name_st( source, st_stamp_name );

    if( !st_stamp_name.ends_in_sc( "_s" ) ) return source.parse_error_fa( "Stamp name '#<sc_t>' must end in '_s'.", st_stamp_name->sc );

    if( source.parse_bl( " #?'('" ) ) // functor args
    {
        m$* args = xoico_args_s!^;
        args.parse( host, source );
        source.parse_fa( " )" );

        foreach( $*e in args )
        {
            m$* self_item = :self_item_s!^;
            self_item.arg_of_initializer = true;
            self_item.copy_from_initializer = ( e.typespec.access_class == TYPEOF_const );

            e.to_self_item_st( o, self_item.st );
            self_item.source_point.copy( e.source_point );
            o.arr_self_item!.push_d( self_item.fork() );
            o.has_compact_initializer = true;
        }
    }

    // using assignment symbol is optional
    if( source.parse_bl( " #?'=' " ) )
    {
        // nothing (error in case use of assignment becomes illegal)
    }

    if( source.parse_bl( " #?w'extending'" ) )
    {
        m st_s* templ_name = st_s!^;
        o.group.parse_name_st( source, templ_name );

        if( !templ_name.ends_in_sc( "_s" ) )
        {
            return source.parse_error_fa( "Extending: Stamp name '#<sc_t>' must end in '_s'.", templ_name->sc );
        }

        c xoico* item = compiler.get_const_item( btypeof( templ_name.sc ) );
        if( !item ) return source.parse_error_fa( "Template #<sc_t> not found.", templ_name.sc );
        if( item._ != xoico_stamp_s~ ) return source.parse_error_fa( "Template #<sc_t> is no stamp.", templ_name.sc );
        o.copy( item.cast( m xoico_stamp_s* ) );
    }
    else if( !verbatim && o.group.extending_stamp )
    {
        o.copy( o.group.extending_stamp );
    }
    else
    {
        /// 'is_aware' is true by default
        if      ( source.parse_bl( " #?w'obliv'" ) ) o.is_aware = false;
        else if ( source.parse_bl( " #?w'aware'" ) ) o.is_aware = true;
        o.trait_name = o.group.tp_name;

        if( !source.parse_bl( " #=?w'func'" ) && !source.parse_bl( " #=?w'trans'" ) ) // if func follows, the brief-functor format was chosen
        {
            st_s^ st_trait_name;
            o.group.parse_name_st( source, st_trait_name );
            if( st_trait_name.size > 0 ) o.trait_name = compiler.entypeof( st_trait_name.sc );
        }

        if( source.parse_bl( " #?w'trans' " ) )
        {
            o.transient_map.parse_update( o, source );
        }

    }

    o.st_name.copy( st_stamp_name );
    o.tp_name = compiler.entypeof( st_stamp_name.sc );
    o.parse_extend( source );
    if( o.has_compact_initializer ) o.push_compact_initializer_func();

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.finalize
{
    //if( o.has_compact_initializer ) o.push_compact_initializer_func();

    m $* compiler = o.group.compiler;
    st_s^ self_buf;

    m bcore_self_item_s* first_array_item = NULL; // !=NULL if stamp has an array;
    {
        m $* self = bcore_self_s!^; self.type = o.tp_name; self.trait = o.trait_name;
        foreach( m $* self_item in o.arr_self_item )
        {
            m $* item = bcore_self_item_s!^;
            self_item.to_bcore_self_item( self, item );

            if( compiler.is_group( item.type ) )
            {
                if( bcore_flect_caps_get_indirection( item.caps ) == 0 )
                {
                    return self_item.source_point.parse_error_fa( "Element is a group at indirection 0. Was a pointer intended?\n" );
                }

                if( verbatim_C{ !item->flags.f_aware } && verbatim_C{ !item->flags.f_obliv } && !bcore_flect_caps_is_typed( item.caps ) )
                {
                    /// group reference: when type awareness is not specified but 'aware' is a legal option, assume 'aware'.
                    self_item.st.insert_sc( 0, "aware " );
                }
            }

            if( bcore_flect_caps_is_array( item.caps ) && !first_array_item ) first_array_item = item.clone()^^;
            self_buf.push_st( self_item.st );
        }
    }

    // set transient classes for x_array
    if( o.trait_name == x_array~ || o.trait_name == bcore_array~ )
    {
        if( !first_array_item )
        {
            return o.source_point.parse_error_fa( "In stamp '#<sc_t>': Stamp is of trait 'x_array' but contains no array", o.st_name.sc );
        }
        o.transient_map.set( compiler.entypeof( "TO" ), o.tp_name );
        if( first_array_item.type ) o.transient_map.set( compiler.entypeof( "TE" ), first_array_item.type );
    }

    // overload transient features
    foreach( tp_t key in o.transient_map.create_key_arr()^ )
    {
        $* feature = o.get_traitline_feature_from_name( key );
        if( feature )
        {
            if( !feature.signature.fits_transient_type_feature() )
            {
                st_s^ feature_reference;
                feature.source_point.source_reference_to_sink( false, feature_reference );
                return o.source_point.parse_error_fa
                (
                    "In stamp '#<sc_t>': Stamp uses transient feature '#<sc_t>' with triggers automatic overload of a feature with this name in the stamps traitline.\n"
                    "A matching feature was found in '#<sc_t>' but it does not satisfy signature-requirements for such an overload.\n"
                    "Suggestions:\n"
                    "   - Define feature in the form 'feature tp_t #<sc_t>( @*o );'. Or\n"
                    "   - If the feature has a different purpose:\n"
                    "       - Rename the feature. Or\n"
                    "       - Rename the transient type identifier '#<sc_t>'.\n",
                    o.st_name.sc,
                    compiler.nameof( feature.signature.name ),
                    feature_reference.sc,
                    compiler.nameof( feature.signature.name ),
                    compiler.nameof( feature.signature.name )
                );
            }

            tp_t type = o.transient_map.get( key );

            st_s^ func_code.push_fa
            (
                "#<sc_t>.#<sc_t> { = TYPEOF_#<sc_t> }",
                compiler.nameof( feature.group.tp_name ),
                compiler.nameof( feature.signature.name ),
                compiler.nameof( type )
            );

            o.push_default_func_from_sc( func_code.sc );
        }
    }

    foreach( m $* wrap in o.wraps )
    {
        wrap.finalize( o );
        o.funcs.push_d( wrap.func.fork() );
    }

    foreach( m $* func in o.funcs )
    {
        func.finalize( o );
        if( func.reflectable( o ) ) func.push_flect_decl_to_sink( o, self_buf );
        compiler.register_func( func );
    }

    o.self_source =< st_s!;
    o.self_source.push_fa( "#<sc_t> =", o.st_name.sc );
    if( o.is_aware ) o.self_source.push_sc( "aware " );
    o.self_source.push_sc( compiler.nameof( o.trait_name ) );
    o.self_source.push_fa( "{#<st_s*>}", self_buf.1 );

    o.self =< bcore_self_s_parse_source( ( bcore_source* )x_source_create_from_st( o.self_source )^, 0, 0, o.group.st_name.sc, false );

    // checking for repetitions in o.self (non-functions)
    m $* hmap_name = bcore_hmap_tp_s!^;
    sz_t self_items = bcore_self_s_items_size( o.self );
    for( sz_t i = 0; i < self_items; i++ )
    {
        c bcore_self_item_s* item = o.self.get_item( i );
        if( item.name && ( item.caps != BCORE_CAPS_EXTERNAL_FUNC ) )
        {
            if( hmap_name.exists( item.name ) )
            {
                return o.source_point.parse_error_fa( "In stamp '#<sc_t>': Repeated use of member name '#<sc_t>'.", o.st_name.sc, compiler.nameof( item.name ) );
            }

            hmap_name.set( item.name );
        }
    }

    // check validity of trait name
    if( !compiler.is_group( o.trait_name ) )
    {
        return o.source_point.parse_error_fa( "In stamp '#<sc_t>': Trait name '#<sc_t>' is not a group.", o.st_name.sc, compiler.nameof( o.trait_name ) );
    }

    // update retrievability of groups in traitline
    for( tp_t trait_name = o.trait_name; trait_name != 0; )
    {
        xoico_group_s* group = compiler.get_group( trait_name );
        trait_name = group.trait_name != group.tp_name ? group.trait_name : 0;
        if( group.is_retrievable ) group.retrievable_stamps!.push( o.tp_name );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.expand_declaration
{
    sc_t sc_name = o.st_name.sc;

    sink.push_fa( "#rn{ }##define TYPEOF_#<sc_t> 0x#pl16'0'{#X<tp_t>}ull\n", indent, sc_name, btypeof( sc_name ) );

    sink.push_fa( "#rn{ }##define BETH_EXPAND_ITEM_#<sc_t>", indent, sc_name, sc_name );
    sink.push_fa( " \\\n#rn{ }  BCORE_DECLARE_OBJECT( #<sc_t> )", indent, sc_name );
    sink.push_fa( " \\\n" );

    bcore_self_s_struct_body_to_sink_newline_escaped( o.self, indent + 2, ( bcore_sink* )sink );
    sink.push_fa( ";" );

    foreach( m $* func in o.funcs ) func.expand_forward( o, indent + 2, sink ); // expands all prototypes
    foreach( m $* func in o.funcs ) func.expand_declaration( o, indent + 2, sink ); // only expands static inline functions

    sink.push_fa( "\n" );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.expand_definition
{
    m st_s* embedded_string = o.create_embedded_string( o.self_source )^^;

    // 4095 is the C99-limit for string literals
    if( embedded_string.size > 4095 )
    {
        return o.source_point.parse_error_fa
        (
            "Reflection embedding failed. (Stamp is too large)\n"
            "The embedded code would require a string literal larger than 4095 characters.\n"
            "This exceeds the limit defined in C99.\n"
        );
    }

    sz_t idx = embedded_string.find_char( 0, -1, '=' );
    sc_t self_def = "";
    if( idx < embedded_string.size )
    {
        self_def = embedded_string.sc + idx + 1;
    }

    sink.push_fa( "\n" );
    sink.push_fa( "#rn{ }BCORE_DEFINE_OBJECT_INST_P( #<sc_t> )\n", indent, o.st_name.sc );

    m st_s* multiline_string = xoico_stamp_create_structured_multiline_string( self_def, indent )^^;
    sink.push_fa( "#<sc_t>;\n", multiline_string.sc );

    foreach( m $* func in o.funcs ) func.expand_definition( o, indent, sink );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.expand_init1
{
    m $* compiler = o.group.compiler;

    foreach( m $* func in o.funcs )
    {
        if( func.reflectable( host ) )
        {
            c $* signature = func.signature;
            sink.push_fa
            (
                "#rn{ }BCORE_REGISTER_FFUNC( #<sc_t>, #<sc_t>_#<sc_t> );\n",
                indent,
                compiler.nameof( signature->global_name ),
                o.st_name.sc,
                compiler.nameof( func.name )
            );
        }
    }
    sink.push_fa( "#rn{ }BCORE_REGISTER_OBJECT( #<sc_t> );\n", indent, o.st_name.sc );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

