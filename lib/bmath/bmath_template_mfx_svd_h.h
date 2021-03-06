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

/** Collection of algorithms related to singular value decomposition (SVD).
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

/** Stable singular value decomposition for a general mxn-matrix a -> a'.
 *  Method: Bi-diagonalization by givens rotations and QR-chasing with stabilized implicit shift.
 *          (Variant of Golub-Reinsch-Algorithm)
 *
 *      * Supports any n,m configuration
 *      * Supports full and thin decomposition.
 *      * Automatically detects and exploits sparsity
 *
 *  It is a = u * a' * vT, with u, v being unitary.
 *  If matrices u, v are desired, their size has to be preset but they need not be preinitialized.
 *  The routine does not change the actual allocation of either matrix, hence u, a, v may
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
 *     v:
 *         NULL         -  v is not computed
 *         nxn matrix   -  full decomposition
 *         nxm matrix   -  thin decomposition for m < n
 *
 *  If thin decomposition is chosen, then a' is returned as (thin) square matrix.
 *  Meaning either a->rows or a->cols is changed to min(m,n) by the routine.
 *
 *  Returns false in case convergence could not be achieved.
 */
bl_t BCATU(bmath_mfx_s,svd)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v );

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

