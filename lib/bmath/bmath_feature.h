/** Copyright 2018 Johannes Bernhard Steffens
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

#ifndef BMATH_FEATURE_H
#define BMATH_FEATURE_H

#include "bcore_feature.h"

typedef u2_t (*bmath_fp_u2_argb_from_f3 )( vd_t o, f3_t v ); // converts f3 value to argb pixel value

/**********************************************************************************************************************/

vd_t bmath_feature_signal_handler( const bcore_signal_s* o );

#endif // BMATH_FEATURE_H

