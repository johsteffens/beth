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

stamp :s = aware x_array
{
    xoico_arg_s => [];
    func xoico_arg.is_variadic { return ( o.size > 0 && o.[ o.size - 1 ].is_variadic() ); };
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) xoico.parse { o.clear(); return o.append( host, source ); };

// ---------------------------------------------------------------------------------------------------------------------

func (:s) er_t append( m @* o, c xoico_host* host, m x_source* source )
{
    bl_t first = true;
    while( !source.parse_bl( " #=?')' " ) ) // args follow
    {
        if( o.is_variadic() ) return source.parse_error_fa( "Cannot append to variadic argument list." );
        if( !first ) xoico_parse_f( source, " , " );
        m $* arg = xoico_arg_s!^^;
        arg.parse( host, source );
        o.push_d( arg.fork() );
        first = false;
    }
    return 0;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) er_t relent( m @* o, c xoico_host* host, tp_t tp_obj_type )
{
    foreach( m $* arg in o ) arg.relent( host, tp_obj_type );
    return 0;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) xoico.convert_transient_types
{
    foreach( m $* arg in o ) arg.convert_transient_types( host, map );
    return  0;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand( c @* o, c xoico_host* host, bl_t first, m x_sink* sink )
{
    foreach( m $* arg in o )
    {
        if( !first ) sink.push_fa( ", " );
        first = false;
        arg.expand( host, sink );
    }
    return 0;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_x( c @* o, c xoico_host* host, bl_t first, m x_sink* sink )
{
    foreach( m $* arg in o )
    {
        if( !first ) sink.push_fa( ", " );
        first = false;
        arg.expand_x( host, sink );
    }
    return 0;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) er_t expand_name( c @* o, c xoico_host* host, bl_t first, m x_sink* sink )
{
    foreach( m $* arg in o )
    {
        if( !first ) sink.push_fa( ", " );
        first = false;
        arg.expand_name( host, sink );
    }
    return 0;
};

// ---------------------------------------------------------------------------------------------------------------------

func (:s) xoico.get_hash
{
    tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o._ );
    foreach( m $* arg in o ) hash = bcore_tp_fold_tp( hash, arg.get_hash() );
    return hash;
};

//----------------------------------------------------------------------------------------------------------------------

func (:s) c xoico_arg_s* get_arg_by_name( c @* o, tp_t name )
{
    foreach( c $* arg in o ) if( arg.name == name ) return arg;
    return NULL;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
