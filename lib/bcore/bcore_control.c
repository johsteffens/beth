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

#include <stdio.h>

#include "bcore_control.h"
#include "bcore_tbman.h"
#include "bcore_signal.h"
#include "bcore_sc.h"

void bcore_terminate( s2_t return_value, bl_t force_abort )
{
    if( force_abort )
    {
        abort();
    }
    else
    {
        exit( return_value );
    }
}

void bcore_abort()
{
    bcore_terminate( 0, true );
}

void bcore_exit( s2_t return_value )
{
    bcore_terminate( return_value, false );
}

void bcore_write_fv( FILE* file, sc_t format, va_list args )
{
    st_s* s = st_s_create_fv( format, args );
    fwrite( s->sc, 1, s->size, file );
    fflush( file );
    st_s_discard( s );
}

void bcore_write_fa( FILE* file, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_write_fv( file, format, args );
    va_end( args );
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

void bcore_msgv( sc_t format, va_list args )
{
    vfprintf( stdout, format, args );
    fflush( stdout );
}

void bcore_msg( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_msgv( format, args );
    va_end( args );
}

void bcore_msg_fv( sc_t format, va_list args )
{
    bcore_write_fv( stdout, format, args );
}

void bcore_msg_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_write_fv( stdout, format, args );
    fflush( stdout );
    va_end( args );
}

void bcore_wrnv( sc_t format, va_list args )
{
    vfprintf( stderr, format, args );
    fprintf( stderr, "\n" );
}

void bcore_wrn( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
}

void bcore_ext_wrn( sc_t func, sc_t file, int line, sc_t format, ... )
{
    fprintf( stderr, "Error in function %s (%s:%i):\n", func, file, line );
    va_list args;
    va_start( args, format );
    vfprintf( stderr, format, args );
    va_end( args );
    fprintf( stderr, "\n" );
}

void bcore_wrn_fv( sc_t format, va_list args )
{
    bcore_writeln_fv( stderr, format, args );
}

void bcore_wrn_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_writeln_fv( stderr, format, args );
    va_end( args );
}

void bcore_ext_wrn_fa( sc_t func, sc_t file, int line, sc_t format, ... )
{
    fprintf( stderr, "Error in function %s (%s:%i):\n", func, file, line );
    va_list args;
    va_start( args, format );
    bcore_writeln_fv( stderr, format, args );
    va_end( args );
}

void bcore_errv( sc_t format, va_list args )
{
    bcore_wrnv( format, args );
    bcore_abort();
}

void bcore_err( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_wrnv( format, args );
    va_end( args );
    bcore_abort();
}

void bcore_ext_err( sc_t func, sc_t file, int line, sc_t format, ... )
{
    fprintf( stderr, "Error in function %s (%s:%i):\n", func, file, line );
    va_list args;
    va_start( args, format );
    bcore_wrnv( format, args );
    va_end( args );
    bcore_abort();
}

void bcore_err_fv( sc_t format, va_list args )
{
    bcore_writeln_fv( stderr, format, args );
    bcore_abort();
}

void bcore_err_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_writeln_fv( stderr, format, args );
    va_end( args );
    bcore_abort();
}

void bcore_ext_err_fa( sc_t func, sc_t file, int line, sc_t format, ... )
{
    fprintf( stderr, "Error in function %s (%s:%i):\n", func, file, line );
    va_list args;
    va_start( args, format );
    bcore_writeln_fv( stderr, format, args );
    va_end( args );
    bcore_abort();
}

/**********************************************************************************************************************/

vd_t bcore_free( vd_t ptr )
{
    bcore_tbman_free( ptr );
    return NULL;
}

/// ptr == NULL: malloc; ptr != NULL && size == 0: free; ptr != NULL && size != 0: realloc; returns a valid address or NULL; alloc failure produces error
vd_t bcore_alloc( vd_t ptr, uz_t size )
{
    return bcore_tbman_alloc( ptr, size );
}

vd_t bcore_malloc( uz_t size )
{
    return bcore_alloc( NULL, size );
}

vd_t bcore_realloc( vd_t ptr, uz_t size )
{
    return bcore_alloc( ptr, size );
}

/// advanced alloc (see description of bcore_tbman_b(n)_alloc)
vd_t bcore_b_alloc( vd_t current_ptr, uz_t requested_bytes, uz_t* granted_bytes )
{
    return bcore_tbman_b_alloc( current_ptr, requested_bytes, granted_bytes );
}

vd_t bcore_bn_alloc( vd_t current_ptr, uz_t current_bytes, uz_t requested_bytes, uz_t* granted_bytes )
{
    return bcore_tbman_bn_alloc( current_ptr, current_bytes, requested_bytes, granted_bytes );
}

vd_t bcore_u_alloc( uz_t unit_bytes, vd_t current_ptr, uz_t requested_units, uz_t* granted_units )
{
    return bcore_tbman_u_alloc( unit_bytes, current_ptr, requested_units, granted_units );
}

vd_t bcore_un_alloc( uz_t unit_bytes, vd_t current_ptr, uz_t current_units, uz_t requested_units, uz_t* granted_units )
{
    return bcore_tbman_un_alloc( unit_bytes, current_ptr, current_units, requested_units, granted_units );
}

uz_t bcore_references( vc_t ptr )
{
    return bcore_tbman_references( ptr );
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

void bcore_release_obj_arr( fp_t down, vd_t ptr, uz_t size, uz_t step )
{
    bcore_tbman_release_obj_arr( ( fp_down_obj )down, ptr, size, step );
}

void bcore_release_arg_arr( fp_t down, vc_t arg, vd_t ptr, uz_t size, uz_t step )
{
    bcore_tbman_release_arg_arr( ( fp_down_arg )down, arg, ptr, size, step );
}

/**********************************************************************************************************************/

vd_t bcore_memzero( vd_t dst, uz_t size )
{
    if( !dst ) dst = bcore_alloc( NULL, size );
    memset( dst, 0, size );
    return dst;
}

vd_t bcore_u_memzero( uz_t unit_bytes, vd_t dst, uz_t size )
{
    if( !dst ) dst = bcore_alloc( NULL, unit_bytes * size );
    memset( dst, 0, unit_bytes * size );
    return dst;
}

vd_t bcore_memset(  vd_t dst, u0_t val, uz_t size )
{
    if( !dst ) dst = bcore_alloc( NULL, size );
    memset( dst, val, size );
    return dst;
}

vd_t bcore_memcpy( vd_t dst, vc_t src, uz_t size )
{
    if( !dst ) dst = bcore_alloc( NULL, size );
    if( size > 0 ) memcpy( dst, src, size );
    return dst;
}

vd_t bcore_u_memcpy( uz_t unit_bytes, vd_t dst, vc_t src, uz_t size )
{
    return bcore_memcpy( dst, src, unit_bytes * size );
}

vd_t bcore_memmove( vd_t dst, vc_t src, uz_t size )
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
    return sc_t_cpy( dst, src );
}

s2_t bcore_strcmp( sc_t str1, sc_t str2 )
{
    return sc_t_cmp( str1, str2 );
}

s2_t bcore_strcmp_n( sc_t str1, uz_t n1, sc_t str2, uz_t n2 )
{
    return sc_t_cmp_n( str1, n1, str2, n2 );
}

uz_t bcore_strlen( sc_t str )
{
    return sc_t_len( str );
}

bl_t bcore_strany( char c, sc_t str )
{
    return sc_t_any( c, str );
}

/**********************************************************************************************************************/

vd_t bcore_control_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_control" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }
    return NULL;
}
