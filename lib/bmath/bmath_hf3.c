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

void bmath_hf3_s_d_make_strong( bmath_hf3_s* o )
{
    if( o->d_size  == 0 ) return;
    if( o->d_space != 0 ) return;
    sz_t  d_size = o->d_size;
    sz_t* d_data = o->d_data;
    o->d_size = 0;
    o->d_data = NULL;
    bmath_hf3_s_set_d_size( o, d_size );
    bcore_u_memcpy( sizeof( sz_t ), o->d_data, d_data, d_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_v_make_strong( bmath_hf3_s* o )
{
    if( o->v_size  == 0 ) return;
    if( o->v_space != 0 ) return;
    sz_t  v_size = o->v_size;
    f3_t* v_data = o->v_data;
    o->v_size = 0;
    o->v_data = NULL;
    bmath_hf3_s_set_v_size( o, v_size );
    bcore_u_memcpy( sizeof( f3_t ), o->v_data, v_data, v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_resize_d_size( bmath_hf3_s* o, sz_t size )
{
    sz_t old_size = o->d_size;
    bmath_hf3_s_d_make_strong( o );
    o->d_size = size;
    o->d_data = bcore_un_alloc( sizeof( sz_t ), o->d_data, o->d_space, o->d_size, &o->d_space );
    for( sz_t i = old_size; i < size; i++ ) o->d_data[ i ] = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_resize_v_size( bmath_hf3_s* o, sz_t size )
{
    sz_t old_size = o->v_size;
    bmath_hf3_s_v_make_strong( o );
    o->v_size = size;
    o->v_data = bcore_un_alloc( sizeof( f3_t ), o->v_data, o->v_space, o->v_size, &o->v_space );
    for( sz_t i = old_size; i < size; i++ ) o->v_data[ i ] = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_d_size( bmath_hf3_s* o, sz_t size )
{
    o->d_size = size;
    if( o->d_space > 0 )
    {
        o->d_data = bcore_un_alloc( sizeof( sz_t ), o->d_data, o->d_space, o->d_size, &o->d_space );
    }
    else
    {
        o->d_data = bcore_un_alloc( sizeof( sz_t ), NULL, 0, o->d_size, &o->d_space );
    }
    bcore_u_memzero( sizeof( sz_t ), o->d_data, o->d_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_v_size( bmath_hf3_s* o, sz_t size )
{
    o->v_size = size;
    if( o->v_space > 0 )
    {
        o->v_data = bcore_un_alloc( sizeof( f3_t ), o->v_data, o->v_space, o->v_size, &o->v_space );
    }
    else
    {
        o->v_data = bcore_un_alloc( sizeof( f3_t ), NULL, 0, o->v_size, &o->v_space );
    }
    bcore_u_memzero( sizeof( f3_t ), o->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_clear_v_data( bmath_hf3_s* o )
{
    if( o->v_space > 0 )
    {
        o->v_data = bcore_un_alloc( sizeof( f3_t ), o->v_data, o->v_space, 0, &o->v_space );
    }
    o->v_data = NULL;
    o->v_size = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_clear_d_data( bmath_hf3_s* o )
{
    if( o->d_space > 0 )
    {
        o->d_data = bcore_un_alloc( sizeof( sz_t ), o->d_data, o->d_space, 0, &o->d_space );
    }
    o->d_data = NULL;
    o->d_size = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_fit_v_size( bmath_hf3_s* o )
{
    sz_t size = 1;
    for( sz_t i = 0; i < o->d_size; i++ ) size *= o->d_data[ i ];
    bmath_hf3_s_set_v_size( o, size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_d_data( bmath_hf3_s* o, const sz_t* d_data, sz_t d_size )
{
    bmath_hf3_s_set_d_size( o, d_size );
    for( sz_t i = 0; i < o->d_size; i++ ) o->d_data[ i ] = d_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_v_data( bmath_hf3_s* o, const f3_t* v_data, sz_t v_size )
{
    bmath_hf3_s_set_v_size( o, v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) o->v_data[ i ] = v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_copy_d_data( bmath_hf3_s* o, const bmath_hf3_s* src )
{
    bmath_hf3_s_set_d_data( o, src->d_data, src->d_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_copy_v_data( bmath_hf3_s* o, const bmath_hf3_s* src )
{
    bmath_hf3_s_set_v_data( o, src->v_data, src->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_size( bmath_hf3_s* o, const sz_t* d_data, sz_t d_size )
{
    bmath_hf3_s_set_d_data( o, d_data, d_size );
    bmath_hf3_s_fit_v_size( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_copy_size( bmath_hf3_s* o, const bmath_hf3_s* src )
{
    bmath_hf3_s_set_size( o, src->d_data, src->d_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_d_data_nv( bmath_hf3_s* o, sz_t d_size, va_list sz_t_args )
{
    bmath_hf3_s_set_d_size( o, d_size );
    for( uz_t i = 0; i < d_size; i++ )
    {
        o->d_data[ i ] = va_arg( sz_t_args, sz_t );
    }
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
    bmath_hf3_s_set_d_data_nv( o, d_size, sz_t_args );
    bmath_hf3_s_fit_v_size( o );
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

sz_t bmath_hf3_s_d_product( const bmath_hf3_s* o )
{
    sz_t prd = 1;
    for( sz_t i = 0; i < o->d_size; i++ ) prd *= o->d_data[ i ];
    return prd;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// comparison

// ---------------------------------------------------------------------------------------------------------------------

bl_t bmath_hf3_s_d_equal( const bmath_hf3_s* o, const bmath_hf3_s* src )
{
    if( o->d_size != src->d_size ) return false;
    for( sz_t i = 0; i < o->d_size; i++ ) if( o->d_data[ i ] != src->d_data[ i ] ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bmath_hf3_s_v_equal( const bmath_hf3_s* o, const bmath_hf3_s* src )
{
    if( o->v_size != src->v_size ) return false;
    for( sz_t i = 0; i < o->v_size; i++ ) if( o->v_data[ i ] != src->v_data[ i ] ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_inc_order( bmath_hf3_s* o, sz_t dim )
{
    bmath_hf3_s_d_make_strong( o );

    o->d_size++;
    o->d_data = bcore_un_alloc( sizeof( sz_t ), o->d_data, o->d_space, o->d_size, &o->d_space );
    o->d_data[ o->d_size - 1 ] = dim;

    if( o->v_size > 0 )
    {
        bmath_hf3_s_v_make_strong( o );

        f3_t* v_data  = o->v_data;
        sz_t  v_size  = o->v_size;
        sz_t  v_space = o->v_space;
        o->v_data = NULL;
        o->v_size = 0;
        o->v_space = 0;
        bmath_hf3_s_fit_v_size( o );
        ASSERT( o->v_size == v_size * dim );

        for( sz_t i = 0; i < dim; i++ )
        {
            bcore_u_memcpy( sizeof( f3_t ), o->v_data + v_size * i, v_data, v_size );
        }

        bcore_un_alloc( sizeof( f3_t ), v_data, v_space, 0, NULL );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_push( bmath_hf3_s* o, const bmath_hf3_s* src )
{
    if( o->d_size == 0 && o->v_size == 0 )
    {
        bmath_hf3_s_copy( o, src );
        bmath_hf3_s_inc_order( o, 1 );
        return;
    }

    ASSERT( o->v_data );
    ASSERT( src->v_data );
    ASSERT( src->d_size <= o->d_size );

    for( sz_t i = 0; i < src->d_size; i++ ) ASSERT( o->d_data[ i ] == src->d_data[ i ] );
    if( src->d_size == o->d_size ) bmath_hf3_s_inc_order( o, 1 );

    ASSERT( o->d_size > 0 );

    bmath_hf3_s_v_make_strong( o );
    sz_t sub_size = bmath_hf3_s_d_product( src );

    o->d_data[ o->d_size -1 ]++;

    bmath_hf3_s_resize_v_size( o, o->v_size + sub_size );
    bcore_u_memcpy( sizeof( f3_t ), o->v_data + o->v_size - sub_size, src->v_data, sub_size );
}

// ---------------------------------------------------------------------------------------------------------------------

static void rec_to_sink( const bmath_hf3_s* o, bcore_sink* sink, f3_t* v_data, sz_t d_idx )
{
    if( d_idx == 0 )
    {
        for( sz_t i = 0; i < o->d_data[ 0 ]; i++ )
        {
            if( i > 0 ) bcore_sink_a_push_fa( sink, " " );
            bcore_sink_a_push_fa( sink, "#<f3_t>", v_data[ i ] );
        }
    }
    else
    {
        sz_t v_block = 1;
        for( sz_t i = 0; i < d_idx; i++ ) v_block *= o->d_data[ i ];
        sz_t dim = o->d_data[ d_idx ];
        for( sz_t i = 0; i < dim; i++ )
        {
            bcore_sink_a_push_fa( sink, "{" );
            rec_to_sink( o, sink, v_data + i * v_block, d_idx - 1 );
            bcore_sink_a_push_fa( sink, "}" );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_to_sink( const bmath_hf3_s* o, bcore_sink* sink )
{
    if( o->v_data )
    {
        if( o->d_size > 0 )
        {
            bcore_sink_a_push_fa( sink, "{" );
            rec_to_sink( o, sink, o->v_data, o->d_size - 1 );
            bcore_sink_a_push_fa( sink, "}" );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "#<f3_t>", o->v_data[ 0 ] );
        }
    }
    else
    {
        for( sz_t i = o->d_size - 1; i >= 0; i-- )
        {
            bcore_sink_a_push_fa( sink, "[#<sz_t>]", o->d_data[ i ] );
        }
        bcore_sink_a_push_fa( sink, "##" );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_to_sink_nl( const bmath_hf3_s* o, bcore_sink* sink )
{
    bmath_hf3_s_to_sink( o, sink );
    bcore_sink_a_push_fa( sink, "\n" );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_to_stdout( const bmath_hf3_s* o, bcore_sink* sink )
{
    bmath_hf3_s_to_sink( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_to_stdout_nl( const bmath_hf3_s* o, bcore_sink* sink )
{
    bmath_hf3_s_to_sink_nl( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_random( bmath_hf3_s* o, f3_t density, f3_t min, f3_t max, u2_t* p_rval )
{
    u2_t rval = p_rval ? *p_rval : 12345;
    f3_t range = max - min;
    for( uz_t i = 0; i < o->v_size; i++ )
    {
        if( f3_xsg1_pos( &rval ) < density )
        {
            o->v_data[ i ] = ( range * f3_xsg1_pos( &rval ) ) + min;
        }
        else
        {
            o->v_data[ i ] = 0;
        }
    }
    if( p_rval ) *p_rval = rval;
}

//----------------------------------------------------------------------------------------------------------------------

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

bmath_mf3_s bmath_hf3_s_get_weak_kmat( const bmath_hf3_s* o, sz_t k )
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

void bmath_hf3_s_zro( const bmath_hf3_s* o )
{
    for( sz_t i = 0; i < o->v_size; i++ ) o->v_data[ i ] = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_cpy( const bmath_hf3_s* o, bmath_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = o->v_data[ i ];
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_tanh( const bmath_hf3_s* o, bmath_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = tanh( o->v_data[ i ] );
}

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

void bmath_hf3_s_mul_scl( const bmath_hf3_s* o, const f3_t* b, bmath_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    bmath_f3_t_vec_mul_scl( o->v_data, *b, r->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_mul_scl_add( const bmath_hf3_s* o, const f3_t* b, const bmath_hf3_s* c, bmath_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    ASSERT( o->v_size == c->v_size );
    bmath_f3_t_vec_mul_scl_add( o->v_data, *b, c->v_data, r->v_data, o->v_size );
}

// ---------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_f3_max( const f3_t* v1, uz_t size )
{
    f3_t max = ( size > 0 ) ? v1[ 0 ] : 0;
    for( uz_t i = 1; i < size; i++ ) max = f3_max( max, v1[ i ] );
    return max;
}

//----------------------------------------------------------------------------------------------------------------------

static f3_t f3_s_f3_min( const f3_t* v1, uz_t size )
{
    f3_t min = ( size > 0 ) ? v1[ 0 ] : 0;
    for( uz_t i = 1; i < size; i++ ) min = f3_min( min, v1[ i ] );
    return min;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_hf3_s_f3_sum( const bmath_hf3_s* o )
{
    return bmath_f3_t_vec_sum( o->v_data, o->v_size );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_hf3_s_f3_avg( const bmath_hf3_s* o )
{
    return o->v_size > 0 ? bmath_hf3_s_f3_sum( o ) / o->v_size : 0;
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_hf3_s_f3_max( const bmath_hf3_s* o )
{
    return f3_s_f3_max( o->v_data, o->v_size );
}

//----------------------------------------------------------------------------------------------------------------------

f3_t bmath_hf3_s_f3_min( const bmath_hf3_s* o )
{
    return f3_s_f3_min( o->v_data, o->v_size );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// specific (max order 2) holor * holor multiplications

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_bmul( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( m->d_size <= 2 );
    ASSERT( r != o );
    ASSERT( r != m );
    assert( o->v_size );
    assert( m->v_size );
    assert( r->v_size );

    switch( o->d_size * 3 + m->d_size )
    {
        case 0: // 0H, 0H
        {
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = o->v_data[ 0 ] * m->v_data[ 0 ];
        }
        break;

        case 1: // 0H, 1H
        {
            ASSERT( m->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( m, o->v_data, r );
        }
        break;

        case 2: // 0H, 2H
        {
            ASSERT( m->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( m->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl( m, o->v_data, r );
        }
        break;

        case 3: // 1H, 0H
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( o, m->v_data, r );
        }
        break;

        case 4: // 1H, 1H
        {
            ERR_fa( "bmul(1H,1H) not defined." );
        }
        break;

        case 5: // 1H, 2H
        {
            ERR_fa( "bmul(1H,2H) not defined." );
        }
        break;

        case 6: // 2H, 0H
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( o->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl( o, m->v_data, r );
        }
        break;

        case 7: // 2H, 1H  (matrix * vector)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_vf3_s vm = bmath_hf3_s_get_weak_vec( m );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_mf3_s_mul_vec( &mo, &vm, &vr );
        }
        break;

        case 8: // 2H, 2H  (matrix * matrix)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mm = bmath_hf3_s_get_weak_mat( m );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_mul( &mo, &mm, &mr );
        }
        break;

        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_bmul_htp( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( m->d_size <= 2 );
    ASSERT( r != o );
    ASSERT( r != m );
    assert( o->v_size );
    assert( m->v_size );
    assert( r->v_size );

    switch( o->d_size * 3 + m->d_size )
    {
        case 0: // 0H, 0H
        {
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = o->v_data[ 0 ] * m->v_data[ 0 ];
        }
        break;

        case 1: // 0H, 1H
        {
            ASSERT( m->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( m, o->v_data, r );
        }
        break;

        case 2: // 0H, 2H
        {
            bmath_mf3_s mm = bmath_hf3_s_get_weak_mat( m );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_htp( &mm, &mr );
            bmath_mf3_s_mul_scl( &mr, o->v_data, &mr );
        }
        break;

        case 3: // 1H, 0H
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( o, m->v_data, r );
        }
        break;

        case 4: // 1H, 1H  (outer product)
        {
            bmath_vf3_s vo = bmath_hf3_s_get_weak_vec( o );
            bmath_vf3_s vm = bmath_hf3_s_get_weak_vec( m );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_opd( &mr, &vo, &vm );
        }
        break;

        case 5: // 1H, 2H
        {
            bmath_vf3_s vo = bmath_hf3_s_get_weak_vec( o );
            bmath_mf3_s mm = bmath_hf3_s_get_weak_mat( m );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_mf3_s_mul_vec( &mm, &vo, &vr );
        }
        break;

        case 6: // 2H, 0H
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( o->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl( o, m->v_data, r );
        }
        break;

        case 7: // 2H, 1H
        {
            ERR_fa( "bmul_htp(2H,1H) not defined." );
        }
        break;

        case 8: // 2H, 2H
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mm = bmath_hf3_s_get_weak_mat( m );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_mul_htp( &mo, &mm, &mr );
        }
        break;

        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_htp_bmul( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( m->d_size <= 2 );
    ASSERT( r != o );
    ASSERT( r != m );
    assert( o->v_size );
    assert( m->v_size );
    assert( r->v_size );

    switch( o->d_size * 3 + m->d_size )
    {
        case 0: // 0H, 0H
        {
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = o->v_data[ 0 ] * m->v_data[ 0 ];
        }
        break;

        case 1: // 0H, 1H
        {
            ASSERT( m->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( m, o->v_data, r );
        }
        break;

        case 2: // 0H, 2H
        {
            ASSERT( m->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( m->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl( m, o->v_data, r );
        }
        break;

        case 3: // 1H, 0H
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( o, m->v_data, r );
        }
        break;

        case 4: // 1H, 1H  (VT V) dot product
        {
            ASSERT( o->d_data[ 0 ] == m->d_data[ 0 ] );
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = bmath_f3_t_vec_mul_vec( o->v_data, m->v_data, o->v_size );
        }
        break;

        case 5: // 1H, 2H
        {
            bmath_vf3_s vo = bmath_hf3_s_get_weak_vec( o );
            bmath_mf3_s mm = bmath_hf3_s_get_weak_mat( m );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_mf3_s_htp_mul_vec( &mm, &vo, &vr );
        }
        break;

        case 6: // 2H, 0H
        {
            bmath_mf3_s om = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_htp( &om, &mr );
            bmath_mf3_s_mul_scl( &mr, m->v_data, &mr );
        }
        break;

        case 7: // 2H, 1H  (matrix * vector)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_vf3_s vm = bmath_hf3_s_get_weak_vec( m );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_mf3_s_htp_mul_vec( &mo, &vm, &vr );
        }
        break;

        case 8: // 2H, 2H  (matrix * matrix)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mm = bmath_hf3_s_get_weak_mat( m );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_htp_mul( &mo, &mm, &mr );
        }
        break;

        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_bmul_add( const bmath_hf3_s* o, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r );

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_bmul_htp_add( const bmath_hf3_s* o, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r );

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_htp_bmul_add( const bmath_hf3_s* o, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r );

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// holor * holor multiplication

/// o * m -> r
void bmath_hf3_s_kmul( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    ASSERT( r != o );
    ASSERT( r != m );
    bmath_mf3_s mo = bmath_hf3_s_get_weak_kmat( o, k );
    bmath_mf3_s mm = bmath_hf3_s_get_weak_kmat( m, bmath_hf3_s_get_k_for_rows( m, mo.cols ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_kmat( r, bmath_hf3_s_get_k_for_rows( r, mo.rows ) );
    bmath_mf3_s_mul( &mo, &mm, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/// o * m^T -> r
void bmath_hf3_s_kmul_htp( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    ASSERT( r != o );
    ASSERT( r != m );
    bmath_mf3_s mo = bmath_hf3_s_get_weak_kmat( o, k );
    bmath_mf3_s mm = bmath_hf3_s_get_weak_kmat( m, bmath_hf3_s_get_k_for_cols( m, mo.cols ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_kmat( r, bmath_hf3_s_get_k_for_rows( r, mo.rows ) );
    bmath_mf3_s_mul_htp( &mo, &mm, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/// o^T * m -> r
void bmath_hf3_s_htp_kmul( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* m, bmath_hf3_s* r )
{
    ASSERT( r != o );
    ASSERT( r != m );
    bmath_mf3_s mo = bmath_hf3_s_get_weak_kmat( o, k );
    bmath_mf3_s mm = bmath_hf3_s_get_weak_kmat( m, bmath_hf3_s_get_k_for_rows( m, mo.rows ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_kmat( r, bmath_hf3_s_get_k_for_rows( r, mo.cols ) );
    bmath_mf3_s_htp_mul( &mo, &mm, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/// ( o * b ) + c -> r
void bmath_hf3_s_kmul_add( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r )
{
    ASSERT( r != o );
    ASSERT( r != b );
    ASSERT( r != c );
    bmath_mf3_s mo = bmath_hf3_s_get_weak_kmat( o, k );
    bmath_mf3_s mb = bmath_hf3_s_get_weak_kmat( b, bmath_hf3_s_get_k_for_rows( b, mo.cols ) );
    bmath_mf3_s mc = bmath_hf3_s_get_weak_kmat( c, bmath_hf3_s_get_k_for_rows( c, mo.rows ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_kmat( r, bmath_hf3_s_get_k_for_rows( r, mo.rows ) );
    bmath_mf3_s_mul_add( &mo, &mb, &mc, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/// ( o * b^T ) + c -> r
void bmath_hf3_s_kmul_htp_add( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r )
{
    ASSERT( r != o );
    ASSERT( r != b );
    ASSERT( r != c );
    bmath_mf3_s mo = bmath_hf3_s_get_weak_kmat( o, k );
    bmath_mf3_s mb = bmath_hf3_s_get_weak_kmat( b, bmath_hf3_s_get_k_for_cols( b, mo.cols ) );
    bmath_mf3_s mc = bmath_hf3_s_get_weak_kmat( c, bmath_hf3_s_get_k_for_rows( c, mo.rows ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_kmat( r, bmath_hf3_s_get_k_for_rows( r, mo.rows ) );
    bmath_mf3_s_mul_htp_add( &mo, &mb, &mc, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/// ( o^T * b ) + c -> r
void bmath_hf3_s_htp_kmul_add( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r )
{
    ASSERT( r != o );
    ASSERT( r != b );
    ASSERT( r != c );
    bmath_mf3_s mo = bmath_hf3_s_get_weak_kmat( o, k );
    bmath_mf3_s mb = bmath_hf3_s_get_weak_kmat( b, bmath_hf3_s_get_k_for_rows( b, mo.rows ) );
    bmath_mf3_s mc = bmath_hf3_s_get_weak_kmat( c, bmath_hf3_s_get_k_for_rows( c, mo.cols ) );
    bmath_mf3_s mr = bmath_hf3_s_get_weak_kmat( r, bmath_hf3_s_get_k_for_rows( r, mo.cols ) );
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
    ASSERT( r != a );
    ASSERT( r != b );
    ASSERT( r != d );

    bmath_mf3_s ma = bmath_hf3_s_get_weak_kmat( a, k );

    bmath_mf3_s mb, md, mr;

    if( !htpa && !htpb )
    {
        mb = bmath_hf3_s_get_weak_kmat( b, bmath_hf3_s_get_k_for_rows( b, ma.cols ) );
        md = bmath_hf3_s_get_weak_kmat( d, bmath_hf3_s_get_k_for_rows( d, ma.rows ) );
        mr = bmath_hf3_s_get_weak_kmat( r, bmath_hf3_s_get_k_for_rows( r, ma.rows ) );
    }
    else if( !htpa && htpb )
    {
        mb = bmath_hf3_s_get_weak_kmat( b, bmath_hf3_s_get_k_for_cols( b, ma.cols ) );
        md = bmath_hf3_s_get_weak_kmat( d, bmath_hf3_s_get_k_for_rows( d, ma.rows ) );
        mr = bmath_hf3_s_get_weak_kmat( r, bmath_hf3_s_get_k_for_rows( r, ma.rows ) );
    }
    else if( htpa && !htpb )
    {
        mb = bmath_hf3_s_get_weak_kmat( b, bmath_hf3_s_get_k_for_rows( b, ma.rows ) );
        md = bmath_hf3_s_get_weak_kmat( d, bmath_hf3_s_get_k_for_rows( d, ma.cols ) );
        mr = bmath_hf3_s_get_weak_kmat( r, bmath_hf3_s_get_k_for_rows( r, ma.cols ) );
    }
    else
    {
        mb = bmath_hf3_s_get_weak_kmat( b, bmath_hf3_s_get_k_for_cols( b, ma.rows ) );
        md = bmath_hf3_s_get_weak_kmat( d, bmath_hf3_s_get_k_for_rows( d, ma.cols ) );
        mr = bmath_hf3_s_get_weak_kmat( r, bmath_hf3_s_get_k_for_rows( r, ma.cols ) );
    }
    bmath_mf3_s_mul_add_cps( htpa, &ma, htpb, &mb, c, &md, e, &mr );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

static void selftest( void )
{
    BCORE_LIFE_INIT();

    BCORE_LIFE_CREATE( bmath_hf3_s, h1 );
    BCORE_LIFE_CREATE( bmath_hf3_s, h2 );

    bmath_hf3_s_set_size_na( h1, 3, 2, 2, 7 );
    bmath_hf3_s_copy_size( h2, h1 );

    u2_t rval = 123;
    bmath_hf3_s_set_random( h1, 1.0, 0, 1, &rval );
    bmath_hf3_s_to_stdout_nl( h1, BCORE_STDOUT );
    bmath_hf3_s_cpy( h1, h2 );

    bmath_hf3_s_mul_scl_f3_add( h1, 2, h2, h2 );

    ASSERT( f3_abs( ( bmath_hf3_s_f3_sum( h2 ) / bmath_hf3_s_f3_sum( h1 ) ) - 3.0 ) < 1E-10 );

    BCORE_LIFE_DOWN();
}

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
            selftest();
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

