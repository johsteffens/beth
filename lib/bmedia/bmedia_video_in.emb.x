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

include <fcntl.h>;
include <unistd.h>;
include <errno.h>;
include <sys/stat.h>;
include <sys/types.h>;
include <sys/ioctl.h>;
include <linux/videodev2.h>;

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) bcore_inst_call.down_e { o.shut_down(); }

//----------------------------------------------------------------------------------------------------------------------

/// returns errno in case of error
func (:s) er_t ioctl( @*o, u3_t request, vd_t arg )
{
    s2_t result = 0;

    /// retry in case of interrupt
    for( result = 0; ( result = ioctl( o.handle_, request, arg ) ) == -1 && errno == EINTR; ) {};

    if( result == -1 )
    {
        = bcore_error_push_fa( errno, "Device: #<sc_t>\nError #<s2_t>: #<sc_t>\n", o.dev_name.sc, errno, strerror(errno) );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// switches video streaming on
func (:s) stream_on
{
    x_lock_s^ lock.set( o.mutex_ );
    if( !o.is_setup_ )
    {
        x_unlock_s^ unlock.set( o.mutex_ );
        o.setup();
    }
    if( o.is_streaming_ ) = 0;
    enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    o.ioctl( VIDIOC_STREAMON, &buf_type );
    o.is_streaming_ = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/** switches video streaming off
 *  It seems one cannot arbitrarily switch streaming off and back on (without a complete re-setup in-between)
 *  When trying, the video capture seems to run into a dead-loop (error: EAGAIN device busy).
 *  Therefore this function should not be considered an interface function. Call shut_down to stop video streaming.
 */
func (:s) er_t stream_off( m@* o )
{
    if( !o.is_streaming_ ) = 0;
    enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    o.ioctl( VIDIOC_STREAMOFF, &buf_type );
    o.is_streaming_ = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// Shut down functor (for error handling)
stamp :shut_down_s
{
    hidden :s* video;
    func o set( m@* o, m :s* video ) { o.video = video; = o; };
    func bcore_inst_call.down_e { if( o.video ) o.video.shut_down(); };
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) setup
{
    if( o.is_setup_ ) = 0;

    /// shut down functor in case or error
    :shut_down_s^ video_shut_down.set( o );
    x_lock_s^ lock.set( o.mutex_ );

    struct stat device_stat = { 0 };

    if( stat( o.dev_name.sc, &device_stat ) == -1 )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "Device #<sc_t> not found.\n#<s2_t>: #<sc_t>\n", o.dev_name.sc, errno, strerror(errno) );
    }

    if( !S_ISCHR( device_stat.st_mode ) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "Improper device: #<sc_t>.\n#<s2_t>: #<sc_t>\n", o.dev_name.sc, errno, strerror(errno) );
    }

    o.handle_ = open( o.dev_name.sc, O_RDWR | O_NONBLOCK, 0 );

    if( o.handle_ == -1 )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "Failed opening #<sc_t>.\n#<s2_t>: #<sc_t>\n", o.dev_name.sc, errno, strerror(errno) );
    }

    struct v4l2_capability capability = { 0 };

    if( o.ioctl( VIDIOC_QUERYCAP, &capability ) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, ( errno == EINVAL ) ? "Device not supported by V4L2.\n" : "\n" );
    }

    if( !(capability.capabilities & V4L2_CAP_VIDEO_CAPTURE) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "#<sc_t> does not support video capture.\n", o.dev_name.sc );
    }

    if( !(capability.capabilities & V4L2_CAP_STREAMING) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "#<sc_t> does not support video streaming.\n", o.dev_name.sc );
    }

    struct v4l2_format format = { 0 };
    format.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width       = o.requested_width;
    format.fmt.pix.height      = o.requested_height;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    format.fmt.pix.field       = V4L2_FIELD_INTERLACED;

    o.ioctl( VIDIOC_S_FMT, &format );

    o.actual_width  = format.fmt.pix.width;
    o.actual_height = format.fmt.pix.height;

    // Format correction in case of known driver bugs.
    o.bytes_per_line = sz_max( format.fmt.pix.bytesperline, o.actual_width * 2 );
    o.bytes_per_image = sz_max( format.fmt.pix.sizeimage, o.bytes_per_line * o.actual_height );

    struct v4l2_requestbuffers requestbuffers = { 0 };

    requestbuffers.count  = o.buffers;
    requestbuffers.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestbuffers.memory = V4L2_MEMORY_USERPTR;

    if( o.ioctl( VIDIOC_REQBUFS, &requestbuffers) )
    {
        = bcore_error_push_fa( TYPEOF_general_error, ( errno == EINVAL ) ? "User pointer mode not supported.\n" : "" );
    }

    for( sz_t i = 0; i < o.buffers; i++ )
    {
        m :image_s* image = o.image_adl_.push_d( :image_s! ).set_size( o.bytes_per_image );
        image.width = o.actual_width;
        image.height = o.actual_height;
        image.bytes_per_line = o.bytes_per_line;

        struct v4l2_buffer v_buf = { 0 };

        v_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        v_buf.memory = V4L2_MEMORY_USERPTR;
        v_buf.index = i;
        v_buf.m.userptr = ( intptr_t )image.data;
        v_buf.length = image.size;

        o.ioctl( VIDIOC_QBUF, &v_buf );
    }

    video_shut_down.set( NULL );
    o.is_setup_ = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) capture_loop
{
    x_lock_s^ _.set( o.mutex_ );

    if( !o.is_streaming_ )
    {
        x_unlock_s^ _.set( o.mutex_ );
        o.stream_on();
    }

    while( capture_exit ? !capture_exit.capture_exit() : true )
    {
        o.mutex_exit_capture_loop_.lock();
        bl_t exit_loop = o.exit_capture_loop_;
        o.mutex_exit_capture_loop_.unlock();
        if( exit_loop ) break;

        for(;;)
        {
            fd_set fds;

            FD_ZERO( &fds );
            FD_SET( o.handle_, &fds );

            struct timeval timeout = { 0 };
            timeout.tv_sec = 2;
            timeout.tv_usec = 0;

            s2_t result = select( o.handle_ + 1, &fds, NULL, NULL, &timeout );
            if( result == -1 )
            {
                if( errno != EINTR )
                {
                    = bcore_error_push_fa( TYPEOF_general_error, "Select error:\n#<s2_t>: #<sc_t>\n", errno, strerror(errno) );
                }
            }
            else if( result == 0 )
            {
                = bcore_error_push_fa( TYPEOF_general_error, "Select timeout:\n#<s2_t>: #<sc_t>\n", errno, strerror(errno) );
            }
            else
            {
                break;
            }
        }

        struct v4l2_buffer buf = { 0 };
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_USERPTR;

        bl_t valid = true;

        if( o.ioctl( VIDIOC_DQBUF, &buf ) )
        {
            if( errno == EAGAIN ) // no data (yet)
            {
                bcore_error_pop_all();
                bcore_sleep_ms( 100 );
                valid = false;
            }
            else
            {
                = bcore_error_push_fa( TYPEOF_general_error, "Data query error:\n#<s2_t>: #<sc_t>\n", errno, strerror(errno) );
            }
        }

        if( valid )
        {
            foreach( :image_s* e in o.image_adl_ )
            {
                if( buf.m.userptr == (intptr_t)e.data )
                {
                    capture_feed.capture_feed( e );
                    break;
                }
            }
            o.ioctl( VIDIOC_QBUF, &buf );
        }
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) shut_down
{
    o.mutex_exit_capture_loop_.lock();
    o.exit_capture_loop_ = true;
    o.mutex_exit_capture_loop_.unlock();

    x_lock_s^ lock.set( o.mutex_ );
    o.stream_off();
    if( o.handle_ != -1 )
    {
        if( close( o.handle_ ) == -1 )
        {
            = bcore_error_push_fa( TYPEOF_general_error, "#<s2_t>: #<sc_t>\n", errno, strerror(errno) );
        }
        o.handle_ = -1;
    }

    o.mutex_exit_capture_loop_.lock();
    o.exit_capture_loop_ = false;
    o.mutex_exit_capture_loop_.unlock();

    o.is_setup_ = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func u0_t clamp16( s2_t v ) { = s2_min( 255, s2_max( 0, v >> 8 ) ); }

//----------------------------------------------------------------------------------------------------------------------

func (:image_s) convert_to_argb
{
    assert( ( o.width & 1 ) == 0 );
    img.format = TYPEOF_bcore_img_u2_argb;
    img.set_size( o.height, o.width );

    for( sz_t j = 0; j < img.rows; j++ )
    {
        u0_t* yp = o.data + o.bytes_per_line * j;

        m u2_t* u2_row = img.data + img.stride * j;
        for( sz_t i = 0; i < img.cols; i += 2 )
        {
            u2_t y1 = ( ( ( s2_t )yp[ i * 2     ] - 16 ) * 298 ) + 128;
            s2_t u  =     ( s2_t )yp[ i * 2 + 1 ] - 128;
            u2_t y2 = ( ( ( s2_t )yp[ i * 2 + 2 ] - 16 ) * 298 ) + 128;
            s2_t v  =     ( s2_t )yp[ i * 2 + 3 ] - 128;

            s2_t rp =             409 * v;
            s2_t gp = - 100 * u - 208 * v;
            s2_t bp =   516 * u;

            u2_row[ i     ] = bcore_img_u2_pixel_from_rgb( img.format, :clamp16( y1 + rp ), :clamp16( y1 + gp ), :clamp16( y1 + bp ) );
            u2_row[ i + 1 ] = bcore_img_u2_pixel_from_rgb( img.format, :clamp16( y2 + rp ), :clamp16( y2 + gp ), :clamp16( y2 + bp ) );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
