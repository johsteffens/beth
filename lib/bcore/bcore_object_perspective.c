/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_object_perspective.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT( bcore_object_item_s )
DEFINE_FUNCTION_DOWN_FLAT( bcore_object_item_s )
DEFINE_FUNCTION_COPY_FLAT( bcore_object_item_s )
DEFINE_FUNCTION_MOVE(      bcore_object_item_s )
DEFINE_FUNCTION_CREATE(    bcore_object_item_s )
DEFINE_FUNCTION_DISCARD(   bcore_object_item_s )
DEFINE_FUNCTION_CLONE(     bcore_object_item_s )

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT( bcore_object_body_s )

void bcore_object_body_s_down( bcore_object_body_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_object_item_s_down( &o->data[ i ] );
    o->data = bcore_un_alloc( sizeof( bcore_object_item_s ), o->data, o->space, 0, &o->space );
    o->size = 0;
}

void bcore_object_body_s_copy( bcore_object_body_s* o, const bcore_object_body_s* src )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_object_item_s_down( &o->data[ i ] );
    o->data = bcore_un_alloc( sizeof( bcore_object_item_s ), o->data, o->space, 0,         &o->space );
    o->data = bcore_un_alloc( sizeof( bcore_object_item_s ), o->data, o->space, src->size, &o->space );
    for( sz_t i = 0; i < src->size; i++ ) bcore_object_item_s_copy( &o->data[ i ], &src->data[ i ] );
    o->size = src->size;
}

DEFINE_FUNCTION_CREATE(    bcore_object_body_s )
DEFINE_FUNCTION_DISCARD(   bcore_object_body_s )
DEFINE_FUNCTION_CLONE(     bcore_object_body_s )

bcore_object_item_s* bcore_object_body_s_push( bcore_object_body_s* o )
{
    if( o->size == o->space )
    {
        sz_t old_space = o->space;
        bcore_object_item_s* old_data = o->data;
        o->data = bcore_u_alloc( sizeof( bcore_object_item_s ), NULL, o->space > 0 ? o->space * 2 : 1, &o->space );
        for( sz_t i = 0; i < o->size; i++  ) bcore_object_item_s_move( &o->data[ i ], &old_data[ i ] );
        bcore_un_alloc( sizeof( bcore_object_item_s ), old_data, old_space, 0, NULL );
    }
    bcore_object_item_s* item = &o->data[ o->size ];
    o->size++;
    bcore_object_item_s_init( item );
    return item;
}

void bcore_object_body_s_pop( bcore_object_body_s* o )
{
    if( o->size == 0 ) return;
    o->size--;
    bcore_object_item_s_down( &o->data[ o->size ] );
}

/**********************************************************************************************************************/
// bcore_object_s

void bcore_object_s_down( bcore_object_s* o );

void bcore_object_s_init( bcore_object_s* o )
{
    bcore_memzero( o, sizeof( bcore_object_s ) );
    bcore_perspective_s_init( &o->_, bcore_object_s_down );
}

void bcore_object_s_down( bcore_object_s* o )
{
    bcore_object_body_s_discard( o->body );
}

bcore_object_s* bcore_object_s_create()
{
    bcore_object_s* o = bcore_alloc( NULL, sizeof( bcore_object_s ) );
    bcore_object_s_init( o );
    return o;
}

void bcore_object_s_discard( bcore_object_s* o )
{
    if( !o ) return;
    bcore_object_s_down( o );
    bcore_free( o );
}

bcore_object_item_s* bcore_object_s_push( bcore_object_s* o )
{
    if( !o->body ) o->body = bcore_object_body_s_create();
    return bcore_object_body_s_push( o->body );
}

void bcore_object_s_verify_type( const bcore_object_s* o, const void* obj, const char* context )
{
    if( !o->aware ) return;
    if( o->_.o_type != *( aware_t* )obj )
    {
        const char* name = bcore_name_try_name( *( aware_t* )obj );
        if( name )
        {
            ERR( "%s on '%s'; expected '%s'", context, ifnameof( *( aware_t* )obj ), ifnameof( o->_.o_type ) );
        }
        else
        {
            ERR( "%s on undefined object; expected '%s'", context, ifnameof( o->_.o_type ) );
        }
    }
}

/**********************************************************************************************************************/

void bcore_object_s__init_o( const bcore_object_s* o, void* obj )
{
#ifndef NDEBUG
    if( !obj ) ERR( "obj == NULL" );
#endif // NDEBUG
    o->init_o( obj );
}

void bcore_object_s__init_flat( const bcore_object_s* o, void* obj )
{
#ifndef NDEBUG
    if( !obj ) ERR( "obj == NULL" );
#endif
    bcore_memzero( obj, o->size );
}

void bcore_object_s__init_generic( const bcore_object_s* o, void* obj )
{
#ifndef NDEBUG
    if( !obj ) ERR( "obj == NULL" );
#endif
    bcore_memzero( obj, o->size );
    sz_t first = 0;
    if( o->aware )
    {
        *( aware_t* )obj = o->_.o_type;
        first = 1;
    }
    for( sz_t i = first; i < o->body->size; i++ )
    {
        const bcore_object_item_s* item = &o->body->data[ i ];
        const bcore_flect_item_s* flect_item = item->flect_item;
        void* item_obj = ( u0_t* )obj + item->flect_item->offset;

        switch( flect_item->caps )
        {
            case BCORE_CAPS_STATIC:
            {
                const bcore_object_s* perspective = item->perspective;
                if( !perspective->init_flat ) perspective->init( perspective, item_obj );
            }
            break;

            case BCORE_CAPS_STATIC_LINK:
            case BCORE_CAPS_TYPED_LINK:
            case BCORE_CAPS_AWARE_LINK:
            case BCORE_CAPS_STATIC_ARRAY:
            case BCORE_CAPS_TYPED_ARRAY:
            case BCORE_CAPS_STATIC_LINK_ARRAY:
            case BCORE_CAPS_TYPED_LINK_ARRAY:
            case BCORE_CAPS_AWARE_LINK_ARRAY:
            break;

            default:
            {
                ERR( "Unhandled capsulation %u", flect_item->caps );
            }
        }
    }
}

void bcore_object_s__down_o( const bcore_object_s* o, void* obj )
{
#ifndef NDEBUG
    if( !obj ) ERR( "obj == NULL" );
    if( o->aware ) bcore_object_s_verify_type( o, obj, __func__ );
#endif // NDEBUG
    o->down_o( obj );
}

void bcore_object_s__down_flat( const bcore_object_s* o, void* obj )
{
#ifndef NDEBUG
    if( !obj ) ERR( "obj == NULL" );
    if( o->aware ) bcore_object_s_verify_type( o, obj, __func__ );
#endif // NDEBUG
    /* nothing to do */
}

void  bcore_object_s__down_generic( const bcore_object_s* o, void* obj )
{
#ifndef NDEBUG
    if( !obj ) ERR( "obj == NULL" );
    if( o->aware ) bcore_object_s_verify_type( o, obj, __func__ );
#endif // NDEBUG
    for( sz_t i = 0; i < o->body->size; i++ )
    {
        const bcore_object_item_s* item = &o->body->data[ i ];
        const bcore_flect_item_s* flect_item = item->flect_item;
        void* item_obj = ( u0_t* )obj + item->flect_item->offset;

        switch( flect_item->caps )
        {
            case BCORE_CAPS_STATIC:
            {
                const bcore_object_s* perspective = item->perspective;
                if( !perspective->down_flat ) perspective->down( perspective, item_obj );
            }
            break;

            case BCORE_CAPS_STATIC_LINK:
            {
                bcore_flect_caps_static_link_s* s = item_obj;
                if( s->link )
                {
                    const bcore_object_s* perspective = item->perspective;
                    perspective->discard( perspective, s->link );
                }
            }
            break;

            case BCORE_CAPS_TYPED_LINK:
            {
                bcore_flect_caps_typed_link_s* s = item_obj;
                if( s->link )
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( s->type );
                    perspective->discard( perspective, s->link );
                }
            }
            break;

            case BCORE_CAPS_AWARE_LINK:
            {
                bcore_flect_caps_aware_link_s* s = item_obj;
                if( s->link )
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( *( aware_t* )s->link );
                    perspective->discard( perspective, s->link );
                }
            }
            break;

            case BCORE_CAPS_STATIC_ARRAY:
            {
                bcore_flect_caps_static_array_s* s = item_obj;
                if( s->data )
                {
                    const bcore_object_s* perspective = item->perspective;
                    if( !perspective->down_flat )
                    {
                        for( sz_t i = 0; i < s->size; i++ )
                        {
                            vd_t obj = ( u0_t* )s->data + perspective->size * i;
                            perspective->down( perspective, obj );
                        }
                    }
                    s->data = bcore_un_alloc( perspective->size, s->data, s->space, 0, &s->space );
                }
            }
            break;

            case BCORE_CAPS_TYPED_ARRAY:
            {
                bcore_flect_caps_typed_array_s* s = item_obj;
                if( s->data )
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( s->type );
                    if( !perspective->down_flat )
                    {
                        for( sz_t i = 0; i < s->size; i++ )
                        {
                            vd_t obj = ( u0_t* )s->data + perspective->size * i;
                            perspective->down( perspective, obj );
                        }
                    }
                    s->data = bcore_un_alloc( perspective->size, s->data, s->space, 0, &s->space );
                }
            }
            break;

            case BCORE_CAPS_STATIC_LINK_ARRAY:
            {
                bcore_flect_caps_static_link_array_s* s = item_obj;
                if( s->data )
                {
                    const bcore_object_s* perspective = item->perspective;
                    for( sz_t i = 0; i < s->size; i++ ) perspective->discard( perspective, s->data[ i ] );
                    s->data = bcore_un_alloc( sizeof( vd_t ), s->data, s->space, 0, &s->space );
                }
            }
            break;

            case BCORE_CAPS_TYPED_LINK_ARRAY:
            {
                bcore_flect_caps_typed_link_array_s* s = item_obj;
                if( s->data )
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( s->type );
                    for( sz_t i = 0; i < s->size; i++ ) perspective->discard( perspective, s->data[ i ] );
                    s->data = bcore_un_alloc( sizeof( vd_t ), s->data, s->space, 0, &s->space );
                }
            }
            break;

            case BCORE_CAPS_AWARE_LINK_ARRAY:
            {
                bcore_flect_caps_aware_link_array_s* s = item_obj;
                if( s->data )
                {
                    for( sz_t i = 0; i < s->size; i++ ) bcore_object_aware_discard( s->data[ i ] );
                    s->data = bcore_un_alloc( sizeof( vd_t ), s->data, s->space, 0, &s->space );
                }
            }
            break;

            default:
            {
                ERR( "Unhandled capsulation %u", flect_item->caps );
            }
        }
    }
}

void bcore_object_s__copy_o( const bcore_object_s* o, void* dst, const void* src )
{
    if( dst == src ) return;
#ifndef NDEBUG
    if( !dst ) ERR( "dst == NULL" );
    if( !src ) ERR( "src == NULL" );
    if( o->aware )
    {
        bcore_object_s_verify_type( o, dst, __func__ );
        bcore_object_s_verify_type( o, src, __func__ );
    }
#endif // NDEBUG
    o->copy_o( dst, src );
}

void bcore_object_s__copy_flat( const bcore_object_s* o, void* dst, const void* src )
{
    if( dst == src ) return;
#ifndef NDEBUG
    if( !dst ) ERR( "dst == NULL" );
    if( !src ) ERR( "src == NULL" );
    if( o->aware )
    {
        bcore_object_s_verify_type( o, dst, __func__ );
        bcore_object_s_verify_type( o, src, __func__ );
    }
#endif // NDEBUG
    bcore_memcpy( dst, src, o->size );
}

void  bcore_object_s__copy_generic( const bcore_object_s* o, void* dst, const void* src )
{
    if( dst == src ) return;
#ifndef NDEBUG
    if( !dst ) ERR( "dst == NULL" );
    if( !src ) ERR( "src == NULL" );
    if( o->aware )
    {
        bcore_object_s_verify_type( o, dst, __func__ );
        bcore_object_s_verify_type( o, src, __func__ );
    }
#endif // NDEBUG
    for( sz_t i = 0; i < o->body->size; i++ )
    {
        const bcore_object_item_s* item = &o->body->data[ i ];
        const bcore_flect_item_s* flect_item = item->flect_item;
        void* dst_obj = ( u0_t* )dst + item->flect_item->offset;
        void* src_obj = ( u0_t* )src + item->flect_item->offset;

        switch( flect_item->caps )
        {
            case BCORE_CAPS_STATIC:
            {
                const bcore_object_s* perspective = item->perspective;
                perspective->copy( perspective, dst_obj, src_obj );
            }
            break;

            case BCORE_CAPS_STATIC_LINK:
            {
                bcore_flect_caps_static_link_s* dst = dst_obj;
                bcore_flect_caps_static_link_s* src = src_obj;
                const bcore_object_s* perspective = item->perspective;
                perspective->discard( perspective, dst->link );
                dst->link = perspective->clone( perspective, src->link );
            }
            break;

            case BCORE_CAPS_TYPED_LINK:
            {
                bcore_flect_caps_typed_link_s* dst = dst_obj;
                bcore_flect_caps_typed_link_s* src = src_obj;
                if( dst->link )
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( dst->type );
                    perspective->discard( perspective, dst->link );
                }
                if( src->link )
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( src->type );
                    dst->link = perspective->clone( perspective, src->link );
                }
                dst->type = src->type;
            }
            break;

            case BCORE_CAPS_AWARE_LINK:
            {
                bcore_flect_caps_aware_link_s* dst = dst_obj;
                bcore_flect_caps_aware_link_s* src = src_obj;
                if( dst->link )
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( *( aware_t* )dst->link );
                    perspective->discard( perspective, dst->link );
                }
                if( src->link )
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( *( aware_t* )src->link );
                    dst->link = perspective->clone( perspective, src->link );
                }
            }
            break;

            case BCORE_CAPS_STATIC_ARRAY:
            {
                bcore_flect_caps_static_array_s* dst = dst_obj;
                bcore_flect_caps_static_array_s* src = src_obj;
                const bcore_object_s* perspective = item->perspective;
                if( perspective->copy_flat )
                {
                    if( src->size > dst->space )
                    {
                        dst->data = bcore_un_alloc( perspective->size, dst->data, dst->space, 0,         &dst->space );
                        dst->data = bcore_un_alloc( perspective->size, dst->data, dst->space, src->size, &dst->space );
                    }
                    bcore_memcpy( dst->data, src->data, perspective->size * src->size );
                    dst->size = src->size;
                }
                else
                {
                    for( sz_t i = 0; i < dst->size; i++ )
                    {
                        perspective->down( perspective, ( u0_t* )dst->data + i * perspective->size );
                    }
                    if( src->size > dst->space )
                    {
                        dst->data = bcore_un_alloc( perspective->size, dst->data, dst->space, 0,         &dst->space );
                        dst->data = bcore_un_alloc( perspective->size, dst->data, dst->space, src->size, &dst->space );
                    }
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        vd_t dst_obj = ( u0_t* )dst->data + i * perspective->size;
                        vd_t src_obj = ( u0_t* )src->data + i * perspective->size;
                        perspective->init( perspective, dst_obj );
                        perspective->copy( perspective, dst_obj, src_obj );
                    }
                    dst->size = src->size;
                }
            }
            break;

            case BCORE_CAPS_TYPED_ARRAY:
            {
                bcore_flect_caps_typed_array_s* dst = dst_obj;
                bcore_flect_caps_typed_array_s* src = src_obj;

                if( dst->space > 0 ) // deplete dst
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( dst->type );
                    if( !perspective->down_flat )
                    {
                        for( sz_t i = 0; i < dst->size; i++ )
                        {
                            perspective->down( perspective, ( u0_t* )dst->data + i * perspective->size );
                        }
                    }
                    dst->data = bcore_un_alloc( perspective->size, dst->data, dst->space, 0, &dst->space );
                    dst->size = 0;
                    dst->type = 0;
                }
                if( src->space > 0 ) // fill dst with new data
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( src->type );
                    dst->data = bcore_un_alloc( perspective->size, dst->data, dst->space, src->size, &dst->space );
                    if( perspective->copy_flat )
                    {
                        bcore_memcpy( dst->data, src->data, perspective->size * src->size );
                    }
                    else
                    {
                        for( sz_t i = 0; i < src->size; i++ )
                        {
                            vd_t dst_obj = ( u0_t* )dst->data + i * perspective->size;
                            vc_t src_obj = ( u0_t* )src->data + i * perspective->size;
                            perspective->init( perspective, dst_obj );
                            perspective->copy( perspective, dst_obj, src_obj );
                        }
                    }
                    dst->size = src->size;
                    dst->type = src->type;
                }
            }
            break;

            case BCORE_CAPS_STATIC_LINK_ARRAY:
            {
                bcore_flect_caps_static_link_array_s* dst = dst_obj;
                bcore_flect_caps_static_link_array_s* src = src_obj;
                const bcore_object_s* perspective = item->perspective;
                for( sz_t i = 0; i < dst->size; i++ ) perspective->discard( perspective, dst->data[ i ] );
                if( src->size > dst->space )
                {
                    dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, 0,         &dst->space );
                    dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, src->size, &dst->space );
                }
                for( sz_t i = 0; i < src->size; i++ ) dst->data[ i ] = perspective->clone( perspective, src->data[ i ] );
                dst->size = src->size;
            }
            break;

            case BCORE_CAPS_TYPED_LINK_ARRAY:
            {
                bcore_flect_caps_typed_link_array_s* dst = dst_obj;
                bcore_flect_caps_typed_link_array_s* src = src_obj;
                if( dst->size > 0 )
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( dst->type );
                    for( sz_t i = 0; i < dst->size; i++ ) perspective->discard( perspective, dst->data[ i ] );
                    dst->size = 0;
                }
                if( src->size > 0 )
                {
                    const bcore_object_s* perspective = bcore_object_s_get_typed( src->type );
                    if( src->size > dst->space )
                    {
                        dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, 0,         &dst->space );
                        dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, src->size, &dst->space );
                    }
                    for( sz_t i = 0; i < src->size; i++ ) dst->data[ i ] = perspective->clone( perspective, src->data[ i ] );
                }
                dst->size = src->size;
                dst->type = src->type;
            }
            break;

            case BCORE_CAPS_AWARE_LINK_ARRAY:
            {
                bcore_flect_caps_aware_link_array_s* dst = dst_obj;
                bcore_flect_caps_aware_link_array_s* src = src_obj;
                if( dst->size > 0 )
                {
                    for( sz_t i = 0; i < dst->size; i++ )
                    {
                        const bcore_object_s* perspective = bcore_object_s_get_typed( *( aware_t* )dst->data[ i ] );
                        perspective->discard( perspective, dst->data[ i ] );
                    }
                    dst->size = 0;
                }
                if( src->size > 0 )
                {
                    if( src->size > dst->space )
                    {
                        dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, 0,         &dst->space );
                        dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, src->size, &dst->space );
                    }
                    for( sz_t i = 0; i < src->size; i++ )
                    {
                        const bcore_object_s* perspective = bcore_object_s_get_typed( *( aware_t* )src->data[ i ] );
                        dst->data[ i ] = perspective->clone( perspective, src->data[ i ] );
                    }
                }
                dst->size = src->size;
            }
            break;

            default:
            {
                ERR( "Unhandled capsulation %u", flect_item->caps );
            }
        }
    }
}

void bcore_object_s__move_o( const bcore_object_s* o, void* obj, void* src )
{
    if( obj == src ) return;
    o->move_o( obj, src );
}

void bcore_object_s__move( const bcore_object_s* o, void* obj, void* src )
{
    if( obj == src ) return;
    o->init( o, obj );
    o->copy( o, obj, src );
    o->down( o, src );
}

vd_t bcore_object_s__create_o( const bcore_object_s* o )
{
    return o->create_o();
}

vd_t bcore_object_s__create( const bcore_object_s* o )
{
    vd_t obj = bcore_u_alloc( o->size, NULL, 1, NULL);
    o->init( o, obj );
    return obj;
}

void bcore_object_s__discard_o( const bcore_object_s* o, vd_t obj )
{
    if( !obj ) return;
    o->discard_o( obj );
}

void bcore_object_s__discard( const bcore_object_s* o, vd_t obj )
{
    if( !obj ) return;
    o->down( o, obj );
    bcore_un_alloc( o->size, obj, 1, 0, NULL);
}

void* bcore_object_s__clone_o( const bcore_object_s* o, const void* src )
{
    if( !src ) return NULL;
    return o->clone_o( src );
}

void* bcore_object_s__clone( const bcore_object_s* o, const void* src )
{
    if( !src ) return NULL;
    vd_t obj = o->create( o );
    o->copy( o, obj, src );
    return obj;
}

bcore_object_s* bcore_object_s_create_from_self( const bcore_flect_self_s* self )
{
    bcore_object_s* o = bcore_object_s_create();
    o->_.p_type = bcore_name_enroll( "bcore_object_s" );
    o->_.o_type = self->type;
    o->init_flat = true;
    o->down_flat = true;
    o->copy_flat = true;
    o->move_flat = true;
    o->size  = self->size;
    o->aware = self->aware;
    if( o->aware ) o->init_flat = 0;
    if( self->body && self->body->size > 0 )
    {
        const bcore_flect_body_s* body = self->body;
        for( sz_t i = 0; i < body->size; i++ )
        {
            const bcore_flect_item_s* item = &body->data[ i ];
            if( item->caps == BCORE_CAPS_EXTERNAL_FUNC )
            {
                if( item->type == typeof( "bcore_fp_init" ) )
                {
                    o->init_o = ( bcore_fp_init )item->f_ptr;
                    o->init_flat = false;
                }
                else if( item->type == typeof( "bcore_fp_down" ) )
                {
                    o->down_o = ( bcore_fp_down )item->f_ptr;
                    o->down_flat = false;
                }
                else if( item->type == typeof( "bcore_fp_copy" ) )
                {
                    o->copy_o = ( bcore_fp_copy )item->f_ptr;
                    o->copy_flat = false;
                }
                else if( item->type == typeof( "bcore_fp_create" ) )
                {
                    o->create_o = ( bcore_fp_create )item->f_ptr;
                }
                else if( item->type == typeof( "bcore_fp_discard" ) )
                {
                    o->discard_o = ( bcore_fp_discard )item->f_ptr;
                }
                else if( item->type == typeof( "bcore_fp_clone"   ) )
                {
                    o->clone_o = ( bcore_fp_clone )item->f_ptr;
                }
            }
            else if( item->caps == BCORE_CAPS_EXTERNAL_DATA )
            {
                /* nothing yet */
            }
            else
            {
                bcore_object_item_s* o_item = bcore_object_s_push( o );
                o_item->flect_item = item;
                if( item->type )
                {
                    o_item->perspective = bcore_object_s_get_typed( item->type );
                    o->init_flat = o->init_flat & o_item->perspective->init_flat;
                    o->copy_flat = o->copy_flat & o_item->perspective->copy_flat;
                    o->down_flat = o->down_flat & o_item->perspective->down_flat;
                }
                switch( item->caps )
                {
                    case BCORE_CAPS_STATIC: break;
                    case BCORE_CAPS_STATIC_LINK:
                    case BCORE_CAPS_TYPED_LINK:
                    case BCORE_CAPS_AWARE_LINK:
                    case BCORE_CAPS_STATIC_ARRAY:
                    case BCORE_CAPS_TYPED_ARRAY:
                    case BCORE_CAPS_STATIC_LINK_ARRAY:
                    case BCORE_CAPS_TYPED_LINK_ARRAY:
                    case BCORE_CAPS_AWARE_LINK_ARRAY:
                    {
                        o->copy_flat = false;
                        o->down_flat = false;
                    }
                    break;
                    default: break;
                }
            }
        }
    }

    if( o->copy_flat && !o->down_flat ) ERR( "copy_flat is not implying down_flat" );
    o->move_flat = o->init_flat && o->copy_flat && o->down_flat;

    o->init    = o->init_o    ? bcore_object_s__init_o    : o->init_flat ? bcore_object_s__init_flat : bcore_object_s__init_generic;
    o->down    = o->down_o    ? bcore_object_s__down_o    : o->down_flat ? bcore_object_s__down_flat : bcore_object_s__down_generic;
    o->copy    = o->copy_o    ? bcore_object_s__copy_o    : o->copy_flat ? bcore_object_s__copy_flat : bcore_object_s__copy_generic;
    o->move    = o->move_o    ? bcore_object_s__move_o    : bcore_object_s__move;
    o->create  = o->create_o  ? bcore_object_s__create_o  : bcore_object_s__create;
    o->discard = o->discard_o ? bcore_object_s__discard_o : bcore_object_s__discard;
    o->clone   = o->clone_o   ? bcore_object_s__clone_o   : bcore_object_s__clone;

    return o;
}

/**********************************************************************************************************************/

const bcore_object_s* bcore_object_s_get_typed( u2_t o_type )
{
    u2_t p_type = typeof( "bcore_object_s" );
    const bcore_object_s* perspective = ( const bcore_object_s* )bcore_perspective_try_perspective( p_type, o_type );
    if( !perspective )
    {
        const bcore_flect_self_s* o_self = bcore_flect_get_self( o_type );
        bcore_object_s* new_perspective = bcore_object_s_create_from_self( o_self );
        bcore_perspective_enroll( p_type, o_type, ( bcore_perspective_s* )new_perspective );
        perspective = new_perspective;
    }
    return perspective;
}

/**********************************************************************************************************************/

bcore_string_s* bcore_object_perspective_selftest()
{
    typedef struct { aware_t _; bcore_flect_caps_typed_link_array_s string_arr; u2_t val1; u3_t val2; s1_t val3; } test_object1_s;

    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " test_object1_s = { aware_t _; typed * [] string_arr; u2_t val1; u3_t val2; s1_t val3; }" ) );

    typedef struct { aware_t _; u2_t val1; test_object1_s* o1; } test_object2_s;
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " test_object2_s = { aware_t _; u2_t val1; test_object1_s* o1; }" ) );

    test_object1_s* o1 = bcore_object_typed_create( typeof( "test_object1_s" ) );

    ASSERT( bcore_strcmp( nameof( o1->_ ), "test_object1_s" ) == 0 );
    ASSERT( o1->val1 == 0 );
    ASSERT( o1->val2 == 0 );
    ASSERT( o1->val3 == 0 );

    o1->val1 = 1234;
    o1->val2 = 1000000000001;
    o1->val3 = -5;

    o1->string_arr.type = typeof( "bcore_string_s" );
    o1->string_arr.space = 20;
    o1->string_arr.data = bcore_u_alloc( sizeof( vd_t ), NULL, o1->string_arr.space, &o1->string_arr.space );
    o1->string_arr.size = 10;
    for( sz_t i = 0; i < o1->string_arr.size; i++ ) o1->string_arr.data[ i ] = bcore_string_s_createf( "String number %04i", i );
    test_object2_s* o2 = bcore_object_typed_create( typeof( "test_object2_s" ) );
    o2->o1 = bcore_object_aware_clone( o1 );

    test_object2_s* o3 = bcore_object_aware_clone( o2 );

    ASSERT( bcore_strcmp( nameof( o3->_ ), "test_object2_s" ) == 0 );
    ASSERT( o3->o1->val1 == 1234 );
    ASSERT( o3->o1->val2 == 1000000000001 );
    ASSERT( o3->o1->val3 == -5 );
    for( sz_t i = 0; i < o3->o1->string_arr.size; i++ )
    {
        bcore_string_s* s = bcore_string_s_createf( "String number %04i", i );
        ASSERT( bcore_strcmp( ( ( bcore_string_s* )o3->o1->string_arr.data[ i ] )->sc, s->sc ) == 0 );
        bcore_string_s_discard( s );
    }

    bcore_object_aware_discard( o1 );
    bcore_object_aware_discard( o2 );
    bcore_object_aware_discard( o3 );

    return NULL;
}

