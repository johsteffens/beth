/** Author and Copyright 2018 Johannes Bernhard Steffens
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

/** Collection of algorithms related to matrix multiplication.
 *  All routines have been designed from scratch and optimized with modern architectures in mind.
 *
 *  Blocking and microkernel strategy:
 *    - Recursion rows-cols trying to reach BLOCK_SIZE
 *    - Microkernels are optimized for BLOCK_SIZE and use AVX-SIMD when available
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

// we use macros instead of const sz_t because we need them to be const expressions for best compiler optimizations

#if BMATH_TEMPLATE_FX_PREC == 2
    // TODO: find optimal value
    #define BMATH_MUL_BLKPK_SIZE ( 8 )
#elif BMATH_TEMPLATE_FX_PREC == 3
    #define BMATH_MUL_BLKPK_SIZE ( 8 )
#endif // BMATH_TEMPLATE_FX_PREC

#define BMATH_MUL_BLOCK_SIZE ( BMATH_MUL_BLKPK_SIZE * P5_SIZE )

/**********************************************************************************************************************/
// Microkernels

/// mul: Fixed size AVX-Microkernel
static void kernel_fixed_mul( const fx_t* o, sz_t o_s, const fx_t* m, sz_t m_s, fx_t* r, sz_t r_s )
{
#ifdef BMATH_AVX

    M5_T r_pk[ BMATH_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
    {
        const fx_t* mj = m + j * m_s;
        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            m_pk[ j ][ k ] = M5_LOAD( mj + k * P5_SIZE );
        }
    }
    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const fx_t* oi = o + i * o_s;
              fx_t* ri = r + i * r_s;

        M5_T o_pk = M5_SET_ALL( oi[ 0 ] );

        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            o_pk = M5_SET_ALL( oi[ j ] );
            for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            M5_STOR( ri + k * P5_SIZE, M5_ADD( M5_LOAD( ri + k * P5_SIZE ), r_pk[ k ] ) );
        }
    }

#else

    fx_t r_p[ BMATH_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
    for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
    {
        for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) m_p[ j ][ k ] = m[ j * m_s + k ];
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const fx_t* oi = o + i * o_s;

        for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            fx_t f = oi[ j ];
            for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
        }

        fx_t* ri = r + i * r_s;
        for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) ri[ k ] += r_p[ k ];
    }

#endif // BMATH_AVX
}

//----------------------------------------------------------------------------------------------------------------------

/** mul: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_c (including 0)
 *  provided o_c <= BMATH_MUL_BLOCK_SIZE && m_c <= BMATH_MUL_BLOCK_SIZE
 */
static void kernel_flexi_mul( const fx_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const fx_t* m, sz_t m_s, sz_t m_c, fx_t* r, sz_t r_s )
{
#ifdef BMATH_AVX

    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_c <= BMATH_MUL_BLOCK_SIZE );

    // notation:
    // p_fit: number of M5_t fitting into m_c
    // p_ful: number of M5_t required to hold all data of m_c (either p_fit or p_fit + 1)
    // rest:  remaining fx_t not fitting into p_fit
    const sz_t m_cp_fit = m_c >> P5_SIZE_EXP;
    const sz_t m_c_rest = m_c - m_cp_fit * P5_SIZE;
    const sz_t m_cp_ful = m_c_rest > 0 ? m_cp_fit + 1 : m_cp_fit;

    M5_T r_pk[ BMATH_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < o_c; j++ )
    {
        const fx_t* mj = m + j * m_s;
        for( sz_t k = 0; k < m_cp_fit; k++ ) m_pk[ j ][ k ] = M5_LOAD( mj + k * P5_SIZE );

        if( m_c_rest > 0 )
        {
            m_pk[ j ][ m_cp_fit ] = M5_SET_ALL( 0 );
            for( sz_t k = 0; k < m_c_rest;  k++ ) m_pk[ j ][ m_cp_fit ][ k ] = mj[ m_cp_fit * P5_SIZE + k ];
        }
    }

    for( sz_t i = 0; i < o_r; i++ )
    {
        const fx_t* oi = o + i * o_s;
              fx_t* ri = r + i * r_s;

        for( sz_t k = 0; k < m_cp_ful; k++ ) r_pk[ k ] = M5_SET_ALL( 0 );

        for( sz_t j = 0; j < o_c; j++ )
        {
            M5_T o_pk = M5_SET_ALL( oi[ j ] );
            for( sz_t k = 0; k < m_cp_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < m_cp_fit; k++ ) M5_STOR( ri + k * P5_SIZE, M5_ADD( M5_LOAD( ri + k * P5_SIZE ), r_pk[ k ] ) );

        if( m_c_rest > 0 )
        {
            for( sz_t k = 0; k < m_c_rest; k++ ) ri[ m_cp_fit * P5_SIZE + k ] += r_pk[ m_cp_fit ][ k ];
        }
    }

#else

    fx_t r_p[ BMATH_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
    for( sz_t j = 0; j < o_c; j++ )
    {
        for( sz_t k = 0; k < m_c; k++ ) m_p[ j ][ k ] = m[ j * m_s + k ];
    }
    for( sz_t i = 0; i < o_r; i++ )
    {
        const fx_t* oi = o + i * o_s;

        for( sz_t k = 0; k < m_c; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_c; j++ )
        {
            fx_t f = oi[ j ];
            for( sz_t k = 0; k < m_c; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
        }

        fx_t* ri = r + i * r_s;
        for( sz_t k = 0; k < m_c; k++ ) ri[ k ] += r_p[ k ];
    }

#endif
}

//----------------------------------------------------------------------------------------------------------------------

/// mul_htp: Fixed size AVX-Microkernel
static void kernel_fixed_mul_htp( const fx_t* o, sz_t o_s, const fx_t* m, sz_t m_s, fx_t* r, sz_t r_s )
{
#ifdef BMATH_AVX

    M5_T r_pk[ BMATH_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
    {
        const fx_t* mj = m + j;
        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = mj[ ( k * P5_SIZE + i ) * m_s ];
            }
        }
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const fx_t* oi = o + i * o_s;
              fx_t* ri = r + i * r_s;

        M5_T o_pk = M5_SET_ALL( oi[ 0 ] );

        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            o_pk = M5_SET_ALL( oi[ j ] );
            for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            M5_STOR( ri + k * P5_SIZE, M5_ADD( M5_LOAD( ri + k * P5_SIZE ), r_pk[ k ] ) );
        }
    }

#else

    fx_t r_p[ BMATH_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ )
    {
        for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ ) m_p[ j ][ k ] = m[ k * m_s + j ];
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const fx_t* oi = o + i * o_s;

        for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            fx_t f = oi[ j ];
            for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
        }

        fx_t* ri = r + i * r_s;
        for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) ri[ k ] += r_p[ k ];
    }

#endif
}

//----------------------------------------------------------------------------------------------------------------------

/** mul_htp: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_r (including 0)
 *  provided o_c <= BMATH_MUL_BLOCK_SIZE && m_r <= BMATH_MUL_BLOCK_SIZE
 */
static void kernel_flexi_mul_htp( const fx_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const fx_t* m, sz_t m_s, sz_t m_r, fx_t* r, sz_t r_s )
{
#ifdef BMATH_AVX

    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_r <= BMATH_MUL_BLOCK_SIZE );

    // notation:
    // p_fit: number of M5_t fitting into m_c
    // p_ful: number of M5_t required to hold all data of m_c (either p_fit or p_fit + 1)
    // rest:  remaining fx_t not fitting into p_fit
    const sz_t m_rp_fit = m_r >> P5_SIZE_EXP;
    const sz_t m_r_rest = m_r - m_rp_fit * P5_SIZE;
    const sz_t m_rp_ful = m_r_rest > 0 ? m_rp_fit + 1 : m_rp_fit;

    M5_T r_pk[ BMATH_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < o_c; j++ )
    {
        const fx_t* mj = m + j;
        for( sz_t k = 0; k < m_rp_fit; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = mj[ ( k * P5_SIZE + i ) * m_s ];
            }
        }

        if( m_r_rest > 0 )
        {
            m_pk[ j ][ m_rp_fit ] = M5_SET_ALL( 0 );
            for( sz_t k = 0; k < m_r_rest; k++ ) m_pk[ j ][ m_rp_fit ][ k ] = mj[ ( m_rp_fit * P5_SIZE + k ) * m_s ];
        }
    }

    for( sz_t i = 0; i < o_r; i++ )
    {
        const fx_t* oi = o + i * o_s;
              fx_t* ri = r + i * r_s;

        for( sz_t k = 0; k < m_rp_ful; k++ ) r_pk[ k ] = M5_SET_ALL( 0 );

        for( sz_t j = 0; j < o_c; j++ )
        {
            M5_T o_pk = M5_SET_ALL( oi[ j ] );
            for( sz_t k = 0; k < m_rp_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < m_rp_fit; k++ ) M5_STOR( ri + k * P5_SIZE, M5_ADD( M5_LOAD( ri + k * P5_SIZE ), r_pk[ k ] ) );

        if( m_r_rest > 0 )
        {
            for( sz_t k = 0; k < m_r_rest; k++ ) ri[ m_rp_fit * P5_SIZE + k ] += r_pk[ m_rp_fit ][ k ];
        }
    }

#else

    fx_t r_p[ BMATH_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < m_r; k++ )
    {
        for( sz_t j = 0; j < o_c; j++ ) m_p[ j ][ k ] = m[ k * m_s + j ];
    }

    for( sz_t i = 0; i < o_r; i++ )
    {
        const fx_t* oi = o + i * o_s;

        for( sz_t k = 0; k < m_r; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_c; j++ )
        {
            fx_t f = oi[ j ];
            for( sz_t k = 0; k < m_r; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
        }

        fx_t* ri = r + i * r_s;
        for( sz_t k = 0; k < m_r; k++ ) ri[ k ] += r_p[ k ];
    }

#endif
}

//----------------------------------------------------------------------------------------------------------------------

/// htp_mul: Fixed size AVX-Microkernel
static void kernel_fixed_htp_mul( const fx_t* o, sz_t o_s, const fx_t* m, sz_t m_s, fx_t* r, sz_t r_s )
{
#ifdef BMATH_AVX

    M5_T r_pk[ BMATH_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
    {
        const fx_t* mj = m + j * m_s;
        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = mj[ ( k * P5_SIZE + i ) ];
            }
        }
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const fx_t* oi = o + i;
              fx_t* ri = r + i * r_s;

        M5_T o_pk = M5_SET_ALL( oi[ 0 ] );

        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            o_pk = M5_SET_ALL( oi[ j * o_s ] );
            for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            M5_STOR( ri + k * P5_SIZE, M5_ADD( M5_LOAD( ri + k * P5_SIZE ), r_pk[ k ] ) );
        }
    }

#else

    fx_t r_p[ BMATH_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ )
    {
        for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ ) m_p[ k ][ j ] = m[ k * m_s + j ];
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            fx_t f = o[ j * o_s + i ];
            for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
        }

        fx_t* ri = r + i * r_s;
        for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) ri[ k ] += r_p[ k ];
    }

#endif
}

//----------------------------------------------------------------------------------------------------------------------

/** htp_mul: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_r (including 0)
 *  provided o_c <= BMATH_MUL_BLOCK_SIZE && m_c <= BMATH_MUL_BLKPK_SIZE * P5_SIZE
 */
static void kernel_flexi_htp_mul( const fx_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const fx_t* m, sz_t m_s, sz_t m_c, fx_t* r, sz_t r_s )
{
#ifdef BMATH_AVX

    assert( o_r <= BMATH_MUL_BLOCK_SIZE );
    assert( m_c <= BMATH_MUL_BLKPK_SIZE * P5_SIZE );

    // notation:
    // p_fit: number of M5_t fitting into m_c
    // p_ful: number of M5_t required to hold all data of m_c (either p_fit or p_fit + 1)
    // rest:  remaining fx_t not fitting into p_fit
    const sz_t m_cp_fit = m_c >> P5_SIZE_EXP;
    const sz_t m_c_rest = m_c - m_cp_fit * P5_SIZE;
    const sz_t m_cp_ful = m_c_rest > 0 ? m_cp_fit + 1 : m_cp_fit;

    M5_T r_pk[ BMATH_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < o_r; j++ )
    {
        const fx_t* mj = m + j * m_s;
        for( sz_t k = 0; k < m_cp_fit; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = mj[ ( k * P5_SIZE + i ) ];
            }
        }

        if( m_c_rest > 0 )
        {
            m_pk[ j ][ m_cp_fit ] = M5_SET_ALL( 0 );
            for( sz_t k = 0; k < m_c_rest;  k++ ) m_pk[ j ][ m_cp_fit ][ k ] = mj[ ( m_cp_fit * P5_SIZE + k ) ];
        }
    }

    for( sz_t i = 0; i < o_c; i++ )
    {
        const fx_t* oi = o + i;
              fx_t* ri = r + i * r_s;

        for( sz_t k = 0; k < m_cp_ful; k++ ) r_pk[ k ] = M5_SET_ALL( 0 );

        for( sz_t j = 0; j < o_r; j++ )
        {
            M5_T o_pk = M5_SET_ALL( oi[ j * o_s ] );
            for( sz_t k = 0; k < m_cp_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < m_cp_fit; k++ ) M5_STOR( ri + k * P5_SIZE, M5_ADD( M5_LOAD( ri + k * P5_SIZE ), r_pk[ k ] ) );

        if( m_c_rest > 0 )
        {
            for( sz_t k = 0; k < m_c_rest; k++ ) ri[ m_cp_fit * P5_SIZE + k ] += r_pk[ m_cp_fit ][ k ];
        }
    }

#else

    fx_t r_p[ BMATH_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];


    for( sz_t j = 0; j < o_r; j++ )
    {
        for( sz_t k = 0; k < m_c; k++ ) m_p[ j ][ k ] = m[ j * m_s + k ];
    }

    for( sz_t i = 0; i < o_c; i++ )
    {
        for( sz_t k = 0; k < m_c; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_r; j++ )
        {
            fx_t f = o[ j * o_s + i ];
            for( sz_t k = 0; k < m_c; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
        }

        fx_t* ri = r + i * r_s;
        for( sz_t k = 0; k < m_c; k++ ) ri[ k ] += r_p[ k ];
    }

#endif
}

//----------------------------------------------------------------------------------------------------------------------

static void kernel_fixed_htp_mul_htp( const fx_t* o, sz_t o_s, const fx_t* m, sz_t m_s, fx_t* r, sz_t r_s )
{
#ifdef BMATH_AVX

    assert( ( BMATH_MUL_BLOCK_SIZE & 3 ) == 0 );

    M5_T r_pk[ BMATH_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
    {
        const fx_t* mj = m + j;
        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = mj[ ( k * P5_SIZE + i ) * m_s ];
            }
        }
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const fx_t* oi = o + i;
              fx_t* ri = r + i * r_s;

        M5_T o_pk = M5_SET_ALL( oi[ 0 ] );

        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            r_pk[ k ] = M5_MUL( m_pk[ 0 ][ k ], o_pk );
        }

        for( sz_t j = 1; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            o_pk = M5_SET_ALL( oi[ j * o_s ] );
            for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < BMATH_MUL_BLKPK_SIZE; k++ )
        {
            M5_STOR( ri + k * P5_SIZE, M5_ADD( M5_LOAD( ri + k * P5_SIZE ), r_pk[ k ] ) );
        }
    }

#else

    fx_t r_p[ BMATH_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ )
    {
        for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ ) m_p[ j ][ k ] = m[ k * m_s + j ];
    }

    for( sz_t i = 0; i < BMATH_MUL_BLOCK_SIZE; i++ )
    {
        const fx_t* oi = o + i;

        for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < BMATH_MUL_BLOCK_SIZE; j++ )
        {
            fx_t f = oi[ j * o_s ];
            for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
        }

        fx_t* ri = r + i * r_s;
        for( sz_t k = 0; k < BMATH_MUL_BLOCK_SIZE; k++ ) ri[ k ] += r_p[ k ];
    }

#endif
}

//----------------------------------------------------------------------------------------------------------------------

/** htp_mul_htp: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_r (including 0)
 *  provided o_r <= BMATH_MUL_BLOCK_SIZE && m_r <= BMATH_MUL_BLOCK_SIZE
 */
static void kernel_flexi_htp_mul_htp( const fx_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const fx_t* m, sz_t m_s, sz_t m_r, fx_t* r, sz_t r_s )
{
#ifdef BMATH_AVX

    assert( o_r <= BMATH_MUL_BLOCK_SIZE );
    assert( m_r <= BMATH_MUL_BLOCK_SIZE );

    // notation:
    // p_fit: number of M5_t fitting into m_c
    // p_ful: number of M5_t required to hold all data of m_c (either p_fit or p_fit + 1)
    // rest:  remaining fx_t not fitting into p_fit
    const sz_t m_rp_fit = m_r >> P5_SIZE_EXP;
    const sz_t m_r_rest = m_r - m_rp_fit * P5_SIZE;
    const sz_t m_rp_ful = m_r_rest > 0 ? m_rp_fit + 1 : m_rp_fit;

    M5_T r_pk[ BMATH_MUL_BLKPK_SIZE ];
    M5_T m_pk[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLKPK_SIZE ];

    for( sz_t j = 0; j < o_r; j++ )
    {
        const fx_t* mj = m + j;
        for( sz_t k = 0; k < m_rp_fit; k++ )
        {
            for( sz_t i = 0; i < P5_SIZE; i++ )
            {
                m_pk[ j ][ k ][ i ] = mj[ ( k * P5_SIZE + i ) * m_s ];
            }
        }

        if( m_r_rest > 0 )
        {
            m_pk[ j ][ m_rp_fit ] = M5_SET_ALL( 0 );
            for( sz_t k = 0; k < m_r_rest;  k++ ) m_pk[ j ][ m_rp_fit ][ k ] = mj[ ( m_rp_fit * P5_SIZE + k ) * m_s ];
        }
    }

    for( sz_t i = 0; i < o_c; i++ )
    {
        const fx_t* oi = o + i;
              fx_t* ri = r + i * r_s;

        for( sz_t k = 0; k < m_rp_ful; k++ ) r_pk[ k ] = M5_SET_ALL( 0 );

        for( sz_t j = 0; j < o_r; j++ )
        {
            M5_T o_pk = M5_SET_ALL( oi[ j * o_s ] );
            for( sz_t k = 0; k < m_rp_ful; k++ )
            {
                r_pk[ k ] = M5_MUL_ADD( m_pk[ j ][ k ], o_pk, r_pk[ k ] );
            }
        }

        for( sz_t k = 0; k < m_rp_fit; k++ ) M5_STOR( ri + k * P5_SIZE, M5_ADD( M5_LOAD( ri + k * P5_SIZE ), r_pk[ k ] ) );

        if( m_r_rest > 0 )
        {
            for( sz_t k = 0; k < m_r_rest; k++ ) ri[ m_rp_fit * P5_SIZE + k ] += r_pk[ m_rp_fit ][ k ];
        }
    }

#else

    fx_t r_p[ BMATH_MUL_BLOCK_SIZE ];
    fx_t m_p[ BMATH_MUL_BLOCK_SIZE ][ BMATH_MUL_BLOCK_SIZE ];
    for( sz_t k = 0; k < m_r; k++ )
    {
        for( sz_t j = 0; j < o_r; j++ ) m_p[ j ][ k ] = m[ k * m_s + j ];
    }

    for( sz_t i = 0; i < o_c; i++ )
    {
        const fx_t* oi = o + i;

        for( sz_t k = 0; k < m_r; k++ ) r_p[ k ] = 0;

        for( sz_t j = 0; j < o_r; j++ )
        {
            fx_t f = oi[ j * o_s ];
            for( sz_t k = 0; k < m_r; k++ ) r_p[ k ] += m_p[ j ][ k ] * f;
        }

        fx_t* ri = r + i * r_s;
        for( sz_t k = 0; k < m_r; k++ ) ri[ k ] += r_p[ k ];
    }

#endif
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// Recursive block multiplication

static sz_t midof( sz_t v, const sz_t bz )
{
    return ( v > ( bz << 1 ) ) ? ( v / ( bz << 1 ) ) * bz : bz;
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_block_mul( const fx_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const fx_t* m, sz_t m_s, sz_t m_c, fx_t* r, sz_t r_s )
{
    if( o_r == BMATH_MUL_BLOCK_SIZE && o_c == BMATH_MUL_BLOCK_SIZE && m_c == BMATH_MUL_BLOCK_SIZE )
    {
        kernel_fixed_mul( o, o_s, m, m_s, r, r_s );
        return;
    }

    if( o_r >= o_c && o_r >= m_c && o_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_r, BMATH_MUL_BLOCK_SIZE );
        recursive_block_mul( o,             o_s,       mid, o_c, m, m_s, m_c,             r, r_s );
        recursive_block_mul( o + mid * o_s, o_s, o_r - mid, o_c, m, m_s, m_c, r + mid * r_s, r_s );
        return;
    }

    if( o_c >= m_c && o_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_c, BMATH_MUL_BLOCK_SIZE );
        recursive_block_mul( o,       o_s, o_r,       mid, m,             m_s, m_c, r, r_s );
        recursive_block_mul( o + mid, o_s, o_r, o_c - mid, m + mid * m_s, m_s, m_c, r, r_s );
        return;
    }

    if( m_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( m_c, BMATH_MUL_BLOCK_SIZE );
        recursive_block_mul( o, o_s, o_r, o_c, m,       m_s,       mid, r,       r_s );
        recursive_block_mul( o, o_s, o_r, o_c, m + mid, m_s, m_c - mid, r + mid, r_s );
        return;
    }

    /// smaller blocks
    assert( o_r <= BMATH_MUL_BLOCK_SIZE );
    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_c <= BMATH_MUL_BLOCK_SIZE );
    kernel_flexi_mul( o, o_s, o_r, o_c, m, m_s, m_c, r, r_s );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_block_mul_htp( const fx_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const fx_t* m, sz_t m_s, sz_t m_r, fx_t* r, sz_t r_s, bl_t sym )
{
    if( o_r == BMATH_MUL_BLOCK_SIZE && o_c == BMATH_MUL_BLOCK_SIZE && m_r == BMATH_MUL_BLOCK_SIZE )
    {
        kernel_fixed_mul_htp( o, o_s, m, m_s, r, r_s );
        return;
    }

    if( o_r >= o_c && o_r >= m_r && o_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_r, BMATH_MUL_BLOCK_SIZE );
        recursive_block_mul_htp( o,             o_s,       mid, o_c, m, m_s, m_r,             r, r_s, sym );
        recursive_block_mul_htp( o + mid * o_s, o_s, o_r - mid, o_c, m, m_s, m_r, r + mid * r_s, r_s, sym );
        return;
    }

    if( o_c >= m_r && o_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_c, BMATH_MUL_BLOCK_SIZE );
        recursive_block_mul_htp( o,       o_s, o_r,       mid, m,       m_s, m_r, r, r_s, sym );
        recursive_block_mul_htp( o + mid, o_s, o_r, o_c - mid, m + mid, m_s, m_r, r, r_s, sym );
        return;
    }

    if( m_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( m_r, BMATH_MUL_BLOCK_SIZE );
        recursive_block_mul_htp( o, o_s, o_r, o_c, m, m_s, mid, r, r_s, sym );

        if( !sym || o != m ) // in case of symmetry skip upper triangle of r
        {
            recursive_block_mul_htp( o, o_s, o_r, o_c, m + mid * m_s, m_s, m_r - mid, r + mid, r_s, sym );
        }
        return;
    }

    /// smaller blocks
    assert( o_r <= BMATH_MUL_BLOCK_SIZE );
    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_r <= BMATH_MUL_BLOCK_SIZE );
    kernel_flexi_mul_htp( o, o_s, o_r, o_c, m, m_s, m_r, r, r_s );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_block_htp_mul( const fx_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const fx_t* m, sz_t m_s, sz_t m_c, fx_t* r, sz_t r_s, bl_t sym )
{
    if( o_r == BMATH_MUL_BLOCK_SIZE && o_c == BMATH_MUL_BLOCK_SIZE && m_c == BMATH_MUL_BLOCK_SIZE )
    {
        kernel_fixed_htp_mul( o, o_s, m, m_s, r, r_s );
        return;
    }

    if( o_r >= o_c && o_r >= m_c && o_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_r, BMATH_MUL_BLOCK_SIZE );
        recursive_block_htp_mul( o,             o_s,       mid, o_c, m,             m_s, m_c, r, r_s, sym );
        recursive_block_htp_mul( o + mid * o_s, o_s, o_r - mid, o_c, m + mid * m_s, m_s, m_c, r, r_s, sym );
        return;
    }

    if( o_c >= m_c && o_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_c, BMATH_MUL_BLOCK_SIZE );
        recursive_block_htp_mul( o,       o_s, o_r,       mid, m, m_s, m_c, r,             r_s, sym );
        recursive_block_htp_mul( o + mid, o_s, o_r, o_c - mid, m, m_s, m_c, r + mid * r_s, r_s, sym );
        return;
    }

    if( m_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( m_c, BMATH_MUL_BLOCK_SIZE );
        recursive_block_htp_mul( o, o_s, o_r, o_c, m, m_s, mid, r, r_s, sym );

        if( !sym || o != m ) // in case of symmetry skip upper triangle of r
        {
            recursive_block_htp_mul( o, o_s, o_r, o_c, m + mid, m_s, m_c - mid, r + mid, r_s, sym );
        }
        return;
    }

    /// smaller blocks
    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_c <= BMATH_MUL_BLOCK_SIZE );
    assert( o_r <= BMATH_MUL_BLOCK_SIZE );
    kernel_flexi_htp_mul( o, o_s, o_r, o_c, m, m_s, m_c, r, r_s );
}

//----------------------------------------------------------------------------------------------------------------------

static void recursive_block_htp_mul_htp( const fx_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const fx_t* m, sz_t m_s, sz_t m_r, fx_t* r, sz_t r_s )
{
    if( o_r == BMATH_MUL_BLOCK_SIZE && o_c == BMATH_MUL_BLOCK_SIZE && m_r == BMATH_MUL_BLOCK_SIZE )
    {
        kernel_fixed_htp_mul_htp( o, o_s, m, m_s, r, r_s );
        return;
    }

    if( o_c >= o_r && o_c >= m_r && o_c > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_c, BMATH_MUL_BLOCK_SIZE );
        recursive_block_htp_mul_htp( o,       o_s, o_r,       mid, m, m_s, m_r,             r, r_s );
        recursive_block_htp_mul_htp( o + mid, o_s, o_r, o_c - mid, m, m_s, m_r, r + mid * r_s, r_s );
        return;
    }

    if( o_r >= m_r && o_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( o_r, BMATH_MUL_BLOCK_SIZE );
        recursive_block_htp_mul_htp( o,             o_s,       mid, o_c, m,       m_s, m_r, r, r_s );
        recursive_block_htp_mul_htp( o + mid * o_s, o_s, o_r - mid, o_c, m + mid, m_s, m_r, r, r_s );
        return;
    }

    if( m_r > BMATH_MUL_BLOCK_SIZE )
    {
        sz_t mid = midof( m_r, BMATH_MUL_BLOCK_SIZE );
        recursive_block_htp_mul_htp( o, o_s, o_r, o_c, m, m_s, mid, r, r_s );
        recursive_block_htp_mul_htp( o, o_s, o_r, o_c, m + mid * m_s, m_s, m_r - mid, r + mid, r_s );
        return;
    }

    /// smaller blocks
    assert( o_r <= BMATH_MUL_BLOCK_SIZE );
    assert( o_c <= BMATH_MUL_BLOCK_SIZE );
    assert( m_r <= BMATH_MUL_BLOCK_SIZE );
    kernel_flexi_htp_mul_htp( o, o_s, o_r, o_c, m, m_s, m_r, r, r_s );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// interface

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul)( const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,mul)( o, m, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( o->cols == m->rows );
    ASSERT( o->rows == r->rows );
    ASSERT( m->cols == r->cols );

    BCATU(bmath_mfx_s,zro)( r );

    recursive_block_mul( o->data, o->stride, o->rows, o->cols, m->data, m->stride, m->cols, r->data, r->stride );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_htp)( const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,mul_htp)( o, m, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( o->cols == m->cols );
    ASSERT( o->rows == r->rows );
    ASSERT( m->rows == r->cols );

    BCATU(bmath_mfx_s,zro)( r );

    bl_t symmetry = ( o == m );

    recursive_block_mul_htp( o->data, o->stride, o->rows, o->cols, m->data, m->stride, m->rows, r->data, r->stride, symmetry );

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

void BCATU(bmath_mfx_s,htp_mul)( const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,htp_mul)( o, m, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( o->rows == m->rows );
    ASSERT( o->cols == r->rows );
    ASSERT( m->cols == r->cols );

    BCATU(bmath_mfx_s,zro)( r );

    bl_t symmetry = ( o == m );

    recursive_block_htp_mul( o->data, o->stride, o->rows, o->cols, m->data, m->stride, m->cols, r->data, r->stride, symmetry );

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

void BCATU(bmath_mfx_s,htp_mul_htp)( const bmath_mfx_s* o, const bmath_mfx_s* m, bmath_mfx_s* r )
{
    if( r == o || r == m )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,htp_mul_htp)( o, m, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( m->cols == o->rows );
    ASSERT( o->cols == r->rows );
    ASSERT( m->rows == r->cols );

    BCATU(bmath_mfx_s,zro)( r );
    recursive_block_htp_mul_htp( o->data, o->stride, o->rows, o->cols, m->data, m->stride, m->rows, r->data, r->stride );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_esp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    if( res == o )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, res->rows, res->cols );
        BCATU(bmath_mfx_s,mul_esp)( o, op, buf );
        BCATU(bmath_mfx_s,cpy)( buf, res );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    // res == op allowed at this point

    ASSERT(  o->cols ==  op->rows );
    ASSERT(  o->rows == res->rows );
    ASSERT( op->cols == res->cols );

    bmath_vfx_s v;
    BCATU(bmath_vfx_s,init)( &v );
    BCATU(bmath_vfx_s,set_size)( &v, op->rows );
    for( uz_t j = 0; j < op->cols; j++ )
    {
        for( uz_t i = 0; i < op->rows; i++ ) v.data[ i ] = op->data[ i * op->stride + j ];
        for( uz_t i = 0; i <  o->rows; i++ )
        {
            res->data[ i * res->stride + j ] = BCATU(bmath,fx,t_vec,mul_vec_esp)( o->data + i * o->stride, v.data, v.size );
        }
    }

    BCATU(bmath_vfx_s,down)( &v );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_htp_esp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols  == op->cols );
    ASSERT( o->rows  == res->rows );
    ASSERT( op->rows == res->cols );

    if( o == op ) // result is symmetric -> we can save half of the work
    {
        if( res == o )
        {
            bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
            BCATU(bmath_mfx_s,set_size)( buf, res->rows, res->cols );
            BCATU(bmath_mfx_s,mul_htp_esp)( o, op, buf );
            BCATU(bmath_mfx_s,cpy)( buf, res );
            BCATU(bmath_mfx_s,discard)( buf );
        }
        else
        {
            for( uz_t i = 0; i < o->rows; i++ )
            {
                const fx_t* vi = o->data + i * o->stride;
                for( uz_t j = 0; j <= i ; j++ )
                {
                    fx_t sum = BCATU(bmath,fx,t_vec,mul_vec_esp)( vi, o->data + j * o->stride, o->cols );
                    res->data[ i * res->stride + j ] = sum;
                    res->data[ j * res->stride + i ] = sum;
                }
            }
        }
        return;
    }

    if( res == o )
    {
        bmath_vfx_s row;
        BCATU(bmath_vfx_s,init)( &row );
        BCATU(bmath_vfx_s,set_size)( &row, o->cols );
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const fx_t* voi = o->data + i *   o->stride;
            BCATU(bmath_vfx_s,zro)( &row );
            for( uz_t j = 0; j < op->rows; j++ )
            {
                row.data[ j ] = BCATU(bmath,fx,t_vec,mul_vec_esp)( voi, op->data + j * op->stride, o->cols );
            }

            fx_t* vri = res->data + i * res->stride;
            for( uz_t k = 0; k < row.size; k++ ) vri[ k ] = row.data[ k ];
        }
        BCATU(bmath_vfx_s,down)( &row );
    }
    else if( res == op )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, res->rows, res->cols );
        BCATU(bmath_mfx_s,mul_htp_esp)( o, op, buf );
        BCATU(bmath_mfx_s,cpy)( buf, res );
        BCATU(bmath_mfx_s,discard)( buf );
    }
    else
    {
        BCATU(bmath_mfx_s,zro)( res );
        for( uz_t i = 0; i < o->rows; i++ )
        {
                  fx_t* vri = res->data + i * res->stride;
            const fx_t* voi =   o->data + i *   o->stride;
            for( uz_t j = 0; j < op->rows; j++ )
            {
                vri[ j ] = BCATU(bmath,fx,t_vec,mul_vec_esp)( voi, op->data + j * op->stride, o->cols );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,htp_mul_esp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->rows  == op->rows );
    ASSERT( o->cols  == res->rows );
    ASSERT( op->cols == res->cols );

    if( o == op ) // result is symmetric -> we can save half of the work
    {
        if( res == o )
        {
            bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
            BCATU(bmath_mfx_s,set_size)( buf, res->rows, res->cols );
            BCATU(bmath_mfx_s,htp_mul_esp)( o, op, buf );
            BCATU(bmath_mfx_s,cpy)( buf, res );
            BCATU(bmath_mfx_s,discard)( buf );
        }
        else
        {
            for( uz_t i = 0; i < o->cols; i++ )
            {
                const fx_t* vi = o->data + i;
                for( uz_t j = 0; j <= i ; j++ )
                {
                    fx_t sum = BCATU(bmath,fx,t_vec,mul_vec_esp_stride)( vi, o->stride, o->data + j, o->stride, o->rows );
                    res->data[ i * res->stride + j ] = sum;
                    res->data[ j * res->stride + i ] = sum;
                }
            }
        }
        return;
    }

    if( res == o || res == op )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, res->rows, res->cols );
        BCATU(bmath_mfx_s,mul_htp_esp)( o, op, buf );
        BCATU(bmath_mfx_s,cpy)( buf, res );
        BCATU(bmath_mfx_s,discard)( buf );
    }
    else
    {
        BCATU(bmath_mfx_s,zro)( res );
        for( uz_t i = 0; i < o->cols; i++ )
        {
                  fx_t* vri = res->data + i * res->stride;
            const fx_t* voi =   o->data + i;
            for( uz_t j = 0; j < op->cols; j++ )
            {
                vri[ j ] = BCATU(bmath,fx,t_vec,mul_vec_esp_stride)( voi, o->stride, op->data + j, op->stride, o->rows );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void mul_udu_htp_( const bmath_mfx_s* u, const bmath_vfx_s* d, bl_t esp, bmath_mfx_s* r )
{
    ASSERT( r != u );
    ASSERT( u->rows == r->cols );
    ASSERT( u->rows == r->rows );
    ASSERT( u->cols == d->size );

    bmath_vfx_s* x = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,set_size)( x, d->size );

    const fx_t* vd = d->data;
          fx_t* vx = x->data;
    for( sz_t i = 0; i < u->rows; i++ )
    {
        const fx_t* ui = u->data + i * u->stride;
              fx_t* ri = r->data + i * r->stride;
        for( sz_t j = 0; j < d->size; j++ ) vx[ j ] = ui[ j ] * vd[ j ];
        for( sz_t j = 0; j < u->rows; j++ )
        {
            const fx_t* uj = u->data + j * u->stride;
            ri[ j ] = esp ? BCATU(bmath,fx,t_vec,mul_vec_esp)( vx, uj, d->size ) :
                            BCATU(bmath,fx,t_vec,mul_vec)(     vx, uj, d->size );
        }
    }

    BCATU(bmath_vfx_s,discard)( x );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_udu_htp)( const bmath_mfx_s* u, const bmath_vfx_s* d, bmath_mfx_s* r )
{
    mul_udu_htp_( u, d, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_udu_htp_esp)( const bmath_mfx_s* u, const bmath_vfx_s* d, bmath_mfx_s* r )
{
    mul_udu_htp_( u, d, true, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void mul_udv_htp_( const bmath_mfx_s* u, const bmath_vfx_s* d, const bmath_mfx_s* v, bl_t esp, bmath_mfx_s* r )
{
    ASSERT( r != u );
    ASSERT( r != v );
    ASSERT( r->rows == u->rows );
    ASSERT( r->cols == v->rows );
    ASSERT( u->cols >= d->size );
    ASSERT( v->cols >= d->size );

    bmath_vfx_s* x = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,set_size)( x, d->size );

    const fx_t* vd = d->data;
          fx_t* vx = x->data;
    for( sz_t i = 0; i < u->rows; i++ )
    {
        const fx_t* ui = u->data + i * u->stride;
              fx_t* ri = r->data + i * r->stride;

        for( sz_t j = 0; j < d->size; j++ ) vx[ j ] = ui[ j ] * vd[ j ];

        for( sz_t j = 0; j < v->rows; j++ )
        {
            const fx_t* vj = v->data + j * v->stride;
            ri[ j ] = esp ? BCATU(bmath,fx,t_vec,mul_vec_esp)( vx, vj, d->size ) :
                            BCATU(bmath,fx,t_vec,mul_vec)(     vx, vj, d->size );
        }
    }

    BCATU(bmath_vfx_s,discard)( x );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_udv_htp)( const bmath_mfx_s* u, const bmath_vfx_s* d, const bmath_mfx_s* v, bmath_mfx_s* r )
{
    mul_udv_htp_( u, d, v, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_udv_htp_esp)( const bmath_mfx_s* u, const bmath_vfx_s* d, const bmath_mfx_s* v, bmath_mfx_s* r )
{
    mul_udv_htp_( u, d, v, true, r );
}

//----------------------------------------------------------------------------------------------------------------------

static void mul_utv_htp_( const bmath_mfx_s* u, const bmath_mfx_s* t, const bmath_mfx_s* v, bl_t esp, bmath_mfx_s* r )
{
    ASSERT( r != u );
    ASSERT( r != v );
    ASSERT( u->rows == r->rows );
    ASSERT( v->rows == r->cols );
    ASSERT( u->cols == t->rows );
    ASSERT( t->cols == v->cols );

    if( BCATU(bmath_mfx_s,is_dag)( t ) )
    {
        bmath_vfx_s* d = BCATU(bmath_vfx_s,create)();
        BCATU(bmath_vfx_s,set_size)( d, sz_min( t->rows, t->cols ) );
        BCATU(bmath_mfx_s,get_dag_vec)( t, d );
        mul_udv_htp_( u, d, v, esp, r );
        BCATU(bmath_vfx_s,discard)( d );
        return;
    }

    bmath_vfx_s* dm = BCATU(bmath_vfx_s,create)(); // main  diag
    bmath_vfx_s* du = BCATU(bmath_vfx_s,create)(); // upper diag (left aligned)
    bmath_vfx_s* dl = BCATU(bmath_vfx_s,create)(); // lower diag (left aligned)
    bmath_vfx_s* x  = BCATU(bmath_vfx_s,create)(); // temp vector

    BCATU(bmath_vfx_s,set_size)( dm, sz_min( t->rows, t->cols ) );

    if( dm->size > 0 )
    {
        BCATU(bmath_vfx_s,set_size)( du, ( ( t->cols > t->rows ) ? dm->size : dm->size - 1 ) );
        BCATU(bmath_vfx_s,set_size)( dl, ( ( t->rows > t->cols ) ? dm->size : dm->size - 1 ) );
        BCATU(bmath_vfx_s,set_size)( x,  dm->size );

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

    const fx_t* vdm = dm->data;
    const fx_t* vdu = du->data;
    const fx_t* vdl = dl->data;
          fx_t* vx  =  x->data;

    for( uz_t i = 0; i < u->rows; i++ )
    {
        const fx_t* ui = u->data + i * u->stride;
              fx_t* ri = r->data + i * r->stride;

        for( sz_t j = 0; j < dm->size; j++ ) vx[ j     ]  = ui[ j     ] * vdm[ j ];
        for( sz_t j = 0; j < du->size; j++ ) vx[ j + 1 ] += ui[ j     ] * vdu[ j ];
        for( sz_t j = 0; j < dl->size; j++ ) vx[ j     ] += ui[ j + 1 ] * vdl[ j ];

        for( sz_t j = 0; j < v->rows; j++ )
        {
            const fx_t* vj = v->data + j * v->stride;
            ri[ j ] = esp ? BCATU(bmath,fx,t_vec,mul_vec_esp)( vx, vj, dm->size ) :
                            BCATU(bmath,fx,t_vec,mul_vec)(     vx, vj, dm->size );
        }
    }

    BCATU(bmath_vfx_s,discard)( dm );
    BCATU(bmath_vfx_s,discard)( du );
    BCATU(bmath_vfx_s,discard)( dl );
    BCATU(bmath_vfx_s,discard)( x );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_utv_htp)( const bmath_mfx_s* u, const bmath_mfx_s* t, const bmath_mfx_s* v, bmath_mfx_s* r )
{
    mul_utv_htp_( u, t, v, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_utv_htp_esp)( const bmath_mfx_s* u, const bmath_mfx_s* t, const bmath_mfx_s* v, bmath_mfx_s* r )
{
    mul_utv_htp_( u, t, v, false, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == o || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,mul_add)( o, b, c, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( o->cols == b->rows );
    ASSERT( o->rows == r->rows );
    ASSERT( b->cols == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r ) BCATU(bmath_mfx_s,cpy)( c, r );

    recursive_block_mul( o->data, o->stride, o->rows, o->cols, b->data, b->stride, b->cols, r->data, r->stride );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_htp_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == o || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,mul_htp_add)( o, b, c, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( o->cols == b->cols );
    ASSERT( o->rows == r->rows );
    ASSERT( b->rows == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r ) BCATU(bmath_mfx_s,cpy)( c, r );

    recursive_block_mul_htp( o->data, o->stride, o->rows, o->cols, b->data, b->stride, b->rows, r->data, r->stride, false );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,htp_mul_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == o || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,htp_mul_add)( o, b, c, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( o->rows == b->rows );
    ASSERT( o->cols == r->rows );
    ASSERT( b->cols == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r ) BCATU(bmath_mfx_s,cpy)( c, r );

    recursive_block_htp_mul( o->data, o->stride, o->rows, o->cols, b->data, b->stride, b->cols, r->data, r->stride, false );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,htp_mul_htp_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == o || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,htp_mul_htp_add)( o, b, c, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( o->rows == b->cols );
    ASSERT( o->cols == r->rows );
    ASSERT( b->rows == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r ) BCATU(bmath_mfx_s,cpy)( c, r );

    recursive_block_htp_mul_htp( o->data, o->stride, o->rows, o->cols, b->data, b->stride, b->rows, r->data, r->stride );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_add_cps)( bl_t htpa, const bmath_mfx_s* a, bl_t htpb, const bmath_mfx_s* b, fx_t c, const bmath_mfx_s* d, fx_t e, bmath_mfx_s* r )
{
    if( r == a || r == b )
    {
        ASSERT( !BCATU(bmath_mfx_s,is_folded)( r ) );
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,mul_add_cps)( htpa, a, htpb, b, c, d, e, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( ( htpa ? a->rows : a->cols ) == ( htpb ? b->cols : b->rows ) );
    ASSERT( ( htpa ? a->cols : a->rows ) == r->rows );
    ASSERT( ( htpb ? b->rows : b->cols ) == r->cols );

    if( d )
    {
        ASSERT( d->rows == r->rows );
        ASSERT( d->cols == r->cols );
        if( c != 0 )
        {
            if( c == e )
            {
                if( d != r ) BCATU(bmath_mfx_s,cpy)( d, r );
            }
            else
            {
                BCATU(bmath_mfx_s,mul_scl_fx)( d, e / c, r );
            }
        }
        else
        {
            if( e == 1.0 )
            {
                if( d != r ) BCATU(bmath_mfx_s,cpy)( d, r );
            }
            else
            {
                BCATU(bmath_mfx_s,mul_scl_fx)( d, e, r );
            }
        }
    }
    else
    {
        BCATU(bmath_mfx_s,zro)( r );
    }

    if( c != 0 )
    {
        bl_t symmetry = ( !d || e == 0.0 ) && ( a == b ) && ( htpa + htpb == 1 );
        switch( htpa * 2 + htpb )
        {
            case 0: recursive_block_mul(         a->data, a->stride, a->rows, a->cols, b->data, b->stride, b->cols, r->data, r->stride           ); break; // a * b
            case 1: recursive_block_mul_htp(     a->data, a->stride, a->rows, a->cols, b->data, b->stride, b->rows, r->data, r->stride, symmetry ); break; // a * b^T
            case 2: recursive_block_htp_mul(     a->data, a->stride, a->rows, a->cols, b->data, b->stride, b->cols, r->data, r->stride, symmetry ); break; // a^T * b
            case 3: recursive_block_htp_mul_htp( a->data, a->stride, a->rows, a->cols, b->data, b->stride, b->rows, r->data, r->stride           ); break; // a^T * b^T
            default: break;
        }
        if( symmetry )
        {
            ASSERT( !BCATU(bmath_mfx_s,is_folded)( r ) );
            for( sz_t i = 0; i < r->rows; i++ )
            {
                for( sz_t j = 0; j < i; j++ ) r->data[ j * r->stride + i ] = r->data[ i * r->stride + j ];
            }
        }
        if( c != 1.0 ) BCATU(bmath_mfx_s,mul_scl_fx)( r, c, r );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_add_cps_selftest)()
{
    BLM_INIT();

    bmath_mfx_s* a  = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* b  = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* ab = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* d  = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* r1 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* r2 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );

    sz_t n  = 25;
    sz_t m1 = 37;
    sz_t m2 = 15;

    fx_t c = 0.5;
    fx_t e = 0.3;

    // a * b
    {
        BCATU(bmath_mfx_s,set_size)(  a, m1, n  );
        BCATU(bmath_mfx_s,set_size)(  b, n,  m2 );
        BCATU(bmath_mfx_s,set_size)(  d, m1, m2 );
        u2_t rval = 1236;
        BCATU(bmath_mfx_s,set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random)( b, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random)( d, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r1, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r2, m1, m2 );
        BCATU(bmath_mfx_s,mul_add_cps)( false, a, false, b, c, d, e, r1 );
        BCATU(bmath_mfx_s,mul_esp)( a, b, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( ab, c, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( d,  e, r2 );
        BCATU(bmath_mfx_s,add)( ab, r2, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, 1E-10 ) );
    }

    // a^T * b
    {
        BCATU(bmath_mfx_s,set_size)(  a, n,  m1  );
        BCATU(bmath_mfx_s,set_size)(  b, n,  m2 );
        BCATU(bmath_mfx_s,set_size)(  d, m1, m2 );
        u2_t rval = 1236;
        BCATU(bmath_mfx_s,set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random)( b, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random)( d, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r1, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r2, m1, m2 );
        BCATU(bmath_mfx_s,mul_add_cps)( true, a, false, b, c, d, e, r1 );
        BCATU(bmath_mfx_s,htp_mul_esp)( a, b, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( ab, c, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( d,  e, r2 );
        BCATU(bmath_mfx_s,add)( ab, r2, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, 1E-10 ) );
    }

    // a * b^T
    {
        BCATU(bmath_mfx_s,set_size)(  a, m1, n  );
        BCATU(bmath_mfx_s,set_size)(  b, m2, n  );
        BCATU(bmath_mfx_s,set_size)(  d, m1, m2 );
        u2_t rval = 1236;
        BCATU(bmath_mfx_s,set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random)( b, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random)( d, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r1, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r2, m1, m2 );
        BCATU(bmath_mfx_s,mul_add_cps)( false, a, true, b, c, d, e, r1 );
        BCATU(bmath_mfx_s,mul_htp_esp)( a, b, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( ab, c, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( d,  e, r2 );
        BCATU(bmath_mfx_s,add)( ab, r2, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, 1E-10 ) );
    }

    // a^T * b^T
    {
        BCATU(bmath_mfx_s,set_size)(  a, n,  m1 );
        BCATU(bmath_mfx_s,set_size)(  b, m2, n  );
        BCATU(bmath_mfx_s,set_size)(  d, m1, m2 );
        u2_t rval = 1236;
        BCATU(bmath_mfx_s,set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random)( b, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random)( d, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r1, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r2, m1, m2 );
        BCATU(bmath_mfx_s,mul_add_cps)( true, a, true, b, c, d, e, r1 );
        BCATU(bmath_mfx_s,htp_mul_htp)( a, b, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( ab, c, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( d,  e, r2 );
        BCATU(bmath_mfx_s,add)( ab, r2, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, 1E-10 ) );
    }

    // a^T * a
    {
        BCATU(bmath_mfx_s,set_size)(  a, m1, n );
        u2_t rval = 1236;
        BCATU(bmath_mfx_s,set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, n, n );
        BCATU(bmath_mfx_s,set_size)( r1, n, n );
        BCATU(bmath_mfx_s,set_size)( r2, n, n );
        BCATU(bmath_mfx_s,mul_add_cps)( true, a, false, a, c, NULL, e, r1 );
        BCATU(bmath_mfx_s,htp_mul_esp)( a, a, r2 );
        BCATU(bmath_mfx_s,mul_scl_fx)( r2, c, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, 1E-10 ) );
    }

    // a * a^T
    {
        BCATU(bmath_mfx_s,set_size)(  a, n, m1 );
        u2_t rval = 1236;
        BCATU(bmath_mfx_s,set_random)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, n, n );
        BCATU(bmath_mfx_s,set_size)( r1, n, n );
        BCATU(bmath_mfx_s,set_size)( r2, n, n );
        BCATU(bmath_mfx_s,mul_add_cps)( false, a, true, a, c, NULL, e, r1 );
        BCATU(bmath_mfx_s,mul_htp_esp)( a, a, r2 );
        BCATU(bmath_mfx_s,mul_scl_fx)( r2, c, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, 1E-10 ) );
    }

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

