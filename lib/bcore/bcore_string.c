/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "stdio.h"
#include "bcore_string.h"
#include "bcore_control.h"
#include "bcore_memory_manager.h"
#include "bcore_name_manager.h"
#include "bcore_flect.h"
#include "bcore_quicktypes.h"

void bcore_string_s_init( bcore_string_s* o )
{
    o->_ = TYPEOF_bcore_string_s;
    o->sc = "";
    o->size = o->space = 0;
}

void bcore_string_s_initvf( bcore_string_s* o, sc_t format, va_list args )
{
    bcore_string_s_init( o );
    va_list args1;
    va_copy( args1, args );
    o->space = vsnprintf( NULL, 0, format, args1 ) + 1;
    va_end( args1 );
    if( o->space < 8 ) o->space = 8;
    o->data = bcore_malloc( o->space );
    vsnprintf( o->data, o->space, format, args );
    o->size = bcore_strlen( o->sc );
}

void bcore_string_s_initf( bcore_string_s* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_string_s_initvf( o, format, args );
    va_end( args );
}

void bcore_string_s_down( bcore_string_s* o )
{
    if( o->space ) bcore_free( o->data );
    o->size = o->space = 0;
}

void bcore_string_s_copy( bcore_string_s* o, const bcore_string_s* src )
{
    if( o == src ) return;
    if( o->space <= src->size )
    {
        if( o->space > 0 ) bcore_free( o->data );
        o->space = src->size < 8 ? 8 : src->size + 1;
        o->data  = bcore_malloc( o->space );
    }
    bcore_memcpy( o->data, src->data, src->size );
    o->size = src->size;
    o->data[ o->size ] = 0;
}

void bcore_string_s_copy_sc( bcore_string_s* o, sc_t sc )
{
    sz_t src_size = bcore_strlen( sc );
    if( o->space <= src_size )
    {
        if( o->space > 0 ) bcore_free( o->data );
        o->space = src_size < 8 ? 8 : src_size + 1;
        o->data  = bcore_malloc( o->space );
    }
    bcore_memcpy( o->data, sc, src_size );
    o->size = src_size;
    o->data[ o->size ] = 0;
}

void bcore_string_s_copyf( bcore_string_s* o, sc_t format, ...  )
{
    bcore_string_s_down( o );
    va_list args;
    va_start( args, format );
    bcore_string_s_initvf( o, format, args );
    va_end( args );
}

void bcore_string_s_copy_typed( bcore_string_s* o, tp_t type, vc_t src )
{
    switch( type )
    {
        case TYPEOF_bcore_string_s: bcore_string_s_copy( o, ( const bcore_string_s* )src ); break;
        case TYPEOF_sc_t: bcore_string_s_copy_sc( o,          *(const sc_t*)src ); break;
        case TYPEOF_sd_t: bcore_string_s_copy_sc( o,          *(const sc_t*)src ); break;
        case TYPEOF_s0_t: bcore_string_s_copyf( o, "%"PRIi8 , *(const s0_t*)src ); break;
        case TYPEOF_s1_t: bcore_string_s_copyf( o, "%"PRIi16, *(const s1_t*)src ); break;
        case TYPEOF_s2_t: bcore_string_s_copyf( o, "%"PRIi32, *(const s2_t*)src ); break;
        case TYPEOF_s3_t: bcore_string_s_copyf( o, "%"PRIi64, *(const s3_t*)src ); break;
        case TYPEOF_u0_t: bcore_string_s_copyf( o, "%"PRIu8 , *(const u0_t*)src ); break;
        case TYPEOF_u1_t: bcore_string_s_copyf( o, "%"PRIu16, *(const u1_t*)src ); break;
        case TYPEOF_u2_t: bcore_string_s_copyf( o, "%"PRIu32, *(const u2_t*)src ); break;
        case TYPEOF_u3_t: bcore_string_s_copyf( o, "%"PRIu64, *(const u3_t*)src ); break;
        case TYPEOF_f2_t: bcore_string_s_copyf( o, "%g",      *(const f2_t*)src ); break;
        case TYPEOF_f3_t: bcore_string_s_copyf( o, "%lg",     *(const f3_t*)src ); break;
        case TYPEOF_sz_t: bcore_string_s_copyf( o, "%zu",     *(const sz_t*)src ); break;
        case TYPEOF_bool: bcore_string_s_copy_sc( o, *(const bool*)src ? "true" : "false" ); break;

        case TYPEOF_tp_t:
        case TYPEOF_aware_t:
        {
            sc_t name = bcore_name_try_name( *(const tp_t*)src );
            if( name )
            {
                bcore_string_s_copy_sc( o, name );
            }
            else
            {
                bcore_string_s_copyf( o, "%u", *(const tp_t*)src ); break;
            }
        }
        break;

        default:
        {
            ERR( "Converting type '%s' into bcore_string_s is not yet implemented", ifnameof( type ) );
        }
        break;
    }
}

void bcore_string_s_copy_aware( bcore_string_s* o, vc_t src )
{
    bcore_string_s_copy_typed( o, *( aware_t* )src, src );
}

void bcore_string_s_move( bcore_string_s* o, bcore_string_s* src )
{
    bcore_string_s_init( o );
    bcore_string_s_copy( o, src );
    bcore_string_s_down( src );
}

bcore_string_s* bcore_string_s_create()
{
    bcore_string_s* o = bcore_malloc( sizeof( bcore_string_s ) );
    bcore_string_s_init( o );
    return o;
}

bcore_string_s* bcore_string_s_createvf( sc_t format, va_list args )
{
    bcore_string_s* o = bcore_malloc( sizeof( bcore_string_s ) );
    bcore_string_s_initvf( o, format, args );
    return o;
}

bcore_string_s* bcore_string_s_createf( sc_t format, ... )
{
    bcore_string_s* o = bcore_malloc( sizeof( bcore_string_s ) );
    va_list args;
    va_start( args, format );
    bcore_string_s_initvf( o, format, args );
    va_end( args );
    return o;
}

bcore_string_s* bcore_string_s_create_sc( sc_t sc )
{
    return bcore_string_s_push_sc( bcore_string_s_create(), sc );
}

bcore_string_s* bcore_string_s_create_l( bcore_life_s* life )
{
    return bcore_life_s_push( life, bcore_string_s_discard, bcore_string_s_create() );
}

bcore_string_s* bcore_string_s_createf_l( bcore_life_s* life, sc_t format, ... )
{
    bcore_string_s* o = bcore_malloc( sizeof( bcore_string_s ) );
    va_list args;
    va_start( args, format );
    bcore_string_s_initvf( o, format, args );
    va_end( args );
    return bcore_life_s_push( life, bcore_string_s_discard, o );
}

bcore_string_s* bcore_string_s_create_l_sc( bcore_life_s* life, sc_t sc )
{
    return bcore_life_s_push( life, bcore_string_s_discard, bcore_string_s_create_sc( sc ) );
}

bcore_string_s* bcore_string_s_create_typed( tp_t type, vc_t src )
{
    bcore_string_s* o = bcore_malloc( sizeof( bcore_string_s ) );
    bcore_string_s_init( o );
    bcore_string_s_copy_typed( o, type, src );
    return o;
}

bcore_string_s* bcore_string_s_create_aware( vc_t src )
{
    return bcore_string_s_create_typed( *( aware_t* )src, src );
}

void bcore_string_s_discard( bcore_string_s* o )
{
    if( !o ) return;
    bcore_string_s_down( o );
    bcore_free( o );
}

bcore_string_s* bcore_string_s_clone( const bcore_string_s* o )
{
    bcore_string_s* o_l = bcore_string_s_create();
    bcore_string_s_copy( o_l, o );
    return o_l;
}

bcore_string_s* bcore_string_s_crop( const bcore_string_s* o, sz_t start, sz_t end )
{
    bcore_string_s* s = bcore_string_s_create();
    if( start >= o->size ) return s;
    sz_t end_l = end < o->size ? end : o->size;
    if( end_l <= start ) return s;
    s->space = end_l - start + 1;
    if( s->space < 8 ) s->space = 8;
    s->data = bcore_malloc( s->space );
    s->size = end_l - start;
    bcore_memcpy( s->data, o->data + start, s->size );
    s->data[ s->size ] = 0;
    return s;
}

bcore_string_s* bcore_string_s_crop_d( bcore_string_s* o, sz_t start, sz_t end )
{
    bcore_string_s* s = bcore_string_s_crop( o, start, end );
    bcore_string_s_discard( o );
    return s;
}

bcore_string_s* bcore_string_s_clear( bcore_string_s* o )
{
    if( o->size > 0 )
    {
        o->size = 0;
        o->data[ o->size ] = 0;
    }
    return o;
}

bcore_string_s* bcore_string_s_push_char( bcore_string_s* o, char c )
{
    if( o->space == 0 )
    {
        o->space = 8;
        o->data = bcore_malloc( o->space );
    }
    else if( o->space < o->size + 2 )
    {
        o->space *= 2;
        o->data = bcore_alloc( o->data, o->space );
    }
    o->data[ o->size ] = c;
    o->size++;
    o->data[ o->size ] = 0;
    return o;
}

bcore_string_s* bcore_string_s_push_char_n( bcore_string_s* o, char c, sz_t n )
{
    for( sz_t i = 0; i < n; i++ ) bcore_string_s_push_char( o, c );
    return o;
}

char bcore_string_s_pop_char( bcore_string_s* o )
{
    if( o->size == 0 ) return 0;
    o->size--;
    char c = o->data[ o->size ];
    o->data[ o->size ] = 0;
    return c;
}

void bcore_string_s_pop_n( bcore_string_s* o, sz_t n )
{
    if( o->size == 0 ) return;
    o->size = ( o->size > n ) ? ( o->size - n ) : 0;
    o->data[ o->size ] = 0;
}

bcore_string_s* bcore_string_s_push_string( bcore_string_s* o, const bcore_string_s* src )
{
    if( o->size == 0 )
    {
        bcore_string_s_copy( o, src );
        return o;
    }
    if( o->space < o->size + src->size + 1 )
    {
        o->space = o->size + src->size + 1;
        o->data = bcore_alloc( o->data, o->space );
    }
    bcore_memcpy( o->data + o->size, src->data, src->size );
    o->size += src->size;
    o->data[ o->size ] = 0;
    return o;
}

bcore_string_s* bcore_string_s_push_string_d( bcore_string_s* o, bcore_string_s* src )
{
    bcore_string_s_push_string( o, src );
    bcore_string_s_discard( src );
    return o;
}

bcore_string_s* bcore_string_s_push_sc( bcore_string_s* o, sc_t sc )
{
    if( o->size == 0 )
    {
        bcore_string_s_copy_sc( o, sc );
        return o;
    }

    sz_t src_size = bcore_strlen( sc );
    if( o->space < o->size + src_size + 1 )
    {
        o->space = o->size + src_size + 1;
        o->data = bcore_alloc( o->data, o->space );
    }
    bcore_memcpy( o->data + o->size, sc, src_size );
    o->size += src_size;
    o->data[ o->size ] = 0;
    return o;
}

bcore_string_s* bcore_string_s_push_typed( bcore_string_s* o, tp_t type, vc_t src )
{
    bcore_string_s* s = bcore_string_s_create();
    bcore_string_s_copy_typed( s, type, src );
    return bcore_string_s_push_string_d( o, s );
}

bcore_string_s* bcore_string_s_pushf( bcore_string_s* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_string_s s;
    bcore_string_s_initvf( &s, format, args );
    va_end( args );
    bcore_string_s_push_string( o, &s );
    bcore_string_s_down( &s );
    return o;
}

sz_t bcore_string_s_find_char( const bcore_string_s* o, sz_t start, sz_t end, char c )
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

sz_t bcore_string_s_find_sc( const bcore_string_s* o, sz_t start, sz_t end, sc_t find_sc )
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

sz_t bcore_string_s_find_string( const bcore_string_s* o, sz_t start, sz_t end, const bcore_string_s* string )
{
    return bcore_string_s_find_sc( o, start, end, string->sc );
}

sz_t bcore_string_s_find_string_d( const bcore_string_s* o, sz_t start, sz_t end, bcore_string_s* string )
{
    sz_t r = bcore_string_s_find_string( o, start, end, string );
    bcore_string_s_discard( string );
    return r;
}

sz_t bcore_string_s_find_any_sc( const bcore_string_s* o, sz_t start, sz_t end, sc_t sc )
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

sz_t bcore_string_s_find_none_sc( const bcore_string_s* o, sz_t start, sz_t end, sc_t sc )
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

bcore_string_s* bcore_string_s_remove( bcore_string_s* o, sz_t start, sz_t size )
{
    if( start >= o->size ) return o;
    sz_t size_l = start + size > o->size ? o->size - start : size;
    bcore_memmove( o->data + start, o->data + start + size_l, o->size - start - size_l );
    o->size -= size_l;
    o->data[ o->size ] = 0;
    return o;
}

sz_t bcore_string_s_count_char( const bcore_string_s* o, sz_t start, sz_t end, char c )
{
    sz_t count = 0;
    sz_t end_l = end < o->size ? end : o->size;
    for( sz_t j = start; j < end_l; j++ ) if( o->data[ j ] == c ) count++;
    return count;
}

sz_t bcore_string_s_count_sc( const bcore_string_s* o, sz_t start, sz_t end, sc_t sc )
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

sz_t bcore_string_s_count_string( const bcore_string_s* o, sz_t start, sz_t end, const bcore_string_s* string )
{
    return bcore_string_s_count_sc( o, start, end, string->sc );
}

bcore_string_s* bcore_string_s_insert_char( bcore_string_s* o, sz_t start, char c )
{
    if( start > o->size ) return o;
    if( o->space == 0 )
    {
        o->space = 8;
        o->data = bcore_malloc( o->space );
    }
    else if( o->space < o->size + 2 )
    {
        o->space *= 2;
        o->data = bcore_alloc( o->data, o->space );
    }
    bcore_memmove( o->data + start + 1, o->data + start, o->size - start );
    o->data[ start ] = c;
    o->size++;
    o->data[ o->size ] = 0;
    return o;
}

bcore_string_s* bcore_string_s_insert_string( bcore_string_s* o, sz_t start, const bcore_string_s* string )
{
    if( o == string )
    {
        bcore_string_s* string_l = bcore_string_s_clone( string );
        bcore_string_s_insert_string( o, start, string_l );
        bcore_string_s_discard( string_l );
        return o;
    }
    if( start > o->size ) return o;
    if( o->space == 0 )
    {
        bcore_string_s_copy( o, string );
        return o;
    }
    if( o->space < o->size + string->size + 1 )
    {
        o->space = o->size + string->size + 1;
        o->data = bcore_alloc( o->data, o->space );
    }
    bcore_memmove( o->data + start + string->size, o->data + start, o->size - start );
    bcore_memcpy(  o->data + start, string->sc, string->size );
    o->size += string->size;
    o->data[ o->size ] = 0;
    return o;
}

bcore_string_s* bcore_string_s_insert_string_d( bcore_string_s* o, sz_t start, bcore_string_s* string )
{
    bcore_string_s_insert_string( o, start, string );
    bcore_string_s_discard( string );
    return o;
}

bcore_string_s* bcore_string_s_insert_sc( bcore_string_s* o, sz_t start, sc_t sc )
{
    if( start > o->size ) return o;
    if( o->space == 0 )
    {
        bcore_string_s_copy_sc( o, sc );
        return o;
    }

    sz_t src_size = bcore_strlen( sc );
    if( o->space < o->size + src_size + 1 )
    {
        o->space = o->size + src_size + 1;
        o->data = bcore_alloc( o->data, o->space );
    }
    bcore_memmove( o->data + start + src_size, o->data + start, o->size - start );
    bcore_memcpy(  o->data + start, sc, src_size );
    o->size += src_size;
    o->data[ o->size ] = 0;
    return o;
}

bcore_string_s* bcore_string_s_replace_char_sc( bcore_string_s* o, char c, sc_t sc )
{
    sz_t start = 0;
    sz_t sz_sc = bcore_strlen( sc );

    while( ( start = bcore_string_s_find_char( o, start, o->size, c ) ) < o->size )
    {
        bcore_string_s_remove(    o, start, 1 );
        bcore_string_s_insert_sc( o, start, sc );
        start += sz_sc;
    }
    return o;
}

bcore_string_s* bcore_string_s_replace_char_string( bcore_string_s* o, char c, const bcore_string_s* string )
{
    return bcore_string_s_replace_char_sc( o, c, string->sc );
}

bcore_string_s* bcore_string_s_replace_char_string_d( bcore_string_s* o, char c, bcore_string_s* string )
{
    bcore_string_s_replace_char_sc( o, c, string->sc );
    bcore_string_s_discard( string );
    return o;
}

bcore_string_s* bcore_string_s_replace_sc_sc( bcore_string_s* o, sc_t match, sc_t replace )
{
    sz_t start = 0;
    sz_t sz_match = bcore_strlen( match );
    sz_t sz_replace = bcore_strlen( replace );
    while( ( start = bcore_string_s_find_sc( o, start, o->size, match ) ) < o->size )
    {
        bcore_string_s_remove(    o, start, sz_match );
        bcore_string_s_insert_sc( o, start, replace );
        start += sz_replace;
    }
    return o;
}

bcore_string_s* bcore_string_s_replace_string_string( bcore_string_s* o, const bcore_string_s* match, const bcore_string_s* replace )
{
    return bcore_string_s_replace_sc_sc( o, match->sc, replace->sc );
}

bcore_string_s* bcore_string_s_replace_string_d_string_d( bcore_string_s* o, bcore_string_s* match, bcore_string_s* replace )
{
    bcore_string_s_replace_string_string( o, match, replace );
    bcore_string_s_discard( match );
    bcore_string_s_discard( replace );
    return o;
}

sz_t bcore_string_s_lineof( const bcore_string_s* o, sz_t pos )
{
    return bcore_string_s_count_char( o, 0, pos, '\n' ) + 1;
}

sz_t bcore_string_s_colof( const bcore_string_s* o, sz_t pos )
{
    sz_t pos_l = pos < o->size ? pos : o->size;
    sz_t count = 0;
    while( pos_l != 0 )
    {
        if( o->data[ pos_l ] == '\n' ) break;
        count++;
        pos_l--;
    }
    return count + 1;
}

sz_t bcore_string_s_posof( const bcore_string_s* o, sz_t line, sz_t col )
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

sz_t bcore_string_s_posofline( const bcore_string_s* o, sz_t pos )
{
    sz_t pos_l = ( pos > o->size ) ? o->size : pos;
    while( pos_l > 0 )
    {
        if( o->data[ pos_l - 1 ] == '\n' ) break;
        pos_l--;
    }
    return pos_l;
}

void bcore_string_s_print( const bcore_string_s* o )
{
    if( !o ) return;
    printf( "%s", o->sc );
}

void bcore_string_s_print_d( bcore_string_s* o )
{
    if( !o ) return;
    bcore_string_s_print( o );
    bcore_string_s_discard( o );
}

bcore_string_s* bcore_string_s_show_line_context( const bcore_string_s* o, sz_t pos )
{
    sz_t start = bcore_string_s_posofline( o, pos );
    sz_t end = bcore_string_s_find_char( o, pos, o->size, '\n' );
    bcore_string_s* s = bcore_string_s_create();
    bcore_string_s_push_string_d( s, bcore_string_s_crop( o, start, end ) );
    bcore_string_s_push_char( s, '\n' );
    bcore_string_s_push_char_n( s, ' ', pos - start );
    bcore_string_s_push_char(   s, '^' );
    bcore_string_s_push_char( s, '\n' );
    return s;
}

sz_t bcore_string_s_parsevf( const bcore_string_s* o, sz_t start, sz_t end, sc_t format, va_list args )
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
            if( ( bcore_strcmp( "#u3_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#u3_t" );
                int size = 0;
                if( sscanf( o->sc + idx, "%"SCNu64"%n", va_arg( args, u3_t* ), &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #u3_t failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#u2_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#u2_t" );
                int size = 0;
                if( sscanf( o->sc + idx, "%"SCNu32"%n", va_arg( args, u2_t* ), &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #u2_t failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#u1_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#u1_t" );
                int size = 0;
                if( sscanf( o->sc + idx, "%"SCNu16"%n", va_arg( args, u1_t* ), &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #u1_t failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#u0_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#u0_t" );
                int size = 0;
                if( sscanf( o->sc + idx, "%"SCNu8"%n", va_arg( args, u0_t* ), &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #u0_t failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#s3_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#s3_t" );
                int size = 0;
                if( sscanf( o->sc + idx, "%"SCNi64"%n", va_arg( args, s3_t* ), &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #s3_t failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#s2_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#s2_t" );
                int size = 0;
                if( sscanf( o->sc + idx, "%"SCNi32"%n", va_arg( args, s2_t* ), &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #s2_t failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#s1_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#s1_t" );
                int size = 0;
                if( sscanf( o->sc + idx, "%"SCNi16"%n", va_arg( args, s1_t* ), &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #s1_t failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#s0_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#s0_t" );
                int size = 0;
                if( sscanf( o->sc + idx, "%"SCNi8"%n", va_arg( args, s0_t* ), &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #s0_t failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#f3_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#f3_t" );
                int size = 0;
                if( sscanf( o->sc + idx, "%lg%n", va_arg( args, f3_t* ), &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #f3_t failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#f2_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#f2_t" );
                int size = 0;
                if( sscanf( o->sc + idx, "%g%n", va_arg( args, f2_t* ), &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #f2_t failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#sz_t", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#sz_t" );
                int size = 0;
                uintmax_t val;
                if( sscanf( o->sc + idx, "%"SCNuMAX"%n", &val, &size ) <= 0 )
                {
                    ERR( "\n%s\nParsing #sz_t failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                *va_arg( args, sz_t* ) = val;
                idx += size;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#bool", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#bool" );
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
                    ERR( "\n%s\nParsing #bool failed at (%zu:%zu): true or false expected.", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                *va_arg( args, bool* ) = val;
                if( idx > end_l ) idx = end_l;
            }
            else if( ( bcore_strcmp( "#name", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#name" );
                bcore_string_s* string = va_arg( args, bcore_string_s* );
                bcore_string_s_clear( string );
                char c = o->sc[ idx ];
                if( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) || c == '_' )
                {
                    if( idx < end_l )
                    {
                        idx++;
                        bcore_string_s_push_char( string, c );
                        for(;;)
                        {
                            c = o->sc[ idx ];
                            if( idx < end_l && ( ( c >= 'A' && c <= 'Z' ) || ( c >= 'a' && c <= 'z' ) || ( c >= '0' && c <= '9' ) || c == '_' ) )
                            {
                                bcore_string_s_push_char( string, c );
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
            else if( ( bcore_strcmp( "#string", fp ) >> 1 ) == 0 )
            {
                fp += strlen( "#string" );
                bcore_string_s* string = va_arg( args, bcore_string_s* );
                bcore_string_s_clear( string );
                if( o->sc[ idx ] != '"' ) ERR( "\n%s\n'\"' expected at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                idx++;
                while ( o->sc[ idx ] != '"' )
                {
                    if( o->sc[ idx ] == '\\' && o->sc[ idx + 1 ] == '\"' )
                    {
                        bcore_string_s_push_char( string, '\"' );
                        idx += 2;
                    }
                    else if( o->sc[ idx ] == '\\' && o->sc[ idx + 1 ] == '\\' )
                    {
                        bcore_string_s_push_char( string, '\\' );
                        idx += 2;
                    }
                    else
                    {
                        bcore_string_s_push_char( string, o->sc[ idx ] );
                        idx++;
                    }
                }
                idx++;
            }
            else if( ( bcore_strcmp( "#?", fp ) >> 1 ) == 0 )
            {
                sc_t fp0 = fp;
                sz_t idx0 = idx;
                fp += strlen( "#?" );
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
            else if( ( bcore_strcmp( "##", fp ) >> 1 ) == 0 )
            {
                if( '#' == o->sc[ idx ] )
                {
                    idx++;
                    fp++;
                }
                else
                {
                    ERR( "\n%s\nMatching format characters '%s' failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, fp, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
                }
                fp += strlen( "##" );
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
                ERR( "\n%s\nMatching format characters '%s' failed at (%zu:%zu).", bcore_string_s_show_line_context( o, idx )->sc, fp0, bcore_string_s_lineof( o, idx ), bcore_string_s_colof( o, idx ) );
            }
        }
    }
    return idx;
}

sz_t bcore_string_s_parsef(  const bcore_string_s* o, sz_t start, sz_t end, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    sz_t idx = bcore_string_s_parsevf( o, start, end, format, args );
    va_end( args );
    return idx;
}

static sz_t flow_snk( vd_t o, vc_t data, sz_t size )
{
    if( *( aware_t*)o != TYPEOF_bcore_string_s ) ERR( "object 'o' of type %u ('%s') must be bcore_string_s", *( aware_t*)o, ifnameof( *( aware_t*)o ) );
    bcore_string_s* s = o;
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

/// sanity feature
void bcore_instance_typed_check_sizeof( u2_t type, sz_t size );
static void check_sanity( vc_t o )
{
    bcore_instance_typed_check_sizeof( TYPEOF_bcore_string_s, sizeof( bcore_string_s ) );

    if( TYPEOF_bcore_string_s != *(aware_t *)o ) ERR( "incorrect type value (%"PRIu32") (expected bcore_string_s)", *(aware_t *)o );
    const bcore_string_s* s = o;

    if( s->size  > 1000000000 )    ERR( "String of size (%zu) appears incorrect.", s->size );
    if( s->space > 1000000000 )    ERR( "String of space (%zu) appears incorrect.", s->space );
    if( s->space < s->size )       ERR( "space (%zu) < size (%zu)", s->space, s->size );
    if( s->data[ s->size ] != 0 )  ERR( "data section does not terminate in zero" );

    for( sz_t i = 0; i < s->size; i++ )
    {
        u0_t c = s->data[ i ];
        if( c < 10 || c >= 128 ) ERR( "invalid character '%c' in string", c );
    }
}

bcore_flect_self_s* bcore_string_s_create_self()
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( " bcore_string_s =  { aware_t _; sd_t data; sz_t size; sz_t space; }", sizeof( bcore_string_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_string_s_init,         "bcore_fp_init",         "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_string_s_down,         "bcore_fp_down",         "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_string_s_copy,         "bcore_fp_copy",         "copy"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_string_s_move,         "bcore_fp_move",         "move"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_string_s_create,       "bcore_fp_create",       "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_string_s_clone,        "bcore_fp_clone",        "clone"        );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_string_s_discard,      "bcore_fp_discard",      "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_string_s_copy_typed,   "bcore_fp_copy_typed",   "copy_typed"   );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_string_s_create_typed, "bcore_fp_create_typed", "create_typed" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )flow_snk,                    "bcore_fp_flow_snk",     "flow_snk"     );
    bcore_flect_self_s_push_external_func( self, ( fp_t )check_sanity,                "bcore_fp_check_sanity", "check_sanity" );
    return self;
}

/**********************************************************************************************************************/

void bcore_string_s_quicktest( void )
{
    sz_t granted_space = bcore_memory_manager_granted_space();

    bcore_life_s* life = bcore_life_s_create();
    bcore_string_s* s = bcore_life_s_push( life, bcore_string_s_discard, bcore_string_s_create() );

    ASSERT( s->size == 0 );
    ASSERT( s->space == 0 );
    bcore_string_s_copy_sc( s, "My little white elephant!" );
    ASSERT( bcore_string_s_equal_sc( s, "My little white elephant!" ) );
    bcore_string_s_replace_sc_sc( s, "white", "green" );
    ASSERT( bcore_string_s_equal_sc( s, "My little green elephant!" ) );

    bool is_green = false;
    bool is_blue  = false;
    bcore_string_s* name = bcore_life_s_push( life, bcore_string_s_discard, bcore_string_s_create() );
    sz_t idx = bcore_string_s_parsef( s, 0, s->size, "My #name #?'blue' #?'green' eleph", name, &is_blue, &is_green );
    ASSERT( bcore_string_s_equal_sc( name, "little" ) );
    ASSERT( is_green );
    ASSERT( !is_blue );
    ASSERT( bcore_strcmp( s->sc + idx, "ant!" ) == 0 );

    s2_t num0 = -12345;
    bcore_string_s_copy_sc( s, "This number is " );
    bcore_string_s_push_typed( s, typeof( "s2_t" ), &num0 );
    bcore_string_s_push_sc( s, " and an integer" );
    s3_t num1 = 0;
    idx = bcore_string_s_parsef( s, 0, s->size, "This number is #s3_t", &num1 );
    ASSERT( num1 == -12345 );
    ASSERT( bcore_strcmp( s->sc + idx, " and an integer" ) == 0 );

    bcore_life_s_discard( life );

    ASSERT( granted_space == bcore_memory_manager_granted_space() );
}
