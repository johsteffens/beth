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

/** Pseudo Random Sequence generators.
 *  Not suitable for cryptographic purposes.
 */

/**********************************************************************************************************************/

#ifndef BCORE_PRSG_H
#define BCORE_PRSG_H

#include "bcore_xoila.h"
#include "bcore_life.h"
#include "bcore_leaf.h"
#include "bcore_control.h"
#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bcore_prsg, bcore_inst )
#ifdef XOILA_SECTION //-------------------------------------------------------------------------------------------------

/** Number of active bits in the u3 value of the generator.
 *  Active bits are right-bound (low order bits).
 *  Inactive bits are always zero.
 */
feature strict sz_t bits( c @* o );

/// maximum possible u3 value
feature strict u3_t max_u3( c @* o );

/// minimum possible u3 value
feature strict u3_t min_u3( c @* o );

/** Function state_* computes the return value from the current state.
 *  The state is not changed.
 */
feature strict u3_t state_u3     ( c @* o );
feature strict u3_t state_bits_u3( c @* o, sz_t bits ); // returns 'bits' bits of the random random state; value range range: [0, (2^bits)-1]
feature strict f3_t state_f3     ( c @* o, f3_t min, f3_t max ); // return value is evenly distributed withing the range [min,max]
feature        bl_t state_bl     ( c @* o ) { = o.state_bits_u3( 1 ) ? true : false; };

/** Function gen* generates a subsequent state.
 *  A return value is computed from the new state according to function state*.
 */
feature strict o    gen        ( m @* o );
feature strict u3_t gen_bits_u3( m @* o, sz_t bits );
feature strict u3_t gen_u3     ( m @* o );
feature strict f3_t gen_f3     ( m @* o, f3_t min, f3_t max );
feature        bl_t gen_bl     ( m @* o ) { = o.gen_bits_u3( 1 ) ? true : false; };

/** Computes a new state derived from a u3_t seed.
 *  Note: Function state_u3 above does not necessarily return that seed value.
 */
feature strict o set_state_u3( m @* o, u3_t seed );

/// Computes a new state derived from state of a and b.
/* Different mixing methods are thinkable:
 * Adding, multiplying or xoring should all be suitable.
 * We should just stick to the same method.
 */
feature o set_state_mix( m @* o, c :* a, c :* b ) = o.set_state_u3( a.state_u3() + b.state_u3() );

/// deprecated: use set_state_u3
feature o reseed( m @* o, u3_t seed ) = o.set_state_u3( seed );

//----------------------------------------------------------------------------------------------------------------------

stump :general_s :
{
    func ^.gen_u3 = o.gen().state_u3();

    func ^.state_bits_u3
    {
        assert( bits >= 0 && bits <= 64 );
        sz_t o_bits = o.bits();
        if( o_bits >= bits )
        {
            = o.state_u3() >> ( o_bits - bits );
        }
        else
        {
            = o.state_u3() << ( bits - o_bits );
        }
    }

    func ^.gen_bits_u3 = o.gen().state_bits_u3( bits );
    func ^.state_f3 = min + ( max - min ) * ( ( f3_t )( o.state_u3() - o.min_u3() ) / ( o.max_u3() - o.min_u3() ) );
    func ^.gen_f3 = o.gen().state_f3( min, max );
}

//----------------------------------------------------------------------------------------------------------------------

// linear congruential generators
group :lcg
{

    stump :u2_s extending ::general_s
    {
        u2_t state      = 16437;
        func ^.bits     = 32;
        func ^.max_u3   = 0xFFFFFFFFu;
        func ^.min_u3   = 0;
        func ^.state_u3 = o.state;
        func ^.set_state_u3 o.state = seed ^ ( seed >> 32 );
    };

    //------------------------------------------------------------------------------------------------------------------

    stump :u3_s extending ::general_s
    {
        u3_t state = 16437;
        func ^.bits     = 64;
        func ^.max_u3   = 0xFFFFFFFFFFFFFFFFull;
        func ^.min_u3   = 0;
        func ^.state_u3 = o.state;
        func ^.set_state_u3 o.state = seed;
    }

    //------------------------------------------------------------------------------------------------------------------

    stamp :u2_00_s extending :u2_s func ^.gen o.state = bcore_lcg00_u2( o.state );
    stamp :u2_01_s extending :u2_s func ^.gen o.state = bcore_lcg01_u2( o.state );
    stamp :u2_02_s extending :u2_s func ^.gen o.state = bcore_lcg02_u2( o.state );
    stamp :u2_03_s extending :u2_s func ^.gen o.state = bcore_lcg03_u2( o.state );
    stamp :u2_04_s extending :u2_s func ^.gen o.state = bcore_lcg04_u2( o.state );
    stamp :u2_05_s extending :u2_s func ^.gen o.state = bcore_lcg05_u2( o.state );

    //------------------------------------------------------------------------------------------------------------------

    stamp :u3_00_s extending :u3_s func ^.gen o.state = bcore_lcg00_u3( o.state );
    stamp :u3_01_s extending :u3_s func ^.gen o.state = bcore_lcg01_u3( o.state );
    stamp :u3_02_s extending :u3_s func ^.gen o.state = bcore_lcg02_u3( o.state );
    stamp :u3_03_s extending :u3_s func ^.gen o.state = bcore_lcg03_u3( o.state );
}

//----------------------------------------------------------------------------------------------------------------------

// xor shift generators
group :xsg
{

    /// internal state is u2_t
    stump :u2_s extending ::general_s
    {
        u2_t state = 16432;
        func ^.bits     = 32;
        func ^.max_u3   = 0xFFFFFFFFu;
        func ^.min_u3   = 1;
        func ^.state_u3 = o.state;
        func ^.set_state_u3 o.state = u2_max( 1, seed ^ ( seed >> 32 ) );
    }

    //------------------------------------------------------------------------------------------------------------------

    stamp :u2_00_s extending :u2_s func ^.gen o.state = bcore_xsg_u2(  o.state );
    stamp :u2_01_s extending :u2_s func ^.gen o.state = bcore_xsg1_u2( o.state );
    stamp :u2_02_s extending :u2_s func ^.gen o.state = bcore_xsg2_u2( o.state );
    stamp :u2_03_s extending :u2_s func ^.gen o.state = bcore_xsg3_u2( o.state );
}

#endif // XOILA_SECTION ------------------------------------------------------------------------------------------------

vd_t bcore_prsg_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_PRSG_H
