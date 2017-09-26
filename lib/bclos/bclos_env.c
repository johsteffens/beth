/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_spect_closure.h"
#include "bcore_quicktypes.h"
#include "bclos_quicktypes.h"
#include "bclos_env.h"

void bclos_env_s_init( bclos_env_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void bclos_env_s_down( bclos_env_s* o )
{
    bcore_inst_typed_down( TYPEOF_bclos_env_s, o );
}

void bclos_env_s_copy( bclos_env_s* o, const bclos_env_s* src )
{
    bcore_inst_typed_copy( TYPEOF_bclos_env_s, o, src );
}

bclos_env_s* bclos_env_s_create()
{
    return bcore_inst_typed_create( TYPEOF_bclos_env_s );
}

bclos_env_s* bclos_env_s_clone( const bclos_env_s* o )
{
    return bcore_inst_typed_clone( TYPEOF_bclos_env_s, o );
}

void bclos_env_s_discard( bclos_env_s* o )
{
    bcore_inst_typed_discard( TYPEOF_bclos_env_s, o );
}

sr_s* bclos_env_s_get( bclos_env_s* o, tp_t name )
{
    sr_s* val = bcore_hmap_tp_sr_s_get( &o->map, name );
    if( !val && o->external ) val = bclos_env_s_get( o->external, name );
    return val;
}

sr_s* bclos_env_s_set( bclos_env_s* o, tp_t name, sr_s val )
{
    return bcore_hmap_tp_sr_s_set( &o->map, name, val );
}

sr_s bclos_env_s_remove( bclos_env_s* o, tp_t name )
{
    return bcore_hmap_tp_sr_s_remove( &o->map, name );
}

bcore_flect_self_s* env_s_create_self( void )
{
    sc_t def =
    "bclos_env_s = "
    "{ "
        "aware_t _;"
        "private bclos_env_s* external;"
        "bcore_hmap_tp_sr_s map;"
    "}";

    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_env_s ) );
    return self;
}

vd_t bclos_env_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_env" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_env_s" ), env_s_create_self );
    }

    return NULL;
}

