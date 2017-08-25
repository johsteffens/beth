/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_life.h"
#include "bcore_control.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT( bcore_life_item_s )

void bcore_life_item_s_down( bcore_life_item_s* o )
{
    if( o->object )
    {
        if( o->discard )
        {
            o->discard( o->object );
        }
        else if( o->type )
        {
            bcore_inst_typed_discard( o->type, o->object );
        }
    }
}

DEFINE_FUNCTION_COPY_FLAT( bcore_life_item_s )
DEFINE_FUNCTION_CREATE(    bcore_life_item_s )
DEFINE_FUNCTION_DISCARD(   bcore_life_item_s )
DEFINE_FUNCTION_CLONE(     bcore_life_item_s )

/**********************************************************************************************************************/

void bcore_life_s_init( struct bcore_life_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void bcore_life_s_down( struct bcore_life_s* o )
{
    if( o->data )
    {
        for( sz_t i = 0; i < o->size; i++ ) bcore_life_item_s_down( &o->data[ i ] );
        bcore_free( o->data );
    }
}

DEFINE_FUNCTION_CREATE(  bcore_life_s )
DEFINE_FUNCTION_DISCARD( bcore_life_s )

bcore_life_item_s* bcore_life_s_push_item( bcore_life_s* o )
{
    if( !o->data )
    {
        o->space = 4;
        o->data = bcore_alloc( NULL, o->space * sizeof( bcore_life_item_s ) );
    }
    else if( o->size == o->space )
    {
        o->space = o->space * 2;
        o->data = bcore_alloc( o->data, o->space * sizeof( bcore_life_item_s ) );
    }
    bcore_life_item_s* r = &o->data[ o->size ];
    o->size++;
    bcore_life_item_s_init( r );
    return r;
}

vd_t bcore_life_s_push( bcore_life_s* o, bcore_fp_discard discard, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->discard = discard;
    item->object  = object;
    return object;
}

vd_t bcore_life_s_push_typed( bcore_life_s* o, tp_t type, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->type   = type;
    item->object = object;
    return object;
}

sr_s bcore_life_s_push_sr( bcore_life_s* o, sr_s object )
{
    if( sr_s_is_strong( &object ) )
    {
        return sr_pocs( object.p, bcore_life_s_push_typed( o, sr_type( object ), object.o ), sr_s_is_const( &object ), false );
    }
    else
    {
        return object;
    }
}

vd_t bcore_life_s_push_aware( bcore_life_s* o, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->discard = bcore_inst_aware_discard;
    item->object  = object;
    return object;
}

vd_t bcore_life_s_push_free( bcore_life_s* o, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->discard = ( fp_t )bcore_free;
    item->object  = object;
    return object;
}

vd_t bcore_life_s_typed_create( bcore_life_s* o, tp_t type )
{
    return bcore_life_s_push_typed( o, type, bcore_inst_typed_create( type ) );
}

/**********************************************************************************************************************/

bcore_flect_self_s* bcore_life_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( " bcore_life_s =  { aware_t _; private vd_t data; private sz_t size; private sz_t space; }", sizeof( bcore_life_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_life_s_init, "bcore_fp_init",  "init" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_life_s_down, "bcore_fp_down",  "down" );
    return self;
}
