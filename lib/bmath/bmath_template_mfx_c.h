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

/** Matrix Template. (For use in *.c files)
 *
 *  Example:
 *
 *  #define BMATH_TEMPLATE_FX_PREC 2 // defines all functions for f2_t
 *  #include "bmath_template_vfx_c.h"
 */

/**********************************************************************************************************************/

// NOTE: Do not include non-templates (template definitions could be undone/changed)
#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,set_size)( bmath_mfx_s* o, uz_t rows, uz_t cols )
{
    bcore_matrix_a_set_size( ( bcore_matrix* )o, rows, cols );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,set_random)( bmath_mfx_s* o, bl_t hsm, bl_t pdf, uz_t rd, fx_t density, fx_t min, fx_t max, u2_t* p_rval )
{
    u2_t rval = p_rval ? *p_rval : 12345;
    fx_t range = max - min;
    if( pdf ) ASSERT( hsm );
    if( hsm ) ASSERT( o->rows == o->cols );

    if( rd == 0 ) // full rank
    {
        if( pdf )
        {
            BCATU(bmath,mfx,s,set_random)( o, false, false, 0, density, min, max, &rval );
            BCATU(bmath,mfx,s,mul_htp)( o, o, o );
        }
        else if( hsm )
        {
            for( uz_t i = 0; i < o->rows; i++ )
            {
                fx_t* vi = o->data + i * o->stride;
                for( uz_t j = 0; j <= i; j++ )
                {
                    fx_t* vj = o->data + j * o->stride;
                    if( BCATU(fx,xsg1_pos)( &rval ) < density )
                    {
                        vi[ j ] = ( range * BCATU(fx,xsg1_pos)( &rval ) ) + min;
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
                    if( BCATU(fx,xsg1_pos)( &rval ) < density )
                    {
                        v[ j ] = ( range * BCATU(fx,xsg1_pos)( &rval ) ) + min;
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
            bmath_mfx_s* m1 = BCATU(bmath,mfx,s,create)();
            bmath_vfx_s* v1 = BCATU(bmath,vfx,s,create)();
            BCATU(bmath,mfx,s,set_size)( m1, o->rows, rank );
            BCATU(bmath,mfx,s,set_random)( m1, false, false, 0, density, min, max, &rval );

            BCATU(bmath,vfx,s,set_size)( v1, rank );
            if( pdf )
            {
                BCATU(bmath,vfx,s,set_random)( v1, density, 0.0, 1.0, &rval );
            }
            else
            {
                BCATU(bmath,vfx,s,set_random)( v1, density, min, max, &rval );
            }

            BCATU(bmath,mfx,s,mul_udu_htp)( m1, v1, o );
            BCATU(bmath,mfx,s,discard)( m1 );
            BCATU(bmath,vfx,s,discard)( v1 );
        }
        else
        {
            bmath_mfx_s* m1 = BCATU(bmath,mfx,s,create)();
            bmath_mfx_s* m2 = BCATU(bmath,mfx,s,create)();
            BCATU(bmath,mfx,s,set_size)( m1, o->rows, rank );
            BCATU(bmath,mfx,s,set_size)( m2, o->cols, rank );
            BCATU(bmath,mfx,s,set_random)( m1, false, false, 0, density, min, max, &rval );
            BCATU(bmath,mfx,s,set_random)( m2, false, false, 0, density, min, max, &rval );
            BCATU(bmath,mfx,s,mul_htp)( m1, m2, o );
            BCATU(bmath,mfx,s,discard)( m1 );
            BCATU(bmath,mfx,s,discard)( m2 );
        }
    }

    if( p_rval ) *p_rval = rval;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s* bmath_mfx_s_create_set_size( uz_t rows, uz_t cols )
{
    bmath_mfx_s* o = BCATU(bmath,mfx,s,create)();
    BCATU(bmath,mfx,s,set_size)( o, rows, cols );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// checks, deviations

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath,mfx,s,is_near_equ)( const bmath_mfx_s* o, const bmath_mfx_s* op, fx_t max_dev )
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

bl_t BCATU(bmath,mfx,s,is_near_zro)( const bmath_mfx_s* o, fx_t max_dev )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ ) if( BCATU(fx,abs)( v1[ j ] ) > max_dev ) return false;
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath,mfx,s,is_near_one)( const bmath_mfx_s* o, fx_t max_dev )
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

bl_t BCATU(bmath,mfx,s,is_near_dag)( const bmath_mfx_s* o, fx_t max_dev )
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

bl_t BCATU(bmath,mfx,s,is_near_trd)( const bmath_mfx_s* o, fx_t max_dev )
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

bl_t BCATU(bmath,mfx,s,is_near_ubd)( const bmath_mfx_s* o, fx_t max_dev )
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

bl_t BCATU(bmath,mfx,s,is_near_lbd)( const bmath_mfx_s* o, fx_t max_dev )
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

bl_t BCATU(bmath,mfx,s,is_near_otn)( const bmath_mfx_s* o, fx_t max_dev )
{
    if( o->rows <= o->cols )
    {
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

bl_t BCATU(bmath,mfx,s,is_near_uni)( const bmath_mfx_s* o, fx_t max_dev )
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

bl_t BCATU(bmath,mfx,s,is_near_utr)( const bmath_mfx_s* o, fx_t max_dev )
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

bl_t BCATU(bmath,mfx,s,is_near_ltr)( const bmath_mfx_s* o, fx_t max_dev )
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

bl_t BCATU(bmath,mfx,s,is_near_hsm)( const bmath_mfx_s* o, fx_t max_dev )
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

bl_t BCATU(bmath,mfx,s,is_nan)( const bmath_mfx_s* o )
{
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* v1 = o ->data + i * o ->stride;
        for( uz_t j = 0; j < o->cols; j++ ) if( BCATU(fx,is_nan)( v1[ j ] ) ) return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,mfx,s,fdev_equ)( const bmath_mfx_s* o, const bmath_mfx_s* op )
{
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == op->cols );

    fx_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* ai = o->data + i * o->stride;
        const fx_t* bi = op->data + i * op->stride;
        for( uz_t j = 0; j < o->cols; j++ ) sum += BCATU(fx,sqr)( ai[ j ] - bi[ j ] );
    }
    return ( sum > 0 ) ? BCATU(fx,srt)( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,mfx,s,fdev_zro)( const bmath_mfx_s* o )
{
    fx_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* oi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ ) sum += BCATU(fx,sqr)( oi[ j ] );
    }
    return ( sum > 0 ) ? BCATU(fx,srt)( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,mfx,s,fdev_one)( const bmath_mfx_s* o )
{
    fx_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* oi = o->data + i * o->stride;
        for( uz_t j = 0; j < o->cols; j++ ) sum += BCATU(fx,sqr)( oi[ j ] - ( i == j ? 1 : 0 ) );
    }
    return ( sum > 0 ) ? BCATU(fx,srt)( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,mfx,s,fdev_otn)( const bmath_mfx_s* o )
{
    fx_t sum = 0;
    if( o->rows <= o->cols )
    {
        for( uz_t i = 0; i < o->rows; i++ )
        {
            const fx_t* v1 = o ->data + i * o ->stride;
            for( uz_t j = i; j < o->rows; j++ )
            {
                const fx_t* v2 = o ->data + j * o ->stride;
                fx_t dot_prd = 0;
                for( uz_t k = 0; k < o->cols; k++ ) dot_prd += v1[ k ] * v2[ k ];
                sum += BCATU(fx,sqr)( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) );
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
                sum += BCATU(fx,sqr)( dot_prd - ( ( j == i ) ? 1.0 : 0.0 ) );
            }
        }
    }
    return ( sum > 0 ) ? BCATU(fx,srt)( sum ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// initializations; copying; basic matrix operations

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,mfx,s,fx_max)( const bmath_mfx_s* o )
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

fx_t BCATU(bmath,mfx,s,fx_min)( const bmath_mfx_s* o )
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

fx_t BCATU(bmath,mfx,s,fx_sum)( const bmath_mfx_s* o )
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

fx_t BCATU(bmath,mfx,s,fx_trc)( const bmath_mfx_s* o )
{
    ASSERT( o->cols == o->rows );
    fx_t sum = 0;
    for( uz_t i = 0; i < o->rows; i++ ) sum += o->data[ i * ( o->stride + 1 ) ];
    return sum;
}

//----------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath,mfx,s,fx_sub_sqr)( const bmath_mfx_s* o, const bmath_mfx_s* op )
{
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, op ) );
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

void BCATU(bmath,mfx,s,add)( const bmath_mfx_s* o, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, b ) );
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, r ) );
    if( BCATU(bmath,mfx,s,is_folded)( r ) )
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

void BCATU(bmath,mfx,s,sub)( const bmath_mfx_s* o, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, b ) );
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, r ) );
    if( BCATU(bmath,mfx,s,is_folded)( r ) )
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

void BCATU(bmath,mfx,s,zro)( bmath_mfx_s* o )
{
    if( BCATU(bmath,mfx,s,is_folded)( o ) )
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
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,one)( bmath_mfx_s* o )
{
    if( BCATU(bmath,mfx,s,is_folded)( o ) )
    {
        BCATU(bmath,mfx,s,zro)( o );
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
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,neg)( const bmath_mfx_s* o, bmath_mfx_s* r )
{
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, r ) );

    if( BCATU(bmath,mfx,s,is_folded)( r ) )
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
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,cpy)( const bmath_mfx_s* o, bmath_mfx_s* r )
{
    if( r == o ) return;
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, r ) );
    if( BCATU(bmath,mfx,s,is_folded)( r ) )
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
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,opd)( bmath_mfx_s* o, const bmath_vfx_s* a, const bmath_vfx_s* b )
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

void BCATU(bmath,mfx,s,opd_add)( bmath_mfx_s* o, const bmath_vfx_s* a, const bmath_vfx_s* b, const bmath_mfx_s* c )
{
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, c ) );
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

void BCATU(bmath,mfx,s,mul_hdm)( const bmath_mfx_s* a, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    ASSERT(  BCATU(bmath,mfx,s,is_equ_size)( a, b ) );
    ASSERT(  BCATU(bmath,mfx,s,is_equ_size)( a, r ) );
    ASSERT( !BCATU(bmath,mfx,s,is_folded)( r ) );

    for( sz_t i = 0; i < a->rows; i++ )
    {
        const fx_t* va = a->data + i * a->stride;
        const fx_t* vb = b->data + i * b->stride;
              fx_t* vr = r->data + i * r->stride;
        for( sz_t j = 0; j < a->cols; j++ ) vr[ j ] = va[ j ] * vb[ j ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath,mfx,s,inv_htp_luc)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, res ) );
    bmath_mfx_s* luc = BCATU(bmath,mfx,s,create)();
    BCATU(bmath,mfx,s,set_size_to)( o, luc );

    bl_t success = BCATU(bmath,mfx,s,decompose_luc)( o, luc );
    BCATU(bmath,mfx,s,one)( res );
    BCATU(bmath,mfx,s,luc_solve_htp_htp)( luc, res, res );
    BCATU(bmath,mfx,s,discard)( luc );
    return success;
}

bl_t BCATU(bmath,mfx,s,inv_htp)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    return BCATU(bmath,mfx,s,inv_htp_luc)( o, res );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath,mfx,s,inv)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    bl_t success = BCATU(bmath,mfx,s,inv_htp)( o, res );
    BCATU(bmath,mfx,s,htp)( res, res );
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

// pseudo inversion
bl_t BCATU(bmath,mfx,s,piv)( const bmath_mfx_s* o, fx_t eps, bmath_mfx_s* res )
{
    ASSERT( o->rows == res->cols );
    ASSERT( o->cols == res->rows );

    uz_t n = uz_min( o->rows, o->cols );
    if( n == 0 ) return true;

    bmath_mfx_s* a = BCATU(bmath,mfx,s,create)();
    // we let 'a' use the space of res
    if( o != res )
    {
        a->data   = res->data;
        a->rows   = o->rows;
        a->cols   = o->cols;
        a->stride = a->cols;
        a->size   = a->rows * a->cols;
        a->space  = 0; // a does not own its space
        BCATU(bmath,mfx,s,cpy)( o, a );
    }
    else
    {
        *a = *res;
        a->space = 0; // a does not own its space
    }

    bmath_mfx_s* u = BCATU(bmath,mfx,s,create)();
    bmath_mfx_s* v = BCATU(bmath,mfx,s,create)();
    bmath_vfx_s* d = BCATU(bmath,vfx,s,create)();

    BCATU(bmath,mfx,s,set_size)( a, o->rows, o->cols );
    BCATU(bmath,mfx,s,set_size)( u, o->rows, n );
    BCATU(bmath,mfx,s,set_size)( v, o->cols, n );
    BCATU(bmath,vfx,s,set_size)( d, n );

    // o = uT * a * v; o^-1 = vT * (a^-1)T * u
    bl_t success = BCATU(bmath,mfx,s,svd)( u, a, v );
    BCATU(bmath,mfx,s,get_dag_vec)( a, d );

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

    BCATU(bmath,vfx,s,discard)( d );
    BCATU(bmath,mfx,s,discard)( v );
    BCATU(bmath,mfx,s,discard)( u );
    BCATU(bmath,mfx,s,discard)( a );

    return success;
}

//----------------------------------------------------------------------------------------------------------------------

// pseudo inversion of a symmetric matrix
bl_t BCATU(bmath,mfx,s,hsm_piv)( const bmath_mfx_s* o, fx_t eps, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath,mfx,s,create)();
        BCATU(bmath,mfx,s,set_size_to)( res, buf );
        bl_t success = BCATU(bmath,mfx,s,hsm_piv)( o, eps, buf );
        BCATU(bmath,mfx,s,cpy)( buf, res );
        BCATU(bmath,mfx,s,discard)( buf );
        return success;
    }

    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, res ) );
    ASSERT( o->rows == o->cols );

    uz_t n = o->rows;
    if( n == 0 ) return true;

    bmath_mfx_s* a = res; // a occupies space of res
    bmath_mfx_s* q = BCATU(bmath,mfx,s,create)();
    BCATU(bmath,mfx,s,set_size)( q, n, n );
    BCATU(bmath,mfx,s,one)( q );

    BCATU(bmath,mfx,s,cpy)( o, a );
    bl_t success = BCATU(bmath,mfx,s,evd_htp)( a, q );

    bmath_vfx_s* dag = BCATU(bmath,vfx,s,create)();
    BCATU(bmath,vfx,s,set_size)( dag, n );
    BCATU(bmath,mfx,s,get_dag_vec)( a, dag );

    // dag is sorted in descending manner
    fx_t s_max = BCATU(fx,max)( BCATU(fx,abs)( dag->data[ 0 ] ), BCATU(fx,abs)( dag->data[ n - 1 ] ) );
    fx_t thr = s_max * eps;
    if( thr == 0 ) thr = 1.0;

    for( uz_t i = 0; i < n; i++ ) dag->data[ i ] = ( BCATU(fx,abs)( dag->data[ i ] ) < thr ) ? 0 : ( 1.0 / dag->data[ i ] );

    BCATU(bmath,mfx,s,htp)( q, q );
    BCATU(bmath,mfx,s,mul_udu_htp)( q, dag, res );

    BCATU(bmath,vfx,s,discard)( dag );
    BCATU(bmath,mfx,s,discard)( q );

    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath,mfx,s,inv_av1)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath,mfx,s,create)();
        BCATU(bmath,mfx,s,set_size_to)( res, buf );
        bl_t success = BCATU(bmath,mfx,s,inv_av1)( o, buf );
        BCATU(bmath,mfx,s,cpy)( buf, res );
        BCATU(bmath,mfx,s,discard)( buf );
        return success;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, res ) );

    bmath_mfx_s o_sub = BCATU(bmath,mfx,s,get_weak_sub_mat)(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mfx_s r_sub = BCATU(bmath,mfx,s,get_weak_sub_mat)( res, 0, 0, res->rows, res->cols - 1 );

    bl_t success = BCATU(bmath,mfx,s,inv)( &o_sub, &r_sub );

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

bl_t BCATU(bmath,mfx,s,pdf_inv_av1)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath,mfx,s,create)();
        BCATU(bmath,mfx,s,set_size_to)( res, buf );
        bl_t success = BCATU(bmath,mfx,s,pdf_inv_av1)( o, buf );
        BCATU(bmath,mfx,s,cpy)( buf, res );
        BCATU(bmath,mfx,s,discard)( buf );
        return success;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, res ) );

    bmath_mfx_s o_sub = BCATU(bmath,mfx,s,get_weak_sub_mat)(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mfx_s r_sub = BCATU(bmath,mfx,s,get_weak_sub_mat)( res, 0, 0, res->rows, res->cols - 1 );

    bl_t success = BCATU(bmath,mfx,s,pdf_inv)( &o_sub, &r_sub );

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

bl_t BCATU(bmath,mfx,s,hsm_piv_av1)( const bmath_mfx_s* o, fx_t eps, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath,mfx,s,create)();
        BCATU(bmath,mfx,s,set_size_to)( res, buf );
        bl_t success = BCATU(bmath,mfx,s,hsm_piv_av1)( o, eps, buf );
        BCATU(bmath,mfx,s,cpy)( buf, res );
        BCATU(bmath,mfx,s,discard)( buf );
        return success;
    }

    ASSERT( o->cols == o->rows + 1 );
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, res ) );

    bmath_mfx_s o_sub = BCATU(bmath,mfx,s,get_weak_sub_mat)(   o, 0, 0,   o->rows,   o->cols - 1 );
    bmath_mfx_s r_sub = BCATU(bmath,mfx,s,get_weak_sub_mat)( res, 0, 0, res->rows, res->cols - 1 );

    bl_t success = BCATU(bmath,mfx,s,hsm_piv)( &o_sub, eps, &r_sub );

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

bl_t BCATU(bmath,mfx,s,div)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    bl_t success = BCATU(bmath,mfx,s,inv)( op, res );
    BCATU(bmath,mfx,s,mul)( o, res, res );
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* v, bmath_vfx_s* r )
{
    if( r == v )
    {
        bmath_vfx_s* buf = BCATU(bmath,vfx,s,create_set_size)( r->size );
        BCATU(bmath,mfx,s,mul_vec)( o, v, buf );
        BCATU(bmath,vfx,s,cpy)( buf, r );
        BCATU(bmath,vfx,s,discard)( buf );
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

void BCATU(bmath,mfx,s,htp_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* v, bmath_vfx_s* r )
{
    if( r == v )
    {
        bmath_vfx_s* buf = BCATU(bmath,vfx,s,create_set_size)( r->size );
        BCATU(bmath,mfx,s,htp_mul_vec)( o, v, buf );
        BCATU(bmath,vfx,s,cpy)( buf, r );
        BCATU(bmath,vfx,s,discard)( buf );
        return;
    }

    ASSERT( o->rows == v->size );
    ASSERT( o->cols == r->size );
    BCATU(bmath,vfx,s,zro)( r );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,fx,t_vec,mul_scl_add)( o->data + i * o->stride, v->data[ i ], r->data, r->data, o->cols );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,mul_vec_add)( const bmath_mfx_s* o, const bmath_vfx_s* v, const bmath_vfx_s* b, bmath_vfx_s* r )
{
    if( r == v )
    {
        bmath_vfx_s* buf = BCATU(bmath,vfx,s,create_set_size)( r->size );
        BCATU(bmath,mfx,s,mul_vec_add)( o, v, b, buf );
        BCATU(bmath,vfx,s,cpy)( buf, r );
        BCATU(bmath,vfx,s,discard)( buf );
        return;
    }

    if( b != r ) BCATU(bmath,vfx,s,cpy)( b, r );

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

void BCATU(bmath,mfx,s,htp_mul_vec_add)( const bmath_mfx_s* o, const bmath_vfx_s* v, const bmath_vfx_s* b, bmath_vfx_s* r )
{
    if( r == v )
    {
        bmath_vfx_s* buf = BCATU(bmath,vfx,s,create_set_size)( r->size );
        BCATU(bmath,mfx,s,htp_mul_vec_add)( o, v, b, buf );
        BCATU(bmath,vfx,s,cpy)( buf, r );
        BCATU(bmath,vfx,s,discard)( buf );
        return;
    }

    ASSERT( o->rows == v->size );
    ASSERT( o->cols == r->size );

    if( b != r ) BCATU(bmath,vfx,s,cpy)( b, r );

    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,fx,t_vec,mul_scl_add)( o->data + i * o->stride, v->data[ i ], r->data, r->data, o->cols );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,mul_av1)( const bmath_mfx_s* o, const bmath_vfx_s* av1, bmath_vfx_s* res )
{
    if( res == av1 )
    {
        bmath_vfx_s* buf = BCATU(bmath,vfx,s,create_set_size)( res->size );
        BCATU(bmath,mfx,s,mul_av1)( o, av1, buf );
        BCATU(bmath,vfx,s,cpy)( buf, res );
        BCATU(bmath,vfx,s,discard)( buf );
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

void BCATU(bmath,mfx,s,mul_scl)( const bmath_mfx_s* o, const fx_t* b_, bmath_mfx_s* d_ )
{
    ASSERT( o->rows == d_->rows );
    ASSERT( o->cols == d_->cols );
    fx_t b = *b_;

    if( BCATU(bmath,mfx,s,is_folded)( d_ ) )
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

void BCATU(bmath,mfx,s,mul_scl_add)( const bmath_mfx_s* o, const fx_t* b_, const bmath_mfx_s* c_, bmath_mfx_s* d_ )
{
    ASSERT( o ->rows == d_->rows );
    ASSERT( o ->cols == d_->cols );
    ASSERT( c_->rows == d_->rows );
    ASSERT( c_->cols == d_->cols );
    fx_t b = *b_;

    if( BCATU(bmath,mfx,s,is_folded)( d_ ) )
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

void BCATU(bmath,mfx,s,eop_map)( const bmath_mfx_s* o, BCATU(bmath_fp,fx,ar1) b, bmath_mfx_s* r )
{
    ASSERT( o ->rows == r->rows );
    ASSERT( o ->cols == r->cols );

    if( BCATU(bmath,mfx,s,is_folded)( r ) )
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

void BCATU(bmath,mfx,s,eop_map_mul)( const bmath_mfx_s* o, BCATU(bmath_fp,fx,ar1) b, const bmath_mfx_s* c_, bmath_mfx_s* r )
{
    ASSERT( o ->rows == r->rows );
    ASSERT( o ->cols == r->cols );
    ASSERT( c_->rows == r->rows );
    ASSERT( c_->cols == r->cols );

    if( BCATU(bmath,mfx,s,is_folded)( r ) )
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

void BCATU(bmath,mfx,s,htp)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    if( o->rows == o->cols )
    {
        // in-place algorithm
        ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, res ) );
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
        return;
    }

    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath,mfx,s,create)();
        BCATU(bmath,mfx,s,set_size)( buf, res->cols, res->rows );
        BCATU(bmath,mfx,s,htp)( o, buf );
        BCATU(bmath,mfx,s,cpy)( buf, res );
        BCATU(bmath,mfx,s,discard)( buf );
        return;
    }

    ASSERT( o->rows == res->cols );
    ASSERT( o->cols == res->rows );

    for( uz_t i = 0; i < res->rows; i++ )
    {
              fx_t* vr = res->data + i * res->stride;
        const fx_t* v1 = o ->data  + i;
        for( uz_t j = 0; j < res->cols; j++ ) vr[ j ] = v1[ j * o->stride ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,pmt_mul)( const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath,mfx,s,create)();
        BCATU(bmath,mfx,s,set_size)( buf, res->rows, res->cols );
        BCATU(bmath,mfx,s,pmt_mul)( o, p, buf );
        BCATU(bmath,mfx,s,cpy)( buf, res );
        BCATU(bmath,mfx,s,discard)( buf );
        return;
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
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,pmt_htp_mul)( const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res )
{
    if( o == res )
    {
        bmath_mfx_s* buf = BCATU(bmath,mfx,s,create)();
        BCATU(bmath,mfx,s,set_size)( buf, res->rows, res->cols );
        BCATU(bmath,mfx,s,pmt_htp_mul)( o, p, buf );
        BCATU(bmath,mfx,s,cpy)( buf, res );
        BCATU(bmath,mfx,s,discard)( buf );
        return;
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
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,mul_pmt)( const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res )
{
    ASSERT( o->cols == p->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );

    for( uz_t i = 0; i < p->size; i++ ) ASSERT( p->data[ i ] < o->cols );

    if( o == res )
    {
        bmath_vfx_s* vec = BCATU(bmath,vfx,s,create_set_size)( o->cols );
        fx_t* v = vec->data;
        for( uz_t i = 0; i < o->rows; i++ )
        {
            fx_t* ri = res->data + i * res->stride;
            for( uz_t j = 0; j < res->cols; j++ ) v[ j ] = ri[ p->data[ j ] ];
            bcore_u_memcpy( sizeof( fx_t ), ri, v, res->cols );
        }
        BCATU(bmath,vfx,s,discard)( vec );
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
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,mul_pmt_htp)( const bmath_mfx_s* o, const bmath_pmt_s* p, bmath_mfx_s* res )
{
    ASSERT( o->cols == p->size );
    ASSERT( o->rows == res->rows );
    ASSERT( o->cols == res->cols );

    for( uz_t i = 0; i < p->size; i++ ) ASSERT( p->data[ i ] < o->cols );

    if( o == res )
    {
        bmath_vfx_s* vec = BCATU(bmath,vfx,s,create_set_size)( o->cols );
        fx_t* v = vec->data;
        for( uz_t i = 0; i < o->rows; i++ )
        {
            fx_t* ri = res->data + i * res->stride;
            for( uz_t j = 0; j < res->cols; j++ ) v[ p->data[ j ] ] = ri[ j ];
            bcore_u_memcpy( sizeof( fx_t ), ri, v, res->cols );
        }
        BCATU(bmath,vfx,s,discard)( vec );
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
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// element-access; col-access; row-access; sub-matrix

void BCATU(bmath,mfx,s,set_row_by_data)( bmath_mfx_s* o, uz_t idx, const fx_t* data )
{
    ASSERT( idx < o->rows );
    bcore_u_memcpy( sizeof( fx_t ), o->data + idx * o->stride, data, o->cols );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,set_col_by_data)( bmath_mfx_s* o, uz_t idx, const fx_t* data )
{
    ASSERT( idx < o->cols );
    fx_t* v = o ->data + idx;
    for( uz_t i = 0; i < o->rows; i++ ) v[ i * o ->stride ] = data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,set_dag_by_data)( bmath_mfx_s* o, const fx_t* data )
{
    uz_t size = uz_min( o->rows, o->cols );
    for( uz_t i = 0; i < size; i++ ) o->data[ i * ( o ->stride + 1 ) ] = data[ i ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,set_row_by_vec)( bmath_mfx_s* o, uz_t idx, const bmath_vfx_s* vec )
{
    ASSERT( o->cols <= vec->size );
    BCATU(bmath,mfx,s,set_row_by_data)( o, idx, vec->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,set_col_by_vec)( bmath_mfx_s* o, uz_t idx, const bmath_vfx_s* vec )
{
    ASSERT( o->rows <= vec->size );
    BCATU(bmath,mfx,s,set_col_by_data)( o, idx, vec->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,set_dag_by_vec)( bmath_mfx_s* o, const bmath_vfx_s* vec )
{
    ASSERT( uz_min( o->rows, o->cols ) <= vec->size );
    BCATU(bmath,mfx,s,set_dag_by_data)( o, vec->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,get_row_vec)(  const bmath_mfx_s* o, uz_t idx, bmath_vfx_s* res )
{
    ASSERT( idx < o->rows );
    ASSERT( o->cols <= res->size );
    bcore_u_memcpy( sizeof( fx_t ), res->data, o->data + idx * o->stride, o->cols );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,get_col_vec)(  const bmath_mfx_s* o, uz_t idx, bmath_vfx_s* res )
{
    ASSERT( idx < o->cols );
    ASSERT( o->rows <= res->size );
    const fx_t* src = o->data + idx;
    for( uz_t i = 0; i < o->rows; i++ ) res->data[ i ] = src[ i * o ->stride ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,get_dag_vec)( const bmath_mfx_s* o, bmath_vfx_s* res )
{
    uz_t size = uz_min( o->rows, o->cols );
    ASSERT( size <= res->size );
    for( uz_t i = 0; i < size; i++ ) res->data[ i ] = o->data[ i * ( o->stride + 1 ) ];
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,swap_row)( bmath_mfx_s* o, uz_t i, uz_t j )
{
    if( i == j ) return;
    ASSERT( i < o->rows );
    ASSERT( j < o->rows );
    fx_t* vi = o->data + i * o->stride;
    fx_t* vj = o->data + j * o->stride;
    for( uz_t k = 0; k < o->cols; k++ ) BCATU(fx,t_swap)( vi + k, vj + k );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,swap_col)( bmath_mfx_s* o, uz_t i, uz_t j )
{
    if( i == j ) return;
    ASSERT( i < o->cols );
    ASSERT( j < o->cols );
    fx_t* vi = o->data + i;
    fx_t* vj = o->data + j;
    for( uz_t k = 0; k < o->rows; k++ ) BCATU(fx,t_swap)( vi + k * o->stride, vj + k * o->stride );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,mul_fx_to_row)( bmath_mfx_s* o, fx_t v, uz_t i )
{
    ASSERT( i < o->rows );
    fx_t* vi = o->data + i * o->stride;
    for( uz_t k = 0; k < o->cols; k++ ) vi[ k ] *= v;
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,mul_fx_to_col)( bmath_mfx_s* o, fx_t v, uz_t i )
{
    ASSERT( i < o->cols );
    fx_t* vi = o->data + i;
    for( uz_t k = 0; k < o->rows; k++ ) vi[ k * o->stride ] *= v;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_mfx_s BCATU(bmath,mfx,s,get_weak_sub_mat)( const bmath_mfx_s* o, uz_t row, uz_t col, uz_t rows, uz_t cols )
{
    ASSERT( row + rows <= o->rows );
    ASSERT( col + cols <= o->cols );
    bmath_mfx_s ret;
    BCATU(bmath,mfx,s,init)( &ret );
    ret.rows = rows;
    ret.cols = cols;
    ret.stride = o->stride;
    ret.data = o->data + row * o->stride + col;
    ret.size = o->size;
    ret.space = 0; // indicates that ret is a weak object. (no shutdown needed)
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------

bmath_vfx_s BCATU(bmath,mfx,s,get_row_weak_vec)( const bmath_mfx_s* o, uz_t idx )
{
    ASSERT( idx < o->rows );
    bmath_vfx_s vec;
    BCATU(bmath,vfx,s,init)( &vec );
    vec.data = o->data + idx * o->stride;
    vec.size = o->cols;
    vec.space = 0;
    return vec;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath,mfx,s,decompose_cholesky)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( BCATU(bmath,mfx,s,is_square)( o ) );
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, res ) );

    bl_t success = true;

    for( uz_t i = 0; i < o->rows; i++ )
    {
              fx_t* vli = res->data + i * res->stride;
        const fx_t* voi =   o->data + i * o->stride;
        for( uz_t j = 0; j <= i; j++ )
        {
            fx_t* vlj = res->data + j * res->stride;
            fx_t acc = voi[ j ];
            for( uz_t k = 0; k < j; k++ ) acc -= vli[ k ] * vlj[ k ];
            if( i == j )
            {
                if( acc <= BCATU(fx,lim_min) ) success = false;
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

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath,mfx,s,decompose_luc)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( BCATU(bmath,mfx,s,is_square)( o ) );
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, res ) );
    uz_t n = o->cols;
    uz_t stride = res->stride;
    bl_t success = true;

    BCATU(bmath,mfx,s,cpy)( o, res );
    for( uz_t i = 0; i < n; i++ )
    {
        fx_t* vi = res->data + i * stride;
        for( uz_t j = 0; j < i; j++ )
        {
            fx_t* vj = res->data + j;
            fx_t sum = 0;
            for( uz_t k = 0; k < j; k++ ) sum += vi[ k ] * vj[ k * stride ];

            fx_t denom = vj[ j * stride ];
            if( BCATU(fx,abs)( denom ) <= BCATU(fx,lim_min) ) success = false;

            vi[ j ] = ( denom != 0 ) ? ( ( vi[ j ] - sum ) / denom ) : 0;
        }

        for( uz_t j = i; j < n; j++ )
        {
            fx_t* vj = res->data + j;
            fx_t sum = 0;
            for( uz_t k = 0; k < i; k++ ) sum += vi[ k ] * vj[ k * stride ];
            vi[ j ] -= sum;
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath,mfx,s,ltr_inv_htp)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( BCATU(bmath,mfx,s,is_square)( o ) );
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, res ) );

    uz_t n = o->rows;

    bl_t success = true;

    // diagonal elements
    for( uz_t i = 0; i < n; i++ )
    {
        fx_t v = o->data[ i * o->stride + i ];
        if( BCATU(fx,abs)( v ) < BCATU(fx,lim_min) ) success = false;
        res->data[ i * res->stride + i ] = ( v != 0 ) ? 1.0 / v : 0;
    }

    // upper off-diagonal elements
    for( uz_t i = 0; i < n; i++ )
    {
        fx_t* vri = res->data + i * res->stride;
        for( uz_t j = i + 1; j < n; j++ )
        {
            const fx_t* voj =   o->data + j *   o->stride;
                  fx_t* vrj = res->data + j * res->stride;

            vri[ j ] = -vrj[ j ] * BCATU(bmath,fx,t_vec,mul_vec)( voj + i, vri + i, j - i );
            vrj[ i ] = 0;
        }
    }

    // zero lower off-diagonal elements; (this part must be kept to last for the in-place algorithm)
    for( uz_t i = 0; i < n; i++ )
    {
        fx_t* vri = res->data + i * res->stride;
        for( uz_t j = 0; j < i; j++ ) vri[ j ] = 0;
    }

    return success;
}

bl_t BCATU(bmath,mfx,s,pdf_inv)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    ASSERT( BCATU(bmath,mfx,s,is_hsm)( o ) );
    bl_t success = BCATU(bmath,mfx,s,decompose_cholesky)( o, res ); // res = ltr
    success = success & BCATU(bmath,mfx,s,ltr_inv_htp)( res, res ); // res = utr
    BCATU(bmath,mfx,s,utr_mul_htp)( res, res );      // res = oI
    return success;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Triangular decompositions, operations and solvers

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,ltr_mul_htp)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( BCATU(bmath,mfx,s,is_square)( o ) );
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, res ) );
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

void BCATU(bmath,mfx,s,utr_mul_htp)( const bmath_mfx_s* o, bmath_mfx_s* res )
{
    // Algorithm works in-place: No need to check for o == res;
    ASSERT( BCATU(bmath,mfx,s,is_square)( o ) );
    ASSERT( BCATU(bmath,mfx,s,is_equ_size)( o, res ) );
    uz_t n = o->rows;

    // compute off diagonal elements; store in lower triangle of res
    for( uz_t i = 0; i < n; i++ )
    {
        const fx_t* voi = o->data + i * o->stride;
        for( uz_t j = i + 1; j < n; j++ )
        {
            const fx_t* voj =   o->data + j *   o->stride;
                  fx_t* vrj = res->data + j * res->stride;

            vrj[ i ] = BCATU(bmath,fx,t_vec,mul_vec)( voi + j, voj + j, n - j );
        }
    }

    // diagonal elements in ascending order
    for( uz_t i = 0; i < n; i++ )
    {
        const fx_t* voi =   o->data + i *   o->stride;
              fx_t* vri = res->data + i * res->stride;

        fx_t sum = 0;
        for( uz_t k = i; k < n; k++ ) sum += BCATU(fx,sqr)( voi[ k ] );
        vri[ i ] = sum;
    }

    // copy lower off-diagonal to upper off-diagonal
    for( uz_t i = 0; i < n; i++ )
    {
        fx_t* vri = res->data + i * res->stride;
        for( uz_t j = 0; j < i; j++ ) res->data[ j * res->stride + i ] = vri[ j ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,ltr_mul_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    for( uz_t i = o->rows - 1; i < o->rows; i-- )
    {
        fx_t* v1 = o->data + i * o->stride;
        res[ i ] = BCATU(bmath,fx,t_vec,mul_vec)( v1, op, i + 1 );
    }
}

void BCATU(bmath,mfx,s,ltr_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath,mfx,s,ltr_mul_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,lt1_mul_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    for( uz_t i = o->rows - 1; i < o->rows; i-- )
    {
        fx_t* v1 = o->data + i * o->stride;
        res[ i ] = BCATU(bmath,fx,t_vec,mul_vec)( v1, op, i ) + op[ i ];
    }
}

void BCATU(bmath,mfx,s,lt1_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath,mfx,s,lt1_mul_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,utr_mul_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    for( uz_t i = 0; i < o->rows; i++ )
    {
        fx_t* v1 = o->data + i * o->stride;
        res[ i ] = BCATU(bmath,fx,t_vec,mul_vec)( v1 + i, op + i, o->cols - i );
    }
}

void BCATU(bmath,mfx,s,utr_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath,mfx,s,utr_mul_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,luc_mul_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    BCATU(bmath,mfx,s,utr_mul_vec_)( o,  op, res );
    BCATU(bmath,mfx,s,lt1_mul_vec_)( o, res, res );
}

void BCATU(bmath,mfx,s,luc_mul_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    ASSERT( o->cols ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath,mfx,s,luc_mul_vec_)( o,  op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,ltr_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,mfx,s,ltr_mul_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,lt1_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,mfx,s,lt1_mul_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,utr_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,mfx,s,utr_mul_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,luc_mul_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,mfx,s,luc_mul_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,ltr_solve_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
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

void BCATU(bmath,mfx,s,ltr_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath,mfx,s,ltr_solve_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,utr_solve_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
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

void BCATU(bmath,mfx,s,utr_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath,mfx,s,utr_solve_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,lt1_solve_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
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

void BCATU(bmath,mfx,s,lt1_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath,mfx,s,lt1_solve_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,luc_solve_vec_)( const bmath_mfx_s* o, const fx_t* op, fx_t* res )
{
    // Algorithm works in-place for op == res;
    BCATU(bmath,mfx,s,lt1_solve_vec_)( o,  op, res );
    BCATU(bmath,mfx,s,utr_solve_vec_)( o, res, res );
}

void BCATU(bmath,mfx,s,luc_solve_vec)( const bmath_mfx_s* o, const bmath_vfx_s* op, bmath_vfx_s* res )
{
    // Algorithm works in-place for op == res;
    ASSERT( o->cols ==   o->rows );
    ASSERT( o->rows ==  op->size );
    ASSERT( o->rows == res->size );
    BCATU(bmath,mfx,s,luc_solve_vec_)( o, op->data, res->data );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,ltr_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,mfx,s,ltr_solve_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,lt1_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,mfx,s,lt1_solve_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,utr_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,mfx,s,utr_solve_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,luc_solve_htp_htp)( const bmath_mfx_s* o, const bmath_mfx_s* op, bmath_mfx_s* res )
{
    ASSERT( o->cols == op->cols );
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == res->cols );
    ASSERT( o->rows == res->rows );
    for( uz_t i = 0; i < o->rows; i++ )
    {
        BCATU(bmath,mfx,s,luc_solve_vec_)( o, op->data + i * op->stride, res->data + i * res->stride );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Other decompositions

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,trd_htp)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    ASSERT( a->rows == a->cols );

    uz_t n = a->rows;

    if( n <= 2 )
    {
        if( v ) BCATU(bmath,mfx,s,one)( v );
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
            BCATU(bmath,mfx,s,arow_rotate)( a, k, &gr, j + 1, l + 1 );
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
                BCATU(bmath,mfx,s,acol_rotate)( a, k, &gr, k + 1, n );
                if( k < n - 2 ) a->data[ ( k + 1 ) * ( a->stride + 1 ) + 1 ] = a->data[ ( k + 2 ) * ( a->stride + 1 ) - 1 ];
            }
        }
        else
        {
            // row swipes
            for( uz_t k = n - 1; k > j; k-- )
            {
                BCATU(bmath,mfx,s,arow_swipe_rev)( a, k, &grv, j + 1, k );
                if( k < n - 1 ) a->data[ k * ( a->stride + 1 ) + 1 ] = a->data[ ( k + 1 ) * ( a->stride + 1 ) - 1 ];
            }
        }
    }

    if( v )
    {
        BCATU(bmath,mfx,s,one)( v );
        for( uz_t j = 0; j < n; j++ )
        {
            for( uz_t l = n - 1; l > j + 1; l-- )
            {
                fx_t rho = a->data[ l * a->stride + j ];
                BCATU(bmath_grt,fx,s,init_from_rho)( &gr, rho );
                BCATU(bmath,mfx,s,arow_rotate)( v, l - 1, &gr, l - j - 1, n );
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

void BCATU(bmath,mfx,s,trd)( bmath_mfx_s* a, bmath_mfx_s* v )
{
    BCATU(bmath,mfx,s,trd_htp)( a, v );
    if( v ) BCATU(bmath,mfx,s,htp)( v, v );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,ubd_to_lbd)( bmath_mfx_s* a, bmath_mfx_s* v )
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

    if( v ) BCATU(bmath,mfx,s,sweep_acol_rotate_fwd)( v, 0, n - 1, &grv, 0, v->rows );

    BCATU(bmath_arr_grt,fx,s,down)( &grv );
}

//----------------------------------------------------------------------------------------------------------------------

// v transposed
void BCATU(bmath,mfx,s,ubd_to_lbd_htp)( bmath_mfx_s* a, bmath_mfx_s* v )
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
        if( v ) BCATU(bmath,mfx,s,arow_rotate)( v, j, &gr, 0, v->cols );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,lbd_to_ubd)( bmath_mfx_s* u, bmath_mfx_s* a )
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

    if( u ) BCATU(bmath,mfx,s,sweep_acol_rotate_fwd)( u, 0, n - 1, &gru, 0, u->rows );

    BCATU(bmath_arr_grt,fx,s,down)( &gru );
}

//----------------------------------------------------------------------------------------------------------------------

// u transposed
void BCATU(bmath,mfx,s,lbd_to_ubd_htp)( bmath_mfx_s* u, bmath_mfx_s* a )
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
        if( u ) BCATU(bmath,mfx,s,arow_rotate)( u, j, &gr, 0, u->cols );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,ubd)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v )
{
    if( a->rows < a->cols )
    {
        BCATU(bmath,mfx,s,lbd)( u, a, v );
        BCATU(bmath,mfx,s,lbd_to_ubd)( u, a );
        return;
    }

    ASSERT( a->rows >= a->cols );

    /// at this point: a->rows >= a->cols;

    if( u )
    {
        BCATU(bmath,mfx,s,one)( u );
        ASSERT( u != a );
        ASSERT( u->rows == a->rows );
        ASSERT( u->cols == a->rows /*full*/ || u->cols == a->cols /*thin*/  ); // u may be full or thin (nothing in-between)
    }

    if( v )
    {
        BCATU(bmath,mfx,s,one)( v );
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
            BCATU(bmath,mfx,s,qrd)( u, a );

            uz_t u_cols = u->cols;

            a->rows = a->cols;
            bmath_mfx_s* u2 = BCATU(bmath,mfx,s,create)();
            BCATU(bmath,mfx,s,set_size)( u2, a->rows, a->rows );
            BCATU(bmath,mfx,s,ubd)( u2, a, v );
            u->cols = u2->rows;

            BCATU(bmath,mfx,s,htp)( u2, u2 );

            BCATU(bmath,mfx,s,mul_htp)( u, u2, u );
            BCATU(bmath,mfx,s,discard)( u2 );

            u->cols = u_cols;
            a->rows = u_cols;
        }
        else
        {
            BCATU(bmath,mfx,s,qrd)( NULL, a );
            uz_t a_rows = a->rows;
            a->rows = a->cols;
            BCATU(bmath,mfx,s,ubd)( NULL, a, v );
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
            BCATU(bmath,mfx,s,drow_rotate)( a, j, l, &gr, j + 1, a->cols );
        }

        // zero upper row
        for( uz_t l = a->cols - 1; l > j + 1; l-- )
        {
            BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, &a->data[ j * a->stride + j + 1 ], &a->data[ j * a->stride + l ] );
            if( v ) a->data[ j * a->stride + l ] = BCATU(bmath_grt,fx,s,rho)( &gr );
            grv.data[ l - 1 ] = gr;
        }

        BCATU(bmath,mfx,s,sweep_dcol_rotate_rev)( a, j + 1, a->cols - 1, &grv, j + 1, a->rows );
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
                BCATU(bmath,mfx,s,drow_rotate)( v, j + 1, k + 1, &gr, j, v->cols );
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
                BCATU(bmath,mfx,s,drow_rotate)( u, j, k + 1, &gr, j, u->cols );
            }
        }
        a->rows = u->cols;
    }

    BCATU(bmath_arr_grt,fx,s,down)( &grv );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,lbd)( bmath_mfx_s* u, bmath_mfx_s* a, bmath_mfx_s* v )
{
    if( a->cols < a->rows )
    {
        BCATU(bmath,mfx,s,ubd)( u, a, v );
        BCATU(bmath,mfx,s,ubd_to_lbd)( a, v );
        return;
    }

    /// at this point: a->cols >= a->rows;

    if( u )
    {
        BCATU(bmath,mfx,s,one)( u );
        ASSERT( u->cols == a->rows );
        ASSERT( u->rows == a->rows );
        ASSERT( u != a );
        ASSERT( u != v );
    }

    if( v )
    {
        BCATU(bmath,mfx,s,one)( v );
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
            BCATU(bmath,mfx,s,lqd)( a, v );

            uz_t v_cols = v->cols;

            a->cols = a->rows;
            bmath_mfx_s* v2 = BCATU(bmath,mfx,s,create)();
            BCATU(bmath,mfx,s,set_size)( v2, a->cols, a->cols );
            BCATU(bmath,mfx,s,lbd)( u, a, v2 );
            v->cols = v2->rows;

            BCATU(bmath,mfx,s,htp)( v2, v2 );

            BCATU(bmath,mfx,s,mul_htp)( v, v2, v );
            BCATU(bmath,mfx,s,discard)( v2 );

            v->cols = v_cols;
            a->cols = v_cols;
        }
        else
        {
            BCATU(bmath,mfx,s,lqd)( a, NULL );
            uz_t a_cols = a->cols;
            a->cols = a->rows;
            BCATU(bmath,mfx,s,lbd)( u, a, NULL );
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

        BCATU(bmath,mfx,s,sweep_dcol_rotate_rev)( a, j, a->cols - 1, &grv, j + 1, a->rows );

        // zero lower column
        for( uz_t l = a->rows - 1; l > j + 1; l-- )
        {
            BCATU(bmath_grt,fx,s,init_and_annihilate_b)( &gr, &a->data[ ( j + 1 ) * a->stride + j ], &a->data[ l * a->stride + j ] );
            if( u ) a->data[ l * a->stride + j ] = BCATU(bmath_grt,fx,s,rho)( &gr );
            BCATU(bmath,mfx,s,drow_rotate)( a, j + 1, l, &gr, j + 1, a->cols );
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
                BCATU(bmath,mfx,s,drow_rotate)( v, j, k + 1, &gr, j, v->cols );
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
                BCATU(bmath,mfx,s,drow_rotate)( u, j + 1, k + 1, &gr, j, u->cols );
            }
        }
    }

    BCATU(bmath_arr_grt,fx,s,down)( &grv );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Covariance

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,set_covariance_on_section_fast)( bmath_mfx_s* o, bmath_arr_vfx_s* arr_vec, uz_t start, uz_t end )
{
    end = end < arr_vec->size ? end : arr_vec->size;
    start = start > end ? end : start;

    BCATU(bmath,mfx,s,zro)( o );
    if( start == end ) return;
    uz_t n = arr_vec->data[ start ].size;
    ASSERT( o->rows == n );
    ASSERT( o->cols == n );

    bmath_vfx_s* avg = BCATU(bmath,vfx,s,create_set_size)( n );
    bmath_vfx_s* vec = BCATU(bmath,vfx,s,create_set_size)( n );

    BCATU(bmath_arr,vfx,s,on_section_get_avg)( arr_vec, start, end, avg );

    for( uz_t i = start; i < end; i++ )
    {
        BCATU(bmath,vfx,s,sub)( &arr_vec->data[ i ], avg, vec );
        BCATU(bmath,mfx,s,opd_add)( o, vec, vec, o );
    }

    fx_t f = 1.0 / ( end - start );
    BCATU(bmath,mfx,s,mul_scl_fx)( o, f, o );

    BCATU(bmath,vfx,s,discard)( avg );
    BCATU(bmath,vfx,s,discard)( vec );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,set_covariance_on_section_sprc)( bmath_mfx_s* o, bmath_arr_vfx_s* arr_vec, uz_t start, uz_t end )
{
    end = end < arr_vec->size ? end : arr_vec->size;
    start = start > end ? end : start;

    BCATU(bmath,mfx,s,zro)( o );
    if( start == end ) return;
    uz_t n = arr_vec->data[ start ].size;
    ASSERT( o->rows == n );
    ASSERT( o->cols == n );

    bmath_vfx_s* avg = BCATU(bmath,vfx,s,create_set_size)( n );
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

    BCATU(bmath,vfx,s,discard)( avg );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Givens Rotations

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,sweep_arow_rotate_fwd)( bmath_mfx_s* o, uz_t row_start, uz_t row_end, const bmath_arr_grt_fx_s* grt, uz_t col_start, uz_t col_end )
{
    assert( grt->size >= row_end - 1 );
    for( uz_t i = row_start; i < row_end; i++ ) BCATU(bmath,mfx,s,arow_rotate)( o, i, &grt->data[ i ], col_start, col_end );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,sweep_arow_rotate_rev)( bmath_mfx_s* o, uz_t row_start, uz_t row_end, const bmath_arr_grt_fx_s* grt, uz_t col_start, uz_t col_end )
{
    assert( grt->size >= row_end - 1 );
    for( uz_t i = row_end; i > row_start; i-- ) BCATU(bmath,mfx,s,arow_rotate)( o, i - 1, &grt->data[ i - 1 ], col_start, col_end );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,sweep_acol_rotate_fwd)( bmath_mfx_s* o, uz_t col_start, uz_t col_end, const bmath_arr_grt_fx_s* grt, uz_t row_start, uz_t row_end )
{
    assert( grt->size >= col_end - 1 );
    if( BCATU(bmath_arr_grt,fx,s,density)( grt, col_start, col_end ) < 0.25 )
    {
        // sparse rotations: rotate columns individually
        for( uz_t i = col_start; i < col_end; i++ ) BCATU(bmath,mfx,s,acol_rotate)( o, i, &grt->data[ i ], row_start, row_end );
    }
    else
    {
        // dense rotations: use cache efficient row-sweeps
        for( uz_t i = row_start; i < row_end; i++ ) BCATU(bmath,mfx,s,arow_swipe_fwd)( o, i, grt, col_start, col_end );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,sweep_dcol_rotate_rev)( bmath_mfx_s* o, uz_t col_start, uz_t col_end, const bmath_arr_grt_fx_s* grt, uz_t row_start, uz_t row_end )
{
    assert( grt->size >= col_end - 1 );
    if( BCATU(bmath_arr_grt,fx,s,density)( grt, col_start, col_end ) < 0.25 )
    {
        // sparse rotations: rotate columns individually
        for( uz_t i = col_end; i > col_start; i-- ) BCATU(bmath,mfx,s,dcol_rotate)( o, col_start, i, &grt->data[ i - 1 ], row_start, row_end );
    }
    else
    {
        // dense rotations: use cache efficient row-sweeps
        uz_t i    = row_start;
        sz_t size = col_end - col_start;
        bmath_grt_fx_s* g = grt->data + col_start;
        fx_t* r0  = o->data + col_start;

        for( ; i < row_end - 4; i += 4 ) BCATU(bmath_simd,fx,4drow_swipe_rev)( r0 + o->stride * i, o->stride, g, size );
        for( ; i < row_end    ; i++    ) BCATU(bmath_simd,fx,drow_swipe_rev)(  r0 + o->stride * i,            g, size );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath,mfx,s,to_sink)( const bmath_mfx_s* o, bcore_sink* sink )
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

void BCATU(bmath,mfx,s,to_image)( const bmath_mfx_s* o, bmath_fp_u2_argb_from_fx fp, vd_t arg, bcore_img_u2_s* image )
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

/**********************************************************************************************************************/
/// Convolution

void BCATU(bmath,mfx,s,to_conv_operand)( bmath_mfx_s* o, sz_t kernel_size, sz_t step_size )
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

static vd_t BCATU(fx,selftest)( void )
{
    BLM_INIT();

    bmath_mfx_s* m1 = BLM_A_PUSH( BCATU(bmath,mfx,s,create)() );
    bmath_mfx_s* m2 = BLM_A_PUSH( BCATU(bmath,mfx,s,create)() );
    bmath_mfx_s* m3 = BLM_A_PUSH( BCATU(bmath,mfx,s,create)() );
    bmath_mfx_s* m4 = BLM_A_PUSH( BCATU(bmath,mfx,s,create)() );
    bmath_mfx_s* m5 = BLM_A_PUSH( BCATU(bmath,mfx,s,create)() );
    bmath_mfx_s* m6 = BLM_A_PUSH( BCATU(bmath,mfx,s,create)() );

    bmath_vfx_s* v1 = BLM_A_PUSH( BCATU(bmath,vfx,s,create)() );
    bmath_vfx_s* v2 = BLM_A_PUSH( BCATU(bmath,vfx,s,create)() );
    bmath_vfx_s* v3 = BLM_A_PUSH( BCATU(bmath,vfx,s,create)() );

    bmath_arr_vfx_s* a1 = BLM_A_PUSH( BCATU(bmath_arr,vfx,s,create)() );

    // basic linear
    {
        BCATU(bmath,mfx,s,set_size)( m1, 3, 2 );
        BCATU(bmath,mfx,s,set_fx)( m1, 0, 0, 1 );
        BCATU(bmath,mfx,s,set_fx)( m1, 0, 1, 2 );
        BCATU(bmath,mfx,s,set_fx)( m1, 1, 0, 3 );
        BCATU(bmath,mfx,s,set_fx)( m1, 1, 1, 4 );
        BCATU(bmath,mfx,s,set_fx)( m1, 2, 0, 5 );
        BCATU(bmath,mfx,s,set_fx)( m1, 2, 1, 6 );
        BCATU(bmath,mfx,s,set_size)( m2, 2, 3 );
        BCATU(bmath,mfx,s,htp)( m1, m2 );
        BCATU(bmath,mfx,s,set_size)( m3, 2, 2 );
        BCATU(bmath,mfx,s,set_size)( m4, 2, 2 );
        BCATU(bmath,mfx,s,mul)( m2, m1, m3 );

        ASSERT( BCATU(bmath,mfx,s,get_fx)( m3, 0, 0 ) == 35 );
        ASSERT( BCATU(bmath,mfx,s,get_fx)( m3, 0, 1 ) == 44 );
        ASSERT( BCATU(bmath,mfx,s,get_fx)( m3, 1, 0 ) == 44 );
        ASSERT( BCATU(bmath,mfx,s,get_fx)( m3, 1, 1 ) == 56 );
        bmath_matrix_a_mul( ( bmath_matrix* )m2, ( bmath_matrix* )m1, ( bmath_matrix* )m4 );
        ASSERT( BCATU(bmath,mfx,s,is_equ)( m3, m4 ) );

        BCATU(bmath,vfx,s,set_size)( v1, 2 );
        BCATU(bmath,vfx,s,set_size)( v2, 3 );
        BCATU(bmath,vfx,s,set_fx)( v1, 0, 1 );
        BCATU(bmath,vfx,s,set_fx)( v1, 1, 2 );
        BCATU(bmath,mfx,s,mul_vec)( m1, v1, v2 );
        ASSERT( BCATU(bmath,vfx,s,get_fx)( v2, 0 ) ==  5 );
        ASSERT( BCATU(bmath,vfx,s,get_fx)( v2, 1 ) == 11 );
        ASSERT( BCATU(bmath,vfx,s,get_fx)( v2, 2 ) == 17 );

        BCATU(bmath,mfx,s,htp_mul_vec)( m1, v2, v1 );
        ASSERT( BCATU(bmath,vfx,s,get_fx)( v1, 0 ) == 123 );
        ASSERT( BCATU(bmath,vfx,s,get_fx)( v1, 1 ) == 156 );

        BCATU(bmath,mfx,s,htp_mul_vec_add)( m1, v2, v1, v1 );
        ASSERT( BCATU(bmath,vfx,s,get_fx)( v1, 0 ) == 123 * 2 );
        ASSERT( BCATU(bmath,vfx,s,get_fx)( v1, 1 ) == 156 * 2 );
    }

    // multi-kernel convolution
    {
        sz_t operand_size = 10000;

        sz_t kernel_size  = 1000;
        sz_t step_size    = 2;
        sz_t kernels      = 100;

        // operand: initially single-row matrix of operand_size
        BCATU(bmath,mfx,s,set_size)( m1, 1, operand_size );
        for( sz_t i = 0; i < m1->cols; i++ ) m1->data[ i ] = i;
        //BCATU(bmath,mfx,s,to_stdout)( m1 );

        // turn to convolution-operand without moving data
        BCATU(bmath,mfx,s,to_conv_operand)( m1, kernel_size, step_size );

        // set up kernel
        BCATU(bmath,mfx,s,set_size)( m2, kernel_size, kernels ); // note: kernel vectors are column-vectors
        for( sz_t j = 0; j < kernels; j++ )
        {
            for( sz_t i = 0; i < m2->rows; i++ ) BCATU(bmath,mfx,s,set_fx)( m2, i, j, j + 1 );
        }
        //BCATU(bmath,mfx,s,to_stdout)( m2 );

        // convolution output
        BCATU(bmath,mfx,s,set_size)( m3, m1->rows, m2->cols );
        BCATU(bmath,mfx,s,mul)( m1, m2, m3 );
        //BCATU(bmath,mfx,s,to_stdout)( m3 );

        // convolution result:
        // row: convolution steps
        // col: response of all kernels for a given convolution step
        ASSERT( m3->cols == kernels );
        for( sz_t j = 0; j < m3->rows; j++ )
        {
            for( sz_t i = 0; i < m3->cols; i++ )
            {
                ASSERT( BCATU(bmath,mfx,s,get_fx)( m3, j, i ) == ( ( ( kernel_size ) * ( kernel_size - 1 ) / 2 ) + kernel_size * j * step_size ) * ( i + 1 ) );
            }
        }

    }

    // lower triangular inversion
    {
        BCATU(bmath,mfx,s,set_size)( m1, 3, 3 );
        BCATU(bmath,mfx,s,set_fx)( m1, 0, 0, 1 ); BCATU(bmath,mfx,s,set_fx)( m1, 0, 1, 0 );  BCATU(bmath,mfx,s,set_fx)( m1, 0, 2, 0 );
        BCATU(bmath,mfx,s,set_fx)( m1, 1, 0, 7 ); BCATU(bmath,mfx,s,set_fx)( m1, 1, 1, 5 );  BCATU(bmath,mfx,s,set_fx)( m1, 1, 2, 0 );
        BCATU(bmath,mfx,s,set_fx)( m1, 2, 0, 1 ); BCATU(bmath,mfx,s,set_fx)( m1, 2, 1, 2 );  BCATU(bmath,mfx,s,set_fx)( m1, 2, 2, 4 );
        BCATU(bmath,mfx,s,set_size)( m2, 3, 3 );
        BCATU(bmath,mfx,s,set_size)( m3, 3, 3 );
        BCATU(bmath,mfx,s,ltr_inv_htp)( m1, m2 );
        BCATU(bmath,mfx,s,mul_htp)( m1, m2, m3 );
        ASSERT( BCATU(bmath,mfx,s,is_near_one)( m3, 1E-10 ) );
    }

    // A * AT
    {
        uz_t n = 100;
        u2_t rval = 1234;
        BCATU(bmath,mfx,s,set_size)( m1, n, n );
        BCATU(bmath,mfx,s,set_size_to)( m1, m2 );
        BCATU(bmath,mfx,s,set_random)( m1, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath,mfx,s,mul_htp)( m1, m1, m2 );
        ASSERT( BCATU(bmath,mfx,s,is_near_hsm)( m2, 1E-8 ) );
    }


    // covariance
    {
        uz_t size = 1000;
        uz_t n    = 10;
        u2_t rval = 124;
        BCATU(bmath_arr,vfx,s,set_size)( a1, size );
        BCATU(bmath_arr,vfx,s,on_section_set_size)( a1, 0, -1, n );
        BCATU(bmath_arr,vfx,s,on_section_set_random)( a1, 0, -1, 1.0, -1, 1, &rval );

        BCATU(bmath,mfx,s,set_size)( m1, n, n );
        BCATU(bmath,mfx,s,set_size)( m2, n, n );
        BCATU(bmath,vfx,s,set_size)( v1, n );

        BCATU(bmath,mfx,s,set_covariance_on_section_fast)( m1, a1, 0, -1 );
        BCATU(bmath,mfx,s,set_covariance_on_section_sprc)( m2, a1, 0, -1 );
        ASSERT( BCATU(bmath,mfx,s,is_near_equ)( m1, m2, 1E-8 ) );
        ASSERT( BCATU(fx,abs)( BCATU(bmath,mfx,s,fx_trc)( m1 ) - n * 0.3333 ) < 1E-1 ); // trace should be near n/3

        fx_t sqr_norm = 4.0;
        BCATU(bmath_arr,vfx,s,on_section_set_sqr)( a1, 0, -1, sqr_norm ); // normalize all vectors to sqr_norm
        BCATU(bmath,mfx,s,set_covariance_on_section_fast)( m1, a1, 0, -1 );
        ASSERT( BCATU(fx,abs)( BCATU(bmath,mfx,s,fx_trc)( m1 ) - sqr_norm ) < 1E-2 ); // trace should be near sqr_norm (independent of n)

        BCATU(bmath_arr,vfx,s,on_section_set_sum)( a1, 0, -1, 0 ); // set sum of vector components to 0 (introducing a linear dependence)
        BCATU(bmath,mfx,s,set_covariance_on_section_fast)( m1, a1, 0, -1 );
        BCATU(bmath,mfx,s,evd_htp)( m1, NULL );
        ASSERT( BCATU(fx,abs)( BCATU(bmath,mfx,s,get_fx)( m1, n - 1, n - 1 ) ) < 1E-8 ); // last eigenvalue should be near zero
    }

    // affine-transformation
    {
        uz_t n = 20;
        BCATU(bmath,mfx,s,set_size)( m1, n, n + 1 );
        BCATU(bmath,mfx,s,set_size)( m2, n, n + 1 );
        BCATU(bmath,vfx,s,set_size)( v1, n );
        BCATU(bmath,vfx,s,set_size)( v2, n );
        BCATU(bmath,vfx,s,set_size)( v3, n );

        u2_t rval = 1236;
        BCATU(bmath,mfx,s,set_random)( m1, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath,vfx,s,set_random)( v1, 1.0, -1, 1, &rval );

        BCATU(bmath,mfx,s,mul_av1)( m1, v1, v2 );
        BCATU(bmath,mfx,s,inv_av1)( m1, m2 );
        BCATU(bmath,mfx,s,mul_av1)( m2, v2, v3 );

        ASSERT( BCATU(bmath,vfx,s,is_near_equ)( v1, v3, 1E-8 ) );
    }

    // hsm-affine-transformation
    {
        uz_t n = 20;
        BCATU(bmath,mfx,s,set_size)( m1, n, n + 1 );
        BCATU(bmath,mfx,s,set_size)( m2, n, n + 1 );
        BCATU(bmath,vfx,s,set_size)( v1, n );
        BCATU(bmath,vfx,s,set_size)( v2, n );
        BCATU(bmath,vfx,s,set_size)( v3, n );

        u2_t rval = 1236;
        BCATU(bmath,mfx,s,set_random)( m1, false, false, 0, 1.0, -1, 1, &rval );

        bmath_mfx_s m1_sub = BCATU(bmath,mfx,s,get_weak_sub_mat)( m1, 0, 0, n, n );
        BCATU(bmath,mfx,s,mul_htp)( &m1_sub, &m1_sub, &m1_sub ); // make nxn submatrix of m1 symmetric

        BCATU(bmath,vfx,s,set_random)( v1, 1.0, -1, 1, &rval );

        BCATU(bmath,mfx,s,mul_av1)( m1, v1, v2 );
        BCATU(bmath,mfx,s,pdf_inv_av1)( m1, m2 );
        BCATU(bmath,mfx,s,mul_av1)( m2, v2, v3 );

        ASSERT( BCATU(bmath,vfx,s,is_near_equ)( v1, v3, 1E-8 ) );
    }

    // hsm-affine-transformation (test via piv)
    {
        uz_t n = 20;
        BCATU(bmath,mfx,s,set_size)( m1, n, n + 1 );
        BCATU(bmath,mfx,s,set_size)( m2, n, n + 1 );
        BCATU(bmath,vfx,s,set_size)( v1, n );
        BCATU(bmath,vfx,s,set_size)( v2, n );
        BCATU(bmath,vfx,s,set_size)( v3, n );

        u2_t rval = 1236;
        BCATU(bmath,mfx,s,set_random)( m1, false, false, 0, 1.0, -1, 1, &rval );

        bmath_mfx_s m1_sub = BCATU(bmath,mfx,s,get_weak_sub_mat)( m1, 0, 0, n, n );
        BCATU(bmath,mfx,s,mul_htp)( &m1_sub, &m1_sub, &m1_sub ); // make nxn submatrix of m1 symmetric

        BCATU(bmath,vfx,s,set_random)( v1, 1.0, -1, 1, &rval );

        BCATU(bmath,mfx,s,mul_av1)( m1, v1, v2 );
        BCATU(bmath,mfx,s,hsm_piv_av1)( m1, 1E-8, m2 );
        BCATU(bmath,mfx,s,mul_av1)( m2, v2, v3 );

        ASSERT( BCATU(bmath,vfx,s,is_near_equ)( v1, v3, 1E-8 ) );
    }

    // mul_udu_htp
    {
        sz_t ma[ 7 ] = { 10, 20, 10, 10, 10,  1,  0 };
        sz_t na[ 7 ] = { 20, 10, 10,  1,  0, 10, 10 };
        for( sz_t i = 0; i < 7; i++ )
        {
            sz_t m = ma[ i ];
            sz_t n = na[ i ];
            BCATU(bmath,mfx,s,set_size)( m1, m, n );
            BCATU(bmath,mfx,s,set_size)( m2, n, n );
            BCATU(bmath,mfx,s,set_size)( m3, m, n );
            BCATU(bmath,mfx,s,set_size)( m4, m, m );
            BCATU(bmath,mfx,s,set_size)( m5, m, m );
            BCATU(bmath,vfx,s,set_size)( v1, n );

            u2_t rval = 1236;
            BCATU(bmath,mfx,s,set_random)( m1, false, false, 0, 1.0, -1, 1, &rval );
            BCATU(bmath,vfx,s,set_random)( v1,                  1.0, -1, 1, &rval );

            BCATU(bmath,mfx,s,mul_udu_htp)( m1, v1, m5 );

            BCATU(bmath,mfx,s,zro)( m2 );
            BCATU(bmath,mfx,s,set_dag_by_vec)( m2, v1 );
            BCATU(bmath,mfx,s,mul)( m1, m2, m3 );
            BCATU(bmath,mfx,s,mul_htp)( m3, m1, m4 );
            ASSERT( BCATU(bmath,mfx,s,is_near_equ)( m4, m5, 1E-8 ) );
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
            BCATU(bmath,mfx,s,set_size)( m1, m, n );
            BCATU(bmath,mfx,s,set_size)( m2, n, n );
            BCATU(bmath,mfx,s,set_size)( m3, m, n );
            BCATU(bmath,mfx,s,set_size)( m4, m, n );
            BCATU(bmath,mfx,s,set_size)( m5, m, m );
            BCATU(bmath,mfx,s,set_size)( m6, m, m );
            BCATU(bmath,vfx,s,set_size)( v2, n );

            u2_t rval = 1236;
            BCATU(bmath,mfx,s,set_random)( m1, false, false, 0, 1.0, -1, 1, &rval );
            BCATU(bmath,vfx,s,set_random)( v2,                  1.0, -1, 1, &rval );
            BCATU(bmath,mfx,s,set_random)( m3, false, false, 0, 1.0, -1, 1, &rval );

            BCATU(bmath,mfx,s,mul_udv_htp)( m1, v2, m3, m5 );

            BCATU(bmath,mfx,s,zro)( m2 );
            BCATU(bmath,mfx,s,set_dag_by_vec)( m2, v2 );
            BCATU(bmath,mfx,s,mul)( m1, m2, m4 );
            BCATU(bmath,mfx,s,mul_htp)( m4, m3, m6 );
            ASSERT( BCATU(bmath,mfx,s,is_near_equ)( m5, m6, 1E-8 ) );
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
            BCATU(bmath,mfx,s,set_size)( m1, m, n );
            BCATU(bmath,mfx,s,set_size)( m2, n, n );
            BCATU(bmath,mfx,s,set_size)( m3, m, n );
            BCATU(bmath,mfx,s,set_size)( m4, m, n );
            BCATU(bmath,mfx,s,set_size)( m5, m, m );
            BCATU(bmath,mfx,s,set_size)( m6, m, m );

            u2_t rval = 1236;
            BCATU(bmath,mfx,s,set_random)( m1, false, false, 0, 1.0, -1, 1, &rval );
            BCATU(bmath,mfx,s,set_random)( m2, false, false, 0, 1.0, -1, 1, &rval );
            BCATU(bmath,mfx,s,set_random)( m3, false, false, 0, 1.0, -1, 1, &rval );

            for( sz_t i = 0; i < m2->rows; i++ )
            {
                for( sz_t j = 0; j < m2->cols; j++ )
                {
                    if( j > i + 1 || j < i - 1 ) m2->data[ i * m2->stride + j ] = 0;
                }
            }

            BCATU(bmath,mfx,s,mul_utv_htp)( m1, m2, m3, m5 );
            BCATU(bmath,mfx,s,mul)( m1, m2, m4 );
            BCATU(bmath,mfx,s,mul_htp)( m4, m3, m6 );
            ASSERT( BCATU(bmath,mfx,s,is_near_equ)( m5, m6, 1E-8 ) );
        }
    }

    /// mul_add_cps
    BCATU(bmath,mfx,s,mul_add_cps_selftest)();

    BLM_DOWN();
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------

static void BCATU(fx,eval_test)( void )
{
    BLM_INIT();
    BCORE_LIFE_CREATE( bmath_mfx_eval_s, eval );
    BCORE_LIFE_CREATE( bmath_arr_mfx_eval_s, arr_eval );

    eval->density = 1.0;
    eval->rows = 20; eval->cols = 20; bmath_arr_mfx_eval_s_push( arr_eval, eval );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,trd)    , ( fp_t )BCATU(bmath,mfx,s,trd));
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,evd_htp), ( fp_t )BCATU(bmath,mfx,s,evd_htp) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,cld)    , ( fp_t )BCATU(bmath,mfx,s,decompose_cholesky) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,lud)    , ( fp_t )BCATU(bmath,mfx,s,decompose_luc) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,inv)    , ( fp_t )BCATU(bmath,mfx,s,inv) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,pdf_inv), ( fp_t )BCATU(bmath,mfx,s,pdf_inv) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,hsm_piv), ( fp_t )BCATU(bmath,mfx,s,hsm_piv) );

    eval->full = false;
    eval->rows = 10; eval->cols = 30; bmath_arr_mfx_eval_s_push( arr_eval, eval );
    eval->rows = 30; eval->cols = 10; bmath_arr_mfx_eval_s_push( arr_eval, eval );
    eval->full = true;
    eval->rows = 10; eval->cols = 30; bmath_arr_mfx_eval_s_push( arr_eval, eval );
    eval->rows = 30; eval->cols = 10; bmath_arr_mfx_eval_s_push( arr_eval, eval );

    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,mul),         ( fp_t )BCATU(bmath,mfx,s,mul) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,mul_htp),     ( fp_t )BCATU(bmath,mfx,s,mul_htp) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,htp_mul),     ( fp_t )BCATU(bmath,mfx,s,htp_mul) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,htp_mul_htp), ( fp_t )BCATU(bmath,mfx,s,htp_mul_htp) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,qrd),     ( fp_t )BCATU(bmath,mfx,s,qrd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,qrd_pmt), ( fp_t )BCATU(bmath,mfx,s,qrd_pmt) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,lqd),     ( fp_t )BCATU(bmath,mfx,s,lqd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,pmt_lqd), ( fp_t )BCATU(bmath,mfx,s,pmt_lqd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,ubd),     ( fp_t )BCATU(bmath,mfx,s,ubd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,lbd),     ( fp_t )BCATU(bmath,mfx,s,lbd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,svd),     ( fp_t )BCATU(bmath,mfx,s,svd) );
    bmath_arr_mfx_eval_s_run( arr_eval, BCATU(TYPEOF_bmath_fp,mfx,s,piv),     ( fp_t )BCATU(bmath,mfx,s,piv) );

    BLM_DOWN();
}

/**********************************************************************************************************************/
// closing template

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

