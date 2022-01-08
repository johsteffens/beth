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

/** Indexer is a support object for fast (O(1)) addressing of a data set distributed across
 *  a list of containers of arbitrary size. A typical use case is accessing audio frames
 *  in an audio sequence.
 *
 *  Problem:
 *  Given a list of containers each representing a list of objects.
 *  We want to address each object by a global index (expressed as single numeric value)
 *  in O(1) time.
 *
 *  Method:
 *  A sub-index of container-index and offset is created. The global index is split into an
 *  address for the sub-index and residual offset from which the correct container and its
 *  offset is calculated.
 */

#ifndef BCORE_INDEXER_H
#define BCORE_INDEXER_H

#include "bcore.xo.h"

XOILA_DEFINE_GROUP( bcore_indexer, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// index (i) an offset (o) for a container base
stamp :io_s = obliv { s3_t i; s3_t o; };
stamp :io_arr_s = x_array { :io_s []; };

/// cumulative index (i) and size (s) describing a container
stamp :cs_s = obliv { s3_t c; s3_t s; };
stamp :cs_arr_s = x_array { :cs_s []; };

stamp :s =
{
    :io_arr_s io_arr;
    :cs_arr_s cs_arr;
    s0_t mask_bits;
    s3_t size; // gloabl_size

    /// Creates the index. size_arr represents the container sizes.
    func (o setup( m@* o, bcore_arr_s3_s* size_arr )) =
    {
        o.size = 0;
        o.cs_arr.set_size( size_arr.size );
        foreach( m$* e in o.cs_arr )
        {
            e.c = o.size;
            e.s = size_arr.[ __i ];
            o.size += e.s;
        };

        sz_t io_size = 1;
        while( io_size < size_arr.size ) io_size <<= 1;
        o.mask_bits = 0;
        while( ( ( o.size - 1 ) >> o.mask_bits ) >= io_size ) o.mask_bits++;

        :io_s io = { 0 };
        o.io_arr.clear().set_size( io_size );
        foreach( m$* e in o.io_arr )
        {
            sz_t c = __i.cast( s3_t ) << o.mask_bits;
            if( c >= o.size ) break;
            io.o = c - o.cs_arr.[ io.i ].c;
            while( io.o >= o.cs_arr.[ io.i ].s )
            {
                io.o -= o.cs_arr.[ io.i ].s;
                io.i++;
            }
            e.0 = io;
        }

        return o;
    };

    /** Computes container-index and offset into the container.
     *  Returns false in case 'index' is out of range.
     */
    func (bl_t get_io( @* o, s3_t index, m :io_s* io )) =
    {
        if( index < 0 || index >= o.size ) return false;
        io.0 = o.io_arr.[ index >> o.mask_bits ];
        io.o += index & ( ( 1 << o.mask_bits ) - 1 );
        while( io.o >= o.cs_arr.[ io.i ].s )
        {
            io.o -= o.cs_arr.[ io.i ].s;
            io.i++;
        }
        return true;
    };
};

//----------------------------------------------------------------------------------------------------------------------

func (void selftest()) =
{
    bcore_prsg_lcg_u3_00_s^ prsg;

    sz_t n = 100;   // containers
    sz_t m = 1000;  // container size-range
    sz_t t = 10000; // test samples

    bcore_arr_s3_s^ arr.set_size( n );
    foreach( m$* e in arr ) { e.0 = prsg.gen_f3( 0.0, m ); }

    :s^ indexer.setup( arr );

    s3_t sum = 0;
    foreach( m$* e in arr ) { sum += e.0; }
    ASSERT( sum == indexer.size );

    for( sz_t i = 0; i < t; i++ )
    {
        s3_t c = prsg.gen_f3( -indexer.size, 2 * indexer.size );

        :io_s io0 = { 0 };
        s3_t sum = 0;
        foreach( $*e in arr )
        {
            if( sum + e.0 > c )
            {
                io0.i = __i;
                io0.o = c - sum;
                break;
            }
            sum += e.0;
        }

        :io_s io1;
        if( indexer.get_io( c, io1 ) )
        {
            if( io0.i != io1.i )
            {
                indexer.get_io( c, io1 );
            }
            ASSERT( io0.i == io1.i );
            ASSERT( io0.o == io1.o );
        }
        else
        {
            ASSERT( c < 0 || c >= indexer.size );
        }
    }

};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_INDEXER_H
