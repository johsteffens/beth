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

#ifndef BCLOS_SIGNAL_H
#define BCLOS_SIGNAL_H

/** Statically bound rudimentary runtime control of library items via signaling.
 */

#include "bcore_feature.h"

/// broadcasts a signal within the library
vd_t bclos_general_signal_handler( const bcore_signal_s* o );

#endif  // BCLOS_SIGNAL_H
