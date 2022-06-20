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

signature er_t relent( m @* o, c xoico_host* host, tp_t tp_obj_type );
signature er_t expand_declaration( c @* o, c xoico_host* host, sc_t sc_func_global_name, sz_t indent, m x_sink* sink );
signature er_t set_global_name( m @* o, c xoico_host* host );
signature bl_t as_member( c @* o ); // indicates that the function can be used as member function to an object

stamp :s = aware :
{
    tp_t name;
    tp_t base_name;
    tp_t global_name;

    xoico_typespec_s typespec_ret; // return type
    xoico_arg_s => arg_o; // (first) object argument (NULL in case of plain function)
    xoico_args_s args; // arguments (excluding arg_o) e.g.: sz_t a, sz_t b

    // points to an argument in case this signature is supposed to directly return that argument
    xoico_arg_s -> direct_return_arg;

    x_source_point_s source_point;

    func xoico.parse;
    func xoico.get_hash;
    func :.set_global_name;

    func xoico.convert_transient_types
    {
        if( o.arg_o )
        {
            o.arg_o.convert_transient_types( host, map );
            if
            (
                o.arg_o.typespec.transient &&
                o.typespec_ret.transient &&
                o.arg_o.typespec.transient.class == o.typespec_ret.transient.class
            )
            {
                o.typespec_ret.type = host.obj_type();
            }
        }

        o.args.convert_transient_types( host, map );
        o.typespec_ret.convert_transient_types( host, map );
        return  0;
    };

    func xoico.get_global_name_tp { return o.global_name; };

    func :.relent
    {
        if( o.arg_o ) o.arg_o.relent( host, tp_obj_type );
        o.args.relent( host, tp_obj_type );
        o.typespec_ret.relent( host, tp_obj_type );
        return 0;
    };

    func :.expand_declaration;

    func xoico_arg.is_variadic { return o.args.is_variadic(); };

    func :.as_member { return o.arg_o != NULL; };

    func xoico.get_source_point { return o.source_point; };

    func bl_t returns_a_value( c @* o ) { return !o.typespec_ret.is_void(); };

    func er_t expand_ret( c @* o, c xoico_host* host, m x_sink* sink )
    {
        o.typespec_ret.expand( host, sink );
        return 0;
    };

    func er_t expand_ret_x( c @* o, c xoico_host* host, m x_sink* sink )
    {
        if( o.typespec_ret.transient && o.typespec_ret.transient.cast_to_var )
        {
            tp_t var_name = o.typespec_ret.transient.cast_to_var;
            sink.push_fa( "#<sc_t> ", host.nameof( var_name ) );
        }
        else
        {
            o.typespec_ret.expand_x( host, sink );
        }
        return 0;
    };

    func c xoico_arg_s* get_arg_by_name( c @* o, tp_t name )
    {
        return ( o.arg_o && o.arg_o.name == name ) ? o.arg_o : o.args.get_arg_by_name( name );
    };

    func bl_t is_anonymous( c @* o ) = o.name == TYPEOF_;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.get_hash
{
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o._ );
    hash = bcore_tp_fold_tp( hash, o.global_name );
    hash = bcore_tp_fold_tp( hash, o.typespec_ret.get_hash() );
    hash = bcore_tp_fold_tp( hash, o.arg_o ? o.arg_o.get_hash() : 0 );
    hash = bcore_tp_fold_tp( hash, o.args.get_hash() );
    return hash;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.set_global_name
{
    o.base_name = host.obj_type();
    o.global_name = host.entypeof( st_s_create_fa( "#<sc_t>_#<sc_t>", host.nameof( o.base_name ), host.nameof( o.name ) )^^.sc );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.parse
{
    m $* compiler = host.compiler();
    m $* name_buf = st_s!^^;

    if( source.parse_bl( " #?'extending'" ) )
    {
        tp_t tp_name = 0;
        host.parse_name_tp( source, tp_name.1 );
        c $* signature = compiler.get_signature( tp_name );
        if( !signature ) return source.parse_error_fa( "Could not find predefined signature '#<sc_t>'.", host.nameof( tp_name ) );
        o.copy( signature );
        o.source_point.setup_from_source( source );
        source.parse_fa( " #name", name_buf );  // empty name is allowed
        o.name = compiler.entypeof( name_buf.sc );
        source.parse_fa( " (" );
        o.args.append( host, source );
        source.parse_fa( " )" );
    }
    else
    {
        o.source_point.setup_from_source( source );
        o.typespec_ret.parse( host, source );
        o.typespec_ret.flag_addressable = false;

        // get name
        source.parse_fa( " #name", name_buf );
        o.name = compiler.entypeof( name_buf.sc );

        source.parse_fa( " (" );

        if( !source.parse_bl( " #=?')'" ) )
        {
            m xoico_arg_s* arg = xoico_arg_s!^;
            arg.parse( host, source );
            if( !source.parse_bl( " #=?')'" ) ) source.parse_fa( ", " );
            if( arg.name == TYPEOF_o )
            {
                o.arg_o =< arg.fork();
                o.args.parse( host, source );
            }
            else
            {
                o.args.push_d( arg.fork() );
                o.args.append( host, source );
            }
        }
        else
        {
            o.args.parse( host, source );
        }

        source.parse_fa( " )" );
    }

    /// if return type is a name in the argument list, copy argument typespec to return typespec
    xoico_arg_s* ret_arg = ( o.typespec_ret.indirection == 0 ) ? o.get_arg_by_name( o.typespec_ret.type ) : NULL;

    if( ret_arg )
    {
        if( o.typespec_ret.access_class != 0  || o.typespec_ret.transient || o.typespec_ret.indirection != 0 )
        {
            return source.parse_error_fa( "Return typespec: Argument name used as type." );
        }
        o.typespec_ret.copy( ret_arg.typespec );
        o.typespec_ret.transient!.cast_to_var = ret_arg.name;
        o.direct_return_arg =< ret_arg.cast( m$* ).fork();
    }

    o.set_global_name( host );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.expand_declaration
{
    o.expand_ret( host, sink );
    sink.push_fa( " #<sc_t>( ", sc_func_global_name );

    if( o.arg_o )
    {
        o.arg_o.expand( host, sink );
        o.args.expand( host, false, sink );
        sink.push_fa( " )" );
    }
    else
    {
        if( o.args.size > 0 )
        {
            o.args.expand( host, true, sink );
        }
        else
        {
            sink.push_fa( "void" );
        }
        sink.push_fa( " )" );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) bl_t fits_transient_type_feature( @*o )
{
    if( o.typespec_ret.type != TYPEOF_tp_t ) = false;
    if( o.typespec_ret.indirection != 0 )    = false;
    if( !o.as_member() )                     = false;
    = true;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
