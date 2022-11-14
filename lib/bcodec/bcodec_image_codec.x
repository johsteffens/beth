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

/** Image codec interface.
 *  --------------------------------------------------------------------
 *
 *  Encoder Usage:
 *
 *  Setup parameters from file (or instantiate class directly form target codec):
 *  m$* param = x_btml_create_from_file( "mycodec.param.cfg" )^;
 *
 *  Create encoder and assign parameters:
 *  m$* encoder = param.create_encoder()^;
 *
 *  Encode image into frame (image is of bcodec_image; e.g. bcodec_image_bgra_s)
 *  m$* frame = encoder.encode( image, param.create_frame()^ );
 *
 *  Save/Stream frame as compatible x_bbml or ultra compact x_bcml.
 *
 *  --------------------------------------------------------------------
 *
 *  Decoder Usage:
 *
 *  Obtain encoded frame:
 *  m bcodec_image_codec_frame* frame = ... ;
 *
 *  Obtain parameters used for encoding:
 *  c bcodec_image_codec_param* param = frame.param();
 *
 *  Create decoder and assign sequence:
 *  m$* decoder = param.create_decoder()^;
 *
 *  Decode to image:
 *  m$* image = decoder.decode( frame, bcore_image_bgra_s!^ );
 *
 *  --------------------------------------------------------------------
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// General codec parameters
group :param
{
    feature d (Tencoder) ::encoder* create_encoder( @* o ); // creates encoder and sets parameters
    feature d (Tdecoder) ::decoder* create_decoder( @* o ); // creates decoder
    feature d (Tframe)   ::frame*   create_frame  ( @* o ); // creates frame (container for encoded image)
}

//----------------------------------------------------------------------------------------------------------------------

/// frame: container for encoded image
group :frame
{
    feature c (Tparam) ::param* param( @* o );
}

//----------------------------------------------------------------------------------------------------------------------

group :encoder
{
    feature o set_param( m@* o, (Tparam) ::param* param );
    feature frame encode( m@* o, bcodec_image* image, m (Tframe) ::frame* frame );
}

//----------------------------------------------------------------------------------------------------------------------

group :decoder
{
    feature image decode( m@* o, (Tframe) ::frame* frame, m bcodec_image* image );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
