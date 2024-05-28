/** Author and Copyright 2019 Johannes Bernhard Steffens
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

#include "bcore_hmap_name.h"
#include "bcore_std.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_hmap_name_s_set_st_d( bcore_hmap_name_s* o, st_s* st )
{
    tp_t key = typeof( st->sc );
    sr_s* sr = NULL;
    if( ( sr = bcore_hmap_tp_sr_s_get( &o->map, key ) ) != NULL )
    {
        st_s* st_l = sr->o;
        if( !st_s_equal_st( st_l, st ) )
        {
            ERR_fa( "Name collision: '#<sc_t>' and '#<sc_t>' produce the same hash value '#<tp_t>'.", st_l->sc, st->sc, key );
        }
        st_s_discard( st );
    }
    else
    {
        bcore_hmap_tp_sr_s_set( &o->map, key, sr_tsm( TYPEOF_st_s, st ) );
    }
    return key;
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_hmap_name_s_set_st_c( bcore_hmap_name_s* o, const st_s* st )
{
    return bcore_hmap_name_s_set_st_d( o, st_s_clone( st ) );
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_hmap_name_s_set_sc( bcore_hmap_name_s* o, sc_t sc )
{
    return bcore_hmap_name_s_set_st_d( o, st_s_create_sc( sc ) );
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_hmap_name_s_get( const bcore_hmap_name_s* o, tp_t key )
{
    sr_s* sr = bcore_hmap_tp_sr_s_get( &o->map, key );
    return sr ? sr->o : NULL;
}

//----------------------------------------------------------------------------------------------------------------------

sc_t  bcore_hmap_name_s_get_sc( const bcore_hmap_name_s* o, tp_t key )
{
    sr_s* sr = bcore_hmap_tp_sr_s_get( &o->map, key );
    return sr ? ( ( st_s* )( sr->o ) )->sc : NULL;
}

void bcore_hmap_name_s_remove( bcore_hmap_name_s* o, tp_t key )
{
    sr_down( bcore_hmap_tp_sr_s_remove( &o->map, key ) );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t  bcore_hmap_name_s_exists( const bcore_hmap_name_s* o, tp_t key )
{
    return bcore_hmap_tp_sr_s_exists( &o->map, key );
}

//----------------------------------------------------------------------------------------------------------------------

void  bcore_hmap_name_s_clear( bcore_hmap_name_s* o )
{
    bcore_hmap_tp_sr_s_clear( &o->map );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bcore_hmap_name_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_hmap_name" ) ) )
    {
        case TYPEOF_init1:
        {
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

/**********************************************************************************************************************/

