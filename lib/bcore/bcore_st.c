/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "stdio.h"
#include "bcore_st.h"
#include "bcore_control.h"
#include "bcore_tbman.h"
#include "bcore_name_manager.h"
#include "bcore_flect.h"
#include "bcore_quicktypes.h"
#include "bcore_life.h"

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
    o->space = vsnprintf( NULL, 0, format, args1 ) + 1;
    va_end( args1 );
    if( o->space < 8 ) o->space = 8;
    o->data = bcore_b_alloc( NULL, o->space, &o->space );
    vsnprintf( o->data, o->space, format, args );
    o->size = bcore_strlen( o->sc );
}

void st_s_initf( st_s* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s_initvf( o, format, args );
    va_end( args );
}

void st_s_init_sc( st_s* o, sc_t sc )
{
    st_s_init( o );
    sz_t src_size = bcore_strlen( sc );
    o->space = src_size < 8 ? 8 : src_size + 1;
    o->data  = bcore_b_alloc( NULL, o->space, &o->space );
    bcore_memcpy( o->data, sc, src_size );
    o->size = src_size;
    o->data[ o->size ] = 0;
}

void st_s_init_weak_sc( st_s* o, sc_t sc )
{
    st_s_init( o );
    sz_t src_size = bcore_strlen( sc );
    o->sc    = sc;
    o->size  = src_size;
}

void st_s_down( st_s* o )
{
    if( o->space ) bcore_bn_alloc( o->data, o->space, 0, &o->space );
    o->size = 0;
}

void st_s_copy( st_s* o, const st_s* src )
{
    if( o == src ) return;
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

void st_s_copy_sc( st_s* o, sc_t sc )
{
    sz_t src_size = bcore_strlen( sc );
    if( o->space <= src_size )
    {
        if( o->space > 0 ) bcore_bn_alloc( o->data, o->space, 0, &o->space );
        o->space = src_size < 8 ? 8 : src_size + 1;
        o->data  = bcore_b_alloc( NULL, o->space, &o->space );
    }
    bcore_memcpy( o->data, sc, src_size );
    o->size = src_size;
    o->data[ o->size ] = 0;
}

void st_s_assign_sc( st_s* o, sc_t sc )
{
    sz_t src_size = bcore_strlen( sc );
    if( o->space > 0 ) bcore_bn_alloc( o->data, o->space, 0, &o->space );
    o->sc   = sc;
    o->size = src_size;
}

void st_s_copyf( st_s* o, sc_t format, ...  )
{
    st_s_down( o );
    va_list args;
    va_start( args, format );
    st_s_initvf( o, format, args );
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
        case TYPEOF_sz_t: st_s_copyf( o, "%zu",     *(const sz_t*)src ); break;
        case TYPEOF_bl_t: st_s_copy_sc( o, *(const bl_t*)src ? "true" : "false" ); break;
        case TYPEOF_tp_t:
        case TYPEOF_aware_t:
        {
            // writing a text based name here is problematic because the corresponding parse feature expects a number
            uintmax_t val = *(const tp_t*)src;
            st_s_copyf( o, "%"PRIuMAX, val );
        }
        break;

        default:
        {
            ERR( "Converting type '%s' into st_s is not yet implemented", ifnameof( type ) );
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
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    va_list args;
    va_start( args, format );
    st_s_initvf( o, format, args );
    va_end( args );
    return o;
}

st_s* st_s_create_sc( sc_t sc )
{
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    st_s_init_sc( o, sc );
    return o;
}

st_s* st_s_create_weak_sc( sc_t sc )
{
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    st_s_init_weak_sc( o, sc );
    return o;
}

st_s* st_s_create_l( bcore_life_s* life )
{
    return bcore_life_s_push( life, st_s_discard, st_s_create() );
}

st_s* st_s_createf_l( bcore_life_s* life, sc_t format, ... )
{
    st_s* o = bcore_b_alloc( NULL, sizeof( st_s ), NULL );
    va_list args;
    va_start( args, format );
    st_s_initvf( o, format, args );
    va_end( args );
    return bcore_life_s_push( life, st_s_discard, o );
}

st_s* st_s_create_l_sc( bcore_life_s* life, sc_t sc )
{
    return bcore_life_s_push( life, st_s_discard, st_s_create_sc( sc ) );
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

void st_s_set_min_space( st_s* o, sz_t min_space )
{
    if( min_space == 0 ) return;
    st_s_make_strong( o );
    if( o->space == 0 )
    {
        sz_t new_space = min_space > 8 ? min_space : 8;
        o->data = bcore_bn_alloc( NULL, 0, new_space, &o->space );
        o->data[ o->size ] = 0;
    }
    else if( o->space < min_space )
    {
        sz_t new_space = ( min_space > o->space * 2 ) ? min_space : o->space * 2;
        o->data = bcore_bn_alloc( o->data, o->space, new_space, &o->space );
    }
}

void st_s_discard( st_s* o )
{
    if( !o ) return;
    bcore_release_obj( st_s_down, o );
}

st_s* st_s_clone( const st_s* o )
{
    st_s* o_l = st_s_create();
    st_s_copy( o_l, o );
    return o_l;
}

st_s* st_s_crop( const st_s* o, sz_t start, sz_t end )
{
    st_s* s = st_s_create();
    if( start >= o->size ) return s;
    sz_t end_l = end < o->size ? end : o->size;
    if( end_l <= start ) return s;
    s->space = end_l - start + 1;
    if( s->space < 8 ) s->space = 8;
    s->data = bcore_b_alloc( NULL, s->space, &s->space );
    s->size = end_l - start;
    bcore_memcpy( s->data, o->data + start, s->size );
    s->data[ s->size ] = 0;
    return s;
}

st_s* st_s_crop_d( st_s* o, sz_t start, sz_t end )
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
            o->data[ o->size ] = 0;
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

st_s* st_s_push_char_n( st_s* o, char c, sz_t n )
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

void st_s_pop_n( st_s* o, sz_t n )
{
    if( o->size  == 0 ) return;
    if( o->space == 0 ) st_s_make_strong( o );
    o->size = ( o->size > n ) ? ( o->size - n ) : 0;
    o->data[ o->size ] = 0;
}

st_s* st_s_push_st( st_s* o, const st_s* src )
{
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

st_s* st_s_push_sc( st_s* o, sc_t sc )
{
    if( o->size == 0 )
    {
        st_s_copy_sc( o, sc );
        return o;
    }
    sz_t src_size = bcore_strlen( sc );
    if( o->space < o->size + src_size + 1 )
    {
        if( o->space == 0 ) st_s_make_strong( o );
        o->data = bcore_bn_alloc( o->data, o->space, o->size + src_size + 1, &o->space );
    }
    bcore_memcpy( o->data + o->size, sc, src_size );
    o->size += src_size;
    o->data[ o->size ] = 0;
    return o;
}

st_s* st_s_push_typed( st_s* o, tp_t type, vc_t src )
{
    st_s* s = st_s_create();
    st_s_copy_typed( s, type, src );
    return st_s_push_st_d( o, s );
}

st_s* st_s_pushf( st_s* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s s;
    st_s_initvf( &s, format, args );
    va_end( args );
    st_s_push_st( o, &s );
    st_s_down( &s );
    return o;
}

sz_t st_s_find_char( const st_s* o, sz_t start, sz_t end, char c )
{
    if( end >= start )
    {
        sz_t end_l = end < o->size ? end : o->size;
        for( sz_t j = start; j < end_l; j++ ) if( o->data[ j ] == c ) return j;
        return end_l;
    }
    else
    {
        sz_t start_l = start < o->size ? start : o->size;
        for( sz_t j = start_l - 1;  j < start_l && j >= end; j-- ) if( o->data[ j ] == c ) return j;
        return start_l;
    }
}

sz_t st_s_find_sc( const st_s* o, sz_t start, sz_t end, sc_t find_sc )
{
    if( !find_sc || find_sc[ 0 ] == 0 ) return start;

    if( end >= start )
    {
        sz_t end_l = end < o->size ? end : o->size;
        for( sz_t j = start; j < end_l; j++ )
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
        sz_t start_l = start < o->size ? start : o->size;
        for( sz_t j = start_l - 1;  j < start_l && j >= end; j-- )
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

sz_t st_s_find_st( const st_s* o, sz_t start, sz_t end, const st_s* st )
{
    return st_s_find_sc( o, start, end, st->sc );
}

sz_t st_s_find_st_d( const st_s* o, sz_t start, sz_t end, st_s* st )
{
    sz_t r = st_s_find_st( o, start, end, st );
    st_s_discard( st );
    return r;
}

sz_t st_s_find_any_sc( const st_s* o, sz_t start, sz_t end, sc_t sc )
{
    if( !sc || sc[ 0 ] == 0 ) return start;
    if( end >= start )
    {
        sz_t end_l = end < o->size ? end : o->size;
        for( sz_t j = start; j < end_l; j++ ) if( bcore_strany( o->data[ j ], sc ) ) return j;
        return end_l;
    }
    else
    {
        sz_t start_l = start < o->size ? start : o->size;
        for( sz_t j = start_l - 1; j < start_l && j >= end; j-- ) if( bcore_strany( o->data[ j ], sc ) ) return j;
        return start_l;
    }
}

sz_t st_s_find_none_sc( const st_s* o, sz_t start, sz_t end, sc_t sc )
{
    if( !sc || sc[ 0 ] == 0 ) return start;
    if( end >= start )
    {
        sz_t end_l = end < o->size ? end : o->size;
        for( sz_t j = start; j < end_l; j++ ) if( !bcore_strany( o->data[ j ], sc ) ) return j;
        return end_l;
    }
    else
    {
        sz_t start_l = start < o->size ? start : o->size;
        for( sz_t j = start_l - 1; j < start_l && j >= end; j-- ) if( !bcore_strany( o->data[ j ], sc ) ) return j;
        return start_l;
    }
}

st_s* st_s_remove( st_s* o, sz_t start, sz_t size )
{
    if( start >= o->size ) return o;
    if( o->space == 0 ) st_s_make_strong( o );
    sz_t size_l = start + size > o->size ? o->size - start : size;
    bcore_memmove( o->data + start, o->data + start + size_l, o->size - start - size_l );
    o->size -= size_l;
    o->data[ o->size ] = 0;
    return o;
}

sz_t st_s_count_char( const st_s* o, sz_t start, sz_t end, char c )
{
    sz_t count = 0;
    sz_t end_l = end < o->size ? end : o->size;
    for( sz_t j = start; j < end_l; j++ ) if( o->data[ j ] == c ) count++;
    return count;
}

sz_t st_s_count_sc( const st_s* o, sz_t start, sz_t end, sc_t sc )
{
    sz_t count = 0;
    if( !sc || sc[ 0 ] == 0 ) return 0;

    sz_t end_l = end < o->size ? end : o->size;
    for( sz_t j = start; j < end_l; j++ )
    {
        if( o->data[ j ] == sc[ 0 ] )
        {
            int m = bcore_strcmp( sc, &o->data[ j ] );
            if( m == 0 || m == 1 ) count++;
        }
    }
    return count;
}

sz_t st_s_count_st( const st_s* o, sz_t start, sz_t end, const st_s* st )
{
    return st_s_count_sc( o, start, end, st->sc );
}

st_s* st_s_insert_char( st_s* o, sz_t start, char c )
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

st_s* st_s_insert_st( st_s* o, sz_t start, const st_s* string )
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

st_s* st_s_insert_st_d( st_s* o, sz_t start, st_s* string )
{
    st_s_insert_st( o, start, string );
    st_s_discard( string );
    return o;
}

st_s* st_s_insert_sc( st_s* o, sz_t start, sc_t sc )
{
    if( start > o->size ) return o;
    if( o->space == 0 ) st_s_make_strong( o );
    if( o->space == 0 )
    {
        st_s_copy_sc( o, sc );
        return o;
    }

    sz_t src_size = bcore_strlen( sc );
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

st_s* st_s_replace_char_sc( st_s* o, char c, sc_t sc )
{
    sz_t start = 0;
    sz_t sz_sc = bcore_strlen( sc );
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
    sz_t start = 0;
    sz_t sz_match = bcore_strlen( match );
    sz_t sz_replace = bcore_strlen( replace );
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

sz_t st_s_lineof( const st_s* o, sz_t pos )
{
    return st_s_count_char( o, 0, pos, '\n' ) + 1;
}

sz_t st_s_colof( const st_s* o, sz_t pos )
{
    sz_t pos_l = pos < o->size ? pos : o->size;
    sz_t count = 0;
    while( pos_l > 0 )
    {
        if( o->data[ pos_l - 1 ] == '\n' ) break;
        count++;
        pos_l--;
    }
    return count + 1;
}

sz_t st_s_posof( const st_s* o, sz_t line, sz_t col )
{
    if( line == 0 ) return 0;
    sz_t pos = 0;
    sz_t count = 1;
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

sz_t st_s_posofline( const st_s* o, sz_t pos )
{
    sz_t pos_l = ( pos > o->size ) ? o->size : pos;
    while( pos_l > 0 )
    {
        if( o->data[ pos_l - 1 ] == '\n' ) break;
        pos_l--;
    }
    return pos_l;
}

void st_s_print( const st_s* o )
{
    if( !o ) return;
    printf( "%s", o->sc );
}

void st_s_print_d( st_s* o )
{
    if( !o ) return;
    st_s_print( o );
    st_s_discard( o );
}

st_s* st_s_show_line_context( const st_s* o, sz_t pos )
{
    sz_t start = st_s_posofline( o, pos );
    sz_t end = st_s_find_char( o, pos, o->size, '\n' );
    st_s* s = st_s_create();
    st_s_push_st_d( s, st_s_crop( o, start, end ) );
    st_s_push_char( s, '\n' );
    st_s_push_char_n( s, ' ', pos - start );
    st_s_push_char(   s, '^' );
    st_s_push_char( s, '\n' );
    return s;
}

sz_t st_s_parsevf( const st_s* o, sz_t start, sz_t end, sc_t format, va_list args )
{
    sz_t end_l = end < o->size ? end : o->size;
    sz_t idx = start;

    sc_t fp = format;
    while( *fp )
    {
        if( *fp == ' ' || *fp == '\n' || *fp == '\t' )
        {
            fp++;
            while( idx < end_l )
            {
                char c = o->data[ idx ];
                if( c == ' ' || c == '\t' || c == '\r' || c == '\n' )
                {
                    idx++;
                }
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
            tp_t type = 0;
            if( fp[ 2 ] == '_' && fp[ 3 ] == 't' ) // all types of format xy_t
            {
                switch( fp[ 0 ] )
                {
                    case 'u':
                    {
                        switch( fp[ 1 ] )
                        {
                            case '0': type = TYPEOF_u0_t; break;
                            case '1': type = TYPEOF_u1_t; break;
                            case '2': type = TYPEOF_u2_t; break;
                            case '3': type = TYPEOF_u3_t; break;
                            default: break;
                        }
                    }
                    break;

                    case 's':
                    {
                        switch( fp[ 1 ] )
                        {
                            case '0': type = TYPEOF_s0_t; break;
                            case '1': type = TYPEOF_s1_t; break;
                            case '2': type = TYPEOF_s2_t; break;
                            case '3': type = TYPEOF_s3_t; break;
                            case 'z': type = TYPEOF_sz_t; break;
                            default: break;
                        }
                    }
                    break;

                    case 'f':
                    {
                        switch( fp[ 1 ] )
                        {
                            case '2': type = TYPEOF_f2_t; break;
                            case '3': type = TYPEOF_f3_t; break;
                            default: break;
                        }
                    }
                    break;

                    case 't': type = ( fp[ 1 ] == 'p' ) ? TYPEOF_tp_t : 0; break;
                    case 'b': type = ( fp[ 1 ] == 'l' ) ? TYPEOF_bl_t : 0; break;
                    default: break;
                }

                if( type == 0 ) ERR( "Unknown format type specifier: '%s'.", fp );

                fp += 4;
                int size = 0;
                int sres = -1;
                uintmax_t mval = 0;
                switch( type )
                {
                    case TYPEOF_u0_t: sres = sscanf( o->sc + idx, "%"SCNu8"%n",  va_arg( args, u0_t* ), &size ); break;
                    case TYPEOF_u1_t: sres = sscanf( o->sc + idx, "%"SCNu16"%n", va_arg( args, u1_t* ), &size ); break;
                    case TYPEOF_u2_t: sres = sscanf( o->sc + idx, "%"SCNu32"%n", va_arg( args, u2_t* ), &size ); break;
                    case TYPEOF_u3_t: sres = sscanf( o->sc + idx, "%"SCNu64"%n", va_arg( args, u3_t* ), &size ); break;
                    case TYPEOF_s0_t: sres = sscanf( o->sc + idx, "%"SCNi8"%n",  va_arg( args, s0_t* ), &size ); break;
                    case TYPEOF_s1_t: sres = sscanf( o->sc + idx, "%"SCNi16"%n", va_arg( args, s1_t* ), &size ); break;
                    case TYPEOF_s2_t: sres = sscanf( o->sc + idx, "%"SCNi32"%n", va_arg( args, s2_t* ), &size ); break;
                    case TYPEOF_s3_t: sres = sscanf( o->sc + idx, "%"SCNi64"%n", va_arg( args, s3_t* ), &size ); break;
                    case TYPEOF_sz_t: sres = sscanf( o->sc + idx, "%"SCNuMAX"%n", &mval, &size ); *va_arg( args, sz_t* ) = mval; break;
                    case TYPEOF_f2_t: sres = sscanf( o->sc + idx, "%g%n",        va_arg( args, f2_t* ), &size ); break;
                    case TYPEOF_f3_t: sres = sscanf( o->sc + idx, "%lg%n",       va_arg( args, f3_t* ), &size ); break;
                    case TYPEOF_tp_t: sres = sscanf( o->sc + idx, "%"SCNuMAX"%n", &mval, &size ); *va_arg( args, tp_t* ) = mval; break;
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
                            ERR( "\n%s\nParsing #bl_t failed at (%zu:%zu): true or false expected.", st_s_show_line_context( o, idx )->sc, st_s_lineof( o, idx ), st_s_colof( o, idx ) );
                        }
                        *va_arg( args, bl_t* ) = val;
                        size = 0;
                        sres = 1;
                    }
                    break;
                }

                if( sres <= 0 )
                {
                    ERR( "\n%s\nParsing type #%s failed at (%zu:%zu).", st_s_show_line_context( o, idx )->sc, ifnameof( type ), st_s_lineof( o, idx ), st_s_colof( o, idx ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( *fp == '?' )
            {
                sc_t fp0 = fp;
                sz_t idx0 = idx;
                fp++;
                bool* flag = va_arg( args, bool* );
                *flag = true;
                if( *fp == '\'' )
                {
                    fp++;
                    while( *fp != '\'' && *fp != 0 && idx < end_l )
                    {
                        if( *flag )
                        {
                            *flag = *fp == o->sc[ idx ];
                            idx += ( idx < end_l );
                        }
                        fp++;
                    }
                    fp += ( *fp == '\'' );
                    if( !*flag ) idx = idx0;
                }
                else
                {
                    ERR( "Unknown format directive: '%s'.", fp0 );
                }
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
                    ERR( "\n%s\n'#' expected at (%zu:%zu).", st_s_show_line_context( o, idx )->sc, fp, st_s_lineof( o, idx ), st_s_colof( o, idx ) );
                }
            }
            else if( ( bcore_strcmp( "aware_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "aware_t" );
                int size = 0;
                uintmax_t val;
                if( sscanf( o->sc + idx, "%"SCNuMAX"%n", &val, &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #aware_t failed at (%zu:%zu).", st_s_show_line_context( o, idx )->sc, st_s_lineof( o, idx ), st_s_colof( o, idx ) );
                }
                *va_arg( args, aware_t* ) = val;
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
                    ERR( "\n%s\nParsing #bool failed at (%zu:%zu): true or false expected.", st_s_show_line_context( o, idx )->sc, st_s_lineof( o, idx ), st_s_colof( o, idx ) );
                }
                *va_arg( args, bool* ) = val;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "name", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "name" );
                st_s* string = va_arg( args, st_s* );
                st_s_clear( string );
                char c = o->sc[ idx ];
                if( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) || c == '_' )
                {
                    if( idx < end_l )
                    {
                        idx++;
                        st_s_push_char( string, c );
                        for(;;)
                        {
                            c = o->sc[ idx ];
                            if( idx < end_l && ( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) || ( c >= '0' && c <= '9' ) || c == '_' ) )
                            {
                                st_s_push_char( string, c );
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
                st_s* string = va_arg( args, st_s* );
                st_s_clear( string );
                if( o->sc[ idx ] != '"' ) ERR( "\n%s\n'\"' expected at (%zu:%zu).", st_s_show_line_context( o, idx )->sc, st_s_lineof( o, idx ), st_s_colof( o, idx ) );
                idx++;
                while ( o->sc[ idx ] != '"' )
                {
                    if( o->sc[ idx ] == '\\' && o->sc[ idx + 1 ] == '\"' )
                    {
                        st_s_push_char( string, '\"' );
                        idx += 2;
                    }
                    else if( o->sc[ idx ] == '\\' && o->sc[ idx + 1 ] == '\\' )
                    {
                        st_s_push_char( string, '\\' );
                        idx += 2;
                    }
                    else
                    {
                        st_s_push_char( string, o->sc[ idx ] );
                        idx++;
                    }
                }
                idx++;
            }
            else if( ( bcore_strcmp( "until", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "until" );
                sc_t err_msg = "Format specifier 'until' must be followed by character enclosed in single quotes ''";
                if( *fp++ != '\'' ) ERR( err_msg );
                char char_l = *fp++;
                if(  char_l == 0  ) ERR( err_msg );
                if( *fp++ != '\'' ) ERR( err_msg );
                st_s* string = va_arg( args, st_s* );
                st_s_clear( string );
                while ( o->sc[ idx ] != char_l ) st_s_push_char( string, o->sc[ idx++ ] );
            }
            else
            {
                ERR( "Unknown format directive: '%s'.", fp );
            }
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
                ERR( "\n%s\nMatching format characters '%s' failed at (%zu:%zu).", st_s_show_line_context( o, idx )->sc, fp0, st_s_lineof( o, idx ), st_s_colof( o, idx ) );
            }
        }
    }
    return idx;
}

sz_t st_s_parsef(  const st_s* o, sz_t start, sz_t end, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    sz_t idx = st_s_parsevf( o, start, end, format, args );
    va_end( args );
    return idx;
}

static sz_t flow_snk( vd_t o, vc_t data, sz_t size )
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

static sz_t flow_src( vd_t o, vd_t data, sz_t size )
{
    if( *( aware_t*)o != TYPEOF_st_s ) ERR( "object 'o' of type %u ('%s') must be st_s", *( aware_t*)o, ifnameof( *( aware_t*)o ) );
    st_s* s = o;
    if( s->space > 0 ) ERR( "String is strong. Only weak strings can be used as flow-source." );
    size = ( size > s->size ) ? s->size : size;
    bcore_memcpy( data, s->data, size );
    s->size -= size;
    return size;
}

static void string_p_errorvf( const st_s* o, sc_t format, va_list args )
{
    st_s* context = st_s_show_line_context( o, 0 );
    st_s* msg     = st_s_createvf( format, args );
    bcore_err( "st_s source parse error:\n%s%s\n", context->sc, msg->sc );
    st_s_discard( msg );
    st_s_discard( context );
}

static void string_parsevf( st_s* o, sc_t format, va_list args )
{
    if( o->space > 0 ) ERR( "String is strong. Only weak strings can be used as flow-source." );
    o->data += st_s_parsevf( o, 0, o->size, format, args );
}

/// sanity feature
void bcore_inst_typed_check_sizeof( u2_t type, sz_t size );
static void check_sanity( vc_t o )
{
    bcore_inst_typed_check_sizeof( TYPEOF_st_s, sizeof( st_s ) );

    if( TYPEOF_st_s != *(aware_t *)o ) ERR( "incorrect type value (%"PRIu32") (expected st_s)", *(aware_t *)o );
    const st_s* s = o;

    if( ( s->space > 0 ) && ( s->space < s->size ) ) ERR( "space (%zu) < size (%zu)", s->space, s->size );
    if( s->data[ s->size ] != 0 )  ERR( "data section does not terminate in zero" );

    for( sz_t i = 0; i < s->size; i++ )
    {
        u0_t c = s->data[ i ];
        if( c < 10 || c >= 128 ) ERR( "invalid character '%c' in string", c );
    }
}

static bcore_flect_self_s* st_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "st_s = { aware_t _; private sd_t data; private sz_t size; private sz_t space; }", sizeof( st_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )st_s_init,         "bcore_fp_init",           "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )st_s_down,         "bcore_fp_down",           "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )st_s_copy,         "bcore_fp_copy",           "copy"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )st_s_move,         "bcore_fp_move",           "move"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )st_s_create,       "bcore_fp_create",         "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )st_s_clone,        "bcore_fp_clone",          "clone"        );
    bcore_flect_self_s_push_external_func( self, ( fp_t )st_s_discard,      "bcore_fp_discard",        "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )st_s_copy_typed,   "bcore_fp_copy_typed",     "copy_typed"   );
    bcore_flect_self_s_push_external_func( self, ( fp_t )st_s_create_typed, "bcore_fp_create_typed",   "create_typed" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )flow_snk,          "bcore_fp_flow_snk",       "flow_snk"     );
    bcore_flect_self_s_push_external_func( self, ( fp_t )flow_src,          "bcore_fp_flow_src",       "flow_src"     );
    bcore_flect_self_s_push_external_func( self, ( fp_t )string_p_errorvf,  "bcore_fp_logvf",          "p_errorvf"    );
    bcore_flect_self_s_push_external_func( self, ( fp_t )string_parsevf,    "bcore_source_fp_parsevf", "parsevf"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )check_sanity,      "bcore_fp_check_sanity",   "check_sanity" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )st_s_cmp_st,       "bcore_fp_compare",        "cmp_st"       );
    return self;
}

static void st_s_quicktest( void )
{
    sz_t granted_space = bcore_tbman_granted_space();

    bcore_life_s* life = bcore_life_s_create();
    st_s* s = bcore_life_s_push( life, st_s_discard, st_s_create() );

    ASSERT( s->size == 0 );
    ASSERT( s->space == 0 );
    st_s_copy_sc( s, "My little white elephant!" );
    ASSERT( st_s_equal_sc( s, "My little white elephant!" ) );
    st_s_replace_sc_sc( s, "white", "green" );
    ASSERT( st_s_equal_sc( s, "My little green elephant!" ) );

    bool is_green = false;
    bool is_blue  = false;
    st_s* name = bcore_life_s_push( life, st_s_discard, st_s_create() );
    sz_t idx = st_s_parsef( s, 0, s->size, "My #name #?'blue' #?'green' eleph", name, &is_blue, &is_green );
    ASSERT( st_s_equal_sc( name, "little" ) );
    ASSERT( is_green );
    ASSERT( !is_blue );
    ASSERT( bcore_strcmp( s->sc + idx, "ant!" ) == 0 );

    s2_t num0 = -12345;
    st_s_copy_sc( s, "This number is " );
    st_s_push_typed( s, typeof( "s2_t" ), &num0 );
    st_s_push_sc( s, " and an integer" );
    s3_t num1 = 0;
    idx = st_s_parsef( s, 0, s->size, "This number is #s3_t", &num1 );
    ASSERT( num1 == -12345 );
    ASSERT( bcore_strcmp( s->sc + idx, " and an integer" ) == 0 );

    st_s* ws = bcore_life_s_push( life, st_s_discard, st_s_create() );
    st_s_assign_sc( ws, "This is a weak string" );

    ASSERT( st_s_cmp_sc( ws, "This is a weak string" ) == 0 );
    ASSERT( ws->space == 0 );

    st_s_replace_sc_sc( ws, "weak", "strong" );
    ASSERT( st_s_cmp_sc( ws, "This is a strong string" ) == 0 );
    ASSERT( ws->space > 0 );

    bcore_life_s_discard( life );

    ASSERT( granted_space == bcore_tbman_granted_space() );
}

/**********************************************************************************************************************/

vd_t bcore_st_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_st" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "st_s"  ), st_s_create_self  );
        st_s_quicktest();
    }
    else if( signal == typeof( "selftest" ) )
    {
        st_s_quicktest();
    }

    return NULL;
}

/**********************************************************************************************************************/
