/** Copyright 2019 Johannes Bernhard Steffens
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

#ifndef BMATH_VF3_H
#define BMATH_VF3_H

/** bmath_vfx expansion for f3_t
 *  See bmath_vfx_h.h for details.
 */

#include "bcore_std.h"

/**********************************************************************************************************************/
// dynamic size vector of f3_t

BCORE_DECLARE_OBJECT( bmath_vf3_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, );
};

#define BFPREC_LEVEL 3
#include "bmath_vfx_h.h"

/**********************************************************************************************************************/

vd_t bmath_vf3_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_VF3_H
