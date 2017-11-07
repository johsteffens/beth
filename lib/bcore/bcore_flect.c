/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include <stdio.h>
#include "bcore_flect.h"
#include "bcore_control.h"
#include "bcore_threads.h"
#include "bcore_life.h"
#include "bcore_quicktypes.h"
#include "bcore_hmap.h"
#include "bcore_signature.h"
#include "bcore_tbman.h"
#include "bcore_trait.h"

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

bl_t bcore_flect_caps_is_array( u2_t caps )
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

st_s* bcore_flect_item_s_show( const bcore_flect_item_s* o )
{
    st_s* s = st_s_create();
    st_s_push_fa( s, "bcore_flect_item_s" );
    st_s_push_fa( s, "\n{" );
    st_s_push_fa( s, "\n    type    : #<tp_t> '#<sc_t>'", o->type, ifnameof( o->type ) );
    st_s_push_fa( s, "\n    name    : #<tp_t> '#<sc_t>'", o->name, ifnameof( o->name ) );
    st_s_push_fa( s, "\n    flags   : #<tp_t>",  o->flags );
    st_s_push_fa( s, "\n    caps    : #<sc_t>",  bcore_flect_caps_e_sc( o->caps ) );
    st_s_push_fa( s, "\n    default : #<s3_t>",  o->default_s3 );
    st_s_pushf( s, "\n}" );
    return s;
}

bcore_flect_item_s* bcore_flect_item_s_create_func( sc_t fname, fp_t func, sc_t type, sc_t name )
{
    bcore_flect_item_s* o = bcore_flect_item_s_create();
    o->caps  = BCORE_CAPS_EXTERNAL_FUNC;
    o->type  = entypeof( type );
    o->name  = entypeof( name );
    o->default_tp = bcore_function_set_sc( fname, func );
    return o;
}

sz_t bcore_flect_aligned_offset( sz_t align, sz_t raw_offset )
{
    if( align < 2 ) return raw_offset;
    sz_t offset = raw_offset;
    if( ( offset & ( align - 1 ) ) != 0 ) offset = ( offset + align ) & ~( align - 1 );
    return offset;
}

bcore_signature_s* bcore_flect_item_s_push_to_signature( const bcore_flect_item_s* o, bcore_signature_s* sig )
{
    bcore_signature_s_push( sig, o->type );
    bcore_signature_s_push( sig, o->name );
    tp_t t = o->caps;
    bcore_signature_s_push( sig, t );
    t = o->default_u3;
    bcore_signature_s_push( sig, t );
    if( sizeof( t ) < sizeof( o->default_u3 ) )
    {
        t = o->default_u3 >> ( sizeof( t ) * 8 );
        bcore_signature_s_push( sig, t );
    }
    return sig;
}

s2_t bcore_flect_item_s_cmp( const bcore_flect_item_s* o1, const bcore_flect_item_s* o2 )
{
    if( o1->type       != o2->type       ) return ( o1->type < o2->type ) ? 1 : -1;
    if( o1->name       != o2->name       ) return ( o1->name < o2->name ) ? 1 : -1;
    if( o1->caps       != o2->caps       ) return ( o1->caps < o2->caps ) ? 1 : -1;
    if( o1->flags      != o2->flags      ) return ( o1->flags < o2->flags ) ? 1 : -1;
    if( o1->default_u3 != o2->default_u3 ) return ( o1->default_u3 < o2->default_u3 ) ? 1 : -1;
    return 0;
}

static bcore_flect_self_s* flect_item_s_create_self( void )
{
    sc_t def =
    "bcore_flect_item_s ="
    "{"
        "tp_t type;"    // hash of type
        "tp_t name;"    // hash of name
        "u2_t caps;"    // data encapsulation
        "tp_t flags;"   // collection of attribute flags
        "u3_t default;" // container of default data
    "}";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_flect_item_s ) );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_item_s_init,    "bcore_fp_init",    "init"    );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_item_s_down,    "bcore_fp_down",    "down"    );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_item_s_copy,    "bcore_fp_copy",    "copy"    );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_item_s_move,    "bcore_fp_move",    "move"    );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_item_s_create,  "bcore_fp_create",  "create"  );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_item_s_clone,   "bcore_fp_clone",   "clone"   );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_item_s_discard, "bcore_fp_discard", "discard" );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_item_s_cmp,     "bcore_fp_compare", "cmp"     );
    return self;
}

/**********************************************************************************************************************/

void bcore_flect_body_s_init( bcore_flect_body_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->complete = true;
}

void bcore_flect_body_s_down( bcore_flect_body_s* o )
{
    bcore_release_obj_arr( ( fp_t )bcore_flect_item_s_down, o->data, o->size, sizeof( bcore_flect_item_s ) );
    o->data  = NULL;
    o->size  = 0;
    o->space = 0;
}

void bcore_flect_body_s_copy( bcore_flect_body_s* o, const bcore_flect_body_s* src )
{
    bcore_release_obj_arr( ( fp_t )bcore_flect_item_s_down, o->data, o->size, sizeof( bcore_flect_item_s ) );
    o->data = NULL;
    o->size = 0;
    o->space = 0;

    o->data = bcore_un_alloc( sizeof( bcore_flect_item_s ), o->data, o->space, src->size, &o->space );
    for( sz_t i = 0; i < src->size; i++ )
    {
        bcore_flect_item_s_init( &o->data[ i ] );
        bcore_flect_item_s_copy( &o->data[ i ], &src->data[ i ] );
    }
    o->size = src->size;
    o->complete = src->complete;
}

DEFINE_FUNCTION_CREATE(  bcore_flect_body_s )
DEFINE_FUNCTION_DISCARD( bcore_flect_body_s )
DEFINE_FUNCTION_CLONE(   bcore_flect_body_s )

void bcore_flect_body_s_push( bcore_flect_body_s* o, const bcore_flect_item_s* item )
{
    if( o->size == o->space )
    {
        bcore_flect_item_s* old_data = o->data;
        o->data = bcore_u_alloc( sizeof( bcore_flect_item_s ), NULL, o->space > 0 ? o->space * 2 : 1, &o->space );
        for( sz_t i = 0; i < o->size; i++ ) bcore_flect_item_s_move( &o->data[ i ], &old_data[ i ] );
        bcore_release( old_data );
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

st_s* bcore_flect_body_s_show( const bcore_flect_body_s* o )
{
    st_s* s = st_s_create();
    st_s_pushf( s, "bcore_flect_body_s" );
    st_s_pushf( s, "\n{" );
    for( sz_t i = 0; i < o->size; i++ )
    {
        st_s_pushf( s, "\n    data[%lu]:", i );
        st_s_push_st_d( s, st_s_replace_char_sc( bcore_flect_item_s_show( &o->data[ i ] ), '\n', "\n    " ) );
    }
    st_s_pushf( s, "\n}" );
    return s;
}

bcore_flect_body_s* bcore_flect_body_s_build_parse( const st_s* text, sz_t* p_idx )
{
    bcore_life_s* life = bcore_life_s_create();
    bcore_flect_body_s* o = bcore_flect_body_s_create();
    sz_t idx = ( p_idx != NULL ) ? *p_idx : 0;
    idx = st_s_parse_fa( text, idx, text->size, " { " );

    while( text->sc[ idx ] != '}' )
    {
        bcore_life_s* life = bcore_life_s_create();
        st_s* type_name = st_s_create_l( life );
        st_s* item_name = st_s_create_l( life );
        bcore_flect_item_s* item = bcore_life_s_push( life, ( bcore_fp_discard )bcore_flect_item_s_discard, bcore_flect_item_s_create() );
        bl_t f_private = false, f_hidden = false, f_shell = false, f_func = false, f_link = false, f_arr = false;

        idx = st_s_parse_fa( text, idx, text->size, " #?'func' ", &f_func );

        if( f_func ) // function declaration
        {
            st_s* assign_name = st_s_create_l( life );
            idx = st_s_parse_fa( text, idx, text->size, "#name #name = #name", type_name, item_name, assign_name );
            item->type = bcore_name_enroll( type_name->sc );
            item->name = bcore_name_enroll( item_name->sc );
            item->caps = BCORE_CAPS_EXTERNAL_FUNC;
            item->default_tp = bcore_name_enroll( item_name->sc );
        }
        else // data type declaration
        {
            idx = st_s_parse_fa( text, idx, text->size, "#?'private' #?'hidden' #?'shell'",  &f_private, &f_hidden, &f_shell );

            // type can be specified by explicit type id number (anonymous types) or by name
            tp_t type_val = 0;
            idx = st_s_parse_fa( text, idx, text->size, " " );
            if( text->data[ idx ] == '{' ) // nested anonymous type
            {
                type_val = bcore_flect_type_parse( text, &idx );
            }
            else if( text->data[ idx ] >= '0' && text->data[ idx ] <= '9' ) // type is specified by number
            {
                idx = st_s_parse_fa( text, idx, text->size, "#tp_t ",  &type_val );
            }
            else if( text->data[ idx ] == '.' && text->data[ idx + 1 ] == '.' && text->data[ idx + 2 ] == '.' )
            {
                o->complete = false;
                idx += 3;
                bcore_life_s_discard( life );
                break;
            }
            else // type is specified by name
            {
                idx = st_s_parse_fa( text, idx, text->size, "#name ",  type_name );
            }

            bl_t assign_default = false;

            idx = st_s_parse_fa( text, idx, text->size, "#?'*' #?'[]' #name #?'=' ", &f_link, &f_arr, item_name, &assign_default );

            item->f_private  = f_private;
            item->f_hidden   = f_hidden;
            item->f_shell    = f_shell;

            if( st_s_equal_sc( type_name, "typed" ) )
            {
                if( !f_link && !f_arr )
                {
                    st_s* context = st_s_show_line_context( text, st_s_find_sc( text, idx, 0, "typed" ) );
                    ERR( "\n%s\nTyped objects cannot be nested. Use 'typed *' to clarify method of referencing.", context->sc );
                }
                item->type = 0;
                item->name = bcore_name_enroll( item_name->sc );
                item->caps = f_arr ? ( f_link ? BCORE_CAPS_TYPED_LINK_ARRAY : BCORE_CAPS_TYPED_ARRAY ) : BCORE_CAPS_TYPED_LINK;
            }
            else if( st_s_equal_sc( type_name, "aware" ) )
            {
                if( !f_link )
                {
                    st_s* context = st_s_show_line_context( text, st_s_find_sc( text, idx, 0, "aware" ) );
                    ERR( "\n%s\nSelf-aware objects must be referenced by a link. Use 'aware *' to clarify method of referencing.", context->sc );
                }
                item->type = 0;
                item->name = bcore_name_enroll( item_name->sc );
                item->caps = f_arr ? BCORE_CAPS_AWARE_LINK_ARRAY : BCORE_CAPS_AWARE_LINK;
            }
            else
            {
                item->type = ( type_val > 0 ) ? type_val : bcore_name_enroll( type_name->sc );
                item->name = bcore_name_enroll( item_name->sc );
                item->caps = f_arr ? ( f_link ? BCORE_CAPS_STATIC_LINK_ARRAY : BCORE_CAPS_STATIC_ARRAY ) : ( f_link ? BCORE_CAPS_STATIC_LINK : BCORE_CAPS_STATIC );
            }

            if( item->type == typeof( "aware_t" ) )
            {
                if( o->size > 0 )
                {
                    st_s* context = st_s_show_line_context( text, idx );
                    ERR( "\n%s\n'aware-t' must be first element in body and not used elsewhere.", context->sc );
                }
            }
            if( assign_default )
            {
                if( item->caps == BCORE_CAPS_STATIC )
                {
                    switch( item->type )
                    {
                        case TYPEOF_s0_t:
                        case TYPEOF_s1_t:
                        case TYPEOF_s2_t:
                        case TYPEOF_s3_t:
                            idx = st_s_parse_fa( text, idx, text->size, "#s3_t", &item->default_s3 );
                            break;
                        case TYPEOF_u0_t:
                        case TYPEOF_u1_t:
                        case TYPEOF_u2_t:
                        case TYPEOF_u3_t:
                        case TYPEOF_sz_t:
                        case TYPEOF_tp_t:
                            idx = st_s_parse_fa( text, idx, text->size, "#u3_t", &item->default_u3 );
                            break;
                        case TYPEOF_f2_t:
                        case TYPEOF_f3_t:
                            idx = st_s_parse_fa( text, idx, text->size, "#f3_t", &item->default_f3 );
                            break;

                        case TYPEOF_bl_t:
                        {
                            bl_t flag = false;
                            idx = st_s_parse_fa( text, idx, text->size, "#bl_t", &flag );
                            item->default_u3 = flag;
                        }
                        break;

                        default: ERR( "Cannot assign default value to type '%s'", ifnameof( item->type ) );
                    }
                }
                else
                {
                    st_s* context = st_s_show_line_context( text, idx );
                    ERR( "\n%s\nAssignment of default value only for static capsulation.", context->sc );
                }
            }
        }

        idx = st_s_parse_fa( text, idx, text->size, " ; " );

        bcore_flect_body_s_push( o, item );
        bcore_life_s_discard( life );
    }
    idx = st_s_parse_fa( text, idx, text->size, " } " );

    if ( p_idx != NULL ) *p_idx = idx;
    bcore_life_s_discard( life );
    return o;
}

bcore_signature_s* bcore_flect_body_s_push_to_signature( const bcore_flect_body_s* o, bcore_signature_s* sig )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_flect_item_s_push_to_signature( &o->data[ i ], sig );
    return sig;
}

s2_t bcore_flect_body_s_cmp( const bcore_flect_body_s* o1, const bcore_flect_body_s* o2 )
{
    if( o1->size != o2->size ) return ( o1->size < o2->size ) ? 1 : -1;
    for( sz_t i = 0; i < o1->size; i++ )
    {
        s2_t c = bcore_flect_item_s_cmp( &o1->data[ i ], &o2->data[ i ] );
        if( c != 0 ) return c;
    }
    return 0;
}

static bcore_flect_self_s* flect_body_s_create_self( void )
{
    sc_t def =
    "bcore_flect_body_s ="
    "{"
        "bcore_flect_item_s [] arr;"
        "bl_t complete;"
    "}";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_flect_body_s ) );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_body_s_init,    "bcore_fp_init",    "init"    );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_body_s_down,    "bcore_fp_down",    "down"    );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_body_s_copy,    "bcore_fp_copy",    "copy"    );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_body_s_create,  "bcore_fp_create",  "create"  );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_body_s_clone,   "bcore_fp_clone",   "clone"   );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_body_s_discard, "bcore_fp_discard", "discard" );
    return self;
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

bcore_flect_self_s* bcore_flect_self_s_clone( const bcore_flect_self_s* o )
{
    bcore_flect_self_s* dst = bcore_flect_self_s_create();
    bcore_flect_self_s_copy( dst, o );
    return dst;
}

void bcore_flect_self_s_discard( bcore_flect_self_s* o )
{
    if( !o ) return;
    bcore_release_obj( ( fp_t )bcore_flect_self_s_down, o );
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

void bcore_flect_self_s_push_func( bcore_flect_self_s* o, sc_t fname, fp_t func, sc_t type, sc_t name )
{
    bcore_flect_self_s_push_d( o, bcore_flect_item_s_create_func( fname, func, type, name ) );
}

void bcore_flect_self_s_push_ns_func( bcore_flect_self_s* o, fp_t func, sc_t type, sc_t name )
{
    st_s* fname = st_s_create_fa( "#<sc_t>_#<sc_t>", nameof( o->type ), name );
    bcore_flect_self_s_push_d( o, bcore_flect_item_s_create_func( fname->sc, func, type, name ) );
    st_s_discard( fname );
}

void bcore_flect_self_s_push_fp_set( bcore_flect_self_s* o, bcore_fp_set func, sc_t name )
{
    bcore_flect_self_s_push_ns_func( o, (fp_t )func, "bcore_fp_set", name );
}

void bcore_flect_self_s_push_fp_get( bcore_flect_self_s* o, bcore_fp_get func, sc_t name )
{
    bcore_flect_self_s_push_ns_func( o, (fp_t )func, "bcore_fp_get", name );
}

void bcore_flect_self_s_init_plain( bcore_flect_self_s* o, tp_t type, sz_t size )
{
    bcore_flect_self_s_init( o );
    o->type  = type;
    o->size  = size;
    if( o->body ) bcore_flect_body_s_discard( o->body );
    o->body = NULL;
}

st_s* bcore_flect_self_s_show( const bcore_flect_self_s* o )
{
    st_s* s = st_s_create();
    st_s_pushf( s, "bcore_flect_self_s" );
    st_s_pushf( s, "\n{" );
    st_s_pushf( s, "\n    type:  %x '%s'", o->type, ifnameof( o->type ) );
    st_s_pushf( s, "\n    size:  %zu", o->size );
    st_s_pushf( s, "\n    body: " );
    if( o->body ) st_s_push_st_d( s, st_s_replace_char_sc( bcore_flect_body_s_show( o->body ), '\n', "\n    " ) );
    st_s_pushf( s, "\n}" );
    return s;
}

void bcore_flect_self_s_check_consistency( const bcore_flect_self_s* o )
{
    if( o->trait )
    {
        st_s* log = st_s_create();
        if( !bcore_trait_supported( o->trait, o, log ) )
        {
            ERR( "Reflection of '%s' does not support trait '%s': %s\n", ifnameof( o->type ), ifnameof( o->trait ), log->sc );
        }
        st_s_discard( log );
    }
}

bcore_flect_self_s* bcore_flect_self_s_create_plain( tp_t type, sz_t size )
{
    bcore_flect_self_s* o = bcore_flect_self_s_create();
    bcore_flect_self_s_init_plain( o, type, size );
    return o;
}

bcore_flect_self_s* bcore_flect_self_s_create_static_array( tp_t item_type )
{
    bcore_flect_self_s* o = bcore_flect_self_s_create();
    bcore_flect_item_s* item = bcore_flect_item_s_create();
    item->caps = BCORE_CAPS_STATIC_ARRAY;
    item->type = item_type;
    bcore_flect_self_s_push( o, item );
    bcore_flect_item_s_discard( item );

    bcore_signature_s* sig = bcore_signature_s_create();
    bcore_flect_body_s_push_to_signature( o->body, sig );
    o->type = bcore_signature_s_get_hash( sig );
    bcore_signature_s_discard( sig );

    return o;
}

bcore_flect_self_s* bcore_flect_self_s_create_static_link_array( tp_t item_type )
{
    bcore_flect_self_s* o = bcore_flect_self_s_create();
    bcore_flect_item_s* item = bcore_flect_item_s_create();
    item->caps = BCORE_CAPS_STATIC_LINK_ARRAY;
    item->type = item_type;
    bcore_flect_self_s_push( o, item );
    bcore_flect_item_s_discard( item );

    bcore_signature_s* sig = bcore_signature_s_create();
    bcore_flect_body_s_push_to_signature( o->body, sig );
    o->type = bcore_signature_s_get_hash( sig );
    bcore_signature_s_discard( sig );

    return o;
}

bcore_flect_self_s* bcore_flect_self_s_build_parse( const st_s* text, sz_t* p_idx, sz_t size_of )
{
    bcore_life_s* life = bcore_life_s_create();
    bcore_flect_self_s* o = bcore_flect_self_s_create();

    sz_t idx = ( p_idx != NULL ) ? *p_idx : 0;

    st_s* type_name = st_s_create_l( life );
    idx = st_s_parse_fa( text, idx, text->size, " #name ", type_name );
    o->type = ( type_name->size > 0 ) ? bcore_name_enroll( type_name->sc ) : 0;
    if( o->type ) idx = st_s_parse_fa( text, idx, text->size, "= " );

    st_s* assigned_name = st_s_create_l( life );
    idx = st_s_parse_fa( text, idx, text->size, " #name ", assigned_name );
    tp_t assigned_type = ( assigned_name->size > 0 ) ? bcore_name_enroll( assigned_name->sc ) : 0;

    if( text->sc[ idx ] == '{' )
    {
        o->trait = assigned_type;
        o->body = bcore_flect_body_s_build_parse( text, &idx );
        if( !o->type )
        {
            bcore_signature_s* sig = bcore_life_s_push_aware( life, bcore_signature_s_create() );
            bcore_flect_body_s_push_to_signature( o->body, sig );

            // we do not need to enroll the signature because anonymous reflections are tested directly for collisions
            o->type = bcore_signature_s_get_hash( sig );
        }
    }
    else
    {
        o->trait = 0;
        if( !o->type )
        {
            st_s* context = bcore_life_s_push_aware( life, st_s_show_line_context( text, idx ) );
            ERR( "\n%s\nAnonymous types need a body.", context->sc );
        }
        const bcore_flect_self_s* self_l = bcore_flect_try_self( assigned_type );
        if( !self_l )
        {
            st_s* context = st_s_show_line_context( text, idx );
            ERR( "\n%s\nType %s not defined", context->sc, assigned_name->sc );
        }
        o->body = bcore_flect_body_s_clone( self_l->body );
        o->size = self_l->size;
    }

    if( p_idx != NULL ) *p_idx = idx;
    o->size = size_of;
    bcore_life_s_discard( life );
    return o;
}

bcore_flect_self_s* bcore_flect_self_s_build_parse_sc( sc_t text, sz_t size_of )
{
    st_s* string = st_s_create_sc( text );
    bcore_flect_self_s* ret = bcore_flect_self_s_build_parse( string, NULL, size_of );
    st_s_discard( string );
    return ret;
}

bcore_signature_s* bcore_flect_self_s_push_to_signature( const bcore_flect_self_s* o, bcore_signature_s* sig )
{
    bcore_signature_s_push( sig, o->type );
    bcore_signature_s_push( sig, o->trait );
    if( o->body ) bcore_flect_body_s_push_to_signature( o->body, sig );
    return sig;
}

s2_t bcore_flect_self_s_cmp( const bcore_flect_self_s* o1, const bcore_flect_self_s* o2 )
{
    if( o1->type != o2->type ) return ( o1->type < o2->type ) ? 1 : -1;
    if( o1->body != o2->body )
    {
        if( !o1->body ) return  1;
        if( !o2->body ) return -1;
        return bcore_flect_body_s_cmp( o1->body, o2->body );
    }
    return 0;
}

fp_t bcore_flect_self_s_try_external_fp( const bcore_flect_self_s* o, tp_t type, tp_t name )
{
    if( !o->body ) return NULL;
    const bcore_flect_body_s* body = o->body;
    for( sz_t i = 0; i < body->size; i++ )
    {
        const bcore_flect_item_s* item = &body->data[ i ];
        if( ( item->caps == BCORE_CAPS_EXTERNAL_FUNC ) &&
            ( type == 0 || item->type == type        ) &&
            ( name == 0 || item->name == name        ) )
        {
            if( item->default_tp )
            {
                fp_t fp = bcore_function_get( item->default_tp );
                if( !fp ) ERR( "Function '%s' (%"PRItp_t") not registered in function manager.", ifnameof(item->default_tp ), item->default_tp );
                return fp;
            }
            else
            {
                ERR( "Object '%s': Function '%s' declared but not assigned.", ifnameof( o->type ), ifnameof( name ) );
            }
        }
    }
    return NULL;
}

fp_t bcore_flect_self_s_get_external_fp( const bcore_flect_self_s* o, tp_t type, tp_t name )
{
    fp_t fp = bcore_flect_self_s_try_external_fp( o, type, name );
    if( !fp ) ERR( "Function '%s' of name '%s' not found in reflection '%s'.", ifnameof( type ), ifnameof( name ), ifnameof(o->type) );
    return fp;
}

vd_t bcore_flect_self_s_get_static( const bcore_flect_self_s* o, tp_t type, tp_t name )
{
    bcore_fp_create fp = ( bcore_fp_create )bcore_flect_self_s_try_external_fp( o, type, name );
    if( !fp ) ERR( "Static creator '%s' of name '%s' not found in reflection '%s'.", ifnameof( type ), ifnameof( name ), ifnameof(o->type) );
    return fp();
}

vd_t bcore_flect_self_s_try_static( const bcore_flect_self_s* o, tp_t type, tp_t name )
{
    bcore_fp_create fp = ( bcore_fp_create )bcore_flect_self_s_try_external_fp( o, type, name );
    return fp ? fp() : NULL;
}

bool bcore_flect_self_s_is_aware( const bcore_flect_self_s* o )
{
    if( !o->body ) return false;
    const bcore_flect_body_s* body = o->body;
    if( body->size == 0 ) return false;
    return body->data[ 0 ].type == TYPEOF_aware_t;
}

static bcore_flect_self_s* flect_self_s_create_self( void )
{
    sc_t def =
    "bcore_flect_self_s ="
    "{"
        "aware_t _;"
        "tp_t type;"
        "tp_t trait;"
        "sz_t size;"
        "bcore_flect_body_s* body;"
    "}";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_flect_self_s ) );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_self_s_init,         "bcore_fp_init",         "init"         );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_self_s_down,         "bcore_fp_down",         "down"         );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_self_s_copy,         "bcore_fp_copy",         "copy"         );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_self_s_create,       "bcore_fp_create",       "create"       );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_self_s_clone,        "bcore_fp_clone",        "clone"        );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )bcore_flect_self_s_discard,      "bcore_fp_discard",      "discard"      );
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
    bcore_release_obj( ( fp_t )creator_map_s_down, o );
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
    bcore_release_obj( ( fp_t )self_map_s_down, o );
}

/**********************************************************************************************************************/
/// Global reflection manager (thread safe)

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

static void flect_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, bcore_create_flect_maps );
}

static void flect_close()
{
    bcore_discard_flect_maps();
}

bl_t bcore_flect_exists( tp_t type )
{
    assert( self_map_s_g != NULL );
    bl_t exists = false;

    bcore_mutex_lock( &self_map_s_g->mutex );
    exists = bcore_hmap_u2vd_s_exists( self_map_s_g->hmap, type );
    bcore_mutex_unlock( &self_map_s_g->mutex );
    if( exists ) return true;

    assert( creator_map_s_g != NULL );
    bcore_mutex_lock( &creator_map_s_g->mutex );
    exists = bcore_hmap_u2vd_s_exists( creator_map_s_g->hmap, type );
    bcore_mutex_unlock( &creator_map_s_g->mutex );
    return exists;
}

const bcore_flect_self_s* bcore_flect_try_self( tp_t type )
{
    // try reflection registry
    assert( self_map_s_g != NULL );
    bcore_mutex_lock( &self_map_s_g->mutex );
    vd_t* p_val = bcore_hmap_u2vd_s_get( self_map_s_g->hmap, type );
    bcore_mutex_unlock( &self_map_s_g->mutex );
    if( p_val ) return *p_val;

    // try constructing self by reflection creator
    bcore_flect_self_s* self = NULL;
    assert( creator_map_s_g != NULL );

    bcore_mutex_lock( &creator_map_s_g->mutex );
    fp_t* p_func = bcore_hmap_u2vd_s_getf( creator_map_s_g->hmap, type );
    bcore_mutex_unlock( &creator_map_s_g->mutex );


    if( p_func )
    {
        self = ( ( bcore_flect_create_self_fp )*p_func )();
        if( self->type != type )
        {
            ERR( "Type of created reflection '%s' differs from requested type '%s'.\n"
                 "Requested type was probably mapped to the wrong self-creator during registration.\n",
                 ifnameof( self->type ), ifnameof( type ) );
        }
        bcore_flect_self_s_check_consistency( self );

        bcore_mutex_lock( &self_map_s_g->mutex );

        if( bcore_hmap_u2vd_s_exists( self_map_s_g->hmap, type ) )
        {
            // If this reflection has meanwhile been registered by another thread,
            // kill current self and retrieve registered version
            bcore_flect_self_s_discard( self );
            self = *bcore_hmap_u2vd_s_get( self_map_s_g->hmap, type );
        }
        else
        {
            // else: register current self
            bcore_hmap_u2vd_s_set( self_map_s_g->hmap, type, self, true );
        }
        bcore_mutex_unlock( &self_map_s_g->mutex );
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

tp_t bcore_flect_define_self_d( bcore_flect_self_s* self )
{
    assert( self_map_s_g != NULL );
    bcore_flect_self_s_check_consistency( self );
    tp_t type = self->type;
    bcore_mutex_lock( &self_map_s_g->mutex );
    if( bcore_hmap_u2vd_s_exists( self_map_s_g->hmap, type ) ) ERR( "'%s' (%"PRItp_t") is already defined", ifnameof( type ), type );
    bcore_hmap_u2vd_s_set( self_map_s_g->hmap, type, self, true );
    bcore_mutex_unlock( &self_map_s_g->mutex );
    return self->type;
}

static tp_t flect_define_self_rentrant_d( bcore_flect_self_s* self )
{
    assert( self_map_s_g != NULL );
    bcore_flect_self_s_check_consistency( self );
    tp_t type = self->type;
    bcore_mutex_lock( &self_map_s_g->mutex );
    if( !bcore_hmap_u2vd_s_exists( self_map_s_g->hmap, type ) ) bcore_hmap_u2vd_s_set( self_map_s_g->hmap, type, self, true );
    bcore_mutex_unlock( &self_map_s_g->mutex );
    return self->type;
}

tp_t bcore_flect_define_self_c( const bcore_flect_self_s* self )
{
    return bcore_flect_define_self_d( bcore_flect_self_s_clone( self ) );
}

tp_t bcore_flect_define_parse( const st_s* string, sz_t* idx )
{
    return bcore_flect_define_self_d( bcore_flect_self_s_build_parse( string, idx, 0 ) );
}

tp_t bcore_flect_define_parse_sc( sc_t sc )
{
    st_s* string = st_s_create_sc( sc );
    sz_t idx = 0;
    tp_t type = bcore_flect_define_parse( string, &idx );
    st_s_discard( string );
    return type;
}

tp_t bcore_flect_define_parse_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s* string = st_s_createvf( format, args );
    va_end( args );
    sz_t idx = 0;
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse( string, &idx, 0 );
    tp_t type = bcore_flect_define_self_d( self );
    st_s_discard( string );
    return type;
}

tp_t bcore_flect_type_self_d( bcore_flect_self_s* self )
{
    const bcore_flect_self_s* self_l = bcore_flect_try_self( self->type );
    if( self_l )
    {
        if( bcore_flect_self_s_cmp( self_l, self ) != 0 )
        {
            ERR( "Attempt to register a different reflection under type = '%s' (%"PRItp_t")\nRegistered:\n%s\nNew:\n%s\n",
                 ifnameof( self->type ), self->type,
                 bcore_flect_self_s_show( self_l ),
                 bcore_flect_self_s_show( self ) );
        }
        bcore_flect_self_s_discard( self );
        return self_l->type;
    }
    else
    {
        flect_define_self_rentrant_d( self );
        return self->type;
    }
}

tp_t bcore_flect_type_self_c( const bcore_flect_self_s* self )
{
    return bcore_flect_type_self_d( bcore_flect_self_s_clone( self ) );
}

tp_t bcore_flect_type_parse( const st_s* string, sz_t* idx )
{
    return bcore_flect_type_self_d( bcore_flect_self_s_build_parse( string, idx, 0 ) );
}

tp_t bcore_flect_type_parse_sc( sc_t sc )
{
    st_s* string = st_s_create_sc( sc );
    tp_t ret = bcore_flect_type_parse( string, 0 );
    st_s_discard( string );
    return ret;
}

tp_t bcore_flect_type_parse_fa( sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    st_s* string = st_s_createvf( format, args );
    va_end( args );
    tp_t type = bcore_flect_type_parse( string, 0 );
    st_s_discard( string );
    return type;
}

void bcore_flect_define_creator( tp_t type, bcore_flect_create_self_fp creator )
{
    assert( creator_map_s_g != NULL );
    bcore_mutex_lock( &creator_map_s_g->mutex );
    if( bcore_hmap_u2vd_s_exists( creator_map_s_g->hmap, type ) ) ERR( "'%s' (%"PRItp_t") is already defined", ifnameof( type ), type );
    bcore_hmap_u2vd_s_setf( creator_map_s_g->hmap, type, ( fp_t )creator );
    bcore_mutex_unlock( &creator_map_s_g->mutex );
}

/**********************************************************************************************************************/

static void flect_define_basics()
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
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "bl_t" ), sizeof( bl_t ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_create_plain( bcore_name_enroll( "void" ), 0              ) );

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
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( "bcore_static_link_s       = { vd_t    link; }", sizeof( bcore_static_link_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( "bcore_typed_link_s        = { typed * link; }", sizeof( bcore_typed_link_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( "bcore_aware_link_s        = { aware * link; }", sizeof( bcore_aware_link_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( "bcore_static_array_s      = { vd_t  data; sz_t size; sz_t space; }", sizeof( bcore_static_array_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( "bcore_typed_array_s       = { typed  []; }",    sizeof( bcore_typed_array_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( "bcore_static_link_array_s = { vd_t* data; sz_t size; sz_t space; }", sizeof( bcore_static_link_array_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( "bcore_typed_link_array_s  = { typed* []; }",    sizeof( bcore_typed_link_array_s ) ) );
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( "bcore_aware_link_array_s  = { aware* []; }",    sizeof( bcore_aware_link_array_s ) ) );

    // specific objects
    bcore_flect_define_creator( typeof( "bcore_flect_item_s" ), flect_item_s_create_self );
    bcore_flect_define_creator( typeof( "bcore_flect_body_s" ), flect_body_s_create_self );
    bcore_flect_define_creator( typeof( "bcore_flect_self_s" ), flect_self_s_create_self );
}

/**********************************************************************************************************************/

static st_s* flect_selftest( void )
{
    {
        bcore_flect_define_parse_sc(" teabag =    { u3_t leaves; s1_t flavor; s0_t color;  }" );
        bcore_flect_define_parse_sc(" container = { u3_t elements; teabag*[] bags; u1_t flags; }" );
        bcore_flect_define_parse_sc(" delivery =  { aware_t _; u3_t tag; container* cargo; aware*[] attachments; }" );
        bcore_flect_define_parse_sc(" another = delivery" );
    }

    st_s* s = st_s_create();
    st_s_push_st_d( s, bcore_flect_self_s_show( bcore_flect_get_self( typeof( "st_s" ) ) ) );

    {
        tp_t t_teabag    = bcore_flect_type_parse_sc( "{ u3_t leaves; s1_t flavor; s0_t color; }" );
        tp_t t_container = bcore_flect_type_parse_fa( "{ u3_t elements = 343; %"PRItp_t" bag; u1_t flags; }", t_teabag );

        st_s_push_st_d( s, bcore_flect_self_s_show( bcore_flect_get_self( t_container ) ) );
        st_s_pushf( s, "\n" );
        st_s_pushf( s, "anonymous teabag type %u\n", t_teabag );
    }

    {
        tp_t anonymous = bcore_flect_type_parse_sc( "{ u3_t leaves; s1_t flavor; s0_t color; }" );
        st_s_pushf( s, "anonymous teabag type %u (reentered)\n", anonymous );
    }

    return s;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_flect_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_flect" ) ) return NULL;

    if( signal == typeof( "init0" ) )
    {
        flect_open();
        flect_define_basics();
    }
    else if( signal == typeof( "init1" ) )
    {
    }
    else if( signal == typeof( "down0" ) )
    {
        if( object && ( *( bl_t* )object ) )
        {
            sz_t space = bcore_tbman_granted_space();
            flect_close();
            bcore_msg( "  reflection mananger . % 6zu\n", space - bcore_tbman_granted_space() );
        }
        else
        {
            flect_close();
        }
    }
    else if( signal == typeof( "selftest" ) )
    {
        return flect_selftest();
    }

    return NULL;
}

