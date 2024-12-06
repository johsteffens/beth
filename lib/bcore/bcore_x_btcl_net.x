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
    hidden x_source_point_s source_point;
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
    hidden x_source_point_s source_point;

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

    /// replaces an existing branch
    func er_t push_or_explace_d( m@* o, d :net_branch_s* branch )
    {
        if( branch.name )
        {
            foreach( m$* e in o )
            {
                if( e.name == branch.name )
                {
                    m$* b = branch^;
                    e.sr.clear();
                    e.sr = b.sr;
                    b.sr = sr_null();
                    = 0;
                }
            }
        }

        o.push_d( branch );
        = 0;
    }

    /// pushes an anonymous by forking sr
    func er_t push_anonymous_fork( m@* o, m sr_s* sr )
    {
        m :net_branch_s* branch = :net_branch_s!^;
        branch.sr.tsm( sr.type(), sr.o.fork() );
        o.push_d( branch.fork() );
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

stamp :net_wire_s
{
    tp_t rack_name;
    tp_t branch_name;
    hidden x_source_point_s source_point;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_net_node_modifier( m@* o, m x_source* source, m sr_s* node_sr )
{
    node_sr.0 = sr_clone( node_sr.0 );
    m$* node = node_sr.o.cast( m :net_node_s* ).fork()^;

    m$* branch = :net_branch_s!^;
    if( source.parse_bl( " #?'.'") )
    {
        if( !:is_identifier( source ) ) = source.parse_error_fa( "Identifier expected.\n" );
        branch.name = bcore_name_enroll( o.nameof( o.get_identifier( source, true ) ) );
        source.parse_fa( " =");
        o.eval( 0, source, branch.sr );
    }
    else if( !source.parse_bl( " #=?')'") )
    {
        o.eval( 0, source, branch.sr );
    }
    else
    {
        branch = NULL;
    }

    if( branch ) node.push_or_explace_d( branch.fork() );

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
