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

/** Collection of algorithms related to spliced-matrix multiplication.
 *  Blocking and microkernel strategy:
 *    - Begin recursion reducing rows-xons block size to xons==1
 *    - Continue recursion rows-slos similar to row-cols in mf3
 *    - Microkernels are row-slo-adpated, otherwise similar to mf3.
 *
 *  This header is included in bmath_smf3.h and should not be used in isolation
 *  See bmath_smf3.h for nomenclature.
 */

#include "bmath_smf3.h"
#include "bmath_smf3_mul.h"

/**********************************************************************************************************************/
// mul

#define BMATH_SMUL_BLOCK_SIZE 32

static sz_t smf3_midof( sz_t v, const sz_t bz )
{
    return ( v > ( bz << 1 ) ) ? ( v / ( bz << 1 ) ) * bz : bz;
}

//----------------------------------------------------------------------------------------------------------------------

#ifdef BMATH_AVX
/** smul: Fixed size AVX-Microkernel
 *  Requirement: o_slos == BMATH_SMUL_BLOCK_SIZE && m_slos == BMATH_SMUL_BLOCK_SIZE
 */
static void bmath_smf3_s_mul_rsblock_avx_fix_kernel
(
    const bmath_smf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo,
    const bmath_smf3_s* m,                          sz_t m_xon, sz_t m_slo,
    bmath_smf3_s* r
)
{
    assert( ( BMATH_SMUL_BLOCK_SIZE & 3 ) == 0 );

    #define BMATH_SMUL_BLKP4_SIZE ( BMATH_SMUL_BLOCK_SIZE >> 2 )

    __m256d r_p4[ BMATH_SMUL_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_SMUL_BLOCK_SIZE ][ BMATH_SMUL_BLKP4_SIZE ];

    for( sz_t j = 0; j < BMATH_SMUL_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_SMUL_BLKP4_SIZE; k++ )
        {
            m_p4[ j ][ k ] = _mm256_loadu_pd( &m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + k * 4 ] );
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
              f3_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( o_row + i ) ] + m_slo ];

        __m256d o_p4 = _mm256_set1_pd( o_d[ 0 ] );

        for( sz_t k = 0; k < BMATH_SMUL_BLKP4_SIZE; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_SMUL_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( o_d[ j ] );
            for( sz_t k = 0; k < BMATH_SMUL_BLKP4_SIZE; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < BMATH_SMUL_BLKP4_SIZE; k++ )
        {
            _mm256_storeu_pd( r_d + k * 4, _mm256_add_pd( _mm256_loadu_pd( r_d + k * 4 ), r_p4[ k ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Flexible AVX-Microkernel
 *  Requirement: o_slos <= BMATH_SMUL_BLOCK_SIZE && m_slos <= BMATH_SMUL_BLOCK_SIZE
 */
static void bmath_smf3_s_mul_rsblock_avx_flex_kernel
(
    const bmath_smf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_smf3_s* m,                          sz_t m_xon, sz_t m_slo, sz_t m_slos,
    bmath_smf3_s* r
)
{
    #define BMATH_SMUL_BLKP4_SIZE ( BMATH_SMUL_BLOCK_SIZE >> 2 )

    ASSERT( o_slos <= BMATH_SMUL_BLOCK_SIZE );
    ASSERT( m_slos <= BMATH_SMUL_BLOCK_SIZE );

    const sz_t m_slos4l = m_slos >> 2;
    const sz_t m_slosr  = m_slos - m_slos4l * 4;
    const sz_t m_slos4p = m_slosr > 0 ? m_slos4l + 1 : m_slos4l;

    __m256d r_p4[ BMATH_SMUL_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_SMUL_BLOCK_SIZE ][ BMATH_SMUL_BLKP4_SIZE ];

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

/// xons == 1
static void bmath_smf3_s_mul_rsblock
(
    const bmath_smf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_slo, sz_t o_slos,
    const bmath_smf3_s* m,                          sz_t m_xon, sz_t m_slo, sz_t m_slos,
    bmath_smf3_s* r
)
{
    if( o_slos == BMATH_SMUL_BLOCK_SIZE && m_slos == BMATH_SMUL_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_smf3_s_mul_rsblock_avx_fix_kernel( o, o_row, o_rows, o_xon, o_slo, m, m_xon, m_slo, r );
        #else
            f3_t r_p[ BMATH_SMUL_BLOCK_SIZE ];
            f3_t m_p[ BMATH_SMUL_BLOCK_SIZE ][ BMATH_SMUL_BLOCK_SIZE ];
            for( sz_t j = 0; j < BMATH_SMUL_BLOCK_SIZE; j++ )
            {
                for( sz_t k = 0; k < BMATH_SMUL_BLOCK_SIZE; k++ ) m_p[ j ][ k ] = m->v_data[ m->i_data[ m_xon * m->i_stride + ( o_xon * o->slos + o_slo + j ) ] + m_slo + k ];
            }

            for( sz_t i = 0; i < o_rows; i++ )
            {
                const f3_t* o_d = &o->v_data[ o->i_data[ o_xon * o->i_stride + ( o_row + i ) ] + o_slo ];
                for( sz_t k = 0; k < BMATH_SMUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < BMATH_SMUL_BLOCK_SIZE; j++ )
                {
                    for( sz_t k = 0; k < BMATH_SMUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_d[ j ];
                }

                f3_t* r_d = &r->v_data[ r->i_data[ m_xon * r->i_stride + ( o_row + i ) ] + m_slo ];
                for( sz_t k = 0; k < BMATH_SMUL_BLOCK_SIZE; k++ ) r_d[ k ] += r_p[ k ];
            }
        #endif // BMATH_AVX
        return;
    }

    if( o_rows >= o_slos && o_rows >= m_slos && o_rows > BMATH_SMUL_BLOCK_SIZE )
    {
        sz_t mid = smf3_midof( o_rows, BMATH_SMUL_BLOCK_SIZE );
        bmath_smf3_s_mul_rsblock( o, o_row,                mid, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
        bmath_smf3_s_mul_rsblock( o, o_row + mid, o_rows - mid, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
        return;
    }

    if( o_slos >= m_slos && o_slos > BMATH_SMUL_BLOCK_SIZE )
    {
        sz_t mid = smf3_midof( o_slos, BMATH_SMUL_BLOCK_SIZE );
        bmath_smf3_s_mul_rsblock( o, o_row, o_rows, o_xon, o_slo,                mid, m, m_xon, m_slo, m_slos, r );
        bmath_smf3_s_mul_rsblock( o, o_row, o_rows, o_xon, o_slo + mid, o_slos - mid, m, m_xon, m_slo, m_slos, r );
        return;
    }

    if( m_slos > BMATH_SMUL_BLOCK_SIZE )
    {
        sz_t mid = smf3_midof( m_slos, BMATH_SMUL_BLOCK_SIZE );
        bmath_smf3_s_mul_rsblock( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo,                mid, r );
        bmath_smf3_s_mul_rsblock( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo + mid, m_slos - mid, r );
        return;
    }

    #ifdef BMATH_AVX
        bmath_smf3_s_mul_rsblock_avx_flex_kernel( o, o_row, o_rows, o_xon, o_slo, o_slos, m, m_xon, m_slo, m_slos, r );
    #else
        f3_t r_p[ BMATH_SMUL_BLOCK_SIZE ];
        f3_t m_p[ BMATH_SMUL_BLOCK_SIZE ][ BMATH_SMUL_BLOCK_SIZE ];
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

static void bmath_smf3_s_mul_rxblock
(
    const bmath_smf3_s* o, sz_t o_row, sz_t o_rows, sz_t o_xon, sz_t o_xons,
    const bmath_smf3_s* m,                          sz_t m_xon, sz_t m_xons,
          bmath_smf3_s* r
)
{
    if( o_rows >= o_xons * o->slos && o_rows >= m_xons * m->slos && o_rows > BMATH_SMUL_BLOCK_SIZE )
    {
        sz_t mid = smf3_midof( o_rows, BMATH_SMUL_BLOCK_SIZE );
        bmath_smf3_s_mul_rxblock( o, o_row,                mid, o_xon, o_xons, m, m_xon, m_xons, r );
        bmath_smf3_s_mul_rxblock( o, o_row + mid, o_rows - mid, o_xon, o_xons, m, m_xon, m_xons, r );
        return;
    }

    if( o_xons >= m_xons && o_xons > 1 )
    {
        sz_t mid = smf3_midof( o_xons, 1 );
        bmath_smf3_s_mul_rxblock( o, o_row, o_rows, o_xon,                mid, m, m_xon, m_xons, r );
        bmath_smf3_s_mul_rxblock( o, o_row, o_rows, o_xon + mid, o_xons - mid, m, m_xon, m_xons, r );
        return;
    }

    if( m_xons > 1 )
    {
        sz_t mid = smf3_midof( m_xons, 1 );
        bmath_smf3_s_mul_rxblock( o, o_row, o_rows, o_xon, o_xons, m, m_xon,                mid, r );
        bmath_smf3_s_mul_rxblock( o, o_row, o_rows, o_xon, o_xons, m, m_xon + mid, m_xons - mid, r );
        return;
    }

    bmath_smf3_s_mul_rsblock( o, o_row, o_rows, o_xon, 0, o->slos, m, m_xon, 0, m->slos, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_smf3_s_mul( const bmath_smf3_s* o, const bmath_smf3_s* m, bmath_smf3_s* r )
{
    ASSERT( o->rows           == r->rows );
    ASSERT( o->xons * o->slos == m->rows );
    ASSERT( m->xons           == r->xons );
    ASSERT( m->slos           == r->slos );
    bmath_smf3_s_zro( r );
    bmath_smf3_s_mul_rxblock( o, 0, o->rows, 0, o->xons, m, 0, m->xons, r );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

