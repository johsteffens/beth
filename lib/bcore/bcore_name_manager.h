/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** Name management framework
 *  This framework links names to hash values and vice versa.
 *  Hash value 0 is reserved. Names resulting in hash 0 are rejected.
 */

#ifndef BCORE_NAME_MANAGER_H
#define BCORE_NAME_MANAGER_H

#include "bcore_control.h"

/// initializes name manager
void bcore_name_manager_open();

/// closes name manager and frees all memory
void bcore_name_manager_close();

/// enroll name in global manager (thread safe); checks for collisions; returns hash
tp_t bcore_name_enroll( sc_t name );

/// name --> hash
static inline tp_t bcore_name_get_hash( sc_t name ) { return bcore_fnv_hash_text_u2( name ); }

/// hash --> name; returns NULL when not enrolled (thread safe)
sc_t bcore_name_try_name( tp_t type );

/// hash --> name; returns error when not enrolled (thread safe)
sc_t bcore_name_get_name( tp_t type );

/// removes hash and associated name; no effect when not enrolled (thread safe)
void bcore_name_remove( tp_t type );

/// syntactic sugar
static inline u2_t typeof(   sc_t name ) { return bcore_name_get_hash( name ); }
static inline sc_t nameof(   u2_t type ) { return bcore_name_get_name( type ); }
static inline sc_t ifnameof( u2_t type ) { sc_t n = bcore_name_try_name( type ); return n ? n : ""; }

#endif // BCORE_NAME_MANAGER_H
