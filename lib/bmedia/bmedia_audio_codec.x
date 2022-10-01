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

/** Audio codec interface.
 *
 *  --------------------------------------------------------------------
 *
 *  Encoder Usage:
 *
 *  Setup parameters from file (or instantiate class directly form target codec):
 *  m$* param = x_btml_create_from_file( "audio_codec_waw_120kbps.param.cfg" )^;
 *
 *  Set rate and channels:
 *  param.set_rate( 44100 );
 *  param.set_channels( 2 );
 *
 *  Create encoder and assign parameters:
 *  m$* encoder = param.create_encoder()^;
 *
 *  Push audio buffers:
 *  while( ... ) encoder.push_d( buffer.fork() );
 *
 *  Get encoded sequence (this finalizes encoding and resets encoder)
 *  m bmedia_audio_codec_sequence* sequence = encoder.d_get_sequence()^;
 *
 *  Save/Stream as compatible x_bbml or ultra compact x_bcml.
 *
 *  --------------------------------------------------------------------
 *
 *  Decoder Usage:
 *
 *  Obtain encoded sequence:
 *  m bmedia_audio_codec_sequence* sequence = ... ;
 *
 *  Obtain parameters used for encoding:
 *  c bmedia_audio_codec_param* param = sequence.param();
 *
 *  Create decoder and assign sequence:
 *  m$* decoder = param.create_decoder()^.set_sequence_d( sequence.fork() );
 *
 *  Pull buffers until function returns NULL or decoder.eos():
 *  while( !decoder.eos() ) { d$* buffer = decoder.d_get_buffer(); ... }
 *
 *  --------------------------------------------------------------------
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :context
{
    feature o update      ( m @* o, c @* src );
    feature o stat_to_sink( c @* o, m x_sink* sink );
}

//----------------------------------------------------------------------------------------------------------------------

/// General codec parameters
group :param
{
    feature d (Tencoder) ::encoder* create_encoder( @* o ); // creates encoder and sets parameters
    feature d (Tdecoder) ::decoder* create_decoder( @* o ); // creates decoder
    feature d (Tcontext) ::context* create_context( @* o );
    feature d (Tpage   ) ::page*    create_page   ( @* o );

    feature sz_t frames_per_page ( @* o );
    feature sz_t frames_per_slice( @* o );

    /// retrieves / overwrites rate and channels settings
    feature o set_rate    ( m@* o, s2_t rate     );
    feature o set_channels( m@* o, s2_t channels );
    feature s2_t get_rate    ( @* o     );
    feature s2_t get_channels( @* o );

    feature o setup( m@* o ); // setup for general processing
}

//----------------------------------------------------------------------------------------------------------------------

/// Generic map applied on a vector inside a codec
group :map
{
    // map function v_in and v_out can be identical
    feature v_out _( @* o, ::param* param, c bmath_vf2_s* v_in, m bmath_vf2_s* v_out );
}

//----------------------------------------------------------------------------------------------------------------------

group :page
{
    feature o encode
    (
        m@* o, c bcodec_audio_sequence_indexer_s* indexer,
        s3_t offset,
        c (Tparam) ::param* param,
        m (Tcontext) ::context* context
    );

    feature o decode
    (
        c@* o,
        m bcodec_audio_sequence_indexer_s* indexer,
        s3_t offset,
        c (Tparam  ) ::param* param,
        m (Tcontext) ::context* context,
        m x_mutex_s* left_mutex,
        m x_mutex_s* right_mutex
    );
}

//----------------------------------------------------------------------------------------------------------------------

group :sequence
{
    feature s3_t total_frames( @* o );
    feature c (Tparam) ::param* param( @* o );
}

//----------------------------------------------------------------------------------------------------------------------

group :encoder
{
    feature o set_param( m@* o, (Tparam) ::param* param );
    feature o push_d   ( m@* o, d bcodec_audio_buffer_s* buffer );
    feature o push_c   ( m@* o, c bcodec_audio_buffer_s* buffer ) = o.push_d( buffer.clone() );
    feature d (Tsequence) ::sequence* d_get_sequence( m@* o );
    feature c (Tcontext ) ::context* get_context( c@* o ) = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

group :decoder
{
    feature o set_sequence_d( m@* o, d (Tsequence) ::sequence* sequence );
    feature bl_t eos( @* o );
    feature d bcodec_audio_buffer_s* d_get_buffer( m@* o );
    feature c (Tcontext) ::context* get_context( c@* o ) = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
