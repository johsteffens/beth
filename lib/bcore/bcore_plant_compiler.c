/** Author and Copyright 2019 Johannes Bernhard Steffens
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
#include "bcore_plant_compiler.h"
#include "bcore_sources.h"
#include "bcore_sinks.h"
#include "bcore_spect_inst.h"
#include "bcore_life.h"
#include "bcore_sc.h"
#include "bcore_file.h"
#include "bcore_spect_hash.h"
#include "bcore_sr.h"
#include "bcore_leaf.h"
#include "bcore_trait.h"
#include "bcore_hmap_tp_sr.h"
#include "bcore_const_manager.h"


//#define BCORE_PLANT_DRY_RUN

//----------------------------------------------------------------------------------------------------------------------
// forwards

BCORE_FORWARD_OBJECT( bcore_plant );
BCORE_FORWARD_OBJECT( bcore_plant_s );
BCORE_FORWARD_OBJECT( bcore_plant_group_s );
BCORE_FORWARD_OBJECT( bcore_plant_source_s );
BCORE_FORWARD_OBJECT( bcore_plant_target_s );
BCORE_FORWARD_OBJECT( bcore_plant_compiler_s );

static const bcore_plant* bcore_plant_compiler_s_item_get( const bcore_plant_compiler_s* o, tp_t item_id );

static bl_t bcore_plant_compiler_s_item_exists( const bcore_plant_compiler_s* o, tp_t item_id );
static tp_t bcore_plant_compiler_s_item_register(     bcore_plant_compiler_s* o, const bcore_plant* item,          bcore_source* source );
static tp_t bcore_plant_compiler_s_group_register(    bcore_plant_compiler_s* o, const bcore_plant_group_s* group, bcore_source* source );
static void bcore_plant_compiler_s_life_a_push(       bcore_plant_compiler_s* o, vd_t object );
static void bcore_plant_group_s_parse_name( bcore_plant_group_s* o, st_s* name, bcore_source* source );
static void bcore_plant_group_s_parse_name_recursive( bcore_plant_group_s* o, st_s* name, bcore_source* source );
static void bcore_plant_source_s_push_group( bcore_plant_source_s* o, bcore_plant_group_s* group );

//----------------------------------------------------------------------------------------------------------------------
// globals

static bcore_plant_compiler_s* plant_compiler_g = NULL;

/**********************************************************************************************************************/
/// plant perspective

BCORE_FORWARD_OBJECT( bcore_plant );

/// features
typedef void (*bcore_plant_fp_parse                     )(       bcore_plant* o, bcore_source* source );
typedef tp_t (*bcore_plant_fp_get_hash                  )( const bcore_plant* o );
typedef sc_t (*bcore_plant_fp_get_global_name_sc        )( const bcore_plant* o );
typedef void (*bcore_plant_fp_finalize                  )(       bcore_plant* o );

typedef void (*bcore_plant_fp_expand_forward            )( const bcore_plant* o, sz_t indent, bcore_sink* sink );
typedef void (*bcore_plant_fp_expand_indef_typedef      )( const bcore_plant* o, sz_t indent, bcore_sink* sink );
typedef void (*bcore_plant_fp_expand_spect_declaration  )( const bcore_plant* o, sz_t indent, bcore_sink* sink );
typedef void (*bcore_plant_fp_expand_spect_definition   )( const bcore_plant* o, sz_t indent, bcore_sink* sink );
typedef void (*bcore_plant_fp_expand_declaration        )( const bcore_plant* o, sz_t indent, bcore_sink* sink );
typedef void (*bcore_plant_fp_expand_indef_declaration  )( const bcore_plant* o, sz_t indent, bcore_sink* sink );
typedef void (*bcore_plant_fp_expand_definition         )( const bcore_plant* o, sz_t indent, bcore_sink* sink );
typedef void (*bcore_plant_fp_expand_init1              )( const bcore_plant* o, sz_t indent, bcore_sink* sink );

BCORE_DECLARE_SPECT( bcore_plant )
{
    bcore_spect_header_s header;

    /// features
    bcore_plant_fp_parse                    parse;
    bcore_plant_fp_get_hash                 get_hash;
    bcore_plant_fp_get_global_name_sc       get_global_name_sc;
    bcore_plant_fp_finalize                 finalize;

    bcore_plant_fp_expand_forward           expand_forward;
    bcore_plant_fp_expand_indef_typedef     expand_indef_typedef;
    bcore_plant_fp_expand_spect_declaration expand_spect_declaration;
    bcore_plant_fp_expand_spect_definition  expand_spect_definition;
    bcore_plant_fp_expand_declaration       expand_declaration;
    bcore_plant_fp_expand_indef_declaration expand_indef_declaration;
    bcore_plant_fp_expand_definition        expand_definition;
    bcore_plant_fp_expand_init1             expand_init1;
};

BCORE_FUNC_SPECT_CONST1_RET0_ARG1_MAPX( bcore_plant, parse,         bcore_source*, source )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP1( bcore_plant, get_hash, tp_t                       )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP1( bcore_plant, get_global_name_sc, sc_t             )
BCORE_FUNC_SPECT_CONST1_RET0_ARG0_MAPX( bcore_plant, finalize                             )

BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bcore_plant, expand_forward,           sz_t, indent, bcore_sink*, sink )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bcore_plant, expand_indef_typedef,     sz_t, indent, bcore_sink*, sink )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bcore_plant, expand_spect_declaration, sz_t, indent, bcore_sink*, sink )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bcore_plant, expand_spect_definition,  sz_t, indent, bcore_sink*, sink )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bcore_plant, expand_declaration,       sz_t, indent, bcore_sink*, sink )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bcore_plant, expand_indef_declaration, sz_t, indent, bcore_sink*, sink )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bcore_plant, expand_definition,        sz_t, indent, bcore_sink*, sink )
BCORE_FUNC_SPECT_CONST1_RET0_ARG2_MAPX( bcore_plant, expand_init1,             sz_t, indent, bcore_sink*, sink )

BCORE_DEFINE_SPECT( bcore_inst, bcore_plant )
"{"
    "bcore_spect_header_s header;"

    "feature        bcore_plant_fp: parse;"
    "feature strict bcore_plant_fp: get_hash;"
    "feature        bcore_plant_fp: get_global_name_sc;"

    "feature        bcore_plant_fp: finalize;" // last call before expansion

    "feature        bcore_plant_fp: expand_forward;"
    "feature        bcore_plant_fp: expand_indef_typedef;"
    "feature        bcore_plant_fp: expand_spect_declaration;"
    "feature        bcore_plant_fp: expand_spect_definition;"
    "feature        bcore_plant_fp: expand_declaration;"
    "feature        bcore_plant_fp: expand_indef_declaration;"
    "feature        bcore_plant_fp: expand_definition;"
    "feature        bcore_plant_fp: expand_init1;"
"}";

void bcore_plant_default_finalize                 ( const bcore_plant_s* p, const bcore_plant* o                                ) {}
void bcore_plant_default_expand_forward           ( const bcore_plant_s* p, const bcore_plant* o, sz_t indent, bcore_sink* sink ) {}
void bcore_plant_default_expand_indef_typedef     ( const bcore_plant_s* p, const bcore_plant* o, sz_t indent, bcore_sink* sink ) {}
void bcore_plant_default_expand_spect_declaration ( const bcore_plant_s* p, const bcore_plant* o, sz_t indent, bcore_sink* sink ) {}
void bcore_plant_default_expand_spect_definition  ( const bcore_plant_s* p, const bcore_plant* o, sz_t indent, bcore_sink* sink ) {}
void bcore_plant_default_expand_declaration       ( const bcore_plant_s* p, const bcore_plant* o, sz_t indent, bcore_sink* sink ) {}
void bcore_plant_default_expand_indef_declaration ( const bcore_plant_s* p, const bcore_plant* o, sz_t indent, bcore_sink* sink ) {}
void bcore_plant_default_expand_definition        ( const bcore_plant_s* p, const bcore_plant* o, sz_t indent, bcore_sink* sink ) {}
void bcore_plant_default_expand_init1             ( const bcore_plant_s* p, const bcore_plant* o, sz_t indent, bcore_sink* sink ) {}

/**********************************************************************************************************************/
// plant objects

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_plant_arg_s )
{
    aware_t _;
    bcore_plant_group_s* group;
    st_s type;
    st_s name;
    bcore_source_point_s source_point;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_plant_arg_s )
"{"
    "aware_t _;"
    "hidden vd_t group;"
    "st_s type;"
    "st_s name;"
    "hidden bcore_source_point_s source_point;"

    "func bcore_plant_fp : parse;"
    "func bcore_plant_fp : get_hash;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_plant_args_s )
{
    aware_t _;
    bcore_plant_group_s* group;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( bcore_plant_arg_s, );
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_plant_args_s )
"{"
    "aware_t _;"
    "vd_t group;"
    "bcore_plant_arg_s [] arr;"

    "func bcore_plant_fp : parse;"
    "func bcore_plant_fp : get_hash;"
"}";

//----------------------------------------------------------------------------------------------------------------------

// example:
// signature void myfeature( mutable, sz_t a, sz_t b );
BCORE_DECLARE_OBJECT( bcore_plant_signature_s )
{
    aware_t _;
    bcore_plant_group_s* group;
    st_s name;       // myfeature
    st_s global_name;
    bl_t has_ret;
    st_s ret_type;        // void
    bcore_source_point_s source_point;
    bcore_plant_args_s args;       // sz_t a, sz_t b
    tp_t arg_o; // mutable | const | 0
};

BCORE_DEFINE_OBJECT_INST( bcore_plant, bcore_plant_signature_s )
"{"
    "aware_t _;"
    "vd_t group;"
    "st_s name;"         // myfeature
    "st_s global_name;"
    "bl_t has_ret;"
    "st_s ret_type;"        // void
    "bcore_source_point_s source_point;"
    "bcore_plant_args_s args;"       // sz_t a, sz_t b
    "tp_t arg_o;" // mutable | const | 0

    "func bcore_plant_fp : parse;"
    "func bcore_plant_fp : get_hash;"
    "func bcore_plant_fp : get_global_name_sc;"
"}";

//----------------------------------------------------------------------------------------------------------------------

// function body (name is always local to group_name)
BCORE_DECLARE_OBJECT( bcore_plant_body_s )
{
    aware_t _;
    bcore_plant_group_s* group;
    st_s name;
    st_s global_name;
    st_s code;
    bl_t go_inline; // prefer inline expansion
    bcore_source_point_s source_point;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_plant_body_s )
"{"
    "aware_t _;"
    "vd_t group;"
    "st_s name;"
    "st_s global_name;"
    "st_s code;"
    "bl_t go_inline;"
    "bcore_source_point_s source_point;"

    "func bcore_plant_fp : parse;"
    "func bcore_plant_fp : get_hash;"
    "func bcore_plant_fp : get_global_name_sc;"
"}";

//----------------------------------------------------------------------------------------------------------------------

// example:
// feature void myfeature( mutable, sz_t a, sz_t b );
BCORE_DECLARE_OBJECT( bcore_plant_feature_s )
{
    aware_t _;
    bcore_plant_group_s* group;

    st_s name;       // myfeature
    st_s global_name;
    st_s default_name;
    bcore_plant_body_s* default_body;

    bl_t strict;

    bl_t flag_p;
    bl_t flag_t;
    bl_t flag_a;
    bl_t flag_r;

    bl_t has_ret;
    st_s ret_type;        // void
    bcore_plant_args_s args;       // sz_t a, sz_t b
    bl_t mutable;
    bcore_source_point_s source_point;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_plant_feature_s )
"{"
    "aware_t _;"
    "vd_t group;"

    "st_s name;"       // myfeature
    "st_s global_name;"
    "st_s default_name;"
    "bcore_plant_body_s => default_body;"

    "bl_t strict;"

    "bl_t flag_p;"
    "bl_t flag_t;"
    "bl_t flag_a;"
    "bl_t flag_r;"

    "bl_t has_ret;"
    "st_s ret_type;"        // void
    "bcore_plant_args_s args;"       // sz_t a, sz_t b
    "bl_t mutable;"
    "bcore_source_point_s source_point;"

    "func bcore_plant_fp : parse;"
    "func bcore_plant_fp : get_hash;"
    "func bcore_plant_fp : get_global_name_sc;"

    "func bcore_plant_fp : expand_indef_typedef;"
    "func bcore_plant_fp : expand_spect_declaration;"
    "func bcore_plant_fp : expand_spect_definition;"
    "func bcore_plant_fp : expand_indef_declaration;"
    "func bcore_plant_fp : expand_definition;"
    "func bcore_plant_fp : expand_init1;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_plant_name_s )
{
    aware_t _;
    bcore_plant_group_s* group;
    st_s name; // deemed global
    bcore_source_point_s source_point;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_plant_name_s )
"{"
    "aware_t _;"
    "vd_t group;"
    "st_s name;" // deemed global
    "bcore_source_point_s source_point;"

    "func bcore_plant_fp : parse;"
    "func bcore_plant_fp : get_hash;"
    "func bcore_plant_fp : get_global_name_sc;"

    "func bcore_plant_fp : expand_declaration;"
    "func bcore_plant_fp : expand_init1;"
"}";

//----------------------------------------------------------------------------------------------------------------------

// used in bcore_plant_stamp_s and bcore_plant_group_s
BCORE_DECLARE_OBJECT( bcore_plant_func_s )
{
    aware_t _;
    bcore_plant_group_s* group;
    st_s name; // local name
    st_s decl; // declaration string in stamp
    tp_t type; // full type of function ((feature|signature):name)
    bl_t overloadable;
    bcore_plant_body_s* body;
    bcore_source_point_s source_point;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_plant_func_s )
"{"
    "aware_t _;"
    "vd_t group;"
    "st_s name;"
    "st_s decl;" // declaration string for stamp
    "tp_t type;"
    "bl_t overloadable = false;"
    "bcore_plant_body_s => body;"
    "bcore_source_point_s source_point;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_plant_funcs_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_plant_func_s, );
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_plant_funcs_s )
"{"
    "aware_t _;"
    "bcore_plant_func_s => [] arr;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_plant_stamp_s )
{
    aware_t _;
    bcore_plant_group_s* group;

    st_s           name; // global name
    st_s           trait_name;
    st_s         * self_source;
    bcore_plant_funcs_s funcs;
    bcore_source_point_s source_point;
};

BCORE_DEFINE_OBJECT_INST( bcore_plant, bcore_plant_stamp_s )
"{"
    "aware_t _;"
    "vd_t group;"

    "st_s            name;" // global name
    "st_s            trait_name;"
    "st_s         => self_source;"
    "bcore_plant_funcs_s funcs;"
    "bcore_source_point_s source_point;"

    "func bcore_plant_fp : parse;"
    "func bcore_plant_fp : get_hash;"
    "func bcore_plant_fp : get_global_name_sc;"
    "func bcore_plant_fp : finalize;"

    "func bcore_plant_fp : expand_declaration;"
    "func bcore_plant_fp : expand_forward;"
    "func bcore_plant_fp : expand_indef_declaration;"
    "func bcore_plant_fp : expand_definition;"
    "func bcore_plant_fp : expand_init1;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_plant_group_s )
{
    aware_t _;
    bcore_plant_group_s* group; // parent group;

    st_s name; // global name
    st_s trait_name; // trait name (default bcore_inst)

    tp_t hash;
    bl_t expandable;
    bl_t has_features;
    bl_t is_aware;
    bl_t retrievable; // causes all stamps to fully enroll during init cycle;
    BCORE_ARRAY_DYN_LINK_AWARE_VIRTUAL_S( bcore_plant, );

    bcore_plant_stamp_s*  extending; // !=NULL: extends this stamp on subsequent stamps
    bcore_plant_funcs_s   funcs;
    bcore_plant_source_s* source;
    bcore_source_point_s  source_point;
};

BCORE_DEFINE_OBJECT_INST( bcore_plant, bcore_plant_group_s )
"{"
    "aware_t _;"
    "vd_t group;" // parent group;

    "st_s name;" // global name
    "st_s trait_name = \"bcore_inst\";" // trait name
    "tp_t hash;"
    // 'expandable' is set 'false' for groups that is not intended to be expanded into actual code
    // but may contain information referenced in other groups (e.g. global features)
    "bl_t expandable = true;"
    "bl_t has_features;"
    "bl_t is_aware;"
    "bl_t retrievable;"
    "aware bcore_plant => [] arr;"

    "private bcore_plant_stamp_s -> extending;" // !=NULL: extends this stamp on subsequent stamps
    "bcore_plant_funcs_s  funcs;"
    "private vd_t         source;"
    "bcore_source_point_s source_point;"

    "func bcore_plant_fp : parse;"
    "func bcore_plant_fp : get_hash;"
    "func bcore_plant_fp : get_global_name_sc;"
    "func bcore_plant_fp : finalize;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_plant_nested_group_s )
{
    aware_t _;
    bcore_plant_group_s* group; // group object;
};

BCORE_DEFINE_OBJECT_INST( bcore_plant, bcore_plant_nested_group_s )
"{"
    "aware_t _;"
    "vd_t group;" // group object;
    "func bcore_plant_fp : get_hash;"
    "func bcore_plant_fp : expand_forward;"
    "func bcore_plant_fp : expand_indef_declaration;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_plant_source_s )
{
    aware_t _;
    st_s name; // source name (e.g. "bcore_plant_sample")
    st_s path; // source path excluding extension
    tp_t hash;
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_plant_group_s, );
    bcore_plant_target_s* target;
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_plant_source_s )
"{"
    "aware_t _;"
    "st_s name;" // file name excluding directory and extension
    "st_s path;" // file path excluding extension
    "tp_t hash;"
    "bcore_plant_group_s => [] arr;"
    "private vd_t target;"
    "func bcore_plant_fp : finalize;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_plant_target_s )
{
    aware_t _;
    st_s name; // target name (e.g. "bcore_plant")
    st_s path; // path excluding extension
    tp_t hash;
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_plant_source_s, );
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_plant_target_s )
"{"
    "aware_t _;"
    "st_s name;" // target name (e.g. "bcore_plant")
    "st_s path;" // path excluding extension
    "tp_t hash;"
    "bcore_plant_source_s => [] arr;"
    "func bcore_plant_fp : finalize;"
"}";

//----------------------------------------------------------------------------------------------------------------------

BCORE_DECLARE_OBJECT( bcore_plant_compiler_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_plant_target_s, );
    bcore_hmap_tpvd_s hmap_group;
    bcore_hmap_tpvd_s hmap_item;
    bcore_life_s      life; // lifetime manager for items no managed elsewhere
};

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_plant_compiler_s )
"{"
    "aware_t _;"
    "bcore_plant_target_s => [] arr;"
    "bcore_hmap_tpvd_s hmap_group;"
    "bcore_hmap_tpvd_s hmap_item;"
    "bcore_life_s      life;" // lifetime manager for items no managed elsewhere
    "func bcore_plant_fp : finalize;"
"}";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// parsing tools

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
/// arg

static void bcore_plant_arg_s_parse( bcore_plant_arg_s* o, bcore_source* source )
{
    bcore_source_point_s_set( &o->source_point, source );
    st_s* s = st_s_create();
    if( bcore_source_a_parse_bl_fa( source, "#?'const' " ) ) st_s_push_sc( &o->type, "const " );

    if( bcore_source_a_parse_bl_fa( source, "#?':' " ) )
    {
        bcore_plant_group_s_parse_name_recursive( o->group, s, source );
    }
    else
    {
        bcore_source_a_parse_fa( source, "#name ", s );
        if( s->size == 0 ) bcore_source_a_parse_err_fa( source, "Argument: Type expected." );
    }
    st_s_push_st( &o->type, s );


    while( bcore_source_a_parse_bl_fa( source, "#?'*' " ) ) st_s_push_sc( &o->type, "*" );

    bcore_source_a_parse_fa( source, "#name ", s );
    if( s->size == 0 ) bcore_source_a_parse_err_fa( source, "Argument: Name expected." );
    st_s_push_st( &o->name, s );

    st_s_discard( s );
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_plant_arg_s_get_hash( const bcore_plant_arg_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->type.sc );
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// args

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_args_s_clear( bcore_plant_args_s* o )
{
    bcore_array_a_set_space( ( bcore_array* ) o, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_args_s_parse( bcore_plant_args_s* o, bcore_source* source )
{
    bcore_plant_args_s_clear( o );
    while( bcore_source_a_parse_bl_fa( source, " #?',' " ) ) // args follow
    {
        bcore_plant_arg_s* arg = bcore_plant_arg_s_create();
        arg->group = o->group;
        bcore_plant_arg_s_parse( arg, source );
        bcore_array_a_push( ( bcore_array* ) o, sr_asd( arg ) );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_args_s_append( bcore_plant_args_s* o, bcore_source* source )
{
    bl_t first = true;
    while( !bcore_source_a_parse_bl_fa( source, " #=?')' " ) ) // args follow
    {
        if( !first ) bcore_source_a_parse_fa( source, " , " );
        bcore_plant_arg_s* arg = bcore_plant_arg_s_create();
        arg->group = o->group;
        bcore_plant_arg_s_parse( arg, source );
        bcore_array_a_push( ( bcore_array* ) o, sr_asd( arg ) );
        first = false;
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_arg_s_expand( const bcore_plant_arg_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#<sc_t> #<sc_t>", o->type.sc, o->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_arg_s_expand_name( const bcore_plant_arg_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#<sc_t>", o->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_args_s_expand( const bcore_plant_args_s* o, bl_t first, bcore_sink* sink )
{
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( !first ) bcore_sink_a_push_fa( sink, ", " );
        first = false;
        bcore_plant_arg_s_expand( &o->data[ i ], sink );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_args_s_expand_name( const bcore_plant_args_s* o, bl_t first, bcore_sink* sink )
{
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( !first ) bcore_sink_a_push_fa( sink, ", " );
        first = false;
        bcore_plant_arg_s_expand_name( &o->data[ i ], sink );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_plant_args_s_get_hash( const bcore_plant_args_s* o )
{
    tp_t hash = bcore_tp_init();
    for( sz_t i = 0; i < o->size; i++ ) hash = bcore_tp_fold_tp( hash, bcore_plant_arg_s_get_hash( &o->data[ i ] ) );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// signature

//----------------------------------------------------------------------------------------------------------------------

static sc_t bcore_plant_signature_s_get_global_name_sc( const bcore_plant_signature_s* o )
{
    return o->global_name.sc;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_plant_signature_s_get_hash( const bcore_plant_signature_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->global_name.sc );
    hash = bcore_tp_fold_u0( hash, o->has_ret ? 1 : 0 );
    hash = bcore_tp_fold_sc( hash, o->ret_type.sc );
    hash = bcore_tp_fold_tp( hash, bcore_plant_args_s_get_hash( &o->args ) );
    hash = bcore_tp_fold_tp( hash, o->arg_o );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_signature_s_parse( bcore_plant_signature_s* o, bcore_source* source )
{
    BLM_INIT();

    st_s* name_buf = BLM_CREATE( st_s );
    st_s* name_candidate = BLM_CREATE( st_s );
    st_s_clear( &o->ret_type );

    bcore_source_point_s_set( &o->source_point, source );

    bcore_plant_group_s_parse_name( o->group, name_buf, source );

    bl_t predefined = false;

    if( !bcore_source_a_parse_bl_fa( source, " #=?'*'" ) )
    {
        if( bcore_source_a_parse_bl_fa( source, " #?':'" ) )
        {
            predefined = true;

            bcore_source_a_parse_fa( source, " #name", name_candidate );
            if( name_buf->size == 0 )
            {
                st_s_push_fa( name_buf, "#<sc_t>_#<sc_t>", o->group->name.sc, name_candidate->sc );
            }
            else
            {
                st_s_push_fa( name_buf, "_#<sc_t>", name_candidate->sc );
            }
        }

        tp_t tp_name = typeof( name_buf->sc );

        // if name_buf refers to another signature
        if( bcore_plant_compiler_s_item_exists( plant_compiler_g, tp_name ) )
        {
            vc_t item = bcore_plant_compiler_s_item_get( plant_compiler_g, tp_name );
            if( *(aware_t*)item == TYPEOF_bcore_plant_signature_s )
            {
                const bcore_plant_signature_s* signature = item;
                o->has_ret = signature->has_ret;
                st_s_copy( &o->ret_type, &signature->ret_type );
                bcore_plant_args_s_copy( &o->args, &signature->args );
                o->arg_o = signature->arg_o;
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
    o->args.group = o->group;
    if( !predefined )
    {
        bcore_source_a_parse_fa( source, " ( " );
        if(      bcore_source_a_parse_bl_fa(  source, " #?'mutable' " ) ) o->arg_o = TYPEOF_mutable;
        else if( bcore_source_a_parse_bl_fa(  source, " #?'const' "   ) ) o->arg_o = TYPEOF_const;
        else if( bcore_source_a_parse_bl_fa(  source, " #?'plain' "   ) ) o->arg_o = 0;
        else     bcore_source_a_parse_err_fa( source, "'plain', mutable' or 'const' expected." );

        bcore_plant_args_s_parse( &o->args, source );
        bcore_source_a_parse_fa( source, " ) " );
    }
    else if( bcore_source_a_parse_bl_fa( source, " #?'('" ) )
    {
        bcore_plant_args_s_append( &o->args, source );
        bcore_source_a_parse_fa( source, " ) " );
    }

    st_s_copy_fa( &o->global_name, "#<sc_t>_#<sc_t>", o->group->name.sc, o->name.sc );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// body

//----------------------------------------------------------------------------------------------------------------------

static sc_t bcore_plant_body_s_get_global_name_sc( const bcore_plant_body_s* o )
{
    return o->global_name.sc;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_plant_body_s_get_hash( const bcore_plant_body_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    hash = bcore_tp_fold_sc( hash, o->code.sc );
    hash = bcore_tp_fold_u0( hash, o->go_inline ? 1 : 0 );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_body_s_parse_code( bcore_plant_body_s* o, bcore_source* source )
{
    BLM_INIT();

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

            case '"': // string literal
            {
                st_s_push_char( &o->code, c );
                bl_t esc = false;
                while( !bcore_source_a_eos( source ) )
                {
                    u0_t c = bcore_source_a_get_u0( source );
                    st_s_push_char( &o->code, c );
                    if( !esc && c == '"' ) break;
                    esc = ( c == '\\' );
                    if( c == '\n' ) bcore_source_a_parse_err_fa( source, "Newline in string literal." );
                }
                c = 0;
            }
            break;

            case '\\': // escape
            {
                c = bcore_source_a_get_u0( source );
                if( c != ':' ) st_s_push_char( &o->code, '\\' );
            }
            break;

            case '\n': new_line = true; o->go_inline = false; break;
            case ' ' : if( new_line ) space_count++; break;

            // namespace: if a name or another ':' follows immediately,
            // ':' is interpreted as namespace-prepend
            // ordinary c-code using ':' should append a whitespace or prepend escape '\:'
            case ':':
            {
                if( bcore_source_a_parse_bl_fa( source, "#?([0]==':'||([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z'))" ) )
                {
                    st_s* name = st_s_create();
                    bcore_plant_group_s_parse_name_recursive( o->group, name, source );
                    st_s_push_sc( &o->code, name->sc );
                    st_s_discard( name );
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

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_body_s_parse( bcore_plant_body_s* o, bcore_source* source )
{
    BLM_INIT();

    st_s* string = BLM_CREATE( st_s );

    bcore_source_point_s_set( &o->source_point, source );

    if( !bcore_source_a_parse_bl_fa( source, " #=?'='" ) )
    {
        bcore_source_a_parse_fa( source, " #name", string );
        if( string->size == 0 ) bcore_source_a_parse_err_fa( source, "Body name expected." );
        st_s_push_fa( &o->name, "#<sc_t>", string->sc );
    }

    bcore_source_a_parse_fa( source, " =" );

    if( bcore_source_a_parse_bl_fa( source, " #=?'{'" ) )
    {
        bcore_plant_body_s_parse_code( o, source );
    }
    else
    {
        st_s* name = BLM_CREATE( st_s );
        bcore_plant_group_s_parse_name( o->group, name, source );
        if( name->size == 0 ) bcore_source_a_parse_err_fa( source, "Body name expected." );

        tp_t tp_name = typeof( name->sc );
        // if name_buf refers to another body
        if( bcore_plant_compiler_s_item_exists( plant_compiler_g, tp_name ) )
        {
            vc_t item = bcore_plant_compiler_s_item_get( plant_compiler_g, tp_name );
            if( *(aware_t*)item == TYPEOF_bcore_plant_body_s )
            {
                const bcore_plant_body_s* body = item;
                st_s_copy( &o->code, &body->code );
            }
        }
        else
        {
            bcore_source_a_parse_err_fa( source, "Cannot resolve body name '#<sc_t>'\n", name->sc );
        }
    }

    st_s_copy_fa( &o->global_name, "#<sc_t>_#<sc_t>", o->group->name.sc, o->name.sc );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// feature

//----------------------------------------------------------------------------------------------------------------------

static sc_t bcore_plant_feature_s_get_global_name_sc( const bcore_plant_feature_s* o )
{
    return o->global_name.sc;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_plant_feature_s_get_hash( const bcore_plant_feature_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->global_name.sc );
    hash = bcore_tp_fold_sc( hash, o->default_name.sc );
    if( o->default_body )
    {
        hash = bcore_tp_fold_tp( hash, bcore_plant_body_s_get_hash( o->default_body ) );
    }

    hash = bcore_tp_fold_u0( hash, o->strict ? 1 : 0 );
    hash = bcore_tp_fold_u0( hash, o->flag_p ? 1 : 0 );
    hash = bcore_tp_fold_u0( hash, o->flag_t ? 1 : 0 );
    hash = bcore_tp_fold_u0( hash, o->flag_a ? 1 : 0 );
    hash = bcore_tp_fold_u0( hash, o->flag_r ? 1 : 0 );

    hash = bcore_tp_fold_u0( hash, o->has_ret ? 1 : 0 );
    hash = bcore_tp_fold_sc( hash, o->ret_type.sc );
    hash = bcore_tp_fold_tp( hash, bcore_plant_args_s_get_hash( &o->args ) );
    hash = bcore_tp_fold_u0( hash, o->mutable ? 1 : 0 );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_feature_s_parse( bcore_plant_feature_s* o, bcore_source* source )
{
    BLM_INIT();

    bcore_source_point_s_set( &o->source_point, source );

    o->strict = bcore_source_a_parse_bl_fa(  source, " #?w'strict' " );

    if( bcore_source_a_parse_bl_fa( source, " #?|'|" ) )
    {
        st_s* flags = st_s_create();
        bcore_source_a_parse_fa(  source, " #until'''", flags );
        for( sz_t i = 0; i < flags->size; i++ )
        {
            switch( flags->data[ i ] )
            {
                case 'p': o->flag_p = true; break;
                case 't': o->flag_t = true; break;
                case 'a': o->flag_a = true; break;
                case 'r': o->flag_r = true; break;
                default: bcore_source_a_parse_err_fa( source, "Feature: Flag '#<char>' not handled. Choose from 'ptar'.", flags->data[ i ] ); break;
            }
        }
        st_s_discard( flags );
        bcore_source_a_parse_fa( source, "' " );
    }
    else
    {
        bcore_source_a_parse_err_fa( source, "Feature: '<flags>' expected. Example: 'ptar'" );
    }

    bcore_plant_signature_s* signature = BLM_CREATE( bcore_plant_signature_s );

    signature->group = o->group;
    bcore_plant_signature_s_parse( signature, source );
    st_s_copy( &o->name, &signature->name );
    o->has_ret = signature->has_ret;
    st_s_copy( &o->ret_type, &signature->ret_type );
    bcore_plant_args_s_copy( &o->args, &signature->args );

    if( !signature->arg_o )
    {
        bcore_source_a_parse_err_fa( source, "Feature: The first argument must be 'mutable' or 'const'." );
    }

    o->mutable = ( signature->arg_o == TYPEOF_mutable );

    if( bcore_source_a_parse_bl_fa( source, " #?'=' " ) )
    {
        if( bcore_source_a_parse_bl_fa( source, " #=?'{' " ) )
        {
            if( o->strict )  bcore_source_a_parse_err_fa( source, "Feature is 'strict'. Default function would have no effect." );
            o->default_body = bcore_plant_body_s_create();
            o->default_body->group = o->group;
            bcore_plant_body_s_parse_code( o->default_body, source );
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

    st_s_copy_fa( &o->global_name, "#<sc_t>_#<sc_t>", o->group->name.sc, o->name.sc );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_feature_s_expand_indef_typedef( const bcore_plant_feature_s* o, sz_t indent, bcore_sink* sink )
{
    //typedef ret_t (*feature_func)( feature* o, arg_t arg1 );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  typedef #<sc_t> (*#<sc_t>_#<sc_t>)(", indent, o->ret_type.sc, o->group->name.sc, o->name.sc );
    if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
    bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group->name.sc );
    bcore_plant_args_s_expand( &o->args, false, sink );
    bcore_sink_a_push_fa( sink, " );" );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_feature_s_expand_spect_declaration( const bcore_plant_feature_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }#<sc_t> #<sc_t>;", indent, o->global_name.sc, o->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_feature_s_expand_spect_definition( const bcore_plant_feature_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }\"feature ", indent );
    if( o->strict ) bcore_sink_a_push_fa( sink, "strict " );
    if( o->flag_a ) bcore_sink_a_push_fa( sink, "aware " );

    bcore_sink_a_push_fa( sink, "#<sc_t> : #<sc_t>", o->group->name.sc, o->name.sc );
    if( o->default_name.size > 0 )
    {
        bcore_sink_a_push_fa( sink, " = #<sc_t>_#<sc_t>", o->group->name.sc, o->default_name.sc );
    }
    bcore_sink_a_push_fa( sink, ";\"\n" );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_feature_s_expand_indef_declaration( const bcore_plant_feature_s* o, sz_t indent, bcore_sink* sink )
{
    if( o->flag_p )
    {
        // static inline ret_t feature_p_func( const spect* __p, feature* o, arg_t arg1 ) { assert( __p->func ); return __p->func( o, arg1 ); }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t> #<sc_t>_p_#<sc_t>( const #<sc_t>_s* __p,", indent, o->ret_type.sc, o->group->name.sc, o->name.sc, o->group->name.sc );
        if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
        bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group->name.sc );
        bcore_plant_args_s_expand( &o->args, false, sink );
        bcore_sink_a_push_fa( sink, " ) { " );
        bcore_sink_a_push_fa( sink, "assert( __p->#<sc_t> ); ", o->name.sc );
        if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
        bcore_sink_a_push_fa( sink, "__p->#<sc_t>( o", o->name.sc );
        bcore_plant_args_s_expand_name( &o->args, false, sink );
        bcore_sink_a_push_fa( sink, " ); }" );
    }

    if( o->flag_t )
    {
        // static inline ret_t feature_t_func( tp_t __t, feature* o, arg_t arg1 ) { features_s* p = features_s_get_typed( __t ); assert( p->func ); return features_s_get_typed( __t )->func( o, arg1 ); }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t> #<sc_t>_t_#<sc_t>( tp_t __t,", indent, o->ret_type.sc, o->group->name.sc, o->name.sc, o->group->name.sc );
        if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
        bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group->name.sc );
        bcore_plant_args_s_expand( &o->args, false, sink );
        bcore_sink_a_push_fa( sink, " ) { " );
        bcore_sink_a_push_fa( sink, "const #<sc_t>_s* p = #<sc_t>_s_get_typed( __t ); ", o->group->name.sc, o->group->name.sc );
        bcore_sink_a_push_fa( sink, "assert( p->#<sc_t> ); ", o->name.sc );
        if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
        bcore_sink_a_push_fa( sink, "p->#<sc_t>( o", o->name.sc );
        bcore_plant_args_s_expand_name( &o->args, false, sink );
        bcore_sink_a_push_fa( sink, " ); }" );
    }

    if( o->flag_a )
    {
        // static inline ret_t feature_a_func( feature* o, arg_t arg1 ) { const features_s* p = features_s_get_aware( o ); assert( p->func ); return p->func( o, arg1 ); }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t> #<sc_t>_a_#<sc_t>(", indent, o->ret_type.sc, o->group->name.sc, o->name.sc );
        if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
        bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group->name.sc );
        bcore_plant_args_s_expand( &o->args, false, sink );
        bcore_sink_a_push_fa( sink, " ) { " );
        bcore_sink_a_push_fa( sink, "const #<sc_t>_s* p = #<sc_t>_s_get_aware( o ); ", o->group->name.sc, o->group->name.sc );
        bcore_sink_a_push_fa( sink, "assert( p->#<sc_t> ); ", o->name.sc );
        if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
        bcore_sink_a_push_fa( sink, "p->#<sc_t>( o", o->name.sc );
        bcore_plant_args_s_expand_name( &o->args, false, sink );
        bcore_sink_a_push_fa( sink, " ); }" );
    }

    if( o->flag_r )
    {
        // static inline ret_t feature_r_func( const sr_s* o, arg_t arg1 ) { const features_s* p = (const features_s*)ch_spect_p( o->p, TYPEOF_feature_s ) ); assert( p->func ); return p->func( o->o, arg1 ); }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t> #<sc_t>_r_#<sc_t>(", indent, o->ret_type.sc, o->group->name.sc, o->name.sc );
        bcore_sink_a_push_fa( sink, " const sr_s* o" );
        bcore_plant_args_s_expand( &o->args, false, sink );
        bcore_sink_a_push_fa( sink, " ) { " );
        if( o->mutable ) bcore_sink_a_push_fa( sink, "ASSERT( !sr_s_is_const( o ) ); ", indent );
        bcore_sink_a_push_fa( sink, "const #<sc_t>_s* p = (const #<sc_t>_s*)ch_spect_p( o->p, TYPEOF_#<sc_t>_s ); ", o->group->name.sc, o->group->name.sc, o->group->name.sc );
        bcore_sink_a_push_fa( sink, "assert( p->#<sc_t> ); ", o->name.sc );
        if( o->has_ret ) bcore_sink_a_push_fa( sink, "return " );
        bcore_sink_a_push_fa( sink, "p->#<sc_t>( o->o", o->name.sc );
        bcore_plant_args_s_expand_name( &o->args, false, sink );
        bcore_sink_a_push_fa( sink, " ); }" );
    }

    if( o->flag_p )
    {
        // static inline bl_t feature_p_defines_func( const spect* __p ) { return __p->func != NULL; }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline bl_t #<sc_t>_p_defines_#<sc_t>( const #<sc_t>_s* __p ) ", indent, o->group->name.sc, o->name.sc, o->group->name.sc );
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
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline bl_t #<sc_t>_t_defines_#<sc_t>( tp_t __t ) ", indent, o->group->name.sc, o->name.sc );
        if( o->strict || o->default_body || o->default_name.size > 0 )
        {
            bcore_sink_a_push_fa( sink, "{ return true; }", o->name.sc );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "{ return #<sc_t>_s_get_typed( __t )->#<sc_t> != NULL; }", o->group->name.sc, o->name.sc );
        }
    }

    if( o->flag_a )
    {
        // static inline bl_t feature_a_defines_func( const feature* o) { return features_s_get_aware( o )->func != NULL; }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline bl_t #<sc_t>_a_defines_#<sc_t>( const #<sc_t>* o ) ", indent, o->group->name.sc, o->name.sc, o->group->name.sc );
        if( o->strict || o->default_body || o->default_name.size > 0 )
        {
            bcore_sink_a_push_fa( sink, "{ return true; }", o->name.sc );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "{ return #<sc_t>_s_get_aware( o )->#<sc_t> != NULL; }", o->group->name.sc, o->name.sc );
        }
    }

    if( o->flag_r )
    {
        // static inline bl_t feature_r_defines_func( const feature* o) { return features_s_get_typed( o->o_type )->func != NULL; }
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline bl_t #<sc_t>_r_defines_#<sc_t>( const sr_s* o ) ", indent, o->group->name.sc, o->name.sc );
        if( o->strict || o->default_body || o->default_name.size > 0 )
        {
            bcore_sink_a_push_fa( sink, "{ return true; }", o->name.sc );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "{ return ( (#<sc_t>_s*)ch_spect_p( o->p, TYPEOF_#<sc_t>_s ) )->#<sc_t> != NULL; }", o->group->name.sc, o->group->name.sc, o->name.sc );
        }
    }

    if( o->default_name.size > 0 )
    {
        if( o->default_body && o->default_body->go_inline )
        {
            // ret_t feature_default( feature* o, arg_t arg1 );
            bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t> #<sc_t>_#<sc_t>(", indent, o->ret_type.sc, o->group->name.sc, o->default_name.sc );
            if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
            bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group->name.sc );
            bcore_plant_args_s_expand( &o->args, false, sink );
            bcore_sink_a_push_fa( sink, " ) #<sc_t>", o->default_body->code.sc );
        }
        else
        {
            // ret_t feature_default( feature* o, arg_t arg1 );
            bcore_sink_a_push_fa( sink, " \\\n#rn{ }  #<sc_t> #<sc_t>_#<sc_t>(", indent, o->ret_type.sc, o->group->name.sc, o->default_name.sc );
            if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
            bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group->name.sc );
            bcore_plant_args_s_expand( &o->args, false, sink );
            bcore_sink_a_push_fa( sink, " );" );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_feature_s_expand_definition( const bcore_plant_feature_s* o, sz_t indent, bcore_sink* sink )
{
    if( o->default_body && !o->default_body->go_inline )
    {
        // ret_t feature_default( feature* o, arg_t arg1 );
        bcore_sink_a_push_fa( sink, "\n" );
        bcore_sink_a_push_fa( sink, "\n#<sc_t> #<sc_t>_#<sc_t>(", o->ret_type.sc, o->group->name.sc, o->default_name.sc );
        if( !o->mutable ) bcore_sink_a_push_fa( sink, " const" );
        bcore_sink_a_push_fa( sink, " #<sc_t>* o", o->group->name.sc );
        bcore_plant_args_s_expand( &o->args, false, sink );
        bcore_sink_a_push_fa( sink, " )\n", o->default_body->code.sc );
        bcore_sink_a_push_fa( sink, "#<sc_t>", o->default_body->code.sc );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_feature_s_expand_init1( const bcore_plant_feature_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_FEATURE( #<sc_t> );\n", indent, o->global_name.sc );
    if( o->default_name.size > 0 )
    {
        bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_FFUNC( #<sc_t>, #<sc_t>_#<sc_t> );\n", indent, o->global_name.sc, o->group->name.sc, o->default_name.sc );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// func

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_plant_func_s_get_hash( const bcore_plant_func_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    hash = bcore_tp_fold_sc( hash, o->decl.sc );
    hash = bcore_tp_fold_tp( hash, o->type );
    if( o->body ) hash = bcore_tp_fold_tp( hash, bcore_plant_body_s_get_hash( o->body ) );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

/// stamp should be NULL func is not parsed inside a stamp
static void bcore_plant_func_s_parse( bcore_plant_func_s* o, bcore_plant_group_s* group, bcore_plant_stamp_s* stamp, bcore_source* source )
{
    BLM_INIT();

    /// global name of function
    st_s* type_name = BLM_CREATE( st_s );

    bcore_source_point_s_set( &o->source_point, source );

    st_s_push_sc( &o->decl, "func " );

    if( bcore_source_a_parse_bl_fa( source, " #?'^'" ) )
    {
        if( !stamp ) bcore_source_a_parse_err_fa( source, "'^' is only inside a stamp allowed." );
        st_s_copy( type_name, &stamp->trait_name );
        st_s_push_fa( &o->decl, "^" );
    }
    else
    {
        bcore_plant_group_s_parse_name( group, type_name, source );

        if( stamp && st_s_equal_st( type_name, &stamp->trait_name ) )
        {
            st_s_push_fa( &o->decl, "^" );
        }
        else
        {
            st_s_push_fa( &o->decl, "#<sc_t>", type_name->sc );
        }
    }

    bcore_source_a_parse_fa( source, " :" );
    st_s_push_sc( &o->decl, ":" );

    bcore_source_a_parse_fa( source, " #name", &o->name );
    if( o->name.size == 0 ) bcore_source_a_parse_err_fa( source, "Function name expected." );
    st_s_push_sc( &o->decl, o->name.sc );

    st_s_push_fa( type_name, "_#<sc_t>", o->name.sc );
    o->type = typeof( type_name->sc );

    if( bcore_source_a_parse_bl_fa( source, " #=?'='" ) )
    {
        o->body = bcore_plant_body_s_create();
        o->body->group = group;
        bcore_plant_body_s_parse( o->body, source );
    }

    bcore_source_a_parse_fa( source, " ; " );
    st_s_push_sc( &o->decl, ";" );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// funcs

//----------------------------------------------------------------------------------------------------------------------

static bl_t bcore_plant_funcs_s_exists( bcore_plant_funcs_s* o, tp_t type )
{
    for( sz_t i = 0; i < o->size; i++ ) if( o->data[ i ]->type == type ) return true;
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

/// returns -1 if not found
static sz_t bcore_plant_funcs_s_get_index( bcore_plant_funcs_s* o, tp_t type )
{
    for( sz_t i = 0; i < o->size; i++ ) if( o->data[ i ]->type == type ) return i;
    return -1;
}

//----------------------------------------------------------------------------------------------------------------------

/// replacing by removing old function and appending new function keeping order
static void bcore_plant_funcs_s_replace_d( bcore_plant_funcs_s* o, sz_t idx, bcore_plant_func_s* func )
{
    ASSERT( idx >= 0 && idx < o->size );
    bcore_plant_func_s_detach( &o->data[ idx ] );
    for( sz_t i = idx + 1; i < o->size; i++ ) o->data[ i - 1 ] = o->data[ i ];
    o->data[ o->size - 1 ] = func;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// stamp

//----------------------------------------------------------------------------------------------------------------------

static sc_t bcore_plant_stamp_s_get_global_name_sc( const bcore_plant_stamp_s* o )
{
    return o->name.sc;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_plant_stamp_s_get_hash( const bcore_plant_stamp_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    hash = bcore_tp_fold_sc( hash, o->self_source->sc );
    for( sz_t i = 0; i < o->funcs.size; i++ )
    {
        hash = bcore_tp_fold_tp( hash, bcore_plant_func_s_get_hash( o->funcs.data[ i ] ) );
    }
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

/// resolve special characters in a string
static void bcore_plant_stamp_s_resolve_chars( bcore_plant_stamp_s* o, st_s* string )
{
    st_s* buf = st_s_create();
    for( sz_t i = 0; i < string->size; i++ )
    {
        char c = string->data[ i ];
        switch( c )
        {
            case '@':
            {
                st_s_push_sc( buf, o->name.sc );
                char next = string->data[ i + 1 ];
                if( ( next >= 'A' && next <= 'Z' ) || ( next >= 'a' && next <= 'z' )|| ( next >= '0' && next <= '9' ) || ( next == '_' ) )
                {
                    st_s_push_char( buf, '_' );
                }
            }
            break;

            default:
            {
                st_s_push_char( buf, c );
            }
            break;
        }
    }
    st_s_copy( string, buf );
    st_s_discard( buf );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_stamp_s_extend( bcore_plant_stamp_s* o, bcore_plant_group_s* group, bcore_source* source, bl_t verbatim )
{
    BLM_INIT();
    ASSERT( o->self_source );

    bcore_source_a_parse_fa( source, " {" );

    if( o->self_source->sc[ o->self_source->size - 1 ] == '}' )
    {
        o->self_source->size--;
    }
    else
    {
        st_s_push_fa( o->self_source, "{" );
    }

    while( !bcore_source_a_eos( source ) && !bcore_source_a_parse_bl_fa( source, " #?'}'" ) )
    {
        if( bcore_source_a_parse_bl_fa( source, " #?w'func'" ) )
        {
            bcore_plant_func_s* func = bcore_plant_func_s_create();
            bcore_plant_func_s_parse( func, group, o, source );

            sz_t idx = bcore_plant_funcs_s_get_index( &o->funcs, func->type );

            if( idx >= 0 )
            {
                bcore_plant_func_s* prex_func = o->funcs.data[ idx ];
                if( prex_func->overloadable )
                {
                    bcore_plant_funcs_s_replace_d( &o->funcs, idx, func );
                    st_s_replace_sc_sc( o->self_source, prex_func->decl.sc, "" );
                    st_s_push_st( o->self_source, &func->decl );
                }
                else
                {
                    bcore_source_a_parse_err_fa( source, "Function '#<sc_t>' has already been defined and is not overloadable.", func->name.sc );
                }
            }
            else
            {
                bcore_array_a_push( ( bcore_array* )&o->funcs, sr_asd( func ) );
                st_s_push_st( o->self_source, &func->decl );
            }
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
                        bcore_plant_group_s_parse_name_recursive( group, name, source );
                        st_s_push_st( o->self_source, name );
                        st_s_discard( name );
                    }
                    break;

                    case ';':
                    {
                        st_s_push_char( o->self_source, c );
                        exit = true;
                    }
                    break;

                    default:
                    {
                        st_s_push_char( o->self_source, c );
                    }
                    break;
                }
            }
        }
    }
    bcore_source_a_parse_fa( source, " ; " );

    // apply all functions of group and parents, which are not yet defined in stamp
    if( !verbatim )
    {
        for( bcore_plant_group_s* fgroup = group; fgroup != NULL; fgroup = fgroup->group )
        {
            for( sz_t i = 0; i < fgroup->funcs.size; i++ )
            {
                bcore_plant_func_s* func = fgroup->funcs.data[ i ];
                if( !bcore_plant_funcs_s_exists( &o->funcs, func->type ) )
                {
                    st_s_push_st( o->self_source, &func->decl );
                    bcore_array_a_push( ( bcore_array* )&o->funcs, sr_awc( func ) );
                }
            }
        }
    }

    st_s_push_sc( o->self_source, "}" );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_stamp_s_parse( bcore_plant_stamp_s* o, bcore_plant_group_s* group, bcore_source* source )
{
    BLM_INIT();

    bl_t verbatim = bcore_source_a_parse_bl_fa( source, " #?w'verbatim'" );

    o->self_source = st_s_create();

    st_s* stamp_name  = BLM_CREATE( st_s );
    st_s* trait_name  = BLM_CREATE( st_s );

    bcore_source_point_s_set( &o->source_point, source );

    o->group = group;

    bcore_plant_group_s_parse_name( group, stamp_name, source );

    if( stamp_name->size >= 2 && sc_t_equ( stamp_name->sc + stamp_name->size - 2, "_s" ) )
    {
        WRN_fa( "Stamp '#<sc_t>' ends in '_s'. In beth, this ending identifies a structure. Using it as stamp name can have side effects. Is this intended?", stamp_name->sc );
    }

    st_s_push_fa( stamp_name, "_s" );

    bcore_source_a_parse_fa( source, " = " );

    if( bcore_source_a_parse_bl_fa( source, " #?w'extending'" ) )
    {
        st_s* templ_name = BLM_CREATE( st_s );
        bcore_plant_group_s_parse_name( group, templ_name, source );
        st_s_push_fa( templ_name, "_s" );
        const bcore_plant* item = bcore_plant_compiler_s_item_get( plant_compiler_g, typeof( templ_name->sc ) );
        if( !item ) bcore_source_a_parse_err_fa( source, "Template #<sc_t> not found.", templ_name->sc );
        if( *(aware_t*)item != TYPEOF_bcore_plant_stamp_s ) bcore_source_a_parse_err_fa( source, "Template #<sc_t> is no stamp.", templ_name->sc );
        bcore_plant_stamp_s_copy( o, ( bcore_plant_stamp_s* )item );
    }
    else if( !verbatim && group->extending )
    {
        bcore_plant_stamp_s_copy( o, group->extending );
    }
    else
    {
        st_s_push_fa( o->self_source, "@ =" );

        if( bcore_source_a_parse_bl_fa( source, " #?w'aware'" ) ) st_s_push_sc( o->self_source, "aware " );

        if( bcore_source_a_parse_bl_fa( source, " #?':'" ) )
        {
            st_s_copy( trait_name, &group->name );
        }
        else
        {
            bcore_plant_group_s_parse_name( o->group, trait_name, source );
            if( trait_name->size == 0 ) bcore_source_a_parse_err_fa( source, "Trait name expected." );
        }

        st_s_copy( &o->trait_name, trait_name );
        st_s_push_st( o->self_source, trait_name );
    }

    st_s_copy( &o->name, stamp_name );

    bcore_plant_stamp_s_extend( o, group, source, verbatim );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_stamp_s_finalize( bcore_plant_stamp_s* o )
{
    // resolve special characters in self string and function bodies
    bcore_plant_stamp_s_resolve_chars( o, o->self_source );
    for( sz_t i = 0; i < o->funcs.size; i++ )
    {
        bcore_plant_func_s* func = o->funcs.data[ i ];
        if( func->body ) bcore_plant_stamp_s_resolve_chars( o, &func->body->code );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_stamp_s_make_funcs_overloadable( bcore_plant_stamp_s* o )
{
    for( sz_t i = 0; i < o->funcs.size; i++ )
    {
        bcore_plant_func_s* func = o->funcs.data[ i ];
        func->overloadable = true;
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_stamp_s_expand_forward( const bcore_plant_stamp_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_stamp_s_expand_indef_declaration( const bcore_plant_stamp_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  BETH_EXPAND_ITEM_#<sc_t>", indent, o->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_stamp_s_expand_declaration( const bcore_plant_stamp_s* o, sz_t indent, bcore_sink* sink )
{
    BLM_INIT();
    bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, o->name.sc, typeof( o->name.sc ) );

    bcore_sink_a_push_fa( sink, "#rn{ }##define BETH_EXPAND_ITEM_#<sc_t>", indent, o->name.sc, o->name.sc );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  BCORE_DECLARE_OBJECT( #<sc_t> )", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }    ", indent );

    bcore_self_s* self = BLM_A_PUSH
    (
        bcore_self_s_parse_source
        (
            BLM_A_PUSH( (bcore_source*)bcore_source_string_s_create_from_string( o->self_source ) ),
            0,
            o->group->name.sc,
            false
        )
    );

    bcore_self_s_struct_body_to_sink_single_line( self, sink );
    bcore_sink_a_push_fa( sink, ";" );

    for( sz_t i = 0; i < o->funcs.size; i++ )
    {
        bcore_plant_func_s* func = o->funcs.data[ i ];
        if( bcore_plant_compiler_s_item_exists( plant_compiler_g, func->type ) )
        {
            const bcore_plant* item = bcore_plant_compiler_s_item_get( plant_compiler_g, func->type );
            if( *(aware_t*)item == TYPEOF_bcore_plant_feature_s )
            {
                const bcore_plant_feature_s* feature = ( bcore_plant_feature_s* )item;
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  ", indent );

                bl_t go_inline = func->body && func->body->go_inline;

                if( go_inline ) bcore_sink_a_push_fa( sink, "static inline " );

                bcore_sink_a_push_fa( sink, "#<sc_t> #<sc_t>_#<sc_t>( ", feature->ret_type.sc, o->name.sc, func->name.sc );
                bcore_sink_a_push_fa( sink, "#<sc_t>", feature->mutable ? "" : "const " );
                bcore_sink_a_push_fa( sink, "#<sc_t>* o", o->name.sc );
                bcore_plant_args_s_expand( &feature->args, false, sink );
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
            else if( *(aware_t*)item == TYPEOF_bcore_plant_signature_s )
            {
                const bcore_plant_signature_s* signature = ( bcore_plant_signature_s* )item;
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  ", indent );

                bl_t go_inline = func->body && func->body->go_inline;

                if( go_inline ) bcore_sink_a_push_fa( sink, "static inline " );

                bcore_sink_a_push_fa( sink, "#<sc_t> #<sc_t>_#<sc_t>( ", signature->ret_type.sc, o->name.sc, func->name.sc );

                if( signature->arg_o )
                {
                    bcore_sink_a_push_fa( sink, "#<sc_t>", ( signature->arg_o == TYPEOF_mutable ) ? "" : "const " );
                    bcore_sink_a_push_fa( sink, "#<sc_t>* o", o->name.sc );
                    bcore_plant_args_s_expand( &signature->args, false, sink );
                }
                else
                {
                    if( signature->args.size > 0 )
                    {
                        bcore_plant_args_s_expand( &signature->args, true, sink );
                    }
                    else
                    {
                        bcore_sink_a_push_fa( sink, "void" );
                    }
                }

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
            else
            {
                bcore_source_point_s_parse_err_fa( &func->source_point, "Plant Compiler:\nStamp #<sc_t>: Could not resolve function #<sc_t> #<sc_t>", o->name.sc, ifnameof( func->type ), func->name.sc );
            }
        }
        else
        {
            bcore_source_point_s_parse_err_fa( &func->source_point, "Plant Compiler:\nStamp #<sc_t>: Could not resolve function #<sc_t> #<sc_t>", o->name.sc, ifnameof( func->type ), func->name.sc );
        }
    }

    // expand array
    if( self->trait == TYPEOF_bcore_array )
    {
        sz_t items = bcore_self_s_items_size( self );
        const bcore_self_item_s* array_item = NULL;
        for( sz_t i = 0; i < items; i++ )
        {
            const bcore_self_item_s* self_item = bcore_self_s_get_item( self, i );
            if( bcore_flect_caps_is_array( self_item->caps ) )
            {
                array_item = self_item;
                break;
            }
        }

        if( !array_item )
        {
            bcore_source_point_s_parse_err_fa( &o->source_point, "Expanding object #<sc_t>: Object is of trait array but contains no array.", o->name.sc );
        }

        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_set_space( #<sc_t>* o, sz_t size ) { bcore_array_t_set_space( TYPEOF_#<sc_t>, ( bcore_array* )o, size ); }", indent, o->name.sc, o->name.sc, o->name.sc );
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_set_size( #<sc_t>* o, sz_t size ) { bcore_array_t_set_size( TYPEOF_#<sc_t>, ( bcore_array* )o, size ); }", indent, o->name.sc, o->name.sc, o->name.sc );
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_clear( #<sc_t>* o ) { bcore_array_t_set_space( TYPEOF_#<sc_t>, ( bcore_array* )o, 0 ); }", indent, o->name.sc, o->name.sc, o->name.sc );

        sc_t  sc_name = ifnameof( array_item->name );
        st_s* st_last = st_s_create_fa( "o->#<sc_t>#<sc_t>data[ o->#<sc_t>#<sc_t>size - 1 ]", sc_name, sc_name[ 0 ] ? "_" : "", sc_name, sc_name[ 0 ] ? "_" : ""  );
        sc_t  sc_last = st_last->sc;
        if( array_item->type != 0 && nameof( array_item->type ) != NULL )
        {
            sc_t  sc_type = ifnameof( array_item->type );
            if( array_item->caps == BCORE_CAPS_ARRAY_DYN_LINK_AWARE )
            {
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push_c( #<sc_t>* o, const #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_awc( v ) ); return #<sc_t>; }", indent, sc_type, o->name.sc, o->name.sc, sc_type, o->name.sc, sc_last );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push_d( #<sc_t>* o,       #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_asd( v ) ); return #<sc_t>; }", indent, sc_type, o->name.sc, o->name.sc, sc_type, o->name.sc, sc_last );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push_t( #<sc_t>* o, tp_t t )", indent, sc_type, o->name.sc, o->name.sc );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  {", indent );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      bcore_trait_assert_satisfied_type( TYPEOF_#<sc_t>, t );",                    indent, sc_type );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_t_create( t ) );", indent, o->name.sc );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      return #<sc_t>;", indent, sc_last );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  }", indent );
            }
            else if( array_item->caps == BCORE_CAPS_ARRAY_DYN_SOLID_STATIC )
            {
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push_c( #<sc_t>* o, const #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_twc( TYPEOF_#<sc_t>, v ) ); return &#<sc_t>; }", indent, sc_type, o->name.sc, o->name.sc, sc_type, o->name.sc, sc_type, sc_last );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push_d( #<sc_t>* o,       #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_tsd( TYPEOF_#<sc_t>, v ) ); return &#<sc_t>; }", indent, sc_type, o->name.sc, o->name.sc, sc_type, o->name.sc, sc_type, sc_last );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push( #<sc_t>* o )", indent, sc_type, o->name.sc, o->name.sc );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  {", indent );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_null() );",   indent, o->name.sc );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      return &#<sc_t>;", indent, sc_last );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  }", indent );
            }
            else if( array_item->caps == BCORE_CAPS_ARRAY_DYN_LINK_STATIC )
            {
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push_c( #<sc_t>* o, const #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_twc( TYPEOF_#<sc_t>, v ) ); return #<sc_t>; }", indent, sc_type, o->name.sc, o->name.sc, sc_type, o->name.sc, sc_type, sc_last );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push_d( #<sc_t>* o,       #<sc_t>* v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_tsd( TYPEOF_#<sc_t>, v ) ); return #<sc_t>; }", indent, sc_type, o->name.sc, o->name.sc, sc_type, o->name.sc, sc_type, sc_last );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline #<sc_t>* #<sc_t>_push( #<sc_t>* o )", indent, sc_type, o->name.sc, o->name.sc );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  {", indent );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_t_create( TYPEOF_#<sc_t> ) );", indent, o->name.sc, sc_type );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }      return #<sc_t>;", indent, sc_last );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  }", indent );
            }
        }
        else
        {
            if( bcore_flect_caps_is_aware( array_item->caps ) )
            {
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_push_c( #<sc_t>* o, vc_t v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_awc( v ) ); }", indent, o->name.sc, o->name.sc, o->name.sc );
                bcore_sink_a_push_fa( sink, " \\\n#rn{ }  static inline void #<sc_t>_push_d( #<sc_t>* o, vd_t v ) { bcore_array_t_push( TYPEOF_#<sc_t>, ( bcore_array* )o, sr_asd( v ) ); }", indent, o->name.sc, o->name.sc, o->name.sc );
            }
        }
        st_s_detach( &st_last );
    }

    bcore_sink_a_push_fa( sink, "\n" );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_stamp_s_expand_definition( const bcore_plant_stamp_s* o, sz_t indent, bcore_sink* sink )
{
    BLM_INIT();
    st_s* embedded_string = BLM_A_PUSH( create_embedded_string( o->self_source ) );

    // 4095 is the C99-limit for string literals
    if( embedded_string->size > 4095 )
    {
        bcore_source_point_s_parse_err_fa
        (
            &o->source_point,
            "Precoder reflection embedding failed.\n"
            "The embedded code would require a string literal larger than 4095 characters.\n"
            "This exceeds the limit defined in C99.\n"
        );
    }

    sz_t idx = st_s_find_char( embedded_string, 0, -1, '=' );
    sc_t self_def = "";
    if( idx < embedded_string->size )
    {
        self_def = embedded_string->sc + idx + 1;
    }

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_DEFINE_OBJECT_INST_P( #<sc_t> )\n", indent, o->name.sc );

    st_s* multiline_string = create_structured_multiline_string( self_def, indent );
    bcore_sink_a_push_fa( sink, "#<sc_t>;\n", multiline_string->sc );

    for( sz_t i = 0; i < o->funcs.size; i++ )
    {
        bcore_plant_func_s* func = o->funcs.data[ i ];
        if( bcore_plant_compiler_s_item_exists( plant_compiler_g, func->type ) )
        {
            const bcore_plant* item = bcore_plant_compiler_s_item_get( plant_compiler_g, func->type );
            if( *(aware_t*)item == TYPEOF_bcore_plant_feature_s )
            {
                const bcore_plant_feature_s* feature = ( bcore_plant_feature_s* )item;

                if( func->body && !func->body->go_inline )
                {
                    bcore_sink_a_push_fa( sink, "\n" );
                    bcore_sink_a_push_fa( sink, "#rn{ }#<sc_t> #<sc_t>_#<sc_t>( ", indent, feature->ret_type.sc, o->name.sc, func->name.sc );
                    bcore_sink_a_push_fa( sink, "#<sc_t>", feature->mutable ? "" : "const " );
                    bcore_sink_a_push_fa( sink, "#<sc_t>* o", o->name.sc );
                    bcore_plant_args_s_expand( &feature->args, false, sink );
                    bcore_sink_a_push_fa( sink, " )\n" );
                    bcore_sink_a_push_fa( sink, "#<sc_t>", func->body->code.sc );
                    bcore_sink_a_push_fa( sink, "\n" );
                }
            }
            else if( *(aware_t*)item == TYPEOF_bcore_plant_signature_s )
            {
                const bcore_plant_signature_s* signature = ( bcore_plant_signature_s* )item;

                if( func->body && !func->body->go_inline )
                {
                    bcore_sink_a_push_fa( sink, "\n" );
                    bcore_sink_a_push_fa( sink, "#rn{ }#<sc_t> #<sc_t>_#<sc_t>( ", indent, signature->ret_type.sc, o->name.sc, func->name.sc );
                    if( signature->arg_o )
                    {
                        bcore_sink_a_push_fa( sink, "#<sc_t>", ( signature->arg_o == TYPEOF_mutable ) ? "" : "const " );
                        bcore_sink_a_push_fa( sink, "#<sc_t>* o", o->name.sc );
                        bcore_plant_args_s_expand( &signature->args, false, sink );
                    }
                    else
                    {
                        if( signature->args.size > 0 )
                        {
                            bcore_plant_args_s_expand( &signature->args, true, sink );
                        }
                        else
                        {
                            bcore_sink_a_push_fa( sink, "void" );
                        }
                    }

                    bcore_sink_a_push_fa( sink, " )\n" );
                    bcore_sink_a_push_fa( sink, "#<sc_t>", func->body->code.sc );
                    bcore_sink_a_push_fa( sink, "\n" );
                }
            }
            else
            {
                bcore_source_point_s_parse_err_fa( &func->source_point, "Plant Compiler:\nStamp #<sc_t>: Could not resolve function #<sc_t> #<sc_t>", o->name.sc, ifnameof( func->type ), func->name.sc );
            }
        }
        else
        {
            bcore_source_point_s_parse_err_fa( &func->source_point, "Plant Compiler:\nStamp #<sc_t>: Could not resolve function #<sc_t> #<sc_t>", o->name.sc, ifnameof( func->type ), func->name.sc );
        }
    }

    st_s_discard( multiline_string );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_stamp_s_expand_init1( const bcore_plant_stamp_s* o, sz_t indent, bcore_sink* sink )
{
    for( sz_t i = 0; i < o->funcs.size; i++ )
    {
        bcore_plant_func_s* func = o->funcs.data[ i ];
        if( bcore_plant_compiler_s_item_exists( plant_compiler_g, func->type ) )
        {
            const bcore_plant* item = bcore_plant_compiler_s_item_get( plant_compiler_g, func->type );
            if( *(aware_t*)item == TYPEOF_bcore_plant_feature_s )
            {
                const bcore_plant_feature_s* feature = ( bcore_plant_feature_s* )item;
                bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_FFUNC( #<sc_t>, #<sc_t>_#<sc_t> );\n", indent, feature->global_name.sc, o->name.sc, func->name.sc );
            }
            else if( *(aware_t*)item == TYPEOF_bcore_plant_signature_s )
            {
                const bcore_plant_signature_s* signature = ( bcore_plant_signature_s* )item;
                bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_FFUNC( #<sc_t>, #<sc_t>_#<sc_t> );\n", indent, signature->global_name.sc, o->name.sc, func->name.sc );
            }
            else
            {
                bcore_source_point_s_parse_err_fa( &func->source_point, "Plant Compiler:\nStamp #<sc_t>: Could not resolve function #<sc_t> #<sc_t>", o->name.sc, ifnameof( func->type ), func->name.sc );
            }
        }
        else
        {
            bcore_source_point_s_parse_err_fa( &func->source_point, "Plant Compiler:\nStamp #<sc_t>: Could not resolve function #<sc_t> #<sc_t>", o->name.sc, ifnameof( func->type ), func->name.sc );
        }
    }
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_OBJECT( #<sc_t> );\n", indent, o->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// name

//----------------------------------------------------------------------------------------------------------------------

static sc_t bcore_plant_name_s_get_global_name_sc( const bcore_plant_name_s* o )
{
    return o->name.sc;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_plant_name_s_get_hash( const bcore_plant_name_s* o )
{
    tp_t hash = bcore_tp_init();
    hash = bcore_tp_fold_sc( hash, o->name.sc );
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_name_s_parse( bcore_plant_name_s* o, bcore_source* source )
{
    bcore_source_point_s_set( &o->source_point, source );

    if( bcore_source_a_parse_bl_fa( source, " #?':'" ) )
    {
        st_s* name = st_s_create();
        bcore_source_a_parse_fa( source, " #name", name );
        st_s_push_fa( &o->name, "#<sc_t>#<sc_t>#<sc_t>", o->group->name.sc, name->sc[ 0 ] ? "_" : "", name->sc );
        st_s_discard( name );
    }
    else
    {
        bcore_source_a_parse_fa( source, " #name", &o->name );
    }
    if( o->name.size == 0 ) bcore_source_a_parse_err_fa( source, "Feature: Name missing." );
    bcore_source_a_parse_fa( source, " ; " );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_name_s_expand_declaration( const bcore_plant_name_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, o->name.sc, typeof( o->name.sc ) );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_name_s_expand_init1( const bcore_plant_name_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_NAME( #<sc_t> );\n", indent, o->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// group

//----------------------------------------------------------------------------------------------------------------------

static sc_t bcore_plant_group_s_get_global_name_sc( const bcore_plant_group_s* o )
{
    return o->name.sc;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_plant_group_s_get_hash( const bcore_plant_group_s* o )
{
    return o->hash;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_group_s_parse_name_recursive( bcore_plant_group_s* o, st_s* name, bcore_source* source )
{
    if( bcore_source_a_parse_bl_fa( source, "#?':'" ) )
    {
        if( o->group )
        {
            bcore_plant_group_s_parse_name_recursive( o->group, name, source );
        }
        else
        {
            bcore_source_a_parse_err_fa( source, "Too many colons: Namespace depth exceeded." );
        }
    }
    else
    {
        st_s_copy( name, &o->name );
        st_s* s = st_s_create();
        bcore_source_a_parse_fa( source, " #name", s );
        if( s->size > 0 )
        {
            st_s_push_fa( name, "_#<sc_t>", s->sc );
        }
        st_s_discard( s );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_group_s_parse_name( bcore_plant_group_s* o, st_s* name, bcore_source* source )
{
    if( bcore_source_a_parse_bl_fa( source, " #?':'" ) )
    {
        bcore_plant_group_s_parse_name_recursive( o, name, source );
    }
    else
    {
        bcore_source_a_parse_fa( source, " #name", name );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_group_s_parse( bcore_plant_group_s* o, bcore_source* source )
{
    BLM_INIT();
    sc_t precode_termination = NULL;

    bcore_source_point_s_set( &o->source_point, source );

    o->hash = typeof( o->name.sc );

    if( o->group ) // this group is nested in another group, the group body is enclosed in { ... }
    {
        bcore_source_a_parse_fa( source, " {" );
        precode_termination = " #?'}'";
    }
    else // this group is root, the group body is enclosed in an non-parsable section of c source
    {
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
                bcore_source_a_parse_fa( source, " PLANT_SECTION " );
                precode_termination = " #?'#endif'";
                break;
            }

            char c = bcore_source_a_get_u0( source );
            if( c != ' ' && c != '\t' && c != '\n' ) bcore_source_a_parse_err_fa( source, "Opening c-style comment '/*' or '#<sc_t>' expected.", "#ifdef PLANT_SECTION" );
        }
    }

    bl_t extend_stump = false;

    while( !bcore_source_a_parse_bl_fa( source, precode_termination ) )
    {
        if( bcore_source_a_eos( source ) )  bcore_source_a_parse_err_fa( source, "Closing c-style comment '*/' expected." );

        bcore_plant* item = NULL;

        if( bcore_source_a_parse_bl_fa( source, " #?w'stamp' " ) )
        {
            bcore_plant_stamp_s* stamp = bcore_plant_stamp_s_create();
            bcore_plant_stamp_s_parse( stamp, o, source );
            bcore_plant_compiler_s_item_register( plant_compiler_g, ( bcore_plant* )stamp, source );
            item = ( bcore_plant* )stamp;
        }

        /// stumps are 'hidden' stamps (not expanded into final code), but they can be used as template
        else if( bcore_source_a_parse_bl_fa( source, " #?w'stump' " ) )
        {
            bcore_plant_stamp_s* stump = bcore_plant_stamp_s_create();
            bcore_plant_stamp_s_parse( stump, o, source );
            bcore_plant_stamp_s_make_funcs_overloadable( stump );
            bcore_plant_compiler_s_item_register( plant_compiler_g, ( bcore_plant* )stump, source );
            bcore_plant_compiler_s_life_a_push( plant_compiler_g, stump );
            if( extend_stump )
            {
                o->extending = stump;
                extend_stump = false;
            }
            item = NULL;
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'signature' " ) )
        {
            bcore_plant_signature_s* signature = bcore_plant_signature_s_create();
            signature->group = o;
            bcore_plant_signature_s_parse( signature, source );
            bcore_source_a_parse_fa( source, " ; " );
            bcore_plant_compiler_s_item_register( plant_compiler_g, ( bcore_plant* )signature, source );
            item = ( bcore_plant* )signature;
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'body' " ) )
        {
            bcore_plant_body_s* body = bcore_plant_body_s_create();
            body->group = o;
            bcore_plant_body_s_parse( body, source );
            bcore_source_a_parse_fa( source, " ; " );
            bcore_plant_compiler_s_item_register( plant_compiler_g, ( bcore_plant* )body, source );
            item = ( bcore_plant* )body;
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'feature' " ) )
        {
            bcore_plant_feature_s* feature = bcore_plant_feature_s_create();
            feature->group = o;
            bcore_plant_feature_s_parse( feature, source );
            bcore_plant_compiler_s_item_register( plant_compiler_g, ( bcore_plant* )feature, source );
            o->has_features = true;
            if( feature->flag_a ) o->is_aware = true;
            item = ( bcore_plant* )feature;
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'name' " ) )
        {
            bcore_plant_name_s* name = bcore_plant_name_s_create();
            name->group = o;
            bcore_plant_name_s_parse( name, source );
            item = ( bcore_plant* )name;
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'expandable'" ) )
        {
            bcore_source_a_parse_fa( source, " = " );
            if(      bcore_source_a_parse_bl_fa( source, " #?'true'"  ) ) o->expandable = true;
            else if( bcore_source_a_parse_bl_fa( source, " #?'false'" ) ) o->expandable = false;
            else
            {
                bcore_source_a_parse_err_fa( source, "'true' or 'false' expected." );
            }
            bcore_source_a_parse_fa( source, " ; " );
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'extending'" ) )
        {
            o->extending = NULL;
            if( bcore_source_a_parse_bl_fa( source, " #=?';'" ) )
            {
                // just reset o->extending - nothing else
            }
            else if( bcore_source_a_parse_bl_fa( source, " #=?w'stump'" ) )
            {
                extend_stump = true;
            }
            else
            {
                st_s* templ_name = BLM_CREATE( st_s );
                bcore_plant_group_s_parse_name( o, templ_name, source );
                st_s_push_fa( templ_name, "_s" );
                const bcore_plant* item = bcore_plant_compiler_s_item_get( plant_compiler_g, typeof( templ_name->sc ) );
                if( !item ) bcore_source_a_parse_err_fa( source, "Template #<sc_t> not found.", templ_name->sc );
                if( *(aware_t*)item != TYPEOF_bcore_plant_stamp_s ) bcore_source_a_parse_err_fa( source, "Template #<sc_t> is no stamp.", templ_name->sc );
                o->extending = ( bcore_plant_stamp_s* )item;
                bcore_source_a_parse_fa( source, " ;" );
            }
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'func'" ) )
        {
            bcore_plant_func_s* func = bcore_plant_func_s_create();
            bcore_plant_func_s_parse( func, o, NULL, source );
            func->overloadable = true;
            o->hash = bcore_tp_fold_tp( o->hash, bcore_plant_func_s_get_hash( func ) );

            if( bcore_plant_funcs_s_exists( &o->funcs, func->type ) )
            {
                bcore_plant_funcs_s_replace_d( &o->funcs, bcore_plant_funcs_s_get_index( &o->funcs, func->type ), func );
            }
            else
            {
                bcore_array_a_push( ( bcore_array* )&o->funcs, sr_asd( func ) );
            }
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'group' " ) )
        {
            bcore_plant_group_s* group = bcore_plant_group_s_create();
            bcore_plant_source_s_push_group( o->source, group );
            group->group       = o;
            group->source      = o->source;
            group->extending   = o->extending;
            bcore_plant_group_s_parse_name( o, &group->name, source );
            bcore_source_a_parse_fa( source, " =", &group->name, &group->trait_name );

            // flags
            if( bcore_source_a_parse_bl_fa( source, " #?w'retrievable' " ) ) group->retrievable = true;

            bcore_plant_group_s_parse_name( o, &group->trait_name, source );
            if( group->trait_name.size == 0 ) st_s_copy( &group->trait_name, &o->name );
            bcore_plant_group_s_parse( group, source );
            bcore_source_a_parse_fa( source, " ; " );
            o->source->hash = bcore_tp_fold_tp( o->source->hash, group->hash );
            bcore_plant_compiler_s_group_register( plant_compiler_g, group, source );
            bcore_plant_nested_group_s* nested_group = bcore_plant_nested_group_s_create();
            nested_group->group = group;
            item = ( bcore_plant* )nested_group;
        }
        else if( bcore_source_a_parse_bl_fa( source, " #?w'set' " ) )
        {
            if( bcore_source_a_parse_bl_fa( source, " #?w'retrievable' " ) )
            {
                o->retrievable = true;
            }
            else
            {
                bcore_source_a_parse_err_fa( source, "Invalid flag." );
            }

            bcore_source_a_parse_fa( source, " ;" );
        }
        else
        {
            bcore_source_a_parse_err_fa( source, "Plant: syntax error." );
        }

        // hash group parameters
        o->hash = bcore_tp_fold_tp( o->hash, o->retrievable ? 1 : 0 );

        if( item )
        {
            o->hash = bcore_tp_fold_tp( o->hash, bcore_plant_a_get_hash( item ) );
            bcore_array_a_push( ( bcore_array* )o, sr_asd( item ) );
        }
    }
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_group_s_finalize( bcore_plant_group_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_a_finalize( o->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_group_s_expand_forward( const bcore_plant_group_s* o, sz_t indent, bcore_sink* sink )
{
    if( !o->expandable ) return;
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o->name.sc );
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_a_expand_forward( o->data[ i ], indent, sink );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_group_s_expand_spect_declaration( const bcore_plant_group_s* o, sz_t indent, bcore_sink* sink )
{
    if( !o->expandable ) return;
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }BCORE_DECLARE_SPECT( #<sc_t> )", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }{", indent );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }    bcore_spect_header_s header;", indent );
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_a_expand_spect_declaration( o->data[ i ], indent + 4, sink );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }};", indent );

    bcore_sink_a_push_fa( sink, " \\\n#rn{ }static inline #<sc_t>* #<sc_t>_t_create( tp_t t ) { bcore_trait_assert_satisfied_type( TYPEOF_#<sc_t>, t ); return ( #<sc_t>* )bcore_inst_t_create( t ); }", indent, o->name.sc, o->name.sc, o->name.sc, o->name.sc );
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }static inline bl_t #<sc_t>_t_is_trait_of( tp_t t ) { return bcore_trait_is_of( t, TYPEOF_#<sc_t> ); }", indent, o->name.sc, o->name.sc );

    /// some extra functionality for aware types
    if( o->is_aware )
    {
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }BCORE_DECLARE_VIRTUAL_AWARE_OBJECT( #<sc_t> )", indent, o->name.sc );
        bcore_sink_a_push_fa( sink, " \\\n#rn{ }static inline bl_t #<sc_t>_a_is_trait_of( vc_t o ) { return bcore_trait_is_of( o ? *(aware_t*)o : 0, TYPEOF_#<sc_t> ); }", indent, o->name.sc, o->name.sc );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_group_s_expand_declaration( const bcore_plant_group_s* o, sz_t indent, bcore_sink* sink )
{
    if( !o->expandable ) return;
    BLM_INIT();
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }//#rn{-}\n", indent, sz_max( 0, 118 - indent ) );
    bcore_sink_a_push_fa( sink, "#rn{ }// group: #<sc_t>\n", indent, o->name.sc );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, o->name.sc, typeof( o->name.sc ) );

    {
        st_s* spect_name = BLM_A_PUSH( st_s_create_fa( "#<sc_t>_s", o->name.sc ) );
        bcore_sink_a_push_fa( sink, "#rn{ }##define TYPEOF_#<sc_t> #<tp_t>\n", indent, spect_name->sc, typeof( spect_name->sc ) );
    }

    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_a_expand_declaration( o->data[ i ], indent, sink );
    bcore_sink_a_push_fa( sink, "#rn{ }##define BETH_EXPAND_GROUP_#<sc_t>", indent, o->name.sc );

    bcore_plant_group_s_expand_forward( o, indent + 2, sink );

    if( o->has_features )
    {
        for( sz_t i = 0; i < o->size; i++ ) bcore_plant_a_expand_indef_typedef( o->data[ i ], indent, sink );
        bcore_plant_group_s_expand_spect_declaration( o, indent + 2, sink );
    }

    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_a_expand_indef_declaration( o->data[ i ], indent, sink );

    bcore_sink_a_push_fa( sink, "\n" );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_group_s_expand_spect_definition( const bcore_plant_group_s* o, sz_t indent, bcore_sink* sink )
{
    if( !o->expandable ) return;
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }BCORE_DEFINE_SPECT( #<sc_t>, #<sc_t> )\n", indent, o->trait_name.sc, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }\"{\"\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    \"bcore_spect_header_s header;\"\n", indent );
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_a_expand_spect_definition( o->data[ i ], indent + 4, sink );
    bcore_sink_a_push_fa( sink, "#rn{ }\"}\";\n", indent );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_group_s_expand_definition( const bcore_plant_group_s* o, sz_t indent, bcore_sink* sink )
{
    if( !o->expandable ) return;
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }//#rn{-}\n", indent, sz_max( 0, 118 - indent ) );
    bcore_sink_a_push_fa( sink, "#rn{ }// group: #<sc_t>\n", indent, o->name.sc );
    for( sz_t i = 0; i < o->size; i++ )
    {
        // non-features
        if( *(aware_t*)o->data[ i ] != TYPEOF_bcore_plant_feature_s )
        {
            bcore_plant_a_expand_definition( o->data[ i ], indent, sink );
        }
    }

    if( o->has_features )
    {
        bcore_plant_group_s_expand_spect_definition( o, indent, sink );
        for( sz_t i = 0; i < o->size; i++ )
        {
            // just-features
            if( *(aware_t*)o->data[ i ] == TYPEOF_bcore_plant_feature_s )
            {
                bcore_plant_a_expand_definition( o->data[ i ], indent, sink );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_group_s_expand_init1( const bcore_plant_group_s* o, sz_t indent, bcore_sink* sink )
{
    if( !o->expandable ) return;
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_a_expand_init1( o->data[ i ], indent, sink );

    if( o->has_features )
    {
        bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_SPECT( #<sc_t> );\n", indent, o->name.sc );
    }
    else
    {
        bcore_sink_a_push_fa( sink, "#rn{ }BCORE_REGISTER_TRAIT( #<sc_t>, #<sc_t> );\n", indent, o->name.sc, o->trait_name.sc );
    }

    if( o->retrievable )
    {
        for( sz_t i = 0; i < o->size; i++ )
        {
            if( *(aware_t*)o->data[ i ] == TYPEOF_bcore_plant_stamp_s )
            {
                const bcore_plant_stamp_s* stamp = ( bcore_plant_stamp_s* )o->data[ i ];
                bcore_sink_a_push_fa( sink, "#rn{ }bcore_inst_s_get_typed( TYPEOF_#<sc_t> );\n", indent, stamp->name.sc );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// nested_group

//----------------------------------------------------------------------------------------------------------------------

static tp_t bcore_plant_nested_group_s_get_hash( const bcore_plant_nested_group_s* o )
{
    return o->group ? o->group->hash : 0;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_nested_group_s_expand_forward( const bcore_plant_group_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }BCORE_FORWARD_OBJECT( #<sc_t> );", indent, o->group->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_nested_group_s_expand_indef_declaration( const bcore_plant_nested_group_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, " \\\n#rn{ }  BETH_EXPAND_GROUP_#<sc_t>", indent, o->group->name.sc );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// source

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_source_s_push_group( bcore_plant_source_s* o, bcore_plant_group_s* group )
{
    bcore_array_a_push( ( bcore_array* )o, sr_asd( group ) );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_source_s_parse( bcore_plant_source_s* o, bcore_source* source )
{
    while( !bcore_source_a_eos( source ) )
    {
        if( bcore_source_a_parse_bl_fa( source, "#?w'PLANT_GROUP'" ) )
        {
            bcore_plant_group_s* group = bcore_plant_group_s_create();
            bcore_plant_source_s_push_group( o, group );
            group->source = o;
            bcore_source_a_parse_fa( source, " ( #name, #name )", &group->name, &group->trait_name );
            if( group->trait_name.size == 0 ) st_s_copy_sc( &group->trait_name, "bcore_inst" );
            bcore_plant_group_s_parse( group, source );
            o->hash = bcore_tp_fold_tp( o->hash, group->hash );
            bcore_plant_compiler_s_group_register( plant_compiler_g, group, source );
        }
        else
        {
            bcore_source_a_get_u0( source );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_source_s_finalize( bcore_plant_source_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_group_s_finalize( o->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_source_s_expand_declaration( const bcore_plant_source_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    bcore_sink_a_push_fa( sink, "#rn{ }// source: #<sc_t>.h\n", indent, o->name.sc );
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_group_s_expand_declaration( o->data[ i ], indent, sink );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_source_s_expand_definition( const bcore_plant_source_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    bcore_sink_a_push_fa( sink, "#rn{ }// source: #<sc_t>.h\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"#<sc_t>.h\"\n", indent, o->name.sc );
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_group_s_expand_definition( o->data[ i ], indent, sink );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_source_s_expand_init1( const bcore_plant_source_s* o, sz_t indent, bcore_sink* sink )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_group_s_expand_init1( o->data[ i ], indent, sink );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// target

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_target_s_parse( bcore_plant_target_s* o, sc_t source_path )
{
    BLM_INIT();

    st_s* source_name        = BLM_A_PUSH( bcore_file_strip_extension( bcore_file_name( source_path ) ) );
    st_s* source_folder_path = BLM_A_PUSH( bcore_file_folder_path( source_path ) );
    st_s* source_path_n      = BLM_A_PUSH( st_s_create_fa( "#<sc_t>/#<sc_t>", source_folder_path->sc, source_name->sc ) );
    st_s* source_path_h      = BLM_A_PUSH( st_s_create_fa( "#<sc_t>.h", source_path_n->sc ) );
//    st_s* source_path_c      = BLM_A_PUSH( st_s_create_fa( "#<sc_t>.c", source_path_n->sc ) );

    bcore_plant_source_s* plant_source = bcore_plant_source_s_create();
    plant_source->target = o;
    st_s_copy_sc( &plant_source->name, source_name->sc );
    st_s_copy   ( &plant_source->path, source_path_n );

    plant_source->hash = bcore_tp_init();

    if( bcore_file_exists( source_path_h->sc ) )
    {
        bcore_plant_source_s_parse( plant_source, BLM_A_PUSH( bcore_file_open_source( source_path_h->sc ) ) );
    }

    // parsing *.c files is generally not helpful  (currently plant code can only reside in header files)
    // if( bcore_file_exists( source_path_c->sc ) ) bcore_plant_source_s_parse( plant_source, BLM_A_PUSH( bcore_file_open_source( source_path_c->sc ) ) );

    o->hash = bcore_tp_fold_tp( o->hash, plant_source->hash );
    bcore_array_a_push( ( bcore_array* )o, sr_asd( plant_source ) );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_target_s_finalize( bcore_plant_target_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_source_s_finalize( o->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_target_s_expand_license( const bcore_plant_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "/** This file was generated by the beth-plant-compiler.\n" );
    bcore_sink_a_push_fa( sink, " *\n" );
    bcore_sink_a_push_fa( sink, " *  Copyright and License:\n" );
    bcore_sink_a_push_fa( sink, " *    Generated code inherits the copyright and license of the beth-plant source code.\n" );
    bcore_sink_a_push_fa( sink, " *    The following files contain all source code related to this file:\n" );
    for( sz_t i = 0; i < o->size; i++ )
    {
        bcore_plant_source_s* source = o->data[ i ];
        bcore_sink_a_push_fa( sink, " *    #<sc_t>.h\n", source->name.sc );
    }
    bcore_sink_a_push_fa( sink, " *\n" );
    bcore_sink_a_push_fa( sink, " */\n" );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_target_s_expand_h( const bcore_plant_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_plant_target_s_expand_license( o, indent, sink );

    st_s* name_upper = st_s_clone( &o->name );
    st_s_set_uppercase( name_upper );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##ifndef #<sc_t>_H\n", indent, name_upper->sc );
    bcore_sink_a_push_fa( sink, "#rn{ }##define #<sc_t>_H\n", indent, name_upper->sc );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_control.h\"\n", indent );

    bcore_sink_a_push_fa( sink, "\n" );

    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_source_s_expand_declaration( o->data[ i ], indent, sink );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }/*#rn{*}*/\n", indent, sz_max( 0, 116 - indent ) );
    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }vd_t #<sc_t>_signal_handler( const bcore_signal_s* o );\n", indent, o->name.sc );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##endif // #<sc_t>_H\n", indent, name_upper->sc );

    st_s_discard( name_upper );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_target_s_expand_init1( const bcore_plant_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "#rn{ }// Comment or remove line below to rebuild this target.\n", indent, o->name.sc, o->hash );
    bcore_sink_a_push_fa( sink, "#rn{ }bcore_const_x_set_d( typeof( \"#<sc_t>_hash\" ), sr_tp( #<tp_t> ) );\n", indent, o->name.sc, o->hash );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_target_s_expand_c( const bcore_plant_target_s* o, sz_t indent, bcore_sink* sink )
{
    bcore_plant_target_s_expand_license( o, indent, sink );

    bcore_sink_a_push_fa( sink, "\n" );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"#<sc_t>.h\"\n", indent, o->name.sc );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_spect.h\"\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_spect_inst.h\"\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_sr.h\"\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }##include \"bcore_const_manager.h\"\n", indent );

    /// definition section
    bcore_sink_a_push_fa( sink, "\n" );
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_source_s_expand_definition( o->data[ i ], indent, sink );

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
    bcore_plant_target_s_expand_init1( o, indent + 12, sink );
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_source_s_expand_init1( o->data[ i ], indent + 12, sink );
    bcore_sink_a_push_fa( sink, "#rn{ }        }\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }        break;\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }        default: break;\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    }\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }    return NULL;\n", indent );
    bcore_sink_a_push_fa( sink, "#rn{ }}\n", indent );
}

//----------------------------------------------------------------------------------------------------------------------

/// returns true if a file was modified
static bl_t bcore_plant_target_s_expand( bcore_plant_target_s* o )
{
    BLM_INIT();
    bl_t modified = true;

    tp_t key = typeof( ( ( st_s* )BLM_A_PUSH( st_s_create_fa( "#<sc_t>_hash", o->name.sc ) ) )->sc );
    if( bcore_const_exists( key ) )
    {
        modified = ( *( tp_t* )bcore_const_get_o( key ) != o->hash );
    }
    else
    {
        bcore_msg_fa( "Key for target '#<sc_t>' does not exist. Is signal handler of '#<sc_t>.h' integrated?\n", o->name.sc, o->name.sc );
    }

    if( modified )
    {
        st_s* txt_h = BLM_CREATE( st_s );
        st_s* txt_c = BLM_CREATE( st_s );
        bcore_plant_target_s_expand_h( o, 0, ( bcore_sink* )txt_h );
        bcore_plant_target_s_expand_c( o, 0, ( bcore_sink* )txt_c );

        st_s* file_h = BLM_A_PUSH( st_s_create_fa( "#<sc_t>.h", o->path.sc ) );
        st_s* file_c = BLM_A_PUSH( st_s_create_fa( "#<sc_t>.c", o->path.sc ) );

        #ifdef BCORE_PLANT_DRY_RUN

        bcore_msg_fa( "\nPlant compiler is in mode 'dry-run'\n" );

        bcore_msg_fa( "#r80{+}\n" );
        bcore_msg_fa( "File #<sc_t>:\n", file_h->sc );
        bcore_msg_fa( "#r80{+}\n" );
        st_s_to_stdout( txt_h );

        bcore_msg_fa( "#r80{+}\n" );
        bcore_msg_fa( "File #<sc_t>:\n", file_c->sc );
        bcore_msg_fa( "#r80{+}\n" );
        st_s_to_stdout( txt_c );

        #else

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
        bcore_sink_a_push_data( BLM_A_PUSH( bcore_file_open_sink( file_h->sc ) ), txt_h->data, txt_h->size );

        bcore_msg_fa( "writing '#<sc_t>'\n", file_c->sc );
        bcore_sink_a_push_data( BLM_A_PUSH( bcore_file_open_sink( file_c->sc ) ), txt_c->data, txt_c->size );

        #endif // BCORE_PLANT_DRY_RUN
    }

    BLM_RETURNV( bl_t, modified );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// plant_compiler

/// returns false if already registered; checks for collision
static tp_t bcore_plant_compiler_s_group_register( bcore_plant_compiler_s* o, const bcore_plant_group_s* group, bcore_source* source )
{
    sc_t global_name = group->name.sc;
    tp_t global_id = typeof( global_name );
    if( bcore_hmap_tpvd_s_exists( &o->hmap_group, global_id ) )
    {
        /// check collision
        const bcore_plant_group_s* group2 = *bcore_hmap_tpvd_s_get( &o->hmap_group, global_id );
        sc_t global_name2 = group2->name.sc;

        if( !sc_t_equal( global_name, global_name2 ) )
        {
            bcore_source_a_parse_err_fa( source, "Collision detected: '#<sc_t>' and '#<sc_t>' produce same hash\n", global_name, global_name2 );
        }
        else
        {
            bcore_source_a_parse_err_fa( source, "'#<sc_t>' was already registered\n", global_name );
        }
    }
    bcore_hmap_tpvd_s_set( &o->hmap_group, global_id, ( vd_t )group );
    return global_id;
}

//----------------------------------------------------------------------------------------------------------------------

/// returns false if already registered; checks for collision
static tp_t bcore_plant_compiler_s_item_register( bcore_plant_compiler_s* o, const bcore_plant* item, bcore_source* source )
{
    sc_t global_name = bcore_plant_a_get_global_name_sc( item );
    tp_t global_id = typeof( global_name );
    if( bcore_hmap_tpvd_s_exists( &o->hmap_item, global_id ) )
    {
        /// check collision
        const bcore_plant* item2 = *bcore_hmap_tpvd_s_get( &o->hmap_item, global_id );
        sc_t global_name2 = bcore_plant_a_get_global_name_sc( item2 );
        if( !sc_t_equal( global_name, global_name2 ) )
        {
            bcore_source_a_parse_err_fa( source, "Collision detected: '#<sc_t>' and '#<sc_t>' produce same hash\n", global_name, global_name2 );
        }
        else
        {
            bcore_source_a_parse_err_fa( source, "'#<sc_t>' was already registered\n", global_name );
        }
    }

    bcore_hmap_tpvd_s_set( &o->hmap_item, global_id, ( vd_t )item );
    return global_id;
}

//----------------------------------------------------------------------------------------------------------------------

static bl_t bcore_plant_compiler_s_item_exists( const bcore_plant_compiler_s* o, tp_t item_id )
{
    return bcore_hmap_tpvd_s_exists( &o->hmap_item, item_id );
}

//----------------------------------------------------------------------------------------------------------------------

static const bcore_plant* bcore_plant_compiler_s_item_get( const bcore_plant_compiler_s* o, tp_t item_id )
{
    vd_t* ptr = bcore_hmap_tpvd_s_get( &o->hmap_item, item_id );
    return ptr ? ( const bcore_plant* )*ptr : NULL;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_compiler_s_parse( bcore_plant_compiler_s* o, sc_t target_name, sc_t source_path )
{
    BLM_INIT();

    st_s* source_folder_path = BLM_A_PUSH( bcore_file_folder_path( source_path ) );
    st_s* target_path        = BLM_A_PUSH( st_s_create_fa( "#<sc_t>/#<sc_t>", source_folder_path->sc, target_name ) );

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
        bcore_plant_target_s* target = bcore_plant_target_s_create();
        target->hash = bcore_tp_init();
        st_s_copy_sc( &target->name, target_name );
        st_s_copy(    &target->path, target_path );
        bcore_array_a_push( ( bcore_array* )o, sr_asd( target ) );
        target_index = o->size - 1;
    }

    bcore_plant_target_s* target = o->data[ target_index ];
    bcore_plant_target_s_parse( target, source_path );

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_plant_compiler_s_finalize( bcore_plant_compiler_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_plant_target_s_finalize( o->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

/// returns true if a file was modified
static bl_t bcore_plant_compiler_s_expand( bcore_plant_compiler_s* o )
{
    bl_t modified = false;
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( bcore_plant_target_s_expand( o->data[ i ] ) ) modified = true;
    }
    return modified;
}

//----------------------------------------------------------------------------------------------------------------------

/// adds an item to lifetime manager
static void bcore_plant_compiler_s_life_a_push( bcore_plant_compiler_s* o, vd_t object )
{
    bcore_life_s_push_aware( &o->life, object );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// plant interface

//----------------------------------------------------------------------------------------------------------------------

void bcore_plant_compile( sc_t target_name, sc_t source_path )
{
    if( !plant_compiler_g ) plant_compiler_g = bcore_plant_compiler_s_create();
    bcore_plant_compiler_s_parse( plant_compiler_g, target_name, source_path );
    bcore_plant_compiler_s_finalize( plant_compiler_g );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_plant_expand()
{
    ASSERT( plant_compiler_g );
    bl_t modified = false;
    f3_t time = 0;
    CPU_TIME_OF( modified = bcore_plant_compiler_s_expand( plant_compiler_g ), time );
    if( modified )
    {
        bcore_msg_fa( "PLANT: Expanded in #<f3_t> sec.\n", time );
        bcore_msg_fa( "PLANT: Files were updated. Rebuild is necessary.\n" );
    }
    bcore_plant_compiler_s_discard( plant_compiler_g );
    plant_compiler_g = NULL;
    return modified;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_plant_run_globally()
{
    f3_t time = 0;
    CPU_TIME_OF( bcore_run_signal_globally( TYPEOF_all, TYPEOF_plant, NULL ), time );
    bcore_msg_fa( "PLANT: Compiled in #<f3_t> sec.\n", time );
    return bcore_plant_expand();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bcore_plant_compiler_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_plant_compiler" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FEATURE( bcore_plant_fp_parse );
            BCORE_REGISTER_FEATURE( bcore_plant_fp_get_hash );
            BCORE_REGISTER_FEATURE( bcore_plant_fp_get_global_name_sc );
            BCORE_REGISTER_FEATURE( bcore_plant_fp_finalize );
            BCORE_REGISTER_FEATURE( bcore_plant_fp_expand_forward );
            BCORE_REGISTER_FEATURE( bcore_plant_fp_expand_indef_typedef );
            BCORE_REGISTER_FEATURE( bcore_plant_fp_expand_spect_declaration );
            BCORE_REGISTER_FEATURE( bcore_plant_fp_expand_spect_definition );
            BCORE_REGISTER_FEATURE( bcore_plant_fp_expand_declaration );
            BCORE_REGISTER_FEATURE( bcore_plant_fp_expand_indef_declaration );
            BCORE_REGISTER_FEATURE( bcore_plant_fp_expand_definition );
            BCORE_REGISTER_FEATURE( bcore_plant_fp_expand_init1 );
            BCORE_REGISTER_SPECT( bcore_plant );

            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_hash, bcore_plant_arg_s_get_hash );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_parse,    bcore_plant_arg_s_parse );
            BCORE_REGISTER_OBJECT( bcore_plant_arg_s );

            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_hash, bcore_plant_args_s_get_hash );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_parse,    bcore_plant_args_s_parse );
            BCORE_REGISTER_OBJECT( bcore_plant_args_s );

            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_hash,           bcore_plant_signature_s_get_hash );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_parse,              bcore_plant_signature_s_parse );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_global_name_sc, bcore_plant_signature_s_get_global_name_sc );
            BCORE_REGISTER_OBJECT( bcore_plant_signature_s );

            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_hash,           bcore_plant_body_s_get_hash );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_parse,              bcore_plant_body_s_parse );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_global_name_sc, bcore_plant_body_s_get_global_name_sc );
            BCORE_REGISTER_OBJECT( bcore_plant_body_s );

            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_hash,                 bcore_plant_feature_s_get_hash );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_parse,                    bcore_plant_feature_s_parse );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_global_name_sc,       bcore_plant_feature_s_get_global_name_sc );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_indef_typedef,     bcore_plant_feature_s_expand_indef_typedef );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_spect_declaration, bcore_plant_feature_s_expand_spect_declaration );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_spect_definition,  bcore_plant_feature_s_expand_spect_definition );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_indef_declaration, bcore_plant_feature_s_expand_indef_declaration );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_definition,        bcore_plant_feature_s_expand_definition );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_init1,             bcore_plant_feature_s_expand_init1 );
            BCORE_REGISTER_OBJECT( bcore_plant_feature_s );

            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_hash, bcore_plant_func_s_get_hash );
            BCORE_REGISTER_OBJECT( bcore_plant_func_s );

            BCORE_REGISTER_OBJECT( bcore_plant_funcs_s );

            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_hash,                  bcore_plant_stamp_s_get_hash );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_parse,                     bcore_plant_stamp_s_parse );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_finalize,                  bcore_plant_stamp_s_finalize );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_global_name_sc,        bcore_plant_stamp_s_get_global_name_sc );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_declaration,        bcore_plant_stamp_s_expand_declaration );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_forward,            bcore_plant_stamp_s_expand_forward );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_indef_declaration,  bcore_plant_stamp_s_expand_indef_declaration );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_definition,         bcore_plant_stamp_s_expand_definition );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_init1,              bcore_plant_stamp_s_expand_init1 );
            BCORE_REGISTER_OBJECT( bcore_plant_stamp_s );

            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_hash,           bcore_plant_name_s_get_hash );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_parse,              bcore_plant_name_s_parse );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_global_name_sc, bcore_plant_name_s_get_global_name_sc );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_declaration, bcore_plant_name_s_expand_declaration );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_init1,       bcore_plant_name_s_expand_init1 );
            BCORE_REGISTER_OBJECT( bcore_plant_name_s );

            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_hash,           bcore_plant_group_s_get_hash );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_parse,              bcore_plant_group_s_parse );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_finalize,           bcore_plant_group_s_finalize );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_global_name_sc, bcore_plant_group_s_get_global_name_sc );
            BCORE_REGISTER_OBJECT( bcore_plant_group_s );

            BCORE_REGISTER_FFUNC( bcore_plant_fp_get_hash,                  bcore_plant_nested_group_s_get_hash );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_forward,            bcore_plant_nested_group_s_expand_forward );
            BCORE_REGISTER_FFUNC( bcore_plant_fp_expand_indef_declaration,  bcore_plant_nested_group_s_expand_indef_declaration );
            BCORE_REGISTER_OBJECT( bcore_plant_nested_group_s );

            BCORE_REGISTER_OBJECT( bcore_plant_source_s );
            BCORE_REGISTER_FFUNC(  bcore_plant_fp_finalize, bcore_plant_source_s_finalize );
            BCORE_REGISTER_OBJECT( bcore_plant_target_s );
            BCORE_REGISTER_FFUNC(  bcore_plant_fp_finalize, bcore_plant_target_s_finalize );
            BCORE_REGISTER_OBJECT( bcore_plant_compiler_s );
            BCORE_REGISTER_FFUNC(  bcore_plant_fp_finalize, bcore_plant_compiler_s_finalize );
        }
        break;

        case TYPEOF_down1:
        {
             if( plant_compiler_g )  bcore_plant_compiler_s_discard( plant_compiler_g );
             plant_compiler_g = NULL;
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( plant );

            BCORE_REGISTER_QUICKTYPE( bcore_plant );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_arg_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_args_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_signature_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_body_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_feature_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_func_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_funcs_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_stamp_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_name_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_group_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_nested_group_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_source_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_target_s );
            BCORE_REGISTER_QUICKTYPE( bcore_plant_compiler_s );

            BCORE_REGISTER_QUICKTYPE( const );
            BCORE_REGISTER_QUICKTYPE( mutable );
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

