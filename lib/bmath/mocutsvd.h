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

#ifndef MOCUTSVD_H
#define MOCUTSVD_H

#ifdef __cplusplus
   extern "C" {
#endif // __cplusplus

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

/**********************************************************************************************************************/
/// Error and warning codes

//----------------------------------------------------------------------------------------------------------------------

enum
{
    MOCUT_NO_ERR = 0,
    MOCUT_WRN_CONVERGENCE, // svd failed to converge
    MOCUT_ERR_STRIDE,      // incorrect stride value (e.g. stride < cols)
    MOCUT_ERR_ALLOC,        // memory allocation failed
    MOCUT_ERR_ROW_OUT_OF_RANGE, // row-index out of range
    MOCUT_ERR_COL_OUT_OF_RANGE, // column-index out of range
    MOCUT_ERR_U_ROWS,     // U has the wrong number of rows
    MOCUT_ERR_U_COLS,     // U has the wrong number of columns
    MOCUT_ERR_V_ROWS,     // V has the wrong number of rows
    MOCUT_ERR_V_COLS,     // V has the wrong number of columns
    MOCUT_ERR_UNKNOWN_DECOMPOSITION_TYPE,
    MOCUT_ERR_ROWS_MISMATCH,
    MOCUT_ERR_COLS_MISMATCH,
    MOCUT_ERR_MAT_CONTAINS_NAN,
    MOCUT_ERR_SAME_MATRIX,
};

static inline const char* mocut_err_text( int err_code )
{
    switch( err_code )
    {
        case MOCUT_NO_ERR:               return "No error.";
        case MOCUT_WRN_CONVERGENCE:      return "No convergence.";
        case MOCUT_ERR_STRIDE:           return "Incorrect stride value (e.g. stride < cols).";
        case MOCUT_ERR_ALLOC:            return "Memory allocation failed.";
        case MOCUT_ERR_ROW_OUT_OF_RANGE: return "Row-index out of range.";
        case MOCUT_ERR_COL_OUT_OF_RANGE: return "Column-index out of range.";
        case MOCUT_ERR_U_ROWS:           return "Matrix U has incorrect number of rows.";
        case MOCUT_ERR_U_COLS:           return "Matrix U has incorrect number of columns.";
        case MOCUT_ERR_V_ROWS:           return "Matrix V has incorrect number of rows.";
        case MOCUT_ERR_V_COLS:           return "Matrix V has incorrect number of columns.";
        case MOCUT_ERR_UNKNOWN_DECOMPOSITION_TYPE:
                                         return "Unknown decomposition type.";
        case MOCUT_ERR_ROWS_MISMATCH:    return "Matrix has the wrong number of rows.";
        case MOCUT_ERR_COLS_MISMATCH:    return "Matrix has the wrong number of columns.";
        case MOCUT_ERR_MAT_CONTAINS_NAN: return "Matrix contains one or more NAN values.";

        case MOCUT_ERR_SAME_MATRIX:      return "Two or more arguments reference the same matrix. Different matrices were expected.";

        default: break;
    }

    return "Invalid error code.";
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/** Heap memory management.
 *  Heap memory management affects matrix allocation and clearing functions. By default these functions allocate
 *  the matrix data with proper row-alignment for maximum speed on most platforms.
 *  You can override this behavior as follows:
 *
 *  If you wish to use you own external memory, use mocut_mat_s_setup instead of mocut_mat_s_alloc.
 *
 *  If you wish to use customize heap memory functions, before including this header #define MOCUT_MEM_ALLOC( size )
 *  and #define MOCUT_MEM_FREE( data ).
 *  Example:
 *  #define MOCUT_MEM_ALLOC( size ) malloc( size )
 *  #define MOCUT_MEM_FREE( data ) free( data )
 *
 *  If heap memory functions are not available on your platform an the compiler generates an error,
 *  before including this header #define MOCUT_NO_MEM_ALLOC.
 *  In that case function mocut_mat_s_alloc generates an error.
 *
 *  If you wish to customize the data alignment, before including this header #define MOCUT_VAL_ALIGN with your own preferred value.
 *  Example:
 *  #define MOCUT_VAL_ALIGN 32
 */

#ifdef MOCUT_NO_MEM_ALLOC
    #define MOCUT_MEM_ALLOC( size ) NULL
    #define MOCUT_MEM_FREE( data )
#endif

#ifndef MOCUT_MEM_ALLOC
    #include <stdlib.h>
#endif

#ifndef MOCUT_VAL_ALIGN
    #define MOCUT_VAL_ALIGN 32
#endif

#ifndef MOCUT_MEM_ALIGN
    #define MOCUT_MEM_ALIGN (MOCUT_VAL_ALIGN * sizeof(double))
#endif

#ifndef MOCUT_MEM_ALLOC
    #define MOCUT_MEM_ALLOC( size ) aligned_alloc( MOCUT_MEM_ALIGN, size )
#endif

#ifndef MOCUT_MEM_FREE
    #define MOCUT_MEM_FREE( data ) free( data )
#endif

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/** Matrix Interface
 *  The matrix has a strided row-major data layout.
 */

//----------------------------------------------------------------------------------------------------------------------

typedef struct
{
    size_t rows;   // matrix-rows
    size_t cols;   // matrix-columns
    size_t stride; // stride between adjacent rows
    size_t space;  // allocated size (0 in case matrix references external data)
    double* data;  // matrix data
} mocut_mat_s;

//----------------------------------------------------------------------------------------------------------------------

static inline void mocut_mat_s_init( mocut_mat_s* o )
{
    o->rows   = 0;
    o->cols   = 0;
    o->stride = 0;
    o->space  = 0;
    o->data   = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

static inline mocut_mat_s* mocut_mat_s_create()
{
    mocut_mat_s* m = MOCUT_MEM_ALLOC( sizeof( mocut_mat_s ) );
    if( m ) mocut_mat_s_init( m );
    return m;
}

//----------------------------------------------------------------------------------------------------------------------

static inline void mocut_mat_s_clear( mocut_mat_s* o )
{
    if( o->data != NULL && o->space > 0 ) MOCUT_MEM_FREE( o->data );
    o->rows   = 0;
    o->cols   = 0;
    o->stride = 0;
    o->space  = 0;
    o->data   = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

static inline int mocut_mat_s_setup( mocut_mat_s* o, size_t rows, size_t cols, size_t stride, double* data )
{
    mocut_mat_s_clear( o );
    if( stride < cols ) return MOCUT_ERR_STRIDE;
    o->rows   = rows;
    o->cols   = cols;
    o->stride = stride;
    o->space  = 0;
    o->data   = data;
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

static inline int mocut_mat_s_init_setup( mocut_mat_s* o, size_t rows, size_t cols, size_t stride, double* data )
{
    mocut_mat_s_init( o );
    return mocut_mat_s_setup( o, rows, cols, stride, data );
}

//----------------------------------------------------------------------------------------------------------------------

/// computes the aligned row-stride from matrix cols
static inline size_t mocut_mat_s_aligned_stride( size_t cols )
{
    size_t align = MOCUT_MEM_ALIGN / sizeof( double );
    return ( ( cols % align ) == 0 ) ? cols : ( cols + align - ( cols % align ) );
}

//----------------------------------------------------------------------------------------------------------------------

static inline void mocut_mat_s_set_zero( mocut_mat_s* o )
{
    for( size_t i = 0; i < o->rows; i++ )
        for( size_t j = 0; j < o->cols; j++ ) o->data[ i * o->stride + j ] = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// allocates and initializes values with zero
static inline int mocut_mat_s_alloc( mocut_mat_s* o, size_t rows, size_t cols )
{
    mocut_mat_s_clear( o );
    size_t stride = mocut_mat_s_aligned_stride( cols );
    size_t size = rows * stride;
    if( size == 0 ) return 0;
    double* data = MOCUT_MEM_ALLOC( size * sizeof( double ) );
    if( data == 0 ) return MOCUT_ERR_ALLOC;
    o->rows   = rows;
    o->cols   = cols;
    o->stride = stride;
    o->space  = size;
    o->data   = data;
    mocut_mat_s_set_zero( o );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

static inline int mocut_mat_s_init_alloc( mocut_mat_s* o, size_t rows, size_t cols )
{
    mocut_mat_s_init( o );
    return mocut_mat_s_alloc( o, rows, cols );
}

//----------------------------------------------------------------------------------------------------------------------

/// returns NULL in case allocation fails
static inline mocut_mat_s* mocut_mat_s_create_alloc( size_t rows, size_t cols )
{
    mocut_mat_s* o = mocut_mat_s_create();
    if( !o ) return NULL;
    if( mocut_mat_s_alloc( o, rows, cols ) ) return NULL;
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

/// returns NULL in case allocation fails
static inline mocut_mat_s* mocut_mat_s_create_setup( size_t rows, size_t cols, size_t stride, double* data )
{
    mocut_mat_s* o = mocut_mat_s_create();
    if( !o ) return NULL;
    if( mocut_mat_s_setup( o, rows, cols, stride, data ) ) return NULL;
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

static inline void mocut_mat_s_down( mocut_mat_s* o )
{
    mocut_mat_s_clear( o );
}

//----------------------------------------------------------------------------------------------------------------------

static inline void mocut_mat_s_discard( mocut_mat_s* o )
{
    if( !o ) return;
    mocut_mat_s_down( o );
    MOCUT_MEM_FREE( o );
}

//----------------------------------------------------------------------------------------------------------------------

static inline double* mocut_mat_s_ptr( const mocut_mat_s* o, size_t row, size_t col ) { return o->data + row * o->stride + col; }

//----------------------------------------------------------------------------------------------------------------------

static inline double mocut_mat_s_get( const mocut_mat_s* o, size_t row, size_t col )
{
    if( row >= o->rows ) return 0;
    if( col >= o->cols ) return 0;
    return o->data[ row * o->stride + col ];
}

//----------------------------------------------------------------------------------------------------------------------

static inline int mocut_mat_s_set( const mocut_mat_s* o, size_t row, size_t col, double value )
{
    if( row >= o->rows ) return MOCUT_ERR_ROW_OUT_OF_RANGE;
    if( col >= o->cols ) return MOCUT_ERR_COL_OUT_OF_RANGE;
    o->data[ row * o->stride + col ] = value;
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// copies data of matrix m to matrix o; o must be properly allocated
static inline int mocut_mat_s_copy( mocut_mat_s* o, const mocut_mat_s* m )
{
    if( o == m ) return 0;
    if( o->rows != m->rows ) return MOCUT_ERR_ROWS_MISMATCH;
    if( o->cols != m->cols ) return MOCUT_ERR_COLS_MISMATCH;
    for( size_t i = 0; i < o->rows; i++ )
        for( size_t j = 0; j < o->cols; j++ )
            o->data[ i * o->stride + j ] = m->data[ i * m->stride + j ];

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// copies transposed data of matrix m to matrix o; o must have the transposed size of m
static inline int mocut_mat_s_copy_transposed( mocut_mat_s* o, const mocut_mat_s* m )
{
    // in place transposition allowed for square matrix
    if( o == m )
    {
        if( !o ) return 0;
        if( o->rows == o->cols )
        {
            for( size_t i = 0; i < o->rows; i++ )
            {
                for( size_t j = 0; j < i; j++ )
                {
                    double t = o->data[ i * o->stride + j ];
                    o->data[ i * o->stride + j ] = o->data[ j * o->stride + i ];
                    o->data[ j * o->stride + i ] = t;
                }

            }
        }
        else
        {
            return MOCUT_ERR_SAME_MATRIX;
        }
    }
    else
    {
        if( o->cols != m->rows ) return MOCUT_ERR_ROWS_MISMATCH;
        if( o->rows != m->cols ) return MOCUT_ERR_COLS_MISMATCH;

        for( size_t i = 0; i < o->rows; i++ )
            for( size_t j = 0; j < o->cols; j++ )
                o->data[ i * o->stride + j ] = m->data[ j * m->stride + i ];
    }


    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

/// tests if matrix contains a NAN value
static inline int mocut_mat_s_contains_nan( const mocut_mat_s* o )
{
    for( size_t i = 0; i < o->rows; i++ )
    {
        for( size_t j = 0; j < o->cols; j++ )
        {
            double v = o->data[ i * o->stride + j ];
            if( v != v ) return 1;
        }
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Singular value decomposition on (m x n) matrix M.
 *
 *  This function performs the thin SVD: M -> U*, D, V*.
 *
 *  With k := min(m,n):
 *  - U*, V* are matrices of k singular row-vectors. (U* = U transposed)
 *  - D is the corresponding vector of k singular values.
 *  - All singular values are non-negative and sorted from largest to smallest.
 *  - Matrices are represented in strided row-major format.
 *
 *  The singular value decomposition satisfies the equation M = U·Σ·V*
 *  where Σ is the diagonal matrix of singular values.
 *
 *  Arguments
 *    a_rows: n = Rows    of matrix M, A
 *    a_cols: m = Columns of matrix M, A
 *
 *    a       : Pointer to data buffer for matrix A
 *    a_stride: row-stride of matrix A
 *       - before execution matrix A is equal to M
 *       - after execution a represents a vector of k singular values D
 *
 *    u       : Pointer to data buffer for matrix U; set NULL if U is not wanted
 *    u_stride: row-stride of matrix U; set 0 if U is not wanted
 *
 *    v       : Pointer to data buffer for matrix V; set NULL if V is not wanted
 *    v_stride: row-stride of matrix V; set 0 if V is not wanted
 *
 *  Return value:
 *    0                   : successful decomposition
 *   MOCUT_WRN_CONVERGENCE: convergence failure (This happens very rarely and D, U*, V* may still resemble the correct result)
 *
 */
int mocut_thin_svd( size_t a_rows, size_t a_cols, double* restrict a, size_t a_stride, double* restrict u, size_t u_stride, double* restrict v, size_t v_stride );

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Transforms (rows x cols) matrix A to lower or upper diagonal form.
 *  Resulting A is upper diagonal when n>=m and lower diagonal when n<m.
 *  bnd: Resulting A is band-diagonal (band size according to constant BAND_SIZE)
 *  bid: Resulting A is bi-diagonal
 *  Performs a thin decomposition.
 *  Matrices U, V are transposed.
 */
void mocut_thin_bnd( size_t a_rows, size_t a_cols, double* restrict a, size_t a_stride, double* restrict u, size_t u_stride, double* restrict v, size_t v_stride );
void mocut_thin_bid( size_t a_rows, size_t a_cols, double* restrict a, size_t a_stride, double* restrict u, size_t u_stride, double* restrict v, size_t v_stride );

//----------------------------------------------------------------------------------------------------------------------

enum
{
    MOCUT_SVD = 0, // singular value decomposition
    MOCUT_BID,     // bi-diagonal decomposition
    MOCUT_BND      // band-diagonal decomposition
};

/** Thin Decomposition on (m x n) matrix M:
 *
 *  This function performs the thin decomposition: M -> U*, D, V*.
 *  The decomposition satisfies the equation M = U·D·V*
 *  SVD: M = U·Σ·V*, where Σ is the diagonal matrix of singular values.
 *
 *  With k := min(m,n):
 *  - U*, V* are matrices of k singular row-vectors. (U* = U transposed)
 *  - D is the specified diagonal form (SVD: diagonal, BID: bi-diagonal, BND: band-diagonal)
 *  - SVD: All singular values are non-negative and sorted from largest (upper-left) to smallest (lower-right).
 *  - Matrices are represented in strided row-major format.
 *
 *
 *  Arguments
 *    a_rows: n = Rows    of matrix M, A
 *    a_cols: m = Columns of matrix M, A
 *
 *    a : Matrix A
 *       - before execution matrix A is equal to M
 *       - after execution matrix A is equal to Σ: Singular values on diagonal; all other elements are zero.
 *
 *    u : Matrix U* or NULL
 *    v : Matrix V* or NULL
 *    decomposition_type: type of decomposition: MOCUT_SVD | MOCUT_BID | MOCUT_BND
 *
 *  Return value:
 *   0                    : successful decomposition
 *   MOCUT_WRN_CONVERGENCE: convergence failure (This happens very rarely and D, U*, V* may still resemble the correct result)
 *   MOCUT_ERR_U_ROWS     : U is pre-allocated but has the wrong number of rows
 *   MOCUT_ERR_U_COLS     : U is pre-allocated but has the wrong number of columns
 *   MOCUT_ERR_V_ROWS     : V is pre-allocated but has the wrong number of rows
 *   MOCUT_ERR_V_COLS     : V is pre-allocated but has the wrong number of columns
 *
 */
static inline int mocut_decompose( mocut_mat_s* restrict a, mocut_mat_s* restrict u, mocut_mat_s* restrict v, int decomposition_type )
{
    if( a == u ) return MOCUT_ERR_SAME_MATRIX;
    if( a == v ) return MOCUT_ERR_SAME_MATRIX;

    if( mocut_mat_s_contains_nan( a ) ) return MOCUT_ERR_MAT_CONTAINS_NAN;

    size_t n = ( a->rows < a->cols ) ? a->rows : a->cols;
    if( n == 0 ) return 0;

    int ret = 0;

    double* u_data = NULL;
    size_t u_stride = 0;
    double* v_data = NULL;
    size_t v_stride = 0;

    if( u )
    {
        if( u == v ) return MOCUT_ERR_SAME_MATRIX;

        if( u->data )
        {
            if( u->rows != n       ) return MOCUT_ERR_U_ROWS;
            if( u->cols != a->rows ) return MOCUT_ERR_U_COLS;
        }
        else
        {
            if( ( ret = mocut_mat_s_alloc( u, n, a->rows ) ) ) return ret;
        }

        u_data = u->data;
        u_stride = u->stride;
    }

    if( v )
    {
        if( v->data )
        {
            if( v->rows != n       ) return MOCUT_ERR_V_ROWS;
            if( v->cols != a->cols ) return MOCUT_ERR_V_COLS;
        }
        else
        {
            if( ( ret = mocut_mat_s_alloc( v, n, a->cols ) ) ) return ret;
        }

        v_data = v->data;
        v_stride = v->stride;
    }

    switch( decomposition_type )
    {
        case MOCUT_SVD: ret = mocut_thin_svd( a->rows, a->cols, a->data, a->stride, u_data, u_stride, v_data, v_stride ); break;
        case MOCUT_BID:       mocut_thin_bid( a->rows, a->cols, a->data, a->stride, u_data, u_stride, v_data, v_stride ); break;
        case MOCUT_BND:       mocut_thin_bnd( a->rows, a->cols, a->data, a->stride, u_data, u_stride, v_data, v_stride ); break;
        default: return MOCUT_ERR_UNKNOWN_DECOMPOSITION_TYPE;
    }

    return ret;
}

//----------------------------------------------------------------------------------------------------------------------

static inline int mocut_svd( mocut_mat_s* restrict a, mocut_mat_s* restrict u, mocut_mat_s* restrict v ) { return mocut_decompose( a, u, v, MOCUT_SVD ); }
static inline int mocut_bid( mocut_mat_s* restrict a, mocut_mat_s* restrict u, mocut_mat_s* restrict v ) { return mocut_decompose( a, u, v, MOCUT_BID ); }
static inline int mocut_bnd( mocut_mat_s* restrict a, mocut_mat_s* restrict u, mocut_mat_s* restrict v ) { return mocut_decompose( a, u, v, MOCUT_BND ); }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#ifdef __cplusplus
   }
#endif // __cplusplus

#endif  // MOCUTSVD_H
