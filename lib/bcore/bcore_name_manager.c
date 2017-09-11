/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_name_manager.h"
#include "bcore_memory_manager.h"
#include "bcore_string.h"
#include "bcore_threads.h"
#include "bcore_hmap.h"

/**********************************************************************************************************************/
// hash map

typedef struct hmap_s
{
    bcore_hmap_u2vd_s* map;
    bcore_mutex_t mutex;
} hmap_s;

static void hmap_s_init( hmap_s* o )
{
    o->map = NULL;
    bcore_mutex_init( &o->mutex );
}

static void hmap_s_node_down( vd_t obj, u2_t key, vd_t val )
{
    bcore_string_s* string = val;
    bcore_string_s_discard( string );
}

static void hmap_s_down( hmap_s* o )
{
    bcore_mutex_lock( &o->mutex );
    if( o->map )
    {
        bcore_hmap_u2vd_s_run_c( o->map, NULL, hmap_s_node_down );
        bcore_hmap_u2vd_s_discard( o->map );
        o->map = NULL;
    }
    bcore_mutex_unlock( &o->mutex );
    bcore_mutex_down( &o->mutex );
}

/**********************************************************************************************************************/

static hmap_s* hmap_s_g = NULL;

static void create_hmap_s()
{
    if( hmap_s_g == NULL )
    {
        hmap_s_g = bcore_alloc( NULL, sizeof( hmap_s ) );
        hmap_s_init( hmap_s_g );
    }
}

static void discard_hmap_s()
{
    if( hmap_s_g )
    {
        hmap_s_down( hmap_s_g );
        hmap_s_g = bcore_free( hmap_s_g );
    }
}

static void name_manager_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, create_hmap_s );
}

static void name_manager_close()
{
    discard_hmap_s();
}

sc_t bcore_name_try_name( tp_t type )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    vd_t* vdp = bcore_hmap_u2vd_s_get( hmap_s_g->map, type );
    sc_t name = NULL;
    if( vdp )
    {
        bcore_string_s* s = *vdp;
        name = s->sc;
    }
    bcore_mutex_unlock( &hmap_s_g->mutex );
    return name;
}

sc_t bcore_name_get_name( tp_t type )
{
    sc_t name = bcore_name_try_name( type );
    if( !name ) ERR( "hash %"PRIu32" has no name", type );
    return name;
}

tp_t bcore_name_enroll( sc_t name )
{
    assert( hmap_s_g != NULL );
    u2_t hash = bcore_name_get_hash( name );
    if( hash == 0 ) ERR( "Hash of '%s' is zero. Zero is a reserved value.", name );
    bcore_mutex_lock( &hmap_s_g->mutex );
    if( !hmap_s_g->map ) hmap_s_g->map = bcore_hmap_u2vd_s_create();
    vd_t* vdp = bcore_hmap_u2vd_s_get( hmap_s_g->map, hash );
    if( vdp )
    {
        const bcore_string_s* string = *vdp;
        if( !bcore_string_s_equal_sc( string, name ) ) ERR( "%s collides with %s", name, string->sc );
    }
    else
    {
        // name manager owns string because lifetime of name management exceeds that of perspective management
        bcore_hmap_u2vd_s_set( hmap_s_g->map, hash, bcore_string_s_create_sc( name ), false );
    }
    bcore_mutex_unlock( &hmap_s_g->mutex );
    return hash;
}

void bcore_name_remove( tp_t type )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    bcore_string_s* s = bcore_hmap_u2vd_s_remove_h( hmap_s_g->map, type );
    bcore_string_s_discard( s );
    bcore_mutex_unlock( &hmap_s_g->mutex );
}

/**********************************************************************************************************************/
// signal

vd_t bcore_name_manager_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_name_manager" ) ) return NULL;
    if( signal == typeof( "init0" ) )
    {
        name_manager_open();
    }
    else if( signal == typeof( "down0" ) )
    {
        sz_t space = bcore_memory_manager_granted_space();
        name_manager_close();
        bcore_msg( "  name mananger ....... % 6zu\n", space - bcore_memory_manager_granted_space() );
    }
    return NULL;
}

