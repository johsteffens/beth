/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_instance_perspective.h"
#include "bcore_quicktypes.h"
#include "bcore_spect.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT( bcore_instance_item_s )
DEFINE_FUNCTION_DOWN_FLAT( bcore_instance_item_s )
DEFINE_FUNCTION_COPY_FLAT( bcore_instance_item_s )
DEFINE_FUNCTION_MOVE(      bcore_instance_item_s )
DEFINE_FUNCTION_CREATE(    bcore_instance_item_s )
DEFINE_FUNCTION_DISCARD(   bcore_instance_item_s )
DEFINE_FUNCTION_CLONE(     bcore_instance_item_s )

void bcore_instance_item_s_set_size_align( bcore_instance_item_s* o )
{
    if( o->flect_item->caps == BCORE_CAPS_STATIC )
    {
        const bcore_instance_s* inst = bcore_instance_s_get_typed( o->flect_item->type );
        o->size  = inst->size;
        o->align = inst->align;
    }
    else
    {
        o->size  = bcore_flect_caps_e_size( o->flect_item->caps );
        o->align = bcore_flect_caps_e_align( o->flect_item->caps );
    }
}

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT( bcore_instance_body_s )

void bcore_instance_body_s_down( bcore_instance_body_s* o )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_instance_item_s_down( &o->data[ i ] );
    o->data = bcore_un_alloc( sizeof( bcore_instance_item_s ), o->data, o->space, 0, &o->space );
    o->size = 0;
}

void bcore_instance_body_s_copy( bcore_instance_body_s* o, const bcore_instance_body_s* src )
{
    for( sz_t i = 0; i < o->size; i++ ) bcore_instance_item_s_down( &o->data[ i ] );
    o->data = bcore_un_alloc( sizeof( bcore_instance_item_s ), o->data, o->space, 0,         &o->space );
    o->data = bcore_un_alloc( sizeof( bcore_instance_item_s ), o->data, o->space, src->size, &o->space );
    for( sz_t i = 0; i < src->size; i++ ) bcore_instance_item_s_copy( &o->data[ i ], &src->data[ i ] );
    o->size = src->size;
}

DEFINE_FUNCTION_CREATE(    bcore_instance_body_s )
DEFINE_FUNCTION_DISCARD(   bcore_instance_body_s )
DEFINE_FUNCTION_CLONE(     bcore_instance_body_s )

bcore_instance_item_s* bcore_instance_body_s_push( bcore_instance_body_s* o )
{
    if( o->size == o->space )
    {
        sz_t old_space = o->space;
        bcore_instance_item_s* old_data = o->data;
        o->data = bcore_u_alloc( sizeof( bcore_instance_item_s ), NULL, o->space > 0 ? o->space * 2 : 1, &o->space );
        for( sz_t i = 0; i < o->size; i++  ) bcore_instance_item_s_move( &o->data[ i ], &old_data[ i ] );
        bcore_un_alloc( sizeof( bcore_instance_item_s ), old_data, old_space, 0, NULL );
    }
    bcore_instance_item_s* item = &o->data[ o->size ];
    o->size++;
    bcore_instance_item_s_init( item );
    return item;
}

void bcore_instance_body_s_pop( bcore_instance_body_s* o )
{
    if( o->size == 0 ) return;
    o->size--;
    bcore_instance_item_s_down( &o->data[ o->size ] );
}

/**********************************************************************************************************************/
// bcore_instance_s

static void instance_s_down( bcore_instance_s* o );

void instance_s_init( bcore_instance_s* o )
{
    bcore_memzero( o, sizeof( bcore_instance_s ) );
    o->p_type = typeof( "bcore_instance_s" );
}

void instance_s_down( bcore_instance_s* o )
{
    bcore_instance_body_s_discard( o->body );
}

static bcore_instance_s* instance_s_create()
{
    bcore_instance_s* o = bcore_alloc( NULL, sizeof( bcore_instance_s ) );
    instance_s_init( o );
    return o;
}

static void instance_s_discard( bcore_instance_s* o )
{
    if( !o ) return;
    instance_s_down( o );
    bcore_free( o );
}

static bcore_signature_s* instance_s_create_signature( bcore_instance_s* o )
{
    return bcore_signature_s_create_an( 2, o->p_type, o->o_type );
}

static bcore_instance_item_s* instance_s_push( bcore_instance_s* o )
{
    if( !o->body ) o->body = bcore_instance_body_s_create();
    return bcore_instance_body_s_push( o->body );
}

static void verify_aware_type( tp_t type, vc_t obj, sc_t context )
{
    if( type != *( aware_t* )obj )
    {
        const char* name = bcore_name_try_name( *( aware_t* )obj );
        if( name )
        {
            ERR( "%s on '%s'; expected '%s'", context, ifnameof( *( aware_t* )obj ), ifnameof( type ) );
        }
        else
        {
            ERR( "%s on undefined object; expected '%s'", context, ifnameof( type ) );
        }
    }
}

bcore_flect_self_s* bcore_instance_s_create_self()
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_instance_s" ), sizeof( bcore_instance_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )instance_s_init,             "bcore_fp_init",                    "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )instance_s_down,             "bcore_fp_down",                    "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )instance_s_create,           "bcore_fp_create",                  "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )instance_s_discard,          "bcore_fp_discard",                 "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )instance_s_create_signature, "bcore_spect_fp_create_signature",  "create_signature" );
    return self;
}

/**********************************************************************************************************************/

static void init_o( const bcore_instance_s* p, vd_t o )
{
#ifndef NDEBUG
    if( !o ) ERR( "o == NULL" );
#endif // NDEBUG
    p->init_o( o );
}

static void init_flat( const bcore_instance_s* p, vd_t o )
{
#ifndef NDEBUG
    if( !o ) ERR( "o == NULL" );
#endif
    bcore_memzero( o, p->size );
}

static void init_generic( const bcore_instance_s* p, vd_t o )
{
#ifndef NDEBUG
    if( !o ) ERR( "o == NULL" );
#endif
    bcore_memzero( o, p->size );
    sz_t first = 0;
    if( p->aware )
    {
        *( aware_t* )o = p->o_type;
        first = 1;
    }
    for( sz_t i = first; i < p->body->size; i++ )
    {
        const bcore_instance_item_s* inst_item = &p->body->data[ i ];
        const bcore_flect_item_s*   flect_item = inst_item->flect_item;
        void* item_obj = ( u0_t* )o + inst_item->offset;

        switch( flect_item->caps )
        {
            case BCORE_CAPS_STATIC:
            {
                const bcore_instance_s* perspective = inst_item->perspective;
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

/**********************************************************************************************************************/

static void down_o( const bcore_instance_s* p, vd_t o )
{
#ifndef NDEBUG
    if( !o ) ERR( "o == NULL" );
    if( p->aware ) verify_aware_type( p->o_type, o, __func__ );
#endif // NDEBUG
    p->down_o( o );
}

static void down_flat( const bcore_instance_s* p, vd_t o )
{
#ifndef NDEBUG
    if( !o ) ERR( "o == NULL" );
    if( p->aware ) verify_aware_type( p->o_type, o, __func__ );
#endif // NDEBUG
    /* nothing to do */
}

static void down_generic( const bcore_instance_s* p, vd_t o )
{
#ifndef NDEBUG
    if( !o ) ERR( "o == NULL" );
    if( p->aware ) verify_aware_type( p->o_type, o, __func__ );
#endif // NDEBUG
    for( sz_t i = 0; i < p->body->size; i++ )
    {
        const bcore_instance_item_s* inst_item = &p->body->data[ i ];
        if( inst_item->no_trace ) continue;

        const bcore_flect_item_s* flect_item = inst_item->flect_item;
        void* item_obj = ( u0_t* )o + inst_item->offset;

        switch( flect_item->caps )
        {
            case BCORE_CAPS_STATIC:
            {
                const bcore_instance_s* perspective = inst_item->perspective;
                if( !perspective->down_flat ) perspective->down( perspective, item_obj );
            }
            break;

            case BCORE_CAPS_STATIC_LINK:
            {
                bcore_static_link_s* s = item_obj;
                if( s->link )
                {
                    const bcore_instance_s* perspective = inst_item->perspective;
                    perspective->discard( perspective, s->link );
                }
            }
            break;

            case BCORE_CAPS_TYPED_LINK:
            {
                bcore_typed_link_s* s = item_obj;
                if( s->link )
                {
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( s->type );
                    perspective->discard( perspective, s->link );
                }
            }
            break;

            case BCORE_CAPS_AWARE_LINK:
            {
                bcore_aware_link_s* s = item_obj;
                if( s->link )
                {
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( *( aware_t* )s->link );
                    perspective->discard( perspective, s->link );
                }
            }
            break;

            case BCORE_CAPS_STATIC_ARRAY:
            {
                bcore_static_array_s* s = item_obj;
                if( s->data )
                {
                    const bcore_instance_s* perspective = inst_item->perspective;
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
                bcore_typed_array_s* s = item_obj;
                if( s->data )
                {
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( s->type );
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
                bcore_static_link_array_s* s = item_obj;
                if( s->data )
                {
                    const bcore_instance_s* perspective = inst_item->perspective;
                    for( sz_t i = 0; i < s->size; i++ ) perspective->discard( perspective, s->data[ i ] );
                    s->data = bcore_un_alloc( sizeof( vd_t ), s->data, s->space, 0, &s->space );
                }
            }
            break;

            case BCORE_CAPS_TYPED_LINK_ARRAY:
            {
                bcore_typed_link_array_s* s = item_obj;
                if( s->data )
                {
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( s->type );
                    for( sz_t i = 0; i < s->size; i++ ) perspective->discard( perspective, s->data[ i ] );
                    s->data = bcore_un_alloc( sizeof( vd_t ), s->data, s->space, 0, &s->space );
                }
            }
            break;

            case BCORE_CAPS_AWARE_LINK_ARRAY:
            {
                bcore_aware_link_array_s* s = item_obj;
                if( s->data )
                {
                    for( sz_t i = 0; i < s->size; i++ ) bcore_instance_aware_discard( s->data[ i ] );
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

/**********************************************************************************************************************/

static void copy_o( const bcore_instance_s* p, vd_t dst, vc_t src )
{
    if( dst == src ) return;
#ifndef NDEBUG
    if( !dst ) ERR( "dst == NULL" );
    if( !src ) ERR( "src == NULL" );
    if( p->aware )
    {
        verify_aware_type( p->o_type, dst, __func__ );
        verify_aware_type( p->o_type, src, __func__ );
    }
#endif // NDEBUG
    p->copy_o( dst, src );
}

static void copy_flat( const bcore_instance_s* p, vd_t dst, vc_t src )
{
    if( dst == src ) return;
#ifndef NDEBUG
    if( !dst ) ERR( "dst == NULL" );
    if( !src ) ERR( "src == NULL" );
    if( p->aware )
    {
        verify_aware_type( p->o_type, dst, __func__ );
        verify_aware_type( p->o_type, src, __func__ );
    }
#endif // NDEBUG
    bcore_memcpy( dst, src, p->size );
}

static void copy_generic( const bcore_instance_s* p, vd_t dst, vc_t src )
{
    if( dst == src ) return;
#ifndef NDEBUG
    if( !dst ) ERR( "dst == NULL" );
    if( !src ) ERR( "src == NULL" );
    if( p->aware )
    {
        verify_aware_type( p->o_type, dst, __func__ );
        verify_aware_type( p->o_type, src, __func__ );
    }
#endif // NDEBUG
    for( sz_t i = 0; i < p->body->size; i++ )
    {
        const bcore_instance_item_s* inst_item = &p->body->data[ i ];
        if( inst_item->no_trace ) continue;

        const bcore_flect_item_s* flect_item = inst_item->flect_item;
        void* dst_obj = ( u0_t* )dst + inst_item->offset;
        void* src_obj = ( u0_t* )src + inst_item->offset;

        switch( flect_item->caps )
        {
            case BCORE_CAPS_STATIC:
            {
                const bcore_instance_s* perspective = inst_item->perspective;
                perspective->copy( perspective, dst_obj, src_obj );
            }
            break;

            case BCORE_CAPS_STATIC_LINK:
            {
                bcore_static_link_s* dst = dst_obj;
                bcore_static_link_s* src = src_obj;
                const bcore_instance_s* perspective = inst_item->perspective;
                perspective->discard( perspective, dst->link );
                dst->link = perspective->clone( perspective, src->link );
            }
            break;

            case BCORE_CAPS_TYPED_LINK:
            {
                bcore_typed_link_s* dst = dst_obj;
                bcore_typed_link_s* src = src_obj;
                if( dst->link )
                {
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( dst->type );
                    perspective->discard( perspective, dst->link );
                }
                if( src->link )
                {
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( src->type );
                    dst->link = perspective->clone( perspective, src->link );
                }
                dst->type = src->type;
            }
            break;

            case BCORE_CAPS_AWARE_LINK:
            {
                bcore_aware_link_s* dst = dst_obj;
                bcore_aware_link_s* src = src_obj;
                if( dst->link )
                {
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( *( aware_t* )dst->link );
                    perspective->discard( perspective, dst->link );
                }
                if( src->link )
                {
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( *( aware_t* )src->link );
                    dst->link = perspective->clone( perspective, src->link );
                }
            }
            break;

            case BCORE_CAPS_STATIC_ARRAY:
            {
                bcore_static_array_s* dst = dst_obj;
                bcore_static_array_s* src = src_obj;
                const bcore_instance_s* perspective = inst_item->perspective;
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
                bcore_typed_array_s* dst = dst_obj;
                bcore_typed_array_s* src = src_obj;

                if( dst->space > 0 ) // deplete dst
                {
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( dst->type );
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
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( src->type );
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
                bcore_static_link_array_s* dst = dst_obj;
                bcore_static_link_array_s* src = src_obj;
                const bcore_instance_s* perspective = inst_item->perspective;
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
                bcore_typed_link_array_s* dst = dst_obj;
                bcore_typed_link_array_s* src = src_obj;
                if( dst->size > 0 )
                {
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( dst->type );
                    for( sz_t i = 0; i < dst->size; i++ ) perspective->discard( perspective, dst->data[ i ] );
                    dst->size = 0;
                }
                if( src->size > 0 )
                {
                    const bcore_instance_s* perspective = bcore_instance_s_get_typed( src->type );
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
                bcore_aware_link_array_s* dst = dst_obj;
                bcore_aware_link_array_s* src = src_obj;
                if( dst->size > 0 )
                {
                    for( sz_t i = 0; i < dst->size; i++ )
                    {
                        const bcore_instance_s* perspective = bcore_instance_s_get_typed( *( aware_t* )dst->data[ i ] );
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
                        const bcore_instance_s* perspective = bcore_instance_s_get_typed( *( aware_t* )src->data[ i ] );
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

/**********************************************************************************************************************/

static void copy_typed_o( const bcore_instance_s* p, vd_t dst, tp_t type, vc_t src )
{
    if( dst == src ) return;
#ifndef NDEBUG
    if( !dst ) ERR( "dst == NULL" );
    if( !src ) ERR( "src == NULL" );
    if( p->aware ) verify_aware_type( p->o_type, dst, __func__ );
#endif // NDEBUG
    p->copy_typed_o( dst, type, src );
}

static void copy_typed( const bcore_instance_s* p, vd_t dst, tp_t type, vc_t src )
{
    tp_t dst_type = p->o_type;
    switch( dst_type )
    {
        case TYPEOF_s3_t:
        {
            s3_t* dst_l = dst;
            switch( type )
            {
                case TYPEOF_s3_t: *dst_l = *( s3_t* )src; return;
                case TYPEOF_s2_t: *dst_l = *( s2_t* )src; return;
                case TYPEOF_s1_t: *dst_l = *( s1_t* )src; return;
                case TYPEOF_s0_t: *dst_l = *( s0_t* )src; return;
                case TYPEOF_u3_t: *dst_l = *( u3_t* )src; return;
                case TYPEOF_u2_t: *dst_l = *( u2_t* )src; return;
                case TYPEOF_u1_t: *dst_l = *( u1_t* )src; return;
                case TYPEOF_u0_t: *dst_l = *( u0_t* )src; return;
                case TYPEOF_f3_t: *dst_l = *( f3_t* )src; return;
                case TYPEOF_f2_t: *dst_l = *( f2_t* )src; return;
                case TYPEOF_sz_t: *dst_l = *( sz_t* )src; return;
                default: break;
            }
        }
        case TYPEOF_s2_t:
        {
            s2_t* dst_l = dst;
            switch( type )
            {
                case TYPEOF_s3_t: *dst_l = *( s3_t* )src; return;
                case TYPEOF_s2_t: *dst_l = *( s2_t* )src; return;
                case TYPEOF_s1_t: *dst_l = *( s1_t* )src; return;
                case TYPEOF_s0_t: *dst_l = *( s0_t* )src; return;
                case TYPEOF_u3_t: *dst_l = *( u3_t* )src; return;
                case TYPEOF_u2_t: *dst_l = *( u2_t* )src; return;
                case TYPEOF_u1_t: *dst_l = *( u1_t* )src; return;
                case TYPEOF_u0_t: *dst_l = *( u0_t* )src; return;
                case TYPEOF_f3_t: *dst_l = *( f3_t* )src; return;
                case TYPEOF_f2_t: *dst_l = *( f2_t* )src; return;
                case TYPEOF_sz_t: *dst_l = *( sz_t* )src; return;
                default: break;
            }
        }
        case TYPEOF_s1_t:
        {
            s1_t* dst_l = dst;
            switch( type )
            {
                case TYPEOF_s3_t: *dst_l = *( s3_t* )src; return;
                case TYPEOF_s2_t: *dst_l = *( s2_t* )src; return;
                case TYPEOF_s1_t: *dst_l = *( s1_t* )src; return;
                case TYPEOF_s0_t: *dst_l = *( s0_t* )src; return;
                case TYPEOF_u3_t: *dst_l = *( u3_t* )src; return;
                case TYPEOF_u2_t: *dst_l = *( u2_t* )src; return;
                case TYPEOF_u1_t: *dst_l = *( u1_t* )src; return;
                case TYPEOF_u0_t: *dst_l = *( u0_t* )src; return;
                case TYPEOF_f3_t: *dst_l = *( f3_t* )src; return;
                case TYPEOF_f2_t: *dst_l = *( f2_t* )src; return;
                case TYPEOF_sz_t: *dst_l = *( sz_t* )src; return;
                default: break;
            }
        }
        case TYPEOF_s0_t:
        {
            s0_t* dst_l = dst;
            switch( type )
            {
                case TYPEOF_s3_t: *dst_l = *( s3_t* )src; return;
                case TYPEOF_s2_t: *dst_l = *( s2_t* )src; return;
                case TYPEOF_s1_t: *dst_l = *( s1_t* )src; return;
                case TYPEOF_s0_t: *dst_l = *( s0_t* )src; return;
                case TYPEOF_u3_t: *dst_l = *( u3_t* )src; return;
                case TYPEOF_u2_t: *dst_l = *( u2_t* )src; return;
                case TYPEOF_u1_t: *dst_l = *( u1_t* )src; return;
                case TYPEOF_u0_t: *dst_l = *( u0_t* )src; return;
                case TYPEOF_f3_t: *dst_l = *( f3_t* )src; return;
                case TYPEOF_f2_t: *dst_l = *( f2_t* )src; return;
                case TYPEOF_sz_t: *dst_l = *( sz_t* )src; return;
                default: break;
            }
        }
        case TYPEOF_u3_t:
        {
            u3_t* dst_l = dst;
            switch( type )
            {
                case TYPEOF_s3_t: *dst_l = *( s3_t* )src; return;
                case TYPEOF_s2_t: *dst_l = *( s2_t* )src; return;
                case TYPEOF_s1_t: *dst_l = *( s1_t* )src; return;
                case TYPEOF_s0_t: *dst_l = *( s0_t* )src; return;
                case TYPEOF_u3_t: *dst_l = *( u3_t* )src; return;
                case TYPEOF_u2_t: *dst_l = *( u2_t* )src; return;
                case TYPEOF_u1_t: *dst_l = *( u1_t* )src; return;
                case TYPEOF_u0_t: *dst_l = *( u0_t* )src; return;
                case TYPEOF_f3_t: *dst_l = *( f3_t* )src; return;
                case TYPEOF_f2_t: *dst_l = *( f2_t* )src; return;
                case TYPEOF_sz_t: *dst_l = *( sz_t* )src; return;
                default: break;
            }
        }
        case TYPEOF_u2_t:
        {
            u2_t* dst_l = dst;
            switch( type )
            {
                case TYPEOF_s3_t: *dst_l = *( s3_t* )src; return;
                case TYPEOF_s2_t: *dst_l = *( s2_t* )src; return;
                case TYPEOF_s1_t: *dst_l = *( s1_t* )src; return;
                case TYPEOF_s0_t: *dst_l = *( s0_t* )src; return;
                case TYPEOF_u3_t: *dst_l = *( u3_t* )src; return;
                case TYPEOF_u2_t: *dst_l = *( u2_t* )src; return;
                case TYPEOF_u1_t: *dst_l = *( u1_t* )src; return;
                case TYPEOF_u0_t: *dst_l = *( u0_t* )src; return;
                case TYPEOF_f3_t: *dst_l = *( f3_t* )src; return;
                case TYPEOF_f2_t: *dst_l = *( f2_t* )src; return;
                case TYPEOF_sz_t: *dst_l = *( sz_t* )src; return;
                default: break;
            }
        }
        case TYPEOF_u1_t:
        {
            u1_t* dst_l = dst;
            switch( type )
            {
                case TYPEOF_s3_t: *dst_l = *( s3_t* )src; return;
                case TYPEOF_s2_t: *dst_l = *( s2_t* )src; return;
                case TYPEOF_s1_t: *dst_l = *( s1_t* )src; return;
                case TYPEOF_s0_t: *dst_l = *( s0_t* )src; return;
                case TYPEOF_u3_t: *dst_l = *( u3_t* )src; return;
                case TYPEOF_u2_t: *dst_l = *( u2_t* )src; return;
                case TYPEOF_u1_t: *dst_l = *( u1_t* )src; return;
                case TYPEOF_u0_t: *dst_l = *( u0_t* )src; return;
                case TYPEOF_f3_t: *dst_l = *( f3_t* )src; return;
                case TYPEOF_f2_t: *dst_l = *( f2_t* )src; return;
                case TYPEOF_sz_t: *dst_l = *( sz_t* )src; return;
                default: break;
            }
        }
        case TYPEOF_u0_t:
        {
            u0_t* dst_l = dst;
            switch( type )
            {
                case TYPEOF_s3_t: *dst_l = *( s3_t* )src; return;
                case TYPEOF_s2_t: *dst_l = *( s2_t* )src; return;
                case TYPEOF_s1_t: *dst_l = *( s1_t* )src; return;
                case TYPEOF_s0_t: *dst_l = *( s0_t* )src; return;
                case TYPEOF_u3_t: *dst_l = *( u3_t* )src; return;
                case TYPEOF_u2_t: *dst_l = *( u2_t* )src; return;
                case TYPEOF_u1_t: *dst_l = *( u1_t* )src; return;
                case TYPEOF_u0_t: *dst_l = *( u0_t* )src; return;
                case TYPEOF_f3_t: *dst_l = *( f3_t* )src; return;
                case TYPEOF_f2_t: *dst_l = *( f2_t* )src; return;
                case TYPEOF_sz_t: *dst_l = *( sz_t* )src; return;
                default: break;
            }
        }
        case TYPEOF_f3_t:
        {
            f3_t* dst_l = dst;
            switch( type )
            {
                case TYPEOF_s3_t: *dst_l = *( s3_t* )src; return;
                case TYPEOF_s2_t: *dst_l = *( s2_t* )src; return;
                case TYPEOF_s1_t: *dst_l = *( s1_t* )src; return;
                case TYPEOF_s0_t: *dst_l = *( s0_t* )src; return;
                case TYPEOF_u3_t: *dst_l = *( u3_t* )src; return;
                case TYPEOF_u2_t: *dst_l = *( u2_t* )src; return;
                case TYPEOF_u1_t: *dst_l = *( u1_t* )src; return;
                case TYPEOF_u0_t: *dst_l = *( u0_t* )src; return;
                case TYPEOF_f3_t: *dst_l = *( f3_t* )src; return;
                case TYPEOF_f2_t: *dst_l = *( f2_t* )src; return;
                case TYPEOF_sz_t: *dst_l = *( sz_t* )src; return;
                default: break;
            }
        }
        case TYPEOF_f2_t:
        {
            f2_t* dst_l = dst;
            switch( type )
            {
                case TYPEOF_s3_t: *dst_l = *( s3_t* )src; return;
                case TYPEOF_s2_t: *dst_l = *( s2_t* )src; return;
                case TYPEOF_s1_t: *dst_l = *( s1_t* )src; return;
                case TYPEOF_s0_t: *dst_l = *( s0_t* )src; return;
                case TYPEOF_u3_t: *dst_l = *( u3_t* )src; return;
                case TYPEOF_u2_t: *dst_l = *( u2_t* )src; return;
                case TYPEOF_u1_t: *dst_l = *( u1_t* )src; return;
                case TYPEOF_u0_t: *dst_l = *( u0_t* )src; return;
                case TYPEOF_f3_t: *dst_l = *( f3_t* )src; return;
                case TYPEOF_f2_t: *dst_l = *( f2_t* )src; return;
                case TYPEOF_sz_t: *dst_l = *( sz_t* )src; return;
                default: break;
            }
        }
        case TYPEOF_sz_t:
        {
            sz_t* dst_l = dst;
            switch( type )
            {
                case TYPEOF_s3_t: *dst_l = *( s3_t* )src; return;
                case TYPEOF_s2_t: *dst_l = *( s2_t* )src; return;
                case TYPEOF_s1_t: *dst_l = *( s1_t* )src; return;
                case TYPEOF_s0_t: *dst_l = *( s0_t* )src; return;
                case TYPEOF_u3_t: *dst_l = *( u3_t* )src; return;
                case TYPEOF_u2_t: *dst_l = *( u2_t* )src; return;
                case TYPEOF_u1_t: *dst_l = *( u1_t* )src; return;
                case TYPEOF_u0_t: *dst_l = *( u0_t* )src; return;
                case TYPEOF_f3_t: *dst_l = *( f3_t* )src; return;
                case TYPEOF_f2_t: *dst_l = *( f2_t* )src; return;
                case TYPEOF_sz_t: *dst_l = *( sz_t* )src; return;
                default: break;
            }
        }
        default: break;
    }

    if( dst_type == type )
    {
        p->copy( p, dst, src );
    }
    else
    {
        ERR( "Type conversion '%s' --> '%s' is not defined", ifnameof( type ), ifnameof( dst_type ) );
    }
}

static void copy_aware( const bcore_instance_s* p, vd_t dst, vc_t src )
{
    p->copy_typed( p, dst, *( aware_t* )src, src );
}

/**********************************************************************************************************************/

static void move_o( const bcore_instance_s* p, vd_t dst, vd_t src )
{
    if( dst == src ) return;
    p->move_o( dst, src );
}

static void move( const bcore_instance_s* p, vd_t dst, vd_t src )
{
    if( dst == src ) return;
    p->init( p, dst );
    p->copy( p, dst, src );
    p->down( p, src );
}

static vd_t create_o( const bcore_instance_s* p )
{
    return p->create_o();
}

static vd_t create( const bcore_instance_s* p )
{
    vd_t o = bcore_u_alloc( p->size, NULL, 1, NULL);
    p->init( p, o );
    return o;
}

static vd_t create_typed_o( const bcore_instance_s* p, tp_t t, vc_t src )
{
    return p->create_typed_o( t, src );
}

static vd_t create_typed( const bcore_instance_s* p, tp_t t, vc_t src )
{
    vd_t o = bcore_u_alloc( p->size, NULL, 1, NULL );
    p->init( p, o );
    p->copy_typed( p, o, t, src );
    return o;
}

static void discard_o( const bcore_instance_s* p, vd_t o )
{
    if( !o ) return;
    p->discard_o( o );
}

static void discard( const bcore_instance_s* p, vd_t o )
{
    if( !o ) return;
    p->down( p, o );
    bcore_un_alloc( p->size, o, 1, 0, NULL);
}

static vd_t clone_o( const bcore_instance_s* p, vc_t src )
{
    if( !src ) return NULL;
    return p->clone_o( src );
}

static vd_t clone( const bcore_instance_s* p, vc_t src )
{
    if( !src ) return NULL;
    vd_t obj = p->create( p );
    p->copy( p, obj, src );
    return obj;
}

/**********************************************************************************************************************/

static void check_sanity( const bcore_instance_s* p, vc_t o )
{
    if( p->check_sanity_o )
    {
        p->check_sanity_o( o );
    }
    else
    {
        if( p->aware )
        {
            verify_aware_type( p->o_type, o, "check_sanity" );
        }
    }
}

/**********************************************************************************************************************/

static sz_t aligned_offset( sz_t align, sz_t raw_offset )
{
    if( align < 2 ) return raw_offset;
    sz_t offset = raw_offset;
    if( ( offset & ( align - 1 ) ) != 0 ) offset = ( offset + align ) & ~( align - 1 );
    return offset;
}

bcore_instance_s* bcore_instance_s_create_from_self( const bcore_flect_self_s* self )
{
    bcore_instance_s* o = instance_s_create();
    o->o_type  = self->type;
    o->init_flat = true;
    o->down_flat = true;
    o->copy_flat = true;
    o->move_flat = true;
    o->align     = 0;

    if( self->body && self->body->size > 0 )
    {
        const bcore_flect_body_s* flect_body = self->body;
        bcore_instance_item_s* last_inst_item = NULL;

        for( sz_t i = 0; i < flect_body->size; i++ )
        {
            const bcore_flect_item_s* flect_item = &flect_body->data[ i ];

            if( flect_item->caps == BCORE_CAPS_EXTERNAL_FUNC )
            {
                if( flect_item->type == typeof( "bcore_fp_init" ) )
                {
                    o->init_o = ( bcore_fp_init )flect_item->f_ptr;
                    o->init_flat = false;
                }
                else if( flect_item->type == typeof( "bcore_fp_down" ) )
                {
                    o->down_o = ( bcore_fp_down )flect_item->f_ptr;
                    o->down_flat = false;
                }
                else if( flect_item->type == typeof( "bcore_fp_copy" ) )
                {
                    o->copy_o = ( bcore_fp_copy )flect_item->f_ptr;
                    o->copy_flat = false;
                }
                else if( flect_item->type == typeof( "bcore_fp_copy_typed" ) )
                {
                    o->copy_typed_o = ( bcore_fp_copy_typed )flect_item->f_ptr;
                }
                else if( flect_item->type == typeof( "bcore_fp_create" ) )
                {
                    o->create_o = ( bcore_fp_create )flect_item->f_ptr;
                }
                else if( flect_item->type == typeof( "bcore_fp_create_typed" ) )
                {
                    o->create_typed_o = ( bcore_fp_create_typed )flect_item->f_ptr;
                }
                else if( flect_item->type == typeof( "bcore_fp_discard" ) )
                {
                    o->discard_o = ( bcore_fp_discard )flect_item->f_ptr;
                }
                else if( flect_item->type == typeof( "bcore_fp_clone"   ) )
                {
                    o->clone_o = ( bcore_fp_clone )flect_item->f_ptr;
                }
                else if( flect_item->type == typeof( "bcore_fp_check_sanity"   ) )
                {
                    o->check_sanity_o = ( bcore_fp_check_sanity )flect_item->f_ptr;
                }
            }
            else if( flect_item->caps == BCORE_CAPS_EXTERNAL_DATA )
            {
                /* nothing yet */
            }
            else
            {
                bcore_instance_item_s* inst_item = instance_s_push( o );

                inst_item->flect_item = flect_item;
                if( flect_item->type )
                {
                    if( !bcore_flect_exists( flect_item->type ) )
                    {
                        ERR( "Constructing bcore_instance_s of %s:\n"
                             "Type '%s' (%"PRIu32") does not exist.", ifnameof( self->type ), ifnameof( flect_item->type ) );
                    }
                    else if( flect_item->type == self->type )
                    {
                        if( flect_item->caps == BCORE_CAPS_STATIC )
                        {
                            ERR( "Constructing bcore_instance_s of %s:\n"
                                 "Type '%s' (%"PRIu32") is static member of itself, which is inconstructible.\n"
                                 "A static link might be a viable alternative.\n" , ifnameof( self->type ), ifnameof( flect_item->type ) );
                        }
                        else
                        {
                            inst_item->perspective = o;
                        }
                    }
                    else
                    {
                        inst_item->perspective = bcore_instance_s_get_typed( flect_item->type );
                    }

                    if( flect_item->type == typeof( "aware_t" ) )
                    {
                        o->init_flat = false;
                        o->aware = true;
                    }
                    o->init_flat = o->init_flat & inst_item->perspective->init_flat;
                    o->copy_flat = o->copy_flat & inst_item->perspective->copy_flat;
                    o->down_flat = o->down_flat & inst_item->perspective->down_flat;
                }

                switch( flect_item->caps )
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

                if( flect_item->caps == BCORE_CAPS_STATIC )
                {
                    inst_item->size  = inst_item->perspective->size;
                    inst_item->align = inst_item->perspective->align;
                }
                else
                {
                    inst_item->size  = bcore_flect_caps_e_size(  flect_item->caps );
                    inst_item->align = bcore_flect_caps_e_align( flect_item->caps );
                }

                if( flect_item->attr == TYPEOF_private ||
                    flect_item->attr == TYPEOF_cyclic  ||
                    flect_item->attr == TYPEOF_external )
                {
                    inst_item->no_trace = true;
                    o->copy_flat        = false;
                }

                if( last_inst_item )
                {
                    inst_item->offset = aligned_offset( inst_item->align, last_inst_item->offset + last_inst_item->size );
                }
                else
                {
                    inst_item->offset = 0;
                }

                o->align = ( inst_item->align > o->align ) ? inst_item->align : o->align;

                bcore_instance_item_s_discard( last_inst_item );
                last_inst_item = bcore_instance_item_s_clone( inst_item );
            }
        }

        if( last_inst_item )
        {
            o->size = aligned_offset( o->align, last_inst_item->offset + last_inst_item->size );
            bcore_instance_item_s_discard( last_inst_item );
        }
        else
        {
            o->size = 0;
        }
    }
    else
    {
        o->size  = self->size;
        o->align = self->size;
    }

    if( ( self->size > 0 ) && ( self->size != o->size ) )
    {
        ERR( "Size mismatch: sizeof(%s):%zu differs from reflective body size:%zu.\n"
             "The object's reflection is probably out of sync with its compile-time definition.\n",
            ifnameof( self->type ), self->size, o->size );
    }

    if( o->copy_flat && !o->down_flat ) ERR( "copy_flat is not implying down_flat" );
    o->move_flat = o->init_flat && o->copy_flat && o->down_flat;

    o->init         = o->init_o       ? init_o         : o->init_flat ? init_flat : init_generic;
    o->down         = o->down_o       ? down_o         : o->down_flat ? down_flat : down_generic;
    o->copy         = o->copy_o       ? copy_o         : o->copy_flat ? copy_flat : copy_generic;
    o->copy_typed   = o->copy_typed_o ? copy_typed_o   : copy_typed;
    o->move         = o->move_o       ? move_o         : move;
    o->create       = o->create_o     ? create_o       : create;
    o->create_typed = o->create_o     ? create_typed_o : create_typed;
    o->discard      = o->discard_o    ? discard_o      : discard;
    o->clone        = o->clone_o      ? clone_o        : clone;

    o->copy_aware   = copy_aware;
    o->check_sanity = check_sanity;

    return o;
}

/**********************************************************************************************************************/

const bcore_instance_s* bcore_instance_s_get_typed( tp_t o_type )
{
    tp_t sig = bcore_signature_get_hash_na( 2, typeof( "bcore_instance_s" ), o_type );
    const bcore_instance_s* instance_p = bcore_spect_try( sig );
    if( !instance_p )
    {
        const bcore_flect_self_s* o_self = bcore_flect_get_self( o_type );
        bcore_instance_s* new_instance_p = bcore_instance_s_create_from_self( o_self );
        bcore_spect_enroll_d( new_instance_p );
        instance_p = new_instance_p;
    }
    return instance_p;
}

/**********************************************************************************************************************/

const bcore_instance_s* bcore_instance_s_get_aware( vc_t obj )
{
    return bcore_instance_s_get_typed( *( const aware_t* )obj );
}

void bcore_instance_spect_init( const bcore_instance_s* o, vd_t obj )
{
    o->init( o, obj );
}

void bcore_instance_typed_init( tp_t type, vd_t obj )
{
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    o->init( o, obj );
}

void bcore_instance_spect_down( const bcore_instance_s* o, vd_t obj )
{
    o->down( o, obj );
}

void bcore_instance_typed_down( tp_t type, vd_t obj )
{
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    o->down( o, obj );
}

void bcore_instance_aware_down( vd_t obj )
{
    const bcore_instance_s* o = bcore_instance_s_get_aware( obj  );
    o->down( o, obj );
}

void bcore_instance_spect_copy( const bcore_instance_s* o, vd_t dst, vc_t src )
{
    if( dst == src ) return;
    o->copy( o, dst, src );
}

void bcore_instance_typed_copy( tp_t type, vd_t dst, vc_t src )
{
    if( dst == src ) return;
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    o->copy( o, dst, src );
}

void bcore_instance_aware_copy( vd_t dst, vc_t src )
{
    if( dst == src ) return;
    const bcore_instance_s* o = bcore_instance_s_get_aware( dst );
    o->copy( o, dst, src );
}

void bcore_instance_spect_copy_typed( const bcore_instance_s* o, vd_t dst, tp_t src_type, vc_t src )
{
    if( dst == src ) return;
    if( o->o_type == src_type )
    {
        o->copy( o, dst, src );
    }
    else
    {
        o->copy_typed( o, dst, src_type, src );
    }
}

void bcore_instance_typed_copy_typed( tp_t type, vd_t dst, tp_t src_type, vc_t src )
{
    if( dst == src ) return;
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    if( o->o_type == src_type )
    {
        o->copy( o, dst, src );
    }
    else
    {
        o->copy_typed( o, dst, src_type, src );
    }
}

void bcore_instance_aware_copy_typed( vd_t dst, tp_t src_type, vc_t src )
{
    if( dst == src ) return;
    const bcore_instance_s* o = bcore_instance_s_get_aware( dst );
    if( o->o_type == src_type )
    {
        o->copy( o, dst, src );
    }
    else
    {
        o->copy_typed( o, dst, src_type, src );
    }
}

void bcore_instance_spect_move( const bcore_instance_s* o, vd_t dst, vd_t src )
{
    if( dst == src ) return;
    o->move( o, dst, src );
}

void bcore_instance_typed_move( tp_t type, vd_t dst, vd_t src )
{
    if( dst == src ) return;
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    o->move( o, dst, src );
}

void bcore_instance_aware_move( vd_t dst, vd_t src )
{
    if( dst == src ) return;
    const bcore_instance_s* o = bcore_instance_s_get_aware( src );
    o->move( o, dst, src );
}

vd_t bcore_instance_spect_create( const bcore_instance_s* o )
{
    return o->create( o );
}

vd_t bcore_instance_typed_create( tp_t type )
{
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    return o->create( o );
}

void bcore_instance_spect_discard( const bcore_instance_s* o, vd_t obj )
{
    if( !obj ) return;
    o->discard( o, obj );
}

void bcore_instance_typed_discard( tp_t type, vd_t obj )
{
    if( !obj ) return;
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    o->discard( o, obj );
}

void bcore_instance_aware_discard( vd_t obj )
{
    if( !obj ) return;
    const bcore_instance_s* o = bcore_instance_s_get_aware( obj  );
    o->discard( o, obj );
}

vd_t bcore_instance_spect_clone( const bcore_instance_s* o, vc_t obj )
{
    if( !obj ) return NULL;
    return o->clone( o, obj );
}

vd_t bcore_instance_typed_clone( tp_t type, vc_t obj )
{
    if( !obj ) return NULL;
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    return o->clone( o, obj );
}

vd_t bcore_instance_aware_clone( vc_t obj )
{
    if( !obj ) return NULL;
    const bcore_instance_s* o = bcore_instance_s_get_aware( obj  );
    return o->clone( o, obj );
}

void bcore_instance_spect_check_sanity( const bcore_instance_s* o, vc_t obj )
{
    if( !obj ) return;
    o->check_sanity( o, obj );
}

void bcore_instance_typed_check_sanity( tp_t type, vc_t obj )
{
    if( !obj ) return;
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    o->check_sanity( o, obj );
}

void bcore_instance_aware_check_sanity( vc_t obj )
{
    if( !obj ) return;
    const bcore_instance_s* o = bcore_instance_s_get_aware( obj  );
    o->check_sanity( o, obj );
}

void bcore_instance_spect_check_sizeof( const bcore_instance_s* o, sz_t size )
{
    if( o->size != size )
    {
        ERR( "Size mismatch for object '%s': Instance has measured %zu but sizeof( object ) is %zu bytes.\n"
             "This error might be caused due to a difference between object's c-definitions and its self reflection.\n" , nameof( o->o_type ), o->size, size );
    }
}

void bcore_instance_typed_check_sizeof( tp_t type, sz_t size )
{
    const bcore_instance_s* o = bcore_instance_s_get_typed( type );
    bcore_instance_spect_check_sizeof( o, size );
}

/**********************************************************************************************************************/

bcore_string_s* bcore_instance_perspective_selftest()
{
    typedef struct { aware_t _; bcore_typed_link_array_s string_arr; u2_t val1; u3_t val2; s1_t val3; } test_object1_s;

    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " test_object1_s = { aware_t _; typed * [] string_arr; u2_t val1; u3_t val2; s1_t val3; }", 0 ) );

    typedef struct { aware_t _; u2_t val1; test_object1_s* o1; } test_object2_s;
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " test_object2_s = { aware_t _; u2_t val1; test_object1_s* o1; }", 0 ) );

    test_object1_s* o1 = bcore_instance_typed_create( typeof( "test_object1_s" ) );

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
    test_object2_s* o2 = bcore_instance_typed_create( typeof( "test_object2_s" ) );
    o2->o1 = bcore_instance_aware_clone( o1 );

    test_object2_s* o3 = bcore_instance_aware_clone( o2 );

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

    bcore_instance_aware_discard( o1 );
    bcore_instance_aware_discard( o2 );
    bcore_instance_aware_discard( o3 );

    return NULL;
}

