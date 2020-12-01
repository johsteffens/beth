/** Author and Copyright 2019 Johannes Bernhard Steffens
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

#ifndef BHVM_HOP_H
#define BHVM_HOP_H

/**********************************************************************************************************************/

/** Collection of Holor-Operators
 *
 */

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm.xo.h"
#include "bhvm_holor.h"

/**********************************************************************************************************************/
/// Macros

#define BODY_ASSERT_VVV assert( a->v.size == r->v.size ); assert( b->v.size == r->v.size );
#define BODY_ASSERT_VSV assert( a->v.size == r->v.size ); assert( b->v.size == 1         );
#define BODY_ASSERT_SVV assert( a->v.size == 1         ); assert( b->v.size == r->v.size );
#define BODY_ASSERT_VVS assert( a->v.size == b->v.size ); assert( r->v.size == 1         );
#define BODY_ASSERT_MVV assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 0 ] ); assert( r->v.size == a->s.data[ 1 ] );
#define BODY_ASSERT_VMV assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 1 ] ); assert( r->v.size == b->s.data[ 0 ] );
#define BODY_ASSERT_VVM assert( r->s.size == 2 ); assert( a->v.size == r->s.data[ 1 ] ); assert( b->v.size == r->s.data[ 0 ] );
#define BODY_ASSERT_TVV assert( a->s.size == 2 ); assert( b->v.size == a->s.data[ 1 ] ); assert( r->v.size == a->s.data[ 0 ] );
#define BODY_ASSERT_VTV assert( b->s.size == 2 ); assert( a->v.size == b->s.data[ 0 ] ); assert( r->v.size == b->s.data[ 1 ] );
#define BODY_ASSERT_VVT assert( r->s.size == 2 ); assert( a->v.size == r->s.data[ 0 ] ); assert( b->v.size == r->s.data[ 1 ] );

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bhvm_hop, bcore_inst, "bhvm_hop.x" )

/**********************************************************************************************************************/

vd_t bhvm_hop_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_HOP_H
