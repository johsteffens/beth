/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_interpreter.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_spect.h"

/**********************************************************************************************************************/
// bcore_interpreter_s

static void interpreter_s_down( bcore_interpreter_s* o );

static void interpreter_s_init( bcore_interpreter_s* o )
{
    bcore_memzero( o, sizeof( bcore_interpreter_s ) );
    o->p_type = typeof( "bcore_interpreter_s" );
}

static void interpreter_s_down( bcore_interpreter_s* o )
{
}

static bcore_interpreter_s* interpreter_s_create()
{
    bcore_interpreter_s* o = bcore_alloc( NULL, sizeof( bcore_interpreter_s ) );
    interpreter_s_init( o );
    return o;
}

static void interpreter_s_discard( bcore_interpreter_s* o )
{
    if( !o ) return;
    interpreter_s_down( o );
    bcore_free( o );
}

static bcore_signature_s* interpreter_s_create_signature( bcore_interpreter_s* o )
{
    return bcore_signature_s_create_an( 3, o->p_type, o->t_type, o->o_type );
}

/**********************************************************************************************************************/

// o_self can be NULL
static bcore_interpreter_s* create_from_self( const bcore_flect_self_s** p_self )
{
    assert( p_self != NULL );
    const bcore_flect_self_s* t_self = p_self[ 0 ];
    const bcore_flect_self_s* o_self = p_self[ 1 ];
    assert( t_self != NULL );

    bcore_interpreter_s* o = interpreter_s_create();
    o->t_type = t_self->type;
    o->o_type = o_self ? o_self->type : 0;
    o->interpret_body = o_self ? ( bcore_fp_interpret_body )bcore_flect_self_s_try_external_fp( o_self, bcore_name_enroll( "bcore_fp_interpret_body" ), 0 ) : NULL;
    if( !o->interpret_body ) o->interpret_body = ( bcore_fp_interpret_body )bcore_flect_self_s_get_external_fp( t_self, typeof( "bcore_fp_interpret_body" ), 0 );
    o->interpret_body_amoeba = o_self ? bcore_flect_self_s_try_external_fp( o_self, typeof( "ap_t" ), bcore_name_enroll( "interpret_body" ) ) : NULL;
    o->interpret_object = o_self ? ( bcore_fp_interpret_object )bcore_flect_self_s_try_external_fp( o_self, bcore_name_enroll( "bcore_fp_interpret_object" ), 0 ) : NULL;
    if( !o->interpret_object ) o->interpret_object = ( bcore_fp_interpret_object )bcore_flect_self_s_get_external_fp( t_self, typeof( "bcore_fp_interpret_object" ), 0 );
    return o;
}

bcore_flect_self_s* bcore_interpreter_s_create_self()
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_interpreter_s" ), sizeof( bcore_interpreter_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )interpreter_s_init,             "bcore_fp_init",                   "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )interpreter_s_down,             "bcore_fp_down",                   "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )interpreter_s_create,           "bcore_fp_create",                 "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )interpreter_s_discard,          "bcore_fp_discard",                "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )interpreter_s_create_signature, "bcore_spect_fp_create_signature", "create_signature" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,               "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bcore_interpreter_s* bcore_interpreter_s_get_typed( tp_t t_type, tp_t o_type )
{
    tp_t type_arr[ 2 ] = { t_type, o_type };
    return bcore_spect_get_typed_n( typeof( "bcore_interpreter_s" ), 2, type_arr );
}
/*
const bcore_interpreter_s* bcore_interpreter_s_get_typed( tp_t t_type, tp_t o_type )
{
    tp_t sig = bcore_signature_get_hash_na( 3, typeof( "bcore_interpreter_s" ), t_type , o_type );
    const bcore_interpreter_s* spect_p = bcore_spect_try( sig );
    if( !spect_p )
    {
        const bcore_flect_self_s* t_self = bcore_flect_get_self( t_type );
        const bcore_flect_self_s* o_self = o_type ? bcore_flect_get_self( o_type ) : NULL;
        bcore_interpreter_s* new_spect_p = create_from_self( t_self, o_self );
        bcore_spect_enroll_d( new_spect_p );
        spect_p = new_spect_p;
    }
    return spect_p;
}
*/
/**********************************************************************************************************************/

typedef struct { ap_t ap; const bcore_interpreter_s* p; vc_t inter; vd_t source; tp_t type; vd_t obj; } interpret_body_nc;

static void interpret_body_amoeba( interpret_body_nc* nc )
{
    nc->p->interpret_body( nc->inter, nc->source, nc->type, nc->obj );
}

void bcore_interpret_spect_body( const bcore_interpreter_s* spect, vd_t intrp, vd_t source, vd_t obj )
{
    if( spect->interpret_body_amoeba )
    {
        interpret_body_nc nc = { ( ap_t )interpret_body_amoeba, spect, intrp, source, spect->o_type, obj };
        spect->interpret_body_amoeba( &nc );
    }
    else
    {
        spect->interpret_body( intrp, source, spect->o_type, obj );
    }
}

void bcore_interpret_typed_body( vd_t intrp, vd_t source, tp_t o_type, vd_t obj )
{
    bcore_interpret_spect_body( bcore_interpreter_s_get_typed( *( aware_t *)intrp, o_type ), intrp, source, obj );
}

void bcore_interpret_aware_body( vd_t intrp, vd_t source, vd_t obj )
{
    tp_t o_type = *( aware_t *)obj;
    bcore_interpret_typed_body( intrp, source, o_type, obj );
}

dt_p bcore_interpret_object( vd_t intrp, vd_t source )
{
    const bcore_interpreter_s* spect = bcore_interpreter_s_get_typed( *( aware_t *)intrp, 0 );
    return spect->interpret_object( intrp, source );
}

/**********************************************************************************************************************/

