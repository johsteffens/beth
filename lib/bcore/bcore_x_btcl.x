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

/** BTCL: Beth text constructive language (interpreter) */

/**********************************************************************************************************************/
/// supportive functions

//----------------------------------------------------------------------------------------------------------------------

/// creates a strong clone in case sr is weak; const flag is preserved
func void clone_if_weak( m sr_s* sr )
{
    if( sr.is_weak() )
    {
        bl_t is_const = sr.is_const();
        sr.0 = sr_clone( sr );
        sr.set_const( is_const );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// reserved keywords
name if;
name else;
name self;
name func;
name true;
name false;

// The context is created in the root frame and shared across all child frames
stamp :context_s
{
    /// reserved keywors
    bcore_hmap_name_s hmap_reserved;

    func o setup( m@* o )
    {
        o.hmap_reserved.set_sc( "if" );
        o.hmap_reserved.set_sc( "else" );
        o.hmap_reserved.set_sc( "self" );
        o.hmap_reserved.set_sc( "func" );
        o.hmap_reserved.set_sc( "true" );
        o.hmap_reserved.set_sc( "false" );
    }

    func bl_t is_reserved( @* o, tp_t name ) = o.hmap_reserved.exists( name );
    func sc_t sc_reserved( @* o, tp_t name ) = o.hmap_reserved.get_sc( name );
    func er_t check_reserved( @* o, tp_t name, m x_source* source )
    {
        if( o.is_reserved( name ) ) = source.parse_error_fa( "#<sc_t> is a reserved keyword.\n", o.sc_reserved( name ) );
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------
// priority groups

func s2_t priority_a() = 5000; // element access, function/modifier call
func s2_t priority_b() = 4000; // unary prefix operators
func s2_t priority_c() = 3000; // most remaining binary operators
func s2_t priority_d() = 2000; // reserved
func s2_t priority_e() = 1000; // continuation

//----------------------------------------------------------------------------------------------------------------------

stamp :frame_s
{
    private :frame_s* parent;

    :context_s -> context;
    func bl_t is_reserved( @* o, tp_t name ) = o.context.is_reserved( name );
    func sc_t sc_reserved( @* o, tp_t name ) = o.context.sc_reserved( name );
    func er_t check_reserved( @* o, tp_t name, m x_source* source ) = o.context.check_reserved( name, source );


    /// if parent is NULL, this frame is the root frame
    func o setup_as_root( m@* o, m :context_s* context )
    {
        o.parent =< NULL;

        if( context )
        {
            o.context =< context.fork();
        }
        else
        {
            o.context =< :context_s!.setup();
        }
    }

    /// if parent is NULL, this frame is the root frame
    func o setup( m@* o, @* parent )
    {
        if( !parent ) ERR_fa( "No parent: call setup_as_root\n" );
        o.parent = parent.cast( m$* );
        o.context =< o.parent.context.fork();
    }

    // object pool (hooks)
    hidden bcore_arr_sr_s obj_pool; // pool of objects with preserved lifetime
    func o preserve_and_set_weak( m@*o, m sr_s* sr )
    {
        o.obj_pool.push_sr( sr );
        sr.set_strong( false );
    }

    bcore_hmap_name_s hmap_name;
    func tp_t entypeof( m@* o, sc_t name ) = o.hmap_name.set_sc( name );
    func sc_t nameof( @* o, tp_t type )
    {
        sc_t name = o.hmap_name.get_sc( type ); = name ? name : bnameof( type );
        if( !name && o.parent ) name = o.parent.nameof( type );
        if( !name ) name = bnameof( type );
        = name;
    };

    // variables
    hidden bcore_hmap_tp_sr_s var_map; // variable map
    func bl_t var_exists( @* o, tp_t name )
    {
         if( o.var_map.exists( name ) ) = true;
         = o.parent ? o.parent.var_exists( name ) : false;
    }

    func m sr_s* var_get( @* o, tp_t name )
    {
         m sr_s* sr = o.var_map.get( name );
         if( sr ) = sr;
         = o.parent ? o.parent.var_get( name ) : NULL;
    }

    func m sr_s* var_set( m@* o, tp_t name, sr_s sr )
    {
         = o.var_map.set( name, sr );
    }

}

//----------------------------------------------------------------------------------------------------------------------

/// label or identifier
stamp :label_s
{
    tp_t tp_name;
    st_s st_name;

    func o setup( m@* o, sc_t sc_name )
    {
        o.tp_name = btypeof( sc_name );
        o.st_name.copy_sc( sc_name );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// A null variable is a variable that is to be created in the assignment operator
stamp :null_variable_s { $ tp_t tp_name; }

//----------------------------------------------------------------------------------------------------------------------

/** A null member is a stamp member defined as reference and being NULL at the point of query.
 *  Normally this represents an error except when the expression stand left of an assignment.
 *  The null member allows assignments being regular binary operators.
 */
stamp :null_member_s
{
    sr_s base;
    tp_t tp_name;
    func o setup( m@* o, m sr_s* base, tp_t tp_name )
    {
        o.base.twm( base.o_type(), base.o );
        o.tp_name = tp_name;
    }

    func sr_s set_sr( m@* o, m sr_s* src )
    {
        x_stamp_t_set_sr( o.base.o.cast( m x_stamp* ), o.base.o_type(), o.tp_name, sr_cw( src ) );
        = x_stamp_t_m_get_sr( o.base.o.cast( m x_stamp* ), o.base.o_type(), o.tp_name );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** Null array element: Concept of a null member applied to arrays.
 */
stamp :null_arr_element_s
{
    sr_s base;
    s3_t index;
    func o setup( m@* o, m sr_s* base, tp_t index )
    {
        o.base.twm( base.o_type(), base.o );
        o.index = index;
    }

    func sr_s set_sr( m@* o, m sr_s* src )
    {
        x_array_t_set_sr( o.base.o.cast( m x_stamp* ), o.base.o_type(), o.index, sr_cw( src ) );
        = x_array_t_m_get_sr( o.base.o.cast( m x_stamp* ), o.base.o_type(), o.index );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// generic list (result of a join operation)
stamp :list_s
{
    bcore_arr_sr_s arr;

    func o push_clone( m@* o, c sr_s* a ) o.arr.push_sr( sr_clone( sr_cw( a ) ) );
    func o push_fork ( m@* o, m sr_s* a ) o.arr.push_sr( sr_fork ( sr_cw( a ) ) );

    func o push_list_clone( m@* o, c @* a ) foreach( c$*e in a.arr ) o.push_clone( e );
    func o push_list_fork ( m@* o, m @* a ) foreach( m$*e in a.arr ) o.push_fork ( e );
}

//----------------------------------------------------------------------------------------------------------------------

/** block: code enclosed in { .... }
 *  Note: A block should not own a lexical frame.
 *        If it is part of a function, it cannot know any lexical environment except the argument list, because
 *        a block can be constructed in an environment (e.g. function generating function)
 *        that may no longer be valid once the block is executed.
 *        If it is merely part of a branch and has a right to access the environment of the
 *        branch, the block is actually executed in the frame where it was defined, thus the lexical frame can be
 *        passed at the time of execution.
 */
stamp :block_s
{
    x_source_point_s source_point;
}

//----------------------------------------------------------------------------------------------------------------------

func (:block_s) er_t parse( m@* o, m :frame_s* frame, m x_source* source )
{
    source.parse_fa( " {" );

    o.source_point.setup_from_source( source );

    sz_t indent = 0;
    while( !source.eos() )
    {
        if( source.parse_bl( " #=?'}'" ) )
        {
            if( indent > 0  )
            {
                source.get_char();
                indent--;
            }
            else
            {
                break;
            }
        }
        else if( source.parse_bl( " #?'{'" ) )
        {
            indent++;
        }
        else if( source.parse_bl( " #=?'\"'" ) )
        {
            source.parse_fa( "#-string" );
        }
        else
        {
            source.get_char();
        }
    }

    source.parse_fa( " }" );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/** Evaluates the block in its own frame */
func (:block_s) er_t eval( @* o, c :frame_s* parent_frame, m sr_s* obj )
{
    m$* frame = :frame_s!^.setup( parent_frame );
    m x_source* source = o.source_point.source;
    s3_t index = source.get_index();
    source.set_index( o.source_point.index );

    er_t err = frame.eval( 0, source, obj );

    source.set_index( index );

    = err;
}

//----------------------------------------------------------------------------------------------------------------------

/// signature of a function: syntax func ( a, b, ... )   (a,b arbitrary identifiers for function arguments)
stamp :signature_s
{
    x_source_point_s source_point;

    // argument list
    bcore_arr_tp_s arg_list;
}

//----------------------------------------------------------------------------------------------------------------------

func (:signature_s) er_t parse( m@* o, m :frame_s* frame, m x_source* source )
{
    o.source_point.setup_from_source( source );

    source.parse_fa( " (" );

    while( !source.eos() && !source.parse_bl( " #=?')'" ) )
    {
        // identifier
        if( source.parse_bl( " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" ) )
        {
            tp_t name = frame.get_identifier( source, true );
            frame.check_reserved( name, source );
            o.arg_list.push( name );
        }
        else
        {
            = source.parse_error_fa( "Identifier expected.\n" );
        }

        if( !source.parse_bl( " #?','" ) ) break;
    }

    source.parse_fa( " )" );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// a function is a composition of signature and block
stamp :function_s
{
    :signature_s -> signature;
    :block_s -> block;

    func o setup( m@* o, m :signature_s* signature, m :block_s* block )
    {
        o.signature =< signature.fork();
        o.block =< block.fork();
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// descriptive error if sr is a wrapper for undefined objects
func (:frame_s) er_t error_if_undefined( @* o, m x_source* source, sr_s* sr )
{
    if( sr.o_type() == :label_s~            ) = source.parse_error_fa( "'#<sc_t>' is not defined.\n", sr.o.cast( :label_s* ).st_name.sc );
    if( sr.o_type() == :null_member_s~      ) = source.parse_error_fa( "Member '#<sc_t>' is NULL.\n", o.nameof( sr.o.cast( :null_member_s* ).tp_name ) );
    if( sr.o_type() == :null_arr_element_s~ ) = source.parse_error_fa( "Array element [#<s3_t>] is NULL.\n", o.nameof( sr.o.cast( :null_arr_element_s* ).index ) );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) tp_t get_identifier( m @* o, m x_source* source, bl_t take_from_source )
{
    tp_t tp_identifier = 0;
    if( source.parse_bl( " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" ) )
    {
        sz_t source_index = take_from_source ? 0 : source.get_index();
        m st_s* st_name = st_s!^;
        source.parse_fa( "#name", st_name );
        tp_identifier = o.entypeof( st_name.sc );
        if( !take_from_source ) source.set_index( source_index );
    }

    return tp_identifier;
};

//----------------------------------------------------------------------------------------------------------------------

/// parses number: (all integer, hex and float encodings); creates f3_t or s3_t depending on syntax; makes sr constant
func(:frame_s) er_t eval_number_literal( m @* o, m x_source* source, m sr_s* sr )
{
    bl_t is_hex = false;
    bl_t is_float = false;
    st_s^ st;

    if( source.parse_bl( "#?'0x'" ) )
    {
        st.push_sc( "0x" );
        is_hex = true;
    }
    else if( source.parse_bl( "#?'0X'" ) )
    {
        st.push_sc( "0X" );
        is_hex = true;
    }

    if( is_hex )
    {
        while( source.parse_bl( "#?(([0]>='0'&&[0]<='9')||([0]>='a'&&[0]<='f')||([0]>='A'&&[0]<='F'))" ) )
        {
            st.push_char( source.get_char() );
        }
    }
    else // decimal
    {
        while( source.parse_bl( "#?([0]>='0'&&[0]<='9')" ) ) st.push_char( source.get_char() );
    }

    if( !is_hex && source.parse_bl( "#?([0]=='.')" ) )
    {
        st.push_char( source.get_char() );
        while( source.parse_bl( "#?([0]>='0'&&[0]<='9')" ) ) st.push_char( source.get_char() );
        is_float = true;
    }

    bl_t exponent = false;

    if( !is_hex && source.parse_bl( "#?([0]=='e'||[0]=='E')" ) )
    {
        st.push_char( source.get_char() );
        exponent = true;
        is_float = true;
    }

    if( exponent )
    {
        if( source.parse_bl( "#?([0]=='+'||[0]=='-')" ) )
        {
            st.push_char( source.get_char() );
        }

        while( source.parse_bl( "#?([0]>='0'&&[0]<='9')" ) )
        {
            st.push_char( source.get_char() );
        }
    }

    if( is_float )
    {
        f3_t f3 = 0;
        st.parse_fa( 0, -1, "#<f3_t*>", f3.1 );
        sr.const_from_f3( f3 );
    }
    else
    {
        s3_t s3 = 0;
        st.parse_fa( 0, -1, "#<s3_t*>", s3.1 );
        sr.const_from_s3( s3 );
    }

    = 0;
};

//----------------------------------------------------------------------------------------------------------------------

/// parses number: (all integer, hex and float encodings); creates f3_t or s3_t depending on syntax; makes sr constant
func(:frame_s) er_t eval_condition( m @* o, m x_source* source, m bl_t* condition )
{
    m sr_s* sb = sr_s!^;
    o.eval( 0, source, sb );
    if( !sb.is_numeric() )  = source.parse_error_fa( "Internal error: Expression does not evaluate to a condition.\n" );
    if( condition ) condition.0 = ( sb.to_f3() != 0 );
    = 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// unary operators

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t negate( m@* o, m x_source* source, m sr_s* sr )
{
    o.error_if_undefined( source, sr );

    if( sr.is_integer() )
    {
        sr.const_from_s3( -sr.to_s3() );
    }
    else if( sr.is_float() )
    {
        sr.const_from_f3( -sr.to_f3() );
    }
    else
    {
        = source.parse_error_fa( "Negation of #<sc_t> is not defined.\n", bnameof( sr.o_type() ) );
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t logic_not( m@* o, m x_source* source, m sr_s* sr )
{
    o.error_if_undefined( source, sr );

    if( sr.is_numeric() )
    {
        sr.const_from_bl( !sr.to_bl() );
    }
    else
    {
        = source.parse_error_fa( "Negation of #<sc_t> is not defined.\n", bnameof( sr.o_type() ) );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t to_sink( @* o, bl_t detailed, sr_s* sr, m x_sink* sink )
{
    if( detailed )
    {
        x_btml_t_to_sink( sr.o.cast( x_btml* ), sr.o_type(), sink );
    }
    else
    {
        if     ( !sr.o ) sink.push_fa( "NULL" );
        else if( sr.is_integer() ) sink.push_fa( "#<s3_t>", sr.to_s3() );
        else if( sr.is_float() )   sink.push_fa( "#<f3_t>", sr.to_f3() );
        else if( sr.o_type() == st_s~ ) sink.push_fa( "#<sc_t>", sr.o.cast( st_s* ).sc );
        else x_btml_t_to_sink( sr.o.cast( x_btml* ), sr.o_type(), sink );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_in_frame( m@* o, s2_t priority, m x_source* source, m sr_s* obj )
{
    m$* frame = :frame_s!^.parent = o;
    frame.eval( priority, source, obj );
    :clone_if_weak( obj );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval( m@* o, s2_t exit_priority, m x_source* source, m sr_s* obj )
{
    ASSERT( obj.o == NULL );

    /// prefix operators
    if( source.parse_bl( " #?'+'" ) )
    {
        o.eval( :priority_c(), source, obj );
    }
    else if( source.parse_bl( " #?'?'" ) )
    {
        bl_t detail = source.parse_bl( " #?'?'" );
        o.eval( :priority_c(), source, obj );
        o.to_sink( detail, obj, x_sink_stdout() );
        x_sink_stdout().flush();
    }
    else if( source.parse_bl( " #?'-'" ) )
    {
        o.eval( :priority_c(), source, obj );
        o.negate( source, obj );
    }
    else if( source.parse_bl( " #?'!'" ) )
    {
        o.eval( :priority_c(), source, obj );
        o.logic_not( source, obj );
    }

    /// number literal
    else if( source.parse_bl( " #?([0]>='0'&&[0]<='9')" ) )
    {
        o.eval_number_literal( source, obj );
    }

    /// string literal
    else if( source.parse_bl( " #=?'\"'" ) )
    {
        source.parse_fa( " #cstring", obj.asm( st_s! ).o.cast( m st_s* ) );
    }

    /// Block { .... } definition
    else if( source.parse_bl( " #=?'{'" ) )
    {
        m :block_s* block = :block_s!^;
        block.parse( o, source );
        obj.asm( block.fork() );
    }

    /// Identifier
    else if( source.parse_bl( " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" ) )
    {
        tp_t name = o.get_identifier( source, true );

        /// reserved keyword
        if( o.is_reserved( name ) )
        {
            switch( name )
            {
                /// Signature or Function definition
                case TYPEOF_else: = source.parse_error_fa( "Misplaced 'else'.\n" );

                /// Signature or Function definition
                case TYPEOF_if:
                {
                    bl_t condition = false;
                    source.parse_fa( " (" );
                    o.eval_condition( source, condition );
                    source.parse_fa( " )" );
                    {
                        m :block_s* block = :block_s!^;
                        block.parse( o, source );
                        if( condition ) block.eval( o, obj );
                    }
                    if( source.parse_bl( " #?w'else'" ) )
                    {
                        m :block_s* block = :block_s!^;
                        block.parse( o, source );
                        if( !condition ) block.eval( o, obj );
                    }
                }
                break;

                /// Signature or Function definition
                case TYPEOF_func:
                {
                    m :signature_s* signature = :signature_s!^;
                    signature.parse( o, source );

                    if( source.parse_bl( " #=?'{'" ) ) // block follows signature -> function
                    {
                        m :block_s* block = :block_s!^;
                        block.parse( o, source );

                        m :function_s* function = :function_s!^.setup( signature, block );

                        obj.asm( function.fork() );
                    }
                    else
                    {
                        obj.asm( signature.fork() );
                    }
                }
                break;

                /// Recursion
                case TYPEOF_self:
                {
                    if( o.var_exists( TYPEOF_self ) )
                    {
                        m sr_s* sr = o.var_get( name );
                        if( sr.o_type() !=  :function_s~ ) = source.parse_error_fa( "Internal error: Keyword 'self': Not linked to a function.\n" );

                        // we fork the object so it can survive beyond a local frame
                        obj.tsc( sr.o_type(), bcore_fork( sr.o ) );
                    }
                    else
                    {
                        = source.parse_error_fa( "Keyword 'self': Used outside a function.\n" );
                    }
                }
                break;

                case TYPEOF_true:  obj.from_bl( true ); break;
                case TYPEOF_false: obj.from_bl( false ); break;

                default: = source.parse_error_fa( "Internal error: Keyword '#<sc_t>': missing implementation.\n", o.sc_reserved( name ) );
            }
        }
        else if( source.parse_bl( " #?([0]=='='&&[1]!='=')" ) ) // identifier with assignment --> variable declaration
        {
            obj.asm( :null_variable_s!( name ) );
        }
        else if( o.var_exists( name ) )
        {
            m sr_s* sr = o.var_get( name );

            // we fork the object so it can survive beyond a local frame
            obj.tsc( sr.o_type(), bcore_fork( sr.o ) );
        }
        else
        {
            = source.parse_error_fa( "Unknown identifier #<sc_t>.\n", o.nameof( name ) );
        }
    }

    /// BTML object
    else if( source.parse_bl( " #=?'<'" ) )
    {
        tp_t o_type = 0;
        d x_inst* o_inst = x_btml_create_from_source_t( source, o_type.1 );
        if( !o_inst ) = TYPEOF_parse_error; // if o_inst == 0 the error stack should hold a descriptive message from x_btml_create_from_source_t
        obj.0 = sr_tsc( o_type, o_inst );
    }

    /// Bracket: content evaluated in dedicated frame
    else if( source.parse_bl( " #?'('" ) )
    {
        o.eval_in_frame( 0, source, obj );
        source.parse_fa( " )" );
    }

    else
    {
        = source.parse_error_fa( "Expression does not evaluate to an object.\n" );
    }

    // binary operators if any
    o.eval_bop( exit_priority, source, obj );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func parse_create_object
{
    m$* frame = :frame_s!^.setup_as_root( NULL );
    m$* sr = sr_s!^;
    frame.eval( 0, source, sr );

    if( obj )
    {
        if( sr.o )
        {
            if( sr.is_strong() )
            {
                obj.tsm( sr.o_type(), bcore_fork( sr.o ) );
            }
            else
            {
                /** A weak reference might be an embedded member of an object.
                 *  We clone it here to be sure we don't get runtime issues.
                 *  If the object is not embedded, it could be forked ...
                 */
                obj.tsm( sr.o_type(), x_inst_t_clone( sr.o, sr.o_type() ) );
            }
        }
        else
        {
            obj.clear();
        }
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func void selftest()
{
//    st_s^ txt.push_sc( "  6.0 + ( 2 - 3 ) * 2;" );
//    st_s^ txt.push_sc( "<bcodec_audio_codec_waw_param_s></>.loudness_mask = <bcodec_audio_codec_waw_param_loudness_mask_s></>" );
//    st_s^ txt.push_sc( "x = 3 | <bcodec_audio_codec_waw_param_s></>.signal_exponent = x | x = 5" );
//    st_s^ txt.push_sc( "<#file>  \"/home/johannes/dev/beth/data/bcodec/audio_codec/audio_codec_waw_120kbps.param.cfg\" </>.loudness_mask.[2]" );

//    st_s^ txt.push_sc( "1 : 2 : 3 : 4" );

/*
    sc_t txt = " \
    x = <bcodec_audio_codec_waw_param_s></>; \
    y = x( .signal_exponent = 3 ); \
    y; \
    ";

*/

    sc_t txt = " \
    x = { 3 }; \
    (?x) \
    ";

    m x_source* source = x_source_create_from_sc( txt )^;

    m$* frame = :frame_s!^;
    m$* sr = sr_s!^;

    if( frame.eval( 0, source, sr ) )
    {
        bcore_error_pop_all_to_stderr();
        return;
    }

    ASSERT( true );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

embed "bcore_x_btcl_bop.x";

