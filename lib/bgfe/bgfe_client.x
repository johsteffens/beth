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

forward bgfe_frame;
forward bgfe_frame_button_s;
forward bgfe_frame_entry_s;

/**********************************************************************************************************************/

/** Data editing
 *  If 'content' == 'o', the entered information refers to the client (before escalation).
 *  If 'content' is != 'o', the entered information refers to a client member.
 *  If a unique match is required, a handler ideally matches the address 'content'.
 *  Matching content_type or content_name will affect all nested elements that escalate the request.
 *  To prevent further escalation, set action_type to approve~ or 0.
 */

//----------------------------------------------------------------------------------------------------------------------

/// Allows editing the frame type.
feature 'at' er_t bgfe_edit_frame_type( @* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m tp_t* action_type, m tp_t* frame_type ) { action_type.0 = escapprove~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Presents the client the frame serving it and allows the client to editing the frame properties inside this function.
 *  The client may reference 'frame' for later (property-invariant) usage (e.g. calling 'upsync', 'downsunc', etc)
 *  For editing, preferably use frame-property features. See bgfe_frame_features_propery.emb.x.
 */
feature 'at' er_t bgfe_edit_frame( m@* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m tp_t* action_type, m bgfe_frame* frame ) = 0;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Frame interaction - Escalating

//----------------------------------------------------------------------------------------------------------------------

/** Notification from frame to client that the GFE received an input to change a variable.
 *  The client can take its own action.
 *  The client can delegate action to the frame in the following ways via action_type:
 *  - 'approve'     : The client approves the input. The frame will perform the downsync. A change_confirm is sent in afterwards
 *  - 'acknowledge' : The client acknowledges the request but initiates the change itself via downsync.
 *  - 'escalate'    : 'acknowledge' with the frame passing the request to the parent. (escalate is default, in case the client does not overload the request)
 *  - 'escapprove'  : 'acknowledge' with the frame passing the notification to the parent; approve in case of no parent.
 *  - 'reject'      : The client rejects the request. The frame discards the GFE-input and sets the affected control back to the client's value.
 */
feature 'at' er_t bgfe_change_request( m@* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m bgfe_frame* initiator, m tp_t* action_type ) { action_type.0 = escapprove~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Notification from frame to client that the frame has performed a change (downsync or link change) on the object.
 *  The client can take action and also respond in one of the following possible ways via action_type:
 *  - 'acknowledge, approve' : The client acknowledges the event.
 *  - 'escalate, escapprove' : 'acknowledge' with the frame passing the notification to the parent.
 */
feature 'at' er_t bgfe_change_confirm( m@* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m bgfe_frame* initiator, m tp_t* action_type ) { action_type.0 = escalate~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Button pressed event.  (Instantly after button was pressed (before released or clicked))
 *  The client can take action and also respond in one of the following possible ways via action_type:
 *  - 'acknowledge, approve' : The client acknowledges the event.
 *  - 'escalate, escapprove' : 'acknowledge' with the frame passing the notification to the parent.
 */
feature 'at' er_t bgfe_button_pressed( m@* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m bgfe_frame* initiator, m tp_t* action_type ) { action_type.0 = escalate~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Button clicked event.  (clicked = pressed and released in succession)
 *  The client can take action and also respond in one of the following possible ways via action_type:
 *  - 'acknowledge, approve' : The client acknowledges the event.
 *  - 'escalate, escapprove' : 'acknowledge' with the frame passing the notification to the parent.
 */
feature 'at' er_t bgfe_button_clicked( m@* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m bgfe_frame* initiator, m tp_t* action_type ) { action_type.0 = escalate~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Activate event.  (Generated by line text editor (entry); typically on enter pressed)
 *  The client can take action and also respond in one of the following possible ways via action_type:
 *  - 'acknowledge, approve' : The client acknowledges the event.
 *  - 'escalate, escapprove' : 'acknowledge' with the frame passing the notification to the parent.
 */
feature 'at' er_t bgfe_activate( m@* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m bgfe_frame* initiator, m tp_t* action_type ) { action_type.0 = escalate~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Widget's placement changed. (Generated by canvas, window)
 *  The client can take action and also respond in one of the following possible ways via action_type:
 *  - 'acknowledge, approve' : The client acknowledges the event.
 *  - 'escalate, escapprove' : 'acknowledge' with the frame passing the notification to the parent.
 */
feature 'at' er_t bgfe_placement_changed( m@* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m bgfe_frame* initiator, bgfe_placement_s* placement, m tp_t* action_type ) { action_type.0 = escalate~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Widget's mouse status changed. (Generated by canvas)
 *  modified_type indicates which type event caused the modification (press | release | motion | scroll)
 *  The client can take action and also respond in one of the following possible ways via action_type:
 *  - 'acknowledge, approve' : The client acknowledges the event.
 *  - 'escalate, escapprove' : 'acknowledge' with the frame passing the notification to the parent.
 */
feature 'at' er_t bgfe_mouse_changed( m@* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m bgfe_frame* initiator, bgfe_mouse_s* mouse, tp_t modified_type, m tp_t* action_type ) { action_type.0 = escalate~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Frame interaction - Broadcasting

//----------------------------------------------------------------------------------------------------------------------

/** Broadcast request to all affected clients whether closing is permitted based on a GFE close input.
 *  A close request is rejected if any asked client sets action_type 'reject';
 *  If ant asked client sets action_type to 'approve', closing commences and other sub-clients are not asked.
 */
feature 'at' er_t bgfe_close_request( m@* o, m bgfe_frame* initiator, m tp_t* action_type ) { action_type.0 = escapprove~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Broadcast notifcation to all affected clients that their frame is closing.
 *  If any notified client sets action_type to 'approve', sub-clients are not asked.
 */
feature 'at' er_t bgfe_close_confirm( m@* o, m bgfe_frame* initiator, m tp_t* action_type ) { action_type.0 = escalate~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Broadcast notifcation to all affected clients that an ancestor has experienced a distraction.
 *  A distraction typically closes fleeting windows (such a popup choice)
 *  A distraction is generated by the main window when it is moved or resized.
 */
feature 'at' er_t bgfe_distraction( m@* o, m bgfe_frame* initiator, m tp_t* action_type ) { action_type.0 = escalate~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Client data conversion

//----------------------------------------------------------------------------------------------------------------------

/// type conversion to low-level types (o-type is repeated to cover default implementation for low-level types)
feature 'at' er_t bgfe_copy_to_typed( @* o, tp_t o_type, tp_t dst_type, m x_inst* dst ) = x_inst_t_copy_typed( dst, dst_type, o_type, o );

//----------------------------------------------------------------------------------------------------------------------

/// type conversion to low-level types (o-type is repeated to cover default implementation for low-level types)
feature 'at' er_t bgfe_copy_from_typed( m @* o, tp_t o_type, tp_t src_type, x_inst* src ) = x_inst_t_copy_typed( o, o_type, src_type, src );

//----------------------------------------------------------------------------------------------------------------------

/** A glimpse is a short one-line text summarizing or describing the client's content
 *  It is typically displayed on a button opening the client in a dedicated window.
 */
feature 'at' er_t bgfe_get_glimpse( @* o, m st_s* text ); // no overload here because overload state is actively tested in frames

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

