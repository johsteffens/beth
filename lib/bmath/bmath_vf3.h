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

#ifndef BMATH_VF3_H
#define BMATH_VF3_H

/** bmath_vfx expansion for f3_t
 *  See bmath_template_vfx_h.h for details.
 */

#include "bcore_std.h"
#include "bmath_hwflags.h"
#include "bmath_spect_algebraic.h"
#include "bmath_pmt.h"

/**********************************************************************************************************************/
// dynamic size vector of f3_t

BCORE_DECLARE_OBJECT( bmath_vf3_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, );
};

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_vfx_h.h"

void bmath_vf3_s_copy_typed( bmath_vf3_s* o, tp_t type, vc_t src );
static inline void bmath_vf3_s_copy_t( bmath_vf3_s* o, tp_t type, vc_t src ) { bmath_vf3_s_copy_typed( o, type, src ); }
static inline void bmath_vf3_s_copy_a( bmath_vf3_s* o, vc_t src ) { if( src ) bmath_vf3_s_copy_t( o, *(aware_t*)src, src ); else bmath_vf3_s_clear( o ); }

/**********************************************************************************************************************/

vd_t bmath_vf3_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_VF3_H
