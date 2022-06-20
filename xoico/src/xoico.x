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

include "bcore_std.h";

name mutable;
name const;
name void;
name int;
name long;
name float;
name double;
name type;
name aware;
name obliv;
name _;
name o;

forward :name_s;
forward :forward_s;
forward :arg_s;
forward :args_s;
forward :signature_s;
forward :body_s;
forward :feature_s;

forward :func_s;
forward :funcs_s;
forward :stamp_s;

forward :group_s;
forward :nested_group_s;

forward :source_s;
forward :target_s;
forward :compiler_s;
forward :transient_map_s;
forward :cengine;
forward :host;

signature er_t parse_sc( m @* o, c :host* host, sc_t sc );
signature er_t parse_fa( m @* o, c :host* host, sc_t format, ... );

// converts explicit types accordingly according to transient map and removes all transient types
feature 'ap' er_t convert_transient_types( m @* o, c :host* host, c :transient_map_s* map );

feature 'ap' tp_t get_hash                  ( c @* o );
feature 'ap' tp_t get_global_name_tp        ( c @* o );
feature 'ap' er_t parse                     ( m @* o, c :host* host, m x_source* source );
feature 'ap' er_t finalize                  ( m @* o, c :host* host ) { return 0; };  // final stage in the compilation phase
feature 'ap' er_t expand_setup              ( m @* o, c :host* host ) { return 0; };  // first stage in the expansion phase
feature 'ap' er_t expand_forward            ( c @* o, c :host* host, sz_t indent, m x_sink* sink ) { return 0; };
feature 'ap' er_t expand_indef_typedef      ( c @* o, c :host* host, sz_t indent, m x_sink* sink ) { return 0; };
feature 'ap' er_t expand_spect_declaration  ( c @* o, c :host* host, sz_t indent, m x_sink* sink ) { return 0; };
feature 'ap' er_t expand_spect_definition   ( c @* o, c :host* host, sz_t indent, m x_sink* sink ) { return 0; };
feature 'ap' er_t expand_declaration        ( c @* o, c :host* host, sz_t indent, m x_sink* sink ) { return 0; };
feature 'ap' er_t expand_indef_declaration  ( c @* o, c :host* host, sz_t indent, m x_sink* sink ) { return 0; };
feature 'ap' er_t expand_definition         ( c @* o, c :host* host, sz_t indent, m x_sink* sink ) { return 0; };
feature 'ap' er_t expand_init1              ( c @* o, c :host* host, sz_t indent, m x_sink* sink ) { return 0; };
feature 'ap' er_t expand_manifesto          ( c @* o, c :host* host, sz_t indent, m x_sink* sink ) { return 0; }; // manifestation: last expansion stage at which (group-)macros are immediately expanded
feature 'ap' c x_source_point_s* get_source_point( c @* o ) { ERR_fa( "Not implemented in #<sc_t>\n", bnameof( o._ ) ); return NULL; };

//----------------------------------------------------------------------------------------------------------------------
// functions

func er_t parse_f( m x_source* source, sc_t format )
{
    return source.parse_fa( format );
};

//----------------------------------------------------------------------------------------------------------------------

/// opens an include file from an include directive in parent
func er_t embed_file_open( m x_source* parent, sc_t file_name, m x_source** include_source )
{
    m st_s* folder = bcore_file_folder_path( parent.get_file() )^^;
    if( folder.size == 0 ) folder.push_char( '.' );

    m st_s* path = st_s!^^;
    if( file_name[ 0 ] == '/' )
    {
        path.copy_sc( file_name );
    }
    else
    {
        path.copy_fa( "#<sc_t>/#<sc_t>", folder.sc, file_name );
    }

    if( !bcore_file_exists( path.sc ) )
    {
        return parent.parse_error_fa( "Xoico: File '#<sc_t>' not found.", path->sc );
    }

    include_source.1 = bcore_file_open_source( path.sc );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func :.parse_sc
{
    return o.parse( host, x_source_create_from_sc( sc )^ );
};

//----------------------------------------------------------------------------------------------------------------------

func :.parse_fa
{
    va_list args;
    va_start( args, format );
    er_t ret = o.parse( host, x_source_create_from_fv( format, args )^ );
    va_end( args );
    return ret;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// external identifiers

//----------------------------------------------------------------------------------------------------------------------

identifier bcore_self_s_struct_body_to_sink_newline_escaped;
identifier bcore_flect_caps_is_array_fix;
identifier bcore_flect_caps_is_array_dyn;
identifier bcore_flect_caps_is_typed;
identifier bcore_flect_caps_get_indirection;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

