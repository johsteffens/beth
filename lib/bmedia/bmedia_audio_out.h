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

/** Audio Play Interface (based on the ALSA API)
 *
 *  Usage for continuous recording:
 *
 *  Setup and start capturing:
 *  - Create instance: bmedia_audio_out_s^ my_audio;
 *  - Change parameters in my_audio. (if needed)
 *  - Run: my_audio.setup()
 *  - Check/Process status values in my_audio. (if desired)
 *  - Call my_audio.stream_start()  (Turns streaming on)
 *  - Call repeatedly
 *        my_audio.stream_play( ... )
 *
 *  Stop capturing (optional):
 *  - Call my_audio.stream_stop() or my_audio.stream_cut()
 *
 *  Clean shut down:
 *  - Destroy my_audio or call my_audio.shut_down()
 *
 *  ---------------------------------
 *
 *  Usage for one-time playing:
 *  - Create instance: bmedia_audio_in_s^ my_audio;
 *  - Change parameters in my_audio. (if needed)
 *  - Run: my_audio.setup()
 *  - Check/Process status values in my_audio. (if desired)
 *  - Call my_audio.stream_play( ... ).
 *
 *  ---------------------------------
 *
 *  Usage via bmedia_audio_out_player_s:
 *  - m$* player = bmedia_audio_in_player_s!.setup( audio );
 *  - Call player.play any time to feed more data
 *  - When finished capturing simply destroy player.
 *
 *  ---------------------------------
 *
 *  References (used for development):
 *    - ALSA Project: https://www.alsa-project.org/alsa-doc/alsa-lib/index.html
 */

#ifndef BMEDIA_AUDIO_OUT_H
#define BMEDIA_AUDIO_OUT_H

#include <alsa/asoundlib.h> // link with -lasound
#include "bcore_std.h"
#include "bmedia_audio.h"

//----------------------------------------------------------------------------------------------------------------------

XOILA_DEFINE_GROUP( bmedia_audio_out, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

type snd_pcm_t;
type snd_pcm_hwparams_t;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s =
{
    /// =========== parameters ===========

    /// Set these parameters before calling 'setup'
    //st_s device_name = "hw:0,0";
    st_s device_name = "default";

    /** Number of channels (normally 2 (stereo))
     */
    s2_t channels = 2;

    /** Requested values can be overridden by the hardware.
     *  A negative value indicates that no value is requested.
     *  After setup the corresponding actual values (see 'status')
     *  reflect the setting used by the alsa interface.
     */

    /// Requested rate in hertz
    s2_t requested_rate = 44100;

    /** Requested Number of periods in the data (ring-)buffer.
     *  A period is a data-fragment that is bulk-transferred between ordinary memory and hardware.
     *  See also: frames_per_period
     */
    s2_t requested_periods = 2;

    /** Requested period-size given by number of frames per period.
     *  Increasing this size increases latency but reduces synchronization-frequency and transfer-overhead.
     *  A frame is the data set for all channels (e.g. 4 bytes for stereo and SND_PCM_FORMAT_S16)
     */
    s2_t requested_frames_per_period = 2048;

    /// ==================================

    /// =========== status ===============
    /// Check these values after calling 'setup'

    /// Sampling rate in hertz
    private s2_t actual_rate;

    /// Number of periods in the hw-ring-buffer.
    private s2_t actual_periods;

    /// Period-size given by number of frames per period.
    private s2_t actual_frames_per_period;

    /// ==================================

    /// =========== functions ============

    func (er_t setup( m@* o ));
    func (er_t shut_down( m@* o ));

    /** Plays a sequence in one sitting.
     *  A single frame contains <o.channels> samples.
     *  Not intended for seamless concatenation of multiple sequences.
     *  See also stream-functions below.
     */
    func (er_t play( m@* o, s1_t* interleaved_samples, u2_t frames ));
    func (er_t play_buffer( m@* o, bmedia_audio_buffer_s* buf )) = { return o.play( buf.data, buf.size / o.channels ); };

    /// Resets player to sequence channels and rate if needed
    func (er_t play_sequence( m@* o, bmedia_audio_sequence_s* sequence ));

    /// Starts a continuous stream (no effect if already started)
    func (er_t stream_start( m@* o ));

    /// Stops an ongoing stream (if any). Starts a continuous stream.
    func (er_t stream_restart( m@* o ));

    /** Plays interleaved samples as part of a continuous stream.
     *  A single frame contains <o.channels> samples.
     *  This function seamlessly concatenates subsequent samples to a continuous stream.
     *  To avoid underruns, a subsequent call must be initiated before the last finishes playing.
     */
    func (er_t stream_play       ( m@* o, s1_t* interleaved_samples, u2_t frames ));
    func (er_t stream_play_buffer( m@* o, bmedia_audio_buffer_s* buf )) = { return o.stream_play( buf.data, buf.size / o.channels ); };
    func (er_t stream_play_zero  ( m@* o, u2_t frames )) = { return ( frames > 0 ) ? o.stream_play_buffer( bmedia_audio_buffer_s!^.set_size( frames * o.channels ) ) : 0; };
    func (er_t stream_play_sequence( m@* o, bmedia_audio_sequence_s* sequence )); // error if sequence rate and channels settings mismatch audio settings

    /// Gentle stop of a continuous stream (by draining) (no effect if not streaming)
    func (er_t stream_stop( m@* o ));

    /// Forced stop of a continuous stream (by dropping frames) (no effect if not streaming)
    func (er_t stream_cut( m@* o ));

    /// Plays a test sound
    func (er_t sound_check( m@* o ));

    /// ==================================

    /// other internal status data
    private snd_pcm_t* handle_;
    private bl_t is_open_ = false;
    private bl_t is_setup_ = false;
    private bl_t is_streaming_ = false;
};

//----------------------------------------------------------------------------------------------------------------------

/** Dedicated Audio Player using a background thread.
 *  Simplifies setup and control for one-time or continuous playing.
 *  Usage:
 *      m$* player = bmedia_audio_out_player_s!;
 *      my_thread.setup( audio );
 *      call player.play( ... ) anytime
 *      when finished, simply destroy player.
 *
 *  - Takes ownership of audio object
 *  - Unlimited buffer space.
 *
 */
stamp :player_s =
{
    /// =========== parameters ===========

    /** Maximum buffer size of a single sequence-element.
     *  When shutting down, the maximum latency is given by this value.
     */
    sz_t buf_frames = 16384;

    /// ==================================

    :s -> audio;

    hidden bmedia_audio_sequence_s sequence;

    hidden x_thread_s    thread;
    hidden x_mutex_s     mutex;
    hidden x_condition_s condition_play;
    hidden x_condition_s condition_buffer_empty;

    hidden bl_t thread_exit_;
    hidden bl_t is_setup_;
    hidden er_t thread_error_;

    func x_thread.m_thread_func;

    func (er_t setup( m@* o, d :s* audio ));
    func (er_t shut_down( m@* o ));

    /** Appends data to the play-buffer.
     *  If not already playing, playing is triggered.
     */
    func (er_t play( m@* o, s1_t* interleaved_samples, u2_t frames ));
    func (er_t play_buffer( m@* o, bmedia_audio_buffer_s* buf )) = { ASSERT( o.audio ); return o.play( buf.data, buf.size / o.audio.channels ); };
    func (er_t play_zero( m@* o, u2_t frames )) = { ASSERT( o.audio ); return ( frames > 0 ) ? o.play_buffer( bmedia_audio_buffer_s!^.set_size( frames * o.audio.channels ) ) : 0; };

    /** Plays entire sequence.
     *  This function resets player to sequence channels and rate if needed.
     */
    func (er_t play_sequence( m@* o, bmedia_audio_sequence_s* sequence ));

    /** Plays sequence from iterator position for number of frames or until end (whichever occurs first).
     *  frames == -1: Plays until end.
     *  This function resets player to sequence channels and rate if needed.
     */
    func (er_t play_iterator( m@* o, m bmedia_audio_sequence_iterator_s* iterator, sz_t frames ));

    /// Returns current (unplayed) buffer frames
    func (bl_t buffer_frames( m@* o ));

    /// Checks if play-buffer is empty
    func (bl_t is_empty( m@* o ));

    /// Suspends calling thread until play-buffer is empty
    func (er_t wait_until_empty( m@* o ));
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bmedia_audio_out.x";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif // BMEDIA_AUDIO_OUT_H
