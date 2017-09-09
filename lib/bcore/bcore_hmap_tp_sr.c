/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_hmap_tp_sr.h"
#include "bcore_life.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_compare.h"

void bcore_hnode_tp_sr_s_init( bcore_hnode_tp_sr_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void bcore_hnode_tp_sr_s_down( bcore_hnode_tp_sr_s* o )
{
    sr_s_down( &o->val );
}

void bcore_hnode_tp_sr_s_clear( bcore_hnode_tp_sr_s* o )
{
    sr_s_clear( &o->val );
    o->key = 0;
}

void bcore_hnode_tp_sr_s_copy( bcore_hnode_tp_sr_s* o, const bcore_hnode_tp_sr_s* src )
{
    o->flag_trace = src->flag_trace;
    o->key = src->key;
    sr_s_copy( &o->val, &src->val );
}

sr_s* bcore_hnode_tp_sr_s_set( bcore_hnode_tp_sr_s* o, sr_s obj )
{
    sr_s_set( &o->val, obj );
    return &o->val;
}

/**********************************************************************************************************************/

static bcore_flect_self_s* hnode_tp_sr_s_create_self( void )
{
    sc_t def = "\
        bcore_hnode_tp_sr_s = \
        { \
            private bl_t flag_trace; \
            tp_t key; \
            sr_s val; \
        }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_hnode_tp_sr_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_hnode_tp_sr_s_init, "bcore_fp_init", "init" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_hnode_tp_sr_s_down, "bcore_fp_down", "down" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_hnode_tp_sr_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

/**********************************************************************************************************************/
// bcore_hmap_tp_sr_s

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

void bcore_hmap_tp_sr_s_init( bcore_hmap_tp_sr_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = typeof( "bcore_hmap_tp_sr_s" );
    o->size_limit = 0xFFFFFFFFu;
}

void bcore_hmap_tp_sr_s_down( bcore_hmap_tp_sr_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_hnode_tp_sr_s_down( &o->data[ i ] );
    bcore_un_alloc( sizeof( bcore_hnode_tp_sr_s ), o->data, o->space, 0, NULL );
}

void bcore_hmap_tp_sr_s_copy( bcore_hmap_tp_sr_s* o, const bcore_hmap_tp_sr_s* src )
{
    bcore_inst_aware_copy( o, src );
}

static void set_size( bcore_hmap_tp_sr_s* o, sz_t size )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_hnode_tp_sr_s_clear( &o->data[ i ] );

    if( ( size & ( size - 1 ) ) != 0 ) ERR( "size %zu must be a power of two", size );
    if( size > o->space )
    {
        o->data = bcore_un_alloc( sizeof( bcore_hnode_tp_sr_s ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( bcore_hnode_tp_sr_s ), o->data, 0, size,  &o->space );
    }
    o->size = size;
    bcore_memzero( o->data, sizeof( bcore_hnode_tp_sr_s ) * o->size );

    // we set depth_limit to log2( size ) + 1;
    o->depth_limit = 1;
    while( size >> o->depth_limit ) o->depth_limit++;
}

DEFINE_FUNCTION_CREATE(  bcore_hmap_tp_sr_s )
DEFINE_FUNCTION_DISCARD( bcore_hmap_tp_sr_s )
DEFINE_FUNCTION_CLONE(   bcore_hmap_tp_sr_s )

/**********************************************************************************************************************/

static sz_t find( const bcore_hmap_tp_sr_s* o, u2_t key ) // returns valid index or o->size
{
    if( o->size == 0 ) return o->size;
    u2_t mask = o->size - 1;
    {
        sz_t idx = hash_tpu2_1( key ) & mask;
        if( o->data[ idx ].key == key ) return idx;
    }

    {
        sz_t idx = hash_tpu2_2( key ) & mask;
        if( o->data[ idx ].key == key ) return idx;
    }

    return o->size;
}

static bcore_hnode_tp_sr_s* set( const bcore_hmap_tp_sr_s* o, bcore_hnode_tp_sr_s node, sz_t depth ) // sets new node
{
    if( o->size == 0 ) return NULL;
    u2_t mask = o->size - 1;
    node.flag_trace = 0;
    if( depth == o->depth_limit ) return NULL;

    bcore_hnode_tp_sr_s* node1 = &o->data[ hash_tpu2_1( node.key ) & mask ];
    if( node1->key == 0 )
    {
        *node1 = node;
        return node1;
    }

    bcore_hnode_tp_sr_s* node2 = &o->data[ hash_tpu2_2( node.key ) & mask ];
    if( node2->key == 0 )
    {
        *node2 = node;
        return node2;
    }

    if( !node1->flag_trace )
    {
        node1->flag_trace = 1;
        if( set( o, *node1, depth + 1 ) )
        {
            *node1 = node;
            return node1;
        }
        else
        {
            node1->flag_trace = 0;
        }
    }

    if( !node2->flag_trace )
    {
        node2->flag_trace = 1;
        if( set( o, *node2, depth + 1 ) )
        {
            *node2 = node;
            return node2;
        }
        else
        {
            node2->flag_trace = 0;
        }
    }

    return NULL;
}

sr_s* bcore_hmap_tp_sr_s_get( const bcore_hmap_tp_sr_s* o, tp_t key )
{
    if( !key ) return NULL;
    sz_t idx = find( o, key );
    return ( idx < o->size ) ? &o->data[ idx ].val : NULL;
}

sr_s* bcore_hmap_tp_sr_s_set( bcore_hmap_tp_sr_s* o, tp_t key, sr_s val )
{
    if( !key ) ERR( "key is zero" );

    {
        sz_t idx = find( o, key );
        if( idx < o->size ) return bcore_hnode_tp_sr_s_set( &o->data[ idx ], val );
    }

    {
        bcore_hnode_tp_sr_s node;
        bcore_hnode_tp_sr_s_init( &node );
        node.key = key;
        bcore_hnode_tp_sr_s* p_node = set( o, node, 1 );
        if( p_node ) return bcore_hnode_tp_sr_s_set( p_node, val );
    }

    // rehash
    {
        bcore_hnode_tp_sr_s* buf_data  = o->data;
        sz_t         buf_size  = o->size;
        sz_t         buf_space = o->space;
        if( o->size > 0 )
        {
            o->size *= 2;
            o->depth_limit++;
        }
        else
        {
            o->size = 4;
            o->depth_limit = 3;
        }
        if( o->size > o->size_limit ) ERR( "size limit (%zu) exceeded", o->size_limit );
        o->data = bcore_u_alloc( sizeof( bcore_hnode_tp_sr_s ), NULL, o->size, &o->space );

        // Note: We can safely reset the memory as long as sr_s init state is all zero
        bcore_memzero( o->data, sizeof( bcore_hnode_tp_sr_s ) * o->size );

        for( sz_t i = 0; i < buf_size; i++ )
        {
            bcore_hnode_tp_sr_s* p_node = &buf_data[ i ];
            if( p_node->key ) bcore_hmap_tp_sr_s_set( o, p_node->key, p_node->val );
        }

        // ownership of all buffered objects have been transferred -> no explicit shutting down
        bcore_un_alloc( sizeof( bcore_hnode_tp_sr_s ), buf_data, buf_space, 0, NULL );
    }

    return bcore_hmap_tp_sr_s_set( o, key, val );
}

sr_s bcore_hmap_tp_sr_s_remove( bcore_hmap_tp_sr_s* o, tp_t key )
{
    if( key )
    {
        sz_t idx = find( o, key );
        if( idx < o->size )
        {
            bcore_hnode_tp_sr_s* node = &o->data[ idx ];
            sr_s ret = node->val;
            node->val = sr_null();
            node->key = 0;
            return ret;
        }
    }

    return sr_null();
}

bl_t bcore_hmap_tp_sr_s_exists( const bcore_hmap_tp_sr_s* o, u2_t key )
{
    if( !key ) return false;
    sz_t idx = find( o, key );
    return ( idx < o->size );
}

void bcore_hmap_tp_sr_s_clear( bcore_hmap_tp_sr_s* o )
{
    set_size( o, 0 );
}

sz_t bcore_hmap_tp_sr_s_keys( const bcore_hmap_tp_sr_s* o )
{
    sz_t count = 0;
    for( sz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ].key > 0 );
    return count;
}

sz_t bcore_hmap_tp_sr_s_size( const bcore_hmap_tp_sr_s* o )
{
    return o->size;
}

u2_t bcore_hmap_tp_sr_s_idx_key( const bcore_hmap_tp_sr_s* o, sz_t idx )
{
    assert( idx < o->size );
    return o->data[ idx ].key;
}

const sr_s* bcore_hmap_tp_sr_s_idx_val( const bcore_hmap_tp_sr_s* o, sz_t idx )
{
    assert( idx < o->size );
    return &o->data[ idx ].val;
}

/**********************************************************************************************************************/

static bcore_flect_self_s* hmap_tp_sr_s_create_self( void )
{
    sc_t def = "\
        bcore_hmap_tp_sr_s = \
        { \
            aware_t _; \
            bcore_hnode_tp_sr_s [] arr; \
            sz_t depth_limit; \
            sz_t size_limit; \
        }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_hmap_tp_sr_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_hmap_tp_sr_s_init,    "bcore_fp_init",    "init"    );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_hmap_tp_sr_s_down,    "bcore_fp_down",    "down"    );
    return self;
}

/**********************************************************************************************************************/

void bcore_hmap_tp_sr_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bcore_hnode_tp_sr_s" ), hnode_tp_sr_s_create_self );
    bcore_flect_define_creator( typeof( "bcore_hmap_tp_sr_s"  ), hmap_tp_sr_s_create_self  );
}

vd_t bcore_hmap_tp_sr_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_hmap_tp_sr" ) ) return NULL;

    if( signal == typeof( "init" ) )
    {
        bcore_flect_define_creator( typeof( "bcore_hnode_tp_sr_s" ), hnode_tp_sr_s_create_self );
        bcore_flect_define_creator( typeof( "bcore_hmap_tp_sr_s"  ), hmap_tp_sr_s_create_self  );
    }

    return NULL;
}

/**********************************************************************************************************************/
// selftest

#include <time.h>
#include "bcore_quicktypes.h"

bcore_string_s* bcore_hmap_tp_sr_s_status( bcore_hmap_tp_sr_s* o )
{
    bcore_string_s* string = bcore_string_s_create();
    sz_t keys = bcore_hmap_tp_sr_s_keys( o );
    bcore_string_s_pushf( string, "keys ........... %zu\n", keys );
    bcore_string_s_pushf( string, "nodes .......... %lu\n", o->size );
    bcore_string_s_pushf( string, "keys/nodes ..... %5.4f\n", o->size > 0 ? ( f3_t )( keys ) / o->size : 0 );
    return string;
}

bcore_string_s* bcore_hmap_tp_sr_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_string_s* log = bcore_string_s_createf( "== bcore_hmap_tp_sr_selftest " );
    bcore_string_s_push_char_n( log, '=', 120 - log->size );
    bcore_string_s_push_char( log, '\n' );

    bcore_hmap_tp_sr_s* map = bcore_life_s_push_aware( l, bcore_hmap_tp_sr_s_create() );
    const sz_t cycles = 1000000;

    typedef struct
    {
        tp_t key;
        u3_t val;
    } kv_s;

    kv_s* kvbuf = bcore_alloc( NULL, cycles * sizeof( kv_s ) );
    sz_t kvbuf_size = 0;

    clock_t time = clock();
    bcore_string_s_pushf( log, "Mixed access: " );

    {
        u3_t rval1 = 1;
        u3_t rval2 = 12345;
        for( sz_t i = 0; i < cycles; i++ )
        {
            rval1 = bcore_xsg_u2( rval1 );
            rval2 = bcore_xsg_u2( rval2 );
            kv_s kv;
            kv.key = ( tp_t )rval1;
            kv.val = ( u3_t )rval2;
            kvbuf[ kvbuf_size++ ] = kv;

            // set
            bcore_hmap_tp_sr_s_set( map, kv.key, sr_create_strong_typed( TYPEOF_u3_t, &kv.val ) );

            // retrieve
            rval1 = bcore_xsg_u2( rval1 );
            kv = kvbuf[ rval1 % kvbuf_size ];
            u3_t* val_ptr = bcore_hmap_tp_sr_s_get( map, kv.key )->o;
            if( kv.val != *val_ptr ) ERR( "value mismatch (%lu vs %lu)", kv.val, *val_ptr );

            // delete
            rval1 = bcore_xsg_u2( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_xsg_u2( rval1 );
                sz_t idx = rval1 % kvbuf_size;
                kv_s kv = kvbuf[ idx ];
                if( !bcore_hmap_tp_sr_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
                sr_down( bcore_hmap_tp_sr_s_remove( map, kv.key ) );
                if( bcore_hmap_tp_sr_s_get(  map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    bcore_string_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );
    bcore_string_s_push_string_d( log, bcore_hmap_tp_sr_s_status( map ) );

    time = clock();
    bcore_hmap_tp_sr_s* map2 = bcore_life_s_push_aware( l, bcore_hmap_tp_sr_s_clone( map ) );
    time = clock() - time;
    bcore_string_s_pushf( log, "Clone .......... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    if( bcore_compare_aware( map, map2 ) != 0 )
    {
        ERR( "comparison failed:\n%s", bcore_diff_aware( map, map2 )->sc );
    }

    time = clock() - time;
    bcore_string_s_pushf( log, "Comparison ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    bcore_string_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    sz_t read_cycles = 20;
    for( sz_t j = 0; j < read_cycles; j++ )
    {
        for( sz_t i = 0; i < kvbuf_size; i++ )
        {
            u3_t val = *( u3_t* )bcore_hmap_tp_sr_s_get( map, kvbuf[ i ].key )->o;
            if( val != kvbuf[ i ].val ) ERR( "value mismatch (%lu vs %lu)", val, kvbuf[ i ].val );
        }
    }
    time = clock() - time;
    bcore_string_s_pushf( log, "(%gs per access)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * read_cycles ) );

    bcore_string_s_pushf( log, "\n" );
    bcore_string_s_pushf( log, "Removal: " );
    time = clock();
    while( kvbuf_size )
    {
        kvbuf_size--;
        kv_s kv = kvbuf[ kvbuf_size ];
        if( !bcore_hmap_tp_sr_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
        sr_down( bcore_hmap_tp_sr_s_remove( map, kv.key ) );
        if( bcore_hmap_tp_sr_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    bcore_string_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );
    bcore_string_s_push_string_d( log, bcore_hmap_tp_sr_s_status( map ) );
    bcore_alloc( kvbuf, 0 );
    bcore_life_s_discard( l );

    bcore_string_s_push_char_n( log, '=', 120 );
    bcore_string_s_push_char( log, '\n' );
    return log;
}

