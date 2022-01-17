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

#ifndef BMEDIA_AUDIO_H
#define BMEDIA_AUDIO_H

#include <alsa/asoundlib.h> // link with -lasound
#include "bcore_std.h"

//----------------------------------------------------------------------------------------------------------------------

XOILA_DEFINE_GROUP( bmedia_audio, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// General buffer for audio data
stamp :buffer_s = x_array
{
    s1_t [];
};

//----------------------------------------------------------------------------------------------------------------------

/// General buffer for audio data
stamp :buffer_adl_s = x_array
{
    :buffer_s => [];
};

//----------------------------------------------------------------------------------------------------------------------

/** Self contained audio sequence.
 *  Can be used as simple container or FIFO queue.
 *  Buffers are organized in a deque.
 *  deque elements are managed by a ring buffer.
 */
stamp :sequence_s =
{
    /// Number of channels (2 == stereo); -1: not specified
    s2_t channels = -1;

    /// Sample rate in hertz; -1: not specified
    s2_t rate = -1;

    /** Preferred Frames per Buffer
     *  This number is used where a buffer size needs to be specified
     *  but was not already provided externally.
     *  Note that the buffer adl can hold buffers of arbitrary size
     *  regardless of this number.
     */
    sz_t preferred_frames_per_buffer = 32768;

    :buffer_adl_s adl;

    sz_t first; // index to first element
    sz_t size;  // number of used elements

    func (void clear( m@* o )) =
    {
        o.adl.clear();
        o.first = o.size = 0;
    };

    /// Access function: Returns NULL if out of range
    func (m :buffer_s* buffer_m( m@* o, sz_t index )) = { return ( index >= 0 && index < o.size ) ? o.adl.[ ( o.first + index ) % o.adl.size ] : NULL; };
    func (c :buffer_s* buffer_c( c@* o, sz_t index )) = { return o.cast( m@* ).buffer_m( index ); };
    func (m :buffer_s* last_m( m@* o )) = { return o.buffer_m( o.size - 1 ); };
    func (c :buffer_s* last_c( m@* o )) = { return o.cast( m@* ).last_m(); };

    /// Appends new element to sequence
    func (m :buffer_s* push_empty_buffer( m@* o ));
    func (m :buffer_s* push_copy_buffer( m@* o, :buffer_s* buffer )) = { m :buffer_s* b = o.push_empty_buffer(); b.copy( buffer ); return b; };

    /// Takes first element from sequence
    func (d :buffer_s* pop_first_buffer( m@* o ));

    /// Sum of all buffer sizes
    func (sz_t sum_buffer_size( @* o ));
    func (sz_t sum_buffer_frames( @* o )) = { return o.sum_buffer_size() / o.channels; };

    /// Sends sequence in RIFF-WAVE format to sink
    func (er_t wav_to_sink( @* o, m x_sink* sink ));
    func (er_t wav_to_file( @* o, sc_t path ));

    /// Receives sequence in RIFF-WAVE format from source
    func (er_t wav_from_source( m@* o, m x_source* source ));
    func (er_t wav_from_file(   m@* o, sc_t path ));

    /// returns an iterator for sequence
    func (d :sequence_iterator_s* create_iterator( @* o )) =
    {
        return :sequence_iterator_s!.setup( o );
    };
};

//----------------------------------------------------------------------------------------------------------------------

/** Iterator used to seamlessly extract frames from a sequence.
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

    /// frames past current index
    s3_t past_frames;

    /// end of sequence was reached
    bl_t eos = true;

    /// setup; sets index to zero
    func (o setup( m@* o, :sequence_s* sequence ));

    /// sets index to zero
    func (o reset( m@* o ));

    /// goes to the next frame after past_frames
    func (o go_to( m@* o, sz_t past_frames ));

    /// obtains given number of frames; excess frames are padded with zeros
    func (o get_frames( m@* o, m s1_t* data, sz_t frames ));

    /// obtains given number of frames; excess frames are padded with zeros; allocates buffer
    func (o get_buffer( m@* o, m :buffer_s* buffer, sz_t frames ));

    /// obtains given number of frames; excess frames are padded with zeros; returns buffer of copied data
    func (d :buffer_s* create_buffer( m@* o, sz_t frames ));
};

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bmedia_audio.x";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif // BMEDIA_AUDIO_H