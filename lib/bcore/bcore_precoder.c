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
#include "bcore_leaf.h"
#include "bcore_hmap_tp_sr.h"
#include "bcore_const_manager.h"

/**********************************************************************************************************************/
/// parsing

/// removes comments, excessive whitespaces; trailing whitespaces; keeps strings but replaces '"' with '\"'
static st_s* create_embedded_string( const st_s* s )
{
    st_s* out = st_s_create();
    for( sz_t i = 0; i < s->size; i++ )
    {
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
    if( out->size > 0 && out->data[ out->size - 1 ] == ' ' )
    {
        out->data[ out->size - 1 ] = 0;
        out->size--;
    }
    return out;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bcore_precoder_arg_s )
{
    aware_t _;
    st_s type;
    st_s name;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_arg_s )
"{"
    "aware_t _;"
    "st_s type;"
    "st_s name;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_precoder_args_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( bcore_precoder_arg_s, );
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_args_s )
"{"
    "aware_t _;"
    "bcore_precoder_arg_s [] arr;"
"}";

//----------------------------------------------------------------------------------------------------------------------

// example:
// feature void myfeature( mutable, sz_t a, sz_t b );
BCORE_DECLARE_OBJECT( bcore_precoder_feature_s )
{
    aware_t _;
    st_s name;       // myfeature
    sc_t group_name;
    sc_t item_name;

    bl_t has_ret;
    st_s ret_type;        // void
    bcore_precoder_args_s args;       // sz_t a, sz_t b
    bl_t mutable;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_feature_s )
"{"
    "aware_t _;"
    "st_s name;"       // myfeature
    "sc_t group_name;"
    "sc_t item_name;"
    "bl_t has_ret;"
    "st_s ret_type;"        // void
    "bcore_precoder_args_s args;"       // sz_t a, sz_t b
    "bl_t mutable;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_precoder_object_s )
{
    aware_t _;
    sc_t item_name;
    st_s self_source;
    bcore_self_s self;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_object_s )
"{"
    "aware_t _;"
    "sc_t item_name;"
    "st_s self_source;"
    "bcore_self_s self;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_FORWARD_OBJECT( bcore_precoder_s );
BCORE_FORWARD_OBJECT( bcore_precoder_group_s );
BCORE_DECLARE_OBJECT( bcore_precoder_item_s )
{
    aware_t _;
    tp_t type;
    st_s name;
    tp_t id; // typeof( name )
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
    "tp_t id;"   // typeof( name )
    "tp_t hash;"
    "sr_s data;"
    "st_s source_name;"
    "st_s source_file;"
    "sz_t source_line;"
    "private vd_t group;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_FORWARD_OBJECT( bcore_precoder_source_s );
BCORE_DECLARE_OBJECT( bcore_precoder_group_s )
{
    aware_t _;
    st_s name;
    tp_t id;
    tp_t hash;
    bl_t has_features;
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_precoder_item_s, );
    bcore_precoder_source_s* source;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_group_s )
"{"
    "aware_t _;"
    "st_s name;"
    "tp_t id;"
    "tp_t hash;"
    "bl_t has_features;"
    "bcore_precoder_item_s => [] arr;"
    "private vd_t source;"
"}";

//----------------------------------------------------------------------------------------------------------------------

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

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_source_s )
"{"
    "aware_t _;"
    "st_s name;" // file name excluding directory and extension
    "st_s path;" // file path excluding extension
    "tp_t hash;"
    "bcore_precoder_group_s => [] arr;"
    "private vd_t target;"
"}";

//----------------------------------------------------------------------------------------------------------------------

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

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_target_s )
"{"
    "aware_t _;"
    "st_s name;" // target name (e.g. "bcore_precoder")
    "st_s path;" // path excluding extension
    "tp_t hash;"
    "bcore_precoder_source_s => [] arr;"
    "private vd_t precoder;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_precoder_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_precoder_target_s, );
    bcore_hmap_tpvd_s hmap_group;
    bcore_hmap_tpvd_s hmap_item;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_s )
"{"
    "aware_t _;"
    "bcore_precoder_target_s => [] arr;"
    "bcore_hmap_tpvd_s hmap_group;"
    "bcore_hmap_tpvd_s hmap_item;"
"}";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static bl_t bcore_precoder_s_register_item( bcore_precoder_s* o, const bcore_precoder_item_s* item );

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_args_s_clear( bcore_precoder_args_s* o )
{
    bcore_array_a_set_space( ( bcore_array* ) o, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_arg_s_compile( bcore_precoder_arg_s* o, bcore_source* source )
{
    bcore_source_a_parse_fa( source, "#name #name", &o->type, &o->name );
    if( o->type.size == 0 ) bcore_source_a_parse_err_fa( source, "Argument: Type expected." );
    if( o->name.size == 0 ) bcore_source_a_parse_err_fa( source, "Argument: Name expected." );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_args_s_compile( bcore_precoder_args_s* o, bcore_source* source )
{
    bcore_precoder_args_s_clear( o );
    while( bcore_source_a_parse_bl_fa( source, " #?',' " ) ) // args follow
    {
        bcore_precoder_arg_s* arg = bcore_precoder_arg_s_create();
        bcore_precoder_arg_s_compile( arg, source );
        bcore_array_a_push( ( bcore_array* ) o, sr_asd( arg ) );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_arg_s_expand( const bcore_precoder_arg_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#<sc_t> #<sc_t>", o->type.sc, o->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_arg_s_expand_name( const bcore_precoder_arg_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#<sc_t>", o->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_args_s_expand( const bcore_precoder_args_s* o, bcore_sink* sink )
{
    for( sz_t i = 0; i < o->size; i++ )
    {
        bcore_sink_a_push_fa( sink, ", " );
        bcore_precoder_arg_s_expand( &o->data[ i ], sink );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_args_s_expand_name( const bcore_precoder_args_s* o, bcore_sink* sink )
{
    for( sz_t i = 0; i < o->size; i++ )
    {
        bcore_sink_a_push_fa( sink, ", " );
        bcore_precoder_arg_s_expand_name( &o->data[ i ], sink );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_precoder_arg_s_get_hash( const bcore_precoder_arg_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->type.sc );
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_precoder_args_s_get_hash( const bcore_precoder_args_s* o )
{
    tp_t hash = bcore_tp_init();
    for( sz_t i = 0; i < o->size; i++ ) hash = bcore_tp_fold_tp( hash, bcore_precoder_arg_s_get_hash( &o->data[ i ] ) );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_feature_s_compile( bcore_precoder_feature_s* o, bcore_source* source )
{
    bcore_source_a_parse_fa( source, " #name #name ( ", &o->ret_type, &o->name );
    if( o->name.size == 0      ) bcore_source_a_parse_err_fa( source, "Feature: Name missing." );
    if( o->ret_type.size  == 0 ) bcore_source_a_parse_err_fa( source, "Feature: Return type missing." );
    o->has_ret = !st_s_equal_sc( &o->ret_type, "void" );

    if(      bcore_source_a_parse_bl_fa(  source, " #?'mutable' " ) ) o->mutable = true;
    else if( bcore_source_a_parse_bl_fa(  source, " #?'const' "   ) ) o->mutable = false;
    else     bcore_source_a_parse_err_fa( source, "Feature: 'mutable' or 'const' expected." );

    bcore_precoder_args_s_compile( &o->args, source );
    bcore_source_a_parse_fa( source, " ) ; " );
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_precoder_feature_s_get_hash( const bcore_precoder_feature_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    hash = bcore_tp_fold_sc( hash, o->group_name );
    hash = bcore_tp_fold_u0( hash, o->has_ret ? 1 : 0 );
    hash = bcore_tp_fold_sc( hash, o->ret_type.sc );
    hash = bcore_tp_fold_tp( hash, bcore_precoder_args_s_get_hash( &o->args ) );
    hash = bcore_tp_fold_u0( hash, o->mutable ? 1 : 0 );
    return hash;
}

static void bcore_precoder_feature_s_expand_default( const bcore_precoder_feature_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }static #<sc_t> #<sc_t>_default( ", indent, o->ret_type.sc, o->item_name );
    bcore_sink_a_push_fa( sink, "#<sc_t>#<sc_t>* o", o->mutable ? "" : "const ", o->group_name );
    bcore_precoder_args_s_expand( &o->args, sink );
    bcore_sink_a_push_fa( sink, " )\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }{\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    ERR_fa( \"Feature has no binding.\" );\n", indent );
    if( o->has_ret ) bcore_sink_a_push_fa( sink, "#rn{ }    return *(#<sc_t>*)NULL;\n", indent, o->ret_type.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }}\n", indent );
}

//----------------------------------------------------------------------------------------------------------------------

//void bcore_precoder_sample_features_t_setup( const bcore_precoder_sample_features_s* p, bcore_precoder_sample_features* o, sz_t n ) { bcore_precoder_sample_features_s_get_aware( o )->setup( o, n ); }
/*
static void bcore_precoder_feature_s_expand_virtual_p( const bcore_precoder_feature_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }#<sc_t> #<sc_t>_p_#<sc_t>( const #<sc_t>_s* p, ", indent, o->ret_type.sc, o->group_name, o->name.sc, o->group_name );
    bcore_sink_a_push_fa( sink, "#<sc_t>#<sc_t>* o", o->mutable ? "" : "const ", o->group_name );
    bcore_precoder_args_s_expand( &o->args, sink );
    bcore_sink_a_push_fa( sink, " ) { " );
    if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
    bcore_sink_a_push_fa( sink, "p->#<sc_t>( o", o->name.sc );
    bcore_precoder_args_s_expand_name( &o->args, sink );
    bcore_sink_a_push_fa( sink, " ); }\n" );
}
*/

//----------------------------------------------------------------------------------------------------------------------

//void bcore_precoder_sample_features_t_setup( tp_t t, bcore_precoder_sample_features* o, sz_t n ) { bcore_precoder_sample_features_s_get_aware( o )->setup( o, n ); }
static void bcore_precoder_feature_s_expand_virtual_t( const bcore_precoder_feature_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }#<sc_t> #<sc_t>_t_#<sc_t>( tp_t __t, ", indent, o->ret_type.sc, o->group_name, o->name.sc );
    bcore_sink_a_push_fa( sink, "#<sc_t>#<sc_t>* o", o->mutable ? "" : "const ", o->group_name );
    bcore_precoder_args_s_expand( &o->args, sink );
    bcore_sink_a_push_fa( sink, " )\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }{\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    ", indent );
    if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
    bcore_sink_a_push_fa( sink, "#<sc_t>_s_get_typed( __t )->#<sc_t>( o", o->group_name, o->name.sc );
    bcore_precoder_args_s_expand_name( &o->args, sink );
    bcore_sink_a_push_fa( sink, " );\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }}\n", indent );
}

//----------------------------------------------------------------------------------------------------------------------

//void bcore_precoder_sample_features_a_setup( bcore_precoder_sample_features* o, sz_t n ) { bcore_precoder_sample_features_s_get_aware( o )->setup( o, n ); }
static void bcore_precoder_feature_s_expand_virtual_a( const bcore_precoder_feature_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }#<sc_t> #<sc_t>_a_#<sc_t>( ", indent, o->ret_type.sc, o->group_name, o->name.sc );
    bcore_sink_a_push_fa( sink, "#<sc_t>#<sc_t>* o", o->mutable ? "" : "const ", o->group_name );
    bcore_precoder_args_s_expand( &o->args, sink );
    bcore_sink_a_push_fa( sink, " )\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }{\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    ", indent );
    if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
    bcore_sink_a_push_fa( sink, "#<sc_t>_s_get_aware( o )->#<sc_t>( o", o->group_name, o->name.sc );
    bcore_precoder_args_s_expand_name( &o->args, sink );
    bcore_sink_a_push_fa( sink, " );\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }}\n", indent );
}

//----------------------------------------------------------------------------------------------------------------------

//void bcore_precoder_sample_features_r_setup( const sr_s* o, sz_t n ) { bcore_precoder_sample_features_s_get_typed( o->o_type )->setup( o->o, n ); }
static void bcore_precoder_feature_s_expand_virtual_r( const bcore_precoder_feature_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }#<sc_t> #<sc_t>_r_#<sc_t>( const sr_s* o", indent, o->ret_type.sc, o->group_name, o->name.sc );
    bcore_precoder_args_s_expand( &o->args, sink );
    bcore_sink_a_push_fa( sink, " )\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }{\n", indent );

    if( o->mutable ) bcore_sink_a_push_fa( sink, "#rn{ }    ASSERT( !sr_s_is_const( o ) );\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    ", indent );

    if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
    bcore_sink_a_push_fa( sink, "( (#<sc_t>_s*)ch_spect_p( o->p, TYPEOF_#<sc_t>_s ) )->#<sc_t>( o->o", o->group_name, o->group_name, o->name.sc );
    bcore_precoder_args_s_expand_name( &o->args, sink );
    bcore_sink_a_push_fa( sink, " );\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }}\n", indent );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_object_s_expand_declaration( const bcore_precoder_object_s* o, const bcore_precoder_s* precoder, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, o->item_name, typeof( o->item_name ) );

    bcore_sink_a_push_fa( sink, "#rn{ }##define BETH_EXPAND_ITEM_#<sc_t>", indent, o->item_name, o->item_name );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  BCORE_DECLARE_OBJECT( #<sc_t> ) ", indent, o->item_name );
    bcore_self_s_struct_body_to_sink_single_line( &o->self, sink );
    bcore_sink_a_push_fa( sink, ";" );

    sz_t items = bcore_self_s_items_size( &o->self );
    for( sz_t i = 0; i < items; i++ )
    {
        const bcore_self_item_s* self_item = bcore_self_s_get_item( &o->self, i );
        if( self_item->caps == BCORE_CAPS_EXTERNAL_FUNC && bcore_hmap_tpvd_s_exists( &precoder->hmap_item, self_item->type ) )
        {
            const bcore_precoder_item_s* item = *bcore_hmap_tpvd_s_get( &precoder->hmap_item, self_item->type );
            ASSERT( sr_s_type( &item->data ) == TYPEOF_bcore_precoder_feature_s );
            const bcore_precoder_feature_s* feature = item->data.o;
            bcore_sink_a_push_fa( sink, " \\\n#rn{ }  #<sc_t> #<sc_t>_#<sc_t>( ", indent, feature->ret_type.sc, o->item_name, ifnameof( self_item->name ) );
            bcore_sink_a_push_fa( sink, "#<sc_t>", feature->mutable ? "" : "const " );
            bcore_sink_a_push_fa( sink, "#<sc_t>* o", o->item_name );
            bcore_precoder_args_s_expand( &feature->args, sink );
            bcore_sink_a_push_fa( sink, " );" );
        }
    }

    bcore_sink_a_push_fa( sink, "\n" );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_object_s_expand_init1( const bcore_precoder_object_s* o, const bcore_precoder_s* precoder, sz_t indent, bcore_sink* sink )
{
    sz_t items = bcore_self_s_items_size( &o->self );
    for( sz_t i = 0; i < items; i++ )
    {
        const bcore_self_item_s* self_item = bcore_self_s_get_item( &o->self, i );
        if( self_item->caps == BCORE_CAPS_EXTERNAL_FUNC && bcore_hmap_tpvd_s_exists( &precoder->hmap_item, self_item->type ) )
        {
            const bcore_precoder_item_s* item = *bcore_hmap_tpvd_s_get( &precoder->hmap_item, self_item->type );
            ASSERT( sr_s_type( &item->data ) == TYPEOF_bcore_precoder_feature_s );
            const bcore_precoder_feature_s* feature = item->data.o;
            bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_FFUNC( #<sc_t>, #<sc_t>_#<sc_t> );\n", indent, feature->item_name, o->item_name, ifnameof( self_item->name ) );
        }
    }
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_OBJECT( #<sc_t> );\n", indent, o->item_name );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_feature_s_expand_declaration( const bcore_precoder_feature_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }##define BETH_EXPAND_ITEM_#<sc_t>", indent, o->item_name );

    //typedef ret_t (*feature_func)( feature* o, arg_t arg1 );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  typedef #<sc_t> (*#<sc_t>_#<sc_t>)(", indent, o->ret_type.sc, o->group_name, o->name.sc );
    if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
    bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group_name );
    bcore_precoder_args_s_expand( &o->args, sink );
    bcore_sink_a_push_fa( sink, " );" );
/*
    // ret_t feature_p_func( const spect* p, feature* o, arg_t arg1 );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  #<sc_t> #<sc_t>_p_#<sc_t>( const #<sc_t>_s* p,", indent, o->ret_type.sc, o->group_name, o->name.sc, o->group_name );
    if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
    bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group_name );
    bcore_precoder_args_s_expand( &o->args, sink );
    bcore_sink_a_push_fa( sink, " );" );
*/
    // ret_t feature_t_func( tp_t t, feature* o, arg_t arg1 );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  #<sc_t> #<sc_t>_t_#<sc_t>( tp_t t,", indent, o->ret_type.sc, o->group_name, o->name.sc, o->group_name );
    if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
    bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group_name );
    bcore_precoder_args_s_expand( &o->args, sink );
    bcore_sink_a_push_fa( sink, " );" );

    // ret_t feature_a_func( feature* o, arg_t arg1 );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  #<sc_t> #<sc_t>_a_#<sc_t>(", indent, o->ret_type.sc, o->group_name, o->name.sc );
    if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
    bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group_name );
    bcore_precoder_args_s_expand( &o->args, sink );
    bcore_sink_a_push_fa( sink, " );" );

    // ret_t feature_r_func( const sr_s* o, arg_t arg1 );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  #<sc_t> #<sc_t>_r_#<sc_t>(", indent, o->ret_type.sc, o->group_name, o->name.sc );
    bcore_sink_a_push_fa( sink, " const sr_s* o" );
    bcore_precoder_args_s_expand( &o->args, sink );
    bcore_sink_a_push_fa( sink, " );" );

    bcore_sink_a_push_fa( sink, "\n" );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_item_s_expand_declaration( const bcore_precoder_item_s* o, sz_t indent, bcore_sink* sink )
{
    switch( sr_s_type( &o->data ) )
    {
        case TYPEOF_bcore_precoder_object_s:
        {
            bcore_precoder_object_s_expand_declaration( o->data.o, o->group->source->target->precoder, indent, sink );
        }
        break;

        case TYPEOF_bcore_precoder_feature_s:
        {
            bcore_precoder_feature_s_expand_declaration( o->data.o, indent, sink );
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
            bcore_sink_a_push_fa( sink, "\n" );
            bcore_sink_a_push_fa( sink, "#rn{ }BCORE_DEFINE_OBJECT_INST( #<sc_t>, #<sc_t> )\\\n", indent, ifnameof( self->trait ), ifnameof( self->type ) );
            bcore_sink_a_push_fa( sink, "#rn{ }  \"#<sc_t>\";\n", indent, self_body );
        }
        break;

        case TYPEOF_bcore_precoder_feature_s:
        {
            /* nothing */
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
    switch( sr_s_type( &o->data ) )
    {
        case TYPEOF_bcore_precoder_object_s:
        {
            bcore_precoder_object_s_expand_init1( o->data.o, o->group->source->target->precoder, indent, sink );
        }
        break;

        case TYPEOF_bcore_precoder_feature_s:
        {
            bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_FEATURE( #<sc_t> );\n", indent, o->name.sc );
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

static void bcore_precoder_item_s_setup( bcore_precoder_item_s* o, vc_t object )
{
    tp_t type = *( aware_t* )object;
    sr_down( o->data );
    o->data = sr_null();
    o->type = type;
    o->data = sr_create_strong_typed( type, object );

    switch( type )
    {
        case TYPEOF_bcore_precoder_object_s:
        {
            const bcore_precoder_object_s* precoder_object = object;
            st_s_copy_sc( &o->name, nameof( precoder_object->self.type ) );
            o->hash = bcore_hash_a_get_tp( ( bcore_hash* )&precoder_object->self );
            ( ( bcore_precoder_object_s* )o->data.o )->item_name = o->name.sc;
        }
        break;

        case TYPEOF_bcore_precoder_feature_s:
        {
            const bcore_precoder_feature_s* precoder_feature = object;
            st_s_copy_fa( &o->name, "#<sc_t>_#<sc_t>", precoder_feature->group_name, precoder_feature->name.sc );
            o->hash = bcore_precoder_feature_s_get_hash( precoder_feature );
            ( ( bcore_precoder_feature_s* )o->data.o )->item_name = o->name.sc;
        }
        break;

        default:
        {
            ERR_fa( "Unhandled: #<sc_t>\n", ifnameof( type ) );
        }
        break;
    }

    o->id = typeof( o->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static bl_t bcore_precoder_s_register_group( bcore_precoder_s* o, const bcore_precoder_group_s* group );

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

        bcore_precoder_item_s* item = bcore_precoder_item_s_create();
        item->group = o;

        if( bcore_source_a_parse_bl_fa( source, " #?w'self' " ) )
        {
            BCORE_LIFE_INIT();
            s3_t source_index1 = bcore_source_a_get_index( source );
            bcore_self_s* self = BCORE_LIFE_A_PUSH( bcore_self_s_parse_source( source, 0, false ) );
            s3_t source_index2 = bcore_source_a_get_index( source );
            bcore_source_a_set_index( source, source_index1 );

            BCORE_LIFE_CREATE( st_s, self_string );

            st_s_set_size( self_string, 0, source_index2 - source_index1 );
            bcore_source_a_get_data( source, self_string->data, source_index2 - source_index1 );
            st_s* self_embedded_string = BCORE_LIFE_A_PUSH( create_embedded_string( self_string ) );

            /// 4095 is the C99-limit for string literals
            if( self_embedded_string->size > 4095 )
            {
                bcore_source_a_parse_err_fa( source, "Precoder reflection embedding failed. Embedded code needs a string literal larger than 4095 characters." );
            }

            st_s st_weak = st_weak_st( self_embedded_string );
            bcore_self_s* embedded_self = BCORE_LIFE_A_PUSH( bcore_self_s_parse_source( ( bcore_source* )&st_weak, 0, false ) );
            if( bcore_self_s_cmp( self, embedded_self ) != 0 )
            {
                bcore_source_a_parse_err_fa( source, "Precoder reflection embedding failed. Embedded code:\n#<sc_t>", self_embedded_string->sc );
            }

            BCORE_LIFE_CREATE( bcore_precoder_object_s, precoder_object );

            bcore_self_s_copy( &precoder_object->self, self );
            st_s_copy( &precoder_object->self_source, self_embedded_string );

            bcore_precoder_item_s_setup( item, precoder_object );

            BCORE_LIFE_DOWN();
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'feature' " ) )
        {
            BCORE_LIFE_INIT();
            BCORE_LIFE_CREATE( bcore_precoder_feature_s, precoder_feature );
            precoder_feature->group_name = o->name.sc;
            bcore_precoder_feature_s_compile( precoder_feature, source );

            bcore_precoder_item_s_setup( item, precoder_feature );

            o->has_features = true;
            BCORE_LIFE_DOWN();
        }
        else
        {
            bcore_source_a_parse_err_fa( source, "Precode syntax error." );
        }

        o->hash = bcore_tp_fold_tp( o->hash, item->hash );
        if( !bcore_precoder_s_register_item( o->source->target->precoder, item ) )
        {
            bcore_source_a_parse_err_fa( source, "Identifier #<sc_t> is already in use.", item->name.sc );
        }
        bcore_array_a_push( ( bcore_array* )o, sr_asd( item ) );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_expand_declaration( const bcore_precoder_group_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }// group: #<sc_t>\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, o->name.sc, typeof( o->name.sc ) );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_expand_declaration( o->data[ i ], indent + 2, sink );
    bcore_sink_a_push_fa( sink, "#rn{ }##define BETH_EXPAND_GROUP_#<sc_t>", indent, o->name.sc );
    if( o->has_features )
    {
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o->name.sc );
    }
    for( sz_t i = 0; i < o->size; i++ )
    {
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  BETH_EXPAND_ITEM_#<sc_t>", indent, o->data[ i ]->name.sc );
    }
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "\n" );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_expand_spect( const bcore_precoder_group_s* o, sz_t indent, bcore_sink* sink )
{
    BCORE_LIFE_INIT();
    bcore_sink_a_push_fa( sink, "\n" );
    st_s* spect_name = BCORE_LIFE_A_PUSH( st_s_create_fa( "#<sc_t>_s", o->name.sc ) );
    bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, spect_name->sc, typeof( spect_name->sc ) );

    // declaration (need not be shared)
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_DECLARE_SPECT( #<sc_t> )\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }{\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    bcore_spect_header_s header;\n", indent );
    for( sz_t i = 0; i < o->size; i++ )
    {
        const bcore_precoder_item_s* item = o->data[ i ];
        if( item->type == TYPEOF_bcore_precoder_feature_s )
        {
            const bcore_precoder_feature_s* feature = item->data.o;
            bcore_sink_a_push_fa( sink, "#rn{ }    #<sc_t> #<sc_t>;\n", indent, feature->item_name, feature->name.sc );
        }
    }
    bcore_sink_a_push_fa( sink, "#rn{ }};\n", indent );

    // definition (need not be shared)
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_DEFINE_SPECT( bcore_inst, #<sc_t> )\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }\"{\"\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    \"bcore_spect_header_s header;\"\n", indent );
    for( sz_t i = 0; i < o->size; i++ )
    {
        const bcore_precoder_item_s* item = o->data[ i ];
        if( item->type == TYPEOF_bcore_precoder_feature_s )
        {
            const bcore_precoder_feature_s* feature = item->data.o;
            bcore_sink_a_push_fa( sink, "#rn{ }    \"feature #<sc_t> : #<sc_t> = #<sc_t>_default;\"\n", indent, o->name.sc, feature->name.sc, item->name.sc );
        }
    }
    bcore_sink_a_push_fa( sink, "#rn{ }\"}\";\n", indent );

    // default functions
    for( sz_t i = 0; i < o->size; i++ )
    {
        const bcore_precoder_item_s* item = o->data[ i ];
        if( item->type == TYPEOF_bcore_precoder_feature_s )
        {
            bcore_precoder_feature_s_expand_default( item->data.o, indent, sink );
//            bcore_precoder_feature_s_expand_virtual_p( item->data.o, indent, sink );
            bcore_precoder_feature_s_expand_virtual_t( item->data.o, indent, sink );
            bcore_precoder_feature_s_expand_virtual_a( item->data.o, indent, sink );
            bcore_precoder_feature_s_expand_virtual_r( item->data.o, indent, sink );
        }
    }

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_expand_definition( const bcore_precoder_group_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }//#rn{-}\n", indent, sz_max( 0, 118 - indent ) );
    bcore_sink_a_push_fa( sink, "#rn{ }/// group: #<sc_t>\n", indent, o->name.sc );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_expand_definition( o->data[ i ], indent, sink );
    if( o->has_features )
    {
        bcore_precoder_group_s_expand_spect( o, indent, sink );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_expand_init1( const bcore_precoder_group_s* o, sz_t indent, bcore_sink* sink )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_expand_init1( o->data[ i ], indent, sink );
    if( o->has_features )
    {
        // default functions
        for( sz_t i = 0; i < o->size; i++ )
        {
            const bcore_precoder_item_s* item = o->data[ i ];
            if( item->type == TYPEOF_bcore_precoder_feature_s )
            {
                const bcore_precoder_feature_s* feature = item->data.o;
                bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_FFUNC( #<sc_t>, #<sc_t>_default );\n", indent, feature->item_name, feature->item_name );
            }
        }
        bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_SPECT( #<sc_t> );\n", indent, o->name.sc );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

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
            bcore_precoder_s_register_group( o->target->precoder, group );
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
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    bcore_sink_a_push_fa( sink, "#rn{ }/// source: #<sc_t>\n", indent, o->name.sc );
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

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_target_s_expand_license( const bcore_precoder_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "/** This file was generated by the beth-precode-compiler. Not suitable for manual editing.\n" );
    bcore_sink_a_push_fa( sink, " *\n" );
    bcore_sink_a_push_fa( sink, " *  Copyright and License:\n" );
    bcore_sink_a_push_fa( sink, " *    If a specification exists for the repository or folder in which the file is located,\n" );
    bcore_sink_a_push_fa( sink, " *    that specification shall apply. Otherwise, the Copyright and License of the associated\n" );
    bcore_sink_a_push_fa( sink, " *    source code files containing beth-precode shall apply.\n" );
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
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_spect.h\"\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_spect_inst.h\"\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_sr.h\"\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_const_manager.h\"\n", indent );

    /// definition section
    bcore_sink_a_push_fa( sink, "\n" );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_source_s_expand_definition( o->data[ i ], indent, sink );

    /// signal section
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
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


//        bcore_precoder_target_s_expand_h( o, 0, BCORE_STDOUT );
//        bcore_precoder_target_s_expand_c( o, 0, BCORE_STDOUT );
    }

    BCORE_LIFE_RETURN( modified );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// returns false if already registered
static bl_t bcore_precoder_s_register_group( bcore_precoder_s* o, const bcore_precoder_group_s* group )
{
    if( bcore_hmap_tpvd_s_exists( &o->hmap_group, group->id ) ) return false;
    bcore_hmap_tpvd_s_set( &o->hmap_group, group->id, ( vd_t )group );
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

/// returns false if already registered
static bl_t bcore_precoder_s_register_item( bcore_precoder_s* o, const bcore_precoder_item_s* item )
{
    if( bcore_hmap_tpvd_s_exists( &o->hmap_item, item->id ) ) return false;
    bcore_hmap_tpvd_s_set( &o->hmap_item, item->id, ( vd_t )item );
    return true;
}

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
        bcore_msg_fa( "PRECODER: Expanded in #<f3_t> sec.\n", time );
        bcore_msg_fa( "PRECODER: Files were updated. Rebuild is necessary.\n" );
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
    bcore_msg_fa( "PRECODER: Compiled in #<f3_t> sec.\n", time );
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
            BCORE_REGISTER_OBJECT( bcore_precoder_arg_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_args_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_feature_s );
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
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_arg_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_args_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_feature_s );
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

