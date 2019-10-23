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

/**********************************************************************************************************************/

/** Collection of algorithms related to eigenvalue decomposition (EVD).
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

/** Stable in-place EVD for a symmetric matrix. Jacobi Method.
 *  Input:  a  (symmetric), v  (preallocated or NULL)
 *  Output: a' (diagonal),  v' (rotation) with a = v'T * a' * v'.
 *  Returns 'true' on successful convergence, 'false' otherwise with a' likely not being diagonal.
 *  (Convergence failure is very rare.)
 *  Diagonal elements are sorted in descending value order.
 *  v == NULL allowed, in which case only a' is computed.
 */
bl_t BCATU(bmath_mfx_s,evd_htp_jacobi)( bmath_mfx_s* a, bmath_mfx_s* v );

/** In-place EVD for a symmetric matrix.
 *  Approach: TRD, QR with explicit shifting. (Variant of Francis' QR-Algorithm)
 *  Input:  a  (symmetric), v  (preallocated or NULL)
 *  Output: a' (diagonal),  v' (rotation) with a = v'T * a' * v'.
 *  Returns 'true' on successful convergence, 'false' otherwise with a' likely not being diagonal.
 *  (Convergence failure is very rare.)
 *  Diagonal elements are sorted in descending value order.
 *  v == NULL allowed, in which case only a' is computed.
 */
bl_t BCATU(bmath_mfx_s,evd_htp)( bmath_mfx_s* a, bmath_mfx_s* v );

/**********************************************************************************************************************/
// closing template

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

