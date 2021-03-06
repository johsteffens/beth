/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include "bclos_quicktypes.h"
#include "bcore_signal.h"
#include "bclos_frame.h"

void bclos_frame_s_init( bclos_frame_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void bclos_frame_s_down( bclos_frame_s* o )
{
    bcore_inst_t_down( TYPEOF_bclos_frame_s, ( bcore_inst* )o );
}

void bclos_frame_s_copy( bclos_frame_s* o, const bclos_frame_s* src )
{
    bcore_inst_t_copy( TYPEOF_bclos_frame_s, (bcore_inst*)o, src );
}

bclos_frame_s* bclos_frame_s_create()
{
    return bcore_inst_t_create( TYPEOF_bclos_frame_s );
}

bclos_frame_s* bclos_frame_s_clone( const bclos_frame_s* o )
{
    return bcore_inst_t_clone( TYPEOF_bclos_frame_s, (bcore_inst*)o );
}

void bclos_frame_s_discard( bclos_frame_s* o )
{
    bcore_inst_t_discard( TYPEOF_bclos_frame_s, (bcore_inst*)o );
}

sr_s* bclos_frame_s_get( const bclos_frame_s* o, tp_t name )
{
    sr_s* val = bcore_hmap_tp_sr_s_get( &o->map, name );
    if( !val && o->external ) val = bclos_frame_s_get( o->external, name );
    return val;
}

sr_s* bclos_frame_s_set( bclos_frame_s* o, tp_t name, sr_s val )
{
    return bcore_hmap_tp_sr_s_set( &o->map, name, val );
}

sr_s bclos_frame_s_remove( bclos_frame_s* o, tp_t name )
{
    return bcore_hmap_tp_sr_s_remove( &o->map, name );
}

void bclos_frame_s_clear( bclos_frame_s* o )
{
    bcore_hmap_tp_sr_s_clear( &o->map );
}

sr_s* bclos_frame_s_get_local( const bclos_frame_s* o, tp_t name )
{
    return bcore_hmap_tp_sr_s_get( &o->map, name );
}

bcore_self_s* frame_s_create_self( void )
{
    sc_t def =
    "bclos_frame_s = bcore_inst"
    "{ "
        "aware_t _;"
        "private bclos_frame_s* external;"
        "bcore_hmap_tp_sr_s map;"
    "}";

    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( def, bclos_frame_s );
    return self;
}

/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTIONS_IDC_FLAT( bclos_address_s )
BCORE_DEFINE_FUNCTIONS_CDC(      bclos_address_s )
bclos_address_s* bclos_address_s_create_tp( tp_t name )
{
    bclos_address_s* o = bclos_address_s_create();
    o->name = name;
    return o;
}

BCORE_DEFINE_CREATE_SELF( bclos_address_s, "bclos_address_s = { tp_t name; }" )

/**********************************************************************************************************************/

vd_t bclos_frame_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bclos_frame" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "bclos_address_s" ), bclos_address_s_create_self );
            bcore_flect_define_creator( typeof( "bclos_frame_s" ), frame_s_create_self );
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

