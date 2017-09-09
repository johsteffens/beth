/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCLOS_SIGNAL_H
#define BCLOS_SIGNAL_H

/** Statically bound rudimentary runtime control of library items via signaling.
 */

#include "bcore_features.h"

/// broadcasts a signal within the library
vd_t bclos_signal( tp_t target, tp_t signal, vd_t object );

#endif  // BCLOS_SIGNAL_H
