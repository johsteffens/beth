/** Author and Copyright 2024 Johannes Bernhard Steffens
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

/** Beth comparison group.
 *
 *  The generic comparison is a comparison between two (generically ordered) objects a, b yielding 1, 0, or -1.
 *  (The generic order is imposed via the a object's reflection.)
 *
 *  1: a, b have positive order (can be considered as a < b)
 *  0: a, b are equal
 * -1: a, b have negative order (can be considered as a > b)
 *
 *  Type and numeric dominance:
 *  A type dominant comparison returns the order of types in case both types are different.
 *  A numeric dominant (num_dominant) comparison returns the order of values in case both types are numeric (including boolean types).
 *
 *  The dominance is a deep operation: Meaning it compares recursively all elements of an object with the same specified dominance.
 */

#ifndef BCORE_X_COMPARE_H
#define BCORE_X_COMPARE_H

#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_compare, bcore_inst )

#ifdef XOILA_SECTION

include 'h' "bcore_spect_compare.h";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// interface

/// generic bi-typed comparison with type dominance
signature s2_t t_type_dominant( tp_t ta, obliv x_inst* a, tp_t tb, obliv x_inst* b );
signature s2_t   type_dominant(          aware x_inst* a,          aware x_inst* b );

/// generic bi-typed comparison with numeric dominance
signature s2_t t_num_dominant( tp_t ta, obliv x_inst* a, tp_t tb, obliv x_inst* b );
signature s2_t   num_dominant(          aware x_inst* a,          aware x_inst* b );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// implementations

//----------------------------------------------------------------------------------------------------------------------

func t_type_dominant = ( ta == tb ) ? bcore_compare_typed( ta, a, b ) : ( ta < tb ) ? 1 : -1;
func   type_dominant = :t_type_dominant( a ? a._ : 0, a, b ? b._ : 0, b );
func t_num_dominant = bcore_compare_num_dominant_sr( sr_twc( ta, a ), sr_twc( tb, b ) );
func   num_dominant = :t_num_dominant( a ? a._ : 0, a, b ? b._ : 0, b );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_COMPARE_H
