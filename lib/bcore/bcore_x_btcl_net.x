/** Author and Copyright 2024 Johannes Bernhard Steffens
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

/** BTCL: Network Builder
 *  These objects are part of a generic network structure which can be constructed using the btcl syntax.
 *  The resulting network object
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :net_branch_s
{
    $ tp_t name;
    $ sr_s sr;
    hidden x_source_point_s sp;

    /// forking sr (sr can be NULL); reentrant
    func o setup( m@* o,tp_t name, x_source_point_s* sp, m sr_s* sr )
    {
        o.name = name;
        o.sp.copy( sp );
        o.sr.clear();
        if( sr ) o.sr.tsm( sr.type(), sr.o.fork() );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// node types
name rack;
name wire;

stamp :net_node_s x_array
{
    tp_t type; // node type 0 for generic node
    tp_t name;
    :net_branch_s => [];
    hidden x_source_point_s sp;

    func o setup( m@* o, tp_t type, tp_t name, x_source_point_s* sp )
    {
        o.type = type;
        o.name = name;
        o.sp.copy( sp );
        = o;
    }

    func o setup_wire( m@* o, tp_t rack_name, tp_t wire_name, x_source_point_s* sp )
    {
        = o.setup( wire~, rack_name, sp ).push_branch( wire_name, true, sp, NULL );
    }

    func bl_t exists( m@* o, tp_t branch_name )
    {
        foreach( m$* e in o ) if( e.name == branch_name ) = true;
        = false;
    }

    /// returns NULL if not existing
    func m :net_branch_s* get_branch( m@* o, tp_t name )
    {
        foreach( m$* e in o ) if( e.name == name ) = e;
        = NULL;
    }

    /// pushes a branch by forking sr (sr can be NULL); replace: if name exists, the branch is replaced
    func o push_branch( m@* o, tp_t name, bl_t replace, x_source_point_s* sp, m sr_s* sr )
    {
        m :net_branch_s* branch = NULL;
        if( replace ) branch = o.get_branch( name );
        if( !branch ) branch = o.push();
        branch.setup( name, sp, sr );
        = o;
    }

}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_net_node_modifier( m@* o, m x_source* source, m sr_s* node_sr )
{
    node_sr.0 = sr_clone( node_sr.0 );
    m$* node = node_sr.o.cast( m :net_node_s* ).fork()^;

    x_source_point_s* sp = x_source_point_s!^.setup_from_source( source );
    m sr_s* branch_sr = sr_s!^;

    if( source.parse_bl( " #?'.'") )
    {
        if( !:is_identifier( source ) ) = source.parse_error_fa( "Identifier expected.\n" );
        tp_t branch_name = bcore_name_enroll( o.nameof( o.get_identifier( source, true ) ) );
        source.parse_fa( " =");
        o.eval( 0, source, branch_sr );
        node.push_branch( branch_name, true, sp, branch_sr );
    }
    else if( !source.parse_bl( " #=?')'") )
    {
        o.eval( 0, source, branch_sr );
        node.push_branch( 0, false, sp, branch_sr );
    }

    node_sr.o.cast( m x_stamp* ).t_mutated( node_sr.type() );
    if( source.parse_bl( " #?','" ) ) = o.eval_net_node_modifier( source, node_sr );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_net_node_member( m@* o, m x_source* source, m sr_s* sr )
{
    m$* node = sr.o.cast( m :net_node_s* ).fork()^;

    /// Identifier
    if( !:is_identifier( source ) ) = source.parse_error_fa( "Member name '<literal>' expected." );

    tp_t name = o.get_identifier( source, true );
    m$* branch = node.get_branch( name );
    if( !branch ) = source.parse_error_fa( "Branch name '#<sc_t>' not found.", o.nameof( name ) );

    if( sr.is_strong() ) o.preserve_and_set_weak( sr );

    sr.tsm( branch.sr.type(), branch.sr.o.fork() );
    sr.set_const( true );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
