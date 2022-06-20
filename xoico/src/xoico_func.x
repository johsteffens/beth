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

signature tp_t get_hash( c @* o );
signature bl_t reflectable( c @* o, c xoico_host* host );
signature er_t setup_from_signature( m @* o, c xoico_host* host, c xoico_signature_s* signature );

stamp :s = aware :
{
    tp_t name;                   // declarative name (not global name)
    tp_t global_name;            // function name in c-implementation (if left 0 it is computed during finalization)
    tp_t obj_type;               // obj_type to relent signatures

    tp_t signature_global_name;

    bl_t expandable = true;
    bl_t overloadable = false;
    bl_t declare_in_expand_forward = true;

    tp_t pre_hash = 0;

    xoico_body_s => body;

    // if not defined at finalization it is retrieved via signature_global_name
    xoico_signature_s => signature;

    x_source_point_s source_point;

    func :.get_hash;

    func :.reflectable
    {
        return o.expandable && host.compiler().is_feature( o.signature_global_name );
    };

    func xoico.parse;
    func xoico.finalize;
    func xoico.expand_forward;
    func xoico.expand_declaration;
    func xoico.expand_definition;
    func xoico.get_source_point { return o.source_point; };

    func xoico_signature.as_member { return o.signature ? o.signature.as_member() : false; };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.get_hash
{
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o._ );
    hash = bcore_tp_fold_tp( hash, o.pre_hash );
    hash = bcore_tp_fold_tp( hash, o.name );
    hash = bcore_tp_fold_tp( hash, o.global_name );
    hash = bcore_tp_fold_tp( hash, o.obj_type );
    hash = bcore_tp_fold_tp( hash, o.signature_global_name );
    hash = bcore_tp_fold_bl( hash, o.expandable );
    hash = bcore_tp_fold_bl( hash, o.overloadable );
    hash = bcore_tp_fold_bl( hash, o.declare_in_expand_forward );
    if( o.body      ) hash = bcore_tp_fold_tp( hash, o.body.get_hash() );
    if( o.signature ) hash = bcore_tp_fold_tp( hash, o.signature.get_hash() );
    return hash;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t freeze_global_name( m @* o, c xoico_host* host )
{
    if( o.global_name ) return 0;
    m $* compiler = host.compiler();
    o.global_name = compiler.entypeof( st_s_create_fa( "#<sc_t>_#<sc_t>", compiler.nameof( host.obj_type() ), compiler.nameof( o.name ) )^^.sc );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.parse
{
    m $* compiler = host.compiler();

    o.source_point.setup_from_source( source );

    bl_t is_enclosed_signature = source.parse_bl( " #?'('" ); // old style enclosed signature

    // We first try parsing a direct signature. If that fails, we assume the signature is referenced by an identifier.
    // This allows parsing without old style bracket enclosing. If that is a save practice under all circumstances is to be seen.
    s3_t index = source.get_index();
    m xoico_signature_s* signature = xoico_signature_s!^;

    if( is_enclosed_signature )
    {
        return source.parse_error_fa( "Signature-enclosure '( ...sig... )' is deprecated." );

        signature.parse( host, source );
        source.parse_fa( " ) " );
    }
    else if( signature.parse( host, source ) != 0 )
    {
        bcore_error_remove_last();
        source.set_index( index );
        signature = NULL;
    }

    if( signature )
    {
        compiler.life_a_push( signature.fork() );
        o.pre_hash = bcore_tp_fold_tp( o.pre_hash, signature.get_hash() );
        o.name = signature.name;
        compiler.register_item( signature );
        o.signature_global_name = signature.global_name;
        o.signature =< signature.clone();
        o.signature.relent( host, host.obj_type() );
        if( host.defines_transient_map() ) o.signature.convert_transient_types( host, host.transient_map() );
    }
    else // use of external signature definition
    {
        tp_t tp_signature_base_name = 0;
        tp_t tp_signature_global_name = 0;

        if( source.parse_bl( " #?'^'" ) )
        {
            if( host._ == xoico_stamp_s~ )
            {
                tp_signature_base_name = host.cast( c xoico_stamp_s* ).trait_name;
            }
            else
            {
                return source.parse_error_fa( "'^' can only be used inside a stamp." );
            }
            source.parse_fa( " ." );
        }
        else
        {
            tp_t name = 0;
            host.parse_name_tp( source, name.1 );
            if( source.parse_bl( " #?'.'" ) )
            {
                tp_signature_base_name = name;
            }
            else
            {
                sz_t idx = -1;
                if( host._ == xoico_stamp_s~ && ( idx = host.cast( xoico_stamp_s* ).funcs.get_index_from_name( name ) ) >= 0 )
                {
                    tp_signature_global_name = host.cast( xoico_stamp_s* ).funcs.[ idx ].signature_global_name;
                }
                else
                {
                    tp_signature_base_name = host.cast( c xoico* ).get_global_name_tp();
                }
                o.name = name;
            }
        }

        if( !o.name )
        {
            st_s^ st_name;
            source.parse_fa( " #name", st_name.1 ); // empty name is allowed
            o.name = compiler.entypeof( st_name.sc );
        }

        if( !tp_signature_global_name )
        {
            tp_signature_global_name = compiler.entypeof( st_s_create_fa( "#<sc_t>_#<sc_t>", compiler.nameof( tp_signature_base_name ), compiler.nameof( o.name ) )^.sc );
        }

        o.signature_global_name = tp_signature_global_name;
    }

    if( !source.parse_bl( " #=?';'" ) ) o.body!.parse( host, source );

    source.parse_fa( " #-?';' " ); // closing semicolon is optional

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t push_flect_decl_to_sink( c @* o, c xoico_host* host, m x_sink* sink )
{
    m $* compiler = host.compiler();
    sink.push_sc( "func " );
    if( host._ == xoico_stamp_s~ && o.signature.base_name == host.cast( c xoico_stamp_s* ).trait_name )
    {
        sink.push_fa( "^:#<sc_t>", compiler.nameof( o.name ) );
    }
    else
    {
        sink.push_fa( "#<sc_t>:#<sc_t>", compiler.nameof( o.signature.base_name ), compiler.nameof( o.name ) );
    }
    sink.push_sc( ";" );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.finalize
{
    m $* compiler = host.compiler();
    o.freeze_global_name( host );
    o.obj_type = host.obj_type();

    if( !o.signature )
    {
        c xoico_signature_s* signature = compiler.get_signature( o.signature_global_name );
        if( !signature )
        {
            return o.source_point.parse_error_fa
            (
                "Function #<sc_t>: Could not find signature #<sc_t>",
                compiler.nameof( o.name ),
                compiler.nameof( o.signature_global_name )
            );
        }

        o.signature = signature.clone();
        o.signature.relent( host, host.obj_type() );
        if( host.defines_transient_map() ) o.signature.convert_transient_types( host, host.transient_map() );
    }

    if( o.body ) o.body.finalize( host );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.expand_forward
{
    if( !o->expandable ) return 0;
    if( !o->declare_in_expand_forward ) return 0;

    m $* compiler = host.compiler();
    sink.push_fa( " \\\n#rn{ }", indent );
    bl_t go_inline = o.body && o.body.go_inline;
    c $* signature = o.signature;
    ASSERT( signature );
    if( go_inline )
    {
        sink.push_fa( "static inline " );
        signature.expand_declaration( host, compiler.nameof( o.global_name ), indent, sink );
    }
    else
    {
        signature.expand_declaration( host, compiler.nameof( o.global_name ), indent, sink );
    }
    sink.push_fa( ";" );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.expand_declaration
{
    if( !o.expandable ) return 0;

    bl_t go_inline = o.body && o.body.go_inline;
    c $* signature = o.signature;
    ASSERT( signature );
    m $* compiler = host.compiler();

    if( go_inline )
    {
        sink.push_fa( " \\\n#rn{ }", indent );
        sink.push_fa( "static inline " );
        signature.expand_declaration( host, xoico_compiler_s_nameof( compiler, o->global_name ), indent, sink );
        o.body.expand( host, signature, indent, sink );
    }
    else if( !o->declare_in_expand_forward )
    {
        sink.push_fa( " \\\n#rn{ }", indent );
        signature.expand_declaration( host, xoico_compiler_s_nameof( compiler, o->global_name ), indent, sink );
        sink.push_fa( ";" );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.expand_definition
{
    if( !o.expandable ) return 0;
    m $* compiler = host.compiler();

    c $* signature = o.signature;
    ASSERT( signature );
    if( o.body && !o.body.go_inline )
    {
        sink.push_fa( "\n" );
        sink.push_fa( "#rn{ }", indent );
        signature.expand_declaration( host, compiler.nameof( o.global_name ), indent, sink );
        sink.push_fa( "\n" );
        o.body.expand( host, signature, indent, sink );
        sink.push_fa( "\n" );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

