/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bclos_sig.h"
#include "bclos_quicktypes.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT( bclos_arg_sig_s )
DEFINE_FUNCTION_DOWN_FLAT( bclos_arg_sig_s )
DEFINE_FUNCTION_COPY_FLAT( bclos_arg_sig_s )
DEFINE_FUNCTION_CREATE(    bclos_arg_sig_s )
DEFINE_FUNCTION_DISCARD(   bclos_arg_sig_s )
DEFINE_FUNCTION_CLONE(     bclos_arg_sig_s )

static bcore_flect_self_s* arg_sig_s_create_self( void )
{
    sc_t def = "bclos_arg_sig_s = { tp_t type; bl_t is_const; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_arg_sig_s ) );
    return self;
}

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_SPECT( bclos_sig_s )
DEFINE_FUNCTION_DOWN_SPECT( bclos_sig_s )
DEFINE_FUNCTION_COPY_SPECT( bclos_sig_s )
DEFINE_FUNCTION_CREATE(     bclos_sig_s )
DEFINE_FUNCTION_DISCARD(    bclos_sig_s )
DEFINE_FUNCTION_CLONE(      bclos_sig_s )

void bclos_sig_s_push( bclos_sig_s* o, bclos_arg_sig_s arg_sig )
{
    if( o->size > o->space ) bcore_array_typed_make_strong( TYPEOF_bclos_sig_s, o );
    if( o->size == o->space ) bcore_un_alloc( sizeof( bclos_arg_sig_s ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    o->data[ o->size++ ] = arg_sig;
}

static bcore_flect_self_s* sig_s_create_self( void )
{
    sc_t def = "bclos_sig_s = { bclos_arg_sig_s [] arr; tp_t ret; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_sig_s ) );
    return self;
}

/**********************************************************************************************************************/

void bclos_sig_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bclos_arg_sig_s" ), arg_sig_s_create_self );
    bcore_flect_define_creator( typeof( "bclos_sig_s"     ), sig_s_create_self );
}

