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

BCORE_DECLARE_OBJECT( bcore_precoder_item_s )
{
    aware_t _;
    tp_t type;
    tp_t id;
    tp_t hash;
    sr_s data;

    st_s source_name;
    st_s source_file;
    sz_t source_line;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_item_s )
"{"
    "aware_t _;"
    "tp_t type;"
    "tp_t id;"
    "tp_t hash;"
    "sr_s data;"
    "st_s source_name;"
    "st_s source_file;"
    "sz_t source_line;"
"}";

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
            o->id   = src->self.type;
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

static void bcore_precoder_item_s_write_declaration( const bcore_precoder_item_s* o, sz_t indent, bcore_sink* sink )
{
    switch( sr_s_type( &o->data ) )
    {
        case TYPEOF_bcore_precoder_object_s:
        {
            sc_t name = ifnameof( o->id );
            const bcore_precoder_object_s* precoder_object = o->data.o;
            bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, name, o->id );
            bcore_sink_a_push_fa( sink, "#rn{ }##define EMPLACE_OBJECT_#<sc_t> BCORE_DECLARE_OBJECT( #<sc_t> ) ", indent, name, name );
            bcore_self_s_struct_body_to_sink_single_line( &precoder_object->self, sink );
            bcore_sink_a_push_fa( sink, "\n" );
            bcore_sink_a_push_fa( sink, "\n" );
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

static void bcore_precoder_item_s_write_key( const bcore_precoder_item_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }//KEY #<tp_t> #<tp_t>\n", indent, o->id, o->hash );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_item_s_write_definition( const bcore_precoder_item_s* o, sz_t indent, bcore_sink* sink )
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

static void bcore_precoder_item_s_write_init1( const bcore_precoder_item_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_OBJECT( #<sc_t> );\n", indent, ifnameof( o->id ) );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bcore_precoder_target_s )
{
    aware_t _;
    st_s name; // target name (e.g. "bcore_precoder")
    st_s path; // path excluding extension
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_precoder_item_s, );
    bcore_hmap_tpuz_s hmap; // index map for above array
};

BCORE_DEFINE_OBJECT_INST_AUT( bcore_inst, bcore_precoder_target_s )
"{"
    "aware_t _;"
    "st_s name;" // target name (e.g. "bcore_precoder")
    "st_s path;" // path excluding extension
    "bcore_precoder_item_s => [] arr;"
    "bcore_hmap_tpuz_s hmap;" // index map for above array
"}";

//----------------------------------------------------------------------------------------------------------------------

/// pushes new item if not already defined; returns false in case item exists with different body
static bl_t bcore_precoder_target_s_push( bcore_precoder_target_s* o, sr_s item_sr )
{
    bl_t ret = true;
    const bcore_precoder_item_s* item = item_sr.o;
    if( bcore_hmap_tpuz_s_exists( &o->hmap, item->id ) )
    {
        sz_t idx = *bcore_hmap_tpuz_s_get( &o->hmap, item->id );
        if( o->data[ idx ]->hash != item->hash ) ret = false;
    }
    else
    {
        bcore_hmap_tpuz_s_set( &o->hmap, item->id, o->size );
        bcore_array_a_push( ( bcore_array* )o, item_sr );
    }
    sr_down( item_sr );
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_target_s_write_license( const bcore_precoder_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "/** This file is generated from 'precode' by the beth-precode-compiler.\n" );
    bcore_sink_a_push_fa( sink, " *\n" );
    bcore_sink_a_push_fa( sink, " *  Copyright and License:\n" );
    bcore_sink_a_push_fa( sink, " *      As specified for the repository in which this file resides.\n" );
    bcore_sink_a_push_fa( sink, " *      In case of doubt, the specification of the associated precode shall apply. \n" );
    bcore_sink_a_push_fa( sink, " *\n" );
    bcore_sink_a_push_fa( sink, " *  Not suitable for manual editing.\n" );
    bcore_sink_a_push_fa( sink, " */\n" );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_target_s_write_h( const bcore_precoder_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_precoder_target_s_write_license( o, indent, sink );

    st_s* name_upper = st_s_clone( &o->name );
    st_s_set_uppercase( name_upper );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##ifndef #<sc_t>_H\n", indent, name_upper->sc );
    bcore_sink_a_push_fa( sink, "#rn{ }##define #<sc_t>_H\n", indent, name_upper->sc );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_control.h\"\n", indent, name_upper->sc );

    bcore_sink_a_push_fa( sink, "\n" );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_write_declaration( o->data[ i ], indent, sink );
    bcore_sink_a_push_fa( sink, "#rn{ }vd_t #<sc_t>_signal_handler( const bcore_signal_s* o );\n", indent, o->name.sc );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##endif // #<sc_t>_H\n", indent, name_upper->sc );

    st_s_discard( name_upper );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_target_s_write_c( const bcore_precoder_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_precoder_target_s_write_license( o, indent, sink );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"#<sc_t>.h\"\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_spect_inst.h\"\n", indent );

    /// object specific includes
    bcore_sink_a_push_fa( sink, "\n" );
    {
        BCORE_LIFE_INIT();
        BCORE_LIFE_CREATE( bcore_arr_st_s, arr_st1 );
        BCORE_LIFE_CREATE( bcore_arr_st_s, arr_st2 );
        for( sz_t i = 0; i < o->size; i++ ) bcore_arr_st_s_push_st( arr_st1, &o->data[ i ]->source_name );
        bcore_arr_st_s_sort( arr_st1, 1 );

        /// remove duplicates
        for( sz_t i = 0; i < arr_st1->size; i++ )
        {
            if( arr_st2->size == 0 || !st_s_equal_st( arr_st1->data[ i ], arr_st2->data[ arr_st2->size - 1 ] ) )
            {
                bcore_arr_st_s_push_st( arr_st2, arr_st1->data[ i ] );
            }
        }

        for( sz_t i = 0; i < arr_st2->size; i++ )
        {
            bcore_sink_a_push_fa( sink, "#rn{ }##include \"#<sc_t>.h\"\n", indent, arr_st2->data[ i ]->sc );
        }

        BCORE_LIFE_DOWN();
    }

    /// key section
    bcore_sink_a_push_fa( sink, "\n" );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_write_key( o->data[ i ], indent, sink );

    /// definition section
    bcore_sink_a_push_fa( sink, "\n" );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_write_definition( o->data[ i ], indent, sink );

    /// signal section
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }vd_t #<sc_t>_signal_handler( const bcore_signal_s* o )\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }{\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    switch( bcore_signal_s_handle_type( o, typeof( \"#<sc_t>_precoder\" ) ) )\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }    {\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }        case TYPEOF_init1:\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }        {\n", indent );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_write_init1( o->data[ i ], indent + 12, sink );
    bcore_sink_a_push_fa( sink, "#rn{ }        }\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }        break;\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }        default: break;\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    }\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    return NULL;\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }}\n", indent );
}

//----------------------------------------------------------------------------------------------------------------------

static bl_t bcore_precoder_target_s_match_keys( const bcore_precoder_target_s* o )
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( st_s, file );
    st_s_push_fa( file, "#<sc_t>.c", o->path.sc );
    bcore_source* source = BCORE_LIFE_A_PUSH( bcore_file_open_source( file->sc ) );
    sz_t match_count = 0;

    while( !bcore_source_a_eos( source ) )
    {
        if( bcore_source_a_parse_bl_fa( source, "#?'//KEY'" ) )
        {
            tp_t id = 0;
            tp_t hash = 0;
            bcore_source_a_parse_fa( source, " #<tp_t*> #<tp_t*>", &id, &hash );
            if( !bcore_hmap_tpuz_s_exists( &o->hmap, id ) ) BCORE_LIFE_RETURN( false );
            tp_t hash1 = o->data[ *bcore_hmap_tpuz_s_get( &o->hmap, id ) ]->hash;
            if( hash1 != hash )  BCORE_LIFE_RETURN( false );
            match_count++;
        }
        else
        {
            bcore_source_a_get_u0( source );
        }
    }

    if( match_count == o->size ) BCORE_LIFE_RETURN( true );

    BCORE_LIFE_RETURN( false );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// source parsing

//----------------------------------------------------------------------------------------------------------------------

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

static void bcore_precoder_compile_source_target( bcore_source* source, bcore_precoder_target_s* target )
{
    BCORE_LIFE_INIT();

    ASSERT( target != NULL );
    sr_s source_sr = sr_awd( source );

    BCORE_LIFE_CREATE( st_s, source_file );
    BCORE_LIFE_CREATE( st_s, source_name );

    st_s_copy_sc( source_file, bcore_source_a_get_file( source ) );

    {
        sz_t idx = st_s_find_char( source_file, -1, 0, '/' ) + 1;
        if( idx >= source_file->size ) idx = 0;
        st_s_copy_sc( source_name, source_file->sc + idx );
        if( source_name->size >= 2 && source_name->data[ source_name->size - 2 ] == '.' )
        {
            source_name->data[ source_name->size - 2 ] = 0;
            source_name->size -= 2;
        }
    }

    while( !bcore_source_r_eos( &source_sr ) )
    {
        if( bcore_source_r_get_u0( &source_sr ) != '/' ) continue;

        sc_t precode_terminator = NULL;
        if( bcore_source_r_parse_bl_fa( &source_sr, "#?'*#PRECODE'" ) ||
            bcore_source_r_parse_bl_fa( &source_sr, "#?'**#PRECODE'" ) )
        {
            precode_terminator = "#?'*/'";
        }

        if( precode_terminator )
        {
            while( !bcore_source_r_eos( &source_sr ) &&
                   !bcore_source_r_parse_bl_fa( &source_sr, precode_terminator ) )
            {
                if( bcore_source_r_parse_bl_fa( &source_sr, " #?w'self' " ) )
                {
                    BCORE_LIFE_INIT();
                    s3_t source_index1 = bcore_source_r_get_index( &source_sr );
                    bcore_self_s* self = BCORE_LIFE_A_PUSH( bcore_self_s_build_parse_source( source, 0 ) );
                    s3_t source_index2 = bcore_source_r_get_index( &source_sr );
                    bcore_source_r_set_index( &source_sr, source_index1 );

                    BCORE_LIFE_CREATE( st_s, self_string );

                    st_s_set_size( self_string, 0, source_index2 - source_index1 );
                    bcore_source_r_get_data( &source_sr, self_string->data, source_index2 - source_index1 );
                    st_s* self_embedded_string = BCORE_LIFE_A_PUSH( create_embedded_string( self_string ) );

                    st_s st_weak = st_weak_st( self_embedded_string );
                    bcore_self_s* embedded_self = BCORE_LIFE_A_PUSH( bcore_self_s_build_parse_source( ( bcore_source* )&st_weak, 0 ) );
                    if( bcore_self_s_cmp( self, embedded_self ) != 0 )
                    {
                        bcore_source_r_parse_err_fa( &source_sr, "Precoder reflection embedding failed. Embedded code:\n#<sc_t>", self_embedded_string->sc );
                    }

                    BCORE_LIFE_CREATE( bcore_precoder_object_s, precoder_object );

                    bcore_self_s_copy( &precoder_object->self, self );
                    st_s_copy( &precoder_object->self_source, self_embedded_string );

                    BCORE_LIFE_CREATE( bcore_precoder_item_s, precoder_item );

                    bcore_precoder_item_s_setup( precoder_item, precoder_object );
                    st_s_copy( &precoder_item->source_file, source_file );
                    st_s_copy( &precoder_item->source_name, source_name );

                    if( !bcore_precoder_target_s_push( target, sr_awc( precoder_item ) ) )
                    {
                        sz_t idx = *bcore_hmap_tpuz_s_get( &target->hmap, precoder_item->id );
                        sc_t file = target->data[ idx ]->source_file.sc;
                        bcore_source_r_parse_err_fa( &source_sr, "Precoder reflection has already been defined with different body in file '#<sc_t>'.", file );
                    }

                    BCORE_LIFE_DOWN();
                }
                else
                {
                    bcore_source_r_parse_err_fa( &source_sr, "Precode syntax error." );
                }
            }
        }
        else
        {
            bcore_source_r_get_u0( &source_sr );
        }
    }
    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_compile_target( const char* file, bcore_precoder_target_s* target )
{
    bcore_source* source = bcore_file_open_source( file );
    bcore_precoder_compile_source_target( source, target );
    bcore_inst_a_discard( source );
}

//----------------------------------------------------------------------------------------------------------------------

/// returns true if a file was modified
static bl_t bcore_precoder_finalize_target( const bcore_precoder_target_s* target )
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( st_s, file_h );
    BCORE_LIFE_CREATE( st_s, file_c );
    st_s_push_fa( file_h, "#<sc_t>.h", target->path.sc );
    st_s_push_fa( file_c, "#<sc_t>.c", target->path.sc );

    if( !bcore_precoder_target_s_match_keys( target ) )
    {
        bcore_msg_fa( "writing #<sc_t>\n", file_h->sc );
        bcore_precoder_target_s_write_h( target, 0, BCORE_LIFE_A_PUSH( bcore_file_open_sink( file_h->sc ) ) );
        bcore_msg_fa( "writing #<sc_t>\n", file_c->sc );
        bcore_precoder_target_s_write_c( target, 0, BCORE_LIFE_A_PUSH( bcore_file_open_sink( file_c->sc ) ) );
        BCORE_LIFE_RETURN( true );
    }
    BCORE_LIFE_RETURN( false );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// precoder interface

static bcore_mutex_s*      mutex_compile = NULL;
static bcore_hmap_tp_sr_s* hmap_target   = NULL;

void bcore_precoder_compile( const char* file, const char* target_name )
{
    bcore_mutex_s_lock( mutex_compile );

    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( st_s, source_file );
    BCORE_LIFE_CREATE( st_s, target_file );
    st_s_copy_sc( source_file, file );

    {
        uz_t idx = st_s_find_char( source_file, source_file->size, 0, '/' );
        if( idx < source_file->size )
        {
            st_s_push_sc_n( target_file, source_file->sc, idx + 1 );
        }
        st_s_push_sc( target_file, target_name );
    }

    tp_t target_hash = typeof( target_file->sc );
    if( !bcore_hmap_tp_sr_s_exists( hmap_target, target_hash ) )
    {
        bcore_precoder_target_s* target = bcore_precoder_target_s_create();
        bcore_hmap_tp_sr_s_set( hmap_target, target_hash, sr_asd( target ) );
        st_s_copy( &target->path, target_file );
    }

    bcore_precoder_target_s* target = bcore_hmap_tp_sr_s_get( hmap_target, target_hash )->o;
    st_s_copy_sc( &target->name, target_name );

    if( source_file->size > 2 &&
        ( source_file->sc[ source_file->size - 1 ] == 'c' || source_file->sc[ source_file->size - 1 ] == 'h' ) &&
        source_file->sc[ source_file->size - 2 ] == '.' )
    {
        source_file->data[ source_file->size - 1 ] = 'h';
        if( bcore_file_exists( source_file->sc ) ) bcore_precoder_compile_target( source_file->sc, target );
        source_file->data[ source_file->size - 1 ] = 'c';
        if( bcore_file_exists( source_file->sc ) ) bcore_precoder_compile_target( source_file->sc, target );
    }
    else
    {
        bcore_precoder_compile_target( source_file->sc, target );
    }

    BCORE_LIFE_DOWN();

    bcore_mutex_s_unlock( mutex_compile );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_precoder_finalize()
{
    bl_t modified = false;
    bcore_mutex_s_lock( mutex_compile );
    for( sz_t i = 0; i < hmap_target->size; i++ )
    {
        tp_t key = bcore_hmap_tp_sr_s_idx_key( hmap_target, i );
        if( !key ) continue;
        bcore_precoder_target_s* target = bcore_hmap_tp_sr_s_idx_val( hmap_target, i )->o;
        if( bcore_precoder_finalize_target( target ) ) modified = true;
    }
    bcore_hmap_tp_sr_s_clear( hmap_target );
    bcore_mutex_s_unlock( mutex_compile );
    return modified;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_precoder_run_globally()
{
    bcore_run_signal_globally( TYPEOF_all, TYPEOF_precoder, NULL );
    return bcore_precoder_finalize();
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
            BCORE_REGISTER_OBJECT( bcore_precoder_target_s );
            if( !mutex_compile ) mutex_compile = bcore_mutex_s_create();
            if( !hmap_target )   hmap_target = bcore_hmap_tp_sr_s_create();

        }
        break;

        case TYPEOF_down1:
        {
             if( hmap_target )   bcore_hmap_tp_sr_s_discard( hmap_target );
             if( mutex_compile ) bcore_mutex_s_discard( mutex_compile );
             hmap_target = NULL;
             mutex_compile = NULL;
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_object_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_item_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_target_s );
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

