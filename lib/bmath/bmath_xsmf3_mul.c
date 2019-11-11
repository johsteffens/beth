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

/** Collection of algorithms related to spliced-matrix multiplication.
 *  Blocking and microkernel strategy:
 *    - Begin recursion reducing rows-xons block size to xons==1 (rxblock)
 *    - Continue recursion rows-slos similar to row-cols in mf3  (rsblock)
 *    - Microkernels are row-slo-adpated, otherwise similar to mf3.
 */

#include "bmath_xsmf3.h"
#include "bmath_xsmf3_mul.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_fx_begin.h"

#define BMATH_XSM_MUL_BLKPK_SIZE 8
#define BMATH_XSM_MUL_BLOCK_SIZE ( BMATH_XSM_MUL_BLKPK_SIZE * P5_SIZE )

/**********************************************************************************************************************/
// Microkernels

static sz_t midof( sz_t v, const sz_t bz )
{
    return ( v > ( bz << 1 ) ) ? ( v / ( bz << 1 ) ) * bz : bz;
}

//----------------------------------------------------------------------------------------------------------------------

/// o_slos == BMATH_XSM_MUL_BLOCK_SIZE && m_slos == BMATH_XSM_MUL_BLOCK_SIZE
static void kernel_fixed_mul
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo,
    const bmath_xsmfx_s* m,                          sz_t m_xon, sz_t m_slo,
          bmath_xsmfx_s* r
)
{
#ifdef BMATH_AVX
    M5_T r_pk[ BMATH_XSM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
        {
            m_pk[ j ][ k ] = M5_LOAD( &m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + k * P5_SIZE ] );
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const fx_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
              fx_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( o_row + i ) ] + m_slo ];

        M5_T o_pk = M5_SET_ALL( o_d[ 0 ] );

        for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
        {
            o_pk = M5_SET_ALL( o_d[ j ] );
            for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
        {
            M5_STOR( r_d + k * P5_SIZE, M5_ADD( M5_LOAD( r_d + k * P5_SIZE ), r_pk[ k ] ) );
        }
    }
#else
    fx_t r_p[ BMATH_XSM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLOCK_SIZE ];
    for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) m_p[ j ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + k ];
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const fx_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
        {
            for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_d[ j ];
        }

        fx_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( o_row + i ) ] + m_slo ];
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_d[ k ] += r_p[ k ];
    }
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/// o_slos <= BMATH_XSM_MUL_BLOCK_SIZE && m_slos <= BMATH_XSM_MUL_BLOCK_SIZE
static void kernel_flexi_mul
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmfx_s* m,                          sz_t m_xon, sz_t m_slo, sz_t m_slos,
          bmath_xsmfx_s* r
)
{
#ifdef BMATH_AVX
    ASSERT( o_slos <= BMATH_XSM_MUL_BLOCK_SIZE );
    ASSERT( m_slos <= BMATH_XSM_MUL_BLOCK_SIZE );

    // notation:
    // p_fit: number of M5_t fitting into block
    // p_ful: number of M5_t required to hold all data of block (either p_fit or p_fit + 1)
    // rest:  remaining fx_t not fitting into p_fit
    const sz_t m_slos_p_fit = m_slos >> P5_SIZE_B2E;
    const sz_t m_slos_rest  = m_slos - m_slos_p_fit * P5_SIZE;
    const sz_t m_slos_p_ful = m_slos_rest > 0 ? m_slos_p_fit + 1 : m_slos_p_fit;

    M5_T r_pk[ BMATH_XSM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < o_slos; j++ )
    {
        for( sz_t k = 0; k < m_slos_p_fit; k++ )
        {
            m_pk[ j ][ k ] = M5_LOAD( &m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + k * P5_SIZE ] );
        }

        if( m_slos_rest > 0 )
        {
            m_pk[ j ][ m_slos_p_fit ] = M5_SET_ZERO();
            for( sz_t k = 0; k < m_slos_rest;  k++ )
            {
                m_pk[ j ][ m_slos_p_fit ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + m_slos_p_fit * P5_SIZE + k ];
            }
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const fx_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
              fx_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( o_row + i ) ] + m_slo ];

        for( sz_t k = 0; k < m_slos_p_ful; k++ ) r_pk[ k ] = M5_SET_ZERO();

        for( sz_t j = 0; j < o_slos; j++ )
        {
            M5_T o_pk = M5_SET_ALL( o_d[ j ] );
            for( sz_t k = 0; k < m_slos_p_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < m_slos_p_fit; k++ ) M5_STOR( r_d + k * P5_SIZE, M5_ADD( M5_LOAD( r_d + k * P5_SIZE ), r_pk[ k ] ) );

        if( m_slos_rest > 0 )
        {
            for( sz_t k = 0; k < m_slos_rest; k++ ) r_d[ m_slos_p_fit * P5_SIZE + k ] += r_pk[ m_slos_p_fit ][ k ];
        }
    }
#else
    fx_t r_p[ BMATH_XSM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLOCK_SIZE ];
    for( sz_t j = 0; j < o_slos; j++ )
    {
        for( sz_t k = 0; k < m_slos; k++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + k ];
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const fx_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
        for( sz_t k = 0; k < m_slos; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_slos; j++ )
        {
            for( sz_t k = 0; k < m_slos; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_d[ j ];
        }

        fx_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( o_row + i ) ] + m_slo ];
        for( sz_t k = 0; k < m_slos; k++ ) r_d[ k ] += r_p[ k ];
    }
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/// o_slos == BMATH_XSM_MUL_BLOCK_SIZE && m_slos == BMATH_XSM_MUL_BLOCK_SIZE
static void kernel_fixed_mul_htp
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo,
    const bmath_xsmfx_s* m,
          bmath_xsmfx_s* r,                          sz_t r_xon, sz_t r_slo
)
{
#ifdef BMATH_AVX
    M5_T r_pk[ BMATH_XSM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLKPK_SIZE ];

    for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
    {
        sz_t idx = o_xon * m->i_stride + ( r_xon * r->slos + r_slo + k * P5_SIZE );
        for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = m->v_data[ m->i_data[ idx + i ] + o_slo + j ];
            }
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const fx_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
              fx_t* r_d = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( o_row + i ) ] + r_slo ];

        M5_T o_pk = M5_SET_ALL( o_d[ 0 ] );

        for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
        {
            o_pk = M5_SET_ALL( o_d[ j ] );
            for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
        {
            M5_STOR( r_d + k * P5_SIZE, M5_ADD( M5_LOAD( r_d + k * P5_SIZE ), r_pk[ k ] ) );
        }
    }
#else
    fx_t r_p[ BMATH_XSM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ )
    {
        for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ o_xon * m->i_stride + ( r_xon * r->slos + r_slo + k ) ] + o_slo + j ];
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const fx_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
              fx_t* r_d = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( o_row + i ) ] + r_slo ];

        for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;
        for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
        {
            for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_d[ j ];
        }
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_d[ k ] += r_p[ k ];
    }
#endif
}

//----------------------------------------------------------------------------------------------------------------------

/// o_slos <= BMATH_XSM_MUL_BLOCK_SIZE && m_slos <= BMATH_XSM_MUL_BLOCK_SIZE
static void kernel_flexi_mul_htp
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmfx_s* m,
          bmath_xsmfx_s* r,                          sz_t r_xon, sz_t r_slo, sz_t r_slos
)
{
#ifdef BMATH_AVX
    ASSERT( o_slos <= BMATH_XSM_MUL_BLOCK_SIZE );
    ASSERT( r_slos <= BMATH_XSM_MUL_BLOCK_SIZE );

    const sz_t r_slos_p_fit = r_slos >> P5_SIZE_B2E;
    const sz_t r_slos_rest  = r_slos - r_slos_p_fit * P5_SIZE;
    const sz_t r_slos_p_ful = r_slos_rest > 0 ? r_slos_p_fit + 1 : r_slos_p_fit;

    M5_T r_pk[ BMATH_XSM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLKPK_SIZE ];

    for( sz_t k = 0; k < r_slos_p_fit; k++ )
    {
        sz_t idx = o_xon * m->i_stride + ( r_xon * r->slos + r_slo + k * P5_SIZE );
        for( sz_t j = 0; j < o_slos; j++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = m->v_data[ m->i_data[ idx + i ] + o_slo + j ];
            }
        }
    }

    if( r_slos_rest > 0 )
    {
        for( sz_t j = 0; j < o_slos; j++ ) m_pk[ j ][ r_slos_p_fit ] = M5_SET_ZERO();
        for( sz_t k = 0; k < r_slos_rest; k++ )
        {
            for( sz_t j = 0; j < o_slos; j++ )
            {
                m_pk[ j ][ r_slos_p_fit ][ k ] = m->v_data[ m->i_data[ o_xon * m->i_stride + ( r_xon * r->slos + r_slo + r_slos_p_fit * P5_SIZE + k ) ] + o_slo + j ];
            }
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const fx_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
              fx_t* r_d = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( o_row + i ) ] + r_slo ];

        for( sz_t k = 0; k < r_slos_p_ful; k++ ) r_pk[ k ] = M5_SET_ZERO();

        for( sz_t j = 0; j < o_slos; j++ )
        {
            M5_T o_pk = M5_SET_ALL( o_d[ j ] );
            for( sz_t k = 0; k < r_slos_p_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < r_slos_p_fit; k++ ) M5_STOR( r_d + k * P5_SIZE, M5_ADD( M5_LOAD( r_d + k * P5_SIZE ), r_pk[ k ] ) );

        if( r_slos_rest > 0 )
        {
            for( sz_t k = 0; k < r_slos_rest; k++ ) r_d[ r_slos_p_fit * P5_SIZE + k ] += r_pk[ r_slos_p_fit ][ k ];
        }
    }
#else
    fx_t r_p[ BMATH_XSM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < r_slos; k++ )
    {
        for( sz_t j = 0; j < o_slos; j++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ o_xon * m->i_stride + ( r_xon * r->slos + r_slo + k ) ] + o_slo + j ];
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const fx_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];

        for( sz_t k = 0; k < r_slos; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_slos; j++ )
        {
            for( sz_t k = 0; k < r_slos; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_d[ j ];
        }

        fx_t* r_d = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( o_row + i ) ] + r_slo ];
        for( sz_t k = 0; k < r_slos; k++ ) r_d[ k ] += r_p[ k ];
    }
#endif
}

//----------------------------------------------------------------------------------------------------------------------

/// o_rows == BMATH_XSM_MUL_BLOCK_SIZE && m_slos == BMATH_XSM_MUL_BLOCK_SIZE
static void kernel_fixed_htp_mul
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmfx_s* m,             sz_t m_xon, sz_t m_slo,
          bmath_xsmfx_s* r
)
{
#ifdef BMATH_AVX
    M5_T r_pk[ BMATH_XSM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
        {
            m_pk[ j ][ k ] = M5_LOAD( &m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_row + j ) ] + m_slo + k * P5_SIZE ] );
        }
    }

    for( sz_t i = 0; i < o_slos; i++ )
    {
        const fx_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + o_row ] + o_slo ];
        M5_T o_pk = M5_SET_ALL( o_d[ i ] );

        for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
        {
            const fx_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo ];
            M5_T o_pk = M5_SET_ALL( o_d[ i ] );
            for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        sz_t m_row = o_xon * o->slos + o_slo;
        fx_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( m_row + i ) ] + m_slo ];
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
        {
            M5_STOR( r_d + k * P5_SIZE, M5_ADD( M5_LOAD( r_d + k * P5_SIZE ), r_pk[ k ] ) );
        }
    }
#else
    fx_t r_p[ BMATH_XSM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLOCK_SIZE ];
    for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_row + j ) ] + m_slo + k ];
        }
    }

    for( sz_t i = 0; i < o_slos; i++ )
    {
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
        {
            fx_t o_v = o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo + i ];
            for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
        }

        sz_t m_row = o_xon * o->slos + o_slo;

        fx_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( m_row + i ) ] + m_slo ];
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_d[ k ] += r_p[ k ];
    }
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/// o_rows <= BMATH_XSM_MUL_BLOCK_SIZE && m_slos <= BMATH_XSM_MUL_BLOCK_SIZE
static void kernel_flexi_htp_mul
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmfx_s* m,                          sz_t m_xon, sz_t m_slo, sz_t m_slos,
          bmath_xsmfx_s* r
)
{
#ifdef BMATH_AVX
    assert( o_rows <= BMATH_XSM_MUL_BLOCK_SIZE );
    assert( m_slos <= BMATH_XSM_MUL_BLOCK_SIZE );

    const sz_t m_slos_p_fit = m_slos >> P5_SIZE_B2E;
    const sz_t m_slos_rest  = m_slos - m_slos_p_fit * P5_SIZE;
    const sz_t m_slos_p_ful = m_slos_rest > 0 ? m_slos_p_fit + 1 : m_slos_p_fit;

    M5_T r_pk[ BMATH_XSM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < o_rows; j++ )
    {
        for( sz_t k = 0; k < m_slos_p_fit; k++ )
        {
            m_pk[ j ][ k ] = M5_LOAD( &m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_row + j ) ] + m_slo + k * P5_SIZE ] );

        }

        if( m_slos_rest > 0 )
        {
            m_pk[ j ][ m_slos_p_fit ] = M5_SET_ZERO();
            for( sz_t k = 0; k < m_slos_rest;  k++ )
            {
                m_pk[ j ][ m_slos_p_fit ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_row + j ) ] + m_slo + m_slos_p_fit * P5_SIZE + k ];
            }
        }
    }

    for( sz_t i = 0; i < o_slos; i++ )
    {
        for( sz_t k = 0; k < m_slos_p_ful; k++ ) r_pk[ k ] = M5_SET_ZERO();

        for( sz_t j = 0; j < o_rows; j++ )
        {
            const fx_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo ];
            M5_T o_pk = M5_SET_ALL( o_d[ i ] );
            for( sz_t k = 0; k < m_slos_p_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        sz_t m_row = o_xon * o->slos + o_slo;
        fx_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( m_row + i ) ] + m_slo ];
        for( sz_t k = 0; k < m_slos_p_fit; k++ ) M5_STOR( r_d + k * P5_SIZE, M5_ADD( M5_LOAD( r_d + k * P5_SIZE ), r_pk[ k ] ) );

        if( m_slos_rest > 0 )
        {
            for( sz_t k = 0; k < m_slos_rest; k++ ) r_d[ m_slos_p_fit * P5_SIZE + k ] += r_pk[ m_slos_p_fit ][ k ];
        }
    }
#else
    fx_t r_p[ BMATH_XSM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLOCK_SIZE ];
    for( sz_t j = 0; j < o_rows; j++ )
    {
        for( sz_t k = 0; k < m_slos; k++ )
        {
            m_p[ j ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_row + j ) ] + m_slo + k ];
        }
    }

    for( sz_t i = 0; i < o_slos; i++ )
    {
        for( sz_t k = 0; k < m_slos; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_rows; j++ )
        {
            fx_t o_v = o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo + i ];
            for( sz_t k = 0; k < m_slos; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
        }

        sz_t m_row = o_xon * o->slos + o_slo;

        fx_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( m_row + i ) ] + m_slo ];
        for( sz_t k = 0; k < m_slos; k++ ) r_d[ k ] += r_p[ k ];
    }
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/// o_rows == BMATH_XSM_MUL_BLOCK_SIZE && m_rows == BMATH_XSM_MUL_BLOCK_SIZE
static void kernel_fixed_htp_mul_htp
(
    const bmath_xsmfx_s* o, sz_t o_row,              sz_t o_xon, sz_t o_slo,
    const bmath_xsmfx_s* m, sz_t m_row,              sz_t m_xon, sz_t m_slo,
          bmath_xsmfx_s* r, sz_t r_row, sz_t r_rows, sz_t r_xon, sz_t r_slo
)
{
#ifdef BMATH_AVX
    M5_T r_pk[ BMATH_XSM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLKPK_SIZE ];
    for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
    {
        for( sz_t i = 0; i < P5_SIZE; i++ )
        {
            fx_t* m_v = &m->v_data[ m->i_data[ m_xon * m->i_stride + ( m_row + k * P5_SIZE + i ) ] + m_slo ];
            for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ ) m_pk[ j ][ k ][ i ] = m_v[ j ];
        }
    }

    for( sz_t i = 0; i < r_rows; i++ )
    {
        fx_t o_vi = o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row ) ] + o_slo + i ];
        M5_T o_pk = M5_SET_ALL( o_vi );

        for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
        {
            fx_t o_vi = o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo + i ];
            o_pk = M5_SET_ALL( o_vi );
            for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        fx_t* r_v = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( r_row + i ) ] + r_slo ];
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLKPK_SIZE; k++ )
        {
            M5_STOR( r_v + k * P5_SIZE, M5_ADD( M5_LOAD( r_v + k * P5_SIZE ), r_pk[ k ] ) );
        }
    }
#else
    fx_t r_p[ BMATH_XSM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ )
    {
        fx_t* m_v = &m->v_data[ m->i_data[ m_xon * m->i_stride + ( m_row + k ) ] + m_slo ];
        for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ ) m_p[ j ][ k ] = m_v[ j ];
    }

    for( sz_t i = 0; i < r_rows; i++ )
    {
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;
        for( sz_t j = 0; j < BMATH_XSM_MUL_BLOCK_SIZE; j++ )
        {
            fx_t o_vi = o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo + i ];
            for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_vi;
        }

        fx_t* r_v = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( r_row + i ) ] + r_slo ];
        for( sz_t k = 0; k < BMATH_XSM_MUL_BLOCK_SIZE; k++ ) r_v[ k ] += r_p[ k ];
    }
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/// o_rows <= BMATH_XSM_MUL_BLOCK_SIZE && m_rows <= BMATH_XSM_MUL_BLOCK_SIZE
static void kernel_flexi_htp_mul_htp
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo,
    const bmath_xsmfx_s* m, sz_t m_row, sz_t m_rows, sz_t m_xon, sz_t m_slo,
          bmath_xsmfx_s* r, sz_t r_row, sz_t r_rows, sz_t r_xon, sz_t r_slo
)
{
#ifdef BMATH_AVX
    M5_T r_pk[ BMATH_XSM_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLKPK_SIZE ];
    const sz_t m_rows_p_fit = m_rows >> P5_SIZE_B2E;
    const sz_t m_rows_rest  = m_rows - m_rows_p_fit * P5_SIZE;
    const sz_t m_rows_p_ful = m_rows_rest > 0 ? m_rows_p_fit + 1 : m_rows_p_fit;

    for( sz_t k = 0; k < m_rows_p_fit; k++ )
    {
        for( sz_t i = 0; i < P5_SIZE; i++ )
        {
            fx_t* m_v = &m->v_data[ m->i_data[ m_xon * m->i_stride + ( m_row + k * P5_SIZE + i ) ] + m_slo ];
            for( sz_t j = 0; j < o_rows; j++ ) m_pk[ j ][ k ][ i ] = m_v[ j ];
        }
    }

    if( m_rows_rest > 0 )
    {
        for( sz_t j = 0; j < m_rows; j++ ) m_pk[ j ][ m_rows_p_fit ] = M5_SET_ZERO();
        for( sz_t i = 0; i < m_rows_rest; i++ )
        {
            fx_t* m_v = &m->v_data[ m->i_data[ m_xon * m->i_stride + ( m_row + m_rows_p_fit * P5_SIZE + i ) ] + m_slo ];
            for( sz_t j = 0; j < o_rows; j++ ) m_pk[ j ][ m_rows_p_fit ][ i ] = m_v[ j ];
        }
    }

    for( sz_t i = 0; i < r_rows; i++ )
    {
        fx_t o_vi = o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row ) ] + o_slo + i ];
        M5_T o_pk = M5_SET_ALL( o_vi );

        for( sz_t k = 0; k < m_rows_p_ful; k++ ) r_pk[ k ] = M5_SET_ZERO();

        for( sz_t j = 0; j < o_rows; j++ )
        {
            fx_t o_vi = o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo + i ];
            o_pk = M5_SET_ALL( o_vi );
            for( sz_t k = 0; k < m_rows_p_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        fx_t* r_v = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( r_row + i ) ] + r_slo ];
        for( sz_t k = 0; k < m_rows_p_fit; k++ )
        {
            M5_STOR( r_v + k * P5_SIZE, M5_ADD( M5_LOAD( r_v + k * P5_SIZE ), r_pk[ k ] ) );
        }
        for( sz_t i = 0; i < m_rows_rest; i++ ) r_v[ m_rows_p_fit * P5_SIZE + i ] += r_pk[ m_rows_p_fit ][ i ];
    }
#else
    fx_t r_p[ BMATH_XSM_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_XSM_MUL_BLOCK_SIZE ][ BMATH_XSM_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < m_rows; k++ )
    {
        fx_t* m_v = &m->v_data[ m->i_data[ m_xon * m->i_stride + ( m_row + k ) ] + m_slo ];
        for( sz_t j = 0; j < o_rows; j++ ) m_p[ j ][ k ] = m_v[ j ];
    }

    for( sz_t i = 0; i < r_rows; i++ )
    {
        for( sz_t k = 0; k < m_rows; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_rows; j++ )
        {
            fx_t o_vi = o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo + i ];
            for( sz_t k = 0; k < m_rows; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_vi;
        }

        fx_t* r_v = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( r_row + i ) ] + r_slo ];
        for( sz_t k = 0; k < m_rows; k++ ) r_v[ k ] += r_p[ k ];
    }
#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// Recursive block multiplication

//----------------------------------------------------------------------------------------------------------------------

static void recursive_rsblock_mul
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmfx_s* m,                          sz_t m_xon, sz_t m_slo, sz_t m_slos,
          bmath_xsmfx_s* r
)
{
    if( o_slos == BMATH_XSM_MUL_BLOCK_SIZE && m_slos == BMATH_XSM_MUL_BLOCK_SIZE )
    {
        kernel_fixed_mul( o, o_row, o_rows, o_xon, o_slo, m, m_xon, m_slo, r );
        return;
    }

    if( o_rows >= o_slos && o_rows >= m_slos && o_rows > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_rows, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_mul( o, o_row,                mid, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
        recursive_rsblock_mul( o, o_row + mid, o_rows - mid, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
        return;
    }

    if( o_slos >= m_slos && o_slos > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_slos, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_mul( o, o_row, o_rows, o_xon, o_slo,                mid, m, m_xon, m_slo, m_slos, r );
        recursive_rsblock_mul( o, o_row, o_rows, o_xon, o_slo + mid, o_slos - mid, m, m_xon, m_slo, m_slos, r );
        return;
    }

    if( m_slos > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( m_slos, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_mul( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo,                mid, r );
        recursive_rsblock_mul( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo + mid, m_slos - mid, r );
        return;
    }

    kernel_flexi_mul( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_rxblock_mul
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_xons,
    const bmath_xsmfx_s* m,                          sz_t m_xon, sz_t m_xons,
          bmath_xsmfx_s* r
)
{
    if( o_rows >= o_xons * o->slos && o_rows >= m_xons * m->slos && o_rows > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_rows, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rxblock_mul( o, o_row,                mid, o_xon, o_xons, m, m_xon, m_xons, r );
        recursive_rxblock_mul( o, o_row + mid, o_rows - mid, o_xon, o_xons, m, m_xon, m_xons, r );
        return;
    }

    if( o_xons >= m_xons && o_xons > 1 )
    {
        sz_t mid = midof( o_xons, 1 );
        recursive_rxblock_mul( o, o_row, o_rows, o_xon,                mid, m, m_xon, m_xons, r );
        recursive_rxblock_mul( o, o_row, o_rows, o_xon + mid, o_xons - mid, m, m_xon, m_xons, r );
        return;
    }

    if( m_xons > 1 )
    {
        sz_t mid = midof( m_xons, 1 );
        recursive_rxblock_mul( o, o_row, o_rows, o_xon, o_xons, m, m_xon,                mid, r );
        recursive_rxblock_mul( o, o_row, o_rows, o_xon, o_xons, m, m_xon + mid, m_xons - mid, r );
        return;
    }

    recursive_rsblock_mul( o, o_row, o_rows, o_xon, 0, o->slos, m, m_xon, 0, m->slos, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_rsblock_mul_htp
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmfx_s* m,
          bmath_xsmfx_s* r,                          sz_t r_xon, sz_t r_slo, sz_t r_slos
)
{
    if( o_slos == BMATH_XSM_MUL_BLOCK_SIZE && r_slos == BMATH_XSM_MUL_BLOCK_SIZE )
    {
        kernel_fixed_mul_htp( o, o_row, o_rows, o_xon, o_slo, m, r, r_xon, r_slo );
        return;
    }

    if( o_rows >= o_slos && o_rows >= r_slos && o_rows > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_rows, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_mul_htp( o, o_row,                mid, o_xon, o_slo, o_slos, m, r, r_xon, r_slo, r_slos );
        recursive_rsblock_mul_htp( o, o_row + mid, o_rows - mid, o_xon, o_slo, o_slos, m, r, r_xon, r_slo, r_slos );
        return;
    }

    if( o_slos >= r_slos && o_slos > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_slos, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_mul_htp( o, o_row, o_rows, o_xon, o_slo,                mid, m, r, r_xon, r_slo, r_slos );
        recursive_rsblock_mul_htp( o, o_row, o_rows, o_xon, o_slo + mid, o_slos - mid, m, r, r_xon, r_slo, r_slos );
        return;
    }

    if( r_slos > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( r_slos, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_mul_htp( o, o_row, o_rows, o_xon, o_slo, o_slos, m, r, r_xon, r_slo,                mid );
        recursive_rsblock_mul_htp( o, o_row, o_rows, o_xon, o_slo, o_slos, m, r, r_xon, r_slo + mid, r_slos - mid );
        return;
    }

    kernel_flexi_mul_htp( o, o_row, o_rows, o_xon, o_slo, o_slos, m, r, r_xon, r_slo, r_slos );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_rxblock_mul_htp
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_xons,
    const bmath_xsmfx_s* m,
          bmath_xsmfx_s* r,                          sz_t r_xon, sz_t r_xons
)
{
    if( o_rows >= o_xons * o->slos && o_rows >= r_xons * r->slos && o_rows > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_rows, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rxblock_mul_htp( o, o_row,                mid, o_xon, o_xons, m, r, r_xon, r_xons );
        recursive_rxblock_mul_htp( o, o_row + mid, o_rows - mid, o_xon, o_xons, m, r, r_xon, r_xons );
        return;
    }

    if( o_xons >= r_xons && o_xons > 1 )
    {
        sz_t mid = midof( o_xons, 1 );
        recursive_rxblock_mul_htp( o, o_row, o_rows, o_xon,                mid, m, r, r_xon, r_xons );
        recursive_rxblock_mul_htp( o, o_row, o_rows, o_xon + mid, o_xons - mid, m, r, r_xon, r_xons );
        return;
    }

    if( r_xons > 1 )
    {
        sz_t mid = midof( r_xons, 1 );
        recursive_rxblock_mul_htp( o, o_row, o_rows, o_xon, o_xons, m, r, r_xon,                mid );
        recursive_rxblock_mul_htp( o, o_row, o_rows, o_xon, o_xons, m, r, r_xon + mid, r_xons - mid );
        return;
    }

    recursive_rsblock_mul_htp( o, o_row, o_rows, o_xon, 0, o->slos, m, r, r_xon, 0, r->slos );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_rsblock_htp_mul
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmfx_s* m,                          sz_t m_xon, sz_t m_slo, sz_t m_slos,
          bmath_xsmfx_s* r
)
{

    if( o_rows == BMATH_XSM_MUL_BLOCK_SIZE && m_slos == BMATH_XSM_MUL_BLOCK_SIZE )
    {
        kernel_fixed_htp_mul( o, o_row, o_xon, o_slo, o_slos, m, m_xon, m_slo, r );
        return;
    }

    if( o_rows >= o_slos && o_rows >= m_slos && o_rows > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_rows, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_htp_mul( o, o_row,                mid, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
        recursive_rsblock_htp_mul( o, o_row + mid, o_rows - mid, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
        return;
    }

    if( o_slos >= m_slos && o_slos > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_slos, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_htp_mul( o, o_row, o_rows, o_xon, o_slo,                mid, m, m_xon, m_slo, m_slos, r );
        recursive_rsblock_htp_mul( o, o_row, o_rows, o_xon, o_slo + mid, o_slos - mid, m, m_xon, m_slo, m_slos, r );
        return;
    }

    if( m_slos > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( m_slos, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_htp_mul( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo,                mid, r );
        recursive_rsblock_htp_mul( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo + mid, m_slos - mid, r );
        return;
    }

    kernel_flexi_htp_mul( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_rxblock_htp_mul
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_xons,
    const bmath_xsmfx_s* m,                          sz_t m_xon, sz_t m_xons,
          bmath_xsmfx_s* r
)
{
    if( o_rows >= o_xons * o->slos && o_rows >= m_xons * m->slos && o_rows > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_rows, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rxblock_htp_mul( o, o_row,                mid, o_xon, o_xons, m, m_xon, m_xons, r );
        recursive_rxblock_htp_mul( o, o_row + mid, o_rows - mid, o_xon, o_xons, m, m_xon, m_xons, r );
        return;
    }

    if( o_xons >= m_xons && o_xons > 1 )
    {
        sz_t mid = midof( o_xons, 1 );
        recursive_rxblock_htp_mul( o, o_row, o_rows, o_xon,                mid, m, m_xon, m_xons, r );
        recursive_rxblock_htp_mul( o, o_row, o_rows, o_xon + mid, o_xons - mid, m, m_xon, m_xons, r );
        return;
    }

    if( m_xons > 1 )
    {
        sz_t mid = midof( m_xons, 1 );
        recursive_rxblock_htp_mul( o, o_row, o_rows, o_xon, o_xons, m, m_xon,                mid, r );
        recursive_rxblock_htp_mul( o, o_row, o_rows, o_xon, o_xons, m, m_xon + mid, m_xons - mid, r );
        return;
    }

    recursive_rsblock_htp_mul( o, o_row, o_rows, o_xon, 0, o->slos, m, m_xon, 0, m->slos, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_rsblock_htp_mul_htp
(
    const bmath_xsmfx_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo,
    const bmath_xsmfx_s* m, sz_t m_row, sz_t m_rows, sz_t m_xon, sz_t m_slo,
          bmath_xsmfx_s* r, sz_t r_row, sz_t r_rows, sz_t r_xon, sz_t r_slo
)
{
    if( o_rows == BMATH_XSM_MUL_BLOCK_SIZE && m_rows == BMATH_XSM_MUL_BLOCK_SIZE )
    {
        kernel_fixed_htp_mul_htp( o, o_row,         o_xon, o_slo,
                                  m, m_row,         m_xon, m_slo,
                                  r, r_row, r_rows, r_xon, r_slo );
        return;
    }

    if( r_rows >= m_rows && r_rows >= o_rows && r_rows > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( r_rows, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_htp_mul_htp( o, o_row, o_rows, o_xon, o_slo      , m, m_row, m_rows, m_xon, m_slo, r, r_row,                mid, r_xon, r_slo );
        recursive_rsblock_htp_mul_htp( o, o_row, o_rows, o_xon, o_slo + mid, m, m_row, m_rows, m_xon, m_slo, r, r_row + mid, r_rows - mid, r_xon, r_slo );
        return;
    }

    if( o_rows >= m_rows && o_rows > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_rows, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_htp_mul_htp( o, o_row,                mid, o_xon, o_slo, m, m_row, m_rows, m_xon, m_slo,       r, r_row, r_rows, r_xon, r_slo );
        recursive_rsblock_htp_mul_htp( o, o_row + mid, o_rows - mid, o_xon, o_slo, m, m_row, m_rows, m_xon, m_slo + mid, r, r_row, r_rows, r_xon, r_slo );
        return;
    }

    if( m_rows > BMATH_XSM_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( m_rows, BMATH_XSM_MUL_BLOCK_SIZE );
        recursive_rsblock_htp_mul_htp( o, o_row, o_rows, o_xon, o_slo, m, m_row,                mid, m_xon, m_slo, r, r_row, r_rows, r_xon, r_slo       );
        recursive_rsblock_htp_mul_htp( o, o_row, o_rows, o_xon, o_slo, m, m_row + mid, m_rows - mid, m_xon, m_slo, r, r_row, r_rows, r_xon, r_slo + mid );
        return;
    }

    kernel_flexi_htp_mul_htp( o, o_row, o_rows, o_xon, o_slo,
                              m, m_row, m_rows, m_xon, m_slo,
                              r, r_row, r_rows, r_xon, r_slo );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_rxblock_htp_mul_htp
(
    const bmath_xsmfx_s* o, sz_t o_xon, sz_t o_xons,
    const bmath_xsmfx_s* m, sz_t m_xon, sz_t m_xons,
          bmath_xsmfx_s* r, sz_t r_xon, sz_t r_xons
)
{
    if( r_xons >= o_xons && r_xons >= m_xons && r_xons > 1 )
    {
        sz_t mid = midof( r_xons, 1 );
        recursive_rxblock_htp_mul_htp( o, o_xon, o_xons, m, m_xon, m_xons, r, r_xon,                mid );
        recursive_rxblock_htp_mul_htp( o, o_xon, o_xons, m, m_xon, m_xons, r, r_xon + mid, r_xons - mid );
        return;
    }

    if( o_xons >= m_xons && o_xons > 1 )
    {
        sz_t mid = midof( o_xons, 1 );
        recursive_rxblock_htp_mul_htp( o, o_xon,                mid, m, m_xon, m_xons, r, r_xon, r_xons );
        recursive_rxblock_htp_mul_htp( o, o_xon + mid, o_xons - mid, m, m_xon, m_xons, r, r_xon, r_xons );
        return;
    }

    if( m_xons > 1 )
    {
        sz_t mid = midof( m_xons, 1 );
        recursive_rxblock_htp_mul_htp( o, o_xon, o_xons, m, m_xon,                mid, r, r_xon, r_xons );
        recursive_rxblock_htp_mul_htp( o, o_xon, o_xons, m, m_xon + mid, m_xons - mid, r, r_xon, r_xons );
        return;
    }

    sz_t o_row  = m_xon  * m->slos;
    sz_t o_rows = m_xons * m->slos;
    sz_t m_row  = r_xon  * r->slos;
    sz_t m_rows = r_xons * r->slos;
    sz_t r_row  = o_xon  * o->slos;
    sz_t r_rows = o_xons * o->slos;

    recursive_rsblock_htp_mul_htp( o, o_row, o_rows, o_xon, 0,
                                   m, m_row, m_rows, m_xon, 0,
                                   r, r_row, r_rows, r_xon, 0 );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// Interface

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_xsmfx_s,mul)( const bmath_xsmfx_s* o, const bmath_xsmfx_s* m, bmath_xsmfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_xsmfx_s* buf = BCATU(bmath_xsmfx_s,create_shape_alike)( r );
        BCATU(bmath_xsmfx_s,mul)( o, m, buf );
        BCATU(bmath_xsmfx_s,copy)( r, buf );
        BCATU(bmath_xsmfx_s,discard)( buf );
        return;
    }
    ASSERT( o->rows           == r->rows );
    ASSERT( o->xons * o->slos == m->rows );
    ASSERT( m->xons           == r->xons );
    ASSERT( m->slos           == r->slos );
    BCATU(bmath_xsmfx_s,zro)( r );
    recursive_rxblock_mul( o, 0, o->rows, 0, o->xons, m, 0, m->xons, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_xsmfx_s,mul_htp)( const bmath_xsmfx_s* o, const bmath_xsmfx_s* m, bmath_xsmfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_xsmfx_s* buf = BCATU(bmath_xsmfx_s,create_shape_alike)( r );
        BCATU(bmath_xsmfx_s,mul_htp)( o, m, buf );
        BCATU(bmath_xsmfx_s,copy)( r, buf );
        BCATU(bmath_xsmfx_s,discard)( buf );
        return;
    }
    ASSERT( o->rows == r->rows );
    ASSERT( m->rows == r->xons * r->slos );
    ASSERT( o->xons == m->xons );
    ASSERT( o->slos == m->slos );
    BCATU(bmath_xsmfx_s,zro)( r );
    recursive_rxblock_mul_htp( o, 0, o->rows, 0, o->xons, m, r, 0, r->xons );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_xsmfx_s,htp_mul)( const bmath_xsmfx_s* o, const bmath_xsmfx_s* m, bmath_xsmfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_xsmfx_s* buf = BCATU(bmath_xsmfx_s,create_shape_alike)( r );
        BCATU(bmath_xsmfx_s,htp_mul)( o, m, buf );
        BCATU(bmath_xsmfx_s,copy)( r, buf );
        BCATU(bmath_xsmfx_s,discard)( buf );
        return;
    }
    ASSERT( o->xons * o->slos == r->rows );
    ASSERT( o->rows           == m->rows );
    ASSERT( m->xons           == r->xons );
    ASSERT( m->slos           == r->slos );
    BCATU(bmath_xsmfx_s,zro)( r );
    recursive_rxblock_htp_mul( o, 0, o->rows, 0, o->xons, m, 0, m->xons, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_xsmfx_s,htp_mul_htp)( const bmath_xsmfx_s* o, const bmath_xsmfx_s* m, bmath_xsmfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_xsmfx_s* buf = BCATU(bmath_xsmfx_s,create_shape_alike)( r );
        BCATU(bmath_xsmfx_s,htp_mul_htp)( o, m, buf );
        BCATU(bmath_xsmfx_s,copy)( r, buf );
        BCATU(bmath_xsmfx_s,discard)( buf );
        return;
    }
    ASSERT( m->xons * m->slos == o->rows );
    ASSERT( o->xons * o->slos == r->rows );
    ASSERT( r->xons * r->slos == m->rows );

    BCATU(bmath_xsmfx_s,zro)( r );
    recursive_rxblock_htp_mul_htp( o, 0, o->xons, m, 0, m->xons, r, 0, r->xons );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
