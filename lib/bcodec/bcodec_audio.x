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

/// Audio data buffering and sequencing.

include "bcore_std.h";
include "bmath_std.h";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// General buffer for audio data
stamp :buffer_s x_array
{
    s1_t channels;
    s1_t [];
    func o set_size( m@* o, sz_t size, sz_t channels ) { o.cast( m x_array* ).set_size( size ); o.channels = channels; = o; };
    func o set_frames( m@* o, sz_t frames, sz_t channels ) { = o.set_size( frames * channels, channels ); };
    func o set_zero( m@* o ) { foreach( m$* e in o ) e.0 = 0; = o; };

    func sz_t frames( @* o ) = o.channels > 0 ? o.size / o.channels : 0;

    /// converts buffer channel into vector with value range [ -1.0 , +1.0 [
    func vec get_vf2( @* o, m bmath_vf2_s* vec, sz_t channel );

    /// vector into buffer value for given channel (overflow-check by truncation)
    func o set_from_vf2( @* o, c bmath_vf2_s* vec, sz_t channel );

    func f3_t energy( @* o )
    {
        f3_t sum = 0;
        foreach( $e in o ) sum += f3_sqr( e );
        = sum;
    }

    func f3_t sum( @* o )
    {
        f3_t sum = 0;
        foreach( $e in o ) sum += e;
        = sum;
    }

    func f3_t max_abs( @* o )
    {
        f3_t max_abs = 0;
        foreach( $e in o ) max_abs = f3_max( max_abs, f3_abs( e ) );
        = max_abs;
    }

    func o scale( m@* o, f3_t factor )
    {
        foreach( m$*e in o ) e.0 = f3_min( 32767, f3_max( -32768, e.0 * factor ) );
    }

    // scales frames linearly across buffer from start_factor to end_factor
    func o scale_lin( m@* o, f3_t start_factor, f3_t end_factor );

    // scales frames exponentially across buffer from start_factor to end_factor
    func o scale_exp( m@* o, f3_t start_factor, f3_t end_factor );

    // copy_spread_channels with scaling
    func o copy_spread_channels( m@* o, f3_t scale_factor, sz_t dst_channels, @* src );

    /** Copies from src buffer while resampling with interpolation.
     *  frame_step (>0):
     *     Stepping across frames in src.
     *     >1 leads to higher pitch
     *     <1 leads to lower pitch
     *  frame_index (>=0) is the starting index in src
     *     If src is the first buffer in a sequence frame_index is normally '0'.
     *     For subsequent buffers: frame_index --> frame_index + frame_step * o.frames() - src.frames().
     *  'next' can be NULL, otherwise it represents the next buffer in a sequence.
     */
    func o copy_resample( m@* o, f3_t frame_index, f3_t frame_step, f3_t scale_factor, @* src, @* next );
}

//----------------------------------------------------------------------------------------------------------------------

/** Self contained audio sequence.
 *  Can be used as simple container or FIFO queue.
 *  Buffers are organized in a deque.
 */
stamp :sequence_s x_deque trans(TE :buffer_s)
{
    x_deque_inst_s deque;

    /// Number of channels (2 == stereo); -1: not specified
    s2_t channels = -1;

    /// Sample rate in hertz; -1: not specified
    s2_t rate = -1;

    /** Preferred Frames per Buffer
     *  This number is used where a buffer size needs to be specified
     *  but was not already provided externally.
     *  Note that the sequence may hold buffers of arbitrary size
     *  regardless of this number.
     */
    sz_t preferred_frames_per_buffer = 32768;

    func x_deque.size = o.deque.size;
    func x_deque.m_get = o.deque.m_get( index ).cast( m :buffer_s* );
    func x_deque.c_get = o.deque.c_get( index ).cast( c :buffer_s* );

    func o set_zero( m@* o ) { for( sz_t i = 0; i < o.size(); i++ ) o.m_get( i ).set_zero(); }

    /// sets up empty sequence
    func o setup( m@* o, s2_t channels, s2_t rate ) { o.clear(); o.channels = channels; o.rate = rate; = o; }

    /// allocates frames and channels and sets all zero
    func o setup_frames( m@* o, s2_t channels, s2_t rate, s3_t frames );

    /// creates difference (a - b) of sequences a and b
    func o setup_diff( m@* o, @* a, @* b );

    /// setup from src by forking all buffers in src
    func o setup_fork_buffers( m@* o, m @* src );

    /// Access function: Returns NULL if out of range
    func m :buffer_s* m_buffer( m@* o, sz_t index ) = ( index >= 0 && index < o.deque.size ) ? o.m_get( index ) : NULL;
    func c :buffer_s* c_buffer( c@* o, sz_t index ) = o.cast( m@* ).m_buffer( index );
    func m :buffer_s* m_first ( m@* o ) = o.m_buffer( 0 );
    func c :buffer_s* c_first ( c@* o ) = o.cast( m@* ).m_first();
    func m :buffer_s* m_last  ( m@* o ) = o.m_buffer( o.size() - 1 );
    func c :buffer_s* c_last  ( c@* o ) = o.cast( m@* ).m_last();

    /// Appends new element to sequence
    func m :buffer_s* push_buffer_d( m@* o, d :buffer_s* buffer );
    func m :buffer_s* push_buffer_c( m@* o,   :buffer_s* buffer ) = o.push_buffer_d( buffer.clone() );
    func m :buffer_s* push_empty_buffer( m@* o ) = o.push_buffer_d( :buffer_s! );

    /// Takes first element from sequence
    func d :buffer_s* pop_first_buffer( m@* o ) = o.d_pop_first();

    /// Sum of all buffer sizes
    func sz_t sum_buffer_size( @* o );
    func sz_t sum_buffer_frames( @* o ) = o.sum_buffer_size() / o.channels;

    /// Sends sequence in RIFF-WAVE format to sink
    func er_t wav_to_sink( @* o, m x_sink* sink );
    func er_t wav_to_file( @* o, sc_t path );

    /// Receives sequence in RIFF-WAVE format from source
    func er_t wav_from_source( m@* o, m x_source* source );
    func er_t wav_from_file(   m@* o, sc_t path );

    func f3_t energy( @* o )
    {
        f3_t sum = 0;
        for( sz_t i = 0; i < o.size(); i++ ) sum += o.c_buffer( i ).energy();
        = sum;
    }

    func f3_t sum( @* o )
    {
        f3_t sum = 0;
        for( sz_t i = 0; i < o.size(); i++ ) sum += o.c_buffer( i ).sum();
        = sum;
    }

    func f3_t max_abs( @* o )
    {
        f3_t max_abs = 0;
        for( sz_t i = 0; i < o.size(); i++ ) max_abs = f3_max( max_abs, o.c_buffer( i ).max_abs() );
        = max_abs;
    }

    func o scale( m@* o, f3_t factor )
    {
        for( sz_t i = 0; i < o.size(); i++ ) o.m_buffer( i ).scale( factor );
    }

    func o copy_scale( m@* o, @* src, f3_t factor )
    {
        o.setup( src.channels, src.rate );
        for( sz_t i = 0; i < src.size(); i++ ) o.push_buffer_d( src.c_buffer( i ).clone().scale( factor ) );
    }

    /** Copies from src sequence while resampling with interpolation.
     *  frame_step (>0):
     *     Stepping across frames in src.
     *     >1 leads to higher pitch
     *     <1 leads to lower pitch
     *  frame_index (>=0) is the starting index in src
     *     If src is the first buffer in a sequence frame_index is normally '0'.
     *     For subsequent buffers: frame_index --> frame_index + frame_step * o.frames() - src.frames().
     *  'next' can be NULL, otherwise it represents the next buffer in a sequence.
     */
    func o copy_resample( m@* o, f3_t frame_index, f3_t frame_step, f3_t scale_factor, @* src, @* next )
    {
        o.setup( src.channels, src.rate );
        for( sz_t i = 0; i < src.size(); i++ )
        {
            bcodec_audio_buffer_s* src_buf = src.c_buffer( i );
            bcodec_audio_buffer_s* next_buf = ( i + 1 < src.size() ) ? src.c_buffer( i + 1 ) : ( next && next.size() > 0 ) ? next.c_buffer( 0 ) : NULL;
            m bcodec_audio_buffer_s* buf = o.push_empty_buffer();
            buf.copy_resample( frame_index, frame_step, scale_factor, src_buf, next_buf );
            frame_index += frame_step * buf.frames() - src_buf.frames();
        }
    }

    /// returns an indexer for sequence
    func d :sequence_indexer_s* create_indexer( @* o )
    {
        = :sequence_indexer_s!.setup( o.cast( m$* ) );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** Indexer providing fast linear access to sequence.
 *  The indexer uses forked references to audio buffers to the original sequence passed in 'setup'.
 *  While in use, the original sequence may be discarded or may add or remove buffers.
 *  Only shared buffers may not be modified in size.
 */
stamp :sequence_indexer_s =
{
    :sequence_s => sequence;

    s3_t size; // global size
    s2_t channels;
    hidden bcore_indexer_s indexer;

    /// setup; sets index to zero; forks buffers
    func o setup( m@* o, m :sequence_s* sequence );

    /// reads 'frames' from sequence at global index position. Out-of-range positions are filled with zeros.
    func o get_data( @* o, m s1_t* data, s3_t index, sz_t frames );

    /// sets 'frames' in sequence at global index position. Out-of-range positions are ignored.
    func o set_data( m @* o, s1_t* data, s3_t index, sz_t frames );

    /// obtains given number of frames; off-range frames are padded with zeros; allocates buffer
    func buffer get_buffer( @* o, m :buffer_s* buffer, s3_t index, sz_t frames );

    /// obtains given number of frames; off-range frames are padded with zeros; returns buffer of copied data
    func d :buffer_s* create_buffer( @* o, s3_t index, sz_t frames );

    /// obtains given number of frames; off-range frames are padded with zeros; allocates buffer
    func o set_from_buffer( m @* o, c :buffer_s* buffer, s3_t index );
}

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bcodec_audio.emb.x";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
