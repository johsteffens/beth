/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_HMAP_H
#define BCORE_HMAP_H

#include "bcore_features.h"
#include "bcore_string.h"

/** Hashmap implementing a flavor of "cuckoo hashing" with single monolithic storage and
 *  up to three hash functions (default: 2 functions).
 *  Average storage efficiency is comparable to btree.
 *
 *  Advantages:
 *     Complexity   : O(1) for insert, remove, retrieve.
 *     Average speed: Typically one order of magnitude better than btree.
 *
 *  Disadvantage:
 *     No well defined lower bound on worst case storage efficiency.
 *     However, in practice a significant deviation from average efficiency is very unlikely.
 *     (See studies on cuckoo hashing for more details).
 */

/**********************************************************************************************************************/

/** bcore_hmap_u2vd_s:
 *    key-type: u2_d
 *    val-type: vd_t.
 *    Maximum number of keys: 2^32
 *    Map does not assume ownership of objects referenced by val.
 */
typedef struct bcore_hmap_u2vd_s bcore_hmap_u2vd_s;
typedef u2_t (*bcore_hash_u2u2)( u2_t v );

DECLARE_FUNCTION_INIT(    bcore_hmap_u2vd_s )
DECLARE_FUNCTION_DOWN(    bcore_hmap_u2vd_s )
DECLARE_FUNCTION_COPY(    bcore_hmap_u2vd_s )
DECLARE_FUNCTION_CREATE(  bcore_hmap_u2vd_s )
DECLARE_FUNCTION_DISCARD( bcore_hmap_u2vd_s )
DECLARE_FUNCTION_CLONE(   bcore_hmap_u2vd_s )

void  bcore_hmap_u2vd_s_set_hash(     bcore_hmap_u2vd_s* o, sz_t index, bcore_hash_u2u2 hash ); // sets optional external hash function (up to three functions can be specified via index 0, 1, 2)
vd_t* bcore_hmap_u2vd_s_get(    const bcore_hmap_u2vd_s* o, u2_t key ); // returns pinter to value or NULL when key does not exist
void  bcore_hmap_u2vd_s_set(          bcore_hmap_u2vd_s* o, u2_t key, vd_t val );
vd_t  bcore_hmap_u2vd_s_remove(       bcore_hmap_u2vd_s* o, u2_t key ); // removes key and returns its value. returns NULL if key did not exist
bool  bcore_hmap_u2vd_s_exists( const bcore_hmap_u2vd_s* o, u2_t key ); // checks if key exists
void  bcore_hmap_u2vd_s_clear(        bcore_hmap_u2vd_s* o           ); // removes all entries and frees memory
sz_t  bcore_hmap_u2vd_s_keys(   const bcore_hmap_u2vd_s* o           ); // returns number of registered keys
sz_t  bcore_hmap_u2vd_s_size(   const bcore_hmap_u2vd_s* o           ); // returns current size of the hash map
void  bcore_hmap_u2vd_s_run_c(  const bcore_hmap_u2vd_s* o, void (*fp)( u2_t key, vd_t  val ) ); // runs a function on all registered key-value pairs
void  bcore_hmap_u2vd_s_run_d(        bcore_hmap_u2vd_s* o, void (*fp)( u2_t key, vd_t* val ) ); // runs a function on all registered key-value pairs; value can be changed

bcore_string_s* bcore_hmap_u2vd_selftest();

/**********************************************************************************************************************/

#endif // BCORE_HMAP_H

