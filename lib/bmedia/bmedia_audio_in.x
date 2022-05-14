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

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

type snd_pcm_t;
type snd_pcm_hwparams_t;

//----------------------------------------------------------------------------------------------------------------------

/** Callback feature. Called when a new fragment is available.
 *  Image data is laid out in YUYV format.
 */
feature void capture_feed( m@* o, bmedia_audio_buffer_s* buf );

/// Optional callback for each loop cycle; return true to exit loop
feature bl_t capture_exit( m@* o ) { = false; };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    /// =========== parameters ===========

    /// Set these parameters before calling 'setup'
    //st_s device_name = "hw:0,0";
    st_s device_name = "default";

    /** Number of channels.
     *  Sound hardware might expect value '2' (== stereo) here
     *  even though the input is connected to only one microphone.
     */
    s2_t channels = 2;

    /** Requested values can be overridden by the hardware.
     *  A negative value indicates that no value is requested.
     *  After setup the corresponding actual values (see 'status')
     *  reflect the setting used by the alsa interface.
     */

    /// Requested rate in hertz
    s2_t requested_rate = 44100;

    /** Number of periods in the data (ring-)buffer.
     *  A period is a data-fragment that is bulk-transferred between ordinary memory and hardware.
     *  See also: frames_per_period
     */
    s2_t requested_periods = 2; // number of periods (fragments)

    /** Period-size given by number of frames per period.
     *  Increasing this size increases latency but reduces synchronization-frequency and transfer-overhead.
     *  A frame is the data set for all channels (e.g. 4 bytes for stereo and SND_PCM_FORMAT_S16)
     *  The permissible value range is hardware dependent.
     */
    s2_t requested_frames_per_period = 2048;

    /// ==================================

    /// =========== status ===============
    /// Check these values after calling 'setup'

    /// Sampling rate in hertz
    private s2_t actual_rate;

    /// Number of periods in the data (ring-)buffer.
    private s2_t actual_periods;

    /// Period-size given by number of frames per period.
    private s2_t actual_frames_per_period;

    /// ==================================

    /// =========== functions ============

    func er_t setup( m@* o );
    func er_t shut_down( m@* o );

    /// Records interleaved samples in a single session.
    func er_t record( m@* o, m s1_t* interleaved_samples, sz_t frames );

    /// Records (appends) to sequence. If sequence is empty, 'rate' and 'channels' are set appropriately.
    func er_t record_to_sequence( m@* o, m bmedia_audio_sequence_s* sequence, sz_t frames );

    /// Starts a continuous stream (no effect if already started)
    func er_t stream_start( m@* o );

    /// Stops an ongoing stream (if any). Starts a continuous stream.
    func er_t stream_restart( m@* o );

    /** Records interleaved samples as part of a continuous stream.
     *  This function seamlessly concatenates subsequent recordings to a continuous stream.
     *  To avoid underruns, a subsequent recording must be initiated before the buffer fills up.
     */
    func er_t stream_record( m@* o, m s1_t* interleaved_samples, sz_t frames );

    /// Records (appends) to sequence. If sequence is empty, 'rate' and 'channels' are set appropriately.
    func er_t stream_record_to_sequence( m@* o, m bmedia_audio_sequence_s* sequence, sz_t frames );

    /// Gentle stop of a continuous stream (by draining)
    func er_t stream_stop( m@* o );

    /// Forced stop of a continuous stream (by dropping frames)
    func er_t stream_cut( m@* o );

    /** 'capture_loop' calls 'feed.capture_feed' for each new fragment.
     *  The loop terminates when 'shut_down' is called or when 'feed.capture_exit()' returns 'false'.
     *
     *  Other actions:
     *  Sets up stamp in case it was not yet set up.
     *  Activates audio streaming if not already streaming.
     *
     *  Note:
     *  This function may run in a dedicated thread.
     *  Features 'capture_exit' and 'capture_feed' are called directly from this function.
     *  If a condition terminates the loop, it can be restarted.
     */
    func er_t capture_loop( m@* o, m:* capture_feed, m:* capture_exit /* can be NULL */ );

    /// ==================================

    /// other internal status data
    private snd_pcm_t* handle_;
    private bl_t is_open_ = false;
    private bl_t is_setup_ = false;
    private bl_t is_streaming_ = false;

    hidden x_mutex_s mutex_;
    hidden x_mutex_s mutex_exit_capture_loop_;
    hidden bl_t            exit_capture_loop_ = false;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Dedicated thread for the capture loop.
 *  Simplifies setup and control for continuous audio recording.
 *  Usage:
 *      m$* my_thread = bmedia_audio_in_capture_thread_s!;
 *      my_thread.setup( audio, capture_feed );
 *
 *  When done, simply destroy my_thread.
 *
 *  - Takes ownership of audio object
 *  - No ownership of capture_feed object
 *  - Audio stream is started during setup if not started already.
 *  - capture_loop_error_ holds an error occurring in the thread.
 */
stamp :capture_thread_s =
{
    hidden :s -> audio;
    private aware :* capture_feed;

    x_thread_s thread;
    x_mutex_s  mutex;
    bl_t exit_loop_;
    er_t capture_loop_error_ = 0;

    func er_t setup( m@* o, d :s* audio, m aware :* capture_feed )
    {
        o.shut_down();
        o.audio =< audio;
        o.audio.stream_start();
        o.capture_feed = capture_feed;
        o.exit_loop_ = false;
        o.thread.call_m_thread_func( o );
        = 0;
    }

    func x_thread.m_thread_func
    {
        er_t error = o.audio.capture_loop( o.capture_feed, o );
        if( error )
        {
            o.mutex.lock();
            o.capture_loop_error_ = error;
            o.mutex.unlock();
        }
        = NULL;
    }

    func bmedia_audio_in.capture_exit
    {
        o.mutex.lock();
        bl_t exit_loop_ = o.exit_loop_;
        o.mutex.unlock();
        = exit_loop_;
    }

    func er_t shut_down( m@* o )
    {
        o.mutex.lock();
        o.exit_loop_ = true;
        o.mutex.unlock();
        o.thread.join();
        if( o.audio ) o.audio.stream_cut();
        o.audio =< NULL;
        o.capture_feed = NULL;
        er_t capture_loop_error_ = o.capture_loop_error_;
        o.capture_loop_error_ = 0;
        = capture_loop_error_;
    }

    func bcore_inst_call.down_e { o.shut_down(); }
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bmedia_audio_in.emb.x";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
