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

/** Popup window offering a simple text message.
 *
 *  bgfe_popup_message_s!^.run( parent, "Error", "Incorrect text was entered." ); // parent can be NULL
 *
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    // parameters
    sz_t width  = 300;
    sz_t height = 100;
    st_s title;
    st_s text;
    sc_t ok_button = "OK";

    hidden bl_t exit;
    hidden bgfe_window_s => window;

    func bgfe_client.bgfe_edit_frame_type
    {
        if( content == ( vd_t )o.text.1 )
        {
            frame_type.0 = bgfe_frame_label_s~;
            action_type.0 = approve~;
        }
        else if( content == ( vd_t )o.ok_button.1 )
        {
            frame_type.0 = bgfe_frame_button_s~;
            action_type.0 = approve~;
        }
        = 0;
    }

    func bgfe_client.bgfe_edit_frame
    {
        if( content == (vd_t)o )
        {
            frame.set_width( o.width );
            frame.set_height( o.height );
            frame.set_widget_name( "bgfe_popup_message" );
            action_type.0 = approve~;
        }
        else if( content == ( vd_t )o.text.1 )
        {
            frame.set_show_tooltip( false );
            action_type.0 = approve~;
        }
        else if( content == ( vd_t )o.ok_button.1 )
        {
            frame.set_tooltip( "Closes this message" );
            action_type.0 = approve~;
        }
        = 0;
    }

    func bgfe_client.bgfe_button_clicked
    {
        if( initiator.client() == (vd_t)o.ok_button.1 ) o.exit = true;
        action_type.0 = approve~;
        = 0;
    }

    func er_t open( m@* o, m bgfe_frame* parent )
    {
        o.window =< bgfe_window_s!;
        o.window.set_client( o, 0 );
        o.window.set_title( o.title.sc );
        o.window.add_content( o.text, 0 );
        o.window.add_content_t( o.ok_button.1, sc_t~, 0 );
        o.window.set_keep_above( true );
        //o.window.set_decorated( false );
        //o.window.set_close_on_lost_focus( true );
        o.window.open( parent );
        = 0;
    }

    func bl_t is_open( @* o ) = ( o.window && o.window.is_open() );

    func er_t close( m@* o )
    {
        o.window =< NULL;
        = 0;
    }

    func er_t cycle( m@* o, tp_t action_type )
    {
        if( !o.window ) = 0;
        if( o.window ) o.window.cycle( action_type );
        if( o.exit ) o.close();
        = 0;
    }

    func er_t run( m@* o, m bgfe_frame* parent, sc_t title, sc_t text )
    {
        o.title.copy_sc( title );
        o.text.copy_sc( text );
        o.open( parent );
        while( o.is_open() && !o.exit ) o.window.cycle_sleep_ms( approve~, 10 );
        o.close();
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
