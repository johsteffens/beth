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
/// List of operator types ordered in groups and descending priority

//----------------------------------------------------------------------------------------------------------------------

// Group A, binary
name member, frame;

// builtin functions
name exp, log, log2, log10, sin, cos, tan, tanh, sign, sqrt, abs, ceil, floor, max, min, ife, size;

// Group B, unary
name identity, neg, not, print, println, printx, assert, mkdir, rmdir, file_exists;

// Group C, unary
name pow, div, mod, chain, mul_dot_colon, mul_dot, mul_colon;
name mul, sub, add;
name equal, unequal, larger_equal, larger, smaller_equal, smaller;
name and, or;
name conditional;
name spawn, cat, shift_left, assign;

// Group E, binary
name continuation;

//----------------------------------------------------------------------------------------------------------------------

/// returns the operator symbol for given type
func sc_t operator_symbol( tp_t type )
{
    switch( type )
    {
        // Group A, binary
        case member~: = ".";
        case frame~:  = "(";

        // builtin functions
        case exp~:     = "EXP";
        case log~:     = "LOG";
        case log2~:    = "LOG2";
        case log10~:   = "LOG10";
        case sin~:     = "SIN";
        case cos~:     = "COS";
        case tan~:     = "TAN";
        case tanh~:    = "TANH";
        case sign~:    = "SIGN";
        case sqrt~:    = "SQRT";
        case abs~:     = "ABS";
        case ceil~:    = "CEIL";
        case floor~:   = "FLOOR";
        case max~:     = "MAX";
        case min~:     = "MIN";
        case ife~:     = "IFE";
        case size~:    = "SIZE";
        case print~:   = "PRINT";
        case println~: = "PRINTLN";
        case printx~:  = "PRINTX";
        case assert~:  = "ASSERT";
        case mkdir~:   = "MKDIR";
        case rmdir~:   = "RMDIR";
        case file_exists~: = "FILE_EXISTS";

        // Group B, unary
        case identity~: = "+";
        case neg~:  = "-";
        case not~:  = "!";

        // Group C, unary
        case pow~:           = "^";
        case div~:           = "/";
        case mod~:           = "%";
        case chain~:         = "**";
        case mul_dot_colon~: = "*.:";
        case mul_dot~:       = "*.";
        case mul_colon~:     = "*:";

        case mul~: = "*";
        case sub~: = "-";
        case add~: = "+";

        case equal~:         = "==";
        case unequal~:       = "!=";
        case larger_equal~:  = ">=";
        case larger~:        = ">";
        case smaller_equal~: = "<=";
        case smaller~:       = "<";

        case and~:         = "&";
        case or~:          = "|";
        case conditional~: = "?";
        case spawn~:       = "::";
        case cat~:         = ":";
        case shift_left~:  = "<<";
        case assign~:      = "=";

        // Group E, binary
        case continuation~: = ";";

        default: break;
    }

    ERR_fa( "Unhandled operator type '#<sc_t>'.", bnameof( type ) );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// supportive functions

func bl_t is_identifier( m x_source* source ) = source.parse_bl( " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" );

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

// copies sb into sr by first trying type conversion then using generic conversions
func er_t generic_copy( m sr_s* sr, c sr_s* sb )
{
    er_t copy_typed_err = x_inst_t_copy_typed( sr.o, sr.o_type(), sb.o_type(), sb.o );
    if( copy_typed_err == conversion_error~ )
    {
        if( sb.o_type() == :list_s~ )
        {
            m x_array* array = NULL;
            tp_t t_array = 0;
            if( x_array_t_is_array( sr.type() ) )
            {
                array = sr.o;
                t_array = sr.type();
            }
            else if( x_stamp_t_is_aware( sr.type() ) && ( array = sr.o.cast( m x_array_feature* ).m_get_wrapped_array() ) )
            {
                t_array = array._;
            }

            if( array )
            {
                :list_s* list = sb.o.cast( :list_s* );
                x_array_t_clear( array, t_array );
                for( sz_t i = 0; i < list.arr.size; i++ ) x_array_t_push_sr( array, t_array, sr_cw( list.arr.[ i ] ) );
                bcore_error_remove_last_if_of( conversion_error~ );
                copy_typed_err = 0;
            }
        }
    }
    = copy_typed_err;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// reserved keywords
name if;
name else;
name self;
name func;
name embed;
name eval_file;
name eval_string;
name embed_file;
name embed_string;
name prefix;

// The context is created in the root frame and shared across all child frames
stamp :context_s
{
    /// Search paths for embedding other files.
    bcore_arr_st_s => eval_path_arr;

    /// reserved keywords
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
        o.hmap_reserved_key.set_sc( "eval_file" );
        o.hmap_reserved_key.set_sc( "eval_string" );
        o.hmap_reserved_key.set_sc( "embed_file" );
        o.hmap_reserved_key.set_sc( "embed_string" );
        o.hmap_reserved_key.set_sc( "prefix" );
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

/// crates a usable path from an embedded file path
func (:context_s) er_t get_embedding_file_path( @* o, m x_source* source, sc_t in_path, m st_s* out_path )
{
    m$* try_path = st_s!^;

    if( in_path[ 0 ] == '/' )
    {
        try_path.copy_sc( in_path );
    }
    else
    {
        m st_s* folder = bcore_file_folder_path( source.get_file() )^;
        if( folder.size == 0 ) folder.push_char( '.' );
        try_path.copy_fa( "#<sc_t>/#<sc_t>", folder.sc, in_path );

        if( !bcore_file_exists( try_path.sc ) )
        {
            if( o.eval_path_arr )
            {
                foreach( $* st in o.eval_path_arr )
                {
                    try_path.copy_fa( "#<sc_t>/#<sc_t>", st.sc, in_path );
                    if( bcore_file_exists( try_path.sc ) ) break;
                }
            }
        }
    }

    if( !bcore_file_exists( try_path.sc ) )
    {
        = source.parse_error_fa( "embed: File '#<sc_t>' not found.", in_path );
    }

    out_path.copy( try_path );

    = 0;
};

//----------------------------------------------------------------------------------------------------------------------


/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// global constants

// frame depth limit: used to detect unlimited recursions
func sz_t max_frame_depth() = 0x500;

// frame eval depth limit: used to detect recursive evaluation structures
func sz_t max_frame_eval_depth() = 0x1000;

/// priority groups
func s2_t priority_a() = 50000; // element access, function/modifier call
func s2_t priority_b() = 40000; // unary prefix operators
func s2_t priority_c() = 30000; // most remaining binary operators
func s2_t priority_d() = 20000; // reserved
func s2_t priority_e() = 10000; // continuation

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :frame_s
{
    private :frame_s* parent;
    :context_s -> context;
    sz_t depth; // counts depth of parenting
    sz_t eval_depth; // counts depth of evaluation

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

    // adds variables from src_frame optionally applying '<prefix>_' to variable names
    func o import_forked_vars( m@* o, tp_t prefix, m@* src_frame )
    {
        sz_t size = src_frame.var_map.size;
        for( sz_t i = 0; i < size; i++ )
        {
            tp_t key = src_frame.var_map.idx_key( i );
            if( key )
            {
                tp_t name = key;
                if( prefix )
                {
                    sc_t prefix_name = o.nameof( prefix );
                    name = o.entypeof( st_s!^.copy_fa( "#<sc_t>_#<sc_t>", prefix_name, src_frame.nameof( key ) ).sc ) ;
                }
                c sr_s* src_sr = src_frame.var_map.idx_val( i );
                o.var_set( name, sr_tsm( src_sr.type(), src_sr.o.fork() ) );
            }
        }
    }

    // copies sb into sr by first trying type conversion then using generic conversions
    func er_t generic_copy( m@* o, x_source_point_s* sp, m sr_s* sr, c sr_s* sb )
    {
        /// convert functions into functor
        if( sb.type() == :function_s~ )
        {
            m$* function = sb.o.cast( m :function_s* );
            m$* functor = :functor_s!^;
            functor.setup( sp, function, o );
            sr_s functor_sr = sr_awc( functor );
            :generic_copy( sr, functor_sr );
        }
        else
        {
            :generic_copy( sr, sb );
        }
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// A null variable is a variable that is to be created in the assignment operator
stamp :null_variable_s { $ tp_t tp_name; }

//----------------------------------------------------------------------------------------------------------------------

/** A null member is a stamp member defined as reference and being NULL at the point of query.
 *  Normally this represents an error except when the expression stands left of an assignment.
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

    func sr_s set_sr( m@* o, m :frame_s* frame, x_source_point_s* sp, m sr_s* src )
    {
        uz_t index = x_stamp_t_index( o.base.o_type(), o.tp_name );

        if( x_stamp_t_is_static_i( o.base.o_type(), index ) && src.o )
        {
            tp_t dst_type = x_stamp_t_type_i( o.base.o, o.base.o_type(), index );
            sr_s sr = sr_tsm( dst_type, x_inst_t_create( dst_type ) );
            frame.generic_copy( sp, sr, src );
            x_stamp_t_set_sr( o.base.o, o.base.o_type(), o.tp_name, sr );
        }
        else
        {
            x_stamp_t_set_sr( o.base.o, o.base.o_type(), o.tp_name, sr_cw( src ) );
        }

        = x_stamp_t_m_get_sr( o.base.o, o.base.o_type(), o.tp_name );
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

    func sr_s set_sr( m@* o, m :frame_s* frame, x_source_point_s* sp, m sr_s* src )
    {
        if( x_array_t_is_static( o.base.o_type() ) && src.o )
        {
            tp_t dst_type = x_array_t_get_static_type( o.base.o_type() );
            sr_s sr = sr_tsm( dst_type, x_inst_t_create( dst_type ) );
            frame.generic_copy( sp, sr, src );
            x_array_t_set_sr( o.base.o.cast( m x_stamp* ), o.base.o_type(), o.index, sr );
        }
        else
        {
            x_array_t_set_sr( o.base.o.cast( m x_stamp* ), o.base.o_type(), o.index, sr_cw( src ) );
        }

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

    func o push_sr   ( m@* o, sr_s sr )   o.arr.push_sr( sr );
    func o push_clone( m@* o, c sr_s* a ) o.push_sr( sr_clone( sr_cw( a ) ) );
    func o push_fork ( m@* o, m sr_s* a ) o.push_sr( sr_fork ( sr_cw( a ) ) );

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
    hidden x_source_point_s source_point;
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
    hidden x_source_point_s source_point;

    // argument list
    bcore_arr_tp_s arg_list;

    func sz_t args( @* o ) = o.arg_list.size;
    func tp_t arg_name( @* o, sz_t index ) = o.arg_list.[ index ];

    func bl_t is_unary( @*o ) = ( o.arg_list.size == 1 );
}

//----------------------------------------------------------------------------------------------------------------------

func (:signature_s) er_t parse( m@* o, m :frame_s* frame, m x_source* source )
{
    o.source_point.setup_from_source( source );

    source.parse_fa( " (" );

    while( !source.eos() && !source.parse_bl( " #=?')'" ) )
    {
        // identifier
        if( :is_identifier( source ) )
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

/// a function of an external stamp via feature bctl_function
stamp :external_function_s
{
    tp_t name;
    bl_t is_mutable;
    : -> object;

    func o setup( m@* o, tp_t name, bl_t is_mutable, :* object )
    {
        o.name = name;
        o.is_mutable = is_mutable;
        o.object =< object.cast( m$* ).fork();
    }

    func er_t execute( m@* o, x_source_point_s* sp, m :frame_s* lexical_frame, bcore_arr_sr_s* args, m sr_s* sr )
    {
        sr.clear();
        if( o.is_mutable )
        {
            o.object.clone()^.m_btcl_function( o.name, sp, lexical_frame, args, sr );
        }
        else
        {
            o.object.btcl_function( o.name, sp, lexical_frame, args, sr );
        }
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// a function is a composition of signature and block
stamp :function_s
{
    :signature_s         -> signature;
    :block_s             -> block;
    :external_function_s -> external_function;
    :function_s          -> wrapped_function;  // wrapped function on partial call
    bcore_arr_sr_s       -> wrapped_arg_list;  // wrapped arguments on partial call

    :function_s => tail; // tail of a cain of function (the head is the function that defines the signature of the entire chain)

    func o setup( m@* o, m :signature_s* signature, m :block_s* block, c :function_s* tail )
    {
        o.signature =< signature.fork();
        o.block     =< block.fork();
        o.tail      =< tail.clone();
        o.external_function =< NULL;
    }

    func o setup_external_function( m@* o, tp_t name, s2_t arity, bl_t is_mutable, :* object )
    {
        o.signature =< :signature_s!;
        o.signature.arg_list.set_size( arity );
        for( sz_t i = 0; i < arity; i++ ) o.signature.arg_list.[ i ] = i + 1;
        o.external_function =< :external_function_s!.setup( name, is_mutable, object );
        o.block =< NULL;
        o.tail  =< NULL;
        = o;
    }

    func o setup_wrapped_function( m@* o, m :function_s* wrapped_function, m bcore_arr_sr_s* wrapped_arg_list )
    {
        o.wrapped_function =< wrapped_function.fork();
        o.wrapped_arg_list =< wrapped_arg_list.fork();

        m$* signature = :signature_s!^;
        for( sz_t i = wrapped_arg_list.size; i < o.wrapped_function.signature.arg_list.size; i++ )
        {
            signature.arg_list.push( o.wrapped_function.signature.arg_list.[ i ] );
        }

        o.signature =< signature.fork();
        = o;
    }

    func o append_tail( m@* o, c :function_s* tail )
    {
        if( !o.tail )
        {
            o.tail =< tail.clone();
        }
        else
        {
            o.tail.append_tail( tail );
        }
        = o;
    }

    func sz_t args( @* o ) = o.signature.args();
    func tp_t arg_name( @* o, sz_t index ) = o.signature.arg_name( index );

    func bl_t is_unary( @*o ) = o.signature.is_unary();
}

//----------------------------------------------------------------------------------------------------------------------

/// sets up frame for executing the function
func (:function_s) er_t setup_frame( m@* o, m :frame_s* lexical_frame, m x_source_point_s* source_point, m :frame_s* frame )
{
    if( lexical_frame.depth >= :max_frame_depth() ) = source_point.parse_error_fa( "Maximum frame depth (#<sz_t>) exceeded. Check for unguarded recursions.\n", :max_frame_depth() );
    frame.setup( lexical_frame );
    frame.var_set( TYPEOF_self, sr_asm( o.fork() ) );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// executes function in frame (variables are set in frame) from a list of arguments
func (:function_s) er_t call( m@* o, m x_source_point_s* source_point, m :frame_s* lexical_frame, m bcore_arr_sr_s* arg_list, m sr_s* result )
{
    :signature_s* signature = o.signature;
    if( arg_list.size < signature.arg_list.size )
    {
        result.asm( :function_s!.setup_wrapped_function( o, arg_list) );
        = 0;
    }
    else if( arg_list.size == signature.arg_list.size )
    {
        if( o.block )
        {
            m$* frame = :frame_s!^;
            o.setup_frame( lexical_frame, source_point, frame );
            for( sz_t i = 0; i < signature.arg_list.size; i++ ) frame.var_set( signature.arg_list.[ i ], sr_null() ).fork_from( arg_list.[ i ] );
            result.clear();
            o.block.eval( frame, result );
            :clone_if_weak( result );
        }
        else if( o.external_function )
        {
            o.external_function.execute( source_point, lexical_frame, arg_list, result );
        }
        else if( o.wrapped_function )
        {
            m $* full_arg_list = bcore_arr_sr_s!^;
            for( sz_t i = 0; i < o.wrapped_arg_list.size; i++ ) full_arg_list.push_sr( sr_null() ).fork_from( o.wrapped_arg_list.[ i ] );
            for( sz_t i = 0; i < arg_list.size          ; i++ ) full_arg_list.push_sr( sr_null() ).fork_from(           arg_list.[ i ] );
            o.wrapped_function.call( source_point, lexical_frame, full_arg_list, result );
        }
        else
        {
            source_point.parse_error_fa( "Internal error: Function is incomplete.\n" );
        }

        // chain of functions ...
        if( o.tail )
        {
            o.tail.call_unary( source_point, lexical_frame, result, result );
        }
        = 0;
    }
    else
    {
        source_point.parse_error_fa( "Too many arguments. Function accepts #<sz_t> argument#<sc_t>.\n", signature.arg_list.size, signature.arg_list.size != 1 ? "s" : "" );
    }
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:function_s) er_t call_unary( m@* o, m x_source_point_s* source_point, m :frame_s* lexical_frame, m sr_s* s_arg, m sr_s* result )
{
    m $* arg_list = bcore_arr_sr_s!^;
    arg_list.push_sr( sr_null() ).fork_from( s_arg );
    = o.call( source_point, lexical_frame, arg_list, result );
}

//----------------------------------------------------------------------------------------------------------------------

func (:function_s) er_t call_via_arg_list( m@* o, m x_source_point_s* source_point, m :frame_s* lexical_frame, m :list_s* arg_list1, m sr_s* result )
{
    m $* arg_list = bcore_arr_sr_s!^.set_size( arg_list1.size() );
    for( sz_t i = 0; i < arg_list.size; i++ ) arg_list.[ i ].fork_from( arg_list1.arr.[ i ] );
    = o.call( source_point, lexical_frame, arg_list, result );
}

//----------------------------------------------------------------------------------------------------------------------

func (:function_s) er_t call_via_evaluation( m@* o, m x_source* source, m :frame_s* lexical_frame, m sr_s* sr )
{
    m $* arg_list = bcore_arr_sr_s!^;
    while( !source.eos() && !source.parse_bl( " #=?')'" ) )
    {
        m$* sr_arg = sr_s!^;
        lexical_frame.eval( 0, source, sr_arg );
        arg_list.push_sr( sr_null() ).fork_from( sr_arg );
        if( !source.parse_bl( " #=?')'" ) ) source.parse_fa( " ," );
    }
    = o.call( x_source_point_s!^( source ), lexical_frame, arg_list, sr );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) tp_t get_identifier( m @* o, m x_source* source, bl_t take_from_source )
{
    tp_t tp_identifier = 0;
    if( :is_identifier( source ) )
    {
        sz_t source_index = take_from_source ? 0 : source.get_index();
        m st_s* st_name = st_s!^;
        source.parse_fa( " #name", st_name );
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

    f3_t factor = 1.0;
    bl_t use_suffix = true;

    char c = source.inspect_char();
    switch( c )
    {
        case 'd': factor = 1E-1;  break; // deci
        case 'c': factor = 1E-2;  break; // centi
        case 'm': factor = 1E-3;  break; // milli
        case 'u': factor = 1E-6;  break; // micro
        case 'n': factor = 1E-9;  break; // nano
        case 'p': factor = 1E-12; break; // pico
        case 'f': factor = 1E-15; break; // femto
        case 'a': factor = 1E-18; break; // atto
        case 'z': factor = 1E-21; break; // zepto
        case 'y': factor = 1E-24; break; // yocto
        case 'r': factor = 1E-27; break; // ronto
        case 'q': factor = 1E-30; break; // quecto

        case 'D': factor = 1E+1;  break; // deca
        case 'C': factor = 1E+2;  break; // cento
        case 'K': factor = 1E+3;  break; // kilo
        case 'M': factor = 1E+6;  break; // mega
        case 'G': factor = 1E+9;  break; // giga
        case 'T': factor = 1E+12; break; // tera
        case 'P': factor = 1E+15; break; // peta
        case 'X': factor = 1E+18; break; // exa
        case 'Z': factor = 1E+21; break; // zetta
        case 'Y': factor = 1E+24; break; // yotta
        case 'R': factor = 1E+27; break; // ronna
        case 'Q': factor = 1E+30; break; // quetta

        default: use_suffix = false; break;
    }

    if( use_suffix ) source.get_char();

    if( is_float )
    {
        f3_t f3 = 0;
        st.parse_fa( 0, -1, "#<f3_t*>", f3.1 );
        sr.const_from_f3( f3 * factor );
    }
    else
    {
        s3_t s3 = 0;
        st.parse_fa( 0, -1, "#<s3_t*>", s3.1 );
        if( use_suffix )
        {
            sr.const_from_f3( s3 * factor );
        }
        else
        {
            sr.const_from_s3( s3 );
        }
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
        else if( sr.type() == bl_t~ ) sink.push_fa( "#<sc_t>", sr.to_bl() ? "true" : "false" );
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
    o.eval_depth++;

    if( o.eval_depth >= :max_frame_eval_depth() ) = source.parse_error_fa( "Evaluation depth exceeded. Check for unguarded recursive expressions.\n" );

    ASSERT( obj.o == NULL );

    /// prefix operators
    if     ( source.parse_bl( " #?'+'" ) ) :operator_eval_uop_type( o, identity~, :priority_c(), source, false, obj );
    else if( source.parse_bl( " #?'-'" ) ) :operator_eval_uop_type( o, neg~,      :priority_c(), source, false, obj );
    else if( source.parse_bl( " #?'!'" ) ) :operator_eval_uop_type( o, not~,      :priority_c(), source, false, obj );

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

    /// labels (hashed string)
    else if( source.parse_bl( " #=?|'|" ) )
    {
        m$* st = st_s!^;
        source.parse_fa( " #label", st );
        obj.from_tp( bentypeof( st.sc ) );
    }

    /// Identifier
    else if( :is_identifier( source ) )
    {
        tp_t name = o.get_identifier( source, true );

        /// reserved keyword
        if( o.is_reserved_key( name ) )
        {
            switch( name )
            {
                case TYPEOF_else: = source.parse_error_fa( "Misplaced 'else'.\n" );

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

                        m :function_s* function = :function_s!^.setup( signature, block, NULL );

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
                case TYPEOF_embed_file:
                case TYPEOF_eval_file:
                {
                    m sr_s* sb = sr_s!^;
                    source.parse_fa( " (" );
                    o.eval( 0, source, sb );
                    source.parse_fa( " )" );
                    if( sb.type() != st_s~ ) = source.parse_error_fa( "Keyword 'eval_file': Expression must evaluate to a string, which can be used as file-path.\n" );
                    m st_s* path = st_s!^;
                    o.context.get_embedding_file_path( source, sb.o.cast( st_s* ).sc, path );
                    m x_source* emb_source = bcore_file_open_source( path.sc )^;
                    o.eval( 0, emb_source, obj );

                    emb_source.parse_fa( " " );
                    if( !emb_source.eos() ) emb_source.parse_error_fa( "Unexpected expression. Semicolon ';' missing on previous expression?\n" );
                }
                break;

                case TYPEOF_embed_string:
                case TYPEOF_eval_string:
                {
                    m sr_s* sb = sr_s!^;
                    source.parse_fa( " (" );
                    o.eval( 0, source, sb );
                    source.parse_fa( " )" );
                    if( sb.type() != st_s~ ) = source.parse_error_fa( "Keyword 'eval_string': Expression must evaluate to a string.\n" );
                    m x_source* emb_source = x_source_create_from_st( sb.o.cast( st_s* ) )^;
                    o.eval( 0, emb_source, obj );
                    emb_source.parse_fa( " " );
                    if( !emb_source.eos() ) emb_source.parse_error_fa( "Unexpected expression. Semicolon ';' missing on previous expression?\n" );
                }
                break;

                /// Prefixing
                case TYPEOF_prefix:
                {
                    source.parse_fa( " (" );

                    m$* result = sr_s!^;
                    m$* frame = :frame_s!^.setup( o );
                    frame.eval( 0, source, result );

                    m$* st_prefix = st_s!^;
                    tp_t prefix_name = 0;

                    if( result.type() == st_s~ )
                    {
                        st_prefix.copy( result.o.cast( st_s* ) );
                        if( st_prefix.size == 0 ) = source.parse_error_fa( "Keyword 'prefix': Prefix is the empty string.\n" );
                        prefix_name = o.entypeof( st_prefix.sc );
                    }
                    else if( result.type() == tp_t~ )
                    {
                        prefix_name = result.o.cast( tp_t* ).0;
                        sc_t sc_prefix = bnameof( prefix_name );
                        if( !sc_prefix ) = source.parse_error_fa( "Keyword 'prefix': Prefix identifier is tp_t without a registered name.\n" );
                        st_prefix.copy_sc( sc_prefix );
                    }
                    else
                    {
                        = source.parse_error_fa( "Keyword 'prefix': Prefix identifier must evaluate to a string or label.\n" );
                    }

                    if( st_prefix.find_any_sc( 0, st_prefix.size, " \t\r\n" ) < st_prefix.size )
                    {
                        = source.parse_error_fa( "Keyword 'prefix': Prefix contains whitespaces.\n" );
                    }

                    source.parse_fa( " ," );

                    result.clear();

                    frame.eval( 0, source, result );
                    :clone_if_weak( result );

                    o.import_forked_vars( prefix_name, frame );

                    if( result.o )
                    {
                        o.var_set( prefix_name, sr_tsm( result.type(), result.o.fork() ) );
                    }

                    source.parse_fa( " )" );
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
            o.eval_reserved_func( name, source, false, obj );
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
            if( i > 0 )
            {
                source.parse_fa( " ," );
                // a trailing comma is allowed on a non-empty list
                if( source.parse_bl( " #=?']'" ) ) break;
            }

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
        if( !o_inst ) = TYPEOF_parse_error; // if o_inst == 0 the error stack holds a descriptive message from x_btml_create_from_source_t
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

    /// Exportable object
    else if( source.parse_bl( " #?'@'" ) )
    {
        x_source_point_s* sp = x_source_point_s!^.setup_from_source( source );
        if( source.parse_bl( " #?'~'" ) ) // wire
        {
            tp_t rack_name = 0;
            if( :is_identifier( source ) )
            {
                rack_name = bcore_name_enroll( o.nameof( o.get_identifier( source, true ) ) );
            }

            tp_t wire_name = 0;
            if( source.parse_bl( " #?':' " ) )
            {
                if( !:is_identifier( source ) ) = source.parse_error_fa( "Socket identifier expected.\n" );
                wire_name = bcore_name_enroll( o.nameof( o.get_identifier( source, true ) ) );
            }

            obj.asm( :net_node_s!.setup_wire( rack_name, wire_name, sp ) );
        }
        else
        {
            tp_t node_type = 0; // generic
            if( source.parse_bl( " #?':'" ) ) node_type = rack~;
            tp_t node_name = 0;
            if( :is_identifier( source ) )
            {
                node_name = bcore_name_enroll( o.nameof( o.get_identifier( source, true ) ) );
            }

            obj.asm( :net_node_s!.setup( node_type, node_name, sp ) );
        }
    }
    else
    {
        = source.parse_error_fa( "Expression does not evaluate to an object.\n" );
    }

    // operators (if any)
    o.eval_op( exit_priority, source, obj );

    o.eval_depth--;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

/** On entering, obj should be sr_null
 *  In case of error obj need not be discarded
 *  if default_obj is defined, obj copies from default_obj before parsing the body
 */
func (:frame_s) er_t parse_create_final_object( m@* o, m x_source* source, m sr_s* obj )
{
    m$* sr = sr_s!^;
    o.eval( 0, source, sr );

    source.parse_fa( " " );

    if( !source.eos() )
    {
        = source.parse_error_fa( "Isolated expression: Operator or semicolon expected." );
    }

    if( obj )
    {
        if( sr.o )
        {
            /// convert function into functor
            if( sr.type() == :function_s~ )
            {
                m$* functor = :functor_s!^;
                functor.setup( x_source_point_s!^.setup_from_source( source ), sr.o.cast( m :function_s* ), o );
                sr.asm( functor.fork() );
            }

            if( sr.is_strong() )
            {
                obj.fork_from( sr );
            }
            else
            {
                /** A weak reference might be an embedded member of an object.
                 *  We clone it here to be sure we don't get runtime issues.
                 *  If the object is not embedded, it could be forked (s. above)
                 */
                obj.clone_from( sr );
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

func (:s) parse_create_object
{
    m$* context = :context_s!^.setup();
    context.eval_path_arr =< o.eval_path_arr.fork();
    m$* frame = :frame_s!^.setup_as_root( context );
    = frame.parse_create_final_object( source, obj );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

embed "bcore_x_btcl_builtin.x";
embed "bcore_x_btcl_op.x";

group :operator    { embed "bcore_x_btcl_operator.x";  }
group :operator_f3 { embed "bcore_x_btcl_operator_f3.x"; }
group :net         { embed "bcore_x_btcl_net.x";     }
group :functor     { embed "bcore_x_btcl_functor.x"; }
group :functor_f3  { embed "bcore_x_btcl_functor_f3.x"; }
group :random      { embed "bcore_x_btcl_random.x"; }
group :test        { embed "bcore_x_btcl_test.x"; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// for testing

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

