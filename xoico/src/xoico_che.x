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

group :result = : { embed "xoico_che_result.x"; };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// stack for variable declarations
group :stack_var = :
{
    stamp :unit_s = aware bcore_inst
    {
        sz_t level;
        tp_t name;
        xoico_typespec_s typespec;
    };

    stamp :unit_adl_s = aware x_array { :unit_s => []; };

    signature m @* push_unit( m @* o, c :unit_s* unit );
    signature m @* pop_level( m @* o, sz_t level ); // pop all units of or above level

    signature bl_t exists( c @* o, tp_t name );
    signature c xoico_typespec_s* get_typespec( c @* o, tp_t name );
    signature c sz_t get_level( c @* o, tp_t name );
    signature void clear( m @* o );
    signature void rehash_names( m @* o );

    stamp :s = aware :
    {
        :unit_adl_s adl;
        bcore_hmap_tpuz_s hmap_name;

        func :.exists { return o.hmap_name.exists( name ); };

        func :.rehash_names
        {
            o.hmap_name.clear();
            for( sz_t i = 0; i < o.adl.size; i++ ) o.hmap_name.set( o.adl.[i].name, i );
        };

        func :.push_unit
        {
            o.adl.push_c( unit );
            o.hmap_name.set( unit->name, o.adl.size - 1 );
            return o;
        };

        func :.pop_level
        {
            sz_t size = o->adl.size;
            while( size > 0 && o.adl.data[ size - 1 ]->level >= level ) size--;
            o.adl.set_size( size );
            o.rehash_names();
            return o;
        };

        func :.get_typespec
        {
            m uz_t* p_idx = o.hmap_name.get( name );
            if( !p_idx ) return NULL;
            return o.adl.[ p_idx.0 ].typespec;
        };

        /// returns -1 if not found
        func :.get_level
        {
            m uz_t* p_idx = o.hmap_name.get( name );
            if( !p_idx ) return -1;
            return o.adl.[ p_idx.0 ].level;
        };

        func :.clear
        {
            o.adl.clear();
            o.hmap_name.clear();
        };
    };
};

//----------------------------------------------------------------------------------------------------------------------

/// stack for block/level specific data
group :stack_block = :
{
    stamp :unit_s = aware bcore_inst
    {
        sz_t level; // level of this block
        bl_t use_blm = false;

        /** This block represents a break-ledge for a break-command inside
         *  this block or higher-level blocks up to the next break-level
         */
        bl_t break_ledge = false;

        /** This block represents syntactically a statement but is treated
         *  semantically as block.
         *  This flag is used to warn the developer on constructions where
         *  this fact can have unexpected side effects.
         */
        bl_t statement_wrapped_as_block = false;
    };

    stamp :unit_adl_s = aware x_array { :unit_s => []; };

    signature m @* push( m @* o );
    signature m @* push_unit( m @* o, c :unit_s* unit );
    signature m @* pop(  m @* o );
    signature void clear( m @* o );
    signature sz_t get_size( c @* o );

    stamp :s = aware :
    {
        :unit_adl_s adl;
        func :.push      { o.adl.push_d( :unit_s! ); return o; };
        func :.push_unit { o.adl.push_c( unit ); return o; };

        func :.pop
        {
            o.adl.set_size( sz_max( o->adl.size - 1, 0 ) );
            return o;
        };

        func :.clear { o.adl.clear(); };
        func :.get_size { return o.adl.size; };
    };

};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

signature tp_t entypeof(  m @* o, sc_t name );
signature sc_t nameof(    m @* o, tp_t type );
signature void init_level0( m @* o );
signature void inc_level( m @* o );
signature void dec_level( m @* o );
signature void inc_block( m @* o );
signature void dec_block( m @* o );
signature void push_typedecl( m @* o, c xoico_typespec_s* typespec, tp_t name );
signature m :stack_block_unit_s* stack_block_get_top_unit( m @* o );
signature m :stack_block_unit_s* stack_block_get_bottom_unit( m @* o );
signature m :stack_block_unit_s* stack_block_get_level_unit( m @* o, sz_t level );
signature er_t push_typespec( m @* o, c xoico_typespec_s* typespec, m :result* result );
signature void typespec_to_sink( m @* o, c xoico_typespec_s* typespec, m x_sink* sink );

signature bl_t is_var( c @* o, tp_t name );

name static;
name volatile;
name cast;
name verbatim_C;
name keep;
name scope;
name t_scope;
name scope_local;
name scope_func;
name keep_func;
name keep_block;
name fork;
name try;
name if;
name else;
name while;
name do;
name for;
name foreach;
name switch;
name case;
name default;
name break;
name return;
name completion;
name continue;
name goto;
name true;
name false;

stamp :s = aware :
{
    /// parameters

    bl_t verbose = false;

    /// Prepends a commented reference to the xoila source for each function in *xoila_out.c
    bl_t insert_source_reference = true;

    /// Prepends a commented reference to the xoila source for each function in *xoila_out.c
    bl_t for_all_functions_enable_try = true;

    /// Waivers for purity-control and side effects ...

    /// (purity) Condition: identifier specifies an undeclared member variable
    bl_t waive_unknown_member_variable = false;

    /// (purity) Condition: identifier specifies an undeclared member function
    bl_t waive_unknown_member_function = false;

    /// (purity) Condition: trans_expression: function identifier is not used in a tractable way (e.g. not as function call).
    bl_t waive_function_in_untraced_context = false;

    /// (purity) Condition: Unknown type was detected in an the arglist of a function
    bl_t waive_unknown_type = false;

    /// (purity) Condition: trans_expression encounters an unknown identifier
    bl_t waive_unknown_identifier = false;

    /** (side effect)
     *  Condition: Operator '^' used in block-wrapped-statement that appear syntactically not being a block.
     *  E.g.:
     *  In case '^' is used inside <statement>:
     *    if( <condition> ) <statement>
     *    should better be written as
     *    if( <condition> ) { <statement> }
     */
    bl_t waive_local_scope_operator_creates_implicit_block = false;

    /// end parameters

    /// runtime data
    hidden aware xoico_host*       host;
    hidden aware xoico_compiler_s* compiler;
    xoico_signature_s => signature;

    /// runtime state

    hidden sz_t block_level;
    hidden sz_t try_block_level;
    hidden :stack_var_s   stack_var;
    hidden :stack_block_s stack_block;
    hidden bcore_hmap_name_s hmap_name;

    hidden bl_t has_completion; // function has at least one completion statement (used to detect missing returns)
    hidden bl_t has_verbatim_code; // function has verbatim code (disables error on missing return)

    func xoico.get_hash
    {
        tp_t hash = bcore_tp_init();

        hash = bcore_tp_fold_bl( hash, o.verbose );
        hash = bcore_tp_fold_bl( hash, o.insert_source_reference );
        hash = bcore_tp_fold_bl( hash, o.for_all_functions_enable_try );

        hash = bcore_tp_fold_bl( hash, o.waive_unknown_member_variable );
        hash = bcore_tp_fold_bl( hash, o.waive_unknown_member_function );
        hash = bcore_tp_fold_bl( hash, o.waive_function_in_untraced_context );
        hash = bcore_tp_fold_bl( hash, o.waive_unknown_type );
        hash = bcore_tp_fold_bl( hash, o.waive_unknown_identifier );
        hash = bcore_tp_fold_bl( hash, o.waive_local_scope_operator_creates_implicit_block );

        return hash;
    };

    func xoico_cengine.translate;

    func xoico_cengine.is_reserved
    {
        return o.is_builtin_func( tp_identifier ) ||
               o.is_control_name( tp_identifier ) ||
               tp_identifier == TYPEOF_verbatim_C;
    };

    func :.entypeof { return o.hmap_name.set_sc( name ); };

    func :.nameof
    {
        sc_t name = o.hmap_name.get_sc( type );
        if( !name ) name = o.compiler.nameof( type );
        return name;
    };

    func :.init_level0
    {
        o.stack_block.clear();
        o.stack_block.push();
        o.block_level = 0;
    };

    func :.inc_block
    {
        o.stack_block.push();
        o->block_level++;
        o.stack_block_get_top_unit().level = o.block_level;
    };

    func :.dec_block
    {
        o.stack_var.pop_level( o.block_level );
        o.block_level--;
        ASSERT( o.block_level >= 0 );
        o.stack_block.pop();
    };

    func :.stack_block_get_top_unit
    {
        return o.stack_block.adl.[ o.stack_block.adl.size - 1 ];
    };

    func :.stack_block_get_bottom_unit
    {
        return o.stack_block.adl.[ 0 ];
    };

    func :.stack_block_get_level_unit
    {
        foreach( m $* e in o.stack_block.adl ) if( e.level == level ) return e;
        ERR_fa( "Level #<sz_t> not found.", level );
        return NULL;
    };

    func :.push_typedecl
    {
        m :stack_var_unit_s* unit = :stack_var_unit_s!^^;
        unit.level = o.block_level;
        unit.name = name;
        unit.typespec.copy( typespec );
        o.stack_var.push_unit( unit );
    };

    func :.push_typespec;
    func :.typespec_to_sink
    {
        m $* result = :result_arr_s!^;
        o.push_typespec( typespec, result );
        result.to_sink( sink );
    };

    func xoico_compiler.is_type { return o.compiler.is_type( name ); };
    func xoico_compiler.is_identifier { return o.compiler.is_identifier( name ); };
    func xoico_compiler.is_name  { return o.compiler.is_name( name ); };
    func xoico_compiler.is_group { return o.compiler.is_group( name ); };
    func xoico_compiler.is_stamp { return o.compiler.is_stamp( name ); };
    func xoico_compiler.is_func  { return o.compiler.is_func( name ); };
    func :.is_var { return o.stack_var.exists( name ); };

    func xoico_compiler.get_group { return o.compiler.get_group( name ); };
    func xoico_compiler.get_stamp { return o.compiler.get_stamp( name ); };
    func xoico_compiler.get_func  { return o.compiler.get_func( name ); };
    func xoico_compiler.get_transient_map { return o.compiler.get_transient_map( type ); };

    func bl_t returns_a_value( c @* o ) { return o.signature.returns_a_value(); };

    func er_t trans( c @* o, m x_source* source, sc_t format, m :result* result )
    {
        try( source.parse_fa( format ));
        result.push_sc( format );
        return 0;
    };


    func bl_t returns_er_t( c @* o )
    {
        return o.signature.typespec_ret.type == er_t~ && o.signature.typespec_ret.indirection == 0;
    }
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Inspects name and enrolls it and returns hash;
 *  Detects '$' as TYEPOF_type_deduce
 *  Detects '@' as type of class or group
 *  If no identifier could be detected: Returns 0 and leave source unchanged.
 */
func (:s) tp_t get_identifier( m @* o, m x_source* source, bl_t take_from_source )
{
    tp_t tp_identifier = 0;
    if( source.parse_bl( "#?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_'||[0]=='@'||[0]=='$'||([0]==':'&&([1]!=' '&&[1]!='\t'&&[1]!='\n'&&[1]!='/')))" ) )
    {
        sz_t source_index = take_from_source ? 0 : source.get_index();
        switch( source.inspect_char() )
        {
            case '@':
            {
                source.get_char();
                m st_s* st_name = st_s!^^;
                st_name.copy_sc( o.nameof( o.host.obj_type() ) );
                if( source.parse_bl( "#?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_'||([0]>='0'&&[0]<='9'))" ) )
                {
                    source.parse_fa( "#:name", st_name );
                }
                tp_identifier = o.entypeof( st_name.sc );
            }
            break;

            case '$':
            {
                source.get_char();
                tp_identifier = type_deduce~;
            }
            break;

            case ':':
            {
                o.host.parse_name_tp( source, tp_identifier.1 );
            }
            break;

            default:
            {
                m st_s* st_name = st_s!^;
                source.parse_fa( "#name", st_name );
                tp_identifier = o.entypeof( st_name.sc );
            }
            break;
        }

        if( !take_from_source ) source.set_index( source_index );
    }

    return tp_identifier;
};

//----------------------------------------------------------------------------------------------------------------------

/** transfers identifier, enrolls it and sets 'tp_identifier' */
func(:s) er_t trans_identifier
(
    m @* o,
    m x_source* source,
    m :result* result,    // can be NULL
    m tp_t* tp_identifier // can be NULL
)
{
    tp_t identifier = o.get_identifier( source, true );
    if( !identifier )
    {
        return source.parse_error_fa( "Identifier expected" );
    }
    if( tp_identifier ) tp_identifier.0 = identifier;
    if( result ) result.push_sc( o.nameof( identifier ) );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/// parses number: (all integer, hex and float encodings)
func(:s) er_t trans_number_literal( m @* o, m x_source* source, m :result* result )
{
    bl_t hex = false;
    if( source.parse_bl( "#?'0x'" ) )
    {
        result.push_sc( "0x" );
        hex = true;
    }
    else if( source.parse_bl( "#?'0X'" ) )
    {
        result.push_sc( "0X" );
        hex = true;
    }

    if( hex )
    {
        while( source.parse_bl( "#?(([0]>='0'&&[0]<='9')||([0]>='a'&&[0]<='f')||([0]>='A'&&[0]<='F'))" ) )
        {
            result.push_char( source.get_char() );
        }
    }
    else // decimal
    {
        while( source.parse_bl( "#?([0]>='0'&&[0]<='9')" ) ) result.push_char( source.get_char() );
    }

    if( source.parse_bl( "#?([0]=='.')" ) )
    {
        result.push_char( source.get_char() );
        while( source.parse_bl( "#?([0]>='0'&&[0]<='9')" ) ) result.push_char( source.get_char() );
    }

    bl_t exponent = false;

    if( source.parse_bl( "#?([0]=='e'||[0]=='E')" ) )
    {
        result.push_char( source.get_char() );
        exponent = true;
    }

    if( exponent )
    {
        if( source.parse_bl( "#?([0]=='+'||[0]=='-')" ) )
        {
            result.push_char( source.get_char() );
        }

        while( source.parse_bl( "#?([0]>='0'&&[0]<='9')" ) )
        {
            result.push_char( source.get_char() );
        }
    }

    // suffix
    while( source.parse_bl( "#?([0]=='l'||[0]=='L'||[0]=='u'||[0]=='U'||[0]=='f'||[0]=='F')" ) )
    {
        result.push_char( source.get_char() );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/// parses string
func (:s) er_t trans_string_literal( m @* o, m x_source* source, m :result* result )
{
    o.trans( source, "\"", result );

    while( !source.eos() && !source.parse_bl( "#=?'\"'" ) )
    {
        if( source.parse_bl( "#?'\\'" ) ) result.push_char( '\\' );
        result.push_char( source.get_char() );
    }

    o.trans( source, "\"", result );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/// character literal
func (:s) er_t trans_char_literal( m @* o, m x_source* source, m :result* result )
{
    o.trans( source, "'", result );

    while( !source.eos() && !source.parse_bl( "#=?|'|" ) )
    {
        if( source.parse_bl( "#?'\\'" ) ) result.push_char( '\\' );
        result.push_char( source.get_char() );
    }

    o.trans( source, "'", result );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/// parses whitespaces including comments
func (:s) er_t trans_whitespace( m @* o, m x_source* source, m :result* result /* can be NULL */ )
{
    bl_t exit_loop = false;

    m st_s* st = result ? st_s!^ : NULL;

    while( !exit_loop && !source.eos() )
    {
        char c =  source.inspect_char();
        switch( c )
        {
            case ' ' :
            case '\t':
            case '\n':
            {
                char c =  source.get_char();
                if( st ) st.push_char( c );
            }
            break;

            case '/':
            {
                if( source.parse_bl( "#?'//'" ) )
                {
                    if( st ) st.push_sc( "//" );
                    while( !source.eos() )
                    {
                        char c =  source.get_char();
                        if( st ) st.push_char( c );
                        if( c == '\n' ) break;
                    }
                }
                else if( source.parse_bl( "#?'/*'" ) )
                {
                    if( st ) st.push_sc( "/*" );
                    while( !source.eos() )
                    {
                        if( source.parse_bl( "#?'*/'" ) )
                        {
                            if( st ) st.push_sc( "*/" );
                            break;
                        }
                        char c =  source.get_char();
                        if( st ) st.push_char( c );
                    }
                }
                else
                {
                    exit_loop = true;
                }
            }
            break;

            default:
            {
                exit_loop = true;
            }
            break;
        }
    }

    if( result && st.size > 0 ) result.push_result_d( :result_whitespace_s!( st.fork() ) );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_preprocessor( m @* o, m x_source* source, m :result* result )
{
    source.parse_fa( "##" );
    result.push_sc( "#" );
    while( !source.eos() )
    {
        char c = source.get_char();
        result.push_char( c );
        if( c == '\\' && source.inspect_char() == '\n' ) result.push_char( source.get_char() );
        if( c == '\n' ) break;
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

// any state; returns !=0 in case an operator was consumed
func (:s) tp_t trans_inert_operator( m @* o, m x_source* source, m :result* result )
{
    switch( source.inspect_char() )
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
        case '>':
        case '<':
        case '!':
        case '|':
        case '&':
        case '^':
        case '~':
        case '%':
        {
            result.push_char( source.get_char() );
            return 1;
        }
        break;

        default: break;
    }

    return false;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) bl_t trans_operator( m @* o, m x_source* source, m :result* result )
{
    switch( source.inspect_char() )
    {
        case '+':
        {
            result.push_char( source.get_char() );
            char c = source.inspect_char();
            if( c == '=' || c == '+' ) result.push_char( source.get_char() );
        }
        return true;

        case '-':
        {
            result.push_char( source.get_char() );
            char c = source.inspect_char();
            if( c == '=' || c == '-' ) result.push_char( source.get_char() );
        }
        return true;

        case '*':
        case '/':
        case '=':
        case '!':
        case '^':
        case '%':
        {
            result.push_char( source.get_char() );
            if( source.inspect_char() == '=' ) result.push_char( source.get_char() );
        }
        return true;

        case '>':
        {
            result.push_char( source.get_char() );
            char c = source.inspect_char();
            if( c == '=' || c == '>' ) result.push_char( source.get_char() );
            if( c == '>' && source.inspect_char() == '=' ) result.push_char( source.get_char() );
        }
        return true;

        case '<':
        {
            result.push_char( source.get_char() );
            char c = source.inspect_char();
            if( c == '=' || c == '<' ) result.push_char( source.get_char() );
            if( c == '<' && source.inspect_char() == '=' ) result.push_char( source.get_char() );
        }
        return true;

        case '|':
        {
            result.push_char( source.get_char() );
            char c = source.inspect_char();
            if( c == '=' || c == '|' ) result.push_char( source.get_char() );
        }
        return true;

        case '&':
        {
            result.push_char( source.get_char() );
            char c = source.inspect_char();
            if( c == '=' || c == '&' ) result.push_char( source.get_char() );
        }
        return true;

        case '~':
        {
            result.push_char( source.get_char() );
        }
        return true;

        default: break;
    }

    return false;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t adapt_expression_indirection
(
    m @* o,
    m x_source* source,
    c xoico_typespec_s* typespec_expr,
    sz_t target_indirection,
    c :result* result_expr,
    m :result* result
)
{
    if( target_indirection == typespec_expr.indirection )
    {
        result.push_result_c( result_expr );
    }
    else if( target_indirection == typespec_expr.indirection + 1 )
    {
        if( typespec_expr.flag_addressable )
        {
            result.push_sc( "&(" );
            result.push_result_c( result_expr );
            result.push_sc( ")" );
        }
        else
        {
            return source.parse_error_fa( "Increasing indirection: Expression has no address." );
        }
    }
    else if( target_indirection < typespec_expr.indirection )
    {
        for( sz_t ind = typespec_expr.indirection; ind > target_indirection; ind-- ) result.push_sc( "*" );
        result.push_sc( "(" );
        result.push_result_c( result_expr );
        result.push_sc( ")" );
    }
    else
    {
        return source.parse_error_fa
        (
            "Cannot adapt from expression's indirection '#<sz_t>' to target indirection '#<sz_t>'.",
            typespec_expr.indirection,
            target_indirection
        );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t adapt_expression
(
    m @* o,
    m x_source* source,
    c xoico_typespec_s* typespec_expr,
    c xoico_typespec_s* typespec_target,
    c :result* result_expr,
    m :result* result
)
{
    if( !typespec_expr.type )
    {
        return source.parse_error_fa
        (
            "xoico_che_s_adapt_expression: no expression type specified.",
            typespec_expr.indirection,
            typespec_target.indirection
        );
    }

    if( !typespec_target.type )
    {
        return source.parse_error_fa
        (
            "xoico_che_s_adapt_expression: no target type specified.",
            typespec_expr.indirection,
            typespec_target.indirection
        );
    }

    if( typespec_target.type == type_deduce~ ) return source.parse_error_fa( "adapt_expression: typespec_target is 'type_deduce'" );
    if( typespec_target.type == type_object~ ) return source.parse_error_fa( "adapt_expression: typespec_target is 'type_object'" );
    if( typespec_expr.type == type_deduce~ ) return source.parse_error_fa( "adapt_expression: typespec_expr is 'type_deduce'" );
    if( typespec_expr.type == type_object~ ) return source.parse_error_fa( "adapt_expression: typespec_expr is 'type_object'" );

    bl_t discarding_const =
        ( typespec_expr.access_class == TYPEOF_const && typespec_expr.access_class != typespec_target.access_class ) &&
        ( typespec_expr.indirection > 0 && typespec_target.indirection > 0 );

    if( discarding_const ) return source.parse_error_fa( "Discarding 'const' qualifier." );

    if( typespec_target.access_class == TYPEOF_discardable && typespec_expr.access_class != TYPEOF_discardable )
    {
        return source.parse_error_fa( "Discardable expression expected." );
    }

    if( typespec_expr.converts_to( typespec_target ) )
    {
        result.push_result_c( result_expr );
    }
    else
    {
        bl_t implicit_cast = false;
        m st_s* fail_msg = NULL;
        if( typespec_target.type != typespec_expr.type )
        {
            if( o.is_group( typespec_target.type ) && typespec_target.indirection == 1 )
            {
                if( o.is_group( typespec_expr.type ) )
                {
                    implicit_cast = true;
                }
                else if( o.is_stamp( typespec_expr.type ) )
                {
                    c xoico_stamp_s* stamp = o.get_stamp( typespec_expr.type );
                    if( stamp.is_aware || typespec_target.flag_obliv )
                    {
                        implicit_cast = true;
                    }
                    else
                    {
                        fail_msg = st_s_create_fa( "'#<sc_t>' is obliv but the target typespec does not explicitly tolerate obliv objects.", o.nameof( typespec_expr.type ) )^^;
                    }
                }
                else if( o.is_type( typespec_expr.type ) )
                {
                    if( typespec_target.flag_obliv )
                    {
                        implicit_cast = true;
                    }
                    else
                    {
                        fail_msg = st_s_create_fa( "'#<sc_t>' is presumed obliv but the target typespec does not explicitly tolerate obliv objects.", o.nameof( typespec_expr.type ) )^^;
                    }
                }
                else
                {
                    fail_msg = st_s_create_fa( "Source type cannot be classified." )^^;
                }
            }
            else
            {
                fail_msg = st_s_create_fa( "Target is not a group or target indirection is != 1." )^^;
            }
        }

        if( fail_msg )
        {
            m $* st_typespec_expr = st_s!^^;
            m $* st_typespec_target = st_s!^^;
            o.typespec_to_sink( typespec_expr, st_typespec_expr );
            o.typespec_to_sink( typespec_target, st_typespec_target );
            m $* msg = st_s!^^;
            msg.push_fa( "Implicit cast from '#<sc_t>' to '#<sc_t>' is not allowed. ", st_typespec_expr.sc, st_typespec_target.sc );
            msg.push_fa( "Reason: #<st_s*>\n", fail_msg );
            return source.parse_error_fa( "#<st_s*>", msg );
        }

        m :result* result_expression = :result_arr_s!^;
        o.adapt_expression_indirection( source, typespec_expr, typespec_target.indirection, result_expr, result_expression );

        if( implicit_cast )
        {
            result.push_result_d( :result_cast_s!( o, typespec_target.clone(), result_expression.fork() ).reduce() );
        }
        else
        {
            result.push_result_d( result_expression.fork() );
        }

    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_member( m @* o, m x_source* source, m :result* result )
{
    if(      source.parse_bl( "#?'.'"  ) ) result.push_sc( "." );
    else if( source.parse_bl( "#?'->'" ) ) result.push_sc( "->" );

    o.trans_whitespace( source, result );

    char c = source.inspect_char();

    if( c == '*' || c == '&' )
    {
        return source.parse_error_fa( "Postfix operators '&' and '*' are deprecated.\n" );
    }

    if( c >= '0' && c <= '9' )
    {
        return source.parse_error_fa( "Setting indirection: Expression not tractable\n" );
    }

    tp_t tp_identifier = o.get_identifier( source, false );

    if( o.is_builtin_func( tp_identifier ) )
    {
        return source.parse_error_fa( "Expression before '#<sc_t>' not tractable\n", o.nameof( tp_identifier ) );
    }

    o.trans_identifier( source, result, NULL );
    o.trans_whitespace( source, result );
    if( source.parse_bl( "#?'('") )
    {
        return source.parse_error_fa( "Untraced member function '#<sc_t>'\n", o.nameof( tp_identifier ) );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/** Tests for typespec
 *  If possible, takes typespec from source, otherwise leaves source unchanged
 */
func (:s) er_t try_take_typespec
(
    m @* o,
    m x_source* source,
    m xoico_typespec_s* typespec,
    bl_t require_tractable_type,
    m bl_t* success
)
{
    if( success ) success.0 = false;

    sz_t index = source.get_index();

    o.trans_whitespace( source, NULL );
    tp_t tp_identifier = o.get_identifier( source, true );

    if( !tp_identifier )
    {
        source.set_index( index );
        return 0;
    }

    o.trans_whitespace( source, NULL );

    // take fails if identifier is used as function
    if( source.parse_bl( "#?'('" ) )
    {
        source.set_index( index );
        return 0;
    }

    tp_t access_class = 0;

    switch( tp_identifier )
    {
        case TYPEOF_c:
        case TYPEOF_const:
            access_class = TYPEOF_const;
            break;

        case TYPEOF_m:
        case TYPEOF_mutable:
            access_class = TYPEOF_mutable;
            break;

        case TYPEOF_d:
        case TYPEOF_discardable:
            access_class = TYPEOF_discardable;
            break;

        default: break;
    }

    if( access_class )
    {
        tp_identifier = o.get_identifier( source, true );
        if( !tp_identifier )
        {
            source.set_index( index );
            return 0;
        }
        o.trans_whitespace( source, NULL );
    }

    while( tp_identifier )
    {
        if     ( tp_identifier == TYPEOF_static   && !typespec.flag_static   ) typespec.flag_static   = true;
        else if( tp_identifier == TYPEOF_volatile && !typespec.flag_volatile ) typespec.flag_volatile = true;
        else if( tp_identifier == TYPEOF_aware    && !typespec.flag_aware    ) typespec.flag_aware    = true;
        else if( tp_identifier == TYPEOF_obliv    && !typespec.flag_obliv    ) typespec.flag_obliv    = true;
        else break;

        tp_identifier = 0;
        o.trans_identifier( source, NULL, tp_identifier );
        o.trans_whitespace( source, NULL );
    }

    typespec.type = tp_identifier;

    if( source.parse_bl( "#?'.' " ) )
    {
        if( !source.parse_bl( "#?([0]>='0'&&[0]<='9') " ) )
        {
            source.set_index( index );
            return 0;
        }

        sz_t indirection = 0;
        source.parse_fa( "#<sz_t*> ", indirection.1 );
        typespec.indirection = indirection;
    }
    else
    {
        while( source.parse_bl( "#?'*' " ) ) typespec.indirection++;
    }

    if( require_tractable_type )
    {
        if( !( tp_identifier == type_deduce~ || o.is_type( tp_identifier ) ) )
        {
            source.set_index( index );
            return 0;
        }
    }

    if( source.parse_bl( " #?'^'") ) typespec.flag_scope = true;

    if( source.parse_bl( " #?w'restrict'") ) typespec.flag_restrict = true;

    if( typespec.indirection > 0 && access_class == 0 )
    {
        access_class = TYPEOF_const; // const is default
    }

    typespec.access_class = access_class;

    if( success ) success.0 = true;
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t take_typespec
(
    m @* o,
    m x_source* source,
    m xoico_typespec_s* typespec,
    bl_t require_tractable_type
)
{
    bl_t success = false;
    o.try_take_typespec( source, typespec, require_tractable_type, success );

    if( !success )
    {
        if( require_tractable_type )
        {
            return source.parse_error_fa( "Typespec: Syntax error or intractable type." );
        }
        else
        {
            return source.parse_error_fa( "Typespec: Syntax error." );
        }
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.push_typespec
{
    tp_t type = typespec.type;

    if( type == type_object~ )
    {
        ERR_fa( "Cannot resolve 'type_object' at this point." );
    }
    else if( type == type_deduce~ )
    {
        ERR_fa( "Cannot resolve 'type_deduce' at this point." );
    }
    else if( type == 0 )
    {
        ERR_fa( "Type is 0." );
    }

    m st_s* st_type = st_s_create_sc( o.nameof( type ) )^^;

    if( st_type.size == 0 )
    {
        ERR_fa( "Type '#<tp_t>' has no name.", type );
    }

    sc_t sc_type = st_type.sc;
    if( typespec.flag_static   ) result.push_fa( "static " );
    if( typespec.access_class == TYPEOF_const ) result.push_fa( "const " );
    if( typespec.flag_volatile ) result.push_fa( "volatile " );
    result.push_fa( "#<sc_t>", sc_type );

    for( sz_t i = 0; i < typespec.indirection; i++ ) result.push_fa( "*" );
    if( typespec.flag_restrict ) result.push_fa( "restrict " );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_type
(
    m @* o,
    m x_source* source,
    m :result* result, // can be NULL
    m xoico_typespec_s* out_typespec // optional
)
{
    m $* result_local = :result_arr_s!^;
    tp_t tp_identifier;
    o.trans_identifier( source, result_local, tp_identifier );
    o.trans_whitespace( source, result_local );
    if( source.parse_bl( "#?'!'" ) )
    {
        if( o.is_group( tp_identifier ) )
        {
            return source.parse_error_fa( "Operator '!': lvalue is a group." );
        }
        result_local.clear();
        result_local.push_fa( "#<sc_t>_create()", o.nameof( tp_identifier ) );

        m xoico_typespec_s* typespec = xoico_typespec_s!^^;
        typespec.type = tp_identifier;
        typespec.indirection = 1;
        typespec.flag_addressable = false;
        typespec.access_class = TYPEOF_discardable;

        if( source.parse_bl( "#=?'^'" ) )
        {
            o.trans_builtin_scope( source, result_local, typespec, result, NULL );
            typespec.access_class = TYPEOF_mutable;
        }
        else
        {
            result.push_result_d( result_local.fork() );
        }

        o.trans_typespec_expression( source, result, typespec, out_typespec );
    }
    else if( source.parse_bl( "#?'~'" ) )
    {
        m xoico_typespec_s* typespec = xoico_typespec_s!^^;
        typespec.type = tp_t~;
        typespec.indirection = 0;
        typespec.flag_addressable = false;
        typespec.access_class = 0;
        result.push_fa( "((tp_t)(TYPEOF_#<sc_t>))", o.nameof( tp_identifier ) );
        o.trans_typespec_expression( source, result, typespec, out_typespec );
    }
    else
    {
        result.push_result_d( result_local.fork() );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_name
(
    m @* o,
    m x_source* source,
    m :result* result, // can be NULL
    m xoico_typespec_s* out_typespec // optional
)
{
    m $* result_local = :result_arr_s!^;
    tp_t tp_identifier;
    o.trans_identifier( source, result_local, tp_identifier );
    o.trans_whitespace( source, result_local );
    if( source.parse_bl( "#?'~'" ) )
    {
        m xoico_typespec_s* typespec = xoico_typespec_s!^^;
        typespec.type = tp_t~;
        typespec.indirection = 0;
        typespec.flag_addressable = false;
        typespec.access_class = 0;
        result.push_fa( "((tp_t)(TYPEOF_#<sc_t>))", o.nameof( tp_identifier ) );
        o.trans_typespec_expression( source, result, typespec, out_typespec );
    }
    else
    {
        result.push_result_d( result_local.fork() );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_ternary_branch
(
    m @* o,
    m x_source* source,
    m :result* result, // can be NULL
    m xoico_typespec_s* out_typespec // optional
)
{
    source.parse_fa( "?" );
    result.push_sc( "?" );
    m xoico_typespec_s* typespec_true  = xoico_typespec_s!^;
    m xoico_typespec_s* typespec_false = xoico_typespec_s!^;

    o.trans_expression( source, result, typespec_true );
    source.parse_fa( ": " );
    result.push_sc( ": " );
    o.trans_expression( source, result, typespec_false );

    if( typespec_true.type && typespec_false.type )
    {
        if( typespec_true.type != typespec_false.type )
        {
            source.parse_error_fa
            (
                "Ternary operator: Branches differ in type: ('#<sc_t>' : '#<sc_t>')",
                o.nameof( typespec_true.type ),
                o.nameof( typespec_false.type )
            );
        }

        if( typespec_true.indirection != typespec_false.indirection )
        {
            source.parse_error_fa
            (
                "Ternary operator: Branches differ in levels of indirection: ('#<sz_t>' : '#<sz_t>')",
                typespec_true.indirection,
                typespec_false.indirection
            );
        }
    }

    if( out_typespec && typespec_true.type )
    {
        out_typespec.copy( typespec_true );
        out_typespec.flag_addressable = false;
    }
    return 0;

};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_bracket
(
    m @* o,
    m x_source* source,
    m :result* result, // can be NULL
    m xoico_typespec_s* out_typespec // optional
)
{
    source.parse_fa( "(" );
    result.push_char( '(' );
    while( !source.eos() )
    {
        o.trans_expression( source, result, out_typespec );
        if     ( source.parse_bl( "#?')'" ) ) break;
        else if( source.parse_bl( "#?','" ) ) result.push_char( ',' );
        else return source.parse_error_fa( "Syntax error in bracket expression." );
        if( out_typespec ) out_typespec.type = 0;
    }
    result.push_char( ')' );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_array_subscript
(
    m @* o,
    m x_source* source,
    m :result* result, // can be NULL
    m xoico_typespec_s* out_typespec // optional
)
{
    source.parse_fa( "[" );
    result.push_sc( "[" );
    o.trans_expression( source, result, NULL );
    source.parse_fa( "]" );
    result.push_sc( "]" );
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_expression
(
    m @* o,
    m x_source* source,
    m :result* result_out, // can be NULL
    m xoico_typespec_s* out_typespec // optional
)
{
    sc_t sc_bl_end_of_expression = "#?([0]==';'||[0]=='{'||[0]=='}'||[0]==')'||[0]==']'||[0]==','||([0]=='.'&&[1]=='.')||([0]==':'&&([1]==' '||[1]=='\t'||[1]=='\n'||[1]=='/')))";

    o.trans_whitespace( source, result_out );

    m $* result = :result_arr_s!^;
    bl_t continuation = true;

    if( out_typespec ) out_typespec.reset();

    tp_t tp_identifier = o.get_identifier( source, false );

    if( tp_identifier )
    {
        // examples of builtin functions: scope, cast, try, fork
        if( o.is_builtin_func( tp_identifier ) )
        {
            m xoico_typespec_s* typespec_builtin = xoico_typespec_s!^^;
            o.trans_builtin( tp_identifier, source, NULL, NULL, result, typespec_builtin );
            o.trans_typespec_expression( source, result, typespec_builtin, out_typespec );
        }

        else if( tp_identifier == TYPEOF_verbatim_C )
        {
            source.parse_fa( "verbatim_C {" );
            o.trans_block_inside_verbatim_c( source, result );
            source.parse_fa( " }" );
        }

        // a control name marks end of expression...
        else if( o.is_control_name( tp_identifier ) )
        {
            continuation = false;
        }

        // identifier represents a boolean literal
        else if( tp_identifier == TYPEOF_true || tp_identifier == TYPEOF_false )
        {
            o.trans_identifier( source, result, NULL );
            o.trans_whitespace( source, result );
            m $* typespec = xoico_typespec_s!^^;
            typespec.type = bl_t~;
            typespec.indirection = 0;
            typespec.flag_addressable = false;
            o.trans_typespec_expression( source, result, typespec, out_typespec );
        }

        // identifier represents a registered variable
        else if( o.is_var( tp_identifier ) )
        {
            o.trans_identifier( source, result, NULL );
            o.trans_whitespace( source, result );
            c xoico_typespec_s* typespec_var = o.stack_var.get_typespec( tp_identifier );
            o.trans_typespec_expression( source, result, typespec_var, out_typespec );
        }

        // identifier represents a type known to the compiler
        else if( o.is_type( tp_identifier ) )
        {
            o.trans_type( source, result, out_typespec );
        }

        // identifier represents a type known to the compiler
        else if( o.is_name( tp_identifier ) )
        {
            o.trans_name( source, result, out_typespec );
        }

        // identifier represents a (global) function name
        else if( o.is_func( tp_identifier ) )
        {
            o.trans_identifier( source, NULL, NULL );
            sz_t source_index = source.get_index();

            if( source.parse_bl( " #=?'('" ) ) // actual function call
            {
                c xoico_func_s* func = o.get_func( tp_identifier );
                m $* typespec_ret = xoico_typespec_s!^^;
                o.trans_function( source, func, NULL, NULL, result, typespec_ret );
                o.trans_typespec_expression( source, result, typespec_ret, out_typespec );
            }
            else // function name used in untraced context
            {
                if( !o.waive_function_in_untraced_context ) return source.parse_error_fa( "Function #<sc_t> used in untraced context.\n.", o.nameof( tp_identifier ) );
                result.push_sc( o.nameof( tp_identifier ) );
                source.set_index( source_index );
                o.trans_whitespace( source, result );
            }
        }
        else // unknown/unspecified identifier
        {
            if( !o.waive_unknown_identifier )
            {
                if( !o.is_identifier( tp_identifier ) )
                {
                    sc_t sc_identifier = o.nameof( tp_identifier );
                    if( sc_t_cmp( "TYPEOF_", sc_identifier ) != 1 )
                    {
                        return source.parse_error_fa( "Unknwon identifier '#<sc_t>'\n.", o.nameof( tp_identifier ) );
                    }

                    sc_identifier += bcore_strlen( "TYPEOF_" );
                    if( !o.is_identifier( btypeof( sc_identifier ) ) )
                    {
                        return source.parse_error_fa( "Unknwon identifier '#<sc_t>'\n.", o.nameof( tp_identifier ) );
                    }
                }
            }
            o.trans_identifier( source, result, NULL );
            o.trans_whitespace( source, result );

            // assume untraced function call: bracket evaluation without internal type evaluation
            if( source.parse_bl( "#=?'('" ) )
            {
                o.trans_bracket( source, result, NULL );
            }
        }
    }

    // end of expression...
    else if( source.parse_bl( sc_bl_end_of_expression ) )
    {
        continuation = false;
    }

    // literals and members
    else if( source.parse_bl( "#?([0]>='0'&&[0]<='9')" ) ) o.trans_number_literal( source, result );
    else if( source.parse_bl( "#=?'\"'" )                ) o.trans_string_literal( source, result );
    else if( source.parse_bl( "#=?|'|" )                 ) o.trans_char_literal( source, result );
    else if( source.parse_bl( "#=?'.'" )                 ) o.trans_member( source, result );
    else if( source.parse_bl( "#=?'->'" )                ) o.trans_member( source, result );

    else if( source.parse_bl( "#=?'=<'" )                ) return source.parse_error_fa( "Attach operator: Expression not tractable." );

    else if( o.trans_inert_operator( source, result )       ) {} // inert operators are not interpreted by che and passed to the c-compiler

    // ternary branch operator
    else if( source.parse_bl( "#=?'?'" ) )
    {
        o.trans_ternary_branch( source, result, out_typespec );
    }

    // general bracket
    else if( source.parse_bl( "#=?'('" ) )
    {
        m xoico_typespec_s* typespec_local = xoico_typespec_s!^^;
        o.trans_bracket( source, result, typespec_local );
        if( typespec_local.type )
        {
            o.trans_typespec_expression( source, result, typespec_local, out_typespec );
        }
    }

    // array subscript
    else if( source.parse_bl( "#=?'['" ) )
    {
        o.trans_array_subscript( source, result, out_typespec );
    }

    // unhandled
    else
    {
        return source.parse_error_fa( "Syntax error after '#<sc_t>'", result.create_st()^^.sc );
    }

    if( continuation )
    {
        if( !source.parse_bl( sc_bl_end_of_expression ) )
        {
            if( out_typespec ) out_typespec.reset();


            o.trans_whitespace( source, result );
            o.trans_operator( source, result );


            o.trans_expression( source, result, NULL );
        }
    }

    if( result_out ) result_out.push_result_d( result.fork() );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/** Translates declaration including optional subsequent assignment.
 *  If no declaration was detected, source and result_out are not modified.
 */
func (:s) er_t try_trans_declaration
(
    m @* o,
    m x_source* source,
    m :result* result_out,
    m bl_t* success
)
{
    if( success ) success.0 = false;

    m xoico_typespec_s* typespec_var = xoico_typespec_s!^^;

    sz_t index = source.get_index();

    m $* result_var = :result_arr_s!^;

    bl_t success_take_typespec = false;
    o.try_take_typespec( source, typespec_var, true, success_take_typespec.1 );

    if( !success_take_typespec )
    {
        source.set_index( index );
        return 0;
    }

    o.trans_whitespace( source, result_var );

    if
    (
        typespec_var.type &&
        source.parse_bl( "#?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" )
    )
    {
        tp_t tp_identifier = 0;
        o.trans_identifier( source, result_var, tp_identifier.1 );
        o.trans_whitespace( source, result_var );

        if( source.parse_bl( "#?'='" ) ) // assignment
        {
            if( typespec_var.flag_scope ) return source.parse_error_fa( "Declaration-syntax: Stack-scope requested with subsequent assignment." );
            bl_t pushed_typedecl = false;
            if( typespec_var.type != type_deduce~ )
            {
                o.push_typedecl( typespec_var, tp_identifier );
                pushed_typedecl = true;
            }

            result_var.push_sc( "=" );
            m xoico_typespec_s* typespec_expr = xoico_typespec_s!^^;
            m $* result_expr = :result_arr_s!^^;
            o.trans_expression( source, result_expr, typespec_expr );

            if( typespec_var.type == type_deduce~ )
            {
                if( !typespec_expr.type )
                {
                    return source.parse_error_fa( "Declaration-syntax: Deduce requested but expression is intractable." );
                }
                typespec_var.type = typespec_expr.type;
            }

            if( typespec_expr.type )
            {
                if( typespec_expr.access_class != typespec_var.access_class )
                {
                    if( typespec_expr.access_class == TYPEOF_discardable )
                    {
                        return source.parse_error_fa( "Declaration-syntax: Assignment: Conversion discards typespec 'discardable'." );
                    }

                    if( typespec_var.access_class == TYPEOF_discardable )
                    {
                        return source.parse_error_fa( "Declaration-syntax: Assignment: Conversion requires typespec 'discardable'." );
                    }
                }
                o.adapt_expression( source, typespec_expr, typespec_var, result_expr, result_var );
            }
            else
            {
                result_var.push_result_d( result_expr.fork() );
            }
            o.push_typespec( typespec_var, result_out );
            result_out.push_char( ' ' );
            result_out.push_result_d( result_var.fork() );
            if( !pushed_typedecl ) o.push_typedecl( typespec_var, tp_identifier );
        }
        else if( typespec_var.type == type_deduce~ )
        {
            return source.parse_error_fa( "Declaration-syntax: Deduce requested without assignment." );
        }
        else if( source.parse_bl( "#=?'['" ) ) // c-style array
        {
            if( typespec_var.flag_scope ) return source.parse_error_fa( "Declaration-syntax: Stack-scope requested on C-style array." );
            o.push_typespec( typespec_var, result_out );
            while( source.parse_bl( "#?'['" ) )
            {
                result_var.push_char( '[' );
                o.trans_expression( source, result_var, NULL );
                source.parse_fa( "]" );
                result_var.push_char( ']' );
                typespec_var.indirection++;
            }
            result_out.push_char( ' ' );
            result_out.push_result_d( result_var.fork() );
            o.push_typedecl( typespec_var, tp_identifier );

            o.trans_whitespace( source, result_var );
            if( source.parse_bl( "#?'='" ) ) // c-style initialization
            {
                result_var.push_sc( "=" );
                o.trans_whitespace( source, result_var );
                o.trans_block( source, result_var, false );
            }
        }
        else
        {
            o.push_typespec( typespec_var, result_out );
            result_out.push_char( ' ' );
            result_out.push_result_d( result_var.fork() );
            o.push_typedecl( typespec_var, tp_identifier );

            if( typespec_var.flag_scope )
            {
                result_out.push_char( ';' );
                result_out.push_fa( "BLM_T_INIT_SPUSH(#<sc_t>, &#<sc_t>);", o.nameof( typespec_var.type ), o.nameof( tp_identifier ) );
                o.stack_block.adl.[ o.block_level ].use_blm = true;

                // debug
                if( !source.parse_bl( " #=?';'" ) )
                {
                    m $* result_local = :result_arr_s!^^;
                    result_local.push_fa( "#<sc_t>", o.nameof( tp_identifier ) );
                    o.trans_typespec_expression( source, result_local, typespec_var, NULL );
                    result_out.push_result_d( result_local.fork() );
                }
            }
        }

        // catch group variable at indirection 0
        if( typespec_var.indirection == 0 && o.is_group( typespec_var.type ) )
        {
            return source.parse_error_fa( "Declaration-syntax: Variable of group-type has indirection 0." );
        }

        if( success ) success.0 = true;
    }
    else // no declaration
    {
        source.set_index( index );
        return 0;
    }

    if( success ) success.0 = true;

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func(:s) er_t inspect_expression( m @* o, m x_source* source )
{
    source.parse_fa( "\?\?" );

    m $* st = st_s!^;
    m $* result_local = :result_arr_s!^;
    m xoico_typespec_s* typespec = xoico_typespec_s!^;
    source.parse_fa( " #until';' ", st );
    source.parse_fa( ";" );
    bcore_msg_fa( " \?? #<sc_t>;\n", st.sc );
    if( o.trans_expression( x_source_create_from_fa( "#<st_s*>;", st )^, result_local, typespec ) )
    {
        bcore_error_pop_to_sink( x_sink_stdout().cast( m x_sink* ) );
        bcore_msg_fa( "\n" );
    }
    else
    {
        if( st.size == 0 ) return source.parse_error_fa( "Variable name expected." );
        bcore_msg_fa( "--> #<sc_t>;\n", result_local.create_st()^.sc );

        if( typespec.type )
        {
            bcore_msg_fa( "Expression yields typespec:\n" );
            bcore_msg_fa( "  access_class : #<sc_t>\n",  ifnameof( typespec.access_class ) );
            bcore_msg_fa( "  type         : #<sc_t>\n", o.nameof( typespec.type ) );
            bcore_msg_fa( "  indirection  : #<sz_t>\n", typespec.indirection );
            bcore_msg_fa( "  flags        : " );
            if( typespec.flag_static   ) bcore_msg_fa( "static " );
            if( typespec.flag_volatile ) bcore_msg_fa( "volatile " );
            if( typespec.flag_restrict ) bcore_msg_fa( "restrict " );
            if( typespec.flag_obliv    ) bcore_msg_fa( "obliv " );
            if( typespec.flag_aware    ) bcore_msg_fa( "aware " );
            if( typespec.flag_scope    ) bcore_msg_fa( "scope " );
            if( typespec.flag_addressable ) bcore_msg_fa( "addressable " );
            if( typespec.flag_variadic ) bcore_msg_fa( "variadic " );
            bcore_msg_fa( "\n" );
        }
        else
        {
            bcore_msg_fa( "Expression does not yield a typespec.\n" );
        }
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_statement_expression( m @* o, m x_source* source, m :result* result )
{
    m$* result_statement = :result_statement_s!^( :result_arr_s! );

    if( o.try_block_level > 0 )
    {
        m xoico_typespec_s* typespec = xoico_typespec_s!^^;
        m $* result_expr = :result_arr_s!^^;
        o.trans_expression( source, result_expr, typespec );
        if
        (
            ( typespec.type == er_t~ ) &&
            ( typespec.indirection == 0 ) &&
            ( typespec.flag_addressable == false )
        )
        {
            if( !source.parse_bl( " #?';'" ) )
            {
                return source.parse_error_fa( "Inside a try-block: Expressions yielding 'er_t' must end with ';'" );
            }
            if( o.returns_er_t() )
            {
                result_statement.expression.push_sc( "BLM_TRY(" );
            }
            else
            {
                result_statement.expression.push_sc( "BLM_TRY_EXIT(" );
            }
            result_statement.expression.push_result_d( result_expr.fork() );
            result_statement.expression.push_sc( ")" );
        }
        else
        {
            result_statement.expression.push_result_d( result_expr.fork() );
        }
    }
    else
    {
        o.trans_expression( source, result_statement.expression, NULL );
    }

    o.trans_whitespace( source, result_statement.expression );

    result.push_result_d( result_statement.reduce().fork() );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_statement( m @* o, m x_source* source, m :result* result )
{
    o.trans_whitespace( source, result );

    bl_t handled = true;

    char c = source.inspect_char();
    switch( c )
    {
        case '{':
        {
            o.trans_block( source, result, false );
        }
        break;

        case '#':
        {
            o.trans_preprocessor( source, result );
        }
        break;

        case ';': /// consume semicolon (!)
        {
            source.get_char();
            result.push_char( ';' );
        }
        break;

        case ',':
        {
            source.get_char();
            result.push_char( ',' );
        }
        break;

        case '=': // completion statement (same as return)
        {
            o.trans_control( TYPEOF_completion, source, result );
        }
        break;

        case '?':
        {
            if( source.parse_bl( "#?([1]=='?')" ) )
            {
                o.inspect_expression( source );
            }
            else
            {
                handled = false;
            }
        }
        break;

        case ')':
        {
            return source.parse_error_fa( "Unmatched closing bracket." );
        }
        break;

        case '.':
        {
            return source.parse_error_fa( "Incorrect use of delimiter '.'." );
        }
        break;

        case '}':
        {
            // nothing (caller is a block-handler)
        }
        break;

        default:
        {
            handled = false;
        }
        break;
    }

    if( !handled )
    {
        tp_t tp_identifier = o.get_identifier( source, false );

        if( tp_identifier && o.is_control_name( tp_identifier ) )
        {
            o.trans_control( tp_identifier, source, result );
        }
        else
        {
            bl_t success_declaration = false;
            o.try_trans_declaration( source, result, success_declaration.1 );
            if( !success_declaration ) o.trans_statement_expression( source, result );
        }
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_block_inside( m @* o, m x_source* source, m :result* result_out )
{
    m $* result = :result_arr_s!^;

    s3_t source_index = source.get_index();

    while( !source.parse_bl( "#=?'}'" ) && !source.eos() )
    {
        o.trans_statement( source, result );

        s3_t index = source.get_index();
        if( index == source_index && !source.eos() )
        {
            return source.parse_error_fa( "Internal error: Statement translator did not progress." );
        }
        source_index = index;
    }

    if( o.stack_block_get_top_unit().use_blm )
    {
        m $* result_block = :result_block_s!( o.block_level, true )^^;

        result_block.push_result_d( :result_blm_init_s!( o.block_level ) );
        result_block.push_result_d( result.fork() );

        if( ( o.block_level > 0 ) || !o.returns_a_value() )
        {
            result_block.push_result_d( :result_blm_down_s! );
        }

        result_out.push_result_d( result_block.fork() );
    }
    else
    {
        result_out.push_result_d( result.fork() );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_block( m @* o, m x_source* source, m :result* result_out, bl_t is_break_ledge )
{
    o.inc_block();
    m $* result = :result_arr_s!^;
    o.stack_block_get_top_unit().break_ledge = is_break_ledge;
    o.trans_whitespace( source, result );
    o.trans( source, "{", result );
    o.trans_block_inside( source, result );
    o.trans_whitespace( source, result );
    o.trans( source, "}", result );
    result_out.push_result_d( result.fork() );
    o.dec_block();
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_statement_as_block( m @* o, m x_source* source, m :result* result_out, bl_t is_break_ledge )
{
    m $* result = :result_arr_s!^;

    o.inc_block();

    {
        m :stack_block_unit_s* top_unit = o.stack_block_get_top_unit();
        top_unit.break_ledge = is_break_ledge;
        top_unit.statement_wrapped_as_block = true;
    }

    o.trans_statement( source, result );
    o.trans_whitespace( source, result );
    if( source.parse_bl( "#?';'" ) ) result.push_sc( ";" );

    if( o.stack_block_get_top_unit().use_blm )
    {
        m $* result_block = :result_block_s!( o.block_level, true )^^;
        result_block.push_result_d( :result_blm_init_s!( o.block_level ) );
        result_block.push_result_d( result.fork() );
        result_block.push_result_d( :result_blm_down_s! );

        result_out.push_sc( "{" );
        result_out.push_result_d( result_block.fork() );
        result_out.push_sc( "}" );
    }
    else
    {
        result_out.push_result_d( result.fork() );
    }

    o.dec_block();
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t trans_block_inside_verbatim_c( m @* o, m x_source* source, m :result* result )
{
    o.has_verbatim_code = true;
    o.trans_whitespace( source, result );
    while( !source.parse_bl( "#=?'}'" ) && !source.eos() )
    {
        switch( source.inspect_char() )
        {
            case '"':
            {
                o.trans_string_literal( source, result );
            }
            break;

            case '\'':
            {
                o.trans_char_literal( source, result );
            }
            break;

            case '{':
            {
                result.push_char( source.get_char() );
                o.trans_block_inside_verbatim_c( source, result );
                source.parse_fa( "}" );
                result.push_char( '}' );
            }
            break;

            default:
            {
                result.push_char( source.get_char() );
            }
            break;
        }
        o.trans_whitespace( source, result );
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

/// function-level block
func (:s) er_t trans_level0_block( m @* o, bl_t exit_after_first_statement, m x_source* source, m :result* result_out )
{
    m $* result = :result_arr_s!^;

    s3_t source_index = source.get_index();

    while( !source.parse_bl( "#=?'}'" ) && !source.eos() )
    {
        o.trans_statement( source, result );

        if( exit_after_first_statement )
        {
            if( source.parse_bl( "#?';'" ) ) result.push_sc( ";" );
            break;
        }

        s3_t index = source.get_index();
        if( index == source_index && !source.eos() )
        {
            return source.parse_error_fa( "Internal error: Statement translator did not progress." );
        }
        source_index = index;
    }

    if( o.stack_block_get_top_unit().use_blm )
    {
        m $* result_block = :result_block_s!( o.block_level, true )^^;

        result_block.push_result_d( :result_blm_init_s!( o.block_level ) );
        result_block.push_result_d( result.fork() );

        if( !o.returns_a_value() )
        {
            result_block.push_result_d( :result_blm_down_s! );
        }

        result_out.push_result_d( result_block.fork() );
    }
    else
    {
        result_out.push_result_d( result.fork() );
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t setup( m @* o, c xoico_host* host, c xoico_signature_s* signature )
{
    o.signature =< signature.clone();
    o.signature.relent( host, host.obj_type() );

    o.host     = host.cast( m $* );
    o.compiler = host.compiler();
    o.block_level     = 0;
    o.try_block_level = 0;
    o.stack_var.clear();
    o.init_level0();

    if( o.signature.arg_o )
    {
        m $* unit = xoico_che_stack_var_unit_s!^^;
        tp_t tp_member_obj_name  = o.entypeof( "o" );

        unit.typespec.copy( o.signature.arg_o.typespec );
        unit.name = tp_member_obj_name;
        unit.level = o.block_level;

        if( !o.waive_unknown_type && !o.is_type( unit.typespec.type ) )
        {
            return signature.source_point.parse_error_fa( "Declaration: Argument '#<sc_t>' has an unknown type.", o.nameof( unit.name ) );
        }

        o.stack_var.push_unit( unit );
        o.hmap_name.set_sc( o.compiler.nameof( o.signature.arg_o.typespec.type ) );
        o.hmap_name.set_sc( o.compiler.nameof( tp_member_obj_name ) );
    }

    foreach( m $* arg in o.signature.args )
    {
        if( arg.typespec.type && arg.name )
        {
            m $* unit = xoico_che_stack_var_unit_s!^^;
            unit.typespec.copy( arg.typespec );
            unit.name = arg.name;
            unit.level = o.block_level;

            if( !o.waive_unknown_type && !o.is_type( unit.typespec.type ) )
            {
                return signature.source_point.parse_error_fa( "Declaration: Argument '#<sc_t>' has an unknown type.", o.nameof( unit.name ) );
            }

            o.stack_var.push_unit( unit );
            o.hmap_name.set_sc( o.compiler.nameof( arg.typespec.type ) );
            o.hmap_name.set_sc( o.compiler.nameof( arg.name ) );
        }
    }
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) sz_t assess_indentation( m x_source* source )
{
    sz_t index = source.get_index();
    while( !source.eos() ) if( source.get_char() == '\n' ) break;
    sz_t indentation = 0;
    while( !source.eos() )
    {
        if( source.get_char() != ' ' ) break;
        indentation++;
    }
    source.set_index( index );
    return indentation;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) void remove_indentation( m st_s* string, sz_t indentation )
{
    ASSERT( string.space >= string.size );

    sz_t j = 0;
    for( sz_t i = 0; i < string.size; i++ )
    {
        char c = string.[ i ];
        string.[ j++ ] = c;
        if( c == '\n' )
        {
            for( sz_t k = 0; k < indentation; k++ )
            {
                if( string.[ i + 1 ] != ' ' ) break;
                i++;
            }
        }
    }

    string.[ j ] = 0;
    string.size = j;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) er_t translate_mutable( m @* o, c xoico_host* host, c xoico_body_s* body, c xoico_signature_s* signature, m x_sink* sink )
{
    o.setup( host, signature );

    m x_source* source = body.code.source_point.clone_source()^^;

    m $* result = :result_arr_s!^;

    bl_t flag_verbatim_c = false;
    bl_t flag_try = o.for_all_functions_enable_try;

    if( source.parse_bl( " #?'('" ) )
    {
        while( !source.parse_bl( " #?')'" ) )
        {
            if( source.parse_bl( " #?w'try'" ) )
            {
                return source.parse_error_fa( "prefix (try) is deprecated." );
                //flag_try = true;
            }
            else if( source.parse_bl( " #?w'verbatim_C'" ) )
            {
                source.parse_error_fa( "prefix (verbatim_C) is deprecated." );
                flag_verbatim_c = true;
            }
            else
            {
                return source.parse_error_fa( "Invalid c-engine expression." );
            }
        }
    }

    sz_t indentation = 0;

    if( source.parse_bl( " #?'{'" ) )
    {
        if( !body.go_inline ) indentation = o.assess_indentation( source );

        if( flag_verbatim_c )
        {
            o.trans_block_inside_verbatim_c( source, result );
        }
        else
        {
            o.try_block_level += flag_try;
            o.trans_level0_block( false, source, result );
            o.try_block_level -= flag_try;
        }
        source.parse_fa( " }" );
    }
    else // single statement body
    {
        if( !body.go_inline ) indentation = o.assess_indentation( source );
        o.try_block_level += flag_try;
        o.trans_level0_block( true, source, result );
        o.try_block_level -= flag_try;
    }

    if( o.returns_a_value() && !o.has_completion && !o.has_verbatim_code )
    {
        if( o.signature.direct_return_arg )
        {
            result.push_result_d( :result_return_s!( o, :result_plain_s!( st_s!^.push_sc( o.nameof( o.signature.direct_return_arg.name ) ) ) ) );
        }
        else
        {
            o.signature.cast( x_btml* ).to_stdout();
            return source.parse_error_fa( "Function must return a value. Completion statement ('return' or '=') expected." );
        }
    }

    m $* result_block = :result_block_s!( o.block_level, o.stack_block_get_bottom_unit().use_blm )^^;
    result_block.cast( m :result_block_s* ).is_root = true;
    result_block.push_result_d( result.fork() );

    m st_s* buf = st_s!^^;
    result_block.to_sink( buf );

    //remove trailing whitespaces
    for( sz_t i = buf.size - 1; i >= 0; i-- )
    {
        if( buf.[ i ] != ' ' && buf.[ i ] != '\t' && buf.[ i ] != '\n' ) break;
        buf.[ i ] = 0;
        buf.size = i;
    }

    if( o.insert_source_reference && !body.code.single_line )
    {
        sink.push_fa( "// " );
        body.code.source_point.source_reference_to_sink( true, sink );
        sink.push_fa( "\n" );
    }

    if( indentation > 0 ) o.remove_indentation( buf, indentation );

    if( o.verbose )
    {
        x_sink_stdout().push_fa( "##############################\n" );
        x_sink_stdout().push_fa( "#<st_s*>\n", buf );
    }

    sink.push_sc( buf.sc );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico_cengine.translate
{
    er_t er = o.clone()^^.translate_mutable( host, body, signature, sink );
    return er;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

embed "xoico_che_trans_typespec.x";
embed "xoico_che_function.x";
embed "xoico_che_builtin.x";
embed "xoico_che_control.x";

/**********************************************************************************************************************/

