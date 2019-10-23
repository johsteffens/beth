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

/** Collection of algorithms related to QR and LQ decomposition.
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,qrd_plain)( bmath_mfx_s* u, bmath_mfx_s* a )
{
    if( u )
    {
        ASSERT( u != a );
        ASSERT( u->rows == a->rows );
        ASSERT( u->cols == a->rows /*full*/ || u->cols == a->cols /*thin*/  ); // u may be full or thin (nothing in-between)
        BCATU(bmath_mfx_s,one)( u );
    }

    if( a->rows <= 1 )
    {
        if( u ) a->rows = u->cols;
        return; // nothing else to do
    }

    bmath_grt_fx_s gr;

    for( uz_t j = 0; j < a->cols; j++ )
    {
        // zero lower column
        for( uz_t l = a->rows - 1; l > j; l-- )
        {
            BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, &a->data[ j * a->stride + j ], &a->data[ l * a->stride + j ] );
            if( u ) a->data[ l * a->stride + j ] = BCATU(bmath_grt,fx,s,rho)( &gr );
            BCATU(bmath_mfx_s,drow_rotate)( a, j, l, &gr, j + 1, a->cols );
        }
    }

    if( u ) // reverse construction of u
    {
        for( uz_t j = a->cols - 1; j < a->cols; j-- )
        {
            for( uz_t k = j; k < a->rows - 1; k++ )
            {
                fx_t rho = 0;
                BCATU(fx,t_swap)( &a->data[ j + a->stride * ( k + 1 ) ], &rho );
                BCATU(bmath_grt,fx,s,init_from_rho)( &gr, -rho );
                BCATU(bmath_mfx_s,drow_rotate)( u, j, k + 1, &gr, j, u->cols );
            }
        }
        a->rows = u->cols;
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,qrd_pmt_plain)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_pmt_s* p )
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
        BCATU(bmath_mfx_s,one)( u );
    }

    if( a->rows <= 1 )
    {
        if( u ) a->rows = u->cols;
        return; // nothing else to do
    }

    bmath_vfx_s* v = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,set_size)( v, a->cols );
    BCATU(bmath_vfx_s,zro)( v );

    for( uz_t j = 0; j < a->rows; j++ )
    {
        fx_t* aj = a->data + j * a->stride;
        for( uz_t i = 0; i < a->cols; i++ ) v->data[ i ] += BCATU(fx,sqr)( aj[ i ] );
    }

    bmath_grt_fx_s gr;

    for( uz_t j = 0; j < a->cols; j++ )
    {
        uz_t v_idx =  0;
        fx_t v_max = -1;
        for( uz_t i = j; i < a->cols; i++ )
        {
            v_idx = ( v->data[ i ] > v_max ) ? i : v_idx;
            v_max = ( v->data[ i ] > v_max ) ? v->data[ i ] : v_max;
        }

        BCATU(fx,t_swap)( v->data + j, v->data + v_idx );
        if( p ) uz_t_swap( p->data + j, p->data + v_idx );

        for( uz_t i = 0; i <= j; i++ )
        {
            fx_t* ai = a->data + i * a->stride;
            BCATU(fx,t_swap)( ai + j, ai + v_idx );
        }

        fx_t* aj = a->data + j * a->stride;

        // zero lower column
        for( uz_t l = a->rows - 1; l > j; l-- )
        {
            fx_t* al = a->data + l * a->stride;
            BCATU(fx,t_swap)( al + j, al + v_idx );

            BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, aj + j, al + j );
            if( u ) a->data[ l * a->stride + j ] = BCATU(bmath_grt,fx,s,rho)( &gr );
            BCATU(bmath_mfx_s,drow_rotate)( a, j, l, &gr, j + 1, a->cols );
        }

        for( uz_t i = j; i < a->cols; i++ ) v->data[ i ] -= BCATU(fx,sqr)( aj[ i ] );
    }

    // make diagonal elements of a non-negative
    uz_t n = uz_min( a->cols, a->rows );
    for( uz_t j = 0; j < n; j++ )
    {
        fx_t* aj = a->data + j * a->stride;
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
                fx_t rho = 0;
                BCATU(fx,t_swap)( &a->data[ j + a->stride * ( k + 1 ) ], &rho );
                BCATU(bmath_grt,fx,s,init_from_rho)( &gr, -rho );
                BCATU(bmath_mfx_s,drow_rotate)( u, j, k + 1, &gr, j, u->cols );
            }
        }
        a->rows = u->cols;
    }

    BCATU(bmath_vfx_s,discard)( v );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,lqd_plain)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    if( v )
    {
        ASSERT( v != a );
        ASSERT( v->rows == a->cols );
        ASSERT( v->cols == a->cols || v->cols == a->rows ); // v may be full or thin (nothing in-between)
        BCATU(bmath_mfx_s,one)( v );
    }

    if( a->cols <= 1 )
    {
        if( v ) a->cols = v->cols;
        return; // nothing else to do
    }

    bmath_arr_grt_fx_s grv = BCATU(bmath_arr_grt,fx,of_size)( a->cols );
    bmath_grt_fx_s gr;

    for( uz_t j = 0; j < a->rows; j++ )
    {
        // zero upper row
        for( uz_t l = a->cols - 1; l > j; l-- )
        {
            BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, &a->data[ j * a->stride + j ], &a->data[ j * a->stride + l ] );
            if( v ) a->data[ j * a->stride + l ] = BCATU(bmath_grt,fx,s,rho)( &gr );
            grv.data[ l - 1 ] = gr;
        }

        BCATU(bmath_mfx_s,sweep_dcol_rotate_rev)( a, j, a->cols - 1, &grv, j + 1, a->rows );
    }

    if( v ) // reverse construction of v
    {
        for( uz_t j = a->rows - 1; j < a->rows; j-- )
        {
            for( uz_t k = j; k < a->cols - 1; k++ )
            {
                fx_t rho = 0;
                BCATU(fx,t_swap)( &a->data[ j * a->stride + k + 1 ], &rho );
                BCATU(bmath_grt,fx,s,init_from_rho)( &gr, -rho );
                BCATU(bmath_mfx_s,drow_rotate)( v, j, k + 1, &gr, j, v->cols );
            }
        }
        a->cols = v->cols;
    }

    BCATU(bmath_arr_grt,fx,s,down)( &grv );
}

//----------------------------------------------------------------------------------------------------------------------

static void qrd_accurate_recursive_annihilate( bmath_mfx_s* a, uz_t offset, uz_t start, uz_t end )
{
    const sz_t block_size = 64;

    bmath_grt_fx_s gr;
    if( start + block_size < end )
    {
        uz_t mid = start + ( ( end - start ) >> 1 ) + 1;
        if( start < mid - 1 ) qrd_accurate_recursive_annihilate( a, offset, start, mid - 1 );
        if( mid   < end     ) qrd_accurate_recursive_annihilate( a, offset, mid, end );

        BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, &a->data[ start * a->stride + offset ], &a->data[ mid * a->stride + offset ] );
        a->data[ mid * a->stride + offset ] = BCATU(bmath_grt,fx,s,rho)( &gr );
        BCATU(bmath_mfx_s,drow_rotate)( a, start, mid, &gr, offset + 1, a->cols );
    }
    else
    {
        for( sz_t l = end; l > start; l-- )
        {
            BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, &a->data[ start * a->stride + offset ], &a->data[ l * a->stride + offset ] );
            a->data[ l * a->stride + offset ] = BCATU(bmath_grt,fx,s,rho)( &gr );
            BCATU(bmath_mfx_s,drow_rotate)( a, start, l, &gr, offset + 1, a->cols );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void qrd_accurate_recursive_construct( bmath_mfx_s* u, const bmath_mfx_s* a, uz_t offset, uz_t start, uz_t end )
{
    const sz_t block_size = 64;

    bmath_grt_fx_s gr;

    if( start + block_size < end )
    {
        uz_t mid = start + ( ( end - start ) >> 1 ) + 1;

        fx_t rho = a->data[ offset + a->stride * mid ];
        BCATU(bmath_grt,fx,s,init_from_rho)( &gr, -rho );
        BCATU(bmath_mfx_s,drow_rotate)( u, start, mid, &gr, offset, u->cols );

        if( start < mid - 1 ) qrd_accurate_recursive_construct( u, a, offset, start, mid - 1 );
        if( mid   < end     ) qrd_accurate_recursive_construct( u, a, offset, mid, end );
    }
    else
    {
        for( sz_t k = start + 1; k <= end; k++ )
        {
            fx_t rho = a->data[ offset + a->stride * k ];
            BCATU(bmath_grt,fx,s,init_from_rho)( &gr, -rho );
            BCATU(bmath_mfx_s,drow_rotate)( u, start, k, &gr, offset, u->cols );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** Accurate QRD
 *  Enhanced accuracy via divide & conquer particularly on thin decompositions.
 *  Slightly slower than plain QRD
 */
void BCATU(bmath_mfx_s,qrd_accurate)( bmath_mfx_s* u, bmath_mfx_s* a )
{
    if( u )
    {
        ASSERT( u != a );
        ASSERT( u->rows == a->rows );
        ASSERT( u->cols == a->rows /*full*/ || u->cols == a->cols /*thin*/  ); // u may be full or thin (nothing in-between)
        BCATU(bmath_mfx_s,one)( u );
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
        fx_t* ai = a->data + i * a->stride;
        uz_t end = uz_min( a->cols, i );
        for( uz_t j = 0; j < end; j++ ) ai[ j ] = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** Accurate rank revealing QRD.
 *  Enhanced accuracy via divide & conquer particularly on thin decompositions.
 *  Slightly slower than plain QRD
 */
void BCATU(bmath_mfx_s,qrd_pmt_accurate)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_pmt_s* p )
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
        BCATU(bmath_mfx_s,one)( u );
    }

    if( a->rows <= 1 )
    {
        if( u ) a->rows = u->cols;
        return; // nothing else to do
    }

    uz_t n = uz_min( a->cols, a->rows );

    bmath_vfx_s* v = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,set_size)( v, a->cols );
    BCATU(bmath_vfx_s,zro)( v );

    for( uz_t j = 0; j < a->rows; j++ )
    {
        fx_t* aj = a->data + j * a->stride;
        for( uz_t i = 0; i < a->cols; i++ ) v->data[ i ] += BCATU(fx,sqr)( aj[ i ] );
    }

    for( uz_t j = 0; j < n; j++ )
    {
        uz_t v_idx =  0;
        fx_t v_max = -1;
        for( uz_t i = j; i < a->cols; i++ )
        {
            v_idx = ( v->data[ i ] > v_max ) ? i : v_idx;
            v_max = ( v->data[ i ] > v_max ) ? v->data[ i ] : v_max;
        }

        BCATU(fx,t_swap)( v->data + j, v->data + v_idx );
        if( p ) uz_t_swap( p->data + j, p->data + v_idx );

        BCATU(bmath_mfx_s,swap_col)( a, j, v_idx );

        qrd_accurate_recursive_annihilate( a, j, j, a->rows - 1 );

        fx_t* aj = a->data + j * a->stride;
        for( uz_t i = j; i < a->cols; i++ ) v->data[ i ] -= BCATU(fx,sqr)( aj[ i ] );
    }

    // make diagonal elements of a non-negative
    for( uz_t j = 0; j < n; j++ )
    {
        fx_t* aj = a->data + j * a->stride;
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
        fx_t* ai = a->data + i * a->stride;
        uz_t end = uz_min( a->cols, i );
        for( uz_t j = 0; j < end; j++ ) ai[ j ] = 0;
    }

    BCATU(bmath_vfx_s,discard)( v );
}

//----------------------------------------------------------------------------------------------------------------------

static void lqd_accurate_recursive_annihilate( bmath_mfx_s* a, uz_t offset, uz_t start, uz_t end )
{
    const sz_t block_size = 64;

    bmath_grt_fx_s gr;
    if( start + block_size < end )
    {
        uz_t mid = start + ( ( end - start ) >> 1 ) + 1;
        if( start < mid - 1 ) lqd_accurate_recursive_annihilate( a, offset, start, mid - 1 );
        if( mid   < end     ) lqd_accurate_recursive_annihilate( a, offset, mid, end );

        BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, &a->data[ offset * a->stride + start ], &a->data[ offset * a->stride + mid ] );
        a->data[ offset * a->stride + mid ] = BCATU(bmath_grt,fx,s,rho)( &gr );


        /** Trying to express the recursive approach in column swipes is inefficient.
         *  Column rotations are acceptable at this place because the recursive approach favors cache
         */
        BCATU(bmath_mfx_s,dcol_rotate)( a, start, mid, &gr, offset + 1, a->rows );
    }
    else
    {
        for( sz_t l = end; l > start; l-- )
        {
            BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, &a->data[ offset * a->stride + start ], &a->data[ offset * a->stride + l ] );
            a->data[ offset * a->stride + l ] = BCATU(bmath_grt,fx,s,rho)( &gr );
            BCATU(bmath_mfx_s,dcol_rotate)( a, start, l, &gr, offset + 1, a->rows );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void lqd_accurate_recursive_construct( const bmath_mfx_s* a, bmath_mfx_s* v, uz_t offset, uz_t start, uz_t end )
{
    const sz_t block_size = 64;

    bmath_grt_fx_s gr;

    if( start + block_size < end )
    {
        uz_t mid = start + ( ( end - start ) >> 1 ) + 1;

        fx_t rho = a->data[ a->stride * offset + mid ];
        BCATU(bmath_grt,fx,s,init_from_rho)( &gr, -rho );
        BCATU(bmath_mfx_s,drow_rotate)( v, start, mid, &gr, offset, v->cols );

        if( start < mid - 1 ) lqd_accurate_recursive_construct( a, v, offset, start, mid - 1 );
        if( mid   < end     ) lqd_accurate_recursive_construct( a, v, offset, mid, end );
    }
    else
    {
        for( sz_t k = start + 1; k <= end; k++ )
        {
            fx_t rho = a->data[ offset * a->stride + k ];
            BCATU(bmath_grt,fx,s,init_from_rho)( &gr, -rho );
            BCATU(bmath_mfx_s,drow_rotate)( v, start, k, &gr, offset, v->cols );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** Accurate LQD
 *  Enhanced accuracy via divide & conquer particularly on thin decompositions.
 *  Slightly slower than plain LQD
 */
void BCATU(bmath_mfx_s,lqd_accurate)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    if( v )
    {
        ASSERT( v != a );
        ASSERT( v->rows == a->cols );
        ASSERT( v->cols == a->cols || v->cols == a->rows ); // v may be full or thin (nothing in-between)
        BCATU(bmath_mfx_s,one)( v );
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
        fx_t* ai = a->data + i * a->stride;
        for( uz_t j = i + 1; j < a->cols; j++ ) ai[ j ] = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** Accurate rank revealing LQD.
 *  Enhanced accuracy via divide & conquer particularly on thin decompositions.
 *  Slightly slower than plain LQD
 */
void BCATU(bmath_mfx_s,pmt_lqd_accurate)( bmath_pmt_s* p, bmath_mfx_s* a, bmath_mfx_s* v )
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
        BCATU(bmath_mfx_s,one)( v );
    }

    if( a->cols <= 1 )
    {
        if( v ) a->cols = v->cols;
        return; // nothing else to do
    }

    uz_t n = uz_min( a->cols, a->rows );

    bmath_vfx_s* vec = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,set_size)( vec, a->rows );
    BCATU(bmath_vfx_s,zro)( vec );

    for( uz_t j = 0; j < a->rows; j++ )
    {
        fx_t* aj = a->data + j * a->stride;
        for( uz_t i = 0; i < a->cols; i++ ) vec->data[ j ] += BCATU(fx,sqr)( aj[ i ] );
    }

    for( uz_t j = 0; j < n; j++ )
    {
        uz_t vec_idx =  0;
        fx_t vec_max = -1;
        for( uz_t i = j; i < a->rows; i++ )
        {
            vec_idx = ( vec->data[ i ] > vec_max ) ? i : vec_idx;
            vec_max = ( vec->data[ i ] > vec_max ) ? vec->data[ i ] : vec_max;
        }

        BCATU(fx,t_swap)( vec->data + j, vec->data + vec_idx );
        if( p ) uz_t_swap( p->data + j, p->data + vec_idx );

        BCATU(bmath_mfx_s,swap_row)( a, j, vec_idx );

        lqd_accurate_recursive_annihilate( a, j, j, a->cols - 1 );

        for( uz_t i = j; i < a->rows; i++ ) vec->data[ i ] -= BCATU(fx,sqr)( a->data[ i * a->stride + j ] );
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
        fx_t* ai = a->data + i * a->stride;
        for( uz_t j = i + 1; j < a->cols; j++ ) ai[ j ] = 0;
    }

    BCATU(bmath_vfx_s,discard)( vec );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,qrd)( bmath_mfx_s* u, bmath_mfx_s* a )
{
//    BCATU(bmath_mfx_s,qrd_plain)( u, a );
    BCATU(bmath_mfx_s,qrd_accurate)( u, a );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,qrd_pmt)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_pmt_s* p )
{
//    BCATU(bmath_mfx_s,qrd_pmt_plain)( u, a, p );
    BCATU(bmath_mfx_s,qrd_pmt_accurate)( u, a, p );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,lqd)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    //BCATU(bmath_mfx_s,lqd_plain)( a, v );
    BCATU(bmath_mfx_s,lqd_accurate)( a, v );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,pmt_lqd)( bmath_pmt_s* p, bmath_mfx_s* a, bmath_mfx_s* v )
{
    //BCATU(bmath_mfx_s,lqd_plain)( a, v );
    BCATU(bmath_mfx_s,pmt_lqd_accurate)( p, a, v );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

