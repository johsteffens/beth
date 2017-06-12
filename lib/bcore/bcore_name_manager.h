/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** Name registration framework
 *  This framework links names to hash values and vice versa.
 *  Hash value 0 is reserved. Names resulting is hash 0 are rejected.
 */

#ifndef BCORE_NAME_MANAGER_H
#define BCORE_NAME_MANAGER_H

#include "bcore_control.h"

/// enroll name in global manager (thread safe); checks for collisions; returns hash
tp_t bcore_name_enroll( sc_t name );

/// closes name manager and frees all memory
void bcore_name_manager_close();

/// name --> hash
static inline tp_t bcore_name_get_type( sc_t name ) { return bcore_fnv_hash_text_u2( name ); }

/// hash --> name; returns NULL when not enrolled (thread safe)
sc_t bcore_name_try_name( tp_t type );

/// hash --> name; returns error when not enrolled (thread safe)
sc_t bcore_name_get_name( tp_t type );

/// syntactic sugar
static inline u2_t typeof(   sc_t name ) { return bcore_name_get_type( name ); }
static inline sc_t nameof(   u2_t type ) { return bcore_name_get_name( type ); }
static inline sc_t ifnameof( u2_t type ) { sc_t n = bcore_name_try_name( type ); return n ? n : ""; }

void bcore_name_print_predefined_list( tp_t (*hash)( sc_t name ) );

/** Predefined types of frequently used objects for given hash function.
 *  Run bcore_name_print_predefined_list( new_hash ) in case the hash function changes.
 */
#define BCORE_TYPEOF_s3_t 0x6835cf40
#define BCORE_TYPEOF_s2_t 0x0d0e488b
#define BCORE_TYPEOF_s1_t 0x44a88362
#define BCORE_TYPEOF_s0_t 0xeabe312d
#define BCORE_TYPEOF_u3_t 0x2583bb32
#define BCORE_TYPEOF_u2_t 0xcafb983d
#define BCORE_TYPEOF_u1_t 0x46969e10
#define BCORE_TYPEOF_u0_t 0xec0e7b1b
#define BCORE_TYPEOF_f3_t 0xd95df3e5
#define BCORE_TYPEOF_f2_t 0x5485acfa
#define BCORE_TYPEOF_sz_t 0x9cdbf133
#define BCORE_TYPEOF_sd_t 0xf9ae9579
#define BCORE_TYPEOF_sc_t 0xd328b750
#define BCORE_TYPEOF_vd_t 0x607aa784
#define BCORE_TYPEOF_vc_t 0x10e046e5
#define BCORE_TYPEOF_fp_t 0xaf449918
#define BCORE_TYPEOF_bcore_string_s 0x9035dee8

#endif // BCORE_NAME_MANAGER_H
