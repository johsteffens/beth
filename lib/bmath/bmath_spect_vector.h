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

#ifndef BMATH_SPECT_VECTOR_H
#define BMATH_SPECT_VECTOR_H

/// A vector is defined by a ring-type and an array.

#include "bmath_spect_algebraic.h"
#include "bcore_spect_array.h"

/**********************************************************************************************************************/

// bmath_ring_s
typedef struct bmath_vector_s
{
    bcore_spect_header_s header;
    const bmath_ring_s*  ring;  // ring of vector element
    const bcore_array_s* array; // array of vector
} bmath_vector_s;

const bmath_vector_s* bmath_vector_s_get_typed( tp_t type );
const bmath_vector_s* bmath_vector_s_get_aware( vc_t o );

sz_t bmath_vector_spect_get_dim( const bmath_vector_s* p, vc_t o );
void bmath_vector_spect_add(     const bmath_vector_s* p, vc_t o, vc_t op, vd_t result );
void bmath_vector_spect_sub(     const bmath_vector_s* p, vc_t o, vc_t op, vd_t result );
void bmath_vector_spect_dot_prd( const bmath_vector_s* p, vc_t o, vc_t op, vd_t result );

/**********************************************************************************************************************/

vd_t bmath_spect_vector_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_SPECT_VECTOR_H

