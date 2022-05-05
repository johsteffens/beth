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

/** Efficient Huffman CODEC implementation for a series of signed or unsigned integers.
 *  Complexity: O(n1*log(n2)) (Worst case for scanning, encoding and decoding)
 *  Where n1 is the number of data samples and n2 the number of different
 *  data values.
 *
 *  Choose a suitable data type from s2_t, s3_t, u2_t, u3_t.
 *  Only use  s3_t|u3_t when the original data values may exceed 32 bits.
 *  Usage: See example below.
 *
 *  Note:
 *  Decoding a corrupted bit_buffer causes runtime errors.
 *  Suggest using hashed-envelopes and re-synchronization points
 *  for corruption detection and recovery from the carrier.
 *
 *  Do not use function itr.eos() to retrieve the number of samples.
 *  Reason: Certain data constellations may generate no footprint in the bit buffer.
 *  Hence, eos can be reached before all samples were decoded.
 */

/** Example:

// Encoding:
    bcore_huffman_codec_s^ codec;
    codec.scan_start();
    for( sz_t i = 0; i < size; i++ ) codec.scan_s2( data[ i ] );
    codec.scan_end();
    bcore_huffman_bit_buffer_s^ buf;
    codec.encode( buf );
    buf.push_packed_u3( size );
    for( sz_t i = 0; i < size; i++ ) codec.encode_s2( data[ i ], buf );
    buf.cast( x_bbml_s* ).to_sink( my_sink );

// Decoding:
    bcore_huffman_bit_buffer_s^ buf.cast( x_bbml ).from_source( my_source );
    bcore_huffman_bit_buffer_iterator_s^ itr.setup( buf );
    bcore_huffman_codec_s^ codec.decode( itr );
    sz_t size = itr.read_packed_u3();
    for( sz_t i = 0; i < size; i++ ) data[ i ] = codec.decode_s2( itr );

*/

#ifndef BCORE_HUFFMAN_H
#define BCORE_HUFFMAN_H

#include "bcore_hmap.h"
#include "bcore.xo.h"

XOILA_DEFINE_GROUP( bcore_huffman, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :codec_s =
{
    :hist_s       => hist;
    :count_map_s  => count_map;
    :tree_s       => tree;
    :index_s      => leaf_index;

    func o clear( m@* o );

    func o scan_start( m@* o );

    // For optimal performance use best fitting function for your input data
    func o scan_u2( m@* o, u2_t val );
    func o scan_u3( m@* o, u3_t val );
    func o scan_s2( m@* o, s2_t val );
    func o scan_s3( m@* o, s3_t val );

    func o scan_end( m@* o );

    // For optimal performance use best fitting function for your input data
    func o    encode_u2( @* o, u2_t val, m :bit_buffer_s* bit_buffer );
    func o    encode_u3( @* o, u3_t val, m :bit_buffer_s* bit_buffer );
    func o    encode_s2( @* o, s2_t val, m :bit_buffer_s* bit_buffer );
    func o    encode_s3( @* o, s3_t val, m :bit_buffer_s* bit_buffer );
    func u2_t decode_u2( @* o,           m :bit_buffer_iterator_s* iterator );
    func u3_t decode_u3( @* o,           m :bit_buffer_iterator_s* iterator );
    func s2_t decode_s2( @* o,           m :bit_buffer_iterator_s* iterator );
    func s3_t decode_s3( @* o,           m :bit_buffer_iterator_s* iterator );

    // en/decodes codec
    func o encode(  @* o, m :bit_buffer_s* bit_buffer );
    func o decode( m@* o, m :bit_buffer_iterator_s* iterator );
};

//----------------------------------------------------------------------------------------------------------------------

/// Bit buffer. Stores compacted data.
stamp :bit_buffer_s = x_array
{
    u3_t bits;
    u0_t [];

    func o clear( m@* o );

    // Low-level write functions. See :bit_buffer_iterator_s for corresponding read functions.

    /// Writes one bit
    func o push_bl( m@* o, bl_t bit );

    /// Writes an unsigned integer with a specified number of bits
    func o push_u3( m@* o, u3_t val, sz_t bits );

    /// Writes an unsigned integer. Stores number of used bits eliminating leading zeros. (7 ... 70 bits)
    func o push_packed_u3( m@* o, u3_t val );

    /// Writes a signed integer as two-s complement with a specified number of bits
    func o push_s3( m@* o, s3_t val, sz_t bits );

    /// Writes a signed integer. Stores number of used bits eliminating leading zeros. (7 ... 70 bits); Uses one extra bit for the sign
    func o push_packed_s3( m@* o, s3_t val );

    /// Writes a float. Accuracy bits used to loss-encode the mantissa, exponent is encoded lossless
    func o push_f3( m@* o, f3_t v, sz_t bits );

    /// Appends a bit buffer
    func o push_bit_buffer( m@* o, @* src );

    /// computes a hash value from content
    func tp_t get_hash( @* o );

    /// Compact I/O
    func x_bcml.bcml_body_to_sink;
    func x_bcml.bcml_body_from_source;
    func x_bbml.bbml_body_to_sink o.bcml_body_to_sink( sink );
    func x_bbml.bbml_body_from_source = o.bcml_body_from_source( source );
};

//----------------------------------------------------------------------------------------------------------------------

/// Bit buffer iterator. Used to read from a bit buffer.
stamp :bit_buffer_iterator_s =
{
    hidden :bit_buffer_s* bit_buffer;
    sz_t bit_index;

    /// Resets read index
    func o reset( m@* o ) { o.bit_index = 0; = o; }

    /// Resets and assigns a bit buffer
    func o setup( m@* o, :bit_buffer_s* bit_buffer ) { o.reset(); o.bit_buffer = bit_buffer.cast( m$* ); = o; }

    // Low-level read functions.

    /// Reads one bit; complementary to :bit_buffer_s.push_bl
    func bl_t read_bl( m@* o );

    /// Reads an unsigned integer a specified number of bits; complementary to :bit_buffer_s.push_u3
    func u3_t read_u3( m@* o, sz_t bits );

    /// Reads a compacted unsigned integer; complementary to :bit_buffer_s.push_packed_u3
    func u3_t read_packed_u3( m@* o );

    /// Reads an signed integer as two's complement; complementary to :bit_buffer_s.push_s3
    func s3_t read_s3( m@* o, sz_t bits );

    /// Reads a compacted signed integer; complementary to :bit_buffer_s.push_packed_s3
    func u3_t read_packed_s3( m@* o );

    /// Reads a float. Accuracy bits used to loss-encode the mantissa, exponent is encoded lossless; complementary to :bit_buffer_s.push_f3
    func f3_t read_f3( m@* o, sz_t bits );

    /** Read index reached end of buffer.
     *  Note:
     *    Only use with low level read functions. Higher level encodings (codec) might not
     *    always leave a footprint in bit_buffer.
     */
    func bl_t eos( m@* o ) { = o.bit_index >= o.bit_buffer.bits; }
}

//----------------------------------------------------------------------------------------------------------------------

embed "bcore_huffman.x";

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_HUFFMAN_H
