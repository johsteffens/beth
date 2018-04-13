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

/** Matrix Perspective.
 *  Basis for tables, images, (mathematical) matrices, layouts, etc.
 *  A matrix is given by an array with features cols, rows
 *  to specify matrix dimensions.
 */

#ifndef BCORE_SPECT_MATRIX_H
#define BCORE_SPECT_MATRIX_H

#include "bcore_spect_array.h"
#include "bcore_quicktypes.h"

/**********************************************************************************************************************/

typedef struct bcore_matrix_s
{
    bcore_spect_header_s header;
    const bcore_array_s* spect_array;
    sz_t rows_fix;   // feature for fixed rows (0 otherwise)
    sz_t cols_fix;   // feature for fixed cols (0 otherwise)
    sz_t stride_fix; // feature for fixed row-stride

    sz_t rows_off;   // feature for dynamic row offset
    sz_t cols_off;   // feature for dynamic col offset
    sz_t stride_off; // feature for dynamic row-stride
} bcore_matrix_s;

BCORE_DEFINE_INLINE_SPECT_GET_TYPED_CACHED( bcore_matrix_s )
BCORE_DEFINE_INLINE_SPECT_GET_AWARE( bcore_matrix_s )

static inline
sz_t bcore_matrix_spect_get_rows( const bcore_matrix_s* p, vc_t o )
{
    return p->rows_fix > 0 ? p->rows_fix : *( sz_t* )BCORE_OFFSET( o, p->rows_off );
}

static inline
sz_t bcore_matrix_spect_get_cols( const bcore_matrix_s* p, vc_t o )
{
    return p->cols_fix > 0 ? p->cols_fix : *( sz_t* )BCORE_OFFSET( o, p->cols_off );
}

static inline
sz_t bcore_matrix_spect_get_stride( const bcore_matrix_s* p, vc_t o )
{
    return p->stride_fix > 0 ? p->stride_fix : *( sz_t* )BCORE_OFFSET( o, p->stride_off );
}

void bcore_matrix_spect_set_size( const bcore_matrix_s* p, vd_t o, sz_t rows, sz_t cols );

static inline
sz_t bcore_matrix_spect_get_row_index( const bcore_matrix_s* p, vc_t o, sz_t i )
{
    assert( i < bcore_matrix_spect_get_rows( p, o ) );
    return i * bcore_matrix_spect_get_stride( p, o );
}

static inline
sz_t bcore_matrix_spect_get_index( const bcore_matrix_s* p, vc_t o, sz_t i, sz_t j )
{
    assert( j < bcore_matrix_spect_get_cols( p, o ) );
    return bcore_matrix_spect_get_row_index( p, o, i ) + j;
}

static inline
sr_s bcore_matrix_spect_get_cell( const bcore_matrix_s* p, vc_t o, sz_t i, sz_t j )
{
    return bcore_array_spect_get( p->spect_array, o, bcore_matrix_spect_get_index( p, o, i, j ) );
}

static inline
void bcore_matrix_spect_set_cell( const bcore_matrix_s* p, vd_t o, sz_t i, sz_t j, sr_s src )
{
    bcore_array_spect_set( p->spect_array, o, bcore_matrix_spect_get_index( p, o, i, j ), src );
}

sz_t bcore_matrix_typed_get_rows(      tp_t t, vc_t o );
sz_t bcore_matrix_typed_get_cols(      tp_t t, vc_t o );
sz_t bcore_matrix_typed_get_stride(    tp_t t, vc_t o );
void bcore_matrix_typed_set_size(      tp_t t, vd_t o, sz_t rows, sz_t cols );
sz_t bcore_matrix_typed_get_row_index( tp_t t, vc_t o, sz_t i );
sz_t bcore_matrix_typed_get_index(     tp_t t, vc_t o, sz_t i, sz_t j );
sr_s bcore_matrix_typed_get_cell(      tp_t t, vc_t o, sz_t i, sz_t j );
void bcore_matrix_typed_set_cell(      tp_t t, vd_t o, sz_t i, sz_t j, sr_s src );

sz_t bcore_matrix_aware_get_rows(      vc_t o );
sz_t bcore_matrix_aware_get_cols(      vc_t o );
sz_t bcore_matrix_aware_get_stride(    vc_t o );
void bcore_matrix_aware_set_size(      vd_t o, sz_t rows, sz_t cols );
sz_t bcore_matrix_aware_get_row_index( vc_t o, sz_t i );
sz_t bcore_matrix_aware_get_index(     vc_t o, sz_t i, sz_t j );
sr_s bcore_matrix_aware_get_cell(      vc_t o, sz_t i, sz_t j );
void bcore_matrix_aware_set_cell(      vd_t o, sz_t i, sz_t j, sr_s src );

sz_t bcore_matrix_get_rows(      sr_s o );
sz_t bcore_matrix_get_cols(      sr_s o );
sz_t bcore_matrix_get_stride(    sr_s o );
void bcore_matrix_set_size(      sr_s o, sz_t rows, sz_t cols );
sz_t bcore_matrix_get_row_index( sr_s o, sz_t i );
sz_t bcore_matrix_get_index(     sr_s o, sz_t i, sz_t j );
sr_s bcore_matrix_get_cell(      sr_s o, sz_t i, sz_t j );
void bcore_matrix_set_cell(      sr_s o, sz_t i, sz_t j, sr_s src );

sz_t bcore_matrix_q_get_rows(      const sr_s* o );
sz_t bcore_matrix_q_get_cols(      const sr_s* o );
sz_t bcore_matrix_q_get_stride(    const sr_s* o );
void bcore_matrix_q_set_size(      const sr_s* o, sz_t rows, sz_t cols );
sz_t bcore_matrix_q_get_row_index( const sr_s* o, sz_t i );
sz_t bcore_matrix_q_get_index(     const sr_s* o, sz_t i, sz_t j );
sr_s bcore_matrix_q_get_cell(      const sr_s* o, sz_t i, sz_t j );
void bcore_matrix_q_set_cell(      const sr_s* o, sz_t i, sz_t j, sr_s src );

/**********************************************************************************************************************/

vd_t bcore_spect_array_signal_handler( const bcore_signal_s* o );

#endif

