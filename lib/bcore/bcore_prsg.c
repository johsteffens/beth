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

#include "bcore_prsg.h"

/**********************************************************************************************************************/

static void selftest( void )
{
    BLM_INIT();
    bcore_prsg* prsg = ( bcore_prsg* )BLM_CREATE( bcore_prsg_lcg_u3_00_s );
    bcore_prsg_a_reseed( prsg, 1234 );

    /// same sequence should be reproducible on different platforms
    ASSERT( bcore_prsg_a_gen_u3( prsg ) ==  6370341672916135467u );
    ASSERT( bcore_prsg_a_gen_u3( prsg ) ==  9413649121373430640u );
    ASSERT( bcore_prsg_a_gen_u3( prsg ) ==  5049661203087052977u );
    ASSERT( bcore_prsg_a_gen_u3( prsg ) == 15841467423200399038u );

    BLM_DOWN();
}


/**********************************************************************************************************************/

vd_t bcore_prsg_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_prsg" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            selftest();
        }
        break;

        default: break;
    }
    return NULL;
}

/**********************************************************************************************************************/
