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
 *  Blocking and microkernel strategy: Same as bmath_mf3_mul
 */

#include "bmath_mf3_sf.h"
#include "bmath_mf3_sf_mul.h"

#define BMATH_MUL_SF_BLOCK_SIZE 48
#define BMATH_MUL_SF_BLKP4_SIZE ( BMATH_MUL_SF_BLOCK_SIZE >> 2 )

#ifdef TYPEOF_bmath_mf3_sf_s

/**********************************************************************************************************************/
// mul

static sz_t mf3_sf_midof( sz_t v, const sz_t bz )
{
    return ( v > ( bz << 1 ) ) ? ( v / ( bz << 1 ) ) * bz : bz;
}

//----------------------------------------------------------------------------------------------------------------------

#ifdef BMATH_AVX

/** smul: Fixed size AVX-Microkernel
 *  Requirement: o_cols == BMATH_MUL_SF_BLOCK_SIZE && m_cols == BMATH_MUL_SF_BLOCK_SIZE
 */
static void bmath_mf3_sf_s_mul_block_avx_fix_kernel
(
    const bmath_mf3_sf_s* o, sz_t o_row, sz_t o_rows, sz_t o_col,
    const bmath_mf3_sf_s* m,                          sz_t m_col,
          bmath_mf3_sf_s* r
)
{
    __m256d r_p4[ BMATH_MUL_SF_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLKP4_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
        {
            sz_t* m_x = &m->i_data[ ( o_col + j ) * m->i_stride + m_col + k * 4 ];
            m_p4[ j ][ k ][ 0 ] = m->v_data[ m_x[ 0 ] ];
            m_p4[ j ][ k ][ 1 ] = m->v_data[ m_x[ 1 ] ];
            m_p4[ j ][ k ][ 2 ] = m->v_data[ m_x[ 2 ] ];
            m_p4[ j ][ k ][ 3 ] = m->v_data[ m_x[ 3 ] ];
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
              sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + m_col ];

        __m256d o_p4 = _mm256_set1_pd( o->v_data[ o_x[ 0 ] ] );

        for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( o->v_data[ o_x[ j ] ] );
            for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
        {
            r->v_data[ r_x[ k * 4 + 0 ] ] += r_p4[ k ][ 0 ];
            r->v_data[ r_x[ k * 4 + 1 ] ] += r_p4[ k ][ 1 ];
            r->v_data[ r_x[ k * 4 + 2 ] ] += r_p4[ k ][ 2 ];
            r->v_data[ r_x[ k * 4 + 3 ] ] += r_p4[ k ][ 3 ];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Flexible AVX-Microkernel
 *  Requirement: o_cols <= BMATH_MUL_SF_BLOCK_SIZE && m_cols <= BMATH_MUL_SF_BLOCK_SIZE
 */
static void bmath_mf3_sf_s_mul_block_avx_flex_kernel
(
    const bmath_mf3_sf_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_mf3_sf_s* m,                          sz_t m_col, sz_t m_cols,
          bmath_mf3_sf_s* r
)
{
    ASSERT( o_cols <= BMATH_MUL_SF_BLOCK_SIZE );
    ASSERT( m_cols <= BMATH_MUL_SF_BLOCK_SIZE );

    const sz_t m_cols4l = m_cols >> 2;
    const sz_t m_colsr  = m_cols - m_cols4l * 4;
    const sz_t m_cols4p = m_colsr > 0 ? m_cols4l + 1 : m_cols4l;

    __m256d r_p4[ BMATH_MUL_SF_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLKP4_SIZE ];

    for( sz_t j = 0; j < o_cols; j++ )
    {
        for( sz_t k = 0; k < m_cols4l; k++ )
        {
            sz_t* m_x = &m->i_data[ ( o_col + j ) * m->i_stride + m_col + k * 4 ];
            m_p4[ j ][ k ][ 0 ] = m->v_data[ m_x[ 0 ] ];
            m_p4[ j ][ k ][ 1 ] = m->v_data[ m_x[ 1 ] ];
            m_p4[ j ][ k ][ 2 ] = m->v_data[ m_x[ 2 ] ];
            m_p4[ j ][ k ][ 3 ] = m->v_data[ m_x[ 3 ] ];
        }

        if( m_colsr > 0 )
        {
            m_p4[ j ][ m_cols4l ] = _mm256_set1_pd( 0 );
            for( sz_t k = 0; k < m_colsr;  k++ )
            {
                m_p4[ j ][ m_cols4l ][ k ] = m->v_data[ m->i_data[ ( o_col + j ) * m->i_stride + m_col + m_cols4l * 4 + k ] ];
            }
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
              sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + m_col ];

        for( sz_t k = 0; k < m_cols4p; k++ ) r_p4[ k ] = _mm256_set1_pd( 0 );

        for( sz_t j = 0; j < o_cols; j++ )
        {
            __m256d o_p4 = _mm256_set1_pd( o->v_data[ o_x[ j ] ] );
            for( sz_t k = 0; k < m_cols4p; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < m_cols4l; k++ )
        {
            r->v_data[ r_x[ k * 4 + 0 ] ] += r_p4[ k ][ 0 ];
            r->v_data[ r_x[ k * 4 + 1 ] ] += r_p4[ k ][ 1 ];
            r->v_data[ r_x[ k * 4 + 2 ] ] += r_p4[ k ][ 2 ];
            r->v_data[ r_x[ k * 4 + 3 ] ] += r_p4[ k ][ 3 ];
        }

        if( m_colsr > 0 )
        {
            for( sz_t k = 0; k < m_colsr; k++ ) r->v_data[ r_x[ m_cols4l * 4 + k ] ] += r_p4[ m_cols4l ][ k ];
        }
    }
}

#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mf3_sf_s_mul_block
(
    const bmath_mf3_sf_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_mf3_sf_s* m,                          sz_t m_col, sz_t m_cols,
          bmath_mf3_sf_s* r
)
{
    if( o_cols == BMATH_MUL_SF_BLOCK_SIZE && m_cols == BMATH_MUL_SF_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_mf3_sf_s_mul_block_avx_fix_kernel( o, o_row, o_rows, o_col, m, m_col, r );
        #else
            f3_t r_p[ BMATH_MUL_SF_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLOCK_SIZE ];
            for( sz_t j = 0; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
            {
                for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ )
                {
                    m_p[ j ][ k ] = m->v_data[ m->i_data[ ( o_col + j ) * m->i_stride + m_col + k ] ];
                }
            }

            for( sz_t i = 0; i < o_rows; i++ )
            {
                const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
                for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
                {
                    f3_t o_v = o->v_data[ o_x[ j ] ];
                    for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
                }

                sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + m_col ];
                for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
            }
        #endif // BMATH_AVX
        return;
    }

    if( o_rows >= o_cols && o_rows >= m_cols && o_rows > BMATH_MUL_SF_BLOCK_SIZE )
    {
        sz_t mid = mf3_sf_midof( o_rows, BMATH_MUL_SF_BLOCK_SIZE );
        bmath_mf3_sf_s_mul_block( o, o_row,                mid, o_col, o_cols, m, m_col, m_cols, r );
        bmath_mf3_sf_s_mul_block( o, o_row + mid, o_rows - mid, o_col, o_cols, m, m_col, m_cols, r );
        return;
    }

    if( o_cols >= m_cols && o_cols > BMATH_MUL_SF_BLOCK_SIZE )
    {
        sz_t mid = mf3_sf_midof( o_cols, BMATH_MUL_SF_BLOCK_SIZE );
        bmath_mf3_sf_s_mul_block( o, o_row, o_rows, o_col,                mid, m, m_col, m_cols, r );
        bmath_mf3_sf_s_mul_block( o, o_row, o_rows, o_col + mid, o_cols - mid, m, m_col, m_cols, r );
        return;
    }

    if( m_cols > BMATH_MUL_SF_BLOCK_SIZE )
    {
        sz_t mid = mf3_sf_midof( m_cols, BMATH_MUL_SF_BLOCK_SIZE );
        bmath_mf3_sf_s_mul_block( o, o_row, o_rows, o_col, o_cols, m, m_col,                mid, r );
        bmath_mf3_sf_s_mul_block( o, o_row, o_rows, o_col, o_cols, m, m_col + mid, m_cols - mid, r );
        return;
    }

    {
    #ifdef BMATH_AVX
        bmath_mf3_sf_s_mul_block_avx_flex_kernel( o, o_row, o_rows, o_col, o_cols, m, m_col, m_cols, r );
    #else
        f3_t r_p[ BMATH_MUL_SF_BLOCK_SIZE ];
        f3_t m_p[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLOCK_SIZE ];
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
                f3_t o_v = o->v_data[ o_x[ j ] ];
                for( sz_t k = 0; k < m_cols; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
            }

            sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + m_col ];
            for( sz_t k = 0; k < m_cols; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
        }
    #endif
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_sf_s_mul( const bmath_mf3_sf_s* o, const bmath_mf3_sf_s* m, bmath_mf3_sf_s* r )
{
    ASSERT( o->rows == r->rows );
    ASSERT( o->cols == m->rows );
    ASSERT( m->cols == r->cols );
    bmath_mf3_sf_s_zro( r );
    bmath_mf3_sf_s_mul_block( o, 0, o->rows, 0, o->cols, m, 0, m->cols, r );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// mul_htp

#ifdef BMATH_AVX
/** smul: Fixed size AVX-Microkernel
 *  Requirement: o_cols == BMATH_MUL_SF_BLOCK_SIZE && m_cols == BMATH_MUL_SF_BLOCK_SIZE
 */
static void bmath_mf3_sf_s_mul_htp_block_avx_fix_kernel
(
    const bmath_mf3_sf_s* o, sz_t o_row, sz_t o_rows, sz_t o_col,
    const bmath_mf3_sf_s* m,
          bmath_mf3_sf_s* r,                          sz_t r_col
)
{
    assert( ( BMATH_MUL_SF_BLOCK_SIZE & 3 ) == 0 );

    __m256d r_p4[ BMATH_MUL_SF_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLKP4_SIZE ];

    for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
    {
        sz_t m_row = r_col + k * 4;
        for( sz_t j = 0; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
        {
            m_p4[ j ][ k ][ 0 ] = m->v_data[ m->i_data[ ( m_row + 0 ) * m->i_stride + o_col + j ] ];
            m_p4[ j ][ k ][ 1 ] = m->v_data[ m->i_data[ ( m_row + 1 ) * m->i_stride + o_col + j ] ];
            m_p4[ j ][ k ][ 2 ] = m->v_data[ m->i_data[ ( m_row + 2 ) * m->i_stride + o_col + j ] ];
            m_p4[ j ][ k ][ 3 ] = m->v_data[ m->i_data[ ( m_row + 3 ) * m->i_stride + o_col + j ] ];
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
              sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + r_col ];

        __m256d o_p4 = _mm256_set1_pd( o->v_data[ o_x[ 0 ] ] );

        for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( o->v_data[ o_x[ j ] ] );
            for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
        {
            r->v_data[ r_x[ k * 4 + 0 ] ] += r_p4[ k ][ 0 ];
            r->v_data[ r_x[ k * 4 + 1 ] ] += r_p4[ k ][ 1 ];
            r->v_data[ r_x[ k * 4 + 2 ] ] += r_p4[ k ][ 2 ];
            r->v_data[ r_x[ k * 4 + 3 ] ] += r_p4[ k ][ 3 ];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Flexible AVX-Microkernel
 *  Requirement: o_cols <= BMATH_MUL_SF_BLOCK_SIZE && m_cols <= BMATH_MUL_SF_BLOCK_SIZE
 */
static void bmath_mf3_sf_s_mul_htp_block_avx_flex_kernel
(
    const bmath_mf3_sf_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_mf3_sf_s* m,
          bmath_mf3_sf_s* r,                          sz_t r_col, sz_t r_cols
)
{
    ASSERT( o_cols <= BMATH_MUL_SF_BLOCK_SIZE );
    ASSERT( r_cols <= BMATH_MUL_SF_BLOCK_SIZE );

    const sz_t r_cols4l = r_cols >> 2;
    const sz_t r_colsr  = r_cols - r_cols4l * 4;
    const sz_t r_cols4p = r_colsr > 0 ? r_cols4l + 1 : r_cols4l;

    __m256d r_p4[ BMATH_MUL_SF_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLKP4_SIZE ];

    for( sz_t k = 0; k < r_cols4l; k++ )
    {
        sz_t m_row = r_col + k * 4;
        for( sz_t j = 0; j < o_cols; j++ )
        {
            m_p4[ j ][ k ][ 0 ] = m->v_data[ m->i_data[ ( m_row + 0 ) * m->i_stride + o_col + j ] ];
            m_p4[ j ][ k ][ 1 ] = m->v_data[ m->i_data[ ( m_row + 1 ) * m->i_stride + o_col + j ] ];
            m_p4[ j ][ k ][ 2 ] = m->v_data[ m->i_data[ ( m_row + 2 ) * m->i_stride + o_col + j ] ];
            m_p4[ j ][ k ][ 3 ] = m->v_data[ m->i_data[ ( m_row + 3 ) * m->i_stride + o_col + j ] ];
        }
    }

    if( r_colsr > 0 )
    {
        for( sz_t j = 0; j < o_cols; j++ ) m_p4[ j ][ r_cols4l ] = _mm256_set1_pd( 0 );
        for( sz_t k = 0; k < r_colsr; k++ )
        {
            for( sz_t j = 0; j < o_cols; j++ )
            {
                m_p4[ j ][ r_cols4l ][ k ] = m->v_data[ m->i_data[ ( r_col + r_cols4l * 4 + k ) * m->i_stride + o_col + j ] ];
            }
        }
    }

    for( sz_t i = 0; i < o_rows; i++ )
    {
        const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
              sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + r_col ];

        for( sz_t k = 0; k < r_cols4p; k++ ) r_p4[ k ] = _mm256_set1_pd( 0 );

        for( sz_t j = 0; j < o_cols; j++ )
        {
            __m256d o_p4 = _mm256_set1_pd( o->v_data[ o_x[ j ] ] );
            for( sz_t k = 0; k < r_cols4p; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < r_cols4l; k++ )
        {
            r->v_data[ r_x[ k * 4 + 0 ] ] += r_p4[ k ][ 0 ];
            r->v_data[ r_x[ k * 4 + 1 ] ] += r_p4[ k ][ 1 ];
            r->v_data[ r_x[ k * 4 + 2 ] ] += r_p4[ k ][ 2 ];
            r->v_data[ r_x[ k * 4 + 3 ] ] += r_p4[ k ][ 3 ];
        }

        if( r_colsr > 0 )
        {
            for( sz_t k = 0; k < r_colsr; k++ ) r->v_data[ r_x[ r_cols4l * 4 + k ] ] += r_p4[ r_cols4l ][ k ];
        }
    }
}

#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mf3_sf_s_mul_htp_block
(
    const bmath_mf3_sf_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_mf3_sf_s* m,
          bmath_mf3_sf_s* r,                          sz_t r_col, sz_t r_cols
)
{
    if( o_cols == BMATH_MUL_SF_BLOCK_SIZE && r_cols == BMATH_MUL_SF_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_mf3_sf_s_mul_htp_block_avx_fix_kernel( o, o_row, o_rows, o_col, m, r, r_col );
        #else
            f3_t r_p[ BMATH_MUL_SF_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLOCK_SIZE ];
            for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ )
            {
                for( sz_t j = 0; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
                {
                    m_p[ j ][ k ] = m->v_data[ m->i_data[ ( r_col + k ) * m->i_stride + o_col + j ] ];
                }
            }

            for( sz_t i = 0; i < o_rows; i++ )
            {
                const sz_t* o_x = &o->i_data[ ( o_row + i ) * o->i_stride + o_col ];
                      sz_t* r_x = &r->i_data[ ( o_row + i ) * r->i_stride + r_col ];

                for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ ) r_p[ k ] = 0;
                for( sz_t j = 0; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
                {
                    for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o->v_data[ o_x[ j ] ];
                }
                for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
            }
        #endif
        return;
    }

    if( o_rows >= o_cols && o_rows >= r_cols && o_rows > BMATH_MUL_SF_BLOCK_SIZE )
    {
        sz_t mid = mf3_sf_midof( o_rows, BMATH_MUL_SF_BLOCK_SIZE );
        bmath_mf3_sf_s_mul_htp_block( o, o_row,                mid, o_col, o_cols, m, r, r_col, r_cols );
        bmath_mf3_sf_s_mul_htp_block( o, o_row + mid, o_rows - mid, o_col, o_cols, m, r, r_col, r_cols );
        return;
    }

    if( o_cols >= r_cols && o_cols > BMATH_MUL_SF_BLOCK_SIZE )
    {
        sz_t mid = mf3_sf_midof( o_cols, BMATH_MUL_SF_BLOCK_SIZE );
        bmath_mf3_sf_s_mul_htp_block( o, o_row, o_rows, o_col,                mid, m, r, r_col, r_cols );
        bmath_mf3_sf_s_mul_htp_block( o, o_row, o_rows, o_col + mid, o_cols - mid, m, r, r_col, r_cols );
        return;
    }

    if( r_cols > BMATH_MUL_SF_BLOCK_SIZE )
    {
        sz_t mid = mf3_sf_midof( r_cols, BMATH_MUL_SF_BLOCK_SIZE );
        bmath_mf3_sf_s_mul_htp_block( o, o_row, o_rows, o_col, o_cols, m, r, r_col,                mid );
        bmath_mf3_sf_s_mul_htp_block( o, o_row, o_rows, o_col, o_cols, m, r, r_col + mid, r_cols - mid );
        return;
    }

    {
        #ifdef BMATH_AVX
            bmath_mf3_sf_s_mul_htp_block_avx_flex_kernel( o, o_row, o_rows, o_col, o_cols, m, r, r_col, r_cols );
        #else
            f3_t r_p[ BMATH_MUL_SF_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLOCK_SIZE ];
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
        #endif
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_sf_s_mul_htp( const bmath_mf3_sf_s* o, const bmath_mf3_sf_s* m, bmath_mf3_sf_s* r )
{
    ASSERT( o->rows == r->rows );
    ASSERT( m->rows == r->cols );
    ASSERT( o->cols == m->cols );
    bmath_mf3_sf_s_zro( r );
    bmath_mf3_sf_s_mul_htp_block( o, 0, o->rows, 0, o->cols, m, r, 0, r->cols );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// htp_mul

#ifdef BMATH_AVX

/** smul: Fixed size AVX-Microkernel
 *  Requirement: o_rows == BMATH_MUL_SF_BLOCK_SIZE && m_cols == BMATH_MUL_SF_BLOCK_SIZE
 */
static void bmath_mf3_sf_s_htp_mul_block_avx_fix_kernel
(
    const bmath_mf3_sf_s* o, sz_t o_row, sz_t o_col, sz_t o_cols,
    const bmath_mf3_sf_s* m,             sz_t m_col,
          bmath_mf3_sf_s* r
)
{
    __m256d r_p4[ BMATH_MUL_SF_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLKP4_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
        {
            sz_t idx = ( o_row + j ) * m->i_stride + m_col + k * 4;
            m_p4[ j ][ k ][ 0 ] = m->v_data[ m->i_data[ idx + 0 ] ];
            m_p4[ j ][ k ][ 1 ] = m->v_data[ m->i_data[ idx + 1 ] ];
            m_p4[ j ][ k ][ 2 ] = m->v_data[ m->i_data[ idx + 2 ] ];
            m_p4[ j ][ k ][ 3 ] = m->v_data[ m->i_data[ idx + 3 ] ];
        }
    }

    for( sz_t i = 0; i < o_cols; i++ )
    {
        const sz_t* o_x = &o->i_data[ o_row * o->i_stride + o_col ];
        __m256d o_p4 = _mm256_set1_pd( o->v_data[ o_x[ i ] ] );

        for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
        {
            const sz_t* o_x = &o->i_data[ ( o_row + j ) * o->i_stride + o_col ];
            __m256d o_p4 = _mm256_set1_pd( o->v_data[ o_x[ i ] ] );
            for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        sz_t m_row = o_col;
        const sz_t* r_x = &r->i_data[ ( m_row + i ) * r->i_stride + m_col ];
        for( sz_t k = 0; k < BMATH_MUL_SF_BLKP4_SIZE; k++ )
        {
            r->v_data[ r_x[ k * 4 + 0 ] ] += r_p4[ k ][ 0 ];
            r->v_data[ r_x[ k * 4 + 1 ] ] += r_p4[ k ][ 1 ];
            r->v_data[ r_x[ k * 4 + 2 ] ] += r_p4[ k ][ 2 ];
            r->v_data[ r_x[ k * 4 + 3 ] ] += r_p4[ k ][ 3 ];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** smul: Flexible AVX-Microkernel
 *  Requirement: o_rows <= BMATH_MUL_SF_BLOCK_SIZE && m_cols <= BMATH_MUL_SF_BLOCK_SIZE
 */
static void bmath_mf3_sf_s_htp_mul_block_avx_flex_kernel
(
    const bmath_mf3_sf_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_mf3_sf_s* m,                          sz_t m_col, sz_t m_cols,
          bmath_mf3_sf_s* r
)
{
    assert( o_rows <= BMATH_MUL_SF_BLOCK_SIZE );
    assert( m_cols <= BMATH_MUL_SF_BLOCK_SIZE );

    const sz_t m_cols4l = m_cols >> 2;
    const sz_t m_colsr  = m_cols - m_cols4l * 4;
    const sz_t m_cols4p = m_colsr > 0 ? m_cols4l + 1 : m_cols4l;

    __m256d r_p4[ BMATH_MUL_SF_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLKP4_SIZE ];

    for( sz_t j = 0; j < o_rows; j++ )
    {
        for( sz_t k = 0; k < m_cols4l; k++ )
        {
            sz_t idx = ( o_row + j ) * m->i_stride + m_col + k * 4;
            m_p4[ j ][ k ][ 0 ] = m->v_data[ m->i_data[ idx + 0 ] ];
            m_p4[ j ][ k ][ 1 ] = m->v_data[ m->i_data[ idx + 1 ] ];
            m_p4[ j ][ k ][ 2 ] = m->v_data[ m->i_data[ idx + 2 ] ];
            m_p4[ j ][ k ][ 3 ] = m->v_data[ m->i_data[ idx + 3 ] ];
        }

        if( m_colsr > 0 )
        {
            m_p4[ j ][ m_cols4l ] = _mm256_set1_pd( 0 );
            for( sz_t k = 0; k < m_colsr;  k++ )
            {
                m_p4[ j ][ m_cols4l ][ k ] = m->v_data[ m->i_data[ ( o_row + j ) * m->i_stride + m_col + m_cols4l * 4 + k ] ];
            }
        }
    }

    for( sz_t i = 0; i < o_cols; i++ )
    {
        for( sz_t k = 0; k < m_cols4p; k++ ) r_p4[ k ] = _mm256_set1_pd( 0 );

        for( sz_t j = 0; j < o_rows; j++ )
        {
            const sz_t* o_x = &o->i_data[ ( o_row + j ) * o->i_stride + o_col ];
            __m256d o_p4 = _mm256_set1_pd( o->v_data[ o_x[ i ] ] );
            for( sz_t k = 0; k < m_cols4p; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        sz_t m_row = o_col;
        const sz_t* r_x = &r->i_data[ ( m_row + i ) * r->i_stride + m_col ];
        for( sz_t k = 0; k < m_cols4l; k++ )
        {
            r->v_data[ r_x[ k * 4 + 0 ] ] += r_p4[ k ][ 0 ];
            r->v_data[ r_x[ k * 4 + 1 ] ] += r_p4[ k ][ 1 ];
            r->v_data[ r_x[ k * 4 + 2 ] ] += r_p4[ k ][ 2 ];
            r->v_data[ r_x[ k * 4 + 3 ] ] += r_p4[ k ][ 3 ];
        }

        if( m_colsr > 0 )
        {
            for( sz_t k = 0; k < m_colsr; k++ ) r->v_data[ r_x[ m_cols4l * 4 + k ] ] += r_p4[ m_cols4l ][ k ];
        }
    }
}

#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mf3_sf_s_htp_mul_block
(
    const bmath_mf3_sf_s* o, sz_t o_row, sz_t o_rows, sz_t o_col, sz_t o_cols,
    const bmath_mf3_sf_s* m,                          sz_t m_col, sz_t m_cols,
          bmath_mf3_sf_s* r
)
{

    if( o_rows == BMATH_MUL_SF_BLOCK_SIZE && m_cols == BMATH_MUL_SF_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_mf3_sf_s_htp_mul_block_avx_fix_kernel( o, o_row, o_col, o_cols, m, m_col, r );
        #else
            f3_t r_p[ BMATH_MUL_SF_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLOCK_SIZE ];
            for( sz_t j = 0; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
            {
                for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ )
                {
                    m_p[ j ][ k ] = m->v_data[ m->i_data[ ( o_row + j ) * m->i_stride + m_col + k ] ];
                }
            }

            for( sz_t i = 0; i < o_cols; i++ )
            {
                for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < BMATH_MUL_SF_BLOCK_SIZE; j++ )
                {
                    f3_t o_v = o->v_data[ o->i_data[ ( o_row + j ) * o->i_stride + o_col + i ] ];
                    for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
                }

                sz_t m_row = o_col;

                const sz_t* r_x = &r->i_data[ ( m_row + i ) * r->i_stride + m_col ];
                for( sz_t k = 0; k < BMATH_MUL_SF_BLOCK_SIZE; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
            }
        #endif // BMATH_AVX
        return;
    }

    if( o_rows >= o_cols && o_rows >= m_cols && o_rows > BMATH_MUL_SF_BLOCK_SIZE )
    {
        sz_t mid = mf3_sf_midof( o_rows, BMATH_MUL_SF_BLOCK_SIZE );
        bmath_mf3_sf_s_htp_mul_block( o, o_row,                mid, o_col, o_cols, m, m_col, m_cols, r );
        bmath_mf3_sf_s_htp_mul_block( o, o_row + mid, o_rows - mid, o_col, o_cols, m, m_col, m_cols, r );
        return;
    }

    if( o_cols >= m_cols && o_cols > BMATH_MUL_SF_BLOCK_SIZE )
    {
        sz_t mid = mf3_sf_midof( o_cols, BMATH_MUL_SF_BLOCK_SIZE );
        bmath_mf3_sf_s_htp_mul_block( o, o_row, o_rows, o_col,                mid, m, m_col, m_cols, r );
        bmath_mf3_sf_s_htp_mul_block( o, o_row, o_rows, o_col + mid, o_cols - mid, m, m_col, m_cols, r );
        return;
    }

    if( m_cols > BMATH_MUL_SF_BLOCK_SIZE )
    {
        sz_t mid = mf3_sf_midof( m_cols, BMATH_MUL_SF_BLOCK_SIZE );
        bmath_mf3_sf_s_htp_mul_block( o, o_row, o_rows, o_col, o_cols, m, m_col,                mid, r );
        bmath_mf3_sf_s_htp_mul_block( o, o_row, o_rows, o_col, o_cols, m, m_col + mid, m_cols - mid, r );
        return;
    }

    #ifdef BMATH_AVX
        bmath_mf3_sf_s_htp_mul_block_avx_flex_kernel( o, o_row, o_rows, o_col, o_cols, m, m_col, m_cols, r );
    #else
        f3_t r_p[ BMATH_MUL_SF_BLOCK_SIZE ];
        f3_t m_p[ BMATH_MUL_SF_BLOCK_SIZE ][ BMATH_MUL_SF_BLOCK_SIZE ];
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
                f3_t o_v = o->v_data[ o->i_data[ ( o_row + j ) * o->i_stride + o_col + i ] ];
                for( sz_t k = 0; k < m_cols; k++ ) r_p[ k ] += m_p[ j ][ k ] * o_v;
            }

            sz_t m_row = o_col;

            const sz_t* r_x = &r->i_data[ ( m_row + i ) * r->i_stride + m_col ];
            for( sz_t k = 0; k < m_cols; k++ ) r->v_data[ r_x[ k ] ] += r_p[ k ];
        }
    #endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_sf_s_htp_mul( const bmath_mf3_sf_s* o, const bmath_mf3_sf_s* m, bmath_mf3_sf_s* r )
{
    ASSERT( o->cols == r->rows );
    ASSERT( o->rows == m->rows );
    ASSERT( m->cols == r->cols );
    bmath_mf3_sf_s_zro( r );
    bmath_mf3_sf_s_htp_mul_block( o, 0, o->rows, 0, o->cols, m, 0, m->cols, r );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // TYPEOF_bmath_mf3_sf_s
