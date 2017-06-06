/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_FEATURES_H
#define BCORE_FEATURES_H

#include "bcore_types.h"

/// Creation, copying, relocation
typedef void  (*bcore_fp_init   )( void* o );                  // initialization (construction)
typedef void  (*bcore_fp_down   )( void* o );                  // destruction
typedef void  (*bcore_fp_copy   )( void* o, const void* src ); // deep copy
typedef void  (*bcore_fp_plant  )( void* o, const void* src ); // init(o); copy(o, src);
typedef void  (*bcore_fp_move   )( void* o,       void* src ); // init(o); copy(o, src); down(src)
typedef void* (*bcore_fp_create )(         );                  // allocate & init
typedef void  (*bcore_fp_discard)( void* o );                  // down & deallocate; o may be NULL in which case function does nothing
typedef void* (*bcore_fp_clone  )( const void* o );            // allocate & copy; o may be NULL in which case NULL is returned

/**********************************************************************************************************************/
/// Testing. A test shall either complete safely or fail with a descriptive error (via bcore_err).

typedef struct bcore_string_s bcore_string_s; // forward declaration

/// Silent and fast ( <10ms )
typedef void (*bcore_fp_quicktest)(void);

/** A selftest may be extensive and verbose but should complete in reasonable time (seconds).
 *  Selftest may return a log-string or NULL. The caller takes ownership of the returned string.
 */
typedef bcore_string_s* (*bcore_fp_selftest)(void);

#endif // BCORE_FEATURES_H

