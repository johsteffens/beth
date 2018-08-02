/** Copyright 2017 Johannes Bernhard Steffens
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

#ifndef BCORE_CONTROL_H
#define BCORE_CONTROL_H

#include <stdarg.h>
#include <time.h>

/// if this include causes an ERROR, try disable it (dependencies, if any, should not be critical)
#include <sys/time.h>

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_features.h"

/**
 * Collection of low-level functions and macros.
 */

/**********************************************************************************************************************/
/// basic messaging

/// formatted message to stdout
void bcore_msg   ( sc_t format, ... );
void bcore_msg_fa( sc_t format, ... ); // beth format

/// formatted message + newline to stderr
void bcore_wrnv  ( sc_t format, va_list args );
void bcore_wrn   ( sc_t format, ... );
void bcore_wrn_fv( sc_t format, va_list args ); // beth format (avoid on low-level errors)
void bcore_wrn_fa( sc_t format, ... );          // beth format (avoid on low-level errors)

/// extended formatted message + newline to stderr
void bcore_ext_wrn   ( sc_t f_name, sc_t file, int line, sc_t format, ... );
void bcore_ext_wrn_fa( sc_t f_name, sc_t file, int line, sc_t format, ... ); // beth format (avoid on low-level errors)

/// formatted message + newline to stderr and abort
void bcore_errv  ( sc_t format, va_list args );
void bcore_err   ( sc_t format, ... );
void bcore_err_fv( sc_t format, va_list args ); // beth format (avoid on low-level errors)
void bcore_err_fa( sc_t format, ... );          // beth format (avoid on low-level errors)

/// extended formatted message + newline to stderr and abort
void bcore_ext_err   ( sc_t f_name, sc_t file, int line, sc_t format, ... );
void bcore_ext_err_fa( sc_t f_name, sc_t file, int line, sc_t format, ... ); // beth format (avoid on low-level errors)

/**********************************************************************************************************************/
/** Memory Management
 *  Most functions wrap a corresponding function in bcore_tbman.h.
 *  See bcore_tbman.h for implementation and usage details
 */

/// buf == NULL: malloc; buf != NULL && size == 0: free; buf != NULL && size != 0: realloc; returns a valid address or NULL; alloc failure produces error
vd_t bcore_alloc  ( vd_t ptr, uz_t size );
vd_t bcore_malloc (           uz_t size );
vd_t bcore_realloc( vd_t ptr, uz_t size );
vd_t bcore_free   ( vd_t ptr            );

vd_t bcore_b_alloc(  vd_t current_ptr,                     uz_t requested_bytes, uz_t* granted_bytes );
vd_t bcore_bn_alloc( vd_t current_ptr, uz_t current_bytes, uz_t requested_bytes, uz_t* granted_bytes );

vd_t bcore_u_alloc(  uz_t unit_bytes, vd_t current_ptr,                     uz_t requested_units, uz_t* granted_units );
vd_t bcore_un_alloc( uz_t unit_bytes, vd_t current_ptr, uz_t current_units, uz_t requested_units, uz_t* granted_units );

/**********************************************************************************************************************/
/** Reference Management
 *  Most functions wrap a corresponding function in bcore_tbman.h.
 *  See bcore_tbman.h for implementation and usage details
 */

uz_t bcore_references     (                      vc_t ptr );
vd_t bcore_fork           (                      vd_t ptr );
void bcore_release        (                      vd_t ptr );
void bcore_release_obj    ( fp_t down,           vd_t ptr );
void bcore_release_arg    ( fp_t down, vc_t arg, vd_t ptr );
void bcore_release_obj_arr( fp_t down,           vd_t ptr, uz_t size, uz_t step );
void bcore_release_arg_arr( fp_t down, vc_t arg, vd_t ptr, uz_t size, uz_t step );

/**********************************************************************************************************************/
/// memory set, copy, move

/// sets memory to zero. When dst == NULL function allocates destination first, copies and returns address.
vd_t bcore_memzero(                    vd_t dst, uz_t size );
vd_t bcore_u_memzero( uz_t unit_bytes, vd_t dst, uz_t size );

/// sets memory to val. When dst == NULL function allocates destination first, copies and returns address.
vd_t bcore_memset(  vd_t dst, u0_t val, uz_t size );

/// like stdlib memcpy but when dst == NULL function allocates destination first, copies and returns address; size == 0 allowed
vd_t bcore_memcpy(                    vd_t dst, vc_t src, uz_t size );
vd_t bcore_u_memcpy( uz_t unit_bytes, vd_t dst, vc_t src, uz_t size ); // copies multiple of a given unit size

/// like stdlib memmove but when dst == NULL function allocates destination first, copies and returns address; size == 0 allowed
vd_t bcore_memmove( vd_t dst, vc_t src, uz_t size );

/**********************************************************************************************************************/
/// 0-terminated string

/// like strcpy but when dst == NULL, function allocates destination first, copies and returns address
sd_t bcore_strcpy( sd_t dst, sc_t src );

/** Compatible to strcmp but also accepts NULL and provides more distinctive results
 *  returns   0 when both strings are equal
 *  returns  +1 when src2 is longer than str 1 but at the length of str1 all characters match
 *  returns  -1 when src1 is longer than str 2 but at the length of str2 all characters match
 *  returns >+1 when none of above conditions apply and str2 comes after str1 in lexicographic order
 *  returns <-1 when none of above conditions apply and str1 comes after str2 in lexicographic order
 *  NULL is ordered before the empty string.
 *
 * Examples:
 *   if str1 is equal to str2 ...............: if( bcore_strcmp( str1, str2 ) == 0 )
 *   if str1 is ordered before str2 .........: if( bcore_strcmp( str1, str2 ) >  0 )
 *   if str2 is ordered before str1 .........: if( bcore_strcmp( str1, str2 ) <  0 )
 *   if str1 matches the beginning of str2 ..: if( (  bcore_strcmp( str1, str2 ) >> 1 ) == 0 )
 *   if str2 matches the beginning of str1 ..: if( ( -bcore_strcmp( str1, str2 ) >> 1 ) == 0 )
 *
 */
s2_t bcore_strcmp( sc_t str1, sc_t str2 );
s2_t bcore_strcmp_n( sc_t str1, uz_t n1, sc_t str2, uz_t n2 );

/// like strlen but also accepts NULL
uz_t bcore_strlen( sc_t str );

/// returns true iff c occurs in str
bl_t bcore_strany( char c, sc_t str );

/**********************************************************************************************************************/
/// Pseudo random generators (non-cryptographic)

/** Linear Congruential Generators.
 *  Generators below have a complete (2^n) period; (I did not fully evaluate the quality of randomness.)
 *  As for all LCG, the period length of lower order bits is maximally 2^bit-order. Hence higher order bits
 *  are more suitable for randomization than lower order bits.
 */
static inline u2_t bcore_lcg_u2(  u2_t val ) { return val *  67261 + 1385791711; } // period verified
static inline u2_t bcore_lcg1_u2( u2_t val ) { return val * 122497 + 1283800139; } // period verified
static inline u2_t bcore_lcg2_u2( u2_t val ) { return val * 309313 + 1427550431; } // period verified
static inline u2_t bcore_lcg3_u2( u2_t val ) { return val *  76157 + 1175399809; } // period verified

/** Xor Shift Generators.
 *  Generators below should have a period of 2^n - 1 with 0 being the fix-point; (-> 0 cannot be used as as seed)
 *  They belong to the family of xorshift generators discovered by George Marsaglia (http://www.jstatsoft.org/v08/i14/paper)
 *  These generators exhibit better randomness than LCG but require about 50% more CPU time.
 *
 *  Note: xsg, xsg2 produce structured distributions in 3d polar coordinates.
 */
static inline u2_t bcore_xsg_u2(  u2_t rval ) { rval ^= ( rval >>  7 ); rval ^= ( rval << 25 ); return rval ^ ( rval >> 12 ); } // period verified
static inline u2_t bcore_xsg1_u2( u2_t rval ) { rval ^= ( rval >> 11 ); rval ^= ( rval << 21 ); return rval ^ ( rval >> 13 ); } // period verified
static inline u2_t bcore_xsg2_u2( u2_t rval ) { rval ^= ( rval >>  5 ); rval ^= ( rval << 27 ); return rval ^ ( rval >>  8 ); } // period verified
static inline u2_t bcore_xsg3_u2( u2_t rval ) { rval ^= ( rval >> 17 ); rval ^= ( rval << 15 ); return rval ^ ( rval >> 23 ); } // period verified

/**********************************************************************************************************************/
/// Hashing (non-cryptographic)

/** FNV-1a for 0-terminated strings and data blocks.
 *  Conceived by Glenn Fowler, Landon Curt Noll and Kiem-Phon Vo in 1991.
 *  References:
 *  http://www.isthe.com/chongo/tech/comp/fnv/index.html
 *  http://en.wikipedia.org/wiki/Fowler_Noll_Vo_hash
 */
#define FNV_U2_INIT 0x811c9dc5u
#define FNV_U2_FOLD 0x01000193u

static inline u2_t bcore_fnv_fold_text_u2( u2_t hash, sc_t text )
{
    while( *text ) hash = ( hash ^ ( u0_t )( *text++ ) ) * FNV_U2_FOLD;
    return hash;
}

static inline u2_t bcore_fnv_fold_u2_u2( u2_t hash, u2_t val )
{
    hash = ( hash ^ ( ( val       ) & 0x0FF ) ) * FNV_U2_FOLD;
    hash = ( hash ^ ( ( val >>  8 ) & 0x0FF ) ) * FNV_U2_FOLD;
    hash = ( hash ^ ( ( val >> 16 ) & 0x0FF ) ) * FNV_U2_FOLD;
    hash = ( hash ^ ( ( val >> 24 ) & 0x0FF ) ) * FNV_U2_FOLD;
    return hash;
}
static inline u2_t bcore_fnv_fold_data_u2( u2_t hash, vc_t data, uz_t size )
{
    const u0_t* src = data;
    for( uz_t i = 0; i < size; i++ ) hash = ( hash ^ src[ i ] ) * FNV_U2_FOLD;
    return hash;
}

static inline u2_t bcore_fnv_hash_text_u2( sc_t text            ) { return bcore_fnv_fold_text_u2( FNV_U2_INIT, text );       }
static inline u2_t bcore_fnv_hash_u2_u2(   u2_t val             ) { return bcore_fnv_fold_u2_u2(   FNV_U2_INIT, val );        }
static inline u2_t bcore_fnv_hash_data_u2( vc_t data, uz_t size ) { return bcore_fnv_fold_data_u2( FNV_U2_INIT, data, size ); }

/**********************************************************************************************************************/

vd_t bcore_control_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/
/// Macros

/// warning message
#define WRN( ... ) bcore_ext_wrn( __func__, __FILE__, __LINE__, __VA_ARGS__ )
#define WRN_fa( ... ) bcore_ext_wrn_fa( __func__, __FILE__, __LINE__, __VA_ARGS__ )

/// error message
#define ERR( ... ) bcore_ext_err( __func__, __FILE__, __LINE__, __VA_ARGS__ )
#define ERR_fa( ... ) bcore_ext_err_fa( __func__, __FILE__, __LINE__, __VA_ARGS__ )

/// same purpose as assert() but cannot be switched off via NDEBUG; typically used in selftests
#define ASSERT( condition ) if( !(condition) ) bcore_err( "assertion '%s' failed in function %s (%s line %i)\n", #condition, __func__, __FILE__, __LINE__ )

/**********************************************************************************************************************/

// cpu time assessment
#define CPU_TIME_OF( expression, time_var ) \
    f3_t __time_diff_sec = 0; \
    { \
        clock_t time = clock(); \
        expression; \
        __time_diff_sec = clock() - time; \
        __time_diff_sec /= CLOCKS_PER_SEC; \
    } \
    time_var = __time_diff_sec;

//  gettimeofday is not standardized on all platforms
#define ABS_TIME_OF( expression, time_var ) \
    f3_t __time_diff_sec = 0; \
    { \
        struct timeval t0, t1; \
        gettimeofday( &t0, NULL ); \
        expression; \
        gettimeofday( &t1, NULL ); \
        __time_diff_sec = t1.tv_sec - t0.tv_sec; \
        __time_diff_sec += ( t1.tv_usec - t0.tv_usec ) * 1E-6; \
    } \
    time_var = __time_diff_sec;

#define CPU_TIME_TO_STDOUT( expression ) \
{ \
    CPU_TIME_OF( expression, f3_t __time_sec ) \
    if( __time_sec >= 100 ) \
    { \
        bcore_msg_fa( "#pl5 {#<uz_t>}s : "#expression"\n", ( uz_t ) __time_sec ); \
    } \
    else \
    { \
        bcore_msg_fa( "#pl5 {#<uz_t>}ms: "#expression"\n", ( uz_t ) ( 1E3 * __time_sec ) ); \
    } \
}

#define ABS_TIME_TO_STDOUT( expression ) \
{ \
    ABS_TIME_OF( expression, f3_t __time_sec ) \
    if( __time_sec >= 100 ) \
    { \
        bcore_msg_fa( "#pl5 {#<uz_t>}s : "#expression"\n", ( uz_t ) __time_sec ); \
    } \
    else \
    { \
        bcore_msg_fa( "#pl5 {#<uz_t>}ms: "#expression"\n", ( uz_t ) ( 1E3 * __time_sec ) ); \
    } \
}

#define CPU_TIME_TO_STRING( expression, string ) \
{ \
    CPU_TIME_OF( expression, f3_t __time_sec ) \
    if( __time_sec >= 100 ) \
    { \
        if( string ) st_s_push_fa( string, "#pl5 {#<uz_t>}s : "#expression"\n", ( uz_t ) __time_sec ); \
    } \
    else \
    { \
        if( string ) st_s_push_fa( string, "#pl5 {#<uz_t>}ms: "#expression"\n", ( uz_t ) ( 1E3 * __time_sec ) ); \
    } \
}

#define ABS_TIME_TO_STRING( expression, string ) \
{ \
    ABS_TIME_OF( expression, f3_t __time_sec ) \
    if( __time_sec >= 100 ) \
    { \
        if( string ) st_s_push_fa( string, "#pl5 {#<uz_t>}s : "#expression"\n", ( uz_t ) __time_sec ); \
    } \
    else \
    { \
        if( string ) st_s_push_fa( string, "#pl5 {#<uz_t>}ms: "#expression"\n", ( uz_t ) ( 1E3 * __time_sec ) ); \
    } \
}

/**********************************************************************************************************************/
/// object related functions
#define BCORE_DECLARE_FUNCTION_INIT( name )    void name##_init( name* o );
#define BCORE_DECLARE_FUNCTION_DOWN( name )    void name##_down( name* o );
#define BCORE_DECLARE_FUNCTION_COPY( name )    void name##_copy( name* o, const name* src );
#define BCORE_DECLARE_FUNCTION_CREATE( name ) name* name##_create();
#define BCORE_DECLARE_FUNCTION_DISCARD( name ) void name##_discard( name* o );
#define BCORE_DECLARE_FUNCTION_CLONE( name )  name* name##_clone( const name* o );

#define BCORE_DECLARE_FUNCTIONS_OBJ( name )\
    BCORE_DECLARE_FUNCTION_INIT( name ) \
    BCORE_DECLARE_FUNCTION_DOWN( name ) \
    BCORE_DECLARE_FUNCTION_COPY( name ) \
    BCORE_DECLARE_FUNCTION_CREATE( name ) \
    BCORE_DECLARE_FUNCTION_DISCARD( name ) \
    BCORE_DECLARE_FUNCTION_CLONE( name )

#define BCORE_DEFINE_FUNCTION_INIT_FLAT( name ) \
void name##_init( name* o ) \
{ \
    bcore_memzero( o, sizeof( name ) ); \
}

#define BCORE_DEFINE_FUNCTION_DOWN_FLAT( name ) \
void name##_down( name* o ) {}

#define BCORE_DEFINE_FUNCTION_COPY_FLAT( name ) \
void name##_copy( name* o, const name* src ) \
{ \
    if( o == src ) return; \
    bcore_memcpy( o, src, sizeof( name ) ); \
}

#define BCORE_DEFINE_FUNCTIONS_IDC_FLAT( name )\
    BCORE_DEFINE_FUNCTION_INIT_FLAT( name )\
    BCORE_DEFINE_FUNCTION_DOWN_FLAT( name )\
    BCORE_DEFINE_FUNCTION_COPY_FLAT( name )\

#define BCORE_DEFINE_FUNCTION_MOVE( name ) \
void name##_move( name* o, name* src ) \
{ \
    if( o == src ) return; \
    name##_init( o ); \
    name##_copy( o, src ); \
    name##_down( src ); \
}

#define BCORE_DEFINE_FUNCTION_CREATE( name ) \
name* name##_create() \
{ \
    name* o = bcore_alloc( NULL, sizeof( name ) ); \
    name##_init( o ); \
    return o; \
}

#define BCORE_DEFINE_FUNCTION_DISCARD( name ) \
void name##_discard( name* o ) \
{ \
    if( !o ) return; \
    bcore_release_obj( (fp_t)name##_down, o );\
}

#define BCORE_DEFINE_FUNCTION_CLONE( name ) \
name* name##_clone( const name* o ) \
{ \
    if( !o ) return NULL; \
    name* o_l = name##_create(); \
    name##_copy( o_l, o ); \
    return o_l; \
}

#define BCORE_DEFINE_FUNCTIONS_CDC( name )\
    BCORE_DEFINE_FUNCTION_CREATE( name )\
    BCORE_DEFINE_FUNCTION_DISCARD( name )\
    BCORE_DEFINE_FUNCTION_CLONE( name )\

#define BCORE_DEFINE_FUNCTIONS_OBJ_FLAT( name )\
    BCORE_DEFINE_FUNCTIONS_IDC_FLAT( name )\
    BCORE_DEFINE_FUNCTIONS_CDC( name )

#define BCORE_DECLARE_OBJECT( name )\
    typedef struct name name; \
    BCORE_DECLARE_FUNCTIONS_OBJ( name ) \
    struct name \

// Body definition only
#define BCORE_DECLARE_OBJECT_BODY( name ) \
    struct name

#endif // BCORE_CONTROL_H
