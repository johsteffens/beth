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

/** Frame for displaying/editing an object.
 *  A frame is a reflection of the editable/displayable part of the object.
 *  A frame has a tree structure akin the object's tree structure.
 *  The object is a member of the 'client'.
 *  The client owns (or otherwise masters) the frame.
 *  The frame knows its parent at runtime.
 *  The frame knows its client when it is the root frame.
 *  The root frame normally has a client at runtime.
 *
 *  Some frames (e.g. bgfe_frame_s) can also exists without a client but still
 *  part of a node in a frame-tree. Such frames pass client-related signals up
 *  the tree if they are not root.
 */


/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bgfe_frame_features_property.emb.x"; // property features
embed "bgfe_frame_features_client.emb.x";   // client <-> frame communication

//----------------------------------------------------------------------------------------------------------------------

/// Arrange type
name horizontal; // always horizontal arrangement
name vertical;   // always vertical arrangement
name min_volume;       // choose vertical or horizontal based on which volume is smaller
name square;           // choose vertical or horizontal based on which complexity is closer to a square
name hor_golden_ratio; // choose vertical or horizontal based on which complexity is closer to a horizontal rectangle of golden ratio
name ver_golden_ratio; // choose vertical or horizontal based on which complexity is closer to a vertical rectangle of golden ratio

//----------------------------------------------------------------------------------------------------------------------

feature m GtkWidget* rtt_widget( @* o ) { ERR_fa( "Not implemented in '#name'", o._ ); = NULL; }

//----------------------------------------------------------------------------------------------------------------------

feature er_t open    ( m@* o, m bgfe_frame* parent ) = GERR_fa( "Not implemented in '#name'", o._ );
feature er_t close   ( m@* o                       ) = GERR_fa( "Not implemented in '#name'", o._ );
feature er_t cycle   ( m@* o, tp_t action_type     ) = GERR_fa( "Not implemented in '#name'", o._ ); // client: (periodic) polling; preset action type in case of a front-end change
feature er_t upsync  ( m@* o                       ) = GERR_fa( "Not implemented in '#name'", o._ ); // client -> frame sync
feature er_t downsync( m@* o                       ) = GERR_fa( "Not implemented in '#name'", o._ ); // frame -> client sync

/** Setup client:
 *  This function defines a client to the frame.
 *  The client is only weakly referenced by the frame.
 *  The client must stay alive while the frame is open.
 *  --> Explicitly close the frame before the client is destroyed.
 *  The client is notified via client-features about gfe-changes to its content.
 *  The optional client-name is the name under which the client is identified by a possible parent.
 */
feature er_t set_client_t( m@* o, m obliv bgfe_client* client, tp_t client_type, tp_t client_name );
feature er_t set_client  ( m@* o, m aware bgfe_client* client,                   tp_t client_name ) = o.set_client_t( client, client ? client._ : 0, client_name );

/** Like Setup client above, plus all the client's content is added as frame-content.
 *  Note that only elements resulting in a non-empty frame are actually added.
 *  This function defaults to set_client on leaf frames.
 */
feature er_t set_client_with_content  ( m@* o, m aware bgfe_client* client,                   tp_t client_name ) = o.set_client  ( client,              client_name );
feature er_t set_client_with_content_t( m@* o, m obliv bgfe_client* client, tp_t client_type, tp_t client_name ) = o.set_client_t( client, client_type, client_name );

/** Adds content to a frame.
 *  No effect if frame does not support content.
 */
feature er_t add_content  ( m@* o, m aware bgfe_client* content,                    tp_t content_name ) = 0;
feature er_t add_content_t( m@* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name ) = 0;

/** Adds a frame to the content list.
 *  The client of that frame and the client of o need not be related or even exist.
 *  No effect if frame does not support content.
 */
feature er_t add_frame( m@* o, m bgfe_frame* frame ) = 0;

/** Vertical and horizontal complexity.
 *  The complexity indicates how much space the widget of a frame tends to occupy in horizontal and vertical direction.
 *  It can be used to decide about arrangements and embedding of composite frames.
 *  The unit size cor complexity represents the typically size of a elementary widget (e.g. a checkbox).
 *  For example: A line editor has typically a v-complexity of 1 and a h-complexity between 4 and 10.
 *  Composite frames sum up the complexity of their members accordingly.
 */
feature f3_t h_complexity( @* o ) = 1;
feature f3_t v_complexity( @* o ) = 1;


/** Wrapping level used to determine border and color styling.
 *  bgfe_frame_s tends to have a wrap level by one higher than the highest wrap level of its elements.
 */
feature sz_t wrap_level( @* o ) = 0;


/// where a feature size is defined by width and height
func f3_t complexity_unit_size() = 24;

/** Indicates if the frame has a compact format.
 *  This information is used by frames that decide whether to embed a feature or display it in a dedicated window.
 *  Implementation should take into account that o can be NULL.
 */
feature 'at' bl_t is_compact( @* o ) = false;

//----------------------------------------------------------------------------------------------------------------------

/// retrieve client and parent info from frame
feature tp_t client_name( @* o ) = 0;
feature tp_t client_type( @* o ) = 0;
feature m bgfe_client* client( @* o ) = NULL;
feature m :*           parent( @* o ) = NULL;
feature bl_t           is_open( @* o ) = false;

/// preferred arrangement
feature tp_t arrangement( @* o ) = TYPEOF_horizontal;

/// Returns the nearest (towards root) window or NULL if none is present.
forward bgfe_window_s;
func m bgfe_window_s* nearest_window( m @* o )
{
    if( o._ == bgfe_frame_s~ && o.cast( m bgfe_frame_s* ).window != NULL )  = o.cast( m bgfe_frame_s* ).window;
    if( !o.parent() ) = NULL;
    = o.parent().nearest_window();
}

//----------------------------------------------------------------------------------------------------------------------

stamp :list_s x_array { : => []; }

//----------------------------------------------------------------------------------------------------------------------

/// creates a default frame for the specified client
func tp_t default_frame_type( tp_t client_type )
{
    switch( client_type )
    {
        case f3_t~: = bgfe_frame_scale_s~;
        case f2_t~: = bgfe_frame_scale_s~;
        case s3_t~: = bgfe_frame_entry_s~;
        case s2_t~: = bgfe_frame_entry_s~;
        case s1_t~: = bgfe_frame_entry_s~;
        case s0_t~: = bgfe_frame_entry_s~;
        case u3_t~: = bgfe_frame_entry_s~;
        case u2_t~: = bgfe_frame_entry_s~;
        case u1_t~: = bgfe_frame_entry_s~;
        case u0_t~: = bgfe_frame_entry_s~;
        case sz_t~: = bgfe_frame_entry_s~;
        case uz_t~: = bgfe_frame_entry_s~;
        case tp_t~: = bgfe_frame_entry_s~;
        case er_t~: = bgfe_frame_entry_s~;
        case aware_t~: = bgfe_frame_entry_s~;
        case st_s~: = bgfe_frame_entry_s~;
        case sd_t~: = bgfe_frame_entry_s~;
        case sc_t~: = bgfe_frame_label_s~;
        case bl_t~: = bgfe_frame_check_button_s~;
        default   : = bgfe_frame_s~;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// arrangement types
name horizontal;
name vertical;

stamp :s
{
    /// parameters
    sz_t width;  // optional preset width
    sz_t height; // optional preset height
    bl_t show_client_name = true; // true: does not display client name
    bl_t show_border      = true;
    tp_t arrange = square;
    st_s => widget_name;   // optional gtk widget name overrides default widget name
    sz_t spacing;   // spacing between elements
    bl_t end_bound; // packs elements with reference to the end of the box (false: reference to the start)
    bl_t center = true;    // centers widgets in an expanded space
    bl_t stretch = true;   // stretches elements to fill expanded space

    func bgfe_frame.set_width   { o.width   = value; = 0; }
    func bgfe_frame.set_height  { o.height  = value; = 0; }
    func bgfe_frame.set_arrange { o.arrange = arrange; = 0; }
    func bgfe_frame.set_show_client_name { o.show_client_name = flag; = 0; }
    func bgfe_frame.set_show_border { o.show_border = flag; = 0; }
    func bgfe_frame.set_widget_name{ o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_spacing  { o.spacing   = value; = 0; }
    func bgfe_frame.set_end_bound{ o.end_bound = flag; = 0; }
    func bgfe_frame.set_center   { o.center    = flag; = 0; }
    func bgfe_frame.set_stretch  { o.stretch   = flag; = 0; }

    /// internals (client can be NULL; when client is != NULL, client_type != 0 is also provided)
    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name under which the client's owner holds the client
    hidden bgfe_frame* parent;
    hidden bgfe_window_s* window; // associated window; NULL if frame has no associated window

    hidden f3_t h_complexity; // computed during opening
    hidden f3_t v_complexity; // computed during opening
    hidden sz_t wrap_level;   // computed during opening
    hidden bl_t vertical; // vertical arrangement (computed during opening)

    func bgfe_frame.client = o.client;
    func bgfe_frame.client_type = o.client_type;
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.is_open = o.is_open;
    func bgfe_frame.h_complexity { if( o.is_open ) = o.h_complexity; ERR_fa( "Frame is not open." ); = 1; }
    func bgfe_frame.v_complexity { if( o.is_open ) = o.v_complexity; ERR_fa( "Frame is not open." ); = 1; }
    func bgfe_frame.wrap_level   { if( o.is_open ) = o.wrap_level; ERR_fa( "Frame is not open." ); = 0; }
    func bgfe_frame.is_compact = false;

    hidden :list_s => content_list;
    hidden bgfe_rte_s* rte;
    hidden GtkWidget* rtt_widget;
    hidden GtkWidget* rtt_gtk_box;
    hidden bl_t is_open;

    func bl_t is_empty( @* o ) = ( !o.content_list || o.content_list.size == 0 );
    func bcore_inst_call.down_e o.close();
    func :.rtt_widget = o.rtt_widget;
    func :.open;
    func :.close;
    func :.cycle;
    func :.arrangement = o.vertical ? TYPEOF_vertical : TYPEOF_horizontal;
    func :.client_close_ok;
    func :.client_close_confirm;

    /// Notifications ...
    bgfe_notify_ligand_pool_s => ligand_pool;
    func bgfe_notify_sender.supports_notify = true;
    func bgfe_notify_sender.ligand_pool = o.ligand_pool!;
    func er_t notify_send( m@* o, tp_t type ) { if( o.ligand_pool ) o.ligand_pool.send( type, o ); = 0; }
    func :.client_change_confirm  { o.notify_send( change_confirm~    ); = o.client_change_confirm_default( initiator, action_type ); }

    /// Interface functions ...

    /** Setup client:
     *  This function defines a client to the frame.
     *  After the client was defined, content should be manually added (add_content).
     *  The client is only weakly referenced by the frame.
     *  The client must stay alive while the frame is open.
     *  --> Explicitly close the frame before the client is destroyed.
     *  The client is notified via client-features about gfe-changes to its content.
     */
    func :.set_client_t;

    /** Like Setup client above, plus all the client's content is added as frame-content.
     *  Note that only elements resulting in a non-empty frame are actually added.
     */
    func :.set_client_with_content;
    func :.set_client_with_content_t;

    /** Adding content:
     *  This function attempts to add a content-frame to which 'content' is the full client.
     *  The content has normally some form of child-relationship to the client (e.g. stamp-member)
     *  Only content which results in a non-empty frame is actually added.
     *  The content is only weakly referenced by the frame.
     *  The content must stay alive while the frame is open.
     */
    func :.add_content;
    func :.add_content_t;

    /** Adding linked content:
     *  Linked content is content dynamically owned by this frame's client (typically a declared link).
     *  It is represented by a button which opens a window with the respective content object.
     *  Unlike add_content above, the added content object may be NULL, which means that opening the
     *  content window will create the content object at the same time.
     */
    func er_t add_linked_content( m@* o, tp_t content_name );

    /** Adds a frame to the content list.
     *  The client of that frame and the client of o need not be related or even exist.
     */
    func :.add_frame;

    func :.downsync;
    func :.upsync;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// utilities

//----------------------------------------------------------------------------------------------------------------------

/// attach widget of o
func er_t rtt_attach_widget( m@* o, m GtkWidget* new_widget, m GtkWidget** attched_widget )
{
    if( attched_widget.1 != NULL ) o.rtt_detach_widget( attched_widget );
    if( new_widget.1 == NULL ) = GERR_fa( "#name: new_widget is NULL (widget creation failed)\n", o._ );
    if( G_IS_INITIALLY_UNOWNED( new_widget ) ) new_widget = g_object_ref_sink( new_widget );
    attched_widget.1 = new_widget;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// detaches a member widget of o
func er_t rtt_detach_widget( m@* o, m GtkWidget** widget )
{
    if( widget.1 == NULL ) = 0;
    g_signal_handlers_disconnect_by_data( widget.1, o );
    g_object_unref( widget.1 );
    widget.1 = NULL;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t rtt_remove_widget_from_container( m@* o, m GtkWidget* widget, m GtkWidget* container )
{
    if( widget == NULL ) = 0;
    if( container == NULL ) = 0;
    gtk_container_remove( GTK_CONTAINER( container ), widget );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func void rtt_callback_remove_all_widgets_from_container( m GtkWidget* widget, vd_t container )
{
    gtk_container_remove( GTK_CONTAINER( container ), widget );
}

//----------------------------------------------------------------------------------------------------------------------

func er_t rtt_remove_all_widgets_from_container( m@* o, m GtkWidget* container )
{
    gtk_container_foreach( GTK_CONTAINER( container ), bgfe_frame_rtt_callback_remove_all_widgets_from_container, container );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_widget_grab_focus;
identifier GTK_WIDGET;

func er_t rtt_grab_focus( m@* o, vd_t widget )
{
    gtk_widget_grab_focus( GTK_WIDGET( widget ) );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

feature er_t grab_focus( m@* o )
{
    m bgfe_rte_s* rte = 0;
    bgfe_rte_get( rte.2 );

    if( rte.1 ) rte.1.run( o.rtt_grab_focus.cast( bgfe_rte_fp_rtt ), o, o.rtt_widget() );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bgfe_frame.emb.x";

