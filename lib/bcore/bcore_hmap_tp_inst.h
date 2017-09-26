/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_HMAP_TP_INST_H
#define BCORE_HMAP_TP_INST_H

#include "bcore_features.h"
#include "bcore_string.h"
#include "bcore_flect.h"
#include "bcore_spect_inst.h"

/** Hashmap using tp_t as key and bcore_inst_po as value.
 *  All objects are owned by hashmap.
 *  (See bcore_hmap.h for algorithmic details)
 *  This version uses fixed internal hash functions and supports most generic perspectives.
 */

/**********************************************************************************************************************/

typedef struct bcore_hnode_tp_inst_s
{
    tp_t key;
    bl_t flag_trace; // used internally during rehashing
    bcore_inst_op val;
} bcore_hnode_tp_inst_s;

DECLARE_FUNCTION_INIT(    bcore_hnode_tp_inst_s )
DECLARE_FUNCTION_DOWN(    bcore_hnode_tp_inst_s )
DECLARE_FUNCTION_COPY(    bcore_hnode_tp_inst_s )
DECLARE_FUNCTION_CREATE(  bcore_hnode_tp_inst_s )
DECLARE_FUNCTION_DISCARD( bcore_hnode_tp_inst_s )
DECLARE_FUNCTION_CLONE(   bcore_hnode_tp_inst_s )

typedef struct bcore_hmap_tp_inst_s
{
    aware_t _;
    union
    {
        bcore_static_array_s arr;
        struct
        {
            bcore_hnode_tp_inst_s* data;
            sz_t size, space;
        };
    };
    sz_t depth_limit;
    sz_t size_limit;
} bcore_hmap_tp_inst_s;

DECLARE_FUNCTION_INIT(    bcore_hmap_tp_inst_s )
DECLARE_FUNCTION_DOWN(    bcore_hmap_tp_inst_s )
DECLARE_FUNCTION_COPY(    bcore_hmap_tp_inst_s )
DECLARE_FUNCTION_CREATE(  bcore_hmap_tp_inst_s )
DECLARE_FUNCTION_DISCARD( bcore_hmap_tp_inst_s )
DECLARE_FUNCTION_CLONE(   bcore_hmap_tp_inst_s )

vd_t                 bcore_hmap_tp_inst_s_get(    const bcore_hmap_tp_inst_s* o, tp_t key ); // returns pinter to object or NULL when key does not exist
const bcore_inst_op* bcore_hmap_tp_inst_s_get_op( const bcore_hmap_tp_inst_s* o, tp_t key ); // returns pinter to bcore_inst_op or NULL when key does not exist
const bcore_inst_op* bcore_hmap_tp_inst_s_set_type(     bcore_hmap_tp_inst_s* o, tp_t key, tp_t type );
const bcore_inst_op* bcore_hmap_tp_inst_s_set_type_d(   bcore_hmap_tp_inst_s* o, tp_t key, tp_t type, vd_t obj ); // takes object
const bcore_inst_op* bcore_hmap_tp_inst_s_set_type_c(   bcore_hmap_tp_inst_s* o, tp_t key, tp_t type, vc_t obj ); // copies object
const bcore_inst_op* bcore_hmap_tp_inst_s_set_aware_d(  bcore_hmap_tp_inst_s* o, tp_t key, vd_t obj ); // takes object
const bcore_inst_op* bcore_hmap_tp_inst_s_set_aware_c(  bcore_hmap_tp_inst_s* o, tp_t key, vc_t obj ); // copies object
const bcore_inst_op* bcore_hmap_tp_inst_s_set_d(        bcore_hmap_tp_inst_s* o, tp_t key, bcore_inst_op op ); // takes state of bcore_inst_op
bcore_inst_op        bcore_hmap_tp_inst_s_remove_h(     bcore_hmap_tp_inst_s* o, tp_t key ); // removes key, returns bcore_inst_op holding object. Returns empty bcore_inst_op if key did not exist.
void                 bcore_hmap_tp_inst_s_remove_d(     bcore_hmap_tp_inst_s* o, tp_t key ); // removes key and discards object if held. No effect if key did not exist.

bool  bcore_hmap_tp_inst_s_exists( const bcore_hmap_tp_inst_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_tp_inst_s_clear(        bcore_hmap_tp_inst_s* o           ); // removes all entries and frees memory
sz_t  bcore_hmap_tp_inst_s_keys(   const bcore_hmap_tp_inst_s* o           ); // returns number of registered keys
sz_t  bcore_hmap_tp_inst_s_size(   const bcore_hmap_tp_inst_s* o           ); // returns current size of the hash map (note that this includes empty places)

tp_t                 bcore_hmap_tp_inst_s_idx_key( const bcore_hmap_tp_inst_s* o, sz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
const bcore_inst_op* bcore_hmap_tp_inst_s_idx_op(  const bcore_hmap_tp_inst_s* o, sz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

/**********************************************************************************************************************/

vd_t bcore_hmap_tp_inst_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_HMAP_TP_INST_H

