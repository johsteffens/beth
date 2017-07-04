/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include <stdio.h>
#include "bcore_flect.h"
#include "bcore_control.h"
#include "bcore_threads.h"
#include "bcore_life.h"
#include "bcore_quicktypes.h"
#include "bcore_hmap.h"

/**********************************************************************************************************************/

const sc_t bcore_flect_caps_e_sc_arr[] =
{
    "STATIC",
    "STATIC_LINK",
    "TYPED_LINK",
    "AWARE_LINK",
    "STATIC_ARRAY",
    "TYPED_ARRAY",
    "STATIC_LINK_ARRAY",
    "TYPED_LINK_ARRAY",
    "AWARE_LINK_ARRAY",
    "EXTERNAL_DATA",
    "EXTERNAL_FUNC",
};

const sz_t bcore_flect_caps_e_size_arr[] =
{
    0, // STATIC
    sizeof( bcore_static_link_s ),
    sizeof( bcore_typed_link_s ),
    sizeof( bcore_aware_link_s ),
    sizeof( bcore_static_array_s ),
    sizeof( bcore_typed_array_s ),
    sizeof( bcore_static_link_array_s ),
    sizeof( bcore_typed_link_array_s ),
    sizeof( bcore_aware_link_array_s ),
    0,  // EXTERNAL_DATA
    0,  // EXTERNAL_FUNCTION
};

const sz_t bcore_flect_caps_e_align_arr[] =
{
    0, // STATIC
    _Alignof( bcore_static_link_s ),
    _Alignof( bcore_typed_link_s ),
    _Alignof( bcore_aware_link_s ),
    _Alignof( bcore_static_array_s ),
    _Alignof( bcore_typed_array_s ),
    _Alignof( bcore_static_link_array_s ),
    _Alignof( bcore_typed_link_array_s ),
    _Alignof( bcore_aware_link_array_s ),
    0,  // EXTERNAL_DATA
    0,  // EXTERNAL_FUNCTION
};

bool bcore_flect_caps_is_array( u2_t caps )
{
    switch( caps )
    {
        case BCORE_CAPS_STATIC:            return false;
        case BCORE_CAPS_STATIC_LINK:       return false;
        case BCORE_CAPS_TYPED_LINK:        return false;
        case BCORE_CAPS_AWARE_LINK:        return false;
        case BCORE_CAPS_STATIC_ARRAY:      return true;
        case BCORE_CAPS_TYPED_ARRAY:       return true;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return true;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return true;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return true;
        case BCORE_CAPS_EXTERNAL_DATA:     return false;
        case BCORE_CAPS_EXTERNAL_FUNC:     return false;
        default: break;
    }
    return false;
}

sc_t bcore_flect_caps_e_sc( u2_t caps )
{
    const u2_t size = sizeof( bcore_flect_caps_e_sc_arr ) / sizeof( sc_t );
    if( caps >= size ) ERR( "invalid value %u", caps );
    return bcore_flect_caps_e_sc_arr[ caps ];
}

u2_t bcore_flect_caps_e_u2( sc_t sc )
{
    const u2_t size = sizeof( bcore_flect_caps_e_sc_arr ) / sizeof( sc_t );
    for( u2_t i = 0; i < size; i++ ) if( bcore_strcmp( sc, bcore_flect_caps_e_sc_arr[ i ] ) == 0 ) return i;
    ERR( "invalid string '%s'", sc );
    return 0;
}

sz_t bcore_flect_caps_e_size( u2_t caps )
{
    const u2_t size = sizeof( bcore_flect_caps_e_size_arr ) / sizeof( sz_t );
    if( caps >= size ) ERR( "invalid value %u", caps );
    return bcore_flect_caps_e_size_arr[ caps ];
}

sz_t bcore_flect_caps_e_align( u2_t caps )
{
    const u2_t size = sizeof( bcore_flect_caps_e_align_arr ) / sizeof( sz_t );
    if( caps >= size ) ERR( "invalid value %u", caps );
    return bcore_flect_caps_e_align_arr[ caps ];
}

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT( bcore_flect_item_s )
DEFINE_FUNCTION_DOWN_FLAT( bcore_flect_item_s )
DEFINE_FUNCTION_COPY_FLAT( bcore_flect_item_s )
DEFINE_FUNCTION_MOVE (     bcore_flect_item_s )

DEFINE_FUNCTION_CREATE(    bcore_flect_item_s )
DEFINE_FUNCTION_DISCARD(   bcore_flect_item_s )
DEFINE_FUNCTION_CLONE(     bcore_flect_item_s )

bcore_string_s* bcore_flect_item_s_show( const bcore_flect_item_s* o )
{
    bcore_string_s* s = bcore_string_s_create();
    bcore_string_s_pushf( s, "bcore_flect_item_s" );
    bcore_string_s_pushf( s, "\n{" );
    bcore_string_s_pushf( s, "\n    type  : %x '%s'", o->type, ifnameof( o->type ) );
    bcore_string_s_pushf( s, "\n    name  : %x '%s'", o->name, ifnameof( o->name ) );
    bcore_string_s_pushf( s, "\n    attr  : %x '%s'", o->attr, ifnameof( o->attr ) );
    bcore_string_s_pushf( s, "\n    caps  : %s",  bcore_flect_caps_e_sc( o->caps ) );
    if( o->caps == BCORE_CAPS_EXTERNAL_DATA )
    {
        bcore_string_s_pushf( s, "\n    d_ptr : %p", o->d_ptr );
    }
    else if( o->caps == BCORE_CAPS_EXTERNAL_FUNC )
    {
        bcore_string_s_pushf( s, "\n    f_ptr : %p", o->f_ptr );
    }
    bcore_string_s_pushf( s, "\n}" );
    return s;
}

bcore_flect_item_s* bcore_flect_item_s_create_external_data( vc_t data, sc_t type, sc_t name )
{
    bcore_flect_item_s* o = bcore_flect_item_s_create();
    o->caps  = BCORE_CAPS_EXTERNAL_DATA;
    o->d_ptr = data;
    o->type  = typeof( type );
    o->name  = bcore_name_enroll( name );
    return o;
}

bcore_flect_item_s* bcore_flect_item_s_create_external_func( fp_t func, sc_t type, sc_t name )
{
    bcore_flect_item_s* o = bcore_flect_item_s_create();
    o->caps  = BCORE_CAPS_EXTERNAL_FUNC;
    o->f_ptr = func;
    o->type  = typeof( type );
    o->name  = bcore_name_enroll( name );
    return o;
}

sz_t bcore_flect_aligned_offset( sz_t align, sz_t raw_offset )
{
    if( align < 2 ) return raw_offset;
    sz_t offset = raw_offset;
    if( ( offset & ( align - 1 ) ) != 0 ) offset = ( offset + align ) & ~( align - 1 );
    return offset;
}

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT( bcore_flect_body_s )

void bcore_flect_body_s_down( bcore_flect_body_s* o )
{
    if( o->data )
    {
        for( sz_t i = 0; i < o->size; i++ ) bcore_flect_item_s_down( &o->data[ i ] );
        o->data = bcore_un_alloc( sizeof( bcore_flect_item_s ), o->data, o->space, 0, &o->space );
    }
}

void bcore_flect_body_s_copy( bcore_flect_body_s* o, const bcore_flect_body_s* src )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_flect_item_s_down( &o->data[ i ] );
    o->data = bcore_un_alloc( sizeof( bcore_flect_item_s ), o->data, o->space,         0, &o->space );
    o->data = bcore_un_alloc( sizeof( bcore_flect_item_s ), o->data, o->space, src->size, &o->space );
    for( sz_t i = 0; i < src->size; i++ )
    {
        bcore_flect_item_s_init( &o->data[ i ] );
        bcore_flect_item_s_copy( &o->data[ i ], &src->data[ i ] );
    }
    o->size = src->size;
}

DEFINE_FUNCTION_CREATE(  bcore_flect_body_s )
DEFINE_FUNCTION_DISCARD( bcore_flect_body_s )
DEFINE_FUNCTION_CLONE(   bcore_flect_body_s )

void bcore_flect_body_s_push( bcore_flect_body_s* o, const bcore_flect_item_s* item )
{
    if( o->size == o->space )
    {
        sz_t old_space = o->space;
        bcore_flect_item_s* old_data = o->data;
        o->data = bcore_u_alloc( sizeof( bcore_flect_item_s ), NULL, o->space > 0 ? o->space * 2 : 1, &o->space );
        for( sz_t i = 0; i < o->size; i++ ) bcore_flect_item_s_move( &o->data[ i ], &old_data[ i ] );
        bcore_un_alloc( sizeof( bcore_flect_item_s ), old_data, old_space, 0, NULL );
    }
    bcore_flect_item_s_init( &o->data[ o->size ] );
    bcore_flect_item_s_copy( &o->data[ o->size ], item );
    o->size++;
}

void bcore_flect_body_s_push_d( bcore_flect_body_s* o, bcore_flect_item_s* item )
{
    bcore_flect_body_s_push( o, item );
    bcore_flect_item_s_discard( item );
}

bcore_string_s* bcore_flect_body_s_show( const bcore_flect_body_s* o )
{
    bcore_string_s* s = bcore_string_s_create();
    bcore_string_s_pushf( s, "bcore_flect_body_s" );
    bcore_string_s_pushf( s, "\n{" );
    for( sz_t i = 0; i < o->size; i++ )
    {
        bcore_string_s_pushf( s, "\n    data[%lu]:", i );
        bcore_string_s_push_string_d( s, bcore_string_s_replace_char_sc( bcore_flect_item_s_show( &o->data[ i ] ), '\n', "\n    " ) );
    }
    bcore_string_s_pushf( s, "\n}" );
    return s;
}

bcore_flect_body_s* bcore_flect_body_s_build_parse( const bcore_string_s* text, sz_t* p_idx )
{
    bcore_life_s* life = bcore_life_s_create();
    bcore_flect_body_s* o = bcore_flect_body_s_create();
    sz_t idx = ( p_idx != NULL ) ? *p_idx : 0;
    idx = bcore_string_s_parsef( text, idx, text->size, " { " );

    while( text->sc[ idx ] != '}' )
    {
        bcore_life_s* life = bcore_life_s_create();
        bcore_string_s* name1 = bcore_string_s_create_l( life );
        bcore_string_s* name2 = bcore_string_s_create_l( life );
        bcore_flect_item_s* item = bcore_life_s_push( life, bcore_flect_item_s_discard, bcore_flect_item_s_create() );
        bool private, cyclic, external, link, arr;
        idx = bcore_string_s_parsef( text, idx, text->size, "#?'private' #?'cyclic' #?'external' #name #?'*' #?'[]' #name ; ",  &private, &cyclic, &external, name1, &link, &arr, name2 );
        if( ( int )private + cyclic + external > 1 )
        {
            bcore_string_s* context = bcore_string_s_show_line_context( text, bcore_string_s_find_sc( text, idx, 0, ";" ) );
            ERR( "\n%s\nAttributes are mutually exclusive. Choose maximally one attribute.", context->sc );
        }

        if( cyclic && !link )
        {
            bcore_string_s* context = bcore_string_s_show_line_context( text, bcore_string_s_find_sc( text, idx, 0, "cyclic" ) );
            ERR( "\n%s\nOnly links (*) can be cyclic.", context->sc );
        }

        if( external && !link )
        {
            bcore_string_s* context = bcore_string_s_show_line_context( text, bcore_string_s_find_sc( text, idx, 0, "external" ) );
            ERR( "\n%s\nOnly links (*) can be external.", context->sc );
        }

        item->attr = private ? TYPEOF_private : cyclic ? TYPEOF_cyclic : 0;

        if( bcore_string_s_equal_sc( name1, "typed" ) )
        {
            if( !link && !arr )
            {
                bcore_string_s* context = bcore_string_s_show_line_context( text, bcore_string_s_find_sc( text, idx, 0, "typed" ) );
                ERR( "\n%s\nTyped objects cannot be embedded. Use 'typed *' to clarify method of referencing.", context->sc );
            }
            item->type = 0; //or typeof( "vd_t" );
            item->name = bcore_name_enroll( name2->sc );
            item->caps = arr ? ( link ? BCORE_CAPS_TYPED_LINK_ARRAY : BCORE_CAPS_TYPED_ARRAY ) : BCORE_CAPS_TYPED_LINK;
        }
        else if( bcore_string_s_equal_sc( name1, "aware" ) )
        {
            if( !link )
            {
                bcore_string_s* context = bcore_string_s_show_line_context( text, bcore_string_s_find_sc( text, idx, 0, "aware" ) );
                ERR( "\n%s\nSelf-aware objects must be referenced by a link. Use 'aware *' to clarify method of referencing.", context->sc );
            }
            item->type = 0; //or typeof( "vd_t" );
            item->name = bcore_name_enroll( name2->sc );
            item->caps = arr ? BCORE_CAPS_AWARE_LINK_ARRAY : BCORE_CAPS_AWARE_LINK;
        }
        else
        {
            item->type = typeof( name1->sc );
            item->name = bcore_name_enroll( name2->sc );
            item->caps = arr ? ( link ? BCORE_CAPS_STATIC_LINK_ARRAY : BCORE_CAPS_STATIC_ARRAY ) : ( link ? BCORE_CAPS_STATIC_LINK : BCORE_CAPS_STATIC );
        }

        if( item->type == typeof( "aware_t" ) )
        {
            if( o->size > 0 )
            {
                bcore_string_s* context = bcore_string_s_show_line_context( text, idx );
                ERR( "\n%s\n'aware-t' must be first element in body and not used elsewhere.", context->sc );
            }
        }
        bcore_flect_body_s_push( o, item );
        bcore_life_s_discard( life );
    }

    if ( p_idx != NULL ) *p_idx = idx;
    bcore_life_s_discard( life );
    return o;
}

/**********************************************************************************************************************/
// bcore_flect_self_s

void bcore_flect_self_s_init( bcore_flect_self_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_flect_self_s;
}

void bcore_flect_self_s_down( bcore_flect_self_s* o )
{
    bcore_flect_body_s_discard( o->body );
}

void bcore_flect_self_s_copy( bcore_flect_self_s* o, const bcore_flect_self_s* src )
{
    bcore_flect_body_s_discard( o->body );
    bcore_memcpy( o, src, sizeof( bcore_flect_self_s ) );
    o->body = bcore_flect_body_s_clone( src->body );
}

bcore_flect_self_s* bcore_flect_self_s_create()
{
    bcore_flect_self_s* o = bcore_alloc( NULL, sizeof( bcore_flect_self_s ) );
    bcore_flect_self_s_init( o );
    return o;
}

bcore_flect_self_s* bcore_flect_self_s_create_plain( tp_t type, sz_t size )
{
    bcore_flect_self_s* o = bcore_flect_self_s_create();
    bcore_flect_self_s_init_plain( o, type, size );
    return o;
}

bcore_flect_self_s* bcore_flect_self_s_clone( const bcore_flect_self_s* o )
{
    bcore_flect_self_s* dst = bcore_flect_self_s_create();
    bcore_flect_self_s_copy( dst, o );
    return dst;
}

void bcore_flect_self_s_discard( bcore_flect_self_s* o )
{
    if( !o ) return;
    bcore_flect_self_s_down( o );
    bcore_free( o );
}

void bcore_flect_self_s_push( bcore_flect_self_s* o, const bcore_flect_item_s* item )
{
    if( !o->body ) o->body = bcore_flect_body_s_create();
    bcore_flect_body_s_push( o->body, item );
}

void bcore_flect_self_s_push_d( bcore_flect_self_s* o, bcore_flect_item_s* item )
{
    bcore_flect_self_s_push( o, item );
    bcore_flect_item_s_discard( item );
}

void bcore_flect_self_s_push_external_data( bcore_flect_self_s* o, vc_t data, sc_t type, sc_t name )
{
    bcore_flect_self_s_push_d( o, bcore_flect_item_s_create_external_data( data, type, name ) );
}

void bcore_flect_self_s_push_external_func( bcore_flect_self_s* o, fp_t func, sc_t type, sc_t name )
{
    bcore_flect_self_s_push_d( o, bcore_flect_item_s_create_external_func( func, type, name ) );
}

void bcore_flect_self_s_init_plain( bcore_flect_self_s* o, tp_t type, sz_t size )
{
    bcore_flect_self_s_init( o );
    o->type  = type;
    o->size  = size;
    if( o->body ) bcore_flect_body_s_discard( o->body );
    o->body = NULL;
}

bcore_string_s* bcore_flect_self_s_show( const bcore_flect_self_s* o )
{
    bcore_string_s* s = bcore_string_s_create();
    bcore_string_s_pushf( s, "bcore_flect_self_s" );
    bcore_string_s_pushf( s, "\n{" );
    bcore_string_s_pushf( s, "\n    type:  %x '%s'", o->type, ifnameof( o->type ) );
    bcore_string_s_pushf( s, "\n    size:  %zu", o->size );
    bcore_string_s_pushf( s, "\n    body: " );
    if( o->body ) bcore_string_s_push_string_d( s, bcore_string_s_replace_char_sc( bcore_flect_body_s_show( o->body ), '\n', "\n    " ) );
    bcore_string_s_pushf( s, "\n}" );
    return s;
}

bcore_flect_self_s* bcore_flect_self_s_build_parse( const bcore_string_s* text, sz_t* p_idx, sz_t size_of )
{
    bcore_life_s* life = bcore_life_s_create();
    bcore_flect_self_s* o = bcore_flect_self_s_create();

    sz_t idx = ( p_idx != NULL ) ? *p_idx : 0;

    bcore_string_s* name = bcore_string_s_create_l( life );
    idx = bcore_string_s_parsef( text, idx, text->size, " #name = ", name );
    o->type  = bcore_name_enroll( name->sc );
    if( text->sc[ idx ] == '{' )
    {
        o->body  = bcore_flect_body_s_build_parse( text, &idx );
    }
    else
    {
        idx = bcore_string_s_parsef( text, idx, text->size, "#name", name );
        const bcore_flect_self_s* self_l = bcore_flect_try_self( typeof( name->sc ) );
        if( !self_l )
        {
            bcore_string_s* context = bcore_string_s_show_line_context( text, idx );
            ERR( "\n%s\nType %s not defined", context->sc, name->sc );
        }
        o->body  = bcore_flect_body_s_clone( self_l->body );
        o->size  = self_l->size;
    }

    if( p_idx != NULL ) *p_idx = idx;
    o->size = size_of;
    bcore_life_s_discard( life );
    return o;
}

bcore_flect_self_s* bcore_flect_self_s_build_parse_sc( sc_t text, sz_t size_of )
{
    bcore_string_s* string = bcore_string_s_create_sc( text );
    bcore_flect_self_s* ret = bcore_flect_self_s_build_parse( string, NULL, size_of );
    bcore_string_s_discard( string );
    return ret;
}

bcore_flect_self_s* bcore_flect_self_s_create_self()
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_flect_self_s" ), sizeof( bcore_flect_self_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_flect_self_s_init,         "bcore_fp_init",         "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_flect_self_s_down,         "bcore_fp_down",         "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_flect_self_s_copy,         "bcore_fp_copy",         "copy"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_flect_self_s_create,       "bcore_fp_create",       "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_flect_self_s_clone,        "bcore_fp_clone",        "clone"        );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bcore_flect_self_s_discard,      "bcore_fp_discard",      "discard"      );
    return self;
}

/**********************************************************************************************************************/
// map of functions creating a self-reflection

typedef struct creator_map_s
{
    bcore_hmap_u2vd_s* hmap;
    bcore_mutex_t mutex;
} creator_map_s;

static void creator_map_s_init( creator_map_s* o )
{
    o->hmap = bcore_hmap_u2vd_s_create();
    bcore_mutex_init( &o->mutex );
}

static void creator_map_s_down( creator_map_s* o )
{
    bcore_mutex_lock( &o->mutex );
    bcore_hmap_u2vd_s_discard( o->hmap );
    bcore_mutex_unlock( &o->mutex );
    bcore_mutex_down( &o->mutex );
}

static creator_map_s* creator_map_s_create()
{
    creator_map_s* o = bcore_alloc( NULL, sizeof( creator_map_s ) );
    creator_map_s_init( o );
    return o;
}

static void creator_map_s_discard( creator_map_s* o )
{
    if( !o ) return;
    creator_map_s_down( o );
    bcore_free( o );
}

static void creator_map_s_insert( creator_map_s* o, tp_t type, bcore_flect_create_self_fp fp )
{
    bcore_mutex_lock( &o->mutex );
    if( bcore_hmap_u2vd_s_exists( o->hmap, type ) ) ERR( "'%s' (%"PRIu32") is already defined", ifnameof( type ), type );
    if( sizeof( fp ) > sizeof( vd_t ) )
    {
        ERR( "Platform specific error: Cannot convert function pointer into data pointer." );
    }
    bcore_hmap_u2vd_s_set( o->hmap, type, ( vd_t )fp, false );
    bcore_mutex_unlock( &o->mutex );
}

static const bcore_flect_create_self_fp creator_map_s_get_fp( creator_map_s* o, tp_t type )
{
    bcore_mutex_lock( &o->mutex );
    vd_t* p_val = bcore_hmap_u2vd_s_get( o->hmap, type );
    bcore_mutex_unlock( &o->mutex );
    return p_val ? ( bcore_flect_create_self_fp )*p_val : NULL;
}

static bool creator_map_s_exists( creator_map_s* o, tp_t type )
{
    bcore_mutex_lock( &o->mutex );
    bool ret = bcore_hmap_u2vd_s_exists( o->hmap, type );
    bcore_mutex_unlock( &o->mutex );
    return ret;
}

/**********************************************************************************************************************/
// map of self reflections

typedef struct self_map_s
{
    bcore_hmap_u2vd_s* hmap;
    bcore_mutex_t mutex;
} self_map_s;

static void self_map_s_init( self_map_s* o )
{
    o->hmap = bcore_hmap_u2vd_s_create();
    bcore_mutex_init( &o->mutex );
}

static void self_map_s_down( self_map_s* o )
{
    bcore_mutex_lock( &o->mutex );
    if( o->hmap )
    {
        // We manually detach objects here to keep hmap from invoking
        // perspective management, which may (already) be down at this point.
        sz_t hmap_size = bcore_hmap_u2vd_s_size( o->hmap );
        for( sz_t i = 0; i < hmap_size; i++ )
        {
            if( bcore_hmap_u2vd_s_idx_holds( o->hmap, i ) )
            {
                tp_t key = bcore_hmap_u2vd_s_idx_key( o->hmap, i );
                bcore_flect_self_s_discard( bcore_hmap_u2vd_s_detach_h( o->hmap, key ) );
            }
        }
        bcore_hmap_u2vd_s_discard( o->hmap );
    }
    bcore_mutex_unlock( &o->mutex );
    bcore_mutex_down( &o->mutex );
}

static self_map_s* self_map_s_create()
{
    self_map_s* o = bcore_alloc( NULL, sizeof( self_map_s ) );
    self_map_s_init( o );
    return o;
}

static void self_map_s_discard( self_map_s* o )
{
    if( !o ) return;
    self_map_s_down( o );
    bcore_free( o );
}

static void self_map_s_insert( self_map_s* o, tp_t type, bcore_flect_self_s* self, bool hold_self )
{
    bcore_mutex_lock( &o->mutex );
    if( bcore_hmap_u2vd_s_exists( o->hmap, type ) ) ERR( "'%s' (%"PRIu32") is already defined", ifnameof( type ), type );
    bcore_hmap_u2vd_s_set( o->hmap, type, self, hold_self );
    bcore_mutex_unlock( &o->mutex );
}

static const bcore_flect_self_s* self_map_s_get_self( self_map_s* o, tp_t type )
{
    bcore_mutex_lock( &o->mutex );
    vd_t* p_val = bcore_hmap_u2vd_s_get( o->hmap, type );
    bcore_mutex_unlock( &o->mutex );
    return p_val ? *p_val : NULL;
}

static bool self_map_s_exists( self_map_s* o, tp_t type )
{
    bcore_mutex_lock( &o->mutex );
    bool ret = bcore_hmap_u2vd_s_exists( o->hmap, type );
    bcore_mutex_unlock( &o->mutex );
    return ret;
}

/**********************************************************************************************************************/
static self_map_s*    self_map_s_g    = NULL;
static creator_map_s* creator_map_s_g = NULL;

void bcore_create_flect_maps()
{
    if( !self_map_s_g    ) self_map_s_g    = self_map_s_create();
    if( !creator_map_s_g ) creator_map_s_g = creator_map_s_create();
}

void bcore_discard_flect_maps()
{
    if( self_map_s_g )
    {
        self_map_s_discard( self_map_s_g );
        self_map_s_g = NULL;
    }
    if( creator_map_s_g )
    {
        creator_map_s_discard( creator_map_s_g );
        creator_map_s_g = NULL;
    }
}

void bcore_flect_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, bcore_create_flect_maps );
}

void bcore_flect_close()
{
    bcore_discard_flect_maps();
}

bool bcore_flect_exists( tp_t type )
{
    assert( self_map_s_g != NULL );
    if( self_map_s_exists(    self_map_s_g,    type ) ) return true;
    assert( creator_map_s_g != NULL );
    if( creator_map_s_exists( creator_map_s_g, type ) ) return true;
    return false;
}

const bcore_flect_self_s* bcore_flect_try_self( tp_t type )
{
    assert( self_map_s_g != NULL );
    const bcore_flect_self_s* self = self_map_s_get_self( self_map_s_g, type );
    if( !self )
    {
        assert( creator_map_s_g != NULL );
        bcore_flect_create_self_fp fp = creator_map_s_get_fp( creator_map_s_g, type );
        if( fp )
        {
            self_map_s_insert( self_map_s_g, type, fp(), true );
            self = self_map_s_get_self( self_map_s_g, type );
        }
    }
    return self;
}

const bcore_flect_self_s* bcore_flect_get_self( tp_t type )
{
    const bcore_flect_self_s* self = bcore_flect_try_self( type );
    if( !self )
    {
        const char* name = bcore_name_try_name( type );
        if( !name )
        {
            ERR( "type '0x%x'; has neither name nor self-reflection", type );
        }
        else
        {
            ERR( "type '%s' has no self-reflection", name );
        }
    }
    return self;
}

void bcore_flect_define_self_d( bcore_flect_self_s* self )
{
    assert( self_map_s_g != NULL );
    self_map_s_insert( self_map_s_g, self->type, self, true );
}

void bcore_flect_define_self_c( const bcore_flect_self_s* self )
{
    bcore_flect_define_self_d( bcore_flect_self_s_clone( self ) );
}

sz_t bcore_flect_parse( const bcore_string_s* string, sz_t idx )
{
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse( string, &idx, 0 ) );
    return idx;
}

sc_t bcore_flect_parse_sc( sc_t sc )
{
    bcore_string_s* string = bcore_string_s_create_sc( sc );
    sz_t idx = 0;
    idx = bcore_flect_parse( string, idx );
    bcore_string_s_discard( string );
    return sc + idx;
}

void bcore_flect_define_creator( tp_t type, bcore_flect_create_self_fp creator )
{
    assert( creator_map_s_g != NULL );
    creator_map_s_insert( creator_map_s_g, type, creator );
}

void bcore_flect_define_alias( tp_t alias, tp_t type )
{
    if( !self_map_s_g ) bcore_flect_open();
    bcore_flect_self_s* self = ( bcore_flect_self_s* )bcore_flect_try_self( type );
    if( !self )
    {
        const char* name = bcore_name_try_name( type );
        if( !name )
        {
            ERR( "type '%u; has neither name nor self-reflection", type );
        }
        else
        {
            ERR( "type '%s' has no self-reflection", name );
        }
    }
    self_map_s_insert( self_map_s_g, alias, self, false );
}

void bcore_flect_define_alias_sc( sc_t alias, sc_t type )
{
    tp_t alias_l = bcore_name_enroll( alias );
    tp_t type_l = bcore_name_get_hash( type );
    bcore_flect_define_alias( alias_l, type_l );
}

/**********************************************************************************************************************/

void bcore_flect_define_basics()
{
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "s3_t" ), sizeof( s3_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "s2_t" ), sizeof( s2_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "s1_t" ), sizeof( s1_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "s0_t" ), sizeof( s0_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "u3_t" ), sizeof( u3_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "u2_t" ), sizeof( u2_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "u1_t" ), sizeof( u1_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "u0_t" ), sizeof( u0_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "f3_t" ), sizeof( f3_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "f2_t" ), sizeof( f2_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "sz_t" ), sizeof( sz_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "sd_t" ), sizeof( sd_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "sc_t" ), sizeof( sc_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "vd_t" ), sizeof( vd_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "vc_t" ), sizeof( vc_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "fp_t" ), sizeof( fp_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "bool" ), sizeof( bool ) ) );

    bcore_flect_define_alias( bcore_name_enroll( "int64_t" ),  typeof( "s3_t" ) );
    bcore_flect_define_alias( bcore_name_enroll( "int32_t" ),  typeof( "s2_t" ) );
    bcore_flect_define_alias( bcore_name_enroll( "int16_t" ),  typeof( "s1_t" ) );
    bcore_flect_define_alias( bcore_name_enroll( "int8_t"  ),  typeof( "s0_t" ) );
    bcore_flect_define_alias( bcore_name_enroll( "uint64_t" ), typeof( "u3_t" ) );
    bcore_flect_define_alias( bcore_name_enroll( "uint32_t" ), typeof( "u2_t" ) );
    bcore_flect_define_alias( bcore_name_enroll( "uint16_t" ), typeof( "u1_t" ) );
    bcore_flect_define_alias( bcore_name_enroll( "uint8_t" ),  typeof( "u0_t" ) );
    bcore_flect_define_alias( bcore_name_enroll( "double" ),   typeof( "f3_t" ) );
    bcore_flect_define_alias( bcore_name_enroll( "float" ),    typeof( "f2_t" ) );
    bcore_flect_define_alias( bcore_name_enroll( "size_t" ),   typeof( "sz_t" ) );

    // special purpose types
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "tp_t"    ), sizeof( tp_t    ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "aware_t" ), sizeof( aware_t ) ) );

    // special purpose functions
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_fp_init"    ), sizeof( bcore_fp_init ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_fp_down"    ), sizeof( bcore_fp_down ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_fp_copy"    ), sizeof( bcore_fp_copy ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_fp_move"    ), sizeof( bcore_fp_move ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_fp_create"  ), sizeof( bcore_fp_create ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_fp_discard" ), sizeof( bcore_fp_discard ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_fp_clone"   ), sizeof( bcore_fp_clone ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_fp_copy_typed"   ), sizeof( bcore_fp_copy_typed ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_fp_create_typed" ), sizeof( bcore_fp_create_typed ) ) );

    // encapsulation structure
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " bcore_static_link_s       = { vd_t link; }",            sizeof( bcore_static_link_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " bcore_typed_link_s        = { vd_t link; tp_t type; }", sizeof( bcore_typed_link_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " bcore_aware_link_s        = { vd_t link; }",            sizeof( bcore_aware_link_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " bcore_static_array_s      = { vd_t  data; sz_t size; sz_t space; }", sizeof( bcore_static_array_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " bcore_typed_array_s       = { typed []; }",             sizeof( bcore_typed_array_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " bcore_static_link_array_s = { vd_t* data; sz_t size; sz_t space; }", sizeof( bcore_static_link_array_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " bcore_typed_link_array_s  = { typed* []; }",            sizeof( bcore_typed_link_array_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " bcore_aware_link_array_s  = { aware* []; }",            sizeof( bcore_aware_link_array_s ) ) );

    // specific objects
    bcore_flect_define_creator( typeof( "bcore_flect_self_s" ), bcore_flect_self_s_create_self ); // self
    bcore_flect_define_creator( typeof( "bcore_string_s" ),     bcore_string_s_create_self     ); // string
}

/**********************************************************************************************************************/

bcore_string_s* bcore_flect_selftest()
{
    {
        bcore_flect_parse_sc(" teabag =    { u3_t leaves; s1_t flavor; s0_t color;  }" );
        bcore_flect_parse_sc(" container = { u3_t elements; teabag*[] bags; u1_t flags; }" );
        bcore_flect_parse_sc(" delivery =  { aware_t _; u3_t tag; container* cargo; aware*[] attachments; }" );
        bcore_flect_parse_sc(" another = delivery" );
    }

    bcore_string_s* s = bcore_string_s_create();
    bcore_string_s_push_string_d( s, bcore_flect_self_s_show( bcore_flect_get_self( typeof( "bcore_string_s" ) ) ) );


    return s;
}

/**********************************************************************************************************************/

