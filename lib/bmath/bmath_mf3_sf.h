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

#ifndef BMATH_MF3_SF_H
#define BMATH_MF3_SF_H

/**********************************************************************************************************************/

/** Fully Indexing Spliced Matrix.
 *  A fully indexing spliced matrix is a 2D matrix where each matrix element associates (via index) an element in another
 *  data structure, which is typically described as vector.
 *
 *  | (VAL) (VAL) (VAL) ... |
 *  | (VAL) (VAL) (VAL) ... |
 *  | (VAL) (VAL) (VAL) ... |
 *  |        ...        ... |
 *
 * 'CAL' denotes a single scalar value.
 *  '()' indicates that the value bock is referenced via index.
 *
 * A special feature of the spliced matrix is that different matrix element can have the same index thus
 * referencing the same value of the underlying data structure.
 *
 * Consequently, the underlying data structure can be less than the number (rwos*cols) of indices the matrix is representing.
 *
 * Matrix operations with spliced matrices as arguments are equivalent to standard matrix operations where
 * the spliced operand is constant or has non-overlapping xons. For operations creating or modifying
 * spliced operands with overlapping references, special rules apply.
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
#include "bmath_planted.h"
#include "bmath_mf3.h"
#include "bmath_mf3_sf_mul.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bmath_mf3_sf

BETH_PRECODE( bmath_mf3_sf )
#ifdef BETH_PRECODE_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    stamp bmath_mf3_sf = aware bcore_inst
    {
        sz_t cols;     // values per row
        sz_t rows;     // number of rows
        sz_t i_stride; // stride of splicing

        sz_t [] i; // index data
        f3_t [] v; // value data
    };
#endif // BETH_PRECODE_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// sets size of splicing (zeros all indexes)
void bmath_mf3_sf_s_set_size_splicing( bmath_mf3_sf_s* o, sz_t rows, sz_t cols );

/// sets size of data (zeros all values)
void bmath_mf3_sf_s_set_size_data( bmath_mf3_sf_s* o, sz_t size );

/// sets size (non-overlapping, all owning, all initialized)
void bmath_mf3_sf_s_set_size( bmath_mf3_sf_s* o, sz_t rows, sz_t cols );

/// allocates and initializes data to given splicing
void bmath_mf3_sf_s_fit_size_data( bmath_mf3_sf_s* o );

/** Sets all matrix elements to random values.
 *  Parameters adhere to function bmath_mf3_s_set_random
 *  This function first creates a random bmath_mf3_s which
 *  is then deflated into bmath_mf3_s.
 */
void bmath_mf3_sf_s_set_random( bmath_mf3_sf_s* o, bl_t hsm, bl_t pdf, uz_t rd, f3_t density, f3_t min, f3_t max, u2_t* p_rval );

/// zeros all values
void bmath_mf3_sf_s_zro( bmath_mf3_sf_s* o );

/// copies and deflates data from mf3
void bmath_mf3_sf_s_cpy_dfl_from_mf3( bmath_mf3_sf_s* o, const bmath_mf3_s* src );

/// copies and inflates data to mf3
void bmath_mf3_sf_s_cpy_ifl_to_mf3( const bmath_mf3_sf_s* o, bmath_mf3_s* dst );

/**********************************************************************************************************************/
// convolution

/// sets splicing for 1D convolution with dense kernel; does not allocate or change data
void bmath_mf3_sf_s_set_splicing_for_convolution_1d( bmath_mf3_sf_s* o, sz_t size_in, sz_t size_kernel, sz_t step );

/// sets splicing for 2D convolution with dense kernel; does not allocate or change data
void bmath_mf3_sf_s_set_splicing_for_convolution_2d( bmath_mf3_sf_s* o, sz_t rows_in, sz_t cols_in, sz_t rows_kernel, sz_t cols_kernel, sz_t row_step, sz_t col_step );

/**********************************************************************************************************************/
// checks, deviations

/** Near-state means: For each matrix element the absolute difference
 *  to the specified state is less or equal max_dev.
 *  Hence, max_dev == 0 tests for the exact state (s. below).
 */
bl_t bmath_mf3_sf_s_is_near_equ( const bmath_mf3_sf_s* o, const bmath_mf3_sf_s* op, f3_t max_dev ); // equality
bl_t bmath_mf3_sf_s_is_near_zro( const bmath_mf3_sf_s* o, f3_t max_dev ); // zero

static inline bl_t bmath_mf3_sf_s_is_equ( const bmath_mf3_sf_s* o, const bmath_mf3_sf_s* op ) { return bmath_mf3_sf_s_is_near_equ( o, op, 0 ); }
static inline bl_t bmath_mf3_sf_s_is_zro( const bmath_mf3_sf_s* o ) { return bmath_mf3_sf_s_is_near_zro( o, 0 ); }

//----------------------------------------------------------------------------------------------------------------------
// Frobenius norm

/** fdev = ||f(o) - x||
 *  '|| ... ||' = Frobenius norm
 *  f is either idenity (o-->o) or a specifed function
 *  Matrix x is a specified (or implied) state.
 *  Note: By this definition fdev_zro is the Frobenius norm of o.
 */
f3_t bmath_mf3_sf_s_fdev_equ( const bmath_mf3_sf_s* o, const bmath_mf3_sf_s* op );
f3_t bmath_mf3_sf_s_fdev_zro( const bmath_mf3_sf_s* o );

// ---------------------------------------------------------------------------------------------------------------------
// debugging

// For easy inspection
void bmath_mf3_sf_s_to_sink( const bmath_mf3_sf_s* o, bcore_sink* sink );
static inline void bmath_mf3_sf_s_to_string( const bmath_mf3_sf_s* o, st_s* string ) { bmath_mf3_sf_s_to_sink( o, ( bcore_sink* )string ); }
static inline void bmath_mf3_sf_s_to_stdout( const bmath_mf3_sf_s* o               ) { bmath_mf3_sf_s_to_sink( o, BCORE_STDOUT ); }

#endif // TYPEOF_bmath_mf3_sf

/**********************************************************************************************************************/

vd_t bmath_mf3_sf_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_MF3_SF_H
