/** Author and Copyright 2022 Johannes Bernhard Steffens
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

/** Linear estimator (x-wrapper of bmath_estimator.h)
 *  The estimator assumes the existence of an affine transformation A: X -> Y
 *  behind a data-set given by (weighted) samples x, y.
 *  After digesting all data points, it can compute an estimate of the transformation.
 *
 *  The method employed is generalized least square analysis.
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp bmath_estimator_s =
{
    f3_t eps = 1E-8; // epsilon (for pseudoinversion)
    bmath_mf3_s xx;  // accumulated xx outer product
    bmath_mf3_s yx;  // accumulated yx outer product

    /// clears accumulators
    func (o clear(m @* o ));

    /// digests a weighted data sample
    func (o digest( m@* o, f3_t weight, bmath_vf3_s* x, bmath_vf3_s* y ));

    /// re-weights by multiplying weight to all digested weights
    func (o reweight( m@* o, f3_t weight ));

    /// computes final affine matrix (use with bmath_mf3_s_mul_av1)
    func (mat get_matrix( @* o, m bmath_mf3_s* mat ));
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

