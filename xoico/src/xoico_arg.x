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

signature bl_t is_variadic( c @* o );

stamp :s = aware :
{
    x_source_point_s source_point;
    xoico_typespec_s typespec;
    tp_t name;

    func :.is_variadic { return o.typespec.flag_variadic; };
    func xoico.get_source_point { return o.source_point; };
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) xoico.parse
{
    m $* compiler = host.compiler();
    o.source_point.setup_from_source( source );
    o.typespec.parse( host, source );

    if( o.typespec.type == type_deduce~ )
    {
        return o.source_point.parse_error_fa( "'$' (type_deduce) can not be used in an argument." );
    }

    if( o.typespec.flag_variadic ) return 0;

    if( o.typespec.type == void~ && o.typespec.indirection == 0 )
    {
        return o.source_point.parse_error_fa( "'void' is misplaced here." );
    }

    m $* s = st_s!^;
    source.parse_fa( "#name ", s );
    if( s.size == 0 )
    {
        return source.parse_error_fa( "Argument: Name expected." );
    }
    o.name = compiler.entypeof( s.sc );

    return 0;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) xoico.get_hash
{
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o._ );
    hash = bcore_tp_fold_tp( hash, o.typespec.get_hash() );
    hash = bcore_tp_fold_tp( hash, o.name );
    return hash;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) er_t relent( m @* o, c xoico_host* host, tp_t tp_obj_type )
{
    return o.typespec.relent( host, tp_obj_type );
};


// ---------------------------------------------------------------------------------------------------------------------

func (:s) xoico.convert_transient_types { return o.typespec.convert_transient_types( host, map ); };

// ---------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand( c @* o, c xoico_host* host, m x_sink* sink )
{
    try( o.typespec.expand( host, sink ) );
    if( o.name )
    {
        sink.push_fa( " " );
        o.expand_name( host, sink );
    }
    return 0;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_x( c @* o, c xoico_host* host, m x_sink* sink )
{
    try( o.typespec.expand_x( host, sink ) );
    if( o.name )
    {
        sink.push_fa( " " );
        o.expand_name( host, sink );
    }
    return 0;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_name( c @* o, c xoico_host* host, m x_sink* sink )
{
    if( !o.name ) return 0;
    sink.push_fa( "#<sc_t>", host.compiler().nameof( o.name ) );
    return 0;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) er_t to_self_item_st( c @* o, c xoico_host* host, m st_s* st )
{
    st.clear();
    if( o.typespec.indirection > 1 ) return o.source_point.parse_error_fa( "Indirection > 1 is not allowed as functor argument." );
    if( o.typespec.transient       ) return o.source_point.parse_error_fa( "Transient type is not allowed as functor argument." );

    if( o.typespec.indirection == 1 && o.typespec.access_class == TYPEOF_mutable )
    {
        st.push_fa( "hidden " );
    }

    if( o.typespec.flag_aware )
    {
        st.push_fa( "aware " );
    }
    else if( o.typespec.flag_obliv )
    {
        st.push_fa( "obliv " );
    }

    st.push_fa( "#<sc_t> ", host.nameof( o.typespec.type ) );

    if( o.typespec.indirection == 1 )
    {
        switch( o.typespec.access_class )
        {
            case TYPEOF_discardable: st.push_fa( "=> " ); break;
            case TYPEOF_mutable:     st.push_fa( "* "  ); break;
            case TYPEOF_const:       st.push_fa( "=> " ); break;
            default: return o.source_point.parse_error_fa( "Functor argument: Unhandled access class." ); break;
        }
    }

    st.push_fa( "#<sc_t>", host.nameof( o.name ) );
    st.push_fa( ";" );

    return 0;
};

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
