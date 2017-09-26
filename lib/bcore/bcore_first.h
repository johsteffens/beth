/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_FIRST_H
#define BCORE_FIRST_H

/// This header should be included first of all other headers

/** Runtime checks are enabled by flag RTCHECKS which should
 *  preferably enabled by the build environment in DEBUG modes.
 */
#ifndef RTCHECKS
    #ifndef NDEBUG
        #define NDEBUG
    #endif
#endif

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#endif // BCORE_FIRST_H
