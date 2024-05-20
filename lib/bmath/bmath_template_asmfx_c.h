/** Author and Copyright 2019 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,set_size_splicing)( bmath_asmfx_s* o, sz_t rows, sz_t cols )
{
    o->i_size = cols * rows;
    o->i_data = bcore_un_alloc( sizeof( sz_t ), o->i_data, o->i_space, o->i_size, &o->i_space );
    bcore_u_memzero( sizeof( sz_t ), o->i_data, o->i_size );
    o->rows = rows;
    o->cols = cols;
    o->i_stride = cols;
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,copy_splicing)( bmath_asmfx_s* o, const bmath_asmfx_s* src )
{
    if( o == src ) return;
    BCATU(bmath_asmfx_s,set_size_splicing)( o, o->rows, o->cols );
    bcore_u_memcpy( sizeof( sz_t ), o->i_data, src->i_data, o->i_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,set_size_data)( bmath_asmfx_s* o, sz_t size )
{
    o->v_size = size;
    o->v_data = bcore_un_alloc( sizeof( fx_t ), o->v_data, o->v_space, o->v_size, &o->v_space );
    bcore_u_memzero( sizeof( fx_t ), o->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,clear)( bmath_asmfx_s* o )
{
    o->i_size = 0;
    o->i_data = bcore_un_alloc( sizeof( sz_t ), o->i_data, o->i_space, o->i_size, &o->i_space );
    o->rows = 0;
    o->cols = 0;
    o->i_stride = 0;
    o->v_size = 0;
    o->v_data = bcore_un_alloc( sizeof( fx_t ), o->v_data, o->v_space, o->v_size, &o->v_space );
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,set_size)( bmath_asmfx_s* o, sz_t rows, sz_t cols )
{
    BCATU(bmath_asmfx_s,set_size_splicing)( o, rows, cols );
    BCATU(bmath_asmfx_s,set_size_data)( o, rows * cols );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        for( sz_t j = 0; j < o->cols; j++ ) o->i_data[ i * o->i_stride + j ] = i * o->cols + j;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,set_shape_alike)( bmath_asmfx_s* o, const bmath_asmfx_s* src )
{
    BCATU(bmath_asmfx_s,copy_splicing)( o, src );
    BCATU(bmath_asmfx_s,set_size_data)( o, src->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,fit_size_data)( bmath_asmfx_s* o )
{
    sz_t max_idx = 0;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        for( sz_t j = 0; j < o->cols; j++ )
        {
            max_idx = sz_max( o->i_data[ i * o->i_stride + j ], max_idx );
        }
    }
    BCATU(bmath_asmfx_s,set_size_data)( o, max_idx + 1 );
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,set_random)( bmath_asmfx_s* o, bl_t hsm, bl_t pdf, uz_t rd, fx_t density, fx_t min, fx_t max, bcore_prsg* prsg  )
{
    BLM_INIT();
    if( !prsg ) prsg = ( bcore_prsg* )BLM_CREATE( bcore_prsg_lcg_u3_00_s );
    bmath_mfx_s* m1 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    BCATU(bmath_mfx_s,set_size)( m1, o->rows, o->cols );
    BCATU(bmath_mfx_s,set_random)( m1, hsm, pdf, rd, density, min, max, prsg );
    BCATU(bmath_asmfx_s,cpy_dfl_from,mfx)( o, m1 );
    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,set_random_u3)( bmath_asmfx_s* o, bl_t hsm, bl_t pdf, uz_t rd, fx_t density, fx_t min, fx_t max, u3_t* p_rval )
{
    BLM_INIT();
    bmath_mfx_s* m1 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    BCATU(bmath_mfx_s,set_size)( m1, o->rows, o->cols );
    BCATU(bmath_mfx_s,set_random_u3)( m1, hsm, pdf, rd, density, min, max, p_rval );
    BCATU(bmath_asmfx_s,cpy_dfl_from,mfx)( o, m1 );
    BLM_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,zro)( bmath_asmfx_s* o )
{
    bcore_u_memzero( sizeof( fx_t ), o->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,cpy_dfl_from,mfx)( bmath_asmfx_s* o, const bmath_mfx_s* src )
{
    ASSERT( src->rows == o->rows );
    ASSERT( src->cols == o->cols );
    BCATU(bmath_asmfx_s,zro)( o );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const fx_t* b = src->data + src->stride * i;
        const sz_t* x = o->i_data + o->i_stride * i;
        for( sz_t j = 0; j < o->cols; j++ ) o->v_data[ x[ j ] ] += b[ j ];
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,cpy_dfl_from,mfy)( bmath_asmfx_s* o, const bmath_mfy_s* src )
{
    ASSERT( src->rows == o->rows );
    ASSERT( src->cols == o->cols );
    BCATU(bmath_asmfx_s,zro)( o );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const fy_t* b = src->data + src->stride * i;
        const sz_t* x = o->i_data + o->i_stride * i;
        for( sz_t j = 0; j < o->cols; j++ ) o->v_data[ x[ j ] ] += b[ j ];
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,cpy_ifl_to,mfx)( const bmath_asmfx_s* o, bmath_mfx_s* dst )
{
    ASSERT( dst->rows == o->rows );
    ASSERT( dst->cols == o->cols );
    for( sz_t i = 0; i < o->rows; i++ )
    {
              fx_t* b = dst->data + dst->stride * i;
        const sz_t* x = o->i_data + o->i_stride * i;
        for( sz_t j = 0; j < o->cols; j++ ) b[ j ] = o->v_data[ x[ j ] ];
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,cpy_ifl_to,mfy)( const bmath_asmfx_s* o, bmath_mfy_s* dst )
{
    ASSERT( dst->rows == o->rows );
    ASSERT( dst->cols == o->cols );
    for( sz_t i = 0; i < o->rows; i++ )
    {
              fy_t* b = dst->data + dst->stride * i;
        const sz_t* x = o->i_data + o->i_stride * i;
        for( sz_t j = 0; j < o->cols; j++ ) b[ j ] = o->v_data[ x[ j ] ];
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// convolution

void BCATU(bmath_asmfx_s,set_splicing_for_convolution_1d)( bmath_asmfx_s* o, sz_t size_in, sz_t size_kernel, sz_t step )
{
    ASSERT( size_in >= 0 );
    ASSERT( size_kernel >= 0 );
    ASSERT( step > 0 );

    sz_t rows = sz_max( 0, 1 + ( ( size_in - size_kernel ) / step ) );
    sz_t cols = size_kernel;

    BCATU(bmath_asmfx_s,set_size_splicing)( o, rows, cols );
    for( sz_t i = 0; i < rows; i++ )
    {
        for( sz_t j = 0; j < cols; j++ )
        {
            o->i_data[ i * o->i_stride + j ] = i * step + j;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void BCATU(bmath_asmfx_s,set_splicing_for_convolution_2d)( bmath_asmfx_s* o, sz_t rows_in, sz_t cols_in, sz_t rows_kernel, sz_t cols_kernel, sz_t row_step, sz_t col_step )
{
    ASSERT( rows_in >= 0 );
    ASSERT( cols_in >= 0 );
    ASSERT( rows_kernel >= 0 );
    ASSERT( cols_kernel >= 0 );
    ASSERT( row_step > 0 );
    ASSERT( col_step > 0 );

    sz_t row_steps = sz_max( 0, 1 + ( ( rows_in - rows_kernel ) / row_step ) );
    sz_t col_steps = sz_max( 0, 1 + ( ( cols_in - cols_kernel ) / col_step ) );

    o->rows = row_steps * col_steps;
    o->cols = rows_kernel * cols_kernel;

    BCATU(bmath_asmfx_s,set_size_splicing)( o, o->rows, o->cols );

    sz_t o_row = 0;

    for( sz_t rs = 0; rs < row_steps; rs++ )
    {
        sz_t in_row = rs * row_step;
        for( sz_t cs = 0; cs < col_steps; cs++ )
        {
            sz_t in_col = cs * col_step;
            sz_t o_col = 0;

            for( sz_t k_row = 0; k_row < rows_kernel; k_row++ )
            {
                for( sz_t k_col = 0; k_col < cols_kernel; k_col++ )
                {
                    o->i_data[ o_row * o->i_stride + o_col ] = ( in_row + k_row ) * cols_in + in_col + k_col;
                    o_col++;
                }
            }
            o_row++;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// checks, deviations

bl_t BCATU(bmath_asmfx_s,is_near_equ)( const bmath_asmfx_s* o, const bmath_asmfx_s* op, fx_t max_dev )
{
    if( o->rows != op->rows ) return false;
    if( o->cols != op->cols ) return false;

    for( sz_t row = 0; row <= o->rows; row++ )
    {
        for( sz_t col = 0; col <= o->cols; col++ )
        {
            fx_t diff = o->v_data[ o->i_data[ row * o->i_stride + col ] ] - op->v_data[ op->i_data[ row * op->i_stride + col ] ];
            if( BCATU(fx,abs)( diff ) > max_dev ) return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t BCATU(bmath_asmfx_s,is_near_zro)( const bmath_asmfx_s* o, fx_t max_dev )
{
    for( sz_t row = 0; row <= o->rows; row++ )
    {
        for( sz_t col = 0; col <= o->cols; col++ )
        {
            if( BCATU(fx,abs)( o->v_data[ o->i_data[ row * o->i_stride + col ] ] ) > max_dev ) return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// Frobenius norm

fx_t BCATU(bmath_asmfx_s,fdev_equ)( const bmath_asmfx_s* o, const bmath_asmfx_s* op )
{
    ASSERT( o->rows == op->rows );
    ASSERT( o->cols == op->cols );
    fx_t sum = 0;
    for( sz_t row = 0; row <= o->rows; row++ )
    {
        for( sz_t col = 0; col <= o->cols; col++ )
        {
            fx_t diff = o->v_data[ o->i_data[ row * o->i_stride + col ] ] - op->v_data[ op->i_data[ row * op->i_stride + col ] ];
            sum += BCATU(fx,sqr)( diff );
        }
    }
    return ( sum > 0 ) ? BCATU(fx,srt)( sum ) : 0;
}

// ---------------------------------------------------------------------------------------------------------------------

fx_t BCATU(bmath_asmfx_s,fdev_zro)( const bmath_asmfx_s* o )
{
    fx_t sum = 0;
    for( sz_t row = 0; row <= o->rows; row++ )
    {
        for( sz_t col = 0; col <= o->cols; col++ )
        {
            sum += BCATU(fx,sqr)( o->v_data[ o->i_data[ row * o->i_stride + col ] ] );
        }
    }
    return ( sum > 0 ) ? BCATU(fx,srt)( sum ) : 0;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// debugging

void BCATU(bmath_asmfx_s,to_sink)( const bmath_asmfx_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "(#<sz_t> x #<sz_t>)\n", o->rows, o->cols );
    for( sz_t row = 0; row < o->rows; row++ )
    {
        for( sz_t col = 0; col < o->cols; col++ )
        {
            fx_t v = o->v_data[ o->i_data[ row * o->i_stride + col ] ];
            if( col > 0 ) bcore_sink_a_push_fa( sink, ", " );
            bcore_sink_a_pushf( sink, "%9.3g", v );
        }
        bcore_sink_a_push_fa( sink, "\n" );
    }
}

/**********************************************************************************************************************/
// type conversion

er_t BCATU(bmath_asmfx_s,copy_typed)( bmath_asmfx_s* o, tp_t type, vc_t src )
{
    if( !src )
    {
        BCATU(bmath_asmfx_s,clear)( o );
        return 0;
    }

    switch( type )
    {
        case TYPEOF_bmath_asmfx_s:
        {
            BCATU(bmath_asmfx_s,copy)( o, src );
        }
        break;

        case TYPEOF_bmath_asmfy_s:
        {
            const bmath_asmfy_s* v = src;
            BCATU(bmath_asmfx_s,set_size_splicing)( o, v->rows, v->cols );
            BCATU(bmath_asmfx_s,set_size_data)( o, v->v_size );
            bcore_u_memcpy( sizeof( sz_t ), o->i_data, v->i_data, v->i_size );
            bcore_u_memcpy( sizeof( fx_t ), o->v_data, v->v_data, v->v_size );
        }
        break;

        default:
        {
            return bcore_error_push_fa( TYPEOF_conversion_error, "copy_typed to '#<sc_t>': no conversion from '#<sc_t>'.", ifnameof( o->_ ), ifnameof( type ) );
        }
        break;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// testing

// ---------------------------------------------------------------------------------------------------------------------

static vd_t selftest( void )
{
    BLM_INIT();

    bmath_mfx_s*    m1 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_mfx_s*    m2 = BLM_A_PUSH( BCATU(bmath_mfx_s,create)() );
    bmath_asmfx_s* sm1 = BLM_A_PUSH( BCATU(bmath_asmfx_s,create)() );

    {
        sz_t rows = 5;
        sz_t cols = 6;
        u3_t rval = 1234;

        BCATU(bmath_mfx_s,set_size)( m1, rows, cols );
        BCATU(bmath_mfx_s,set_size)( m2, rows, cols );
        BCATU(bmath_asmfx_s,set_size)( sm1, rows, cols );
        BCATU(bmath_mfx_s,set_random_u3)( m1, false, false, 0, 1.0, -1.0, 1.0, &rval );
        BCATU(bmath_asmfx_s,cpy_dfl_from,mfx)( sm1, m1 );
        BCATU(bmath_asmfx_s,cpy_ifl_to,mfx)(   sm1, m2 );
        ASSERT( BCATU(bmath_mfx_s,is_equ)( m1, m2 ) );
    }

    BLM_RETURNV( vd_t, NULL );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

