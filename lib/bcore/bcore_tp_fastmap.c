/** Copyright 2018 Johannes Bernhard Steffens
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

#include "bcore_tp_fastmap.h"
#include "bcore_threads.h"
#include "bcore_quicktypes.h"

#define BCORE_TP_FASTMAP_MAX_TABLES 20
#define BCORE_TP_FASTMAP_START_SIZE  8

typedef struct bcore_tp_kv_s
{
    atomic_tp_t key;
    atomic_vc_t val;
} bcore_tp_kv_s;

typedef struct bcore_tp_table_s
{
    sz_t size;
    tp_t mask;
    bcore_tp_kv_s * arr;
} bcore_tp_table_s;

void bcore_tp_table_s_init( bcore_tp_table_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void bcore_tp_table_s_down( bcore_tp_table_s* o )
{
    if( o->arr ) bcore_release( o->arr );
}

sz_t bcore_tp_table_s_keys( bcore_tp_table_s* o )
{
    sz_t count = 0;
    for( sz_t i = 0; i < o->size; i++ )
    {
        if( o->arr[ i ].key ) count++;
    }
    return count;
}

vc_t bcore_tp_table_s_get( const bcore_tp_table_s* o, tp_t key )
{
    sz_t idx = key & o->mask;
    if( o->arr[ idx     ].key == key ) return o->arr[ idx     ].val;
    if( o->arr[ idx + 1 ].key == key ) return o->arr[ idx + 1 ].val;
    if( o->arr[ idx + 2 ].key == key ) return o->arr[ idx + 2 ].val;
    if( o->arr[ idx + 3 ].key == key ) return o->arr[ idx + 3 ].val;
    return NULL;
}

bl_t bcore_tp_table_s_set( bcore_tp_table_s* o, tp_t key, vc_t val )
{
    sz_t idx = key & o->mask;
    if( o->arr[ idx ].key == key ) return true;
    if( o->arr[ idx ].key == 0   ) { o->arr[ idx ].key = key; o->arr[ idx ].val = val; return true; }
    idx++;
    if( o->arr[ idx ].key == key ) return true;
    if( o->arr[ idx ].key == 0   ) { o->arr[ idx ].key = key; o->arr[ idx ].val = val; return true; }
    idx++;
    if( o->arr[ idx ].key == key ) return true;
    if( o->arr[ idx ].key == 0   ) { o->arr[ idx ].key = key; o->arr[ idx ].val = val; return true; }
    idx++;
    if( o->arr[ idx ].key == key ) return true;
    if( o->arr[ idx ].key == 0   ) { o->arr[ idx ].key = key; o->arr[ idx ].val = val; return true; }
    return false;
}

/// creates new table of double size and copies keys
void bcore_tp_table_s_init_new( bcore_tp_table_s* o, const bcore_tp_table_s* src )
{
    o->size = src ? src->size << 1 : BCORE_TP_FASTMAP_START_SIZE;
    o->mask = ( o->size - 1 ) ^ 3;
    o->arr = bcore_malloc( sizeof( bcore_tp_kv_s ) * o->size );
    bcore_memzero( o->arr, sizeof( bcore_tp_kv_s ) * o->size );
    if( src )
    {
        for( sz_t i = 0; i < src->size; i++ )
        {
            if( src->arr[ i ].key ) bcore_tp_table_s_set( o, src->arr[ i ].key, src->arr[ i ].val );
        }
    }
}

/**********************************************************************************************************************/

typedef struct bcore_tp_fastmap_s
{
    atomic_int table_index_get; // valid table index for get function
    atomic_int table_index_set; // valid table index for set function
    bcore_tp_table_s table_arr[ BCORE_TP_FASTMAP_MAX_TABLES ];
    bcore_mutex_s mutex;
}
bcore_tp_fastmap_s;

void bcore_tp_fastmap_s_init( bcore_tp_fastmap_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    bcore_mutex_s_init( &o->mutex );

    bcore_mutex_s_lock( &o->mutex );
    for( sz_t i = 0; i < BCORE_TP_FASTMAP_MAX_TABLES; i++ ) bcore_tp_table_s_init( &o->table_arr[ i ] );
    bcore_tp_table_s_init_new( &o->table_arr[ 0 ], NULL );
    bcore_mutex_s_unlock( &o->mutex );
}

void bcore_tp_fastmap_s_down( bcore_tp_fastmap_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    for( sz_t i = 0; i < BCORE_TP_FASTMAP_MAX_TABLES; i++ ) bcore_tp_table_s_down( &o->table_arr[ i ] );
    bcore_mutex_s_unlock( &o->mutex );

    bcore_mutex_s_down( &o->mutex );
}

void bcore_tp_fastmap_s_copy( bcore_tp_fastmap_s* o, const bcore_tp_fastmap_s* src )
{
    ERR_fa( "Copying bcore_tp_fastmap_s is not supported." );
}

BCORE_DEFINE_FUNCTIONS_CDC( bcore_tp_fastmap_s )

vc_t bcore_tp_fastmap_s_get( const bcore_tp_fastmap_s* o, tp_t key )
{
    return bcore_tp_table_s_get( &o->table_arr[ o->table_index_get ], key );
}

bl_t bcore_tp_fastmap_s_set( bcore_tp_fastmap_s* o, tp_t key, vc_t val )
{
    bcore_mutex_s_lock( &o->mutex );
    bcore_tp_table_s* table = &o->table_arr[ o->table_index_set ];
    if( bcore_tp_table_s_set( table, key, val ) )
    {
        bcore_mutex_s_unlock( &o->mutex );
        return true;
    }

    if( o->table_index_set == BCORE_TP_FASTMAP_MAX_TABLES - 1 )
    {
        bcore_mutex_s_unlock( &o->mutex );
        return false; // table size limit reached
    }


    o->table_index_set++;
    bcore_tp_table_s* new_table = &o->table_arr[ o->table_index_set ];
    bcore_tp_table_s_init_new( new_table, table );
    bcore_mutex_s_unlock( &o->mutex );

    // inserting a fence to prevent table_index_get being updated too early
    bcore_mutex_s_lock( &o->mutex );
    o->table_index_get = o->table_index_set; // this update must be locked to prevent table_index_get being left with a lower value in a race condition
    bcore_mutex_s_unlock( &o->mutex );

    return bcore_tp_fastmap_s_set( o, key, val );
}

sz_t bcore_tp_fastmap_s_size( bcore_tp_fastmap_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    sz_t size = o->table_arr[ o->table_index_set ].size;
    bcore_mutex_s_unlock( &o->mutex );
    return size;
}

sz_t bcore_tp_fastmap_s_keys( bcore_tp_fastmap_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    sz_t keys = bcore_tp_table_s_keys( &o->table_arr[ o->table_index_set ] );
    bcore_mutex_s_unlock( &o->mutex );
    return keys;
}

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    st_s* log = st_s_createf( "== bcore_tp_fastmap_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    const sz_t cycles = 2000000;
    sz_t max_keys = 1 << 18;
    tp_t key_mask = max_keys - 1;

    bcore_tp_fastmap_s* map = bcore_tp_fastmap_s_create();

    u2_t rinit = 12234;
    u2_t rval = rinit;

    for( sz_t i = 0; i < cycles; i++ )
    {
        rval = bcore_xsg_u2( rval );
        tp_t key = rval & key_mask;
        vc_t val = ( vc_t )( intptr_t )key;

        vc_t v = bcore_tp_fastmap_s_get( map, key );
        if( v )
        {
            ASSERT( v == val );
        }
        else
        {
            ASSERT( bcore_tp_fastmap_s_set( map, key, val ) );
            vc_t v = bcore_tp_fastmap_s_get( map, key );
            ASSERT( v == val );
        }
    }

    rval = rinit;

    // test retrieve only
    for( sz_t i = 0; i < cycles; i++ )
    {
        rval = bcore_xsg_u2( rval );
        tp_t key = rval & key_mask;
        vc_t val = ( vc_t )( intptr_t )key;
        vc_t v = bcore_tp_fastmap_s_get( map, key );
        ASSERT( v == val );
    }

    // overhead time
    clock_t overhead = clock();
    for( sz_t i = 0; i < cycles; i++ )
    {
        rval = bcore_xsg_u2( rval );
    }
    overhead = clock() - overhead;

    // time test retrieve only
    clock_t time = clock();
    for( sz_t i = 0; i < cycles; i++ )
    {
        rval = bcore_xsg_u2( rval );
        tp_t key = rval & key_mask;
        bcore_tp_fastmap_s_get( map, key );
    }
    time = clock() - time;

    st_s_push_fa( log, "get ...... #<sz_t>ns\n", ( sz_t ) ( ( 1E9 * ( double )( time - overhead )/CLOCKS_PER_SEC ) / cycles ) );

    st_s_push_fa( log, "max_keys . #<sz_t>\n", max_keys );
    st_s_push_fa( log, "size ..... #<sz_t>\n", bcore_tp_fastmap_s_size( map ) );
    st_s_push_fa( log, "keys ..... #<sz_t>\n", bcore_tp_fastmap_s_keys( map ) );

    sz_t bcore_tp_fastmap_s_keys( bcore_tp_fastmap_s* o );


    bcore_tp_fastmap_s_discard( map );

    st_s_push_char_n( log, '=', 120 );
    st_s_push_char( log, '\n' );
    return log;
}

/**********************************************************************************************************************/

vd_t bcore_tp_fastmap_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_tp_fastmap" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            return selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

