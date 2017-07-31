/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_HMAP_H
#define BCORE_HMAP_H

#include "bcore_features.h"
#include "bcore_string.h"

/** Hashmap implementing a flavor of "cuckoo hashing" with single monolithic storage and
 *  up to three hash functions (default: 2 functions).
 *  Memory requirements (2 hash function) is comparable to btree.
 *
 *  Features:
 *    * Supports instance perspective.
 *    * Can manage data and function pointers
 *    * Can hold (==own) referenced data objects
 *    * Storage efficiency around 50% with 2 hash functions and around 90% with 3 hash functions.
 *
 *  Advantages:
 *     Complexity   : O(1) for insert, remove, retrieve.
 *     Average speed: Typically one order of magnitude better than btree. (2 hash functions)
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

/** bcore_hmap_u2vd_s:
 *    key-type: u2_d
 *    val-type: vd_t.
 *    Maximum number of keys: 2^32
 *    Map holds aware objects if so desired.
 *    Map can reference any object but hold only aware objects.
 */
typedef struct bcore_hmap_u2vd_s bcore_hmap_u2vd_s;
typedef u2_t (*bcore_hash_u2u2)( u2_t v );

DECLARE_FUNCTION_INIT(    bcore_hmap_u2vd_s )
DECLARE_FUNCTION_DOWN(    bcore_hmap_u2vd_s )
DECLARE_FUNCTION_COPY(    bcore_hmap_u2vd_s )
DECLARE_FUNCTION_CREATE(  bcore_hmap_u2vd_s )
DECLARE_FUNCTION_DISCARD( bcore_hmap_u2vd_s )
DECLARE_FUNCTION_CLONE(   bcore_hmap_u2vd_s )

void  bcore_hmap_u2vd_s_set_hash_function( bcore_hmap_u2vd_s* o, sz_t index, bcore_hash_u2u2 hf ); // optionally sets external hash function (up to three functions can be specified via index 0, 1, 2)
vd_t* bcore_hmap_u2vd_s_get(    const bcore_hmap_u2vd_s* o, u2_t key ); // returns pinter to value or NULL when key does not exist
void  bcore_hmap_u2vd_s_set(          bcore_hmap_u2vd_s* o, u2_t key, vd_t val, bool hold ); // hold==true only for aware objects
fp_t* bcore_hmap_u2vd_s_getf(   const bcore_hmap_u2vd_s* o, u2_t key ); // returns pointer to function pointer; returns NULL when key does not exist
void  bcore_hmap_u2vd_s_setf(         bcore_hmap_u2vd_s* o, u2_t key, fp_t func );
vd_t  bcore_hmap_u2vd_s_remove_h(     bcore_hmap_u2vd_s* o, u2_t key ); // removes key, returns object and transfers ownership if held. Returns NULL if key did not exist.
fp_t  bcore_hmap_u2vd_s_removef_h(    bcore_hmap_u2vd_s* o, u2_t key ); // removes key and returns function pointer. Returns NULL if key did not exist.
void  bcore_hmap_u2vd_s_remove_d(     bcore_hmap_u2vd_s* o, u2_t key ); // removes key and discards object if held. No effect if key did not exist.
vd_t  bcore_hmap_u2vd_s_detach_h(     bcore_hmap_u2vd_s* o, u2_t key ); // sets hold-flag 'false' and returns value
bool  bcore_hmap_u2vd_s_exists( const bcore_hmap_u2vd_s* o, u2_t key ); // checks if key exists
bool  bcore_hmap_u2vd_s_holds(  const bcore_hmap_u2vd_s* o, u2_t key ); // checks if object is held; Returns false if key does not exist.
void  bcore_hmap_u2vd_s_clear(        bcore_hmap_u2vd_s* o           ); // removes all entries and frees memory
sz_t  bcore_hmap_u2vd_s_keys(   const bcore_hmap_u2vd_s* o           ); // returns number of registered keys
sz_t  bcore_hmap_u2vd_s_size(   const bcore_hmap_u2vd_s* o           ); // returns current size of the hash map (note that this includes empty places)
u2_t  bcore_hmap_u2vd_s_idx_key(   const bcore_hmap_u2vd_s* o, sz_t idx ); // returns indexed key (idx indexs the entire table including empty places)
bool  bcore_hmap_u2vd_s_idx_holds( const bcore_hmap_u2vd_s* o, sz_t idx ); // returns indexed hold-flag (idx indexs the entire table including empty places)
vd_t  bcore_hmap_u2vd_s_idx_val(   const bcore_hmap_u2vd_s* o, sz_t idx ); // returns indexed value (idx indexs the entire table including empty places)
void  bcore_hmap_u2vd_s_run_c(  const bcore_hmap_u2vd_s* o, vd_t obj, void (*fp)( vd_t obj, u2_t key, vd_t  val ) ); // runs a function on all registered key-value pairs
void  bcore_hmap_u2vd_s_run_d(        bcore_hmap_u2vd_s* o, vd_t obj, void (*fp)( vd_t obj, u2_t key, vd_t* val ) ); // runs a function on all registered key-value pairs; value can be changed

/// self reflection
typedef struct bcore_flect_self_s bcore_flect_self_s;
bcore_flect_self_s* bcore_hmap_u2vd_s_create_self( void );

bcore_string_s* bcore_hmap_u2vd_selftest( void );

/**********************************************************************************************************************/

#endif // BCORE_HMAP_H

