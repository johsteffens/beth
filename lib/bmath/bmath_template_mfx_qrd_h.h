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

/** Collection of algorithms related to QR and LQ decomposition.
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Stable QR decomposition for a general mxn-matrix a -> a'. Based on givens rotations.
 *  It is a = u * a', with a' = upper triangular and u = unitary.
 *      * Supports any n,m configuration
 *      * Supports full and thin decomposition.
 *      * Highly efficient on dense and sparse matrices. (Automatically detects and exploits sparsity)
 *
 *  If matrix u is desired, its size has to be preset but it need not be preinitialized.
 *  The routine does not change the actual allocation of either matrix, hence u, a may
 *  safely reference external data.
 *  Whether full or thin decomposition is computed depends on how matrices have been preset:
 *
 *     a:  mxn input matrix to be triangonalized
 *
 *     u:
 *         NULL         -  u is not computed
 *         mxm matrix   -  full decomposition
 *         mxn matrix   -  thin decomposition for n < m
 *
 *  If thin decomposition is chosen, then a' is returned as (thin) nxn square matrix.
 *
 */
void BCATU(bmath_mfx_s,qrd)( bmath_mfx_s* u, bmath_mfx_s* a );

//----------------------------------------------------------------------------------------------------------------------

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
 *     a:  mxn input matrix to be triangonalized
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
void BCATU(bmath_mfx_s,qrd_pmt)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_pmt_s* p );

//----------------------------------------------------------------------------------------------------------------------

/** Stable LQ decomposition for a general mxn-matrix a -> a'. Based on givens rotations.
 *  It is a = a' * v^T, with a' = lower triangular and v = unitary.
 *      * Supports any n,m configuration
 *      * Supports full and thin decomposition.
 *      * Highly efficient on dense and sparse matrices. (Automatically detects and exploits sparsity)
 *
 *  If matrix v is desired, its size has to be preset but it need not be preinitialized.
 *  The routine does not change the actual allocation of either matrix, hence v, a may
 *  safely reference external data.
 *  Whether full or thin decomposition is computed depends on how matrices have been preset:
 *
 *     a:  mxn input matrix to be triangonalized
 *
 *     v:
 *         NULL         -  v is not computed
 *         nxn matrix   -  full decomposition
 *         nxm matrix   -  thin decomposition for m < n
 *
 *  If thin decomposition is chosen, then a' is returned as (thin) mxm square matrix.
 *
 */
void BCATU(bmath_mfx_s,lqd)( bmath_mfx_s* a, bmath_mfx_s* v );

//----------------------------------------------------------------------------------------------------------------------

/** Stable rank revealing PLQ decomposition for a general mxn-matrix a -> a'. Based on givens rotations.
 *  It is a = p * a' * v^T, with v being unitary.
 *  Diagonal elements of a' are non-negative and sorted in descending manner.
 *      * Supports any n,m configuration
 *      * Supports full and thin decomposition.
 *      * Efficient on dense and sparse matrices. (Automatically detects and exploits sparsity)
 *
 *  If matrix v is desired, its size has to be preset but it need not be preinitialized.
 *  The routine does not change the actual allocation of either matrix, hence a, v may
 *  safely reference external data.
 *  Whether full or thin decomposition is computed depends on how matrices have been preset:
 *
 *     a:  mxn input matrix to be triangonalized
 *
 *     v:
 *         NULL         -  v is not computed
 *         nxn matrix   -  full decomposition
 *         nxm matrix   -  thin decomposition for m < n
 *
 *     p:
 *         NULL         -  p is not computed
 *         m-permutation
 *
 *  If thin decomposition is chosen, then a' is returned as (thin) mxm square matrix.
 *
 */
void BCATU(bmath_mfx_s,pmt_lqd)( bmath_pmt_s* p, bmath_mfx_s* a, bmath_mfx_s* v );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

