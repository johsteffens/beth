/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_hmap_tp_sr.h"
#include "bcore_life.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_compare.h"
#include "bcore_spect_array.h"
#include "bcore_signal.h"
#include <time.h>

/**********************************************************************************************************************/

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
    o->key = src->key;
    sr_s_copy( &o->val, &src->val );
}

sr_s* bcore_hnode_tp_sr_s_set( bcore_hnode_tp_sr_s* o, sr_s obj )
{
    sr_s_set( &o->val, obj );
    return &o->val;
}

static bcore_self_s* hnode_tp_sr_s_create_self( void )
{
    sc_t def = "\
        bcore_hnode_tp_sr_s = bcore_inst \
        { \
            tp_t key; \
            sr_s val; \
        }";
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( def, bcore_hnode_tp_sr_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hnode_tp_sr_s_init, "bcore_fp_init", "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hnode_tp_sr_s_down, "bcore_fp_down", "down" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hnode_tp_sr_s_copy, "bcore_fp_copy", "copy" );
    return self;
}

/**********************************************************************************************************************/
// bcore_hmap_tp_sr_s

void bcore_hmap_tp_sr_s_init( bcore_hmap_tp_sr_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = typeof( "bcore_hmap_tp_sr_s" );
    o->size_limit = 0xFFFFFFFFu;
}

void bcore_hmap_tp_sr_s_down( bcore_hmap_tp_sr_s* o )
{
    for( uz_t i = 0; i < o->size; i++ ) bcore_hnode_tp_sr_s_down( &o->nodes[ i ] );
    o->nodes = bcore_alloc( o->nodes, 0 );
    o->flags = bcore_alloc( o->flags, 0 );
    o->size = 0;
}

void bcore_hmap_tp_sr_s_copy( bcore_hmap_tp_sr_s* o, const bcore_hmap_tp_sr_s* src )
{
    for( uz_t i = 0; i < o->size; i++ ) bcore_hnode_tp_sr_s_down( &o->nodes[ i ] );
    o->nodes = bcore_alloc( o->nodes, 0 );
    o->flags = bcore_alloc( o->flags, 0 );
    o->size = 0;

    o->nodes = bcore_u_memzero( sizeof( bcore_hnode_tp_sr_s ), o->nodes, src->size );
    for( uz_t i = 0; i < src->size; i++ ) bcore_hnode_tp_sr_s_copy( &o->nodes[ i ], &src->nodes[ i ] );
    o->size  = src->size;
    o->depth_limit = src->depth_limit;
    o->size_limit  = src->size_limit;
}

BCORE_DEFINE_FUNCTION_CREATE(  bcore_hmap_tp_sr_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_hmap_tp_sr_s )
BCORE_DEFINE_FUNCTION_CLONE(   bcore_hmap_tp_sr_s )

/**********************************************************************************************************************/

static uz_t tp_sr_find( const bcore_hmap_tp_sr_s* o, tp_t key ) // returns valid index or o->size
{
    if( o->size == 0 ) return o->size;
    u2_t mask = o->size - 1;
    uz_t idx = hash_tpu2_1( key ) & mask;
    if( o->nodes[ idx ].key == key ) return idx;
    idx = hash_tpu2_2( key ) & mask;
    if( o->nodes[ idx ].key == key ) return idx;
    return o->size;
}

static uz_t tp_sr_set( bcore_hmap_tp_sr_s* o, bcore_hnode_tp_sr_s node, uz_t depth ) // sets new node
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
        if( tp_sr_set( o, o->nodes[ idx1 ], depth + 1 ) < size )
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
        if( tp_sr_set( o, o->nodes[ idx2 ], depth + 1 ) < size )
        {
            o->flags[ idx2 ] = false;
            o->nodes[ idx2 ] = node;
            return idx2;
        }
        o->flags[ idx2 ] = false;
    }

    return size;
}

sr_s* bcore_hmap_tp_sr_s_get( const bcore_hmap_tp_sr_s* o, tp_t key )
{
    if( !key ) return NULL;
    uz_t idx = tp_sr_find( o, key );
    return ( idx < o->size ) ? &o->nodes[ idx ].val : NULL;
}

sr_s* bcore_hmap_tp_sr_s_set( bcore_hmap_tp_sr_s* o, tp_t key, sr_s val )
{
    if( !key ) ERR( "key is zero" );

    uz_t idx = tp_sr_find( o, key );
    if( idx < o->size ) return bcore_hnode_tp_sr_s_set( &o->nodes[ idx ], val );

    {
        bcore_hnode_tp_sr_s node;
        bcore_hnode_tp_sr_s_init( &node );
        node.key = key;
        idx = tp_sr_set( o, node, 1 );
        if( idx < o->size ) return bcore_hnode_tp_sr_s_set( &o->nodes[ idx ], val );
    }

    // rehash
    {
        o->flags = bcore_u_alloc( sizeof( bl_t ), o->flags, 0, NULL );
        bcore_hnode_tp_sr_s* buf_nodes = o->nodes;
        uz_t  buf_size = o->size;
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

        // Note: We can safely reset the memory as long as bcore_hnode_tp_sr_s_init is memzero
        o->nodes = bcore_u_memzero( sizeof( bcore_hnode_tp_sr_s ), NULL, o->size );

        for( uz_t i = 0; i < buf_size; i++ )
        {
            if( buf_nodes[ i ].key ) bcore_hmap_tp_sr_s_set( o, buf_nodes[ i ].key, buf_nodes[ i ].val );
        }

        // ownership of all buffered nodes has been been transferred above by passing sr_s by value -> nodes must not explicitly shut down
        bcore_alloc( buf_nodes, 0 );
    }

    return bcore_hmap_tp_sr_s_set( o, key, val );
}

sr_s bcore_hmap_tp_sr_s_remove( bcore_hmap_tp_sr_s* o, tp_t key )
{
    if( key )
    {
        uz_t idx = tp_sr_find( o, key );
        if( idx < o->size )
        {
            bcore_hnode_tp_sr_s* node = &o->nodes[ idx ];
            sr_s ret = node->val;
            node->val = sr_null();
            node->key = 0;
            return ret;
        }
    }

    return sr_null();
}
bl_t bcore_hmap_tp_sr_s_exists( const bcore_hmap_tp_sr_s* o, tp_t key )

{
    if( !key ) return false;
    uz_t idx = tp_sr_find( o, key );
    return ( idx < o->size );
}

void bcore_hmap_tp_sr_s_clear( bcore_hmap_tp_sr_s* o )
{
    for( uz_t i = 0; i < o->size; i++ ) bcore_hnode_tp_sr_s_down( &o->nodes[ i ] );
    o->nodes = bcore_alloc( o->nodes, 0 );
    o->flags = bcore_alloc( o->flags, 0 );
    o->size = 0;
}

uz_t bcore_hmap_tp_sr_s_keys( const bcore_hmap_tp_sr_s* o )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ ) count += ( o->nodes[ i ].key > 0 );
    return count;
}

uz_t bcore_hmap_tp_sr_s_size( const bcore_hmap_tp_sr_s* o )
{
    return o->size;
}

tp_t bcore_hmap_tp_sr_s_idx_key( const bcore_hmap_tp_sr_s* o, uz_t idx )
{
    assert( idx < o->size );
    return o->nodes[ idx ].key;
}

const sr_s* bcore_hmap_tp_sr_s_idx_val( const bcore_hmap_tp_sr_s* o, uz_t idx )
{
    assert( idx < o->size );
    return &o->nodes[ idx ].val;
}

static sr_s tp_sr_s_get_data( const bcore_hmap_tp_sr_s* o )
{
    tp_t t_data = bcore_flect_type_parse_sc( "{ bcore_hnode_tp_sr_s []; }" );
    tp_t t_node = typeof( "bcore_hnode_tp_sr_s" );
    sr_s sr = sr_cp( sr_create( t_data ), TYPEOF_bcore_array_s );
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->nodes[ i ].key ) bcore_array_r_push( &sr, sr_twc( t_node, &o->nodes[ i ] ) );
    }
    return sr;
}

static void tp_sr_s_set_data( bcore_hmap_tp_sr_s* o, sr_s data )
{
    bcore_hmap_tp_sr_s_clear( o );
    assert( sr_s_type( &data ) == bcore_flect_type_parse_sc( "{ bcore_hnode_tp_sr_s []; }" ) );
    data = sr_cp( data, TYPEOF_bcore_array_s );
    uz_t size = bcore_array_r_get_size( &data );
    if( sr_s_is_strong( &data ) )
    {
        bcore_hnode_tp_sr_s* src = bcore_array_r_get_d_data( &data );
        for( uz_t i = 0; i < size; i++ )
        {
            bcore_hmap_tp_sr_s_set( o, src[ i ].key, src[ i ].val );
            src[ i ].val = sr_null();
        }
    }
    else
    {
        const bcore_hnode_tp_sr_s* src = bcore_array_r_get_c_data( &data );
        for( uz_t i = 0; i < size; i++ )
        {
            bcore_hmap_tp_sr_s_set( o, src[ i ].key, sr_cw( src[ i ].val ) );
        }
    }
    sr_down( data );
}

/**********************************************************************************************************************/

static bcore_self_s* hmap_tp_sr_s_create_self( void )
{
    sc_t def = "\
        bcore_hmap_tp_sr_s = bcore_inst \
        { \
            aware_t _; \
            private bcore_hnode_tp_sr_s* nodes; \
            private bl_t* flags; \
            private uz_t size; \
            private uz_t depth_limit; \
            private uz_t size_limit; \
            shell { bcore_hnode_tp_sr_s []; } data; } \
        }";
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( def, bcore_hmap_tp_sr_s );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tp_sr_s_init,    "bcore_fp_init",  "init"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tp_sr_s_down,    "bcore_fp_down",  "down"     );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_hmap_tp_sr_s_copy,    "bcore_fp_copy",  "copy"     );
    bcore_self_s_push_ns_func( self, ( fp_t )tp_sr_s_get_data,           "bcore_fp_get",   "get_data" );
    bcore_self_s_push_ns_func( self, ( fp_t )tp_sr_s_set_data,           "bcore_fp_set",   "set_data" );
    return self;
}

/**********************************************************************************************************************/
// selftest

#include <time.h>

static st_s* hmap_tp_sr_s_status( bcore_hmap_tp_sr_s* o )
{
    st_s* string = st_s_create();
    uz_t keys = bcore_hmap_tp_sr_s_keys( o );
    st_s_pushf( string, "keys ........... %zu\n", keys );
    st_s_pushf( string, "nodes .......... %lu\n", o->size );
    st_s_pushf( string, "keys/nodes ..... %5.4f\n", o->size > 0 ? ( f3_t )( keys ) / o->size : 0 );
    return string;
}

static st_s* hmap_tp_sr_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    st_s* log = st_s_createf( "== bcore_hmap_tp_sr_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    bcore_hmap_tp_sr_s* map = bcore_life_s_push_aware( l, bcore_hmap_tp_sr_s_create() );
    const uz_t cycles = 200000;

    typedef struct
    {
        tp_t key;
        u3_t val;
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
                uz_t idx = rval1 % kvbuf_size;
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
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );
    st_s_push_st_d( log, hmap_tp_sr_s_status( map ) );

    time = clock();
    bcore_hmap_tp_sr_s* map2 = bcore_life_s_push_aware( l, bcore_hmap_tp_sr_s_clone( map ) );
    time = clock() - time;
    st_s_pushf( log, "Clone .......... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    if( bcore_compare_aware( map, map2 ) != 0 )
    {
        ERR( "comparison failed:\n%s", bcore_diff_aware( map, map2 )->sc );
    }

    time = clock() - time;
    st_s_pushf( log, "Comparison ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );

    time = clock();
    bcore_hmap_tp_sr_s* map3 = bcore_life_s_push_aware( l, bcore_hmap_tp_sr_s_create() );
    bcore_via_a_nset( (bcore_via*)map3, typeof( "data" ), bcore_via_a_nget( (bcore_via*)map, typeof( "data" ) ) );
    time = clock() - time;
    st_s_pushf( log, "Via-assign ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );
    time = clock();
    for( uz_t i = 0; i < map->size; i++ )
    {
        tp_t key = bcore_hmap_tp_sr_s_idx_key( map, i );
        if( key != 0 )
        {
            u3_t val = *( u3_t * )bcore_hmap_tp_sr_s_idx_val( map, i )->o;
            ASSERT( bcore_hmap_tp_sr_s_exists( map3, key ) );
            ASSERT( val == *( u3_t * )bcore_hmap_tp_sr_s_get( map3, key )->o );
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
            u3_t val = *( u3_t* )bcore_hmap_tp_sr_s_get( map, kvbuf[ i ].key )->o;
            if( val != kvbuf[ i ].val ) ERR( "value mismatch (%lu vs %lu)", val, kvbuf[ i ].val );
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
        if( !bcore_hmap_tp_sr_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
        sr_down( bcore_hmap_tp_sr_s_remove( map, kv.key ) );
        if( bcore_hmap_tp_sr_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );
    st_s_push_st_d( log, hmap_tp_sr_s_status( map ) );
    bcore_alloc( kvbuf, 0 );
    bcore_life_s_discard( l );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_hmap_tp_sr_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_hmap_tp_sr" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "bcore_hnode_tp_sr_s" ), hnode_tp_sr_s_create_self );
            bcore_flect_define_creator( typeof( "bcore_hmap_tp_sr_s"  ), hmap_tp_sr_s_create_self  );
        }
        break;

        case TYPEOF_selftest:
        {
            return hmap_tp_sr_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}
