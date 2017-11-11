/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_name_manager.h"
#include "bcore_tbman.h"
#include "bcore_st.h"
#include "bcore_threads.h"
#include "bcore_hmap.h"

/**********************************************************************************************************************/
// hash map

typedef struct hnode_s
{
    tp_t name_space;
    st_s name;
} hnode_s;

static void hnode_s_init( hnode_s* o )
{
    o->name_space = 0;
    st_s_init( &o->name );
}

static void hnode_s_down( hnode_s* o )
{
    st_s_down( &o->name );
}

static hnode_s* hnode_s_create()
{
    hnode_s* o = bcore_alloc( NULL, sizeof( hnode_s ) );
    hnode_s_init( o );
    return o;
}

static void hnode_s_discard( hnode_s* o )
{
    bcore_release_obj( ( fp_t )hnode_s_down, o );
}

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
    hnode_s* node = val;
    hnode_s_discard( node );
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
        bcore_release_obj( ( fp_t )hmap_s_down, hmap_s_g );
        hmap_s_g = NULL;
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

st_s* cat_name_sn( tp_t name_space, sc_t name, sz_t n )
{
    st_s* cat_name = st_s_create();
    if( name_space )
    {
        st_s_push_st_d( cat_name, ifnameof_s( name_space ) );
        st_s_push_char( cat_name, ':' );
    }
    st_s_push_sc_n( cat_name, name, n );
    return cat_name;
}

st_s* cat_name_s( tp_t name_space, sc_t name )
{
    return cat_name_sn( name_space, name, bcore_strlen( name ) );
}

sc_t bcore_name_try_name( tp_t type )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    vd_t* vdp = bcore_hmap_u2vd_s_get( hmap_s_g->map, type );
    sc_t name = NULL;
    if( vdp )
    {
        hnode_s* node = *vdp;
        name = node->name.sc;
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

st_s* bcore_name_try_name_s( tp_t type )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    vd_t* vdp = bcore_hmap_u2vd_s_get( hmap_s_g->map, type );
    bcore_mutex_unlock( &hmap_s_g->mutex );
    if( vdp )
    {
        hnode_s* node = *vdp;
        return cat_name_sn( node->name_space, node->name.sc, node->name.size );
    }
    return NULL;
}

/// hash --> full name; returns error when not enrolled (thread safe); passes ownership
st_s* bcore_name_get_name_s( tp_t type )
{
    st_s* s = bcore_name_try_name_s( type );
    if( !s ) ERR( "hash %"PRIu32" has no name", type );
    return s;
}

tp_t bcore_name_enroll_sn( tp_t name_space, sc_t name, sz_t n )
{
    assert( hmap_s_g != NULL );
    tp_t hash = bcore_name_get_hash_sn( name_space, name, n );
    if( hash == 0 ) ERR( "Hash of '%s' is zero. Zero is a reserved value.", cat_name_sn( name_space, name, n )->sc );
    bcore_mutex_lock( &hmap_s_g->mutex );
    if( !hmap_s_g->map ) hmap_s_g->map = bcore_hmap_u2vd_s_create();
    vd_t* vdp = bcore_hmap_u2vd_s_get( hmap_s_g->map, hash );
    if( vdp )
    {
        const hnode_s* node = *vdp;
        if( node->name_space != name_space || ( !st_s_equal_sc_n( &node->name, name, n ) ) )
        {
            bcore_mutex_unlock( &hmap_s_g->mutex );
            ERR( "'%s' collides with '%s' at %"PRItp_t"", cat_name_sn( name_space, name, n )->sc, cat_name_s( node->name_space, node->name.sc )->sc, hash );
            bcore_mutex_lock( &hmap_s_g->mutex );
        }
    }
    else
    {
        if( name_space && !bcore_hmap_u2vd_s_get( hmap_s_g->map, name_space ) )
        {
            bcore_mutex_unlock( &hmap_s_g->mutex );
            ERR( "Namespace '%u' has not been enrolled\n", name_space );
            bcore_mutex_lock( &hmap_s_g->mutex );
        }
        hnode_s* node = hnode_s_create();
        node->name_space = name_space;
        st_s_copy_sc_n( &node->name, name, n );
        bcore_hmap_u2vd_s_set( hmap_s_g->map, hash, node, false );
    }
    bcore_mutex_unlock( &hmap_s_g->mutex );
    return hash;
}

tp_t bcore_name_enroll_s( tp_t name_space, sc_t name )
{
    return bcore_name_enroll_sn( name_space, name, bcore_strlen( name ) );
}

tp_t bcore_name_enroll_n( sc_t name, sz_t n )
{
    return bcore_name_enroll_sn( 0, name, n );
}

tp_t bcore_name_enroll( sc_t name )
{
    return bcore_name_enroll_sn( 0, name, bcore_strlen( name ) );
}

void bcore_name_remove( tp_t type )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    hnode_s* node = bcore_hmap_u2vd_s_remove_h( hmap_s_g->map, type );
    hnode_s_discard( node );
    bcore_mutex_unlock( &hmap_s_g->mutex );
}

sz_t  bcore_name_size()
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    sz_t size = bcore_hmap_u2vd_s_keys( hmap_s_g->map );
    bcore_mutex_unlock( &hmap_s_g->mutex );
    return size;
}

st_s* bcore_name_show()
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    st_s* log = st_s_create();
    sz_t size = bcore_hmap_u2vd_s_size( hmap_s_g->map );
    for( sz_t i = 0; i < size; i++ )
    {
        hnode_s* node = bcore_hmap_u2vd_s_idx_val( hmap_s_g->map, i );
        if( node )
        {
            bcore_mutex_unlock( &hmap_s_g->mutex );
            st_s_push_st_d( log, cat_name_s( node->name_space, node->name.sc ) );
            bcore_mutex_lock( &hmap_s_g->mutex );
            st_s_push_char( log, '\n' );
        }
    }
    bcore_mutex_unlock( &hmap_s_g->mutex );
    return log;
}

/**********************************************************************************************************************/

st_s* bcore_name_manager_selftest()
{
    tp_t mynamespace_t = entypeof( "mynamespace" );
    tp_t myname_t = entypeof_s( mynamespace_t, "myname" );
    ASSERT( myname_t == typeof( "mynamespace:myname" ) );
    st_s* cname = nameof_s( myname_t );
    ASSERT( st_s_cmp_sc( cname, "mynamespace:myname" ) == 0 );
    st_s_discard( cname );
    return NULL;
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
    else if( signal == typeof( "selftest" ) )
    {
        return bcore_name_manager_selftest();
    }
    else if( signal == typeof( "down0" ) )
    {
        // bcore_tbman_instance_disgnostics();

        if( object && ( *( bl_t* )object ) )
        {
            sz_t space = bcore_tbman_granted_space();
            name_manager_close();
            bcore_msg( "  name mananger ....... % 6zu\n", space - bcore_tbman_granted_space() );
        }
        else
        {
            name_manager_close();
        }
    }
    return NULL;
}

