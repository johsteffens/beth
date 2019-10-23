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

/** bcore_hmap_name is a tp-hashmap with key = typeof( <string> );
 *  Collision checks.
 */

#ifndef BCORE_HMAP_NAME_H
#define BCORE_HMAP_NAME_H

#include "bcore_plant_compiler.h"
#include "bcore_trait.h"
#include "bcore_planted.h"
#include "bcore_hmap_tp_sr.h"
#include "bcore_arr.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_hmap_name
PLANT_GROUP( bcore_hmap_name, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp : = aware bcore_inst { bcore_hmap_tp_sr_s map; };

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif // TYPEOF_bcore_hmap_name

tp_t  bcore_hmap_name_s_set_st_c(     bcore_hmap_name_s* o, const st_s* st ); // copies string (no effect when already registered); collision test; returns key
tp_t  bcore_hmap_name_s_set_st_d(     bcore_hmap_name_s* o,       st_s* st ); // owns string (discards string when already registered); collision test; returns key
tp_t  bcore_hmap_name_s_set_sc(       bcore_hmap_name_s* o,       sc_t  sc ); // copies string (no effect when already registered); collision test; returns key
st_s* bcore_hmap_name_s_get(    const bcore_hmap_name_s* o, tp_t key ); // returns pointer to string or NULL when key does not exist
sc_t  bcore_hmap_name_s_get_sc( const bcore_hmap_name_s* o, tp_t key ); // returns sc_t or NULL when key does not exist
void  bcore_hmap_name_s_remove(       bcore_hmap_name_s* o, tp_t key ); // removes key if existing
bl_t  bcore_hmap_name_s_exists( const bcore_hmap_name_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_name_s_clear(        bcore_hmap_name_s* o           ); // removes all entries and frees memory

/**********************************************************************************************************************/

vd_t bcore_hmap_name_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BCORE_HMAP_NAME_H
