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

#ifndef BMATH_VF2_H
#define BMATH_VF2_H

/** bmath_vfx expansion for f2_t
 *  See bmath_template_vfx_h.h for details.
 */

#include "bcore_std.h"
#include "bmath_hwflags.h"
#include "bmath_spect_algebraic.h"
#include "bmath_pmt.h"

/**********************************************************************************************************************/
// dynamic size vector of f2_t

BCORE_DECLARE_OBJECT( bmath_vf2_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( f2_t, );
};

#define BMATH_TEMPLATE_FX_PREC 2
#include "bmath_template_vfx_h.h"

/**********************************************************************************************************************/

vd_t bmath_vf2_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_VF2_H
