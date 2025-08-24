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

/**********************************************************************************************************************/

/** File chooser interactive frame editor based on GTK 'FileChooserWidget'
 *  Associated types: st_s and convertible types
 */

//----------------------------------------------------------------------------------------------------------------------

stamp :s bgfe_frame
{
    sz_t width = 640;   // optional preset width
    sz_t height = 400;  // optional preset height
    tp_t chooser_action = save;
    st_s => path;     // editable path
    st_s => widget_name; // optional gtk widget name overrides default widget name
    tp_t take_action_on = activate; // condition under which frame takes action on client: modified : activate
    bl_t close_window = true; // closes nearest ancestor window when a final decision has been reached

    func bgfe_frame.set_width       { o.width = value; = 0; }
    func bgfe_frame.set_height      { o.height = value; = 0; }
    func bgfe_frame.set_text        { o.path!.copy_sc( text ); = 0; }
    func bgfe_frame.set_widget_name { o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_chooser_action{ o.chooser_action = name; = 0; } // open, save, select_folder, create_folder;
    func bgfe_frame.set_take_action_on{ o.take_action_on = name; = 0; }
    func bgfe_frame.set_close_window  { o.close_window = flag; = 0; }

    /// internals
    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name of the client (under which the client's owner holds the client)
    hidden bgfe_frame* parent;
    func bgfe_frame.client = o.client;
    func bgfe_frame.client_type = o.client_type;
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.set_parent o.parent = parent;
    func bgfe_frame.is_open = o.is_open;
    func bgfe_frame.h_complexity = 20;
    func bgfe_frame.v_complexity = 20;
    func bgfe_frame.is_compact = false;

    hidden st_s => rts_path;
    hidden bl_t rts_path_modified; // path was modified by the front end
    hidden bl_t rts_activate;      // activation signal
    hidden bl_t rts_cancel_clicked;
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget* rtt_widget;
    hidden GtkWidget* rtt_bottom_bar;
    hidden GtkWidget* rtt_button_cancel;
    hidden GtkWidget* rtt_button_apply;

    hidden bl_t client_is_numeric;
    hidden bl_t is_open;
    hidden bl_t rtt_ignore_signal_value_changed;
    hidden bl_t path_modified; // text was modified by the front-end but changes were not yet downsynched
    hidden bl_t activate;      // front end has signaled activation but changes were not yet downsynched
    hidden bl_t lost_focus;    // activated when widget lost focus

    func bcore_inst_call.down_e o.close();

    func bgfe_frame.rtt_widget = o.rtt_widget;
    func bgfe_frame.open;
    func bgfe_frame.close;

    /// interface functions ...
    func bgfe_frame.set_client_t;
    func bgfe_frame.cycle;
    func bgfe_frame.upsync;
    func bgfe_frame.downsync;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t client_to_st( @* o, m st_s* st )
{
    bgfe_client_t_bgfe_copy_to_typed( o.client, o.client_type, o.client_type, TYPEOF_st_s, st.cast( m x_inst* ) );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t st_to_client( @* o, st_s* st )
{
    bgfe_client_t_bgfe_copy_from_typed( o.client, o.client_type, o.client_type, TYPEOF_st_s, st );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

type GtkFileChooser, GtkFileChooserConfirmation;

// possible return values
type GTK_FILE_CHOOSER_CONFIRMATION_ACCEPT_FILENAME, GTK_FILE_CHOOSER_CONFIRMATION_CONFIRM, GTK_FILE_CHOOSER_CONFIRMATION_SELECT_AGAIN;

identifier gtk_file_chooser_get_filename;

func (:s) GtkFileChooserConfirmation rtt_signal_confirm_overwrite( m GtkFileChooser* chooser, m@* o )
{
    = GTK_FILE_CHOOSER_CONFIRMATION_ACCEPT_FILENAME;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_current_folder_changed( m GtkFileChooser* chooser, m@* o )
{
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_file_activated( m GtkFileChooser* chooser, m@* o )
{
    sc_t path = gtk_file_chooser_get_filename( chooser );

    bcore_msg_fa( "#name: rtt_signal_file_activated; path: #<sc_t>\n", o._, path );

    if( path != NULL && path[ 0 ] != 0 )
    {
        o.mutex.lock();
        o.rts_path!.copy_sc( path );
        o.rts_path_modified = true;
        o.rts_activate = true;
        o.mutex.unlock();
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_selection_changed( m GtkFileChooser* chooser, m@* o )
{
    sc_t path = gtk_file_chooser_get_filename( chooser );
    bcore_msg_fa( "#name: rtt_signal_selection_changed; path: #<sc_t>\n", o._, path );

    if( path != NULL && path[ 0 ] != 0 )
    {
        o.mutex.lock();
        o.rts_path!.copy_sc( path );
        o.rts_path_modified = true;
        o.mutex.unlock();
    }
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_signal_update_preview( m GtkFileChooser* chooser, m@* o )
{
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) open
{
    if( o.is_open ) = 0;
    if( !o.client ) = GERR_fa( "No client defined. Call set_client or set_client_t first." );
    bgfe_rte_get( &o.rte );

    ASSERT( parent );
    o.parent = parent;

    o.client_to_st( o.path! );

    o.mutex.lock();
    o.rts_path =< o.path.clone();
    o.mutex.unlock();

    o.rte.run( o.rtt_open.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = true;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_button_cancel_signal_clicked( m GtkWidget* win, m@* o )
{
    o.mutex.lock();
    o.rts_cancel_clicked = true;
    o.mutex.unlock();
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) void rtt_button_apply_signal_clicked( m GtkWidget* win, m@* o )
{
    sc_t path = gtk_file_chooser_get_filename( GTK_FILE_CHOOSER( o.rtt_widget ) );

    bcore_msg_fa( "#name: rtt_button_apply_signal_clicked; path: #<sc_t>\n", o._, path );

    if( path != NULL && path[ 0 ] != 0 )
    {
        o.mutex.lock();
        o.rts_path!.copy_sc( path );
        o.rts_path_modified = true;
        o.rts_activate = true;
        o.mutex.unlock();
    }
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_file_chooser_widget_new, gtk_file_chooser_set_filename, gtk_file_chooser_set_file, GTK_FILE_CHOOSER, g_file_new_for_path;
identifier GTK_FILE_CHOOSER_ACTION_OPEN, GTK_FILE_CHOOSER_ACTION_SAVE, GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER;
identifier gtk_file_chooser_set_extra_widget;
type GtkFileChooserAction, GFile;

func (:s) er_t rtt_set_path( m@* o, sc_t path )
{
    gtk_file_chooser_set_filename( GTK_FILE_CHOOSER( o.rtt_widget ), path );

    // seems to be the same behavior as `set_filename`
//    m GFile* g_file = g_file_new_for_path( path );
//    gtk_file_chooser_set_file( GTK_FILE_CHOOSER( o.rtt_widget ), g_file, NULL );
//    g_object_unref( g_file );

    = 0;
}

func (:s) er_t rtt_open( m@* o, vd_t unused )
{
    GtkFileChooserAction gtk_chooser_action = GTK_FILE_CHOOSER_ACTION_OPEN;
    switch( o.chooser_action )
    {
        case open~:          gtk_chooser_action = GTK_FILE_CHOOSER_ACTION_OPEN; break;
        case save~:          gtk_chooser_action = GTK_FILE_CHOOSER_ACTION_SAVE; break;
        case select_folder~: gtk_chooser_action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER; break;
        case create_folder~: gtk_chooser_action = GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER; break;
        default: break;
    }

    o.rtt_attach_widget( gtk_file_chooser_widget_new( gtk_chooser_action ), o.rtt_widget );

    if( o.rts_path ) o.rtt_set_path( o.rts_path.sc );

    gtk_widget_set_name( o.rtt_widget, o.widget_name ? o.widget_name.sc : ifnameof( o._ ) );
    gtk_widget_set_size_request( o.rtt_widget, o.width, o.height );

    sz_t bottom_spacing = 10;
    o.rtt_attach_widget( gtk_box_new( GTK_ORIENTATION_HORIZONTAL, bottom_spacing ), o.rtt_bottom_bar );
    gtk_widget_set_size_request( o.rtt_bottom_bar, sz_max( 0, o.width - 20 ), 0 );

    o.rtt_attach_widget( gtk_button_new_with_label( "Apply"  ), o.rtt_button_apply );
    g_signal_connect( o.rtt_button_apply, "clicked", G_CALLBACK( :s_rtt_button_apply_signal_clicked ), o );
    gtk_box_pack_end( GTK_BOX( o.rtt_bottom_bar ), o.rtt_button_apply,  false, false, 0 );

    if( o.close_window )
    {
        o.rtt_attach_widget( gtk_button_new_with_label( "Cancel" ), o.rtt_button_cancel );
        g_signal_connect( o.rtt_button_cancel, "clicked", G_CALLBACK( :s_rtt_button_cancel_signal_clicked ), o );
        gtk_box_pack_end( GTK_BOX( o.rtt_bottom_bar ), o.rtt_button_cancel, false, false, 0 );
    }

    gtk_file_chooser_set_extra_widget( GTK_FILE_CHOOSER( o.rtt_widget ), o.rtt_bottom_bar );

    gtk_widget_show( o.rtt_widget        );
    gtk_widget_show( o.rtt_bottom_bar    );
    gtk_widget_show( o.rtt_button_apply  );
    gtk_widget_show( o.rtt_button_cancel );

    g_signal_connect( o.rtt_widget, "confirm-overwrite",      G_CALLBACK( :s_rtt_signal_confirm_overwrite      ), o );
    g_signal_connect( o.rtt_widget, "current-folder-changed", G_CALLBACK( :s_rtt_signal_current_folder_changed ), o );
    g_signal_connect( o.rtt_widget, "file-activated",         G_CALLBACK( :s_rtt_signal_file_activated         ), o );
    g_signal_connect( o.rtt_widget, "selection-changed",      G_CALLBACK( :s_rtt_signal_selection_changed      ), o );
    g_signal_connect( o.rtt_widget, "update-preview",         G_CALLBACK( :s_rtt_signal_update_preview         ), o );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) close
{
    if( !o.is_open ) = 0;
    o.rte.run( o.rtt_close.cast( bgfe_rte_fp_rtt ), o, NULL );
    o.is_open = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t rtt_close( m@* o, vd_t arg )
{
    o.rtt_detach_widget( o.rtt_widget );
    o.rtt_detach_widget( o.rtt_bottom_bar );
    o.rtt_detach_widget( o.rtt_button_cancel );
    o.rtt_detach_widget( o.rtt_button_apply );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) set_client_t
{
    if( o.is_open ) = GERR_fa( "Frame is open." );

    o.client      = client;
    o.client_type = client_type;
    o.client_name = client_name;

    if( bcore_tp_is_numeric( client_type ) )
    {
        o.client_is_numeric = true;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t take_action( m@* o, tp_t action_type )
{
    if( o.path_modified )
    {
        if( action_type == approve~ ) = o.downsync();
        if( action_type == reject~  ) = o.upsync();
        o.client_change_request( o, action_type.1 );
        if( action_type == approve~ ) = o.downsync();
        if( action_type == reject~  ) = o.upsync();
        o.path_modified = false;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.cycle
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    o.mutex.lock();
    o.path_modified = o.rts_path_modified;
    o.activate      = o.rts_activate;
    bl_t cancel_clicked = o.rts_cancel_clicked;
    o.rts_path_modified  = false;
    o.rts_activate       = false;
    o.rts_cancel_clicked = false;
    if( o.path_modified ) o.path =< o.rts_path.clone();
    o.mutex.unlock();

    bl_t take_action = ( o.path_modified && o.take_action_on == modified~ ) || ( ( o.activate ) && o.take_action_on == activate~ );
    if( take_action ) o.take_action( action_type );

    if( o.activate )
    {
        tp_t action_type = escalate~;
        o.client_activate( o, action_type.1 );
        if( o.close_window && o.nearest_window() ) o.nearest_window().requesting_close();
        o.activate = false;
    }

    if( cancel_clicked )
    {
        if( o.close_window && o.nearest_window() ) o.nearest_window().requesting_close();
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.downsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    if( o.path_modified )
    {
        if( o.st_to_client( o.path! ) )
        {
            {
                m$* msg = st_s!^;
                msg.push_fa( "Converting \"#<sc_t>\" into <#name>:\n", o.path.sc, o.client_type );
                bcore_error_pop_all_to_st( msg );
                bgfe_popup_message_s!^.run( o, "Error", msg.sc );
            }
            o.upsync();
        }
        else
        {
            tp_t action_type = TYPEOF_escalate;
            o.client_change_confirm( o, action_type.1 );
        }
        o.path_modified = false;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:s) bgfe_frame.upsync
{
    if( !o.is_open ) = 0; // no error because frame window could have been closed

    m$* client_path = st_s!^;
    o.client_to_st( client_path );

    bcore_msg_fa( "#name: upsync #<sc_t>\n", o._, client_path.sc );

    if( !o.path || !client_path.equal( o.path ) )
    {
        o.path =< client_path.clone();
        o.mutex.lock();
        o.rts_path =< o.path.clone();
        o.mutex.unlock();

        o.rte.run( o.rtt_set_path.cast( bgfe_rte_fp_rtt ), o, o.rts_path );
        o.path_modified = false;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
