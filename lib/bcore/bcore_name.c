/** Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_name.h"
#include "bcore_name_manager.h"
#include "bcore_signal.h"
#include "bcore_flect.h"
#include "bcore_spect_array.h"

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

BCORE_DEFINE_FUNCTION_CREATE( bcore_name_s )
BCORE_DEFINE_FUNCTION_CLONE( bcore_name_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_name_s )

static sr_s name_s_get_name_st( bcore_name_s* o )
{
    return sr_asd( st_s_create_sc( o->name ) );
}

static void name_s_set_name_st( bcore_name_s* o, sr_s sr )
{
    bcore_release( o->name );
    assert( sr_s_type( &sr ) == TYPEOF_st_s );
    o->name = bcore_strcpy( NULL, ( ( st_s* )sr.o )->sc );
    sr_down( sr );
}

static bcore_self_s* name_s_create_self( void )
{
    sc_t def = "\
        bcore_name_s = bcore_inst \
        { \
            tp_t key;\
            tp_t name_space;\
            private sd_t name;\
            shell st_s name_st;\
        }";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_name_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_name_s_init,    "bcore_fp_init",  "init"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_name_s_down,    "bcore_fp_down",  "down"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_name_s_copy,    "bcore_fp_copy",  "copy"     );
    bcore_self_s_push_ns_func( self, ( fp_t )name_s_get_name_st,   "bcore_fp_get",   "get_name_st" );
    bcore_self_s_push_ns_func( self, ( fp_t )name_s_set_name_st,   "bcore_fp_set",   "set_name_st" );
    return self;
}

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

BCORE_DEFINE_FUNCTION_CREATE( bcore_name_map_s )
BCORE_DEFINE_FUNCTION_CLONE( bcore_name_map_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_name_map_s )

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
        if( idx < o->size )
        {
            bcore_name_s_down( &name );
            return idx;
        }
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

static sr_s name_map_s_get_data( const bcore_name_map_s* o )
{
    tp_t t_data = bcore_flect_type_parse_sc( "{ bcore_name_s []; }" );
    tp_t t_node = typeof( "bcore_name_s" );
    sr_s data = sr_cp( sr_create( t_data ), TYPEOF_bcore_array_s );
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->data[ i ].key ) bcore_array_q_push( &data, sr_twc( t_node, &o->data[ i ] ) );
    }
    return data;
}

static void name_map_s_set_data( bcore_name_map_s* o, sr_s data )
{
    bcore_name_map_s_clear( o );
    assert( sr_s_type( &data ) == bcore_flect_type_parse_sc( "{ bcore_name_s []; }" ) );
    data = sr_cp( data, TYPEOF_bcore_array_s );
    sz_t size = bcore_array_q_get_size( &data );
    bcore_name_s* src = bcore_array_q_get_d_data( &data );
    for( sz_t i = 0; i < size; i++ )
    {
        bcore_name_map_s_set( o, src[ i ] );
        src[ i ].name = NULL;
    }
    sr_down( data );
}

static bcore_self_s* name_map_s_create_self( void )
{
    sc_t def = "\
        bcore_name_map_s = bcore_inst \
            { \
                aware_t _; \
                private bcore_name_s* data; \
                private bl_t* flags; \
                private sz_t size; \
                private sz_t depth_limit; \
                private sz_t size_limit; \
                shell { bcore_name_s []; } data; } \
            }";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_name_map_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_name_map_s_init,    "bcore_fp_init",  "init"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_name_map_s_down,    "bcore_fp_down",  "down"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_name_map_s_copy,    "bcore_fp_copy",  "copy"     );
    bcore_self_s_push_ns_func( self, ( fp_t )name_map_s_get_data,      "bcore_fp_get",   "get_data" );
    bcore_self_s_push_ns_func( self, ( fp_t )name_map_s_set_data,      "bcore_fp_set",   "set_data" );
    return self;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_name_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_name" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( TYPEOF_bcore_name_s,     name_s_create_self );
            bcore_flect_define_creator( TYPEOF_bcore_name_map_s, name_map_s_create_self );
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}
