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

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

type snd_pcm_t;
type snd_pcm_hwparams_t;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    /// =========== parameters ===========

    /// Set these parameters before calling 'setup'
    //st_s device_name = "hw:0,0";
    st_s device_name = "default";

    /** Number of channels (normally 2 (stereo))
     */
    $ s2_t channels = 2;

    /** Requested values can be overridden by the hardware.
     *  A negative value indicates that no value is requested.
     *  After setup the corresponding actual values (see 'status')
     *  reflect the setting used by the alsa interface.
     */

    /// Requested rate in hertz
    $ s2_t requested_rate = 44100;

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

    func er_t setup( m@* o );
    func er_t shut_down( m@* o );

    /** Plays a sequence in one sitting.
     *  A single frame contains <o.channels> samples.
     *  Not intended for seamless concatenation of multiple sequences.
     *  See also stream-functions below.
     */
    func er_t play( m@* o, s1_t* interleaved_samples, u2_t frames );
    func er_t play_buffer( m@* o, bcodec_audio_buffer_s* buf ) { = o.play( buf.data, buf.size / o.channels ); }

    /// Resets player to sequence channels and rate if needed
    func er_t play_sequence( m@* o, bcodec_audio_sequence_s* sequence );

    /// Starts a continuous stream (no effect if already started)
    func er_t stream_start( m@* o );

    /// Stops an ongoing stream (if any). Starts a continuous stream.
    func er_t stream_restart( m@* o );

    /** Plays interleaved samples as part of a continuous stream.
     *  A single frame contains <o.channels> samples.
     *  This function seamlessly concatenates subsequent samples to a continuous stream.
     *  To avoid underruns, a subsequent call must be initiated before the last finishes playing.
     */
    func er_t stream_play       ( m@* o, s1_t* interleaved_samples, u2_t frames );
    func er_t stream_play_buffer( m@* o, bcodec_audio_buffer_s* buf ) { = o.stream_play( buf.data, buf.size / o.channels ); }
    func er_t stream_play_zero  ( m@* o, u2_t frames ) { = ( frames > 0 ) ? o.stream_play_buffer( bcodec_audio_buffer_s!^.set_size( frames * o.channels, o.channels ) ) : 0; }
    func er_t stream_play_sequence( m@* o, bcodec_audio_sequence_s* sequence ); // error if sequence rate and channels settings mismatch audio settings

    /// Gentle stop of a continuous stream (by draining) (no effect if not streaming)
    func er_t stream_stop( m@* o );

    /// Forced stop of a continuous stream (by dropping frames) (no effect if not streaming)
    func er_t stream_cut( m@* o );

    /// Plays a test sound
    func er_t sound_check( m@* o );

    /// ==================================

    /// other internal status data
    private snd_pcm_t* handle_;
    private bl_t is_open_ = false;
    private bl_t is_setup_ = false;
    private bl_t is_streaming_ = false;
}

//----------------------------------------------------------------------------------------------------------------------

/** Dedicated Audio Player using a background thread.
 *  Simplifies setup and control for one-time or continuous playing.
 *  Usage:
 *      m$* player = bmedia_audio_out_player_s!^
 *      player.setup( audio );
 *      call player.play( ... ) anytime
 *      call player.wait_unltil_empty() to suspend calling thread until play buffer is empty (everything was played)
 *      when finished, simply destroy player.
 *
 *  - Takes ownership of audio object
 *  - Unlimited buffer space.
 *
 */
stamp :player_s
{
    /// =========== parameters ===========

    /** Maximum buffer size of a single sequence-element.
     *  When shutting down, the maximum latency is given by this value.
     */
    sz_t buf_frames = 16384;

    /** If this value is > 0 the player keeps sending zeros (buffers) while idle (empty) to audio until real data arrive.
     *  This is useful to keep the audio interface in stream-playing mode in case playable data arrive at larger time gaps.
     */
    sz_t idle_zero_frames = 0;

    /** Changes nice level for the audio thread. Range: [-20, 19]
     *  Note: Instead of lowering this value here, consider calling function x_threads_set_nice_level
     *  by the caller thread or even at program start. Spawned threads will inherit that new nice level.
     *  Rationale: Unless the player's buffers pipeline remains large enough, the feeding thread
     *  might need a higher priority, too.
     *
     *  A negative level is useful for seamless streaming.
     *  An ineffective nice level will produce a warning message during player setup.
     *  Note that a regular user must have the permission for negative nice levels.
     *  Permissions are specified in: /etc/security/limits.conf
     */
    s2_t nice_level = 0;

    /// ==================================

    :s -> audio;

    hidden bcodec_audio_sequence_s sequence;

    hidden x_thread_s    thread;
    hidden x_mutex_s     mutex;
    hidden x_condition_s condition_play;
    hidden x_condition_s condition_below_min_size;

    hidden sz_t min_size = 0;
    hidden bl_t thread_exit_;
    hidden bl_t is_setup_;
    hidden er_t thread_error_;

    func x_thread.m_thread_func;

    func er_t setup( m@* o, d :s* audio );

    /// sets up audio parameters according to sequence
    func er_t setup_from_sequence( m@* o, bcodec_audio_sequence_s* sequence );

    func er_t shut_down( m@* o );

    /// retrieves sample rate
    func sz_t rate( c@* o ) = o.audio ? o.audio.actual_rate : 0;

    /// retrieves channels
    func sz_t channels( c@* o ) = o.audio ? o.audio.channels : 0;

    /** Appends data to the play-buffer.
     *  If not already playing, playing is triggered.
     */
    func er_t play( m@* o, s1_t* interleaved_samples, u2_t frames );
    func er_t play_buffer( m@* o, bcodec_audio_buffer_s* buf ) { ASSERT( o.audio && o.audio.channels == buf.channels ); = o.play( buf.data, buf.size / o.audio.channels ); }
    func er_t play_zero( m@* o, u2_t frames ) { ASSERT( o.audio ); = ( frames > 0 ) ? o.play_buffer( bcodec_audio_buffer_s!^.set_size( frames * o.audio.channels, o.audio.channels ) ) : 0; }

    /** Plays entire sequence.
     *  This function resets player to sequence channels and rate if needed.
     */
    func er_t play_sequence( m@* o, bcodec_audio_sequence_s* sequence );

    /// Returns current (unplayed) buffers
    func sz_t buffers( m@* o ) = x_lock_s!^( o.mutex ).attn( o ).sequence.size();

    /// Returns current (unplayed) buffer frames
    func sz_t buffer_frames( m@* o ) = x_lock_s!^( o.mutex ).attn( o ).sequence.sum_buffer_size() / o.audio.channels;

    /// Checks if play-buffer is empty
    func bl_t is_empty( m@* o ) = x_lock_s!^( o.mutex ).attn( o ).sequence.size() == 0;

    /// Suspends calling thread until play-buffer drops below size
    func er_t wait_until_below( m@* o, sz_t size );

   /// Suspends calling thread until play-buffer is empty
    func er_t wait_until_empty( m@* o ) = o.wait_until_below( 1 );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bmedia_audio_out.emb.x";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
