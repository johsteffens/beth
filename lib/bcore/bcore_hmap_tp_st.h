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

/** bcore_hmap_tp_st is a tp-hashmap with key = typeof( <string> );
 *  Collision checks.
 */

#ifndef BCORE_HMAP_TP_ST_H
#define BCORE_HMAP_TP_ST_H

#include "bcore_xoila.h"
#include "bcore_trait.h"
#include "bcore.xo.h"
#include "bcore_hmap.h"
#include "bcore_x_inst.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bcore_hmap_tp_st, x_inst )
#ifdef XOILA_SECTION

signature c st_s* get    ( c@* o, tp_t key ); // returns pointer to value or NULL when key does not exist
signature m st_s* set    ( m@* o, tp_t key, c st_s* val ); // sets (overwrites) key-value
signature m st_s* set_d  ( m@* o, tp_t key, d st_s* val ); // sets (overwrites) key-value
signature m st_s* set_sc ( m@* o, tp_t key, sc_t val ); // sets (overwrites) key-value
signature void    remove ( m@* o, tp_t key ); // removes key
signature bl_t    exists ( c@* o, tp_t key ); // checks if key exists
signature void    clear  ( m@* o ); // removes all entries and frees memory
signature sz_t    keys   ( c@* o ); // returns number of registered keys
signature sz_t    size   ( c@* o ); // returns current size of the hash map (note that this includes empty places)
signature tp_t    idx_key( c@* o, sz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
signature c st_s* idx_val( c@* o, sz_t idx ); // returns indexed value (idx indexes the entire table including empty places)


stamp :s = aware bcore_inst
{
    bcore_hmap_tpaw_s map;
    func :.get     { st_s** p_st = o.map.get( key ).cast( st_s** ); return p_st ? p_st.1 : NULL; };
    func :.set_d   { return o.map.set_d( key, val ).cast( m st_s* ); };
    func :.set     { return o.set_d( key, val.clone() ); };
    func :.set_sc  { return o.set_d( key, st_s_create_sc( val ) ); };
    func :.remove  { o.map.remove( key ); };
    func :.exists  { return o.map.exists( key ); };
    func :.clear   { o.map.clear(); };
    func :.keys    { return o.map.keys(); };
    func :.size    { return o.map.size(); };
    func :.idx_key { return o.map.idx_key( idx ); };
    func :.idx_val { return o.map.idx_val( idx ).cast( st_s* ); };
};

func void selftest();

#endif

/**********************************************************************************************************************/

#endif  // BCORE_HMAP_TP_ST_H
