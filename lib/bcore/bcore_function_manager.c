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

#include "bcore_function_manager.h"
#include "bcore_tbman.h"
#include "bcore_st.h"
#include "bcore_threads.h"
#include "bcore_hmap.h"
#include "bcore_signal.h"

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

tp_t bcore_function_set_sc( sc_t name, fp_t f )
{
    tp_t t = entypeof( name );
    bcore_function_set( t, f );
    return t;
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

vd_t bcore_function_manager_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_function_manager" ) ) )
    {
        case TYPEOF_init0:
        {
            function_manager_open();
        }
        break;

        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_down0:
        {
            if( o->object && ( *( bl_t* )o->object ) )
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
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}
