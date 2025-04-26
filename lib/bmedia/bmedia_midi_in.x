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

/** Raw Midi input interface (uses the ALSA RAWMIDI API)
 *
 *  Usage (bmedia_midi_in_s):
 *
 *    - Set parameter 'device_name'
 *      S. 'device_name' options below for details.
 *
 *    - Call 'open'
 *      After opening 'actual_device_name' holds the device name that was actually opened
 *
 *    - Use functions 'has_data', 'get_u0' as needed.
 *
 *  device_name:
 *
 *  - ALSA-specific notation: 'hw:<card-nr>[.<device-nr>]' or 'default';
 *      Example: device_name = "hw:0,0";
 *
 *  - "detect": Scans all devices until an available device is found.
 *    Devices currently in use elsewhere are skipped.
 *      Example: device_name = "detect";
 *
 *  - List of above notations, separated by ';': Scans entries in order until an available device is found.
 *    Devices currently in use elsewhere are skipped.
 *      Example: device_name = "default;hw:1.0;detect";
 *
 *  Note: Using a scanning option may produce intermittend ALSA-warnings to stderr.
 *
 *  ---------------------------------
 *
 *  ALSA RAWMIDI API Reference: https://www.alsa-project.org/alsa-doc/alsa-lib/rawmidi.html
 */

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

type snd_rawmidi_t;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    /// =========== parameters ===========

    st_s device_name = "detect";

    sz_t buffer_size = 1024;

    /// ==================================

    /// device that was actually opened (after function 'open')
    st_s actual_device_name;

    private snd_rawmidi_t* handle;

    hidden bcore_arr_u0_s buffer;
    hidden sz_t buffer_limit; // current size of buffer
    hidden sz_t buffer_index;

    func bcore_inst_call.down_e { o.close(); }

    func bl_t is_open( @* o ) = ( o.handle != NULL );

    /// setup; needs explicit calling; error if device could not be found.
    func er_t open( m@* o );

    /// auto-closed when stamp shuts down.
    func er_t close( m@* o );

    /// checks for available data
    func bl_t has_data( m@* o );

    /// retrieves next byte; waits (blocks) while no data is available (check 'has_data' to avoid a potentially blocking call)
    func u0_t get_u0( m@* o );
};

//----------------------------------------------------------------------------------------------------------------------

embed "bmedia_midi_in.emb.x";

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
