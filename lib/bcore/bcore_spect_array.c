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

#include "bcore_spect_array.h"
#include "bcore_spect_compare.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_signal.h"

#define NPX( name ) bcore_array_##name

/**********************************************************************************************************************/
// bcore_array_s

static void array_s_down( bcore_array_s* o );

static void array_s_init( bcore_array_s* o )
{
    bcore_memzero( o, sizeof( bcore_array_s ) );
    o->p_type = TYPEOF_bcore_array_s;
}

static void array_s_down( bcore_array_s* o )
{
}

static bcore_array_s* array_s_create()
{
    bcore_array_s* o = bcore_alloc( NULL, sizeof( bcore_array_s ) );
    array_s_init( o );
    return o;
}

static void array_s_discard( bcore_array_s* o )
{
    if( !o ) return;
    bcore_release_obj( ( fp_t )array_s_down, o );
}

static inline vd_t obj_vd( const bcore_array_s* p, vd_t o )
{
    return ( u0_t* )o + p->caps_offset;
}

static inline vc_t obj_vc( const bcore_array_s* p, vc_t o )
{
    return ( const u0_t* )o + p->caps_offset;
}

static inline const bcore_array_dyn_head_s* dyn_head_vc( const bcore_array_s* p, vc_t o )
{
    return obj_vc( p, o );
}

static inline bcore_array_dyn_head_s* dyn_head_vd( const bcore_array_s* p, vd_t o )
{
    return obj_vd( p, o );
}

/**********************************************************************************************************************/

static sz_t get_dyn_size( const bcore_array_s* p, vc_t o )
{
    return dyn_head_vc( p, o )->size;
}

static sz_t get_dyn_space( const bcore_array_s* p, vc_t o )
{
    return dyn_head_vc( p, o )->space;
}

static sz_t get_size( const bcore_array_s* p, vc_t o )
{
    return p->size_fix > 0 ? p->size_fix : get_dyn_size( p, o );
}

static sz_t get_space( const bcore_array_s* p, vc_t o )
{
    return p->size_fix > 0 ? p->size_fix : get_dyn_space( p, o );
}

/**********************************************************************************************************************/

void bcore_array_spect_make_strong( const bcore_array_s* p, vd_t o )
{
    if( p->size_fix > 0 ) return;
    vd_t obj = ( u0_t* )o + p->caps_offset;
    if( ( ( bcore_array_dyn_head_s* )obj )->size <= ( ( bcore_array_dyn_head_s* )obj )->space ) return;
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
        {
            bcore_array_dyn_solid_static_s* arr = obj;
            const bcore_inst_s* instance_p = p->item_p;
            sz_t unit_size = instance_p->size;
            vc_t src_data = arr->data;
            arr->data = bcore_u_alloc( unit_size, NULL, arr->size, &arr->space );
            if( instance_p->move_flat )
            {
                bcore_u_memcpy( unit_size, arr->data, src_data, arr->size );
            }
            else
            {
                vd_t dst_data = arr->data;
                for( sz_t i = 0; i < arr->size; i++ )
                {
                    instance_p->copy( instance_p, dst_data, src_data );
                    dst_data = ( u0_t* )dst_data + unit_size;
                    src_data = ( u0_t* )src_data + unit_size;
                }
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
        {
            bcore_array_dyn_solid_typed_s* arr = obj;
            if( !arr->type ) ERR( "attempt to take ownership of type-zero array" );
            const bcore_inst_s* instance_p = bcore_inst_s_get_typed( arr->type );
            sz_t unit_size = instance_p->size;
            vc_t src_data = arr->data;
            arr->data = bcore_u_alloc( unit_size, NULL, arr->size, &arr->space );
            if( instance_p->move_flat )
            {
                bcore_u_memcpy( unit_size, arr->data, src_data, arr->size );
            }
            else
            {
                vd_t dst_data = arr->data;
                for( sz_t i = 0; i < arr->size; i++ )
                {
                    instance_p->copy( instance_p, dst_data, src_data );
                    dst_data = ( u0_t* )dst_data + unit_size;
                    src_data = ( u0_t* )src_data + unit_size;
                }
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
        {
            bcore_array_dyn_link_static_s* arr = obj;
            const bcore_inst_s* instance_p = p->item_p;
            vd_t* src_data = arr->data;
            arr->data = bcore_u_alloc( sizeof( vd_t ), NULL, arr->size, &arr->space );
            vd_t* dst_data = arr->data;
            for( sz_t i = 0; i < arr->size; i++ ) *dst_data++ = instance_p->clone( instance_p, *src_data++ );
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
        {
            bcore_array_dyn_link_typed_s* arr = obj;
            if( !arr->type ) ERR( "attempt to take ownership of type-zero array" );
            const bcore_inst_s* instance_p = bcore_inst_s_get_typed( arr->type );
            vd_t* src_data = arr->data;
            arr->data = bcore_u_alloc( sizeof( vd_t ), NULL, arr->size, &arr->space );
            vd_t* dst_data = arr->data;
            for( sz_t i = 0; i < arr->size; i++ ) *dst_data++ = instance_p->clone( instance_p, *src_data++ );
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
        {
            bcore_array_dyn_link_aware_s* arr = obj;
            vd_t* src_data = arr->data;
            arr->data = bcore_u_alloc( sizeof( vd_t ), NULL, arr->size, &arr->space );
            vd_t* dst_data = arr->data;
            for( sz_t i = 0; i < arr->size; i++ ) *dst_data++ = bcore_inst_aware_clone( *src_data++ );
        }
        break;

        default: ERR( "invalid type_caps (%"PRIu32")", ( u2_t )p->type_caps );
    }
}

void bcore_array_spect_set_space( const bcore_array_s* p, vd_t o, sz_t space )
{
    if( p->size_fix > 0 )
    {
        if( p->size_fix == space ) return;
        ERR( "Cannot change space for fixed-size-array" );
    }
    vd_t obj = ( u0_t* )o + p->caps_offset;
    if( ( ( bcore_array_dyn_head_s* )obj )->size > ( ( bcore_array_dyn_head_s* )obj )->space ) bcore_array_spect_make_strong( p, o );
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
        {
            bcore_array_dyn_solid_static_s* arr = obj;
            const bcore_inst_s* instance_p = p->item_p;
            sz_t unit_size = instance_p->size;
            if( instance_p->move_flat )
            {
                arr->data = bcore_un_alloc( unit_size, arr->data, arr->space, space, &arr->space );
                arr->size = ( arr->size > space ) ? space : arr->size;
            }
            else
            {
                if( arr->size > space )
                {
                    if( instance_p->down_flat )
                    {
                        arr->size = space;
                    }
                    else
                    {
                        while( arr->size > space )
                        {
                            arr->size--;
                            instance_p->down( instance_p, ( u0_t* )arr->data + unit_size * arr->size );
                        }
                    }
                }
                vd_t old_data = arr->data;
                arr->data = bcore_un_alloc( unit_size, NULL, 0, space, &arr->space );
                for( sz_t i = 0; i < arr->size; i++ )
                {
                    vd_t dst = ( u0_t* )arr->data + unit_size * i;
                    vc_t src = ( u0_t* )old_data  + unit_size * i;
                    instance_p->init( instance_p, dst );
                    instance_p->copy( instance_p, dst, src );
                }
                bcore_release_arg_arr( ( fp_t )instance_p->down, instance_p, old_data, arr->size, unit_size );
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
        {
            bcore_array_dyn_solid_typed_s* arr = obj;
            if( space == arr->space ) break;
            if( !arr->type ) ERR( "attempt to change space on type-zero array" );
            const bcore_inst_s* instance_p = bcore_inst_s_get_typed( arr->type );
            sz_t unit_size = instance_p->size;
            if( instance_p->move_flat )
            {
                arr->data = bcore_un_alloc( unit_size, arr->data, arr->space, space, &arr->space );
                arr->size = ( arr->size > space ) ? space : arr->size;
            }
            else
            {
                if( arr->size > space )
                {
                    if( instance_p->down_flat )
                    {
                        arr->size = space;
                    }
                    else
                    {
                        while( arr->size > space )
                        {
                            arr->size--;
                            instance_p->down( instance_p, ( u0_t* )arr->data + unit_size * arr->size );
                        }
                    }
                }

                vd_t old_data = arr->data;
                arr->data = bcore_un_alloc( unit_size, NULL, 0, space, &arr->space );
                for( sz_t i = 0; i < arr->size; i++ )
                {
                    vd_t dst = ( u0_t* )arr->data + unit_size * i;
                    vc_t src = ( u0_t* )old_data  + unit_size * i;
                    instance_p->init( instance_p, dst );
                    instance_p->copy( instance_p, dst, src );
                }
                bcore_release_arg_arr( ( fp_t )instance_p->down, instance_p, old_data, arr->size, unit_size );
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
        {
            bcore_array_dyn_link_static_s* arr = obj;
            const bcore_inst_s* instance_p = p->item_p;
            while( arr->size > space )
            {
                arr->size--;
                instance_p->discard( instance_p, arr->data[ arr->size ] );
            }
            arr->data = bcore_un_alloc( sizeof( vd_t ), arr->data, arr->space, space, &arr->space );
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
        {
            bcore_array_dyn_link_typed_s* arr = obj;
            if( space < arr->size )
            {
                if( !arr->type ) ERR( "type-zero array with non-zero size detected" );
                const bcore_inst_s* instance_p = bcore_inst_s_get_typed( arr->type );
                while( arr->size > space )
                {
                    arr->size--;
                    instance_p->discard( instance_p, arr->data[ arr->size ] );
                }
            }
            arr->data = bcore_un_alloc( sizeof( vd_t ), arr->data, arr->space, space, &arr->space );
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
        {
            bcore_array_dyn_link_aware_s* arr = obj;
            while( arr->size > space )
            {
                arr->size--;
                bcore_inst_aware_discard( arr->data[ arr->size ] );
            }
            arr->data = bcore_un_alloc( sizeof( vd_t ), arr->data, arr->space, space, &arr->space );
        }
        break;

        default: ERR( "invalid type_caps (%"PRIu32")", ( u2_t )p->type_caps );
    }
}

/**********************************************************************************************************************/

void bcore_array_spect_set_size( const bcore_array_s* p, vd_t o, sz_t size )
{
    if( p->size_fix > 0 )
    {
        if( size == p->size_fix ) return;
        ERR( "Cannot change size for fixed-size-array" );
    }

    sz_t space = bcore_array_spect_get_space( p, o );

    if( size > space ) bcore_array_spect_set_space( p, o, ( size <= space * 2 ) ? space * 2 : size );

    // If array was referencing external data and size > 0, it has now copied over external data (taken ownership)
    // If size == 0, array will release external references below

    vd_t obj = ( u0_t* )o + p->caps_offset;
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
        {
            bcore_array_dyn_solid_static_s* arr = obj;
            if( arr->size > arr->space )
            {
                arr->size = 0;
                arr->data = NULL;
            }
            const bcore_inst_s* instance_p = p->item_p;
            sz_t unit_size = instance_p->size;
            if( size < arr->size )
            {
                if( instance_p->down_flat )
                {
                    arr->size = size;
                }
                else
                {
                    while( size < arr->size )
                    {
                        arr->size--;
                        instance_p->down( instance_p, ( u0_t* )arr->data + unit_size * arr->size );
                    }
                }
            }
            else if( size > arr->size )
            {
                if( instance_p->init_flat )
                {
                    bcore_memzero( ( u0_t* )arr->data + unit_size * arr->size, unit_size * ( size - arr->size ) );
                    arr->size = size;
                }
                else
                {
                    while( size > arr->size )
                    {
                        instance_p->init( instance_p, ( u0_t* )arr->data + unit_size * arr->size );
                        arr->size++;
                    }
                }
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
        {
            bcore_array_dyn_solid_typed_s* arr = obj;
            if( arr->size > arr->space )
            {
                arr->size = 0;
                arr->data = NULL;
            }
            if( size == arr->size ) break;
            if( !arr->type ) ERR( "attempt to change size on type-zero array" );
            const bcore_inst_s* instance_p = bcore_inst_s_get_typed( arr->type );
            sz_t unit_size = instance_p->size;
            if( size < arr->size )
            {
                if( instance_p->down_flat )
                {
                    arr->size = size;
                }
                else
                {
                    while( size < arr->size )
                    {
                        arr->size--;
                        instance_p->down( instance_p, ( u0_t* )arr->data + unit_size * arr->size );
                    }
                }
            }
            else if( size > arr->size )
            {
                if( instance_p->init_flat )
                {
                    bcore_memzero( ( u0_t* )arr->data + unit_size * arr->size, unit_size * ( size - arr->size ) );
                    arr->size = size;
                }
                else
                {
                    while( size > arr->size )
                    {
                        instance_p->init( instance_p, ( u0_t* )arr->data + unit_size * arr->size );
                        arr->size++;
                    }
                }
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
        {
            bcore_array_dyn_link_static_s* arr = obj;
            if( arr->size > arr->space )
            {
                arr->size = 0;
                arr->data = NULL;
            }
            const bcore_inst_s* instance_p = p->item_p;
            if( size < arr->size )
            {
                while( size < arr->size )
                {
                    arr->size--;
                    instance_p->discard( instance_p, arr->data[ arr->size ] );
                }
            }
            else if( size > arr->size )
            {
                bcore_memzero( arr->data + arr->size, sizeof( vd_t ) * ( size - arr->size ) );
                arr->size = size;
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
        {
            bcore_array_dyn_link_typed_s* arr = obj;
            if( arr->size > arr->space )
            {
                arr->size = 0;
                arr->data = NULL;
            }
            if( size < arr->size )
            {
                const bcore_inst_s* instance_p = ( arr->type ) ? bcore_inst_s_get_typed( arr->type ) : NULL;
                while( size < arr->size )
                {
                    arr->size--;
                    if( arr->data[ arr->size ] )
                    {
                        if( !instance_p ) ERR( "cannot discard element on type-zero array" );
                        instance_p->discard( instance_p, arr->data[ arr->size ] );
                    }
                }
            }
            else if( size > arr->size )
            {
                bcore_memzero( arr->data + arr->size, sizeof( vd_t ) * ( size - arr->size ) );
                arr->size = size;
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
        {
            bcore_array_dyn_link_aware_s* arr = obj;
            if( arr->size > arr->space )
            {
                arr->size = 0;
                arr->data = NULL;
            }
            if( size < arr->size )
            {
                while( size < arr->size )
                {
                    arr->size--;
                    bcore_inst_aware_discard( arr->data[ arr->size ] );
                }
            }
            else if( size > arr->size )
            {
                bcore_memzero( arr->data + arr->size, sizeof( vd_t ) * ( size - arr->size ) );
                arr->size = size;
            }
        }
        break;

        default: ERR( "invalid type_caps (%"PRIu32")", ( u2_t )p->type_caps );
    }
}

/**********************************************************************************************************************/

static sr_s get_dyn_solid_static( const bcore_array_s* p, vc_t o, sz_t index )
{
    const bcore_array_dyn_solid_static_s* arr = obj_vc( p, o );
    return ( index < arr->size ) ? sr_twd( p->item_p->o_type, ( u0_t* )arr->data + p->item_p->size * index ) : sr_null();
}

static sr_s get_dyn_solid_typed( const bcore_array_s* p, vc_t o, sz_t index )
{
    const bcore_array_dyn_solid_typed_s* arr = obj_vc( p, o );
    if( index < arr->size )
    {
        return sr_twd( arr->type, ( u0_t* )arr->data + bcore_inst_s_get_typed( arr->type )->size * index );
    }
    else
    {
        return sr_null();
    }
}

static sr_s get_dyn_link_static( const bcore_array_s* p, vc_t o, sz_t index )
{
    const bcore_array_dyn_link_static_s* arr = obj_vc( p, o );
    return ( index < arr->size ) ? sr_twd( p->item_p->o_type, arr->data[ index ] ) : sr_null();
}

static sr_s get_dyn_link_typed( const bcore_array_s* p, vc_t o, sz_t index )
{
    const bcore_array_dyn_link_typed_s* arr = obj_vc( p, o );
    if( index < arr->size && arr->data[ index ] )
    {
        return  sr_twd( arr->type, arr->data[ index ] );
    }
    else
    {
        return sr_null();
    }
}

static sr_s get_dyn_link_aware( const bcore_array_s* p, vc_t o, sz_t index )
{
    const bcore_array_dyn_link_aware_s* arr = obj_vc( p, o );
    if( index < arr->size )
    {
        vd_t item = arr->data[ index ];
        return item ? sr_twd( *( aware_t* )item, item ) : sr_null();
    }
    return sr_null();
}

static sr_s get_fix_solid_static( const bcore_array_s* p, vc_t o, sz_t index )
{
    return ( index < p->size_fix ) ? sr_twd( p->item_p->o_type, ( u0_t* )obj_vc( p, o ) + p->item_p->size * index ) : sr_null();
}

static sr_s get_fix_link_static( const bcore_array_s* p, vc_t o, sz_t index )
{
    const vd_t* arr = obj_vc( p, o );
    return ( index < p->size_fix ) ? sr_twd( p->item_p->o_type, arr[ index ] ) : sr_null();
}

static sr_s get_fix_link_aware( const bcore_array_s* p, vc_t o, sz_t index )
{
    const vd_t* arr = obj_vc( p, o );
    if( index < p->size_fix )
    {
        vd_t item = arr[ index ];
        return item ? sr_twd( *( aware_t* )item, item ) : sr_null();
    }
    return sr_null();
}

/**********************************************************************************************************************/

static void set_dyn_solid_static( const bcore_array_s* p, vd_t o, sz_t index, sr_s src )
{
    bcore_array_dyn_solid_static_s* arr = obj_vd( p, o );
    if( index >= arr->size ) bcore_array_spect_set_size( p, o, index + 1 );
    const bcore_inst_s* inst_p = p->item_p;
    vd_t dst = ( u0_t* )arr->data + inst_p->size * index;
    if( src.o )
    {
        if( sr_s_type( &src ) == inst_p->o_type )
        {
            bcore_inst_spect_copy( inst_p, dst, src.o );
        }
        else
        {
            bcore_inst_spect_copy_typed( inst_p, dst, sr_s_type( &src ), src.o );
        }
    }
    sr_down( src );
}

static void set_dyn_solid_typed( const bcore_array_s* p, vd_t o, sz_t index, sr_s src )
{
    bcore_array_dyn_solid_typed_s* arr = obj_vd( p, o );
    if( arr->type == 0 ) arr->type = sr_s_type( &src );
    if( index >= arr->size ) bcore_array_spect_set_size( p, o, index + 1 );
    if( src.o )
    {
        if( sr_s_type( &src ) == arr->type )
        {
            const bcore_inst_s* inst_p = sr_s_get_spect( TYPEOF_bcore_inst_s, &src );
            vd_t dst = ( u0_t* )arr->data + inst_p->size * index;
            bcore_inst_spect_copy( inst_p, dst, src.o );
        }
        else
        {
            const bcore_inst_s* inst_p = bcore_inst_s_get_typed( arr->type );
            vd_t dst = ( u0_t* )arr->data + inst_p->size * index;
            bcore_inst_spect_copy_typed( inst_p, dst, sr_s_type( &src ), src.o );
        }
    }
    sr_down( src );
}

static void set_dyn_link_static( const bcore_array_s* p, vd_t o, sz_t index, sr_s src )
{
    bcore_array_dyn_link_static_s* arr = obj_vd( p, o );
    if( index >= arr->size ) bcore_array_spect_set_size( p, o, index + 1 );
    const bcore_inst_s* inst_p = p->item_p;
    vd_t* dst = &arr->data[ index ];
    if( *dst ) bcore_inst_spect_discard( inst_p, *dst );
    *dst = NULL;
    if( src.o )
    {
        if( sr_s_type( &src ) == inst_p->o_type )
        {
           *dst = sr_s_is_strong( &src ) ? src.o : bcore_inst_spect_clone( inst_p, src.o );
           src = sr_cw( src );
        }
        else
        {
            *dst = bcore_inst_spect_create_typed( inst_p, sr_s_type( &src ), src.o );
        }
    }
    sr_down( src );
}

static void set_dyn_link_typed( const bcore_array_s* p, vd_t o, sz_t index, sr_s src )
{
    bcore_array_dyn_link_typed_s* arr = obj_vd( p, o );
    if( arr->type == 0 ) arr->type = sr_s_type( &src );
    if( index >= arr->size ) bcore_array_spect_set_size( p, o, index + 1 );
    vd_t* dst = &arr->data[ index ];

    if( src.o )
    {
        if( sr_s_type( &src ) == arr->type )
        {
           const bcore_inst_s* inst_p = sr_s_get_spect( TYPEOF_bcore_inst_s, &src );
           if( *dst ) bcore_inst_spect_discard( inst_p, *dst );
           *dst = sr_s_is_strong( &src ) ? src.o : bcore_inst_spect_clone( inst_p, src.o );
           src = sr_cw( src );
        }
        else
        {
            const bcore_inst_s* inst_p = bcore_inst_s_get_typed( arr->type );
            if( *dst ) bcore_inst_spect_discard( inst_p, *dst );
            *dst = bcore_inst_spect_create_typed( inst_p, sr_s_type( &src ), src.o );
        }
    }

    sr_down( src );
}

static void set_dyn_link_aware( const bcore_array_s* p, vd_t o, sz_t index, sr_s src )
{
    bcore_array_dyn_link_aware_s* arr = obj_vd( p, o );
    if( index >= arr->size ) bcore_array_spect_set_size( p, o, index + 1 );
    vd_t* dst = &arr->data[ index ];
    if( *dst ) bcore_inst_aware_discard( *dst );
    *dst = NULL;

    const bcore_inst_s* inst_p = sr_s_get_spect( TYPEOF_bcore_inst_s, &src );
    if( inst_p->aware )
    {
        *dst = sr_s_is_strong( &src ) ? src.o : bcore_inst_spect_clone( inst_p, src.o );
       src = sr_cw( src );
    }
    else
    {
        ERR( "Cannot convert '%s' to self-aware object", ifnameof( sr_s_type( &src ) ) );
    }

    sr_down( src );
}

static void set_fix_solid_static( const bcore_array_s* p, vd_t o, sz_t index, sr_s src )
{
    if( index >= p->size_fix ) ERR_fa( "Index '#<sz_t>' exceeds range of fixed-size-array of size '#<sz_t>'", index, p->size_fix );
    const bcore_inst_s* inst_p = p->item_p;
    vd_t dst = ( u0_t* )obj_vd( p, o ) + inst_p->size * index;
    if( src.o )
    {
        if( sr_s_type( &src ) == inst_p->o_type )
        {
            bcore_inst_spect_copy( inst_p, dst, src.o );
        }
        else
        {
            bcore_inst_spect_copy_typed( inst_p, dst, sr_s_type( &src ), src.o );
        }
    }
    sr_down( src );
}

static void set_fix_link_static( const bcore_array_s* p, vd_t o, sz_t index, sr_s src )
{
    if( index >= p->size_fix ) ERR_fa( "Index '#<sz_t>' exceeds range of fixed-size-array of size '#<sz_t>'", index, p->size_fix );
    const bcore_inst_s* inst_p = p->item_p;
    vd_t* arr = obj_vd( p, o );
    vd_t* dst = &arr[ index ];
    if( *dst ) bcore_inst_spect_discard( inst_p, *dst );
    *dst = NULL;
    if( src.o )
    {
        if( sr_s_type( &src ) == inst_p->o_type )
        {
           *dst = sr_s_is_strong( &src ) ? src.o : bcore_inst_spect_clone( inst_p, src.o );
           src = sr_cw( src );
        }
        else
        {
            *dst = bcore_inst_spect_create_typed( inst_p, sr_s_type( &src ), src.o );
        }
    }
    sr_down( src );
}

static void set_fix_link_aware( const bcore_array_s* p, vd_t o, sz_t index, sr_s src )
{
    if( index >= p->size_fix ) ERR_fa( "Index '#<sz_t>' exceeds range of fixed-size-array of size '#<sz_t>'", index, p->size_fix );
    vd_t* arr = obj_vd( p, o );
    vd_t* dst = &arr[ index ];
    if( *dst ) bcore_inst_aware_discard( *dst );
    *dst = NULL;

    const bcore_inst_s* inst_p = sr_s_get_spect( TYPEOF_bcore_inst_s, &src );
    if( inst_p->aware )
    {
        *dst = sr_s_is_strong( &src ) ? src.o : bcore_inst_spect_clone( inst_p, src.o );
       src = sr_cw( src );
    }
    else
    {
        ERR( "Cannot convert '%s' to self-aware object", ifnameof( sr_s_type( &src ) ) );
    }

    sr_down( src );
}

/**********************************************************************************************************************/

static inline sz_t asz( const bcore_array_s* p, vc_t o ) { return bcore_array_spect_get_size( p, o ); }

sr_s NPX(spect_get_first )( const NPX(s)* p, vc_t o           ) { return NPX(spect_get    )( p, o, 0 ); }
sr_s NPX(spect_get_last  )( const NPX(s)* p, vc_t o           ) { return NPX(spect_get    )( p, o, asz( p, o ) - 1 ); }
void NPX(spect_push      )( const NPX(s)* p, vd_t o, sr_s src ) {        NPX(spect_set    )( p, o, asz( p, o ), src ); }
void NPX(spect_push_s3   )( const NPX(s)* p, vd_t o, s3_t val ) {        NPX(spect_set_s3 )( p, o, asz( p, o ), val ); }
void NPX(spect_push_u3   )( const NPX(s)* p, vd_t o, u3_t val ) {        NPX(spect_set_u3 )( p, o, asz( p, o ), val ); }
void NPX(spect_push_f3   )( const NPX(s)* p, vd_t o, f3_t val ) {        NPX(spect_set_f3 )( p, o, asz( p, o ), val ); }
void NPX(spect_push_sz   )( const NPX(s)* p, vd_t o, sz_t val ) {        NPX(spect_set_sz )( p, o, asz( p, o ), val ); }
void NPX(spect_push_sc   )( const NPX(s)* p, vd_t o, sc_t val ) {        NPX(spect_set_sc )( p, o, asz( p, o ), val ); }
void NPX(spect_push_bl   )( const NPX(s)* p, vd_t o, bl_t val ) {        NPX(spect_set_bl )( p, o, asz( p, o ), val ); }

/**********************************************************************************************************************/

void bcore_array_spect_pop( const bcore_array_s* p, vd_t o )
{
    sz_t size = bcore_array_spect_get_size( p, o );
    if( size > 0 ) bcore_array_spect_set_size( p, o, size - 1 );
}

/**********************************************************************************************************************/

void bcore_array_spect_set_gtype( const bcore_array_s* p, vd_t o, tp_t type )
{
    vd_t obj = obj_vd( p, o );
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
        {
            if( p->item_p->o_type == type ) break;
            ERR( "cannot change type on static-array" );
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
        {
            bcore_array_dyn_solid_typed_s* arr = obj;
            if( arr->type == type ) break;
            if( arr->size > 0 ) ERR( "array size (%zu) must be zero for a type change", arr->size );
            arr->type = type;
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
        {
            if( p->item_p->o_type == type ) break;
            ERR( "cannot change type on static-link-array" );
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
        {
            bcore_array_dyn_link_typed_s* arr = obj;
            if( arr->type == type ) break;
            if( arr->size > 0 ) ERR( "array size (%zu) must be zero for a type change", arr->size );
            arr->type = type;
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
        {
            bcore_array_dyn_link_aware_s* arr = obj;
            if( arr->size > 0 ) ERR( "cannot change type on aware-link-array of non-zero size" );
        }
        break;

        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC:
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:
        {
            ERR( "cannot change type on fixed-size-array" );
        }
        break;

        default:
        {
            ERR( "invalid type_caps (%"PRIu32")", ( u2_t )p->type_caps );
        }
        break;
    }
}

/**********************************************************************************************************************/

vc_t bcore_array_spect_get_c_data( const bcore_array_s* p, vc_t o )
{
    vc_t obj = obj_vc( p, o );
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return ( ( const bcore_array_dyn_solid_static_s* )obj )->data;
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return ( ( const bcore_array_dyn_solid_typed_s*  )obj )->data;
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return ( ( const bcore_array_dyn_link_static_s*  )obj )->data;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return ( ( const bcore_array_dyn_link_typed_s*   )obj )->data;
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   return ( ( const bcore_array_dyn_link_aware_s*   )obj )->data;
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return obj;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return obj;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   return obj;
        default: ERR( "invalid type_caps (%"PRIu32")", ( u2_t )p->type_caps );
    }
    return NULL;
}

vd_t bcore_array_spect_get_d_data( const bcore_array_s* p, vd_t o )
{
    vd_t obj = obj_vd( p, o );
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return ( ( bcore_array_dyn_solid_static_s* )obj )->data;
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return ( ( bcore_array_dyn_solid_typed_s*  )obj )->data;
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return ( ( bcore_array_dyn_link_static_s*  )obj )->data;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return ( ( bcore_array_dyn_link_typed_s*   )obj )->data;
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   return ( ( bcore_array_dyn_link_aware_s*   )obj )->data;
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return obj;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return obj;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   return obj;
        default: ERR( "invalid type_caps (%"PRIu32")", ( u2_t )p->type_caps );
    }
    return NULL;
}

sz_t bcore_array_spect_get_unit_size( const bcore_array_s* p, vc_t o )
{
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:      return p->item_p->size;
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
        {
            vc_t obj = ( u0_t* )o + p->caps_offset;
            const bcore_array_dyn_solid_typed_s* arr = obj;
            if( !arr->type ) ERR( "type-zero array: cannot obtain unit size" );
            return bcore_inst_s_get_typed( arr->type )->size;
        }
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return sizeof( vd_t );
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return sizeof( vd_t );
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   return sizeof( vd_t );
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return p->item_p->size;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return sizeof( vd_t );
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   return sizeof( vd_t );

        default: ERR( "invalid type_caps (%"PRIu32")", ( u2_t )p->type_caps );
    }
    return 0;
}

/**********************************************************************************************************************/

static bl_t array_s_supports( const bcore_self_s* self, st_s* log )
{
    sz_t items_size = bcore_self_s_items_size( self );
    for( sz_t i = 0; i < items_size; i++ )
    {
        if( bcore_flect_caps_is_array( bcore_self_s_get_item( self, i )->caps ) ) return true;
    }
    if( log ) st_s_pushf( log, "Object is no array." );
    return false;
}

static void array_s_define_trait()
{
    tp_t trait = entypeof( "bcore_array" );
    bcore_trait_register_fp_support( trait, array_s_supports );
    bcore_trait_set( trait, entypeof( "bcore_inst" ) );
}

static bcore_array_s* create_from_self( const bcore_self_s* self )
{
    assert( self != NULL );
    bcore_array_s* o = array_s_create();
    o->p_type = bcore_name_enroll( "bcore_array_s" );
    o->o_type = self->type;

    const bcore_inst_s* instance = bcore_inst_s_get_typed( self->type );
    if( !instance->body ) ERR( "'%s' has no body", ifnameof( self->type ) );

    bcore_inst_body_s* body = instance->body;
    bool found = false;
    for( sz_t i = 0; i < body->size; i++ )
    {
        const bcore_self_item_s* self_item = body->data[ i ].self_item;
        if( bcore_flect_caps_is_array( self_item->caps ) )
        {
            o->type_caps = self_item->caps;
            o->caps_offset = body->data[ i ].offset;
            o->item_p = body->data[ i ].inst_p;
            o->size_fix = 0;
            if( bcore_flect_caps_is_array_fix( self_item->caps ) )
            {
                o->size_fix = self_item->array_fix_size;
                if( o->size_fix == 0 ) ERR_fa( "'#<sc_t>' represents a fixed-size array of size zero.", ifnameof( self->type ) );
            }
            found = true;
            break;
        }
    }
    if( !found ) ERR( "'%s' has no array", ifnameof( self->type ) );

    switch( o->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
        {
            o->get = get_dyn_solid_static;
            o->set = set_dyn_solid_static;
            if( !o->item_p ) ERR( "item_p is NULL on solid-static-array" );
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
        {
            o->get = get_dyn_solid_typed;
            o->set = set_dyn_solid_typed;
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
        {
            o->get = get_dyn_link_static;
            o->set = set_dyn_link_static;
            if( !o->item_p ) ERR( "item_p is NULL on link-static-array" );
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
        {
            o->get = get_dyn_link_typed;
            o->set = set_dyn_link_typed;
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
        {
            o->get = get_dyn_link_aware;
            o->set = set_dyn_link_aware;
        }
        break;

        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC:
        {
            o->get = get_fix_solid_static;
            o->set = set_fix_solid_static;
            if( !o->item_p ) ERR( "item_p is NULL on solid-static-array" );
        }
        break;

        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:
        {
            o->get = get_fix_link_static;
            o->set = set_fix_link_static;
            if( !o->item_p ) ERR( "item_p is NULL on link-static-array" );
        }
        break;

        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:
        {
            o->get = get_fix_link_aware;
            o->set = set_fix_link_aware;
        }
        break;

        default: ERR( "invalid type_caps %"PRIu32, ( u2_t )o->type_caps );
    }

    return o;
}

static bcore_self_s* array_s_create_self( void )
{
    sc_t def = "bcore_array_s = spect { aware_t p_type; tp_t o_type; ... }";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_array_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )array_s_init,             "bcore_fp_init",                    "init"         );
    bcore_self_s_push_ns_func( self, ( fp_t )array_s_down,             "bcore_fp_down",                    "down"         );
    bcore_self_s_push_ns_func( self, ( fp_t )array_s_create,           "bcore_fp_create",                  "create"       );
    bcore_self_s_push_ns_func( self, ( fp_t )array_s_discard,          "bcore_fp_discard",                 "discard"      );
    bcore_self_s_push_ns_func( self, ( fp_t )create_from_self,         "bcore_spect_fp_create_from_self",  "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

tp_t bcore_array_dyn_solid_static_type_of( tp_t type )
{
    return bcore_flect_type_self_d( bcore_self_s_create_array_dyn_solid_static( type ) );
}

tp_t bcore_array_dyn_link_static_type_of( tp_t type )
{
    return bcore_flect_type_self_d( bcore_self_s_create_array_dyn_link_static( type ) );
}

tp_t bcore_array_fix_solid_static_type_of( tp_t type, sz_t size )
{
    return bcore_flect_type_self_d( bcore_self_s_create_array_fix_solid_static( type, size ) );
}

tp_t bcore_array_fix_link_static_type_of( tp_t type, sz_t size )
{
    return bcore_flect_type_self_d( bcore_self_s_create_array_fix_link_static( type, size ) );
}

tp_t bcore_array_fix_link_aware_type_of( sz_t size )
{
    return bcore_flect_type_self_d( bcore_self_s_create_array_fix_link_aware( size ) );
}

/**********************************************************************************************************************/

sz_t bcore_array_spect_get_size   ( const bcore_array_s* p, vc_t o )                                 { return get_size( p, o ); }
sz_t bcore_array_spect_get_space  ( const bcore_array_s* p, vc_t o )                                 { return get_space( p, o ); }
sr_s bcore_array_spect_get        ( const bcore_array_s* p, vc_t o, sz_t index )                     { return p->get( p, o, index ); }
void bcore_array_spect_set        ( const bcore_array_s* p, vd_t o, sz_t index, sr_s src )           {        p->set( p, o, index, src ); }
void bcore_array_spect_set_s3     ( const bcore_array_s* p, vd_t o, sz_t index, s3_t val )           {        p->set( p, o, index, sr_twc( TYPEOF_s3_t, &val ) ); }
void bcore_array_spect_set_u3     ( const bcore_array_s* p, vd_t o, sz_t index, u3_t val )           {        p->set( p, o, index, sr_twc( TYPEOF_u3_t, &val ) ); }
void bcore_array_spect_set_f3     ( const bcore_array_s* p, vd_t o, sz_t index, f3_t val )           {        p->set( p, o, index, sr_twc( TYPEOF_f3_t, &val ) ); }
void bcore_array_spect_set_sz     ( const bcore_array_s* p, vd_t o, sz_t index, sz_t val )           {        p->set( p, o, index, sr_twc( TYPEOF_sz_t, &val ) ); }
void bcore_array_spect_set_sc     ( const bcore_array_s* p, vd_t o, sz_t index, sc_t val )           {        p->set( p, o, index, sr_twc( TYPEOF_sc_t, &val ) ); }
void bcore_array_spect_set_bl     ( const bcore_array_s* p, vd_t o, sz_t index, bl_t val )           {        p->set( p, o, index, sr_twc( TYPEOF_bl_t, &val ) ); }

bl_t bcore_array_spect_is_fixed( const bcore_array_s* p )
{
    return p->size_fix > 0;
}

bl_t bcore_array_spect_is_static( const bcore_array_s* p )
{
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return true;
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return true;
        default: break;
    }
    return false;
}

bl_t bcore_array_spect_is_mono_typed(  const bcore_array_s* p )
{
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE: return false;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE: return false;
        default: break;
    }
    return true;
}

bl_t bcore_array_spect_is_mutable_mono_typed(  const bcore_array_s* p )
{
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return true;
        default: break;
    }
    return false;
}

bl_t bcore_array_spect_is_multi_typed( const bcore_array_s* p )
{
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE: return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE: return true;
        default: break;
    }
    return false;
}

bl_t bcore_array_spect_is_of_aware( const bcore_array_s* p )
{
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE: return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE: return true;
        default: break;
    }
    return false;
}

bl_t bcore_array_spect_is_of_links( const bcore_array_s* p )
{
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   return true;
        default: break;
    }
    return false;
}

bl_t bcore_array_spect_is_weak( const bcore_array_s* p, vc_t o )
{
    vc_t obj = obj_vc( p, o );
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: { const bcore_array_dyn_solid_static_s* arr = obj; return arr->size > arr->space; }
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  { const bcore_array_dyn_solid_typed_s*  arr = obj; return arr->size > arr->space; }
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  { const bcore_array_dyn_link_static_s*  arr = obj; return arr->size > arr->space; }
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   { const bcore_array_dyn_link_typed_s*   arr = obj; return arr->size > arr->space; }
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   { const bcore_array_dyn_link_aware_s*   arr = obj; return arr->size > arr->space; }
        default: break;
    }
    return false;
}

tp_t bcore_array_spect_get_static_type( const bcore_array_s* p )
{
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return p->item_p->o_type;
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return p->item_p->o_type;
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return p->item_p->o_type;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return p->item_p->o_type;
        default: break;
    }
    return 0;
}

tp_t bcore_array_spect_get_mono_type( const bcore_array_s* p, vc_t o )
{
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return p->item_p->o_type;
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return ( ( const bcore_array_dyn_solid_typed_s* )obj_vc( p, o ) )->type;
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return p->item_p->o_type;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return ( ( const bcore_array_dyn_link_typed_s*  )obj_vc( p, o ) )->type;
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return p->item_p->o_type;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return p->item_p->o_type;
        default: break;
    }
    return 0;
}

tp_t bcore_array_spect_get_type( const bcore_array_s* p, vc_t o, sz_t index )
{
    switch( p->type_caps )
    {
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return p->item_p->o_type;
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return ( ( const bcore_array_dyn_solid_typed_s* )obj_vc( p, o ) )->type;
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return p->item_p->o_type;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return ( ( const bcore_array_dyn_link_typed_s*  )obj_vc( p, o ) )->type;
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   { sr_s sr = p->get( p, o, index ); tp_t t = sr_s_type( &sr ); sr_down( sr ); return t; }
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return p->item_p->o_type;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return p->item_p->o_type;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   { sr_s sr = p->get( p, o, index ); tp_t t = sr_s_type( &sr ); sr_down( sr ); return t; }
        default: break;
    }
    return 0;
}

static s2_t cmp_aware( vc_t o, vc_t obj1, vc_t obj2 )
{
    return bcore_compare_aware( obj1, obj2 );
}

vc_t bcore_array_spect_max_f( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, bcore_cmp_f cmp, s2_t direction )
{
    sz_t size = get_size( p, o );
    sz_t end_l = end < size ? end : size;
    if( start >= end_l ) return NULL;
    vc_t ret;

    if( bcore_array_spect_is_of_links( p ) )
    {
        vc_t* data = ( vc_t* )bcore_array_spect_get_c_data( p, o );
        sz_t  idx = start;
        for( sz_t i = start + 1; i < end_l; i++ ) idx = ( cmp.f( cmp.o, data[ idx ], data[ i ] ) * direction > 0 ) ? i : idx;
        ret = idx < end_l ? data[ idx ] : NULL;
    }
    else
    {
        sz_t unit_size = bcore_array_spect_get_unit_size( p, o );
        vc_t src = ( u0_t* )bcore_array_spect_get_c_data( p, o ) + unit_size * start;
        vc_t cur = src;
        for( sz_t i = start + 1; i < end_l; i++ )
        {
            src = ( u0_t* )src + unit_size;
            cur = ( cmp.f( cmp.o, cur, src ) * direction > 0 ) ? src : cur;
        }
        ret = cur;
    }
    return ret;
}

vc_t bcore_array_spect_max( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, s2_t direction )
{
    bl_t is_of_aware = bcore_array_spect_is_of_aware( p );
    bcore_cmp_f cmp =
    {
        .f = is_of_aware ? cmp_aware : ( bcore_fp_cmp_o )bcore_compare_spect,
        .o = is_of_aware ? NULL : bcore_compare_s_get_typed( bcore_array_spect_get_mono_type( p, o ) )
    };
    return bcore_array_spect_max_f( p, o, start, end, cmp, direction );
}

/**********************************************************************************************************************/

sz_t bcore_array_spect_max_index_f( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, bcore_cmp_f cmp, s2_t direction )
{
    sz_t size = get_size( p, o );
    sz_t end_l = end < size ? end : size;
    if( start >= end_l ) return end_l;

    sz_t ret;

    if( bcore_array_spect_is_of_links( p ) )
    {
        vc_t* data = ( vc_t* )bcore_array_spect_get_c_data( p, o );
        sz_t  idx = start;
        for( sz_t i = start + 1; i < end_l; i++ ) idx = ( cmp.f( cmp.o, data[ idx ], data[ i ] ) * direction > 0 ) ? i : idx;
        ret = idx;
    }
    else
    {
        sz_t unit_size = bcore_array_spect_get_unit_size( p, o );
        vc_t src = ( u0_t* )bcore_array_spect_get_c_data( p, o ) + unit_size * start;
        vc_t cur = src;
        sz_t idx = start;
        for( sz_t i = start + 1; i < end_l; i++ )
        {
            src = ( u0_t* )src + unit_size;
            if( cmp.f( cmp.o, cur, src ) * direction > 0 )
            {
                cur = src;
                idx = i;
            }
        }
        ret = idx;
    }
    return ret;
}

sz_t bcore_array_spect_max_index( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, s2_t direction )
{
    bl_t is_of_aware = bcore_array_spect_is_of_aware( p );
    bcore_cmp_f cmp =
    {
        .f = is_of_aware ? cmp_aware : ( bcore_fp_cmp_o )bcore_compare_spect,
        .o = is_of_aware ? NULL      : bcore_compare_s_get_typed( bcore_array_spect_get_mono_type( p, o ) )
    };
    return bcore_array_spect_max_index_f( p, o, start, end, cmp, direction );
}

/**********************************************************************************************************************/

static void buf_sort_link( vc_t* data, sz_t size, vc_t* buf, bcore_cmp_f cmp, s2_t direction )
{
    if( size < 2 ) return;
    sz_t size1 = size >> 1;
    buf_sort_link( data,         size1       , buf, cmp, direction );
    buf_sort_link( data + size1, size - size1, buf, cmp, direction );
    bcore_memcpy( buf, data, size1 * sizeof( vc_t ) );
    for( sz_t i = 0, w = 0, r = size1; i < size1; )
    {
        data[ w++ ] = ( r == size || cmp.f( cmp.o, buf[ i ], data[ r ] ) * direction >= 0 ) ? buf[ i++ ] : data[ r++ ];
    }
}

static void buf_sort_spect_empl( const bcore_inst_s* p, vd_t data, sz_t size, vd_t buf, bcore_cmp_f cmp, s2_t direction )
{
    if( size < 2 ) return;

    sz_t unit_size = p->size;
    sz_t size1 = size >> 1;
    buf_sort_spect_empl( p, data,                              size1       , buf, cmp, direction );
    buf_sort_spect_empl( p, ( u0_t* )data + size1 * unit_size, size - size1, buf, cmp, direction );

    if( p->copy_flat )
    {
        bcore_memcpy( buf, data, size1 * unit_size );
    }
    else
    {
        for( sz_t i = 0; i < size1; i++ ) p->copy( p, ( u0_t* )buf + i * unit_size, ( u0_t* )data + i * unit_size );
    }

    for( sz_t i = 0, w = 0, r = size1; i < size1; )
    {
        vc_t src1 = ( u0_t* )buf  + i * unit_size;
        vc_t src2 = ( u0_t* )data + r * unit_size;
        vd_t dst =  ( u0_t* )data + w * unit_size;
        if( r == size || cmp.f( cmp.o, src1, src2 ) * direction >= 0 )
        {
            if( p->copy_flat )
            {
                bcore_memcpy( dst, src1, unit_size );
            }
            else
            {
                p->copy( p, dst, src1 );
            }

            i++;
            w++;
        }
        else
        {
            if( p->copy_flat )
            {
                bcore_memcpy( dst, src2, unit_size );
            }
            else
            {
                p->copy( p, dst, src2 );
            }
            r++;
            w++;
        }
    }
}

void bcore_array_spect_sort_f( const bcore_array_s* p, vd_t o, sz_t start, sz_t end, bcore_cmp_f cmp, s2_t direction )
{
    sz_t size = get_size( p, o );
    if( size > get_space( p, o ) ) bcore_array_spect_make_strong( p, o );
    sz_t end_l = end < size ? end : size;
    if( start >= end_l ) return;
    sz_t range = end_l - start;

    if( bcore_array_spect_is_of_links( p ) )
    {
        vc_t* data = ( vc_t* )bcore_array_spect_get_d_data( p, o ) + start;
        vc_t* buf = bcore_un_alloc( sizeof( vd_t ), NULL, 0, range >> 1, NULL );
        buf_sort_link( data, range, buf, cmp, direction );
        bcore_un_alloc( sizeof( vd_t ), buf, range >> 1, 0, NULL );
    }
    else
    {
        const bcore_inst_s* item_p = bcore_array_spect_is_mutable_mono_typed( p ) ? bcore_inst_s_get_typed( bcore_array_spect_get_mono_type( p, o ) ) : p->item_p;
        sz_t unit_size = item_p->size;
        vd_t data = ( u0_t* )bcore_array_spect_get_d_data( p, o ) + start * unit_size;

        sz_t range1 = range >> 1;
        vd_t buf = bcore_un_alloc( unit_size, NULL, 0, range1, NULL );
        for( sz_t i = 0; i < range1; i++ ) item_p->init( item_p, ( u0_t* )buf + i * unit_size );

        buf_sort_spect_empl( item_p, data, range, buf, cmp, direction );

        for( sz_t i = 0; i < range1; i++ ) item_p->down( item_p, ( u0_t* )buf + i * unit_size );
        bcore_un_alloc( unit_size, buf, range1, 0, NULL );
    }
}

void bcore_array_spect_sort( const bcore_array_s* p, vd_t o, sz_t start, sz_t end, s2_t direction )
{
    bl_t is_of_aware = bcore_array_spect_is_of_aware( p );
    bcore_cmp_f cmp =
    {
        .f = is_of_aware ? cmp_aware : ( bcore_fp_cmp_o )bcore_compare_spect,
        .o = is_of_aware ? NULL      : bcore_compare_s_get_typed( bcore_array_spect_get_mono_type( p, o ) )
    };
    bcore_array_spect_sort_f( p, o, start, end, cmp, direction );
}

void bcore_array_spect_do( const bcore_array_s* p, vd_t o, sz_t start, sz_t end, fp_t func )
{
    sz_t size = get_size( p, o );
    sz_t end_l = end < size ? end : size;
    if( start >= end_l ) return;

    if( bcore_array_spect_is_of_links( p ) )
    {
        vd_t* data = ( vd_t* )bcore_array_spect_get_d_data( p, o );
        for( sz_t i = start; i < end_l; i++ ) ( ( void (*)( vd_t ) )func )( data[ i ] );
    }
    else
    {
        sz_t unit_size = bcore_array_spect_get_unit_size( p, o );
        vd_t ptr = ( u0_t* )bcore_array_spect_get_d_data( p, o ) + unit_size * start;
        for( sz_t i = start; i < end_l; i++ )
        {
            ( ( void (*)( vd_t ) )func )( ptr );
            ptr = ( u0_t* )ptr + unit_size;
        }
    }
}

/**********************************************************************************************************************/

static void buf_order_sort( vc_t* data, sz_t* order, sz_t size, sz_t* buf, bcore_cmp_f cmp, s2_t direction )
{
    if( size < 2 ) return;
    sz_t size1 = size >> 1;
    buf_order_sort( data, order,         size1       , buf, cmp, direction );
    buf_order_sort( data, order + size1, size - size1, buf, cmp, direction );
    bcore_memcpy( buf, order, size1 * sizeof( sz_t ) );
    for( sz_t i = 0, w = 0, r = size1; i < size1; )
    {
        order[ w++ ] = ( r == size || cmp.f( cmp.o, data[ buf[ i ] ], data[ order[ r ] ] ) * direction >= 0 ) ? buf[ i++ ] : order[ r++ ];
    }
}

bcore_arr_sz_s* bcore_array_spect_create_sorted_order_f( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, bcore_cmp_f cmp, s2_t direction )
{
    bcore_arr_sz_s* order = bcore_arr_sz_s_create();
    sz_t size = get_size( p, o );
    sz_t end_l = end < size ? end : size;
    if( start >= end_l ) return order;
    sz_t range = end_l - start;

    bcore_arr_sz_s_step_fill( order, 0, 1, range );

    if( bcore_array_spect_is_of_links( p ) )
    {
        vc_t* data = ( vc_t* )bcore_array_spect_get_c_data( p, o ) + start;
        sz_t* buf = bcore_un_alloc( sizeof( sz_t ), NULL, 0, range >> 1, NULL );
        buf_order_sort( data, order->data, range, buf, cmp, direction );
        bcore_un_alloc( sizeof( sz_t ), buf, range >> 1, 0, NULL );
    }
    else
    {
        const bcore_inst_s* item_p = bcore_array_spect_is_mutable_mono_typed( p ) ? bcore_inst_s_get_typed( bcore_array_spect_get_mono_type( p, o ) ) : p->item_p;
        sz_t unit_size = item_p->size;
        vc_t src = ( u0_t* )bcore_array_spect_get_c_data( p, o ) + start * unit_size;
        vc_t* data = bcore_un_alloc( sizeof( vc_t ), NULL, 0, range, NULL );
        for( sz_t i = 0; i < range; i++ ) data[ i ] = ( u0_t* )src + i * unit_size;

        sz_t* buf = bcore_un_alloc( sizeof( sz_t ), NULL, 0, range >> 1, NULL );
        buf_order_sort( data, order->data, range, buf, cmp, direction );
        bcore_un_alloc( sizeof( sz_t ), buf,  range >> 1, 0, NULL );
        bcore_un_alloc( sizeof( vc_t ), data, range,      0, NULL );
    }

    for( sz_t i = 0; i < order->size; i++ ) order->data[ i ] += start;

    return order;
}

bcore_arr_sz_s* bcore_array_spect_create_sorted_order( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, s2_t direction )
{
    bl_t is_of_aware = bcore_array_spect_is_of_aware( p );
    bcore_cmp_f cmp =
    {
        .f = is_of_aware ? cmp_aware : ( bcore_fp_cmp_o )bcore_compare_spect,
        .o = is_of_aware ? NULL      : bcore_compare_s_get_typed( bcore_array_spect_get_mono_type( p, o ) )
    };
    return bcore_array_spect_create_sorted_order_f( p, o, start, end, cmp, direction );
}

/**********************************************************************************************************************/

void bcore_array_spect_reorder( const bcore_array_s* p, vd_t o, const bcore_arr_sz_s* order )
{
    sz_t arr_size = bcore_array_spect_get_size( p, o );
    bl_t is_fixed = bcore_array_spect_is_fixed( p );

    if( is_fixed )
    {
        if( arr_size != order->size ) ERR( "Cannot reorder a fixed-size-array to a different order size." );
    }

    if( arr_size > get_space( p, o ) ) bcore_array_spect_make_strong( p, o );

    if( bcore_array_spect_is_of_links( p ) )
    {
        vd_t* data = bcore_array_spect_get_d_data( p, o );
        sz_t buf_space = 0;
        vd_t* buf = bcore_un_alloc( sizeof( vd_t ), NULL, 0, order->size, &buf_space );

        for( sz_t i = 0; i < order->size; i++ )
        {
            assert( order->data[ i ] < arr_size );
            buf[ i ] = bcore_fork( data[ order->data[ i ] ] );
        }

        if( bcore_array_spect_is_mono_typed( p ) )
        {
            tp_t mono_type = bcore_array_spect_get_mono_type( p, o );
            const bcore_inst_s* inst = bcore_inst_s_get_typed( mono_type );
            for( sz_t i = 0; i < arr_size; i++ )
            {
                bcore_release_arg( ( fp_t )bcore_inst_spect_down, inst, data[ i ] );
                data[ i ] = NULL;
            }
        }
        else if( bcore_array_spect_is_of_aware( p ) )
        {
            for( sz_t i = 0; i < arr_size; i++ )
            {
                bcore_release_obj( ( fp_t )bcore_inst_aware_down, data[ i ] );
                data[ i ] = NULL;
            }
        }
        else
        {
            // not reachable as of 2017-10-12
            ERR( "Unhandled array architecture" );
        }

        bcore_array_spect_set_size( p, o, order->size );
        data = bcore_array_spect_get_d_data( p, o );
        for( sz_t i = 0; i < order->size; i++ ) data[ i ] = buf[ i ];

        bcore_un_alloc( sizeof( vd_t ), buf, buf_space, 0, NULL );
    }
    else
    {
        // nested elements are always mono-typed
        tp_t mono_type = bcore_array_spect_get_mono_type( p, o );
        const bcore_inst_s* inst = bcore_inst_s_get_typed( mono_type );
        if( inst->init_flat && inst->copy_flat && inst->down_flat ) // all flat
        {
            sz_t buf_space = 0;
            vd_t buf = bcore_u_alloc( inst->size, NULL, order->size, &buf_space );
            vd_t data = bcore_array_spect_get_d_data( p, o );
            vd_t dst = buf;
            for( sz_t i = 0; i < order->size; i++ )
            {
                assert( order->data[ i ] < arr_size );
                bcore_memcpy( dst, ( u0_t* )data + order->data[ i ] * inst->size, inst->size );
                dst = ( u0_t* )dst + inst->size;
            }
            bcore_array_spect_set_size( p, o, order->size );
            bcore_u_memcpy( inst->size, bcore_array_spect_get_d_data( p, o ), buf, order->size );
            bcore_un_alloc( inst->size, buf, buf_space, 0, NULL );
        }
        else
        {
            sz_t buf_space = 0;
            vd_t buf = bcore_u_alloc( inst->size, NULL, order->size, &buf_space );
            vd_t data = bcore_array_spect_get_d_data( p, o );
            for( sz_t i = 0; i < order->size; i++ )
            {
                assert( order->data[ i ] < arr_size );
                vd_t dst = ( u0_t* )buf  + i * inst->size;
                vc_t src = ( u0_t* )data + order->data[ i ] * inst->size;
                bcore_inst_spect_init( inst, dst );
                bcore_inst_spect_copy( inst, dst, src );
            }
            if( is_fixed )
            {
                for( sz_t i = 0; i < order->size; i++ )
                {
                    vd_t src = ( u0_t* )buf + i * inst->size;
                    bcore_array_spect_set( p, o, i, sr_pwc( inst, src ) );
                    bcore_inst_spect_down( inst, src );
                }
            }
            else
            {
                bcore_array_spect_set_size( p, o, 0 );
                for( sz_t i = 0; i < order->size; i++ )
                {
                    vd_t src = ( u0_t* )buf + i * inst->size;
                    bcore_array_spect_push( p, o, sr_pwc( inst, src ) );
                    bcore_inst_spect_down( inst, src );
                }
            }
            bcore_un_alloc( inst->size, buf, buf_space, 0, NULL );
        }
    }
}

/**********************************************************************************************************************/

static inline const bcore_array_s* atpd( tp_t tp ) { return bcore_array_s_get_typed( tp ); }

sz_t NPX(typed_get_size             )( tp_t tp, vc_t o                                 ) { return NPX(spect_get_size             )( atpd( tp ), o             ); }
sz_t NPX(typed_get_space            )( tp_t tp, vc_t o                                 ) { return NPX(spect_get_space            )( atpd( tp ), o             ); }
sr_s NPX(typed_get                  )( tp_t tp, vc_t o, sz_t index                     ) { return NPX(spect_get                  )( atpd( tp ), o, index      ); }
void NPX(typed_set                  )( tp_t tp, vd_t o, sz_t index, sr_s src           ) {        NPX(spect_set                  )( atpd( tp ), o, index, src ); }
void NPX(typed_set_s3               )( tp_t tp, vd_t o, sz_t index, s3_t val           ) {        NPX(spect_set_s3               )( atpd( tp ), o, index, val ); }
void NPX(typed_set_u3               )( tp_t tp, vd_t o, sz_t index, u3_t val           ) {        NPX(spect_set_u3               )( atpd( tp ), o, index, val ); }
void NPX(typed_set_f3               )( tp_t tp, vd_t o, sz_t index, f3_t val           ) {        NPX(spect_set_f3               )( atpd( tp ), o, index, val ); }
void NPX(typed_set_sz               )( tp_t tp, vd_t o, sz_t index, sz_t val           ) {        NPX(spect_set_sz               )( atpd( tp ), o, index, val ); }
void NPX(typed_set_sc               )( tp_t tp, vd_t o, sz_t index, sc_t val           ) {        NPX(spect_set_sc               )( atpd( tp ), o, index, val ); }
void NPX(typed_set_bl               )( tp_t tp, vd_t o, sz_t index, bl_t val           ) {        NPX(spect_set_bl               )( atpd( tp ), o, index, val ); }
void NPX(typed_set_size             )( tp_t tp, vd_t o, sz_t size                      ) {        NPX(spect_set_size             )( atpd( tp ), o, size       ); }
void NPX(typed_set_space            )( tp_t tp, vd_t o, sz_t space                     ) {        NPX(spect_set_space            )( atpd( tp ), o, space      ); }
void NPX(typed_make_strong          )( tp_t tp, vd_t o                                 ) {        NPX(spect_make_strong          )( atpd( tp ), o             ); }
sr_s NPX(typed_get_first            )( tp_t tp, vc_t o                                 ) { return NPX(spect_get_first            )( atpd( tp ), o             ); }
sr_s NPX(typed_get_last             )( tp_t tp, vc_t o                                 ) { return NPX(spect_get_last             )( atpd( tp ), o             ); }
void NPX(typed_push                 )( tp_t tp, vd_t o, sr_s src                       ) {        NPX(spect_push                 )( atpd( tp ), o, src        ); }
void NPX(typed_push_s3              )( tp_t tp, vd_t o, s3_t val                       ) {        NPX(spect_push_s3              )( atpd( tp ), o, val        ); }
void NPX(typed_push_u3              )( tp_t tp, vd_t o, u3_t val                       ) {        NPX(spect_push_u3              )( atpd( tp ), o, val        ); }
void NPX(typed_push_f3              )( tp_t tp, vd_t o, f3_t val                       ) {        NPX(spect_push_f3              )( atpd( tp ), o, val        ); }
void NPX(typed_push_sz              )( tp_t tp, vd_t o, sz_t val                       ) {        NPX(spect_push_sz              )( atpd( tp ), o, val        ); }
void NPX(typed_push_sc              )( tp_t tp, vd_t o, sc_t val                       ) {        NPX(spect_push_sc              )( atpd( tp ), o, val        ); }
void NPX(typed_push_bl              )( tp_t tp, vd_t o, bl_t val                       ) {        NPX(spect_push_bl              )( atpd( tp ), o, val        ); }
void NPX(typed_pop                  )( tp_t tp, vd_t o                                 ) {        NPX(spect_pop                  )( atpd( tp ), o             ); }
void NPX(typed_set_gtype            )( tp_t tp, vd_t o, tp_t type                      ) {        NPX(spect_set_gtype            )( atpd( tp ), o, type       ); }
bl_t NPX(typed_is_fixed             )( tp_t tp                                         ) { return NPX(spect_is_fixed             )( atpd( tp )                ); }
bl_t NPX(typed_is_static            )( tp_t tp                                         ) { return NPX(spect_is_static            )( atpd( tp )                ); }
bl_t NPX(typed_is_mono_typed        )( tp_t tp                                         ) { return NPX(spect_is_mono_typed        )( atpd( tp )                ); }
bl_t NPX(typed_is_mutable_mono_typed)( tp_t tp                                         ) { return NPX(spect_is_mutable_mono_typed)( atpd( tp )                ); }
bl_t NPX(typed_is_multi_typed       )( tp_t tp                                         ) { return NPX(spect_is_multi_typed       )( atpd( tp )                ); }
bl_t NPX(typed_is_of_aware          )( tp_t tp                                         ) { return NPX(spect_is_of_aware          )( atpd( tp )                ); }
bl_t NPX(typed_is_of_links          )( tp_t tp                                         ) { return NPX(spect_is_of_links          )( atpd( tp )                ); }
tp_t NPX(typed_get_static_type      )( tp_t tp                                         ) { return NPX(spect_get_static_type      )( atpd( tp )                ); }
tp_t NPX(typed_get_mono_type        )( tp_t tp, vc_t o                                 ) { return NPX(spect_get_mono_type        )( atpd( tp ), o             ); }
tp_t NPX(typed_get_type             )( tp_t tp, vc_t o, sz_t index                     ) { return NPX(spect_get_type             )( atpd( tp ), o, index      ); }
vc_t NPX(typed_get_c_data           )( tp_t tp, vc_t o                                 ) { return NPX(spect_get_c_data           )( atpd( tp ), o             ); }
vd_t NPX(typed_get_d_data           )( tp_t tp, vd_t o                                 ) { return NPX(spect_get_d_data           )( atpd( tp ), o             ); }
sz_t NPX(typed_get_unit_size        )( tp_t tp, vc_t o                                 ) { return NPX(spect_get_unit_size        )( atpd( tp ), o             ); }
vc_t NPX(typed_max                  )( tp_t tp, vc_t o, sz_t st, sz_t nd, s2_t d       ) { return NPX(spect_max                  )( atpd( tp ), o, st, nd, d  ); }
sz_t NPX(typed_max_index            )( tp_t tp, vc_t o, sz_t st, sz_t nd, s2_t d       ) { return NPX(spect_max_index            )( atpd( tp ), o, st, nd, d  ); }
void NPX(typed_sort                 )( tp_t tp, vd_t o, sz_t st, sz_t nd, s2_t d       ) {        NPX(spect_sort                 )( atpd( tp ), o, st, nd, d  ); }
void NPX(typed_do                   )( tp_t tp, vd_t o, sz_t st, sz_t nd, fp_t f       ) {        NPX(spect_do                   )( atpd( tp ), o, st, nd, f  ); }
void NPX(typed_reorder              )( tp_t tp, vd_t o, const bcore_arr_sz_s* od       ) {        NPX(spect_reorder              )( atpd( tp ), o, od         ); }
vc_t NPX(typed_max_f                )( tp_t tp, vc_t o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { return NPX(spect_max_f       )( atpd( tp ), o, st, nd, c, d ); }
sz_t NPX(typed_max_index_f          )( tp_t tp, vc_t o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { return NPX(spect_max_index_f )( atpd( tp ), o, st, nd, c, d ); }
void NPX(typed_sort_f               )( tp_t tp, vd_t o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) {        NPX(spect_sort_f      )( atpd( tp ), o, st, nd, c, d ); }
bcore_arr_sz_s* NPX(typed_create_sorted_order  )( tp_t tp, vc_t o, sz_t st, sz_t nd, s2_t d                ) { return NPX(spect_create_sorted_order   )( atpd( tp ), o, st, nd, d ); }
bcore_arr_sz_s* NPX(typed_create_sorted_order_f)( tp_t tp, vc_t o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { return NPX(spect_create_sorted_order_f )( atpd( tp ), o, st, nd, c, d ); }

sz_t NPX(aware_get_size             )( vc_t o                                 ) { return NPX(typed_get_size             )( *( aware_t* )o, o             ); }
sz_t NPX(aware_get_space            )( vc_t o                                 ) { return NPX(typed_get_space            )( *( aware_t* )o, o             ); }
sr_s NPX(aware_get                  )( vc_t o, sz_t index                     ) { return NPX(typed_get                  )( *( aware_t* )o, o, index      ); }
void NPX(aware_set                  )( vd_t o, sz_t index, sr_s src           ) {        NPX(typed_set                  )( *( aware_t* )o, o, index, src ); }
void NPX(aware_set_s3               )( vd_t o, sz_t index, s3_t val           ) {        NPX(typed_set_s3               )( *( aware_t* )o, o, index, val ); }
void NPX(aware_set_u3               )( vd_t o, sz_t index, u3_t val           ) {        NPX(typed_set_u3               )( *( aware_t* )o, o, index, val ); }
void NPX(aware_set_f3               )( vd_t o, sz_t index, f3_t val           ) {        NPX(typed_set_f3               )( *( aware_t* )o, o, index, val ); }
void NPX(aware_set_sz               )( vd_t o, sz_t index, sz_t val           ) {        NPX(typed_set_sz               )( *( aware_t* )o, o, index, val ); }
void NPX(aware_set_sc               )( vd_t o, sz_t index, sc_t val           ) {        NPX(typed_set_sc               )( *( aware_t* )o, o, index, val ); }
void NPX(aware_set_bl               )( vd_t o, sz_t index, bl_t val           ) {        NPX(typed_set_bl               )( *( aware_t* )o, o, index, val ); }
void NPX(aware_set_size             )( vd_t o, sz_t size                      ) {        NPX(typed_set_size             )( *( aware_t* )o, o, size       ); }
void NPX(aware_set_space            )( vd_t o, sz_t space                     ) {        NPX(typed_set_space            )( *( aware_t* )o, o, space      ); }
void NPX(aware_make_strong          )( vd_t o                                 ) {        NPX(typed_make_strong          )( *( aware_t* )o, o             ); }
sr_s NPX(aware_get_first            )( vc_t o                                 ) { return NPX(typed_get_first            )( *( aware_t* )o, o             ); }
sr_s NPX(aware_get_last             )( vc_t o                                 ) { return NPX(typed_get_last             )( *( aware_t* )o, o             ); }
void NPX(aware_push                 )( vd_t o, sr_s src                       ) {        NPX(typed_push                 )( *( aware_t* )o, o, src        ); }
void NPX(aware_push_s3              )( vd_t o, s3_t val                       ) {        NPX(typed_push_s3              )( *( aware_t* )o, o, val        ); }
void NPX(aware_push_u3              )( vd_t o, u3_t val                       ) {        NPX(typed_push_u3              )( *( aware_t* )o, o, val        ); }
void NPX(aware_push_f3              )( vd_t o, f3_t val                       ) {        NPX(typed_push_f3              )( *( aware_t* )o, o, val        ); }
void NPX(aware_push_sz              )( vd_t o, sz_t val                       ) {        NPX(typed_push_sz              )( *( aware_t* )o, o, val        ); }
void NPX(aware_push_sc              )( vd_t o, sc_t val                       ) {        NPX(typed_push_sc              )( *( aware_t* )o, o, val        ); }
void NPX(aware_push_bl              )( vd_t o, bl_t val                       ) {        NPX(typed_push_bl              )( *( aware_t* )o, o, val        ); }
void NPX(aware_pop                  )( vd_t o                                 ) {        NPX(typed_pop                  )( *( aware_t* )o, o             ); }
void NPX(aware_set_gtype            )( vd_t o, tp_t type                      ) {        NPX(typed_set_gtype            )( *( aware_t* )o, o, type       ); }
bl_t NPX(aware_is_fixed             )( vc_t o                                 ) { return NPX(typed_is_fixed             )( *( aware_t* )o                ); }
bl_t NPX(aware_is_static            )( vc_t o                                 ) { return NPX(typed_is_static            )( *( aware_t* )o                ); }
bl_t NPX(aware_is_mono_typed        )( vc_t o                                 ) { return NPX(typed_is_mono_typed        )( *( aware_t* )o                ); }
bl_t NPX(aware_is_mutable_mono_typed)( vc_t o                                 ) { return NPX(typed_is_mutable_mono_typed)( *( aware_t* )o                ); }
bl_t NPX(aware_is_multi_typed       )( vc_t o                                 ) { return NPX(typed_is_multi_typed       )( *( aware_t* )o                ); }
bl_t NPX(aware_is_of_aware          )( vc_t o                                 ) { return NPX(typed_is_of_aware          )( *( aware_t* )o                ); }
bl_t NPX(aware_is_of_links          )( vc_t o                                 ) { return NPX(typed_is_of_links          )( *( aware_t* )o                ); }
tp_t NPX(aware_get_static_type      )( vc_t o                                 ) { return NPX(typed_get_static_type      )( *( aware_t* )o                ); }
tp_t NPX(aware_get_mono_type        )( vc_t o                                 ) { return NPX(typed_get_mono_type        )( *( aware_t* )o, o             ); }
tp_t NPX(aware_get_type             )( vc_t o, sz_t index                     ) { return NPX(typed_get_type             )( *( aware_t* )o, o, index      ); }
vc_t NPX(aware_get_c_data           )( vc_t o                                 ) { return NPX(typed_get_c_data           )( *( aware_t* )o, o             ); }
vd_t NPX(aware_get_d_data           )( vd_t o                                 ) { return NPX(typed_get_d_data           )( *( aware_t* )o, o             ); }
sz_t NPX(aware_get_unit_size        )( vc_t o                                 ) { return NPX(typed_get_unit_size        )( *( aware_t* )o, o             ); }
vc_t NPX(aware_max                  )( vc_t o, sz_t st, sz_t nd, s2_t d       ) { return NPX(typed_max                  )( *( aware_t* )o, o, st, nd, d  ); }
sz_t NPX(aware_max_index            )( vc_t o, sz_t st, sz_t nd, s2_t d       ) { return NPX(typed_max_index            )( *( aware_t* )o, o, st, nd, d  ); }
void NPX(aware_sort                 )( vd_t o, sz_t st, sz_t nd, s2_t d       ) {        NPX(typed_sort                 )( *( aware_t* )o, o, st, nd, d  ); }
void NPX(aware_do                   )( vd_t o, sz_t st, sz_t nd, fp_t f       ) {        NPX(typed_do                   )( *( aware_t* )o, o, st, nd, f  ); }
void NPX(aware_reorder              )( vd_t o, const bcore_arr_sz_s* od       ) {        NPX(typed_reorder              )( *( aware_t* )o, o, od         ); }
vc_t NPX(aware_max_f                )( vc_t o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { return NPX(typed_max_f       )( *( aware_t* )o, o, st, nd, c, d ); }
sz_t NPX(aware_max_index_f          )( vc_t o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { return NPX(typed_max_index_f )( *( aware_t* )o, o, st, nd, c, d ); }
void NPX(aware_sort_f               )( vd_t o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) {        NPX(typed_sort_f      )( *( aware_t* )o, o, st, nd, c, d ); }
bcore_arr_sz_s* NPX(aware_create_sorted_order  )( vc_t o, sz_t st, sz_t nd, s2_t d                ) { return NPX(typed_create_sorted_order   )( *( aware_t* )o, o, st, nd, d ); }
bcore_arr_sz_s* NPX(aware_create_sorted_order_f)( vc_t o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { return NPX(typed_create_sorted_order_f )( *( aware_t* )o, o, st, nd, c, d ); }

inline static vc_t w_spect( sr_s o ) { if( sr_s_is_const( &o ) ) ERR( "Attempt to modify a constant object" ); return ch_spect_p( o.p, TYPEOF_bcore_array_s ); }
inline static vc_t r_spect( sr_s o ) { return ch_spect_p( o.p, TYPEOF_bcore_array_s ); }
inline static vc_t x_spect( sr_s o ) { return ch_spect_p( o.p, TYPEOF_bcore_array_s ); }

sz_t NPX(get_size             )( sr_s o                           ) { sz_t r = NPX(spect_get_size             )( r_spect( o ), o.o             ); sr_down( o ); return r; }
sz_t NPX(get_space            )( sr_s o                           ) { sz_t r = NPX(spect_get_space            )( r_spect( o ), o.o             ); sr_down( o ); return r; }
sr_s NPX(get                  )( sr_s o, sz_t index               ) { sr_s r = NPX(spect_get                  )( x_spect( o ), o.o, index      ); sr_s_set_const( &r, sr_s_is_const( &o ) ); sr_down( o ); return r; }
void NPX(set                  )( sr_s o, sz_t index, sr_s src     ) {          NPX(spect_set                  )( w_spect( o ), o.o, index, src ); sr_down( o );           }
void NPX(set_s3               )( sr_s o, sz_t index, s3_t val     ) {          NPX(spect_set_s3               )( w_spect( o ), o.o, index, val ); sr_down( o );           }
void NPX(set_u3               )( sr_s o, sz_t index, u3_t val     ) {          NPX(spect_set_u3               )( w_spect( o ), o.o, index, val ); sr_down( o );           }
void NPX(set_f3               )( sr_s o, sz_t index, f3_t val     ) {          NPX(spect_set_f3               )( w_spect( o ), o.o, index, val ); sr_down( o );           }
void NPX(set_sz               )( sr_s o, sz_t index, sz_t val     ) {          NPX(spect_set_sz               )( w_spect( o ), o.o, index, val ); sr_down( o );           }
void NPX(set_sc               )( sr_s o, sz_t index, sc_t val     ) {          NPX(spect_set_sc               )( w_spect( o ), o.o, index, val ); sr_down( o );           }
void NPX(set_bl               )( sr_s o, sz_t index, bl_t val     ) {          NPX(spect_set_bl               )( w_spect( o ), o.o, index, val ); sr_down( o );           }
void NPX(set_size             )( sr_s o, sz_t size                ) {          NPX(spect_set_size             )( w_spect( o ), o.o, size       ); sr_down( o );           }
void NPX(set_space            )( sr_s o, sz_t space               ) {          NPX(spect_set_space            )( w_spect( o ), o.o, space      ); sr_down( o );           }
void NPX(make_strong          )( sr_s o                           ) {          NPX(spect_make_strong          )( w_spect( o ), o.o             ); sr_down( o );           }
sr_s NPX(get_first            )( sr_s o                           ) { sr_s r = NPX(spect_get_first            )( r_spect( o ), o.o             ); sr_down( o ); return r; }
sr_s NPX(get_last             )( sr_s o                           ) { sr_s r = NPX(spect_get_last             )( r_spect( o ), o.o             ); sr_down( o ); return r; }
void NPX(push                 )( sr_s o, sr_s src                 ) {          NPX(spect_push                 )( w_spect( o ), o.o, src        ); sr_down( o );           }
void NPX(push_s3              )( sr_s o, s3_t val                 ) {          NPX(spect_push_s3              )( w_spect( o ), o.o, val        ); sr_down( o );           }
void NPX(push_u3              )( sr_s o, u3_t val                 ) {          NPX(spect_push_u3              )( w_spect( o ), o.o, val        ); sr_down( o );           }
void NPX(push_f3              )( sr_s o, f3_t val                 ) {          NPX(spect_push_f3              )( w_spect( o ), o.o, val        ); sr_down( o );           }
void NPX(push_sz              )( sr_s o, sz_t val                 ) {          NPX(spect_push_sz              )( w_spect( o ), o.o, val        ); sr_down( o );           }
void NPX(push_sc              )( sr_s o, sc_t val                 ) {          NPX(spect_push_sc              )( w_spect( o ), o.o, val        ); sr_down( o );           }
void NPX(push_bl              )( sr_s o, bl_t val                 ) {          NPX(spect_push_bl              )( w_spect( o ), o.o, val        ); sr_down( o );           }
void NPX(pop                  )( sr_s o                           ) {          NPX(spect_pop                  )( w_spect( o ), o.o             ); sr_down( o );           }
void NPX(set_gtype            )( sr_s o, tp_t type                ) {          NPX(spect_set_gtype            )( w_spect( o ), o.o, type       ); sr_down( o );           }
bl_t NPX(is_fixed             )( sr_s o                           ) { bl_t r = NPX(spect_is_fixed             )( r_spect( o )                  ); sr_down( o ); return r; }
bl_t NPX(is_static            )( sr_s o                           ) { bl_t r = NPX(spect_is_static            )( r_spect( o )                  ); sr_down( o ); return r; }
bl_t NPX(is_mono_typed        )( sr_s o                           ) { bl_t r = NPX(spect_is_mono_typed        )( r_spect( o )                  ); sr_down( o ); return r; }
bl_t NPX(is_mutable_mono_typed)( sr_s o                           ) { bl_t r = NPX(spect_is_mutable_mono_typed)( r_spect( o )                  ); sr_down( o ); return r; }
bl_t NPX(is_multi_typed       )( sr_s o                           ) { bl_t r = NPX(spect_is_multi_typed       )( r_spect( o )                  ); sr_down( o ); return r; }
bl_t NPX(is_of_aware          )( sr_s o                           ) { bl_t r = NPX(spect_is_of_aware          )( r_spect( o )                  ); sr_down( o ); return r; }
bl_t NPX(is_of_links          )( sr_s o                           ) { bl_t r = NPX(spect_is_of_links          )( r_spect( o )                  ); sr_down( o ); return r; }
tp_t NPX(get_static_type      )( sr_s o                           ) { tp_t r = NPX(spect_get_static_type      )( r_spect( o )                  ); sr_down( o ); return r; }
tp_t NPX(get_mono_type        )( sr_s o                           ) { tp_t r = NPX(spect_get_mono_type        )( r_spect( o ), o.o             ); sr_down( o ); return r; }
tp_t NPX(get_type             )( sr_s o, sz_t index               ) { tp_t r = NPX(spect_get_type             )( r_spect( o ), o.o, index      ); sr_down( o ); return r; }
vc_t NPX(get_c_data           )( sr_s o                           ) { vc_t r = NPX(spect_get_c_data           )( r_spect( o ), o.o             ); sr_down( o ); return r; }
vd_t NPX(get_d_data           )( sr_s o                           ) { vd_t r = NPX(spect_get_d_data           )( w_spect( o ), o.o             ); sr_down( o ); return r; }
sz_t NPX(get_unit_size        )( sr_s o                           ) { sz_t r = NPX(spect_get_unit_size        )( r_spect( o ), o.o             ); sr_down( o ); return r; }
vc_t NPX(max                  )( sr_s o, sz_t st, sz_t nd, s2_t d ) { vc_t r = NPX(spect_max                  )( r_spect( o ), o.o, st, nd, d  ); sr_down( o ); return r; }
sz_t NPX(max_index            )( sr_s o, sz_t st, sz_t nd, s2_t d ) { sz_t r = NPX(spect_max_index            )( r_spect( o ), o.o, st, nd, d  ); sr_down( o ); return r; }
void NPX(sort                 )( sr_s o, sz_t st, sz_t nd, s2_t d ) {          NPX(spect_sort                 )( w_spect( o ), o.o, st, nd, d  ); sr_down( o );           }
void NPX(do                   )( sr_s o, sz_t st, sz_t nd, fp_t f ) {          NPX(spect_do                   )( w_spect( o ), o.o, st, nd, f  ); sr_down( o );           }
void NPX(reorder              )( sr_s o, const bcore_arr_sz_s* od ) {          NPX(spect_reorder              )( w_spect( o ), o.o, od         ); sr_down( o );           }
vc_t NPX(max_f                )( sr_s o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { vc_t r = NPX(spect_max_f       )( r_spect( o ), o.o, st, nd, c, d ); sr_down( o ); return r; }
sz_t NPX(max_index_f          )( sr_s o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { sz_t r = NPX(spect_max_index_f )( r_spect( o ), o.o, st, nd, c, d ); sr_down( o ); return r; }
void NPX(sort_f               )( sr_s o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) {          NPX(spect_sort_f      )( w_spect( o ), o.o, st, nd, c, d ); sr_down( o );           }
bcore_arr_sz_s* NPX(create_sorted_order  )( sr_s o, sz_t st, sz_t nd, s2_t d                ) { bcore_arr_sz_s* r = NPX(spect_create_sorted_order   )( r_spect( o ), o.o, st, nd, d    ); sr_down( o ); return r; }
bcore_arr_sz_s* NPX(create_sorted_order_f)( sr_s o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { bcore_arr_sz_s* r = NPX(spect_create_sorted_order_f )( r_spect( o ), o.o, st, nd, c, d ); sr_down( o ); return r; }

sz_t NPX(q_get_size             )( const sr_s* o                           ) { return   NPX(spect_get_size             )( r_spect( *o ), o->o             ); }
sz_t NPX(q_get_space            )( const sr_s* o                           ) { return   NPX(spect_get_space            )( r_spect( *o ), o->o             ); }
sr_s NPX(q_get                  )( const sr_s* o, sz_t index               ) { sr_s r = NPX(spect_get                  )( x_spect( *o ), o->o, index      ); sr_s_set_const( &r, sr_s_is_const( o ) ); return r; }
void NPX(q_set                  )( const sr_s* o, sz_t index, sr_s src     ) {          NPX(spect_set                  )( w_spect( *o ), o->o, index, src ); }
void NPX(q_set_s3               )( const sr_s* o, sz_t index, s3_t val     ) {          NPX(spect_set_s3               )( w_spect( *o ), o->o, index, val ); }
void NPX(q_set_u3               )( const sr_s* o, sz_t index, u3_t val     ) {          NPX(spect_set_u3               )( w_spect( *o ), o->o, index, val ); }
void NPX(q_set_f3               )( const sr_s* o, sz_t index, f3_t val     ) {          NPX(spect_set_f3               )( w_spect( *o ), o->o, index, val ); }
void NPX(q_set_sz               )( const sr_s* o, sz_t index, sz_t val     ) {          NPX(spect_set_sz               )( w_spect( *o ), o->o, index, val ); }
void NPX(q_set_sc               )( const sr_s* o, sz_t index, sc_t val     ) {          NPX(spect_set_sc               )( w_spect( *o ), o->o, index, val ); }
void NPX(q_set_bl               )( const sr_s* o, sz_t index, bl_t val     ) {          NPX(spect_set_bl               )( w_spect( *o ), o->o, index, val ); }
void NPX(q_set_size             )( const sr_s* o, sz_t size                ) {          NPX(spect_set_size             )( w_spect( *o ), o->o, size       ); }
void NPX(q_set_space            )( const sr_s* o, sz_t space               ) {          NPX(spect_set_space            )( w_spect( *o ), o->o, space      ); }
void NPX(q_make_strong          )( const sr_s* o                           ) {          NPX(spect_make_strong          )( w_spect( *o ), o->o             ); }
sr_s NPX(q_get_first            )( const sr_s* o                           ) { return   NPX(spect_get_first            )( r_spect( *o ), o->o             ); }
sr_s NPX(q_get_last             )( const sr_s* o                           ) { return   NPX(spect_get_last             )( r_spect( *o ), o->o             ); }
void NPX(q_push                 )( const sr_s* o, sr_s src                 ) {          NPX(spect_push                 )( w_spect( *o ), o->o, src        ); }
void NPX(q_push_s3              )( const sr_s* o, s3_t val                 ) {          NPX(spect_push_s3              )( w_spect( *o ), o->o, val        ); }
void NPX(q_push_u3              )( const sr_s* o, u3_t val                 ) {          NPX(spect_push_u3              )( w_spect( *o ), o->o, val        ); }
void NPX(q_push_f3              )( const sr_s* o, f3_t val                 ) {          NPX(spect_push_f3              )( w_spect( *o ), o->o, val        ); }
void NPX(q_push_sz              )( const sr_s* o, sz_t val                 ) {          NPX(spect_push_sz              )( w_spect( *o ), o->o, val        ); }
void NPX(q_push_sc              )( const sr_s* o, sc_t val                 ) {          NPX(spect_push_sc              )( w_spect( *o ), o->o, val        ); }
void NPX(q_push_bl              )( const sr_s* o, bl_t val                 ) {          NPX(spect_push_bl              )( w_spect( *o ), o->o, val        ); }
void NPX(q_pop                  )( const sr_s* o                           ) {          NPX(spect_pop                  )( w_spect( *o ), o->o             ); }
void NPX(q_set_gtype            )( const sr_s* o, tp_t type                ) {          NPX(spect_set_gtype            )( w_spect( *o ), o->o, type       ); }
bl_t NPX(q_is_fixed             )( const sr_s* o                           ) { return   NPX(spect_is_fixed             )( r_spect( *o )                   ); }
bl_t NPX(q_is_static            )( const sr_s* o                           ) { return   NPX(spect_is_static            )( r_spect( *o )                   ); }
bl_t NPX(q_is_mono_typed        )( const sr_s* o                           ) { return   NPX(spect_is_mono_typed        )( r_spect( *o )                   ); }
bl_t NPX(q_is_mutable_mono_typed)( const sr_s* o                           ) { return   NPX(spect_is_mutable_mono_typed)( r_spect( *o )                   ); }
bl_t NPX(q_is_multi_typed       )( const sr_s* o                           ) { return   NPX(spect_is_multi_typed       )( r_spect( *o )                   ); }
bl_t NPX(q_is_of_aware          )( const sr_s* o                           ) { return   NPX(spect_is_of_aware          )( r_spect( *o )                   ); }
bl_t NPX(q_is_of_links          )( const sr_s* o                           ) { return   NPX(spect_is_of_links          )( r_spect( *o )                   ); }
tp_t NPX(q_get_static_type      )( const sr_s* o                           ) { return   NPX(spect_get_static_type      )( r_spect( *o )                   ); }
tp_t NPX(q_get_mono_type        )( const sr_s* o                           ) { return   NPX(spect_get_mono_type        )( r_spect( *o ), o->o             ); }
tp_t NPX(q_get_type             )( const sr_s* o, sz_t index               ) { return   NPX(spect_get_type             )( r_spect( *o ), o->o, index      ); }
vc_t NPX(q_get_c_data           )( const sr_s* o                           ) { return   NPX(spect_get_c_data           )( r_spect( *o ), o->o             ); }
vd_t NPX(q_get_d_data           )( const sr_s* o                           ) { return   NPX(spect_get_d_data           )( w_spect( *o ), o->o             ); }
sz_t NPX(q_get_unit_size        )( const sr_s* o                           ) { return   NPX(spect_get_unit_size        )( r_spect( *o ), o->o             ); }
vc_t NPX(q_max                  )( const sr_s* o, sz_t st, sz_t nd, s2_t d ) { return   NPX(spect_max                  )( r_spect( *o ), o->o, st, nd, d  ); }
sz_t NPX(q_max_index            )( const sr_s* o, sz_t st, sz_t nd, s2_t d ) { return   NPX(spect_max_index            )( r_spect( *o ), o->o, st, nd, d  ); }
void NPX(q_sort                 )( const sr_s* o, sz_t st, sz_t nd, s2_t d ) {          NPX(spect_sort                 )( w_spect( *o ), o->o, st, nd, d  ); }
void NPX(q_do                   )( const sr_s* o, sz_t st, sz_t nd, fp_t f ) {          NPX(spect_do                   )( w_spect( *o ), o->o, st, nd, f  ); }
void NPX(q_reorder              )( const sr_s* o, const bcore_arr_sz_s* od ) {          NPX(spect_reorder              )( w_spect( *o ), o->o, od         ); }
vc_t NPX(q_max_f                )( const sr_s* o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { return NPX(spect_max_f       )( r_spect( *o ), o->o, st, nd, c, d ); }
sz_t NPX(q_max_index_f          )( const sr_s* o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { return NPX(spect_max_index_f )( r_spect( *o ), o->o, st, nd, c, d ); }
void NPX(q_sort_f               )( const sr_s* o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) {        NPX(spect_sort_f      )( w_spect( *o ), o->o, st, nd, c, d ); }
bcore_arr_sz_s* NPX(q_create_sorted_order  )( const sr_s* o, sz_t st, sz_t nd, s2_t d                ) { return NPX(spect_create_sorted_order   )( r_spect( *o ), o->o, st, nd, d    ); }
bcore_arr_sz_s* NPX(q_create_sorted_order_f)( const sr_s* o, sz_t st, sz_t nd, bcore_cmp_f c, s2_t d ) { return NPX(spect_create_sorted_order_f )( r_spect( *o ), o->o, st, nd, c, d ); }

/**********************************************************************************************************************/
// testing, debugging

static void test_string_array( sc_t type_sc )
{
    vd_t arr = bcore_inst_typed_create( typeof( type_sc ) );
    const bcore_array_s* arr_p = bcore_array_s_get_aware( arr );

    bl_t is_dynamic = !bcore_array_spect_is_fixed( arr_p );

    if( is_dynamic ) bcore_array_spect_set_gtype( arr_p, arr, typeof( "st_s" ) );
    if( is_dynamic ) bcore_array_spect_set_size( arr_p, arr, 5 );
    bcore_array_spect_set( arr_p, arr, 0, sr_asd( st_s_createf( "test line a" ) ) );
    bcore_array_spect_set( arr_p, arr, 1, sr_asd( st_s_createf( "some nonsense: sakjd" ) ) );
    bcore_array_spect_set( arr_p, arr, 2, sr_asd( st_s_createf( "some nonsense: dspaud" ) ) );
    bcore_array_spect_set( arr_p, arr, 7, sr_asd( st_s_createf( "test line x" ) ) );
    if( is_dynamic )
    {
        bcore_array_spect_push( arr_p, arr, sr_asd( st_s_createf( "test line p" ) ) );
    }
    else
    {
        bcore_array_spect_set( arr_p, arr, 8, sr_asd( st_s_createf( "test line p" ) ) );
    }

    if( is_dynamic ) bcore_array_spect_set_space( arr_p, arr, 20 );

    ASSERT( bcore_array_spect_max_index( arr_p, arr, 0, -1, 1 ) == 7 );

    bcore_array_spect_sort( arr_p, arr, 0, -1, 1 );
    {
        bcore_arr_sz_s* order = bcore_array_spect_create_sorted_order( arr_p, arr, 0, -1, -1 );
        bcore_array_spect_reorder( arr_p, arr, order );
        bcore_arr_sz_s_discard( order );
    }

    ASSERT( bcore_array_spect_max_index( arr_p, arr, 0, -1, 1 ) == 0 );

    ASSERT( st_s_cmp_sc( ( const st_s* )arr_p->get( arr_p, arr, 0 ).o, "test line x" ) == 0 );
    ASSERT( st_s_cmp_sc( ( const st_s* )arr_p->get( arr_p, arr, 1 ).o, "test line p" ) == 0 );
    ASSERT( st_s_cmp_sc( ( const st_s* )arr_p->get( arr_p, arr, 2 ).o, "test line a" ) == 0 );
    ASSERT( st_s_cmp_sc( ( const st_s* )arr_p->get( arr_p, arr, 3 ).o, "some nonsense: sakjd" ) == 0 );
    ASSERT( st_s_cmp_sc( ( const st_s* )arr_p->get( arr_p, arr, 4 ).o, "some nonsense: dspaud" ) == 0 );

    if( is_dynamic ) ASSERT( get_size( arr_p, arr ) == 9 );

    bcore_arr_sz_s* order = bcore_arr_sz_s_create();
    bcore_arr_sz_s_push( order, 2 );
    bcore_arr_sz_s_push( order, 2 );
    bcore_arr_sz_s_push( order, 1 );
    bcore_arr_sz_s_push( order, 0 );
    bcore_arr_sz_s_push( order, 3 );
    bcore_arr_sz_s_push( order, 4 );

    if( is_dynamic )
    {
        bcore_array_spect_reorder( arr_p, arr, order );
        ASSERT( get_size( arr_p, arr ) == order->size );

        ASSERT( st_s_cmp_sc( ( const st_s* )arr_p->get( arr_p, arr, 0 ).o, "test line a" ) == 0 );
        ASSERT( st_s_cmp_sc( ( const st_s* )arr_p->get( arr_p, arr, 1 ).o, "test line a" ) == 0 );
        ASSERT( st_s_cmp_sc( ( const st_s* )arr_p->get( arr_p, arr, 2 ).o, "test line p" ) == 0 );
        ASSERT( st_s_cmp_sc( ( const st_s* )arr_p->get( arr_p, arr, 3 ).o, "test line x" ) == 0 );
        ASSERT( st_s_cmp_sc( ( const st_s* )arr_p->get( arr_p, arr, 4 ).o, "some nonsense: sakjd" ) == 0 );
        ASSERT( st_s_cmp_sc( ( const st_s* )arr_p->get( arr_p, arr, 5 ).o, "some nonsense: dspaud" ) == 0 );
    }

    bcore_arr_sz_s_discard( order );
    bcore_inst_aware_discard( arr );
}

static st_s* spect_array_selftest( void )
{
    bcore_flect_define_self_d( bcore_self_s_build_parse_sc( "string_array = { aware_t _; st_s [] string_arr; }", 0 ) );
    vd_t arr = bcore_inst_typed_create( typeof( "string_array" ) );
    const bcore_array_s* arr_p = bcore_array_s_get_aware( arr );

    bcore_array_spect_push( arr_p, arr, sr_asd( st_s_createf( "string_static_dyn_array      = { aware_t _; st_s    []   arr; }" ) ) );
    bcore_array_spect_push( arr_p, arr, sr_asd( st_s_createf( "string_static_link_dyn_array = { aware_t _; st_s *  []   arr; }" ) ) );
    bcore_array_spect_push( arr_p, arr, sr_asd( st_s_createf( "string_typed_dyn_array       = { aware_t _; typed   []   arr; }" ) ) );
    bcore_array_spect_push( arr_p, arr, sr_asd( st_s_createf( "string_typed_link_dyn_array  = { aware_t _; typed * []   arr; }" ) ) );
    bcore_array_spect_push( arr_p, arr, sr_asd( st_s_createf( "string_aware_link_dyn_array  = { aware_t _; aware * []   arr; }" ) ) );
    bcore_array_spect_push( arr_p, arr, sr_asd( st_s_createf( "string_static_fix_array      = { aware_t _; st_s    [10] arr; }" ) ) );
    bcore_array_spect_push( arr_p, arr, sr_asd( st_s_createf( "string_static_link_fix_array = { aware_t _; st_s *  [10] arr; }" ) ) );
    bcore_array_spect_push( arr_p, arr, sr_asd( st_s_createf( "string_aware_link_fix_array  = { aware_t _; aware * [10] arr; }" ) ) );

    for( sz_t i = 0; i < get_size( arr_p, arr ); i++ )
    {

        const st_s* code = arr_p->get( arr_p, arr, i ).o;
        bcore_self_s* self = bcore_self_s_build_parse_sc( code->sc, 0 );
        ASSERT( !bcore_spect_trait_supported( typeof( "bcore_array" ), self->type ) );
        tp_t type = bcore_flect_type_self_c( self );
        ASSERT( bcore_spect_trait_supported( typeof( "bcore_array" ), type ) );
        test_string_array( nameof( type ) );
        bcore_self_s_discard( self );
    }

    bcore_inst_aware_discard( arr );

    // some non-arrays
    ASSERT( !bcore_spect_trait_supported( typeof( "bcore_array" ), typeof( "st_s" ) ) );
    ASSERT( !bcore_spect_trait_supported( typeof( "bcore_array" ), typeof( "f3_t" ) ) );
    ASSERT( !bcore_spect_trait_supported( typeof( "bcore_array" ), typeof( "bcore_txt_ml_interpreter_s" ) ) );

    // sorting a permutation
    {
        bcore_arr_sz_s* arr = bcore_arr_sz_s_create_random_permutation( bcore_xsg_u2, 2, 1000 );
        bcore_array_aware_sort( arr, 0, -1, 1 );
        for( sz_t i = 0; i < arr->size; i++ ) ASSERT( arr->data[ i ] == i );
        bcore_arr_sz_s_discard( arr );
    }
    {
        bcore_arr_sz_s* arr = bcore_arr_sz_s_create_random_permutation( bcore_xsg_u2, 2, 1000 );
        bcore_arr_sz_s_sort( arr, 1 );
        for( sz_t i = 0; i < arr->size; i++ ) ASSERT( arr->data[ i ] == i );
        bcore_arr_sz_s_discard( arr );
    }

    return NULL;
}

/**********************************************************************************************************************/
// signal

BCORE_DEFINE_SPECT_CACHE( bcore_array_s );
vd_t bcore_spect_array_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_array" ) ) )
    {
        case TYPEOF_init1:
        {
            array_s_define_trait();
            bcore_flect_define_creator( typeof( "bcore_array_s"  ), array_s_create_self  );
            bcore_spect_setup_cache( &bcore_array_s_cache_g );
        }
        break;

        case TYPEOF_selftest:
        {
            return spect_array_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

