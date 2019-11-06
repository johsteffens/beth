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

#define BMATH_MUL_SX_BLOCK_SIZE 32
#define BMATH_MUL_SX_BLKP4_SIZE ( BMATH_MUL_SX_BLOCK_SIZE >> 2 )

#ifdef TYPEOF_bmath_xsmf3_s

/**********************************************************************************************************************/
// mul

static sz_t mf3_sx_midof( sz_t v, const sz_t bz )
{
    return ( v > ( bz << 1 ) ) ? ( v / ( bz << 1 ) ) * bz : bz;
}

//----------------------------------------------------------------------------------------------------------------------

#ifdef BMATH_AVX
/** smul: Fixed size AVX-Microkernel
 *  Requirement: o_slos == BMATH_MUL_SX_BLOCK_SIZE && m_slos == BMATH_MUL_SX_BLOCK_SIZE
 */
static void bmath_xsmf3_s_mul_rsblock_avx_fix_kernel
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo,
    const bmath_xsmf3_s* m,                          sz_t m_xon, sz_t m_slo,
          bmath_xsmf3_s* r
)
{
    assert( ( BMATH_MUL_SX_BLOCK_SIZE & 3 ) == 0 );

    __m256d r_p4[ BMATH_MUL_SX_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLKP4_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
        {
            m_p4[ j ][ k ] = _mm256_loadu_pd( &m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + k * 4 ] );
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
              f3_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( o_row + i ) ] + m_slo ];

        __m256d o_p4 = _mm256_set1_pd( o_d[ 0 ] );

        for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( o_d[ j ] );
            for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
        {
            _mm256_storeu_pd( r_d + k * 4, _mm256_add_pd( _mm256_loadu_pd( r_d + k * 4 ), r_p4[ k ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Flexible AVX-Microkernel
 *  Requirement: o_slos <= BMATH_MUL_SX_BLOCK_SIZE && m_slos <= BMATH_MUL_SX_BLOCK_SIZE
 */
static void bmath_xsmf3_s_mul_rsblock_avx_flex_kernel
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmf3_s* m,                          sz_t m_xon, sz_t m_slo, sz_t m_slos,
          bmath_xsmf3_s* r
)
{
    ASSERT( o_slos <= BMATH_MUL_SX_BLOCK_SIZE );
    ASSERT( m_slos <= BMATH_MUL_SX_BLOCK_SIZE );

    const sz_t m_slos4l = m_slos >> 2;
    const sz_t m_slosr  = m_slos - m_slos4l * 4;
    const sz_t m_slos4p = m_slosr > 0 ? m_slos4l + 1 : m_slos4l;

    __m256d r_p4[ BMATH_MUL_SX_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLKP4_SIZE ];

    for( sz_t j = 0; j < o_slos; j++ )
    {
        for( sz_t k = 0; k < m_slos4l; k++ )
        {
            m_p4[ j ][ k ] = _mm256_loadu_pd( &m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + k * 4 ] );
        }

        if( m_slosr > 0 )
        {
            m_p4[ j ][ m_slos4l ] = _mm256_set1_pd( 0 );
            for( sz_t k = 0; k < m_slosr;  k++ )
            {
                m_p4[ j ][ m_slos4l ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + m_slos4l * 4 + k ];
            }
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
              f3_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( o_row + i ) ] + m_slo ];

        for( sz_t k = 0; k < m_slos4p; k++ ) r_p4[ k ] = _mm256_set1_pd( 0 );

        for( sz_t j = 0; j < o_slos; j++ )
        {
            __m256d o_p4 = _mm256_set1_pd( o_d[ j ] );
            for( sz_t k = 0; k < m_slos4p; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < m_slos4l; k++ ) _mm256_storeu_pd( r_d + k * 4, _mm256_add_pd( _mm256_loadu_pd( r_d + k * 4 ), r_p4[ k ] ) );

        if( m_slosr > 0 )
        {
            for( sz_t k = 0; k < m_slosr; k++ ) r_d[ m_slos4l * 4 + k ] += r_p4[ m_slos4l ][ k ];
        }
    }
}

#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

static void bmath_xsmf3_s_mul_rsblock
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmf3_s* m,                          sz_t m_xon, sz_t m_slo, sz_t m_slos,
          bmath_xsmf3_s* r
)
{
    if( o_slos == BMATH_MUL_SX_BLOCK_SIZE && m_slos == BMATH_MUL_SX_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_xsmf3_s_mul_rsblock_avx_fix_kernel( o, o_row, o_rows, o_xon, o_slo, m, m_xon, m_slo, r );
        #else
            f3_t r_p[ BMATH_MUL_SX_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLOCK_SIZE ];
            for( sz_t j = 0; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
            {
                for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ ) m_p[ j ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + k ];
            }

            for( sz_t i = 0; i < o_rows; i++ )
            {
                const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
                for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
                {
                    for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_d[ j ];
                }

                f3_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( o_row + i ) ] + m_slo ];
                for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ ) r_d[ k ] += r_p[ k ];
            }
        #endif // BMATH_AVX
        return;
    }

    if( o_rows >= o_slos && o_rows >= m_slos && o_rows > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( o_rows, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_mul_rsblock( o, o_row,                mid, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
        bmath_xsmf3_s_mul_rsblock( o, o_row + mid, o_rows - mid, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
        return;
    }

    if( o_slos >= m_slos && o_slos > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( o_slos, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_mul_rsblock( o, o_row, o_rows, o_xon, o_slo,                mid, m, m_xon, m_slo, m_slos, r );
        bmath_xsmf3_s_mul_rsblock( o, o_row, o_rows, o_xon, o_slo + mid, o_slos - mid, m, m_xon, m_slo, m_slos, r );
        return;
    }

    if( m_slos > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( m_slos, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_mul_rsblock( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo,                mid, r );
        bmath_xsmf3_s_mul_rsblock( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo + mid, m_slos - mid, r );
        return;
    }

    #ifdef BMATH_AVX
        bmath_xsmf3_s_mul_rsblock_avx_flex_kernel( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
    #else
        f3_t r_p[ BMATH_MUL_SX_BLOCK_SIZE ];
        f3_t m_p[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLOCK_SIZE ];
        for( sz_t j = 0; j < o_slos; j++ )
        {
            for( sz_t k = 0; k < m_slos; k++ )
            {
                m_p[ j ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + k ];
            }
        }

        for( sz_t i = 0; i < o_rows; i++ )
        {
            const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
            for( sz_t k = 0; k < m_slos; k++ ) r_p[ k ] = 0;

            for( sz_t j = 0; j < o_slos; j++ )
            {
                for( sz_t k = 0; k < m_slos; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_d[ j ];
            }

            f3_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( o_row + i ) ] + m_slo ];
            for( sz_t k = 0; k < m_slos; k++ ) r_d[ k ] += r_p[ k ];
        }
    #endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_xsmf3_s_mul_rxblock
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_xons,
    const bmath_xsmf3_s* m,                          sz_t m_xon, sz_t m_xons,
          bmath_xsmf3_s* r
)
{
    if( o_rows >= o_xons * o->slos && o_rows >= m_xons * m->slos && o_rows > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( o_rows, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_mul_rxblock( o, o_row,                mid, o_xon, o_xons, m, m_xon, m_xons, r );
        bmath_xsmf3_s_mul_rxblock( o, o_row + mid, o_rows - mid, o_xon, o_xons, m, m_xon, m_xons, r );
        return;
    }

    if( o_xons >= m_xons && o_xons > 1 )
    {
        sz_t mid = mf3_sx_midof( o_xons, 1 );
        bmath_xsmf3_s_mul_rxblock( o, o_row, o_rows, o_xon,                mid, m, m_xon, m_xons, r );
        bmath_xsmf3_s_mul_rxblock( o, o_row, o_rows, o_xon + mid, o_xons - mid, m, m_xon, m_xons, r );
        return;
    }

    if( m_xons > 1 )
    {
        sz_t mid = mf3_sx_midof( m_xons, 1 );
        bmath_xsmf3_s_mul_rxblock( o, o_row, o_rows, o_xon, o_xons, m, m_xon,                mid, r );
        bmath_xsmf3_s_mul_rxblock( o, o_row, o_rows, o_xon, o_xons, m, m_xon + mid, m_xons - mid, r );
        return;
    }

    bmath_xsmf3_s_mul_rsblock( o, o_row, o_rows, o_xon, 0, o->slos, m, m_xon, 0, m->slos, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_mul( const bmath_xsmf3_s* o, const bmath_xsmf3_s* m, bmath_xsmf3_s* r )
{
    ASSERT( o->rows           == r->rows );
    ASSERT( o->xons * o->slos == m->rows );
    ASSERT( m->xons           == r->xons );
    ASSERT( m->slos           == r->slos );
    bmath_xsmf3_s_zro( r );
    bmath_xsmf3_s_mul_rxblock( o, 0, o->rows, 0, o->xons, m, 0, m->xons, r );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// mul_htp

#ifdef BMATH_AVX
/** smul: Fixed size AVX-Microkernel
 *  Requirement: o_slos == BMATH_MUL_SX_BLOCK_SIZE && m_slos == BMATH_MUL_SX_BLOCK_SIZE
 */
static void bmath_xsmf3_s_mul_htp_rsblock_avx_fix_kernel
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo,
    const bmath_xsmf3_s* m,
          bmath_xsmf3_s* r,                          sz_t r_xon, sz_t r_slo
)
{
    assert( ( BMATH_MUL_SX_BLOCK_SIZE & 3 ) == 0 );

    __m256d r_p4[ BMATH_MUL_SX_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLKP4_SIZE ];

    for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
    {
        sz_t idx = o_xon * m->i_stride + ( r_xon * r->slos + r_slo + k * 4 );
        for( sz_t j = 0; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
        {
            m_p4[ j ][ k ][ 0 ] = m->v_data[ m->i_data[ idx + 0 ] + o_slo + j ];
            m_p4[ j ][ k ][ 1 ] = m->v_data[ m->i_data[ idx + 1 ] + o_slo + j ];
            m_p4[ j ][ k ][ 2 ] = m->v_data[ m->i_data[ idx + 2 ] + o_slo + j ];
            m_p4[ j ][ k ][ 3 ] = m->v_data[ m->i_data[ idx + 3 ] + o_slo + j ];
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
              f3_t* r_d = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( o_row + i ) ] + r_slo ];

        __m256d o_p4 = _mm256_set1_pd( o_d[ 0 ] );

        for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( o_d[ j ] );
            for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
        {
            _mm256_storeu_pd( r_d + k * 4, _mm256_add_pd( _mm256_loadu_pd( r_d + k * 4 ), r_p4[ k ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Flexible AVX-Microkernel
 *  Requirement: o_slos <= BMATH_MUL_SX_BLOCK_SIZE && m_slos <= BMATH_MUL_SX_BLOCK_SIZE
 */
static void bmath_xsmf3_s_mul_htp_rsblock_avx_flex_kernel
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmf3_s* m,
          bmath_xsmf3_s* r,                          sz_t r_xon, sz_t r_slo, sz_t r_slos
)
{
    ASSERT( o_slos <= BMATH_MUL_SX_BLOCK_SIZE );
    ASSERT( r_slos <= BMATH_MUL_SX_BLOCK_SIZE );

    const sz_t r_slos4l = r_slos >> 2;
    const sz_t r_slosr  = r_slos - r_slos4l * 4;
    const sz_t r_slos4p = r_slosr > 0 ? r_slos4l + 1 : r_slos4l;

    __m256d r_p4[ BMATH_MUL_SX_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLKP4_SIZE ];

    for( sz_t k = 0; k < r_slos4l; k++ )
    {
        sz_t idx = o_xon * m->i_stride + ( r_xon * r->slos + r_slo + k * 4 );
        for( sz_t j = 0; j < o_slos; j++ )
        {
            m_p4[ j ][ k ][ 0 ] = m->v_data[ m->i_data[ idx + 0 ] + o_slo + j ];
            m_p4[ j ][ k ][ 1 ] = m->v_data[ m->i_data[ idx + 1 ] + o_slo + j ];
            m_p4[ j ][ k ][ 2 ] = m->v_data[ m->i_data[ idx + 2 ] + o_slo + j ];
            m_p4[ j ][ k ][ 3 ] = m->v_data[ m->i_data[ idx + 3 ] + o_slo + j ];
        }
    }

    if( r_slosr > 0 )
    {
        for( sz_t j = 0; j < o_slos; j++ ) m_p4[ j ][ r_slos4l ] = _mm256_set1_pd( 0 );
        for( sz_t k = 0; k < r_slosr; k++ )
        {
            for( sz_t j = 0; j < o_slos; j++ )
            {
                m_p4[ j ][ r_slos4l ][ k ] = m->v_data[ m->i_data[ o_xon * m->i_stride + ( r_xon * r->slos + r_slo + r_slos4l * 4 + k ) ] + o_slo + j ];
            }
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
              f3_t* r_d = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( o_row + i ) ] + r_slo ];

        for( sz_t k = 0; k < r_slos4p; k++ ) r_p4[ k ] = _mm256_set1_pd( 0 );

        for( sz_t j = 0; j < o_slos; j++ )
        {
            __m256d o_p4 = _mm256_set1_pd( o_d[ j ] );
            for( sz_t k = 0; k < r_slos4p; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < r_slos4l; k++ ) _mm256_storeu_pd( r_d + k * 4, _mm256_add_pd( _mm256_loadu_pd( r_d + k * 4 ), r_p4[ k ] ) );

        if( r_slosr > 0 )
        {
            for( sz_t k = 0; k < r_slosr; k++ ) r_d[ r_slos4l * 4 + k ] += r_p4[ r_slos4l ][ k ];
        }
    }
}

#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

static void bmath_xsmf3_s_mul_htp_rsblock
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmf3_s* m,
          bmath_xsmf3_s* r,                          sz_t r_xon, sz_t r_slo, sz_t r_slos
)
{
    if( o_slos == BMATH_MUL_SX_BLOCK_SIZE && r_slos == BMATH_MUL_SX_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_xsmf3_s_mul_htp_rsblock_avx_fix_kernel( o, o_row, o_rows, o_xon, o_slo, m, r, r_xon, r_slo );
        #else
            f3_t r_p[ BMATH_MUL_SX_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLOCK_SIZE ];
            for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ )
            {
                for( sz_t j = 0; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
                {
                    m_p[ j ][ k ] = m->v_data[ m->i_data[ o_xon * m->i_stride + ( r_xon * r->slos + r_slo + k ) ] + o_slo + j ];
                }
            }

            for( sz_t i = 0; i < o_rows; i++ )
            {
                const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
                      f3_t* r_d = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( o_row + i ) ] + r_slo ];

                for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ ) r_p[ k ] = 0;
                for( sz_t j = 0; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
                {
                    for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_d[ j ];
                }
                for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ ) r_d[ k ] += r_p[ k ];
            }
        #endif
        return;
    }

    if( o_rows >= o_slos && o_rows >= r_slos && o_rows > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( o_rows, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_mul_htp_rsblock( o, o_row,                mid, o_xon, o_slo, o_slos, m, r, r_xon, r_slo, r_slos );
        bmath_xsmf3_s_mul_htp_rsblock( o, o_row + mid, o_rows - mid, o_xon, o_slo, o_slos, m, r, r_xon, r_slo, r_slos );
        return;
    }

    if( o_slos >= r_slos && o_slos > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( o_slos, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_mul_htp_rsblock( o, o_row, o_rows, o_xon, o_slo,                mid, m, r, r_xon, r_slo, r_slos );
        bmath_xsmf3_s_mul_htp_rsblock( o, o_row, o_rows, o_xon, o_slo + mid, o_slos - mid, m, r, r_xon, r_slo, r_slos );
        return;
    }

    if( r_slos > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( r_slos, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_mul_htp_rsblock( o, o_row, o_rows, o_xon, o_slo, o_slos, m, r, r_xon, r_slo,                mid );
        bmath_xsmf3_s_mul_htp_rsblock( o, o_row, o_rows, o_xon, o_slo, o_slos, m, r, r_xon, r_slo + mid, r_slos - mid );
        return;
    }

    {
        #ifdef BMATH_AVX
            bmath_xsmf3_s_mul_htp_rsblock_avx_flex_kernel( o, o_row, o_rows, o_xon, o_slo, o_slos, m, r, r_xon, r_slo, r_slos );
        #else
            f3_t r_p[ BMATH_MUL_SX_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLOCK_SIZE ];
            for( sz_t k = 0; k < r_slos; k++ )
            {
                for( sz_t j = 0; j < o_slos; j++ )
                {
                    m_p[ j ][ k ] = m->v_data[ m->i_data[ o_xon * m->i_stride + ( r_xon * r->slos + r_slo + k ) ] + o_slo + j ];
                }
            }

            for( sz_t i = 0; i < o_rows; i++ )
            {
                const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];

                for( sz_t k = 0; k < r_slos; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < o_slos; j++ )
                {
                    for( sz_t k = 0; k < r_slos; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_d[ j ];
                }

                f3_t* r_d = &r->v_data[ r->i_data[ r_xon * r->i_stride + ( o_row + i ) ] + r_slo ];
                for( sz_t k = 0; k < r_slos; k++ ) r_d[ k ] += r_p[ k ];
            }
        #endif
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_xsmf3_s_mul_htp_rxblock
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_xons,
    const bmath_xsmf3_s* m,
          bmath_xsmf3_s* r,                          sz_t r_xon, sz_t r_xons
)
{
    if( o_rows >= o_xons * o->slos && o_rows >= r_xons * r->slos && o_rows > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( o_rows, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_mul_htp_rxblock( o, o_row,                mid, o_xon, o_xons, m, r, r_xon, r_xons );
        bmath_xsmf3_s_mul_htp_rxblock( o, o_row + mid, o_rows - mid, o_xon, o_xons, m, r, r_xon, r_xons );
        return;
    }

    if( o_xons >= r_xons && o_xons > 1 )
    {
        sz_t mid = mf3_sx_midof( o_xons, 1 );
        bmath_xsmf3_s_mul_htp_rxblock( o, o_row, o_rows, o_xon,                mid, m, r, r_xon, r_xons );
        bmath_xsmf3_s_mul_htp_rxblock( o, o_row, o_rows, o_xon + mid, o_xons - mid, m, r, r_xon, r_xons );
        return;
    }

    if( r_xons > 1 )
    {
        sz_t mid = mf3_sx_midof( r_xons, 1 );
        bmath_xsmf3_s_mul_htp_rxblock( o, o_row, o_rows, o_xon, o_xons, m, r, r_xon,                mid );
        bmath_xsmf3_s_mul_htp_rxblock( o, o_row, o_rows, o_xon, o_xons, m, r, r_xon + mid, r_xons - mid );
        return;
    }

    bmath_xsmf3_s_mul_htp_rsblock( o, o_row, o_rows, o_xon, 0, o->slos, m, r, r_xon, 0, r->slos );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_mul_htp( const bmath_xsmf3_s* o, const bmath_xsmf3_s* m, bmath_xsmf3_s* r )
{
    ASSERT( o->rows == r->rows );
    ASSERT( m->rows == r->xons * r->slos );
    ASSERT( o->xons == m->xons );
    ASSERT( o->slos == m->slos );
    bmath_xsmf3_s_zro( r );
    bmath_xsmf3_s_mul_htp_rxblock( o, 0, o->rows, 0, o->xons, m, r, 0, r->xons );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// htp_mul

#ifdef BMATH_AVX

/** smul: Fixed size AVX-Microkernel
 *  Requirement: o_rows == BMATH_MUL_SX_BLOCK_SIZE && m_slos == BMATH_MUL_SX_BLOCK_SIZE
 */
static void bmath_xsmf3_s_htp_mul_rsblock_avx_fix_kernel
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmf3_s* m,             sz_t m_xon, sz_t m_slo,
          bmath_xsmf3_s* r
)
{
    __m256d r_p4[ BMATH_MUL_SX_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLKP4_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
        {
            m_p4[ j ][ k ] = _mm256_loadu_pd( &m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_row + j ) ] + m_slo + k * 4 ] );
        }
    }

    for( sz_t i = 0; i < o_slos; i++ )
    {
        const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + o_row ] + o_slo ];
        __m256d o_p4 = _mm256_set1_pd( o_d[ i ] );

        for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
        {
            const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo ];
            __m256d o_p4 = _mm256_set1_pd( o_d[ i ] );
            for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        sz_t m_row = o_xon * o->slos + o_slo;
        f3_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( m_row + i ) ] + m_slo ];
        for( sz_t k = 0; k < BMATH_MUL_SX_BLKP4_SIZE; k++ )
        {
            _mm256_storeu_pd( r_d + k * 4, _mm256_add_pd( _mm256_loadu_pd( r_d + k * 4 ), r_p4[ k ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Flexible AVX-Microkernel
 *  Requirement: o_rows <= BMATH_MUL_SX_BLOCK_SIZE && m_slos <= BMATH_MUL_SX_BLOCK_SIZE
 */
static void bmath_xsmf3_s_htp_mul_rsblock_avx_flex_kernel
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmf3_s* m,                          sz_t m_xon, sz_t m_slo, sz_t m_slos,
          bmath_xsmf3_s* r
)
{
    assert( o_rows <= BMATH_MUL_SX_BLOCK_SIZE );
    assert( m_slos <= BMATH_MUL_SX_BLOCK_SIZE );

    const sz_t m_slos4l = m_slos >> 2;
    const sz_t m_slosr  = m_slos - m_slos4l * 4;
    const sz_t m_slos4p = m_slosr > 0 ? m_slos4l + 1 : m_slos4l;

    __m256d r_p4[ BMATH_MUL_SX_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLKP4_SIZE ];

    for( sz_t j = 0; j < o_rows; j++ )
    {
        for( sz_t k = 0; k < m_slos4l; k++ )
        {
            m_p4[ j ][ k ] = _mm256_loadu_pd( &m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_row + j ) ] + m_slo + k * 4 ] );

        }

        if( m_slosr > 0 )
        {
            m_p4[ j ][ m_slos4l ] = _mm256_set1_pd( 0 );
            for( sz_t k = 0; k < m_slosr;  k++ )
            {
                m_p4[ j ][ m_slos4l ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_row + j ) ] + m_slo + m_slos4l * 4 + k ];
            }
        }
    }

    for( sz_t i = 0; i < o_slos; i++ )
    {

        for( sz_t k = 0; k < m_slos4p; k++ ) r_p4[ k ] = _mm256_set1_pd( 0 );

        for( sz_t j = 0; j < o_rows; j++ )
        {
            const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo ];
            __m256d o_p4 = _mm256_set1_pd( o_d[ i ] );
            for( sz_t k = 0; k < m_slos4p; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        sz_t m_row = o_xon * o->slos + o_slo;
        f3_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( m_row + i ) ] + m_slo ];
        for( sz_t k = 0; k < m_slos4l; k++ ) _mm256_storeu_pd( r_d + k * 4, _mm256_add_pd( _mm256_loadu_pd( r_d + k * 4 ), r_p4[ k ] ) );

        if( m_slosr > 0 )
        {
            for( sz_t k = 0; k < m_slosr; k++ ) r_d[ m_slos4l * 4 + k ] += r_p4[ m_slos4l ][ k ];
        }
    }
}

#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

static void bmath_xsmf3_s_htp_mul_rsblock
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_xsmf3_s* m,                          sz_t m_xon, sz_t m_slo, sz_t m_slos,
          bmath_xsmf3_s* r
)
{

    if( o_rows == BMATH_MUL_SX_BLOCK_SIZE && m_slos == BMATH_MUL_SX_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_xsmf3_s_htp_mul_rsblock_avx_fix_kernel( o, o_row, o_xon, o_slo, o_slos, m, m_xon, m_slo, r );
        #else
            f3_t r_p[ BMATH_MUL_SX_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLOCK_SIZE ];
            for( sz_t j = 0; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
            {
                for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ )
                {
                    m_p[ j ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_row + j ) ] + m_slo + k ];
                }
            }

            for( sz_t i = 0; i < o_slos; i++ )
            {
                for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < BMATH_MUL_SX_BLOCK_SIZE; j++ )
                {
                    f3_t o_v = o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo + i ];
                    for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
                }

                sz_t m_row = o_xon * o->slos + o_slo;

                f3_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( m_row + i ) ] + m_slo ];
                for( sz_t k = 0; k < BMATH_MUL_SX_BLOCK_SIZE; k++ ) r_d[ k ] += r_p[ k ];
            }
        #endif // BMATH_AVX
        return;
    }

    if( o_rows >= o_slos && o_rows >= m_slos && o_rows > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( o_rows, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_htp_mul_rsblock( o, o_row,                mid, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
        bmath_xsmf3_s_htp_mul_rsblock( o, o_row + mid, o_rows - mid, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
        return;
    }

    if( o_slos >= m_slos && o_slos > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( o_slos, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_htp_mul_rsblock( o, o_row, o_rows, o_xon, o_slo,                mid, m, m_xon, m_slo, m_slos, r );
        bmath_xsmf3_s_htp_mul_rsblock( o, o_row, o_rows, o_xon, o_slo + mid, o_slos - mid, m, m_xon, m_slo, m_slos, r );
        return;
    }

    if( m_slos > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( m_slos, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_htp_mul_rsblock( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo,                mid, r );
        bmath_xsmf3_s_htp_mul_rsblock( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo + mid, m_slos - mid, r );
        return;
    }

    #ifdef BMATH_AVX
        bmath_xsmf3_s_htp_mul_rsblock_avx_flex_kernel( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
    #else
        f3_t r_p[ BMATH_MUL_SX_BLOCK_SIZE ];
        f3_t m_p[ BMATH_MUL_SX_BLOCK_SIZE ][ BMATH_MUL_SX_BLOCK_SIZE ];
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
                f3_t o_v = o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + j ) ] + o_slo + i ];
                for( sz_t k = 0; k < m_slos; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
            }

            sz_t m_row = o_xon * o->slos + o_slo;

            f3_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( m_row + i ) ] + m_slo ];
            for( sz_t k = 0; k < m_slos; k++ ) r_d[ k ] += r_p[ k ];
        }
    #endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_xsmf3_s_htp_mul_rxblock
(
    const bmath_xsmf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_xons,
    const bmath_xsmf3_s* m,                          sz_t m_xon, sz_t m_xons,
          bmath_xsmf3_s* r
)
{
    if( o_rows >= o_xons * o->slos && o_rows >= m_xons * m->slos && o_rows > BMATH_MUL_SX_BLOCK_SIZE )
    {
        sz_t mid = mf3_sx_midof( o_rows, BMATH_MUL_SX_BLOCK_SIZE );
        bmath_xsmf3_s_htp_mul_rxblock( o, o_row,                mid, o_xon, o_xons, m, m_xon, m_xons, r );
        bmath_xsmf3_s_htp_mul_rxblock( o, o_row + mid, o_rows - mid, o_xon, o_xons, m, m_xon, m_xons, r );
        return;
    }

    if( o_xons >= m_xons && o_xons > 1 )
    {
        sz_t mid = mf3_sx_midof( o_xons, 1 );
        bmath_xsmf3_s_htp_mul_rxblock( o, o_row, o_rows, o_xon,                mid, m, m_xon, m_xons, r );
        bmath_xsmf3_s_htp_mul_rxblock( o, o_row, o_rows, o_xon + mid, o_xons - mid, m, m_xon, m_xons, r );
        return;
    }

    if( m_xons > 1 )
    {
        sz_t mid = mf3_sx_midof( m_xons, 1 );
        bmath_xsmf3_s_htp_mul_rxblock( o, o_row, o_rows, o_xon, o_xons, m, m_xon,                mid, r );
        bmath_xsmf3_s_htp_mul_rxblock( o, o_row, o_rows, o_xon, o_xons, m, m_xon + mid, m_xons - mid, r );
        return;
    }

    bmath_xsmf3_s_htp_mul_rsblock( o, o_row, o_rows, o_xon, 0, o->slos, m, m_xon, 0, m->slos, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_htp_mul( const bmath_xsmf3_s* o, const bmath_xsmf3_s* m, bmath_xsmf3_s* r )
{
    ASSERT( o->xons * o->slos == r->rows );
    ASSERT( o->rows           == m->rows );
    ASSERT( m->xons           == r->xons );
    ASSERT( m->slos           == r->slos );
    bmath_xsmf3_s_zro( r );
    bmath_xsmf3_s_htp_mul_rxblock( o, 0, o->rows, 0, o->xons, m, 0, m->xons, r );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // TYPEOF_bmath_xsmf3_s
