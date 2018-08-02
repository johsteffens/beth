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
#include "bcore_quicktypes.h"


void bcore_tp_fastmap_table_s_init( bcore_tp_fastmap_table_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void bcore_tp_fastmap_table_s_clear( bcore_tp_fastmap_table_s* o )
{
    if( o->arr ) bcore_release( o->arr );
    o->arr = NULL;
    o->size = 0;
    o->mask = 0;
}

void bcore_tp_fastmap_table_s_down( bcore_tp_fastmap_table_s* o )
{
    bcore_tp_fastmap_table_s_clear( o );
}

uz_t bcore_tp_fastmap_table_s_keys( bcore_tp_fastmap_table_s* o )
{
    uz_t count = 0;
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->arr[ i ].key ) count++;
    }
    return count;
}

bl_t bcore_tp_fastmap_table_s_set( bcore_tp_fastmap_table_s* o, tp_t key, vc_t val )
{
    uz_t idx = key & o->mask;
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
void bcore_tp_fastmap_table_s_init_new( bcore_tp_fastmap_table_s* o, const bcore_tp_fastmap_table_s* src )
{
    o->size = src ? src->size << 1 : BCORE_TP_FASTMAP_START_SIZE;
    o->mask = ( o->size - 1 ) ^ ( BCORE_TP_FASTMAP_SCAN_LENGTH - 1 );

    uz_t alloc_size = sizeof( bcore_tp_fastmap_kv_s ) * o->size;
    o->arr = bcore_malloc( alloc_size );
    bcore_memzero( o->arr, alloc_size );
    if( src )
    {
        for( uz_t i = 0; i < src->size; i++ )
        {
            if( src->arr[ i ].key ) bcore_tp_fastmap_table_s_set( o, src->arr[ i ].key, src->arr[ i ].val );
        }
    }
}

/**********************************************************************************************************************/

void bcore_tp_fastmap_s_init( bcore_tp_fastmap_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    bcore_mutex_s_init( &o->mutex );

    bcore_mutex_s_lock( &o->mutex );
    for( uz_t i = 0; i < BCORE_TP_FASTMAP_MAX_TABLES; i++ ) bcore_tp_fastmap_table_s_init( &o->table_arr[ i ] );
    bcore_tp_fastmap_table_s_init_new( &o->table_arr[ 0 ], NULL );
    bcore_mutex_s_unlock( &o->mutex );

    o->locked = false;
}

void bcore_tp_fastmap_s_clear( bcore_tp_fastmap_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    for( uz_t i = 0; i < BCORE_TP_FASTMAP_MAX_TABLES; i++ ) bcore_tp_fastmap_table_s_clear( &o->table_arr[ i ] );
    o->table_index_get = 0;
    o->table_index_set = 0;
    bcore_tp_fastmap_table_s_init_new( &o->table_arr[ 0 ], NULL );
    bcore_mutex_s_unlock( &o->mutex );
}

void bcore_tp_fastmap_s_down( bcore_tp_fastmap_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    for( uz_t i = 0; i < BCORE_TP_FASTMAP_MAX_TABLES; i++ ) bcore_tp_fastmap_table_s_down( &o->table_arr[ i ] );
    bcore_mutex_s_unlock( &o->mutex );

    bcore_mutex_s_down( &o->mutex );
}

void bcore_tp_fastmap_s_copy( bcore_tp_fastmap_s* o, const bcore_tp_fastmap_s* src )
{
    ERR_fa( "Copying bcore_tp_fastmap_s is not supported." );
}

BCORE_DEFINE_FUNCTIONS_CDC( bcore_tp_fastmap_s )

void bcore_tp_fastmap_s_set_locked( bcore_tp_fastmap_s* o, bl_t flag )
{
    bcore_mutex_s_lock( &o->mutex );
    o->locked = flag;
    bcore_mutex_s_unlock( &o->mutex );
}

bl_t bcore_tp_fastmap_s_set( bcore_tp_fastmap_s* o, tp_t key, vc_t val )
{
    bcore_mutex_s_lock( &o->mutex );
    if( o->locked )
    {
        bcore_mutex_s_unlock( &o->mutex );
        return false; // no change during lock
    }
    bcore_tp_fastmap_table_s* table = &o->table_arr[ o->table_index_set ];
    if( bcore_tp_fastmap_table_s_set( table, key, val ) )
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
    bcore_tp_fastmap_table_s* new_table = &o->table_arr[ o->table_index_set ];
    bcore_tp_fastmap_table_s_init_new( new_table, table );
    bcore_mutex_s_unlock( &o->mutex );

    // inserting a fence to prevent table_index_get being updated too early
    bcore_mutex_s_lock( &o->mutex );
    o->table_index_get = o->table_index_set; // this update must be locked to prevent table_index_get being left with a lower value in a race condition
    bcore_mutex_s_unlock( &o->mutex );

    return bcore_tp_fastmap_s_set( o, key, val );
}

uz_t bcore_tp_fastmap_s_size( bcore_tp_fastmap_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    uz_t size = o->table_arr[ o->table_index_set ].size;
    bcore_mutex_s_unlock( &o->mutex );
    return size;
}

uz_t bcore_tp_fastmap_s_keys( bcore_tp_fastmap_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    uz_t keys = bcore_tp_fastmap_table_s_keys( &o->table_arr[ o->table_index_set ] );
    bcore_mutex_s_unlock( &o->mutex );
    return keys;
}

/**********************************************************************************************************************/

static vd_t selftest( void )
{
    st_s* log = st_s_createf( "== bcore_tp_fastmap_selftest " );
    st_s_push_char_n( log, '=', 120 - log->size );
    st_s_push_char( log, '\n' );

    const uz_t cycles = 10000;
    const uz_t kvbuf_size = 2000;

    typedef struct
    {
        tp_t key;
        vc_t val;
    } kv_s;

    u2_t rinit = 12234;
    u2_t rval = rinit;

    kv_s* kvbuf = bcore_alloc( NULL, kvbuf_size * sizeof( kv_s ) );
    for( uz_t i = 0; i < kvbuf_size; i++ )
    {
        rval = bcore_xsg_u2( rval );
        tp_t key = rval;
        vc_t val = ( vc_t )( intptr_t )key;
        kvbuf[ i ].key = key;
        kvbuf[ i ].val = val;
    }

    bcore_tp_fastmap_s* map = bcore_tp_fastmap_s_create();

    for( uz_t i = 0; i < cycles; i++ )
    {
        for( uz_t j = 0; j < kvbuf_size; j++ )
        {
            kv_s kv = kvbuf[ j ];
            vc_t v = bcore_tp_fastmap_s_get( map, kv.key );
            if( v )
            {
                ASSERT( v == kv.val );
            }
            else
            {
                ASSERT( bcore_tp_fastmap_s_set( map, kv.key, kv.val ) );
                vc_t v = bcore_tp_fastmap_s_get( map, kv.key );
                ASSERT( v == kv.val );
            }
        }
    }

    rval = rinit;

    // test retrieve only
    for( uz_t i = 0; i < cycles; i++ )
    {
        for( uz_t j = 0; j < kvbuf_size; j++ )
        {
            kv_s kv = kvbuf[ j ];
            vc_t v = bcore_tp_fastmap_s_get( map, kv.key );
            ASSERT( v == kv.val );
        }
    }

    // overhead time
    clock_t time, overhead;
    {
        overhead = clock();
        uz_t sum = 0;
        for( uz_t i = 0; i < cycles; i++ )
        {
            for( uz_t j = 0; j < kvbuf_size; j++ )
            {
                kv_s kv = kvbuf[ j ];
                sum += ( uz_t )kv.val;
            }
        }
        st_s_push_fa( log, "", sum ); // let compiler think sum is used
        overhead = clock() - overhead;
    }

    // time test retrieve only
    {
        time = clock();
        uz_t sum = 0;
        for( uz_t i = 0; i < cycles; i++ )
        {
            for( uz_t j = 0; j < kvbuf_size; j++ )
            {
                kv_s kv = kvbuf[ j ];
                vc_t v = bcore_tp_fastmap_s_get( map, kv.key );
                sum += ( uz_t )v;
            }
        }
        time = clock() - time;
        st_s_push_fa( log, "", sum ); // let compiler think sum is used
    }

    st_s_push_fa( log, "get ... #<uz_t>ps\n", ( uz_t ) ( ( 1E12 * ( double )( time - overhead )/CLOCKS_PER_SEC ) / ( cycles * kvbuf_size ) ) );
    st_s_push_fa( log, "size .. #<uz_t>\n", bcore_tp_fastmap_s_size( map ) );
    st_s_push_fa( log, "keys .. #<uz_t>\n", bcore_tp_fastmap_s_keys( map ) );

    uz_t bcore_tp_fastmap_s_keys( bcore_tp_fastmap_s* o );


    bcore_tp_fastmap_s_discard( map );
    bcore_free( kvbuf );

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

