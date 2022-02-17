/** Author and Copyright 2021 Johannes Bernhard Steffens
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

/** bmath inexpandable group.
 *  This group does not generate effective xoila-code but is otherwise accessible within the xoila framework.
 */

#ifndef BMATH_X_ROOT_INEXPANDABLE_H
#define BMATH_X_ROOT_INEXPANDABLE_H

#include "bmath_std.h"
#include "bmath.xo.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bmath_x_root
XOILA_DEFINE_GROUP( bmath_x_root, x_inst )

//----------------------------------------------------------------------------------------------------------------------

#ifdef XOILA_SECTION

set inexpandable;

identifier sin;
identifier cos;
identifier tan;
identifier tanh;
identifier pow;
identifier lrint;
identifier atan2;
identifier frexp;

identifier f3_pi;
identifier f3_pi_2; // PI / 2
identifier f3_tau; // 2 * PI
identifier f3_is_nan; // nan compares unequal to itself
identifier f3_exp;
identifier f3_log;
identifier f3_log10;
identifier f3_pow;
identifier f3_erf;

identifier f2_pi;
identifier f2_pi_2; // PI / 2
identifier f2_tau; // 2 * PI
identifier f2_is_nan; // nan compares unequal to itself
identifier f2_exp;
identifier f2_log;
identifier f2_log10;
identifier f2_pow;
identifier f2_erf;

//----------------------------------------------------------------------------------------------------------------------

stamp bmath_arr_vf2_s = x_array
{
    bmath_vf2_s [];
};

stamp bmath_arr_vf3_s = x_array
{
    bmath_vf3_s [];
};

//----------------------------------------------------------------------------------------------------------------------

embed "bmath_x_vf_inexpandable.x";  // vector
embed "bmath_x_mf_inexpandable.x";  // matrix
embed "bmath_x_cf_inexpandable.x";  // complex number
embed "bmath_x_vcf_inexpandable.x"; // complex vector
embed "bmath_x_fourier_inexpandable.x"; // fourier transformation
embed "bmath_x_estimator_inexpandable.x"; // linear estimator

#endif // XOILA_SECTION

#endif // TYPEOF_bmath_x_root

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

#endif  // BMATH_X_ROOT_INEXPANDABLE_H
