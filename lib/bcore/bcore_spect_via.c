/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_via.h"
#include "bcore_spect_array.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_quicktypes.h"

#define NPX( name ) bcore_via_##name

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

tp_t bcore_via_spect_iget_name( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return p->vitem_arr[ index ].name;
}

tp_t bcore_via_spect_iget_type( const bcore_via_s* p, vc_t o, sz_t index )
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

rf_s bcore_via_spect_irget( const bcore_via_s* p, vc_t o, sz_t index         )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            return rf_wd( ( vc_t )( ( u0_t* )o + vitem->offs ), vitem->type );
        }

        case BCORE_CAPS_STATIC_LINK:
        {
            const bcore_static_link_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            return rf_wd( dst->link, vitem->type );
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            const bcore_typed_link_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            return rf_wd( dst->link, dst->type );
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            const bcore_aware_link_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            return rf_wd( dst->link, dst->link ? *(aware_t*)dst->link : 0 );
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            return rf_wd( ( vc_t )( ( u0_t* )o + vitem->offs ), vitem->type );
        }

        default: break;
    }

    return rf_wd( NULL, 0 );
}

rf_s bcore_via_spect_irset( const bcore_via_s* p, vd_t o, sz_t index, rf_s src )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem  = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            vd_t dst = ( u0_t* )o + vitem->offs;
            const bcore_inst_s* inst_p = p->inst_arr[ index ];
            if( src.t == vitem->type )
            {
                inst_p->copy( inst_p, dst, src.o );
            }
            else
            {
                inst_p->copy_typed( inst_p, dst, src.t, src.o );
            }
            rf_s_down( src );
            return rf_wd( dst, vitem->type );
        }

        case BCORE_CAPS_STATIC_LINK:
        {
            bcore_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            const bcore_inst_s* inst_p = p->inst_arr[ index ];
            if( dst->link ) inst_p->discard( inst_p, dst->link );

            if( src.t == vitem->type )
            {
                dst->link = src.s ? src.o : inst_p->clone( inst_p, src.o );
                src.s = false;
            }
            else
            {
                dst->link = inst_p->create_typed( inst_p, src.t, src.o );
            }
            rf_s_down( src );
            return rf_wd( dst->link, vitem->type );
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->type && dst->link )
            {
                bcore_inst_typed_discard( dst->type, dst->link );
                dst->link = NULL;
            }

            if( src.t )
            {
                dst->type = src.t;
                dst->link = src.s ? src.o : bcore_inst_typed_clone( src.t, src.o );
                src.s = false;
            }

            rf_s_down( src );
            return rf_wd( dst->link, dst->type );
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            bcore_aware_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );

            if( dst->link )
            {
                bcore_inst_aware_discard( dst->link );
                dst->link = NULL;
            }

            if( src.o )
            {
                if( src.t )
                {
                    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( src.t );
                    if( inst_p->aware )
                    {
                        dst->link = src.s ? src.o : bcore_inst_spect_clone( inst_p, src.o );
                        src.s = false;
                    }
                    else
                    {
                        ERR( "Cannot convert '%s' to self-aware object", ifnameof( src.t ) );
                    }
                }
                else
                {
                    dst->link = src.s ? src.o : bcore_inst_aware_clone( src.o );
                    src.s = false;
                }
            }

            rf_s_down( src );
            return rf_wd( dst->link, dst->link ? *(aware_t*)dst->link : 0 );
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            const bcore_inst_s* inst_p = p->inst_arr[ index ];
            vd_t dst = ( ( u0_t* )o + vitem->offs );
            if( src.t == vitem->type )
            {
                bcore_inst_spect_copy( inst_p, dst, src.o );
            }
            else
            {
                bcore_inst_spect_copy_typed( inst_p, dst, src.t, src.o );
            }
            rf_s_down( src );
            return rf_wd( dst, vitem->type );
        }

        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
            ERR( "External object '%s' cannot be changed though perspective %s", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        default:
            ERR( "Unsupported caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
            break;
    }

    return rf_wd( NULL, 0 );
}

vc_t bcore_via_spect_iget_c( const bcore_via_s* p, vc_t o, sz_t index )
{
    rf_s r = bcore_via_spect_irget( p, o, index );
    if( r.s ) ERR( "%s:%s yielded a strong reference", ifnameof( p->o_type ), ifnameof( bcore_via_spect_iget_name( p, index ) ) );
    return r.o;
}

vd_t bcore_via_spect_iget_d( const bcore_via_s* p, vd_t o, sz_t index )
{
    rf_s r = bcore_via_spect_irget( p, o, index );
    if( r.c ) ERR( "%s:%s yielded a const reference", ifnameof( p->o_type ), ifnameof( bcore_via_spect_iget_name( p, index ) ) );
    if( r.s ) ERR( "%s:%s yielded a strong reference", ifnameof( p->o_type ), ifnameof( bcore_via_spect_iget_name( p, index ) ) );
    return r.o;
}

rf_s bcore_via_spect_iset_s3( const bcore_via_s* p, vd_t o, sz_t index, s3_t val ) { return bcore_via_spect_irset( p, o, index, rf_wc( &val, TYPEOF_s3_t ) ); }
rf_s bcore_via_spect_iset_u3( const bcore_via_s* p, vd_t o, sz_t index, u3_t val ) { return bcore_via_spect_irset( p, o, index, rf_wc( &val, TYPEOF_u3_t ) ); }
rf_s bcore_via_spect_iset_f3( const bcore_via_s* p, vd_t o, sz_t index, f3_t val ) { return bcore_via_spect_irset( p, o, index, rf_wc( &val, TYPEOF_f3_t ) ); }
rf_s bcore_via_spect_iset_sz( const bcore_via_s* p, vd_t o, sz_t index, sz_t val ) { return bcore_via_spect_irset( p, o, index, rf_wc( &val, TYPEOF_sz_t ) ); }
rf_s bcore_via_spect_iset_sc( const bcore_via_s* p, vd_t o, sz_t index, sc_t val ) { return bcore_via_spect_irset( p, o, index, rf_wc( &val, TYPEOF_sc_t ) ); }
rf_s bcore_via_spect_iset_bl( const bcore_via_s* p, vd_t o, sz_t index, bl_t val ) { return bcore_via_spect_irset( p, o, index, rf_wc( &val, TYPEOF_bl_t ) ); }

const bcore_vitem_s* bcore_via_spect_iget_vitem( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return &p->vitem_arr[ index ];
}

const bcore_via_s* bcore_via_spect_iget_via( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return p->vitem_arr[ index ].via;
}

const bcore_array_s* bcore_via_spect_iget_array( const bcore_via_s* p, sz_t index )
{
    if( !bcore_flect_caps_is_array( p->vitem_arr[ index ].caps ) ) return NULL;
    return bcore_array_s_get_typed( p->vitem_arr[ index ].type );
}

vc_t bcore_via_spect_iget_spect( const bcore_via_s* p, vc_t o, sz_t index, tp_t spect_type )
{
    return bcore_spect_get_typed( spect_type, bcore_via_spect_iget_type( p, o, index ) );
}

/**********************************************************************************************************************/

static inline sz_t vidx( const bcore_via_s* p, tp_t name )
{
    for( sz_t i = 0; i < p->size; i++ ) if( p->vitem_arr[ i ].name == name ) return i;
    ERR( "object '%s' has no element of name '%s'", ifnameof( p->o_type ), ifnameof( name ) );
    return 0;
}

sz_t                 NPX(spect_nget_index   )( const NPX(s)* p,         tp_t n                   ) { return vidx( p, n ); }
tp_t                 NPX(spect_nget_type    )( const NPX(s)* p, vc_t o, tp_t n                   ) { return NPX(spect_iget_type    )( p, o, vidx( p, n )         ); }
rf_s                 NPX(spect_nrget        )( const NPX(s)* p, vc_t o, tp_t n                   ) { return NPX(spect_irget        )( p, o, vidx( p, n )         ); }
rf_s                 NPX(spect_nrset        )( const NPX(s)* p, vd_t o, tp_t n, rf_s src         ) { return NPX(spect_irset        )( p, o, vidx( p, n ), src    ); }
vc_t                 NPX(spect_nget_c       )( const NPX(s)* p, vc_t o, tp_t n                   ) { return NPX(spect_iget_c       )( p, o, vidx( p, n )         ); }
vd_t                 NPX(spect_nget_d       )( const NPX(s)* p, vd_t o, tp_t n                   ) { return NPX(spect_iget_d       )( p, o, vidx( p, n )         ); }
rf_s                 NPX(spect_nset_s3      )( const NPX(s)* p, vd_t o, tp_t n, s3_t val         ) { return NPX(spect_iset_s3      )( p, o, vidx( p, n ), val    ); }
rf_s                 NPX(spect_nset_u3      )( const NPX(s)* p, vd_t o, tp_t n, u3_t val         ) { return NPX(spect_iset_u3      )( p, o, vidx( p, n ), val    ); }
rf_s                 NPX(spect_nset_f3      )( const NPX(s)* p, vd_t o, tp_t n, f3_t val         ) { return NPX(spect_iset_f3      )( p, o, vidx( p, n ), val    ); }
rf_s                 NPX(spect_nset_sz      )( const NPX(s)* p, vd_t o, tp_t n, sz_t val         ) { return NPX(spect_iset_sz      )( p, o, vidx( p, n ), val    ); }
rf_s                 NPX(spect_nset_sc      )( const NPX(s)* p, vd_t o, tp_t n, sc_t val         ) { return NPX(spect_iset_sc      )( p, o, vidx( p, n ), val    ); }
rf_s                 NPX(spect_nset_bl      )( const NPX(s)* p, vd_t o, tp_t n, bl_t val         ) { return NPX(spect_iset_bl      )( p, o, vidx( p, n ), val    ); }
const bcore_vitem_s* NPX(spect_nget_vitem   )( const NPX(s)* p,         tp_t n                   ) { return NPX(spect_iget_vitem   )( p,    vidx( p, n )         ); }
const NPX(s)*        NPX(spect_nget_via     )( const NPX(s)* p,         tp_t n                   ) { return NPX(spect_iget_via     )( p,    vidx( p, n )         ); }
const bcore_array_s* NPX(spect_nget_array   )( const NPX(s)* p,         tp_t n                   ) { return NPX(spect_iget_array   )( p,    vidx( p, n )         ); }
vc_t                 NPX(spect_nget_spect   )( const NPX(s)* p, vc_t o, tp_t n, tp_t stp         ) { return NPX(spect_iget_spect   )( p, o, vidx( p, n ), stp    ); }

static inline const bcore_via_s* vtpd( tp_t tp ) { return bcore_via_s_get_typed( tp ); }

tp_t                 NPX(typed_nget_type    )( tp_t tp, vc_t o, tp_t n                   ) { return NPX(spect_nget_type    )( vtpd( tp ), o, n         ); }
rf_s                 NPX(typed_nrget        )( tp_t tp, vc_t o, tp_t n                   ) { return NPX(spect_nrget        )( vtpd( tp ), o, n         ); }
rf_s                 NPX(typed_nrset        )( tp_t tp, vd_t o, tp_t n, rf_s src         ) { return NPX(spect_nrset        )( vtpd( tp ), o, n, src    ); }
vc_t                 NPX(typed_nget_c       )( tp_t tp, vc_t o, tp_t n                   ) { return NPX(spect_nget_c       )( vtpd( tp ), o, n         ); }
vd_t                 NPX(typed_nget_d       )( tp_t tp, vd_t o, tp_t n                   ) { return NPX(spect_nget_d       )( vtpd( tp ), o, n         ); }
rf_s                 NPX(typed_nset_s3      )( tp_t tp, vd_t o, tp_t n, s3_t val         ) { return NPX(spect_nset_s3      )( vtpd( tp ), o, n, val    ); }
rf_s                 NPX(typed_nset_u3      )( tp_t tp, vd_t o, tp_t n, u3_t val         ) { return NPX(spect_nset_u3      )( vtpd( tp ), o, n, val    ); }
rf_s                 NPX(typed_nset_f3      )( tp_t tp, vd_t o, tp_t n, f3_t val         ) { return NPX(spect_nset_f3      )( vtpd( tp ), o, n, val    ); }
rf_s                 NPX(typed_nset_sz      )( tp_t tp, vd_t o, tp_t n, sz_t val         ) { return NPX(spect_nset_sz      )( vtpd( tp ), o, n, val    ); }
rf_s                 NPX(typed_nset_sc      )( tp_t tp, vd_t o, tp_t n, sc_t val         ) { return NPX(spect_nset_sc      )( vtpd( tp ), o, n, val    ); }
rf_s                 NPX(typed_nset_bl      )( tp_t tp, vd_t o, tp_t n, bl_t val         ) { return NPX(spect_nset_bl      )( vtpd( tp ), o, n, val    ); }
const bcore_vitem_s* NPX(typed_nget_vitem   )( tp_t tp,         tp_t n                   ) { return NPX(spect_nget_vitem   )( vtpd( tp ),    n         ); }
const NPX(s)*        NPX(typed_nget_via     )( tp_t tp,         tp_t n                   ) { return NPX(spect_nget_via     )( vtpd( tp ),    n         ); }
const bcore_array_s* NPX(typed_nget_array   )( tp_t tp,         tp_t n                   ) { return NPX(spect_nget_array   )( vtpd( tp ),    n         ); }
vc_t                 NPX(typed_nget_spect   )( tp_t tp, vc_t o, tp_t n, tp_t stp         ) { return NPX(spect_nget_spect   )( vtpd( tp ), o, n, stp    ); }

tp_t                 NPX(aware_nget_type    )( vc_t o, tp_t n                   ) { return NPX(typed_nget_type    )( *( aware_t* )o, o, n         ); }
rf_s                 NPX(aware_nrget        )( vc_t o, tp_t n                   ) { return NPX(typed_nrget        )( *( aware_t* )o, o, n         ); }
rf_s                 NPX(aware_nrset        )( vd_t o, tp_t n, rf_s src         ) { return NPX(typed_nrset        )( *( aware_t* )o, o, n, src    ); }
vc_t                 NPX(aware_nget_c       )( vc_t o, tp_t n                   ) { return NPX(typed_nget_c       )( *( aware_t* )o, o, n         ); }
vd_t                 NPX(aware_nget_d       )( vd_t o, tp_t n                   ) { return NPX(typed_nget_d       )( *( aware_t* )o, o, n         ); }
rf_s                 NPX(aware_nset_s3      )( vd_t o, tp_t n, s3_t val         ) { return NPX(typed_nset_s3      )( *( aware_t* )o, o, n, val    ); }
rf_s                 NPX(aware_nset_u3      )( vd_t o, tp_t n, u3_t val         ) { return NPX(typed_nset_u3      )( *( aware_t* )o, o, n, val    ); }
rf_s                 NPX(aware_nset_f3      )( vd_t o, tp_t n, f3_t val         ) { return NPX(typed_nset_f3      )( *( aware_t* )o, o, n, val    ); }
rf_s                 NPX(aware_nset_sz      )( vd_t o, tp_t n, sz_t val         ) { return NPX(typed_nset_sz      )( *( aware_t* )o, o, n, val    ); }
rf_s                 NPX(aware_nset_sc      )( vd_t o, tp_t n, sc_t val         ) { return NPX(typed_nset_sc      )( *( aware_t* )o, o, n, val    ); }
rf_s                 NPX(aware_nset_bl      )( vd_t o, tp_t n, bl_t val         ) { return NPX(typed_nset_bl      )( *( aware_t* )o, o, n, val    ); }
const bcore_vitem_s* NPX(aware_nget_vitem   )( vc_t o, tp_t n                   ) { return NPX(typed_nget_vitem   )( *( aware_t* )o,    n         ); }
const NPX(s)*        NPX(aware_nget_via     )( vc_t o, tp_t n                   ) { return NPX(typed_nget_via     )( *( aware_t* )o,    n         ); }
const bcore_array_s* NPX(aware_nget_array   )( vc_t o, tp_t n                   ) { return NPX(typed_nget_array   )( *( aware_t* )o,    n         ); }
vc_t                 NPX(aware_nget_spect   )( vc_t o, tp_t n, tp_t stp         ) { return NPX(typed_nget_spect   )( *( aware_t* )o, o, n, stp    ); }

/**********************************************************************************************************************/

bl_t bcore_via_spect_is_pure_array( const bcore_via_s* p )
{
    return ( ( p->size == 1 ) && bcore_via_spect_iis_array( p, 0 ) );
}

bl_t bcore_via_spect_iis_array( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return bcore_flect_caps_is_array( p->vitem_arr[ index ].caps );
}

bl_t bcore_via_spect_iis_static( const bcore_via_s* p, sz_t index )
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

bl_t bcore_via_spect_iis_link( const bcore_via_s* p, sz_t index )
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

const bcore_via_s* bcore_via_s_get_aware( vc_t obj )
{
    return bcore_via_s_get_typed( *( const aware_t* )obj );
}

/**********************************************************************************************************************/

bcore_string_s* bcore_spect_via_selftest( void )
{
    bcore_flect_define_parse_sc( "via_specs = { sz_t size; u2_t param1; s2_t; }" );
    vd_t via_specs = bcore_inst_typed_create( typeof( "via_specs" ) );
    const bcore_via_s* via_specs_v = bcore_via_s_get_typed( typeof( "via_specs" ) );

    {
        bcore_via_spect_nset_sz( via_specs_v, via_specs, typeof( "size"   ),  10 );
        bcore_via_spect_nset_u3( via_specs_v, via_specs, typeof( "param1" ), 200 );
        bcore_via_spect_nset_s3( via_specs_v, via_specs, typeof( "" ), -50 );
    }

    ASSERT( *( sz_t* )bcore_via_spect_nget_c( via_specs_v, via_specs, typeof( "size"   ) ) ==  10 );
    ASSERT( *( u2_t* )bcore_via_spect_nget_c( via_specs_v, via_specs, typeof( "param1" ) ) == 200 );
    ASSERT( *( s2_t* )bcore_via_spect_nget_c( via_specs_v, via_specs, typeof( "" ) )       == -50 );

    bcore_flect_define_parse_sc( "via_specs_arr = { aware_t _; u3_t flags; via_specs [] arr; }" );
    vd_t via_specs_arr = bcore_inst_typed_create( typeof( "via_specs_arr" ) );

    const bcore_via_s* via_specs_arr_v = bcore_via_s_get_typed( typeof( "via_specs_arr" ) );

    vd_t arr = bcore_via_spect_nget_d( via_specs_arr_v, via_specs_arr, typeof( "arr" ) );
    tp_t arr_type = bcore_via_spect_nget_type( via_specs_arr_v, via_specs_arr, typeof( "arr" ) );
    const bcore_array_s* arr_p = bcore_array_s_get_typed( arr_type );

    sz_t arr_size = 100000;

    for( sz_t i = 0; i < arr_size; i++ )
    {
        bcore_array_spect_push_c( arr_p, arr, via_specs );
    }

    for( sz_t i = 0; i < arr_size; i++ )
    {
        vd_t via_specs_l = arr_p->get_d( arr_p, arr, i );
        bcore_via_spect_nset_sz( via_specs_v, via_specs_l, typeof( "size" ), i );
    }

    vd_t via_specs_arr2 = bcore_inst_typed_create( typeof( "via_specs_arr" ) );

    bcore_via_spect_nrset( via_specs_arr_v, via_specs_arr2, typeof( "arr" ), bcore_via_spect_nrget( via_specs_arr_v, via_specs_arr, typeof( "arr" ) ) );

    vd_t arr2 = bcore_via_spect_nget_d( via_specs_arr_v, via_specs_arr2, typeof( "arr" ) );

    for( sz_t i = 0; i < arr_size; i++ )
    {
        vc_t via_specs_l = arr_p->get_c( arr_p, arr2, i );
        ASSERT( i == *( sz_t* )bcore_via_spect_nget_c( via_specs_v, via_specs_l, typeof( "size" ) ) );
    }

    ASSERT( bcore_strcmp( nameof( arr_type ), "via_specs__static_array" ) == 0 );
    ASSERT( arr_p->get_size( arr_p, arr ) == arr_size );

    bcore_inst_typed_discard( typeof( "via_specs" ), via_specs );
    bcore_inst_aware_discard( via_specs_arr );
    bcore_inst_aware_discard( via_specs_arr2 );

    return NULL;
}

