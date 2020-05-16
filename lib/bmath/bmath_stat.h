/** Author and Copyright 2020 Johannes Bernhard Steffens
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

#ifndef BMATH_STAT_H
#define BMATH_STAT_H

/**********************************************************************************************************************/
/// Functions for advanced statistics

#include "bcore_signal.h"
#include "bmath_leaf.h"
#include "bmath_hwflags.h"
#include "bmath_quicktypes.h"

/**********************************************************************************************************************/

/** For functions below
 *  '*scuccess' holds 'true' on successful completion, 'false' otherwise.
 *  On failure, the return value should be deemed invalid.
 */

/// Regularized gamma function.
f3_t bmath_stat_regularized_gamma( f3_t s, f3_t x, bl_t* success );

/// Chi-squared cumulative distribution function.
f3_t bmath_stat_chisqu_cdf( sz_t k, f3_t x, bl_t* success );

/** Inverse chi-squared cumulative distribution function.
 *  Computes the chi-square limit for a given probability 'p'.
 */
f3_t bmath_stat_chisqu_cdf_inv( sz_t k, f3_t p, bl_t* success );

/**********************************************************************************************************************/

vd_t bmath_stat_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_STAT_H

