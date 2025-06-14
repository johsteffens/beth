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

/** Action types delegate certain type of standard actions actions
 *  - 'approve'     : The client approves by requesting an automatic downsync in case of front-end-modifcation. The frame will perform the downsync and sends a downsync_confirm where changes occurred.
 *  - 'acknowledge' : The client acknowledges the input but no further action is requested. The client may perform a downsync itself.
 *  - 'escalate'    : The client acknowledges (as above) and asks the frame to escalate the event to the parent (if existing).
 *  - 'reject'      : The client rejects the request and asks the frame to restore the front-end to the state before the event was triggered.
 */
name approve;
name acknowledge;
name escalate;
name reject;

/// Arrange type
name horizontal;
name vertiocal;

//----------------------------------------------------------------------------------------------------------------------

feature m GtkWidget* rtt_widget( @* o ) { ERR_fa( "Not implemented in '#name'", o._ ); = NULL; }

//----------------------------------------------------------------------------------------------------------------------

feature er_t open    ( m@* o, m bgfe_frame* parent ) = GERR_fa( "Not implemented in '#name'", o._ );
feature er_t close   ( m@* o                       ) = GERR_fa( "Not implemented in '#name'", o._ );
feature er_t cycle   ( m@* o, tp_t action_type     ) = GERR_fa( "Not implemented in '#name'", o._ ); // client: (periodic) polling; preset action type in case of a front-end change
feature er_t upsync  ( m@* o                       ) = GERR_fa( "Not implemented in '#name'", o._ ); // client -> frame sync
feature er_t downsync( m@* o                       ) = GERR_fa( "Not implemented in '#name'", o._ ); // frame -> client sync

/// downsync request to client
feature er_t downsync_request( m@* o, m bgfe_frame* initiator, m tp_t* action_type )
{
    if( o.client() ) o.client().t_frame_downsync_request( o.client_type(), initiator, action_type.1 );
    if( action_type.0 == TYPEOF_escalate )
    {
        if( o.parent() )
        {
            o.parent().downsync_request( initiator, action_type.1 );
        }
    }
    = 0;
}

/// downsync confirmation to client
feature er_t downsync_confirm( m@* o, m bgfe_frame* initiator, m tp_t* action_type )
{
    if( o.client() ) o.client().t_frame_downsync_confirm( o.client_type(), initiator, action_type.1 );
    if( action_type.0 == escalate~ ) if( o.parent() ) o.parent().downsync_confirm( initiator, action_type.1 );
    = 0;
}

/// button was pressed
feature er_t button_pressed( m@* o, m bgfe_frame_button_s* initiator, m tp_t* action_type )
{
    if( o.client() ) o.client().t_frame_button_pressed( o.client_type(), initiator, action_type.1 );
    if( action_type.0 == escalate~ ) if( o.parent() ) o.parent().button_pressed( initiator, action_type.1 );
    = 0;
}

/// close query to client
feature bl_t close_ok( m@* o )
{
    = o.client() ? o.client().t_frame_close_ok( o.client_type() ) : true;
}

/// close confirmation to client
feature er_t close_confirm( m@* o )
{
    = o.client() ? o.client().t_frame_close_confirm( o.client_type() ) : 0;
}

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

/// obtains builder if available
feature er_t get_builder( m@* o, m bgfe_builder** builder ) = 0;

/** Build frame for client:
 *  This function constructs a frame for the specified client based on client properties and stamp's parameters.
 *  Output: frame.
 *  If the type is not supported, frame.1 is set to NULL.
 */
feature er_t build_frame_t( m@* o, m obliv bgfe_client* client, tp_t client_type, tp_t client_name, d :** frame ) export = bgfe_builder_build_frame_t_std( client, client_type, client_name, frame );
feature er_t build_frame  ( m@* o, m aware bgfe_client* client,                   tp_t client_name, d :** frame ) = o.build_frame_t( client, client ? client._ : 0, client_name, frame );

//----------------------------------------------------------------------------------------------------------------------

/// retrieve client and parent info from frame
feature tp_t client_name( @* o );
feature tp_t client_type( @* o );
feature m bgfe_client* client( @* o );
feature m :*           parent( @* o );

/// preferred arrangement
feature tp_t arrangement( @* o ) = TYPEOF_horizontal;

//----------------------------------------------------------------------------------------------------------------------

stamp :list_s x_array { : => []; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// arrangement types
name horizontal;
name vertical;

forward bgfe_builder;

stamp :s
{
    /// parameters
    sz_t width;  // optional preset width
    sz_t height; // optional preset height
    bl_t hide_client_name; // true: does not display client name
    tp_t arrange;          // (0 == auto) | vertical | horizontal

    /** Optional builder using with feature build_frame
     *  This builder is also used for child frames
     *  unless a child frame defines a build itself.
     */
    bgfe_builder => builder;

    /// internals
    bgfe_builder -> local_builder; // builder used to construct content
    hidden bgfe_client* client; // client
    hidden tp_t client_type; // to cover obliv clients
    hidden tp_t client_name; // name under which the client's owner holds the client
    hidden bgfe_frame* parent;
    func bgfe_frame.client = o.client;
    func bgfe_frame.client_type = o.client_type;
    func bgfe_frame.client_name = o.client_name;
    func bgfe_frame.parent = o.parent;

    hidden :list_s => content_list;
    hidden bgfe_rte_s* rte;
    hidden x_mutex_s mutex;
    hidden GtkWidget*   rtt_widget;
    hidden bl_t is_open;

    func bl_t is_empty( @* o ) = ( !o.content_list || o.content_list.size == 0 );
    func bcore_inst_call.down_e o.close();
    func :.rtt_widget = o.rtt_widget;
    func :.open;
    func :.close;
    func :.cycle;
    func :.close_ok;
    func :.close_confirm;
    func :.get_builder;
    func :.build_frame_t;
    func :.arrangement;

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
    func er_t set_client_with_content  ( m@* o, m aware bgfe_client* client,                   tp_t client_name );
    func er_t set_client_with_content_t( m@* o, m obliv bgfe_client* client, tp_t client_type, tp_t client_name );

    /** Adding content:
     *  This function attempts to add a content-frame to which 'content' is the full client.
     *  The content has normally some form of child-relationship to the client (e.g. stamp-member)
     *  Only content which results in a non-empty frame is actually added.
     *  The content is only weakly referenced by the frame.
     *  The content must stay alive while the frame is open.
     */
    func er_t add_content  ( m@* o, m aware bgfe_client* content,                    tp_t content_name );
    func er_t add_content_t( m@* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name );

    func :.downsync;
    func :.upsync;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bgfe_frame.emb.x";

