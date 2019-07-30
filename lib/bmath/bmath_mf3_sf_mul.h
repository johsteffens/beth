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

#ifndef BMATH_MF3_SF_MUL_H
#define BMATH_MF3_SF_MUL_H

/**********************************************************************************************************************/

/** Collection of algorithms related to spliced-matrix multiplication.
 *  All routines have been designed from scratch and optimized with modern architectures in mind.
 *
 *  This header is included in bmath_mf3_sf.h and should not be used in isolation
 *  See bmath_mf3_sf.h for nomenclature.
 */

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_hwflags.h"
#include "bmath_spect_algebraic.h"
#include "bmath_vf3.h"
#include "bmath_grt.h"

/**********************************************************************************************************************/
BCORE_FORWARD_OBJECT( bmath_mf3_sf_s );

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_sf_s_mul(     const bmath_mf3_sf_s* o, const bmath_mf3_sf_s* m, bmath_mf3_sf_s* r ); // o * m -> r
void bmath_mf3_sf_s_mul_htp( const bmath_mf3_sf_s* o, const bmath_mf3_sf_s* m, bmath_mf3_sf_s* r ); // o * m^T -> r
void bmath_mf3_sf_s_htp_mul( const bmath_mf3_sf_s* o, const bmath_mf3_sf_s* m, bmath_mf3_sf_s* r ); // o^T * m -> r

//----------------------------------------------------------------------------------------------------------------------

#endif  // BMATH_MF3_SF_MUL_H
