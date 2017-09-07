/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_interpreter.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_quicktypes.h"

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

/**********************************************************************************************************************/

static bl_t supports( const bcore_flect_self_s* self )
{
    if( !bcore_flect_self_s_is_aware( self )                                           ) return false;
    if( !bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_interpret" ), 0 ) ) return false;
    return true;
}

static bcore_interpreter_s* create_from_self( const bcore_flect_self_s* self )
{
    assert( self != NULL );
    bcore_interpreter_s* o = interpreter_s_create();
    o->o_type = self->type;
    o->fp_interpret = ( bcore_fp_interpret )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_fp_interpret" ), 0 );
    return o;
}

bcore_flect_self_s* bcore_interpreter_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_interpreter_s" ), sizeof( bcore_interpreter_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )interpreter_s_init,             "bcore_fp_init",                   "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )interpreter_s_down,             "bcore_fp_down",                   "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )interpreter_s_create,           "bcore_fp_create",                 "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )interpreter_s_discard,          "bcore_fp_discard",                "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )supports,                       "bcore_spect_fp_supports",         "supports"     );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,               "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bcore_interpreter_s* bcore_interpreter_s_get_typed( tp_t type )
{
    return bcore_spect_get_typed( typeof( "bcore_interpreter_s" ), type );
}

/**********************************************************************************************************************/

sr_s bcore_interpret( sr_s o, sr_s source )
{
    const bcore_interpreter_s* p = ch_spect( o.p, TYPEOF_bcore_interpreter_s );
    sr_s ret = p->fp_interpret( o.o, source );
    sr_down( o );
    return ret;
}

sr_s bcore_interpret_q( const sr_s* o, sr_s source )
{
    const bcore_interpreter_s* p = ch_spect( o->p, TYPEOF_bcore_interpreter_s );
    return p->fp_interpret( o->o, source );
}

/**********************************************************************************************************************/

