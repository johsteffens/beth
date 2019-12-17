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

/** Beth inexpandable group.
 *  This group does not generate effective plat-code but is otherwise accessible within the plant framework.
 *  It is used to add components, which have a low-level implementation: E.g. beth-system features.
 */

#ifndef BCORE_PLANT_INEXPANDABLE_H
#define BCORE_PLANT_INEXPANDABLE_H

#include "bcore_std.h"
#include "bcore_planted.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_fp
PLANT_GROUP( bcore_fp, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

expandable = false;

feature '' void copy_typed( mutable, tp_t type, vc_t src );

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // optional enclosure

/**********************************************************************************************************************/

vd_t bcore_plant_inexpandable_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BCORE_PLANT_INEXPANDABLE_H
