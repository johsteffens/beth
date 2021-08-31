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

func (:sequence_s) push_buffer =
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

    return o.adl.[ index ] = :buffer_s!;
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) pop_first_buffer =
{
    d :buffer_s* buffer = NULL;

    if( o.size > 0 )
    {
        buffer = o.adl.[ o.first ];
        o.adl.[ o.first ] = NULL;
        o.first = ( o.first + 1 ) % o.adl.size;
        o.size--;
    }

    return buffer;
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) sum_buffer_size =
{
    sz_t sum = 0;
    for( sz_t i = 0; i < o.size; i++ ) sum += o.buffer_c( i ).size;
    return sum;
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) wav_to_sink =
{
    if( o.channels <= 0 ) return bcore_error_push_fa( TYPEOF_general_error, "channels <= 0" );
    if( o.rate     <= 0 ) return bcore_error_push_fa( TYPEOF_general_error, "rate <= 0" );

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

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) wav_to_file =
{
    m x_sink* sink = x_sink_create_from_file( path )^;
    if( !sink ) return bcore_error_push_fa( TYPEOF_general_error, "Could not open file path '#<sc_t>'.", path );
    return o.wav_to_sink( sink );
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) wav_from_source =
{
    /// file header
    u2_t chunk_size = 0;
    source.parse_fa( "RIFF" );
    if( source.get_data( chunk_size.1, sizeof( chunk_size ) ) != sizeof( chunk_size ) )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: File format error." );
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
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (format_length) File format error." );
    }

    if( source.get_data( format_tag.1, sizeof( format_tag ) ) != sizeof( format_tag ) )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (format_tag) File format error." );
    }

    if( source.get_data( channels.1, sizeof( channels ) ) != sizeof( channels ) )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (channels) File format error." );
    }

    if( source.get_data( rate.1, sizeof( rate ) ) != sizeof( rate ) )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (rate) File format error." );
    }

    if( source.get_data( bytes_per_second.1, sizeof( bytes_per_second ) ) != sizeof( bytes_per_second ) )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (bytes_per_second) File format error." );
    }

    if( source.get_data( block_alignment.1, sizeof( block_alignment ) ) != sizeof( block_alignment ) )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (block_alignment) File format error." );
    }

    if( source.get_data( bits_per_sample.1, sizeof( bits_per_sample ) ) != sizeof( bits_per_sample ) )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (bits_per_sample) File format error." );
    }

    if( format_length != 16 )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (format_length) Invalid value (#<u2_t>).", format_length );
    }

    if( format_tag != 1 )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (format_tag) Invalid value (#<u1_t>).", format_tag );
    }

    if( channels < 1 || channels > 256 )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (channels) Suspicious value (#<u1_t>). Probably invalid.", channels );
    }

    if( rate == 0 )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: Rate is zero." );
    }

    o.clear();
    o.rate = rate;
    o.channels = channels;

    if( bits_per_sample != sizeof( s1_t ) * 8 )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (bits_per_sample) Unhandled value (#<u1_t>)", bits_per_sample );
    }

    if( block_alignment != o.channels * 2 )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (bits_per_sample) Unhandled value (#<u1_t>", block_alignment );
    }

    u2_t data_length = 0;
    source.parse_fa( "data" );
    if( source.get_data( data_length.1, sizeof( data_length ) ) != sizeof( data_length ) )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: (data_length) File format error." );
    }

    if( data_length % ( o.channels * sizeof( s1_t ) ) != 0 )
    {
        return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: data_length (#<u2_t>) is not a multiple of frame size (#<sz_t>).", data_length, o.channels * sizeof( s1_t ) );
    }

    sz_t frames = data_length / ( o.channels * sizeof( s1_t ) );
    sz_t frames_per_buffer = 32768;

    while( frames > 0 )
    {
        sz_t buffered_frames = frames < frames_per_buffer ? frames : frames_per_buffer;
        m :buffer_s* buffer = o.push_buffer().set_size( buffered_frames * o.channels );
        if( source.get_data( buffer.data, buffer.size * sizeof( s1_t ) ) != buffer.size * sizeof( s1_t ) )
        {
            return bcore_error_push_fa( TYPEOF_general_error, "wav_from_source: Unexpected end of stream." );
        }
        frames -= buffered_frames;
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:sequence_s) wav_from_file =
{
    m x_source* source = x_source_create_from_file( path )^;
    if( !source ) return bcore_error_push_fa( TYPEOF_general_error, "Could not open file path '#<sc_t>'.", path );
    return o.wav_from_source( source );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

