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

#ifndef BMATH_VCF2_H
#define BMATH_VCF2_H

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_cf2.h"
#include "bmath_vf2.h"

/**********************************************************************************************************************/
// dynamic size vector of bmath_cf2_s

BCORE_DECLARE_OBJECT( bmath_vcf2_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_cf2_s, );
};

#define BMATH_TEMPLATE_FX_PREC 2
#include "bmath_template_vcfx_h.h"

/**********************************************************************************************************************/

vd_t bmath_vcf2_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_VCF2_H
