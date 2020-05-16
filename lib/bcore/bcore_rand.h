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

#ifndef BCORE_RAND_H
#define BCORE_RAND_H

/**********************************************************************************************************************/

/** Pseudo Random Generator Framework */

/**********************************************************************************************************************/

#include "bcore_plant_compiler.h"
#include "bcore_planted.h"
#include "bcore_trait.h"
#include "bcore_st.h"
#include "bcore_spect_source.h"
#include "bcore_spect_sink.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_rand

PLANT_GROUP( bcore_rand, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

feature 'a' u3_t max_u3( const ); // maximum possible u3 value for given generator
feature 'a' u3_t min_u3( const ); // minimum possible u3 value for given generator
feature 'a' u3_t gen_u3( mutable ); // generates an integer random value
feature 'a' f3_t gen_f3( mutable, f3_t min, f3_t max ); // generates a f3_t value within [min, max]
feature 'a' void reseed(  mutable, u3_t seed ); // reseeds generator using an u3 seed (all seeds are allowed)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// lgc_u2
stamp :lcg  = aware :
{
    u2_t state = 16432;
    func : :max_u3 = { return 0xFFFFFFFFu; };
    func : :min_u3 = { return 0; };
    func : :gen_u3 = { return o->state = bcore_lcg_u2( o->state ); };
    func : :gen_f3 = { return min + ( max - min ) * ( ( f3_t )( @_gen_u3( o ) - @_min_u3( o ) ) / ( @_max_u3( o ) - @_min_u3( o ) ) ); };
    func : :reseed  = { o->state = seed; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// xsg1_u3
stamp :xsg  = aware :
{
    u2_t state = 13754;
    func : :max_u3 = { return 0xFFFFFFFFu; };
    func : :min_u3 = { return 1; };
    func : :gen_u3 = { return o->state = bcore_xsg1_u2( o->state ); };
    func : :gen_f3 = { return min + ( max - min ) * ( ( f3_t )( @_gen_u3( o ) - @_min_u3( o ) ) / ( @_max_u3( o ) - @_min_u3( o ) ) ); };
    func : :reseed  = { o->state = ( ( seed & 0xFFFFFFFFu ) == 0 ) ? 1 : seed; };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/** Two lcg and two xsg generators each with a period of 2^64.
 *  Shifted and xored. (Variation on Marsaglia's idea: KISS)
 */
stamp :lxm  = aware :
{
    u3_t l0 = 27458;
    u3_t l1 = 33296;
    u3_t x0 = 23532;
    u3_t x1 = 10954;

};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bcore_rand

/**********************************************************************************************************************/

vd_t bcore_rand_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_RAND_H
