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

#ifndef BCORE_SIGNAL_H
#define BCORE_SIGNAL_H

/** Statically bound rudimentary runtime control of library items via signaling.
 */

#include "bcore_feature.h"
#include "bcore_quicktypes.h"

typedef struct bcore_signal_s
{
    tp_t target;
    tp_t type;
    vd_t object;
} bcore_signal_s;

void bcore_signal_s_init( bcore_signal_s* o );
void bcore_signal_s_down( bcore_signal_s* o );
void bcore_signal_s_copy( bcore_signal_s* o, const bcore_signal_s* src );
bcore_signal_s* bcore_signal_s_create();
void            bcore_signal_s_discard( bcore_signal_s* o);
bcore_signal_s* bcore_signal_s_clone( const bcore_signal_s* o );

bcore_signal_s bcore_signal_init( tp_t target, tp_t type, vd_t object );

/// returns type for switching statement in case target matches; returns TYPEOF_none otherwise;
tp_t bcore_signal_s_handle_type( const bcore_signal_s* o, tp_t target );

/**********************************************************************************************************************/

/// broadcasts a signal to specified signal handlers
vd_t bcore_signal_s_broadcast( const bcore_signal_s* o, bcore_fp_signal_handler* arr, uz_t size );

/// signal handler hub for this library
vd_t bcore_signal_handler( const bcore_signal_s* o );
static inline vd_t bcore_general_signal_handler( const bcore_signal_s* o ) { return bcore_signal_handler( o ); }


#endif  // BCORE_SIGNAL_H
