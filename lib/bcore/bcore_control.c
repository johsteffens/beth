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
    fprintf( stderr, "Error in function %s (%s:%i):\n", func, file, line );
    va_list args;
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
    abort();
}

void bcore_writeln_fv( FILE* file, sc_t format, va_list args )
{
    st_s* s = st_s_create_fv( format, args );
    fwrite( s->sc, 1, s->size, file );
    fwrite( "\n", 1, 1, file );
    fflush( file );
    st_s_discard( s );
}

void bcore_writeln_fa( FILE* file, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_writeln_fv( file, format, args );
    va_end( args );
}

void bcore_msg_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_writeln_fv( stdout, format, args );
    va_end( args );
}

void bcore_err_fv( sc_t format, va_list args )
{
    bcore_writeln_fv( stderr, format, args );
    abort();
}

void bcore_err_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_writeln_fv( stderr, format, args );
    va_end( args );
    abort();
}

void bcore_ext_err_fa( sc_t func, sc_t file, int line, sc_t format, ... )
{
    fprintf( stderr, "Error in function %s (%s:%i):\n", func, file, line );
    va_list args;
    va_start( args, format );
    bcore_writeln_fv( stderr, format, args );
    va_end( args );
    abort();
}

/**********************************************************************************************************************/

vd_t bcore_free( vd_t ptr )
{
    bcore_tbman_free( ptr );
    return NULL;
}

/// ptr == NULL: malloc; ptr != NULL && size == 0: free; ptr != NULL && size != 0: realloc; returns a valid address or NULL; alloc failure produces error
vd_t bcore_alloc( vd_t ptr, sz_t size )
{
    return bcore_tbman_alloc( ptr, size );
}

vd_t bcore_malloc( sz_t size )
{
    return bcore_alloc( NULL, size );
}

vd_t bcore_realloc( vd_t ptr, sz_t size )
{
    return bcore_alloc( ptr, size );
}

/// advanced alloc (see description of bcore_tbman_b(n)_alloc)
vd_t bcore_b_alloc( vd_t current_ptr, sz_t requested_bytes, sz_t* granted_bytes )
{
    return bcore_tbman_b_alloc( current_ptr, requested_bytes, granted_bytes );
}

vd_t bcore_bn_alloc( vd_t current_ptr, sz_t current_bytes, sz_t requested_bytes, sz_t* granted_bytes )
{
    return bcore_tbman_bn_alloc( current_ptr, current_bytes, requested_bytes, granted_bytes );
}

vd_t bcore_u_alloc( sz_t unit_bytes, vd_t current_ptr, sz_t requested_units, sz_t* granted_units )
{
    return bcore_tbman_u_alloc( unit_bytes, current_ptr, requested_units, granted_units );
}

vd_t bcore_un_alloc( sz_t unit_bytes, vd_t current_ptr, sz_t current_units, sz_t requested_units, sz_t* granted_units )
{
    return bcore_tbman_un_alloc( unit_bytes, current_ptr, current_units, requested_units, granted_units );
}

vd_t bcore_fork( vd_t ptr )
{
    return bcore_tbman_fork( ptr );
}

void bcore_release( vd_t ptr )
{
    bcore_tbman_release( ptr );
}

void bcore_release_obj( fp_t down, vd_t ptr )
{
    bcore_tbman_release_obj( ( fp_down_obj )down, ptr );
}

void bcore_release_arg( fp_t down, vc_t arg, vd_t ptr )
{
    bcore_tbman_release_arg( ( fp_down_arg )down, arg, ptr );
}

void bcore_release_obj_arr( fp_t down, vd_t ptr, sz_t size, sz_t step )
{
    bcore_tbman_release_obj_arr( ( fp_down_obj )down, ptr, size, step );
}

void bcore_release_arg_arr( fp_t down, vc_t arg, vd_t ptr, sz_t size, sz_t step )
{
    bcore_tbman_release_arg_arr( ( fp_down_arg )down, arg, ptr, size, step );
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
    if( !dst && !src ) return NULL;
    sz_t len = src ? strlen( src ) : 0;
    dst = bcore_alloc( dst, len + 1 );
    bcore_memcpy( dst, src, len );
    dst[ len ] = 0;
    return dst;
}

int bcore_strcmp( sc_t str1, sc_t str2 )
{
    if( str1 == NULL ) return ( str2 == NULL ) ? 0 :  1;
    if( str2 == NULL ) return -1;
    while( ( *str1 != 0 ) && ( *str2 != 0 ) )
    {
        if( *str1 != *str2 ) return ( *str1 < *str2 ) ? 2 : -2;
        str1++;
        str2++;
    }
    return ( *str1 == 0 ) ? ( ( *str2 == 0 ) ? 0 : +1 ) : -1;
}

int bcore_strcmp_n( sc_t str1, sz_t n1, sc_t str2, sz_t n2 )
{
    if( str1 == NULL ) return ( str2 == NULL ) ? 0 :  1;
    if( str2 == NULL ) return -1;
    if( n1 == 0 ) return ( n2 == 0 ) ? 0 :  1;
    if( n2 == 0 ) return -1;
    sz_t i1 = 0;
    sz_t i2 = 0;
    while( ( i1 < n1 ) && ( i2 < n2 ) )
    {
        if( str1[ i1 ] != str2[ i2 ] ) return ( str1[ i1 ] < str2[ i2 ] ) ? 2 : -2;
        i1++;
        i2++;
    }
    return ( i1 == n1 ) ? ( ( i2 == n2 ) ? 0 : +1 ) : -1;
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
