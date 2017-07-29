/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_source.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_spect.h"

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
    source_s_down( o );
    bcore_free( o );
}

static bcore_signature_s* source_s_create_signature( bcore_source_s* o )
{
    return bcore_signature_s_create_an( 2, o->p_type, o->o_type );
}

/**********************************************************************************************************************/

static sz_t get_data( const bcore_source_s* p, vd_t o, vd_t data, sz_t size )
{
    return p->fp_flow_src( o, data, size );
}

static void parse_errvf( const bcore_source_s* p, vd_t o, sc_t format, va_list args )
{
    if( p->fp_parse_errvf )
    {
        p->fp_parse_errvf( o, format, args );
    }
    else
    {
        ERR( "Parse error:\n%s\n", bcore_string_s_createvf( format, args )->sc );
    }
}

static void parse_errf( const bcore_source_s* p, vd_t o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    parse_errvf( p, o, format, args );
    va_end( args );
}

static void parsevf( const bcore_source_s* p, vd_t o, sc_t format, va_list args )
{
    if( p->fp_parse_errvf )
    {
        p->fp_parsevf( o, format, args );
    }
    else
    {
        ERR( "Object '%s' does not support feature 'bcore_source_fp_parsevf'.", nameof( p->o_type ) );
    }
}

static void parsef( const bcore_source_s* p, vd_t o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    parsevf( p, o, format, args );
    va_end( args );
}

static bool parse_boolf( const bcore_source_s* p, vd_t o, sc_t format )
{
    bool flag = false;
    parsef( p, o, format, &flag );
    return flag;
}

static void set_supplier( const bcore_source_s* p, vd_t o, vd_t supplier )
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

static bcore_source_s* create_from_self( const bcore_flect_self_s** p_self )
{
    assert( p_self != NULL );
    const bcore_flect_self_s* self = *p_self;
    assert( self != NULL );
    assert( bcore_flect_self_s_is_aware( self ) );

    bcore_source_s* o = source_s_create();
    o->o_type = self->type;
    o->fp_flow_src     = ( bcore_fp_flow_src            )bcore_flect_self_s_get_external_fp( self, bcore_name_enroll( "bcore_fp_flow_src" ), 0 );
    o->fp_parsevf      = ( bcore_source_fp_parsevf      )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_parsevf" ), 0 );
    o->fp_parse_errvf  = ( bcore_fp_logvf               )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bcore_fp_logvf" ), bcore_name_enroll( "p_errorvf" ) );
    o->fp_set_supplier = ( bcore_source_fp_set_supplier )bcore_flect_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_set_supplier" ), 0 );

    o->get_data     = get_data;
    o->parsevf      = parsevf;
    o->parsef       = parsef;
    o->parse_errvf  = parse_errvf;
    o->parse_errf   = parse_errf;
    o->parse_boolf  = parse_boolf;
    o->set_supplier = set_supplier;
    return o;
}

bcore_flect_self_s* bcore_source_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_source_s" ), sizeof( bcore_source_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )source_s_init,             "bcore_fp_init",                    "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )source_s_down,             "bcore_fp_down",                    "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )source_s_create,           "bcore_fp_create",                  "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )source_s_discard,          "bcore_fp_discard",                 "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )source_s_create_signature, "bcore_spect_fp_create_signature",  "create_signature" );
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

sz_t bcore_source_get_data( vd_t o, vd_t data, sz_t size )
{
    const bcore_source_s* p = bcore_source_s_get_typed( *( aware_t* )o );
    return p->get_data( p, o, data, size );
}

void bcore_source_parsevf( vd_t o, sc_t format, va_list args )
{
    const bcore_source_s* p = bcore_source_s_get_typed( *( aware_t* )o );
    if( p->fp_parse_errvf )
    {
        p->fp_parsevf( o, format, args );
    }
    else
    {
        ERR( "Object '%s' does not support feature 'bcore_source_fp_parsevf'.", nameof( p->o_type ) );
    }
}

void bcore_source_parsef( vd_t o, sc_t format, ... )
{
    const bcore_source_s* p = bcore_source_s_get_typed( *( aware_t* )o );
    va_list args;
    va_start( args, format );
    if( p->fp_parse_errvf )
    {
        p->fp_parsevf( o, format, args );
    }
    else
    {
        ERR( "Object '%s' does not support feature 'bcore_source_fp_parsevf'.", nameof( p->o_type ) );
    }
    va_end( args );
}

void bcore_source_parse_errvf(  vd_t o, sc_t format, va_list args )
{
    const bcore_source_s* p = bcore_source_s_get_typed( *( aware_t* )o );
    if( p->fp_parse_errvf )
    {
        p->fp_parse_errvf( o, format, args );
    }
    else
    {
        ERR( "Parse error:\n%s\n", bcore_string_s_createvf( format, args )->sc );
    }
}

void bcore_source_parse_errf(  vd_t o, sc_t format, ... )
{
    const bcore_source_s* p = bcore_source_s_get_typed( *( aware_t* )o );
    va_list args;
    va_start( args, format );
    if( p->fp_parse_errvf )
    {
        p->fp_parse_errvf( o, format, args );
    }
    else
    {
        ERR( "Parse error:\n%s\n", bcore_string_s_createvf( format, args )->sc );
    }
    va_end( args );
}

bool bcore_source_parse_boolf( vd_t o, sc_t format )
{
    return parse_boolf( bcore_source_s_get_typed( *( aware_t* )o ), o, format );
}

void bcore_source_set_supplier( vd_t o, vd_t supplier )
{
    const bcore_source_s* p = bcore_source_s_get_typed( *( aware_t* )o );
    p->set_supplier( p, o, supplier );
}
