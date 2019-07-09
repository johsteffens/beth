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

#include "bmath_hf3.h"
#include "bmath_mf3.h"
#include "bmath_vector.h"

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_d_size( bmath_hf3_s* o, sz_t size )
{
    o->d_size = size;
    o->d_data = bcore_un_alloc( sizeof( sz_t ), o->d_data, o->d_space, o->d_size, &o->d_space );
    bcore_u_memzero( sizeof( sz_t ), o->d_data, o->d_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_v_size( bmath_hf3_s* o, sz_t size )
{
    o->v_size = size;
    o->v_data = bcore_un_alloc( sizeof( f3_t ), o->v_data, o->v_space, o->v_size, &o->v_space );
    bcore_u_memzero( sizeof( f3_t ), o->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_fit_v_size( bmath_hf3_s* o )
{
    sz_t size = 1;
    for( sz_t i = 0; i < o->d_size; i++ ) size *= o->d_data[ i ];
    bmath_hf3_s_set_v_size( o, size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_d_data( bmath_hf3_s* o, sz_t* d_data, sz_t d_size )
{
    bmath_hf3_s_set_d_size( o, d_size );
    for( sz_t i = 0; i < o->d_size; i++ ) o->d_data[ i ] = d_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_size( bmath_hf3_s* o, sz_t* d_data, sz_t d_size )
{
    bmath_hf3_s_set_d_data( o, d_data, d_size );
    bmath_hf3_s_fit_v_size( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_d_data_nv( bmath_hf3_s* o, sz_t d_size, va_list sz_t_args )
{
    bmath_hf3_s_set_d_data_nv( o, d_size, sz_t_args );
    bmath_hf3_s_fit_v_size( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_d_data_na( bmath_hf3_s* o, sz_t d_size, ... )
{
    va_list args;
    va_start( args, d_size );
    bmath_hf3_s_set_d_data_nv( o, d_size, args );
    va_end( args );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_size_nv( bmath_hf3_s* o, sz_t d_size, va_list sz_t_args )
{
    bmath_hf3_s_set_d_size( o, d_size );
    for( uz_t i = 0; i < d_size; i++ )
    {
        o->d_data[ i ] = va_arg( sz_t_args, sz_t );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_size_na( bmath_hf3_s* o, sz_t d_size, ... )
{
    va_list args;
    va_start( args, d_size );
    bmath_hf3_s_set_size_nv( o, d_size, args );
    va_end( args );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

sz_t bmath_hf3_s_get_k_for_cols( const bmath_hf3_s* o, sz_t cols )
{
    sz_t p = 1;
    if( cols == p ) return 0;
    for( sz_t i = 0; i < o->d_size; i++ )
    {
        if( cols == p ) return i;
        p *= o->d_data[ i ];
    }
    if( cols == p ) return o->d_size;
    ERR_fa( "Cannot convert holor to matrix with <#sz_t> columns.", cols );
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------

sz_t bmath_hf3_s_get_k_for_rows( const bmath_hf3_s* o, sz_t rows )
{
    sz_t p = 1;
    if( rows == p ) return o->d_size;
    for( sz_t i = o->d_size; i > 0; i-- )
    {
        if( rows == p ) return i;
        p *= o->d_data[ i - 1 ];
    }
    if( rows == p ) return 0;
    ERR_fa( "Cannot convert holor to matrix with <#sz_t> rows.", rows );
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------------

bmath_mf3_s bmath_hf3_s_get_weak_mat( const bmath_hf3_s* o, sz_t k )
{
    ASSERT( k >= 0 && k <= o->d_size );
    sz_t cols = 1;
    sz_t rows = 1;
    for( sz_t i = 0; i < k;         i++ ) cols *= o->d_data[ i ];
    for( sz_t i = k; i < o->d_size; i++ ) rows *= o->d_data[ i ];
    return bmath_mf3_init_weak( rows, cols, cols, o->v_data );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// elementwise operations

// ---------------------------------------------------------------------------------------------------------------------

/// o + m -> r
void bmath_hf3_s_add( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    ASSERT( o->v_size == m->v_size );
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = o->v_data[ i ] + m->v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

/// o - m -> r
void bmath_hf3_s_sub( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    ASSERT( o->v_size == m->v_size );
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = o->v_data[ i ] - m->v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

/// o <*> m -> r (hadamard product)
void bmath_hf3_s_hmul( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    ASSERT( o->v_size == m->v_size );
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = o->v_data[ i ] * m->v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// holor * holor multiplication

/// o * m -> r
void bmath_hf3_s_kmul( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o, k );
    bmath_mf3_s mm = bmath_hf3_s_get_weak_mat( m, bmath_hf3_s_get_k_for_rows( m, mo.cols ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r, bmath_hf3_s_get_k_for_rows( r, mo.rows ) );
    bmath_mf3_s_mul( &mo, &mm, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/// o * m^T -> r
void bmath_hf3_s_kmul_htp( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o, k );
    bmath_mf3_s mm = bmath_hf3_s_get_weak_mat( m, bmath_hf3_s_get_k_for_cols( m, mo.cols ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r, bmath_hf3_s_get_k_for_rows( r, mo.rows ) );
    bmath_mf3_s_mul_htp( &mo, &mm, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/// o^T * m -> r
void bmath_hf3_s_htp_kmul( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o, k );
    bmath_mf3_s mm = bmath_hf3_s_get_weak_mat( m, bmath_hf3_s_get_k_for_rows( m, mo.rows ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r, bmath_hf3_s_get_k_for_rows( r, mo.cols ) );
    bmath_mf3_s_htp_mul( &mo, &mm, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/// ( o * b ) + c -> r
void bmath_hf3_s_kmul_add( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r )
{
    bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o, k );
    bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b, bmath_hf3_s_get_k_for_rows( b, mo.cols ) );
    bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c, bmath_hf3_s_get_k_for_rows( c, mo.rows ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r, bmath_hf3_s_get_k_for_rows( r, mo.rows ) );
    bmath_mf3_s_mul_add( &mo, &mb, &mc, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/// ( o * b^T ) + c -> r
void bmath_hf3_s_kmul_htp_add( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r )
{
    bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o, k );
    bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b, bmath_hf3_s_get_k_for_cols( b, mo.cols ) );
    bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c, bmath_hf3_s_get_k_for_rows( c, mo.rows ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r, bmath_hf3_s_get_k_for_rows( r, mo.rows ) );
    bmath_mf3_s_mul_htp_add( &mo, &mb, &mc, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/// ( o^T * b ) + c -> r
void bmath_hf3_s_htp_kmul_add( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r )
{
    bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o, k );
    bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b, bmath_hf3_s_get_k_for_rows( b, mo.rows ) );
    bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c, bmath_hf3_s_get_k_for_rows( c, mo.cols ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r, bmath_hf3_s_get_k_for_rows( r, mo.cols ) );
    bmath_mf3_s_htp_mul_add( &mo, &mb, &mc, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/** composite multiply-add function. Satisfies functionality of BLAS:DGEMM.
 *  op(a) * op(b) * c + d * e -> r
 *  op(x) = htpx ? x^T : x;
 *  c, e are scalar.
 *  d can be NULL
 */
void bmath_hf3_s_kmul_add_cps( bl_t htpa, const bmath_hf3_s* a, sz_t k, bl_t htpb, const bmath_hf3_s* b, f3_t c, const bmath_hf3_s* d, f3_t e, bmath_hf3_s* r )
{
    bmath_mf3_s ma = bmath_hf3_s_get_weak_mat( a, k );

    bmath_mf3_s mb, md, mr;

    if( !htpa && !htpb )
    {
        mb = bmath_hf3_s_get_weak_mat( b, bmath_hf3_s_get_k_for_rows( b, ma.cols ) );
        md = bmath_hf3_s_get_weak_mat( d, bmath_hf3_s_get_k_for_rows( d, ma.rows ) );
        mr = bmath_hf3_s_get_weak_mat( r, bmath_hf3_s_get_k_for_rows( r, ma.rows ) );
    }
    else if( !htpa && htpb )
    {
        mb = bmath_hf3_s_get_weak_mat( b, bmath_hf3_s_get_k_for_cols( b, ma.cols ) );
        md = bmath_hf3_s_get_weak_mat( d, bmath_hf3_s_get_k_for_rows( d, ma.rows ) );
        mr = bmath_hf3_s_get_weak_mat( r, bmath_hf3_s_get_k_for_rows( r, ma.rows ) );
    }
    else if( htpa && !htpb )
    {
        mb = bmath_hf3_s_get_weak_mat( b, bmath_hf3_s_get_k_for_rows( b, ma.rows ) );
        md = bmath_hf3_s_get_weak_mat( d, bmath_hf3_s_get_k_for_rows( d, ma.cols ) );
        mr = bmath_hf3_s_get_weak_mat( r, bmath_hf3_s_get_k_for_rows( r, ma.cols ) );
    }
    else
    {
        mb = bmath_hf3_s_get_weak_mat( b, bmath_hf3_s_get_k_for_cols( b, ma.rows ) );
        md = bmath_hf3_s_get_weak_mat( d, bmath_hf3_s_get_k_for_rows( d, ma.cols ) );
        mr = bmath_hf3_s_get_weak_mat( r, bmath_hf3_s_get_k_for_rows( r, ma.cols ) );
    }
    bmath_mf3_s_mul_add_cps( htpa, &ma, htpb, &mb, c, &md, e, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bmath_hf3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_hf3" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_get_quicktypes:
        {
        }
        break;

        case TYPEOF_selftest:
        {
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

/**********************************************************************************************************************/

