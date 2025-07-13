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

func (:item_s) void rtt_select_button_toggled( m GtkWidget* win, m@* o )
{
    if( o.rtt_ignore_select_button_toggled ) return;
    o.mutex.lock();
    o.rts_selected = gtk_toggle_button_get_active( GTK_TOGGLE_BUTTON( o.rtt_select_button ) );
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:item_s) er_t rtt_set_select_button( m@* o, bl_t* flag )
{
    o.rtt_ignore_select_button_toggled = true;
    o.mutex.lock();
    o.rts_selected = flag.0;
    gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( o.rtt_select_button ), flag.0 );
    o.mutex.unlock();
    o.rtt_ignore_select_button_toggled = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:item_s) set_selected
{
    o.rte.run( o.rtt_set_select_button.cast( bgfe_rte_fp_rtt ), o, flag.1 );
    o.selected = flag;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:item_s) open
{
    if( o.is_open ) = 0;
    bgfe_rte_get( &o.rte );
    if( parent != o.parent.cast( m bgfe_frame* ) ) = GERR_fa( "Parent mismatch. Call setup first.\n" );
    o.parent = parent.cast( m :s* );

    m$* array = o.parent.client_array();
    tp_t array_type = o.parent.client_type;
    sz_t size = array.t_size( array_type );
    if( o.index < 0 || o.index >= size ) = GERR_fa( "Index #<sz_t> out of range [0,#<sz_t>]. Call setup first.\n", o.index, size - 1 );

    if( array.t_is_of_links( array_type ) )
    {
        m$* sr_content = sr_s!^;
        sr_content.0 = array.t_m_get_sr( array_type, o.index );

        m$* frame_link = bgfe_frame_link_s!^;
        frame_link.clearable = !array.t_is_static( array_type );

        if( sr_content.o )
        {
            frame_link.set_client_t( sr_content.o, sr_content.type(), 0 );
        }
        else
        {
            tp_t content_type = x_array_t_get_type( array, array_type, o.index );
            frame_link.set_client_t( NULL, content_type, 0 );
        }

        frame_link.set_holder_t( o.parent.client, o.parent.client_type, o.parent.client_name, true, o.index );
        o.client_frame =< frame_link.fork();
        frame_link.open( o );
    }
    else
    {
        m$* sr_content = sr_s!^;
        sr_content.0 = array.t_m_get_sr( array_type, o.index );
        if( sr_content.o && sr_content.is_weak() )
        {
            tp_t frame_type = bgfe_frame_default_frame_type( sr_content.type() );
            tp_t action_type = escapprove~;

            o.client_edit_frame_type( sr_content.o, sr_content.type(), 0, action_type, frame_type );
            m bgfe_frame* frame = x_inst_create( frame_type ).cast( d bgfe_frame* )^;

            action_type = escapprove~;
            o.client_edit_frame( sr_content.o, sr_content.type(), 0, action_type, frame );
            if( frame )
            {
                frame.set_client_with_content_t( sr_content.o, sr_content.type(), 0 );
                frame.open( o );
            }
            o.client_frame =< frame.fork();
        }
    }

    o.is_vertical = o.client_frame ? ( o.client_frame.arrangement() == vertical~ ) : false;

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );

    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier GTK_JUSTIFY_RIGHT;
identifier gtk_label_set_justify, gtk_label_set_xalign;

func (:item_s) er_t rtt_open( m@* o, vd_t unused )
{
    o.rtt_attach_widget( gtk_box_new( o.is_vertical ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL, 0 ), o.rtt_widget );
    gtk_widget_show( o.rtt_widget );
    gtk_widget_set_name( o.rtt_widget, ifnameof( o._ ) );
    gtk_widget_set_size_request( o.rtt_widget, o.width, o.height );

    if( o.show_index )
    {
        o.rtt_attach_widget( gtk_label_new( st_s!^.copy_fa( "#<sz_t>", o.index ).sc ), o.rtt_index_label );
        gtk_widget_set_name( o.rtt_index_label, "index" );
        gtk_widget_set_size_request( o.rtt_index_label, 25, 0 );
        gtk_label_set_xalign( GTK_LABEL( o.rtt_index_label ), 1 );
        gtk_widget_show( o.rtt_index_label );
        gtk_box_pack_start( GTK_BOX( o.rtt_widget ), o.rtt_index_label, false, true, 0 );
    }

    if( o.has_select_button )
    {
        o.rtt_attach_widget( gtk_check_button_new(), o.rtt_select_button );
        gtk_widget_set_name( o.rtt_select_button, ifnameof( o._ ) );
        gtk_toggle_button_set_active( GTK_TOGGLE_BUTTON( o.rtt_select_button ), o.selected );
        gtk_widget_show( o.rtt_select_button );
        g_signal_connect( o.rtt_select_button, "toggled", G_CALLBACK( o.rtt_select_button_toggled ), o );
        gtk_box_pack_start( GTK_BOX( o.rtt_widget ), o.rtt_select_button, false, false, 0 );
    }

    if( o.client_frame )
    {
        gtk_box_pack_start( GTK_BOX( o.rtt_widget ), o.client_frame.rtt_widget(), true, true, 0 );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:item_s) close
{
    if( !o.is_open ) = 0;
    if( o.client_frame ) o.client_frame.close();
    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:item_s) er_t rtt_close( m@* o, vd_t arg )
{
    o.rtt_detach_widget( o.rtt_index_label );
    o.rtt_detach_widget( o.rtt_select_button );
    o.rtt_detach_widget( o.rtt_widget );
    o.rtt_select_button = NULL;
    o.rtt_widget = NULL;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:item_s) setup
{
    if( o.is_open ) o.close();
    o.parent = parent;
    o.index = index;
    o.open( parent );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:item_s) cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    o.mutex.lock();
    o.selected = o.rts_selected;
    o.mutex.unlock();
    if( o.client_frame ) o.client_frame.cycle( action_type );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:item_s) downsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    if( o.client_frame ) o.client_frame.downsync();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:item_s) upsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed
    if( o.client_frame ) o.client_frame.upsync();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:item_s) client_close_ok
{
    bl_t ok = true;
    if( o.client_frame ) ok = o.client_frame.client_close_ok();
    = ok;
}

//----------------------------------------------------------------------------------------------------------------------

func (:item_s) client_close_confirm
{
    if( o.client_frame ) o.client_frame.client_close_confirm();
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

