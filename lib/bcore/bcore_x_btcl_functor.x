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
}

//----------------------------------------------------------------------------------------------------------------------

name signal_arg_uop_update_val;

// argument reference acting as unitary operator to be placed in the operator-tree
stamp :arg_uop_s
{
    sz_t index; // index into the arg array
    :arg_val_s -> val; // pointer to value of arg array

    func ::export.is_operator = true;
    func ::export.is_exportable_operand = true;

    func ::export.solve
    {
        success = false;
        = 0;
    }

    func ::export.execute
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


    func ::export.signal
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
    :arg_val_s => val;
    :arg_uop_s => uop;

    func o set_val( m@* o, m sr_s* sr ) o.val!.sr.fork_from( sr );
    func o set_f3 ( m@* o, f3_t v     ) o.val!.sr.from_f3( v );

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

    hidden x_mutex_s call_mutex;
    hidden x_source_point_s source_point;


    func bl_t is_operator( @* o ) = ( o.op_tree && ::export_sr_is_operator( o.op_tree ) );

    func m x_btcl_export* operator( m@* o ) = o.op_tree.o.cast( m x_btcl_export* );

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

    func er_t setup( m@* o, x_source_point_s* source_point, m ::function_s* function, m ::frame_s* lexical_frame )
    {
        o.source_point.copy( source_point );
        sz_t args = function.signature.arg_list.size;
        o.arg_arr.set_size( args );
        m$* arr_sr = bcore_arr_sr_s!^.set_size( args );
        for( sz_t i = 0; i < args; i++ )
        {
            arr_sr.[ i ].asc( o.arg_arr.[ i ].get_uop().fork() );
        }
        function.call( o.source_point, lexical_frame, arr_sr, o.op_tree! );
        = 0;
    }

    func sz_t args( @* o ) = o.arg_arr.size;

    func o set_arg_sr( @* o, sz_t index, m sr_s* sr )
    {
        if( index < 0 || index >= o.args() ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, o.args() - 1 );
        o.arg_arr.[ index ].set_val( sr );
    }

    func o set_arg_f3( @* o, sz_t index, f3_t v ) o.set_arg_sr( index, sr_s!^.from_f3( v ) );

    // for external use (thread safe)
    func er_t call( @* o, m sr_s* result )
    {
        o.cast( m$* ).call_mutex.create_lock()^;
        if( !o.op_tree ) = GERR_fa( "Functor has not been set up." );
        if( ::export_sr_is_operator( o.op_tree ) )
        {
            o.op_tree.o.cast( x_btcl_export* ).execute( result );
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

