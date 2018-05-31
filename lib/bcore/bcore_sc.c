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

#include "stdio.h"
#include "bcore_sc.h"
#include "bcore_st.h"
#include "bcore_signal.h"

/** Scans type; returns number of characters consumed; returns -1 in case of syntax error
 *  Syntax:
 *    - Elementary types may be expressed as is, e.g. sz_t.
 *    - Any type may be enclosed in <>
 *    - A pointer is specified via <...*> e.g. <u3_t*>
 *    - no whitespaces permitted
 */
s2_t sc_t_scan_type( sc_t format, tp_t* p_type, bl_t* p_is_ptr )
{
    sc_t fp = format;
    if( fp[ 0 ] == 0 ) return -1;
    tp_t type = 0;
    if( fp[ 0 ] == '<' )
    {
        fp++;
        sc_t fp0 = fp;
        while( *fp != 0 && *fp != '>' ) fp++;
        if( *fp != '>' ) return -1;
        sc_t fp1 = fp++;
        if( fp1[ -1 ] == '*' )
        {
            fp1--;
            if( p_is_ptr ) *p_is_ptr = true;
        }
        else
        {
            if( p_is_ptr ) *p_is_ptr = false;
        }
        type = typeof_n( fp0, fp1 - fp0 );
    }
    else if( fp[ 1 ] != 0 && fp[ 2 ] == '_' && fp[ 3 ] == 't' ) // all types of format xy_t
    {
        type = typeof_n( fp, 4 );
        fp += 4;
    }
    else
    {
        return -1;
    }
    if( p_type ) *p_type = type;
    return fp - format;
}

sz_t sc_t_fnv( sd_t o, sz_t space, sc_t f, sz_t fsize, va_list* p_args )
{
    if( !o ) space = 0;
    sd_t dst = o;
    sz_t i = 0;
    sz_t ret_size = 0;
    while( i < fsize )
    {
        if( f[ i ] == '#' )
        {
            i++;
            tp_t type = 0;
            int inc = 0;
            bl_t is_ptr = false;

            if( f[ i ] == 'r' ) // repeating (repetition of a string resulting from 1-pass format-block evaluation)
            {
                i++;
                sz_t n = 0;
                if( f[ i ] >= '0' && f[ i ] <= '9' )
                {
                    n = atoi( f + i );
                    while( f[ i ] >= '0' && f[ i ] <= '9' ) i++;
                }
                else if( f[ i ] == 'n' )  // get number of repetitions from sz_t argument
                {
                    i++;
                    n = va_arg( *p_args, sz_t );
                }
                else
                {
                    ERR( "Could not obtain number of repetitions repeat-expression '%s'.", f );
                }
                char start_char = f[ i ];
                sz_t start_block = i + 1;
                char stop_char = 0;
                switch( start_char )
                {
                    case '(': stop_char = ')'; break;
                    case '{': stop_char = '}'; break;
                    case '[': stop_char = ']'; break;
                    case '<': stop_char = '>'; break;
                    default: ERR( "Invalid bracket character '%c' in format string '%s'.", start_char, f ); break;
                }

                sz_t count = 1;
                while( i < fsize && count > 0 )
                {
                    i++;
                    if     ( f[ i ] == stop_char  ) count--;
                    else if( f[ i ] == start_char ) count++;
                }
                if( count > 0 ) ERR( "Missing block termination in format string '%s'.", start_char, f );
                sz_t stop_block = i;
                i++;

                if( n > 0 )
                {
                    sz_t txt_size = 0;
                    txt_size = sc_t_fnv( dst, space, f + start_block, stop_block - start_block, p_args );
                    sc_t src = dst;
                    dst   += ( txt_size < space ) ? txt_size : space;
                    space -= ( txt_size < space ) ? txt_size : space;
                    ret_size += txt_size;
                    for( sz_t i = 1; i < n; i++ )
                    {
                        for( sz_t i = 0; i < txt_size; i++ )
                        {
                            if( space > 0 )
                            {
                                *dst++ = src[ i ];
                                space--;
                            }
                        }
                        ret_size += txt_size;
                    }
                }

            }
            else if( f[ i ] == 'p' ) // size control with padding: resulting string can not be smaller than target size
            {
                i++;
                bl_t left = false;
                if( f[ i ] == 'l' ) // left padding
                {
                    i++;
                    left = true;
                }
                sz_t n = 0;
                if( f[ i ] >= '0' && f[ i ] <= '9' )
                {
                    n = atoi( f + i );
                    while( f[ i ] >= '0' && f[ i ] <= '9' ) i++;
                }
                else if( f[ i ] == 'n' )  // get target size from sz_t argument
                {
                    i++;
                    n = va_arg( *p_args, sz_t );
                }
                else
                {
                    ERR( "Could not obtain target size in padding expression '%s'.", f );
                }
                char pad_char;
                if( f[ i ] == '\'' )
                {
                    i++;
                    pad_char = f[ i++ ];
                    if( f[ i++ ] != '\'' ) ERR( "Closing mark (') for padding character missing in padding expression '%s'.", f );
                }
                else
                {
                    pad_char = f[ i++ ];
                }
                char start_char = f[ i ];
                sz_t start_block = i + 1;
                char stop_char = 0;
                switch( start_char )
                {
                    case '(': stop_char = ')'; break;
                    case '{': stop_char = '}'; break;
                    case '[': stop_char = ']'; break;
                    case '<': stop_char = '>'; break;
                    default: ERR( "Invalid bracket character '%c' in format string '%s'.", start_char, f ); break;
                }

                sz_t count = 1;
                while( i < fsize && count > 0 )
                {
                    i++;
                    if     ( f[ i ] == stop_char  ) count--;
                    else if( f[ i ] == start_char ) count++;
                }
                if( count > 0 ) ERR( "Missing block termination in format string '%s'.", start_char, f );
                sz_t stop_block = i;
                i++;
                sz_t txt_size = 0;
                if( left ) // compute space
                {
                    va_list argsl;
                    va_copy( argsl, *p_args );
                    txt_size = sc_t_fnv( NULL, 0, f + start_block, stop_block - start_block, &argsl );
                    va_end( argsl );
                }
                else
                {
                    txt_size = sc_t_fnv( dst, space, f + start_block, stop_block - start_block, p_args );
                    dst   += ( txt_size < space ) ? txt_size : space;
                    space -= ( txt_size < space ) ? txt_size : space;
                    ret_size += txt_size;
                }

                // padding
                if( txt_size < n )
                {
                    for( sz_t i = n - txt_size; i > 0; i-- )
                    {
                        if( space > 1 )
                        {
                            *dst++ = pad_char;
                            space--;
                        }
                        ret_size++;
                    }
                }

                if( left )
                {
                    if( space > 0 )
                    {
                        txt_size = sc_t_fnv( dst, space, f + start_block, stop_block - start_block, p_args );
                        dst   += ( txt_size < space ) ? txt_size : space;
                        space -= ( txt_size < space ) ? txt_size : space;
                    }
                    else
                    {
                        txt_size = sc_t_fnv( NULL, 0, f + start_block, stop_block - start_block, p_args );
                    }
                    ret_size += txt_size;
                }
            }
            else if( f[ i ] == 't' ) // size control with truncation: resulting string can not be bigger than target size
            {
                i++;
                bl_t left = false;
                if( f[ i ] == 'l' ) // left truncation
                {
                    i++;
                    left = true;
                }
                sz_t n = 0;
                if( f[ i ] >= '0' && f[ i ] <= '9' )
                {
                    n = atoi( f + i );
                    while( f[ i ] >= '0' && f[ i ] <= '9' ) i++;
                }
                else if( f[ i ] == 'n' )  // get target size from sz_t argument
                {
                    i++;
                    n = va_arg( *p_args, sz_t );
                }
                else
                {
                    ERR( "Could not obtain target size in padding expression '%s'.", f );
                }
                char start_char = f[ i ];
                sz_t start_block = i + 1;
                char stop_char = 0;
                switch( start_char )
                {
                    case '(': stop_char = ')'; break;
                    case '{': stop_char = '}'; break;
                    case '[': stop_char = ']'; break;
                    case '<': stop_char = '>'; break;
                    default: ERR( "Invalid bracket character '%c' in format string '%s'.", start_char, f ); break;
                }

                sz_t count = 1;
                while( i < fsize && count > 0 )
                {
                    i++;
                    if     ( f[ i ] == stop_char  ) count--;
                    else if( f[ i ] == start_char ) count++;
                }
                if( count > 0 ) ERR( "Missing block termination in format string '%s'.", start_char, f );
                sz_t stop_block = i;
                i++;
                sz_t txt_size = 0;
                // compute space
                {
                    va_list argsl;
                    va_copy( argsl, *p_args );
                    txt_size = sc_t_fnv( NULL, 0, f + start_block, stop_block - start_block, &argsl );
                    va_end( argsl );
                }
                if( txt_size > n )
                {
                    sd_t buf = bcore_malloc( txt_size + 1 );
                    sc_t_fnv( buf, txt_size + 1, f + start_block, stop_block - start_block, p_args );
                    sc_t src = left ? buf + txt_size - n : buf;
                    sz_t ncpy = ( n < space ) ? n : space;
                    bcore_memcpy( dst, src, ncpy );
                    dst   += ncpy;
                    space -= ncpy;
                    bcore_free( buf );
                    ret_size += n;
                }
                else
                {
                    txt_size = sc_t_fnv( dst, space, f + start_block, stop_block - start_block, p_args );
                    dst   += ( txt_size < space ) ? txt_size : space;
                    space -= ( txt_size < space ) ? txt_size : space;
                    ret_size += txt_size;
                }
            }
            else if( ( inc = sc_t_scan_type( f + i, &type, &is_ptr ) ) >= 0 )
            {
                int sres = 0;
                if( is_ptr )
                {
                    switch( type )
                    {
                        case TYPEOF_u0_t:
                        {
                            u0_t* v = va_arg( *p_args, u0_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIu0_t"", *v );
                        }
                        break;

                        case TYPEOF_u1_t:
                        {
                            u1_t* v = va_arg( *p_args, u1_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIu1_t"", *v );
                        }
                        break;

                        case TYPEOF_u2_t:
                        {
                            u2_t* v = va_arg( *p_args, u2_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIu2_t"", *v );
                        }
                        break;

                        case TYPEOF_u3_t:
                        {
                            u3_t* v = va_arg( *p_args, u3_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIu3_t"", *v );
                        }
                        break;

                        case TYPEOF_umax_t:
                        {
                            umax_t* v = va_arg( *p_args, umax_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIumax_t"", *v );
                        }
                        break;

                        case TYPEOF_s0_t:
                        {
                            s0_t* v = va_arg( *p_args, s0_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIs0_t"", *v );
                        }
                        break;

                        case TYPEOF_s1_t:
                        {
                            s1_t* v = va_arg( *p_args, s1_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIs1_t"", *v );
                        }
                        break;

                        case TYPEOF_s2_t:
                        {
                            s2_t* v = va_arg( *p_args, s2_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIs2_t"", *v );
                        }
                        break;

                        case TYPEOF_s3_t:
                        {
                            s3_t* v = va_arg( *p_args, s3_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIs3_t"", *v );
                        }
                        break;

                        case TYPEOF_smax_t:
                        {
                            smax_t* v = va_arg( *p_args, smax_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIsmax_t"", *v );
                        }
                        break;

                        case TYPEOF_sz_t:
                        {
                            sz_t* v = va_arg( *p_args, sz_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIsz_t"", *v );
                        }
                        break;

                        case TYPEOF_offset_t:
                        {
                            offset_t* v = va_arg( *p_args, offset_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIoffset_t"", *v );
                        }
                        break;

                        case TYPEOF_f2_t:
                        {
                            f2_t* v = va_arg( *p_args, f2_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIf2_t"", *v );
                        }
                        break;

                        case TYPEOF_f3_t:
                        {
                            f3_t* v = va_arg( *p_args, f3_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRIf3_t"", *v );
                        }
                        break;

                        case TYPEOF_tp_t:
                        {
                            tp_t* v = va_arg( *p_args, tp_t* );
                            if( v ) sres = snprintf( dst, space, "%"PRItp_t"", *v );
                        }
                        break;

                        case TYPEOF_bl_t:
                        {
                            bl_t* v = va_arg( *p_args, bl_t* );
                            if( v ) sres = snprintf( dst, space, *v ? "true" : "false" );
                        }
                        break;

                        case TYPEOF_sc_t:
                        case TYPEOF_sd_t:
                        {
                            sc_t* v = va_arg( *p_args, sc_t* );
                            if( v ) sres = snprintf( dst, space, "%s", *v );
                        }
                        break;

                        case TYPEOF_st_s:
                        {
                            st_s* v = va_arg( *p_args, st_s* );
                            ASSERT( *(aware_t*)v == TYPEOF_st_s );
                            if( v ) sres = snprintf( dst, space, "%s", v->sc );
                        }
                        break;

                        default: ERR( "Unhandled type specifier '%s'.", f + i ); break;
                    }
                }
                else
                {
                    switch( type )
                    {
                        case TYPEOF_u0_t: // u0 and u1 is promoted to unsigned int
                        case TYPEOF_u1_t: sres = snprintf( dst, space, "%u", va_arg( *p_args, unsigned int ) ); break;
                        case TYPEOF_u2_t: sres = snprintf( dst, space, "%"PRIu2_t"", va_arg( *p_args, u2_t ) ); break;
                        case TYPEOF_u3_t: sres = snprintf( dst, space, "%"PRIu3_t"", va_arg( *p_args, u3_t ) ); break;
                        case TYPEOF_s0_t: // s0 and s1 is promoted to int
                        case TYPEOF_s1_t: sres = snprintf( dst, space, "%i",         va_arg( *p_args, int  ) ); break;
                        case TYPEOF_s2_t: sres = snprintf( dst, space, "%"PRIs2_t"", va_arg( *p_args, s2_t ) ); break;
                        case TYPEOF_s3_t: sres = snprintf( dst, space, "%"PRIs3_t"", va_arg( *p_args, s3_t ) ); break;
                        case TYPEOF_sz_t: sres = snprintf( dst, space, "%"PRIsz_t"", va_arg( *p_args, sz_t ) ); break;
                        // f2_t is promoted to double
                        case TYPEOF_f2_t: sres = snprintf( dst, space, "%g",       va_arg( *p_args, double ) ); break;
                        case TYPEOF_f3_t: sres = snprintf( dst, space, "%"PRIf3_t"", va_arg( *p_args, f3_t ) ); break;
                        case TYPEOF_tp_t: sres = snprintf( dst, space, "%"PRItp_t"", va_arg( *p_args, tp_t ) ); break;
                        // bl_t is promoted to int
                        case TYPEOF_bl_t: sres = snprintf( dst, space, va_arg( *p_args, int ) ? "true" : "false" ); break;
                        case TYPEOF_sc_t: sres = snprintf( dst, space, "%s", va_arg( *p_args, sc_t ) ); break;
                        case TYPEOF_sd_t: sres = snprintf( dst, space, "%s", va_arg( *p_args, sd_t ) ); break;
                        case TYPEOF_st_s: sres = snprintf( dst, space, "%s", va_arg( *p_args, st_s ).sc ); break;

                        default:
                        {
                            /// some special types
                            if( type == TYPEOF_char )
                            {
                                sres = snprintf( dst, space, "%c", va_arg( *p_args, int ) );
                            }
                            else
                            {
                                ERR( "Unhandled type specifier '%s", f + i ); break;
                            }
                        }
                    }
                }
                if( sres < 0 ) ERR( "Format error '%s", f + i );
                i += inc;
                ret_size += sres;
                if( sres < space )
                {
                    dst += sres;
                    space -= sres;
                }
                else
                {
                    dst += space;
                    space = 0;
                }
            }
            else if( f[ i ] == '#' )
            {
                if( space > 1 )
                {
                    *dst++ = f[ i ];
                    space--;
                }
                ret_size++;
                i++;
            }
            else if( ( bcore_strcmp( "char", &f[ i ] ) & 2 ) == 0 )
            {
                i += 4;
                snprintf( dst, space, "%c", va_arg( *p_args, unsigned int ) );
                if( space > 1 )
                {
                    space--;
                    dst++;
                }
                ret_size++;
            }
            else
            {
                ERR( "Format directive in '%s' not recognized", f );
            }
        }
        else
        {
            if( space > 1 )
            {
                *dst++ = f[ i ];
                space--;
            }
            ret_size++;
            i++;
        }
    }
    if( space > 0 ) *dst = 0;
    return ret_size;
}

sd_t sc_t_cpy( sd_t dst, sc_t src )
{
    if( !dst && !src ) return NULL;
    sz_t len = src ? strlen( src ) : 0;
    dst = bcore_alloc( dst, len + 1 );
    bcore_memcpy( dst, src, len );
    dst[ len ] = 0;
    return dst;
}

s2_t sc_t_cmp( sc_t str1, sc_t str2 )
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

s2_t sc_t_cmp_n( sc_t str1, sz_t n1, sc_t str2, sz_t n2 )
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

sz_t sc_t_len( sc_t str )
{
    if( !str ) return 0;
    return strlen( str );
}

bl_t sc_t_any( char c, sc_t str )
{
    if( str == NULL ) return false;
    while( *str != 0 ) if( *str++ == c ) return true;
    return false;
}

/**********************************************************************************************************************/

vd_t bcore_sc_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_sc" ) ) )
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

/**********************************************************************************************************************/

