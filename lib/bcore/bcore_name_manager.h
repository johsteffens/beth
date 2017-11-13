/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

/** Name management framework
 *  This framework links names to hash values and vice versa.
 *  Hash value 0 is reserved. Names resulting in hash 0 are rejected.
 */

#ifndef BCORE_NAME_MANAGER_H
#define BCORE_NAME_MANAGER_H

#include "bcore_name.h"

/** Names and Name-Spaces
 *  Each name has an associated name-space.
 *  The root name space (default) has type 0.
 *  Each name-type can be used as name-space.
 *  Functions with postfix _s explicitly consider name spaces:
 *     Functions returning a string construct a full name using ':' to catenate name spaces.
 *     Functions returning a type expect the name space as argument
 *  It is
 *  typeof_s( typeof( "mynamespace" ), "myname" ) == typeof( "mynamespace:myname" ).
 *
 *  Accordingly...
 *    tp_t type = entypeof_s( typeof( "mynamespace" ), "myname" )
 *  or
 *    tp_t type = entypeof( "mynamespace:myname" )
 *  ... would produce the same value of type.
 *  However, nameof( type ) would yield different names.
 *  Therefore, trying to enroll both, causes a collision error.
 *  Best practice: Avoid colons inside regular names.
 */

/// forward declarations
typedef struct st_s st_s;
st_s* st_s_create();

/// enroll name in global manager (thread safe); checks for collisions; returns hash
tp_t bcore_name_enroll(                     sc_t name );
tp_t bcore_name_enroll_n(                   sc_t name, sz_t n );
tp_t bcore_name_enroll_s(  tp_t name_space, sc_t name );
tp_t bcore_name_enroll_sn( tp_t name_space, sc_t name, sz_t n );

/// hash --> name; returns NULL when not enrolled (thread safe)
sc_t bcore_name_try_name( tp_t type );

/// hash --> name; returns error when not enrolled (thread safe)
sc_t bcore_name_get_name( tp_t type );

/// hash --> full name; returns NULL when not enrolled (thread safe); passes ownership
st_s* bcore_name_try_name_s( tp_t type );

/// hash --> full name; returns error when not enrolled (thread safe); passes ownership
st_s* bcore_name_get_name_s( tp_t type );

/// removes hash and associated name; no effect when not enrolled (thread safe)
void bcore_name_remove( tp_t type );

/// syntactic sugar
static inline tp_t typeof(               sc_t name         ) { return bcore_name_key(          name    ); }
static inline tp_t typeof_n(             sc_t name, sz_t n ) { return bcore_name_key_n(        name, n ); }
static inline tp_t typeof_ns(   tp_t ns, sc_t name         ) { return bcore_name_key_ns(   ns, name    ); }
static inline tp_t typeof_ns_n( tp_t ns, sc_t name, sz_t n ) { return bcore_name_key_ns_n( ns, name, n ); }

static inline tp_t entypeof(               sc_t name         ) { return bcore_name_enroll(        name    ); }
static inline tp_t entypeof_n(             sc_t name, sz_t n ) { return bcore_name_enroll_n(      name, n ); }
static inline tp_t entypeof_ns(   tp_t ns, sc_t name         ) { return bcore_name_enroll_s(  ns, name    ); }
static inline tp_t entypeof_ns_n( tp_t ns, sc_t name, sz_t n ) { return bcore_name_enroll_sn( ns, name, n ); }

static inline sc_t    nameof(   u2_t type ) { return bcore_name_get_name( type ); }
static inline sc_t  ifnameof(   u2_t type ) { sc_t n = bcore_name_try_name( type ); return n ? n : ""; }
static inline st_s*   nameof_s( u2_t type ) { return bcore_name_get_name_s( type ); }
static inline st_s* ifnameof_s( u2_t type ) { st_s* n = bcore_name_try_name_s( type ); return n ? n : st_s_create(); }

/// statistics
typedef struct st_s st_s;
sz_t  bcore_name_size(); // number of registered names
st_s* bcore_name_show(); // store all names in a string, separated by newline

bcore_name_map_s* bcore_name_create_name_map();

vd_t bcore_name_manager_signal( tp_t target, tp_t signal, vd_t object );

#endif // BCORE_NAME_MANAGER_H
