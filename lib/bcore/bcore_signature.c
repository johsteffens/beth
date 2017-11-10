/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_signature.h"
#include "bcore_name_manager.h"
#include "bcore_tbman.h"
#include "bcore_hmap.h"
#include "bcore_threads.h"

/**********************************************************************************************************************/

void bcore_signature_s_init( bcore_signature_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = typeof( "bcore_signature_s" );
}

void bcore_signature_s_down( bcore_signature_s* o )
{
    o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, 0, &o->space );
}

void bcore_signature_s_copy( bcore_signature_s* o, const bcore_signature_s* src )
{
    o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, 0,         &o->space );
    o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, src->size, &o->space );
    bcore_u_memcpy( sizeof( tp_t ), o->data, src->data, src->size );
    o->size = src->size;
}

DEFINE_FUNCTION_CREATE( bcore_signature_s )
DEFINE_FUNCTION_DISCARD( bcore_signature_s )
DEFINE_FUNCTION_CLONE( bcore_signature_s )

bcore_signature_s* bcore_signature_s_create_vn( sz_t n, va_list args )
{
    bcore_signature_s* o = bcore_signature_s_create();
    o->data = bcore_u_alloc( sizeof( tp_t ), o->data, n, &o->space );
    for( sz_t i = 0; i < n; i++ ) o->data[ o->size++ ] = va_arg( args, tp_t );
    return o;
}

bcore_signature_s* bcore_signature_s_create_an( sz_t n, ... )
{
    va_list args;
    va_start( args, n );
    bcore_signature_s* o = bcore_signature_s_create_vn( n, args );
    va_end( args );
    return o;
}

void bcore_signature_s_clear( bcore_signature_s* o )
{
    o->size = 0;
}

void bcore_signature_s_push( bcore_signature_s* o, tp_t type )
{
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, o->space > 0 ? o->space * 2 : 1, &o->space );
    }
    o->data[ o->size ] = type;
    o->size++;
}

void bcore_signature_s_push_nv( bcore_signature_s* o, sz_t n, va_list args )
{
    if( o->size + n >= o->space )
    {
        o->data = bcore_un_alloc( sizeof( tp_t ), o->data, o->space, o->space * 2 >= o->size + n ? o->space * 2 : o->size + n, &o->space );
    }
    for( sz_t i = 0; i < n; i++ ) o->data[ o->size++ ] = va_arg( args, tp_t );
}

void bcore_signature_s_push_na( bcore_signature_s* o, sz_t n, ... )
{
    va_list args;
    va_start( args, n );
    bcore_signature_s_push_nv( o, n, args );
    va_end( args );
}

void bcore_signature_s_pop_n( bcore_signature_s* o, sz_t n )
{
    o->size = n < o->size ? o->size - n : 0;
}

tp_t bcore_signature_s_pop( bcore_signature_s* o )
{
    if( o->size == 0 ) return 0;
    tp_t ret = o->data[ o->size ];
    o->size--;
    return ret;
}

st_s* bcore_signature_s_create_string( const bcore_signature_s* o )
{
    st_s* s = st_s_create();
    for( sz_t i = 0; i < o->size; i++ )
    {
        st_s_push_sc( s, ifnameof( o->data[ i ] ) );
        st_s_push_char( s, i == o->size - 1 ? ';' : ',' );
    }
    return s;
}

u2_t bcore_signature_s_cmp( const bcore_signature_s* sig1, const bcore_signature_s* sig2 )
{
    sz_t idx1 = 0;
    sz_t idx2 = 0;
    tp_t* dp1 = sig1->data;
    tp_t* dp2 = sig2->data;
    while( idx1 < sig1->size && idx2 < sig2->size )
    {
        if( *dp1 != *dp2 ) return ( *dp1 < *dp2 ) ? 2 : -2;
        idx1++;
        idx2++;
    }
    return ( idx1 == sig1->size ) ? ( ( idx2 == sig2->size ) ? 0 : +1 ) : -1;
}

bool bcore_signature_s_equal( const bcore_signature_s* sig1, const bcore_signature_s* sig2 )
{
    return bcore_signature_s_cmp( sig1, sig2 ) == 0;
}

tp_t bcore_signature_s_get_hash( const bcore_signature_s* o )
{
    if( o->size == 0 ) return 0;
    tp_t type = o->data[ 0 ];
    for( sz_t i = 1; i < o->size; i++ )
    {
        type = bcore_fnv_fold_u2_u2( type, o->data[ i ] );
    }
    return type;
}

tp_t bcore_signature_get_hash_nv( sz_t n, va_list args )
{
    if( n == 0 ) return 0;
    tp_t type = va_arg( args, tp_t );
    for( sz_t i = 1; i < n; i++ )
    {
        type = bcore_fnv_fold_u2_u2( type, va_arg( args, tp_t ) );
    }
    return type;
}

tp_t bcore_signature_get_hash_na( sz_t n, ... )
{
    va_list args;
    va_start( args, n );
    tp_t ret = bcore_signature_get_hash_nv( n, args );
    va_end( args );
    return ret;
}

tp_t bcore_signature_get_hash_arr( sz_t n, const tp_t* arr )
{
    if( n == 0 ) return 0;
    tp_t type = arr[ 0 ];
    for( sz_t i = 1; i < n; i++ )
    {
        type = bcore_fnv_fold_u2_u2( type, arr[ i ] );
    }
    return type;
}

tp_t bcore_signature_fold_hash_arr( tp_t hash, sz_t n, const tp_t* arr )
{
    tp_t type = hash;
    for( sz_t i = 0; i < n; i++ )
    {
        type = bcore_fnv_fold_u2_u2( type, arr[ i ] );
    }
    return type;
}

static bcore_flect_self_s* signature_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( " bcore_signature_s =  { aware_t _; tp_t[] data; }", sizeof( bcore_signature_s ) );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_signature_s_init,         "bcore_fp_init",         "init"         );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_signature_s_down,         "bcore_fp_down",         "down"         );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_signature_s_copy,         "bcore_fp_copy",         "copy"         );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_signature_s_create,       "bcore_fp_create",       "create"       );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_signature_s_clone,        "bcore_fp_clone",        "clone"        );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_signature_s_discard,      "bcore_fp_discard",      "discard"      );
    return self;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_signature_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_signature" ) ) return NULL;
    if( signal == typeof( "init0" ) )
    {
    }
    else if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bcore_signature_s"  ), signature_s_create_self  );
    }
    else if( signal == typeof( "down0" ) )
    {
    }
    return NULL;
}

/**********************************************************************************************************************/

