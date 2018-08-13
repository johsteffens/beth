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

void bmath_mf3_s_qrd_plain( bmath_mf3_s* u, bmath_mf3_s* a )
{
    if( u )
    {
        ASSERT( u != a );
        ASSERT( u->rows == a->rows );
        ASSERT( u->cols == a->rows /*full*/ || u->cols == a->cols /*thin*/  ); // u may be full or thin (nothing in-between)
        bmath_mf3_s_one( u );
    }

    if( a->rows <= 1 )
    {
        if( u ) a->rows = u->cols;
        return; // nothing else to do
    }

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

void bmath_mf3_s_qrd_pmt_plain( bmath_mf3_s* u, bmath_mf3_s* a, bmath_pmt_s* p )
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

    if( a->rows <= 1 )
    {
        if( u ) a->rows = u->cols;
        return; // nothing else to do
    }

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

void bmath_mf3_s_lqd_plain( bmath_mf3_s* a, bmath_mf3_s* v )
{
    if( v )
    {
        ASSERT( v != a );
        ASSERT( v->rows == a->cols );
        ASSERT( v->cols == a->cols || v->cols == a->rows ); // v may be full or thin (nothing in-between)
        bmath_mf3_s_one( v );
    }

    if( a->cols <= 1 )
    {
        if( v ) a->cols = v->cols;
        return; // nothing else to do
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

static void qrd_accurate_recursive_annihilate( bmath_mf3_s* a, uz_t offset, uz_t start, uz_t end )
{
    const sz_t block_size = 64;

    bmath_grt_f3_s gr;
    if( start + block_size < end )
    {
        uz_t mid = start + ( ( end - start ) >> 1 ) + 1;
        if( start < mid - 1 ) qrd_accurate_recursive_annihilate( a, offset, start, mid - 1 );
        if( mid   < end     ) qrd_accurate_recursive_annihilate( a, offset, mid, end );

        bmath_grt_f3_s_init_and_annihilate_b( &gr, &a->data[ start * a->stride + offset ], &a->data[ mid * a->stride + offset ] );
        a->data[ mid * a->stride + offset ] = bmath_grt_f3_s_rho( &gr );
        bmath_mf3_s_drow_rotate( a, start, mid, &gr, offset + 1, a->cols );
    }
    else
    {
        for( sz_t l = end; l > start; l-- )
        {
            bmath_grt_f3_s_init_and_annihilate_b( &gr, &a->data[ start * a->stride + offset ], &a->data[ l * a->stride + offset ] );
            a->data[ l * a->stride + offset ] = bmath_grt_f3_s_rho( &gr );
            bmath_mf3_s_drow_rotate( a, start, l, &gr, offset + 1, a->cols );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

static void qrd_accurate_recursive_construct( bmath_mf3_s* u, const bmath_mf3_s* a, uz_t offset, uz_t start, uz_t end )
{
    const sz_t block_size = 64;

    bmath_grt_f3_s gr;

    if( start + block_size < end )
    {
        uz_t mid = start + ( ( end - start ) >> 1 ) + 1;

        f3_t rho = a->data[ offset + a->stride * mid ];
        bmath_grt_f3_s_init_from_rho( &gr, -rho );
        bmath_mf3_s_drow_rotate( u, start, mid, &gr, offset, u->cols );

        if( start < mid - 1 ) qrd_accurate_recursive_construct( u, a, offset, start, mid - 1 );
        if( mid   < end     ) qrd_accurate_recursive_construct( u, a, offset, mid, end );
    }
    else
    {
        for( sz_t k = start + 1; k <= end; k++ )
        {
            f3_t rho = a->data[ offset + a->stride * k ];
            bmath_grt_f3_s_init_from_rho( &gr, -rho );
            bmath_mf3_s_drow_rotate( u, start, k, &gr, offset, u->cols );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

/** Accurate QRD
 *  Enhanced accuracy via divide & conquer particularly on thin decompositions.
 *  Slightly slower than plain QRD
 */
void bmath_mf3_s_qrd_accurate( bmath_mf3_s* u, bmath_mf3_s* a )
{
    if( u )
    {
        ASSERT( u != a );
        ASSERT( u->rows == a->rows );
        ASSERT( u->cols == a->rows /*full*/ || u->cols == a->cols /*thin*/  ); // u may be full or thin (nothing in-between)
        bmath_mf3_s_one( u );
    }

    if( a->rows <= 1 )
    {
        if( u ) a->rows = u->cols;
        return; // nothing else to do
    }

    uz_t n = uz_min( a->cols, a->rows );

    for( uz_t j = 0; j < n; j++ )
    {
        qrd_accurate_recursive_annihilate( a, j, j, a->rows - 1 );
    }

    if( u ) // reverse construction of u
    {
        for( uz_t j = n - 1; j < a->cols; j-- )
        {
            qrd_accurate_recursive_construct( u, a, j, j, a->rows - 1 );
        }
        a->rows = u->cols;
    }

    // zero lower tridiagonal
    for( uz_t i = 1; i < a->rows; i++ )
    {
        f3_t* ai = a->data + i * a->stride;
        uz_t end = uz_min( a->cols, i );
        for( uz_t j = 0; j < end; j++ ) ai[ j ] = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------

/** Accurate rank revealing QRD.
 *  Enhanced accuracy via divide & conquer particularly on thin decompositions.
 *  Slightly slower than plain QRD
 */
void bmath_mf3_s_qrd_pmt_accurate( bmath_mf3_s* u, bmath_mf3_s* a, bmath_pmt_s* p )
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

    if( a->rows <= 1 )
    {
        if( u ) a->rows = u->cols;
        return; // nothing else to do
    }

    uz_t n = uz_min( a->cols, a->rows );

    bmath_vf3_s* v = bmath_vf3_s_create();
    bmath_vf3_s_set_size( v, a->cols );
    bmath_vf3_s_zro( v );

    for( uz_t j = 0; j < a->rows; j++ )
    {
        f3_t* aj = a->data + j * a->stride;
        for( uz_t i = 0; i < a->cols; i++ ) v->data[ i ] += f3_sqr( aj[ i ] );
    }

    for( uz_t j = 0; j < n; j++ )
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

        bmath_mf3_s_swap_col( a, j, v_idx );

        qrd_accurate_recursive_annihilate( a, j, j, a->rows - 1 );

        f3_t* aj = a->data + j * a->stride;
        for( uz_t i = j; i < a->cols; i++ ) v->data[ i ] -= f3_sqr( aj[ i ] );
    }

    // make diagonal elements of a non-negative
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
        for( uz_t j = n - 1; j < a->cols; j-- )
        {
            qrd_accurate_recursive_construct( u, a, j, j, a->rows - 1 );
        }
        a->rows = u->cols;
    }

    // zero lower tridiagonal
    for( uz_t i = 1; i < a->rows; i++ )
    {
        f3_t* ai = a->data + i * a->stride;
        uz_t end = uz_min( a->cols, i );
        for( uz_t j = 0; j < end; j++ ) ai[ j ] = 0;
    }

    bmath_vf3_s_discard( v );
}

//---------------------------------------------------------------------------------------------------------------------

static void lqd_accurate_recursive_annihilate( bmath_mf3_s* a, uz_t offset, uz_t start, uz_t end )
{
    const sz_t block_size = 64;

    bmath_grt_f3_s gr;
    if( start + block_size < end )
    {
        uz_t mid = start + ( ( end - start ) >> 1 ) + 1;
        if( start < mid - 1 ) lqd_accurate_recursive_annihilate( a, offset, start, mid - 1 );
        if( mid   < end     ) lqd_accurate_recursive_annihilate( a, offset, mid, end );

        bmath_grt_f3_s_init_and_annihilate_b( &gr, &a->data[ offset * a->stride + start ], &a->data[ offset * a->stride + mid ] );
        a->data[ offset * a->stride + mid ] = bmath_grt_f3_s_rho( &gr );


        /** Trying to express the recursive approach in column swipes is inefficient.
         *  Column rotations are acceptable at this place because the recursive approach favors cache
         */
        bmath_mf3_s_dcol_rotate( a, start, mid, &gr, offset + 1, a->rows );
    }
    else
    {
        for( sz_t l = end; l > start; l-- )
        {
            bmath_grt_f3_s_init_and_annihilate_b( &gr, &a->data[ offset * a->stride + start ], &a->data[ offset * a->stride + l ] );
            a->data[ offset * a->stride + l ] = bmath_grt_f3_s_rho( &gr );
            bmath_mf3_s_dcol_rotate( a, start, l, &gr, offset + 1, a->rows );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

static void lqd_accurate_recursive_construct( const bmath_mf3_s* a, bmath_mf3_s* v, uz_t offset, uz_t start, uz_t end )
{
    const sz_t block_size = 64;

    bmath_grt_f3_s gr;

    if( start + block_size < end )
    {
        uz_t mid = start + ( ( end - start ) >> 1 ) + 1;

        f3_t rho = a->data[ a->stride * offset + mid ];
        bmath_grt_f3_s_init_from_rho( &gr, -rho );
        bmath_mf3_s_drow_rotate( v, start, mid, &gr, offset, v->cols );

        if( start < mid - 1 ) lqd_accurate_recursive_construct( a, v, offset, start, mid - 1 );
        if( mid   < end     ) lqd_accurate_recursive_construct( a, v, offset, mid, end );
    }
    else
    {
        for( sz_t k = start + 1; k <= end; k++ )
        {
            f3_t rho = a->data[ offset * a->stride + k ];
            bmath_grt_f3_s_init_from_rho( &gr, -rho );
            bmath_mf3_s_drow_rotate( v, start, k, &gr, offset, v->cols );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

/** Accurate LQD
 *  Enhanced accuracy via divide & conquer particularly on thin decompositions.
 *  Slightly slower than plain LQD
 */
void bmath_mf3_s_lqd_accurate( bmath_mf3_s* a, bmath_mf3_s* v )
{
    if( v )
    {
        ASSERT( v != a );
        ASSERT( v->rows == a->cols );
        ASSERT( v->cols == a->cols || v->cols == a->rows ); // v may be full or thin (nothing in-between)
        bmath_mf3_s_one( v );
    }

    if( a->cols <= 1 )
    {
        if( v ) a->cols = v->cols;
        return; // nothing else to do
    }

    uz_t n = uz_min( a->cols, a->rows );

    for( uz_t j = 0; j < n; j++ )
    {
        lqd_accurate_recursive_annihilate( a, j, j, a->cols - 1 );
    }

    if( v ) // reverse construction of v
    {
        for( uz_t j = n - 1; j < n; j-- )
        {
            lqd_accurate_recursive_construct( a, v, j, j, a->cols - 1 );
        }
        a->cols = v->cols;
    }

    // zero upper tridiagonal
    for( uz_t i = 0; i < a->rows; i++ )
    {
        f3_t* ai = a->data + i * a->stride;
        for( uz_t j = i + 1; j < a->cols; j++ ) ai[ j ] = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------

/** Accurate rank revealing LQD.
 *  Enhanced accuracy via divide & conquer particularly on thin decompositions.
 *  Slightly slower than plain LQD
 */
void bmath_mf3_s_pmt_lqd_accurate( bmath_pmt_s* p, bmath_mf3_s* a, bmath_mf3_s* v )
{
    if( p )
    {
        ASSERT( p->size == a->rows );
        bmath_pmt_s_one( p );
    }

    if( v )
    {
        ASSERT( v != a );
        ASSERT( v->rows == a->cols );
        ASSERT( v->cols == a->cols || v->cols == a->rows ); // v may be full or thin (nothing in-between)
        bmath_mf3_s_one( v );
    }

    if( a->cols <= 1 )
    {
        if( v ) a->cols = v->cols;
        return; // nothing else to do
    }

    uz_t n = uz_min( a->cols, a->rows );

    bmath_vf3_s* vec = bmath_vf3_s_create();
    bmath_vf3_s_set_size( vec, a->rows );
    bmath_vf3_s_zro( vec );

    for( uz_t j = 0; j < a->rows; j++ )
    {
        f3_t* aj = a->data + j * a->stride;
        for( uz_t i = 0; i < a->cols; i++ ) vec->data[ j ] += f3_sqr( aj[ i ] );
    }

    for( uz_t j = 0; j < n; j++ )
    {
        uz_t vec_idx =  0;
        f3_t vec_max = -1;
        for( uz_t i = j; i < a->rows; i++ )
        {
            vec_idx = ( vec->data[ i ] > vec_max ) ? i : vec_idx;
            vec_max = ( vec->data[ i ] > vec_max ) ? vec->data[ i ] : vec_max;
        }

        f3_t_swap( vec->data + j, vec->data + vec_idx );
        if( p ) uz_t_swap( p->data + j, p->data + vec_idx );

        bmath_mf3_s_swap_row( a, j, vec_idx );

        lqd_accurate_recursive_annihilate( a, j, j, a->cols - 1 );

        for( uz_t i = j; i < a->rows; i++ ) vec->data[ i ] -= f3_sqr( a->data[ i * a->stride + j ] );
    }

    // make diagonal elements of a non-negative
    for( uz_t j = 0; j < n; j++ )
    {
        if( a->data[ j * ( a->stride + 1 ) ] < 0 )
        {
            if( v ) v->data[ j * ( v->stride + 1 ) ] = -1;
            for( uz_t i = j; i < a->rows; i++ ) a->data[ i * a->stride + j ] = -a->data[ i * a->stride + j ];
        }
    }

    if( v ) // reverse construction of v
    {
        for( uz_t j = n - 1; j < n; j-- )
        {
            lqd_accurate_recursive_construct( a, v, j, j, a->cols - 1 );
        }
        a->cols = v->cols;
    }

    // zero upper tridiagonal
    for( uz_t i = 0; i < a->rows; i++ )
    {
        f3_t* ai = a->data + i * a->stride;
        for( uz_t j = i + 1; j < a->cols; j++ ) ai[ j ] = 0;
    }

    bmath_vf3_s_discard( vec );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_qrd( bmath_mf3_s* u, bmath_mf3_s* a )
{
//    bmath_mf3_s_qrd_plain( u, a );
    bmath_mf3_s_qrd_accurate( u, a );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_qrd_pmt( bmath_mf3_s* u, bmath_mf3_s* a, bmath_pmt_s* p )
{
//    bmath_mf3_s_qrd_pmt_plain( u, a, p );
    bmath_mf3_s_qrd_pmt_accurate( u, a, p );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_lqd( bmath_mf3_s* a, bmath_mf3_s* v )
{
    //bmath_mf3_s_lqd_plain( a, v );
    bmath_mf3_s_lqd_accurate( a, v );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_pmt_lqd( bmath_pmt_s* p, bmath_mf3_s* a, bmath_mf3_s* v )
{
    //bmath_mf3_s_lqd_plain( a, v );
    bmath_mf3_s_pmt_lqd_accurate( p, a, v );
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

