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

#ifndef BCORE_LEAF_OBJ_H
#define BCORE_LEAF_OBJ_H

/**********************************************************************************************************************/

/// Turns leaf types into regular objects by defining object related functions

/**********************************************************************************************************************/

#include "bcore_leaf.h"
#include "bcore_types.h"
#include "bcore_control.h"

/**********************************************************************************************************************/

BCORE_DECLARE_FUNCTIONS_OBJ( u0_t )
BCORE_DECLARE_FUNCTIONS_OBJ( u1_t )
BCORE_DECLARE_FUNCTIONS_OBJ( u2_t )
BCORE_DECLARE_FUNCTIONS_OBJ( u3_t )
BCORE_DECLARE_FUNCTIONS_OBJ( s0_t )
BCORE_DECLARE_FUNCTIONS_OBJ( s1_t )
BCORE_DECLARE_FUNCTIONS_OBJ( s2_t )
BCORE_DECLARE_FUNCTIONS_OBJ( s3_t )
BCORE_DECLARE_FUNCTIONS_OBJ( f2_t )
BCORE_DECLARE_FUNCTIONS_OBJ( f3_t )
BCORE_DECLARE_FUNCTIONS_OBJ( sz_t )
BCORE_DECLARE_FUNCTIONS_OBJ( uz_t )
BCORE_DECLARE_FUNCTIONS_OBJ( bl_t )
BCORE_DECLARE_FUNCTIONS_OBJ( tp_t )
BCORE_DECLARE_FUNCTIONS_OBJ( er_t )
BCORE_DECLARE_FUNCTIONS_OBJ( aware_t )
BCORE_DECLARE_FUNCTIONS_OBJ( offset_t )

/**********************************************************************************************************************/

vd_t bcore_leaf_obj_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_LEAF_OBJ_H

