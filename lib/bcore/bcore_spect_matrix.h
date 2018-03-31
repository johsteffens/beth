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
 *  Basis for tables, images, (mathematical) matrices, etc.
 *  A matrix is given by an array with features cols, rows
 *  to specify matrix dimensions.
 */



#ifndef BCORE_SPECT_MATRIX_H
#define BCORE_SPECT_MATRIX_H

#include "bcore_spect_array.h"

/**********************************************************************************************************************/

typedef struct bcore_matrix_s
{
    bcore_spect_header_s header;
    const bcore_array_s* spect_array;
    sz_t fixed_cols; // feature
    sz_t fixed_rows; // feature
    sz_t * cols;     // feature
    sz_t * rows;     // feature
} bcore_matrix_s;

const bcore_matrix_s* bcore_matrix_s_get_typed( tp_t type );

static inline const bcore_matrix_s* bcore_matrix_s_get_aware( vc_t obj )
{
    return bcore_matrix_s_get_typed( *( const aware_t* )obj );
}


/**********************************************************************************************************************/

vd_t bcore_spect_array_signal_handler( const bcore_signal_s* o );

#endif

