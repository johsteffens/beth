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
        o.[ i ] = f3_rs2( f2_min( 32767, f2_max( -32768, v * 32768.0 ) ) );
    }
    = o;
}

//----------------------------------------------------------------------------------------------------------------------

func (:buffer_s) scale_lin
{
    sz_t frames = o.frames();
    f3_t step = ( o.size > 0 ) ? ( end_factor - start_factor ) / frames : 0;
    f3_t f = start_factor;

    for( sz_t i = 0; i < frames; i++ )
    {
        for( sz_t j = 0; j < o.channels; j++ )
        {
            o.[ i * o.channels + j ] = f3_rs2( f3_min( 32767, f3_max( -32768, o.[ i * o.channels + j ] * f ) ) );
        }
        f += step;
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:buffer_s) scale_exp
{
    sz_t frames = o.frames();
    f3_t step = 0;
    if( frames > 0 && end_factor > 0 && start_factor > 0 )
    {
        step = f3_pow( end_factor / start_factor, 1.0 / frames );
    }
    f3_t f = start_factor;

    for( sz_t i = 0; i < frames; i++ )
    {
        for( sz_t j = 0; j < o.channels; j++ )
        {
            o.[ i * o.channels + j ] = f3_rs2( f3_min( 32767, f3_max( -32768, o.[ i * o.channels + j ] * f ) ) );
        }
        f *= step;
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:buffer_s) copy_spread_channels
{
    ASSERT( src.channels > 0 );
    sz_t frames = src.size / src.channels;
    o.set_frames( frames, dst_channels );
    for( sz_t i = 0; i < frames; i++ )
    {
        for( sz_t j = 0; j < dst_channels; j++ )
        {
            f3_t v = src.[ i * src.channels + ( ( j < src.channels ) ? j : src.channels - 1 ) ];
            o.[ i * o  .channels + j ] = f3_rs2( f3_min( 32767, f3_max( -32768, v * scale_factor ) ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:buffer_s) copy_resample
{
    ASSERT( frame_index >= 0 && frame_step > 0 );
    o.set_size( 0, src.channels );
    while( frame_index < src.frames() )
    {
        sz_t int_index = frame_index;
        f3_t offs = frame_index - int_index;
        for( sz_t j = 0; j < o.channels; j++ )
        {
            uz_t i0 = int_index * o.channels + j;
            uz_t i1 = i0 + o.channels;
            f3_t v0 = i0 < src.size ? src.[ i0 ] : 0;
            f3_t v1 = i1 < src.size ? src.[ i1 ] : ( next && ( i1 - src.size < next.size ) ) ? next.[ i1 - src.size ] : 0;
            f3_t v = v0 * ( 1.0 - offs ) + v1 * offs;
            o.push().0 = f3_rs2( f3_min( 32767, f3_max( -32768, v * scale_factor ) ) );
        }
        frame_index += frame_step;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) setup_frames
{
    o.clear();
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

func (:sequence_s) setup_fork_buffers
{
    o.setup( src.channels, src.rate );
    for( sz_t i = 0; i < src.size(); i++ ) o.push_buffer_d( src.m_buffer( i ).fork() );
}

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) push_buffer_d
{
    if( o.channels < 0 )
    {
        ASSERT( buffer.channels >= 0 );
        o.channels = buffer.channels;
    }
    else if( buffer.size == 0 )
    {
        buffer.channels = o.channels;
    }
    else
    {
        ASSERT( buffer.channels == o.channels );
    }

    = o.push_last_d( buffer );
}

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) sum_buffer_size
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o.size(); i++ ) sum += o.c_buffer( i ).size;
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

    for( sz_t i = 0; i < o.size(); i++ )
    {
        :buffer_s* buffer = o.c_buffer( i );
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

    if( source.parse_bl( "#?'LIST'" ) ) // list chunk (RIFF extension)
    {
        u2_t chunk_length = 0;

        if( source.get_data( chunk_length.1, sizeof( chunk_length ) ) != sizeof( chunk_length ) )
        {
            = bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: LIST-chunk: (chunk_length) File format error." );
        }

        /// we skip over this chunk
        for( sz_t i = 0; i < chunk_length; i++ ) source.get_char();
    }

    source.parse_bl( "#?'data'" );

    u2_t data_length = 0;

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

/// setup; sets index to zero
func (:sequence_indexer_s) setup
{
    o.sequence!.setup_fork_buffers( sequence );
    o.channels = sequence.channels;
    bcore_arr_s3_s^ arr.set_size( sequence.size() );
    foreach( m$* e in arr ) e.0 = sequence.c_buffer( __i ).size / o.channels;
    o.indexer.setup( arr );
    o.size = o.indexer.size;
    = o;
};

//----------------------------------------------------------------------------------------------------------------------

/// reads 'frames' from sequence at global index position. Out-of-range positions are filled with zeros.
func (:sequence_indexer_s) get_data
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
        s1_t* o_data = o.sequence.c_buffer( io.i ).data + io.o * o.channels;
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
func (:sequence_indexer_s) set_data
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
        m s1_t* o_data = o.sequence.m_buffer( io.i ).data + io.o * o.channels;
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
func (:sequence_indexer_s) get_buffer
{
    o.get_data( buffer.set_size( frames * o.channels, o.channels ).data, index, frames );
    = buffer;
};

//----------------------------------------------------------------------------------------------------------------------

/// obtains given number of frames; off-range frames are padded with zeros; returns buffer of copied data
func (:sequence_indexer_s) create_buffer
{
    d$* buffer = :buffer_s!;
    = o.get_buffer( buffer, index, frames );
};

//----------------------------------------------------------------------------------------------------------------------

/// obtains given number of frames; off-range frames are padded with zeros; allocates buffer
func (:sequence_indexer_s) set_from_buffer
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


