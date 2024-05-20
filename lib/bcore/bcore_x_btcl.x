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

/** Expression implementation */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :frame_s
{
    private :frame_s* parent;

    // object pool (hooks)
    hidden bcore_arr_sr_s obj_pool; // pool of objects with preserved lifetime
    func o preserve_and_set_weak( m@*o, m sr_s* sr )
    {
        o.obj_pool.push_sr( sr );
        sr.set_strong( false );
    }

    hidden bcore_hmap_name_s hmap_name;
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
        o.base.twd( base.o_type(), base.o );
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
        o.base.twd( base.o_type(), base.o );
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

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t negate( m@* o, m x_source* source, m sr_s* sr )
{
    o.error_if_undefined( source, sr );

    if( sr.is_integer() )
    {
        sr.const_from_s3( -sr.to_s3() );
        = 0;
    }

    if( sr.is_float() )
    {
        sr.const_from_f3( -sr.to_f3() );
        = 0;
    }

    if( sr.o_type() == :label_s~ ) = source.parse_error_fa( "'#<sc_t>' is not defined.\n", sr.o.cast( :label_s* ).st_name.sc );

    = source.parse_error_fa( "Negation of #<sc_t> is not defined.\n", bnameof( sr.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_member( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    o.error_if_undefined( source, sr );

    sr_s^ sb;
    o.eval( bop_priority, source, sb );

    :label_s* label = NULL;
    m x_stamp* stmp = sr.o.cast( m x_stamp* );

    tp_t stmp_t = sr.o_type();

    if( sb.o_type() == :label_s~ )
    {
        label = sb.o.cast( :label_s* );
    }
    else if( sb.o_type() == st_s~ )
    {
        label = :label_s!^^.setup( sb.o.cast( st_s* ).sc );
    }

    if( label )
    {
        if( stmp.t_exists( stmp_t, label.tp_name ) )
        {
            if( sr.is_strong() ) o.preserve_and_set_weak( sr );
            sr_s sc = stmp.t_m_get_sr( stmp_t, label.tp_name );

            if( sc.o )
            {
                sr.0 = sc;
            }
            else
            {
                sr.asd( :null_member_s!.setup( sr, label.tp_name ) );
            }
        }
        else
        {
            = source.parse_error_fa( "#<sc_t>.#<sc_t> does not exist.\n", bnameof( stmp_t ), label.st_name.sc );
        }
    }
    else
    {
        if( x_stamp_t_is_array( stmp_t ) )
        {
            m x_array* arry = stmp;
            if( sb.is_numeric() )
            {
                if( sr.is_strong() ) o.preserve_and_set_weak( sr );
                s3_t index = sb.to_s3();
                s3_t size = arry.t_size( stmp_t );
                if( index <     0 ) = source.parse_error_fa( "Array index '<s3_t>' < 0.\n", index );
                if( index >= size ) = source.parse_error_fa( "Array index '<s3_t>' >= array size '<s3_t>'.\n", index, size );
                sr_s sc = arry.t_m_get_sr( stmp_t, index );
                if( sc.o )
                {
                    sr.0 = sc;
                }
                else
                {
                    sr.asd( :null_arr_element_s!.setup( sr, index ) );
                }
            }
            else
            {
                = source.parse_error_fa( "Identifier or array subscript expected.\n" );
            }
        }
        else
        {
            = source.parse_error_fa( "Identifier expected.\n" );
        }
    }


    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_div( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    o.error_if_undefined( source, sr );
    sr_s^ sb; o.eval( bop_priority, source, sb );
    o.error_if_undefined( source, sb );

    if( bcore_tp_is_numeric( sr.o_type() ) && bcore_tp_is_numeric( sb.o_type() ) )
    {
        if( sb.to_f3() == 0 ) = source.parse_error_fa( "Division by zero.\n" );

        if( bcore_tp_is_integer( sr.o_type() ) && bcore_tp_is_integer( sb.o_type() ) )
        {
            sr.const_from_s3( sr.to_s3() / sb.to_s3() );
        }
        else
        {
            sr.const_from_f3( sr.to_f3() / sb.to_f3() );
        }
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> / #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_mul( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    o.error_if_undefined( source, sr );
    sr_s^ sb; o.eval( bop_priority, source, sb );
    o.error_if_undefined( source, sb );

    if( bcore_tp_is_numeric( sr.o_type() ) && bcore_tp_is_numeric( sb.o_type() ) )
    {
        if( bcore_tp_is_integer( sr.o_type() ) && bcore_tp_is_integer( sb.o_type() ) )
        {
            sr.const_from_s3( sr.to_s3() * sb.to_s3() );
        }
        else
        {
            sr.const_from_f3( sr.to_f3() * sb.to_f3() );
        }
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> * #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_sub( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    o.error_if_undefined( source, sr );
    sr_s^ sb; o.eval( bop_priority, source, sb );
    o.error_if_undefined( source, sb );

    if( bcore_tp_is_numeric( sr.o_type() ) && bcore_tp_is_numeric( sb.o_type() ) )
    {
        if( bcore_tp_is_integer( sr.o_type() ) && bcore_tp_is_integer( sb.o_type() ) )
        {
            sr.const_from_s3( sr.to_s3() - sb.to_s3() );
        }
        else
        {
            sr.const_from_f3( sr.to_f3() - sb.to_f3() );
        }
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> - #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_add( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    o.error_if_undefined( source, sr );
    sr_s^ sb; o.eval( bop_priority, source, sb );
    o.error_if_undefined( source, sb );

    if( bcore_tp_is_numeric( sr.o_type() ) && bcore_tp_is_numeric( sb.o_type() ) )
    {
        if( bcore_tp_is_integer( sr.o_type() ) && bcore_tp_is_integer( sb.o_type() ) )
        {
            sr.const_from_s3( sr.to_s3() + sb.to_s3() );
        }
        else
        {
            sr.const_from_f3( sr.to_f3() + sb.to_f3() );
        }
        = 0;
    }

    = source.parse_error_fa( "Operator #<sc_t> + #<sc_t> is not defined.\n", bnameof( sr.o_type() ), bnameof( sb.o_type() ) );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_join( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    o.error_if_undefined( source, sr );
    sr_s^ sb; o.eval( bop_priority, source, sb );
    o.error_if_undefined( source, sb );

    d :list_s* list = :list_s!;

    if( sr.is_strong() )
    {
        if( sr.type() == :list_s~ )
        {
            list.push_list_fork( sr.o.cast( m :list_s* ) );
        }
        else
        {
            list.push_fork( sr );
        }
    }
    else
    {
        if( sr.type() == :list_s~ )
        {
            list.push_list_clone( sr.o.cast( :list_s* ) );
        }
        else
        {
            list.push_clone( sr );
        }
    }

    if( sr.is_strong() )
    {
        if( sb.type() == :list_s~ )
        {
            list.push_list_fork( sb.o.cast( m :list_s* ) );
        }
        else
        {
            list.push_fork( sb );
        }
    }
    else
    {
        if( sb.type() == :list_s~ )
        {
            list.push_list_clone( sb.o.cast( :list_s* ) );
        }
        else
        {
            list.push_clone( sb );
        }
    }

    sr.asd( list );

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_assign( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    if( sr.is_const() ) = source.parse_error_fa( "Assignment to a const object.\n" );

    sr_s^ sb; o.eval( bop_priority, source, sb );
    o.error_if_undefined( source, sb );

    sb.set_const( true );

    switch( sr.o_type() )
    {
        case :null_member_s~:
        {
            m :null_member_s* null_member = sr.o.cast( m :null_member_s* );
            sr_s sr1 = null_member.set_sr( sb );
            sr.down();
            sr.0 = sr1;
        }
        break;

        case :null_arr_element_s~:
        {
            m :null_arr_element_s* null_arr_element = sr.o.cast( m :null_arr_element_s* );
            sr_s sr1 = null_arr_element.set_sr( sb );
            sr.down();
            sr.0 = sr1;
        }
        break;

        case :label_s~:
        {
            tp_t tp_var_name = sr.o.cast( :label_s* ).tp_name;
            //if( o.var_exists( tp_var_name ) ) = source.parse_error_fa( "Variable '#<sc_t>' has already been defined.\n", o.nameof( tp_var_name ) );

            sr_s var_sr = sr_clone( sr_cw( sb.0 ) );
            var_sr.set_const( false );
            m sr_s* sr_var = o.var_set( tp_var_name, var_sr );
            sr.down();
            sr.0 = sr_cw( sr_var );
        }
        break;

        default:
        {
            x_inst_t_copy_typed( sr.o, sr.o_type(), sb.o_type(), sb.o );
        }
        break;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_continuation( m@* o, s2_t bop_priority, m x_source* source, m sr_s* sr )
{
    sr.clear();
    o.eval( bop_priority, source, sr );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval( m@* o, s2_t priority, m x_source* source, m sr_s* obj )
{
    ASSERT( obj.o == NULL );
    // sign
    s3_t sign = 1;
    if     ( source.parse_bl( " #?'+'" ) ) sign =  1;
    else if( source.parse_bl( " #?'-'" ) ) sign = -1;

    // object
    if( source.parse_bl( " #?([0]>='0'&&[0]<='9')" ) ) // constant number
    {
        o.eval_number_literal( source, obj );
    }
    // identifier
    else if( source.parse_bl( " #?(([0]>='A'&&[0]<='Z')||([0]>='a'&&[0]<='z')||[0]=='_')" ) )
    {
        tp_t name = o.get_identifier( source, true );

        if( o.var_exists( name ) )
        {
            m sr_s* sr = o.var_get( name );
            obj.twd( sr.o_type(), sr.o );
        }
        else
        {
            obj.asd( :label_s!.setup( o.nameof( name ) ) );
        }
    }
    // btml object
    else if( source.parse_bl( " #=?'<'" ) )
    {
        tp_t o_type = 0;
        d x_inst* o_inst = x_btml_create_from_source_t( source, o_type.1 );
        if( !o_inst ) = TYPEOF_parse_error; // if o_inst == 0 the error stack should hold a descriptive message from x_btml_create_from_source_t
        obj.0 = sr_tsd( o_type, o_inst );
        obj.set_const( true );
    }
    else if( source.parse_bl( " #?'('" ) ) // bracket
    {
        o.eval( 0, source, obj );
        source.parse_fa( " )" );
    }
    else if( source.eos() ) // end of stream; valid condition for ending an expression
    {
        = 0;
    }
    else
    {
        = source.parse_error_fa( "Expression does not evaluate to an object.\n" );
    }

    /// Unary (postfix) Operators

    // sign
    if( sign == -1 ) o.negate( source, obj );

    /** Binary Operators
     *  Binary operators span a binary tree.
     *  bop_priority determines which operator takes the root position for each branch.
     */
    s2_t bop_priority = 1000; // > number of operators

    // operators must be parsed in descending order of priority

    if( priority >= bop_priority ) = 0;
    while( source.parse_bl( " #?'.'" ) ) o.eval_member( bop_priority, source, obj );
    bop_priority--;

    if( priority >= bop_priority ) = 0;
    while( source.parse_bl( " #?'/'" ) ) o.eval_div( bop_priority, source, obj );
    bop_priority--;

    if( priority >= bop_priority ) = 0;
    while( source.parse_bl( " #?'*'" ) ) o.eval_mul( bop_priority, source, obj );
    bop_priority--;

    if( priority >= bop_priority ) = 0;
    while( source.parse_bl( " #?'-'" ) ) o.eval_sub( bop_priority, source, obj );
    bop_priority--;

    if( priority >= bop_priority ) = 0;
    while( source.parse_bl( " #?'+'" ) ) o.eval_add( bop_priority, source, obj );
    bop_priority--;

    if( priority >= bop_priority ) = 0;
    while( source.parse_bl( " #?':'" ) ) o.eval_join( bop_priority, source, obj );
    bop_priority--;

    if( priority >= bop_priority ) = 0;
    while( source.parse_bl( " #?'='" ) ) o.eval_assign( bop_priority, source, obj );
    bop_priority--;

    if( priority >= bop_priority ) = 0;
    while( source.parse_bl( " #?';'" ) ) o.eval_continuation( bop_priority, source, obj );
    bop_priority--;

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func void selftest()
{
//    st_s^ txt.push_sc( "  6.0 + ( 2 - 3 ) * 2;" );
//    st_s^ txt.push_sc( "<bcodec_audio_codec_waw_param_s></>.loudness_mask = <bcodec_audio_codec_waw_param_loudness_mask_s></>" );
//    st_s^ txt.push_sc( "x = 3 | <bcodec_audio_codec_waw_param_s></>.signal_exponent = x | x = 5" );
//    st_s^ txt.push_sc( "<#file>  \"/home/johannes/dev/beth/data/bcodec/audio_codec/audio_codec_waw_120kbps.param.cfg\" </>.loudness_mask.[2]" );

//    st_s^ txt.push_sc( "1 : 2 : 3 : 4" );

    sc_t txt = " \
x = <bcodec_audio_codec_waw_param_s></>; \n\
x.signal_exponent = 3; \n\
x                      \n\
    ";


    m x_source* source = x_source_create_from_sc( txt )^;

    :frame_s^ frame;

    sr_s^ sr;

    if( frame.eval( 0, source, sr ) )
    {
        bcore_error_pop_all_to_stderr();
        return;
    }

    if( sr.o )
    {
        x_btml_t_to_stdout( sr.o, sr.o_type() );
    }

    ASSERT( true );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
