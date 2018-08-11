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

#ifndef BMATH_MATRIX_QRD_H
#define BMATH_MATRIX_QRD_H

/**********************************************************************************************************************/

/** Collection of algorithms related to QR and LQ decomposition.
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 *
 *  This header is included in bmath_mf3.h and should not be used in isolation
 *  See bmath_mf3.h for nomenclature.
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

/** Stable QR decomposition for a general mxn-matrix a -> a'. Based on givens rotations.
 *  It is a = u * a', with u being unitary.
 *      * Supports any n,m configuration
 *      * Supports full and thin decomposition.
 *      * Highly efficient on dense and sparse matrices. (Automatically detects and exploits sparsity)
 *
 *  If matrix u is desired, its size has to be preset but it need not be preinitialized.
 *  The routine does not change the actual allocation of either matrix, hence u, a may
 *  safely reference external data.
 *  Whether full or thin decomposition is computed depends on how matrices have been preset:
 *
 *     a:  mxn input matrix to be diagonalized
 *
 *     u:
 *         NULL         -  u is not computed
 *         mxm matrix   -  full decomposition
 *         mxn matrix   -  thin decomposition for n < m
 *
 *  If thin decomposition is chosen, then a' is returned as (thin) nxn square matrix.
 *
 */
void bmath_mf3_s_qrd( bmath_mf3_s* u, bmath_mf3_s* a );

//---------------------------------------------------------------------------------------------------------------------

/** Stable rank revealing QRP decomposition for a general mxn-matrix a -> a'. Based on givens rotations.
 *  It is a = u * a' * p^T, with u being unitary.
 *  Diagonal elements of a' are non-negative and sorted in descending manner.
 *      * Supports any n,m configuration
 *      * Supports full and thin decomposition.
 *      * Highly efficient on dense and sparse matrices. (Automatically detects and exploits sparsity)
 *
 *  If matrix u is desired, its size has to be preset but it need not be preinitialized.
 *  The routine does not change the actual allocation of either matrix, hence u, a may
 *  safely reference external data.
 *  Whether full or thin decomposition is computed depends on how matrices have been preset:
 *
 *     a:  mxn input matrix to be diagonalized
 *
 *     u:
 *         NULL         -  u is not computed
 *         mxm matrix   -  full decomposition
 *         mxn matrix   -  thin decomposition for n < m
 *
 *     p:
 *         NULL         -  p is not computed
 *         n-permutation
 *
 *  If thin decomposition is chosen, then a' is returned as (thin) nxn square matrix.
 *
 */
void bmath_mf3_s_qrd_pmt( bmath_mf3_s* u, bmath_mf3_s* a, bmath_pmt_s* p );

//---------------------------------------------------------------------------------------------------------------------

/** Stable LQ decomposition for a general mxn-matrix a -> a'. Based on givens rotations.
 *  It is a = a' * v^T, with v being unitary.
 *      * Supports any n,m configuration
 *      * Supports full and thin decomposition.
 *      * Highly efficient on dense and sparse matrices. (Automatically detects and exploits sparsity)
 *
 *  If matrix v is desired, its size has to be preset but it need not be preinitialized.
 *  The routine does not change the actual allocation of either matrix, hence v, a may
 *  safely reference external data.
 *  Whether full or thin decomposition is computed depends on how matrices have been preset:
 *
 *     a:  mxn input matrix to be diagonalized
 *
 *     v:
 *         NULL         -  v is not computed
 *         nxn matrix   -  full decomposition
 *         nxm matrix   -  thin decomposition for m < n
 *
 *  If thin decomposition is chosen, then a' is returned as (thin) mxm square matrix.
 *
 */
void bmath_mf3_s_lqd( bmath_mf3_s* a, bmath_mf3_s* v );

//---------------------------------------------------------------------------------------------------------------------

#endif  // BMATH_MATRIX_QRD_H
