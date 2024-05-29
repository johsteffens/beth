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
/// supportive functions

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_member( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
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
        :function_s* function = sr.o.cast( m :function_s* ).fork()^;
        :signature_s* signature = function.signature;

        if( o.depth >= :max_frame_depth() ) = source.parse_error_fa( "Maximum frame depth (#<sz_t>) exceeded. Check for unlimited recursions.\n", :max_frame_depth() );

        m$* frame = :frame_s!^.setup( o );

        frame.var_set( TYPEOF_self, sr_tsm( sr.o_type(), bcore_fork( sr.o ) ) );

        for( sz_t i = 0; i < signature.arg_list.size; i++ )
        {
            if( i > 0 ) source.parse_fa( " ," );
            if( source.parse_bl( " #=?')'" ) ) = source.parse_error_fa( "Function argument expected.\n" );
            m$* sr_arg = sr_s!^;
            o.eval( 0, source, sr_arg );
            frame.var_set( signature.arg_list.[ i ], sr_tsm( sr_arg.o_type(), bcore_fork( sr_arg.o ) ) );
        }

        sr.clear();
        function.block.eval( frame /* parent frame */, sr );
        :clone_if_weak( sr );
    }
    else if( sr.o_type() == :block_s~ )
    {
        = source.parse_error_fa( "Attempt to execute a block as function. Join block with signature to create a function.\n" );
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
        if( sb.o_type() == :list_s~ && x_array_t_is_array( sr.o_type() ) )
        {
            :list_s* list = sb.o.cast( :list_s* );
            x_array_t_clear( sr.o, sr.o_type() );
            for( sz_t i = 0; i < list.arr.size; i++ ) x_array_t_push_sr( sr.o, sr.o_type(), sr_cw( list.arr.[ i ] ) );
        }
        else
        {
            er_t err = x_inst_t_copy_typed( sr.o, sr.o_type(), sb.o_type(), sb.o );
            if( err ) { = source.parse_error_fa( "#<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc ); }
        }
    }
    if( source.parse_bl( " #?','" ) ) = o.eval_bop_modifier( source, sr );
    = 0;
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

    = source.parse_error_fa( "Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
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

func (:frame_s) er_t eval_bop_list_bop( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    m sr_s* sb = sr_s!^; o.eval( bop_priority, source, sb );

    m :list_s* list = :list_s!^;

    if( sr.is_numeric() )
    {
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
    }
    else if( sr.type() == :list_s~ && sb.type() == :function_s~ )
    {
        :list_s* src_list = sr.o.cast( :list_s* );
        list.set_size( src_list.size() );

        m :frame_s* frame = :frame_s!^.setup( o );
        :function_s* function = sb.o.cast( :function_s* );
        if( function.args() != 1 ) = source.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Right operand must be unary (single argument).\n", bnameof( sr.type() ), bnameof( sb.type() ) );
        tp_t arg_name = function.arg_name( 0 );
        for( sz_t i = 0; i < list.arr.size; i++ )
        {
            frame.var_set( arg_name, sr_cw( src_list.arr.[ i ] ) );
            function.block.eval( frame, list.arr.[ i ] );
        }
    }
    else
    {
        = source.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Cannot construct a list from these operands.\n", bnameof( sr.type() ), bnameof( sb.type() ) );
    }

    sr.asc( list.fork() );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_join( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    m sr_s* sb = sr_s!^; o.eval( bop_priority, source, sb );

    :clone_if_weak( sr );
    :clone_if_weak( sb );

    if( sr.type() == :signature_s~ && sb.type() == :block_s~ )
    {
        m :signature_s* signature = sr.o.cast( m :signature_s* );
        m :block_s*     block     = sb.o.cast( m :block_s* );
        m :function_s*  function = :function_s!^.setup( signature, block );
        sr.asc( function.fork() );
    }
    else
    {
        m :list_s* list = :list_s!^;
        if( sr.type() == :list_s~ ) list.push_list_fork( sr.o.cast( m :list_s* ) ); else list.push_fork( sr );
        if( sb.type() == :list_s~ ) list.push_list_fork( sb.o.cast( m :list_s* ) ); else list.push_fork( sb );
        sr.asc( list.fork() );
    }

    = 0;
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

    sr.from_bl( sr.to_bl() && sr.to_bl() );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_logic_or( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    if( !sr.is_numeric() ) = source.parse_error_fa( "Logic AND: Left operant must be boolean or numeric.\n" );
    sr_s^ sb; o.eval( bop_priority, source, sb );
    if( !sb.is_numeric() ) = source.parse_error_fa( "Logic AND: Right operant must be boolean or numeric.\n" );

    sr.from_bl( sr.to_bl() || sr.to_bl() );
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
            // TODO: fork in case sb is strong and sb.o has only one reference.
            m sr_s* sr1 = o.var_set( sr.o.cast( :null_variable_s* ).tp_name, sr_tsm( sb.o_type(), x_inst_t_clone( sb.o, sb.o_type() ) ) );
            sr.twc( sr1.o_type(), sr1.o );
        }
        break;

        case :null_member_s~:
        {
            m :null_member_s* null_member = sr.o.cast( m :null_member_s* );
            sr_s sr1 = null_member.set_sr( sb );
            sr.down();
            sr.0 = sr1;
        }
        break;

        case :null_arr_element_s~:
        {
            m :null_arr_element_s* null_arr_element = sr.o.cast( m :null_arr_element_s* );
            sr_s sr1 = null_arr_element.set_sr( sb );
            sr.down();
            sr.0 = sr1;
        }
        break;

        default:
        {
            if( sr.o )
            {
                if( x_inst_t_copy_typed( sr.o, sr.o_type(), sb.o_type(), sb.o ) )
                {
                    = source.parse_error_fa( "operator '=': #<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc );
                }
            }
        }
        break;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_bop_continuation( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
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
    while( source.parse_bl( " #?'.'" ) ) o.eval_bop_member( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'('" ) ) { o.eval_bop_functional( source, obj ); source.parse_fa( " )" ); }
    bop_priority--;

    /// priority group c ---------------------

    bop_priority = :priority_c();

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'/'" ) ) o.eval_bop_div( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'%'" ) ) o.eval_bop_mod( bop_priority, source, obj );
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
    while( source.parse_bl( " #?'::'" ) ) o.eval_bop_list_bop( bop_priority, source, obj );
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
    while( source.parse_bl( " #?'<'" ) ) o.eval_bop_smaller( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'&'" ) ) o.eval_bop_logic_and( bop_priority, source, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'|'" ) ) o.eval_bop_logic_or( bop_priority, source, obj );
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
