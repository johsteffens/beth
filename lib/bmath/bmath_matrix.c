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

/** Matrix types and operations.
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 */

#include "bmath_matrix.h"
#include "bmath_spect_matrix.h"
#include "bmath_fourier.h"
#include "bmath_grt_f3.h"

#include <stdio.h>

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bmath_matrix, bmath_mf3_s )
"{"
    "aware_t _;"
    "sz_t rows;"
    "sz_t cols;"
    "sz_t stride;"
    "f3_t [] arr;"

    "const tp_t type_vector_assoc = bmath_vf3_s;"

    "func bmath_fp: is_equ;"
    "func bmath_fp: is_zro;"
    "func bmath_fp: is_one;"
    "func bmath_fp: add;"
    "func bmath_fp: zro;"
    "func bmath_fp: neg;"
    "func bmath_fp: sub;"
    "func bmath_fp: cpy;"
    "func bmath_fp: mul;"
    "func bmath_fp: one;"
    "func bmath_fp: inv;"
    "func bmath_fp: div;"
    "func bmath_fp_matrix: mul_vec;"
    "func bmath_fp_matrix: mul_scl;"

"}";

/**********************************************************************************************************************/

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_size( bmath_mf3_s* o, sz_t rows, sz_t cols )
{
    bcore_matrix_a_set_size( ( bcore_matrix* )o, rows, cols );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_fill_random( bmath_mf3_s* o, f3_t min, f3_t max, u2_t* rval )
{
    f3_t range = max - min;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        f3_t* v = o->data + i * o->stride;
        for( sz_t j = 0; j < o->cols; j++ )
        {
            v[ j ] = ( range * f3_xsg1_pos( rval ) ) + min;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

bmath_mf3_s* bmath_mf3_s_create_set_size( sz_t rows, sz_t cols )
{
    bmath_mf3_s* o = bmath_mf3_s_create();
    bmath_mf3_s_set_size( o, rows, cols );
    return o;
}

//---------------------------------------------------------------------------------------------------------------------

bmath_mf3_s* bmath_mf3_s_create_fill_random( sz_t rows, sz_t cols, f3_t min, f3_t max, u2_t* rval )
{
    bmath_mf3_s* o = bmath_mf3_s_create_set_size( rows, cols );
    bmath_mf3_s_fill_random( o, min, max, rval );
    return o;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_equ( const bmath_mf3_s* o, const bmath_mf3_s* op, f3_t max_dev )
{
    if( o->rows != op->rows ) return false;
    if( o->cols != op->cols ) return false;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        const f3_t* v2 = op->data + i * op->stride;
        for( sz_t j = 0; j < o->cols; j++ ) if( f3_abs( v1[ j ] - v2[ j ] ) > max_dev ) return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_zro( const bmath_mf3_s* o, f3_t max_dev )
{
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        for( sz_t j = 0; j < o->cols; j++ ) if( f3_abs( v1[ j ] ) > max_dev ) return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_one( const bmath_mf3_s* o, f3_t max_dev )
{
    if( o->rows != o->cols ) return false;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        for( sz_t j = 0; j < o->cols; j++ )
        {
            if( f3_abs( v1[ j ] - ( ( j == i ) ? 1.0 : 0.0 ) ) > max_dev ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_dag( const bmath_mf3_s* o, f3_t max_dev )
{
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        for( sz_t j = 0; j < o->cols; j++ )
        {
            if( ( i != j ) && ( f3_abs( v1[ j ] ) > max_dev ) ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_trd( const bmath_mf3_s* o, f3_t max_dev )
{
    if( o->rows != o->cols ) return false;
    sz_t n = o->rows;
    for( sz_t i = 0; i < n; i++ )
    {
        const f3_t* vi = o ->data + i * o ->stride;
        for( sz_t j = i + 2; j < n; j++ )
        {
            const f3_t* vj = o ->data + j * o ->stride;
            if( f3_abs( vi[ j ] ) > max_dev ) return false;
            if( f3_abs( vj[ i ] ) > max_dev ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_ubd( const bmath_mf3_s* o, f3_t max_dev )
{
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vi = o->data + i * o->stride;
        for( sz_t j = 0; j < o->cols; j++ )
        {
            if( ( j < i || j > i + 1 ) && ( f3_abs( vi[ j ] ) > max_dev ) ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_lbd( const bmath_mf3_s* o, f3_t max_dev )
{
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vi = o->data + i * o->stride;
        for( sz_t j = 0; j < o->cols; j++ )
        {
            if( ( j + 1 < i || j > i ) && ( f3_abs( vi[ j ] ) > max_dev ) ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_iso( const bmath_mf3_s* o, f3_t max_dev )
{
    if( o->rows != o->cols ) return false;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        for( sz_t j = i; j < o->rows; j++ )
        {
            const f3_t* v2 = o ->data + j * o ->stride;
            f3_t dot_prd = 0;
            for( sz_t k = 0; k < o->cols; k++ ) dot_prd += v1[ k ] * v2[ k ];
            if( f3_abs( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) ) > max_dev ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_utr( const bmath_mf3_s* o, f3_t max_dev )
{
    if( o->rows != o->cols ) return false;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v = o ->data + i * o ->stride;
        for( sz_t j = 0; j < i; j++ )
        {
            if( f3_abs( v[ j ] ) > max_dev ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_ltr( const bmath_mf3_s* o, f3_t max_dev )
{
    if( o->rows != o->cols ) return false;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v = o ->data + i * o ->stride;
        for( sz_t j = i + 1; j < o->rows; j++ )
        {
            if( f3_abs( v[ j ] ) > max_dev ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_hsm( const bmath_mf3_s* o, f3_t max_dev )
{
    if( o->rows != o->cols ) return false;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vi = o ->data + i * o ->stride;
        for( sz_t j = 0; j < i; j++ )
        {
            const f3_t* vj = o ->data + j * o ->stride;
            if( f3_abs( vi[ j ] - vj[ i ] ) > max_dev ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_mf3_s_f3_trc( const bmath_mf3_s* o )
{
    ASSERT( o->cols == o->rows );
    f3_t sum = 0;
    for( sz_t i = 0; i < o->rows; i++ ) sum += o->data[ i * ( o->stride + 1 ) ];
    return sum;
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_mf3_s_f3_sub_sqr( const bmath_mf3_s* o, const bmath_mf3_s* op )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, op ) );
    f3_t sum = 0;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 =  o->data + i *  o->stride;
        const f3_t* v2 = op->data + i * op->stride;
        for( sz_t i = 0; i < o->cols; i++ ) sum += f3_sqr( v1[ i ] - v2[ i ] );
    }
    return sum;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_add( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, op ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data  + i * o ->stride;
        const f3_t* v2 = op->data  + i * op->stride;
              f3_t* vr = res->data + i * res->stride;
        for( sz_t j = 0; j < o->cols; j++ ) vr[ j ] = v1[ j ] + v2[ j ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_sub( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, op ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data  + i * o ->stride;
        const f3_t* v2 = op->data  + i * op->stride;
              f3_t* vr = res->data + i * res->stride;
        for( sz_t j = 0; j < o->cols; j++ ) vr[ j ] = v1[ j ] - v2[ j ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_zro( bmath_mf3_s* o )
{
    for( sz_t i = 0; i < o->rows; i++ )
    {
        f3_t* v1 = o ->data  + i * o ->stride;
        for( sz_t j = 0; j < o->cols; j++ ) v1[ j ] = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_one( bmath_mf3_s* o )
{
    for( sz_t i = 0; i < o->rows; i++ )
    {
        f3_t* v1 = o ->data  + i * o ->stride;
        for( sz_t j = 0; j < o->cols; j++ ) v1[ j ] = ( j == i ) ? 1 : 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_neg( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data  + i * o ->stride;
              f3_t* vr = res->data + i * res->stride;
        for( sz_t j = 0; j < o->cols; j++ ) vr[ j ] = -v1[ j ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_cpy( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    if( res == o ) return;
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data  + i * o ->stride;
              f3_t* vr = res->data + i * res->stride;
        for( sz_t j = 0; j < o->cols; j++ ) vr[ j ] = v1[ j ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    if( res == o || res == op )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_mul( o, op, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT(  o->cols ==  op->rows );
    ASSERT(  o->rows == res->rows );
    ASSERT( op->cols == res->cols );

    // cache friendly inner loop
    bmath_mf3_s_zro( res );
    for( sz_t j = 0; j < o->cols; j++ )
    {
        const f3_t* bj = op->data + j * op->stride;
        for( sz_t i = 0; i < o->rows; i++ )
        {
            f3_t aij =   o->data[  i *   o->stride + j ];
            f3_t* ri = res->data + i * res->stride;
            for( sz_t k = 0; k < op->cols; k++ ) ri[ k ] += aij * bj[ k ];
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_dag( const bmath_mf3_s* o, const bmath_vf3_s* dag, bmath_mf3_s* res )
{
    ASSERT( o->cols == o->rows   );
    ASSERT( o->rows == dag->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );
    const f3_t* vd = dag->data;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vo =   o->data + i *   o->stride;
              f3_t* vr = res->data + i * res->stride;
        for( sz_t j = 0; j < o->cols; j++ ) vr[ j ] = vo[ j ] * vd[ j ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_dag_mul( const bmath_mf3_s* o, const bmath_vf3_s* dag, bmath_mf3_s* res )
{
    ASSERT( o->cols == o->rows   );
    ASSERT( o->rows == dag->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );
    const f3_t* vd = dag->data;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vo =   o->data + i *   o->stride;
              f3_t* vr = res->data + i * res->stride;
              f3_t d = vd[ i ];
        for( sz_t j = 0; j < o->cols; j++ ) vr[ j ] = vo[ j ] * d;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_add_opd( const bmath_mf3_s* o, const bmath_vf3_s* op1, const bmath_vf3_s* op2, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    ASSERT( o->rows == op1->size );
    ASSERT( o->cols == op2->size );

    const f3_t* v1 = op1->data;
    const f3_t* v2 = op2->data;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* oi = o->data   + o->stride * i;
              f3_t* ri = res->data + res->stride * i;
        for( sz_t j = 0; j < o->cols; j++ ) ri[ j ] = oi[ j ] + ( v1[ i ] * v2[ j ] );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    if( res == o || res == op )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_mul_htp( o, op, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->cols  == op->cols );
    ASSERT( o->rows  == res->rows );
    ASSERT( op->rows == res->cols );

    if( o == op ) // result is symmetric -> we can safe half of the work
    {
        for( sz_t i = 0; i < o->rows; i++ )
        {
            const f3_t* vi = o->data + i * o->stride;
            for( sz_t j = 0; j <= i ; j++ )
            {
                const f3_t* vj = o->data + j * o->stride;
                f3_t sum = 0;
                for( sz_t k = 0; k < o->cols; k++ ) sum += vi[ k ] * vj[ k ];
                res->data[ i * res->stride + j ] = sum;
                res->data[ j * res->stride + i ] = sum;
            }
        }
        return;
    }

    bmath_mf3_s_zro( res );
    for( sz_t i = 0; i < o->rows; i++ )
    {
              f3_t* vri = res->data + i * res->stride;
        const f3_t* voi =   o->data + i *   o->stride;
        for( sz_t j = 0; j < op->rows; j++ )
        {
            const f3_t* vpj = op->data + j * op->stride;
            for( sz_t k = 0; k < o->cols; k++ ) vri[ j ] += voi[ k ] * vpj[ k ];
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_inv_htp_colesky( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size_to( res, buf );
        bmath_mf3_s_inv_htp_colesky( o, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );

    // Inversion via cholesky decomposition:
    bmath_mf3_s* buf = bmath_mf3_s_create();
    bmath_mf3_s_set_size_to( o, buf );
    bmath_mf3_s_htp( o, buf );            // buf = oT
    bmath_mf3_s_mul_htp( buf, buf, res ); // res = S = oT * o
    bmath_mf3_s_decompose_cholesky( res, res );     // res = L
    bmath_mf3_s_ltr_inv_htp( res, res );  // res = LIT (upper triangular)
    bmath_mf3_s_utr_mul_htp( res, buf );  // res = SI = LIT * LI
    bmath_mf3_s_mul_htp( o, buf, res );   // buf = o * SIT = o * (oI*oIT) = oIT; (SI==SIT)
    bmath_mf3_s_discard( buf );

}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_inv_htp_luc( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    bmath_mf3_s* luc = bmath_mf3_s_create();
    bmath_mf3_s_set_size_to( o, luc );

    bmath_mf3_s_decompose_luc( o, luc );
    bmath_mf3_s_one( res );
    bmath_mf3_s_luc_solve_htp_htp( luc, res, res );
    bmath_mf3_s_discard( luc );
}

void bmath_mf3_s_inv_htp( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    bmath_mf3_s_inv_htp_luc( o, res );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_inv( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    bmath_mf3_s_inv_htp( o, res );
    bmath_mf3_s_htp( res, res );
}

//---------------------------------------------------------------------------------------------------------------------

// pseudo inversion
void bmath_mf3_s_piv( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res )
{
    ASSERT( o->rows == res->cols );
    ASSERT( o->cols == res->rows );

    sz_t n = sz_min( o->rows, o->cols );
    if( n == 0 ) return;

    bmath_mf3_s* a = bmath_mf3_s_create();
    // we let 'a' use the space of res
    if( o != res )
    {
        a->data   = res->data;
        a->rows   = o->rows;
        a->cols   = o->cols;
        a->stride = a->cols;
        a->size   = a->rows * a->cols;
        a->space  = 0; // a does not own its space
        bmath_mf3_s_cpy( o, a );
    }
    else
    {
        *a = *res;
        a->space = 0; // a does not own its space
    }

    bmath_mf3_s* u = bmath_mf3_s_create();
    bmath_mf3_s* v = bmath_mf3_s_create();
    bmath_vf3_s* d = bmath_vf3_s_create();

    bmath_mf3_s_set_size( a, o->rows, o->cols );
    bmath_mf3_s_set_size( u, o->rows, o->rows );
    bmath_mf3_s_set_size( v, o->cols, o->cols );
    bmath_vf3_s_set_size( d, n );
    bmath_mf3_s_one( u );
    bmath_mf3_s_one( v );

    // o = uT * a * v; o^-1 = vT * (a^-1)T * u
    bmath_mf3_s_svd( u, a, v );
    bmath_mf3_s_get_dag_vec( a, d );

    // diagonal elements are sorted in descending manner
    f3_t s_max = f3_max( f3_abs( d->data[ 0 ] ), f3_abs( d->data[ n - 1 ] ) );
    f3_t thr = s_max * eps;
    if( thr == 0 ) thr = 1.0;

    // pseudo invert diagonal
    for( sz_t i = 0; i < n; i++ ) d->data[ i ] = ( fabs( d->data[ i ] ) < thr ) ? 0 : 1.0 / d->data[ i ];

    // transpose u, v
    bmath_mf3_s_htp( u, u );
    bmath_mf3_s_htp( v, v );

    for( sz_t i = 0; i < res->rows; i++ )
    {
        f3_t* ri = res->data + res->stride * i;
        f3_t* vi =   v->data +   v->stride * i;
        for( sz_t j = 0; j < res->cols; j++ )
        {
            f3_t* uj = u->data + u->stride * j;
            f3_t  r = 0;
            for( sz_t k = 0; k < n; k++ ) r += vi[ k ] * uj[ k ] * d->data[ k ];
            ri[ j ] = r;
        }
    }

    bmath_vf3_s_discard( d );
    bmath_mf3_s_discard( v );
    bmath_mf3_s_discard( u );
    bmath_mf3_s_discard( a );
}

//---------------------------------------------------------------------------------------------------------------------

// pseudo inversion of a symmetric matrix
void bmath_mf3_s_hsm_piv( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res )
{
    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size_to( res, buf );
        bmath_mf3_s_hsm_piv( o, eps, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    ASSERT( o->rows == o->cols );

    sz_t n = o->rows;
    if( n == 0 ) return;

    bmath_mf3_s* a = res; // a occupies space of res
    bmath_mf3_s* q = bmath_mf3_s_create();
    bmath_mf3_s_set_size( q, n, n );
    bmath_mf3_s_one( q );

    bmath_mf3_s_cpy( o, a );
    bmath_mf3_s_evd( a, q );

    bmath_vf3_s* dag = bmath_vf3_s_create();
    bmath_vf3_s_set_size( dag, n );
    bmath_mf3_s_get_dag_vec( a, dag );

    // dag is sorted in descending manner
    f3_t s_max = f3_max( f3_abs( dag->data[ 0 ] ), f3_abs( dag->data[ n - 1 ] ) );
    f3_t thr = s_max * eps;
    if( thr == 0 ) thr = 1.0;

    for( sz_t i = 0; i < n; i++ ) dag->data[ i ] = ( dag->data[ i ] < thr ) ? 0 : ( 1.0 / dag->data[ i ] );

    bmath_mf3_s_htp( q, q );
    bmath_mf3_s_udu_htp( q, dag, res );

    bmath_vf3_s_discard( dag );
    bmath_mf3_s_discard( q );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_inv_av1( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size_to( res, buf );
        bmath_mf3_s_inv_av1( o, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );

    bmath_mf3_s o_sub = bmath_mf3_s_get_weak_sub_mat(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mf3_s r_sub = bmath_mf3_s_get_weak_sub_mat( res, 0, 0, res->rows, res->cols - 1 );

    bmath_mf3_s_inv( &o_sub, &r_sub );

    sz_t n = o_sub.rows;

    for( sz_t i = 0; i < n; i++ )
    {
        f3_t sum = 0;
        f3_t* row = r_sub.data + i * r_sub.stride;
        for( sz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
        res->data[ i * res->stride + n ] = -sum;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_hsm_inv_av1( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size_to( res, buf );
        bmath_mf3_s_hsm_inv_av1( o, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );

    bmath_mf3_s o_sub = bmath_mf3_s_get_weak_sub_mat(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mf3_s r_sub = bmath_mf3_s_get_weak_sub_mat( res, 0, 0, res->rows, res->cols - 1 );

    bmath_mf3_s_hsm_inv( &o_sub, &r_sub );

    sz_t n = o_sub.rows;

    for( sz_t i = 0; i < n; i++ )
    {
        f3_t sum = 0;
        f3_t* row = r_sub.data + i * r_sub.stride;
        for( sz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
        res->data[ i * res->stride + n ] = -sum;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_hsm_piv_av1( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res )
{
    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size_to( res, buf );
        bmath_mf3_s_hsm_piv_av1( o, eps, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );

    bmath_mf3_s o_sub = bmath_mf3_s_get_weak_sub_mat(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mf3_s r_sub = bmath_mf3_s_get_weak_sub_mat( res, 0, 0, res->rows, res->cols - 1 );

    bmath_mf3_s_hsm_piv( &o_sub, eps, &r_sub );

    sz_t n = o_sub.rows;

    for( sz_t i = 0; i < n; i++ )
    {
        f3_t sum = 0;
        f3_t* row = r_sub.data + i * r_sub.stride;
        for( sz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
        res->data[ i * res->stride + n ] = -sum;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_div( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    bmath_mf3_s_inv( op, res );
    bmath_mf3_s_mul( o, res, res );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* vec, bmath_vf3_s* res )
{
    if( res == vec )
    {
        bmath_vf3_s* buf = bmath_vf3_s_create_set_size( res->size );
        bmath_mf3_s_mul_vec( o, vec, buf );
        bmath_vf3_s_cpy( buf, res );
        bmath_vf3_s_discard( buf );
        return;
    }

    ASSERT( o->cols == vec->size );
    ASSERT( o->rows == res->size );
    f3_t* vr = res->data;
    f3_t* v2 = vec->data;
    for( sz_t i = 0; i < res->size; i++ )
    {
        f3_t* v1 = o->data + i * o->stride;
        f3_t sum = 0;
        for( sz_t j = 0; j < o->cols; j++ ) sum += v1[ j ] * v2[ j ];
        vr[ i ] = sum;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_av1( const bmath_mf3_s* o, const bmath_vf3_s* av1, bmath_vf3_s* res )
{
    if( res == av1 )
    {
        bmath_vf3_s* buf = bmath_vf3_s_create_set_size( res->size );
        bmath_mf3_s_mul_av1( o, av1, buf );
        bmath_vf3_s_cpy( buf, res );
        bmath_vf3_s_discard( buf );
        return;
    }

    sz_t n = av1->size;
    ASSERT( o->cols == n + 1 );
    ASSERT( o->rows == res->size );
    f3_t* vr = res->data;
    f3_t* v2 = av1->data;
    for( sz_t i = 0; i < res->size; i++ )
    {
        f3_t* v1 = o->data + i * o->stride;
        f3_t sum = 0;
        for( sz_t j = 0; j < n; j++ ) sum += v1[ j ] * v2[ j ];
        vr[ i ] = sum + v1[ n ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_udu_htp( const bmath_mf3_s* o, const bmath_vf3_s* dag, bmath_mf3_s* res )
{
    ASSERT( res != o );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    ASSERT( o->rows == dag->size );

    const f3_t* vd = dag->data;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vi =   o->data + i *   o->stride;
              f3_t* vr = res->data + i * res->stride;
        for( sz_t j = 0; j < o->rows; j++ )
        {
            const f3_t* vj = o->data + j * o->stride;
            f3_t sum = 0;
            for( sz_t k = 0; k < o->rows; k++ ) sum += vi[ k ] * vj[ k ] * vd[ k ];
            vr[ j ] = sum;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_htp( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    if( o->rows == o->cols )
    {
        // in-place algorithm
        ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
        sz_t n = o->rows;
        for( sz_t i = 0; i < n; i++ )
        {
            const f3_t* voi =   o->data + i *   o->stride;
                  f3_t* vri = res->data + i * res->stride;
            for( sz_t j = 0; j < i; j++ )
            {
                const f3_t* voj =   o->data + j *   o->stride;
                      f3_t* vrj = res->data + j * res->stride;

                f3_t tmp = voi[ j ];
                vri[ j ] = voj[ i ];
                vrj[ i ] = tmp;
            }
        }

        // copy diag
        if( o != res ) for( sz_t i = 0; i < n; i++ ) res->data[ i * res->stride + i ] = o->data[ i * o->stride + i ];
        return;
    }

    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->cols, res->rows );
        bmath_mf3_s_htp( o, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->rows == res->cols );
    ASSERT( o->cols == res->rows );

    for( sz_t i = 0; i < res->rows; i++ )
    {
              f3_t* vr = res->data + i * res->stride;
        const f3_t* v1 = o ->data  + i;
        for( sz_t j = 0; j < res->cols; j++ ) vr[ j ] = v1[ j * o->stride ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_scl( const bmath_mf3_s* o, const f3_t* op, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    f3_t f = *op;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data  + i * o ->stride;
              f3_t* vr = res->data + i * res->stride;
        for( sz_t j = 0; j < o->cols; j++ ) vr[ j ] = v1[ j ] * f;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_row_by_data( bmath_mf3_s* o, sz_t idx, const f3_t* data )
{
    ASSERT( idx < o->rows );
    bcore_u_memcpy( sizeof( f3_t ), o->data + idx * o->stride, data, o->cols );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_col_by_data( bmath_mf3_s* o, sz_t idx, const f3_t* data )
{
    ASSERT( idx < o->cols );
    f3_t* v = o ->data + idx;
    for( sz_t i = 0; i < o->rows; i++ ) v[ i * o ->stride ] = data[ i ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_dag_by_data( bmath_mf3_s* o, const f3_t* data )
{
    sz_t size = sz_min( o->rows, o->cols );
    for( sz_t i = 0; i < size; i++ ) o->data[ i * ( o ->stride + 1 ) ] = data[ i ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_row_by_vec( bmath_mf3_s* o, sz_t idx, const bmath_vf3_s* vec )
{
    ASSERT( o->cols <= vec->size );
    bmath_mf3_s_set_row_by_data( o, idx, vec->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_col_by_vec( bmath_mf3_s* o, sz_t idx, const bmath_vf3_s* vec )
{
    ASSERT( o->rows <= vec->size );
    bmath_mf3_s_set_col_by_data( o, idx, vec->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_dag_by_vec( bmath_mf3_s* o, const bmath_vf3_s* vec )
{
    ASSERT( sz_min( o->rows, o->cols ) <= vec->size );
    bmath_mf3_s_set_dag_by_data( o, vec->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_get_row_vec(  const bmath_mf3_s* o, sz_t idx, bmath_vf3_s* res )
{
    ASSERT( idx < o->rows );
    ASSERT( o->cols <= res->size );
    bcore_u_memcpy( sizeof( f3_t ), res->data, o->data + idx * o->stride, o->cols );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_get_col_vec(  const bmath_mf3_s* o, sz_t idx, bmath_vf3_s* res )
{
    ASSERT( idx < o->cols );
    ASSERT( o->cols <= res->size );
    for( sz_t i = 0; i < o->rows; i++ ) res->data[ i ] = o->data[ i * o ->stride ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_get_dag_vec( const bmath_mf3_s* o, bmath_vf3_s* res )
{
    sz_t size = sz_min( o->rows, o->cols );
    ASSERT( size <= res->size );
    for( sz_t i = 0; i < size; i++ ) res->data[ i ] = o->data[ i * ( o->stride + 1 ) ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_swap_row( bmath_mf3_s* o, sz_t i, sz_t j )
{
    if( i == j ) return;
    ASSERT( i < o->rows );
    ASSERT( j < o->rows );
    f3_t* vi = o->data + i * o->stride;
    f3_t* vj = o->data + j * o->stride;
    for( sz_t k = 0; k < o->cols; k++ ) f3_t_swap( vi + k, vj + k );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_swap_col( bmath_mf3_s* o, sz_t i, sz_t j )
{
    if( i == j ) return;
    ASSERT( i < o->cols );
    ASSERT( j < o->cols );
    f3_t* vi = o->data + i;
    f3_t* vj = o->data + j;
    for( sz_t k = 0; k < o->rows; k++ ) f3_t_swap( vi + k * o->stride, vj + k * o->stride );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_f3_to_row( bmath_mf3_s* o, f3_t v, sz_t i )
{
    ASSERT( i < o->rows );
    f3_t* vi = o->data + i * o->stride;
    for( sz_t k = 0; k < o->cols; k++ ) vi[ k ] *= v;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_f3_to_col( bmath_mf3_s* o, f3_t v, sz_t i )
{
    ASSERT( i < o->cols );
    f3_t* vi = o->data + i;
    for( sz_t k = 0; k < o->rows; k++ ) vi[ k * o->stride ] *= v;
}

//---------------------------------------------------------------------------------------------------------------------

bmath_mf3_s bmath_mf3_s_get_weak_sub_mat( const bmath_mf3_s* o, sz_t row, sz_t col, sz_t rows, sz_t cols )
{
    ASSERT( row + rows <= o->rows );
    ASSERT( col + cols <= o->cols );
    bmath_mf3_s ret;
    bmath_mf3_s_init( &ret );
    ret.rows = rows;
    ret.cols = cols;
    ret.stride = o->stride;
    ret.data = o->data + row * o->stride + col;
    ret.size = o->size;
    ret.space = 0; // indicates that ret is a weak object. (no shutdown needed)
    return ret;
}

//---------------------------------------------------------------------------------------------------------------------

bmath_vf3_s bmath_mf3_s_get_row_weak_vec( const bmath_mf3_s* o, sz_t idx )
{
    ASSERT( idx < o->rows );
    bmath_vf3_s vec;
    bmath_vf3_s_init( &vec );
    vec.data = o->data + idx * o->stride;
    vec.size = o->cols;
    vec.space = 0;
    return vec;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_decompose_cholesky( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );

    for( sz_t i = 0; i < o->rows; i++ )
    {
              f3_t* vli = res->data + i * res->stride;
        const f3_t* voi =   o->data + i * o->stride;
        for( sz_t j = 0; j <= i; j++ )
        {
            f3_t* vlj = res->data + j * res->stride;
            f3_t acc = voi[ j ];
            for( sz_t k = 0; k < j; k++ ) acc -= vli[ k ] * vlj[ k ];
            if( i == j )
            {
                vli[ j ] = ( acc > 0 ) ? sqrt( acc ) : 0;
            }
            else
            {
                vli[ j ] = ( vlj[ j ] != 0 ) ? acc / vlj[ j ] : 0;
                vlj[ i ] = 0; // upper triangle element
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_decompose_luc( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    sz_t n = o->cols;
    sz_t stride = res->stride;

    bmath_mf3_s_cpy( o, res );
    for( sz_t i = 0; i < n; i++ )
    {
        f3_t* vi = res->data + i * stride;
        for( sz_t j = 0; j < i; j++ )
        {
            f3_t* vj = res->data + j;
            f3_t sum = 0;
            for( sz_t k = 0; k < j; k++ ) sum += vi[ k ] * vj[ k * stride ];
            vi[ j ] = ( vj[ j * stride ] != 0 ) ? ( ( vi[ j ] - sum ) / vj[ j * stride ] ) : 0;
        }

        for( sz_t j = i; j < n; j++ )
        {
            f3_t* vj = res->data + j;
            f3_t sum = 0;
            for( sz_t k = 0; k < i; k++ ) sum += vi[ k ] * vj[ k * stride ];
            vi[ j ] -= sum;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ltr_inv_htp( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );

    sz_t n = o->rows;

    // diagonal elements
    for( sz_t i = 0; i < n; i++ )
    {
        f3_t v = o->data[ i * o->stride + i ];
        res->data[ i * res->stride + i ] = ( v != 0 ) ? 1.0 / v : 0;
    }

    // upper off-diagonal elements
    for( sz_t i = 0; i < n; i++ )
    {
        f3_t* vri = res->data + i * res->stride;
        for( sz_t j = i + 1; j < n; j++ )
        {
            const f3_t* voj =   o->data + j *   o->stride;
                  f3_t* vrj = res->data + j * res->stride;
            f3_t sum = 0;
            for( sz_t k = i; k < j; k++ ) sum += voj[ k ] * vri[ k ];
            vri[ j ] = -vrj[ j ] * sum;
            vrj[ i ] = 0;
        }
    }

    // zero lower off-diagonal elements; (this part must be kept to last for the in-place algorithm)
    for( sz_t i = 0; i < n; i++ )
    {
        f3_t* vri = res->data + i * res->stride;
        for( sz_t j = 0; j < i; j++ ) vri[ j ] = 0;
    }
}

void bmath_mf3_s_hsm_inv( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_hsm( o ) );
    bmath_mf3_s_decompose_cholesky( o, res ); // res = ltr
    bmath_mf3_s_ltr_inv_htp( res, res );      // res = utr
    bmath_mf3_s_utr_mul_htp( res, res );      // res = oI
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ltr_mul_htp( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    sz_t n = o->rows;

    // compute off diagonal elements; store in upper triangle of res
    for( sz_t i = 0; i < n; i++ )
    {
        const f3_t* voi = o->data + i * o->stride;
        for( sz_t j = 0; j < i; j++ )
        {
            const f3_t* voj =   o->data + j *   o->stride;
                  f3_t* vrj = res->data + j * res->stride;
            f3_t sum = 0;
            for( sz_t k = 0; k <= j; k++ ) sum += voi[ k ] * voj[ k ];
            vrj[ i ] = sum;
        }
    }

    // diagonal elements in reverse order
    for( sz_t i = n - 1; i < n; i-- )
    {
        const f3_t* voi =   o->data + i *   o->stride;
              f3_t* vri = res->data + i * res->stride;

        f3_t sum = 0;
        for( sz_t k = 0; k <= i; k++ ) sum += voi[ k ] * voi[ k ];
        vri[ i ] = sum;
    }

    // copy lower off-diagonal from upper off-diagonal
    for( sz_t i = 0; i < n; i++ )
    {
        f3_t* vri = res->data + i * res->stride;
        for( sz_t j = 0; j < i; j++ ) vri[ j ] = res->data[ j * res->stride + i ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_utr_mul_htp( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    sz_t n = o->rows;

    // compute off diagonal elements; store in lower triangle of res
    for( sz_t i = 0; i < n; i++ )
    {
        const f3_t* voi = o->data + i * o->stride;
        for( sz_t j = i + 1; j < n; j++ )
        {
            const f3_t* voj =   o->data + j *   o->stride;
                  f3_t* vrj = res->data + j * res->stride;
            f3_t sum = 0;
            for( sz_t k = j; k < n; k++ ) sum += voi[ k ] * voj[ k ];
            vrj[ i ] = sum;
        }
    }

    // diagonal elements in ascending order
    for( sz_t i = 0; i < n; i++ )
    {
        const f3_t* voi =   o->data + i *   o->stride;
              f3_t* vri = res->data + i * res->stride;

        f3_t sum = 0;
        for( sz_t k = i; k < n; k++ ) sum += f3_sqr( voi[ k ] );
        vri[ i ] = sum;
    }

    // copy lower off-diagonal to upper off-diagonal
    for( sz_t i = 0; i < n; i++ )
    {
        f3_t* vri = res->data + i * res->stride;
        for( sz_t j = 0; j < i; j++ ) res->data[ j * res->stride + i ] = vri[ j ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ltr_mul_vec_( const bmath_mf3_s* o, const f3_t* op, f3_t* res )
{
    // Algorithm works in-place for op == res;
    for( sz_t i = o->rows - 1; i < o->rows; i-- )
    {
        f3_t* v1 = o->data + i * o->stride;
        f3_t sum = 0;
        for( sz_t j = 0; j <= i; j++ ) sum += v1[ j ] * op[ j ];
        res[ i ] = sum;
    }
}

void bmath_mf3_s_ltr_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    bmath_mf3_s_ltr_mul_vec_( o, op->data, res->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_lt1_mul_vec_( const bmath_mf3_s* o, const f3_t* op, f3_t* res )
{
    // Algorithm works in-place for op == res;
    for( sz_t i = o->rows - 1; i < o->rows; i-- )
    {
        f3_t* v1 = o->data + i * o->stride;
        f3_t sum = 0;
        for( sz_t j = 0; j < i; j++ ) sum += v1[ j ] * op[ j ];
        res[ i ] = sum + op[ i ];
    }
}

void bmath_mf3_s_lt1_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    bmath_mf3_s_lt1_mul_vec_( o, op->data, res->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_utr_mul_vec_( const bmath_mf3_s* o, const f3_t* op, f3_t* res )
{
    // Algorithm works in-place for op == res;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        f3_t* v1 = o->data + i * o->stride;
        f3_t sum = 0;
        for( sz_t j = i; j < o->cols; j++ ) sum += v1[ j ] * op[ j ];
        res[ i ] = sum;
    }
}

void bmath_mf3_s_utr_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    bmath_mf3_s_utr_mul_vec_( o, op->data, res->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_luc_mul_vec_( const bmath_mf3_s* o, const f3_t* op, f3_t* res )
{
    // Algorithm works in-place for op == res;
    bmath_mf3_s_utr_mul_vec_( o,  op, res );
    bmath_mf3_s_lt1_mul_vec_( o, res, res );
}

void bmath_mf3_s_luc_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    bmath_mf3_s_luc_mul_vec_( o,  op->data, res->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ltr_mul_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        bmath_mf3_s_ltr_mul_vec_( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_lt1_mul_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        bmath_mf3_s_lt1_mul_vec_( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_utr_mul_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        bmath_mf3_s_utr_mul_vec_( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_luc_mul_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        bmath_mf3_s_luc_mul_vec_( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ltr_solve_vec_( const bmath_mf3_s* o, const f3_t* op, f3_t* res )
{
    // Algorithm works in-place for op == res;
    if( op != res ) bcore_u_memcpy( sizeof( f3_t ), res, op, o->rows );
    sz_t n = o->rows;
    for( sz_t i = 0; i < n; i++ )
    {
        const f3_t* voi = o->data + i * o->stride;
        f3_t sum = 0;
        for( sz_t j = 0; j < i; j++ ) sum += voi[ j ] * res[ j ];
        res[ i ] = ( voi[ i ] != 0 ) ? ( res[ i ] - sum ) / voi[ i ] : 0;
    }
}

void bmath_mf3_s_ltr_solve_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    bmath_mf3_s_ltr_solve_vec_( o, op->data, res->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_utr_solve_vec_( const bmath_mf3_s* o, const f3_t* op, f3_t* res )
{
    // Algorithm works in-place for op == res;
    if( op != res ) bcore_u_memcpy( sizeof( f3_t ), res, op, o->rows );
    sz_t n = o->rows;
    for( sz_t i = n - 1; i < n; i-- )
    {
        const f3_t* voi = o->data + i * o->stride;
        f3_t sum = 0;
        for( sz_t j = i + 1; j < n; j++ ) sum += voi[ j ] * res[ j ];
        res[ i ] = ( voi[ i ] != 0 ) ? ( res[ i ] - sum ) / voi[ i ] : 0;
    }
}

void bmath_mf3_s_utr_solve_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    bmath_mf3_s_utr_solve_vec_( o, op->data, res->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_lt1_solve_vec_( const bmath_mf3_s* o, const f3_t* op, f3_t* res )
{
    // Algorithm works in-place for op == res;
    if( op != res ) bcore_u_memcpy( sizeof( f3_t ), res, op, o->rows );
    sz_t n = o->rows;
    for( sz_t i = 0; i < n; i++ )
    {
        const f3_t* voi = o->data + i * o->stride;
        f3_t sum = 0;
        for( sz_t j = 0; j < i; j++ ) sum += voi[ j ] * res[ j ];
        res[ i ] -= sum;
    }
}

void bmath_mf3_s_lt1_solve_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    bmath_mf3_s_lt1_solve_vec_( o, op->data, res->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_luc_solve_vec_( const bmath_mf3_s* o, const f3_t* op, f3_t* res )
{
    // Algorithm works in-place for op == res;
    bmath_mf3_s_lt1_solve_vec_( o,  op, res );
    bmath_mf3_s_utr_solve_vec_( o, res, res );
}

void bmath_mf3_s_luc_solve_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    bmath_mf3_s_luc_solve_vec_( o, op->data, res->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ltr_solve_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        bmath_mf3_s_ltr_solve_vec_( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_lt1_solve_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        bmath_mf3_s_lt1_solve_vec_( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_utr_solve_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        bmath_mf3_s_utr_solve_vec_( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_luc_solve_htp_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        bmath_mf3_s_luc_solve_vec_( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_hsm_trd_htp( bmath_mf3_s* a, bmath_mf3_s* v )
{
    ASSERT( bmath_mf3_s_is_hsm( a ) );
    if( v )
    {
        ASSERT( v != a );
        ASSERT( v->cols == a->rows );
        ASSERT( v->rows == a->rows );
    }

    grt_s gr;

    for( sz_t j = 0; j < a->rows; j++ )
    {
        for( sz_t k = a->rows - 2; k > j; k-- )
        {
            f3_t* ak;

            // zero upper row
            ak = a->data + k;
            sz_t jstride = j * a->stride;
            grt_s_init_and_annihilate_b( &gr, ak + jstride, ak + jstride + 1 );
            grt_s_rotate_col( &gr, ak, ak + 1, a->stride, j + 1, a->rows );

            // transposed operation
            ak = a->data + k * a->stride;
            grt_s_annihilate_b( &gr, ak + j, ak + a->stride + j );
            grt_s_rotate_row( &gr, ak, ak + a->stride, j + 1, a->cols );

            // rotation matrix
            if( v ) grt_s_rotate_row( &gr, v->data + k * v->stride, v->data + ( k + 1 ) * v->stride, 0, v->cols );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ubd_htp( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v )
{
    if( u )
    {
        ASSERT( u->cols == a->rows );
        ASSERT( u->rows == a->rows );
        ASSERT( u != a );
        ASSERT( u != v );
    }

    if( v )
    {
        ASSERT( v->cols == a->cols );
        ASSERT( v->rows == a->cols );
        ASSERT( v != a );
    }

    grt_s gr;
    sz_t min_cols_rows = sz_min( a->cols, a->rows );

    for( sz_t j = 0; j < min_cols_rows; j++ )
    {
        // zero lower column
        for( sz_t l = a->rows - 1; l > j; l-- )
        {
            f3_t* al = a->data + l * a->stride;
            grt_s_init_and_annihilate_b( &gr, al - a->stride + j, al + j );
            grt_s_rotate_row( &gr, al - a->stride, al, j + 1, a->cols );
            if( u ) grt_s_rotate_row( &gr, u->data + ( l - 1 ) * u->stride, u->data + l * u->stride, 0, u->cols );
        }

        // zero upper row
        for( sz_t l = a->cols - 1; l > j + 1; l-- )
        {
            f3_t* al = a->data + l;
            grt_s_init_and_annihilate_b( &gr, al - 1 + j * a->stride, al + j * a->stride );
            grt_s_rotate_col( &gr, al - 1, al, a->stride, j + 1, a->rows );
            if( v ) grt_s_rotate_row( &gr, v->data + ( l - 1 ) * v->stride, v->data + l * v->stride, 0, v->cols );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_lbd_htp( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v )
{
    if( u )
    {
        ASSERT( u->cols == a->rows );
        ASSERT( u->rows == a->rows );
        ASSERT( u != a );
        ASSERT( u != v );
    }

    if( v )
    {
        ASSERT( v->cols == a->cols );
        ASSERT( v->rows == a->cols );
        ASSERT( v != a );
    }

    grt_s gr;
    sz_t min_cols_rows = sz_min( a->cols, a->rows );

    for( sz_t j = 0; j < min_cols_rows; j++ )
    {
        // zero upper row
        for( sz_t l = a->cols - 1; l > j; l-- )
        {
            f3_t* al = a->data + l;
            grt_s_init_and_annihilate_b( &gr, al - 1 + j * a->stride, al + j * a->stride );
            grt_s_rotate_col( &gr, al - 1, al, a->stride, j + 1, a->rows );
            if( v ) grt_s_rotate_row( &gr, v->data + ( l - 1 ) * v->stride, v->data + l * v->stride, 0, v->cols );
        }

        // zero lower column
        for( sz_t l = a->rows - 1; l > j + 1; l-- )
        {
            f3_t* al = a->data + l * a->stride;
            grt_s_init_and_annihilate_b( &gr, al - a->stride + j, al + j );
            grt_s_rotate_row( &gr, al - a->stride, al, j + 1, a->cols );
            if( u ) grt_s_rotate_row( &gr, u->data + ( l - 1 ) * u->stride, u->data + l * u->stride, 0, u->cols );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_decompose_qr( bmath_mf3_s* q, bmath_mf3_s* r )
{
    ASSERT( bmath_mf3_s_is_square( r ) );

    if( q )
    {
        ASSERT( r != q );
        ASSERT( bmath_mf3_s_is_equ_size( r, q ) );
    }

    sz_t n = r->rows;
    for( sz_t l = 0; l < n; l++ )
    {
        f3_t* rl = r->data + l * r->stride;
        for( sz_t k = l + 1; k < n; k++ )
        {
            f3_t* rk = r->data + k * r->stride;
            grt_s gr;
            grt_s_init_and_annihilate_b( &gr, rl + l, rk + l );
            grt_s_rotate_row( &gr, rl, rk, l + 1, n );
            if( q ) grt_s_rotate_row( &gr, q->data + l * q->stride, q->data + k * q->stride, 0, n );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_evd_jacobi( bmath_mf3_s* a, bmath_mf3_s* v )
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
bl_t bmath_mf3_s_evd_qr_xshift( bmath_mf3_s* a, bmath_mf3_s* v )
{
    sz_t n = a->rows;
    if( n <= 1 ) return true; // nothing to do

    // tridiagonalization
    bmath_mf3_s_hsm_trd_htp( a, v );

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

            grt_s gr0;
            grt_s_init_to_annihilate_b( &gr0, r00, a0[ a->stride ] );

            for( sz_t i = 0; i < block_n - 1; i++ )
            {
                if( v ) grt_s_rotate_row( &gr0, v->data + i * v->stride, v->data + ( i + 1 ) * v->stride, 0, n );

                f3_t* a1 = a0 + a->stride;

                f3_t r11 = a1[ 1 ] * gr0.c - r01 * gr0.s;
                f3_t r12 = a1[ 2 ] * gr0.c;

                     r00 = r00 * gr0.c + a1[ 0 ] * gr0.s;
                     r01 = r01 * gr0.c + a1[ 1 ] * gr0.s;
                f3_t r02 =               a1[ 2 ] * gr0.s;

                a0[ 0 ] = r00 * cp * gr0.c + r01 * gr0.s;

                grt_s gr1;
                grt_s_init_to_annihilate_b( &gr1, r11, ( i == block_n - 2 ) ? 0 : a1[ a->stride + 1 ] );

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
 *  bmath_mf3_s_evd_htp for more details.
 */
bl_t bmath_mf3_s_evd_qr_ishift( bmath_mf3_s* a, bmath_mf3_s* v )
{
    sz_t n = a->rows;
    if( n <= 1 ) return true; // nothing to do

    /// tridiagonalization
    bmath_mf3_s_hsm_trd_htp( a, v );

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

            grt_s gr_i = { .c = 1, .s = 0 };
            grt_s gr_j;
            f3_t b = gr_i.c * ajj - gr_i.s * ch * aij;
            grt_s_init_to_annihilate_b( &gr_j, b - gr_i.c * lambda, ajk );

            for( sz_t j = 0; j < block_n - 1; j++ )
            {
                if( v ) grt_s_rotate_row( &gr_j, v->data + j * v->stride, v->data + ( j + 1 ) * v->stride, 0, n );

                f3_t akk = pajj[ a->stride + 1 ];
                f3_t akl = ( j == block_n - 2 ) ? 0 : pajj[ a->stride + 2 ];

                f3_t rjj = gr_j.c * b + gr_j.s * ajk;
                f3_t rjk = gr_j.c * gr_i.c * ajk + gr_j.s * akk;
                f3_t rjl = gr_j.s * akl;

                f3_t f3_sqr_si_cj_lambda = f3_sqr( gr_i.s ) * gr_j.c * lambda;

                pajj[ 0 ] =  rjj * gr_j.c * gr_i.c + rjk * gr_j.s + gr_j.c * f3_sqr_si_cj_lambda;

                b = gr_j.c * akk - gr_j.s * gr_i.c * ajk;

                grt_s gr_k;
                grt_s_init_to_annihilate_b( &gr_k, b - gr_j.c * lambda, akl );

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

bl_t bmath_mf3_s_evd( bmath_mf3_s* a, bmath_mf3_s* v )
{
    return bmath_mf3_s_evd_qr_ishift( a, v );
}

//---------------------------------------------------------------------------------------------------------------------

/** SVD for a->rows >= a->cols
 *  Method: upper bidiagonalization + chase algorithm with implicit shift
 */
bl_t bmath_mf3_s_svd_ubd( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v )
{
    ASSERT( a->rows >= a->cols );

    // creating upper-bidiagonal
    bmath_mf3_s_ubd_htp( u, a, v );

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

            grt_s gr_l; // left rotation
            grt_s gr_r; // right rotation

            // left rotation strategically diagonalizes a * aT but creates a1[ 0 ]
            grt_s_init_to_annihilate_b( &gr_l, f3_sqr( a0[ 0 ] ) + f3_sqr( a0[ 1 ] ) - lambda, a0[ 1 ] * a1[ 1 ] );

            // right rotation annihilates a1[0]
            grt_s_init_to_annihilate_b( &gr_r, gr_l.c * a1[ 1 ] - gr_l.s * a0[ 1 ], gr_l.s * a0[ 0 ] );

            f3_t a00 = a0[ 0 ];
            f3_t a01 = a0[ 1 ];
            f3_t a11 = a1[ 1 ];

            a0[ 0 ] = gr_r.c * a00 + gr_r.s * a01; a0[ 1 ] = gr_r.c * a01 - gr_r.s * a00;
            a1[ 0 ] =                gr_r.s * a11; a1[ 1 ] = gr_r.c * a11;
            grt_s_rotate_row( &gr_l, a0, a1, 0, 3 );
            a1[ 0 ] = 0;

            if( u ) grt_s_rotate_row( &gr_l, u->data, u->data + u->stride, 0, u->cols );
            if( v ) grt_s_rotate_row( &gr_r, v->data, v->data + v->stride, 0, v->cols );

            // chasing, annihilating off-bidiagonals
            for( sz_t k = 0; k < block_n - 2; k++ )
            {
                f3_t* ak = a->data + ( a->stride + 1 ) * k + 1;
                f3_t* al = ak + a->stride;
                f3_t* am = al + a->stride;
                grt_s_init_and_annihilate_b( &gr_r, ak, ak + 1 );
                grt_s_rotate( &gr_r, al, al + 1 );
                grt_s_rotate( &gr_r, am, am + 1 );

                grt_s_init_and_annihilate_b( &gr_l, al, am );
                grt_s_rotate( &gr_l, al + 1, am + 1 );
                if( k < block_n - 3 ) grt_s_rotate( &gr_l, al + 2, am + 2 );

                if( u ) grt_s_rotate_row( &gr_l, u->data + u->stride * ( k + 1 ), u->data + u->stride * ( k + 2 ), 0, u->cols );
                if( v ) grt_s_rotate_row( &gr_r, v->data + v->stride * ( k + 1 ), v->data + v->stride * ( k + 2 ), 0, v->cols );
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
bl_t bmath_mf3_s_svd_lbd( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v )
{
    ASSERT( a->cols >= a->rows );

    // creating lower-bidiagonal
    bmath_mf3_s_lbd_htp( u, a, v );

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

            grt_s gr_l; // left rotation
            grt_s gr_r; // right rotation

            // left rotation strategically diagonalizes aT * a but creates a0[ 1 ]
            grt_s_init_to_annihilate_b( &gr_r, f3_sqr( a0[ 0 ] ) + f3_sqr( a1[ 0 ] ) - lambda, a1[ 0 ] * a1[ 1 ] );

            // right rotation annihilates a0[ 1 ]
            grt_s_init_to_annihilate_b( &gr_l, gr_r.c * a1[ 1 ] - gr_r.s * a1[ 0 ], gr_r.s * a0[ 0 ] );

            f3_t a00 = a0[ 0 ];
            f3_t a10 = a1[ 0 ];
            f3_t a11 = a1[ 1 ];

            a0[ 0 ] = gr_l.c * a00 + gr_l.s * a10; a1[ 0 ] = gr_l.c * a10 - gr_l.s * a00;
            a0[ 1 ] =                gr_l.s * a11; a1[ 1 ] = gr_l.c * a11;
            grt_s_rotate_col( &gr_r, a0, a0 + 1, a->stride, 0, 3 );
            a0[ 1 ] = 0;

            if( u ) grt_s_rotate_row( &gr_l, u->data, u->data + u->stride, 0, u->cols );
            if( v ) grt_s_rotate_row( &gr_r, v->data, v->data + v->stride, 0, v->cols );

            // chasing, annihilating off-bidiagonals
            for( sz_t k = 0; k < block_n - 2; k++ )
            {
                f3_t* ak = a->data + ( a->stride + 1 ) * ( k + 1 ) - 1;
                f3_t* al = ak + a->stride;
                f3_t* am = al + a->stride;

                grt_s_init_and_annihilate_b( &gr_l, ak, al );
                grt_s_rotate( &gr_l, ak + 1, al + 1 );
                grt_s_rotate( &gr_l, ak + 2, al + 2 );

                grt_s_init_and_annihilate_b( &gr_r, ak + 1, ak + 2 );
                grt_s_rotate( &gr_r, al + 1, al + 2 );
                if( k < block_n - 3 ) grt_s_rotate( &gr_r, am + 1, am + 2 );

                if( u ) grt_s_rotate_row( &gr_l, u->data + u->stride * ( k + 1 ), u->data + u->stride * ( k + 2 ), 0, u->cols );
                if( v ) grt_s_rotate_row( &gr_r, v->data + v->stride * ( k + 1 ), v->data + v->stride * ( k + 2 ), 0, v->cols );

            }
        }
        if( cycle == max_cyles ) success = false;
    }

    return success;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_svd(  bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v )
{
    bl_t success = false;

    if( a->rows >= a->cols )
    {
        success = bmath_mf3_s_svd_ubd( u, a, v );
    }
    else
    {
        success = bmath_mf3_s_svd_lbd( u, a, v );
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

void bmath_mf3_s_set_covariance_on_section_fast( bmath_mf3_s* o, bmath_arr_vf3_s* arr_vec, sz_t start, sz_t end )
{
    end = end < arr_vec->size ? end : arr_vec->size;
    start = start > end ? end : start;

    bmath_mf3_s_zro( o );
    if( start == end ) return;
    sz_t n = arr_vec->data[ start ].size;
    ASSERT( o->rows == n );
    ASSERT( o->cols == n );

    bmath_vf3_s* avg = bmath_vf3_s_create_set_size( n );
    bmath_vf3_s* vec = bmath_vf3_s_create_set_size( n );

    bmath_arr_vf3_s_on_section_get_avg( arr_vec, start, end, avg );

    for( sz_t i = start; i < end; i++ )
    {
        bmath_vf3_s_sub( &arr_vec->data[ i ], avg, vec );
        bmath_mf3_s_add_opd( o, vec, vec, o );
    }

    f3_t f = 1.0 / ( end - start );
    bmath_mf3_s_mul_scl_f3( o, f, o );

    bmath_vf3_s_discard( avg );
    bmath_vf3_s_discard( vec );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_covariance_on_section_sprc( bmath_mf3_s* o, bmath_arr_vf3_s* arr_vec, sz_t start, sz_t end )
{
    end = end < arr_vec->size ? end : arr_vec->size;
    start = start > end ? end : start;

    bmath_mf3_s_zro( o );
    if( start == end ) return;
    sz_t n = arr_vec->data[ start ].size;
    ASSERT( o->rows == n );
    ASSERT( o->cols == n );

    bmath_vf3_s* avg = bmath_vf3_s_create_set_size( n );
    bmath_arr_vf3_s_on_section_get_avg_sprc( arr_vec, start, end, avg );

    for( sz_t i = 0; i < n; i++ )
    {
        for( sz_t j = 0; j <= i; j++ )
        {
            f3_t v = bmath_arr_vf3_s_on_section_f3_sum_coprd_sprec( arr_vec, start, end, avg->data[ i ], avg->data[ j ], i, j );
            v /= (end - start);
            o->data[ i * o->stride + j ] = v;
            o->data[ j * o->stride + i ] = v;
        }
    }

    bmath_vf3_s_discard( avg );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_to_stdout( const bmath_mf3_s* o )
{
    printf( "(%zu x %zu)\n", o->rows, o->cols );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v = o->data + i * o->stride;
        for( sz_t j = 0; j < o->cols; j++ )
        {
            if( j > 0 ) printf( " " );
            printf( "%9.3g", v[ j ] );
        }
        printf( "\n" );
    }
}

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    BCORE_LIFE_INIT();

    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m3 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m4 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m5 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v1 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v2 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v3 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v4 );
    BCORE_LIFE_CREATE( bmath_arr_vf3_s, a1 );

    // basic linear
    {
        bmath_mf3_s_set_size( m1, 3, 2 );
        bmath_mf3_s_set_f3( m1, 0, 0, 1 );
        bmath_mf3_s_set_f3( m1, 0, 1, 2 );
        bmath_mf3_s_set_f3( m1, 1, 0, 3 );
        bmath_mf3_s_set_f3( m1, 1, 1, 4 );
        bmath_mf3_s_set_f3( m1, 2, 0, 5 );
        bmath_mf3_s_set_f3( m1, 2, 1, 6 );
        bmath_mf3_s_set_size( m2, 2, 3 );
        bmath_mf3_s_htp( m1, m2 );
        bmath_mf3_s_set_size( m3, 2, 2 );
        bmath_mf3_s_set_size( m4, 2, 2 );
        bmath_mf3_s_mul( m2, m1, m3 );

        ASSERT( bmath_mf3_s_get_f3( m3, 0, 0 ) == 35 );
        ASSERT( bmath_mf3_s_get_f3( m3, 0, 1 ) == 44 );
        ASSERT( bmath_mf3_s_get_f3( m3, 1, 0 ) == 44 );
        ASSERT( bmath_mf3_s_get_f3( m3, 1, 1 ) == 56 );
        bmath_matrix_a_mul( ( bmath_matrix* )m2, ( bmath_matrix* )m1, ( bmath_matrix* )m4 );
        ASSERT( bmath_mf3_s_is_equ( m3, m4 ) );

        bmath_vf3_s_set_size( v1, 2 );
        bmath_vf3_s_set_size( v2, 3 );
        bmath_vf3_s_set_f3( v1, 0, 1 );
        bmath_vf3_s_set_f3( v1, 1, 2 );
        bmath_mf3_s_mul_vec( m1, v1, v2 );
        ASSERT( bmath_vf3_s_get_f3( v2, 0 ) ==  5 );
        ASSERT( bmath_vf3_s_get_f3( v2, 1 ) == 11 );
        ASSERT( bmath_vf3_s_get_f3( v2, 2 ) == 17 );

    }

    // Cholesky decomposition
    {
        bmath_mf3_s_set_size( m1, 3, 3 );
        bmath_mf3_s_set_f3( m1, 0, 0, 1 ); bmath_mf3_s_set_f3( m1, 0, 1, 2 );  bmath_mf3_s_set_f3( m1, 0, 2, 3 );
        bmath_mf3_s_set_f3( m1, 1, 0, 7 ); bmath_mf3_s_set_f3( m1, 1, 1, 5 );  bmath_mf3_s_set_f3( m1, 1, 2, 2 );
        bmath_mf3_s_set_f3( m1, 2, 0, 1 ); bmath_mf3_s_set_f3( m1, 2, 1, 2 );  bmath_mf3_s_set_f3( m1, 2, 2, 4 );

        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_size_to( m1, m3 );
        bmath_mf3_s_set_size_to( m1, m4 );
        bmath_mf3_s_mul_htp( m1, m1, m2 ); // m2 = m1 * m1T = (symmetric)

        bmath_mf3_s_decompose_cholesky( m2, m3 );
        bmath_mf3_s_mul_htp( m3, m3, m4 ); // test cholesky lower triangular
        ASSERT( bmath_mf3_s_is_near_equ( m2, m4, 1E-8 ) );

        bmath_vf3_s_set_size( v1, 3 );
        bmath_vf3_s_set_size_to( v1, v2 );
        bmath_vf3_s_set_size_to( v1, v3 );
        bmath_vf3_s_set_size_to( v1, v4 );

        u2_t rval = 123;
        bmath_vf3_s_fill_random( v1, -1, 1, &rval );
        bmath_mf3_s_mul_vec( m2, v1, v2 );

        bmath_mf3_s_htp( m3, m4 );
        bmath_mf3_s_utr_mul_vec( m4, v1, v3 );
        bmath_mf3_s_ltr_mul_vec( m3, v3, v3 );
        ASSERT( bmath_vf3_s_is_near_equ( v2, v3, 1E-8 ) );

        bmath_mf3_s_ltr_mul_vec(   m3, v1, v2 );
        bmath_mf3_s_ltr_solve_vec( m3, v2, v3 );
        ASSERT( bmath_vf3_s_is_near_equ( v1, v3, 1E-8 ) );

        bmath_mf3_s_utr_mul_vec(   m4, v1, v2 );
        bmath_mf3_s_utr_solve_vec( m4, v2, v3 );
        ASSERT( bmath_vf3_s_is_near_equ( v1, v3, 1E-8 ) );
    }

    // lower triangular inversion
    {
        bmath_mf3_s_set_size( m1, 3, 3 );
        bmath_mf3_s_set_f3( m1, 0, 0, 1 ); bmath_mf3_s_set_f3( m1, 0, 1, 0 );  bmath_mf3_s_set_f3( m1, 0, 2, 0 );
        bmath_mf3_s_set_f3( m1, 1, 0, 7 ); bmath_mf3_s_set_f3( m1, 1, 1, 5 );  bmath_mf3_s_set_f3( m1, 1, 2, 0 );
        bmath_mf3_s_set_f3( m1, 2, 0, 1 ); bmath_mf3_s_set_f3( m1, 2, 1, 2 );  bmath_mf3_s_set_f3( m1, 2, 2, 4 );
        bmath_mf3_s_ltr_inv_htp( m1, m2 );
        bmath_mf3_s_mul_htp( m1, m2, m3 );
        ASSERT( bmath_mf3_s_is_near_one( m3, 1E-10 ) );
    }

    // lu-decomposition
    {
        u2_t rval = 1234;

        sz_t n = 100;
        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );

        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_size_to( m1, m3 );
        bmath_mf3_s_set_size_to( m1, m4 );
        bmath_mf3_s_set_size_to( m1, m5 );

        bmath_mf3_s_decompose_luc( m1, m2 );

        bmath_vf3_s_set_size( v1, n );
        bmath_vf3_s_set_size( v2, n );
        bmath_vf3_s_set_size( v3, n );
        bmath_vf3_s_set_size( v4, n );

        bmath_vf3_s_fill_random( v1, -1, 1, &rval );

        bmath_mf3_s_mul_vec( m1, v1, v2 );
        bmath_mf3_s_utr_mul_vec( m2, v1, v3 );
        bmath_mf3_s_lt1_mul_vec( m2, v3, v4 );
        ASSERT( bmath_vf3_s_is_near_equ( v2, v4, 1E-8 ) );
        bmath_mf3_s_luc_mul_vec( m2, v1, v3 );
        ASSERT( bmath_vf3_s_is_near_equ( v2, v3, 1E-8 ) );

        bmath_mf3_s_luc_solve_vec( m2, v3, v4 );
        ASSERT( bmath_vf3_s_is_near_equ( v1, v4, 1E-8 ) );

        bmath_mf3_s_fill_random( m3, -1, 1, &rval );
        bmath_mf3_s_mul_htp( m1, m3, m4 );

        bmath_mf3_s_luc_mul_htp_htp( m2, m3, m5 );
        bmath_mf3_s_htp( m5, m5 );
        ASSERT( bmath_mf3_s_is_near_equ( m4, m5, 1E-8 ) );

        bmath_mf3_s_htp( m5, m5 );
        bmath_mf3_s_luc_solve_htp_htp( m2, m5, m4 );

        ASSERT( bmath_mf3_s_is_near_equ( m4, m3, 1E-8 ) );
    }

    // A * AT
    {
        sz_t n = 100;
        u2_t rval = 1234;
        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );
        bmath_mf3_s_mul_htp( m1, m1, m2 );
        ASSERT( bmath_mf3_s_is_near_hsm( m2, 1E-8 ) );
    }


    // general inversion
    {
        sz_t n = 100;
        u2_t rval = 1234;
        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_size_to( m1, m3 );
        bmath_mf3_s_set_size_to( m1, m4 );
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );

        bmath_mf3_s_inv( m1, m2 );
        bmath_mf3_s_mul( m1, m2, m3 );
        ASSERT( bmath_mf3_s_is_near_one( m3, 1E-8 ) );
    }

    // general pseudo inversion
    {
        for( sz_t test_id = 0; test_id < 3; test_id++ )
        {
            sz_t n = ( test_id == 0 ) ? 20 : 10;
            sz_t m = ( test_id == 2 ) ? 20 : 10;
            u2_t rval = 1234;
            bmath_mf3_s_set_size( m1, n, m );
            bmath_mf3_s_set_size( m2, m, n );
            bmath_mf3_s_fill_random( m1, -1, 1, &rval );
            bmath_mf3_s_piv( m1, 1E-8, m2 );
            if( m >= n )
            {
                bmath_mf3_s_set_size( m3, n, n );
                bmath_mf3_s_mul( m1, m2, m3 );
            }
            else
            {
                bmath_mf3_s_set_size( m3, m, m );
                bmath_mf3_s_mul( m2, m1, m3 );
            }
            ASSERT( bmath_mf3_s_is_near_one( m3, 1E-8 ) );
        }
    }

    // symmetric inversion, pseudo inversion
    {
        sz_t n = 100;
        u2_t rval = 1234;
        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_size_to( m1, m3 );
        bmath_mf3_s_set_size_to( m1, m4 );
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );
        bmath_mf3_s_mul_htp( m1, m1, m1 ); // m1 = m1*m1T

        bmath_mf3_s_hsm_inv( m1, m2 );
        bmath_mf3_s_mul( m1, m2, m3 );
        ASSERT( bmath_mf3_s_is_near_one( m3, 1E-8 ) );

        bmath_mf3_s_hsm_piv( m1, 1E-8, m2 );
        bmath_mf3_s_mul( m1, m2, m3 );
        ASSERT( bmath_mf3_s_is_near_one( m3, 1E-8 ) );
    }

    // tridiagonalization
    {
        sz_t n = 100;

        bmath_mf3_s_set_size( m1, n, n );
        u2_t rval = 1236;
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );
        bmath_mf3_s_mul_htp( m1, m1, m1 );

        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_size_to( m1, m3 );
        bmath_mf3_s_set_size_to( m1, m4 );
        bmath_mf3_s_cpy( m1, m2 );
        bmath_mf3_s_hsm_trd_htp( m2, NULL );
        ASSERT( bmath_mf3_s_is_trd( m2 ) );
        bmath_mf3_s_cpy( m1, m2 );
        bmath_mf3_s_one( m3 );
        bmath_mf3_s_hsm_trd_htp( m2, m3 );
        ASSERT( bmath_mf3_s_is_trd( m2 ) );

        ASSERT( bmath_mf3_s_is_near_iso( m3, 1E-8 ) );

        bmath_mf3_s_mul( m2, m3, m4 );
        bmath_mf3_s_htp( m3, m3 );
        bmath_mf3_s_mul( m3, m4, m4 );

        ASSERT( bmath_mf3_s_is_near_equ( m1, m4, 1E-8 ) );
    }

    // QR decomposition
    {
        sz_t n = 100;
        u2_t rval = 1236;

        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_size_to( m1, m3 );
        bmath_mf3_s_set_size_to( m1, m4 );
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );

        bmath_mf3_s_cpy( m1, m2 );
        bmath_mf3_s_one( m3 );

        bmath_mf3_s_decompose_qr( m3, m2 );
        ASSERT( bmath_mf3_s_is_near_utr( m2, 1E-8 ) );
        ASSERT( bmath_mf3_s_is_near_iso( m3, 1E-8 ) );

        bmath_mf3_s_htp( m3, m3 );
        bmath_mf3_s_mul( m3, m2, m4 );

        ASSERT( bmath_mf3_s_is_near_equ( m1, m4, 1E-8 ) );
    }

    // eigendecomposition
    {
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
            bmath_mf3_s_evd_jacobi( m2, NULL );
            ASSERT( bmath_mf3_s_is_dag( m2 ) );
            bmath_mf3_s_cpy( m1, m2 );
            bmath_mf3_s_one( m3 );
            bmath_mf3_s_evd_jacobi( m2, m3 );
            ASSERT( bmath_mf3_s_is_dag( m2 ) );
            ASSERT( bmath_mf3_s_is_near_iso( m3, 1E-8 ) );
            bmath_mf3_s_mul( m2, m3, m4 );
            bmath_mf3_s_htp( m3, m3 );
            bmath_mf3_s_mul( m3, m4, m4 );
            ASSERT( bmath_mf3_s_is_near_equ( m1, m4, 1E-8 ) );
        }

        // default evd
        {
            bmath_mf3_s_cpy( m1, m2 );
            bmath_mf3_s_evd( m2, NULL );
            ASSERT( bmath_mf3_s_is_dag( m2 ) );
            bmath_mf3_s_cpy( m1, m2 );
            bmath_mf3_s_one( m3 );
            bmath_mf3_s_evd( m2, m3 );
            ASSERT( bmath_mf3_s_is_dag( m2 ) );
            ASSERT( bmath_mf3_s_is_near_iso( m3, 1E-8 ) );
            bmath_mf3_s_mul( m2, m3, m4 );
            bmath_mf3_s_htp( m3, m3 );
            bmath_mf3_s_mul( m3, m4, m4 );
            ASSERT( bmath_mf3_s_is_near_equ( m1, m4, 1E-8 ) );
        }

    }

    // covariance
    {
        sz_t size = 1000;
        sz_t n    = 10;
        u2_t rval = 124;
        bmath_arr_vf3_s_set_size( a1, size );
        bmath_arr_vf3_s_on_section_set_size( a1, 0, -1, n );
        bmath_arr_vf3_s_on_section_fill_random( a1, 0, -1, -1, 1, &rval );

        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size( m2, n, n );
        bmath_vf3_s_set_size( v1, n );

        bmath_mf3_s_set_covariance_on_section_fast( m1, a1, 0, -1 );
        bmath_mf3_s_set_covariance_on_section_sprc( m2, a1, 0, -1 );
        ASSERT( bmath_mf3_s_is_near_equ( m1, m2, 1E-8 ) );
        ASSERT( f3_abs( bmath_mf3_s_f3_trc( m1 ) - n * 0.3333 ) < 1E-1 ); // trace should be near n/3

        f3_t sqr_norm = 4.0;
        bmath_arr_vf3_s_on_section_set_sqr( a1, 0, -1, sqr_norm ); // normalize all vectors to sqr_norm
        bmath_mf3_s_set_covariance_on_section_fast( m1, a1, 0, -1 );
        ASSERT( f3_abs( bmath_mf3_s_f3_trc( m1 ) - sqr_norm ) < 1E-2 ); // trace should be near sqr_norm (independent of n)

        bmath_arr_vf3_s_on_section_set_sum( a1, 0, -1, 0 ); // set sum of vector components to 0 (introducing a linear dependence)
        bmath_mf3_s_set_covariance_on_section_fast( m1, a1, 0, -1 );
        bmath_mf3_s_evd( m1, NULL );
        ASSERT( f3_abs( bmath_mf3_s_get_f3( m1, n - 1, n - 1 ) ) < 1E-8 ); // last eigenvalue should be near zero
    }

    // affine-transformation
    {
        sz_t n = 20;
        bmath_mf3_s_set_size( m1, n, n + 1 );
        bmath_mf3_s_set_size( m2, n, n + 1 );
        bmath_vf3_s_set_size( v1, n );
        bmath_vf3_s_set_size( v2, n );
        bmath_vf3_s_set_size( v3, n );

        u2_t rval = 1236;
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );
        bmath_vf3_s_fill_random( v1, -1, 1, &rval );

        bmath_mf3_s_mul_av1( m1, v1, v2 );
        bmath_mf3_s_inv_av1( m1, m2 );
        bmath_mf3_s_mul_av1( m2, v2, v3 );

        ASSERT( bmath_vf3_s_is_near_equ( v1, v3, 1E-8 ) );
    }

    // hsm-affine-transformation
    {
        sz_t n = 20;
        bmath_mf3_s_set_size( m1, n, n + 1 );
        bmath_mf3_s_set_size( m2, n, n + 1 );
        bmath_vf3_s_set_size( v1, n );
        bmath_vf3_s_set_size( v2, n );
        bmath_vf3_s_set_size( v3, n );

        u2_t rval = 1236;
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );

        bmath_mf3_s m1_sub = bmath_mf3_s_get_weak_sub_mat( m1, 0, 0, n, n );
        bmath_mf3_s_mul_htp( &m1_sub, &m1_sub, &m1_sub ); // make nxn submatrix of m1 symmetric

        bmath_vf3_s_fill_random( v1, -1, 1, &rval );

        bmath_mf3_s_mul_av1( m1, v1, v2 );
        bmath_mf3_s_hsm_inv_av1( m1, m2 );
        bmath_mf3_s_mul_av1( m2, v2, v3 );

        ASSERT( bmath_vf3_s_is_near_equ( v1, v3, 1E-8 ) );
    }

    // hsm-affine-transformation (test via piv)
    {
        sz_t n = 20;
        bmath_mf3_s_set_size( m1, n, n + 1 );
        bmath_mf3_s_set_size( m2, n, n + 1 );
        bmath_vf3_s_set_size( v1, n );
        bmath_vf3_s_set_size( v2, n );
        bmath_vf3_s_set_size( v3, n );

        u2_t rval = 1236;
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );

        bmath_mf3_s m1_sub = bmath_mf3_s_get_weak_sub_mat( m1, 0, 0, n, n );
        bmath_mf3_s_mul_htp( &m1_sub, &m1_sub, &m1_sub ); // make nxn submatrix of m1 symmetric

        bmath_vf3_s_fill_random( v1, -1, 1, &rval );

        bmath_mf3_s_mul_av1( m1, v1, v2 );
        bmath_mf3_s_hsm_piv_av1( m1, 1E-8, m2 );
        bmath_mf3_s_mul_av1( m2, v2, v3 );

        ASSERT( bmath_vf3_s_is_near_equ( v1, v3, 1E-8 ) );
    }

    BCORE_LIFE_DOWN();
    return NULL;
}

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
        bmath_mf3_s_svd( NULL, a, NULL );
        ASSERT( bmath_mf3_s_is_dag( a ) );

        // a = u'T * a' * v
        bmath_mf3_s_cpy( m0, a );
        bmath_mf3_s_svd( u, a, v );
        ASSERT( bmath_mf3_s_is_dag( a ) );

        ASSERT( bmath_mf3_s_is_near_iso( u, 1E-8 ) );
        ASSERT( bmath_mf3_s_is_near_iso( v, 1E-8 ) );

        bmath_mf3_s_set_size( m1, n, m );
        bmath_mf3_s_mul( a, v, m1 );
        bmath_mf3_s_htp( u, u );
        bmath_mf3_s_mul( u, m1, m1 );

        ASSERT( bmath_mf3_s_is_near_equ( m0, m1, 1E-8 ) );
    }

    BCORE_LIFE_DOWN();
}

/**********************************************************************************************************************/

vd_t bmath_matrix_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_matrix" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FFUNC( bmath_fp_is_equ,         bmath_mf3_s_is_equ );
            BCORE_REGISTER_FFUNC( bmath_fp_is_zro,         bmath_mf3_s_is_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_is_one,         bmath_mf3_s_is_one );

            BCORE_REGISTER_FFUNC( bmath_fp_add,            bmath_mf3_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro,            bmath_mf3_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg,            bmath_mf3_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub,            bmath_mf3_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_cpy,            bmath_mf3_s_cpy );
            BCORE_REGISTER_FFUNC( bmath_fp_mul,            bmath_mf3_s_mul );
            BCORE_REGISTER_FFUNC( bmath_fp_one,            bmath_mf3_s_one );
            BCORE_REGISTER_FFUNC( bmath_fp_inv,            bmath_mf3_s_inv );
            BCORE_REGISTER_FFUNC( bmath_fp_div,            bmath_mf3_s_div );
            BCORE_REGISTER_FFUNC( bmath_fp_matrix_mul_vec, bmath_mf3_s_mul_vec );
            BCORE_REGISTER_FFUNC( bmath_fp_matrix_mul_scl, bmath_mf3_s_mul_scl );

            BCORE_REGISTER_OBJECT( bmath_mf3_s );
        }
        break;

        case TYPEOF_selftest:
        {
            selftest();
            bmath_mf3_s_svd_selftest();
            return NULL;
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

