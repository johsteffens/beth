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

// we use macros instead of const sz_t because we need them to be const expressions for best compiler optimizations
#define mul_kernel_o_rows ( 4 * 8 )
#define mul_kernel_o_cols ( 4 * 8 )
#define mul_kernel_m_cols ( 4 * 8 )

#ifdef BMATH_AVX
/// mul: Fixed size AVX-Microkernel
static void bmath_mf3_s_mul_avx_fix_kernel( const f3_t* o, sz_t o_s, const f3_t* m, sz_t m_s, f3_t* r, sz_t r_s )
{
    ASSERT( ( mul_kernel_m_cols & 3 ) == 0 );

    #define mul_kernel_m_c4 ( mul_kernel_m_cols >> 2 )

    __m256d r_p4[ mul_kernel_m_c4   ];
    __m256d m_p4[ mul_kernel_o_cols ][ mul_kernel_m_c4 ];

    for( sz_t j = 0; j < mul_kernel_o_cols; j++ )
    {
        const f3_t* mj = m + j * m_s;
        for( sz_t k = 0; k < mul_kernel_m_c4; k++ )
        {
            m_p4[ j ][ k ] = _mm256_loadu_pd( mj + k * 4 );
        }
    }

    for( sz_t i = 0; i < mul_kernel_o_rows; i++ )
    {
        const f3_t* oi = o + i * o_s;
              f3_t* ri = r + i * r_s;

        __m256d o_p4 = _mm256_set1_pd( oi[ 0 ] );

        for( sz_t k = 0; k < mul_kernel_m_c4; k++ )
        {
            r_p4[ k ] = _mm256_mul_pd( m_p4[ 0 ][ k ], o_p4 );
        }

        for( sz_t j = 1; j < mul_kernel_o_cols; j++ )
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

/** mul: Flexible AVX-Microkernel
 *  Allows all combinations o_r, o_c, m_c (including 0)
 *  provided o_c <= mul_kernel_o_cols && m_c <= mul_kernel_m_c4 * 4
 */
static void bmath_mf3_s_mul_avx_flex_kernel( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_c, f3_t* r, sz_t r_s )
{
    #define mul_kernel_m_c4 ( mul_kernel_m_cols >> 2 )

    ASSERT( o_c <= mul_kernel_o_cols );
    ASSERT( m_c <= mul_kernel_m_c4 * 4 );

    const sz_t m_c4l = m_c >> 2;
    const sz_t m_cr  = m_c - m_c4l * 4;
    const sz_t m_c4p = m_cr > 0 ? m_c4l + 1 : m_c4l;

    __m256d r_p4[ mul_kernel_m_c4 ];
    __m256d m_p4[ mul_kernel_o_cols ][ mul_kernel_m_c4 ];

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

static void bmath_mf3_s_f3_t_mul( const f3_t* o, sz_t o_s, sz_t o_r, sz_t o_c, const f3_t* m, sz_t m_s, sz_t m_c, f3_t* r, sz_t r_s )
{
    if( o_r == mul_kernel_o_rows && o_c == mul_kernel_o_cols && m_c == mul_kernel_m_cols )
    {
        #ifdef BMATH_AVX
            bmath_mf3_s_mul_avx_fix_kernel( o, o_s, m, m_s, r, r_s );
        #else

            f3_t r_buf[ mul_kernel_m_cols ];
            for( sz_t i = 0; i < mul_kernel_o_rows; i++ )
            {
                const f3_t* oi = o + i * o_s;

                for( sz_t k = 0; k < mul_kernel_m_cols; k++ ) r_buf[ k ] = 0;

                for( sz_t j = 0; j < mul_kernel_o_cols; j++ )
                {
                    const f3_t* mj = m + j * m_s;
                    f3_t f = oi[ j ];
                    for( sz_t k = 0; k < mul_kernel_m_cols; k++ )
                    {
                        r_buf[ k ] += mj[ k ] * f;
                    }
                }

                f3_t* ri = r + i * r_s;
                for( sz_t k = 0; k < mul_kernel_m_cols; k++ ) ri[ k ] += r_buf[ k ];
            }
        #endif // BMATH_AVX2_FMA

        return;
    }

    if( o_r >= o_c && o_r >= m_c && o_r > mul_kernel_o_rows )
    {
        sz_t mid = ( o_r / ( mul_kernel_o_rows << 1 ) ) * mul_kernel_o_rows;
        mid += ( mid == 0 ) ? 1 : 0;
        bmath_mf3_s_f3_t_mul( o,             o_s,       mid, o_c, m, m_s, m_c,             r, r_s );
        bmath_mf3_s_f3_t_mul( o + mid * o_s, o_s, o_r - mid, o_c, m, m_s, m_c, r + mid * r_s, r_s );
        return;
    }

    if( o_c >= m_c && o_c > mul_kernel_o_cols )
    {
        sz_t mid = ( o_c / ( mul_kernel_o_cols << 1 ) ) * mul_kernel_o_cols;
        mid += ( mid == 0 ) ? 1 : 0;
        bmath_mf3_s_f3_t_mul( o,       o_s, o_r,       mid, m,             m_s, m_c, r, r_s );
        bmath_mf3_s_f3_t_mul( o + mid, o_s, o_r, o_c - mid, m + mid * m_s, m_s, m_c, r, r_s );
        return;
    }

    if( m_c > mul_kernel_m_cols )
    {
        sz_t mid = ( m_c / ( mul_kernel_m_cols << 1 ) ) * mul_kernel_m_cols;
        mid += ( mid == 0 ) ? 1 : 0;
        bmath_mf3_s_f3_t_mul( o, o_s, o_r, o_c, m,       m_s,       mid, r,       r_s );
        bmath_mf3_s_f3_t_mul( o, o_s, o_r, o_c, m + mid, m_s, m_c - mid, r + mid, r_s );
        return;
    }

    /// smaller blocks
    #ifdef BMATH_AVX
        bmath_mf3_s_mul_avx_flex_kernel( o, o_s, o_r, o_c, m, m_s, m_c, r, r_s );
    #else
        f3_t r_buf[ mul_kernel_m_cols ];
        for( sz_t i = 0; i < o_r; i++ )
        {
            const f3_t* oi = o + i * o_s;

            for( sz_t k = 0; k < m_c; k++ ) r_buf[ k ] = 0;

            for( sz_t j = 0; j < o_c; j++ )
            {
                f3_t f = oi[ j ];
                const f3_t* mj = m + j * m_s;
                for( sz_t k = 0; k < m_c; k++ )
                {
                    r_buf[ k ] += mj[ k ] * f;
                }
            }

            f3_t* ri = r + i * r_s;
            for( sz_t k = 0; k < m_c; k++ ) ri[ k ] += r_buf[ k ];
        }
    #endif // BMATH_AVX2_FMA
}

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

//---------------------------------------------------------------------------------------------------------------------

static void bmath_f3_t_mat_mul_htp_add( const f3_t* o, sz_t o_stride, sz_t o_rows, sz_t o_cols, const f3_t* m, sz_t m_stride, sz_t m_rows, f3_t* r, sz_t r_stride, bl_t sym )
{
    const sz_t block_size = 8 * 4; // must be multiple of 4

    if( o_rows >= m_rows && o_rows > block_size )
    {
        sz_t mid = o_rows >> 1;
        bmath_f3_t_mat_mul_htp_add( o,                  o_stride, mid,          o_cols, m, m_stride, m_rows, r,                  r_stride, sym );
        bmath_f3_t_mat_mul_htp_add( o + mid * o_stride, o_stride, o_rows - mid, o_cols, m, m_stride, m_rows, r + mid * r_stride, r_stride, sym );
    }
    else if( m_rows > block_size )
    {
        sz_t mid = m_rows >> 1;
        bmath_f3_t_mat_mul_htp_add( o, o_stride, o_rows, o_cols, m, m_stride, mid, r, r_stride, sym );

        if( !sym || o != m ) // in case of symmetry skip upper triangle of r
        {
            bmath_f3_t_mat_mul_htp_add( o, o_stride, o_rows, o_cols, m + mid * m_stride, m_stride, m_rows - mid, r + mid, r_stride, sym );
        }
    }
    else
    {
        sz_t l = 0;
        const sz_t block_size_p4 = block_size >> 2;

        #ifdef BMATH_AVX2
            for( ; l <= o_cols - block_size; l += block_size )
            {
                for( sz_t i = 0; i < o_rows; i++ )
                {
                    const f3_t* vo = o + o_stride * i + l;
                          f3_t* vr = r + r_stride * i;

                    __m256d o_p4[ block_size_p4 ];

                    for( sz_t k = 0; k < block_size_p4; k++ )
                    {
                        o_p4[ k ] = _mm256_loadu_pd( vo + k * 4 );
                    }

                    for( sz_t j = 0; j < m_rows; j++ )
                    {
                        const f3_t* vm = m + j * m_stride + l;

                        __m256d r_p4 = _mm256_mul_pd( o_p4[ 0 ], _mm256_loadu_pd( vm ) );
                        for( sz_t k = 1; k < block_size_p4; k++ )
                        {
                            #ifdef BMATH_AVX2_FMA
                                r_p4 = _mm256_fmadd_pd( o_p4[ k ], _mm256_loadu_pd( vm + k * 4 ), r_p4 );
                            #else
                                r_p4 = _mm256_add_pd( _mm256_mul_pd( o_p4[ k ], _mm256_loadu_pd( vm + k * 4 ) ), r_p4 );
                            #endif // BMATH_AVX2_FMA
                        }
                        r_p4 = _mm256_hadd_pd( r_p4, r_p4 );

                        vr[ j ] += r_p4[ 0 ] + r_p4[ 2 ];
                    }
                }
            }
        #else  // fallback code
            for( ; l <= o_cols - block_size; l += block_size )
            {
                for( sz_t i = 0; i < o_rows; i++ )
                {
                    const f3_t* vo = o + o_stride * i + l;
                          f3_t* vr = r + r_stride * i;

                    f3_t o_p4[ block_size_p4 ][ 4 ];

                    for( sz_t k = 0; k < block_size_p4; k++ )
                    {
                        o_p4[ k ][ 0 ] = vo[ k * 4 + 0 ];
                        o_p4[ k ][ 1 ] = vo[ k * 4 + 1 ];
                        o_p4[ k ][ 2 ] = vo[ k * 4 + 2 ];
                        o_p4[ k ][ 3 ] = vo[ k * 4 + 3 ];
                    }

                    for( sz_t j = 0; j < m_rows; j++ )
                    {
                        const f3_t* vm = m + j * m_stride + l;

                        f3_t r_p4[ 4 ];
                        r_p4[ 0 ] = o_p4[ 0 ][ 0 ] * vm[ 0 ];
                        r_p4[ 1 ] = o_p4[ 0 ][ 1 ] * vm[ 1 ];
                        r_p4[ 2 ] = o_p4[ 0 ][ 2 ] * vm[ 2 ];
                        r_p4[ 3 ] = o_p4[ 0 ][ 3 ] * vm[ 3 ];

                        for( sz_t k = 1; k < block_size_p4; k++ )
                        {
                            r_p4[ 0 ] += o_p4[ k ][ 0 ] * vm[ k * 4 + 0 ];
                            r_p4[ 1 ] += o_p4[ k ][ 1 ] * vm[ k * 4 + 1 ];
                            r_p4[ 2 ] += o_p4[ k ][ 2 ] * vm[ k * 4 + 2 ];
                            r_p4[ 3 ] += o_p4[ k ][ 3 ] * vm[ k * 4 + 3 ];
                        }

                        r_p4[ 0 ] += r_p4[ 1 ];
                        r_p4[ 2 ] += r_p4[ 3 ];

                        vr[ j ] += r_p4[ 0 ] + r_p4[ 2 ];
                    }
                }
            }
        #endif // BMATH_AVX2

        for( sz_t i = 0; i < o_rows; i++ )
        {
            const f3_t* vo = o + o_stride * i;
                  f3_t* vr = r + r_stride * i;
            for( sz_t j = 0; j < m_rows; j++ )
            {
                const f3_t* vm = m + m_stride * j;
                f3_t sum[ 4 ] = { 0, 0, 0, 0 };
                sz_t k = l;
                for( ; k <= o_cols - 4; k += 4 )
                {
                    sum[ 0 ] += vo[ k + 0 ] * vm[ k + 0 ];
                    sum[ 1 ] += vo[ k + 1 ] * vm[ k + 1 ];
                    sum[ 2 ] += vo[ k + 2 ] * vm[ k + 2 ];
                    sum[ 3 ] += vo[ k + 3 ] * vm[ k + 3 ];
                }
                for( ; k < o_cols; k++ )
                {
                    sum[ 0 ] += vo[ k ] * vm[ k ];
                }

                vr[ j ] += sum[ 0 ] + sum[ 1 ] + sum[ 2 ] + sum[ 3 ];
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_htp( const bmath_mf3_s* o, const bmath_mf3_s* b, bmath_mf3_s* r )
{
    if( r == o || r == b )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, r->rows, r->cols );
        bmath_mf3_s_mul_htp( o, b, buf );
        bmath_mf3_s_cpy( buf, r );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->cols == b->cols );
    ASSERT( o->rows == r->rows );
    ASSERT( b->rows == r->cols );

    bmath_mf3_s_zro( r );

    bl_t symmetry = ( o == b );

    bmath_f3_t_mat_mul_htp_add( o->data, o->stride, o->rows, o->cols, b->data, b->stride, b->rows, r->data, r->stride, symmetry );

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

//---------------------------------------------------------------------------------------------------------------------

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

//---------------------------------------------------------------------------------------------------------------------

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

//---------------------------------------------------------------------------------------------------------------------

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

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_udu_htp( const bmath_mf3_s* u, const bmath_vf3_s* d, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_udu_htp_( u, d, false, r );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_udu_htp_esp( const bmath_mf3_s* u, const bmath_vf3_s* d, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_udu_htp_( u, d, true, r );
}

//---------------------------------------------------------------------------------------------------------------------

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

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_udv_htp( const bmath_mf3_s* u, const bmath_vf3_s* d, const bmath_mf3_s* v, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_udv_htp_( u, d, v, false, r );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_udv_htp_esp( const bmath_mf3_s* u, const bmath_vf3_s* d, const bmath_mf3_s* v, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_udv_htp_( u, d, v, true, r );
}

//---------------------------------------------------------------------------------------------------------------------

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

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_utv_htp( const bmath_mf3_s* u, const bmath_mf3_s* t, const bmath_mf3_s* v, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_utv_htp_( u, t, v, false, r );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_utv_htp_esp( const bmath_mf3_s* u, const bmath_mf3_s* t, const bmath_mf3_s* v, bmath_mf3_s* r )
{
    bmath_mf3_s_mul_utv_htp_( u, t, v, false, r );
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

