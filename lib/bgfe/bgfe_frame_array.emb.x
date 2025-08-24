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

func (:s) er_t copy_selected( m@* o )
{
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

func (:s) er_t cut_selected( m@* o )
{
    o.copy_selected();
    o.remove_selected();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t paste_to_end( m@* o )
{
    tp_t action_type = escapprove~;
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
func (:s) er_t append_element( m@* o )
{
    tp_t action_type = escapprove~;
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

// removes last element (if eny)
func (:s) er_t remove_last( m@* o )
{
    tp_t action_type = escapprove~;
    o.client_change_request( o, action_type.1 );
    if( action_type == reject~  ) = 0;

    m$* array = o.client_array();
    array.t_pop( o.client_type );
    o.rebuild();

    tp_t confirm_action_type = TYPEOF_escalate;
    o.client_change_confirm( o, confirm_action_type.1 );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

// removes selected elements
func (:s) er_t remove_selected( m@* o )
{
    tp_t action_type = escapprove~;
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

func (:s) er_t indexed_remove( m@* o, sz_t index )
{
    m$* array = o.client_array();
    if( index < 0 || index >= array.t_size( o.client_type ) ) = 0;

    tp_t action_type = escapprove~;
    o.client_change_request( o, action_type.1 );
    if( action_type == reject~  ) = 0;

    array.t_remove( o.client_type, index );
    o.rebuild();

    tp_t confirm_action_type = TYPEOF_escalate;
    o.client_change_confirm( o, confirm_action_type.1 );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t indexed_cut( m@* o, sz_t index )
{
    m$* array = o.client_array();
    if( index < 0 || index >= array.t_size( o.client_type ) ) = 0;

    tp_t action_type = escapprove~;
    o.client_change_request( o, action_type.1 );
    if( action_type == reject~  ) = 0;

    o.copied_elements!.clear();
    o.copied_elements.push_sr( sr_clone( array.t_c_get_sr( o.client_type, index ) ) );
    array.t_remove( o.client_type, index );
    o.rebuild();

    tp_t confirm_action_type = TYPEOF_escalate;
    o.client_change_confirm( o, confirm_action_type.1 );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t indexed_move_to_prev( m@* o, sz_t index )
{
    m$* array = o.client_array();
    if( index <= 0 || index >= array.t_size( o.client_type ) ) = 0;

    tp_t action_type = escapprove~;
    o.client_change_request( o, action_type.1 );
    if( action_type == reject~  ) = 0;

    array.t_swap( o.client_type, index, index - 1 );
    o.rebuild();

    tp_t confirm_action_type = TYPEOF_escalate;
    o.client_change_confirm( o, confirm_action_type.1 );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t indexed_move_to_next( m@* o, sz_t index )
{
    m$* array = o.client_array();
    if( index < 0 || index >= array.t_size( o.client_type ) - 1 ) = 0;

    tp_t action_type = escapprove~;
    o.client_change_request( o, action_type.1 );
    if( action_type == reject~  ) = 0;

    array.t_swap( o.client_type, index, index + 1 );
    o.rebuild();

    tp_t confirm_action_type = TYPEOF_escalate;
    o.client_change_confirm( o, confirm_action_type.1 );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t indexed_move_to_first( m@* o, sz_t index )
{
    m$* array = o.client_array();
    if( index <= 0 || index >= array.t_size( o.client_type ) ) = 0;

    tp_t action_type = escapprove~;
    o.client_change_request( o, action_type.1 );
    if( action_type == reject~  ) = 0;

    array.t_swap( o.client_type, index, 0 );
    o.rebuild();

    tp_t confirm_action_type = TYPEOF_escalate;
    o.client_change_confirm( o, confirm_action_type.1 );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t indexed_move_to_last( m@* o, sz_t index )
{
    m$* array = o.client_array();
    if( index < 0 || index >= array.t_size( o.client_type ) - 1 ) = 0;

    tp_t action_type = escapprove~;
    o.client_change_request( o, action_type.1 );
    if( action_type == reject~  ) = 0;

    array.t_swap( o.client_type, index, array.t_size( o.client_type ) - 1 );
    o.rebuild();

    tp_t confirm_action_type = TYPEOF_escalate;
    o.client_change_confirm( o, confirm_action_type.1 );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t indexed_copy( m@* o, sz_t index )
{
    m$* array = o.client_array();
    if( index < 0 || index >= array.t_size( o.client_type ) ) = 0;

    o.copied_elements!.clear();
    o.copied_elements.push_sr( sr_clone( array.t_c_get_sr( o.client_type, index ) ) );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t indexed_duplicate( m@* o, sz_t index )
{
    m$* array = o.client_array();
    if( index < 0 || index >= array.t_size( o.client_type ) ) = 0;

    tp_t action_type = escapprove~;
    o.client_change_request( o, action_type.1 );
    if( action_type == reject~ ) = 0;


    array.t_insert_sr( o.client_type, index, sr_clone( array.t_c_get_sr( o.client_type, index ) ) );
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
    if( o.rtt_scrolled_container )
    {
        o.rtt_remove_widget_from_container( o.rtt_list_box, o.rtt_scrolled_container );
    }
    else if( o.rtt_fixed_container )
    {
        o.rtt_remove_widget_from_container( o.rtt_list_box, o.rtt_fixed_container );
    }

    o.rtt_detach_widget( o.rtt_list_box );
    o.rtt_attach_widget( gtk_box_new( o.is_vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, o.spacing ), o.rtt_list_box );
    gtk_widget_show( o.rtt_list_box );

    for( sz_t i = 0; i < o.item_arr.size; i++ )
    {
        m$* frame = o.item_arr.[ i ];
        if( frame )
        {
            if( o.end_bound )
            {
                gtk_box_pack_end( GTK_BOX( o.rtt_list_box ), frame.rtt_widget(), o.center, o.stretch, 0 );
            }
            else
            {
                gtk_box_pack_start( GTK_BOX( o.rtt_list_box ), frame.rtt_widget(), o.center, o.stretch, 0 );
            }
        }
    }

    if( o.rtt_scrolled_container )
    {
        gtk_container_add( GTK_CONTAINER( o.rtt_scrolled_container ), o.rtt_list_box );

        // immediately updates the adjustment of scrolled window to the most recent size  (the automatic update seems to be delayed)
        gtk_scrolled_window_set_vadjustment( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ), NULL /*NULL creates a new adjustment*/ );
        gtk_scrolled_window_set_hadjustment( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ), NULL /*NULL creates a new adjustment*/ );
    }
    else if( o.rtt_fixed_container )
    {
        gtk_container_add( GTK_CONTAINER( o.rtt_fixed_container ), o.rtt_list_box );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_scroll_to_end( m@* o, vd_t unused )
{
    if( o.rtt_scrolled_container )
    {
        if( o.is_vertical )
        {
            m GtkAdjustment* adjustment = gtk_scrolled_window_get_vadjustment( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ) );
            gtk_adjustment_set_value( adjustment, gtk_adjustment_get_upper( adjustment ) - gtk_adjustment_get_page_size( adjustment ) );
        }
        else
        {
            m GtkAdjustment* adjustment = gtk_scrolled_window_get_hadjustment( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ) );
            gtk_adjustment_set_value( adjustment, gtk_adjustment_get_upper( adjustment ) - gtk_adjustment_get_page_size( adjustment ) );
        }
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


    if( o.arr_editable && !x_array_t_is_fixed( o.client_type ) )
    {
        o.menu!;
        o.menu.set_client( o );
        o.menu.set_arrange( horizontal~ );
        o.menu.push( append~      , "+", "Append new element" );
        o.menu.push( remove_last~ , "⌫", "Remove last element" );
        o.menu.push( cut_selected~, "✂", "Cut selected elements to clipboard" );
        m$* choice = o.menu.push_choice( "" );

        choice.push( select_all~   , "All ☑", NULL );
        choice.push( select_none~  , "All ☐", NULL );
        choice.push( copy_selected~, "Copy 📋" , "Copy selected elements to clipboard" );
        choice.push( paste_to_end~ , "Paste"   , "Append copied elements" );

        o.menu_frame!;
        o.menu_frame.set_show_border( false );
        o.menu_frame.set_client_with_content( o.menu, 0 );
        o.menu_frame.open( o );
    }


    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );

    o.rebuild();
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    o.rtt_attach_widget( gtk_box_new( GTK_ORIENTATION_VERTICAL, 0 ), o.rtt_main_box );
    gtk_widget_show( o.rtt_main_box );

    o.rtt_attach_widget( gtk_box_new( GTK_ORIENTATION_HORIZONTAL, 0 ), o.rtt_top_bar );
    gtk_box_pack_start( GTK_BOX( o.rtt_main_box ), o.rtt_top_bar, true, true, 5 );

    m$* box_name = st_s!^;
    box_name.push_sc( o.widget_name ? o.widget_name.sc : "bgfe_frame" );
    if( o.show_border ) box_name.push_fa( "_border_style#<sz_t>", ( sz_t )( o.nesting_level % 5 ) );
    gtk_widget_set_name( o.rtt_main_box, box_name.sc );
    gtk_widget_set_size_request( o.rtt_main_box, o.width, o.height );

    sc_t label_name = NULL;

    if( o.title )
    {
        label_name = o.title.sc;
    }
    else if( o.show_client_name && o.client_name )
    {
        label_name = bnameof( o.client_name );
    }
    else if( o.menu_frame )
    {
        label_name = "[...]";
    }

    if( label_name )
    {
        m GtkWidget* label = gtk_label_new( label_name );
        if( !label ) = GERR_fa( "'gtk_label_new' failed\n" );
        gtk_widget_set_name( label, "client_name" );
        gtk_box_pack_start( GTK_BOX( o.rtt_top_bar ), label, true, false, 0 );
        gtk_widget_show( label );
        gtk_widget_show( o.rtt_top_bar );
    }

    if( o.menu_frame )
    {
        gtk_box_pack_start( GTK_BOX( o.rtt_top_bar ), o.menu_frame.rtt_widget(), false, false, 5 );
        gtk_widget_show( o.rtt_top_bar );
    }

    o.rtt_attach_widget( gtk_box_new( o.is_vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 0 ), o.rtt_content_box );
    if( o.insensitive ) gtk_widget_set_state_flags( o.rtt_content_box, GTK_STATE_FLAG_INSENSITIVE, false );
    gtk_widget_show( o.rtt_content_box );
    gtk_box_pack_start( GTK_BOX( o.rtt_main_box ), o.rtt_content_box, true, true, 0 );

    if( o.scrollable )
    {
        o.rtt_attach_widget( gtk_scrolled_window_new( NULL, NULL ), o.rtt_scrolled_container );

        gtk_scrolled_window_set_propagate_natural_width( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ), true );
        gtk_scrolled_window_set_propagate_natural_height( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ), true );

        gtk_scrolled_window_set_min_content_height( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ), o.min_content_height );
        gtk_scrolled_window_set_max_content_height( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ), o.max_content_height );
        gtk_scrolled_window_set_min_content_width( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ), o.min_content_width );
        gtk_scrolled_window_set_max_content_width( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ), o.max_content_width );

        // overlay scrolling does not seem to work reliably in this context
        gtk_scrolled_window_set_overlay_scrolling( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ), false );
        gtk_scrolled_window_set_policy( GTK_SCROLLED_WINDOW( o.rtt_scrolled_container ), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );

        gtk_widget_show( o.rtt_scrolled_container );

        gtk_box_pack_start( GTK_BOX( o.rtt_content_box ), o.rtt_scrolled_container, true, true, 0 );
    }
    else // not scrollable
    {
        o.rtt_attach_widget( gtk_box_new( GTK_ORIENTATION_VERTICAL, 0 ), o.rtt_fixed_container );
        gtk_widget_set_size_request( o.rtt_fixed_container, o.min_content_width, o.min_content_height );
        gtk_widget_show( o.rtt_fixed_container );
        gtk_box_pack_start( GTK_BOX( o.rtt_content_box ), o.rtt_fixed_container, true, true, 0 );
    }

    o.rtt_widget = o.rtt_main_box;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;
    if( o.menu_frame ) o.menu_frame.close();

    foreach( m$* e in o.item_arr ) e.close();

    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );

    o.item_arr =< NULL;

    o.menu_frame =< NULL;
    o.menu =< NULL;

    o.is_open = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_close( m@* o, vd_t arg )
{
    o.rtt_detach_widget( o.rtt_main_box );
    o.rtt_detach_widget( o.rtt_top_bar );
    o.rtt_detach_widget( o.rtt_content_box );
    o.rtt_detach_widget( o.rtt_list_box );
    o.rtt_detach_widget( o.rtt_scrolled_container );
    o.rtt_detach_widget( o.rtt_fixed_container );
    o.rtt_widget = 0;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_choice_client.choice_item_is_active
{
    switch( item.case_tp  )
    {
        case select_all~: =o.item_arr.size > 0;
        case select_none~: =o.item_arr.size > 0;
        case copy_selected~: = o.any_selected();
        case cut_selected~: = true;
        case paste_to_end~: = o.any_copied();
        default: break;
    }
    = true;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_choice_client.choice_item_selection
{
    switch( item.case_tp )
    {
        case select_all~:      o.select_all(); break;
        case select_none~:     o.select_none(); break;
        case copy_selected~:   o.copy_selected(); break;
        case cut_selected~:    o.cut_selected(); break;
        case paste_to_end~:    o.paste_to_end(); break;
        case append~:          o.append_element(); break;
        case remove_selected~: o.remove_selected(); break;
        case remove_last~:     o.remove_last(); break;
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

    if( o.menu_frame ) o.menu_frame.cycle( approve~ );

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

func (:s) client_close_request
{
    if( action_type.0 == approve~ || action_type.0 == reject~ ) = 0;
    if( o.client ) o.client.t_bgfe_close_request( o.client_type, initiator, action_type );

    foreach( m$* e in o.item_arr )
    {
        if( action_type.0 == approve~ || action_type.0 == reject~ ) = 0;
        e.client_close_request( initiator, action_type );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) client_close_confirm
{
    if( action_type.0 == approve~ ) = 0;
    if( o.client ) o.client.t_bgfe_close_confirm( o.client_type, initiator, action_type );
    if( action_type.0 == approve~ ) = 0;
    foreach( m$* e in o.item_arr ) e.client_close_confirm( initiator, action_type );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) client_distraction
{
    if( action_type.0 == approve~ ) = 0;
    if( o.menu_frame ) o.menu_frame.client_distraction( initiator, action_type );
    if( o.client ) o.client.t_bgfe_distraction( o.client_type, initiator, action_type );
    foreach( m$* e in o.item_arr ) e.client_distraction( initiator, action_type );
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

func (:s) bgfe_frame.open_window_request
{
    switch( o.window_policy )
    {
        case any~ : action_type.0 = approve~; break;
        case zero~: action_type.0 = reject~;  break;
        case one~: o.close_all_item_windows(); action_type.0 = approve~; break;
        default: break;
    }
    = 0;
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

func (:s) close_all_item_windows
{
    foreach( m$* e in o.item_arr ) e.close_window_request();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
