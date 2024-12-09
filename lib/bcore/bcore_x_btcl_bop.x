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

func (:frame_s) er_t eval_bop_func_list_unfold( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr_s^ sb; o.eval( bop_priority, source, sb );

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
            f.call_unary( x_source_point_s!^( source ), o, list.arr.[ i ], list_r.arr.[ i ] );
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
            f.call_via_arg_list( x_source_point_s!^( source ), o, list.arr.[ i ].o.cast( m :list_s* ), list_r.arr.[ i ] );
        }

        sr.asm( list_r.fork() );

        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t bop_cat_ab( m@* o, m sr_s* sa, m sr_s* sb, m sr_s* sr )
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
    while( source.parse_bl( " #?'^'" ) ) o.eval_export_bop_type( pow~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'/'" ) ) o.eval_export_bop_type( div~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'%'" ) ) o.eval_export_bop_type( mod~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'**'" ) ) o.eval_export_bop_type( chain~, bop_priority, source, obj, obj );
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
    while( source.parse_bl( " #?'*'" ) ) o.eval_export_bop_type( mul~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'-'" ) ) o.eval_export_bop_type( sub~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'+'" ) ) o.eval_export_bop_type( add~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'::'" ) ) o.eval_export_bop_type( spawn~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?':'" ) ) o.eval_export_bop_type( cat~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'!='" ) ) o.eval_export_bop_type( unequal~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'=='" ) ) o.eval_export_bop_type( equal~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'>='" ) ) o.eval_export_bop_type( larger_equal~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?([0]=='>'&&[1]!='>')" ) ) { source.get_char(); o.eval_export_bop_type( larger~, bop_priority, source, obj, obj ); }
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'<='" ) ) o.eval_export_bop_type( smaller_equal~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?([0]=='<'&&[1]!='<')" ) ) { source.get_char(); o.eval_export_bop_type( smaller~, bop_priority, source, obj, obj ); }
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'&&'" ) ) o.eval_export_bop_type( and~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'||'" ) ) o.eval_export_bop_type( or~, bop_priority, source, obj, obj );
    bop_priority--;

    if( bop_priority <= exit_priority ) = 0;
    while( source.parse_bl( " #?'<<'" ) ) = o.eval_export_bop_type( shift_left~, bop_priority, source, obj, obj );
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
