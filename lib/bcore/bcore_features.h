/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_FEATURES_H
#define BCORE_FEATURES_H

#include <stdarg.h>

#include "bcore_types.h"

/**********************************************************************************************************************/

/// create, copy, discard
typedef void (*bcore_fp_init   )( vd_t o );           // initialization (construction)
typedef void (*bcore_fp_down   )( vd_t o );           // destruction
typedef void (*bcore_fp_copy   )( vd_t o, vc_t src ); // deep copy
typedef void (*bcore_fp_plant  )( vd_t o, vc_t src ); // init(o); copy(o, src);
typedef void (*bcore_fp_move   )( vd_t o, vd_t src ); // init(o); copy(o, src); down(src)
typedef vd_t (*bcore_fp_create )( void   );           // allocate & init
typedef void (*bcore_fp_discard)( vd_t o );           // down & deallocate; o may be NULL in which case function does nothing
typedef vd_t (*bcore_fp_clone  )( vc_t o );           // allocate & copy; o may be NULL in which case NULL is returned

/// copy, create from another object with type conversion
typedef void (*bcore_fp_copy_typed   )( vd_t o, tp_t type, vc_t src ); // deep conversion & copy
typedef vd_t (*bcore_fp_create_typed )(         tp_t type, vc_t src ); // deep conversion & create

/// Sanity check of an object
typedef void (*bcore_fp_check_sanity )( vc_t o );

/**********************************************************************************************************************/

/// formated logging
typedef void (*bcore_fp_logvf )( vd_t o, sc_t format, va_list args );

/**********************************************************************************************************************/

/// bifunctions
typedef s2_t (*bcore_fp_cmp  )( vc_t v1,  vc_t v2  ); // comparison: ==0: equal; >0: v1 before v2; <0 :v1 after v2
typedef vd_t (*bcore_fp_fold )( vd_t acc, vc_t arg ); // folding

/**********************************************************************************************************************/

/// data flow
typedef sz_t (*bcore_fp_flow_snk )( vd_t o, vc_t data, sz_t size ); // flow-sink;   size in bytes; returns number of bytes transferred
typedef sz_t (*bcore_fp_flow_src )( vd_t o, vd_t data, sz_t size ); // flow_source; size in bytes; returns number of bytes transferred

/**********************************************************************************************************************/

/// marshaling, building, injection
typedef void ( *bcore_fp_translate_object )( vc_t o, tp_t type, vc_t obj, vd_t fsnk ); // translates object to be constructible via 'interpret_object'
typedef void ( *bcore_fp_translate_body   )( vc_t o, tp_t type, vc_t obj, vd_t fsnk ); // translates object to be constructible via 'interpret_body'
typedef dt_p ( *bcore_fp_interpret_object )( vc_t o, vd_t fsrc );                      // constructs object (no information given)
typedef dt_p ( *bcore_fp_interpret_typed  )( vc_t o, vd_t fsrc, tp_t type );           // constructs object of given type (possibly via type conversion)
typedef dt_p ( *bcore_fp_interpret_body   )( vc_t o, vd_t fsrc, tp_t type, vd_t obj ); // injects into object instance of given type

/**********************************************************************************************************************/
/// Testing, Debugging. A test shall either complete safely or fail with a descriptive error (via bcore_err).

typedef struct bcore_string_s bcore_string_s; // forward declaration

/// Silent and fast ( <10ms )
typedef void (*bcore_fp_quicktest)(void);

/** A selftest may be extensive and verbose but should complete in reasonable time (seconds).
 *  Selftest may return a log-string or NULL. The caller takes ownership of the returned string.
 */
typedef bcore_string_s* (*bcore_fp_selftest)(void);

#endif // BCORE_FEATURES_H

