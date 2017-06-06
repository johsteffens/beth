/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#ifndef BCORE_CONTROL_H
#define BCORE_CONTROL_H

#include "bcore_first.h"
#include "bcore_types.h"

/**
 * Collection of low-level functions and macros.
 */

/**********************************************************************************************************************/
/// basic messaging

/// formatted message to stdout
void bcore_msg( sc_t format, ... );

/// formatted message + newline to stderr and abort
void bcore_err( sc_t format, ... );

/// extended formatted message + newline to stderr and abort
void bcore_ext_err( sc_t f_name, sc_t file, int line, sc_t format, ... );

/**********************************************************************************************************************/
/// memory management

/** When USE_BCORE_MEMORY_MANAGER is defined, memory management functions below use bcore_memory manager
 *  otherwise they fall back to directly using stdlib functions malloc, realloc and free.
 */
#define USE_BCORE_MEMORY_MANAGER

/// checks NULL; frees memory in case !NULL; returns NULL
vd_t bcore_free( vd_t buf );

/// buf == NULL: malloc; buf != NULL && size == 0: free; buf != NULL && size != 0: realloc; returns a valid address or NULL; alloc failure produces error
vd_t bcore_alloc( vd_t buf, sz_t size );

static inline vd_t bcore_malloc( sz_t size ) { return bcore_alloc( NULL, size ); }
static inline vd_t bcore_realloc( vd_t buf, sz_t size ) { return bcore_alloc( buf, size ); }

/// advanced alloc (see description of bcore_memory_manager_b(n)_alloc)
vd_t bcore_b_alloc(                   vd_t current_ptr,                     sz_t requested_bytes, sz_t* granted_bytes );
vd_t bcore_bn_alloc(                  vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes );
vd_t bcore_u_alloc(  sz_t unit_bytes, vd_t current_ptr,                     sz_t requested_units, sz_t* granted_units );
vd_t bcore_un_alloc( sz_t unit_bytes, vd_t current_ptr, sz_t current_units, sz_t requested_units, sz_t* granted_units );

/**********************************************************************************************************************/
/// memory set, copy, move

/// sets memory to zero. When dst == NULL function allocates destination first, copies and returns address.
vd_t bcore_memzero( vd_t dst, sz_t size );

/// like stdlib memcpy but when dst == NULL function allocates destination first, copies and returns address; size == 0 allowed
vd_t bcore_memcpy( vd_t dst, vc_t src, sz_t size );

/// like stdlib memmove but when dst == NULL function allocates destination first, copies and returns address; size == 0 allowed
vd_t bcore_memmove( vd_t dst, vc_t src, sz_t size );

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
int bcore_strcmp( sc_t str1, sc_t str2 );

/// like strlen but also accepts NULL
sz_t bcore_strlen( sc_t str );

/// returns true iff c occurs in str
bool bcore_strany( char c, sc_t str );

/**********************************************************************************************************************/
/// Pseudo random generators (non-cryptographic)

/** Linear Congruential Generators.
 *  Generators below have a complete (2^n) period; (I did not fully evaluate the quality of randomness.)
 *  As for all LCG, the period length of lower order bits is maximally 2^bit-order. Hence higher order bits
 *  are more suitable for randomization than lower order bits.
 */
static inline u2_t bcore_lcg_u2( u2_t val ) { return val * 67261      + 1385791711;          }

/** Xor Shift Generators.
 *  Generators below should have a period of 2^n - 1 with 0 being the fix-point; (-> 0 cannot be used as as seed)
 *  They belong to the family of xorshift generators discovered by George Marsaglia (http://www.jstatsoft.org/v08/i14/paper)
 *  These generators exhibit better randomness than LCG but require about 50% more CPU time.
 */
static inline u2_t bcore_xsg_u2( u2_t rval ) { rval ^= ( rval >>  7 ); rval ^= ( rval << 25 ); return rval ^ ( rval >> 12 ); } // period verified

/**********************************************************************************************************************/
/// Hashing (non-cryptographic)

/** FNV-1a for 0-terminated strings and data blocks.
 *  Conceived by Glenn Fowler, Landon Curt Noll and Kiem-Phon Vo in 1991.
 *  References:
 *  http://www.isthe.com/chongo/tech/comp/fnv/index.html
 *  http://en.wikipedia.org/wiki/Fowler_Noll_Vo_hash
 */
static inline u2_t bcore_fnv_fold_text_u2( u2_t hash, sc_t text )
{
    while( *text ) hash = ( hash ^ ( u0_t )( *text++ ) ) * 0x01000193u;
    return hash;
}

static inline u2_t bcore_fnv_fold_data_u2( u2_t hash, vc_t data, sz_t size )
{
    const u0_t* src = data;
    for( sz_t i = 0; i < size; i++ ) hash = ( hash ^ src[ i ] ) * 0x01000193u;
    return hash;
}

static inline u2_t bcore_fnv_hash_text_u2( sc_t text )            { return bcore_fnv_fold_text_u2( 0x811c9dc5u, text );       }
static inline u2_t bcore_fnv_hash_data_u2( vc_t data, sz_t size ) { return bcore_fnv_fold_data_u2( 0x811c9dc5u, data, size ); }

/**********************************************************************************************************************/
/// Macros

/// error message
#define ERR( ... ) bcore_ext_err( __func__, __FILE__, __LINE__, __VA_ARGS__ )

/// same purpose as assert() but cannot be switched off via NDEBUG; typically used in selftests
#define ASSERT( condition ) if( !(condition) ) bcore_err( "assertion '%s' failed in function %s (%s line %i)\n", #condition, __func__, __FILE__, __LINE__ )

/// object related functions
#define DEFINE_FUNCTION_INIT_FLAT( name ) \
void name##_init( name* o ) \
{ \
    bcore_memzero( o, sizeof( name ) ); \
}

#define DEFINE_FUNCTION_DOWN_FLAT( name ) \
void name##_down( name* o ) {}

#define DEFINE_FUNCTION_COPY_FLAT( name ) \
void name##_copy( name* o, const name* src ) \
{ \
    if( o == src ) return; \
    bcore_memcpy( o, src, sizeof( name ) ); \
}

#define DEFINE_FUNCTION_MOVE( name ) \
void name##_move( name* o, name* src ) \
{ \
    if( o == src ) return; \
    name##_init( o ); \
    name##_copy( o, src ); \
    name##_down( src ); \
}

#define DEFINE_FUNCTION_CREATE( name ) \
name* name##_create() \
{ \
    name* o = bcore_alloc( NULL, sizeof( name ) ); \
    name##_init( o ); \
    return o; \
}

#define DEFINE_FUNCTION_DISCARD( name ) \
void name##_discard( name* o ) \
{ \
    if( !o ) return; \
    name##_down( o ); \
    bcore_bn_alloc( o, sizeof( name ), 0, NULL ); \
}

#define DEFINE_FUNCTION_CLONE( name ) \
name* name##_clone( const name* o ) \
{ \
    if( !o ) return NULL; \
    name* o_l = name##_create(); \
    name##_copy( o_l, o ); \
    return o_l; \
}

#endif // BCORE_CONTROL_H
