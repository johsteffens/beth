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

/** Choice as a list of buttons.
 *
 *  Usage
 *    - push choice options  (type, display-text)
 *       * the option can be a type and/or a string;
 *       * if either is missing (value 0), it is generated on the fly from the other value via global name manager
 *       * argument active: (normally true) false: displays button but renders it insensitive
 *    - modal usage: call 'popup_run' (this opens a popup window and runs it in a model-mode)
 *    - non-model usage: call 'popup_open' and 'popup_cycle' as needed; check via 'popup_is_open'; close via popup_close
 *    - submenu: use push_choice
 *    - After closing:
 *    - use function selected() to determine which case was selected
 *       * returns '0' if choice was cancelled
 *
 *
 *  Usage example (popup):
 *
 *  m$* choice = bgfe_choice_s!^;
 *  choice.push( select_all~, "Select All" )
 *        .push( remove_all~, "Remove All" )
 *        .push( reset~, "Reset" );
 *
 *  choice.popup_run( NULL ); // instead of NULL: pass parent frame if available
 *
 *  switch( choice.selected() )
 *  {
 *      case select_all~: ...; break;
 *      case remove_all~: ...; break;
 *      case reset~     : ...; break;
 *      default         : break;
 *  }
 *
 *  // choice.selected() returns 0 in case the choice was canceled
 *
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

feature er_t set_parent_choice( m@* o, m :s* parent_choice ) = 0;

stamp :item_s
{
    hidden tp_t    case_tp;
    hidden st_s => case_st; // Optional string (for display); If NULL, bnameof( case_tp ) is displayed
    hidden st_s => tooltip; // Optional tooltip
    hidden :s * parent_choice; // set by parent when the array frame is initialized

    func :.set_parent_choice
    {
        o.parent_choice = parent_choice;
        = 0;
    }

    // returns nearest client or NULL
    func m :client* nearest_client( m@* o )
    {
        if( o.parent_choice ) = o.parent_choice.nearest_client();
        = NULL;
    }

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
        if( content == (vd_t)o )
        {
            frame_type.0 = bgfe_frame_button_s~;
        }
        action_type.0 = approve~;
        = 0;
    }

    func bgfe_client.bgfe_edit_frame
    {
        if( ( content == (vd_t)o ) && ( frame._ == bgfe_frame_button_s~ ) )
        {
            frame.set_widget_name( "bgfe_choice_item" );
            if( o.tooltip )
            {
                frame.set_tooltip( o.tooltip.sc );
            }
            else
            {
                frame.set_show_tooltip( false );
            }

            if( o.parent_choice )
            {
                if( o.parent_choice.arrange == vertical~ ) frame.set_text_xalign( 0 );
            }

            m :client* cl = o.nearest_client();
            if( cl ) frame.set_insensitive( !cl.choice_item_is_active( o ) );
            action_type.0 = approve~;
        }
        action_type.0 = approve~;
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

stamp :item_arr_s x_array { : => []; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s
{
    hidden $ st_s => name; // Optional name in case this choice is a sub-choice

    /// parameters
    hidden sz_t max_content_width = 300; // if popup is larger, a scrollbar appears
    hidden sz_t max_content_height = 300; // if popup is larger, a scrollbar appears
    hidden tp_t arrange = vertical;
    hidden bl_t scrollable = false;

    func bgfe_frame.set_arrange            { o.arrange = name; = 0; }
    func bgfe_frame.set_scrollable         { o.scrollable = flag; = 0; }
    func bgfe_frame.set_max_content_width  { o.max_content_width = value; = 0; }
    func bgfe_frame.set_max_content_height { o.max_content_height = value; = 0; }

    /// internals
    :item_arr_s item_arr;
    hidden tp_t selected;  // selected case
    hidden bl_t in_window; // choice is open in a dedicated (popup) window

    hidden bgfe_window_s -> window;
    hidden bgfe_frame_array_s* frame_array;

    hidden :s * parent_choice; // !=NULL: this choice is a sub-choice of parent choice

    func :.set_parent_choice
    {
        o.parent_choice = parent_choice;
        = 0;
    }

    /// optional client implementing :client features below
    hidden :client* client;

    /// returns nearest client or NULL
    func m :client* nearest_client( m@* o )
    {
        if( o.client ) = o.client;
        if( o.parent_choice ) = o.parent_choice.nearest_client();
        = NULL;
    }

    func bgfe_client.bgfe_get_glimpse
    {
        if( o.name ) text.copy( o.name );
        = 0;
    }

    func m :item_s* push_item( m@* o ) = o.item_arr.push_d( :item_s! ).cast( m :item_s* );

    func o push( m@* o, tp_t case_tp, sc_t text, sc_t tooltip )
    {
        if( ( case_tp == 0 ) && ( text != NULL ) ) case_tp = bentypeof( text );
        m$* item = o.push_item();
        item.case_tp = case_tp;
        if( text ) item.case_st!.copy_sc( text );
        if( tooltip && tooltip[ 0 ] != 0 ) item.tooltip!.copy_sc( tooltip );
    }

    func m :s* push_choice( m@* o, sc_t text )
    {
        m$* choice = :s!^;
        choice.name!.copy_fa( "#<sc_t>⋮", text );
        choice.parent_choice = o;
        o.item_arr.push_d( choice.fork() );
        = choice;
    }

    func tp_t selected( @* o ) = o.selected;

    func bgfe_client.bgfe_edit_frame
    {
        if( content == (vd_t)o )
        {
            if( frame._ == bgfe_window_s~ )
            {
                m$* window = frame.cast( m bgfe_window_s* );
                window.set_width( 0 );
                window.set_height( 0 );
                window.set_keep_above( true );
                window.set_decorated( false );
                window.set_fleeting( true );
                o.in_window = true;
                if( o.window != window ) o.window =< frame.cast( m bgfe_window_s* ).fork();
                if( o.parent_choice && o.parent_choice.window ) o.parent_choice.window.set_close_on_lost_focus( false );
                frame.set_widget_name( o.in_window ? "bgfe_choice_in_window" : "bgfe_choice" );
            }
            else if( frame._ == bgfe_frame_link_s~ )
            {
                if( o.parent_choice && ( o.parent_choice.arrange == vertical~ ) ) frame.set_text_xalign( 0 );
            }
        }
        else if( content == (vd_t)o.item_arr.1 )
        {
            if( frame._ == bgfe_frame_array_s~ )
            {
                frame.set_arrange( o.arrange );
                frame.set_scrollable( o.scrollable );
                frame.set_arr_editable( false );
                frame.set_show_client_name( false );
                frame.set_min_content_width( 0 );
                frame.set_min_content_height( 0 );
                frame.set_max_content_width( o.max_content_width );
                frame.set_max_content_height( o.max_content_height );
                frame.set_show_border( false );
                frame.set_show_index( false );
                frame.set_center( false );
                frame.set_stretch( false );
                o.frame_array = frame.cast( m bgfe_frame_array_s* );
                foreach( m$* e in o.item_arr ) e.set_parent_choice( o );
            }
        }

        action_type.0 = approve~;
        = 0;
    }

    func bgfe_client.bgfe_button_clicked
    {
        if( initiator.client_type() == :item_s~ )
        {
            o.apply_item( initiator.client().cast( :item_s* ) );
            if( o.frame_array ) o.frame_array.close_all_item_windows();
        }
        action_type.0 = approve~;
        = 0;
    }

    func er_t apply_item( m@* o, :item_s* item )
    {
        o.selected = item.case_tp;
        if( o.window ) o.window.requesting_close();
        if( o.client ) o.client.choice_item_selection( item );
        if( o.parent_choice ) o.parent_choice.apply_item( item );
        = 0;
    }

    func er_t popup_open( m@* o, m bgfe_frame* parent )
    {
        if( o.item_arr.size == 0 ) = 0;
        o.window =< bgfe_window_s!;
        parent.place_at_upper_right( o.window );
        o.window.set_client_with_content( o, 0 );
        o.window.set_close_on_lost_focus( true );

        tp_t action_type = escapprove~;
        o.window.client_edit_frame( o, o._, 0, action_type.1, o.window );

        o.window.open( parent );
        = 0;
    }

    func bl_t popup_is_open( @* o ) = ( o.window && o.window.is_open() );

    func er_t popup_close( m@* o )
    {
        o.in_window = false;
        if( o.window )
        {
            o.window.close();
            o.window =< NULL;
        }
        = 0;
    }

    func er_t popup_cycle( m@* o, tp_t action_type )
    {
        if( !o.window ) = 0;
        if( o.window ) o.window.cycle( action_type );
        = 0;
    }

    /// runs choice in a modal fashion
    func er_t popup_run( m@* o, m bgfe_frame* parent )
    {
        o.popup_open( parent );
        while( o.popup_is_open() ) o.window.cycle_sleep_ms( approve~, 10 );
        o.popup_close();
        = 0;
    }

    /// optional client implementing :client features below
    func er_t set_client( m@* o, m :client* client )
    {
        o.client = client;
        = 0;
    }

}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :client
{
    feature er_t choice_item_selection( m@* o, ::item_s* item ) { = 0; }
    feature bl_t choice_item_is_active( m@* o, ::item_s* item ) { = true; }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

