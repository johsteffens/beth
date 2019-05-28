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

#ifndef BMATH_MF3_SX_H
#define BMATH_MF3_SX_H

/**********************************************************************************************************************/

/** Exon Spliced Matrix.
 *  An exon spliced matrix is a 2D matrix embedding a 3D data structure composed of rows and exons (exon appreviated 'xon')
 *  using a specific layout:
 *
 *  | (XON) (XON) (XON) ... |
 *  | (XON) (XON) (XON) ... |
 *  | (XON) (XON) (XON) ... |
 *  |        ...        ... |
 *
 * 'XON' denotes a contiguous data block of fixed size.
 *  '()' indicates that the data bock is referenced via index.
 *
 * Each row is composed of a fixed number of xons.
 * The matrix is composed of a fixed number of rows.
 *
 * Size variables:
 *  'slos' denotes the number of slots, which is the size (number of values) per xon
 *  'xons' denotes the number of xons per row (hence slos*xons is the number of columns)
 *  'rows' number of rows of the matrix.
 *
 * Each xon is referenced via index. The index points into a common data pool indicating the first value of the xon.
 * The data structure of all indexes form a (xons x rows)-matrix, which is called 'splicing'. The splicing index matrix
 * is transposed to the spliced matrix.
 *
 * A special feature of the spliced matrix is that any two xons may have overlapping (even identical) data areas:
 *
 * Example:
 *   Data area: [ABCDEFG]
 *   xon1:      (ABCDE)
 *   xon2:      (BCDEF)
 *   xon3:      (CDEFG)
 *
 * Consequently, the data storage size can be less than the number (slos*xons*rows) of values the matrix is representing.
 * E.g.: A spliced matrix can memory efficiently represent Toeplitz or Hankel matrices.
 *
 * Matrix operations with spliced matrices as arguments are equivalent to standard matrix operations where
 * the spliced operand is constant or has non-overlapping xons. For operations creating or modifying
 * spliced operands with overlapping xons, special rules apply.
 *
 * Inflation:
 *   Inflation is the process of transferring data from a spliced source matrix to a regular target matrix.
 *   Each source value is duplicated into all mapped target values.
 *
 * Deflation:
 *   Deflation is the process of transferring data from a regular source matrix to a spliced target matrix.
 *   Each target value is set to the sum of all mapped source values.
 */

/** Nomenclature (see also nomenclature of mf3)
 *    dfl: deflation
 *    ifl: inflation
 */

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_mf3_sx_mul.h"
#include "bmath_precoded.h"

/**********************************************************************************************************************/

BETH_PRECODE( bmath_mf3_sx )
#ifdef BETH_PRECODE_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    self bmath_mf3_sx_s = bcore_inst
    {
        aware_t _;

        sz_t slos;   // values per xon
        sz_t xons;   // xons  per row
        sz_t rows;   // number of rows
        sz_t i_stride; // stride of splicing

        sz_t [] i; // index data
        f3_t [] v; // value data
    };
#endif // BETH_PRECODE_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// sets size of splicing (zeros all indexes)
void bmath_mf3_sx_s_set_size_splicing( bmath_mf3_sx_s* o, sz_t rows, sz_t xons );

/// sets size of data (zeros all values)
void bmath_mf3_sx_s_set_size_data( bmath_mf3_sx_s* o, sz_t size );

/// sets size (non-overlapping, all owning, all initialized)
void bmath_mf3_sx_s_set_size( bmath_mf3_sx_s* o, sz_t rows, sz_t xons, sz_t slos );

/// allocates and initializes data to given splicing
void bmath_mf3_sx_s_fit_size_data( bmath_mf3_sx_s* o );

/** Sets all matrix elements to random values.
 *  Parameters adhere to function bmath_mf3_s_set_random
 *  This function first creates a random bmath_mf3_s which
 *  is then deflated into bmath_mf3_s.
 */
void bmath_mf3_sx_s_set_random( bmath_mf3_sx_s* o, bl_t hsm, bl_t pdf, uz_t rd, f3_t density, f3_t min, f3_t max, u2_t* p_rval );

/// zeros all values
void bmath_mf3_sx_s_zro( bmath_mf3_sx_s* o );

/// copies and deflates data from mf3
void bmath_mf3_sx_s_cpy_dfl_from_mf3( bmath_mf3_sx_s* o, const bmath_mf3_s* src );

/// copies and inflates data to mf3
void bmath_mf3_sx_s_cpy_ifl_to_mf3( const bmath_mf3_sx_s* o, bmath_mf3_s* dst );

/**********************************************************************************************************************/
// convolution

/// sets splicing for 1D convolution with dense kernel; does not allocate or change data
void bmath_mf3_sx_s_set_splicing_for_convolution_1d( bmath_mf3_sx_s* o, sz_t size_in, sz_t size_kernel, sz_t step );

/// sets splicing for 2D convolution with dense kernel; does not allocate or change data
void bmath_mf3_sx_s_set_splicing_for_convolution_2d( bmath_mf3_sx_s* o, sz_t rows_in, sz_t cols_in, sz_t rows_kernel, sz_t cols_kernel, sz_t row_step, sz_t col_step );

/**********************************************************************************************************************/
// checks, deviations

/** Near-state means: For each matrix element the absolute difference
 *  to the specified state is less or equal max_dev.
 *  Hence, max_dev == 0 tests for the exact state (s. below).
 */
bl_t bmath_mf3_sx_s_is_near_equ( const bmath_mf3_sx_s* o, const bmath_mf3_sx_s* op, f3_t max_dev ); // equality
bl_t bmath_mf3_sx_s_is_near_zro( const bmath_mf3_sx_s* o, f3_t max_dev ); // zero

static inline bl_t bmath_mf3_sx_s_is_equ( const bmath_mf3_sx_s* o, const bmath_mf3_sx_s* op ) { return bmath_mf3_sx_s_is_near_equ( o, op, 0 ); }
static inline bl_t bmath_mf3_sx_s_is_zro( const bmath_mf3_sx_s* o ) { return bmath_mf3_sx_s_is_near_zro( o, 0 ); }

//----------------------------------------------------------------------------------------------------------------------
// Frobenius norm

/** fdev = ||f(o) - x||
 *  '|| ... ||' = Frobenius norm
 *  f is either idenity (o-->o) or a specifed function
 *  Matrix x is a specified (or implied) state.
 *  Note: By this definition fdev_zro is the Frobenius norm of o.
 */
f3_t bmath_mf3_sx_s_fdev_equ( const bmath_mf3_sx_s* o, const bmath_mf3_sx_s* op );
f3_t bmath_mf3_sx_s_fdev_zro( const bmath_mf3_sx_s* o );

// ---------------------------------------------------------------------------------------------------------------------
// debugging

// For easy inspection
void bmath_mf3_sx_s_to_sink( const bmath_mf3_sx_s* o, bcore_sink* sink );
static inline void bmath_mf3_sx_s_to_string( const bmath_mf3_sx_s* o, st_s* string ) { bmath_mf3_sx_s_to_sink( o, ( bcore_sink* )string ); }
static inline void bmath_mf3_sx_s_to_stdout( const bmath_mf3_sx_s* o               ) { bmath_mf3_sx_s_to_sink( o, BCORE_STDOUT ); }

/**********************************************************************************************************************/

vd_t bmath_mf3_sx_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_MF3_SX_H