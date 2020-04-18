/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include "bclos_library.h"
#include "bclos_quicktypes.h"
#include "bcore_spect_inst.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTION_INIT_INST( bclos_library_s )
BCORE_DEFINE_FUNCTION_DOWN_INST( bclos_library_s )
BCORE_DEFINE_FUNCTION_COPY_INST( bclos_library_s )
BCORE_DEFINE_FUNCTION_CREATE(     bclos_library_s )
BCORE_DEFINE_FUNCTION_DISCARD(    bclos_library_s )
BCORE_DEFINE_FUNCTION_CLONE(      bclos_library_s )

static bcore_self_s* library_s_create_self( void )
{
    sc_t def = "bclos_library_s = bcore_inst { aware_t _; bcore_hmap_tp_sr_s map; bclos_library_arr_s* deps; }";
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( def, bclos_library_s );
    return self;
}

/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTION_INIT_INST( bclos_library_arr_s )
BCORE_DEFINE_FUNCTION_DOWN_INST( bclos_library_arr_s )
BCORE_DEFINE_FUNCTION_COPY_INST( bclos_library_arr_s )
BCORE_DEFINE_FUNCTION_CREATE(     bclos_library_arr_s )
BCORE_DEFINE_FUNCTION_DISCARD(    bclos_library_arr_s )
BCORE_DEFINE_FUNCTION_CLONE(      bclos_library_arr_s )

static bcore_self_s* library_arr_s_create_self( void )
{
    sc_t def = "bclos_library_arr_s = bcore_inst_s { aware_t _; library_s* [] arr; }";
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( def, bclos_library_arr_s );
    return self;
}

/**********************************************************************************************************************/

vd_t bclos_library_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bclos_library" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "bclos_library_s" ), library_s_create_self );
            bcore_flect_define_creator( typeof( "bclos_library_arr_s" ), library_arr_s_create_self );
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/


