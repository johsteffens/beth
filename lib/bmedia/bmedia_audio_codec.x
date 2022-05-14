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
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :context
{
    feature o update      ( m @* o, c @* src );
    feature o stat_to_sink( c @* o, m x_sink* sink );
}

//----------------------------------------------------------------------------------------------------------------------

group :param
{
    feature d (Tencoder) ::encoder* create_encoder( @* o );
    feature d (Tdecoder) ::decoder* create_decoder( @* o );
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

group :page
{
    feature o encode
    (
        m@* o, c bmedia_audio_sequence_indexer_s* indexer,
        s3_t offset,
        c (Tparam) ::param* param,
        m (Tcontext) ::context* context
    );

    feature o decode
    (
        c@* o,
        m bmedia_audio_sequence_indexer_s* indexer,
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
    feature o push_d   ( m@* o, d bmedia_audio_buffer_s* buffer );
    feature o push_c   ( m@* o, c bmedia_audio_buffer_s* buffer ) = o.push_d( buffer.clone() );
    feature d (Tsequence) ::sequence* d_get_sequence( m@* o );
    feature c (Tcontext ) ::context* get_context( c@* o ) = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

group :decoder
{
    feature o set_sequence_d( m@* o, d (Tsequence) ::sequence* sequence );
    feature bl_t eos( @* o );
    feature d bmedia_audio_buffer_s* d_get_buffer( m@* o );
    feature c (Tcontext) ::context* get_context( c@* o ) = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
