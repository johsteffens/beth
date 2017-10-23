/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_spect_source.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_quicktypes.h"

#define NPX( name ) bcore_source_##name

/**********************************************************************************************************************/
// bcore_source_s

static void source_s_down( bcore_source_s* o );

static void source_s_init( bcore_source_s* o )
{
    bcore_memzero( o, sizeof( bcore_source_s ) );
    o->p_type = typeof( "bcore_source_s" );
}

static void source_s_down( bcore_source_s* o )
{
}

static bcore_source_s* source_s_create()
{
    bcore_source_s* o = bcore_alloc( NULL, sizeof( bcore_source_s ) );
    source_s_init( o );
    return o;
}

static void source_s_discard( bcore_source_s* o )
{
    if( !o ) return;
    bcore_release_obj( ( fp_t )source_s_down, o );
}

/**********************************************************************************************************************/

sz_t bcore_source_spect_get_data( const bcore_source_s* p, vd_t o, vd_t data, sz_t size )
{
    return p->fp_flow_src( o, data, size );
}

void bcore_source_spect_parse_errvf( const bcore_source_s* p, vd_t o, sc_t format, va_list args )
{
    if( p->fp_parse_errvf )
    {
        p->fp_parse_errvf( o, format, args );
    }
    else
    {
        ERR( "Parse error:\n%s\n", st_s_createvf( format, args )->sc );
    }
}

void bcore_source_spect_parse_errf( const bcore_source_s* p, vd_t o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_source_spect_parse_errvf( p, o, format, args );
    va_end( args );
}

void bcore_source_spect_parse_fv( const bcore_source_s* p, vd_t o, sc_t format, va_list args )
{
    if( p->fp_parse_errvf )
    {
        p->fp_parse_fv( o, format, args );
    }
    else
    {
        ERR( "Object '%s' does not support feature 'bcore_source_fp_parse_fv'.", nameof( p->o_type ) );
    }
}

void bcore_source_spect_parse_fa( const bcore_source_s* p, vd_t o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_source_spect_parse_fv( p, o, format, args );
    va_end( args );
}

bl_t bcore_source_spect_parse_boolf( const bcore_source_s* p, vd_t o, sc_t format )
{
    bl_t flag = false;
    bcore_source_spect_parse_fa( p, o, format, &flag );
    return flag;
}

void bcore_source_spect_set_supplier( const bcore_source_s* p, vd_t o, vd_t supplier )
{
    if( p->fp_set_supplier )
    {
        p->fp_set_supplier( o, supplier );
    }
    else
    {
        ERR( "Object '%s' does not support feature 'bcore_source_fp_set_supplier'.", nameof( p->o_type ) );
    }
}

/**********************************************************************************************************************/

static void source_s_define_trait()
{
    tp_t trait = entypeof( "bcore_source_s" );
    bcore_trait_require_awareness( trait );
    bcore_trait_require_function( trait, entypeof( "bcore_fp_flow_src" ), 0 );
    bcore_trait_set( trait, entypeof( "bcore_inst_s" ) );
}

static bcore_source_s* create_from_self( const bcore_flect_self_s* self )
{
    assert( self != NULL );
    assert( bcore_flect_self_s_is_aware( self ) );

    bcore_source_s* o = source_s_create();
    o->o_type = self->type;
    o->fp_flow_src     = ( bcore_fp_flow_src            )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_fp_flow_src" ), 0 );
    o->fp_parse_fv      = ( bcore_source_fp_parse_fv      )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_parse_fv" ), 0 );
    o->fp_parse_errvf  = ( bcore_fp_logvf               )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bcore_fp_logvf" ), bcore_name_enroll( "p_errorvf" ) );
    o->fp_set_supplier = ( bcore_source_fp_set_supplier )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_set_supplier" ), 0 );
    return o;
}

static bcore_flect_self_s* source_s_create_self( void )
{
    sc_t def = "bcore_source_s = spect { aware_t p_type; tp_t o_type; ... }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_source_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )source_s_init,             "bcore_fp_init",                    "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )source_s_down,             "bcore_fp_down",                    "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )source_s_create,           "bcore_fp_create",                  "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )source_s_discard,          "bcore_fp_discard",                 "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,          "bcore_spect_fp_create_from_self",  "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bcore_source_s* bcore_source_s_get_typed( tp_t o_type )
{
    return bcore_spect_get_typed( typeof( "bcore_source_s" ), o_type );
}

const bcore_source_s* bcore_source_s_get_aware( vc_t obj )
{
    return bcore_source_s_get_typed( *( const aware_t* )obj );
}

/**********************************************************************************************************************/

static inline const bcore_source_s* gtpd( tp_t tp ) { return bcore_source_s_get_typed( tp ); }

sz_t NPX(aware_get_data    )( vd_t o, vd_t d, sz_t sz   ) { return NPX(spect_get_data    )( gtpd( *( aware_t* )o ), o, d, sz ); }
void NPX(aware_parse_fv    )( vd_t o, sc_t f, va_list a ) {        NPX(spect_parse_fv    )( gtpd( *( aware_t* )o ), o, f, a  ); }
void NPX(aware_parse_errvf )( vd_t o, sc_t f, va_list a ) {        NPX(spect_parse_errvf )( gtpd( *( aware_t* )o ), o, f, a  ); }
bl_t NPX(aware_parse_boolf )( vd_t o, sc_t f            ) { return NPX(spect_parse_boolf )( gtpd( *( aware_t* )o ), o, f     ); }
void NPX(aware_set_supplier)( vd_t o, vd_t s            ) {        NPX(spect_set_supplier)( gtpd( *( aware_t* )o ), o, s     ); }

void NPX(aware_parse_fa    )( vd_t o, sc_t f, ...       ) { va_list a; va_start( a, f ); NPX(aware_parse_fv    )( o, f, a ); va_end( a ); }
void NPX(aware_parse_errf  )( vd_t o, sc_t f, ...       ) { va_list a; va_start( a, f ); NPX(aware_parse_errvf)( o, f, a ); va_end( a ); }

inline static vc_t w_spect( sr_s o ) { if( sr_s_is_const( &o ) ) ERR( "Attempt to modify a constant object" ); return ch_spect_p( o.p, TYPEOF_bcore_source_s ); }
inline static vc_t r_spect( sr_s o ) { return ch_spect_p( o.p, TYPEOF_bcore_source_s ); }

sz_t NPX(get_data    )( sr_s o, vd_t d, sz_t sz   ) { sz_t r = NPX(spect_get_data    )( w_spect( o ), o.o, d, sz ); sr_down( o ); return r; }
void NPX(parse_fv    )( sr_s o, sc_t f, va_list a ) {          NPX(spect_parse_fv    )( w_spect( o ), o.o, f, a  ); sr_down( o ); }
void NPX(parse_errvf )( sr_s o, sc_t f, va_list a ) {          NPX(spect_parse_errvf )( w_spect( o ), o.o, f, a  ); sr_down( o ); }
bl_t NPX(parse_boolf )( sr_s o, sc_t f            ) { bl_t r = NPX(spect_parse_boolf )( w_spect( o ), o.o, f     ); sr_down( o ); return r; }
void NPX(set_supplier)( sr_s o, vd_t s            ) {          NPX(spect_set_supplier)( w_spect( o ), o.o, s     ); sr_down( o ); }

void NPX(parse_fa    )( sr_s o, sc_t f, ...       ) { va_list a; va_start( a, f ); NPX(parse_fv   )( o, f, a ); va_end( a ); }
void NPX(parse_errf  )( sr_s o, sc_t f, ...       ) { va_list a; va_start( a, f ); NPX(parse_errvf)( o, f, a ); va_end( a ); }

sz_t NPX(q_get_data    )( const sr_s* o, vd_t d, sz_t sz   ) { return NPX(spect_get_data    )( w_spect( *o ), o->o, d, sz ); }
void NPX(q_parse_fv    )( const sr_s* o, sc_t f, va_list a ) {        NPX(spect_parse_fv    )( w_spect( *o ), o->o, f, a  ); }
void NPX(q_parse_errvf )( const sr_s* o, sc_t f, va_list a ) {        NPX(spect_parse_errvf )( w_spect( *o ), o->o, f, a  ); }
bl_t NPX(q_parse_boolf )( const sr_s* o, sc_t f            ) { return NPX(spect_parse_boolf )( w_spect( *o ), o->o, f     ); }
void NPX(q_set_supplier)( const sr_s* o, vd_t s            ) {        NPX(spect_set_supplier)( w_spect( *o ), o->o, s     ); }
void NPX(q_parse_fa    )( const sr_s* o, sc_t f, ...       ) { va_list a; va_start( a, f ); NPX(q_parse_fv   )( o, f, a ); va_end( a ); }
void NPX(q_parse_errf  )( const sr_s* o, sc_t f, ...       ) { va_list a; va_start( a, f ); NPX(q_parse_errvf)( o, f, a ); va_end( a ); }

/**********************************************************************************************************************/

vd_t bcore_spect_source_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_spect_source" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        source_s_define_trait();
        bcore_flect_define_creator( typeof( "bcore_source_s"  ), source_s_create_self  );
    }

    return NULL;
}

/**********************************************************************************************************************/


