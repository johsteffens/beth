/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include <stdio.h>

#include "bcore_control.h"
#include "bcore_tbman.h"

void bcore_msg( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    vfprintf( stdout, format, args );
    va_end( args );
}

void bcore_errv( sc_t format, va_list args )
{
    vfprintf( stderr, format, args );
    fprintf( stderr, "\n" );
    abort();
}

void bcore_err( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
    abort();
}

void bcore_ext_err( sc_t func, sc_t file, int line, sc_t format, ... )
{
    fprintf( stderr, "error in function %s (%s:%i):\n", func, file, line );
    va_list args;
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
    abort();
}

/**********************************************************************************************************************/

// implemented in tbman
vd_t bcore_external_b_alloc(  vd_t current_ptr,                     sz_t requested_bytes, sz_t* granted_bytes );
vd_t bcore_external_bn_alloc( vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes );

vd_t bcore_external_u_alloc(  sz_t unit_bytes,  vd_t current_ptr,                       sz_t requested_units, sz_t* reserved_units );
vd_t bcore_external_un_alloc( sz_t unit_bytes,  vd_t current_ptr,   sz_t current_units, sz_t requested_units, sz_t* reserved_units );

vd_t bcore_free( vd_t buf )
{
#ifdef USE_BCORE_TBMAN
    return bcore_tbman_b_alloc( buf, 0, NULL );
#else
    return bcore_external_b_alloc(       buf, 0, NULL );
#endif // USE_BCORE_TBMAN
}

vd_t bcore_alloc( vd_t buf, sz_t size )
{
#ifdef USE_BCORE_TBMAN
    return bcore_tbman_b_alloc( buf, size, NULL );
#else
    return bcore_external_b_alloc( buf, size, NULL );
#endif // USE_BCORE_TBMAN
}

vd_t bcore_b_alloc( vd_t current_ptr, sz_t requested_bytes, sz_t* granted_bytes )
{
#ifdef USE_BCORE_TBMAN
    return bcore_tbman_b_alloc( current_ptr, requested_bytes, granted_bytes );
#else
    return bcore_external_b_alloc( current_ptr, requested_bytes, granted_bytes );
#endif // USE_BCORE_TBMAN
}

vd_t bcore_bn_alloc( vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes )
{
#ifdef USE_BCORE_TBMAN
    return bcore_tbman_bn_alloc( current_ptr, current_bytes, requested_bytes, granted_bytes );
#else
    return bcore_external_bn_alloc( current_ptr, current_bytes, requested_bytes, granted_bytes );
#endif // USE_BCORE_TBMAN
}

vd_t bcore_u_alloc( sz_t unit_bytes, vd_t current_ptr, sz_t requested_units, sz_t* granted_units )
{
#ifdef USE_BCORE_TBMAN
    return bcore_tbman_u_alloc( unit_bytes, current_ptr, requested_units, granted_units );
#else
    return bcore_external_u_alloc( unit_bytes, current_ptr, requested_units, granted_units );
#endif // USE_BCORE_TBMAN
}

vd_t bcore_un_alloc( sz_t unit_bytes, vd_t current_ptr, sz_t current_units, sz_t requested_units, sz_t* granted_units )
{
#ifdef USE_BCORE_TBMAN
    return bcore_tbman_un_alloc( unit_bytes, current_ptr, current_units, requested_units, granted_units );
#else
    return bcore_external_un_alloc( unit_bytes, current_ptr, current_units, requested_units, granted_units );
#endif // USE_BCORE_TBMAN
}

/**********************************************************************************************************************/

vd_t bcore_memzero( vd_t dst, sz_t size )
{
    if( !dst ) dst = bcore_alloc( NULL, size );
    memset( dst, 0, size );
    return dst;
}

vd_t bcore_u_memzero( sz_t unit_bytes, vd_t dst, sz_t size )
{
    if( !dst ) dst = bcore_alloc( NULL, unit_bytes * size );
    memset( dst, 0, unit_bytes * size );
    return dst;
}

vd_t bcore_memset(  vd_t dst, u0_t val, sz_t size )
{
    if( !dst ) dst = bcore_alloc( NULL, size );
    memset( dst, val, size );
    return dst;
}

vd_t bcore_memcpy( vd_t dst, vc_t src, sz_t size )
{
    if( !dst ) dst = bcore_alloc( NULL, size );
    if( size > 0 ) memcpy( dst, src, size );
    return dst;
}

vd_t bcore_u_memcpy( sz_t unit_bytes, vd_t dst, vc_t src, sz_t size )
{
    return bcore_memcpy( dst, src, unit_bytes * size );
}

vd_t bcore_memmove( vd_t dst, vc_t src, sz_t size )
{
    if( !dst )
    {
        dst = bcore_memcpy( dst, src, size );
    }
    else
    {
        if( size > 0 ) memmove( dst, src, size );

    }
    return dst;
}

/**********************************************************************************************************************/

sd_t bcore_strcpy( sd_t dst, sc_t src )
{
    dst = bcore_alloc( dst, strlen( src ) + 1 );
    strcpy( dst, src );
    return dst;
}

int bcore_strcmp( sc_t str1, sc_t str2 )
{
    if( str1 == NULL ) return ( str2 == NULL ) ? 0 :  1;
    if( str2 == NULL ) return ( str1 == NULL ) ? 0 : -1;
    while( ( *str1 != 0 ) && ( *str2 != 0 ) )
    {
        if( *str1 != *str2 ) return ( *str1 < *str2 ) ? 2 : -2;
        str1++;
        str2++;
    }
    return ( *str1 == 0 ) ? ( ( *str2 == 0 ) ? 0 : +1 ) : -1;
}

sz_t bcore_strlen( sc_t str )
{
    if( !str ) return 0;
    return strlen( str );
}

bool bcore_strany( char c, sc_t str )
{
    if( str == NULL ) return false;
    while( *str != 0 ) if( *str == c ) return true;
    return false;
}

/**********************************************************************************************************************/

vd_t bcore_control_signal( tp_t target, tp_t signal, vd_t object )
{
    return NULL;
}
