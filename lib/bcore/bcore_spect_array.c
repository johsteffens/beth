/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_array.h"
#include "bcore_spect_compare.h"
#include "bcore_spect.h"
#include "bcore_quicktypes.h"

#define NPX( name ) bcore_array_##name

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

void bcore_array_spect_set_space( const bcore_array_s* p, vd_t o, sz_t space )
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

void bcore_array_spect_set_size( const bcore_array_s* p, vd_t o, sz_t size )
{
    sz_t space = bcore_array_spect_get_space( p, o );
    if( size > space ) bcore_array_spect_set_space( p, o, ( size <= space * 2 ) ? space * 2 : size );

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

static rf_s get_static( const bcore_array_s* p, vc_t o, sz_t index )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    const bcore_static_array_s* arr = obj;
    return ( index < arr->size ) ? rf_wd( ( u0_t* )arr->data + p->item_p->size * index, p->item_p->o_type ) : rf_null();
}

static rf_s get_typed( const bcore_array_s* p, vc_t o, sz_t index )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    const bcore_typed_array_s* arr = obj;
    if( index < arr->size )
    {
        return rf_wd( ( u0_t* )arr->data + bcore_inst_s_get_typed( arr->type )->size * index, arr->type );
    }
    else
    {
        return rf_null();
    }
}

static rf_s get_static_link( const bcore_array_s* p, vc_t o, sz_t index )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    const bcore_static_link_array_s* arr = obj;
    return ( index < arr->size ) ? rf_wd( arr->data[ index ], p->item_p->o_type ) : rf_null();
}

static rf_s get_typed_link( const bcore_array_s* p, vc_t o, sz_t index )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    const bcore_typed_link_array_s* arr = obj;
    return ( index < arr->size ) ? rf_wd( arr->data[ index ], arr->type ) : rf_null();
}

static rf_s get_aware_link( const bcore_array_s* p, vc_t o, sz_t index )
{
    vc_t obj = ( u0_t* )o + p->caps_offset;
    const bcore_aware_link_array_s* arr = obj;
    if( index < arr->size )
    {
        vd_t item = arr->data[ index ];
        return item ? rf_wd( item, *( aware_t* )item ) : rf_null();
    }
    return rf_null();
}

/**********************************************************************************************************************/

static rf_s set_static( const bcore_array_s* p, vd_t o, sz_t index, rf_s src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_static_array_s* arr = obj;
    if( index >= arr->size ) bcore_array_spect_set_size( p, o, index + 1 );
    const bcore_inst_s* inst_p = p->item_p;
    vd_t dst = ( u0_t* )arr->data + inst_p->size * index;
    if( src.o )
    {
        if( src.t == inst_p->o_type )
        {
            bcore_inst_spect_copy( inst_p, dst, src.o );
        }
        else
        {
            bcore_inst_spect_copy_typed( inst_p, dst, src.t, src.o );
        }
    }
    rf_s_down( src );
    return rf_wd( dst, inst_p->o_type );
}

static rf_s set_typed( const bcore_array_s* p, vd_t o, sz_t index, rf_s src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_typed_array_s* arr = obj;
    if( arr->type == 0 ) arr->type = src.t;
    if( index >= arr->size ) bcore_array_spect_set_size( p, o, index + 1 );
    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( arr->type );
    vd_t dst = ( u0_t* )arr->data + inst_p->size * index;
    if( src.o )
    {
        if( src.t == inst_p->o_type )
        {
            bcore_inst_spect_copy( inst_p, dst, src.o );
        }
        else
        {
            bcore_inst_spect_copy_typed( inst_p, dst, src.t, src.o );
        }
    }
    rf_s_down( src );
    return rf_wd( dst, inst_p->o_type );
}

static rf_s set_static_link( const bcore_array_s* p, vd_t o, sz_t index, rf_s src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_static_link_array_s* arr = obj;
    if( index >= arr->size ) bcore_array_spect_set_size( p, o, index + 1 );
    const bcore_inst_s* inst_p = p->item_p;
    vd_t* dst = &arr->data[ index ];
    if( *dst ) bcore_inst_spect_discard( inst_p, *dst );
    *dst = NULL;
    if( src.o )
    {
        if( src.t == inst_p->o_type )
        {
           *dst = src.s ? src.o : bcore_inst_spect_clone( inst_p, src.o );
            src.s = false;
        }
        else
        {
            *dst = bcore_inst_spect_create_typed( inst_p, src.t, src.o );
        }
    }
    rf_s_down( src );
    return rf_wd( *dst, inst_p->o_type );
}

static rf_s set_typed_link( const bcore_array_s* p, vd_t o, sz_t index, rf_s src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_typed_link_array_s* arr = obj;
    if( arr->type == 0 ) arr->type = src.t;
    if( index >= arr->size ) bcore_array_spect_set_size( p, o, index + 1 );
    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( arr->type );
    vd_t* dst = &arr->data[ index ];
    if( *dst ) bcore_inst_spect_discard( inst_p, *dst );
    *dst = NULL;

    if( src.o )
    {
        if( src.t == inst_p->o_type )
        {
           *dst = src.s ? src.o : bcore_inst_spect_clone( inst_p, src.o );
            src.s = false;
        }
        else
        {
            *dst = bcore_inst_spect_create_typed( inst_p, src.t, src.o );
        }
    }

    rf_s_down( src );
    return rf_wd( *dst, inst_p->o_type );
}

static rf_s set_aware_link( const bcore_array_s* p, vd_t o, sz_t index, rf_s src )
{
    vd_t obj = ( u0_t* )o + p->caps_offset;
    bcore_aware_link_array_s* arr = obj;
    if( index >= arr->size ) bcore_array_spect_set_size( p, o, index + 1 );
    vd_t* dst = &arr->data[ index ];
    if( *dst ) bcore_inst_aware_discard( *dst );
    *dst = NULL;

    if( src.t )
    {
        const bcore_inst_s* inst_p = bcore_inst_s_get_typed( src.t );
        if( inst_p->aware )
        {
            *dst = src.s ? src.o : bcore_inst_spect_clone( inst_p, src.o );
            src.s = false;
        }
        else
        {
            ERR( "Cannot convert '%s' to self-aware object", ifnameof( src.t ) );
        }
    }
    else
    {
        *dst = src.s ? src.o : bcore_inst_aware_clone( src.o );
        src.s = false;
    }

    rf_s_down( src );
    return rf_wd( *dst, *dst ? *(aware_t*)*dst : 0 );
}

/**********************************************************************************************************************/

static inline sz_t asz( const bcore_array_s* p, vc_t o ) { return bcore_array_spect_get_size( p, o ); }

rf_s NPX(spect_get_first )( const NPX(s)* p, vc_t o           ) { return NPX(spect_get    )( p, o, 0 ); }
rf_s NPX(spect_get_last  )( const NPX(s)* p, vc_t o           ) { return NPX(spect_get    )( p, o, asz( p, o ) - 1 ); }
rf_s NPX(spect_push      )( const NPX(s)* p, vd_t o, rf_s src ) { return NPX(spect_set    )( p, o, asz( p, o ), src ); }
rf_s NPX(spect_push_s3   )( const NPX(s)* p, vd_t o, s3_t val ) { return NPX(spect_set_s3 )( p, o, asz( p, o ), val ); }
rf_s NPX(spect_push_u3   )( const NPX(s)* p, vd_t o, u3_t val ) { return NPX(spect_set_u3 )( p, o, asz( p, o ), val ); }
rf_s NPX(spect_push_f3   )( const NPX(s)* p, vd_t o, f3_t val ) { return NPX(spect_set_f3 )( p, o, asz( p, o ), val ); }
rf_s NPX(spect_push_sz   )( const NPX(s)* p, vd_t o, sz_t val ) { return NPX(spect_set_sz )( p, o, asz( p, o ), val ); }
rf_s NPX(spect_push_sc   )( const NPX(s)* p, vd_t o, sc_t val ) { return NPX(spect_set_sc )( p, o, asz( p, o ), val ); }
rf_s NPX(spect_push_bl   )( const NPX(s)* p, vd_t o, bl_t val ) { return NPX(spect_set_bl )( p, o, asz( p, o ), val ); }

/**********************************************************************************************************************/

void bcore_array_spect_pop( const bcore_array_s* p, vd_t o )
{
    sz_t size = bcore_array_spect_get_size( p, o );
    if( size > 0 ) bcore_array_spect_set_size( p, o, size - 1 );
}

/**********************************************************************************************************************/

void bcore_array_spect_set_gtype( const bcore_array_s* p, vd_t o, tp_t type )
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

vc_t bcore_array_spect_get_c_data( const bcore_array_s* p, vc_t o )
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

vd_t bcore_array_spect_get_d_data( const bcore_array_s* p, vd_t o )
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

sz_t bcore_array_spect_get_unit_size( const bcore_array_s* p, vc_t o )
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
            o->get_size    = get_size_static;
            o->get_space   = get_space_static;
            o->get         = get_static;
            o->set         = set_static;
//            o->get_c       = get_c_static;
//            o->get_d       = get_d_static;
//            o->set_c       = set_c_static;
//            o->set_typed_c = set_typed_c_static;
//            o->set_d       = set_d_static;
            if( !o->item_p ) ERR( "item_p is NULL on static-array" );
        }
        break;

        case BCORE_CAPS_TYPED_ARRAY:
        {
            o->get_size    = get_size_typed;
            o->get_space   = get_space_typed;
            o->get         = get_typed;
            o->set         = set_typed;
//            o->get_c       = get_c_typed;
//            o->get_d       = get_d_typed;
//            o->set_c       = set_c_typed;
//            o->set_typed_c = set_typed_c_typed;
//            o->set_d       = set_d_typed;
        }
        break;

        case BCORE_CAPS_STATIC_LINK_ARRAY:
        {
            o->get_size    = get_size_static_link;
            o->get_space   = get_space_static_link;
            o->get         = get_static_link;
            o->set         = set_static_link;
//            o->get_c       = get_c_static_link;
//            o->get_d       = get_d_static_link;
//            o->set_c       = set_c_static_link;
//            o->set_typed_c = set_typed_c_static_link;
//            o->set_d       = set_d_static_link;
            if( !o->item_p ) ERR( "item_p is NULL on static-link-array" );
        }
        break;

        case BCORE_CAPS_TYPED_LINK_ARRAY:
        {
            o->get_size    = get_size_typed_link;
            o->get_space   = get_space_typed_link;
            o->get         = get_typed_link;
            o->set         = set_typed_link;
//            o->get_c       = get_c_typed_link;
//            o->get_d       = get_d_typed_link;
//            o->set_c       = set_c_typed_link;
//            o->set_typed_c = set_typed_c_typed_link;
//            o->set_d       = set_d_typed_link;
        }
        break;

        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            o->get_size    = get_size_aware_link;
            o->get_space   = get_space_aware_link;
            o->get         = get_aware_link;
            o->set         = set_aware_link;
//            o->get_c       = get_c_aware_link;
//            o->get_d       = get_d_aware_link;
//            o->set_c       = set_c_aware_link;
//            o->set_typed_c = set_typed_c_aware_link;
//            o->set_d       = set_d_aware_link;
        }
        break;

        default: ERR( "invalid caps_type %"PRIu32, ( u2_t )o->caps_type );
    }

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

tp_t bcore_static_array_type_of( tp_t type )
{
    sc_t name = bcore_name_try_name( type );
    if( !name ) ERR( "type %u has no name", type );
    bcore_string_s* arr_name = bcore_string_s_createf( "%s__static_array", name );
    tp_t arr_type = typeof( arr_name->sc );
    if( !bcore_flect_try_self( arr_type ) )
    {
        bcore_string_s* code = bcore_string_s_createf( "%s = { %s []; }", arr_name->sc, name );
        bcore_flect_define_parse( code, 0 );
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
        bcore_flect_define_parse( code, 0 );
        bcore_string_s_discard( code );
    }
    bcore_string_s_discard( arr_name );
    return arr_type;
}

/**********************************************************************************************************************/

sz_t bcore_array_spect_get_size   ( const bcore_array_s* p, vc_t o )                                 { return p->get_size( p, o ); }
sz_t bcore_array_spect_get_space  ( const bcore_array_s* p, vc_t o )                                 { return p->get_space( p, o ); }
rf_s bcore_array_spect_get        ( const bcore_array_s* p, vc_t o, sz_t index )                     { return p->get( p, o, index ); }
rf_s bcore_array_spect_set        ( const bcore_array_s* p, vd_t o, sz_t index, rf_s src )           { return p->set( p, o, index, src ); }
rf_s bcore_array_spect_set_s3     ( const bcore_array_s* p, vd_t o, sz_t index, s3_t val )           { return p->set( p, o, index, rf_wc( &val, TYPEOF_s3_t ) ); }
rf_s bcore_array_spect_set_u3     ( const bcore_array_s* p, vd_t o, sz_t index, u3_t val )           { return p->set( p, o, index, rf_wc( &val, TYPEOF_u3_t ) ); }
rf_s bcore_array_spect_set_f3     ( const bcore_array_s* p, vd_t o, sz_t index, f3_t val )           { return p->set( p, o, index, rf_wc( &val, TYPEOF_f3_t ) ); }
rf_s bcore_array_spect_set_sz     ( const bcore_array_s* p, vd_t o, sz_t index, sz_t val )           { return p->set( p, o, index, rf_wc( &val, TYPEOF_sz_t ) ); }
rf_s bcore_array_spect_set_sc     ( const bcore_array_s* p, vd_t o, sz_t index, sc_t val )           { return p->set( p, o, index, rf_wc( &val, TYPEOF_sc_t ) ); }
rf_s bcore_array_spect_set_bl     ( const bcore_array_s* p, vd_t o, sz_t index, bl_t val )           { return p->set( p, o, index, rf_wc( &val, TYPEOF_bl_t ) ); }

bl_t bcore_array_spect_is_static( const bcore_array_s* p )
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

bl_t bcore_array_spect_is_mono_typed(  const bcore_array_s* p )
{
    return p->caps_type != BCORE_CAPS_AWARE_LINK_ARRAY;
}

bl_t bcore_array_spect_is_mutable_mono_typed(  const bcore_array_s* p )
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

bl_t bcore_array_spect_is_multi_typed( const bcore_array_s* p )
{
    return p->caps_type == BCORE_CAPS_AWARE_LINK_ARRAY;
}

bl_t bcore_array_spect_is_of_aware( const bcore_array_s* p )
{
    return p->caps_type == BCORE_CAPS_AWARE_LINK_ARRAY;
}

bl_t bcore_array_spect_is_of_links( const bcore_array_s* p )
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

tp_t bcore_array_spect_get_static_type( const bcore_array_s* p )
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

tp_t bcore_array_spect_get_mono_type( const bcore_array_s* p, vc_t o )
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

tp_t bcore_array_spect_get_type( const bcore_array_s* p, vc_t o, sz_t index )
{
    switch( p->caps_type )
    {
        case BCORE_CAPS_STATIC_ARRAY:      return p->item_p->o_type;
        case BCORE_CAPS_TYPED_ARRAY:       return ( ( const bcore_typed_array_s*       )( ( u0_t* )o + p->caps_offset ) )->type;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return p->item_p->o_type;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return ( ( const bcore_typed_link_array_s*  )( ( u0_t* )o + p->caps_offset ) )->type;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return p->get( p, o, index ).t;
        default: ERR( "Unhandled encapsulation '%s'", bcore_flect_caps_e_sc( p->caps_type ) );
    }
    return 0;
}

vc_t bcore_array_spect_max( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, s2_t order )
{
    sz_t size = p->get_size( p, o );
    sz_t end_l = end < size ? end : size;
    if( start >= end_l ) return NULL;

    if( bcore_array_spect_is_of_aware( p ) )
    {
        vc_t* data = ( vc_t* )bcore_array_spect_get_c_data( p, o );
        sz_t  idx = start;
        for( sz_t i = start + 1; i < end_l; i++ ) idx = ( bcore_compare_aware( data[ idx ], data[ i ] ) * order > 0 ) ? i : idx;
        return idx < end_l ? data[ idx ] : NULL;
    }

    const bcore_compare_s* cmp_p = bcore_compare_s_get_typed( bcore_array_spect_get_mono_type( p, o ) );

    if( bcore_array_spect_is_of_links( p ) )
    {
        vc_t* data = ( vc_t* )bcore_array_spect_get_c_data( p, o );
        sz_t  idx = start;
        for( sz_t i = start + 1; i < end_l; i++ ) idx = ( bcore_compare_spect( cmp_p, data[ idx ], data[ i ] ) * order > 0 ) ? i : idx;
        return idx < end_l ? data[ idx ] : NULL;
    }

    // else
    {
        sz_t unit_size = bcore_array_spect_get_unit_size( p, o );
        vc_t src = ( u0_t* )bcore_array_spect_get_c_data( p, o ) + unit_size * start;
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

sz_t bcore_array_spect_max_index( const bcore_array_s* p, vc_t o, sz_t start, sz_t end, s2_t order )
{
    sz_t size = p->get_size( p, o );
    sz_t end_l = end < size ? end : size;
    if( start >= end_l ) return end_l;


    if( bcore_array_spect_is_of_aware( p ) )
    {
        vc_t* data = ( vc_t* )bcore_array_spect_get_c_data( p, o );
        sz_t  idx = start;
        for( sz_t i = start + 1; i < end_l; i++ ) idx = ( bcore_compare_aware( data[ idx ], data[ i ] ) * order > 0 ) ? i : idx;
        return idx;
    }

    const bcore_compare_s* cmp_p = bcore_compare_s_get_typed( bcore_array_spect_get_mono_type( p, o ) );

    if( bcore_array_spect_is_of_links( p ) )
    {
        vc_t* data = ( vc_t* )bcore_array_spect_get_c_data( p, o );
        sz_t  idx = start;
        for( sz_t i = start + 1; i < end_l; i++ ) idx = ( bcore_compare_spect( cmp_p, data[ idx ], data[ i ] ) * order > 0 ) ? i : idx;
        return idx;
    }


    // else
    {
        sz_t unit_size = bcore_array_spect_get_unit_size( p, o );
        vc_t src = ( u0_t* )bcore_array_spect_get_c_data( p, o ) + unit_size * start;
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

void bcore_array_spect_sort( const bcore_array_s* p, vd_t o, sz_t start, sz_t end, s2_t order )
{
    sz_t size = p->get_size( p, o );
    sz_t end_l = end < size ? end : size;
    if( start >= end_l ) return;
    sz_t range = end_l - start;

    if( bcore_array_spect_is_of_aware( p ) )
    {
        vc_t* data = ( vc_t* )bcore_array_spect_get_d_data( p, o ) + start;
        vc_t* buf = bcore_un_alloc( sizeof( vd_t ), NULL, 0, range >> 1, NULL );
        buf_sort_aware( data, range, buf, order );
        bcore_un_alloc( sizeof( vd_t ), buf, range >> 1, 0, NULL );
        return;
    }

    const bcore_compare_s* cmp_p = bcore_compare_s_get_typed( bcore_array_spect_get_mono_type( p, o ) );

    if( bcore_array_spect_is_of_links( p ) )
    {
        vc_t* data = ( vc_t* )bcore_array_spect_get_d_data( p, o ) + start;
        vc_t* buf = bcore_un_alloc( sizeof( vd_t ), NULL, 0, range >> 1, NULL );
        buf_sort_spect_link( data, range, buf, cmp_p, order );
        bcore_un_alloc( sizeof( vd_t ), buf, range >> 1, 0, NULL );
        return;
    }

    /// else
    {
        const bcore_inst_s* item_p = bcore_array_spect_is_mutable_mono_typed( p ) ? bcore_inst_s_get_typed( bcore_array_spect_get_mono_type( p, o ) ) : p->item_p;
        sz_t unit_size = item_p->size;
        vd_t data = ( u0_t* )bcore_array_spect_get_d_data( p, o ) + start * unit_size;

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

static inline const bcore_array_s* atpd( tp_t tp ) { return bcore_array_s_get_typed( tp ); }

sz_t NPX(typed_get_size             )( tp_t tp, vc_t o                                 ) { return NPX(spect_get_size             )( atpd( tp ), o             ); }
sz_t NPX(typed_get_space            )( tp_t tp, vc_t o                                 ) { return NPX(spect_get_space            )( atpd( tp ), o             ); }
rf_s NPX(typed_get                  )( tp_t tp, vc_t o, sz_t index                     ) { return NPX(spect_get                  )( atpd( tp ), o, index      ); }
rf_s NPX(typed_set                  )( tp_t tp, vd_t o, sz_t index, rf_s src           ) { return NPX(spect_set                  )( atpd( tp ), o, index, src ); }
rf_s NPX(typed_set_s3               )( tp_t tp, vd_t o, sz_t index, s3_t val           ) { return NPX(spect_set_s3               )( atpd( tp ), o, index, val ); }
rf_s NPX(typed_set_u3               )( tp_t tp, vd_t o, sz_t index, u3_t val           ) { return NPX(spect_set_u3               )( atpd( tp ), o, index, val ); }
rf_s NPX(typed_set_f3               )( tp_t tp, vd_t o, sz_t index, f3_t val           ) { return NPX(spect_set_f3               )( atpd( tp ), o, index, val ); }
rf_s NPX(typed_set_sz               )( tp_t tp, vd_t o, sz_t index, sz_t val           ) { return NPX(spect_set_sz               )( atpd( tp ), o, index, val ); }
rf_s NPX(typed_set_sc               )( tp_t tp, vd_t o, sz_t index, sc_t val           ) { return NPX(spect_set_sc               )( atpd( tp ), o, index, val ); }
rf_s NPX(typed_set_bl               )( tp_t tp, vd_t o, sz_t index, bl_t val           ) { return NPX(spect_set_bl               )( atpd( tp ), o, index, val ); }
void NPX(typed_set_size             )( tp_t tp, vd_t o, sz_t size                      ) {        NPX(spect_set_size             )( atpd( tp ), o, size       ); }
void NPX(typed_set_space            )( tp_t tp, vd_t o, sz_t space                     ) {        NPX(spect_set_space            )( atpd( tp ), o, space      ); }
rf_s NPX(typed_get_first            )( tp_t tp, vc_t o                                 ) { return NPX(spect_get_first            )( atpd( tp ), o             ); }
rf_s NPX(typed_get_last             )( tp_t tp, vc_t o                                 ) { return NPX(spect_get_last             )( atpd( tp ), o             ); }
rf_s NPX(typed_push                 )( tp_t tp, vd_t o, rf_s src                       ) { return NPX(spect_push                 )( atpd( tp ), o, src        ); }
void NPX(typed_pop                  )( tp_t tp, vd_t o                                 ) {        NPX(spect_pop                  )( atpd( tp ), o             ); }
void NPX(typed_set_gtype            )( tp_t tp, vd_t o, tp_t type                      ) {        NPX(spect_set_gtype            )( atpd( tp ), o, type       ); }
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

sz_t NPX(aware_get_size             )( vc_t o                                 ) { return NPX(typed_get_size             )( *( aware_t* )o, o             ); }
sz_t NPX(aware_get_space            )( vc_t o                                 ) { return NPX(typed_get_space            )( *( aware_t* )o, o             ); }
rf_s NPX(aware_get                  )( vc_t o, sz_t index                     ) { return NPX(typed_get                  )( *( aware_t* )o, o, index      ); }
rf_s NPX(aware_set                  )( vd_t o, sz_t index, rf_s src           ) { return NPX(typed_set                  )( *( aware_t* )o, o, index, src ); }
rf_s NPX(aware_set_s3               )( vd_t o, sz_t index, s3_t val           ) { return NPX(typed_set_s3               )( *( aware_t* )o, o, index, val ); }
rf_s NPX(aware_set_u3               )( vd_t o, sz_t index, u3_t val           ) { return NPX(typed_set_u3               )( *( aware_t* )o, o, index, val ); }
rf_s NPX(aware_set_f3               )( vd_t o, sz_t index, f3_t val           ) { return NPX(typed_set_f3               )( *( aware_t* )o, o, index, val ); }
rf_s NPX(aware_set_sz               )( vd_t o, sz_t index, sz_t val           ) { return NPX(typed_set_sz               )( *( aware_t* )o, o, index, val ); }
rf_s NPX(aware_set_sc               )( vd_t o, sz_t index, sc_t val           ) { return NPX(typed_set_sc               )( *( aware_t* )o, o, index, val ); }
rf_s NPX(aware_set_bl               )( vd_t o, sz_t index, bl_t val           ) { return NPX(typed_set_bl               )( *( aware_t* )o, o, index, val ); }
void NPX(aware_set_size             )( vd_t o, sz_t size                      ) {        NPX(typed_set_size             )( *( aware_t* )o, o, size       ); }
void NPX(aware_set_space            )( vd_t o, sz_t space                     ) {        NPX(typed_set_space            )( *( aware_t* )o, o, space      ); }
rf_s NPX(aware_get_first            )( vc_t o                                 ) { return NPX(typed_get_first            )( *( aware_t* )o, o             ); }
rf_s NPX(aware_get_last             )( vc_t o                                 ) { return NPX(typed_get_last             )( *( aware_t* )o, o             ); }
rf_s NPX(aware_push                 )( vd_t o, rf_s src                       ) { return NPX(typed_push                 )( *( aware_t* )o, o, src        ); }
void NPX(aware_pop                  )( vd_t o                                 ) {        NPX(typed_pop                  )( *( aware_t* )o, o             ); }
void NPX(aware_set_gtype            )( vd_t o, tp_t type                      ) {        NPX(typed_set_gtype            )( *( aware_t* )o, o, type       ); }
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

/**********************************************************************************************************************/
// testing, debugging

static void test_string_array( sc_t type_sc )
{
    vd_t arr = bcore_inst_typed_create( typeof( type_sc ) );
    const bcore_array_s* arr_p = bcore_array_s_get_aware( arr );

    bcore_array_spect_set_gtype( arr_p, arr, typeof( "bcore_string_s" ) );
    bcore_array_spect_set_size( arr_p, arr, 5 );
    arr_p->set( arr_p, arr, 0, rf_asd( bcore_string_s_createf( "test line a" ) ) );
    arr_p->set( arr_p, arr, 1, rf_asd( bcore_string_s_createf( "some nonsense: sakjd" ) ) );
    arr_p->set( arr_p, arr, 2, rf_asd( bcore_string_s_createf( "some nonsense: dspaud" ) ) );
    arr_p->set( arr_p, arr, 7, rf_asd( bcore_string_s_createf( "test line x" ) ) );
    bcore_array_spect_push( arr_p, arr, rf_asd( bcore_string_s_createf( "test line p" ) ) );
    bcore_array_spect_set_space( arr_p, arr, 20 );

    ASSERT( bcore_array_spect_max_index( arr_p, arr, 0, -1, 1 ) == 7 );
    bcore_array_spect_sort( arr_p, arr, 0, -1, -1 );
    ASSERT( bcore_array_spect_max_index( arr_p, arr, 0, -1, 1 ) == 0 );

    ASSERT( bcore_string_s_cmp_sc( ( const bcore_string_s* )arr_p->get( arr_p, arr, 0 ).o, "test line x" ) == 0 );
    ASSERT( bcore_string_s_cmp_sc( ( const bcore_string_s* )arr_p->get( arr_p, arr, 1 ).o, "test line p" ) == 0 );
    ASSERT( bcore_string_s_cmp_sc( ( const bcore_string_s* )arr_p->get( arr_p, arr, 2 ).o, "test line a" ) == 0 );
    ASSERT( bcore_string_s_cmp_sc( ( const bcore_string_s* )arr_p->get( arr_p, arr, 3 ).o, "some nonsense: sakjd" ) == 0 );
    ASSERT( bcore_string_s_cmp_sc( ( const bcore_string_s* )arr_p->get( arr_p, arr, 4 ).o, "some nonsense: dspaud" ) == 0 );
    ASSERT( arr_p->get_size( arr_p, arr ) == 9 );
    bcore_inst_aware_discard( arr );
}

bcore_string_s* bcore_spect_array_selftest( void )
{
    bcore_flect_define_self_d( bcore_flect_self_s_build_parse_sc( "string_array = { aware_t _; bcore_string_s [] string_arr; }", 0 ) );
    vd_t arr = bcore_inst_typed_create( typeof( "string_array" ) );
    const bcore_array_s* arr_p = bcore_array_s_get_aware( arr );

    bcore_array_spect_push( arr_p, arr, rf_asd( bcore_string_s_createf( "string_static_array      = { aware_t _; bcore_string_s   [] arr; }" ) ) );
    bcore_array_spect_push( arr_p, arr, rf_asd( bcore_string_s_createf( "string_static_link_array = { aware_t _; bcore_string_s * [] arr; }" ) ) );
    bcore_array_spect_push( arr_p, arr, rf_asd( bcore_string_s_createf( "string_typed_array       = { aware_t _; typed            [] arr; }" ) ) );
    bcore_array_spect_push( arr_p, arr, rf_asd( bcore_string_s_createf( "string_typed_link_array  = { aware_t _; typed *          [] arr; }" ) ) );
    bcore_array_spect_push( arr_p, arr, rf_asd( bcore_string_s_createf( "string_aware_link_array  = { aware_t _; aware *          [] arr; }" ) ) );

    for( sz_t i = 0; i < arr_p->get_size( arr_p, arr ); i++ )
    {
        const bcore_string_s* code = arr_p->get( arr_p, arr, i ).o;
        bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( code->sc, 0 );
        bcore_flect_define_self_c( self );
        test_string_array( nameof( self->type ) );
        bcore_flect_self_s_discard( self );
    }

    bcore_inst_aware_discard( arr );

    return NULL;
}

/**********************************************************************************************************************/

