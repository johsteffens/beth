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

signature er_t register_item(  m @* o, c xoico* item );
signature er_t register_group( m @* o, c xoico_group_s* group ); // no effect if group was already registered
signature er_t register_func(  m @* o, c xoico_func_s* func );
signature er_t register_external_type( m @* o, tp_t type );
signature er_t register_external_identifier( m @* o, tp_t name );

signature bl_t is_item(  c @* o, tp_t name );
signature bl_t is_group( c @* o, tp_t name ); // checks if name represents a registered group
signature bl_t is_func(  c @* o, tp_t name ); // checks if name represents the global name of a function (==name of implementation)
signature bl_t is_stamp( c @* o, tp_t name ); // checks if name represents a registered stamp
signature bl_t is_body(  c @* o, tp_t name ); // checks if name represents a registered body
signature bl_t is_identifier( c @* o, tp_t name ); // checks if name represents a registered identifier (either group, stamp, external type, name or external identifier)
signature bl_t is_type(  c @* o, tp_t name ); // checks if name represents a registered type (either group, stamp or external type)
signature bl_t is_name(  c @* o, tp_t name ); // checks if name represents a registered name (declared with keyword 'name')
signature bl_t is_signature( c @* o, tp_t name ); // checks if name represents a registered signature
signature bl_t is_feature( c @* o, tp_t name ); // checks if name represents a registered feature
signature bl_t is_signature_or_feature( c @* o, tp_t name ); // checks if name represents a registered signature or feature

// getters below return NULL in case object was not registered
signature m xoico* get_item( m @* o, tp_t name );
signature c xoico* get_const_item( c @* o,  tp_t name );
signature m xoico_stamp_s* get_stamp( m @* o, tp_t name );
signature m xoico_body_s*  get_body(  m @* o, tp_t name );
signature m xoico_func_s*  get_func(  m @* o, tp_t name );
signature m xoico_group_s* get_group( m @* o, tp_t name );
signature m xoico_transient_map_s* get_transient_map( m @* o, tp_t type );
signature c xoico_feature_s* get_feature( c @* o, tp_t name );

/// returns signature in case name represents a signature or feature
signature c xoico_signature_s* get_signature( c @* o, tp_t name );

signature er_t life_a_push(     m @* o, m bcore_inst* object );

/** If body_signature is != 0 it is matched against the file and a clear_overwrite oly set in case
 *  the file's body signature differs
 */
signature er_t check_overwrite( c @* o, sc_t file, tp_t body_signature, mutable bl_t* clear_to_overwrite );

signature bl_t get_self(        c @* o, tp_t type, c bcore_self_s** self ); // returns success


stamp :type_info_s = aware :
{
    xoico_typespec_s typespec;
    hidden aware xoico* item; // group or stamp
};

stamp :element_info_s = aware :
{
    :type_info_s type_info;
    hidden xoico_func_s* func; // NULL in case element is no function
};

signature bl_t get_type_info(               c @* o, tp_t type,            m :type_info_s*    info );

//signature bl_t get_type_element_info(         c @* o, tp_t type, tp_t name, m :element_info_s* info );
signature bl_t get_type_member_function_info( c @* o, tp_t type, tp_t name, m :element_info_s* info );
signature bl_t get_type_member_object_info(   c @* o, tp_t type, tp_t name, m :element_info_s* info );

signature bl_t get_type_array_element_info( c @* o, tp_t type,            m :element_info_s* info );

// external interface ...
signature er_t parse
(
    m @* o,
    sc_t  target_name,
    sc_t  target_ext,
    st_s* target_output_folder, // can be NULL (in which case the source code folder is also the output folder)
    sc_t  source_path,
    sc_t  group_name, // can be NULL
    sc_t  trait_name, // can be NULL
    m sz_t* p_target_index
);

signature er_t update_target_files( m @* o, m bl_t* p_modified );
signature bl_t update_required    ( m @* o );
signature sz_t get_verbosity      ( c @* o );
signature tp_t entypeof           ( m @* o, sc_t name );
signature sc_t nameof             ( c @* o, tp_t type );

stamp :s = aware :
{
    hidden xoico_target_s => [];
    hidden bcore_life_s life; // lifetime manager for items generation during processing

    hidden bcore_hmap_tpvd_s hmap_group;
    hidden bcore_hmap_tpvd_s hmap_item;
    hidden bcore_hmap_tpvd_s hmap_func;  // maps the global name of a function to the func instance
    hidden bcore_hmap_tp_s hmap_external_type; // externally registered types (keyword 'type')
    hidden bcore_hmap_tp_s hmap_external_identifier; // externally registered identifiers (keyword 'identifier')
    hidden bcore_hmap_tp_s hmap_declared_name; // declared names

    hidden bcore_hmap_name_s name_map;   // general name manager

    hidden bl_t has_main_function = false; // a main function was declared somewhere in the project

    // parameters

    /** target_pre_hash: Changing this value changes the hash of all targets.
     *  Purpose: To force rebuild all targets created by an older compiler.
     *  This is used during development or when a new version changes the
     *  target_out files in a material way.
     */
    tp_t target_pre_hash                 = 71;
    bl_t work_build_time_into_pre_hash   = true;
    bl_t register_signatures             = false;
    bl_t overwrite_unsigned_target_files = false;
    bl_t always_expand                   = false; // true: always expands targets even when the hash has not changed;
    bl_t dry_run                         = false; // dry_run: performs target computation but does not update target files
    sz_t verbosity                       = 1;

    // functions
    func xoico.finalize;
    func xoico.expand_setup;

    func :.register_item;
    func :.register_group;
    func :.register_func;
    func :.register_external_type { o.hmap_external_type.set( type ); return 0; };
    func :.register_external_identifier { o.hmap_external_identifier.set( name ); return 0; };

    func :.is_item  { return o.hmap_item.exists( name ); };
    func :.is_group { return o.hmap_group.exists( name ); };
    func :.is_func  { return o.hmap_func.exists( name ); };

    func :.is_type
    {
        if( o.is_group( name ) ) return true;
        if( o.is_stamp( name ) ) return true;
        if( o.hmap_external_type.exists( name ) ) return true;
        return false;
    };

    func :.is_identifier
    {
        if( o.is_type( name ) ) return true;
        if( o.hmap_declared_name.exists( name ) ) return true;
        if( o.hmap_external_identifier.exists( name ) ) return true;
        return false;
    };

    func :.is_name
    {
        if( o.hmap_declared_name.exists( name ) ) return true;
        return false;
    };

    func :.is_stamp
    {
        c xoico* item = o.get_const_item( name );
        if( item && ( item->_ == xoico_stamp_s~ ) ) return true;
        return false;
    };

    func :.is_body
    {
        c xoico* item = o.get_const_item( name );
        if( item && ( item->_ == xoico_body_s~ ) ) return true;
        return false;
    };

    func :.is_signature
    {
        c xoico* item = o.get_const_item( name );
        if( item && ( item->_ == xoico_signature_s~ ) ) return true;
        return false;
    };

    func :.is_signature_or_feature
    {
        c xoico* item = o.get_const_item( name );
        if( !item ) return NULL;
        if( item->_ == xoico_signature_s~ || item->_ == xoico_feature_s~ ) return true;
        return false;
    };

    func :.is_feature
    {
        c xoico* item = o.get_const_item( name );
        if( item && ( item->_ == xoico_feature_s~ ) ) return true;
        return false;
    };

    func :.get_const_item
    {
        vd_t* ptr = o.hmap_item.get( name );
        return ptr ? ( const xoico* )*ptr : NULL;
    };

    func :.get_item
    {
        vd_t* ptr = o.hmap_item.get( name );
        return ptr ? ( xoico* )*ptr : NULL;
    };

    func :.get_stamp
    {
        c xoico* item = o.get_const_item( name );
        return ( item ) ? ( item._ == xoico_stamp_s~ ) ? item.cast( m xoico_stamp_s* ) : NULL : NULL;
    };

    func :.get_body
    {
        c xoico* item = o.get_const_item( name );
        return ( item ) ? ( item._ == xoico_body_s~ ) ? item.cast( m xoico_body_s* ) : NULL : NULL;
    };

    func :.get_feature
    {
        c xoico* item = o.get_const_item( name );
        return ( item ) ? ( item._ == xoico_feature_s~ ) ? item.cast( m xoico_feature_s* ) : NULL : NULL;
    };

    func :.get_signature
    {
        c xoico* item = o.get_const_item( name );
        if( !item ) return NULL;
        if( item._ == xoico_signature_s~ ) return item.cast( m xoico_signature_s* );
        if( item._ == xoico_feature_s~   ) return item.cast( m xoico_feature_s* ).signature;
        return NULL;
    };

    func :.get_group
    {
        vd_t* ptr = o.hmap_group.get( name );
        return ptr ? ( xoico_group_s* )*ptr : NULL;
    };

    func :.get_transient_map
    {
        m xoico_stamp_s* stamp = o.get_stamp( type );
        return stamp ? stamp.transient_map.1 : NULL;
    };

    func :.get_func
    {
        vd_t* ptr = o.hmap_func.get( name );
        return ptr ? ( xoico_func_s* )*ptr : NULL;
    };

    func :.life_a_push
    {
        bcore_life_s_push_aware( o.life.1, object );
        return 0;
    };

    func :.check_overwrite;
    func :.get_self;
    func :.get_type_info;
    //func :.get_type_element_info;
    func :.get_type_member_function_info;
    func :.get_type_member_object_info;
    func :.get_type_array_element_info;

    // external interface ...
    func :.parse;
    func :.update_target_files;
    func :.update_required { return o.to_be_modified(); };
    func :.get_verbosity { return o.verbosity; };

    func :.entypeof { return o.name_map.set_sc( name ); };
    func :.nameof   { return o.name_map.get_sc( type ); };

    func bcore_inst_call.init_x
    {
        if( o-> work_build_time_into_pre_hash )
        {
            o->target_pre_hash = bcore_tp_fold_sc( o->target_pre_hash, __DATE__ );
            o->target_pre_hash = bcore_tp_fold_sc( o->target_pre_hash, __TIME__ );
        }
    };

    func void push_d( m @* o, d xoico_target_s* target )
    {
        o.cast( m x_array* ).push_d( target );
    };

    /// clears flags in targets
    func void clear_flags( m @* o ) { foreach( m $* e in o ) e.flag = false; };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.register_group
{
    if( o.hmap_group.exists( group.tp_name ) ) return 0;
    o.hmap_group.set( group.tp_name, ( vd_t )group );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.register_item
{
    tp_t global_id = item.get_global_name_tp();
    if( o.hmap_item.exists( global_id ) )
    {
        return item.get_source_point().cast( m$* ).parse_error_fa( "'#<sc_t>' was already registered\n", o.nameof( global_id ) );
    }
    o.hmap_item.set( global_id, ( vd_t )item );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.register_func
{
    if( o.hmap_func.exists( func.global_name ) )
    {
        m xoico_func_s* func_registered = o.hmap_func.get( func.global_name ).cast( m xoico_func_s** );

        if( func_registered == func )
        {
            return x_source_point_s_parse_error_fa
            (
                func.source_point,
                "Global func name '#<sc_t>' was already registered from same instance.\n",
                o.nameof( func.global_name )
            );
        }
        else
        {
            return x_source_point_s_parse_error_fa
            (
                func.source_point,
                "Global func name '#<sc_t>' was already registered from different instance.\n"
                "'#<sc_t>' vs '#<sc_t>'\n",
                o.nameof( func.global_name ),
                o.nameof( func.name ),
                o.nameof( func_registered.name )
            );
        }
    }

    o.hmap_func.set( func.global_name, ( vd_t )func );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/// returns true if correct file signature could be verified
func bl_t is_correctly_signed( st_s* data )
{
    if( data.size < bcore_strlen( "// XOICO_FILE_SIGNATURE" ) ) return false;

    sz_t idx = data.size - 1;
    while( idx >= 0 && data.[ idx ] != '/' ) idx--;
    if( idx > 0 ) idx--;

    if( data.find_sc( idx, -1, "// XOICO_FILE_SIGNATURE" ) != idx ) return false;

    tp_t hash = 0;
    sz_t end_idx = data.parse_fa( idx, -1, "// XOICO_FILE_SIGNATURE #<tp_t*> ", hash.1 );

    if( end_idx != data.size ) return false;

    data.[ idx ] = 0;

    if( hash != bcore_tp_fold_sc( bcore_tp_init(), data.sc ) ) return false;

    return true;
};

//----------------------------------------------------------------------------------------------------------------------

/// returns true if correct file signature could be verified
func tp_t body_signature( st_s* data )
{
    // search from bottom upwards ...
    sz_t idx = 0;
    if( ( idx = data.find_sc( data.size, 0, "// XOICO_BODY_SIGNATURE" ) ) == data.size ) return 0;

    tp_t hash = 0;
    data.parse_fa( idx, -1, "// XOICO_BODY_SIGNATURE #<tp_t*>", hash.1 );

    return hash;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.check_overwrite
{
    if( clear_to_overwrite ) clear_to_overwrite.0 = true;
    if( !bcore_file_exists( file ) ) return 0;

    m x_source* source = bcore_file_open_source( file )^;
    m st_s* data = st_s!^;
    while( !source.eos() ) data.push_char( source.get_u0() );

    if( !o.is_correctly_signed( data ) )
    {
        m st_s* s = st_s!^;
        s.push_fa( "Planted file #<sc_t>: Signature check failed.\n", file );
        s.push_fa( "This file might have been created or edited outside the xoico framework.\n" );
        if( o.overwrite_unsigned_target_files )
        {
            if( clear_to_overwrite ) clear_to_overwrite.0 = true;
            s.push_fa( "Flag 'overwrite_unsigned_target_files' is 'true'. The file will be overwritten.\n" );
            x_sink_stderr().push_fa( "\nWARNING: #<sc_t>\n", s->sc );
        }
        else
        {
            if( clear_to_overwrite ) clear_to_overwrite.0 = false;
            s.push_fa( "Xoico has currently no permission to overwrite unsigned target files.\n" );
            s.push_fa( "You can fix it in one of following ways:\n" );
            s.push_fa( "* Rename or (re)move the file.\n" );
            s.push_fa( "* Use command line flag '-f'.\n" );
            return bcore_error_push_fa( general_error~, "\nERROR: #<sc_t>\n", s->sc );
        }
    }

    if( clear_to_overwrite && body_signature && !o.always_expand )
    {
        clear_to_overwrite.0 = o.body_signature( data ) != body_signature;
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.parse
{
    st_s* source_folder = bcore_file_folder_path( source_path )^;
    st_s* output_folder = target_output_folder ? target_output_folder : source_folder;
    m st_s* target_output_path = st_s_create_fa( "#<sc_t>/#<sc_t>.#<sc_t>", output_folder.sc, target_name, target_ext )^;

    sz_t target_index = -1;
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( sc_t_equal( target_name, o.[ i ].name.sc ) )
        {
            target_index = i;
            break;
        }
    }

    if( target_index == -1 )
    {
        d xoico_target_s* target = xoico_target_s!;
        target.compiler = o;
        target.name        .copy_sc( target_name );
        target.ext         .copy_sc( target_ext );
        target.include_path.copy_fa( "#<sc_t>.#<sc_t>", target_name, target_ext );
        target.output_path.copy( target_output_path );
        o.push_d( target );
        target_index = o->size - 1;
    }

    m xoico_target_s* target = o.[ target_index ];

    /// check paths and extensions
    if( !target.ext.equal_sc( target_ext ) )
    {
        ERR_fa( "Target '#<sc_t>': Assigned extension '#<sc_t>' differs from requested extension '#<sc_t>'.", target.name.sc, target.ext.sc, target_ext );
    }
    if( !target.output_path.equal_st( target_output_path ) )
    {
        ERR_fa( "Target '#<sc_t>': Assigned output path '#<sc_t>' differs from requested output path '#<sc_t>'.", target.name.sc, target.output_path.sc, target_output_path.sc );
    }

    target.parse_from_path( source_path, group_name, trait_name );
    if( p_target_index ) p_target_index.0 = target_index;

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.finalize
{
    foreach( m $* e in o ) e.finalize( o );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.expand_setup
{
    for( sz_t i = 0; i < o->size; i++ ) o.[ i ].expand_setup( o );

    /* We obtain all names from the global map.
     * This is necessary because names of reflection elements are parsed outside
     * this framework and remembered by the global map.
     */
    bcore_name_push_all( o.name_map.1 );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/// returns true if a file will be modified in function xoico_compiler_s_expand
func (:s) bl_t to_be_modified( c @* o )
{
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o.[ i ].to_be_modified() ) return true;
    }
    return false;
};

//----------------------------------------------------------------------------------------------------------------------

/// returns true if a file was modified
func (:s) er_t expand( m @* o, m bl_t* p_modified )
{
    bl_t modified = false;

    o.expand_setup( o );
    for( sz_t i = 0; i < o->size; i++ ) o.[i].expand_phase1( modified.1 );
    for( sz_t i = 0; i < o->size; i++ ) o.[i].expand_phase2( modified.1 );

    if( p_modified ) p_modified.0 = modified;
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.get_self
{
    c xoico* item = o.get_const_item( type );
    if( !item ) return false;

    if( item->_ == xoico_stamp_s~ )
    {
        c xoico_stamp_s* stamp = item.cast( c xoico_stamp_s* );
        if( self ) self.1 = stamp->self;
        return true;
    }

    return false;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.get_type_info
{
    c xoico* item = o.get_const_item( type );
    if( !item ) return false;
    ASSERT( info );
    if( item->_ == xoico_stamp_s~ || item->_ == xoico_group_s~ )
    {
        info.item = item.cast( m $* );
        info.typespec.type = type;
        return true;
    }

    return false;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) get_type_member_function_info
{
    c xoico* xoico_item = o.get_const_item( type );
    if( !xoico_item )
    {
        xoico_item = o.cast( m $* ).get_group( type );
        if( !xoico_item ) return false;
    }

    bl_t success = false;
    ASSERT( info );
    info.type_info.item = xoico_item.cast( m $* );

    if( xoico_item->_ == xoico_stamp_s~ )
    {
        c xoico_stamp_s* stamp = xoico_item.cast( c xoico_stamp_s* );
        c bcore_self_s* self = stamp.self;
        c bcore_self_item_s* self_item = NULL; // returns NULL in case of no match

        if( ( info.func = stamp.get_traitline_func_from_name( name ).cast( m $* ) ) ) /// trait-line function
        {
            success = true;
        }
        else if( ( self_item = self.get_item_by_name( name ) ) )
        {
            if( self_item.caps == BCORE_CAPS_EXTERNAL_FUNC )
            {
                info.func = stamp.get_func_from_name( name ).cast( m $* );
                ASSERT( info.func );
                success = true;
            }
        }
    }
    else if( xoico_item._ == xoico_group_s~ )
    {
        c $* group = xoico_item.cast( m xoico_group_s* );
        info.func = group.get_traitline_func_from_name( name ).cast( m $* );
        if( info.func ) success = true;
    }

    return success;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) get_type_member_object_info
{
    c xoico* xoico_item = o.get_const_item( type );
    if( !xoico_item )
    {
        xoico_item = o.cast( m $* ).get_group( type );
        if( !xoico_item ) return false;
    }

    bl_t success = false;
    ASSERT( info );
    info.type_info.item = xoico_item.cast( m $* );

    if( xoico_item->_ == xoico_stamp_s~ )
    {
        c xoico_stamp_s* stamp = xoico_item.cast( c xoico_stamp_s* );
        c bcore_self_s* self = stamp.self;
        c bcore_self_item_s* self_item = NULL; // returns NULL in case of no match

        if( ( self_item = self.get_item_by_name( name ) ) )
        {
            if( bcore_flect_caps_is_array_fix( self_item.caps ) )
            {
                info.type_info.typespec.access_class = TYPEOF_mutable;
                info.type_info.typespec.type = self_item.type;
                info.type_info.typespec.indirection = 1;
                success = true;
            }
            else if( !bcore_flect_caps_is_array_dyn( self_item.caps ) ) // dynamic arrays are handled separately
            {
                sz_t indirection = bcore_flect_caps_get_indirection( self_item.caps );
                info.type_info.typespec.type = self_item.type;
                info.type_info.typespec.indirection = indirection;
                info.type_info.typespec.access_class = ( indirection > 0 ) ? TYPEOF_mutable : 0;
                success = true;
            }
        }
        else if( ( self_item = bcore_self_s_get_first_anonymous_array_item( self ) ) ) /// builtin elements for arrays
        {
            if( name == TYPEOF_size )
            {
                info.type_info.typespec.type = uz_t~;
                info.type_info.typespec.indirection = 0;
                info.type_info.typespec.access_class = 0;
                success = true;
            }
            else if( name == TYPEOF_space )
            {
                info.type_info.typespec.type = uz_t~;
                info.type_info.typespec.indirection = 0;
                info.type_info.typespec.access_class = 0;
                success = true;
            }
            else if( name == TYPEOF_data )
            {
                info.type_info.typespec.access_class = TYPEOF_mutable;
                info.type_info.typespec.type = self_item.type ? self_item.type : x_inst~;
                info.type_info.typespec.indirection = bcore_flect_caps_get_indirection( self_item.caps ) + 1;
                success = true;
            }
            else if( name == TYPEOF_type )
            {
                if( bcore_flect_caps_is_typed( self_item.caps ) )
                {
                    info.type_info.typespec.type = tp_t~;
                    info.type_info.typespec.indirection = 0;
                    info.type_info.typespec.access_class = 0;
                    success = true;
                }
            }
        }
    }
    else if( xoico_item._ == xoico_group_s~ )
    {
        if( name == TYPEOF__ ) // group builtin element '_'
        {
            info.type_info.typespec.type = aware_t~;
            info.type_info.typespec.indirection = 0;
            success = true;
        }
    }

    return success;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.get_type_array_element_info
{
    ASSERT( info );

    c xoico* xoico_item = o.get_const_item( type );
    if( !xoico_item ) return false;
    bl_t success = false;
    info.type_info.item = xoico_item.cast( m $* );

    if( xoico_item->_ == xoico_stamp_s~ )
    {
        c xoico_stamp_s* stamp = xoico_item.cast( c xoico_stamp_s* );
        c bcore_self_item_s* self_item = bcore_self_s_get_first_anonymous_array_item( stamp.self );
        if( self_item )
        {
            info.type_info.typespec.type = self_item.type;
            info.type_info.typespec.indirection = bcore_flect_caps_get_indirection( self_item.caps );
            success = true;
        }
    }

    return success;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// xoico interface

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.update_target_files
{
    bl_t modified = false;
    bl_t verbosity = o.verbosity;
    f3_t time = 0;

    er_t er = 0;
    ABS_TIME_OF( er = o.expand( modified.1 ), time );
    try( er );

    if( modified )
    {
        if( verbosity > 0 ) bcore_msg_fa( "XOICO: Expanded in #<f3_t> sec.\n", time );
        if( verbosity > 0 ) bcore_msg_fa( "XOICO: Files were updated. Rebuild is necessary.\n" );
    }

    if( p_modified ) p_modified.0 = modified;

    return er;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
