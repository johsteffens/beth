/** Copyright 2018 Johannes Bernhard Steffens
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

/** Fast hash-map of (already hashed) tp keys.
 *  Values are of type vc_t.
 *  Intended for fast get-lock-free mapping (e.g. perspective caching).
 *
 *  Get- & Set-functions are concurrent.
 *  Get function is lock-free.
 *
 *  Inspired by Dr. Cliff Click's work on lock-free hash tables:
 *  https://web.stanford.edu/class/ee380/Abstracts/070221_LockFreeHash.pdf.
 *  This solution is somewhat different (less sophisticated, possibly faster), though,
 *  because the set-function has a lock and following simplifications apply:
 *  * The key is assumed to already represent a hash value.
 *  * The value for a given key never changes.
 *  * Keys are never removed.
 */

#ifndef BCORE_TP_FASTMAP_H
#define BCORE_TP_FASTMAP_H

#include "bcore_features.h"
#include "bcore_st.h"
#include "bcore_flect.h"
#include "bcore_signal.h"

typedef struct bcore_tp_fastmap_s bcore_tp_fastmap_s;
BCORE_DECLARE_OBJECT( bcore_tp_fastmap_s )

vc_t bcore_tp_fastmap_s_get( const bcore_tp_fastmap_s* o, tp_t key );
bl_t bcore_tp_fastmap_s_set( bcore_tp_fastmap_s* o, tp_t key, vc_t val );

/// statistics (thread safe)
sz_t bcore_tp_fastmap_s_size( bcore_tp_fastmap_s* o );
sz_t bcore_tp_fastmap_s_keys( bcore_tp_fastmap_s* o );

/**********************************************************************************************************************/

vd_t bcore_tp_fastmap_signal_handler( const bcore_signal_s* o );

#endif // BCORE_TP_FASTMAP_H

