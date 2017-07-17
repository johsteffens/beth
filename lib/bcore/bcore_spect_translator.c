/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_translator.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_spect.h"

/**********************************************************************************************************************/
// bcore_translator_s

static void translator_s_down( bcore_translator_s* o );

static void translator_s_init( bcore_translator_s* o )
{
    bcore_memzero( o, sizeof( bcore_translator_s ) );
    o->p_type = typeof( "bcore_translator_s" );
}

static void translator_s_down( bcore_translator_s* o )
{
}

static bcore_translator_s* translator_s_create()
{
    bcore_translator_s* o = bcore_alloc( NULL, sizeof( bcore_translator_s ) );
    translator_s_init( o );
    return o;
}

static void translator_s_discard( bcore_translator_s* o )
{
    if( !o ) return;
    translator_s_down( o );
    bcore_free( o );
}

static bcore_signature_s* translator_s_create_signature( bcore_translator_s* o )
{
    return bcore_signature_s_create_an( 3, o->p_type, o->t_type, o->o_type );
}

bcore_flect_self_s* bcore_translator_s_create_self()
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_translator_s" ), sizeof( bcore_translator_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_init,             "bcore_fp_init",                    "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_down,             "bcore_fp_down",                    "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_create,           "bcore_fp_create",                  "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_discard,          "bcore_fp_discard",                 "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_create_signature, "bcore_spect_fp_create_signature",  "create_signature" );
    return self;
}

/**********************************************************************************************************************/

static bcore_translator_s* create_from_self( const bcore_flect_self_s* t_self, const bcore_flect_self_s* o_self )
{
    bcore_translator_s* o = translator_s_create();
    o->t_type = t_self->type;
    o->o_type = o_self->type;
    o->translate_body = bcore_flect_self_s_try_external_fp( o_self, typeof( "bcore_fp_translate_body" ), 0 );
    if( !o->translate_body ) o->translate_body = bcore_flect_self_s_get_external_fp( t_self, typeof( "bcore_fp_translate_body" ), 0 );
    o->translate_object = bcore_flect_self_s_try_external_fp( o_self, typeof( "bcore_fp_translate_object" ), 0 );
    if( !o->translate_object ) o->translate_object = bcore_flect_self_s_get_external_fp( t_self, typeof( "bcore_fp_translate_object" ), 0 );
    return o;
}

/**********************************************************************************************************************/

const bcore_translator_s* bcore_translator_s_get_typed( tp_t t_type, tp_t o_type )
{
    tp_t sig = bcore_signature_get_hash_na( 3, typeof( "bcore_translator_s" ), t_type , o_type );
    const bcore_translator_s* spect_p = bcore_spect_try( sig );
    if( !spect_p )
    {
        const bcore_flect_self_s* t_self = bcore_flect_get_self( t_type );
        const bcore_flect_self_s* o_self = bcore_flect_get_self( o_type );
        bcore_translator_s* new_spect_p = create_from_self( t_self, o_self );
        bcore_spect_enroll_d( new_spect_p );
        spect_p = new_spect_p;
    }
    return spect_p;
}

/**********************************************************************************************************************/

void bcore_translate_typed_body( vd_t trans, tp_t o_type, vc_t obj, vd_t sink )
{
    const bcore_translator_s* spect = bcore_translator_s_get_typed( *( aware_t *)trans, o_type );
    spect->translate_body( trans, o_type, obj, sink );
}

void bcore_translate_aware_body( vd_t trans, vc_t obj, vd_t sink )
{
    tp_t o_type = *( aware_t *)obj;
    bcore_translate_typed_body( trans, o_type, obj, sink );
}

void bcore_translate_typed_object( vd_t trans, tp_t o_type, vc_t obj, vd_t sink )
{
    const bcore_translator_s* spect = bcore_translator_s_get_typed( *( aware_t *)trans, o_type );
    spect->translate_object( trans, o_type, obj, sink );
}

void bcore_translate_aware_object( vd_t trans,              vc_t obj, vd_t sink )
{
    tp_t o_type = *( aware_t *)obj;
    bcore_translate_typed_object( trans, o_type, obj, sink );
}

/**********************************************************************************************************************/

