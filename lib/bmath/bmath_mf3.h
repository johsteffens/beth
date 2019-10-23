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

#ifndef BMATH_MF3_H
#define BMATH_MF3_H

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_hwflags.h"
#include "bmath_feature.h"
#include "bmath_spect_algebraic.h"
#include "bmath_vf3.h"
#include "bmath_arr_vf3.h"
#include "bmath_simd.h"
#include "bmath_grt.h"

/**********************************************************************************************************************/
/// Matrix from templates for fx == f3

BCORE_DECLARE_OBJECT( bmath_mf3_s )
{
    aware_t _;
    uz_t rows;
    uz_t cols;
    uz_t stride;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( f3_t, );
};

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_mfx_mul_h.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_mfx_qrd_h.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_mfx_evd_h.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_mfx_svd_h.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_mfx_h.h"

/**********************************************************************************************************************/
/// array of mf3_s

BCORE_DECLARE_ARRAY_DYN_SOLID_STATIC( bmath_mf3_s, bmath_arr_mf3_s );

/**********************************************************************************************************************/

vd_t bmath_mf3_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_MF3_H
