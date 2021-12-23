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

/** Huffman codec
 */

#ifndef BCORE_X_HUFFMAN_H
#define BCORE_X_HUFFMAN_H

#include "bcore_hmap.h"
#include "bcore_x_stamp.h"
#include "bcore_x_array.h"
#include "bcore.xo.h"

XOILA_DEFINE_GROUP( x_huffman, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :codec_s =
{
    :hist_s       => hist;
    :count_map_s  => count_map;
    :tree_s       => tree;
    :index_s      => leaf_index;

    func (o clear( m@* o ));

    func (o scan_start( m@* o ));

    // use best fitting function for your input data
    func (o scan_u2( m@* o, u2_t val ));
    func (o scan_u3( m@* o, u3_t val ));
    func (o scan_s2( m@* o, s2_t val ));
    func (o scan_s3( m@* o, s3_t val ));

    func (o scan_end( m@* o ));

    // use best fitting function for your input data
    func (o    encode_u2( @* o, u2_t val, m :bit_buffer_s* bit_buffer ));
    func (o    encode_u3( @* o, u3_t val, m :bit_buffer_s* bit_buffer ));
    func (o    encode_s2( @* o, s2_t val, m :bit_buffer_s* bit_buffer ));
    func (o    encode_s3( @* o, s3_t val, m :bit_buffer_s* bit_buffer ));
    func (u2_t decode_u2( @* o,           m :bit_buffer_s* bit_buffer ));
    func (u3_t decode_u3( @* o,           m :bit_buffer_s* bit_buffer ));
    func (s2_t decode_s2( @* o,           m :bit_buffer_s* bit_buffer ));
    func (s3_t decode_s3( @* o,           m :bit_buffer_s* bit_buffer ));

    // en/decodes codec
    func (o encode(  @* o, m :bit_buffer_s* bit_buffer ));
    func (o decode( m@* o, m :bit_buffer_s* bit_buffer ));
};

//----------------------------------------------------------------------------------------------------------------------

stamp :bit_buffer_s = x_array
{
    u3_t bits;
    hidden sz_t read_bit_index;
    u0_t [];

    /// Resets read index
    func (o reset( m@* o ));

    /** Read index reached end of buffer.
     *  Note:
     *    Do not use this function as indicator that all compressed data have been decoded
     *    because this will yield no information in case the entire scan consists yields
     *    only one value.
     */
    func (bl_t eos( m@* o ));

    func (o clear( m@* o ));
    func (o push_bl( m@* o, bl_t bit ));

    func (bl_t read_bl( m@* o ));
};

//----------------------------------------------------------------------------------------------------------------------

embed "bcore_x_huffman.x";

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_HUFFMAN_H
