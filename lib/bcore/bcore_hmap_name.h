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

#include "bcore_xoila.h"
#include "bcore_trait.h"
#include "bcore.xo.h"
#include "bcore_hmap_tp_sr.h"
#include "bcore_arr.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bcore_hmap_name, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

signature   tp_t  set_st_c( m @* o, c st_s* st ); // copies string (no effect when already registered); collision test; returns key
signature   tp_t  set_st_d( m @* o, m st_s* st ); // owns string (discards string when already registered); collision test; returns key
signature   tp_t  set_sc(   m @* o, sc_t  sc ); // copies string (no effect when already registered); collision test; returns key
signature m st_s* get(      c @* o, tp_t key ); // returns pointer to string or NULL when key does not exist
signature   sc_t  get_sc(   c @* o, tp_t key ); // returns sc_t or NULL when key does not exist
signature   void  remove(   m @* o, tp_t key ); // removes key if existing
signature   bl_t  exists(   c @* o, tp_t key ); // checks if key exists
signature   void  clear(    m @* o           ); // removes all entries and frees memory

stamp :s = aware bcore_inst
{
    bcore_hmap_tp_sr_s map;
    func : .set_st_c;
    func : .set_st_d;
    func : .set_sc;
    func : .get;
    func : .get_sc;
    func : .remove;
    func : .exists;
    func : .clear;
};

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

vd_t bcore_hmap_name_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BCORE_HMAP_NAME_H
