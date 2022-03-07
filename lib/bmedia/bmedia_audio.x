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

//----------------------------------------------------------------------------------------------------------------------

/// converts buffer channel into vector with value range [ -1.0 , +1.0 [
func (:buffer_s) get_vf2
{
    vec.set_size( o.size / o.channels );
    f2_t f = 1.0 / 32768;
    for( sz_t i = 0; i < vec.size; i++ ) vec.[ i ] = o.[ i * o.channels + channel ] * f;
    = vec;
}

//----------------------------------------------------------------------------------------------------------------------

/// vector into buffer value for given channel (overflow-check by truncation)
func (:buffer_s) set_from_vf2
{
    sz_t k = 0;
    for( sz_t i = channel; i < o.size; i += o.channels )
    {
        f2_t v = k < vec.size ? vec.[ k++ ] : 0;
        v = f2_min( 1.0, f2_max( -1.0, v ));
        o.[ i ] = lrintf( f2_min( 32767, f2_max( -32768, v * 32768.0 ) ) );
    }
    = o;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) setup_frames
{
    o.adl.clear();
    o.channels = channels;
    o.rate = rate;
    while( frames > 0 )
    {
        sz_t buffer_frames = s3_min( frames, o.preferred_frames_per_buffer );
        m :buffer_s* buffer = o.push_empty_buffer();
        buffer.set_frames( buffer_frames, channels );
        frames -= buffer_frames;
    }
    = o;
}

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) setup_diff
{
    :sequence_indexer_s* idx_a = a.create_indexer()^;
    :sequence_indexer_s* idx_b = b.create_indexer()^;
    o.setup_frames( a.channels, a.rate, s3_min( idx_a.size, idx_b.size ) );
    m :sequence_indexer_s* idx_o = o.create_indexer()^;
    if( idx_o.size == 0 ) = o;
    ASSERT( a.channels == b.channels );
    ASSERT( a.rate == b.rate );
    sz_t buf_frames = o.preferred_frames_per_buffer;
    sz_t buf_size = buf_frames * o.channels;

    :buffer_s^ buf_a.set_size( buf_size, o.channels );
    :buffer_s^ buf_b.set_size( buf_size, o.channels );
    :buffer_s^ buf_o.set_size( buf_size, o.channels );

    for( s3_t index = 0; index < idx_o.size; index += buf_frames )
    {
        idx_a.get_buffer( buf_a, index, buf_frames );
        idx_b.get_buffer( buf_b, index, buf_frames );
        for( sz_t i = 0; i < buf_o.size; i++ )
        {
            s2_t diff = buf_a.[ i ] - buf_b.[ i ];
            buf_o.[ i ] = s2_max( -32768, s2_min( 32767, diff ) );
        }
        idx_o.set_from_buffer( buf_o, index );
    }

    = o;
}

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) push_empty_buffer
{
    if( o.size == o.adl.size )
    {
        sz_t old_adl_size = o.adl.size;
        o.adl.set_size( sz_max( 1, old_adl_size * 2 ) );

        /// unwrap deque
        sz_t size_wrap = o.first + o.size - old_adl_size;
        for( sz_t i = 0; i < size_wrap; i++ )
        {
            o.adl.[ i + old_adl_size ] = o.adl.[ i ];
            o.adl.[ i ] = NULL;
        }
    }

    sz_t index = ( o.first + o.size ) % o.adl.size;
    assert( o.adl.[ index ] == NULL );
    o.size++;

    d$* buffer = :buffer_s!;
    buffer.channels = o.channels;
    = o.adl.[ index ] = buffer;
}

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) pop_first_buffer
{
    d :buffer_s* buffer = NULL;

    if( o.size > 0 )
    {
        buffer = o.adl.[ o.first ];
        o.adl.[ o.first ] = NULL;
        o.first = ( o.first + 1 ) % o.adl.size;
        o.size--;
    }

    = buffer;
}

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) sum_buffer_size
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o.size; i++ ) sum += o.buffer_c( i ).size;
    = sum;
}

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) wav_to_sink
{
    if( o.channels <= 0 ) = bcore_error_push_fa( TYPEOF_general_error, "channels <= 0" );
    if( o.rate     <= 0 ) = bcore_error_push_fa( TYPEOF_general_error, "rate <= 0" );

    /// file header
    sz_t sum_buffer_size = o.sum_buffer_size();
    u2_t chunk_size = sum_buffer_size * sizeof( s1_t ) + 44 - 8; /// file size in bytes - 8
    sink.push_sc( "RIFF" );
    sink.push_data( chunk_size.1, sizeof( chunk_size ) );
    sink.push_sc( "WAVE" );

    /// format header
    u2_t format_length    = 16; // remaining size of format header
    u1_t format_tag       = 1;  // format tag: 1 == 'PCM'
    u1_t channels         = o.channels;
    u2_t rate             = o.rate;
    u2_t bytes_per_second = o.channels * 2 * o.rate;
    u1_t block_alignment  = o.channels * 2;
    u1_t bits_per_sample  = sizeof( s1_t ) * 8;
    sink.push_sc( "fmt " ); // 'fmt' plus one space
    sink.push_data( format_length.1, sizeof( format_length ) );
    sink.push_data( format_tag.1, sizeof( format_tag ) );
    sink.push_data( channels.1, sizeof( channels ) );
    sink.push_data( rate.1, sizeof( rate ) );
    sink.push_data( bytes_per_second.1, sizeof( bytes_per_second ) );
    sink.push_data( block_alignment.1, sizeof( block_alignment ) );
    sink.push_data( bits_per_sample.1, sizeof( bits_per_sample ) );

    /// data header
    u2_t data_length = sum_buffer_size * sizeof( s1_t );
    sink.push_sc( "data" );
    sink.push_data( data_length.1, sizeof( data_length ) );

    for( sz_t i = 0; i < o.size; i++ )
    {
        :buffer_s* buffer = o.buffer_c( i );
        sink.push_data( buffer.data, sizeof( s1_t ) * buffer.size );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) wav_to_file
{
    m x_sink* sink = x_sink_create_from_file( path )^;
    if( !sink ) = bcore_error_push_fa( TYPEOF_general_error, "Could not open file path '#<sc_t>'.", path );
    = o.wav_to_sink( sink );
}

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) wav_from_source
{
    /// file header
    u2_t chunk_size = 0;
    source.parse_fa( "RIFF" );
    if( source.get_data( chunk_size.1, sizeof( chunk_size ) ) != sizeof( chunk_size ) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: File format error." );
    }
    source.parse_fa( "WAVE" );

    u2_t format_length    = 0; // remaining size of format header
    u1_t format_tag       = 0; // format tag: 1 == 'PCM'
    u1_t channels         = 0;
    u2_t rate             = 0;
    u2_t bytes_per_second = 0;
    u1_t block_alignment  = 0;
    u1_t bits_per_sample  = 0;
    source.parse_fa( "fmt " );
    if( source.get_data( format_length.1, sizeof( format_length ) ) != sizeof( format_length ) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (format_length) File format error." );
    }

    if( source.get_data( format_tag.1, sizeof( format_tag ) ) != sizeof( format_tag ) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (format_tag) File format error." );
    }

    if( source.get_data( channels.1, sizeof( channels ) ) != sizeof( channels ) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (channels) File format error." );
    }

    if( source.get_data( rate.1, sizeof( rate ) ) != sizeof( rate ) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (rate) File format error." );
    }

    if( source.get_data( bytes_per_second.1, sizeof( bytes_per_second ) ) != sizeof( bytes_per_second ) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (bytes_per_second) File format error." );
    }

    if( source.get_data( block_alignment.1, sizeof( block_alignment ) ) != sizeof( block_alignment ) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (block_alignment) File format error." );
    }

    if( source.get_data( bits_per_sample.1, sizeof( bits_per_sample ) ) != sizeof( bits_per_sample ) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (bits_per_sample) File format error." );
    }

    if( format_length != 16 )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (format_length) Invalid value (#<u2_t>).", format_length );
    }

    if( format_tag != 1 )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (format_tag) Invalid value (#<u1_t>).", format_tag );
    }

    if( channels < 1 || channels > 256 )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (channels) Unlikely value (#<u1_t>). Probably invalid.", channels );
    }

    if( rate == 0 )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: Rate is zero." );
    }

    o.clear();
    o.rate = rate;
    o.channels = channels;

    if( bits_per_sample != sizeof( s1_t ) * 8 )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (bits_per_sample) Unhandled value (#<u1_t>)", bits_per_sample );
    }

    if( block_alignment != o.channels * 2 )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (bits_per_sample) Unhandled value (#<u1_t>", block_alignment );
    }

    u2_t data_length = 0;
    source.parse_fa( "data" );
    if( source.get_data( data_length.1, sizeof( data_length ) ) != sizeof( data_length ) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (data_length) File format error." );
    }

    if( data_length % ( o.channels * sizeof( s1_t ) ) != 0 )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: data_length (#<u2_t>) is not a multiple of frame size (#<sz_t>).", data_length, o.channels * sizeof( s1_t ) );
    }

    sz_t frames = data_length / ( o.channels * sizeof( s1_t ) );
    sz_t frames_per_buffer = o.preferred_frames_per_buffer;

    while( frames > 0 )
    {
        sz_t buffered_frames = frames < frames_per_buffer ? frames : frames_per_buffer;
        m :buffer_s* buffer = o.push_empty_buffer().set_size( buffered_frames * o.channels, o.channels );
        if( source.get_data( buffer.data, buffer.size * sizeof( s1_t ) ) != buffer.size * sizeof( s1_t ) )
        {
            = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: Unexpected end of stream." );
        }
        frames -= buffered_frames;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) wav_from_file
{
    m x_source* source = x_source_create_from_file( path )^;
    if( !source ) = bcore_error_push_fa( TYPEOF_general_error, "Could not open file path '#<sc_t>'.", path );
    = o.wav_from_source( source );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_iterator_s) setup =
{
    if( sequence )
    {
        o.sequence = sequence.cast( m :sequence_s* );
        o.channels = o.sequence.channels;
        o.buffer_index = 0;
        o.frame_index = 0;
        o.total_frames = o.sequence.sum_buffer_frames();
        o.global_frame_index = 0;
        o.eos = ( o.total_frames == 0 );
    }
    else
    {
        o.sequence = NULL;
        o.channels = 0;
        o.buffer_index = 0;
        o.frame_index = 0;
        o.total_frames = 0;
        o.global_frame_index = 0;
        o.eos = true;
    }
    = o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_iterator_s) reset =
{
    o.buffer_index = 0;
    o.frame_index = 0;
    o.eos = ( o.total_frames == 0 );
    o.global_frame_index = 0;
    = o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_iterator_s) move =
{
    sz_t frames_left = diff;
    while( frames_left > 0 )
    {
        if( o.eos ) = o;
        :buffer_s* buffer = o.sequence.buffer_c( o.buffer_index );
        sz_t bsize = buffer.size % o.channels;

        if( bsize - o.frame_index > frames_left )
        {
            o.frame_index += frames_left;
            o.global_frame_index += frames_left;
            frames_left = 0;
        }
        else
        {
            sz_t frames = bsize - o.frame_index;
            frames_left -= frames;
            o.global_frame_index += frames;
            o.buffer_index++;
            o.frame_index = 0;
            o.eos = ( o.buffer_index >= o.sequence.size );
        }
    }

    while( frames_left < 0 )
    {
        if( o.frame_index == 0 )
        {
            if( o.buffer_index == 0 ) = o;
            o.buffer_index--;
            :buffer_s* buffer = o.sequence.buffer_c( o.buffer_index );
            sz_t bsize = buffer.size % o.channels;
            o.frame_index = bsize;
            o.eos = false;
        }
        else
        {
            if( o.frame_index + frames_left >= 0 )
            {
                o.frame_index += frames_left;
                o.global_frame_index += frames_left;
                frames_left = 0;
            }
            else
            {
                o.global_frame_index -= o.frame_index;
                frames_left += o.frame_index;
                o.frame_index = 0;
            }
        }
    }
    = o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_iterator_s) go_to =
{
    = o.move( global_frame_index - o.global_frame_index );
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_iterator_s) get_frames =
{
    sz_t frames_left = frames;
    sz_t data_index = 0;
    while( frames_left > 0 )
    {
        if( !o.eos )
        {
            :buffer_s* buffer = o.sequence.buffer_c( o.buffer_index );
            sz_t buf_size = buffer.size - ( buffer.size % o.channels );

            sz_t buf_start = o.frame_index * o.channels;
            sz_t buf_end = sz_min( buf_size, buf_start + frames_left * o.channels );
            for( sz_t i = buf_start; i < buf_end; i++ ) data[ data_index++ ] = buffer.[ i ];
            sz_t frames_copied = ( buf_end - buf_start ) / o.channels;
            if( buf_end == buf_size )
            {
                o.buffer_index++;
                o.frame_index = 0;
                o.eos = ( o.buffer_index >= o.sequence.size );
            }
            else
            {
                o.frame_index += frames_copied;
            }
            frames_left -= frames_copied;
            o.global_frame_index += frames_copied;
        }
        else
        {
            sz_t pad_size = frames_left * o.channels;
            for( sz_t i = 0; i < pad_size; i++ ) data[ data_index++ ] = 0;
            frames_left = 0;
        }
    }
    = o;
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_iterator_s) get_buffer =
{
    buffer.set_size( frames * o.channels, o.channels );
    = o.get_frames( buffer.data, frames );
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_iterator_s) create_buffer =
{
    d :buffer_s* buffer = :buffer_s!;
    o.get_buffer( buffer, frames );
    = buffer;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// setup; sets index to zero
func (:sequence_indexer_s) setup =
{
    o.sequence = sequence;
    o.channels = sequence.channels;
    bcore_arr_s3_s^ arr.set_size( sequence.size );
    foreach( m$* e in arr ) e.0 = sequence.buffer_c( __i ).size / o.channels;
    o.indexer.setup( arr );
    o.size = o.indexer.size;
    = o;
};

//----------------------------------------------------------------------------------------------------------------------

/// reads 'frames' from sequence at global index position. Out-of-range positions are filled with zeros.
func (:sequence_indexer_s) get_data =
{
    if( index < 0 )
    {
        sz_t size = sz_min( -index, frames );
        bcore_memset( data, 0, sizeof( s1_t ) * size * o.channels );
        frames -= size;
        index  += size;
        data   += size * o.channels;
    }

    bcore_indexer_io_s io = { 0 };

    while( frames > 0 && o.indexer.get_io( index, io ) )
    {
        s1_t* o_data = o.sequence.buffer_c( io.i ).data + io.o * o.channels;
        sz_t size = o.indexer.cs_arr.[ io.i ].s - io.o;
        size = sz_min( size, frames );
        bcore_memcpy( data, o_data, sizeof( s1_t ) * size * o.channels );
        frames -= size;
        index  += size;
        data   += size * o.channels;
    }

    if( frames > 0 ) bcore_memset( data, 0, sizeof( s1_t ) * frames * o.channels );
    = o;
};

//----------------------------------------------------------------------------------------------------------------------

/// sets 'frames' in sequence at global index position. Out-of-range positions are ignored.
func (:sequence_indexer_s) set_data =
{
    if( index < 0 )
    {
        sz_t size = sz_min( -index, frames );
        frames -= size;
        index  += size;
        data   += size * o.channels;
    }

    bcore_indexer_io_s io = { 0 };

    while( frames > 0 && o.indexer.get_io( index, io ) )
    {
        m s1_t* o_data = o.sequence.buffer_m( io.i ).data + io.o * o.channels;
        sz_t size = o.indexer.cs_arr.[ io.i ].s - io.o;
        size = sz_min( size, frames );
        bcore_memcpy( o_data, data, sizeof( s1_t ) * size * o.channels );
        frames -= size;
        index  += size;
        data   += size * o.channels;
    }

    = o;
};

//----------------------------------------------------------------------------------------------------------------------

/// obtains given number of frames; off-range frames are padded with zeros; allocates buffer
func (:sequence_indexer_s) get_buffer =
{
    o.get_data( buffer.set_size( frames * o.channels, o.channels ).data, index, frames );
    = buffer;
};

//----------------------------------------------------------------------------------------------------------------------

/// obtains given number of frames; off-range frames are padded with zeros; returns buffer of copied data
func (:sequence_indexer_s) create_buffer =
{
    d$* buffer = :buffer_s!;
    = o.get_buffer( buffer, index, frames );
};

//----------------------------------------------------------------------------------------------------------------------

/// obtains given number of frames; off-range frames are padded with zeros; allocates buffer
func (:sequence_indexer_s) set_from_buffer =
{
    if( buffer.size > 0 )
    {
        ASSERT( o.channels == buffer.channels );
        o.set_data( buffer.data, index, buffer.size / o.channels );
    }
    = o;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


