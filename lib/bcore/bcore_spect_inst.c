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

#include "bcore_spect_inst.h"
#include "bcore_signal.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_spect_compare.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_interpreter.h"
#include "bcore_tp_fastmap.h"

/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTION_INIT_FLAT( bcore_inst_item_s )
BCORE_DEFINE_FUNCTION_DOWN_FLAT( bcore_inst_item_s )
BCORE_DEFINE_FUNCTION_COPY_FLAT( bcore_inst_item_s )
BCORE_DEFINE_FUNCTION_MOVE(      bcore_inst_item_s )
BCORE_DEFINE_FUNCTION_CREATE(    bcore_inst_item_s )
BCORE_DEFINE_FUNCTION_DISCARD(   bcore_inst_item_s )
BCORE_DEFINE_FUNCTION_CLONE(     bcore_inst_item_s )

/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTION_INIT_FLAT( bcore_inst_body_s )

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
    for( uz_t i = 0; i < src->size; i++ ) bcore_inst_item_s_copy( &o->data[ i ], &src->data[ i ] );
    o->size = src->size;
}

BCORE_DEFINE_FUNCTION_CREATE(    bcore_inst_body_s )
BCORE_DEFINE_FUNCTION_DISCARD(   bcore_inst_body_s )
BCORE_DEFINE_FUNCTION_CLONE(     bcore_inst_body_s )

bcore_inst_item_s* bcore_inst_body_s_push( bcore_inst_body_s* o )
{
    if( o->size == o->space )
    {
        bcore_inst_item_s* old_data = o->data;
        o->data = bcore_u_alloc( sizeof( bcore_inst_item_s ), NULL, o->space > 0 ? o->space * 2 : 1, &o->space );
        for( uz_t i = 0; i < o->size; i++  )
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

const bcore_inst_item_s* bcore_inst_body_s_get_item_from_self_item( const bcore_inst_body_s* o, const bcore_self_item_s* item )
{
    for( uz_t i = 0; i < o->size; i++ ) if( o->data[ i ].self_item == item ) return &o->data[ i ];
    return NULL;
}

/**********************************************************************************************************************/
// bcore_inst_s

static void inst_s_down( bcore_inst_s* o );

void inst_s_init( bcore_inst_s* o )
{
    bcore_memzero( o, sizeof( bcore_inst_s ) );
    o->header.p_type = typeof( "bcore_inst_s" );
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

void bcore_inst_s_discard( bcore_inst_s* o )
{
    if( !o ) return;
    bcore_release_obj( ( fp_t )inst_s_down, o );
}

uz_t bcore_inst_s_get_items_size( const bcore_inst_s* o )
{
    return o->body ? o->body->size : 0;
}

const bcore_inst_item_s* bcore_inst_s_get_item( const bcore_inst_s* o, uz_t index )
{
    return o->body ? index < o->body->size ? &o->body->data[ index ] : NULL : NULL;
}

const bcore_inst_item_s* bcore_inst_s_get_item_from_self_item( const bcore_inst_s* o, const bcore_self_item_s* item )
{
    if( !o->body ) return NULL;
    return bcore_inst_body_s_get_item_from_self_item( o->body, item );
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
    uz_t first = 0;
    if( p->aware )
    {
        *( aware_t* )o = p->header.o_type;
        first = 1;
    }
    for( uz_t i = first; i < p->body->size; i++ )
    {
        const bcore_inst_item_s* inst_item = &p->body->data[ i ];
        const bcore_self_item_s* self_item = inst_item->self_item;
        vd_t dst_obj = ( u0_t* )o + inst_item->offset;

        switch( self_item->caps )
        {
            case BCORE_CAPS_SOLID_STATIC:
            {
                const bcore_inst_s* inst_p = inst_item->inst_p;
                if( !inst_p->init_flat ) inst_p->init( inst_p, dst_obj );
                if( self_item->default_u3 )
                {
                    switch( self_item->type )
                    {
                        case TYPEOF_u0_t: *( u0_t* )dst_obj = self_item->default_u3; break;
                        case TYPEOF_u1_t: *( u1_t* )dst_obj = self_item->default_u3; break;
                        case TYPEOF_u2_t: *( u2_t* )dst_obj = self_item->default_u3; break;
                        case TYPEOF_u3_t: *( u3_t* )dst_obj = self_item->default_u3; break;
                        case TYPEOF_s0_t: *( s0_t* )dst_obj = self_item->default_s3; break;
                        case TYPEOF_s1_t: *( s1_t* )dst_obj = self_item->default_s3; break;
                        case TYPEOF_s2_t: *( s2_t* )dst_obj = self_item->default_s3; break;
                        case TYPEOF_s3_t: *( s3_t* )dst_obj = self_item->default_s3; break;
                        case TYPEOF_f2_t: *( f2_t* )dst_obj = self_item->default_f3; break;
                        case TYPEOF_f3_t: *( f3_t* )dst_obj = self_item->default_f3; break;
                        case TYPEOF_sz_t: *( sz_t* )dst_obj = self_item->default_sz; break;
                        case TYPEOF_uz_t: *( uz_t* )dst_obj = self_item->default_uz; break;
                        case TYPEOF_tp_t: *( tp_t* )dst_obj = self_item->default_tp; break;
                        case TYPEOF_bl_t: *( bl_t* )dst_obj = self_item->default_bl; break;
                        case TYPEOF_offset_t: *( offset_t* )dst_obj = self_item->default_uz; break;

                        case TYPEOF_fp_t: // expect a function registered in function_manager
                        {
                            fp_t fp = bcore_function_get( self_item->default_tp );
                            if( !fp )
                            {
                                ERR_fa( "Initializing '#<sc_t>': Default function '#<sc_t>' not found.", ifnameof( p->header.o_type ), ifnameof( self_item->default_tp ) );
                            }
                            *( fp_t* )dst_obj = fp;
                        }
                        break;

                        default:
                        {
                            if( self_item->flags.f_fp ) // expect a function registered in function_manager
                            {
                                fp_t fp = bcore_function_get( self_item->default_tp );
                                if( !fp )
                                {
                                    ERR_fa( "Initializing '#<sc_t>': Default function '#<sc_t>' not found.", ifnameof( p->header.o_type ), ifnameof( self_item->default_tp ) );
                                }
                                *( fp_t* )dst_obj = fp;
                            }
                            else
                            {
                                ERR( "Default value not supported for type '%s'", ifnameof( self_item->type ) );
                            }
                        }
                        break;

                    }
                }
            }
            break;

            // only perspectives are initialized
            case BCORE_CAPS_LINK_STATIC:
            {
                if( self_item->flags.f_spect ) *( vc_t* )dst_obj = bcore_spect_get_typed( self_item->type, p->header.o_type );
            }
            break;

            case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC:
            {
                const bcore_inst_s* inst_p = inst_item->inst_p;
                if( !inst_p->init_flat )
                {
                    uz_t arr_size = bcore_inst_item_s_array_fix_size( inst_item );
                    uz_t element_size = inst_p->size;
                    for( uz_t i = 0; i < arr_size; i++ )
                    {
                        inst_p->init( inst_p, ( u0_t* )dst_obj + element_size * i );
                    }
                }
            }
            break;

            case BCORE_CAPS_LINK_TYPED:
            case BCORE_CAPS_LINK_AWARE:
            case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
            case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
            case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
            case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
            case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:

            case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:
            case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:
            break;

            default:
            {
                ERR_fa( "Unhandled nesting '#<sc_t>'", bcore_flect_caps_e_sc( self_item->caps ) );
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
        if( p->aware ) verify_aware_type( p->header.o_type, o, __func__ );
    #endif // RTCHECKS
    ( ( bcore_fp_down )p->down_o )( o );
}

static void down_null( const bcore_inst_s* p, vd_t o ) { }

static void down_flat( const bcore_inst_s* p, vd_t o )
{
    #ifdef RTCHECKS
        assert( o != NULL );
        if( p->aware ) verify_aware_type( p->header.o_type, o, __func__ );
    #endif // RTCHECKS
    /* nothing to do */
}

static void down_generic( const bcore_inst_s* p, vd_t o )
{
#ifdef RTCHECKS
    assert( o != NULL );
    if( p->aware ) verify_aware_type( p->header.o_type, o, __func__ );
#endif // RTCHECKS
    for( uz_t i = 0; i < p->body->size; i++ )
    {
        const bcore_inst_item_s* inst_item = &p->body->data[ i ];
        if( inst_item->no_trace ) continue;

        const bcore_self_item_s* self_item = inst_item->self_item;
        vd_t item_obj = ( u0_t* )o + inst_item->offset;

        switch( self_item->caps )
        {
            case BCORE_CAPS_SOLID_STATIC:
            {
                const bcore_inst_s* inst_p = inst_item->inst_p;
                if( !inst_p->down_flat ) inst_p->down( inst_p, item_obj );
            }
            break;

            case BCORE_CAPS_LINK_STATIC:
            {
                bcore_link_static_s* s = item_obj;
                if( s->link )
                {
                    const bcore_inst_s* inst_p = inst_item->inst_p;
                    inst_p->discard( inst_p, s->link );
                }
            }
            break;

            case BCORE_CAPS_LINK_TYPED:
            {
                bcore_link_typed_s* s = item_obj;
                if( s->link ) bcore_inst_t_discard( s->type, s->link );
            }
            break;

            case BCORE_CAPS_LINK_AWARE:
            {
                bcore_link_aware_s* s = item_obj;
                if( s->link ) bcore_inst_a_discard( s->link );
            }
            break;

            case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
            {
                bcore_array_dyn_solid_static_s* s = item_obj;
                if( s->space ) // space == 0 means array does not own data
                {
                    const bcore_inst_s* inst_p = inst_item->inst_p;
                    if( !inst_p->down_flat )
                    {
                        bcore_release_arg_arr( ( fp_t )inst_p->down, inst_p, s->data, s->size, inst_p->size );
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

            case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
            {
                bcore_array_dyn_solid_typed_s* s = item_obj;
                if( s->space ) // space == 0 means array does not own data
                {
                    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( s->type );
                    if( !inst_p->down_flat )
                    {
                        bcore_release_arg_arr( ( fp_t )inst_p->down, inst_p, s->data, s->size, inst_p->size );
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

            case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
            {
                bcore_array_dyn_link_static_s* s = item_obj;
                if( s->space ) // space == 0 means array does not own data
                {
                    const bcore_inst_s* inst_p = inst_item->inst_p;
                    for( uz_t i = 0; i < s->size; i++ ) inst_p->discard( inst_p, s->data[ i ] );
                    bcore_free( s->data );
                    s->data = NULL;
                    s->space = 0;
                }
            }
            break;

            case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
            {
                bcore_array_dyn_link_typed_s* s = item_obj;
                if( s->space ) // space == 0 means array does not own data
                {
                    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( s->type );
                    for( uz_t i = 0; i < s->size; i++ ) inst_p->discard( inst_p, s->data[ i ] );
                    bcore_free( s->data );
                    s->data = NULL;
                    s->space = 0;
                }
            }
            break;

            case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
            {
                bcore_array_dyn_link_aware_s* s = item_obj;
                if( s->space ) // space == 0 means array does not own data
                {
                    for( uz_t i = 0; i < s->size; i++ ) bcore_inst_a_discard( s->data[ i ] );
                    bcore_free( s->data );
                    s->data = NULL;
                    s->space = 0;
                }
            }
            break;

            case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC:
            {
                const bcore_inst_s* inst_p = inst_item->inst_p;
                if( !inst_p->down_flat )
                {
                    uz_t arr_size = bcore_inst_item_s_array_fix_size( inst_item );
                    uz_t element_size = inst_p->size;
                    for( uz_t i = 0; i < arr_size; i++ )
                    {
                        vd_t obj = ( u0_t* )item_obj + element_size * i;
                        inst_p->down( inst_p, obj );
                    }
                }
            }
            break;

            case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:
            {
                const bcore_inst_s* inst_p = inst_item->inst_p;
                uz_t arr_size = bcore_inst_item_s_array_fix_size( inst_item );
                vd_t* arr = item_obj;
                for( uz_t i = 0; i < arr_size; i++ )
                {
                    bcore_inst_p_discard( inst_p, arr[ i ] );
                    arr[ i ] = NULL;
                }
            }
            break;

            case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:
            {
                uz_t arr_size = bcore_inst_item_s_array_fix_size( inst_item );
                vd_t* arr = item_obj;
                for( uz_t i = 0; i < arr_size; i++ )
                {
                    bcore_inst_a_discard( arr[ i ] );
                    arr[ i ] = NULL;
                }
            }
            break;

            default:
            {
                ERR_fa( "Unhandled nesting '#<sc_t>'", bcore_flect_caps_e_sc( self_item->caps ) );
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
            verify_aware_type( p->header.o_type, dst, __func__ );
            verify_aware_type( p->header.o_type, src, __func__ );
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
            verify_aware_type( p->header.o_type, dst, __func__ );
            verify_aware_type( p->header.o_type, src, __func__ );
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
            verify_aware_type( p->header.o_type, dst, __func__ );
            verify_aware_type( p->header.o_type, src, __func__ );
        }
    #endif // RTCHECKS
    for( uz_t i = 0; i < p->body->size; i++ )
    {
        const bcore_inst_item_s* inst_item = &p->body->data[ i ];
        if( inst_item->no_trace ) continue;

        const bcore_self_item_s* self_item = inst_item->self_item;
        void* dst_obj = ( u0_t* )dst + inst_item->offset;
        void* src_obj = ( u0_t* )src + inst_item->offset;

        switch( self_item->caps )
        {
            case BCORE_CAPS_SOLID_STATIC:
            {
                const bcore_inst_s* inst_p = inst_item->inst_p;
                inst_p->copy( inst_p, dst_obj, src_obj );
            }
            break;

            case BCORE_CAPS_LINK_STATIC:
            {
                bcore_link_static_s* dst = dst_obj;
                bcore_link_static_s* src = src_obj;
                if( dst->link )
                {
                    bcore_inst_p_discard( inst_item->inst_p, dst->link );
                }
                if( src->link )
                {
                    if( self_item->flags.f_deep_copy )
                    {
                        dst->link = bcore_inst_p_clone( inst_item->inst_p, src->link );
                    }
                    else
                    {
                        dst->link = bcore_fork( src->link );
                    }
                }
                else
                {
                    dst->link = NULL;
                }
            }
            break;

            case BCORE_CAPS_LINK_TYPED:
            {
                bcore_link_typed_s* dst = dst_obj;
                bcore_link_typed_s* src = src_obj;
                if( dst->link ) bcore_inst_t_discard( dst->type, dst->link );
                if( src->link )
                {
                    if( self_item->flags.f_deep_copy )
                    {
                        dst->link = bcore_inst_t_clone( src->type, src->link );
                    }
                    else
                    {
                        dst->link = bcore_fork( src->link );
                    }
                }
                else
                {
                    dst->link = NULL;
                }
                dst->type = src->type;
            }
            break;

            case BCORE_CAPS_LINK_AWARE:
            {
                bcore_link_aware_s* dst = dst_obj;
                bcore_link_aware_s* src = src_obj;
                if( dst->link ) bcore_inst_a_discard( dst->link );
                if( src->link )
                {
                    if( self_item->flags.f_deep_copy )
                    {
                        dst->link = bcore_inst_a_clone( src->link );
                    }
                    else
                    {
                        dst->link = bcore_fork( src->link );
                    }
                }
                else
                {
                    dst->link = NULL;
                }
            }
            break;

            case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
            {
                bcore_array_dyn_solid_static_s* dst = dst_obj;
                bcore_array_dyn_solid_static_s* src = src_obj;
                const bcore_inst_s* inst_p = inst_item->inst_p;
                if( inst_p->copy_flat )
                {
                    if( src->size > dst->space )
                    {
                        dst->data = bcore_un_alloc( inst_p->size, dst->data, dst->space, 0,         &dst->space );
                        dst->data = bcore_un_alloc( inst_p->size, dst->data, dst->space, src->size, &dst->space );
                    }
                    bcore_memcpy( dst->data, src->data, inst_p->size * src->size );
                    dst->size = src->size;
                }
                else
                {
                    if( dst->space > 0 ) // dst->space == 0 means array references external data
                    {
                        if( !inst_p->down_flat )
                        {
                            bcore_release_arg_arr( ( fp_t )inst_p->down, inst_p, dst->data, dst->size, inst_p->size );
                        }
                        else
                        {
                            bcore_free( dst->data );
                        }
                        dst->data = NULL;
                        dst->size = 0;
                        dst->space = 0;
                    }
                    dst->data = bcore_un_alloc( inst_p->size, dst->data, dst->space, src->size, &dst->space );
                    for( uz_t i = 0; i < src->size; i++ )
                    {
                        vd_t dst_obj = ( u0_t* )dst->data + i * inst_p->size;
                        vd_t src_obj = ( u0_t* )src->data + i * inst_p->size;
                        inst_p->init( inst_p, dst_obj );
                        inst_p->copy( inst_p, dst_obj, src_obj );
                    }
                    dst->size = src->size;
                }
            }
            break;

            case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
            {
                bcore_array_dyn_solid_typed_s* dst = dst_obj;
                bcore_array_dyn_solid_typed_s* src = src_obj;

                if( dst->space > 0 ) // deplete dst
                {
                    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( dst->type );
                    if( !inst_p->down_flat )
                    {
                        bcore_release_arg_arr( ( fp_t )inst_p->down, inst_p, dst->data, dst->size, inst_p->size );
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
                    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( src->type );
                    dst->data = bcore_un_alloc( inst_p->size, dst->data, dst->space, src->size, &dst->space );
                    if( inst_p->copy_flat )
                    {
                        bcore_memcpy( dst->data, src->data, inst_p->size * src->size );
                    }
                    else
                    {
                        for( uz_t i = 0; i < src->size; i++ )
                        {
                            vd_t dst_obj = ( u0_t* )dst->data + i * inst_p->size;
                            vc_t src_obj = ( u0_t* )src->data + i * inst_p->size;
                            inst_p->init( inst_p, dst_obj );
                            inst_p->copy( inst_p, dst_obj, src_obj );
                        }
                    }
                    dst->size = src->size;
                }
            }
            break;

            case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
            {
                bcore_array_dyn_link_static_s* dst = dst_obj;
                bcore_array_dyn_link_static_s* src = src_obj;
                const bcore_inst_s* inst_p = inst_item->inst_p;
                if( dst->space > 0 ) // dst->space == 0 means array references external data
                {
                    for( uz_t i = 0; i < dst->size; i++ ) inst_p->discard( inst_p, dst->data[ i ] );
                }
                if( src->size > dst->space )
                {
                    dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, 0,         &dst->space );
                    dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, src->size, &dst->space );
                }
                if( self_item->flags.f_deep_copy )
                {
                    for( uz_t i = 0; i < src->size; i++ ) dst->data[ i ] = inst_p->clone( inst_p, src->data[ i ] );
                }
                else
                {
                    for( uz_t i = 0; i < src->size; i++ ) dst->data[ i ] = bcore_fork( src->data[ i ] );
                }

                dst->size = src->size;
            }
            break;

            case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
            {
                bcore_array_dyn_link_typed_s* dst = dst_obj;
                bcore_array_dyn_link_typed_s* src = src_obj;
                if( dst->space > 0 ) // dst->space == 0 means array references external data
                {
                    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( dst->type );
                    for( uz_t i = 0; i < dst->size; i++ ) bcore_inst_p_discard( inst_p, dst->data[ i ] );
                }
                if( src->size > 0 )
                {
                    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( src->type );
                    if( src->size > dst->space )
                    {
                        dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, 0,         &dst->space );
                        dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, src->size, &dst->space );
                    }
                    if( self_item->flags.f_deep_copy )
                    {
                        for( uz_t i = 0; i < src->size; i++ ) dst->data[ i ] = bcore_inst_p_clone( inst_p, src->data[ i ] );
                    }
                    else
                    {
                        for( uz_t i = 0; i < src->size; i++ ) dst->data[ i ] = bcore_fork( src->data[ i ] );
                    }
                }
                dst->size = src->size;
                dst->type = src->type;
            }
            break;

            case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
            {
                bcore_array_dyn_link_aware_s* dst = dst_obj;
                bcore_array_dyn_link_aware_s* src = src_obj;
                if( dst->space > 0 )  // dst->space == 0 means array references external data
                {
                    for( uz_t i = 0; i < dst->size; i++ ) bcore_inst_a_discard( dst->data[ i ] );
                }
                if( src->size > 0 )
                {
                    if( src->size > dst->space )
                    {
                        dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, 0,         &dst->space );
                        dst->data = bcore_un_alloc( sizeof( vd_t ), dst->data, dst->space, src->size, &dst->space );
                    }
                    if( self_item->flags.f_deep_copy )
                    {
                        for( uz_t i = 0; i < src->size; i++ )
                        {
                            dst->data[ i ] = bcore_inst_a_clone( src->data[ i ] );
                        }
                    }
                    else
                    {
                        for( uz_t i = 0; i < src->size; i++ )
                        {
                            dst->data[ i ] = bcore_fork( src->data[ i ] );
                        }
                    }
                }
                dst->size = src->size;
            }
            break;

            case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC:
            {
                const bcore_inst_s* inst_p = inst_item->inst_p;
                uz_t arr_size = bcore_inst_item_s_array_fix_size( inst_item );
                uz_t element_size = inst_p->size;

                if( inst_p->copy_flat )
                {
                    bcore_memcpy( dst_obj, src_obj, element_size * arr_size );
                }
                else
                {
                    for( uz_t i = 0; i < arr_size; i++ )
                    {
                        vd_t dst = ( u0_t* )dst_obj + element_size * i;
                        vc_t src = ( u0_t* )src_obj + element_size * i;
                        bcore_inst_p_copy( inst_p, dst, src );
                    }
                }
            }
            break;

            case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:
            {
                const bcore_inst_s* inst_p = inst_item->inst_p;
                uz_t arr_size = bcore_inst_item_s_array_fix_size( inst_item );
                vd_t* dst_arr = dst_obj;
                vd_t* src_arr = src_obj;
                for( uz_t i = 0; i < arr_size; i++ )
                {
                    vd_t dst = dst_arr[ i ];
                    vd_t src = src_arr[ i ];
                    bcore_inst_p_discard( inst_p, dst );
                    if( self_item->flags.f_deep_copy )
                    {
                        dst = bcore_inst_p_clone( inst_p, src );
                    }
                    else
                    {
                        dst = bcore_fork( src );
                    }
                }
            }
            break;

            case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:
            {
                uz_t arr_size = bcore_inst_item_s_array_fix_size( inst_item );
                vd_t* dst_arr = dst_obj;
                vd_t* src_arr = src_obj;
                for( uz_t i = 0; i < arr_size; i++ )
                {
                    vd_t dst = dst_arr[ i ];
                    vd_t src = src_arr[ i ];
                    bcore_inst_a_discard( dst );
                    if( self_item->flags.f_deep_copy )
                    {
                        dst = bcore_inst_a_clone( src );
                    }
                    else
                    {
                        dst = bcore_fork( src );
                    }
                }
            }
            break;

            default:
            {
                ERR_fa( "Unhandled nesting '#<sc_t>'", bcore_flect_caps_e_sc( self_item->caps ) );
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
        if( p->aware ) verify_aware_type( p->header.o_type, dst, __func__ );
    #endif // RTCHECKS
    p->copy_typed_o( dst, type, src );
}

static void copy_typed( const bcore_inst_s* p, vd_t dst, tp_t type, vc_t src )
{
    tp_t dst_type = p->header.o_type;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
                default: break;
            }
        }
        case TYPEOF_uz_t:
        {
            uz_t* dst_l = dst;
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
                case TYPEOF_uz_t: *dst_l = *( uz_t* )src; return;
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

static uz_t aligned_offset( uz_t align, uz_t raw_offset )
{
    if( align < 2 ) return raw_offset;
    uz_t offset = raw_offset;
    if( ( offset & ( align - 1 ) ) != 0 ) offset = ( offset + align ) & ~( align - 1 );
    return offset;
}

static void inst_s_define_trait()
{
    bcore_trait_set( entypeof( "bcore_inst" ), entypeof( "root" ) );
}

bcore_inst_s* create_from_self( const bcore_self_s* self )
{
    assert( self != NULL );

    bcore_inst_s* o = inst_s_create();
    o->header.o_type  = self->type;
    o->move_flat = true;
    o->align     = 0;

    /// amoebas
    fp_t fp_init_a    = bcore_self_s_try_external_fp( self, typeof( "ap_t" ), typeof( "init" ) );
    fp_t fp_down_a    = bcore_self_s_try_external_fp( self, typeof( "ap_t" ), typeof( "down" ) );
    fp_t fp_copy_a    = bcore_self_s_try_external_fp( self, typeof( "ap_t" ), typeof( "copy" ) );
    o->init_o         = bcore_self_s_try_external_fp( self, typeof( "bcore_fp_init"         ), 0 );
    o->down_o         = bcore_self_s_try_external_fp( self, typeof( "bcore_fp_down"         ), 0 );
    o->copy_o         = bcore_self_s_try_external_fp( self, typeof( "bcore_fp_copy"         ), 0 );
    o->copy_typed_o   = ( bcore_fp_copy_typed   )bcore_self_s_try_external_fp( self, typeof( "bcore_fp_copy_typed"   ), 0 );
    o->create_o       = ( bcore_fp_create       )bcore_self_s_try_external_fp( self, typeof( "bcore_fp_create"       ), 0 );
    o->create_typed_o = ( bcore_fp_create_typed )bcore_self_s_try_external_fp( self, typeof( "bcore_fp_create_typed" ), 0 );
    o->discard_o      = ( bcore_fp_discard      )bcore_self_s_try_external_fp( self, typeof( "bcore_fp_discard"      ), 0 );
    o->clone_o        = ( bcore_fp_clone        )bcore_self_s_try_external_fp( self, typeof( "bcore_fp_clone"        ), 0 );

    o->init_o = fp_init_a ? fp_init_a : o->init_o;
    o->down_o = fp_down_a ? fp_down_a : o->down_o;
    o->copy_o = fp_copy_a ? fp_copy_a : o->copy_o;
    o->init_flat = ( o->init_o == NULL );
    o->down_flat = ( o->down_o == NULL );
    o->copy_flat = ( o->copy_o == NULL );

    bl_t body_undefined_or_complete = true;

    if( bcore_self_s_items_size( self ) > 0 )
    {
        uz_t items_size = bcore_self_s_items_size( self );
        bl_t body_complete = bcore_self_body_s_get_complete( self->body );
        body_undefined_or_complete = body_complete;
        bcore_inst_item_s* last_inst_item = NULL;

        for( uz_t i = 0; i < items_size; i++ )
        {
            bl_t first_item = ( i == 0 );
            const bcore_self_item_s* self_item = bcore_self_s_get_item( self, i );

            if( self_item->flags.f_shell                    ) continue; // shells are invisible to instance (but handled in via-inst_p)
            if( self_item->flags.f_const                    ) continue; // constants are invisible to instance
            if( self_item->caps == BCORE_CAPS_EXTERNAL_FUNC ) continue; // nothing yet or handled above

            switch( self_item->caps )
            {
                case BCORE_CAPS_LINK_STATIC:
                case BCORE_CAPS_LINK_TYPED:
                case BCORE_CAPS_LINK_AWARE:
                case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
                case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
                case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
                case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
                case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
                case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:
                case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:
                {
                    o->copy_flat = false;
                    o->down_flat = false;
                }
                break;

                default: break;
            }

            bcore_inst_item_s* inst_item = inst_s_push( o );

            inst_item->self_item = self_item;
            if( self_item->type )
            {
                if( !bcore_flect_exists( self_item->type ) )
                {
                    ERR_fa( "Constructing bcore_inst_s of #<sc_t>:\n"
                         "Type '#<sc_t>' (#<tp_t>) does not exist.", ifnameof( self->type ), ifnameof( self_item->type ), self_item->type );
                }
                else if( self_item->type == self->type )
                {
                    if( self_item->caps == BCORE_CAPS_SOLID_STATIC )
                    {
                        ERR_fa( "Constructing bcore_inst_s of #<sc_t>:\n"
                             "Type '#<sc_t>' (#<tp_t>) is static member of itself, which is inconstructible.\n"
                             "A static link might be a viable alternative.\n" , ifnameof( self->type ), ifnameof( self_item->type ), self_item->type );
                    }
                    else
                    {
                        inst_item->inst_p = o;
                    }
                }
                else
                {
                    inst_item->inst_p = bcore_inst_s_get_typed( self_item->type );
                }

                if( self_item->type == TYPEOF_aware_t || self_item->type == TYPEOF_bcore_spect_header_s )
                {
                    if( first_item )
                    {
                        o->init_flat = false;
                        o->aware = true;
                    }
                }

                /// default value specified
                {
                    if( bcore_self_item_s_has_default_value( self_item ) ) o->init_flat = false;
                }

                o->init_flat = o->init_flat & inst_item->inst_p->init_flat;
                o->copy_flat = o->copy_flat & inst_item->inst_p->copy_flat;
                o->down_flat = o->down_flat & inst_item->inst_p->down_flat;
            }

            if( self_item->caps == BCORE_CAPS_SOLID_STATIC )
            {
                inst_item->size  = inst_item->inst_p->size;
                inst_item->align = inst_item->inst_p->align;
            }
            else if( self_item->caps == BCORE_CAPS_ARRAY_FIX_SOLID_STATIC )
            {
                inst_item->size  = inst_item->inst_p->size * self_item->array_fix_size;
                inst_item->align = inst_item->inst_p->align;
            }
            else
            {
                inst_item->size  = bcore_self_item_s_inst_size( self_item );
                inst_item->align = bcore_self_item_s_inst_align( self_item );
            }

            if( self_item->flags.f_private )
            {
                inst_item->no_trace = true;
                if( !self_item->flags.f_spect ) o->copy_flat = false;
            }

            if( self_item->flags.f_spect )
            {
                inst_item->no_trace = true;
                o->init_flat = false;
            }

            if( self_item->flags.f_feature )
            {
                inst_item->no_trace = true;
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

        if( body_complete && last_inst_item )
        {
            o->size = aligned_offset( o->align, last_inst_item->offset + last_inst_item->size );
        }
        else
        {
            o->size = self->size;
        }

        // If alignment is zero, the body does not contain physical data.
        // In this case alignment must be treated as if the body was not defined.
        if( o->align == 0 ) o->align = self->size;

        bcore_inst_item_s_discard( last_inst_item );
    }
    else
    {
        o->size  = self->size;
        o->align = self->size;
    }

    if( body_undefined_or_complete && ( self->size > 0 ) && ( self->size != o->size ) )
    {
        ERR( "Size mismatch: sizeof(%s):%zu differs from reflective body size:%zu.\n"
             "The object's reflection is probably out of sync with its compile-time definition.\n",
            ifnameof( self->type ), self->size, o->size );
    }

    if( !o->down_flat ) o->copy_flat = false;
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

static bcore_self_s* bcore_inst_s_create_self( void )
{
//    sc_t def = "bcore_inst_s = spect { aware_t p_type; tp_t o_type; ... }";
//    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_inst_s ) );

//  We need to create this reflection manually because self_s_build_parse uses it.
    bcore_self_s* self = bcore_self_s_create_plain( entypeof( "bcore_inst_s" ), typeof( "spect" ), sizeof( bcore_inst_s ) );
    bcore_self_s_push_d( self, bcore_self_item_s_create_plain( BCORE_CAPS_SOLID_STATIC, TYPEOF_bcore_spect_header_s, entypeof( "header"  ) ) );
    bcore_self_body_s_set_complete( self->body, false );

    bcore_self_s_push_ns_func( self, ( fp_t )inst_s_init,             "bcore_fp_init",                   "init"         );
    bcore_self_s_push_ns_func( self, ( fp_t )inst_s_down,             "bcore_fp_down",                   "down"         );
    bcore_self_s_push_ns_func( self, ( fp_t )inst_s_create,           "bcore_fp_create",                 "create"       );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_inst_s_discard,    "bcore_fp_discard",                "discard"      );
    bcore_self_s_push_ns_func( self, ( fp_t )create_from_self,        "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

/*
bcore_tp_fastmap_s bcore_inst_s_cache_g;
const bcore_inst_s* bcore_inst_s_get_typed( tp_t o_type )
{
    vc_t ret = bcore_tp_fastmap_s_get( &bcore_inst_s_cache_g, o_type );
    if( ret ) return ret;
    ret = bcore_spect_get_typed( TYPEOF_bcore_inst_s, o_type );
    bcore_tp_fastmap_s_set( &bcore_inst_s_cache_g, o_type, ret );
    return ret;
}
*/

/**********************************************************************************************************************/

vd_t bcore_inst_t_create_typed( tp_t type, tp_t otp, vc_t obj )
{
    return bcore_inst_p_create_typed( bcore_inst_s_get_typed( type ), otp, obj );
}

void bcore_inst_t_discard( tp_t type, vd_t obj )
{
    if( !obj ) return;
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    o->discard( o, obj );
}

void bcore_inst_a_discard( vd_t obj )
{
    if( !obj ) return;
    const bcore_inst_s* o = bcore_inst_s_get_aware( obj );
    o->discard( o, obj );
}

sr_s bcore_inst_t_clone_sr( tp_t type, vc_t obj )
{
    const bcore_inst_s* p = bcore_inst_s_get_typed( type );
    return sr_psd( p, bcore_inst_p_clone( p, obj ) );
}

void bcore_inst_p_check_sizeof( const bcore_inst_s* o, uz_t size )
{
    if( o->size != size )
    {
        ERR( "Size mismatch for object '%s': Instance has measured %zu but sizeof( object ) is %zu bytes.\n"
             "This error might be caused due to a difference between object's c-definitions and its self reflection.\n" , nameof( o->header.o_type ), o->size, size );
    }
}

void bcore_inst_t_check_sizeof( tp_t type, uz_t size )
{
    const bcore_inst_s* o = bcore_inst_s_get_typed( type );
    bcore_inst_p_check_sizeof( o, size );
}

// only discards when o is a strong reference; does nothing otherwise
void bcore_inst_x_discard( sr_s o )
{
    if( o.o && sr_s_is_strong( &o ) ) bcore_inst_p_discard( ch_spect_p( o.p, TYPEOF_bcore_inst_s ), o.o );
}

sr_s bcore_inst_x_clone_sr( sr_s o )
{
    if( !o.o ) return sr_null();
    sr_s ret = sr_psd( o.p, bcore_inst_p_clone( ch_spect_p( o.p, TYPEOF_bcore_inst_s ), o.o ) );
    sr_down( o );
    return ret;
}

sr_s bcore_inst_r_clone_sr( const sr_s* o )
{
    if( !o->o ) return sr_null();
    return sr_psd( o->p, bcore_inst_p_clone( ch_spect_p( o->p, TYPEOF_bcore_inst_s ), o->o ) );
}


/**********************************************************************************************************************/

static st_s* spect_inst_selftest( void )
{
    typedef struct { aware_t _; bcore_array_dyn_link_typed_s string_arr; u2_t val1; u3_t val2; s1_t val3; } test_object1_s;

    bcore_flect_define_self_d( bcore_self_s_build_parse_sc( " test_object1_s = { aware_t _; typed * [] string_arr; u2_t val1; u3_t val2; s1_t val3; }", 0 ) );

    typedef struct { aware_t _; u2_t val1; test_object1_s* o1; } test_object2_s;
    bcore_flect_define_self_d( bcore_self_s_build_parse_sc( " test_object2_s = { aware_t _; u2_t val1; test_object1_s* o1; }", 0 ) );

    test_object1_s* o1 = bcore_inst_t_create( typeof( "test_object1_s" ) );

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
    for( uz_t i = 0; i < o1->string_arr.size; i++ ) o1->string_arr.data[ i ] = st_s_createf( "String number %04i", i );
    test_object2_s* o2 = bcore_inst_t_create( typeof( "test_object2_s" ) );
    o2->o1 = bcore_inst_a_clone( (bcore_inst*)o1 );

    test_object2_s* o3 = bcore_inst_a_clone( (bcore_inst*)o2 );

    ASSERT( bcore_strcmp( nameof( o3->_ ), "test_object2_s" ) == 0 );
    ASSERT( o3->o1->val1 == 1234 );
    ASSERT( o3->o1->val2 == 1000000000001 );
    ASSERT( o3->o1->val3 == -5 );
    for( uz_t i = 0; i < o3->o1->string_arr.size; i++ )
    {
        st_s* s = st_s_createf( "String number %04i", i );
        ASSERT( bcore_strcmp( ( ( st_s* )o3->o1->string_arr.data[ i ] )->sc, s->sc ) == 0 );
        st_s_discard( s );
    }

    bcore_inst_a_discard( o1 );
    bcore_inst_a_discard( o2 );
    bcore_inst_a_discard( o3 );

    // deep vs shallow links
    {
        typedef struct { aware_t _; st_s * str; } deep_object_s;
        bcore_flect_define_self_d( bcore_self_s_build_parse_sc( " deep_object_s = { aware_t _; st_s => str; }", sizeof( deep_object_s ) ) );
        typedef struct { aware_t _; st_s * str; } shallow_object_s;
        bcore_flect_define_self_d( bcore_self_s_build_parse_sc( " shallow_object_s = { aware_t _; st_s -> str; }", sizeof( shallow_object_s ) ) );

        deep_object_s*    do1 = bcore_inst_t_create( typeof( "deep_object_s" ) );
        shallow_object_s* so1 = bcore_inst_t_create( typeof( "shallow_object_s" ) );

        do1->str = st_s_create_sc( "the" );
        so1->str = st_s_create_sc( "quick" );

        ASSERT( st_s_equal_sc( do1->str, "the") );
        ASSERT( st_s_equal_sc( so1->str, "quick") );

        deep_object_s*    do2 = bcore_inst_a_clone( (bcore_inst*)do1 );
        shallow_object_s* so2 = bcore_inst_a_clone( (bcore_inst*)so1 );

        do2->str = st_s_replace_sc_sc( do2->str, "the", "brown" );
        so2->str = st_s_replace_sc_sc( so2->str, "quick", "fox" );

        ASSERT( st_s_equal_sc( do1->str, "the") );
        ASSERT( st_s_equal_sc( do2->str, "brown") );

        ASSERT( st_s_equal_sc( so2->str, "fox") );
        ASSERT( st_s_equal_sc( so1->str, "fox") );

        bcore_inst_a_discard( do1 );
        bcore_inst_a_discard( do2 );
        bcore_inst_a_discard( so1 );
        bcore_inst_a_discard( so2 );
    }

    // just links
    {
        typedef struct { st_s* str1; st_s* str2; } links_object_s;
        bcore_flect_define_self_d( bcore_self_s_build_parse_sc( "links_object_s = { st_s => str1; st_s => str2; }", sizeof( links_object_s ) ) );
        links_object_s* o = bcore_inst_t_create( typeof( "links_object_s" ) );
        o->str1 = st_s_create_sc( "hello " );
        o->str2 = st_s_create_sc( "world!" );
        bcore_inst_t_discard( typeof( "links_object_s" ), o );
    }

    // fixed size array
    {
        typedef struct { st_s str_arr[ 10 ]; } arr_object_s;
        bcore_flect_define_self_d( bcore_self_s_build_parse_sc( "arr_object_s = { st_s [ 10 ] str_arr; }", sizeof( arr_object_s ) ) );
        arr_object_s* o1 = bcore_inst_t_create( typeof( "arr_object_s" ) );
        arr_object_s* o2 = bcore_inst_t_create( typeof( "arr_object_s" ) );

        for( uz_t i = 0; i < 10; i++ ) st_s_copy_fa( &o1->str_arr[ i ], "#<uz_t>", i );

        bcore_inst_t_copy( typeof( "arr_object_s" ), (bcore_inst*)o2, o1 );

        for( uz_t i = 0; i < 10; i++ )
        {
            st_s* ref = st_s_create_fa( "#<uz_t>", i );
            ASSERT( st_s_equal_st( &o2->str_arr[ i ], ref ) );
            st_s_discard( ref );
        }

        bcore_inst_t_discard( typeof( "arr_object_s" ), o1 );
        bcore_inst_t_discard( typeof( "arr_object_s" ), o2 );
    }

    return NULL;
}

/**********************************************************************************************************************/
// signal

BCORE_DEFINE_SPECT_CACHE( bcore_inst_s );
vd_t bcore_spect_inst_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_inst" ) ) )
    {
        case TYPEOF_init1:
        {
            inst_s_define_trait();
            bcore_flect_define_creator( typeof( "bcore_inst_s" ), bcore_inst_s_create_self );
            bcore_spect_setup_cache( &bcore_inst_s_cache_g );
        }
        break;

        case TYPEOF_selftest:
        {
            return spect_inst_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

