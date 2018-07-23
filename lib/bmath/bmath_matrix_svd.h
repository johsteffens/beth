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

#ifndef BMATH_MATRIX_SVD_H
#define BMATH_MATRIX_SVD_H

/**********************************************************************************************************************/

/** Collection of algorithms related to singular value decomposition (SVD).
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 *
 *  This header is included in bmath_matrix.h and should not be used in isolation
 *  See bmath_matrix.h for nomenclature.
 */

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_spect_algebraic.h"
#include "bmath_vector.h"
#include "bmath_grt.h"

/**********************************************************************************************************************/
// dynamic size matrix of f3_t

typedef struct bmath_mf3_s bmath_mf3_s;

//---------------------------------------------------------------------------------------------------------------------

/** Stable in-place full SVD for a general matrix.
 *  Method: Bi-diagonalization by givens rotations and QR-chasing with implicit shift.
 *          (Variant of Golub-Reinsch-Algorithm)
 *
 *  Matrices u, a, v are being modified: mat -> mat'
 *  Input:  u  (nxn unitary or NULL), a  (nxm, any data), v  (nxn unitary or NULL),
 *  Output: u' (nxn unitary or NULL), a' (diagonal),      v' (nxn unitary or NULL)
 *  It is uT * a * v = u'T * a' * v'
 *
 *  Returns 'true' on successful convergence, 'false' otherwise with a' likely not being diagonal.
 *  (Convergence failure is very rare.)
 *
 *  On success:
 *    - a' is fully diagonal
 *    - diagonal elements are non-negative
 *    - diagonal elements are sorted in descending value order
 *    - Det(u') == 1
 *    - Det(v') == 1 or -1
 */
bl_t bmath_mf3_s_svd_htp( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v ); // for all matrices

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_svd_selftest();

#endif  // BMATH_MATRIX_SVD_H
