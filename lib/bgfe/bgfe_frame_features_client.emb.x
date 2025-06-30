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
/// features for frame-client communications

/** Action types delegate certain type of standard actions actions
 *  - 'approve'     : The client approves by requesting an automatic downsync in case of front-end-modifcation. The frame will perform the downsync and sends a change_confirm where changes occurred.
 *  - 'acknowledge' : The client acknowledges the input but no further action is requested. The client may perform a downsync itself.
 *  - 'escalate'    : The client acknowledges (as above) and asks the frame to escalate the event to the parent (if existing).
 *  - 'escapprove'  : The client acknowledges (as above) and asks the frame to escalate the event to the parent or approve if no escalation is possible.
 *  - 'reject'      : The client rejects the request and asks the frame to restore the front-end to the state before the event was triggered.
 */
name approve;
name escapprove;
name escalate;
name acknowledge;
name reject;

//----------------------------------------------------------------------------------------------------------------------

/// change request to client
feature er_t client_change_request( m@* o, m bgfe_frame* initiator, m tp_t* action_type )
{
    if( o.client() ) o.client().t_bgfe_change_request( o.client_type(), initiator, action_type.1 );
    if( action_type.0 == escalate~ || action_type.0 == escapprove~ )
    {
        if( o.parent() )
        {
            o.parent().client_change_request( initiator, action_type.1 );
        }
        else if( action_type.0 == escapprove~ )
        {
            action_type.0 = approve~;
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// change confirmation to client
name change_confirm; // notify type
feature er_t client_change_confirm( m@* o, m bgfe_frame* initiator, m tp_t* action_type )
{
    if( o.client() ) o.client().t_bgfe_change_confirm( o.client_type(), initiator, action_type.1 );
    if( action_type.0 == escalate~ ) if( o.parent() ) o.parent().client_change_confirm( initiator, action_type.1 );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// button was pressed
feature er_t client_button_clicked( m@* o, m bgfe_frame_button_s* initiator, m tp_t* action_type )
{
    if( o.client() ) o.client().t_bgfe_button_clicked( o.client_type(), initiator, action_type.1 );
    if( action_type.0 == escalate~ || action_type.0 == escapprove~ )
    {
        if( o.parent() )
        {
            o.parent().client_button_clicked( initiator, action_type.1 );
        }
        else if( action_type.0 == escapprove~ )
        {
            action_type.0 = TYPEOF_approve;
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// enter was pressed
feature er_t client_enter_pressed( m@* o, m bgfe_frame_entry_s* initiator, m tp_t* action_type )
{
    if( o.client() ) o.client().t_bgfe_enter_pressed( o.client_type(), initiator, action_type.1 );
    if( action_type.0 == escalate~ || action_type.0 == escapprove~ )
    {
        if( o.parent() )
        {
            o.parent().client_enter_pressed( initiator, action_type.1 );
        }
        else if( action_type.0 == escapprove~ )
        {
            action_type.0 = TYPEOF_approve;
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// close query to client
feature bl_t client_close_ok( m@* o )
{
    = o.client() ? o.client().t_bgfe_close_ok( o.client_type() ) : true;
}

//----------------------------------------------------------------------------------------------------------------------

/// close confirmation to client
feature er_t client_close_confirm( m@* o )
{
    = o.client() ? o.client().t_bgfe_close_confirm( o.client_type() ) : 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// see feature in bgfe_client
feature er_t client_edit_frame_type_recursive( @* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m tp_t* action_type, m tp_t* frame_type )
{
    if( o.client() ) o.client().t_bgfe_edit_frame_type( o.client_type(), content, content_type, content_name, action_type, frame_type );
    if( action_type.0 == escalate~ || action_type.0 == escapprove~ )
    {
        if( o.parent() )
        {
            o.parent().client_edit_frame_type_recursive( content, content_type, content_name, action_type, frame_type );
        }
        else if( action_type.0 == escapprove~ )
        {
            action_type.0 = TYPEOF_approve;
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// see feature in bgfe_client
feature er_t client_edit_frame_type( @* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m tp_t* action_type, m tp_t* frame_type )
{
    if( content ) content.t_bgfe_edit_frame_type( content_type, content, content_type, content_name, action_type, frame_type );
    = o.client_edit_frame_type_recursive( content, content_type, content_name, action_type, frame_type );
}

//----------------------------------------------------------------------------------------------------------------------

/// Allows editing the properties of the frame describing the specified content
feature er_t client_edit_frame_recursive( @* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m tp_t* action_type, m bgfe_frame* frame )
{
    if( o.client() ) o.client().t_bgfe_edit_frame( o.client_type(), content, content_type, content_name, action_type, frame );
    if( action_type.0 == escalate~ || action_type.0 == escapprove~ )
    {
        if( o.parent() )
        {
            o.parent().client_edit_frame_recursive( content, content_type, content_name, action_type, frame );
        }
        else if( action_type.0 == escapprove~ )
        {
            action_type.0 = TYPEOF_approve;
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// see feature in bgfe_client
feature er_t client_edit_frame( @* o, m obliv bgfe_client* content, tp_t content_type, tp_t content_name, m tp_t* action_type, m bgfe_frame* frame )
{
    if( content ) content.t_bgfe_edit_frame( content_type, content, content_type, content_name, action_type, frame );
    = o.client_edit_frame_recursive( content, content_type, content_name, action_type, frame );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
