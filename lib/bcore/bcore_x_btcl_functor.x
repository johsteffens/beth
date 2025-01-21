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

/** BTCL: Exportable functor
 *  The functor holds a tree of operators. Typically originating from a btcl function.
 *  The functor can execute all exportable operators outside outside the x_btcl_frame.
 *  It represents an exportable btcl function.
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :arg_val_s
{
    sr_s sr;

    func o set_f3( m@* o, f3_t v ) o.sr.from_f3( v );

    func m f3_t* get_f3_ptr( m@* o, f3_t v_init )
    {
        if( o.sr.type() != f3_t~ ) o.sr.from_f3( v_init );
        = o.sr.o.cast( m f3_t* );
    }
}

//----------------------------------------------------------------------------------------------------------------------

name signal_arg_uop_update_val;

// argument reference acting as unitary operator to be placed in the operator-tree
stamp :arg_uop_s
{
    sz_t index; // index into the arg array
    :arg_val_s -> val; // pointer to value of arg array

    func ::operator.is_operator = true;
    func ::operator.is_exportable_operand = true;

    func ::operator.solve
    {
        success = false;
        = 0;
    }

    func ::operator.execute
    {
        if( o.val )
        {
            result.tsc( o.val.sr.type(), o.val.sr.o.fork() );
        }
        else
        {
            result.clear();
        }
        = 0;
    }

    func ::operator.signal
    {
        if( name == signal_arg_uop_update_val~ )
        {
            ASSERT( arg._ == :arg_arr_s~ );
            m :arg_arr_s* arg_arr = arg.cast( m :arg_arr_s* );
            if( o.index < 0 || o.index >= arg_arr.size ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", o.index, arg_arr.size - 1 );
            o.val =< arg_arr.[ o.index ].val.fork();
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

// copyable functor argument
stamp :arg_s
{
    tp_t name;

    :arg_val_s => val;
    :arg_uop_s => uop;

    func o set_val( m@* o, m sr_s* sr ) o.val!.sr.fork_from( sr );

    func o set_f3( m@* o, f3_t v ) o.val!.set_f3( v );
    func m f3_t* get_f3_ptr( @* o, f3_t v_init ) = o.val!.get_f3_ptr( v_init );

    func o clear_val( m@* o ) o.val!.sr.clear();

    func m :arg_uop_s* get_uop( m@* o )
    {
        o.uop!.val =< o.val!.fork();
        = o.uop;
    }
}

//----------------------------------------------------------------------------------------------------------------------

stamp :arg_arr_s x_array
{
    :arg_s [];

    func o set_size( m@* o, sz_t size )
    {
        o.cast( m x_array* ).set_size( size );
        for( sz_t i = 0; i < size; i++ )
        {
            o.[ i ].get_uop().index = i;
            o.[ i ].set_f3( 0 );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

name call;
stamp :s
{
    :arg_arr_s arg_arr;
    sr_s => op_tree; // operator tree

    hidden x_source_point_s sp;

    func bl_t is_operator( @* o ) = ( o.op_tree && ::operator_sr_is_operator( o.op_tree ) );

    func m x_btcl_operator* operator( m@* o ) = o.op_tree.o.cast( m x_btcl_operator* );

    /// signal to tree
    func o signal( m@* o, tp_t name, m x_inst* arg )
    {
        if( o.is_operator() ) o.operator().signal( name, arg );
    }

    func bcore_inst_call.copy_x
    {
        o.signal( signal_arg_uop_update_val~, o.arg_arr );
    }

    func bcore_via_call.mutated
    {
        o.signal( signal_arg_uop_update_val~, o.arg_arr );
    }

    func er_t setup( m@* o, x_source_point_s* sp, m ::function_s* function, m ::frame_s* lexical_frame )
    {
        o.sp.copy( sp );
        sz_t args = function.signature.arg_list.size;
        o.arg_arr.set_size( args );
        m$* arr_sr = bcore_arr_sr_s!^.set_size( args );
        for( sz_t i = 0; i < args; i++ )
        {
            arr_sr.[ i ].asc( o.arg_arr.[ i ].get_uop().fork() );
            o.arg_arr.[ i ].name = function.signature.arg_name( i );
        }
        function.call( o.sp, lexical_frame, arr_sr, o.op_tree! );
        = 0;
    }

    func sz_t args( @* o ) = o.arg_arr.size;

    func tp_t arg_name( @* o, sz_t index )
    {
        if( index < 0 || index >= o.args() ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, o.args() - 1 );
        = o.arg_arr.[ index ].name;
    }

    /// returns -1 in case name is not an argument name
    func sz_t arg_index( @* o, tp_t name )
    {
        for( sz_t i = 0; i < o.arg_arr.size; i++ ) if( o.arg_name( i ) == name ) = i;
        = -1;
    }

    func bl_t arg_name_exists( @* o, tp_t name ) = o.arg_index( name ) >= 0;

    func o set_arg_sr( @* o, sz_t index, m sr_s* sr )
    {
        if( index < 0 || index >= o.args() ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, o.args() - 1 );
        o.arg_arr.[ index ].set_val( sr );
    }

    /// no effect on name mismatch
    func o set_arg_sr_by_name( @* o, tp_t name, m sr_s* sr )
    {
        sz_t index = o.arg_index( name );
        if( index >= 0 ) o.set_arg_sr( index, sr );
    }

    func o set_arg_f3( @* o, sz_t index, f3_t v ) o.set_arg_sr( index, sr_s!^.from_f3( v ) );

    func m f3_t* get_arg_f3_ptr( @* o, sz_t index, f3_t v_init )
    {
        if( index < 0 || index >= o.args() ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, o.args() - 1 );
        = o.arg_arr.[ index ].get_f3_ptr( v_init );
    }

    /// no effect on name mismatch
    func o set_arg_f3_by_name( @* o, tp_t name, f3_t v )
    {
        sz_t index = o.arg_index( name );
        if( index >= 0 ) o.set_arg_f3( index, v );
    }

    /// returns NULL on mismatch
    func m f3_t* get_arg_f3_ptr_by_name( @* o, tp_t name, f3_t v_init )
    {
        sz_t index = o.arg_index( name );
        if( index >= 0 ) = o.get_arg_f3_ptr( index, v_init );
        = NULL;
    }

    // for external use
    func er_t call( @* o, m sr_s* result )
    {
        if( !o.op_tree ) = EM_ERR_fa( "Functor has not been set up." );
        if( ::operator_sr_is_operator( o.op_tree ) )
        {
            o.op_tree.o.cast( x_btcl_operator* ).execute( result );
        }
        else
        {
            result.tsc( o.op_tree.type(), o.op_tree.o.fork() );
        }
        = 0;
    }

    func f3_t call_to_f3( @* o )
    {
        m$* result = sr_s!^;
        o.call( result );
        = result.to_f3();
    }

    func x_btcl.btcl_function_arity
    {
        switch( name )
        {
            case call~: = o.args(); // add_a accepts one argument
            default:    = -1;
        }
        = -1;
    }

    func x_btcl.btcl_function
    {
        switch( name )
        {
            case call~:
            {
                for( sz_t i = 0; i < o.args(); i++ ) o.set_arg_sr( i, args.[i] );
                o.call( result );
            }
            break;

            default: break; // never reached
        }
        = 0;
    }

    func ::.nullary_f3
    {
        ASSERT( o.args() == 0 );
        = o.call_to_f3();
    }

    func ::.unary_f3
    {
        ASSERT( o.args() == 1 );
        o.set_arg_f3( 0, x );
        = o.call_to_f3();
    }

    func ::.binary_f3
    {
        ASSERT( o.args() == 2 );
        o.set_arg_f3( 0, x );
        o.set_arg_f3( 1, y );
        = o.call_to_f3();
    }

    func ::.ternary_f3
    {
        ASSERT( o.args() == 3 );
        o.set_arg_f3( 0, x );
        o.set_arg_f3( 1, y );
        o.set_arg_f3( 2, z );
        = o.call_to_f3();
    }
}

//----------------------------------------------------------------------------------------------------------------------

func er_t eval_modifier( m ::frame_s* frame, m x_source* source, m sr_s* functor_sr )
{
    functor_sr.0 = sr_clone( functor_sr.0 );
    m$* functor = functor_sr.o.cast( m :s* ).fork()^;

    x_source_point_s* sp = x_source_point_s!^.setup_from_source( source );
    m sr_s* sb = sr_s!^;
    frame.eval( 0, source, sb );

    if( sb.type() != ::function_s~ )
    {
        = source.parse_error_fa( "Cannot convert '#<sc_t>' to a functor. A function was expected.\n", bnameof( sb.type() ) );
    }

    m$* function = sb.o.cast( m ::function_s* );

    functor.setup( sp, function, frame );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

