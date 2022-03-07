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

/** Hashmap generic framework */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:tp_st_s) (d st_s* create_st_status( c@* o ))
{
    d st_s* string = st_s_create();
    string.push_fa( "keys ........... #<sz_t>\n", o.keys() );
    string.push_fa( "nodes .......... #<sz_t>\n", o.size() );
    string.push_fa( "keys/nodes ..... #<f3_t>\n", o.size() > 0 ? ( f3_t )( o.keys() ) / o.size() : 0 );
    return string;
};

//----------------------------------------------------------------------------------------------------------------------

group :tp_test =
{
    stamp :kv_s = obliv { tp_t key; u3_t val; };
    stamp :kv_arr_s = x_array { :kv_s []; };
    stamp :val_s = { u3_t val; }; // aware value (used to create a fast testable hmap)
    stamp :map_s = x_hmap_tp trans(TE :val_s) { x_hmap_tp_s map; };

    //----------------------------------------------------------------------------------------------------------------------

    func (:map_s) (d st_s* create_st_status( c@* o ))
    {
        d st_s* string = st_s_create();
        string.push_fa( "keys ........... #<sz_t>\n", o.keys() );
        string.push_fa( "nodes .......... #<sz_t>\n", o.size() );
        string.push_fa( "keys/nodes ..... #<f3_t>\n", o.size() > 0 ? ( f3_t )( o.keys() ) / o.size() : 0 );
        return string;
    };

    //----------------------------------------------------------------------------------------------------------------------

    func (void selftest())
    {
        m $* log = x_sink_stdout();
        log.push_fa( "#p80'='{== #<sc_t>_selftest }\n", bnameof( :~ ) );

        sz_t cycles = 200000;

        m $* map    = :map_s!^;
        m $* kv_arr = :kv_arr_s!^;
        kv_arr.set_space( cycles );

        clock_t time = clock();
        log.push_fa( "Mixed access: " );

        :val_s^ mval;

        {
            u3_t rval1 = 1;
            u3_t rval2 = 12345;
            for( sz_t i = 0; i < cycles; i++ )
            {
                rval1 = bcore_lcg00_u3( rval1 );
                rval2 = bcore_lcg00_u3( rval2 );
                m$* kv = kv_arr.push();
                kv.key = ( tp_t )rval1;
                kv.val = rval2;
                mval.val = kv.val;
                map.set_c( kv.key, mval );

                // retrieve
                rval1 = bcore_lcg00_u3( rval1 );
                kv = kv_arr.[ rval1 % kv_arr.size ].1;

                u3_t val = map.c_get( kv.key ).val;
                if( kv.val != val ) ERR_fa( "value mismatch (#<u3_t> vs #<u3_t>)", kv.val, val );

                // delete
                rval1 = bcore_lcg00_u3( rval1 );
                if( ( ( rval1 >> 10 ) & 1 ) == 1 )
                {
                    rval1 = bcore_lcg00_u3( rval1 );
                    sz_t idx = rval1 % kv_arr.size;
                    $* kv = kv_arr.[ idx ];
                    if( !map.c_get( kv.key ) ) ERR_fa( "key #<tp_t> not found", kv.key );
                    map.remove( kv.key );
                    if( map.c_get( kv.key ) )  ERR_fa( "deleted key #<tp_t> still exists", kv.key );
                    kv_arr.[ idx ] = kv_arr.[ kv_arr.size - 1 ];
                    kv_arr.set_size( kv_arr.size - 1 );
                }
            }
        }

        time = clock() - time;
        log.push_fa( "#<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
        log.push_st_d( map.create_st_status() );

        time = clock();
        $* map_cloned = map.clone()^;
        time = clock() - time;
        log.push_fa( "Clone .......... #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );

        time = clock();
        if( bcore_compare_aware( map, map_cloned ) != 0 )
        {
            ERR( "comparison failed:\n%s", bcore_diff_aware( map, map_cloned )->sc );
        }

        time = clock() - time;
        log.push_fa( "Comparison ..... #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );

        {
            time = clock();
            m $* map_trans = :map_s!^;
            map_trans.cast( m x_stamp* ).set_sr( btypeof( "map" ), map.cast( m x_stamp* ).c_get_sr( btypeof( "map" ) ) );
            time = clock() - time;
            log.push_fa( "Stamp-assign ... #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
            time = clock();
            sz_t map_size = map.size();
            for( uz_t i = 0; i < map_size; i++ )
            {
                tp_t key = map.idx_key( i );
                if( key != 0 )
                {
                    ASSERT( map_trans.exists( key ) );
                    ASSERT( map.c_get( key ).val == map_trans.c_get( key ).val );
                }
            }
            time = clock() - time;
            log.push_fa( "verify assign .. #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
        }

        {
            time = clock();
            m $* map_trans = :map_s!^;
            d st_s* buf = st_s!;
            map.cast( x_bbml* ).to_sink( buf );
            map_trans.cast( m x_bbml* ).from_source( x_source_create_from_st_d( buf )^ );
            time = clock() - time;
            log.push_fa( "BBML Transfer .. #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
            time = clock();
            sz_t map_size = map.size();
            for( uz_t i = 0; i < map_size; i++ )
            {
                tp_t key = map.idx_key( i );
                if( key != 0 )
                {
                    ASSERT( map_trans.exists( key ) );
                    ASSERT( map.c_get( key ).val == map_trans.c_get( key ).val );
                }
            }
            time = clock() - time;
            log.push_fa( "verify transfer  #<f3_t>s\n", ( f3_t )time/CLOCKS_PER_SEC );
        }

        time = clock();
        log.push_fa( "\nRead-access of #<sz_t> keys: ", kv_arr.size );
        uz_t read_cycles = 20;
        for( uz_t j = 0; j < read_cycles; j++ )
        {
            for( uz_t i = 0; i < kv_arr.size; i++ )
            {
                u3_t val = map.c_get( kv_arr.[ i ].key ).val;
                if( val != kv_arr.[ i ].val ) ERR_fa( "value mismatch (#<u3_t> vs #<u3_t>)", val, kv_arr.[ i ].val );
            }
        }
        time = clock() - time;
        log.push_fa( "(#<f3_t> per access)\n", ( ( f3_t )time/CLOCKS_PER_SEC ) / ( kv_arr.size * read_cycles ) );

        log.push_fa( "\n" );
        log.push_fa( "Removal: " );
        time = clock();
        while( kv_arr.size )
        {
            $* kv = kv_arr.[ kv_arr.size - 1 ];
            if( !map.c_get( kv.key ) ) ERR_fa( "key #<tp_t> not found", kv.key );
            map.remove( kv.key );
            if( map.c_get( kv.key ) ) ERR_fa( "deleted key #<tp_t> still exists", kv.key );
            kv_arr.set_size( kv_arr.size - 1 );
        }
        time = clock() - time;
        log.push_fa( "(#<f3_t>s)\n", ( f3_t )time/CLOCKS_PER_SEC );
        log.push_st_d( map.create_st_status() );
        log.push_fa( "#r80{=}\n" );
    };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

