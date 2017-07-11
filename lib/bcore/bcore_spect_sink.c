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

bcore_flect_self_s* bcore_sink_s_create_self()
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_sink_s" ), sizeof( bcore_sink_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_init,             "bcore_fp_init",                    "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_down,             "bcore_fp_down",                    "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_create,           "bcore_fp_create",                  "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_discard,          "bcore_fp_discard",                 "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )sink_s_create_signature, "bcore_spect_fp_create_signature",  "create_signature" );
    return self;
}

/**********************************************************************************************************************/

static void push_data( const struct bcore_sink_s* p, vd_t o, vc_t data, sz_t size )
{
    p->flow_snk( o, data, size );
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

/**********************************************************************************************************************/

static bcore_sink_s* create_from_self( const bcore_flect_self_s* self )
{
    bcore_sink_s* o = sink_s_create();
    o->o_type = self->type;
    if( !self->body ) ERR( "'%s' has no body", ifnameof( self->type ) );
    const bcore_flect_body_s* body = self->body;

    for( sz_t i = 0; i < body->size; i++ )
    {
        const bcore_flect_item_s* item = &body->data[ i ];
        if( item->caps == BCORE_CAPS_EXTERNAL_FUNC && item->type == typeof( "bcore_fp_flow_snk" ) )
        {
            o->flow_snk = ( bcore_fp_flow_snk )item->f_ptr;
            break;
        }
    }

    if( !o->flow_snk ) ERR( "'%s' has no feature 'bcore_fp_flow_snk'", ifnameof( self->type ) );

    o->push_data     = push_data;
    o->pushf         = pushf;
    o->push_char     = push_char;
    o->push_sc       = push_sc;
    o->push_string   = push_string;
    o->push_string_d = push_string_d;

    return o;
}

/**********************************************************************************************************************/

const bcore_sink_s* bcore_sink_s_get_typed( tp_t o_type )
{
    tp_t sig = bcore_signature_get_hash_na( 2, typeof( "bcore_sink_s" ), o_type );
    const bcore_sink_s* sink_p = bcore_spect_try( sig );
    if( !sink_p )
    {
        const bcore_flect_self_s* o_self = bcore_flect_get_self( o_type );
        bcore_sink_s* new_sink_p = create_from_self( o_self );
        bcore_spect_enroll_d( new_sink_p );
        sink_p = new_sink_p;
    }
    return sink_p;
}

/**********************************************************************************************************************/

