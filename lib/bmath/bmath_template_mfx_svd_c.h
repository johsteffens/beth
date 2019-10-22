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

/** Collection of algorithms related to singular value decomposition (SVD).
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static bl_t ud_is_zero( bmath_mfx_s* a, uz_t row_idx )
{
    fx_t* a00 = a->data + ( a->stride + 1 ) * row_idx;
    fx_t  a01_abs = BCATU(fx,abs)( a00[ 1 ] );
    if( a01_abs < BCATU(fx,lim_min) ||
        (
            a01_abs < BCATU(fx,abs)( a00[ 0             ] ) * BCATU(fx,lim_eps) &&
            a01_abs < BCATU(fx,abs)( a00[ a->stride + 1 ] ) * BCATU(fx,lim_eps) )
        )
    {
        a00[ 1 ] = 0;
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

static void ud_set_zero( bmath_mfx_s* a, uz_t row_idx )
{
    a->data[ ( a->stride + 1 ) * row_idx + 1 ] = 0;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath,mfx,s,svd)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v )
{
    // creating upper-bidiagonal
    BCATU(bmath,mfx,s,ubd)( u, a, v );

    uz_t n = BCATU(fx,min)( a->cols, a->rows );
    if( n <= 1 ) return true; // nothing else to do

    // transposed ur, vr
    bmath_mfx_s* ut = u;
    bmath_mfx_s* vt = v;

    if( u )
    {
        if( u->rows != u->cols )
        {
            // for the thin matrix we only need to track the (lesser) nxn square part
            // and postmultiply it to the matrix
            ut = BCATU(bmath,mfx,s,create)();
            BCATU(bmath,mfx,s,set_size)( ut, n, n );
            BCATU(bmath,mfx,s,one)( ut );
        }
        else
        {
            BCATU(bmath,mfx,s,htp)( u, ut );
        }
    }

    if( v )
    {
        if( v->rows != v->cols )
        {
            vt = BCATU(bmath,mfx,s,create)();
            BCATU(bmath,mfx,s,set_size)( vt, n, n );
            BCATU(bmath,mfx,s,one)( vt );
        }
        else
        {
            BCATU(bmath,mfx,s,htp)( v, vt );
        }
    }

    // remove defective diagonal elements
    // zero defective superdiagonal elements
    bl_t defective = false;
    for( uz_t i = 0; i < n - 1; i++ )
    {
        fx_t* ai = a->data + i * ( a->stride + 1 );
        if( BCATU(fx,abs)( ai[ 1 ] ) < BCATU(fx,abs)( ai[ 0 ] ) * BCATU(fx,lim_eps) )
        {
            ai[ 1 ] = 0;
        }
        else if( BCATU(fx,abs)( ai[ 0 ] ) < BCATU(fx,abs)( ai[ 1 ] ) * BCATU(fx,lim_eps) )
        {
            ai[ 0 ] = 0;
            defective = true;
        }
    }
    if( defective )
    {
        BCATU(bmath,mfx,s,ubd_to_lbd_htp)( a, vt );
        BCATU(bmath,mfx,s,lbd_to_ubd_htp)( ut, a );
    }

    bmath_arr_grt_fx_s gru = BCATU(bmath_arr_grt,fx,of_size)( n );
    bmath_arr_grt_fx_s grv = BCATU(bmath_arr_grt,fx,of_size)( n );

    bl_t success = true;

    // IRB: irreducible block
    uz_t irb_start = 0;

    for(;;)
    {
        /// new IRB start
        while( irb_start < n - 1 && a->data[ ( a->stride + 1 ) * irb_start + 1 ] == 0 ) irb_start++;

        if( irb_start >= n - 1 ) break;

        uz_t irb_end = irb_start + 1;
        while( irb_end < n && a->data[ ( a->stride + 1 ) * ( irb_end - 1 ) + 1 ] != 0 ) irb_end++;

        /// diagonalize IRB
        {
            // in practice convergence hardly ever needs more than 4 cycles
            const uz_t max_cycles = 50;

            bl_t exit_cycle = false;
            uz_t cycle;
            for( cycle = 0; cycle < max_cycles && !exit_cycle; cycle++ )
            {

                fx_t lambda;

                {
                    fx_t* a0 = a->data + ( a->stride + 1 ) * ( irb_end - 2 );
                    fx_t* a1 = a0 + a->stride;

                    fx_t m11 = BCATU(fx,sqr)( a1[ 1 ] );
                    fx_t m00 = BCATU(fx,sqr)( a0[ 0 ] ) + BCATU(fx,sqr)( a0[ 1 ] );
                    fx_t m01 = a0[ 1 ] * a1[ 1 ];

                    fx_t p = 0.5 * ( m00 + m11 );
                    fx_t d = sqrt( 0.25 * BCATU(fx,sqr)( m00 - m11 ) + m01 * m01 );

                    // set shift to eigenvalue of lower 2x2 sub-matrix which is closest to m11
                    lambda = ( m11 >= p ) ? p + d : p - d;
                }

                fx_t* a0 = a->data + ( a->stride + 1 ) * irb_start;
                fx_t* a1 = a0 + a->stride;

                bmath_grt_fx_s gr_l; // left rotation
                bmath_grt_fx_s gr_r; // right rotation

                // left rotation strategically diagonalizes a * aT but creates a1[ 0 ]
                BCATU(bmath_grt,fx,s,init_to_annihilate_b)( &gr_l, BCATU(fx,sqr)( a0[ 0 ] ) + BCATU(fx,sqr)( a0[ 1 ] ) - lambda, a0[ 1 ] * a1[ 1 ] );

                // right rotation annihilates a1[0]
                BCATU(bmath_grt,fx,s,init_to_annihilate_b)( &gr_r, gr_l.c * a1[ 1 ] - gr_l.s * a0[ 1 ], gr_l.s * a0[ 0 ] );

                fx_t a00 = a0[ 0 ];
                fx_t a01 = a0[ 1 ];
                fx_t a11 = a1[ 1 ];

                a0[ 0 ] = gr_r.c * a00 + gr_r.s * a01; a0[ 1 ] = gr_r.c * a01 - gr_r.s * a00;
                a1[ 0 ] =                gr_r.s * a11; a1[ 1 ] = gr_r.c * a11;

                BCATU(bmath_grt,fx,s,row_rotate)( &gr_l, a0, a1, 0, a->cols > irb_start + 2 ? 3 : 2 );
                a1[ 0 ] = 0;

                gru.data[ irb_start ] = gr_l;
                grv.data[ irb_start ] = gr_r;

                // chasing, annihilating off-bidiagonals
                for( uz_t k = irb_start; k < irb_end - 2; k++ )
                {
                    fx_t* ak = a->data + ( a->stride + 1 ) * k + 1;
                    fx_t* al = ak + a->stride;
                    fx_t* am = al + a->stride;
                    BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr_r, ak, ak + 1 );
                    BCATU(bmath_grt,fx,s,rotate)( &gr_r, al, al + 1 );
                    BCATU(bmath_grt,fx,s,rotate)( &gr_r, am, am + 1 );

                    BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr_l, al, am );
                    BCATU(bmath_grt,fx,s,rotate)( &gr_l, al + 1, am + 1 );
                    if( k < irb_end - 3 ) BCATU(bmath_grt,fx,s,rotate)( &gr_l, al + 2, am + 2 );

                    gru.data[ k + 1 ] = gr_l;
                    grv.data[ k + 1 ] = gr_r;

                    exit_cycle = exit_cycle | ud_is_zero( a, k );
                }

                if( ut ) BCATU(bmath,mfx,s,sweep_arow_rotate_fwd)( ut, irb_start, irb_end - 1, &gru, 0, ut->cols );
                if( vt ) BCATU(bmath,mfx,s,sweep_arow_rotate_fwd)( vt, irb_start, irb_end - 1, &grv, 0, vt->cols );

                exit_cycle = exit_cycle | ud_is_zero( a, irb_end - 2 );
            }

            if( cycle == max_cycles )
            {
                // force last super-diagonal zero to allow progression
                ud_set_zero( a, irb_end - 2 );
                success = false;
            }
        }
    }

    BCATU(bmath_arr_grt,fx,s,down)( &grv );
    BCATU(bmath_arr_grt,fx,s,down)( &gru );

    if( success )
    {
        // sorts by descending diagonal values; turns negative values
        for( uz_t i = 0; i < n; i++ )
        {
            fx_t vmax = BCATU(fx,abs)( a->data[ i * ( a->stride + 1 ) ] );
            uz_t imax = i;
            for( uz_t j = i + 1; j < n; j++ )
            {
                fx_t v = BCATU(fx,abs)( a->data[ j * ( a->stride + 1 ) ] );
                imax = ( v > vmax ) ? j : imax;
                vmax = ( v > vmax ) ? v : vmax;
            }

            if( imax != i )
            {
                BCATU(fx,t_swap)( a->data + i * ( a->stride + 1 ), a->data + imax * ( a->stride + 1 ) );
                if( ut ) BCATU(bmath,mfx,s,swap_row)( ut, i, imax );
                if( vt ) BCATU(bmath,mfx,s,swap_row)( vt, i, imax );
            }

            if( a->data[ i * ( a->stride + 1 ) ] < 0 )
            {
                a->data[ i * ( a->stride + 1 ) ] *= -1.0;
                if( vt ) BCATU(bmath,mfx,s,mul_fx_to_row)( vt, -1.0, i );
            }
        }
    }

    if( u )
    {
        if( ut != u )
        {
            BCATU(bmath,mfx,s,mul_htp)( u, ut, u );
            BCATU(bmath,mfx,s,discard)( ut );
        }
        else
        {
            BCATU(bmath,mfx,s,htp)( ut, u );
        }
    }

    if( v )
    {
        if( vt != v )
        {
            BCATU(bmath,mfx,s,mul_htp)( v, vt, v );
            BCATU(bmath,mfx,s,discard)( vt );
        }
        else
        {
            BCATU(bmath,mfx,s,htp)( vt, v );
        }
    }

    return success;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// closing template

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

