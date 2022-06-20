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

/// Processes the argument section '(....)' of a function
func (:s) er_t trans_function_args
(
    m @* o,
    m x_source* source,
    c xoico_func_s* func,
    c :result* result_object_expr,
    c xoico_typespec_s* typespec_object,
    m :result* result,
    m xoico_typespec_s* typespec_return
)
{
    tp_t transient_return_type = 0;

    tp_t object_type = func.obj_type;
    c xoico_signature_s* signature = func.signature;
    typespec_return.type = 0;
    o.trans( source, "(", result );

    tp_t cast_to_var = signature.typespec_ret.transient ? signature.typespec_ret.transient.cast_to_var : 0;

    tp_t ret_transient_class = signature.typespec_ret.transient ? signature.typespec_ret.transient.class : 0;

    c xoico_transient_map_s* transient_map = NULL;

    if( signature.arg_o )
    {
        if( !result_object_expr )
        {
            m $* result_expr   = :result_arr_s!^^;
            m $* typespec_expr = xoico_typespec_s!^^;
            o.trans_expression( source, result_expr, typespec_expr );
            result_object_expr = result_expr;
            typespec_object = typespec_expr;
            if( signature.args.size > 0 ) source.parse_fa( " ," );
        }

        transient_map = ( typespec_object ) ? o.get_transient_map( typespec_object.type ) : NULL;

        m xoico_typespec_s* typespec_object_adapted = signature.arg_o.typespec.clone()^^;

        if( object_type ) typespec_object_adapted.relent( o.host, object_type );

        if( typespec_object.type )
        {
            if
            (
                ret_transient_class &&
                signature.arg_o.typespec.transient &&
                signature.arg_o.typespec.transient.class == ret_transient_class
            )
            {
                transient_return_type = typespec_object.type;
            }
            o.adapt_expression( source, typespec_object, typespec_object_adapted, result_object_expr, result );
        }
        else
        {
            result.push_result_c( result_object_expr );
        }
        if( signature.args.size > 0 ) result.push_sc( "," );

        if( cast_to_var == signature.arg_o.name )
        {
            typespec_return.copy( typespec_object );
            typespec_return.indirection = typespec_object_adapted.indirection;
        }
    }

    if( transient_map && ret_transient_class )
    {
        if( !transient_return_type ) transient_return_type = transient_map.get( ret_transient_class );
    }

    foreach( c $* arg in signature.args )
    {
        if( arg.is_variadic() ) break;

        m $* result_expr = :result_arr_s!^^;
        m $* typespec_expr = xoico_typespec_s!^^;
        source.parse_fa( " " );

        if( __i > 0 )
        {
            source.parse_fa( " ," );
            result.push_sc( "," );
        }

        o.trans_whitespace( source, result_expr );
        if( source.parse_bl( "#?')'" ) ) return source.parse_error_fa( "Function argument '#<sc_t>': No value passed.", o.nameof( arg.name ) );

        o.trans_expression( source, result_expr, typespec_expr );

        if( typespec_expr.type )
        {
            if( arg.typespec.transient )
            {
                if( transient_return_type == 0 && arg.typespec.transient.class == ret_transient_class )
                {
                    transient_return_type = typespec_expr.type;
                }

                if( transient_map )
                {
                    tp_t t = transient_map.get( ret_transient_class );
                    if( o.is_stamp( t ) )
                    {
                        if( typespec_expr.type != t )
                        {
                            return source.parse_error_fa( "Function argument '#<sc_t>': Type '#<sc_t>' expected but type '#<sc_t>' passed.", o.nameof( arg.name ), o.nameof( t ), o.nameof( typespec_expr.type ) );
                        }
                    }
                }
            }

            o.adapt_expression( source, typespec_expr, arg.typespec, result_expr, result );
        }
        else
        {
            result.push_result_d( result_expr.fork() );
        }

        if( cast_to_var == arg.name )
        {
            typespec_return.copy( typespec_expr );
            typespec_return.indirection = arg.typespec.indirection;
        }
    }

    if( signature.args.is_variadic() )
    {
        while( !source.eos() && !source.parse_bl( " #=?')'" ) )
        {
            source.parse_fa( " ," );
            result.push_sc( "," );
            o.trans_expression( source, result, NULL );
        }
    }

    source.parse_fa( " " );
    o.trans( source, ")", result );

    if( typespec_return.type == 0 )
    {
        typespec_return.copy( signature.typespec_ret );
        if( transient_return_type != 0 ) typespec_return.type = transient_return_type;
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_function
(
    m @* o,
    m x_source* source,
    c xoico_func_s* func,
    c :result* result_object_expr, // NULL on direct calls
    c xoico_typespec_s* typespec_object, // NULL on direct calls
    m :result* result,
    m xoico_typespec_s* return_typespec
)
{
    m $* typespec_ret = xoico_typespec_s!^;
    m $* result_args = :result_arr_s!^;

    o.trans_function_args( source, func, result_object_expr, typespec_object, result_args, typespec_ret );

    m $* result_expression = :result_arr_s!^;
    result_expression.push_sc( o.nameof( func.global_name ) );
    result_expression.push_result_d( result_args.fork() );

    /** Casting can become necessary when the functions declared return typespec is less specific than
     *  the expected return type due to transient types.
     */
    if
    (
        typespec_ret.type        != func.signature.typespec_ret.type ||
        typespec_ret.indirection != func.signature.typespec_ret.indirection
    )
    {
        result.push_result_d( :result_cast_s!( o, typespec_ret.clone(), result_expression.fork() ).reduce() );
    }
    else
    {
        result.push_result_d( result_expression.fork() );
    }

    if( return_typespec ) return_typespec.copy( typespec_ret );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

