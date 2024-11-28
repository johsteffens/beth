/** Author and Copyright 2023 Johannes Bernhard Steffens
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

#include "bcore_global_manager.h"
#include "bcore_tbman.h"
#include "bcore_hmap_tp_sr.h"

static bcore_hmap_tp_sr_s* hmap_g = NULL;
static bcore_mutex_s*      mutex_g = NULL;

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void bcore_global_t_set_d( tp_t key, tp_t t, vd_t v )
{
    assert( hmap_g );
    bcore_mutex_s_lock( mutex_g );
    bcore_hmap_tp_sr_s_set( hmap_g, key, sr_tsm( t, v ) );
    bcore_mutex_s_unlock( mutex_g );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_global_a_set_d( tp_t key, vd_t v )
{
    bcore_global_t_set_d( key, v ? *(aware_t*)v : 0, v );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_global_x_set_d( tp_t key, sr_s v )
{
    assert( hmap_g );
    bcore_mutex_s_lock( mutex_g );
    bcore_hmap_tp_sr_s_set( hmap_g, key, v );
    bcore_mutex_s_unlock( mutex_g );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_global_t_set_c( tp_t key, tp_t t, vc_t v )
{
    bcore_global_t_set_d( key, t, bcore_inst_t_clone( t, v ) );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_global_a_set_c( tp_t key, vc_t v )
{
    bcore_global_a_set_d( key, bcore_inst_a_clone( v ) );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_global_x_set_c( tp_t key, sr_s v )
{
    bcore_global_x_set_d( key, sr_clone( v ) );
}

//----------------------------------------------------------------------------------------------------------------------
vd_t bcore_global_get_ifnexists_set( tp_t key, tp_t t )
{
    assert( hmap_g );
    bcore_mutex_s_lock( mutex_g );
    if( !bcore_hmap_tp_sr_s_exists( hmap_g, key ) )
    {
        bcore_hmap_tp_sr_s_set( hmap_g, key, sr_create( t ) );
    }
    const sr_s* sr = bcore_hmap_tp_sr_s_get( hmap_g, key );
    bcore_mutex_s_unlock( mutex_g );
    return ( sr ) ? sr->o : NULL;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_global_remove( tp_t key )
{
    bcore_mutex_s_lock( mutex_g );
    sr_down( bcore_hmap_tp_sr_s_remove( hmap_g, key ) );
    bcore_mutex_s_unlock( mutex_g );
}

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_global_get_o( tp_t key )
{
    bcore_mutex_s_lock( mutex_g );
    const sr_s* sr = bcore_hmap_tp_sr_s_get( hmap_g, key );
    bcore_mutex_s_unlock( mutex_g );
    return ( sr ) ? sr->o : NULL;
}

//----------------------------------------------------------------------------------------------------------------------

sr_s bcore_global_get_x( tp_t key )
{
    bcore_mutex_s_lock( mutex_g );
    const sr_s* sr = bcore_hmap_tp_sr_s_get( hmap_g, key );
    bcore_mutex_s_unlock( mutex_g );
    return ( sr ) ? sr_pwc( sr->p, sr->o ) : sr_null();
}

//----------------------------------------------------------------------------------------------------------------------

sr_s* bcore_global_get_r( tp_t key )
{
    bcore_mutex_s_lock( mutex_g );
    sr_s* sr = bcore_hmap_tp_sr_s_get( hmap_g, key );
    bcore_mutex_s_unlock( mutex_g );
    return sr;
}

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_global_vget_o( tp_t key, tp_t type )
{
    return bcore_global_vget_r( key, type )->o;
}

//----------------------------------------------------------------------------------------------------------------------

sr_s bcore_global_vget_x( tp_t key, tp_t type )
{
    const sr_s* sr = bcore_global_vget_r( key, type );
    return sr_pwc( sr->p, sr->o );
}

//----------------------------------------------------------------------------------------------------------------------

sr_s* bcore_global_vget_r( tp_t key, tp_t type )
{
    sr_s* sr = bcore_global_get_r( key );
    if( !sr ) ERR_fa( "Key '#<sc_t>' (#<tp_t>) not registered.", ifnameof( key ), key );
    if( type != 0 )
    {
        if( sr_s_type( sr ) != type )
        {
            ERR_fa( "Key '#<sc_t>': Type mismatch: '#<sc_t>' requested but '#<sc_t>' registered.",
                    ifnameof( key ), ifnameof( type ), ifnameof( sr_s_type( sr ) ) );
        }
    }
    return sr;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_global_exists( tp_t key )
{
    bcore_mutex_s_lock( mutex_g );
    bl_t bl = bcore_hmap_tp_sr_s_exists( hmap_g, key );
    bcore_mutex_s_unlock( mutex_g );
    return bl;
}

//----------------------------------------------------------------------------------------------------------------------

sz_t bcore_global_size()
{
    bcore_mutex_s_lock( mutex_g );
    sz_t sz = bcore_hmap_tp_sr_s_keys( hmap_g );
    bcore_mutex_s_unlock( mutex_g );
    return sz;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// signal

vd_t bcore_global_manager_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_global_manager" ) ) )
    {
        case TYPEOF_init0:
        {
            hmap_g = bcore_hmap_tp_sr_s_create();
            mutex_g = bcore_mutex_s_create();

        }
        break;

        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_down1:
        {
        }
        break;

        case TYPEOF_down0:
        {
            s2_t verbosity = o->object ? *( s2_t* )o->object : 0;
            if( verbosity > 0 )
            {
                sz_t count = bcore_global_size();
                sz_t space1 = bcore_tbman_total_granted_space();
                bcore_hmap_tp_sr_s_discard( hmap_g );
                bcore_mutex_s_discard( mutex_g );
                sz_t space2 = bcore_tbman_total_granted_space();
                sz_t space = space1 - space2;
                bcore_msg_fa( " global manager ....... #pl7 {#<sz_t>} (by #pl4 {#<sz_t>} objects      )\n", space, count );
            }
            else
            {
                bcore_hmap_tp_sr_s_discard( hmap_g );
                bcore_mutex_s_discard( mutex_g );
            }
            hmap_g = NULL;
            mutex_g = NULL;
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
