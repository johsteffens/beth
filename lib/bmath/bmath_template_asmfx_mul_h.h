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

/**********************************************************************************************************************/

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

void BCATU(bmath_asmf3_s,mul)(         const bmath_asmf3_s* o, const bmath_asmf3_s* m, bmath_asmf3_s* r ); // o * m -> r
void BCATU(bmath_asmf3_s,mul_htp)(     const bmath_asmf3_s* o, const bmath_asmf3_s* m, bmath_asmf3_s* r ); // o * m^T -> r
void BCATU(bmath_asmf3_s,htp_mul)(     const bmath_asmf3_s* o, const bmath_asmf3_s* m, bmath_asmf3_s* r ); // o^T * m -> r
void BCATU(bmath_asmf3_s,htp_mul_htp)( const bmath_asmf3_s* o, const bmath_asmf3_s* m, bmath_asmf3_s* r ); // o^T * m^T -> r

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

