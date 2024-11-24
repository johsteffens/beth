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

#ifndef BCORE_HMAP_TP_SR_H
#define BCORE_HMAP_TP_SR_H

#include "bcore_feature.h"
#include "bcore_st.h"
#include "bcore_flect.h"
#include "bcore_spect_inst.h"

/** Hashmap using tp_t as key and sr_s as value.
 *  (cuckoo hashing: See bcore_hmap.h for algorithmic details)
 *  This version uses fixed internal hash functions and supports generic perspectives.
 *
 *  Data persistence during rehashing:
 *  The memory location of the referenced object does not change;
 *  the location of the sr_s-instance does change;
 */

/**********************************************************************************************************************/

typedef struct bcore_hnode_tp_sr_s
{
    tp_t key;
    sr_s val;
} bcore_hnode_tp_sr_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_hnode_tp_sr_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_hnode_tp_sr_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_hnode_tp_sr_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_hnode_tp_sr_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_hnode_tp_sr_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_hnode_tp_sr_s )

typedef struct bcore_hmap_tp_sr_s
{
    aware_t _;
    bcore_hnode_tp_sr_s* nodes;
    bl_t* flags;
    uz_t size;
    uz_t depth_limit;
} bcore_hmap_tp_sr_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_hmap_tp_sr_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_hmap_tp_sr_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_hmap_tp_sr_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_hmap_tp_sr_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_hmap_tp_sr_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_hmap_tp_sr_s )

sr_s* bcore_hmap_tp_sr_s_get(    const bcore_hmap_tp_sr_s* o, tp_t key ); // returns pointer to value or sr_null when key does not exist
sr_s* bcore_hmap_tp_sr_s_set(          bcore_hmap_tp_sr_s* o, tp_t key, sr_s val ); // assumes ownership (fork if necessary); no deep copy
sr_s  bcore_hmap_tp_sr_s_remove(       bcore_hmap_tp_sr_s* o, tp_t key ); // removes key, returns value; call sr_down( remove(...) ) to eliminate entry
bl_t  bcore_hmap_tp_sr_s_exists( const bcore_hmap_tp_sr_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_tp_sr_s_clear(        bcore_hmap_tp_sr_s* o           ); // removes all entries and frees memory
uz_t  bcore_hmap_tp_sr_s_keys(   const bcore_hmap_tp_sr_s* o           ); // returns number of registered keys
uz_t  bcore_hmap_tp_sr_s_size(   const bcore_hmap_tp_sr_s* o           ); // returns current size of the hash map (note that this includes empty places)

tp_t        bcore_hmap_tp_sr_s_idx_key( const bcore_hmap_tp_sr_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
const sr_s* bcore_hmap_tp_sr_s_idx_val( const bcore_hmap_tp_sr_s* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

const bcore_inst* bcore_hmap_tp_sr_s_get_c_inst( const bcore_hmap_tp_sr_s* o, tp_t key ); // returns associated instance (NULL when key does not exists)
      bcore_inst* bcore_hmap_tp_sr_s_get_m_inst(       bcore_hmap_tp_sr_s* o, tp_t key ); // returns associated instance (NULL when key does not exists)
tp_t              bcore_hmap_tp_sr_s_get_type(   const bcore_hmap_tp_sr_s* o, tp_t key ); // returns associated instance-type (0 when key does not exists)
sr_s* bcore_hmap_tp_sr_s_set_t_inst_c( bcore_hmap_tp_sr_s* o, tp_t key, tp_t type, const bcore_inst* inst ); // sets new value by copying typed inst
sr_s* bcore_hmap_tp_sr_s_set_t_inst_d( bcore_hmap_tp_sr_s* o, tp_t key, tp_t type,       bcore_inst* inst ); // sets new value by referencing discardable typed inst
sr_s* bcore_hmap_tp_sr_s_set_inst_c  ( bcore_hmap_tp_sr_s* o, tp_t key,            const bcore_inst* inst ); // sets new value by copying aware inst
sr_s* bcore_hmap_tp_sr_s_set_inst_d  ( bcore_hmap_tp_sr_s* o, tp_t key,                  bcore_inst* inst ); // sets new value by referencing discardable aware inst

/// bcore_hmap_tp_sr_s_sc_-functions below use a sc_t-key via bentypeof, bnameof (all key names get enrolled)
sr_s* bcore_hmap_tp_sr_s_sc_get(    const bcore_hmap_tp_sr_s* o, sc_t key ); // returns pointer to value or sr_null when key does not exist
sr_s* bcore_hmap_tp_sr_s_sc_set(          bcore_hmap_tp_sr_s* o, sc_t key, sr_s val ); // assumes ownership (fork if necessary); no deep copy
sr_s  bcore_hmap_tp_sr_s_sc_remove(       bcore_hmap_tp_sr_s* o, sc_t key ); // removes key, returns value; call sr_down( remove(...) ) to eliminate entry
bl_t  bcore_hmap_tp_sr_s_sc_exists( const bcore_hmap_tp_sr_s* o, sc_t key ); // checks if key exists
sc_t  bcore_hmap_tp_sr_s_sc_idx_key( const bcore_hmap_tp_sr_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
const bcore_inst* bcore_hmap_tp_sr_s_sc_get_c_inst( const bcore_hmap_tp_sr_s* o, sc_t key ); // returns associated instance (NULL when key does not exists)
      bcore_inst* bcore_hmap_tp_sr_s_sc_get_m_inst(       bcore_hmap_tp_sr_s* o, sc_t key ); // returns associated instance (NULL when key does not exists)
tp_t              bcore_hmap_tp_sr_s_sc_get_type(   const bcore_hmap_tp_sr_s* o, sc_t key ); // returns associated instance-type (0 when key does not exists)
sr_s* bcore_hmap_tp_sr_s_sc_set_t_inst_c( bcore_hmap_tp_sr_s* o, sc_t key, tp_t type, const bcore_inst* inst ); // sets new value by copying typed inst
sr_s* bcore_hmap_tp_sr_s_sc_set_t_inst_d( bcore_hmap_tp_sr_s* o, sc_t key, tp_t type,       bcore_inst* inst ); // sets new value by referencing discardable typed inst
sr_s* bcore_hmap_tp_sr_s_sc_set_inst_c  ( bcore_hmap_tp_sr_s* o, sc_t key,            const bcore_inst* inst ); // sets new value by copying aware inst
sr_s* bcore_hmap_tp_sr_s_sc_set_inst_d  ( bcore_hmap_tp_sr_s* o, sc_t key,                  bcore_inst* inst ); // sets new value by referencing discardable aware inst

/**********************************************************************************************************************/

vd_t bcore_hmap_tp_sr_signal_handler( const bcore_signal_s* o );

#endif // BCORE_HMAP_TP_SR_H

