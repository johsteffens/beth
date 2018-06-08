/** Copyright 2018 Johannes Bernhard Steffens
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

#include "bcore_const_manager.h"
#include "bcore_tbman.h"
#include "bcore_hmap_tp_sr.h"

static bcore_hmap_tp_sr_s* hmap_g = NULL;

/**********************************************************************************************************************/

//---------------------------------------------------------------------------------------------------------------------

void bcore_const_t_set_d( tp_t key, tp_t t, vd_t v )
{
    ASSERT( hmap_g );
    if( bcore_const_exists( key ) ) ERR_fa( "Key '#<sc_t>' (#<tp_t>) already exists.", ifnameof( key ), key );
    bcore_hmap_tp_sr_s_set( hmap_g, key, sr_tsd( t, v ) );
}

//---------------------------------------------------------------------------------------------------------------------

void bcore_const_a_set_d( tp_t key, vd_t v )
{
    bcore_const_t_set_d( key, v ? *(aware_t*)v : 0, v );
}

//---------------------------------------------------------------------------------------------------------------------

void bcore_const_x_set_d( tp_t key, sr_s v )
{
    ASSERT( hmap_g );
    if( bcore_const_exists( key ) ) ERR_fa( "Key '#<sc_t>' (#<tp_t>) already exists.", ifnameof( key ), key );
    bcore_hmap_tp_sr_s_set( hmap_g, key, v );
}

//---------------------------------------------------------------------------------------------------------------------

void bcore_const_t_set_c( tp_t key, tp_t t, vc_t v )
{
    bcore_const_t_set_d( key, t, bcore_inst_t_clone( t, v ) );
}

//---------------------------------------------------------------------------------------------------------------------

void bcore_const_a_set_c( tp_t key, vc_t v )
{
    bcore_const_a_set_d( key, bcore_inst_a_clone( v ) );
}

//---------------------------------------------------------------------------------------------------------------------

void bcore_const_x_set_c( tp_t key, sr_s v )
{
    bcore_const_x_set_d( key, sr_clone( v ) );
}

//---------------------------------------------------------------------------------------------------------------------

void bcore_const_remove( tp_t key )
{
    sr_down( bcore_hmap_tp_sr_s_remove( hmap_g, key ) );
}

//---------------------------------------------------------------------------------------------------------------------

vc_t bcore_const_get_o( tp_t key )
{
    const sr_s* sr = bcore_hmap_tp_sr_s_get( hmap_g, key );
    return ( sr ) ? sr->o : NULL;
}

//---------------------------------------------------------------------------------------------------------------------

sr_s bcore_const_get_x( tp_t key )
{
    const sr_s* sr = bcore_hmap_tp_sr_s_get( hmap_g, key );
    return ( sr ) ? sr_pwc( sr->p, sr->o ) : sr_null();
}

//---------------------------------------------------------------------------------------------------------------------

const sr_s* bcore_const_get_r( tp_t key )
{
    return bcore_hmap_tp_sr_s_get( hmap_g, key );
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bcore_const_exists( tp_t key )
{
    return bcore_hmap_tp_sr_s_exists( hmap_g, key );
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// signal

vd_t bcore_const_manager_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_const_manager" ) ) )
    {
        case TYPEOF_init0:
        {
            hmap_g = bcore_hmap_tp_sr_s_create();
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
            if( o->object && ( *( bl_t* )o->object ) )
            {
                sz_t space = bcore_tbman_granted_space();
                bcore_hmap_tp_sr_s_discard( hmap_g );
                bcore_msg( "  const manager ....... % 6zu\n", space - bcore_tbman_granted_space() );
            }
            else
            {
                bcore_hmap_tp_sr_s_discard( hmap_g );
            }
            hmap_g = NULL;
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
