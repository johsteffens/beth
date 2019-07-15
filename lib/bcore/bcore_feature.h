/** Copyright 2017 Johannes Bernhard Steffens
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

#ifndef BCORE_FEATURE_H
#define BCORE_FEATURE_H

#include <stdarg.h>

#include "bcore_types.h"
#include "bcore_sr.h"

/**********************************************************************************************************************/

/// init, down, copy
typedef void (*bcore_fp_init )( vd_t o );           // initialization (construction)
typedef void (*bcore_fp_down )( vd_t o );           // destruction
typedef void (*bcore_fp_copy )( vd_t o, vc_t src ); // deep copy

/// create, discard, clone and move are derived from above features
typedef void (*bcore_fp_move      )( vd_t  o, vd_t src ); // init(o); copy(o, src); down(src)
typedef vd_t (*bcore_fp_create    )( void   );            // allocate & init
typedef void (*bcore_fp_discard   )( vd_t  o );           // down & deallocate; o may be NULL in which case function does nothing
typedef vd_t (*bcore_fp_clone     )( vc_t  o );           // allocate & copy; o may be NULL in which case NULL is returned
typedef void (*bcore_fp_detach    )( vd_t* o );           // if( o ) { discard( *o ); *o = NULL; }
typedef void (*bcore_fp_attach    )( vd_t* o, vd_t src ); // if( o ) { if( *o != src ) { discard( *o ); *o = src; } }
typedef void (*bcore_fp_replicate )( vd_t* o, vc_t src ); // if( o ) { if( *o != src ) { discard( *o ); *o = clone( src ); } }

/// copy, create from another object with type conversion
typedef void (*bcore_fp_copy_typed   )( vd_t o, tp_t type, vc_t src ); // deep conversion & copy
typedef vd_t (*bcore_fp_create_typed )(         tp_t type, vc_t src ); // deep conversion & create

/// Sanity check of an object
typedef void (*bcore_fp_check_sanity )( vc_t o );

/**********************************************************************************************************************/

/// element access
typedef sr_s (*bcore_fp_get )( vc_t o );
typedef void (*bcore_fp_set )( vd_t o, sr_s val );

/**********************************************************************************************************************/

/// formatted logging
typedef void (*bcore_fp_logvf )( vd_t o, sc_t format, va_list args );

/*********************************************************************************************************************/

/// comparison
typedef s2_t (*bcore_fp_cmp   )(         vc_t v1, vc_t v2 ); // comparison: ==0: equal; >0: v1 before v2; <0 :v1 after v2
typedef s2_t (*bcore_fp_cmp_o )( vc_t o, vc_t v1, vc_t v2 ); // comparison: ==0: equal; >0: v1 before v2; <0 :v1 after v2
typedef struct bcore_cmp_f { bcore_fp_cmp_o f; vc_t o; } bcore_cmp_f; // comparison functor

/**********************************************************************************************************************/

/// generator
typedef bl_t (*bcore_fp_gen_o )( vc_t o, uz_t idx, vd_t dst ); // generator (on valid index: sets *dst (if dst != NULL) and returns true)
typedef struct bcore_gen_f { bcore_fp_gen_o f; vc_t o; } bcore_gen_f; // generator functor

/**********************************************************************************************************************/

/// data flow
typedef uz_t (*bcore_fp_flow_snk )( vd_t o, vc_t data, uz_t size ); // flow-sink;   size in bytes; returns number of bytes transferred
typedef uz_t (*bcore_fp_flow_src )( vd_t o, vd_t data, uz_t size ); // flow_source; size in bytes; returns number of bytes transferred

/**********************************************************************************************************************/

/// marshaling (translator, interpreter objects)
typedef void ( *bcore_fp_translate )( vc_t o, sr_s obj, sr_s snk ); // translates object to be constructible via 'interpret_object'
typedef sr_s ( *bcore_fp_interpret )( vc_t o,           sr_s src ); // constructs object (no information given)

/**********************************************************************************************************************/

/// rudimentary signaling (the actual meaning of arguments and return type is defined by the usage context)
typedef struct bcore_signal_s bcore_signal_s;
typedef vd_t ( *bcore_fp_signal_handler )( const bcore_signal_s* o );

/**********************************************************************************************************************/

vd_t bcore_feature_signal_handler( const bcore_signal_s* o );

#endif // BCORE_FEATURE_H

