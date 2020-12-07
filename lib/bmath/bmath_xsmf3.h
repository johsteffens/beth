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

#ifndef BMATH_XSMF3_H
#define BMATH_XSMF3_H

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath.xo.h"
#include "bmath_mf2.h"
#include "bmath_mf3.h"

/**********************************************************************************************************************/

XOILA_DEFINE_GROUP( bmath_xsmf3, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    stamp bmath_xsmf3_s = aware bcore_inst
    {
        sz_t slos;   // values per xon
        sz_t xons;   // xons  per row
        sz_t rows;   // number of rows
        sz_t i_stride; // stride of splicing

        sz_t [] i; // index data
        f3_t [] v; // value data

        func bcore_fp . copy_typed;
    };

    // function pointer names
    name bmath_fp_xsmf3_s_mul;
    name bmath_fp_xsmf3_s_mul_htp;
    name bmath_fp_xsmf3_s_htp_mul;
    name bmath_fp_xsmf3_s_htp_mul_htp;

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_xsmfx_h.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_xsmfx_mul_h.h"

/**********************************************************************************************************************/

vd_t bmath_xsmf3_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_XSMF3_H
