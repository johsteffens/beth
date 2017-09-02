/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bclos_args.h"
#include "bclos_quicktypes.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_SPECT( bclos_args_s )
DEFINE_FUNCTION_DOWN_SPECT( bclos_args_s )
DEFINE_FUNCTION_COPY_SPECT( bclos_args_s )
DEFINE_FUNCTION_CREATE(     bclos_args_s )
DEFINE_FUNCTION_DISCARD(    bclos_args_s )
DEFINE_FUNCTION_CLONE(      bclos_args_s )

void bclos_args_s_push( bclos_args_s* o, sr_s sr )
{
    if( o->size > o->space ) bcore_array_typed_make_strong( TYPEOF_bclos_args_s, o );
    if( o->size == o->space ) bcore_un_alloc( sizeof( sr_s ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    o->data[ o->size++ ] = sr;
}

bclos_args_s bclos_args_s_weak_crop( const bclos_args_s* src, sz_t start, sz_t end )
{
    bclos_args_s args;
    bclos_args_s_init( &args );
    if( end <= start || start >= src->size ) return args;
    args.data = src->data + start;
    args.size = ( end <= src->size ? end : src->size ) - start;
    return args;
}

static bcore_flect_self_s* args_s_create_self( void )
{
    sc_t def = "bclos_args_s = { sr_s [] arr; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_args_s ) );
    return self;
}

void bclos_args_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bclos_args_s"         ), args_s_create_self );
}

/**********************************************************************************************************************/

