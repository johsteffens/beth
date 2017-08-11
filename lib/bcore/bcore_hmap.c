/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_hmap.h"
#include "bcore_life.h"
#include "bcore_spect_inst.h"

typedef struct bcore_hnode_u2vd_s
{
    u2_t key;
    unsigned flag_trace : 1; // used internally during rehashing
    unsigned flag_holds : 1; // object referenced by value is self aware and managed by hmap (always false for function pointers)
    union
    {
        vd_t dp; // data pointer
        fp_t fp; // function pointer
    };
} bcore_hnode_u2vd_s;

static void bcore_hnode_u2vd_s_init( bcore_hnode_u2vd_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

static void bcore_hnode_u2vd_s_down( bcore_hnode_u2vd_s* o )
{
    if( o->flag_holds && o->dp ) bcore_inst_aware_discard( o->dp );
}

static void bcore_hnode_u2vd_s_clear( bcore_hnode_u2vd_s* o )
{
    if( o->flag_holds && o->dp ) bcore_inst_aware_discard( o->dp );
    o->dp = 0;
    o->flag_holds = 0;
    o->key = 0;
}

static void bcore_hnode_u2vd_s_copy( bcore_hnode_u2vd_s* o, const bcore_hnode_u2vd_s* src )
{
    if( o->flag_holds && o->dp )
    {
        bcore_inst_aware_discard( o->dp );
        o->dp = 0;
    }

    bcore_memcpy( o, src, sizeof( *o ) );

    if( src->flag_holds && src->dp )
    {
        o->dp = bcore_inst_aware_clone( src->dp );
    }
}

static void bcore_hnode_u2vd_s_set_val( bcore_hnode_u2vd_s* o, vd_t val, bool hold )
{
    if( o->flag_holds )
    {
        if( o->dp ) bcore_inst_aware_discard( o->dp );
        o->flag_holds = 0;
    }
    o->dp = val;
    o->flag_holds = hold;
}

static void bcore_hnode_u2vd_s_set_func( bcore_hnode_u2vd_s* o, fp_t func )
{
    if( o->flag_holds )
    {
        if( o->dp ) bcore_inst_aware_discard( o->dp );
        o->flag_holds = 0;
    }
    o->fp = func;
    o->flag_holds = false;
}

/**********************************************************************************************************************/

/// hashing (non-cryptographic)
static u2_t hash_u2u2_1( u2_t key )
{
    u2_t h = ( 632432329 ^ ( ( key       ) & 0x0FFFFu ) ) * 88888888901;
         h = ( h         ^ ( ( key >> 16 ) & 0x0FFFFu ) ) * 88888888901;
    return h;
}

/// hashing (non-cryptographic)
static u2_t hash_u2u2_2( u2_t key )
{
    u2_t h = ( 368653234 ^ ( ( key       ) & 0x0FFFFu ) ) * 77777777827;
         h = ( h         ^ ( ( key >> 16 ) & 0x0FFFFu ) ) * 77777777827;
    return h;
}

void bcore_hmap_u2vd_s_init( bcore_hmap_u2vd_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = typeof( "bcore_hmap_u2vd_s" );
    o->size_limit = 0xFFFFFFFFu;
    o->h1         = hash_u2u2_1;
    o->h2         = hash_u2u2_2;
    o->h3         = NULL;
}

void bcore_hmap_u2vd_s_down( bcore_hmap_u2vd_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_hnode_u2vd_s_down( &o->data[ i ] );
    bcore_un_alloc( sizeof( bcore_hnode_u2vd_s ), o->data, o->space, 0, NULL );
}

void bcore_hmap_u2vd_s_copy( bcore_hmap_u2vd_s* o, const bcore_hmap_u2vd_s* src )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_hnode_u2vd_s_down( &o->data[ i ] );
    if( src->size > o->space )
    {
        o->data = bcore_un_alloc( sizeof( bcore_hnode_u2vd_s ), o->data, o->space,  0, &o->space );
        o->data = bcore_un_alloc( sizeof( bcore_hnode_u2vd_s ), o->data, 0, src->size, &o->space );
    }
    o->size = src->size;
    for( sz_t i = 0; i < o->size; i++ )
    {
        bcore_hnode_u2vd_s_init( &o->data[ i ] );
        bcore_hnode_u2vd_s_copy( &o->data[ i ], &src->data[ i ] );
    }
    o->depth_limit = src->depth_limit;
    o->size_limit  = src->size_limit;
    o->h1          = src->h1;
    o->h2          = src->h2;
    o->h3          = src->h3;
}

static void set_size( bcore_hmap_u2vd_s* o, sz_t size )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_hnode_u2vd_s_clear( &o->data[ i ] );

    if( ( size & ( size - 1 ) ) != 0 ) ERR( "size %zu must be a power of two", size );
    if( size > o->space )
    {
        o->data = bcore_un_alloc( sizeof( bcore_hnode_u2vd_s ), o->data, o->space, 0, &o->space );
        o->data = bcore_un_alloc( sizeof( bcore_hnode_u2vd_s ), o->data, 0, size,  &o->space );
    }
    o->size = size;
    bcore_memzero( o->data, sizeof( bcore_hnode_u2vd_s ) * o->size );

    // we set depth_limit to log2( size ) + 1;
    o->depth_limit = 1;
    while( size >> o->depth_limit ) o->depth_limit++;
}

DEFINE_FUNCTION_CREATE(  bcore_hmap_u2vd_s )
DEFINE_FUNCTION_DISCARD( bcore_hmap_u2vd_s )
DEFINE_FUNCTION_CLONE(   bcore_hmap_u2vd_s )

/**********************************************************************************************************************/

static sz_t find( const bcore_hmap_u2vd_s* o, u2_t key ) // returns valid index or o->size
{
    if( o->size == 0 ) return o->size;
    u2_t mask = o->size - 1;
    if( o->h1 )
    {
        sz_t idx = o->h1( key ) & mask;
        if( o->data[ idx ].key == key ) return idx;
    }

    if( o->h2 )
    {
        sz_t idx = o->h2( key ) & mask;
        if( o->data[ idx ].key == key ) return idx;
    }

    if( o->h3 )
    {
        sz_t idx = o->h3( key ) & mask;
        if( o->data[ idx ].key == key ) return idx;
    }

    return o->size;
}

static bcore_hnode_u2vd_s* set( const bcore_hmap_u2vd_s* o, bcore_hnode_u2vd_s node, sz_t depth ) // sets new node
{
    if( o->size == 0 ) return NULL;
    u2_t mask = o->size - 1;
    node.flag_trace = 0;
    if( depth == o->depth_limit ) return NULL;

    bcore_hnode_u2vd_s* node1 = NULL;
    if( o->h1 )
    {
        node1 = &o->data[ o->h1( node.key ) & mask ];
        if( node1->key == 0 )
        {
            *node1 = node;
            return node1;
        }
    }

    bcore_hnode_u2vd_s* node2 = NULL;
    if( o->h2 )
    {
        node2 = &o->data[ o->h2( node.key ) & mask ];
        if( node2->key == 0 )
        {
            *node2 = node;
            return node2;
        }
    }

    bcore_hnode_u2vd_s* node3 = NULL;
    if( o->h3 )
    {
        node3 = &o->data[ o->h3( node.key ) & mask ];
        if( node3->key == 0 )
        {
            *node3 = node;
            return node3;
        }
    }

    if( node1 && !node1->flag_trace )
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

    if( node2 && !node2->flag_trace )
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

    if( node3 && !node3->flag_trace )
    {
        node3->flag_trace = 1;
        if( set( o, *node3, depth + 1 ) )
        {
            *node3 = node;
            return node3;
        }
        else
        {
            node3->flag_trace = 0;
        }
    }

    return NULL;
}

void bcore_hmap_u2vd_s_set_hash_function( bcore_hmap_u2vd_s* o, sz_t index, bcore_hash_u2u2 hf )
{
    if( index > 2 ) ERR( "index (%zu) out of range", index );
    switch( index )
    {
        case 0: o->h1 = hf; break;
        case 1: o->h2 = hf; break;
        case 2: o->h3 = hf; break;
        default: break;
    }
}

vd_t* bcore_hmap_u2vd_s_get( const bcore_hmap_u2vd_s* o, u2_t key )
{
    if( !key ) return NULL;
    sz_t idx = find( o, key );
    return ( idx < o->size ) ? &o->data[ idx ].dp : NULL;
}

fp_t* bcore_hmap_u2vd_s_getf( const bcore_hmap_u2vd_s* o, u2_t key )
{
    if( !key ) return NULL;
    sz_t idx = find( o, key );
    return ( idx < o->size ) ? &o->data[ idx ].fp : NULL;
}

void bcore_hmap_u2vd_s_set( bcore_hmap_u2vd_s* o, u2_t key, vd_t val, bool hold )
{
    if( !key ) ERR( "key is zero" );

    {
        sz_t idx = find( o, key );
        if( idx < o->size )
        {
            bcore_hnode_u2vd_s_set_val( &o->data[ idx ], val, hold );
            return;
        }
    }

    {
        bcore_hnode_u2vd_s node;
        bcore_hnode_u2vd_s_init( &node );
        node.key = key;
        bcore_hnode_u2vd_s* p_node = set( o, node, 1 );
        if( p_node )
        {
            bcore_hnode_u2vd_s_set_val( p_node, val, hold );
            return;
        }
    }

    // rehash
    {
        bcore_hnode_u2vd_s* buf_data  = o->data;
        sz_t         buf_size  = o->size;
        sz_t         buf_space = o->space;
        if( o->size > 0 )
        {
            o->size *= 2;
            o->depth_limit++;
        }
        else
        {
            o->size = 8;
            o->depth_limit = 4;
        }
        if( o->size > o->size_limit ) ERR( "size limit (%zu) exceeded", o->size_limit );
        o->data = bcore_u_alloc( sizeof( bcore_hnode_u2vd_s ), NULL, o->size, &o->space );
        bcore_memzero( o->data, sizeof( bcore_hnode_u2vd_s ) * o->size );
        for( sz_t i = 0; i < buf_size; i++ )
        {
            bcore_hnode_u2vd_s* p_node = &buf_data[ i ];
            if( p_node->key ) bcore_hmap_u2vd_s_set( o, p_node->key, p_node->dp, p_node->flag_holds );
        }
        bcore_un_alloc( sizeof( bcore_hnode_u2vd_s ), buf_data, buf_space, 0, NULL );
    }

    bcore_hmap_u2vd_s_set( o, key, val, hold );
}

void bcore_hmap_u2vd_s_setf( bcore_hmap_u2vd_s* o, u2_t key, fp_t func )
{
    if( !key ) ERR( "key is zero" );

    {
        sz_t idx = find( o, key );
        if( idx < o->size )
        {
            bcore_hnode_u2vd_s_set_func( &o->data[ idx ], func );
            return;
        }
    }

    {
        bcore_hnode_u2vd_s node;
        bcore_hnode_u2vd_s_init( &node );
        node.key = key;
        bcore_hnode_u2vd_s* p_node = set( o, node, 1 );
        if( p_node )
        {
            bcore_hnode_u2vd_s_set_func( p_node, func );
            return;
        }
    }

    // rehash
    {
        bcore_hnode_u2vd_s* buf_data  = o->data;
        sz_t buf_size  = o->size;
        sz_t buf_space = o->space;
        if( o->size > 0 )
        {
            o->size *= 2;
            o->depth_limit++;
        }
        else
        {
            o->size = 8;
            o->depth_limit = 4;
        }
        if( o->size > o->size_limit ) ERR( "size limit (%zu) exceeded", o->size_limit );
        o->data = bcore_u_alloc( sizeof( bcore_hnode_u2vd_s ), NULL, o->size, &o->space );
        bcore_memzero( o->data, sizeof( bcore_hnode_u2vd_s ) * o->size );
        for( sz_t i = 0; i < buf_size; i++ )
        {
            bcore_hnode_u2vd_s* p_node = &buf_data[ i ];
            if( p_node->key ) bcore_hmap_u2vd_s_set( o, p_node->key, p_node->dp, p_node->flag_holds );
        }
        bcore_un_alloc( sizeof( bcore_hnode_u2vd_s ), buf_data, buf_space, 0, NULL );
    }

    bcore_hmap_u2vd_s_setf( o, key, func );
}

vd_t bcore_hmap_u2vd_s_remove_h( bcore_hmap_u2vd_s* o, u2_t key )
{
    if( !key ) return NULL;

    sz_t idx = find( o, key );
    if( idx < o->size )
    {
        bcore_hnode_u2vd_s* node = &o->data[ idx ];
        vd_t ret = node->dp;
        node->key = 0;
        node->dp  = 0;
        node->flag_holds = 0;
        return ret;
    }
    return NULL;
}

fp_t bcore_hmap_u2vd_s_removef_h( bcore_hmap_u2vd_s* o, u2_t key )
{
    if( !key ) return NULL;

    sz_t idx = find( o, key );
    if( idx < o->size )
    {
        bcore_hnode_u2vd_s* node = &o->data[ idx ];
        fp_t ret = node->fp;
        node->key = 0;
        node->dp  = 0;
        node->flag_holds = 0;
        return ret;
    }
    return NULL;
}

void bcore_hmap_u2vd_s_remove_d( bcore_hmap_u2vd_s* o, u2_t key )
{
    if( !key ) return;
    sz_t idx = find( o, key );
    if( idx < o->size )
    {
        bcore_hnode_u2vd_s* node = &o->data[ idx ];
        if( node->flag_holds && node->dp ) bcore_inst_aware_discard( node->dp );
        node->key = 0;
        node->dp = 0;
        node->flag_holds = 0;
    }
    return;
}

vd_t bcore_hmap_u2vd_s_detach_h( bcore_hmap_u2vd_s* o, u2_t key )
{
    if( !key ) return NULL;

    sz_t idx = find( o, key );
    if( idx < o->size )
    {
        bcore_hnode_u2vd_s* node = &o->data[ idx ];
        vd_t ret = node->dp;
        node->flag_holds = 0;
        return ret;
    }
    return NULL;
}

bool bcore_hmap_u2vd_s_exists( const bcore_hmap_u2vd_s* o, u2_t key )
{
    if( !key ) return false;
    sz_t idx = find( o, key );
    return ( idx < o->size );
}

void bcore_hmap_u2vd_s_clear( bcore_hmap_u2vd_s* o )
{
    set_size( o, 0 );
}

sz_t bcore_hmap_u2vd_s_keys( const bcore_hmap_u2vd_s* o )
{
    sz_t count = 0;
    for( sz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ].key > 0 );
    return count;
}

sz_t bcore_hmap_u2vd_s_size( const bcore_hmap_u2vd_s* o )
{
    return o->size;
}

u2_t bcore_hmap_u2vd_s_idx_key( const bcore_hmap_u2vd_s* o, sz_t idx )
{
    assert( idx < o->size );
    return o->data[ idx ].key;
}

bool bcore_hmap_u2vd_s_idx_holds( const bcore_hmap_u2vd_s* o, sz_t idx )
{
    assert( idx < o->size );
    return o->data[ idx ].flag_holds;
}

vd_t bcore_hmap_u2vd_s_idx_val( const bcore_hmap_u2vd_s* o, sz_t idx )
{
    assert( idx < o->size );
    return o->data[ idx ].dp;
}

void bcore_hmap_u2vd_s_run_c( const bcore_hmap_u2vd_s* o, vd_t obj, void (*fp)( vd_t obj, u2_t key, vd_t val ) )
{
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->data[ i ].key > 0 ) fp( obj, o->data[ i ].key, o->data[ i ].dp );
    }
}

void bcore_hmap_u2vd_s_run_d( bcore_hmap_u2vd_s* o, vd_t obj, void (*fp)( vd_t obj, u2_t key, vd_t* val ) )
{
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->data[ i ].key > 0 ) fp( obj, o->data[ i ].key, &o->data[ i ].dp );
    }
}

/**********************************************************************************************************************/

bcore_flect_self_s* bcore_hmap_u2vd_s_create_self( void )
{
    sc_t def = "\
        bcore_hmap_u2vd_s = \
            { \
                aware_t _; \
                private bcore_static_array_s arr; \
                sz_t depth_limit; \
                sz_t size_limit; \
                private fp_t h1; \
                private fp_t h2; \
                private fp_t h3; \
            }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_hmap_u2vd_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_hmap_u2vd_s_init,    "bcore_fp_init",    "init"    );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_hmap_u2vd_s_down,    "bcore_fp_down",    "down"    );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_hmap_u2vd_s_copy,    "bcore_fp_copy",    "copy"    );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_hmap_u2vd_s_create,  "bcore_fp_create",  "create"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_hmap_u2vd_s_clone,   "bcore_fp_clone",   "clone"   );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_hmap_u2vd_s_discard, "bcore_fp_discard", "discard" );
    return self;
}

/**********************************************************************************************************************/

bcore_string_s* bcore_hmap_u2vd_s_status( bcore_hmap_u2vd_s* o )
{
    bcore_string_s* string = bcore_string_s_create();
    sz_t keys = bcore_hmap_u2vd_s_keys( o );
    bcore_string_s_pushf( string, "keys ........... %zu\n", keys );
    bcore_string_s_pushf( string, "nodes .......... %lu\n", o->size );
    bcore_string_s_pushf( string, "keys/nodes ..... %5.4f\n", o->size > 0 ? ( f3_t )( keys ) / o->size : 0 );
    return string;
}

void bcore_hmap_u2vd_filltest()
{
    bcore_life_s* life = bcore_life_s_create();
    bcore_hmap_u2vd_s* map = bcore_life_s_push( life, bcore_hmap_u2vd_s_discard, bcore_hmap_u2vd_s_create() );

    sz_t cycles = 2;
    sz_t size = 65536 * 64;

    for( sz_t i = 0; i < cycles; i++ )
    {
        bcore_hmap_u2vd_s_clear( map );
        u2_t key = bcore_xsg3_u2( i + 1 );
        for( sz_t j = 0; j < size; j++ )
        {
            key = bcore_xsg_u2( key );
            sz_t size1 = map->size;
            bcore_hmap_u2vd_s_set( map, key, 0, false );
            sz_t size2 = map->size;
            if( size2 > size1 )
            {
                bcore_string_s_print_d( bcore_string_s_createf( "%zu/%zu (%f)\n", j, size1, size1 > 0 ? ( f3_t ) j / size1 : 0.0 ) );
            }
        }
        bcore_string_s_print_d( bcore_hmap_u2vd_s_status( map ) );
    }

    bcore_life_s_discard( life );
}

/**********************************************************************************************************************/

#include <time.h> // only for selftest

bcore_string_s* bcore_hmap_u2vd_selftest( void )
{
    bcore_string_s* log = bcore_string_s_createf( "== bcore_hmap_u2vd_selftest " );
    bcore_string_s_push_char_n( log, '=', 120 - log->size );
    bcore_string_s_push_char( log, '\n' );

    bcore_hmap_u2vd_s* map = bcore_hmap_u2vd_s_create();
    const sz_t cycles = 1000000;

    typedef struct
    {
        u2_t key;
        vd_t val;
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
            kv.key = ( u2_t )rval1;
            kv.val = ( vd_t )rval2;
            kvbuf[ kvbuf_size++ ] = kv;

            // set
            bcore_hmap_u2vd_s_set( map, kv.key, kv.val, false );

            // retrieve
            rval1 = bcore_xsg_u2( rval1 );
            kv = kvbuf[ rval1 % kvbuf_size ];
            vd_t* val_ptr = bcore_hmap_u2vd_s_get( map, kv.key );
            if( kv.val != *val_ptr ) ERR( "value mismatch (%lu vs %lu)", kv.val, *val_ptr );

            // delete
            rval1 = bcore_xsg_u2( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_xsg_u2( rval1 );
                sz_t idx = rval1 % kvbuf_size;
                kv_s kv = kvbuf[ idx ];
                if( !bcore_hmap_u2vd_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
                bcore_hmap_u2vd_s_remove_d( map, kv.key );
                if( bcore_hmap_u2vd_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    bcore_string_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    bcore_string_s_push_string_d( log, bcore_hmap_u2vd_s_status( map ) );

    time = clock();
    bcore_string_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    sz_t read_cycles = 20;
    for( sz_t j = 0; j < read_cycles; j++ )
    {
        for( sz_t i = 0; i < kvbuf_size; i++ )
        {
            if( *bcore_hmap_u2vd_s_get( map, kvbuf[ i ].key ) != kvbuf[ i ].val ) ERR( "value mismatch (%lu vs %lu)", kvbuf[ i ].key, kvbuf[ i ].val );
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
        if( !bcore_hmap_u2vd_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
        bcore_hmap_u2vd_s_remove_d( map, kv.key );
        if( bcore_hmap_u2vd_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    bcore_string_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    bcore_string_s_push_string_d( log, bcore_hmap_u2vd_s_status( map ) );

    bcore_hmap_u2vd_s_discard( map );
    bcore_alloc( kvbuf, 0 );

    bcore_string_s_push_char_n( log, '=', 120 );
    bcore_string_s_push_char( log, '\n' );
    return log;
}
