/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_life.h"
#include "bcore_control.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT( bcore_life_item_s )

void bcore_life_item_s_down( bcore_life_item_s* o )
{
    if( o->object && o->discard ) o->discard( o->object );
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

void* bcore_life_s_push( bcore_life_s* o, bcore_fp_discard discard, void* object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->discard  = discard ;
    item->object = object;
    return object;
}

void* bcore_life_s_push_free( bcore_life_s* o, void* object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->discard  = ( fp_t )bcore_free;
    item->object = object;
    return object;
}
