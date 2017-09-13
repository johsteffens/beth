/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_sink.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_quicktypes.h"

/**********************************************************************************************************************/
// bcore_sink_s

static void sink_s_down( bcore_sink_s* o );

static void sink_s_init( bcore_sink_s* o )
{
    bcore_memzero( o, sizeof( bcore_sink_s ) );
    o->p_type = typeof( "bcore_sink_s" );
}

static void sink_s_down( bcore_sink_s* o )
{
}

static bcore_sink_s* sink_s_create()
{
    bcore_sink_s* o = bcore_alloc( NULL, sizeof( bcore_sink_s ) );
    sink_s_init( o );
    return o;
}

static void sink_s_discard( bcore_sink_s* o )
{
    if( !o ) return;
    sink_s_down( o );
    bcore_free( o );
}

/**********************************************************************************************************************/

sz_t bcore_sink_spect_push_data( const bcore_sink_s* p, vd_t o, vc_t data, sz_t size )
{
    return p->flow_snk( o, data, size );
}

void bcore_sink_spect_flush( const bcore_sink_s* p, vd_t o )
{
    if( p->fp_flush ) p->fp_flush( o );
}

void bcore_sink_spect_pushvf( const bcore_sink_s* p, vd_t o, sc_t format, va_list args )
{
    bcore_string_s* s = bcore_string_s_createvf( format, args );
    p->flow_snk( o, s->data, s->size );
    bcore_string_s_discard( s );
}

void bcore_sink_spect_pushf( const bcore_sink_s* p, vd_t o, sc_t f, ... )
{
    va_list a;
    va_start( a, f );
    bcore_sink_spect_pushvf( p, o, f, a );
    va_end( a );
}

void bcore_sink_spect_push_char( const bcore_sink_s* p, vd_t o, char c )
{
    p->flow_snk( o, &c, 1 );
}

void bcore_sink_spect_push_sc( const bcore_sink_s* p, vd_t o, sc_t sc )
{
    while( *sc )
    {
        bcore_sink_spect_push_char( p, o, *sc );
        sc++;
    }
}

void bcore_sink_spect_push_string( const bcore_sink_s* p, vd_t o, const bcore_string_s* string )
{
    p->flow_snk( o, string->data, string->size );
}

void bcore_sink_spect_push_string_d( const bcore_sink_s* p, vd_t o, bcore_string_s* string )
{
    p->flow_snk( o, string->data, string->size );
    bcore_string_s_discard( string );
}

void bcore_sink_spect_set_consumer( const bcore_sink_s* p, vd_t o, vd_t consumer )
{
    if( p->fp_set_consumer )
    {
        p->fp_set_consumer( o, consumer );
    }
    else
    {
        ERR( "Object '%s' does not support feature 'bcore_sink_fp_set_consumer'.", nameof( p->o_type ) );
    }
}

/**********************************************************************************************************************/

static void sink_s_define_trait()
{
    tp_t trait = entypeof( "bcore_sink_s" );
    bcore_trait_require_awareness( trait );
    bcore_trait_require_function( trait, entypeof( "bcore_fp_flow_snk" ), 0 );
    bcore_trait_set( trait, entypeof( "bcore_inst_s" ) );
}

static bcore_sink_s* create_from_self( const bcore_flect_self_s* self )
{
    assert( self != NULL );
    assert( bcore_flect_self_s_is_aware( self ) );

    bcore_sink_s* o = sink_s_create();
    o->o_type = self->type;
    o->flow_snk        = ( bcore_fp_flow_snk          )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_fp_flow_snk" ), 0 );
    o->fp_set_consumer = ( bcore_sink_fp_set_consumer )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bcore_sink_fp_set_consumer" ), 0 );
    o->fp_flush        = ( bcore_sink_fp_flush        )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bcore_sink_fp_flush" ), 0 );
    return o;
}

static bcore_flect_self_s* sink_s_create_self( void )
{
    sc_t def = "bcore_sink_s = spect { aware_t p_type; tp_t o_type; ... }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_sink_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_init,             "bcore_fp_init",                   "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_down,             "bcore_fp_down",                   "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_create,           "bcore_fp_create",                 "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_discard,          "bcore_fp_discard",                "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,        "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bcore_sink_s* bcore_sink_s_get_typed( tp_t o_type )
{
    return bcore_spect_get_typed( typeof( "bcore_sink_s" ), o_type );
}

const bcore_sink_s* bcore_sink_s_get_aware( vc_t obj )
{
    return bcore_sink_s_get_typed( *( const aware_t* )obj );
}

/**********************************************************************************************************************/

sz_t bcore_sink_aware_push_data    ( vd_t o, vc_t d, sz_t sz              ) { return bcore_sink_spect_push_data( bcore_sink_s_get_typed( *( aware_t* )o ), o, d, sz ); }
void bcore_sink_aware_flush        ( vd_t o                               ) { bcore_sink_spect_flush(         bcore_sink_s_get_typed( *( aware_t* )o ), o ); }
void bcore_sink_aware_pushvf       ( vd_t o, sc_t f, va_list a            ) { bcore_sink_spect_pushvf(        bcore_sink_s_get_typed( *( aware_t* )o ), o, f, a ); }
void bcore_sink_aware_push_char    ( vd_t o, char c                       ) { bcore_sink_spect_push_char(     bcore_sink_s_get_typed( *( aware_t* )o ), o, c ); }
void bcore_sink_aware_push_sc      ( vd_t o, sc_t s                       ) { bcore_sink_spect_push_sc(       bcore_sink_s_get_typed( *( aware_t* )o ), o, s ); }
void bcore_sink_aware_push_string  ( vd_t o, const bcore_string_s* s      ) { bcore_sink_spect_push_string(   bcore_sink_s_get_typed( *( aware_t* )o ), o, s ); }
void bcore_sink_aware_push_string_d( vd_t o,       bcore_string_s* s      ) { bcore_sink_spect_push_string_d( bcore_sink_s_get_typed( *( aware_t* )o ), o, s ); }
void bcore_sink_aware_set_consumer(  vd_t o, vd_t c                       ) { bcore_sink_spect_set_consumer(  bcore_sink_s_get_typed( *( aware_t* )o ), o, c ); }
void bcore_sink_aware_pushf        ( vd_t o, sc_t f, ...                  ) { va_list a; va_start( a, f ); bcore_sink_aware_pushvf( o, f, a ); va_end( a ); }

inline static vc_t w_spect( sr_s o ) { if( sr_s_is_const( &o ) ) ERR( "Attempt to modify a constant object" ); return ch_spect( o.p, TYPEOF_bcore_sink_s ); }
inline static vc_t r_spect( sr_s o ) { return ch_spect( o.p, TYPEOF_bcore_sink_s ); }

sz_t bcore_sink_push_data    ( sr_s o, vc_t d, sz_t sz )   { sz_t r = bcore_sink_spect_push_data(     w_spect( o ), o.o, d, sz ); sr_down( o ); return r; }
void bcore_sink_flush        ( sr_s o                  )   {          bcore_sink_spect_flush(         w_spect( o ), o.o        ); sr_down( o ); }
void bcore_sink_pushvf       ( sr_s o, sc_t f, va_list a ) {          bcore_sink_spect_pushvf(        w_spect( o ), o.o, f, a  ); sr_down( o ); }
void bcore_sink_pushf        ( sr_s o, sc_t f, ... )       { va_list a; va_start( a, f ); bcore_sink_pushvf( o, f, a ); va_end( a ); }
void bcore_sink_push_char    ( sr_s o, char c )            {          bcore_sink_spect_push_char(     w_spect( o ), o.o, c ); sr_down( o ); }
void bcore_sink_push_sc      ( sr_s o, sc_t s )            {          bcore_sink_spect_push_sc(       w_spect( o ), o.o, s ); sr_down( o ); }
void bcore_sink_push_string  ( sr_s o, const bcore_string_s* s ) {    bcore_sink_spect_push_string(   w_spect( o ), o.o, s ); sr_down( o ); }
void bcore_sink_push_string_d( sr_s o,       bcore_string_s* s ) {    bcore_sink_spect_push_string_d( w_spect( o ), o.o, s ); sr_down( o ); }
void bcore_sink_set_consumer ( sr_s o, vd_t c )            {          bcore_sink_spect_set_consumer(  w_spect( o ), o.o, c ); sr_down( o ); }

sz_t bcore_sink_q_push_data    ( const sr_s* o, vc_t d, sz_t sz )   { return   bcore_sink_spect_push_data(     w_spect( *o ), o->o, d, sz ); }
void bcore_sink_q_flush        ( const sr_s* o                  )   {          bcore_sink_spect_flush(         w_spect( *o ), o->o        ); }
void bcore_sink_q_pushvf       ( const sr_s* o, sc_t f, va_list a ) {          bcore_sink_spect_pushvf(        w_spect( *o ), o->o, f, a  ); }
void bcore_sink_q_pushf        ( const sr_s* o, sc_t f, ... )       { va_list a; va_start( a, f ); bcore_sink_q_pushvf( o, f, a ); va_end( a ); }
void bcore_sink_q_push_char    ( const sr_s* o, char c )            {          bcore_sink_spect_push_char(     w_spect( *o ), o->o, c ); }
void bcore_sink_q_push_sc      ( const sr_s* o, sc_t s )            {          bcore_sink_spect_push_sc(       w_spect( *o ), o->o, s ); }
void bcore_sink_q_push_string  ( const sr_s* o, const bcore_string_s* s ) {    bcore_sink_spect_push_string(   w_spect( *o ), o->o, s ); }
void bcore_sink_q_push_string_d( const sr_s* o,       bcore_string_s* s ) {    bcore_sink_spect_push_string_d( w_spect( *o ), o->o, s ); }
void bcore_sink_q_set_consumer ( const sr_s* o, vd_t c )            {          bcore_sink_spect_set_consumer(  w_spect( *o ), o->o, c ); }

/**********************************************************************************************************************/

vd_t bcore_spect_sink_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_spect_sink" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        sink_s_define_trait();
        bcore_flect_define_creator( typeof( "bcore_sink_s"  ), sink_s_create_self  );
    }

    return NULL;
}

/**********************************************************************************************************************/

