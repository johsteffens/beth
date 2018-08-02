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

#include "bcore_hmap.h"
#include "bcore_life.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_compare.h"
#include "bcore_signal.h"
#include "bcore_spect_array.h"

// only needed for selftests...
#include "bcore_txt_ml.h"
#include "bcore_bin_ml.h"
#include <time.h>

/**********************************************************************************************************************/
// hash functions

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
// bcore_hnode_u2vd_s, bcore_hmap_u2vd_s

typedef struct bcore_hnode_u2vd_s
{
    u2_t key;
    bl_t flag_trace; // used internally during rehashing
    bl_t flag_holds; // object referenced by value is self aware and managed by hmap (always false for function pointers)
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
    if( o->flag_holds && o->dp ) bcore_inst_a_discard( o->dp );
}

static void bcore_hnode_u2vd_s_clear( bcore_hnode_u2vd_s* o )
{
    if( o->flag_holds && o->dp ) bcore_inst_a_discard( o->dp );
    o->dp = 0;
    o->flag_holds = 0;
    o->key = 0;
}

static void bcore_hnode_u2vd_s_copy( bcore_hnode_u2vd_s* o, const bcore_hnode_u2vd_s* src )
{
    if( o->flag_holds && o->dp )
    {
        bcore_inst_a_discard( o->dp );
        o->dp = 0;
    }

    bcore_memcpy( o, src, sizeof( *o ) );

    if( src->flag_holds && src->dp )
    {
        o->dp = bcore_inst_a_clone( src->dp );
    }
}

static void bcore_hnode_u2vd_s_set_val( bcore_hnode_u2vd_s* o, vd_t val, bool hold )
{
    if( o->flag_holds )
    {
        if( o->dp ) bcore_inst_a_discard( o->dp );
        o->flag_holds = 0;
    }
    o->dp = val;
    o->flag_holds = hold;
}

static void bcore_hnode_u2vd_s_set_func( bcore_hnode_u2vd_s* o, fp_t func )
{
    if( o->flag_holds )
    {
        if( o->dp ) bcore_inst_a_discard( o->dp );
        o->flag_holds = 0;
    }
    o->fp = func;
    o->flag_holds = false;
}

/**********************************************************************************************************************/

void bcore_hmap_u2vd_s_init( bcore_hmap_u2vd_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_hmap_u2vd_s;
    o->size_limit = 0xFFFFFFFFu;
    o->h1         = hash_u2u2_1;
    o->h2         = hash_u2u2_2;
    o->h3         = NULL;
}

void bcore_hmap_u2vd_s_down( bcore_hmap_u2vd_s* o )
{
    for( uz_t i = 0; i < o->size; i++ ) bcore_hnode_u2vd_s_down( &o->data[ i ] );
    bcore_un_alloc( sizeof( bcore_hnode_u2vd_s ), o->data, o->space, 0, NULL );
}

void bcore_hmap_u2vd_s_copy( bcore_hmap_u2vd_s* o, const bcore_hmap_u2vd_s* src )
{
    for( uz_t i = 0; i < o->size; i++ ) bcore_hnode_u2vd_s_down( &o->data[ i ] );
    if( src->size > o->space )
    {
        o->data = bcore_un_alloc( sizeof( bcore_hnode_u2vd_s ), o->data, o->space,  0, &o->space );
        o->data = bcore_un_alloc( sizeof( bcore_hnode_u2vd_s ), o->data, 0, src->size, &o->space );
    }
    o->size = src->size;
    for( uz_t i = 0; i < o->size; i++ )
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

static void u2vd_set_size( bcore_hmap_u2vd_s* o, uz_t size )
{
    for( uz_t i = 0; i < o->size; i++ ) bcore_hnode_u2vd_s_clear( &o->data[ i ] );

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

BCORE_DEFINE_FUNCTION_CREATE(  bcore_hmap_u2vd_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_hmap_u2vd_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_hmap_u2vd_s )

/**********************************************************************************************************************/

static uz_t u2vd_find( const bcore_hmap_u2vd_s* o, u2_t key ) // returns valid index or o->size
{
    if( o->size == 0 ) return o->size;
    u2_t mask = o->size - 1;
    if( o->h1 )
    {
        uz_t idx = o->h1( key ) & mask;
        if( o->data[ idx ].key == key ) return idx;
    }

    if( o->h2 )
    {
        uz_t idx = o->h2( key ) & mask;
        if( o->data[ idx ].key == key ) return idx;
    }

    if( o->h3 )
    {
        uz_t idx = o->h3( key ) & mask;
        if( o->data[ idx ].key == key ) return idx;
    }

    return o->size;
}

static bcore_hnode_u2vd_s* u2vd_set( const bcore_hmap_u2vd_s* o, bcore_hnode_u2vd_s node, uz_t depth ) // sets new node
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
        if( u2vd_set( o, *node1, depth + 1 ) )
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
        if( u2vd_set( o, *node2, depth + 1 ) )
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
        if( u2vd_set( o, *node3, depth + 1 ) )
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

void bcore_hmap_u2vd_s_set_hash_function( bcore_hmap_u2vd_s* o, uz_t index, bcore_hash_u2u2 hf )
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
    uz_t idx = u2vd_find( o, key );
    return ( idx < o->size ) ? &o->data[ idx ].dp : NULL;
}

fp_t* bcore_hmap_u2vd_s_getf( const bcore_hmap_u2vd_s* o, u2_t key )
{
    if( !key ) return NULL;
    uz_t idx = u2vd_find( o, key );
    return ( idx < o->size ) ? &o->data[ idx ].fp : NULL;
}

void bcore_hmap_u2vd_s_set( bcore_hmap_u2vd_s* o, u2_t key, vd_t val, bool hold )
{
    if( !key ) ERR( "key is zero" );

    {
        uz_t idx = u2vd_find( o, key );
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
        bcore_hnode_u2vd_s* p_node = u2vd_set( o, node, 1 );
        if( p_node )
        {
            bcore_hnode_u2vd_s_set_val( p_node, val, hold );
            return;
        }
    }

    // rehash
    {
        bcore_hnode_u2vd_s* buf_data  = o->data;
        uz_t         buf_size  = o->size;
        uz_t         buf_space = o->space;
        o->depth_limit = ( o->size > 0 ) ? o->depth_limit + 1 : 4;
        o->size        = ( o->size > 0 ) ? o->size * 2        : 8;
        if( o->size > o->size_limit ) ERR( "size limit (%zu) exceeded", o->size_limit );
        o->data = bcore_u_alloc( sizeof( bcore_hnode_u2vd_s ), NULL, o->size, &o->space );
        bcore_memzero( o->data, sizeof( bcore_hnode_u2vd_s ) * o->size );
        for( uz_t i = 0; i < buf_size; i++ )
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
        uz_t idx = u2vd_find( o, key );
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
        bcore_hnode_u2vd_s* p_node = u2vd_set( o, node, 1 );
        if( p_node )
        {
            bcore_hnode_u2vd_s_set_func( p_node, func );
            return;
        }
    }

    // rehash
    {
        bcore_hnode_u2vd_s* buf_data  = o->data;
        uz_t buf_size  = o->size;
        uz_t buf_space = o->space;
        o->depth_limit = ( o->size > 0 ) ? o->depth_limit + 1 : 4;
        o->size        = ( o->size > 0 ) ? o->size * 2        : 8;
        if( o->size > o->size_limit ) ERR( "size limit (%zu) exceeded", o->size_limit );
        o->data = bcore_u_alloc( sizeof( bcore_hnode_u2vd_s ), NULL, o->size, &o->space );
        bcore_memzero( o->data, sizeof( bcore_hnode_u2vd_s ) * o->size );
        for( uz_t i = 0; i < buf_size; i++ )
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

    uz_t idx = u2vd_find( o, key );
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

    uz_t idx = u2vd_find( o, key );
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
    uz_t idx = u2vd_find( o, key );
    if( idx < o->size )
    {
        bcore_hnode_u2vd_s* node = &o->data[ idx ];
        if( node->flag_holds && node->dp ) bcore_inst_a_discard( node->dp );
        node->key = 0;
        node->dp = 0;
        node->flag_holds = 0;
    }
    return;
}

vd_t bcore_hmap_u2vd_s_detach_h( bcore_hmap_u2vd_s* o, u2_t key )
{
    if( !key ) return NULL;

    uz_t idx = u2vd_find( o, key );
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
    uz_t idx = u2vd_find( o, key );
    return ( idx < o->size );
}

void bcore_hmap_u2vd_s_clear( bcore_hmap_u2vd_s* o )
{
    u2vd_set_size( o, 0 );
}

uz_t bcore_hmap_u2vd_s_keys( const bcore_hmap_u2vd_s* o )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->data[ i ].key > 0 );
    return count;
}

uz_t bcore_hmap_u2vd_s_size( const bcore_hmap_u2vd_s* o )
{
    return o->size;
}

u2_t bcore_hmap_u2vd_s_idx_key( const bcore_hmap_u2vd_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->data[ idx ].key;
}

bool bcore_hmap_u2vd_s_idx_holds( const bcore_hmap_u2vd_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->data[ idx ].flag_holds;
}

vd_t bcore_hmap_u2vd_s_idx_val( const bcore_hmap_u2vd_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->data[ idx ].dp;
}

void bcore_hmap_u2vd_s_run_c( const bcore_hmap_u2vd_s* o, vd_t obj, void (*fp)( vd_t obj, u2_t key, vd_t val ) )
{
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->data[ i ].key > 0 ) fp( obj, o->data[ i ].key, o->data[ i ].dp );
    }
}

void bcore_hmap_u2vd_s_run_d( bcore_hmap_u2vd_s* o, vd_t obj, void (*fp)( vd_t obj, u2_t key, vd_t* val ) )
{
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->data[ i ].key > 0 ) fp( obj, o->data[ i ].key, &o->data[ i ].dp );
    }
}

/**********************************************************************************************************************/

static bcore_self_s* hmap_u2vd_s_create_self( void )
{
    sc_t def = "\
        bcore_hmap_u2vd_s = bcore_inst \
            { \
                aware_t _; \
                private bcore_array_dyn_solid_static_s arr; \
                uz_t depth_limit; \
                uz_t size_limit; \
                private fp_t h1; \
                private fp_t h2; \
                private fp_t h3; \
            }";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_hmap_u2vd_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_u2vd_s_init,    "bcore_fp_init",    "init"    );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_u2vd_s_down,    "bcore_fp_down",    "down"    );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_u2vd_s_copy,    "bcore_fp_copy",    "copy"    );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_u2vd_s_create,  "bcore_fp_create",  "create"  );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_u2vd_s_clone,   "bcore_fp_clone",   "clone"   );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_u2vd_s_discard, "bcore_fp_discard", "discard" );
    return self;
}

/**********************************************************************************************************************/

static st_s* hmap_u2vd_s_status( bcore_hmap_u2vd_s* o )
{
    st_s* string = st_s_create();
    uz_t keys = bcore_hmap_u2vd_s_keys( o );
    st_s_pushf( string, "keys ........... %zu\n", keys );
    st_s_pushf( string, "nodes .......... %lu\n", o->size );
    st_s_pushf( string, "keys/nodes ..... %5.4f\n", o->size > 0 ? ( f3_t )( keys ) / o->size : 0 );
    return string;
}

void bcore_hmap_u2vd_filltest()
{
    bcore_life_s* life = bcore_life_s_create();
    bcore_hmap_u2vd_s* map = bcore_life_s_push( life, ( bcore_fp_discard )bcore_hmap_u2vd_s_discard, bcore_hmap_u2vd_s_create() );

    uz_t cycles = 2;
    uz_t size = 65536 * 64;

    for( uz_t i = 0; i < cycles; i++ )
    {
        bcore_hmap_u2vd_s_clear( map );
        u2_t key = bcore_xsg3_u2( i + 1 );
        for( uz_t j = 0; j < size; j++ )
        {
            key = bcore_xsg_u2( key );
            uz_t size1 = map->size;
            bcore_hmap_u2vd_s_set( map, key, 0, false );
            uz_t size2 = map->size;
            if( size2 > size1 )
            {
                st_s_print_d( st_s_createf( "%zu/%zu (%f)\n", j, size1, size1 > 0 ? ( f3_t ) j / size1 : 0.0 ) );
            }
        }
        st_s_print_d( hmap_u2vd_s_status( map ) );
    }

    bcore_life_s_discard( life );
}

/**********************************************************************************************************************/

static st_s* hmap_u2vd_selftest( void )
{
    st_s* log = st_s_createf( "== bcore_hmap_u2vd_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    bcore_hmap_u2vd_s* map = bcore_hmap_u2vd_s_create();
    const uz_t cycles = 200000;

    typedef struct
    {
        u2_t key;
        vd_t val;
    } kv_s;

    kv_s* kvbuf = bcore_alloc( NULL, cycles * sizeof( kv_s ) );
    uz_t kvbuf_size = 0;

    clock_t time = clock();
    st_s_pushf( log, "Mixed access: " );

    {
        u3_t rval1 = 1;
        u3_t rval2 = 12345;
        for( uz_t i = 0; i < cycles; i++ )
        {
            rval1 = bcore_xsg_u2( rval1 );
            rval2 = bcore_xsg_u2( rval2 );
            kv_s kv;
            kv.key = ( u2_t )rval1;
            kv.val = ( vd_t )(intptr_t)rval2;
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
                uz_t idx = rval1 % kvbuf_size;
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
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_u2vd_s_status( map ) );

    time = clock();
    st_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    uz_t read_cycles = 20;
    for( uz_t j = 0; j < read_cycles; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            if( *bcore_hmap_u2vd_s_get( map, kvbuf[ i ].key ) != kvbuf[ i ].val ) ERR( "value mismatch (%lu vs %lu)", kvbuf[ i ].key, kvbuf[ i ].val );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%gs per access)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * read_cycles ) );

    st_s_pushf( log, "\n" );
    st_s_pushf( log, "Removal: " );
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
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_u2vd_s_status( map ) );

    bcore_hmap_u2vd_s_discard( map );
    bcore_alloc( kvbuf, 0 );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/

/**********************************************************************************************************************/
// bcore_hmap_tpsz_s

static bcore_self_s* hnode_tpsz_s_create_self( void )
{
    sc_t def =
    "bcore_hnode_tpsz_s = bcore_inst"
    "{"
        "tp_t key;"
        "uz_t val;"
    "}";
    return bcore_self_s_build_parse_sc( def, sizeof( bcore_hnode_tpsz_s ) );
}

void bcore_hmap_tpsz_s_init( bcore_hmap_tpsz_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_hmap_tpsz_s;
    o->size_limit = 0xFFFFFFFFu;
}

void bcore_hmap_tpsz_s_down( bcore_hmap_tpsz_s* o )
{
    o->nodes = bcore_u_alloc( sizeof( bcore_hnode_tpsz_s ), o->nodes, 0, NULL );
    o->flags = bcore_u_alloc( sizeof( bl_t ),               o->flags, 0, NULL );
    o->size  = 0;
}

void bcore_hmap_tpsz_s_copy( bcore_hmap_tpsz_s* o, const bcore_hmap_tpsz_s* src )
{
    o->nodes = bcore_u_alloc( sizeof( bcore_hnode_tpsz_s ), o->nodes, 0, NULL );
    o->flags = bcore_u_alloc( sizeof( bl_t ),               o->flags, 0, NULL );
    o->size  = 0;

    o->nodes = bcore_u_memcpy( sizeof( bcore_hnode_tpsz_s ), o->nodes, src->nodes, src->size );
    o->size  = src->size;

    o->depth_limit = src->depth_limit;
    o->size_limit  = src->size_limit;
}

BCORE_DEFINE_FUNCTION_CREATE(  bcore_hmap_tpsz_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_hmap_tpsz_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_hmap_tpsz_s )

/**********************************************************************************************************************/

static uz_t tpsz_find( const bcore_hmap_tpsz_s* o, tp_t key ) // returns valid index or o->size
{
    if( o->size == 0 ) return o->size;
    u2_t mask = o->size - 1;
    uz_t idx = hash_tpu2_1( key ) & mask;
    if( o->nodes[ idx ].key == key ) return idx;
    idx = hash_tpu2_2( key ) & mask;
    if( o->nodes[ idx ].key == key ) return idx;
    return o->size;
}

uz_t tpsz_set( bcore_hmap_tpsz_s* o, bcore_hnode_tpsz_s node, uz_t depth ) // sets new node, returns valid index on success; o->size otherwise
{
    uz_t size = o->size;
    if( size == 0 ) return size;
    u2_t mask = o->size - 1;

    if( depth == o->depth_limit ) return size;

    uz_t idx1 = hash_tpu2_1( node.key ) & mask;
    if( !o->nodes[ idx1 ].key )
    {
        o->nodes[ idx1 ] = node;
        return idx1;
    }

    uz_t idx2 = hash_tpu2_2( node.key ) & mask;
    if( !o->nodes[ idx2 ].key )
    {
        o->nodes[ idx2 ] = node;
        return idx2;
    }

    if( !o->flags ) o->flags = bcore_u_memzero( sizeof( bl_t ), NULL, size );

    if( !o->flags[ idx1 ] )
    {
        o->flags[ idx1 ] = true;
        if( tpsz_set( o, o->nodes[ idx1 ], depth + 1 ) < size )
        {
            o->flags[ idx1 ] = false;
            o->nodes[ idx1 ] = node;
            return idx1;
        }
        o->flags[ idx1 ] = false;
    }

    if( !o->flags[ idx2 ] )
    {
        o->flags[ idx2 ] = true;
        if( tpsz_set( o, o->nodes[ idx2 ], depth + 1 ) < size )
        {
            o->flags[ idx2 ] = false;
            o->nodes[ idx2 ] = node;
            return idx2;
        }
        o->flags[ idx2 ] = false;
    }

    return size;
}

uz_t* bcore_hmap_tpsz_s_get( const bcore_hmap_tpsz_s* o, tp_t key )
{
    if( !key ) return NULL;
    uz_t idx = tpsz_find( o, key );
    return ( idx < o->size ) ? &o->nodes[ idx ].val : NULL;
}

uz_t* bcore_hmap_tpsz_s_fget( bcore_hmap_tpsz_s* o, tp_t key, uz_t init_val )
{
    if( !key ) ERR( "key is 0" );
    uz_t idx = tpsz_find( o, key );
    if( idx < o->size )
    {
        return &o->nodes[ idx ].val;
    }
    else
    {
        return bcore_hmap_tpsz_s_set( o, key, init_val );
    }
}

uz_t* bcore_hmap_tpsz_s_set( bcore_hmap_tpsz_s* o, tp_t key, uz_t val )
{
    if( !key ) ERR( "key is zero" );

    uz_t idx = tpsz_find( o, key );
    if( idx < o->size )
    {
        o->nodes[ idx ].val = val;
        return &o->nodes[ idx ].val;
    }

    idx = tpsz_set( o, ( bcore_hnode_tpsz_s ){ .key = key, .val = val }, 1 );
    if( idx < o->size ) return &o->nodes[ idx ].val;

    // rehash
    {
        o->flags = bcore_u_alloc( sizeof( bl_t ), o->flags, 0, NULL );
        bcore_hnode_tpsz_s* buf_nodes = o->nodes;
        uz_t  buf_size = o->size;
        o->depth_limit = ( o->size > 0 ) ? o->depth_limit + 1 : 4;
        o->size        = ( o->size > 0 ) ? o->size * 2        : 8;
        if( o->size > o->size_limit ) ERR( "size limit (%zu) exceeded", o->size_limit );
        o->nodes = bcore_u_memzero( sizeof( bcore_hnode_tpsz_s ), NULL, o->size );
        for( uz_t i = 0; i < buf_size; i++ )
        {
            if( buf_nodes[ i ].key ) bcore_hmap_tpsz_s_set( o, buf_nodes[ i ].key, buf_nodes[ i ].val );
        }
        bcore_alloc( buf_nodes, 0 );
    }

    return bcore_hmap_tpsz_s_set( o, key, val );
}

uz_t bcore_hmap_tpsz_s_remove( bcore_hmap_tpsz_s* o, tp_t key )
{
    if( !key ) return 0;
    uz_t idx = tpsz_find( o, key );
    if( idx < o->size )
    {
        uz_t val = o->nodes[ idx ].val;
        o->nodes[ idx ].key = 0;
        o->nodes[ idx ].val = 0;
        return val;
    }
    return 0;
}

bl_t bcore_hmap_tpsz_s_exists( const bcore_hmap_tpsz_s* o, tp_t key )
{
    if( !key ) return false;
    uz_t idx = tpsz_find( o, key );
    return ( idx < o->size );
}

void bcore_hmap_tpsz_s_clear( bcore_hmap_tpsz_s* o )
{
    o->nodes = bcore_alloc( o->nodes, 0 );
    o->flags = bcore_alloc( o->flags, 0 );
    o->size  = 0;
}

uz_t bcore_hmap_tpsz_s_keys( const bcore_hmap_tpsz_s* o )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->nodes[ i ].key > 0 );
    return count;
}

uz_t bcore_hmap_tpsz_s_size( const bcore_hmap_tpsz_s* o )
{
    return o->size;
}

tp_t bcore_hmap_tpsz_s_idx_key( const bcore_hmap_tpsz_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->nodes[ idx ].key;
}

uz_t bcore_hmap_tpsz_s_idx_val( const bcore_hmap_tpsz_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->nodes[ idx ].val;
}

static sr_s tpsz_s_get_data( const bcore_hmap_tpsz_s* o )
{
    tp_t t_data = bcore_flect_type_parse_sc( "{ bcore_hnode_tpsz_s []; }" );
    tp_t t_node = typeof( "bcore_hnode_tpsz_s" );
    sr_s data = sr_cp( sr_create( t_data ), TYPEOF_bcore_array_s );
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->nodes[ i ].key ) bcore_array_r_push( &data, sr_twc( t_node, &o->nodes[ i ] ) );
    }
    return data;
}

static void tpsz_s_set_data( bcore_hmap_tpsz_s* o, sr_s data )
{
    bcore_hmap_tpsz_s_clear( o );
    assert( sr_s_type( &data ) == bcore_flect_type_parse_sc( "{ bcore_hnode_tpsz_s []; }" ) );
    data = sr_cp( data, TYPEOF_bcore_array_s );
    uz_t size = bcore_array_r_get_size( &data );
    const bcore_hnode_tpsz_s* src = bcore_array_r_get_c_data( &data );
    for( uz_t i = 0; i < size; i++ ) bcore_hmap_tpsz_s_set( o, src[ i ].key, src[ i ].val );
    sr_down( data );
}

/**********************************************************************************************************************/

static bcore_self_s* hmap_tpsz_s_create_self( void )
{
    sc_t def = "\
        bcore_hmap_tpsz_s = bcore_inst \
            { \
                aware_t _; \
                private bcore_hnode_tpsz_s* nodes; \
                private bl_t* flags; \
                private uz_t size; \
                private uz_t depth_limit; \
                private uz_t size_limit; \
                shell { bcore_hnode_tpsz_s []; } data; } \
            }";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_hmap_tpsz_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tpsz_s_init,    "bcore_fp_init",  "init"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tpsz_s_down,    "bcore_fp_down",  "down"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tpsz_s_copy,    "bcore_fp_copy",  "copy"     );
    bcore_self_s_push_ns_func( self, ( fp_t )tpsz_s_get_data,           "bcore_fp_get",   "get_data" );
    bcore_self_s_push_ns_func( self, ( fp_t )tpsz_s_set_data,           "bcore_fp_set",   "set_data" );
    return self;
}

/**********************************************************************************************************************/

static st_s* hmap_tpsz_s_status( bcore_hmap_tpsz_s* o )
{
    st_s* string = st_s_create();
    uz_t keys = bcore_hmap_tpsz_s_keys( o );
    st_s_pushf( string, "keys ........... %zu\n", keys );
    st_s_pushf( string, "nodes .......... %lu\n", o->size );
    st_s_pushf( string, "keys/nodes ..... %5.4f\n", o->size > 0 ? ( f3_t )( keys ) / o->size : 0 );
    return string;
}

/**********************************************************************************************************************/

static st_s* hmap_tpsz_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    st_s* log = st_s_createf( "== bcore_hmap_tpsz_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    bcore_hmap_tpsz_s* map = bcore_hmap_tpsz_s_create();
    const uz_t cycles = 200000;

    typedef struct
    {
        tp_t key;
        uz_t val;
    } kv_s;

    kv_s* kvbuf = bcore_alloc( NULL, cycles * sizeof( kv_s ) );
    uz_t kvbuf_size = 0;

    clock_t time = clock();
    st_s_pushf( log, "Mixed access: " );

    {
        u3_t rval1 = 1;
        u3_t rval2 = 12345;
        for( uz_t i = 0; i < cycles; i++ )
        {
            rval1 = bcore_xsg_u2( rval1 );
            rval2 = bcore_xsg_u2( rval2 );
            kv_s kv;
            kv.key = ( u2_t )rval1;
            kv.val = ( uz_t )rval2;
            kvbuf[ kvbuf_size++ ] = kv;

            // set
            bcore_hmap_tpsz_s_set( map, kv.key, kv.val );

            // retrieve
            rval1 = bcore_xsg_u2( rval1 );
            kv = kvbuf[ rval1 % kvbuf_size ];
            uz_t* val_ptr = bcore_hmap_tpsz_s_get( map, kv.key );
            if( kv.val != *val_ptr ) ERR( "value mismatch (%lu vs %lu)", kv.val, *val_ptr );

            // delete
            rval1 = bcore_xsg_u2( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_xsg_u2( rval1 );
                uz_t idx = rval1 % kvbuf_size;
                kv_s kv = kvbuf[ idx ];
                if( !bcore_hmap_tpsz_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
                bcore_hmap_tpsz_s_remove( map, kv.key );
                if( bcore_hmap_tpsz_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_tpsz_s_status( map ) );

    time = clock();
    bcore_hmap_tpsz_s* map2 = bcore_life_s_push_aware( l, bcore_hmap_tpsz_s_clone( map ) );
    time = clock() - time;
    st_s_pushf( log, "Clone .......... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    ASSERT( bcore_compare_aware( map, map2 ) == 0 );
    time = clock() - time;
    st_s_pushf( log, "Comparison ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    bcore_hmap_tpsz_s* map3 = bcore_life_s_push_aware( l, bcore_hmap_tpsz_s_create() );
    bcore_via_a_nset( (bcore_via*)map3, typeof( "data" ), bcore_via_a_nget( (bcore_via*)map, typeof( "data" ) ) );
    time = clock() - time;
    st_s_pushf( log, "Via-assign ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );
    time = clock();
    for( uz_t i = 0; i < map->size; i++ )
    {
        tp_t key = bcore_hmap_tpsz_s_idx_key( map, i );
        if( key != 0 )
        {
            uz_t val = bcore_hmap_tpsz_s_idx_val( map, i );
            ASSERT( bcore_hmap_tpsz_s_exists( map3, key ) );
            ASSERT( val == *bcore_hmap_tpsz_s_get( map3, key ) );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "verify assign .. %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    st_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    uz_t read_cycles = 20;
    for( uz_t j = 0; j < read_cycles; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            if( *bcore_hmap_tpsz_s_get( map, kvbuf[ i ].key ) != kvbuf[ i ].val ) ERR( "value mismatch (%lu vs %lu)", kvbuf[ i ].key, kvbuf[ i ].val );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%gs per access)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * read_cycles ) );

    st_s_pushf( log, "\n" );
    st_s_pushf( log, "Removal: " );
    time = clock();
    while( kvbuf_size )
    {
        kvbuf_size--;
        kv_s kv = kvbuf[ kvbuf_size ];
        if( !bcore_hmap_tpsz_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
        bcore_hmap_tpsz_s_remove( map, kv.key );
        if( bcore_hmap_tpsz_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_tpsz_s_status( map ) );

    bcore_hmap_tpsz_s_discard( map );
    bcore_alloc( kvbuf, 0 );

    bcore_life_s_discard( l );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/

/**********************************************************************************************************************/
// bcore_hmap_tpfp_s

static bcore_self_s* hnode_tpfp_s_create_self( void )
{
    sc_t def =
    "bcore_hnode_tpfp_s = bcore_inst"
    "{"
        "tp_t key;"
        "fp_t val;"
    "}";
    return bcore_self_s_build_parse_sc( def, sizeof( bcore_hnode_tpfp_s ) );
}

void bcore_hmap_tpfp_s_init( bcore_hmap_tpfp_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_hmap_tpfp_s;
    o->size_limit = 0xFFFFFFFFu;
}

void bcore_hmap_tpfp_s_down( bcore_hmap_tpfp_s* o )
{
    o->nodes = bcore_u_alloc( sizeof( bcore_hnode_tpfp_s ), o->nodes, 0, NULL );
    o->flags = bcore_u_alloc( sizeof( bl_t ),               o->flags, 0, NULL );
    o->size  = 0;
}

void bcore_hmap_tpfp_s_copy( bcore_hmap_tpfp_s* o, const bcore_hmap_tpfp_s* src )
{
    o->nodes = bcore_u_alloc( sizeof( bcore_hnode_tpfp_s ), o->nodes, 0, NULL );
    o->flags = bcore_u_alloc( sizeof( bl_t ),               o->flags, 0, NULL );
    o->size  = 0;

    o->nodes = bcore_u_memcpy( sizeof( bcore_hnode_tpfp_s ), o->nodes, src->nodes, src->size );
    o->size  = src->size;

    o->depth_limit = src->depth_limit;
    o->size_limit  = src->size_limit;
}

BCORE_DEFINE_FUNCTION_CREATE(  bcore_hmap_tpfp_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_hmap_tpfp_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_hmap_tpfp_s )

/**********************************************************************************************************************/

static uz_t tpfp_find( const bcore_hmap_tpfp_s* o, tp_t key ) // returns valid index or o->size
{
    if( o->size == 0 ) return o->size;
    u2_t mask = o->size - 1;
    uz_t idx = hash_tpu2_1( key ) & mask;
    if( o->nodes[ idx ].key == key ) return idx;
    idx = hash_tpu2_2( key ) & mask;
    if( o->nodes[ idx ].key == key ) return idx;
    return o->size;
}

uz_t tpfp_set( bcore_hmap_tpfp_s* o, bcore_hnode_tpfp_s node, uz_t depth ) // sets new node, returns valid index on success; o->size otherwise
{
    uz_t size = o->size;
    if( size == 0 ) return size;
    u2_t mask = o->size - 1;

    if( depth == o->depth_limit ) return size;

    uz_t idx1 = hash_tpu2_1( node.key ) & mask;
    if( !o->nodes[ idx1 ].key )
    {
        o->nodes[ idx1 ] = node;
        return idx1;
    }

    uz_t idx2 = hash_tpu2_2( node.key ) & mask;
    if( !o->nodes[ idx2 ].key )
    {
        o->nodes[ idx2 ] = node;
        return idx2;
    }

    if( !o->flags ) o->flags = bcore_u_memzero( sizeof( bl_t ), NULL, size );

    if( !o->flags[ idx1 ] )
    {
        o->flags[ idx1 ] = true;
        if( tpfp_set( o, o->nodes[ idx1 ], depth + 1 ) < size )
        {
            o->flags[ idx1 ] = false;
            o->nodes[ idx1 ] = node;
            return idx1;
        }
        o->flags[ idx1 ] = false;
    }

    if( !o->flags[ idx2 ] )
    {
        o->flags[ idx2 ] = true;
        if( tpfp_set( o, o->nodes[ idx2 ], depth + 1 ) < size )
        {
            o->flags[ idx2 ] = false;
            o->nodes[ idx2 ] = node;
            return idx2;
        }
        o->flags[ idx2 ] = false;
    }

    return size;
}

fp_t* bcore_hmap_tpfp_s_get( const bcore_hmap_tpfp_s* o, tp_t key )
{
    if( !key ) return NULL;
    uz_t idx = tpfp_find( o, key );
    return ( idx < o->size ) ? &o->nodes[ idx ].val : NULL;
}

fp_t* bcore_hmap_tpfp_s_fget( bcore_hmap_tpfp_s* o, tp_t key, fp_t init_val )
{
    if( !key ) ERR( "key is 0" );
    uz_t idx = tpfp_find( o, key );
    if( idx < o->size )
    {
        return &o->nodes[ idx ].val;
    }
    else
    {
        return bcore_hmap_tpfp_s_set( o, key, init_val );
    }
}

fp_t* bcore_hmap_tpfp_s_set( bcore_hmap_tpfp_s* o, tp_t key, fp_t val )
{
    if( !key ) ERR( "key is zero" );

    uz_t idx = tpfp_find( o, key );
    if( idx < o->size )
    {
        o->nodes[ idx ].val = val;
        return &o->nodes[ idx ].val;
    }

    idx = tpfp_set( o, ( bcore_hnode_tpfp_s ){ .key = key, .val = val }, 1 );
    if( idx < o->size ) return &o->nodes[ idx ].val;

    // rehash
    {
        o->flags = bcore_u_alloc( sizeof( bl_t ), o->flags, 0, NULL );
        bcore_hnode_tpfp_s* buf_nodes = o->nodes;
        uz_t  buf_size = o->size;
        o->depth_limit = ( o->size > 0 ) ? o->depth_limit + 1 : 4;
        o->size        = ( o->size > 0 ) ? o->size * 2        : 8;
        if( o->size > o->size_limit ) ERR( "size limit (%zu) exceeded", o->size_limit );
        o->nodes = bcore_u_memzero( sizeof( bcore_hnode_tpfp_s ), NULL, o->size );
        for( uz_t i = 0; i < buf_size; i++ )
        {
            if( buf_nodes[ i ].key ) bcore_hmap_tpfp_s_set( o, buf_nodes[ i ].key, buf_nodes[ i ].val );
        }
        bcore_alloc( buf_nodes, 0 );
    }

    return bcore_hmap_tpfp_s_set( o, key, val );
}

fp_t bcore_hmap_tpfp_s_remove( bcore_hmap_tpfp_s* o, tp_t key )
{
    if( !key ) return 0;
    uz_t idx = tpfp_find( o, key );
    if( idx < o->size )
    {
        fp_t val = o->nodes[ idx ].val;
        o->nodes[ idx ].key = 0;
        o->nodes[ idx ].val = NULL;
        return val;
    }
    return 0;
}

bl_t bcore_hmap_tpfp_s_exists( const bcore_hmap_tpfp_s* o, tp_t key )
{
    if( !key ) return false;
    uz_t idx = tpfp_find( o, key );
    return ( idx < o->size );
}

void bcore_hmap_tpfp_s_clear( bcore_hmap_tpfp_s* o )
{
    o->nodes = bcore_alloc( o->nodes, 0 );
    o->flags = bcore_alloc( o->flags, 0 );
    o->size  = 0;
}

uz_t bcore_hmap_tpfp_s_keys( const bcore_hmap_tpfp_s* o )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->nodes[ i ].key > 0 );
    return count;
}

uz_t bcore_hmap_tpfp_s_size( const bcore_hmap_tpfp_s* o )
{
    return o->size;
}

tp_t bcore_hmap_tpfp_s_idx_key( const bcore_hmap_tpfp_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->nodes[ idx ].key;
}

fp_t bcore_hmap_tpfp_s_idx_val( const bcore_hmap_tpfp_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->nodes[ idx ].val;
}

static sr_s tpfp_s_get_data( const bcore_hmap_tpfp_s* o )
{
    tp_t t_data = bcore_flect_type_parse_sc( "{ bcore_hnode_tpfp_s []; }" );
    tp_t t_node = typeof( "bcore_hnode_tpfp_s" );
    sr_s data = sr_cp( sr_create( t_data ), TYPEOF_bcore_array_s );
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->nodes[ i ].key ) bcore_array_r_push( &data, sr_twc( t_node, &o->nodes[ i ] ) );
    }
    return data;
}

static void tpfp_s_set_data( bcore_hmap_tpfp_s* o, sr_s data )
{
    bcore_hmap_tpfp_s_clear( o );
    assert( sr_s_type( &data ) == bcore_flect_type_parse_sc( "{ bcore_hnode_tpfp_s []; }" ) );
    data = sr_cp( data, TYPEOF_bcore_array_s );
    uz_t size = bcore_array_r_get_size( &data );
    const bcore_hnode_tpfp_s* src = bcore_array_r_get_c_data( &data );
    for( uz_t i = 0; i < size; i++ ) bcore_hmap_tpfp_s_set( o, src[ i ].key, src[ i ].val );
    sr_down( data );
}

/**********************************************************************************************************************/

static bcore_self_s* hmap_tpfp_s_create_self( void )
{
    sc_t def = "\
        bcore_hmap_tpfp_s = bcore_inst \
            { \
                aware_t _; \
                private bcore_hnode_tpfp_s* nodes; \
                private bl_t* flags; \
                private uz_t size; \
                private uz_t depth_limit; \
                private uz_t size_limit; \
                shell { bcore_hnode_tpfp_s []; } data; } \
            }";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_hmap_tpfp_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tpfp_s_init,    "bcore_fp_init",  "init"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tpfp_s_down,    "bcore_fp_down",  "down"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tpfp_s_copy,    "bcore_fp_copy",  "copy"     );
    bcore_self_s_push_ns_func( self, ( fp_t )tpfp_s_get_data,           "bcore_fp_get",   "get_data" );
    bcore_self_s_push_ns_func( self, ( fp_t )tpfp_s_set_data,           "bcore_fp_set",   "set_data" );
    return self;
}

/**********************************************************************************************************************/

static st_s* hmap_tpfp_s_status( bcore_hmap_tpfp_s* o )
{
    st_s* string = st_s_create();
    uz_t keys = bcore_hmap_tpfp_s_keys( o );
    st_s_pushf( string, "keys ........... %zu\n", keys );
    st_s_pushf( string, "nodes .......... %lu\n", o->size );
    st_s_pushf( string, "keys/nodes ..... %5.4f\n", o->size > 0 ? ( f3_t )( keys ) / o->size : 0 );
    return string;
}

/**********************************************************************************************************************/

static st_s* hmap_tpfp_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    st_s* log = st_s_createf( "== bcore_hmap_tpfp_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    bcore_hmap_tpfp_s* map = bcore_hmap_tpfp_s_create();
    const uz_t cycles = 200000;

    typedef struct
    {
        tp_t key;
        fp_t val;
    } kv_s;

    kv_s* kvbuf = bcore_alloc( NULL, cycles * sizeof( kv_s ) );
    uz_t kvbuf_size = 0;

    clock_t time = clock();
    st_s_pushf( log, "Mixed access: " );

    {
        u3_t rval1 = 1;
        u3_t rval2 = 12345;
        for( uz_t i = 0; i < cycles; i++ )
        {
            rval1 = bcore_xsg_u2( rval1 );
            rval2 = bcore_xsg_u2( rval2 );
            kv_s kv;
            kv.key = ( u2_t )rval1;
            kv.val = ( fp_t )(intptr_t)rval2;
            kvbuf[ kvbuf_size++ ] = kv;

            // set
            bcore_hmap_tpfp_s_set( map, kv.key, kv.val );

            // retrieve
            rval1 = bcore_xsg_u2( rval1 );
            kv = kvbuf[ rval1 % kvbuf_size ];
            fp_t* val_ptr = bcore_hmap_tpfp_s_get( map, kv.key );
            if( kv.val != *val_ptr ) ERR( "value mismatch (%lu vs %lu)", kv.val, *val_ptr );

            // delete
            rval1 = bcore_xsg_u2( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_xsg_u2( rval1 );
                uz_t idx = rval1 % kvbuf_size;
                kv_s kv = kvbuf[ idx ];
                if( !bcore_hmap_tpfp_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
                bcore_hmap_tpfp_s_remove( map, kv.key );
                if( bcore_hmap_tpfp_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_tpfp_s_status( map ) );

    time = clock();
    bcore_life_s_push_aware( l, bcore_hmap_tpfp_s_clone( map ) );
    time = clock() - time;
    st_s_pushf( log, "Clone .......... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    bcore_hmap_tpfp_s* map3 = bcore_life_s_push_aware( l, bcore_hmap_tpfp_s_create() );
    bcore_via_a_nset( (bcore_via*)map3, typeof( "data" ), bcore_via_a_nget( (bcore_via*)map, typeof( "data" ) ) );
    time = clock() - time;
    st_s_pushf( log, "Via-assign ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );
    time = clock();
    for( uz_t i = 0; i < map->size; i++ )
    {
        tp_t key = bcore_hmap_tpfp_s_idx_key( map, i );
        if( key != 0 )
        {
            fp_t val = bcore_hmap_tpfp_s_idx_val( map, i );
            ASSERT( bcore_hmap_tpfp_s_exists( map3, key ) );
            ASSERT( val == *bcore_hmap_tpfp_s_get( map3, key ) );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "verify assign .. %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    st_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    uz_t read_cycles = 20;
    for( uz_t j = 0; j < read_cycles; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            if( *bcore_hmap_tpfp_s_get( map, kvbuf[ i ].key ) != kvbuf[ i ].val ) ERR( "value mismatch (%lu vs %lu)", kvbuf[ i ].key, kvbuf[ i ].val );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%gs per access)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * read_cycles ) );

    st_s_pushf( log, "\n" );
    st_s_pushf( log, "Removal: " );
    time = clock();
    while( kvbuf_size )
    {
        kvbuf_size--;
        kv_s kv = kvbuf[ kvbuf_size ];
        if( !bcore_hmap_tpfp_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
        bcore_hmap_tpfp_s_remove( map, kv.key );
        if( bcore_hmap_tpfp_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_tpfp_s_status( map ) );

    bcore_hmap_tpfp_s_discard( map );
    bcore_alloc( kvbuf, 0 );

    bcore_life_s_discard( l );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/

/**********************************************************************************************************************/
// bcore_hmap_tptp_s

static bcore_self_s* hnode_tptp_s_create_self( void )
{
    sc_t def =
    "bcore_hnode_tptp_s = bcore_inst"
    "{"
        "tp_t key;"
        "tp_t val;"
    "}";
    return bcore_self_s_build_parse_sc( def, sizeof( bcore_hnode_tptp_s ) );
}

void bcore_hmap_tptp_s_init( bcore_hmap_tptp_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_hmap_tptp_s;
    o->size_limit = 0xFFFFFFFFu;
}

void bcore_hmap_tptp_s_down( bcore_hmap_tptp_s* o )
{
    o->nodes = bcore_alloc( o->nodes, 0 );
    o->flags = bcore_alloc( o->flags, 0 );
    o->size  = 0;
}

void bcore_hmap_tptp_s_copy( bcore_hmap_tptp_s* o, const bcore_hmap_tptp_s* src )
{
    o->nodes = bcore_alloc( o->nodes, 0 );
    o->flags = bcore_alloc( o->flags, 0 );
    o->size  = 0;
    o->nodes = bcore_u_memcpy( sizeof( bcore_hnode_tptp_s ), o->nodes, src->nodes, src->size );
    o->size  = src->size;
    o->depth_limit = src->depth_limit;
    o->size_limit  = src->size_limit;
}

BCORE_DEFINE_FUNCTION_CREATE(  bcore_hmap_tptp_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_hmap_tptp_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_hmap_tptp_s )

/**********************************************************************************************************************/

static uz_t tptp_find( const bcore_hmap_tptp_s* o, tp_t key ) // returns valid index or o->size
{
    if( o->size == 0 ) return o->size;
    u2_t mask = o->size - 1;
    uz_t idx = hash_tpu2_1( key ) & mask;
    if( o->nodes[ idx ].key == key ) return idx;
    idx = hash_tpu2_2( key ) & mask;
    if( o->nodes[ idx ].key == key ) return idx;
    return o->size;
}

uz_t tptp_set( bcore_hmap_tptp_s* o, bcore_hnode_tptp_s node, uz_t depth ) // sets new node, returns valid index on success; o->size otherwise
{
    uz_t size = o->size;
    if( size == 0 ) return size;
    u2_t mask = o->size - 1;

    if( depth == o->depth_limit ) return size;

    uz_t idx1 = hash_tpu2_1( node.key ) & mask;
    if( !o->nodes[ idx1 ].key )
    {
        o->nodes[ idx1 ] = node;
        return idx1;
    }

    uz_t idx2 = hash_tpu2_2( node.key ) & mask;
    if( !o->nodes[ idx2 ].key )
    {
        o->nodes[ idx2 ] = node;
        return idx2;
    }

    if( !o->flags ) o->flags = bcore_u_memzero( sizeof( bl_t ), NULL, size );

    if( !o->flags[ idx1 ] )
    {
        o->flags[ idx1 ] = true;
        if( tptp_set( o, o->nodes[ idx1 ], depth + 1 ) < size )
        {
            o->flags[ idx1 ] = false;
            o->nodes[ idx1 ] = node;
            return idx1;
        }
        o->flags[ idx1 ] = false;
    }

    if( !o->flags[ idx2 ] )
    {
        o->flags[ idx2 ] = true;
        if( tptp_set( o, o->nodes[ idx2 ], depth + 1 ) < size )
        {
            o->flags[ idx2 ] = false;
            o->nodes[ idx2 ] = node;
            return idx2;
        }
        o->flags[ idx2 ] = false;
    }

    return size;
}

tp_t* bcore_hmap_tptp_s_get( const bcore_hmap_tptp_s* o, tp_t key )
{
    if( !key ) return NULL;
    uz_t idx = tptp_find( o, key );
    return ( idx < o->size ) ? &o->nodes[ idx ].val : NULL;
}

tp_t* bcore_hmap_tptp_s_fget( bcore_hmap_tptp_s* o, tp_t key, tp_t init_val )
{
    if( !key ) ERR( "key is 0" );
    uz_t idx = tptp_find( o, key );
    if( idx < o->size )
    {
        return &o->nodes[ idx ].val;
    }
    else
    {
        return bcore_hmap_tptp_s_set( o, key, init_val );
    }
}

tp_t* bcore_hmap_tptp_s_set( bcore_hmap_tptp_s* o, tp_t key, tp_t val )
{
    if( !key ) ERR( "key is zero" );

    uz_t idx = tptp_find( o, key );
    if( idx < o->size )
    {
        o->nodes[ idx ].val = val;
        return &o->nodes[ idx ].val;
    }

    idx = tptp_set( o, ( bcore_hnode_tptp_s ){ .key = key, .val = val }, 1 );
    if( idx < o->size ) return &o->nodes[ idx ].val;

    // rehash
    {
        o->flags = bcore_u_alloc( sizeof( bl_t ), o->flags, 0, NULL );
        bcore_hnode_tptp_s* buf_nodes = o->nodes;
        uz_t  buf_size = o->size;
        o->depth_limit = ( o->size > 0 ) ? o->depth_limit + 1 : 4;
        o->size        = ( o->size > 0 ) ? o->size * 2        : 8;
        if( o->size > o->size_limit ) ERR( "size limit (%zu) exceeded", o->size_limit );
        o->nodes = bcore_u_memzero( sizeof( bcore_hnode_tptp_s ), NULL, o->size );
        for( uz_t i = 0; i < buf_size; i++ )
        {
            if( buf_nodes[ i ].key ) bcore_hmap_tptp_s_set( o, buf_nodes[ i ].key, buf_nodes[ i ].val );
        }
        bcore_alloc( buf_nodes, 0 );
    }

    return bcore_hmap_tptp_s_set( o, key, val );
}

tp_t bcore_hmap_tptp_s_remove( bcore_hmap_tptp_s* o, tp_t key )
{
    if( !key ) return 0;
    uz_t idx = tptp_find( o, key );
    if( idx < o->size )
    {
        tp_t val = o->nodes[ idx ].val;
        o->nodes[ idx ].key = 0;
        o->nodes[ idx ].val = 0;
        return val;
    }
    return 0;
}

bl_t bcore_hmap_tptp_s_exists( const bcore_hmap_tptp_s* o, tp_t key )
{
    if( !key ) return false;
    uz_t idx = tptp_find( o, key );
    return ( idx < o->size );
}

void bcore_hmap_tptp_s_clear( bcore_hmap_tptp_s* o )
{
    o->nodes = bcore_alloc( o->nodes, 0 );
    o->flags = bcore_alloc( o->flags, 0 );
    o->size  = 0;
}

uz_t bcore_hmap_tptp_s_keys( const bcore_hmap_tptp_s* o )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->nodes[ i ].key > 0 );
    return count;
}

uz_t bcore_hmap_tptp_s_size( const bcore_hmap_tptp_s* o )
{
    return o->size;
}

tp_t bcore_hmap_tptp_s_idx_key( const bcore_hmap_tptp_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->nodes[ idx ].key;
}

tp_t bcore_hmap_tptp_s_idx_val( const bcore_hmap_tptp_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->nodes[ idx ].val;
}

static sr_s tptp_s_get_data( const bcore_hmap_tptp_s* o )
{
    tp_t t_data = bcore_flect_type_parse_sc( "{ bcore_hnode_tptp_s []; }" );
    tp_t t_node = typeof( "bcore_hnode_tptp_s" );
    sr_s data = sr_cp( sr_create( t_data ), TYPEOF_bcore_array_s );
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->nodes[ i ].key ) bcore_array_r_push( &data, sr_twc( t_node, &o->nodes[ i ] ) );
    }
    return data;
}

static void tptp_s_set_data( bcore_hmap_tptp_s* o, sr_s data )
{
    bcore_hmap_tptp_s_clear( o );
    assert( sr_s_type( &data ) == bcore_flect_type_parse_sc( "{ bcore_hnode_tptp_s []; }" ) );
    data = sr_cp( data, TYPEOF_bcore_array_s );
    uz_t size = bcore_array_r_get_size( &data );
    const bcore_hnode_tptp_s* src = bcore_array_r_get_c_data( &data );
    for( uz_t i = 0; i < size; i++ ) bcore_hmap_tptp_s_set( o, src[ i ].key, src[ i ].val );
    sr_down( data );
}

/**********************************************************************************************************************/

static bcore_self_s* hmap_tptp_s_create_self( void )
{
    sc_t def = "\
        bcore_hmap_tptp_s = bcore_inst \
            { \
                aware_t _; \
                private bcore_hnode_tptp_s* nodes; \
                private bl_t* flags; \
                private uz_t size; \
                private uz_t depth_limit; \
                private uz_t size_limit; \
                shell { bcore_hnode_tptp_s []; } data; } \
            }";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_hmap_tptp_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tptp_s_init,    "bcore_fp_init",  "init"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tptp_s_down,    "bcore_fp_down",  "down"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tptp_s_copy,    "bcore_fp_copy",  "copy"     );
    bcore_self_s_push_ns_func( self, ( fp_t )tptp_s_get_data,           "bcore_fp_get",   "get_data" );
    bcore_self_s_push_ns_func( self, ( fp_t )tptp_s_set_data,           "bcore_fp_set",   "set_data" );
    return self;
}

/**********************************************************************************************************************/

static st_s* hmap_tptp_s_status( bcore_hmap_tptp_s* o )
{
    st_s* string = st_s_create();
    uz_t keys = bcore_hmap_tptp_s_keys( o );
    st_s_pushf( string, "keys ........... %zu\n", keys );
    st_s_pushf( string, "nodes .......... %lu\n", o->size );
    st_s_pushf( string, "keys/nodes ..... %5.4f\n", o->size > 0 ? ( f3_t )( keys ) / o->size : 0 );
    return string;
}

/**********************************************************************************************************************/

static st_s* hmap_tptp_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    st_s* log = st_s_createf( "== bcore_hmap_tptp_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    bcore_hmap_tptp_s* map = bcore_hmap_tptp_s_create();
    const uz_t cycles = 200000;

    typedef struct
    {
        tp_t key;
        tp_t val;
    } kv_s;

    kv_s* kvbuf = bcore_alloc( NULL, cycles * sizeof( kv_s ) );
    uz_t kvbuf_size = 0;

    clock_t time = clock();
    st_s_pushf( log, "Mixed access: " );

    {
        u3_t rval1 = 1;
        u3_t rval2 = 12345;
        for( uz_t i = 0; i < cycles; i++ )
        {
            rval1 = bcore_xsg_u2( rval1 );
            rval2 = bcore_xsg_u2( rval2 );
            kv_s kv;
            kv.key = ( tp_t )rval1;
            kv.val = ( tp_t )rval2;
            kvbuf[ kvbuf_size++ ] = kv;

            // set
            bcore_hmap_tptp_s_set( map, kv.key, kv.val );

            // retrieve
            rval1 = bcore_xsg_u2( rval1 );
            kv = kvbuf[ rval1 % kvbuf_size ];
            tp_t* val_ptr = bcore_hmap_tptp_s_get( map, kv.key );
            if( kv.val != *val_ptr ) ERR( "value mismatch (%lu vs %lu)", kv.val, *val_ptr );

            // delete
            rval1 = bcore_xsg_u2( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_xsg_u2( rval1 );
                uz_t idx = rval1 % kvbuf_size;
                kv_s kv = kvbuf[ idx ];
                if( !bcore_hmap_tptp_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
                bcore_hmap_tptp_s_remove( map, kv.key );
                if( bcore_hmap_tptp_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_tptp_s_status( map ) );

    time = clock();
    bcore_hmap_tptp_s* map2 = bcore_life_s_push_aware( l, bcore_hmap_tptp_s_clone( map ) );
    time = clock() - time;
    st_s_pushf( log, "Clone .......... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    ASSERT( bcore_compare_aware( map, map2 ) == 0 );
    time = clock() - time;
    st_s_pushf( log, "Comparison ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    bcore_hmap_tptp_s* map3 = bcore_life_s_push_aware( l, bcore_hmap_tptp_s_create() );
    bcore_via_a_nset( (bcore_via*)map3, typeof( "data" ), bcore_via_a_nget( (bcore_via*)map, typeof( "data" ) ) );
    time = clock() - time;
    st_s_pushf( log, "Via-assign ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );
    time = clock();
    for( uz_t i = 0; i < map->size; i++ )
    {
        tp_t key = bcore_hmap_tptp_s_idx_key( map, i );
        if( key != 0 )
        {
            tp_t val = bcore_hmap_tptp_s_idx_val( map, i );
            ASSERT( bcore_hmap_tptp_s_exists( map3, key ) );
            ASSERT( val == *bcore_hmap_tptp_s_get( map3, key ) );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "verify assign .. %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    st_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    uz_t read_cycles = 20;
    for( uz_t j = 0; j < read_cycles; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            if( *bcore_hmap_tptp_s_get( map, kvbuf[ i ].key ) != kvbuf[ i ].val ) ERR( "value mismatch (%lu vs %lu)", kvbuf[ i ].key, kvbuf[ i ].val );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%gs per access)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * read_cycles ) );

    st_s_pushf( log, "\n" );
    st_s_pushf( log, "Removal: " );
    time = clock();
    while( kvbuf_size )
    {
        kvbuf_size--;
        kv_s kv = kvbuf[ kvbuf_size ];
        if( !bcore_hmap_tptp_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
        bcore_hmap_tptp_s_remove( map, kv.key );
        if( bcore_hmap_tptp_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_tptp_s_status( map ) );

    bcore_hmap_tptp_s_discard( map );
    bcore_alloc( kvbuf, 0 );

    bcore_life_s_discard( l );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/

/**********************************************************************************************************************/
// bcore_hmap_tpto_s

void bcore_hmap_tpto_s_init( bcore_hmap_tpto_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_hmap_tpto_s;
    o->size_limit = 0xFFFFFFFFu;
}

void bcore_hmap_tpto_s_down( bcore_hmap_tpto_s* o )
{
    if( o->size > 0 )
    {
        const bcore_inst_s* inst_p = NULL;
        for( uz_t i = 0; i < o->size; i++ )
        {
            if( o->nodes[ i ].val )
            {
                if( !inst_p ) inst_p = bcore_inst_s_get_typed( o->type );
                bcore_inst_p_discard( inst_p, o->nodes[ i ].val );
            }
        }
    }
    o->nodes = bcore_alloc( o->nodes, 0 );
    o->flags = bcore_alloc( o->flags, 0 );
    o->size  = 0;
}

void bcore_hmap_tpto_s_copy( bcore_hmap_tpto_s* o, const bcore_hmap_tpto_s* src )
{
    const bcore_inst_s* inst_p = o->type ? bcore_inst_s_get_typed( o->type ) : NULL;
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->nodes[ i ].val ) bcore_inst_p_discard( inst_p, o->nodes[ i ].val );
    }
    o->nodes = bcore_alloc( o->nodes, 0 );
    o->flags = bcore_alloc( o->flags, 0 );
    o->size  = 0;

    o->nodes = bcore_u_memzero( sizeof( bcore_hnode_tpto_s ), o->nodes, src->size );
    inst_p = src->type ? bcore_inst_s_get_typed( src->type ) : NULL;
    for( uz_t i = 0; i < src->size; i++ )
    {
        const bcore_hnode_tpto_s* src_node = &src->nodes[ i ];
              bcore_hnode_tpto_s* dst_node =   &o->nodes[ i ];

        dst_node->key = src_node->key;
        if( src_node->val )
        {
            dst_node->val = bcore_inst_p_clone( inst_p, src_node->val );
        }
    }

    o->type = src->type;
    o->size = src->size;
    o->depth_limit = src->depth_limit;
    o->size_limit  = src->size_limit;
}

BCORE_DEFINE_FUNCTION_CREATE(  bcore_hmap_tpto_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_hmap_tpto_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_hmap_tpto_s )

/**********************************************************************************************************************/

static uz_t tpto_find( const bcore_hmap_tpto_s* o, tp_t key ) // returns valid index or o->size
{
    if( o->size == 0 ) return o->size;
    u2_t mask = o->size - 1;
    uz_t idx = hash_tpu2_1( key ) & mask;
    if( o->nodes[ idx ].key == key ) return idx;
    idx = hash_tpu2_2( key ) & mask;
    if( o->nodes[ idx ].key == key ) return idx;
    return o->size;
}

uz_t tpto_set( bcore_hmap_tpto_s* o, bcore_hnode_tpto_s node, uz_t depth ) // sets new node, returns valid index on success; o->size otherwise
{
    uz_t size = o->size;
    if( size == 0 ) return size;
    u2_t mask = o->size - 1;

    if( depth == o->depth_limit ) return size;

    uz_t idx1 = hash_tpu2_1( node.key ) & mask;
    if( !o->nodes[ idx1 ].key )
    {
        o->nodes[ idx1 ] = node;
        return idx1;
    }

    uz_t idx2 = hash_tpu2_2( node.key ) & mask;
    if( !o->nodes[ idx2 ].key )
    {
        o->nodes[ idx2 ] = node;
        return idx2;
    }

    if( !o->flags ) o->flags = bcore_u_memzero( sizeof( bl_t ), NULL, size );

    if( !o->flags[ idx1 ] )
    {
        o->flags[ idx1 ] = true;
        if( tpto_set( o, o->nodes[ idx1 ], depth + 1 ) < size )
        {
            o->flags[ idx1 ] = false;
            o->nodes[ idx1 ] = node;
            return idx1;
        }
        o->flags[ idx1 ] = false;
    }

    if( !o->flags[ idx2 ] )
    {
        o->flags[ idx2 ] = true;
        if( tpto_set( o, o->nodes[ idx2 ], depth + 1 ) < size )
        {
            o->flags[ idx2 ] = false;
            o->nodes[ idx2 ] = node;
            return idx2;
        }
        o->flags[ idx2 ] = false;
    }

    return size;
}

void bcore_hmap_tpto_s_set_type( bcore_hmap_tpto_s* o, tp_t type )
{
    if( o->type == type ) return;
    bcore_hmap_tpto_s_clear( o );
    o->type = type;
}

vd_t* bcore_hmap_tpto_s_get( const bcore_hmap_tpto_s* o, tp_t key )
{
    if( !key ) return NULL;
    uz_t idx = tpto_find( o, key );
    return ( idx < o->size ) ? &o->nodes[ idx ].val : NULL;
}

vd_t* bcore_hmap_tpto_s_fget_d( bcore_hmap_tpto_s* o, tp_t key, vd_t init_val )
{
    if( !key ) ERR( "key is 0" );
    if( !o->type ) ERR( "type is 0" );
    uz_t idx = tpto_find( o, key );
    if( idx < o->size )
    {
        if( init_val ) bcore_inst_t_discard( o->type, init_val );
        return &o->nodes[ idx ].val;
    }
    else
    {
        return bcore_hmap_tpto_s_set_d( o, key, init_val );
    }
}

vd_t bcore_hmap_tpto_s_fget( bcore_hmap_tpto_s* o, tp_t key )
{
    vd_t* dst = bcore_hmap_tpto_s_fget_d( o, key, NULL );
    if( !*dst ) *dst = bcore_inst_t_create( o->type );
    return *dst;
}

vd_t* bcore_hmap_tpto_s_set_d( bcore_hmap_tpto_s* o, tp_t key, vd_t val )
{
    if( !key ) ERR( "key is zero" );
    if( !o->type ) ERR( "type is not defined" );

    uz_t idx = tpto_find( o, key );
    if( idx < o->size )
    {
        if( o->nodes[ idx ].val ) bcore_inst_t_discard( o->type, o->nodes[ idx ].val );
        o->nodes[ idx ].val = val;
        return &o->nodes[ idx ].val;
    }

    idx = tpto_set( o, ( bcore_hnode_tpto_s ){ .key = key, .val = val }, 1 );
    if( idx < o->size ) return &o->nodes[ idx ].val;

    // rehash
    {
        o->flags = bcore_u_alloc( sizeof( bl_t ), o->flags, 0, NULL );
        bcore_hnode_tpto_s* buf_nodes = o->nodes;
        uz_t  buf_size = o->size;
        o->depth_limit = ( o->size > 0 ) ? o->depth_limit + 1 : 4;
        o->size        = ( o->size > 0 ) ? o->size * 2        : 8;
        if( o->size > o->size_limit ) ERR( "size limit (%zu) exceeded", o->size_limit );
        o->nodes = bcore_u_memzero( sizeof( bcore_hnode_tpto_s ), NULL, o->size );
        for( uz_t i = 0; i < buf_size; i++ )
        {
            if( buf_nodes[ i ].key ) bcore_hmap_tpto_s_set_d( o, buf_nodes[ i ].key, buf_nodes[ i ].val );
        }
        bcore_alloc( buf_nodes, 0 ); // rehashing does not destroy any object (only rearranges nodes)
    }

    return bcore_hmap_tpto_s_set_d( o, key, val );
}

void bcore_hmap_tpto_s_remove( bcore_hmap_tpto_s* o, tp_t key )
{
    if( !key ) return;
    uz_t idx = tpto_find( o, key );
    if( idx < o->size )
    {
        if( o->nodes[ idx ].val ) bcore_inst_t_discard( o->type, o->nodes[ idx ].val );
        o->nodes[ idx ].key = 0;
        o->nodes[ idx ].val = 0;
    }
}

bl_t bcore_hmap_tpto_s_exists( const bcore_hmap_tpto_s* o, tp_t key )
{
    if( !key ) return false;
    uz_t idx = tpto_find( o, key );
    return ( idx < o->size );
}

void bcore_hmap_tpto_s_clear( bcore_hmap_tpto_s* o )
{
    const bcore_inst_s* inst_p = o->type ? bcore_inst_s_get_typed( o->type ) : NULL;
    for( uz_t i = 0; i < o->size; i++ ) if( o->nodes[ i ].val ) bcore_inst_p_discard( inst_p, o->nodes[ i ].val );
    o->nodes = bcore_alloc( o->nodes, 0 );
    o->flags = bcore_alloc( o->flags, 0 );
    o->size  = 0;
}

uz_t bcore_hmap_tpto_s_keys( const bcore_hmap_tpto_s* o )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->nodes[ i ].key > 0 );
    return count;
}

uz_t bcore_hmap_tpto_s_size( const bcore_hmap_tpto_s* o )
{
    return o->size;
}

tp_t bcore_hmap_tpto_s_idx_key( const bcore_hmap_tpto_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->nodes[ idx ].key;
}

vd_t bcore_hmap_tpto_s_idx_val( const bcore_hmap_tpto_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->nodes[ idx ].val;
}

static sr_s tpto_s_get_data( const bcore_hmap_tpto_s* o )
{
    tp_t t_data = bcore_flect_type_parse_sc( "{ tp_t type; { tp_t key; sr_s obj; } [] arr; }" );
    sr_s data = sr_create( t_data );
    bcore_via_r_nset( &data, typeof( "type" ), sr_twc( TYPEOF_tp_t, &o->type ) );
    struct { tp_t key; sr_s obj; } node;
    const bcore_inst_s* inst_p = o->type ? bcore_inst_s_get_typed( o->type ) : NULL;
    tp_t t_node = bcore_array_r_get_static_type( &data );

    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->nodes[ i ].key )
        {
            node.key = o->nodes[ i ].key;
            node.obj = sr_pwc( inst_p, o->nodes[ i ].val );
            bcore_array_r_push( &data, sr_twc( t_node, &node ) );
        }
    }

    return data;
}

static void tpto_s_set_data( bcore_hmap_tpto_s* o, sr_s data )
{
    bcore_hmap_tpto_s_clear( o );
    assert( sr_s_type( &data ) == bcore_flect_type_parse_sc( "{ tp_t type; { tp_t key; sr_s obj; } [] arr; }" ) );

    {
        sr_s type_sr = bcore_via_r_nget( &data, typeof( "type" ) );
        bcore_hmap_tpto_s_set_type( o, *( tp_t* )type_sr.o );
        sr_down( type_sr );
    }

    uz_t size = bcore_array_r_get_size( &data );
    if( sr_s_is_strong( &data ) )
    {
        struct { tp_t key; sr_s obj; }* src = bcore_array_r_get_d_data( &data );
        for( uz_t i = 0; i < size; i++ )
        {
            bcore_hmap_tpto_s_set_d( o, src[ i ].key, src[ i ].obj.o );
            src[ i ].obj = sr_null();
        }
    }
    else
    {
        const struct { tp_t key; sr_s obj; }* src = bcore_array_r_get_c_data( &data );
        for( uz_t i = 0; i < size; i++ )
        {
            bcore_hmap_tpto_s_set_d( o, src[ i ].key, bcore_inst_r_clone( &src[ i ].obj ) );
        }
    }
    sr_down( data );
}

/**********************************************************************************************************************/

static bcore_self_s* hmap_tpto_s_create_self( void )
{
    sc_t def = "\
        bcore_hmap_tpto_s = bcore_inst \
            { \
                aware_t _; \
                private tp_t type; \
                private vd_t* nodes; \
                private bl_t* flags; \
                private uz_t size; \
                private uz_t depth_limit; \
                private uz_t size_limit; \
                shell { tp_t type; { tp_t key; sr_s obj; } []; } data; } \
            }";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_hmap_tpto_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tpto_s_init,    "bcore_fp_init",  "init"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tpto_s_down,    "bcore_fp_down",  "down"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tpto_s_copy,    "bcore_fp_copy",  "copy"     );
    bcore_self_s_push_ns_func( self, ( fp_t )tpto_s_get_data,           "bcore_fp_get",   "get_data" );
    bcore_self_s_push_ns_func( self, ( fp_t )tpto_s_set_data,           "bcore_fp_set",   "set_data" );
    return self;
}

/**********************************************************************************************************************/

static st_s* hmap_tpto_s_status( bcore_hmap_tpto_s* o )
{
    st_s* string = st_s_create();
    uz_t keys = bcore_hmap_tpto_s_keys( o );
    st_s_pushf( string, "keys ........... %zu\n", keys );
    st_s_pushf( string, "nodes .......... %lu\n", o->size );
    st_s_pushf( string, "keys/nodes ..... %5.4f\n", o->size > 0 ? ( f3_t )( keys ) / o->size : 0 );
    return string;
}

/**********************************************************************************************************************/

static st_s* hmap_tpto_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    st_s* log = st_s_createf( "== bcore_hmap_tpto_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    bcore_hmap_tpto_s* map = bcore_hmap_tpto_s_create();
    bcore_hmap_tpto_s_set_type( map, TYPEOF_uz_t );
    const bcore_inst_s* val_p = bcore_inst_s_get_typed( TYPEOF_uz_t );

    const uz_t cycles = 200000;

    typedef struct
    {
        tp_t key;
        uz_t val;
    } kv_s;

    kv_s* kvbuf = bcore_alloc( NULL, cycles * sizeof( kv_s ) );
    uz_t kvbuf_size = 0;

    clock_t time = clock();
    st_s_pushf( log, "Mixed access: " );

    {
        u3_t rval1 = 1;
        u3_t rval2 = 12345;
        for( uz_t i = 0; i < cycles; i++ )
        {
            rval1 = bcore_xsg_u2( rval1 );
            rval2 = bcore_xsg_u2( rval2 );
            kv_s kv;
            kv.key = ( tp_t )rval1;
            kv.val = ( uz_t )rval2;
            kvbuf[ kvbuf_size++ ] = kv;

            // set
            bcore_hmap_tpto_s_set_d( map, kv.key, bcore_inst_p_clone( val_p, (bcore_inst*)&kv.val ) );

            // retrieve
            rval1 = bcore_xsg_u2( rval1 );
            kv = kvbuf[ rval1 % kvbuf_size ];
            uz_t* val_ptr = *bcore_hmap_tpto_s_get( map, kv.key );
            if( kv.val != *val_ptr ) ERR( "value mismatch (%lu vs %lu)", kv.val, *val_ptr );

            // delete
            rval1 = bcore_xsg_u2( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_xsg_u2( rval1 );
                uz_t idx = rval1 % kvbuf_size;
                kv_s kv = kvbuf[ idx ];
                if( !bcore_hmap_tpto_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
                bcore_hmap_tpto_s_remove( map, kv.key );
                if( bcore_hmap_tpto_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_tpto_s_status( map ) );

    time = clock();
    bcore_hmap_tpto_s* map2 = bcore_life_s_push_aware( l, bcore_hmap_tpto_s_clone( map ) );
    time = clock() - time;
    st_s_pushf( log, "Clone .......... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    ASSERT( bcore_compare_aware( map, map2 ) == 0 );
    time = clock() - time;
    st_s_pushf( log, "Comparison ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    bcore_hmap_tpto_s* map3 = bcore_life_s_push_aware( l, bcore_hmap_tpto_s_create() );
    bcore_via_a_nset( (bcore_via*)map3, typeof( "data" ), bcore_via_a_nget( (bcore_via*)map, typeof( "data" ) ) );
    time = clock() - time;
    st_s_pushf( log, "Via-assign ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );
    time = clock();
    for( uz_t i = 0; i < map->size; i++ )
    {
        tp_t key = bcore_hmap_tpto_s_idx_key( map, i );
        if( key != 0 )
        {
            uz_t val = *( uz_t* )bcore_hmap_tpto_s_idx_val( map, i );
            ASSERT( bcore_hmap_tpto_s_exists( map3, key ) );
            ASSERT( val == *( uz_t * )*bcore_hmap_tpto_s_get( map3, key ) );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "verify assign .. %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    st_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    uz_t read_cycles = 20;
    for( uz_t j = 0; j < read_cycles; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            if( *( uz_t*)*bcore_hmap_tpto_s_get( map, kvbuf[ i ].key ) != kvbuf[ i ].val ) ERR( "value mismatch" );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%gs per access)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * read_cycles ) );

    st_s_pushf( log, "\n" );
    st_s_pushf( log, "Removal: " );
    time = clock();
    while( kvbuf_size )
    {
        kvbuf_size--;
        kv_s kv = kvbuf[ kvbuf_size ];
        if( !bcore_hmap_tpto_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
        bcore_hmap_tpto_s_remove( map, kv.key );
        if( bcore_hmap_tpto_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_tpto_s_status( map ) );

    bcore_hmap_tpto_s_discard( map );
    bcore_alloc( kvbuf, 0 );

    bcore_life_s_discard( l );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/

/**********************************************************************************************************************/
// bcore_hmap_tp_s

void bcore_hmap_tp_s_init( bcore_hmap_tp_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_hmap_tp_s;
    o->size_limit = 0xFFFFFFFFu;
}

void bcore_hmap_tp_s_down( bcore_hmap_tp_s* o )
{
    o->keys  = bcore_u_alloc( sizeof( tp_t ), o->keys,  0, NULL );
    o->flags = bcore_u_alloc( sizeof( bl_t ), o->flags, 0, NULL );
    o->size = 0;
}

void bcore_hmap_tp_s_copy( bcore_hmap_tp_s* o, const bcore_hmap_tp_s* src )
{
    o->keys  = bcore_u_alloc( sizeof( tp_t ), o->keys,  0, NULL );
    o->flags = bcore_u_alloc( sizeof( bl_t ), o->flags, 0, NULL );
    o->size  = 0;
    o->keys  = bcore_u_memcpy( sizeof( tp_t ), o->keys, src->keys, src->size );
    o->size  = src->size;
    o->depth_limit = src->depth_limit;
    o->size_limit  = src->size_limit;
}

BCORE_DEFINE_FUNCTION_CREATE(  bcore_hmap_tp_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_hmap_tp_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_hmap_tp_s )

/**********************************************************************************************************************/

static uz_t tp_find( const bcore_hmap_tp_s* o, tp_t key ) // returns valid index or o->size
{
    if( o->size == 0 ) return o->size;
    u2_t mask = o->size - 1;
    uz_t idx = hash_tpu2_1( key ) & mask;
    if( o->keys[ idx ] == key ) return idx;
    idx = hash_tpu2_2( key ) & mask;
    if( o->keys[ idx ] == key ) return idx;
    return o->size;
}

static uz_t tp_set( bcore_hmap_tp_s* o, tp_t key, uz_t depth ) // sets new node, returns valid index on success; o->size otherwise
{
    uz_t size = o->size;
    if( size == 0 ) return size;
    u2_t mask = o->size - 1;

    if( depth == o->depth_limit ) return size;

    uz_t idx1 = hash_tpu2_1( key ) & mask;
    if( !o->keys[ idx1 ] )
    {
        o->keys[ idx1 ] = key;
        return idx1;
    }

    uz_t idx2 = hash_tpu2_2( key ) & mask;
    if( !o->keys[ idx2 ] )
    {
        o->keys[ idx2 ] = key;
        return idx2;
    }

    if( !o->flags ) o->flags = bcore_u_memzero( sizeof( bl_t ), NULL, size );

    if( !o->flags[ idx1 ] )
    {
        o->flags[ idx1 ] = true;
        if( tp_set( o, o->keys[ idx1 ], depth + 1 ) < size )
        {
            o->flags[ idx1 ] = false;
            o->keys[  idx1 ] = key;
            return idx1;
        }
        o->flags[ idx1 ] = false;
    }

    if( !o->flags[ idx2 ] )
    {
        o->flags[ idx2 ] = true;
        if( tp_set( o, o->keys[ idx2 ], depth + 1 ) < size )
        {
            o->flags[ idx2 ] = false;
            o->keys[  idx2 ] = key;
            return idx2;
        }
        o->flags[ idx2 ] = false;
    }

    return size;
}

uz_t bcore_hmap_tp_s_get( const bcore_hmap_tp_s* o, tp_t key )
{
    if( !key ) return o->size;
    return tp_find( o, key );
}

uz_t bcore_hmap_tp_s_fget( bcore_hmap_tp_s* o, tp_t key )
{
    if( !key ) ERR( "key is 0" );
    uz_t idx = tp_find( o, key );
    if( idx < o->size )
    {
        return idx;
    }
    else
    {
        return bcore_hmap_tp_s_set( o, key );
    }
}

uz_t bcore_hmap_tp_s_set( bcore_hmap_tp_s* o, tp_t key )
{
    if( !key ) ERR( "key is zero" );

    {
        uz_t idx = tp_find( o, key );
        if( idx < o->size ) return idx;
        idx = tp_set( o, key, 1 );
        if( idx < o->size ) return idx;
    }

    // rehash
    {
        tp_t* buf_keys  = o->keys;
        uz_t  buf_size  = o->size;
        o->flags = bcore_u_alloc( sizeof( bl_t ), o->flags, 0, NULL );
        o->depth_limit = ( o->size > 0 ) ? o->depth_limit + 1 : 4;
        o->size        = ( o->size > 0 ) ? o->size * 2        : 8;
        if( o->size > o->size_limit ) ERR( "size limit (%zu) exceeded", o->size_limit );
        o->keys = bcore_u_memzero( sizeof( tp_t ), NULL, o->size );
        for( uz_t i = 0; i < buf_size; i++ )
        {
            if( buf_keys[ i ] ) bcore_hmap_tp_s_set( o, buf_keys[ i ] );
        }
        bcore_u_alloc( sizeof( tp_t ), buf_keys, 0, NULL );
    }

    return bcore_hmap_tp_s_set( o, key );
}

uz_t bcore_hmap_tp_s_remove( bcore_hmap_tp_s* o, tp_t key )
{
    if( !key ) return o->size;

    uz_t idx = tp_find( o, key );
    if( idx < o->size )
    {
        o->keys[ idx ] = 0;
    }
    return idx;
}

bl_t bcore_hmap_tp_s_exists( const bcore_hmap_tp_s* o, tp_t key )
{
    if( !key ) return false;
    return ( tp_find( o, key ) < o->size );
}

void bcore_hmap_tp_s_clear( bcore_hmap_tp_s* o )
{
    o->keys  = bcore_u_alloc( sizeof( tp_t ), o->keys,  0, NULL );
    o->flags = bcore_u_alloc( sizeof( bl_t ), o->flags, 0, NULL );
    o->size = 0;
}

uz_t bcore_hmap_tp_s_keys( const bcore_hmap_tp_s* o )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->keys[ i ] > 0 );
    return count;
}

uz_t bcore_hmap_tp_s_size( const bcore_hmap_tp_s* o )
{
    return o->size;
}

tp_t bcore_hmap_tp_s_idx_key( const bcore_hmap_tp_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->keys[ idx ];
}

static sr_s tp_s_get_data( const bcore_hmap_tp_s* o )
{
    tp_t t_data = bcore_flect_type_parse_sc( "{ tp_t []; }" );
    sr_s sr = sr_cp( sr_create( t_data ), TYPEOF_bcore_array_s );
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->keys[ i ] != 0 ) bcore_array_r_push( &sr, sr_twc( TYPEOF_tp_t, &o->keys[ i ] ) );
    }
    return sr;
}

static void tp_s_set_data( bcore_hmap_tp_s* o, sr_s data )
{
    bcore_hmap_tp_s_clear( o );
    assert( sr_s_type( &data ) == bcore_flect_type_parse_sc( "{ tp_t []; }" ) );
    data = sr_cp( data, TYPEOF_bcore_array_s );
    uz_t size = bcore_array_r_get_size( &data );
    const tp_t* src = bcore_array_r_get_c_data( &data );
    for( uz_t i = 0; i < size; i++ ) bcore_hmap_tp_s_set( o, src[ i ] );
    sr_down( data );
}

/**********************************************************************************************************************/

static bcore_self_s* hmap_tp_s_create_self( void )
{
    sc_t def = "\
        bcore_hmap_tp_s = bcore_inst \
            { \
                aware_t _; \
                private tp_t * keys; \
                private bl_t * flags; \
                private uz_t size; \
                private uz_t depth_limit; \
                private uz_t size_limit; \
                shell { tp_t []; } data; } \
            }";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_hmap_tp_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tp_s_init,    "bcore_fp_init",  "init"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tp_s_down,    "bcore_fp_down",  "down"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tp_s_copy,    "bcore_fp_copy",  "copy"     );
    bcore_self_s_push_ns_func( self, ( fp_t )tp_s_get_data,           "bcore_fp_get",   "get_data" );
    bcore_self_s_push_ns_func( self, ( fp_t )tp_s_set_data,           "bcore_fp_set",   "set_data" );
    return self;
}

/**********************************************************************************************************************/

static st_s* hmap_tp_s_status( bcore_hmap_tp_s* o )
{
    st_s* string = st_s_create();
    uz_t keys = bcore_hmap_tp_s_keys( o );
    st_s_pushf( string, "keys ........... %zu\n", keys );
    st_s_pushf( string, "nodes .......... %lu\n", o->size );
    st_s_pushf( string, "keys/nodes ..... %5.4f\n", o->size > 0 ? ( f3_t )( keys ) / o->size : 0 );
    return string;
}

/**********************************************************************************************************************/

static st_s* hmap_tp_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    st_s* log = st_s_createf( "== bcore_hmap_tp_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    bcore_hmap_tp_s* map = bcore_hmap_tp_s_create();
    const uz_t cycles = 200000;

    typedef struct
    {
        tp_t key;
        uz_t val;
    } kv_s;

    kv_s* kvbuf = bcore_alloc( NULL, cycles * sizeof( kv_s ) );
    uz_t kvbuf_size = 0;

    clock_t time = clock();
    st_s_pushf( log, "Mixed access: " );

    {
        u3_t rval1 = 1;
        u3_t rval2 = 12345;
        for( uz_t i = 0; i < cycles; i++ )
        {
            rval1 = bcore_xsg_u2( rval1 );
            rval2 = bcore_xsg_u2( rval2 );
            kv_s kv;
            kv.key = ( u2_t )rval1;
            kv.val = bcore_hmap_tp_s_set( map, kv.key );
            kvbuf[ kvbuf_size++ ] = kv;

            // retrieve
            rval1 = bcore_xsg_u2( rval1 );
            kv = kvbuf[ rval1 % kvbuf_size ];
            uz_t val = bcore_hmap_tp_s_get( map, kv.key );
            if( val == map->size ) ERR( "index error (%lu)", val );

            // delete
            rval1 = bcore_xsg_u2( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_xsg_u2( rval1 );
                uz_t idx = rval1 % kvbuf_size;
                kv_s kv = kvbuf[ idx ];
                if( bcore_hmap_tp_s_get( map, kv.key ) == map->size )  ERR( "key (%lu) not found", kv.key );
                bcore_hmap_tp_s_remove( map, kv.key );
                if( bcore_hmap_tp_s_get( map, kv.key ) <  map->size )  ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_tp_s_status( map ) );

    time = clock();
    bcore_hmap_tp_s* map2 = bcore_life_s_push_aware( l, bcore_hmap_tp_s_clone( map ) );
    time = clock() - time;
    st_s_pushf( log, "Clone .......... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );
    ASSERT( bcore_compare_aware( map, map2 ) == 0 );

    time = clock();
    ASSERT( bcore_compare_aware( map, map2 ) == 0 );
    time = clock() - time;
    st_s_pushf( log, "Comparison ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    bcore_hmap_tp_s* map3 = bcore_life_s_push_aware( l, bcore_hmap_tp_s_create() );
    bcore_via_a_nset( (bcore_via*)map3, typeof( "data" ), bcore_via_a_nget( (bcore_via*)map, typeof( "data" ) ) );
    time = clock() - time;
    st_s_pushf( log, "Via-assign ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    for( uz_t i = 0; i < map->size; i++ )
    {
        tp_t key = bcore_hmap_tp_s_idx_key( map, i );
        ASSERT( ( key == 0 ) || bcore_hmap_tp_s_exists( map3, key ) );
    }
    time = clock() - time;
    st_s_pushf( log, "verify assign .. %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    st_s_pushf( log, "\nRead-access of %lu keys: ", kvbuf_size );
    uz_t read_cycles = 20;
    for( uz_t j = 0; j < read_cycles; j++ )
    {
        for( uz_t i = 0; i < kvbuf_size; i++ )
        {
            if( bcore_hmap_tp_s_get( map, kvbuf[ i ].key ) == map->size ) ERR( "index error (%lu)", kvbuf[ i ].val );
        }
    }
    time = clock() - time;
    st_s_pushf( log, "(%gs per access)\n", ( ( double )time/CLOCKS_PER_SEC ) / ( kvbuf_size * read_cycles ) );

    st_s_pushf( log, "\n" );
    st_s_pushf( log, "Removal: " );
    time = clock();
    while( kvbuf_size )
    {
        kvbuf_size--;
        kv_s kv = kvbuf[ kvbuf_size ];
        if( bcore_hmap_tp_s_get( map, kv.key ) == map->size )  ERR( "key (%lu) not found", kv.key );
        bcore_hmap_tp_s_remove( map, kv.key );
        if( bcore_hmap_tp_s_get( map, kv.key )  < map->size )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );

    st_s_push_st_d( log, hmap_tp_s_status( map ) );

    bcore_hmap_tp_s_discard( map );
    bcore_alloc( kvbuf, 0 );

    bcore_life_s_discard( l );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/

vd_t bcore_hmap_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_hmap" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( TYPEOF_bcore_hmap_u2vd_s      , hmap_u2vd_s_create_self  );
            bcore_flect_define_creator( typeof( "bcore_hnode_tpsz_s" ), hnode_tpsz_s_create_self  );
            bcore_flect_define_creator( TYPEOF_bcore_hmap_tpsz_s      , hmap_tpsz_s_create_self  );
            bcore_flect_define_creator( typeof( "bcore_hnode_tpfp_s" ), hnode_tpfp_s_create_self  );
            bcore_flect_define_creator( TYPEOF_bcore_hmap_tpfp_s      , hmap_tpfp_s_create_self  );
            bcore_flect_define_creator( typeof( "bcore_hnode_tptp_s" ), hnode_tptp_s_create_self  );
            bcore_flect_define_creator( TYPEOF_bcore_hmap_tptp_s      , hmap_tptp_s_create_self  );
            bcore_flect_define_creator( TYPEOF_bcore_hmap_tpto_s      , hmap_tpto_s_create_self  );
            bcore_flect_define_creator( TYPEOF_bcore_hmap_tp_s        , hmap_tp_s_create_self    );
        }
        break;

        case TYPEOF_selftest:
        {
            st_s* log = st_s_create();
            st_s_push_st_d( log, hmap_u2vd_selftest() );
            st_s_push_st_d( log, hmap_tpsz_selftest() );
            st_s_push_st_d( log, hmap_tpfp_selftest() );
            st_s_push_st_d( log, hmap_tptp_selftest() );
            st_s_push_st_d( log, hmap_tpto_selftest() );
            st_s_push_st_d( log, hmap_tp_selftest()   );
            return log;
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

