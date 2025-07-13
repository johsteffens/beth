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

/** Frame for displaying/editing arrays.
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) bl_t rebuild_is_necessary( @* o )
{
    m$* array = o.client_array();
    if( array.t_size( o.client_type ) != o.item_arr.size ) = true;
    if( array.t_get_data_m( o.client_type ) != o.array_base_address ) = true;
    = false;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bl_t any_selected( m@* o ) { foreach( m$* e in o.item_arr ) if( e.selected ) = true; = false; }
func (:s) bl_t any_copied  ( m@* o ) = ( o.copied_elements != NULL ) && ( o.copied_elements.size > 0 );

func (:s) er_t select_all ( m@* o ) { foreach( m$* e in o.item_arr ) e.set_selected( true  ); = 0; }
func (:s) er_t select_none( m@* o ) { foreach( m$* e in o.item_arr ) e.set_selected( false ); = 0; }

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t copy_selected( m@* o, tp_t action_type )
{
    if( action_type == reject~ ) = 0;
    m$* array = o.client_array();

    o.copied_elements!.clear();
    foreach( m$* e in o.item_arr )
    {
        if( e.selected )
        {
            o.copied_elements.push_sr( sr_clone( array.t_c_get_sr( o.client_type, e.index ) ) );
        }
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t cut_selected( m@* o, tp_t action_type )
{
    o.copy_selected( action_type );
    o.remove_selected( action_type );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t paste_to_end( m@* o, tp_t action_type )
{
    if( !o.copied_elements || o.copied_elements.size == 0 ) = 0;

    o.client_change_request( o, action_type.1 );
    if( action_type == reject~  ) = 0;

    m$* array = o.client_array();

    foreach( m$* e in o.copied_elements ) array.t_push_sr( o.client_type, sr_twc( e.type(), e.o ) );
    o.rebuild();

    sz_t size = o.copied_elements.size;
    for( sz_t i = 0; i < size; i++ ) o.item_arr.[ o.item_arr.size - size + i ].set_selected( true );

    o.rte.run( o.rtt_scroll_to_end.cast( bgfe_rte_fp_rtt ), o, NULL );

    tp_t confirm_action_type = TYPEOF_escalate;
    o.client_change_confirm( o, confirm_action_type.1 );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

// appends an element to the array
func (:s) er_t append_element( m@* o, tp_t action_type )
{
    o.client_change_request( o, action_type.1 );
    if( action_type == reject~  ) = 0;

    m$* array = o.client_array();
    array.t_push( o.client_type );
    o.rebuild();
    o.focus_to_end();
    o.rte.run( o.rtt_scroll_to_end.cast( bgfe_rte_fp_rtt ), o, NULL );

    tp_t confirm_action_type = TYPEOF_escalate;
    o.client_change_confirm( o, confirm_action_type.1 );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

// removes selected elements
func (:s) er_t remove_selected( m@* o, tp_t action_type )
{
    o.client_change_request( o, action_type.1 );
    if( action_type == reject~  ) = 0;

    m$* index_arr = bcore_arr_uz_s!^;
    bl_t any_selected = false;
    for( sz_t i = 0; i < o.item_arr.size; i++ )
    {
        m$* item = o.item_arr.[ i ];
        if( item.selected )
        {
            any_selected = true;
        }
        else
        {
            index_arr.push( i );
        }
    }

    if( !any_selected ) = 0;

    m$* array = o.client_array();
    array.t_reorder( o.client_type, index_arr );
    o.rebuild();

    tp_t confirm_action_type = TYPEOF_escalate;
    o.client_change_confirm( o, confirm_action_type.1 );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rebuild( m@* o )
{
    o.item_arr!.clear();

    m$* array = o.client_array();
    sz_t size = array.t_size( o.client_type );
    o.array_base_address = array.t_get_data_m( o.client_type );

    for( sz_t i = 0; i < size; i++ )
    {
        m$* item = :item_s!^;
        item.has_select_button = o.arr_editable;
        item.show_index = o.show_index;
        item.setup( o, i );

        o.item_arr.push_d( item.fork() );
    }

    o.rte.run( o.rtt_rebuild.cast( bgfe_rte_fp_rtt ), o, NULL );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_rebuild( m@* o, vd_t unused )
{
    o.rtt_remove_widget_from_container( o.rtt_gtk_list_box, o.rtt_gtk_scrolled_window );
    o.rtt_detach_widget( o.rtt_gtk_list_box );
    o.rtt_attach_widget( gtk_box_new( o.is_vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, o.spacing ), o.rtt_gtk_list_box );
    gtk_widget_show( o.rtt_gtk_list_box );

    for( sz_t i = 0; i < o.item_arr.size; i++ )
    {
        m$* frame = o.item_arr.[ i ];
        if( frame )
        {
            if( o.end_bound )
            {
                gtk_box_pack_end( GTK_BOX( o.rtt_gtk_list_box ), frame.rtt_widget(), o.center, o.stretch, 0 );
            }
            else
            {
                gtk_box_pack_start( GTK_BOX( o.rtt_gtk_list_box ), frame.rtt_widget(), o.center, o.stretch, 0 );
            }
        }
    }

    gtk_container_add( GTK_CONTAINER( o.rtt_gtk_scrolled_window ), o.rtt_gtk_list_box );

    // immediately updates the adjustment of scrolled window to the most recent size  (the automatic update seems to be delayed)
    gtk_scrolled_window_set_vadjustment( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), NULL /*NULL creates a new adjustment*/ );
    gtk_scrolled_window_set_hadjustment( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), NULL /*NULL creates a new adjustment*/ );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_scroll_to_end( m@* o, vd_t unused )
{
    if( o.is_vertical )
    {
        m GtkAdjustment* adjustment = gtk_scrolled_window_get_vadjustment( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ) );
        gtk_adjustment_set_value( adjustment, gtk_adjustment_get_upper( adjustment ) - gtk_adjustment_get_page_size( adjustment ) );
    }
    else
    {
        m GtkAdjustment* adjustment = gtk_scrolled_window_get_hadjustment( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ) );
        gtk_adjustment_set_value( adjustment, gtk_adjustment_get_upper( adjustment ) - gtk_adjustment_get_page_size( adjustment ) );
    }

    = 0;

}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t focus_to_end( m@* o )
{
    if( o.item_arr.size > 0 )
    {
        m bgfe_frame* frame  = o.item_arr.[ o.item_arr.size - 1 ].client_frame;
        if( frame ) frame.grab_focus();
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) open
{
    if( o.is_open ) = 0;
    if( !o.client ) = GERR_fa( "No client defined. Call 'set_client' first." );

    bgfe_rte_get( &o.rte );

    o.parent = parent;

    tp_t arrangement = o.arrangement();
    o.is_vertical = ( arrangement == TYPEOF_vertical );
    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );

    o.rebuild();
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_append_button_clicked( m GtkWidget* win, m@* o )
{
    o.mutex.lock();
    o.rts_append_button_clicked = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_remove_button_clicked( m GtkWidget* win, m@* o )
{
    o.mutex.lock();
    o.rts_remove_button_clicked = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_menu_button_clicked( m GtkWidget* win, m@* o )
{
    o.mutex.lock();
    o.rts_menu_button_clicked = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    o.rtt_attach_widget( gtk_box_new( o.is_vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 0 ), o.rtt_gtk_box );
    gtk_widget_show( o.rtt_gtk_box );

    tp_t label_name = ( o.show_client_name && o.client_name && bnameof( o.client_name ) ) ? o.client_name : 0;

    if( label_name )
    {
        m GtkWidget* label = gtk_label_new( bnameof( label_name ) );
        if( !label ) = GERR_fa( "'gtk_label_new' failed\n" );
        gtk_widget_set_name( label, "client_name" );
        gtk_label_set_angle( GTK_LABEL( label ), o.is_vertical ? 0 : 90 );
        gtk_box_pack_start( GTK_BOX( o.rtt_gtk_box ), label, false, false, 0 );
        gtk_widget_show( label );
    }

    o.rtt_attach_widget( gtk_scrolled_window_new( NULL, NULL ), o.rtt_gtk_scrolled_window );

    gtk_scrolled_window_set_propagate_natural_width( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), true );
    gtk_scrolled_window_set_propagate_natural_height( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), true );

    if( o.is_vertical )
    {
        gtk_scrolled_window_set_min_content_height( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), o.min_content_height );
        gtk_scrolled_window_set_max_content_height( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), o.max_content_height );
    }
    else
    {
        gtk_scrolled_window_set_min_content_width( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), o.min_content_width );
        gtk_scrolled_window_set_max_content_width( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), o.max_content_width );
    }

    // overlay scrolling does not seem to work reliably in this context
    gtk_scrolled_window_set_overlay_scrolling( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), false );
    gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( o.rtt_gtk_scrolled_window ), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );

    gtk_widget_show( o.rtt_gtk_scrolled_window );

    m$* box_name = st_s!^;
    box_name.push_sc( o.widget_name ? o.widget_name.sc : "bgfe_frame_array" );
    if( o.show_border ) box_name.push_sc( "_border" );

    gtk_widget_set_name( o.rtt_gtk_box, box_name.sc );
    gtk_widget_set_size_request( o.rtt_gtk_box, o.width, o.height );

    gtk_box_pack_start( GTK_BOX( o.rtt_gtk_box ), o.rtt_gtk_scrolled_window, true, true, 0 );

    if( o.arr_editable && !x_array_t_is_fixed( o.client_type ) )
    {
        o.rtt_attach_widget( gtk_box_new( o.is_vertical ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL, 0 ), o.rtt_gtk_edit_bar );
        gtk_widget_show( o.rtt_gtk_edit_bar );

        o.rtt_attach_widget( gtk_button_new_with_label( "+" ), o.rtt_gtk_append_button );
        o.rtt_attach_widget( gtk_button_new_with_label( "-" ), o.rtt_gtk_remove_button );
        o.rtt_attach_widget( gtk_button_new_with_label( "..." ), o.rtt_gtk_menu_button );
        g_signal_connect( o.rtt_gtk_append_button, "clicked", G_CALLBACK( o.rtt_signal_append_button_clicked ), o );
        g_signal_connect( o.rtt_gtk_remove_button, "clicked", G_CALLBACK( o.rtt_signal_remove_button_clicked ), o );
        g_signal_connect( o.rtt_gtk_menu_button,   "clicked", G_CALLBACK( o.rtt_signal_menu_button_clicked   ), o );
        gtk_widget_set_tooltip_text( o.rtt_gtk_append_button, "Append new element" );
        gtk_widget_set_tooltip_text( o.rtt_gtk_remove_button, "Remove all selected elements" );
        gtk_widget_set_tooltip_text( o.rtt_gtk_menu_button, "More edit options ..." );
        gtk_widget_show( o.rtt_gtk_append_button );
        gtk_widget_show( o.rtt_gtk_remove_button );
        gtk_widget_show( o.rtt_gtk_menu_button );
        gtk_box_pack_start( GTK_BOX( o.rtt_gtk_edit_bar ), o.rtt_gtk_remove_button, false, false, 0 );
        gtk_box_pack_start( GTK_BOX( o.rtt_gtk_edit_bar ), o.rtt_gtk_append_button, true, true, 0 );
        gtk_box_pack_start( GTK_BOX( o.rtt_gtk_edit_bar ), o.rtt_gtk_menu_button, false, false, 0 );
        gtk_box_pack_start( GTK_BOX( o.rtt_gtk_box ), o.rtt_gtk_edit_bar, false, false, 0 );
    }


    o.rtt_widget = o.rtt_gtk_box;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;

    foreach( m$* e in o.item_arr ) e.close();

    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );

    o.item_arr =< NULL;

    o.is_open = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_close( m@* o, vd_t arg )
{
    o.rtt_detach_widget( o.rtt_gtk_box );
    o.rtt_detach_widget( o.rtt_gtk_list_box );
    o.rtt_detach_widget( o.rtt_gtk_scrolled_window );
    o.rtt_detach_widget( o.rtt_gtk_append_button );
    o.rtt_detach_widget( o.rtt_gtk_remove_button );
    o.rtt_detach_widget( o.rtt_gtk_menu_button );
    o.rtt_detach_widget( o.rtt_gtk_edit_bar );
    o.rtt_widget = 0;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

name select_all, select_none, copy_selected, cut_selected, paste_to_end;
func (:s) er_t run_menu( m@* o, tp_t action_type  )
{
    m$* menu = bgfe_popup_choice_s!^;
    menu.push( select_all~ , "Select All" );
    menu.push( select_none~, "Select None" );
    if( o.any_selected() ) menu.push( copy_selected~, "Copy" );
    if( o.any_selected() ) menu.push( cut_selected~ , "Cut" );
    if( o.any_copied()   ) menu.push( paste_to_end~ , "Paste to End" );
    menu.run( o );
    switch( menu.selected() )
    {
        case select_all~:    o.select_all(); break;
        case select_none~:   o.select_none(); break;
        case copy_selected~: o.copy_selected( action_type ); break;
        case cut_selected~:  o.cut_selected( action_type ); break;
        case paste_to_end~:  o.paste_to_end( action_type ); break;
        default: break;
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    /** The array is automatically fully resynchronized when its size or memory location has changed
     *  between cycles. This is necessary to avoid temporary memory inconsistencies of client references
     *  in content frames
     */
    if( o.rebuild_is_necessary() ) o.rebuild();

    o.mutex.lock();
    bl_t append_button_clicked = o.rts_append_button_clicked;
    bl_t remove_button_clicked = o.rts_remove_button_clicked;
    bl_t menu_button_clicked   = o.rts_menu_button_clicked;
    o.rts_append_button_clicked = false;
    o.rts_remove_button_clicked = false;
    o.rts_menu_button_clicked   = false;
    o.mutex.unlock();

    if( append_button_clicked ) o.append_element( action_type ); // resyncs automatically
    if( remove_button_clicked ) o.remove_selected( action_type ); // resyncs automatically

    if( menu_button_clicked ) o.run_menu( action_type );

    foreach( m$* e in o.item_arr ) e.cycle( action_type );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) downsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    foreach( m$* e in o.item_arr ) e.downsync();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) upsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    if( o.rebuild_is_necessary() )
    {
        o.rebuild(); // this includes upsyncing the array
    }
    else
    {
        foreach( m$* e in o.item_arr ) e.upsync(); // upsync only elements
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) client_close_ok
{
    if( o.client )
    {
        if( !o.client.t_bgfe_close_ok( o.client_type ) ) = false;
    }

    foreach( m$* e in o.item_arr )
    {
        if( !e.client_close_ok() ) = false;
    }

    = true;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) client_close_confirm
{
    if( o.client ) o.client.t_bgfe_close_confirm( o.client_type );
    foreach( m$* e in o.item_arr ) e.client_close_confirm();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) arrangement
{
    if( o.arrange ) = o.arrange;

    /// >0 horizontal; < 0 vertical
    sz_t clients_tendency = 0;

    foreach( m$* e in o.item_arr )
    {
        switch( e.arrangement() )
        {
            case TYPEOF_horizontal: clients_tendency++; break;
            case TYPEOF_vertical  : clients_tendency--; break;
            default: break;
        }
    }

    // the chosen arrangement is the opposite of the clients tendency;
    = ( clients_tendency >= 0 ) ? TYPEOF_vertical : TYPEOF_horizontal;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_client_t
{
    if( o.is_open ) = GERR_fa( "Frame is open." );

    if( !client ) // reset
    {
        o.client = NULL;
        o.client_type = 0;
        o.client_name = 0;
        = 0;
    }

    o.client      = client;
    o.client_type = client_type;
    o.client_name = client_name;

    // the array content is added when the frame is opened

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
