/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_function_manager.h"
#include "bcore_tbman.h"
#include "bcore_st.h"
#include "bcore_threads.h"
#include "bcore_hmap.h"

/**********************************************************************************************************************/
// hash map

typedef struct hmap_s
{
    bcore_hmap_tpfp_s* map;
    bcore_mutex_t mutex;
} hmap_s;

static void hmap_s_init( hmap_s* o )
{
    o->map = NULL;
    bcore_mutex_init( &o->mutex );
}

static void hmap_s_down( hmap_s* o )
{
    bcore_mutex_lock( &o->mutex );
    bcore_hmap_tpfp_s_discard( o->map );
    o->map = NULL;
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
        bcore_release_obj( ( fp_t )hmap_s_down, hmap_s_g );
        hmap_s_g = NULL;
    }
}

static void function_manager_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, create_hmap_s );
}

static void function_manager_close()
{
    discard_hmap_s();
}

fp_t bcore_function_get( tp_t t )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    fp_t* vdp = bcore_hmap_tpfp_s_get( hmap_s_g->map, t );
    fp_t fp = vdp ? *vdp : NULL;
    bcore_mutex_unlock( &hmap_s_g->mutex );
    return fp;
}

void bcore_function_set( tp_t t, fp_t f )
{
    assert( hmap_s_g != NULL );
    if( t == 0 ) ERR( "Type is zero. Zero is a reserved value." );
    bcore_mutex_lock( &hmap_s_g->mutex );
    if( !hmap_s_g->map ) hmap_s_g->map = bcore_hmap_tpfp_s_create();
    fp_t* vdp = bcore_hmap_tpfp_s_get( hmap_s_g->map, t );
    if( vdp )
    {
        if( *vdp != f )
        {
            ERR( "Under type '%s' (%"PRItp_t") a different function has already been enrolled.", ifnameof( t ), t );
        }
    }
    else
    {
        bcore_hmap_tpfp_s_set( hmap_s_g->map, t, f );
    }
    bcore_mutex_unlock( &hmap_s_g->mutex );
}

void bcore_function_remove( tp_t t )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    bcore_hmap_tpfp_s_remove( hmap_s_g->map, t );
    bcore_mutex_unlock( &hmap_s_g->mutex );
}

/**********************************************************************************************************************/
// signal

vd_t bcore_function_manager_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_function_manager" ) ) return NULL;
    if( signal == typeof( "init0" ) )
    {
        function_manager_open();
    }
    else if( signal == typeof( "down0" ) )
    {
        if( object && ( *( bl_t* )object ) )
        {
            sz_t space = bcore_tbman_granted_space();
            function_manager_close();
            bcore_msg( "  function manager .... % 6zu\n", space - bcore_tbman_granted_space() );
        }
        else
        {
            function_manager_close();
        }
    }
    return NULL;
}

