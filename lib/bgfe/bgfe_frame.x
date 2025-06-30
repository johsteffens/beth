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
 */


/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bgfe_frame_features_property.emb.x"; // property features
embed "bgfe_frame_features_client.emb.x";   // client <-> frame communication

//----------------------------------------------------------------------------------------------------------------------

/// Arrange type
name horizontal;
name vertical;

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

//----------------------------------------------------------------------------------------------------------------------

/// retrieve client and parent info from frame
feature tp_t client_name( @* o );
feature tp_t client_type( @* o );
feature m bgfe_client* client( @* o );
feature m :*           parent( @* o );
feature bl_t           is_open( @* o );

/// when the client is a link, embed the frame rather than displaying it in a dedicated window.
feature bl_t embed_link( @* o ) { = false; }

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
    tp_t arrange;          // (0 == auto) | vertical | horizontal
    st_s => widget_name;   // optional gtk widget name overrides default widget name
    sz_t spacing;   // spacing between elements
    bl_t end_bound; // packs elements with reference to the end of the box (false: reference to the start)
    bl_t center;    // centers widgets in an expanded space
    bl_t stretch;   // stretches elements to fill expanded space

    func bgfe_frame.set_width   { o.width   = value; = 0; }
    func bgfe_frame.set_height  { o.height  = value; = 0; }
    func bgfe_frame.set_arrange { o.arrange = arrange; = 0; }
    func bgfe_frame.set_show_client_name { o.show_client_name = flag; = 0; }
    func bgfe_frame.set_widget_name{ o.widget_name!.copy_sc( text ); = 0; }
    func bgfe_frame.set_spacing  { o.spacing   = value; = 0; }
    func bgfe_frame.set_end_bound{ o.end_bound = flag; = 0; }
    func bgfe_frame.set_center   { o.center    = flag; = 0; }
    func bgfe_frame.set_stretch  { o.stretch   = flag; = 0; }

    /// internals
    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name under which the client's owner holds the client
    hidden bgfe_frame* parent;
    hidden bgfe_window_s* window; // associated window; NULL if frame has no associated window
    func bgfe_frame.client = o.client;
    func bgfe_frame.client_type = o.client_type;
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;
    func bgfe_frame.is_open = o.is_open;

    hidden :list_s => content_list;
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget* rtt_widget;
    hidden GtkWidget* rtt_gtk_box;
    hidden bl_t is_open;

    func bl_t is_empty( @* o ) = ( !o.content_list || o.content_list.size == 0 );
    func bcore_inst_call.down_e o.close();
    func :.rtt_widget = o.rtt_widget;
    func :.open;
    func :.close;
    func :.cycle;
    func :.arrangement;
    func :.client_close_ok;
    func :.client_close_confirm;

    /// Notifications ...
    bgfe_notify_ligand_pool_s => ligand_pool;
    func bgfe_notify_sender.ligand_pool = o.ligand_pool!;
    func er_t notify_send( m@* o, tp_t type ) { if( o.ligand_pool ) o.ligand_pool.send( type, o ); = 0; }
    func :.client_change_confirm    { o.notify_send( change_confirm~    ); = o.client_change_confirm_default   ( initiator, action_type ); }
//    func :.client_link_change_confirm { o.notify_send( link_change_confirm~ ); = o.client_link_change_confirm_default( initiator, action_type ); }

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
    func er_t add_content  ( m@* o, m aware bgfe_client* content,                    tp_t content_name );
    func er_t add_content_t( m@* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name );

    /** Adding linked content:
     *  Linked content is content dynamically owned by this frame's client (typically a declared link).
     *  It is represented by a button which opens a window with the respective content object.
     *  Unlike add_content above, the added content object may be NULL, which means that opening the
     *  content window will create the content object at the same time.
     */
    func er_t add_linked_content( m@* o, tp_t content_name );

    func :.downsync;
    func :.upsync;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bgfe_frame.emb.x";

