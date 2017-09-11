/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/**********************************************************************************************************************/
// trait system

#include "bcore_hmap.h"
#include "bcore_threads.h"
#include "bcore_memory_manager.h"

typedef struct system_s
{
    bcore_hmap_tptp_s map;
    bcore_mutex_t mutex;
} system_s;

static void system_s_init( system_s* o )
{
    bcore_hmap_tptp_s_init( &o->map );
    bcore_mutex_init( &o->mutex );
}

static void system_s_down( system_s* o )
{
    bcore_mutex_lock( &o->mutex );
    bcore_hmap_tptp_s_down( &o->map );
    bcore_mutex_unlock( &o->mutex );
    bcore_mutex_down( &o->mutex );
}

/**********************************************************************************************************************/

static system_s system_s_g;
static void system_s_g_init()   { system_s_init( &system_s_g ); }
static void system_s_g_down()   { system_s_down( &system_s_g ); }
static void system_s_g_lock()   { bcore_mutex_lock( &system_s_g.mutex   ); }
static void system_s_g_unlock() { bcore_mutex_unlock( &system_s_g.mutex ); }

static void trait_manager_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, system_s_g_init );
}

static void trait_manager_close()
{
    system_s_g_down();
}

static bl_t trait_is( tp_t trait, tp_t ancestor )
{
    for(;;)
    {
        if( trait == ancestor ) return true;
        tp_t* parent = bcore_hmap_tptp_s_get( &system_s_g.map, trait );
        if( !parent || !(*parent) ) return false;
        trait = *parent;
    }
}

void bcore_trait_set( tp_t trait, tp_t parent )
{
    system_s_g_lock();
    if( trait_is( parent, trait ) )
    {
        system_s_g_unlock();
        ERR( "trait '%s' is already ancestor to parent '%s'", ifnameof( trait ), ifnameof( parent ) );
    }
    bcore_hmap_tptp_s_set( &system_s_g.map, trait, parent );
    system_s_g_unlock();
}

bl_t bcore_trait_exists( tp_t trait )
{
    system_s_g_lock();
    bl_t flag = bcore_hmap_tptp_s_exists( &system_s_g.map, trait );
    system_s_g_unlock();
    return flag;
}

tp_t bcore_trait_parent( tp_t trait )
{
    system_s_g_lock();
    tp_t* parent = bcore_hmap_tptp_s_get( &system_s_g.map, trait );
    system_s_g_unlock();
    return parent ? *parent : 0;
}

bl_t bcore_trait_is( tp_t trait, tp_t ancestor )
{
    system_s_g_lock();
    bl_t flag = trait_is( trait, ancestor );
    system_s_g_unlock();
    return flag;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_trait_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_trait" ) ) return NULL;
    if( signal == typeof( "init0" ) )
    {
        trait_manager_open();
    }
    else if( signal == typeof( "init1" ) )
    {
    }
    else if( signal == typeof( "down0" ) )
    {
        sz_t space = bcore_memory_manager_granted_space();
        trait_manager_close();
        bcore_msg( "  trait mananger ...... % 6zu\n", space - bcore_memory_manager_granted_space() );
    }
    return NULL;
}
