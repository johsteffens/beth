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

#ifndef BHPT_H
#define BHPT_H

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bhpt.xo.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bhpt      ,          x_inst,       "bhpt.x"         )
XOILA_DEFINE_GROUP( bhpt_frame,          x_inst,       "bhpt_frame.x"   )
XOILA_DEFINE_GROUP( bhpt_adaptor_stamp,  bhpt_adaptor, "bhpt_adaptor.x" )
XOILA_DEFINE_GROUP( bhpt_tutor_language, x_inst,       "bhpt_tutor_language.x" )
XOILA_DEFINE_GROUP( bhpt_sampler,        x_inst,       "bhpt_tutor_sampler.x" )

/**********************************************************************************************************************/

#endif // BHPT_H
