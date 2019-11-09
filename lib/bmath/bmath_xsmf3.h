/** Author and Copyright 2019 Johannes Bernhard Steffens
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

#ifndef BMATH_XSMF3_H
#define BMATH_XSMF3_H

/**********************************************************************************************************************/

/** Exon Spliced Matrix. (XSM)
 *  We define the XSM as 2D matrix embedding a 3D data structure composed of rows and exons (exon abbreviated 'xon')
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
 *  The reason for this specific matrix design is the efficient computation of of convolutions, transposed convolutions
 *  and their respective gradient propagation by expressing them as fast matrix*matrix multiplications.
 *  Conceptually the ASM and XSM serve the same purpose. ASM is most flexible and applicable on any type of convolution
 *  including a series of operations, which are not strictly convolutions. XSM can theoretically represent any ASM but
 *  is more efficient on convolution operations used typically with neural networks.
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
 * Consequently, the data storage size can be smaller than the number (slos*xons*rows) of values the matrix is representing.
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
#include "bmath_xsmf3_mul.h"
#include "bmath_planted.h"

/**********************************************************************************************************************/

PLANT_GROUP( bmath_xsmf3, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    stamp bmath_xsmf3 = aware bcore_inst
    {
        sz_t slos;   // values per xon
        sz_t xons;   // xons  per row
        sz_t rows;   // number of rows
        sz_t i_stride; // stride of splicing

        sz_t [] i; // index data
        f3_t [] v; // value data
    };

    // function pointer names
    name bmath_fp_xsmf3_s_mul;
    name bmath_fp_xsmf3_s_mul_htp;
    name bmath_fp_xsmf3_s_htp_mul;
    name bmath_fp_xsmf3_s_htp_mul_htp;

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/
// features

typedef void (*bmath_fp_xsmf3_s_mul        )( const bmath_xsmf3_s* o, const bmath_xsmf3_s* op, bmath_xsmf3_s* res );
typedef void (*bmath_fp_xsmf3_s_mul_htp    )( const bmath_xsmf3_s* o, const bmath_xsmf3_s* op, bmath_xsmf3_s* res );
typedef void (*bmath_fp_xsmf3_s_htp_mul    )( const bmath_xsmf3_s* o, const bmath_xsmf3_s* op, bmath_xsmf3_s* res );
typedef void (*bmath_fp_xsmf3_s_htp_mul_htp)( const bmath_xsmf3_s* o, const bmath_xsmf3_s* op, bmath_xsmf3_s* res );

/**********************************************************************************************************************/

/// sets size of splicing (zeros all indexes)
void bmath_xsmf3_s_set_size_splicing( bmath_xsmf3_s* o, sz_t rows, sz_t xons );

/// copies splicing (does not affect values or value allocation)
void bmath_xsmf3_s_copy_splicing( bmath_xsmf3_s* o, const bmath_xsmf3_s* src );

/// sets size of data (zeros all values)
void bmath_xsmf3_s_set_size_data( bmath_xsmf3_s* o, sz_t size );

/// sets size (non-overlapping, all owning, all initialized)
void bmath_xsmf3_s_set_size( bmath_xsmf3_s* o, sz_t rows, sz_t xons, sz_t slos );

/// sets size and copies index index data (like copy without copying values)
void bmath_xsmf3_s_set_shape_alike( bmath_xsmf3_s* o, const bmath_xsmf3_s* src );

/// sets size and clones index index data (like clone without copying values)
static inline bmath_xsmf3_s* bmath_xsmf3_s_create_shape_alike( const bmath_xsmf3_s* src )
{
    bmath_xsmf3_s* o = bmath_xsmf3_s_create();
    bmath_xsmf3_s_set_shape_alike( o, src );
    return o;
}

/// allocates and initializes data to given splicing
void bmath_xsmf3_s_fit_size_data( bmath_xsmf3_s* o );

/** Sets all matrix elements to random values.
 *  Parameters adhere to function bmath_mf3_s_set_random
 *  This function first creates a random bmath_mf3_s which
 *  is then deflated into bmath_mf3_s.
 */
void bmath_xsmf3_s_set_random( bmath_xsmf3_s* o, bl_t hsm, bl_t pdf, uz_t rd, f3_t density, f3_t min, f3_t max, u2_t* p_rval );

/// zeros all values
void bmath_xsmf3_s_zro( bmath_xsmf3_s* o );

/// copies and deflates data from mf3
void bmath_xsmf3_s_cpy_dfl_from_mf3( bmath_xsmf3_s* o, const bmath_mf3_s* src );

/// copies and inflates data to mf3
void bmath_xsmf3_s_cpy_ifl_to_mf3( const bmath_xsmf3_s* o, bmath_mf3_s* dst );

/**********************************************************************************************************************/
// convolution

/// sets splicing for 1D convolution with dense kernel; does not allocate or change data
void bmath_xsmf3_s_set_splicing_for_convolution_1d( bmath_xsmf3_s* o, sz_t size_in, sz_t size_kernel, sz_t step );

/// sets splicing for 2D convolution with dense kernel; does not allocate or change data
void bmath_xsmf3_s_set_splicing_for_convolution_2d( bmath_xsmf3_s* o, sz_t rows_in, sz_t cols_in, sz_t rows_kernel, sz_t cols_kernel, sz_t row_step, sz_t col_step );

/**********************************************************************************************************************/
// checks, deviations

/** Near-state means: For each matrix element the absolute difference
 *  to the specified state is less or equal max_dev.
 *  Hence, max_dev == 0 tests for the exact state (s. below).
 */
bl_t bmath_xsmf3_s_is_near_equ( const bmath_xsmf3_s* o, const bmath_xsmf3_s* op, f3_t max_dev ); // equality
bl_t bmath_xsmf3_s_is_near_zro( const bmath_xsmf3_s* o, f3_t max_dev ); // zero

static inline bl_t bmath_xsmf3_s_is_equ( const bmath_xsmf3_s* o, const bmath_xsmf3_s* op ) { return bmath_xsmf3_s_is_near_equ( o, op, 0 ); }
static inline bl_t bmath_xsmf3_s_is_zro( const bmath_xsmf3_s* o ) { return bmath_xsmf3_s_is_near_zro( o, 0 ); }

//----------------------------------------------------------------------------------------------------------------------
// Frobenius norm

/** fdev = ||f(o) - x||
 *  '|| ... ||' = Frobenius norm
 *  f is either idenity (o-->o) or a specifed function
 *  Matrix x is a specified (or implied) state.
 *  Note: By this definition fdev_zro is the Frobenius norm of o.
 */
f3_t bmath_xsmf3_s_fdev_equ( const bmath_xsmf3_s* o, const bmath_xsmf3_s* op );
f3_t bmath_xsmf3_s_fdev_zro( const bmath_xsmf3_s* o );

// ---------------------------------------------------------------------------------------------------------------------
// debugging

// For easy inspection
void bmath_xsmf3_s_to_sink( const bmath_xsmf3_s* o, bcore_sink* sink );
static inline void bmath_xsmf3_s_to_string( const bmath_xsmf3_s* o, st_s* string ) { bmath_xsmf3_s_to_sink( o, ( bcore_sink* )string ); }
static inline void bmath_xsmf3_s_to_stdout( const bmath_xsmf3_s* o               ) { bmath_xsmf3_s_to_sink( o, BCORE_STDOUT ); }

/**********************************************************************************************************************/

vd_t bmath_xsmf3_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_XSMF3_H
