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

#include "bmath_matrix.h"
#include "bmath_spect_matrix.h"
#include "bmath_fourier.h"
#include <stdio.h>

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST_( bmath_matrix, bmath_mf3_s )
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

bl_t bmath_mf3_s_is_equ( const bmath_mf3_s* o, const bmath_mf3_s* op )
{
    if( o->rows != op->rows ) return false;
    if( o->cols != op->cols ) return false;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        const f3_t* v2 = op->data + i * op->stride;
        for( sz_t j = 0; j < o->cols; j++ ) if( v1[ j ] != v2[ j ] ) return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_zro( const bmath_mf3_s* o )
{
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        for( sz_t j = 0; j < o->cols; j++ ) if( v1[ j ] != 0.0 ) return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_one( const bmath_mf3_s* o )
{
    if( o->rows != o->cols ) return false;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        for( sz_t j = 0; j < o->cols; j++ ) if( v1[ j ] != ( ( j == i ) ? 1.0 : 0.0 ) ) return false;
    }
    return true;
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

f3_t bmath_mf3_s_trc( const bmath_mf3_s* o )
{
    ASSERT( o->cols == o->rows );
    f3_t sum = 0;
    for( sz_t i = 0; i < o->rows; i++ ) sum += o->data[ i * ( o->stride + 1 ) ];
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

    ASSERT(  o->rows ==  op->cols );
    ASSERT(  o->cols ==  op->rows );
    ASSERT(  o->rows == res->rows );
    ASSERT( op->cols == res->cols );

    for( sz_t i = 0; i < res->rows; i++ )
    {
        f3_t* vr = res->data + i * res->stride;
        const f3_t* v1 = o ->data + i * o ->stride;

        for( sz_t j = 0; j < res->cols; j++ )
        {
            const f3_t* v2 = op->data + j;
            f3_t sum = 0;
            for( sz_t k = 0; k < o->cols; k++ )
            {
                sum += v1[ k ] * v2[ k * op->stride ];
            }
            vr[ j ] = sum;
        }
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

    sz_t n = o->rows;

    ASSERT( n == res->rows );
    ASSERT( n == res->cols );

    if( o == op ) // symmetric result
    {
        for( sz_t i = 0; i < n; i++ )
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

    ASSERT( bmath_mf3_s_is_equ_size( o, op ) );

    for( sz_t i = 0; i < n; i++ )
    {
              f3_t* vri = res->data + i * res->stride;
        const f3_t* voi =   o->data + i *   o->stride;

        for( sz_t j = 0; j < n; j++ )
        {
            const f3_t* vpj = op->data + j * op->stride;
            f3_t sum = 0;
            for( sz_t k = 0; k < o->cols; k++ ) sum += voi[ k ] * vpj[ k ];
            vri[ j ] = sum;
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
        bmath_mf3_s_inv( o, buf );
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
    bmath_mf3_s* luc = bmath_mf3_s_create();
    bmath_mf3_s_set_size_to( o, luc );
    bmath_mf3_s_set_size_to( o, res );
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

void bmath_mf3_s_div( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    bmath_mf3_s_inv( op, res );
    bmath_mf3_s_mul( o, res, res );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_vec( const bmath_mf3_s* o, const bmath_vf3_s* op, bmath_vf3_s* res )
{
    if( res == op )
    {
        bmath_vf3_s* buf = bmath_vf3_s_create_set_size( res->size );
        bmath_mf3_s_mul_vec( o, op, buf );
        bmath_vf3_s_cpy( buf, res );
        bmath_vf3_s_discard( buf );
        return;
    }

    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    f3_t* vr = res->data;
    f3_t* v2 = op->data;
    for( sz_t i = 0; i < res->size; i++ )
    {
        f3_t* v1 = o->data + i * o->stride;
        f3_t sum = 0;
        for( sz_t j = 0; j < o->cols; j++ ) sum += v1[ j ] * v2[ j ];
        vr[ i ] = sum;
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
    bcore_u_memcpy( sizeof( f3_t ), o->data + idx * o->stride, data, o->cols );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_col_by_data( bmath_mf3_s* o, sz_t idx, const f3_t* data )
{
    f3_t* v = o ->data + idx;
    for( sz_t i = 0; i < o->rows; i++ ) v[ i * o ->stride ] = data[ i ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_row_by_vec ( bmath_mf3_s* o, sz_t idx, const bmath_vf3_s* vec )
{
    ASSERT( idx < o->rows );
    ASSERT( o->cols <= vec->size );
    bmath_mf3_s_set_row_by_data( o, idx, vec->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_col_by_vec ( bmath_mf3_s* o, sz_t idx, const bmath_vf3_s* vec )
{
    ASSERT( idx < o->cols );
    ASSERT( o->rows <= vec->size );
    bmath_mf3_s_set_col_by_data( o, idx, vec->data );
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

void bmath_mf3_s_hsm_svd_jacobi_htp( const bmath_mf3_s* o, bmath_mf3_s* d, bmath_mf3_s* r )
{
    ASSERT( o != r );
    // o == diag is allowed
    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, r ) );
    bmath_mf3_s_cpy( o, d );
    bmath_mf3_s_one( r );

    sz_t n = o->rows;

    f3_t max_off = 0;

    do
    {
        max_off = 0;

        for( sz_t k = 0; k < n; k++ )
        {
            f3_t* dk = d->data + k * d->stride;
            f3_t* rk = r->data + k * r->stride;
            for( sz_t l = 0; l < k; l++ )
            {
                f3_t* dl = d->data + l * d->stride;
                f3_t* rl = r->data + l * r->stride;
                f3_t theta = 0.5 * atan2( 2 * dk[l], dk[k] - dl[l] );
                f3_t c = cos( theta );
                f3_t s = sin( theta );

                f3_t cc = c * c;
                f3_t ss = s * s;

                f3_t dkk = dk[k];
                f3_t dll = dl[l];
                f3_t dkl = dk[l];
                f3_t dklcsx2 = 2 * dkl * c * s;

                dk[k] = dkk * cc + dll * ss + dklcsx2;
                dl[l] = dll * cc + dkk * ss - dklcsx2;
                dk[l] = 0;
                dl[k] = 0;

                for( sz_t i = 0; i < n; i++ )
                {
                    if( i != k && i != l )
                    {
                        f3_t* di = d->data + i * d->stride;
                        f3_t dki = dk[ i ] * c + dl[ i ] * s;
                        f3_t dli = dl[ i ] * c - dk[ i ] * s;
                        dk[ i ] = dki;
                        dl[ i ] = dli;
                        di[ k ] = dki;
                        di[ l ] = dli;

                        max_off = f3_max( max_off, f3_max( dki, dkl ) );
                    }
                }

                for( sz_t i = 0; i < n; i++ )
                {
                    f3_t rki = rk[ i ];
                    f3_t rli = rl[ i ];
                    rk[ i ] = rki * c + rli * s;
                    rl[ i ] = rli * c - rki * s;
                }
            }
        }
    }
    while( max_off > 0 );

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
    bcore_life_s* l = bcore_life_s_create();
    bmath_mf3_s* m1 = bcore_life_s_push_aware( l, bmath_mf3_s_create() );
    bmath_mf3_s* m2 = bcore_life_s_push_aware( l, bmath_mf3_s_create() );
    bmath_mf3_s* m3 = bcore_life_s_push_aware( l, bmath_mf3_s_create() );
    bmath_mf3_s* m4 = bcore_life_s_push_aware( l, bmath_mf3_s_create() );
    bmath_mf3_s* m5 = bcore_life_s_push_aware( l, bmath_mf3_s_create() );
    bmath_vf3_s* v1 = bcore_life_s_push_aware( l, bmath_vf3_s_create() );
    bmath_vf3_s* v2 = bcore_life_s_push_aware( l, bmath_vf3_s_create() );
    bmath_vf3_s* v3 = bcore_life_s_push_aware( l, bmath_vf3_s_create() );
    bmath_vf3_s* v4 = bcore_life_s_push_aware( l, bmath_vf3_s_create() );

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

    // general inversion
    {
        sz_t n = 100;
        u2_t rval = 1234;
        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_size_to( m1, m3 );
        bmath_mf3_s_set_size_to( m1, m4 );
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );
        TIME_TO_STDOUT( bmath_mf3_s_inv( m1, m2 ) );
        bmath_mf3_s_mul( m1, m2, m3 );
        ASSERT( bmath_mf3_s_is_near_one( m3, 1E-7 ) );
    }

    // symmetric inversion
    {
        sz_t n = 100;
        u2_t rval = 1234;
        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_size_to( m1, m3 );
        bmath_mf3_s_set_size_to( m1, m4 );
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );
        bmath_mf3_s_mul_htp( m1, m1, m1 ); // m1 = m1*m1T

        TIME_TO_STDOUT( bmath_mf3_s_hsm_inv( m1, m2 ) );
        bmath_mf3_s_mul( m1, m2, m3 );
        ASSERT( bmath_mf3_s_is_near_one( m3, 1E-7 ) );
    }

    // jacobi svd
    {
        sz_t n = 100;

        bmath_mf3_s_set_size( m1, n, n );
        u2_t rval = 1234;
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );
        bmath_mf3_s_mul_htp( m1, m1, m1 );

        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_size_to( m1, m3 );
        bmath_mf3_s_set_size_to( m1, m4 );
        TIME_TO_STDOUT( bmath_mf3_s_hsm_svd_jacobi_htp( m1, m2, m3 ) );
        ASSERT( bmath_mf3_s_is_near_iso( m3, 1E-8 ) );

        bmath_mf3_s_mul( m2, m3, m4 );
        bmath_mf3_s_htp( m3, m3 );
        bmath_mf3_s_mul( m3, m4, m4 );

        ASSERT( bmath_mf3_s_is_near_equ( m1, m4, 1E-8 ) );
    }

    bcore_life_s_discard( l );
    return NULL;
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
            return selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

