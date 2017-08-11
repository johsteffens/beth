/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_via.h"
#include "bcore_spect_array.h"
#include "bcore_flect.h"
#include "bcore_spect.h"


/**********************************************************************************************************************/
// bcore_via_s

static void via_s_down( bcore_via_s* o );

static void via_s_init( bcore_via_s* o )
{
    bcore_memzero( o, sizeof( bcore_via_s ) );
    o->p_type = typeof( "bcore_via_s" );
}

static void via_s_down( bcore_via_s* o )
{
    if( o->vitem_arr ) bcore_un_alloc( sizeof( bcore_via_s       ), o->vitem_arr, o->size, 0, NULL );
    if( o->inst_arr  ) bcore_un_alloc( sizeof( bcore_inst_s* ), o->inst_arr,  o->size, 0, NULL );
}

static bcore_via_s* via_s_create()
{
    bcore_via_s* o = bcore_alloc( NULL, sizeof( bcore_via_s ) );
    via_s_init( o );
    return o;
}

static void via_s_discard( bcore_via_s* o )
{
    if( !o ) return;
    via_s_down( o );
    bcore_free( o );
}

static bcore_signature_s* via_s_create_signature( bcore_via_s* o )
{
    return bcore_signature_s_create_an( 2, o->p_type, o->o_type );
}

/**********************************************************************************************************************/

static tp_t iget_name( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return p->vitem_arr[ index ].name;
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
            const bcore_typed_link_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            return dst->type;
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            const bcore_aware_link_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
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
//        case BCORE_CAPS_EXTERNAL_DATA:
//        case BCORE_CAPS_EXTERNAL_FUNC:
            return vitem->type;

        default: break;
    }

    return 0;
}

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
    const bcore_vitem_s* vitem  = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            vd_t dst = ( u0_t* )o + vitem->offs;
            const bcore_inst_s* inst_p = p->inst_arr[ index ];
            inst_p->copy( inst_p, dst, src );
            return dst;
        }

        case BCORE_CAPS_STATIC_LINK:
        {
            bcore_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            const bcore_inst_s* inst_p = p->inst_arr[ index ];
            if( dst->link ) inst_p->discard( inst_p, dst->link );
            dst->link = inst_p->clone( inst_p, src );
            return dst->link;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->type )
            {
                const bcore_inst_s* inst_p = bcore_inst_s_get_typed( dst->type );
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
            bcore_aware_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );

            if( dst->link )
            {
                const bcore_inst_s* inst_p = bcore_inst_s_get_aware( dst->link );
                inst_p->discard( inst_p, dst->link );
                dst->link = NULL;
            }

            if( src )
            {
                const bcore_inst_s* inst_p = bcore_inst_s_get_aware( src );
                dst->link = inst_p->clone( inst_p, src );
            }

            return dst->link;
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            const bcore_inst_s* inst_p = p->inst_arr[ index ];
            vd_t dst = ( ( u0_t* )o + vitem->offs );
            inst_p->copy( inst_p, dst, src );
            return dst;
        }

        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
            ERR( "External object '%s' cannot be changed though perspective %s", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        default:
            ERR( "Unsupported caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
            break;
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
            const bcore_inst_s* inst_p = p->inst_arr[ index ];
            inst_p->copy( inst_p, dst, src );
            inst_p->discard( inst_p, src );
            return dst;
        }

        case BCORE_CAPS_STATIC_LINK:
        {
            bcore_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            const bcore_inst_s* inst_p = p->inst_arr[ index ];
            if( dst->link ) inst_p->discard( inst_p, dst->link );
            dst->link = src;
            return dst->link;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->type )
            {
                const bcore_inst_s* inst_p = bcore_inst_s_get_typed( dst->type );
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
            bcore_aware_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link )
            {
                const bcore_inst_s* inst_p = bcore_inst_s_get_aware( dst->link );
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
            ERR( "This function cannot be used for array type '%s' (consider set_c)", bcore_flect_caps_e_sc( vitem->caps ) );
            break;

        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
            ERR( "External object '%s' cannot be changed though perspective %s", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        default:
            ERR( "Unsupported caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
            break;
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

static vd_t icreate( const bcore_via_s* p, vd_t o, sz_t index )
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
            bcore_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            const bcore_inst_s* inst_p = p->inst_arr[ index ];
            if( dst->link ) inst_p->discard( inst_p, dst->link );
            dst->link = inst_p->create( inst_p );
            return dst->link;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->type )
            {
                const bcore_inst_s* inst_p = bcore_inst_s_get_typed( dst->type );
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
            ERR( "Element is aware linked. Use function typed_create." );
            return NULL;
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
            ERR( "Use array perspective to change '%s'", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
            ERR( "External object '%s' cannot be changed though perspective %s", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        default:
            ERR( "Unsupported caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
            break;
    }

    return NULL;
}

static vd_t ityped_create( const bcore_via_s* p, vd_t o, sz_t index, tp_t type )
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
            const bcore_inst_s* inst_p = p->inst_arr[ index ];
            if( inst_p->o_type != type )
            {
               ERR( "Element is static type '%s'. Requested type '%s'.", ifnameof( inst_p->o_type ), ifnameof( type ) );
            }

            bcore_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link ) inst_p->discard( inst_p, dst->link );
            dst->link = inst_p->create( inst_p );
            return dst->link;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link )
            {
                bcore_inst_typed_discard( dst->type, dst->link );
                dst->link = NULL;
            }
            dst->type = type;
            dst->link = bcore_inst_typed_create( dst->type );
            return dst->link;
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            bcore_aware_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link )
            {
                bcore_inst_aware_discard( dst->link );
                dst->link = NULL;
            }
            dst->link = bcore_inst_typed_create( type );
            return dst->link;
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
            ERR( "Use array perspective to change '%s'", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
            ERR( "External object '%s' cannot be changed though perspective %s", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        default:
            ERR( "Unsupported caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
            break;
    }

    return NULL;
}

static void idiscard( const bcore_via_s* p, vd_t o, sz_t index )
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
            const bcore_inst_s* inst_p = p->inst_arr[ index ];
            bcore_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link ) inst_p->discard( inst_p, dst->link );
            dst->link = NULL;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link )
            {
                bcore_inst_typed_discard( dst->type, dst->link );
                dst->link = NULL;
            }
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            bcore_aware_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->link )
            {
                bcore_inst_aware_discard( dst->link );
                dst->link = NULL;
            }
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
            ERR( "Use array perspective to change '%s'", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
            ERR( "External object '%s' cannot be changed though perspective %s", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        default:
            ERR( "Unsupported caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
            break;
    }
}

static vd_t idetach( const bcore_via_s* p, vd_t o, sz_t index )
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
            bcore_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            vd_t ret = dst->link;
            dst->link = NULL;
            return ret;
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            vd_t ret = dst->link;
            dst->link = NULL;
            return ret;
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            bcore_aware_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            vd_t ret = dst->link;
            dst->link = NULL;
            return ret;
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
            ERR( "Cannot detach '%s'", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
            ERR( "External object '%s' cannot be changed though perspective %s", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        default:
            ERR( "Unsupported caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
            break;
    }

    return NULL;
}

/**********************************************************************************************************************/

static sz_t nget_index( const bcore_via_s* p, tp_t name )
{
    for( sz_t i = 0; i < p->size; i++ ) if( p->vitem_arr[ i ].name == name ) return i;
    ERR( "object '%s' has no element of name '%s'", ifnameof( p->o_type ), ifnameof( name ) );
    return 0;
}

static tp_t nget_type( const bcore_via_s* p, vc_t o, tp_t name )
{
    return iget_type( p, o, nget_index( p, name ) );
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

static vd_t ncreate( const bcore_via_s* p, vd_t o, tp_t name )
{
    return icreate( p, o, nget_index( p, name ) );
}

static vd_t ntyped_create( const bcore_via_s* p, vd_t o, tp_t name, tp_t type )
{
    return ityped_create( p, o, nget_index( p, name ), type );
}

static void ndiscard( const bcore_via_s* p, vd_t o, tp_t name )
{
    idiscard( p, o, nget_index( p, name ) );
}

static vd_t ndetach( const bcore_via_s* p, vd_t o, tp_t name )
{
    return idetach( p, o, nget_index( p, name ) );
}

/**********************************************************************************************************************/

bl_t bcore_via_is_array( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return bcore_flect_caps_is_array( p->vitem_arr[ index ].caps );
}

bl_t bcore_via_is_static( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    switch( p->vitem_arr[ index ].caps )
    {
        case BCORE_CAPS_STATIC:            return true;
        case BCORE_CAPS_STATIC_LINK:       return true;
        case BCORE_CAPS_TYPED_LINK:        return false;
        case BCORE_CAPS_AWARE_LINK:        return false;
        case BCORE_CAPS_STATIC_ARRAY:      return true;
        case BCORE_CAPS_TYPED_ARRAY:       return true;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return true;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return true;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return true;
        default: ERR( "Unhandled encapsulation '%s'", bcore_flect_caps_e_sc( p->vitem_arr[ index ].caps ) );
    }
    return false;
}

bl_t bcore_via_is_link( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    switch( p->vitem_arr[ index ].caps )
    {
        case BCORE_CAPS_STATIC:            return false;
        case BCORE_CAPS_STATIC_LINK:       return true;
        case BCORE_CAPS_TYPED_LINK:        return true;
        case BCORE_CAPS_AWARE_LINK:        return true;
        case BCORE_CAPS_STATIC_ARRAY:      return false;
        case BCORE_CAPS_TYPED_ARRAY:       return false;
        case BCORE_CAPS_STATIC_LINK_ARRAY: return false;
        case BCORE_CAPS_TYPED_LINK_ARRAY:  return false;
        case BCORE_CAPS_AWARE_LINK_ARRAY:  return false;
        default: ERR( "Unhandled encapsulation '%s'", bcore_flect_caps_e_sc( p->vitem_arr[ index ].caps ) );
    }
    return false;
}

const bcore_array_s* bcore_via_get_spect_array( const bcore_via_s* p, sz_t index )
{
    if( !bcore_flect_caps_is_array( p->vitem_arr[ index ].caps ) ) return NULL;
    return bcore_array_s_get_typed( p->vitem_arr[ index ].type );
}

/**********************************************************************************************************************/

static bcore_via_s* create_from_self( const bcore_flect_self_s** p_self )
{
    assert( p_self != NULL );
    const bcore_flect_self_s* self = *p_self;
    assert( self != NULL );

    bcore_via_s* o = via_s_create();
    o->p_type = bcore_name_enroll( "bcore_via_s" );
    o->o_type = self->type;

    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( self->type );

    sz_t size = self->body ? self->body->size : 0;
    sz_t index = 0;

    o->vitem_arr = bcore_u_alloc( sizeof( bcore_via_s   ), NULL, size, NULL );
    o->inst_arr  = bcore_u_alloc( sizeof( bcore_inst_s* ), NULL, size, NULL );
    bcore_memzero( o->vitem_arr, sizeof( bcore_via_s    ) * size );
    bcore_memzero( o->inst_arr,  sizeof( bcore_inst_s*  ) * size );

    if( inst_p->body )
    {
        for( sz_t i = 0; i < inst_p->body->size; i++ )
        {
            const bcore_inst_item_s* inst_item = &inst_p->body->data[ i ];
            if( inst_item->no_trace ) continue;

            const bcore_flect_item_s*   flect_item = inst_item->flect_item;
            ASSERT( index < size );
            bcore_vitem_s* vitem = &o->vitem_arr[ index ];
            vitem->name = flect_item->name;
            vitem->caps = flect_item->caps;
            vitem->offs = inst_item->offset;
            switch( vitem->caps )
            {
                case BCORE_CAPS_STATIC:
                case BCORE_CAPS_STATIC_LINK:
                {
                    vitem->type = flect_item->type;
                    if( flect_item->type == o->o_type )
                    {
                        vitem->via = o;
                    }
                    else
                    {
                        vitem->via = bcore_via_s_get_typed( vitem->type );
                    }
                }
                break;

                case BCORE_CAPS_TYPED_LINK:
                case BCORE_CAPS_AWARE_LINK:
                    vitem->type = 0;
                    vitem->via  = NULL;
                    break;

                case BCORE_CAPS_STATIC_ARRAY:
                    vitem->type = bcore_static_array_type_of( flect_item->type );
                    vitem->via  = NULL;
                    break;

                case BCORE_CAPS_TYPED_ARRAY:
                    vitem->type = typeof( "bcore_typed_array_s" );
                    vitem->via  = NULL;
                    break;

                case BCORE_CAPS_STATIC_LINK_ARRAY:
                    vitem->type = bcore_static_link_array_type_of( flect_item->type );
                    vitem->via  = NULL;
                    break;

                case BCORE_CAPS_TYPED_LINK_ARRAY:
                    vitem->type = typeof( "bcore_typed_link_array_s" );
                    vitem->via  = NULL;
                    break;

                case BCORE_CAPS_AWARE_LINK_ARRAY:
                    vitem->type = typeof( "bcore_aware_link_array_s" );
                    vitem->via  = NULL;
                    break;

                default:
                    ERR( "Unexpected caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
                    break;
            }

            if( vitem->type )
            {
                o->inst_arr[ index ] = bcore_inst_s_get_typed( vitem->type );
            }
            index++;
        }
    }

    o->size = index;
    if( o->size < size ) // realloc to save memory;
    {
        o->vitem_arr = bcore_u_alloc( sizeof( bcore_via_s   ), o->vitem_arr, o->size, NULL );
        o->inst_arr  = bcore_u_alloc( sizeof( bcore_inst_s* ), o->inst_arr,  o->size, NULL );
    }

    o->iget_name     = iget_name;
    o->iget_c        = iget_c;
    o->iget_d        = iget_d;
    o->iset_c        = iset_c;
    o->iset_d        = iset_d;
    o->iget_vitem    = iget_vitem;
    o->iget_via      = iget_via;
    o->iget_type     = iget_type;
    o->icreate       = icreate;
    o->ityped_create = ityped_create;
    o->idiscard      = idiscard;
    o->idetach       = idetach;

    o->nget_index    = nget_index;
    o->nget_c        = nget_c;
    o->nget_d        = nget_d;
    o->nset_c        = nset_c;
    o->nset_d        = nset_d;
    o->nget_vitem    = nget_vitem;
    o->nget_via      = nget_via;
    o->nget_type     = nget_type;
    o->ncreate       = ncreate;
    o->ntyped_create = ntyped_create;
    o->ndiscard      = ndiscard;
    o->ndetach       = ndetach;

    return o;
}

bcore_flect_self_s* bcore_via_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_create_plain( bcore_name_enroll( "bcore_via_s" ), sizeof( bcore_via_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )via_s_init,             "bcore_fp_init",                   "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )via_s_down,             "bcore_fp_down",                   "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )via_s_create,           "bcore_fp_create",                 "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )via_s_discard,          "bcore_fp_discard",                "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )via_s_create_signature, "bcore_spect_fp_create_signature", "create_signature" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self,       "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

const bcore_via_s* bcore_via_s_get_typed( u2_t o_type )
{
    return bcore_spect_get_typed( typeof( "bcore_via_s" ), o_type );
}

/**********************************************************************************************************************/

bcore_string_s* bcore_spect_via_selftest( void )
{
    bcore_flect_parse_sc( "via_specs = { sz_t size; u2_t param1; s2_t; }" );
    vd_t via_specs = bcore_inst_typed_create( typeof( "via_specs" ) );
    const bcore_via_s* via_specs_v = bcore_via_s_get_typed( typeof( "via_specs" ) );

    {
        sz_t size   =  10;
        u2_t param1 = 200;
        s2_t param2 = -50;
        via_specs_v->nset_c( via_specs_v, via_specs, typeof( "size"   ), &size );
        via_specs_v->nset_c( via_specs_v, via_specs, typeof( "param1" ), &param1 );
        via_specs_v->nset_c( via_specs_v, via_specs, typeof( "" ), &param2 );
    }

    ASSERT( *( sz_t* )via_specs_v->nget_c( via_specs_v, via_specs, typeof( "size"   ) ) ==  10 );
    ASSERT( *( u2_t* )via_specs_v->nget_c( via_specs_v, via_specs, typeof( "param1" ) ) == 200 );
    ASSERT( *( s2_t* )via_specs_v->nget_c( via_specs_v, via_specs, typeof( "" ) )       == -50 );

    bcore_flect_parse_sc( "via_specs_arr = { aware_t _; u3_t flags; via_specs [] arr; }" );
    vd_t via_specs_arr = bcore_inst_typed_create( typeof( "via_specs_arr" ) );

    const bcore_via_s* via_specs_arr_v = bcore_via_s_get_typed( typeof( "via_specs_arr" ) );

    vd_t arr = via_specs_arr_v->nget_d( via_specs_arr_v, via_specs_arr, typeof( "arr" ) );
    tp_t arr_type = via_specs_arr_v->nget_type( via_specs_arr_v, via_specs_arr, typeof( "arr" ) );
    const bcore_array_s* arr_p = bcore_array_s_get_typed( arr_type );

    sz_t arr_size = 100000;

    for( sz_t i = 0; i < arr_size; i++ )
    {
        arr_p->push_c( arr_p, arr, via_specs );
    }

    for( sz_t i = 0; i < arr_size; i++ )
    {
        vd_t via_specs_l = arr_p->get_d( arr_p, arr, i );
        via_specs_v->nset_c( via_specs_v, via_specs_l, typeof( "size" ), &i );
    }

    vd_t via_specs_arr2 = bcore_inst_typed_create( typeof( "via_specs_arr" ) );

    via_specs_arr_v->nset_c( via_specs_arr_v, via_specs_arr2, typeof( "arr" ), via_specs_arr_v->nget_c( via_specs_arr_v, via_specs_arr, typeof( "arr" ) ) );

    vd_t arr2 = via_specs_arr_v->nget_d( via_specs_arr_v, via_specs_arr2, typeof( "arr" ) );

    for( sz_t i = 0; i < arr_size; i++ )
    {
        vc_t via_specs_l = arr_p->get_c( arr_p, arr2, i );
        ASSERT( i == *( sz_t* )via_specs_v->nget_c( via_specs_v, via_specs_l, typeof( "size" ) ) );
    }

    ASSERT( bcore_strcmp( nameof( arr_type ), "via_specs__static_array" ) == 0 );
    ASSERT( arr_p->get_size( arr_p, arr ) == arr_size );

    bcore_inst_typed_discard( typeof( "via_specs" ), via_specs );
    bcore_inst_aware_discard( via_specs_arr );
    bcore_inst_aware_discard( via_specs_arr2 );

    return NULL;
}
