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

/** Collection of algorithms related to singular value decomposition (SVD).
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 *
 *  This header is included in bmath_matrix.h and should not be used in isolation
 *  See bmath_matrix.h for nomenclature.
 */

#include "bmath_matrix.h"
#include "bmath_matrix_evd.h"

#include <stdio.h>

/**********************************************************************************************************************/

/** SVD for a->rows >= a->cols
 *  Method: upper bidiagonalization + chase algorithm with implicit shift
 */
bl_t bmath_mf3_s_svd_htp_ubd( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v )
{
    ASSERT( a->rows >= a->cols );

    // creating upper-bidiagonal
    bmath_mf3_s_decompose_ubd_htp( u, a, v );

    sz_t n = a->cols;
    if( n <= 1 ) return true; // nothing else to do

    bl_t success = true;

    // in practice convergence hardly ever needs more than 4 cycles
    const sz_t max_cyles = 100;

    for( sz_t block_n = n; block_n > 1; block_n-- )
    {
        sz_t cycle;
        for( cycle = 0; cycle < max_cyles; cycle++ )
        {
            f3_t lambda;

            {
                f3_t* a0 = a->data + ( a->stride + 1 ) * ( block_n - 2 );
                f3_t* a1 = a0 + a->stride;

                // exit cycle if bottom off-diagonal is zero
                if( ( f3_abs( a0[ 1 ] ) < f3_lim_min ) || ( f3_abs( a0[ 1 ] ) < f3_abs( a1[ 1 ] ) * f3_lim_eps ) )
                {
                    a0[ 1 ] = 0;
                    break;
                }

                f3_t m11 = f3_sqr( a1[ 1 ] );
                f3_t m00 = f3_sqr( a0[ 0 ] ) + f3_sqr( a0[ 1 ] );
                f3_t m01 = a0[ 1 ] * a1[ 1 ];

                f3_t p = 0.5 * ( m00 + m11 );
                f3_t d = sqrt( 0.25 * f3_sqr( m00 - m11 ) + m01 * m01 );

                // set shift to eigenvalue of lower 2x2 sub-matrix which is closest to m11
                lambda = ( m11 >= p ) ? p + d : p - d;
            }

            f3_t* a0 = a->data;
            f3_t* a1 = a->data + a->stride;

            bmath_grt_f3_s gr_l; // left rotation
            bmath_grt_f3_s gr_r; // right rotation

            // left rotation strategically diagonalizes a * aT but creates a1[ 0 ]
            bmath_grt_f3_s_init_to_annihilate_b( &gr_l, f3_sqr( a0[ 0 ] ) + f3_sqr( a0[ 1 ] ) - lambda, a0[ 1 ] * a1[ 1 ] );

            // right rotation annihilates a1[0]
            bmath_grt_f3_s_init_to_annihilate_b( &gr_r, gr_l.c * a1[ 1 ] - gr_l.s * a0[ 1 ], gr_l.s * a0[ 0 ] );

            f3_t a00 = a0[ 0 ];
            f3_t a01 = a0[ 1 ];
            f3_t a11 = a1[ 1 ];

            a0[ 0 ] = gr_r.c * a00 + gr_r.s * a01; a0[ 1 ] = gr_r.c * a01 - gr_r.s * a00;
            a1[ 0 ] =                gr_r.s * a11; a1[ 1 ] = gr_r.c * a11;
            bmath_grt_f3_s_row_rotate( &gr_l, a0, a1, 0, 3 );
            a1[ 0 ] = 0;

            if( u ) bmath_grt_f3_s_row_rotate( &gr_l, u->data, u->data + u->stride, 0, u->cols );
            if( v ) bmath_grt_f3_s_row_rotate( &gr_r, v->data, v->data + v->stride, 0, v->cols );

            // chasing, annihilating off-bidiagonals
            for( sz_t k = 0; k < block_n - 2; k++ )
            {
                f3_t* ak = a->data + ( a->stride + 1 ) * k + 1;
                f3_t* al = ak + a->stride;
                f3_t* am = al + a->stride;
                bmath_grt_f3_s_init_and_annihilate_b( &gr_r, ak, ak + 1 );
                bmath_grt_f3_s_rotate( &gr_r, al, al + 1 );
                bmath_grt_f3_s_rotate( &gr_r, am, am + 1 );

                bmath_grt_f3_s_init_and_annihilate_b( &gr_l, al, am );
                bmath_grt_f3_s_rotate( &gr_l, al + 1, am + 1 );
                if( k < block_n - 3 ) bmath_grt_f3_s_rotate( &gr_l, al + 2, am + 2 );

                if( u ) bmath_grt_f3_s_row_rotate( &gr_l, u->data + u->stride * ( k + 1 ), u->data + u->stride * ( k + 2 ), 0, u->cols );
                if( v ) bmath_grt_f3_s_row_rotate( &gr_r, v->data + v->stride * ( k + 1 ), v->data + v->stride * ( k + 2 ), 0, v->cols );
            }
        }
        if( cycle == max_cyles ) success = false;
    }

    return success;
}

//---------------------------------------------------------------------------------------------------------------------

/** SVD for a->rows >= a->cols
 *  Method: lower bidiagonalization + chase algorithm with implicit shift
 */
bl_t bmath_mf3_s_svd_htp_lbd( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v )
{
    ASSERT( a->cols >= a->rows );

    // creating lower-bidiagonal
    bmath_mf3_s_decompose_lbd_htp( u, a, v );

    sz_t n = a->rows;
    if( n <= 1 ) return true; // nothing else to do

    bl_t success = true;

    // in practice convergence hardly ever needs more than 4 cycles
    const sz_t max_cyles = 100;

    for( sz_t block_n = n; block_n > 1; block_n-- )
    {
        sz_t cycle;
        for( cycle = 0; cycle < max_cyles; cycle++ )
        {
            f3_t lambda;

            {
                f3_t* a0 = a->data + ( a->stride + 1 ) * ( block_n - 2 );
                f3_t* a1 = a0 + a->stride;

                // exit cycle if bottom off-diagonal is zero
                if( ( f3_abs( a1[ 0 ] ) < f3_lim_min ) || ( f3_abs( a1[ 0 ] ) < f3_abs( a1[ 1 ] ) * f3_lim_eps ) )
                {
                    a1[ 0 ] = 0;
                    break;
                }

                f3_t m11 = f3_sqr( a1[ 1 ] );
                f3_t m00 = f3_sqr( a0[ 0 ] ) + f3_sqr( a1[ 0 ] );
                f3_t m01 = a1[ 0 ] * a1[ 1 ];

                f3_t p = 0.5 * ( m00 + m11 );
                f3_t d = sqrt( 0.25 * f3_sqr( m00 - m11 ) + m01 * m01 );

                // set shift to eigenvalue of lower 2x2 sub-matrix which is closest to m11
                lambda = ( m11 >= p ) ? p + d : p - d;
            }

            f3_t* a0 = a->data;
            f3_t* a1 = a->data + a->stride;

            bmath_grt_f3_s gr_l; // left rotation
            bmath_grt_f3_s gr_r; // right rotation

            // left rotation strategically diagonalizes aT * a but creates a0[ 1 ]
            bmath_grt_f3_s_init_to_annihilate_b( &gr_r, f3_sqr( a0[ 0 ] ) + f3_sqr( a1[ 0 ] ) - lambda, a1[ 0 ] * a1[ 1 ] );

            // right rotation annihilates a0[ 1 ]
            bmath_grt_f3_s_init_to_annihilate_b( &gr_l, gr_r.c * a1[ 1 ] - gr_r.s * a1[ 0 ], gr_r.s * a0[ 0 ] );

            f3_t a00 = a0[ 0 ];
            f3_t a10 = a1[ 0 ];
            f3_t a11 = a1[ 1 ];

            a0[ 0 ] = gr_l.c * a00 + gr_l.s * a10; a1[ 0 ] = gr_l.c * a10 - gr_l.s * a00;
            a0[ 1 ] =                gr_l.s * a11; a1[ 1 ] = gr_l.c * a11;
            bmath_grt_f3_s_col_rotate( &gr_r, a0, a0 + 1, a->stride, 0, 3 );
            a0[ 1 ] = 0;

            if( u ) bmath_grt_f3_s_row_rotate( &gr_l, u->data, u->data + u->stride, 0, u->cols );
            if( v ) bmath_grt_f3_s_row_rotate( &gr_r, v->data, v->data + v->stride, 0, v->cols );

            // chasing, annihilating off-bidiagonals
            for( sz_t k = 0; k < block_n - 2; k++ )
            {
                f3_t* ak = a->data + ( a->stride + 1 ) * ( k + 1 ) - 1;
                f3_t* al = ak + a->stride;
                f3_t* am = al + a->stride;

                bmath_grt_f3_s_init_and_annihilate_b( &gr_l, ak, al );
                bmath_grt_f3_s_rotate( &gr_l, ak + 1, al + 1 );
                bmath_grt_f3_s_rotate( &gr_l, ak + 2, al + 2 );

                bmath_grt_f3_s_init_and_annihilate_b( &gr_r, ak + 1, ak + 2 );
                bmath_grt_f3_s_rotate( &gr_r, al + 1, al + 2 );
                if( k < block_n - 3 ) bmath_grt_f3_s_rotate( &gr_r, am + 1, am + 2 );

                if( u ) bmath_grt_f3_s_row_rotate( &gr_l, u->data + u->stride * ( k + 1 ), u->data + u->stride * ( k + 2 ), 0, u->cols );
                if( v ) bmath_grt_f3_s_row_rotate( &gr_r, v->data + v->stride * ( k + 1 ), v->data + v->stride * ( k + 2 ), 0, v->cols );

            }
        }
        if( cycle == max_cyles ) success = false;
    }

    return success;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_svd_htp(  bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v )
{
    bl_t success = false;

    if( a->rows >= a->cols )
    {
        success = bmath_mf3_s_svd_htp_ubd( u, a, v );
    }
    else
    {
        success = bmath_mf3_s_svd_htp_lbd( u, a, v );
    }

    if( !success ) return false;

    // sorts by descending diagonal values; turns negative values
    sz_t n = sz_min( a->rows, a->cols );
    for( sz_t i = 0; i < n; i++ )
    {
        f3_t vmax = f3_abs( a->data[ i * ( a->stride + 1 ) ] );
        sz_t imax = i;
        for( sz_t j = i + 1; j < n; j++ )
        {
            f3_t v = f3_abs( a->data[ j * ( a->stride + 1 ) ] );
            imax = ( v > vmax ) ? j : imax;
            vmax = ( v > vmax ) ? v : vmax;
        }
        if( imax != i )
        {
            f3_t_swap( a->data + i * ( a->stride + 1 ), a->data + imax * ( a->stride + 1 ) );
            if( u ) bmath_mf3_s_swap_row( u, i, imax );
            if( v ) bmath_mf3_s_swap_row( v, i, imax );
        }

        if( a->data[ i * ( a->stride + 1 ) ] < 0 )
        {
            a->data[ i * ( a->stride + 1 ) ] *= -1.0;
            if( v ) bmath_mf3_s_mul_f3_to_row( v, -1.0, i );
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

void bmath_mf3_s_svd_selftest()
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m0 );
    BCORE_LIFE_CREATE( bmath_mf3_s, u );
    BCORE_LIFE_CREATE( bmath_mf3_s, a );
    BCORE_LIFE_CREATE( bmath_mf3_s, v );
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );

    for( sz_t test_id = 0; test_id < 3; test_id++ )
    {
        sz_t n = ( test_id == 0 ) ? 20 : 10;
        sz_t m = ( test_id == 2 ) ? 20 : 10;

        bmath_mf3_s_set_size( m0, n, m );
        bmath_mf3_s_set_size( a, n, m );
        u2_t rval = 1236;
        bmath_mf3_s_fill_random( m0, -1, 1, &rval );

        bmath_mf3_s_set_size( u, n, n );
        bmath_mf3_s_set_size( v, m, m );
        bmath_mf3_s_one( u );
        bmath_mf3_s_one( v );

        bmath_mf3_s_cpy( m0, a );
        bmath_mf3_s_svd_htp( NULL, a, NULL );
        ASSERT( bmath_mf3_s_is_dag( a ) );

        // a = u'T * a' * v
        bmath_mf3_s_cpy( m0, a );
        bmath_mf3_s_svd_htp( u, a, v );
        ASSERT( bmath_mf3_s_is_dag( a ) );

        ASSERT( bmath_mf3_s_is_near_uni( u, 1E-8 ) );
        ASSERT( bmath_mf3_s_is_near_uni( v, 1E-8 ) );

        bmath_mf3_s_set_size( m1, n, m );
        bmath_mf3_s_mul( a, v, m1 );
        bmath_mf3_s_htp( u, u );
        bmath_mf3_s_mul( u, m1, m1 );

        ASSERT( bmath_mf3_s_is_near_equ( m0, m1, 1E-8 ) );
    }

    BCORE_LIFE_DOWN();
}

/**********************************************************************************************************************/


