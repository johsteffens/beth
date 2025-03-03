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

stamp :plain_branch_s
{
    tp_t name;
    sr_s sr;
    hidden x_source_point_s => sp;

    /// forking sr (sr can be NULL); reentrant
    func o setup( m@* o, tp_t name, x_source_point_s* sp, m sr_s* sr )
    {
        o.name = name;
        o.sp!.copy( sp );
        o.sr.clear();
        if( sr ) o.sr.tsm( sr.type(), sr.o.fork() );
    }
}

stamp :plain_branch_arr_s x_array { :plain_branch_s []; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :socket_branch_s
{
    tp_t name;
    sr_s sr;
    hidden x_source_point_s => sp;

    /// forking sr (sr can be NULL); reentrant
    func o setup( m@* o, tp_t name, x_source_point_s* sp, m sr_s* sr )
    {
        o.name = name;
        o.sp!.copy( sp );
        o.sr.clear();
        if( sr ) o.sr.tsm( sr.type(), sr.o.fork() );
    }
}

stamp :socket_branch_arr_s x_array { :socket_branch_s []; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// node types
name rack;
name wire;

stamp :node_s
{
    tp_t type; // node type 0 for generic node
    tp_t name;
    tp_t sub_name; // type specific (socket name in case of wire)

    :plain_branch_arr_s  => plain_branch_arr;
    :socket_branch_arr_s => socket_branch_arr;
    hidden x_source_point_s => sp;

    func ::operator.is_exportable_operand = true;

    func o setup( m@* o, tp_t type, tp_t name, x_source_point_s* sp )
    {
        o.type = type;
        o.name = name;
        o.sp!.copy( sp );
        = o;
    }

    func o setup_wire( m@* o, tp_t rack_name, tp_t wire_name, x_source_point_s* sp )
    {
        o.setup( wire~, rack_name, sp );
        o.sub_name = wire_name;
        = o;
    }

    func bl_t plain_branch_exists( m@* o, tp_t branch_name )
    {
        if( !o.plain_branch_arr ) = false;
        foreach( m$* e in o.plain_branch_arr ) if( e.name == branch_name ) = true;
        = false;
    }

    func bl_t socket_branch_exists( m@* o, tp_t branch_name )
    {
        if( !o.socket_branch_arr ) = false;
        foreach( m$* e in o.socket_branch_arr ) if( e.name == branch_name ) = true;
        = false;
    }

    func sz_t plain_branches( @* o ) = o.plain_branch_arr ? o.plain_branch_arr.size : 0;
    func m :plain_branch_s* m_plain_branch_by_name( m@* o, tp_t name ) { foreach( m$* e in o.plain_branch_arr ) if( e.name == name ) = e; = NULL; }
    func c :plain_branch_s* c_plain_branch_by_name( c@* o, tp_t name ) { foreach( c$* e in o.plain_branch_arr ) if( e.name == name ) = e; = NULL; }
    func m :plain_branch_s* m_plain_branch_by_index( m@* o, sz_t index ) = o.plain_branch_arr.[ index ];
    func c :plain_branch_s* c_plain_branch_by_index( c@* o, sz_t index ) = o.plain_branch_arr.[ index ];

    func sz_t socket_branches( @* o ) = o.socket_branch_arr ? o.socket_branch_arr.size : 0;
    func m :socket_branch_s* m_socket_branch_by_name( m@* o, tp_t name ) { foreach( m$* e in o.socket_branch_arr ) if( e.name == name ) = e; = NULL; }
    func c :socket_branch_s* c_socket_branch_by_name( c@* o, tp_t name ) { foreach( c$* e in o.socket_branch_arr ) if( e.name == name ) = e; = NULL; }
    func m :socket_branch_s* m_socket_branch_by_index( m@* o, sz_t index ) = o.socket_branch_arr.[ index ];
    func c :socket_branch_s* c_socket_branch_by_index( c@* o, sz_t index ) = o.socket_branch_arr.[ index ];

    /// pushes a branch by forking sr (sr can be NULL); replace: if name exists, the branch is replaced
    func o push_plain_branch( m@* o, tp_t name, bl_t replace, x_source_point_s* sp, m sr_s* sr )
    {
        m :plain_branch_s* branch = NULL;
        if( replace ) branch = o.m_plain_branch_by_name( name );
        if( !branch ) branch = o.plain_branch_arr!.push();
        branch.setup( name, sp, sr );
        = o;
    }

    /// pushes a branch by forking sr (sr can be NULL); replace: if name exists, the branch is replaced
    func o push_socket_branch( m@* o, tp_t name, bl_t replace, x_source_point_s* sp, m sr_s* sr )
    {
        m :socket_branch_s* branch = NULL;
        if( replace ) branch = o.m_socket_branch_by_name( name );
        if( !branch ) branch = o.socket_branch_arr!.push();
        branch.setup( name, sp, sr );
        = o;
    }
}

//----------------------------------------------------------------------------------------------------------------------

func er_t eval_node_modifier( m ::frame_s* frame, m x_source* source, m sr_s* node_sr )
{
    node_sr.0 = sr_clone( node_sr.0 );
    m$* node = node_sr.o.cast( m :node_s* ).fork()^;

    bl_t do_loop = true;
    while( do_loop )
    {
        m sr_s* branch_sr = sr_s!^;
        x_source_point_s* sp = x_source_point_s!^.setup_from_source( source );

        if( source.parse_bl( " #?'.'") )
        {
            if( !::is_identifier( source ) ) = source.parse_error_fa( "Identifier expected.\n" );
            tp_t branch_name = bcore_name_enroll( frame.nameof( frame.get_identifier( source, true ) ) );
            source.parse_fa( " =" );
            frame.eval( 0, source, branch_sr );

            // convert functions to functors
            if( branch_sr.type() == ::function_s~ )
            {
                m$* functor = x_btcl_functor_s!^;
                functor.setup( x_source_point_s!( source )^, branch_sr.o.cast( m ::function_s* ), frame );
                branch_sr.asm( functor.fork() );
            }

            node.push_plain_branch( branch_name, true, sp, branch_sr );
        }
        else if( source.parse_bl( " #?':'") )
        {
            if( !::is_identifier( source ) ) = source.parse_error_fa( "Identifier expected.\n" );
            tp_t branch_name = bcore_name_enroll( frame.nameof( frame.get_identifier( source, true ) ) );
            source.parse_fa( " =" );
            frame.eval( 0, source, branch_sr );
            node.push_socket_branch( branch_name, true, sp, branch_sr );
        }
        else
        {
            frame.eval( 0, source, branch_sr );

            // convert functions to functors
            if( branch_sr.type() == ::function_s~ )
            {
                m$* functor = x_btcl_functor_s!^;
                functor.setup( x_source_point_s!( source )^, branch_sr.o.cast( m ::function_s* ), frame );
                branch_sr.asm( functor.fork() );
            }

            node.push_plain_branch( 0, false, sp, branch_sr );
        }

        do_loop = false;

        if( source.parse_bl( " #?','"  ) ) do_loop = true;
        if( source.parse_bl( " #=?')'" ) ) do_loop = false;
    }

    node_sr.o.cast( m x_stamp* ).t_mutated( node_sr.type() );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t eval_node_member( m ::frame_s* frame, m x_source* source, m sr_s* sr )
{
    m$* node = sr.o.cast( m :node_s* ).fork()^;

    /// Identifier
    if( !::is_identifier( source ) ) = source.parse_error_fa( "Member name '<literal>' expected." );

    tp_t name = frame.get_identifier( source, true );
    m$* branch = node.m_plain_branch_by_name( name );
    if( !branch ) = source.parse_error_fa( "Branch name '#<sc_t>' not found.", frame.nameof( name ) );

    if( sr.is_strong() ) frame.preserve_and_set_weak( sr );

    sr.tsm( branch.sr.type(), branch.sr.o.fork() );
    sr.set_const( true );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
