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

#ifndef BMATH_ESTIMATOR_H
#define BMATH_ESTIMATOR_H

/** Linear estimator
 *  The estimator assumes an affine transformation A: X -> Y.
 *  behind a dataset given by (weightd) samples x, y.
 *  After digesting all data points, it can compute an estimate
 *  of the transformation.
 *
 *  The method employed is generalized least square analysis.
 */

#include "bcore_std.h"
#include "bmath_hwflags.h"
#include "bmath_mf3.h"

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_estimator_s )
{
    aware_t _;
    f3_t eps;       // epsilon (for pseudoinversion)
    bmath_mf3_s xx; // accumulated xx outer product
    bmath_mf3_s yx; // accumulated yx outer product
};

/// clears accumulators
void bmath_estimator_s_clear( bmath_estimator_s* o );

/// digests a weighted data sample
void bmath_estimator_s_digest( bmath_estimator_s* o, f3_t weight, const bmath_vf3_s* x, const bmath_vf3_s* y );

/// reweights by multiplying weight to all digested weights
void bmath_estimator_s_reweight( bmath_estimator_s* o, f3_t weight );

/// computes final affine matrix (use with bmath_mf3_s_mul_av1)
void bmath_estimator_s_get_matrix( const bmath_estimator_s* o, bmath_mf3_s* mat );

/**********************************************************************************************************************/

vd_t bmath_estimator_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_ESTIMATOR_H
