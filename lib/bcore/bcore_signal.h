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

#ifndef BCORE_SIGNAL_H
#define BCORE_SIGNAL_H

/** Statically bound rudimentary runtime control of library items via signaling.
 */

#include "bcore_features.h"

/// broadcasts a signal to specified signal handlers
vd_t bcore_signal_broadcast( bcore_fp_signal* arr, sz_t size, tp_t target, tp_t signal, vd_t object );

/// broadcasts a signal to this library and all dependents
vd_t bcore_signal( tp_t target, tp_t signal, vd_t object );

#endif  // BCORE_SIGNAL_H
