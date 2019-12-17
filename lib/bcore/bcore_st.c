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

#include "stdio.h"
#include "bcore_st.h"
#include "bcore_sc.h"
#include "bcore_control.h"
#include "bcore_tbman.h"
#include "bcore_name_manager.h"
#include "bcore_flect.h"
#include "bcore_signal.h"
#include "bcore_life.h"
#include "bcore_signal.h"

void st_s_init( st_s* o )
{
    o->_ = TYPEOF_st_s;
    o->sc = "";
    o->size = o->space = 0;
}

void st_s_initvf( st_s* o, sc_t format, va_list args )
{
    st_s_init( o );
    va_list args1;
    va_copy( args1, args );
    s3_t n = vsnprintf( NULL, 0, format, args1 );
    if( n < 0 ) ERR( "Format error in '%s'\n", format );
    o->space = n + 1;
    va_end( args1 );
    if( o->space < 8 ) o->space = 8;
    o->data = bcore_b_alloc( NULL, o->space, &o->space );
    vsnprintf( o->data, o->space, format, args );
    o->size = bcore_strlen( o->sc );
}

void st_s_init_fv( st_s* o, sc_t format, va_list args )
{
    st_s_init( o );
    uz_t format_size = bcore_strlen( format );
    s3_t n = 0;
    {
        va_list argsl;
        va_copy( argsl, args );
        n = sc_t_fnv( NULL, 0, format, format_size, &argsl );
        va_end( argsl );
    }
    if( n < 0 ) ERR( "Format error in '%s'\n", format );
    o->space = n + 1;
    if( o->space < 8 ) o->space = 8;
    o->data = bcore_b_alloc( NULL, o->space, &o->space );

    {
        va_list argsl;
        va_copy( argsl, args );
        sc_t_fnv( o->data, o->space, format, format_size, &argsl );
        va_end( argsl );
    }

    o->size = bcore_strlen( o->sc );
}

void st_s_initf( st_s* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s_initvf( o, format, args );
    va_end( args );
}

void st_s_init_fa( st_s* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s_init_fv( o, format, args );
    va_end( args );
}

void st_s_init_sc_n( st_s* o, sc_t sc, uz_t sc_size )
{
    st_s_init( o );
    o->space = sc_size < 8 ? 8 : sc_size + 1;
    o->data  = bcore_b_alloc( NULL, o->space, &o->space );
    bcore_memcpy( o->data, sc, sc_size );
    o->size = sc_size;
    o->data[ o->size ] = 0;
}

void st_s_init_sd_d( st_s* o, sd_t sd )
{
    st_s_init( o );
    if( sd != NULL )
    {
        o->data  = sd;
        o->size  = strlen( sd );
        o->space = o->size + 1;
    }
}

void st_s_init_sc( st_s* o, sc_t sc )
{
    st_s_init_sc_n( o, sc, bcore_strlen( sc ) );
}

void  st_s_init_weak_st( st_s* o, const st_s* st )
{
    st_s_init( o );
    o->sc    = st->sc;
    o->size  = st->size;
}

void st_s_init_weak_sc( st_s* o, sc_t sc )
{
    st_s_init( o );
    o->sc    = sc;
    o->size  = bcore_strlen( sc );
}

void st_s_down( st_s* o )
{
    if( o->space ) bcore_bn_alloc( o->data, o->space, 0, &o->space );
    o->size = 0;
}

void  st_s_set_size( st_s* o, u0_t fill_char, uz_t size )
{
    if( o->space <= size )
    {
        if( o->space > 0 ) bcore_bn_alloc( o->data, o->space, 0, &o->space );
        o->space = size < 8 ? 8 : size + 1;
        o->data  = bcore_b_alloc( NULL, o->space, &o->space );
    }
    for( sz_t i = 0; i < size; i++ ) o->data[ i ] = fill_char;
    o->size = size;
    o->data[ o->size ] = 0;
}

void st_s_copy( st_s* o, const st_s* src )
{
    if( !src || o == src ) return;
    if( o->space <= src->size )
    {
        if( o->space > 0 ) bcore_bn_alloc( o->data, o->space, 0, &o->space );
        o->space = src->size < 8 ? 8 : src->size + 1;
        o->data  = bcore_b_alloc( NULL, o->space, &o->space );
    }
    bcore_memcpy( o->data, src->data, src->size );
    o->size = src->size;
    o->data[ o->size ] = 0;
}

void st_s_copy_sc_n( st_s* o, sc_t sc, uz_t sc_size )
{
    if( o->space <= sc_size )
    {
        if( o->space > 0 ) bcore_bn_alloc( o->data, o->space, 0, &o->space );
        o->space = sc_size < 8 ? 8 : sc_size + 1;
        o->data  = bcore_b_alloc( NULL, o->space, &o->space );
    }
    bcore_memcpy( o->data, sc, sc_size );
    o->size = sc_size;
    o->data[ o->size ] = 0;
}

void st_s_copy_sc( st_s* o, sc_t sc )
{
    st_s_copy_sc_n( o, sc, bcore_strlen( sc ) );
}

void st_s_assign_sc( st_s* o, sc_t sc )
{
    uz_t src_size = bcore_strlen( sc );
    if( o->space > 0 ) bcore_bn_alloc( o->data, o->space, 0, &o->space );
    o->sc   = sc;
    o->size = src_size;
}

void st_s_copyvf( st_s* o, sc_t format, va_list args  )
{
    st_s_down( o );
    st_s_initvf( o, format, args );
}

void st_s_copyf( st_s* o, sc_t format, ...  )
{
    va_list args;
    va_start( args, format );
    st_s_copyvf( o, format, args );
    va_end( args );
}

void st_s_copy_fv( st_s* o, sc_t format, va_list args  )
{
    st_s_down( o );
    st_s_init_fv( o, format, args );
}

void st_s_copy_fa( st_s* o, sc_t format, ...  )
{
    va_list args;
    va_start( args, format );
    st_s_copy_fv( o, format, args );
    va_end( args );
}

void st_s_copy_typed( st_s* o, tp_t type, vc_t src )
{
    switch( type )
    {
        case TYPEOF_st_s: st_s_copy( o,              (const st_s*)src ); break;
        case TYPEOF_sc_t: st_s_copy_sc( o,          *(const sc_t*)src ); break;
        case TYPEOF_sd_t: st_s_copy_sc( o,          *(const sc_t*)src ); break;
        case TYPEOF_s0_t: st_s_copyf( o, "%"PRIi8 , *(const s0_t*)src ); break;
        case TYPEOF_s1_t: st_s_copyf( o, "%"PRIi16, *(const s1_t*)src ); break;
        case TYPEOF_s2_t: st_s_copyf( o, "%"PRIi32, *(const s2_t*)src ); break;
        case TYPEOF_s3_t: st_s_copyf( o, "%"PRIi64, *(const s3_t*)src ); break;
        case TYPEOF_u0_t: st_s_copyf( o, "%"PRIu8 , *(const u0_t*)src ); break;
        case TYPEOF_u1_t: st_s_copyf( o, "%"PRIu16, *(const u1_t*)src ); break;
        case TYPEOF_u2_t: st_s_copyf( o, "%"PRIu32, *(const u2_t*)src ); break;
        case TYPEOF_u3_t: st_s_copyf( o, "%"PRIu64, *(const u3_t*)src ); break;
        case TYPEOF_f2_t: st_s_copyf( o, "%g",      *(const f2_t*)src ); break;
        case TYPEOF_f3_t: st_s_copyf( o, "%lg",     *(const f3_t*)src ); break;
        case TYPEOF_sz_t: st_s_copyf( o, "%ti",     *(const sz_t*)src ); break;
        case TYPEOF_uz_t: st_s_copyf( o, "%zu",     *(const uz_t*)src ); break;
        case TYPEOF_vd_t: st_s_copyf( o, "%p",      *(const vd_t*)src ); break;
        case TYPEOF_vc_t: st_s_copyf( o, "%p",      *(const vc_t*)src ); break;

        case TYPEOF_smax_t:   st_s_copyf( o, "%"PRIiMAX, *(const smax_t*)src ); break;
        case TYPEOF_umax_t:   st_s_copyf( o, "%"PRIuMAX, *(const umax_t*)src ); break;
        case TYPEOF_offset_t: st_s_copyf( o, "%zu",      *(const offset_t*)src ); break;

        case TYPEOF_bl_t: st_s_copy_sc( o, *(const bl_t*)src ? "true" : "false" ); break;
        case TYPEOF_tp_t:
        case TYPEOF_aware_t:
        {
            uintmax_t val = *(const tp_t*)src;
            st_s_copyf( o, "%"PRIuMAX, val );
        }
        break;

        default:
        {
            ERR( "Converting type '%s' into 'st_s' is not supported.", ifnameof( type ) );
        }
        break;
    }
}

void st_s_copy_aware( st_s* o, vc_t src )
{
    st_s_copy_typed( o, *( aware_t* )src, src );
}

void st_s_move( st_s* o, st_s* src )
{
    st_s_init( o );
    st_s_copy( o, src );
    st_s_down( src );
}

st_s* st_s_create()
{
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    st_s_init( o );
    return o;
}

st_s* st_s_createvf( sc_t format, va_list args )
{
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    st_s_initvf( o, format, args );
    return o;
}

st_s* st_s_createf( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s* o = st_s_createvf( format, args );
    va_end( args );
    return o;
}

st_s* st_s_create_fv( sc_t format, va_list args )
{
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    st_s_init_fv( o, format, args );
    return o;
}

st_s* st_s_create_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s* o = st_s_create_fv( format, args );
    va_end( args );
    return o;
}

st_s* st_s_create_sc_n( sc_t sc, uz_t sc_size )
{
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    st_s_init_sc_n( o, sc, sc_size );
    return o;
}

st_s* st_s_create_sd_d( sd_t sd )
{
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    st_s_init_sd_d( o, sd );
    return o;
}

st_s* st_s_create_sc( sc_t sc )
{
    return st_s_create_sc_n( sc, bcore_strlen( sc ) );
}

st_s* st_s_create_weak_sc( sc_t sc )
{
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    st_s_init_weak_sc( o, sc );
    return o;
}

st_s* st_s_create_l( bcore_life_s* life )
{
    return bcore_life_s_push( life, ( bcore_fp_discard )st_s_discard, st_s_create() );
}

st_s* st_s_createf_l( bcore_life_s* life, sc_t format, ... )
{
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    va_list args;
    va_start( args, format );
    st_s_initvf( o, format, args );
    va_end( args );
    return bcore_life_s_push( life, ( bcore_fp_discard )st_s_discard, o );
}

st_s* st_s_create_l_sc_n( bcore_life_s* life, sc_t sc, uz_t sc_size )
{
    return bcore_life_s_push( life, ( bcore_fp_discard )st_s_discard, st_s_create_sc_n( sc, sc_size ) );
}

st_s* st_s_create_l_sc( bcore_life_s* life, sc_t sc )
{
    return bcore_life_s_push( life, ( bcore_fp_discard )st_s_discard, st_s_create_sc( sc ) );
}

st_s* st_s_create_typed( tp_t type, vc_t src )
{
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    st_s_init( o );
    st_s_copy_typed( o, type, src );
    return o;
}

st_s* st_s_create_aware( vc_t src )
{
    return st_s_create_typed( *( aware_t* )src, src );
}

bl_t st_s_is_weak( const st_s* o )
{
    return ( o->size > 0 ) && ( o->space == 0 );
}

void st_s_make_strong( st_s* o )
{
    if( !st_s_is_weak( o ) ) return;
    sc_t src = o->sc;
    o->size = 0;
    st_s_copy_sc( o, src );
}

void st_s_set_min_space( st_s* o, uz_t min_space )
{
    if( min_space == 0 ) return;
    st_s_make_strong( o );
    if( o->space == 0 )
    {
        uz_t new_space = min_space > 8 ? min_space : 8;
        o->data = bcore_bn_alloc( NULL, 0, new_space, &o->space );
        o->data[ o->size ] = 0;
    }
    else if( o->space < min_space )
    {
        uz_t new_space = ( min_space > o->space * 2 ) ? min_space : o->space * 2;
        o->data = bcore_bn_alloc( o->data, o->space, new_space, &o->space );
    }
}

void st_s_discard( st_s* o )
{
    if( !o ) return;
    bcore_release_obj( ( fp_t )st_s_down, o );
}

st_s* st_s_clone( const st_s* o )
{
    st_s* o_l = st_s_create();
    st_s_copy( o_l, o );
    return o_l;
}

st_s* st_s_crop( const st_s* o, uz_t start, uz_t end )
{
    st_s* s = st_s_create();
    if( start >= o->size ) return s;
    uz_t end_l = end < o->size ? end : o->size;
    if( end_l <= start ) return s;
    s->space = end_l - start + 1;
    if( s->space < 8 ) s->space = 8;
    s->data = bcore_b_alloc( NULL, s->space, &s->space );
    s->size = end_l - start;
    bcore_memcpy( s->data, o->data + start, s->size );
    s->data[ s->size ] = 0;
    return s;
}

st_s* st_s_crop_d( st_s* o, uz_t start, uz_t end )
{
    st_s* s = st_s_crop( o, start, end );
    st_s_discard( o );
    return s;
}

st_s* st_s_clear( st_s* o )
{
    if( o->size > 0 )
    {
        if( o->space > 0 )
        {
            o->data[ 0 ] = 0;
        }
        else
        {
            o->sc = "";
        }
        o->size = 0;
    }
    return o;
}

st_s* st_s_push_char( st_s* o, char c )
{
    if( o->space == 0 ) st_s_make_strong( o );
    if( o->space == 0 )
    {
        o->space = 8;
        o->data = bcore_b_alloc( NULL, o->space, &o->space );
    }
    else if( o->space < o->size + 2 )
    {
        o->data = bcore_bn_alloc( o->data, o->space, o->space * 2, &o->space );
    }
    o->data[ o->size ] = c;
    o->size++;
    o->data[ o->size ] = 0;
    return o;
}

st_s* st_s_push_char_n( st_s* o, char c, uz_t n )
{
    st_s_set_min_space( o, o->size + n + 1 );
    bcore_memset( o->data + o->size, c, n );
    o->size += n;
    o->data[ o->size ] = 0;
    return o;
}

char st_s_pop_char( st_s* o )
{
    if( o->size  == 0 ) return 0;
    if( o->space == 0 ) st_s_make_strong( o );
    o->size--;
    char c = o->data[ o->size ];
    o->data[ o->size ] = 0;
    return c;
}

void st_s_pop_n( st_s* o, uz_t n )
{
    if( o->size  == 0 ) return;
    if( o->space == 0 ) st_s_make_strong( o );
    o->size = ( o->size > n ) ? ( o->size - n ) : 0;
    o->data[ o->size ] = 0;
}

st_s* st_s_push_st( st_s* o, const st_s* src )
{
    if( !src ) return o;
    if( o->size == 0 )
    {
        st_s_copy( o, src );
        return o;
    }
    if( o->space < o->size + src->size + 1 )
    {
        if( o->space == 0 ) st_s_make_strong( o );
        o->data = bcore_bn_alloc( o->data, o->space, o->size + src->size + 1, &o->space );
    }
    bcore_memcpy( o->data + o->size, src->data, src->size );
    o->size += src->size;
    o->data[ o->size ] = 0;
    return o;
}

st_s* st_s_push_st_d( st_s* o, st_s* src )
{
    st_s_push_st( o, src );
    st_s_discard( src );
    return o;
}

st_s* st_s_push_sc_n( st_s* o, sc_t sc, uz_t sc_size )
{
    if( o->space < o->size + sc_size + 1 )
    {
        if( o->space == 0 ) st_s_make_strong( o );
        o->data = bcore_bn_alloc( o->data, o->space, o->size + sc_size + 1, &o->space );
    }
    bcore_memcpy( o->data + o->size, sc, sc_size );
    o->size += sc_size;
    o->data[ o->size ] = 0;
    return o;
}

st_s* st_s_push_sc( st_s* o, sc_t sc )
{
    return st_s_push_sc_n( o, sc, bcore_strlen( sc ) );
}

st_s* st_s_push_typed( st_s* o, tp_t type, vc_t src )
{
    st_s* s = st_s_create();
    st_s_copy_typed( s, type, src );
    return st_s_push_st_d( o, s );
}

st_s* st_s_pushvf( st_s* o, sc_t format, va_list args )
{
    st_s s;
    st_s_initvf( &s, format, args );
    st_s_push_st( o, &s );
    st_s_down( &s );
    return o;
}

st_s* st_s_pushf( st_s* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s_pushvf( o, format, args );
    va_end( args );
    return o;
}

st_s* st_s_push_fv( st_s* o, sc_t format, va_list args )
{
    st_s s;
    st_s_init_fv( &s, format, args );
    st_s_push_st( o, &s );
    st_s_down( &s );
    return o;
}

st_s* st_s_push_fa( st_s* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s_push_fv( o, format, args );
    va_end( args );
    return o;
}

uz_t st_s_find_char( const st_s* o, uz_t start, uz_t end, char c )
{
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ ) if( o->data[ j ] == c ) return j;
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == c ) return j;
        return start_l;
    }
}

uz_t st_s_find_sc( const st_s* o, uz_t start, uz_t end, sc_t find_sc )
{
    if( !find_sc || find_sc[ 0 ] == 0 ) return start;

    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ )
        {
            if( o->data[ j ] == find_sc[ 0 ] )
            {
                int m = bcore_strcmp( find_sc, &o->data[ j ] );
                if( m == 0 || m == 1 ) return j;
            }
        }
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1;  j < start_l && j >= end; j-- )
        {
            if( o->data[ j ] == find_sc[ 0 ] )
            {
                int m = bcore_strcmp( find_sc, &o->data[ j ] );
                if( m == 0 || m == 1 ) return j;
            }
        }
        return start_l;
    }
}

uz_t st_s_find_st( const st_s* o, uz_t start, uz_t end, const st_s* st )
{
    return st_s_find_sc( o, start, end, st->sc );
}

uz_t st_s_find_st_d( const st_s* o, uz_t start, uz_t end, st_s* st )
{
    uz_t r = st_s_find_st( o, start, end, st );
    st_s_discard( st );
    return r;
}

uz_t st_s_find_any_sc( const st_s* o, uz_t start, uz_t end, sc_t sc )
{
    if( !sc || sc[ 0 ] == 0 ) return start;
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ ) if( bcore_strany( o->data[ j ], sc ) ) return j;
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1; j < start_l && j >= end; j-- ) if( bcore_strany( o->data[ j ], sc ) ) return j;
        return start_l;
    }
}

uz_t st_s_find_none_sc( const st_s* o, uz_t start, uz_t end, sc_t sc )
{
    if( !sc || sc[ 0 ] == 0 ) return start;
    if( end >= start )
    {
        uz_t end_l = end < o->size ? end : o->size;
        for( uz_t j = start; j < end_l; j++ ) if( !bcore_strany( o->data[ j ], sc ) ) return j;
        return end_l;
    }
    else
    {
        uz_t start_l = start < o->size ? start : o->size;
        for( uz_t j = start_l - 1; j < start_l && j >= end; j-- ) if( !bcore_strany( o->data[ j ], sc ) ) return j;
        return start_l;
    }
}

st_s* st_s_remove( st_s* o, uz_t start, uz_t size )
{
    if( start >= o->size ) return o;
    if( o->space == 0 ) st_s_make_strong( o );
    uz_t size_l = start + size > o->size ? o->size - start : size;
    bcore_memmove( o->data + start, o->data + start + size_l, o->size - start - size_l );
    o->size -= size_l;
    o->data[ o->size ] = 0;
    return o;
}

uz_t st_s_count_char( const st_s* o, uz_t start, uz_t end, char c )
{
    uz_t count = 0;
    uz_t end_l = end < o->size ? end : o->size;
    for( uz_t j = start; j < end_l; j++ ) if( o->data[ j ] == c ) count++;
    return count;
}

uz_t st_s_count_sc( const st_s* o, uz_t start, uz_t end, sc_t sc )
{
    uz_t count = 0;
    if( !sc || sc[ 0 ] == 0 ) return 0;

    uz_t end_l = end < o->size ? end : o->size;
    for( uz_t j = start; j < end_l; j++ )
    {
        if( o->data[ j ] == sc[ 0 ] )
        {
            int m = bcore_strcmp( sc, &o->data[ j ] );
            if( m == 0 || m == 1 ) count++;
        }
    }
    return count;
}

uz_t st_s_count_st( const st_s* o, uz_t start, uz_t end, const st_s* st )
{
    return st_s_count_sc( o, start, end, st->sc );
}

st_s* st_s_insert_char( st_s* o, uz_t start, char c )
{
    if( start > o->size ) return o;
    if( o->space == 0 ) st_s_make_strong( o );
    if( o->space == 0 )
    {
        o->space = 8;
        o->data = bcore_b_alloc( NULL, o->space, &o->space );
    }
    else if( o->space < o->size + 2 )
    {
        o->data = bcore_bn_alloc( o->data, o->space, o->space * 2, &o->space );
    }
    bcore_memmove( o->data + start + 1, o->data + start, o->size - start );
    o->data[ start ] = c;
    o->size++;
    o->data[ o->size ] = 0;
    return o;
}

st_s* st_s_insert_st( st_s* o, uz_t start, const st_s* string )
{
    if( o == string )
    {
        st_s* string_l = st_s_clone( string );
        st_s_insert_st( o, start, string_l );
        st_s_discard( string_l );
        return o;
    }
    if( start > o->size ) return o;
    if( o->space == 0 ) st_s_make_strong( o );
    if( o->space == 0 )
    {
        st_s_copy( o, string );
        return o;
    }
    if( o->space < o->size + string->size + 1 )
    {
        o->data = bcore_bn_alloc( o->data, o->space, o->size + string->size + 1, &o->space );
    }
    bcore_memmove( o->data + start + string->size, o->data + start, o->size - start );
    bcore_memcpy(  o->data + start, string->sc, string->size );
    o->size += string->size;
    o->data[ o->size ] = 0;
    return o;
}

st_s* st_s_insert_st_d( st_s* o, uz_t start, st_s* string )
{
    st_s_insert_st( o, start, string );
    st_s_discard( string );
    return o;
}

st_s* st_s_insert_sc( st_s* o, uz_t start, sc_t sc )
{
    if( start > o->size ) return o;
    if( o->space == 0 ) st_s_make_strong( o );
    if( o->space == 0 )
    {
        st_s_copy_sc( o, sc );
        return o;
    }

    uz_t src_size = bcore_strlen( sc );
    if( o->space < o->size + src_size + 1 )
    {
        o->data = bcore_bn_alloc( o->data, o->space, o->size + src_size + 1, &o->space );
    }
    bcore_memmove( o->data + start + src_size, o->data + start, o->size - start );
    bcore_memcpy(  o->data + start, sc, src_size );
    o->size += src_size;
    o->data[ o->size ] = 0;
    return o;
}

st_s* st_s_replace_char( st_s* o, uz_t start, char c )
{
    if( start >= o->size ) return o;
    if( o->space == 0 ) st_s_make_strong( o );
    o->data[ start ] = c;
    return o;
}

st_s* st_s_replace_st( st_s* o, uz_t start, const st_s* string )
{
    if( o == string )
    {
        st_s* string_l = st_s_clone( string );
        st_s_replace_st( o, start, string_l );
        st_s_discard( string_l );
        return o;
    }
    if( start >= o->size ) return o;
    if( o->space == 0 ) st_s_make_strong( o );
    for( uz_t i = 0; i < string->size; i++ )
    {
        uz_t idx = i + start;
        if( idx == o->size ) break;
        o->data[ idx ] = string->data[ i ];
    }
    return o;
}

st_s* st_s_replace_st_d( st_s* o, uz_t start, st_s* string )
{
    st_s_replace_st( o, start, string );
    st_s_discard( string );
    return o;
}

st_s* st_s_replace_sc( st_s* o, uz_t start, sc_t sc )
{
    if( start >= o->size ) return o;
    if( o->space == 0 ) st_s_make_strong( o );
    for( uz_t i = 0; *sc != 0; i++ )
    {
        uz_t idx = i + start;
        if( idx == o->size ) break;
        o->data[ idx ] = *sc++;
    }
    return o;
}

st_s* st_s_replace_char_char( st_s* o, char c, char replace )
{
    if( o->space == 0 ) st_s_make_strong( o );
    for( uz_t i = 0; i < o->size; i++ ) if( o->data[ i ] == c ) o->data[ i ] = replace;
    return o;
}

st_s* st_s_replace_char_sc( st_s* o, char c, sc_t sc )
{
    uz_t start = 0;
    uz_t sz_sc = bcore_strlen( sc );
    if( o->space == 0 ) st_s_make_strong( o );

    while( ( start = st_s_find_char( o, start, o->size, c ) ) < o->size )
    {
        st_s_remove(    o, start, 1 );
        st_s_insert_sc( o, start, sc );
        start += sz_sc;
    }
    return o;
}

st_s* st_s_replace_char_st( st_s* o, char c, const st_s* string )
{
    return st_s_replace_char_sc( o, c, string->sc );
}

st_s* st_s_replace_char_st_d( st_s* o, char c, st_s* string )
{
    st_s_replace_char_sc( o, c, string->sc );
    st_s_discard( string );
    return o;
}

st_s* st_s_replace_sc_sc( st_s* o, sc_t match, sc_t replace )
{
    uz_t start = 0;
    uz_t sz_match = bcore_strlen( match );
    uz_t sz_replace = bcore_strlen( replace );
    if( o->space == 0 ) st_s_make_strong( o );
    while( ( start = st_s_find_sc( o, start, o->size, match ) ) < o->size )
    {
        st_s_remove(    o, start, sz_match );
        st_s_insert_sc( o, start, replace );
        start += sz_replace;
    }
    return o;
}

st_s* st_s_replace_st_st( st_s* o, const st_s* match, const st_s* replace )
{
    return st_s_replace_sc_sc( o, match->sc, replace->sc );
}

st_s* st_s_replace_st_d_st_d( st_s* o, st_s* match, st_s* replace )
{
    st_s_replace_st_st( o, match, replace );
    st_s_discard( match );
    st_s_discard( replace );
    return o;
}

void st_s_set_lowercase( st_s* o )
{
    if( o->space == 0 ) st_s_make_strong( o );
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->data[ i ] >= 'A' && o->data[ i ] <= 'Z' ) o->data[ i ] += 'a' - 'A';
    }
}

void st_s_set_uppercase( st_s* o )
{
    if( o->space == 0 ) st_s_make_strong( o );
    for( uz_t i = 0; i < o->size; i++ )
    {
        if( o->data[ i ] >= 'a' && o->data[ i ] <= 'z' ) o->data[ i ] += 'A' - 'a';
    }
}

uz_t st_s_lineof( const st_s* o, uz_t pos )
{
    return st_s_count_char( o, 0, pos, '\n' ) + 1;
}

uz_t st_s_colof( const st_s* o, uz_t pos )
{
    uz_t pos_l = pos < o->size ? pos : o->size;
    uz_t count = 0;
    while( pos_l > 0 )
    {
        if( o->data[ pos_l - 1 ] == '\n' ) break;
        count++;
        pos_l--;
    }
    return count + 1;
}

uz_t st_s_posof( const st_s* o, uz_t line, uz_t col )
{
    if( line == 0 ) return 0;
    uz_t pos = 0;
    uz_t count = 1;
    if( line > 1 )
    {
        for( pos = 0; pos < o->size; pos++ )
        {
            if( o->data[ pos ] == '\n' )
            {
                count++;
                if( count == line ) break;
            }
        }
    }
    return pos + col;
}

uz_t st_s_posofline( const st_s* o, uz_t pos )
{
    uz_t pos_l = ( pos > o->size ) ? o->size : pos;
    while( pos_l > 0 )
    {
        if( o->data[ pos_l - 1 ] == '\n' ) break;
        pos_l--;
    }
    return pos_l;
}


void st_s_to_stdout( const st_s* o )
{
    if( !o ) return;
    bcore_msg( "%s", o->sc );
}

void st_s_to_stdout_d( st_s* o )
{
    if( !o ) return;
    st_s_to_stdout( o );
    st_s_discard( o );
}

void st_s_to_stdout_fv( sc_t format, va_list args )
{
    st_s_to_stdout_d( st_s_create_fv( format, args ) );
}

void st_s_to_stdout_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s_to_stdout_fv( format, args );
    va_end( args );
}


void st_s_print( const st_s* o )
{
    if( !o ) return;
    bcore_msg( "%s", o->sc );
}

void st_s_print_d( st_s* o )
{
    if( !o ) return;
    st_s_print( o );
    st_s_discard( o );
}

void st_s_print_fv( sc_t format, va_list args )
{
    st_s_print_d( st_s_create_fv( format, args ) );
}

void st_s_print_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s_print_fv( format, args );
    va_end( args );
}

st_s* st_s_show_line_context( const st_s* o, uz_t pos )
{
    uz_t start = st_s_posofline( o, pos );
    uz_t end = st_s_find_char( o, pos, o->size, '\n' );
    st_s* s = st_s_create();
    st_s_push_st_d( s, st_s_crop( o, start, end ) );
    st_s_push_char( s, '\n' );
    st_s_push_char_n( s, ' ', pos - start );
    st_s_push_char(   s, '^' );
    st_s_push_char( s, '\n' );
    return s;
}

/** Expects character expression and returns resulting character.
 *  Either '<char>' for const character or [<index>] for indexed character.
 *  Returns ETX (value 3) (end of text) when index is out of range.
 */
static char c_char( sc_t o, uz_t on, sc_t f, uz_t* fi )
{
    uz_t i = *fi;
    char c = 3;  // ETX (end of text) when index is out of range
    if( f[ i ] == '\'' )
    {
        c = f[ ++i ];
        if( !f[ i ] || f[ ++i ] != '\'' ) ERR( "Format '%s' at position %zu: '<char>' expected.", f, i - 2 );
        i++;
    }
    else if( f[ i ] == '[' )
    {
        uz_t i0 = i;
        i++;
        int idx = atoi( f + i );
        if( idx >= 0 && idx < on ) c = o[ idx ];
        while( f[ i ] && f[ i ] != ']' ) i++;
        if( !f[ i ] || f[ i ] != ']' ) ERR( "Format '%s' at position %zu: [<index>] expected.", f, i0 );
        i++;
    }
    else
    {
        ERR( "Format '%s' at position %zu: '<char>' or [<index>] expected.", f, i );
    }
    *fi = i;
    return c;
}

/** evaluates logical match condition for characters
 * format "([0]=='a'&&[1]=='a'...)" (no whitespaces).
 */
static bl_t c_match( sc_t o, uz_t on, sc_t f, uz_t* fi )
{
    if( f[ *fi ] == '\'' || f[ *fi ] == '[' )
    {
        uz_t i = *fi;
        char c1 = c_char( o, on, f, &i );
        uz_t i0 = i;
        char rc1 = f[ i ];
        char rc2 = f[ i + 1 ];
        if( rc1 ) i += ( rc2 == '=' ) ? 2 : 1;
        char c2 = c_char( o, on, f, &i );
        *fi = i;
        switch( rc1 )
        {
            case '=': return ( rc2 == '=' ) ? ( c1 == c2 ) : false;
            case '>': return ( rc2 == '=' ) ? ( c1 >= c2 ) : ( c1 > c2 );
            case '<': return ( rc2 == '=' ) ? ( c1 <= c2 ) : ( c1 < c2 );
            case '!': return ( rc2 == '=' ) ? ( c1 != c2 ) : false;
            default: ERR( "Format '%s' at position %zu: relation expected.", f, i0 );
        }
    }
    else if( f[ *fi ] == '(' )
    {
        uz_t i = *fi + 1;
        bl_t flag = c_match( o, on, f, &i );
        while( f[ i ] && f[ i ] != ')' )
        {
            if( f[ i ] == '&' && f[ i + 1 ] == '&' )
            {
                i+= 2;
                bl_t flag2 = c_match( o, on, f,  &i );
                flag = flag && flag2;
            }
            else if( f[ i ] == '|' && f[ i + 1 ] == '|' )
            {
                i+= 2;
                bl_t flag2 = c_match( o, on, f,  &i );
                flag = flag || flag2;
            }
            else
            {
                ERR( "Format '%s' at position %zu: logical operator (&& or ||) expected.", f, i );
            }
        }
        if( f[ i ] != ')' ) ERR( "Format '%s' at position %zu: ) expected.", f, i );
        i++;
        *fi = i;
        return flag;
    }
    else if( f[ *fi ] == '!' )
    {
        ( *fi )++;
        return !c_match( o, on, f, fi );
    }
    else
    {
        ERR( "Format '%s' at position %zu: invalid expression.", f, *fi );
    }
    return false;
}

uz_t st_s_parse_err( vd_t arg, const st_s* o, uz_t idx, st_s* msg )
{
    ERR( "Parse error at (%zu:%zu):\n%s\n%s\n", st_s_lineof( o, idx ), st_s_colof( o, idx ), st_s_show_line_context( o, idx )->sc, msg->sc );
    return idx;
}

uz_t st_s_parse_msg( vd_t arg, const st_s* o, uz_t idx, st_s* msg )
{
    bcore_msg( "At (%zu:%zu):\n%s\n%s\n", st_s_lineof( o, idx ), st_s_colof( o, idx ), st_s_show_line_context( o, idx )->sc, msg->sc );
    return idx;
}

uz_t st_s_parse_efv( const st_s* o, uz_t start, uz_t end, fp_st_s_parse_err errfp, vd_t arg, sc_t format, va_list args )
{
    uz_t end_l = end < o->size ? end : o->size;
    uz_t idx = start;

    sc_t fp = format;
    while( *fp )
    {
        // skip whitespaces
        if( *fp == ' ' || *fp == '\n' || *fp == '\t' )
        {
            fp++;
            while( idx < end_l )
            {
                char c = o->data[ idx ];
                // common whitespaces
                if( c == ' ' || c == '\t' || c == '\r' || c == '\n' )
                {
                    idx++;
                }
                // c-style comments
                else if( ( c == '/' ) && ( idx < end_l - 1 ) )
                {
                    if( o->data[ idx + 1 ] == '/' )
                    {
                        idx += 2;
                        while(   ( idx < end_l ) && ( o->data[ idx ] != '\n' ) ) idx++;
                        idx += ( ( idx < end_l ) && ( o->data[ idx ] == '\n' ) );
                    }
                    else if( o->data[ idx + 1 ] == '*'  )
                    {
                        idx += 2;
                        while( ( idx < end_l - 1 ) && !( ( o->data[ idx ] == '*' ) && ( o->data[ idx + 1 ] == '/' ) ) ) idx++;
                        idx += 2 * ( ( o->data[ idx ] == '*' ) && ( o->data[ idx + 1 ] == '/' ) );
                    }
                    else
                    {
                        break;
                    }
                }
                // '#!' is treated as line-comment to allow the use of posix shebang
                else if( ( c == '#' ) && ( idx < end_l - 1 ) && o->data[ idx + 1 ] == '!' )
                {
                    idx += 2;
                    while(   ( idx < end_l ) && ( o->data[ idx ] != '\n' ) ) idx++;
                    idx += ( ( idx < end_l ) && ( o->data[ idx ] == '\n' ) );
                }
                else
                {
                    break;
                }
            }
        }
        else if( *fp == '#' )
        {
            fp++;

            uz_t enter_idx = idx;

            bl_t cat_arg = false; // append evaluation result to arg (used for strings)
            bl_t set_arg = true;
            bl_t consume = true;
            for( bl_t loop = true; loop;  )
            {
                switch( *fp )
                {
                    case '=': consume = false; fp++; break;
                    case '-': set_arg = false; fp++; break;
                    case ':': cat_arg = true;  fp++; break;
                    default : loop    = false; break;
                }
            }

            tp_t type = 0;
            int fp_inc = 0;
            bl_t is_ptr = true;
            if( ( fp_inc = sc_t_scan_type( fp, &type, &is_ptr ) ) >= 0 )
            {
                if( !is_ptr ) ERR( "Type specifier '%s' must be a pointer. Use syntax <...*>.", fp );
                fp += fp_inc;
                int size = 0;
                int sres = -1;
                switch( type )
                {
                    case TYPEOF_char:
                    {
                        u0_t v;
                        sres = sscanf( o->sc + idx, "%c%n", &v, &size );
                        if( set_arg ) *va_arg( args, char* ) = v;
                        break;
                    }

                    case TYPEOF_u0_t:
                    {
                        u0_t v;
                        sres = sscanf( o->sc + idx, "%"SCNu8"%n", &v, &size );
                        if( set_arg ) *va_arg( args, u0_t* ) = v;
                        break;
                    }

                    case TYPEOF_u1_t:
                    {
                        u1_t v;
                        sres = sscanf( o->sc + idx, "%"SCNu16"%n", &v, &size );
                        if( set_arg ) *va_arg( args, u1_t* ) = v;
                        break;
                    }

                    case TYPEOF_u2_t:
                    {
                        u2_t v;
                        sres = sscanf( o->sc + idx, "%"SCNu32"%n", &v, &size );
                        if( set_arg ) *va_arg( args, u2_t* ) = v;
                        break;
                    }

                    case TYPEOF_u3_t:
                    {
                        u3_t v;
                        sres = sscanf( o->sc + idx, "%"SCNu64"%n", &v, &size );
                        if( set_arg ) *va_arg( args, u3_t* ) = v;
                        break;
                    }

                    case TYPEOF_umax_t:
                    {
                        umax_t v;
                        sres = sscanf( o->sc + idx, "%"SCNuMAX"%n", &v, &size );
                        if( set_arg ) *va_arg( args, umax_t* ) = v;
                        break;
                    }

                    case TYPEOF_s0_t:
                    {
                        s0_t v;
                        sres = sscanf( o->sc + idx, "%"SCNi8"%n", &v, &size );
                        if( set_arg ) *va_arg( args, s0_t* ) = v;
                        break;
                    }

                    case TYPEOF_s1_t:
                    {
                        s1_t v;
                        sres = sscanf( o->sc + idx, "%"SCNi16"%n", &v, &size );
                        if( set_arg ) *va_arg( args, s1_t* ) = v;
                        break;
                    }

                    case TYPEOF_s2_t:
                    {
                        s2_t v;
                        sres = sscanf( o->sc + idx, "%"SCNi32"%n", &v, &size );
                        if( set_arg ) *va_arg( args, s2_t* ) = v;
                        break;
                    }

                    case TYPEOF_s3_t:
                    {
                        s3_t v;
                        sres = sscanf( o->sc + idx, "%"SCNi64"%n", &v, &size );
                        if( set_arg ) *va_arg( args, s3_t* ) = v;
                        break;
                    }

                    case TYPEOF_smax_t:
                    {
                        smax_t v;
                        sres = sscanf( o->sc + idx, "%"SCNiMAX"%n", &v, &size );
                        if( set_arg ) *va_arg( args, smax_t* ) = v;
                        break;
                    }

                    case TYPEOF_sz_t:
                    {
                        smax_t v;
                        sres = sscanf( o->sc + idx, "%"SCNiMAX"%n", &v, &size );
                        if( set_arg ) *va_arg( args, sz_t* ) = v;
                        break;
                    }

                    case TYPEOF_uz_t:
                    {
                        umax_t v;
                        sres = sscanf( o->sc + idx, "%"SCNuMAX"%n", &v, &size );
                        if( set_arg ) *va_arg( args, uz_t* ) = v;
                        break;
                    }

                    case TYPEOF_offset_t:
                    {
                        umax_t v;
                        sres = sscanf( o->sc + idx, "%"SCNuMAX"%n", &v, &size );
                        if( set_arg ) *va_arg( args, offset_t* ) = v;
                        break;
                    }

                    case TYPEOF_f2_t:
                    {
                        f2_t v;
                        sres = sscanf( o->sc + idx, "%g%n", &v, &size );
                        if( set_arg ) *va_arg( args, f2_t* ) = v;
                        break;
                    }

                    case TYPEOF_f3_t:
                    {
                        f3_t v;
                        sres = sscanf( o->sc + idx, "%lg%n", &v, &size );
                        if( set_arg ) *va_arg( args, f3_t* ) = v;
                        break;
                    }

                    case TYPEOF_tp_t:
                    {
                        uintmax_t v = 0;
                        sres = sscanf( o->sc + idx, "%"SCNuMAX"%n", &v, &size );
                        if( set_arg ) *va_arg( args, tp_t* ) = v;
                        break;
                    }

                    case TYPEOF_bl_t:
                    {
                        bl_t val = false;
                        if( ( bcore_strcmp( "true", o->sc + idx ) >> 1 ) == 0 )
                        {
                            val = true;
                            idx += strlen( "true" );
                        }
                        else if( ( bcore_strcmp( "false", o->sc + idx ) >> 1 ) == 0 )
                        {
                            val = false;
                            idx += strlen( "false" );
                        }
                        else
                        {
                            return errfp( arg, o, idx, st_s_createf( "Parsing #bl_t failed; true or false expected." ) );
                        }
                        if( set_arg ) *va_arg( args, bl_t* ) = val;
                        size = 0;
                        sres = 1;
                    }
                    break;

                    default:
                    {
                        ERR( "Unknown format type specifier: '%s'.", fp );
                    }
                    break;
                }

                if( sres <= 0 )
                {
                    return errfp( arg, o, idx, st_s_createf( "Parsing type #%s failed.", ifnameof( type ) ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( *fp == '?' )
            {
                sc_t fp0 = fp;
                fp++;
                bl_t flag = false;
                bl_t invert = false;
                if( *fp == '!' )
                {
                    fp++;
                    invert = true;
                }
                if( *fp == '(' )
                {
                    uz_t i = 0;
                    flag = c_match( &o->sc[ idx ], end_l - idx, fp, &i );
                    fp += i;
                }
                else
                {
                    bl_t word = false;
                    if( *fp == 'w' )
                    {
                        fp++;
                        word = true;
                    }

                    uz_t idx0 = idx;
                    if( *fp )
                    {
                        char term = *fp++;
                        flag = true;
                        while( *fp != term && *fp != 0 )
                        {
                            if( flag )
                            {
                                flag = *fp == o->sc[ idx ];
                                idx += ( idx < end_l );
                            }
                            fp++;
                        }
                        fp += ( *fp == term );
                        if( flag && word )
                        {
                            if( ( idx < end_l ) )
                            {
                                char c = o->sc[ idx ];
                                if(      c >= 'A' && c <= 'Z' ) flag = false;
                                else if( c >= 'a' && c <= 'z' ) flag = false;
                                else if( c >= '0' && c <= '9' ) flag = false;
                                else if( c == '_'             ) flag = false;
                            }
                        }
                        if( !flag ) idx = idx0;
                    }
                    else
                    {
                        ERR( "Unknown format directive: '%s'.", fp0 );
                    }
                }
                if( invert ) flag = !flag;
                if( set_arg ) *va_arg( args, bl_t* ) = flag;
            }
            else if( *fp == '#' )
            {
                if( '#' == o->sc[ idx ] )
                {
                    idx++;
                    fp++;
                }
                else
                {
                    return errfp( arg, o, idx, st_s_createf( "'#' expected." ) );
                }
            }
            else if( ( bcore_strcmp( "aware_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "aware_t" );
                int size = 0;
                uintmax_t val;
                if( sscanf( o->sc + idx, "%"SCNuMAX"%n", &val, &size ) <= 0 )
                {
                    return errfp( arg, o, idx, st_s_createf( "Parsing #aware_t failed." ) );
                }
                if( set_arg ) *va_arg( args, aware_t* ) = val;
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "bool", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "bool" );
                bool val = false;
                if( ( bcore_strcmp( "true", o->sc + idx ) >> 1 ) == 0 )
                {
                    val = true;
                    idx += strlen( "true" );
                }
                else if( ( bcore_strcmp( "false", o->sc + idx ) >> 1 ) == 0 )
                {
                    val = false;
                    idx += strlen( "false" );
                }
                else
                {
                    return errfp( arg, o, idx, st_s_createf( "Parsing #bool failed: true or false expected." ) );
                }
                if( set_arg ) *va_arg( args, bool* ) = val;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "char", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "char" );
                u0_t c = o->sc[ idx ];
                if( idx < end_l ) idx++;
                if( set_arg ) *va_arg( args, u0_t* ) = c;
            }
            else if( ( bcore_strcmp( "name", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "name" );
                st_s* string = NULL;
                if( set_arg ) { string = va_arg( args, st_s* ); if( !cat_arg ) st_s_clear( string ); }
                char c = o->sc[ idx ];
                if( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) || c == '_' )
                {
                    if( idx < end_l )
                    {
                        idx++;
                        if( set_arg ) st_s_push_char( string, c );
                        for(;;)
                        {
                            c = o->sc[ idx ];
                            if( idx < end_l && ( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) || ( c >= '0' && c <= '9' ) || c == '_' ) )
                            {
                                if( set_arg ) st_s_push_char( string, c );
                                idx++;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                }
            }
            else if( ( bcore_strcmp( "string", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "string" );
                st_s* string = NULL;
                if( set_arg ) { string = va_arg( args, st_s* ); if( !cat_arg ) st_s_clear( string ); }
                if( o->sc[ idx ] != '"' )
                {
                    return errfp( arg, o, idx, st_s_createf( "'\"' expected." ) );
                }
                idx++;
                while ( o->sc[ idx ] != '"' )
                {
                    if( o->sc[ idx ] == '\\' && o->sc[ idx + 1 ] == '\"' )
                    {
                        if( set_arg ) st_s_push_char( string, '\"' );
                        idx += 2;
                    }
                    else if( o->sc[ idx ] == '\\' && o->sc[ idx + 1 ] == '\\' )
                    {
                        if( set_arg ) st_s_push_char( string, '\\' );
                        idx += 2;
                    }
                    else
                    {
                        if( set_arg ) st_s_push_char( string, o->sc[ idx ] );
                        idx++;
                    }
                }
                idx++;
            }
            else if( ( bcore_strcmp( "skip", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "skip" );
                sc_t err_msg = "Format specifier 'skip' must be followed by character enclosed in single quotes ''";
                if( *fp++ != '\'' ) ERR( err_msg );
                char char_l = *fp++;
                if(  char_l == 0  ) ERR( err_msg );
                if( *fp++ != '\'' ) ERR( err_msg );
                while ( o->sc[ idx ] == char_l ) idx++;
            }
            else if( ( bcore_strcmp( "until", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "until" );
                sc_t err_msg = "Format specifier 'until' must be followed by character enclosed in single quotes ''";
                if( *fp++ != '\'' ) ERR( err_msg );
                char char_l = *fp++;
                if(  char_l == 0  ) ERR( err_msg );
                if( *fp++ != '\'' ) ERR( err_msg );
                st_s* string = NULL;
                if( set_arg ) { string = va_arg( args, st_s* ); if( !cat_arg ) st_s_clear( string ); }
                while ( o->sc[ idx ] != char_l )
                {
                    if( set_arg ) st_s_push_char( string, o->sc[ idx++ ] );
                }
            }
            else
            {
                ERR( "Unknown format directive: '%s'.", fp );
            }

            if( !consume ) idx = enter_idx;
        }
        else
        {
            sc_t fp0 = fp;
            if( *fp == o->sc[ idx ] )
            {
                idx++;
                fp++;
            }
            else
            {
                return errfp( arg, o, idx, st_s_createf( "Matching format characters '%s' failed", fp0 ) );
            }
        }
    }
    return idx;
}

uz_t st_s_parse_efa( const st_s* o, uz_t start, uz_t end, fp_st_s_parse_err errfp, vd_t arg, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    uz_t idx = st_s_parse_efv( o, start, end, errfp, arg, format, args );
    va_end( args );
    return idx;
}

uz_t st_s_parse_fv( const st_s* o, uz_t start, uz_t end, sc_t format, va_list args )
{
    return st_s_parse_efv( o, start, end, st_s_parse_err, NULL, format, args );
}

uz_t st_s_parse_fa( const st_s* o, uz_t start, uz_t end, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    uz_t idx = st_s_parse_fv( o, start, end, format, args );
    va_end( args );
    return idx;
}

static uz_t flow_snk( vd_t o, vc_t data, uz_t size )
{
    if( *( aware_t*)o != TYPEOF_st_s ) ERR( "object 'o' of type %u ('%s') must be st_s", *( aware_t*)o, ifnameof( *( aware_t*)o ) );
    st_s* s = o;
    if( s->space == 0 ) st_s_make_strong( s );
    if( s->space == 0 )
    {
        s->data = bcore_bn_alloc( NULL, 0, size < 7 ? 8 : size + 1, &s->space );
    }
    else if( s->space < s->size + size + 1 )
    {
        s->data = bcore_bn_alloc( s->data, s->space, ( s->space * 2 < s->size + size + 1 ) ? s->size + size + 1 : s->space * 2, &s->space );
    }
    bcore_memcpy( s->data + s->size, data, size );

    s->size += size;
    s->data[ s->size ] = 0;
    return size;
}

static uz_t flow_src( vd_t o, vd_t data, uz_t size )
{
    if( *( aware_t*)o != TYPEOF_st_s ) ERR( "object 'o' of type %u ('%s') must be st_s", *( aware_t*)o, ifnameof( *( aware_t*)o ) );
    st_s* s = o;
    if( s->space > 0 ) ERR( "String is strong. Only weak strings can be used as flow-source." );
    size = ( size > s->size ) ? s->size : size;
    bcore_memcpy( data, s->data, size );
    s->data += size;
    s->size -= size;
    return size;
}

static void p_errorvf( const st_s* o, sc_t format, va_list args )
{
    st_s* context = st_s_show_line_context( o, 0 );
    st_s* msg     = st_s_createvf( format, args );
    bcore_err( "st_s source parse error:\n%s%s\n", context->sc, msg->sc );
    st_s_discard( msg );
    st_s_discard( context );
}

static void parse_fv( st_s* o, sc_t format, va_list args )
{
    if( o->space > 0 ) ERR( "String is strong. Only weak strings can be used as flow-source." );
    uz_t size = st_s_parse_fv( o, 0, o->size, format, args );
    o->data += size;
    o->size -= size;
}

static bl_t eos( const st_s* o )
{
    if( o->space > 0 ) ERR( "String is strong. Only weak strings can be used as flow-source." );
    return o->size == 0;
}

static s3_t get_index( const st_s* o )
{
    if( o->space > 0 ) ERR( "String is strong. Only weak strings can be used as flow-source." );
    return -( s3_t )o->size;
}

static void set_index( st_s* o, s3_t index )
{
    if( o->space > 0 ) ERR( "String is strong. Only weak strings can be used as flow-source." );
    if( index > 0    ) ERR( "Index is out of range." );

    vd_t root = NULL;
    uz_t granted_space = 0;
    bcore_tbman_get_instance( o->data, &root, &granted_space );

    vd_t new_data = o->data + o->size + index;

    if( ( uz_t )( ptrdiff_t )( ( u0_t* )new_data - ( u0_t* )root ) >= granted_space )
    {
        ERR( "Index exceeds range of memory instance." );
    }

    o->data = new_data;
    o->size = -index;
}

/// sanity feature
void bcore_inst_t_check_sizeof( u2_t type, uz_t size );
static void check_sanity( vc_t o )
{
    bcore_inst_t_check_sizeof( TYPEOF_st_s, sizeof( st_s ) );

    if( TYPEOF_st_s != *(aware_t *)o ) ERR( "incorrect type value (%"PRIu32") (expected st_s)", *(aware_t *)o );
    const st_s* s = o;

    if( ( s->space > 0 ) && ( s->space < s->size ) ) ERR( "space (%zu) < size (%zu)", s->space, s->size );
    if( s->data[ s->size ] != 0 )  ERR( "data section does not terminate in zero" );

    for( uz_t i = 0; i < s->size; i++ )
    {
        u0_t c = s->data[ i ];
        if( c < 10 || c >= 128 ) ERR( "invalid character '%c' in string", c );
    }
}

static bcore_self_s* st_s_create_self( void )
{
//  bcore_self_s* self = bcore_self_s_build_parse_sc( "st_s = { aware_t _; private sd_t data; private uz_t size; private uz_t space; }", sizeof( st_s ) );

//  We need to create this reflection manually because bcore_self_s_build_parse_sc uses it.
    bcore_self_s* self = bcore_self_s_create_plain( entypeof( "st_s" ), typeof( "bcore_inst" ), sizeof( st_s ) );
    bcore_self_s_push_d( self, bcore_self_item_s_create_plain( BCORE_CAPS_SOLID_STATIC, TYPEOF_aware_t, entypeof( "_"  ) ) );
    bcore_self_s_push_d( self, bcore_self_item_s_create_plain( BCORE_CAPS_SOLID_STATIC, TYPEOF_sd_t, entypeof( "data"  ) ) )->flags.f_private = true;
    bcore_self_s_push_d( self, bcore_self_item_s_create_plain( BCORE_CAPS_SOLID_STATIC, TYPEOF_uz_t, entypeof( "size"  ) ) )->flags.f_private = true;
    bcore_self_s_push_d( self, bcore_self_item_s_create_plain( BCORE_CAPS_SOLID_STATIC, TYPEOF_uz_t, entypeof( "space" ) ) )->flags.f_private = true;

    bcore_self_s_push_ns_func( self, ( fp_t )st_s_init,         "bcore_fp_init",            "init"         );
    bcore_self_s_push_ns_func( self, ( fp_t )st_s_down,         "bcore_fp_down",            "down"         );
    bcore_self_s_push_ns_func( self, ( fp_t )st_s_copy,         "bcore_fp_copy",            "copy"         );
    bcore_self_s_push_ns_func( self, ( fp_t )st_s_move,         "bcore_fp_move",            "move"         );
    bcore_self_s_push_ns_func( self, ( fp_t )st_s_create,       "bcore_fp_create",          "create"       );
    bcore_self_s_push_ns_func( self, ( fp_t )st_s_clone,        "bcore_fp_clone",           "clone"        );
    bcore_self_s_push_ns_func( self, ( fp_t )st_s_discard,      "bcore_fp_discard",         "discard"      );
    bcore_self_s_push_ns_func( self, ( fp_t )st_s_copy_typed,   "bcore_fp_copy_typed",      "copy_typed"   );
    bcore_self_s_push_ns_func( self, ( fp_t )st_s_create_typed, "bcore_fp_create_typed",    "create_typed" );
    bcore_self_s_push_ns_func( self, ( fp_t )flow_snk,          "bcore_fp_flow_snk",        "flow_snk"     );
    bcore_self_s_push_ns_func( self, ( fp_t )flow_src,          "bcore_fp_flow_src",        "flow_src"     );
    bcore_self_s_push_ns_func( self, ( fp_t )p_errorvf,         "bcore_fp_logvf",           "p_errorvf"    );
    bcore_self_s_push_ns_func( self, ( fp_t )parse_fv,          "bcore_source_fp_parse_fv", "parse_fv"     );
    bcore_self_s_push_ns_func( self, ( fp_t )eos,               "bcore_source_fp_eos",      "eos"          );
    bcore_self_s_push_ns_func( self, ( fp_t )get_index,         "bcore_source_fp_get_index","get_index"    );
    bcore_self_s_push_ns_func( self, ( fp_t )set_index,         "bcore_source_fp_set_index","set_index"    );
    bcore_self_s_push_ns_func( self, ( fp_t )check_sanity,      "bcore_fp_check_sanity",    "check_sanity" );
    bcore_self_s_push_ns_func( self, ( fp_t )st_s_cmp_st,       "bcore_fp_compare",         "cmp_st"       );
    return self;
}

static void st_s_quicktest( void )
{
    uz_t granted_space = bcore_tbman_total_granted_space();

    bcore_life_s* life = bcore_life_s_create();
    st_s* s = bcore_life_s_push( life, ( bcore_fp_discard )st_s_discard, st_s_create() );

    ASSERT( s->size == 0 );
    ASSERT( s->space == 0 );
    st_s_copy_sc( s, "My little white elephant!" );
    ASSERT( st_s_equal_sc( s, "My little white elephant!" ) );
    st_s_replace_sc_sc( s, "white", "green" );
    ASSERT( st_s_equal_sc( s, "My little green elephant!" ) );

    bool is_green = false;
    bool is_blue  = false;
    st_s* name = bcore_life_s_push( life, ( bcore_fp_discard )st_s_discard, st_s_create() );
    uz_t idx = st_s_parse_fa( s, 0, s->size, "My #name #?w'blue' #?w'green' eleph", name, &is_blue, &is_green );
    ASSERT( st_s_equal_sc( name, "little" ) );
    ASSERT( is_green );
    ASSERT( !is_blue );
    ASSERT( bcore_strcmp( s->sc + idx, "ant!" ) == 0 );

    s2_t num0 = -12345;
    st_s_copy_sc( s, "This number is " );
    st_s_push_typed( s, typeof( "s2_t" ), &num0 );
    st_s_push_sc( s, " and an integer" );
    s3_t num1 = 0;
    idx = st_s_parse_fa( s, 0, s->size, "This number is #<s3_t*>", &num1 );
    ASSERT( num1 == -12345 );
    ASSERT( bcore_strcmp( s->sc + idx, " and an integer" ) == 0 );

    idx = st_s_parse_fa( s, 0, s->size, "This number is #s3_t", &num1 );
    ASSERT( num1 == -12345 );
    ASSERT( bcore_strcmp( s->sc + idx, " and an integer" ) == 0 );

    st_s* ws = bcore_life_s_push( life, ( bcore_fp_discard )st_s_discard, st_s_create() );
    st_s_assign_sc( ws, "This is a weak string" );

    ASSERT( st_s_cmp_sc( ws, "This is a weak string" ) == 0 );
    ASSERT( ws->space == 0 );

    // replacing data forces weak strings to become strong
    st_s_replace_sc_sc( ws, "weak", "strong" );
    ASSERT( st_s_cmp_sc( ws, "This is a strong string" ) == 0 );
    ASSERT( ws->space > 0 );

    // advanced formatting
    {
        s2_t v = -100;
        st_s_clear( s );
        st_s_push_fa( s, "abc#<s2_t>def", v );
        ASSERT( st_s_cmp_sc( s, "abc-100def" ) == 0 );
        st_s_clear( s );
        st_s_push_fa( s, "abc#s2_tdef", v );
        ASSERT( st_s_cmp_sc( s, "abc-100def" ) == 0 );
        st_s_clear( s );
        st_s_push_fa( s, "abc#<s2_t*>def", &v );
        ASSERT( st_s_cmp_sc( s, "abc-100def" ) == 0 );
        st_s_clear( s );
        st_s_push_fa( s, "#p10.{#<s2_t*>}\n", &v );
        ASSERT( st_s_cmp_sc( s, "-100......\n" ) == 0 );
        st_s_clear( s );
        st_s_push_fa( s, "#pl10.{#<s2_t*>}\n", &v );
        ASSERT( st_s_cmp_sc( s, "......-100\n" ) == 0 );
        st_s_clear( s );
        st_s_push_fa( s, "#p3.{#<s2_t*>}\n", &v );
        ASSERT( st_s_cmp_sc( s, "-100\n" ) == 0 );
        st_s_clear( s );
        st_s_push_fa( s, "#pl10.{#pn*{#<s2_t*>}}\n", ( uz_t )6, &v );
        ASSERT( st_s_cmp_sc( s, "....-100**\n" ) == 0 );
        st_s_clear( s );
        st_s_push_fa( s, "#r3{#<s2_t*>}\n", &v );
        ASSERT( st_s_cmp_sc( s, "-100-100-100\n" ) == 0 );
        st_s_clear( s );
        st_s_push_fa( s, "#t3{#<s2_t*>}\n", &v );
        ASSERT( st_s_cmp_sc( s, "-10\n" ) == 0 );
        st_s_clear( s );
        st_s_push_fa( s, "#tl3{#<s2_t*>}\n", &v );
        ASSERT( st_s_cmp_sc( s, "100\n" ) == 0 );
        st_s_clear( s );
        st_s_push_fa( s, "#tl5{#<s2_t*>}\n", &v );
        ASSERT( st_s_cmp_sc( s, "-100\n" ) == 0 );
    }

    // advanced parsing
    {
        st_s_clear( s );
        st_s_push_fa( s, "This is my string." );
        bl_t flag = false;
        uz_t idx;
        idx = st_s_parse_fa( s, 0, s->size, "This is #?([0]=='m'&&[3]=='s')my string.", &flag );
        ASSERT( idx == s->size );
        ASSERT( flag );
        idx = st_s_parse_fa( s, 0, s->size, "This is #?!([0]=='m'&&![3]=='k')my string.", &flag );
        ASSERT( idx == s->size );
        ASSERT( !flag );
        idx = st_s_parse_fa( s, 0, s->size, "This is #?([0]<=[3])my string.", &flag );
        ASSERT( idx == s->size );
        ASSERT( flag );
        idx = st_s_parse_fa( s, 0, s->size, "This is #?!([0]>='0'&&[0]<='9')my string.", &flag );
        ASSERT( idx == s->size );
        ASSERT( flag );
    }

    bcore_life_s_discard( life );

    ASSERT( granted_space == bcore_tbman_total_granted_space() );
}

/**********************************************************************************************************************/

vd_t bcore_st_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_st" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "st_s"  ), st_s_create_self  );
            st_s_quicktest();
        }
        break;

        case TYPEOF_selftest:
        {
            st_s_quicktest();
        }
        break;

        default: break;
    }
    return NULL;
}

/**********************************************************************************************************************/

