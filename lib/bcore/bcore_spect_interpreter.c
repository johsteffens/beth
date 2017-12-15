/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_spect_interpreter.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_quicktypes.h"
#include "bcore_life.h"
#include "bcore_txt_ml.h"
#include "bcore_spect_source.h"
#include "bcore_sources.h"

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
    bcore_release_obj( ( fp_t )interpreter_s_down, o );
}

/**********************************************************************************************************************/

static void interpreter_s_define_trait()
{
    tp_t trait = entypeof( "bcore_interpreter" );
    bcore_trait_require_awareness( trait );
    bcore_trait_require_function( trait, entypeof( "bcore_fp_interpret" ), 0 );
    bcore_trait_set( trait, entypeof( "bcore_inst" ) );
}

static bcore_interpreter_s* create_from_self( const bcore_flect_self_s* self )
{
    assert( self != NULL );
    bcore_interpreter_s* o = interpreter_s_create();
    o->o_type = self->type;
    o->fp_interpret = ( bcore_fp_interpret )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_fp_interpret" ), 0 );
    return o;
}

sr_s bcore_interpret_auto( sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    sr_s src = sr_cp( bcore_life_s_push_sr( l, source ), TYPEOF_bcore_source_s );
    bcore_txt_ml_interpreter_s* txt_ml = bcore_life_s_push_aware( l, bcore_txt_ml_interpreter_s_create() );
    sr_s interpreter = bcore_txt_ml_interpreter_s_interpret( txt_ml, src );

    if( !interpreter.o ) bcore_source_parse_err_fa( src, "No interpreter specified." );

    st_s* log = bcore_life_s_push_aware( l, st_s_create() );
    if( !bcore_trait_satisfied_type( typeof( "bcore_interpreter" ), sr_s_type( &interpreter ), log ) )
    {
        bcore_source_parse_err_fa( src, "Object '#<sc_t>' is no interpreter.\nReason: #<sc_t>", ifnameof( sr_s_type( &interpreter ) ), log->sc );
    }

    sr_s obj = bcore_interpret( interpreter, src );

    bcore_life_s_discard( l );

    return obj;
}

sr_s bcore_interpret_auto_file( sc_t file )
{
    sr_s chain = sr_asd( bcore_source_chain_s_create() );
    bcore_source_chain_s_push_d( chain.o, bcore_source_file_s_create_name( file ) );
    // TODO: test if bcore_source_string_s works correctly on binary files
    bcore_source_chain_s_push_d( chain.o, bcore_inst_typed_create( typeof( "bcore_source_string_s" ) ) );
    sr_s ret = bcore_interpret_auto( chain );
    return ret;
}

static bcore_flect_self_s* interpreter_s_create_self( void )
{
    sc_t def = "bcore_interpreter_s = spect { aware_t p_type; tp_t o_type; ... }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_interpreter_s ) );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )interpreter_s_init,             "bcore_fp_init",                   "init"         );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )interpreter_s_down,             "bcore_fp_down",                   "down"         );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )interpreter_s_create,           "bcore_fp_create",                 "create"       );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )interpreter_s_discard,          "bcore_fp_discard",                "discard"      );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )create_from_self,               "bcore_spect_fp_create_from_self", "create_from_self" );
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
    const bcore_interpreter_s* p = ch_spect_p( o.p, TYPEOF_bcore_interpreter_s );
    sr_s ret = p->fp_interpret( o.o, source );
    sr_down( o );
    return ret;
}

sr_s bcore_interpret_q( const sr_s* o, sr_s source )
{
    const bcore_interpreter_s* p = ch_spect_p( o->p, TYPEOF_bcore_interpreter_s );
    return p->fp_interpret( o->o, source );
}

/**********************************************************************************************************************/

vd_t bcore_spect_interpreter_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_spect_interpreter" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        interpreter_s_define_trait();
        bcore_flect_define_creator( typeof( "bcore_interpreter_s"  ), interpreter_s_create_self  );
    }

    return NULL;
}

/**********************************************************************************************************************/


