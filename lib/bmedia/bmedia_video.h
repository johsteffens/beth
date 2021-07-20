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
 *  - In 'my_stamp':
 *    - Implement feature bmedia_video.capture_feed  (mandatory, should be thread-safe)
 *    - Implement feature bmedia_video.capture_exit  (optional, should be thread-safe)
 *
 *  Setup and start capturing:
 *  - Create instance: bmedia_video_s^ my_video;
 *  - Change parameters in my_video. (if needed)
 *  - Run: my_video.setup()
 *  - Check/Process status values in my_video. (if desired)
 *  - Call my_video.steram_on()  (optional, Turns camera on)
 *  - Call my_video.capture_loop( my_stamp )  (typically from dedicated thread)
 *
 *  Stop capturing (optional):
 *  - Have bmedia_video.capture_exit return 'true'
 *    . This causes my_video.capture_loop to return.
 *    . The camera stays on (!)
 *    . Call my_video.capture_loop to restart capturing.
 *
 *  Clean shut down:
 *  - Destroy my_video or call my_video.shut_down()
 *    . Stops capturing.
 *    . Turns camera off.
 *
 *  References (used for development):
 *    - Info about the V4L API: https://www.kernel.org/doc/html/v4.9/media/uapi/v4l/v4l2.html
 *    - V4L sample application: https://dri.freedesktop.org/docs/drm/media/uapi/v4l/capture.c.html
 */

#ifndef BMEDIA_VIDEO_H
#define BMEDIA_VIDEO_H

#include "bcore_std.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bmedia_video, x_inst )

#ifdef XOILA_SECTION

//----------------------------------------------------------------------------------------------------------------------

stamp :image_s = x_array
{
    sz_t width;
    sz_t height;
    sz_t bytes_per_line;
    u0_t [];

    func (void convert_to_argb( @* o, m bcore_img_u2_s* img ));
};

/** Callback feature. Called when a new image is available.
 *  Image data is laid out in YUYV format.
 */
feature void capture_feed( m@* o, :image_s* image );

/// Optional callback for each loop cycle; return true to exit loop
feature bl_t capture_exit( m@* o ) = { return false; };

stamp :image_adl_s = x_array { :image_s => []; };

stamp :s =
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
    hidden sz_t actual_width    = 0;
    hidden sz_t actual_height   = 0;
    hidden sz_t bytes_per_line  = 0; // bytes per line in image buffer
    hidden sz_t bytes_per_image = 0; // bytes per image in image buffer

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
     *  If another setup (e.g. with different parameters) is desired, call shut_down first.
     */
    func (er_t setup( m@* o));

    /** (Optional) Explicitly switches video streaming on.
     *  Useful to capture additional errors or to explicitly time
     *  camera activity. If not called explicitly, it will be called
     *  by function 'capture_loop'.
     */
    func (er_t stream_on( m@* o ) );

    /** 'capture_loop' calls 'feed.capture_feed' for each new frame.
     *  The loop terminates when 'shut_down' is called or when 'feed.capture_exit()' returns 'false'.
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
    func (er_t capture_loop( m@* o, m:* feed ));

    /** 'shut_down' brings the stamp into a valid state before 'setup'.
     *  Stops capture_loop (if running in a different thread).
     *  Stops video steaming (if running).
     *  Does nothing if stamp was not set up.
     */
    func (er_t shut_down( m@* o));

    /// ==================================

    /// other internal status data
    hidden s2_t handler_ = -1;
    hidden bl_t is_setup_     = false;
    hidden bl_t is_streaming_ = false;
    hidden :image_adl_s image_adl_;

    hidden x_mutex_s mutex_;
    hidden x_mutex_s mutex_exit_capture_loop_;
    hidden bl_t            exit_capture_loop_ = false;
};

//----------------------------------------------------------------------------------------------------------------------

embed "bmedia_video.x";

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif // BMEDIA_VIDEO_H
