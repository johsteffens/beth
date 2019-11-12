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

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

#define BMATH_ASM_MUL_BLKPK_SIZE 12
#define BMATH_ASM_MUL_BLOCK_SIZE ( BMATH_ASM_MUL_BLKPK_SIZE * P5_SIZE )

/**********************************************************************************************************************/
// Microkernels

static sz_t midof( sz_t v, const sz_t bz )
{
    return ( v > ( bz << 1 ) ) ? ( v / ( bz << 1 ) ) * bz : bz;
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Fixed size Microkernel
 *  Requirement: o_cols == BMATH_ASM_MUL_BLOCK_SIZE && m_cols == BMATH_ASM_MUL_BLOCK_SIZE
 */
static void kernel_fixed_mul
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_col,
    const bmath_asmfx_s* m,                          sz_t m_col,
          bmath_asmfx_s* r
)
{
#ifdef BMATH_AVX

    M5_T r_pk[ BMATH_ASM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
        {
            sz_t* m_x = &m->i_data[ ( o_col + j ) * m->i_stride + m_col + k * P5_SIZE ];
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = m->v_data[ m_x[ i ] ];
            }
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
              sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + m_col ];

        M5_T o_pk = M5_SET_ALL( o->v_data[ o_x[ 0 ] ] );

        for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            o_pk = M5_SET_ALL( o->v_data[ o_x[ j ] ] );
            for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                r->v_data[ r_x[ k * P5_SIZE + i ] ] += r_pk[ k ][ i ];
            }
        }
    }

#else

    fx_t r_p[ BMATH_ASM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLOCK_SIZE ];
    for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ ( o_col + j ) * m->i_stride + m_col + k ] ];
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            fx_t o_v = o->v_data[ o_x[ j ] ];
            for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
        }

        sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + m_col ];
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
    }

#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Flexible Microkernel
 *  Requirement: o_cols <= BMATH_ASM_MUL_BLOCK_SIZE && m_cols <= BMATH_ASM_MUL_BLOCK_SIZE
 */
static void kernel_flexi_mul
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_asmfx_s* m,                          sz_t m_col, sz_t m_cols,
          bmath_asmfx_s* r
)
{
#ifdef BMATH_AVX

    ASSERT( o_cols <= BMATH_ASM_MUL_BLOCK_SIZE );
    ASSERT( m_cols <= BMATH_ASM_MUL_BLOCK_SIZE );

    // notation:
    // p_fit: number of M5_t fitting into block
    // p_ful: number of M5_t required to hold all data of block (either p_fit or p_fit + 1)
    // rest:  remaining fx_t not fitting into p_fit
    const sz_t m_cols_p_fit = m_cols >> P5_SIZE_B2E;
    const sz_t m_cols_rest  = m_cols - m_cols_p_fit * P5_SIZE;
    const sz_t m_cols_p_ful = m_cols_rest > 0 ? m_cols_p_fit + 1 : m_cols_p_fit;

    M5_T r_pk[ BMATH_ASM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < o_cols; j++ )
    {
        for( sz_t k = 0; k < m_cols_p_fit; k++ )
        {
            sz_t* m_x = &m->i_data[ ( o_col + j ) * m->i_stride + m_col + k * P5_SIZE ];
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = m->v_data[ m_x[ i ] ];
            }
        }

        if( m_cols_rest > 0 )
        {
            m_pk[ j ][ m_cols_p_fit ] = M5_SET_ZERO();
            for( sz_t k = 0; k < m_cols_rest;  k++ )
            {
                m_pk[ j ][ m_cols_p_fit ][ k ] = m->v_data[ m->i_data[ ( o_col + j ) * m->i_stride + m_col + m_cols_p_fit * P5_SIZE + k ] ];
            }
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
              sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + m_col ];

        for( sz_t k = 0; k < m_cols_p_ful; k++ ) r_pk[ k ] = M5_SET_ZERO();

        for( sz_t j = 0; j < o_cols; j++ )
        {
            M5_T o_pk = M5_SET_ALL( o->v_data[ o_x[ j ] ] );
            for( sz_t k = 0; k < m_cols_p_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < m_cols_p_fit; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                r->v_data[ r_x[ k * P5_SIZE + i ] ] += r_pk[ k ][ i ];
            }
        }

        if( m_cols_rest > 0 )
        {
            for( sz_t k = 0; k < m_cols_rest; k++ ) r->v_data[ r_x[ m_cols_p_fit * P5_SIZE + k ] ] += r_pk[ m_cols_p_fit ][ k ];
        }
    }

#else

    fx_t r_p[ BMATH_ASM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLOCK_SIZE ];
    for( sz_t j = 0; j < o_cols; j++ )
    {
        for( sz_t k = 0; k < m_cols; k++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ ( o_col + j ) * m->i_stride + m_col + k ] ];
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
        for( sz_t k = 0; k < m_cols; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_cols; j++ )
        {
            fx_t o_v = o->v_data[ o_x[ j ] ];
            for( sz_t k = 0; k < m_cols; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
        }

        sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + m_col ];
        for( sz_t k = 0; k < m_cols; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
    }

#endif
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Fixed size Microkernel
 *  Requirement: o_cols == BMATH_ASM_MUL_BLOCK_SIZE && m_cols == BMATH_ASM_MUL_BLOCK_SIZE
 */
static void kernel_fixed_mul_htp
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_col,
    const bmath_asmfx_s* m,
          bmath_asmfx_s* r,                          sz_t r_col
)
{
#ifdef BMATH_AVX
    assert( ( BMATH_ASM_MUL_BLOCK_SIZE & 3 ) == 0 );

    M5_T r_pk[ BMATH_ASM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLKPK_SIZE ];

    for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
    {
        sz_t m_row = r_col + k * P5_SIZE;
        for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = m->v_data[ m->i_data[ ( m_row + i ) * m->i_stride + o_col + j ] ];
            }
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
              sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + r_col ];

        M5_T o_pk = M5_SET_ALL( o->v_data[ o_x[ 0 ] ] );

        for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            o_pk = M5_SET_ALL( o->v_data[ o_x[ j ] ] );
            for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                r->v_data[ r_x[ k * P5_SIZE + i ] ] += r_pk[ k ][ i ];
            }
        }
    }
#else
    fx_t r_p[ BMATH_ASM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ )
    {
        for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ ( r_col + k ) * m->i_stride + o_col + j ] ];
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
              sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + r_col ];

        for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;
        for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o->v_data[ o_x[ j ] ];
        }
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
    }
#endif
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Flexible Microkernel
 *  Requirement: o_cols <= BMATH_ASM_MUL_BLOCK_SIZE && m_cols <= BMATH_ASM_MUL_BLOCK_SIZE
 */
static void kernel_flexi_mul_htp
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_asmfx_s* m,
          bmath_asmfx_s* r,                          sz_t r_col, sz_t r_cols
)
{
#ifdef BMATH_AVX
    ASSERT( o_cols <= BMATH_ASM_MUL_BLOCK_SIZE );
    ASSERT( r_cols <= BMATH_ASM_MUL_BLOCK_SIZE );

    const sz_t r_cols_p_fit = r_cols >> P5_SIZE_B2E;
    const sz_t r_cols_rest  = r_cols - r_cols_p_fit * P5_SIZE;
    const sz_t r_cols_p_ful = r_cols_rest > 0 ? r_cols_p_fit + 1 : r_cols_p_fit;

    M5_T r_pk[ BMATH_ASM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLKPK_SIZE ];

    for( sz_t k = 0; k < r_cols_p_fit; k++ )
    {
        sz_t m_row = r_col + k * P5_SIZE;
        for( sz_t j = 0; j < o_cols; j++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = m->v_data[ m->i_data[ ( m_row + i ) * m->i_stride + o_col + j ] ];
            }
        }
    }

    if( r_cols_rest > 0 )
    {
        for( sz_t j = 0; j < o_cols; j++ ) m_pk[ j ][ r_cols_p_fit ] = M5_SET_ZERO();
        for( sz_t k = 0; k < r_cols_rest; k++ )
        {
            for( sz_t j = 0; j < o_cols; j++ )
            {
                m_pk[ j ][ r_cols_p_fit ][ k ] = m->v_data[ m->i_data[ ( r_col + r_cols_p_fit * P5_SIZE + k ) * m->i_stride + o_col + j ] ];
            }
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
              sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + r_col ];

        for( sz_t k = 0; k < r_cols_p_ful; k++ ) r_pk[ k ] = M5_SET_ZERO();

        for( sz_t j = 0; j < o_cols; j++ )
        {
            M5_T o_pk = M5_SET_ALL( o->v_data[ o_x[ j ] ] );
            for( sz_t k = 0; k < r_cols_p_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < r_cols_p_fit; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                r->v_data[ r_x[ k * P5_SIZE + i ] ] += r_pk[ k ][ i ];
            }
        }

        if( r_cols_rest > 0 )
        {
            for( sz_t k = 0; k < r_cols_rest; k++ ) r->v_data[ r_x[ r_cols_p_fit * P5_SIZE + k ] ] += r_pk[ r_cols_p_fit ][ k ];
        }
    }
#else
    fx_t r_p[ BMATH_ASM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < r_cols; k++ )
    {
        for( sz_t j = 0; j < o_cols; j++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ ( r_col + k ) * m->i_stride + o_col + j ] ];
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];

        for( sz_t k = 0; k < r_cols; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_cols; j++ )
        {
            for( sz_t k = 0; k < r_cols; k++ ) r_p[ k ] += m_p[ j ][ k ] * o->v_data[ o_x[ j ] ];
        }

        const sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + r_col ];
        for( sz_t k = 0; k < r_cols; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
    }
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Fixed size Microkernel
 *  Requirement: o_rows == BMATH_ASM_MUL_BLOCK_SIZE && m_cols == BMATH_ASM_MUL_BLOCK_SIZE
 */
static void kernel_fixed_htp_mul
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_col, sz_t o_cols,
    const bmath_asmfx_s* m,             sz_t m_col,
          bmath_asmfx_s* r
)
{
#ifdef BMATH_AVX
    M5_T r_pk[ BMATH_ASM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
        {
            sz_t idx = ( o_row + j ) * m->i_stride + m_col + k * P5_SIZE;
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = m->v_data[ m->i_data[ idx + i ] ];
            }
        }
    }

    for( sz_t i = 0; i < o_cols; i++ )
    {
        const sz_t* o_x = &o->i_data[ o_row * o->i_stride + o_col ];
        M5_T o_pk = M5_SET_ALL( o->v_data[ o_x[ i ] ] );

        for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            const sz_t* o_x = &o->i_data[ ( o_row + j ) * o->i_stride + o_col ];
            M5_T o_pk = M5_SET_ALL( o->v_data[ o_x[ i ] ] );
            for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        sz_t m_row = o_col;
        const sz_t* r_x = &r->i_data[ ( m_row + i ) * r->i_stride + m_col ];
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                r->v_data[ r_x[ k * P5_SIZE + i ] ] += r_pk[ k ][ i ];
            }
        }
    }
#else
    fx_t r_p[ BMATH_ASM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLOCK_SIZE ];
    for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ ( o_row + j ) * m->i_stride + m_col + k ] ];
        }
    }

    for( sz_t i = 0; i < o_cols; i++ )
    {
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            fx_t o_v = o->v_data[ o->i_data[ ( o_row + j ) * o->i_stride + o_col + i ] ];
            for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
        }

        sz_t m_row = o_col;

        const sz_t* r_x = &r->i_data[ ( m_row + i ) * r->i_stride + m_col ];
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
    }
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Flexible Microkernel
 *  Requirement: o_rows <= BMATH_ASM_MUL_BLOCK_SIZE && m_cols <= BMATH_ASM_MUL_BLOCK_SIZE
 */
static void kernel_flexi_htp_mul
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_asmfx_s* m,                          sz_t m_col, sz_t m_cols,
          bmath_asmfx_s* r
)
{
#ifdef BMATH_AVX
    assert( o_rows <= BMATH_ASM_MUL_BLOCK_SIZE );
    assert( m_cols <= BMATH_ASM_MUL_BLOCK_SIZE );

    const sz_t m_cols_p_fit = m_cols >> P5_SIZE_B2E;
    const sz_t m_cols_rest  = m_cols - m_cols_p_fit * P5_SIZE;
    const sz_t m_cols_p_ful = m_cols_rest > 0 ? m_cols_p_fit + 1 : m_cols_p_fit;

    M5_T r_pk[ BMATH_ASM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < o_rows; j++ )
    {
        for( sz_t k = 0; k < m_cols_p_fit; k++ )
        {
            sz_t idx = ( o_row + j ) * m->i_stride + m_col + k * P5_SIZE;
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = m->v_data[ m->i_data[ idx + i ] ];
            }
        }

        if( m_cols_rest > 0 )
        {
            m_pk[ j ][ m_cols_p_fit ] = M5_SET_ZERO();
            for( sz_t k = 0; k < m_cols_rest;  k++ )
            {
                m_pk[ j ][ m_cols_p_fit ][ k ] = m->v_data[ m->i_data[ ( o_row + j ) * m->i_stride + m_col + m_cols_p_fit * P5_SIZE + k ] ];
            }
        }
    }

    for( sz_t i = 0; i < o_cols; i++ )
    {
        for( sz_t k = 0; k < m_cols_p_ful; k++ ) r_pk[ k ] = M5_SET_ZERO();

        for( sz_t j = 0; j < o_rows; j++ )
        {
            const sz_t* o_x = &o->i_data[ ( o_row + j ) * o->i_stride + o_col ];
            M5_T o_pk = M5_SET_ALL( o->v_data[ o_x[ i ] ] );
            for( sz_t k = 0; k < m_cols_p_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        sz_t m_row = o_col;
        const sz_t* r_x = &r->i_data[ ( m_row + i ) * r->i_stride + m_col ];
        for( sz_t k = 0; k < m_cols_p_fit; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                r->v_data[ r_x[ k * P5_SIZE + i ] ] += r_pk[ k ][ i ];
            }
        }

        if( m_cols_rest > 0 )
        {
            for( sz_t k = 0; k < m_cols_rest; k++ ) r->v_data[ r_x[ m_cols_p_fit * P5_SIZE + k ] ] += r_pk[ m_cols_p_fit ][ k ];
        }
    }
#else
    fx_t r_p[ BMATH_ASM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLOCK_SIZE ];
    for( sz_t j = 0; j < o_rows; j++ )
    {
        for( sz_t k = 0; k < m_cols; k++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ ( o_row + j ) * m->i_stride + m_col + k ] ];
        }
    }

    for( sz_t i = 0; i < o_cols; i++ )
    {
        for( sz_t k = 0; k < m_cols; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_rows; j++ )
        {
            fx_t o_v = o->v_data[ o->i_data[ ( o_row + j ) * o->i_stride + o_col + i ] ];
            for( sz_t k = 0; k < m_cols; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
        }

        sz_t m_row = o_col;

        const sz_t* r_x = &r->i_data[ ( m_row + i ) * r->i_stride + m_col ];
        for( sz_t k = 0; k < m_cols; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
    }
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Fixed size Microkernel
 *  Requirement: o_rows == BMATH_ASM_MUL_BLOCK_SIZE && m_rows == BMATH_ASM_MUL_BLOCK_SIZE
 */
static void kernel_fixed_htp_mul_htp
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_col, sz_t o_cols,
    const bmath_asmfx_s* m,             sz_t m_row,
          bmath_asmfx_s* r
)
{
#ifdef BMATH_AVX
    M5_T r_pk[ BMATH_ASM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLKPK_SIZE ];

    for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
    {
        for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = m->v_data[ m->i_data[ ( m_row + k * P5_SIZE + i ) * m->i_stride + o_row + j ] ];
            }
        }
    }

    for( sz_t i = 0; i < o_cols; i++ )
    {
        const sz_t* o_x = &o->i_data[ o_row * o->i_stride + o_col ];
        M5_T o_pk = M5_SET_ALL( o->v_data[ o_x[ i ] ] );

        for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            const sz_t* o_x = &o->i_data[ ( o_row + j ) * o->i_stride + o_col ];
            M5_T o_pk = M5_SET_ALL( o->v_data[ o_x[ i ] ] );
            for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        const sz_t* r_x = &r->i_data[ ( o_col + i ) * r->i_stride + m_row ];
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLKPK_SIZE; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                r->v_data[ r_x[ k * P5_SIZE + i ] ] += r_pk[ k ][ i ];
            }
        }
    }
#else
    fx_t r_p[ BMATH_ASM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ )
    {
        for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ ( m_row + k ) * m->i_stride + o_row + j ] ];
        }
    }

    for( sz_t i = 0; i < o_cols; i++ )
    {
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < BMATH_ASM_MUL_BLOCK_SIZE; j++ )
        {
            fx_t o_v = o->v_data[ o->i_data[ ( o_row + j ) * o->i_stride + o_col + i ] ];
            for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
        }

        const sz_t* r_x = &r->i_data[ ( o_col + i ) * r->i_stride + m_row ];
        for( sz_t k = 0; k < BMATH_ASM_MUL_BLOCK_SIZE; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
    }
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Flexible Microkernel
 *  Requirement: o_rows <= BMATH_ASM_MUL_BLOCK_SIZE && m_rows <= BMATH_ASM_MUL_BLOCK_SIZE
 */
static void kernel_flexi_htp_mul_htp
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_asmfx_s* m,                          sz_t m_row, sz_t m_rows,
          bmath_asmfx_s* r
)
{
#ifdef BMATH_AVX
    ASSERT( o_rows <= BMATH_ASM_MUL_BLOCK_SIZE );
    ASSERT( m_rows <= BMATH_ASM_MUL_BLOCK_SIZE );

    sz_t r_col  = m_row;
    sz_t r_cols = m_rows;

    const sz_t r_cols_p_fit = r_cols >> P5_SIZE_B2E;
    const sz_t r_cols_rest  = r_cols - r_cols_p_fit * P5_SIZE;
    const sz_t r_cols_p_ful = r_cols_rest > 0 ? r_cols_p_fit + 1 : r_cols_p_fit;

    M5_T r_pk[ BMATH_ASM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLKPK_SIZE ];

    for( sz_t k = 0; k < r_cols_p_fit; k++ )
    {
        sz_t m_row = r_col + k * P5_SIZE;
        for( sz_t j = 0; j < o_rows; j++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = m->v_data[ m->i_data[ ( m_row + i ) * m->i_stride + o_row + j ] ];
            }
        }
    }

    if( r_cols_rest > 0 )
    {
        for( sz_t j = 0; j < o_rows; j++ ) m_pk[ j ][ r_cols_p_fit ] = M5_SET_ZERO();
        for( sz_t k = 0; k < r_cols_rest; k++ )
        {
            for( sz_t j = 0; j < o_rows; j++ )
            {
                m_pk[ j ][ r_cols_p_fit ][ k ] = m->v_data[ m->i_data[ ( r_col + r_cols_p_fit * P5_SIZE + k ) * m->i_stride + o_row + j ] ];
            }
        }
    }

    for( sz_t i = 0; i < o_cols; i++ )
    {
        const sz_t* o_x = &o->i_data[ o_row * o->i_stride + o_col + i ];
              sz_t* r_x = &r->i_data[ ( o_col + i ) * r->i_stride + r_col ];

        for( sz_t k = 0; k < r_cols_p_ful; k++ ) r_pk[ k ] = M5_SET_ZERO();

        for( sz_t j = 0; j < o_rows; j++ )
        {
            M5_T o_pk = M5_SET_ALL( o->v_data[ o_x[ j * o->i_stride ] ] );
            for( sz_t k = 0; k < r_cols_p_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < r_cols_p_fit; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                r->v_data[ r_x[ k * P5_SIZE + i ] ] += r_pk[ k ][ i ];
            }
        }

        if( r_cols_rest > 0 )
        {
            for( sz_t k = 0; k < r_cols_rest; k++ ) r->v_data[ r_x[ r_cols_p_fit * P5_SIZE + k ] ] += r_pk[ r_cols_p_fit ][ k ];
        }
    }
#else
    fx_t r_p[ BMATH_ASM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_ASM_MUL_BLOCK_SIZE ][ BMATH_ASM_MUL_BLOCK_SIZE ];

    sz_t r_col  = m_row;
    sz_t r_cols = m_rows;

    for( sz_t k = 0; k < r_cols; k++ )
    {
        for( sz_t j = 0; j < o_rows; j++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ ( r_col + k ) * m->i_stride + o_row + j ] ];
        }
    }

    for( sz_t i = 0; i < o_cols; i++ )
    {
        const sz_t* o_x = &o->i_data[ o_row * o->i_stride + o_col + i ];

        for( sz_t k = 0; k < r_cols; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_rows; j++ )
        {
            for( sz_t k = 0; k < r_cols; k++ ) r_p[ k ] += m_p[ j ][ k ] * o->v_data[ o_x[ j * o->i_stride ] ];
        }

        const sz_t* r_x = &r->i_data[ ( o_col + i ) * r->i_stride + r_col ];
        for( sz_t k = 0; k < r_cols; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
    }
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// Recursive block multiplication

//----------------------------------------------------------------------------------------------------------------------

static void recursive_block_mul
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_asmfx_s* m,                          sz_t m_col, sz_t m_cols,
          bmath_asmfx_s* r
)
{
    if( o_cols == BMATH_ASM_MUL_BLOCK_SIZE && m_cols == BMATH_ASM_MUL_BLOCK_SIZE )
    {
        kernel_fixed_mul( o, o_row, o_rows, o_col, m, m_col, r );
        return;
    }

    if( o_rows >= o_cols && o_rows >= m_cols && o_rows > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_rows, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_mul( o, o_row,                mid, o_col, o_cols, m, m_col, m_cols, r );
        recursive_block_mul( o, o_row + mid, o_rows - mid, o_col, o_cols, m, m_col, m_cols, r );
        return;
    }

    if( o_cols >= m_cols && o_cols > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_cols, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_mul( o, o_row, o_rows, o_col,                mid, m, m_col, m_cols, r );
        recursive_block_mul( o, o_row, o_rows, o_col + mid, o_cols - mid, m, m_col, m_cols, r );
        return;
    }

    if( m_cols > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( m_cols, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_mul( o, o_row, o_rows, o_col, o_cols, m, m_col,                mid, r );
        recursive_block_mul( o, o_row, o_rows, o_col, o_cols, m, m_col + mid, m_cols - mid, r );
        return;
    }

    kernel_flexi_mul( o, o_row, o_rows, o_col, o_cols, m, m_col, m_cols, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_block_mul_htp
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_asmfx_s* m,
          bmath_asmfx_s* r,                          sz_t r_col, sz_t r_cols
)
{
    if( o_cols == BMATH_ASM_MUL_BLOCK_SIZE && r_cols == BMATH_ASM_MUL_BLOCK_SIZE )
    {
        kernel_fixed_mul_htp( o, o_row, o_rows, o_col, m, r, r_col );
        return;
    }

    if( o_rows >= o_cols && o_rows >= r_cols && o_rows > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_rows, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_mul_htp( o, o_row,                mid, o_col, o_cols, m, r, r_col, r_cols );
        recursive_block_mul_htp( o, o_row + mid, o_rows - mid, o_col, o_cols, m, r, r_col, r_cols );
        return;
    }

    if( o_cols >= r_cols && o_cols > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_cols, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_mul_htp( o, o_row, o_rows, o_col,                mid, m, r, r_col, r_cols );
        recursive_block_mul_htp( o, o_row, o_rows, o_col + mid, o_cols - mid, m, r, r_col, r_cols );
        return;
    }

    if( r_cols > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( r_cols, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_mul_htp( o, o_row, o_rows, o_col, o_cols, m, r, r_col,                mid );
        recursive_block_mul_htp( o, o_row, o_rows, o_col, o_cols, m, r, r_col + mid, r_cols - mid );
        return;
    }

    {
        kernel_flexi_mul_htp( o, o_row, o_rows, o_col, o_cols, m, r, r_col, r_cols );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_block_htp_mul
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_asmfx_s* m,                          sz_t m_col, sz_t m_cols,
          bmath_asmfx_s* r
)
{

    if( o_rows == BMATH_ASM_MUL_BLOCK_SIZE && m_cols == BMATH_ASM_MUL_BLOCK_SIZE )
    {
        kernel_fixed_htp_mul( o, o_row, o_col, o_cols, m, m_col, r );
        return;
    }

    if( o_rows >= o_cols && o_rows >= m_cols && o_rows > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_rows, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_htp_mul( o, o_row,                mid, o_col, o_cols, m, m_col, m_cols, r );
        recursive_block_htp_mul( o, o_row + mid, o_rows - mid, o_col, o_cols, m, m_col, m_cols, r );
        return;
    }

    if( o_cols >= m_cols && o_cols > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_cols, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_htp_mul( o, o_row, o_rows, o_col,                mid, m, m_col, m_cols, r );
        recursive_block_htp_mul( o, o_row, o_rows, o_col + mid, o_cols - mid, m, m_col, m_cols, r );
        return;
    }

    if( m_cols > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( m_cols, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_htp_mul( o, o_row, o_rows, o_col, o_cols, m, m_col,                mid, r );
        recursive_block_htp_mul( o, o_row, o_rows, o_col, o_cols, m, m_col + mid, m_cols - mid, r );
        return;
    }

    kernel_flexi_htp_mul( o, o_row, o_rows, o_col, o_cols, m, m_col, m_cols, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_block_htp_mul_htp
(
    const bmath_asmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_asmfx_s* m,                          sz_t m_row, sz_t m_rows,
          bmath_asmfx_s* r
)
{
    if( o_rows == BMATH_ASM_MUL_BLOCK_SIZE && m_rows == BMATH_ASM_MUL_BLOCK_SIZE )
    {
        kernel_fixed_htp_mul_htp( o, o_row, o_col, o_cols, m, m_row, r );
        return;
    }

    if( o_cols >= o_rows && o_cols >= m_rows && o_cols > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_cols, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_htp_mul_htp( o, o_row, o_rows, o_col,                mid, m, m_row, m_rows, r );
        recursive_block_htp_mul_htp( o, o_row, o_rows, o_col + mid, o_cols - mid, m, m_row, m_rows, r );
        return;
    }

    if( o_rows >= m_rows && o_rows > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_rows, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_htp_mul_htp( o, o_row,                mid, o_col, o_cols, m, m_row, m_rows, r );
        recursive_block_htp_mul_htp( o, o_row + mid, o_rows - mid, o_col, o_cols, m, m_row, m_rows, r );
        return;
    }

    if( m_rows > BMATH_ASM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( m_rows, BMATH_ASM_MUL_BLOCK_SIZE );
        recursive_block_htp_mul_htp( o, o_row, o_rows, o_col, o_cols, m, m_row,                mid, r );
        recursive_block_htp_mul_htp( o, o_row, o_rows, o_col, o_cols, m, m_row + mid, m_rows - mid, r );
        return;
    }

    kernel_flexi_htp_mul_htp( o, o_row, o_rows, o_col, o_cols, m, m_row, m_rows, r );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// Interface

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,mul)( const bmath_asmfx_s* o, const bmath_asmfx_s* m, bmath_asmfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_asmfx_s* buf = BCATU(bmath_asmfx_s,create_shape_alike)( r );
        BCATU(bmath_asmfx_s,mul)( o, m, buf );
        BCATU(bmath_asmfx_s,copy)( r, buf );
        BCATU(bmath_asmfx_s,discard)( buf );
        return;
    }

    ASSERT( o->rows == r->rows );
    ASSERT( o->cols == m->rows );
    ASSERT( m->cols == r->cols );
    BCATU(bmath_asmfx_s,zro)( r );
    recursive_block_mul( o, 0, o->rows, 0, o->cols, m, 0, m->cols, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,mul_htp)( const bmath_asmfx_s* o, const bmath_asmfx_s* m, bmath_asmfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_asmfx_s* buf = BCATU(bmath_asmfx_s,create_shape_alike)( r );
        BCATU(bmath_asmfx_s,mul_htp)( o, m, buf );
        BCATU(bmath_asmfx_s,copy)( r, buf );
        BCATU(bmath_asmfx_s,discard)( buf );
        return;
    }

    ASSERT( o->rows == r->rows );
    ASSERT( m->rows == r->cols );
    ASSERT( o->cols == m->cols );
    BCATU(bmath_asmfx_s,zro)( r );
    recursive_block_mul_htp( o, 0, o->rows, 0, o->cols, m, r, 0, r->cols );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,htp_mul)( const bmath_asmfx_s* o, const bmath_asmfx_s* m, bmath_asmfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_asmfx_s* buf = BCATU(bmath_asmfx_s,create_shape_alike)( r );
        BCATU(bmath_asmfx_s,htp_mul)( o, m, buf );
        BCATU(bmath_asmfx_s,copy)( r, buf );
        BCATU(bmath_asmfx_s,discard)( buf );
        return;
    }

    ASSERT( o->cols == r->rows );
    ASSERT( o->rows == m->rows );
    ASSERT( m->cols == r->cols );
    BCATU(bmath_asmfx_s,zro)( r );
    recursive_block_htp_mul( o, 0, o->rows, 0, o->cols, m, 0, m->cols, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,htp_mul_htp)( const bmath_asmfx_s* o, const bmath_asmfx_s* m, bmath_asmfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_asmfx_s* buf = BCATU(bmath_asmfx_s,create_shape_alike)( r );
        BCATU(bmath_asmfx_s,htp_mul_htp)( o, m, buf );
        BCATU(bmath_asmfx_s,copy)( r, buf );
        BCATU(bmath_asmfx_s,discard)( buf );
        return;
    }

    ASSERT( m->cols == o->rows );
    ASSERT( o->cols == r->rows );
    ASSERT( m->rows == r->cols );
    BCATU(bmath_asmfx_s,zro)( r );
    recursive_block_htp_mul_htp( o, 0, o->rows, 0, o->cols, m, 0, m->rows, r );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

