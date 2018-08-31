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

#ifndef BMATH_MATRIX_MUL_H
#define BMATH_MATRIX_MUL_H

/**********************************************************************************************************************/

/** Collection of algorithms related to matrix multiplication.
 *  All routines have been designed from scratch and optimized with modern architectures in mind.
 *
 *  This header is included in bmath_mf3.h and should not be used in isolation
 *  See bmath_mf3.h for nomenclature.
 */

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_hwflags.h"
#include "bmath_spect_algebraic.h"
#include "bmath_vector.h"
#include "bmath_grt.h"

/**********************************************************************************************************************/
// dynamic size matrix of f3_t

typedef struct bmath_mf3_s bmath_mf3_s;

//---------------------------------------------------------------------------------------------------------------------

/// o * op --> res
void bmath_mf3_s_mul( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

/// o * op --> res (esp version)
void bmath_mf3_s_mul_esp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

/// o * op^T --> res
void bmath_mf3_s_mul_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

/// o * op^T --> res (esp version)
void bmath_mf3_s_mul_htp_esp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res );

/// o * d --> res (d: diagonal matrix; columns get scaled)
void bmath_mf3_s_mul_od( const bmath_mf3_s* o, const bmath_vf3_s* d, bmath_mf3_s* res );

/// d * o --> res (d: diagonal matrix; rows get scaled)
void bmath_mf3_s_mul_do( const bmath_vf3_s* d, const bmath_mf3_s* o, bmath_mf3_s* res );

///  u * d * u^T --> res (in place)
void bmath_mf3_s_mul_udu_htp( const bmath_mf3_s* u, const bmath_vf3_s* d, bmath_mf3_s* res );

///  u * d * u^T --> res (in place) (esp version)
void bmath_mf3_s_mul_udu_htp_esp( const bmath_mf3_s* u, const bmath_vf3_s* d, bmath_mf3_s* res );

///  u * d * v^T --> res (in place)
void bmath_mf3_s_mul_udv_htp( const bmath_mf3_s* u, const bmath_vf3_s* d, const bmath_mf3_s* v, bmath_mf3_s* res );

///  u * d * v^T --> res (in place) (esp version)
void bmath_mf3_s_mul_udv_htp_esp( const bmath_mf3_s* u, const bmath_vf3_s* d, const bmath_mf3_s* v, bmath_mf3_s* res );

//---------------------------------------------------------------------------------------------------------------------

#endif  // BMATH_MATRIX_MUL_H
