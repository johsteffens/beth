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
 *  '()' indicates that the data block is referenced via index.
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

/** Nomenclature (see also nomenclature of mfx)
 *    dfl: deflation
 *    ifl: inflation
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/
// features

typedef void (*BCATU(bmath_fp,xsmfx,s,mul)        )( const bmath_xsmfx_s* o, const bmath_xsmfx_s* op, bmath_xsmfx_s* res );
typedef void (*BCATU(bmath_fp,xsmfx,s,mul_htp)    )( const bmath_xsmfx_s* o, const bmath_xsmfx_s* op, bmath_xsmfx_s* res );
typedef void (*BCATU(bmath_fp,xsmfx,s,htp_mul)    )( const bmath_xsmfx_s* o, const bmath_xsmfx_s* op, bmath_xsmfx_s* res );
typedef void (*BCATU(bmath_fp,xsmfx,s,htp_mul_htp))( const bmath_xsmfx_s* o, const bmath_xsmfx_s* op, bmath_xsmfx_s* res );

/**********************************************************************************************************************/

/// sets size of splicing (zeros all indexes)
void BCATU(bmath_xsmfx_s,set_size_splicing)( bmath_xsmfx_s* o, sz_t rows, sz_t xons );

/// copies splicing (does not affect values or value allocation)
void BCATU(bmath_xsmfx_s,copy_splicing)( bmath_xsmfx_s* o, const bmath_xsmfx_s* src );

/// sets size of data (zeros all values)
void BCATU(bmath_xsmfx_s,set_size_data)( bmath_xsmfx_s* o, sz_t size );

/// sets size (non-overlapping, all owning, all initialized)
void BCATU(bmath_xsmfx_s,set_size)( bmath_xsmfx_s* o, sz_t rows, sz_t xons, sz_t slos );

/// clears all data
void BCATU(bmath_xsmfx_s,clear)( bmath_xsmfx_s* o );

/// sets size and copies index index data (like copy without copying values)
void BCATU(bmath_xsmfx_s,set_shape_alike)( bmath_xsmfx_s* o, const bmath_xsmfx_s* src );

/// sets size and clones index index data (like clone without copying values)
static inline bmath_xsmfx_s* BCATU(bmath_xsmfx_s,create_shape_alike)( const bmath_xsmfx_s* src )
{
    bmath_xsmfx_s* o = BCATU(bmath_xsmfx_s,create)();
    BCATU(bmath_xsmfx_s,set_shape_alike)( o, src );
    return o;
}

/// allocates and initializes data to given splicing
void BCATU(bmath_xsmfx_s,fit_size_data)( bmath_xsmfx_s* o );

/** Sets all matrix elements to random values.
 *  Parameters adhere to function bmath_mfx_s_set_random_u3
 *  This function first creates a random bmath_mfx_s which
 *  is then deflated into bmath_mfx_s.
 */
void BCATU(bmath_xsmfx_s,set_random)(    bmath_xsmfx_s* o, bl_t hsm, bl_t pdf, uz_t rd, fx_t density, fx_t min, fx_t max, bcore_prsg* prsg );
void BCATU(bmath_xsmfx_s,set_random_u3)( bmath_xsmfx_s* o, bl_t hsm, bl_t pdf, uz_t rd, fx_t density, fx_t min, fx_t max, u3_t* p_rval );

/// zeros all values
void BCATU(bmath_xsmfx_s,zro)( bmath_xsmfx_s* o );

/// copies and deflates data from mfx,y
void BCATU(bmath_xsmfx_s,cpy_dfl_from,mfx)( bmath_xsmfx_s* o, const bmath_mfx_s* src );
void BCATU(bmath_xsmfx_s,cpy_dfl_from,mfy)( bmath_xsmfx_s* o, const bmath_mfy_s* src );

/// copies and inflates data to mfx,y
void BCATU(bmath_xsmfx_s,cpy_ifl_to,mfx)( const bmath_xsmfx_s* o, bmath_mfx_s* dst );
void BCATU(bmath_xsmfx_s,cpy_ifl_to,mfy)( const bmath_xsmfx_s* o, bmath_mfy_s* dst );

/**********************************************************************************************************************/
// convolution

/// sets splicing for 1D convolution with dense kernel; does not allocate or change data
void BCATU(bmath_xsmfx_s,set_splicing_for_convolution_1d)( bmath_xsmfx_s* o, sz_t size_in, sz_t size_kernel, sz_t step );

/// sets splicing for 2D convolution with dense kernel; does not allocate or change data
void BCATU(bmath_xsmfx_s,set_splicing_for_convolution_2d)( bmath_xsmfx_s* o, sz_t rows_in, sz_t cols_in, sz_t rows_kernel, sz_t cols_kernel, sz_t row_step, sz_t col_step );

/**********************************************************************************************************************/
// checks, deviations

/** Near-state means: For each matrix element the absolute difference
 *  to the specified state is less or equal max_dev.
 *  Hence, max_dev == 0 tests for the exact state (s. below).
 */
bl_t BCATU(bmath_xsmfx_s,is_near_equ)( const bmath_xsmfx_s* o, const bmath_xsmfx_s* op, fx_t max_dev ); // equality
bl_t BCATU(bmath_xsmfx_s,is_near_zro)( const bmath_xsmfx_s* o, fx_t max_dev ); // zero

static inline bl_t BCATU(bmath_xsmfx_s,is_equ)( const bmath_xsmfx_s* o, const bmath_xsmfx_s* op ) { return BCATU(bmath_xsmfx_s,is_near_equ)( o, op, 0 ); }
static inline bl_t BCATU(bmath_xsmfx_s,is_zro)( const bmath_xsmfx_s* o ) { return BCATU(bmath_xsmfx_s,is_near_zro)( o, 0 ); }

//----------------------------------------------------------------------------------------------------------------------
// Frobenius norm

/** fdev = ||f(o) - x||
 *  '|| ... ||' = Frobenius norm
 *  f is either idenity (o-->o) or a specifed function
 *  Matrix x is a specified (or implied) state.
 *  Note: By this definition fdev_zro is the Frobenius norm of o.
 */
fx_t BCATU(bmath_xsmfx_s,fdev_equ)( const bmath_xsmfx_s* o, const bmath_xsmfx_s* op );
fx_t BCATU(bmath_xsmfx_s,fdev_zro)( const bmath_xsmfx_s* o );

// ---------------------------------------------------------------------------------------------------------------------
// debugging

// For easy inspection
void BCATU(bmath_xsmfx_s,to_sink)( const bmath_xsmfx_s* o, bcore_sink* sink );
static inline void BCATU(bmath_xsmfx_s,to_string)( const bmath_xsmfx_s* o, st_s* string ) { BCATU(bmath_xsmfx_s,to_sink)( o, ( bcore_sink* )string ); }
static inline void BCATU(bmath_xsmfx_s,to_stdout)( const bmath_xsmfx_s* o               ) { BCATU(bmath_xsmfx_s,to_sink)( o, BCORE_STDOUT ); }

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// type conversion

er_t BCATU(bmath_xsmfx_s,copy_typed)( bmath_xsmfx_s* o, tp_t type, vc_t src );

static inline void BCATU(bmath_xsmfx_s,copy_t)( bmath_xsmfx_s* o, tp_t type, vc_t src )
{
    BCATU(bmath_xsmfx_s,copy_typed)( o, type, src );
}

static inline void BCATU(bmath_xsmfx_s,copy_a)( bmath_xsmfx_s* o, vc_t src )
{
    if( src )
    {
        BCATU(bmath_xsmfx_s,copy_t)( o, *(aware_t*)src, src );
    }
    else
    {
        BCATU(bmath_xsmfx_s,clear)( o );
    }
}

static inline bmath_xsmfx_s* BCATU(bmath_xsmfx_s,clone_t)( tp_t type, vc_t src )
{
    if( !src ) return NULL;
    bmath_xsmfx_s* o = BCATU(bmath_xsmfx_s,create)();
    BCATU(bmath_xsmfx_s,copy_t)( o, type, src );
    return o;
}

static inline bmath_xsmfx_s* BCATU(bmath_xsmfx_s,clone_a)( vc_t src )
{
    if( !src ) return NULL;
    bmath_xsmfx_s* o = BCATU(bmath_xsmfx_s,create)();
    BCATU(bmath_xsmfx_s,copy_a)( o, src );
    return o;
}

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

