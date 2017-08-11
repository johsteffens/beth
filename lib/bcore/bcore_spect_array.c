/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_array.h"
#include "bcore_spect_compare.h"
#include "bcore_spect.h"

/**********************************************************************************************************************/
// bcore_array_s

static void array_s_down( bcore_array_s* o );

static void array_s_init( bcore_array_s* o )
{
    bcore_memzero( o, sizeof( bcore_array_s ) );
    o->p_type = typeof( "bcore_array_s" );
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
    array_s_down( o );
    bcore_free( o );
}

static bcore_signature_s* array_s_create_signature( bcore_array_s* o )
{
    return bcore_signature_s_create_an( 2, o->p_type, o->o_type );
}

/**********************************************************************************************************************/

static sz_t get_size_static( const bcore_array_s* p, vc_t o )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    return ( ( bcore_static_array_s* )obj )->size;
}

static sz_t get_size_typed( const bcore_array_s* p, vc_t o )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    return ( ( bcore_typed_array_s* )obj )->size;
}

static sz_t get_size_static_link( const bcore_array_s* p, vc_t o )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    return ( ( bcore_static_link_array_s* )obj )->size;
}

static sz_t get_size_typed_link( const bcore_array_s* p, vc_t o )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    return ( ( bcore_typed_link_array_s* )obj )->size;
}

static sz_t get_size_aware_link( const bcore_array_s* p, vc_t o )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    return ( ( bcore_aware_link_array_s* )obj )->size;
}

/**********************************************************************************************************************/

static sz_t get_space_static( const bcore_array_s* p, vc_t o )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    return ( ( bcore_static_array_s* )obj )->space;
}

static sz_t get_space_typed( const bcore_array_s* p, vc_t o )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    return ( ( bcore_typed_array_s* )obj )->space;
}

static sz_t get_space_static_link( const bcore_array_s* p, vc_t o )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    return ( ( bcore_static_link_array_s* )obj )->space;
}

static sz_t get_space_typed_link( const bcore_array_s* p, vc_t o )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    return ( ( bcore_typed_link_array_s* )obj )->space;
}

static sz_t get_space_aware_link( const bcore_array_s* p, vc_t o )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    return ( ( bcore_aware_link_array_s* )obj )->space;
}

/**********************************************************************************************************************/

static void set_space( const bcore_array_s* p, vd_t o, sz_t space )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:
        {
            bcore_static_array_s* arr = obj;
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
                vd_t old_data  = arr->data;
                sz_t old_space = arr->space;
                arr->data = bcore_un_alloc( unit_size, NULL, 0, space, &arr->space );
                for( sz_t i = 0; i < arr->size; i++ )
                {
                    instance_p->move( instance_p, ( u0_t* )arr->data + unit_size * i, ( u0_t* )old_data + unit_size * i );
                }
                bcore_un_alloc( unit_size, old_data, old_space, 0, NULL );
            }
        }
        break;

        case BCORE_CAPS_TYPED_ARRAY:
        {
            bcore_typed_array_s* arr = obj;
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
                vd_t old_data  = arr->data;
                sz_t old_space = arr->space;
                arr->data = bcore_un_alloc( unit_size, NULL, 0, space, &arr->space );
                for( sz_t i = 0; i < arr->size; i++ )
                {
                    instance_p->move( instance_p, ( u0_t* )arr->data + unit_size * i, ( u0_t* )old_data + unit_size * i );
                }
                bcore_un_alloc( unit_size, old_data, old_space, 0, NULL );
            }
        }
        break;

        case BCORE_CAPS_STATIC_LINK_ARRAY:
        {
            bcore_static_link_array_s* arr = obj;
            const bcore_inst_s* instance_p = p->item_p;
            while( arr->size > space )
            {
                arr->size--;
                instance_p->discard( instance_p, arr->data[ arr->size ] );
            }
            arr->data = bcore_un_alloc( sizeof( vd_t ), arr->data, arr->space, space, &arr->space );
        }
        break;

        case BCORE_CAPS_TYPED_LINK_ARRAY:
        {
            bcore_typed_link_array_s* arr = obj;
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

        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            bcore_aware_link_array_s* arr = obj;
            while( arr->size > space )
            {
                arr->size--;
                bcore_inst_aware_discard( arr->data[ arr->size ] );
            }
            arr->data = bcore_un_alloc( sizeof( vd_t ), arr->data, arr->space, space, &arr->space );
        }
        break;

        default: ERR( "invalid caps_type (%"PRIu32")", ( u2_t )p->caps_type );
    }
}

/**********************************************************************************************************************/

static void set_size( const bcore_array_s* p, vd_t o, sz_t size )
{
    sz_t space = p->get_space( p, o );
    if( size > space ) p->set_space( p, o, ( size <= space * 2 ) ? space * 2 : size );

    vd_t obj = ( u0_t* )o + p->caps_offset;
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:
        {
            bcore_static_array_s* arr = obj;
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

        case BCORE_CAPS_TYPED_ARRAY:
        {
            bcore_typed_array_s* arr = obj;
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

        case BCORE_CAPS_STATIC_LINK_ARRAY:
        {
            bcore_static_link_array_s* arr = obj;
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

        case BCORE_CAPS_TYPED_LINK_ARRAY:
        {
            bcore_typed_link_array_s* arr = obj;
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

        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            bcore_aware_link_array_s* arr = obj;
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

        default: ERR( "invalid caps_type (%"PRIu32")", ( u2_t )p->caps_type );
    }
}

/**********************************************************************************************************************/

static vc_t get_c_static( const bcore_array_s* p, vc_t o, sz_t index )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    const bcore_static_array_s* arr = obj;
    return ( index < arr->size ) ? ( u0_t* )arr->data + p->item_p->size * index : NULL;
}

static vc_t get_c_typed( const bcore_array_s* p, vc_t o, sz_t index )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    const bcore_typed_array_s* arr = obj;
    if( index < arr->size )
    {
        return ( u0_t* )arr->data + bcore_inst_s_get_typed( arr->type )->size * index;
    }
    else
    {
        return NULL;
    }
}

static vc_t get_c_static_link( const bcore_array_s* p, vc_t o, sz_t index )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    const bcore_static_link_array_s* arr = obj;
    return ( index < arr->size ) ? arr->data[ index ] : NULL;
}

static vc_t get_c_typed_link( const bcore_array_s* p, vc_t o, sz_t index )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    const bcore_typed_link_array_s* arr = obj;
    return ( index < arr->size ) ? arr->data[ index ] : NULL;
}

static vc_t get_c_aware_link( const bcore_array_s* p, vc_t o, sz_t index )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    const bcore_aware_link_array_s* arr = obj;
    return ( index < arr->size ) ? arr->data[ index ] : NULL;
}

/**********************************************************************************************************************/

static vd_t get_d_static( const bcore_array_s* p, vd_t o, sz_t index )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_static_array_s* arr = obj;
    return ( index < arr->size ) ? ( u0_t* )arr->data + p->item_p->size * index : NULL;
}

static vd_t get_d_typed( const bcore_array_s* p, vd_t o, sz_t index )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    const bcore_typed_array_s* arr = obj;
    if( index < arr->size )
    {
        return ( u0_t* )arr->data + bcore_inst_s_get_typed( arr->type )->size * index;
    }
    else
    {
        return NULL;
    }
}

static vd_t get_d_static_link( const bcore_array_s* p, vd_t o, sz_t index )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_static_link_array_s* arr = obj;
    return ( index < arr->size ) ? arr->data[ index ] : NULL;
}

static vd_t get_d_typed_link( const bcore_array_s* p, vd_t o, sz_t index )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_typed_link_array_s* arr = obj;
    return ( index < arr->size ) ? arr->data[ index ] : NULL;
}

static vd_t get_d_aware_link( const bcore_array_s* p, vd_t o, sz_t index )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_aware_link_array_s* arr = obj;
    return ( index < arr->size ) ? arr->data[ index ] : NULL;
}

/**********************************************************************************************************************/

static vd_t set_c_static( const bcore_array_s* p, vd_t o, sz_t index, vc_t src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_static_array_s* arr = obj;
    if( index >= arr->size ) set_size( p, o, index + 1 );
    const bcore_inst_s* instance_p = p->item_p;
    vd_t dst = ( u0_t* )arr->data + instance_p->size * index;
    if( src ) instance_p->copy( instance_p, dst, src );
    return dst;
}

static vd_t set_c_typed( const bcore_array_s* p, vd_t o, sz_t index, vc_t src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_typed_array_s* arr = obj;
    if( index >= arr->size ) set_size( p, o, index + 1 );
    const bcore_inst_s* item_p = bcore_inst_s_get_typed( arr->type );
    vd_t dst = ( u0_t* )arr->data + item_p->size * index;
    item_p->copy( item_p, dst, src );
    return dst;
}

static vd_t set_c_static_link( const bcore_array_s* p, vd_t o, sz_t index, vc_t src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_static_link_array_s* arr = obj;
    if( index >= arr->size ) set_size( p, o, index + 1 );
    const bcore_inst_s* instance_p = p->item_p;
    vd_t* dst = &arr->data[ index ];
    if( *dst ) instance_p->discard( instance_p, *dst );
    *dst = instance_p->clone( instance_p, src );
    return *dst;
}

static vd_t set_c_typed_link( const bcore_array_s* p, vd_t o, sz_t index, vc_t src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_typed_link_array_s* arr = obj;
    if( index >= arr->size ) set_size( p, o, index + 1 );
    vd_t* dst = &arr->data[ index ];
    if( *dst ) bcore_inst_typed_discard( arr->type, *dst );
    *dst = bcore_inst_typed_clone( arr->type, src );
    return *dst;
}

static vd_t set_c_aware_link( const bcore_array_s* p, vd_t o, sz_t index, vc_t src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_aware_link_array_s* arr = obj;
    if( index >= arr->size ) set_size( p, o, index + 1 );
    vd_t* dst = &arr->data[ index ];
    if( *dst ) bcore_inst_aware_discard( *dst );
    *dst = bcore_inst_aware_clone( src );
    return *dst;
}

/**********************************************************************************************************************/

static vd_t set_d_static( const bcore_array_s* p, vd_t o, sz_t index, vd_t src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_static_array_s* arr = obj;
    if( index >= arr->size ) set_size( p, o, index + 1 );
    const bcore_inst_s* instance_p = p->item_p;
    vd_t dst = ( u0_t* )arr->data + instance_p->size * index;
    if( src )
    {
        instance_p->copy( instance_p, dst, src );
        instance_p->discard( instance_p, src );
    }
    return dst;
}

static vd_t set_d_typed( const bcore_array_s* p, vd_t o, sz_t index, vd_t src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_typed_array_s* arr = obj;
    if( index >= arr->size ) set_size( p, o, index + 1 );
    const bcore_inst_s* item_p = bcore_inst_s_get_typed( arr->type );
    vd_t dst = ( u0_t* )arr->data + item_p->size * index;
    item_p->copy( item_p, dst, src );
    item_p->discard( item_p, src );
    return dst;
}

static vd_t set_d_static_link( const bcore_array_s* p, vd_t o, sz_t index, vd_t src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_static_link_array_s* arr = obj;
    if( index >= arr->size ) set_size( p, o, index + 1 );
    const bcore_inst_s* instance_p = p->item_p;
    vd_t* dst = &arr->data[ index ];
    if( *dst ) instance_p->discard( instance_p, *dst );
    *dst = src;
    return *dst;
}

static vd_t set_d_typed_link( const bcore_array_s* p, vd_t o, sz_t index, vd_t src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_typed_link_array_s* arr = obj;
    if( index >= arr->size ) set_size( p, o, index + 1 );
    vd_t* dst = &arr->data[ index ];
    if( *dst ) bcore_inst_typed_discard( arr->type, *dst );
    *dst = src;
    return *dst;
}

static vd_t set_d_aware_link( const bcore_array_s* p, vd_t o, sz_t index, vd_t src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_aware_link_array_s* arr = obj;
    if( index >= arr->size ) set_size( p, o, index + 1 );
    vd_t* dst = &arr->data[ index ];
    if( *dst ) bcore_inst_aware_discard( *dst );
    *dst = src;
    return *dst;
}

/**********************************************************************************************************************/

static vc_t get_c_first( const bcore_array_s* p, vc_t o )
{
    return p->get_c( p, o, 0 );
}

/**********************************************************************************************************************/

static vd_t get_d_first( const bcore_array_s* p, vd_t o )
{
    return p->get_d( p, o, 0 );
}

/**********************************************************************************************************************/

static vc_t get_c_last( const bcore_array_s* p, vc_t o )
{
    return p->get_c( p, o, p->get_size( p, o ) - 1 );
}

/**********************************************************************************************************************/

static vd_t get_d_last( const bcore_array_s* p, vd_t o )
{
    return p->get_d( p, o, p->get_size( p, o ) - 1 );
}

/**********************************************************************************************************************/

static vd_t push_c( const bcore_array_s* p, vd_t o, vc_t src )
{
    return p->set_c( p, o, p->get_size( p, o ), src );
}

/**********************************************************************************************************************/

static vd_t push_d( const bcore_array_s* p, vd_t o, vd_t src )
{
    return p->set_d( p, o, p->get_size( p, o ), src );
}

/**********************************************************************************************************************/

static void pop( const bcore_array_s* p, vd_t o )
{
    sz_t size = p->get_size( p, o );
    if( size > 0 ) p->set_size( p, o, size - 1 );
}

/**********************************************************************************************************************/

static tp_t get_gtype( const bcore_array_s* p, vc_t o )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return p->item_p->o_type;
        case BCORE_CAPS_TYPED_ARRAY:       return ( ( const bcore_typed_array_s*       )obj )->type;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return p->item_p->o_type;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return ( ( const bcore_typed_link_array_s*  )obj )->type;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return 0;
        default: ERR( "invalid caps_type (%"PRIu32")", ( u2_t )p->caps_type );
    }
    return 0;
}

static tp_t get_itype( const bcore_array_s* p, vc_t o, sz_t index )
{
    if( p->caps_type == BCORE_CAPS_AWARE_LINK_ARRAY )
    {
        vc_t obj = p->get_c( p, o, index );
        return ( obj ) ? *( aware_t* )obj : 0;
    }
    else
    {
        return get_gtype( p, o );
    }
}

/**********************************************************************************************************************/

static void set_gtype( const bcore_array_s* p, vd_t o, tp_t type )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:
        {
            if( p->item_p->o_type == type ) break;
            ERR( "cannot change type on static-array" );
        }
        break;

        case BCORE_CAPS_TYPED_ARRAY:
        {
            bcore_typed_array_s* arr = obj;
            if( arr->type == type ) break;
            if( arr->size > 0 ) ERR( "array size (%zu) must be zero for a type change", arr->size );
            arr->type = type;
        }
        break;

        case BCORE_CAPS_STATIC_LINK_ARRAY:
        {
            if( p->item_p->o_type == type ) break;
            ERR( "cannot change type on static-link-array" );
        }
        break;

        case BCORE_CAPS_TYPED_LINK_ARRAY:
        {
            bcore_typed_link_array_s* arr = obj;
            if( arr->type == type ) break;
            if( arr->size > 0 ) ERR( "array size (%zu) must be zero for a type change", arr->size );
            arr->type = type;
        }
        break;

        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            bcore_aware_link_array_s* arr = obj;
            if( arr->size > 0 ) ERR( "cannot change type on aware-link-array of non-zero size" );
        }
        break;

        default:
        {
            ERR( "invalid caps_type (%"PRIu32")", ( u2_t )p->caps_type );
        }
        break;
    }
}

/**********************************************************************************************************************/

static vc_t get_c_data( const bcore_array_s* p, vc_t o )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return ( ( const bcore_static_array_s*      )obj )->data;
        case BCORE_CAPS_TYPED_ARRAY:       return ( ( const bcore_typed_array_s*       )obj )->data;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return ( ( const bcore_static_link_array_s* )obj )->data;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return ( ( const bcore_typed_link_array_s*  )obj )->data;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return ( ( const bcore_aware_link_array_s*  )obj )->data;
        default: ERR( "invalid caps_type (%"PRIu32")", ( u2_t )p->caps_type );
    }
    return NULL;
}

static vd_t get_d_data( const bcore_array_s* p, vd_t o )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return ( ( bcore_static_array_s*      )obj )->data;
        case BCORE_CAPS_TYPED_ARRAY:       return ( ( bcore_typed_array_s*       )obj )->data;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return ( ( bcore_static_link_array_s* )obj )->data;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return ( ( bcore_typed_link_array_s*  )obj )->data;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return ( ( bcore_aware_link_array_s*  )obj )->data;
        default: ERR( "invalid caps_type (%"PRIu32")", ( u2_t )p->caps_type );
    }
    return NULL;
}

static bool is_linked( const bcore_array_s* p )
{
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return false;
        case BCORE_CAPS_TYPED_ARRAY:       return false;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return true;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return true;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return true;
        default: ERR( "invalid caps_type (%"PRIu32")", ( u2_t )p->caps_type );
    }
    return false;
}

static bool is_typed( const bcore_array_s* p )
{
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return false;
        case BCORE_CAPS_TYPED_ARRAY:       return true;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return false;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return true;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return false;
        default: ERR( "invalid caps_type (%"PRIu32")", ( u2_t )p->caps_type );
    }
    return false;
}

static bool is_aware( const bcore_array_s* p )
{
    return p->caps_type == BCORE_CAPS_AWARE_LINK_ARRAY;
}

static sz_t get_unit_size( const bcore_array_s* p, vc_t o )
{
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return p->item_p->size;
        case BCORE_CAPS_TYPED_ARRAY:
        {
            vc_t obj = ( u0_t* )o + p->caps_offset;
            const bcore_typed_array_s* arr = obj;
            if( !arr->type ) ERR( "type-zero array: cannot obtain unit size" );
            return bcore_inst_s_get_typed( arr->type )->size;
        }
        case BCORE_CAPS_STATIC_LINK_ARRAY: return sizeof( vd_t );
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return sizeof( vd_t );
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return sizeof( vd_t );
        default: ERR( "invalid caps_type (%"PRIu32")", ( u2_t )p->caps_type );
    }
    return 0;
}

/**********************************************************************************************************************/

static bcore_array_s* create_from_self( const bcore_flect_self_s** p_self )
{
    assert( p_self != NULL );
    const bcore_flect_self_s* self = *p_self;
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
        if( bcore_flect_caps_is_array( body->data[ i ].flect_item->caps ) )
        {
            o->caps_type = body->data[ i ].flect_item->caps;
            o->caps_offset = body->data[ i ].offset;
            o->item_p = body->data[ i ].perspective;
            found = true;
            break;
        }
    }
    if( !found ) ERR( "'%s' has no array", ifnameof( self->type ) );

    switch( o->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:
        {
            o->get_size  = get_size_static;
            o->get_space = get_space_static;
            o->get_c     = get_c_static;
            o->get_d     = get_d_static;
            o->set_c     = set_c_static;
            o->set_d     = set_d_static;
            if( !o->item_p ) ERR( "item_p is NULL on static-array" );
        }
        break;

        case BCORE_CAPS_TYPED_ARRAY:
        {
            o->get_size  = get_size_typed;
            o->get_space = get_space_typed;
            o->get_c     = get_c_typed;
            o->get_d     = get_d_typed;
            o->set_c     = set_c_typed;
            o->set_d     = set_d_typed;
        }
        break;

        case BCORE_CAPS_STATIC_LINK_ARRAY:
        {
            o->get_size   = get_size_static_link;
            o->get_space  = get_space_static_link;
            o->get_c      = get_c_static_link;
            o->get_d      = get_d_static_link;
            o->set_c      = set_c_static_link;
            o->set_d      = set_d_static_link;
            if( !o->item_p ) ERR( "item_p is NULL on static-link-array" );
        }
        break;

        case BCORE_CAPS_TYPED_LINK_ARRAY:
        {
            o->get_size  = get_size_typed_link;
            o->get_space = get_space_typed_link;
            o->get_c     = get_c_typed_link;
            o->get_d     = get_d_typed_link;
            o->set_c     = set_c_typed_link;
            o->set_d     = set_d_typed_link;
        }
        break;

        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            o->get_size  = get_size_aware_link;
            o->get_space = get_space_aware_link;
            o->get_c     = get_c_aware_link;
            o->get_d     = get_d_aware_link;
            o->set_c     = set_c_aware_link;
            o->set_d     = set_d_aware_link;
        }
        break;

        default: ERR( "invalid caps_type %"PRIu32, ( u2_t )o->caps_type );
    }

    o->set_size      = set_size;
    o->set_space     = set_space;
    o->get_c_first   = get_c_first;
    o->get_d_first   = get_d_first;
    o->get_c_last    = get_c_last;
    o->get_d_last    = get_d_last;
    o->push_c        = push_c;
    o->push_d        = push_d;
    o->pop           = pop;
    o->get_itype     = get_itype;
    o->get_gtype     = get_gtype;
    o->set_gtype     = set_gtype;
    o->get_c_data    = get_c_data;
    o->get_d_data    = get_d_data;
    o->is_linked     = is_linked;
    o->is_typed      = is_typed;
    o->is_aware      = is_aware;
    o->get_unit_size = get_unit_size;
    return o;
}

bcore_flect_self_s* bcore_array_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_array_s" ), sizeof( bcore_array_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )array_s_init,             "bcore_fp_init",                    "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )array_s_down,             "bcore_fp_down",                    "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )array_s_create,           "bcore_fp_create",                  "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )array_s_discard,          "bcore_fp_discard",                 "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )array_s_create_signature, "bcore_spect_fp_create_signature",  "create_signature" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,         "bcore_spect_fp_create_from_self",  "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bcore_array_s* bcore_array_s_get_typed( tp_t o_type )
{
    return bcore_spect_get_typed( typeof( "bcore_array_s" ), o_type );
}

/**********************************************************************************************************************/

static void test_string_array( sc_t type_sc )
{
    vd_t arr = bcore_inst_typed_create( typeof( type_sc ) );
    const bcore_array_s* arr_p = bcore_array_s_get_aware( arr );

    arr_p->set_gtype( arr_p, arr, typeof( "bcore_string_s" ) );
    arr_p->set_size( arr_p, arr, 5 );
    arr_p->set_d( arr_p, arr, 0, bcore_string_s_createf( "test line a" ) );
    arr_p->set_d( arr_p, arr, 1, bcore_string_s_createf( "some nonsense: sakjd" ) );
    arr_p->set_d( arr_p, arr, 2, bcore_string_s_createf( "some nonsense: dspaud" ) );
    arr_p->set_d( arr_p, arr, 7, bcore_string_s_createf( "test line x" ) );
    arr_p->push_d( arr_p, arr, bcore_string_s_createf( "test line p" ) );
    arr_p->set_space( arr_p, arr, 20 );

    ASSERT( bcore_array_max_index( arr_p, arr, 0, -1, 1 ) == 7 );
    bcore_array_sort( arr_p, arr, 0, -1, -1 );
    ASSERT( bcore_array_max_index( arr_p, arr, 0, -1, 1 ) == 0 );

    ASSERT( bcore_string_s_cmp_sc( ( const bcore_string_s* )arr_p->get_c( arr_p, arr, 0 ), "test line x" ) == 0 );
    ASSERT( bcore_string_s_cmp_sc( ( const bcore_string_s* )arr_p->get_c( arr_p, arr, 1 ), "test line p" ) == 0 );
    ASSERT( bcore_string_s_cmp_sc( ( const bcore_string_s* )arr_p->get_c( arr_p, arr, 2 ), "test line a" ) == 0 );
    ASSERT( bcore_string_s_cmp_sc( ( const bcore_string_s* )arr_p->get_c( arr_p, arr, 3 ), "some nonsense: sakjd" ) == 0 );
    ASSERT( bcore_string_s_cmp_sc( ( const bcore_string_s* )arr_p->get_c( arr_p, arr, 4 ), "some nonsense: dspaud" ) == 0 );
    ASSERT( arr_p->get_size( arr_p, arr ) == 9 );
    bcore_inst_aware_discard( arr );
}

bcore_string_s* bcore_spect_array_selftest( void )
{
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( "string_array = { aware_t _; bcore_string_s [] string_arr; }", 0 ) );
    vd_t arr = bcore_inst_typed_create( typeof( "string_array" ) );
    const bcore_array_s* arr_p = bcore_array_s_get_aware( arr );

    arr_p->push_d( arr_p, arr, bcore_string_s_createf( "string_static_array      = { aware_t _; bcore_string_s   [] arr; }" ) );
    arr_p->push_d( arr_p, arr, bcore_string_s_createf( "string_static_link_array = { aware_t _; bcore_string_s * [] arr; }" ) );
    arr_p->push_d( arr_p, arr, bcore_string_s_createf( "string_typed_array       = { aware_t _; typed            [] arr; }" ) );
    arr_p->push_d( arr_p, arr, bcore_string_s_createf( "string_typed_link_array  = { aware_t _; typed *          [] arr; }" ) );
    arr_p->push_d( arr_p, arr, bcore_string_s_createf( "string_aware_link_array  = { aware_t _; aware *          [] arr; }" ) );

    for( sz_t i = 0; i < arr_p->get_size( arr_p, arr ); i++ )
    {
        const bcore_string_s* code = arr_p->get_c( arr_p, arr, i );
        bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( code->sc, 0 );
        bcore_flect_define_self_c( self );
        test_string_array( nameof( self->type ) );
        bcore_flect_self_s_discard( self );
    }

    bcore_inst_aware_discard( arr );

    return NULL;
}

/**********************************************************************************************************************/

tp_t bcore_static_array_type_of( tp_t type )
{
    sc_t name = bcore_name_try_name( type );
    if( !name ) ERR( "type %u has no name", type );
    bcore_string_s* arr_name = bcore_string_s_createf( "%s__static_array", name );
    tp_t arr_type = typeof( arr_name->sc );
    if( !bcore_flect_try_self( arr_type ) )
    {
        bcore_string_s* code = bcore_string_s_createf( "%s = { %s []; }", arr_name->sc, name );
        bcore_flect_parse( code, 0 );
        bcore_string_s_discard( code );

    }
    bcore_string_s_discard( arr_name );
    return arr_type;
}

/**********************************************************************************************************************/

tp_t bcore_static_link_array_type_of( tp_t type )
{
    sc_t name = bcore_name_try_name( type );
    if( !name ) ERR( "type %u has no name", type );
    bcore_string_s* arr_name = bcore_string_s_createf( "%s__static_link_array", name );
    tp_t arr_type = typeof( arr_name->sc );
    if( !bcore_flect_try_self( arr_type ) )
    {
        bcore_string_s* code = bcore_string_s_createf( "%s = { %s * []; }", arr_name->sc, name );
        bcore_flect_parse( code, 0 );
        bcore_string_s_discard( code );
    }
    bcore_string_s_discard( arr_name );
    return arr_type;
}

/**********************************************************************************************************************/

bl_t bcore_array_is_static( const bcore_array_s* p )
{
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return true;
        case BCORE_CAPS_TYPED_ARRAY:       return false;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return true;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return false;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return false;
        default: ERR( "Unhandled encapsulation '%s'", bcore_flect_caps_e_sc( p->caps_type ) );
    }
    return false;
}

bl_t bcore_array_is_mono_typed(  const bcore_array_s* p )
{
    return p->caps_type != BCORE_CAPS_AWARE_LINK_ARRAY;
}

bl_t bcore_array_is_mutable_mono_typed(  const bcore_array_s* p )
{
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return false;
        case BCORE_CAPS_TYPED_ARRAY:       return true;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return false;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return true;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return false;
        default: ERR( "Unhandled encapsulation '%s'", bcore_flect_caps_e_sc( p->caps_type ) );
    }
    return false;
}

bl_t bcore_array_is_multi_typed( const bcore_array_s* p )
{
    return p->caps_type == BCORE_CAPS_AWARE_LINK_ARRAY;
}

bl_t bcore_array_is_of_aware( const bcore_array_s* p )
{
    return p->caps_type == BCORE_CAPS_AWARE_LINK_ARRAY;
}

bl_t bcore_array_is_of_links( const bcore_array_s* p )
{
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return false;
        case BCORE_CAPS_TYPED_ARRAY:       return false;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return true;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return true;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return true;
        default: ERR( "Unhandled encapsulation '%s'", bcore_flect_caps_e_sc( p->caps_type ) );
    }
    return false;
}

tp_t bcore_array_get_static_type( const bcore_array_s* p )
{
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return p->item_p->o_type;
        case BCORE_CAPS_TYPED_ARRAY:       return 0;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return p->item_p->o_type;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return 0;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return 0;
        default: ERR( "Unhandled encapsulation '%s'", bcore_flect_caps_e_sc( p->caps_type ) );
    }
    return 0;
}

tp_t bcore_array_get_mono_type( const bcore_array_s* p, vc_t o )
{
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return p->item_p->o_type;
        case BCORE_CAPS_TYPED_ARRAY:       return ( ( const bcore_typed_array_s*       )( ( u0_t* )o + p->caps_offset ) )->type;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return p->item_p->o_type;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return ( ( const bcore_typed_link_array_s*  )( ( u0_t* )o + p->caps_offset ) )->type;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return 0;
        default: ERR( "Unhandled encapsulation '%s'", bcore_flect_caps_e_sc( p->caps_type ) );
    }
    return 0;
}

tp_t bcore_array_get_type( const bcore_array_s* p, vc_t o, sz_t index )
{
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return p->item_p->o_type;
        case BCORE_CAPS_TYPED_ARRAY:       return ( ( const bcore_typed_array_s*       )( ( u0_t* )o + p->caps_offset ) )->type;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return p->item_p->o_type;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return ( ( const bcore_typed_link_array_s*  )( ( u0_t* )o + p->caps_offset ) )->type;
        case BCORE_CAPS_AWARE_LINK_ARRAY: { vc_t obj = p->get_c( p, o, index ); return ( obj ) ? *( aware_t* )obj : 0; }
        default: ERR( "Unhandled encapsulation '%s'", bcore_flect_caps_e_sc( p->caps_type ) );
    }
    return 0;
}

vc_t bcore_array_max( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, s2_t order )
{
    sz_t size = p->get_size( p, o );
    sz_t end_l = end < size ? end : size;
    if( start >= end_l ) return NULL;

    if( p->is_aware( p ) )
    {
        vc_t* data = ( vc_t* )p->get_c_data( p, o );
        sz_t  idx = start;
        for( sz_t i = start + 1; i < end_l; i++ ) idx = ( bcore_compare_aware( data[ idx ], data[ i ] ) * order > 0 ) ? i : idx;
        return idx < end_l ? data[ idx ] : NULL;
    }

    const bcore_compare_s* cmp_p = bcore_compare_s_get_typed( p->get_gtype( p, o ) );

    if( p->is_linked( p ) )
    {
        vc_t* data = ( vc_t* )p->get_c_data( p, o );
        sz_t  idx = start;
        for( sz_t i = start + 1; i < end_l; i++ ) idx = ( bcore_compare_spect( cmp_p, data[ idx ], data[ i ] ) * order > 0 ) ? i : idx;
        return idx < end_l ? data[ idx ] : NULL;
    }

    // else
    {
        sz_t unit_size = p->get_unit_size( p, o );
        vc_t src = ( u0_t* )p->get_c_data( p, o ) + unit_size * start;
        vc_t cur = src;
        for( sz_t i = start + 1; i < end_l; i++ )
        {
            src = ( u0_t* )src + unit_size;
            cur = ( bcore_compare_spect( cmp_p, cur, src ) * order > 0 ) ? src : cur;
        }
        return cur;
    }
}

/**********************************************************************************************************************/

sz_t bcore_array_max_index( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, s2_t order )
{
    sz_t size = p->get_size( p, o );
    sz_t end_l = end < size ? end : size;
    if( start >= end_l ) return end_l;


    if( p->is_aware( p ) )
    {
        vc_t* data = ( vc_t* )p->get_c_data( p, o );
        sz_t  idx = start;
        for( sz_t i = start + 1; i < end_l; i++ ) idx = ( bcore_compare_aware( data[ idx ], data[ i ] ) * order > 0 ) ? i : idx;
        return idx;
    }

    const bcore_compare_s* cmp_p = bcore_compare_s_get_typed( p->get_gtype( p, o ) );

    if( p->is_linked( p ) )
    {
        vc_t* data = ( vc_t* )p->get_c_data( p, o );
        sz_t  idx = start;
        for( sz_t i = start + 1; i < end_l; i++ ) idx = ( bcore_compare_spect( cmp_p, data[ idx ], data[ i ] ) * order > 0 ) ? i : idx;
        return idx;
    }


    // else
    {
        sz_t unit_size = p->get_unit_size( p, o );
        vc_t src = ( u0_t* )p->get_c_data( p, o ) + unit_size * start;
        vc_t cur = src;
        sz_t idx = start;
        for( sz_t i = start + 1; i < end_l; i++ )
        {
            src = ( u0_t* )src + unit_size;
            if( bcore_compare_spect( cmp_p, cur, src ) * order > 0 )
            {
                cur = src;
                idx = i;
            }
        }
        return idx;
    }
}

/**********************************************************************************************************************/

static void buf_sort_spect_link( vc_t* data, sz_t size, vc_t* buf, const bcore_compare_s* cmp_p, s2_t order )
{
    if( size < 2 ) return;
    sz_t size1 = size >> 1;
    buf_sort_spect_link( data,         size1       , buf, cmp_p, order );
    buf_sort_spect_link( data + size1, size - size1, buf, cmp_p, order );
    bcore_memcpy( buf, data, size1 * sizeof( vc_t ) );
    for( sz_t i = 0, w = 0, r = size1; i < size1; )
    {
        data[ w++ ] = ( r == size || bcore_compare_spect( cmp_p, buf[ i ], data[ r ] ) * order > 0 ) ? buf[ i++ ] : data[ r++ ];
    }
}

static void buf_sort_aware( vc_t* data, sz_t size, vc_t* buf, s2_t order )
{
    if( size < 2 ) return;
    sz_t size1 = size >> 1;
    buf_sort_aware( data,         size1       , buf, order );
    buf_sort_aware( data + size1, size - size1, buf, order );
    bcore_memcpy( buf, data, size1 * sizeof( vc_t ) );
    for( sz_t i = 0, w = 0, r = size1; i < size1; )
    {
        data[ w++ ] = ( r == size || bcore_compare_aware( buf[ i ], data[ r ] ) * order > 0 ) ? buf[ i++ ] : data[ r++ ];
    }
}

static void buf_sort_spect_empl( const bcore_inst_s* p, vd_t data, sz_t size, vd_t buf, const bcore_compare_s* cmp_p, s2_t order )
{
    if( size < 2 ) return;

    sz_t unit_size = p->size;
    sz_t size1 = size >> 1;
    buf_sort_spect_empl( p, data,                              size1       , buf, cmp_p, order );
    buf_sort_spect_empl( p, ( u0_t* )data + size1 * unit_size, size - size1, buf, cmp_p, order );

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
        if( r == size || bcore_compare_spect( cmp_p, src1, src2 ) * order > 0 )
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

void bcore_array_sort( const bcore_array_s* p, vd_t o, sz_t start, sz_t end, s2_t order )
{
    sz_t size = p->get_size( p, o );
    sz_t end_l = end < size ? end : size;
    if( start >= end_l ) return;
    sz_t range = end_l - start;

    if( p->is_aware( p ) )
    {
        vc_t* data = ( vc_t* )p->get_d_data( p, o ) + start;
        vc_t* buf = bcore_un_alloc( sizeof( vd_t ), NULL, 0, range >> 1, NULL );
        buf_sort_aware( data, range, buf, order );
        bcore_un_alloc( sizeof( vd_t ), buf, range >> 1, 0, NULL );
        return;
    }

    const bcore_compare_s* cmp_p = bcore_compare_s_get_typed( p->get_gtype( p, o ) );

    if( p->is_linked( p ) )
    {
        vc_t* data = ( vc_t* )p->get_d_data( p, o ) + start;
        vc_t* buf = bcore_un_alloc( sizeof( vd_t ), NULL, 0, range >> 1, NULL );
        buf_sort_spect_link( data, range, buf, cmp_p, order );
        bcore_un_alloc( sizeof( vd_t ), buf, range >> 1, 0, NULL );
        return;
    }

    /// else
    {
        const bcore_inst_s* item_p = p->is_typed( p ) ? bcore_inst_s_get_typed( p->get_gtype( p, o ) ) : p->item_p;
        sz_t unit_size = item_p->size;
        vd_t data = ( u0_t* )p->get_d_data( p, o ) + start * unit_size;

        sz_t range1 = range >> 1;
        vd_t buf = bcore_un_alloc( unit_size, NULL, 0, range1, NULL );
        for( sz_t i = 0; i < range1; i++ ) item_p->init( item_p, ( u0_t* )buf + i * unit_size );

        buf_sort_spect_empl( item_p, data, range, buf, cmp_p, order );

        for( sz_t i = 0; i < range1; i++ ) item_p->down( item_p, ( u0_t* )buf + i * unit_size );
        bcore_un_alloc( unit_size, buf, range1, 0, NULL );
        return;
    }
}

/**********************************************************************************************************************/
