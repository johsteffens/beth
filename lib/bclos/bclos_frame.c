/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_quicktypes.h"
#include "bclos_quicktypes.h"
#include "bclos_frame.h"

void bclos_frame_s_init( bclos_frame_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void bclos_frame_s_down( bclos_frame_s* o )
{
    bcore_inst_typed_down( TYPEOF_bclos_frame_s, o );
}

void bclos_frame_s_copy( bclos_frame_s* o, const bclos_frame_s* src )
{
    bcore_inst_typed_copy( TYPEOF_bclos_frame_s, o, src );
}

bclos_frame_s* bclos_frame_s_create()
{
    return bcore_inst_typed_create( TYPEOF_bclos_frame_s );
}

bclos_frame_s* bclos_frame_s_clone( const bclos_frame_s* o )
{
    return bcore_inst_typed_clone( TYPEOF_bclos_frame_s, o );
}

void bclos_frame_s_discard( bclos_frame_s* o )
{
    bcore_inst_typed_discard( TYPEOF_bclos_frame_s, o );
}

sr_s* bclos_frame_s_get( bclos_frame_s* o, tp_t name )
{
    sr_s* val = bcore_hmap_tp_sr_s_get( &o->map, name );
    if( !val && o->external ) val = bclos_frame_s_get( o->external, name );
    if( !val ) ERR( "Address '%s' does not exist.", ifnameof( name ) );
    return val;
}

sr_s* bclos_frame_s_set( bclos_frame_s* o, tp_t name, sr_s val )
{
    return bcore_hmap_tp_sr_s_set( &o->map, name, val );
}

sr_s bclos_frame_s_remove( bclos_frame_s* o, tp_t name )
{
    return bcore_hmap_tp_sr_s_remove( &o->map, name );
}

bcore_flect_self_s* frame_s_create_self( void )
{
    sc_t def =
    "bclos_frame_s = "
    "{ "
        "aware_t _;"
        "private bclos_frame_s* external;"
        "bcore_hmap_tp_sr_s map;"
    "}";

    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_frame_s ) );
    return self;
}

/**********************************************************************************************************************/

DEFINE_IDC_FUNCTIONS_FLAT( bclos_address_s )
DEFINE_CDC_FUNCTIONS(      bclos_address_s )
bclos_address_s* bclos_address_s_create_tp( tp_t name )
{
    bclos_address_s* o = bclos_address_s_create();
    o->name = name;
    return o;
}

DEFINE_CREATE_SELF( bclos_address_s, "bclos_address_s = { tp_t name; }" )

/**********************************************************************************************************************/

vd_t bclos_frame_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_frame" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_address_s" ), bclos_address_s_create_self );
        bcore_flect_define_creator( typeof( "bclos_frame_s" ), frame_s_create_self );
    }

    return NULL;
}

