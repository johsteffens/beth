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

#ifndef BHVM_LOP_H
#define BHVM_LOP_H

/**********************************************************************************************************************/

/** Collection of Leaf-Operators (Element Operators)
 *  A leaf operator has leaf types as operands. In this case these types are floating point types.
 *  On certain extended types (vectors, matrices, holors) leaf operator can be applied in vectorized manner.
 *
 *  Object Nomenclature:
 *    Dendrite pass is specified by the object (not function)
 *    Suffix: dp_<signature>
 *
 *  Function Nomenclature:
 *    f    - plain pass
 *   of    - functor pass
 *
 */

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm.xo.h"
#include "bhvm_lop.xo.h"

/**********************************************************************************************************************/

#define SIZEOF_FX( type ) ( ( type == TYPEOF_f2_t ) ? sizeof( f2_t ) : ( type == TYPEOF_f3_t ) ? sizeof( f3_t ) : -1 )

/// type knitting macros
#define BKNIT_FA1( type ) ( ( type == TYPEOF_f2_t ) ? 0 : ( type == TYPEOF_f3_t ) ? 1 : -256 )
#define BKNIT_FA1_ERR( t1             ) ERR_fa( "Invalid type: '#<sc_t>'.", ifnameof( t1 ) )
#define BKNIT_FA2_ERR( t1, t2         ) ERR_fa( "Invalid type combination: '#<sc_t>', '#<sc_t>'.", ifnameof( t1 ), ifnameof( t2 ) )
#define BKNIT_FA3_ERR( t1, t2, t3     ) ERR_fa( "Invalid type combination: '#<sc_t>', '#<sc_t>', '#<sc_t>'.", ifnameof( t1 ), ifnameof( t2 ), ifnameof( t3 ) )
#define BKNIT_FA4_ERR( t1, t2, t3, t4 ) ERR_fa( "Invalid type combination: '#<sc_t>', '#<sc_t>', '#<sc_t>', '#<sc_t>'.", ifnameof( t1 ), ifnameof( t2 ), ifnameof( t3 ), ifnameof( t4 ) )

#define BKNIT_FA2( t1, t2 )         ( BKNIT_FA1( t1 )         * 2 + BKNIT_FA1( t2 ) )
#define BKNIT_FA3( t1, t2, t3 )     ( BKNIT_FA2( t1, t2 )     * 2 + BKNIT_FA1( t3 ) )
#define BKNIT_FA4( t1, t2, t3, t4 ) ( BKNIT_FA3( t1, t2, t3 ) * 2 + BKNIT_FA1( t4 ) )

#define BKNIT_FA_L1_FROM_KNIT( knit ) (   knit        & 1 )
#define BKNIT_FA_L2_FROM_KNIT( knit ) ( ( knit >> 1 ) & 1 )
#define BKNIT_FA_L3_FROM_KNIT( knit ) ( ( knit >> 2 ) & 1 )
#define BKNIT_FA_L4_FROM_KNIT( knit ) ( ( knit >> 3 ) & 1 )

#define BKNIT_F2 BKNIT_FA1( TYPEOF_f2_t )
#define BKNIT_F3 BKNIT_FA1( TYPEOF_f3_t )

#define BKNIT_F22 BKNIT_FA2( TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F23 BKNIT_FA2( TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F32 BKNIT_FA2( TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F33 BKNIT_FA2( TYPEOF_f3_t, TYPEOF_f3_t )

#define BKNIT_F222 BKNIT_FA3( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F223 BKNIT_FA3( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F232 BKNIT_FA3( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F233 BKNIT_FA3( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f3_t )
#define BKNIT_F322 BKNIT_FA3( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F323 BKNIT_FA3( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F332 BKNIT_FA3( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F333 BKNIT_FA3( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f3_t )

#define BKNIT_F2222 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F2223 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F2232 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F2233 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f3_t )
#define BKNIT_F2322 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F2323 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F2332 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F2333 BKNIT_FA4( TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f3_t )
#define BKNIT_F3222 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F3223 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F3232 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F3233 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f3_t, TYPEOF_f3_t )
#define BKNIT_F3322 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f2_t )
#define BKNIT_F3323 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f2_t, TYPEOF_f3_t )
#define BKNIT_F3332 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f2_t )
#define BKNIT_F3333 BKNIT_FA4( TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f3_t, TYPEOF_f3_t )

/**********************************************************************************************************************/

/// greatest common divisor
static inline sz_t bhvm_lop_gcd2( sz_t a, sz_t b ) { return ( a == 0 ) ? b : bhvm_lop_gcd2( b % a, a ); }
static inline sz_t bhvm_lop_gcd3( sz_t a, sz_t b, sz_t c ) { return bhvm_lop_gcd2( a, bhvm_lop_gcd2( b, c ) ); }

XOILA_DEFINE_GROUP( bhvm_lop, bcore_inst, "bhvm_lop.x" )

/**********************************************************************************************************************/

#endif // BHVM_LOP_H
