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

#ifndef BHVM_VOP_H
#define BHVM_VOP_H

/**********************************************************************************************************************/

/** Collection of V-Machine-Operators
 */

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_vop.xo.h"
#include "bhvm_hop.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bhvm_vop, bcore_inst, "bhvm_vop.x" )

void bhvm_vop_a_to_sink( bhvm_vop* o, bcore_sink* sink );

/**********************************************************************************************************************/

vd_t bhvm_vop_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_VOP_H
