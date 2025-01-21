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

/** BTCL: Exportable functor highly optimized for f3 operations.
 *  The functor holds a tree of f3-operators. (see ::operator_f3)
 *  The functor can execute all exportable f3 operators outside outside the x_btcl_frame.
 *  It represents an exportable btcl function.
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

name call;
stamp :s
{
    ::operator_f3_arg_arr_s arg_arr;
    ::operator_f3 => op_tree; // operator tree

    hidden x_source_point_s sp;

    func bcore_inst_call.copy_x
    {
        o.setup_tree();
    }

    func bcore_fp.copy_typed
    {
        switch( type )
        {
            case @~: o.copy( src.cast( @* ) ); break;
            case ::functor_s~: o.from_functor( src.cast( ::functor_s* ) ); break;
            default: GERR_fa( "Cannot convert from #<sc_t>.", bnameof( type ) );
        }
        = 0;
    }

    func bcore_via_call.mutated
    {
        o.setup_tree();
    }

    func er_t setup_tree( m@* o )
    {
        if( o.op_tree )
        {
            o.op_tree.setup();
            o.op_tree.set_args( o.arg_arr );
            o.op_tree.check_consistency();
        }
        = 0;
    }

    func er_t from_functor( m@* o, ::functor_s* functor )
    {
        sz_t args = functor.args();
        o.arg_arr.set_size( args );
        for( sz_t i = 0; i < args; i++ ) o.arg_arr.[ i ].name = functor.arg_arr.[ i ].name;
        o.sp.copy( functor.sp );
        o.op_tree =< ::operator_f3_create_op( functor.op_tree );
        o.setup_tree();
        = 0;
    }

    func er_t setup( m@* o, x_source_point_s* sp, m ::function_s* function, m ::frame_s* lexical_frame )
    {
        m$* functor = ::functor_s!^;
        functor.setup( sp, function, lexical_frame );
        o.from_functor( functor );
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

    func o set_arg_f3( @* o, sz_t index, f3_t v )
    {
        if( !o.op_tree ) ERR_fa( "o.op_tree == NULL. Functor has not been setup." );
        if( index < 0 || index >= o.args() ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, o.args() - 1 );
        o.arg_arr.[ index ].val = v;
    }

    func m f3_t* get_arg_f3_ptr( @* o, sz_t index )
    {
        if( !o.op_tree ) ERR_fa( "o.op_tree == NULL. Functor has not been setup." );
        if( index < 0 || index >= o.args() ) ERR_fa( "index (#<sz_t>) is out of range [0,#<sz_t>]", index, o.args() - 1 );
        = o.arg_arr.[ index ].val.1;
    }

    /// no effect on name mismatch
    func o set_arg_f3_by_name( @* o, tp_t name, f3_t v )
    {
        sz_t index = o.arg_index( name );
        if( index >= 0 ) o.set_arg_f3( index, v );
    }

    /// returns NULL on mismatch
    func m f3_t* get_arg_f3_ptr_by_name( @* o, tp_t name )
    {
        sz_t index = o.arg_index( name );
        if( index >= 0 ) = o.get_arg_f3_ptr( index );
        = NULL;
    }

    func f3_t call( @* o ) { assert( o.op_tree ); = o.op_tree.get(); }
    func f3_t call_to_f3( @* o ) = o.call(); // compatibility to functor_s

    func x_btcl.btcl_function_arity
    {
        switch( name )
        {
            case call~: = o.args();
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
                for( sz_t i = 0; i < o.args(); i++ ) o.set_arg_f3( i, args.[i].to_f3() );
                result.from_f3( o.call() );
            }
            break;

            default: break; // never reached
        }
        = 0;
    }

    func ::.nullary_f3
    {
        ASSERT( o.args() == 0 );
        = o.call();
    }

    func ::.unary_f3
    {
        ASSERT( o.args() == 1 );
        o.set_arg_f3( 0, x );
        = o.call();
    }

    func ::.binary_f3
    {
        ASSERT( o.args() == 2 );
        o.set_arg_f3( 0, x );
        o.set_arg_f3( 1, y );
        = o.call();
    }

    func ::.ternary_f3
    {
        ASSERT( o.args() == 3 );
        o.set_arg_f3( 0, x );
        o.set_arg_f3( 1, y );
        o.set_arg_f3( 2, z );
        = o.call();
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

