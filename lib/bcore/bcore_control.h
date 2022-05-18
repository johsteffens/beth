/** Author and Copyright 2017 Johannes Bernhard Steffens
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

/// if this include causes an ERROR, try disable it (dependencies, if any, should not be critical)
#include <time.h>
#include <sys/time.h>

// This definition prevents clashes with redefining some POSIX types in other libraries (here: struct timespec)
#define _POSIX_C_SOURCE 1

#include "bcore_first.h"
#include "bcore_types.h"
#include "bcore_feature.h"

/**
 * Collection of low-level functions and macros.
 */

/**********************************************************************************************************************/
/// exiting
void bcore_abort(); // program abort due to an internal error
void bcore_exit( s2_t return_value ); // program exit with return value

/**********************************************************************************************************************/
/// basic messaging

/// formatted message to stdout
void bcore_msgv  ( sc_t format, va_list args );
void bcore_msg   ( sc_t format, ... );
void bcore_msg_fv( sc_t format, va_list args ); // beth format (avoid on low-level errors)
void bcore_msg_fa( sc_t format, ... );          // beth format

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

/// Linear Congruential Generators

// full-cycle generators with good lattice structure (tested for dimensions 1, ..., 8)
static inline u2_t bcore_lcg00_u2( u2_t val ) { return val * (  14000369 * 4 + 1 ) + 1; }
static inline u2_t bcore_lcg01_u2( u2_t val ) { return val * (   7329461 * 4 + 1 ) + 1; }
static inline u2_t bcore_lcg02_u2( u2_t val ) { return val * ( 123457211 * 4 + 1 ) + 1; }
static inline u2_t bcore_lcg03_u2( u2_t val ) { return val * ( 274300003 * 4 + 1 ) + 1; }
static inline u2_t bcore_lcg04_u2( u2_t val ) { return val * ( 274300451 * 4 + 1 ) + 1; }
static inline u2_t bcore_lcg05_u2( u2_t val ) { return val * 309313 + 1427550431; } // from bcore_lcg2_u2

static inline u3_t bcore_lcg00_u3( u3_t val ) { return val * (  505810533149048947 * 4 + 1 ) + 1; }
static inline u3_t bcore_lcg01_u3( u3_t val ) { return val * (  505810533149049769 * 4 + 1 ) + 1; }
static inline u3_t bcore_lcg02_u3( u3_t val ) { return val * (  249949779319063069 * 4 + 1 ) + 1; }
static inline u3_t bcore_lcg03_u3( u3_t val ) { return val * ( 1298444067566256367 * 4 + 1 ) + 1; }

// deprecated generators (poor lattice or renamed)
static inline u2_t bcore_lcg_u2(  u2_t val ) { return val *  67261 + 1385791711; } // poor lattice
static inline u2_t bcore_lcg1_u2( u2_t val ) { return val * 122497 + 1283800139; } // poor lattice
static inline u2_t bcore_lcg2_u2( u2_t val ) { return val * 309313 + 1427550431; } // renamed bcore_lcg05_u2
static inline u2_t bcore_lcg3_u2( u2_t val ) { return val *  76157 + 1175399809; } // poor lattice

/** Xor Shift Generators.
 *  Generators below have a period of 2^n - 1 with 0 being a fix-point; (-> 0 cannot be used as as seed)
 *  They belong to the family of xorshift generators discovered by George Marsaglia (http://www.jstatsoft.org/v08/i14/paper)
 *  These generators do not exhibit lcg-specific lattice patterns but require about 50% more CPU time than lcg.
 *  They also tend to fail on vector distribution tests (s. below).
 *
 *  Vector distribution tests (dims 2, ..., 6)
 *     xsg  fails for dim 2
 *     xsg1 fails for dim 4, 5
 *     xsg2 fails for dim 2, 3
 *     xsg3 fails for dim 3, 4(badly), 5(badly), 6
 */
static inline u2_t bcore_xsg_u2(  u2_t rval ) { rval ^= ( rval >>  7 ); rval ^= ( rval << 25 ); return rval ^ ( rval >> 12 ); }
static inline u2_t bcore_xsg1_u2( u2_t rval ) { rval ^= ( rval >> 11 ); rval ^= ( rval << 21 ); return rval ^ ( rval >> 13 ); }
static inline u2_t bcore_xsg2_u2( u2_t rval ) { rval ^= ( rval >>  5 ); rval ^= ( rval << 27 ); return rval ^ ( rval >>  8 ); }
static inline u2_t bcore_xsg3_u2( u2_t rval ) { rval ^= ( rval >> 17 ); rval ^= ( rval << 15 ); return rval ^ ( rval >> 23 ); }

/**********************************************************************************************************************/
/// Object generic functions

/// if *o is NULL it is created otherwise just passed;
static inline vd_t bcore_pass_create( vd_t* o, bcore_fp_create create )
{
    assert( o );
    return ( *o ) ? *o : ( *o = create() );
}

/// if *o is NULL it is created otherwise just passed;
static inline vd_t bcore_pass_test( vd_t o, sc_t f_name, sc_t file, int line )
{
    if( !o ) bcore_ext_err( f_name, file, line, "Evaluating a NULL pointer." );
    return o;
}

/// if *o is NULL it is created otherwise just passed;
static inline vd_t bcore_pass_bounds( vd_t o, sz_t size, sz_t index, sc_t f_name, sc_t file, int line )
{
    if( index < 0 || index >= size ) bcore_ext_err_fa( f_name, file, line, "Array subscript: index '#<sz_t>' is out of range.", index );
    return o;
}

/**********************************************************************************************************************/
/// Timer

/// retrieves time based on function gettimeofday
void bcore_get_time( u3_t* seconds, u3_t* microseconds );
u3_t bcore_time_us( void ); /// returns (full) time in microseconds
u3_t bcore_time_ms( void ); /// returns (full) time in milliseconds

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
{ \
    clock_t time = clock(); \
    expression; \
    time_var = clock() - time; \
    time_var /= CLOCKS_PER_SEC; \
} \

//  gettimeofday is not standardized on all platforms
#define ABS_TIME_OF( expression, time_var ) \
{ \
    struct timeval t0, t1; \
    gettimeofday( &t0, NULL ); \
    expression; \
    gettimeofday( &t1, NULL ); \
    time_var = t1.tv_sec - t0.tv_sec; \
    time_var += ( t1.tv_usec - t0.tv_usec ) * 1E-6; \
} \

#define CPU_TIME_TO_STDOUT( expression ) \
{ \
    f3_t __time_sec; \
    CPU_TIME_OF( expression, __time_sec ) \
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
    f3_t __time_sec; \
    ABS_TIME_OF( expression, __time_sec ) \
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
#define BCORE_DECLARE_FUNCTION_DETACH( name )    static inline void  name##_detach(    name** o ) { if( !o ) return; name##_discard( *o ); *o = NULL; }
#define BCORE_DECLARE_FUNCTION_ATTACH( name )    static inline name* name##_attach(    name** o, name* src ) { if( !o ) return NULL; name##_discard( *o ); *o = src; return *o; }
#define BCORE_DECLARE_FUNCTION_REPLICATE( name ) static inline name* name##_replicate( name** o, name* src ) { if( !o ) return NULL; name##_discard( *o ); *o = name##_clone( src );  return *o; }

#define BCORE_DECLARE_FUNCTIONS_OBJ( name )\
    BCORE_DECLARE_FUNCTION_INIT( name ) \
    BCORE_DECLARE_FUNCTION_DOWN( name ) \
    BCORE_DECLARE_FUNCTION_COPY( name ) \
    BCORE_DECLARE_FUNCTION_CREATE( name ) \
    BCORE_DECLARE_FUNCTION_DISCARD( name ) \
    BCORE_DECLARE_FUNCTION_CLONE( name ) \
    BCORE_DECLARE_FUNCTION_DETACH( name ) \
    BCORE_DECLARE_FUNCTION_ATTACH( name ) \
    BCORE_DECLARE_FUNCTION_REPLICATE( name )

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

// forward declaration
#define BCORE_FORWARD_OBJECT( name )\
    typedef struct name name

#define BCORE_DECLARE_OBJECT( name )\
    BCORE_FORWARD_OBJECT( name ); \
    BCORE_DECLARE_FUNCTIONS_OBJ( name ) \
    struct name \

// Body definition only
#define BCORE_DECLARE_OBJECT_BODY( name ) \
    struct name

#define BCORE_PASS_CREATE( type, expr ) \
    ((type*)bcore_pass_create((vd_t*)&(expr),(bcore_fp_create)type##_create))

#define BCORE_PASS_TEST( type, expr ) \
    ((type*)bcore_pass_test((vd_t)(expr),__func__,__FILE__,__LINE__))

#define BCORE_PASS_BOUNDS( type, expr, index ) \
    ((type*)bcore_pass_bounds((vd_t)(expr),expr->size,index,__func__,__FILE__,__LINE__))

#endif // BCORE_CONTROL_H
