/** Copyright 2017 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "bcore_name_manager.h"
#include "bcore_tbman.h"
#include "bcore_st.h"
#include "bcore_threads.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/
// hash map

typedef struct hmap_s
{
    bcore_name_map_s map;
    bcore_mutex_s mutex;
} hmap_s;

static void hmap_s_init( hmap_s* o )
{
    bcore_name_map_s_init( &o->map );
    bcore_mutex_s_init( &o->mutex );
}

static void hmap_s_down( hmap_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    bcore_name_map_s_down( &o->map );
    bcore_mutex_s_unlock( &o->mutex );
    bcore_mutex_s_down( &o->mutex );
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
    static bcore_once_s flag = bcore_once_init;
    bcore_once_s_run( &flag, create_hmap_s );
}

static void name_manager_close()
{
    discard_hmap_s();
}

st_s* cat_name_ns_n( tp_t name_space, sc_t name, uz_t n )
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

st_s* cat_name_ns( tp_t name_space, sc_t name )
{
    return cat_name_ns_n( name_space, name, bcore_strlen( name ) );
}

sc_t bcore_name_try_name( tp_t type )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_s_lock( &hmap_s_g->mutex );
    bcore_name_s* name = bcore_name_map_s_get( &hmap_s_g->map, type );
    bcore_mutex_s_unlock( &hmap_s_g->mutex );
    return name ? name->name : NULL;
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
    bcore_mutex_s_lock( &hmap_s_g->mutex );
    bcore_name_s* name = bcore_name_map_s_get( &hmap_s_g->map, type );
    bcore_mutex_s_unlock( &hmap_s_g->mutex );

    if( name )
    {
        return cat_name_ns( name->name_space, name->name );
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

tp_t bcore_name_enroll_sn( tp_t name_space, sc_t name, uz_t n )
{
    assert( hmap_s_g != NULL );
    tp_t key = bcore_name_key_ns_n( name_space, name, n );
    if( key == 0 ) ERR( "Hash of '%s' is zero. Zero is a reserved value.", cat_name_ns_n( name_space, name, n )->sc );
    bcore_mutex_s_lock( &hmap_s_g->mutex );
    bcore_name_s* node = bcore_name_map_s_get( &hmap_s_g->map, key );
    if( node )
    {
        if( node->name_space != name_space || ( bcore_strcmp_n( name, n, node->name, bcore_strlen( node->name ) ) != 0 ) )
        {
            bcore_mutex_s_unlock( &hmap_s_g->mutex );
            ERR( "'%s' collides with '%s' at %"PRItp_t"", cat_name_ns_n( name_space, name, n )->sc, cat_name_ns( node->name_space, node->name )->sc, key );
            bcore_mutex_s_lock( &hmap_s_g->mutex );
        }
    }
    else
    {
        if( name_space && !bcore_name_map_s_exists( &hmap_s_g->map, name_space ) ) ERR( "Namespace %"PRItp_t" not found", name_space );
        bcore_name_map_s_set( &hmap_s_g->map, bcore_name_ns_sc_n( name_space, name, n ) );
    }
    bcore_mutex_s_unlock( &hmap_s_g->mutex );
    return key;
}

tp_t bcore_name_enroll_s( tp_t name_space, sc_t name )
{
    return bcore_name_enroll_sn( name_space, name, bcore_strlen( name ) );
}

tp_t bcore_name_enroll_n( sc_t name, uz_t n )
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
    bcore_mutex_s_lock( &hmap_s_g->mutex );
    bcore_name_map_s_remove( &hmap_s_g->map, type );
    bcore_mutex_s_unlock( &hmap_s_g->mutex );
}

uz_t  bcore_name_size()
{
    assert( hmap_s_g != NULL );
    bcore_mutex_s_lock( &hmap_s_g->mutex );
    uz_t size = bcore_name_map_s_keys( &hmap_s_g->map );
    bcore_mutex_s_unlock( &hmap_s_g->mutex );
    return size;
}

st_s* bcore_name_show()
{
    assert( hmap_s_g != NULL );
    bcore_mutex_s_lock( &hmap_s_g->mutex );
    st_s* log = st_s_create();
    uz_t size = bcore_name_map_s_size( &hmap_s_g->map );
    for( uz_t i = 0; i < size; i++ )
    {
        bcore_name_s* node = bcore_name_map_s_idx_name( &hmap_s_g->map, i );
        if( node->key )
        {
            bcore_mutex_s_unlock( &hmap_s_g->mutex );
            st_s_push_st_d( log, cat_name_ns( node->name_space, node->name ) );
            bcore_mutex_s_lock( &hmap_s_g->mutex );
            st_s_push_char( log, '\n' );
        }
    }
    bcore_mutex_s_unlock( &hmap_s_g->mutex );
    return log;
}

bcore_name_map_s* bcore_name_create_name_map()
{
    assert( hmap_s_g != NULL );
    bcore_mutex_s_lock( &hmap_s_g->mutex );
    bcore_name_map_s* ret = bcore_name_map_s_clone( &hmap_s_g->map );
    bcore_mutex_s_unlock( &hmap_s_g->mutex );
    return ret;
}

/**********************************************************************************************************************/

st_s* bcore_name_manager_selftest()
{
    tp_t mynamespace_t = entypeof( "mynamespace" );
    tp_t myname_t = entypeof_ns( mynamespace_t, "myname" );
    ASSERT( myname_t == typeof( "mynamespace:myname" ) );
    st_s* cname = nameof_s( myname_t );
    ASSERT( st_s_cmp_sc( cname, "mynamespace:myname" ) == 0 );
    st_s_discard( cname );
    return NULL;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_name_manager_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_name_manager" ) ) )
    {
        case TYPEOF_init0:
        {
            name_manager_open();
        }
        break;

        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_down0:
        {
            // bcore_tbman_instance_disgnostics();

            if( o->object && ( *( bl_t* )o->object ) )
            {
                uz_t space = bcore_tbman_granted_space();
                name_manager_close();
                bcore_msg( "  name mananger ....... % 6zu\n", space - bcore_tbman_granted_space() );
            }
            else
            {
                name_manager_close();
            }
        }
        break;

        case TYPEOF_selftest:
        {
            return bcore_name_manager_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}
