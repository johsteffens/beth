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

signature er_t parse(  m @* o, c xoico_host* host, m x_source* source );
signature er_t relent( m @* o, c xoico_host* host, tp_t tp_obj_type );
signature er_t expand( c @* o, c xoico_host* host, m x_sink* sink );
signature er_t expand_x( c @* o, c xoico_host* host, m x_sink* sink ); // expands in x-format
signature bl_t converts_to( c @* o, c @* b ); // in 'C' auto-converts to b without a cast
signature bl_t is_ptr_type( c @* o ); // type is a pointer type (like vc_t, vd_t, sc_t, sd_t)

signature void reset( m @* o );

name type_deduce;
name type_object;
name const;
name mutable;
name discardable;
name c;
name m;
name d;

stamp :transient_s = aware :
{
    tp_t class;
    tp_t cast_to_var; // cast to typespec of variable with given name (typically a return typespec of a function)

    func xoico.get_hash
    {
        tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o._ );
        hash = bcore_tp_fold_tp( hash, o.class );
        return hash;
    };
};

stamp :s = aware :
{
    tp_t type; // possible variable types are TYPEOF_type_deduce and type_object~
    tp_t access_class; // 'const|mutable|discardable'

    :transient_s => transient;

    sz_t indirection;

    bl_t flag_static;
    bl_t flag_volatile;
    bl_t flag_restrict;
    bl_t flag_obliv; // obliv indicates that this type can reference obliv objects (extends range of implicit casts)
    bl_t flag_aware;   // aware object is required (allows extended tests)
    bl_t flag_scope;   // object is in scope
    bl_t flag_addressable = true;  // object can have a pointer ('false' for objects returned by a function)
    bl_t flag_variadic; // variadic arguments

    func xoico.get_hash;
    func     :.parse;
    func     :.relent
    {
        if( o.type == type_object~ ) o.type = tp_obj_type;
        return 0;
    };

    func xoico.convert_transient_types
    {
        if( o.transient )
        {
            tp_t type = map.get( o.transient.class );
            if( type ) o.type = type;
        }
        return 0;
    };

    func :.expand;
    func :.expand_x;
    func :.converts_to;
    func :.is_ptr_type;

    func :.reset
    {
        o.access_class = 0;
        o.transient =< NULL;
        o.type = 0;
        o.indirection = 0;
        o.flag_static   = false;
        o.flag_volatile = false;
        o.flag_restrict = false;
        o.flag_aware    = false;
        o.flag_obliv    = false;
        o.flag_scope    = false;
        o.flag_addressable = true;  // object can have a pointer ('false' for objects returned by a function)
    };

    func  bl_t is_void( c @* o ) { return (o.type == 0 || o.type == void~) && o.indirection == 0; };
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.parse
{
    o.reset();
    m $* compiler = host.compiler();

    if( source.parse_bl( "#?'...' " ) )
    {
        o.flag_variadic = true;
        return 0;
    }

    tp_t access_class = 0;

    if(      source.parse_bl( " #?w'c'" ) || source.parse_bl( " #?w'const'"       ) ) access_class = TYPEOF_const;
    else if( source.parse_bl( " #?w'm'" ) || source.parse_bl( " #?w'mutable'"     ) ) access_class = TYPEOF_mutable;
    else if( source.parse_bl( " #?w'd'" ) || source.parse_bl( " #?w'discardable'" ) ) access_class = TYPEOF_discardable;

    while( !source.eos() )
    {
        if     ( source.parse_bl( " #?w'static'"   ) && !o->flag_static ) o->flag_static = true;
        else if( source.parse_bl( " #?w'volatile'" ) && !o->flag_volatile ) o->flag_volatile = true;
        else if( source.parse_bl( " #?w'obliv'"    ) && !o->flag_obliv ) o->flag_obliv = true;
        else if( source.parse_bl( " #?w'aware'"    ) && !o->flag_aware ) o->flag_aware = true;
        else break;
    }

    source.parse_fa( " " );

    if( source.parse_bl( "#?'(' " ) )
    {
        m st_s* s = st_s!^^;
        source.parse_fa( "#name ", s );
        if( s.size == 0 ) source.parse_error_fa( "Transient class: Identifier expected." );
        o.transient!.class = compiler.entypeof( s.sc );
        source.parse_fa( " ) " );
    }

    if( source.parse_bl( "#=?':'" ) )
    {
        host.parse_name_tp( source, o.type.1 );
        source.parse_fa( " " );
    }
    else if( source.parse_bl( "#?'@' " ) )
    {
        o->type = type_object~;
    }
    else if( source.parse_bl( "#?'$' " ) )
    {
        o->type = type_deduce~;
    }
    else
    {
        m st_s* s = st_s!^^;
        source.parse_fa( "#name ", s );
        if( s.size == 0 ) source.parse_error_fa( "Argument: Type expected." );
        o->type = xoico_compiler_s_entypeof( compiler, s->sc );
    }

    if( source.parse_bl( "#?'.' " ) )
    {
        if( !source.parse_bl( "#?([0]>='0'&&[0]<='9') " ) ) source.parse_error_fa( "Argument: Indirection literal expected." );
        sz_t indirection = 0;
        source.parse_fa( "#<sz_t*> ", indirection.1 );
        o.indirection = indirection;
    }
    else
    {
        while( source.parse_bl( "#?'*' " ) ) o.indirection++;
    }

    if( source.parse_bl( " #?'restrict' " ) ) o.flag_restrict = true;

    if( o.indirection > 0 && access_class == 0 )
    {
        access_class = TYPEOF_const;
    }

    o.access_class = access_class;
    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.get_hash
{
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o._ );
    hash = bcore_tp_fold_tp( hash, o.type );
    hash = bcore_tp_fold_tp( hash, o.access_class );
    hash = bcore_tp_fold_tp( hash, o.transient ? o.transient.get_hash() : 0 );
    hash = bcore_tp_fold_bl( hash, o.flag_static );
    hash = bcore_tp_fold_bl( hash, o.flag_volatile );
    hash = bcore_tp_fold_bl( hash, o.flag_restrict );
    hash = bcore_tp_fold_bl( hash, o.flag_scope );
    hash = bcore_tp_fold_u3( hash, o.indirection );
    return hash;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.expand
{
    if( o.flag_variadic )
    {
        sink.push_fa( "..." );
        return 0;
    }

    m $* compiler = host.compiler();

    tp_t type = o.type;

    if( type == type_object~ )
    {
        //if( !sc_obj_type ) ERR_fa( "Cannot resolve 'type_object' at this point." );
        type = host.obj_type(); //compiler.entypeof( sc_obj_type );
    }
    else if( type == type_deduce~ )
    {
        ERR_fa( "Cannot resolve 'type_deduce' at this point." );
    }

    m st_s* st_type = st_s_create_sc( compiler.nameof( type ) )^^;

    sc_t sc_type = st_type.sc;
    if( o.flag_static   ) sink.push_fa( "static " );
    if( o.access_class == TYPEOF_const ) sink.push_fa( "const " );
    if( o.flag_volatile ) sink.push_fa( "volatile " );
    sink.push_fa( "#<sc_t>", sc_type );

    for( sz_t i = 0; i < o.indirection; i++ ) sink.push_fa( "*" );
    if( o.flag_restrict ) sink.push_fa( " restrict " );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.expand_x
{
    if( o.flag_variadic )
    {
        sink.push_fa( "..." );
        return 0;
    }

    m $* compiler = host.compiler();

    tp_t type = o.type;

    if( type == type_object~ )
    {
        type = host.obj_type(); //compiler.entypeof( sc_obj_type );
    }
    else if( type == type_deduce~ )
    {
        ERR_fa( "Cannot resolve 'type_deduce' at this point." );
    }

    m st_s* st_type = st_s_create_sc( compiler.nameof( type ) )^^;

    switch( o.access_class )
    {
        case TYPEOF_const:       sink.push_fa( "c " ); break;
        case TYPEOF_mutable:     sink.push_fa( "m " ); break;
        case TYPEOF_discardable: sink.push_fa( "d " ); break;
        default: break;
    }

    if( o.flag_static   ) sink.push_fa( "static " );
    if( o.flag_volatile ) sink.push_fa( "volatile " );
    sc_t sc_type = st_type.sc;

    if( o.transient )
    {
        sink.push_fa( "(#<sc_t>)", host.nameof( o.transient.class ) );
    }

    sink.push_fa( "#<sc_t>", sc_type );

    if( o.indirection > 0 ) sink.push_fa( ".#<sz_t>", o.indirection );
    if( o.flag_restrict ) sink.push_fa( " restrict " );

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func bl_t is_numeric( tp_t type )
{
    switch( type )
    {
        case u0_t~: return true;
        case u1_t~: return true;
        case u2_t~: return true;
        case u3_t~: return true;
        case s0_t~: return true;
        case s1_t~: return true;
        case s2_t~: return true;
        case s3_t~: return true;
        case f2_t~: return true;
        case f3_t~: return true;
        case sz_t~: return true;
        case uz_t~: return true;
        case tp_t~: return true;
        case aware_t~: return true;
        case er_t~: return true;
        case bl_t~: return true;
        case char~: return true;
        case int~:    return true;
        case long~:   return true;
        case float~:  return true;
        case double~: return true;
        default: break;
    }
    return false;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) :.converts_to
{
    if( o.type == b.type )
    {
        if( o.indirection == b.indirection )
        {
            if( o.access_class == TYPEOF_const )
            {
                return ( b.access_class == TYPEOF_const );
            }
            else
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        if( o.indirection == 0 && b.indirection == 0 )
        {
            if( :is_numeric( o.type ) && :is_numeric( b.type ) )
            {
                return true;
            }
            else if( o.type == vd_t~ && b.type == vc_t~ )
            {
                return true;
            }
            else if( o.type == sd_t~ && b.type == sc_t~ )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if( b.type == vc_t~ && b.indirection == 0 )
        {
            if( o.indirection == 1 )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if( b.type == vd_t~ && b.indirection == 0 )
        {
            if( o.indirection == 1 && ( o.access_class != TYPEOF_const ) )
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if( b.type == bl_t~ && b.indirection == 0 )
        {
            if( o.indirection > 0 )
            {
                return true;
            }
            else if( o.type == vd_t~ || o.type == vc_t~ || o.type == sc_t~ || o.type == sd_t~ )
            {
                return ( o.indirection == 0 );
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) is_ptr_type
{
    return o.type == TYPEOF_vd_t ||
           o.type == TYPEOF_vc_t ||
           o.type == TYPEOF_sc_t ||
           o.type == TYPEOF_sd_t;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

