/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_H
#define BCORE_H

#include "bcore_features.h"

/// Global (once) initializer for library bcore
void bcore_library_init( bcore_fp_signal signal );

/// Global closer for library bcore
void bcore_library_down( bl_t verbose );

#endif // BCORE_H
