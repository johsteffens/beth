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
//name identity, neg, not, print, printx;
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

/// indicates if o is an operator
feature 'at' bl_t is_operator( @* o ) = false;
func bl_t sr_is_operator( sr_s* sr ) = sr ? sr.o ? :t_is_operator( sr.o.cast( x_btcl* ), sr.type() ) : false : false;

/// true: if o is used as operand, the operator (together with the operand) should be exported
feature 'at' bl_t is_exportable_operand( @* o ) = false;
func bl_t sr_is_exportable_operand( sr_s* sr ) = :t_is_exportable_operand( sr.o.cast( x_btcl* ), sr.type() );

/// Solves operator according to operator and operand specific criteria
feature er_t solve( m@* o, m ::frame_s* frame, m sr_s* result, m bl_t* success )
{
    = EM_ERR_fa( "'#<sc_t>' is no operator.", bnameof( o._ ) );
}

/// Signal broadcast in operator tree
feature o signal( m@* o, tp_t name, m x_inst* arg )
{
    ERR_fa( "'#<sc_t>' is no operator.", bnameof( o._ ) );
}

/// Executes operator recursively. Typically used in an exported functor.
feature 'at' er_t execute( @* o, m sr_s* result )
{
    = EM_ERR_fa( "'#<sc_t>' is no operator.", bnameof( o._ ) );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// unary operator

//----------------------------------------------------------------------------------------------------------------------

stamp :uop_s
{
    tp_t type;

    sr_s a;

    hidden x_source_point_s sp;

    func :.is_operator = true;
    func :.is_exportable_operand = true;

    func o _( m@* o, tp_t type, m sr_s* a, m x_source_point_s* source_point )
    {
        o.type = type;
        o.a.tsm( a.type(), a.o.fork() );
        if( source_point ) o.sp.copy( source_point );
    }

    func :.solve;
    func :.execute;

    func :.signal
    {
        if( :sr_is_operator( o.a ) ) o.a.o.cast( m:* ).signal( name, arg );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// frameless uop-solve function (covers solve and execute)
func (:uop_s) er_t solve_exportable_a( @* o, sr_s* a, m sr_s* result, m bl_t* success )
{
    success.0 = true;

    switch( o.type )
    {
        case identity~: result.tsc( a.type(), a.o.fork() ); = 0;

        case neg~:
        {
            if( a.is_integer() ) { result.const_from_s3( -a.to_s3() ); = 0; }
            if( a.is_float()   ) { result.const_from_f3( -a.to_f3() ); = 0; }
        }
        break;

        case size~:
        {
            if( a.type() == ::list_s~          ) { result.const_from_s3( a.o.cast( ::list_s* ).arr.size ); = 0; }
            if( x_array_t_is_array( a.type() ) ) { result.const_from_s3( x_array_t_size( a.o, a.type() ) ); = 0; }
        }
        break;

        case not~:   if( a.is_numeric() ) { result.const_from_bl( !a.to_bl() ); = 0; } break;
        case exp~:   if( a.is_numeric() ) { result.const_from_f3( f3_exp(   a.to_f3() ) ); = 0; } break;
        case log~:   if( a.is_numeric() ) { result.const_from_f3( f3_log(   a.to_f3() ) ); = 0; } break;
        case log2~:  if( a.is_numeric() ) { result.const_from_f3( f3_log2(  a.to_f3() ) ); = 0; } break;
        case log10~: if( a.is_numeric() ) { result.const_from_f3( f3_log10( a.to_f3() ) ); = 0; } break;
        case sin~:   if( a.is_numeric() ) { result.const_from_f3( f3_sin(   a.to_f3() ) ); = 0; } break;
        case cos~:   if( a.is_numeric() ) { result.const_from_f3( f3_cos(   a.to_f3() ) ); = 0; } break;
        case tan~:   if( a.is_numeric() ) { result.const_from_f3( f3_tan(   a.to_f3() ) ); = 0; } break;
        case tanh~:  if( a.is_numeric() ) { result.const_from_f3( f3_tanh(  a.to_f3() ) ); = 0; } break;
        case sqrt~:  if( a.is_numeric() ) { result.const_from_f3( f3_sqrt(  a.to_f3() ) ); = 0; } break;
        case sign~:  if( a.is_numeric() ) { result.const_from_s3( f3_sign(  a.to_f3() ) ); = 0; } break;

        case abs~:
        {
            if( a.is_integer() ) { result.const_from_s3( s3_abs( a.to_s3() ) ); = 0; }
            if( a.is_numeric() ) { result.const_from_f3( f3_abs( a.to_f3() ) ); = 0; }
        }
        break;

        case ceil~:  if( a.is_numeric() ) { result.const_from_f3( f3_ceil(  a.to_f3() ) ); = 0; } break;
        case floor~: if( a.is_numeric() ) { result.const_from_f3( f3_floor( a.to_f3() ) ); = 0; } break;

        default: break;
    }

    success.0 = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:uop_s) solve
{
    o.solve_exportable_a( o.a, result, success );
    if( success.0 ) = 0;

    if( :sr_is_exportable_operand( o.a ) )
    {
        result.asc( o.fork() );
        success.0 = true;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:uop_s) execute
{
    sr_s* a = NULL;
    if( :sr_is_operator( o.a ) )
    {
        m sr_s* r = sr_s!^^;
        o.a.o.cast( :* ).execute( r );
        a = r;
    }
    else
    {
        a = o.a.1;
    }

    bl_t success = false;
    o.solve_exportable_a( a, result, success );
    if( success ) = 0;

    = o.sp.parse_error_fa( "Operator #<sc_t> #<sc_t> is not executable.\n", ::operator_symbol( o.type ), bnameof( a.type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

/// postfix: true: result is operand, false: operand has yet to be parsed
func er_t eval_uop_type( m ::frame_s * frame, tp_t type, s2_t priority, m x_source* source, bl_t postfix, m sr_s* result )
{
    m$* source_point = x_source_point_s!^( source );
    m sr_s* sa = sr_s!^;

    if( postfix )
    {
        sa.0 = result.0;
        result.0 = sr_null();
    }
    else
    {
        frame.eval( priority, source, sa );
    }

    m$* uop = :uop_s!^( type, sa, source_point );
    bl_t success = false;
    uop.solve( frame, result, success );
    if( !success )
    {
        = source_point.parse_error_fa( "Unary operator '#<sc_t>' on '#<sc_t>' is not defined.\n", ::operator_symbol( type ), bnameof( sa.o_type() ) );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// binary operator

//----------------------------------------------------------------------------------------------------------------------

stamp :bop_s
{
    tp_t type;

    sr_s a; // l-value
    sr_s b; // r-value

    func :.is_operator = true;
    func :.is_exportable_operand = true;

    hidden x_source_point_s sp;

    func o _( m@* o, tp_t type, m sr_s* a, m sr_s* b, m x_source_point_s* source_point )
    {
        o.type = type;
        o.a.tsm( a.type(), a.o.fork() );
        o.b.tsm( b.type(), b.o.fork() );
        if( source_point ) o.sp.copy( source_point );
    }

    func :.solve;
    func :.execute;

    func :.signal
    {
        if( :sr_is_operator( o.a ) ) o.a.o.cast( m:* ).signal( name, arg );
        if( :sr_is_operator( o.b ) ) o.b.o.cast( m:* ).signal( name, arg );
    }
}

//----------------------------------------------------------------------------------------------------------------------

func er_t eval_bop_type( m ::frame_s* frame, tp_t type, s2_t priority, m x_source* source, m sr_s* sa, m sr_s* result )
{
    m$* source_point = x_source_point_s!^( source );
    sr_s^ sb; frame.eval( priority, source, sb );
    m$* bop = :bop_s!^( type, sa, sb, source_point );
    bl_t success = false;
    bop.solve( frame, result, success );
    if( !success )
    {
        = source_point.parse_error_fa( "Operator '#<sc_t> #<sc_t> #<sc_t>' is not defined.\n", bnameof( sa.o_type() ), ::operator_symbol( type ), bnameof( sb.o_type() ) );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// frameless bop-solve function (covers solve and execute)
func (:bop_s) er_t solve_exportable_a_b( @* o, sr_s* a, sr_s* b, m sr_s* result, m bl_t* success )
{
    success.0 = true;

    switch( o.type )
    {
        case pow~:
        {
            if( bcore_tp_is_numeric( a.type() ) && bcore_tp_is_numeric( b.type() ) )
            {
                result.const_from_f3( pow( a.to_f3(), b.to_f3() ) );
                = 0;
            }
        }
        break;

        case div~:
        {
            if( bcore_tp_is_numeric( a.type() ) && bcore_tp_is_numeric( b.type() ) )
            {
                if( b.to_f3() == 0 ) = o.sp.parse_error_fa( "Division by zero.\n" );

                if( bcore_tp_is_integer( a.type() ) && bcore_tp_is_integer( b.type() ) )
                {
                    result.const_from_s3( a.to_s3() / b.to_s3() );
                }
                else
                {
                    result.const_from_f3( a.to_f3() / b.to_f3() );
                }
                = 0;
            }
        }
        break;

        case mod~:
        {
            if( bcore_tp_is_integer( a.type() ) && bcore_tp_is_integer( b.type() ) )
            {
                if( b.to_s3() == 0 ) = o.sp.parse_error_fa( "Modulo division by zero.\n" );
                result.const_from_s3( a.to_s3() % b.to_s3() );
                = 0;
            }
        }
        break;

        case chain~:
        {
            if( a.type() == ::function_s~ && b.type() == ::function_s~ )
            {
                m ::function_s* fa = a.o.cast( m ::function_s* ).fork()^;
                m ::function_s* fb = b.o.cast( m ::function_s* ).fork()^;

                if( !fa.is_unary() ) o.sp.parse_error_fa( "Operator *: Left argument is not a unary function.\n" );

                m ::function_s* fc = ::function_s!^.setup( fb.signature, fb.block, fb.tail );
                fc.append_tail( fa );

                result.asm( fc.fork() );
                = 0;
            }
        }
        break;

        case mul~:
        {
            if( bcore_tp_is_numeric( a.type() ) && bcore_tp_is_numeric( b.type() ) )
            {
                if( bcore_tp_is_integer( a.type() ) && bcore_tp_is_integer( b.type() ) )
                {
                    result.const_from_s3( a.to_s3() * b.to_s3() );
                }
                else
                {
                    result.const_from_f3( a.to_f3() * b.to_f3() );
                }
                = 0;
            }
            else if( a.type() == ::list_s~ )
            {
                ::list_s* list_a = a.o.cast( ::list_s* );
                if( b.type() == ::list_s~ )
                {
                    ::list_s* list_b = b.o.cast( ::list_s* );
                    m ::list_s* list_r = ::list_s!^;
                    list_r.set_size( list_a.size() * list_b.size() );
                    for( sz_t i = 0; i < list_a.size(); i++ )
                    {
                        m sr_s* sa = list_a.arr.[ i ];

                        for( sz_t j = 0; j < list_b.size(); j++ )
                        {
                            m sr_s* sb = list_b.arr.[ j ];
                            ::bop_cat_ab( sa, sb, list_r.arr.[ i * list_b.size() + j ] );
                        }
                    }
                    result.asc( list_r.fork() );
                    = 0;
                }
            }
        }
        break;

        case sub~:
        {
            if( bcore_tp_is_numeric( a.type() ) && bcore_tp_is_numeric( b.type() ) )
            {
                if( bcore_tp_is_integer( a.type() ) && bcore_tp_is_integer( b.type() ) )
                {
                    result.const_from_s3( a.to_s3() - b.to_s3() );
                }
                else
                {
                    result.const_from_f3( a.to_f3() - b.to_f3() );
                }
                = 0;
            }
        }
        break;

        case add~:
        {
            if( bcore_tp_is_numeric( a.type() ) )
            {
                if( bcore_tp_is_numeric( b.type() ) )
                {
                    if( bcore_tp_is_integer( a.type() ) && bcore_tp_is_integer( b.type() ) )
                    {
                        result.const_from_s3( a.to_s3() + b.to_s3() );
                    }
                    else
                    {
                        result.const_from_f3( a.to_f3() + b.to_f3() );
                    }
                    = 0;
                }
            }

            if( a.type() == st_s~ || b.type() == st_s~ )
            {
                m $* st1 = st_s!^;
                m $* st2 = st_s!^;
                if( st1.copy_typed( a.type(), a.o ) ) { = o.sp.parse_error_fa( "operator '+': #<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc ); }
                if( st2.copy_typed( b.type(), b.o ) ) { = o.sp.parse_error_fa( "operator '+': #<sc_t>\n", bcore_error_pop_all_to_st( st_s!^ ).sc ); }
                st1.push_st( st2 );

                result.asc( st1.fork() );
                = 0;
            }
        }
        break;

        case spawn~:
        {
            if( a.is_numeric() && b.type() != ::function_s~ )
            {
                m ::list_s* list = ::list_s!^;
                list.arr.set_size( a.to_s3() );
                for( sz_t i = 0; i < list.arr.size; i++ )
                {
                    list.arr.[ i ].tsc( b.type(), x_inst_t_clone( b.o, b.type() ) );
                }
                result.asc( list.fork() );
                = 0;
            }
        }
        break;

        case cat~: = ::bop_cat_ab( a.cast( m$* ), b.cast( m$* ), result );

        case equal~:
        {
            if( :sr_is_operator( a ) || :sr_is_operator( b ) ) break;
            result.from_bl( 0 == x_compare_t_num_dominant( a.type(), a.o, b.type(), b.o ) );
            = 0;
        }
        break;

        case unequal~:
        {
            if( :sr_is_operator( a ) || :sr_is_operator( b ) ) break;
            result.from_bl( 0 != x_compare_t_num_dominant( a.type(), a.o, b.type(), b.o ) );
             = 0;
        }
        break;

        case larger_equal~:
        {
            if( :sr_is_operator( a ) || :sr_is_operator( b ) ) break;
            result.from_bl( 0 >= x_compare_t_num_dominant( a.type(), a.o, b.type(), b.o ) );
             = 0;
        }
        break;


        case larger~:
        {
            if( :sr_is_operator( a ) || :sr_is_operator( b ) ) break;
            result.from_bl( 0 >  x_compare_t_num_dominant( a.type(), a.o, b.type(), b.o ) );
             = 0;
        }
        break;

        case smaller_equal~:
        {
            if( :sr_is_operator( a ) || :sr_is_operator( b ) ) break;
            result.from_bl( 0 <= x_compare_t_num_dominant( a.type(), a.o, b.type(), b.o ) );
             = 0;
        }
        break;

        case smaller~:
        {
            if( :sr_is_operator( a ) || :sr_is_operator( b ) ) break;
            result.from_bl( 0 <  x_compare_t_num_dominant( a.type(), a.o, b.type(), b.o ) );
             = 0;
        }
        break;

        case and~: if( a.is_numeric() && b.is_numeric() ) { result.from_bl( a.to_bl() && b.to_bl() ); = 0; } break;
        case or~:  if( a.is_numeric() && b.is_numeric() ) { result.from_bl( a.to_bl() || b.to_bl() ); = 0; } break;

        case shift_left~:
        {
            if( a.type() == ::net_node_s~ )
            {
                m ::net_node_s* node = a.o.cast( m ::net_node_s* ).clone()^;
                node.push_branch( 0, false, o.sp, b.cast( m$* ) );
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

func (:bop_s) solve
{
    // exportable operations
    o.solve_exportable_a_b( o.a, o.b, result, success );
    if( success.0 ) = 0;

    success.0 = true;

    // residual (not exportable) operations
    switch( o.type )
    {
        case mul~:
        {
            if( o.a.type() == ::function_s~ )
            {
                m ::function_s* f = o.a.o.cast( m ::function_s* ).fork()^;
                m $* arg_list = bcore_arr_sr_s!^;
                arg_list.push_sr( sr_null() ).fork_from( o.b );
                f.call( o.sp, frame, arg_list, result );
                = 0;
            }
        }
        break;

        case spawn~:
        {
            if( o.a.is_numeric() )
            {
                m ::list_s* list = ::list_s!^;
                list.arr.set_size( o.a.to_s3() );
                if( o.b.type() == ::function_s~ )
                {
                    m ::frame_s* local_frame = ::frame_s!^.setup( frame );
                    ::function_s* function = o.b.o.cast( ::function_s* );
                    if( function.args() != 1 ) = o.sp.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Right operand must be unary (single argument).\n", bnameof( o.a.type() ), bnameof( o.b.type() ) );
                    tp_t arg_name = function.arg_name( 0 );
                    for( sz_t i = 0; i < list.arr.size; i++ )
                    {
                        local_frame.var_set( arg_name, sr_s3( i ) );
                        function.block.eval( local_frame, list.arr.[ i ] );
                    }
                    result.asc( list.fork() );
                    = 0;
                }
            }
            else if( o.a.type() == ::list_s~ )
            {
                ::list_s* list_a = o.a.o.cast( ::list_s* );

                if( o.b.type() == ::function_s~ )
                {
                    m ::frame_s* local_frame = ::frame_s!^.setup( frame );
                    ::function_s* function = o.b.o.cast( ::function_s* );

                    if( function.args() == 1 )
                    {
                        m ::list_s* list = ::list_s!^;
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
                            = o.sp.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Left operand must be a list of size >= 1.\n", bnameof( o.a.type() ), bnameof( o.b.type() ) );
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
                        = o.sp.parse_error_fa( "Operator #<sc_t> :: #<sc_t>: Right operand must be unary (one argument) or binary (two arguments).\n", bnameof( o.a.type() ), bnameof( o.b.type() ) );
                    }
                }
            }
        }
        break;

        case shift_left~:
        {
            if( o.a.type() == ::function_s~ )
            {
                m ::function_s* f = o.a.o.cast( m ::function_s* ).fork()^;
                m $* arg_list = bcore_arr_sr_s!^;
                arg_list.push_sr( sr_null() ).fork_from( o.b );
                f.call( o.sp, frame, arg_list, result );
                = 0;
            }
        }
        break;

        default: break;
    }

    if( :sr_is_exportable_operand( o.a ) )
    {
        if( :sr_is_exportable_operand( o.b ) ) { result.asc( o.fork() ); = 0; }
        if( o.b.is_numeric() ) { result.asc( o.fork() ); = 0; }
    }

    if( :sr_is_exportable_operand( o.b ) )
    {
        if( o.a.is_numeric() ) { result.asc( o.fork() ); = 0; }
    }

    success.0 = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:bop_s) execute
{
    sr_s* a = NULL;
    sr_s* b = NULL;
    if( :sr_is_operator( o.a ) )
    {
        m sr_s* r = sr_s!^^;
        o.a.o.cast( :* ).execute( r );
        a = r;
    }
    else
    {
        a = o.a.1;
    }

    if( :sr_is_operator( o.b ) )
    {
        m sr_s* r = sr_s!^^;
        o.b.o.cast( :* ).execute( r );
        b = r;
    }
    else
    {
        b = o.b.1;
    }

    bl_t success = false;
    o.solve_exportable_a_b( a, b, result, success );
    if( success ) = 0;

    = o.sp.parse_error_fa( "Operator #<sc_t> #<sc_t> #<sc_t> is not executable.\n", bnameof( a.type() ), ::operator_symbol( o.type ), bnameof( b.type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// ternary operator

//----------------------------------------------------------------------------------------------------------------------

stamp :top_s
{
    tp_t type;

    sr_s a; // l-value
    sr_s b; // m-value
    sr_s c; // r-value

    func :.is_operator = true;
    func :.is_exportable_operand = true;

    hidden x_source_point_s sp;

    func o _( m@* o, tp_t type, m sr_s* a, m sr_s* b, m sr_s* c, m x_source_point_s* source_point )
    {
        o.type = type;
        o.a.tsm( a.type(), a.o.fork() );
        o.b.tsm( b.type(), b.o.fork() );
        o.c.tsm( c.type(), c.o.fork() );
        if( source_point ) o.sp.copy( source_point );
    }

    func :.solve;
    func :.execute;

    func :.signal
    {
        if( :sr_is_operator( o.a ) ) o.a.o.cast( m:* ).signal( name, arg );
        if( :sr_is_operator( o.b ) ) o.b.o.cast( m:* ).signal( name, arg );
        if( :sr_is_operator( o.c ) ) o.c.o.cast( m:* ).signal( name, arg );
    }
}

//----------------------------------------------------------------------------------------------------------------------

func er_t eval_top_type( m ::frame_s* frame, tp_t type, s2_t priority, m x_source* source, m sr_s* sa, m sr_s* result )
{
    m$* source_point = x_source_point_s!^( source );
    sr_s^ sb; frame.eval( priority, source, sb );

    source.parse_fa( " :" );

    sr_s^ sc; frame.eval( priority, source, sc );

    m$* top = :top_s!^( type, sa, sb, sc, source_point );
    bl_t success = false;
    top.solve( frame, result, success );
    if( !success )
    {
        = source_point.parse_error_fa( "Operator '#<sc_t> #<sc_t> #<sc_t> '<div-operator>' #<sc_t>' is not defined.\n", bnameof( sa.type() ), ::operator_symbol( type ), bnameof( sb.type() ), bnameof( sc.type() ) );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// frameless bop-solve function (covers solve and execute)
func (:top_s) er_t solve_exportable_a_b_c( @* o, sr_s* a, sr_s* b, sr_s* c, m sr_s* result, m bl_t* success )
{
    success.0 = true;

    switch( o.type )
    {
        case conditional~:
        {
            if( a.is_numeric() )
            {
                if( a.to_bl() )
                {
                    result.tsc( b.type(), b.o.fork() );
                }
                else
                {
                    result.tsc( c.type(), c.o.fork() );
                }
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

func (:top_s) solve
{
    // exportable operations
    o.solve_exportable_a_b_c( o.a, o.b, o.c, result, success );
    if( success.0 ) = 0;

    success.0 = true;

    // residual (not exportable) operations
    switch( o.type )
    {
        default: break;
    }

    if
    (
           ( :sr_is_exportable_operand( o.a ) || o.a.is_numeric() )
        && ( :sr_is_exportable_operand( o.b ) || o.b.is_numeric() )
        && ( :sr_is_exportable_operand( o.c ) || o.c.is_numeric() )
    )
    {
        result.asc( o.fork() );
        = 0;
    }

    success.0 = false;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:top_s) execute
{
    sr_s* a = NULL;
    sr_s* b = NULL;
    sr_s* c = NULL;
    if( :sr_is_operator( o.a ) )
    {
        m sr_s* r = sr_s!^^;
        o.a.o.cast( :* ).execute( r );
        a = r;
    }
    else
    {
        a = o.a.1;
    }

    if( :sr_is_operator( o.b ) )
    {
        m sr_s* r = sr_s!^^;
        o.b.o.cast( :* ).execute( r );
        b = r;
    }
    else
    {
        b = o.b.1;
    }

    if( :sr_is_operator( o.c ) )
    {
        m sr_s* r = sr_s!^^;
        o.c.o.cast( :* ).execute( r );
        c = r;
    }
    else
    {
        c = o.c.1;
    }

    bl_t success = false;
    o.solve_exportable_a_b_c( a, b, c, result, success );
    if( success ) = 0;

    = o.sp.parse_error_fa( "Operator '#<sc_t> #<sc_t> #<sc_t> '<div-operator>' #<sc_t>' is not executable.\n", bnameof( a.type() ), ::operator_symbol( o.type ), bnameof( b.type() ), bnameof( c.type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


