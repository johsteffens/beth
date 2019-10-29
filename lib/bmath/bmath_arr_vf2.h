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

#ifndef BMATH_ARR_VF2_H
#define BMATH_ARR_VF2_H

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_vf2.h"

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_arr_vf2_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_vf2_s, );
};

#define BMATH_TEMPLATE_FX_PREC 2
#include "bmath_template_arr_vfx_h.h"

void bmath_arr_vf2_s_copy_typed( bmath_arr_vf2_s* o, tp_t type, vc_t src );
static inline void bmath_arr_vf2_s_copy_t( bmath_arr_vf2_s* o, tp_t type, vc_t src ) { bmath_arr_vf2_s_copy_typed( o, type, src ); }
static inline void bmath_arr_vf2_s_copy_a( bmath_arr_vf2_s* o, vc_t src ) { if( src ) bmath_arr_vf2_s_copy_t( o, *(aware_t*)src, src ); else bmath_arr_vf2_s_clear( o ); }

/**********************************************************************************************************************/

vd_t bmath_arr_vf2_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_ARR_VF2_H
