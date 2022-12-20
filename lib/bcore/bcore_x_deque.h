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

/** Double ended queue generic framework.

    Purpose:
      Quickly creating a new deque-stamp with all relevant functions predefined.

    Usage:
    - Define a stamp with x_deque in ts traitline.
    - Set type identifier 'TE' to the target object.
    - Place x_deque_inst_s as first object of the stamp. The name is irrelevant.

    Example:
      stamp my_deque_my_object_s x_deque trans(TE my_object_s)
      {
          x_deque_inst_s deque; // x_deque_inst_s must be first; the name is irrelevant.

          // for minimum latency, it is recommended to overload three functions rather than using the group functions
          func x_deque.size  = o.deque.size;
          func x_deque.m_get = o.deque.m_get( index ).cast( m my_object_s* );
          func x_deque.c_get = o.deque.c_get( index ).cast( c my_object_s* );

          // additional objects and functions follow here:
          ....
      }

 */

#ifndef BCORE_X_DEQUE_H
#define BCORE_X_DEQUE_H

#include "bcore_x_array.h"
#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_deque, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

/// Interface
signature o              set_space( m@* o, sz_t space );
signature o              clear(   m@* o );
signature sz_t           size(    c@* o );
signature m (TE) x_inst* m_get(   m@* o, sz_t index ); // returns NULL if index is out of range
signature c (TE) x_inst* c_get(   c@* o, sz_t index ); // returns NULL if index is out of range
signature m (TE) x_inst* m_get_first( m@* o ); // returns NULL if size is 0
signature c (TE) x_inst* c_get_first( c@* o ); // returns NULL if size is 0
signature m (TE) x_inst* m_get_last(  m@* o ); // returns NULL if size is 0
signature c (TE) x_inst* c_get_last(  c@* o ); // returns NULL if size is 0
signature m (TE) x_inst* push_last_d ( m@* o, d (TE) x_inst* inst ); // push to end of queue
signature m (TE) x_inst* push_first_d( m@* o, d (TE) x_inst* inst ); // push to front of queue
signature d (TE) x_inst* d_pop_last (  m@* o );                      // pop from end of queue
signature d (TE) x_inst* d_pop_first(  m@* o );                      // pop from front of queue
signature o remove_last (   m@* o );   // remove 1 from end of queue if possible
signature o remove_first(   m@* o );   // remove 1 from front of queue if possible
signature o remove_last_n ( m@* o, sz_t n ); // remove (maximally) n from end of queue
signature o remove_first_n( m@* o, sz_t n ); // remove (maximally) n from front of queue

//----------------------------------------------------------------------------------------------------------------------

stamp :inst_adl_s x_array { x_inst => []; };
stamp :inst_s
{
    :inst_adl_s adl;
    sz_t size;
    sz_t first;

    func o set_space( m@* o, sz_t space )
    {
        ASSERT( space >= o.size );
        o.adl.set_space( space );
    }

    func o clear( m@* o )
    {
        o.adl.clear();
        o.first = o.size = 0;
    }

    func sz_t adl_idx( c@* o, sz_t index ) = ( o.first + index ) % o.adl.size;
    func m x_inst* m_get( m@* o, sz_t index ) = ( index >= 0 && index < o.size ) ? o.adl.[ o.adl_idx( index ) ] : NULL;
    func c x_inst* c_get( c@* o, sz_t index ) = ( index >= 0 && index < o.size ) ? o.adl.[ o.adl_idx( index ) ] : NULL;
    func m x_inst* m_get_first( m@* o ) = ( o.size > 0 ) ? o.adl.[ o.first ] : NULL;
    func c x_inst* c_get_first( c@* o ) = ( o.size > 0 ) ? o.adl.[ o.first ] : NULL;
    func m x_inst* m_get_last(  m@* o ) = ( o.size > 0 ) ? o.adl.[ o.adl_idx( o.size - 1 ) ] : NULL;
    func c x_inst* c_get_last(  c@* o ) = ( o.size > 0 ) ? o.adl.[ o.adl_idx( o.size - 1 ) ] : NULL;

    func o inc_adl_size( m@* o )
    {
        o.adl.set_size( sz_max( 1, o.adl.size * 2 ) );
        for( sz_t i = 0; i < o.first; i++ )
        {
            o.adl.[ i + o.size ] = o.adl.[ i ];
            o.adl.[ i          ] = NULL;
        }
    }

    func m x_inst* push_last_d( m@* o, d x_inst* inst )
    {
        if( o.size == o.adl.size ) o.inc_adl_size();
        o.adl.[ ( o.first + o.size ) % o.adl.size ] =< inst;
        o.size++;
        = inst.cast( m$* );
    }

    func m x_inst* push_first_d( m@* o, d x_inst* inst )
    {
        if( o.size == o.adl.size )  o.inc_adl_size();
        o.first = ( o.first == 0 ) ? o.adl.size - 1 : o.first - 1;
        o.adl.[ o.first ] =< inst;
        o.size++;
        = inst.cast( m$* );
    }

    func d x_inst* d_pop_last( m@* o )
    {
        if( o.size == 0 ) = NULL;
        sz_t adl_idx = o.adl_idx( o.size - 1 );
        d x_inst* inst = o.adl.[ adl_idx ].cast( d x_inst* );
        o.adl.[ adl_idx ] = NULL;
        o.size--;
        = inst;
    }

    func d x_inst* d_pop_first( m@* o )
    {
        if( o.size == 0 ) = NULL;
        d x_inst* inst = o.adl.[ o.first ].cast( d x_inst* );
        o.adl.[ o.first ] = NULL;
        o.size--;
        o.first = ( o.first + 1 ) % o.adl.size;
        = inst;
    }

    func :.remove_first o.d_pop_first().discard();
    func :.remove_last  o.d_pop_last().discard();

    func :.remove_first_n
    {
        for( sz_t i = 0; i < n && o.size > 0; i++ ) o.remove_first();
    }

    func :.remove_last_n
    {
        for( sz_t i = 0; i < n && o.size > 0; i++ ) o.remove_last();
    }
};

//----------------------------------------------------------------------------------------------------------------------

func m :inst_s* m_inst_( m@* o )
{
    assert( o.cast( x_stamp* ).is_aware() );
    m x_inst* que = o.cast( m x_stamp* ).m_get_i( 0 );
    if( !que || que._ != TYPEOF_:inst_s ) ERR_fa( "First element of '#<sc_t>' must be of type '#<sc_t>'", bnameof( o._ ), bnameof( :inst_s~ ) );
    = que.cast(m :inst_s*);
}

func c :inst_s* c_inst_( c@* o )
{
    assert( o.cast( x_stamp* ).is_aware() );
    c x_inst* que = o.cast( m x_stamp* ).c_get_i( 0 );
    if( !que || que._ != TYPEOF_:inst_s ) ERR_fa( "First element of '#<sc_t>' must be of type '#<sc_t>'", bnameof( o._ ), bnameof( :inst_s~ ) );
    = que.cast(c :inst_s*);
}

func set_space { o.m_inst_().set_space( space ); = o; }
func clear     { o.m_inst_().clear();            = o; }
func size         = o.c_inst_().size;
func m_get        = o.m_inst_().m_get( index );
func c_get        = o.c_inst_().c_get( index );
func m_get_first  = o.m_inst_().m_get_first();
func c_get_first  = o.c_inst_().c_get_first();
func m_get_last   = o.m_inst_().m_get_last();
func c_get_last   = o.c_inst_().c_get_last();
func push_last_d  = o.m_inst_().push_last_d( inst );
func push_first_d = o.m_inst_().push_first_d( inst );
func d_pop_last   = o.m_inst_().d_pop_last();
func d_pop_first  = o.m_inst_().d_pop_first();
func remove_last  = o.m_inst_().remove_last();
func remove_first = o.m_inst_().remove_first();
func remove_last_n  = o.m_inst_().remove_last_n( n );
func remove_first_n = o.m_inst_().remove_first_n( n );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_DEQUE_H
