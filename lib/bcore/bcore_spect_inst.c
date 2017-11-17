/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_spect_inst.h"
#include "bcore_quicktypes.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_spect_compare.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_interpreter.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT( bcore_inst_item_s )
DEFINE_FUNCTION_DOWN_FLAT( bcore_inst_item_s )
DEFINE_FUNCTION_COPY_FLAT( bcore_inst_item_s )
DEFINE_FUNCTION_MOVE(      bcore_inst_item_s )
DEFINE_FUNCTION_CREATE(    bcore_inst_item_s )
DEFINE_FUNCTION_DISCARD(   bcore_inst_item_s )
DEFINE_FUNCTION_CLONE(     bcore_inst_item_s )

void bcore_inst_item_s_set_size_align( bcore_inst_item_s* o )
{
    if( o->flect_item->caps == BCORE_CAPS_STATIC )
    {
        const bcore_inst_s* inst = bcore_inst_s_get_typed( o->flect_item->type );
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

DEFINE_FUNCTION_INIT_FLAT( bcore_inst_body_s )

void bcore_inst_body_s_down( bcore_inst_body_s* o )
{
    bcore_release_obj_arr( ( fp_t )bcore_inst_item_s_down, o->data, o->size, sizeof( bcore_inst_item_s ) );
    o->data = NULL;
    o->size = o->space = 0;
}

void bcore_inst_body_s_copy( bcore_inst_body_s* o, const bcore_inst_body_s* src )
{
    bcore_release_obj_arr( ( fp_t )bcore_inst_item_s_down, o->data, o->size, sizeof( bcore_inst_item_s ) );
    o->data = NULL;
    o->size = o->space = 0;
    o->data = bcore_un_alloc( sizeof( bcore_inst_item_s ), o->data, o->space, src->size, &o->space );
    for( sz_t i = 0; i < src->size; i++ ) bcore_inst_item_s_copy( &o->data[ i ], &src->data[ i ] );
    o->size = src->size;
}

DEFINE_FUNCTION_CREATE(    bcore_inst_body_s )
DEFINE_FUNCTION_DISCARD(   bcore_inst_body_s )
DEFINE_FUNCTION_CLONE(     bcore_inst_body_s )

bcore_inst_item_s* bcore_inst_body_s_push( bcore_inst_body_s* o )
{
    if( o->size == o->space )
    {
        bcore_inst_item_s* old_data = o->data;
        o->data = bcore_u_alloc( sizeof( bcore_inst_item_s ), NULL, o->space > 0 ? o->space * 2 : 1, &o->space );
        for( sz_t i = 0; i < o->size; i++  )
        {
            bcore_inst_item_s_init( &o->data[ i ] );
            bcore_inst_item_s_copy( &o->data[ i ], &old_data[ i ] );
        }
        bcore_release_obj_arr( ( fp_t )bcore_inst_item_s_down, old_data, o->size, sizeof( bcore_inst_item_s ) );
    }
    bcore_inst_item_s* item = &o->data[ o->size ];
    o->size++;
    bcore_inst_item_s_init( item );
    return item;
}

void bcore_inst_body_s_pop( bcore_inst_body_s* o )
{
    if( o->size == 0 ) return;
    o->size--;
    bcore_inst_item_s_down( &o->data[ o->size ] );
}

bcore_inst_item_s* bcore_inst_body_s_inst_item_of_flect_item( const bcore_inst_body_s* o, const bcore_flect_item_s* item )
{
    for( sz_t i = 0; i < o->size; i++ ) if( o->data[ i ].flect_item == item ) return &o->data[ i ];
    return NULL;
}

/**********************************************************************************************************************/
// bcore_inst_s

static void inst_s_down( bcore_inst_s* o );

void inst_s_init( bcore_inst_s* o )
{
    bcore_memzero( o, sizeof( bcore_inst_s ) );
    o->p_type = typeof( "bcore_inst_s" );
}

void inst_s_down( bcore_inst_s* o )
{
    bcore_inst_body_s_discard( o->body );
}

static bcore_inst_s* inst_s_create()
{
    bcore_inst_s* o = bcore_alloc( NULL, sizeof( bcore_inst_s ) );
    inst_s_init( o );
    return o;
}

static void inst_s_discard( bcore_inst_s* o )
{
    if( !o ) return;
    bcore_release_obj( ( fp_t )inst_s_down, o );
}

static bcore_inst_item_s* inst_s_push( bcore_inst_s* o )
{
    if( !o->body ) o->body = bcore_inst_body_s_create();
    return bcore_inst_body_s_push( o->body );
}

#ifdef RTCHECKS
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
#endif // RTCHECKS

/**********************************************************************************************************************/

static void init_o( const bcore_inst_s* p, vd_t o )
{
    assert( o != NULL );
    ( ( bcore_fp_init )p->init_o )( o );
}

static void init_null( const bcore_inst_s* p, vd_t o ) { }

static void init_flat( const bcore_inst_s* p, vd_t o )
{
    assert( o != NULL );
    bcore_memzero( o, p->size );
}

static void init_generic( const bcore_inst_s* p, vd_t o )
{
    assert( o != NULL );
    bcore_memzero( o, p->size );
    sz_t first = 0;
    if( p->aware )
    {
        *( aware_t* )o = p->o_type;
        first = 1;
    }
    for( sz_t i = first; i < p->body->size; i++ )
    {
        const bcore_inst_item_s*  inst_item = &p->body->data[ i ];
        const bcore_flect_item_s* flect_item = inst_item->flect_item;
        vd_t dst_obj = ( u0_t* )o + inst_item->offset;

        switch( flect_item->caps )
        {
            case BCORE_CAPS_STATIC:
            {
                const bcore_inst_s* perspective = inst_item->perspective;
                if( !perspective->init_flat ) perspective->init( perspective, dst_obj );
                if( flect_item->default_u3 )
                {
                    switch( flect_item->type )
                    {
                        case TYPEOF_u0_t: *( u0_t* )dst_obj = flect_item->default_u3; break;
                        case TYPEOF_u1_t: *( u1_t* )dst_obj = flect_item->default_u3; break;
                        case TYPEOF_u2_t: *( u2_t* )dst_obj = flect_item->default_u3; break;
                        case TYPEOF_u3_t: *( u3_t* )dst_obj = flect_item->default_u3; break;
                        case TYPEOF_s0_t: *( s0_t* )dst_obj = flect_item->default_s3; break;
                        case TYPEOF_s1_t: *( s1_t* )dst_obj = flect_item->default_s3; break;
                        case TYPEOF_s2_t: *( s2_t* )dst_obj = flect_item->default_s3; break;
                        case TYPEOF_s3_t: *( s3_t* )dst_obj = flect_item->default_s3; break;
                        case TYPEOF_f2_t: *( f2_t* )dst_obj = flect_item->default_f3; break;
                        case TYPEOF_f3_t: *( f3_t* )dst_obj = flect_item->default_f3; break;
                        case TYPEOF_sz_t: *( sz_t* )dst_obj = flect_item->default_u3; break;
                        case TYPEOF_tp_t: *( tp_t* )dst_obj = flect_item->default_u3; break;
                        case TYPEOF_bl_t: *( bl_t* )dst_obj = flect_item->default_u3; break;
                        default: ERR( "Default value not supported for type '%s'", ifnameof( flect_item->type ) );
                    }
                }
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

typedef struct { ap_t ap; const bcore_inst_s* p; vd_t o; } init_nc;

static void init_amoeba( init_nc* nc )
{
    init_generic( nc->p, nc->o );
}

static void init_amoebic( const bcore_inst_s* p, vd_t o )
{
    assert( o != NULL );
    init_nc nc = { ( ap_t )init_amoeba, p, o };
    ( ( ap_t )p->init_o )( &nc );
}

/**********************************************************************************************************************/

static void down_o( const bcore_inst_s* p, vd_t o )
{
    #ifdef RTCHECKS
        assert( o != NULL );
        if( p->aware ) verify_aware_type( p->o_type, o, __func__ );
    #endif // RTCHECKS
    ( ( bcore_fp_down )p->down_o )( o );
}

static void down_null( const bcore_inst_s* p, vd_t o ) { }

static void down_flat( const bcore_inst_s* p, vd_t o )
{
    #ifdef RTCHECKS
        assert( o != NULL );
        if( p->aware ) verify_aware_type( p->o_type, o, __func__ );
    #endif // RTCHECKS
    /* nothing to do */
}

static void down_generic( const bcore_inst_s* p, vd_t o )
{
#ifdef RTCHECKS
    assert( o != NULL );
    if( p->aware ) verify_aware_type( p->o_type, o, __func__ );
#endif // RTCHECKS
    for( sz_t i = 0; i < p->body->size; i++ )
    {
        const bcore_inst_item_s* inst_item = &p->body->data[ i ];
        if( inst_item->no_trace ) continue;

        const bcore_flect_item_s* flect_item = inst_item->flect_item;
        void* item_obj = ( u0_t* )o + inst_item->offset;

        switch( flect_item->caps )
        {
            case BCORE_CAPS_STATIC:
            {
                const bcore_inst_s* perspective = inst_item->perspective;
                if( !perspective->down_flat ) perspective->down( perspective, item_obj );
            }
            break;

            case BCORE_CAPS_STATIC_LINK:
            {
                bcore_static_link_s* s = item_obj;
                if( s->link )
                {
                    const bcore_inst_s* perspective = inst_item->perspective;
                    perspective->discard( perspective, s->link );
                }
            }
            break;

            case BCORE_CAPS_TYPED_LINK:
            {
                bcore_typed_link_s* s = item_obj;
                if( s->link )
                {
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( s->type );
                    perspective->discard( perspective, s->link );
                }
            }
            break;

            case BCORE_CAPS_AWARE_LINK:
            {
                bcore_aware_link_s* s = item_obj;
                if( s->link )
                {
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( *( aware_t* )s->link );
                    perspective->discard( perspective, s->link );
                }
            }
            break;

            case BCORE_CAPS_STATIC_ARRAY:
            {
                bcore_static_array_s* s = item_obj;
                if( s->space ) // space == 0 means array does not own data
                {
                    const bcore_inst_s* perspective = inst_item->perspective;
                    if( !perspective->down_flat )
                    {
                        bcore_release_arg_arr( ( fp_t )perspective->down, perspective, s->data, s->size, perspective->size );
                    }
                    else
                    {
                        bcore_free( s->data );
                    }
                    s->data = NULL;
                    s->space = 0;
                }
            }
            break;

            case BCORE_CAPS_TYPED_ARRAY:
            {
                bcore_typed_array_s* s = item_obj;
                if( s->space ) // space == 0 means array does not own data
                {
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( s->type );
                    if( !perspective->down_flat )
                    {
                        bcore_release_arg_arr( ( fp_t )perspective->down, perspective, s->data, s->size, perspective->size );
                    }
                    else
                    {
                        bcore_free( s->data );
                    }
                    s->data = NULL;
                    s->space = 0;
                }
            }
            break;

            case BCORE_CAPS_STATIC_LINK_ARRAY:
            {
                bcore_static_link_array_s* s = item_obj;
                if( s->space ) // space == 0 means array does not own data
                {
                    const bcore_inst_s* perspective = inst_item->perspective;
                    for( sz_t i = 0; i < s->size; i++ ) perspective->discard( perspective, s->data[ i ] );
                    bcore_free( s->data );
                    s->data = NULL;
                    s->space = 0;
                }
            }
            break;

            case BCORE_CAPS_TYPED_LINK_ARRAY:
            {
                bcore_typed_link_array_s* s = item_obj;
                if( s->space ) // space == 0 means array does not own data
                {
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( s->type );
                    for( sz_t i = 0; i < s->size; i++ ) perspective->discard( perspective, s->data[ i ] );
                    bcore_free( s->data );
                    s->data = NULL;
                    s->space = 0;
                }
            }
            break;

            case BCORE_CAPS_AWARE_LINK_ARRAY:
            {
                bcore_aware_link_array_s* s = item_obj;
                if( s->space ) // space == 0 means array does not own data
                {
                    for( sz_t i = 0; i < s->size; i++ ) bcore_inst_aware_discard( s->data[ i ] );
                    bcore_free( s->data );
                    s->data = NULL;
                    s->space = 0;
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

typedef struct { ap_t ap; const bcore_inst_s* p; vd_t o; } down_nc;

static void down_amoeba( down_nc* nc )
{
    down_generic( nc->p, nc->o );
}

static void down_amoebic( const bcore_inst_s* p, vd_t o )
{
    assert( o != NULL );
    down_nc nc = { ( ap_t )down_amoeba, p, o };
    ( ( ap_t )p->down_o )( &nc );
}

/**********************************************************************************************************************/

static void copy_null( const bcore_inst_s* p, vd_t dst, vc_t src ) {}

static void copy_o( const bcore_inst_s* p, vd_t dst, vc_t src )
{
    if( dst == src ) return;
    #ifdef RTCHECKS
        if( !dst ) ERR( "dst == NULL" );
        if( !src ) ERR( "src == NULL" );
        if( p->aware )
        {
            verify_aware_type( p->o_type, dst, __func__ );
            verify_aware_type( p->o_type, src, __func__ );
        }
    #endif // RTCHECKS
    ( ( bcore_fp_copy )p->copy_o )( dst, src );
}

static void copy_flat( const bcore_inst_s* p, vd_t dst, vc_t src )
{
    if( dst == src ) return;
    #ifdef RTCHECKS
        if( !dst ) ERR( "dst == NULL" );
        if( !src ) ERR( "src == NULL" );
        if( p->aware )
        {
            verify_aware_type( p->o_type, dst, __func__ );
            verify_aware_type( p->o_type, src, __func__ );
        }
    #endif // RTCHECKS
    bcore_memcpy( dst, src, p->size );
}

static void copy_generic( const bcore_inst_s* p, vd_t dst, vc_t src )
{
    if( dst == src ) return;
    #ifdef RTCHECKS
        if( !dst ) ERR( "dst == NULL" );
        if( !src ) ERR( "src == NULL" );
        if( p->aware )
        {
            verify_aware_type( p->o_type, dst, __func__ );
            verify_aware_type( p->o_type, src, __func__ );
        }
    #endif // RTCHECKS
    for( sz_t i = 0; i < p->body->size; i++ )
    {
        const bcore_inst_item_s* inst_item = &p->body->data[ i ];
        if( inst_item->no_trace ) continue;

        const bcore_flect_item_s* flect_item = inst_item->flect_item;
        void* dst_obj = ( u0_t* )dst + inst_item->offset;
        void* src_obj = ( u0_t* )src + inst_item->offset;

        switch( flect_item->caps )
        {
            case BCORE_CAPS_STATIC:
            {
                const bcore_inst_s* perspective = inst_item->perspective;
                perspective->copy( perspective, dst_obj, src_obj );
            }
            break;

            case BCORE_CAPS_STATIC_LINK:
            {
                bcore_static_link_s* dst = dst_obj;
                bcore_static_link_s* src = src_obj;
                const bcore_inst_s* perspective = inst_item->perspective;
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
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( dst->type );
                    perspective->discard( perspective, dst->link );
                }
                if( src->link )
                {
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( src->type );
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
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( *( aware_t* )dst->link );
                    perspective->discard( perspective, dst->link );
                }
                if( src->link )
                {
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( *( aware_t* )src->link );
                    dst->link = perspective->clone( perspective, src->link );
                }
            }
            break;

            case BCORE_CAPS_STATIC_ARRAY:
            {
                bcore_static_array_s* dst = dst_obj;
                bcore_static_array_s* src = src_obj;
                const bcore_inst_s* perspective = inst_item->perspective;
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
                    if( dst->space > 0 ) // dst->space == 0 means array references external data
                    {
                        if( !perspective->down_flat )
                        {
                            bcore_release_arg_arr( ( fp_t )perspective->down, perspective, dst->data, dst->size, perspective->size );
                        }
                        else
                        {
                            bcore_free( dst->data );
                        }
                        dst->data = NULL;
                        dst->size = 0;
                        dst->space = 0;
                    }
                    dst->data = bcore_un_alloc( perspective->size, dst->data, dst->space, src->size, &dst->space );
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
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( dst->type );
                    if( !perspective->down_flat )
                    {
                        bcore_release_arg_arr( ( fp_t )perspective->down, perspective, dst->data, dst->size, perspective->size );
                    }
                    else
                    {
                        bcore_free( dst->data );
                    }
                }
                dst->data = NULL;
                dst->size = 0;
                dst->type = src->type;
                if( src->size > 0 ) // fill dst with new data
                {
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( src->type );
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
                }
            }
            break;

            case BCORE_CAPS_STATIC_LINK_ARRAY:
            {
                bcore_static_link_array_s* dst = dst_obj;
                bcore_static_link_array_s* src = src_obj;
                const bcore_inst_s* perspective = inst_item->perspective;
                if( dst->space > 0 ) // dst->space == 0 means array references external data
                {
                    for( sz_t i = 0; i < dst->size; i++ ) perspective->discard( perspective, dst->data[ i ] );
                }
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
                if( dst->space > 0 ) // dst->space == 0 means array references external data
                {
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( dst->type );
                    for( sz_t i = 0; i < dst->size; i++ ) perspective->discard( perspective, dst->data[ i ] );
                }
                if( src->size > 0 )
                {
                    const bcore_inst_s* perspective = bcore_inst_s_get_typed( src->type );
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
                if( dst->space > 0 )  // dst->space == 0 means array references external data
                {
                    for( sz_t i = 0; i < dst->size; i++ )
                    {
                        const bcore_inst_s* perspective = bcore_inst_s_get_typed( *( aware_t* )dst->data[ i ] );
                        perspective->discard( perspective, dst->data[ i ] );
                    }
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
                        const bcore_inst_s* perspective = bcore_inst_s_get_typed( *( aware_t* )src->data[ i ] );
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

typedef struct { ap_t ap; const bcore_inst_s* p; vd_t dst; vc_t src; } copy_nc;

static void copy_amoeba( copy_nc* nc )
{
    copy_generic( nc->p, nc->dst, nc->src );
}

static void copy_amoebic( const bcore_inst_s* p, vd_t dst, vc_t src )
{
    if( dst == src ) return;
    assert( dst != NULL );
    assert( src != NULL );
    copy_nc nc = { ( ap_t )copy_amoeba, p, dst, src };
    ( ( ap_t )p->copy_o )( &nc );
}

/**********************************************************************************************************************/

static void copy_typed_null( const bcore_inst_s* p, vd_t dst, tp_t type, vc_t src ) {}

static void copy_typed_o( const bcore_inst_s* p, vd_t dst, tp_t type, vc_t src )
{
    if( dst == src ) return;
    #ifdef RTCHECKS
        if( !dst ) ERR( "dst == NULL" );
        if( !src ) ERR( "src == NULL" );
        if( p->aware ) verify_aware_type( p->o_type, dst, __func__ );
    #endif // RTCHECKS
    p->copy_typed_o( dst, type, src );
}

static void copy_typed( const bcore_inst_s* p, vd_t dst, tp_t type, vc_t src )
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

/**********************************************************************************************************************/

static void move_null( const bcore_inst_s* p, vd_t dst, vd_t src ) {}

static void move_o( const bcore_inst_s* p, vd_t dst, vd_t src )
{
    if( dst == src ) return;
    p->move_o( dst, src );
}

static void move( const bcore_inst_s* p, vd_t dst, vd_t src )
{
    if( dst == src ) return;
    p->init( p, dst );
    p->copy( p, dst, src );
    p->down( p, src );
}

static vd_t create_null( const bcore_inst_s* p ) { return NULL; }

static vd_t create_o( const bcore_inst_s* p )
{
    return p->create_o();
}

static vd_t create( const bcore_inst_s* p )
{
    vd_t o = bcore_u_alloc( p->size, NULL, 1, NULL );
    p->init( p, o );
    return o;
}

static vd_t create_typed_null( const bcore_inst_s* p, tp_t t, vc_t src ) { return NULL; }

static vd_t create_typed_o( const bcore_inst_s* p, tp_t t, vc_t src )
{
    return p->create_typed_o( t, src );
}

static vd_t create_typed( const bcore_inst_s* p, tp_t t, vc_t src )
{
    vd_t o = bcore_u_alloc( p->size, NULL, 1, NULL );
    p->init( p, o );
    p->copy_typed( p, o, t, src );
    return o;
}

static void discard_null( const bcore_inst_s* p, vd_t o ) {}

static void discard_o( const bcore_inst_s* p, vd_t o )
{
    if( !o ) return;
    p->discard_o( o );
}

static void discard( const bcore_inst_s* p, vd_t o )
{
    if( !o ) return;
    bcore_release_arg( ( fp_t )p->down, p, o );
}

static vd_t clone_null( const bcore_inst_s* p, vc_t src ) { return NULL; }

static vd_t clone_o( const bcore_inst_s* p, vc_t src )
{
    if( !src ) return NULL;
    return p->clone_o( src );
}

static vd_t clone( const bcore_inst_s* p, vc_t src )
{
    if( !src ) return NULL;
    vd_t obj = p->create( p );
    p->copy( p, obj, src );
    return obj;
}

/**********************************************************************************************************************/

static sz_t aligned_offset( sz_t align, sz_t raw_offset )
{
    if( align < 2 ) return raw_offset;
    sz_t offset = raw_offset;
    if( ( offset & ( align - 1 ) ) != 0 ) offset = ( offset + align ) & ~( align - 1 );
    return offset;
}

static void inst_s_define_trait()
{
    bcore_trait_set( entypeof( "bcore_inst" ), entypeof( "root" ) );
}

bcore_inst_s* create_from_self( const bcore_flect_self_s* self )
{
    assert( self != NULL );

    bcore_inst_s* o = inst_s_create();
    o->o_type  = self->type;
    o->move_flat = true;
    o->align     = 0;

    /// amoebas
    fp_t fp_init_a    = bcore_flect_self_s_try_external_fp( self, typeof( "ap_t" ), typeof( "init" ) );
    fp_t fp_down_a    = bcore_flect_self_s_try_external_fp( self, typeof( "ap_t" ), typeof( "down" ) );
    fp_t fp_copy_a    = bcore_flect_self_s_try_external_fp( self, typeof( "ap_t" ), typeof( "copy" ) );
    o->init_o         = bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_init"         ), 0 );
    o->down_o         = bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_down"         ), 0 );
    o->copy_o         = bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_copy"         ), 0 );
    o->copy_typed_o   = ( bcore_fp_copy_typed   )bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_copy_typed"   ), 0 );
    o->create_o       = ( bcore_fp_create       )bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_create"       ), 0 );
    o->create_typed_o = ( bcore_fp_create_typed )bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_create_typed" ), 0 );
    o->discard_o      = ( bcore_fp_discard      )bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_discard"      ), 0 );
    o->clone_o        = ( bcore_fp_clone        )bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_clone"        ), 0 );

    o->init_o = fp_init_a ? fp_init_a : o->init_o;
    o->down_o = fp_down_a ? fp_down_a : o->down_o;
    o->copy_o = fp_copy_a ? fp_copy_a : o->copy_o;
    o->init_flat = ( o->init_o == NULL );
    o->down_flat = ( o->down_o == NULL );
    o->copy_flat = ( o->copy_o == NULL );

    if( self->body && self->body->size > 0 )
    {
        const bcore_flect_body_s* flect_body = self->body;
        bcore_inst_item_s* last_inst_item = NULL;

        for( sz_t i = 0; i < flect_body->size; i++ )
        {
            const bcore_flect_item_s* flect_item = &flect_body->data[ i ];

            if( flect_item->f_shell ) continue; // shells are invisible to instance (but handled in via-perspective)

            if( flect_item->caps == BCORE_CAPS_EXTERNAL_FUNC || flect_item->caps == BCORE_CAPS_EXTERNAL_DATA )
            {
                /* nothing yet or handled above */
            }
            else
            {
                bcore_inst_item_s* inst_item = inst_s_push( o );

                inst_item->flect_item = flect_item;
                if( flect_item->type )
                {
                    if( !bcore_flect_exists( flect_item->type ) )
                    {
                        ERR( "Constructing bcore_inst_s of %s:\n"
                             "Type '%s' (%"PRIu32") does not exist.", ifnameof( self->type ), ifnameof( flect_item->type ) );
                    }
                    else if( flect_item->type == self->type )
                    {
                        if( flect_item->caps == BCORE_CAPS_STATIC )
                        {
                            ERR( "Constructing bcore_inst_s of %s:\n"
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
                        inst_item->perspective = bcore_inst_s_get_typed( flect_item->type );
                    }

                    if( flect_item->type == typeof( "aware_t" ) )
                    {
                        o->init_flat = false;
                        o->aware = true;
                    }

                    if( flect_item->default_u3 != 0 ) o->init_flat = false;

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

                if( flect_item->f_private )
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

                bcore_inst_item_s_discard( last_inst_item );
                last_inst_item = bcore_inst_item_s_clone( inst_item );
            }
        }

        if( flect_body->complete && last_inst_item )
        {
            o->size = aligned_offset( o->align, last_inst_item->offset + last_inst_item->size );
            bcore_inst_item_s_discard( last_inst_item );
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

    o->init         = o->init_o       ? ( fp_init_a ? init_amoebic : init_o ) : ( o->init_flat ? ( o->size > 0 ? init_flat : init_null ) : init_generic );
    o->down         = o->down_o       ? ( fp_down_a ? down_amoebic : down_o ) : ( o->down_flat ? ( o->size > 0 ? down_flat : down_null ) : down_generic );
    o->copy         = o->copy_o       ? ( fp_copy_a ? copy_amoebic : copy_o ) : ( o->copy_flat ? ( o->size > 0 ? copy_flat : copy_null ) : copy_generic );
    o->copy_typed   = o->copy_typed_o ? copy_typed_o   : ( o->size > 0 ? copy_typed   : copy_typed_null   );
    o->move         = o->move_o       ? move_o         : ( o->size > 0 ? move         : move_null         );
    o->create       = o->create_o     ? create_o       : ( o->size > 0 ? create       : create_null       );
    o->create_typed = o->create_o     ? create_typed_o : ( o->size > 0 ? create_typed : create_typed_null );
    o->discard      = o->discard_o    ? discard_o      : ( o->size > 0 ? discard      : discard_null      );
    o->clone        = o->clone_o      ? clone_o        : ( o->size > 0 ? clone        : clone_null        );

    return o;
}

static bcore_flect_self_s* inst_s_create_self( void )
{
//    sc_t def = "bcore_inst_s = spect { aware_t p_type; tp_t o_type; ... }";
//    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_inst_s ) );

//  We need to create this reflection manually because self_s_build_parse uses it.
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( entypeof( "bcore_inst_s" ), typeof( "spect" ), sizeof( bcore_inst_s ) );
    bcore_flect_self_s_push_d( self, bcore_flect_item_s_create_plain( BCORE_CAPS_STATIC, TYPEOF_aware_t, entypeof( "p_type"  ) ) );
    bcore_flect_self_s_push_d( self, bcore_flect_item_s_create_plain( BCORE_CAPS_STATIC, TYPEOF_tp_t,    entypeof( "o_type"  ) ) );
    self->body->complete = false;

    bcore_flect_self_s_push_ns_func( self, ( fp_t )inst_s_init,             "bcore_fp_init",                   "init"         );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )inst_s_down,             "bcore_fp_down",                   "down"         );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )inst_s_create,           "bcore_fp_create",                 "create"       );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )inst_s_discard,          "bcore_fp_discard",                "discard"      );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )create_from_self,        "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bcore_inst_s* bcore_inst_s_get_typed( tp_t o_type )
{
    return bcore_spect_get_typed( typeof( "bcore_inst_s" ), o_type );
}

/**********************************************************************************************************************/

const bcore_inst_s* bcore_inst_s_get_aware( vc_t obj )
{
    return bcore_inst_s_get_typed( *( const aware_t* )obj );
}

void bcore_inst_spect_init( const bcore_inst_s* o, vd_t obj )
{
    o->init( o, obj );
}

void bcore_inst_typed_init( tp_t type, vd_t obj )
{
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    o->init( o, obj );
}

void bcore_inst_spect_down( const bcore_inst_s* o, vd_t obj )
{
    o->down( o, obj );
}

void bcore_inst_typed_down( tp_t type, vd_t obj )
{
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    o->down( o, obj );
}

void bcore_inst_aware_down( vd_t obj )
{
    const bcore_inst_s* o = bcore_inst_s_get_aware( obj  );
    o->down( o, obj );
}

void bcore_inst_spect_copy( const bcore_inst_s* o, vd_t dst, vc_t src )
{
    if( dst == src ) return;
    o->copy( o, dst, src );
}

void bcore_inst_typed_copy( tp_t type, vd_t dst, vc_t src )
{
    if( dst == src ) return;
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    o->copy( o, dst, src );
}

void bcore_inst_aware_copy( vd_t dst, vc_t src )
{
    if( dst == src ) return;
    const bcore_inst_s* o = bcore_inst_s_get_aware( dst );
    o->copy( o, dst, src );
}

void bcore_inst_spect_copy_typed( const bcore_inst_s* o, vd_t dst, tp_t src_type, vc_t src )
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

void bcore_inst_typed_copy_typed( tp_t type, vd_t dst, tp_t src_type, vc_t src )
{
    if( dst == src ) return;
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    if( o->o_type == src_type )
    {
        o->copy( o, dst, src );
    }
    else
    {
        o->copy_typed( o, dst, src_type, src );
    }
}

void bcore_inst_aware_copy_typed( vd_t dst, tp_t src_type, vc_t src )
{
    if( dst == src ) return;
    const bcore_inst_s* o = bcore_inst_s_get_aware( dst );
    if( o->o_type == src_type )
    {
        o->copy( o, dst, src );
    }
    else
    {
        o->copy_typed( o, dst, src_type, src );
    }
}

void bcore_inst_spect_move( const bcore_inst_s* o, vd_t dst, vd_t src )
{
    if( dst == src ) return;
    o->move( o, dst, src );
}

void bcore_inst_typed_move( tp_t type, vd_t dst, vd_t src )
{
    if( dst == src ) return;
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    o->move( o, dst, src );
}

void bcore_inst_aware_move( vd_t dst, vd_t src )
{
    if( dst == src ) return;
    const bcore_inst_s* o = bcore_inst_s_get_aware( src );
    o->move( o, dst, src );
}

vd_t bcore_inst_spect_create( const bcore_inst_s* o )
{
    return o->create( o );
}

vd_t bcore_inst_typed_create( tp_t type )
{
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    return o->create( o );
}

vd_t bcore_inst_spect_create_typed( const bcore_inst_s* o, tp_t otp, vc_t obj )
{
    return o->create_typed( o, otp, obj );
}

sr_s bcore_inst_typed_create_sr( tp_t type )
{
    const bcore_inst_s* p = bcore_inst_s_get_typed( type );
    return sr_psd( p, bcore_inst_spect_create( p ) );
}

vd_t bcore_inst_typed_create_typed( tp_t type, tp_t otp, vc_t obj )
{
    return bcore_inst_spect_create_typed( bcore_inst_s_get_typed( type ), otp, obj );
}

void bcore_inst_spect_discard( const bcore_inst_s* o, vd_t obj )
{
    if( !obj ) return;
    o->discard( o, obj );
}

void bcore_inst_typed_discard( tp_t type, vd_t obj )
{
    if( !obj ) return;
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    o->discard( o, obj );
}

void bcore_inst_aware_discard( vd_t obj )
{
    if( !obj ) return;
    const bcore_inst_s* o = bcore_inst_s_get_aware( obj  );
    o->discard( o, obj );
}

vd_t bcore_inst_spect_clone( const bcore_inst_s* o, vc_t obj )
{
    if( !obj ) return NULL;
    return o->clone( o, obj );
}

vd_t bcore_inst_typed_clone( tp_t type, vc_t obj )
{
    if( !obj ) return NULL;
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    return o->clone( o, obj );
}

sr_s bcore_inst_typed_clone_sr( tp_t type, vc_t obj )
{
    const bcore_inst_s* p = bcore_inst_s_get_typed( type );
    return sr_psd( p, bcore_inst_spect_clone( p, obj ) );
}

vd_t bcore_inst_aware_clone( vc_t obj )
{
    if( !obj ) return NULL;
    const bcore_inst_s* o = bcore_inst_s_get_aware( obj  );
    return o->clone( o, obj );
}

void bcore_inst_spect_check_sizeof( const bcore_inst_s* o, sz_t size )
{
    if( o->size != size )
    {
        ERR( "Size mismatch for object '%s': Instance has measured %zu but sizeof( object ) is %zu bytes.\n"
             "This error might be caused due to a difference between object's c-definitions and its self reflection.\n" , nameof( o->o_type ), o->size, size );
    }
}

void bcore_inst_typed_check_sizeof( tp_t type, sz_t size )
{
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    bcore_inst_spect_check_sizeof( o, size );
}

// only discards when o is a strong reference; does nothing otherwise
void bcore_inst_discard( sr_s o )
{
    if( o.o && sr_s_is_strong( &o ) ) bcore_inst_spect_discard( ch_spect_p( o.p, TYPEOF_bcore_inst_s ), o.o );
}

vd_t bcore_inst_clone( sr_s o )
{
    if( !o.o ) return NULL;
    o.p = ch_spect_p( o.p, TYPEOF_bcore_inst_s );
    vd_t ret = bcore_inst_spect_clone( o.p, o.o );
    sr_down( o );
    return ret;
}

sr_s bcore_inst_clone_sr( sr_s o )
{
    if( !o.o ) return sr_null();
    sr_s ret = sr_psd( o.p, bcore_inst_spect_clone( ch_spect_p( o.p, TYPEOF_bcore_inst_s ), o.o ) );
    sr_down( o );
    return ret;
}

vd_t bcore_inst_q_clone( const sr_s* o )
{
    if( !o->o ) return NULL;
    return bcore_inst_spect_clone( ch_spect_p( o->p, TYPEOF_bcore_inst_s ), o->o );
}

sr_s bcore_inst_q_clone_sr( const sr_s* o )
{
    if( !o->o ) return sr_null();
    return sr_psd( o->p, bcore_inst_spect_clone( ch_spect_p( o->p, TYPEOF_bcore_inst_s ), o->o ) );
}


/**********************************************************************************************************************/

static st_s* spect_inst_selftest( void )
{
    typedef struct { aware_t _; bcore_typed_link_array_s string_arr; u2_t val1; u3_t val2; s1_t val3; } test_object1_s;

    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " test_object1_s = { aware_t _; typed * [] string_arr; u2_t val1; u3_t val2; s1_t val3; }", 0 ) );

    typedef struct { aware_t _; u2_t val1; test_object1_s* o1; } test_object2_s;
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( " test_object2_s = { aware_t _; u2_t val1; test_object1_s* o1; }", 0 ) );

    test_object1_s* o1 = bcore_inst_typed_create( typeof( "test_object1_s" ) );

    ASSERT( bcore_strcmp( nameof( o1->_ ), "test_object1_s" ) == 0 );
    ASSERT( o1->val1 == 0 );
    ASSERT( o1->val2 == 0 );
    ASSERT( o1->val3 == 0 );

    o1->val1 = 1234;
    o1->val2 = 1000000000001;
    o1->val3 = -5;

    o1->string_arr.type = typeof( "st_s" );
    o1->string_arr.space = 20;
    o1->string_arr.data = bcore_u_alloc( sizeof( vd_t ), NULL, o1->string_arr.space, &o1->string_arr.space );
    o1->string_arr.size = 10;
    for( sz_t i = 0; i < o1->string_arr.size; i++ ) o1->string_arr.data[ i ] = st_s_createf( "String number %04i", i );
    test_object2_s* o2 = bcore_inst_typed_create( typeof( "test_object2_s" ) );
    o2->o1 = bcore_inst_aware_clone( o1 );

    test_object2_s* o3 = bcore_inst_aware_clone( o2 );

    ASSERT( bcore_strcmp( nameof( o3->_ ), "test_object2_s" ) == 0 );
    ASSERT( o3->o1->val1 == 1234 );
    ASSERT( o3->o1->val2 == 1000000000001 );
    ASSERT( o3->o1->val3 == -5 );
    for( sz_t i = 0; i < o3->o1->string_arr.size; i++ )
    {
        st_s* s = st_s_createf( "String number %04i", i );
        ASSERT( bcore_strcmp( ( ( st_s* )o3->o1->string_arr.data[ i ] )->sc, s->sc ) == 0 );
        st_s_discard( s );
    }

    bcore_inst_aware_discard( o1 );
    bcore_inst_aware_discard( o2 );
    bcore_inst_aware_discard( o3 );

    return NULL;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_spect_inst_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_spect_inst" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        inst_s_define_trait();
        bcore_flect_define_creator( typeof( "bcore_inst_s"  ), inst_s_create_self  );
    }
    else if( signal == typeof( "selftest" ) )
    {
        return spect_inst_selftest();
    }

    return NULL;
}


