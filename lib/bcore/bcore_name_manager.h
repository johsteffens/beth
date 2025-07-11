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

/** Name management framework
 *  This framework links names to hash values and vice versa.
 *  Hash value 0 is reserved. Names resulting in hash 0 are rejected.
 *  Collisions are checked where possible.
 *
 *  LIST OF KNOWN COLLISIONS:
 *    - so far none is known -
 */

#ifndef BCORE_NAME_MANAGER_H
#define BCORE_NAME_MANAGER_H

#include "bcore_name.h"

BCORE_FORWARD_OBJECT( bcore_hmap_name_s );

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
 *  ... produce the same value of type.
 *  However, nameof( type ) would yield different names.
 *  Therefore, trying to enroll both, causes a collision error.
 *  Best practice: Avoid colons inside regular names.
 */

/// forward declarations
typedef struct st_s st_s;
st_s* st_s_create();

/** Enrolls name in global manager (thread safe); returns hash; no effect if name is already enrolled
 *  Checks for collisions; In case of collision, program aborts with message to stderr.
 *  For a more controlled collision check, you can use functions 'check_collision' below.
 *  A collision is extremely rare (none is known so far).
 */
tp_t bcore_name_enroll(                     sc_t name );             // enrolls name
tp_t bcore_name_enroll_n(                   sc_t name, uz_t n );     // enrolls first n characters of name
tp_t bcore_name_enroll_s(  tp_t name_space, sc_t name );             // enrolls name in a namespace
tp_t bcore_name_enroll_sn( tp_t name_space, sc_t name, uz_t n );     // enrolls first n characters of name in a namespace

/** Check for collision. Does not change state of name manager.
 *  Also checks for hashing to '0' or invalid name space.
 *  Returns 0 in case of no collision. Otherwise sends an error condition to the error manager.
 *  In case of no collision, enroll functions above can be used safely.
 */
er_t bcore_name_check_collision(                     sc_t name );         // checks name
er_t bcore_name_check_collision_n(                   sc_t name, uz_t n ); // checks first n characters of name
er_t bcore_name_check_collision_s(  tp_t name_space, sc_t name );         // checks name in a namespace
er_t bcore_name_check_collision_sn( tp_t name_space, sc_t name, uz_t n ); // checks first n characters of name in a namespace

/// hash --> name; returns NULL when not enrolled (thread safe)
sc_t bcore_name_try_name( tp_t type );
static inline bl_t bcore_name_exists( tp_t type ) { return bcore_name_try_name( type ) != NULL; }

/// hash --> name; returns error when not enrolled (thread safe)
sc_t bcore_name_get_name( tp_t type );

/// hash --> full name; returns NULL when not enrolled (thread safe); passes ownership
st_s* bcore_name_try_name_s( tp_t type );

/// hash --> full name; returns error when not enrolled (thread safe); passes ownership
st_s* bcore_name_get_name_s( tp_t type );

/// removes hash and associated name; no effect when not enrolled (thread safe)
void bcore_name_remove( tp_t type );

/// pushes all names to name_map (thread safe)
void bcore_name_push_all( bcore_hmap_name_s* name_map );

// TODO (johannes#1#): move away from name 'typeof' (e.g. use btypeof instead) ...
// It conflicts with a keyword in a gcc extension
#define typeof      btypeof // workaround for function typeof, which is a keyword in a gcc specific extension
#define entypeof    bentypeof
#define typeof_fv   btypeof_fv
#define typeof_fa   btypeof_fa
#define entypeof_fv bentypeof_fv
#define entypeof_fa bentypeof_fa
#define nameof      bnameof
#define ifnameof    bifnameof
#define nameof_s    bnameof
#define ifnameof_s  bifnameof

/// syntactic sugar
static inline tp_t btypeof(               sc_t name         ) { return bcore_name_key(          name    ); }
static inline tp_t btypeof_n(             sc_t name, uz_t n ) { return bcore_name_key_n(        name, n ); }
static inline tp_t btypeof_ns(   tp_t ns, sc_t name         ) { return bcore_name_key_ns(   ns, name    ); }
static inline tp_t btypeof_ns_n( tp_t ns, sc_t name, uz_t n ) { return bcore_name_key_ns_n( ns, name, n ); }

static inline tp_t bentypeof(               sc_t name         ) { return bcore_name_enroll(        name    ); }
static inline tp_t bentypeof_n(             sc_t name, uz_t n ) { return bcore_name_enroll_n(      name, n ); }
static inline tp_t bentypeof_ns(   tp_t ns, sc_t name         ) { return bcore_name_enroll_s(  ns, name    ); }
static inline tp_t bentypeof_ns_n( tp_t ns, sc_t name, uz_t n ) { return bcore_name_enroll_sn( ns, name, n ); }

// composes a string from arguments and computes key
tp_t btypeof_fv( sc_t format, va_list args );
tp_t btypeof_fa( sc_t format, ... );

tp_t bentypeof_fv( sc_t format, va_list args );
tp_t bentypeof_fa( sc_t format, ... );

static inline sc_t    bnameof(   tp_t type ) { return bcore_name_try_name( type ); } // returns NULL if not enrolled
static inline sc_t  bifnameof(   tp_t type ) { sc_t n = bcore_name_try_name( type ); return n ? n : ""; }
static inline st_s*   bnameof_s( tp_t type ) { return bcore_name_try_name_s( type ); }
static inline st_s* bifnameof_s( tp_t type ) { st_s* n = bcore_name_try_name_s( type ); return n ? n : st_s_create(); }

/// statistics
typedef struct st_s st_s;
sz_t  bcore_name_size(); // number of registered names
st_s* bcore_name_show(); // store all names in a string, separated by newline

bcore_name_map_s* bcore_name_create_name_map();

vd_t bcore_name_manager_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

// Macros
#define BCORE_REGISTER_NAME( name ) bcore_name_enroll( #name )

#endif // BCORE_NAME_MANAGER_H
