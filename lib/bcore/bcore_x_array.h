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

#include "bcore_std.h"
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

func (m (TO) @* t_set_size (m (TO) @* o, tp_t t, sz_t size      )) = { bcore_array_t_set_size ( t, o.cast( m bcore_array* ), size  ); return o; };
func (m (TO) @* t_set_space(m (TO) @* o, tp_t t, sz_t space     )) = { bcore_array_t_set_space( t, o.cast( m bcore_array* ), space ); return o; };
func (m (TO) @* t_clear    (m (TO) @* o, tp_t t                 )) = { return o.t_set_space( t, 0 ); };
func (m (TO) @* t_sort(     m (TO) @* o, tp_t t, s2_t direction )) = { bcore_array_t_sort( t, o.cast( m bcore_array* ), 0, -1, direction ); return o; };

//----------------------------------------------------------------------------------------------------------------------

func (m (TE) x_inst* t_push_d( m @* o, tp_t t, d unaware (TE) x_inst* v )) = (verbatim_C)
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

func (m (TE) x_inst* t_push_c( m @* o, tp_t t, c unaware (TE) x_inst* v )) = (verbatim_C)
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

func (m (TE) x_inst* t_push_t( m @* o, tp_t t, tp_t val_type )) = (verbatim_C)
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

func (m (TE) x_inst* t_push( m @* o, tp_t t )) = (verbatim_C)
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

func (m (TO) @* clear(     m (TO) @* o                 )) = { return o.t_clear( o._ ); };
func (m (TO) @* set_size ( m (TO) @* o, sz_t size      )) = { return o.t_set_size( o._, size ); };
func (m (TO) @* set_space( m (TO) @* o, sz_t space     )) = { return o.t_set_space( o._, space ); };
func (m (TO) @* sort(      m (TO) @* o, s2_t direction )) = { return o.t_sort( o._, direction ); };

func (m (TE) x_inst* push_d( m @* o, d unaware (TE) x_inst* v )) = { return o.t_push_d( o._, v ); };
func (m (TE) x_inst* push_c( m @* o, c unaware (TE) x_inst* v )) = { return o.t_push_c( o._, v ); };
func (m (TE) x_inst* push_t( m @* o, tp_t val_type )) = { return o.t_push_t( o._, val_type ); };
func (m (TE) x_inst* push  ( m @* o )) = { return o.t_push( o._ ); };

#endif // XOILA_SECTION

/**********************************************************************************************************************/

#endif  // BCORE_X_ARRAY_H
