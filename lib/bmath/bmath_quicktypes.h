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

// Quickly obtainable types.

#ifndef BMATH_QUICKTYPES_H
#define BMATH_QUICKTYPES_H

#include "bcore_name_manager.h"
#include "bcore_quicktypes.h"

/// This function computes and formats defines below to stdout
void bmath_quicktypes_to_stdout( tp_t (*hash)( sc_t name ) );
void bmath_quicktypes_create_header( sc_t folder );

/** List of predefined types for quick access.
 *  Created via bclos_quicktypes_to_stdout( NULL );
 */

#define TYPEOF_bmath_cf3_s    2506167476
#define TYPEOF_bmath_group    1421936371
#define TYPEOF_bmath_group_s  1219600405
#define TYPEOF_bmath_matrix   3796445417
#define TYPEOF_bmath_matrix_s 1909792099
#define TYPEOF_bmath_mf3_s    2915726298
#define TYPEOF_bmath_ring     2462086090
#define TYPEOF_bmath_ring_s   1513035188
#define TYPEOF_bmath_vcf3_s   3723112310
#define TYPEOF_bmath_vector   4070600319
#define TYPEOF_bmath_vector_s 1200091625
#define TYPEOF_bmath_vf3_s    3565809519

#endif // BMATH_QUICKTYPES_H
