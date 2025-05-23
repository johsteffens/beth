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

#ifndef BCORE_HMAP_H
#define BCORE_HMAP_H

#include "bcore_feature.h"
#include "bcore_st.h"
#include "bcore_flect.h"

/** Hashmap implementing a flavor of "cuckoo hashing" with single monolithic storage and
 *  up to three hash functions (default: 2 functions).
 *  Memory requirements (2 hash function) is comparable to btree.
 *
 *  Features:
 *    * Supports instance perspective.
 *    * Can manage data and function pointers
 *    * Can hold (==own) referenced data objects
 *    * Storage efficiency around 50% with 2 hash functions and around 90% with 3 hash functions.
 *      (Found by our experiments)
 *
 *  Advantages:
 *     Complexity   : O(1) for insert, remove, retrieve.
 *     Average speed: When using 2 hash functions we observe typically one order of magnitude better than btree.
 *                    Note: Using 3 hash functions can be significantly slower on large hash maps.
 *                          Processing time scales exponentially with number of hash functions.
 *
 *  Disadvantage:
 *     No well defined lower bound on worst case storage efficiency.
 *
 *     Note: In practice a significant deviation from average efficiency is very unlikely.
 *           See public studies on cuckoo hashing for more details.
 */

/**********************************************************************************************************************/
/// forward declarations
BCORE_FORWARD_OBJECT( bcore_array );

/**********************************************************************************************************************/

/** bcore_hmap_u3vd_s:
 *    key-type: u3_t
 *    val-type: vd_t or fp_t.
 *    Map holds aware objects if so desired.
 *    Map can reference any object but hold only aware objects.
 */
typedef struct bcore_hnode_u3vd_s bcore_hnode_u3vd_s;
typedef u3_t (*bcore_hash_u3u3)( u3_t v );

typedef struct bcore_hmap_u3vd_s
{
    aware_t _;
    union
    {
        bcore_array_dyn_solid_static_s arr;
        struct
        {
            bcore_hnode_u3vd_s* data;
            uz_t size;
            uz_t space;
        };
    };
    uz_t depth_limit;
    bcore_hash_u3u3 h1, h2, h3;
} bcore_hmap_u3vd_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_hmap_u3vd_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_hmap_u3vd_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_hmap_u3vd_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_hmap_u3vd_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_hmap_u3vd_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_hmap_u3vd_s )

void  bcore_hmap_u3vd_s_set_hash_function( bcore_hmap_u3vd_s* o, uz_t index, bcore_hash_u3u3 hf ); // optionally sets external hash function (up to three functions can be specified via index 0, 1, 2)
vd_t* bcore_hmap_u3vd_s_get(       const bcore_hmap_u3vd_s* o, u3_t key ); // returns pointer to value or NULL when key does not exist
void  bcore_hmap_u3vd_s_set(             bcore_hmap_u3vd_s* o, u3_t key, vd_t val, bool hold ); // hold==true only for aware objects
fp_t* bcore_hmap_u3vd_s_getf(      const bcore_hmap_u3vd_s* o, u3_t key ); // returns pointer to function pointer; returns NULL when key does not exist
void  bcore_hmap_u3vd_s_setf(            bcore_hmap_u3vd_s* o, u3_t key, fp_t func );
vd_t  bcore_hmap_u3vd_s_remove_h(        bcore_hmap_u3vd_s* o, u3_t key ); // removes key, returns object and transfers ownership if held. Returns NULL if key did not exist.
fp_t  bcore_hmap_u3vd_s_removef_h(       bcore_hmap_u3vd_s* o, u3_t key ); // removes key and returns function pointer. Returns NULL if key did not exist.
void  bcore_hmap_u3vd_s_remove_d(        bcore_hmap_u3vd_s* o, u3_t key ); // removes key and discards object if held. No effect if key did not exist.
vd_t  bcore_hmap_u3vd_s_detach_h(        bcore_hmap_u3vd_s* o, u3_t key ); // sets hold-flag 'false' and returns value
bool  bcore_hmap_u3vd_s_exists(    const bcore_hmap_u3vd_s* o, u3_t key ); // checks if key exists
bool  bcore_hmap_u3vd_s_holds(     const bcore_hmap_u3vd_s* o, u3_t key ); // checks if object is held; Returns false if key does not exist.
void  bcore_hmap_u3vd_s_clear(           bcore_hmap_u3vd_s* o           ); // removes all entries and frees memory
uz_t  bcore_hmap_u3vd_s_keys(      const bcore_hmap_u3vd_s* o           ); // returns number of registered keys
uz_t  bcore_hmap_u3vd_s_size(      const bcore_hmap_u3vd_s* o           ); // returns current size of the hash map (note that this includes empty places)
u3_t  bcore_hmap_u3vd_s_idx_key(   const bcore_hmap_u3vd_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
bool  bcore_hmap_u3vd_s_idx_holds( const bcore_hmap_u3vd_s* o, uz_t idx ); // returns indexed hold-flag (idx indexes the entire table including empty places)
vd_t  bcore_hmap_u3vd_s_idx_val(   const bcore_hmap_u3vd_s* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)
void  bcore_hmap_u3vd_s_run_c(     const bcore_hmap_u3vd_s* o, vd_t obj, void (*fp)( vd_t obj, u3_t key, vd_t  val ) ); // runs a function on all registered key-value pairs
void  bcore_hmap_u3vd_s_run_d(           bcore_hmap_u3vd_s* o, vd_t obj, void (*fp)( vd_t obj, u3_t key, vd_t* val ) ); // runs a function on all registered key-value pairs; value can be changed

/**********************************************************************************************************************/

/** bcore_hmap_tpuz_s:
 *    key-type: tp_t
 *    val-type: uz_t.
 *    content shell { bcore_hnode_tpuz_s []; } data; }
 */
typedef struct bcore_hnode_tpuz_s
{
    tp_t key;
    uz_t val;
} bcore_hnode_tpuz_s;

typedef struct bcore_hmap_tpuz_s
{
    aware_t _;
    bcore_hnode_tpuz_s* nodes;
    bl_t* flags;
    uz_t size;
    uz_t depth_limit;
} bcore_hmap_tpuz_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_hmap_tpuz_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_hmap_tpuz_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_hmap_tpuz_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_hmap_tpuz_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_hmap_tpuz_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_hmap_tpuz_s )

uz_t* bcore_hmap_tpuz_s_get(     const bcore_hmap_tpuz_s* o, tp_t key ); // returns pointer to value or NULL when key does not exist
uz_t* bcore_hmap_tpuz_s_fget(          bcore_hmap_tpuz_s* o, tp_t key, uz_t init_val ); // forced-get: returns pointer to value associated with key; if key does not exist, it is created and value initialized init_val
uz_t* bcore_hmap_tpuz_s_set(           bcore_hmap_tpuz_s* o, tp_t key, uz_t val ); // sets new key; sets/overwrites value and returns pointer to value location
uz_t  bcore_hmap_tpuz_s_remove(        bcore_hmap_tpuz_s* o, tp_t key ); // removes key, returns copy of associated value if existing, 0 otherwise.
bl_t  bcore_hmap_tpuz_s_exists(  const bcore_hmap_tpuz_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_tpuz_s_clear(         bcore_hmap_tpuz_s* o           ); // removes all entries and frees memory
uz_t  bcore_hmap_tpuz_s_keys(    const bcore_hmap_tpuz_s* o           ); // returns number of registered keys
uz_t  bcore_hmap_tpuz_s_size(    const bcore_hmap_tpuz_s* o           ); // returns current size of the hash map (note that this includes empty places)
tp_t  bcore_hmap_tpuz_s_idx_key( const bcore_hmap_tpuz_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
uz_t  bcore_hmap_tpuz_s_idx_val( const bcore_hmap_tpuz_s* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

/** Compute a map from (aware) array where -
 *  - key is the hash from the specified member of each array-element.
 *  - val is the index position of the array-element.
 *  Uses via and hash perspectives.
 *  Elements without 'member_name' are not indexed.
 *  In case of duplicate-key, the first entry for the key is indexed.
 *  Duplicate keys can arise from duplicate member values or hash collisions.
 *  Returns the number of duplicate-keys detected.
 */
sz_t bcore_hmap_tpuz_s_from_array( bcore_hmap_tpuz_s* o, bcore_array* array, tp_t member_name );

/**********************************************************************************************************************/

/** bcore_hmap_tpsz_s:
 *    key-type: tp_t
 *    val-type: sz_t.
 *    content shell { bcore_hnode_tpsz_s []; } data; }
 */
typedef struct bcore_hnode_tpsz_s
{
    tp_t key;
    sz_t val;
} bcore_hnode_tpsz_s;

typedef struct bcore_hmap_tpsz_s
{
    aware_t _;
    bcore_hnode_tpsz_s* nodes;
    bl_t* flags;
    uz_t size;
    uz_t depth_limit;
} bcore_hmap_tpsz_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_hmap_tpsz_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_hmap_tpsz_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_hmap_tpsz_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_hmap_tpsz_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_hmap_tpsz_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_hmap_tpsz_s )

sz_t* bcore_hmap_tpsz_s_get(     const bcore_hmap_tpsz_s* o, tp_t key ); // returns pointer to value or NULL when key does not exist
sz_t* bcore_hmap_tpsz_s_fget(          bcore_hmap_tpsz_s* o, tp_t key, sz_t init_val ); // forced-get: returns pointer to value associated with key; if key does not exist, it is created and value initialized init_val
sz_t* bcore_hmap_tpsz_s_set(           bcore_hmap_tpsz_s* o, tp_t key, sz_t val ); // sets new key; sets/overwrites value and returns pointer to value location
sz_t  bcore_hmap_tpsz_s_remove(        bcore_hmap_tpsz_s* o, tp_t key ); // removes key, returns copy of associated value if existing, 0 otherwise.
bl_t  bcore_hmap_tpsz_s_exists(  const bcore_hmap_tpsz_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_tpsz_s_clear(         bcore_hmap_tpsz_s* o           ); // removes all entries and frees memory
sz_t  bcore_hmap_tpsz_s_keys(    const bcore_hmap_tpsz_s* o           ); // returns number of registered keys
sz_t  bcore_hmap_tpsz_s_size(    const bcore_hmap_tpsz_s* o           ); // returns current size of the hash map (note that this includes empty places)
tp_t  bcore_hmap_tpsz_s_idx_key( const bcore_hmap_tpsz_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
sz_t  bcore_hmap_tpsz_s_idx_val( const bcore_hmap_tpsz_s* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

/** Compute a map from (aware) array where -
 *  - key is the hash from the specified member of each array-element.
 *  - val is the index position of the array-element.
 *  Uses via and hash perspectives.
 *  Elements without 'member_name' are not indexed.
 *  In case of duplicate-key, the first entry for the key is indexed.
 *  Duplicate keys can arise from duplicate member values or hash collisions.
 *  Returns the number of duplicate-keys detected.
 */
sz_t bcore_hmap_tpsz_s_from_array( bcore_hmap_tpsz_s* o, bcore_array* array, tp_t member_name );

/**********************************************************************************************************************/

/** bcore_hmap_tpfp_s:
 *    key-type: tp_t
 *    val-type: fp_t.
 *    Not serializable
 */
typedef struct bcore_hnode_tpfp_s
{
    tp_t key;
    fp_t val;
} bcore_hnode_tpfp_s;

typedef struct bcore_hmap_tpfp_s
{
    aware_t _;
    bcore_hnode_tpfp_s* nodes;
    bl_t* flags;
    uz_t size;
    uz_t depth_limit;
} bcore_hmap_tpfp_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_hmap_tpfp_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_hmap_tpfp_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_hmap_tpfp_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_hmap_tpfp_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_hmap_tpfp_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_hmap_tpfp_s )

fp_t* bcore_hmap_tpfp_s_get(     const bcore_hmap_tpfp_s* o, tp_t key ); // returns pointer to value or NULL when key does not exist
fp_t* bcore_hmap_tpfp_s_fget(          bcore_hmap_tpfp_s* o, tp_t key, fp_t init_val ); // forced-get: returns pointer to value associated with key; if key does not exist, it is created and value initialized init_val
fp_t* bcore_hmap_tpfp_s_set(           bcore_hmap_tpfp_s* o, tp_t key, fp_t val ); // sets new key; sets/overwrites value and returns pointer to value location
fp_t  bcore_hmap_tpfp_s_remove(        bcore_hmap_tpfp_s* o, tp_t key ); // removes key, returns copy of associated value if existing, 0 otherwise.
bl_t  bcore_hmap_tpfp_s_exists(  const bcore_hmap_tpfp_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_tpfp_s_clear(         bcore_hmap_tpfp_s* o           ); // removes all entries and frees memory
uz_t  bcore_hmap_tpfp_s_keys(    const bcore_hmap_tpfp_s* o           ); // returns number of registered keys
uz_t  bcore_hmap_tpfp_s_size(    const bcore_hmap_tpfp_s* o           ); // returns current size of the hash map (note that this includes empty places)
tp_t  bcore_hmap_tpfp_s_idx_key( const bcore_hmap_tpfp_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
fp_t  bcore_hmap_tpfp_s_idx_val( const bcore_hmap_tpfp_s* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

/**********************************************************************************************************************/

/** bcore_hmap_tptp_s:
 *    key-type: tp_t
 *    val-type: tp_t.
 *    content shell { bcore_hnode_tptp_s []; } data;
 */
typedef struct bcore_hnode_tptp_s
{
    tp_t key;
    tp_t val;
} bcore_hnode_tptp_s;

typedef struct bcore_hmap_tptp_s
{
    aware_t _;
    bcore_hnode_tptp_s* nodes;
    bl_t* flags;
    uz_t size;
    uz_t depth_limit;
} bcore_hmap_tptp_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_hmap_tptp_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_hmap_tptp_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_hmap_tptp_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_hmap_tptp_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_hmap_tptp_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_hmap_tptp_s )

tp_t* bcore_hmap_tptp_s_get(     const bcore_hmap_tptp_s* o, tp_t key ); // returns pointer to value or NULL when key does not exist
tp_t* bcore_hmap_tptp_s_fget(          bcore_hmap_tptp_s* o, tp_t key, tp_t init_val ); // forced-get: returns pointer to value associated with key; if key does not exist, it is created and value initialized init_val
tp_t* bcore_hmap_tptp_s_set(           bcore_hmap_tptp_s* o, tp_t key, tp_t val ); // sets new key; sets/overwrites value and returns pointer to value location
tp_t  bcore_hmap_tptp_s_remove(        bcore_hmap_tptp_s* o, tp_t key ); // removes key, returns copy of associated value if existing, 0 otherwise.
bl_t  bcore_hmap_tptp_s_exists(  const bcore_hmap_tptp_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_tptp_s_clear(         bcore_hmap_tptp_s* o           ); // removes all entries and frees memory
uz_t  bcore_hmap_tptp_s_keys(    const bcore_hmap_tptp_s* o           ); // returns number of registered keys
uz_t  bcore_hmap_tptp_s_size(    const bcore_hmap_tptp_s* o           ); // returns current size of the hash map (note that this includes empty places)
tp_t  bcore_hmap_tptp_s_idx_key( const bcore_hmap_tptp_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
tp_t  bcore_hmap_tptp_s_idx_val( const bcore_hmap_tptp_s* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

/**********************************************************************************************************************/

/** bcore_hmap_tpvd_s: maps keys to pointer
 *    key-type: tp_t
 *    val-type: vd_t (not owned, no assumption about type)
 *    shell: no shell
 */
typedef struct bcore_hnode_tpvd_s
{
    tp_t key;
    vd_t val;
} bcore_hnode_tpvd_s;

BCORE_DECLARE_OBJECT( bcore_hmap_tpvd_s )
{
    aware_t _;
    bcore_hnode_tpvd_s* nodes;
    bl_t* flags;
    uz_t size;
    uz_t depth_limit;
};

vd_t* bcore_hmap_tpvd_s_set(           bcore_hmap_tpvd_s* o, tp_t key, vd_t val ); // sets new key; sets/overwrites value and returns pointer to value location
vd_t* bcore_hmap_tpvd_s_get(     const bcore_hmap_tpvd_s* o, tp_t key ); // returns pointer to value or NULL when key does not exist
void  bcore_hmap_tpvd_s_remove(        bcore_hmap_tpvd_s* o, tp_t key ); // removes key, destroys associated object (if present)
bl_t  bcore_hmap_tpvd_s_exists(  const bcore_hmap_tpvd_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_tpvd_s_clear(         bcore_hmap_tpvd_s* o           ); // removes all entries and frees memory
uz_t  bcore_hmap_tpvd_s_keys(    const bcore_hmap_tpvd_s* o           ); // returns number of registered keys
uz_t  bcore_hmap_tpvd_s_size(    const bcore_hmap_tpvd_s* o           ); // returns current size of the hash map (note that this includes empty places)
tp_t  bcore_hmap_tpvd_s_idx_key( const bcore_hmap_tpvd_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
vd_t  bcore_hmap_tpvd_s_idx_val( const bcore_hmap_tpvd_s* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

/**********************************************************************************************************************/

/** bcore_hmap_tpto_s: maps keys to objects of same type
 *    key-type: tp_t
 *    val-type: vd_t. (typed, owned)
 *    All values are either NULL or of the same type.
 *    shell: { tp_t type; { tp_t key; sr_s obj; } []; } data;
 */
typedef struct bcore_hnode_tpto_s
{
    tp_t key;
    vd_t val;
} bcore_hnode_tpto_s;

typedef struct bcore_hmap_tpto_s
{
    aware_t _;
    tp_t type;
    bcore_hnode_tpto_s* nodes;
    bl_t* flags;
    uz_t size;
    uz_t depth_limit;
} bcore_hmap_tpto_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_hmap_tpto_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_hmap_tpto_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_hmap_tpto_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_hmap_tpto_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_hmap_tpto_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_hmap_tpto_s )

void  bcore_hmap_tpto_s_set_type(      bcore_hmap_tpto_s* o, tp_t type ); // sets type; resets array in case of type difference
vd_t* bcore_hmap_tpto_s_get(     const bcore_hmap_tpto_s* o, tp_t key ); // returns pointer to value or NULL when key does not exist
vd_t* bcore_hmap_tpto_s_fget_d(        bcore_hmap_tpto_s* o, tp_t key, vd_t init_val ); // forced-get: returns pointer to value associated with key; if key does not exist, it is created and value initialized init_val
vd_t  bcore_hmap_tpto_s_fget(          bcore_hmap_tpto_s* o, tp_t key ); // forced-get: returns pointer to value associated with key; if key does not exist, it is created and the value created according to type
vd_t* bcore_hmap_tpto_s_set_d(         bcore_hmap_tpto_s* o, tp_t key, vd_t val ); // sets new key; sets/overwrites value and returns pointer to value location
void  bcore_hmap_tpto_s_remove(        bcore_hmap_tpto_s* o, tp_t key ); // removes key, destroys associated object (if present)
bl_t  bcore_hmap_tpto_s_exists(  const bcore_hmap_tpto_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_tpto_s_clear(         bcore_hmap_tpto_s* o           ); // removes all entries and frees memory
uz_t  bcore_hmap_tpto_s_keys(    const bcore_hmap_tpto_s* o           ); // returns number of registered keys
uz_t  bcore_hmap_tpto_s_size(    const bcore_hmap_tpto_s* o           ); // returns current size of the hash map (note that this includes empty places)
tp_t  bcore_hmap_tpto_s_idx_key( const bcore_hmap_tpto_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
vd_t  bcore_hmap_tpto_s_idx_val( const bcore_hmap_tpto_s* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

/**********************************************************************************************************************/

/** bcore_hmap_tpaw_s: maps keys to aware object
 *    key-type: tp_t
 *    val-type: vd_t. (aware, owned)
 *    shell: { { tp_t key; aware => val; } []; } data;
 *
 *  Routines cache the perspective instead querying it for each type.
 *  Hence, if most or all elements are of the same type, performance runs close to hmap_tpto
 *  This map is well suited for use in generic mapping of aware types.
 */
typedef struct bcore_hnode_tpaw_s
{
    tp_t key;
    vd_t val;
} bcore_hnode_tpaw_s;

typedef struct bcore_hmap_tpaw_s
{
    aware_t _;
    bcore_hnode_tpaw_s* nodes;
    bl_t* flags;
    uz_t size;
    uz_t depth_limit;
} bcore_hmap_tpaw_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_hmap_tpaw_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_hmap_tpaw_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_hmap_tpaw_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_hmap_tpaw_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_hmap_tpaw_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_hmap_tpaw_s )

vd_t* bcore_hmap_tpaw_s_get(     const bcore_hmap_tpaw_s* o, tp_t key ); // returns pointer to value or NULL when key does not exist
vd_t* bcore_hmap_tpaw_s_set_c(         bcore_hmap_tpaw_s* o, tp_t key, vc_t val ); // discards old object (if any); sets new key; copies object; returns pointer to pointer location
vd_t* bcore_hmap_tpaw_s_set_d(         bcore_hmap_tpaw_s* o, tp_t key, vd_t val ); // discards old object (if any); sets new key; assigns object pointer; returns pointer to pointer location
void  bcore_hmap_tpaw_s_remove(        bcore_hmap_tpaw_s* o, tp_t key ); // removes key, discards associated object (if present)
bl_t  bcore_hmap_tpaw_s_exists(  const bcore_hmap_tpaw_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_tpaw_s_clear(         bcore_hmap_tpaw_s* o           ); // removes all entries and frees memory
uz_t  bcore_hmap_tpaw_s_keys(    const bcore_hmap_tpaw_s* o           ); // returns number of registered keys
uz_t  bcore_hmap_tpaw_s_size(    const bcore_hmap_tpaw_s* o           ); // returns current size of the hash map (note that this includes empty places)
tp_t  bcore_hmap_tpaw_s_idx_key( const bcore_hmap_tpaw_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
vd_t  bcore_hmap_tpaw_s_idx_val( const bcore_hmap_tpaw_s* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

/**********************************************************************************************************************/

/** bcore_hmap_tp_s:  maps a key to automatic index
 *    key-type: tp_t
 *    val-type: no explicit value
 *    content shell: { tp_t []; } data;
 */
typedef struct bcore_hmap_tp_s
{
    aware_t _;
    tp_t* keys;
    bl_t* flags; // flags for tracing
    uz_t size;
    uz_t depth_limit;
} bcore_hmap_tp_s;

BCORE_DECLARE_FUNCTION_INIT(    bcore_hmap_tp_s )
BCORE_DECLARE_FUNCTION_DOWN(    bcore_hmap_tp_s )
BCORE_DECLARE_FUNCTION_COPY(    bcore_hmap_tp_s )
BCORE_DECLARE_FUNCTION_CREATE(  bcore_hmap_tp_s )
BCORE_DECLARE_FUNCTION_DISCARD( bcore_hmap_tp_s )
BCORE_DECLARE_FUNCTION_CLONE(   bcore_hmap_tp_s )

uz_t  bcore_hmap_tp_s_get(     const bcore_hmap_tp_s* o, tp_t key ); // returns index of key; if not existing, index is o->size
uz_t  bcore_hmap_tp_s_set(           bcore_hmap_tp_s* o, tp_t key ); // sets new key if not already existing; returns index
uz_t  bcore_hmap_tp_s_remove(        bcore_hmap_tp_s* o, tp_t key ); // removes key, returns old index
bl_t  bcore_hmap_tp_s_exists(  const bcore_hmap_tp_s* o, tp_t key ); // checks if key exists
void  bcore_hmap_tp_s_clear(         bcore_hmap_tp_s* o           ); // removes all entries and frees memory
uz_t  bcore_hmap_tp_s_keys(    const bcore_hmap_tp_s* o           ); // returns number of registered keys
uz_t  bcore_hmap_tp_s_size(    const bcore_hmap_tp_s* o           ); // returns current size of the hash map (note that this includes empty places)
tp_t  bcore_hmap_tp_s_idx_key( const bcore_hmap_tp_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)

/**********************************************************************************************************************/

vd_t bcore_hmap_signal_handler( const bcore_signal_s* o );

#endif // BCORE_HMAP_H

