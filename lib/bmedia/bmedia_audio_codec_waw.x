/** Author and Copyright 2022 Johannes Bernhard Steffens
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

/** Beth Audio Codec Waw.
 *
 *  'Waw' denotes the 'sixth iteration' of an audio codec development project in Beth.
 *
 *  Approach:
 *  Basis are techniques that were already well published at or before the year 2000. (I purposely did not
 *  study newer techniques.)
 *
 *  I extended/improved upon those ideas through design and experimentation, thus arriving at this specific
 *  codec solution.
 *
 *  Methods:
 *  'Waw' uses MDCT as principal transformation and subdivides the frequency spectrum in band and sub-bands.
 *  For each band and sub-band, the energy distribution is analyzes in order to determine the best discretization.
 *
 *  The 40DB acoustic equal loudness contour (see: Fletcher-Munson-Characteristic, ISO 226) is used to align
 *  the discretization level with human audio perception.
 *
 *  Pre-Echo is mitigated by analyzing time-loudness variation on slice-level and modulating the compression
 *  rate accordingly.
 *
 *  Audio data is divided into multiple sections (called slices) of equal size representing and typically 20..50ms
 *  playtime. A slice holds the amount of frames to perform the MDCT. Multiple slices (typically 10 ... 50) are
 *  grouped into a page. An encoded audio sequence consists of a sequence of pages.
 *
 *  The discretized data is packed, runlength encoded and/or huffman encoded (whichever combination yields
 *  highest compactness) on page-level.
 *
 *  The framework supports any amount of audio channels. Encoder and decoder achieve acceptable speeds:
 *  Per available cpu core typically >50x realtime play or record speed.
 *  Both are multi threaded.
 *
 *  The behavior and average compression rate is widely configurable.
 *
 *  Compression rate and quality level:
 *  This codec exhibits variable compression rates depending on audio content.
 *  Typical (usable) compression rates range between 56 ... 240 kbps (average on 2-channels at 44.1 kHz).
 *  At 240kbps the difference to the original is practically imperceptible.
 *
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// transient types map
trans
(
    Tparam    :param_s,
    Tcontext  :context_s,
    Tencoder  :encoder_s,
    Tdecoder  :decoder_s,
    Tsequence :sequence_s,
    Tpage     :page
);

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :context_s
{
    bmath_cosine_mdct_f2_s mdct;
    bmath_vf2_s mdct_vec;
    bmedia_audio_buffer_s audio_buffer;
    bmath_vf2_s audio_vec;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :window_function =
{
    feature vec gen( @* o, sz_t size, m bmath_vf2_s* vec );

    stamp :const_s func :.gen
    {
        vec.set_size( size );
        f3_t v = f3_srt( 0.5 );
        for( sz_t i = 0; i < size; i++ ) { vec.[ i ] = v; }
        = vec;
    };

    stamp :cosine_s func :.gen
    {
        bmath_cf3_s^ c.urt( 1, size * 4 );
        bmath_cf3_s^ w.urt( 1, size * 2 );
        vec.set_size( size );
        for( sz_t i = 0; i < size; i++ ) { vec.[ i ] = c.i; c.mul( w, c ); }
        = vec;
    };

    stamp :sine_cosine_s func :.gen
    {
        bmath_cf3_s^ c.urt( 1, size * 4 );
        bmath_cf3_s^ w.urt( 1, size * 2 );
        vec.set_size( size );
        for( sz_t i = 0; i < size; i++ ) { vec.[ i ] = sin( 0.5 * f3_pi() * c.i * c.i ); c.mul( w, c ); }
        = vec;
    };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** codec parameters
 *  The default setting represent high quality around 240kbps (varying depending on content)
 */
group :param
{
    stamp :band_s
    {
        /// frequency-span in octaves (per mille)
        $ s3_t frequency           =    0; // start frequency
        $ sz_t subband_size        =   20;
        $ f3_t band_discretization =  500; // discretization within a band
        $ f3_t subb_discretization =   16; // discretization within a sub band
        $ f3_t zero_ratio_band     = 0.01; // if band signal / global signal < zero_ratio_band, the entire band is zeroed
        $ f3_t zero_ratio_subb     = 0.01; // if sub band signal / band signal < zero_ratio_subb, the entire sub band is zeroed

        // values below are computed during setup from band parameters
        hidden sz_t subbands; // number of subbands
        hidden sz_t index;    // index into frequency domain
    }
    stamp :band_arr_s x_array { :band_s []; }

    //------------------------------------------------------------------------------------------------------------------

    stamp :fv_s { $ f3_t f; $ f3_t v; }
    stamp :loudness_mask_s x_array
    {
        :fv_s [];
        func d bmath_spliced_func_c0_s* log_spliced_func( @* o )
        {
            d $* f = bmath_spliced_func_c0_s!;
            foreach( $*e in o ) f.push_xy( f3_log10( e.f ), e.v );
            = f;
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    stamp :shadow_s
    {
        f3_t timing   =  0.02;  // window size to resolve timing effects (in seconds)
        f3_t exponent =  12.0;   // exponent to compute shadow factor: factor = 1 + ( shadow_factor - 1 ) * ( shadow ^ exponent )
        f3_t factor_discretization = 40.0;  // factor at which subb_discretization is to be changed in the presence of a shadow
        f3_t factor_zero_ratio     =  0.1;  // factor at which subb_zero_ratio is to be changed in the presence of a shadow

        func f3_t discretization_factor( @* o, f3_t shadow ) = 1.0 + ( o.factor_discretization - 1.0 ) * shadow;
        func f3_t     zero_ratio_factor( @* o, f3_t shadow ) = 1.0 + ( o.factor_zero_ratio     - 1.0 ) * shadow;
    }

    //------------------------------------------------------------------------------------------------------------------

    stamp :s
    {
        s3_t channels                = 2;     // will be overwritten
        s3_t rate                    = 44100; // will be overwritten
        s3_t slices_per_page         = 25;    // number of slices per page (half_frames * channels) combined to a page

        s3_t frames                  = 4096;
        s3_t min_frequency           = 0;
        s3_t max_frequency           = 16000; // -1: unlimited
        f3_t signal_exponent         = 1.0;   // x -> sign(x)*|x|^signal_exponent
        f3_t loudness_equalization   = 1.0;   // 0.0 ... 1.0 exponent at which loudness values are equalized: 0.0 = no equalization; 1.0 : full equalization
        f3_t loudness_masking        = 1.0;   // 0.0 ... 1.0
        f3_t gain                    = 1.0;   // global gain (used to compensate intensity distortions due to signal_exponent)

        f3_t global_discretization   = 10000; // global discretization
        f3_t zero_base_global        = 0.01;  // if base_global < zero_base_global the entire signal is considered zero

        :shadow_s shadow;

        :band_arr_s bands;

        :loudness_mask_s => loudness_mask;

        s2_t rle_min_length = 4;     // RLE block: minimum length
        s2_t rle_max_length = 32767; // RLE block: maximum length

        tp_t tp_page = :page_stage30_s;
        tp_t tp_window_function = ::window_function_sine_cosine_s;

        s1_t encoder_threads = 10;
        s1_t decoder_threads = 10;

        bl_t mix_dual_channels  = false; // in case of two channels apply mixing transformation: A + B, A - B
        bl_t collect_statistics = false;

        func bmedia_audio_codec_param.frames_per_slice = o.frames >> 1;
        func bmedia_audio_codec_param.frames_per_page = o.frames_per_slice() * o.slices_per_page;

        /// runtime data usable after setup()
        hidden bmath_vf2_s => loudness_mask_vec;
        hidden bmath_vf2_s => equal_loudness_vec;
        hidden bmath_vf2_s => window_function_vec;
        hidden bl_t is_setup;

        func d ::window_function* create_window_function( @* o ) = x_inst_create( o.tp_window_function );

        /// Interface
        func bmedia_audio_codec_param.setup;
        func bmedia_audio_codec_param.create_page;
        func bmedia_audio_codec_param.create_encoder;
        func bmedia_audio_codec_param.create_decoder;
        func bmedia_audio_codec_param.create_context;
        func bmedia_audio_codec_param.set_rate     o.rate     = rate;
        func bmedia_audio_codec_param.set_channels o.channels = channels;
        func bmedia_audio_codec_param.get_rate     = o.rate;
        func bmedia_audio_codec_param.get_channels = o.channels;
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:s) bmedia_audio_codec_param.create_page
    {
        if( !x_inst_exists( o.tp_page ) ) ERR_fa( "'tp_page' does not hold a valid type." );
        = x_inst_create( o.tp_page );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:s) bmedia_audio_codec_param.create_encoder
    {
        = ::encoder_s!.set_param( o );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:s) bmedia_audio_codec_param.create_decoder
    {
        = ::decoder_s!;
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:s) bmedia_audio_codec_param.create_context
    {
        = ::context_s!;
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:s) o setup_default_param_band( m@* o )
    {
        ///        frequency, subband_size, band_discretization, subb_discretization, zero_ratio_band, zero_ratio_subb
        o.bands.clear();
        o.bands.push()._(     0,     10,          256,                10.0,                0.000,            0.00           );
        o.bands.push()._(   220,     20,          256,                10.0,                0.001,            0.00           );
        o.bands.push()._(  1000,     32,          256,                10.0,                0.001,            0.01           );
        o.bands.push()._(  5000,     64,          256,                10.0,                0.002,            0.02           );
        o.bands.push()._( 12000,     64,          256,                 4.0,                0.1,              0.1            );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:s) o setup_default_loudness_mask( m@* o )
    {
        m$* m = o.loudness_mask!.clear();
        m.push()._(    10, 0.0 );
        m.push()._(    15, 0.0 );
        m.push()._(    20, 0.8 );
        m.push()._(    50, 1.0 );
        m.push()._( 14000, 1.0 );
        m.push()._( 16000, 0.5 );
        m.push()._( 20000, 0.0 );
        m.push()._( 21000, 0.0 );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:s) bmedia_audio_codec_param.setup
    {
        if( o.is_setup ) = o;
        o.is_setup = true;

        o.window_function_vec =< o.create_window_function()^.gen( o.frames, bmath_vf2_s! );
        o.window_function_vec.mul_scl_fx( f2_srt( 2.0 ), o.window_function_vec ); // factor srt(2) needed for correct scale reconstruction

        if( o.bands.size == 0 ) o.setup_default_param_band();
        if( !o.loudness_mask ) o.setup_default_loudness_mask();

        {
            sz_t prev_size = sz_min( o.frames >> 1, ( o.frames * o.max_frequency ) / o.rate );
            sz_t idx1 = sz_min( prev_size, ( o.frames * o.bands.[ 0 ].frequency ) / o.rate );
            idx1 -= idx1 % o.bands.[ 0 ].subband_size;
            for( sz_t idx_b = 0; idx_b < o.bands.size; idx_b++ )
            {
                m :band_s* band = o.bands.[ idx_b ];
                s3_t end_frequency = ( idx_b + 1 ) < o.bands.size ? o.bands.[ idx_b + 1 ].frequency : o.max_frequency;
                sz_t idx2 = sz_min( prev_size, ( o.frames * end_frequency ) / o.rate );
                idx2 -= ( idx2 - idx1 ) % band.subband_size;
                band.index    = idx1;
                band.subbands = ( idx2 - idx1 ) / band.subband_size;
                idx1 += band.subbands * band.subband_size;
            }
        }

        {
            bmedia_iso226_eql_list_s^ list;

            $* equal_loudness_func = list.eql_func_log10_f_spl( bmath_spliced_func_c1_s!^, 40 ); // 40DB curve
            $* loudness_mask_func = o.loudness_mask.log_spliced_func()^;

            f3_t loudness_1khz = equal_loudness_func( f3_log10( 1000 ) );

            sz_t half_frames = o.frames >> 1;

            o.equal_loudness_vec =< bmath_vf2_s!.set_size( half_frames );
            o.loudness_mask_vec  =< bmath_vf2_s!.set_size( half_frames );
            for( sz_t i = 0; i < half_frames; i++ )
            {
                f3_t f = ( ( o.rate.cast( f3_t ) * 0.5 ) * ( i + 1.0 ) ) / o.equal_loudness_vec.size;
                f3_t equal_loudness = f3_pow( 10.0, ( equal_loudness_func( f3_log10( f ) ) - loudness_1khz ) * 0.05 * o.loudness_equalization );
                f3_t loudness_mask = f3_max( 0, f3_min( 1.0, loudness_mask_func( f3_log10( f ) ) ) ) * o.loudness_masking + ( 1.0 - o.loudness_masking );

                o.equal_loudness_vec.[ i ] = equal_loudness;
                o.loudness_mask_vec.[ i ] = o.gain * loudness_mask / equal_loudness;
            }
        }
        =o;
    }

    //------------------------------------------------------------------------------------------------------------------
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// s2-discretization in dedicated banks; RLE codec
stamp :bank_s x_array
{
    s2_t [];
    func o push( m@* o, s2_t v ) o.push_c( v );
    func o push_bank( m@* o, @* src ) foreach( $ e in src ) o.push( e );
    func o copy_bank_section( m@* o, @* src, sz_t start, sz_t size )
    {
        size = sz_min( size, src.size - start );
        o.set_size( size );
        for( sz_t i = 0; i < size; i++ ) o.[ i ] = src.[ i + start ];
    }

    // bits: bits used to store the mantissa
    func void push_f3( m@* o, f3_t v, sz_t bits )
    {
        ASSERT( bits <= 15 );
        s2_t exp_s2 = 0;
        s2_t man = lrint( frexp( v, exp_s2.1 ) * ( ( 1 << bits ) - 1 ) );
        s2_t exp = exp_s2;
        o.push_c( man );
        o.push_c( exp );
    };

    func f3_t read_f3( c@* o, sz_t bits, m sz_t* index )
    {
        ASSERT( o.size >= 2 );
        ASSERT( bits <= 15 );
        s2_t man = o.[ index.0     ];
        s2_t exp = o.[ index.0 + 1 ];
        index.0 += 2;
        return ( 1.0 / ( ( 1 << bits ) - 1 ) ) * man * pow( 2.0, exp );
    };

    func f3_t energy( @* o ) { f3_t sum = 0; foreach( $e in o ) { sum += f3_sqr( e ); } = sum; }
}

//------------------------------------------------------------------------------------------------------------------

func (:bank_s) o rle_encode( m@* o, @* src, :param_s* param )
{
    if( o == src ) src = o.clone()^^;

    /// escape character is the least used and closest to 0 value within -64 and 63.
    s2_t escape = -64;
    {
        bcore_arr_sz_s^ hist.set_size( 128 );
        for( sz_t i = 0; i < src.size; i++ )
        {
            s2_t v = src.[ i ];
            if( v >= -64 && v <= 63 ) hist.[ 64 + v ]++;
        }

        sz_t esc_count = src.size + 1;
        escape = 0;

        for( sz_t i = 0; i < hist.size; i++ )
        {
            s2_t esc = i - 64;
            if( hist.[ i ] < esc_count )
            {
                esc_count = hist.[ i ];
                escape = esc;
            }
            else if( hist.[ i ] == esc_count )
            {
                escape = s2_abs( esc ) < s2_abs( escape ) ? esc : escape;
            }
        }
    }

    o.set_space( src.size ).set_size( 0 ).push( escape );

    s2_t v_last = 0;
    sz_t v_count = 0;

    for( sz_t i = 0; i < src.size; i++ )
    {
        s2_t v = src.[ i ];
        if( ( v_count > 0 && v_last != v ) || v_count == param.rle_max_length || i == src.size - 1 )
        {
            if( v_count >= param.rle_min_length )
            {
                o.push( escape ).push( v_last ).push( v_count - param.rle_min_length );
            }
            else
            {
                for( sz_t i = 0; i < v_count; i++ ) o.push( v_last );
            }
            v_count = 0;
        }

        if( v == escape )
        {
            o.push( escape ).push( escape );
        }
        else
        {
            v_count += ( v_count == 0 || v_last == v );
            v_last = v;
        }
    }
    if( v_count > 0 ) o.push( v_last ); // v_count can only be 0 or 1 at this point
}

//------------------------------------------------------------------------------------------------------------------

func (:bank_s) o rle_decode( m@* o, @* src, :param_s* param )
{
    if( o == src ) src = o.clone()^^;
    s2_t escape = src.[ 0 ];
    o.set_space( src.size ).set_size( 0 );

    for( sz_t i = 1; i < src.size; i++ )
    {
        if( src.[ i ] != escape )
        {
            o.push( src.[ i ] );
        }
        else
        {
            s2_t v = src.[ ++i ];
            if( v != escape )
            {
                sz_t v_count = src.[ ++i ] + param.rle_min_length;
                for( sz_t j = 0; j < v_count; j++ ) o.push( v );
            }
            else
            {
                o.push( v );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bank_s) sz_t bits_per_value( c@* o )
{
    s2_t max = 0;
    s2_t min = 0;
    foreach( $e in o ) { max = s2_max( max, e ); min = s2_min( min, e ); }

    sz_t bits = 0;
    while( ( max >> bits ) > 0 || ( min >> bits ) < -1 ) bits++;
    = ( max != min ) ? bits + 1 : 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:bank_s) bit_buffer encode_to_bit_buffer( c@* o, :param_s* param, m :context_s* context, m bcore_huffman_bit_buffer_s* bit_buffer )
{
    @* bank = o;

    @^ bank_rle.rle_encode( bank, param );
    bl_t use_rle = ( bank_rle.size < bank.size );
    bank = use_rle ? bank_rle.1 : bank;

    bcore_huffman_bit_buffer_s^ bit_buffer_huffman;
    bcore_huffman_codec_s^ codec.scan_start();
    foreach( $e in bank ) codec.scan_s2( e );
    codec.scan_end().encode( bit_buffer_huffman );
    foreach( $e in bank ) codec.encode_s2( e, bit_buffer_huffman );

    sz_t bits_per_value = bank.bits_per_value();
    bl_t use_huffman = bit_buffer_huffman.size < bits_per_value * bank.size;
    bit_buffer.push_bl( use_huffman );
    bit_buffer.push_bl( use_rle );
    bit_buffer.push_packed_u3( bank.size );

    if( use_huffman )
    {
        bit_buffer.push_bit_buffer( bit_buffer_huffman );
    }
    else
    {
        bit_buffer.push_u3( bits_per_value, 5 );
        foreach( $e in bank ) bit_buffer.push_s3( e, bits_per_value );
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:bank_s) o decode_from_bit_buffer( m@* o, :param_s* param, m :context_s* context, m bcore_huffman_bit_buffer_iterator_s* iterator )
{
    bl_t use_huffman = iterator.read_bl();
    bl_t use_rle     = iterator.read_bl();
    sz_t bank_size   = iterator.read_packed_u3();
    @^ bank.set_size( bank_size );

    if( use_huffman )
    {
        bcore_huffman_codec_s^ codec.decode( iterator );
        foreach( m$* e in bank ) e.0 = codec.decode_s2( iterator );
    }
    else
    {
        sz_t bits_per_value = iterator.read_u3( 5 );
        foreach( m$* e in bank ) e.0 = iterator.read_s3( bits_per_value );
    }

    if( use_rle )
    {
        o.rle_decode( bank, param );
    }
    else
    {
        o.copy( bank );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :slice
{
    feature o encode( m@* o, c bmath_vf2_s* vec, ::param_s* param, m ::context_s* context );
    feature o decode( c@* o, m bmath_vf2_s* vec, ::param_s* param, m ::context_s* context );

    func f2_t signal_pow( f2_t signal, f2_t exponent ) = ( signal >= 0 ) ? pow( signal, exponent ) : -pow( -signal, exponent );

    //------------------------------------------------------------------------------------------------------------------

    /** Initial Interface
     *  Window Function application
     */
    stamp :stage10_s
    {
        f3_t shadow;
        bmath_vf2_s vec;

        func f3_t shadow( @* o, c bmath_vf2_s* vec, ::param_s* param )
        {
            sz_t size = sz_min( vec.size, f3_rs2( param.shadow.timing * param.rate ) );

            f3_t inv_size = 1.0 / size;

            f3_t s1 = 0;
            f3_t s2 = 0;

            for( sz_t i = 0; i < size; i++ )
            {
                s1 += vec.[ i ];
                s2 += f3_sqr( vec.[ i ] );
            }

            f3_t vol = ( s2 * inv_size ) - f3_sqr( s1 * inv_size );
            f3_t vol_sum = vol;
            f3_t vol_min = vol;

            for( sz_t i = size; i < vec.size; i++ )
            {
                s1 +=         vec.[ i ]   -         vec.[ i - size ];
                s2 += f3_sqr( vec.[ i ] ) - f3_sqr( vec.[ i - size ] );
                vol = ( s2 * inv_size ) - f3_sqr( s1 * inv_size );
                vol_sum += vol;
                vol_min = f3_min( vol, vol_min );
            }

            f3_t evenly = vol_min > 0 ? ( vol_min * ( vec.size - size + 1 ) ) / vol_sum : 1.0;
            return f3_pow( 1.0 - evenly, param.shadow.exponent );
        }

        func :.encode
        {
            o.shadow = o.shadow( vec, param );
            param.window_function_vec.mul_hdm( vec, o.vec.set_size( param.frames ) );
        }

        func :.decode
        {
            param.window_function_vec.mul_hdm_add( o.vec, vec, vec );
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    /******************************************************************************************************************/

    //------------------------------------------------------------------------------------------------------------------

    /** MDCT
     *  Frequency selection
     *  Frequency hull application
     *  Loudness equalization
     */
    stamp :stage20_s x_array
    {
        f3_t shadow;
        f2_t [];

        //--------------------------------------------------------------------------------------------------------------

        func f3_t energy( @*o, sz_t idx, sz_t size )
        {
            f3_t energy = 0;
            size = sz_min( size, o.size - idx );
            for( sz_t i = 0; i < size; i++ ) energy += f3_sqr( o.[ i + idx ] );
            = energy;
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage20_s) :.encode
    {
        :stage10_s^ prev.encode( vec, param, context );
        o.shadow = prev.shadow;

        sz_t frames = param.frames;
        ASSERT( prev.vec.size == frames );
        o.set_size( param.frames >> 1 );
        context.mdct.ffv( prev.vec, context.mdct_vec );
        for( sz_t i = 0; i < o.size; i++ )
        {
            o.[ i ] = :signal_pow( context.mdct_vec.[ i ] * param.loudness_mask_vec.[ i ], param.signal_exponent );
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage20_s) :.decode
    {
        ASSERT( o.size == param.frames >> 1 );

        f3_t inv_signal_exponent = 1.0 / param.signal_exponent;

        context.mdct_vec.set_size( param.frames >> 1 );
        for( sz_t i = 0; i < o.size; i++ )
        {
            context.mdct_vec.[ i ] = :signal_pow( o.[ i ], inv_signal_exponent ) * param.equal_loudness_vec.[ i ];
        }

        :stage10_s^ prev;
        prev.shadow = o.shadow;
        context.mdct.fiv( context.mdct_vec, prev.vec );

        prev.decode( vec, param, context );
    }

    //------------------------------------------------------------------------------------------------------------------

    /******************************************************************************************************************/

    //------------------------------------------------------------------------------------------------------------------

    /// Band and Sub-band processing
    stamp :subb_s x_array
    {
        f3_t base;
        s1_t [];
        func o zro( m@* o ) { o.base = 0; foreach( m$* e in o ) e.0 = 0; }
    }

    stamp :band_s x_array
    {
        f3_t base;
        f3_t shadow;
        :subb_s [];
        func o zro( m@* o ) { o.base = 0; foreach( m$* e in o ) e.zro(); }
        func o alloc( m@* o, sz_t subbs, sz_t subb_size ) { o.set_size( subbs ); foreach( m$*e in o ) e.set_size( subb_size ); }
    }

    stamp :stage30_s x_array
    {
        f3_t base;
        f3_t shadow;
        :band_s [];
        func o alloc( m@* o, ::param_s* param )
        {
            o.set_size( param.bands.size );
            for( sz_t i = 0; i < param.bands.size; i++ ) o.[ i ].alloc( param.bands.[ i ].subbands, param.bands.[ i ].subband_size );
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage30_s) :.encode
    {
        :stage20_s^ prev.encode( vec, param, context );
        o.shadow = prev.shadow;
        o.alloc( param );
        o.base = f3_srt( prev.energy( 0, prev.size ) );
        f3_t shadow_discretization_factor = param.shadow.discretization_factor( o.shadow );
        for( sz_t i = 0; i < param.bands.size; i++ )
        {
            m :band_s* band = o.[ i ];
            ::param_band_s* param_band = param.bands.[ i ];
            band.base = f3_srt( prev.energy( param_band.index, param_band.subbands * param_band.subband_size ) );
            for( sz_t i = 0; i < param_band.subbands; i++ )
            {
                m :subb_s* subb = band.[ i ];
                sz_t idx = param_band.index + param_band.subband_size * i;
                f3_t base_subb = f3_srt( prev.energy( idx, param_band.subband_size ) );
                f3_t discretization = f3_min( param_band.band_discretization, param_band.subb_discretization * shadow_discretization_factor );
                f3_t factor_subb = ( base_subb > 0 ) ? discretization / base_subb : 0;
                subb.base = base_subb;
                bl_t all_zero = true;
                for( sz_t i = 0; i < subb.size; i++ )
                {
                    subb.[ i ] = f3_rs2( prev.[ idx + i ] * factor_subb );
                    all_zero = all_zero && ( subb.[ i ] == 0 );
                }
                if( all_zero ) subb.base = 0;
            }
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage30_s) :.decode
    {
        :stage20_s^ prev.set_size( param.frames >> 1 );
        prev.shadow = o.shadow;
        ASSERT( o.size == param.bands.size );

        f3_t shadow_discretization_factor = param.shadow.discretization_factor( o.shadow );
        for( sz_t i = 0; i < param.bands.size; i++ )
        {
            :band_s* band = o.[ i ];
            ::param_band_s* param_band = param.bands.[ i ];
            ASSERT( band.size == param_band.subbands );
            for( sz_t i = 0; i < param_band.subbands; i++ )
            {
                :subb_s* subb = band.[ i ];
                ASSERT( subb.size == param_band.subband_size );
                sz_t idx = param_band.index + param_band.subband_size * i;
                f3_t discretization = f3_min( param_band.band_discretization, param_band.subb_discretization * shadow_discretization_factor );
                f3_t factor = ( discretization > 0 ) ? subb.base / discretization : 0;
                for( sz_t i = 0; i < subb.size; i++ ) prev.[ idx + i ] = subb.[ i ] * factor;
            }
        }
        prev.decode( vec, param, context );
    }

    //------------------------------------------------------------------------------------------------------------------

    /******************************************************************************************************************/

    //------------------------------------------------------------------------------------------------------------------

    /******************************************************************************************************************/
    /// bank encoding
    //------------------------------------------------------------------------------------------------------------------

    func (::param_s) sz_t banks( @* o ) = 2 + o.bands.size;

    stamp :stage40_s x_array
    {
        f3_t base;
        f3_t shadow;
        ::bank_s [];
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage40_s) :.encode
    {
        :stage30_s^ prev.encode( vec, param, context );

        f3_t base_global = ( prev.base > param.zero_base_global ) ? prev.base : 0;
        o.base = base_global;
        o.shadow = prev.shadow;

        o.set_size( param.banks() );

        f3_t shadow_zero_ratio_factor = param.shadow.zero_ratio_factor( o.shadow );

        if( base_global > 0 )
        {
            m ::bank_s* enc_base_band = o.[ 0 ];
            m ::bank_s* enc_base_subb = o.[ 1 ];
            for( sz_t i = 0; i < param.bands.size; i++ )
            {
                ::param_band_s* param_band = param.bands.[ i ];
                :band_s* band = prev.[ i ];
                f3_t band_ratio = band.base / base_global;
                s1_t base_band = ( band_ratio > param_band.zero_ratio_band ) ? f3_rs2( band_ratio * param.global_discretization ) : 0;
                enc_base_band.push( base_band );
                if( base_band > 0 )
                {
                    m ::bank_s* bank = o.[ 2 + i ];
                    bank.set_space( param_band.subbands * param_band.subband_size );
                    for( sz_t i = 0; i < param_band.subbands; i++ )
                    {
                        :subb_s* subb = band.[ i ];
                        f3_t subb_ratio = subb.base / band.base;
                        s1_t base_subb = ( subb_ratio > param_band.zero_ratio_subb * shadow_zero_ratio_factor ) ? f3_rs2( subb_ratio * param_band.band_discretization ) : 0;
                        enc_base_subb.push( base_subb );
                        if( base_subb > 0 )
                        {
                            for( sz_t i = 0; i < param_band.subband_size; i++ ) bank.push( subb.[ i ] );
                        }
                    }
                }
            }
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage40_s) :.decode
    {
        :stage30_s^ prev.alloc( param );

        if( o.size > 0 )
        {
            f3_t base_global = o.base;
            prev.base = base_global;
            prev.shadow = o.shadow;

            if( base_global > 0 )
            {
                sz_t read_base_band = 0;
                sz_t read_base_subb = 0;
                ::bank_s* enc_base_band = o.[ 0 ];
                ::bank_s* enc_base_subb = o.[ 1 ];
                for( sz_t i = 0; i < param.bands.size; i++ )
                {
                    ::param_band_s* param_band = param.bands.[ i ];
                    m :band_s* band = prev.[ i ];
                    s1_t base_band = enc_base_band.[ read_base_band++ ];
                    band.base = ( base_band * prev.base ) / param.global_discretization;
                    if( base_band > 0 )
                    {
                        m ::bank_s* bank = o.[ 2 + i ];
                        sz_t read1 = 0;

                        for( sz_t i = 0; i < param_band.subbands; i++ )
                        {
                            m :subb_s* subb = band.[ i ];
                            s1_t base_subb = enc_base_subb.[ read_base_subb++ ];
                            subb.base = ( base_subb * band.base ) / param_band.band_discretization;
                            if( base_subb > 0 )
                            {
                                for( sz_t i = 0; i < param_band.subband_size; i++ ) subb.[ i ] = bank.[ read1++ ];
                            }
                        }
                    }
                }
            }
        }
        prev.decode( vec, param, context );
    }

    //------------------------------------------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :page bmedia_audio_codec_page
{

    //------------------------------------------------------------------------------------------------------------------

    /******************************************************************************************************************/

    //------------------------------------------------------------------------------------------------------------------

    /// Slice level encoding with channel mixing
    stamp :stage10_s x_array
    {
        ::slice_stage40_s [];

        func void dmix( m bmath_vf2_s* va, m bmath_vf2_s* vb )
        {
            for( sz_t i = 0; i < va.size; i++ )
            {
                f2_t a = va.[ i ];
                f2_t b = vb.[ i ];
                va.[ i ] = a + b;
                vb.[ i ] = a - b;
            }
        }

        //--------------------------------------------------------------------------------------------------------------

        func void dumix( m bmath_vf2_s* va, m bmath_vf2_s* vb )
        {
            for( sz_t i = 0; i < va.size; i++ )
            {
                f2_t a = va.[ i ];
                f2_t b = vb.[ i ];
                va.[ i ] = ( a + b ) * 0.5;
                vb.[ i ] = ( a - b ) * 0.5;
            }
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage10_s) bmedia_audio_codec_page.encode
    {
        ASSERT( param.is_setup );
        sz_t half_frames = param.frames >> 1;
        o.set_size( param.channels * param.slices_per_page );
        if( param.mix_dual_channels && param.channels == 2 )
        {
            bmath_vf2_s^ vec_a.set_size( param.frames );
            bmath_vf2_s^ vec_b.set_size( param.frames );
            for( sz_t slice = 0; slice < param.slices_per_page; slice++ )
            {
                indexer.get_buffer( context.audio_buffer, offset + ( slice - 1 ) * half_frames, param.frames );
                context.audio_buffer.get_vf2( vec_a, 0 );
                context.audio_buffer.get_vf2( vec_b, 1 );
                o.dmix( vec_a, vec_b );
                o.[ slice * param.channels + 0 ].encode( vec_a, param, context );
                o.[ slice * param.channels + 1 ].encode( vec_b, param, context );
            }
        }
        else
        {
            for( sz_t slice = 0; slice < param.slices_per_page; slice++ )
            {
                indexer.get_buffer( context.audio_buffer, offset + ( slice - 1 ) * half_frames, param.frames );
                for( sz_t channel = 0; channel < param.channels; channel++ )
                {
                    context.audio_buffer.get_vf2( context.audio_vec, channel );
                    o.[ slice * param.channels + channel ].encode( context.audio_vec, param, context );
                }
            }
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage10_s) bmedia_audio_codec_page.decode
    {
        ASSERT( param.is_setup );
        sz_t half_frames = param.frames >> 1;
        ASSERT( o.size == param.channels * param.slices_per_page );
        if( param.mix_dual_channels && param.channels == 2 )
        {

            bmath_vf2_s^ vec_a.set_size( param.frames );
            bmath_vf2_s^ vec_b.set_size( param.frames );
            for( sz_t slice = 0; slice < param.slices_per_page; slice++ )
            {
                if( left_mutex &&  slice == 0 ) left_mutex.lock();
                if( right_mutex && slice == param.slices_per_page -1 ) right_mutex.lock();

                indexer.get_buffer( context.audio_buffer, offset + ( slice - 1 ) * half_frames, param.frames );
                context.audio_buffer.get_vf2( vec_a, 0 );
                context.audio_buffer.get_vf2( vec_b, 1 );
                o.dmix( vec_a, vec_b );
                o.[ slice * param.channels + 0 ].decode( vec_a, param, context );
                o.[ slice * param.channels + 1 ].decode( vec_b, param, context );
                o.dumix( vec_a, vec_b );
                context.audio_buffer.set_from_vf2( vec_a, 0 );
                context.audio_buffer.set_from_vf2( vec_b, 1 );
                indexer.set_from_buffer( context.audio_buffer, offset + ( slice - 1 ) * half_frames );

                if( left_mutex &&  slice == 0 ) left_mutex.unlock();
                if( right_mutex && slice == param.slices_per_page -1 ) right_mutex.unlock();
            }
        }
        else
        {
            for( sz_t slice = 0; slice < param.slices_per_page; slice++ )
            {
                if( left_mutex &&  slice == 0 ) left_mutex.lock();
                if( right_mutex && slice == param.slices_per_page -1 ) right_mutex.lock();

                indexer.get_buffer( context.audio_buffer, offset + ( slice - 1 ) * half_frames, param.frames );
                for( sz_t channel = 0; channel < param.channels; channel++ )
                {
                    context.audio_buffer.get_vf2( context.audio_vec, channel );
                    o.[ slice * param.channels + channel ].decode( context.audio_vec, param, context );
                    context.audio_buffer.set_from_vf2( context.audio_vec, channel );
                }
                indexer.set_from_buffer( context.audio_buffer, offset + ( slice - 1 ) * half_frames );

                if( left_mutex &&  slice == 0 ) left_mutex.unlock();
                if( right_mutex && slice == param.slices_per_page -1 ) right_mutex.unlock();
            }
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    /******************************************************************************************************************/

    //------------------------------------------------------------------------------------------------------------------

    stamp :stage20_s x_array { ::bank_s => []; }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage20_s) bmedia_audio_codec_page.encode
    {
        ASSERT( param.is_setup );
        :stage10_s^ prev.encode( indexer, offset, param, context );
        sz_t banks = param.banks();
        sz_t bank_base_index = banks;
        sz_t bank_size_index = banks + 1;
        o.set_size( banks + 2 );

        foreach( m$.2 e in o ) e.1 = ::bank_s!;

        foreach( $* e in prev )
        {
            o.[ bank_base_index ].push_f3( e.base, 15 );
            o.[ bank_base_index ].push( f3_rs2( e.shadow * param.global_discretization ) );
        }

        for( sz_t bank_i = 0; bank_i < banks; bank_i++ )
        {
            foreach( $* e in prev )
            {
                o.[ bank_size_index ].push( e.[ bank_i ].size );
                o.[ bank_i ].push_bank( e.[ bank_i ] );
            }
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage20_s) bmedia_audio_codec_page.decode
    {
        ASSERT( param.is_setup );
        :stage10_s^ prev.set_size( param.channels * param.slices_per_page );

        if( o.size > 0 )
        {
            sz_t banks = param.banks();
            sz_t bank_base_index = banks;
            sz_t bank_size_index = banks + 1;

            sz_t size_index = 0;
            sz_t base_index = 0;

            f3_t inv_global_discretization = 1.0 / param.global_discretization;

            foreach( m$* e in prev )
            {
                e.base   = o.[ bank_base_index ].read_f3( 15, base_index );
                e.shadow = o.[ bank_base_index ].[ base_index++ ] * inv_global_discretization;
                e.set_size( banks );
            }

            for( sz_t bank_i = 0; bank_i < banks; bank_i++ )
            {
                sz_t section_index = 0;
                foreach( m$* e in prev )
                {
                    sz_t section_size = o.[ bank_size_index ].[ size_index++ ];
                    e.[ bank_i ].copy_bank_section( o.[ bank_i ], section_index, section_size );
                    section_index += section_size;
                }
            }
        }
        prev.decode( indexer, offset, param, context, left_mutex, right_mutex );
    }

    //------------------------------------------------------------------------------------------------------------------

    /******************************************************************************************************************/

    //------------------------------------------------------------------------------------------------------------------

    stamp :stage30_s { bcore_huffman_bit_buffer_s bit_buffer; }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage30_s) bmedia_audio_codec_page.encode
    {
        ASSERT( param.is_setup );
        :stage20_s^ prev.encode( indexer, offset, param, context );
        o.bit_buffer.push_packed_u3( prev.size );
        foreach( $* e in prev ) e.encode_to_bit_buffer( param, context, o.bit_buffer );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage30_s) bmedia_audio_codec_page.decode
    {
        ASSERT( param.is_setup );
        :stage20_s^ prev;
        if( o.bit_buffer.size > 0 )
        {
            bcore_huffman_bit_buffer_iterator_s^ iterator.setup( o.bit_buffer );
            sz_t size = iterator.read_packed_u3();
            for( sz_t i = 0; i < size; i++ ) { prev.push_d( ::bank_s!.decode_from_bit_buffer( param, context, iterator ) ); }
        }
        prev.decode( indexer, offset, param, context, left_mutex, right_mutex );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage30_s) x_bcml.bcml_body_to_sink o.bbml_body_to_sink( sink );
    func (:stage30_s) x_bbml.bbml_body_to_sink
    {
        o.bit_buffer.bcml_body_to_sink( sink );
        sink.push_tp( o.bit_buffer.get_hash() );
    }

    //------------------------------------------------------------------------------------------------------------------

    func (:stage30_s) x_bcml.bcml_body_from_source = o.bbml_body_from_source( source );
    func (:stage30_s) x_bbml.bbml_body_from_source
    {
        o.bit_buffer.bcml_body_from_source( source );
        if( o.bit_buffer.get_hash() != source.get_tp() ) o.bit_buffer.set_size( 0 );
        = 0;
    }

    //------------------------------------------------------------------------------------------------------------------

    /******************************************************************************************************************/

    //------------------------------------------------------------------------------------------------------------------

}

//----------------------------------------------------------------------------------------------------------------------

stamp :sequence_s x_array
{
    :param_s param;
    s3_t total_frames;
    aware :page => [];
    func o clear( m@* o ) { o.total_frames = 0; o.cast( m x_array* ).clear(); }
    func bmedia_audio_codec_sequence.total_frames = o.total_frames;
    func bmedia_audio_codec_sequence.param = o.param;

}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :encoder_thread_s
{
    :page -> page;
    bmedia_audio_sequence_s => sequence;
    s3_t frame_offset;
    :param_s -> param;

    x_thread_s thread;
    func bcore_inst_call.down_e o.thread.join();

    func o ( m@* o, d : *page, d bmedia_audio_sequence_s* sequence, s3_t frame_offset, d :param_s* param, d :context_s * shared_context )
    {
        o.thread.join();
        o.page =< page;
        o.param =< param;
        o.sequence =< sequence;
        o.frame_offset = frame_offset;
    }

    func o start( m@* o ) o.thread.call_m_thread_func( o );
    func o join ( m@* o ) o.thread.join();

    func x_thread.m_thread_func
    {
        m :context_s* context = :context_s!^;
        o.page.encode( o.sequence.create_indexer()^, o.frame_offset, o.param, context );
        = NULL;
    }
}

stamp :encoder_thread_queue_s x_deque trans(TE :encoder_thread_s) { x_deque_inst_s deque; };

//------------------------------------------------------------------------------------------------------------------

stamp :encoder_s bmedia_audio_codec_encoder
{
    :param_s => param;
    bl_t is_setup;

                :sequence_s => sequence;
    bmedia_audio_sequence_s => sequence_buf;

    :encoder_thread_queue_s thread_queue;
    func bcore_inst_call.down_e o.thread_queue.clear();

    s3_t pushed_frames;
    sz_t buffer_frames;
    sz_t buffer_digested_frames;

    hidden :context_s => context;

    func o add_frames( m@* o, sz_t frames ) { o.pushed_frames += frames; o.buffer_frames += frames; }

    func o push_buffer_d( m@* o, d bmedia_audio_buffer_s* buffer )
    {
        if( buffer.channels != o.param.channels )
        {
            ERR_fa( "buffer.channels '#<sz_t>' differs from param.channels '#<sz_t>'.", (sz_t)buffer.channels, (sz_t)o.param.channels );
        }
        o.add_frames( buffer.frames() ).sequence_buf.push_buffer_d( buffer );
    }

    func o finish( m@* o )
    {
        o.setup().encode( true );
        o.thread_queue.clear();
    }

    /// interface functions ...

    func bmedia_audio_codec_encoder.set_param o.reset().param =< param.clone();
    func bmedia_audio_codec_encoder.push_d = o.setup().push_buffer_d( buffer ).encode( false );
    func bmedia_audio_codec_encoder.push_c = o.push_d( buffer.clone() );
    func m :sequence_s* m_get_sequence( m@* o ) = o.finish().sequence;
    func c :sequence_s* c_get_sequence( m@* o ) = o.m_get_sequence();
    func bmedia_audio_codec_encoder.d_get_sequence
    {
        d :sequence_s* sequence = o.m_get_sequence().fork();
        o.sequence =< NULL;
        = sequence;
    }
    func bmedia_audio_codec_encoder.get_context = o.context;
};

//----------------------------------------------------------------------------------------------------------------------

func (:encoder_s) o setup( m@* o )
{
    if( !o.is_setup )
    {
        o.param!.setup();
        o.sequence!.clear();
        o.sequence.param.copy( o.param );
        o.sequence_buf!.clear().setup( o.param.channels, o.param.rate );
        o.pushed_frames = 0;
        o.buffer_frames = 0;
        o.buffer_digested_frames = 0;
        o.context =< o.param.collect_statistics ? :context_s! : NULL;
        o.is_setup = true;
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:encoder_s) o reset( m@* o )
{
    if( o.is_setup )
    {
        o.sequence_buf =< NULL;
        o.pushed_frames = 0;
        o.buffer_frames = 0;
        o.buffer_digested_frames = 0;
        o.is_setup = false;
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:encoder_s) sz_t frames_left( @* o ) = o.buffer_frames - o.buffer_digested_frames;
func (:encoder_s) bl_t pages_left ( @* o, bl_t finish ) = o.frames_left() >= ( finish ? 1 : o.param.frames_per_page() + o.param.frames_per_slice() );
func (:encoder_s) o encode( m@* o, bl_t finish )
{
    sz_t max_threads = sz_max( o.param.encoder_threads, 1 );
    if( o.pages_left( finish ) )
    {
        while( o.pages_left( finish ) )
        {
            d $* thread = :encoder_thread_s!.
            (
                o.sequence.push_d( o.param.create_page() ).fork(),
                bmedia_audio_sequence_s!.setup_fork_buffers( o.sequence_buf ),
                o.buffer_digested_frames,
                o.param.fork(),
                o.context.fork()
            );

            if( o.thread_queue.size() >= max_threads ) o.thread_queue.d_pop_first().join().discard();
            o.thread_queue.push_last_d( thread.start() );

            o.buffer_digested_frames += o.param.frames_per_page();
        }

        while( o.sequence_buf.size() > 0 && o.sequence_buf.c_first().frames() <= o.buffer_digested_frames - o.param.frames_per_slice() )
        {
            sz_t popped_frames = o.sequence_buf.pop_first_buffer()^.frames();
            o.buffer_digested_frames -= popped_frames;
            o.buffer_frames          -= popped_frames;
        }
    }

    if( finish ) o.sequence.total_frames = o.pushed_frames;

    = finish ? o.reset() : o;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :decoder_thread_s
{
    :page -> page;
    bmedia_audio_sequence_s => sequence;
    s3_t frame_offset;
    :param_s -> param;
    :context_s -> shared_context;

    x_mutex_s -> l_mutex; // mutex protecting the left end of a page
    x_mutex_s -> r_mutex; // mutex protecting the right end of a page

    x_thread_s thread;
    func bcore_inst_call.down_e o.thread.join();

    func o
    (
        m@* o,
        d : *page,
        d bmedia_audio_sequence_s* sequence,
        s3_t frame_offset,
        d :param_s* param,
        d :context_s* shared_context,
        d x_mutex_s* l_mutex,
        d x_mutex_s* r_mutex
    )
    {
        o.thread.join();
        o.page =< page;
        o.param =< param;
        o.shared_context =< shared_context;
        o.sequence =< sequence;
        o.l_mutex =< l_mutex;
        o.r_mutex =< r_mutex;
        o.frame_offset = frame_offset;
    }

    func o start( m@* o ) o.thread.call_m_thread_func( o );
    func o join ( m@* o ) o.thread.join();

    func x_thread.m_thread_func
    {
        m :context_s* context = :context_s!^;
        o.page.decode( o.sequence.create_indexer()^, o.frame_offset, o.param, context, o.l_mutex, o.r_mutex );
        = NULL;
    }
}

stamp :decoder_thread_queue_s x_deque trans(TE :decoder_thread_s) { x_deque_inst_s deque; };

//------------------------------------------------------------------------------------------------------------------

stamp :decoder_s bmedia_audio_codec_decoder
{
    :param_s => param;

                :sequence_s => sequence;
    bmedia_audio_sequence_s => sequence_buf;

    :decoder_thread_queue_s thread_queue;
    func bcore_inst_call.down_e o.thread_queue.clear();

    bl_t is_setup;

    sz_t sequence_index;
    sz_t buffer_decoding_index;
    sz_t buffer_decoded_index;
    s3_t buffered_frames;

    hidden :context_s => context;

    /// interface functions ...

    func bmedia_audio_codec_decoder.set_sequence_d { o.reset().sequence =< sequence; o.setup().decode(); }
    func bmedia_audio_codec_decoder.eos = ( !o.sequence_buf || o.sequence_buf.size() == 0 );
    func bmedia_audio_codec_decoder.d_get_buffer = o.setup().decode().pop_first_buffer();
    func bmedia_audio_codec_decoder.get_context = o.context;
}

//----------------------------------------------------------------------------------------------------------------------

func (:decoder_s) o reset( m@* o )
{
    if( o.is_setup )
    {
        o.sequence     =< NULL;
        o.sequence_buf =< NULL;
        o.sequence_index = 0;
        o.buffer_decoding_index = 0;
        o.buffer_decoded_index = 0;
        o.buffered_frames = 0;
        o.is_setup = false;
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:decoder_s) o setup( m@* o )
{
    if( !o.is_setup )
    {
        o.param =< o.sequence!.param.clone();
        if( o.param.slices_per_page < 2 ) ERR_fa( "At least 2 slices per page required." );
        o.param.setup();
        o.sequence_buf!.clear().setup( o.param.channels, o.param.rate );
        o.sequence_index = 0;
        o.buffer_decoding_index = 0;
        o.buffer_decoded_index = 0;
        o.buffered_frames = 0;
        o.context =< o.param.collect_statistics ? :context_s! : NULL;
        o.is_setup = true;
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:decoder_s) o decode( m@* o )
{
    sz_t max_threads = sz_max( o.param.decoder_threads, 1 );
    if( o.buffer_decoded_index > sz_max( 1, o.param.slices_per_page >> 1 ) ) = o;

    if( o.sequence_index < o.sequence.size && o.thread_queue.size() < max_threads )
    {
        sz_t slices_left = o.param.slices_per_page - o.sequence_buf.size() + o.buffer_decoding_index + 1;
        sz_t frames_left = sz_min( slices_left * o.param.frames_per_slice(), o.sequence.total_frames - o.buffered_frames );

        while( frames_left > 0 )
        {
            m$* buffer = o.sequence_buf.push_empty_buffer();
            sz_t frames = sz_min( frames_left, o.param.frames_per_slice() );
            buffer.set_frames( frames, o.param.channels );
            frames_left       -= frames;
            o.buffered_frames += frames;
        }

        d $* thread = :decoder_thread_s!.
        (
            o.sequence.[ o.sequence_index ].fork(),
            bmedia_audio_sequence_s!.setup_fork_buffers( o.sequence_buf ),
            o.buffer_decoding_index * o.param.frames_per_slice(),
            o.param.fork(),
            o.context.fork(),
            ( o.thread_queue.size() > 0 ) ? o.thread_queue.m_get_last().r_mutex.fork() : NULL,
            x_mutex_s!
        );

        o.thread_queue.push_last_d( thread.start() );

        o.sequence_index++;
        o.buffer_decoding_index += sz_min( o.sequence_buf.size() - o.buffer_decoding_index, o.param.slices_per_page );
    }

    if( o.buffer_decoded_index < 2 )
    {
        if( o.thread_queue.size() > 0 ) o.thread_queue.d_pop_first().join().discard();
        o.buffer_decoded_index += sz_min( o.sequence_buf.size() - o.buffer_decoded_index, o.param.slices_per_page );
    }

    = o;
}

//----------------------------------------------------------------------------------------------------------------------

func (:decoder_s) d bmedia_audio_buffer_s* pop_first_buffer( m@* o )
{
    if( o.eos() ) = NULL;
    o.buffer_decoding_index--;
    o.buffer_decoded_index--;
    = o.sequence_buf.pop_first_buffer();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
