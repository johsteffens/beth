/** Author and Copyright 2021 Johannes Bernhard Steffens
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

#include "bcore_xoila.h"
#include "bcore_const_manager.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

bcore_hmap_tpto_s* bcore_xoila_get_group_stamp_map( void )
{
    bcore_hmap_tpto_s* group_stamp_map = ( vd_t )bcore_const_get_o( TYPEOF_bcore_const_group_stamp_map );
    if( !group_stamp_map )
    {
        group_stamp_map = bcore_hmap_tpto_s_create();
        bcore_const_t_set_d( TYPEOF_bcore_const_group_stamp_map, TYPEOF_bcore_hmap_tpto_s, group_stamp_map );
        bcore_hmap_tpto_s_set_type( group_stamp_map, TYPEOF_bcore_arr_tp_s );
    }
    return group_stamp_map;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_xoila_set_arr_traitline_stamps_d( tp_t group, bcore_arr_tp_s* list )
{
    bcore_hmap_tpto_s_set_d( bcore_xoila_get_group_stamp_map(), group, list );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

