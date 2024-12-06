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

/** BTCL: Beth text constructive language (interpreter) - Binary Operators */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_member( m@* o, m x_source* source, m sr_s* sr )
{
    if( sr.type() == :net_node_s~ ) = o.eval_net_node_member( source, sr );

    bl_t is_const = sr.is_const();

    /// Identifier
    if( source.parse_bl( " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" ) )
    {
        tp_t name = o.get_identifier( source, true );

        if( o.is_reserved_func( name ) )
        {
            source.parse_fa( " (" );
            o.eval_reserved_func( name, source, sr );
            source.parse_fa( " )" );
        }
        else if( x_stamp_t_exists( sr.o_type(), name ) )
        {
            if( sr.is_strong() ) o.preserve_and_set_weak( sr );
            sr_s sc = x_stamp_t_m_get_sr( sr.o, sr.o_type(), name );

            if( sc.o )
            {
                sr.0 = sc;
            }
            else if( source.parse_bl( " #?([0]=='='&&[1]!='=')" ) )  // identifier with assignment
            {
                sr.asm( :null_member_s!.setup( sr, name ) );
            }
            else
            {
                = source.parse_error_fa( "#<sc_t>.#<sc_t> is NULL (use '=' to assign a value).\n", bnameof( sr.o_type() ), o.nameof( name ) );
            }
        }
        else if( sr.o.cast( :* ).btcl_function_arity( name ) >= 0 )
        {
            s2_t arity = sr.o.cast( :* ).btcl_function_arity( name );
            sr.asm( :function_s!.setup_external_function( name, arity, sr.o.cast( :* ) ) );
        }
        else
        {
            = source.parse_error_fa( "#<sc_t>.#<sc_t> does not exist.\n", bnameof( sr.o_type() ), o.nameof( name ) );
        }
    }
    else if( source.parse_bl( " #?'['" ) ) // array index
    {

        m$* sb = sr_s!^;
        o.eval( 0, source, sb );
        source.parse_fa( " ]" );
        if( sb.is_numeric() )
        {
            if( sr.is_strong() ) o.preserve_and_set_weak( sr );
            s3_t index = sb.to_s3();

            s3_t size = 0;
            sr_s sc = sr_null();

            if( x_array_t_is_array( sr.o_type() ) )
            {
                size = x_array_t_size( sr.o, sr.o_type() );
                if( index <     0 ) = source.parse_error_fa( "Array index '<s3_t>' < 0.\n", index );
                if( index >= size ) = source.parse_error_fa( "Array index '<s3_t>' >= array size '<s3_t>'.\n", index, size );
                sc = x_array_t_m_get_sr( sr.o, sr.o_type(), index );
            }
            else if( sr.o_type() == :list_s~ )
            {
                :list_s* list = sr.o.cast( :list_s* );
                size = list.arr.size;
                if( index <     0 ) = source.parse_error_fa( "Array index '<s3_t>' < 0.\n", index );
                if( index >= size ) = source.parse_error_fa( "Array index '<s3_t>' >= array size '<s3_t>'.\n", index, size );
                sc = sr_cw( list.arr.[ index ] );
                is_const = true;
            }
            else
            {
                = source.parse_error_fa( "#<sc_t> is no array or list.\n", bnameof( sr.o_type() ) );
            }

            if( sc.o )
            {
                sr.0 = sc;
            }
            else if( source.parse_bl( " #?([0]=='='&&[1]!='=')" ) )  // identifier with assignment
            {
                sr.asm( :null_arr_element_s!.setup( sr, index ) );
            }
            else
            {
                = source.parse_error_fa( "#<sc_t>.[#<s3_t>] is NULL (use '=' to assign a value).\n", bnameof( sr.o_type() ), index );
            }
        }
        else
        {
            = source.parse_error_fa( "Numeric index expected.\n" );
        }
    }
    else
    {
        = source.parse_error_fa( "Member name '<literal>' or subscript '[<expr>]' expected." );
    }

    sr.set_const( is_const );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_functional( m@* o, m x_source* source, m sr_s* sr )
{
    if( sr.o_type() == :function_s~ )
    {
        sr.o.cast( m :function_s* ).fork()^.call_via_evaluation( source, o, sr );
    }
    else if( sr.o_type() == :block_s~ )
    {
        = source.parse_error_fa( "Attempt to evaluate a block as function. Join the block with a signature to create a function.\n" );
    }
    else if( sr.o_type() == :net_node_s~ )
    {
        :frame_s!^.setup( o ).eval_net_node_modifier( source, sr );
        :clone_if_weak( sr );
    }
    else
    {
        :frame_s!^.setup( o ).eval_bop_modifier( source, sr );
        :clone_if_weak( sr );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_modifier( m@* o, m x_source* source, m sr_s* sr )
{
    sr.0 = sr_clone( sr.0 );
    sr_s^ sr_weak;
    sr_weak = sr_cw( sr.0 );
    if( source.parse_bl( " #=?'.'") )
    {
        o.eval_bop( 0, source, sr_weak );
    }
    else
    {
        m$* sb = sr_s!^;
        o.eval( 0, source, sb );
        er_t err = :generic_copy( sr, sb );
        if( err ) { = source.parse_error_fa( "#<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc ); }
    }

    sr.o.cast( m x_stamp* ).t_mutated( sr.type() );

    if( source.parse_bl( " #?','" ) ) = o.eval_bop_modifier( source, sr );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_pow( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

    if( bcore_tp_is_numeric( sr.o_type() ) && bcore_tp_is_numeric( sb.o_type() ) )
    {
        sr.const_from_f3( pow( sr.to_f3(), sb.to_f3() ) );
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> / #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_div( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

    if( bcore_tp_is_numeric( sr.o_type() ) && bcore_tp_is_numeric( sb.o_type() ) )
    {
        if( sb.to_f3() == 0 ) = source.parse_error_fa( "Division by zero.\n" );

        if( bcore_tp_is_integer( sr.o_type() ) && bcore_tp_is_integer( sb.o_type() ) )
        {
            sr.const_from_s3( sr.to_s3() / sb.to_s3() );
        }
        else
        {
            sr.const_from_f3( sr.to_f3() / sb.to_f3() );
        }
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> / #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_mod( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

    if( bcore_tp_is_integer( sr.o_type() ) && bcore_tp_is_integer( sb.o_type() ) )
    {
        if( sb.to_s3() == 0 ) = source.parse_error_fa( "Modulo division by zero.\n" );
        sr.const_from_s3( sr.to_s3() % sb.to_s3() );
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> % #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_func_chain( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

    if( sr.type() == :function_s~ && sb.type() == :function_s~ )
    {
        m :function_s* fa = sr.o.cast( m :function_s* ).fork()^;
        m :function_s* fb = sb.o.cast( m :function_s* ).fork()^;

        if( !fa.is_unary() ) source.parse_error_fa( "Operator *: Left argument is not a unary function.\n" );

        m :function_s* fc = :function_s!^.setup( fb.signature, fb.block, fb.tail );
        fc.append_tail( fa );

        sr.asm( fc.fork() );
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_func_list_unfold( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

    if( sr.type() == :function_s~ && sb.type() == :list_s~ )
    {
        m :function_s* f = sr.o.cast( m :function_s* ).fork()^;
        m :list_s* list = sb.o.cast( m :list_s* ).fork()^;
        f.call_via_arg_list( source, o, list, sr );
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_func_list_transform( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

    if( sr.type() == :function_s~ && sb.type() == :list_s~ )
    {
        m :function_s* f = sr.o.cast( m :function_s* ).fork()^;
        if( !f.is_unary() ) = source.parse_error_fa( "Operator '*:' : Function is not unary.\n" );
        m :list_s* list = sb.o.cast( m :list_s* ).fork()^;
        m :list_s* list_r = :list_s!^;
        list_r.set_size( list.size() );
        for( sz_t i = 0; i < list.size(); i++ )
        {
            f.call_unary( source, o, list.arr.[ i ], list_r.arr.[ i ] );
        }

        sr.asm( list_r.fork() );

        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_func_list_unfold_transform( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

    if( sr.type() == :function_s~ && sb.type() == :list_s~ )
    {
        m :function_s* f = sr.o.cast( m :function_s* ).fork()^;
        m :list_s* list = sb.o.cast( m :list_s* ).fork()^;
        m :list_s* list_r = :list_s!^;
        list_r.set_size( list.size() );
        for( sz_t i = 0; i < list.size(); i++ )
        {
            if( list.arr.[ i ].type() != :list_s~ ) = source.parse_error_fa( "Operator '*.:' : List element '#<sz_t>' of type '#<sc_t>' is not a list.\n", i, bnameof( list.arr.[ i ].type() ) );
            f.call_via_arg_list( source, o, list.arr.[ i ].o.cast( m :list_s* ), list_r.arr.[ i ] );
        }

        sr.asm( list_r.fork() );

        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_mul( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

    if( bcore_tp_is_numeric( sr.o_type() ) && bcore_tp_is_numeric( sb.o_type() ) )
    {
        if( bcore_tp_is_integer( sr.o_type() ) && bcore_tp_is_integer( sb.o_type() ) )
        {
            sr.const_from_s3( sr.to_s3() * sb.to_s3() );
        }
        else
        {
            sr.const_from_f3( sr.to_f3() * sb.to_f3() );
        }
        = 0;
    }
    else if( sr.type() == :list_s~ )
    {
        :list_s* list_a = sr.o.cast( :list_s* );
        if( sb.type() == :list_s~ )
        {
            :list_s* list_b = sb.o.cast( :list_s* );
            m :list_s* list_r = :list_s!^;
            list_r.set_size( list_a.size() * list_b.size() );
            for( sz_t i = 0; i < list_a.size(); i++ )
            {
                m sr_s* sa = list_a.arr.[ i ];

                for( sz_t j = 0; j < list_b.size(); j++ )
                {
                    m sr_s* sb = list_b.arr.[ j ];
                    o.eval_bop_join_ab( source, sa, sb, list_r.arr.[ i * list_b.size() + j ] );
                }
            }
            sr.asc( list_r.fork() );
            = 0;
        }
    }
    else if( sr.type() == :function_s~ )
    {
        m :function_s* f = sr.o.cast( m :function_s* ).fork()^;
        m $* arg_list = bcore_arr_sr_s!^;
        arg_list.push_sr( sr_null() ).fork_from( sb );
        f.call( source, o, arg_list, sr );
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_stream_left( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

    if( sr.type() == :function_s~ )
    {
        m :function_s* f = sr.o.cast( m :function_s* ).fork()^;
        m $* arg_list = bcore_arr_sr_s!^;
        arg_list.push_sr( sr_null() ).fork_from( sb );
        f.call( source, o, arg_list, sr );
        = 0;
    }
    else if( sr.type() == :net_node_s~ )
    {
        m :net_node_s* node = sr.o.cast( m :net_node_s* ).clone()^;
        node.push_anonymous_fork( sb );
        sr.asc( node.fork() );
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> << #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_sub( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

    if( bcore_tp_is_numeric( sr.o_type() ) && bcore_tp_is_numeric( sb.o_type() ) )
    {
        if( bcore_tp_is_integer( sr.o_type() ) && bcore_tp_is_integer( sb.o_type() ) )
        {
            sr.const_from_s3( sr.to_s3() - sb.to_s3() );
        }
        else
        {
            sr.const_from_f3( sr.to_f3() - sb.to_f3() );
        }
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> - #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_add( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

    if( bcore_tp_is_numeric( sr.o_type() ) )
    {
        if( bcore_tp_is_numeric( sb.o_type() ) )
        {
            if( bcore_tp_is_integer( sr.o_type() ) && bcore_tp_is_integer( sb.o_type() ) )
            {
                sr.const_from_s3( sr.to_s3() + sb.to_s3() );
            }
            else
            {
                sr.const_from_f3( sr.to_f3() + sb.to_f3() );
            }
            = 0;
        }
    }

    if( sr.o_type() == st_s~ || sb.o_type() == st_s~ )
    {
        m $* st1 = st_s!^;
        m $* st2 = st_s!^;
        if( st1.copy_typed( sr.o_type(), sr.o ) ) { = source.parse_error_fa( "operator '+': #<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc ); }
        if( st2.copy_typed( sb.o_type(), sb.o ) ) { = source.parse_error_fa( "operator '+': #<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc ); }
        st1.push_st( st2 );

        sr.asc( st1.fork() );
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> + #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_spawn( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    m sr_s* sb = sr_s!^; o.eval( bop_priority, source, sb );

    if( sr.is_numeric() )
    {
        m :list_s* list = :list_s!^;
        list.arr.set_size( sr.to_s3() );
        if( sb.type() == :function_s~ )
        {
            m :frame_s* frame = :frame_s!^.setup( o );
            :function_s* function = sb.o.cast( :function_s* );
            if( function.args() != 1 ) = source.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Right operand must be unary (single argument).\n", bnameof( sr.type() ), bnameof( sb.type() ) );
            tp_t arg_name = function.arg_name( 0 );
            for( sz_t i = 0; i < list.arr.size; i++ )
            {
                frame.var_set( arg_name, sr_s3( i ) );
                function.block.eval( frame, list.arr.[ i ] );
            }
        }
        else
        {
            for( sz_t i = 0; i < list.arr.size; i++ )
            {
                list.arr.[ i ].tsc( sb.type(), x_inst_t_clone( sb.o, sb.type() ) );
            }
        }

        sr.asc( list.fork() );
    }
    else if( sr.type() == :list_s~ )
    {
        :list_s* list_a = sr.o.cast( :list_s* );

        if( sb.type() == :function_s~ )
        {
            m :frame_s* frame = :frame_s!^.setup( o );
            :function_s* function = sb.o.cast( :function_s* );

            if( function.args() == 1 )
            {
                m :list_s* list = :list_s!^;
                list.set_size( list_a.size() );
                tp_t arg_name = function.arg_name( 0 );
                for( sz_t i = 0; i < list.arr.size; i++ )
                {
                    frame.var_set( arg_name, sr_cw( list_a.arr.[ i ] ) );
                    function.block.eval( frame, list.arr.[ i ] );
                }
                sr.asc( list.fork() );
            }
            else if( function.args() == 2 )
            {
                if( list_a.arr.size < 1 )
                {
                    = source.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Left operand must be a list of size >= 1.\n", bnameof( sr.type() ), bnameof( sb.type() ) );
                }

                m sr_s* sa = sr_s!^;
                sa.0 = sr_cw( list_a.arr.[ 0 ] );

                for( sz_t i = 1; i < list_a.arr.size; i++ )
                {
                    frame.var_set( function.arg_name( 0 ), sa );
                    sa.0 = sr_null();
                    frame.var_set( function.arg_name( 1 ), sr_cw( list_a.arr.[ i ] ) );
                    function.block.eval( frame, sa );
                }

                sr.fork_from( sa );
            }
            else
            {
                = source.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Right operand must be unary (one argument) or binary (two arguments).\n", bnameof( sr.type() ), bnameof( sb.type() ) );
            }
        }
        else
        {
            = source.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Cannot spawn from these operands.\n", bnameof( sr.type() ), bnameof( sb.type() ) );
        }
    }
    else
    {
        = source.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Cannot spawn from these operands.\n", bnameof( sr.type() ), bnameof( sb.type() ) );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_join_ab( m@* o, m x_source* source, m sr_s* sa, m sr_s* sb, m sr_s* sr )
{
    :clone_if_weak( sa );
    :clone_if_weak( sb );

    if( sa.type() == :signature_s~ && sb.type() == :block_s~ )
    {
        m :signature_s* signature = sa.o.cast( m :signature_s* );
        m :block_s*     block     = sb.o.cast( m :block_s* );
        m :function_s*  function = :function_s!^.setup( signature, block, NULL );
        sr.asc( function.fork() );
    }
    else
    {
        m :list_s* list = :list_s!^;
        if( sa.type() == :list_s~ ) list.push_list_fork( sa.o.cast( m :list_s* ) ); else list.push_fork( sa );
        if( sb.type() == :list_s~ ) list.push_list_fork( sb.o.cast( m :list_s* ) ); else list.push_fork( sb );
        sr.asc( list.fork() );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_join( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    m sr_s* sa = sr_s!^; sa.0 = sr.0; sr.0 = sr_null();
    m sr_s* sb = sr_s!^; o.eval( bop_priority, source, sb );
    = o.eval_bop_join_ab( source, sa, sb, sr );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_equal( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );
    s2_t result = x_compare_t_num_dominant( sr.o_type(), sr.o, sb.o_type(), sb.o );
    sr.from_bl( 0 == result );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_unequal( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );
    s2_t result = x_compare_t_num_dominant( sr.o_type(), sr.o, sb.o_type(), sb.o );
    sr.from_bl( 0 != result );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_larger_equal( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );
    s2_t result = x_compare_t_num_dominant( sr.o_type(), sr.o, sb.o_type(), sb.o );
    sr.from_bl( 0 >= result );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_larger( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );
    s2_t result = x_compare_t_num_dominant( sr.o_type(), sr.o, sb.o_type(), sb.o );
    sr.from_bl( 0 > result );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_smaller_equal( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );
    s2_t result = x_compare_t_num_dominant( sr.o_type(), sr.o, sb.o_type(), sb.o );
    sr.from_bl( 0 <= result );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_smaller( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );
    s2_t result = x_compare_t_num_dominant( sr.o_type(), sr.o, sb.o_type(), sb.o );
    sr.from_bl( 0 < result );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_logic_and( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    if( !sr.is_numeric() ) = source.parse_error_fa( "Logic AND: Left operant must be boolean or numeric.\n" );
    sr_s^ sb; o.eval( bop_priority, source, sb );
    if( !sb.is_numeric() ) = source.parse_error_fa( "Logic AND: Right operant must be boolean or numeric.\n" );

    sr.from_bl( sr.to_bl() && sb.to_bl() );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_logic_or( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    if( !sr.is_numeric() ) = source.parse_error_fa( "Logic AND: Left operant must be boolean or numeric.\n" );
    sr_s^ sb; o.eval( bop_priority, source, sb );
    if( !sb.is_numeric() ) = source.parse_error_fa( "Logic AND: Right operant must be boolean or numeric.\n" );

    sr.from_bl( sr.to_bl() || sb.to_bl() );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_assign( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    if( sr.is_const() ) = source.parse_error_fa( "Assignment to a const object.\n" );

    sr_s^ sb; o.eval( bop_priority, source, sb );

    sb.set_const( true );

    switch( sr.o_type() )
    {
        case :null_variable_s~:
        {
            :clone_if_weak_or_twice_referenced( sb );
            m sr_s* sr1 = o.var_set( sr.o.cast( :null_variable_s* ).tp_name, sr_tsm( sb.o_type(), bcore_fork( sb.o ) ) );
            sr.twc( sr1.o_type(), sr1.o );
        }
        break;

        case :null_member_s~:
        {
            sr_s sr1 = sr.o.cast( m :null_member_s* ).set_sr( sb );
            sr.down();
            sr.0 = sr1;
        }
        break;

        case :null_arr_element_s~:
        {
            sr_s sr1 = sr.o.cast( m :null_arr_element_s* ).set_sr( sb );
            sr.down();
            sr.0 = sr1;
        }
        break;

        default:
        {
            if( sr.o )
            {
                er_t err = :generic_copy( sr, sb );
                if( err ) { = source.parse_error_fa( "operator '=': #<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc ); }
            }
        }
        break;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_continuation( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    // if the last expression in a file, block or frame has a trailing semicolon, the continuation is ignored
    if( source.parse_bl( " #=?([0]=='}'||[0]==')')" ) ) = 0;
    if( source.eos() ) = 0;

    sr.clear();
    o.eval( bop_priority, source, sr );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/** Binary Operators
 *  Binary operators span a binary tree.
 *  bop_priority determines which operator takes the root position for each branch.
 */
func (:frame_s) er_t eval_bop( m@* o, s2_t exit_priority, m x_source* source, m sr_s* obj )
{
    // operators in descending order of priority

    /// priority group a ---------------------

    s2_t bop_priority = :priority_a();

    if( bop_priority <= exit_priority ) = 0;

    while( source.parse_bl( " #?([0]=='.'||[0]=='(')" ) )
    {
        if( source.parse_bl( " #?'.'" ) )
        {
            o.eval_bop_member( source, obj );
        }

        if( source.parse_bl( " #?'('" ) )
        {
            o.eval_bop_functional( source, obj );
            source.parse_fa( " )" );
        }
    }
    bop_priority--;

    /// priority group c ---------------------

    bop_priority = :priority_c();

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'^'" ) ) o.eval_bop_pow( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'/'" ) ) o.eval_bop_div( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'%'" ) ) o.eval_bop_mod( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'**'" ) ) o.eval_bop_func_chain( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'*.:'" ) ) o.eval_bop_func_list_unfold_transform( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'*.'" ) ) o.eval_bop_func_list_unfold( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'*:'" ) ) o.eval_bop_func_list_transform( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'*'" ) ) o.eval_bop_mul( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'-'" ) ) o.eval_bop_sub( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'+'" ) ) o.eval_bop_add( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'::'" ) ) o.eval_bop_spawn( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?':'" ) ) o.eval_bop_join( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'!='" ) ) o.eval_bop_unequal( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'=='" ) ) o.eval_bop_equal( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'>='" ) ) o.eval_bop_larger_equal( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'>'" ) ) o.eval_bop_larger( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'<='" ) ) o.eval_bop_smaller_equal( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?([0]=='<'&&[1]!='<')" ) ) { source.get_char(); o.eval_bop_smaller( bop_priority, source, obj ); }
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'&'" ) ) o.eval_bop_logic_and( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'|'" ) ) o.eval_bop_logic_or( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'<<'" ) ) = o.eval_bop_stream_left( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'='" ) ) o.eval_bop_assign( bop_priority, source, obj );
    bop_priority--;

    /// priority group e ---------------------

    bop_priority = :priority_e();

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?';'" ) ) o.eval_bop_continuation( bop_priority, source, obj );
    bop_priority--;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
