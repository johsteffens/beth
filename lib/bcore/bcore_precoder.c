/** Copyright 2019 Johannes Bernhard Steffens
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

#include "bcore.h"
#include "bcore_precoder.h"
#include "bcore_sources.h"
#include "bcore_sinks.h"
#include "bcore_spect_inst.h"
#include "bcore_life.h"
#include "bcore_sc.h"
#include "bcore_file.h"
#include "bcore_spect_hash.h"
#include "bcore_sr.h"
#include "bcore_hmap_tp_sr.h"
#include "bcore_const_manager.h"

/**********************************************************************************************************************/
/// parsing

/// removes comments, excessive whitespaces; keeps strings but replaces '"' with '\"'
static st_s* create_embedded_string( st_s* s )
{
    st_s* out = st_s_create();
    for( sz_t i = 0; i < s->size; i++ )
    {
        if( s->data[ i ] == '"' )
        {
            i++;
            st_s_push_char( out, s->data[ i ] );
            while( i < s->size && s->data[ i ] != '\n' ) i++;
        }

        if( s->data[ i ] == '/' && s->data[ i + 1 ] == '/' )
        {
            while( i < s->size && s->data[ i ] != '\n' ) i++;
        }
        else if( s->data[ i ] == '/' && s->data[ i + 1 ] == '*' )
        {
            while( i < s->size )
            {
                if( s->data[ i ] == '*' && s->data[ i + 1 ] == '/' )
                {
                    if( out->data[ out->size - 1 ] != ' ' ) st_s_push_char( out, ' ' );
                    i += 2;
                    break;
                }
            }
        }
        else if( s->data[ i ] == '"' )
        {
            st_s_push_char( out, '\\' );
            st_s_push_char( out, '"' );
            i++;
            while( i < s->size && s->data[ i ] != '"' ) st_s_push_char( out, s->data[ i++ ] );
            if( s->data[ i ] == '"' )
            {
                st_s_push_char( out, '\\' );
                st_s_push_char( out, '"' );
                i++;
            }
        }
        else if( s->data[ i ] == '\n' )
        {
            if( out->data[ out->size - 1 ] != ' ' ) st_s_push_char( out, ' ' );
        }
        else if( s->data[ i ] == '\t' )
        {
            if( out->data[ out->size - 1 ] != ' ' ) st_s_push_char( out, ' ' );
        }
        else if( s->data[ i ] == ' ' )
        {
            if( out->data[ out->size - 1 ] != ' ' ) st_s_push_char( out, ' ' );
        }
        else
        {
            st_s_push_char( out, s->data[ i ] );
        }
    }
    return out;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bcore_precoder_object_s )
{
    aware_t _;
    st_s self_source;
    bcore_self_s self;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_object_s )
"{"
    "aware_t _;"
    "st_s self_source;"
    "bcore_self_s self;"
"}";

/**********************************************************************************************************************/

BCORE_FORWARD_OBJECT( bcore_precoder_group_s );
BCORE_DECLARE_OBJECT( bcore_precoder_item_s )
{
    aware_t _;
    tp_t type;
    st_s name;
    tp_t hash;
    sr_s data;

    st_s source_name;
    st_s source_file;
    sz_t source_line;
    bcore_precoder_group_s* group;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_item_s )
"{"
    "aware_t _;"
    "tp_t type;"
    "st_s name;"
    "tp_t hash;"
    "sr_s data;"
    "st_s source_name;"
    "st_s source_file;"
    "sz_t source_line;"
    "private vd_t group;"
"}";

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_item_s_expand_declaration( const bcore_precoder_item_s* o, sz_t indent, bcore_sink* sink )
{
    switch( sr_s_type( &o->data ) )
    {
        case TYPEOF_bcore_precoder_object_s:
        {
            const bcore_precoder_object_s* precoder_object = o->data.o;
            bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, o->name.sc, typeof( o->name.sc ) );
            bcore_sink_a_push_fa( sink, "#rn{ }##define BETH_EXPAND_ITEM_#<sc_t> BCORE_DECLARE_OBJECT( #<sc_t> ) ", indent, o->name.sc, o->name.sc );
            bcore_self_s_struct_body_to_sink_single_line( &precoder_object->self, sink );
            bcore_sink_a_push_fa( sink, ";\n" );
        }
        break;

        default:
        {
            ERR_fa( "Unhandled: #<sc_t>\n", ifnameof( sr_s_type( &o->data ) ) );
        }
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_item_s_expand_definition( const bcore_precoder_item_s* o, sz_t indent, bcore_sink* sink )
{
    switch( sr_s_type( &o->data ) )
    {
        case TYPEOF_bcore_precoder_object_s:
        {
            const bcore_precoder_object_s* precoder_object = o->data.o;
            const bcore_self_s* self = &precoder_object->self;
            const st_s* self_string = &precoder_object->self_source;
            sz_t idx = st_s_find_char( self_string, 0, -1, '{' );
            sc_t self_body = "";
            if( idx < self_string->size ) self_body = self_string->sc + idx;
            bcore_sink_a_push_fa( sink, "#rn{ }BCORE_DEFINE_OBJECT_INST( #<sc_t>, #<sc_t> ) \"#<sc_t>\";\n", indent, ifnameof( self->trait ), ifnameof( self->type ), self_body );
        }
        break;

        default:
        {
            ERR_fa( "Unhandled: #<sc_t>\n", ifnameof( sr_s_type( &o->data ) ) );
        }
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_item_s_expand_init1( const bcore_precoder_item_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_OBJECT( #<sc_t> );\n", indent, o->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_item_s_setup( bcore_precoder_item_s* o, vc_t object )
{
    tp_t type = *( aware_t* )object;
    sr_down( o->data );
    o->data = sr_null();

    switch( type )
    {
        case TYPEOF_bcore_precoder_object_s:
        {
            const bcore_precoder_object_s* src = object;
            o->type = type;
            st_s_copy_sc( &o->name, nameof( src->self.type ) );
            o->hash = bcore_hash_a_get_tp( ( bcore_hash* )&src->self );
            o->data = sr_create_strong_typed( type, object );
        }
        break;

        default:
        {
            ERR_fa( "Unhandled: #<sc_t>\n", ifnameof( type ) );
        }
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_FORWARD_OBJECT( bcore_precoder_source_s );
BCORE_DECLARE_OBJECT( bcore_precoder_group_s )
{
    aware_t _;
    st_s name;
    tp_t id;
    tp_t hash;
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_precoder_item_s, );
    bcore_precoder_source_s* source;
};

BCORE_DEFINE_OBJECT_INST_AUT( bcore_inst, bcore_precoder_group_s )
"{"
    "aware_t _;"
    "st_s name;"
    "tp_t id;"
    "tp_t hash;"
    "bcore_precoder_item_s => [] arr;"
    "private vd_t source;"
"}";

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_compile( bcore_precoder_group_s* o, bcore_source* source )
{
    while( !bcore_source_a_eos( source ) )
    {
        if( bcore_source_a_parse_bl_fa( source, "#?'/*'" ) ) break;
        char c = bcore_source_a_get_u0( source );
        if( c != ' ' && c != '\t' && c != '\n' ) bcore_source_a_parse_err_fa( source, "Opening c-style comment '/*' expected." );
    }
    bcore_source_a_parse_fa( source, "#skip'*'" );

    while( !bcore_source_a_parse_bl_fa( source, " #?'*/'" ) )
    {
        if( bcore_source_a_eos( source ) )  bcore_source_a_parse_err_fa( source, "Closing c-style comment '*/' expected." );

        if( bcore_source_a_parse_bl_fa( source, " #?w'self' " ) )
        {
            BCORE_LIFE_INIT();
            s3_t source_index1 = bcore_source_a_get_index( source );
            bcore_self_s* self = BCORE_LIFE_A_PUSH( bcore_self_s_build_parse_source( source, 0 ) );
            s3_t source_index2 = bcore_source_a_get_index( source );
            bcore_source_a_set_index( source, source_index1 );

            BCORE_LIFE_CREATE( st_s, self_string );

            st_s_set_size( self_string, 0, source_index2 - source_index1 );
            bcore_source_a_get_data( source, self_string->data, source_index2 - source_index1 );
            st_s* self_embedded_string = BCORE_LIFE_A_PUSH( create_embedded_string( self_string ) );

            st_s st_weak = st_weak_st( self_embedded_string );
            bcore_self_s* embedded_self = BCORE_LIFE_A_PUSH( bcore_self_s_build_parse_source( ( bcore_source* )&st_weak, 0 ) );
            if( bcore_self_s_cmp( self, embedded_self ) != 0 )
            {
                bcore_source_a_parse_err_fa( source, "Precoder reflection embedding failed. Embedded code:\n#<sc_t>", self_embedded_string->sc );
            }

            BCORE_LIFE_CREATE( bcore_precoder_object_s, precoder_object );

            bcore_self_s_copy( &precoder_object->self, self );
            st_s_copy( &precoder_object->self_source, self_embedded_string );

            bcore_precoder_item_s* item = bcore_precoder_item_s_create();
            item->group = o;

            bcore_precoder_item_s_setup( item, precoder_object );

            o->hash = bcore_tp_fold_tp( o->hash, item->hash );
            bcore_array_a_push( ( bcore_array* )o, sr_asd( item ) );
            BCORE_LIFE_DOWN();
        }
        else
        {
            bcore_source_a_parse_err_fa( source, "Precode syntax error." );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_expand_declaration( const bcore_precoder_group_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, o->name.sc, typeof( o->name.sc ) );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_expand_declaration( o->data[ i ], indent, sink );
    bcore_sink_a_push_fa( sink, "#rn{ }##define BETH_EXPAND_GROUP_#<sc_t>", indent, o->name.sc );
    for( sz_t i = 0; i < o->size; i++ )
    {
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }    BETH_EXPAND_ITEM_#<sc_t>", indent, o->data[ i ]->name.sc );
    }
    bcore_sink_a_push_fa( sink, "\n" );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_expand_definition( const bcore_precoder_group_s* o, sz_t indent, bcore_sink* sink )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_expand_definition( o->data[ i ], indent, sink );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_expand_init1( const bcore_precoder_group_s* o, sz_t indent, bcore_sink* sink )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_expand_init1( o->data[ i ], indent, sink );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_FORWARD_OBJECT( bcore_precoder_target_s );
BCORE_DECLARE_OBJECT( bcore_precoder_source_s )
{
    aware_t _;
    st_s name; // source name (e.g. "bcore_precoder_sample")
    st_s path; // source path excluding extension
    tp_t hash;
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_precoder_group_s, );
    bcore_precoder_target_s* target;
};

BCORE_DEFINE_OBJECT_INST_AUT( bcore_inst, bcore_precoder_source_s )
"{"
    "aware_t _;"
    "st_s name;" // file name excluding directory and extension
    "st_s path;" // file path excluding extension
    "tp_t hash;"
    "bcore_precoder_group_s => [] arr;"
    "private vd_t target;"
"}";

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_source_s_compile( bcore_precoder_source_s* o, bcore_source* source )
{
    while( !bcore_source_a_eos( source ) )
    {
        if( bcore_source_a_parse_bl_fa( source, " #?'BETH_PRECODE'" ) )
        {
            bcore_precoder_group_s* group = bcore_precoder_group_s_create();
            group->source = o;
            bcore_source_a_parse_fa( source, " ( #name )", &group->name );
            group->id = typeof( group->name.sc );
            group->hash = group->id;
            bcore_precoder_group_s_compile( group, source );
            o->hash = bcore_tp_fold_tp( o->hash, group->hash );
            bcore_array_a_push( ( bcore_array* )o, sr_asd( group ) );
        }
        else
        {
            bcore_source_a_get_u0( source );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_source_s_expand_declaration( const bcore_precoder_source_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }// source: #<sc_t>\n", indent, o->name.sc );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_group_s_expand_declaration( o->data[ i ], indent, sink );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_source_s_expand_definition( const bcore_precoder_source_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }// source: #<sc_t>\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"#<sc_t>.h\"\n", indent, o->name.sc );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_group_s_expand_definition( o->data[ i ], indent, sink );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_source_s_expand_init1( const bcore_precoder_source_s* o, sz_t indent, bcore_sink* sink )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_group_s_expand_init1( o->data[ i ], indent, sink );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_FORWARD_OBJECT( bcore_precoder_s );
BCORE_DECLARE_OBJECT( bcore_precoder_target_s )
{
    aware_t _;
    st_s name; // target name (e.g. "bcore_precoder")
    st_s path; // path excluding extension
    tp_t hash;
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_precoder_source_s, );
    bcore_precoder_s* precoder;
};

BCORE_DEFINE_OBJECT_INST_AUT( bcore_inst, bcore_precoder_target_s )
"{"
    "aware_t _;"
    "st_s name;" // target name (e.g. "bcore_precoder")
    "st_s path;" // path excluding extension
    "tp_t hash;"
    "bcore_precoder_source_s => [] arr;"
    "private vd_t precoder;"
"}";

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_target_s_expand_license( const bcore_precoder_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "/** This file was generated by the beth-precode-compiler. Not suitable for manual editing.\n" );
    bcore_sink_a_push_fa( sink, " *\n" );
    bcore_sink_a_push_fa( sink, " *  Copyright and License:\n" );
    bcore_sink_a_push_fa( sink, " *      If a specification exists for the repository or folder in which the file is located,\n" );
    bcore_sink_a_push_fa( sink, " *      that specification shall apply. Otherwise, the Copyright and License of the associated\n" );
    bcore_sink_a_push_fa( sink, " *      source code files containing beth-precode shall apply.\n" );
    bcore_sink_a_push_fa( sink, " */\n" );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_target_s_expand_h( const bcore_precoder_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_precoder_target_s_expand_license( o, indent, sink );

    st_s* name_upper = st_s_clone( &o->name );
    st_s_set_uppercase( name_upper );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##ifndef #<sc_t>_H\n", indent, name_upper->sc );
    bcore_sink_a_push_fa( sink, "#rn{ }##define #<sc_t>_H\n", indent, name_upper->sc );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_control.h\"\n", indent );

    bcore_sink_a_push_fa( sink, "\n" );

    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_source_s_expand_declaration( o->data[ i ], indent, sink );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }vd_t #<sc_t>_signal_handler( const bcore_signal_s* o );\n", indent, o->name.sc );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##endif // #<sc_t>_H\n", indent, name_upper->sc );

    st_s_discard( name_upper );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_target_s_expand_init1( const bcore_precoder_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }bcore_const_x_set_d( typeof( \"#<sc_t>_hash\" ), sr_tp( #<tp_t> ) );\n", indent, o->name.sc, o->hash );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_target_s_expand_c( const bcore_precoder_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_precoder_target_s_expand_license( o, indent, sink );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"#<sc_t>.h\"\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_spect_inst.h\"\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_const_manager.h\"\n", indent );

    /// definition section
    bcore_sink_a_push_fa( sink, "\n" );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_source_s_expand_definition( o->data[ i ], indent, sink );

    /// signal section
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }vd_t #<sc_t>_signal_handler( const bcore_signal_s* o )\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }{\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    switch( bcore_signal_s_handle_type( o, typeof( \"#<sc_t>\" ) ) )\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }    {\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }        case TYPEOF_init1:\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }        {\n", indent );
    bcore_precoder_target_s_expand_init1( o, indent + 12, sink );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_source_s_expand_init1( o->data[ i ], indent + 12, sink );
    bcore_sink_a_push_fa( sink, "#rn{ }        }\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }        break;\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }        default: break;\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    }\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    return NULL;\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }}\n", indent );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_target_s_compile( bcore_precoder_target_s* o, sc_t source_path )
{
    BCORE_LIFE_INIT();

    st_s* source_name        = BCORE_LIFE_A_PUSH( bcore_file_strip_extension( bcore_file_name( source_path ) ) );
    st_s* source_folder_path = BCORE_LIFE_A_PUSH( bcore_file_folder_path( source_path ) );
    st_s* source_path_n      = BCORE_LIFE_A_PUSH( st_s_create_fa( "#<sc_t>/#<sc_t>", source_folder_path->sc, source_name->sc ) );
    st_s* source_path_h      = BCORE_LIFE_A_PUSH( st_s_create_fa( "#<sc_t>.h", source_path_n->sc ) );
    st_s* source_path_c      = BCORE_LIFE_A_PUSH( st_s_create_fa( "#<sc_t>.c", source_path_n->sc ) );

    bcore_precoder_source_s* precoder_source = bcore_precoder_source_s_create();
    precoder_source->target = o;
    st_s_copy_sc( &precoder_source->name, source_name->sc );
    st_s_copy   ( &precoder_source->path, source_path_n );

    precoder_source->hash = bcore_tp_init();

    if( bcore_file_exists( source_path_h->sc ) )
    {
        bcore_precoder_source_s_compile( precoder_source, BCORE_LIFE_A_PUSH( bcore_file_open_source( source_path_h->sc ) ) );
    }

    if( bcore_file_exists( source_path_c->sc ) ) bcore_precoder_source_s_compile( precoder_source, BCORE_LIFE_A_PUSH( bcore_file_open_source( source_path_c->sc ) ) );

    o->hash = bcore_tp_fold_tp( o->hash, precoder_source->hash );
    bcore_array_a_push( ( bcore_array* )o, sr_asd( precoder_source ) );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

/// returns true if a file was modified
static bl_t bcore_precoder_target_s_expand( bcore_precoder_target_s* o )
{
    BCORE_LIFE_INIT();
    bl_t modified = true;

    tp_t key = typeof( ( ( st_s* )BCORE_LIFE_A_PUSH( st_s_create_fa( "#<sc_t>_hash", o->name.sc ) ) )->sc );
    if( bcore_const_exists( key ) ) modified = ( *( tp_t* )bcore_const_get_o( key ) != o->hash );

    if( modified )
    {
        st_s* file_h = BCORE_LIFE_A_PUSH( st_s_create_fa( "#<sc_t>.h", o->path.sc ) );
        bcore_msg_fa( "writing '#<sc_t>'\n", file_h->sc );
        bcore_precoder_target_s_expand_h( o, 0, BCORE_LIFE_A_PUSH( bcore_file_open_sink( file_h->sc ) ) );
        st_s* file_c = BCORE_LIFE_A_PUSH( st_s_create_fa( "#<sc_t>.c", o->path.sc ) );
        bcore_msg_fa( "writing '#<sc_t>'\n", file_c->sc );
        bcore_precoder_target_s_expand_c( o, 0, BCORE_LIFE_A_PUSH( bcore_file_open_sink( file_c->sc ) ) );
    }

    BCORE_LIFE_RETURN( modified );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bcore_precoder_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_precoder_target_s, );
};

BCORE_DEFINE_OBJECT_INST_AUT( bcore_inst, bcore_precoder_s )
"{"
    "aware_t _;"
    "bcore_precoder_target_s => [] arr;"
"}";

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_s_compile( bcore_precoder_s* o, sc_t target_name, sc_t source_path )
{
    BCORE_LIFE_INIT();

    st_s* source_folder_path = BCORE_LIFE_A_PUSH( bcore_file_folder_path( source_path ) );
    st_s* target_path        = BCORE_LIFE_A_PUSH( st_s_create_fa( "#<sc_t>/#<sc_t>", source_folder_path->sc, target_name ) );

    sz_t target_index = -1;
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( st_s_equal_st( target_path, &o->data[ i ]->path ) )
        {
            target_index = i;
            break;
        }
    }

    if( target_index == -1 )
    {
        bcore_precoder_target_s* target = bcore_precoder_target_s_create();
        target->precoder = o;
        target->hash = bcore_tp_init();
        st_s_copy_sc( &target->name, target_name );
        st_s_copy(    &target->path, target_path );
        bcore_array_a_push( ( bcore_array* )o, sr_asd( target ) );
        target_index = o->size - 1;
    }

    bcore_precoder_target_s* target = o->data[ target_index ];
    bcore_precoder_target_s_compile( target, source_path );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

/// returns true if a file was modified
static bl_t bcore_precoder_s_expand( bcore_precoder_s* o )
{
//    bcore_txt_ml_a_to_stdout( o );
    bl_t modified = false;
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( bcore_precoder_target_s_expand( o->data[ i ] ) ) modified = true;
    }
    return modified;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// precoder interface

static bcore_precoder_s* precoder_g = NULL;

//----------------------------------------------------------------------------------------------------------------------

void bcore_precoder_compile( sc_t target_name, sc_t source_path )
{
    if( !precoder_g ) precoder_g = bcore_precoder_s_create();
    bcore_precoder_s_compile( precoder_g, target_name, source_path );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_precoder_expand()
{
    ASSERT( precoder_g );
    bl_t modified = false;
    f3_t time = 0;
    CPU_TIME_OF( modified = bcore_precoder_s_expand( precoder_g ), time );
    if( modified )
    {
        bcore_msg_fa( "beth-precoder expanding: #<f3_t> sec.\n", time );
        bcore_msg_fa( "Precoded files were updated. Rebuild is necessary.\n" );
    }
    bcore_precoder_s_discard( precoder_g );
    precoder_g = NULL;
    return modified;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_precoder_run_globally()
{
    f3_t time = 0;
    CPU_TIME_OF( bcore_run_signal_globally( TYPEOF_all, TYPEOF_precoder, NULL ), time );
    bcore_msg_fa( "beth-precoder compiling: #<f3_t> sec.\n", time );
    return bcore_precoder_expand();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bcore_precoder_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_precoder" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bcore_precoder_object_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_item_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_group_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_source_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_target_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_s );

        }
        break;

        case TYPEOF_down1:
        {
             if( precoder_g )  bcore_precoder_s_discard( precoder_g );
             precoder_g = NULL;
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_object_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_item_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_group_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_source_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_target_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_s );
        }
        break;

        case TYPEOF_selftest:
        {
//            bcore_precoder_run();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

