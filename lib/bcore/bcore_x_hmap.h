/** Author and Copyright 2021 Johannes Bernhard Steffens
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

/** Hashmap generic framework

    Purpose:
      Quickly creating a new hashmap-stamp with all relevant hmap-functions predefined.

    Usage: (Map of tp_t -> my_object_s)
    - Define a stamp with x_hmap_tp in ts traitline.
    - Set type identifier 'TE' to the target object.
    - Place x_hmap_tp_s as first object of the stamp. The name is irrelevant.

    Example:
      stamp my_hmap_my_object_s x_hmap_tp trans(TE my_object_s)
      {
          x_hmap_tp_s my_first_object; // x_hmap_tp_s must be first; the name is irrelevant.

          // additional objects and functions follow here:
          ....
      }
 */

#ifndef BCORE_X_HMAP_H
#define BCORE_X_HMAP_H

#include "bcore_hmap.h"
#include "bcore_x_stamp.h"
#include "bcore_x_array.h"
#include "bcore_arr.h"
#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_hmap, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** hashmap mapping tp_t to an aware object
 *  The object must be aware and have bcore_hmap_tpaw as first element.
 */
group :tp =
{
    /******************************************************************************************************************/
    /// Interface

    signature c (TE) x_inst*     c_get ( c@* o, tp_t key ); // returns pointer to value or NULL when key does not exist
    signature m (TE) x_inst*     m_get ( m@* o, tp_t key );  // returns pointer to value or NULL when key does not exist
    signature m (TE) x_inst*     m_getf( m@* o, tp_t key );  // returns pointer to value or set_d( key, x_inst_create( o.TE() ) ) when key does not exist
    signature m (TE) x_inst*     set_c ( m@* o, tp_t key, c (TE) x_inst* val ); // sets (overwrites) key-value
    signature m (TE) x_inst*     set_d ( m@* o, tp_t key, d (TE) x_inst* val ); // sets (overwrites) key-value
    signature o                remove  ( m@* o, tp_t key ); // removes key
    signature bl_t             exists  ( c@* o, tp_t key ); // checks if key exists
    signature o                clear   ( m@* o ); // removes all entries and frees memory
    signature sz_t             keys    ( c@* o ); // returns number of registered keys
    signature sz_t             size    ( c@* o ); // returns current size of the hash map (note that this includes empty places)
    signature tp_t             idx_key ( c@* o, sz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
    signature c (TE) x_inst* c_idx_val ( c@* o, sz_t idx ); // returns indexed value (idx indexes the entire table including empty places)
    signature m (TE) x_inst* m_idx_val ( m@* o, sz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

    signature key_arr   get_key_arr( c@* o, m bcore_arr_tp_s* key_arr ); // returns array of all valid keys
    signature val_arr m_get_val_arr( m@* o, m x_array*        val_arr ); // returns array of all forked(!) values (key_arr and val_arr are aligned)
    signature val_arr c_get_val_arr( c@* o, m x_array*        val_arr ); // returns array of all cloned(!) values (key_arr and val_arr are aligned)

    /** Returns discardable array of all valid keys.
     *  This is a convenience function that lets easily iterate though all keys:
     *  example:
     *  foreach( tp_t key in map.all_keys()^ ) ...
     */
    signature d bcore_arr_tp_s* all_keys( c@* o );

    /// This feature is automatically overloaded by stamps defining the transient type identifier 'TE' returning its actual type.
    feature tp_t TE( @* o ) { = 0; }

    /******************************************************************************************************************/
    /** First Element  (tp-map to aware objects)
     *  This stamp should be first element of the object that uses this group in its traitline
     */
    stamp :s
    {
        bcore_hmap_tpaw_s map;
        func :.c_get     { c vd_t* e = o.map.get( key ); = e ? e.0.cast( c x_inst* ) : NULL; }
        func :.m_get     { m vd_t* e = o.map.get( key ); = e ? e.0.cast( m x_inst* ) : NULL; }
        func :.set_c     { = o.map.set_c( key, val ).cast( m x_inst* ); }
        func :.set_d     { = o.map.set_d( key, val ).cast( m x_inst* ); }
        func :.remove    { o.map.remove( key ); = o; }
        func :.exists    { = o.map.exists( key ); }
        func :.clear     { o.map.clear(); = o; }
        func :.keys      { = o.map.keys(); }
        func :.size      { = o.map.size(); }
        func :.idx_key   { = o.map.idx_key( idx ); }
        func :.c_idx_val { = o.map.idx_val( idx ).cast( c x_inst* ); }
        func :.m_idx_val { = o.map.idx_val( idx ).cast( m x_inst* ); }
    }

    /******************************************************************************************************************/
    /// Predefined maps

    /// tp - st_s
    stamp :st_s : trans(TE st_s) { :s map; };

    /******************************************************************************************************************/
    /// Implementation

    //------------------------------------------------------------------------------------------------------------------

    func (:s) :.get_key_arr
    {
        key_arr.set_size( 0 );
        sz_t size = o.size();
        tp_t key = 0;
        for( sz_t i = 0; i < size; i++ ) if( (key = o.idx_key( i )) ) key_arr.push( key );
        = key_arr;
    }

    func (:s) :.all_keys { = o.get_key_arr( bcore_arr_tp_s! ); }

    func (:s) :.m_get_val_arr
    {
        val_arr.set_size( 0 );
        sz_t size = o.size();
        for( sz_t i = 0; i < size; i++ ) if( o.idx_key( i ) ) val_arr.push_d( o.m_idx_val( i ).fork() );
        = val_arr;
    }

    func (:s) :.c_get_val_arr
    {
        val_arr.set_size( 0 );
        sz_t size = o.size();
        for( sz_t i = 0; i < size; i++ ) if( o.idx_key( i ) ) val_arr.push_d( o.c_idx_val( i ).clone() );
        = val_arr;
    }

    //------------------------------------------------------------------------------------------------------------------

    func m :s* m_map_( m@* o )
    {
        assert( o.cast( x_stamp* ).is_aware() );
        m x_inst* map = o.cast( m x_stamp* ).m_get_i( 0 );
        if( !map || map._ != TYPEOF_:s ) ERR_fa( "First element of '#<sc_t>' must be of type '#<sc_t>'", bnameof( o._ ), bnameof( :s~ ) );
        = map.cast(m :s*);
    }

    func c :s* c_map_( c@* o )
    {
        assert( o.cast( x_stamp* ).is_aware() );
        c x_inst* map = o.cast( c x_stamp* ).c_get_i( 0 );
        if( !map || map._ != TYPEOF_:s ) ERR_fa( "First element of '#<sc_t>' must be of type '#<sc_t>'", bnameof( o._ ), bnameof( :s~ ) );
        = map.cast(c :s*);
    }

    func c_get  { = o.c_map_().c_get( key ); }
    func m_get  { = o.m_map_().m_get( key ); }
    func set_c  { = o.m_map_().set_c( key, val ); }
    func set_d  { = o.m_map_().set_d( key, val ); }
    func remove { = o.m_map_().remove( key ); }
    func exists { = o.c_map_().exists( key ); }
    func clear  { = o.m_map_().clear(); }
    func keys   { = o.c_map_().keys(); }
    func size   { = o.c_map_().size(); }
    func idx_key   { = o.c_map_().idx_key( idx ); }
    func c_idx_val { = o.c_map_().c_idx_val( idx ); }
    func m_idx_val { = o.m_map_().m_idx_val( idx ); }
    func get_key_arr   { = o.c_map_().get_key_arr( key_arr ); }
    func m_get_val_arr { = o.m_map_().m_get_val_arr( val_arr ); }
    func c_get_val_arr { = o.c_map_().c_get_val_arr( val_arr ); }
    func all_keys { = o.c_map_().all_keys(); }

    func m_getf
    {
        m x_inst* inst = o.m_map_().m_get( key );
        if( !inst ) inst = x_inst_create( o.TE() );
        if( !inst ) ERR_fa( "'#<sc_t>.m_getf:' stamp must define transient type 'TE'.", bnameof( o._ ) );
        = inst;
    }
};

//----------------------------------------------------------------------------------------------------------------------

embed "bcore_x_hmap.x";

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_HMAP_H
