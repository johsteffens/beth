/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_name.h"
#include "bcore_name_manager.h"
#include "bcore_quicktypes.h"

/// hashing (non-cryptographic)
static u2_t hash_tpu2_1( tp_t key )
{
    u2_t h = ( 632432329 ^ ( ( key       ) & 0x0FFFFu ) ) * 88888888901;
         h = ( h         ^ ( ( key >> 16 ) & 0x0FFFFu ) ) * 88888888901;
    return h;
}

/// hashing (non-cryptographic)
static u2_t hash_tpu2_2( tp_t key )
{
    u2_t h = ( 368653234 ^ ( ( key       ) & 0x0FFFFu ) ) * 77777777827;
         h = ( h         ^ ( ( key >> 16 ) & 0x0FFFFu ) ) * 77777777827;
    return h;
}

/**********************************************************************************************************************/

void bcore_name_s_init( bcore_name_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void bcore_name_s_clear( bcore_name_s* o )
{
    if( o->name ) bcore_release( o->name );
    bcore_memzero( o, sizeof( *o ) );
}

void bcore_name_s_down( bcore_name_s* o )
{
    if( o->name ) bcore_release( o->name );
}

void bcore_name_s_copy( bcore_name_s* o, const bcore_name_s* src )
{
    if( o->name ) bcore_release( o->name );
    bcore_memcpy( o, src, sizeof( *o ) );
    o->name = bcore_strcpy( NULL, src->name );
}

DEFINE_FUNCTION_CREATE( bcore_name_s )
DEFINE_FUNCTION_CLONE( bcore_name_s )
DEFINE_FUNCTION_DISCARD( bcore_name_s )

/// creation using default hashing
bcore_name_s bcore_name_sc( sc_t name )
{
    bcore_name_s o;
    bcore_name_s_init( &o );
    o.name = bcore_strcpy( NULL, name );
    o.name_space = 0;
    o.key = bcore_name_key( name );
    return o;
}

bcore_name_s bcore_name_ns_sc( tp_t name_space, sc_t name )
{
    bcore_name_s o;
    bcore_name_s_init( &o );
    o.name = bcore_strcpy( NULL, name );
    o.name_space = name_space;
    o.key = bcore_name_key_ns( name_space, name );
    return o;
}

bcore_name_s bcore_name_sc_n( sc_t name, sz_t n )
{
    bcore_name_s o;
    bcore_name_s_init( &o );
    o.name = bcore_alloc( NULL, n + 1 );
    bcore_memcpy( o.name, name, n );
    o.name[ n ] = 0;
    o.name_space = 0;
    o.key = bcore_name_key_n( name, n );
    return o;
}

bcore_name_s bcore_name_ns_sc_n( tp_t name_space, sc_t name, sz_t n )
{
    bcore_name_s o;
    bcore_name_s_init( &o );
    o.name = bcore_alloc( NULL, n + 1 );
    bcore_memcpy( o.name, name, n );
    o.name[ n ] = 0;
    o.name_space = name_space;
    o.key = bcore_name_key_ns_n( name_space, name, n );
    return o;
}

/**********************************************************************************************************************/

void bcore_name_map_s_init( bcore_name_map_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_name_map_s;
    o->size_limit = 0xFFFFFFFFu;
}

void bcore_name_map_s_clear( bcore_name_map_s* o )
{
    if( o->data ) for( sz_t i = 0; i < o->size; i++ ) bcore_name_s_down( &o->data[ i ] );
    bcore_release( o->data );
    bcore_release( o->flags );
    o->data = 0;
    o->flags = 0;
    o->size = 0;
    o->depth_limit = 0;
}

void bcore_name_map_s_down( bcore_name_map_s* o )
{
    bcore_name_map_s_clear( o );
}

void bcore_name_map_s_copy( bcore_name_map_s* o, const bcore_name_map_s* src )
{
    bcore_name_map_s_clear( o );
    o->data = bcore_alloc( NULL, sizeof( bcore_name_s ) * src->size );
    for( sz_t i = 0; i < src->size; i++ )
    {
        bcore_name_s_init( &o->data[ i ] );
        bcore_name_s_copy( &o->data[ i ], &src->data[ i ] );
    }
    o->size  = src->size;
    o->depth_limit = src->depth_limit;
    o->size_limit  = src->size_limit;
}

DEFINE_FUNCTION_CREATE( bcore_name_map_s )
DEFINE_FUNCTION_CLONE( bcore_name_map_s )
DEFINE_FUNCTION_DISCARD( bcore_name_map_s )

static sz_t find( const bcore_name_map_s* o, tp_t key ) // returns valid index or o->size
{
    if( o->size == 0 ) return o->size;
    u2_t mask = o->size - 1;
    sz_t idx = hash_tpu2_1( key ) & mask;
    if( o->data[ idx ].key == key ) return idx;
    idx = hash_tpu2_2( key ) & mask;
    if( o->data[ idx ].key == key ) return idx;
    return o->size;
}

static sz_t set( bcore_name_map_s* o, bcore_name_s name, sz_t depth ) // sets new node, returns valid index on success; o->size otherwise
{
    sz_t size = o->size;
    if( size == 0 ) return size;
    u2_t mask = o->size - 1;

    if( depth == o->depth_limit ) return size;

    sz_t idx1 = hash_tpu2_1( name.key ) & mask;
    if( !o->data[ idx1 ].key )
    {
        o->data[ idx1 ] = name;
        return idx1;
    }

    sz_t idx2 = hash_tpu2_2( name.key ) & mask;
    if( !o->data[ idx2 ].key )
    {
        o->data[ idx2 ] = name;
        return idx2;
    }

    if( !o->flags ) o->flags = bcore_u_memzero( sizeof( bl_t ), NULL, size );

    if( !o->flags[ idx1 ] )
    {
        o->flags[ idx1 ] = true;
        if( set( o, o->data[ idx1 ], depth + 1 ) < size )
        {
            o->flags[ idx1 ] = false;
            o->data[  idx1 ] = name;
            return idx1;
        }
        o->flags[ idx1 ] = false;
    }

    if( !o->flags[ idx2 ] )
    {
        o->flags[ idx2 ] = true;
        if( set( o, o->data[ idx2 ], depth + 1 ) < size )
        {
            o->flags[ idx2 ] = false;
            o->data[  idx2 ] = name;
            return idx2;
        }
        o->flags[ idx2 ] = false;
    }

    return size;
}

sz_t set_rehash( bcore_name_map_s* o, bcore_name_s name )
{
    if( !name.key ) ERR( "key is zero" );

    // set
    {
        sz_t idx = find( o, name.key );
        if( idx < o->size ) return idx;
        idx = set( o, name, 1 );
        if( idx < o->size ) return idx;
    }

    // rehash
    {
        bcore_name_s* buf_data  = o->data;
        sz_t          buf_size  = o->size;
        o->flags = bcore_u_alloc( sizeof( bl_t ), o->flags, 0, NULL );
        o->depth_limit = ( o->size > 0 ) ? o->depth_limit + 1 : 4;
        o->size        = ( o->size > 0 ) ? o->size * 2        : 8;
        if( o->size > o->size_limit ) ERR( "size limit (%zu) exceeded", o->size_limit );
        o->data = bcore_u_memzero( sizeof( bcore_name_s ), NULL, o->size );
        for( sz_t i = 0; i < buf_size; i++ ) if( buf_data[ i ].key ) set_rehash( o, buf_data[ i ] );
        bcore_u_alloc( sizeof( tp_t ), buf_data, 0, NULL );
    }

    return set_rehash( o, name );
}

bcore_name_s* bcore_name_map_s_get( const bcore_name_map_s* o, tp_t key )
{
    sz_t idx = find( o, key );
    return ( idx < o->size ) ? &o->data[ idx ] : NULL;
}

void bcore_name_map_s_set( bcore_name_map_s* o, bcore_name_s name )
{
    if( name.name_space && !bcore_name_map_s_exists( o, name.name_space ) )
    {
        ERR( "Namespace %"PRItp_t" not found", name.name_space );
    }
    set_rehash( o, name );
}

void bcore_name_map_s_remove( bcore_name_map_s* o, tp_t key )
{
    sz_t idx = find( o, key );
    if( idx < o->size ) bcore_name_s_clear( &o->data[ idx ] );
}

bl_t bcore_name_map_s_exists( const bcore_name_map_s* o, tp_t key )
{
    return find( o, key ) < o->size;
}

sz_t bcore_name_map_s_keys( const bcore_name_map_s* o )
{
    sz_t n = 0;
    for( sz_t i = 0; i < o->size; i++ ) n += ( o->data[ i ].key != 0 );
    return n;
}

sz_t bcore_name_map_s_size( const bcore_name_map_s* o )
{
    return o->size;
}

bcore_name_s* bcore_name_map_s_idx_name( const bcore_name_map_s* o, sz_t idx )
{
    return idx < o->size ? &o->data[ idx ] : NULL;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_name_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_name" ) ) return NULL;
    if( signal == typeof( "init1" ) )
    {
    }
    else if( signal == typeof( "selftest" ) )
    {
    }
    else if( signal == typeof( "down0" ) )
    {
    }
    return NULL;
}

