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
    o->v_space = 0;
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

void bmath_hf3_s_clear( bmath_hf3_s* o )
{
    bmath_hf3_s_clear_d_data( o );
    bmath_hf3_s_clear_v_data( o );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_fit_v_size( bmath_hf3_s* o )
{
    sz_t size = 1;
    for( sz_t i = 0; i < o->d_size; i++ ) size *= o->d_data[ i ];
    bmath_hf3_s_set_v_size( o, size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_vacant( bmath_hf3_s* o )
{
    bmath_hf3_s_clear_v_data( o );
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

void bmath_hf3_s_copy_v_data_from_vf3( bmath_hf3_s* o, const bmath_vf3_s* src )
{
    bmath_hf3_s_set_v_data( o, src->data, src->size );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_copy_v_data_to_vf3( const bmath_hf3_s* o, bmath_vf3_s* dst )
{
    bmath_vf3_s_set_data( dst, o->v_data, o->v_size );
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
    bmath_hf3_s_set_d_data( o, src->d_data, src->d_size );
    if( o->v_data )
    {
        bmath_hf3_s_fit_v_size( o );
    }
    else
    {
        bmath_hf3_s_clear_v_data( o );
    }
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

void bmath_hf3_s_set_d_scalar( bmath_hf3_s* o )
{
    bmath_hf3_s_set_d_data( o, NULL, 0 );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_scalar_pf3( bmath_hf3_s* o, f3_t* v )
{
    bmath_hf3_s_set_d_scalar( o );
    if( v )
    {
        bmath_hf3_s_fit_v_size( o );
        o->v_data[ 0 ] = *v;
    }
    else
    {
        bmath_hf3_s_clear_v_data( o );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_set_scalar_f3( bmath_hf3_s* o, f3_t v )
{
    bmath_hf3_s_set_scalar_pf3( o, &v );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// status

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

bl_t bmath_hf3_s_is_equal( const bmath_hf3_s* o, const bmath_hf3_s* op )
{
    if( !bmath_hf3_s_d_equal( o, op ) ) return false;
    if( !bmath_hf3_s_v_equal( o, op ) ) return false;
    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bmath_hf3_s_is_scalar( const bmath_hf3_s* o )
{
    return o->d_size == 0;
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

    ASSERT( src->d_size <= o->d_size );

    for( sz_t i = 0; i < src->d_size; i++ ) ASSERT( o->d_data[ i ] == src->d_data[ i ] );
    if( src->d_size == o->d_size ) bmath_hf3_s_inc_order( o, 1 );

    ASSERT( o->d_size > 0 );

    bmath_hf3_s_v_make_strong( o );
    sz_t sub_size = bmath_hf3_s_d_product( src );

    o->d_data[ o->d_size -1 ]++;

    if( o->v_data )
    {
        ASSERT( src->v_data );
        bmath_hf3_s_resize_v_size( o, o->v_size + sub_size );
        bcore_u_memcpy( sizeof( f3_t ), o->v_data + o->v_size - sub_size, src->v_data, sub_size );
    }
    else
    {
        ASSERT( !src->v_data );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static void rec_to_sink( const bmath_hf3_s* o, sz_t indent, bl_t formatted, bcore_sink* sink, f3_t* v_data, sz_t d_idx )
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
            if( formatted && d_idx > 0 ) bcore_sink_a_push_fa( sink, "\n#rn{ }", indent );
            bcore_sink_a_push_fa( sink, "(" );
            rec_to_sink( o, indent + 2, formatted, sink, v_data + i * v_block, d_idx - 1 );
            if( formatted && d_idx > 1 ) bcore_sink_a_push_fa( sink, "\n#rn{ }", indent );
            bcore_sink_a_push_fa( sink, ")" );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static void hf3_s_to_sink( const bmath_hf3_s* o, sz_t max_size, bcore_sink* sink )
{
    if( o->v_size > 0 && ( max_size < 0 || o->v_size <= max_size ) )
    {
        if( o->d_size > 0 )
        {
            bcore_sink_a_push_fa( sink, "(" );
            rec_to_sink( o, 0, false, sink, o->v_data, o->d_size - 1 );
            bcore_sink_a_push_fa( sink, ")" );
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
        if( o->v_size == 0 )
        {
            bcore_sink_a_push_fa( sink, "##" );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "D" );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_to_sink( const bmath_hf3_s* o, bcore_sink* sink )
{
    hf3_s_to_sink( o, -1, sink );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_brief_to_sink( const bmath_hf3_s* o, bcore_sink* sink )
{
    hf3_s_to_sink( o, 16, sink );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_formatted_to_sink( const bmath_hf3_s* o, bcore_sink* sink )
{
    if( o->v_data )
    {
        if( o->d_size > 0 )
        {
            bcore_sink_a_push_fa( sink, "(" );
            rec_to_sink( o, 2, true, sink, o->v_data, o->d_size - 1 );
            if( o->d_size > 1 ) bcore_sink_a_push_fa( sink, "\n" );
            bcore_sink_a_push_fa( sink, ")\n" );
        }
        else
        {
            bcore_sink_a_push_fa( sink, "#<f3_t>\n", o->v_data[ 0 ] );
        }
    }
    else
    {
        for( sz_t i = o->d_size - 1; i >= 0; i-- )
        {
            bcore_sink_a_push_fa( sink, "[#<sz_t>]", o->d_data[ i ] );
        }
        bcore_sink_a_push_fa( sink, "##\n" );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_to_sink_nl( const bmath_hf3_s* o, bcore_sink* sink )
{
    bmath_hf3_s_to_sink( o, sink );
    bcore_sink_a_push_fa( sink, "\n" );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_to_stdout( const bmath_hf3_s* o )
{
    bmath_hf3_s_to_sink( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_brief_to_stdout( const bmath_hf3_s* o )
{
    bmath_hf3_s_brief_to_sink( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_to_stdout_nl( const bmath_hf3_s* o )
{
    bmath_hf3_s_to_sink_nl( o, BCORE_STDOUT );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_formatted_to_stdout( const bmath_hf3_s* o )
{
    bmath_hf3_s_formatted_to_sink( o, BCORE_STDOUT );
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

void bmath_hf3_s_exp( const bmath_hf3_s* o, bmath_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = exp( o->v_data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_tanh( const bmath_hf3_s* o, bmath_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = tanh( o->v_data[ i ] );
}

static f3_t relu_f3( f3_t x ) { return x > 0 ? x : 0; }
void bmath_hf3_s_relu( const bmath_hf3_s* o, bmath_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = relu_f3( o->v_data[ i ] );
}

// ---------------------------------------------------------------------------------------------------------------------

static f3_t lrelu_f3( f3_t x ) { return x > 0 ? x : x * 0.01; }
void bmath_hf3_s_lrelu( const bmath_hf3_s* o, bmath_hf3_s* r )
{
    ASSERT( o->v_size == r->v_size );
    for( sz_t i = 0; i < o->v_size; i++ ) r->v_data[ i ] = lrelu_f3( o->v_data[ i ] );
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

void bmath_hf3_s_bmul( const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( b->d_size <= 2 );
    assert( o->v_size );
    assert( b->v_size );
    assert( r->v_size );

    switch( o->d_size * 3 + b->d_size )
    {
        case 0: // 0H * 0H
        {
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = o->v_data[ 0 ] * b->v_data[ 0 ];
        }
        break;

        case 1: // 0H * 1H
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( b, o->v_data, r );
        }
        break;

        case 2: // 0H * 2H
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( b->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl( b, o->v_data, r );
        }
        break;

        case 3: // 1H * 0H
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( o, b->v_data, r );
        }
        break;

        case 4: // 1H * 1H
        {
            ERR_fa( "'1H * 1H' not defined." );
        }
        break;

        case 5: // 1H * 2H
        {
            ERR_fa( "'1H * 2H' not defined." );
        }
        break;

        case 6: // 2H * 0H
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( o->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl( o, b->v_data, r );
        }
        break;

        case 7: // 2H * 1H  (matrix * vector)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_vf3_s vb = bmath_hf3_s_get_weak_vec( b );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_vf3_s* pb = ( b == r ) ? &vr : &vb;
            bmath_mf3_s_mul_vec( &mo, pb, &vr );
        }
        break;

        case 8: // 2H * 2H  (matrix * matrix)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s* po = ( o == b ) ? ( ( o == r ) ? &mr : &mb ) : ( ( o == r ) ? &mr : &mo );
            bmath_mf3_s* pb = ( b == r ) ? &mr : &mb;
            bmath_mf3_s_mul( po, pb, &mr );
        }
        break;

        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_bmul_htp( const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( b->d_size <= 2 );
    assert( o->v_size );
    assert( b->v_size );
    assert( r->v_size );

    switch( o->d_size * 3 + b->d_size )
    {
        case 0: // 0H * 0Ht
        {
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = o->v_data[ 0 ] * b->v_data[ 0 ];
        }
        break;

        case 1: // 0H * 1Ht
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( b, o->v_data, r );
        }
        break;

        case 2: // 0H * 2Ht
        {
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_htp( &mb, &mr );
            bmath_mf3_s_mul_scl( &mr, o->v_data, &mr );
        }
        break;

        case 3: // 1H * 0Ht
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( o, b->v_data, r );
        }
        break;

        case 4: // 1H * 1Ht  (outer product)
        {
            bmath_vf3_s vo = bmath_hf3_s_get_weak_vec( o );
            bmath_vf3_s vb = bmath_hf3_s_get_weak_vec( b );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_opd( &mr, &vo, &vb );
        }
        break;

        case 5: // 1H * 2Ht
        {
            ERR_fa( "'1H * 2Ht' not defined." );
        }
        break;

        case 6: // 2H * 0Ht
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( o->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl( o, b->v_data, r );
        }
        break;

        case 7: // 2H * 1Ht
        {
            ERR_fa( "'2H * 1Ht' not defined." );
        }
        break;

        case 8: // 2H * 2Ht
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s* po = ( o == b ) ? ( ( o == r ) ? &mr : &mb ) : ( ( o == r ) ? &mr : &mo );
            bmath_mf3_s* pb = ( b == r ) ? &mr : &mb;
            bmath_mf3_s_mul_htp( po, pb, &mr );
        }
        break;

        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_htp_bmul( const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( b->d_size <= 2 );
    assert( o->v_size );
    assert( b->v_size );
    assert( r->v_size );

    switch( o->d_size * 3 + b->d_size )
    {
        case 0: // 0Ht * 0H
        {
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = o->v_data[ 0 ] * b->v_data[ 0 ];
        }
        break;

        case 1: // 0Ht * 1H
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( b, o->v_data, r );
        }
        break;

        case 2: // 0Ht * 2H
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( b->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl( b, o->v_data, r );
        }
        break;

        case 3: // 1Ht * 0H
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( o, b->v_data, r );
        }
        break;

        case 4: // 1Ht * 1H  (VT V) dot product
        {
            ASSERT( o->d_data[ 0 ] == b->d_data[ 0 ] );
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = bmath_f3_t_vec_mul_vec( o->v_data, b->v_data, o->v_size );
        }
        break;

        case 5: // 1Ht * 2H
        {
            bmath_vf3_s vo = bmath_hf3_s_get_weak_vec( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_vf3_s* po = ( o == r ) ? &vr : &vo;
            bmath_mf3_s_htp_mul_vec( &mb, po, &vr );
        }
        break;

        case 6: // 2Ht * 0H
        {
            bmath_mf3_s om = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_htp( &om, &mr );
            bmath_mf3_s_mul_scl( &mr, b->v_data, &mr );
        }
        break;

        case 7: // 2Ht * 1H  (matrix * vector)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_vf3_s vb = bmath_hf3_s_get_weak_vec( b );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_vf3_s* pb = ( b == r ) ? &vr : &vb;
            bmath_mf3_s_htp_mul_vec( &mo, pb, &vr );
        }
        break;

        case 8: // 2Ht * 2H  (matrix * matrix)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s* po = ( o == b ) ? ( ( o == r ) ? &mr : &mb ) : ( ( o == r ) ? &mr : &mo );
            bmath_mf3_s* pb = ( b == r ) ? &mr : &mb;
            bmath_mf3_s_htp_mul( po, pb, &mr );
        }
        break;

        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_htp_bmul_htp( const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( b->d_size <= 2 );
    assert( o->v_size );
    assert( b->v_size );
    assert( r->v_size );

    switch( o->d_size * 3 + b->d_size )
    {
        case 0: // 0Ht * 0Ht
        {
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = o->v_data[ 0 ] * b->v_data[ 0 ];
        }
        break;

        case 1: // 0Ht * 1Ht
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( b, o->v_data, r );
        }
        break;

        case 2: // 0Ht * 2Ht
        {
            bmath_mf3_s bm = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_htp( &bm, &mr );
            bmath_mf3_s_mul_scl( &mr, o->v_data, &mr );
        }
        break;

        case 3: // 1Ht * 0Ht
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl( o, b->v_data, r );
        }
        break;

        case 4: // 1Ht * 1Ht  (VT VT) not defined
        {
            ERR_fa( "'1Ht * 1Ht' not defined." );
        }
        break;

        case 5: // 1Ht * 2Ht
        {
            bmath_vf3_s vo = bmath_hf3_s_get_weak_vec( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_vf3_s* po = ( o == r ) ? &vr : &vo;
            bmath_mf3_s_mul_vec( &mb, po, &vr );
        }
        break;

        case 6: // 2Ht * 0Ht
        {
            bmath_mf3_s om = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_htp( &om, &mr );
            bmath_mf3_s_mul_scl( &mr, b->v_data, &mr );
        }
        break;

        case 7: // 2Ht * 1Ht  (matrix * vector)
        {
            ERR_fa( "'2Ht * 1Ht' not defined." );
        }
        break;

        case 8: // 2Ht * 2Ht  (matrix * matrix)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s* po = ( o == b ) ? ( ( o == r ) ? &mr : &mb ) : ( ( o == r ) ? &mr : &mo );
            bmath_mf3_s* pb = ( b == r ) ? &mr : &mb;
            bmath_mf3_s_htp_mul_htp( po, pb, &mr );
        }
        break;

        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_bmul_add( const bmath_hf3_s* o, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( b->d_size <= 2 );
    ASSERT( bmath_hf3_s_d_equal( c, r ) );
    assert( o->v_size );
    assert( b->v_size );
    assert( r->v_size );

    switch( o->d_size * 3 + b->d_size )
    {
        case 0: // 0H * 0H
        {
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = o->v_data[ 0 ] * b->v_data[ 0 ] + c->v_data[ 0 ];
        }
        break;

        case 1: // 0H * 1H
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl_add( b, o->v_data, c, r );
        }
        break;

        case 2: // 0H * 2H
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( b->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl_add( b, o->v_data, c, r );
        }
        break;

        case 3: // 1H * 0H
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl_add( o, b->v_data, c, r );
        }
        break;

        case 4: // 1H * 1H
        {
            ERR_fa( "bmul(1H,1H) not defined." );
        }
        break;

        case 5: // 1H * 2H
        {
            ERR_fa( "bmul(1H,2H) not defined." );
        }
        break;

        case 6: // 2H * 0H
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( o->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl_add( o, b->v_data, c, r );
        }
        break;

        case 7: // 2H * 1H  (matrix * vector)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_vf3_s vb = bmath_hf3_s_get_weak_vec( b );
            bmath_vf3_s vc = bmath_hf3_s_get_weak_vec( c );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_vf3_s* pb = ( b == r ) ? &vr : &vb;
            bmath_vf3_s* pc = ( c == r ) ? &vr : &vc;
            bmath_mf3_s_mul_vec_add( &mo, pb, pc, &vr );
        }
        break;

        case 8: // 2H * 2H  (matrix * matrix)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s* po = ( o == b ) ? ( ( o == r ) ? &mr : &mb ) : ( ( o == r ) ? &mr : &mo );
            bmath_mf3_s* pb = ( b == r ) ? &mr : &mb;
            bmath_mf3_s* pc = ( c == r ) ? &mr : &mc;
            bmath_mf3_s_mul_add( po, pb, pc, &mr );
        }
        break;

        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_bmul_htp_add( const bmath_hf3_s* o, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( b->d_size <= 2 );
    ASSERT( bmath_hf3_s_d_equal( c, r ) );
    assert( o->v_size );
    assert( b->v_size );
    assert( r->v_size );

    switch( o->d_size * 3 + b->d_size )
    {
        case 0: // 0H * 0Ht
        {
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = o->v_data[ 0 ] * b->v_data[ 0 ] + c->v_data[ 0 ];
        }
        break;

        case 1: // 0H * 1Ht
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl_add( b, o->v_data, c, r );
        }
        break;

        case 2: // 0H * 2Ht
        {
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c );
            bmath_mf3_s_htp( &mb, &mr );
            bmath_mf3_s_mul_scl_add( &mr, o->v_data, &mc, &mr );
        }
        break;

        case 3: // 1H * 0Ht
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl_add( o, b->v_data, c, r );
        }
        break;

        case 4: // 1H * 1Ht  (outer product)
        {
            bmath_vf3_s vo = bmath_hf3_s_get_weak_vec( o );
            bmath_vf3_s vb = bmath_hf3_s_get_weak_vec( b );
            bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s* pc = ( c == r ) ? &mr : &mc;

            // TODO: replace with bmath_mf3_s_opd_add
            bmath_mf3_s_opd( &mr, &vo, &vb );
            bmath_mf3_s_add( &mr, pc, &mr );
        }
        break;

        case 5: // 1H, 2Ht
        {
            ERR_fa( "bmul_htp(1H,2H) not defined." );
        }
        break;

        case 6: // 2H * 0Ht
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( o->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl_add( o, b->v_data, c, r );
        }
        break;

        case 7: // 2H * 1Ht
        {
            ERR_fa( "bmul_htp(2H,1H) not defined." );
        }
        break;

        case 8: // 2H * 2Ht
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s* po = ( o == b ) ? ( ( o == r ) ? &mr : &mb ) : ( ( o == r ) ? &mr : &mo );
            bmath_mf3_s* pb = ( b == r ) ? &mr : &mb;
            bmath_mf3_s* pc = ( c == r ) ? &mr : &mc;
            bmath_mf3_s_mul_htp_add( po, pb, pc, &mr );
        }
        break;

        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_htp_bmul_add( const bmath_hf3_s* o, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( b->d_size <= 2 );
    ASSERT( bmath_hf3_s_d_equal( c, r ) );
    assert( o->v_size );
    assert( b->v_size );
    assert( r->v_size );

    switch( o->d_size * 3 + b->d_size )
    {
        case 0: // 0Ht * 0H
        {
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = o->v_data[ 0 ] * b->v_data[ 0 ] + c->v_data[ 0 ];
        }
        break;

        case 1: // 0Ht * 1H
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl_add( b, o->v_data, c, r );
        }
        break;

        case 2: // 0Ht * 2H
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            ASSERT( b->d_data[ 1 ] == r->d_data[ 1 ] );
            bmath_hf3_s_mul_scl_add( b, o->v_data, c, r );
        }
        break;

        case 3: // 1Ht * 0H
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl_add( o, b->v_data, c, r );
        }
        break;

        case 4: // 1Ht * 1H  (VT V) dot product
        {
            ASSERT( o->d_data[ 0 ] == b->d_data[ 0 ] );
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = bmath_f3_t_vec_mul_vec( o->v_data, b->v_data, o->v_size ) + c->v_data[ 0 ];
        }
        break;

        case 5: // 1Ht * 2H
        {
            bmath_vf3_s vo = bmath_hf3_s_get_weak_vec( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_vf3_s vc = bmath_hf3_s_get_weak_vec( c );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_vf3_s* po = ( o == r ) ? &vr : &vo;
            bmath_vf3_s* pc = ( c == r ) ? &vr : &vc;
            bmath_mf3_s_htp_mul_vec_add( &mb, po, pc, &vr );
        }
        break;

        case 6: // 2Ht * 0H
        {
            bmath_mf3_s om = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s_htp( &om, &mr );

            bmath_mf3_s* pc = ( c == r ) ? &mr : &mc;
            bmath_mf3_s_mul_scl_add( &mr, b->v_data, pc, &mr );
        }
        break;

        case 7: // 2Ht * 1H  (matrix * vector)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_vf3_s vb = bmath_hf3_s_get_weak_vec( b );
            bmath_vf3_s vc = bmath_hf3_s_get_weak_vec( c );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_vf3_s* pb = ( b == r ) ? &vr : &vb;
            bmath_vf3_s* pc = ( c == r ) ? &vr : &vc;
            bmath_mf3_s_htp_mul_vec_add( &mo, pb, pc, &vr );
        }
        break;

        case 8: // 2Ht * 2H  (matrix * matrix)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s* po = ( o == b ) ? ( ( o == r ) ? &mr : &mb ) : ( ( o == r ) ? &mr : &mo );
            bmath_mf3_s* pb = ( b == r ) ? &mr : &mb;
            bmath_mf3_s* pc = ( c == r ) ? &mr : &mc;
            bmath_mf3_s_htp_mul_add( po, pb, pc, &mr );
        }
        break;

        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_htp_bmul_htp_add( const bmath_hf3_s* o, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r )
{
    ASSERT( o->d_size <= 2 );
    ASSERT( b->d_size <= 2 );
    ASSERT( bmath_hf3_s_d_equal( c, r ) );
    assert( o->v_size );
    assert( b->v_size );
    assert( r->v_size );

    switch( o->d_size * 3 + b->d_size )
    {
        case 0: // 0Ht * 0Ht
        {
            ASSERT( r->d_size == 0 );
            r->v_data[ 0 ] = o->v_data[ 0 ] * b->v_data[ 0 ] + c->v_data[ 0 ];
        }
        break;

        case 1: // 0Ht * 1Ht
        {
            ASSERT( b->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl_add( b, o->v_data, c, r );
        }
        break;

        case 2: // 0Ht * 2Ht
        {
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c );
            bmath_mf3_s_htp( &mb, &mr );
            bmath_mf3_s_mul_scl_add( &mr, o->v_data, &mc, &mr );
        }
        break;

        case 3: // 1Ht * 0Ht
        {
            ASSERT( o->d_data[ 0 ] == r->d_data[ 0 ] );
            bmath_hf3_s_mul_scl_add( o, b->v_data, c, r );
        }
        break;

        case 4: // 1Ht * 1Ht  (VT VT) not defined
        {
            ERR_fa( "htp_bmul_htp(1H,1H) not defined." );
        }
        break;

        case 5: // 1Ht * 2Ht
        {
            bmath_vf3_s vo = bmath_hf3_s_get_weak_vec( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_vf3_s vc = bmath_hf3_s_get_weak_vec( c );
            bmath_vf3_s vr = bmath_hf3_s_get_weak_vec( r );
            bmath_vf3_s* po = ( o == r ) ? &vr : &vo;
            bmath_vf3_s* pc = ( c == r ) ? &vr : &vc;
            bmath_mf3_s_mul_vec_add( &mb, po, pc, &vr );
        }
        break;

        case 6: // 2Ht * 0Ht
        {
            bmath_mf3_s om = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s* pc = ( c == r ) ? &mr : &mc;
            bmath_mf3_s_htp( &om, &mr );
            bmath_mf3_s_mul_scl_add( &mr, b->v_data, pc, &mr );
        }
        break;

        case 7: // 2Ht * 1Ht  (matrix * vector)
        {
            ERR_fa( "htp_bmul_htp(2H,1H) not defined." );
        }
        break;

        case 8: // 2Ht * 2Ht  (matrix * matrix)
        {
            bmath_mf3_s mo = bmath_hf3_s_get_weak_mat( o );
            bmath_mf3_s mb = bmath_hf3_s_get_weak_mat( b );
            bmath_mf3_s mc = bmath_hf3_s_get_weak_mat( c );
            bmath_mf3_s mr = bmath_hf3_s_get_weak_mat( r );
            bmath_mf3_s* po = ( o == b ) ? ( ( o == r ) ? &mr : &mb ) : ( ( o == r ) ? &mr : &mo );
            bmath_mf3_s* pb = ( b == r ) ? &mr : &mb;
            bmath_mf3_s* pc = ( c == r ) ? &mr : &mc;
            bmath_mf3_s_htp_mul_htp_add( po, pb, pc, &mr );
        }
        break;

        default: break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bmath_hf3_s_set_d_bmul( const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r )
{
    if( o->d_size > 2 ) return false;
    if( b->d_size > 2 ) return false;
    u2_t combination = o->d_size * 3 + b->d_size;

    switch( combination )
    {
        case 0: /* 0H*0H */ break;
        case 1: /* 0H*1H */ break;
        case 2: /* 0H*2H */ break;
        case 3: /* 1H*0H */ break;
        case 4: /* 1H*1H */ return false;
        case 5: /* 1H*2H */ return false;
        case 6: /* 2H*0H */ break;
        case 7: /* 2H*1H */ if( o->d_data[ 0 ] != b->d_data[ 0 ] ) return false; else break;
        case 8: /* 2H*2H */ if( o->d_data[ 0 ] != b->d_data[ 1 ] ) return false; else break;
        default: break;
    }

    switch( combination )
    {
        case 0: /* 0H*0H */ bmath_hf3_s_set_d_data_na( r, 0                                 ); return true;
        case 1: /* 0H*1H */ bmath_hf3_s_set_d_data_na( r, 1, b->d_data[ 0 ]                 ); return true;
        case 2: /* 0H*2H */ bmath_hf3_s_set_d_data_na( r, 2, b->d_data[ 0 ], b->d_data[ 1 ] ); return true;
        case 3: /* 1H*0H */ bmath_hf3_s_set_d_data_na( r, 1, o->d_data[ 0 ]                 ); return true;
        case 4: /* 1H*1H */ return false;
        case 5: /* 1H*2H */ return false;
        case 6: /* 2H*0H */ bmath_hf3_s_set_d_data_na( r, 2, o->d_data[ 0 ], o->d_data[ 1 ] ); return true;
        case 7: /* 2H*1H */ bmath_hf3_s_set_d_data_na( r, 1, o->d_data[ 1 ]                 ); return true;
        case 8: /* 2H*2H */ bmath_hf3_s_set_d_data_na( r, 2, b->d_data[ 0 ], o->d_data[ 1 ] ); return true;
        default: break;
    }
    return false;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bmath_hf3_s_set_d_bmul_htp( const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r )
{
    if( o->d_size > 2 ) return false;
    if( b->d_size > 2 ) return false;
    u2_t combination = o->d_size * 3 + b->d_size;

    switch( combination )
    {
        case 0: /* 0H*0Ht */ break;
        case 1: /* 0H*1Ht */ break;
        case 2: /* 0H*2Ht */ break;
        case 3: /* 1H*0Ht */ break;
        case 4: /* 1H*1Ht */ break; /* outer product */
        case 5: /* 1H*2Ht */ return false;
        case 6: /* 2H*0Ht */ break;
        case 7: /* 2H*1Ht */ return false;
        case 8: /* 2H*2Ht */ if( o->d_data[ 0 ] != b->d_data[ 0 ] ) return false; else break;
        default: break;
    }
    switch( combination )
    {
        case 0: /* 0H*0Ht */ bmath_hf3_s_set_d_data_na( r, 0                                 ); return true;
        case 1: /* 0H*1Ht */ bmath_hf3_s_set_d_data_na( r, 1, b->d_data[ 0 ]                 ); return true;
        case 2: /* 0H*2Ht */ bmath_hf3_s_set_d_data_na( r, 2, b->d_data[ 1 ], b->d_data[ 0 ] ); return true;
        case 3: /* 1H*0Ht */ bmath_hf3_s_set_d_data_na( r, 1, o->d_data[ 0 ]                 ); return true;
        case 4: /* 1H*1Ht */ bmath_hf3_s_set_d_data_na( r, 2, b->d_data[ 0 ], o->d_data[ 0 ] ); return true; /* outer product */
        case 5: /* 1H*2Ht */ return false;
        case 6: /* 2H*0Ht */ bmath_hf3_s_set_d_data_na( r, 2, o->d_data[ 0 ], o->d_data[ 1 ] ); return true;
        case 7: /* 2H*1Ht */ return false;
        case 8: /* 2H*2Ht */ bmath_hf3_s_set_d_data_na( r, 2, b->d_data[ 1 ], o->d_data[ 1 ] ); return true;
        default: break;
    }
    return false;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bmath_hf3_s_set_d_htp_bmul( const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r )
{
    if( o->d_size > 2 ) return false;
    if( b->d_size > 2 ) return false;
    u2_t combination = o->d_size * 3 + b->d_size;

    switch( combination )
    {
        case 0: /* 0Ht*0H */ break;
        case 1: /* 0Ht*1H */ break;
        case 2: /* 0Ht*2H */ break;
        case 3: /* 1Ht*0H */ break;
        case 4: /* 1Ht*1H */ if( o->d_data[ 0 ] != b->d_data[ 0 ] ) return false; else break; /* dot product */
        case 5: /* 1Ht*2H */ if( o->d_data[ 0 ] != b->d_data[ 1 ] ) return false; else break;
        case 6: /* 2Ht*0H */ break;
        case 7: /* 2Ht*1H */ if( o->d_data[ 1 ] != b->d_data[ 0 ] ) return false; else break;
        case 8: /* 2Ht*2H */ if( o->d_data[ 1 ] != b->d_data[ 1 ] ) return false; else break;
        default: break;
    }
    switch( combination )
    {
        case 0: /* 0Ht*0H */ bmath_hf3_s_set_d_data_na( r, 0                                 ); return true;
        case 1: /* 0Ht*1H */ bmath_hf3_s_set_d_data_na( r, 1, b->d_data[ 0 ]                 ); return true;
        case 2: /* 0Ht*2H */ bmath_hf3_s_set_d_data_na( r, 2, b->d_data[ 0 ], b->d_data[ 1 ] ); return true;
        case 3: /* 1Ht*0H */ bmath_hf3_s_set_d_data_na( r, 1, o->d_data[ 0 ]                 ); return true;
        case 4: /* 1Ht*1H */ bmath_hf3_s_set_d_data_na( r, 0                                 ); return true; /* dot product */
        case 5: /* 1Ht*2H */ bmath_hf3_s_set_d_data_na( r, 1, b->d_data[ 0 ]                 ); return true;
        case 6: /* 2Ht*0H */ bmath_hf3_s_set_d_data_na( r, 2, o->d_data[ 1 ], o->d_data[ 0 ] ); return true;
        case 7: /* 2Ht*1H */ bmath_hf3_s_set_d_data_na( r, 1, o->d_data[ 0 ]                 ); return true;
        case 8: /* 2Ht*2H */ bmath_hf3_s_set_d_data_na( r, 2, b->d_data[ 0 ], o->d_data[ 0 ] ); return true;
        default: break;
    }
    return false;
}

// ---------------------------------------------------------------------------------------------------------------------

bl_t bmath_hf3_s_set_d_htp_bmul_htp( const bmath_hf3_s* o, const bmath_hf3_s* b, bmath_hf3_s* r )
{
    if( o->d_size > 2 ) return false;
    if( b->d_size > 2 ) return false;
    u2_t combination = o->d_size * 3 + b->d_size;
    switch( combination )
    {
        case 0: /* 0Ht*0Ht */ break;
        case 1: /* 0Ht*1Ht */ break;
        case 2: /* 0Ht*2Ht */ break;
        case 3: /* 1Ht*0Ht */ break;
        case 4: /* 1Ht*1Ht */ return false;
        case 5: /* 1Ht*2Ht */ if( o->d_data[ 0 ] != b->d_data[ 0 ] ) return false; else break;
        case 6: /* 2Ht*0Ht */ break;
        case 7: /* 2Ht*1Ht */ return false;
        case 8: /* 2Ht*2Ht */ if( o->d_data[ 1 ] != b->d_data[ 0 ] ) return false; else break;
        default: break;
    }
    switch( combination )
    {
        case 0: /* 0Ht*0Ht */ bmath_hf3_s_set_d_data_na( r, 0                                 ); return true;
        case 1: /* 0Ht*1Ht */ bmath_hf3_s_set_d_data_na( r, 1, b->d_data[ 0 ]                 ); return true;
        case 2: /* 0Ht*2Ht */ bmath_hf3_s_set_d_data_na( r, 2, b->d_data[ 1 ], b->d_data[ 0 ] ); return true;
        case 3: /* 1Ht*0Ht */ bmath_hf3_s_set_d_data_na( r, 1, o->d_data[ 0 ]                 ); return true;
        case 4: /* 1Ht*1Ht */ return false;
        case 5: /* 1Ht*2Ht */ bmath_hf3_s_set_d_data_na( r, 1, b->d_data[ 1 ]                 ); return true;
        case 6: /* 2Ht*0Ht */ bmath_hf3_s_set_d_data_na( r, 2, o->d_data[ 1 ], o->d_data[ 0 ] ); return true;
        case 7: /* 2Ht*1Ht */ return false;
        case 8: /* 2Ht*2Ht */ bmath_hf3_s_set_d_data_na( r, 2, b->d_data[ 1 ], o->d_data[ 0 ] ); return true;
        default: break;
    }
    return false;
}

// ---------------------------------------------------------------------------------------------------------------------

void bmath_hf3_s_bmul_add_cps( bl_t htpa, const bmath_hf3_s* a, sz_t k, bl_t htpb, const bmath_hf3_s* b, f3_t c, const bmath_hf3_s* d, f3_t e, bmath_hf3_s* r )
{
    if( d )
    {
        ASSERT( bmath_hf3_s_d_equal( d, r ) );
        if( c != 0 )
        {
            if( c == e )
            {
                if( d != r ) bmath_hf3_s_cpy( d, r );
            }
            else
            {
                bmath_hf3_s_mul_scl_f3( d, e / c, r );
            }
        }
        else
        {
            if( e == 1.0 )
            {
                if( d != r ) bmath_hf3_s_cpy( d, r );
            }
            else
            {
                bmath_hf3_s_mul_scl_f3( d, e, r );
            }
        }
    }
    else
    {
        bmath_hf3_s_zro( r );
    }

    if( c != 0 )
    {
        switch( htpa * 2 + htpb )
        {
            case 0: bmath_hf3_s_bmul_add(         a, b, r, r ); break; // a * b
            case 1: bmath_hf3_s_bmul_htp_add(     a, b, r, r ); break; // a * b^T
            case 2: bmath_hf3_s_htp_bmul_add(     a, b, r, r ); break; // a^T * b
            case 3: bmath_hf3_s_htp_bmul_htp_add( a, b, r, r ); break; // a^T * b^T
            default: break;
        }
        if( c != 1.0 ) bmath_hf3_s_mul_scl_f3( r, c, r );
    }
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

