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

/// givens rotation
typedef struct gr_s { f3_t c; f3_t s; } gr_s;

/// setup rotation to annihilate b
static inline void gr_s_init_as_ani_b( gr_s* o, f3_t a, f3_t b )
{
    b = ( a > 0 ) ? b : -b;
    a = ( a > 0 ) ? a : -a;
    f3_t r = hypot( b, a );
    o->c = ( r > 1E-308 ) ? a / r : 1;
    o->s = ( r > 1E-308 ) ? b / r : 0;
}

/// setup rotation to annihilate b and apply rotation to vector (a,b)
static inline void gr_s_init_do_ani_b( gr_s* o, f3_t* a, f3_t* b )
{
    gr_s_init_as_ani_b( o, *a, *b );
    *a = o->c * *a + o->s * *b;
    *b = 0;
}

/// apply rotation to vector (a,b) assuming it annihilates b (sets b zero)
static inline void gr_s_ani_b( gr_s* o, f3_t* a, f3_t* b )
{
    *a = o->c * *a + o->s * *b;
    *b = 0;
}

/// apply rotation to vector (a,b)
static inline void gr_s_rot( const gr_s* o, f3_t* a, f3_t* b )
{
    f3_t a0 = *a;
    *a = o->c * a0 + o->s * *b;
    *b = o->c * *b - o->s * a0;
}

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

bl_t bmath_mf3_s_is_dag( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_dag( o, 0 ); }
bl_t bmath_mf3_s_is_trd( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_trd( o, 0 ); }
bl_t bmath_mf3_s_is_utr( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_utr( o, 0 ); }
bl_t bmath_mf3_s_is_ltr( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_ltr( o, 0 ); }
bl_t bmath_mf3_s_is_hsm( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_hsm( o, 0 ); }
bl_t bmath_mf3_s_is_ubd( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_ubd( o, 0 ); }
bl_t bmath_mf3_s_is_lbd( const bmath_mf3_s* o ) { return bmath_mf3_s_is_near_lbd( o, 0 ); }

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
        const f3_t* vi = o ->data + i * o ->stride;
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
        const f3_t* vi = o ->data + i * o ->stride;
        for( sz_t j = 0; j < o->cols; j++ )
        {
            if( ( j < i - 1 || j > i ) && f3_abs( vi[ j ] ) > max_dev ) return false;
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

    ASSERT( o->rows  == res->rows );
    ASSERT( op->rows == res->cols );

    if( o == op ) // symmetric result
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


    for( sz_t i = 0; i < o->rows; i++ )
    {
              f3_t* vri = res->data + i * res->stride;
        const f3_t* voi =   o->data + i *   o->stride;

        for( sz_t j = 0; j < op->rows; j++ )
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

void bmath_mf3_s_hsm_trd_htp_givens( bmath_mf3_s* a, bmath_mf3_s* v )
{
    ASSERT( a->rows == a->cols );
    if( v )
    {
        ASSERT( v != a );
        ASSERT( v->cols == a->rows );
        ASSERT( v->rows == a->rows );
    }

    gr_s gr;

    for( sz_t j = 0; j < a->rows; j++ )
    {
        for( sz_t k = a->rows - 2; k > j; k-- )
        {
            sz_t l = k + 1;
            f3_t* ak;

            // zero upper row
            ak = a->data + k;
            sz_t jstride = j * a->stride;
            gr_s_init_do_ani_b( &gr, ak + jstride, ak + jstride + 1 );
            for( sz_t i = j + 1; i < a->rows; i++ ) gr_s_rot( &gr, ak + i * a->stride, ak + i * a->stride + 1 );

            // transposed operation
            ak = a->data + k * a->stride;
            gr_s_ani_b( &gr, ak + j, ak + a->stride + j );
            for( sz_t i = j + 1; i < a->cols; i++ ) gr_s_rot( &gr, ak + i, ak + a->stride + i );

            // rotation matrix
            if( v )
            {
                f3_t* vk = v->data + k * v->stride;
                f3_t* vl = v->data + l * v->stride;
                for( sz_t i = 0; i < v->cols; i++ ) gr_s_rot( &gr, vk + i, vl + i );
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ubd_htp_givens( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v )
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

    gr_s gr;
    sz_t min_cols_rows = sz_min( a->cols, a->rows );

    for( sz_t j = 0; j < min_cols_rows; j++ )
    {
        // zero lower column
        for( sz_t l = a->rows - 1; l > j; l-- )
        {
            sz_t k = l - 1;
            f3_t* ak = a->data + k * a->stride;
            f3_t* al = a->data + l * a->stride;
            gr_s_init_do_ani_b( &gr, ak + j, al + j );
            for( sz_t i = j + 1; i < a->cols; i++ ) gr_s_rot( &gr, ak + i, al + i );
            if( u )
            {
                f3_t* uk = u->data + k * u->stride;
                f3_t* ul = u->data + l * u->stride;
                for( sz_t i = 0; i < u->cols; i++ ) gr_s_rot( &gr, uk + i, ul + i );
            }
        }

        // zero upper row
        for( sz_t l = a->cols - 1; l > j + 1; l-- )
        {
            sz_t k = l - 1;
            f3_t* ak = a->data + k;
            f3_t* al = a->data + l;
            sz_t jstride = j * a->stride;
            gr_s_init_do_ani_b( &gr, ak + jstride, al + jstride );
            for( sz_t i = j + 1; i < a->rows; i++ ) gr_s_rot( &gr, ak + i * a->stride, al + i * a->stride );
            if( v )
            {
                f3_t* vk = v->data + k * v->stride;
                f3_t* vl = v->data + l * v->stride;
                for( sz_t i = 0; i < v->cols; i++ ) gr_s_rot( &gr, vk + i, vl + i );
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_qr_rot_htp_utr_givens( bmath_mf3_s* q, bmath_mf3_s* r )
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

            f3_t s, c;
            f3_arc_to_sin_cos( rk[l], rl[l], &s, &c );

            rl[ l ] = rl[ l ] * c + rk[ l ] * s;
            rk[ l ] = 0;

            for( sz_t i = l + 1; i < n; i++ )
            {
                f3_t rli = rl[ i ];
                f3_t rki = rk[ i ];
                rl[ i ] = rli * c + rki * s;
                rk[ i ] = rki * c - rli * s;
            }

            if( q )
            {
                f3_t* ql = q->data + l * q->stride;
                f3_t* qk = q->data + k * q->stride;
                for( sz_t i = 0; i < n; i++ )
                {
                    f3_t qli = ql[ i ];
                    f3_t qki = qk[ i ];
                    ql[ i ] = qli * c + qki * s;
                    qk[ i ] = qki * c - qli * s;
                }
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_evd_htp_jacobi( bmath_mf3_s* a, bmath_mf3_s* v )
{
    ASSERT( bmath_mf3_s_is_square( a ) );

    if( v )
    {
        ASSERT( a != v );
        ASSERT( bmath_mf3_s_is_equ_size( a, v ) );
    }

    sz_t n = a->rows;
    sz_t max_cycles = 100;

    for( sz_t cycle = 0; cycle < max_cycles; cycle++ )
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
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_evd_htp_qr_xshift( bmath_mf3_s* a, bmath_mf3_s* v )
{
    sz_t n = a->rows;
    if( n <= 1 ) return; // nothing to do

    // tridiagonalization (this part takes the bulk cpu time)
    bmath_mf3_s_hsm_trd_htp_givens( a, v );

    for( sz_t block_n = n; block_n > 1; block_n-- )
    {
        sz_t max_cycles = 100; // usually convergence is reached after 2...3 cycles
        f3_t shift_sum = 0;
        for( sz_t cycle = 0; cycle < max_cycles; cycle++ )
        {
            // aij of lower 2x2 sub-matrix
            f3_t a11 = a->data[ ( block_n - 1 ) * ( a->stride + 1 )     ];
            f3_t a00 = a->data[ ( block_n - 2 ) * ( a->stride + 1 )     ];
            f3_t a01 = a->data[ ( block_n - 2 ) * ( a->stride + 1 ) + 1 ];

            if( ( f3_abs( a01 ) < 1E-308 ) || ( f3_abs( a01 ) < f3_abs( a11 ) * 1E-20 ) )
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

            f3_t c, s;
            f3_arc_to_sin_cos( a0[ a->stride ], r00, &s, &c );

            for( sz_t i = 0; i < block_n - 1; i++ )
            {
                if( v )
                {
                    f3_t* v0 = v->data + i * v->stride;
                    f3_t* v1 = v0      +     v->stride;
                    for( sz_t i = 0; i < n; i++ )
                    {
                        f3_t v0i = v0[ i ];
                        f3_t v1i = v1[ i ];
                        v0[ i ] = v0i * c + v1i * s;
                        v1[ i ] = v1i * c - v0i * s;
                    }
                }

                f3_t* a1 = a0 + a->stride;

                f3_t r11 = a1[ 1 ] * c - r01 * s;
                f3_t r12 = a1[ 2 ] * c;

                     r00 = r00 * c + a1[ 0 ] * s;
                     r01 = r01 * c + a1[ 1 ] * s;
                f3_t r02 =           a1[ 2 ] * s;

                a0[ 0 ] = r00 * cp * c + r01 * s;

                f3_t cn, sn;
                f3_arc_to_sin_cos( ( i == block_n - 2 ) ? 0 : a1[ a->stride + 1 ], r11, &sn, &cn );

                // off-diagonal element
                a1[ 0 ] = a0[ 1 ] = -r00 * s * cp * cn + r01 * c * cn + r02 * sn;

                r00 = r11;
                r01 = r12;

                a0 = a1 + 1;

                cp = c;

                c = cn;
                s = sn;
            }
            a0[ 0 ] = r00 * cp;

        }
        for( sz_t i = 0; i < block_n; i++ ) a->data[ i * ( a->stride + 1 ) ] += shift_sum;
    }

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

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_evd_htp_qr_ishift( bmath_mf3_s* a, bmath_mf3_s* v )
{
    sz_t n = a->rows;
    if( n <= 1 ) return; // nothing to do

    /// tridiagonalization
    bmath_mf3_s_hsm_trd_htp_givens( a, v );

    // qr iteration until smallest non-diag element < offd_limit;

    for( sz_t block_n = n; block_n > 1; block_n-- )
    {
        sz_t max_cycles = 100; // usually convergence is reached after 2...3 cycles
        for( sz_t cycle = 0; cycle < max_cycles; cycle++ )
        {
            f3_t lambda;
            {
                // aij of lower 2x2 sub-matrix
                f3_t a11 = a->data[ ( block_n - 1 ) * ( a->stride + 1 )     ];
                f3_t a00 = a->data[ ( block_n - 2 ) * ( a->stride + 1 )     ];
                f3_t a01 = a->data[ ( block_n - 2 ) * ( a->stride + 1 ) + 1 ];

                if( ( f3_abs( a01 ) < 1E-308 ) || ( f3_abs( a01 ) < f3_abs( a11 ) * 1E-20 ) )
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
            f3_t ci = 1, si = 0;
            f3_t cj, sj;

            f3_t b = ci * ajj - si * ch * aij;
            f3_arc_to_sin_cos( ajk, b - ci * lambda, &sj, &cj );

            for( sz_t j = 0; j < block_n - 1; j++ )
            {
                if( v )
                {
                    f3_t* v0 = v->data + j * v->stride;
                    f3_t* v1 = v0      +     v->stride;
                    for( sz_t i = 0; i < n; i++ )
                    {
                        f3_t v0i = v0[ i ];
                        f3_t v1i = v1[ i ];
                        v0[ i ] = v0i * cj + v1i * sj;
                        v1[ i ] = v1i * cj - v0i * sj;
                    }
                }

                f3_t akk = pajj[ a->stride + 1 ];
                f3_t akl = ( j == block_n - 2 ) ? 0 : pajj[ a->stride + 2 ];

                f3_t rjj = cj * b + sj * ajk;
                f3_t rjk = cj * ci * ajk + sj * akk;
                f3_t rjl = sj * akl;

                f3_t f3_sqr_si_cj_lambda = f3_sqr( si ) * cj * lambda;

                pajj[ 0 ] =  rjj * cj * ci + rjk * sj + cj * f3_sqr_si_cj_lambda;

                b = cj * akk - sj * ci * ajk;
                f3_t ck, sk;
                f3_arc_to_sin_cos( akl, b - cj * lambda, &sk, &ck );

                f3_t offd = -rjj * sj * ci * ck + rjk * ck * cj + rjl * sk - sj * ck * f3_sqr_si_cj_lambda;
                pajj[ 1 ] = offd;
                pajj[ a->stride ] = offd;

                pajj += a->stride + 1;
                ch = ci;
                ci = cj; si = sj;
                cj = ck; sj = sk;

                aij = ajk;
                ajj = akk;
                ajk = akl;
            }

            pajj[ 0 ] = b * ci + f3_sqr( si ) * lambda;
        }
    }

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

//---------------------------------------------------------------------------------------------------------------------

// pseudo inversion
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
    bmath_mf3_s_evd_htp( a, q );

    bmath_vf3_s* dag = bmath_vf3_s_create();
    bmath_vf3_s_set_size( dag, n );
    bmath_mf3_s_get_dag_vec( a, dag );

    f3_t thr = dag->data[ 0 ] * eps; // dag is sorted in descending manner

    for( sz_t i = 0; i < n; i++ ) dag->data[ i ] = ( dag->data[ i ] < thr ) ? 0 : ( 1.0 / dag->data[ i ] );

    bmath_mf3_s_htp( q, q );
    bmath_mf3_s_udu_htp( q, dag, res );

    bmath_vf3_s_discard( dag );
    bmath_mf3_s_discard( q );
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
        ASSERT( bmath_mf3_s_is_near_hsm( m2, 1E-7 ) );
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

        bmath_mf3_s_hsm_inv( m1, m2 );
        bmath_mf3_s_mul( m1, m2, m3 );
        ASSERT( bmath_mf3_s_is_near_one( m3, 1E-7 ) );
    }

    // symmetric pseudo inversion
    {
        sz_t n = 50;
        u2_t rval = 1234;
        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_size_to( m1, m3 );
        bmath_mf3_s_set_size_to( m1, m4 );
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );
        bmath_mf3_s_mul_htp( m1, m1, m1 ); // m1 = m1*m1T

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
        bmath_mf3_s_hsm_trd_htp_givens( m2, NULL );
        ASSERT( bmath_mf3_s_is_trd( m2 ) );
        bmath_mf3_s_cpy( m1, m2 );
        bmath_mf3_s_one( m3 );
        bmath_mf3_s_hsm_trd_htp_givens( m2, m3 );
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

        bmath_mf3_s_qr_rot_htp_utr_givens( m3, m2 );
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
            bmath_mf3_s_evd_htp_jacobi( m2, NULL );
            ASSERT( bmath_mf3_s_is_dag( m2 ) );
            bmath_mf3_s_cpy( m1, m2 );
            bmath_mf3_s_one( m3 );
            bmath_mf3_s_evd_htp_jacobi( m2, m3 );
            ASSERT( bmath_mf3_s_is_dag( m2 ) );
            ASSERT( bmath_mf3_s_is_near_iso( m3, 1E-8 ) );
            bmath_mf3_s_mul( m2, m3, m4 );
            bmath_mf3_s_htp( m3, m3 );
            bmath_mf3_s_mul( m3, m4, m4 );
            ASSERT( bmath_mf3_s_is_near_equ( m1, m4, 1E-8 ) );
        }

        // qr_xshift
        {
            bmath_mf3_s_cpy( m1, m2 );
            bmath_mf3_s_evd_htp_qr_xshift( m2, NULL );
            ASSERT( bmath_mf3_s_is_dag( m2 ) );
            bmath_mf3_s_cpy( m1, m2 );
            bmath_mf3_s_one( m3 );
            bmath_mf3_s_evd_htp_qr_xshift( m2, m3 );
            ASSERT( bmath_mf3_s_is_dag( m2 ) );
            ASSERT( bmath_mf3_s_is_near_iso( m3, 1E-8 ) );
            bmath_mf3_s_mul( m2, m3, m4 );
            bmath_mf3_s_htp( m3, m3 );
            bmath_mf3_s_mul( m3, m4, m4 );
            ASSERT( bmath_mf3_s_is_near_equ( m1, m4, 1E-8 ) );
        }

        // qr_ishift
        {
            bmath_mf3_s_cpy( m1, m2 );
            bmath_mf3_s_evd_htp_qr_ishift( m2, NULL );
            ASSERT( bmath_mf3_s_is_dag( m2 ) );
            bmath_mf3_s_cpy( m1, m2 );
            bmath_mf3_s_one( m3 );
            bmath_mf3_s_evd_htp_qr_ishift( m2, m3 );
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
        bmath_mf3_s_evd_htp( m1, NULL );
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

