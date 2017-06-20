/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_sink_perspective.h"
#include "bcore_flect.h"

/**********************************************************************************************************************/
// bcore_sink_s

void bcore_sink_s_down( bcore_sink_s* o );

void bcore_sink_s_init( bcore_sink_s* o )
{
    bcore_memzero( o, sizeof( bcore_sink_s ) );
    bcore_perspective_s_init( &o->_, bcore_sink_s_down );
}

void bcore_sink_s_down( bcore_sink_s* o )
{
}

bcore_sink_s* bcore_sink_s_create()
{
    bcore_sink_s* o = bcore_alloc( NULL, sizeof( bcore_sink_s ) );
    bcore_sink_s_init( o );
    return o;
}

void bcore_sink_s_discard( bcore_sink_s* o )
{
    if( !o ) return;
    bcore_sink_s_down( o );
    bcore_free( o );
}

/**********************************************************************************************************************/

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
    bcore_sink_s* o = bcore_sink_s_create();
    o->_.p_type = bcore_name_enroll( "bcore_sink_s" );
    o->_.o_type = self->type;
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

    o->pushf         = pushf;
    o->push_char     = push_char;
    o->push_sc       = push_sc;
    o->push_string   = push_string;
    o->push_string_d = push_string_d;

    return o;
}

/**********************************************************************************************************************/

const bcore_sink_s* bcore_sink_s_get_typed( u2_t o_type )
{
    u2_t p_type = typeof( "bcore_sink_s" );
    const bcore_sink_s* perspective = ( const bcore_sink_s* )bcore_perspective_try_perspective( p_type, o_type );
    if( !perspective )
    {
        const bcore_flect_self_s* o_self = bcore_flect_get_self( o_type );
        bcore_sink_s* new_perspective = create_from_self( o_self );
        bcore_perspective_enroll( p_type, o_type, ( bcore_perspective_s* )new_perspective );
        perspective = new_perspective;
    }
    return perspective;
}

/**********************************************************************************************************************/

