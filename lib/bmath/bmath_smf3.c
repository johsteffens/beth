/** Copyright 2019 Johannes Bernhard Steffens
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
#include "bmath_smf3.h"


/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_smf3_s )
"{"
    "aware_t _;"

    "sz_t vals;"   // values per xon
    "sz_t xons;"   // xons  per row
    "sz_t rows;"   // number of rows
    "sz_t stride;" // stride of splicing

    "sz_t [] i_arr;" // index data
    "f3_t [] d_arr;" // value data
"}";

// ---------------------------------------------------------------------------------------------------------------------

void bmath_smf3_s_set_size_splicing( bmath_smf3_s* o, sz_t rows, sz_t xons )
{
    o->i_size = rows * xons;
    o->i_data = bcore_un_alloc( sizeof( sz_t ), o->i_data, o->i_space, o->i_size, &o->i_space );
    bcore_u_memzero( sizeof( sz_t ), o->i_data, o->i_size );
    o->rows   = rows;
    o->xons   = xons;
    o->i_stride = xons;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_smf3_s_set_size_data( bmath_smf3_s* o, sz_t size )
{
    o->v_size = size;
    o->v_data = bcore_un_alloc( sizeof( f3_t ), o->v_data, o->v_space, o->v_size, &o->v_space );
    bcore_u_memzero( sizeof( f3_t ), o->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_smf3_s_set_size( bmath_smf3_s* o, sz_t rows, sz_t xons, sz_t vals )
{
    bmath_smf3_s_set_size_splicing( o, rows, xons );
    bmath_smf3_s_set_size_data( o, rows * xons * vals );
    o->vals = vals;
    sz_t k = 0;
    for( sz_t i = 0; i < o->rows; i++ )
    {
        sz_t* a = o->i_data + o->i_stride * i;
        for( sz_t j = 0; j < o->xons; j++ )
        {
            a[ j ] = k * o->vals;
            k++;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_smf3_s_zro( bmath_smf3_s* o )
{
    bcore_u_memzero( sizeof( f3_t ), o->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_smf3_s_cpy_dfl_from_mf3( bmath_smf3_s* o, const bmath_mf3_s* src )
{
    ASSERT( src->rows == o->rows );
    ASSERT( src->cols == o->xons * o->vals );
    bmath_smf3_s_zro( o );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const sz_t* a = o  ->i_data + o  ->i_stride * i;
        const f3_t* b = src->  data + src->  stride * i;
        for( sz_t j = 0; j < o->xons; j++ )
        {
            f3_t* xon = o->v_data + a[ j ];
            for( sz_t k = 0; k < o->vals; k++ ) xon[ k ] += b[ j * o->vals + k ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_smf3_s_cpy_ifl_to_mf3( const bmath_smf3_s* o, bmath_mf3_s* dst )
{
    ASSERT( dst->rows == o->rows );
    ASSERT( dst->cols == o->xons * o->vals );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const sz_t* a = o  ->i_data + o  ->i_stride * i;
              f3_t* b = dst->  data + dst->  stride * i;
        for( sz_t j = 0; j < o->xons; j++ )
        {
            const f3_t* xon = o->v_data + a[ j ];
            for( sz_t k = 0; k < o->vals; k++ ) b[ j * o->vals + k ] = xon[ k ];
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// inspection

void bmath_smf3_s_to_sink( const bmath_smf3_s* o, bcore_sink* sink )
{
    bcore_sink_a_push_fa( sink, "(#<sz_t> x #<sz_t> x #<sz_t>)\n", o->rows, o->xons, o->vals );
    for( sz_t i = 0; i < o->rows; i++ )
    {
        const sz_t* a = o  ->i_data + o  ->i_stride * i;
        for( sz_t j = 0; j < o->xons; j++ )
        {
            const f3_t* xon = o->v_data + a[ j ];
            bcore_sink_a_push_fa( sink, "(" );

            for( sz_t k = 0; k < o->vals; k++ )
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
    BCORE_LIFE_CREATE( bmath_smf3_s, sm1 );

    {
        sz_t rows = 5;
        sz_t xons = 2;
        sz_t vals = 3;
        sz_t cols = xons * vals;

        u2_t rval = 1234;

        bmath_mf3_s_set_size( m1, rows, cols );
        bmath_mf3_s_set_size( m2, rows, cols );
        bmath_smf3_s_set_size( sm1, rows, xons, vals );
        bmath_mf3_s_set_random( m1, false, false, 0, 1.0, -1.0, 1.0, &rval );
        bmath_smf3_s_cpy_dfl_from_mf3( sm1, m1 );
        bmath_smf3_s_cpy_ifl_to_mf3(   sm1, m2 );
        ASSERT( bmath_mf3_s_is_equ( m1, m2 ) );

    }

    BCORE_LIFE_DOWN();
    return NULL;
}

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bmath_smf3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_smf3" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_smf3_s );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_smf3_s );
        }
        break;

        case TYPEOF_selftest:
        {
            selftest();
            return NULL;
        }
        break;

        default: break;
    }

    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


