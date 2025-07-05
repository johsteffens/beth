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

 /** General purpose notification framework.
  *
  *  Receiver
  *    - Implements the required feature in group ':receiver'.
  *
  *    As long as senders are alive:
  *    - Calls sender.set_notify...    to all notifications the receiver wants to receive.
  *    - Calls sender.remove_notify... where reception is no longer desired.
  *    - Calls sender.remove_notify_receiver prior to receiver shut down (e.g. in on inst_call.down_e).
  *      (Might be omitted if receiver shuts down the sender as well)
  *
  *  Sender
  *    - Owns :ligand_pool_s
  *    - Implements feature 'ligand_pool'.
  *    - Calls ligand_pool.send for events corresponding to a notify type handled by the sender.
  *
  */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

forward :ligand;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :receiver
{
    /// implement what is needed
    feature er_t notify_receive     ( m@* o, tp_t notify_type );
    feature er_t notify_receive_full( m@* o, tp_t notify_type, ::ligand* ligand, x_inst* sender );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :sender
{
    forward ::ligand_pool_s;
    feature bl_t supports_notify( @* o ) { = false; }
    feature m ::ligand_pool_s* ligand_pool( m@* o ) { ERR_fa( "Not implemented in '#name'. Consider calling feature 'supports_notify' to check beforehand.", o._ ); = NULL; }
    func er_t set_notify       ( m@* o, m ::receiver* receiver, tp_t notify_type ) export = o.ligand_pool().set_notify( receiver, notify_type );
    func er_t set_notify_ligand( m@* o, c ::ligand* ligand                       ) export = o.ligand_pool().set_notify_ligand( ligand );
    func er_t remove_notify_receiver( m@* o, ::receiver* receiver ) export = o.ligand_pool().remove_notify_receiver( receiver );
    func er_t remove_notify_type    ( m@* o, tp_t type           ) export = o.ligand_pool().remove_notify_type( type );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group :ligand
{
    feature er_t send( @* o, x_inst* sender ) = GERR_fa( "Not implemented in '#name'.", o._ );
    feature ::receiver* receiver( @* o ) { ERR_fa( "Not implemented in '#name'.", o._ ); = NULL; }
    feature tp_t notify_type    ( @* o ) { ERR_fa( "Not implemented in '#name'.", o._ ); = 0; }

    stamp :arr_s x_array { : => []; };

    stamp :pool_s
    {
        :arr_s arr;

        func void clear( m@* o ) o.arr.clear();
        func sz_t size ( c@* o ) = o.arr.size;

        func er_t set_notify( m@* o, m ::receiver* receiver, tp_t notify_type )
        {
            m$* ligand = :s!^;
            ligand.receiver = receiver;
            ligand.notify_type = notify_type;
            o.set_notify_ligand( ligand );
            = 0;
        }

        func er_t set_notify_ligand( m@* o, c ::ligand* ligand )
        {
            if( !ligand ) = 0;
            o.arr.push_d( ligand.clone() );
            = 0;
        }

        func er_t remove_notify_receiver( m@* o, ::receiver* receiver )
        {
            for( sz_t i = 0; i < o.arr.size(); i++ ) if( o.arr.[ i ].receiver() == receiver ) o.arr.[ i ] =< NULL;
            o.arr.remove_null_elements();
            = 0;
        }

        func er_t remove_notify_type( m@* o, tp_t type )
        {
            for( sz_t i = 0; i < o.arr.size(); i++ ) if( o.arr.[ i ].notify_type() == type ) o.arr.[ i ] =< NULL;
            o.arr.remove_null_elements();
            = 0;
        }

        func er_t send( @* o, tp_t notify_type, ::sender* sender )
        {
            foreach( $* e in o.arr ) if( e.notify_type() == notify_type ) e.send( sender );
            = 0;
        }
    }

    //------------------------------------------------------------------------------------------------------------------

    stamp :s
    {
        tp_t notify_type;
        hidden ::receiver* receiver;
        func :.receiver = o.receiver;
        func :.notify_type = o.notify_type;

        func :.send
        {
            if( !o.receiver ) = 0;
            if( o.receiver.defines_notify_receive()      ) o.receiver.notify_receive( o.notify_type );
            if( o.receiver.defines_notify_receive_full() ) o.receiver.notify_receive_full( o.notify_type, o, sender );
            = 0;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

