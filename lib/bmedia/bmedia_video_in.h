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

/** Video Capture Interface (using the Video for Linux API)
 *
 *  Usage:
 *  - In 'my_capture_stamp':
 *    - Implement feature bmedia_video_in.capture_feed  (mandatory, should be thread-safe)
 *  - In 'my_exit_stamp':
 *    - Implement feature bmedia_video_in.capture_exit  (optional, should be thread-safe)
 *
 *  Setup and start capturing:
 *  - Create instance: bmedia_video_in_s^ my_video;
 *  - Change parameters in my_video. (if needed)
 *  - Run: my_video.setup()
 *  - Check/Process status values in my_video. (if desired)
 *  - Call my_video.steram_on()  (Turns camera on)
 *  - Call my_video.capture_loop( my_capture_stamp, my_exit_stamp (or NULL) )  (This function is typically called from a dedicated thread)
 *
 *  Stop capturing (optional):
 *  - Have bmedia_video_in.capture_exit return 'true'
 *    . This causes my_video.capture_loop to return.
 *    . The camera stays on (!)
 *    . Call my_video.capture_loop to restart capturing.
 *
 *  Clean shut down:
 *  - Destroy my_video or call my_video.shut_down()
 *    . Stops capturing.
 *    . Turns camera off.
 *
 *  Using bmedia_video_in_capture_thread_s
 *  - m$* thread = bmedia_video_in_capture_thread_s!.setup( video, capture_feed );
 *  - When finished capturing simply destroy thread.
 *
 *  Notes:
 *   If the Framerate appears limited while enough CPU is availble:
 *      - Check if camera has slow or adaptive shutter time.
 *      - Check USB bandwith.
 *      - Avoid operating camera on USB hub.
 *
 *  References (used for development):
 *    - Info about the V4L API: https://www.kernel.org/doc/html/v4.9/media/uapi/v4l/v4l2.html
 *    - V4L sample application: https://dri.freedesktop.org/docs/drm/media/uapi/v4l/capture.c.html
 */

#ifndef BMEDIA_VIDEO_IN_H
#define BMEDIA_VIDEO_IN_H

#include "bcore_std.h"

XOILA_DEFINE_GROUP( bmedia_video_in, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :image_s x_array
{
    sz_t width;
    sz_t height;
    sz_t bytes_per_line;
    u0_t [];

    func void convert_to_argb( @* o, m bcore_img_u2_s* img );
}

/** Callback feature. Called when a new image is available.
 *  Image data is laid out in YUYV format.
 */
feature void capture_feed( m@* o, :image_s* image );

/// Optional callback for each loop cycle; return true to exit loop
feature bl_t capture_exit( m@* o ) { = false; };

stamp :image_adl_s x_array { :image_s => []; }

stamp :s
{
    /// =========== parameters ===========

    /// Set these parameters before calling 'setup'
    st_s dev_name = "/dev/video0";
    sz_t requested_width  = 640;
    sz_t requested_height = 480;
    sz_t buffers = 4;

    /// ==================================

    /// =========== status ===============

    /// Check these values after calling 'setup'
    private sz_t actual_width    = 0;
    private sz_t actual_height   = 0;
    private sz_t bytes_per_line  = 0; // bytes per line in image buffer
    private sz_t bytes_per_image = 0; // bytes per image in image buffer

    /// ==================================

    /// =========== functions ============

    /** 'setup' connects to video device and prepares video stamp for capturing.
     *  Fails safely with error message on stack.
     *  Does not start actual streaming.
     *
     *  If setup completes without error, the parameters
     *  actual_width, actual_height, bytes_per_line, bytes_per_image
     *  contain valid values.
     *
     *  Does nothing if stamp has already been set up.
     *  If a follow-up setup (e.g. with different parameters) is desired, call shut_down inbetween.
     */
    func er_t setup( m@* o );

    /** Explicitly switches video streaming on. Usage is optional.
     *  Useful to capture additional errors or to explicitly time
     *  camera activity.
     *  If this function was not called explicitly, it is called
     *  by function 'capture_loop'.
     */
    func er_t stream_on( m@* o );

    /** 'capture_loop' calls 'capture_feed.capture_feed()' for each new frame.
     *  The loop terminates when 'shut_down' is called or when 'capture_exit.capture_exit()' returns 'false'.
     *
     *  Other actions:
     *  Sets up stamp in case it was not yet set up.
     *  Activates video streaming if not already streaming.
     *
     *  Note:
     *  This function may run in a dedicated thread.
     *  Features 'capture_exit' and 'capture_feed' are called directly from this function.
     *  If a condition terminates the loop, it can be restarted.
     */
    func er_t capture_loop( m@* o, m:* capture_feed, m:* capture_exit /* can be NULL */ );

    /** 'shut_down' brings the stamp into a valid state for 'setup'.
     *  Stops capture_loop (if running in a different thread).
     *  Stops video steaming (if running).
     *  Does nothing if stamp was not set up.
     */
    func er_t shut_down( m@* o );

    /// ==================================

    /// other internal status data
    private s2_t handle_ = -1;
    private bl_t is_setup_     = false;
    private bl_t is_streaming_ = false;
    hidden :image_adl_s image_adl_;

    hidden x_mutex_s mutex_;
    hidden x_mutex_s mutex_exit_capture_loop_;
    hidden bl_t            exit_capture_loop_ = false;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Dedicated thread for the capture loop.
 *  Simplifies video setup and control.
 *  Usage:
 *      m$* my_thread = bmedia_video_in_capture_thread_s!;
 *      my_thread.setup( video, capture_feed );
 *
 *  When done, simply destroy my_thread.
 *
 *  - Takes ownership of video object
 *  - No ownership of capture_feed object
 *  - Video stream is started during setup if not started already.
 *  - capture_loop_error_ holds an error occurring in the thread
 */
stamp :capture_thread_s
{
    hidden :s -> video;
    private aware :* capture_feed;

    x_thread_s thread;
    x_mutex_s  mutex;
    bl_t exit_loop_;
    er_t capture_loop_error_ = 0;

    func er_t setup( m@* o, d :s* video, m aware :* capture_feed )
    {
        o.shut_down();
        o.video =< video;
        o.video.stream_on();
        o.capture_feed = capture_feed;
        o.exit_loop_ = false;
        o.thread.call_m_thread_func( o );
        = 0;
    };

    func x_thread.m_thread_func
    {
        er_t error = o.video.capture_loop( o.capture_feed, o );
        if( error )
        {
            o.mutex.lock();
            o.capture_loop_error_ = error;
            o.mutex.unlock();
        }
        = NULL;
    };

    func bmedia_video_in.capture_exit
    {
        o.mutex.lock();
        bl_t exit_loop_ = o.exit_loop_;
        o.mutex.unlock();
        = exit_loop_;
    };

    func er_t shut_down( m@* o )
    {
        o.mutex.lock();
        o.exit_loop_ = true;
        o.mutex.unlock();
        o.thread.join();
        o.video =< NULL;
        o.capture_feed = NULL;
        er_t capture_loop_error_ = o.capture_loop_error_;
        o.capture_loop_error_ = 0;
        = capture_loop_error_;
    };

    func bcore_inst_call.down_e { o.shut_down(); };
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bmedia_video_in.x";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif // BMEDIA_VIDEO_IN_H
