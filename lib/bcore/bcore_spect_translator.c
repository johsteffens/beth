/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_translator.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_quicktypes.h"

/**********************************************************************************************************************/
// bcore_translator_s

static void translator_s_down( bcore_translator_s* o );

static void translator_s_init( bcore_translator_s* o )
{
    bcore_memzero( o, sizeof( bcore_translator_s ) );
    o->p_type = TYPEOF_bcore_translator_s;
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

/**********************************************************************************************************************/

static bl_t supports( const bcore_flect_self_s* self )
{
    if( !bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_translate" ), 0 ) ) return false;
    return true;
}

static bcore_translator_s* create_from_self( const bcore_flect_self_s* self )
{
    assert( self != NULL );
    bcore_translator_s* o = translator_s_create();
    o->o_type = self->type;
    o->fp_translate = ( bcore_fp_translate )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_fp_translate" ), 0 );
    return o;
}

bcore_flect_self_s* bcore_translator_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_translator_s" ), sizeof( bcore_translator_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_init,             "bcore_fp_init",                   "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_down,             "bcore_fp_down",                   "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_create,           "bcore_fp_create",                 "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )translator_s_discard,          "bcore_fp_discard",                "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )supports,                      "bcore_spect_fp_supports",         "supports"     );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,              "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bcore_translator_s* bcore_translator_s_get_typed( tp_t type )
{
    return bcore_spect_get_typed( typeof( "bcore_translator_s" ), type );
}

/**********************************************************************************************************************/

void bcore_translate( sr_s o, sr_s obj, sr_s sink )
{
    const bcore_translator_s* p = ch_spect( o.p, TYPEOF_bcore_translator_s );
    p->fp_translate( o.o, obj, sink );
    sr_down( o );
}

void bcore_translate_q( const sr_s* o, sr_s obj, sr_s sink )
{
    const bcore_translator_s* p = ch_spect( o->p, TYPEOF_bcore_translator_s );
    p->fp_translate( o->o, obj, sink );
}

/**********************************************************************************************************************/

