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

/** Audio Capture Interface (based on the ALSA API)
 *
 *  Usage for continuous recording:
 *  - In 'my_capture_stamp':
 *    - Implement feature bmedia_audio_in.capture_feed  (mandatory, should be thread-safe)
 *  - In 'my_exit_stamp':
 *    - Implement feature bmedia_audio_in.capture_exit  (optional, should be thread-safe)
 *
 *  Setup and start capturing:
 *  - Create instance: bmedia_audio_in_s^ my_audio;
 *  - Change parameters in my_audio. (if needed)
 *  - Run: my_audio.setup()
 *  - Check/Process status values in my_audio. (if desired)
 *  - Call my_audio.stream_start()  (Turns streaming on)
 *  - Call my_audio.capture_loop( my_capture_stamp, my_exit_stamp (or NULL) )  (This function is typically called from a dedicated thread)
 *
 *  Stop capturing (optional):
 *  - Have bmedia_audio_in.capture_exit return 'true'
 *    . This causes my_audio.capture_loop to return.
 *    . Streaming stays on (!)
 *    . Call my_audio.capture_loop to restart capturing.
 *
 *  Clean shut down:
 *  - Destroy my_audio or call my_audio.shut_down()
 *    . Stops capturing.
 *    . Turns streaming off.
 *
 *  ---------------------------------
 *
 *  Usage via bmedia_audio_in_capture_thread_s:
 *  - m$* thread = bmedia_audio_in_capture_thread_s!.setup( audio, capture_feed );
 *  - When finished capturing simply destroy thread.
 *
 *  ---------------------------------
 *
 *  Usage for one-time recording:
 *  - Create instance: bmedia_audio_in_s^ my_audio;
 *  - Change parameters in my_audio. (if needed)
 *  - Run: my_audio.setup()
 *  - Check/Process status values in my_audio. (if desired)
 *  - Call my_audio.stream_record( ... ) to get the desired number of frames.
 *
 *  ---------------------------------
 *
 *  References (used for development):
 *    - ALSA Project: https://www.alsa-project.org/alsa-doc/alsa-lib/index.html
 */

include <alsa/asoundlib.h>; // link with -lasound
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

    // in case channels mismatch, trailing channels are truncated or duplicated into o
    func o copy_spread_channels( m@* o, sz_t dst_channels, @* src );
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

    /// returns an iterator for sequence
    func d :sequence_iterator_s* create_iterator( @* o )
    {
        = :sequence_iterator_s!.setup( o );
    }

    /// returns an indexer for sequence
    func d :sequence_indexer_s* create_indexer( @* o )
    {
        = :sequence_indexer_s!.setup( o.cast( m$* ) );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** Deprecated: Use indexer below.
 *  Iterator used to seamlessly extract frames from a sequence.
 *  While the iterator is in use, the sequence should not be modified.
 */
stamp :sequence_iterator_s =
{
    /// pointer to sequence
    hidden :sequence_s* sequence;

    /// copied form sequence.channels
    s2_t channels;

    /// index to current buffer in sequence
    sz_t buffer_index;

    /// index to current frame in current buffer
    sz_t frame_index;

    /// total number of frames in sequence
    s3_t total_frames;

    /// global frame index
    s3_t global_frame_index;

    /// end of sequence was reached
    bl_t eos = true;

    /// setup; sets index to zero
    func o setup( m@* o, :sequence_s* sequence );

    /// sets index to zero
    func o reset( m@* o );

    /** Move to new position.
     *  diff(erence) can be positive or negative.
     *  Moving to or past end of sequenced sets eos flag.
     *  End position is truncated in case destination is out of range.
     */
    func o move( m@* o, s3_t diff );

    /// goes to the global index (via move)
    func o go_to( m@* o, s3_t global_frame_index );

    /// obtains given number of frames; excess frames are padded with zeros
    func o get_frames( m@* o, m s1_t* data, sz_t frames );

    /// obtains given number of frames; excess frames are padded with zeros; allocates buffer
    func o get_buffer( m@* o, m :buffer_s* buffer, sz_t frames );

    /// obtains given number of frames; excess frames are padded with zeros; returns buffer of copied data
    func d :buffer_s* create_buffer( m@* o, sz_t frames );
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

embed "bmedia_audio.emb.x";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

