/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_HMAP_TP_SR_H
#define BCORE_HMAP_TP_SR_H

#include "bcore_features.h"
#include "bcore_st.h"
#include "bcore_flect.h"
#include "bcore_spect_inst.h"

/** Hashmap using tp_t as key and sr_s as value.
 *  (See bcore_hmap.h for algorithmic details)
 *  This version uses fixed internal hash functions and supports generic perspectives.
 */

/**********************************************************************************************************************/

typedef struct bcore_hnode_tp_sr_s
{
    tp_t key;
    sr_s val;
} bcore_hnode_tp_sr_s;

DECLARE_FUNCTION_INIT(    bcore_hnode_tp_sr_s )
DECLARE_FUNCTION_DOWN(    bcore_hnode_tp_sr_s )
DECLARE_FUNCTION_COPY(    bcore_hnode_tp_sr_s )
DECLARE_FUNCTION_CREATE(  bcore_hnode_tp_sr_s )
DECLARE_FUNCTION_DISCARD( bcore_hnode_tp_sr_s )
DECLARE_FUNCTION_CLONE(   bcore_hnode_tp_sr_s )

typedef struct bcore_hmap_tp_sr_s
{
    aware_t _;
    bcore_hnode_tp_sr_s* nodes;
    bl_t* flags;
    sz_t size;
    sz_t depth_limit;
    sz_t size_limit;
} bcore_hmap_tp_sr_s;

DECLARE_FUNCTION_INIT(    bcore_hmap_tp_sr_s )
DECLARE_FUNCTION_DOWN(    bcore_hmap_tp_sr_s )
DECLARE_FUNCTION_COPY(    bcore_hmap_tp_sr_s )
DECLARE_FUNCTION_CREATE(  bcore_hmap_tp_sr_s )
DECLARE_FUNCTION_DISCARD( bcore_hmap_tp_sr_s )
DECLARE_FUNCTION_CLONE(   bcore_hmap_tp_sr_s )

sr_s* bcore_hmap_tp_sr_s_get(    const bcore_hmap_tp_sr_s* o, tp_t key ); // returns pinter to value or sr_null when key does not exist
sr_s* bcore_hmap_tp_sr_s_set(          bcore_hmap_tp_sr_s* o, tp_t key, sr_s val );
sr_s  bcore_hmap_tp_sr_s_remove(       bcore_hmap_tp_sr_s* o, tp_t key ); // removes key, returns value; call sr_down( remove(...) ) to eliminate entry
bl_t  bcore_hmap_tp_sr_s_exists( const bcore_hmap_tp_sr_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_tp_sr_s_clear(        bcore_hmap_tp_sr_s* o           ); // removes all entries and frees memory
sz_t  bcore_hmap_tp_sr_s_keys(   const bcore_hmap_tp_sr_s* o           ); // returns number of registered keys
sz_t  bcore_hmap_tp_sr_s_size(   const bcore_hmap_tp_sr_s* o           ); // returns current size of the hash map (note that this includes empty places)

tp_t        bcore_hmap_tp_sr_s_idx_key( const bcore_hmap_tp_sr_s* o, sz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
const sr_s* bcore_hmap_tp_sr_s_idx_val( const bcore_hmap_tp_sr_s* o, sz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

/**********************************************************************************************************************/

vd_t bcore_hmap_tp_sr_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_HMAP_TP_SR_H

