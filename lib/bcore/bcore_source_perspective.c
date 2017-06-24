/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_source_perspective.h"
#include "bcore_instance_perspective.h"
#include "bcore_flect.h"

/**********************************************************************************************************************/
// bcore_source_s

void bcore_source_s_down( bcore_source_s* o );

void bcore_source_s_init( bcore_source_s* o )
{
    bcore_memzero( o, sizeof( bcore_source_s ) );
    bcore_perspective_s_init( &o->_, bcore_source_s_down );
}

void bcore_source_s_down( bcore_source_s* o )
{
}

bcore_source_s* bcore_source_s_create()
{
    bcore_source_s* o = bcore_alloc( NULL, sizeof( bcore_source_s ) );
    bcore_source_s_init( o );
    return o;
}

void bcore_source_s_discard( bcore_source_s* o )
{
    if( !o ) return;
    bcore_source_s_down( o );
    bcore_free( o );
}

/**********************************************************************************************************************/

static void parse_errf( const bcore_source_s* p, vd_t o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    p->fp_parse_errvf( o, format, args );
    va_end( args );
}

static void parsef( const bcore_source_s* p, vd_t o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    p->fp_parsevf( o, format, args );
    va_end( args );
}

static bool parse_boolf( const bcore_source_s* p, vd_t o, sc_t format )
{
    bool flag = false;
    parsef( p, o, format, &flag );
    return flag;
}

/**********************************************************************************************************************/

static bcore_source_s* create_from_self( const bcore_flect_self_s* self )
{
    bcore_source_s* o = bcore_source_s_create();
    o->_.p_type = bcore_name_enroll( "bcore_source_s" );
    o->_.o_type = self->type;

    const bcore_flect_body_s* flect_body = self->body;

    for( sz_t i = 0; i < flect_body->size; i++ )
    {
        const bcore_flect_item_s* flect_item = &flect_body->data[ i ];

        if( flect_item->caps == BCORE_CAPS_EXTERNAL_FUNC &&
            flect_item->type == typeof( "bcore_fp_flow_src" ) )
        {
            o->fp_flow_src = ( bcore_fp_flow_src )flect_item->f_ptr;
        }

        if( flect_item->caps == BCORE_CAPS_EXTERNAL_FUNC &&
            flect_item->type == typeof( "bcore_source_fp_parsevf" ) )
        {
            o->fp_parsevf = ( bcore_source_fp_parsevf )flect_item->f_ptr;
        }

        if( flect_item->caps == BCORE_CAPS_EXTERNAL_FUNC &&
            flect_item->type == typeof( "bcore_fp_logvf" ) &&
            flect_item->name == typeof( "p_errorvf" ) )
        {
            o->fp_parse_errvf = ( bcore_fp_logvf )flect_item->f_ptr;
        }
    }

    if( !o->fp_flow_src    ) ERR( "%s needs feature 'bcore_fp_flow_src'",       ifnameof( self->type ) );
    if( !o->fp_parsevf     ) ERR( "%s needs feature 'bcore_source_fp_parsevf'", ifnameof( self->type ) );
    if( !o->fp_parse_errvf ) ERR( "%s needs feature 'bcore_fp_logvf' of name 'p_errorvf' to be used as error message handler", ifnameof( self->type ) );

    o->parsef      = parsef;
    o->parse_errf  = parse_errf;
    o->parse_boolf = parse_boolf;

    return o;
}

/**********************************************************************************************************************/

const bcore_source_s* bcore_source_s_get_typed( u2_t o_type )
{
    u2_t p_type = typeof( "bcore_source_s" );
    const bcore_source_s* perspective = ( const bcore_source_s* )bcore_perspective_try_perspective( p_type, o_type );
    if( !perspective )
    {
        const bcore_flect_self_s* o_self = bcore_flect_get_self( o_type );
        bcore_source_s* new_perspective = create_from_self( o_self );
        bcore_perspective_enroll( p_type, o_type, ( bcore_perspective_s* )new_perspective );
        perspective = new_perspective;
    }
    return perspective;
}

/**********************************************************************************************************************/

