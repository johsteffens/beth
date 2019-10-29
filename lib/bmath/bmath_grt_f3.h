/** Author and Copyright 2018 Johannes Bernhard Steffens
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

#ifndef BMATH_GRT_F3_H
#define BMATH_GRT_F3_H

#include "bcore_std.h"
#include "bmath_hwflags.h"
#include "bmath_leaf.h"

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_grt_f3_s ) { f3_t c; f3_t s; };

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_grt_fx_h.h"

//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------

vd_t bmath_grt_f3_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_GRT_F3_H
