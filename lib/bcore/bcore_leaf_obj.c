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

#include "bcore_leaf_obj.h"

/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( u0_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( u1_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( u2_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( u3_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( s0_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( s1_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( s2_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( s3_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( f2_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( f3_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( sz_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( uz_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( bl_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( tp_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( er_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( aware_t )
BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( offset_t )

/**********************************************************************************************************************/

vd_t bcore_leaf_obj_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_leaf_obj" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

