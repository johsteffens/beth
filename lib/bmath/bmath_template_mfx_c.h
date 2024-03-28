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

/** Template: Matrix */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,clear)( bmath_mfx_s* o )
{
    bcore_matrix_a_clear( ( bcore_matrix* )o );
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s* BCATU(bmath_mfx_s,set_size)( bmath_mfx_s* o, uz_t rows, uz_t cols )
{
    bcore_matrix_a_set_size( ( bcore_matrix* )o, rows, cols );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,set_random)( bmath_mfx_s* o, bl_t hsm, bl_t pdf, sz_t rank_deficit, fx_t density, fx_t min, fx_t max, bcore_prsg* prsg )
{
    BLM_INIT();
    if( !prsg ) prsg = ( bcore_prsg* )BLM_CREATE( bcore_prsg_lcg_u3_00_s );
    if( pdf ) ASSERT( hsm );
    if( hsm ) ASSERT( o->rows == o->cols );

    if( rank_deficit == 0 ) // full rank
    {
        if( pdf )
        {
            BCATU(bmath_mfx_s,set_random)( o, false, false, 0, density, min, max, prsg );
            BCATU(bmath_mfx_s,mul_htp)( o, o, o );
        }
        else if( hsm )
        {
            for( uz_t i = 0; i < o->rows; i++ )
            {
                fx_t* vi = o->data + i * o->stride;
                for( uz_t j = 0; j <= i; j++ )
                {
                    fx_t* vj = o->data + j * o->stride;
                    if( bcore_prsg_a_gen_f3( prsg, 0, 1 ) < density )
                    {
                        vi[ j ] = bcore_prsg_a_gen_f3( prsg, min, max );
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
                fx_t* v = o->data + i * o->stride;
                for( uz_t j = 0; j < o->cols; j++ )
                {
                    if( bcore_prsg_a_gen_f3( prsg, 0, 1 ) < density )
                    {
                        v[ j ] = bcore_prsg_a_gen_f3( prsg, min, max );
                    }
                    else
                    {
                        v[ j ] = 0;
                    }
                }
            }
        }
    }
    else if( rank_deficit < uz_min( o->cols, o->rows ) ) // rank is uz_min( o->cols, o->rows ) - rank_deficit
    {
        uz_t n = uz_min( o->cols, o->rows );
        uz_t rank = n - rank_deficit;
        if( hsm )
        {
            bmath_mfx_s* m1 = BCATU(bmath_mfx_s,create)();
            bmath_vfx_s* v1 = BCATU(bmath_vfx_s,create)();
            BCATU(bmath_mfx_s,set_size)( m1, o->rows, rank );
            BCATU(bmath_mfx_s,set_random)( m1, false, false, 0, density, min, max, prsg );

            BCATU(bmath_vfx_s,set_size)( v1, rank );
            if( pdf )
            {
                BCATU(bmath_vfx_s,set_random)( v1, density, 0.0, 1.0, prsg );
            }
            else
            {
                BCATU(bmath_vfx_s,set_random)( v1, density, min, max, prsg );
            }

            BCATU(bmath_mfx_s,mul_udu_htp)( m1, v1, o );
            BCATU(bmath_mfx_s,discard)( m1 );
            BCATU(bmath_vfx_s,discard)( v1 );
        }
        else
        {
            bmath_mfx_s* m1 = BCATU(bmath_mfx_s,create)();
            bmath_mfx_s* m2 = BCATU(bmath_mfx_s,create)();
            BCATU(bmath_mfx_s,set_size)( m1, o->rows, rank );
            BCATU(bmath_mfx_s,set_size)( m2, o->cols, rank );
            BCATU(bmath_mfx_s,set_random)( m1, false, false, 0, density, min, max, prsg );
            BCATU(bmath_mfx_s,set_random)( m2, false, false, 0, density, min, max, prsg );
            BCATU(bmath_mfx_s,mul_htp)( m1, m2, o );
            BCATU(bmath_mfx_s,discard)( m1 );
            BCATU(bmath_mfx_s,discard)( m2 );
        }
    }
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,set_random_u3)( bmath_mfx_s* o, bl_t hsm, bl_t pdf, sz_t rank_deficit, fx_t density, fx_t min, fx_t max, u3_t* p_rval )
{
    BLM_INIT();
    bcore_prsg* prsg = ( bcore_prsg* )BLM_CREATE( bcore_prsg_lcg_u3_00_s );
    if( p_rval ) bcore_prsg_a_reseed( prsg, *p_rval );
    BCATU(bmath_mfx_s,set_random)( o, hsm, pdf, rank_deficit, density, min, max, prsg );
    if( p_rval ) *p_rval = bcore_prsg_a_gen_u3( prsg );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,set_random_full_rank)( bmath_mfx_s* o, bl_t pdf, fx_t eps, bcore_prsg* prsg )
{
    BLM_INIT();
    if( !prsg ) prsg = ( bcore_prsg* )BLM_CREATE( bcore_prsg_lcg_u3_00_s );

    ASSERT( BCATU(bmath_mfx_s,is_square)( o ) );
    ASSERT( eps > 0 && eps <= 1.0 );

    BCATU(bmath_mfx_s,set_random)( o, false, false, 0, 1.0, -1.0, 1.0, prsg );
    sz_t n = o->rows;

    bmath_mfx_s* u = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_vfx_s* d = BLM_A_PUSH( BCATU(bmath_vfx_s,create)() );
    bmath_mfx_s* v = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );

    BCATU(bmath_mfx_s,set_size)( u, n, n );
    BCATU(bmath_mfx_s,set_size)( v, n, n );
    BCATU(bmath_vfx_s,set_size)( d, n );

    BCATU(bmath_mfx_s,svd)( u, o, v );

    BCATU(bmath_vfx_s,set_random)( d, 1.0, eps, 1.0, prsg );

    if( pdf )
    {
        BCATU(bmath_mfx_s,mul_udu_htp)( u, d, o );

        // enforce symmetry (mul_udu_htp may leave small symmetry deficits)
        for( sz_t i = 0; i < n; i++ )
        {
            fx_t* vi = o->data + i * o->stride;
            for( sz_t j = 0; j < i; j++ )
            {
                fx_t* vj = o->data + j * o->stride;
                vj[ i ] = vi[ j ];
            }
        }
    }
    else
    {
        BCATU(bmath_mfx_s,mul_udv_htp)( u, d, v, o );
    }

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,set_random_full_rank_u3)( bmath_mfx_s* o, bl_t pdf, fx_t eps, u3_t* p_rval )
{
    BLM_INIT();
    bcore_prsg* prsg = ( bcore_prsg* )BLM_CREATE( bcore_prsg_lcg_u3_00_s );
    if( p_rval ) bcore_prsg_a_reseed( prsg, *p_rval );
    BCATU(bmath_mfx_s,set_random_full_rank)( o, pdf, eps, prsg );
    if( p_rval ) *p_rval = bcore_prsg_a_gen_u3( prsg );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s* BCATU(bmath_mfx_s,create_set_size)( uz_t rows, uz_t cols )
{
    bmath_mfx_s* o = BCATU(bmath_mfx_s,create)();
    BCATU(bmath_mfx_s,set_size)( o, rows, cols );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// checks, deviations

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_equ)( const bmath_mfx_s* o, const bmath_mfx_s* op, fx_t max_dev )
{
    if( o->rows != op->rows ) return false;
    if( o->cols != op->cols ) return false;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v1 = o ->data + i * o ->stride;
        const fx_t* v2 = op->data + i * op->stride;
        for( uz_t j = 0; j < o->cols; j++ ) if( BCATU(fx,abs)( v1[ j ] - v2[ j ] ) > max_dev ) return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_zro)( const bmath_mfx_s* o, fx_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ ) if( BCATU(fx,abs)( v1[ j ] ) > max_dev ) return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_one)( const bmath_mfx_s* o, fx_t max_dev )
{
    if( o->rows != o->cols ) return false;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            if( BCATU(fx,abs)( v1[ j ] - ( ( j == i ) ? 1.0 : 0.0 ) ) > max_dev ) return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_dag)( const bmath_mfx_s* o, fx_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            if( ( i != j ) && ( BCATU(fx,abs)( v1[ j ] ) > max_dev ) ) return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_evd_dag)( const bmath_mfx_s* o, fx_t max_dev )
{
    if( !BCATU(bmath_mfx_s,is_near_dag)( o, max_dev ) ) return false;
    sz_t n = sz_min( o->cols, o->rows );
    sz_t sp1 = o->stride + 1;
    for( sz_t i = 1; i < n; i++ )
    {
        if( ( o->data[ i * sp1 ] - o->data[ ( i - 1 ) * sp1 ] ) > max_dev ) return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_svd_dag)( const bmath_mfx_s* o, fx_t max_dev )
{
    if( !BCATU(bmath_mfx_s,is_near_evd_dag)( o, max_dev ) ) return false;
    sz_t n = sz_min( o->cols, o->rows );
    for( sz_t i = 0; i < n; i++ ) if( o->data[ i * ( o->stride + 1 ) ] < -max_dev ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_trd)( const bmath_mfx_s* o, fx_t max_dev )
{
    if( o->rows != o->cols ) return false;
    uz_t n = o->rows;
    for( uz_t i = 0; i < n; i++ )
    {
        const fx_t* vi = o ->data + i * o ->stride;
        for( uz_t j = i + 2; j < n; j++ )
        {
            const fx_t* vj = o ->data + j * o ->stride;
            if( BCATU(fx,abs)( vi[ j ] ) > max_dev ) return false;
            if( BCATU(fx,abs)( vj[ i ] ) > max_dev ) return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_ubd)( const bmath_mfx_s* o, fx_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* vi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            if( ( j < i || j > i + 1 ) && ( BCATU(fx,abs)( vi[ j ] ) > max_dev ) ) return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_lbd)( const bmath_mfx_s* o, fx_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* vi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            if( ( j + 1 < i || j > i ) && ( BCATU(fx,abs)( vi[ j ] ) > max_dev ) ) return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_otn)( const bmath_mfx_s* o, fx_t max_dev )
{
    if( o->rows <= o->cols )
    {
        for( sz_t i = 0; i < o->rows; i++ )
        {
            const fx_t* v1 = o ->data + i * o ->stride;
            for( sz_t j = i; j < o->rows; j++ )
            {
                const fx_t* v2 = o ->data + j * o ->stride;
                fx_t dot_prd = BCATU(bmath,fx,t_vec,mul_vec_esp)( v1, v2, o->cols );
                if( BCATU(fx,abs)( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) ) > max_dev ) return false;
            }
        }
    }
    else
    {
        for( uz_t i = 0; i < o->cols; i++ )
        {
            const fx_t* v1 = o ->data + i;
            for( uz_t j = i; j < o->cols; j++ )
            {
                const fx_t* v2 = o ->data + j;
                fx_t dot_prd = 0;
                for( uz_t k = 0; k < o->rows; k++ ) dot_prd += v1[ k * o->stride ] * v2[ k * o->stride ];
                if( BCATU(fx,abs)( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) ) > max_dev ) return false;
            }
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_uni)( const bmath_mfx_s* o, fx_t max_dev )
{
    if( o->rows != o->cols ) return false;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = i; j < o->rows; j++ )
        {
            const fx_t* v2 = o ->data + j * o ->stride;
            fx_t dot_prd = 0;
            for( uz_t k = 0; k < o->cols; k++ ) dot_prd += v1[ k ] * v2[ k ];
            if( BCATU(fx,abs)( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) ) > max_dev ) return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_utr)( const bmath_mfx_s* o, fx_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v = o ->data + i * o->stride;
        uz_t j_end = uz_min( i, o->cols );
        for( uz_t j = 0; j < j_end; j++ )
        {
            if( BCATU(fx,abs)( v[ j ] ) > max_dev ) return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_ltr)( const bmath_mfx_s* o, fx_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v = o ->data + i * o->stride;
        for( uz_t j = i + 1; j < o->cols; j++ )
        {
            if( BCATU(fx,abs)( v[ j ] ) > max_dev ) return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_near_hsm)( const bmath_mfx_s* o, fx_t max_dev )
{
    if( o->rows != o->cols ) return false;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* vi = o ->data + i * o ->stride;
        for( uz_t j = 0; j < i; j++ )
        {
            const fx_t* vj = o ->data + j * o ->stride;
            if( BCATU(fx,abs)( vi[ j ] - vj[ i ] ) > max_dev ) return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,is_nan)( const bmath_mfx_s* o )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ ) if( BCATU(fx,is_nan)( v1[ j ] ) ) return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_mfx_s,tss)( const bmath_mfx_s* o )
{
    f3_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ ) sum += f3_sqr( v1[ j ] );
    }
    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_mfx_s,fdev_equ)( const bmath_mfx_s* o, const bmath_mfx_s* op )
{
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == op->cols );

    f3_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* ai = o->data + i * o->stride;
        const fx_t* bi = op->data + i * op->stride;
        f3_t sum0 = 0;
        for( uz_t j = 0; j < o->cols; j++ ) sum0 += f3_sqr( ai[ j ] - bi[ j ] );
        sum += sum0;
    }
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_mfx_s,fdev_zro)( const bmath_mfx_s* o )
{
    f3_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* oi = o->data + i * o->stride;
        f3_t sum0 = 0;
        for( uz_t j = 0; j < o->cols; j++ ) sum0 += f3_sqr( oi[ j ] );
        sum += sum0;
    }
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_mfx_s,fdev_one)( const bmath_mfx_s* o )
{
    f3_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* oi = o->data + i * o->stride;
        f3_t sum0 = 0;
        for( uz_t j = 0; j < o->cols; j++ ) sum0 += f3_sqr( oi[ j ] - ( i == j ? 1 : 0 ) );
        sum += sum0;
    }
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_mfx_s,fdev_otn)( const bmath_mfx_s* o )
{
    f3_t sum = 0;
    if( o->rows <= o->cols )
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const fx_t* v1 = o ->data + i * o ->stride;
            f3_t sum0 = 0;
            for( uz_t j = i; j < o->rows; j++ )
            {
                const fx_t* v2 = o ->data + j * o ->stride;
                f3_t dot_prd = BCATU(bmath,fx,t_vec,mul_vec_esp)( v1, v2, o->cols );
                sum0 += f3_sqr( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) );
            }
            sum += sum0;
        }
    }
    else
    {
        for( uz_t i = 0; i < o->cols; i++ )
        {
            const fx_t* v1 = o ->data + i;
            f3_t sum0 = 0;
            for( uz_t j = i; j < o->cols; j++ )
            {
                const fx_t* v2 = o ->data + j;
                f3_t dot_prd = 0;
                for( uz_t k = 0; k < o->rows; k++ ) dot_prd += v1[ k * o->stride ] * v2[ k * o->stride ];
                sum0 += f3_sqr( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) );
            }
            sum += sum0;
        }
    }
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// initializations; copying; basic matrix operations

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_mfx_s,fx_max)( const bmath_mfx_s* o )
{
    fx_t max = ( o->rows * o->cols > 0 ) ? o->data[ 0 ] : 0;

    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* oi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ ) max = BCATU(fx,max)( max, oi[ j ] );
    }

    return max;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_mfx_s,fx_min)( const bmath_mfx_s* o )
{
    fx_t min = ( o->rows * o->cols > 0 ) ? o->data[ 0 ] : 0;

    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* oi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ ) min = BCATU(fx,min)( min, oi[ j ] );
    }
    return min;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_mfx_s,fx_sum)( const bmath_mfx_s* o )
{
    fx_t sum = 0;

    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* oi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ ) sum += oi[ j ];
    }

    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_mfx_s,fx_trc)( const bmath_mfx_s* o )
{
    ASSERT( o->cols == o->rows );
    fx_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ ) sum += o->data[ i * ( o->stride + 1 ) ];
    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_mfx_s,fx_sub_sqr)( const bmath_mfx_s* o, const bmath_mfx_s* op )
{
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, op ) );
    fx_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v1 =  o->data + i *  o->stride;
        const fx_t* v2 = op->data + i * op->stride;
        for( uz_t i = 0; i < o->cols; i++ ) sum += BCATU(fx,sqr)( v1[ i ] - v2[ i ] );
    }
    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_mfx_s,max)( const bmath_mfx_s* o )
{
    f3_t max = ( o->rows * o->cols > 0 ) ? o->data[ 0 ] : 0;

    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* oi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ ) max = f3_max( max, oi[ j ] );
    }

    return max;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_mfx_s,min)( const bmath_mfx_s* o )
{
    f3_t min = ( o->rows * o->cols > 0 ) ? o->data[ 0 ] : 0;

    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* oi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ ) min = f3_min( min, oi[ j ] );
    }
    return min;
}

//----------------------------------------------------------------------------------------------------------------------

const bmath_mfx_s* BCATU(bmath_mfx_s,get_min_max)( const bmath_mfx_s* o, f3_t* p_min, f3_t* p_max )
{
    f3_t min = ( o->rows * o->cols > 0 ) ? o->data[ 0 ] : 0;
    f3_t max = min;

    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* oi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            min = f3_min( min, oi[ j ] );
            max = f3_min( max, oi[ j ] );
        }
    }
    if( p_min ) *p_min = min;
    if( p_max ) *p_max = max;
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_mfx_s,sum)( const bmath_mfx_s* o )
{
    f3_t sum = 0;

    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* oi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ ) sum += oi[ j ];
    }

    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_mfx_s,trc)( const bmath_mfx_s* o )
{
    ASSERT( o->cols == o->rows );
    f3_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ ) sum += o->data[ i * ( o->stride + 1 ) ];
    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t BCATU(bmath_mfx_s,sub_sqr)( const bmath_mfx_s* o, const bmath_mfx_s* op )
{
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, op ) );
    f3_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v1 =  o->data + i *  o->stride;
        const fx_t* v2 = op->data + i * op->stride;
        for( uz_t i = 0; i < o->cols; i++ ) sum += f3_sqr( v1[ i ] - v2[ i ] );
    }
    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,add)( const bmath_mfx_s* o, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, b ) );
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, r ) );
    if( BCATU(bmath_mfx_s,is_folded)( r ) )
    {
        ASSERT( o->stride == r->stride );
        ASSERT( b->stride == r->stride );
        sz_t size = ( o->rows - 1 ) * o->stride + o->cols;
        for( sz_t i = 0; i < size; i++ ) r->data[ i ] = o->data[ i ] + b->data[ i ];
    }
    else
    {
        for( sz_t i = 0; i < o->rows; i++ )
        {
            const fx_t* vo = o->data + i * o->stride;
            const fx_t* vb = b->data + i * b->stride;
                  fx_t* vr = r->data + i * r->stride;
            for( sz_t j = 0; j < o->cols; j++ ) vr[ j ] = vo[ j ] + vb[ j ];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,sub)( const bmath_mfx_s* o, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, b ) );
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, r ) );
    if( BCATU(bmath_mfx_s,is_folded)( r ) )
    {
        ASSERT( o->stride == r->stride );
        ASSERT( b->stride == r->stride );
        sz_t size = ( o->rows - 1 ) * o->stride + o->cols;
        for( sz_t i = 0; i < size; i++ ) r->data[ i ] = o->data[ i ] - b->data[ i ];
    }
    else
    {
        for( sz_t i = 0; i < o->rows; i++ )
        {
            const fx_t* vo = o->data + i * o->stride;
            const fx_t* vb = b->data + i * b->stride;
                  fx_t* vr = r->data + i * r->stride;
            for( sz_t j = 0; j < o->cols; j++ ) vr[ j ] = vo[ j ] - vb[ j ];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s* BCATU(bmath_mfx_s,zro)( bmath_mfx_s* o )
{
    if( BCATU(bmath_mfx_s,is_folded)( o ) )
    {
        sz_t size = ( o->rows - 1 ) * o->stride + o->cols;
        assert( size <= o->size );
        for( sz_t i = 0; i < size; i++ ) o->data[ i ] = 0;
    }
    else
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            fx_t* v1 = o ->data + i * o ->stride;
            for( uz_t j = 0; j < o->cols; j++ ) v1[ j ] = 0;
        }
    }
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s* BCATU(bmath_mfx_s,one)( bmath_mfx_s* o )
{
    if( BCATU(bmath_mfx_s,is_folded)( o ) )
    {
        BCATU(bmath_mfx_s,zro)( o );
        sz_t diag_size = uz_min( o->rows, o->cols );
        for( sz_t i = 0; i < diag_size; i++ ) o->data[ i * ( o->stride + 1 ) ] = 1;
    }
    else
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            fx_t* v1 = o ->data + i * o ->stride;
            for( uz_t j = 0; j < o->cols; j++ ) v1[ j ] = ( j == i ) ? 1 : 0;
        }
    }
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s* BCATU(bmath_mfx_s,neg)( const bmath_mfx_s* o, bmath_mfx_s* r )
{
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, r ) );

    if( BCATU(bmath_mfx_s,is_folded)( r ) )
    {
        ASSERT( o->stride == r->stride );
        sz_t size = ( o->rows - 1 ) * o->stride + o->cols;
        for( sz_t i = 0; i < size; i++ ) r->data[ i ] = -o->data[ i ];
    }
    else
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const fx_t* vo = o->data + i * o->stride;
                  fx_t* vr = r->data + i * r->stride;
            for( uz_t j = 0; j < o->cols; j++ ) vr[ j ] = -vo[ j ];
        }
    }
    return r;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s* BCATU(bmath_mfx_s,cpy)( const bmath_mfx_s* o, bmath_mfx_s* r )
{
    if( r == o ) return r;
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, r ) );
    if( BCATU(bmath_mfx_s,is_folded)( r ) )
    {
        ASSERT( o->stride == r->stride );
        sz_t size = ( o->rows - 1 ) * o->stride + o->cols;
        for( sz_t i = 0; i < size; i++ ) r->data[ i ] = o->data[ i ];
    }
    else
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const fx_t* vo = o->data + i * o->stride;
                  fx_t* vr = r->data + i * r->stride;
            for( uz_t j = 0; j < o->cols; j++ ) vr[ j ] = vo[ j ];
        }
    }
    return r;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,opd)( bmath_mfx_s* o, const bmath_vfx_s* a, const bmath_vfx_s* b )
{
    ASSERT( o->rows == a->size );
    ASSERT( o->cols == b->size );

    const fx_t* v1 = a->data;
    const fx_t* v2 = b->data;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        fx_t* oi = o->data   + o->stride * i;
        for( uz_t j = 0; j < o->cols; j++ ) oi[ j ] = ( v1[ i ] * v2[ j ] );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,opd_add)( bmath_mfx_s* o, const bmath_vfx_s* a, const bmath_vfx_s* b, const bmath_mfx_s* c )
{
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, c ) );
    ASSERT( o->rows == a->size );
    ASSERT( o->cols == b->size );

    const fx_t* v1 = a->data;
    const fx_t* v2 = b->data;
    for( uz_t i = 0; i < o->rows; i++ )
    {
              fx_t* oi = o->data + o->stride * i;
        const fx_t* ci = c->data + c->stride * i;
        for( uz_t j = 0; j < o->cols; j++ ) oi[ j ] = ci[ j ] + ( v1[ i ] * v2[ j ] );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,opd_mul_add)( bmath_mfx_s* o, const bmath_vfx_s* a, const bmath_vfx_s* b, f3_t w, const bmath_mfx_s* c )
{
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, c ) );
    ASSERT( o->rows == a->size );
    ASSERT( o->cols == b->size );

    const fx_t* v1 = a->data;
    const fx_t* v2 = b->data;
    for( uz_t i = 0; i < o->rows; i++ )
    {
              fx_t* oi = o->data + o->stride * i;
        const fx_t* ci = c->data + c->stride * i;
        for( uz_t j = 0; j < o->cols; j++ ) oi[ j ] = ci[ j ] + ( w * v1[ i ] * v2[ j ] );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_hdm)( const bmath_mfx_s* a, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    ASSERT(  BCATU(bmath_mfx_s,is_equ_size)( a, b ) );
    ASSERT(  BCATU(bmath_mfx_s,is_equ_size)( a, r ) );
    ASSERT( !BCATU(bmath_mfx_s,is_folded)( r ) );

    for( sz_t i = 0; i < a->rows; i++ )
    {
        const fx_t* va = a->data + i * a->stride;
        const fx_t* vb = b->data + i * b->stride;
              fx_t* vr = r->data + i * r->stride;
        for( sz_t j = 0; j < a->cols; j++ ) vr[ j ] = va[ j ] * vb[ j ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,inv_htp_via_luc)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, res ) );
    bmath_mfx_s* luc = BCATU(bmath_mfx_s,create)();
    BCATU(bmath_mfx_s,set_size_to)( o, luc );

    bl_t success = BCATU(bmath_mfx_s,decompose_luc)( o, luc );
    BCATU(bmath_mfx_s,one)( res );
    BCATU(bmath_mfx_s,luc_solve_htp_htp)( luc, res, res );
    BCATU(bmath_mfx_s,discard)( luc );
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,inv_via_cld)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // Makes use of the relationship A^-1 = ( A * A^T )^-1
    // A * A^T is positive definite and can be solved via cld
    BCATU(bmath_mfx_s,mul_htp)( o, o, res );
    bl_t success = BCATU(bmath_mfx_s,pdf_inv)( res, res );
    BCATU(bmath_mfx_s,htp_mul)( o, res, res );
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,inv_htp)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    bl_t success = BCATU(bmath_mfx_s,inv)( o, res );
    BCATU(bmath_mfx_s,htp)( res, res );
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,inv)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    return BCATU(bmath_mfx_s,inv_via_cld)( o, res );
}

//----------------------------------------------------------------------------------------------------------------------

// pseudo inversion
bl_t BCATU(bmath_mfx_s,piv)( const bmath_mfx_s* o, fx_t eps, bmath_mfx_s* res )
{
    ASSERT( o->rows == res->cols );
    ASSERT( o->cols == res->rows );

    uz_t n = uz_min( o->rows, o->cols );
    if( n == 0 ) return true;

    bmath_mfx_s* a = BCATU(bmath_mfx_s,create)();
    // we let 'a' use the space of res
    if( o != res )
    {
        a->data   = res->data;
        a->rows   = o->rows;
        a->cols   = o->cols;
        a->stride = a->cols;
        a->size   = a->rows * a->cols;
        a->space  = 0; // a does not own its space
        BCATU(bmath_mfx_s,cpy)( o, a );
    }
    else
    {
        *a = *res;
        a->space = 0; // a does not own its space
    }

    bmath_mfx_s* u = BCATU(bmath_mfx_s,create)();
    bmath_mfx_s* v = BCATU(bmath_mfx_s,create)();
    bmath_vfx_s* d = BCATU(bmath_vfx_s,create)();

    BCATU(bmath_mfx_s,set_size)( a, o->rows, o->cols );
    BCATU(bmath_mfx_s,set_size)( u, o->rows, n );
    BCATU(bmath_mfx_s,set_size)( v, o->cols, n );
    BCATU(bmath_vfx_s,set_size)( d, n );

    // o = uT * a * v; o^-1 = vT * (a^-1)T * u
    bl_t success = BCATU(bmath_mfx_s,svd)( u, a, v );
    BCATU(bmath_mfx_s,get_dag_vec)( a, d );

    // diagonal elements are sorted in descending manner
    fx_t s_max = BCATU(fx,max)( BCATU(fx,abs)( d->data[ 0 ] ), BCATU(fx,abs)( d->data[ n - 1 ] ) );
    fx_t thr = s_max * eps;
    if( thr == 0 ) thr = 1.0;

    // pseudo invert diagonal
    for( uz_t i = 0; i < n; i++ ) d->data[ i ] = ( fabs( d->data[ i ] ) < thr ) ? 0 : 1.0 / d->data[ i ];

    for( uz_t i = 0; i < res->rows; i++ )
    {
        fx_t* ri = res->data + res->stride * i;
        fx_t* vi =   v->data +   v->stride * i;
        for( uz_t j = 0; j < res->cols; j++ )
        {
            fx_t* uj = u->data + u->stride * j;
            ri[ j ] = BCATU(bmath,fx,t_vec,mul3_vec)( vi, uj, d->data, n );
        }
    }

    BCATU(bmath_vfx_s,discard)( d );
    BCATU(bmath_mfx_s,discard)( v );
    BCATU(bmath_mfx_s,discard)( u );
    BCATU(bmath_mfx_s,discard)( a );

    return success;
}

//----------------------------------------------------------------------------------------------------------------------

// pseudo inversion of a symmetric matrix
bl_t BCATU(bmath_mfx_s,hsm_piv)( const bmath_mfx_s* o, fx_t eps, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size_to)( res, buf );
        bl_t success = BCATU(bmath_mfx_s,hsm_piv)( o, eps, buf );
        BCATU(bmath_mfx_s,cpy)( buf, res );
        BCATU(bmath_mfx_s,discard)( buf );
        return success;
    }

    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, res ) );
    ASSERT( o->rows == o->cols );

    uz_t n = o->rows;
    if( n == 0 ) return true;

    bmath_mfx_s* a = res; // a occupies space of res
    bmath_mfx_s* q = BCATU(bmath_mfx_s,create)();
    BCATU(bmath_mfx_s,set_size)( q, n, n );
    BCATU(bmath_mfx_s,one)( q );

    BCATU(bmath_mfx_s,cpy)( o, a );
    bl_t success = BCATU(bmath_mfx_s,evd_htp)( a, q );

    bmath_vfx_s* dag = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,set_size)( dag, n );
    BCATU(bmath_mfx_s,get_dag_vec)( a, dag );

    // dag is sorted in descending manner
    fx_t s_max = BCATU(fx,max)( BCATU(fx,abs)( dag->data[ 0 ] ), BCATU(fx,abs)( dag->data[ n - 1 ] ) );
    fx_t thr = s_max * eps;
    if( thr == 0 ) thr = 1.0;

    for( uz_t i = 0; i < n; i++ ) dag->data[ i ] = ( BCATU(fx,abs)( dag->data[ i ] ) < thr ) ? 0 : ( 1.0 / dag->data[ i ] );

    BCATU(bmath_mfx_s,htp)( q, q );
    BCATU(bmath_mfx_s,mul_udu_htp)( q, dag, res );

    BCATU(bmath_vfx_s,discard)( dag );
    BCATU(bmath_mfx_s,discard)( q );

    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,inv_av1)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size_to)( res, buf );
        bl_t success = BCATU(bmath_mfx_s,inv_av1)( o, buf );
        BCATU(bmath_mfx_s,cpy)( buf, res );
        BCATU(bmath_mfx_s,discard)( buf );
        return success;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, res ) );

    bmath_mfx_s o_sub = BCATU(bmath_mfx_s,get_weak_sub_mat)(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mfx_s r_sub = BCATU(bmath_mfx_s,get_weak_sub_mat)( res, 0, 0, res->rows, res->cols - 1 );

    bl_t success = BCATU(bmath_mfx_s,inv)( &o_sub, &r_sub );

    uz_t n = o_sub.rows;

    for( uz_t i = 0; i < n; i++ )
    {
        fx_t sum = 0;
        fx_t* row = r_sub.data + i * r_sub.stride;
        for( uz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
        res->data[ i * res->stride + n ] = -sum;
    }

    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,pdf_inv_av1)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size_to)( res, buf );
        bl_t success = BCATU(bmath_mfx_s,pdf_inv_av1)( o, buf );
        BCATU(bmath_mfx_s,cpy)( buf, res );
        BCATU(bmath_mfx_s,discard)( buf );
        return success;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, res ) );

    bmath_mfx_s o_sub = BCATU(bmath_mfx_s,get_weak_sub_mat)(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mfx_s r_sub = BCATU(bmath_mfx_s,get_weak_sub_mat)( res, 0, 0, res->rows, res->cols - 1 );

    bl_t success = BCATU(bmath_mfx_s,pdf_inv)( &o_sub, &r_sub );

    uz_t n = o_sub.rows;

    for( uz_t i = 0; i < n; i++ )
    {
        fx_t sum = 0;
        fx_t* row = r_sub.data + i * r_sub.stride;
        for( uz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
        res->data[ i * res->stride + n ] = -sum;
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,hsm_piv_av1)( const bmath_mfx_s* o, fx_t eps, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size_to)( res, buf );
        bl_t success = BCATU(bmath_mfx_s,hsm_piv_av1)( o, eps, buf );
        BCATU(bmath_mfx_s,cpy)( buf, res );
        BCATU(bmath_mfx_s,discard)( buf );
        return success;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, res ) );

    bmath_mfx_s o_sub = BCATU(bmath_mfx_s,get_weak_sub_mat)(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mfx_s r_sub = BCATU(bmath_mfx_s,get_weak_sub_mat)( res, 0, 0, res->rows, res->cols - 1 );

    bl_t success = BCATU(bmath_mfx_s,hsm_piv)( &o_sub, eps, &r_sub );

    uz_t n = o_sub.rows;

    for( uz_t i = 0; i < n; i++ )
    {
        fx_t sum = 0;
        fx_t* row = r_sub.data + i * r_sub.stride;
        for( uz_t j = 0; j < n; j++ ) sum += row[ j ] * o->data[ j * o->stride + n ];
        res->data[ i * res->stride + n ] = -sum;
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,div)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    bl_t success = BCATU(bmath_mfx_s,inv)( op, res );
    BCATU(bmath_mfx_s,mul)( o, res, res );
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* v, bmath_vfx_s* r )
{
    if( r == v )
    {
        bmath_vfx_s* buf = BCATU(bmath_vfx_s,create_set_size)( r->size );
        BCATU(bmath_mfx_s,mul_vec)( o, v, buf );
        BCATU(bmath_vfx_s,cpy)( buf, r );
        BCATU(bmath_vfx_s,discard)( buf );
        return;
    }

    ASSERT( o->cols == v->size );
    ASSERT( o->rows == r->size );
    fx_t* vr = r->data;
    fx_t* v2 = v->data;
    for( uz_t i = 0; i < r->size; i++ )
    {
        vr[ i ] = BCATU(bmath,fx,t_vec,mul_vec)( o->data + i * o->stride, v2, o->cols );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,htp_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* v, bmath_vfx_s* r )
{
    if( r == v )
    {
        bmath_vfx_s* buf = BCATU(bmath_vfx_s,create_set_size)( r->size );
        BCATU(bmath_mfx_s,htp_mul_vec)( o, v, buf );
        BCATU(bmath_vfx_s,cpy)( buf, r );
        BCATU(bmath_vfx_s,discard)( buf );
        return;
    }

    ASSERT( o->rows == v->size );
    ASSERT( o->cols == r->size );
    BCATU(bmath_vfx_s,zro)( r );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,fx,t_vec,mul_scl_add)( o->data + i * o->stride, v->data[ i ], r->data, r->data, o->cols );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_vec_add)( const bmath_mfx_s* o, const bmath_vfx_s* v, const bmath_vfx_s* b, bmath_vfx_s* r )
{
    if( r == v )
    {
        bmath_vfx_s* buf = BCATU(bmath_vfx_s,create_set_size)( r->size );
        BCATU(bmath_mfx_s,mul_vec_add)( o, v, b, buf );
        BCATU(bmath_vfx_s,cpy)( buf, r );
        BCATU(bmath_vfx_s,discard)( buf );
        return;
    }

    if( b != r ) BCATU(bmath_vfx_s,cpy)( b, r );

    ASSERT( o->cols == v->size );
    ASSERT( o->rows == r->size );
    fx_t* vr = r->data;
    fx_t* v2 = v->data;
    for( uz_t i = 0; i < r->size; i++ )
    {
        vr[ i ] += BCATU(bmath,fx,t_vec,mul_vec)( o->data + i * o->stride, v2, o->cols );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,htp_mul_vec_add)( const bmath_mfx_s* o, const bmath_vfx_s* v, const bmath_vfx_s* b, bmath_vfx_s* r )
{
    if( r == v )
    {
        bmath_vfx_s* buf = BCATU(bmath_vfx_s,create_set_size)( r->size );
        BCATU(bmath_mfx_s,htp_mul_vec_add)( o, v, b, buf );
        BCATU(bmath_vfx_s,cpy)( buf, r );
        BCATU(bmath_vfx_s,discard)( buf );
        return;
    }

    ASSERT( o->rows == v->size );
    ASSERT( o->cols == r->size );

    if( b != r ) BCATU(bmath_vfx_s,cpy)( b, r );

    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,fx,t_vec,mul_scl_add)( o->data + i * o->stride, v->data[ i ], r->data, r->data, o->cols );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_av1)( const bmath_mfx_s* o, const bmath_vfx_s* av1, bmath_vfx_s* res )
{
    if( res == av1 )
    {
        bmath_vfx_s* buf = BCATU(bmath_vfx_s,create_set_size)( res->size );
        BCATU(bmath_mfx_s,mul_av1)( o, av1, buf );
        BCATU(bmath_vfx_s,cpy)( buf, res );
        BCATU(bmath_vfx_s,discard)( buf );
        return;
    }

    uz_t n = av1->size;
    ASSERT( o->cols == n + 1 );
    ASSERT( o->rows == res->size );
    fx_t* vr = res->data;
    fx_t* v2 = av1->data;
    for( uz_t i = 0; i < res->size; i++ )
    {
        fx_t* v1 = o->data + i * o->stride;
        vr[ i ] = BCATU(bmath,fx,t_vec,mul_vec)( v1, v2, n ) + v1[ n ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_scl)( const bmath_mfx_s* o, const fx_t* b_, bmath_mfx_s* d_ )
{
    ASSERT( o->rows == d_->rows );
    ASSERT( o->cols == d_->cols );
    fx_t b = *b_;

    if( BCATU(bmath_mfx_s,is_folded)( d_ ) )
    {
        ASSERT( o->stride == d_->stride );
        sz_t size = ( o->rows - 1 ) * o->stride + o->cols;
        const fx_t* a = o ->data;
              fx_t* d = d_->data;
        for( sz_t i = 0; i < size; i++ ) d[ i ] = a[ i ] * b;
    }
    else
    {
        for( sz_t i = 0; i < o->rows; i++ )
        {
            fx_t* a = o ->data + i * o ->stride;
            fx_t* d = d_->data + i * d_->stride;
            for( sz_t j = 0; j < o->cols; j++ ) d[ j ] = a[ j ] * b;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_scl_add)( const bmath_mfx_s* o, const fx_t* b_, const bmath_mfx_s* c_, bmath_mfx_s* d_ )
{
    ASSERT( o ->rows == d_->rows );
    ASSERT( o ->cols == d_->cols );
    ASSERT( c_->rows == d_->rows );
    ASSERT( c_->cols == d_->cols );
    fx_t b = *b_;

    if( BCATU(bmath_mfx_s,is_folded)( d_ ) )
    {
        ASSERT( o ->stride == d_->stride );
        ASSERT( c_->stride == d_->stride );
        sz_t size = ( o->rows - 1 ) * o->stride + o->cols;
        const fx_t* a = o ->data;
        const fx_t* c = c_->data;
              fx_t* d = d_->data;
        for( sz_t i = 0; i < size; i++ ) d[ i ] = a[ i ] * b + c[ i ];
    }
    else
    {
        for( sz_t i = 0; i < o->rows; i++ )
        {
            fx_t* a = o ->data + i * o ->stride;
            fx_t* c = c_->data + i * c_->stride;
            fx_t* d = d_->data + i * d_->stride;
            for( sz_t j = 0; j < o->cols; j++ ) d[ j ] = a[ j ] * b + c[ j ];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,eop_map)( const bmath_mfx_s* o, BCATU(bmath_fp,fx,ar1) b, bmath_mfx_s* r )
{
    ASSERT( o ->rows == r->rows );
    ASSERT( o ->cols == r->cols );

    if( BCATU(bmath_mfx_s,is_folded)( r ) )
    {
        ASSERT( o->stride == r->stride );
        sz_t size = ( o->rows - 1 ) * o->stride + o->cols;
        const fx_t* a = o->data;
              fx_t* c = r->data;
        for( sz_t i = 0; i < size; i++ ) c[ i ] = b( a[ i ] );
    }
    else
    {
        for( sz_t i = 0; i < o->rows; i++ )
        {
            const fx_t* a = o->data + i * o->stride;
                  fx_t* c = r->data + i * r->stride;
            for( sz_t j = 0; j < o->cols; j++ ) c[ j ] = b( a[ j ] );
        }
    }

}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,eop_map_mul)( const bmath_mfx_s* o, BCATU(bmath_fp,fx,ar1) b, const bmath_mfx_s* c_, bmath_mfx_s* r )
{
    ASSERT( o ->rows == r->rows );
    ASSERT( o ->cols == r->cols );
    ASSERT( c_->rows == r->rows );
    ASSERT( c_->cols == r->cols );

    if( BCATU(bmath_mfx_s,is_folded)( r ) )
    {
        ASSERT( o ->stride == r->stride );
        ASSERT( c_->stride == r->stride );
        sz_t size = ( o->rows - 1 ) * o->stride + o->cols;
        const fx_t* a = o ->data;
        const fx_t* c = c_->data;
              fx_t* d = r ->data;
        for( sz_t i = 0; i < size; i++ ) d[ i ] = b( a[ i ] ) * c[ i ];
    }
    else
    {
        for( sz_t i = 0; i < o->rows; i++ )
        {
            const fx_t* a = o ->data + i * o ->stride;
            const fx_t* c = c_->data + i * c_->stride;
                  fx_t* d = r ->data + i * r ->stride;
            for( sz_t j = 0; j < o->cols; j++ ) d[ j ] = b( a[ j ] ) * c[ j ];
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s* BCATU(bmath_mfx_s,htp_eval)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    if( o->rows == o->cols )
    {
        // in-place algorithm
        ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, res ) );
        uz_t n = o->rows;
        for( uz_t i = 0; i < n; i++ )
        {
            const fx_t* voi =   o->data + i *   o->stride;
                  fx_t* vri = res->data + i * res->stride;
            for( uz_t j = 0; j < i; j++ )
            {
                const fx_t* voj =   o->data + j *   o->stride;
                      fx_t* vrj = res->data + j * res->stride;

                fx_t tmp = voi[ j ];
                vri[ j ] = voj[ i ];
                vrj[ i ] = tmp;
            }
        }

        // copy diag
        if( o != res ) for( uz_t i = 0; i < n; i++ ) res->data[ i * res->stride + i ] = o->data[ i * o->stride + i ];
        return res;
    }

    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, res->cols, res->rows );
        BCATU(bmath_mfx_s,htp)( o, buf );
        BCATU(bmath_mfx_s,cpy)( buf, res );
        BCATU(bmath_mfx_s,discard)( buf );
        return res;
    }

    ASSERT( o->rows == res->cols );
    ASSERT( o->cols == res->rows );

    for( uz_t i = 0; i < res->rows; i++ )
    {
              fx_t* vr = res->data + i * res->stride;
        const fx_t* v1 = o ->data  + i;
        for( uz_t j = 0; j < res->cols; j++ ) vr[ j ] = v1[ j * o->stride ];
    }
    return res;
}

//----------------------------------------------------------------------------------------------------------------------

/// transpose a block of rows == cols == n in place
static void BCATU(mfx,block_htp)( fx_t* data, sz_t stride, sz_t n )
{
    for( sz_t i = 0; i < n; i++ )
    {
        fx_t* vi = data + i * stride;
        for( sz_t j = 0; j < i; j++ ) BCATU(fx_t,swap)( vi + j, data + j * stride + i );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// swaps two blocks of rows == cols == n
static void BCATU(mfx,block_swap_htp)( fx_t* data1, fx_t* data2, sz_t stride, sz_t n )
{
    for( sz_t i = 0; i < n; i++ )
    {
        for( sz_t j = 0; j < n; j++ ) BCATU(fx_t,swap)( data1 + i * stride + j, data2 + j * stride + i );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// copies a block of rows == cols == n
static void BCATU(mfx,block_copy_htp)( const fx_t* src, sz_t src_stride, fx_t* dst, sz_t dst_stride, sz_t n )
{
    for( sz_t i = 0; i < n; i++ )
    {
        for( sz_t j = 0; j < n; j++ ) ( dst + j * dst_stride )[ i ] = ( src + i * src_stride )[ j ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

#undef BMATH_HTP_BLOCK_SIZE
#if BMATH_TEMPLATE_FX_PREC == 2
    #define BMATH_HTP_BLOCK_SIZE 32
#elif BMATH_TEMPLATE_FX_PREC == 3
    #define BMATH_HTP_BLOCK_SIZE 32
#endif // BMATH_TEMPLATE_FX_PREC

//----------------------------------------------------------------------------------------------------------------------

/// block optimized transposition; o == r (in-place transposition) is allowed when matrix is square
bmath_mfx_s* BCATU(bmath_mfx_s,htp)( const bmath_mfx_s* o, bmath_mfx_s* r )
{
    ASSERT( o->rows == r->cols );
    ASSERT( o->cols == r->rows );

    /// swap the upper left square matrix
    sz_t n = sz_min( o->rows, o->cols );
    {
        // copy square section to r
        if( o != r )
        {
            for( sz_t i = 0; i < n; i++ )
            {
                const fx_t* vo = o->data + i * o->stride;
                      fx_t* vr = r->data + i * r->stride;
                for( sz_t j = 0; j < n; j++ ) vr[ j ] = vo[ j ];
            }
        }

        // htp square section in place
        sz_t i = 0;
        for( ; i <= n - BMATH_HTP_BLOCK_SIZE; i += BMATH_HTP_BLOCK_SIZE )
        {
            for( sz_t j = 0; j < i; j += BMATH_HTP_BLOCK_SIZE )
            {
                BCATU(mfx,block_swap_htp)( r->data + i * r->stride + j, r->data + j * r->stride + i, r->stride, BMATH_HTP_BLOCK_SIZE );
            }
            BCATU(mfx,block_htp)( r->data + i * r->stride + i, r->stride, BMATH_HTP_BLOCK_SIZE );
        }

        for( ; i < n; i++ )
        {
            for( sz_t j = 0; j < i; j++ ) BCATU(fx_t,swap)( r->data + i * r->stride + j, r->data + j * r-> stride + i );
        }
    }

    /// copy and transpose the non-square section (this implies o != r)
    if( o->rows > o->cols )
    {
        sz_t o_rows = o->rows; // conversion to sz_t
        sz_t o_cols = o->cols; // conversion to sz_t
        sz_t i = n;
        for( ; i <= o_rows - BMATH_HTP_BLOCK_SIZE; i += BMATH_HTP_BLOCK_SIZE )
        {
            sz_t j = 0;
            for( ; j <= o_cols - BMATH_HTP_BLOCK_SIZE; j += BMATH_HTP_BLOCK_SIZE )
            {
                BCATU(mfx,block_copy_htp)( o->data + i * o->stride + j, o->stride, r->data + j * r->stride + i, r->stride, BMATH_HTP_BLOCK_SIZE );
            }
            for( ; j < o_cols; j++ ) for( sz_t k = i; k < i + BMATH_HTP_BLOCK_SIZE; k++ ) ( r->data + j * r->stride )[ k ] = ( o->data + k * o->stride )[ j ];
        }

        for( ; i < o_rows; i++ )
        {
            for( sz_t j = 0; j < ( sz_t )o->cols; j++ ) ( r->data + j * r->stride )[ i ] = ( o->data + i * o->stride )[ j ];
        }
    }
    else if( r->rows > r->cols )
    {
        sz_t r_rows = r->rows; // conversion to sz_t
        sz_t r_cols = r->cols; // conversion to sz_t
        sz_t i = n;
        for( ; i <= r_rows - BMATH_HTP_BLOCK_SIZE; i += BMATH_HTP_BLOCK_SIZE )
        {
            sz_t j = 0;
            for( ; j <= r_cols - BMATH_HTP_BLOCK_SIZE; j += BMATH_HTP_BLOCK_SIZE )
            {
                BCATU(mfx,block_copy_htp)( o->data + j * o->stride + i, o->stride, r->data + i * r->stride + j, r->stride, BMATH_HTP_BLOCK_SIZE );
            }
            for( ; j < r_cols; j++ ) for( sz_t k = i; k < i + BMATH_HTP_BLOCK_SIZE; k++ ) ( r->data + k * r->stride )[ j ] = ( o->data + j * o->stride )[ k ];
        }

        for( ; i < r_rows; i++ )
        {
            for( sz_t j = 0; j < r_cols; j++ ) ( r->data + i * r->stride )[ j ] = ( o->data + j * o->stride )[ i ];
        }
    }


    return r;
}

//----------------------------------------------------------------------------------------------------------------------


bmath_mfx_s* BCATU(bmath_mfx_s,pmt_mul)( const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, res->rows, res->cols );
        BCATU(bmath_mfx_s,pmt_mul)( o, p, buf );
        BCATU(bmath_mfx_s,cpy)( buf, res );
        BCATU(bmath_mfx_s,discard)( buf );
        return res;
    }

    ASSERT( o->rows == p->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );

    for( uz_t i = 0; i < p->size; i++ )
    {
        uz_t idx = p->data[ i ];
        ASSERT( idx < o->rows );
        bcore_u_memcpy( sizeof( fx_t ), res->data + idx * res->stride, o->data + i * o->stride, o->cols );
    }
    return res;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s* BCATU(bmath_mfx_s,pmt_htp_mul)( const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, res->rows, res->cols );
        BCATU(bmath_mfx_s,pmt_htp_mul)( o, p, buf );
        BCATU(bmath_mfx_s,cpy)( buf, res );
        BCATU(bmath_mfx_s,discard)( buf );
        return res;
    }

    ASSERT( o->rows == p->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );

    for( uz_t i = 0; i < p->size; i++ )
    {
        uz_t idx = p->data[ i ];
        ASSERT( idx < o->rows );
        bcore_u_memcpy( sizeof( fx_t ), res->data + i * res->stride, o->data + idx * o->stride, o->cols );
    }
    return res;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s* BCATU(bmath_mfx_s,mul_pmt)( const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res )
{
    ASSERT( o->cols == p->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );

    for( uz_t i = 0; i < p->size; i++ ) ASSERT( p->data[ i ] < o->cols );

    if( o == res )
    {
        bmath_vfx_s* vec = BCATU(bmath_vfx_s,create_set_size)( o->cols );
        fx_t* v = vec->data;
        for( uz_t i = 0; i < o->rows; i++ )
        {
            fx_t* ri = res->data + i * res->stride;
            for( uz_t j = 0; j < res->cols; j++ ) v[ j ] = ri[ p->data[ j ] ];
            bcore_u_memcpy( sizeof( fx_t ), ri, v, res->cols );
        }
        BCATU(bmath_vfx_s,discard)( vec );
    }
    else
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            fx_t* oi =   o->data + i *   o->stride;
            fx_t* ri = res->data + i * res->stride;
            for( uz_t j = 0; j < o->cols; j++ ) ri[ j ] = oi[ p->data[ j ] ];
        }
    }
    return res;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s* BCATU(bmath_mfx_s,mul_pmt_htp)( const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res )
{
    ASSERT( o->cols == p->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );

    for( uz_t i = 0; i < p->size; i++ ) ASSERT( p->data[ i ] < o->cols );

    if( o == res )
    {
        bmath_vfx_s* vec = BCATU(bmath_vfx_s,create_set_size)( o->cols );
        fx_t* v = vec->data;
        for( uz_t i = 0; i < o->rows; i++ )
        {
            fx_t* ri = res->data + i * res->stride;
            for( uz_t j = 0; j < res->cols; j++ ) v[ p->data[ j ] ] = ri[ j ];
            bcore_u_memcpy( sizeof( fx_t ), ri, v, res->cols );
        }
        BCATU(bmath_vfx_s,discard)( vec );
    }
    else
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            fx_t* oi =   o->data + i *   o->stride;
            fx_t* ri = res->data + i * res->stride;
            for( uz_t j = 0; j < o->cols; j++ ) ri[ p->data[ j ] ] = oi[ j ];
        }
    }
    return res;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// element-access; col-access; row-access; sub-matrix

void BCATU(bmath_mfx_s,set_row_by_data)( bmath_mfx_s* o, uz_t idx, const fx_t* data )
{
    ASSERT( idx < o->rows );
    bcore_u_memcpy( sizeof( fx_t ), o->data + idx * o->stride, data, o->cols );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,set_col_by_data)( bmath_mfx_s* o, uz_t idx, const fx_t* data )
{
    ASSERT( idx < o->cols );
    fx_t* v = o ->data + idx;
    for( uz_t i = 0; i < o->rows; i++ ) v[ i * o ->stride ] = data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,set_dag_by_data)( bmath_mfx_s* o, const fx_t* data )
{
    uz_t size = uz_min( o->rows, o->cols );
    for( uz_t i = 0; i < size; i++ ) o->data[ i * ( o ->stride + 1 ) ] = data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,set_row_by_vec)( bmath_mfx_s* o, uz_t idx, const bmath_vfx_s* vec )
{
    ASSERT( o->cols <= vec->size );
    BCATU(bmath_mfx_s,set_row_by_data)( o, idx, vec->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,set_col_by_vec)( bmath_mfx_s* o, uz_t idx, const bmath_vfx_s* vec )
{
    ASSERT( o->rows <= vec->size );
    BCATU(bmath_mfx_s,set_col_by_data)( o, idx, vec->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,set_dag_by_vec)( bmath_mfx_s* o, const bmath_vfx_s* vec )
{
    ASSERT( uz_min( o->rows, o->cols ) <= vec->size );
    BCATU(bmath_mfx_s,set_dag_by_data)( o, vec->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,get_row_vec)(  const bmath_mfx_s* o, uz_t idx, bmath_vfx_s* res )
{
    ASSERT( idx < o->rows );
    ASSERT( o->cols <= res->size );
    bcore_u_memcpy( sizeof( fx_t ), res->data, o->data + idx * o->stride, o->cols );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,get_col_vec)(  const bmath_mfx_s* o, uz_t idx, bmath_vfx_s* res )
{
    ASSERT( idx < o->cols );
    ASSERT( o->rows <= res->size );
    const fx_t* src = o->data + idx;
    for( uz_t i = 0; i < o->rows; i++ ) res->data[ i ] = src[ i * o ->stride ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,get_dag_vec)( const bmath_mfx_s* o, bmath_vfx_s* res )
{
    uz_t size = uz_min( o->rows, o->cols );
    ASSERT( size <= res->size );
    for( uz_t i = 0; i < size; i++ ) res->data[ i ] = o->data[ i * ( o->stride + 1 ) ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,swap_row)( bmath_mfx_s* o, uz_t i, uz_t j )
{
    if( i == j ) return;
    ASSERT( i < o->rows );
    ASSERT( j < o->rows );
    fx_t* vi = o->data + i * o->stride;
    fx_t* vj = o->data + j * o->stride;
    for( uz_t k = 0; k < o->cols; k++ ) BCATU(fx,t_swap)( vi + k, vj + k );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,swap_col)( bmath_mfx_s* o, uz_t i, uz_t j )
{
    if( i == j ) return;
    ASSERT( i < o->cols );
    ASSERT( j < o->cols );
    fx_t* vi = o->data + i;
    fx_t* vj = o->data + j;
    for( uz_t k = 0; k < o->rows; k++ ) BCATU(fx,t_swap)( vi + k * o->stride, vj + k * o->stride );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_fx_to_row)( bmath_mfx_s* o, fx_t v, uz_t i )
{
    ASSERT( i < o->rows );
    fx_t* vi = o->data + i * o->stride;
    for( uz_t k = 0; k < o->cols; k++ ) vi[ k ] *= v;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,mul_fx_to_col)( bmath_mfx_s* o, fx_t v, uz_t i )
{
    ASSERT( i < o->cols );
    fx_t* vi = o->data + i;
    for( uz_t k = 0; k < o->rows; k++ ) vi[ k * o->stride ] *= v;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s BCATU(bmath_mfx_s,get_weak_sub_mat)( const bmath_mfx_s* o, uz_t row, uz_t col, uz_t rows, uz_t cols )
{
    ASSERT( row + rows <= o->rows );
    ASSERT( col + cols <= o->cols );
    bmath_mfx_s ret;
    BCATU(bmath_mfx_s,init)( &ret );
    ret.rows = rows;
    ret.cols = cols;
    ret.stride = o->stride;
    ret.data = o->data + row * o->stride + col;
    ret.size = o->size;
    ret.space = 0; // indicates that ret is a weak object. (no shutdown needed)
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vfx_s BCATU(bmath_mfx_s,get_row_weak_vec)( const bmath_mfx_s* o, uz_t idx )
{
    ASSERT( idx < o->rows );
    bmath_vfx_s vec;
    BCATU(bmath_vfx_s,init)( &vec );
    vec.data = o->data + idx * o->stride;
    vec.size = o->cols;
    vec.space = 0;
    return vec;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,decompose_cholesky)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( BCATU(bmath_mfx_s,is_square)( o ) );
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, res ) );

    bl_t success = true;

    sz_t n = o->rows;

    bmath_vfx_s* inv_diag = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,set_size)( inv_diag, n );

    fx_t* q = inv_diag->data;

    BCATU(bmath_mfx_s,cpy)( o, res );

    /* This implementation actively uses the upper triangle of res.
     * It allows a more cache efficient data progression in innermost loops.
     */
    for( sz_t i = 0; i < n; i++ )
    {
        fx_t* vi = res->data + i * res->stride;
        for( sz_t j = 0; j < i; j++ )
        {
            fx_t* vj = res->data + j * res->stride;
            vi[ j ] *= q[ j ];
            vj[ i ] = vi[ j ]; // we keep res symmetric
            f3_t f = -vi[ j ];

            #ifdef BMATH_AVX
                M5_T f_pk = M5_SET_ALL( f );
                sz_t k = j + 1;
                for( ; k <= i - P5_SIZE; k += P5_SIZE ) M5_STOR( vi + k, M5_MUL_ADD( f_pk, M5_LOAD( vj + k ), M5_LOAD( vi + k ) ) );
                for( ; k < i; k++ ) vi[ k ] += f * vj[ k ];
            #else
                for( sz_t k = j + 1; k < i; k++ ) vi[ k ] += f * vj[ k ];
            #endif // BMATH_AVX
        }

        for( sz_t j = 0; j < i; j++ ) vi[ i ] -= BCATU(fx,sqr)( vi[ j ] );

        if( vi[ i ] <= BCATU(fx,lim_min) )
        {
            success = false;
            vi[ i ] = 0;
            q [ i ] = 0;
        }
        else
        {
            vi[ i ] = sqrt( vi[ i ] );
            q [ i ] = ( 1.0 / vi[ i ] );
        }
    }

    /* Zeroing upper triangle.
     * This guarantees the equality: o = res * res^T.
     * Typical use cases do not require that equality.
     * Since the footprint is negligible, we do it anyway.
     */
    for( sz_t i = 0; i < n; i++ )
    {
        fx_t* vi = res->data + i * res->stride;
        for( sz_t j = i + 1; j < n; j++ ) vi[ j ] = 0;
    }

    BCATU(bmath_vfx_s,discard)( inv_diag );
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,decompose_luc)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // o == res is allowed
    ASSERT( BCATU(bmath_mfx_s,is_square)( o ) );
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, res ) );
    sz_t n = o->cols;
    sz_t stride = res->stride;
    bl_t success = true;

    BCATU(bmath_mfx_s,cpy)( o, res );

    bmath_vfx_s* inv_diag = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,set_size)( inv_diag, res->rows );

    fx_t* q = inv_diag->data;

    for( sz_t i = 0; i < n; i++ )
    {
        fx_t* vi = res->data + i * stride;

        for( sz_t j = 0; j < i; j++ )
        {
            fx_t* vj = res->data + j * stride;
            for( sz_t k = j + 1; k < n; k++ ) vi[ k ] -= vi[ j ] * q[ j ] * vj[ k ];
        }

        for( sz_t j = 0; j < i; j++ ) vi[ j ] *= q[ j ];

        fx_t denom = vi[ i ];
        if( BCATU(fx,abs)( denom ) <= BCATU(fx,lim_min) )
        {
            success = false;
            q[ i ] = 0;
        }
        else
        {
            q[ i ] = 1.0 / denom;
        }
    }

    BCATU(bmath_vfx_s,discard)( inv_diag );
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,ltr_inv)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( BCATU(bmath_mfx_s,is_square)( o ) );
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, res ) );

    BCATU(bmath_mfx_s,cpy)( o, res );

    sz_t n = res->rows;

    bl_t success = true;

    bmath_vfx_s* buf = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,set_size)( buf, n );
    fx_t* q = buf->data;

    for( sz_t i = 0; i < n; i++ )
    {
        fx_t* vi = res->data + i * res->stride;

        for( sz_t j = 0; j < i; j++ ) q[ j ] = 0;
        for( sz_t j = 0; j < i; j++ )
        {
            fx_t* vj = res->data + j * res->stride;
            #ifdef BMATH_AVX
                M5_T f_pk = M5_SET_ALL( -vi[ j ] );
                sz_t k = 0;
                for( ; k <= j - P5_SIZE + 1; k += P5_SIZE ) M5_STOR( q + k, M5_MUL_ADD( f_pk, M5_LOAD( vj + k ), M5_LOAD( q + k ) ) );
                for( ; k <= j; k++ ) q[ k ] -= vi[ j ] * vj[ k ];
            #else
                for( sz_t k = 0; k <= j; k++ ) q[ k ] -= vi[ j ] * vj[ k ];
            #endif // BMATH_AVX
        }

        bl_t stable = ( BCATU(fx,abs)( vi[ i ] ) > BCATU(fx,lim_min) );
        vi[ i ] = stable ? 1.0 / vi[ i ] : 0;
        success = success && stable;

        for( sz_t j = 0; j < i; j++ ) vi[ j ] = q[ j ] * vi[ i ];
    }

    BCATU(bmath_vfx_s,discard)( buf );

    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,ltr_inv_htp)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    bl_t success = BCATU(bmath_mfx_s,ltr_inv)( o, res );
    sz_t n = res->rows;

    // transpose result
    for( sz_t i = 0; i < n; i++ )
    {
        fx_t* vi = res->data + i * res->stride;
        for( sz_t j = 0; j < i; j++ )
        {
            fx_t* vj = res->data + j * res->stride;
            vj[ i ] = vi[ j ];
            vi[ j ] = 0;
        }
    }

    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_mfx_s,pdf_inv)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // Method: Combine cholesky decomposition with fast inversion of a triangular matrix.
    // O    =  R * R^T
    // O^-1 = (R * R^T)^-1 = R^T^-1 * R^-1 = (R^-1)^T * (R^-1)

    ASSERT( BCATU(bmath_mfx_s,is_hsm)( o ) );
    bl_t success = BCATU(bmath_mfx_s,decompose_cholesky)( o, res ); // res <- R
    success = success & BCATU(bmath_mfx_s,ltr_inv_htp)( res, res ); // res <- R^-1^T
    BCATU(bmath_mfx_s,utr_mul_htp)( res, res );                     // res <- o^-1 = R^-1^T * R^-1

    return success;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Triangular decompositions, operations and solvers

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,ltr_mul_htp)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( BCATU(bmath_mfx_s,is_square)( o ) );
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, res ) );
    uz_t n = o->rows;

    // compute off diagonal elements; store in upper triangle of res
    for( uz_t i = 0; i < n; i++ )
    {
        const fx_t* voi = o->data + i * o->stride;
        for( uz_t j = 0; j < i; j++ )
        {
            const fx_t* voj =   o->data + j *   o->stride;
                  fx_t* vrj = res->data + j * res->stride;
            vrj[ i ] = BCATU(bmath,fx,t_vec,mul_vec)( voi, voj, j + 1 );
        }
    }

    // diagonal elements in reverse order
    for( uz_t i = n - 1; i < n; i-- )
    {
        const fx_t* voi =   o->data + i *   o->stride;
              fx_t* vri = res->data + i * res->stride;

        vri[ i ] = BCATU(bmath,fx,t_vec,mul_vec)( voi, voi, i + 1 );
    }

    // copy lower off-diagonal from upper off-diagonal
    for( uz_t i = 0; i < n; i++ )
    {
        fx_t* vri = res->data + i * res->stride;
        for( uz_t j = 0; j < i; j++ ) vri[ j ] = res->data[ j * res->stride + i ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,utr_mul_htp)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( BCATU(bmath_mfx_s,is_square)( o ) );
    ASSERT( BCATU(bmath_mfx_s,is_equ_size)( o, res ) );
    sz_t n = o->rows;

    BCATU(bmath_mfx_s,cpy)( o, res );

    bmath_vfx_s* buf = BCATU(bmath_vfx_s,create)();
    BCATU(bmath_vfx_s,set_size)( buf, n );
    fx_t* q = buf->data;

    // copy upper off-diagonal to lower off-diagonal
    for( sz_t i = 0; i < n; i++ )
    {
        fx_t* vi = res->data + i * res->stride;
        for( sz_t j = 0; j < i; j++ )
        {
            fx_t* vj = res->data + j * res->stride;
            vi[ j ] = vj[ i ];
        }
    }

    for( sz_t i = 0; i < n; i++ )
    {
        fx_t* vi = res->data + i * o->stride;
        for( sz_t j = i; j < n; j++ ) q[ j ] = 0;

        for( sz_t j = i; j < n; j++ )
        {
            fx_t* vj = res->data + j * res->stride;
            #ifdef BMATH_AVX
                sz_t k = i;
                M5_T f_pk = M5_SET_ALL( vi[ j ] );
                for( ; k <= j - P5_SIZE + 1; k += P5_SIZE ) M5_STOR( q + k, M5_MUL_ADD( f_pk, M5_LOAD( vj + k ), M5_LOAD( q + k ) ) );
                for( ; k <= j; k++ ) q[ k ] += vi[ j ] * vj[ k ];
            #else
                for( sz_t k = i; k <= j; k++ ) q[ k ] += vi[ j ] * vj[ k ];
            #endif // BMATH_AVX
        }

        for( sz_t j = i; j < n; j++ ) vi[ j ] = q[ j ];
    }

    // copy upper off-diagonal to lower off-diagonal
    for( sz_t i = 0; i < n; i++ )
    {
        fx_t* vi = res->data + i * res->stride;
        for( sz_t j = 0; j < i; j++ )
        {
            fx_t* vj = res->data + j * res->stride;
            vi[ j ] = vj[ i ];
        }
    }

    BCATU(bmath_vfx_s,discard)( buf );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,ltr_mul_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    for( uz_t i = o->rows - 1; i < o->rows; i-- )
    {
        fx_t* v1 = o->data + i * o->stride;
        res[ i ] = BCATU(bmath,fx,t_vec,mul_vec)( v1, op, i + 1 );
    }
}

void BCATU(bmath_mfx_s,ltr_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath_mfx_s,ltr_mul_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,lt1_mul_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    for( uz_t i = o->rows - 1; i < o->rows; i-- )
    {
        fx_t* v1 = o->data + i * o->stride;
        res[ i ] = BCATU(bmath,fx,t_vec,mul_vec)( v1, op, i ) + op[ i ];
    }
}

void BCATU(bmath_mfx_s,lt1_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath_mfx_s,lt1_mul_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,utr_mul_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        fx_t* v1 = o->data + i * o->stride;
        res[ i ] = BCATU(bmath,fx,t_vec,mul_vec)( v1 + i, op + i, o->cols - i );
    }
}

void BCATU(bmath_mfx_s,utr_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath_mfx_s,utr_mul_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,luc_mul_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    BCATU(bmath_mfx_s,utr_mul_vec_)( o,  op, res );
    BCATU(bmath_mfx_s,lt1_mul_vec_)( o, res, res );
}

void BCATU(bmath_mfx_s,luc_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath_mfx_s,luc_mul_vec_)( o,  op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,ltr_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath_mfx_s,ltr_mul_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,lt1_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath_mfx_s,lt1_mul_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,utr_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath_mfx_s,utr_mul_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,luc_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath_mfx_s,luc_mul_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,ltr_solve_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    if( op != res ) bcore_u_memcpy( sizeof( fx_t ), res, op, o->rows );
    uz_t n = o->rows;
    for( uz_t i = 0; i < n; i++ )
    {
        const fx_t* voi = o->data + i * o->stride;
        fx_t sum = BCATU(bmath,fx,t_vec,mul_vec)( voi, res, i );
        res[ i ] = ( voi[ i ] != 0 ) ? ( res[ i ] - sum ) / voi[ i ] : 0;
    }
}

void BCATU(bmath_mfx_s,ltr_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath_mfx_s,ltr_solve_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,utr_solve_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    if( op != res ) bcore_u_memcpy( sizeof( fx_t ), res, op, o->rows );
    uz_t n = o->rows;
    for( uz_t i = n - 1; i < n; i-- )
    {
        const fx_t* voi = o->data + i * o->stride;
        fx_t sum = BCATU(bmath,fx,t_vec,mul_vec)( voi + i + 1, res + i + 1, n - i - 1 );
        res[ i ] = ( voi[ i ] != 0 ) ? ( res[ i ] - sum ) / voi[ i ] : 0;
    }
}

void BCATU(bmath_mfx_s,utr_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath_mfx_s,utr_solve_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,lt1_solve_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    if( op != res ) bcore_u_memcpy( sizeof( fx_t ), res, op, o->rows );
    uz_t n = o->rows;
    for( uz_t i = 0; i < n; i++ )
    {
        const fx_t* voi = o->data + i * o->stride;
        res[ i ] -= BCATU(bmath,fx,t_vec,mul_vec)( voi, res, i );
    }
}

void BCATU(bmath_mfx_s,lt1_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath_mfx_s,lt1_solve_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,luc_solve_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    BCATU(bmath_mfx_s,lt1_solve_vec_)( o,  op, res );
    BCATU(bmath_mfx_s,utr_solve_vec_)( o, res, res );
}

void BCATU(bmath_mfx_s,luc_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath_mfx_s,luc_solve_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,ltr_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath_mfx_s,ltr_solve_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,lt1_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath_mfx_s,lt1_solve_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,utr_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath_mfx_s,utr_solve_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,luc_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath_mfx_s,luc_solve_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Other decompositions

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,trd_htp)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    ASSERT( a->rows == a->cols );

    uz_t n = a->rows;

    if( n <= 2 )
    {
        if( v ) BCATU(bmath_mfx_s,one)( v );
        return; // nothing else to do
    }

    if( v )
    {
        ASSERT( v->cols == n );
        ASSERT( v->rows == n );
        ASSERT( v != a );
    }

    bmath_arr_grt_fx_s grv = BCATU(bmath_arr_grt,fx,of_size)( n );
    bmath_grt_fx_s gr;

    for( uz_t j = 0; j < n; j++ )
    {
        // zero lower column
        for( uz_t k = n - 2; k > j; k-- )
        {
            uz_t l = k + 1;
            BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, &a->data[ k * a->stride + j ], &a->data[ l * a->stride + j ] );
            a->data[ l * a->stride + j ] = BCATU(bmath_grt,fx,s,rho)( &gr );
            BCATU(bmath_mfx_s,arow_rotate)( a, k, &gr, j + 1, l + 1 );
            fx_t* a00 = a->data + k * ( a->stride + 1 );
            a00[ -a->stride ] = a00[ -1 ];
            BCATU(bmath_grt,fx,s,rotate)( &gr, a00, a00 + 1 );
            grv.data[ l - 1 ] = gr;
        }

        if( BCATU(bmath_arr_grt,fx,s,density)( &grv, j + 1, n - 1 ) < 0.25 )
        {
            // sparse column operations
            for( uz_t k = n - 2; k > j; k-- )
            {
                gr = grv.data[ k ];
                BCATU(bmath_mfx_s,acol_rotate)( a, k, &gr, k + 1, n );
                if( k < n - 2 ) a->data[ ( k + 1 ) * ( a->stride + 1 ) + 1 ] = a->data[ ( k + 2 ) * ( a->stride + 1 ) - 1 ];
            }
        }
        else
        {
            // row swipes
            for( uz_t k = n - 1; k > j; k-- )
            {
                BCATU(bmath_mfx_s,arow_swipe_rev)( a, k, &grv, j + 1, k );
                if( k < n - 1 ) a->data[ k * ( a->stride + 1 ) + 1 ] = a->data[ ( k + 1 ) * ( a->stride + 1 ) - 1 ];
            }
        }
    }

    if( v )
    {
        BCATU(bmath_mfx_s,one)( v );
        for( uz_t j = 0; j < n; j++ )
        {
            for( uz_t l = n - 1; l > j + 1; l-- )
            {
                fx_t rho = a->data[ l * a->stride + j ];
                BCATU(bmath_grt,fx,s,init_from_rho)( &gr, rho );
                BCATU(bmath_mfx_s,arow_rotate)( v, l - 1, &gr, l - j - 1, n );
            }
        }
    }

    // set off-tri diag elements zero
    for( uz_t i = 0; i < n; i++ )
    {
        fx_t* ai = a->data + i * a->stride;
        if( i > 1 ) for( uz_t j = 0; j < i - 1; j++ ) ai[ j ] = 0;
        for( uz_t j = i + 2; j < n; j++ ) ai[ j ] = 0;
    }

    // symmetrize off-diag
    for( uz_t i = 0; i < n - 1; i++ )
    {
        fx_t* ai = a->data + i * ( a->stride + 1 );
        fx_t b = ( ai[ 1 ] + ai[ a->stride ] ) * 0.5;
        ai[ 1 ] = ai[ a->stride ] = b;
    }

    BCATU(bmath_arr_grt,fx,s,down)( &grv );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,trd)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    BCATU(bmath_mfx_s,trd_htp)( a, v );
    if( v ) BCATU(bmath_mfx_s,htp)( v, v );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,ubd_to_lbd)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    uz_t n = uz_min( a->rows, a->cols );

    if( n <= 1 ) return; // nothing to do

    if( v ) ASSERT( v->cols >= n );

    bmath_arr_grt_fx_s grv = BCATU(bmath_arr_grt,fx,of_size)( n );

    for( uz_t j = 0; j < n - 1; j++ )
    {
        fx_t* aj = a->data + j * ( a->stride + 1 );
        BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &grv.data[ j ], aj, aj + 1 );
        BCATU(bmath_grt,fx,s,rotate)( &grv.data[ j ], aj + a->stride, aj + a->stride + 1 );
    }

    if( v ) BCATU(bmath_mfx_s,sweep_acol_rotate_fwd)( v, 0, n - 1, &grv, 0, v->rows );

    BCATU(bmath_arr_grt,fx,s,down)( &grv );
}

//----------------------------------------------------------------------------------------------------------------------

// v transposed
void BCATU(bmath_mfx_s,ubd_to_lbd_htp)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    uz_t n = uz_min( a->rows, a->cols );

    if( n <= 1 ) return; // nothing to do

    if( v ) ASSERT( v->rows >= n );

    bmath_grt_fx_s gr;

    for( uz_t j = 0; j < n - 1; j++ )
    {
        fx_t* aj = a->data + j * ( a->stride + 1 );
        BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, aj, aj + 1 );
        BCATU(bmath_grt,fx,s,rotate)( &gr, aj + a->stride, aj + a->stride + 1 );
        if( v ) BCATU(bmath_mfx_s,arow_rotate)( v, j, &gr, 0, v->cols );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,lbd_to_ubd)( bmath_mfx_s* u, bmath_mfx_s* a )
{
    uz_t n = uz_min( a->rows, a->cols );

    if( n <= 1 ) return; // nothing to do

    if( u ) ASSERT( u->cols >= n );

    bmath_arr_grt_fx_s gru = BCATU(bmath_arr_grt,fx,of_size)( n );

    for( uz_t j = 0; j < n - 1; j++ )
    {
        fx_t* aj = a->data + j * ( a->stride + 1 );
        BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gru.data[ j ], aj, aj + a->stride );
        BCATU(bmath_grt,fx,s,rotate)( &gru.data[ j ], aj + 1, aj + a->stride + 1 );
    }

    if( u ) BCATU(bmath_mfx_s,sweep_acol_rotate_fwd)( u, 0, n - 1, &gru, 0, u->rows );

    BCATU(bmath_arr_grt,fx,s,down)( &gru );
}

//----------------------------------------------------------------------------------------------------------------------

// u transposed
void BCATU(bmath_mfx_s,lbd_to_ubd_htp)( bmath_mfx_s* u, bmath_mfx_s* a )
{
    uz_t n = uz_min( a->rows, a->cols );

    if( n <= 1 ) return; // nothing to do

    if( u ) ASSERT( u->rows >= n );

    bmath_grt_fx_s gr;

    for( uz_t j = 0; j < n - 1; j++ )
    {
        fx_t* aj = a->data + j * ( a->stride + 1 );
        BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, aj, aj + a->stride );
        BCATU(bmath_grt,fx,s,rotate)( &gr, aj + 1, aj + a->stride + 1 );
        if( u ) BCATU(bmath_mfx_s,arow_rotate)( u, j, &gr, 0, u->cols );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,ubd)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v )
{
    if( a->rows < a->cols )
    {
        BCATU(bmath_mfx_s,lbd)( u, a, v );
        BCATU(bmath_mfx_s,lbd_to_ubd)( u, a );
        return;
    }

    ASSERT( a->rows >= a->cols );

    /// at this point: a->rows >= a->cols;

    if( u )
    {
        BCATU(bmath_mfx_s,one)( u );
        ASSERT( u != a );
        ASSERT( u->rows == a->rows );
        ASSERT( u->cols == a->rows /*full*/ || u->cols == a->cols /*thin*/  ); // u may be full or thin (nothing in-between)
    }

    if( v )
    {
        BCATU(bmath_mfx_s,one)( v );
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
            BCATU(bmath_mfx_s,qrd)( u, a );

            uz_t u_cols = u->cols;

            a->rows = a->cols;
            bmath_mfx_s* u2 = BCATU(bmath_mfx_s,create)();
            BCATU(bmath_mfx_s,set_size)( u2, a->rows, a->rows );
            BCATU(bmath_mfx_s,ubd)( u2, a, v );
            u->cols = u2->rows;

            BCATU(bmath_mfx_s,htp)( u2, u2 );

            BCATU(bmath_mfx_s,mul_htp)( u, u2, u );
            BCATU(bmath_mfx_s,discard)( u2 );

            u->cols = u_cols;
            a->rows = u_cols;
        }
        else
        {
            BCATU(bmath_mfx_s,qrd)( NULL, a );
            uz_t a_rows = a->rows;
            a->rows = a->cols;
            BCATU(bmath_mfx_s,ubd)( NULL, a, v );
            a->rows = a_rows;
        }
        return;
    }

    bmath_arr_grt_fx_s grv = BCATU(bmath_arr_grt,fx,of_size)( a->cols );
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

        // zero upper row
        for( uz_t l = a->cols - 1; l > j + 1; l-- )
        {
            BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, &a->data[ j * a->stride + j + 1 ], &a->data[ j * a->stride + l ] );
            if( v ) a->data[ j * a->stride + l ] = BCATU(bmath_grt,fx,s,rho)( &gr );
            grv.data[ l - 1 ] = gr;
        }

        BCATU(bmath_mfx_s,sweep_dcol_rotate_rev)( a, j + 1, a->cols - 1, &grv, j + 1, a->rows );
    }

    if( v ) // reverse construction of v
    {
        for( uz_t j = a->cols - 1; j < a->cols; j-- )
        {
            for( uz_t k = j + 1; k < a->cols - 1; k++ )
            {
                fx_t rho = 0;
                BCATU(fx,t_swap)( &a->data[ j * a->stride + k + 1 ], &rho );
                BCATU(bmath_grt,fx,s,init_from_rho)( &gr, -rho );
                BCATU(bmath_mfx_s,drow_rotate)( v, j + 1, k + 1, &gr, j, v->cols );
            }
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

    BCATU(bmath_arr_grt,fx,s,down)( &grv );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,lbd)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v )
{
    if( a->cols < a->rows )
    {
        BCATU(bmath_mfx_s,ubd)( u, a, v );
        BCATU(bmath_mfx_s,ubd_to_lbd)( a, v );
        return;
    }

    /// at this point: a->cols >= a->rows;

    if( u )
    {
        BCATU(bmath_mfx_s,one)( u );
        ASSERT( u->cols == a->rows );
        ASSERT( u->rows == a->rows );
        ASSERT( u != a );
        ASSERT( u != v );
    }

    if( v )
    {
        BCATU(bmath_mfx_s,one)( v );
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
            BCATU(bmath_mfx_s,lqd)( a, v );

            uz_t v_cols = v->cols;

            a->cols = a->rows;
            bmath_mfx_s* v2 = BCATU(bmath_mfx_s,create)();
            BCATU(bmath_mfx_s,set_size)( v2, a->cols, a->cols );
            BCATU(bmath_mfx_s,lbd)( u, a, v2 );
            v->cols = v2->rows;

            BCATU(bmath_mfx_s,htp)( v2, v2 );

            BCATU(bmath_mfx_s,mul_htp)( v, v2, v );
            BCATU(bmath_mfx_s,discard)( v2 );

            v->cols = v_cols;
            a->cols = v_cols;
        }
        else
        {
            BCATU(bmath_mfx_s,lqd)( a, NULL );
            uz_t a_cols = a->cols;
            a->cols = a->rows;
            BCATU(bmath_mfx_s,lbd)( u, a, NULL );
            a->cols = a_cols;
        }
        return;
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

        // zero lower column
        for( uz_t l = a->rows - 1; l > j + 1; l-- )
        {
            BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, &a->data[ ( j + 1 ) * a->stride + j ], &a->data[ l * a->stride + j ] );
            if( u ) a->data[ l * a->stride + j ] = BCATU(bmath_grt,fx,s,rho)( &gr );
            BCATU(bmath_mfx_s,drow_rotate)( a, j + 1, l, &gr, j + 1, a->cols );
        }
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

    if( u ) // reverse construction of u
    {
        for( uz_t j = a->rows - 2; j < a->rows; j-- )
        {
            for( uz_t k = j + 1; k < a->rows - 1; k++ )
            {
                fx_t rho = 0;
                BCATU(fx,t_swap)( &a->data[ j + a->stride * ( k + 1 ) ], &rho );
                BCATU(bmath_grt,fx,s,init_from_rho)( &gr, -rho );
                BCATU(bmath_mfx_s,drow_rotate)( u, j + 1, k + 1, &gr, j, u->cols );
            }
        }
    }

    BCATU(bmath_arr_grt,fx,s,down)( &grv );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Covariance

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,set_covariance_on_section_fast)( bmath_mfx_s* o, bmath_arr_vfx_s* arr_vec, uz_t start, uz_t end )
{
    end = end < arr_vec->size ? end : arr_vec->size;
    start = start > end ? end : start;

    BCATU(bmath_mfx_s,zro)( o );
    if( start == end ) return;
    uz_t n = arr_vec->data[ start ].size;
    ASSERT( o->rows == n );
    ASSERT( o->cols == n );

    bmath_vfx_s* avg = BCATU(bmath_vfx_s,create_set_size)( n );
    bmath_vfx_s* vec = BCATU(bmath_vfx_s,create_set_size)( n );

    BCATU(bmath_arr,vfx,s,on_section_get_avg)( arr_vec, start, end, avg );

    for( uz_t i = start; i < end; i++ )
    {
        BCATU(bmath_vfx_s,sub)( &arr_vec->data[ i ], avg, vec );
        BCATU(bmath_mfx_s,opd_add)( o, vec, vec, o );
    }

    fx_t f = 1.0 / ( end - start );
    BCATU(bmath_mfx_s,mul_scl_fx)( o, f, o );

    BCATU(bmath_vfx_s,discard)( avg );
    BCATU(bmath_vfx_s,discard)( vec );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,set_covariance_on_section_sprc)( bmath_mfx_s* o, bmath_arr_vfx_s* arr_vec, uz_t start, uz_t end )
{
    end = end < arr_vec->size ? end : arr_vec->size;
    start = start > end ? end : start;

    BCATU(bmath_mfx_s,zro)( o );
    if( start == end ) return;
    uz_t n = arr_vec->data[ start ].size;
    ASSERT( o->rows == n );
    ASSERT( o->cols == n );

    bmath_vfx_s* avg = BCATU(bmath_vfx_s,create_set_size)( n );
    BCATU(bmath_arr,vfx,s,on_section_get_avg_sprc)( arr_vec, start, end, avg );

    for( uz_t i = 0; i < n; i++ )
    {
        for( uz_t j = 0; j <= i; j++ )
        {
            fx_t v = BCATU(bmath_arr,vfx,s,on_section_fx_sum_coprd_sprec)( arr_vec, start, end, avg->data[ i ], avg->data[ j ], i, j );
            v /= (end - start);
            o->data[ i * o->stride + j ] = v;
            o->data[ j * o->stride + i ] = v;
        }
    }

    BCATU(bmath_vfx_s,discard)( avg );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Givens Rotations

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,sweep_arow_rotate_fwd)( bmath_mfx_s* o, uz_t row_start, uz_t row_end, const bmath_arr_grt_fx_s* grt, uz_t col_start, uz_t col_end )
{
    assert( grt->size >= row_end - 1 );
    for( uz_t i = row_start; i < row_end; i++ ) BCATU(bmath_mfx_s,arow_rotate)( o, i, &grt->data[ i ], col_start, col_end );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,sweep_arow_rotate_rev)( bmath_mfx_s* o, uz_t row_start, uz_t row_end, const bmath_arr_grt_fx_s* grt, uz_t col_start, uz_t col_end )
{
    assert( grt->size >= row_end - 1 );
    for( uz_t i = row_end; i > row_start; i-- ) BCATU(bmath_mfx_s,arow_rotate)( o, i - 1, &grt->data[ i - 1 ], col_start, col_end );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,sweep_acol_rotate_fwd)( bmath_mfx_s* o, uz_t col_start, uz_t col_end, const bmath_arr_grt_fx_s* grt, uz_t row_start, uz_t row_end )
{
    assert( grt->size >= col_end - 1 );
    if( BCATU(bmath_arr_grt,fx,s,density)( grt, col_start, col_end ) < 0.25 )
    {
        // sparse rotations: rotate columns individually
        for( uz_t i = col_start; i < col_end; i++ ) BCATU(bmath_mfx_s,acol_rotate)( o, i, &grt->data[ i ], row_start, row_end );
    }
    else
    {
        // dense rotations: use cache efficient row-sweeps
        for( uz_t i = row_start; i < row_end; i++ ) BCATU(bmath_mfx_s,arow_swipe_fwd)( o, i, grt, col_start, col_end );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,sweep_dcol_rotate_rev)( bmath_mfx_s* o, uz_t col_start, uz_t col_end, const bmath_arr_grt_fx_s* grt, uz_t row_start, uz_t row_end )
{
    assert( grt->size >= col_end - 1 );
    if( BCATU(bmath_arr_grt,fx,s,density)( grt, col_start, col_end ) < 0.25 )
    {
        // sparse rotations: rotate columns individually
        for( uz_t i = col_end; i > col_start; i-- ) BCATU(bmath_mfx_s,dcol_rotate)( o, col_start, i, &grt->data[ i - 1 ], row_start, row_end );
    }
    else
    {
        // dense rotations: use cache efficient row-sweeps
        uz_t i    = row_start;
        sz_t size = col_end - col_start;
        bmath_grt_fx_s* g = grt->data + col_start;
        fx_t* r0  = o->data + col_start;

        for( ; i + 4 < row_end; i += 4 ) BCATU(bmath_simd,fx,4drow_swipe_rev)( r0 + o->stride * i, o->stride, g, size );
        for( ; i < row_end    ; i++    ) BCATU(bmath_simd,fx,drow_swipe_rev)(  r0 + o->stride * i,            g, size );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,to_sink)( const bmath_mfx_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "(#<uz_t> x #<uz_t>)\n", o->rows, o->cols );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ )
        {
            if( j > 0 ) bcore_sink_a_push_fa( sink, " " );
            bcore_sink_a_pushf( sink, "%9.3g", v[ j ] );
        }
        bcore_sink_a_push_fa( sink, "\n" );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,to_image)( const bmath_mfx_s* o, BCATU(bmath_fp_u2_argb_from,fx) fp, vd_t arg, bcore_img_u2_s* image )
{
    bcore_img_u2_s_set_size( image, o->rows, o->cols );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        for( sz_t j = 0; j < o->cols; j++ )
        {
            bcore_img_u2_s_set_pixel( image, i, j, fp( arg, o->data[ i * o->stride + j ] ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Convolution

void BCATU(bmath_mfx_s,to_conv_operand)( bmath_mfx_s* o, sz_t kernel_size, sz_t step_size )
{
    // Make sure matrix is not already a convolution operand
    if( o->stride < o->cols ) ERR_fa( "Matrix is already a convolution operand." );

    sz_t operand_size = o->rows * o->cols;

    if( kernel_size > operand_size ) ERR_fa( "Kernel is too large." );

    sz_t conv_steps = ( ( operand_size - kernel_size ) / step_size ) + 1;

    // We require a full coverage at this point;
    if( ( conv_steps - 1 ) * step_size + kernel_size != operand_size ) ERR_fa( "step_size does not yield full coverage." );

    o->stride = step_size;
    o->cols   = kernel_size;
    o->rows   = conv_steps;
}

/**********************************************************************************************************************/
// type conversion

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,copy_typed)( bmath_mfx_s* o, tp_t type, vc_t src )
{
    if( !src )
    {
        BCATU(bmath_mfx_s,clear)( o );
        return;
    }

    switch( type )
    {
        case TYPEOF_bmath_mfx_s:
        {
            BCATU(bmath_mfx_s,copy)( o, src );
        }
        break;

        case TYPEOF_bmath_mfy_s:
        {
            const bmath_mfy_s* v = src;
            BCATU(bmath_mfx_s,set_size)( o, v->rows, v->cols );
            for( sz_t i = 0; i < o->rows; i++ )
            {
                const fy_t* v_fy = &v->data[ v->stride * i ];
                      fx_t* o_fx = &o->data[ o->stride * i ];
                for( sz_t j = 0; j < o->cols; j++ ) o_fx[ j ] = v_fy[ j ];
            }
        }
        break;

        default:
        {
            bcore_err_fa( "Cannot copy from #<sc_t>.", ifnameof( type ) );
        }
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    BLM_INIT();

    bmath_mfx_s* m1 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* m2 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* m3 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* m4 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* m5 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* m6 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );

    bmath_vfx_s* v1 = BLM_A_PUSH( BCATU(bmath_vfx_s,create)() );
    bmath_vfx_s* v2 = BLM_A_PUSH( BCATU(bmath_vfx_s,create)() );
    bmath_vfx_s* v3 = BLM_A_PUSH( BCATU(bmath_vfx_s,create)() );

    bmath_arr_vfx_s* a1 = BLM_A_PUSH( BCATU(bmath_arr,vfx,s,create)() );

    // basic linear
    {
        BCATU(bmath_mfx_s,set_size)( m1, 3, 2 );
        BCATU(bmath_mfx_s,set_fx)( m1, 0, 0, 1 );
        BCATU(bmath_mfx_s,set_fx)( m1, 0, 1, 2 );
        BCATU(bmath_mfx_s,set_fx)( m1, 1, 0, 3 );
        BCATU(bmath_mfx_s,set_fx)( m1, 1, 1, 4 );
        BCATU(bmath_mfx_s,set_fx)( m1, 2, 0, 5 );
        BCATU(bmath_mfx_s,set_fx)( m1, 2, 1, 6 );
        BCATU(bmath_mfx_s,set_size)( m2, 2, 3 );
        BCATU(bmath_mfx_s,htp)( m1, m2 );
        BCATU(bmath_mfx_s,set_size)( m3, 2, 2 );
        BCATU(bmath_mfx_s,set_size)( m4, 2, 2 );
        BCATU(bmath_mfx_s,mul)( m2, m1, m3 );

        ASSERT( BCATU(bmath_mfx_s,get_fx)( m3, 0, 0 ) == 35 );
        ASSERT( BCATU(bmath_mfx_s,get_fx)( m3, 0, 1 ) == 44 );
        ASSERT( BCATU(bmath_mfx_s,get_fx)( m3, 1, 0 ) == 44 );
        ASSERT( BCATU(bmath_mfx_s,get_fx)( m3, 1, 1 ) == 56 );
        bmath_matrix_a_mul( ( bmath_matrix* )m2, ( bmath_matrix* )m1, ( bmath_matrix* )m4 );
        ASSERT( BCATU(bmath_mfx_s,is_equ)( m3, m4 ) );

        BCATU(bmath_vfx_s,set_size)( v1, 2 );
        BCATU(bmath_vfx_s,set_size)( v2, 3 );
        BCATU(bmath_vfx_s,set_fx)( v1, 0, 1 );
        BCATU(bmath_vfx_s,set_fx)( v1, 1, 2 );
        BCATU(bmath_mfx_s,mul_vec)( m1, v1, v2 );
        ASSERT( BCATU(bmath_vfx_s,get_fx)( v2, 0 ) ==  5 );
        ASSERT( BCATU(bmath_vfx_s,get_fx)( v2, 1 ) == 11 );
        ASSERT( BCATU(bmath_vfx_s,get_fx)( v2, 2 ) == 17 );

        BCATU(bmath_mfx_s,htp_mul_vec)( m1, v2, v1 );
        ASSERT( BCATU(bmath_vfx_s,get_fx)( v1, 0 ) == 123 );
        ASSERT( BCATU(bmath_vfx_s,get_fx)( v1, 1 ) == 156 );

        BCATU(bmath_mfx_s,htp_mul_vec_add)( m1, v2, v1, v1 );
        ASSERT( BCATU(bmath_vfx_s,get_fx)( v1, 0 ) == 123 * 2 );
        ASSERT( BCATU(bmath_vfx_s,get_fx)( v1, 1 ) == 156 * 2 );
    }

    // multi-kernel convolution
    {
        sz_t operand_size = 1000;

        sz_t kernel_size  = 100;
        sz_t step_size    = 2;
        sz_t kernels      = 10;

        // operand: initially single-row matrix of operand_size
        BCATU(bmath_mfx_s,set_size)( m1, 1, operand_size );
        for( sz_t i = 0; i < m1->cols; i++ ) m1->data[ i ] = i;
        //BCATU(bmath_mfx_s,to_stdout)( m1 );

        // turn to convolution-operand without moving data
        BCATU(bmath_mfx_s,to_conv_operand)( m1, kernel_size, step_size );

        // set up kernel
        BCATU(bmath_mfx_s,set_size)( m2, kernel_size, kernels ); // note: kernel vectors are column-vectors
        for( sz_t j = 0; j < kernels; j++ )
        {
            for( sz_t i = 0; i < m2->rows; i++ ) BCATU(bmath_mfx_s,set_fx)( m2, i, j, j + 1 );
        }
        //BCATU(bmath_mfx_s,to_stdout)( m2 );

        // convolution output
        BCATU(bmath_mfx_s,set_size)( m3, m1->rows, m2->cols );
        BCATU(bmath_mfx_s,mul)( m1, m2, m3 );
        //BCATU(bmath_mfx_s,to_stdout)( m3 );

        // convolution result:
        // row: convolution steps
        // col: response of all kernels for a given convolution step
        ASSERT( m3->cols == kernels );
        for( sz_t j = 0; j < m3->rows; j++ )
        {
            for( sz_t i = 0; i < m3->cols; i++ )
            {
                fx_t diff = BCATU(bmath_mfx_s,get_fx)( m3, j, i ) - ( ( ( kernel_size ) * ( kernel_size - 1 ) / 2 ) + kernel_size * j * step_size ) * ( i + 1 );
                //bcore_msg_fa( "#<"BSTR( fx_t )">\n", diff );
                ASSERT( diff == 0 );
            }
        }

    }

    fx_t max_dev = sizeof( fx_t ) == 4 ? 1E-2 : 1E-8;
    fx_t epsilon = sizeof( fx_t ) == 4 ? 1E-5 : 1E-8;

    // lower triangular inversion
    {
        BCATU(bmath_mfx_s,set_size)( m1, 3, 3 );
        BCATU(bmath_mfx_s,set_fx)( m1, 0, 0, 1 ); BCATU(bmath_mfx_s,set_fx)( m1, 0, 1, 0 );  BCATU(bmath_mfx_s,set_fx)( m1, 0, 2, 0 );
        BCATU(bmath_mfx_s,set_fx)( m1, 1, 0, 7 ); BCATU(bmath_mfx_s,set_fx)( m1, 1, 1, 5 );  BCATU(bmath_mfx_s,set_fx)( m1, 1, 2, 0 );
        BCATU(bmath_mfx_s,set_fx)( m1, 2, 0, 1 ); BCATU(bmath_mfx_s,set_fx)( m1, 2, 1, 2 );  BCATU(bmath_mfx_s,set_fx)( m1, 2, 2, 4 );
        BCATU(bmath_mfx_s,set_size)( m2, 3, 3 );
        BCATU(bmath_mfx_s,set_size)( m3, 3, 3 );
        BCATU(bmath_mfx_s,ltr_inv_htp)( m1, m2 );
        BCATU(bmath_mfx_s,mul_htp)( m1, m2, m3 );
        ASSERT( BCATU(bmath_mfx_s,is_near_one)( m3, max_dev ) );
    }

    // A * AT
    {
        uz_t n = 100;
        u3_t rval = 1234;
        BCATU(bmath_mfx_s,set_size)( m1, n, n );
        BCATU(bmath_mfx_s,set_size_to)( m1, m2 );
        BCATU(bmath_mfx_s,set_random_u3)( m1, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,mul_htp)( m1, m1, m2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_hsm)( m2, max_dev ) );
    }


    // covariance
    {
        uz_t size = 1000;
        uz_t n    = 20;
        u3_t rval = 124;
        BCATU(bmath_arr,vfx,s,set_size)( a1, size );
        BCATU(bmath_arr,vfx,s,on_section_set_size)( a1, 0, -1, n );
        BCATU(bmath_arr,vfx,s,on_section_set_random_u3)( a1, 0, -1, 1.0, -1, 1, &rval );

        BCATU(bmath_mfx_s,set_size)( m1, n, n );
        BCATU(bmath_mfx_s,set_size)( m2, n, n );
        BCATU(bmath_vfx_s,set_size)( v1, n );

        BCATU(bmath_mfx_s,set_covariance_on_section_fast)( m1, a1, 0, -1 );
        BCATU(bmath_mfx_s,set_covariance_on_section_sprc)( m2, a1, 0, -1 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( m1, m2, max_dev ) );
        ASSERT( BCATU(fx,abs)( BCATU(bmath_mfx_s,fx_trc)( m1 ) - n * 0.3333 ) < 1E-1 ); // trace should be near n/3

        fx_t sqr_norm = 4.0;
        BCATU(bmath_arr,vfx,s,on_section_set_sqr)( a1, 0, -1, sqr_norm ); // normalize all vectors to sqr_norm
        BCATU(bmath_mfx_s,set_covariance_on_section_fast)( m1, a1, 0, -1 );
        ASSERT( BCATU(fx,abs)( BCATU(bmath_mfx_s,fx_trc)( m1 ) - sqr_norm ) < 1E-2 ); // trace should be near sqr_norm (independent of n)

        BCATU(bmath_arr,vfx,s,on_section_set_sum)( a1, 0, -1, 0 ); // set sum of vector components to 0 (introducing a linear dependence)
        BCATU(bmath_mfx_s,set_covariance_on_section_fast)( m1, a1, 0, -1 );
        BCATU(bmath_mfx_s,evd_htp)( m1, NULL );
        ASSERT( BCATU(fx,abs)( BCATU(bmath_mfx_s,get_fx)( m1, n - 1, n - 1 ) ) < max_dev ); // last eigenvalue should be near zero
    }

    // affine-transformation
    {
        uz_t n = 10;
        BCATU(bmath_mfx_s,set_size)( m1, n, n + 1 );
        BCATU(bmath_mfx_s,set_size)( m2, n, n + 1 );
        BCATU(bmath_vfx_s,set_size)( v1, n );
        BCATU(bmath_vfx_s,set_size)( v2, n );
        BCATU(bmath_vfx_s,set_size)( v3, n );

        u3_t rval = 1243;
        BCATU(bmath_mfx_s,set_random_u3)( m1, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_vfx_s,set_random_u3)( v1, 1.0, -1, 1, &rval );

        BCATU(bmath_mfx_s,mul_av1)( m1, v1, v2 );
        BCATU(bmath_mfx_s,inv_av1)( m1, m2 );
        BCATU(bmath_mfx_s,mul_av1)( m2, v2, v3 );

        ASSERT( BCATU(bmath_vfx_s,is_near_equ)( v1, v3, max_dev ) );
    }

    // hsm-affine-transformation
    {
        uz_t n = 10;
        BCATU(bmath_mfx_s,set_size)( m1, n, n + 1 );
        BCATU(bmath_mfx_s,set_size)( m2, n, n + 1 );
        BCATU(bmath_vfx_s,set_size)( v1, n );
        BCATU(bmath_vfx_s,set_size)( v2, n );
        BCATU(bmath_vfx_s,set_size)( v3, n );

        u3_t rval = 1236;
        BCATU(bmath_mfx_s,set_random_u3)( m1, false, false, 0, 1.0, -1, 1, &rval );

        bmath_mfx_s m1_sub = BCATU(bmath_mfx_s,get_weak_sub_mat)( m1, 0, 0, n, n );
        BCATU(bmath_mfx_s,mul_htp)( &m1_sub, &m1_sub, &m1_sub ); // make nxn submatrix of m1 symmetric

        BCATU(bmath_vfx_s,set_random_u3)( v1, 1.0, -1, 1, &rval );

        BCATU(bmath_mfx_s,mul_av1)( m1, v1, v2 );
        BCATU(bmath_mfx_s,pdf_inv_av1)( m1, m2 );
        BCATU(bmath_mfx_s,mul_av1)( m2, v2, v3 );

        ASSERT( BCATU(bmath_vfx_s,is_near_equ)( v1, v3, max_dev ) );
    }

    // hsm-affine-transformation (test via piv)
    {
        uz_t n = 10;
        BCATU(bmath_mfx_s,set_size)( m1, n, n + 1 );
        BCATU(bmath_mfx_s,set_size)( m2, n, n + 1 );
        BCATU(bmath_vfx_s,set_size)( v1, n );
        BCATU(bmath_vfx_s,set_size)( v2, n );
        BCATU(bmath_vfx_s,set_size)( v3, n );

        u3_t rval = 1236;
        BCATU(bmath_mfx_s,set_random_u3)( m1, false, false, 0, 1.0, -1, 1, &rval );

        bmath_mfx_s m1_sub = BCATU(bmath_mfx_s,get_weak_sub_mat)( m1, 0, 0, n, n );
        BCATU(bmath_mfx_s,mul_htp)( &m1_sub, &m1_sub, &m1_sub ); // make nxn submatrix of m1 symmetric

        BCATU(bmath_vfx_s,set_random_u3)( v1, 1.0, -1, 1, &rval );

        BCATU(bmath_mfx_s,mul_av1)( m1, v1, v2 );
        BCATU(bmath_mfx_s,hsm_piv_av1)( m1, epsilon, m2 );
        BCATU(bmath_mfx_s,mul_av1)( m2, v2, v3 );

        ASSERT( BCATU(bmath_vfx_s,is_near_equ)( v1, v3, max_dev ) );
    }

    // mul_udu_htp
    {
        sz_t ma[ 7 ] = { 10, 20, 10, 10, 10,  1,  0 };
        sz_t na[ 7 ] = { 20, 10, 10,  1,  0, 10, 10 };
        for( sz_t i = 0; i < 7; i++ )
        {
            sz_t m = ma[ i ];
            sz_t n = na[ i ];
            BCATU(bmath_mfx_s,set_size)( m1, m, n );
            BCATU(bmath_mfx_s,set_size)( m2, n, n );
            BCATU(bmath_mfx_s,set_size)( m3, m, n );
            BCATU(bmath_mfx_s,set_size)( m4, m, m );
            BCATU(bmath_mfx_s,set_size)( m5, m, m );
            BCATU(bmath_vfx_s,set_size)( v1, n );

            u3_t rval = 1236;
            BCATU(bmath_mfx_s,set_random_u3)( m1, false, false, 0, 1.0, -1, 1, &rval );
            BCATU(bmath_vfx_s,set_random_u3)( v1,                  1.0, -1, 1, &rval );

            BCATU(bmath_mfx_s,mul_udu_htp)( m1, v1, m5 );

            BCATU(bmath_mfx_s,zro)( m2 );
            BCATU(bmath_mfx_s,set_dag_by_vec)( m2, v1 );
            BCATU(bmath_mfx_s,mul)( m1, m2, m3 );
            BCATU(bmath_mfx_s,mul_htp)( m3, m1, m4 );
            ASSERT( BCATU(bmath_mfx_s,is_near_equ)( m4, m5, max_dev ) );
        }
    }

    // mul_udv_htp
    {
        sz_t ma[ 7 ] = { 10, 20, 10, 10, 10,  1,  0 };
        sz_t na[ 7 ] = { 20, 10, 10,  1,  0, 10, 10 };
        for( sz_t i = 0; i < 7; i++ )
        {
            sz_t m = ma[ i ];
            sz_t n = na[ i ];
            BCATU(bmath_mfx_s,set_size)( m1, m, n );
            BCATU(bmath_mfx_s,set_size)( m2, n, n );
            BCATU(bmath_mfx_s,set_size)( m3, m, n );
            BCATU(bmath_mfx_s,set_size)( m4, m, n );
            BCATU(bmath_mfx_s,set_size)( m5, m, m );
            BCATU(bmath_mfx_s,set_size)( m6, m, m );
            BCATU(bmath_vfx_s,set_size)( v2, n );

            u3_t rval = 1236;
            BCATU(bmath_mfx_s,set_random_u3)( m1, false, false, 0, 1.0, -1, 1, &rval );
            BCATU(bmath_vfx_s,set_random_u3)( v2,                  1.0, -1, 1, &rval );
            BCATU(bmath_mfx_s,set_random_u3)( m3, false, false, 0, 1.0, -1, 1, &rval );

            BCATU(bmath_mfx_s,mul_udv_htp)( m1, v2, m3, m5 );

            BCATU(bmath_mfx_s,zro)( m2 );
            BCATU(bmath_mfx_s,set_dag_by_vec)( m2, v2 );
            BCATU(bmath_mfx_s,mul)( m1, m2, m4 );
            BCATU(bmath_mfx_s,mul_htp)( m4, m3, m6 );
            ASSERT( BCATU(bmath_mfx_s,is_near_equ)( m5, m6, max_dev ) );
        }
    }

    // mul_utv_htp
    {
        sz_t ma[ 7 ] = { 10, 20, 10, 10, 10,  1,  0 };
        sz_t na[ 7 ] = { 20, 10, 10,  1,  0, 10, 10 };
        for( sz_t i = 0; i < 7; i++ )
        {
            sz_t m = ma[ i ];
            sz_t n = na[ i ];
            BCATU(bmath_mfx_s,set_size)( m1, m, n );
            BCATU(bmath_mfx_s,set_size)( m2, n, n );
            BCATU(bmath_mfx_s,set_size)( m3, m, n );
            BCATU(bmath_mfx_s,set_size)( m4, m, n );
            BCATU(bmath_mfx_s,set_size)( m5, m, m );
            BCATU(bmath_mfx_s,set_size)( m6, m, m );

            u3_t rval = 1236;
            BCATU(bmath_mfx_s,set_random_u3)( m1, false, false, 0, 1.0, -1, 1, &rval );
            BCATU(bmath_mfx_s,set_random_u3)( m2, false, false, 0, 1.0, -1, 1, &rval );
            BCATU(bmath_mfx_s,set_random_u3)( m3, false, false, 0, 1.0, -1, 1, &rval );

            for( sz_t i = 0; i < m2->rows; i++ )
            {
                for( sz_t j = 0; j < m2->cols; j++ )
                {
                    if( j > i + 1 || j < i - 1 ) m2->data[ i * m2->stride + j ] = 0;
                }
            }

            BCATU(bmath_mfx_s,mul_utv_htp)( m1, m2, m3, m5 );
            BCATU(bmath_mfx_s,mul)( m1, m2, m4 );
            BCATU(bmath_mfx_s,mul_htp)( m4, m3, m6 );
            ASSERT( BCATU(bmath_mfx_s,is_near_equ)( m5, m6, max_dev ) );
        }
    }

    /// mul_add_cps
    BCATU(bmath_mfx_s,mul_add_cps_selftest)();

    BLM_DOWN();
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

static void eval_test( void )
{
    BLM_INIT();
    bmath_mfx_eval_s* eval = BLM_CREATE( bmath_mfx_eval_s );
    bmath_arr_mfx_eval_s* arr_eval = BLM_CREATE( bmath_arr_mfx_eval_s );

    eval->density = 1.0;
    eval->rows = 20; eval->cols = 20; bmath_arr_mfx_eval_s_push( arr_eval, eval );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,trd)    , ( fp_t )BCATU(bmath_mfx_s,trd));
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,evd_htp), ( fp_t )BCATU(bmath_mfx_s,evd_htp) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,cld)    , ( fp_t )BCATU(bmath_mfx_s,decompose_cholesky) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,lud)    , ( fp_t )BCATU(bmath_mfx_s,decompose_luc) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,inv)    , ( fp_t )BCATU(bmath_mfx_s,inv) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,pdf_inv), ( fp_t )BCATU(bmath_mfx_s,pdf_inv) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,hsm_piv), ( fp_t )BCATU(bmath_mfx_s,hsm_piv) );

    eval->thin_decomposition = true;
    eval->rows = 10; eval->cols = 30; bmath_arr_mfx_eval_s_push( arr_eval, eval );
    eval->rows = 30; eval->cols = 10; bmath_arr_mfx_eval_s_push( arr_eval, eval );
    eval->thin_decomposition = false;
    eval->rows = 10; eval->cols = 30; bmath_arr_mfx_eval_s_push( arr_eval, eval );
    eval->rows = 30; eval->cols = 10; bmath_arr_mfx_eval_s_push( arr_eval, eval );

    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,mul),         ( fp_t )BCATU(bmath_mfx_s,mul) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,mul_htp),     ( fp_t )BCATU(bmath_mfx_s,mul_htp) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,htp_mul),     ( fp_t )BCATU(bmath_mfx_s,htp_mul) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,htp_mul_htp), ( fp_t )BCATU(bmath_mfx_s,htp_mul_htp) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,qrd),     ( fp_t )BCATU(bmath_mfx_s,qrd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,qrd_pmt), ( fp_t )BCATU(bmath_mfx_s,qrd_pmt) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,lqd),     ( fp_t )BCATU(bmath_mfx_s,lqd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,pmt_lqd), ( fp_t )BCATU(bmath_mfx_s,pmt_lqd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,ubd),     ( fp_t )BCATU(bmath_mfx_s,ubd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,lbd),     ( fp_t )BCATU(bmath_mfx_s,lbd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,svd),     ( fp_t )BCATU(bmath_mfx_s,svd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,piv),     ( fp_t )BCATU(bmath_mfx_s,piv) );

    BLM_DOWN();
}

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

