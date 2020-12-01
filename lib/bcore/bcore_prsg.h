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
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Number of active bits in the u3 value of the generator.
 *  Active bits are right-bound (low order bits).
 *  Inactive bits are always zero.
 */
feature strict sz_t bits(   const );

/// maximum possible u3 value
feature strict u3_t max_u3( const );

/// minimum possible u3 value
feature strict u3_t min_u3( const );

/** Function state_* computes the return value from the current state.
 *  The state is not changed.
 */
feature strict u3_t state_u3     ( const );
feature strict u3_t state_bits_u3( const, sz_t bits ); // returns 'bits' bits of the random random state; value range range: [0, (2^bits)-1]
feature strict f3_t state_f3     ( const, f3_t min, f3_t max ); // return value is evenly distributed withing the range [min,max]
feature        bl_t state_bl     ( const ) = { return :a_state_bits_u3( o, 1 ) ? true : false; };

/** Function gen* generates a subsequent state.
 *  A return value is computed from the new state according to function state*.
 */
feature strict void gen        ( mutable );
feature strict u3_t gen_bits_u3( mutable, sz_t bits );
feature strict u3_t gen_u3     ( mutable );
feature strict f3_t gen_f3     ( mutable, f3_t min, f3_t max );
feature        bl_t gen_bl     ( mutable ) = { return :a_gen_bits_u3( o, 1 ) ? true : false; };

/** Computes a new state derived from a u3_t seed.
 *  Note: Function state_u3 above does not necessarily return that seed value.
 */
feature strict :* set_state_u3( mutable, u3_t seed );

/// Computes a new state derived from state of a and b.
feature :*  set_state_mix( mutable, const :* a, const :* b ) =
{
    /* Different mixing methods are thinkable:
     * Adding, multiplying or xoring should all be suitable.
     * We should just stick to the same method.
     */
    return :a_set_state_u3( o, :a_state_u3( a ) + :a_state_u3( b ) );
};

/// deprecated: use set_state_u3
feature void reseed( mutable, u3_t seed ) = { :a_set_state_u3( o, seed ); };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stump :general = aware :
{
    func ^ .gen_u3 =
    {
        @_gen( o );
        return @_state_u3( o );
    };

    func ^ .state_bits_u3 =
    {
        assert( bits >= 0 && bits <= 64 );
        sz_t o_bits = @_bits( o );
        if( o_bits >= bits )
        {
            return @_state_u3( o ) >> ( o_bits - bits );
        }
        else
        {
            return @_state_u3( o ) << ( bits - o_bits );
        }
    };

    func ^ .gen_bits_u3 =
    {
        @_gen( o );
        return @_state_bits_u3( o, bits );
    };

    func ^ .state_f3 =
    {
        return min + ( max - min ) * ( ( f3_t )( @_state_u3( o ) - @_min_u3( o ) ) / ( @_max_u3( o ) - @_min_u3( o ) ) );
    };

    func ^ .gen_f3 =
    {
        @_gen( o );
        return @_state_f3( o, min, max );
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// linear congruential generators
group :lcg =
{

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    stump :u2 = extending ::general
    {
        u2_t state = 16437;
        func ^ .bits   = { return 32; };
        func ^ .max_u3 = { return 0xFFFFFFFFu; };
        func ^ .min_u3 = { return 0; };
        func ^ .state_u3 = { return o->state; };
        func ^ .set_state_u3 = { o->state = seed ^ ( seed >> 32 ); return (::*)o; };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    stump :u3 = extending ::general
    {
        u3_t state = 16437;
        func ^ .bits   = { return 64; };
        func ^ .max_u3 = { return 0xFFFFFFFFFFFFFFFFull; };
        func ^ .min_u3 = { return 0; };
        func ^ .state_u3 = { return o->state; };
        func ^ .set_state_u3 = { o->state = seed; return (::*)o; };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    stamp :u2_00 = extending :u2 { func ^ .gen = { o->state = bcore_lcg00_u2( o->state ); }; };
    stamp :u2_01 = extending :u2 { func ^ .gen = { o->state = bcore_lcg01_u2( o->state ); }; };
    stamp :u2_02 = extending :u2 { func ^ .gen = { o->state = bcore_lcg02_u2( o->state ); }; };
    stamp :u2_03 = extending :u2 { func ^ .gen = { o->state = bcore_lcg03_u2( o->state ); }; };
    stamp :u2_04 = extending :u2 { func ^ .gen = { o->state = bcore_lcg04_u2( o->state ); }; };
    stamp :u2_05 = extending :u2 { func ^ .gen = { o->state = bcore_lcg05_u2( o->state ); }; };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    stamp :u3_00 = extending :u3 { func ^ .gen = { o->state = bcore_lcg00_u3( o->state ); }; };
    stamp :u3_01 = extending :u3 { func ^ .gen = { o->state = bcore_lcg01_u3( o->state ); }; };
    stamp :u3_02 = extending :u3 { func ^ .gen = { o->state = bcore_lcg02_u3( o->state ); }; };
    stamp :u3_03 = extending :u3 { func ^ .gen = { o->state = bcore_lcg03_u3( o->state ); }; };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// xor shift generators
group :xsg =
{

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    /// internal state is u2_t
    stump :u2 = extending ::general
    {
        u2_t state = 16432;
        func ^ .bits   = { return 32; };
        func ^ .max_u3 = { return 0xFFFFFFFFu; };
        func ^ .min_u3 = { return 1; };
        func ^ .state_u3 = { return o->state; };
        func ^ .set_state_u3 = { o->state = u2_max( 1, seed ^ ( seed >> 32 ) ); return (::*)o; };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    stamp :u2_00 = extending :u2 { func ^ .gen = { o->state = bcore_xsg_u2(  o->state ); }; };
    stamp :u2_01 = extending :u2 { func ^ .gen = { o->state = bcore_xsg1_u2( o->state ); }; };
    stamp :u2_02 = extending :u2 { func ^ .gen = { o->state = bcore_xsg2_u2( o->state ); }; };
    stamp :u2_03 = extending :u2 { func ^ .gen = { o->state = bcore_xsg3_u2( o->state ); }; };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

};

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

vd_t bcore_prsg_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_PRSG_H
