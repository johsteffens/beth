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

// use dry run when debugging decoder (does not change precoded files; output to stadout)
//#define BCORE_PRECODER_DRY_RUN

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
                i++;
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


/** Creates a structured multiline string for direct code embedding
 *  from an embedded string
 */
static st_s* create_structured_multiline_string( const sc_t s, sz_t indent )
{
    st_s* out = st_s_create();
    sz_t ind = indent;
    bl_t newline = true;
    for( sz_t i = 0; s[ i ] != 0; i++ )
    {
        char c = s[ i ];
        if( c == ';' )
        {
            if( newline ) st_s_push_fa( out, "#rn{ }\"", ind );
            st_s_push_char( out, c );
            st_s_push_fa( out, "\"\n" );
            newline = true;
        }
        else if( c == '{' )
        {
            if( newline )
            {
                st_s_push_fa( out, "#rn{ }\"{\"", ind );
            }
            else
            {
                st_s_push_fa( out, "\"\n#rn{ }\"{\"", ind );
            }
            if( s[ i + 1 ] != 0 ) st_s_push_char( out, '\n' );
            ind += 4;
            newline = true;

        }
        else if( c == '}' )
        {
            ind -= 4;
            if( newline )
            {
                st_s_push_fa( out, "#rn{ }\"}\"", ind );
            }
            else
            {
                st_s_push_fa( out, "\"\n#rn{ }\"}\"", ind );
            }
            if( s[ i + 1 ] != 0 ) st_s_push_char( out, '\n' );
            newline = true;
        }
        else if( c == ' ' )
        {
            if( !newline )
            {
                if( s[ i + 1 ] != ';' && s[ i + 1 ] != '{' && s[ i + 1 ] != '}' && s[ i + 1 ] != 0 )
                {
                    st_s_push_char( out, c );
                }
            }
        }
        else
        {
            if( newline )
            {
                st_s_push_fa( out, "#rn{ }\"", ind );
                newline = false;
            }
            st_s_push_char( out, c );
        }
    }

    if( !newline ) st_s_push_char( out, '"' );
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
// signature void myfeature( mutable, sz_t a, sz_t b );
BCORE_DECLARE_OBJECT( bcore_precoder_signature_s )
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

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_signature_s )
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

// function body (name is always local to group_name)
BCORE_DECLARE_OBJECT( bcore_precoder_body_s )
{
    aware_t _;
    st_s name;
    sc_t group_name;
    st_s code;
    bl_t go_inline; // prefer inline expansion
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_body_s )
"{"
    "aware_t _;"
    "st_s name;"
    "sc_t group_name;"
    "st_s code;"
    "bl_t go_inline;"
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
    st_s default_name;
    bcore_precoder_body_s* default_body;

    bl_t strict;

    bl_t flag_p;
    bl_t flag_t;
    bl_t flag_a;
    bl_t flag_r;

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
    "st_s default_name;"
    "bcore_precoder_body_s => default_body;"

    "bl_t strict;"

    "bl_t flag_p;"
    "bl_t flag_t;"
    "bl_t flag_a;"
    "bl_t flag_r;"

    "bl_t has_ret;"
    "st_s ret_type;"        // void
    "bcore_precoder_args_s args;"       // sz_t a, sz_t b
    "bl_t mutable;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_precoder_name_s )
{
    aware_t _;
    st_s name; // deemed global
    sc_t group_name;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_name_s )
"{"
    "aware_t _;"
    "st_s name;" // deemed global
    "sc_t group_name;"
"}";

//----------------------------------------------------------------------------------------------------------------------

// used in bcore_precoder_stamp_s
BCORE_DECLARE_OBJECT( bcore_precoder_func_s )
{
    st_s name;
    tp_t type;
    bcore_precoder_body_s* body;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_func_s )
"{"
    "st_s name;"
    "tp_t type;"
    "bcore_precoder_body_s => body;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_precoder_funcs_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( bcore_precoder_func_s, );
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_funcs_s )
"{"
    "aware_t _;"
    "bcore_precoder_func_s [] arr;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_precoder_stamp_s )
{
    aware_t _;
    sc_t item_name;
    st_s         * self_source;
    bcore_self_s * self;
    bcore_precoder_funcs_s funcs;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_stamp_s )
"{"
    "aware_t _;"
    "sc_t item_name;"
    "st_s         => self_source;"
    "bcore_self_s => self;"
    "bcore_precoder_funcs_s funcs;"
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
    bl_t is_aware;
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
    "bl_t is_aware;"
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

static bcore_precoder_s* precoder_g = NULL;
static bl_t bcore_precoder_s_item_register( bcore_precoder_s* o, const bcore_precoder_item_s* item );
static const bcore_precoder_item_s* bcore_precoder_s_item_get( const bcore_precoder_s* o, tp_t item_id );
static bl_t                         bcore_precoder_s_item_exists( const bcore_precoder_s* o, tp_t item_id );

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_args_s_clear( bcore_precoder_args_s* o )
{
    bcore_array_a_set_space( ( bcore_array* ) o, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_arg_s_compile( bcore_precoder_arg_s* o, const bcore_precoder_group_s* group, bcore_source* source )
{
    st_s* s = st_s_create();
    if( bcore_source_a_parse_bl_fa( source, "#?'const' " ) ) st_s_push_sc( &o->type, "const " );

    if( bcore_source_a_parse_bl_fa( source, "#?':' " ) )
    {
        st_s_push_st( &o->type, &group->name );
        bcore_source_a_parse_fa( source, "#name ", s );
        st_s_push_fa( &o->type, "#<sc_t>#<sc_t>", s->sc[ 0 ] ? "_" : "", s->sc );
    }
    else
    {
        bcore_source_a_parse_fa( source, "#name ", s );
        if( s->size == 0 ) bcore_source_a_parse_err_fa( source, "Argument: Type expected." );
        st_s_push_st( &o->type, s );
    }


    while( bcore_source_a_parse_bl_fa( source, "#?'*' " ) ) st_s_push_sc( &o->type, "*" );

    bcore_source_a_parse_fa( source, "#name ", s );
    if( s->size == 0 ) bcore_source_a_parse_err_fa( source, "Argument: Name expected." );
    st_s_push_st( &o->name, s );

    st_s_discard( s );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_args_s_compile( bcore_precoder_args_s* o, const bcore_precoder_group_s* group, bcore_source* source )
{
    bcore_precoder_args_s_clear( o );
    while( bcore_source_a_parse_bl_fa( source, " #?',' " ) ) // args follow
    {
        bcore_precoder_arg_s* arg = bcore_precoder_arg_s_create();
        bcore_precoder_arg_s_compile( arg, group, source );
        bcore_array_a_push( ( bcore_array* ) o, sr_asd( arg ) );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_args_s_append( bcore_precoder_args_s* o, const bcore_precoder_group_s* group, bcore_source* source )
{
    while( !bcore_source_a_parse_bl_fa( source, " #=?')' " ) ) // args follow
    {
        bcore_precoder_arg_s* arg = bcore_precoder_arg_s_create();
        bcore_precoder_arg_s_compile( arg, group, source );
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

static void bcore_precoder_signature_s_compile( bcore_precoder_signature_s* o, const bcore_precoder_group_s* group, bcore_source* source )
{
    BCORE_LIFE_INIT();

    BCORE_LIFE_CREATE( st_s, name_buf );
    BCORE_LIFE_CREATE( st_s, name_candidate );
    st_s_clear( &o->ret_type );

    bcore_source_a_parse_fa( source, " #name", name_buf );

    bl_t predefined = false;

    if( !bcore_source_a_parse_bl_fa( source, " #=?'*'" ) )
    {
        if( bcore_source_a_parse_bl_fa( source, " #?':'" ) )
        {
            predefined = true;
            bcore_source_a_parse_fa( source, " #name", name_candidate );
            if( name_buf->size == 0 )
            {
                st_s_push_fa( name_buf, "#<sc_t>_#<sc_t>", o->group_name, name_candidate->sc );
            }
            else
            {
                st_s_push_fa( name_buf, "_#<sc_t>", name_candidate->sc );
            }
        }

        tp_t tp_name = typeof( name_buf->sc );
        // if name_buf refers to another signature
        if( bcore_precoder_s_item_exists( precoder_g, tp_name ) )
        {
            const bcore_precoder_item_s* item = bcore_precoder_s_item_get( precoder_g, tp_name );
            if( sr_s_type( &item->data ) == TYPEOF_bcore_precoder_signature_s )
            {
                const bcore_precoder_signature_s* signature = item->data.o;
                o->has_ret = signature->has_ret;
                st_s_copy( &o->ret_type, &signature->ret_type );
                bcore_precoder_args_s_copy( &o->args, &signature->args );
                o->mutable = signature->mutable;
                predefined = true;
            }
        }
        else if( predefined )
        {
            bcore_source_a_parse_err_fa( source, "Syntax error." );
        }
    }

    if( !predefined )
    {
        // get return type
        if( name_buf->size == 0 ) bcore_source_a_parse_err_fa( source, "Return type missing." );
        st_s_push_fa( &o->ret_type, "#<sc_t>", name_buf->sc );
        if( st_s_equal_sc( name_buf, "const" ) )
        {
            bcore_source_a_parse_fa( source, " #name", name_buf );
            if( name_buf->size == 0 ) bcore_source_a_parse_err_fa( source, "Return type missing." );
            st_s_push_fa( &o->ret_type, " #<sc_t>", name_buf->sc );
        }
        while( bcore_source_a_parse_bl_fa( source, " #?'*'" ) ) st_s_push_char( &o->ret_type, '*' );
    }

    // get name
    bcore_source_a_parse_fa( source, " #name", &o->name );
    if( o->name.size == 0 )
    {
        if( name_candidate->size > 0 )
        {
            st_s_copy( &o->name, name_candidate );
        }
        else
        {
            bcore_source_a_parse_err_fa( source, "Name missing." );
        }
    }

    o->has_ret = !st_s_equal_sc( &o->ret_type, "void" );

    // get or append args
    if( !predefined )
    {
        bcore_source_a_parse_fa( source, " ( " );
        if(      bcore_source_a_parse_bl_fa(  source, " #?'mutable' " ) ) o->mutable = true;
        else if( bcore_source_a_parse_bl_fa(  source, " #?'const' "   ) ) o->mutable = false;
        else     bcore_source_a_parse_err_fa( source, "'mutable' or 'const' expected." );
        bcore_precoder_args_s_compile( &o->args, group, source );
        bcore_source_a_parse_fa( source, " ) " );
    }
    else if( bcore_source_a_parse_bl_fa( source, " #?'('" ) )
    {
        bcore_precoder_args_s_append( &o->args, group, source );
        bcore_source_a_parse_fa( source, " ) " );
    }

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_body_s_compile_code( bcore_precoder_body_s* o, const bcore_precoder_group_s* group, bcore_source* source )
{
    BCORE_LIFE_INIT();

    st_s_clear( &o->code );

    bcore_source_a_parse_fa( source, " {" );
    st_s_push_sc( &o->code, "{" );
    sz_t nest_count = 1;
    bl_t exit_loop = false;
    bl_t new_line = false;
    o->go_inline = true;
    sz_t space_count = 0;
    sz_t indentation = 0;
    while( !bcore_source_a_eos( source ) && !exit_loop )
    {
        u0_t c = bcore_source_a_get_u0( source );
        switch( c )
        {
            case '{':
            {
                nest_count++;
            }
            break;

            case '}':
            {
                nest_count--;
                if( !nest_count )
                {
                    indentation = space_count;
                    exit_loop = true;
                }
            }
            break;

            case '\n': new_line = true; o->go_inline = false; break;
            case ' ' : if( new_line ) space_count++; break;

            case ':': // if a name follows immediately, ':' is interpreted as namespace-prepend (ordinary c-code using : should append a whitespace)
            {
                if( bcore_source_a_parse_bl_fa( source, "#?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z'))" ) )
                {
                    st_s_push_sc( &o->code, o->group_name );
                    st_s_push_char( &o->code, '_' );
                    c = 0;
                }
            }
            break;

            default: break;
        }
        if( c != '\n' && c != ' ' && c != '\t' )
        {
            new_line = false;
            space_count = 0;
        }
        if( c ) st_s_push_char( &o->code, c );
    }

    if( indentation > 0 )
    {
        st_s* match_format = st_s_create_fa( "\n#rn{ }", indentation );
        st_s_replace_sc_sc( &o->code, match_format->sc, "\n" );
        st_s_discard( match_format );
    }

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_body_s_compile( bcore_precoder_body_s* o, const bcore_precoder_group_s* group, bcore_source* source )
{
    BCORE_LIFE_INIT();

    BCORE_LIFE_CREATE( st_s, string );

    o->group_name = group->name.sc;

    if( !bcore_source_a_parse_bl_fa( source, " #=?'='" ) )
    {
        bcore_source_a_parse_fa( source, " #name", string );
        if( string->size == 0 ) bcore_source_a_parse_err_fa( source, "Body name expected." );
        st_s_push_fa( &o->name, "#<sc_t>", string->sc );
    }

    bcore_source_a_parse_fa( source, " =" );

    if( bcore_source_a_parse_bl_fa( source, " #=?'{'" ) )
    {
        bcore_precoder_body_s_compile_code( o, group, source );
    }
    else
    {
        BCORE_LIFE_CREATE( st_s, name );

        if( bcore_source_a_parse_bl_fa( source, " #?':'" ) )
        {
            st_s_push_fa( name, "#<sc_t>_", group->name.sc );
        }
        bcore_source_a_parse_fa( source, " #name", string );
        if( string->size == 0 ) bcore_source_a_parse_err_fa( source, "Body name expected." );
        st_s_push_fa( name, "#<sc_t>", string->sc );

        tp_t tp_name = typeof( name->sc );
        // if name_buf refers to another body
        if( bcore_precoder_s_item_exists( precoder_g, tp_name ) )
        {
            const bcore_precoder_item_s* item = bcore_precoder_s_item_get( precoder_g, tp_name );
            if( sr_s_type( &item->data ) == TYPEOF_bcore_precoder_body_s )
            {
                const bcore_precoder_body_s* body = item->data.o;
                st_s_copy( &o->code, &body->code );
            }
        }
        else
        {
            bcore_source_a_parse_err_fa( source, "Cannot resolve body name '#<sc_t>'\n", name->sc );
        }
    }

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_feature_s_compile( bcore_precoder_feature_s* o, const bcore_precoder_group_s* group, bcore_source* source )
{
    BCORE_LIFE_INIT();

    o->strict = bcore_source_a_parse_bl_fa(  source, " #?w'strict' " );

    if( bcore_source_a_parse_bl_fa( source, " #?|'|" ) )
    {
        st_s* flags = st_s_create();
        bcore_source_a_parse_fa(  source, " #until'''", flags );
        o->flag_p = ( st_s_find_char( flags, 0, -1, 'p' ) < flags->size );
        o->flag_t = ( st_s_find_char( flags, 0, -1, 't' ) < flags->size );
        o->flag_a = ( st_s_find_char( flags, 0, -1, 'a' ) < flags->size );
        o->flag_r = ( st_s_find_char( flags, 0, -1, 'r' ) < flags->size );
        st_s_discard( flags );
        bcore_source_a_parse_fa(  source, "' " );
    }
    else
    {
        bcore_source_a_parse_err_fa( source, "Feature: '<flags>' expected. Example: 'ptar'" );
    }

    BCORE_LIFE_CREATE( bcore_precoder_signature_s, signature );
    signature->group_name = o->group_name;
    bcore_precoder_signature_s_compile( signature, group, source );
    st_s_copy( &o->name, &signature->name );
    o->has_ret = signature->has_ret;
    st_s_copy( &o->ret_type, &signature->ret_type );
    bcore_precoder_args_s_copy( &o->args, &signature->args );
    o->mutable = signature->mutable;

    if( bcore_source_a_parse_bl_fa( source, " #?'=' " ) )
    {
        if( bcore_source_a_parse_bl_fa( source, " #=?'{' " ) )
        {
            if( o->strict )  bcore_source_a_parse_err_fa( source, "Feature is 'strict'. Default function would have no effect." );
            o->default_body = bcore_precoder_body_s_create();
            bcore_precoder_body_s_compile_code( o->default_body, group, source );
            st_s_copy_fa( &o->default_name, "#<sc_t>__", o->name.sc );
        }
        else
        {
            if( o->strict )  bcore_source_a_parse_err_fa( source, "Feature is 'strict'. Default function would have no effect." );
            bcore_source_a_parse_fa( source, " #name ", &o->default_name );
            if( o->default_name.size == 0 ) bcore_source_a_parse_err_fa( source, "Feature: Default function name expected." );
            if( st_s_equal_st( &o->default_name, &o->name ) )
            {
                bcore_source_a_parse_err_fa( source, "Feature: Default function name must differ from feature name." );
            }
        }
    }

    bcore_source_a_parse_fa( source, " ; " );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_stamp_s_compile( bcore_precoder_stamp_s* o, bcore_precoder_group_s* group, bcore_source* source )
{
    BCORE_LIFE_INIT();

    BCORE_LIFE_CREATE( st_s, self_string );
    BCORE_LIFE_CREATE( st_s, stamp_name );
    BCORE_LIFE_CREATE( st_s, trait_name );
    BCORE_LIFE_CREATE( st_s, type_name );
    BCORE_LIFE_CREATE( st_s, string );

    if( bcore_source_a_parse_bl_fa( source, " #?':'" ) )
    {
        st_s_push_fa( stamp_name, "#<sc_t>", group->name.sc );
    }

    bcore_source_a_parse_fa( source, " #name", string );

    if( string->size > 0 )
    {
        if( stamp_name->size > 0 ) st_s_push_fa( stamp_name, "_" );
        st_s_push_fa( stamp_name, "#<sc_t>", string->sc );
    }

    st_s_push_fa( self_string, "#<sc_t>", stamp_name->sc );
    if( stamp_name->size >= 2 && sc_t_equ( stamp_name->sc + stamp_name->size - 2, "_s" ) )
    {
        WRN_fa( "Stamp '#<sc_t>' ends in '_s'. In beth, this ending identifies a structure. Using it as stamp name can have side effects. Is this intended?", stamp_name->sc );
    }
    else
    {
        st_s_push_fa( self_string, "_s" );
    }

    bcore_source_a_parse_fa( source, " = " );
    st_s_push_fa( self_string, "=" );


//    bcore_source_a_parse_fa( source, " #until'='=", string );
//    st_s_push_fa( self_string, "#<sc_t>=", string->sc );

    if( bcore_source_a_parse_bl_fa( source, " #?w'aware'" ) ) st_s_push_sc( self_string, "aware " );

    if( bcore_source_a_parse_bl_fa( source, " #?':'" ) )
    {
        st_s_push_fa( trait_name, "#<sc_t>", group->name.sc );
    }
    else
    {
        bcore_source_a_parse_fa( source, " #name", string );
        if( string->size == 0 ) bcore_source_a_parse_err_fa( source, "Trait name expected." );
        st_s_push_fa( trait_name, "#<sc_t>", string->sc );
    }

    st_s_push_st( self_string, trait_name );

    bcore_source_a_parse_fa( source, " {" );
    st_s_push_fa( self_string, "{" );

    while( !bcore_source_a_eos( source ) && !bcore_source_a_parse_bl_fa( source, " #?'}'" ) )
    {
        if( bcore_source_a_parse_bl_fa( source, " #?w'func'" ) )
        {
            bcore_precoder_funcs_s* funcs = &o->funcs;
            bcore_array_a_push( ( bcore_array* )funcs, sr_null() );
            bcore_precoder_func_s* func = &funcs->data[ funcs->size - 1 ];

            st_s_push_sc( self_string, "func " );
            bcore_source_a_parse_fa( source, " #name", type_name );
            st_s_push_fa( self_string, "#<sc_t>", type_name->sc );

            bl_t bind = bcore_source_a_parse_bl_fa( source, " #?':'" );
            if( bind ) st_s_push_sc( self_string, ":" );

            bcore_source_a_parse_fa( source, " #name", &func->name );
            if( func->name.size == 0 ) bcore_source_a_parse_err_fa( source, "Function name expected." );
            st_s_push_sc( self_string, func->name.sc );

            if( bind )
            {
                if( type_name->size == 0 )
                {
                    st_s_push_fa( type_name, "#<sc_t>", trait_name->sc );
                }
                st_s_push_fa( type_name, "_#<sc_t>", func->name.sc );
            }
            func->type = typeof( type_name->sc );

            if( bcore_source_a_parse_bl_fa( source, " #=?'='" ) )
            {
                func->body = bcore_precoder_body_s_create();
                bcore_precoder_body_s_compile( func->body, group, source );
            }

            bcore_source_a_parse_fa( source, " ; " );
            st_s_push_sc( self_string, ";" );
        }
        else
        {
            bl_t exit = false;
            while( !exit && !bcore_source_a_eos( source ) )
            {
                u0_t c = bcore_source_a_get_u0( source );
                switch( c )
                {
                    case ':':
                    {
                        st_s* name = st_s_create();
                        bcore_source_a_parse_fa( source, " #name", name );
                        st_s_push_fa( self_string, "#<sc_t>_#<sc_t>", group->name.sc, name->sc );
                        st_s_discard( name );
                    }
                    break;

                    case ';':
                    {
                        st_s_push_char( self_string, c );
                        exit = true;
                    }
                    break;

                    default:
                    {
                        st_s_push_char( self_string, c );
                    }
                    break;
                }
            }
        }
    }
    st_s_push_sc( self_string, "}" );

    st_s self_string_source;
    st_s_init_weak_sc( &self_string_source, self_string->sc );

    o->self = bcore_self_s_parse_source( ( bcore_source* )&self_string_source, 0, group->name.sc, false );

    bcore_source_a_parse_fa( source, " ; " );

    o->self_source = create_embedded_string( self_string );

    /// 4095 is the C99-limit for string literals
    if( o->self_source->size > 4095 )
    {
        bcore_source_a_parse_err_fa
        (
            source,
            "Precoder reflection embedding failed.\n"
            "The embedded code would require a string literal larger than 4095 characters.\n"
            "This exceeds the limit defined in C99.\n"
        );
    }

    // test embedded string
    {
        st_s* self_source_copy = st_s_clone( o->self_source );

        // remove string escape
        st_s_replace_sc_sc( self_source_copy, "\\\"", "\"" );

        st_s st_weak = st_weak_st( self_source_copy );
        bcore_self_s* embedded_self = BCORE_LIFE_A_PUSH( bcore_self_s_parse_source( ( bcore_source* )&st_weak, 0, group->name.sc, false ) );
        if( bcore_self_s_cmp( o->self, embedded_self ) != 0 )
        {
            bcore_source_a_parse_err_fa( source, "Precoder reflection embedding failed. Embedded code:\n#<sc_t>", o->self_source->sc );
        }

        st_s_discard( self_source_copy );
    }

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_precoder_signature_s_get_hash( const bcore_precoder_signature_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    hash = bcore_tp_fold_sc( hash, o->group_name );
    hash = bcore_tp_fold_sc( hash, o->item_name );

    hash = bcore_tp_fold_u0( hash, o->has_ret ? 1 : 0 );
    hash = bcore_tp_fold_sc( hash, o->ret_type.sc );
    hash = bcore_tp_fold_tp( hash, bcore_precoder_args_s_get_hash( &o->args ) );
    hash = bcore_tp_fold_u0( hash, o->mutable ? 1 : 0 );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_precoder_body_s_get_hash( const bcore_precoder_body_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    hash = bcore_tp_fold_sc( hash, o->code.sc );
    hash = bcore_tp_fold_u0( hash, o->go_inline ? 1 : 0 );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_precoder_func_s_get_hash( const bcore_precoder_func_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    hash = bcore_tp_fold_tp( hash, o->type );
    if( o->body ) hash = bcore_tp_fold_tp( hash, bcore_precoder_body_s_get_hash( o->body ) );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_precoder_feature_s_get_hash( const bcore_precoder_feature_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    hash = bcore_tp_fold_sc( hash, o->group_name );
    hash = bcore_tp_fold_sc( hash, o->item_name );
    hash = bcore_tp_fold_sc( hash, o->default_name.sc );
    if( o->default_body )
    {
        hash = bcore_tp_fold_tp( hash, bcore_precoder_body_s_get_hash( o->default_body ) );
    }

    hash = bcore_tp_fold_u0( hash, o->strict ? 1 : 0 );
    hash = bcore_tp_fold_u0( hash, o->flag_p ? 1 : 0 );
    hash = bcore_tp_fold_u0( hash, o->flag_t ? 1 : 0 );
    hash = bcore_tp_fold_u0( hash, o->flag_a ? 1 : 0 );
    hash = bcore_tp_fold_u0( hash, o->flag_r ? 1 : 0 );

    hash = bcore_tp_fold_u0( hash, o->has_ret ? 1 : 0 );
    hash = bcore_tp_fold_sc( hash, o->ret_type.sc );
    hash = bcore_tp_fold_tp( hash, bcore_precoder_args_s_get_hash( &o->args ) );
    hash = bcore_tp_fold_u0( hash, o->mutable ? 1 : 0 );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_precoder_stamp_s_get_hash( const bcore_precoder_stamp_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->self_source->sc );
    for( sz_t i = 0; i < o->funcs.size; i++ )
    {
        hash = bcore_tp_fold_tp( hash, bcore_precoder_func_s_get_hash( &o->funcs.data[ i ] ) );
    }
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_feature_s_expand_indef_typedef( const bcore_precoder_feature_s* o, sz_t indent, bcore_sink* sink )
{
    //typedef ret_t (*feature_func)( feature* o, arg_t arg1 );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  typedef #<sc_t> (*#<sc_t>_#<sc_t>)(", indent, o->ret_type.sc, o->group_name, o->name.sc );
    if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
    bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group_name );
    bcore_precoder_args_s_expand( &o->args, sink );
    bcore_sink_a_push_fa( sink, " );" );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_feature_s_expand_indef_declaration( const bcore_precoder_feature_s* o, sz_t indent, bcore_sink* sink )
{
    if( o->flag_p )
    {
        // static inline ret_t feature_p_func( const spect* __p, feature* o, arg_t arg1 ) { return __p->func( o, arg1 ); }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t> #<sc_t>_p_#<sc_t>( const #<sc_t>_s* __p,", indent, o->ret_type.sc, o->group_name, o->name.sc, o->group_name );
        if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
        bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group_name );
        bcore_precoder_args_s_expand( &o->args, sink );
        bcore_sink_a_push_fa( sink, " ) { " );
        if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
        bcore_sink_a_push_fa( sink, "__p->#<sc_t>( o", o->name.sc );
        bcore_precoder_args_s_expand_name( &o->args, sink );
        bcore_sink_a_push_fa( sink, " ); }" );
    }

    if( o->flag_t )
    {
        // static inline ret_t feature_t_func( tp_t __t, feature* o, arg_t arg1 ) { return features_s_get_typed( __t )->func( o, arg1 ); }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t> #<sc_t>_t_#<sc_t>( tp_t __t,", indent, o->ret_type.sc, o->group_name, o->name.sc, o->group_name );
        if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
        bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group_name );
        bcore_precoder_args_s_expand( &o->args, sink );
        bcore_sink_a_push_fa( sink, " ) { " );
        if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
        bcore_sink_a_push_fa( sink, "#<sc_t>_s_get_typed( __t )->#<sc_t>( o", o->group_name, o->name.sc );
        bcore_precoder_args_s_expand_name( &o->args, sink );
        bcore_sink_a_push_fa( sink, " ); }" );
    }

    if( o->flag_a )
    {
        // static inline ret_t feature_a_func( feature* o, arg_t arg1 ) { return features_s_get_aware( o )->func( o, arg1 ); }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t> #<sc_t>_a_#<sc_t>(", indent, o->ret_type.sc, o->group_name, o->name.sc );
        if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
        bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group_name );
        bcore_precoder_args_s_expand( &o->args, sink );
        bcore_sink_a_push_fa( sink, " ) { " );
        if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
        bcore_sink_a_push_fa( sink, "#<sc_t>_s_get_aware( o )->#<sc_t>( o", o->group_name, o->name.sc );
        bcore_precoder_args_s_expand_name( &o->args, sink );
        bcore_sink_a_push_fa( sink, " ); }" );
    }

    if( o->flag_r )
    {
        // static inline ret_t feature_r_func( const sr_s* o, arg_t arg1 ) { return features_s_get_typed( o->o_type )->func( o->o, arg1 ); }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t> #<sc_t>_r_#<sc_t>(", indent, o->ret_type.sc, o->group_name, o->name.sc );
        bcore_sink_a_push_fa( sink, " const sr_s* o" );
        bcore_precoder_args_s_expand( &o->args, sink );
        bcore_sink_a_push_fa( sink, " ) { " );
        if( o->mutable ) bcore_sink_a_push_fa( sink, "ASSERT( !sr_s_is_const( o ) ); ", indent );
        if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
        bcore_sink_a_push_fa( sink, "( (#<sc_t>_s*)ch_spect_p( o->p, TYPEOF_#<sc_t>_s ) )->#<sc_t>( o->o", o->group_name, o->group_name, o->name.sc );
        bcore_precoder_args_s_expand_name( &o->args, sink );
        bcore_sink_a_push_fa( sink, " ); }" );
    }

    if( o->flag_p )
    {
        // static inline bl_t feature_p_defines_func( const spect* __p ) { return __p->func != NULL; }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline bl_t #<sc_t>_p_defines_#<sc_t>( const #<sc_t>_s* __p ) ", indent, o->group_name, o->name.sc, o->group_name );
        if( o->strict || o->default_body || o->default_name.size > 0 )
        {
            bcore_sink_a_push_fa( sink, "{ return true; }", o->name.sc );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "{ return __p->#<sc_t> != NULL; }", o->name.sc );
        }
    }

    if( o->flag_t )
    {
        // static inline bl_t feature_t_defines_func( tp_t __t ) { return features_s_get_typed( __t )->func != NULL; }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline bl_t #<sc_t>_t_defines_#<sc_t>( tp_t __t ) ", indent, o->group_name, o->name.sc );
        if( o->strict || o->default_body || o->default_name.size > 0 )
        {
            bcore_sink_a_push_fa( sink, "{ return true; }", o->name.sc );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "{ return #<sc_t>_s_get_typed( __t )->#<sc_t> != NULL; }", o->group_name, o->name.sc );
        }
    }

    if( o->flag_a )
    {
        // static inline bl_t feature_a_defines_func( const feature* o) { return features_s_get_aware( o )->func != NULL; }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline bl_t #<sc_t>_a_defines_#<sc_t>( const #<sc_t>* o ) ", indent, o->group_name, o->name.sc, o->group_name );
        if( o->strict || o->default_body || o->default_name.size > 0 )
        {
            bcore_sink_a_push_fa( sink, "{ return true; }", o->name.sc );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "{ return #<sc_t>_s_get_aware( o )->#<sc_t> != NULL; }", o->group_name, o->name.sc );
        }
    }

    if( o->flag_r )
    {
        // static inline bl_t feature_r_defines_func( const feature* o) { return features_s_get_typed( o->o_type )->func != NULL; }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline bl_t #<sc_t>_r_defines_#<sc_t>( const sr_s* o ) ", indent, o->group_name, o->name.sc );
        if( o->strict || o->default_body || o->default_name.size > 0 )
        {
            bcore_sink_a_push_fa( sink, "{ return true; }", o->name.sc );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "{ return ( (#<sc_t>_s*)ch_spect_p( o->p, TYPEOF_#<sc_t>_s ) )->#<sc_t> != NULL; }", o->group_name, o->group_name, o->name.sc );
        }
    }

    if( o->default_name.size > 0 )
    {
        if( o->default_body && o->default_body->go_inline )
        {
            // ret_t feature_default( feature* o, arg_t arg1 );
            bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t> #<sc_t>_#<sc_t>(", indent, o->ret_type.sc, o->group_name, o->default_name.sc );
            if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
            bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group_name );
            bcore_precoder_args_s_expand( &o->args, sink );
            bcore_sink_a_push_fa( sink, " ) #<sc_t>", o->default_body->code.sc );
        }
        else
        {
            // ret_t feature_default( feature* o, arg_t arg1 );
            bcore_sink_a_push_fa( sink, " \\\n#rn{ }  #<sc_t> #<sc_t>_#<sc_t>(", indent, o->ret_type.sc, o->group_name, o->default_name.sc );
            if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
            bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group_name );
            bcore_precoder_args_s_expand( &o->args, sink );
            bcore_sink_a_push_fa( sink, " );" );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_feature_s_expand_definition( const bcore_precoder_feature_s* o, sz_t indent, const bcore_precoder_group_s* group, bcore_sink* sink )
{
    if( o->default_body && !o->default_body->go_inline )
    {
        // ret_t feature_default( feature* o, arg_t arg1 );
        bcore_sink_a_push_fa( sink, "\n" );
        bcore_sink_a_push_fa( sink, "\n#<sc_t> #<sc_t>_#<sc_t>(", o->ret_type.sc, o->group_name, o->default_name.sc );
        if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
        bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group_name );
        bcore_precoder_args_s_expand( &o->args, sink );
        bcore_sink_a_push_fa( sink, " )\n", o->default_body->code.sc );
        bcore_sink_a_push_fa( sink, "#<sc_t>", o->default_body->code.sc );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_name_s_compile( bcore_precoder_name_s* o, const bcore_precoder_group_s* group, bcore_source* source )
{
    if( bcore_source_a_parse_bl_fa( source, " #?':'" ) )
    {
        st_s* name = st_s_create();
        bcore_source_a_parse_fa( source, " #name", name );
        st_s_push_fa( &o->name, "#<sc_t>#<sc_t>#<sc_t>", group->name.sc, name->sc[ 0 ] ? "_" : "", name->sc );
        st_s_discard( name );
    }
    else
    {
        bcore_source_a_parse_fa( source, " #name", &o->name );
    }
    if( o->name.size == 0      ) bcore_source_a_parse_err_fa( source, "Feature: Name missing." );
    bcore_source_a_parse_fa( source, " ; " );
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_precoder_name_s_get_hash( const bcore_precoder_name_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    hash = bcore_tp_fold_sc( hash, o->group_name );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_stamp_s_expand_declaration( const bcore_precoder_stamp_s* o, const bcore_precoder_s* precoder, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, o->item_name, typeof( o->item_name ) );

    bcore_sink_a_push_fa( sink, "#rn{ }##define BETH_EXPAND_ITEM_#<sc_t>", indent, o->item_name, o->item_name );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  BCORE_DECLARE_OBJECT( #<sc_t> )", indent, o->item_name );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }    ", indent );
    bcore_self_s_struct_body_to_sink_single_line( o->self, sink );
    bcore_sink_a_push_fa( sink, ";" );

    for( sz_t i = 0; i < o->funcs.size; i++ )
    {
        bcore_precoder_func_s* func = &o->funcs.data[ i ];
        if( bcore_precoder_s_item_exists( precoder, func->type ) )
        {
            const bcore_precoder_item_s* item = bcore_precoder_s_item_get( precoder, func->type );
            ASSERT( sr_s_type( &item->data ) == TYPEOF_bcore_precoder_feature_s );
            const bcore_precoder_feature_s* feature = item->data.o;
            bcore_sink_a_push_fa( sink, " \\\n#rn{ }  ", indent );

            bl_t go_inline = func->body && func->body->go_inline;

            if( go_inline ) bcore_sink_a_push_fa( sink, "static inline " );

            bcore_sink_a_push_fa( sink, "#<sc_t> #<sc_t>_#<sc_t>( ", feature->ret_type.sc, o->item_name, func->name.sc );
            bcore_sink_a_push_fa( sink, "#<sc_t>", feature->mutable ? "" : "const " );
            bcore_sink_a_push_fa( sink, "#<sc_t>* o", o->item_name );
            bcore_precoder_args_s_expand( &feature->args, sink );
            bcore_sink_a_push_fa( sink, " )" );

            if( go_inline )
            {
                bcore_sink_a_push_fa( sink, "#<sc_t>", func->body->code.sc );
            }
            else
            {
                bcore_sink_a_push_fa( sink, ";" );
            }
        }
    }

    // expand array
    if( o->self->trait == TYPEOF_bcore_array )
    {
        sz_t items = bcore_self_s_items_size( o->self );
        const bcore_self_item_s* array_item = NULL;
        for( sz_t i = 0; i < items; i++ )
        {
            const bcore_self_item_s* self_item = bcore_self_s_get_item( o->self, i );
            if( bcore_flect_caps_is_array( self_item->caps ) )
            {
                array_item = self_item;
                break;
            }
        }

        if( !array_item )
        {
            ERR_fa( "Expanding object #<sc_t>: Object is of trait array but contains no array.", o->item_name );
        }

        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_set_space( #<sc_t>* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_#<sc_t>, ( bcore_array* )o, size ); }", indent, o->item_name, o->item_name, o->item_name );
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_set_size( #<sc_t>* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_#<sc_t>, ( bcore_array* )o, size ); }", indent, o->item_name, o->item_name, o->item_name );
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_clear( #<sc_t>* o ) { bcore_array_t_set_space( TYPEOF_#<sc_t>, ( bcore_array* )o, 0 ); }", indent, o->item_name, o->item_name, o->item_name );

        if( array_item->type != 0 && nameof( array_item->type ) != NULL )
        {
            sc_t type_name = ifnameof( array_item->type );
            if( array_item->caps == BCORE_CAPS_ARRAY_DYN_LINK_AWARE )
            {
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_push_c( #<sc_t>* o, const #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_awc( v ) ); }", indent, o->item_name, o->item_name, type_name, o->item_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_push_d( #<sc_t>* o,       #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_asd( v ) ); }", indent, o->item_name, o->item_name, type_name, o->item_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push_t( #<sc_t>* o, tp_t t )", indent, type_name, o->item_name, o->item_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  {", indent );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      bcore_trait_assert_satisfied_type( TYPEOF_#<sc_t>, t );",                    indent, type_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_t_create( t ) );", indent, o->item_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      return bcore_array_t_get_last( TYPEOF_#<sc_t>, ( bcore_array* )o ).o;",      indent, o->item_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  }", indent );
            }
            else if( array_item->caps == BCORE_CAPS_ARRAY_DYN_SOLID_STATIC )
            {
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_push_c( #<sc_t>* o, const #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_twc( TYPEOF_#<sc_t>, v ) ); }", indent, o->item_name, o->item_name, type_name, o->item_name, type_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_push_d( #<sc_t>* o,       #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_tsd( TYPEOF_#<sc_t>, v ) ); }", indent, o->item_name, o->item_name, type_name, o->item_name, type_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push( #<sc_t>* o )", indent, type_name, o->item_name, o->item_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  {", indent );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_null() );",   indent, o->item_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      return bcore_array_t_get_last( TYPEOF_#<sc_t>, ( bcore_array* )o ).o;", indent, o->item_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  }", indent );
            }
            else if( array_item->caps == BCORE_CAPS_ARRAY_DYN_LINK_STATIC )
            {
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_push_c( #<sc_t>* o, const #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_twc( TYPEOF_#<sc_t>, v ) ); }", indent, o->item_name, o->item_name, type_name, o->item_name, type_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_push_d( #<sc_t>* o,       #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_tsd( TYPEOF_#<sc_t>, v ) ); }", indent, o->item_name, o->item_name, type_name, o->item_name, type_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push( #<sc_t>* o )", indent, type_name, o->item_name, o->item_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  {", indent );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_t_create( TYPEOF_#<sc_t> ) );", indent, o->item_name, type_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      return bcore_array_t_get_last( TYPEOF_#<sc_t>, ( bcore_array* )o ).o;",                   indent, o->item_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  }", indent );
            }
        }
        else
        {
            if( bcore_flect_caps_is_aware( array_item->caps ) )
            {
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_push_c( #<sc_t>* o, vc_t v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_awc( v ) ); }", indent, o->item_name, o->item_name, o->item_name );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_push_d( #<sc_t>* o, vd_t v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_asd( v ) ); }", indent, o->item_name, o->item_name, o->item_name );
            }
        }
    }

    bcore_sink_a_push_fa( sink, "\n" );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_stamp_s_expand_definition( const bcore_precoder_stamp_s* o, const bcore_precoder_s* precoder, sz_t indent, bcore_sink* sink )
{
    const bcore_self_s* self = o->self;
    const st_s* self_string = o->self_source;
    sz_t idx = st_s_find_char( self_string, 0, -1, '=' );
    sc_t self_def = "";
    if( idx < self_string->size )
    {
        self_def = self_string->sc + idx + 1;
    }

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_DEFINE_OBJECT_INST_P( #<sc_t> )\n", indent, ifnameof( self->type ) );

    st_s* multiline_string = create_structured_multiline_string( self_def, indent );
    bcore_sink_a_push_fa( sink, "#<sc_t>;\n", multiline_string->sc );

    for( sz_t i = 0; i < o->funcs.size; i++ )
    {
        bcore_precoder_func_s* func = &o->funcs.data[ i ];
        if( bcore_precoder_s_item_exists( precoder, func->type ) )
        {
            const bcore_precoder_item_s* item = bcore_precoder_s_item_get( precoder, func->type );
            ASSERT( sr_s_type( &item->data ) == TYPEOF_bcore_precoder_feature_s );
            const bcore_precoder_feature_s* feature = item->data.o;

            if( func->body && !func->body->go_inline )
            {
                bcore_sink_a_push_fa( sink, "\n" );
                bcore_sink_a_push_fa( sink, "#rn{ }#<sc_t> #<sc_t>_#<sc_t>( ", indent, feature->ret_type.sc, o->item_name, func->name.sc );
                bcore_sink_a_push_fa( sink, "#<sc_t>", feature->mutable ? "" : "const " );
                bcore_sink_a_push_fa( sink, "#<sc_t>* o", o->item_name );
                bcore_precoder_args_s_expand( &feature->args, sink );
                bcore_sink_a_push_fa( sink, " )\n" );
                bcore_sink_a_push_fa( sink, "#<sc_t>", func->body->code.sc );
                bcore_sink_a_push_fa( sink, "\n" );
            }
        }
    }

    st_s_discard( multiline_string );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_stamp_s_expand_init1( const bcore_precoder_stamp_s* o, const bcore_precoder_s* precoder, sz_t indent, bcore_sink* sink )
{
    for( sz_t i = 0; i < o->funcs.size; i++ )
    {
        bcore_precoder_func_s* func = &o->funcs.data[ i ];
        if( bcore_precoder_s_item_exists( precoder, func->type ) )
        {
            const bcore_precoder_item_s* item = bcore_precoder_s_item_get( precoder, func->type );
            ASSERT( sr_s_type( &item->data ) == TYPEOF_bcore_precoder_feature_s );
            const bcore_precoder_feature_s* feature = item->data.o;
            bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_FFUNC( #<sc_t>, #<sc_t>_#<sc_t> );\n", indent, feature->item_name, o->item_name, func->name.sc );
        }

    }
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_OBJECT( #<sc_t> );\n", indent, o->item_name );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_item_s_expand_forward( const bcore_precoder_item_s* o, sz_t indent, bcore_sink* sink )
{
    switch( sr_s_type( &o->data ) )
    {
        case TYPEOF_bcore_precoder_stamp_s:
        {
            bcore_sink_a_push_fa( sink, " \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o->name.sc );
        }
        break;

        case TYPEOF_bcore_precoder_signature_s:
        {
            // nothing to do
        }
        break;

        case TYPEOF_bcore_precoder_body_s:
        {
            // nothing to do
        }
        break;

        case TYPEOF_bcore_precoder_feature_s:
        {
            // nothing to do
        }
        break;

        case TYPEOF_bcore_precoder_name_s:
        {
            // nothing to do
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

static void bcore_precoder_item_s_expand_declaration( const bcore_precoder_item_s* o, sz_t indent, bcore_sink* sink )
{
    switch( sr_s_type( &o->data ) )
    {
        case TYPEOF_bcore_precoder_stamp_s:
        {
            bcore_precoder_stamp_s_expand_declaration( o->data.o, o->group->source->target->precoder, indent + 2, sink );
        }
        break;

        case TYPEOF_bcore_precoder_signature_s:
        {
            // nothing to do
        }
        break;

        case TYPEOF_bcore_precoder_body_s:
        {
            // nothing to do
        }
        break;

        case TYPEOF_bcore_precoder_feature_s:
        {
            // nothing to do
        }
        break;

        case TYPEOF_bcore_precoder_name_s:
        {
            bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, o->name.sc, typeof( o->name.sc ) );
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

static void bcore_precoder_items_s_expand_indef_declaration( const bcore_precoder_item_s* o, sz_t indent, bcore_sink* sink )
{
    switch( sr_s_type( &o->data ) )
    {
        case TYPEOF_bcore_precoder_signature_s:
        {
            // nothing to do
        }
        break;

        case TYPEOF_bcore_precoder_body_s:
        {
            // nothing to do
        }
        break;

        case TYPEOF_bcore_precoder_feature_s:
        {
            const bcore_precoder_feature_s* feature = o->data.o;
            bcore_precoder_feature_s_expand_indef_declaration( feature, indent, sink );
        }
        break;

        case TYPEOF_bcore_precoder_name_s:
        {
            // nothing to do
        }
        break;

        case TYPEOF_bcore_precoder_stamp_s:
        {
            bcore_sink_a_push_fa( sink, " \\\n#rn{ }  BETH_EXPAND_ITEM_#<sc_t>", indent, o->name.sc );
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
        case TYPEOF_bcore_precoder_stamp_s:
        {
            bcore_precoder_stamp_s_expand_definition( o->data.o, o->group->source->target->precoder, indent, sink );
        }
        break;

        case TYPEOF_bcore_precoder_signature_s:
        {
            /* nothing */
        }
        break;

        case TYPEOF_bcore_precoder_body_s:
        {
            /* nothing */
        }
        break;

        case TYPEOF_bcore_precoder_feature_s:
        {
            /* nothing */
        }
        break;

        case TYPEOF_bcore_precoder_name_s:
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
        case TYPEOF_bcore_precoder_stamp_s:
        {
            bcore_precoder_stamp_s_expand_init1( o->data.o, o->group->source->target->precoder, indent, sink );
        }
        break;

        case TYPEOF_bcore_precoder_signature_s:
        {
            // nothing to do
        }
        break;

        case TYPEOF_bcore_precoder_body_s:
        {
            // nothing to do
        }
        break;

        case TYPEOF_bcore_precoder_feature_s:
        {
            bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_FEATURE( #<sc_t> );\n", indent, o->name.sc );
        }
        break;

        case TYPEOF_bcore_precoder_name_s:
        {
            bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_NAME( #<sc_t> );\n", indent, o->name.sc );
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
        case TYPEOF_bcore_precoder_stamp_s:
        {
            const bcore_precoder_stamp_s* stamp = object;
            st_s_copy_sc( &o->name, nameof( stamp->self->type ) );
            o->hash = bcore_precoder_stamp_s_get_hash( stamp );
            ( ( bcore_precoder_stamp_s* )o->data.o )->item_name = o->name.sc;
        }
        break;

        case TYPEOF_bcore_precoder_signature_s:
        {
            const bcore_precoder_signature_s* precoder_signature = object;
            st_s_copy_fa( &o->name, "#<sc_t>_#<sc_t>", precoder_signature->group_name, precoder_signature->name.sc );
            o->hash = bcore_precoder_signature_s_get_hash( precoder_signature );
            ( ( bcore_precoder_signature_s* )o->data.o )->item_name = o->name.sc;
        }
        break;

        case TYPEOF_bcore_precoder_body_s:
        {
            const bcore_precoder_body_s* body = object;
            st_s_copy_fa( &o->name, "#<sc_t>_#<sc_t>", body->group_name, body->name.sc );
            o->hash = bcore_precoder_body_s_get_hash( body );
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

        case TYPEOF_bcore_precoder_name_s:
        {
            const bcore_precoder_name_s* precoder_name = object;
            st_s_copy( &o->name, &precoder_name->name );
            o->hash = bcore_precoder_name_s_get_hash( precoder_name );
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

static bl_t bcore_precoder_s_group_register( bcore_precoder_s* o, const bcore_precoder_group_s* group );

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_compile( bcore_precoder_group_s* o, bcore_source* source )
{
    sc_t precode_termination = NULL;

    while( !bcore_source_a_eos( source ) )
    {
        if( bcore_source_a_parse_bl_fa( source, "#?'/*'" ) )
        {
            bcore_source_a_parse_fa( source, "#skip'*'" );
            precode_termination = " #?'*/'";
            break;
        }
        else if( bcore_source_a_parse_bl_fa( source, "#?'#ifdef'" ) )
        {
            bcore_source_a_parse_fa( source, " BETH_PRECODE_SECTION " );
            precode_termination = " #?'#endif'";
            break;
        }

        char c = bcore_source_a_get_u0( source );
        if( c != ' ' && c != '\t' && c != '\n' ) bcore_source_a_parse_err_fa( source, "Opening c-style comment '/*' or '#<sc_t>' expected.", "#ifdef BETH_PRECODE_SECTION" );
    }


    while( !bcore_source_a_parse_bl_fa( source, precode_termination ) )
    {
        if( bcore_source_a_eos( source ) )  bcore_source_a_parse_err_fa( source, "Closing c-style comment '*/' expected." );

        bcore_precoder_item_s* item = bcore_precoder_item_s_create();
        item->group = o;

        if( bcore_source_a_parse_bl_fa( source, " #?w'stamp' " ) )
        {
            BCORE_LIFE_INIT();
            BCORE_LIFE_CREATE( bcore_precoder_stamp_s, stamp );
            bcore_precoder_stamp_s_compile( stamp, o, source );
            bcore_precoder_item_s_setup( item, stamp );
            if( !bcore_precoder_s_item_register( o->source->target->precoder, item ) )
            {
                bcore_source_a_parse_err_fa( source, "Identifier #<sc_t> is already in use.", item->name.sc );
            }
            BCORE_LIFE_DOWN();
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'signature' " ) )
        {
            BCORE_LIFE_INIT();
            BCORE_LIFE_CREATE( bcore_precoder_signature_s, precoder_signature );
            precoder_signature->group_name = o->name.sc;
            bcore_precoder_signature_s_compile( precoder_signature, o, source );
            bcore_source_a_parse_fa( source, " ; " );
            bcore_precoder_item_s_setup( item, precoder_signature );

            if( !bcore_precoder_s_item_register( o->source->target->precoder, item ) )
            {
                bcore_source_a_parse_err_fa( source, "Identifier #<sc_t> is already in use.", item->name.sc );
            }
            BCORE_LIFE_DOWN();
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'body' " ) )
        {
            BCORE_LIFE_INIT();
            BCORE_LIFE_CREATE( bcore_precoder_body_s, body );
            bcore_precoder_body_s_compile( body, o, source );
            bcore_source_a_parse_fa( source, " ; " );
            bcore_precoder_item_s_setup( item, body );
            if( !bcore_precoder_s_item_register( o->source->target->precoder, item ) )
            {
                bcore_source_a_parse_err_fa( source, "Identifier #<sc_t> is already in use.", item->name.sc );
            }
            BCORE_LIFE_DOWN();
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'feature' " ) )
        {
            BCORE_LIFE_INIT();
            BCORE_LIFE_CREATE( bcore_precoder_feature_s, precoder_feature );
            precoder_feature->group_name = o->name.sc;
            bcore_precoder_feature_s_compile( precoder_feature, o, source );

            bcore_precoder_item_s_setup( item, precoder_feature );

            o->has_features = true;
            if( precoder_feature->flag_a ) o->is_aware = true;

            if( !bcore_precoder_s_item_register( o->source->target->precoder, item ) )
            {
                bcore_source_a_parse_err_fa( source, "Identifier #<sc_t> is already in use.", item->name.sc );
            }
            BCORE_LIFE_DOWN();
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'name' " ) )
        {
            BCORE_LIFE_INIT();
            BCORE_LIFE_CREATE( bcore_precoder_name_s, precoder_name );
            precoder_name->group_name = o->name.sc;
            bcore_precoder_name_s_compile( precoder_name, o, source );
            bcore_precoder_item_s_setup( item, precoder_name );

            // names are not registered

            o->has_features = true;
            BCORE_LIFE_DOWN();
        }
        else
        {
            bcore_source_a_parse_err_fa( source, "Precode syntax error." );
        }

        o->hash = bcore_tp_fold_tp( o->hash, item->hash );
        bcore_array_a_push( ( bcore_array* )o, sr_asd( item ) );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_expand_forward( const bcore_precoder_group_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o->name.sc );
    for( sz_t i = 0; i < o->size; i++ )
    {
        const bcore_precoder_item_s* item = o->data[ i ];
        bcore_precoder_item_s_expand_forward( item, indent, sink );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_expand_declaration( const bcore_precoder_group_s* o, sz_t indent, bcore_sink* sink )
{
    BCORE_LIFE_INIT();
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }//#rn{-}\n", indent, sz_max( 0, 118 - indent ) );
    bcore_sink_a_push_fa( sink, "#rn{ }// group: #<sc_t>\n", indent, o->name.sc );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, o->name.sc, typeof( o->name.sc ) );

    {
        st_s* spect_name = BCORE_LIFE_A_PUSH( st_s_create_fa( "#<sc_t>_s", o->name.sc ) );
        bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, spect_name->sc, typeof( spect_name->sc ) );
    }

    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_expand_declaration( o->data[ i ], indent, sink );
    bcore_sink_a_push_fa( sink, "#rn{ }##define BETH_EXPAND_GROUP_#<sc_t>", indent, o->name.sc );

    bcore_precoder_group_s_expand_forward( o, indent + 2, sink );

    if( o->has_features )
    {
        for( sz_t i = 0; i < o->size; i++ )
        {
            const bcore_precoder_item_s* item = o->data[ i ];
            if( item->type == TYPEOF_bcore_precoder_feature_s )
            {
                const bcore_precoder_feature_s* feature = item->data.o;
                bcore_precoder_feature_s_expand_indef_typedef( feature, indent, sink );
            }
        }

        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  BCORE_DECLARE_SPECT( #<sc_t> )", indent, o->name.sc );
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  {", indent );
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }      bcore_spect_header_s header;", indent );
        for( sz_t i = 0; i < o->size; i++ )
        {
            const bcore_precoder_item_s* item = o->data[ i ];
            if( item->type == TYPEOF_bcore_precoder_feature_s )
            {
                const bcore_precoder_feature_s* feature = item->data.o;
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      #<sc_t> #<sc_t>;", indent, feature->item_name, feature->name.sc );
            }
        }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  };", indent );

        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_#<sc_t>, t ); return ( #<sc_t>* )bcore_inst_t_create( t ); }", indent, o->name.sc, o->name.sc, o->name.sc, o->name.sc );
        if( o->is_aware )
        {
            bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_a_clone( const #<sc_t>* o ) { return ( #<sc_t>* )bcore_inst_a_clone( ( bcore_inst* )o ); }", indent, o->name.sc, o->name.sc, o->name.sc, o->name.sc );
            bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_a_discard( #<sc_t>* o ) { bcore_inst_a_discard( ( bcore_inst* )o ); }", indent, o->name.sc, o->name.sc );
            bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_a_replicate( #<sc_t>** o, const #<sc_t>* src ) { bcore_inst_a_replicate( ( bcore_inst** )o, ( bcore_inst* )src ); }", indent, o->name.sc, o->name.sc, o->name.sc );
        }
    }

    for( sz_t i = 0; i < o->size; i++ )
    {
        const bcore_precoder_item_s* item = o->data[ i ];
        bcore_precoder_items_s_expand_indef_declaration( item, indent, sink );
    }

    bcore_sink_a_push_fa( sink, "\n" );
    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_expand_spect( const bcore_precoder_group_s* o, sz_t indent, bcore_sink* sink )
{
    BCORE_LIFE_INIT();
    bcore_sink_a_push_fa( sink, "\n" );

    // definition
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_DEFINE_SPECT( bcore_inst, #<sc_t> )\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }\"{\"\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    \"bcore_spect_header_s header;\"\n", indent );
    for( sz_t i = 0; i < o->size; i++ )
    {
        const bcore_precoder_item_s* item = o->data[ i ];
        if( item->type == TYPEOF_bcore_precoder_feature_s )
        {
            const bcore_precoder_feature_s* feature = item->data.o;
            bcore_sink_a_push_fa( sink, "#rn{ }    \"feature ", indent );
            if( feature->strict ) bcore_sink_a_push_fa( sink, "strict " );
            if( feature->flag_a ) bcore_sink_a_push_fa( sink, "aware " );

            bcore_sink_a_push_fa( sink, "#<sc_t> : #<sc_t>", o->name.sc, feature->name.sc );
            if( feature->default_name.size > 0 )
            {
                bcore_sink_a_push_fa( sink, " = #<sc_t>_#<sc_t>", o->name.sc, feature->default_name.sc );
            }
            bcore_sink_a_push_fa( sink, ";\"\n" );
        }
    }
    bcore_sink_a_push_fa( sink, "#rn{ }\"}\";\n", indent );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_group_s_expand_definition( const bcore_precoder_group_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }//#rn{-}\n", indent, sz_max( 0, 118 - indent ) );
    bcore_sink_a_push_fa( sink, "#rn{ }// group: #<sc_t>\n", indent, o->name.sc );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_item_s_expand_definition( o->data[ i ], indent, sink );
    if( o->has_features )
    {
        bcore_precoder_group_s_expand_spect( o, indent, sink );
        for( sz_t i = 0; i < o->size; i++ )
        {
            const bcore_precoder_item_s* item = o->data[ i ];
            if( item->type == TYPEOF_bcore_precoder_feature_s )
            {
                const bcore_precoder_feature_s* feature = item->data.o;
                bcore_precoder_feature_s_expand_definition( feature, indent, o, sink );
            }
        }
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
                if( feature->default_name.size > 0 )
                {
                    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_FFUNC( #<sc_t>, #<sc_t>_#<sc_t> );\n", indent, feature->item_name, feature->group_name, feature->default_name.sc );
                }
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
        if( bcore_source_a_parse_bl_fa( source, "#?w'BETH_PRECODE'" ) )
        {
            bcore_precoder_group_s* group = bcore_precoder_group_s_create();
            group->source = o;
            bcore_source_a_parse_fa( source, " ( #name )", &group->name );
            group->id = typeof( group->name.sc );
            group->hash = group->id;
            bcore_precoder_group_s_compile( group, source );
            o->hash = bcore_tp_fold_tp( o->hash, group->hash );
            if( !bcore_precoder_s_group_register( o->target->precoder, group ) )
            {
                bcore_source_a_parse_err_fa( source, "Group #<sc_t> is already in use.", group->name.sc );
            }
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
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    bcore_sink_a_push_fa( sink, "#rn{ }// source: #<sc_t>\n", indent, o->name.sc );
    for( sz_t i = 0; i < o->size; i++ ) bcore_precoder_group_s_expand_declaration( o->data[ i ], indent, sink );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_source_s_expand_definition( const bcore_precoder_source_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
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
    bcore_sink_a_push_fa( sink, "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }vd_t #<sc_t>_signal_handler( const bcore_signal_s* o );\n", indent, o->name.sc );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##endif // #<sc_t>_H\n", indent, name_upper->sc );

    st_s_discard( name_upper );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_precoder_target_s_expand_init1( const bcore_precoder_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }// Comment or remove line below to rebuild this target.\n", indent, o->name.sc, o->hash );
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
    if( bcore_const_exists( key ) )
    {
        modified = ( *( tp_t* )bcore_const_get_o( key ) != o->hash );
    }
    else
    {
        bcore_msg_fa( "Key for target '#<sc_t>' does not exist. Is signal handler of '#<sc_t>.h' integrated?\n", o->name.sc, o->name.sc );
    }

    #ifdef BCORE_PRECODER_DRY_RUN
    bcore_msg_fa( "Precoder is in mode 'dry-run'\n" );
    if( modified )
    {
        bcore_precoder_target_s_expand_h( o, 0, BCORE_STDOUT );
        bcore_precoder_target_s_expand_c( o, 0, BCORE_STDOUT );
    }
    #else
    if( modified )
    {
        BCORE_LIFE_CREATE( st_s, txt_h );
        BCORE_LIFE_CREATE( st_s, txt_c );
        bcore_precoder_target_s_expand_h( o, 0, ( bcore_sink* )txt_h );
        bcore_precoder_target_s_expand_c( o, 0, ( bcore_sink* )txt_c );

        st_s* file_h = BCORE_LIFE_A_PUSH( st_s_create_fa( "#<sc_t>.h", o->path.sc ) );
        st_s* file_c = BCORE_LIFE_A_PUSH( st_s_create_fa( "#<sc_t>.c", o->path.sc ) );

        if( bcore_file_exists( file_h->sc ) )
        {
            st_s* backup_name = st_s_create_fa( "#<sc_t>.backup", file_h->sc );
            bcore_file_rename( file_h->sc, backup_name->sc );
            st_s_discard( backup_name );
        }

        if( bcore_file_exists( file_c->sc ) )
        {
            st_s* backup_name = st_s_create_fa( "#<sc_t>.backup", file_c->sc );
            bcore_file_rename( file_c->sc, backup_name->sc );
            st_s_discard( backup_name );
        }

        bcore_msg_fa( "writing '#<sc_t>'\n", file_h->sc );
        bcore_sink_a_push_data( BCORE_LIFE_A_PUSH( bcore_file_open_sink( file_h->sc ) ), txt_h->data, txt_h->size );

        bcore_msg_fa( "writing '#<sc_t>'\n", file_c->sc );
        bcore_sink_a_push_data( BCORE_LIFE_A_PUSH( bcore_file_open_sink( file_c->sc ) ), txt_c->data, txt_c->size );

    }

    #endif // BCORE_PRECODER_DRY_RUN

    BCORE_LIFE_RETURN( modified );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// returns false if already registered; checks for collision
static bl_t bcore_precoder_s_group_register( bcore_precoder_s* o, const bcore_precoder_group_s* group )
{
    if( bcore_hmap_tpvd_s_exists( &o->hmap_group, group->id ) )
    {
        /// check collision
        const bcore_precoder_group_s* registered_group = *bcore_hmap_tpvd_s_get( &o->hmap_group, group->id );
        if( registered_group->hash != group->hash )
        {
            ERR_fa( "Collision detected: #<sc_t> vs #<sc_t>\n", group->name.sc, registered_group->name.sc );
        }
        return false;
    }
    bcore_hmap_tpvd_s_set( &o->hmap_group, group->id, ( vd_t )group );
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

/// returns false if already registered; checks for collision
static bl_t bcore_precoder_s_item_register( bcore_precoder_s* o, const bcore_precoder_item_s* item )
{
    if( bcore_hmap_tpvd_s_exists( &o->hmap_item, item->id ) )
    {
        /// check collision
        const bcore_precoder_item_s* registered_item = *bcore_hmap_tpvd_s_get( &o->hmap_item, item->id );
        if( registered_item->hash != item->hash )
        {
            ERR_fa( "Collision detected: #<sc_t> vs #<sc_t>\n", item->name.sc, registered_item->name.sc );
        }
        return false;
    }

    bcore_hmap_tpvd_s_set( &o->hmap_item, item->id, ( vd_t )item );
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

static bl_t bcore_precoder_s_item_exists( const bcore_precoder_s* o, tp_t item_id )
{
    return bcore_hmap_tpvd_s_exists( &o->hmap_item, item_id );
}

//----------------------------------------------------------------------------------------------------------------------

static const bcore_precoder_item_s* bcore_precoder_s_item_get( const bcore_precoder_s* o, tp_t item_id )
{
    vd_t* ptr = bcore_hmap_tpvd_s_get( &o->hmap_item, item_id );
    return ptr ? ( const bcore_precoder_item_s* )*ptr : NULL;
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
            BCORE_REGISTER_OBJECT( bcore_precoder_signature_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_body_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_feature_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_func_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_funcs_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_stamp_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_name_s );
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
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_signature_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_body_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_feature_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_func_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_funcs_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_stamp_s );
            BCORE_REGISTER_QUICKTYPE( bcore_precoder_name_s );
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

