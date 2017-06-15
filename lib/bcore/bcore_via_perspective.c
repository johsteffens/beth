/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_via_perspective.h"
#include "bcore_flect.h"


/**********************************************************************************************************************/
// bcore_via_s

void bcore_via_s_down( bcore_via_s* o );

void bcore_via_s_init( bcore_via_s* o )
{
    bcore_memzero( o, sizeof( bcore_via_s ) );
    bcore_perspective_s_init( &o->_, bcore_via_s_down );
}

void bcore_via_s_down( bcore_via_s* o )
{
    if( o->vitem_arr ) bcore_un_alloc( sizeof( bcore_via_s       ), o->vitem_arr, o->size, 0, NULL );
    if( o->inst_arr  ) bcore_un_alloc( sizeof( bcore_instance_s* ), o->inst_arr,  o->size, 0, NULL );
}

bcore_via_s* bcore_via_s_create()
{
    bcore_via_s* o = bcore_alloc( NULL, sizeof( bcore_via_s ) );
    bcore_via_s_init( o );
    return o;
}

void bcore_via_s_discard( bcore_via_s* o )
{
    if( !o ) return;
    bcore_via_s_down( o );
    bcore_free( o );
}

/**********************************************************************************************************************/

static vc_t iget_c( const bcore_via_s* p, vc_t o, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            return ( vc_t )( ( u0_t* )o + vitem->offs );
        }

        case BCORE_CAPS_STATIC_LINK:
        case BCORE_CAPS_TYPED_LINK:
        case BCORE_CAPS_AWARE_LINK:
        {
            return *( vc_t* )( vc_t )( ( u0_t* )o + vitem->offs );
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            return ( vc_t )( ( u0_t* )o + vitem->offs );
        }

        case BCORE_CAPS_EXTERNAL_DATA:
        {
            return vitem->d_ptr;
        }

        case BCORE_CAPS_EXTERNAL_FUNC:
        {
            return &vitem->f_ptr;
        }

        default: break;
    }

    return NULL;
}

static vd_t iget_d( const bcore_via_s* p, vd_t o, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            return ( vd_t )( ( u0_t* )o + vitem->offs );
        }

        case BCORE_CAPS_STATIC_LINK:
        case BCORE_CAPS_TYPED_LINK:
        case BCORE_CAPS_AWARE_LINK:
        {
            return *( vd_t* )( vd_t )( ( u0_t* )o + vitem->offs );
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            return ( vd_t )( ( u0_t* )o + vitem->offs );
        }

        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
        default: break;
    }

    return NULL;
}

static vd_t iset_c( const bcore_via_s* p, vd_t o, sz_t index, vc_t src )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s*    vitem  = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            vd_t dst = ( u0_t* )o + vitem->offs;
            const bcore_instance_s* inst_p = p->inst_arr[ index ];
            inst_p->copy( inst_p, dst, src );
            return dst;
        }

        case BCORE_CAPS_STATIC_LINK:
        {
            bcore_flect_caps_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            const bcore_instance_s* inst_p = p->inst_arr[ index ];
            if( dst->link ) inst_p->discard( inst_p, dst->link );
            dst->link = inst_p->clone( inst_p, src );
            return dst->link;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_flect_caps_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->type )
            {
                const bcore_instance_s* inst_p = bcore_instance_s_get_typed( dst->type );
                if( dst->link ) inst_p->discard( inst_p, dst->link );
                dst->link = inst_p->clone( inst_p, src );
            }
            else if( src )
            {
                ERR( "destination type is zero" );
            }
            return dst->link;
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            bcore_flect_caps_aware_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );

            if( dst->link )
            {
                const bcore_instance_s* inst_p = bcore_instance_s_get_aware( dst->link );
                inst_p->discard( inst_p, dst->link );
                dst->link = NULL;
            }

            if( src )
            {
                const bcore_instance_s* inst_p = bcore_instance_s_get_aware( src );
                dst->link = inst_p->clone( inst_p, src );
            }

            return dst->link;
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
        default:
        {
            ERR( "Function is not yet available for caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
        }
    }

    return NULL;
}

static vd_t iset_d( const bcore_via_s* p, vd_t o, sz_t index, vd_t src )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem  = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            vd_t dst = ( u0_t* )o + vitem->offs;
            const bcore_instance_s* inst_p = p->inst_arr[ index ];
            inst_p->copy( inst_p, dst, src );
            inst_p->discard( inst_p, src );
            return dst;
        }

        case BCORE_CAPS_STATIC_LINK:
        {
            bcore_flect_caps_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            const bcore_instance_s* inst_p = p->inst_arr[ index ];
            if( dst->link ) inst_p->discard( inst_p, dst->link );
            dst->link = src;
            return dst->link;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_flect_caps_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->type )
            {
                const bcore_instance_s* inst_p = bcore_instance_s_get_typed( dst->type );
                if( dst->link ) inst_p->discard( inst_p, dst->link );
                dst->link = src;
            }
            else if( src )
            {
                ERR( "destination type is zero" );
            }
            return dst->link;
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            bcore_flect_caps_aware_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link )
            {
                const bcore_instance_s* inst_p = bcore_instance_s_get_aware( dst->link );
                inst_p->discard( inst_p, dst->link );
            }
            dst->link = src;
            return dst->link;
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
        default:
        {
            ERR( "Function is not yet available for caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
        }
    }

    return NULL;
}

static const bcore_vitem_s* iget_vitem( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return &p->vitem_arr[ index ];
}

static const bcore_via_s* iget_via( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return p->vitem_arr[ index ].via;
}

static tp_t iget_type( const bcore_via_s* p, vc_t o, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        case BCORE_CAPS_STATIC_LINK:
        {
            return vitem->type;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            const bcore_flect_caps_typed_link_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            return dst->type;
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            const bcore_flect_caps_aware_link_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            if( dst->link )
            {
                return *( aware_t* )dst->link;
            }
            else
            {
                return 0;
            }
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            ERR( "Function is not yet available for caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
        }

        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
        {
            return vitem->type;
        }

        default: break;
    }

    return 0;
}

static vd_t icreate( const bcore_via_s* p, vc_t o, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem  = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            ERR( "Element is embedded" );
            return NULL;
        }

        case BCORE_CAPS_STATIC_LINK:
        {
            bcore_flect_caps_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            const bcore_instance_s* inst_p = p->inst_arr[ index ];
            if( dst->link ) inst_p->discard( inst_p, dst->link );
            dst->link = inst_p->create( inst_p );
            return dst->link;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_flect_caps_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->type )
            {
                const bcore_instance_s* inst_p = bcore_instance_s_get_typed( dst->type );
                if( dst->link ) inst_p->discard( inst_p, dst->link );
                dst->link = inst_p->create( inst_p );
            }
            else
            {
                ERR( "destination type is zero" );
            }
            return dst->link;
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            ERR( "Element is aware linked. Use function create_type." );
            return NULL;
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
        default:
        {
            ERR( "Function is not yet available for caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
        }
    }

    return NULL;
}

static vd_t icreate_type( const bcore_via_s* p, vc_t o, sz_t index, tp_t type )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem  = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            ERR( "Element is embedded" );
            return NULL;
        }

        case BCORE_CAPS_STATIC_LINK:
        {
            const bcore_instance_s* inst_p = p->inst_arr[ index ];
            if( inst_p->_.o_type != type )
            {
               ERR( "Element is static type '%s'. Requested type '%s'.", ifnameof( inst_p->_.o_type ), ifnameof( type ) );
            }

            bcore_flect_caps_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link ) inst_p->discard( inst_p, dst->link );
            dst->link = inst_p->create( inst_p );
            return dst->link;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_flect_caps_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link )
            {
                bcore_instance_typed_discard( dst->type, dst->link );
                dst->link = NULL;
            }
            dst->type = type;
            dst->link = bcore_instance_typed_create( dst->type );
            return dst->link;
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            bcore_flect_caps_aware_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link )
            {
                bcore_instance_aware_discard( dst->link );
                dst->link = NULL;
            }
            dst->link = bcore_instance_typed_create( type );
            return dst->link;
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
        default:
        {
            ERR( "Function is not yet available for caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
        }
    }

    return NULL;
}

static void idiscard( const bcore_via_s* p, vc_t o, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem  = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            ERR( "Element is embedded" );
        }

        case BCORE_CAPS_STATIC_LINK:
        {
            const bcore_instance_s* inst_p = p->inst_arr[ index ];
            bcore_flect_caps_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link ) inst_p->discard( inst_p, dst->link );
            dst->link = NULL;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_flect_caps_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link )
            {
                bcore_instance_typed_discard( dst->type, dst->link );
                dst->link = NULL;
            }
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            bcore_flect_caps_aware_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link )
            {
                bcore_instance_aware_discard( dst->link );
                dst->link = NULL;
            }
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
        default:
        {
            ERR( "Function is not yet available for caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
        }
    }
}

static vd_t idetach( const bcore_via_s* p, vc_t o, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem  = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            ERR( "Element is embedded" );
            return NULL;
        }

        case BCORE_CAPS_STATIC_LINK:
        {
            bcore_flect_caps_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            vd_t ret = dst->link;
            dst->link = NULL;
            return ret;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_flect_caps_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            vd_t ret = dst->link;
            dst->link = NULL;
            return ret;
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            bcore_flect_caps_aware_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            vd_t ret = dst->link;
            dst->link = NULL;
            return ret;
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
        default:
        {
            ERR( "Function is not yet available for caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
        }
    }

    return NULL;
}

/**********************************************************************************************************************/

static sz_t nget_index( const bcore_via_s* p, tp_t name )
{
    for( sz_t i = 0; i < p->size; i++ ) if( p->vitem_arr[ i ].name == name ) return i;
    ERR( "object '%s' has no element of name '%s'", ifnameof( p->inst_p->_.o_type ), ifnameof( name ) );
    return 0;
}

static vc_t nget_c( const bcore_via_s* p, vc_t o, tp_t name )
{
    return iget_c( p, o, nget_index( p, name ) );
}

static vd_t nget_d( const bcore_via_s* p, vd_t o, tp_t name )
{
    return iget_d( p, o, nget_index( p, name ) );
}

static vd_t nset_c( const bcore_via_s* p, vd_t o, tp_t name, vc_t src )
{
    return iset_c( p, o, nget_index( p, name ), src );
}

static vd_t nset_d( const bcore_via_s* p, vd_t o, tp_t name, vd_t src )
{
    return iset_d( p, o, nget_index( p, name ), src );
}

static const bcore_vitem_s* nget_vitem( const bcore_via_s* p, tp_t name )
{
    return iget_vitem( p, nget_index( p, name ) );
}

static const bcore_via_s* nget_via( const bcore_via_s* p, tp_t name )
{
    return iget_via( p, nget_index( p, name ) );
}

static tp_t nget_type( const bcore_via_s* p, vc_t o, tp_t name )
{
    return iget_type( p, o, nget_index( p, name ) );
}

static vd_t ncreate( const bcore_via_s* p, vc_t o, tp_t name )
{
    return icreate( p, o, nget_index( p, name ) );
}

static vd_t ncreate_type( const bcore_via_s* p, vc_t o, tp_t name, tp_t type )
{
    return icreate_type( p, o, nget_index( p, name ), type );
}

static void ndiscard( const bcore_via_s* p, vc_t o, tp_t name )
{
    idiscard( p, o, nget_index( p, name ) );
}

static vd_t ndetach( const bcore_via_s* p, vc_t o, tp_t name )
{
    return idetach( p, o, nget_index( p, name ) );
}

/**********************************************************************************************************************/

static bcore_via_s* create_from_self( const bcore_flect_self_s* self )
{
    bcore_via_s* o = bcore_via_s_create();
    o->_.p_type = bcore_name_enroll( "bcore_via_s" );
    o->_.o_type = self->type;

    const bcore_instance_s* inst_p = bcore_instance_s_get_typed( self->type );
    o->inst_p = inst_p;

    if( self->body )
    {
        for( sz_t i = 0; i < self->body->size; i++ )
        {
            switch( self->body->data[ i ].caps )
            {
                case BCORE_CAPS_EXTERNAL_DATA:
                case BCORE_CAPS_EXTERNAL_FUNC:
                o->size++;
                default: break;
            }
        }
    }

    if( inst_p->body ) o->size += inst_p->body->size;

    o->vitem_arr = bcore_u_alloc( sizeof( bcore_via_s       ), NULL, o->size, NULL );
    o->inst_arr  = bcore_u_alloc( sizeof( bcore_instance_s* ), NULL, o->size, NULL );
    bcore_memzero( o->vitem_arr, sizeof( bcore_via_s       ) * o->size );
    bcore_memzero( o->inst_arr,  sizeof( bcore_instance_s* ) * o->size );

    sz_t idx = 0;

    if( inst_p->body )
    {
        for( sz_t i = 0; i < inst_p->body->size; i++ )
        {
            const bcore_instance_item_s* inst_item = &inst_p->body->data[ i ];
            const bcore_flect_item_s*   flect_item = inst_item->flect_item;
            bcore_vitem_s* vitem = &o->vitem_arr[ idx ];
            vitem->type = flect_item->type;
            vitem->name = flect_item->name;
            vitem->caps = flect_item->caps;
            vitem->offs = inst_item->offset;
            if( vitem->type )
            {
                vitem->via  = bcore_via_s_get_typed( vitem->type );
                o->inst_arr[ idx ] = bcore_instance_s_get_typed( vitem->type );
            }
            idx++;
        }
    }

    if( self->body )
    {
        for( sz_t i = 0; i < self->body->size; i++ )
        {
            const bcore_flect_item_s* flect_item = &self->body->data[ i ];
            switch( flect_item->caps )
            {
                case BCORE_CAPS_EXTERNAL_DATA:
                {
                    bcore_vitem_s* vitem = &o->vitem_arr[ idx ];
                    vitem->type  = flect_item->type;
                    vitem->name  = flect_item->name;
                    vitem->caps  = flect_item->caps;
                    vitem->d_ptr = flect_item->d_ptr;
                    idx++;
                }
                break;

                case BCORE_CAPS_EXTERNAL_FUNC:
                {
                    bcore_vitem_s* vitem = &o->vitem_arr[ idx ];
                    vitem->type  = flect_item->type;
                    vitem->name  = flect_item->name;
                    vitem->caps  = flect_item->caps;
                    vitem->f_ptr = flect_item->f_ptr;
                    idx++;
                }
                break;

                default: break;
            }
        }
    }

    o->iget_c       = iget_c;
    o->iget_d       = iget_d;
    o->iset_c       = iset_c;
    o->iset_d       = iset_d;
    o->iget_vitem   = iget_vitem;
    o->iget_via     = iget_via;
    o->iget_type    = iget_type;
    o->icreate      = icreate;
    o->icreate_type = icreate_type;
    o->idiscard     = idiscard;
    o->idetach      = idetach;

    o->nget_index   = nget_index;
    o->nget_c       = nget_c;
    o->nget_d       = nget_d;
    o->nset_c       = nset_c;
    o->nset_d       = nset_d;
    o->nget_vitem   = nget_vitem;
    o->nget_via     = nget_via;
    o->nget_type    = nget_type;
    o->ncreate      = ncreate;
    o->ncreate_type = ncreate_type;
    o->ndiscard     = ndiscard;
    o->ndetach      = ndetach;

    return o;
}

/**********************************************************************************************************************/

const bcore_via_s* bcore_via_s_get_typed( u2_t o_type )
{
    u2_t p_type = typeof( "bcore_via_s" );
    const bcore_via_s* perspective = ( const bcore_via_s* )bcore_perspective_try_perspective( p_type, o_type );
    if( !perspective )
    {
        const bcore_flect_self_s* o_self = bcore_flect_get_self( o_type );
        bcore_via_s* new_perspective = create_from_self( o_self );
        bcore_perspective_enroll( p_type, o_type, ( bcore_perspective_s* )new_perspective );
        perspective = new_perspective;
    }
    return perspective;
}

/**********************************************************************************************************************/

bcore_string_s* bcore_via_perspective_selftest()
{
    bcore_string_s* s = bcore_string_s_create();
    bcore_flect_parse_sc( "specs = { sz_t size; u2_t param1; s2_t param2; }" );
    vd_t specs = bcore_instance_typed_create( typeof( "specs" ) );
    const bcore_via_s* specs_v = bcore_via_s_get_typed( typeof( "specs" ) );

    bcore_string_s_pushf( s, "size   = %zu\n", *( sz_t* )specs_v->nget_c( specs_v, specs, typeof( "size"   ) ) );
    bcore_string_s_pushf( s, "param1 = %u\n",  *( u2_t* )specs_v->nget_c( specs_v, specs, typeof( "param1" ) ) );
    bcore_string_s_pushf( s, "param2 = %i\n",  *( s2_t* )specs_v->nget_c( specs_v, specs, typeof( "param2" ) ) );

    {
        sz_t size   =  10;
        u2_t param1 = 200;
        s2_t param2 = -50;
        specs_v->nset_c( specs_v, specs, typeof( "size"   ), &size );
        specs_v->nset_c( specs_v, specs, typeof( "param1" ), &param1 );
        specs_v->nset_c( specs_v, specs, typeof( "param2" ), &param2 );
    }

    bcore_string_s_pushf( s, "size   = %zu\n", *( sz_t* )specs_v->nget_c( specs_v, specs, typeof( "size"   ) ) );
    bcore_string_s_pushf( s, "param1 = %u\n",  *( u2_t* )specs_v->nget_c( specs_v, specs, typeof( "param1" ) ) );
    bcore_string_s_pushf( s, "param2 = %i\n",  *( s2_t* )specs_v->nget_c( specs_v, specs, typeof( "param2" ) ) );

    return s;
}
