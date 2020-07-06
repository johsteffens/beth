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

#include "bcore_plant.h"
#include "bcore_leaf.h"
#include "bcore_control.h"
#include "bcore_planted.h"

/**********************************************************************************************************************/

BETH_PLANT_DEFINE_GROUP( bcore_prsg, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'a' sz_t bits(   const ); // Number of (rightbound) active bits in the u3 value of the generator
feature 'a' u3_t max_u3( const ); // maximum possible u3 value for given generator
feature 'a' u3_t min_u3( const ); // minimum possible u3 value for given generator
feature 'a' u3_t gen_u3( mutable ); // generates an integer random value
feature 'a' f3_t gen_f3( mutable, f3_t min, f3_t max ); // generates a f3_t value within [min, max]
feature 'a' void reseed( mutable, u3_t seed ); // resets generator using an u3 seed (all seeds are allowed)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stump :general = aware :
{
    func ^ :gen_f3 =
    {
        return min + ( max - min ) * ( ( f3_t )( @_gen_u3( o ) - @_min_u3( o ) ) / ( @_max_u3( o ) - @_min_u3( o ) ) );
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
        func ^ :bits   = { return 32; };
        func ^ :max_u3 = { return 0xFFFFFFFFu; };
        func ^ :min_u3 = { return 0; };
        func ^ :reseed = { o->state = seed ^ ( seed >> 32 ); };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    stump :u3 = extending ::general
    {
        u3_t state = 16437;
        func ^ :bits   = { return 64; };
        func ^ :max_u3 = { return 0xFFFFFFFFFFFFFFFFull; };
        func ^ :min_u3 = { return 0; };
        func ^ :reseed = { o->state = seed; };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    stamp :u2_00 = extending :u2 { func ^ :gen_u3 = { return ( o->state = bcore_lcg00_u2( o->state ) ); }; };
    stamp :u2_01 = extending :u2 { func ^ :gen_u3 = { return ( o->state = bcore_lcg01_u2( o->state ) ); }; };
    stamp :u2_02 = extending :u2 { func ^ :gen_u3 = { return ( o->state = bcore_lcg02_u2( o->state ) ); }; };
    stamp :u2_03 = extending :u2 { func ^ :gen_u3 = { return ( o->state = bcore_lcg03_u2( o->state ) ); }; };
    stamp :u2_04 = extending :u2 { func ^ :gen_u3 = { return ( o->state = bcore_lcg04_u2( o->state ) ); }; };
    stamp :u2_05 = extending :u2 { func ^ :gen_u3 = { return ( o->state = bcore_lcg05_u2( o->state ) ); }; };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    stamp :u3_00 = extending :u3 { func ^ :gen_u3 = { return ( o->state = bcore_lcg00_u3( o->state ) ); }; };
    stamp :u3_01 = extending :u3 { func ^ :gen_u3 = { return ( o->state = bcore_lcg01_u3( o->state ) ); }; };
    stamp :u3_02 = extending :u3 { func ^ :gen_u3 = { return ( o->state = bcore_lcg02_u3( o->state ) ); }; };
    stamp :u3_03 = extending :u3 { func ^ :gen_u3 = { return ( o->state = bcore_lcg03_u3( o->state ) ); }; };

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// xor shift generators
group :xsg =
{

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    stump :u2 = extending ::general
    {
        u2_t state = 16432;
        func ^ :bits   = { return 32; };
        func ^ :max_u3 = { return 0xFFFFFFFFu; };
        func ^ :min_u3 = { return 1; };
        func ^ :reseed = { o->state = u2_max( 1, seed ^ ( seed >> 32 ) ); };
    };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    stamp :u2_00 = extending :u2 { func ^ :gen_u3 = { return ( o->state = bcore_xsg_u2(  o->state ) ); }; };
    stamp :u2_01 = extending :u2 { func ^ :gen_u3 = { return ( o->state = bcore_xsg1_u2( o->state ) ); }; };
    stamp :u2_02 = extending :u2 { func ^ :gen_u3 = { return ( o->state = bcore_xsg2_u2( o->state ) ); }; };
    stamp :u2_03 = extending :u2 { func ^ :gen_u3 = { return ( o->state = bcore_xsg3_u2( o->state ) ); }; };

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

};

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

vd_t bcore_prsg_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_PRSG_H
