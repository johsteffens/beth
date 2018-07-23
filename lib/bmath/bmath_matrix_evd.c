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

/** Collection of algorithms related to eigenvalue decomposition (EVD).
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 *
 *  This header is included in bmath_matrix.h and should not be used in isolation
 *  See bmath_matrix.h for nomenclature.
 */

#include "bmath_matrix.h"
#include "bmath_matrix_evd.h"

#include <stdio.h>

/**********************************************************************************************************************/

bl_t bmath_mf3_s_evd_htp_jacobi( bmath_mf3_s* a, bmath_mf3_s* v )
{
    ASSERT( bmath_mf3_s_is_hsm( a ) );

    if( v )
    {
        ASSERT( a != v );
        ASSERT( bmath_mf3_s_is_equ_size( a, v ) );
    }

    sz_t n = a->rows;
    sz_t max_cycles = 100;

    sz_t cycle;
    for( cycle = 0; cycle < max_cycles; cycle++ )
    {
        f3_t max_dev = 0;
        for( sz_t k = 0; k < n; k++ )
        {
            f3_t* ak =     a->data + k * a->stride;
            f3_t* vk = v ? v->data + k * v->stride : NULL;
            for( sz_t l = k + 1; l < n; l++ )
            {
                f3_t akl = ak[l];
                if( akl == 0 ) continue;
                max_dev = f3_max( max_dev, f3_abs( akl ) );

                f3_t* al =     a->data + l * a->stride;
                f3_t* vl = v ? v->data + l * v->stride : NULL;

                // Using explicit trigonometric functions yields maximum stability
                f3_t theta = 0.5 * atan2( 2 * ak[l], ak[k] - al[l] );
                f3_t s = sin( theta );
                f3_t c = cos( theta );

                f3_t cc = c * c;
                f3_t ss = s * s;

                f3_t akk = ak[k];
                f3_t all = al[l];
                f3_t aklcsx2 = 2 * akl * c * s;

                ak[k] = akk * cc + all * ss + aklcsx2;
                al[l] = all * cc + akk * ss - aklcsx2;
                ak[l] = 0;
                al[k] = 0;

                for( sz_t i = 0; i < n; i++ )
                {
                    if( i != k && i != l )
                    {
                        f3_t* ai = a->data + i * a->stride;
                        f3_t aki = ak[ i ] * c + al[ i ] * s;
                        f3_t ali = al[ i ] * c - ak[ i ] * s;
                        ak[ i ] = aki;
                        al[ i ] = ali;
                        ai[ k ] = aki;
                        ai[ l ] = ali;
                    }

                    if( v )
                    {
                        f3_t vki = vk[ i ];
                        f3_t vli = vl[ i ];
                        vk[ i ] = vki * c + vli * s;
                        vl[ i ] = vli * c - vki * s;
                    }
                }
            }
        }
        if( max_dev == 0 ) break;
    }

    return ( cycle < max_cycles );
}

//---------------------------------------------------------------------------------------------------------------------

/** In-place EVD for a symmetric matrix.
 *  (Variant of Francis' QR-Algorithm with shift)
 *  Very efficient for large matrices. >20x faster than Jacobi method but slightly less accurate.
 *  bmath_mf3_s_evd_htp for more details.
 */
bl_t bmath_mf3_s_evd_htp_qr_xshift( bmath_mf3_s* a, bmath_mf3_s* v )
{
    sz_t n = a->rows;
    if( n <= 1 ) return true; // nothing to do

    // tridiagonalization
    bmath_mf3_s_hsm_decompose_trd_htp( a, v );

    bl_t success = true;

    for( sz_t block_n = n; block_n > 1; block_n-- )
    {
        sz_t max_cycles = 100; // usually convergence is reached after 2...3 cycles
        f3_t shift_sum = 0;
        sz_t cycle;
        for( cycle = 0; cycle < max_cycles; cycle++ )
        {
            // aij of lower 2x2 sub-matrix
            f3_t a11 = a->data[ ( block_n - 1 ) * ( a->stride + 1 )     ];
            f3_t a00 = a->data[ ( block_n - 2 ) * ( a->stride + 1 )     ];
            f3_t a01 = a->data[ ( block_n - 2 ) * ( a->stride + 1 ) + 1 ];

            if( ( f3_abs( a01 ) < f3_lim_min ) || ( f3_abs( a01 ) < f3_abs( a11 ) * f3_lim_eps ) )
            {
                a->data[ ( block_n - 2 ) * ( a->stride + 1 ) + 1 ] = 0;
                a->data[ ( block_n - 1 ) * ( a->stride + 1 ) - 1 ] = 0;
                break;
            }
            f3_t p = 0.5 * ( a00 + a11 );
            f3_t d = sqrt( 0.25 * f3_sqr( a00 - a11 ) + a01 * a01 );

            // set shift to eigenvalue of lower 2x2 sub-matrix which is closest to a11
            f3_t shift = ( a11 >= p ) ? p + d : p - d;
            shift_sum += shift;

            for( sz_t i = 0; i < block_n; i++ ) a->data[ i * ( a->stride + 1 ) ] -= shift;

            f3_t* a0 = a->data;

            f3_t r00 = a0[ 0 ];
            f3_t r01 = a0[ 1 ];
            f3_t cp = 1;

            bmath_grt_f3_s gr0;
            bmath_grt_f3_s_init_to_annihilate_b( &gr0, r00, a0[ a->stride ] );

            for( sz_t i = 0; i < block_n - 1; i++ )
            {
                if( v ) bmath_grt_f3_s_row_rotate( &gr0, v->data + i * v->stride, v->data + ( i + 1 ) * v->stride, 0, n );

                f3_t* a1 = a0 + a->stride;

                f3_t r11 = a1[ 1 ] * gr0.c - r01 * gr0.s;
                f3_t r12 = a1[ 2 ] * gr0.c;

                     r00 = r00 * gr0.c + a1[ 0 ] * gr0.s;
                     r01 = r01 * gr0.c + a1[ 1 ] * gr0.s;
                f3_t r02 =               a1[ 2 ] * gr0.s;

                a0[ 0 ] = r00 * cp * gr0.c + r01 * gr0.s;

                bmath_grt_f3_s gr1;
                bmath_grt_f3_s_init_to_annihilate_b( &gr1, r11, ( i == block_n - 2 ) ? 0 : a1[ a->stride + 1 ] );

                // off-diagonal element
                a1[ 0 ] = a0[ 1 ] = -r00 * gr0.s * cp * gr1.c + r01 * gr0.c * gr1.c + r02 * gr1.s;

                r00 = r11;
                r01 = r12;

                a0 = a1 + 1;

                cp = gr0.c;

                gr0 = gr1;
            }
            a0[ 0 ] = r00 * cp;
        }
        if( cycle == max_cycles ) success = false;
        for( sz_t i = 0; i < block_n; i++ ) a->data[ i * ( a->stride + 1 ) ] += shift_sum;
    }

    if( success )
    {
        // sort by descending eigenvalues
        for( sz_t i = 0; i < n - 1; i++ )
        {
            f3_t vmax = a->data[ i * ( a->stride + 1 ) ];
            sz_t imax = i;
            for( sz_t j = i + 1; j < n; j++ )
            {
                f3_t v = a->data[ j * ( a->stride + 1 ) ];
                imax = ( v > vmax ) ? j : imax;
                vmax = ( v > vmax ) ? v : vmax;
            }
            f3_t_swap( a->data + i * ( a->stride + 1 ), a->data + imax * ( a->stride + 1 ) );
            if( v ) bmath_mf3_s_swap_row( v, i, imax );
        }
    }

    return success;
}

//---------------------------------------------------------------------------------------------------------------------

/** In-place EVD for a symmetric matrix. Approach: TRD, QR with isolated shifting.
 *  (Variant of Francis' QR-Algorithm with shift)
 *  Plain shift is not explicitly added/subtracted. Only the shift effect is applied to the matrix.
 *  This minimizes accuracy-loss similarly as implicit shift would do.
 *  bmath_mf3_s_evd for more details.
 */
bl_t bmath_mf3_s_evd_htp_qr_ishift( bmath_mf3_s* a, bmath_mf3_s* v )
{
    sz_t n = a->rows;
    if( n <= 1 ) return true; // nothing to do

    /// tridiagonalization
    bmath_mf3_s_hsm_decompose_trd_htp( a, v );

    // qr iteration until smallest non-diag element < offd_limit;

    bl_t success = true;

    for( sz_t block_n = n; block_n > 1; block_n-- )
    {
        sz_t max_cycles = 100; // usually convergence is reached after 2...3 cycles
        sz_t cycle;
        for( cycle = 0; cycle < max_cycles; cycle++ )
        {
            f3_t lambda;
            {
                // aij of lower 2x2 sub-matrix
                f3_t a11 = a->data[ ( block_n - 1 ) * ( a->stride + 1 )     ];
                f3_t a00 = a->data[ ( block_n - 2 ) * ( a->stride + 1 )     ];
                f3_t a01 = a->data[ ( block_n - 2 ) * ( a->stride + 1 ) + 1 ];

                if( ( f3_abs( a01 ) < f3_lim_min ) || ( f3_abs( a01 ) < f3_abs( a11 ) * f3_lim_eps ) )
                {
                    a->data[ ( block_n - 2 ) * ( a->stride + 1 ) + 1 ] = 0;
                    a->data[ ( block_n - 1 ) * ( a->stride + 1 ) - 1 ] = 0;
                    break;
                }
                f3_t p = 0.5 * ( a00 + a11 );
                f3_t d = sqrt( 0.25 * f3_sqr( a00 - a11 ) + a01 * a01 );

                // set shift to eigenvalue of lower 2x2 sub-matrix which is closest to a11
                lambda = ( a11 >= p ) ? p + d : p - d;
            }

            f3_t* pajj = a->data;

            f3_t aij = 0;
            f3_t ajj = pajj[ 0 ];
            f3_t ajk = pajj[ 1 ];

            f3_t ch = 1;

            bmath_grt_f3_s gr_i = { .c = 1, .s = 0 };
            bmath_grt_f3_s gr_j;
            f3_t b = gr_i.c * ajj - gr_i.s * ch * aij;
            bmath_grt_f3_s_init_to_annihilate_b( &gr_j, b - gr_i.c * lambda, ajk );

            for( sz_t j = 0; j < block_n - 1; j++ )
            {
                if( v ) bmath_grt_f3_s_row_rotate( &gr_j, v->data + j * v->stride, v->data + ( j + 1 ) * v->stride, 0, n );

                f3_t akk = pajj[ a->stride + 1 ];
                f3_t akl = ( j == block_n - 2 ) ? 0 : pajj[ a->stride + 2 ];

                f3_t rjj = gr_j.c * b + gr_j.s * ajk;
                f3_t rjk = gr_j.c * gr_i.c * ajk + gr_j.s * akk;
                f3_t rjl = gr_j.s * akl;

                f3_t f3_sqr_si_cj_lambda = f3_sqr( gr_i.s ) * gr_j.c * lambda;

                pajj[ 0 ] =  rjj * gr_j.c * gr_i.c + rjk * gr_j.s + gr_j.c * f3_sqr_si_cj_lambda;

                b = gr_j.c * akk - gr_j.s * gr_i.c * ajk;

                bmath_grt_f3_s gr_k;
                bmath_grt_f3_s_init_to_annihilate_b( &gr_k, b - gr_j.c * lambda, akl );

                f3_t offd = -rjj * gr_j.s * gr_i.c * gr_k.c + rjk * gr_k.c * gr_j.c + rjl * gr_k.s - gr_j.s * gr_k.c * f3_sqr_si_cj_lambda;
                pajj[ 1 ] = offd;
                pajj[ a->stride ] = offd;

                pajj += a->stride + 1;
                ch = gr_i.c;
                gr_i = gr_j;
                gr_j = gr_k;

                aij = ajk;
                ajj = akk;
                ajk = akl;
            }

            pajj[ 0 ] = b * gr_i.c + f3_sqr( gr_i.s ) * lambda;
        }
        if( cycle == max_cycles ) success = false;
    }

    if( success )
    {
        // sort by descending eigenvalues
        for( sz_t i = 0; i < n - 1; i++ )
        {
            f3_t vmax = a->data[ i * ( a->stride + 1 ) ];
            sz_t imax = i;
            for( sz_t j = i + 1; j < n; j++ )
            {
                f3_t v = a->data[ j * ( a->stride + 1 ) ];
                imax = ( v > vmax ) ? j : imax;
                vmax = ( v > vmax ) ? v : vmax;
            }
            f3_t_swap( a->data + i * ( a->stride + 1 ), a->data + imax * ( a->stride + 1 ) );
            if( v ) bmath_mf3_s_swap_row( v, i, imax );
        }
    }

    return success;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_evd_htp( bmath_mf3_s* a, bmath_mf3_s* v )
{
    return bmath_mf3_s_evd_htp_qr_ishift( a, v );
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

void bmath_mf3_s_evd_selftest()
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m3 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m4 );

    sz_t n = 100;

    bmath_mf3_s_set_size( m1, n, n );
    u2_t rval = 1236;
    bmath_mf3_s_fill_random( m1, -1, 1, &rval );
    bmath_mf3_s_mul_htp( m1, m1, m1 );

    bmath_mf3_s_set_size_to( m1, m2 );
    bmath_mf3_s_set_size_to( m1, m3 );
    bmath_mf3_s_set_size_to( m1, m4 );

    // jacobi
    {
        bmath_mf3_s_cpy( m1, m2 );
        bmath_mf3_s_evd_htp_jacobi( m2, NULL );
        ASSERT( bmath_mf3_s_is_dag( m2 ) );
        bmath_mf3_s_cpy( m1, m2 );
        bmath_mf3_s_one( m3 );
        bmath_mf3_s_evd_htp_jacobi( m2, m3 );
        ASSERT( bmath_mf3_s_is_dag( m2 ) );
        ASSERT( bmath_mf3_s_is_near_uni( m3, 1E-8 ) );
        bmath_mf3_s_mul( m2, m3, m4 );
        bmath_mf3_s_htp( m3, m3 );
        bmath_mf3_s_mul( m3, m4, m4 );
        ASSERT( bmath_mf3_s_is_near_equ( m1, m4, 1E-8 ) );
    }

    // default evd
    {
        bmath_mf3_s_cpy( m1, m2 );
        bmath_mf3_s_evd_htp( m2, NULL );
        ASSERT( bmath_mf3_s_is_dag( m2 ) );
        bmath_mf3_s_cpy( m1, m2 );
        bmath_mf3_s_one( m3 );
        bmath_mf3_s_evd_htp( m2, m3 );
        ASSERT( bmath_mf3_s_is_dag( m2 ) );
        ASSERT( bmath_mf3_s_is_near_uni( m3, 1E-8 ) );
        bmath_mf3_s_mul( m2, m3, m4 );
        bmath_mf3_s_htp( m3, m3 );
        bmath_mf3_s_mul( m3, m4, m4 );
        ASSERT( bmath_mf3_s_is_near_equ( m1, m4, 1E-8 ) );
    }

    BCORE_LIFE_DOWN();
}

/**********************************************************************************************************************/


