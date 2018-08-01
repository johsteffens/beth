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

#ifndef BMATH_MATRIX_EVAL_H
#define BMATH_MATRIX_EVAL_H

/**********************************************************************************************************************/

/** Collection of evaluation routines for bmath_matrix
 */

/**********************************************************************************************************************/

#include "bmath_vector.h"
#include "bmath_matrix.h"

/**********************************************************************************************************************/

typedef bl_t (*bmath_fp_svd )( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v ); // svd function pointer
typedef void (*bmath_fp_ubd )( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v ); // ubd function pointer
typedef void (*bmath_fp_lbd )( bmath_mf3_s* u, bmath_mf3_s* a, bmath_mf3_s* v ); // ubd function pointer
typedef void (*bmath_fp_qrd )( bmath_mf3_s* u, bmath_mf3_s* a                 ); // qrd function pointer
typedef void (*bmath_fp_lqd )(                 bmath_mf3_s* a, bmath_mf3_s* v ); // lqd function pointer

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_matrix_eval_s )
{
    aware_t _;
    sz_t rows;
    sz_t cols;

    u2_t seed;       // random seed
    f3_t density;    // random matrix density
    bl_t full;       // full vs thin decomposition (where applicable)
    f3_t near_limit; // limit for near-assertions

    tp_t fp_type; // typeof( "<one of above f-pointers>" ); e.g.   typeof( "bmath_fp_svd" )
    fp_t fp;      // pointer to evaluation function
};

/// runs evaluation and logs results if desired (log can be NULL)
void bmath_matrix_eval_s_run(              const bmath_matrix_eval_s* o,                        st_s* log );
void bmath_matrix_eval_s_run_fp(           const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp, st_s* log ); // using fp instead of o->fp
void bmath_matrix_eval_s_run_to_stdout(    const bmath_matrix_eval_s* o                         );
void bmath_matrix_eval_s_run_fp_to_stdout( const bmath_matrix_eval_s* o, tp_t fp_type, fp_t fp  );

/**********************************************************************************************************************/

BCORE_DECLARE_OBJECT( bmath_arr_matrix_eval_s )
{
    aware_t _;
    BCORE_ARRAY_DYN_SOLID_STATIC_S( bmath_matrix_eval_s, );
};

static inline void bmath_arr_matrix_eval_s_set_size( bmath_arr_matrix_eval_s* o, sz_t size )
{
    bcore_array_a_set_size( ( bcore_array* )o, size );
}

static inline void bmath_arr_matrix_eval_s_push( bmath_arr_matrix_eval_s* o, const bmath_matrix_eval_s* v )
{
    bcore_array_a_push( ( bcore_array* )o, sr_twc(TYPEOF_bmath_matrix_eval_s, v ) );
}

void bmath_arr_matrix_eval_s_run(              const bmath_arr_matrix_eval_s* o,                        st_s* log );
void bmath_arr_matrix_eval_s_run_fp(           const bmath_arr_matrix_eval_s* o, tp_t fp_type, fp_t fp, st_s* log ); // using fp instead of o->fp
void bmath_arr_matrix_eval_s_run_to_stdout(    const bmath_arr_matrix_eval_s* o                         );
void bmath_arr_matrix_eval_s_run_fp_to_stdout( const bmath_arr_matrix_eval_s* o, tp_t fp_type, fp_t fp  );

/**********************************************************************************************************************/

vd_t bmath_matrix_eval_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BMATH_MATRIX_EVAL_H
