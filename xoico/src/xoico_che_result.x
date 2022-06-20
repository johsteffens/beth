/** Author and Copyright 2022 Johannes Bernhard Steffens
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

feature o clear( m @* o )                         =o;
feature er_t push_char( m @* o, char c )          { ERR_fa( "Not implemented." ); =0; }
feature er_t push_sc( m @* o, sc_t sc )           { ERR_fa( "Not implemented." ); =0; }
feature er_t push_st( m @* o, c st_s* st )        { ERR_fa( "Not implemented." ); =0; }
feature m :* push_result_c( m @* o, c :* result ) { ERR_fa( "Not implemented." ); =NULL; }
feature m :* push_result_d( m @* o, d :* result ) { ERR_fa( "Not implemented." ); =NULL; }
feature o activate(   m @* o )                    { ERR_fa( "Not implemented." ); =NULL; }
feature o deactivate( m @* o )                    { ERR_fa( "Not implemented." ); =NULL; }
feature er_t to_sink( c @* o, m x_sink* sink );

feature void set_parent_block( m @* o, m :block_s* parent ) {}

// returns true in case result represents a cast and sets cast accordingly (pp_cast can be NULL)
feature bl_t get_cast( m @* o, m :cast_s.2 pp_cast ) { if( pp_cast ) pp_cast.1 = NULL; = false; }

feature d st_s* create_st( c @* o )
{
    d $* st = st_s!;
    o.to_sink( st );
    = st;
}

//----------------------------------------------------------------------------------------------------------------------

stamp :whitespace_s( d st_s* st ) func :.to_sink { sink.push_st( o.st ); = 0; }

//----------------------------------------------------------------------------------------------------------------------

stamp :plain_s
{
    $ st_s st;

    func d @* create_from_st(   c st_s* st ) { d $* o = @!; o.st.copy( st ); =o; }
    func d @* create_from_st_d( d st_s* st ) { d $* o = @!; o.st.copy( st ); st.discard(); =o; }
    func d @* create_from_sc(     sc_t  sc ) { d $* o = @!; o.st.copy_sc( sc ); =o; }

    func :.clear     { o.st.clear(); =o; }
    func :.push_char { o.st.push_char( c );  = 0; }
    func :.push_sc   { o.st.push_sc( sc );   = 0; }
    func :.push_st   { o.st.push_st( st );   = 0; }
    func :.to_sink   { sink.push_st( o.st ); = 0; }
    func :.create_st { = o.st.clone(); }
}

func d :* create_from_st( c st_s* st ) { d $* o = :arr_s!; o.push_st( st ); =o; }
func d :* create_from_sc(   sc_t  sc ) { d $* o = :arr_s!; o.push_sc( sc ); =o; }

//----------------------------------------------------------------------------------------------------------------------

stamp :adl_s x_array { aware : -> []; } // !! weak links !!  (if this causes problems, revert to strong links)
stamp :arr_s
{
    :adl_s adl;
    bl_t active = true;

    func :.clear      o.adl.clear();
    func :.activate   o.active = true;
    func :.deactivate o.active = false;

    func m :* last(       m @* o ) = ( o.adl.size == 0 )         ? o.adl.push_d( :plain_s! ) : o.adl.[ o.adl.size - 1 ];
    func m :* last_plain( m @* o ) = ( o.last()._ != :plain_s~ ) ? o.adl.push_d( :plain_s! ) : o.adl.[ o.adl.size - 1 ];

    func :.push_char     = o.last_plain().push_char( c );
    func :.push_sc       = o.last_plain().push_sc( sc );
    func :.push_st       = o.last_plain().push_st( st );
    func :.push_result_d = o.adl.push_d( result );
    func :.push_result_c = o.adl.push_c( result );

    func :.to_sink
    {
        if( o.active ) foreach( m $* e in o.adl ) e.to_sink( sink );
        =0;
    }

    func :.set_parent_block
    {
        foreach( m $* e in o.adl ) e.set_parent_block( parent );
    }

    func :.get_cast
    {
        foreach( m $* e in o.adl ) if( e._ != :whitespace_s~ ) = e.get_cast( pp_cast );
        if( pp_cast ) pp_cast.1 = NULL;
        = false;
    }
}

//----------------------------------------------------------------------------------------------------------------------

stamp :block_s( sz_t level, bl_t is_using_blm )
{
    :arr_s arr;
    bl_t is_root = false;
    hidden @* parent;
    func :.clear       o.arr.clear();
    func :.push_char = o.arr.push_char( c );
    func :.push_sc   = o.arr.push_sc( sc );
    func :.push_st   = o.arr.push_st( st );

    func :.push_result_d
    {
        m :* result_pushed = o.arr.push_result_d( result );
        result_pushed.set_parent_block( o );
        =result_pushed;
    }

    func :.push_result_c
    {
        m :* result_pushed = o.arr.push_result_c( result );
        result_pushed.set_parent_block( o );
        =result_pushed;
    }

    func :.to_sink { =o.arr.to_sink( sink ); }
    func :.set_parent_block { o.parent = parent; }

    func bl_t is_using_blm_until_level( c @* o, sz_t level )
    {
        if( level > o.level ) = false;
        if( o.is_using_blm )  = true;
        if( o.is_root )       = false;

        ASSERT( o.parent );
        = o.parent.is_using_blm_until_level( level );
    }
}

//----------------------------------------------------------------------------------------------------------------------

stamp :blm_init_s( sz_t level )
{
    bl_t active = true;
    func :.to_sink    { if( o.active ) sink.push_fa( "BLM_INIT_LEVEL(#<sz_t>);", o.level ); =0; }
    func :.activate   o.active = true;
    func :.deactivate o.active = false;
}

//----------------------------------------------------------------------------------------------------------------------

stamp :blm_down_s
{
    bl_t active = true;
    func :.to_sink    { if( o.active ) sink.push_sc( "BLM_DOWN();" ); =0; }
    func :.activate   o.active = true;
    func :.deactivate o.active = false;
}

//----------------------------------------------------------------------------------------------------------------------

stamp :cast_s( m xoico_che_s* che, d xoico_typespec_s* target_typespec, d aware xoico_che_result* expression )
{
    bl_t active = true;
    func :.activate   o.active = true;
    func :.deactivate o.active = false;

    func :.get_cast
    {
        if( o.active )
        {
            if( pp_cast ) pp_cast.1 = o;
            =true;
        }
        else
        {
            =o.expression.get_cast( pp_cast );
        }
    }

    func bl_t overrides( @* o, @* a )
    {
        xoico_typespec_s* to = o.target_typespec;
        xoico_typespec_s* ta = a.target_typespec;

        /* Currently we restrict to group and stamp pointers.
         * We might be less restrictive in future.
         */
        if( !o.che.is_group( to.type ) && !o.che.is_stamp( to.type ) ) =false;
        if( !o.che.is_group( ta.type ) && !o.che.is_stamp( ta.type ) ) =false;
        if( to.indirection != 1 ) =false;
        if( ta.indirection != 1 ) =false;

        =true;
    }

    /// removes successive reducible casts
    func o reduce( m@* o )
    {
        m @* prev_cast = NULL;
        if( o.expression.get_cast( prev_cast ) )
        {
            if( o.overrides( prev_cast ) ) prev_cast.deactivate();
        }
        =o;
    }

    func :.set_parent_block { o.expression?.set_parent_block( parent ); };

    func :.to_sink
    {
        if( o.active )
        {
            sink.push_sc( "((" );
            o.che.typespec_to_sink( o.target_typespec, sink );
            sink.push_sc( ")(" );
            o.expression?.to_sink( sink );
            sink.push_sc( "))" );
        }
        else
        {
            o.expression?.to_sink( sink );
        }
        =0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

stamp :statement_s( d aware xoico_che_result* expression )
{
    func :.get_cast { =o.expression.get_cast( pp_cast ); }

    /// removes ineffective code
    func o reduce( m@* o )
    {
        m :cast_s* prev_cast = NULL;
        if( o.expression.get_cast( prev_cast ) ) prev_cast.deactivate();
        =o;
    };

    func :.set_parent_block { o.expression.set_parent_block( parent ); }
    func :.to_sink { =o.expression.to_sink( sink ); }
}

//----------------------------------------------------------------------------------------------------------------------

stamp :break_s( sz_t ledge_level ) =
{
    hidden :block_s* parent;
    func :.set_parent_block { o.parent = parent; }

    func :.to_sink
    {
        if( !o.parent ) ERR_fa( "Parent missing." );
        if( o.parent.is_using_blm_until_level( o.ledge_level ) )
        {
            sink.push_fa( "BLM_BREAK_LEVEL(#<sz_t>);", o.ledge_level );
        }
        else
        {
            sink.push_fa( "break;" );
        }
        =0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

stamp :return_s( m xoico_che_s* che, d aware xoico_che_result* return_expression ) =
{
    hidden :block_s* parent;
    func :.set_parent_block o.parent = parent;

    func :.to_sink
    {
        if( !o.parent ) ERR_fa( "Parent missing." );

        if( o.parent.is_using_blm_until_level( 0 ) )
        {
            if( o.che.returns_a_value() )
            {
                sink.push_sc( "BLM_RETURNV(" );
                o.che.typespec_to_sink( o.che.signature.typespec_ret, sink );
                sink.push_sc( "," );
                o.return_expression.to_sink( sink );
                sink.push_sc( ")" );  // do not terminate BLM_RETURNV macro with a semicolon, otherwise if-else statements might not be handled correctly
            }
            else
            {
                sink.push_sc( "BLM_RETURN()" );
                o.return_expression.to_sink( sink ); //probably not necessary
                sink.push_sc( ";" );
            }
        }
        else
        {
            sink.push_sc( "return " );
            o.return_expression.to_sink( sink );
            sink.push_sc( ";" );
        }
        =0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

func er_t push_fv( m @* o, sc_t format, va_list args )
{
    d st_s* st = st_s_create_fv( format, args );
    er_t ret = o.push_st( st );
    st_s_discard( st );
    =ret;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t push_fa( m @* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    er_t ret = o.push_fv( format, args );
    va_end( args );
    =ret;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t copy_fv( m @* o, sc_t format, va_list args )
{
    xoico_che_result_a_clear( o );
    =o.push_fv( format, args );
}

//----------------------------------------------------------------------------------------------------------------------

func er_t copy_fa( m @* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    er_t ret = o.copy_fv( format, args );
    va_end( args );
    =ret;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

