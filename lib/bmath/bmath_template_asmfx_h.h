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

/** Atomic Spliced Matrix. (ASM)
 *  We define ASM as 2D matrix where each matrix element associates (via index) an element in another data structure,
 *  which is typically described as vector.
 *
 *  | (VAL) (VAL) (VAL) ... |
 *  | (VAL) (VAL) (VAL) ... |
 *  | (VAL) (VAL) (VAL) ... |
 *  |        ...        ... |
 *
 * 'CAL' denotes a single scalar value.
 *  '()' indicates that the value bock is referenced via index.
 *
 *  The reason for this specific matrix design is the efficient computation of of convolutions, transposed convolutions and
 *  their respective gradient propagation by expressing them as fast matrix*matrix multiplications.
 *  Conceptually the ASM and XSM serve the same purpose. ASM is most flexible and applicable on any type of convolution
 *  including a series of operations, which are not strictly convolutions. XSM can theoretically represent any ASM but
 *  is more efficient on convolution operations used typically with neural networks.
 *
 * A special feature of the spliced matrix is that different matrix element can have the same index thus
 * referencing the same value of the underlying data structure.
 *
 * Consequently, the underlying data structure can be smaller than the number (rows*cols) of indices the matrix is representing.
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

/** Nomenclature (see also nomenclature of mfx)
 *    dfl: deflation
 *    ifl: inflation
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/
// features

typedef void (*BCATU(bmath_fp,asmfx,s,mul)        )( const bmath_asmfx_s* o, const bmath_asmfx_s* op, bmath_asmfx_s* res );
typedef void (*BCATU(bmath_fp,asmfx,s,mul_htp)    )( const bmath_asmfx_s* o, const bmath_asmfx_s* op, bmath_asmfx_s* res );
typedef void (*BCATU(bmath_fp,asmfx,s,htp_mul)    )( const bmath_asmfx_s* o, const bmath_asmfx_s* op, bmath_asmfx_s* res );
typedef void (*BCATU(bmath_fp,asmfx,s,htp_mul_htp))( const bmath_asmfx_s* o, const bmath_asmfx_s* op, bmath_asmfx_s* res );

/**********************************************************************************************************************/

/// sets size of splicing (zeros all indexes)
void BCATU(bmath_asmfx_s,set_size_splicing)( bmath_asmfx_s* o, sz_t rows, sz_t cols );

/// copies splicing (does not affect values or value allocation)
void BCATU(bmath_asmfx_s,copy_splicing)( bmath_asmfx_s* o, const bmath_asmfx_s* src );

/// sets size of data (zeros all values)
void BCATU(bmath_asmfx_s,set_size_data)( bmath_asmfx_s* o, sz_t size );

/// sets size (non-overlapping, all owning, all initialized)
void BCATU(bmath_asmfx_s,set_size)( bmath_asmfx_s* o, sz_t rows, sz_t cols );

/// clears all data
void BCATU(bmath_asmfx_s,clear)( bmath_asmfx_s* o );

/// sets size and copies index index data (like copy without copying values)
void BCATU(bmath_asmfx_s,set_shape_alike)( bmath_asmfx_s* o, const bmath_asmfx_s* src );

/// sets size and clones index index data (like clone without copying values)
static inline bmath_asmfx_s* BCATU(bmath_asmfx_s,create_shape_alike)( const bmath_asmfx_s* src )
{
    bmath_asmfx_s* o = BCATU(bmath_asmfx_s,create)();
    BCATU(bmath_asmfx_s,set_shape_alike)( o, src );
    return o;
}

/// allocates and initializes data to given splicing
void BCATU(bmath_asmfx_s,fit_size_data)( bmath_asmfx_s* o );

/** Sets all matrix elements to random values.
 *  Parameters adhere to function bmath_mfx_s_set_random_u3
 *  This function first creates a random bmath_mfx_s which
 *  is then deflated into bmath_mfx_s.
 */
void BCATU(bmath_asmfx_s,set_random_u3)( bmath_asmfx_s* o, bl_t hsm, bl_t pdf, uz_t rd, fx_t density, fx_t min, fx_t max, u3_t* p_rval );

/// zeros all values
void BCATU(bmath_asmfx_s,zro)( bmath_asmfx_s* o );

/// copies and deflates data from mfx,y
void BCATU(bmath_asmfx_s,cpy_dfl_from,mfx)( bmath_asmfx_s* o, const bmath_mfx_s* src );
void BCATU(bmath_asmfx_s,cpy_dfl_from,mfy)( bmath_asmfx_s* o, const bmath_mfy_s* src );

/// copies and inflates data to mfx,y
void BCATU(bmath_asmfx_s,cpy_ifl_to,mfx)( const bmath_asmfx_s* o, bmath_mfx_s* dst );
void BCATU(bmath_asmfx_s,cpy_ifl_to,mfy)( const bmath_asmfx_s* o, bmath_mfy_s* dst );

/**********************************************************************************************************************/
// convolution

/// sets splicing for 1D convolution with dense kernel; does not allocate or change data
void BCATU(bmath_asmfx_s,set_splicing_for_convolution_1d)( bmath_asmfx_s* o, sz_t size_in, sz_t size_kernel, sz_t step );

/// sets splicing for 2D convolution with dense kernel; does not allocate or change data
void BCATU(bmath_asmfx_s,set_splicing_for_convolution_2d)( bmath_asmfx_s* o, sz_t rows_in, sz_t cols_in, sz_t rows_kernel, sz_t cols_kernel, sz_t row_step, sz_t col_step );

/**********************************************************************************************************************/
// checks, deviations

/** Near-state means: For each matrix element the absolute difference
 *  to the specified state is less or equal max_dev.
 *  Hence, max_dev == 0 tests for the exact state (s. below).
 */
bl_t BCATU(bmath_asmfx_s,is_near_equ)( const bmath_asmfx_s* o, const bmath_asmfx_s* op, fx_t max_dev ); // equality
bl_t BCATU(bmath_asmfx_s,is_near_zro)( const bmath_asmfx_s* o, fx_t max_dev ); // zero

static inline bl_t BCATU(bmath_asmfx_s,is_equ)( const bmath_asmfx_s* o, const bmath_asmfx_s* op ) { return BCATU(bmath_asmfx_s,is_near_equ)( o, op, 0 ); }
static inline bl_t BCATU(bmath_asmfx_s,is_zro)( const bmath_asmfx_s* o ) { return BCATU(bmath_asmfx_s,is_near_zro)( o, 0 ); }

//----------------------------------------------------------------------------------------------------------------------
// Frobenius norm

/** fdev = ||f(o) - x||
 *  '|| ... ||' = Frobenius norm
 *  f is either idenity (o-->o) or a specifed function
 *  Matrix x is a specified (or implied) state.
 *  Note: By this definition fdev_zro is the Frobenius norm of o.
 */
fx_t BCATU(bmath_asmfx_s,fdev_equ)( const bmath_asmfx_s* o, const bmath_asmfx_s* op );
fx_t BCATU(bmath_asmfx_s,fdev_zro)( const bmath_asmfx_s* o );

// ---------------------------------------------------------------------------------------------------------------------
// debugging

// For easy inspection
void BCATU(bmath_asmfx_s,to_sink)( const bmath_asmfx_s* o, bcore_sink* sink );
static inline void BCATU(bmath_asmfx_s,to_string)( const bmath_asmfx_s* o, st_s* string ) { BCATU(bmath_asmfx_s,to_sink)( o, ( bcore_sink* )string ); }
static inline void BCATU(bmath_asmfx_s,to_stdout)( const bmath_asmfx_s* o               ) { BCATU(bmath_asmfx_s,to_sink)( o, BCORE_STDOUT ); }

/**********************************************************************************************************************/
// type conversion

void BCATU(bmath_asmfx_s,copy_typed)( bmath_asmfx_s* o, tp_t type, vc_t src );

static inline void BCATU(bmath_asmfx_s,copy_t)( bmath_asmfx_s* o, tp_t type, vc_t src )
{
    BCATU(bmath_asmfx_s,copy_typed)( o, type, src );
}

static inline void BCATU(bmath_asmfx_s,copy_a)( bmath_asmfx_s* o, vc_t src )
{
    if( src )
    {
        BCATU(bmath_asmfx_s,copy_t)( o, *(aware_t*)src, src );
    }
    else
    {
        BCATU(bmath_asmfx_s,clear)( o );
    }
}

static inline bmath_asmfx_s* BCATU(bmath_asmfx_s,clone_t)( tp_t type, vc_t src )
{
    if( !src ) return NULL;
    bmath_asmfx_s* o = BCATU(bmath_asmfx_s,create)();
    BCATU(bmath_asmfx_s,copy_t)( o, type, src );
    return o;
}

static inline bmath_asmfx_s* BCATU(bmath_asmfx_s,clone_a)( vc_t src )
{
    if( !src ) return NULL;
    bmath_asmfx_s* o = BCATU(bmath_asmfx_s,create)();
    BCATU(bmath_asmfx_s,copy_a)( o, src );
    return o;
}

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

