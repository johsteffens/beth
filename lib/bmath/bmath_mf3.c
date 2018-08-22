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

#include "bmath_mf3.h"
#include "bmath_spect_matrix.h"
#include "bmath_fourier.h"
#include "bmath_grt.h"
#include "bmath_mf3_eval.h"
#include "bmath_simd.h"

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

void bmath_mf3_s_set_random( bmath_mf3_s* o, bl_t hsm, bl_t pdf, uz_t rd, f3_t density, f3_t min, f3_t max, u2_t* p_rval )
{
    u2_t rval = p_rval ? *p_rval : 12345;
    f3_t range = max - min;
    if( pdf ) ASSERT( hsm );
    if( hsm ) ASSERT( o->rows == o->cols );

    if( rd == 0 ) // full rank
    {
        if( pdf )
        {
            bmath_mf3_s_set_random( o, false, false, 0, density, min, max, &rval );
            bmath_mf3_s_mul_htp( o, o, o );
        }
        else if( hsm )
        {
            for( uz_t i = 0; i < o->rows; i++ )
            {
                f3_t* vi = o->data + i * o->stride;
                for( uz_t j = 0; j <= i; j++ )
                {
                    f3_t* vj = o->data + j * o->stride;
                    if( f3_xsg1_pos( &rval ) < density )
                    {
                        vi[ j ] = ( range * f3_xsg1_pos( &rval ) ) + min;
                        vj[ i ] = vi[ j ];
                    }
                    else
                    {
                        vi[ j ] = 0;
                        vj[ i ] = 0;
                    }
                }
            }
        }
        else
        {
            for( uz_t i = 0; i < o->rows; i++ )
            {
                f3_t* v = o->data + i * o->stride;
                for( uz_t j = 0; j < o->cols; j++ )
                {
                    if( f3_xsg1_pos( &rval ) < density )
                    {
                        v[ j ] = ( range * f3_xsg1_pos( &rval ) ) + min;
                    }
                    else
                    {
                        v[ j ] = 0;
                    }
                }
            }
        }
    }
    else if( rd < uz_min( o->cols, o->rows ) ) // rank is uz_min( o->cols, o->rows ) - rd
    {
        uz_t n = uz_min( o->cols, o->rows );
        uz_t rank = n - rd;
        if( hsm )
        {
            bmath_mf3_s* m1 = bmath_mf3_s_create();
            bmath_vf3_s* v1 = bmath_vf3_s_create();
            bmath_mf3_s_set_size( m1, o->rows, rank );
            bmath_mf3_s_set_random( m1, false, false, 0, density, min, max, &rval );

            bmath_vf3_s_set_size( v1, rank );
            if( pdf )
            {
                bmath_vf3_s_set_random( v1, density, 0.0, 1.0, &rval );
            }
            else
            {
                bmath_vf3_s_set_random( v1, density, min, max, &rval );
            }

            bmath_mf3_s_udu_htp( m1, v1, o );
            bmath_mf3_s_discard( m1 );
            bmath_vf3_s_discard( v1 );
        }
        else
        {
            bmath_mf3_s* m1 = bmath_mf3_s_create();
            bmath_mf3_s* m2 = bmath_mf3_s_create();
            bmath_mf3_s_set_size( m1, o->rows, rank );
            bmath_mf3_s_set_size( m2, o->cols, rank );
            bmath_mf3_s_set_random( m1, false, false, 0, density, min, max, &rval );
            bmath_mf3_s_set_random( m2, false, false, 0, density, min, max, &rval );
            bmath_mf3_s_mul_htp( m1, m2, o );
            bmath_mf3_s_discard( m1 );
            bmath_mf3_s_discard( m2 );
        }
    }

    if( p_rval ) *p_rval = rval;
}

//---------------------------------------------------------------------------------------------------------------------

bmath_mf3_s* bmath_mf3_s_create_set_size( uz_t rows, uz_t cols )
{
    bmath_mf3_s* o = bmath_mf3_s_create();
    bmath_mf3_s_set_size( o, rows, cols );
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
    if( res == o )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_mul( o, op, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    // res == op allowed at this point

    ASSERT(  o->cols ==  op->rows );
    ASSERT(  o->rows == res->rows );
    ASSERT( op->cols == res->cols );

    bmath_vf3_s v;
    bmath_vf3_s_init( &v );
    bmath_vf3_s_set_size( &v, op->rows );
    for( uz_t j = 0; j < op->cols; j++ )
    {
        for( uz_t i = 0; i < op->rows; i++ ) v.data[ i ] = op->data[ i * op->stride + j ];
        for( uz_t i = 0; i <  o->rows; i++ )
        {
            res->data[ i * res->stride + j ] = bmath_simd_f3_mul_vec( o->data + i * o->stride, v.data, v.size );
        }
    }

    bmath_vf3_s_down( &v );
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
                    f3_t sum = bmath_simd_f3_mul_vec( vi, o->data + j * o->stride, o->cols );
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
            const f3_t* voi = o->data + i *   o->stride;
            bmath_vf3_s_zro( &row );
            for( uz_t j = 0; j < op->rows; j++ )
            {
                row.data[ j ] = bmath_simd_f3_mul_vec( voi, op->data + j * op->stride, o->cols );
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
                vri[ j ] = bmath_simd_f3_mul_vec( voi, op->data + j * op->stride, o->cols );
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_inv_htp_luc( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    bmath_mf3_s* luc = bmath_mf3_s_create();
    bmath_mf3_s_set_size_to( o, luc );

    bl_t success = bmath_mf3_s_decompose_luc( o, luc );
    bmath_mf3_s_one( res );
    bmath_mf3_s_luc_solve_htp_htp( luc, res, res );
    bmath_mf3_s_discard( luc );
    return success;
}

bl_t bmath_mf3_s_inv_htp( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    return bmath_mf3_s_inv_htp_luc( o, res );
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_inv( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    bl_t success = bmath_mf3_s_inv_htp( o, res );
    bmath_mf3_s_htp( res, res );
    return success;
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
    bmath_mf3_s_set_size( u, o->rows, n );
    bmath_mf3_s_set_size( v, o->cols, n );
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
            ri[ j ] = bmath_simd_f3_mul_vec3( vi, uj, d->data, n );
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

    for( uz_t i = 0; i < n; i++ ) dag->data[ i ] = ( f3_abs( dag->data[ i ] ) < thr ) ? 0 : ( 1.0 / dag->data[ i ] );

    bmath_mf3_s_htp( q, q );
    bmath_mf3_s_udu_htp( q, dag, res );

    bmath_vf3_s_discard( dag );
    bmath_mf3_s_discard( q );

    return success;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_inv_av1( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size_to( res, buf );
        bl_t success = bmath_mf3_s_inv_av1( o, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return success;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );

    bmath_mf3_s o_sub = bmath_mf3_s_get_weak_sub_mat(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mf3_s r_sub = bmath_mf3_s_get_weak_sub_mat( res, 0, 0, res->rows, res->cols - 1 );

    bl_t success = bmath_mf3_s_inv( &o_sub, &r_sub );

    uz_t n = o_sub.rows;

    for( uz_t i = 0; i < n; i++ )
    {
        f3_t sum = 0;
        f3_t* row = r_sub.data + i * r_sub.stride;
        for( uz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
        res->data[ i * res->stride + n ] = -sum;
    }

    return success;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_pdf_inv_av1( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size_to( res, buf );
        bl_t success = bmath_mf3_s_pdf_inv_av1( o, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return success;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );

    bmath_mf3_s o_sub = bmath_mf3_s_get_weak_sub_mat(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mf3_s r_sub = bmath_mf3_s_get_weak_sub_mat( res, 0, 0, res->rows, res->cols - 1 );

    bl_t success = bmath_mf3_s_pdf_inv( &o_sub, &r_sub );

    uz_t n = o_sub.rows;

    for( uz_t i = 0; i < n; i++ )
    {
        f3_t sum = 0;
        f3_t* row = r_sub.data + i * r_sub.stride;
        for( uz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
        res->data[ i * res->stride + n ] = -sum;
    }
    return success;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_hsm_piv_av1( const bmath_mf3_s* o, f3_t eps, bmath_mf3_s* res )
{
    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size_to( res, buf );
        bl_t success = bmath_mf3_s_hsm_piv_av1( o, eps, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return success;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );

    bmath_mf3_s o_sub = bmath_mf3_s_get_weak_sub_mat(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mf3_s r_sub = bmath_mf3_s_get_weak_sub_mat( res, 0, 0, res->rows, res->cols - 1 );

    bl_t success = bmath_mf3_s_hsm_piv( &o_sub, eps, &r_sub );

    uz_t n = o_sub.rows;

    for( uz_t i = 0; i < n; i++ )
    {
        f3_t sum = 0;
        f3_t* row = r_sub.data + i * r_sub.stride;
        for( uz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
        res->data[ i * res->stride + n ] = -sum;
    }
    return success;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_div( const bmath_mf3_s* o, const bmath_mf3_s* op, bmath_mf3_s* res )
{
    bl_t success = bmath_mf3_s_inv( op, res );
    bmath_mf3_s_mul( o, res, res );
    return success;
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
        vr[ i ] = bmath_simd_f3_mul_vec( o->data + i * o->stride, v2, o->cols );
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
        vr[ i ] = bmath_simd_f3_mul_vec( v1, v2, n ) + v1[ n ];
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
            vr[ j ] = bmath_simd_f3_mul_vec3( vi, o->data + j * o->stride, vd, o->rows );
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

void bmath_mf3_s_pmt_mul( const bmath_mf3_s* o, const bmath_pmt_s* p, bmath_mf3_s* res )
{
    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_pmt_mul( o, p, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->rows == p->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );

    for( uz_t i = 0; i < p->size; i++ )
    {
        uz_t idx = p->data[ i ];
        ASSERT( idx < o->rows );
        bcore_u_memcpy( sizeof( f3_t ), res->data + idx * res->stride, o->data + i * o->stride, o->cols );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_pmt_htp_mul( const bmath_mf3_s* o, const bmath_pmt_s* p, bmath_mf3_s* res )
{
    if( o == res )
    {
        bmath_mf3_s* buf = bmath_mf3_s_create();
        bmath_mf3_s_set_size( buf, res->rows, res->cols );
        bmath_mf3_s_pmt_htp_mul( o, p, buf );
        bmath_mf3_s_cpy( buf, res );
        bmath_mf3_s_discard( buf );
        return;
    }

    ASSERT( o->rows == p->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );

    for( uz_t i = 0; i < p->size; i++ )
    {
        uz_t idx = p->data[ i ];
        ASSERT( idx < o->rows );
        bcore_u_memcpy( sizeof( f3_t ), res->data + i * res->stride, o->data + idx * o->stride, o->cols );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_pmt( const bmath_mf3_s* o, const bmath_pmt_s* p, bmath_mf3_s* res )
{
    ASSERT( o->cols == p->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );

    for( uz_t i = 0; i < p->size; i++ ) ASSERT( p->data[ i ] < o->cols );

    if( o == res )
    {
        bmath_vf3_s* vec = bmath_vf3_s_create_set_size( o->cols );
        f3_t* v = vec->data;
        for( uz_t i = 0; i < o->rows; i++ )
        {
            f3_t* ri = res->data + i * res->stride;
            for( uz_t j = 0; j < res->cols; j++ ) v[ j ] = ri[ p->data[ j ] ];
            bcore_u_memcpy( sizeof( f3_t ), ri, v, res->cols );
        }
        bmath_vf3_s_discard( vec );
    }
    else
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            f3_t* oi =   o->data + i *   o->stride;
            f3_t* ri = res->data + i * res->stride;
            for( uz_t j = 0; j < o->cols; j++ ) ri[ j ] = oi[ p->data[ j ] ];
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_mul_pmt_htp( const bmath_mf3_s* o, const bmath_pmt_s* p, bmath_mf3_s* res )
{
    ASSERT( o->cols == p->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );

    for( uz_t i = 0; i < p->size; i++ ) ASSERT( p->data[ i ] < o->cols );

    if( o == res )
    {
        bmath_vf3_s* vec = bmath_vf3_s_create_set_size( o->cols );
        f3_t* v = vec->data;
        for( uz_t i = 0; i < o->rows; i++ )
        {
            f3_t* ri = res->data + i * res->stride;
            for( uz_t j = 0; j < res->cols; j++ ) v[ p->data[ j ] ] = ri[ j ];
            bcore_u_memcpy( sizeof( f3_t ), ri, v, res->cols );
        }
        bmath_vf3_s_discard( vec );
    }
    else
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            f3_t* oi =   o->data + i *   o->stride;
            f3_t* ri = res->data + i * res->stride;
            for( uz_t j = 0; j < o->cols; j++ ) ri[ p->data[ j ] ] = oi[ j ];
        }
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

bl_t bmath_mf3_s_decompose_cholesky( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );

    bl_t success = true;

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
                if( acc <= f3_lim_min ) success = false;
                vli[ j ] = ( acc > 0 ) ? sqrt( acc ) : 0;
            }
            else
            {
                vli[ j ] = ( vlj[ j ] != 0 ) ? acc / vlj[ j ] : 0;
                vlj[ i ] = 0; // upper triangle element
            }
        }
    }
    return success;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_decompose_luc( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );
    uz_t n = o->cols;
    uz_t stride = res->stride;
    bl_t success = true;

    bmath_mf3_s_cpy( o, res );
    for( uz_t i = 0; i < n; i++ )
    {
        f3_t* vi = res->data + i * stride;
        for( uz_t j = 0; j < i; j++ )
        {
            f3_t* vj = res->data + j;
            f3_t sum = 0;
            for( uz_t k = 0; k < j; k++ ) sum += vi[ k ] * vj[ k * stride ];

            f3_t denom = vj[ j * stride ];
            if( f3_abs( denom ) <= f3_lim_min ) success = false;

            vi[ j ] = ( denom != 0 ) ? ( ( vi[ j ] - sum ) / denom ) : 0;
        }

        for( uz_t j = i; j < n; j++ )
        {
            f3_t* vj = res->data + j;
            f3_t sum = 0;
            for( uz_t k = 0; k < i; k++ ) sum += vi[ k ] * vj[ k * stride ];
            vi[ j ] -= sum;
        }
    }
    return success;
}

//---------------------------------------------------------------------------------------------------------------------

bl_t bmath_mf3_s_ltr_inv_htp( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( bmath_mf3_s_is_square( o ) );
    ASSERT( bmath_mf3_s_is_equ_size( o, res ) );

    uz_t n = o->rows;

    bl_t success = true;

    // diagonal elements
    for( uz_t i = 0; i < n; i++ )
    {
        f3_t v = o->data[ i * o->stride + i ];
        if( f3_abs( v ) < f3_lim_min ) success = false;
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

            vri[ j ] = -vrj[ j ] * bmath_simd_f3_mul_vec( voj + i, vri + i, j - i );
            vrj[ i ] = 0;
        }
    }

    // zero lower off-diagonal elements; (this part must be kept to last for the in-place algorithm)
    for( uz_t i = 0; i < n; i++ )
    {
        f3_t* vri = res->data + i * res->stride;
        for( uz_t j = 0; j < i; j++ ) vri[ j ] = 0;
    }

    return success;
}

bl_t bmath_mf3_s_pdf_inv( const bmath_mf3_s* o, bmath_mf3_s* res )
{
    ASSERT( bmath_mf3_s_is_hsm( o ) );
    bl_t success = bmath_mf3_s_decompose_cholesky( o, res ); // res = ltr
    success = success & bmath_mf3_s_ltr_inv_htp( res, res ); // res = utr
    bmath_mf3_s_utr_mul_htp( res, res );      // res = oI
    return success;
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
            vrj[ i ] = bmath_simd_f3_mul_vec( voi, voj, j + 1 );
        }
    }

    // diagonal elements in reverse order
    for( uz_t i = n - 1; i < n; i-- )
    {
        const f3_t* voi =   o->data + i *   o->stride;
              f3_t* vri = res->data + i * res->stride;

        vri[ i ] = bmath_simd_f3_mul_vec( voi, voi, i + 1 );
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

            vrj[ i ] = bmath_simd_f3_mul_vec( voi + j, voj + j, n - j );
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
        res[ i ] = bmath_simd_f3_mul_vec( v1, op, i + 1 );
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
        res[ i ] = bmath_simd_f3_mul_vec( v1, op, i ) + op[ i ];
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
        res[ i ] = bmath_simd_f3_mul_vec( v1 + i, op + i, o->cols - i );
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
        f3_t sum = bmath_simd_f3_mul_vec( voi, res, i );
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
        f3_t sum = bmath_simd_f3_mul_vec( voi + i + 1, res + i + 1, n - i - 1 );
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
        res[ i ] -= bmath_simd_f3_mul_vec( voi, res, i );
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

void bmath_mf3_s_trd_htp( bmath_mf3_s* a, bmath_mf3_s* v )
{
    ASSERT( a->rows == a->cols );

    uz_t n = a->rows;

    if( n <= 2 ) return; // nothing to do

    if( v )
    {
        ASSERT( v->cols == n );
        ASSERT( v->rows == n );
        ASSERT( v != a );
    }

    bmath_arr_grt_f3_s grv = bmath_arr_grt_f3_of_size( n );
    bmath_grt_f3_s gr;

    for( uz_t j = 0; j < n; j++ )
    {
        // zero lower column
        for( uz_t k = n - 2; k > j; k-- )
        {
            uz_t l = k + 1;
            bmath_grt_f3_s_init_and_annihilate_b( &gr, &a->data[ k * a->stride + j ], &a->data[ l * a->stride + j ] );
            a->data[ l * a->stride + j ] = bmath_grt_f3_s_rho( &gr );
            bmath_mf3_s_arow_rotate( a, k, &gr, j + 1, l + 1 );
            f3_t* a00 = a->data + k * ( a->stride + 1 );
            a00[ -a->stride ] = a00[ -1 ];
            bmath_grt_f3_s_rotate( &gr, a00, a00 + 1 );
            grv.data[ l - 1 ] = gr;
        }

        if( bmath_arr_grt_f3_s_density( &grv, j + 1, n - 1 ) < 0.25 )
        {
            // sparse column operations
            for( uz_t k = n - 2; k > j; k-- )
            {
                gr = grv.data[ k ];
                bmath_mf3_s_acol_rotate( a, k, &gr, k + 1, n );
                if( k < n - 2 ) a->data[ ( k + 1 ) * ( a->stride + 1 ) + 1 ] = a->data[ ( k + 2 ) * ( a->stride + 1 ) - 1 ];
            }
        }
        else
        {
            // row swipes
            for( uz_t k = n - 1; k > j; k-- )
            {
                bmath_mf3_s_arow_swipe_rev( a, k, &grv, j + 1, k );
                if( k < n - 1 ) a->data[ k * ( a->stride + 1 ) + 1 ] = a->data[ ( k + 1 ) * ( a->stride + 1 ) - 1 ];
            }
        }
    }

    if( v )
    {
        bmath_mf3_s_one( v );
        for( uz_t j = 0; j < n; j++ )
        {
            for( uz_t l = n - 1; l > j + 1; l-- )
            {
                f3_t rho = a->data[ l * a->stride + j ];
                bmath_grt_f3_s_init_from_rho( &gr, rho );
                bmath_mf3_s_arow_rotate( v, l - 1, &gr, l - j - 1, n );
            }
        }
    }

    // set off-tri diag elements zero
    for( uz_t i = 0; i < n; i++ )
    {
        f3_t* ai = a->data + i * a->stride;
        if( i > 1 ) for( uz_t j = 0; j < i - 1; j++ ) ai[ j ] = 0;
        for( uz_t j = i + 2; j < n; j++ ) ai[ j ] = 0;
    }

    // symmetrize off-diag
    for( uz_t i = 0; i < n - 1; i++ )
    {
        f3_t* ai = a->data + i * ( a->stride + 1 );
        f3_t b = ( ai[ 1 ] + ai[ a->stride ] ) * 0.5;
        ai[ 1 ] = ai[ a->stride ] = b;
    }

    bmath_arr_grt_f3_s_down( &grv );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_trd( bmath_mf3_s* a, bmath_mf3_s* v )
{
    bmath_mf3_s_trd_htp( a, v );
    if( v ) bmath_mf3_s_htp( v, v );
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

    if( u )
    {
        bmath_mf3_s_one( u );
        ASSERT( u != a );
        ASSERT( u->rows == a->rows );
        ASSERT( u->cols == a->rows /*full*/ || u->cols == a->cols /*thin*/  ); // u may be full or thin (nothing in-between)
    }

    if( v )
    {
        bmath_mf3_s_one( v );
        ASSERT( v->cols == a->cols );
        ASSERT( v->rows == a->cols );
        ASSERT( v != a );
        ASSERT( v != u );
    }

    if( a->rows <= 1 )
    {
        if( u ) a->rows = u->cols;
        return; // nothing else to do
    }

    if( a->rows > a->cols * 4 )
    {
        if( u )
        {
            bmath_mf3_s_qrd( u, a );

            uz_t u_cols = u->cols;

            a->rows = a->cols;
            bmath_mf3_s* u2 = bmath_mf3_s_create();
            bmath_mf3_s_set_size( u2, a->rows, a->rows );
            bmath_mf3_s_ubd( u2, a, v );
            u->cols = u2->rows;

            bmath_mf3_s_htp( u2, u2 );

            bmath_mf3_s_mul_htp( u, u2, u );
            bmath_mf3_s_discard( u2 );

            u->cols = u_cols;
            a->rows = u_cols;
        }
        else
        {
            bmath_mf3_s_qrd( NULL, a );
            uz_t a_rows = a->rows;
            a->rows = a->cols;
            bmath_mf3_s_ubd( NULL, a, v );
            a->rows = a_rows;
        }
        return;
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

    if( u )
    {
        bmath_mf3_s_one( u );
        ASSERT( u->cols == a->rows );
        ASSERT( u->rows == a->rows );
        ASSERT( u != a );
        ASSERT( u != v );
    }

    if( v )
    {
        bmath_mf3_s_one( v );
        ASSERT( v != a );
        ASSERT( v->rows == a->cols );
        ASSERT( v->cols == a->cols || v->cols == a->rows ); // v may be full or thin (nothing in-between)
    }

    if( a->cols <= 1 )
    {
        if( v ) a->cols = v->cols;
        return; // nothing else to do
    }

    if( a->cols > a->rows * 4 )
    {
        if( v )
        {
            bmath_mf3_s_lqd( a, v );

            uz_t v_cols = v->cols;

            a->cols = a->rows;
            bmath_mf3_s* v2 = bmath_mf3_s_create();
            bmath_mf3_s_set_size( v2, a->cols, a->cols );
            bmath_mf3_s_lbd( u, a, v2 );
            v->cols = v2->rows;

            bmath_mf3_s_htp( v2, v2 );

            bmath_mf3_s_mul_htp( v, v2, v );
            bmath_mf3_s_discard( v2 );

            v->cols = v_cols;
            a->cols = v_cols;
        }
        else
        {
            bmath_mf3_s_lqd( a, NULL );
            uz_t a_cols = a->cols;
            a->cols = a->rows;
            bmath_mf3_s_lbd( u, a, NULL );
            a->cols = a_cols;
        }
        return;
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
    if( bmath_arr_grt_f3_s_density( grt, col_start, col_end ) < 0.25 )
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
    if( bmath_arr_grt_f3_s_density( grt, col_start, col_end ) < 0.25 )
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
    if( bmath_arr_grt_f3_s_density( grt, col_start, col_end ) < 0.25 )
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

void bmath_mf3_s_to_string( const bmath_mf3_s* o, st_s* string )
{
    st_s_pushf( string, "(%zu x %zu)\n", o->rows, o->cols );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* v = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            if( j > 0 ) st_s_pushf( string, " " );
            st_s_pushf( string, "%9.3g", v[ j ] );
        }
        st_s_pushf( string, "\n" );
    }
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_mf3_s_to_stdout( const bmath_mf3_s* o )
{
    st_s* s = st_s_create();
    bmath_mf3_s_to_string( o, s );
    bcore_msg_fa( "#<sc_t>", s->sc );
    st_s_discard( s );
}

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    BCORE_LIFE_INIT();

    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m3 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m4 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v1 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v2 );
    BCORE_LIFE_CREATE( bmath_vf3_s, v3 );
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

    // lower triangular inversion
    {
        bmath_mf3_s_set_size( m1, 3, 3 );
        bmath_mf3_s_set_f3( m1, 0, 0, 1 ); bmath_mf3_s_set_f3( m1, 0, 1, 0 );  bmath_mf3_s_set_f3( m1, 0, 2, 0 );
        bmath_mf3_s_set_f3( m1, 1, 0, 7 ); bmath_mf3_s_set_f3( m1, 1, 1, 5 );  bmath_mf3_s_set_f3( m1, 1, 2, 0 );
        bmath_mf3_s_set_f3( m1, 2, 0, 1 ); bmath_mf3_s_set_f3( m1, 2, 1, 2 );  bmath_mf3_s_set_f3( m1, 2, 2, 4 );
        bmath_mf3_s_set_size( m2, 3, 3 );
        bmath_mf3_s_set_size( m3, 3, 3 );
        bmath_mf3_s_ltr_inv_htp( m1, m2 );
        bmath_mf3_s_mul_htp( m1, m2, m3 );
        ASSERT( bmath_mf3_s_is_near_one( m3, 1E-10 ) );
    }

    // A * AT
    {
        uz_t n = 100;
        u2_t rval = 1234;
        bmath_mf3_s_set_size( m1, n, n );
        bmath_mf3_s_set_size_to( m1, m2 );
        bmath_mf3_s_set_random( m1, false, false, 0, 1.0, -1, 1, &rval );
        bmath_mf3_s_mul_htp( m1, m1, m2 );
        ASSERT( bmath_mf3_s_is_near_hsm( m2, 1E-8 ) );
    }


    // covariance
    {
        uz_t size = 1000;
        uz_t n    = 10;
        u2_t rval = 124;
        bmath_arr_vf3_s_set_size( a1, size );
        bmath_arr_vf3_s_on_section_set_size( a1, 0, -1, n );
        bmath_arr_vf3_s_on_section_set_random( a1, 0, -1, 1.0, -1, 1, &rval );

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
        bmath_mf3_s_set_random( m1, false, false, 0, 1.0, -1, 1, &rval );
        bmath_vf3_s_set_random( v1, 1.0, -1, 1, &rval );

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
        bmath_mf3_s_set_random( m1, false, false, 0, 1.0, -1, 1, &rval );

        bmath_mf3_s m1_sub = bmath_mf3_s_get_weak_sub_mat( m1, 0, 0, n, n );
        bmath_mf3_s_mul_htp( &m1_sub, &m1_sub, &m1_sub ); // make nxn submatrix of m1 symmetric

        bmath_vf3_s_set_random( v1, 1.0, -1, 1, &rval );

        bmath_mf3_s_mul_av1( m1, v1, v2 );
        bmath_mf3_s_pdf_inv_av1( m1, m2 );
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
        bmath_mf3_s_set_random( m1, false, false, 0, 1.0, -1, 1, &rval );

        bmath_mf3_s m1_sub = bmath_mf3_s_get_weak_sub_mat( m1, 0, 0, n, n );
        bmath_mf3_s_mul_htp( &m1_sub, &m1_sub, &m1_sub ); // make nxn submatrix of m1 symmetric

        bmath_vf3_s_set_random( v1, 1.0, -1, 1, &rval );

        bmath_mf3_s_mul_av1( m1, v1, v2 );
        bmath_mf3_s_hsm_piv_av1( m1, 1E-8, m2 );
        bmath_mf3_s_mul_av1( m2, v2, v3 );

        ASSERT( bmath_vf3_s_is_near_equ( v1, v3, 1E-8 ) );
    }

    BCORE_LIFE_DOWN();
    return NULL;
}

//---------------------------------------------------------------------------------------------------------------------

static void eval_test( void )
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_eval_s, eval );
    BCORE_LIFE_CREATE( bmath_arr_mf3_eval_s, arr_eval );

    eval->density = 1.0;
    eval->rows = 20; eval->cols = 20; bmath_arr_mf3_eval_s_push( arr_eval, eval );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_trd    , ( fp_t )bmath_mf3_s_trd,     NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_evd_htp, ( fp_t )bmath_mf3_s_evd_htp, NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_cld    , ( fp_t )bmath_mf3_s_decompose_cholesky, NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_lud    , ( fp_t )bmath_mf3_s_decompose_luc, NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_inv    , ( fp_t )bmath_mf3_s_inv, NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_pdf_inv, ( fp_t )bmath_mf3_s_pdf_inv, NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_hsm_piv, ( fp_t )bmath_mf3_s_hsm_piv, NULL );

    eval->full = false;
    eval->rows = 10; eval->cols = 30; bmath_arr_mf3_eval_s_push( arr_eval, eval );
    eval->rows = 30; eval->cols = 10; bmath_arr_mf3_eval_s_push( arr_eval, eval );
    eval->full = true;
    eval->rows = 10; eval->cols = 30; bmath_arr_mf3_eval_s_push( arr_eval, eval );
    eval->rows = 30; eval->cols = 10; bmath_arr_mf3_eval_s_push( arr_eval, eval );

    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_qrd,     ( fp_t )bmath_mf3_s_qrd,     NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_qrd_pmt, ( fp_t )bmath_mf3_s_qrd_pmt, NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_lqd,     ( fp_t )bmath_mf3_s_lqd,     NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_pmt_lqd, ( fp_t )bmath_mf3_s_pmt_lqd, NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_ubd,     ( fp_t )bmath_mf3_s_ubd,     NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_lbd,     ( fp_t )bmath_mf3_s_lbd,     NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_svd,     ( fp_t )bmath_mf3_s_svd,     NULL );
    bmath_arr_mf3_eval_s_run( arr_eval, TYPEOF_bmath_fp_mf3_s_piv,     ( fp_t )bmath_mf3_s_piv,     NULL );

    BCORE_LIFE_DOWN();
}

/**********************************************************************************************************************/

vd_t bmath_mf3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_mf3" ) ) )
    {
        case TYPEOF_init1:
        {
            // features
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_trd_htp );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_trd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_evd_htp );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_svd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_ubd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_lbd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_qrd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_qrd_pmt );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_lqd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_pmt_lqd );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_cld );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_lud );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_inv );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_pdf_inv );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_piv );
            BCORE_REGISTER_TYPE( function_pointer, bmath_fp_mf3_s_hsm_piv );

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
            return NULL;
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

