/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#ifndef BCORE_SIGNAL_H
#define BCORE_SIGNAL_H

/** Statically bound rudimentary runtime control of library items via signaling.
 */

#include "bcore_features.h"

/// broadcasts a signal to this library and all dependents
vd_t bcore_signal( tp_t target, tp_t signal, vd_t object );

#endif  // BCORE_SIGNAL_H
