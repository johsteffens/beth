/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_SIGNATURE_H
#define BCORE_SIGNATURE_H

/** Signature management framework
 *  A signature is a list of types.
 *  This framework links signatures to hash values and vice versa.
 *  Hash value 0 is reserved and can be associated with the empty signature;
 *  The signature manager rejects signatures resulting in hash 0.
 *  The hash of a signature with one element is identical to that element.
 */

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_control.h"
#include "bcore_flect.h"

#include <stdarg.h>

typedef struct bcore_signature_s
{
    aware_t _;
    union
    {
        bcore_static_array_s arr;
        struct
        {
            tp_t* data;
            sz_t size, space;
        };
    };
} bcore_signature_s;

DECLARE_FUNCTION_INIT(    bcore_signature_s )
DECLARE_FUNCTION_DOWN(    bcore_signature_s )
DECLARE_FUNCTION_COPY(    bcore_signature_s )
DECLARE_FUNCTION_CREATE(  bcore_signature_s )
DECLARE_FUNCTION_DISCARD( bcore_signature_s )
DECLARE_FUNCTION_CLONE(   bcore_signature_s )

bcore_signature_s* bcore_signature_s_create_vn( sz_t n, va_list args ); // creates signature from n types
bcore_signature_s* bcore_signature_s_create_an( sz_t n, ... );          // creates signature from n types

void bcore_signature_s_clear(   bcore_signature_s* o );
void bcore_signature_s_push(    bcore_signature_s* o, tp_t type );
void bcore_signature_s_push_nv( bcore_signature_s* o, sz_t n, va_list args );
void bcore_signature_s_push_na( bcore_signature_s* o, sz_t n, ... );
tp_t bcore_signature_s_pop(     bcore_signature_s* o ); // returns 0 when signature is empty
void bcore_signature_s_pop_n(   bcore_signature_s* o, sz_t n );

/// Converts signature into a string of associated type names
st_s* bcore_signature_s_create_string( const bcore_signature_s* o );

/** Compares two signatures introducing an ordering:
 *  Returns   0 when both signatures are equal
 *  Returns  +1 when sig2 is longer than sig1 but at the length of sig1 all types match
 *  Returns  -1 when sig1 is longer than sig2 but at the length of sig2 all types match
 *  Returns >+1 when none of above conditions apply and sig2 is ordered after sig1
 *  Returns <-1 when none of above conditions apply and sig1 is ordered after sig2
 */
u2_t bcore_signature_s_cmp( const bcore_signature_s* sig1, const bcore_signature_s* sig2 );

bool bcore_signature_s_equal( const bcore_signature_s* sig1, const bcore_signature_s* sig2 );

tp_t bcore_signature_s_get_hash( const bcore_signature_s* o ); // converts signature to type
tp_t bcore_signature_get_hash_nv(  sz_t n, va_list args    );  // converts signature given by argument list to type
tp_t bcore_signature_get_hash_na(  sz_t n, ...             );  // converts signature given by argument list to type
tp_t bcore_signature_get_hash_arr(             sz_t n, const tp_t* arr );  // converts signature given by argument list to type
tp_t bcore_signature_fold_hash_arr( tp_t hash, sz_t n, const tp_t* arr );  // extends hash via folding
static inline tp_t bcore_signature_get_hash_tp_tp( tp_t t1, tp_t t2 ) { return bcore_fnv_fold_u2_u2( t1, t2 ); } // returns hash of two types

/**********************************************************************************************************************/

vd_t bcore_signature_signal( tp_t target, tp_t signal, vd_t object );

/**********************************************************************************************************************/

#endif // BCORE_SIGNATURE_H
