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

/** Collection of algorithms related to QR and LQ decomposition.
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 *
 *  This header is included in bmath_mf3.h and should not be used in isolation
 *  See bmath_mf3.h for nomenclature.
 */

#include "bmath_mf3.h"
#include "bmath_mf3_qrd.h"

#include <stdio.h>

/**********************************************************************************************************************/

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_qrd( bmath_mf3_s* u, bmath_mf3_s* a )
{
    if( u )
    {
        ASSERT( u != a );
        ASSERT( u->rows == a->rows );
        ASSERT( u->cols == a->rows /*full*/ || u->cols == a->cols /*thin*/  ); // u may be full or thin (nothing in-between)
        bmath_mf3_s_one( u );
    }

    if( a->rows <= 1 ) return; // nothing to do

    bmath_grt_f3_s gr;

    for( uz_t j = 0; j < a->cols; j++ )
    {
        // zero lower column
        for( uz_t l = a->rows - 1; l > j; l-- )
        {
            bmath_grt_f3_s_init_and_annihilate_b( &gr, &a->data[ j * a->stride + j ], &a->data[ l * a->stride + j ] );
            if( u ) a->data[ l * a->stride + j ] = bmath_grt_f3_s_rho( &gr );
            bmath_mf3_s_drow_rotate( a, j, l, &gr, j + 1, a->cols );
        }
    }

    if( u ) // reverse construction of u
    {
        for( uz_t j = a->cols - 1; j < a->cols; j-- )
        {
            for( uz_t k = j; k < a->rows - 1; k++ )
            {
                f3_t rho = 0;
                f3_t_swap( &a->data[ j + a->stride * ( k + 1 ) ], &rho );
                bmath_grt_f3_s_init_from_rho( &gr, -rho );
                bmath_mf3_s_drow_rotate( u, j, k + 1, &gr, j, u->cols );
            }
        }
        a->rows = u->cols;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_qrd_pmt( bmath_mf3_s* u, bmath_mf3_s* a, bmath_pmt_s* p )
{
    if( p )
    {
        ASSERT( p->size == a->cols );
        bmath_pmt_s_one( p );
    }

    if( u )
    {
        ASSERT( u != a );
        ASSERT( u->rows == a->rows );
        ASSERT( u->cols == a->rows /*full*/ || u->cols == a->cols /*thin*/  ); // u may be full or thin (nothing in-between)
        bmath_mf3_s_one( u );
    }

    if( a->rows <= 1 ) return; // nothing to do

    bmath_vf3_s* v = bmath_vf3_s_create();
    bmath_vf3_s_set_size( v, a->cols );
    bmath_vf3_s_zro( v );

    for( uz_t j = 0; j < a->rows; j++ )
    {
        f3_t* aj = a->data + j * a->stride;
        for( uz_t i = 0; i < a->cols; i++ ) v->data[ i ] += f3_sqr( aj[ i ] );
    }

    bmath_grt_f3_s gr;

    for( uz_t j = 0; j < a->cols; j++ )
    {
        uz_t v_idx =  0;
        f3_t v_max = -1;
        for( uz_t i = j; i < a->cols; i++ )
        {
            v_idx = ( v->data[ i ] > v_max ) ? i : v_idx;
            v_max = ( v->data[ i ] > v_max ) ? v->data[ i ] : v_max;
        }

        f3_t_swap( v->data + j, v->data + v_idx );
        if( p ) uz_t_swap( p->data + j, p->data + v_idx );

        for( uz_t i = 0; i <= j; i++ )
        {
            f3_t* ai = a->data + i * a->stride;
            f3_t_swap( ai + j, ai + v_idx );
        }

        f3_t* aj = a->data + j * a->stride;

        // zero lower column
        for( uz_t l = a->rows - 1; l > j; l-- )
        {
            f3_t* al = a->data + l * a->stride;
            f3_t_swap( al + j, al + v_idx );

            bmath_grt_f3_s_init_and_annihilate_b( &gr, aj + j, al + j );
            if( u ) a->data[ l * a->stride + j ] = bmath_grt_f3_s_rho( &gr );
            bmath_mf3_s_drow_rotate( a, j, l, &gr, j + 1, a->cols );
        }

        for( uz_t i = j; i < a->cols; i++ ) v->data[ i ] -= f3_sqr( aj[ i ] );
    }

    // make diagonal elements of a non-negative
    uz_t n = uz_min( a->cols, a->rows );
    for( uz_t j = 0; j < n; j++ )
    {
        f3_t* aj = a->data + j * a->stride;
        if( aj[ j ] < 0 )
        {
            if( u ) u->data[ j * ( u->stride + 1 ) ] = -1;
            for( uz_t i = j; i < a->cols; i++ ) aj[ i ] = -aj[ i ];
        }
    }

    if( u ) // reverse construction of u
    {
        for( uz_t j = a->cols - 1; j < a->cols; j-- )
        {
            for( uz_t k = j; k < a->rows - 1; k++ )
            {
                f3_t rho = 0;
                f3_t_swap( &a->data[ j + a->stride * ( k + 1 ) ], &rho );
                bmath_grt_f3_s_init_from_rho( &gr, -rho );
                bmath_mf3_s_drow_rotate( u, j, k + 1, &gr, j, u->cols );
            }
        }
        a->rows = u->cols;
    }

    bmath_vf3_s_discard( v );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_lqd( bmath_mf3_s* a, bmath_mf3_s* v )
{
    if( a->cols <= 1 ) return; // nothing to do

    if( v )
    {
        ASSERT( v != a );
        ASSERT( v->rows == a->cols );
        ASSERT( v->cols == a->cols || v->cols == a->rows ); // v may be full or thin (nothing in-between)
    }

    bmath_arr_grt_f3_s grv = bmath_arr_grt_f3_of_size( a->cols );
    bmath_grt_f3_s gr;

    for( uz_t j = 0; j < a->rows; j++ )
    {
        // zero upper row
        for( uz_t l = a->cols - 1; l > j; l-- )
        {
            bmath_grt_f3_s_init_and_annihilate_b( &gr, &a->data[ j * a->stride + j ], &a->data[ j * a->stride + l ] );
            if( v ) a->data[ j * a->stride + l ] = bmath_grt_f3_s_rho( &gr );
            grv.data[ l - 1 ] = gr;
        }

        bmath_mf3_s_sweep_dcol_rotate_rev( a, j, a->cols - 1, &grv, j + 1, a->rows );
    }

    if( v ) // reverse construction of v
    {
        bmath_mf3_s_one( v );
        for( uz_t j = a->rows - 1; j < a->rows; j-- )
        {
            for( uz_t k = j; k < a->cols - 1; k++ )
            {
                f3_t rho = 0;
                f3_t_swap( &a->data[ j * a->stride + k + 1 ], &rho );
                bmath_grt_f3_s_init_from_rho( &gr, -rho );
                bmath_mf3_s_drow_rotate( v, j, k + 1, &gr, j, v->cols );
            }
        }
        a->cols = v->cols;
    }

    bmath_arr_grt_f3_s_down( &grv );
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

