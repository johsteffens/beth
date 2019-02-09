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
 *  This header is included in bmath_mf3.h and should not be used in isolation
 *  See bmath_mf3.h for nomenclature.
 */

#include "bmath_mf3.h"
#include "bmath_mf3_mul.h"

/**********************************************************************************************************************/
// mul

// we use macros instead of const sz_t because we need them to be const expressions for best compiler optimizations
#define BMATH_MUL_BLOCK_SIZE ( 4 * 8 )


#ifdef BMATH_AVX
/// mul: Fixed size AVX-Microkernel
static void bmath_mf3_s_mul_avx_fix_kernel( const f3_t* o, sz_t o_s, const f3_t* m, sz_t m_s, f3_t* r, sz_t r_s )
{
    ASSERT( ( BMATH_MUL_BLOCK_SIZE & 3 ) == 0 );

    #define mul_kernel_m_c4 ( BMATH_MUL_BLOCK_SIZE >> 2 )

    __m256d r_p4[ mul_kernel_m_c4   ];
    __m256d m_p4[ BMATH_MUL_BLOCK_SIZE ][ mul_kernel_m_c4 ];

    for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
    {
        const f3_t* mj = m + j * m_s;
        for( sz_t k = 0; k < mul_kernel_m_c4; k++ )
        {
            m_p4[ j ][ k ] = _mm256_loadu_pd( mj + k * 4 );
        }
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const f3_t* oi = o + i * o_s;
              f3_t* ri = r + i * r_s;

        __m256d o_p4 = _mm256_set1_pd( oi[ 0 ] );

        for( sz_t k = 0; k < mul_kernel_m_c4; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( oi[ j ] );
            for( sz_t k = 0; k < mul_kernel_m_c4; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < mul_kernel_m_c4; k++ )
        {
            _mm256_storeu_pd( ri + k * 4, _mm256_add_pd( _mm256_loadu_pd( ri + k * 4 ), r_p4[ k ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** mul: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_c (including 0)
 *  provided o_c <= BMATH_MUL_BLOCK_SIZE && m_c <= mul_kernel_m_c4 * 4
 */
static void bmath_mf3_s_mul_avx_flex_kernel( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_c, f3_t* r, sz_t r_s )
{
    #define mul_kernel_m_c4 ( BMATH_MUL_BLOCK_SIZE >> 2 )

    ASSERT( o_c <= BMATH_MUL_BLOCK_SIZE );
    ASSERT( m_c <= mul_kernel_m_c4 * 4 );

    const sz_t m_c4l = m_c >> 2;
    const sz_t m_cr  = m_c - m_c4l * 4;
    const sz_t m_c4p = m_cr > 0 ? m_c4l + 1 : m_c4l;

    __m256d r_p4[ mul_kernel_m_c4 ];
    __m256d m_p4[ BMATH_MUL_BLOCK_SIZE ][ mul_kernel_m_c4 ];

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

static void bmath_mf3_s_f3_t_mul( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_c, f3_t* r, sz_t r_s )
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
        bmath_mf3_s_f3_t_mul( o,             o_s,       mid, o_c, m, m_s, m_c,             r, r_s );
        bmath_mf3_s_f3_t_mul( o + mid * o_s, o_s, o_r - mid, o_c, m, m_s, m_c, r + mid * r_s, r_s );
        return;
    }

    if( o_c >= m_c && o_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_c, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_f3_t_mul( o,       o_s, o_r,       mid, m,             m_s, m_c, r, r_s );
        bmath_mf3_s_f3_t_mul( o + mid, o_s, o_r, o_c - mid, m + mid * m_s, m_s, m_c, r, r_s );
        return;
    }

    if( m_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( m_c, BMATH_MUL_BLOCK_SIZE );
        bmath_mf3_s_f3_t_mul( o, o_s, o_r, o_c, m,       m_s,       mid, r,       r_s );
        bmath_mf3_s_f3_t_mul( o, o_s, o_r, o_c, m + mid, m_s, m_c - mid, r + mid, r_s );
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

    bmath_mf3_s_f3_t_mul( o->data, o->stride, o->rows, o->cols, m->data, m->stride, m->cols, r->data, r->stride );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// mul_htp

//----------------------------------------------------------------------------------------------------------------------

#define BMATH_MUL_HTP_BLOCK_SIZE ( 4 * 8 )

#ifdef BMATH_AVX

/// mul: Fixed size AVX-Microkernel
static void bmath_mf3_s_mul_htp_avx_fix_kernel( const f3_t* o, sz_t o_s, const f3_t* m, sz_t m_s, f3_t* r, sz_t r_s )
{
    ASSERT( ( BMATH_MUL_HTP_BLOCK_SIZE & 3 ) == 0 );

    #define BMATH_MUL_HTP_BLOCK_SIZE4 ( BMATH_MUL_HTP_BLOCK_SIZE >> 2 )

    __m256d r_p4[ BMATH_MUL_HTP_BLOCK_SIZE4 ];
    __m256d m_p4[ BMATH_MUL_HTP_BLOCK_SIZE ][ BMATH_MUL_HTP_BLOCK_SIZE4 ];

    for( sz_t j = 0; j < BMATH_MUL_HTP_BLOCK_SIZE; j++ )
    {
        const f3_t* mj = m + j;
        for( sz_t k = 0; k < BMATH_MUL_HTP_BLOCK_SIZE4; k++ )
        {
            m_p4[ j ][ k ][ 0 ] = mj[ ( k * 4 + 0 ) * m_s ];
            m_p4[ j ][ k ][ 1 ] = mj[ ( k * 4 + 1 ) * m_s ];
            m_p4[ j ][ k ][ 2 ] = mj[ ( k * 4 + 2 ) * m_s ];
            m_p4[ j ][ k ][ 3 ] = mj[ ( k * 4 + 3 ) * m_s ];
        }
    }

    for( sz_t i = 0; i < BMATH_MUL_HTP_BLOCK_SIZE; i++ )
    {
        const f3_t* oi = o + i * o_s;
              f3_t* ri = r + i * r_s;

        __m256d o_p4 = _mm256_set1_pd( oi[ 0 ] );

        for( sz_t k = 0; k < BMATH_MUL_HTP_BLOCK_SIZE4; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_MUL_HTP_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( oi[ j ] );
            for( sz_t k = 0; k < BMATH_MUL_HTP_BLOCK_SIZE4; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_HTP_BLOCK_SIZE4; k++ )
        {
            _mm256_storeu_pd( ri + k * 4, _mm256_add_pd( _mm256_loadu_pd( ri + k * 4 ), r_p4[ k ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** mul: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_r (including 0)
 *  provided o_c <= BMATH_MUL_HTP_BLOCK_SIZE && m_r <= BMATH_MUL_HTP_BLOCK_SIZE4 * 4
 */
static void bmath_mf3_s_mul_htp_avx_flex_kernel( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_r, f3_t* r, sz_t r_s )
{
    #define BMATH_MUL_HTP_BLOCK_SIZE4 ( BMATH_MUL_HTP_BLOCK_SIZE >> 2 )

    ASSERT( o_c <= BMATH_MUL_HTP_BLOCK_SIZE );
    ASSERT( m_r <= BMATH_MUL_HTP_BLOCK_SIZE4 * 4 );

    const sz_t m_r4l = m_r >> 2;
    const sz_t m_rr  = m_r - m_r4l * 4;
    const sz_t m_r4p = m_rr > 0 ? m_r4l + 1 : m_r4l;

    __m256d r_p4[ BMATH_MUL_HTP_BLOCK_SIZE4 ];
    __m256d m_p4[ BMATH_MUL_HTP_BLOCK_SIZE ][ BMATH_MUL_HTP_BLOCK_SIZE4 ];

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

static void bmath_mf3_s_f3_t_mul_htp( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_r, f3_t* r, sz_t r_s, bl_t sym )
{
    if( o_r == BMATH_MUL_HTP_BLOCK_SIZE && o_c == BMATH_MUL_HTP_BLOCK_SIZE && m_r == BMATH_MUL_HTP_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_mf3_s_mul_htp_avx_fix_kernel( o, o_s, m, m_s, r, r_s );
        #else

            f3_t r_p[ BMATH_MUL_HTP_BLOCK_SIZE ];
            f3_t m_p[ BMATH_MUL_HTP_BLOCK_SIZE ][ BMATH_MUL_HTP_BLOCK_SIZE ];
            for( sz_t k = 0; k < BMATH_MUL_HTP_BLOCK_SIZE; k++ )
            {
                for( sz_t j = 0; j < BMATH_MUL_HTP_BLOCK_SIZE; j++ ) m_p[ j ][ k ] = m[ k * m_s + j ];
            }

            for( sz_t i = 0; i < BMATH_MUL_HTP_BLOCK_SIZE; i++ )
            {
                const f3_t* oi = o + i * o_s;

                for( sz_t k = 0; k < BMATH_MUL_HTP_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < BMATH_MUL_HTP_BLOCK_SIZE; j++ )
                {
                    f3_t f = oi[ j ];
                    for( sz_t k = 0; k < BMATH_MUL_HTP_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
                }

                f3_t* ri = r + i * r_s;
                for( sz_t k = 0; k < BMATH_MUL_HTP_BLOCK_SIZE; k++ ) ri[ k ] += r_p[ k ];
            }
        #endif // BMATH_AVX2_FMA

        return;
    }

    if( o_r >= o_c && o_r >= m_r && o_r > BMATH_MUL_HTP_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_r, BMATH_MUL_HTP_BLOCK_SIZE );
        bmath_mf3_s_f3_t_mul_htp( o,             o_s,       mid, o_c, m, m_s, m_r,             r, r_s, sym );
        bmath_mf3_s_f3_t_mul_htp( o + mid * o_s, o_s, o_r - mid, o_c, m, m_s, m_r, r + mid * r_s, r_s, sym );
        return;
    }

    if( o_c >= m_r && o_c > BMATH_MUL_HTP_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_c, BMATH_MUL_HTP_BLOCK_SIZE );
        bmath_mf3_s_f3_t_mul_htp( o,       o_s, o_r,       mid, m,       m_s, m_r, r, r_s, sym );
        bmath_mf3_s_f3_t_mul_htp( o + mid, o_s, o_r, o_c - mid, m + mid, m_s, m_r, r, r_s, sym );
        return;
    }

    if( m_r > BMATH_MUL_HTP_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( m_r, BMATH_MUL_HTP_BLOCK_SIZE );
        bmath_mf3_s_f3_t_mul_htp( o, o_s, o_r, o_c, m, m_s, mid, r, r_s, sym );

        if( !sym || o != m ) // in case of symmetry skip upper triangle of r
        {
            bmath_mf3_s_f3_t_mul_htp( o, o_s, o_r, o_c, m + mid * m_s, m_s, m_r - mid, r + mid, r_s, sym );
        }
        return;
    }

    /// smaller blocks
    assert( o_r <= BMATH_MUL_HTP_BLOCK_SIZE );
    assert( o_c <= BMATH_MUL_HTP_BLOCK_SIZE );
    assert( m_r <= BMATH_MUL_HTP_BLOCK_SIZE );
    #ifdef BMATH_AVX
        bmath_mf3_s_mul_htp_avx_flex_kernel( o, o_s, o_r, o_c, m, m_s, m_r, r, r_s );
    #else
        f3_t r_p[ BMATH_MUL_HTP_BLOCK_SIZE ];
        f3_t m_p[ BMATH_MUL_HTP_BLOCK_SIZE ][ BMATH_MUL_HTP_BLOCK_SIZE ];
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

    bmath_mf3_s_f3_t_mul_htp( o->data, o->stride, o->rows, o->cols, m->data, m->stride, m->rows, r->data, r->stride, symmetry );

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

#define BMATH_HTP_MUL_BLOCK_SIZE ( 4 * 8 )

#ifdef BMATH_AVX

/// mul: Fixed size AVX-Microkernel
static void bmath_mf3_s_htp_mul_avx_fix_kernel( const f3_t* o, sz_t o_s, const f3_t* m, sz_t m_s, f3_t* r, sz_t r_s )
{
    ASSERT( ( BMATH_HTP_MUL_BLOCK_SIZE & 3 ) == 0 );

    #define BMATH_HTP_MUL_BLOCK_SIZE4 ( BMATH_HTP_MUL_BLOCK_SIZE >> 2 )

    __m256d r_p4[ BMATH_HTP_MUL_BLOCK_SIZE4 ];
    __m256d m_p4[ BMATH_HTP_MUL_BLOCK_SIZE ][ BMATH_HTP_MUL_BLOCK_SIZE4 ];

    for( sz_t j = 0; j < BMATH_HTP_MUL_BLOCK_SIZE; j++ )
    {
        const f3_t* mj = m + j * m_s;
        for( sz_t k = 0; k < BMATH_HTP_MUL_BLOCK_SIZE4; k++ )
        {
            m_p4[ j ][ k ][ 0 ] = mj[ ( k * 4 + 0 ) ];
            m_p4[ j ][ k ][ 1 ] = mj[ ( k * 4 + 1 ) ];
            m_p4[ j ][ k ][ 2 ] = mj[ ( k * 4 + 2 ) ];
            m_p4[ j ][ k ][ 3 ] = mj[ ( k * 4 + 3 ) ];
        }
    }

    for( sz_t i = 0; i < BMATH_HTP_MUL_BLOCK_SIZE; i++ )
    {
        const f3_t* oi = o + i;
              f3_t* ri = r + i * r_s;

        __m256d o_p4 = _mm256_set1_pd( oi[ 0 ] );

        for( sz_t k = 0; k < BMATH_HTP_MUL_BLOCK_SIZE4; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < BMATH_HTP_MUL_BLOCK_SIZE; j++ )
        {
            o_p4 = _mm256_set1_pd( oi[ j * o_s ] );
            for( sz_t k = 0; k < BMATH_HTP_MUL_BLOCK_SIZE4; k++ )
            {
                #ifdef BMATH_AVX2_FMA
                    r_p4[ k ] = _mm256_fmadd_pd( m_p4[ j ][ k ], o_p4, r_p4[ k ] );
                #else
                    r_p4[ k ] = _mm256_add_pd( _mm256_mul_pd( m_p4[ j ][ k ], o_p4 ), r_p4[ k ] );
                #endif // BMATH_AVX2_FMA
            }
        }

        for( sz_t k = 0; k < BMATH_HTP_MUL_BLOCK_SIZE4; k++ )
        {
            _mm256_storeu_pd( ri + k * 4, _mm256_add_pd( _mm256_loadu_pd( ri + k * 4 ), r_p4[ k ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** mul: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_r (including 0)
 *  provided o_c <= BMATH_HTP_MUL_BLOCK_SIZE && m_c <= BMATH_HTP_MUL_BLOCK_SIZE4 * 4
 */
static void bmath_mf3_s_htp_mul_avx_flex_kernel( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_c, f3_t* r, sz_t r_s )
{
    #define BMATH_HTP_MUL_BLOCK_SIZE4 ( BMATH_HTP_MUL_BLOCK_SIZE >> 2 )

    ASSERT( o_r <= BMATH_HTP_MUL_BLOCK_SIZE );
    ASSERT( m_c <= BMATH_HTP_MUL_BLOCK_SIZE4 * 4 );

    const sz_t m_c4l = m_c >> 2;
    const sz_t m_cr  = m_c - m_c4l * 4;
    const sz_t m_c4p = m_cr > 0 ? m_c4l + 1 : m_c4l;

    __m256d r_p4[ BMATH_HTP_MUL_BLOCK_SIZE4 ];
    __m256d m_p4[ BMATH_HTP_MUL_BLOCK_SIZE ][ BMATH_HTP_MUL_BLOCK_SIZE4 ];

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

static void bmath_mf3_s_f3_t_htp_mul( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_c, f3_t* r, sz_t r_s, bl_t sym )
{
    if( o_r == BMATH_HTP_MUL_BLOCK_SIZE && o_c == BMATH_HTP_MUL_BLOCK_SIZE && m_c == BMATH_HTP_MUL_BLOCK_SIZE )
    {
        #ifdef BMATH_AVX
            bmath_mf3_s_htp_mul_avx_fix_kernel( o, o_s, m, m_s, r, r_s );
        #else

            f3_t r_p[ BMATH_HTP_MUL_BLOCK_SIZE ];
            f3_t m_p[ BMATH_HTP_MUL_BLOCK_SIZE ][ BMATH_HTP_MUL_BLOCK_SIZE ];
            for( sz_t k = 0; k < BMATH_HTP_MUL_BLOCK_SIZE; k++ )
            {
                for( sz_t j = 0; j < BMATH_HTP_MUL_BLOCK_SIZE; j++ ) m_p[ k ][ j ] = m[ k * m_s + j ];
            }

            for( sz_t i = 0; i < BMATH_HTP_MUL_BLOCK_SIZE; i++ )
            {
                for( sz_t k = 0; k < BMATH_HTP_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

                for( sz_t j = 0; j < BMATH_HTP_MUL_BLOCK_SIZE; j++ )
                {
                    f3_t f = o[ j * o_s + i ];
                    for( sz_t k = 0; k < BMATH_HTP_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
                }

                f3_t* ri = r + i * r_s;
                for( sz_t k = 0; k < BMATH_HTP_MUL_BLOCK_SIZE; k++ ) ri[ k ] += r_p[ k ];
            }

        #endif // BMATH_AVX2_FMA

        return;
    }

    if( o_r >= o_c && o_r >= m_c && o_r > BMATH_HTP_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_r, BMATH_HTP_MUL_BLOCK_SIZE );
        bmath_mf3_s_f3_t_htp_mul( o,             o_s,       mid, o_c, m,             m_s, m_c, r, r_s, sym );
        bmath_mf3_s_f3_t_htp_mul( o + mid * o_s, o_s, o_r - mid, o_c, m + mid * m_s, m_s, m_c, r, r_s, sym );
        return;
    }

    if( o_c >= m_c && o_c > BMATH_HTP_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( o_c, BMATH_HTP_MUL_BLOCK_SIZE );
        bmath_mf3_s_f3_t_htp_mul( o,       o_s, o_r,       mid, m, m_s, m_c, r,             r_s, sym );
        bmath_mf3_s_f3_t_htp_mul( o + mid, o_s, o_r, o_c - mid, m, m_s, m_c, r + mid * r_s, r_s, sym );
        return;
    }

    if( m_c > BMATH_HTP_MUL_BLOCK_SIZE )
    {
        sz_t mid = bmath_mf3_midof( m_c, BMATH_HTP_MUL_BLOCK_SIZE );
        bmath_mf3_s_f3_t_htp_mul( o, o_s, o_r, o_c, m, m_s, mid, r, r_s, sym );

        if( !sym || o != m ) // in case of symmetry skip upper triangle of r
        {
            bmath_mf3_s_f3_t_htp_mul( o, o_s, o_r, o_c, m + mid, m_s, m_c - mid, r + mid, r_s, sym );
        }
        return;
    }

    /// smaller blocks
    assert( o_c <= BMATH_HTP_MUL_BLOCK_SIZE );
    assert( m_c <= BMATH_HTP_MUL_BLOCK_SIZE );
    assert( o_r <= BMATH_HTP_MUL_BLOCK_SIZE );

    #ifdef BMATH_AVX
        bmath_mf3_s_htp_mul_avx_flex_kernel( o, o_s, o_r, o_c, m, m_s, m_c, r, r_s );
    #else

        f3_t r_p[ BMATH_HTP_MUL_BLOCK_SIZE ];
        f3_t m_p[ BMATH_HTP_MUL_BLOCK_SIZE ][ BMATH_HTP_MUL_BLOCK_SIZE ];


        for( sz_t j = 0; j < o_r; j++ )
        {
            for( sz_t k = 0; k < m_c; k++ ) m_p[ j ][ k ] = m[ j * m_s + k ];
        }

        for( sz_t i = 0; i < o_c; i++ )
        {
            for( sz_t k = 0; k < m_c; k++ ) r_p[ k ] = 0;

            for( sz_t j = 0; j < o_r; j++ )
            {
                f3_t f = o[ j * m_s + i ];
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

    bmath_mf3_s_f3_t_htp_mul( o->data, o->stride, o->rows, o->cols, m->data, m->stride, m->cols, r->data, r->stride, symmetry );

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
// other mul functions

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_esp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    if( res == o )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_mul_esp( o, op, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    // res == op allowed at this point

    ASSERT(  o->cols ==  op->rows );
    ASSERT(  o->rows == res->rows );
    ASSERT( op->cols == res->cols );

    bmath_vf3_s v;
    bmath_vf3_s_init( &v );
    bmath_vf3_s_set_size( &v, op->rows );
    for( uz_t j = 0; j < op->cols; j++ )
    {
        for( uz_t i = 0; i < op->rows; i++ ) v.data[ i ] = op->data[ i * op->stride + j ];
        for( uz_t i = 0; i <  o->rows; i++ )
        {
            res->data[ i * res->stride + j ] = bmath_f3_t_vec_mul_vec_esp( o->data + i * o->stride, v.data, v.size );
        }
    }

    bmath_vf3_s_down( &v );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_htp_esp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols  == op->cols );
    ASSERT( o->rows  == res->rows );
    ASSERT( op->rows == res->cols );

    if( o == op ) // result is symmetric -> we can save half of the work
    {
        if( res == o )
        {
            bmath_mf3_s* buf = bmath_mf3_s_create();
            bmath_mf3_s_set_size( buf, res->rows, res->cols );
            bmath_mf3_s_mul_htp_esp( o, op, buf );
            bmath_mf3_s_cpy( buf, res );
            bmath_mf3_s_discard( buf );
        }
        else
        {
            for( uz_t i = 0; i < o->rows; i++ )
            {
                const f3_t* vi = o->data + i * o->stride;
                for( uz_t j = 0; j <= i ; j++ )
                {
                    f3_t sum = bmath_f3_t_vec_mul_vec_esp( vi, o->data + j * o->stride, o->cols );
                    res->data[ i * res->stride + j ] = sum;
                    res->data[ j * res->stride + i ] = sum;
                }
            }
        }
        return;
    }

    if( res == o )
    {
        bmath_vf3_s row;
        bmath_vf3_s_init( &row );
        bmath_vf3_s_set_size( &row, o->cols );
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const f3_t* voi = o->data + i *   o->stride;
            bmath_vf3_s_zro( &row );
            for( uz_t j = 0; j < op->rows; j++ )
            {
                row.data[ j ] = bmath_f3_t_vec_mul_vec_esp( voi, op->data + j * op->stride, o->cols );
            }

            f3_t* vri = res->data + i * res->stride;
            for( uz_t k = 0; k < row.size; k++ ) vri[ k ] = row.data[ k ];
        }
        bmath_vf3_s_down( &row );
    }
    else if( res == op )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_mul_htp_esp( o, op, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
    }
    else
    {
        bmath_mf3_s_zro( res );
        for( uz_t i = 0; i < o->rows; i++ )
        {
                  f3_t* vri = res->data + i * res->stride;
            const f3_t* voi =   o->data + i *   o->stride;
            for( uz_t j = 0; j < op->rows; j++ )
            {
                vri[ j ] = bmath_f3_t_vec_mul_vec_esp( voi, op->data + j * op->stride, o->cols );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_htp_mul_esp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->rows  == op->rows );
    ASSERT( o->cols  == res->rows );
    ASSERT( op->cols == res->cols );

    if( o == op ) // result is symmetric -> we can save half of the work
    {
        if( res == o )
        {
            bmath_mf3_s* buf = bmath_mf3_s_create();
            bmath_mf3_s_set_size( buf, res->rows, res->cols );
            bmath_mf3_s_htp_mul_esp( o, op, buf );
            bmath_mf3_s_cpy( buf, res );
            bmath_mf3_s_discard( buf );
        }
        else
        {
            for( uz_t i = 0; i < o->cols; i++ )
            {
                const f3_t* vi = o->data + i;
                for( uz_t j = 0; j <= i ; j++ )
                {
                    f3_t sum = bmath_f3_t_vec_mul_vec_esp_stride( vi, o->stride, o->data + j, o->stride, o->rows );
                    res->data[ i * res->stride + j ] = sum;
                    res->data[ j * res->stride + i ] = sum;
                }
            }
        }
        return;
    }

    if( res == o || res == op )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_mul_htp_esp( o, op, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
    }
    else
    {
        bmath_mf3_s_zro( res );
        for( uz_t i = 0; i < o->cols; i++ )
        {
                  f3_t* vri = res->data + i * res->stride;
            const f3_t* voi =   o->data + i;
            for( uz_t j = 0; j < op->cols; j++ )
            {
                vri[ j ] = bmath_f3_t_vec_mul_vec_esp_stride( voi, o->stride, op->data + j, op->stride, o->rows );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mf3_s_mul_udu_htp_( const bmath_mf3_s* u, const bmath_vf3_s* d, bl_t esp, bmath_mf3_s* r )
{
    ASSERT( r != u );
    ASSERT( u->rows == r->cols );
    ASSERT( u->rows == r->rows );
    ASSERT( u->cols == d->size );

    bmath_vf3_s* x = bmath_vf3_s_create();
    bmath_vf3_s_set_size( x, d->size );

    const f3_t* vd = d->data;
          f3_t* vx = x->data;
    for( sz_t i = 0; i < u->rows; i++ )
    {
        const f3_t* ui = u->data + i * u->stride;
              f3_t* ri = r->data + i * r->stride;
        for( sz_t j = 0; j < d->size; j++ ) vx[ j ] = ui[ j ] * vd[ j ];
        for( sz_t j = 0; j < u->rows; j++ )
        {
            const f3_t* uj = u->data + j * u->stride;
            ri[ j ] = esp ? bmath_f3_t_vec_mul_vec_esp( vx, uj, d->size ) :
                            bmath_f3_t_vec_mul_vec(     vx, uj, d->size );
        }
    }

    bmath_vf3_s_discard( x );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_udu_htp( const bmath_mf3_s* u, const bmath_vf3_s* d, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_udu_htp_( u, d, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_udu_htp_esp( const bmath_mf3_s* u, const bmath_vf3_s* d, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_udu_htp_( u, d, true, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mf3_s_mul_udv_htp_( const bmath_mf3_s* u, const bmath_vf3_s* d, const bmath_mf3_s* v, bl_t esp, bmath_mf3_s* r )
{
    ASSERT( r != u );
    ASSERT( r != v );
    ASSERT( r->rows == u->rows );
    ASSERT( r->cols == v->rows );
    ASSERT( u->cols >= d->size );
    ASSERT( v->cols >= d->size );

    bmath_vf3_s* x = bmath_vf3_s_create();
    bmath_vf3_s_set_size( x, d->size );

    const f3_t* vd = d->data;
          f3_t* vx = x->data;
    for( sz_t i = 0; i < u->rows; i++ )
    {
        const f3_t* ui = u->data + i * u->stride;
              f3_t* ri = r->data + i * r->stride;

        for( sz_t j = 0; j < d->size; j++ ) vx[ j ] = ui[ j ] * vd[ j ];

        for( sz_t j = 0; j < v->rows; j++ )
        {
            const f3_t* vj = v->data + j * v->stride;
            ri[ j ] = esp ? bmath_f3_t_vec_mul_vec_esp( vx, vj, d->size ) :
                            bmath_f3_t_vec_mul_vec(     vx, vj, d->size );
        }
    }

    bmath_vf3_s_discard( x );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_udv_htp( const bmath_mf3_s* u, const bmath_vf3_s* d, const bmath_mf3_s* v, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_udv_htp_( u, d, v, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_udv_htp_esp( const bmath_mf3_s* u, const bmath_vf3_s* d, const bmath_mf3_s* v, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_udv_htp_( u, d, v, true, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void bmath_mf3_s_mul_utv_htp_( const bmath_mf3_s* u, const bmath_mf3_s* t, const bmath_mf3_s* v, bl_t esp, bmath_mf3_s* r )
{
    ASSERT( r != u );
    ASSERT( r != v );
    ASSERT( u->rows == r->rows );
    ASSERT( v->rows == r->cols );
    ASSERT( u->cols == t->rows );
    ASSERT( t->cols == v->cols );

    if( bmath_mf3_s_is_dag( t ) )
    {
        bmath_vf3_s* d = bmath_vf3_s_create();
        bmath_vf3_s_set_size( d, sz_min( t->rows, t->cols ) );
        bmath_mf3_s_get_dag_vec( t, d );
        bmath_mf3_s_mul_udv_htp_( u, d, v, esp, r );
        bmath_vf3_s_discard( d );
        return;
    }

    bmath_vf3_s* dm = bmath_vf3_s_create(); // main  diag
    bmath_vf3_s* du = bmath_vf3_s_create(); // upper diag (left aligned)
    bmath_vf3_s* dl = bmath_vf3_s_create(); // lower diag (left aligned)
    bmath_vf3_s* x  = bmath_vf3_s_create(); // temp vector

    bmath_vf3_s_set_size( dm, sz_min( t->rows, t->cols ) );

    if( dm->size > 0 )
    {
        bmath_vf3_s_set_size( du, ( ( t->cols > t->rows ) ? dm->size : dm->size - 1 ) );
        bmath_vf3_s_set_size( dl, ( ( t->rows > t->cols ) ? dm->size : dm->size - 1 ) );
        bmath_vf3_s_set_size( x,  dm->size );

        for( sz_t i = 0; i < dm->size - 1; i++ )
        {
            dm->data[ i ] = t->data[   i       * t->stride +   i       ];
            dl->data[ i ] = t->data[ ( i + 1 ) * t->stride +   i       ];
            du->data[ i ] = t->data[   i       * t->stride + ( i + 1 ) ];
        }

                                   dm->data[ dm->size - 1 ] = t->data[ ( dm->size - 1 ) * t->stride + ( dm->size - 1 ) ];
        if( du->size == dm->size ) du->data[ dm->size - 1 ] = t->data[ ( dm->size - 1 ) * t->stride +   dm->size       ];
        if( dl->size == dm->size ) dl->data[ dm->size - 1 ] = t->data[   dm->size       * t->stride + ( dm->size - 1 ) ];
    }

    const f3_t* vdm = dm->data;
    const f3_t* vdu = du->data;
    const f3_t* vdl = dl->data;
          f3_t* vx  =  x->data;

    for( uz_t i = 0; i < u->rows; i++ )
    {
        const f3_t* ui = u->data + i * u->stride;
              f3_t* ri = r->data + i * r->stride;

        for( sz_t j = 0; j < dm->size; j++ ) vx[ j     ]  = ui[ j     ] * vdm[ j ];
        for( sz_t j = 0; j < du->size; j++ ) vx[ j + 1 ] += ui[ j     ] * vdu[ j ];
        for( sz_t j = 0; j < dl->size; j++ ) vx[ j     ] += ui[ j + 1 ] * vdl[ j ];

        for( sz_t j = 0; j < v->rows; j++ )
        {
            const f3_t* vj = v->data + j * v->stride;
            ri[ j ] = esp ? bmath_f3_t_vec_mul_vec_esp( vx, vj, dm->size ) :
                            bmath_f3_t_vec_mul_vec(     vx, vj, dm->size );
        }
    }

    bmath_vf3_s_discard( dm );
    bmath_vf3_s_discard( du );
    bmath_vf3_s_discard( dl );

    bmath_vf3_s_discard( x );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_utv_htp( const bmath_mf3_s* u, const bmath_mf3_s* t, const bmath_mf3_s* v, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_utv_htp_( u, t, v, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_utv_htp_esp( const bmath_mf3_s* u, const bmath_mf3_s* t, const bmath_mf3_s* v, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_utv_htp_( u, t, v, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_add( const bmath_mf3_s* o, const bmath_mf3_s* b, const bmath_mf3_s* c, bmath_mf3_s* r )
{
    if( r == o || r == b )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, r->rows, r->cols );
        bmath_mf3_s_mul_add( o, b, c, buf );
        bmath_mf3_s_cpy( buf, r );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->cols == b->rows );
    ASSERT( o->rows == r->rows );
    ASSERT( b->cols == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r ) bmath_mf3_s_cpy( c, r );

    bmath_mf3_s_f3_t_mul( o->data, o->stride, o->rows, o->cols, b->data, b->stride, b->cols, r->data, r->stride );
}

//----------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_htp_add( const bmath_mf3_s* o, const bmath_mf3_s* b, const bmath_mf3_s* c, bmath_mf3_s* r )
{
    if( r == o || r == b )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, r->rows, r->cols );
        bmath_mf3_s_mul_htp_add( o, b, c, buf );
        bmath_mf3_s_cpy( buf, r );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->cols == b->cols );
    ASSERT( o->rows == r->rows );
    ASSERT( b->rows == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r ) bmath_mf3_s_cpy( c, r );

    bmath_mf3_s_f3_t_mul_htp( o->data, o->stride, o->rows, o->cols, b->data, b->stride, b->rows, r->data, r->stride, false );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

