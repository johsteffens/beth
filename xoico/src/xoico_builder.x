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

/// returns index of target
signature er_t load( m @* o, bl_t readonly, sc_t path );
signature er_t build( m @* o );
signature c @* name_match( c @* o, sc_t name );

signature void push_target_index_to_arr( c @* o, m bcore_arr_sz_s* arr );

stamp :arr_target_s = aware x_array { :target_s => []; };

stamp :target_s = aware :
{
    st_s => name;             // unique target name
    st_s => extension = "xo"; // extension used for xoila output files
    st_s => root_folder;      // root folder of subsequent file paths (used if they are relative)
    bl_t readonly;

    /** Folder in which to store *.xo.* files
     *  A parent's output folder, if defined, takes precedence.
     *  If no output folder was defined anywhere, the source folder is used as output.
     */
    st_s => output_folder;
    func st_s* root_output_folder(@* o)
    {
        st_s* folder = ( o.parent_ ) ? o.parent_.root_output_folder() : NULL;
        return folder ? folder : o.output_folder;
    };

    /** copyright_and_licence_terms: If specified, the associated *.xo.* files carry this text
     *  in the appropriate location for the license preamble.
     */
    st_s => copyright_and_license_terms;

    bcore_arr_st_s dependencies; // dependent target definitions
    bcore_arr_st_s sources;      // array of source files

    /** Function name of principal signal handler for this target
     *  If not defined, it is assumed that the name is <name>_general_signal_handler
     */
    st_s => signal_handler;

    /** Implements <name>_general_signal_handler in xo.c
     *  Set to false if a signal handler is manually implemented for given target.
     */
    bl_t define_signal_handler = true;

    /// Optional cengine that is to be used in all bodies of this target
    aware xoico_cengine => cengine = xoico_che_s;

    private xoico_compiler_s* compiler;

    // Runtime data
    private @* parent_;
    private @* root_;
    hidden  aware :arr_target_s => dependencies_target_;
    hidden  st_s                   full_path_;
    hidden  sz_t                   target_index_ = -1; // Index for target on the compiler; -1 if this target has no representation
    hidden  bcore_hmap_tpvd_s   => hmap_built_target_; // map of targets that have already been built

    func bcore_via_call.source
    {
        if( !o->root_folder )
        {
            o->root_folder = bcore_file_folder_path( source.get_file() );
            o.root_folder =< bcore_file_path_minimized( o.root_folder.sc );
        }
    };

    func :.name_match
    {
        if( o.name && sc_t_equal( name, o.name.sc ) ) return o;
        if( o.parent_ ) return o->parent_.name_match( name );
        return NULL;
    };

    func :.push_target_index_to_arr
    {
        if( o.target_index_ != -1 )
        {
             arr.push( o.target_index_ );
        }
        else
        {
            foreach( m $* e in o.dependencies_target_ ) e.push_target_index_to_arr( arr );
        }
    };

    func xoico.get_hash
    {
        tp_t hash = bcore_tp_init();
        hash = o.name ? bcore_tp_fold_sc( hash, o.name.sc ) : hash;
        hash = o.extension ? bcore_tp_fold_sc( hash, o.extension.sc ) : hash;
        hash = o.root_folder ? bcore_tp_fold_sc( hash, o.root_folder.sc ) : hash;
        foreach( st_s* e in o.dependencies ) hash = bcore_tp_fold_sc( hash, e.sc );
        foreach( st_s* e in o.sources ) hash = bcore_tp_fold_sc( hash, e.sc );
        hash = o.signal_handler ? bcore_tp_fold_sc( hash, o.signal_handler.sc ) : hash;
        hash = bcore_tp_fold_bl( hash, o.define_signal_handler );
        hash = o.cengine ? bcore_tp_fold_bl( hash, o.cengine.get_hash() ) : hash;
        return hash;
    };

    func :.load;
    func :.build;
};

//----------------------------------------------------------------------------------------------------------------------

signature er_t build_from_file( m @* o, sc_t path );
signature bl_t update_required( c @* o );
signature er_t update         ( c @* o );

signature er_t set_dry_run( m @* o, bl_t v );
signature bl_t get_dry_run( c @* o );

signature er_t set_always_expand( m @* o, bl_t v );
signature bl_t get_always_expand( c @* o );

signature er_t set_overwrite_unsigned_target_files( m @* o, bl_t v );
signature bl_t get_overwrite_unsigned_target_files( c @* o );

stamp :main_s = aware :
{
    /// Folder in which to store *.xo.* files (all dependencies)
    st_s => output_folder;

    xoico_compiler_s => compiler!;
    :target_s => target;

    func :.build_from_file;
    func :.update_required { return o.compiler.update_required(); };
    func :.update;

    func :.set_dry_run
    {
        o.compiler.dry_run = v;
        return 0;
    };

    func :.get_dry_run
    {
        return o.compiler.dry_run;
    };

    func :.set_always_expand
    {
        o.compiler.always_expand = v;
        return 0;
    };

    func :.get_always_expand
    {
        return o.compiler.always_expand;
    };

    func :.set_overwrite_unsigned_target_files
    {
        o.compiler.overwrite_unsigned_target_files = v;
        return 0;
    };

    func :.get_overwrite_unsigned_target_files
    {
        return o.compiler.overwrite_unsigned_target_files;
    };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:target_s) :.load
{
    m st_s* st_path = st_s!^^;
    st_path.copy_sc( path );

    if( st_path->sc[ 0 ] != '/' )
    {
        m st_s* current_folder = st_s!^^;
        bcore_folder_get_current( current_folder );
        st_path = st_s_create_fa( "#<sc_t>/#<sc_t>", current_folder.sc, st_path.sc )^^;
    }

    st_path = bcore_file_path_minimized( st_path.sc )^^;

    o.cast( m x_btml* ).from_file( st_path.sc );
    o->full_path_.copy( st_path );
    if( readonly ) o.readonly = true;

    if( !o.name ) return bcore_error_push_fa( general_error~, "In target file: '#<sc_t>'\nTarget name must be specified.", st_path->sc );

    /// check for dependency cycles
    if( o.parent_ )
    {
        c xoico_builder_target_s* match = o->parent_.name_match( o->name.sc );
        if( match )
        {
            if( match.full_path_.equal_st( o.full_path_ ) )
            {
                return bcore_error_push_fa( general_error~, "In target file: '#<sc_t>'\nCyclic dependency detected.", st_path.sc );
            }
            else
            {
                return bcore_error_push_fa( general_error~, "Same target name '#<sc_t>' used in different target files:\n#<sc_t>\n#<sc_t>", o.name.sc, st_path.sc, match.full_path_.sc );
            }
        }
    }

    foreach( m $* e in o.dependencies )
    {
        if( !o.dependencies_target_ ) o.dependencies_target_ = xoico_builder_arr_target_s!;

        m st_s* file_path = st_s!^;
        if( e.sc[ 0 ] != '/' )
        {
            if( o.root_folder ) file_path.push_fa( "#<sc_t>/", o.root_folder.sc );
        }

        bl_t dep_readonly = o.readonly;

        m x_source* source = x_source_create_from_sc( e.sc )^;
        source.parse_fa( " #:until':'", file_path );

        /// remove trailing spaces
        while( file_path.sc[ 0 ] == ' ' || file_path.sc[ 0 ] == '\t' ) file_path.pop_char();

        if( source.parse_bl( "#?':'" ) )
        {
            if( source.parse_bl( " #?w'readonly'" ) )
            {
                dep_readonly = true;
            }
            else
            {
                return source.parse_error_fa( "Syntax error in dependency declaration." );
            }
        }

        m xoico_builder_target_s* target = o->dependencies_target_.push_d( xoico_builder_target_s! );
        target.parent_ = o;

        target.load( dep_readonly, file_path->sc );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:target_s) :.build
{
    if( !o.root_    ) o.root_    = ( o.parent_ ) ? o.parent_.root_    : o;
    if( !o.compiler ) o.compiler = ( o.parent_ ) ? o.parent_.compiler : NULL;

    if( o == o.root_ )
    {
        if( !o.hmap_built_target_ ) o.hmap_built_target_ = bcore_hmap_tpvd_s!;
    }

    ASSERT( o.compiler );
    ASSERT( o.root_ );

    tp_t tp_target_name = bentypeof( o.name.sc );

    foreach( m $* e in o.dependencies_target_ ) e.build();

    if( o.root_.hmap_built_target_.exists( tp_target_name ) )
    {
        m xoico_builder_target_s* target = o.root_.hmap_built_target_.get( tp_target_name ).cast( m xoico_builder_target_s.2 ).1;
        o.target_index_ = target.target_index_;
        return 0;
    }

    o.root_.hmap_built_target_.set( tp_target_name, ( vd_t )o );

    o.target_index_ = -1;

    bcore_msg_fa( "XOICO: compiling #<sc_t>\n", o.full_path_.sc );

    foreach( $* e in o.sources )
    {
        m x_source* source = x_source_create_from_st( e )^;
        st_s^ group_name;
        st_s^ trait_name;
        st_s^ file_path;
        source.parse_fa( " " ); // take whitespaces

        if( source.parse_bl( "#?w'group'" ) )
        {
            source.parse_fa( " #name", group_name.1 );
            if( group_name.size == 0 ) return source.parse_error_fa( "Group name expected in source declaration." );
            source.parse_fa( " = #name", trait_name.1 );
            if( trait_name.size == 0 ) return source.parse_error_fa( "Trait name expected in source declaration." );
            source.parse_fa( " " );
        }

        while( !source.eos() ) file_path.push_char( source.get_char() );
        if( file_path.size == 0 ) return source.parse_error_fa( "File name expected in source declaration." );

        if( e.sc[ 0 ] != '/' && o.root_folder )
        {
            st_s* tmp = file_path.clone()^;
            file_path.copy_fa( "#<sc_t>/#<sc_t>", o.root_folder.sc, tmp.sc );
        }

        if( group_name.size == 0 && file_path.ends_in_sc( ".x" ) )
        {
            group_name.copy( bcore_file_strip_extension( bcore_file_name( file_path.sc ) )^ );
            trait_name.copy_sc( "x_inst" );
        }

        ASSERT( o.name );
        ASSERT( o.extension );
        sz_t index = -1;

        if( group_name.size == 0 )
        {
            if( !file_path.ends_in_sc( ".h" ) )
            {
                return source.parse_error_fa( "File name should have extension *.h" );
            }
            o.compiler.parse( o.name.sc, o.extension.sc, o.root_output_folder(), file_path.sc, NULL, NULL, index.1 );
        }
        else
        {
            if( !file_path.ends_in_sc( ".x" ) )
            {
                return source.parse_error_fa( "File name should have extension *.x" );
            }
            o.compiler.parse( o.name.sc, o.extension.sc, o.root_output_folder(), file_path.sc, group_name.sc, trait_name.sc, index.1 );
        }

        if( o.target_index_ == -1 ) o.target_index_ = index;
        if( index != o.target_index_ )
        {
            return bcore_error_push_fa
            (
                general_error~,
                "Building source '#<sc_t>' in target '#<sc_t>':\n"
                "Target index mismatch.\n"
                "This problem occurs when a target defines sources from different directories.\n",
                e.sc,
                o.name.sc
            );
        }
    }

    if( o.target_index_ >= 0 )
    {
        ASSERT( o.target_index_ < o.compiler.size );
        m bcore_arr_sz_s* dependencies = bcore_arr_sz_s!^;
        foreach( m $* e in o->dependencies_target_ ) e.push_target_index_to_arr( dependencies );

        m xoico_target_s* target = o.compiler.[ o.target_index_ ];

        target.set_dependencies( dependencies );
        m st_s* signal_handler = st_s_create_fa( "#<sc_t>_general_signal_handler", o.name.sc )^^;
        if( o.signal_handler ) signal_handler.copy( o.signal_handler );
        target.signal_handler_name.copy_sc( signal_handler.sc );
        target.define_signal_handler = o.define_signal_handler;
        target.readonly = o.readonly;
        target.cengine =< o.cengine.fork();
        target.pre_hash = o.get_hash();
        target.copyright_and_license_terms =< o.copyright_and_license_terms.fork();
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:main_s) :.build_from_file
{
    o.target =< xoico_builder_target_s!;
    o.target.load( false, path );
    o.target.compiler = o.compiler;
    if( o.output_folder ) o.target.output_folder =< o.output_folder.fork();
    o.target.build();
    o.compiler.finalize( o );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:main_s) :.update
{
    if( bcore_error_stack_size() > 0 ) return error_stack~;
    try( o.compiler.update_target_files( NULL ) );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

