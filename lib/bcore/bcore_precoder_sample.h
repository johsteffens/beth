/** Copyright 2019 Johannes Bernhard Steffens
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

/** Testbed and sample for Precoder Framework */

#ifndef BCORE_PRECODER_SAMPLE_H
#define BCORE_PRECODER_SAMPLE_H

#include "bcore_std.h"

#include "bcore_precoded.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_precoder_sample
/** BETH_PRECODE is a c-macro as well as a precode identifier
 *  The BETH_PRECODE macro is resolved into the c-code of the precode.
 *  The macro argument is a unique identifier for the given precode section.
 *  The associated precode is defined by the commented section immediately
 *  following the macro.
 */
BETH_PRECODE( bcore_precoder_sample )
/*
self bcore_precoder_sample_object_s = bcore_inst
{
    aware_t _;  // precode single line comment (no nesting conflict with c-style)
    st_s name;
    sz_t => [] arr;
}

self bcore_precoder_sample_object2_s = bcore_inst
{
    aware_t _;  // precode single line comment (no nesting conflict with c-style)
    bcore_precoder_sample_object_s object;
    st_s name2;
}
*/

void bcore_precoder_sample_object_s_setup(           bcore_precoder_sample_object_s* o );
void bcore_precoder_sample_object_s_to_stdout( const bcore_precoder_sample_object_s* o );

#endif // TYPEOF_bcore_precoder_sample_identifier1

/**********************************************************************************************************************/

vd_t bcore_precoder_sample_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BCORE_PRECODER_SAMPLE_H
