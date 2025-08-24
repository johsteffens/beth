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
 *  choice: Optionally use function m_choice() to specify a variety of buttons to select
 *  e.g.
 *  o.m_choice().push( yes~,    "Yes",    "" );
 *  o.m_choice().push( no~,     "No",     "" );
 *  o.m_choice().push( cancel~, "Cancel", "" );
 *
 *  If choice was not preset, a single button "Ok" is offered to close the window.
 *
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// choice cases
name ok;
name cancel;
name yes;
name no;

stamp :s
{
    // parameters
    sz_t width  = 200;
    sz_t height = 30;
    st_s title;
    st_s text;

    hidden bgfe_choice_s => choice;
    hidden tp_t selection; // choice selection
    hidden bl_t exit;
    hidden bgfe_window_s => window;
    hidden bl_t clear_choice_on_closing;

    func bgfe_client.bgfe_edit_frame_type
    {
        if( content == ( vd_t )o.text.1 )
        {
            frame_type.0 = bgfe_frame_label_s~;
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
            action_type.0 = approve~;
        }
        else if( content == ( vd_t )o.text.1 )
        {
            frame.set_show_tooltip( false );
            action_type.0 = approve~;
        }
        = 0;
    }

    func bgfe_choice_client.choice_item_selection
    {
        o.selection = o.choice.selection;
        o.exit = true;
        = 0;
    }

    /// creates choice if not existing
    func m bgfe_choice_s* m_choice( m@* o )
    {
        if( o.window )
        {
            bcore_wrn_fa( "choice is not accessible for modification while window is open.\n" );
            = NULL;
        }
        = o.choice!;
    }

    func bgfe_choice_s* c_choice( @* o ) = o.choice;

    func tp_t selection( @* o ) = o.selection;

    func er_t open( m@* o, m bgfe_frame* parent )
    {
        if( o.choice == NULL )
        {
            o.clear_choice_on_closing = true;
            o.choice!.push( ok~, "Ok", "Acknowledge and close window" );
        }

        o.choice!.set_arrange( horizontal~ );
        o.choice.set_show_border( false );
        o.choice.set_spacing( 10 );
        o.choice.set_client( o );

        o.window!;
        o.window.set_client( o, 0 );
        o.window.set_title( o.title.sc );

        m$* f = o.window.add_sub_frame();
        f.add_content( o.text, 0 );
        f.set_show_border( true );
        f.set_spacing( 10 );
        f.set_arrange( vertical~ );
        f.set_stretch( false );
        m$* f1 = f.add_sub_frame();
        f1.add_content( o.choice, 0 );
        f1.set_show_border( true );
        f1.set_arrange( horizontal~ );
        f1.set_stretch( false );

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
        if( o.clear_choice_on_closing ) o.choice =< NULL;
        = 0;
    }

    func er_t cycle( m@* o, tp_t action_type )
    {
        if( !o.window ) = 0;
        if( o.window ) o.window.cycle( action_type );
        if( o.exit ) o.close();
        = 0;
    }

    /// runs the message in a modal-like fashion (choice can be NULL)
    func er_t run( m@* o, m bgfe_frame* parent, sc_t title, sc_t text )
    {
        o.title.copy_sc( title );
        o.text.copy_sc( text );
        o.open( parent );
        while( o.is_open() && !o.exit ) o.window.cycle_sleep_ms( approve~, 10 );
        o.close();
        = 0;
    }

    // In case an error is on stack, the error message is shown.
    func er_t run_error_if_any( m@* o, m bgfe_frame* parent )
    {
        if( bcore_error_stack_size() > 0 )
        {
            m$* st = bcore_error_pop_all_to_st( st_s!^ );
            o.run( parent, "Error", st.sc );
        }
        = 0;
    }

}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
