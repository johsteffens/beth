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

/** BTCL: Beth text constructive language (interpreter) - Operators */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

name parse;
name parse_file;

func (:frame_s) er_t eval_op_member( m@* o, m x_source* source, m sr_s* sr )
{
    if( sr.type() == :net_node_s~ ) = :net_eval_node_member( o, source, sr );

    bl_t is_const = sr.is_const();

    /// Identifier
    if( source.parse_bl( " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" ) )
    {
        tp_t name = o.get_identifier( source, true );

        if( o.is_reserved_func( name ) )
        {
            source.parse_fa( " (" );
            o.eval_reserved_func( name, source, true, sr );
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
        else if( x_btcl_t_btcl_function_arity( sr.o, sr.type(), name ) >= 0 )
        {
            s2_t arity = x_btcl_t_btcl_function_arity( sr.o, sr.type(), name );
            bl_t is_mutable = x_btcl_t_btcl_function_mutable( sr.o, sr.type(), name );
            sr.asm( :function_s!.setup_external_function( name, arity, is_mutable, sr.o.cast( :* ) ) );
        }
        else if( name == TYPEOF_parse )
        {
            m$* result = sr_s!^;
            source.parse_fa( " (" );
            if( x_btcl_t_defines_btcl_external_parse( sr.type() ) )
            {
                x_btcl_t_btcl_external_parse( sr.o, sr.type(), source, o, result );
            }
            else if( x_btcl_t_defines_m_btcl_external_parse( sr.type() ) )
            {
                x_btcl_t_m_btcl_external_parse( sr.o.clone()^, sr.type(), source, o, result );
            }
            source.parse_fa( " )" );
            sr.tsm( result.type(), result.o.fork() );
        }
        else if( name == TYPEOF_parse_file )
        {
            m sr_s* sb = sr_s!^;
            source.parse_fa( " (" );
            o.eval( 0, source, sb );
            source.parse_fa( " )" );
            if( sb.type() != st_s~ ) = source.parse_error_fa( "Member function 'parse_file': Expression must evaluate to a string.\n" );
            m st_s* path = st_s!^;
            o.context.get_embedding_file_path( source, sb.o.cast( st_s* ).sc, path );
            m x_source* emb_source = bcore_file_open_source( path.sc )^;
            m$* result = sr_s!^;
            if( x_btcl_t_defines_btcl_external_parse( sr.type() ) )
            {
                x_btcl_t_btcl_external_parse( sr.o, sr.type(), emb_source, o, result );
            }
            else if( x_btcl_t_defines_m_btcl_external_parse( sr.type() ) )
            {
                x_btcl_t_m_btcl_external_parse( sr.o.clone()^, sr.type(), emb_source, o, result );
            }
            emb_source.parse_fa( " " );
            if( !emb_source.eos() ) emb_source.parse_error_fa( "Unexpected expression found.\n" );
            sr.tsm( result.type(), result.o.fork() );
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

func (:frame_s) er_t eval_op_functional( m@* o, m x_source* source, m sr_s* sr )
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
        if( !source.parse_bl( " #=?')'" ) )
        {
            :net_eval_node_modifier( :frame_s!^.setup( o ), source, sr );
        }
        :clone_if_weak( sr );
    }
    else if( sr.o_type() == :functor_s~ )
    {
        if( !source.parse_bl( " #=?')'" ) )
        {
            :functor_eval_modifier( :frame_s!^.setup( o ), source, sr );
        }
        :clone_if_weak( sr );
    }
    else
    {
        if( !source.parse_bl( " #=?')'" ) )
        {
            :frame_s!^.setup( o ).eval_op_modifier( source, sr );
        }
        :clone_if_weak( sr );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_op_modifier( m@* o, m x_source* source, m sr_s* sr )
{
    sr.0 = sr_clone( sr.0 );

    bl_t do_loop = true;
    while( do_loop )
    {
        if( source.parse_bl( " #=?'.'") )
        {
            sr_s sr_weak = sr_cw( sr.0 );
            o.eval_op( 0, source, sr_weak );
        }
        else
        {
            m$* sb = sr_s!^;
            o.eval( 0, source, sb );
            er_t err = o.generic_copy( x_source_point_s!^.setup_from_source( source ), sr, sb );
            if( err ) { = source.parse_error_fa( "#<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc ); }
        }

        do_loop = false;

        if( source.parse_bl( " #?','"  ) ) do_loop = true;
        if( source.parse_bl( " #=?')'" ) ) do_loop = false;
    }

    sr.o.cast( m x_stamp* ).t_mutated( sr.type() );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_op_func_list_unfold( m@* o, s2_t op_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( op_priority, source, sb );

    if( sr.type() == :function_s~ && sb.type() == :list_s~ )
    {
        m :function_s* f = sr.o.cast( m :function_s* ).fork()^;
        m :list_s* list = sb.o.cast( m :list_s* ).fork()^;
        f.call_via_arg_list( x_source_point_s!^( source ), o, list, sr );
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_op_func_list_transform( m@* o, s2_t op_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( op_priority, source, sb );

    if( sr.type() == :function_s~ && sb.type() == :list_s~ )
    {
        m :function_s* f = sr.o.cast( m :function_s* ).fork()^;
        if( !f.is_unary() ) = source.parse_error_fa( "Operator '*:' : Function is not unary.\n" );
        m :list_s* list = sb.o.cast( m :list_s* ).fork()^;
        m :list_s* list_r = :list_s!^;
        list_r.set_size( list.size() );
        for( sz_t i = 0; i < list.size(); i++ )
        {
            f.call_unary( x_source_point_s!^( source ), o, list.arr.[ i ], list_r.arr.[ i ] );
        }

        sr.asm( list_r.fork() );

        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_op_func_list_unfold_transform( m@* o, s2_t op_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( op_priority, source, sb );

    if( sr.type() == :function_s~ && sb.type() == :list_s~ )
    {
        m :function_s* f = sr.o.cast( m :function_s* ).fork()^;
        m :list_s* list = sb.o.cast( m :list_s* ).fork()^;
        m :list_s* list_r = :list_s!^;
        list_r.set_size( list.size() );
        for( sz_t i = 0; i < list.size(); i++ )
        {
            if( list.arr.[ i ].type() != :list_s~ ) = source.parse_error_fa( "Operator '*.:' : List element '#<sz_t>' of type '#<sc_t>' is not a list.\n", i, bnameof( list.arr.[ i ].type() ) );
            f.call_via_arg_list( x_source_point_s!^( source ), o, list.arr.[ i ].o.cast( m :list_s* ), list_r.arr.[ i ] );
        }

        sr.asm( list_r.fork() );

        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func er_t bop_cat_ab( m sr_s* a, m sr_s* b, m sr_s* sr )
{
    :clone_if_weak( a );
    :clone_if_weak( b );
    if( a.type() == :signature_s~ && b.type() == :block_s~ )
    {
        m :signature_s* signature = a.o.cast( m :signature_s* );
        m :block_s*     block     = b.o.cast( m :block_s* );
        m :function_s*  function = :function_s!^.setup( signature, block, NULL );
        sr.asc( function.fork() );
    }
    else
    {
        m :list_s* list = :list_s!^;
        if( a.type() == :list_s~ ) list.push_list_fork( a.o.cast( m :list_s* ) ); else list.push_fork( a );
        if( b.type() == :list_s~ ) list.push_list_fork( b.o.cast( m :list_s* ) ); else list.push_fork( b );
        sr.asc( list.fork() );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_op_assign( m@* o, s2_t op_priority, m x_source* source, m sr_s* sr )
{
    if( sr.is_const() ) = source.parse_error_fa( "Assignment to a const object.\n" );

    sr_s^ sb; o.eval( op_priority, source, sb );

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
            sr_s sr1 = sr.o.cast( m :null_member_s* ).set_sr( o, x_source_point_s!^.setup_from_source( source ), sb );
            sr.down();
            sr.0 = sr1;
        }
        break;

        case :null_arr_element_s~:
        {
            sr_s sr1 = sr.o.cast( m :null_arr_element_s* ).set_sr( o, x_source_point_s!^.setup_from_source( source ), sb );
            sr.down();
            sr.0 = sr1;
        }
        break;

        default:
        {
            if( sr.o )
            {
                er_t err = o.generic_copy( x_source_point_s!^.setup_from_source( source ), sr, sb );
                if( err ) { = source.parse_error_fa( "operator '=': #<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc ); }
            }
        }
        break;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_op_continuation( m@* o, s2_t op_priority, m x_source* source, m sr_s* sr )
{
    // if the last expression in a file, block or frame has a trailing semicolon, the continuation is ignored
    if( source.parse_bl( " #=?([0]=='}'||[0]==')')" ) ) = 0;
    if( source.eos() ) = 0;

    sr.clear();
    o.eval( op_priority, source, sr );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/** Binary Operators
 *  Binary operators span a binary tree.
 *  op_priority determines which operator takes the root position for each branch.
 */
func (:frame_s) er_t eval_op( m@* o, s2_t exit_priority, m x_source* source, m sr_s* obj )
{
    // operators in descending order of priority

    /// priority group a ---------------------

    s2_t op_priority = :priority_a();

    if( op_priority <= exit_priority ) = 0;

    while( source.parse_bl( " #?([0]=='.'||[0]=='(')" ) )
    {
        if( source.parse_bl( " #?'.'" ) )
        {
            o.eval_op_member( source, obj );
        }

        if( source.parse_bl( " #?'('" ) )
        {
            o.eval_op_functional( source, obj );
            source.parse_fa( " )" );
        }
    }
    op_priority--;

    /// priority group c ---------------------

    op_priority = :priority_c();

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'^'" ) ) :operator_eval_bop_type( o, pow~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'/'" ) ) :operator_eval_bop_type( o, div~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'%'" ) ) :operator_eval_bop_type( o, mod~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'**'" ) ) :operator_eval_bop_type( o, chain~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'*.:'" ) ) o.eval_op_func_list_unfold_transform( op_priority, source, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'*.'" ) ) o.eval_op_func_list_unfold( op_priority, source, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'*:'" ) ) o.eval_op_func_list_transform( op_priority, source, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'*'" ) ) :operator_eval_bop_type( o, mul~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'-'" ) ) :operator_eval_bop_type( o, sub~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'+'" ) ) :operator_eval_bop_type( o, add~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'!='" ) ) :operator_eval_bop_type( o, unequal~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'=='" ) ) :operator_eval_bop_type( o, equal~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'>='" ) ) :operator_eval_bop_type( o, larger_equal~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?([0]=='>'&&[1]!='>')" ) ) { source.get_char(); :operator_eval_bop_type( o, larger~, op_priority, source, obj, obj ); }
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'<='" ) ) :operator_eval_bop_type( o, smaller_equal~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?([0]=='<'&&[1]!='<')" ) ) { source.get_char(); :operator_eval_bop_type( o, smaller~, op_priority, source, obj, obj ); }
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'&&'" ) ) :operator_eval_bop_type( o, and~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'||'" ) ) :operator_eval_bop_type( o, or~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    op_priority--; // extra priority decrease to allow conditional reoccurring in a branch
    /*no loop here:*/ if( source.parse_bl( " #?'?'" ) ) :operator_eval_top_type( o, conditional~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'::'" ) ) :operator_eval_bop_type( o, spawn~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?':'" ) ) :operator_eval_bop_type( o, cat~, op_priority, source, obj, obj );
    op_priority--;

    // !!! '<<' evaluates in RL-order !!!
    if( op_priority < /*not '<=' */ exit_priority ) = 0; // op_priority < exit_priority ensures evaluation from right to left
    while( source.parse_bl( " #?'<<'" ) ) :operator_eval_bop_type( o, shift_left~, op_priority, source, obj, obj );
    op_priority--;

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'='" ) ) o.eval_op_assign( op_priority, source, obj );
    op_priority--;

    /// priority group e ---------------------

    op_priority = :priority_e();

    if( op_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?';'" ) ) o.eval_op_continuation( op_priority, source, obj );
    op_priority--;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
