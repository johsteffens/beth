/** Author and Copyright 2021 Johannes Bernhard Steffens
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

/** Beth group reflection. Navigating inside a group. */

#ifndef BCORE_X_GROUP_H
#define BCORE_X_GROUP_H

#include "bcore.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( x_group, x_inst )

#ifdef XOILA_SECTION

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Returns array of each stamp that has given group in its traitline.
 *  Note: Only for 'retrievable' groups.
 *  Returns NULL in case group does not exist or is not retrievable.
 */
func (bcore_arr_tp_s* get_arr_traitline_stamps( tp_t group )) { return bcore_xoila_get_arr_traitline_stamps( group ); };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

#endif  // BCORE_X_GROUP_H
