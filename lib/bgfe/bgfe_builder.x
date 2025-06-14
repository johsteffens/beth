/** Author and Copyright 2025 Johannes Bernhard Steffens
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

/** The builder constructs a frame for a client based on client specification and internal parameters.
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Build frame for client:
 *  This function constructs a frame for the specified client based on client properties and stamp's parameters.
 *  Output: frame.
 *  If the type is not supported, frame.1 is set to NULL.
 */
feature er_t build_frame_t( @* o, m obliv bgfe_client* client, tp_t client_type, tp_t client_name, d bgfe_frame** frame ) export = bgfe_builder_build_frame_t_std( client, client_type, client_name, frame );
feature er_t build_frame  ( @* o, m aware bgfe_client* client,                   tp_t client_name, d bgfe_frame** frame ) = o.build_frame_t( client, client ? client._ : 0, client_name, frame );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func er_t build_frame_t_std( m obliv bgfe_client* client, tp_t client_type, tp_t client_name, d bgfe_frame** arg_frame )
{
    arg_frame.1 = NULL;
    m bgfe_frame* frame = NULL;

    switch( client_type )
    {
        case f3_t~:
        case f2_t~:
        {
            frame = bgfe_frame_scale_s!^^;
        }
        break;

        case s3_t~:
        case s2_t~:
        case s1_t~:
        case s0_t~:
        case u3_t~:
        case u2_t~:
        case u1_t~:
        case u0_t~:
        case sz_t~:
        case uz_t~:
        {
            frame = bgfe_frame_entry_s!^^;
        }
        break;

        case tp_t~:
        case er_t~:
        case aware_t~:
        {
            frame = bgfe_frame_entry_s!^^;
        }
        break;

        case st_s~:
        case sd_t~:
        {
            frame = bgfe_frame_entry_s!^^;
        }
        break;

        case sc_t~:
        {
            frame = bgfe_frame_label_s!^^;
        }
        break;

        case bl_t~:
        {
            frame = bgfe_frame_check_button_s!^^;
        }
        break;

        default: break;
    }

    if( frame )
    {
        frame.set_client_t( client, client_type, client_name );
    }
    else
    {
        m$* frame_s = bgfe_frame_s!^;
        frame_s.set_client_with_content_t( client, client_type, client_name );
        if( !frame_s.is_empty() ) frame = frame_s.fork()^^;
    }

    arg_frame.1 = frame.fork();

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    func :.build_frame_t;
    func :.build_frame = o.build_frame_t( client, client ? client._ : 0, client_name, frame );
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) build_frame_t
{
    = :build_frame_t_std( client, client_type, client_name, frame );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

