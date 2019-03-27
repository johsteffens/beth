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

#ifndef BMATH_SMF3_H
#define BMATH_SMF3_H

/**********************************************************************************************************************/

/** Spliced Matrix.
 *  A spliced matrix is a 2D matrix embedding a 3D data structure composed of rows and xons using a specific layout:
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
 *  'vals' denotes the size (numbver of values) per xon
 *  'xons' denotes the number of xons per row (hence vals*xons is the number of columns)
 *  'rows' number of rows of the matrix.
 *
 * Each xon is referenced via index. The index points into a common data pool indicating the first value of the xon.
 * The data structure of all indexes form a nxm-matrix, which is called 'splicing'.
 *
 * A special feature of the spliced matrix is that any two xons may have overlapping (even identical) data areas:
 *
 * Example:
 *   Data area: [ABCDEFG]
 *   xon1:      (ABCDE)
 *   xon2:      (BCDEF)
 *   xon3:      (CDEFG)
 *
 * Consequently, the data storage size can be less than the number (vals*xons*rows) of values the matrix is representing.
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

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_smf3_s )
{
    aware_t _;

    sz_t vals;   // values per xon
    sz_t xons;   // xons per row
    sz_t rows;   // number of rows

    sz_t i_stride; // stride of splicing
    BCORE_ARRAY_DYN_SOLID_STATIC_S( sz_t, i_ ); // container of indexes (splicing)
    BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, v_ ); // container of values
};

/// sets size of splicing (zeros all indexes)
void bmath_smf3_s_set_size_splicing( bmath_smf3_s* o, sz_t rows, sz_t xons );

/// sets size of data (zeros all values)
void bmath_smf3_s_set_size_data( bmath_smf3_s* o, sz_t size );

/// sets size (non-overlapping, all owning, all initialized)
void bmath_smf3_s_set_size( bmath_smf3_s* o, sz_t rows, sz_t xons, sz_t vals );

/// zeros all values
void bmath_smf3_s_zro( bmath_smf3_s* o );

/// copies and deflates data from mf3
void bmath_smf3_s_cpy_dfl_from_mf3( bmath_smf3_s* o, const bmath_mf3_s* src );

/// copies and inflates data to mf3
void bmath_smf3_s_cpy_ifl_to_mf3( const bmath_smf3_s* o, bmath_mf3_s* dst );

// ---------------------------------------------------------------------------------------------------------------------

// For easy inspection
void bmath_smf3_s_to_sink( const bmath_smf3_s* o, bcore_sink* sink );
static inline void bmath_smf3_s_to_string( const bmath_smf3_s* o, st_s* string ) { bmath_smf3_s_to_sink( o, ( bcore_sink* )string ); }
static inline void bmath_smf3_s_to_stdout( const bmath_smf3_s* o               ) { bmath_smf3_s_to_sink( o, BCORE_STDOUT ); }

/**********************************************************************************************************************/

vd_t bmath_smf3_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_SMF3_H
