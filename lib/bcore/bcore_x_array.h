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

XOILA_DEFINE_GROUP( x_array, bcore_array )

#ifdef XOILA_SECTION

/** Transient classes
 *  (TO): object type
 *  (TE): element type (locked to element type for static typed arrays)
 */

//----------------------------------------------------------------------------------------------------------------------

func ((TO) @* clear(     (TO) mutable             )) = { bcore_array_a_set_space( o.cast( bcore_array* ), 0 ); return o; };
func ((TO) @* set_size ( (TO) mutable, sz_t size  )) = { bcore_array_a_set_size ( o.cast( bcore_array* ), size  ); return o; };
func ((TO) @* set_space( (TO) mutable, sz_t space )) = { bcore_array_a_set_space( o.cast( bcore_array* ), space ); return o; };
func ((TO) @* sort( (TO) mutable, s2_t direction  )) = { bcore_array_a_sort( o.cast( bcore_array* ), 0, -1, direction ); return o; };

//----------------------------------------------------------------------------------------------------------------------

func ((TE) x_inst* push_d( mutable, unaware (TE) x_inst* v )) = (verbatim_C)
{
    const bcore_array_s* p = bcore_array_s_get_aware( o );

    if( p->item_p )
    {
        tp_t item_type = p->item_p->header.o_type;
        bcore_array_p_push( p, ( bcore_array* )o, sr_tsd( item_type, v ) );
    }
    else
    {
        bcore_array_p_push( p, ( bcore_array* )o, sr_asd( v ) );
    }

    if( p->is_of_links )
    {
        return v;
    }
    else
    {
        return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
    }
};

//----------------------------------------------------------------------------------------------------------------------

func ((TE) x_inst* push_c( mutable, const unaware (TE) x_inst* v )) = (verbatim_C)
{
    const bcore_array_s* p = bcore_array_s_get_aware( o );

    if( p->item_p )
    {
        tp_t item_type = p->item_p->header.o_type;
        bcore_array_p_push( p, ( bcore_array* )o, sr_twc( item_type, v ) );
    }
    else
    {
        bcore_array_p_push( p, ( bcore_array* )o, sr_awc( v ) );
    }

    return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
};

//----------------------------------------------------------------------------------------------------------------------

func ((TE) x_inst* push_t( mutable, tp_t type )) = (verbatim_C)
{
    const bcore_array_s* p = bcore_array_s_get_aware( o );
    bcore_array_p_push( p, ( bcore_array* )o, sr_t_create( type ) );
    return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
};

//----------------------------------------------------------------------------------------------------------------------

func ((TE) x_inst* push( mutable )) = (verbatim_C)
{
    const bcore_array_s* p = bcore_array_s_get_aware( o );

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
        ERR_fa( "Unspecified push to a dynamically typed array. Use push_d or push_c." );
    }

    return bcore_array_p_get_last( p, ( bcore_array* )o ).o;
};

//----------------------------------------------------------------------------------------------------------------------

#endif // XOILA_SECTION

/**********************************************************************************************************************/

#endif  // BCORE_X_ARRAY_H
