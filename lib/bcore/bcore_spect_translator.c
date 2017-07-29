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

/**********************************************************************************************************************/

static bcore_translator_s* create_from_self( const bcore_flect_self_s** p_self )
{
    assert( p_self != NULL );
    const bcore_flect_self_s* t_self = p_self[ 0 ];
    const bcore_flect_self_s* o_self = p_self[ 1 ];
    assert( t_self != NULL );
    assert( o_self != NULL );

    bcore_translator_s* o = translator_s_create();
    o->t_type = t_self->type;
    o->o_type = o_self->type;
    o->translate_body = bcore_flect_self_s_try_external_fp( o_self, bcore_name_enroll( "bcore_fp_translate_body" ), 0 );
    if( !o->translate_body ) o->translate_body = bcore_flect_self_s_get_external_fp( t_self, typeof( "bcore_fp_translate_body" ), 0 );
    o->translate_body_amoeba = bcore_flect_self_s_try_external_fp( o_self, typeof( "ap_t" ), bcore_name_enroll( "translate_body" ) );
    o->translate_object = bcore_flect_self_s_try_external_fp( o_self, bcore_name_enroll( "bcore_fp_translate_object" ), 0 );
    if( !o->translate_object ) o->translate_object = bcore_flect_self_s_get_external_fp( t_self, typeof( "bcore_fp_translate_object" ), 0 );
    return o;
}

bcore_flect_self_s* bcore_translator_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_translator_s" ), sizeof( bcore_translator_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_init,             "bcore_fp_init",                   "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_down,             "bcore_fp_down",                   "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_create,           "bcore_fp_create",                 "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_discard,          "bcore_fp_discard",                "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_create_signature, "bcore_spect_fp_create_signature", "create_signature" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,              "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bcore_translator_s* bcore_translator_s_get_typed( tp_t t_type, tp_t o_type )
{
    tp_t type_arr[ 2 ] = { t_type, o_type };
    return bcore_spect_get_typed_n( typeof( "bcore_translator_s" ), 2, type_arr );
}

/**********************************************************************************************************************/

typedef struct { ap_t ap; const bcore_translator_s* p; vc_t trans; tp_t type; vc_t obj; vd_t sink; } translate_body_nc;

static void translate_body_amoeba( translate_body_nc* nc )
{
    nc->p->translate_body( nc->trans, nc->type, nc->obj, nc->sink );
}

void bcore_translate_spect_body( const bcore_translator_s* spect, vd_t trans, vc_t obj, vd_t sink )
{
    if( spect->translate_body_amoeba )
    {
        translate_body_nc nc = { ( ap_t )translate_body_amoeba, spect, trans, spect->o_type, obj, sink };
        spect->translate_body_amoeba( &nc );
    }
    else
    {
        spect->translate_body( trans, spect->o_type, obj, sink );
    }
}

void bcore_translate_typed_body( vd_t trans, tp_t o_type, vc_t obj, vd_t sink )
{
    bcore_translate_spect_body( bcore_translator_s_get_typed( *( aware_t *)trans, o_type ), trans, obj, sink );
}

void bcore_translate_aware_body( vd_t trans, vc_t obj, vd_t sink )
{
    bcore_translate_typed_body( trans, *( aware_t *)obj, obj, sink );
}

void bcore_translate_spect_object( const bcore_translator_s* spect, vd_t trans, vc_t obj, vd_t sink )
{
    spect->translate_object( trans, spect->o_type, obj, sink );
}

void bcore_translate_typed_object( vd_t trans, tp_t o_type, vc_t obj, vd_t sink )
{
    bcore_translate_spect_object( bcore_translator_s_get_typed( *( aware_t *)trans, o_type ), trans, obj, sink );
}

void bcore_translate_aware_object( vd_t trans, vc_t obj, vd_t sink )
{
    bcore_translate_typed_object( trans, *( aware_t *)obj, obj, sink );
}

/**********************************************************************************************************************/

