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

/** Popup window offering a choice as a list of buttons.
 *
 *  Usage
 *    - push choice options  (type, display-text)
 *       * the option can be a type and/or a string;
 *       * if either is missing (value 0), it is generated on the fly from the other value via global name manager
 *    - modal usage: call 'run' (this opens a popup window and runs it in a model-mode)
 *    - non-model usage: call 'open' and 'cycle' as needed; check via 'is_open'
 *    - After closing:
 *    - use function selected() to determine which case was selected
 *       * returns '0' if choice was cancelled
 *
 *
 *  Usage example:
 *
 *  m$* choice = bgfe_popup_choice_s!^;
 *  choice.push( select_all~, "Select All" );
 *  choice.push( remove_all~, "Remove All" );
 *  choice.push( reset~,      "Reset" );
 *
 *  choice.run( NULL ); // instead of NULL: pass parent frame if available
 *
 *       if( choice.selected() == select_all~ ) ... ;
 *  else if( choice.selected() == remove_all~ ) ... ;
 *  else if( choice.selected() == reset~      ) ... ;
 *  // choice.selected() returns 0 in case the choice was cancelled
 *
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :item_s
{
    tp_t    case_tp;
    st_s => case_st; // Optional string (for display); If NULL, bnameof( case_tp ) is displayed

    func bgfe_client.bgfe_get_glimpse
    {
        if( o.case_st )
        {
            text.copy( o.case_st );
        }
        else
        {
            text.copy_sc( ifnameof( o.case_tp ) );
        }
        = 0;
    }

    func bgfe_client.bgfe_edit_frame_type
    {
        if( content == (vd_t)o ) frame_type.0 = bgfe_frame_button_s~;
        action_type.0 = approve~;
        = 0;
    }

    func bgfe_client.bgfe_edit_frame
    {
        if( content == (vd_t)o ) frame.set_show_tooltip( false );
        action_type.0 = approve~;
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

stamp :item_arr_s x_array { :item_s => []; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    // parameters
    sz_t max_content_height = 300; // if popup is larger; a scrollbar appears

    // internals
    hidden :item_arr_s item_arr;
    hidden bl_t exit;
    hidden tp_t selected; // selected case

    hidden bgfe_window_s => window;

    func o push( m@* o, tp_t case_tp, sc_t text )
    {
        if( ( case_tp == 0 ) && ( text != NULL ) ) case_tp = bentypeof( text );
        m$* item = :item_s!^;
        item.case_tp = case_tp;
        if( text ) item.case_st!.copy_sc( text );
        o.item_arr.push_d( item.fork() );
    }

    func tp_t selected( @* o ) = o.selected;

    func bgfe_client.bgfe_edit_frame
    {
        if( frame._ == bgfe_frame_array_s~ )
        {
            frame.set_arr_editable( false );
            frame.set_show_client_name( false );
            frame.set_min_content_height( 0 );
            frame.set_max_content_height( o.max_content_height );
            frame.set_show_border( false );
            frame.set_show_index( false );
            frame.set_center( true );
            frame.set_stretch( true );
        }
        action_type.0 = approve~;
        = 0;
    }

    func bgfe_client.bgfe_button_clicked
    {
        if( initiator.client_type == :item_s~ )
        {
            o.selected = initiator.client.cast( :item_s* ).case_tp;
            o.exit = true;
        }
        action_type.0 = approve~;
        = 0;
    }

    func er_t open( m@* o, m bgfe_frame* parent )
    {
        if( o.item_arr.size == 0 ) = 0;
        o.window =< bgfe_window_s!;
        o.window.set_client( o, 0 );
        o.window.add_content( o.item_arr, 0 );
        o.window.set_keep_above( true );
        o.window.set_decorated( false );
        o.window.set_close_on_lost_focus( true );
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

    /// runs choice in a modal fashion
    func er_t run( m@* o, m bgfe_frame* parent )
    {
        o.open( parent );
        while( o.is_open() && !o.exit ) o.window.cycle_sleep_ms( approve~, 10 );
        o.close();
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
