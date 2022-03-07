/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include <stdio.h>
#include "bcore_flect.h"
#include "bcore_control.h"
#include "bcore_threads.h"
#include "bcore_life.h"
#include "bcore_hmap.h"
#include "bcore_tbman.h"
#include "bcore_trait.h"
#include "bcore_spect_source.h"
#include "bcore_signal.h"
#include "bcore_arr.h"
#include "bcore_spect_sink.h"
#include "bcore_sinks.h"
#include "bcore_txt_ml.h"
#include "bcore_sc.h"
#include "bcore_const_manager.h"
#include "bcore_error_manager.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static const sc_t bcore_flect_caps_e_sc_arr[] =
{
    "START",

    "SOLID_STATIC",
    "LINK_STATIC",
    "LINK_TYPED",
    "LINK_AWARE",
    "POINTER",
    "ARRAY_DYN_SOLID_STATIC",
    "ARRAY_DYN_SOLID_TYPED",
    "ARRAY_DYN_LINK_STATIC",
    "ARRAY_DYN_LINK_TYPED",
    "ARRAY_DYN_LINK_AWARE",
    "ARRAY_FIX_SOLID_STATIC",
    "ARRAY_FIX_LINK_STATIC",
    "ARRAY_FIX_LINK_AWARE",
    "EXTERNAL_FUNC",

    "END",
};

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_flect_caps_is_array( u2_t caps )
{
    switch( caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return true;
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   return true;
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   return true;
        default: break;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_flect_caps_is_array_dyn( u2_t caps )
{
    switch( caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return true;
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   return true;
        default: break;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_flect_caps_is_array_fix( u2_t caps )
{
    switch( caps )
    {
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   return true;
        default: break;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_flect_caps_is_array_of_links( u2_t caps )
{
    switch( caps )
    {
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:
            return true;

        default:
            return false;
    }
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_flect_caps_is_aware( u2_t caps )
{
    switch( caps )
    {
        case BCORE_CAPS_LINK_AWARE:           return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE: return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE: return true;
        default: break;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_flect_caps_is_typed( u2_t caps )
{
    switch( caps )
    {
        case BCORE_CAPS_LINK_TYPED:            return true;
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED: return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:  return true;
        default: break;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

sz_t bcore_flect_caps_get_indirection( u2_t caps )
{
    switch( caps )
    {
        case BCORE_CAPS_SOLID_STATIC: return 0;
        case BCORE_CAPS_LINK_STATIC:  return 1;
        case BCORE_CAPS_LINK_TYPED:   return 1;
        case BCORE_CAPS_LINK_AWARE:   return 1;
        case BCORE_CAPS_POINTER:      return 1;
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return 0;
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return 0;
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return 1;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return 1;
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   return 1;
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return 0;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return 1;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   return 1;
        case BCORE_CAPS_EXTERNAL_FUNC:          return 0;
        default: return -1;
    }
}

//----------------------------------------------------------------------------------------------------------------------

sc_t bcore_flect_caps_e_sc( u2_t caps )
{
    const u2_t size = sizeof( bcore_flect_caps_e_sc_arr ) / sizeof( sc_t );
    if( caps >= size ) ERR( "invalid value %u", caps );
    return bcore_flect_caps_e_sc_arr[ caps ];
}

//----------------------------------------------------------------------------------------------------------------------

u2_t bcore_flect_caps_e_u2( sc_t sc )
{
    const u2_t size = sizeof( bcore_flect_caps_e_sc_arr ) / sizeof( sc_t );
    for( u2_t i = 0; i < size; i++ ) if( bcore_strcmp( sc, bcore_flect_caps_e_sc_arr[ i ] ) == 0 ) return i;
    ERR( "invalid string '%s'", sc );
    return 0;
}

/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTION_INIT_FLAT( bcore_self_item_s )

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_item_s_down( bcore_self_item_s* o )
{
    if( o->child_item ) bcore_self_item_s_discard( o->child_item );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_item_s_copy( bcore_self_item_s* o, const bcore_self_item_s* src )
{
    if( o->child_item ) bcore_self_item_s_discard( o->child_item );
    bcore_memcpy( o, src, sizeof( bcore_self_item_s ) );
    if( src->child_item ) o->child_item = bcore_self_item_s_clone( src->child_item );
}

//----------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_MOVE (     bcore_self_item_s )
BCORE_DEFINE_FUNCTION_CREATE(    bcore_self_item_s )
BCORE_DEFINE_FUNCTION_DISCARD(   bcore_self_item_s )
BCORE_DEFINE_FUNCTION_CLONE(     bcore_self_item_s )

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_self_item_s_show( const bcore_self_item_s* o )
{
    st_s* s = st_s_create();
    st_s_push_fa( s, "bcore_self_item_s" );
    st_s_push_fa( s, "\n{" );
    st_s_push_fa( s, "\n    type    : #<tp_t> '#<sc_t>'", o->type, ifnameof( o->type ) );
    st_s_push_fa( s, "\n    name    : #<tp_t> '#<sc_t>'", o->name, ifnameof( o->name ) );
    st_s_push_fa( s, "\n    flags   : #<tp_t>",  o->flags );
    st_s_push_fa( s, "\n    caps    : #<sc_t>",  bcore_flect_caps_e_sc( o->caps ) );
    st_s_push_fa( s, "\n    default : #<s3_t>",  o->default_s3 );
    st_s_pushf( s, "\n}" );
    return s;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_item_s* bcore_self_item_s_create_plain( u2_t caps, tp_t type, tp_t name )
{
    bcore_self_item_s* o = bcore_self_item_s_create();
    o->caps  = caps;
    o->type  = type;
    o->name  = name;
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_item_s* bcore_self_item_s_create_func( sc_t fname, fp_t func, sc_t type, sc_t name )
{
    bcore_self_item_s* o = bcore_self_item_s_create();
    o->caps  = BCORE_CAPS_EXTERNAL_FUNC;
    o->type  = entypeof( type );
    o->name  = entypeof( name );
    o->default_tp = bcore_function_set_sc( fname, func );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bcore_flect_aligned_offset( uz_t align, uz_t raw_offset )
{
    if( align < 2 ) return raw_offset;
    uz_t offset = raw_offset;
    if( ( offset & ( align - 1 ) ) != 0 ) offset = ( offset + align ) & ~( align - 1 );
    return offset;
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_self_item_s_fold_tp( const bcore_self_item_s* o, tp_t tp )
{
    tp = bcore_tp_fold_tp( tp, o->type );
    tp = bcore_tp_fold_tp( tp, o->name );
    tp = bcore_tp_fold_u2( tp, o->caps );
    tp = bcore_tp_fold_tp( tp, o->flags.data );

    umax_t v = o->default_umax;
    for( uz_t i = 0; i < sizeof( umax_t ); i++ )
    {
        tp = bcore_tp_fold_u0( tp, v );
        v >>= 8;
    }
    return tp;
}

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_self_item_s_cmp( const bcore_self_item_s* o1, const bcore_self_item_s* o2 )
{
    if( o1->type       != o2->type       ) return ( o1->type < o2->type ) ? 1 : -1;
    if( o1->name       != o2->name       ) return ( o1->name < o2->name ) ? 1 : -1;
    if( o1->caps       != o2->caps       ) return ( o1->caps < o2->caps ) ? 1 : -1;
    if( o1->flags.data != o2->flags.data ) return ( o1->flags.data < o2->flags.data ) ? 1 : -1;
    if( o1->default_u3 != o2->default_u3 ) return ( o1->default_u3 < o2->default_u3 ) ? 1 : -1;
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_self_item_s_has_default_value( const bcore_self_item_s* o )
{
    if( bcore_flect_caps_is_array_fix( o->caps ) ) return false;
    if( o->default_umax == 0 ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bcore_self_item_s_inst_size( const bcore_self_item_s* o )
{
    switch( o->caps )
    {
        case BCORE_CAPS_SOLID_STATIC:           return 0;
        case BCORE_CAPS_LINK_STATIC:            return sizeof( bcore_link_static_s );
        case BCORE_CAPS_LINK_TYPED:             return sizeof( bcore_link_typed_s );
        case BCORE_CAPS_LINK_AWARE:             return sizeof( bcore_link_aware_s );
        case BCORE_CAPS_POINTER:                return sizeof( vd_t );
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return sizeof( bcore_array_dyn_solid_static_s );
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return sizeof( bcore_array_dyn_solid_typed_s );
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return sizeof( bcore_array_dyn_link_static_s );
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return sizeof( bcore_array_dyn_link_typed_s );
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   return sizeof( bcore_array_dyn_link_aware_s );
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return 0;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return sizeof( bcore_link_static_s ) * o->array_fix_size;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   return sizeof( bcore_link_aware_s  ) * o->array_fix_size;
        case BCORE_CAPS_EXTERNAL_FUNC:          return 0;
        default: break;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bcore_self_item_s_inst_align( const bcore_self_item_s* o )
{
    switch( o->caps )
    {
        case BCORE_CAPS_SOLID_STATIC:           return 0;
        case BCORE_CAPS_LINK_STATIC:            return alignof( bcore_link_static_s );
        case BCORE_CAPS_LINK_TYPED:             return alignof( bcore_link_typed_s );
        case BCORE_CAPS_LINK_AWARE:             return alignof( bcore_link_aware_s );
        case BCORE_CAPS_POINTER:                return alignof( vd_t );
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return alignof( bcore_array_dyn_solid_static_s );
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return alignof( bcore_array_dyn_solid_typed_s );
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return alignof( bcore_array_dyn_link_static_s );
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return alignof( bcore_array_dyn_link_typed_s );
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   return alignof( bcore_array_dyn_link_aware_s );
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return 0;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return alignof( bcore_link_static_s );
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   return alignof( bcore_link_aware_s  );
        case BCORE_CAPS_EXTERNAL_FUNC:          return 0;
        default: break;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_item_s_check_integrity( const bcore_self_item_s* o )
{
    if( !o ) return;
    if( o->caps <= BCORE_CAPS_START ) ERR( "Invalid capsulation" );
    if( o->caps >= BCORE_CAPS_END   ) ERR( "Invalid capsulation" );
}

//----------------------------------------------------------------------------------------------------------------------

static er_t bcore_self_item_s_parse_data_src( bcore_self_item_s* o, sr_s src, const bcore_self_s* self, bl_t advanced_checks )
{
    BLM_INIT();
    src = bcore_life_s_push_sr( __bcore_life_curent, sr_cp( src, TYPEOF_bcore_source_s ) );

    tp_t parent_type = self ? self->type : 0;

    st_s* type_name = BLM_CREATE( st_s );
    st_s* item_name = BLM_CREATE( st_s );

    bl_t f_private   = false;
    bl_t f_hidden    = false;
    bl_t f_shell     = false;
    bl_t f_link      = false;
    bl_t f_arr_dyn   = false;
    bl_t f_arr_fix   = false;
    bl_t f_spect     = false;
    bl_t f_const     = false;
    bl_t f_deep_copy = true;
    bl_t f_treat_as_pointer = false;
    bl_t f_feature   = false;
    bl_t f_feature_requires_awareness = false;
    bl_t f_strict    = false;
    bl_t f_aware     = false;
    bl_t f_obliv     = false;
    bl_t f_typed     = false;

    uz_t array_fix_size = 0;

    bl_t f_any_prefix = true;
    while( f_any_prefix )
    {
        f_any_prefix = false;
        if( bcore_source_r_parse_bl_fa( &src, " #?w'private'" ) )
        {
            if( f_private ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'private' occurs twice.", ifnameof( parent_type ) ) );
            f_any_prefix = f_private = true;
        }

        if( bcore_source_r_parse_bl_fa( &src, " #?w'hidden'"  ) )
        {
            if( f_hidden ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'hidden' occurs twice.", ifnameof( parent_type ) ) );
            f_any_prefix = f_hidden = true;
        }

        if( bcore_source_r_parse_bl_fa( &src, " #?w'shell'"   ) )
        {
            if( f_shell ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'shell' occurs twice.", ifnameof( parent_type ) ) );
            f_any_prefix = f_shell = true;
        }

        if( bcore_source_r_parse_bl_fa( &src, " #?w'spect'"   ) )
        {
            if( f_spect ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'spect' occurs twice.", ifnameof( parent_type ) ) );
            f_any_prefix = f_spect = true;
        }

        if( bcore_source_r_parse_bl_fa( &src, " #?w'const'"   ) )
        {
            if( f_const ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'const' occurs twice.", ifnameof( parent_type ) ) );
            f_any_prefix = f_const = true;
        }

        if( bcore_source_r_parse_bl_fa( &src, " #?w'feature'"   ) )
        {
            if( f_feature ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'feature' occurs twice.", ifnameof( parent_type ) ) );
            f_any_prefix = f_feature = true;
        }

        if( bcore_source_r_parse_bl_fa( &src, " #?w'strict'"   ) )
        {
            if( f_strict ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'strict' occurs twice.", ifnameof( parent_type ) ) );
            f_any_prefix = f_strict = true;
        }

        if( bcore_source_r_parse_bl_fa( &src, " #?w'obliv'"   ) )
        {
            if( f_obliv ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'obliv' occurs twice.", ifnameof( parent_type ) ) );
            if( f_aware ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'obliv' cannot be mixed with 'aware'.", ifnameof( parent_type ) ) );
            f_any_prefix = f_obliv = true;
        }

        if( bcore_source_r_parse_bl_fa( &src, " #?w'aware'"   ) )
        {
            if( f_aware ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'aware' occurs twice.", ifnameof( parent_type ) ) );
            if( f_typed ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'aware' cannot be mixed with 'typed'.", ifnameof( parent_type ) ) );
            if( f_obliv ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'aware' cannot be mixed with 'obliv'.", ifnameof( parent_type ) ) );
            f_any_prefix = f_aware = true;
        }

        if( bcore_source_r_parse_bl_fa( &src, " #?w'typed'"   ) )
        {
            if( f_typed ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'typed' occurs twice.", ifnameof( parent_type ) ) );
            if( f_aware ) BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPrefix 'typed' cannot be mixed with 'aware'.", ifnameof( parent_type ) ) );
            f_any_prefix = f_typed = true;
        }
    }

    if( f_aware && f_feature )
    {
        f_feature_requires_awareness = true;
        f_aware = false;
    }

    // type can be specified by explicit type id number (anonymous types) or by name
    tp_t type_val = 0;
    bl_t extend_type_name = false;
    if( bcore_source_r_parse_bl_fa( &src, " #?([0]=='{')" ) ) // nested anonymous type
    {
        type_val = bcore_flect_type_parse_src( src );
    }
    else if( bcore_source_r_parse_bl_fa( &src, " #?([0]>='0'&&[0]<='9')" ) ) // type is specified by number
    {
        BLM_TRY( bcore_source_r_parse_em_fa( &src, "#<tp_t*> ", &type_val ) );
    }
    else // type is specified by name
    {
        BLM_TRY( bcore_source_r_parse_em_fa( &src, "#name #?':' ", type_name, &extend_type_name ) );
    }

    if( bcore_source_r_parse_bl_fa( &src, "#?'*' " ) )
    {
        if( !(f_private || f_hidden ) )
        {
            BLM_DOWN();
            return bcore_source_a_parse_error_fa
            (
                src.o,
                "In definition of '#<sc_t>':\n"
                "Declaring a pointer requires 'hidden' or 'private' specification.\n",
                ifnameof( parent_type )
            );
        }
        f_link = true;
        f_treat_as_pointer = true;
        f_deep_copy = false;
    }
    else if( bcore_source_r_parse_bl_fa( &src, "#?'=>' " ) )
    {
        f_link = true;
        f_deep_copy = true;
    }
    else if( bcore_source_r_parse_bl_fa( &src, "#?'->' " ) )
    {
        f_link = true;
        f_deep_copy = false;
    }

    if( bcore_source_r_parse_bl_fa( &src, "#?'[' " ) )
    {
        if( f_treat_as_pointer )
        {
            BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nArray of pointers is not supported.", ifnameof( parent_type ) ) );
        }

        if( bcore_source_r_parse_bl_fa( &src, "#?']' " ) )
        {
            f_arr_dyn = true;
        }
        else if( bcore_source_r_parse_bl_fa( &src, "#?([0]>='0'&&[0]<='9')" ) )
        {
            f_arr_fix = true;
            BLM_TRY( bcore_source_r_parse_em_fa( &src, "#<uz_t*> ", &array_fix_size ) );
            if( !bcore_source_r_parse_bl_fa( &src, "#?']' " ) )
            {
                BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\n']' expected.", ifnameof( parent_type ) ) );
            }
        }
        else
        {
            BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\n']' or array fixed-size expected.", ifnameof( parent_type ) ) );
        }
    }

    BLM_TRY( bcore_source_r_parse_em_fa( &src, "#name ", item_name ) );

    if( extend_type_name )
    {
        st_s* type_prefix = st_s_clone( type_name );
        st_s_copy_fa( type_name, "#<sc_t>_#<sc_t>", type_prefix->sc, item_name->sc );
        st_s_discard( type_prefix );
    }

    o->flags.f_private   = f_private || f_spect;
    o->flags.f_hidden    = f_hidden;
    o->flags.f_shell     = f_shell;
    o->flags.f_spect     = f_spect;
    o->flags.f_deep_copy = f_deep_copy;
    o->flags.f_const     = f_const;
    o->flags.f_feature   = f_feature;
    o->flags.f_feature_requires_awarenes = f_feature_requires_awareness;
    o->flags.f_strict    = f_strict;
    o->flags.f_virtual   = f_aware || f_typed || f_treat_as_pointer;
    o->flags.f_obliv     = f_obliv;
    o->flags.f_aware     = f_aware;

    if( f_arr_fix ) o->array_fix_size = array_fix_size;

    if( f_spect && ( !f_link || f_deep_copy ) )
    {
        BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nPerspectives are shallow links. Use 'spect #<sc_t> ->' to clarify method of referencing.", ifnameof( parent_type ), type_name->sc ) );
    }

    if( f_treat_as_pointer )
    {
        o->type = ( type_val > 0 ) ? type_val : ( type_name->size > 0 ? entypeof( type_name->sc ) : 0 );
        o->name = entypeof( item_name->sc );
        o->caps = BCORE_CAPS_POINTER;
    }
    else if( f_aware )
    {
        if( !f_link )
        {
            BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nSelf-aware object must be referenced by a link. Use 'aware *|=>|->' to clarify method of referencing.", ifnameof( parent_type ) ) );
        }
        o->type = ( type_val > 0 ) ? type_val : ( type_name->size > 0 ? entypeof( type_name->sc ) : 0 );
        o->name = entypeof( item_name->sc );
        o->caps = f_arr_dyn ? BCORE_CAPS_ARRAY_DYN_LINK_AWARE : f_arr_fix ? BCORE_CAPS_ARRAY_FIX_LINK_AWARE : BCORE_CAPS_LINK_AWARE;
    }
    else if( f_typed )
    {
        if( f_arr_fix )
        {
            BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nTyped objects not supported for fixed size arrays. Suggest dynamic arrays.", ifnameof( parent_type ) ) );
        }

        if( !f_link && !f_arr_dyn )
        {
            BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nTyped objects cannot be nested. Use 'typed *|=>|->' to clarify method of referencing.", ifnameof( parent_type ) ) );
        }

        o->type = ( type_val > 0 ) ? type_val : ( type_name->size > 0 ? entypeof( type_name->sc ) : 0 );
        o->name = entypeof( item_name->sc );
        o->caps = f_arr_dyn ? ( f_link ? BCORE_CAPS_ARRAY_DYN_LINK_TYPED : BCORE_CAPS_ARRAY_DYN_SOLID_TYPED ) : BCORE_CAPS_LINK_TYPED;
    }
    else
    {
        o->type = ( type_val > 0 ) ? type_val : entypeof( type_name->sc );
        o->name = entypeof( item_name->sc );
        o->caps =
            f_arr_dyn ?
                ( f_link ? BCORE_CAPS_ARRAY_DYN_LINK_STATIC : BCORE_CAPS_ARRAY_DYN_SOLID_STATIC )
            :
                f_arr_fix ?
                    ( f_link ? BCORE_CAPS_ARRAY_FIX_LINK_STATIC : BCORE_CAPS_ARRAY_FIX_SOLID_STATIC )
                :
                    ( f_link ? BCORE_CAPS_LINK_STATIC : BCORE_CAPS_SOLID_STATIC );
    }

    if( o->type == typeof( "aware_t" ) )
    {
        if( o->caps != BCORE_CAPS_SOLID_STATIC )
        {
            BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\n'aware_t' can only be used in single solid static nesting.", ifnameof( parent_type ) ) );
        }
    }

    o->flags.f_fp = bcore_trait_is_of( o->type, TYPEOF_function_pointer );

    if( bcore_source_r_parse_bl_fa( &src, "#?'!' " ) )
    {

        if( !o->flags.f_spect && ( o->caps == BCORE_CAPS_LINK_STATIC ) )
        {
            if( o->type == parent_type )
            {
                BLM_DOWN();
                return bcore_source_a_parse_error_fa
                (
                    src.o,
                    "Parent '#<sc_t>':\nItem #<sc_t>: Postfix '!' is not allowed on recursive members.",
                    ifnameof( parent_type ),
                    item_name->sc
                );
            }
            o->flags.f_create_on_init = true;
        }
        else if( !o->flags.f_spect && o->caps == BCORE_CAPS_SOLID_STATIC )
        {
            /* '!' for solid static types is allowed but has no effect */
        }
        else
        {
            BLM_DOWN();
            return bcore_source_a_parse_error_fa
            (
                src.o,
                "Parent '#<sc_t>':\nItem #<sc_t>: Postfix '!' is not supported in this context.",
                ifnameof( parent_type ),
                item_name->sc
            );
        }
    }
    else if( bcore_source_r_parse_bl_fa( &src, "#?'=' " ) )
    {
        if( o->caps == BCORE_CAPS_SOLID_STATIC )
        {
            switch( o->type )
            {
                case TYPEOF_s0_t:
                case TYPEOF_s1_t:
                case TYPEOF_s2_t:
                case TYPEOF_s3_t:
                    BLM_TRY( bcore_source_r_parse_em_fa( &src, " #<s3_t*>", &o->default_s3 ) );
                    break;
                case TYPEOF_u0_t:
                case TYPEOF_u1_t:
                case TYPEOF_u2_t:
                case TYPEOF_u3_t:
                    BLM_TRY( bcore_source_r_parse_em_fa( &src, " #<u3_t*>", &o->default_u3 ) );
                    break;

                case TYPEOF_sz_t:
                case TYPEOF_uz_t:
                case TYPEOF_offset_t:
                {
                    // for these types scanning of negative numbers is allowed, which will be wrapped around u3.
                    if( bcore_source_r_parse_bl_fa( &src, " #=?'-'" ) )
                    {
                        BLM_TRY( bcore_source_r_parse_em_fa( &src, " #<smax_t*>", &o->default_smax ) );
                    }
                    else
                    {
                        BLM_TRY( bcore_source_r_parse_em_fa( &src, " #<umax_t*>", &o->default_umax ) );
                    }
                }
                break;

                case TYPEOF_tp_t:
                case TYPEOF_aware_t:
                case TYPEOF_er_t:
                {
                    if( bcore_source_r_parse_bl_fa( &src, " #?([0]>='0'&&[0]<='9')" ) )
                    {
                        BLM_TRY( bcore_source_r_parse_em_fa( &src, " #<umax_t*>", &o->default_umax ) );
                    }
                    else if( bcore_source_r_parse_bl_fa( &src, " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z'))" ) )
                    {
                        st_s* name = st_s_create();
                        BLM_TRY( bcore_source_r_parse_em_fa( &src, "#name", name ) );
                        o->default_tp = entypeof( name->sc );
                        st_s_discard( name );
                    }
                    else
                    {
                        BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nCannot assign default value to type '#<sc_t>'", ifnameof( parent_type ), ifnameof( o->type ) ) );
                    }
                }
                break;

                case TYPEOF_f2_t:
                case TYPEOF_f3_t:
                    BLM_TRY( bcore_source_r_parse_em_fa( &src, " #<f3_t*>", &o->default_f3 ) );
                    break;

                case TYPEOF_bl_t:
                {
                    bl_t flag = false;
                    BLM_TRY( bcore_source_r_parse_em_fa( &src, " #<bl_t*>", &flag ) );
                    o->default_u3 = flag;
                }
                break;

                case TYPEOF_fp_t:
                {
                    st_s* name = st_s_create();
                    BLM_TRY( bcore_source_r_parse_em_fa( &src, " #name", name ) );
                    tp_t func_tp = typeof( name->sc );
                    if( !bcore_function_exists( func_tp ) )
                    {
                        BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\n'#<sc_t>' is not a registered function.", ifnameof( parent_type ), name->sc ) );
                    }
                    st_s_discard( name );
                    o->default_tp = func_tp;
                }
                break;

                case TYPEOF_sc_t:
                case TYPEOF_st_s:
                {
                    st_s* string = st_s_create();
                    BLM_TRY( bcore_source_r_parse_em_fa( &src, " #string", string ) );
                    o->default_tp = bcore_const_string_set_st_d( string ); // thread and collision safe
                }
                break;

                default:
                {
                    if( o->flags.f_fp ) // assume registered feature
                    {
                        st_s* name = st_s_create();
                        BLM_TRY( bcore_source_r_parse_em_fa( &src, " #name", name ) );
                        tp_t func_tp = typeof( name->sc );

                        if( !bcore_function_exists( func_tp ) )
                        {
                            BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\n'#<sc_t>' is not a registered function.", ifnameof( parent_type ), name->sc ) );
                        }

                        if( bcore_trait_exists( o->type ) )
                        {
                            if( !bcore_trait_is_of( func_tp, o->type ) )
                            {
                                BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nFunction '#<sc_t>' is not of feature '#<sc_t>'.", ifnameof( parent_type ), name->sc, ifnameof( o->type ) ) );
                            }
                        }

                        st_s_discard( name );

                        o->default_tp = func_tp;
                    }
                    else
                    {
                        BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nCannot assign default value to type '#<sc_t>'", ifnameof( parent_type ), ifnameof( o->type ) ) );
                    }
                }
            }
        }
        else if( o->caps == BCORE_CAPS_LINK_STATIC )
        {
            if( f_spect )
            {
                if( bcore_source_r_parse_bl_fa( &src, " #?([0]>='0'&&[0]<='9')" ) )
                {
                    BLM_TRY( bcore_source_r_parse_em_fa( &src, " #<umax_t*>", &o->default_umax ) );
                }
                else if( bcore_source_r_parse_bl_fa( &src, " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z'))" ) )
                {
                    st_s* name = st_s_create();
                    BLM_TRY( bcore_source_r_parse_em_fa( &src, "#name", name ) );
                    o->default_tp = entypeof( name->sc );
                    st_s_discard( name );
                }
                else
                {
                    BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nCannot assign default value to type '#<sc_t>'", ifnameof( parent_type ), ifnameof( o->type ) ) );
                }
            }
            else
            {
                switch( o->type )
                {
                    case TYPEOF_st_s:
                    {
                        st_s* string = st_s_create();
                        BLM_TRY( bcore_source_r_parse_em_fa( &src, " #string", string ) );
                        o->default_tp = bcore_const_string_set_st_d( string ); // thread and collision safe
                    }
                    break;

                    default:
                    {
                        BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nCannot assign default value to type '#<sc_t>'", ifnameof( parent_type ), ifnameof( o->type ) ) );
                    }
                }
            }
        }
        else if( o->caps == BCORE_CAPS_LINK_AWARE )
        {
            st_s* name = st_s_create();
            BLM_TRY( bcore_source_r_parse_em_fa( &src, " #name", name ) );
            if( name->size == 0 )
            {
                BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>': Default must be a type name.", ifnameof( parent_type ), name->sc ) );
            }

            tp_t aware_tp = typeof( name->sc );

            if( advanced_checks )
            {
                if( !bcore_flect_exists( aware_tp ) )
                {
                    BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\n'#<sc_t>' has no registered reflection.", ifnameof( parent_type ), name->sc ) );
                }

                const bcore_self_s* self = bcore_flect_get_self( aware_tp );

                if( !bcore_self_s_is_aware( self ) )
                {
                    BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\n'#<sc_t>' is not self-aware.", ifnameof( parent_type ), name->sc ) );
                }

                if( o->type != 0 ) // assume trait
                {
                    st_s* log = st_s_create();
                    if( !bcore_trait_satisfied_type( o->type, aware_tp, log ) )
                    {
                        BLM_DOWN();
                        return bcore_source_a_parse_error_fa
                        (
                            src.o,
                            "Parent '#<sc_t>':\n'#<sc_t>' does not support trait '#<sc_t>' Reason:\n#<sc_t>",
                            ifnameof( parent_type ),
                            name->sc,
                            ifnameof( o->type ),
                            log->sc
                        );
                    }
                    st_s_discard( log );
                }
            }

            st_s_discard( name );

            o->default_tp = aware_tp;
        }
        else
        {
            BLM_TRY
            (
                bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nAssignment of default value possible for:\n"
                                                      "  - single solid static nesting\n"
                                                      "  - strings\n"
                                                      "  - aware link nesting (default must be an aware type with reflection)\n"
                                                      "other nestings cannot receive a default value.", ifnameof( parent_type ) )
            );
        }
    }
    BLM_RETURNV( er_t, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

static er_t bcore_self_item_s_parse_func_src( bcore_self_item_s* o, sr_s src, const bcore_self_s* self, bl_t advanced_checks )
{
    BLM_INIT();
    src = bcore_life_s_push_sr( __bcore_life_curent, sr_cp( src, TYPEOF_bcore_source_s ) );

    tp_t parent_type = self ? self->type : 0;

    st_s* type_name = BLM_CREATE( st_s );
    st_s* item_name = BLM_CREATE( st_s );

    bl_t extend_type_name = false;
    bl_t use_trait = false;
    BLM_TRY( bcore_source_r_parse_em_fa( &src, "#name #?'^' #?':' #name", type_name, &use_trait, &extend_type_name, item_name ) );

    if( extend_type_name )
    {
        if( use_trait )
        {
            if( type_name->size != 0 )
            {
                BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nMisplaced type name '#<sc_t>'", ifnameof( parent_type ), type_name->sc ) );
            }

            if( self && self->trait )
            {
                st_s_copy_sc( type_name, ifnameof( self->trait ) );
            }
            else
            {
                BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nFunction declaration with trait expansion: Parent has no trait.", ifnameof( parent_type ) ) );
            }
        }
        else if( type_name->size == 0 )
        {
//                BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nType name expected.", ifnameof( parent_type ), type_name->sc ) );
        }

        st_s* type_prefix = st_s_clone( type_name );
        st_s_copy_fa( type_name, "#<sc_t>_#<sc_t>", type_prefix->sc, item_name->sc );
        st_s_discard( type_prefix );
    }
    else if( use_trait )
    {
        BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nMisplaced '^'", ifnameof( parent_type ) ) );
    }

    o->type = entypeof( type_name->sc );
    o->name = entypeof( item_name->sc );
    o->caps = BCORE_CAPS_EXTERNAL_FUNC;
    st_s* assign_name = BLM_CREATE( st_s );

    if( bcore_source_r_parse_bl_fa( &src, " #?'=' " ) )
    {
        BLM_TRY( bcore_source_r_parse_em_fa( &src, "#name", assign_name ) );
    }
    else if( parent_type )
    {
        st_s_push_fa( assign_name, "#<sc_t>_#<sc_t>", ifnameof( parent_type ), item_name->sc );
    }

    if( !st_s_equal_sc( assign_name, "" ) )
    {
        tp_t func_tp = entypeof( assign_name->sc );
        if( advanced_checks && !bcore_function_exists( func_tp ) )
        {
            BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nFunction '#<sc_t>' was not registered.", ifnameof( parent_type ), ifnameof( func_tp ) ) );
        }

        o->default_tp = func_tp;
        if( advanced_checks && bcore_trait_exists( func_tp ) )
        {
            if( !bcore_trait_is_of( func_tp, o->type ) )
            {
                BLM_TRY( bcore_source_a_parse_error_fa( src.o, "Parent '#<sc_t>':\nFunction '#<sc_t>' is not of feature '#<sc_t>'.", ifnameof( parent_type ), ifnameof( func_tp ), ifnameof( o->type ) ) );
            }
        }
    }
    BLM_RETURNV( er_t, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_self_item_s_parse_src( bcore_self_item_s* o, sr_s src, const bcore_self_s* self, bl_t advanced_checks )
{
    if( bcore_source_r_parse_bl_fa( &src, " #?w'func' " ) ) // function declaration
    {
        er_t er = bcore_self_item_s_parse_func_src( o, src, self, advanced_checks );
        if( er ) return er;
    }
    else // data type declaration
    {
        er_t er = bcore_self_item_s_parse_data_src( o, src, self, advanced_checks );
        if( er ) return er;
    }

    if( bcore_source_r_parse_bl_fa( &src, " #?'~>'"   ) ) // relating to
    {
        o->flags.f_feature = true;
        o->child_item = bcore_self_item_s_create();
        er_t er = bcore_self_item_s_parse_src( o->child_item, src, NULL, advanced_checks );
        if( er ) return er;
    }
    else
    {
        er_t er = bcore_source_r_parse_em_fa( &src, " ; " );
        if( er ) return er;
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

// returns false in case nothing was written
bl_t bcore_self_item_s_struct_to_sink( const bcore_self_item_s* o, bcore_sink* sink )
{
    sc_t type = ifnameof( o->type );
    sc_t name = ifnameof( o->name );

    bl_t ret = true;

    switch( o->caps )
    {
        case BCORE_CAPS_SOLID_STATIC:
        {
            bcore_sink_a_push_fa( sink, "#<sc_t> #<sc_t>;", type, name );
        }
        break;

        case BCORE_CAPS_LINK_STATIC:
        {
            bcore_sink_a_push_fa( sink, "#<sc_t>* #<sc_t>;", type, name );
        }
        break;

        case BCORE_CAPS_LINK_TYPED:
        {
            if( o->type )
            {
                bcore_sink_a_push_fa( sink, "BCORE_LINK_TYPED_VIRTUAL_S( #<sc_t>, #<sc_t> );", type, name );
            }
            else
            {
                bcore_sink_a_push_fa( sink, "BCORE_LINK_TYPED_S( #<sc_t> );", name );
            }
        }
        break;

        case BCORE_CAPS_LINK_AWARE:
        {
            if( o->type )
            {
                bcore_sink_a_push_fa( sink, "#<sc_t>* #<sc_t>;", type, name );
            }
            else
            {
                bcore_sink_a_push_fa( sink, "vd_t #<sc_t>;", name );
            }
        }
        break;

        case BCORE_CAPS_POINTER:
        {
            if( o->type )
            {
                bcore_sink_a_push_fa( sink, "#<sc_t>* #<sc_t>;", type, name );
            }
            else
            {
                bcore_sink_a_push_fa( sink, "vd_t #<sc_t>;", name );
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
        {
            bcore_sink_a_push_fa( sink, "BCORE_ARRAY_DYN_SOLID_STATIC_S( #<sc_t>, #<sc_t>#<sc_t>);", type, name, name[ 0 ] ? "_ " : "" );
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
        {
            if( o->type )
            {
                bcore_sink_a_push_fa( sink, "BCORE_ARRAY_DYN_SOLID_TYPED_VIRTUAL_S( #<sc_t>, #<sc_t>#<sc_t>);", type, name, name[ 0 ] ? "_ " : "" );
            }
            else
            {
                bcore_sink_a_push_fa( sink, "BCORE_ARRAY_DYN_SOLID_TYPED_S( #<sc_t>#<sc_t>);", name, name[ 0 ] ? "_ " : "" );
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
        {
            bcore_sink_a_push_fa( sink, "BCORE_ARRAY_DYN_LINK_STATIC_S( #<sc_t>, #<sc_t>#<sc_t>);", type, name, name[ 0 ] ? "_ " : "" );
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
        {
            if( o->type )
            {
                bcore_sink_a_push_fa( sink, "BCORE_ARRAY_DYN_LINK_TYPED_VITUAL_S( #<sc_t>, #<sc_t>#<sc_t>);", type, name, name[ 0 ] ? "_ " : "" );
            }
            else
            {
                bcore_sink_a_push_fa( sink, "BCORE_ARRAY_DYN_LINK_TYPED_S( #<sc_t>#<sc_t>);", name, name[ 0 ] ? "_ " : "" );
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
        {
            if( o->type )
            {
                /// We can represent a virtual type via BCORE_ARRAY_DYN_LINK_STATIC
                bcore_sink_a_push_fa( sink, "BCORE_ARRAY_DYN_LINK_STATIC_S( #<sc_t>, #<sc_t>#<sc_t>);", type, name, name[ 0 ] ? "_ " : "" );
            }
            else
            {
                bcore_sink_a_push_fa( sink, "BCORE_ARRAY_DYN_LINK_AWARE_S( #<sc_t>#<sc_t>);", name, name[ 0 ] ? "_ " : "" );
            }
        }
        break;

        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC:
        {
            bcore_sink_a_push_fa( sink, "#<sc_t> #<sc_t>[ #<sz_t> ];", type, name, o->default_sz );
        }
        break;

        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:
        {
            bcore_sink_a_push_fa( sink, "#<sc_t>* #<sc_t>[ #<sz_t> ];", type, name, o->default_sz );
        }
        break;

        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:
        {
            if( o->type )
            {
                bcore_sink_a_push_fa( sink, "#<sc_t>* #<sc_t>[ #<sz_t> ];", type, name, o->default_sz );
            }
            else
            {
                bcore_sink_a_push_fa( sink, "vd_t #<sc_t>[ #<sz_t> ];", name, o->default_sz );
            }
        }
        break;

        case BCORE_CAPS_EXTERNAL_FUNC:
        {
            /* nothing to do */
            ret = false;
        }
        break;

        default:
        {
            ERR_fa( "Unhandled capsulation '#<u2_t>", o->caps );
        }
        break;
    }

    return ret;
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_self_s* self_item_s_create_self( void )
{
    sc_t def =
    "bcore_self_item_s = bcore_inst"
    "{"
        "tp_t type;"    // hash of type
        "tp_t name;"    // hash of name
        "u2_t caps;"    // data encapsulation
        "bcore_flect_flags_s flags;"   // collection of attribute flags
        "umax_t default;" // container of default data
        "bcore_self_item_s => child_item;"
    "}";
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( def, bcore_self_item_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_item_s_init,    "bcore_fp_init",    "init"    );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_item_s_down,    "bcore_fp_down",    "down"    );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_item_s_copy,    "bcore_fp_copy",    "copy"    );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_item_s_move,    "bcore_fp_move",    "move"    );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_item_s_create,  "bcore_fp_create",  "create"  );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_item_s_clone,   "bcore_fp_clone",   "clone"   );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_item_s_discard, "bcore_fp_discard", "discard" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_item_s_cmp,     "bcore_fp_compare", "cmp"     );
    return self;
}

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

typedef struct bcore_self_body_s
{
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            bcore_self_item_s** data;
            uz_t size, space;
        };
    };

    // A complete body enables inst perspective to calculate the
    // full object's size and alignment from the bodies structure.
    // An incomplete body has unspecified trailing elements. (intermediate unspecified elements are not allowed)
    bl_t complete;
} bcore_self_body_s;

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_body_s_init( bcore_self_body_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->complete = true;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_body_s_down( bcore_self_body_s* o )
{
    for( uz_t i = 0; i < o->size; i++ ) bcore_self_item_s_discard( o->data[ i ] );
    bcore_release( o->data );
    o->data  = NULL;
    o->size  = 0;
    o->space = 0;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_body_s_copy( bcore_self_body_s* o, const bcore_self_body_s* src )
{
    for( uz_t i = 0; i < o->size; i++ ) bcore_self_item_s_discard( o->data[ i ] );
    bcore_release( o->data );
    o->data = NULL;
    o->size = 0;
    o->space = 0;

    o->data = bcore_un_alloc( sizeof( bcore_self_item_s* ), o->data, o->space, src->size, &o->space );
    for( uz_t i = 0; i < src->size; i++ ) o->data[ i ] = bcore_self_item_s_clone( src->data[ i ] );

    o->size = src->size;
    o->complete = src->complete;
}

//----------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_self_body_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_self_body_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_self_body_s )

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_body_s_clear( bcore_self_body_s* o )
{
    for( uz_t i = 0; i < o->size; i++ ) bcore_self_item_s_discard( o->data[ i ] );
    bcore_release( o->data );
    o->data  = NULL;
    o->size  = 0;
    o->space = 0;
    o->complete = true;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_item_s* bcore_self_body_s_push_d( bcore_self_body_s* o, bcore_self_item_s* item )
{
    if( o->size == o->space )
    {
        bcore_self_item_s** old_data = o->data;
        o->data = bcore_u_alloc( sizeof( bcore_self_item_s* ), NULL, o->space > 0 ? o->space * 2 : 1, &o->space );
        for( uz_t i = 0; i < o->size; i++ ) o->data[ i ] = old_data[ i ];
        bcore_release( old_data );
    }
    o->data[ o->size ] = item;
    o->size++;
    return o->data[ o->size -1 ];
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_item_s* bcore_self_body_s_push( bcore_self_body_s* o, const bcore_self_item_s* item )
{
    return bcore_self_body_s_push_d( o, bcore_self_item_s_clone( item ) );
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_self_body_s_show( const bcore_self_body_s* o )
{
    st_s* s = st_s_create();
    st_s_pushf( s, "bcore_self_body_s" );
    st_s_pushf( s, "\n{" );
    for( uz_t i = 0; i < o->size; i++ )
    {
        st_s_pushf( s, "\n    data[%lu]:", i );
        st_s_push_st_d( s, st_s_replace_char_sc( bcore_self_item_s_show( o->data[ i ] ), '\n', "\n    " ) );
    }
    st_s_pushf( s, "\n}" );
    return s;
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_self_body_s_parse_src( bcore_self_body_s* o, sr_s src, const bcore_self_s* self, bl_t aware, bl_t advanced_checks )
{
    bcore_life_s* l = bcore_life_s_create();
    src = bcore_life_s_push_sr( l, sr_cp( src, TYPEOF_bcore_source_s ) );
    bcore_self_body_s_clear( o );

    if( aware )
    {
        bcore_self_item_s* item = bcore_self_item_s_create();
        item->type = TYPEOF_aware_t;
        item->name = typeof( "_" );
        item->caps = BCORE_CAPS_SOLID_STATIC;
        bcore_self_body_s_push_d( o, item );
    }

    if( bcore_source_r_parse_bl_fa( &src, " #?'{' " ) )
    {
        while( !bcore_source_r_parse_bl_fa( &src, " #?'}' " ) )
        {
            if( bcore_source_r_parse_bl_fa( &src, " #?'...'" ) )
            {
                o->complete = false;
                break;
            }
            bcore_self_item_s* item = bcore_self_item_s_create();
            if( bcore_self_item_s_parse_src( item, src, self, advanced_checks ) )
            {
                bcore_error_pop_all_to_stderr();
                bcore_abort();
            }

            if( item->type == typeof( "aware_t" ) )
            {
                if( o->size > 0 )
                {
                    bcore_source_r_parse_err_fa( &src, "'aware_t' declares self-awareness. It can only be first element in body. Use type 'tp_t' elsewhere." );
                }
            }

            bcore_self_body_s_push_d( o, item );
        }
    }


    bcore_life_s_discard( l );
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_self_body_s_fold_tp( const bcore_self_body_s* o, tp_t tp )
{
    for( uz_t i = 0; i < o->size; i++ ) tp = bcore_self_item_s_fold_tp( o->data[ i ], tp );
    return tp;
}

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_self_body_s_cmp( const bcore_self_body_s* o1, const bcore_self_body_s* o2 )
{
    if( o1->size != o2->size ) return ( o1->size < o2->size ) ? 1 : -1;
    for( uz_t i = 0; i < o1->size; i++ )
    {
        s2_t c = bcore_self_item_s_cmp( o1->data[ i ], o2->data[ i ] );
        if( c != 0 ) return c;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_body_s_set_complete( bcore_self_body_s* o, bl_t complete )
{
    o->complete = complete;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_self_body_s_get_complete( const bcore_self_body_s* o )
{
    return o->complete;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_body_s_check_integrity( const bcore_self_body_s* o )
{
    for( uz_t i = 0; i < o->size; i++ ) bcore_self_item_s_check_integrity( o->data[ i ] );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_s_forward_struct_to_sink( const bcore_self_s* o, sz_t indent, bcore_sink* sink )
{
    sc_t name = nameof( o->type );
    if( !name ) ERR_fa( "Type of reflection has no registered name." );
    bcore_sink_a_push_fa( sink, "#rn{ }typedef struct #<sc_t> #<sc_t>;\n", indent, name, name );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_s_struct_body_to_sink( const bcore_self_s* o, sz_t indent, bcore_sink* sink )
{
    sc_t name = nameof( o->type );
    st_s* buf = st_s_create();
    if( !name ) ERR_fa( "Type of reflection has no registered name." );
    bcore_sink_a_push_fa( sink, "#rn{ }{\n", indent );
    if( o->body )
    {
        for( sz_t i = 0; i < o->body->size; i++ )
        {
            const bcore_self_item_s* item = o->body->data[ i ];
            st_s_clear( buf );
            if( bcore_self_item_s_struct_to_sink( item, ( bcore_sink* )buf ) )
            {
                bcore_sink_a_push_fa( sink, "#rn{ }    #<sc_t>\n", indent, buf->sc );
            }
        }
    }
    st_s_discard( buf );
    bcore_sink_a_push_fa( sink, "#rn{ }}", indent );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_s_struct_body_to_sink_newline_escaped( const bcore_self_s* o, sz_t indent, bcore_sink* sink )
{
    sc_t name = nameof( o->type );
    st_s* buf = st_s_create();
    if( !name ) ERR_fa( "Type of reflection has no registered name." );
    bcore_sink_a_push_fa( sink, "#rn{ }{ \\\n", indent );
    if( o->body )
    {
        for( sz_t i = 0; i < o->body->size; i++ )
        {
            const bcore_self_item_s* item = o->body->data[ i ];
            st_s_clear( buf );
            if( bcore_self_item_s_struct_to_sink( item, ( bcore_sink* )buf ) )
            {
                bcore_sink_a_push_fa( sink, "#rn{ }    #<sc_t> \\\n", indent, buf->sc );
            }
        }
    }
    st_s_discard( buf );
    bcore_sink_a_push_fa( sink, "#rn{ }}", indent );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_s_struct_body_to_sink_single_line( const bcore_self_s* o, bcore_sink* sink )
{
    sc_t name = nameof( o->type );
    if( !name ) ERR_fa( "Type of reflection has no registered name." );
    bcore_sink_a_push_fa( sink, "{" );
    if( o->body )
    {
        for( sz_t i = 0; i < o->body->size; i++ )
        {
            const bcore_self_item_s* item = o->body->data[ i ];
            bcore_self_item_s_struct_to_sink( item, sink );
        }
    }
    bcore_sink_a_push_fa( sink, "}" );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_s_struct_to_sink( const bcore_self_s* o, sz_t indent, bcore_sink* sink )
{
    sc_t name = nameof( o->type );
    if( !name ) ERR_fa( "Type of reflection has no registered name." );

    bcore_sink_a_push_fa( sink, "#rn{ }typedef struct #<sc_t>\n", indent, name );
    bcore_self_s_struct_body_to_sink( o, indent, sink );
    bcore_sink_a_push_fa( sink, " #<sc_t>;\n", name );
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_self_s* flect_body_s_create_self( void )
{
    sc_t def =
    "bcore_self_body_s = bcore_inst"
    "{"
        "bcore_self_item_s => [] arr;"
        "bl_t complete;"
    "}";
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( def, bcore_self_body_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_body_s_init,    "bcore_fp_init",    "init"    );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_body_s_down,    "bcore_fp_down",    "down"    );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_body_s_copy,    "bcore_fp_copy",    "copy"    );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_body_s_create,  "bcore_fp_create",  "create"  );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_body_s_clone,   "bcore_fp_clone",   "clone"   );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_body_s_discard, "bcore_fp_discard", "discard" );
    return self;
}

/**********************************************************************************************************************/
// bcore_self_s

void bcore_self_s_init( bcore_self_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_self_s;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_s_down( bcore_self_s* o )
{
    bcore_self_body_s_discard( o->body );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_s_copy( bcore_self_s* o, const bcore_self_s* src )
{
    bcore_self_body_s_discard( o->body );
    bcore_memcpy( o, src, sizeof( bcore_self_s ) );
    o->body = bcore_self_body_s_clone( src->body );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_create()
{
    bcore_self_s* o = bcore_alloc( NULL, sizeof( bcore_self_s ) );
    bcore_self_s_init( o );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_clone( const bcore_self_s* o )
{
    bcore_self_s* dst = bcore_self_s_create();
    bcore_self_s_copy( dst, o );
    return dst;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_s_discard( bcore_self_s* o )
{
    if( !o ) return;
    bcore_release_obj( ( fp_t )bcore_self_s_down, o );
}

//----------------------------------------------------------------------------------------------------------------------

uz_t bcore_self_s_items_size( const bcore_self_s* o )
{
    return o->body ? o->body->size : 0;
}

//----------------------------------------------------------------------------------------------------------------------

const bcore_self_item_s* bcore_self_s_get_item( const bcore_self_s* o, uz_t index )
{
    ASSERT( o->body );
    ASSERT( o->body->size > index );
    return o->body->data[ index ];
}

//----------------------------------------------------------------------------------------------------------------------

const bcore_self_item_s* bcore_self_s_get_item_by_name( const bcore_self_s* o, tp_t name )
{
    if( !o->body ) return NULL;
    ASSERT( o->body );
    for( uz_t i = 0; i < o->body->size; i++ )
    {
        const bcore_self_item_s* item = o->body->data[ i ];
        if( item->name == name ) return item;
    }
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

const bcore_self_item_s* bcore_self_s_get_first_array_item( const bcore_self_s* o )
{
    if( !o->body ) return NULL;
    ASSERT( o->body );
    for( uz_t i = 0; i < o->body->size; i++ )
    {
        const bcore_self_item_s* item = o->body->data[ i ];
        if( bcore_flect_caps_is_array( item->caps ) ) return item;
    }
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

const bcore_self_item_s* bcore_self_s_get_first_anonymous_array_item( const bcore_self_s* o )
{
    if( !o->body ) return NULL;
    ASSERT( o->body );
    for( uz_t i = 0; i < o->body->size; i++ )
    {
        const bcore_self_item_s* item = o->body->data[ i ];
        if( item->name == TYPEOF_ && bcore_flect_caps_is_array( item->caps ) ) return item;
    }
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_item_s* bcore_self_s_push_d( bcore_self_s* o, bcore_self_item_s* item )
{
    if( !o->body ) o->body = bcore_self_body_s_create();
    return bcore_self_body_s_push_d( o->body, item );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_item_s* bcore_self_s_push( bcore_self_s* o, const bcore_self_item_s* item )
{
    return bcore_self_s_push_d( o, bcore_self_item_s_clone( item ) );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_item_s* bcore_self_s_push_func( bcore_self_s* o, sc_t fname, fp_t func, sc_t type, sc_t name )
{
    return bcore_self_s_push_d( o, bcore_self_item_s_create_func( fname, func, type, name ) );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_item_s* bcore_self_s_push_ns_func( bcore_self_s* o, fp_t func, sc_t type, sc_t name )
{
    st_s* fname = st_s_create_fa( "#<sc_t>_#<sc_t>", nameof( o->type ), name );
    bcore_self_item_s* ret = bcore_self_s_push_d( o, bcore_self_item_s_create_func( fname->sc, func, type, name ) );
    st_s_discard( fname );
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_item_s* bcore_self_s_push_ns_amoeba( bcore_self_s* o, bcore_amoebic_t func, sc_t name )
{
    return bcore_self_s_push_ns_func( o, ( fp_t )func, "ap_t", name );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_item_s* bcore_self_s_push_fp_set( bcore_self_s* o, bcore_fp_set func, sc_t name )
{
    return bcore_self_s_push_ns_func( o, (fp_t )func, "bcore_fp_set", name );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_item_s* bcore_self_s_push_fp_get( bcore_self_s* o, bcore_fp_get func, sc_t name )
{
    return bcore_self_s_push_ns_func( o, (fp_t )func, "bcore_fp_get", name );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_s_init_plain( bcore_self_s* o, tp_t type, tp_t trait, uz_t size, uz_t align )
{
    bcore_self_s_init( o );
    o->type  = type;
    o->trait = trait;
    o->size  = size;
    o->align = align;
    if( o->body ) bcore_self_body_s_discard( o->body );
    o->body = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_self_s_show( const bcore_self_s* o )
{
    st_s* s = st_s_create();
    st_s_pushf( s, "bcore_self_s" );
    st_s_pushf( s, "\n{" );
    st_s_pushf( s, "\n    type:  %x '%s'", o->type, ifnameof( o->type ) );
    st_s_pushf( s, "\n    size:  %zu", o->size );
    st_s_pushf( s, "\n    body: " );
    if( o->body ) st_s_push_st_d( s, st_s_replace_char_sc( bcore_self_body_s_show( o->body ), '\n', "\n    " ) );
    st_s_pushf( s, "\n}" );
    return s;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_create_plain( tp_t type, tp_t trait, uz_t size, uz_t align )
{
    bcore_self_s* o = bcore_self_s_create();
    bcore_self_s_init_plain( o, type, trait, size, align );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_create_array_dyn_solid_static( tp_t item_type )
{
    bcore_self_s* o = bcore_self_s_create();
    bcore_self_item_s* item = bcore_self_item_s_create();
    item->caps = BCORE_CAPS_ARRAY_DYN_SOLID_STATIC;
    item->type = item_type;
    bcore_self_s_push( o, item );
    bcore_self_item_s_discard( item );
    o->type = bcore_self_body_s_fold_tp( o->body, bcore_tp_init() );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_create_array_dyn_link_static( tp_t item_type )
{
    bcore_self_s* o = bcore_self_s_create();
    bcore_self_item_s* item = bcore_self_item_s_create();
    item->caps = BCORE_CAPS_ARRAY_DYN_LINK_STATIC;
    item->type = item_type;
    bcore_self_s_push( o, item );
    bcore_self_item_s_discard( item );
    o->type = bcore_self_body_s_fold_tp( o->body, bcore_tp_init() );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_create_array_fix_solid_static( tp_t item_type, uz_t size )
{
    bcore_self_s* o = bcore_self_s_create();
    bcore_self_item_s* item = bcore_self_item_s_create();
    item->caps = BCORE_CAPS_ARRAY_FIX_SOLID_STATIC;
    item->type = item_type;
    item->array_fix_size = size;
    bcore_self_s_push( o, item );
    bcore_self_item_s_discard( item );
    o->type = bcore_self_body_s_fold_tp( o->body, bcore_tp_init() );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_create_array_fix_link_static( tp_t item_type, uz_t size )
{
    bcore_self_s* o = bcore_self_s_create();
    bcore_self_item_s* item = bcore_self_item_s_create();
    item->caps = BCORE_CAPS_ARRAY_FIX_LINK_STATIC;
    item->type = item_type;
    item->array_fix_size = size;
    bcore_self_s_push( o, item );
    bcore_self_item_s_discard( item );
    o->type = bcore_self_body_s_fold_tp( o->body, bcore_tp_init() );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_create_array_fix_link_aware( uz_t size )
{
    bcore_self_s* o = bcore_self_s_create();
    bcore_self_item_s* item = bcore_self_item_s_create();
    item->caps = BCORE_CAPS_ARRAY_FIX_LINK_AWARE;
    item->type = 0;
    item->array_fix_size = size;
    bcore_self_s_push( o, item );
    bcore_self_item_s_discard( item );
    o->type = bcore_self_body_s_fold_tp( o->body, bcore_tp_init() );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_parse_src( sr_s src, uz_t size_of, uz_t align_of, sc_t namespace, bl_t advanced_checks )
{
    bcore_life_s* l = bcore_life_s_create();
    src = bcore_life_s_push_sr( l, sr_cp( src, TYPEOF_bcore_source_s ) );
    bcore_self_s* o = bcore_self_s_create();

    st_s* identifier = st_s_create_l( l );

    if( bcore_source_r_parse_bl_fa( &src, " #?':'" ) )
    {
        ASSERT( namespace );
        st_s* name = st_s_create();
        bcore_source_r_parse_fa( &src, " #name", name );
        st_s_copy_fa( identifier, "#<sc_t>_#<sc_t>", namespace, name->sc );
        st_s_discard( name );
    }
    else
    {
        bcore_source_r_parse_fa( &src, " #name", identifier );
    }

    o->type = ( identifier->size > 0 ) ? entypeof( identifier->sc ) : 0;
    if( o->type ) bcore_source_r_parse_fa( &src, " =" );

    bl_t aware = bcore_source_r_parse_bl_fa( &src, " #?w'aware'" );

    bcore_source_r_parse_fa( &src, " #name", identifier );
    tp_t type2 = ( identifier->size > 0 ) ? entypeof( identifier->sc ) : 0;

    bcore_source_r_parse_fa( &src, " #name", identifier );
    tp_t type3 = ( identifier->size > 0 ) ? entypeof( identifier->sc ) : 0;

    if( aware || bcore_source_r_parse_bl_fa( &src, " #=?'{'" ) )
    {
        o->trait  = type2 ? type2 : typeof( "bcore_inst" );
        o->parent = type3;
        o->body = bcore_self_body_s_create();
        bcore_self_body_s_parse_src( o->body, src, o, aware, advanced_checks );

        if( !o->type )
        {
            o->type = bcore_self_body_s_fold_tp( o->body, bcore_tp_init() );
        }
    }
    else
    {
        if( !o->type ) bcore_source_r_parse_errf( &src, "Anonymous types need a body." );
        const bcore_self_s* self_l = bcore_flect_try_self( type2 );
        if( !self_l ) bcore_source_r_parse_errf( &src, "Type %s not defined.", ifnameof( type2 ) );
        o->trait = self_l->trait;
        o->parent = self_l->parent;
        o->body = bcore_self_body_s_clone( self_l->body );
        o->size = self_l->size;
    }

    o->size = size_of;
    o->align = align_of;
    bcore_life_s_discard( l );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_parse_source( bcore_source* source, uz_t size_of, uz_t align_of, sc_t namespace, bl_t advanced_checks )
{
    return bcore_self_s_parse_src( sr_awd( source ), size_of, align_of, namespace, advanced_checks );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_build_parse_src( sr_s src, uz_t size_of, uz_t align_of )
{
    return bcore_self_s_parse_src( src, size_of, align_of, NULL, true );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_build_parse_source( bcore_source* source, uz_t size_of, uz_t align_of )
{
    return bcore_self_s_build_parse_src( sr_awd( source ), size_of, align_of );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_self_s_build_parse_sc( sc_t text, uz_t size_of, uz_t align_of )
{
    return bcore_self_s_build_parse_src( sr_asd( st_s_create_weak_sc( text ) ), size_of, align_of );
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_self_s_fold_tp( const bcore_self_s* o, tp_t tp )
{
    tp = bcore_tp_fold_tp( tp, o->type );
    tp = bcore_tp_fold_tp( tp, o->trait );
    tp = bcore_tp_fold_tp( tp, o->parent );
    if( o->body ) tp = bcore_self_body_s_fold_tp( o->body, tp );
    return tp;
}

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_self_s_cmp( const bcore_self_s* o1, const bcore_self_s* o2 )
{
    if( o1->type   != o2->type   ) return ( o1->type   < o2->type   ) ? 1 : -1;
    if( o1->trait  != o2->trait  ) return ( o1->trait  < o2->trait  ) ? 1 : -1;
    if( o1->parent != o2->parent ) return ( o1->parent < o2->parent ) ? 1 : -1;
    if( o1->body   != o2->body   )
    {
        if( !o1->body ) return  1;
        if( !o2->body ) return -1;
        return bcore_self_body_s_cmp( o1->body, o2->body );
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

fp_t bcore_self_s_try_external_fp( const bcore_self_s* o, tp_t type, tp_t name )
{
    if( !o->body ) return NULL;
    const bcore_self_body_s* body = o->body;
    for( uz_t i = 0; i < body->size; i++ )
    {
        const bcore_self_item_s* item = body->data[ i ];
        if( ( item->caps == BCORE_CAPS_EXTERNAL_FUNC ) &&
            ( type == 0 || item->type == type        ) &&
            ( name == 0 || item->name == name        ) )
        {
            if( item->default_tp )
            {
                fp_t fp = bcore_function_get( item->default_tp );
                if( !fp ) ERR( "Function '%s' (%"PRItp_t") not registered in function manager.", ifnameof(item->default_tp ), item->default_tp );
                return fp;
            }
            else
            {
                ERR( "Object '%s': Function '%s' declared but not assigned.", ifnameof( o->type ), ifnameof( name ) );
            }
        }
    }
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

fp_t bcore_self_s_get_external_fp( const bcore_self_s* o, tp_t type, tp_t name )
{
    fp_t fp = bcore_self_s_try_external_fp( o, type, name );
    if( !fp ) ERR( "Function '%s' of name '%s' not found in reflection '%s'.", ifnameof( type ), ifnameof( name ), ifnameof(o->type) );
    return fp;
}

//----------------------------------------------------------------------------------------------------------------------

bool bcore_self_s_is_aware( const bcore_self_s* o )
{
    if( !o->body ) return false;
    const bcore_self_body_s* body = o->body;
    if( body->size == 0 ) return false;
    return body->data[ 0 ]->type == TYPEOF_aware_t || body->data[ 0 ]->type == TYPEOF_bcore_spect_header_s;
}

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_self_s_get_static( const bcore_self_s* o, tp_t type, tp_t name )
{
    bcore_fp_create fp = ( bcore_fp_create )bcore_self_s_try_external_fp( o, type, name );
    if( !fp ) ERR( "Static creator '%s' of name '%s' not found in reflection '%s'.", ifnameof( type ), ifnameof( name ), ifnameof(o->type) );
    return fp();
}

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_self_s_try_static( const bcore_self_s* o, tp_t type, tp_t name )
{
    bcore_fp_create fp = ( bcore_fp_create )bcore_self_s_try_external_fp( o, type, name );
    return fp ? fp() : NULL;
}

//----------------------------------------------------------------------------------------------------------------------

vc_t bcore_self_s_try_const( const bcore_self_s* o, tp_t type, tp_t name ) // returns NULL when not found
{
    if( !o->body ) return NULL;
    const bcore_self_body_s* body = o->body;
    for( uz_t i = 0; i < body->size; i++ )
    {
        const bcore_self_item_s* item = body->data[ i ];
        if( ( item->caps == BCORE_CAPS_SOLID_STATIC ) &&
            ( type == 0 || item->type == type       ) &&
            ( name == 0 || item->name == name       ) )
        {
            return &item->default_u3; // u3 maximally aligned and representative for all default types
        }
    }
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

vc_t bcore_self_s_get_const( const bcore_self_s* o, tp_t type, tp_t name )
{
    vc_t vc = bcore_self_s_try_const( o, type, name );
    if( !vc ) ERR( "'const %s %s' not found in reflection '%s'.", ifnameof( type ), ifnameof( name ), ifnameof(o->type) );
    return vc;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_self_s_check_integrity( const bcore_self_s* o )
{
    if( !bcore_trait_supported( o->trait, o, NULL ) )
    {
        st_s* log = st_s_create();
        bcore_trait_supported( o->trait, o, log );
        ERR( "Reflection '%s' is of trait '%s' but does not support it.\nReason: %s\n", ifnameof( o->type ), ifnameof( o->trait ), log->sc );
        st_s_discard( log );
    }
    if( o->body ) bcore_self_body_s_check_integrity( o->body );
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_self_s* flect_self_s_create_self( void )
{
    sc_t def =
    "bcore_self_s = bcore_inst"
    "{"
        "aware_t _;"
        "tp_t type;"
        "tp_t trait;"
        "tp_t parent;"
        "uz_t size;"
        "uz_t align;"
        "bcore_self_body_s => body;"
    "}";
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( def, bcore_self_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_s_init,         "bcore_fp_init",         "init"         );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_s_down,         "bcore_fp_down",         "down"         );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_s_copy,         "bcore_fp_copy",         "copy"         );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_s_create,       "bcore_fp_create",       "create"       );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_s_clone,        "bcore_fp_clone",        "clone"        );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_self_s_discard,      "bcore_fp_discard",      "discard"      );
    return self;
}

/**********************************************************************************************************************/
// map of functions creating a self-reflection

typedef struct creator_map_s
{
    bcore_hmap_u3vd_s* hmap;
    bcore_mutex_s mutex;
} creator_map_s;

//----------------------------------------------------------------------------------------------------------------------

static void creator_map_s_init( creator_map_s* o )
{
    o->hmap = bcore_hmap_u3vd_s_create();
    bcore_mutex_s_init( &o->mutex );
}

//----------------------------------------------------------------------------------------------------------------------

static void creator_map_s_down( creator_map_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    bcore_hmap_u3vd_s_discard( o->hmap );
    bcore_mutex_s_unlock( &o->mutex );
    bcore_mutex_s_down( &o->mutex );
}

//----------------------------------------------------------------------------------------------------------------------

static creator_map_s* creator_map_s_create()
{
    creator_map_s* o = bcore_alloc( NULL, sizeof( creator_map_s ) );
    creator_map_s_init( o );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

static void creator_map_s_discard( creator_map_s* o )
{
    if( !o ) return;
    bcore_release_obj( ( fp_t )creator_map_s_down, o );
}

/**********************************************************************************************************************/
// map of self reflections

typedef struct self_map_s
{
    bcore_hmap_u3vd_s* hmap;
    bcore_mutex_s mutex;
} self_map_s;

//----------------------------------------------------------------------------------------------------------------------

static void self_map_s_init( self_map_s* o )
{
    o->hmap = bcore_hmap_u3vd_s_create();
    bcore_mutex_s_init( &o->mutex );
}

//----------------------------------------------------------------------------------------------------------------------

static void self_map_s_down( self_map_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    if( o->hmap )
    {
        // We manually detach objects here to keep hmap from invoking
        // perspective management, which may (already) be down at this point.
        uz_t hmap_size = bcore_hmap_u3vd_s_size( o->hmap );
        for( uz_t i = 0; i < hmap_size; i++ )
        {
            if( bcore_hmap_u3vd_s_idx_holds( o->hmap, i ) )
            {
                tp_t key = bcore_hmap_u3vd_s_idx_key( o->hmap, i );
                bcore_self_s_discard( bcore_hmap_u3vd_s_detach_h( o->hmap, key ) );
            }
        }
        bcore_hmap_u3vd_s_discard( o->hmap );
    }
    bcore_mutex_s_unlock( &o->mutex );
    bcore_mutex_s_down( &o->mutex );
}

//----------------------------------------------------------------------------------------------------------------------

static self_map_s* self_map_s_create()
{
    self_map_s* o = bcore_alloc( NULL, sizeof( self_map_s ) );
    self_map_s_init( o );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

static void self_map_s_discard( self_map_s* o )
{
    if( !o ) return;
    bcore_release_obj( ( fp_t )self_map_s_down, o );
}

/**********************************************************************************************************************/
/// Global reflection manager (thread safe)

static self_map_s*    self_map_s_g    = NULL;
static creator_map_s* creator_map_s_g = NULL;

//----------------------------------------------------------------------------------------------------------------------

void bcore_create_flect_maps()
{
    if( !self_map_s_g    ) self_map_s_g    = self_map_s_create();
    if( !creator_map_s_g ) creator_map_s_g = creator_map_s_create();
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_discard_flect_maps()
{
    if( self_map_s_g )
    {
        self_map_s_discard( self_map_s_g );
        self_map_s_g = NULL;
    }
    if( creator_map_s_g )
    {
        creator_map_s_discard( creator_map_s_g );
        creator_map_s_g = NULL;
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void flect_open()
{
    static bcore_once_s flag = bcore_once_init;
    bcore_once_s_run( &flag, bcore_create_flect_maps );
}

//----------------------------------------------------------------------------------------------------------------------

static void flect_close()
{
    bcore_discard_flect_maps();
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_flect_exists( tp_t type )
{
    assert( self_map_s_g != NULL );
    bl_t exists = false;

    bcore_mutex_s_lock( &self_map_s_g->mutex );
    exists = bcore_hmap_u3vd_s_exists( self_map_s_g->hmap, type );
    bcore_mutex_s_unlock( &self_map_s_g->mutex );
    if( exists ) return true;

    assert( creator_map_s_g != NULL );
    bcore_mutex_s_lock( &creator_map_s_g->mutex );
    exists = bcore_hmap_u3vd_s_exists( creator_map_s_g->hmap, type );
    bcore_mutex_s_unlock( &creator_map_s_g->mutex );
    return exists;
}

//----------------------------------------------------------------------------------------------------------------------

sz_t bcore_flect_size()
{
    sz_t count = 0;
    bcore_mutex_s_lock( &creator_map_s_g->mutex );
    {
        sz_t size = bcore_hmap_u3vd_s_size( creator_map_s_g->hmap );
        for( sz_t i = 0; i < size; i++ )
        {
            tp_t type = bcore_hmap_u3vd_s_idx_key( creator_map_s_g->hmap, i );
            if( type ) count++;
        }
    }

    bcore_mutex_s_lock( &self_map_s_g->mutex );
    {
        sz_t size = bcore_hmap_u3vd_s_size( self_map_s_g->hmap );
        for( sz_t i = 0; i < size; i++ )
        {
            tp_t type = bcore_hmap_u3vd_s_idx_key( self_map_s_g->hmap, i );
            if( type && !bcore_hmap_u3vd_s_exists( creator_map_s_g->hmap, type ) )
            {
                 count++;
            }
        }
    }

    bcore_mutex_s_unlock( &creator_map_s_g->mutex );
    bcore_mutex_s_unlock( &self_map_s_g->mutex );

    return count;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_flect_push_all_types( bcore_arr_tp_s* arr )
{
    bcore_mutex_s_lock( &creator_map_s_g->mutex );
    {
        sz_t size = bcore_hmap_u3vd_s_size( creator_map_s_g->hmap );
        for( sz_t i = 0; i < size; i++ )
        {
            tp_t type = bcore_hmap_u3vd_s_idx_key( creator_map_s_g->hmap, i );
            if( type ) bcore_arr_tp_s_push( arr, type );
        }
    }

    bcore_mutex_s_lock( &self_map_s_g->mutex );
    {
        sz_t size = bcore_hmap_u3vd_s_size( self_map_s_g->hmap );
        for( sz_t i = 0; i < size; i++ )
        {
            tp_t type = bcore_hmap_u3vd_s_idx_key( self_map_s_g->hmap, i );
            if( type && !bcore_hmap_u3vd_s_exists( creator_map_s_g->hmap, type ) )
            {
                bcore_arr_tp_s_push( arr, type );
            }
        }
    }

    bcore_mutex_s_unlock( &creator_map_s_g->mutex );
    bcore_mutex_s_unlock( &self_map_s_g->mutex );
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_flect_set_trait( const bcore_self_s* self )
{
    bcore_trait_set( self->type, self->trait );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_self_s* bcore_flect_try_d_self( tp_t type )
{
    // try reflection registry
    assert( self_map_s_g != NULL );
    bcore_mutex_s_lock( &self_map_s_g->mutex );
    vd_t* p_val = bcore_hmap_u3vd_s_get( self_map_s_g->hmap, type );
    bcore_mutex_s_unlock( &self_map_s_g->mutex );
    if( p_val ) return *p_val;

    // try constructing self by reflection creator
    bcore_self_s* self = NULL;
    assert( creator_map_s_g != NULL );

    bcore_mutex_s_lock( &creator_map_s_g->mutex );
    fp_t* p_func = bcore_hmap_u3vd_s_getf( creator_map_s_g->hmap, type );
    bcore_mutex_s_unlock( &creator_map_s_g->mutex );

    if( p_func )
    {
        self = ( ( bcore_flect_create_self_fp )*p_func )();
        bcore_self_s* discard_self = NULL; // in case multiple threads land here, redundant creations are discarded again

        if( self->type != type )
        {
            ERR( "Type of created reflection '%s' differs from requested type '%s'.\n"
                 "Requested type was probably mapped to the wrong self-creator during registration.\n",
                 ifnameof( self->type ), ifnameof( type ) );
        }

        bcore_self_s_check_integrity( self );

        bcore_mutex_s_lock( &self_map_s_g->mutex );
        bcore_flect_set_trait( self );

        if( bcore_hmap_u3vd_s_exists( self_map_s_g->hmap, type ) )
        {
            // If this reflection has meanwhile been registered by another thread,
            // kill current self and retrieve registered version
            discard_self = self;
            self = *bcore_hmap_u3vd_s_get( self_map_s_g->hmap, type );
        }
        else
        {
            // else: register current self
            bcore_hmap_u3vd_s_set( self_map_s_g->hmap, type, self, true );
        }
        bcore_mutex_s_unlock( &self_map_s_g->mutex );

        if( discard_self ) bcore_self_s_discard( discard_self );
    }

    return self;
}

//----------------------------------------------------------------------------------------------------------------------

const bcore_self_s* bcore_flect_try_self( tp_t type )
{
    return bcore_flect_try_d_self( type );
}

//----------------------------------------------------------------------------------------------------------------------

const bcore_self_s* bcore_flect_get_self( tp_t type )
{
    const bcore_self_s* self = bcore_flect_try_self( type );
    if( !self )
    {
        const char* name = bcore_name_try_name( type );
        if( !name )
        {
            ERR( "type '0x%x'; has neither name nor self-reflection", type );
        }
        else
        {
            ERR( "type '%s' has no self-reflection", name );
        }
    }
    return self;
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_flect_define_self_d( bcore_self_s* self )
{
    assert( self_map_s_g != NULL );
    bcore_self_s_check_integrity( self );
    tp_t type = self->type;
    bcore_mutex_s_lock( &self_map_s_g->mutex );
    if( bcore_hmap_u3vd_s_exists( self_map_s_g->hmap, type ) ) ERR( "'%s' (%"PRItp_t") is already defined", ifnameof( type ), type );
    bcore_flect_set_trait( self );
    bcore_hmap_u3vd_s_set( self_map_s_g->hmap, type, self, true );
    bcore_mutex_s_unlock( &self_map_s_g->mutex );
    return self->type;
}

//----------------------------------------------------------------------------------------------------------------------

static tp_t flect_define_self_rentrant_d( bcore_self_s* self )
{
    assert( self_map_s_g != NULL );
    bcore_self_s_check_integrity( self );
    tp_t type = self->type;
    bcore_mutex_s_lock( &self_map_s_g->mutex );
    bcore_flect_set_trait( self );
    if( !bcore_hmap_u3vd_s_exists( self_map_s_g->hmap, type ) ) bcore_hmap_u3vd_s_set( self_map_s_g->hmap, type, self, true );
    bcore_mutex_s_unlock( &self_map_s_g->mutex );
    return self->type;
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_flect_define_self_c( const bcore_self_s* self )
{
    return bcore_flect_define_self_d( bcore_self_s_clone( self ) );
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_flect_define_parse_src( sr_s src )
{
    return bcore_flect_define_self_d( bcore_self_s_build_parse_src( src, 0, 0 ) );
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_flect_define_parse_sc( sc_t sc )
{
    return bcore_flect_define_parse_src( sr_asd( st_s_create_weak_sc( sc ) ) );
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_flect_define_parse_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s* string = st_s_createvf( format, args );
    va_end( args );
    tp_t type = bcore_flect_define_parse_sc( string->sc );
    st_s_discard( string );
    return type;
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_flect_type_self_d( bcore_self_s* self )
{
    const bcore_self_s* self_l = bcore_flect_try_self( self->type );
    if( self_l )
    {
        if( bcore_self_s_cmp( self_l, self ) != 0 )
        {
            ERR( "Attempt to register a different reflection under type = '%s' (%"PRItp_t")\nRegistered:\n%s\nNew:\n%s\n",
                 ifnameof( self->type ), self->type,
                 bcore_self_s_show( self_l )->sc,
                 bcore_self_s_show( self )->sc );
        }
        bcore_self_s_discard( self );
        return self_l->type;
    }
    else
    {
        flect_define_self_rentrant_d( self );
        return self->type;
    }
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_flect_type_self_c( const bcore_self_s* self )
{
    return bcore_flect_type_self_d( bcore_self_s_clone( self ) );
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_flect_type_parse_src( sr_s src )
{
    return bcore_flect_type_self_d( bcore_self_s_build_parse_src( src, 0, 0 ) );
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_flect_type_parse_sc( sc_t sc )
{
    return bcore_flect_type_parse_src( sr_asd( st_s_create_weak_sc( sc ) ) );
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_flect_type_parse_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s* string = st_s_create_fv( format, args );
    va_end( args );
    tp_t type = bcore_flect_type_parse_sc( string->sc );
    st_s_discard( string );
    return type;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_flect_define_creator( tp_t type, bcore_flect_create_self_fp creator )
{
    assert( creator_map_s_g != NULL );
    bcore_mutex_s_lock( &creator_map_s_g->mutex );
    if( bcore_hmap_u3vd_s_exists( creator_map_s_g->hmap, type ) ) ERR( "'%s' (%"PRItp_t") is already defined", ifnameof( type ), type );
    bcore_hmap_u3vd_s_setf( creator_map_s_g->hmap, type, ( fp_t )creator );
    bcore_mutex_s_unlock( &creator_map_s_g->mutex );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_flect_push_item_d( tp_t type, bcore_self_item_s* item )
{
    bool allowed = false;
    if( item->caps == BCORE_CAPS_EXTERNAL_FUNC ) allowed = true;
    if( item->flags.f_const                    ) allowed = true;
    if( !allowed )
    {
        WRN_fa( "Extending type '#<sc_t>': item configuration is not allowed.", ifnameof( type ) );
        return;
    }

    bcore_self_s* self = bcore_flect_try_d_self( type );

    if( nameof( self->type ) == NULL )
    {
        WRN_fa( "Extending anonymous type is not allowed.", ifnameof( type ) );
        return;
    }

    if( self )
    {
        bcore_mutex_s_lock( &self_map_s_g->mutex );
        bcore_self_s_push_d( self, item );
        bcore_mutex_s_unlock( &self_map_s_g->mutex );
        bcore_self_s_check_integrity( self );
    }
    else
    {
        WRN_fa( "Extending type '#<sc_t>': Type is not defined.", ifnameof( type ) );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_flect_push_ns_func( tp_t type, fp_t func, sc_t func_type, sc_t func_name )
{
    bcore_self_s* self = bcore_flect_try_d_self( type );

    if( nameof( self->type ) == NULL )
    {
        WRN_fa( "Extending anonymous type is not allowed.", ifnameof( type ) );
        return;
    }

    if( self )
    {
        bcore_mutex_s_lock( &self_map_s_g->mutex );
        bcore_self_s_push_ns_func( self, func, func_type, func_name );
        bcore_mutex_s_unlock( &self_map_s_g->mutex );
        bcore_self_s_check_integrity( self );
    }
    else
    {
        WRN_fa( "Extending type '#<sc_t>': Type is not defined.", ifnameof( type ) );
    }
}

//----------------------------------------------------------------------------------------------------------------------
// debugging

void bcore_flect_parse_all_flects()
{
    bcore_arr_tp_s* type_arr = bcore_arr_tp_s_create();
    bcore_flect_push_all_types( type_arr );
    for( sz_t i = 0; i < type_arr->size; i++ ) bcore_flect_get_self( type_arr->data[ i ] );
    bcore_arr_tp_s_discard( type_arr );
}

/**********************************************************************************************************************/

static void flect_define_basics()
{
    bcore_flect_define_self_d( bcore_self_s_create_plain( entypeof( "void" ), typeof( "leaf" ), 0, 0 ) );

    BCORE_REGISTER_TYPE( num, s0_t );
    BCORE_REGISTER_TYPE( num, s1_t );
    BCORE_REGISTER_TYPE( num, s2_t );
    BCORE_REGISTER_TYPE( num, s3_t );
    BCORE_REGISTER_TYPE( num, sz_t );
    BCORE_REGISTER_TYPE( num, smax_t );

    BCORE_REGISTER_TYPE( num, u0_t );
    BCORE_REGISTER_TYPE( num, u1_t );
    BCORE_REGISTER_TYPE( num, u2_t );
    BCORE_REGISTER_TYPE( num, u3_t );
    BCORE_REGISTER_TYPE( num, uz_t );
    BCORE_REGISTER_TYPE( num, umax_t );

    BCORE_REGISTER_TYPE( num, f2_t );
    BCORE_REGISTER_TYPE( num, f3_t );
    BCORE_REGISTER_TYPE( num, bl_t );
    BCORE_REGISTER_TYPE( leaf, sd_t );
    BCORE_REGISTER_TYPE( leaf, sc_t );
    BCORE_REGISTER_TYPE( leaf, vd_t );
    BCORE_REGISTER_TYPE( leaf, vc_t );
    BCORE_REGISTER_TYPE( leaf, fp_t );

    BCORE_REGISTER_TYPE( num, offset_t );

    // special purpose types
    BCORE_REGISTER_TYPE( type, tp_t                );
    BCORE_REGISTER_TYPE( type, er_t                );
    BCORE_REGISTER_TYPE( type, aware_t             );
    BCORE_REGISTER_TYPE( root, bcore_flect_flags_s );

    // specific objects
    bcore_flect_define_creator( typeof( "bcore_self_item_s" ), self_item_s_create_self );
    bcore_flect_define_creator( typeof( "bcore_self_body_s" ), flect_body_s_create_self );
    bcore_flect_define_creator( typeof( "bcore_self_s" ), flect_self_s_create_self );
}

/// encapsulation structures
BCORE_DEFINE_CREATE_SELF( bcore_link_static_s,            "bcore_link_static_s       = { vd_t     link; }" )
BCORE_DEFINE_CREATE_SELF( bcore_link_typed_s,             "bcore_link_typed_s        = { typed => link; }" )
BCORE_DEFINE_CREATE_SELF( bcore_link_aware_s,             "bcore_link_aware_s        = { aware => link; }" )
BCORE_DEFINE_CREATE_SELF( bcore_array_dyn_solid_static_s, "bcore_array_dyn_solid_static_s = { vd_t  data; uz_t size; uz_t space; }" )
BCORE_DEFINE_CREATE_SELF( bcore_array_dyn_solid_typed_s,  "bcore_array_dyn_solid_typed_s  = { typed  []; }" )
BCORE_DEFINE_CREATE_SELF( bcore_array_dyn_link_static_s,  "bcore_array_dyn_link_static_s  = { vd_t  => data; uz_t size; uz_t space; }" )
BCORE_DEFINE_CREATE_SELF( bcore_array_dyn_link_typed_s,   "bcore_array_dyn_link_typed_s   = { typed => []; }" )
BCORE_DEFINE_CREATE_SELF( bcore_array_dyn_link_aware_s,   "bcore_array_dyn_link_aware_s   = { aware => []; }" )

/**********************************************************************************************************************/

static st_s* flect_selftest( void )
{
    {
        bcore_flect_define_parse_sc(" teabag =    { u3_t leaves; s1_t flavor; s0_t color;  }" );
        bcore_flect_define_parse_sc(" container = { u3_t elements; teabag => [] bags; u1_t flags; }" );
        bcore_flect_define_parse_sc(" delivery =  { aware_t _; u3_t tag; container => cargo; aware => [] attachments; }" );
        bcore_flect_define_parse_sc(" another = delivery" );
    }

    {
        tp_t t_teabag    = bcore_flect_type_parse_sc( "{ u3_t leaves; s1_t flavor; s0_t color; }" );
        tp_t t_container = bcore_flect_type_parse_fa( "{ u3_t elements = 343; #<tp_t> bag; u1_t flags; }", t_teabag );
        bcore_flect_get_self( t_container );
        tp_t anonymous = bcore_flect_type_parse_sc( "{ u3_t leaves; s1_t flavor; s0_t color; }" );
        ASSERT( t_teabag == anonymous );
    }

    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

void flect_test_create_struct()
{

    sc_t def =

    "bcore_my_struct_s = bcore_inst"
    "{"
        "aware_t _;"

        "sz_t slos;"   // values per xon
        "sz_t xons;"   // xons  per row
        "sz_t rows;"   // number of rows
        "sz_t stride;" // stride of splicing

        "sz_t [] i_arr;" // index data
        "f3_t [] d_arr;" // value data
    "}";


    bcore_flect_define_parse_sc( def );

    const bcore_self_s* self = bcore_flect_get_self( typeof( "bcore_my_struct_s" ) );

//    bcore_txt_ml_a_to_stdout( self );

    bcore_self_s_struct_to_sink( self, 0, BCORE_STDOUT );


}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// signal

vd_t bcore_flect_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_flect" ) ) )
    {
        case TYPEOF_init0:
        {
            flect_open();
        }
        break;

        case TYPEOF_init1:
        {
            // some basic traits
            bcore_trait_set( entypeof( "root"             ), 0 ); // root trait (all traits should end in root)
            bcore_trait_set( entypeof( "bcore_inst"       ), typeof( "root" ) ); // all (instantiable) reflections should inherit bcore_inst
            bcore_trait_set( entypeof( "leaf"             ), typeof( "bcore_inst" ) ); // leaf trait
            bcore_trait_set( entypeof( "function_pointer" ), typeof( "leaf" ) );       // type of trait function_pointer can be casted to fp_t
            bcore_trait_set( entypeof( "num"              ), typeof( "leaf" ) );       // a number is a leaf type
            bcore_trait_set( entypeof( "type"             ), typeof( "num"  ) );       // type is encoded as number

            flect_define_basics();
            BCORE_REGISTER_OBJECT( bcore_link_static_s );
            BCORE_REGISTER_OBJECT( bcore_link_typed_s );
            BCORE_REGISTER_OBJECT( bcore_link_aware_s );
            BCORE_REGISTER_OBJECT( bcore_array_dyn_solid_static_s );
            BCORE_REGISTER_OBJECT( bcore_array_dyn_solid_typed_s );
            BCORE_REGISTER_OBJECT( bcore_array_dyn_link_static_s );
            BCORE_REGISTER_OBJECT( bcore_array_dyn_link_typed_s );
            BCORE_REGISTER_OBJECT( bcore_array_dyn_link_aware_s );
        }
        break;

        case TYPEOF_down0:
        {
            s2_t verbosity = o->object ? *( s2_t* )o->object : 0;
            if( verbosity > 0 )
            {
                uz_t count = bcore_flect_size();
                uz_t space = bcore_tbman_total_granted_space();
                flect_close();
                space -= bcore_tbman_total_granted_space();

                bcore_msg( "  reflection mananger . % 7zu (by % 4zu reflections  )\n", space, count );
            }
            else
            {
                flect_close();
            }
        }
        break;

        case TYPEOF_selftest:
        {
            flect_test_create_struct();
            return flect_selftest();
        }
        break;

        default: break;
    }
    return NULL;
}
