/** Author and Copyright 2026 Johannes Bernhard Steffens
 *  https://github.com/johsteffens/mocutsvd
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

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** MOCUT-SVD: Fast, stable and true-scalable Singular Value Decomposition (SVD).
 *
 *  Most relevant for the overall performance is the partitioning of unitary transformations, which I call
 *  Monoclinic Unitary Transformation (MOCUT).The name is inspired from crystallography, where the term monoclinic
 *  describes a special form of oblique crystal cell tiling. It resembles the transformation pattern used in this
 *  algorithm. It is useful in multiple paradigms for computational efficiency.
 *
 *  MOCUT-SVD ensures data-locality (for cache-efficiency) and outer independence (for parallelity)
 *  It uses OpenMP pragmas for parallel processing. Where OpenMP is not available or not wanted
 *  the function simply runs sequentially on the caller thread. (see https://en.wikipedia.org/wiki/OpenMP)
 *
 *  High Portability:
 *  This stand-alone implementation makes minimal assumptions about actual hardware, compiler and libraries.
 *  It expects the compiler to support the C11 standard. Support for OpenMP is only needed for thread-parallelity
 *  but not required.
 *
 *  Matrix ABI:
 *  A matrix representation follows strided row-major ordering:
 *  Rows are stored sequentially in contiguous memory. Row-ends are padded with unused data points to ensure constant stride
 *  between adjacent rows.
 *
 *  Recommended alignment: Matrix rows should be memory-aligned for optimal performance.
 *  Use aligned_alloc for memory allocation with alignment = 128 (0x80) (=16*sizeof(double)).
 *  Use stride value: stride = cols + ( ( cols % 16 ) > 0 ) ? 16 - ( cols % 16 ) : 0.
 *
 *  Recommended compiler flags (gcc)
 *    -O3             Speed optimization
 *    -march=native   Allows the compiler to use CPU-native components (e.g. SIMD)
 *    -fopenmp        Activates OpenMP (must also be presented to the linker)
 *
 */

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#include "mocutsvd.h"

#include <stddef.h>
#include <stdint.h>

#include <math.h>
#include <float.h>
#include <assert.h>

#ifndef __bool_true_false_are_defined
#include <stdbool.h> // stdbool.h is deprecated since C23
#endif

//----------------------------------------------------------------------------------------------------------------------

#pragma GCC diagnostic ignored "-Wunknown-pragmas"

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/** Constants affecting runtime efficiency. These are optimized for most common cache and core architectures.
 *  Revisit these values in future e.g. when the architectures have significantly changed.
 */

//----------------------------------------------------------------------------------------------------------------------

// Size of a temporary data buffer to improve computation accuracy.
#define HSR_VCB_SIZE 8 // must always be a multiple of 2

// Size of a temporary data buffer to improve L1 cache efficiency.
//#define HSR_L1_BUF_SIZE 32 // L1 cache friendly buffering
#define HSR_L1_BUF_SIZE 48 // L1 cache friendly buffering

/* Maximum size of a householder reflection.
 * This value also limits the maximum band-width in band-transformations.
 */
#define HSR_SIZE 65

/* For parallel computation, certain areas are partitioned and distributed across multiple threads.
 * This value controls the maximum size a partition.
 */
#define TPART_SIZE 48

/* Reflection partitioning.
 * Used in bi-diagonalization.
 */
//#define RPART_SIZE 42
#define RPART_SIZE 48

// Maximum off-diagonal band-size. Allowed range: 1 <= BAND_SIZE < HSR_SIZE
#define BAND_SIZE 32

/* For parallel computation, certain areas are partitioned and distributed across multiple threads.
 * This value controls the maximum size a partition in the SVD cycle.
 */
#define SVD_MIN_COL_BLOCK 32
#define SVD_MAX_COL_BLOCK 64

/* Rotation partitioning.
 * Used in in the SVD cycle.
 */
#define SVD_GR_BLOCK 32

/* Determines how much data is gathered before the parallel u, v transformation is invoked.
 * Larger intervals improve on parallelity at the cost of extra wasteful operations.
 */
#define SVD_MAX_GRHO_ROWS 512

//----------------------------------------------------------------------------------------------------------------------

/** Constants controlling the SVD convergence.
 *  A convergence failure should be rare enough to be
 *  deemed impossible on any (not too exotic) matrix.
 *  The values below have been carefully chosen.
 *  Do not change them unless you observe convergence failures.
 *
 *  If you observe one, try increasing one of the limits below.
 *  I (the author) would also be happy to be informed about
 *  your better setup. If possible, together with the offending
 *  matrix donated as a study-object.
 *  (Contact me by opening a githup issue)
 */

/** This value controls at which upper limit intermediate diagonal or
 *  sub-diagonal elements are deemed zero. It triggers a redress
 *  or repartition cycle. A higher limit increases the likelihood of
 *  such cycles preventing convergence failure but at the cost of
 *  speed and (possibly) accuracy.
 */
#define SVD_DEFECT_LIMIT f3_lim_eps

/** This value controls the maximum allowed convergence cycles
 *  until SVD decides that convergence failed.
 *  A higher limit may salvage the situation in the rare case that
 *  convergence is merely slow but not stalled.
 */
#define SVD_CYCLE_LIMIT_FACTOR 50

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Simple types, constants and functions

//----------------------------------------------------------------------------------------------------------------------

/// type names (according to beth-nomenclature)
typedef double    f3_t;
typedef int64_t   s3_t;
typedef ptrdiff_t sz_t; // signed
typedef size_t    uz_t; // unsigned
typedef bool      bl_t; // boolean

/// limits
static const f3_t f3_lim_min = DBL_MIN;     // minimum normalized representable positive number
static const f3_t f3_lim_eps = DBL_EPSILON; // minimum representable positive difference to 1.0
static const f3_t f3_lim_inf = INFINITY;    // floating point representation of infinity

/// Basic functionality related to leaf-types
static inline f3_t f3_sqr  ( f3_t v ) { return v * v; }
static inline f3_t f3_srt  ( f3_t v ) { return sqrt( v ); }
static inline f3_t f3_min  ( f3_t a, f3_t b ) { return a < b ? a : b; }
static inline f3_t f3_max  ( f3_t a, f3_t b ) { return a > b ? a : b; }
static inline f3_t f3_sig  ( f3_t v ) { return v  < 0 ? -1 : 1; }
static inline f3_t f3_abs  ( f3_t v ) { return v  < 0 ? -v : v; }
static inline f3_t f3_inv  ( f3_t v ) { return f3_abs( v ) >= f3_lim_min ? 1.0 / v : f3_lim_inf; }

static inline void f3_t_swap( f3_t* v1, f3_t* v2 ) { f3_t t = *v1; *v1 = *v2; *v2 = t;  }

static inline sz_t sz_min( sz_t a, sz_t b ) { return a < b ? a : b; }
static inline sz_t sz_max( sz_t a, sz_t b ) { return a > b ? a : b; }
static inline sz_t sz_abs( sz_t v ) { return v < 0 ? -v : v; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// MOCUT Functionality via Householder Reflections

//----------------------------------------------------------------------------------------------------------------------

/** AWT: Given vector a:
 *  Computes transformed vector a with z_size trailing zeros.
 *  Computes principal transformation vector w.
 *  w may occupy the same space as a.
 *  Stride specifies the distance between vector components.
 *
 *  Overflow-Safe, Underflow-Safe, Near-Zero-Safe
 */
static void hsr_create_awt( f3_t* a, f3_t* w, sz_t stride, sz_t z_size ) // (!) Do not use 'restrict' on a or w, because both can occupy the same space.
{
    // The maximum is used to adjust the value range of a to prevent overflow or underflow.
    f3_t max_abs_a = 0;
    for( sz_t i = 0; i <= z_size; i++ ) max_abs_a = f3_max( max_abs_a, f3_abs( a[ i * stride ] ) );
    f3_t inv_max_abs_a = max_abs_a >= DBL_MIN ? 1.0 / max_abs_a : 0.0;

    f3_t a1 = a[ 0 ] * inv_max_abs_a;
    f3_t sqr_a = f3_sqr( a1 );
    f3_t buf[ HSR_VCB_SIZE ] = { 0 };
    sz_t i = 0;
    for( ; i <= z_size - HSR_VCB_SIZE; i += HSR_VCB_SIZE )
        for( sz_t k = 0; k < HSR_VCB_SIZE; k++ )
            buf[ k ] += f3_sqr( a[ ( i + k + 1 ) * stride ] * inv_max_abs_a );

    for( ; i < z_size; i++ ) sqr_a += f3_sqr( a[ ( i + 1 ) * stride ] * inv_max_abs_a );
    for( sz_t k = 0; k < HSR_VCB_SIZE; k++ ) sqr_a += buf[ k ];

    f3_t sigma = f3_sig( a1 ) * f3_srt( sqr_a );

    f3_t sqr_v = 2 * ( sqr_a + a1 * sigma );
    f3_t inv_abs_v = sqr_v >= DBL_MIN ? 1.0 / f3_srt( sqr_v ) : 0;

    // if a1 + sigma is negative, we negate the entire vector v (does not affect the reflection)
    // this ensures that v1 is always positive. So, we need not store v1 explicitly but can reconstruct it from the remaining vectors.
    f3_t f_norm  = f3_sig( a1 ) * inv_abs_v;

    for( sz_t i = 1; i <= z_size; i++ )
    {
        f3_t v = a[ i * stride ] * inv_max_abs_a;
        a[ i * stride ] = 0;
        w[ i * stride ] = v * f_norm;
    }

    a[ 0 ] = -sigma * max_abs_a;
}

//----------------------------------------------------------------------------------------------------------------------

/** Computes the transformation vector w from w-tail strored in a
 *  Returns true in case the transformation differs from unity ('unity' would not change any values)
 */
static bl_t hsr_create_w( const f3_t* restrict r, sz_t r_stride, sz_t z_size, f3_t* restrict w )
{
    for( sz_t i = 1; i <= z_size; i++ ) w[ i ] = r[ i * r_stride ];

    f3_t sqr_w = 0;
    f3_t buf[ HSR_VCB_SIZE ] = { 0 };
    sz_t i = 0;
    for( ; i <= z_size - HSR_VCB_SIZE; i += HSR_VCB_SIZE )
        for( sz_t k = 0; k < HSR_VCB_SIZE; k++ )
            buf[ k ] += f3_sqr( w[ i + k + 1 ] );

    for( ; i < z_size; i++ ) sqr_w += f3_sqr( w[ i + 1 ] );
    for( sz_t k = 0; k < HSR_VCB_SIZE; k++ ) sqr_w += buf[ k ];
    w[ 0 ] = f3_srt( f3_max( 0, 1.0 - sqr_w ) );

    return sqr_w >= DBL_EPSILON;
}

//----------------------------------------------------------------------------------------------------------------------

/// Transforms a block of vertical vectors
static void hsr_vmap_block( const f3_t* restrict r, sz_t r_stride, f3_t* restrict m, sz_t m_stride, sz_t v_size, sz_t h_size )
{
    if( h_size <= 0 ) return;

    sz_t z_size = v_size - 1;
    assert( z_size < HSR_SIZE );
    f3_t w[ HSR_SIZE ] = {0};
    if( hsr_create_w( r, r_stride, z_size, w ) )
    {
        sz_t k = 0;
        for( ; k <= h_size - HSR_L1_BUF_SIZE; k += HSR_L1_BUF_SIZE )
        {
            f3_t buf[ HSR_L1_BUF_SIZE ] = { 0 };
            for( sz_t i = 1; i < v_size; i++ )
                for( sz_t j = 0; j < HSR_L1_BUF_SIZE; j++ )
                    buf[ j ] += w[ i ] * m[ i * m_stride + k + j ];

            for( sz_t j = 0; j < HSR_L1_BUF_SIZE; j++ ) buf[ j ] += w[ 0 ] * m[ k + j ]; // isolated for precision

            for( sz_t i = 0; i < v_size; i++ )
                for( sz_t j = 0; j < HSR_L1_BUF_SIZE; j++ )
                    m[ i * m_stride + k + j ] -= 2.0 * buf[ j ] * w[ i ];
        }

        if( k < h_size )
        {
            f3_t buf[ HSR_L1_BUF_SIZE ] = { 0 };
            for( sz_t i = 1; i < v_size; i++ )
                for( sz_t j = k; j < h_size; j++ )
                    buf[ j - k ] += w[ i ] * m[ i * m_stride + j ];

            for( sz_t j = k; j < h_size; j++ ) buf[ j - k ] += w[ 0 ] * m[ j ]; // isolated for precision

            for( sz_t i = 0; i < v_size; i++ )
                for( sz_t j = k; j < h_size; j++ )
                    m[ i * m_stride + j ] -= 2.0 * buf[ j - k ] * w[ i ];
        }
    }
    else
    {
        if( v_size > 0 ) for( sz_t j = 0; j < h_size; j++ ) m[ j ] *= -1;
    }

}

//----------------------------------------------------------------------------------------------------------------------

/// Transforms a block of horizontal vectors
static void hsr_hmap_block( const f3_t* restrict r, sz_t r_stride, f3_t* restrict m, sz_t m_stride, sz_t v_size, sz_t h_size )
{
    sz_t z_size = h_size - 1;
    assert( z_size < HSR_SIZE );
    f3_t w[ HSR_SIZE ] = {0};
    if( hsr_create_w( r, r_stride, z_size, w ) )
    {
        for( sz_t i = 0; i < v_size; i++ )
        {
            f3_t buf[ HSR_VCB_SIZE ] = { 0 };

            sz_t j = 1;
            for( ; j <= h_size-HSR_VCB_SIZE; j += HSR_VCB_SIZE )
                for( sz_t k = 0; k < HSR_VCB_SIZE; k++ )
                    buf[ k ] += w[ j + k ] * m[ i * m_stride + j + k ];

            f3_t sum = 0;
            for( ; j < h_size; j++ ) sum += w[ j ] * m[ i * m_stride + j ];
            for( sz_t k = 0; k < HSR_VCB_SIZE; k += 2 ) { sum += buf[ k ] + buf[ k + 1 ]; }
            sum += w[ 0 ] * m[ i * m_stride ]; // isolated for precision


            for( sz_t j = 0; j < h_size; j++ ) m[ i * m_stride + j ] -= 2.0 * sum * w[ j ];
        }
    }
    else
    {
        if( h_size > 0 ) for( sz_t i = 0; i < v_size; i++ ) m[ i * m_stride ] *= -1;
    }

}

//----------------------------------------------------------------------------------------------------------------------

/// Creates a vertical oblique wt-field inside matrix a
static void hsr_vfield_create( sz_t a_rows, sz_t a_cols, sz_t a_stride, f3_t* restrict a, sz_t a_row, sz_t a_col, sz_t z_size, sz_t f_size )
{
    sz_t f_size_0 = sz_min( a_cols - a_col, f_size );
    for( sz_t i = 0; i < f_size_0; i++ )
    {
        sz_t a_row_i = a_row + i;
        sz_t a_col_i = a_col + i;
        sz_t z_size_i = sz_min( a_rows - a_row_i - 1, z_size );
        if( z_size_i <= 0 ) break;

        f3_t* a_data = a + a_stride * a_row_i + a_col_i;
        hsr_create_awt( a_data, a_data, a_stride, z_size_i );
        hsr_vmap_block( a_data, a_stride, a_data + 1, a_stride, z_size_i + 1, f_size_0 - i - 1 );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// Creates a horizontal oblique wt-field inside matrix a
static void hsr_hfield_create( sz_t a_rows, sz_t a_cols, sz_t a_stride, f3_t* restrict a, sz_t a_row, sz_t a_col, sz_t z_size, sz_t f_size )
{
    sz_t f_size_0 = sz_min( a_rows - a_row, f_size );
    for( sz_t i = 0; i < f_size_0; i++ )
    {
        sz_t a_row_i = a_row + i;
        sz_t a_col_i = a_col + i;
        sz_t z_size_i = sz_min( a_cols - a_col_i - 1, z_size );
        if( z_size_i <= 0 ) break;

        f3_t* a_data = a + a_stride * a_row_i + a_col_i;
        hsr_create_awt( a_data, a_data, 1, z_size_i );
        hsr_hmap_block( a_data, 1, a_data + a_stride, a_stride, f_size_0 - i - 1, z_size_i + 1 );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// Creates awt with vertical vectors a, w at common row but different columns in same matrix
static void hsr_vstrip_create( sz_t a_rows, sz_t a_cols, sz_t a_stride, f3_t* restrict a, sz_t a_row, sz_t a_col, sz_t w_col, sz_t z_size )
{
    sz_t z_size1 = sz_min( a_rows - a_row - 1, z_size );
    if( z_size1 <= 0 ) return;
    hsr_create_awt( a + a_stride * a_row + a_col, a + a_stride * a_row + w_col, a_stride, z_size1 );
}

//----------------------------------------------------------------------------------------------------------------------

/// Transforms a block of vertical vectors from a vertical w (no restrict because a,m can overlap)
static void hsr_vstrip_vmap( sz_t a_rows, sz_t a_cols, sz_t a_stride, const f3_t* a, sz_t a_row, sz_t a_col, sz_t z_size, sz_t m_rows, sz_t m_cols, sz_t m_stride, f3_t* m, sz_t m_col, sz_t m_size )
{
    sz_t m_size_0 = sz_min( m_cols - m_col, m_size );
    sz_t z_size1 = sz_min( a_rows - a_row - 1, z_size );
    if( z_size1 <= 0 ) return;
    const f3_t* a_data = a + a_stride * a_row + a_col;
    hsr_vmap_block( a_data, a_stride, m + m_stride * a_row + m_col, m_stride, z_size1 + 1, m_size_0 );
}

//----------------------------------------------------------------------------------------------------------------------

/// Creates awt with horizontal vectors a, w at common column but different rows in same matrix
static void hsr_hstrip_create( sz_t a_rows, sz_t a_cols, sz_t a_stride, f3_t* restrict a, sz_t a_row, sz_t a_col, sz_t w_row, sz_t z_size )
{
    sz_t z_size1 = sz_min( a_cols - a_col - 1, z_size );
    if( z_size1 <= 0 ) return;
    hsr_create_awt( a + a_stride * a_row + a_col, a + a_stride * w_row + a_col, 1, z_size1 );
}

//----------------------------------------------------------------------------------------------------------------------

/// Transforms a block of horizontal vectors from a horizontal w (no 'restrict' because a,m can overlap)
static void hsr_hstrip_hmap( sz_t a_rows, sz_t a_cols, sz_t a_stride, const f3_t* a, sz_t a_row, sz_t a_col, sz_t z_size, sz_t m_rows, sz_t m_cols, sz_t m_stride, f3_t* m, sz_t m_row, sz_t m_size )
{
    sz_t m_size_0 = sz_min( m_rows - m_row, m_size );
    sz_t z_size1 = sz_min( a_cols - a_col - 1, z_size );
    if( z_size1 <= 0 ) return;
    const f3_t* a_data = a + a_stride * a_row + a_col;
    hsr_hmap_block( a_data, 1, m + m_stride * m_row + a_col, m_stride, m_size_0, z_size1 + 1 );
}

//----------------------------------------------------------------------------------------------------------------------

/// Transforms a block of vertical vectors from a block of vertical w (no 'restrict' because a,m can overlap)
static void hsr_vfield_vmap( sz_t a_rows, sz_t a_cols, sz_t a_stride, const f3_t* a, sz_t a_row, sz_t a_col, sz_t z_size, sz_t f_size, f3_t* m, sz_t m_stride, sz_t m_col, sz_t m_size )
{
    sz_t f_size_0 = sz_min( a_cols - a_col, f_size );

    for( sz_t i = 0; i < f_size_0; i++ )
    {
        sz_t a_row_i = a_row + i;
        sz_t a_col_i = a_col + i;
        sz_t z_size_i = sz_min( a_rows - a_row_i - 1, z_size );
        if( z_size_i <= 0 ) break;

        const f3_t* a_data = a + a_stride * a_row_i + a_col_i;
        hsr_vmap_block( a_data, a_stride, m + m_stride * a_row_i + m_col, m_stride, z_size_i + 1, m_size );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// Transforms a block of vertical vectors from a block of horizontal w (no 'restrict' because a,m can overlap)
static void hsr_hfield_vmap( sz_t a_rows, sz_t a_cols, sz_t a_stride, const f3_t* a, sz_t a_row, sz_t a_col, sz_t z_size, sz_t f_size, f3_t* m, sz_t m_stride, sz_t m_col, sz_t m_size )
{
    sz_t f_size_0 = sz_min( a_rows - a_row, f_size );

    for( sz_t i = 0; i < f_size_0; i++ )
    {
        sz_t a_row_i = a_row + i;
        sz_t a_col_i = a_col + i;
        sz_t z_size_i = sz_min( a_cols - a_col_i - 1, z_size );
        if( z_size_i <= 0 ) break;

        const f3_t* a_data = a + a_stride * a_row_i + a_col_i;
        hsr_vmap_block( a_data, 1, m + m_stride * a_col_i + m_col, m_stride, z_size_i + 1, m_size );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// Transforms a block of horizontal vectors from a block of horizontal w (no 'restrict' because a,m can overlap)
static void hsr_hfield_hmap( sz_t a_rows, sz_t a_cols, sz_t a_stride, const f3_t* a, sz_t a_row, sz_t a_col, sz_t z_size, sz_t f_size, f3_t* m, sz_t m_stride, sz_t m_row, sz_t m_size )
{
    sz_t f_size_0 = sz_min( a_rows - a_row, f_size );

    for( sz_t i = 0; i < f_size_0; i++ )
    {
        sz_t a_row_i = a_row + i;
        sz_t a_col_i = a_col + i;
        sz_t z_size_i = sz_min( a_cols - a_col_i - 1, z_size );
        if( z_size_i <= 0 ) break;

        const f3_t* a_data = a + a_stride * a_row_i + a_col_i;
        hsr_hmap_block( a_data, 1, m + m_stride * m_row + a_col_i, m_stride, m_size, z_size_i + 1 );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// Reverse transforms a block of horizontal vectors from a block of horizontal w  (no 'restrict' because a,m can overlap)
static void hsr_reverse_hfield_hmap( sz_t a_rows, sz_t a_cols, sz_t a_stride, const f3_t* a, sz_t a_row, sz_t a_col, sz_t z_size, sz_t f_size, f3_t* m, sz_t m_stride, sz_t m_row, sz_t m_size )
{
    sz_t f_size_0 = sz_min( a_rows - a_row, f_size );

    for( sz_t i = sz_min( f_size_0 - 1, ( sz_t )a_cols - a_col - 2 ); i >= 0; i-- )
    {
        sz_t a_row_i = a_row + i;
        sz_t a_col_i = a_col + i;
        sz_t z_size_i = sz_min( a_cols - a_col_i - 1, z_size );
        if( z_size_i > 0 )
        {
            const f3_t* a_data = a + a_stride * a_row_i + a_col_i;
            hsr_hmap_block( a_data, 1, m + m_stride * m_row + a_col_i, m_stride, m_size, z_size_i + 1 );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// Reverse transforms a block of horizontal vectors from a block of vertical w (no 'restrict' because a,m can overlap)
static void hsr_reverse_vfield_hmap( sz_t a_rows, sz_t a_cols, sz_t a_stride, const f3_t* a, sz_t a_row, sz_t a_col, sz_t z_size, sz_t f_size, f3_t* m, sz_t m_stride, sz_t m_row, sz_t m_size )
{
    sz_t f_size_0 = sz_min( a_cols - a_col, f_size );

    for( sz_t i = sz_min( f_size_0 - 1, ( sz_t )a_rows - a_row - 2 ); i >= 0; i-- )
    {
        sz_t a_row_i = a_row + i;
        sz_t a_col_i = a_col + i;
        sz_t z_size_i = sz_min( a_rows - a_row - i - 1, z_size );
        if( z_size_i > 0 )
        {
            const f3_t* a_data = a + a_stride * a_row_i + a_col_i;
            hsr_hmap_block( a_data, a_stride, m + m_stride * m_row + a_row_i, m_stride, m_size, z_size_i + 1 );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// givens rotation

//----------------------------------------------------------------------------------------------------------------------

typedef struct grt_s { f3_t c; f3_t s; } grt_s;

//----------------------------------------------------------------------------------------------------------------------

/// Applies specific rotation to vector (a,b) assuming it annihilates b.
static inline void grt_s_init_to_annihilate_b( grt_s* o, f3_t a, f3_t b )
{
    f3_t sig_a = f3_sig( a );
    f3_t r = hypot( b, a );
    o->c = ( r > f3_lim_min ) ? sig_a * a / r : 1;
    o->s = ( r > f3_lim_min ) ? sig_a * b / r : 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// Applies annihilation rotation to vector (a,b) zeroing b
static inline void grt_s_annihilate_b( f3_t* a, f3_t* b )
{
    *a = f3_sig( *a ) * hypot( *a, *b );
    *b = 0;
}

//----------------------------------------------------------------------------------------------------------------------

static inline void grt_s_init_and_annihilate_b( grt_s* o, f3_t* a, f3_t* b )
{
    grt_s_init_to_annihilate_b( o, *a, *b );
    grt_s_annihilate_b( a, b );
}

//----------------------------------------------------------------------------------------------------------------------

static inline void grt_s_rotate( const grt_s* o, f3_t* a, f3_t* b )
{
    f3_t a0 = *a;
    *a = o->c * a0 + o->s * *b;
    *b = o->c * *b - o->s * a0;
}

//----------------------------------------------------------------------------------------------------------------------

/** We store the givens rotation without accuracy-loss in a single f3_t value using a variation of Stewart's approach 1976:
 *  Stewart, G.W.. "The Economical Storage of Plane Rotations.." Numerische Mathematik 25 (1975/76): 137-138
 */

/// rho from givens rotation components
static inline f3_t grho_from_grt( f3_t c, f3_t s )
{
    f3_t abs_c = f3_abs( c );
    return f3_abs( s ) < abs_c ?
                0.5 * f3_sig( c ) * s :
                   abs_c > f3_lim_min ?
                       2.0 * f3_sig( s ) / c :
                           f3_sig( s );

}

//----------------------------------------------------------------------------------------------------------------------

/// rho to givens rotation components
static inline void grho_to_grt( f3_t r, f3_t* c, f3_t* s )
{
    f3_t abs_r = f3_abs( r );
    if( abs_r < 0.5 )
    {
        *s = 2.0 * r;
        *c = f3_srt( 1.0 - *s * *s );
    }
    else if( abs_r > 2.0 )
    {
        *c = 2.0 / abs_r;
        *s = f3_sig( r ) * f3_srt( 1.0 - *c * *c );
    }
    else
    {
        *s = f3_sig( r );
        *c = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Matrix Operations

//----------------------------------------------------------------------------------------------------------------------

/// Diagonals 1, rest 0
static inline void mat_set_one( sz_t rows, sz_t cols, f3_t* restrict m, sz_t stride )
{
    for( sz_t i = 0; i < rows; i++ )
    {
        for( sz_t j = 0; j < cols; j++ ) m[ i * stride + j ] = ( i == j ) ? 1 : 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

// multiplies a constant to a specified row
static void mul_f3_to_row( sz_t m_rows, sz_t m_cols, f3_t* restrict m, sz_t m_stride, f3_t v, sz_t row )
{
    if( row < 0 || row >= m_rows ) return;
    f3_t* mj = m + row * m_stride;
    for( sz_t i = 0; i < m_cols; i++ ) mj[ i ] *= v;
}

//----------------------------------------------------------------------------------------------------------------------

// swaps two rows
static void swap_rows( sz_t m_rows, sz_t m_cols, f3_t* restrict m, sz_t m_stride, sz_t row1, sz_t row2 )
{
    if( row1 < 0 || row1 >= m_rows ) return;
    if( row2 < 0 || row2 >= m_rows ) return;
    f3_t* m_row1 = m + row1 * m_stride;
    f3_t* m_row2 = m + row2 * m_stride;
    for( sz_t i = 0; i < m_cols; i++ ) f3_t_swap( m_row1 + i, m_row2 + i );
}

//----------------------------------------------------------------------------------------------------------------------

// rotates two adjacent rows (row, row + 1)
static void arow_rotate( sz_t m_rows, sz_t m_cols, f3_t* restrict m, sz_t m_stride, sz_t row, const grt_s* gr )
{
    if( row < 0 || row >= m_rows - 1 ) return;
    f3_t* mj = m + row * m_stride;
    for( sz_t i = 0; i < m_cols; i++ ) grt_s_rotate( gr, mj + i, mj + m_stride + i );
}

//----------------------------------------------------------------------------------------------------------------------

// converts from upper bi-diagonal to lower bi-diagonal
static void ubd_to_lbd( sz_t a_rows, sz_t a_cols, f3_t* restrict a, sz_t a_stride, f3_t* restrict v, sz_t v_stride )
{
    sz_t n = sz_min( a_rows, a_cols );
    if( n <= 1 ) return; // nothing to do
    grt_s gr;
    sz_t v_rows = a_rows;
    sz_t v_cols = a_cols;
    for( sz_t j = 0; j < n - 1; j++ )
    {
        f3_t* aj = a + j * ( a_stride + 1 );
        grt_s_init_and_annihilate_b( &gr, aj, aj + 1 );
        grt_s_rotate( &gr, aj + a_stride, aj + a_stride + 1 );
        if( v ) arow_rotate( v_rows, v_cols, v, v_stride, j, &gr );
    }
}

//----------------------------------------------------------------------------------------------------------------------

// converts from lower bi-diagonal to upper bi-diagonal
static void lbd_to_ubd( sz_t a_rows, sz_t a_cols, f3_t* restrict a, sz_t a_stride, f3_t* restrict u, sz_t u_stride )
{
    sz_t n = sz_min( a_rows, a_cols );
    if( n <= 1 ) return; // nothing to do
    grt_s gr;
    sz_t u_rows = a_cols;
    sz_t u_cols = a_rows;
    for( sz_t j = 0; j < n - 1; j++ )
    {
        f3_t* aj = a + j * ( a_stride + 1 );
        grt_s_init_and_annihilate_b( &gr, aj, aj + a_stride );
        grt_s_rotate( &gr, aj + 1, aj + a_stride + 1 );
        if( u ) arow_rotate( u_rows, u_cols, u, u_stride, j, &gr );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** Reverses a bi-diagonal ordering matrix by reversing the order of rows and flipping the off diagonal
 *  thin u,v supported
 */
static void ubd_flip_htp( sz_t a_rows, sz_t a_cols, f3_t* restrict a, sz_t a_stride, f3_t* restrict u, sz_t u_stride, f3_t* restrict v, sz_t v_stride )
{
    ubd_to_lbd( a_rows, a_cols, a, a_stride, v, v_stride );

    sz_t n = sz_min( a_rows, a_cols );
    for( sz_t i = 0; i < n / 2; i++ )
    {
        sz_t ic = n - i - 1;
        f3_t_swap( a + i * ( a_stride + 1 )    , a + ic * ( a_stride + 1 ) );
    }

    for( sz_t i = 0; i < n - 1; i++ )
    {
        sz_t ic = n - i - 1;
        f3_t_swap( a + i * ( a_stride + 1 ) + 1, a + ic * ( a_stride + 1 ) - 1 );
    }

    for( sz_t i = 0; i < n / 2; i++ )
    {
        sz_t ic = n - i - 1;
        if( u )
        {
            sz_t u_cols = a_rows;
            f3_t* p1 = u + i  * u_stride;
            f3_t* p2 = u + ic * u_stride;
            for( sz_t j = 0; j < u_cols; j++ ) f3_t_swap( p1 + j, p2 + j );
        }

        if( v )
        {
            sz_t v_cols = a_cols;
            f3_t* p1 = v + i  * v_stride;
            f3_t* p2 = v + ic * v_stride;
            for( sz_t j = 0; j < v_cols; j++ ) f3_t_swap( p1 + j, p2 + j );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Band Diagonal Decomposition

//----------------------------------------------------------------------------------------------------------------------

void mocut_thin_bnd( size_t a_rows, size_t a_cols, double* restrict a, size_t a_stride, double* restrict u, size_t u_stride, double* restrict v, size_t v_stride )
{
    assert( a_stride >= a_cols );

    sz_t n = sz_min( a_rows, a_cols );
    sz_t u_rows = n;
    sz_t u_cols = a_rows;
    if( u ) assert( u_stride >= u_cols );

    sz_t v_rows = n;
    sz_t v_cols = a_cols;
    if( v ) assert( v_stride >= v_cols );

    if( u ) mat_set_one( u_rows, u_cols, u, u_stride );
    if( v ) mat_set_one( v_rows, v_cols, v, v_stride );

    sz_t z_size = HSR_SIZE - 1;

    if( a_rows >= a_cols ) // computing the upper band-diagonal
    {
        for( sz_t diag = 0; diag < n; diag += BAND_SIZE )
        {
            // left col-band create
            for( sz_t row = a_rows - 2 - ( ( a_rows - 2 - diag ) % z_size ); row >= diag; row -= z_size )
            {
                hsr_vfield_create( a_rows, a_cols, a_stride, a, row, diag, z_size, BAND_SIZE );
            }

            // left col-band map
            #pragma omp parallel for
            for( sz_t a_col = diag + BAND_SIZE; a_col < a_cols; a_col += TPART_SIZE )
            {
                for( sz_t a_row = a_rows - 2 - ( ( a_rows - 2 - diag ) % z_size ); a_row >= diag; a_row -= z_size )
                {
                    hsr_vfield_vmap( a_rows, a_cols, a_stride, a, a_row, diag, z_size, BAND_SIZE, a, a_stride, a_col, sz_min( TPART_SIZE, a_cols - a_col ) );
                }
            }

            // upper row-band create
            for( sz_t a_col = a_cols - 2 - ( ( sz_max( 0, a_cols - 2 - diag - BAND_SIZE ) ) % z_size ); a_col >= diag + BAND_SIZE; a_col -= z_size )
            {
                hsr_hfield_create( a_rows, a_cols, a_stride, a, diag, a_col, z_size, BAND_SIZE );
            }

            // upper row-band map
            #pragma omp parallel for
            for( sz_t a_row = diag + BAND_SIZE; a_row < a_rows; a_row += TPART_SIZE )
            {
                for( sz_t a_col = a_cols - 2 - ( ( sz_max( 0, a_cols - 2 - diag - BAND_SIZE ) ) % z_size ); a_col >= diag + BAND_SIZE; a_col -= z_size )
                {
                    hsr_hfield_hmap( a_rows, a_cols, a_stride, a, diag, a_col, z_size, BAND_SIZE, a, a_stride, a_row, sz_min( TPART_SIZE, a_rows - a_row ) );
                }
            }
        }

        if( u )
        {
            if( a_rows == a_cols ) // on square matrices we can use the full-algorithm, which is slightly more efficient than the thin algorithm
            {
                #pragma omp parallel for
                for( sz_t u_col = 0; u_col < u_cols; u_col += TPART_SIZE )
                {
                    for( sz_t diag = 0; diag < n; diag += BAND_SIZE )
                    {
                        for( sz_t a_row = a_rows - 2 - ( ( a_rows - 2 - diag ) % z_size ); a_row >= diag; a_row -= z_size )
                        {
                            if( a_row < u_col + diag + TPART_SIZE ) hsr_vfield_vmap( a_rows, a_cols, a_stride, a, a_row, diag, z_size, BAND_SIZE, u, u_stride, u_col, sz_min( TPART_SIZE, u_cols - u_col ) );
                        }
                    }
                }
            }
            else // thin-algorithm
            {
                #pragma omp parallel for
                for( sz_t u_row = 0; u_row < u_rows; u_row += TPART_SIZE )
                {
                    // TPART_SIZE + u_row - 1: filters out the area of u which is 0
                    sz_t diag_limit = sz_min( n - 1, TPART_SIZE + u_row - 1 );
                    for( sz_t diag = diag_limit - ( diag_limit % BAND_SIZE ); diag >= 0; diag -= BAND_SIZE )
                    {
                        for( sz_t a_row = diag; a_row <= a_rows - 2 - ( ( a_rows - 2 - diag ) % z_size ); a_row += z_size )
                        {
                            hsr_reverse_vfield_hmap( a_rows, a_cols, a_stride, a, a_row, diag, z_size, BAND_SIZE, u, u_stride, u_row, sz_min( TPART_SIZE, u_rows - u_row ) );
                        }
                    }
                }
            }
        }

        if( v )
        {
            #pragma omp parallel for
            for( sz_t v_col = 0; v_col < v_cols; v_col += TPART_SIZE )
            {
                for( sz_t diag = 0; diag < n - 1; diag += BAND_SIZE )
                {
                    for( sz_t a_col = a_cols - 2 - ( ( sz_max( 0, a_cols - 2 - diag - BAND_SIZE ) ) % z_size ); a_col >= diag + BAND_SIZE; a_col -= z_size )
                    {
                        if( a_col < v_col + diag + TPART_SIZE )
                        {
                            hsr_hfield_vmap( a_rows, a_cols, a_stride, a, diag, a_col, z_size, BAND_SIZE, v, v_stride, v_col, sz_min( TPART_SIZE, v_cols - v_col ) );
                        }
                    }
                }
            }
        }

        // set off-band values in a to zero
        for( sz_t i = 0; i < n; i++ )
        {
            for( sz_t j = 0;                 j < i;      j++ ) a[ i * a_stride + j ] = 0;
            for( sz_t j = i + BAND_SIZE + 1; j < a_cols; j++ ) a[ i * a_stride + j ] = 0;
        }

        for( sz_t i = n; i < a_rows; i++ )
        {
            for( sz_t j = 0; j < a_cols; j++ ) a[ i * a_stride + j ] = 0;
        }
    }
    else // ( a_rows < a_cols ) computing the lower band-diagonal
    {
        for( sz_t diag = 0; diag < n; diag += BAND_SIZE )
        {
            // upper row-band create
            for( sz_t a_col = a_cols - 2 - ( ( sz_max( 0, a_cols - 2 - diag ) ) % z_size ); a_col >= diag; a_col -= z_size )
            {
                hsr_hfield_create( a_rows, a_cols, a_stride, a, diag, a_col, z_size, BAND_SIZE );
            }

            // upper row-band map
            #pragma omp parallel for
            for( sz_t a_row = diag + BAND_SIZE; a_row < a_rows; a_row += TPART_SIZE )
            {
                for( sz_t a_col = a_cols - 2 - ( ( sz_max( 0, a_cols - 2 - diag ) ) % z_size ); a_col >= diag; a_col -= z_size )
                {
                    hsr_hfield_hmap( a_rows, a_cols, a_stride, a, diag, a_col, z_size, BAND_SIZE, a, a_stride, a_row, sz_min( TPART_SIZE, a_rows - a_row ) );
                }
            }

            // left col-band create
            for( sz_t a_row = a_rows - 2 - ( ( a_rows - 2 - diag - BAND_SIZE ) % z_size ); a_row >= diag + BAND_SIZE; a_row -= z_size )
            {
                hsr_vfield_create( a_rows, a_cols, a_stride, a, a_row, diag, z_size, BAND_SIZE );
            }

            // left col-band map
            #pragma omp parallel for
            for( sz_t a_col = diag + BAND_SIZE; a_col < a_cols; a_col += TPART_SIZE )
            {
                for( sz_t a_row = a_rows - 2 - ( ( a_rows - 2 - diag - BAND_SIZE ) % z_size ); a_row >= diag + BAND_SIZE; a_row -= z_size )
                {
                    hsr_vfield_vmap( a_rows, a_cols, a_stride, a, a_row, diag, z_size, BAND_SIZE, a, a_stride, a_col, sz_min( TPART_SIZE, a_cols - a_col ) );
                }
            }
        }

        if( v )
        {
            sz_t block_size = TPART_SIZE;

            #pragma omp parallel for
            for( sz_t v_row = 0; v_row < v_rows; v_row += block_size )
            {
                // block_size + v_row - 1: filters out the area of u which is 0
                sz_t diag_limit = sz_min( n - 1, block_size + v_row - 1 );

                for( sz_t diag = diag_limit - ( diag_limit % BAND_SIZE ); diag >= 0; diag -= BAND_SIZE )
                {
                    for( sz_t a_col = diag; a_col <= a_cols - 2 - ( ( a_cols - 2 - diag ) % z_size ); a_col += z_size )
                    {
                        hsr_reverse_hfield_hmap( a_rows, a_cols, a_stride, a, diag, a_col, z_size, BAND_SIZE, v, v_stride, v_row, sz_min( block_size, v_rows - v_row ) );
                    }
                }
            }
        }

        if( u )
        {
            sz_t block_size = TPART_SIZE;
            #pragma omp parallel for
            for( sz_t u_col = 0; u_col < u_cols; u_col += block_size )
            {
                for( sz_t diag = 0; diag < n - 1; diag += BAND_SIZE )
                {
                    for( sz_t a_row = a_rows - 2 - ( ( a_rows - 2 - diag - BAND_SIZE ) % z_size ); a_row >= diag + BAND_SIZE; a_row -= z_size )
                    {
                        if( a_row < u_col + diag + block_size ) hsr_vfield_vmap( a_rows, a_cols, a_stride, a, a_row, diag, z_size, BAND_SIZE, u, u_stride, u_col, sz_min( block_size, u_cols - u_col ) );
                    }
                }
            }
        }

        // set off-band values in a to zero
        for( sz_t i = 0; i < a_rows; i++ )
        {
            for( sz_t j = 0;     j < i - BAND_SIZE; j++ ) a[ i * a_stride + j ] = 0;
            for( sz_t j = i + 1; j < a_cols;        j++ ) a[ i * a_stride + j ] = 0;
        }

    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Bi-Diagonal Decomposition

//----------------------------------------------------------------------------------------------------------------------

void mocut_thin_bid( size_t a_rows, size_t a_cols, double* restrict a, size_t a_stride, double* restrict u, size_t u_stride, double* restrict v, size_t v_stride )
{
    mocut_thin_bnd( a_rows, a_cols, a, a_stride, u, u_stride, v, v_stride );

    sz_t n = sz_min( a_rows, a_cols );
    sz_t u_cols = a_rows;
    sz_t v_cols = a_cols;

    /** Turning A from band diagonal to bi-diagonal:
     *
     *  Iteratively until A is bi-diagonal
     *  - Zero the upper left strip of the (residual) band.
     *  - This creates zeros outside the band which are chase down by alternating partial QR and LQ transformations.
     *
     *  The accrued orthogonal transformations are temporarily stored in the memory area of A (outside the bi-diagonal area).
     *
     *  Processing U,V:
     *  The partial orthogonal transformations are permuted into a sequence of MOCU-transformations to be applied to U and V.
     */
    for( sz_t i = 0; i < n - 2; i++ )
    {
        sz_t z_size = sz_min( BAND_SIZE - 1, n - i - 2 );
        if( z_size <= 0 ) break;

        if( a_rows >= a_cols )
        {
            f3_t* a_row_i = a + i * ( a_stride + 1 ) + 1;
            hsr_create_awt( a_row_i, a_row_i, 1, z_size );
            hsr_hmap_block( a_row_i, 1, a_row_i + a_stride, a_stride, z_size + 1, z_size + 1 );

            for( sz_t j = i + 1; j < n - 1; j += z_size + 1 )
            {
                hsr_vstrip_create( n, n, a_stride, a, j, j, i, z_size );
                hsr_vstrip_vmap  ( n, n, a_stride, a, j, i, z_size, n, n, a_stride, a, j + 1, 2 * z_size + 1 );
                hsr_hstrip_create( n, n, a_stride, a, j, j + z_size + 1, i, z_size );
                hsr_hstrip_hmap  ( n, n, a_stride, a, i, j + z_size + 1, z_size, n, n, a_stride, a, j + 1, 2 * z_size + 1 );
            }
        }
        else
        {
            f3_t* a_col_i = a + i * ( a_stride + 1 ) + a_stride;
            hsr_create_awt( a_col_i, a_col_i, a_stride, z_size );
            hsr_vmap_block( a_col_i, a_stride, a_col_i + 1, a_stride, z_size + 1, z_size + 1 );

            for( sz_t j = i + 1; j < n - 1; j += z_size + 1 )
            {
                hsr_hstrip_create( n, n, a_stride, a, j, j, i, z_size );
                hsr_hstrip_hmap  ( n, n, a_stride, a, i, j, z_size, n, n, a_stride, a, j + 1, 2 * z_size + 1 );
                hsr_vstrip_create( n, n, a_stride, a, j + z_size + 1, j, i, z_size );
                hsr_vstrip_vmap  ( n, n, a_stride, a, j + z_size + 1, i, z_size, n, n, a_stride, a, j + 1, 2 * z_size + 1 );
            }
        }
    }

    // u and v algorithms are idenical for upper and lower bi-diagonal
    if( v )
    {
        #pragma omp parallel for
        for( sz_t col = 0; col < v_cols; col += TPART_SIZE )
        {
            for( sz_t i = 0; i < n - 2; i += RPART_SIZE )
            {
                sz_t z_size = sz_min( BAND_SIZE - 1, n - 2 - i );
                for( sz_t j = n - 2 - ( ( n - i - 3 ) % ( z_size + 1 ) ); j > i; j -= z_size + 1 )
                {
                    hsr_hfield_vmap( n, n, a_stride, a, i, j, z_size, RPART_SIZE, v, v_stride, col, sz_min( TPART_SIZE, v_cols - col ) );
                }
            }
        }
    }

    if( u )
    {
        #pragma omp parallel for
        for( sz_t col = 0; col < u_cols; col += TPART_SIZE )
        {
            for( sz_t i = 0; i < n - 2; i += RPART_SIZE )
            {
                sz_t z_size = sz_min( BAND_SIZE - 1, n - 2 - i );
                for( sz_t j = n - 2 - ( ( n - i - 3 ) % ( z_size + 1 ) ); j > i; j -= z_size + 1 )
                {
                    hsr_vfield_vmap( n, n, a_stride, a, j, i, z_size, RPART_SIZE, u, u_stride, col, sz_min( TPART_SIZE, u_cols - col ) );
                }
            }
        }
    }

    // set off-bidiag values in a to zero
    for( sz_t i = 0; i < n; i++ )
    {
        sz_t k = ( a_rows >= a_cols ) ? i : i - 1;
        for( sz_t j = 0;     j < k; j++ ) a[ i * a_stride + j ] = 0;
        for( sz_t j = k + 2; j < n; j++ ) a[ i * a_stride + j ] = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// Singular Value Decomposition

//----------------------------------------------------------------------------------------------------------------------

/** Tests for defective sub-diagonal element a01 enclosed by diagonal elements a00 and a11
 *  If found, a01 is set to zero and the function returns 'true'
 */
static bl_t subd_defector( f3_t a00, f3_t* a01, f3_t a11, f3_t limit )
{
    f3_t a01_abs = f3_abs( *a01 );
    if( ( a01_abs < f3_lim_min ) || ( ( a01_abs < f3_abs( a00 ) * limit ) && ( a01_abs < f3_abs( a11 ) * limit ) ) )
    {
        *a01 = 0;
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------

static void grho_zero( sz_t rows, sz_t stride, f3_t* restrict grho, sz_t row_start, sz_t row_end )
{
    for( sz_t j = 0; j < rows; j++ )
        for( sz_t i = row_start; i < row_end; i++ )
            grho[ j * stride + i ] = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/** MOCUT operation via givens rotations
 *  m:  matrix to be rotated
 *  gvr: matrix of givens rotations (size: m->rows * gvr_rows)
 *  rw_start, rw_end: row window on m and gr and gr to which all operations apply
 */
static void block_grho_arow_rotate( sz_t m_rows, sz_t m_cols, sz_t m_stride, f3_t* restrict m, sz_t gr_rows, sz_t gr_stride, const f3_t* restrict gr, sz_t rw_start, sz_t rw_end )
{
    const sz_t  gr_block = SVD_GR_BLOCK;
    //const sz_t col_block = SVD_COL_BLOCK;

    sz_t col_block = SVD_MIN_COL_BLOCK;
    while( ( col_block < SVD_MAX_COL_BLOCK ) && ( 32 * col_block < m_cols ) ) col_block *= 2;

    sz_t rw_size = rw_end - rw_start;
    sz_t dg_size = rw_size + gr_rows;

    sz_t b_rows = ( rw_size / gr_block ) + ( rw_size % gr_block != 0 );
    sz_t b_cols = ( gr_rows / gr_block ) + ( gr_rows % gr_block != 0 );
    sz_t ti_end = ( dg_size / gr_block ) + ( dg_size % gr_block != 0 );

    #pragma omp parallel for
    for( sz_t col = 0; col < m_cols; col += col_block )
    {
        sz_t block_cols = sz_min( m_cols - col, col_block );
        for( sz_t ti = 0; ti < ti_end; ti++ )
        {
            for( sz_t tj = sz_max( ti - b_rows, 0 ); tj < sz_min( ti + 1, b_cols ); tj++ )
            {
                for( sz_t j = tj * gr_block; j < sz_min( gr_rows, tj * gr_block + gr_block ); j++ )
                {
                    /** The two inner loops favor inner-parallelity over data locality (so far best on tested architectures).
                     *
                     *  An alternative (more data-local) implementation bundles multiple rows in the
                     *  inner loop, making it more complex and probably causing the compiler to opt out of verctorization.
                     */
                    for( sz_t i = rw_start + sz_max( 0, ti * gr_block - j ); i < sz_min( rw_end, rw_start + ti * gr_block - j + gr_block ); i++ )
                    {
                        if( gr[ j * gr_stride + i ] != 0 )
                        {
                            f3_t c, s;
                            grho_to_grt( gr[ j * gr_stride + i ], &c, &s );
                            f3_t* r = m + i * m_stride + col;
                            for( sz_t k = 0; k < block_cols; k++ )
                            {
                                f3_t a = r[ k ];
                                f3_t b = r[ m_stride + k ];
                                r[            k ] = a * c + b * s;
                                r[ m_stride + k ] = b * c - a * s;
                            }
                        }
                    }
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void grho_arow_rotate_and_zero( sz_t m_rows, sz_t m_cols, sz_t m_stride, f3_t* restrict m, sz_t gr_rows, sz_t gr_stride, f3_t* restrict gr, sz_t rw_start, sz_t rw_end )
{
    block_grho_arow_rotate( m_rows, m_cols, m_stride, m, gr_rows, gr_stride, gr, rw_start, rw_end );
    grho_zero(                                           gr_rows, gr_stride, gr, rw_start, rw_end );
}

//----------------------------------------------------------------------------------------------------------------------

static void uv_grho_arow_rotate_and_zero( sz_t a_rows, sz_t a_cols, sz_t u_stride, f3_t* restrict u, sz_t v_stride, f3_t* restrict v,
                                          sz_t gr_rows, sz_t gr_stride, f3_t* restrict gr_u, f3_t* restrict gr_v, sz_t rw_start, sz_t rw_end )
{
    sz_t u_rows = a_cols;
    sz_t u_cols = a_rows;
    sz_t v_rows = a_rows;
    sz_t v_cols = a_cols;
    if( u ) grho_arow_rotate_and_zero( u_rows, u_cols, u_stride, u, gr_rows, gr_stride, gr_u, rw_start, rw_end );
    if( v ) grho_arow_rotate_and_zero( v_rows, v_cols, v_stride, v, gr_rows, gr_stride, gr_v, rw_start, rw_end );
}

//----------------------------------------------------------------------------------------------------------------------

int mocut_thin_svd( size_t a_rows, size_t a_cols, double* restrict a, size_t a_stride, double* restrict u, size_t u_stride, double* restrict v, size_t v_stride )
{
    mocut_thin_bid( a_rows, a_cols, a, a_stride, u, u_stride, v, v_stride );
    if( a_rows < a_cols ) lbd_to_ubd( a_rows, a_cols, a, a_stride, u, u_stride );

    // reversing the order of bi-diagonal elements improves the overall accuracy of the chasing algorithm
    ubd_flip_htp( a_rows, a_cols, a, a_stride, u, u_stride, v, v_stride );

    sz_t n = sz_min( a_rows, a_cols );

    sz_t u_rows = n;
    sz_t u_cols = a_rows;
    sz_t v_rows = n;
    sz_t v_cols = a_cols;

    // special handling for n <= 1
    if( n <= 1 )
    {
        if( n <= 0 ) return 0; // nothing else to do

        // make sure diagonal element is >= 0
        if( a[ 0 ] < 0 )
        {
            a[0] *= -1.0;
            if( v ) mul_f3_to_row( v_rows, v_cols, v, v_stride, -1.0, 0 );
        }
        return 0;
    }

    // from here n >= 2 ...

    /** Defect elimination (maximizing stability)
     *
     *  The plain (not partitioned) chasing algorithm can not progress when intermediate subdiagonal or main diagonal elements are zero.
     *  Intermediate means: Not first or last element of the diagonal. Intermediate elements are called 'defective' if they are zero.
     *  An element is considered zero, when a nonzero givens rotation can not be computed from this element and its respective neighbors.
     *
     *  We achieve stability in 2 parts:
     *
     *  1. Defect swapping:
     *  The operation ubd -> lbd -> ubd moves defects from the diagonal to the sub-diagonal. Diagonal element remain zero only when
     *  adjacent subdiagonals were also zero. This special case leaves zero diagonals either at the beginning or the end of a partition (s. below),
     *  which is not defective under partitioning.
     *
     *  2. Irreducible partitioning:
     *  The matrix is partitioned into multiple irreducible submatrices (irreducible: all subdiagonals non-zero).
     *  Each submatrix is solved independently.
     *
     */

    // We move diagonal and sub-diagonal to first two rows of matrix
    // and use the rest of the matrix area to store temporary rotations.
    f3_t* d0 = a + a_stride; // diagonal
    f3_t* d1 = a + 1;        // sub-diagonal

    for( sz_t i = 0; i < n - 1; i++ ) d1[ i ] = a[ i * ( a_stride + 1 ) + 1 ];
    for( sz_t i = 0; i < n    ; i++ ) d0[ i ] = a[ i * ( a_stride + 1 )     ];

    // When A is too small to hold temporary rotations (n <= 3), we use this buffer:
    f3_t gro_buf[ 2 ][ 3 ];

    f3_t* grhou = gro_buf[ 0 ];
    f3_t* grhov = gro_buf[ 1 ];

    /* The constant max_grho_rows determines how much data is gathered before the parallel u, v transformation is invoked.
     * Larger intervals improve parallelity at the cost of extra wasteful operations.
     */
    const sz_t max_grho_rows = SVD_MAX_GRHO_ROWS;

    sz_t grho_rows = sz_min( max_grho_rows, sz_max( 1, ( n - 2 ) / 2 ) );
    sz_t grho_row = 0;
    sz_t grho_stride = a_stride;

    // window of rotations
    sz_t grho_wstart = n;
    sz_t grho_wend   = 0;

    if( n > 3 )
    {
        grhou = a + a_stride * 2;
        grhov = a + a_stride * ( 2 + grho_rows );
    }

    grho_zero( grho_rows, grho_stride, grhou, 0, n );
    grho_zero( grho_rows, grho_stride, grhov, 0, n );

    bl_t success = true;

    /// IRP: irreducible partition (also called irreducible block)
    sz_t irp_start = 0; // start index
    sz_t irp_end   = n; // end index

    /** Reduce IRP via iterative chasing:
     *  In practice, convergence hardly ever needs more than n * 3 cycles.
     *  More cycles are typically invoked when (near-) defective elements occur,
     *  which were not detected in the defect elimination.
     *  If max cycles are exceeded, a loss of precision should be expected and
     *  the svd algorithm is considered unsuccessful (return value) it still represents
     *  best effort to approximate the true svd.
     *
     *  If matrices fail to be successfully diagonalized, try one of the following
     *  - Increase constant SVD_CYCLE_LIMIT_FACTOR
     *  - Increase constant SVD_DEFECT_LIMIT
     *
     */
    const s3_t max_cycles = n * SVD_CYCLE_LIMIT_FACTOR;

    /** Redress runs an lu-cycle on the current partition (ubd->lbd->ubd) instead of the regular chasing cycle.
     *  I use it to handle a rank deficit (zeroes in the main diagonal).
     *  It produces a normalized representation, where each irreducible (sub-)partition
     *  has maximally one zero diagonal element at the last position.
     */
    bl_t redress = false;

    // Repartitioning determines the current partition based on off-diagonal zero elements
    bl_t repartition = true;

    grt_s gvr_l; // left rotation
    grt_s gvr_r; // right rotation

    s3_t cycle;
    for( cycle = 0; cycle < max_cycles; cycle++ )
    {
        if( redress ) // run redressing cycle
        {
            for( sz_t i = irp_start; i < irp_end - 1; i++ )
            {
                grt_s_init_and_annihilate_b( &gvr_r, d0 + i, d1 + i );
                d1[ i ] = d0[ i + 1 ] * gvr_r.s;
                d0[ i + 1 ] *= gvr_r.c;
                if( v ) grhov[ grho_row * grho_stride + i ] = grho_from_grt( gvr_r.c, gvr_r.s );
            }

            for( sz_t i = irp_start; i < irp_end - 1; i++ )
            {
                grt_s_init_and_annihilate_b( &gvr_l, d0 + i, d1 + i );
                d1[ i ] = d0[ i + 1 ] * gvr_l.s;
                d0[ i + 1 ] *= gvr_l.c;
                if( u ) grhou[ grho_row * grho_stride + i ] = grho_from_grt( gvr_l.c, gvr_l.s );
            }

            redress = false;
            repartition = true;
        }
        else // else (no redress): run chasing cycle
        {
            if( repartition )
            {
                /// Determine topmost irreducible partition and end of iteration
                while( irp_start < n - 1 && d1[ irp_start ] == 0 ) irp_start++;
                if( irp_start >= n - 1 ) break;

                // irp_end: end-index (length=irp_end-irp_start)
                irp_end = irp_start + 2;
                while( irp_end < n && d1[ irp_end - 1 ] != 0 ) irp_end++;

                grho_wstart = sz_min( grho_wstart, irp_start );
                grho_wend   = sz_max( grho_wend, irp_end - 1 );

                repartition = false;
            }

            /** Computing shift
             *  For the current irreducible partition:
             *  We set the shift value to an eigenvalue of the lowest 2x2 Matrix.
             *  Out of the two possible eigenvalues, we choose the one closest to
             *  the last diagonal element.
             */
            f3_t shift = 0;
            {
                f3_t a00 = d0[ irp_end - 2 ];
                f3_t a01 = d1[ irp_end - 2 ];
                f3_t a11 = d0[ irp_end - 1 ];

                f3_t max_abs = f3_max( f3_max( f3_abs( a00 ), f3_abs( a01 ) ), f3_abs( a11 ) );

                /** Set shift to eigenvalue of last 2x2 block-matrix M of AAt which is closest to m11
                 *  The scale factor 'scale' is used to avoid underflows or overflows in the calculation
                 */
                if( max_abs >= f3_lim_min )
                {
                    f3_t scale = 1.0 / max_abs;
                    f3_t m11 = f3_sqr( a11 * scale );
                    f3_t m00 = f3_sqr( a00 * scale ) + f3_sqr( a01 * scale );
                    f3_t m01 = ( a01 * scale ) * ( a11 * scale );

                    f3_t p = 0.5 * ( m00 + m11 );

                    // I use hypot to mitigate overflow and underflow effects.
                    // Underflow is likely when the original matrix is sparse and bidiagonalized via householder reflections.
                    f3_t d = 0.5 * hypot( m00 - m11, 2.0 * m01 );

                    shift = ( ( m11 >= p ) ? p + d : p - d ) * f3_sqr( max_abs );
                }
            }

            bl_t exit_chase = false;

            /// Initial Rotations
            f3_t* dp0 = d0 + irp_start;
            f3_t* dp1 = d1 + irp_start;

            f3_t a00 = dp0[ 0 ];
            f3_t a01 = dp1[ 0 ];
            f3_t a11 = dp0[ 1 ];

            /** The left rotation initiates the QR step on the shifted A*At by eliminating its lower off diagonal element,
             *  where A is the upper 2x2 matrix of the current partition. A subsequent right rotation
             *  re-symmetrizes creating a bulge to be chased down. On A the two rotations are applied in reverse order.
             *  The right rotation creates a lower off-diagonal a10 on A, which is rotated into a02 via left-rotation.
             *  a02 is chased down in subsequent loop.
             */
            grt_s_init_to_annihilate_b( &gvr_l, f3_sqr( a00 ) + f3_sqr( a01 ) - shift, a01 * a11 );

            // if this rotation is zero, then a01 * a11 is zero -> run redress cycle
            if( f3_abs( gvr_l.s ) < SVD_DEFECT_LIMIT )
            {
                gvr_l.s = 0;
                gvr_l.c = 1;

                if( f3_abs( a11 ) < f3_abs( a01 ) )
                {
                    a11 = 0;
                    redress = true;
                }
                else
                {
                    a01 = 0;
                    repartition = true;
                }
                exit_chase = true;
            }

            // right rotation annihilates a1[0]
            grt_s_init_to_annihilate_b( &gvr_r, gvr_l.c * a11 - gvr_l.s * a01, gvr_l.s * a00 );

            f3_t tmp, a10;
            tmp = gvr_r.c * a00 + gvr_r.s * a01;
            a01 = gvr_r.c * a01 - gvr_r.s * a00;
            a10 = gvr_r.s * a11;
            a11 = gvr_r.c * a11;
            a00 = gvr_l.c * tmp + gvr_l.s * a10;
            tmp = gvr_l.c * a01 + gvr_l.s * a11;
            a11 = gvr_l.c * a11 - gvr_l.s * a01;
            a01 = tmp;

            f3_t a02 = 0; // newly created 3rd diagonal element
            f3_t a12 = 0;

            if( n > irp_start + 2 )
            {
                a12 = dp1[ 1 ];
                tmp = gvr_l.c * a02 + gvr_l.s * a12;
                a12 = gvr_l.c * a12 - gvr_l.s * a02;
                a02 = tmp;
                dp1[ 1 ] = a12;
            }

            dp0[ 0 ] = a00;
            dp0[ 1 ] = a11;
            dp1[ 0 ] = a01;

            if( u ) grhou[ grho_row * grho_stride + irp_start ] = grho_from_grt( gvr_l.c, gvr_l.s );
            if( v ) grhov[ grho_row * grho_stride + irp_start ] = grho_from_grt( gvr_r.c, gvr_r.s );

            /// Chasing: Annihilation of non-bidiagonals
            for( sz_t k = irp_start; !exit_chase && k < irp_end - 2; k++ )
            {
                f3_t a22 = dp0[ 2 ];
                f3_t a21 = 0;
                grt_s_init_and_annihilate_b( &gvr_r, &a01, &a02 );

                // if the right rotation is effective zero compared to the previous left rotation, then a02 and a12 are zero requiring repartitioning
                if( f3_abs( gvr_r.s ) < f3_abs( gvr_l.s ) * SVD_DEFECT_LIMIT )
                {
                    gvr_r.s = 0;
                    gvr_r.c = 1;
                    a12 = 0;
                    repartition = true;
                    exit_chase = true;
                }

                grt_s_rotate( &gvr_r, &a11, &a12 );
                grt_s_rotate( &gvr_r, &a21, &a22 );
                grt_s_init_and_annihilate_b( &gvr_l, &a11, &a21 );

                // if the left rotation is effective zero compared to the previous right rotation, then a22 is zero requiring redressing
                if( f3_abs( gvr_l.s ) < f3_abs( gvr_r.s ) * SVD_DEFECT_LIMIT )
                {
                    gvr_l.s = 0;
                    gvr_l.c = 1;
                    a22 = 0;
                    redress = true;
                    exit_chase = true;
                }

                grt_s_rotate( &gvr_l, &a12, &a22 );

                dp0[ 0 ] = a00;
                dp0[ 1 ] = a11;
                dp0[ 2 ] = a22;
                dp1[ 0 ] = a01;
                dp1[ 1 ] = a12;

                f3_t a23 = 0;
                f3_t a13 = 0;
                if( k < irp_end - 3 )
                {
                    a23 = dp1[ 2 ];
                    grt_s_rotate( &gvr_l, &a13, &a23 );
                    dp1[ 2 ] = a23;
                }

                a00 = a11;
                a11 = a22;
                a01 = a12;
                a02 = a13;
                a12 = a23;
                dp0++;
                dp1++;

                if( u ) grhou[ grho_row * grho_stride + k + 1 ] = grho_from_grt( gvr_l.c, gvr_l.s );
                if( v ) grhov[ grho_row * grho_stride + k + 1 ] = grho_from_grt( gvr_r.c, gvr_r.s );
            }

            /// Explicitly test for Lowest sub-diagonal element zero: repartitioning
            if( subd_defector( d0[ irp_end - 2 ], &d1[ irp_end - 2 ], d0[ irp_end - 1 ], SVD_DEFECT_LIMIT ) ) repartition = true;
        }

        // U,V rotations if applicable
        if( u || v )
        {
            grho_row++;
            if( grho_row == grho_rows )
            {
                uv_grho_arow_rotate_and_zero( a_rows, a_cols, u_stride, u, v_stride, v, grho_row, grho_stride, grhou, grhov, grho_wstart, grho_wend );
                grho_wstart = irp_start;
                grho_wend   = irp_end - 1;
                grho_row    = 0;
            }
        }
    }

    if( cycle == max_cycles ) success = false;

    // residual U,V rotations if applicable
    if( grho_row > 0 ) uv_grho_arow_rotate_and_zero( a_rows, a_cols, u_stride, u, v_stride, v, grho_row, grho_stride, grhou, grhov, grho_wstart, grho_wend );

    // Make sure all diagonal values are non-negative
    for( sz_t i = 0; i < n; i++ ) if( d0[i] < 0 ) { d0[i] *= -1.0; if( v ) mul_f3_to_row( v_rows, v_cols, v, v_stride, -1.0, i ); }

    // We make sure the diagonal is fully sorted.
    // At this point the diagonal is already mostly sorted, which is a side effect of the chasing algorithm.
    // Sorting below will remove any residual ordering-glitches, which sparsely occur on large matrices.
    // A simple O(n^2) sorting is sufficient at this point (worst case is not time-critical).
    for( sz_t i = 0; i < n; i++ )
    {
        f3_t max_d = d0[ i ];
        sz_t max_j = i;
        for( sz_t j = i + 1; j < n; j++ ) if( d0[j] > max_d ) { max_d = d0[j]; max_j = j; }
        if( max_j != i )
        {
            f3_t_swap( &d0[i], &d0[max_j] );
            if( u ) swap_rows( u_rows, u_cols, u, u_stride, i, max_j );
            if( v ) swap_rows( v_rows, v_cols, v, v_stride, i, max_j );
        }
    }

    // Copy diagonal data and set all other elements zero
    for( sz_t i = 0; i < n; i++ ) a[ i * a_stride + i ] = d0[ i ];

    for( sz_t i = 0; i < a_rows; i++ )
    {
        for( sz_t j = 0; j < a_cols; j++ )
        {
            if( i != j ) a[ i * a_stride + j ] = 0;
        }
    }

    return success ? 0 : MOCUT_WRN_CONVERGENCE;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
