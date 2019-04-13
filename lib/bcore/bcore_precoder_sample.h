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

/** EMPLACE_OBJECT_... is a macro defining the object in c-code
 *  It defines the structure bcore_precoder_sample_object_s,
 *  and related functions.
 *  The macro is defined in bcore_autofile.h.
 *  Code emplacement happens where the structure would naturally be defined in
 *  pure C-code. We cannot emplace in precoded.h because precoder is not aware
 *  of compile-time dependencies.
 */
#ifdef EMPLACE_OBJECT_bcore_precoder_sample_object_s
       EMPLACE_OBJECT_bcore_precoder_sample_object_s;

/// beth-precode is inserted in regular files by directive #PRECODE
/// immediately following a C-comment intro '/*', '/**'
/// The precode section ends with the end of the C-comment section
/*#PRECODE
self bcore_precoder_sample_object_s = bcore_inst
{
    aware_t _;  // precode single line comment (no nesting conflict with c-style)
    st_s name;
    sz_t => [] arr;
}
*/
void bcore_precoder_sample_object_s_setup(           bcore_precoder_sample_object_s* o );
void bcore_precoder_sample_object_s_to_stdout( const bcore_precoder_sample_object_s* o );

#endif // EMPLACE_OBJECT_bcore_precoder_sample_object_s

#ifdef BETH_PRECODE_bcore_precoder_sample_object2_s
/** BETH_PRECODE is a c-macro as well as a precode specifier
 *  Precode follows in a commented section.
 *  The BETH_PRECODE macro is resolved into the c-code of the precode.
 *  The macro argument is a unique identifier for the given precode section.
 */
BETH_PRECODE( bcore_precoder_sample_identifier1 );
/*
feature bl_t bcore_precoder_sample_fp_feature1( vd_t o, sz_t size );

self bcore_precoder_sample_object2_s = bcore_inst
{
    aware_t _;  // precode single line comment (no nesting conflict with c-style)
    st_s name;
    sz_t => [] arr;
}

self bcore_precoder_sample_object3_s = bcore_inst
{
    aware_t _;  // precode single line comment (no nesting conflict with c-style)
    bcore_precoder_sample_object2_s object;
    st_s name;
}
*/

void bcore_precoder_sample_object2_s_setup(           bcore_precoder_sample_object2_s* o );
void bcore_precoder_sample_object2_s_to_stdout( const bcore_precoder_sample_object2_s* o );

#endif // BETH_PRECODE_bcore_precoder_sample_object2_s

// identical redefinition is allowed and has no effect; a different body would produce an error
/**#PRECODE self bcore_precoder_sample_object_s = bcore_inst { aware_t _; st_s name; sz_t => [] arr; }*/

/**#PRECODE self bcore_precoder_another_sample_object_s = bcore_inst { aware_t _; u2_t val1; u2_t val2; }*/
EMPLACE_OBJECT_bcore_precoder_another_sample_object_s;

/**********************************************************************************************************************/

vd_t bcore_precoder_sample_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BCORE_PRECODER_SAMPLE_H
