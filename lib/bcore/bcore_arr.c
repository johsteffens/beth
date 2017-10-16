/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_arr.h"
#include "bcore_quicktypes.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_ref.h"
#include "bcore_spect_compare.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_SPECT( bcore_arr_sz_s )
DEFINE_FUNCTION_DOWN_SPECT( bcore_arr_sz_s )
DEFINE_FUNCTION_COPY_SPECT( bcore_arr_sz_s )
DEFINE_FUNCTION_CREATE(     bcore_arr_sz_s )
DEFINE_FUNCTION_DISCARD(    bcore_arr_sz_s )
DEFINE_FUNCTION_CLONE(      bcore_arr_sz_s )

static bcore_flect_self_s* arr_sz_s_create_self( void )
{
    return bcore_flect_self_s_build_parse_sc( "bcore_arr_sz_s = { aware_t _; sz_t [] arr; }", sizeof( bcore_arr_sz_s ) );
}

void bcore_arr_sz_s_clear( bcore_arr_sz_s* o )
{
    o->size = 0;
    if( o->space == 0 ) o->data = NULL; // in case array is referencing external data
}

void bcore_arr_sz_s_set_space( bcore_arr_sz_s* o, sz_t space )
{
    if( o->space == 0 )
    {
        o->data = bcore_u_alloc( sizeof( sz_t ), NULL, space, &o->space );
    }
    else
    {
        o->data = bcore_un_alloc( sizeof( sz_t ), o->data, o->space, space, &o->space );
    }
    if( o->size > space ) o->size = space;
}

void bcore_arr_sz_s_fill( bcore_arr_sz_s* o, sz_t size, sz_t v )
{
    bcore_arr_sz_s_set_space( o, size );
    for( sz_t i = 0; i < size; i++ ) o->data[ i ] = v;
    o->size = size;
}

void bcore_arr_sz_s_step_fill( bcore_arr_sz_s* o, sz_t v_start, s3_t step, sz_t size )
{
    bcore_arr_sz_s_set_space( o, size );
    sz_t v = v_start;
    for( sz_t i = 0; i < size; i++ )
    {
        o->data[ i ] = v;
        v += step;
    }
    o->size = size;
}

void bcore_arr_sz_s_make_strong( bcore_arr_sz_s* o )
{
    if( o->size > o->space )
    {
        sz_t* data = o->data;
        o->data = bcore_u_alloc( sizeof( sz_t ), NULL, o->size, &o->space );
        bcore_u_memcpy( sizeof( sz_t ), o->data, data, o->size );
    }
}

void bcore_arr_sz_s_push( bcore_arr_sz_s* o, sz_t v )
{
    if( o->size >  o->space ) bcore_arr_sz_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( sz_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = v;
}

sz_t bcore_arr_sz_s_pop( bcore_arr_sz_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    return o->data[ o->size ];
}

static void sz_sort( sz_t* data, sz_t size, sz_t* buf, s2_t order )
{
    if( size < 2 ) return;
    sz_t size1 = size >> 1;
    sz_sort( data,         size1       , buf, order );
    sz_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( sz_t ), buf, data, size1 );
    if( order > 0 )
    {
        for( sz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] <= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
    else
    {
        for( sz_t i = 0, w = 0, r = size1; i < size1; )
        {
            data[ w++ ] = ( r == size || buf[ i ] >= data[ r ] ) ? buf[ i++ ] : data[ r++ ];
        }
    }
}

void bcore_arr_sz_s_sort( bcore_arr_sz_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_sz_s_make_strong( o );
    size_t* buf = bcore_u_alloc( sizeof( size_t ), NULL, o->size >> 1, NULL );
    sz_sort( o->data, o->size, buf, order );
    bcore_free( buf );
}

void bcore_arr_sz_s_reorder( bcore_arr_sz_s* o, const bcore_arr_sz_s* order )
{
    if( o->space < o->size ) bcore_arr_sz_s_make_strong( o );
    sz_t buf_space = 0;
    sz_t* buf = bcore_u_alloc( sizeof( sz_t ), NULL, order->size, &buf_space );
    for( sz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = o->data[ order->data[ i ] ];
    }
    bcore_un_alloc( sizeof( sz_t ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
}

bcore_arr_sz_s* bcore_arr_sz_s_create_random_permutation( u2_t ( *rg )( u2_t ), u2_t rval, sz_t size )
{
    bcore_arr_sz_s* o = bcore_arr_sz_s_create();
    if( size == 0 ) return o;

    bcore_arr_sz_s_step_fill( o, 0, 1, size );
    sz_t idx = 0;

    for( sz_t i = 0; i < size; i++ )
    {
        idx = i + ( ( idx + ( rval = rg( rval ) ) ) % ( size - i ) );
        sz_t t = o->data[ i ];
        o->data[ i ] = o->data[ idx ];
        o->data[ idx ] = t;
    }

    return o;
}

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_SPECT( bcore_arr_st_s )
DEFINE_FUNCTION_DOWN_SPECT( bcore_arr_st_s )
DEFINE_FUNCTION_COPY_SPECT( bcore_arr_st_s )
DEFINE_FUNCTION_CREATE(     bcore_arr_st_s )
DEFINE_FUNCTION_DISCARD(    bcore_arr_st_s )
DEFINE_FUNCTION_CLONE(      bcore_arr_st_s )

static bcore_flect_self_s* arr_st_s_create_self( void )
{
    return bcore_flect_self_s_build_parse_sc( "bcore_arr_st_s = { aware_t _; st_s* [] arr; }", sizeof( bcore_arr_st_s ) );
}

void bcore_arr_st_s_clear( bcore_arr_st_s* o )
{
    if( o->space == 0 )
    {
        o->data = NULL; // in case array is referencing external data
        o->size = 0;
    }

    for( sz_t i = 0; i < o->size; i++ ) st_s_discard( o->data[ i ] );
    o->size = 0;
}

void bcore_arr_st_s_set_space( bcore_arr_st_s* o, sz_t space )
{
    bcore_array_aware_set_space( o, space );
}

void bcore_arr_st_s_make_strong( bcore_arr_st_s* o )
{
    if( o->size > o->space )
    {
        st_s** data = o->data;
        o->data = bcore_u_alloc( sizeof( st_s* ), NULL, o->size, &o->space );
        for( sz_t i = 0; i < o->size; i++ )
        {
            st_s_copy( o->data[ i ], data[ i ] );
        }
    }
}

void bcore_arr_st_s_push_st( bcore_arr_st_s* o, const st_s* st )
{
    if( o->size >  o->space ) bcore_arr_st_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( st_s* ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    if( st )
    {
        st_s_copy( o->data[ o->size++ ], st );
    }
    else
    {
        o->data[ o->size++ ] = NULL;
    }
}

void bcore_arr_st_s_push_st_d( bcore_arr_st_s* o, st_s* st )
{
    if( o->size >  o->space ) bcore_arr_st_s_make_strong( o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( st_s* ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size++ ] = st;
}

void bcore_arr_st_s_push_sc( bcore_arr_st_s* o, sc_t sc )
{
    bcore_arr_st_s_push_st_d( o, st_s_create_sc( sc ) );
}

void bcore_arr_st_s_pop( bcore_arr_st_s* o )
{
    if( o->size == 0 ) return;
    o->size--;
    st_s_discard( o->data[ o->size ] );
}

static void st_sort( st_s** data, sz_t size, st_s** buf, s2_t order )
{
    if( size < 2 ) return;
    sz_t size1 = size >> 1;
    st_sort( data,         size1       , buf, order );
    st_sort( data + size1, size - size1, buf, order );
    bcore_u_memcpy( sizeof( st_s* ), buf, data, size1 );
    for( sz_t i = 0, w = 0, r = size1; i < size1; )
    {
        data[ w++ ] = ( r == size || bcore_strcmp( buf[ i ]->sc, data[ r ]->sc ) * order >= 0 ) ? buf[ i++ ] : data[ r++ ];
    }
}

void bcore_arr_st_s_sort( bcore_arr_st_s* o, s2_t order ) // stable mergesort
{
    if( o->space < o->size ) bcore_arr_st_s_make_strong( o );
    st_s** buf = bcore_u_alloc( sizeof( st_s* ), NULL, o->size >> 1, NULL );
    st_sort( o->data, o->size, buf, order );
    bcore_free( buf );
}

void bcore_arr_st_s_reorder( bcore_arr_st_s* o, const bcore_arr_sz_s* order )
{
    if( o->space < o->size ) bcore_arr_st_s_make_strong( o );
    sz_t buf_space = 0;
    st_s** buf = bcore_u_alloc( sizeof( st_s* ), NULL, order->size, &buf_space );
    for( sz_t i = 0; i < order->size; i++ )
    {
        assert( order->data[ i ] < o->size );
        buf[ i ] = bcore_fork( o->data[ order->data[ i ] ] );
    }
    for( sz_t i = 0; i < o->size; i++ ) st_s_discard( o->data[ i ] );
    bcore_un_alloc( sizeof( st_s* ), o->data, o->space, 0, &o->space );
    o->data = buf;
    o->space = buf_space;
    o->size = order->size;
}

/**********************************************************************************************************************/

#include <time.h>

bcore_string_s* bcore_arr_sz_selftest( void )
{
    bcore_string_s* log = bcore_string_s_createf( "== bcore_arr_sz_selftest " );
    bcore_string_s_push_char_n( log, '=', 120 - log->size );
    bcore_string_s_push_char( log, '\n' );

    bcore_arr_sz_s* sz_arr1 = bcore_arr_sz_s_create();
    bcore_arr_sz_s* sz_arr2 = bcore_arr_sz_s_create();
    sz_t size = 100000;
    {
        u2_t rval = 1;
        clock_t time = clock();
        for( sz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_arr_sz_s_push( sz_arr1, rval );
        }
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_arr_sz_s_push ........ %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    const bcore_array_s* array_p = bcore_array_s_get_typed( TYPEOF_bcore_arr_sz_s );
    const bcore_inst_s* u2_t_p = bcore_inst_s_get_typed( TYPEOF_u2_t );

    {
        u2_t rval = 1;
        clock_t time = clock();
        for( sz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_array_spect_push( array_p, sz_arr2, sr_pwc( u2_t_p, &rval ) );
        }
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_array_spect_push ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    ASSERT( bcore_compare_aware( sz_arr1, sz_arr2 ) == 0 );

    {
        clock_t time = clock();
        bcore_arr_sz_s_sort( sz_arr1, 1 );
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_arr_sz_s_sort ........ %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_aware_sort( sz_arr2, 0, -1, 1 );
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_array_aware_sort ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    bcore_arr_sz_s* permutation = bcore_arr_sz_s_create_random_permutation( bcore_xsg2_u2, 1, size );

    {
        clock_t time = clock();
        bcore_arr_sz_s_reorder( sz_arr1, permutation );
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_arr_sz_s_reorder ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_aware_reorder( sz_arr2, permutation );
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_array_aware_reorder .. %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    ASSERT( bcore_compare_aware( sz_arr1, sz_arr2 ) == 0 );

//    bcore_txt_ml_to_stdout( sr_awc( sz_arr1 ) );

    bcore_arr_sz_s_discard( permutation );
    bcore_arr_sz_s_discard( sz_arr1 );
    bcore_arr_sz_s_discard( sz_arr2 );

    bcore_string_s_push_char_n( log, '=', 120 );
    bcore_string_s_push_char( log, '\n' );
    return log;

}

bcore_string_s* bcore_arr_st_selftest( void )
{
    bcore_string_s* log = bcore_string_s_createf( "== bcore_arr_st_selftest " );
    bcore_string_s_push_char_n( log, '=', 120 - log->size );
    bcore_string_s_push_char( log, '\n' );

    bcore_arr_st_s* st_arr1 = bcore_arr_st_s_create();
    bcore_arr_st_s* st_arr2 = bcore_arr_st_s_create();
    sz_t size = 100000;
    {
        u2_t rval = 1;
        clock_t time = clock();
        for( sz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_arr_st_s_push_st_d( st_arr1, st_s_createf( "%u", rval ) );
        }
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_arr_st_s_push_st_d ... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        u2_t rval = 1;
        clock_t time = clock();
        for( sz_t i = 0; i < size; i++ )
        {
            rval = bcore_xsg_u2( rval );
            bcore_array_aware_push( st_arr2, sr_asd( st_s_createf( "%u", rval ) ) );
        }
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_array_aware_push ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_arr_st_s_sort( st_arr1, 1 );
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_arr_st_s_sort ........ %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_aware_sort( st_arr2, 0, -1, 1 );
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_array_aware_sort ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    bcore_arr_sz_s* permutation = bcore_arr_sz_s_create_random_permutation( bcore_xsg2_u2, 1, size );

    {
        clock_t time = clock();
        bcore_arr_st_s_reorder( st_arr1, permutation );
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_arr_st_s_reorder ..... %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    {
        clock_t time = clock();
        bcore_array_aware_reorder( st_arr2, permutation );
        time = clock() - time;
        bcore_string_s_pushf( log, "bcore_array_aware_reorder .. %zuns\n", ( sz_t ) ( ( 1E9 * ( f3_t )time/CLOCKS_PER_SEC ) / size ) );
    }

    ASSERT( bcore_compare_aware( st_arr1, st_arr2 ) == 0 );

//    bcore_txt_ml_to_stdout( sr_awc( st_arr1 ) );

    bcore_arr_sz_s_discard( permutation );
    bcore_arr_st_s_discard( st_arr1 );
    bcore_arr_st_s_discard( st_arr2 );

    bcore_string_s_push_char_n( log, '=', 120 );
    bcore_string_s_push_char( log, '\n' );
    return log;

}

/**********************************************************************************************************************/

vd_t bcore_arr_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_arr" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bcore_arr_sz_s" ), arr_sz_s_create_self );
        bcore_flect_define_creator( typeof( "bcore_arr_st_s" ), arr_st_s_create_self );
    }
    else if( signal == typeof( "selftest" ) )
    {
        bcore_string_s_print_d( bcore_arr_sz_selftest() );
        bcore_string_s_print_d( bcore_arr_st_selftest() );
    }

    return NULL;
}

