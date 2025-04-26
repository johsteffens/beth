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

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func er_t error( sc_t context, s2_t errnum )
{
    sc_t snd_msg = snd_strerror( errnum );
    = bcore_error_push_fa( TYPEOF_general_error, "midi_in: #<sc_t>: #<sc_t>\n", context, snd_msg );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

type snd_ctl_t;

func (:s) open
{
    if( o.handle ) o.close();

    if( o.device_name.size == 0 ) = GERR_fa( "No device specified." );

    int err = 0;
    bl_t success = false;
    sz_t name_index = 0;
    for( sz_t i = 0; i <= o.device_name.size && !success; i++ )
    {
        if( i == o.device_name.size || o.device_name.[ i ] == ';' )
        {
            m$* device_name = st_s!^.copy_sc_n( o.device_name.sc + name_index, i - name_index );

            if( device_name.equal_sc( "detect" ) )
            {
                int card_nr = -1;
                while( !success )
                {
                    if( ( err = snd_card_next( card_nr.1 ) ) ) { = :error( "snd_card_next failed", err ); }
                    if( card_nr == -1 ) break;
                    int device_nr = -1;
                    m snd_ctl_t* ctl = NULL;
                    m$* card_name = st_s!^;
                    card_name.push_fa( "hw:#<s2_t>", ( s2_t )card_nr );
                    if( ( err = snd_ctl_open( ctl.2, card_name.sc, 0 ) ) ) { = :error( st_s!^.push_fa( "snd_ctl_open failed for '#<sc_t>'", card_name.sc ).sc, err ); }
                    while( !success )
                    {
                        if( ( err = snd_ctl_rawmidi_next_device( ctl, device_nr.1 ) ) ) { = :error( st_s!^.push_fa( "snd_ctl_rawmidi_next_device failed for '#<sc_t>'", card_name.sc ).sc, err ); }
                        if( device_nr == -1 ) break;
                        m$* test_name = st_s!^;
                        test_name.push_fa( "#<sc_t>,#<s2_t>", card_name.sc, device_nr );

                        if( snd_rawmidi_open( o.handle.2, NULL, test_name.sc, SND_RAWMIDI_NONBLOCK ) == 0 )
                        {
                            o.actual_device_name.copy( test_name );
                            success = true;
                        }
                    }
                }
            }
            else if( snd_rawmidi_open( o.handle.2, NULL, device_name.sc, SND_RAWMIDI_NONBLOCK ) == 0 )
            {
                o.actual_device_name.copy( device_name );
                success = true;
            }

            name_index = i + 1;
        }
    }


    if( !success ) = GERR_fa( "No available midi device found." );

    o.buffer.set_size( o.buffer_size );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.handle ) = 0;
    int err = 0;
    if( ( err = snd_rawmidi_drain( o.handle ) ) ) { = :error( "snd_rawmidi_drain failed", err ); }
    if( ( err = snd_rawmidi_close( o.handle ) ) ) { = :error( "snd_rawmidi_close failed", err ); }
    o.handle = NULL;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bl_t refill_buffer( m@* o, bl_t nonblock )
{
    int err = 0;
    if( !o.handle ) GERR_fa( "No handle. (Call 'open' first)" );
    if( ( err = snd_rawmidi_nonblock( o.handle, nonblock ) ) ) { :error( "snd_rawmidi_nonblock failed", err ); }
    sz_t read_size = snd_rawmidi_read( o.handle, o.buffer.data, o.buffer.size );

    if( read_size > 0 )
    {
        o.buffer_limit = read_size;
        o.buffer_index = 0;
        = true;
    }
    else
    {
        = false;
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) has_data
{
    if( o.buffer_index < o.buffer_limit ) = true;
    if( o.refill_buffer( true /*nonblock*/  ) ) = true;
    = false;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) get_u0
{
    if( o.buffer_index >= o.buffer_limit ) o.refill_buffer( false /*nonblock*/ );
    = o.buffer.[ o.buffer_index++ ];
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
