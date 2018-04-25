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

#ifndef BMATH_TYPES_H
#define BMATH_TYPES_H

/** Frequently used types */
/*
/// 'virtual' structures representing undefined (interface-) types of a given category
typedef struct bmath_scl_i bmath_scl_i;
typedef struct bmath_vec_i bmath_vec_i;
typedef struct bmath_mat_i bmath_mat_i;

/// pointers to interface types used for type checks in c
typedef const bmath_scl_i* const bmath_ring*; // const scalar type
typedef       bmath_scl_i* bmath_ring*; //       scalar type
typedef const bmath_vec_i* const bmath_vector*; // const vector type
typedef       bmath_vec_i* bmath_vector*; //       vector type
typedef const bmath_mat_i* const bmath_matrix*; // const matrix type
typedef       bmath_mat_i* bmath_matrix*; //       matrix type
*/
/*
typedef vc_t const bmath_ring*; // const scalar type
typedef vd_t bmath_ring*; //       scalar type
typedef vc_t const bmath_vector*; // const vector type
typedef vd_t bmath_vector*; //       vector type
typedef vc_t const bmath_matrix*; // const matrix type
typedef vd_t bmath_matrix*; //       matrix type
*/

/**********************************************************************************************************************/

vd_t bmath_types_signal_handler( const bcore_signal_s* o );

#endif  // BMATH_TYPES_H
