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

#ifndef BCORE_H
#define BCORE_H

#include "bcore_features.h"

/// Initializes library bcore only (need not be called in case bcore_register_signal_handler is used)
void bcore_init();

/// Registers signal handler and initializes targets
void bcore_register_signal_handler( bcore_fp_signal_handler signal_handler );

/// signal handler for all registered libraries (do not send init or down signals by this function)
vd_t bcore_global_signal_handler( const bcore_signal_s* signal );

/// runs selftest on target
vd_t bcore_run_signal_selftest( tp_t target, vd_t object );

/// Global closer for all libraries
void bcore_down( bl_t verbose );

#endif // BCORE_H
