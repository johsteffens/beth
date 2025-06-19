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

//----------------------------------------------------------------------------------------------------------------------

/** Notification from frame to client that the GFE received an input to change a variable.
 *  The client can take its own action.
 *  The client can delegate action to the frame in the following ways via action_type:
 *  - 'approve'     : The client approves the input. The frame will perform the downsync. A frame_downsync_confirm is sent in afterwards
 *  - 'acknowledge' : The client acknowledges the input. (acknowledge is default, in case the client overloads the request but does not change action_type )
 *  - 'escalate'    : 'acknowledge' with the frame passing the request to the parent. (escalate is default, in case the client does not overload the request)
 *  - 'escapprove'  : 'acknowledge' with the frame passing the notification to the parent; approve in case of no parent.
 *  - 'reject'      : The client rejects the request. The frame discards the GFE-input and sets the affected control back to the client's value.
 */
feature 'at' er_t frame_downsync_request( m@* o, m bgfe_frame* initiator, m tp_t* action_type ) { action_type.0 = escapprove~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Notification from frame to client that the frame has performed a downsync resulting in a variable change.
 *  The client can take action and also respond in one of the following possible ways via action_type:
 *  - 'acknowledge' : The client acknowledges the change.
 *  - 'escalate'    : 'acknowledge' with the frame passing the notification to the parent.
 */
feature 'at' er_t frame_downsync_confirm( m@* o, m bgfe_frame* initiator, m tp_t* action_type ) { action_type.0 = escalate~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Request by the frame to change a link (pointer) managed by the client (or a child of the client)
 *  Requires approval to actually perform the change.
 *  In case of escalation: Auto-approval in case the frame has no parent.
 */
feature 'at' er_t frame_link_change_request( m@* o, m bgfe_frame* initiator, m tp_t* action_type ) { action_type.0 = escapprove~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Button clicked event.  (clicked = pressed and released in succession)
 *  The client can take action and also respond in one of the following possible ways via action_type:
 *  - 'acknowledge' : The client acknowledges the event.
 *  - 'escalate'    : 'acknowledge' with the frame passing the notification to the parent.
 *  - 'escapprove'  : 'acknowledge' with the frame passing the notification to the parent; approve in case of no parent.
 */
feature 'at' er_t frame_button_clicked( m@* o, m bgfe_frame_button_s* initiator, m tp_t* action_type ) { action_type.0 = escalate~; = 0; }

//----------------------------------------------------------------------------------------------------------------------

/** Broadcast request to all affected clients whether closing is premitted based on a GFE close input.
 *  If any client returns 'false' the close input is ignored.
 */
feature 'at' bl_t frame_close_ok( m@* o ) = true;

//----------------------------------------------------------------------------------------------------------------------

/// Notification from frame to client that the frame was closed
feature 'at' er_t frame_close_confirm( m@* o ) = 0;

//----------------------------------------------------------------------------------------------------------------------

/// type conversion to low-level types (o-type is repeated to cover default implementation for low-level types)
feature 'at' er_t copy_to_typed( @* o, tp_t o_type, tp_t dst_type, m x_inst* dst ) = x_inst_t_copy_typed( dst, dst_type, o_type, o );

//----------------------------------------------------------------------------------------------------------------------

/// type conversion to low-level types (o-type is repeated to cover default implementation for low-level types)
feature 'at' er_t copy_from_typed( m @* o, tp_t o_type, tp_t src_type, x_inst* src ) = x_inst_t_copy_typed( o, o_type, src_type, src );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

