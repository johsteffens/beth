/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

/** Name management framework
 *  This framework links names to hash values and vice versa.
 *  Hash value 0 is reserved. Names resulting in hash 0 are rejected.
 *  'ns' appreviates 'namespace':
 *  bcore_name_get_hash( "prefix_postfix" ) ==
 *  bcore_name_ns_get_hash( bcore_name_get_hash( "prefix" ), "postfix" )
 */

#ifndef BCORE_NAME_MANAGER_H
#define BCORE_NAME_MANAGER_H

#include "bcore_control.h"

/// enroll name in global manager (thread safe); checks for collisions; returns hash
tp_t bcore_name_enroll(   sc_t name );
tp_t bcore_name_enroll_n( sc_t name, sz_t n );

/// name --> hash
static inline tp_t bcore_name_get_hash(   sc_t name         ) { return bcore_fnv_hash_text_u2( name    ); }
static inline tp_t bcore_name_get_hash_n( sc_t name, sz_t n ) { return bcore_fnv_hash_data_u2( name, n ); }

/// hash --> name; returns NULL when not enrolled (thread safe)
sc_t bcore_name_try_name( tp_t type );

/// hash --> name; returns error when not enrolled (thread safe)
sc_t bcore_name_get_name( tp_t type );

/// removes hash and associated name; no effect when not enrolled (thread safe)
void bcore_name_remove( tp_t type );

/// syntactic sugar
static inline tp_t typeof(   sc_t name ) { return bcore_name_get_hash( name ); }
static inline tp_t typeof_n( sc_t name, sz_t n ) { return bcore_name_get_hash_n( name, n ); }
static inline tp_t entypeof( sc_t name ) { return bcore_name_enroll(   name ); }
static inline tp_t entypeof_n( sc_t name, sz_t n ) { return bcore_name_enroll_n( name, n ); }
static inline sc_t nameof(   u2_t type ) { return bcore_name_get_name(   type ); }
static inline sc_t ifnameof( u2_t type ) { sc_t n = bcore_name_try_name( type ); return n ? n : ""; }

vd_t bcore_name_manager_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_NAME_MANAGER_H
