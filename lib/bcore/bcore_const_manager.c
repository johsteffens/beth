/** Author and Copyright 2018 Johannes Bernhard Steffens
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

static bcore_hmap_tp_sr_s* hmap_sr_g = NULL; // for general objects
static bcore_hmap_tp_sr_s* hmap_st_g = NULL; // for dedicated strings
static bcore_mutex_s*     mutex_st_g = NULL; // for dedicated strings

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void bcore_const_t_set_d( tp_t key, tp_t t, vd_t v )
{
    assert( hmap_sr_g );
    if( bcore_const_exists( key ) ) ERR_fa( "Key '#<sc_t>' (#<tp_t>) already exists.", ifnameof( key ), key );
    bcore_hmap_tp_sr_s_set( hmap_sr_g, key, sr_tsd( t, v ) );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_const_a_set_d( tp_t key, vd_t v )
{
    bcore_const_t_set_d( key, v ? *(aware_t*)v : 0, v );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_const_x_set_d( tp_t key, sr_s v )
{
    assert( hmap_sr_g );
    if( bcore_const_exists( key ) ) ERR_fa( "Key '#<sc_t>' (#<tp_t>) already exists.", ifnameof( key ), key );
    bcore_hmap_tp_sr_s_set( hmap_sr_g, key, v );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_const_t_set_c( tp_t key, tp_t t, vc_t v )
{
    bcore_const_t_set_d( key, t, bcore_inst_t_clone( t, v ) );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_const_a_set_c( tp_t key, vc_t v )
{
    bcore_const_a_set_d( key, bcore_inst_a_clone( v ) );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_const_x_set_c( tp_t key, sr_s v )
{
    bcore_const_x_set_d( key, sr_clone( v ) );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_const_remove( tp_t key )
{
    sr_down( bcore_hmap_tp_sr_s_remove( hmap_sr_g, key ) );
}

//----------------------------------------------------------------------------------------------------------------------

vc_t bcore_const_get_o( tp_t key )
{
    const sr_s* sr = bcore_hmap_tp_sr_s_get( hmap_sr_g, key );
    return ( sr ) ? sr->o : NULL;
}

//----------------------------------------------------------------------------------------------------------------------

sr_s bcore_const_get_x( tp_t key )
{
    const sr_s* sr = bcore_hmap_tp_sr_s_get( hmap_sr_g, key );
    return ( sr ) ? sr_pwc( sr->p, sr->o ) : sr_null();
}

//----------------------------------------------------------------------------------------------------------------------

const sr_s* bcore_const_get_r( tp_t key )
{
    return bcore_hmap_tp_sr_s_get( hmap_sr_g, key );
}

//----------------------------------------------------------------------------------------------------------------------

vc_t bcore_const_vget_o( tp_t key, tp_t type )
{
    return bcore_const_vget_r( key, type )->o;
}

//----------------------------------------------------------------------------------------------------------------------

sr_s bcore_const_vget_x( tp_t key, tp_t type )
{
    const sr_s* sr = bcore_const_vget_r( key, type );
    return sr_pwc( sr->p, sr->o );
}

//----------------------------------------------------------------------------------------------------------------------

const sr_s* bcore_const_vget_r( tp_t key, tp_t type )
{
    const sr_s* sr = bcore_const_get_r( key );
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

bl_t bcore_const_exists( tp_t key )
{
    return bcore_hmap_tp_sr_s_exists( hmap_sr_g, key );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Dedicated Strings

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_const_string_set_st_d( st_s* string )
{
    tp_t key = typeof( string->sc );
    bcore_mutex_s_lock( mutex_st_g );
    if( bcore_hmap_tp_sr_s_exists( hmap_st_g, key ) )
    {
        st_s* st = bcore_hmap_tp_sr_s_get( hmap_st_g, key )->o;
        if( !st_s_equal_st( st, string ) )
        {
            bcore_mutex_s_unlock( mutex_st_g );
            ERR_fa( "Collision: Strings '#<sc_t>' and '#<sc_t>' produce the same key '#<tp_t>'.", string, st->sc, key );
        }
        st_s_discard( string );
    }
    else
    {
        bcore_hmap_tp_sr_s_set( hmap_st_g, key, sr_asd( string ) );
    }
    bcore_mutex_s_unlock( mutex_st_g );
    return key;
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_const_string_set_st_c( const st_s* string )
{
    return bcore_const_string_set_st_d( st_s_clone( string ) );
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_const_string_set_sc( sc_t string )
{
    return bcore_const_string_set_st_d( st_s_create_sc( string ) );
}

//----------------------------------------------------------------------------------------------------------------------

sc_t bcore_const_string_get_sc( tp_t key )
{
    bcore_mutex_s_lock( mutex_st_g );
    sr_s* sr = bcore_hmap_tp_sr_s_get( hmap_st_g, key );
    sc_t  sc = ( sr ) ? ( ( st_s* )sr->o )->sc : NULL;
    bcore_mutex_s_unlock( mutex_st_g );
    return sc;
}

//----------------------------------------------------------------------------------------------------------------------

const st_s* bcore_const_string_get_st( tp_t key )
{
    bcore_mutex_s_lock( mutex_st_g );
    sr_s* sr = bcore_hmap_tp_sr_s_get( hmap_st_g, key );
    st_s* st = ( sr ) ? sr->o : NULL;
    bcore_mutex_s_unlock( mutex_st_g );
    return st;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_const_string_exists( tp_t key )
{
    bcore_mutex_s_lock( mutex_st_g );
    bl_t bl = bcore_hmap_tp_sr_s_exists( hmap_st_g, key );
    bcore_mutex_s_unlock( mutex_st_g );
    return bl;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

sz_t bcore_const_size()
{
    return bcore_hmap_tp_sr_s_keys( hmap_sr_g ) + bcore_hmap_tp_sr_s_keys( hmap_st_g );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// signal

vd_t bcore_const_manager_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_const_manager" ) ) )
    {
        case TYPEOF_init0:
        {
            hmap_sr_g = bcore_hmap_tp_sr_s_create();
            hmap_st_g = bcore_hmap_tp_sr_s_create();
            mutex_st_g = bcore_mutex_s_create();

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
                sz_t count = bcore_const_size();
                sz_t space1 = bcore_tbman_total_granted_space();
                bcore_hmap_tp_sr_s_discard( hmap_sr_g );
                bcore_hmap_tp_sr_s_discard( hmap_st_g );
                bcore_mutex_s_discard( mutex_st_g );
                sz_t space2 = bcore_tbman_total_granted_space();
                sz_t space = space1 - space2;
                bcore_msg_fa( "  const manager ....... #pl7 {#<sz_t>} (by #pl4 {#<sz_t>} constants    )\n", space, count );
                if( space < 0 )
                {
                    bcore_msg_fa( "    A negative memory balance in the const-manager\n" );
                    bcore_msg_fa( "    can occur when previously unused perspectives\n" );
                    bcore_msg_fa( "    are invoked to shut down objects.\n" );
                    bcore_msg_fa( "    This is no error.\n" );
                }
            }
            else
            {
                bcore_hmap_tp_sr_s_discard( hmap_sr_g );
                bcore_hmap_tp_sr_s_discard( hmap_st_g );
                bcore_mutex_s_discard( mutex_st_g );
            }
            hmap_sr_g = NULL;
            hmap_st_g = NULL;
            mutex_st_g = NULL;
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
