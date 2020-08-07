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

#ifndef BCORE_TP_H
#define BCORE_TP_H

/// Low level functions around tp_t

#include <stdarg.h>
#include "bcore_control.h"

/**********************************************************************************************************************/
/// Hashing (non-cryptographic)

/** FNV-1a for 0-terminated strings and data blocks.
 *  Conceived by Glenn Fowler, Landon Curt Noll and Kiem-Phon Vo in 1991.
 *  References:
 *  http://www.isthe.com/chongo/tech/comp/fnv/index.html
 *  http://en.wikipedia.org/wiki/Fowler_Noll_Vo_hash
 */

/// deprecated ===========================================
#define FNV_U2_INIT 0x811c9dc5u
#define FNV_U2_FOLD 0x01000193u
static inline tp_t bcore_tp_init() { return FNV_U2_INIT; }
static inline tp_t bcore_tp_fold_u0( tp_t o, u0_t v ) { return ( ( o ^ v ) * FNV_U2_FOLD ) & 0x00000000FFFFFFFFull; }
/// deprecated ===========================================

//#define FNV_U3_INIT 0xcbf29ce484222325ull
//#define FNV_U3_FOLD 0x00000100000001B3ull
//
//static inline tp_t bcore_tp_init() { return FNV_U3_INIT; }
//static inline tp_t bcore_tp_fold_u0( tp_t o, u0_t v ) { return ( o ^ v ) * FNV_U3_FOLD; }

static inline tp_t bcore_tp_fold_u1( tp_t o, u1_t v ) { return bcore_tp_fold_u0( bcore_tp_fold_u0( o, v ), v >>  8 ); }
static inline tp_t bcore_tp_fold_u2( tp_t o, u2_t v ) { return bcore_tp_fold_u1( bcore_tp_fold_u1( o, v ), v >> 16 ); }
static inline tp_t bcore_tp_fold_u3( tp_t o, u3_t v ) { return bcore_tp_fold_u2( bcore_tp_fold_u2( o, v ), v >> 32 ); }
static inline tp_t bcore_tp_fold_tp( tp_t o, tp_t v ) { return bcore_tp_fold_u3( o, v ); }

static inline tp_t bcore_tp_fold_sc( tp_t o, sc_t v ) { if( !v ) return bcore_tp_fold_u0( o, 0 ); while( *v ) { o = bcore_tp_fold_u0( o, *v++ ); } return o; }
static inline tp_t bcore_tp_fold_vc( tp_t o, vc_t a, uz_t n ) { for( const u0_t* v = a; n > 0; n-- ) { o = bcore_tp_fold_u0( o, *v++ ); } return o; }
static inline tp_t bcore_tp_fold_uz( tp_t o, uz_t v ) { return bcore_tp_fold_vc( o, &v, sizeof( uz_t ) ); }
static inline tp_t bcore_tp_fold_sz( tp_t o, sz_t v ) { return bcore_tp_fold_vc( o, &v, sizeof( sz_t ) ); }

static inline tp_t bcore_tp_hash_sc( sc_t v ) { return bcore_tp_fold_sc( bcore_tp_init(), v ); }
static inline tp_t bcore_tp_hash_vc( vc_t a, uz_t n ) { return bcore_tp_fold_vc( bcore_tp_init(), a, n ); }

vd_t bcore_tp_signal_handler( const bcore_signal_s* o );

#endif //BCORE_TP_H
