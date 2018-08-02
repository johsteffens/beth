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

typedef struct bcore_matrix bcore_matrix;
BCORE_DECLARE_SPECT( bcore_matrix_s )
{
    bcore_spect_header_s header;
    const bcore_array_s* spect_array;
    uz_t rows_fix;   // feature for fixed rows (0 otherwise)
    uz_t cols_fix;   // feature for fixed cols (0 otherwise)
    uz_t stride_fix; // feature for fixed row-stride

    uz_t rows_off;   // feature for dynamic row offset
    uz_t cols_off;   // feature for dynamic col offset
    uz_t stride_off; // feature for dynamic row-stride
};

static inline
uz_t bcore_matrix_default_get_rows( const bcore_matrix_s* p, const bcore_matrix* o )
{
    return p->rows_fix > 0 ? p->rows_fix : *( uz_t* )BCORE_OFFSET( o, p->rows_off );
}

static inline
uz_t bcore_matrix_default_get_cols( const bcore_matrix_s* p, const bcore_matrix* o )
{
    return p->cols_fix > 0 ? p->cols_fix : *( uz_t* )BCORE_OFFSET( o, p->cols_off );
}

static inline
uz_t bcore_matrix_default_get_stride( const bcore_matrix_s* p, const bcore_matrix* o )
{
    return p->stride_fix > 0 ? p->stride_fix : *( uz_t* )BCORE_OFFSET( o, p->stride_off );
}

void bcore_matrix_default_set_size( const bcore_matrix_s* p, bcore_matrix* o, uz_t rows, uz_t cols );

static inline
uz_t bcore_matrix_default_get_row_index( const bcore_matrix_s* p, const bcore_matrix* o, uz_t i )
{
    assert( i < bcore_matrix_default_get_rows( p, o ) );
    return i * bcore_matrix_default_get_stride( p, o );
}

static inline
uz_t bcore_matrix_default_get_index( const bcore_matrix_s* p, const bcore_matrix* o, uz_t i, uz_t j )
{
    assert( j < bcore_matrix_default_get_cols( p, o ) );
    return bcore_matrix_default_get_row_index( p, o, i ) + j;
}

static inline
sr_s bcore_matrix_default_get_cell( const bcore_matrix_s* p, const bcore_matrix* o, uz_t i, uz_t j )
{
    return bcore_array_p_get( p->spect_array, (bcore_array*)o, bcore_matrix_default_get_index( p, o, i, j ) );
}

static inline
void bcore_matrix_default_set_cell( const bcore_matrix_s* p, bcore_matrix* o, uz_t i, uz_t j, sr_s src )
{
    bcore_array_p_set( p->spect_array, (bcore_array*)o, bcore_matrix_default_get_index( p, o, i, j ), src );
}

BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_matrix, get_rows,      uz_t )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_matrix, get_cols,      uz_t )
BCORE_FUNC_SPECT_CONST1_RET1_ARG0_MAP0( bcore_matrix, get_stride,    uz_t )
BCORE_FUNC_SPECT_CONST0_RET0_ARG2_MAP0( bcore_matrix, set_size,            uz_t, rows, uz_t, cols )
BCORE_FUNC_SPECT_CONST1_RET1_ARG1_MAP0( bcore_matrix, get_row_index, uz_t, uz_t, i )
BCORE_FUNC_SPECT_CONST1_RET1_ARG2_MAP0( bcore_matrix, get_index,     uz_t, uz_t, i, uz_t, j )
BCORE_FUNC_SPECT_CONST1_RET1_ARG2_MAP0( bcore_matrix, get_cell,      sr_s, uz_t, i, uz_t, j )
BCORE_FUNC_SPECT_CONST0_RET0_ARG3_MAP0( bcore_matrix, set_cell,            uz_t, i, uz_t, j, sr_s, src )

/**********************************************************************************************************************/

vd_t bcore_spect_matrix_signal_handler( const bcore_signal_s* o );

#endif

