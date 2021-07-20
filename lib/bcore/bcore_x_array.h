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

/** Beth array group preferably in trait-line of x-arrays.
 */

#ifndef BCORE_X_ARRAY_H
#define BCORE_X_ARRAY_H

#include "bcore_spect_array.h"
#include "bcore_x_inst.h"
#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_array, x_inst )

#ifdef XOILA_SECTION

/** Transient classes
 *  (TO): object type
 *  (TE): element type (locked to element type for static typed arrays)
 */

//----------------------------------------------------------------------------------------------------------------------

name size;
name space;
name data;

//----------------------------------------------------------------------------------------------------------------------

func (sz_t t_size ( c obliv (TO) @* o, tp_t t )) = { return bcore_array_t_get_size ( t, o.cast( m bcore_array* ) ); };
func (sz_t   size ( c aware (TO) @* o         )) = { return o.t_size( o._ ); };

func (m (TO) @* t_set_size( m obliv (TO) @* o, tp_t t, sz_t size )) = { bcore_array_t_set_size ( t, o.cast( m bcore_array* ), size  ); return o; };
func (m (TO) @*   set_size( m aware (TO) @* o,         sz_t size )) = { return o.t_set_size( o._, size ); };

func (m (TO) @* t_set_space( m obliv (TO) @* o, tp_t t, sz_t space )) = { bcore_array_t_set_space( t, o.cast( m bcore_array* ), space ); return o; };
func (m (TO) @*   set_space( m aware (TO) @* o,         sz_t space )) = { return o.t_set_space( o._, space ); };

func (m (TO) @* t_clear( m obliv (TO) @* o, tp_t t )) = { return o.t_set_space( t, 0 ); };
func (m (TO) @*   clear( m aware (TO) @* o         )) = { return o.t_clear( o._ ); };

func (m (TO) @* t_sort( m obliv (TO) @* o, tp_t t, s2_t direction )) = { bcore_array_t_sort( t, o.cast( m bcore_array* ), 0, -1, direction ); return o; };
func (m (TO) @*   sort( m aware (TO) @* o,         s2_t direction )) = { return o.t_sort( o._, direction ); };

//----------------------------------------------------------------------------------------------------------------------

func (bl_t t_is_fixed     ( tp_t t )) = { return bcore_array_s_get_typed( t )->size_fix > 0;  };
func (bl_t t_is_static    ( tp_t t )) = { return bcore_array_s_get_typed( t )->is_static;     };
func (bl_t t_is_of_aware  ( tp_t t )) = { return bcore_array_s_get_typed( t )->is_of_aware;   };
func (bl_t t_is_of_links  ( tp_t t )) = { return bcore_array_s_get_typed( t )->is_of_links;   };
func (bl_t t_is_mono_typed( tp_t t )) = { return bcore_array_s_get_typed( t )->is_mono_typed; };

func (bl_t is_fixed     ( aware @* o )) = { return o.t_is_fixed( o._ );  };
func (bl_t is_static    ( aware @* o )) = { return o.t_is_static( o._ );  };
func (bl_t is_of_aware  ( aware @* o )) = { return o.t_is_of_aware( o._ );  };
func (bl_t is_of_links  ( aware @* o )) = { return o.t_is_of_links( o._ );  };
func (bl_t is_mono_typed( aware @* o )) = { return o.t_is_mono_typed( o._ );  };

/// true if mono_typed and type can be changed (non-static)
func (bl_t t_is_mutable_mono_typed( tp_t t )) = { return bcore_array_p_is_mutable_mono_typed( bcore_array_s_get_typed( t ) ); };
func (bl_t   is_mutable_mono_typed( @* o )) = { return o.t_is_mutable_mono_typed( o._ ); };

/// true if elements can have different types
func (bl_t t_is_is_multi_typed( tp_t t )) = { return bcore_array_p_is_multi_typed( bcore_array_s_get_typed( t ) ); };
func (bl_t   is_is_multi_typed( @* o )) = { return o.t_is_is_multi_typed( o._ ); };

/// returns type if static, 0 otherwise;
func (tp_t t_get_static_type( tp_t t )) = { return bcore_array_p_get_static_type( bcore_array_s_get_typed( t ) ); };
func (tp_t   get_static_type( @* o )) = { return o.t_get_static_type( o._ ); };

/// returns type if monotyped; (o can be NULL on static typed arrays)
func (tp_t t_get_mono_type( @* o, tp_t t )) = { return bcore_array_p_get_mono_type( bcore_array_s_get_typed( t ), o ); };
func (tp_t   get_mono_type( @* o )) = { return o.t_get_mono_type( o._ ); };

///  returns type of indexed element; returns 0 when type cannot be determined
func (tp_t t_get_type( c obliv @* o, tp_t t, sz_t index )) = { return bcore_array_p_get_type( bcore_array_s_get_typed( t ), o, index ); };
func (tp_t   get_type( c aware @* o,         sz_t index )) = { return o.t_get_type( o._, index ); };

//----------------------------------------------------------------------------------------------------------------------

/// On mutable_mono_typed arrays: changes global item-type on empty arrays;
func (void t_set_gtype( m obliv @* o, tp_t t, tp_t type )) = { bcore_array_p_set_gtype( bcore_array_s_get_typed( t ), o.cast( m bcore_array* ), type ); };
func (void   set_gtype( m aware @* o,         tp_t type )) = { o.t_set_gtype( o._, type ); };

//----------------------------------------------------------------------------------------------------------------------

signature m (TE) x_inst* t_push_d(  m obliv @* o, tp_t t, d obliv (TE) x_inst* v );
signature m (TE) x_inst* t_push_c(  m obliv @* o, tp_t t, c obliv (TE) x_inst* v );
signature m (TE) x_inst* t_push_t(  m obliv @* o, tp_t t, tp_t val_type );
signature m (TE) x_inst* t_push(    m obliv @* o, tp_t t );

func (m (TE) void t_push_sr( m obliv @* o, tp_t t, sr_s sr )) = { bcore_array_t_push( t, ( bcore_array* )o, sr ); };

func (m (TE) x_inst* push_d(  m aware @* o, d obliv (TE) x_inst* v )) = { return o.t_push_d( o._, v ); };
func (m (TE) x_inst* push_c(  m aware @* o, c obliv (TE) x_inst* v )) = { return o.t_push_c( o._, v ); };
func (m (TE) x_inst* push_t(  m aware @* o, tp_t val_type )) = { return o.t_push_t( o._, val_type ); };
func (m (TE) x_inst* push  (  m aware @* o )) = { return o.t_push( o._ ); };
func (void           push_sr( m aware @* o, sr_s sr )) = { o.t_push_sr( o._, sr ); };

//----------------------------------------------------------------------------------------------------------------------

signature m (TE) x_inst* t_set_d(  m obliv @* o, tp_t t, sz_t index, d obliv (TE) x_inst* v );
signature m (TE) x_inst* t_set_c(  m obliv @* o, tp_t t, sz_t index, c obliv (TE) x_inst* v );
signature m (TE) x_inst* t_set_t(  m obliv @* o, tp_t t, sz_t index, tp_t val_type );

func (m (TE) void t_set_sr( m obliv @* o, tp_t t, sz_t index, sr_s sr ) ) = { bcore_array_t_set( t, ( bcore_array* )o, index, sr ); };

func (m (TE) x_inst* set_d(  m aware @* o, sz_t index, d obliv (TE) x_inst* v )) = { return o.t_set_d( o._, index, v ); };
func (m (TE) x_inst* set_c(  m aware @* o, sz_t index, c obliv (TE) x_inst* v )) = { return o.t_set_c( o._, index, v ); };
func (m (TE) x_inst* set_t(  m aware @* o, sz_t index, tp_t val_type )) = { return o.t_set_t( o._, index, val_type ); };
func (m (TE) void    set_sr( m aware @* o, sz_t index, sr_s sr )) = { o.t_set_sr( o._, index, sr ); };

/// returns sr_NULL in case of no match
func (sr_s t_m_get_sr( m obliv @* o, tp_t t, sz_t index )) = { return bcore_array_t_get( t, o.cast( m bcore_array* ), index ); };
func (sr_s   m_get_sr( m aware @* o,         sz_t index )) = { return o.t_m_get_sr( o._, index ); };

/// returns sr_NULL in case of no match
func (sr_s t_c_get_sr( c obliv @* o, tp_t t, sz_t index ));
func (sr_s   c_get_sr( c aware @* o,         sz_t index )) = { return o.t_c_get_sr( o._, index ); };

//----------------------------------------------------------------------------------------------------------------------

func t_push_d = (verbatim_C)
{
    const bcore_array_s* p = bcore_array_s_get_typed( t );

    if( p->item_p )
    {
        bcore_array_p_push( p, ( bcore_array* )o, sr_psd( p->item_p, v ) );
    }
    else
    {
        bcore_array_p_push( p, ( bcore_array* )o, sr_asd( v ) );
    }

    if( p->is_of_links )
    {
        return v;
    }
    else if( p->is_static )
    {
        uz_t size = 0;
        u0_t* data = bcore_array_p_get_d_data_size( p, o, &size );
        return ( x_inst* )( data + p->item_p->size * ( size - 1 ) );
    }
    else
    {
        return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
    }
};

//----------------------------------------------------------------------------------------------------------------------

func t_push_c = (verbatim_C)
{
    const bcore_array_s* p = bcore_array_s_get_typed( t );

    if( p->item_p )
    {
        bcore_array_p_push( p, ( bcore_array* )o, sr_pwc( p->item_p, v ) );
    }
    else
    {
        bcore_array_p_push( p, ( bcore_array* )o, sr_awc( v ) );
    }

    if( p->is_of_links )
    {
        uz_t size = 0;
        x_inst** data = bcore_array_p_get_d_data_size( p, o, &size );
        return data[ size - 1 ];
    }
    else if( p->is_static )
    {
        uz_t size = 0;
        u0_t* data = bcore_array_p_get_d_data_size( p, o, &size );
        return ( x_inst* )( data + p->item_p->size * ( size - 1 ) );
    }
    else
    {
        return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
    }
};

//----------------------------------------------------------------------------------------------------------------------

func t_push_t = (verbatim_C)
{
    const bcore_array_s* p = bcore_array_s_get_typed( t );
    bcore_array_p_push( p, ( bcore_array* )o, sr_t_create( val_type ) );

    if( p->is_of_links )
    {
        uz_t size = 0;
        x_inst** data = bcore_array_p_get_d_data_size( p, o, &size );
        return data[ size - 1 ];
    }
    else if( p->is_static )
    {
        uz_t size = 0;
        u0_t* data = bcore_array_p_get_d_data_size( p, o, &size );
        return ( x_inst* )( data + p->item_p->size * ( size - 1 ) );
    }
    else
    {
        return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
    }
};

//----------------------------------------------------------------------------------------------------------------------

func t_push = (verbatim_C)
{
    const bcore_array_s* p = bcore_array_s_get_typed( t );

    if( p->item_p )
    {
        if( p->is_of_links )
        {
            tp_t item_type = p->item_p->header.o_type;
            bcore_array_p_push( p, ( bcore_array* )o, sr_t_create( item_type ) );
        }
        else
        {
            bcore_array_p_push( p, ( bcore_array* )o, sr_null() );
        }
    }
    else
    {
        ERR_fa( "Unspecified push to a dynamically typed array. Use push_d, push_c or push_t." );
    }

    if( p->is_of_links )
    {
        uz_t size = 0;
        x_inst** data = bcore_array_p_get_d_data_size( p, o, &size );
        return data[ size - 1 ];
    }
    else if( p->is_static )
    {
        uz_t size = 0;
        u0_t* data = bcore_array_p_get_d_data_size( p, o, &size );
        return ( x_inst* )( data + p->item_p->size * ( size - 1 ) );
    }
    else
    {
        return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
    }
};

//----------------------------------------------------------------------------------------------------------------------

func t_set_d = (verbatim_C)
{
    const bcore_array_s* p = bcore_array_s_get_typed( t );

    if( p->item_p )
    {
        bcore_array_p_set( p, ( bcore_array* )o, index, sr_psd( p->item_p, v ) );
    }
    else
    {
        bcore_array_p_set( p, ( bcore_array* )o, index, sr_asd( v ) );
    }

    if( p->is_of_links )
    {
        return v;
    }
    else if( p->is_static )
    {
        u0_t* data = bcore_array_p_get_d_data( p, o );
        return ( x_inst* )( data + p->item_p->size * index );
    }
    else
    {
        return bcore_array_p_get( p, ( bcore_array* )o, index ).o;
    }
};

//----------------------------------------------------------------------------------------------------------------------

func t_set_c = (verbatim_C)
{
    const bcore_array_s* p = bcore_array_s_get_typed( t );

    if( p->item_p )
    {
        bcore_array_p_set( p, ( bcore_array* )o, index, sr_pwc( p->item_p, v ) );
    }
    else
    {
        bcore_array_p_set( p, ( bcore_array* )o, index, sr_awc( v ) );
    }

    if( p->is_of_links )
    {
        x_inst** data = bcore_array_p_get_d_data( p, o );
        return data[ index ];
    }
    else if( p->is_static )
    {
        u0_t* data = bcore_array_p_get_d_data( p, o );
        return ( x_inst* )( data + p->item_p->size * index );
    }
    else
    {
        return bcore_array_p_get( p, ( bcore_array* )o, index ).o;
    }
};

//----------------------------------------------------------------------------------------------------------------------

func t_set_t = (verbatim_C)
{
    const bcore_array_s* p = bcore_array_s_get_typed( t );
    bcore_array_p_set( p, ( bcore_array* )o, index, sr_t_create( val_type ) );

    if( p->is_of_links )
    {
        x_inst** data = bcore_array_p_get_d_data( p, o );
        return data[ index ];
    }
    else if( p->is_static )
    {
        u0_t* data = bcore_array_p_get_d_data( p, o );
        return ( x_inst* )( data + p->item_p->size * index );
    }
    else
    {
        return bcore_array_p_get( p, ( bcore_array* )o, index ).o;
    }
};

//----------------------------------------------------------------------------------------------------------------------

func t_c_get_sr =
{
    sr_s sr = bcore_array_t_get( t, o.cast( bcore_array* ), index );
    if( sr.is_weak() ) sr.set_const( true );
    return sr;
};

//----------------------------------------------------------------------------------------------------------------------

#endif // XOILA_SECTION

/**********************************************************************************************************************/

#endif  // BCORE_X_ARRAY_H
