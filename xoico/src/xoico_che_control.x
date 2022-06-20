/** Author and Copyright 2020 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) bl_t is_control_name( c @* o, tp_t tp_identifier )
{
    switch( tp_identifier )
    {
        case TYPEOF_for:
        case TYPEOF_foreach:
        case TYPEOF_if:
        case TYPEOF_else:
        case TYPEOF_break:
        case TYPEOF_while:
        case TYPEOF_do:
        case TYPEOF_switch:
        case TYPEOF_case:
        case TYPEOF_default:
        case TYPEOF_return:
        case TYPEOF_continue:
        case TYPEOF_goto:
            return true;

        default:
            return false;
    }
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_control( m @* o, tp_t tp_control, m x_source* source, m :result* result )
{
    switch( tp_control )
    {
        case TYPEOF_for:     return o.trans_control_for(     source, result );
        case TYPEOF_foreach: return o.trans_control_foreach( source, result );
        case TYPEOF_if:      return o.trans_control_if(      source, result );
        case TYPEOF_else:    return o.trans_control_else(    source, result );
        case TYPEOF_break:   return o.trans_control_break(   source, result );
        case TYPEOF_while:   return o.trans_control_while(   source, result );
        case TYPEOF_do:      return o.trans_control_do(      source, result );
        case TYPEOF_switch:  return o.trans_control_switch(  source, result );
        case TYPEOF_case:    return o.trans_control_case(    source, result );
        case TYPEOF_default: return o.trans_control_default( source, result );
        case TYPEOF_return:     return o.trans_control_return( tp_control, source, result );
        case TYPEOF_completion: return o.trans_control_return( tp_control, source, result );

        // unsupported controls
        case TYPEOF_goto:
        case TYPEOF_continue:
            return source.parse_error_fa( "Control statement '#<sc_t>' is not supported.", ifnameof( tp_control ) );

        default:
            return source.parse_error_fa( "Internal error: Invalid control name '#<sc_t>'", ifnameof( tp_control ) );
    }
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_control_for( m @* o, m x_source* source, m :result* result )
{
    o.inc_block();
    o.stack_block_get_top_unit().break_ledge = true;
    o.trans( source, "for", result );
    o.trans_whitespace( source, result );
    o.trans( source, "(", result );
    if( !source.parse_bl( " #=?';'" ) ) o.trans_statement( source, result ); // def
    o.trans( source, ";", result );
    o.trans_expression( source, result, NULL ); // cond
    o.trans( source, ";", result );
    o.trans_expression( source, result, NULL ); // update
    o.trans_whitespace( source, result );
    o.trans( source, ")", result );
    o.trans_whitespace( source, result );
    if( source.parse_bl( "#=?'{'" ) )
    {
        o.trans_block( source, result, false );
    }
    else
    {
        o.trans_statement_as_block( source, result, false );
    }
    o.dec_block();
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/** foreach( <typespec> var in <arr_expr>[..<sub_element_path>] [; <condition> ] ) <foreach-statement>
 *  -->
 * { <typespec> __a = (match)<arr_expr>; for( sz_t __i = 0; __i < __a->size; __i++ ) [if( <condition> ) ] { <typespec> var = (match)__a->data[ __i ][.element_path]; <foreach-statement> }  }
 */
func (:s) er_t trans_control_foreach( m @* o, m x_source* source, m :result* result )
{
    o.inc_block();
    source.parse_fa( "foreach ( " );

    m xoico_typespec_s* typespec_var = scope( xoico_typespec_s! );
    o.take_typespec( source, typespec_var, true );

    tp_t tp_var_name = 0;

    if( source.parse_bl( "#?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" ) )
    {
        tp_var_name = o.get_identifier( source, true );
    }
    else
    {
        return source.parse_error_fa( "Variable name expected." );
    }

    source.parse_fa( " in " );

    m xoico_typespec_s* typespec_arr_expr = xoico_typespec_s!^;
    m $* result_arr_expr = :result_arr_s!^;
    o.trans_expression( source, result_arr_expr, typespec_arr_expr );

    if( !typespec_arr_expr.type )
    {
        return source.parse_error_fa( "Array expression not tractable." );
    }

    m xoico_compiler_element_info_s* array_element_info = xoico_compiler_element_info_s!^;

    if( !o.compiler.get_type_array_element_info( typespec_arr_expr.type, array_element_info ) )
    {
        return source.parse_error_fa( "Expression does not evaluate to an array." );
    }

    c xoico_typespec_s* typespec_array_element = array_element_info.type_info.typespec;
    m xoico_typespec_s* typespec_access_element = xoico_typespec_s!^;

    m $* result_element_expr = :result_create_from_sc( "__a->data[__i]" )^;

    /// sub-element
    if( source.parse_bl( " #=?'..'" ) )
    {
        source.parse_fa( "." );
        o.trans_typespec_expression( source, result_element_expr, typespec_array_element, typespec_access_element );
    }
    else
    {
        typespec_access_element.copy( typespec_array_element );
    }

    if( typespec_var.type == type_deduce~ ) typespec_var.type = typespec_access_element.type;

    m :result* condition_expr = NULL;

    m xoico_typespec_s* typespec_arr = scope( typespec_arr_expr.clone() );
    typespec_arr.indirection = 1;
    typespec_arr.access_class = TYPEOF_const;

    m xoico_typespec_s* typespec_idx = scope( xoico_typespec_s! );
    typespec_idx.type = sz_t~;

    o.push_typedecl( typespec_var, tp_var_name );
    o.push_typedecl( typespec_arr, o.entypeof( "__a" ) );
    o.push_typedecl( typespec_idx, o.entypeof( "__i" ) );

    /// condition
    if( source.parse_bl( " #=?';'" ) )
    {
        source.parse_fa( " ; " );
        condition_expr = :result_arr_s!^^;
        o.trans_expression( source, condition_expr, NULL );
    }

    source.parse_fa( " )" );

    m $* result_statement = :result_arr_s!^;
    o.inc_block();
    o.stack_block_get_top_unit().break_ledge = true;
    if( source.parse_bl( "#=?'{'" ) )
    {
        o.trans_block( source, result_statement, false );
    }
    else
    {
        o.trans_statement_as_block( source, result_statement, false );
    }
    o.dec_block();

    result.push_fa( "{" );
    d$* blm_init = :result_blm_init_s!( o.block_level );
    result.push_result_d( blm_init );

    o.push_typespec( typespec_arr, result );

    result.push_fa( " __a=" );
    o.adapt_expression( source, typespec_arr_expr, typespec_arr, result_arr_expr, result );
    result.push_fa( ";" );

    result.push_fa( "if(__a)for(sz_t __i=0;__i<__a->size;__i++){" );
    o.push_typespec( typespec_var, result );
    result.push_fa( " #<sc_t>=", xoico_che_s_nameof( o, tp_var_name ) );

    o.adapt_expression( source, typespec_access_element, typespec_var, result_element_expr, result );
    result.push_fa( ";" );

    if( condition_expr )
    {
        result.push_sc( "if(" );
        result.push_result_d( condition_expr.fork() );
        result.push_sc( "){" );
        result.push_result_d( result_statement.fork() );
        result.push_sc( "}" );
    }
    else
    {
        result.push_result_d( result_statement.fork() );
    }

    result.push_fa( "}" );

    d$* blm_down = :result_blm_down_s!;

    if( !o.stack_block_get_top_unit().use_blm )
    {
        blm_init.deactivate();
        blm_down.deactivate();
    }

    result.push_result_d( blm_down );
    result.push_fa( "}" );
    o.dec_block();
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_control_if( m @* o, m x_source* source, m :result* result )
{
    o.trans( source, "if", result );
    o.trans_whitespace( source, result );
    o.trans( source, "(", result );
    o.trans_expression( source, result, NULL ); // def
    o.trans( source, ")", result );
    o.trans_whitespace( source, result );
    if( source.parse_bl( "#=?'{'" ) )
    {
        o.trans_block( source, result, false );
    }
    else
    {
        o.trans_statement_as_block( source, result, false );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_control_while( m @* o, m x_source* source, m :result* result )
{
    o.trans( source, "while", result );
    o.trans_whitespace( source, result );
    o.trans( source, "(", result );
    o.trans_expression( source, result, NULL ); // def
    o.trans( source, ")", result );
    o.trans_whitespace( source, result );
    if( source.parse_bl( "#=?'{'" ) )
    {
        o.trans_block( source, result, true );
    }
    else
    {
        o.trans_statement_as_block( source, result, true );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_control_do( m @* o, m x_source* source, m :result* result )
{
    o.trans( source, "do", result );
    o.trans_whitespace( source, result );
    if( source.parse_bl( "#=?'{'" ) )
    {
        o.trans_block( source, result, true );
    }
    else
    {
        o.trans_statement_as_block( source, result, true );
    }
    o.trans( source, "while", result );
    o.trans_whitespace( source, result );
    o.trans( source, "(", result );
    o.trans_expression( source, result, NULL ); // def
    o.trans( source, ")", result );
    o.trans_whitespace( source, result );
    o.trans( source, ";", result );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_control_else( m @* o, m x_source* source, m :result* result )
{
    o.trans( source, "else", result );
    o.trans_whitespace( source, result );
    if( source.parse_bl( "#=?'{'" ) )
    {
        o.trans_block( source, result, false );
    }
    else
    {
        o.trans_statement_as_block( source, result, false );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_control_switch( m @* o, m x_source* source, m :result* result )
{
    o.trans( source, "switch", result );
    o.trans_whitespace( source, result );
    o.trans( source, "(", result );
    o.trans_expression( source, result, NULL ); // def
    o.trans( source, ")", result );
    o.trans_whitespace( source, result );
    o.trans_block( source, result, true );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_control_case( m @* o, m x_source* source, m :result* result )
{
    o.trans( source, "case", result );
    o.trans_expression( source, result, NULL );
    o.trans_whitespace( source, result );
    o.trans( source, ":", result );
    o.trans_whitespace( source, result );
    o.trans_statement_as_block( source, result, false );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_control_default( m @* o, m x_source* source, m :result* result )
{
    o.trans( source, "default", result );
    o.trans_whitespace( source, result );
    o.trans( source, ":", result );
    o.trans_whitespace( source, result );
    o.trans_statement_as_block( source, result, false );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_control_break( m @* o, m x_source* source, m :result* result )
{
    source.parse_fa( "break ;" );

    sz_t ledge_level = -1;
    bl_t use_blm = false;

    for( sz_t i = o.stack_block.adl.size - 1; i >= 0; i-- )
    {
        c xoico_che_stack_block_unit_s* unit = o.stack_block.adl.[ i ];
        use_blm = use_blm || unit.use_blm;
        if( unit.break_ledge )
        {
            ledge_level = unit.level;
            break;
        }
    }

    if( ledge_level == -1 ) return source.parse_error_fa( "'break' has no ledge." );

    result.push_result_d( :result_break_s!( ledge_level ) );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_control_return( m @* o, tp_t tp_control, m x_source* source, m :result* result )
{
    o.has_completion = true;

    if( tp_control == TYPEOF_return ) source.parse_fa( "return" );
    if( tp_control == TYPEOF_completion ) source.parse_fa( "=" );

    m $* result_expr = :result_arr_s!^;

    m xoico_typespec_s* typespec_expr = scope( xoico_typespec_s! );
    c xoico_typespec_s* typespec_ret = o.signature.typespec_ret;

    o.trans_expression( source, result_expr, typespec_expr );

    if( typespec_expr.type )
    {
        if( !o.returns_a_value() )
        {
            return source.parse_error_fa( "Function does not return a value." );
        }
    }

    o.trans_whitespace( source, result_expr );

    if( !source.parse_bl( "#?';'" ) ) return source.parse_error_fa( "Missing ';' after completion statement." );

    m $* result_expr_adapted = :result_arr_s!^;
    if( o.returns_a_value() && typespec_expr.type )
    {
        if( typespec_expr.access_class != typespec_ret.access_class )
        {
            if( typespec_expr.access_class == TYPEOF_discardable )
            {
                return source.parse_error_fa( "return: Conversion discards typespec 'discardable'." );
            }

            if( typespec_ret.access_class == TYPEOF_discardable )
            {
                return source.parse_error_fa( "return: Conversion requires typespec 'discardable'." );
            }
        }

        o.adapt_expression( source, typespec_expr, typespec_ret, result_expr, result_expr_adapted );
    }
    else
    {
        result_expr_adapted.push_result_d( result_expr.fork() );
    }

    result.push_result_d( :result_return_s!( o, result_expr_adapted.fork() ) );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

