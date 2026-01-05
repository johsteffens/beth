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

forward bgfe_window_s;
forward bgfe_window_list_s;
forward bgfe_frame_link_s;

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

/** Like Setup client above, plus the client's content is added as frame-content.
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

/** Adding linked content:
 *  Linked content is content dynamically owned by this frame's client (typically a declared link).
 *  It is represented by a button which opens a window with the respective content object.
 *  Unlike add_content above, the added content object may be NULL, which means that opening the
 *  content window will create the content object at the same time.
 */
func er_t add_linked_content   ( m@* o, tp_t content_name ) export = o.add_linked_content_with_frame( content_name, NULL );
func er_t add_linked_content_sc( m@* o, sc_t content_name ) export = o.add_linked_content_with_frame( bentypeof( content_name ), NULL );

/** Adding linked content with customizable frame
 *  Linked content is content dynamically owned by this frame's client (typically a declared link).
 *  It is represented by a button which opens a window with the respective content object.
 *  Unlike add_content above, the added content object may be NULL, which means that opening the
 *  content window will create the content object at the same time.
 *  frame_link: optional custom frame_link (NULL for default frame_link)
 */
feature er_t add_linked_content_with_frame   ( m@* o, tp_t content_name, c bgfe_frame_link_s* frame ) = 0;
feature er_t add_linked_content_with_frame_sc( m@* o, sc_t content_name, c bgfe_frame_link_s* frame ) = o.add_linked_content_with_frame( bentypeof( content_name ), frame );

/** Adds a frame to the content list.
 *  The client of that frame and the client of o need not be related or even exist.
 *  No effect if frame does not support content.
 *  This function forks the frame reference.
 */
feature er_t add_frame( m@* o, m bgfe_frame* frame )
{
    WRN_fa( "Cannot add a frame to #name\n", o._ );
    = 0;
}

/** Adds a new bgfe_frame_s to the content list and returns its reference.
 *  The client of that frame and the client of o need not be related or even exist.
 */
feature m bgfe_frame_s* add_sub_frame( m@* o )
{
    m$* frame = bgfe_frame_s!^;
    frame.set_client_t( o.client(), o.client_type(), 0 );
    o.add_frame( frame ); // frame stays alive by adding it
    = frame;
}

/// Removes content (frame_s or window_s)
feature er_t clear_content( m@* o ) = 0;

/// Adds a label frame with text
func er_t add_label( m@* o, sc_t text, sz_t label_width )
{
    m$* label = bgfe_frame_label_s!^;
    label.set_width( label_width );
    label.set_x_align( 0 );
    label.text!.copy_sc( text );
     = o.add_frame( label );
}

/** Adding content with preceding label:
 *  This function adds a label and content in a horizontally aligned sub-frame.
 */
func er_t add_label_content( m@* o, sc_t text, sz_t label_width, m aware bgfe_client* content, tp_t content_name )
{
    if( !content ) = 0;
    = o.add_label_content_t( text, label_width, content, content._, content_name );
}

func er_t add_label_content_t( m@* o, sc_t text, sz_t label_width, m obliv bgfe_client* content, tp_t content_type, tp_t content_name )
{
    m$* f = o.add_sub_frame();
    f.set_show_border( false );
    f.set_stretch( false );
    f.set_center( false );
    f.set_arrange( horizontal~ );
    f.add_label( text, label_width );
    f.add_content_t( content, content_type, content_name );
    = 0;
}

/** Adding content with pre- and post-label:
 *  This function adds a label and content in a horizontally aligned sub-frame.
 */
func er_t add_label_content_label( m@* o, sc_t pre_text, sz_t pre_label_width, m aware bgfe_client* content, tp_t content_name, sc_t post_text, sz_t post_label_width )
{
    if( !content ) = 0;
    = o.add_label_content_t_label( pre_text, pre_label_width, content, content._, content_name, post_text, post_label_width );
}

func er_t add_label_content_t_label( m@* o, sc_t pre_text, sz_t pre_label_width, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, sc_t post_text, sz_t post_label_width )
{
    m$* f = o.add_sub_frame();
    f.set_show_border( false );
    f.set_stretch( false );
    f.set_center( false );
    f.set_arrange( horizontal~ );
    f.add_label( pre_text, pre_label_width );
    f.add_content_t( content, content_type, content_name );
    f.add_label( post_text, post_label_width );
    = 0;
}

/** Vertical and horizontal complexity.
 *  The complexity indicates how much space the widget of a frame tends to occupy in horizontal and vertical direction.
 *  It can be used to decide about arrangements and embedding of composite frames.
 *  The unit size cor complexity represents the typically size of a elementary widget (e.g. a checkbox).
 *  For example: A line editor has typically a v-complexity of 1 and a h-complexity between 4 and 10.
 *  Composite frames sum up the complexity of their members accordingly.
 */
feature f3_t h_complexity( @* o ) = 1;
feature f3_t v_complexity( @* o ) = 1;


/// where a feature size is defined by width and height
func f3_t complexity_unit_size() = 24;

/** Indicates if the frame has a compact format.
 *  This information is used by frames that decide whether to embed a feature or display it in a dedicated window.
 *  Implementation should take into account that o can be NULL.
 */
feature 'at' bl_t is_compact( @* o ) = false;

/** Request sent from an immediate child to open a new window.
 *  This is typically generated from a link-frame.
 *  The parent frame may respond and take action according to its window policy.
 *  'action_type' response should be approve~ or reject~.
 *  This event has no escalation.
 */
feature er_t open_window_request( m@* o, m tp_t* action_type ) { action_type.0 = approve~; = 0; }

/** Request sent from a parent to close a window if any is open.
 *  This is typically generated from a holder of a link-frame based on its window policy.
 */
feature er_t close_window_request( m@* o ) = 0;

feature er_t client_get_glimpse( @* o, sz_t max_chars, m st_s* glimpse )
{
    if( !o.client() ) = 0;
    glimpse.clear();
    if( o.client_type() == st_s~ )
    {
        st_s* src = o.client().cast( st_s* );
        if( src.size <= max_chars )
        {
            glimpse.copy( src );
        }
        else
        {
            glimpse.push_fa( "... #<sc_t>", src.crop( src.size - max_chars + 4, -1 )^.sc );
        }
    }
    else if( bgfe_client_t_defines_bgfe_get_glimpse( o.client_type() ) )
    {
        o.client().t_bgfe_get_glimpse( o.client_type(), glimpse );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// retrieve client and parent info from frame
feature tp_t client_name( @* o ) = 0;
feature tp_t client_type( @* o ) = 0;
feature m bgfe_client* client( @* o ) = NULL;
feature m :*           parent( @* o ) = NULL;
feature bl_t           is_open( @* o ) = false;

feature void set_parent( m@* o, m :* parent ) {};

/// preferred arrangement
feature tp_t arrangement( @* o ) = TYPEOF_horizontal;

/// Returns the nearest (towards root) window or NULL if none is present.
func m bgfe_window_s* nearest_window( m @* o )
{
    if( o._ == bgfe_window_s~ )  = o.cast( m bgfe_window_s* );
    if( o._ == bgfe_frame_s~ && o.cast( m bgfe_frame_s* ).window != NULL )  = o.cast( m bgfe_frame_s* ).window;
    if( !o.parent() ) = NULL;
    = o.parent().nearest_window();
}

//----------------------------------------------------------------------------------------------------------------------

/// Returns the nearest (towards root) client if defined
func er_t get_nearest_client( m @* o, m bgfe_client** client, m tp_t* client_type )
{
    if( o.client() )
    {
        if( client      ) client.1 = o.client();
        if( client_type ) client_type.0 = o.client_type();
        = 0;
    }

    if( o.parent() ) = o.parent().get_nearest_client( client, client_type );
    = 0;
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
        case bcore_img_u2_s~: = bgfe_frame_canvas_s~;
        case bcodec_image_bgra_s~: = bgfe_frame_canvas_s~;
        case bcodec_image_yuyv_s~: = bgfe_frame_canvas_s~;
        case bmath_mf2_s~: = bgfe_frame_canvas_s~;
        case bmath_mf3_s~: = bgfe_frame_canvas_s~;

        default   : = bgfe_frame_s~;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// arrangement types
name horizontal;
name vertical;

/// window policy types
name any;  // any child-window may be opened
name one;  // maximally one child-window (child-window may a have sub-windows)
name zero; // no child window

stamp :s
{
    /// parameters
    sz_t width;  // optional preset width
    sz_t height; // optional preset height
    bl_t insensitive; // insensitive: does not react to user actions
    bl_t show_client_name = true; // true: displays client name;
    bl_t show_border      = true;
    tp_t arrange = square;
    st_s => title;       // optional title (overrides client name)
    st_s => widget_name; // optional gtk widget name overrides default widget name
    sz_t spacing;   // spacing between elements
    bl_t end_bound; // packs elements with reference to the end of the box (false: reference to the start)
    bl_t center = true;  // centers widgets in an expanded space
    bl_t stretch = true; // stretches elements to fill expanded space
    sz_t nesting_level;  // nesting level (embedded nesting)
    tp_t window_policy = any; // window-policy: any~ | one~ | zero~;
    bl_t manual_content; // true: ignores content in function set_client_with_content (content is intended to be added manually via function add_content)

    func bgfe_frame.set_width   { o.width   = value; = 0; }
    func bgfe_frame.set_height  { o.height  = value; = 0; }
    func bgfe_frame.set_insensitive { o.insensitive = flag; = 0; }
    func bgfe_frame.set_arrange { o.arrange = name; = 0; }
    func bgfe_frame.set_show_client_name { o.show_client_name = flag; = 0; }
    func bgfe_frame.set_show_border { o.show_border = flag; = 0; }
    func bgfe_frame.set_widget_name { o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_title       { o.title!.copy_sc( text ); = 0; }
    func bgfe_frame.set_spacing     { o.spacing   = value; = 0; }
    func bgfe_frame.set_end_bound   { o.end_bound = flag; = 0; }
    func bgfe_frame.set_center      { o.center    = flag; = 0; }
    func bgfe_frame.set_stretch     { o.stretch   = flag; = 0; }
    func bgfe_frame.set_nesting_level  { o.nesting_level = value; = 0; }
    func bgfe_frame.set_window_policy  { o.window_policy = name; = 0; }
    func bgfe_frame.set_manual_content { o.manual_content = flag; = 0; }

    /// internals (client can be NULL; when client is != NULL, client_type != 0 is also provided)
    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name under which the client's owner holds the client
    hidden bgfe_frame* parent;
    hidden bgfe_window_s* window; // associated window; NULL if frame has no associated window

    hidden f3_t h_complexity; // computed during opening
    hidden f3_t v_complexity; // computed during opening
    hidden bl_t vertical; // vertical arrangement (computed during opening)

    func bgfe_frame.client = o.client;
    func bgfe_frame.client_type = o.client_type;
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.set_parent o.parent = parent;
    func bgfe_frame.is_open = o.is_open;
    func bgfe_frame.h_complexity { if( o.is_open ) = o.h_complexity; ERR_fa( "Frame is not open." ); = 1; }
    func bgfe_frame.v_complexity { if( o.is_open ) = o.v_complexity; ERR_fa( "Frame is not open." ); = 1; }
    func bgfe_frame.is_compact = false;

    hidden :list_s            => content_list;
    hidden bgfe_window_list_s => window_list; // windows are part of cycling

    hidden bgfe_rte_s* rte;
    hidden GtkWidget* rtt_widget;
    hidden GtkWidget* rtt_main_box;
    hidden GtkWidget* rtt_content_box;
    hidden bl_t is_open;

    func bl_t is_empty( @* o ) = ( !o.content_list || o.content_list.size == 0 );
    func bcore_inst_call.down_e o.close();
    func :.rtt_widget = o.rtt_widget;
    func :.open;
    func :.close;
    func :.cycle;
    func :.arrangement = o.vertical ? TYPEOF_vertical : TYPEOF_horizontal;
    func :.client_close_request;
    func :.client_close_confirm;
    func :.client_distraction;

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
    func :.add_linked_content_with_frame;

    /** Adding frames:
     *  A frame is added to the content-list or a window to the window-list.
     *  The client of that frame and the client of o need not be related or even exist.
     *  If frame is a window (bgfe_window_s):
     *    - If o is open the window is opened immediately, otherwise it is opened when o is opened.
     *    - The window is cycled by o.
     *    - When the window closes while o is open, the window is removed from the window-list
     *    - When o closes, all windows are closed as well.
     *  If frame is not a window:
     *    - The frame is added to the content-list and displayed accordingly.
     *    - The frame must not be open. It can not be added while o is open (warning to stderr).
     */
    func :.add_frame;

    /** Clears all content
     *  Requires frame to be closed
     */
    func :.clear_content;

    func :.downsync;
    func :.upsync;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// utilities

func er_t ancestry_to_sink( @* o, m x_sink* sink  )
{
    if( !o ) = 0;
    sink.push_fa( "#name(#name)", o._, o.client_name() );
    if( o.parent() )
    {
        sink.push_fa( "<-" );
        o.parent().ancestry_to_sink( sink  );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t ancestry_to_stdout( @* o )
{
    o.ancestry_to_sink( x_sink_stdout() );
    bcore_msg_fa( "\n" );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

identifier GTK_WIDGET, G_IS_INITIALLY_UNOWNED;
identifier g_object_ref_sink, g_signal_handlers_disconnect_by_data, g_object_unref, gtk_container_remove;
identifier gtk_container_foreach, gtk_widget_grab_focus, G_OBJECT;
type GObject;

//----------------------------------------------------------------------------------------------------------------------

func er_t rtt_attach_object( m@* o, m GObject* new_object, m GObject** attched_object )
{
    if( attched_object.1 != NULL ) o.rtt_detach_object( attched_object );
    if( new_object.1 == NULL ) = GERR_fa( "#name: new_object is NULL (object creation failed)\n", o._ );
    if( G_IS_INITIALLY_UNOWNED( new_object ) ) new_object = g_object_ref_sink( new_object );
    attched_object.1 = new_object;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t rtt_detach_object( m@* o, m GObject** object )
{
    if( object.1 == NULL ) = 0;
    g_signal_handlers_disconnect_by_data( object.1, o );
    g_object_unref( object.1 );
    object.1 = NULL;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t rtt_attach_widget( m@* o, m GtkWidget* new_widget, m GtkWidget** attched_widget ) = o.rtt_attach_object( new_widget.cast( m GObject* ), attched_widget.cast( m GObject** ) );
func er_t rtt_detach_widget( m@* o, m GtkWidget** widget ) = o.rtt_detach_object( widget.cast( m GObject** ) );

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
    if( container == NULL ) return;
    gtk_container_remove( GTK_CONTAINER( container ), widget );
}

//----------------------------------------------------------------------------------------------------------------------

func er_t rtt_remove_all_widgets_from_container( m@* o, m GtkWidget* container )
{
    if( container == NULL ) = 0;
    gtk_container_foreach( GTK_CONTAINER( container ), bgfe_frame_rtt_callback_remove_all_widgets_from_container, container );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t rtt_grab_focus( m@* o, vd_t widget )
{
    if( widget == NULL ) = 0;
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

/** Places frame at the upper right position relative to o
 */
func er_t place_at_upper_right( m@* o, m bgfe_frame* frame )
{
    m$* shell = bgfe_location_shell_s!^;
    shell.setup( o );
    shell.place_frame_at_upper_right( o.rtt_widget(), frame );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/** Places frame at the upper right position relative to o
 */
func er_t place_at_lower_left( m@* o, m bgfe_frame* frame )
{
    m$* shell = bgfe_location_shell_s!^;
    shell.setup( o );
    shell.place_frame_at_lower_left( o.rtt_widget(), frame );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// Places frame at one of the listed positions
func er_t place_at_position( m@* o, m bgfe_frame* frame, tp_t position )
{
    m$* shell = bgfe_location_shell_s!^;
    shell.setup( o );
    switch( position )
    {
        case upper_right~: shell.place_frame_at_upper_right( o.rtt_widget(), frame ); break;
        case lower_left~ : shell.place_frame_at_lower_left ( o.rtt_widget(), frame ); break;
        default: break;
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bgfe_frame.emb.x";

