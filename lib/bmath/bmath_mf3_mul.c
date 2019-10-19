/** Copyright 2018 Johannes Bernhard Steffens
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

/** Collection of algorithms related to matrix multiplication.
 *  All routines have been designed from scratch and optimized with modern architectures in mind.
 *
 *  Blocking and microkernel strategy:
 *    - Recursion rows-cols trying to reach BLOCK_SIZE
 *    - Microkernels are optimized for BLOCK_SIZE and used AVX-SIMD when available
 */

#include "bmath_mf3.h"

/**********************************************************************************************************************/
// mul

// we use macros instead of const sz_t because we need them to be const expressions for best compiler optimizations
#define BMATH_MUL_BLOCK_SIZE ( 4 * 8 )
#define BMATH_MUL_BLKP4_SIZE ( BMATH_MUL_BLOCK_SIZE >> 2 ) // packed size

#ifdef BMATH_AVX
/// mul: Fixed size AVX-Microkernel
static void bmath_mf3_s_mul_avx_fix_kernel( const f3_t* o, sz_t o_s, const f3_t* m, sz_t m_s, f3_t* r, sz_t r_s )
{
    assert( ( BMATH_MUL_BLOCK_SIZE & 3 ) == 0 );

    __m256d r_p4[ BMATH_MUL_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKP4_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
    {
        const f3_t* mj = m + j * m_s;
        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            m_p4[ j ][ k ] = _mm256_loadu_pd( mj + k * 4 );
        }
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const f3_t* oi = o + i * o_s;
              f3_t* ri = r + i * r_s;

        __m256d o_p4 = _mm256_set1_pd( oi[ 0 ] );

        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( oi[ j ] );
            for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            _mm256_storeu_pd( ri + k * 4, _mm256_add_pd( _mm256_loadu_pd( ri + k * 4 ), r_p4[ k ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** mul: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_c (including 0)
 *  provided o_c <= BMATH_MUL_BLOCK_SIZE && m_c <= BMATH_MUL_BLOCK_SIZE
 */
static void bmath_mf3_s_mul_avx_flex_kernel( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_c, f3_t* r, sz_t r_s )
{
    #define BMATH_MUL_BLKP4_SIZE ( BMATH_MUL_BLOCK_SIZE >> 2 )

    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_c <= BMATH_MUL_BLKP4_SIZE * 4 );

    const sz_t m_c4l = m_c >> 2;
    const sz_t m_cr  = m_c - m_c4l * 4;
    const sz_t m_c4p = m_cr > 0 ? m_c4l + 1 : m_c4l;

    __m256d r_p4[ BMATH_MUL_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKP4_SIZE ];

    for( sz_t j = 0; j < o_c; j++ )
    {
        const f3_t* mj = m + j * m_s;
        for( sz_t k = 0; k < m_c4l; k++ ) m_p4[ j ][ k ] = _mm256_loadu_pd( mj + k * 4 );

        if( m_cr > 0 )
        {
            m_p4[ j ][ m_c4l ] = _mm256_set1_pd( 0 );
            for( sz_t k = 0; k < m_cr;  k++ ) m_p4[ j ][ m_c4l ][ k ] = mj[ m_c4l * 4 + k ];
        }
    }

    for( sz_t i = 0; i < o_r; i++ )
    {
        const f3_t* oi = o + i * o_s;
              f3_t* ri = r + i * r_s;

        for( sz_t k = 0; k < m_c4p; k++ ) r_p4[ k ] = _mm256_set1_pd( 0 );

        for( sz_t j = 0; j < o_c; j++ )
        {
            __m256d o_p4 = _mm256_set1_pd( oi[ j ] );
            for( sz_t k = 0; k < m_c4p; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < m_c4l; k++ ) _mm256_storeu_pd( ri + k * 4, _mm256_add_pd( _mm256_loadu_pd( ri + k * 4 ), r_p4[ k ] ) );

        if( m_cr > 0 )
        {
            for( sz_t k = 0; k < m_cr; k++ ) ri[ m_c4l * 4 + k ] += r_p4[ m_c4l ][ k ];
        }
    }
}
#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

static sz_t bmath_mf3_midof( sz_t v, const sz_t bz )
{
    return ( v > ( bz << 1 ) ) ? ( v / ( bz << 1 ) ) * bz : bz;
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_fx_t_mul( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_c, f3_t* r, sz_t r_s )
{
    if( o_r == BMATH_MUL_BLOCK_SIZE && o_c == BMATH_MUL_BLOCK_SIZE && m_c == BMATH_MUL_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_mf3_s_mul_avx_fix_kernel( o, o_s, m, m_s, r, r_s );
        #else

            f3_t r_p[ BMATH_MUL_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
            for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
            {
                for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) m_p[ j ][ k ] = m[ j * m_s + k ];
            }

            for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
            {
                const f3_t* oi = o + i * o_s;

                for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
                {
                    f3_t f = oi[ j ];
                    for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
                }

                f3_t* ri = r + i * r_s;
                for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) ri[ k ] += r_p[ k ];
            }
        #endif // BMATH_AVX2_FMA

        return;
    }

    if( o_r >= o_c && o_r >= m_c && o_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_r, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_mul( o,             o_s,       mid, o_c, m, m_s, m_c,             r, r_s );
        bmath_mf3_s_fx_t_mul( o + mid * o_s, o_s, o_r - mid, o_c, m, m_s, m_c, r + mid * r_s, r_s );
        return;
    }

    if( o_c >= m_c && o_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_c, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_mul( o,       o_s, o_r,       mid, m,             m_s, m_c, r, r_s );
        bmath_mf3_s_fx_t_mul( o + mid, o_s, o_r, o_c - mid, m + mid * m_s, m_s, m_c, r, r_s );
        return;
    }

    if( m_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( m_c, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_mul( o, o_s, o_r, o_c, m,       m_s,       mid, r,       r_s );
        bmath_mf3_s_fx_t_mul( o, o_s, o_r, o_c, m + mid, m_s, m_c - mid, r + mid, r_s );
        return;
    }

    /// smaller blocks
    assert( o_r <= BMATH_MUL_BLOCK_SIZE );
    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_c <= BMATH_MUL_BLOCK_SIZE );
    #ifdef BMATH_AVX
        bmath_mf3_s_mul_avx_flex_kernel( o, o_s, o_r, o_c, m, m_s, m_c, r, r_s );
    #else
        f3_t r_p[ BMATH_MUL_BLOCK_SIZE ];
        f3_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
        for( sz_t j = 0; j < o_c; j++ )
        {
            for( sz_t k = 0; k < m_c; k++ ) m_p[ j ][ k ] = m[ j * m_s + k ];
        }
        for( sz_t i = 0; i < o_r; i++ )
        {
            const f3_t* oi = o + i * o_s;

            for( sz_t k = 0; k < m_c; k++ ) r_p[ k ] = 0;

            for( sz_t j = 0; j < o_c; j++ )
            {
                f3_t f = oi[ j ];
                for( sz_t k = 0; k < m_c; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
            }

            f3_t* ri = r + i * r_s;
            for( sz_t k = 0; k < m_c; k++ ) ri[ k ] += r_p[ k ];
        }
    #endif // BMATH_AVX2_FMA
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul( const bmath_mf3_s* o, const bmath_mf3_s* m, bmath_mf3_s* r )
{
    if( r == o || r == m )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, r->rows, r->cols );
        bmath_mf3_s_mul( o, m, buf );
        bmath_mf3_s_cpy( buf, r );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->cols == m->rows );
    ASSERT( o->rows == r->rows );
    ASSERT( m->cols == r->cols );

    bmath_mf3_s_zro( r );

    bmath_mf3_s_fx_t_mul( o->data, o->stride, o->rows, o->cols, m->data, m->stride, m->cols, r->data, r->stride );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// mul_htp

//----------------------------------------------------------------------------------------------------------------------

#ifdef BMATH_AVX

/// mul: Fixed size AVX-Microkernel
static void bmath_mf3_s_mul_htp_avx_fix_kernel( const f3_t* o, sz_t o_s, const f3_t* m, sz_t m_s, f3_t* r, sz_t r_s )
{
    assert( ( BMATH_MUL_BLOCK_SIZE & 3 ) == 0 );

    __m256d r_p4[ BMATH_MUL_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKP4_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
    {
        const f3_t* mj = m + j;
        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            m_p4[ j ][ k ][ 0 ] = mj[ ( k * 4 + 0 ) * m_s ];
            m_p4[ j ][ k ][ 1 ] = mj[ ( k * 4 + 1 ) * m_s ];
            m_p4[ j ][ k ][ 2 ] = mj[ ( k * 4 + 2 ) * m_s ];
            m_p4[ j ][ k ][ 3 ] = mj[ ( k * 4 + 3 ) * m_s ];
        }
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const f3_t* oi = o + i * o_s;
              f3_t* ri = r + i * r_s;

        __m256d o_p4 = _mm256_set1_pd( oi[ 0 ] );

        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( oi[ j ] );
            for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            _mm256_storeu_pd( ri + k * 4, _mm256_add_pd( _mm256_loadu_pd( ri + k * 4 ), r_p4[ k ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** mul: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_r (including 0)
 *  provided o_c <= BMATH_MUL_BLOCK_SIZE && m_r <= BMATH_MUL_BLOCK_SIZE
 */
static void bmath_mf3_s_mul_htp_avx_flex_kernel( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_r, f3_t* r, sz_t r_s )
{
    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_r <= BMATH_MUL_BLOCK_SIZE );

    const sz_t m_r4l = m_r >> 2;
    const sz_t m_rr  = m_r - m_r4l * 4;
    const sz_t m_r4p = m_rr > 0 ? m_r4l + 1 : m_r4l;

    __m256d r_p4[ BMATH_MUL_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKP4_SIZE ];

    for( sz_t j = 0; j < o_c; j++ )
    {
        const f3_t* mj = m + j;
        for( sz_t k = 0; k < m_r4l; k++ )
        {
            m_p4[ j ][ k ][ 0 ] = mj[ ( k * 4 + 0 ) * m_s ];
            m_p4[ j ][ k ][ 1 ] = mj[ ( k * 4 + 1 ) * m_s ];
            m_p4[ j ][ k ][ 2 ] = mj[ ( k * 4 + 2 ) * m_s ];
            m_p4[ j ][ k ][ 3 ] = mj[ ( k * 4 + 3 ) * m_s ];
        }

        if( m_rr > 0 )
        {
            m_p4[ j ][ m_r4l ] = _mm256_set1_pd( 0 );
            for( sz_t k = 0; k < m_rr;  k++ ) m_p4[ j ][ m_r4l ][ k ] = mj[ ( m_r4l * 4 + k ) * m_s ];
        }
    }

    for( sz_t i = 0; i < o_r; i++ )
    {
        const f3_t* oi = o + i * o_s;
              f3_t* ri = r + i * r_s;

        for( sz_t k = 0; k < m_r4p; k++ ) r_p4[ k ] = _mm256_set1_pd( 0 );

        for( sz_t j = 0; j < o_c; j++ )
        {
            __m256d o_p4 = _mm256_set1_pd( oi[ j ] );
            for( sz_t k = 0; k < m_r4p; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < m_r4l; k++ ) _mm256_storeu_pd( ri + k * 4, _mm256_add_pd( _mm256_loadu_pd( ri + k * 4 ), r_p4[ k ] ) );

        if( m_rr > 0 )
        {
            for( sz_t k = 0; k < m_rr; k++ ) ri[ m_r4l * 4 + k ] += r_p4[ m_r4l ][ k ];
        }
    }
}

#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_fx_t_mul_htp( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_r, f3_t* r, sz_t r_s, bl_t sym )
{
    if( o_r == BMATH_MUL_BLOCK_SIZE && o_c == BMATH_MUL_BLOCK_SIZE && m_r == BMATH_MUL_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_mf3_s_mul_htp_avx_fix_kernel( o, o_s, m, m_s, r, r_s );
        #else

            f3_t r_p[ BMATH_MUL_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
            for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ )
            {
                for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ ) m_p[ j ][ k ] = m[ k * m_s + j ];
            }

            for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
            {
                const f3_t* oi = o + i * o_s;

                for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
                {
                    f3_t f = oi[ j ];
                    for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
                }

                f3_t* ri = r + i * r_s;
                for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) ri[ k ] += r_p[ k ];
            }
        #endif // BMATH_AVX2_FMA

        return;
    }

    if( o_r >= o_c && o_r >= m_r && o_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_r, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_mul_htp( o,             o_s,       mid, o_c, m, m_s, m_r,             r, r_s, sym );
        bmath_mf3_s_fx_t_mul_htp( o + mid * o_s, o_s, o_r - mid, o_c, m, m_s, m_r, r + mid * r_s, r_s, sym );
        return;
    }

    if( o_c >= m_r && o_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_c, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_mul_htp( o,       o_s, o_r,       mid, m,       m_s, m_r, r, r_s, sym );
        bmath_mf3_s_fx_t_mul_htp( o + mid, o_s, o_r, o_c - mid, m + mid, m_s, m_r, r, r_s, sym );
        return;
    }

    if( m_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( m_r, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_mul_htp( o, o_s, o_r, o_c, m, m_s, mid, r, r_s, sym );

        if( !sym || o != m ) // in case of symmetry skip upper triangle of r
        {
            bmath_mf3_s_fx_t_mul_htp( o, o_s, o_r, o_c, m + mid * m_s, m_s, m_r - mid, r + mid, r_s, sym );
        }
        return;
    }

    /// smaller blocks
    assert( o_r <= BMATH_MUL_BLOCK_SIZE );
    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_r <= BMATH_MUL_BLOCK_SIZE );
    #ifdef BMATH_AVX
        bmath_mf3_s_mul_htp_avx_flex_kernel( o, o_s, o_r, o_c, m, m_s, m_r, r, r_s );
    #else
        f3_t r_p[ BMATH_MUL_BLOCK_SIZE ];
        f3_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
        for( sz_t k = 0; k < m_r; k++ )
        {
            for( sz_t j = 0; j < o_c; j++ ) m_p[ j ][ k ] = m[ k * m_s + j ];
        }

        for( sz_t i = 0; i < o_r; i++ )
        {
            const f3_t* oi = o + i * o_s;

            for( sz_t k = 0; k < m_r; k++ ) r_p[ k ] = 0;

            for( sz_t j = 0; j < o_c; j++ )
            {
                f3_t f = oi[ j ];
                for( sz_t k = 0; k < m_r; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
            }

            f3_t* ri = r + i * r_s;
            for( sz_t k = 0; k < m_r; k++ ) ri[ k ] += r_p[ k ];
        }
    #endif // BMATH_AVX2_FMA
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_htp( const bmath_mf3_s* o, const bmath_mf3_s* m, bmath_mf3_s* r )
{
    if( r == o || r == m )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, r->rows, r->cols );
        bmath_mf3_s_mul_htp( o, m, buf );
        bmath_mf3_s_cpy( buf, r );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->cols == m->cols );
    ASSERT( o->rows == r->rows );
    ASSERT( m->rows == r->cols );

    bmath_mf3_s_zro( r );

    bl_t symmetry = ( o == m );

    bmath_mf3_s_fx_t_mul_htp( o->data, o->stride, o->rows, o->cols, m->data, m->stride, m->rows, r->data, r->stride, symmetry );

    if( symmetry )
    {
        for( sz_t i = 0; i < r->rows; i++ )
        {
            for( sz_t j = 0; j < i; j++ )
            {
                r->data[ j * r->stride + i ] = r->data[ i * r->stride + j ];
            }
        }
    }

}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// htp_mul

#ifdef BMATH_AVX

/// mul: Fixed size AVX-Microkernel
static void bmath_mf3_s_htp_mul_avx_fix_kernel( const f3_t* o, sz_t o_s, const f3_t* m, sz_t m_s, f3_t* r, sz_t r_s )
{
    assert( ( BMATH_MUL_BLOCK_SIZE & 3 ) == 0 );

    __m256d r_p4[ BMATH_MUL_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKP4_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
    {
        const f3_t* mj = m + j * m_s;
        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            m_p4[ j ][ k ][ 0 ] = mj[ ( k * 4 + 0 ) ];
            m_p4[ j ][ k ][ 1 ] = mj[ ( k * 4 + 1 ) ];
            m_p4[ j ][ k ][ 2 ] = mj[ ( k * 4 + 2 ) ];
            m_p4[ j ][ k ][ 3 ] = mj[ ( k * 4 + 3 ) ];
        }
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const f3_t* oi = o + i;
              f3_t* ri = r + i * r_s;

        __m256d o_p4 = _mm256_set1_pd( oi[ 0 ] );

        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( oi[ j * o_s ] );
            for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            _mm256_storeu_pd( ri + k * 4, _mm256_add_pd( _mm256_loadu_pd( ri + k * 4 ), r_p4[ k ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** mul: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_r (including 0)
 *  provided o_c <= BMATH_MUL_BLOCK_SIZE && m_c <= BMATH_MUL_BLKP4_SIZE * 4
 */
static void bmath_mf3_s_htp_mul_avx_flex_kernel( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_c, f3_t* r, sz_t r_s )
{
    assert( o_r <= BMATH_MUL_BLOCK_SIZE );
    assert( m_c <= BMATH_MUL_BLKP4_SIZE * 4 );

    const sz_t m_c4l = m_c >> 2;
    const sz_t m_cr  = m_c - m_c4l * 4;
    const sz_t m_c4p = m_cr > 0 ? m_c4l + 1 : m_c4l;

    __m256d r_p4[ BMATH_MUL_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKP4_SIZE ];

    for( sz_t j = 0; j < o_r; j++ )
    {
        const f3_t* mj = m + j * m_s;
        for( sz_t k = 0; k < m_c4l; k++ )
        {
            m_p4[ j ][ k ][ 0 ] = mj[ ( k * 4 + 0 ) ];
            m_p4[ j ][ k ][ 1 ] = mj[ ( k * 4 + 1 ) ];
            m_p4[ j ][ k ][ 2 ] = mj[ ( k * 4 + 2 ) ];
            m_p4[ j ][ k ][ 3 ] = mj[ ( k * 4 + 3 ) ];
        }

        if( m_cr > 0 )
        {
            m_p4[ j ][ m_c4l ] = _mm256_set1_pd( 0 );
            for( sz_t k = 0; k < m_cr;  k++ ) m_p4[ j ][ m_c4l ][ k ] = mj[ ( m_c4l * 4 + k ) ];
        }
    }

    for( sz_t i = 0; i < o_c; i++ )
    {
        const f3_t* oi = o + i;
              f3_t* ri = r + i * r_s;

        for( sz_t k = 0; k < m_c4p; k++ ) r_p4[ k ] = _mm256_set1_pd( 0 );

        for( sz_t j = 0; j < o_r; j++ )
        {
            __m256d o_p4 = _mm256_set1_pd( oi[ j * o_s ] );
            for( sz_t k = 0; k < m_c4p; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < m_c4l; k++ ) _mm256_storeu_pd( ri + k * 4, _mm256_add_pd( _mm256_loadu_pd( ri + k * 4 ), r_p4[ k ] ) );

        if( m_cr > 0 )
        {
            for( sz_t k = 0; k < m_cr; k++ ) ri[ m_c4l * 4 + k ] += r_p4[ m_c4l ][ k ];
        }
    }
}
#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_fx_t_htp_mul( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_c, f3_t* r, sz_t r_s, bl_t sym )
{
    if( o_r == BMATH_MUL_BLOCK_SIZE && o_c == BMATH_MUL_BLOCK_SIZE && m_c == BMATH_MUL_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_mf3_s_htp_mul_avx_fix_kernel( o, o_s, m, m_s, r, r_s );
        #else

            f3_t r_p[ BMATH_MUL_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
            for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ )
            {
                for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ ) m_p[ k ][ j ] = m[ k * m_s + j ];
            }

            for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
            {
                for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
                {
                    f3_t f = o[ j * o_s + i ];
                    for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
                }

                f3_t* ri = r + i * r_s;
                for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) ri[ k ] += r_p[ k ];
            }

        #endif // BMATH_AVX2_FMA

        return;
    }

    if( o_r >= o_c && o_r >= m_c && o_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_r, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_htp_mul( o,             o_s,       mid, o_c, m,             m_s, m_c, r, r_s, sym );
        bmath_mf3_s_fx_t_htp_mul( o + mid * o_s, o_s, o_r - mid, o_c, m + mid * m_s, m_s, m_c, r, r_s, sym );
        return;
    }

    if( o_c >= m_c && o_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_c, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_htp_mul( o,       o_s, o_r,       mid, m, m_s, m_c, r,             r_s, sym );
        bmath_mf3_s_fx_t_htp_mul( o + mid, o_s, o_r, o_c - mid, m, m_s, m_c, r + mid * r_s, r_s, sym );
        return;
    }

    if( m_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( m_c, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_htp_mul( o, o_s, o_r, o_c, m, m_s, mid, r, r_s, sym );

        if( !sym || o != m ) // in case of symmetry skip upper triangle of r
        {
            bmath_mf3_s_fx_t_htp_mul( o, o_s, o_r, o_c, m + mid, m_s, m_c - mid, r + mid, r_s, sym );
        }
        return;
    }

    /// smaller blocks
    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_c <= BMATH_MUL_BLOCK_SIZE );
    assert( o_r <= BMATH_MUL_BLOCK_SIZE );

    #ifdef BMATH_AVX
        bmath_mf3_s_htp_mul_avx_flex_kernel( o, o_s, o_r, o_c, m, m_s, m_c, r, r_s );
    #else

        f3_t r_p[ BMATH_MUL_BLOCK_SIZE ];
        f3_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];


        for( sz_t j = 0; j < o_r; j++ )
        {
            for( sz_t k = 0; k < m_c; k++ ) m_p[ j ][ k ] = m[ j * m_s + k ];
        }

        for( sz_t i = 0; i < o_c; i++ )
        {
            for( sz_t k = 0; k < m_c; k++ ) r_p[ k ] = 0;

            for( sz_t j = 0; j < o_r; j++ )
            {
                f3_t f = o[ j * o_s + i ];
                for( sz_t k = 0; k < m_c; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
            }

            f3_t* ri = r + i * r_s;
            for( sz_t k = 0; k < m_c; k++ ) ri[ k ] += r_p[ k ];
        }

    #endif // BMATH_AVX2_FMA
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_htp_mul( const bmath_mf3_s* o, const bmath_mf3_s* m, bmath_mf3_s* r )
{
    if( r == o || r == m )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, r->rows, r->cols );
        bmath_mf3_s_htp_mul( o, m, buf );
        bmath_mf3_s_cpy( buf, r );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->rows == m->rows );
    ASSERT( o->cols == r->rows );
    ASSERT( m->cols == r->cols );

    bmath_mf3_s_zro( r );

    bl_t symmetry = ( o == m );

    bmath_mf3_s_fx_t_htp_mul( o->data, o->stride, o->rows, o->cols, m->data, m->stride, m->cols, r->data, r->stride, symmetry );

    if( symmetry )
    {
        for( sz_t i = 0; i < r->rows; i++ )
        {
            for( sz_t j = 0; j < i; j++ )
            {
                r->data[ j * r->stride + i ] = r->data[ i * r->stride + j ];
            }
        }
    }

}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// htp_mul_htp

#ifdef BMATH_AVX

static void bmath_mf3_s_htp_mul_htp_avx_fix_kernel( const f3_t* o, sz_t o_s, const f3_t* m, sz_t m_s, f3_t* r, sz_t r_s )
{
    assert( ( BMATH_MUL_BLOCK_SIZE & 3 ) == 0 );

    __m256d r_p4[ BMATH_MUL_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKP4_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
    {
        const f3_t* mj = m + j;
        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            m_p4[ j ][ k ][ 0 ] = mj[ ( k * 4 + 0 ) * m_s ];
            m_p4[ j ][ k ][ 1 ] = mj[ ( k * 4 + 1 ) * m_s ];
            m_p4[ j ][ k ][ 2 ] = mj[ ( k * 4 + 2 ) * m_s ];
            m_p4[ j ][ k ][ 3 ] = mj[ ( k * 4 + 3 ) * m_s ];
        }
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const f3_t* oi = o + i;
              f3_t* ri = r + i * r_s;

        __m256d o_p4 = _mm256_set1_pd( oi[ 0 ] );

        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( oi[ j * o_s ] );
            for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_BLKP4_SIZE; k++ )
        {
            _mm256_storeu_pd( ri + k * 4, _mm256_add_pd( _mm256_loadu_pd( ri + k * 4 ), r_p4[ k ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** mul: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_r (including 0)
 *  provided o_r <= BMATH_MUL_BLOCK_SIZE && m_r <= BMATH_MUL_BLOCK_SIZE
 */
static void bmath_mf3_s_htp_mul_htp_avx_flex_kernel( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_r, f3_t* r, sz_t r_s )
{
    assert( o_r <= BMATH_MUL_BLOCK_SIZE );
    assert( m_r <= BMATH_MUL_BLOCK_SIZE );

    const sz_t m_r4l = m_r >> 2;
    const sz_t m_rr  = m_r - m_r4l * 4;
    const sz_t m_r4p = m_rr > 0 ? m_r4l + 1 : m_r4l;

    __m256d r_p4[ BMATH_MUL_BLKP4_SIZE ];
    __m256d m_p4[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKP4_SIZE ];

    for( sz_t j = 0; j < o_r; j++ )
    {
        const f3_t* mj = m + j;
        for( sz_t k = 0; k < m_r4l; k++ )
        {
            m_p4[ j ][ k ][ 0 ] = mj[ ( k * 4 + 0 ) * m_s ];
            m_p4[ j ][ k ][ 1 ] = mj[ ( k * 4 + 1 ) * m_s ];
            m_p4[ j ][ k ][ 2 ] = mj[ ( k * 4 + 2 ) * m_s ];
            m_p4[ j ][ k ][ 3 ] = mj[ ( k * 4 + 3 ) * m_s ];
        }

        if( m_rr > 0 )
        {
            m_p4[ j ][ m_r4l ] = _mm256_set1_pd( 0 );
            for( sz_t k = 0; k < m_rr;  k++ ) m_p4[ j ][ m_r4l ][ k ] = mj[ ( m_r4l * 4 + k ) * m_s ];
        }
    }

    for( sz_t i = 0; i < o_c; i++ )
    {
        const f3_t* oi = o + i;
              f3_t* ri = r + i * r_s;

        for( sz_t k = 0; k < m_r4p; k++ ) r_p4[ k ] = _mm256_set1_pd( 0 );

        for( sz_t j = 0; j < o_r; j++ )
        {
            __m256d o_p4 = _mm256_set1_pd( oi[ j * o_s ] );
            for( sz_t k = 0; k < m_r4p; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < m_r4l; k++ ) _mm256_storeu_pd( ri + k * 4, _mm256_add_pd( _mm256_loadu_pd( ri + k * 4 ), r_p4[ k ] ) );

        if( m_rr > 0 )
        {
            for( sz_t k = 0; k < m_rr; k++ ) ri[ m_r4l * 4 + k ] += r_p4[ m_r4l ][ k ];
        }
    }
}

#endif // BMATH_AVX

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_fx_t_htp_mul_htp( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_r, f3_t* r, sz_t r_s )
{
    if( o_r == BMATH_MUL_BLOCK_SIZE && o_c == BMATH_MUL_BLOCK_SIZE && m_r == BMATH_MUL_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_mf3_s_htp_mul_htp_avx_fix_kernel( o, o_s, m, m_s, r, r_s );
        #else

            f3_t r_p[ BMATH_MUL_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
            for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ )
            {
                for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ ) m_p[ j ][ k ] = m[ k * m_s + j ];
            }

            for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
            {
                const f3_t* oi = o + i;

                for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
                {
                    f3_t f = oi[ j * o_s ];
                    for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
                }

                f3_t* ri = r + i * r_s;
                for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) ri[ k ] += r_p[ k ];
            }
        #endif // BMATH_AVX

        return;
    }

    if( o_c >= o_r && o_c >= m_r && o_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_c, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_htp_mul_htp( o,       o_s, o_r,       mid, m, m_s, m_r,             r, r_s );
        bmath_mf3_s_fx_t_htp_mul_htp( o + mid, o_s, o_r, o_c - mid, m, m_s, m_r, r + mid * r_s, r_s );
        return;
    }

    if( o_r >= m_r && o_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_r, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_htp_mul_htp( o,             o_s,       mid, o_c, m,       m_s, m_r, r, r_s );
        bmath_mf3_s_fx_t_htp_mul_htp( o + mid * o_s, o_s, o_r - mid, o_c, m + mid, m_s, m_r, r, r_s );
        return;
    }

    if( m_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( m_r, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_fx_t_htp_mul_htp( o, o_s, o_r, o_c, m, m_s, mid, r, r_s );
        bmath_mf3_s_fx_t_htp_mul_htp( o, o_s, o_r, o_c, m + mid * m_s, m_s, m_r - mid, r + mid, r_s );
        return;
    }

    /// smaller blocks
    assert( o_r <= BMATH_MUL_BLOCK_SIZE );
    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_r <= BMATH_MUL_BLOCK_SIZE );
    #ifdef BMATH_AVX
        bmath_mf3_s_htp_mul_htp_avx_flex_kernel( o, o_s, o_r, o_c, m, m_s, m_r, r, r_s );
    #else
        f3_t r_p[ BMATH_MUL_BLOCK_SIZE ];
        f3_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
        for( sz_t k = 0; k < m_r; k++ )
        {
            for( sz_t j = 0; j < o_r; j++ ) m_p[ j ][ k ] = m[ k * m_s + j ];
        }

        for( sz_t i = 0; i < o_c; i++ )
        {
            const f3_t* oi = o + i;

            for( sz_t k = 0; k < m_r; k++ ) r_p[ k ] = 0;

            for( sz_t j = 0; j < o_r; j++ )
            {
                f3_t f = oi[ j * o_s ];
                for( sz_t k = 0; k < m_r; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
            }

            f3_t* ri = r + i * r_s;
            for( sz_t k = 0; k < m_r; k++ ) ri[ k ] += r_p[ k ];
        }
    #endif // BMATH_AVX2_FMA
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_htp_mul_htp( const bmath_mf3_s* o, const bmath_mf3_s* m, bmath_mf3_s* r )
{
    if( r == o || r == m )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, r->rows, r->cols );
        bmath_mf3_s_htp_mul_htp( o, m, buf );
        bmath_mf3_s_cpy( buf, r );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( m->cols == o->rows );
    ASSERT( o->cols == r->rows );
    ASSERT( m->rows == r->cols );

    bmath_mf3_s_zro( r );
    bmath_mf3_s_fx_t_htp_mul_htp( o->data, o->stride, o->rows, o->cols, m->data, m->stride, m->rows, r->data, r->stride );
}

//----------------------------------------------------------------------------------------------------------------------


/**********************************************************************************************************************/

