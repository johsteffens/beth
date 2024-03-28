/** Author and Copyright 2024 Johannes Bernhard Steffens
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

/** Collection of algorithms related to matrix multiplication.
 *  All routines have been designed from scratch and optimized with modern architectures in mind.
 *
 *  Blocking and microkernel strategy:
 *    - Microkernels are optimized for BLOCK_SIZE
 *
 *  OMP optimization:
 *    - routines are vectorized and parallelized using omp directives
 */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

#pragma GCC diagnostic ignored "-Wunknown-pragmas"

/**********************************************************************************************************************/

#if BMATH_TEMPLATE_FX_PREC == 2
    #define BMATH_OMP_MUL_BLOCK_SIZE ( 64 )
    #define BMATH_OMP_MUL_VCT_SIZE ( 8 )
#elif BMATH_TEMPLATE_FX_PREC == 3
    #define BMATH_OMP_MUL_BLOCK_SIZE ( 64 )
    #define BMATH_OMP_MUL_VCT_SIZE ( 8 )
#endif // BMATH_TEMPLATE_FX_PREC


/**********************************************************************************************************************/
/// mul

//----------------------------------------------------------------------------------------------------------------------

static void mul_raw0( fx_t* a, sz_t a_stride, fx_t* b, sz_t b_stride, fx_t* restrict r, sz_t r_stride, sz_t r_rows, sz_t r_cols, sz_t v_size )
{
    assert( r_cols <= BMATH_OMP_MUL_BLOCK_SIZE );
    for( sz_t i = 0; i < r_rows; i++ )
    {
        f3_t r_buf[ BMATH_OMP_MUL_BLOCK_SIZE ] = {0};
        for( sz_t k = 0; k < v_size; k++ )
        {
            #pragma omp simd
            for( sz_t j = 0; j < r_cols; j++ ) r_buf[ j ] += a[ i * a_stride + k ] * b[ k * b_stride + j ];
        }
        #pragma omp simd
        for( sz_t j = 0; j < r_cols; j++ ) r[ i * r_stride + j ] += r_buf[ j ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void mul_raw1( fx_t* a, sz_t a_stride, fx_t* b, sz_t b_stride, fx_t* restrict r, sz_t r_stride, sz_t r_rows, sz_t r_cols, sz_t v_size )
{
    sz_t block_size = BMATH_OMP_MUL_BLOCK_SIZE;
    for( sz_t i = 0; i < r_rows; i += block_size )
    {
        for( sz_t j = 0; j < r_cols; j += block_size )
        {
            for( sz_t k = 0; k < v_size; k += block_size )
            {
                mul_raw0
                (
                    a + i * a_stride + k, a_stride,
                    b + k * b_stride + j, b_stride,
                    r + i * r_stride + j, r_stride,
                    sz_min( r_rows - i, block_size ),
                    sz_min( r_cols - j, block_size ),
                    sz_min( v_size - k, block_size )
                );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void mul_acc( const bmath_mfx_s* a, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    ASSERT( a->cols == b->rows );
    ASSERT( a->rows == r->rows );
    ASSERT( b->cols == r->cols );

    if( r == a || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        mul_acc( a, b, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    sz_t block_size = 128;
    #pragma omp parallel for
    for( sz_t i = 0; i < r->rows; i += block_size )
    {
        mul_raw1
        (
            a->data + i * a->stride, a->stride,
            b->data, b->stride,
            r->data + i * r->stride, r->stride,
            sz_min( r->rows - i, block_size ),
            r->cols,
            a->cols
        );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// mul_htp

//----------------------------------------------------------------------------------------------------------------------

static void mul_htp_raw0( fx_t* a, sz_t a_stride, fx_t* b, sz_t b_stride, fx_t* restrict r, sz_t r_stride, sz_t r_rows, sz_t r_cols, sz_t v_size )
{
    for( sz_t i = 0; i < r_rows; i++ )
    {
        for( sz_t j = 0; j < r_cols; j++ )
        {
            f3_t sum[ BMATH_OMP_MUL_VCT_SIZE ] = {0};
            sz_t k = 0;

            for( ; k <= v_size - BMATH_OMP_MUL_VCT_SIZE; k += BMATH_OMP_MUL_VCT_SIZE )
            {
                #pragma omp simd
                for( sz_t l = 0; l < BMATH_OMP_MUL_VCT_SIZE; l++ ) sum[ l ] += a[ i * a_stride + k + l ] * b[ j * b_stride + k + l ];
            }

            f3_t sum1 = 0;
            for( ; k < v_size; k++ ) sum1 += a[ i * a_stride + k ] * b[ j * b_stride + k ];
            for( sz_t l = 0; l < BMATH_OMP_MUL_VCT_SIZE; l += 2 ) sum1 += sum[ l ] + sum[ l + 1 ];

            r[ i * r_stride + j ] += sum1;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void mul_htp_raw1( fx_t* a, sz_t a_stride, fx_t* b, sz_t b_stride, fx_t* restrict r, sz_t r_stride, sz_t r_rows, sz_t r_cols, sz_t v_size )
{
    sz_t block_size = BMATH_OMP_MUL_BLOCK_SIZE;
    for( sz_t i = 0; i < r_rows; i += block_size )
    {
        for( sz_t j = 0; j < r_cols; j += block_size )
        {
            for( sz_t k = 0; k < v_size; k += block_size )
            {
                mul_htp_raw0
                (
                    a + i * a_stride + k, a_stride,
                    b + j * b_stride + k, b_stride,
                    r + i * r_stride + j, r_stride,
                    sz_min( r_rows - i, block_size ),
                    sz_min( r_cols - j, block_size ),
                    sz_min( v_size - k, block_size )
                );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void mul_htp_acc( const bmath_mfx_s* a, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    ASSERT( a->cols == b->cols );
    ASSERT( a->rows == r->rows );
    ASSERT( b->rows == r->cols );

    if( r == a || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,cpy)( r, buf );
        mul_htp_acc( a, b, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    sz_t block_size = 128;
    #pragma omp parallel for
    for( sz_t i = 0; i < r->rows; i += block_size )
    {
        mul_htp_raw1
        (
            a->data + i * a->stride, a->stride,
            b->data, b->stride,
            r->data + i * r->stride, r->stride,
            sz_min( r->rows - i, block_size ),
            r->cols,
            a->cols
        );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// htp_mul

//----------------------------------------------------------------------------------------------------------------------

static void htp_mul_raw0( fx_t* a, sz_t a_stride, fx_t* b, sz_t b_stride, fx_t* restrict r, sz_t r_stride, sz_t r_rows, sz_t r_cols, sz_t v_size )
{
    assert( r_cols <= BMATH_OMP_MUL_BLOCK_SIZE );
    for( sz_t i = 0; i < r_rows; i++ )
    {
        f3_t r_buf[ BMATH_OMP_MUL_BLOCK_SIZE ] = {0};
        for( sz_t k = 0; k < v_size; k++ )
        {
            #pragma omp simd
            for( sz_t j = 0; j < r_cols; j++ ) r_buf[ j ] += a[ k * a_stride + i ] * b[ k * b_stride + j ];
        }
        #pragma omp simd
        for( sz_t j = 0; j < r_cols; j++ ) r[ i * r_stride + j ] += r_buf[ j ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void htp_mul_raw1( fx_t* a, sz_t a_stride, fx_t* b, sz_t b_stride, fx_t* restrict r, sz_t r_stride, sz_t r_rows, sz_t r_cols, sz_t v_size )
{
    sz_t block_size = BMATH_OMP_MUL_BLOCK_SIZE;
    for( sz_t i = 0; i < r_rows; i += block_size )
    {
        for( sz_t j = 0; j < r_cols; j += block_size )
        {
            for( sz_t k = 0; k < v_size; k += block_size )
            {
                htp_mul_raw0
                (
                    a + k * a_stride + i, a_stride,
                    b + k * b_stride + j, b_stride,
                    r + i * r_stride + j, r_stride,
                    sz_min( r_rows - i, block_size ),
                    sz_min( r_cols - j, block_size ),
                    sz_min( v_size - k, block_size )
                );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void htp_mul_acc( const bmath_mfx_s* a, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    ASSERT( a->rows == b->rows );
    ASSERT( a->cols == r->rows );
    ASSERT( b->cols == r->cols );

    if( r == a || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,cpy)( r, buf );
        mul_htp_acc( a, b, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    sz_t block_size = 128;
    #pragma omp parallel for
    for( sz_t i = 0; i < r->rows; i += block_size )
    {
        htp_mul_raw1
        (
            a->data + i, a->stride,
            b->data, b->stride,
            r->data + i * r->stride, r->stride,
            sz_min( r->rows - i, block_size ),
            r->cols,
            a->rows
        );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// htp_mul_htp

//----------------------------------------------------------------------------------------------------------------------

static void htp_mul_htp_raw0( fx_t* a, sz_t a_stride, fx_t* b, sz_t b_stride, fx_t* restrict r, sz_t r_stride, sz_t r_rows, sz_t r_cols, sz_t v_size )
{
    assert( r_rows <= BMATH_OMP_MUL_BLOCK_SIZE );
    for( sz_t j = 0; j < r_cols; j++ )
    {
        f3_t r_buf[ BMATH_OMP_MUL_BLOCK_SIZE ] = {0};
        for( sz_t k = 0; k < v_size; k++ )
        {
            #pragma omp simd
            for( sz_t i = 0; i < r_rows; i++ ) r_buf[ i ] += a[ k * a_stride + i ] * b[ j * b_stride + k ];
        }

         #pragma omp simd
         for( sz_t i = 0; i < r_rows; i++ ) r[ i * r_stride + j ] += r_buf[ i ];
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void htp_mul_htp_raw1( fx_t* a, sz_t a_stride, fx_t* b, sz_t b_stride, fx_t* restrict r, sz_t r_stride, sz_t r_rows, sz_t r_cols, sz_t v_size )
{
    sz_t block_size = BMATH_OMP_MUL_BLOCK_SIZE;
    for( sz_t i = 0; i < r_rows; i += block_size )
    {
        for( sz_t j = 0; j < r_cols; j += block_size )
        {
            for( sz_t k = 0; k < v_size; k += block_size )
            {
                htp_mul_htp_raw0
                (
                    a + k * a_stride + i, a_stride,
                    b + j * b_stride + k, b_stride,
                    r + i * r_stride + j, r_stride,
                    sz_min( r_rows - i, block_size ),
                    sz_min( r_cols - j, block_size ),
                    sz_min( v_size - k, block_size )
                );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void htp_mul_htp_acc( const bmath_mfx_s* a, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    ASSERT( a->rows == b->cols );
    ASSERT( a->cols == r->rows );
    ASSERT( b->rows == r->cols );

    if( r == a || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,cpy)( r, buf );
        htp_mul_htp_acc( a, b, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    sz_t block_size = 128;
    #pragma omp parallel for
    for( sz_t i = 0; i < r->rows; i += block_size )
    {
        htp_mul_htp_raw1
        (
            a->data + i, a->stride,
            b->data, b->stride,
            r->data + i * r->stride, r->stride,
            sz_min( r->rows - i, block_size ),
            r->cols,
            a->rows
        );
    }
}

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,omp_mul)( const bmath_mfx_s* a, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    BCATU(bmath_mfx_s,zro)( r );
    mul_acc( a, b, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,omp_mul_htp)( const bmath_mfx_s* a, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    BCATU(bmath_mfx_s,zro)( r );
    mul_htp_acc( a, b, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,omp_htp_mul)( const bmath_mfx_s* a, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    BCATU(bmath_mfx_s,zro)( r );
    htp_mul_acc( a, b, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,omp_htp_mul_htp)( const bmath_mfx_s* a, const bmath_mfx_s* b, bmath_mfx_s* r )
{
    BCATU(bmath_mfx_s,zro)( r );
    htp_mul_htp_acc( a, b, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,omp_mul_add)( const bmath_mfx_s* a, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == a || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,omp_mul_add)( a, b, c, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( a->cols == b->rows );
    ASSERT( a->rows == r->rows );
    ASSERT( b->cols == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r )
    {
        BCATU(bmath_mfx_s,cpy)( c, r );
    }
    else
    {
        BCATU(bmath_mfx_s,zro)( r );
    }

    mul_acc( a, b, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,omp_mul_htp_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == o || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,omp_mul_htp_add)( o, b, c, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( o->cols == b->cols );
    ASSERT( o->rows == r->rows );
    ASSERT( b->rows == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r )
    {
        BCATU(bmath_mfx_s,cpy)( c, r );
    }
    else
    {
        BCATU(bmath_mfx_s,zro)( r );
    }

    mul_htp_acc( o, b, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,omp_htp_mul_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == o || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,omp_htp_mul_add)( o, b, c, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( o->rows == b->rows );
    ASSERT( o->cols == r->rows );
    ASSERT( b->cols == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r )
    {
        BCATU(bmath_mfx_s,cpy)( c, r );
    }
    else
    {
        BCATU(bmath_mfx_s,zro)( r );
    }

    htp_mul_acc( o, b, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,omp_htp_mul_htp_add)( const bmath_mfx_s* o, const bmath_mfx_s* b, const bmath_mfx_s* c, bmath_mfx_s* r )
{
    if( r == o || r == b )
    {
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,omp_htp_mul_htp_add)( o, b, c, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( o->rows == b->cols );
    ASSERT( o->cols == r->rows );
    ASSERT( b->rows == r->cols );
    ASSERT( c->rows == r->rows );
    ASSERT( c->cols == r->cols );

    if( c != r )
    {
        BCATU(bmath_mfx_s,cpy)( c, r );
    }
    else
    {
        BCATU(bmath_mfx_s,zro)( r );
    }

    htp_mul_htp_acc( o, b, r );
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,omp_mul_add_cps)( bl_t htpa, const bmath_mfx_s* a, bl_t htpb, const bmath_mfx_s* b, fx_t c, const bmath_mfx_s* d, f3_t e, bmath_mfx_s* r )
{
    if( r == a || r == b )
    {
        ASSERT( !BCATU(bmath_mfx_s,is_folded)( r ) );
        bmath_mfx_s* buf = BCATU(bmath_mfx_s,create)();
        BCATU(bmath_mfx_s,set_size)( buf, r->rows, r->cols );
        BCATU(bmath_mfx_s,omp_mul_add_cps)( htpa, a, htpb, b, c, d, e, buf );
        BCATU(bmath_mfx_s,cpy)( buf, r );
        BCATU(bmath_mfx_s,discard)( buf );
        return;
    }

    ASSERT( ( htpa ? a->rows : a->cols ) == ( htpb ? b->cols : b->rows ) );
    ASSERT( ( htpa ? a->cols : a->rows ) == r->rows );
    ASSERT( ( htpb ? b->rows : b->cols ) == r->cols );

    if( d )
    {
        ASSERT( d->rows == r->rows );
        ASSERT( d->cols == r->cols );
        if( c != 0 )
        {
            if( c == e )
            {
                if( d != r ) BCATU(bmath_mfx_s,cpy)( d, r );
            }
            else
            {
                BCATU(bmath_mfx_s,mul_scl_fx)( d, e / c, r );
            }
        }
        else
        {
            if( e == 1.0 )
            {
                if( d != r ) BCATU(bmath_mfx_s,cpy)( d, r );
            }
            else
            {
                BCATU(bmath_mfx_s,mul_scl_fx)( d, e, r );
            }
        }
    }
    else
    {
        BCATU(bmath_mfx_s,zro)( r );
    }

    if( c != 0 )
    {
        switch( htpa * 2 + htpb )
        {
            case 0: mul_acc        ( a, b, r ); break; // a * b
            case 1: mul_htp_acc    ( a, b, r ); break; // a * b^T
            case 2: htp_mul_acc    ( a, b, r ); break; // a^T * b
            case 3: htp_mul_htp_acc( a, b, r ); break; // a^T * b^T
            default: break;
        }
        if( c != 1.0 ) BCATU(bmath_mfx_s,mul_scl_fx)( r, c, r );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_mfx_s,omp_mul_add_cps_selftest)()
{
    BLM_INIT();

    bmath_mfx_s* a  = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* b  = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* ab = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* d  = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* r1 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s* r2 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );

    sz_t n  = 25;
    sz_t m1 = 37;
    sz_t m2 = 15;

    fx_t c = 0.5;
    fx_t e = 0.3;

    fx_t max_dev = sizeof( fx_t ) == 4 ? 1E-5 : 1E-10;

    // a * b
    {
        BCATU(bmath_mfx_s,set_size)(  a, m1, n  );
        BCATU(bmath_mfx_s,set_size)(  b, n,  m2 );
        BCATU(bmath_mfx_s,set_size)(  d, m1, m2 );
        u3_t rval = 1236;
        BCATU(bmath_mfx_s,set_random_u3)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random_u3)( b, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random_u3)( d, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r1, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r2, m1, m2 );
        BCATU(bmath_mfx_s,omp_mul_add_cps)( false, a, false, b, c, d, e, r1 );
        BCATU(bmath_mfx_s,mul_esp)( a, b, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( ab, c, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( d,  e, r2 );
        BCATU(bmath_mfx_s,add)( ab, r2, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, max_dev ) );
    }

    // a^T * b
    {
        BCATU(bmath_mfx_s,set_size)(  a, n,  m1  );
        BCATU(bmath_mfx_s,set_size)(  b, n,  m2 );
        BCATU(bmath_mfx_s,set_size)(  d, m1, m2 );
        u3_t rval = 1236;
        BCATU(bmath_mfx_s,set_random_u3)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random_u3)( b, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random_u3)( d, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r1, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r2, m1, m2 );
        BCATU(bmath_mfx_s,omp_mul_add_cps)( true, a, false, b, c, d, e, r1 );
        BCATU(bmath_mfx_s,htp_mul_esp)( a, b, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( ab, c, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( d,  e, r2 );
        BCATU(bmath_mfx_s,add)( ab, r2, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, max_dev ) );
    }

    // a * b^T
    {
        BCATU(bmath_mfx_s,set_size)(  a, m1, n  );
        BCATU(bmath_mfx_s,set_size)(  b, m2, n  );
        BCATU(bmath_mfx_s,set_size)(  d, m1, m2 );
        u3_t rval = 1236;
        BCATU(bmath_mfx_s,set_random_u3)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random_u3)( b, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random_u3)( d, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r1, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r2, m1, m2 );
        BCATU(bmath_mfx_s,omp_mul_add_cps)( false, a, true, b, c, d, e, r1 );
        BCATU(bmath_mfx_s,mul_htp_esp)( a, b, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( ab, c, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( d,  e, r2 );
        BCATU(bmath_mfx_s,add)( ab, r2, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, max_dev ) );
    }

    // a^T * b^T
    {
        BCATU(bmath_mfx_s,set_size)(  a, n,  m1 );
        BCATU(bmath_mfx_s,set_size)(  b, m2, n  );
        BCATU(bmath_mfx_s,set_size)(  d, m1, m2 );
        u3_t rval = 1236;
        BCATU(bmath_mfx_s,set_random_u3)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random_u3)( b, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_random_u3)( d, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r1, m1, m2 );
        BCATU(bmath_mfx_s,set_size)( r2, m1, m2 );
        BCATU(bmath_mfx_s,omp_mul_add_cps)( true, a, true, b, c, d, e, r1 );
        BCATU(bmath_mfx_s,htp_mul_htp)( a, b, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( ab, c, ab );
        BCATU(bmath_mfx_s,mul_scl_fx)( d,  e, r2 );
        BCATU(bmath_mfx_s,add)( ab, r2, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, max_dev ) );
    }

    // a^T * a
    {
        BCATU(bmath_mfx_s,set_size)(  a, m1, n );
        u3_t rval = 1236;
        BCATU(bmath_mfx_s,set_random_u3)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, n, n );
        BCATU(bmath_mfx_s,set_size)( r1, n, n );
        BCATU(bmath_mfx_s,set_size)( r2, n, n );
        BCATU(bmath_mfx_s,omp_mul_add_cps)( true, a, false, a, c, NULL, e, r1 );
        BCATU(bmath_mfx_s,htp_mul_esp)( a, a, r2 );
        BCATU(bmath_mfx_s,mul_scl_fx)( r2, c, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, max_dev ) );
    }

    // a * a^T
    {
        BCATU(bmath_mfx_s,set_size)(  a, n, m1 );
        u3_t rval = 1236;
        BCATU(bmath_mfx_s,set_random_u3)( a, false, false, 0, 1.0, -1, 1, &rval );
        BCATU(bmath_mfx_s,set_size)( ab, n, n );
        BCATU(bmath_mfx_s,set_size)( r1, n, n );
        BCATU(bmath_mfx_s,set_size)( r2, n, n );
        BCATU(bmath_mfx_s,omp_mul_add_cps)( false, a, true, a, c, NULL, e, r1 );
        BCATU(bmath_mfx_s,mul_htp_esp)( a, a, r2 );
        BCATU(bmath_mfx_s,mul_scl_fx)( r2, c, r2 );
        ASSERT( BCATU(bmath_mfx_s,is_near_equ)( r1, r2, max_dev ) );
    }

    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

