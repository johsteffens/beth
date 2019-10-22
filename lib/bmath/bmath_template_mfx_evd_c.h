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

/**********************************************************************************************************************/

/** Collection of algorithms related to eigenvalue decomposition (EVD).
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

bl_t BCATU(bmath_mfx_s,evd_htp_jacobi)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    ASSERT( a->rows == a->cols );

    if( v )
    {
        ASSERT( a != v );
        ASSERT( BCATU(bmath_mfx_s,is_equ_size)( a, v ) );
        BCATU(bmath_mfx_s,one)( v );
    }

    uz_t n = a->rows;
    uz_t max_cycles = 100;

    uz_t cycle;
    for( cycle = 0; cycle < max_cycles; cycle++ )
    {
        fx_t max_dev = 0;
        for( uz_t k = 0; k < n; k++ )
        {
            fx_t* ak =     a->data + k * a->stride;
            fx_t* vk = v ? v->data + k * v->stride : NULL;
            for( uz_t l = k + 1; l < n; l++ )
            {
                fx_t akl = ak[l];
                if( akl == 0 ) continue;
                max_dev = BCATU(fx,max)( max_dev, BCATU(fx,abs)( akl ) );

                fx_t* al =     a->data + l * a->stride;
                fx_t* vl = v ? v->data + l * v->stride : NULL;

                // Using explicit trigonometric functions yields maximum stability
                fx_t theta = 0.5 * atan2( 2 * ak[l], ak[k] - al[l] );
                fx_t s = sin( theta );
                fx_t c = cos( theta );

                fx_t cc = c * c;
                fx_t ss = s * s;

                fx_t akk = ak[k];
                fx_t all = al[l];
                fx_t aklcsx2 = 2 * akl * c * s;

                ak[k] = akk * cc + all * ss + aklcsx2;
                al[l] = all * cc + akk * ss - aklcsx2;
                ak[l] = 0;
                al[k] = 0;

                for( uz_t i = 0; i < n; i++ )
                {
                    if( i != k && i != l )
                    {
                        fx_t* ai = a->data + i * a->stride;
                        fx_t aki = ak[ i ] * c + al[ i ] * s;
                        fx_t ali = al[ i ] * c - ak[ i ] * s;
                        ak[ i ] = aki;
                        al[ i ] = ali;
                        ai[ k ] = aki;
                        ai[ l ] = ali;
                    }

                    if( v )
                    {
                        fx_t vki = vk[ i ];
                        fx_t vli = vl[ i ];
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

//----------------------------------------------------------------------------------------------------------------------

/** In-place EVD for a symmetric matrix.
 *  (Variant of Francis' QR-Algorithm with shift)
 *  Very efficient for large matrices. >20x faster than Jacobi method but slightly less accurate.
 *  BCATU(bmath_mfx_s,evd_htp) for more details.
 */
bl_t BCATU(bmath_mfx_s,evd_htp_qr_xshift)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    uz_t n = a->rows;
    if( n <= 1 ) return true; // nothing to do

    // tridiagonalization
    BCATU(bmath_mfx_s,trd_htp)( a, v );

    bl_t success = true;

    for( uz_t block_n = n; block_n > 1; block_n-- )
    {
        uz_t max_cycles = 100; // usually convergence is reached after 2...3 cycles
        fx_t shift_sum = 0;
        uz_t cycle;
        for( cycle = 0; cycle < max_cycles; cycle++ )
        {
            // aij of lower 2x2 sub-matrix
            fx_t a11 = a->data[ ( block_n - 1 ) * ( a->stride + 1 )     ];
            fx_t a00 = a->data[ ( block_n - 2 ) * ( a->stride + 1 )     ];
            fx_t a01 = a->data[ ( block_n - 2 ) * ( a->stride + 1 ) + 1 ];

            if( ( BCATU(fx,abs)( a01 ) < BCATU(fx,lim_min) ) || ( BCATU(fx,abs)( a01 ) < BCATU(fx,abs)( a11 ) * BCATU(fx,lim_eps) ) )
            {
                a->data[ ( block_n - 2 ) * ( a->stride + 1 ) + 1 ] = 0;
                a->data[ ( block_n - 1 ) * ( a->stride + 1 ) - 1 ] = 0;
                break;
            }
            fx_t p = 0.5 * ( a00 + a11 );
            fx_t d = sqrt( 0.25 * BCATU(fx,sqr)( a00 - a11 ) + a01 * a01 );

            // set shift to eigenvalue of lower 2x2 sub-matrix which is closest to a11
            fx_t shift = ( a11 >= p ) ? p + d : p - d;
            shift_sum += shift;

            for( uz_t i = 0; i < block_n; i++ ) a->data[ i * ( a->stride + 1 ) ] -= shift;

            fx_t* a0 = a->data;

            fx_t r00 = a0[ 0 ];
            fx_t r01 = a0[ 1 ];
            fx_t cp = 1;

            bmath_grt_fx_s gr0;
            BCATU(bmath_grt,fx,s,init_to_annihilate_b)( &gr0, r00, a0[ a->stride ] );

            for( uz_t i = 0; i < block_n - 1; i++ )
            {
                if( v ) BCATU(bmath_grt,fx,s,row_rotate)( &gr0, v->data + i * v->stride, v->data + ( i + 1 ) * v->stride, 0, n );

                fx_t* a1 = a0 + a->stride;

                fx_t r11 = a1[ 1 ] * gr0.c - r01 * gr0.s;
                fx_t r12 = a1[ 2 ] * gr0.c;

                     r00 = r00 * gr0.c + a1[ 0 ] * gr0.s;
                     r01 = r01 * gr0.c + a1[ 1 ] * gr0.s;
                fx_t r02 =               a1[ 2 ] * gr0.s;

                a0[ 0 ] = r00 * cp * gr0.c + r01 * gr0.s;

                bmath_grt_fx_s gr1;
                BCATU(bmath_grt,fx,s,init_to_annihilate_b)( &gr1, r11, ( i == block_n - 2 ) ? 0 : a1[ a->stride + 1 ] );

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
        for( uz_t i = 0; i < block_n; i++ ) a->data[ i * ( a->stride + 1 ) ] += shift_sum;
    }

    if( success )
    {
        // sort by descending eigenvalues
        for( uz_t i = 0; i < n - 1; i++ )
        {
            fx_t vmax = a->data[ i * ( a->stride + 1 ) ];
            uz_t imax = i;
            for( uz_t j = i + 1; j < n; j++ )
            {
                fx_t v = a->data[ j * ( a->stride + 1 ) ];
                imax = ( v > vmax ) ? j : imax;
                vmax = ( v > vmax ) ? v : vmax;
            }
            BCATU(fx,t_swap)( a->data + i * ( a->stride + 1 ), a->data + imax * ( a->stride + 1 ) );
            if( v ) BCATU(bmath_mfx_s,swap_row)( v, i, imax );
        }
    }

    return success;
}

//----------------------------------------------------------------------------------------------------------------------

/** In-place EVD for a symmetric matrix. Approach: TRD, QR with isolated shifting.
 *  (Variant of Francis' QR-Algorithm with shift)
 *  Plain shift is not explicitly added/subtracted. Only the shift effect is applied to the matrix.
 *  This minimizes accuracy-loss similarly as implicit shift would do.
 *  BCATU(bmath_mfx_s,evd) for more details.
 */
bl_t BCATU(bmath_mfx_s,evd_htp_qr_ishift)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    uz_t n = a->rows;
    if( n <= 1 ) return true; // nothing to do

    /// tridiagonalization
    BCATU(bmath_mfx_s,trd_htp)( a, v );

    // qr iteration until smallest non-diag element < offd_limit;

    bl_t success = true;

    for( uz_t block_n = n; block_n > 1; block_n-- )
    {
        uz_t max_cycles = 100; // usually convergence is reached after 2...3 cycles
        uz_t cycle;
        for( cycle = 0; cycle < max_cycles; cycle++ )
        {
            fx_t lambda;
            {
                // aij of lower 2x2 sub-matrix
                fx_t a11 = a->data[ ( block_n - 1 ) * ( a->stride + 1 )     ];
                fx_t a00 = a->data[ ( block_n - 2 ) * ( a->stride + 1 )     ];
                fx_t a01 = a->data[ ( block_n - 2 ) * ( a->stride + 1 ) + 1 ];

                if( ( BCATU(fx,abs)( a01 ) < BCATU(fx,lim_min) ) || ( BCATU(fx,abs)( a01 ) < BCATU(fx,abs)( a11 ) * BCATU(fx,lim_eps) ) )
                {
                    a->data[ ( block_n - 2 ) * ( a->stride + 1 ) + 1 ] = 0;
                    a->data[ ( block_n - 1 ) * ( a->stride + 1 ) - 1 ] = 0;
                    break;
                }
                fx_t p = 0.5 * ( a00 + a11 );
                fx_t d = sqrt( 0.25 * BCATU(fx,sqr)( a00 - a11 ) + a01 * a01 );

                // set shift to eigenvalue of lower 2x2 sub-matrix which is closest to a11
                lambda = ( a11 >= p ) ? p + d : p - d;
            }

            fx_t* pajj = a->data;

            fx_t aij = 0;
            fx_t ajj = pajj[ 0 ];
            fx_t ajk = pajj[ 1 ];

            fx_t ch = 1;

            bmath_grt_fx_s gr_i = { .c = 1, .s = 0 };
            bmath_grt_fx_s gr_j;
            fx_t b = gr_i.c * ajj - gr_i.s * ch * aij;
            BCATU(bmath_grt,fx,s,init_to_annihilate_b)( &gr_j, b - gr_i.c * lambda, ajk );

            for( uz_t j = 0; j < block_n - 1; j++ )
            {
                if( v ) BCATU(bmath_grt,fx,s,row_rotate)( &gr_j, v->data + j * v->stride, v->data + ( j + 1 ) * v->stride, 0, n );

                fx_t akk = pajj[ a->stride + 1 ];
                fx_t akl = ( j == block_n - 2 ) ? 0 : pajj[ a->stride + 2 ];

                fx_t rjj = gr_j.c * b + gr_j.s * ajk;
                fx_t rjk = gr_j.c * gr_i.c * ajk + gr_j.s * akk;
                fx_t rjl = gr_j.s * akl;

                fx_t BCATU(fx,sqr_si_cj_lambda) = BCATU(fx,sqr)( gr_i.s ) * gr_j.c * lambda;

                pajj[ 0 ] =  rjj * gr_j.c * gr_i.c + rjk * gr_j.s + gr_j.c * BCATU(fx,sqr_si_cj_lambda);

                b = gr_j.c * akk - gr_j.s * gr_i.c * ajk;

                bmath_grt_fx_s gr_k;
                BCATU(bmath_grt,fx,s,init_to_annihilate_b)( &gr_k, b - gr_j.c * lambda, akl );

                fx_t offd = -rjj * gr_j.s * gr_i.c * gr_k.c + rjk * gr_k.c * gr_j.c + rjl * gr_k.s - gr_j.s * gr_k.c * BCATU(fx,sqr_si_cj_lambda);
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

            pajj[ 0 ] = b * gr_i.c + BCATU(fx,sqr)( gr_i.s ) * lambda;
        }
        if( cycle == max_cycles ) success = false;
    }

    if( success )
    {
        // sort by descending eigenvalues
        for( uz_t i = 0; i < n - 1; i++ )
        {
            fx_t vmax = a->data[ i * ( a->stride + 1 ) ];
            uz_t imax = i;
            for( uz_t j = i + 1; j < n; j++ )
            {
                fx_t v = a->data[ j * ( a->stride + 1 ) ];
                imax = ( v > vmax ) ? j : imax;
                vmax = ( v > vmax ) ? v : vmax;
            }
            BCATU(fx,t_swap)( a->data + i * ( a->stride + 1 ), a->data + imax * ( a->stride + 1 ) );
            if( v ) BCATU(bmath_mfx_s,swap_row)( v, i, imax );
        }
    }

    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,evd_htp)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    return BCATU(bmath_mfx_s,evd_htp_qr_ishift)( a, v );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// closing template

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

