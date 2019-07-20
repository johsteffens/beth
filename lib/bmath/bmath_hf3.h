/** Copyright 2019 Johannes Bernhard Steffens
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

#ifndef BMATH_HF3_H
#define BMATH_HF3_H

/**********************************************************************************************************************/

/** Holor of f3_t
 *
 *  A holor represents the canonic extension of a matrix (or generalization of a tensor) as inspired by
 *  Moon & Spencer's Theory of Holors.
 *
 *  The term holor in this scope is represented as a multidimensional array, where jagged arrays are explicitly excluded.
 *  Meaning that a (n+1)-holor is an array of n-holors of the same data-layout but independent values.
 *
 *  The number of dimensions is called the order of a holor.
 *
 *  A n-holor is understood as holor or order n (meaning n separate dimensions).
 *  The number of values a holor holds is equal to the product of its dimensions.
 *
 *  bmath_hf3_t is organized as array of dimensions and array of associated data.
 *  Dimension-values are arranged from lowest to highest level.
 *
 *  Examples:
 *    A 0-holor represents a scalar.
 *    A 1-holor represents a vector; vectors-size == d_data[ 0 ];
 *    A 2-holor represents a matrix: cols == d_data[ 0 ]; rows == d_data[ 1 ];
 *    A n-holor represents an array of k (n-1)-holors with k == d_data[ n - 1 ].
 *
 *  The value-data layout is such that any n-holor can be converted to a mf3-matrix referencing the
 *  value data by grouping the dimensions into cols and rows given an k with 0 <= k <= n, such that
 *    cols == d_data[ 0 ] * .... * d_data[ k - 1 ] and
 *    rows == d_data[ k ] * .... * d_data[ n - 1 ] .
 *    The empty product resolves to 1.
 *
 */

/**********************************************************************************************************************/

#include "bcore_std.h"
#include "bmath_planted.h"
#include "bmath_hwflags.h"
#include "bmath_feature.h"
#include "bmath_mf3.h"

/**********************************************************************************************************************/

PLANT_GROUP( bmath_hf3, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp : = aware bcore_inst
{
    sz_t [] d; // dimension data
    f3_t [] v; // value data
};

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**********************************************************************************************************************/

/// turns a weak d-array into a strone one (no effect if it is strong)
void bmath_hf3_s_d_make_strong( bmath_hf3_s* o );

/// turns a weak v-array into a strone one (no effect if it is strong)
void bmath_hf3_s_v_make_strong( bmath_hf3_s* o );

/// sets number of values (initialized to zero)
void bmath_hf3_s_set_v_size( bmath_hf3_s* o, sz_t size );

/// sets number of dimensions (initialized to zero)
void bmath_hf3_s_set_d_size( bmath_hf3_s* o, sz_t size );

/// clears number of values
void bmath_hf3_s_clear_v_data( bmath_hf3_s* o );

/// clears number of dimensions (makes holor of scalar type)
void bmath_hf3_s_clear_d_data( bmath_hf3_s* o );

/// changes number of values preserving content (pads zero)
void bmath_hf3_s_resize_v_size( bmath_hf3_s* o, sz_t size );

/// changes number of dimensions preserving content (pads zero)
void bmath_hf3_s_resize_d_size( bmath_hf3_s* o, sz_t size );

/// sets number of values (initialized to zero)
void bmath_hf3_s_set_v_size( bmath_hf3_s* o, sz_t size );

/// fits number of values to given dimensions (initialized to zero)
void bmath_hf3_s_fit_v_size( bmath_hf3_s* o );

/// sets d_data, d_size to array (does not change v_size or v_data)
void bmath_hf3_s_set_d_data( bmath_hf3_s* o, const sz_t* d_data, sz_t d_size );

/// sets v_data, v_size to array (does not change d_size or d_data)
void bmath_hf3_s_set_v_data( bmath_hf3_s* o, const f3_t* v_data, sz_t v_size );

/// copies d_data from src (does not change v_size or v_data)
void bmath_hf3_s_copy_d_data( bmath_hf3_s* o, const bmath_hf3_s* src );

/// copies v_data from src (does not change d_size or d_data)
void bmath_hf3_s_copy_v_data( bmath_hf3_s* o, const bmath_hf3_s* src );

/// sets size of entire holor including v_data; (v_data initialized to zero)
void bmath_hf3_s_set_size( bmath_hf3_s* o, const sz_t* d_data, sz_t d_size );

/// sets d_data via variadic arguments
void bmath_hf3_s_set_d_data_nv( bmath_hf3_s* o, sz_t d_size, va_list sz_t_args );
void bmath_hf3_s_set_d_data_na( bmath_hf3_s* o, sz_t d_size, ... );

/// sets size via variadic arguments
void bmath_hf3_s_set_size_nv( bmath_hf3_s* o, sz_t d_size, va_list sz_t_args );
void bmath_hf3_s_set_size_na( bmath_hf3_s* o, sz_t d_size, ... );

/// returns product of all dimensions
sz_t bmath_hf3_s_d_product( const bmath_hf3_s* o );

/**********************************************************************************************************************/
/// holor specific operations

static inline sz_t bmath_hf3_s_get_order( const bmath_hf3_s* o ) { return o->d_size; }

/** Canonic increment of order by appending one dimension 'dim' :
 *  If the holder holds data, the data is duplicated 'dim' times.
 */
void bmath_hf3_s_inc_order( bmath_hf3_s* o, sz_t dim );

/** Canonic data append of a sub-holor */
void bmath_hf3_s_push( bmath_hf3_s* o, const bmath_hf3_s* src );

void bmath_hf3_s_to_sink( const bmath_hf3_s* o, bcore_sink* sink );

/**********************************************************************************************************************/
/// elementwise operations

/// o = {0}
void bmath_hf3_s_zro( const bmath_hf3_s* o );

/// o -> r
void bmath_hf3_s_cpy( const bmath_hf3_s* o, bmath_hf3_s* r );

/// tanh(o) -> r
void bmath_hf3_s_tanh( const bmath_hf3_s* o, bmath_hf3_s* r );

/// o + m -> r
void bmath_hf3_s_add( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r );

/// o - m -> r
void bmath_hf3_s_sub( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r );

/// o <*> m -> r (hadamard product)
void bmath_hf3_s_hmul( const bmath_hf3_s* o, const bmath_hf3_s* m, bmath_hf3_s* r );

/**********************************************************************************************************************/
/** holor * holor multiplication
 *  Separator k refers to holor left of k.
 *  The corresponding separators for other holors are fully determined.
 */

/// o * m -> r
void bmath_hf3_s_kmul(         const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* m, bmath_hf3_s* r );

/// o * m^T -> r
void bmath_hf3_s_kmul_htp(     const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* m, bmath_hf3_s* r );

/// o^T * m -> r
void bmath_hf3_s_htp_kmul(     const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* m, bmath_hf3_s* r );

/// ( o * b ) + c -> r
void bmath_hf3_s_kmul_add(     const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r );

/// ( o * b^T ) + c -> r
void bmath_hf3_s_kmul_htp_add( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r );

/// ( o^T * b ) + c -> r
void bmath_hf3_s_htp_kmul_add( const bmath_hf3_s* o, sz_t k, const bmath_hf3_s* b, const bmath_hf3_s* c, bmath_hf3_s* r );

/** composite multiply-add function. Satisfies functionality of BLAS:DGEMM.
 *  op(a) * op(b) * c + d * e -> r
 *  op(x) = htpx ? x^T : x;
 *  c, e are scalar.
 *  d can be NULL
 */
void bmath_hf3_s_kmul_add_cps( bl_t htpa, const bmath_hf3_s* a, sz_t k, bl_t htpb, const bmath_hf3_s* b, f3_t c, const bmath_hf3_s* d, f3_t e, bmath_hf3_s* r );

/**********************************************************************************************************************/

vd_t bmath_hf3_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BMATH_HF3_H



