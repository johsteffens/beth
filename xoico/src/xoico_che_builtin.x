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

func (:s) bl_t is_builtin_func( c @* o, tp_t tp_identifier )
{
    switch( tp_identifier )
    {
        case TYPEOF_cast:
        case TYPEOF_scope:
        case TYPEOF_t_scope:
        case TYPEOF_fork:
        case TYPEOF_try:
            return true;

        default:
            return false;
    }
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_builtin
(
    m @* o,
    tp_t tp_builtin,
    m x_source* source,
    c :result* result_expr,
    c xoico_typespec_s* typespec_expr,
    m :result* result_out,
    m xoico_typespec_s* typespec_out
)
{
    switch( tp_builtin )
    {
        case TYPEOF_cast :   return o.trans_builtin_cast(    source, result_expr, typespec_expr, result_out, typespec_out );
        case TYPEOF_scope:   return o.trans_builtin_scope(   source, result_expr, typespec_expr, result_out, typespec_out );
        case TYPEOF_t_scope: return o.trans_builtin_t_scope( source, result_expr, typespec_expr, result_out, typespec_out );
        case TYPEOF_fork:    return o.trans_builtin_fork(    source, result_expr, typespec_expr, result_out, typespec_out );
        case TYPEOF_try:     return o.trans_builtin_try(     source, result_expr, typespec_expr, result_out, typespec_out );
        default: return source.parse_error_fa( "Internal error: Invalid builtin type '#<sc_t>'", ifnameof( tp_builtin ) );
    }
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_builtin_cast
(
    m @* o,
    m x_source* source,
    c :result* result_expr,
    c xoico_typespec_s* typespec_expr,
    m :result* result_out,
    m xoico_typespec_s* typespec_out
)
{
    if( result_expr ) // member call
    {
        source.parse_fa( " ( " );
        result_out.clear();
    }
    else // direct call
    {
        source.parse_fa( "cast ( " );
        m $* result = :result_arr_s!^^;
        m $* typespec = xoico_typespec_s!^^;
        o.trans_expression( source, result, typespec );
        source.parse_fa( " , " );
        typespec_expr = typespec;
        result_expr = result;
    }

    result_out.push_sc( "((" );
    m $* typespec_cast = xoico_typespec_s!^^;

    typespec_cast.parse( o.host, source );

    //o.take_typespec( source, typespec_cast, true );


    if( typespec_cast.type == type_object~ )
    {
        typespec_cast.type = o.host.obj_type();
    }
    else if( typespec_cast.type == type_deduce~ )
    {
        if( !typespec_expr.type )
        {
            return source.parse_error_fa( "Cast-syntax: Deduce requested but expression is intractable." );
        }
        typespec_cast.type = typespec_expr.type;
    }

    if( !o.waive_unknown_type && !o.is_type( typespec_cast.type ) )
    {
        return source.parse_error_fa( "Cast: Unknown type name was used." );
    }

    o.push_typespec( typespec_cast, result_out );
    result_out.push_sc( ")(" );

    if
    (
        typespec_expr.type &&
        ( typespec_expr.type != vc_t~ ) &&
        ( typespec_expr.type != vd_t~ ) &&
        ( typespec_expr.type != sc_t~ ) &&
        ( typespec_expr.type != sd_t~ ) &&
        ( typespec_expr.type != TYPEOF_void )
    )
    {
        o.adapt_expression_indirection( source, typespec_expr, typespec_cast.indirection, result_expr, result_out );
    }
    else
    {
        result_out.push_result_c( result_expr );
    }

    source.parse_fa( " )" );
    result_out.push_sc( "))" );

    if( typespec_out ) typespec_out.copy( typespec_cast );

    if( typespec_cast.indirection > typespec_expr.indirection )
    {
        typespec_out.flag_addressable = false;
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_builtin_scope
(
    m @* o,
    m x_source* source,
    c :result* result_expr,
    c xoico_typespec_s* typespec_expr,
    m :result* result_out,
    m xoico_typespec_s* typespec_out
)
{
    bl_t has_arg = false;
    bl_t closing_bracket = true;

    sz_t level = 0;

    if( result_expr ) // member call
    {
        if( source.parse_bl( "#?'^'" ) )
        {
            level = source.parse_bl( "#?'^'" ) ? 0 : o.block_level;

            if( !o.waive_local_scope_operator_creates_implicit_block )
            {
                if( level > 0 && o.stack_block_get_top_unit().statement_wrapped_as_block )
                {
                    return source.parse_error_fa( "Scope operator '^' creates an implicit block. Please use an explicit block  '{ ... }' or choose a different scope operator to clarify your intention ." );
                }
            }
            closing_bracket = false;
        }
        else
        {
            source.parse_fa( " ( " );
            result_out.clear();
            has_arg = !source.parse_bl( "#=?')'" );
        }
    }
    else // direct call
    {
        source.parse_fa( "scope ( " );
        m $* result = :result_arr_s!^^;
        m $* typespec = xoico_typespec_s!^^;
        o.trans_expression( source, result, typespec );
        typespec_expr = typespec;
        result_expr = result;
        has_arg = source.parse_bl( "#?','" );
    }

    if( typespec_expr.type == 0 ) return source.parse_error_fa( "scope: Expression not tractable." );
    if( typespec_expr.access_class != TYPEOF_discardable ) return source.parse_error_fa( "scope: Expression is not discardable." );
    m xoico_typespec_s* typespec_scope = typespec_expr.clone()^^;
    typespec_scope.access_class = TYPEOF_mutable;

    result_out.push_sc( "((" );

    if( has_arg )
    {
        source.parse_fa( " " );
        tp_t tp_identifier = o.get_identifier( source, true );

        if( o.is_var( tp_identifier ) )
        {
            level = o.stack_var.get_level( tp_identifier );
        }
        else if( tp_identifier == scope_local~ )
        {
            level = o.block_level;
        }
        else if( tp_identifier == scope_func~ )
        {
            level = 0;
        }
        else
        {
            return source.parse_error_fa( "scope: identifier '#<sc_t>' does not represent a variable.", o.nameof( tp_identifier ) );
        }
    }

    if( closing_bracket ) source.parse_fa( " )" );

    if( typespec_scope.indirection != 1 ) return source.parse_error_fa( "scope: Expression's indirection != 1." );
    if( typespec_scope.flag_scope )       return source.parse_error_fa( "scope: Target is already scoped." );

    o.push_typespec( typespec_scope, result_out );

    if( o.is_group( typespec_scope.type ) )
    {
        if( typespec_scope.flag_obliv )
        {
            return source.parse_error_fa( "scope: Expression yields an oblivious typespec of a group. Use t_scope to clarify which type is intended." );
        }
        result_out.push_fa( ")BLM_LEVEL_A_PUSH(#<sz_t>,", level );
        result_out.push_result_c( result_expr );
        result_out.push_sc( "))" );
    }
    else
    {
        result_out.push_fa( ")BLM_LEVEL_T_PUSH(#<sz_t>,#<sc_t>,", level, o.nameof( typespec_scope.type ) );
        result_out.push_result_c( result_expr );
        result_out.push_sc( "))" );
    }

    o.stack_block.adl.[ level ].use_blm = true;

    if( typespec_out )
    {
        typespec_out.copy( typespec_scope );
        typespec_out.flag_scope = true;
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_builtin_t_scope
(
    m @* o,
    m x_source* source,
    c :result* result_expr,
    c xoico_typespec_s* typespec_expr,
    m :result* result_out,
    m xoico_typespec_s* typespec_out
)
{
    bl_t has_arg = false;
    bl_t closing_bracket = true;

    sz_t level = 0;

    m $* result_type_expr = :result_arr_s!^;

    if( result_expr ) // member call
    {
        source.parse_fa( " ( " );
        result_out.clear();
        o.trans_expression( source, result_type_expr, NULL );
        has_arg = source.parse_bl( "#?','" );
    }
    else // direct call
    {
        source.parse_fa( "scope ( " );
        m $* result = :result_arr_s!^^;
        m $* typespec = xoico_typespec_s!^^;
        o.trans_expression( source, result, typespec );
        typespec_expr = typespec;
        result_expr = result;

        source.parse_fa( " , " );
        o.trans_expression( source, result_type_expr, NULL );

        has_arg = source.parse_bl( "#?','" );
    }

    if( typespec_expr.type == 0 ) return source.parse_error_fa( "scope: Expression not tractable." );
    if( typespec_expr.access_class != TYPEOF_discardable ) return source.parse_error_fa( "scope: Expression is not discardable." );
    m xoico_typespec_s* typespec_scope = typespec_expr.clone()^^;
    typespec_scope.access_class = TYPEOF_mutable;

    result_out.push_sc( "((" );

    if( has_arg )
    {
        source.parse_fa( " " );
        tp_t tp_identifier = o.get_identifier( source, true );

        if( o.is_var( tp_identifier ) )
        {
            level = o.stack_var.get_level( tp_identifier );
        }
        else if( tp_identifier == scope_local~ )
        {
            level = o.block_level;
        }
        else if( tp_identifier == scope_func~ )
        {
            level = 0;
        }
        else
        {
            return source.parse_error_fa( "scope: identifier '#<sc_t>' does not represent a variable.", o.nameof( tp_identifier ) );
        }
    }

    if( closing_bracket ) source.parse_fa( " )" );

    if( typespec_scope.indirection != 1 ) return source.parse_error_fa( "scope: Expression's indirection != 1." );
    if( typespec_scope.flag_scope )       return source.parse_error_fa( "scope: Target is already scoped." );

    o.push_typespec( typespec_scope, result_out );

    result_out.push_fa( ")BLM_LEVEL_TV_PUSH(#<sz_t>,", level );
    result_out.push_result_c( result_type_expr );
    result_out.push_fa( "," );
    result_out.push_result_c( result_expr );
    result_out.push_sc( "))" );

    o.stack_block.adl.[ level ].use_blm = true;

    if( typespec_out )
    {
        typespec_out.copy( typespec_scope );
        typespec_out.flag_scope = true;
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_builtin_fork
(
    m @* o,
    m x_source* source,
    c :result* result_expr,
    c xoico_typespec_s* typespec_expr,
    m :result* result_out,
    m xoico_typespec_s* typespec_out
)
{
    if( result_expr ) // member call
    {
        source.parse_fa( " ( " );
        result_out.clear();
    }
    else // direct call
    {
        source.parse_fa( "fork ( " );
        m $* result = :result_arr_s!^;
        m $* typespec = xoico_typespec_s!^^;
        o.trans_expression( source, result, typespec );
        typespec_expr = typespec;
        result_expr = result;
    }

    m xoico_typespec_s* typespec_fork = typespec_expr.clone()^^;
    typespec_fork.access_class = TYPEOF_discardable;

    result_out.push_sc( "((" );

    source.parse_fa( " )" );

    if( typespec_fork.type        == 0 ) return source.parse_error_fa( "Operator 'fork': Expression not tractable." );
    if( typespec_fork.indirection != 1 ) return source.parse_error_fa( "Operator 'fork': Expression's indirection != 1." );

    o.push_typespec( typespec_fork, result_out );
    result_out.push_sc( ")bcore_fork(" );
    result_out.push_result_c( result_expr );
    result_out.push_sc( "))" );

    if( typespec_out ) typespec_out.copy( typespec_fork );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_builtin_try
(
    m @* o,
    m x_source* source,
    c :result* result_expr,
    c xoico_typespec_s* typespec_expr,
    m :result* result_out,
    m xoico_typespec_s* typespec_out
)
{
    if( typespec_out ) typespec_out.reset();

    if( result_expr ) // member call
    {
        source.parse_fa( " ( " );
        result_out.clear();
    }
    else // direct call
    {
        source.parse_fa( "try " );

        if( source.parse_bl( "#=?'{'" ) ) // try block
        {
            o.try_block_level++;
            o.trans_block( source, result_out, false );
            o.try_block_level--;
            return 0;
        }

        source.parse_fa( "( " );
        m $* result = :result_arr_s!^^;
        m $* typespec = xoico_typespec_s!^^;
        o.trans_expression( source, result, typespec );
        typespec_expr = typespec;
        result_expr = result;
    }

    c xoico_typespec_s* typespec_try = typespec_expr;

    source.parse_fa( " ) ;" );

    if( typespec_try.type != 0 )
    {
        if( typespec_try.type != er_t~ ) return source.parse_error_fa( "Operator 'try': Expression must yield er_t." );
        if( typespec_try.indirection != 0    ) return source.parse_error_fa( "Operator 'try': Expression's indirection != 0." );
    }
    else
    {
        // return source.parse_error_fa( "Operator 'try': Expression not tractable." );
    }

    if( o.returns_er_t() )
    {
        result_out.push_sc( "BLM_TRY(" );
    }
    else
    {
        result_out.push_sc( "BLM_TRY_EXIT(" );
    }
    result_out.push_result_c( result_expr );
    result_out.push_sc( ")" );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

