/** Author and Copyright 2018 Johannes Bernhard Steffens
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

/** This perspective can compute a hash value from an object instance.
 *  It uses spect_via to access elements on a non-leaf object.
 *  For common leaf objects the perspective defines the hash function.
 */

#ifndef BCORE_SPECT_HASH_H
#define BCORE_SPECT_HASH_H

#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_feature.h"
#include "bcore_spect_via.h"

/**********************************************************************************************************************/

BCORE_DECLARE_SPECT( bcore_hash )
{
    bcore_spect_header_s header;
    const bcore_via_s* spect_via;
};

/// Computes folded hash of type tp_t from object.
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_hash, fold_tp, tp_t, tp_t, hash )

/** Computes explicit hash of type tp_t from object.
 *  For string types (st_s, sc_t, sd_t) the returned type is the same as typeof( <string> ).
 */
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_hash, get_tp,  tp_t )

/**********************************************************************************************************************/

vd_t bcore_spect_hash_signal_handler( const bcore_signal_s* o );

#endif // BCORE_SPECT_HASH_H

