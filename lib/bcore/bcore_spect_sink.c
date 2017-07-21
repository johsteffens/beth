/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_sink.h"
#include "bcore_flect.h"
#include "bcore_spect.h"

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

static bcore_signature_s* sink_s_create_signature( bcore_sink_s* o )
{
    return bcore_signature_s_create_an( 2, o->p_type, o->o_type );
}

/**********************************************************************************************************************/

static sz_t push_data( const struct bcore_sink_s* p, vd_t o, vc_t data, sz_t size )
{
    return p->flow_snk( o, data, size );
}

static void flush( const struct bcore_sink_s* p, vd_t o )
{
    if( p->fp_flush ) p->fp_flush( o );
}

static void pushf( const struct bcore_sink_s* p, vd_t o, sc_t format, ... )
{
    bcore_string_s* s = bcore_string_s_create();
    va_list args;
    va_start( args, format );
    bcore_string_s_initvf( s, format, args );
    va_end( args );
    p->flow_snk( o, s->data, s->size );
    bcore_string_s_discard( s );
}

static void push_char( const struct bcore_sink_s* p, vd_t o, char c )
{
    p->flow_snk( o, &c, 1 );
}

static void push_sc( const struct bcore_sink_s* p, vd_t o, sc_t sc )
{
    while( *sc )
    {
        push_char( p, o, *sc );
        sc++;
    }
}

static void push_string( const struct bcore_sink_s* p, vd_t o, const bcore_string_s* string )
{
    p->flow_snk( o, string->data, string->size );
}

static void push_string_d( const struct bcore_sink_s* p, vd_t o, bcore_string_s* string )
{
    p->flow_snk( o, string->data, string->size );
    bcore_string_s_discard( string );
}

static void set_consumer( const bcore_sink_s* p, vd_t o, vd_t consumer )
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

static bcore_sink_s* create_from_self( const bcore_flect_self_s** p_self )
{
    assert( p_self != NULL );
    const bcore_flect_self_s* self = *p_self;
    assert( self != NULL );
    assert( bcore_flect_self_s_is_aware( self ) );

    bcore_sink_s* o = sink_s_create();
    o->o_type = self->type;
    o->flow_snk        = ( bcore_fp_flow_snk          )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_fp_flow_snk" ), 0 );
    o->fp_set_consumer = ( bcore_sink_fp_set_consumer )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bcore_sink_fp_set_consumer" ), 0 );
    o->fp_flush        = ( bcore_sink_fp_flush        )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bcore_sink_fp_flush" ), 0 );

    o->push_data     = push_data;
    o->flush         = flush;
    o->pushf         = pushf;
    o->push_char     = push_char;
    o->push_sc       = push_sc;
    o->push_string   = push_string;
    o->push_string_d = push_string_d;
    o->set_consumer  = set_consumer;
    return o;
}

bcore_flect_self_s* bcore_sink_s_create_self()
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_sink_s" ), sizeof( bcore_sink_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_init,             "bcore_fp_init",                   "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_down,             "bcore_fp_down",                   "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_create,           "bcore_fp_create",                 "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_discard,          "bcore_fp_discard",                "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_create_signature, "bcore_spect_fp_create_signature", "create_signature" );
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

sz_t bcore_sink_push_data( vd_t o, vc_t data, sz_t size )
{
    return push_data( bcore_sink_s_get_typed( *( aware_t* )o ), o, data, size );
}

void bcore_sink_flush( vd_t o )
{
    flush( bcore_sink_s_get_typed( *( aware_t* )o ), o );
}

void bcore_sink_set_consumer( vd_t o, vd_t consumer )
{
    set_consumer( bcore_sink_s_get_typed( *( aware_t* )o ), o, consumer );
}

/**********************************************************************************************************************/

