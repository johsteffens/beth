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

/** Creates a strong clone in case sr is weak; const flag is preserved
 *  This function should be used when the frame in which sr.o probably resides is
 *  about to disappear.
 */
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

/** Creates a strong clone in case sr is weak or at least twice referenced
 *  This function should be used when a copy of sr is intended but de facto not necessary
 *  because sr represents the only existing strong reference.
 */
func void clone_if_weak_or_twice_referenced( m sr_s* sr )
{
    if( sr.is_weak() || sr.references() >= 2 )
    {
        bl_t is_const = sr.is_const();
        sr.0 = sr_clone( sr );
        sr.set_const( is_const );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// crates a usable path from an embedded file path
func er_t get_embedding_file_path( m x_source* source, sc_t in_path, m st_s* out_path )
{
    m st_s* folder = bcore_file_folder_path( source.get_file() )^^;
    if( folder.size == 0 ) folder.push_char( '.' );

    if( in_path[ 0 ] == '/' )
    {
        out_path.copy_sc( in_path );
    }
    else
    {
        out_path.copy_fa( "#<sc_t>/#<sc_t>", folder.sc, in_path );
    }

    if( !bcore_file_exists( out_path.sc ) )
    {
        = source.parse_error_fa( "embed: File '#<sc_t>' not found.", out_path->sc );
    }

    = 0;
};


/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// reserved keywords
name if;
name else;
name self;
name func;
name embed;

// The context is created in the root frame and shared across all child frames
stamp :context_s
{
    /// reserved keywors
    bcore_hmap_name_s hmap_reserved_key;
    bcore_hmap_name_s hmap_reserved_func;
    bcore_hmap_name_s hmap_reserved_const;

    func o setup( m@* o )
    {
        o.set_reserved_keys();
        o.set_reserved_funcs();
        o.set_reserved_consts();
    }

    func o set_reserved_funcs( m@* o );
    func o set_reserved_consts( m@* o );
    func o set_reserved_keys( m@* o )
    {
        o.hmap_reserved_key.set_sc( "if" );
        o.hmap_reserved_key.set_sc( "else" );
        o.hmap_reserved_key.set_sc( "self" );
        o.hmap_reserved_key.set_sc( "func" );
        o.hmap_reserved_key.set_sc( "embed" );
    }


    func bl_t is_reserved_key( @* o, tp_t name ) = o.hmap_reserved_key.exists( name );
    func bl_t is_reserved_func( @* o, tp_t name ) = o.hmap_reserved_func.exists( name );
    func bl_t is_reserved_const( @* o, tp_t name ) = o.hmap_reserved_const.exists( name );

    func sc_t sc_reserved_key( @* o, tp_t name ) = o.hmap_reserved_key.get_sc( name );
    func sc_t sc_reserved_func( @* o, tp_t name ) = o.hmap_reserved_func.get_sc( name );
    func sc_t sc_reserved_const( @* o, tp_t name ) = o.hmap_reserved_const.get_sc( name );

    func bl_t is_reserved( @* o, tp_t name )
    {
        = o.is_reserved_key( name ) ||
        o.is_reserved_func( name ) ||
        o.is_reserved_const( name );
    }

    func sc_t sc_reserved( @* o, tp_t name )
    {
        if( o.is_reserved_key( name ) ) = o.sc_reserved_key( name );
        if( o.is_reserved_func( name ) ) = o.sc_reserved_func( name );
        if( o.is_reserved_const( name ) ) = o.sc_reserved_const( name );
        = NULL;
    }

    func er_t check_reserved( @* o, tp_t name, m x_source* source )
    {
        if( o.is_reserved( name ) ) = source.parse_error_fa( "#<sc_t> is a reserved keyword or function.\n", o.sc_reserved( name ) );
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------
/// global constants

// frame depth limit: used to detect unlimited recursions
func sz_t max_frame_depth() = 0x1000;

/// priority groups
func s2_t priority_a() = 50000; // element access, function/modifier call
func s2_t priority_b() = 40000; // unary prefix operators
func s2_t priority_c() = 30000; // most remaining binary operators
func s2_t priority_d() = 20000; // reserved
func s2_t priority_e() = 10000; // continuation

//----------------------------------------------------------------------------------------------------------------------

stamp :frame_s
{
    private :frame_s* parent;
    :context_s -> context;
    sz_t depth;

    func bl_t is_reserved_key( @* o, tp_t name ) = o.context.is_reserved_key( name );
    func sc_t sc_reserved_key( @* o, tp_t name ) = o.context.sc_reserved_key( name );
    func bl_t is_reserved_func( @* o, tp_t name ) = o.context.is_reserved_func( name );
    func sc_t sc_reserved_func( @* o, tp_t name ) = o.context.sc_reserved_func( name );
    func bl_t is_reserved_const( @* o, tp_t name ) = o.context.is_reserved_const( name );
    func sc_t sc_reserved_const( @* o, tp_t name ) = o.context.sc_reserved_const( name );

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
        if( !parent ) ERR_fa( "No parent: Call setup_as_root\n" );
        o.parent = parent.cast( m$* );
        o.context =< o.parent.context.fork();
        o.depth = o.parent.depth + 1;
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
        if( !name && o.context.is_reserved( type ) ) name = o.context.sc_reserved( type );
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

    func sz_t size( c@* o ) = o.arr.size;
    func o set_size( m@* o, sz_t size ) o.arr.set_size( size );

    func o push_clone( m@* o, c sr_s* a ) o.arr.push_sr( sr_clone( sr_cw( a ) ) );
    func o push_fork ( m@* o, m sr_s* a ) o.arr.push_sr( sr_fork ( sr_cw( a ) ) );

    func o push_list_clone( m@* o, c @* a ) foreach( c$*e in a.arr ) o.push_clone( e );
    func o push_list_fork ( m@* o, m @* a ) foreach( m$*e in a.arr ) o.push_fork ( e );


    func er_t to_sink( @* o, bl_t detailed, m x_sink* sink )
    {
        sink.push_fa( "[" );
        if( detailed ) sink.push_fa( "\n" );
        for( sz_t i = 0; i < o.arr.size; i++ )
        {
            :to_sink( detailed, o.arr.[ i ], sink );
            if( i < o.arr.size - 1 )
            {
                sink.push_fa( "," );
                if( detailed ) sink.push_fa( "\n" );
            }
        }
        sink.push_fa( "]" );
        if( detailed ) sink.push_fa( "\n" );
        = 0;
    }

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
    m x_source* source = o.source_point.source;
    s3_t index = source.get_index();
    source.set_index( o.source_point.index );
    parent_frame.eval_in_frame( 0, source, obj );
    source.parse_fa( " }" );
    source.set_index( index );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// signature of a function: syntax func ( a, b, ... )   (a,b arbitrary identifiers for function arguments)
stamp :signature_s
{
    x_source_point_s source_point;

    // argument list
    bcore_arr_tp_s arg_list;

    func sz_t args( @* o ) = o.arg_list.size;
    func tp_t arg_name( @* o, sz_t index ) = o.arg_list.[ index ];
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

    func sz_t args( @* o ) = o.signature.args();
    func tp_t arg_name( @* o, sz_t index ) = o.signature.arg_name( index );
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

func er_t to_sink( bl_t detailed, sr_s* sr, m x_sink* sink )
{
    if( detailed )
    {
        if( sr.o_type() == :list_s~ ) sr.o.cast( :list_s* ).to_sink( detailed, sink );
        else x_btml_t_to_sink( sr.o.cast( x_btml* ), sr.o_type(), sink );
    }
    else
    {
        if     ( !sr.o ) sink.push_fa( "NULL" );
        else if( sr.is_integer() ) sink.push_fa( "#<s3_t>", sr.to_s3() );
        else if( sr.is_float() )   sink.push_fa( "#<f3_t>", sr.to_f3() );
        else if( sr.o_type() == st_s~ ) sink.push_fa( "#<sc_t>", sr.o.cast( st_s* ).sc );
        else if( sr.o_type() == :list_s~ ) sr.o.cast( :list_s* ).to_sink( detailed, sink );
        else x_btml_t_to_sink( sr.o.cast( x_btml* ), sr.o_type(), sink );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// main evaluation

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_in_frame( @* o, s2_t priority, m x_source* source, m sr_s* obj )
{
    m$* frame = :frame_s!^.setup( o );
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

    /// Identifier
    else if( source.parse_bl( " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" ) )
    {
        tp_t name = o.get_identifier( source, true );

        /// reserved keyword
        if( o.is_reserved_key( name ) )
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

                /// Embedding
                case TYPEOF_embed:
                {
                    m sr_s* sb = sr_s!^;
                    source.parse_fa( " (" );
                    o.eval( 0, source, sb );
                    source.parse_fa( " )" );
                    if( sb.type() != st_s~ )  = source.parse_error_fa( "Keyword 'embed': Expression must evaluate to a string.\n" );
                    m st_s* path = st_s!^;
                    :get_embedding_file_path( source, sb.o.cast( st_s* ).sc, path );
                    m x_source* emb_source = bcore_file_open_source( path.sc )^;
                    o.eval( 0, emb_source, obj );
                }
                break;

                default: = source.parse_error_fa( "Internal error: Keyword '#<sc_t>': missing implementation.\n", o.sc_reserved( name ) );
            }
        }
        else if( o.is_reserved_const( name ) )
        {
            o.eval_reserved_const( name, source, obj );
        }
        else if( o.is_reserved_func( name ) )
        {
            source.parse_fa( " (" );
            o.eval_reserved_func( name, source, obj );
            source.parse_fa( " )" );
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

    /// array literal
    else if( source.parse_bl( " #?'['" ) )
    {
        m :list_s* list = :list_s!^;
        for( sz_t i = 0; !source.eos() && !source.parse_bl( " #=?']'" ); i++ )
        {
            if( i > 0 ) source.parse_fa( " ," );
            m sr_s* sr = sr_s!^;
            o.eval( 0, source, sr );
            if( sr.is_strong() )
            {
                list.push_fork( sr );
            }
            else
            {
                list.push_clone( sr );
            }
        }
        source.parse_fa( " ]" );

        obj.tsc( :list_s~, list.fork() );
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

    /// Block { .... } definition
    else if( source.parse_bl( " #=?'{'" ) )
    {
        m :block_s* block = :block_s!^;
        block.parse( o, source );
        obj.asm( block.fork() );
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

func void selftest( sc_t file )
{
    m$* obj = sr_s!^;
    :parse_create_object( x_source_check_create_from_file( file )^, obj );
    ASSERT( obj.o );
    ASSERT( obj.is_numeric() );
    ASSERT( obj.to_s3() == 0 );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

embed "bcore_x_btcl_builtin.x";
embed "bcore_x_btcl_bop.x";

