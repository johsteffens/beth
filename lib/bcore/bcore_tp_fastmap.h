/** Copyright 2018 Johannes Bernhard Steffens
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

/** Fast concurrent hash-map of tp_t key and vc_t val.
 *  Key is deemed to already represent a hash (no internal hashing).
 *  Intended for fast lock-free retrieval (e.g. perspective caching).
 *
 *  Get- & Set-functions are concurrent.
 *  Get function is lock-free.
 *
 *  Inspired by Dr. Cliff Click's state analysis for lock-free hash tables: https://web.stanford.edu/class/ee380/Abstracts/070221_LockFreeHash.pdf.
 *  This solution is somewhat different (less sophisticated, function get prabably faster), though,
 *  because we keep the set-function fully locked and optimize on the line of beth-type properties:
 *  - The key is assumed to already represent a hash value.
 *  - The value for a given key never changes.
 *  - Keys are never removed.
 *
 *  Comparing to bcore_hmap:
 *  - Storage efficiency is lower (Can be factor 10 worse for 10000+ keys)
 *  - Get is factor 2 or more faster.
 */

#ifndef BCORE_TP_FASTMAP_H
#define BCORE_TP_FASTMAP_H

#include "bcore_features.h"
#include "bcore_st.h"
#include "bcore_flect.h"
#include "bcore_signal.h"
#include "bcore_threads.h"

#define BCORE_TP_FASTMAP_MAX_TABLES 20
#define BCORE_TP_FASTMAP_SCAN_LENGTH 4
#define BCORE_TP_FASTMAP_START_SIZE  BCORE_TP_FASTMAP_SCAN_LENGTH

typedef struct bcore_tp_fastmap_kv_s
{
    atomic_tp_t key;
    atomic_vc_t val;
} bcore_tp_fastmap_kv_s;

typedef struct bcore_tp_fastmap_table_s
{
    sz_t size;
    tp_t mask;
    bcore_tp_fastmap_kv_s * arr;
} bcore_tp_fastmap_table_s;

static inline
vc_t bcore_tp_fastmap_table_s_get( const bcore_tp_fastmap_table_s* o, tp_t key )
{
    const bcore_tp_fastmap_kv_s* arr = o->arr + ( key & o->mask );
    return
        arr[ 0 ].key == key ? arr[ 0 ].val :
        arr[ 1 ].key == key ? arr[ 1 ].val :
        arr[ 2 ].key == key ? arr[ 2 ].val :
        arr[ 3 ].key == key ? arr[ 3 ].val :
        NULL;
}

typedef struct bcore_tp_fastmap_s
{
    atomic_int table_index_get; // valid table index for get function
    atomic_int table_index_set; // valid table index for set function
    /** A locked cache cannot be filled with new data (set retuns false).
     *  This is needed during perspective shutdown procedure.
     */
    bl_t locked;
    bcore_tp_fastmap_table_s table_arr[ BCORE_TP_FASTMAP_MAX_TABLES ];
    bcore_mutex_s mutex;
}
bcore_tp_fastmap_s;

BCORE_DECLARE_FUNCTIONS_OBJ( bcore_tp_fastmap_s )

void bcore_tp_fastmap_s_clear( bcore_tp_fastmap_s* o );
void bcore_tp_fastmap_s_set_locked( bcore_tp_fastmap_s* o, bl_t flag );

static inline
vc_t bcore_tp_fastmap_s_get( const bcore_tp_fastmap_s* o, tp_t key )
{
    return bcore_tp_fastmap_table_s_get( &o->table_arr[ o->table_index_get ], key );
}

bl_t bcore_tp_fastmap_s_set( bcore_tp_fastmap_s* o, tp_t key, vc_t val );

/// statistics (thread safe)
sz_t bcore_tp_fastmap_s_size( bcore_tp_fastmap_s* o );
sz_t bcore_tp_fastmap_s_keys( bcore_tp_fastmap_s* o );

/**********************************************************************************************************************/

vd_t bcore_tp_fastmap_signal_handler( const bcore_signal_s* o );

#endif // BCORE_TP_FASTMAP_H

