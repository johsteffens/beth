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

/**********************************************************************************************************************/
/// support

//----------------------------------------------------------------------------------------------------------------------

/// returns the minimum number of bits needed to fit the value v that noes not exceed n bits where n is a power of 2
func (sz_t min_bits( u3_t v, sz_t n )) =
{
    sz_t n1 = n >> 1;
    u3_t v1 = v >> n1;
    return ( v1 ? n1 : 0 ) + ( n1 ? :min_bits( v1 ? v1 : v, n1 ) : v.cast( sz_t ) );
};

//----------------------------------------------------------------------------------------------------------------------

/** singed codec
 *  Converts a singed integer into an unsigned representation that can be efficiently packed using min_bits function.
 */
func (u2_t u2_from_s2( s2_t s )) = { return ( s < 0 ) ? ( ( ( ~(u2_t)s ) << 1 ) + 1 ) : ( (u2_t)s << 1 ); };
func (u3_t u3_from_s3( s3_t s )) = { return ( s < 0 ) ? ( ( ( ~(u3_t)s ) << 1 ) + 1 ) : ( (u3_t)s << 1 ); };
func (s2_t s2_from_u2( u2_t u )) = { return ( u & 1 ) ? ~( ( u - 1 ) >> 1 ) : ( u >> 1 ); };
func (s3_t s3_from_u3( u3_t u )) = { return ( u & 1 ) ? ~( ( u - 1 ) >> 1 ) : ( u >> 1 ); };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// codec_s

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) clear =
{
    o.hist =< NULL;
    o.tree =< NULL;
    o.count_map =< NULL;
    o.leaf_index =< NULL;
    return o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) scan_start =
{
    o.clear();
    o.hist!.clear();
    return o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) scan_u2 =
{
    if( !o.hist ) ERR_fa( "Enclose scanning with 'scan_start()' ... 'scan_end()'." );
    o.hist.count( val );
    return o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) scan_u3 =
{
    return o.scan_u2( val ).scan_u2( val >> 32 );
};

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) scan_s2 = { return o.scan_u2( :u2_from_s2( val ) ); };
func (:codec_s) scan_s3 = { return o.scan_u3( :u3_from_s3( val ) ); };

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) scan_end =
{
    if( !o.hist ) ERR_fa( "No histogram. scan_end() called before scan_start()?." );
    o.count_map =< :count_map_s!.from_hist( o.hist ).sort( 1 );
    o.hist =< NULL;
    o.tree!.build( o.count_map, o.leaf_index! );
    return o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) encode_u2 =
{
    if( !o.tree ) ERR_fa( "No tree! Load a codec or run scanning first." );
    sz_t idx = o.leaf_index.get( val );
    if( idx == 0 ) ERR_fa( "An encoding for value #<u2_t> does not exist.", val );

    :node_s* node = o.tree.[ idx ];

    u3_t stack = 0;
    sz_t bits = 0;

    while( node.p )
    {
        bl_t bit = ( o.tree.[ node.p ].b1 == idx );
        idx = node.p;
        node = o.tree.[ idx ].1;

        stack = ( stack << 1 ) | ( ( bit ) ? 1 : 0 );
        bits++;
    }

    if( bits > 64 )  ERR_fa( "Encoding of value #<u2_t> consumes more than 64 bits.", val );

    for( sz_t i = 0; i < bits; i++ )
    {
        bit_buffer.push_bl( ( stack & 1 ) );
        stack = stack >> 1;
    }

    return o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) decode_u2 =
{
    if( !o.tree ) ERR_fa( "No tree! Load a codec or run scanning first." );
    :node_s* node = o.tree.[ o.tree.root_index() ];

    while( !iterator.eos() && !node.is_leaf() )
    {
        bl_t bit = iterator.read_bl();
        uz_t idx = ( bit ) ? node.b1 : node.b0;
        node = o.tree.[ idx ].1;
    }

    if( !node.is_leaf() )
    {
        ERR_fa( "Decoding error. Bit buffer appears corrupted." );
    }

    return node.v;
};

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) encode_u3 =
{
    return o.encode_u2( val, bit_buffer ).encode_u2( val >> 32, bit_buffer );
};

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) decode_u3 =
{
    return o.decode_u2( iterator ).cast( u3_t ) | ( o.decode_u2( iterator ).cast( u3_t ) << 32 );
};

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) encode_s2 = { return o.encode_u2( :u2_from_s2( val ), bit_buffer ); };
func (:codec_s) encode_s3 = { return o.encode_u3( :u3_from_s3( val ), bit_buffer ); };
func (:codec_s) decode_s2 = { return :s2_from_u2( o.decode_u2( iterator ) ); };
func (:codec_s) decode_s3 = { return :s3_from_u3( o.decode_u3( iterator ) ); };

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) encode =
{
    if( !o.count_map ) return o;
    o.count_map.encode( bit_buffer );
    return o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:codec_s) decode =
{
    o.clear();
    o.count_map!.decode( iterator );
    o.tree!.build( o.count_map, o.leaf_index! );
    return o;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bit_buffer_s

//----------------------------------------------------------------------------------------------------------------------

func (:bit_buffer_s) clear =
{
    o.cast( m x_array* ).clear();
    o.bits = 0;
    return o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:bit_buffer_s) push_bl =
{
    if( o.bits == o.size * 8 ) o.push().0 = 0;
    sz_t idx = o.bits / 8;
    if( bit )
    {
        u0_t m = 1 << ( o.bits - ( idx * 8 ) );
        o.[ idx ] |= m;
    }
    o.bits++;
    return o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:bit_buffer_s) push_u =
{
    for( sz_t i = 0; i < bits; i++ ) o.push_bl( ( val >> i ) & 1 );
    return o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:bit_buffer_s) push_packed_u =
{
    sz_t bits = sz_max( 1, :min_bits( val, 64 ) );
    return o.push_u( bits - 1, 6 ).push_u( val, bits );
};

//----------------------------------------------------------------------------------------------------------------------

func (:bit_buffer_iterator_s) read_bl =
{
    if( !o.bit_buffer ) ERR_fa( "No buffer assigned. Call setup first." );
    if( o.bit_index >= o.bit_buffer.bits ) ERR_fa( "Reading past end of buffer" );
    sz_t idx = o.bit_index / 8;
    u0_t m = 1 << ( o.bit_index - ( idx * 8 ) );
    o.bit_index++;
    return ( o.bit_buffer.[ idx ] & m ) ? true : false;
};

//----------------------------------------------------------------------------------------------------------------------

func (:bit_buffer_iterator_s) read_u =
{
    u3_t val = 0;
    for( sz_t i = 0; i < bits; i++ ) val = val | ( o.read_bl().cast( u3_t ) << i );
    return val;
};

//----------------------------------------------------------------------------------------------------------------------

func (:bit_buffer_iterator_s) read_packed_u =
{
    return o.read_u( o.read_u( 6 ) + 1 );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :hist_s =
{
    bcore_hmap_tpuz_s hmap_tpuz;
    func (o clear( m@* o )) = { o.hmap_tpuz.clear(); return o; };
    func (o count( m@* o, u2_t val )) = { o.hmap_tpuz.fget( o.key( val ), 0 ).0++; return o; };
    func (uz_t get( @* o, u2_t val )) = { uz_t* p = o.hmap_tpuz.get( o.key( val ) ); return p ? p.0 : 0; };

    func (tp_t key( @* o, u2_t val )) = { return val.cast( tp_t ) + 1; };
    func (u2_t val( @* o, tp_t key )) = { return key - 1; };
};

//----------------------------------------------------------------------------------------------------------------------

stamp :index_s =
{
    bcore_hmap_tpuz_s hmap_tpuz;
    func (tp_t key( @* o, u2_t val )) = { return val.cast( tp_t ) + 1; };
    func (o clear( m@* o )) = { o.hmap_tpuz.clear(); return o; };
    func (o set( m@* o, u2_t val, uz_t index )) = { o.hmap_tpuz.set( o.key( val ), index ); return o; };
    func (uz_t get( @* o, u2_t val )) = { uz_t* p = o.hmap_tpuz.get( o.key( val ) ); return p ? p.0 : 0; };
};

//----------------------------------------------------------------------------------------------------------------------

stamp :node_s = obliv
{
    u3_t c;  // count (frequency)
    uz_t p;  // parent node or 0 (for root)
    uz_t b0; // 0-branch-index or 0 (for leaf)
    uz_t b1; // 1-branch-index or 0 (for leaf)
    u2_t v;  // value
    func (bl_t is_leaf( @* o ) ) = { return o.b0 == 0; };
    func (bl_t is_root( @* o ) ) = { return o.p  == 0; };
};

//----------------------------------------------------------------------------------------------------------------------

stamp :count_node_s = obliv
{
    u3_t c;
    u2_t v;
    func (bl_t is_equal( @*o, @*b )) = { return ( o.c == b.c ) && ( o.v == b.v ); };
};

stamp :count_map_s = x_array
{
    :count_node_s [];
    func (o from_hist( m@* o, :hist_s* hist ) ) =
    {
        o.clear();
        for( sz_t i = 0; i < hist.hmap_tpuz.size; i++ )
        {
            tp_t key = hist.hmap_tpuz.idx_key( i );
            if( key > 0 )
            {
                m :count_node_s* node = o.push();
                node.c = hist.hmap_tpuz.idx_val( i );
                node.v = hist.val( key );
            }
        }
        return o;
    };

    func (bl_t is_sorted( @* o )) =
    {
        for( sz_t i = 1; i < o.size; i++ ) if( o.[ i - 1 ].c > o.[ i ].c ) return false;
        return true;
    };

    func (bl_t is_equal( @* o, @* b )) =
    {
        if( o.size != b.size ) return false;
        for( sz_t i = 1; i < o.size; i++ ) if( !o.[ i ].is_equal( b.[ i ] ) ) return false;
        return true;
    };
};

//----------------------------------------------------------------------------------------------------------------------

func (:count_map_s) (o encode( m@* o, m :bit_buffer_s* out )) =
{
    if( !o.is_sorted() ) ERR_fa( "Map is not sorted." );

    out.push_packed_u( o.size );

    sz_t idx = 0;
    u3_t count = 0;

    {
        for( ; idx < o.size && o.[ idx ].c == 1 ; idx++ ) count++;
        out.push_packed_u( count );
    }

    sz_t bits = 2;
    while( idx < o.size )
    {
        count = 0;
        for( sz_t i = idx; i < o.size && :min_bits( o.[ i ].c, 64 ) == bits ; i++ ) count++;
        if( count > 0 )
        {
            out.push_u( bits - 1, 6 );
            out.push_packed_u( count );
            for( sz_t i = 0; i < count; i++ ) out.push_u( o.[ idx + i ].c, bits - 1 );
        }
        idx += count;
        bits++;
    }

    sz_t max_bits = 0;
    for( sz_t i = 0; i < o.size; i++ ) max_bits = sz_max( max_bits, :min_bits( o.[ i ].v, 32 ) );
    out.push_u( max_bits, 6 );
    for( sz_t i = 0; i < o.size; i++ ) out.push_u( o.[ i ].v, max_bits );

    return o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:count_map_s) (o decode( m@* o, m :bit_buffer_iterator_s* in )) =
{
    o.clear();
    sz_t size = in.read_packed_u();
    o.set_size( size );

    sz_t idx = 0;
    u3_t count = in.read_packed_u();
    ASSERT( count <= o.size );
    for( sz_t i = 0; i < count; i++ ) o.[ idx++ ].c = 1;

    while( idx < o.size )
    {
        sz_t bits_m1 = in.read_u( 6 );
        u3_t count = in.read_packed_u();
        ASSERT( idx + count <= o.size );
        for( sz_t i = 0; i < count; i++ ) o.[ idx + i ].c = in.read_u( bits_m1 ) + ( 1ull << bits_m1 );
        idx += count;
    }

    sz_t max_bits = in.read_u( 6 );
    for( sz_t i = 0; i < o.size; i++ ) o.[ i ].v = in.read_u( max_bits );

    return o;
};

//----------------------------------------------------------------------------------------------------------------------

stamp :tree_s = x_array
{
    :node_s [];

    /// builds tree and creates an optional leaf_index and root_index into tree
    func (o build( m@* o, :count_map_s* count_map, m :index_s* leaf_index )) =
    {
        o.set_size( 1 );
        for( sz_t i = 0; i < count_map.size; i++ )
        {
            m :node_s* node = o.push();
            node.c = count_map.[ i ].c;
            node.v = count_map.[ i ].v;
            if( leaf_index ) leaf_index.set( node.v, o.size - 1 );
        }

        sz_t idx_a = 1;
        sz_t end_a = o.size;
        sz_t idx_b = o.size;
        while( end_a - idx_a + o.size - idx_b >= 2 )
        {
            u3_t b1 = ( idx_a < end_a && ( ( idx_b == o.size ) || ( o.[ idx_a ].c <= o.[ idx_b ].c ) ) ) ? idx_a++ : idx_b++;
            u3_t b0 = ( idx_a < end_a && ( ( idx_b == o.size ) || ( o.[ idx_a ].c <= o.[ idx_b ].c ) ) ) ? idx_a++ : idx_b++;
            o.[ b1 ].p = o.[ b0 ].p = o.size;
            m :node_s* node = o.push();
            node.b1 = b1;
            node.b0 = b0;
            node.c = o.[ b1 ].c + o.[ b0 ].c;
        }

        return o;
    };

    func (sz_t root_index( @* o )) = { return o.size - 1; };
};

//----------------------------------------------------------------------------------------------------------------------

func (void selftest()) =
{
    :codec_s^ codec;

    bcore_prsg_lcg_u3_00_s^ prsg;

    sz_t n = 100000;

    bcore_arr_s3_s^ arr_s3;
    for( sz_t i = 0; i < n; i++ )
    {
        f3_t v = prsg.gen_f3( 0.0, 1.0 );
        s3_t x = pow( sin( v * 3.141 * 2.0 ), 10 + 1 ) * 1000;
        arr_s3.push( x );
    }

    codec.scan_start();
    for( sz_t i = 0; i < n; i++ ) codec.scan_s3( arr_s3.[ i ] );
    codec.scan_end();

    :bit_buffer_s^ bit_buffer;

    codec.encode( bit_buffer );
    for( sz_t i = 0; i < n; i++ ) codec.encode_s3( arr_s3.[ i ], bit_buffer );

    :bit_buffer_iterator_s^ iterator.setup( bit_buffer );

    :codec_s^ codec2.decode( iterator );
    for( sz_t i = 0; i < n; i++ ) ASSERT( codec2.decode_s3( iterator ) == arr_s3.[ i ] );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

