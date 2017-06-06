/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** btree on the basis of a self-balancing 2-3 tree structure.
 *  This structure can be used for well-scalable associative data management.
 *  Wost case complexity is O(log(n)) for access, insertion and deletion.
 *
 *  Internally, this solution uses a node structure with three child pointers, one parent pointer
 *  and two key-value containers.
 *  Root-state is indicated by parent==NULL.
 *  Leaf-state is indicated by child0==BNUL.
 *  Single-key-state is indicated by child2==NULL.
 *  Therefore additional state-flags are not needed.
 *
 *  Available btree structures
 *    - btree_ip: key: u3_t,     value: void*
 *    - btree_ps: key: void*,    value: size_t
 *    - btree_vd: key: void*,    value: no dedicated value
*/

#ifndef BCORE_BTREE_H
#define BCORE_BTREE_H

#include "bcore_first.h"
#include "bcore_string.h"
#include "bcore_types.h"

/**********************************************************************************************************************/
// tree of u3_t as key and void* as value

typedef u3_t bcore_btree_ip_key_t;
typedef vd_t bcore_btree_ip_val_t;

struct bcore_btree_ip_s;
typedef struct bcore_btree_ip_s bcore_btree_ip_s;

/// Creates a new btree_ip
bcore_btree_ip_s* bcore_btree_ip_s_create();

/// Deletes a btree_ip
void bcore_btree_ip_s_discard( bcore_btree_ip_s* o );

/** Returns pointer to the value associated with given key.
 *  Returns NULL when the key does not exist.
 */
bcore_btree_ip_val_t* bcore_btree_ip_s_val( const bcore_btree_ip_s* o, bcore_btree_ip_key_t key );

/** Sets a key-value pair in the tree.
 *  If the key already exists, its value is overwritten.
 *  Return value:
 *    0: key, val already existed -> nothing changed
 *    1: key did not exist (key, val) was created
 *   -1: key already existed but with different value -> value was overwritten
 *   -2: internal error
 */
int bcore_btree_ip_s_set( bcore_btree_ip_s* o, bcore_btree_ip_key_t key, bcore_btree_ip_val_t val );

/** Removes a key from the tree.
 *  Return value:
 *    0: key did not exist -> nothing changed
 *    1: key found and removed
 *   -1: internal error
 */
int bcore_btree_ip_s_remove( bcore_btree_ip_s* o, bcore_btree_ip_key_t key );

/// calls a function for all tree elements
void bcore_btree_ip_s_run( const bcore_btree_ip_s* o, void(*func)( void* arg, bcore_btree_ip_key_t key, bcore_btree_ip_val_t val ), void* arg );

// Testing and debugging

/// shows the tree structure as text string
bcore_string_s* bcore_btree_ip_s_show( struct bcore_btree_ip_s* o );

/// selftest of the btree implementation; returns log string
bcore_string_s* bcore_btree_ip_s_selftest();

/**********************************************************************************************************************/
/**********************************************************************************************************************/
// tree of void* as key and sz_t as value

typedef vd_t bcore_btree_ps_key_t;
typedef sz_t bcore_btree_ps_val_t;

struct bcore_btree_ps_s;
typedef struct bcore_btree_ps_s bcore_btree_ps_s;

/// Creates a new btree_ip
bcore_btree_ps_s* bcore_btree_ps_s_create( void* (*alloc)( void*, size_t size ) );

/// Deletes a btree_ip
void bcore_btree_ps_s_discard( bcore_btree_ps_s* o );

/** Returns pointer to the value associated with given key.
 *  Returns NULL when the key does not exist.
 */
bcore_btree_ps_val_t* bcore_btree_ps_s_val( const bcore_btree_ps_s* o, bcore_btree_ps_key_t key );

/** Sets a key-value pair in the tree.
 *  If the key already exists, its value is overwritten.
 *  Return value:
 *    0: key, val already existed -> nothing changed
 *    1: key did not exist (key, val) was created
 *   -1: key already existed but with different value -> value was overwritten
 *   -2: internal error
 */
int bcore_btree_ps_s_set( bcore_btree_ps_s* o, bcore_btree_ps_key_t key, bcore_btree_ps_val_t val );

/** Removes a key from the tree.
 *  Return value:
 *    0: key did not exist -> nothing changed
 *    1: key found and removed
 *   -1: internal error
 */
int bcore_btree_ps_s_remove( bcore_btree_ps_s* o, bcore_btree_ps_key_t key );

/// calls a function for all tree elements
void bcore_btree_ps_s_run(   const bcore_btree_ps_s* o, void(*func)( void* arg, bcore_btree_ps_key_t key, bcore_btree_ps_val_t val ), void* arg );

/// counts entries for which func returns true; counts all entries in case func is NULL
sz_t bcore_btree_ps_s_count( const bcore_btree_ps_s* o, bool(*func)( void* arg, bcore_btree_ps_key_t key, bcore_btree_ps_val_t val ), void* arg );

/// sums entries for which func returns true; sums all entries in case func is NULL
sz_t bcore_btree_ps_s_sum( const bcore_btree_ps_s* o, bool(*func)( void* arg, bcore_btree_ps_key_t key, bcore_btree_ps_val_t val ), void* arg );

/// return depth of tree
sz_t bcore_btree_ps_s_depth( const bcore_btree_ps_s* o );

// Testing and debugging

/// shows the tree structure as text string
bcore_string_s* bcore_btree_ps_s_show( struct bcore_btree_ps_s* o );

/// selftest of the btree implementation; returns log string
bcore_string_s* bcore_btree_ps_s_selftest();

/**********************************************************************************************************************/
/**********************************************************************************************************************/
// tree of void* as key (no dedicated value)

typedef vd_t bcore_btree_vd_key_t;

struct bcore_btree_vd_s;
typedef struct bcore_btree_vd_s bcore_btree_vd_s;

struct bcore_btree_vd_s;
typedef struct bcore_btree_vd_s bcore_btree_vd_s;

/// Creates a new btree_vd (allows to specify alloc function because this tree type is used in memory management)
bcore_btree_vd_s* bcore_btree_vd_s_create( void* (*alloc)( void*, size_t size ) );

/// Deletes a btree_vd
void bcore_btree_vd_s_discard( bcore_btree_vd_s* o );

/// Checks existence of key
bool bcore_btree_vd_s_exists( const bcore_btree_vd_s* o, bcore_btree_vd_key_t key );

/// Returns the largest stored key euqal or below <key>. Returns NULL in case all stored keys are larger than <key>.
bcore_btree_vd_key_t bcore_btree_vd_s_largest_below_equal( const bcore_btree_vd_s* o, bcore_btree_vd_key_t key );

/** Sets a key in the tree.
 *  Return value:
 *    0: key already existed -> nothing changed
 *    1: key did not exist and was created
 *   -2: internal error
 */
int bcore_btree_vd_s_set( bcore_btree_vd_s* o, bcore_btree_vd_key_t key );

/** Removes a key from the tree.
 *  Return value:
 *    0: key did not exist -> nothing changed
 *    1: key found and removed
 *   -1: internal error
 */
int bcore_btree_vd_s_remove( bcore_btree_vd_s* o, bcore_btree_vd_key_t key );

/// calls a function for all tree elements
void bcore_btree_vd_s_run( const bcore_btree_vd_s* o, void(*func)( void* arg, bcore_btree_vd_key_t key ), void* arg );

/// counts entries for which func returns true; counts all entries in case func is NULL
sz_t bcore_btree_vd_s_count( const bcore_btree_vd_s* o, bool(*func)( void* arg, bcore_btree_vd_key_t key ), void* arg );

/// return depth of tree
sz_t bcore_btree_vd_s_depth( const bcore_btree_vd_s* o );

// Testing and debugging

/// shows the tree structure as text string
bcore_string_s* bcore_btree_vd_s_show( struct bcore_btree_vd_s* o );

/// selftest of the btree implementation; returns log string
bcore_string_s* bcore_btree_vd_s_selftest();

#endif // BCORE_BTREE_H


