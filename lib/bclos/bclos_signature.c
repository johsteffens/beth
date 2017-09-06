/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bclos_signature.h"
#include "bclos_quicktypes.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_spect_source.h"
#include "bcore_sources.h"
#include "bcore_life.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT( bclos_signature_arg_s )
DEFINE_FUNCTION_DOWN_FLAT( bclos_signature_arg_s )
DEFINE_FUNCTION_COPY_FLAT( bclos_signature_arg_s )
DEFINE_FUNCTION_CREATE(    bclos_signature_arg_s )
DEFINE_FUNCTION_DISCARD(   bclos_signature_arg_s )
DEFINE_FUNCTION_CLONE(     bclos_signature_arg_s )

static bcore_flect_self_s* arg_signature_s_create_self( void )
{
    sc_t def = "bclos_signature_arg_s = { tp_t name; tp_t type; bl_t is_const; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_signature_arg_s ) );
    return self;
}

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_SPECT( bclos_signature_s )
DEFINE_FUNCTION_DOWN_SPECT( bclos_signature_s )
DEFINE_FUNCTION_COPY_SPECT( bclos_signature_s )
DEFINE_FUNCTION_CREATE(     bclos_signature_s )
DEFINE_FUNCTION_DISCARD(    bclos_signature_s )
DEFINE_FUNCTION_CLONE(      bclos_signature_s )

void bclos_signature_s_push( bclos_signature_s* o, bclos_signature_arg_s arg )
{
    if( o->size > o->space ) bcore_array_typed_make_strong( TYPEOF_bclos_signature_s, o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc
        (
            sizeof( bclos_signature_arg_s ),
            o->data,
            o->space,
            o->space > 0 ? o->space * 2 : 1,
            &o->space
        );
    }
    o->data[ o->size++ ] = arg;
}

bclos_signature_s* bclos_signature_s_parse_from_source( sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    source = bcore_life_s_push_sr( l, source );
    bclos_signature_s* s = bclos_signature_s_create();

    bcore_string_s* name = bcore_string_s_create_l( l );
    bcore_source_q_parsef( &source, " #name", name );
    if( bcore_source_q_parse_boolf( &source, " #?'('" ) )
    {
        s->ret = 0;
        s->name = bcore_name_enroll( name->sc );
    }
    else
    {
        s->ret = bcore_name_enroll( name->sc );
        bcore_source_q_parsef( &source, " #name (", name );
        s->name = bcore_name_enroll( name->sc );
    }

    while( !bcore_source_q_parse_boolf( &source, " #?')'" ) )
    {
        if( s->size > 0 ) bcore_source_q_parsef( &source, " ," );
        bclos_signature_arg_s arg;
        arg.is_const = bcore_source_q_parse_boolf( &source, " #?'const'" );
        bcore_source_q_parsef( &source, " #name ", name );
        arg.type = bcore_name_enroll( name->sc );
        bcore_source_q_parsef( &source, " #name ", name );
        arg.name = bcore_name_enroll( name->sc );
        bclos_signature_s_push( s, arg );
    }

    bcore_life_s_discard( l );
    return s;
}

bclos_signature_s* bclos_signature_s_parse_from_sc( sc_t sc )
{
    return bclos_signature_s_parse_from_source( sr_asd( bcore_string_s_create_weak_sc( sc ) ) );
}

static bcore_flect_self_s* sig_s_create_self( void )
{
    sc_t def = "bclos_signature_s = { bclos_signature_arg_s [] arr; tp_t name; tp_t ret; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_signature_s ) );
    return self;
}

/**********************************************************************************************************************/

void bclos_signature_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bclos_signature_arg_s" ), arg_signature_s_create_self );
    bcore_flect_define_creator( typeof( "bclos_signature_s"     ), sig_s_create_self );
}

