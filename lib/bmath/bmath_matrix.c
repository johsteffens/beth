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
#include "bmath_grt.h"
#include "bmath_matrix_eval.h"

#include <stdio.h>

/**********************************************************************************************************************/
/// Matrix object of f3_t

BCORE_DEFINE_OBJECT_INST( bmath_matrix, bmath_mf3_s )
"{"
    "aware_t _;"
    "uz_t rows;"
    "uz_t cols;"
    "uz_t stride;"
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

void bmath_mf3_s_set_size( bmath_mf3_s* o, uz_t rows, uz_t cols )
{
    bcore_matrix_a_set_size( ( bcore_matrix* )o, rows, cols );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_fill_random( bmath_mf3_s* o, f3_t min, f3_t max, u2_t* rval )
{
    f3_t range = max - min;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        f3_t* v = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            v[ j ] = ( range * f3_xsg1_pos( rval ) ) + min;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_fill_random_sparse( bmath_mf3_s* o, f3_t min, f3_t max, f3_t density, u2_t* rval )
{
    f3_t range = max - min;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        f3_t* v = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            if( f3_xsg1_pos( rval ) < density )
            {
                v[ j ] = ( range * f3_xsg1_pos( rval ) ) + min;
            }
            else
            {
                v[ j ] = 0;
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_fill_random_pow( bmath_mf3_s* o, f3_t exp, u2_t* rval )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        f3_t* v = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            f3_t r = f3_xsg1_sym( rval );
            v[ j ] = f3_sig( r ) * pow( f3_abs( r ), exp );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

bmath_mf3_s* bmath_mf3_s_create_set_size( uz_t rows, uz_t cols )
{
    bmath_mf3_s* o = bmath_mf3_s_create();
    bmath_mf3_s_set_size( o, rows, cols );
    return o;
}

//---------------------------------------------------------------------------------------------------------------------

bmath_mf3_s* bmath_mf3_s_create_fill_random( uz_t rows, uz_t cols, f3_t min, f3_t max, u2_t* rval )
{
    bmath_mf3_s* o = bmath_mf3_s_create_set_size( rows, cols );
    bmath_mf3_s_fill_random( o, min, max, rval );
    return o;
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// checks, deviations

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_equ( const bmath_mf3_s* o, const bmath_mf3_s* op, f3_t max_dev )
{
    if( o->rows != op->rows ) return false;
    if( o->cols != op->cols ) return false;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        const f3_t* v2 = op->data + i * op->stride;
        for( uz_t j = 0; j < o->cols; j++ ) if( f3_abs( v1[ j ] - v2[ j ] ) > max_dev ) return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_zro( const bmath_mf3_s* o, f3_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ ) if( f3_abs( v1[ j ] ) > max_dev ) return false;
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_one( const bmath_mf3_s* o, f3_t max_dev )
{
    if( o->rows != o->cols ) return false;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            if( f3_abs( v1[ j ] - ( ( j == i ) ? 1.0 : 0.0 ) ) > max_dev ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_dag( const bmath_mf3_s* o, f3_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ )
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
    uz_t n = o->rows;
    for( uz_t i = 0; i < n; i++ )
    {
        const f3_t* vi = o ->data + i * o ->stride;
        for( uz_t j = i + 2; j < n; j++ )
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
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            if( ( j < i || j > i + 1 ) && ( f3_abs( vi[ j ] ) > max_dev ) ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_lbd( const bmath_mf3_s* o, f3_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            if( ( j + 1 < i || j > i ) && ( f3_abs( vi[ j ] ) > max_dev ) ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_otn( const bmath_mf3_s* o, f3_t max_dev )
{
    if( o->rows <= o->cols )
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const f3_t* v1 = o ->data + i * o ->stride;
            for( uz_t j = i; j < o->rows; j++ )
            {
                const f3_t* v2 = o ->data + j * o ->stride;
                f3_t dot_prd = 0;
                for( uz_t k = 0; k < o->cols; k++ ) dot_prd += v1[ k ] * v2[ k ];
                if( f3_abs( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) ) > max_dev ) return false;
            }
        }
    }
    else
    {
        for( uz_t i = 0; i < o->cols; i++ )
        {
            const f3_t* v1 = o ->data + i;
            for( uz_t j = i; j < o->cols; j++ )
            {
                const f3_t* v2 = o ->data + j;
                f3_t dot_prd = 0;
                for( uz_t k = 0; k < o->rows; k++ ) dot_prd += v1[ k * o->stride ] * v2[ k * o->stride ];
                if( f3_abs( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) ) > max_dev ) return false;
            }
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_uni( const bmath_mf3_s* o, f3_t max_dev )
{
    if( o->rows != o->cols ) return false;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = i; j < o->rows; j++ )
        {
            const f3_t* v2 = o ->data + j * o ->stride;
            f3_t dot_prd = 0;
            for( uz_t k = 0; k < o->cols; k++ ) dot_prd += v1[ k ] * v2[ k ];
            if( f3_abs( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) ) > max_dev ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_utr( const bmath_mf3_s* o, f3_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v = o ->data + i * o->stride;
        uz_t j_end = uz_min( i, o->cols );
        for( uz_t j = 0; j < j_end; j++ )
        {
            if( f3_abs( v[ j ] ) > max_dev ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_is_near_ltr( const bmath_mf3_s* o, f3_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v = o ->data + i * o->stride;
        for( uz_t j = i + 1; j < o->cols; j++ )
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
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vi = o ->data + i * o ->stride;
        for( uz_t j = 0; j < i; j++ )
        {
            const f3_t* vj = o ->data + j * o ->stride;
            if( f3_abs( vi[ j ] - vj[ i ] ) > max_dev ) return false;
        }
    }
    return true;
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_mf3_s_fdev_equ( const bmath_mf3_s* o, const bmath_mf3_s* op )
{
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == op->cols );

    f3_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* ai = o->data + i * o->stride;
        const f3_t* bi = op->data + i * op->stride;
        for( uz_t j = 0; j < o->cols; j++ ) sum += f3_sqr( ai[ j ] - bi[ j ] );
    }
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_mf3_s_fdev_zro( const bmath_mf3_s* o )
{
    f3_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* oi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ ) sum += f3_sqr( oi[ j ] );
    }
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_mf3_s_fdev_one( const bmath_mf3_s* o )
{
    f3_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* oi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ ) sum += f3_sqr( oi[ j ] - ( i == j ? 1 : 0 ) );
    }
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_mf3_s_fdev_otn( const bmath_mf3_s* o )
{
    f3_t sum = 0;
    if( o->rows <= o->cols )
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const f3_t* v1 = o ->data + i * o ->stride;
            for( uz_t j = i; j < o->rows; j++ )
            {
                const f3_t* v2 = o ->data + j * o ->stride;
                f3_t dot_prd = 0;
                for( uz_t k = 0; k < o->cols; k++ ) dot_prd += v1[ k ] * v2[ k ];
                sum += f3_sqr( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) );
            }
        }
    }
    else
    {
        for( uz_t i = 0; i < o->cols; i++ )
        {
            const f3_t* v1 = o ->data + i;
            for( uz_t j = i; j < o->cols; j++ )
            {
                const f3_t* v2 = o ->data + j;
                f3_t dot_prd = 0;
                for( uz_t k = 0; k < o->rows; k++ ) dot_prd += v1[ k * o->stride ] * v2[ k * o->stride ];
                sum += f3_sqr( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) );
            }
        }
    }
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// initializations; copying; basic matrix operations

f3_t bmath_mf3_s_f3_trc( const bmath_mf3_s* o )
{
    ASSERT( o->cols == o->rows );
    f3_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ ) sum += o->data[ i * ( o->stride + 1 ) ];
    return sum;
}

//---------------------------------------------------------------------------------------------------------------------

f3_t bmath_mf3_s_f3_sub_sqr( const bmath_mf3_s* o, const bmath_mf3_s* op )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, op ) );
    f3_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 =  o->data + i *  o->stride;
        const f3_t* v2 = op->data + i * op->stride;
        for( uz_t i = 0; i < o->cols; i++ ) sum += f3_sqr( v1[ i ] - v2[ i ] );
    }
    return sum;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_add( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, op ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data  + i * o ->stride;
        const f3_t* v2 = op->data  + i * op->stride;
              f3_t* vr = res->data + i * res->stride;
        for( uz_t j = 0; j < o->cols; j++ ) vr[ j ] = v1[ j ] + v2[ j ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_sub( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, op ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data  + i * o ->stride;
        const f3_t* v2 = op->data  + i * op->stride;
              f3_t* vr = res->data + i * res->stride;
        for( uz_t j = 0; j < o->cols; j++ ) vr[ j ] = v1[ j ] - v2[ j ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_zro( bmath_mf3_s* o )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        f3_t* v1 = o ->data  + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ ) v1[ j ] = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_one( bmath_mf3_s* o )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        f3_t* v1 = o ->data  + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ ) v1[ j ] = ( j == i ) ? 1 : 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_neg( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data  + i * o ->stride;
              f3_t* vr = res->data + i * res->stride;
        for( uz_t j = 0; j < o->cols; j++ ) vr[ j ] = -v1[ j ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_cpy( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    if( res == o ) return;
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data  + i * o ->stride;
              f3_t* vr = res->data + i * res->stride;
        for( uz_t j = 0; j < o->cols; j++ ) vr[ j ] = v1[ j ];
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
    for( uz_t j = 0; j < o->cols; j++ )
    {
        const f3_t* bj = op->data + j * op->stride;
        for( uz_t i = 0; i < o->rows; i++ )
        {
            f3_t aij =   o->data[  i *   o->stride + j ];
            f3_t* ri = res->data + i * res->stride;
            for( uz_t k = 0; k < op->cols; k++ ) ri[ k ] += aij * bj[ k ];
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
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vo =   o->data + i *   o->stride;
              f3_t* vr = res->data + i * res->stride;
        for( uz_t j = 0; j < o->cols; j++ ) vr[ j ] = vo[ j ] * vd[ j ];
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
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vo =   o->data + i *   o->stride;
              f3_t* vr = res->data + i * res->stride;
              f3_t d = vd[ i ];
        for( uz_t j = 0; j < o->cols; j++ ) vr[ j ] = vo[ j ] * d;
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
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* oi = o->data   + o->stride * i;
              f3_t* ri = res->data + res->stride * i;
        for( uz_t j = 0; j < o->cols; j++ ) ri[ j ] = oi[ j ] + ( v1[ i ] * v2[ j ] );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_htp( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    ASSERT( o->cols  == op->cols );
    ASSERT( o->rows  == res->rows );
    ASSERT( op->rows == res->cols );

    if( o == op ) // result is symmetric -> we can safe half of the work
    {
        if( res == o )
        {
            bmath_mf3_s* buf = bmath_mf3_s_create();
            bmath_mf3_s_set_size( buf, res->rows, res->cols );
            bmath_mf3_s_mul_htp( o, op, buf );
            bmath_mf3_s_cpy( buf, res );
            bmath_mf3_s_discard( buf );
        }
        else
        {
            for( uz_t i = 0; i < o->rows; i++ )
            {
                const f3_t* vi = o->data + i * o->stride;
                for( uz_t j = 0; j <= i ; j++ )
                {
                    const f3_t* vj = o->data + j * o->stride;
                    f3_t sum = 0;
                    for( uz_t k = 0; k < o->cols; k++ ) sum += vi[ k ] * vj[ k ];
                    res->data[ i * res->stride + j ] = sum;
                    res->data[ j * res->stride + i ] = sum;
                }
            }
        }
        return;
    }

    if( res == o )
    {
        bmath_vf3_s row;
        bmath_vf3_s_init( &row );
        bmath_vf3_s_set_size( &row, o->cols );
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const f3_t* voi =   o->data + i *   o->stride;
            bmath_vf3_s_zro( &row );
            for( uz_t j = 0; j < op->rows; j++ )
            {
                const f3_t* vpj = op->data + j * op->stride;
                for( uz_t k = 0; k < o->cols; k++ ) row.data[ j ] += voi[ k ] * vpj[ k ];
            }

            f3_t* vri = res->data + i * res->stride;
            for( uz_t k = 0; k < row.size; k++ ) vri[ k ] = row.data[ k ];
        }
        bmath_vf3_s_down( &row );
    }
    else if( res == op )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_mul_htp( o, op, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
    }
    else
    {
        bmath_mf3_s_zro( res );
        for( uz_t i = 0; i < o->rows; i++ )
        {
                  f3_t* vri = res->data + i * res->stride;
            const f3_t* voi =   o->data + i *   o->stride;
            for( uz_t j = 0; j < op->rows; j++ )
            {
                const f3_t* vpj = op->data + j * op->stride;
                for( uz_t k = 0; k < o->cols; k++ ) vri[ j ] += voi[ k ] * vpj[ k ];
            }
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
bl_t bmath_mf3_s_piv( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res )
{
    ASSERT( o->rows == res->cols );
    ASSERT( o->cols == res->rows );

    uz_t n = uz_min( o->rows, o->cols );
    if( n == 0 ) return true;

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

    // o = uT * a * v; o^-1 = vT * (a^-1)T * u
    bl_t success = bmath_mf3_s_svd( u, a, v );
    bmath_mf3_s_get_dag_vec( a, d );

    // diagonal elements are sorted in descending manner
    f3_t s_max = f3_max( f3_abs( d->data[ 0 ] ), f3_abs( d->data[ n - 1 ] ) );
    f3_t thr = s_max * eps;
    if( thr == 0 ) thr = 1.0;

    // pseudo invert diagonal
    for( uz_t i = 0; i < n; i++ ) d->data[ i ] = ( fabs( d->data[ i ] ) < thr ) ? 0 : 1.0 / d->data[ i ];

    for( uz_t i = 0; i < res->rows; i++ )
    {
        f3_t* ri = res->data + res->stride * i;
        f3_t* vi =   v->data +   v->stride * i;
        for( uz_t j = 0; j < res->cols; j++ )
        {
            f3_t* uj = u->data + u->stride * j;
            f3_t  r = 0;
            for( uz_t k = 0; k < n; k++ ) r += vi[ k ] * uj[ k ] * d->data[ k ];
            ri[ j ] = r;
        }
    }

    bmath_vf3_s_discard( d );
    bmath_mf3_s_discard( v );
    bmath_mf3_s_discard( u );
    bmath_mf3_s_discard( a );

    return success;
}

//---------------------------------------------------------------------------------------------------------------------

// pseudo inversion of a symmetric matrix
bl_t bmath_mf3_s_hsm_piv( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res )
{
    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size_to( res, buf );
        bl_t success = bmath_mf3_s_hsm_piv( o, eps, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return success;
    }

    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    ASSERT( o->rows == o->cols );

    uz_t n = o->rows;
    if( n == 0 ) return true;

    bmath_mf3_s* a = res; // a occupies space of res
    bmath_mf3_s* q = bmath_mf3_s_create();
    bmath_mf3_s_set_size( q, n, n );
    bmath_mf3_s_one( q );

    bmath_mf3_s_cpy( o, a );
    bl_t success = bmath_mf3_s_evd_htp( a, q );

    bmath_vf3_s* dag = bmath_vf3_s_create();
    bmath_vf3_s_set_size( dag, n );
    bmath_mf3_s_get_dag_vec( a, dag );

    // dag is sorted in descending manner
    f3_t s_max = f3_max( f3_abs( dag->data[ 0 ] ), f3_abs( dag->data[ n - 1 ] ) );
    f3_t thr = s_max * eps;
    if( thr == 0 ) thr = 1.0;

    for( uz_t i = 0; i < n; i++ ) dag->data[ i ] = ( dag->data[ i ] < thr ) ? 0 : ( 1.0 / dag->data[ i ] );

    bmath_mf3_s_htp( q, q );
    bmath_mf3_s_udu_htp( q, dag, res );

    bmath_vf3_s_discard( dag );
    bmath_mf3_s_discard( q );

    return success;
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

    uz_t n = o_sub.rows;

    for( uz_t i = 0; i < n; i++ )
    {
        f3_t sum = 0;
        f3_t* row = r_sub.data + i * r_sub.stride;
        for( uz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
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

    uz_t n = o_sub.rows;

    for( uz_t i = 0; i < n; i++ )
    {
        f3_t sum = 0;
        f3_t* row = r_sub.data + i * r_sub.stride;
        for( uz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
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

    uz_t n = o_sub.rows;

    for( uz_t i = 0; i < n; i++ )
    {
        f3_t sum = 0;
        f3_t* row = r_sub.data + i * r_sub.stride;
        for( uz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
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
    for( uz_t i = 0; i < res->size; i++ )
    {
        f3_t* v1 = o->data + i * o->stride;
        f3_t sum = 0;
        for( uz_t j = 0; j < o->cols; j++ ) sum += v1[ j ] * v2[ j ];
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

    uz_t n = av1->size;
    ASSERT( o->cols == n + 1 );
    ASSERT( o->rows == res->size );
    f3_t* vr = res->data;
    f3_t* v2 = av1->data;
    for( uz_t i = 0; i < res->size; i++ )
    {
        f3_t* v1 = o->data + i * o->stride;
        f3_t sum = 0;
        for( uz_t j = 0; j < n; j++ ) sum += v1[ j ] * v2[ j ];
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
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* vi =   o->data + i *   o->stride;
              f3_t* vr = res->data + i * res->stride;
        for( uz_t j = 0; j < o->rows; j++ )
        {
            const f3_t* vj = o->data + j * o->stride;
            f3_t sum = 0;
            for( uz_t k = 0; k < o->rows; k++ ) sum += vi[ k ] * vj[ k ] * vd[ k ];
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
        uz_t n = o->rows;
        for( uz_t i = 0; i < n; i++ )
        {
            const f3_t* voi =   o->data + i *   o->stride;
                  f3_t* vri = res->data + i * res->stride;
            for( uz_t j = 0; j < i; j++ )
            {
                const f3_t* voj =   o->data + j *   o->stride;
                      f3_t* vrj = res->data + j * res->stride;

                f3_t tmp = voi[ j ];
                vri[ j ] = voj[ i ];
                vrj[ i ] = tmp;
            }
        }

        // copy diag
        if( o != res ) for( uz_t i = 0; i < n; i++ ) res->data[ i * res->stride + i ] = o->data[ i * o->stride + i ];
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

    for( uz_t i = 0; i < res->rows; i++ )
    {
              f3_t* vr = res->data + i * res->stride;
        const f3_t* v1 = o ->data  + i;
        for( uz_t j = 0; j < res->cols; j++ ) vr[ j ] = v1[ j * o->stride ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_scl( const bmath_mf3_s* o, const f3_t* op, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    f3_t f = *op;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v1 = o ->data  + i * o ->stride;
              f3_t* vr = res->data + i * res->stride;
        for( uz_t j = 0; j < o->cols; j++ ) vr[ j ] = v1[ j ] * f;
    }
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// element-access; col-access; row-access; sub-matrix

void bmath_mf3_s_set_row_by_data( bmath_mf3_s* o, uz_t idx, const f3_t* data )
{
    ASSERT( idx < o->rows );
    bcore_u_memcpy( sizeof( f3_t ), o->data + idx * o->stride, data, o->cols );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_col_by_data( bmath_mf3_s* o, uz_t idx, const f3_t* data )
{
    ASSERT( idx < o->cols );
    f3_t* v = o ->data + idx;
    for( uz_t i = 0; i < o->rows; i++ ) v[ i * o ->stride ] = data[ i ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_dag_by_data( bmath_mf3_s* o, const f3_t* data )
{
    uz_t size = uz_min( o->rows, o->cols );
    for( uz_t i = 0; i < size; i++ ) o->data[ i * ( o ->stride + 1 ) ] = data[ i ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_row_by_vec( bmath_mf3_s* o, uz_t idx, const bmath_vf3_s* vec )
{
    ASSERT( o->cols <= vec->size );
    bmath_mf3_s_set_row_by_data( o, idx, vec->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_col_by_vec( bmath_mf3_s* o, uz_t idx, const bmath_vf3_s* vec )
{
    ASSERT( o->rows <= vec->size );
    bmath_mf3_s_set_col_by_data( o, idx, vec->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_dag_by_vec( bmath_mf3_s* o, const bmath_vf3_s* vec )
{
    ASSERT( uz_min( o->rows, o->cols ) <= vec->size );
    bmath_mf3_s_set_dag_by_data( o, vec->data );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_get_row_vec(  const bmath_mf3_s* o, uz_t idx, bmath_vf3_s* res )
{
    ASSERT( idx < o->rows );
    ASSERT( o->cols <= res->size );
    bcore_u_memcpy( sizeof( f3_t ), res->data, o->data + idx * o->stride, o->cols );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_get_col_vec(  const bmath_mf3_s* o, uz_t idx, bmath_vf3_s* res )
{
    ASSERT( idx < o->cols );
    ASSERT( o->rows <= res->size );
    const f3_t* src = o->data + idx;
    for( uz_t i = 0; i < o->rows; i++ ) res->data[ i ] = src[ i * o ->stride ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_get_dag_vec( const bmath_mf3_s* o, bmath_vf3_s* res )
{
    uz_t size = uz_min( o->rows, o->cols );
    ASSERT( size <= res->size );
    for( uz_t i = 0; i < size; i++ ) res->data[ i ] = o->data[ i * ( o->stride + 1 ) ];
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_swap_row( bmath_mf3_s* o, uz_t i, uz_t j )
{
    if( i == j ) return;
    ASSERT( i < o->rows );
    ASSERT( j < o->rows );
    f3_t* vi = o->data + i * o->stride;
    f3_t* vj = o->data + j * o->stride;
    for( uz_t k = 0; k < o->cols; k++ ) f3_t_swap( vi + k, vj + k );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_swap_col( bmath_mf3_s* o, uz_t i, uz_t j )
{
    if( i == j ) return;
    ASSERT( i < o->cols );
    ASSERT( j < o->cols );
    f3_t* vi = o->data + i;
    f3_t* vj = o->data + j;
    for( uz_t k = 0; k < o->rows; k++ ) f3_t_swap( vi + k * o->stride, vj + k * o->stride );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_f3_to_row( bmath_mf3_s* o, f3_t v, uz_t i )
{
    ASSERT( i < o->rows );
    f3_t* vi = o->data + i * o->stride;
    for( uz_t k = 0; k < o->cols; k++ ) vi[ k ] *= v;
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_f3_to_col( bmath_mf3_s* o, f3_t v, uz_t i )
{
    ASSERT( i < o->cols );
    f3_t* vi = o->data + i;
    for( uz_t k = 0; k < o->rows; k++ ) vi[ k * o->stride ] *= v;
}

//---------------------------------------------------------------------------------------------------------------------

bmath_mf3_s bmath_mf3_s_get_weak_sub_mat( const bmath_mf3_s* o, uz_t row, uz_t col, uz_t rows, uz_t cols )
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

bmath_vf3_s bmath_mf3_s_get_row_weak_vec( const bmath_mf3_s* o, uz_t idx )
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

    for( uz_t i = 0; i < o->rows; i++ )
    {
              f3_t* vli = res->data + i * res->stride;
        const f3_t* voi =   o->data + i * o->stride;
        for( uz_t j = 0; j <= i; j++ )
        {
            f3_t* vlj = res->data + j * res->stride;
            f3_t acc = voi[ j ];
            for( uz_t k = 0; k < j; k++ ) acc -= vli[ k ] * vlj[ k ];
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
    uz_t n = o->cols;
    uz_t stride = res->stride;

    bmath_mf3_s_cpy( o, res );
    for( uz_t i = 0; i < n; i++ )
    {
        f3_t* vi = res->data + i * stride;
        for( uz_t j = 0; j < i; j++ )
        {
            f3_t* vj = res->data + j;
            f3_t sum = 0;
            for( uz_t k = 0; k < j; k++ ) sum += vi[ k ] * vj[ k * stride ];
            vi[ j ] = ( vj[ j * stride ] != 0 ) ? ( ( vi[ j ] - sum ) / vj[ j * stride ] ) : 0;
        }

        for( uz_t j = i; j < n; j++ )
        {
            f3_t* vj = res->data + j;
            f3_t sum = 0;
            for( uz_t k = 0; k < i; k++ ) sum += vi[ k ] * vj[ k * stride ];
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

    uz_t n = o->rows;

    // diagonal elements
    for( uz_t i = 0; i < n; i++ )
    {
        f3_t v = o->data[ i * o->stride + i ];
        res->data[ i * res->stride + i ] = ( v != 0 ) ? 1.0 / v : 0;
    }

    // upper off-diagonal elements
    for( uz_t i = 0; i < n; i++ )
    {
        f3_t* vri = res->data + i * res->stride;
        for( uz_t j = i + 1; j < n; j++ )
        {
            const f3_t* voj =   o->data + j *   o->stride;
                  f3_t* vrj = res->data + j * res->stride;
            f3_t sum = 0;
            for( uz_t k = i; k < j; k++ ) sum += voj[ k ] * vri[ k ];
            vri[ j ] = -vrj[ j ] * sum;
            vrj[ i ] = 0;
        }
    }

    // zero lower off-diagonal elements; (this part must be kept to last for the in-place algorithm)
    for( uz_t i = 0; i < n; i++ )
    {
        f3_t* vri = res->data + i * res->stride;
        for( uz_t j = 0; j < i; j++ ) vri[ j ] = 0;
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

/**********************************************************************************************************************/
/// Triangular decompositions, operations and solvers

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ltr_mul_htp( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    uz_t n = o->rows;

    // compute off diagonal elements; store in upper triangle of res
    for( uz_t i = 0; i < n; i++ )
    {
        const f3_t* voi = o->data + i * o->stride;
        for( uz_t j = 0; j < i; j++ )
        {
            const f3_t* voj =   o->data + j *   o->stride;
                  f3_t* vrj = res->data + j * res->stride;
            f3_t sum = 0;
            for( uz_t k = 0; k <= j; k++ ) sum += voi[ k ] * voj[ k ];
            vrj[ i ] = sum;
        }
    }

    // diagonal elements in reverse order
    for( uz_t i = n - 1; i < n; i-- )
    {
        const f3_t* voi =   o->data + i *   o->stride;
              f3_t* vri = res->data + i * res->stride;

        f3_t sum = 0;
        for( uz_t k = 0; k <= i; k++ ) sum += voi[ k ] * voi[ k ];
        vri[ i ] = sum;
    }

    // copy lower off-diagonal from upper off-diagonal
    for( uz_t i = 0; i < n; i++ )
    {
        f3_t* vri = res->data + i * res->stride;
        for( uz_t j = 0; j < i; j++ ) vri[ j ] = res->data[ j * res->stride + i ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_utr_mul_htp( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    uz_t n = o->rows;

    // compute off diagonal elements; store in lower triangle of res
    for( uz_t i = 0; i < n; i++ )
    {
        const f3_t* voi = o->data + i * o->stride;
        for( uz_t j = i + 1; j < n; j++ )
        {
            const f3_t* voj =   o->data + j *   o->stride;
                  f3_t* vrj = res->data + j * res->stride;
            f3_t sum = 0;
            for( uz_t k = j; k < n; k++ ) sum += voi[ k ] * voj[ k ];
            vrj[ i ] = sum;
        }
    }

    // diagonal elements in ascending order
    for( uz_t i = 0; i < n; i++ )
    {
        const f3_t* voi =   o->data + i *   o->stride;
              f3_t* vri = res->data + i * res->stride;

        f3_t sum = 0;
        for( uz_t k = i; k < n; k++ ) sum += f3_sqr( voi[ k ] );
        vri[ i ] = sum;
    }

    // copy lower off-diagonal to upper off-diagonal
    for( uz_t i = 0; i < n; i++ )
    {
        f3_t* vri = res->data + i * res->stride;
        for( uz_t j = 0; j < i; j++ ) res->data[ j * res->stride + i ] = vri[ j ];
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ltr_mul_vec_( const bmath_mf3_s* o, const f3_t* op, f3_t* res )
{
    // Algorithm works in-place for op == res;
    for( uz_t i = o->rows - 1; i < o->rows; i-- )
    {
        f3_t* v1 = o->data + i * o->stride;
        f3_t sum = 0;
        for( uz_t j = 0; j <= i; j++ ) sum += v1[ j ] * op[ j ];
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
    for( uz_t i = o->rows - 1; i < o->rows; i-- )
    {
        f3_t* v1 = o->data + i * o->stride;
        f3_t sum = 0;
        for( uz_t j = 0; j < i; j++ ) sum += v1[ j ] * op[ j ];
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
    for( uz_t i = 0; i < o->rows; i++ )
    {
        f3_t* v1 = o->data + i * o->stride;
        f3_t sum = 0;
        for( uz_t j = i; j < o->cols; j++ ) sum += v1[ j ] * op[ j ];
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
    for( uz_t i = 0; i < o->rows; i++ )
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
    for( uz_t i = 0; i < o->rows; i++ )
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
    for( uz_t i = 0; i < o->rows; i++ )
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
    for( uz_t i = 0; i < o->rows; i++ )
    {
        bmath_mf3_s_luc_mul_vec_( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ltr_solve_vec_( const bmath_mf3_s* o, const f3_t* op, f3_t* res )
{
    // Algorithm works in-place for op == res;
    if( op != res ) bcore_u_memcpy( sizeof( f3_t ), res, op, o->rows );
    uz_t n = o->rows;
    for( uz_t i = 0; i < n; i++ )
    {
        const f3_t* voi = o->data + i * o->stride;
        f3_t sum = 0;
        for( uz_t j = 0; j < i; j++ ) sum += voi[ j ] * res[ j ];
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
    uz_t n = o->rows;
    for( uz_t i = n - 1; i < n; i-- )
    {
        const f3_t* voi = o->data + i * o->stride;
        f3_t sum = 0;
        for( uz_t j = i + 1; j < n; j++ ) sum += voi[ j ] * res[ j ];
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
    uz_t n = o->rows;
    for( uz_t i = 0; i < n; i++ )
    {
        const f3_t* voi = o->data + i * o->stride;
        f3_t sum = 0;
        for( uz_t j = 0; j < i; j++ ) sum += voi[ j ] * res[ j ];
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
    for( uz_t i = 0; i < o->rows; i++ )
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
    for( uz_t i = 0; i < o->rows; i++ )
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
    for( uz_t i = 0; i < o->rows; i++ )
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
    for( uz_t i = 0; i < o->rows; i++ )
    {
        bmath_mf3_s_luc_solve_vec_( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Other decompositions

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_hsm_decompose_trd_htp( bmath_mf3_s* a, bmath_mf3_s* v )
{
    ASSERT( bmath_mf3_s_is_hsm( a ) );
    if( v )
    {
        ASSERT( v != a );
        ASSERT( v->cols == a->rows );
        ASSERT( v->rows == a->rows );
    }

    bmath_grt_f3_s gr;

    for( uz_t j = 0; j < a->rows; j++ )
    {
        for( uz_t k = a->rows - 2; k > j; k-- )
        {
            f3_t* ak;

            // zero upper row
            ak = a->data + k;
            uz_t jstride = j * a->stride;
            bmath_grt_f3_s_init_and_annihilate_b( &gr, ak + jstride, ak + jstride + 1 );
            bmath_grt_f3_s_col_rotate( &gr, ak, ak + 1, a->stride, j + 1, a->rows );

            // transposed operation
            ak = a->data + k * a->stride;
            bmath_grt_f3_s_annihilate_b( &gr, ak + j, ak + a->stride + j );
            bmath_grt_f3_s_row_rotate( &gr, ak, ak + a->stride, j + 1, a->cols );

            // rotation matrix
            if( v ) bmath_grt_f3_s_row_rotate( &gr, v->data + k * v->stride, v->data + ( k + 1 ) * v->stride, 0, v->cols );
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_qrd( bmath_mf3_s* u, bmath_mf3_s* a )
{
    if( a->rows <= 1 ) return; // nothing to do

    if( u )
    {
        ASSERT( u != a );
        ASSERT( u->rows == a->rows );
        ASSERT( u->cols == a->rows /*full*/ || u->cols == a->cols /*thin*/  ); // u may be full or thin (nothing in-between)
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
        bmath_mf3_s_one( u );
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

void bmath_mf3_s_ubd_to_lbd( bmath_mf3_s* a, bmath_mf3_s* v )
{
    uz_t n = uz_min( a->rows, a->cols );

    if( n <= 1 ) return; // nothing to do

    if( v ) ASSERT( v->cols >= n );

    bmath_arr_grt_f3_s grv = bmath_arr_grt_f3_of_size( n );

    for( uz_t j = 0; j < n - 1; j++ )
    {
        f3_t* aj = a->data + j * ( a->stride + 1 );
        bmath_grt_f3_s_init_and_annihilate_b( &grv.data[ j ], aj, aj + 1 );
        bmath_grt_f3_s_rotate( &grv.data[ j ], aj + a->stride, aj + a->stride + 1 );
    }

    if( v ) bmath_mf3_s_sweep_acol_rotate_fwd( v, 0, n - 1, &grv, 0, v->rows );

    bmath_arr_grt_f3_s_down( &grv );
}

//---------------------------------------------------------------------------------------------------------------------

// v transposed
void bmath_mf3_s_ubd_to_lbd_htp( bmath_mf3_s* a, bmath_mf3_s* v )
{
    uz_t n = uz_min( a->rows, a->cols );

    if( n <= 1 ) return; // nothing to do

    if( v ) ASSERT( v->rows >= n );

    bmath_grt_f3_s gr;

    for( uz_t j = 0; j < n - 1; j++ )
    {
        f3_t* aj = a->data + j * ( a->stride + 1 );
        bmath_grt_f3_s_init_and_annihilate_b( &gr, aj, aj + 1 );
        bmath_grt_f3_s_rotate( &gr, aj + a->stride, aj + a->stride + 1 );
        if( v ) bmath_mf3_s_arow_rotate( v, j, &gr, 0, v->cols );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_lbd_to_ubd( bmath_mf3_s* u, bmath_mf3_s* a )
{
    uz_t n = uz_min( a->rows, a->cols );

    if( n <= 1 ) return; // nothing to do

    if( u ) ASSERT( u->cols >= n );

    bmath_arr_grt_f3_s gru = bmath_arr_grt_f3_of_size( n );

    for( uz_t j = 0; j < n - 1; j++ )
    {
        f3_t* aj = a->data + j * ( a->stride + 1 );
        bmath_grt_f3_s_init_and_annihilate_b( &gru.data[ j ], aj, aj + a->stride );
        bmath_grt_f3_s_rotate( &gru.data[ j ], aj + 1, aj + a->stride + 1 );
    }

    if( u ) bmath_mf3_s_sweep_acol_rotate_fwd( u, 0, n - 1, &gru, 0, u->rows );

    bmath_arr_grt_f3_s_down( &gru );
}

//---------------------------------------------------------------------------------------------------------------------

// u transposed
void bmath_mf3_s_lbd_to_ubd_htp( bmath_mf3_s* u, bmath_mf3_s* a )
{
    uz_t n = uz_min( a->rows, a->cols );

    if( n <= 1 ) return; // nothing to do

    if( u ) ASSERT( u->rows >= n );

    bmath_grt_f3_s gr;

    for( uz_t j = 0; j < n - 1; j++ )
    {
        f3_t* aj = a->data + j * ( a->stride + 1 );
        bmath_grt_f3_s_init_and_annihilate_b( &gr, aj, aj + a->stride );
        bmath_grt_f3_s_rotate( &gr, aj + 1, aj + a->stride + 1 );
        if( u ) bmath_mf3_s_arow_rotate( u, j, &gr, 0, u->cols );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_ubd( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v )
{
    if( a->rows < a->cols )
    {
        bmath_mf3_s_lbd( u, a, v );
        bmath_mf3_s_lbd_to_ubd( u, a );
        return;
    }

    ASSERT( a->rows >= a->cols );

    /// at this point: a->rows >= a->cols;

    if( a->rows <= 1 ) return; // nothing to do

    if( u )
    {
        ASSERT( u != a );
        ASSERT( u->rows == a->rows );
        ASSERT( u->cols == a->rows /*full*/ || u->cols == a->cols /*thin*/  ); // u may be full or thin (nothing in-between)
    }

    if( v )
    {
        ASSERT( v->cols == a->cols );
        ASSERT( v->rows == a->cols );
        ASSERT( v != a );
        ASSERT( v != u );
    }

    bmath_arr_grt_f3_s grv = bmath_arr_grt_f3_of_size( a->cols );
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

        // zero upper row
        for( uz_t l = a->cols - 1; l > j + 1; l-- )
        {
            bmath_grt_f3_s_init_and_annihilate_b( &gr, &a->data[ j * a->stride + j + 1 ], &a->data[ j * a->stride + l ] );
            if( v ) a->data[ j * a->stride + l ] = bmath_grt_f3_s_rho( &gr );
            grv.data[ l - 1 ] = gr;
        }

        bmath_mf3_s_sweep_dcol_rotate_rev( a, j + 1, a->cols - 1, &grv, j + 1, a->rows );
    }

    if( v ) // reverse construction of v
    {
        bmath_mf3_s_one( v );
        for( uz_t j = a->cols - 1; j < a->cols; j-- )
        {
            for( uz_t k = j + 1; k < a->cols - 1; k++ )
            {
                f3_t rho = 0;
                f3_t_swap( &a->data[ j * a->stride + k + 1 ], &rho );
                bmath_grt_f3_s_init_from_rho( &gr, -rho );
                bmath_mf3_s_drow_rotate( v, j + 1, k + 1, &gr, j, v->cols );
            }
        }
    }

    if( u ) // reverse construction of u
    {
        bmath_mf3_s_one( u );
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

    bmath_arr_grt_f3_s_down( &grv );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_lbd( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v )
{
    if( a->cols < a->rows )
    {
        bmath_mf3_s_ubd( u, a, v );
        bmath_mf3_s_ubd_to_lbd( a, v );
        return;
    }

    /// at this point: a->cols >= a->rows;

    if( a->cols <= 1 ) return; // nothing to do

    if( u )
    {
        ASSERT( u->cols == a->rows );
        ASSERT( u->rows == a->rows );
        ASSERT( u != a );
        ASSERT( u != v );
    }

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

        // zero lower column
        for( uz_t l = a->rows - 1; l > j + 1; l-- )
        {
            bmath_grt_f3_s_init_and_annihilate_b( &gr, &a->data[ ( j + 1 ) * a->stride + j ], &a->data[ l * a->stride + j ] );
            if( u ) a->data[ l * a->stride + j ] = bmath_grt_f3_s_rho( &gr );
            bmath_mf3_s_drow_rotate( a, j + 1, l, &gr, j + 1, a->cols );
        }
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

    if( u ) // reverse construction of u
    {
        bmath_mf3_s_one( u );
        for( uz_t j = a->rows - 2; j < a->rows; j-- )
        {
            for( uz_t k = j + 1; k < a->rows - 1; k++ )
            {
                f3_t rho = 0;
                f3_t_swap( &a->data[ j + a->stride * ( k + 1 ) ], &rho );
                bmath_grt_f3_s_init_from_rho( &gr, -rho );
                bmath_mf3_s_drow_rotate( u, j + 1, k + 1, &gr, j, u->cols );
            }
        }
    }

    bmath_arr_grt_f3_s_down( &grv );
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Covariance

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_set_covariance_on_section_fast( bmath_mf3_s* o, bmath_arr_vf3_s* arr_vec, uz_t start, uz_t end )
{
    end = end < arr_vec->size ? end : arr_vec->size;
    start = start > end ? end : start;

    bmath_mf3_s_zro( o );
    if( start == end ) return;
    uz_t n = arr_vec->data[ start ].size;
    ASSERT( o->rows == n );
    ASSERT( o->cols == n );

    bmath_vf3_s* avg = bmath_vf3_s_create_set_size( n );
    bmath_vf3_s* vec = bmath_vf3_s_create_set_size( n );

    bmath_arr_vf3_s_on_section_get_avg( arr_vec, start, end, avg );

    for( uz_t i = start; i < end; i++ )
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

void bmath_mf3_s_set_covariance_on_section_sprc( bmath_mf3_s* o, bmath_arr_vf3_s* arr_vec, uz_t start, uz_t end )
{
    end = end < arr_vec->size ? end : arr_vec->size;
    start = start > end ? end : start;

    bmath_mf3_s_zro( o );
    if( start == end ) return;
    uz_t n = arr_vec->data[ start ].size;
    ASSERT( o->rows == n );
    ASSERT( o->cols == n );

    bmath_vf3_s* avg = bmath_vf3_s_create_set_size( n );
    bmath_arr_vf3_s_on_section_get_avg_sprc( arr_vec, start, end, avg );

    for( uz_t i = 0; i < n; i++ )
    {
        for( uz_t j = 0; j <= i; j++ )
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

/**********************************************************************************************************************/
/// Givens Rotations

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_sweep_arow_rotate_fwd( bmath_mf3_s* o, uz_t row_start, uz_t row_end, const bmath_arr_grt_f3_s* grt, uz_t col_start, uz_t col_end )
{
    assert( grt->size >= row_end - 1 );
    for( uz_t i = row_start; i < row_end; i++ ) bmath_mf3_s_arow_rotate( o, i, &grt->data[ i ], col_start, col_end );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_sweep_arow_rotate_rev( bmath_mf3_s* o, uz_t row_start, uz_t row_end, const bmath_arr_grt_f3_s* grt, uz_t col_start, uz_t col_end )
{
    assert( grt->size >= row_end - 1 );
    for( uz_t i = row_end; i > row_start; i-- ) bmath_mf3_s_arow_rotate( o, i - 1, &grt->data[ i - 1 ], col_start, col_end );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_sweep_acol_rotate_fwd( bmath_mf3_s* o, uz_t col_start, uz_t col_end, const bmath_arr_grt_f3_s* grt, uz_t row_start, uz_t row_end )
{
    assert( grt->size >= col_end - 1 );
    if( bmath_arr_grt_f3_s_density( grt, col_start, col_end ) < 0.0625 )
    {
        // sparse rotations: rotate columns individually
        for( uz_t i = col_start; i < col_end; i++ ) bmath_mf3_s_acol_rotate( o, i, &grt->data[ i ], row_start, row_end );
    }
    else
    {
        // dense rotations: use cache efficient row-sweeps
        for( uz_t i = row_start; i < row_end; i++ ) bmath_mf3_s_arow_swipe_fwd( o, i, grt, col_start, col_end );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_sweep_acol_rotate_rev( bmath_mf3_s* o, uz_t col_start, uz_t col_end, const bmath_arr_grt_f3_s* grt, uz_t row_start, uz_t row_end )
{
    assert( grt->size >= col_end - 1 );
    if( bmath_arr_grt_f3_s_density( grt, col_start, col_end ) < 0.0625 )
    {
        // sparse rotations: rotate columns individually
        for( uz_t i = col_end; i > col_start; i-- ) bmath_mf3_s_acol_rotate( o, i - 1, &grt->data[ i - 1 ], row_start, row_end );
    }
    else
    {
        // dense rotations: use cache efficient row-sweeps
        for( uz_t i = row_start; i < row_end; i++ ) bmath_mf3_s_arow_swipe_rev( o, i, grt, col_start, col_end );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_sweep_dcol_rotate_rev( bmath_mf3_s* o, uz_t col_start, uz_t col_end, const bmath_arr_grt_f3_s* grt, uz_t row_start, uz_t row_end )
{
    assert( grt->size >= col_end - 1 );
    if( bmath_arr_grt_f3_s_density( grt, col_start, col_end ) < 0.0625 )
    {
        // sparse rotations: rotate columns individually
        for( uz_t i = col_end; i > col_start; i-- ) bmath_mf3_s_dcol_rotate( o, col_start, i, &grt->data[ i - 1 ], row_start, row_end );
    }
    else
    {
        // dense rotations: use cache efficient row-sweeps
        for( uz_t i = row_start; i < row_end; i++ ) bmath_mf3_s_drow_swipe_rev( o, i, grt, col_start, col_end );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_to_stdout( const bmath_mf3_s* o )
{
    printf( "(%zu x %zu)\n", o->rows, o->cols );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ )
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

        uz_t n = 100;
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
        uz_t n = 100;
        u2_t rval = 1234;
        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );
        bmath_mf3_s_mul_htp( m1, m1, m2 );
        ASSERT( bmath_mf3_s_is_near_hsm( m2, 1E-8 ) );
    }


    // general inversion
    {
        uz_t n = 100;
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
        for( uz_t test_id = 0; test_id < 3; test_id++ )
        {
            uz_t n = ( test_id == 0 ) ? 20 : 10;
            uz_t m = ( test_id == 2 ) ? 20 : 10;
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
        uz_t n = 100;
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
        uz_t n = 100;

        bmath_mf3_s_set_size( m1, n, n );
        u2_t rval = 1236;
        bmath_mf3_s_fill_random( m1, -1, 1, &rval );
        bmath_mf3_s_mul_htp( m1, m1, m1 );

        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_size_to( m1, m3 );
        bmath_mf3_s_set_size_to( m1, m4 );
        bmath_mf3_s_cpy( m1, m2 );
        bmath_mf3_s_hsm_decompose_trd_htp( m2, NULL );
        ASSERT( bmath_mf3_s_is_trd( m2 ) );
        bmath_mf3_s_cpy( m1, m2 );
        bmath_mf3_s_one( m3 );
        bmath_mf3_s_hsm_decompose_trd_htp( m2, m3 );
        ASSERT( bmath_mf3_s_is_trd( m2 ) );

        ASSERT( bmath_mf3_s_is_near_uni( m3, 1E-8 ) );

        bmath_mf3_s_mul( m2, m3, m4 );
        bmath_mf3_s_htp( m3, m3 );
        bmath_mf3_s_mul( m3, m4, m4 );

        ASSERT( bmath_mf3_s_is_near_equ( m1, m4, 1E-8 ) );
    }

    // covariance
    {
        uz_t size = 1000;
        uz_t n    = 10;
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
        uz_t n = 20;
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
        uz_t n = 20;
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
        uz_t n = 20;
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

static void eval_test( void )
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_matrix_eval_s, eval );
    BCORE_LIFE_CREATE( bmath_arr_matrix_eval_s, arr_eval );

    eval->density = 1.0;
    eval->full = false;
    eval->rows = 10; eval->cols = 30; bmath_arr_matrix_eval_s_push( arr_eval, eval );
    eval->rows = 20; eval->cols = 20; bmath_arr_matrix_eval_s_push( arr_eval, eval );
    eval->rows = 30; eval->cols = 10; bmath_arr_matrix_eval_s_push( arr_eval, eval );
    eval->full = true;
    eval->rows = 10; eval->cols = 30; bmath_arr_matrix_eval_s_push( arr_eval, eval );
    eval->rows = 20; eval->cols = 20; bmath_arr_matrix_eval_s_push( arr_eval, eval );
    eval->rows = 30; eval->cols = 10; bmath_arr_matrix_eval_s_push( arr_eval, eval );

    bmath_arr_matrix_eval_s_run_fp( arr_eval, typeof( "bmath_fp_qrd" ), ( fp_t )bmath_mf3_s_qrd, NULL );
    bmath_arr_matrix_eval_s_run_fp( arr_eval, typeof( "bmath_fp_lqd" ), ( fp_t )bmath_mf3_s_lqd, NULL );
    bmath_arr_matrix_eval_s_run_fp( arr_eval, typeof( "bmath_fp_ubd" ), ( fp_t )bmath_mf3_s_ubd, NULL );
    bmath_arr_matrix_eval_s_run_fp( arr_eval, typeof( "bmath_fp_lbd" ), ( fp_t )bmath_mf3_s_lbd, NULL );
    bmath_arr_matrix_eval_s_run_fp( arr_eval, typeof( "bmath_fp_svd" ), ( fp_t )bmath_mf3_s_svd, NULL );

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
            eval_test();
            bmath_mf3_s_evd_selftest();
            return NULL;
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

