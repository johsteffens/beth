/** Author and Copyright 2021 Johannes Bernhard Steffens
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

#include "bcore_hmap_tp_st.h"
#include "bcore_spect_compare.h"

/**********************************************************************************************************************/
// selftest

#include <time.h>

//----------------------------------------------------------------------------------------------------------------------

static st_s* hmap_tp_st_s_status( bcore_hmap_tp_st_s* o )
{
    st_s* string = st_s_create();
    uz_t keys = bcore_hmap_tp_st_s_keys( o );
    st_s_pushf( string, "keys ........... %zu\n", keys );
    st_s_pushf( string, "nodes .......... %lu\n", o->map.size );
    st_s_pushf( string, "keys/nodes ..... %5.4f\n", o->map.size > 0 ? ( f3_t )( keys ) / o->map.size : 0 );
    return string;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_hmap_tp_st_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    st_s* log = st_s_createf( "== bcore_hmap_tp_st_selftest " );
    st_s_push_char_n( log, '=', 80 - log->size );
    st_s_push_char( log, '\n' );

    bcore_hmap_tp_st_s* map = bcore_life_s_push_aware( l, bcore_hmap_tp_st_s_create() );
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
            rval1 = bcore_lcg00_u3( rval1 );
            rval2 = bcore_lcg00_u3( rval2 );
            kv_s kv;
            kv.key = ( tp_t )rval1;
            kv.val = rval2;
            kvbuf[ kvbuf_size++ ] = kv;

            // set
            bcore_hmap_tp_st_s_set_d( map, kv.key, st_s_create_typed( TYPEOF_u3_t, &kv.val ) );

            // retrieve
            rval1 = bcore_lcg00_u3( rval1 );
            kv = kvbuf[ rval1 % kvbuf_size ];

            u3_t val = 0;
            st_s_parse_fa( bcore_hmap_tp_st_s_get( map, kv.key ), 0, -1, "#<u3_t*>", &val );
            if( kv.val != val ) ERR( "value mismatch (%lu vs %lu)", kv.val, val );

            // delete
            rval1 = bcore_lcg00_u3( rval1 );
            if( ( ( rval1 >> 10 ) & 1 ) == 1 )
            {
                rval1 = bcore_lcg00_u3( rval1 );
                uz_t idx = rval1 % kvbuf_size;
                kv_s kv = kvbuf[ idx ];
                if( !bcore_hmap_tp_st_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
                bcore_hmap_tp_st_s_remove( map, kv.key );
                if( bcore_hmap_tp_st_s_get(  map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
                kvbuf_size--;
                kvbuf[ idx ] = kvbuf[ kvbuf_size ];
            }
        }
    }

    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );
    st_s_push_st_d( log, hmap_tp_st_s_status( map ) );

    time = clock();
    bcore_hmap_tp_st_s* map2 = bcore_life_s_push_aware( l, bcore_hmap_tp_st_s_clone( map ) );
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
    bcore_hmap_tp_st_s* map3 = bcore_life_s_push_aware( l, bcore_hmap_tp_st_s_create() );
    bcore_via_a_nset( (bcore_via*)map3, typeof( "map" ), bcore_via_a_nget( (bcore_via*)map, typeof( "map" ) ) );
    time = clock() - time;
    st_s_pushf( log, "Via-assign ..... %5.3fs\n", ( double )time/CLOCKS_PER_SEC );
    time = clock();
    uz_t map_size = bcore_hmap_tp_st_s_size( map );
    for( uz_t i = 0; i < map_size; i++ )
    {
        tp_t key = bcore_hmap_tp_st_s_idx_key( map, i );
        if( key != 0 )
        {
            u3_t val1 = 0;
            st_s_parse_fa( bcore_hmap_tp_st_s_get( map, key ), 0, -1, "#<u3_t*>", &val1 );
            ASSERT( bcore_hmap_tp_st_s_exists( map3, key ) );
            u3_t val2 = 1;
            st_s_parse_fa( bcore_hmap_tp_st_s_get( map3, key ), 0, -1, "#<u3_t*>", &val2 );
            ASSERT( val1 == val2 );
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
            u3_t val = 0;
            st_s_parse_fa( bcore_hmap_tp_st_s_get( map, kvbuf[ i ].key ), 0, -1, "#<u3_t*>", &val );
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
        if( !bcore_hmap_tp_st_s_get( map, kv.key ) )  ERR( "key (%lu) not found", kv.key );
        bcore_hmap_tp_st_s_remove( map, kv.key );
        if( bcore_hmap_tp_st_s_get( map, kv.key ) )  ERR( "deleted key still exists (%lu)", kv.key );
    }
    time = clock() - time;
    st_s_pushf( log, "(%5.3fs)\n", ( double )time/CLOCKS_PER_SEC );
    st_s_push_st_d( log, hmap_tp_st_s_status( map ) );
    bcore_alloc( kvbuf, 0 );
    bcore_life_s_discard( l );

    st_s_push_char_n( log, '=', 80 );

    bcore_msg_fa( "#<sc_t>\n", log->sc );
    st_s_discard( log );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
