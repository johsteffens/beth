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

/** BTCL: Exportable Objects */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

///// List of operator types ordered in groups and descending priority
//
//// Group A, binary
//name member, frame;
//
//// Group B, unary
//name identity, neg, not, print_compact, print_detailed;
//
//// Group C, unary
//name pow, div, mod, chain, mul_dot_colon, mul_dot, mul_colon;
//name mul, sub, add, spawn, cat;
//name equal, unequal, larger_equal, larger, smaller_equal, smaller;
//name and, or, shift_left, assign;
//
//// Group E, binary
//name continuation;

//----------------------------------------------------------------------------------------------------------------------

/// exportable unary operator
stamp :export_uop_s
{
    tp_t type;

    sr_s a;

    x_source_point_s source_point;

    func o _( m@* o, tp_t type, m sr_s* a, m x_source_point_s* source_point )
    {
        o.type = type;
        o.a.tsm( a.type(), a.o.fork() );
        if( source_point ) o.source_point.copy( source_point );
    }

    /// If reducible: Creates reduced sr, and returns 'true'. Otherwise returns false;
    func er_t get_reduced_sr( m@* o, m :frame_s* frame, m sr_s* result, m bl_t* success );
}

//----------------------------------------------------------------------------------------------------------------------

func (:export_uop_s) get_reduced_sr
{
    success.0 = true;

    switch( o.type )
    {
        case identity~:
        {
            result.tsc( o.a.type(), o.a.o.fork() );
            = 0;
        }
        break;

        case neg~:
        {
            if( o.a.is_integer() ) { result.const_from_s3( -o.a.to_s3() ); = 0; }
            if( o.a.is_float()   ) { result.const_from_f3( -o.a.to_f3() ); = 0; }
        }
        break;

        case not~:
        {
            if( o.a.is_numeric() ) { result.const_from_bl( !o.a.to_bl() ); = 0; }
        }
        break;

        default: break;
    }

    success.0 = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_export_uop_type( m@* o, tp_t type, s2_t priority, m x_source* source, m sr_s* result )
{
    m$* source_point = x_source_point_s!^( source );
    sr_s^ sa; o.eval( priority, source, sa );
    m$* uop = :export_uop_s!^( type, sa, source_point );
    bl_t reduced_success = false;
    uop.get_reduced_sr( o, result, reduced_success );
    if( !reduced_success )
    {
        if( o.is_exportable( type ) )
        {
            result.asc( uop.fork() );
        }
        else
        {
            = source_point.parse_error_fa( "Operator #<sc_t> #<sc_t> is not defined.\n", :operator_symbol( type ), bnameof( sa.o_type() ) );
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// exportable binary operator
stamp :export_bop_s
{
    tp_t type;

    sr_s a; // l-value
    sr_s b; // r-value

    x_source_point_s source_point;

    func o _( m@* o, tp_t type, m sr_s* a, m sr_s* b, m x_source_point_s* source_point )
    {
        o.type = type;
        o.a.tsm( a.type(), a.o.fork() );
        o.b.tsm( b.type(), b.o.fork() );
        if( source_point ) o.source_point.copy( source_point );
    }

    /// If reducible: Creates reduced sr, and returns 'true'. Otherwise returns false;
    func er_t get_reduced_sr( m@* o, m :frame_s* frame, m sr_s* result, m bl_t* success );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_export_bop_type( m@* o, tp_t type, s2_t priority, m x_source* source, m sr_s* sa, m sr_s* result )
{
    m$* source_point = x_source_point_s!^( source );
    sr_s^ sb; o.eval( priority, source, sb );
    m$* bop = :export_bop_s!^( type, sa, sb, source_point );
    bl_t reduced_success = false;
    bop.get_reduced_sr( o, result, reduced_success );
    if( !reduced_success )
    {
        if( o.is_exportable( type ) )
        {
            result.asc( bop.fork() );
        }
        else
        {
            = source_point.parse_error_fa( "Operator #<sc_t> #<sc_t> #<sc_t> is not defined.\n", bnameof( sa.o_type() ), :operator_symbol( type ), bnameof( sb.o_type() ) );
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:export_bop_s) get_reduced_sr
{
    success.0 = true;

    switch( o.type )
    {
        case pow~:
        {
            if( bcore_tp_is_numeric( o.a.type() ) && bcore_tp_is_numeric( o.b.type() ) )
            {
                result.const_from_f3( pow( o.a.to_f3(), o.b.to_f3() ) );
                = 0;
            }
        }
        break;

        case div~:
        {
            if( bcore_tp_is_numeric( o.a.type() ) && bcore_tp_is_numeric( o.b.type() ) )
            {
                if( o.b.to_f3() == 0 ) = o.source_point.parse_error_fa( "Division by zero.\n" );

                if( bcore_tp_is_integer( o.a.type() ) && bcore_tp_is_integer( o.b.type() ) )
                {
                    result.const_from_s3( o.a.to_s3() / o.b.to_s3() );
                }
                else
                {
                    result.const_from_f3( o.a.to_f3() / o.b.to_f3() );
                }
                = 0;
            }
        }
        break;

        case mod~:
        {
            if( bcore_tp_is_integer( o.a.type() ) && bcore_tp_is_integer( o.b.type() ) )
            {
                if( o.b.to_s3() == 0 ) = o.source_point.parse_error_fa( "Modulo division by zero.\n" );
                result.const_from_s3( o.a.to_s3() % o.b.to_s3() );
                = 0;
            }
        }
        break;

        case chain~:
        {
            if( o.a.type() == :function_s~ && o.b.type() == :function_s~ )
            {
                m :function_s* fa = o.a.o.cast( m :function_s* ).fork()^;
                m :function_s* fb = o.b.o.cast( m :function_s* ).fork()^;

                if( !fa.is_unary() ) o.source_point.parse_error_fa( "Operator *: Left argument is not a unary function.\n" );

                m :function_s* fc = :function_s!^.setup( fb.signature, fb.block, fb.tail );
                fc.append_tail( fa );

                result.asm( fc.fork() );
                = 0;
            }
        }
        break;

        case mul~:
        {
            if( bcore_tp_is_numeric( o.a.type() ) && bcore_tp_is_numeric( o.b.type() ) )
            {
                if( bcore_tp_is_integer( o.a.type() ) && bcore_tp_is_integer( o.b.type() ) )
                {
                    result.const_from_s3( o.a.to_s3() * o.b.to_s3() );
                }
                else
                {
                    result.const_from_f3( o.a.to_f3() * o.b.to_f3() );
                }
                = 0;
            }
            else if( o.a.type() == :list_s~ )
            {
                :list_s* list_a = o.a.o.cast( :list_s* );
                if( o.b.type() == :list_s~ )
                {
                    :list_s* list_b = o.b.o.cast( :list_s* );
                    m :list_s* list_r = :list_s!^;
                    list_r.set_size( list_a.size() * list_b.size() );
                    for( sz_t i = 0; i < list_a.size(); i++ )
                    {
                        m sr_s* sa = list_a.arr.[ i ];

                        for( sz_t j = 0; j < list_b.size(); j++ )
                        {
                            m sr_s* sb = list_b.arr.[ j ];
                            frame.bop_cat_ab( sa, sb, list_r.arr.[ i * list_b.size() + j ] );
                        }
                    }
                    result.asc( list_r.fork() );
                    = 0;
                }
            }
            else if( o.a.type() == :function_s~ )
            {
                m :function_s* f = o.a.o.cast( m :function_s* ).fork()^;
                m $* arg_list = bcore_arr_sr_s!^;
                arg_list.push_sr( sr_null() ).fork_from( o.b );
                f.call( o.source_point, frame, arg_list, result );
                = 0;
            }
        }
        break;

        case sub~:
        {
            if( bcore_tp_is_numeric( o.a.type() ) && bcore_tp_is_numeric( o.b.type() ) )
            {
                if( bcore_tp_is_integer( o.a.type() ) && bcore_tp_is_integer( o.b.type() ) )
                {
                    result.const_from_s3( o.a.to_s3() - o.b.to_s3() );
                }
                else
                {
                    result.const_from_f3( o.a.to_f3() - o.b.to_f3() );
                }
                = 0;
            }
        }
        break;

        case add~:
        {
            if( bcore_tp_is_numeric( o.a.type() ) )
            {
                if( bcore_tp_is_numeric( o.b.type() ) )
                {
                    if( bcore_tp_is_integer( o.a.type() ) && bcore_tp_is_integer( o.b.type() ) )
                    {
                        result.const_from_s3( o.a.to_s3() + o.b.to_s3() );
                    }
                    else
                    {
                        result.const_from_f3( o.a.to_f3() + o.b.to_f3() );
                    }
                    = 0;
                }
            }

            if( o.a.type() == st_s~ || o.b.type() == st_s~ )
            {
                m $* st1 = st_s!^;
                m $* st2 = st_s!^;
                if( st1.copy_typed( o.a.type(), o.a.o ) ) { = o.source_point.parse_error_fa( "operator '+': #<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc ); }
                if( st2.copy_typed( o.b.type(), o.b.o ) ) { = o.source_point.parse_error_fa( "operator '+': #<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc ); }
                st1.push_st( st2 );

                result.asc( st1.fork() );
                = 0;
            }
        }
        break;

        case spawn~:
        {
            if( o.a.is_numeric() )
            {
                m :list_s* list = :list_s!^;
                list.arr.set_size( o.a.to_s3() );
                if( o.b.type() == :function_s~ )
                {
                    m :frame_s* local_frame = :frame_s!^.setup( frame );
                    :function_s* function = o.b.o.cast( :function_s* );
                    if( function.args() != 1 ) = o.source_point.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Right operand must be unary (single argument).\n", bnameof( o.a.type() ), bnameof( o.b.type() ) );
                    tp_t arg_name = function.arg_name( 0 );
                    for( sz_t i = 0; i < list.arr.size; i++ )
                    {
                        local_frame.var_set( arg_name, sr_s3( i ) );
                        function.block.eval( local_frame, list.arr.[ i ] );
                    }
                }
                else
                {
                    for( sz_t i = 0; i < list.arr.size; i++ )
                    {
                        list.arr.[ i ].tsc( o.b.type(), x_inst_t_clone( o.b.o, o.b.type() ) );
                    }
                }

                result.asc( list.fork() );

                = 0;
            }
            else if( o.a.type() == :list_s~ )
            {
                :list_s* list_a = o.a.o.cast( :list_s* );

                if( o.b.type() == :function_s~ )
                {
                    m :frame_s* local_frame = :frame_s!^.setup( frame );
                    :function_s* function = o.b.o.cast( :function_s* );

                    if( function.args() == 1 )
                    {
                        m :list_s* list = :list_s!^;
                        list.set_size( list_a.size() );
                        tp_t arg_name = function.arg_name( 0 );
                        for( sz_t i = 0; i < list.arr.size; i++ )
                        {
                            local_frame.var_set( arg_name, sr_cw( list_a.arr.[ i ] ) );
                            function.block.eval( local_frame, list.arr.[ i ] );
                        }
                        result.asc( list.fork() );
                        = 0;
                    }
                    else if( function.args() == 2 )
                    {
                        if( list_a.arr.size < 1 )
                        {
                            = o.source_point.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Left operand must be a list of size >= 1.\n", bnameof( o.a.type() ), bnameof( o.b.type() ) );
                        }

                        m sr_s* sa = sr_s!^;
                        sa.0 = sr_cw( list_a.arr.[ 0 ] );

                        for( sz_t i = 1; i < list_a.arr.size; i++ )
                        {
                            local_frame.var_set( function.arg_name( 0 ), sa );
                            sa.0 = sr_null();
                            local_frame.var_set( function.arg_name( 1 ), sr_cw( list_a.arr.[ i ] ) );
                            function.block.eval( local_frame, sa );
                        }

                        result.fork_from( sa );
                        = 0;
                    }
                    else
                    {
                        = o.source_point.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Right operand must be unary (one argument) or binary (two arguments).\n", bnameof( o.a.type() ), bnameof( o.b.type() ) );
                    }
                }
            }
        }
        break;

        case cat~:            = frame.bop_cat_ab( o.a, o.b, result );
        case equal~:         result.from_bl( 0 == x_compare_t_num_dominant( o.a.type(), o.a.o, o.b.type(), o.b.o ) ); = 0;
        case unequal~:       result.from_bl( 0 != x_compare_t_num_dominant( o.a.type(), o.a.o, o.b.type(), o.b.o ) ); = 0;
        case larger_equal~:  result.from_bl( 0 >= x_compare_t_num_dominant( o.a.type(), o.a.o, o.b.type(), o.b.o ) ); = 0;
        case larger~:        result.from_bl( 0 >  x_compare_t_num_dominant( o.a.type(), o.a.o, o.b.type(), o.b.o ) ); = 0;
        case smaller_equal~: result.from_bl( 0 <= x_compare_t_num_dominant( o.a.type(), o.a.o, o.b.type(), o.b.o ) ); = 0;
        case smaller~:       result.from_bl( 0 <  x_compare_t_num_dominant( o.a.type(), o.a.o, o.b.type(), o.b.o ) ); = 0;

        case and~: if( o.a.is_numeric() && o.b.is_numeric() ) { result.from_bl( o.a.to_bl() && o.b.to_bl() ); = 0; } break;
        case or~:  if( o.a.is_numeric() && o.b.is_numeric() ) { result.from_bl( o.a.to_bl() || o.b.to_bl() ); = 0; } break;

        case shift_left~:
        {
            if( o.a.type() == :function_s~ )
            {
                m :function_s* f = o.a.o.cast( m :function_s* ).fork()^;
                m $* arg_list = bcore_arr_sr_s!^;
                arg_list.push_sr( sr_null() ).fork_from( o.b );
                f.call( o.source_point, frame, arg_list, result );
                = 0;
            }
            else if( o.a.type() == :net_node_s~ )
            {
                m :net_node_s* node = o.a.o.cast( m :net_node_s* ).clone()^;
                node.push_branch( 0, false, o.source_point, o.b );
                result.asc( node.fork() );
                = 0;
            }
        }
        break;

        default: break;
    }

    success.0 = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
