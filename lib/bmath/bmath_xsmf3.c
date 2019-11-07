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

#include "bcore_std.h"
#include "bmath_std.h"
#include "bmath_xsmf3.h"

/**********************************************************************************************************************/
#ifdef TYPEOF_bmath_xsmf3_s

// ---------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_set_size_splicing( bmath_xsmf3_s* o, sz_t rows, sz_t xons )
{
    o->i_size = xons * rows;
    o->i_data = bcore_un_alloc( sizeof( sz_t ), o->i_data, o->i_space, o->i_size, &o->i_space );
    bcore_u_memzero( sizeof( sz_t ), o->i_data, o->i_size );
    o->rows = rows;
    o->xons = xons;
    o->i_stride = rows;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_set_size_data( bmath_xsmf3_s* o, sz_t size )
{
    o->v_size = size;
    o->v_data = bcore_un_alloc( sizeof( f3_t ), o->v_data, o->v_space, o->v_size, &o->v_space );
    bcore_u_memzero( sizeof( f3_t ), o->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_set_size( bmath_xsmf3_s* o, sz_t rows, sz_t xons, sz_t slos )
{
    bmath_xsmf3_s_set_size_splicing( o, rows, xons );
    bmath_xsmf3_s_set_size_data( o, rows * xons * slos );
    o->slos = slos;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        for( sz_t j = 0; j < o->xons; j++ ) o->i_data[ j * o->i_stride + i ] = ( i * o->xons + j ) * o->slos;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_fit_size_data( bmath_xsmf3_s* o )
{
    sz_t max_idx = 0;
    for( sz_t j = 0; j < o->xons; j++ )
    {
        for( sz_t i = 0; i < o->rows; i++ )
        {
            max_idx = sz_max( o->i_data[ j * o->i_stride + i ], max_idx );
        }
    }
    bmath_xsmf3_s_set_size_data( o, max_idx + o->slos );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_set_random( bmath_xsmf3_s* o, bl_t hsm, bl_t pdf, uz_t rd, f3_t density, f3_t min, f3_t max, u2_t* p_rval )
{
    BCORE_LIFE_INIT();
    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    bmath_mf3_s_set_size( m1, o->rows, o->xons * o->slos );
    bmath_mf3_s_set_random( m1, hsm, pdf, rd, density, min, max, p_rval );
    bmath_xsmf3_s_cpy_dfl_from_mf3( o, m1 );
    BCORE_LIFE_DOWN();
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_zro( bmath_xsmf3_s* o )
{
    bcore_u_memzero( sizeof( f3_t ), o->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_cpy_dfl_from_mf3( bmath_xsmf3_s* o, const bmath_mf3_s* src )
{
    ASSERT( src->rows == o->rows );
    ASSERT( src->cols == o->xons * o->slos );
    bmath_xsmf3_s_zro( o );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const f3_t* b = src->data + src->stride * i;
        for( sz_t j = 0; j < o->xons; j++ )
        {
            f3_t* xon = o->v_data + o->i_data[ j * o->i_stride + i ];
            for( sz_t k = 0; k < o->slos; k++ ) xon[ k ] += b[ j * o->slos + k ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_cpy_ifl_to_mf3( const bmath_xsmf3_s* o, bmath_mf3_s* dst )
{
    ASSERT( dst->rows == o->rows );
    ASSERT( dst->cols == o->xons * o->slos );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        f3_t* b = dst->data + dst->stride * i;
        for( sz_t j = 0; j < o->xons; j++ )
        {
            const f3_t* xon = o->v_data + o->i_data[ j * o->i_stride + i ];
            for( sz_t k = 0; k < o->slos; k++ ) b[ j * o->slos + k ] = xon[ k ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// convolution

void bmath_xsmf3_s_set_splicing_for_convolution_1d( bmath_xsmf3_s* o, sz_t size_in, sz_t size_kernel, sz_t step )
{
    ASSERT( size_in >= 0 );
    ASSERT( size_kernel >= 0 );
    ASSERT( step > 0 );

    sz_t rows = sz_max( 0, 1 + ( ( size_in - size_kernel ) / step ) );
    bmath_xsmf3_s_set_size_splicing( o, rows, 1 );
    for( sz_t i = 0; i < rows; i++ ) o->i_data[ i ] = i * step;
    o->slos = size_kernel;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_xsmf3_s_set_splicing_for_convolution_2d( bmath_xsmf3_s* o, sz_t rows_in, sz_t cols_in, sz_t rows_kernel, sz_t cols_kernel, sz_t row_step, sz_t col_step )
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
    o->xons = rows_kernel;
    o->slos = cols_kernel;

    bmath_xsmf3_s_set_size_splicing( o, o->rows, o->xons );

    sz_t o_row = 0;

    for( sz_t rs = 0; rs < row_steps; rs++ )
    {
        sz_t in_row = rs * row_step;
        for( sz_t cs = 0; cs < col_steps; cs++ )
        {
            sz_t in_col = cs * col_step;
            for( sz_t xon = 0; xon < o->xons; xon++ )
            {
                o->i_data[ xon * o->i_stride + o_row ] = ( in_row + xon ) * cols_in + in_col;
            }
            o_row++;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// checks, deviations

bl_t bmath_xsmf3_s_is_near_equ( const bmath_xsmf3_s* o, const bmath_xsmf3_s* op, f3_t max_dev )
{
    if( o->rows != op->rows ) return false;
    if( o->xons != op->xons ) return false;
    if( o->slos != op->slos ) return false;

    for( sz_t row = 0; row <= o->rows; row++ )
    {
        for( sz_t xon = 0; xon <= o->xons; xon++ )
        {
            for( sz_t slo = 0; slo <= o->slos; slo++ )
            {
                f3_t diff = o->v_data[ o->i_data[ xon * o->i_stride + row ] + slo ] - op->v_data[ op->i_data[ xon * op->i_stride + row ] + slo ];
                if( f3_abs( diff ) > max_dev ) return false;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bmath_xsmf3_s_is_near_zro( const bmath_xsmf3_s* o, f3_t max_dev )
{
    for( sz_t row = 0; row <= o->rows; row++ )
    {
        for( sz_t xon = 0; xon <= o->xons; xon++ )
        {
            for( sz_t slo = 0; slo <= o->slos; slo++ )
            {
                if( f3_abs( o->v_data[ o->i_data[ xon * o->i_stride + row ] + slo ] ) > max_dev ) return false;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// Frobenius norm

f3_t bmath_xsmf3_s_fdev_equ( const bmath_xsmf3_s* o, const bmath_xsmf3_s* op )
{
    ASSERT( o->rows == op->rows );
    ASSERT( o->xons == op->xons );
    ASSERT( o->slos == op->slos );
    f3_t sum = 0;
    for( sz_t row = 0; row <= o->rows; row++ )
    {
        for( sz_t xon = 0; xon <= o->xons; xon++ )
        {
            for( sz_t slo = 0; slo <= o->slos; slo++ )
            {
                f3_t diff = o->v_data[ o->i_data[ xon * o->i_stride + row ] + slo ] - op->v_data[ op->i_data[ xon * op->i_stride + row ] + slo ];
                sum += f3_sqr( diff );
            }
        }
    }
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

// ---------------------------------------------------------------------------------------------------------------------

f3_t bmath_xsmf3_s_fdev_zro( const bmath_xsmf3_s* o )
{
    f3_t sum = 0;
    for( sz_t row = 0; row <= o->rows; row++ )
    {
        for( sz_t xon = 0; xon <= o->xons; xon++ )
        {
            for( sz_t slo = 0; slo <= o->slos; slo++ )
            {
                sum += f3_sqr( o->v_data[ o->i_data[ xon * o->i_stride + row ] + slo ] );
            }
        }
    }
    return ( sum > 0 ) ? f3_srt( sum ) : 0;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// debugging

void bmath_xsmf3_s_to_sink( const bmath_xsmf3_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "(#<sz_t> x #<sz_t> x #<sz_t>)\n", o->rows, o->xons, o->slos );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        for( sz_t j = 0; j < o->xons; j++ )
        {
            const f3_t* xon = o->v_data + o->i_data[ j * o->i_stride + i ];
            bcore_sink_a_push_fa( sink, "(" );

            for( sz_t k = 0; k < o->slos; k++ )
            {
                if( k > 0 ) bcore_sink_a_push_fa( sink, ", " );
                bcore_sink_a_pushf( sink, "%9.3g", xon[ k ] );
            }
            bcore_sink_a_push_fa( sink, ")" );
        }
        bcore_sink_a_push_fa( sink, "\n" );
    }
}

/**********************************************************************************************************************/
// testing

// ---------------------------------------------------------------------------------------------------------------------

static vd_t selftest( void )
{
    BCORE_LIFE_INIT();

    BCORE_LIFE_CREATE( bmath_mf3_s, m1 );
    BCORE_LIFE_CREATE( bmath_mf3_s, m2 );
    BCORE_LIFE_CREATE( bmath_xsmf3_s, sm1 );

    {
        sz_t rows = 5;
        sz_t xons = 2;
        sz_t slos = 3;
        sz_t cols = xons * slos;

        u2_t rval = 1234;

        bmath_mf3_s_set_size( m1, rows, cols );
        bmath_mf3_s_set_size( m2, rows, cols );
        bmath_xsmf3_s_set_size( sm1, rows, xons, slos );
        bmath_mf3_s_set_random( m1, false, false, 0, 1.0, -1.0, 1.0, &rval );
        bmath_xsmf3_s_cpy_dfl_from_mf3( sm1, m1 );
        bmath_xsmf3_s_cpy_ifl_to_mf3(   sm1, m2 );
        ASSERT( bmath_mf3_s_is_equ( m1, m2 ) );

    }

    BCORE_LIFE_DOWN();
    return NULL;
}

#endif // TYPEOF_bmath_xsmf3_s

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bmath_xsmf3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_xsmf3" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            selftest();
            return NULL;
        }
        break;

        case TYPEOF_plant:
        {
            bcore_plant_compile( "bmath_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


