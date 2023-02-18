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

include <alsa/asoundlib.h>; // link with -lasound

//----------------------------------------------------------------------------------------------------------------------

stamp :hwparams_s
{
    private snd_pcm_hw_params_t* v;
    func bcore_inst_call.init_x
    {
        if( snd_pcm_hw_params_malloc( o.v.2 ) ) ERR_fa( "Failed allocating hw_params" );
    }

    func bcore_inst_call.down_e
    {
        if( o.v ) snd_pcm_hw_params_free( o.v.1 );
    }
}

//----------------------------------------------------------------------------------------------------------------------

func er_t error( sc_t context, s2_t errnum )
{
    sc_t snd_msg = snd_strerror( errnum );
    = bcore_error_push_fa( TYPEOF_general_error, "audio_out: #<sc_t>: #<sc_t>\n", context, snd_msg );
}

//----------------------------------------------------------------------------------------------------------------------

stamp :shut_down_s
{
    private :s* v;
    func void setup( m@* o, m :s* v ) { o.v = v; };
    func void clear( m@* o ) { o.v = NULL; };
    func bcore_inst_call.down_e { if( o.v ) o.v.shut_down(); };
}

func (:s) setup
{
    if( o.is_setup_ ) = 0;
    s2_t err = 0;
    err = snd_pcm_open( o.handle_.2, o.device_name.sc, SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK );
    if( err < 0 ) = :error( "snd_pcm_open", err );
    o.is_open_ = true;

    :hwparams_s^ hw_params;
    :shut_down_s^ auto_shut_down.setup( o ); // shut down functor in case of error

    // obtains configuration space for given device
    err = snd_pcm_hw_params_any( o.handle_, hw_params.v );
    if( err < 0 ) = :error( "snd_pcm_hw_params_any", err );

    // restricts configuration space to real hardware rates
    err = snd_pcm_hw_params_set_rate_resample( o.handle_, hw_params.v, 1 );
    if( err < 0 ) = :error( "snd_pcm_hw_params_set_rate_resample", err );

    // restricts configuration space to interleaved sampling (LRLR...)
    err = snd_pcm_hw_params_set_access( o.handle_, hw_params.v, SND_PCM_ACCESS_RW_INTERLEAVED );
    if( err < 0 ) = :error( "snd_pcm_hw_params_set_access", err );

    // sets sample value format to 16bit little-endian
    err = snd_pcm_hw_params_set_format( o.handle_, hw_params.v, SND_PCM_FORMAT_S16_LE );
    if( err < 0 ) = :error( "snd_pcm_hw_params_set_format", err );

    // sets number of channels (2 = stereo)
    err = snd_pcm_hw_params_set_channels( o.handle_, hw_params.v, o.channels );
    if( err < 0 ) = :error( "snd_pcm_hw_params_set_channels", err );

    /* sets sampling rate in hertz
     * 'rate' contains the requested value and is modified to the actual (nearest supported) value
     */
    if( o.requested_rate < 0 )
    {
        unsigned int rate = 0;
        err = snd_pcm_hw_params_get_rate( hw_params.v, &rate, NULL );
        if( err < 0 ) = :error( "snd_pcm_hw_params_get_rate", err );
        o.actual_rate = rate;
    }
    else
    {
        unsigned int rate = o.requested_rate;
        err = snd_pcm_hw_params_set_rate_near( o.handle_, hw_params.v, &rate, NULL );
        if( err < 0 ) = :error( "snd_pcm_hw_params_set_rate", err );
        o.actual_rate = rate;
    }

    if( o.requested_periods < 0 )
    {
        unsigned int periods = 0;
        err = snd_pcm_hw_params_get_periods( hw_params.v, &periods, NULL );
        if( err < 0 ) = :error( "snd_pcm_hw_params_get_periods", err );
        o.actual_periods = periods;
    }
    else
    {
        unsigned int periods = o.requested_periods;
        err = snd_pcm_hw_params_set_periods_near( o.handle_, hw_params.v, &periods, NULL );
        if( err < 0 ) = :error( "snd_pcm_hw_params_set_periods_near", err );
        o.actual_periods = periods;
    }


    if( o.requested_frames_per_period  < 0 )
    {
        snd_pcm_uframes_t buffer_size = 0;
        err = snd_pcm_hw_params_get_buffer_size( hw_params.v, &buffer_size );
        if( err < 0 ) = :error( "snd_pcm_hw_params_get_buffer_size", err );
        o.actual_frames_per_period = buffer_size / o.actual_periods;
    }
    else
    {
        snd_pcm_uframes_t buffer_size = o.actual_periods * o.requested_frames_per_period;
        err = snd_pcm_hw_params_set_buffer_size_near( o.handle_, hw_params.v, &buffer_size );
        if( err < 0 ) = :error( "snd_pcm_hw_params_set_buffer_size_near", err );
        o.actual_frames_per_period = buffer_size / o.actual_periods;
    }

    // applies hardware parameters
    err = snd_pcm_hw_params( o.handle_, hw_params.v );
    if( err < 0 ) = :error( "snd_pcm_hw_params", err );

    auto_shut_down.clear();
    o.is_setup_ = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) shut_down
{
    if( o.is_open_ )
    {
        if( o.is_setup_ )
        {
            if( o.is_streaming_ ) o.stream_cut();
            o.is_setup_ = false;
        }

        s2_t err = 0;
        err = snd_pcm_close( o.handle_.1 );
        if( err ) = :error( "snd_pcm_close", err );
        o.handle_ = 0;
        o.is_open_ = false;
    }

    = 0;
}

func (:s) bcore_inst_call.down_e { o.shut_down(); }

//----------------------------------------------------------------------------------------------------------------------

func (:s) play
{
    o.stream_restart();
    o.stream_play( interleaved_samples, frames );
    o.stream_stop();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) play_sequence
{
    if( sequence.size() == 0 ) = 0;
    if( o.actual_rate != sequence.rate || o.channels != sequence.channels )
    {
        o.shut_down();
        o.requested_rate = sequence.rate;
        o.channels = sequence.channels;
    }
    o.stream_restart();
    o.stream_play_sequence( sequence );
    o.stream_stop();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) stream_start
{
    if( o.is_streaming_ ) = 0;
    if( !o.is_setup_ ) o.setup();
    snd_pcm_start( o.handle_.1 );
    o.is_streaming_ = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) stream_restart
{
    if( o.is_streaming_ ) o.stream_stop();
    o.stream_start();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) stream_play
{
    if( !o.is_setup_ ) o.setup();
    if( !o.is_streaming_ ) o.stream_start();
    s2_t frames_left = frames;
    s1_t* data_ptr = interleaved_samples;
    while( frames_left > 0 )
    {
        snd_pcm_wait( o.handle_.1, 100 );
        s2_t frames = frames_left > o.actual_frames_per_period ? o.actual_frames_per_period : frames_left;
        s2_t frames_written = snd_pcm_writei( o.handle_.1, data_ptr, frames );
        if( frames_written < 0 ) frames_written = snd_pcm_recover( o.handle_.1, frames_written, 1 );

        if( frames_written < 0 )
        {
            switch( frames_written )
            {
                case EBADFD:
                {
                    = bcore_error_push_fa( TYPEOF_general_error, "snd_pcm_writei: PCM is in an improper state." );
                }
                break;

                default:
                {
                    snd_pcm_prepare( o.handle_.1 );
                }
                break;
            }
        }
        else
        {
            data_ptr += frames_written * o.channels;
            frames_left -= frames_written;
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) stream_play_sequence
{
    if( sequence.rate != o.actual_rate )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "stream_play_sequence: Stream rate (#<sz_t>) and audio rate (#<sz_t>) mismatch.\n", sequence.rate, o.actual_rate );
    }

    if( sequence.channels != o.channels )
    {
        = bcore_error_push_fa( TYPEOF_general_error, "stream_play_sequence: Stream channels (#<sz_t>) and audio channels (#<sz_t>) mismatch.\n", sequence.channels, o.channels );
    }
    for( sz_t i = 0; i < sequence.size(); i++ ) o.stream_play_buffer( sequence.c_buffer( i ) );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) stream_stop
{
    if( !o.is_streaming_ ) = 0;
    snd_pcm_drain( o.handle_.1 );
    o.is_streaming_ = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) stream_cut
{
    if( !o.is_streaming_ ) = 0;
    snd_pcm_drop( o.handle_.1 );
    o.is_streaming_ = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) sound_check
{
    ASSERT( o.channels == 2 );
    o.setup();
    sz_t frames = 4 * o.actual_frames_per_period;

    bcodec_audio_buffer_s^ buf.set_size( frames * 2, 2 );

    f3_t pi = 3.1415926535897932384626434;
    f3_t tau = pi * 2;

    f3_t rate = o.actual_rate;
    f3_t f_l = 1000;
    f3_t f_r = 1100;

    s2_t level_l = 16000;
    s2_t level_r = 16000;

    sz_t cycles = 3;

    for( sz_t i = 0; i < frames; i++ )
    {
        buf.[ i * 2 + 0 ] = level_l * sin( i * tau * f_l / rate ) * exp( -3.0 * i / frames );
        buf.[ i * 2 + 1 ] = level_r * sin( i * tau * f_r / rate ) * exp( -6.0 * i / frames );
    }

    for( sz_t j = 0; j < cycles; j++ ) o.stream_play( buf.data, frames );

    o.stream_stop();

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:player_s) setup
{
    o.shut_down();
    o.thread_exit_ = false;
    o.audio =< audio;
    o.audio.setup();
    o.thread.call_m_thread_func( o );
    o.is_setup_ = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:player_s) setup_from_sequence
{
    d$* audio = bmedia_audio_out_s!;
    audio.requested_rate = sequence.rate;
    audio.channels = sequence.channels;
    = o.setup( audio );
}

//----------------------------------------------------------------------------------------------------------------------

func (:player_s) shut_down
{
    if( !o.is_setup_ ) = 0;
    o.mutex.lock();
    o.thread_exit_ = true;
    o.mutex.unlock();
    o.condition_play.wake_one();
    o.thread.join();
    if( o.audio ) o.audio.stream_cut();
    o.audio =< NULL;
    o.is_setup_ = false;
    er_t thread_error_ = o.thread_error_;
    o.thread_error_ = 0;
    = thread_error_;
}

func (:player_s) bcore_inst_call.down_e { o.shut_down(); }

//----------------------------------------------------------------------------------------------------------------------

func (:player_s) m_thread_func
{
    o.mutex.lock();
    while( !o.thread_exit_ )
    {
        if( o.sequence.size() < o.min_size ) o.condition_below_min_size.wake_one();
        if( o.sequence.size() > 0 )
        {
            bcodec_audio_buffer_s* buffer = o.sequence.pop_first_buffer()^;
            o.mutex.unlock();
            er_t thread_error = o.audio.stream_play_buffer( buffer );
            o.mutex.lock();
            if( thread_error )
            {
                o.thread_error_ = thread_error;
                o.thread_exit_ = true;
            }
        }
        else
        {
            if( o.idle_zero_frames > 0 )
            {
                o.mutex.unlock();
                o.audio.stream_play_zero( o.idle_zero_frames );
                o.mutex.lock();
            }
            else
            {
                o.condition_play.sleep( o.mutex );
            }
        }
    };

    if( o.min_size > 0 ) o.condition_below_min_size.wake_one();

    o.audio.stream_stop();
    o.mutex.unlock();
    = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

func (:player_s) play
{
    if( !o.is_setup_ ) o.setup( bmedia_audio_out_s! );
    if( frames == 0 ) = 0;
    o.mutex.lock();

    sz_t channels = o.audio.channels;
    sz_t buf_space = o.buf_frames * channels;

    if( o.sequence.size() == 0 ) o.sequence.push_empty_buffer().set_space( buf_space );

    while( frames > 0 )
    {
        m bcodec_audio_buffer_s* buf = o.sequence.m_last();
        if( !buf || buf.size == buf.space )
        {
            buf = o.sequence.push_empty_buffer().set_space( buf_space );
        }
        sz_t values = sz_min( buf.space - buf.size, frames * channels );
        m s1_t* buf_ptr = buf.data + buf.size;
        assert( buf.size + values <= buf.space );
        for( sz_t i = 0; i < values; i++ ) buf_ptr[ i ] = interleaved_samples[ i ];
        buf.size += values;
        interleaved_samples += values;
        frames -= values / channels;
    }

    o.condition_play.wake_one();
    o.mutex.unlock();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:player_s) play_sequence
{
    if( !o.is_setup_ ) o.setup( bmedia_audio_out_s! );
    if( sequence.size() == 0 ) = 0;
    if( sequence.rate < 0 || sequence.channels < 0 ) ERR_fa( "Sequence rate or channels have not been set." );

    if( o.audio.actual_rate != sequence.rate || o.audio.channels != sequence.channels )
    {
        o.wait_until_empty();
        o.mutex.lock();
        o.audio.shut_down();
        o.audio.requested_rate = sequence.rate;
        o.audio.channels = sequence.channels;
        o.audio.setup();
        o.mutex.unlock();
    }
    for( sz_t i = 0; i < sequence.size(); i++ ) o.play_buffer( sequence.c_buffer( i ) );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:player_s) wait_until_below
{
    o.mutex.lock();
    o.min_size = size;
    while( o.sequence.size() >= o.min_size ) o.condition_below_min_size.sleep( o.mutex );
    er_t thread_error = o.thread_error_;
    o.mutex.unlock();
    = thread_error;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

