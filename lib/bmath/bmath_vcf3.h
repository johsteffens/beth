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

#ifndef BMATH_VCF3_H
#define BMATH_VCF3_H

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_cf3.h"
#include "bmath_vf3.h"

/**********************************************************************************************************************/
// dynamic size vector of bmath_cf3_s

BCORE_DECLARE_OBJECT( bmath_vcf3_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_cf3_s, );
};

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_vcfx_h.h"

void bmath_vcf3_s_copy_typed( bmath_vcf3_s* o, tp_t type, vc_t src );
static inline void bmath_vcf3_s_copy_t( bmath_vcf3_s* o, tp_t type, vc_t src ) { bmath_vcf3_s_copy_typed( o, type, src ); }
static inline void bmath_vcf3_s_copy_a( bmath_vcf3_s* o, vc_t src ) { if( src ) bmath_vcf3_s_copy_t( o, *(aware_t*)src, src ); else bmath_vcf3_s_clear( o ); }

/**********************************************************************************************************************/

vd_t bmath_vcf3_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_VCF3_H
