/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_via.h"
#include "bcore_spect_array.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_quicktypes.h"
#include "bcore_life.h"

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

/**********************************************************************************************************************/

sz_t bcore_via_spect_get_size( const bcore_via_s* p )
{
    return p->size;
}

tp_t bcore_via_spect_iget_name( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return p->vitem_arr[ index ].name;
}

sr_s bcore_via_spect_iget( const bcore_via_s* p, vc_t o, sz_t index         )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem = &p->vitem_arr[ index ];
    if( vitem->fp_get )  // explicit getter
    {
        return vitem->fp_get( o );
    }
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            return sr_twd( vitem->type, ( vd_t )( ( u0_t* )o + vitem->offs ) );
        }

        case BCORE_CAPS_STATIC_LINK:
        {
            const bcore_static_link_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            return sr_twd( vitem->type, dst->link );
        }

        case BCORE_CAPS_TYPED_LINK:
        {
            const bcore_typed_link_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            return sr_twd( dst->type, dst->link );
        }

        case BCORE_CAPS_AWARE_LINK:
        {
            const bcore_aware_link_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            return sr_twd( dst->link ? *(aware_t*)dst->link : 0, dst->link );
        }

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            return sr_twd( vitem->type, ( vd_t )( ( u0_t* )o + vitem->offs ) );
        }

        default: break;
    }

    return sr_null();
}

tp_t bcore_via_spect_iget_type( const bcore_via_s* p, vc_t o, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_TYPED_LINK:
        case BCORE_CAPS_AWARE_LINK:
        {
            sr_s sr = bcore_via_spect_iget( p, o, index );
            tp_t t = sr_s_type( &sr );
            sr_down( sr );
            return t;
        }

        default: break;
    }
    return vitem->type;
}

void bcore_via_spect_iset( const bcore_via_s* p, vd_t o, sz_t index, sr_s src )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem  = &p->vitem_arr[ index ];
    if( vitem->fp_set ) // explicit setter
    {
        vitem->fp_set( o, src );
        return;
    }
    switch( vitem->caps )
    {
        case BCORE_CAPS_STATIC:
        {
            vd_t dst = ( u0_t* )o + vitem->offs;
            const bcore_inst_s* inst_p = vitem->via_p->inst_p;
            if( sr_s_type( &src ) == vitem->type )
            {
                bcore_inst_spect_copy( inst_p, dst, src.o );
            }
            else
            {
                bcore_inst_spect_copy_typed( inst_p, dst, sr_s_type( &src ), src.o );
            }
        }
        break;

        case BCORE_CAPS_STATIC_LINK:
        {
            bcore_static_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            const bcore_inst_s* inst_p = vitem->via_p->inst_p;
            if( dst->link ) bcore_inst_spect_discard( inst_p, dst->link );
            if( sr_s_type( &src ) == vitem->type )
            {
                dst->link = sr_s_is_strong( &src ) ? src.o : bcore_inst_spect_clone( inst_p, src.o );
                src = sr_cw( src );
            }
            else
            {
                dst->link = bcore_inst_spect_create_typed( inst_p, sr_s_type( &src ), src.o );
            }
        }
        break;

        case BCORE_CAPS_TYPED_LINK:
        {
            bcore_typed_link_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->type && dst->link )
            {
                bcore_inst_typed_discard( dst->type, dst->link );
                dst->link = NULL;
            }
            if( sr_s_type( &src ) )
            {
                dst->type = sr_s_type( &src );
                dst->link = sr_s_is_strong( &src ) ? src.o : bcore_inst_typed_clone( sr_s_type( &src ), src.o );
                src = sr_cw( src );
            }
        }
        break;

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
                if( sr_s_type( &src ) )
                {
                    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( sr_s_type( &src ) );
                    if( inst_p->aware )
                    {
                        dst->link = sr_s_is_strong( &src ) ? src.o : bcore_inst_spect_clone( inst_p, src.o );
                        src = sr_cw( src );
                    }
                    else
                    {
                        ERR( "Cannot convert '%s' to self-aware object", ifnameof( sr_s_type( &src ) ) );
                    }
                }
                else
                {
                    dst->link = sr_s_is_strong( &src ) ? src.o : bcore_inst_aware_clone( src.o );
                    src = sr_cw( src );
                }
            }
        }
        break;

        case BCORE_CAPS_STATIC_ARRAY:
        case BCORE_CAPS_TYPED_ARRAY:
        case BCORE_CAPS_STATIC_LINK_ARRAY:
        case BCORE_CAPS_TYPED_LINK_ARRAY:
        case BCORE_CAPS_AWARE_LINK_ARRAY:
        {
            vd_t dst = ( ( u0_t* )o + vitem->offs );
            const bcore_inst_s* inst_p = vitem->via_p->inst_p;
            if( sr_s_type( &src ) == vitem->type )
            {
                bcore_inst_spect_copy( inst_p, dst, src.o );
            }
            else
            {
                bcore_inst_spect_copy_typed( inst_p, dst, sr_s_type( &src ), src.o );
            }
        }
        break;

        case BCORE_CAPS_EXTERNAL_DATA:
        case BCORE_CAPS_EXTERNAL_FUNC:
            ERR( "External object '%s' cannot be changed through perspective %s", bcore_flect_caps_e_sc( vitem->caps ), ifnameof( p->p_type ) );
            break;

        default:
            ERR( "Unsupported caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
            break;
    }
    sr_down( src );
}

void bcore_via_spect_iset_s3( const bcore_via_s* p, vd_t o, sz_t index, s3_t val ) { bcore_via_spect_iset( p, o, index, sr_twc( TYPEOF_s3_t, &val ) ); }
void bcore_via_spect_iset_u3( const bcore_via_s* p, vd_t o, sz_t index, u3_t val ) { bcore_via_spect_iset( p, o, index, sr_twc( TYPEOF_u3_t, &val ) ); }
void bcore_via_spect_iset_f3( const bcore_via_s* p, vd_t o, sz_t index, f3_t val ) { bcore_via_spect_iset( p, o, index, sr_twc( TYPEOF_f3_t, &val ) ); }
void bcore_via_spect_iset_sz( const bcore_via_s* p, vd_t o, sz_t index, sz_t val ) { bcore_via_spect_iset( p, o, index, sr_twc( TYPEOF_sz_t, &val ) ); }
void bcore_via_spect_iset_sc( const bcore_via_s* p, vd_t o, sz_t index, sc_t val ) { bcore_via_spect_iset( p, o, index, sr_twc( TYPEOF_sc_t, &val ) ); }
void bcore_via_spect_iset_bl( const bcore_via_s* p, vd_t o, sz_t index, bl_t val ) { bcore_via_spect_iset( p, o, index, sr_twc( TYPEOF_bl_t, &val ) ); }
void bcore_via_spect_iset_tp( const bcore_via_s* p, vd_t o, sz_t index, tp_t val ) { bcore_via_spect_iset( p, o, index, sr_twc( TYPEOF_tp_t, &val ) ); }

const bcore_vitem_s* bcore_via_spect_iget_vitem( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return &p->vitem_arr[ index ];
}

const bcore_via_s* bcore_via_spect_iget_via( const bcore_via_s* p, sz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return p->vitem_arr[ index ].via_p;
}

const bcore_array_s* bcore_via_spect_iget_array( const bcore_via_s* p, sz_t index )
{
    const bcore_vitem_s* vitem = &p->vitem_arr[ index ];
    if( !bcore_flect_caps_is_array( vitem->caps ) )
    {
        ERR( "%s::%s of type '%s' and capsulation '%s' is no array",
             ifnameof( p->o_type ),
             ifnameof( bcore_via_spect_iget_name( p, index ) ),
             ifnameof( vitem->type ),
             bcore_flect_caps_e_sc( vitem->caps )  );
    }
    return bcore_array_s_get_typed( vitem->type );
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

sz_t                 NPX(spect_nget_index   )( const NPX(s)* p,         tp_t n           ) { return vidx( p, n ); }
sr_s                 NPX(spect_nget         )( const NPX(s)* p, vc_t o, tp_t n           ) { return NPX(spect_iget       )( p, o, vidx( p, n )         ); }
void                 NPX(spect_nset         )( const NPX(s)* p, vd_t o, tp_t n, sr_s src ) {        NPX(spect_iset       )( p, o, vidx( p, n ), src    ); }
void                 NPX(spect_nset_s3      )( const NPX(s)* p, vd_t o, tp_t n, s3_t val ) {        NPX(spect_iset_s3    )( p, o, vidx( p, n ), val    ); }
void                 NPX(spect_nset_u3      )( const NPX(s)* p, vd_t o, tp_t n, u3_t val ) {        NPX(spect_iset_u3    )( p, o, vidx( p, n ), val    ); }
void                 NPX(spect_nset_f3      )( const NPX(s)* p, vd_t o, tp_t n, f3_t val ) {        NPX(spect_iset_f3    )( p, o, vidx( p, n ), val    ); }
void                 NPX(spect_nset_sz      )( const NPX(s)* p, vd_t o, tp_t n, sz_t val ) {        NPX(spect_iset_sz    )( p, o, vidx( p, n ), val    ); }
void                 NPX(spect_nset_sc      )( const NPX(s)* p, vd_t o, tp_t n, sc_t val ) {        NPX(spect_iset_sc    )( p, o, vidx( p, n ), val    ); }
void                 NPX(spect_nset_bl      )( const NPX(s)* p, vd_t o, tp_t n, bl_t val ) {        NPX(spect_iset_bl    )( p, o, vidx( p, n ), val    ); }
void                 NPX(spect_nset_tp      )( const NPX(s)* p, vd_t o, tp_t n, tp_t val ) {        NPX(spect_iset_tp    )( p, o, vidx( p, n ), val    ); }
const bcore_vitem_s* NPX(spect_nget_vitem   )( const NPX(s)* p,         tp_t n           ) { return NPX(spect_iget_vitem )( p,    vidx( p, n )         ); }
const NPX(s)*        NPX(spect_nget_via     )( const NPX(s)* p,         tp_t n           ) { return NPX(spect_iget_via   )( p,    vidx( p, n )         ); }
const bcore_array_s* NPX(spect_nget_array   )( const NPX(s)* p,         tp_t n           ) { return NPX(spect_iget_array )( p,    vidx( p, n )         ); }
vc_t                 NPX(spect_nget_spect   )( const NPX(s)* p, vc_t o, tp_t n, tp_t stp ) { return NPX(spect_iget_spect )( p, o, vidx( p, n ), stp    ); }

static inline const bcore_via_s* vtpd( tp_t tp ) { return bcore_via_s_get_typed( tp ); }

sz_t                 NPX(typed_get_size     )( tp_t tp                                   ) { return NPX(spect_get_size     )( vtpd( tp )               ); }
sr_s                 NPX(typed_nget         )( tp_t tp, vc_t o, tp_t n                   ) { return NPX(spect_nget         )( vtpd( tp ), o, n         ); }
void                 NPX(typed_nset         )( tp_t tp, vd_t o, tp_t n, sr_s src         ) {        NPX(spect_nset         )( vtpd( tp ), o, n, src    ); }
void                 NPX(typed_nset_s3      )( tp_t tp, vd_t o, tp_t n, s3_t val         ) {        NPX(spect_nset_s3      )( vtpd( tp ), o, n, val    ); }
void                 NPX(typed_nset_u3      )( tp_t tp, vd_t o, tp_t n, u3_t val         ) {        NPX(spect_nset_u3      )( vtpd( tp ), o, n, val    ); }
void                 NPX(typed_nset_f3      )( tp_t tp, vd_t o, tp_t n, f3_t val         ) {        NPX(spect_nset_f3      )( vtpd( tp ), o, n, val    ); }
void                 NPX(typed_nset_sz      )( tp_t tp, vd_t o, tp_t n, sz_t val         ) {        NPX(spect_nset_sz      )( vtpd( tp ), o, n, val    ); }
void                 NPX(typed_nset_sc      )( tp_t tp, vd_t o, tp_t n, sc_t val         ) {        NPX(spect_nset_sc      )( vtpd( tp ), o, n, val    ); }
void                 NPX(typed_nset_bl      )( tp_t tp, vd_t o, tp_t n, bl_t val         ) {        NPX(spect_nset_bl      )( vtpd( tp ), o, n, val    ); }
void                 NPX(typed_nset_tp      )( tp_t tp, vd_t o, tp_t n, tp_t val         ) {        NPX(spect_nset_tp      )( vtpd( tp ), o, n, val    ); }
const bcore_vitem_s* NPX(typed_nget_vitem   )( tp_t tp,         tp_t n                   ) { return NPX(spect_nget_vitem   )( vtpd( tp ),    n         ); }
const NPX(s)*        NPX(typed_nget_via     )( tp_t tp,         tp_t n                   ) { return NPX(spect_nget_via     )( vtpd( tp ),    n         ); }
const bcore_array_s* NPX(typed_nget_array   )( tp_t tp,         tp_t n                   ) { return NPX(spect_nget_array   )( vtpd( tp ),    n         ); }
vc_t                 NPX(typed_nget_spect   )( tp_t tp, vc_t o, tp_t n, tp_t stp         ) { return NPX(spect_nget_spect   )( vtpd( tp ), o, n, stp    ); }

sz_t                 NPX(aware_get_size     )( vc_t o                           ) { return NPX(typed_get_size     )( *( aware_t* )o               ); }
sr_s                 NPX(aware_nget         )( vc_t o, tp_t n                   ) { return NPX(typed_nget         )( *( aware_t* )o, o, n         ); }
void                 NPX(aware_nset         )( vd_t o, tp_t n, sr_s src         ) {        NPX(typed_nset         )( *( aware_t* )o, o, n, src    ); }
void                 NPX(aware_nset_s3      )( vd_t o, tp_t n, s3_t val         ) {        NPX(typed_nset_s3      )( *( aware_t* )o, o, n, val    ); }
void                 NPX(aware_nset_u3      )( vd_t o, tp_t n, u3_t val         ) {        NPX(typed_nset_u3      )( *( aware_t* )o, o, n, val    ); }
void                 NPX(aware_nset_f3      )( vd_t o, tp_t n, f3_t val         ) {        NPX(typed_nset_f3      )( *( aware_t* )o, o, n, val    ); }
void                 NPX(aware_nset_sz      )( vd_t o, tp_t n, sz_t val         ) {        NPX(typed_nset_sz      )( *( aware_t* )o, o, n, val    ); }
void                 NPX(aware_nset_sc      )( vd_t o, tp_t n, sc_t val         ) {        NPX(typed_nset_sc      )( *( aware_t* )o, o, n, val    ); }
void                 NPX(aware_nset_bl      )( vd_t o, tp_t n, bl_t val         ) {        NPX(typed_nset_bl      )( *( aware_t* )o, o, n, val    ); }
void                 NPX(aware_nset_tp      )( vd_t o, tp_t n, tp_t val         ) {        NPX(typed_nset_tp      )( *( aware_t* )o, o, n, val    ); }
const bcore_vitem_s* NPX(aware_nget_vitem   )( vc_t o, tp_t n                   ) { return NPX(typed_nget_vitem   )( *( aware_t* )o,    n         ); }
const NPX(s)*        NPX(aware_nget_via     )( vc_t o, tp_t n                   ) { return NPX(typed_nget_via     )( *( aware_t* )o,    n         ); }
const bcore_array_s* NPX(aware_nget_array   )( vc_t o, tp_t n                   ) { return NPX(typed_nget_array   )( *( aware_t* )o,    n         ); }
vc_t                 NPX(aware_nget_spect   )( vc_t o, tp_t n, tp_t stp         ) { return NPX(typed_nget_spect   )( *( aware_t* )o, o, n, stp    ); }

inline static vc_t w_spect( sr_s o ) { if( sr_s_is_const( &o ) ) ERR( "Attempt to modify a constant object" ); return ch_spect( o.p, TYPEOF_bcore_via_s ); }
inline static vc_t r_spect( sr_s o ) {                                                               return ch_spect( o.p, TYPEOF_bcore_via_s ); }
inline static vc_t x_spect( sr_s o ) {                                                               return ch_spect( o.p, TYPEOF_bcore_via_s ); }

sz_t                 NPX(get_size     )( sr_s o                   ) { sz_t r = NPX(spect_get_size     )( r_spect( o )              ); sr_down( o ); return r; }
tp_t                 NPX(iget_name    )( sr_s o, sz_t i           ) { tp_t r = NPX(spect_iget_name    )( x_spect( o ),      i      ); sr_down( o ); return r; }
sr_s                 NPX(iget         )( sr_s o, sz_t i           ) { sr_s r = NPX(spect_iget         )( x_spect( o ), o.o, i      ); sr_s_set_const( &r, sr_s_is_const( &o ) ); sr_down( o ); return r; }
void                 NPX(iset         )( sr_s o, sz_t i, sr_s src ) {          NPX(spect_iset         )( w_spect( o ), o.o, i, src ); sr_down( o );           }
void                 NPX(iset_s3      )( sr_s o, sz_t i, s3_t val ) {          NPX(spect_iset_s3      )( w_spect( o ), o.o, i, val ); sr_down( o );           }
void                 NPX(iset_u3      )( sr_s o, sz_t i, u3_t val ) {          NPX(spect_iset_u3      )( w_spect( o ), o.o, i, val ); sr_down( o );           }
void                 NPX(iset_f3      )( sr_s o, sz_t i, f3_t val ) {          NPX(spect_iset_f3      )( w_spect( o ), o.o, i, val ); sr_down( o );           }
void                 NPX(iset_sz      )( sr_s o, sz_t i, sz_t val ) {          NPX(spect_iset_sz      )( w_spect( o ), o.o, i, val ); sr_down( o );           }
void                 NPX(iset_sc      )( sr_s o, sz_t i, sc_t val ) {          NPX(spect_iset_sc      )( w_spect( o ), o.o, i, val ); sr_down( o );           }
void                 NPX(iset_bl      )( sr_s o, sz_t i, bl_t val ) {          NPX(spect_iset_bl      )( w_spect( o ), o.o, i, val ); sr_down( o );           }
void                 NPX(iset_tp      )( sr_s o, sz_t i, tp_t val ) {          NPX(spect_iset_tp      )( w_spect( o ), o.o, i, val ); sr_down( o );           }
const bcore_vitem_s* NPX(iget_vitem   )( sr_s o, sz_t i           ) { vc_t r = NPX(spect_iget_vitem   )( r_spect( o ),      i      ); sr_down( o ); return r; }
const NPX(s)*        NPX(iget_via     )( sr_s o, sz_t i           ) { vc_t r = NPX(spect_iget_via     )( r_spect( o ),      i      ); sr_down( o ); return r; }
const bcore_array_s* NPX(iget_array   )( sr_s o, sz_t i           ) { vc_t r = NPX(spect_iget_array   )( r_spect( o ),      i      ); sr_down( o ); return r; }
vc_t                 NPX(iget_spect   )( sr_s o, sz_t i, tp_t stp ) { vc_t r = NPX(spect_iget_spect   )( r_spect( o ), o.o, i, stp ); sr_down( o ); return r; }
sz_t                 NPX(nget_index   )( sr_s o, tp_t n           ) { sz_t r = NPX(spect_nget_index   )( r_spect( o ),      n      ); sr_down( o ); return r; }
sr_s                 NPX(nget         )( sr_s o, tp_t n           ) { sr_s r = NPX(spect_nget         )( x_spect( o ), o.o, n      ); sr_s_set_const( &r, sr_s_is_const( &o ) ); sr_down( o ); return r; }
void                 NPX(nset         )( sr_s o, tp_t n, sr_s src ) {          NPX(spect_nset         )( w_spect( o ), o.o, n, src ); sr_down( o );           }
void                 NPX(nset_s3      )( sr_s o, tp_t n, s3_t val ) {          NPX(spect_nset_s3      )( w_spect( o ), o.o, n, val ); sr_down( o );           }
void                 NPX(nset_u3      )( sr_s o, tp_t n, u3_t val ) {          NPX(spect_nset_u3      )( w_spect( o ), o.o, n, val ); sr_down( o );           }
void                 NPX(nset_f3      )( sr_s o, tp_t n, f3_t val ) {          NPX(spect_nset_f3      )( w_spect( o ), o.o, n, val ); sr_down( o );           }
void                 NPX(nset_sz      )( sr_s o, tp_t n, sz_t val ) {          NPX(spect_nset_sz      )( w_spect( o ), o.o, n, val ); sr_down( o );           }
void                 NPX(nset_sc      )( sr_s o, tp_t n, sc_t val ) {          NPX(spect_nset_sc      )( w_spect( o ), o.o, n, val ); sr_down( o );           }
void                 NPX(nset_bl      )( sr_s o, tp_t n, bl_t val ) {          NPX(spect_nset_bl      )( w_spect( o ), o.o, n, val ); sr_down( o );           }
void                 NPX(nset_tp      )( sr_s o, tp_t n, tp_t val ) {          NPX(spect_nset_tp      )( w_spect( o ), o.o, n, val ); sr_down( o );           }
const bcore_vitem_s* NPX(nget_vitem   )( sr_s o, tp_t n           ) { vc_t r = NPX(spect_nget_vitem   )( r_spect( o ),      n      ); sr_down( o ); return r; }
const NPX(s)*        NPX(nget_via     )( sr_s o, tp_t n           ) { vc_t r = NPX(spect_nget_via     )( r_spect( o ),      n      ); sr_down( o ); return r; }
const bcore_array_s* NPX(nget_array   )( sr_s o, tp_t n           ) { vc_t r = NPX(spect_nget_array   )( r_spect( o ),      n      ); sr_down( o ); return r; }
vc_t                 NPX(nget_spect   )( sr_s o, tp_t n, tp_t stp ) { vc_t r = NPX(spect_nget_spect   )( r_spect( o ), o.o, n, stp ); sr_down( o ); return r; }
bl_t                 NPX(is_leaf      )( sr_s o                   ) { bl_t r = NPX(spect_is_leaf      )( r_spect( o )    ); sr_down( o ); return r; }
bl_t                 NPX(is_pure_array)( sr_s o                   ) { bl_t r = NPX(spect_is_pure_array)( r_spect( o )    ); sr_down( o ); return r; }
bl_t                 NPX(iis_array    )( sr_s o, sz_t i           ) { bl_t r = NPX(spect_iis_array    )( r_spect( o ), i ); sr_down( o ); return r; }
bl_t                 NPX(iis_static   )( sr_s o, sz_t i           ) { bl_t r = NPX(spect_iis_static   )( r_spect( o ), i ); sr_down( o ); return r; }
bl_t                 NPX(iis_link     )( sr_s o, sz_t i           ) { bl_t r = NPX(spect_iis_link     )( r_spect( o ), i ); sr_down( o ); return r; }

sz_t                 NPX(q_get_size     )( const sr_s* o                   ) { return   NPX(spect_get_size     )( r_spect( *o )               ); }
tp_t                 NPX(q_iget_name    )( const sr_s* o, sz_t i           ) { return   NPX(spect_iget_name    )( x_spect( *o ),       i      ); }
sr_s                 NPX(q_iget         )( const sr_s* o, sz_t i           ) { sr_s r = NPX(spect_iget         )( x_spect( *o ), o->o, i      ); sr_s_set_const( &r, sr_s_is_const( o ) ); return r; }
void                 NPX(q_iset         )( const sr_s* o, sz_t i, sr_s src ) {          NPX(spect_iset         )( w_spect( *o ), o->o, i, src ); }
void                 NPX(q_iset_s3      )( const sr_s* o, sz_t i, s3_t val ) {          NPX(spect_iset_s3      )( w_spect( *o ), o->o, i, val ); }
void                 NPX(q_iset_u3      )( const sr_s* o, sz_t i, u3_t val ) {          NPX(spect_iset_u3      )( w_spect( *o ), o->o, i, val ); }
void                 NPX(q_iset_f3      )( const sr_s* o, sz_t i, f3_t val ) {          NPX(spect_iset_f3      )( w_spect( *o ), o->o, i, val ); }
void                 NPX(q_iset_sz      )( const sr_s* o, sz_t i, sz_t val ) {          NPX(spect_iset_sz      )( w_spect( *o ), o->o, i, val ); }
void                 NPX(q_iset_sc      )( const sr_s* o, sz_t i, sc_t val ) {          NPX(spect_iset_sc      )( w_spect( *o ), o->o, i, val ); }
void                 NPX(q_iset_bl      )( const sr_s* o, sz_t i, bl_t val ) {          NPX(spect_iset_bl      )( w_spect( *o ), o->o, i, val ); }
void                 NPX(q_iset_tp      )( const sr_s* o, sz_t i, tp_t val ) {          NPX(spect_iset_tp      )( w_spect( *o ), o->o, i, val ); }
const bcore_vitem_s* NPX(q_iget_vitem   )( const sr_s* o, sz_t i           ) { return   NPX(spect_iget_vitem   )( r_spect( *o ),       i      ); }
const NPX(s)*        NPX(q_iget_via     )( const sr_s* o, sz_t i           ) { return   NPX(spect_iget_via     )( r_spect( *o ),       i      ); }
const bcore_array_s* NPX(q_iget_array   )( const sr_s* o, sz_t i           ) { return   NPX(spect_iget_array   )( r_spect( *o ),       i      ); }
vc_t                 NPX(q_iget_spect   )( const sr_s* o, sz_t i, tp_t stp ) { return   NPX(spect_iget_spect   )( r_spect( *o ), o->o, i, stp ); }
sz_t                 NPX(q_nget_index   )( const sr_s* o, tp_t n           ) { return   NPX(spect_nget_index   )( r_spect( *o ),       n      ); }
sr_s                 NPX(q_nget         )( const sr_s* o, tp_t n           ) { sr_s r = NPX(spect_nget         )( x_spect( *o ), o->o, n      ); sr_s_set_const( &r, sr_s_is_const( o ) ); return r; }
void                 NPX(q_nset         )( const sr_s* o, tp_t n, sr_s src ) {          NPX(spect_nset         )( w_spect( *o ), o->o, n, src ); }
void                 NPX(q_nset_s3      )( const sr_s* o, tp_t n, s3_t val ) {          NPX(spect_nset_s3      )( w_spect( *o ), o->o, n, val ); }
void                 NPX(q_nset_u3      )( const sr_s* o, tp_t n, u3_t val ) {          NPX(spect_nset_u3      )( w_spect( *o ), o->o, n, val ); }
void                 NPX(q_nset_f3      )( const sr_s* o, tp_t n, f3_t val ) {          NPX(spect_nset_f3      )( w_spect( *o ), o->o, n, val ); }
void                 NPX(q_nset_sz      )( const sr_s* o, tp_t n, sz_t val ) {          NPX(spect_nset_sz      )( w_spect( *o ), o->o, n, val ); }
void                 NPX(q_nset_sc      )( const sr_s* o, tp_t n, sc_t val ) {          NPX(spect_nset_sc      )( w_spect( *o ), o->o, n, val ); }
void                 NPX(q_nset_bl      )( const sr_s* o, tp_t n, bl_t val ) {          NPX(spect_nset_bl      )( w_spect( *o ), o->o, n, val ); }
void                 NPX(q_nset_tp      )( const sr_s* o, tp_t n, tp_t val ) {          NPX(spect_nset_tp      )( w_spect( *o ), o->o, n, val ); }
const bcore_vitem_s* NPX(q_nget_vitem   )( const sr_s* o, tp_t n           ) { return   NPX(spect_nget_vitem   )( r_spect( *o ),       n      ); }
const NPX(s)*        NPX(q_nget_via     )( const sr_s* o, tp_t n           ) { return   NPX(spect_nget_via     )( r_spect( *o ),       n      ); }
const bcore_array_s* NPX(q_nget_array   )( const sr_s* o, tp_t n           ) { return   NPX(spect_nget_array   )( r_spect( *o ),       n      ); }
vc_t                 NPX(q_nget_spect   )( const sr_s* o, tp_t n, tp_t stp ) { return   NPX(spect_nget_spect   )( r_spect( *o ), o->o, n, stp ); }
bl_t                 NPX(q_is_leaf      )( const sr_s* o                   ) { return   NPX(spect_is_leaf      )( r_spect( *o )    ); }
bl_t                 NPX(q_is_pure_array)( const sr_s* o                   ) { return   NPX(spect_is_pure_array)( r_spect( *o )    ); }
bl_t                 NPX(q_iis_array    )( const sr_s* o, sz_t i           ) { return   NPX(spect_iis_array    )( r_spect( *o ), i ); }
bl_t                 NPX(q_iis_static   )( const sr_s* o, sz_t i           ) { return   NPX(spect_iis_static   )( r_spect( *o ), i ); }
bl_t                 NPX(q_iis_link     )( const sr_s* o, sz_t i           ) { return   NPX(spect_iis_link     )( r_spect( *o ), i ); }

/**********************************************************************************************************************/

bl_t bcore_via_spect_is_leaf( const bcore_via_s* p )
{
    return p->is_leaf;
}

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

static void via_s_define_trait()
{
    bcore_trait_set( entypeof( "bcore_via_s" ), entypeof( "bcore_inst_s" ) );
}

static bcore_via_s* create_from_self( const bcore_flect_self_s* self )
{
    assert( self != NULL );

    bcore_via_s* o = via_s_create();
    o->p_type  = entypeof( "bcore_via_s" );
    o->o_type  = self->type;
    o->is_leaf = bcore_type_is_leaf( self->type );

    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( self->type );
    o->inst_p = inst_p;

    o->vitem_arr = NULL;
    sz_t vitem_arr_size  = 0;
    sz_t vitem_arr_space = 0;

    if( self->body )
    {
        for( sz_t i = 0; i < self->body->size; i++ )
        {
            const bcore_flect_item_s* flect_item = &self->body->data[ i ];
            const bcore_inst_item_s* inst_item = bcore_inst_body_s_inst_item_of_flect_item( inst_p->body, flect_item );

            if( i == 0 && flect_item->type == TYPEOF_aware_t ) continue; // self-aware type is not accessible in via
            if( inst_item && inst_item->no_trace             ) continue; // instance no-trace items (private, external, ...) are not accessible in via

            bcore_vitem_s vitem;
            bcore_memzero( &vitem, sizeof( bcore_vitem_s ) );
            vitem.name = flect_item->name;
            vitem.caps = flect_item->caps;
            vitem.offs = inst_item ? inst_item->offset : 0;
            vitem.f_shell = flect_item->f_shell;

            switch( vitem.caps )
            {
                case BCORE_CAPS_STATIC:
                case BCORE_CAPS_STATIC_LINK:
                {
                    vitem.type = flect_item->type;
                }
                break;

                case BCORE_CAPS_TYPED_LINK:
                case BCORE_CAPS_AWARE_LINK:
                    vitem.type = 0;
                    break;

                case BCORE_CAPS_STATIC_ARRAY:
                    vitem.type = bcore_static_array_type_of( flect_item->type );
                    break;

                case BCORE_CAPS_TYPED_ARRAY:
                    vitem.type = typeof( "bcore_typed_array_s" );
                    break;

                case BCORE_CAPS_STATIC_LINK_ARRAY:
                    vitem.type = bcore_static_link_array_type_of( flect_item->type );
                    break;

                case BCORE_CAPS_TYPED_LINK_ARRAY:
                    vitem.type = typeof( "bcore_typed_link_array_s" );
                    break;

                case BCORE_CAPS_AWARE_LINK_ARRAY:
                    vitem.type = typeof( "bcore_aware_link_array_s" );
                    break;

                case BCORE_CAPS_EXTERNAL_DATA:
                case BCORE_CAPS_EXTERNAL_FUNC:
                    continue; // external items are (currently) not accessible in via
                    break;

                default:
                    ERR( "Unexpected caps '%s'", bcore_flect_caps_e_sc( vitem.caps ) );
                    break;
            }

            if( vitem.type )
            {
                if( vitem.type == o->o_type )
                {
                    vitem.via_p = o;
                }
                else
                {
                    vitem.via_p = bcore_via_s_get_typed( vitem.type );
                }
            }

            // explicit setter, getter
            {

                bcore_life_s* l = bcore_life_s_create();
                vitem.fp_get = ( bcore_fp_get )bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_get" ), typeof( bcore_string_s_createf_l( l, "get_%s", ifnameof( vitem.name ) )->sc ) );
                vitem.fp_set = ( bcore_fp_set )bcore_flect_self_s_try_external_fp( self, typeof( "bcore_fp_set" ), typeof( bcore_string_s_createf_l( l, "set_%s", ifnameof( vitem.name ) )->sc ) );
                if( vitem.f_shell )
                {
                    if( !vitem.fp_get ) ERR( "Object '%s' has shell '%s' but no function 'get_%s'.", ifnameof( o->o_type ), ifnameof( vitem.name ), ifnameof( vitem.name ) );
                    if( !vitem.fp_set ) ERR( "Object '%s' has shell '%s' but no function 'set_%s'.", ifnameof( o->o_type ), ifnameof( vitem.name ), ifnameof( vitem.name ) );
                }
                bcore_life_s_discard( l );
            }

            if( vitem_arr_size == vitem_arr_space )
            {
                o->vitem_arr = bcore_un_alloc( sizeof( bcore_vitem_s ), o->vitem_arr, vitem_arr_space, vitem_arr_space > 0 ? vitem_arr_space * 2 : 1, &vitem_arr_space );
            }
            o->vitem_arr[ vitem_arr_size++ ] = vitem;
        }
    }

    o->size = vitem_arr_size;

    return o;
}

static bcore_flect_self_s* via_s_create_self( void )
{
    sc_t def = "bcore_via_s = spect { aware_t p_type; tp_t o_type; ... }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bcore_via_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )via_s_init,       "bcore_fp_init",                   "init"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )via_s_down,       "bcore_fp_down",                   "down"         );
    bcore_flect_self_s_push_external_func( self, ( fp_t )via_s_create,     "bcore_fp_create",                 "create"       );
    bcore_flect_self_s_push_external_func( self, ( fp_t )via_s_discard,    "bcore_fp_discard",                "discard"      );
    bcore_flect_self_s_push_external_func( self, ( fp_t )create_from_self, "bcore_spect_fp_create_from_self", "create_from_self" );
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

static bcore_string_s* spect_via_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_flect_define_parse_sc( "via_specs = { sz_t size; u2_t param1; s2_t; }" );
    sr_s via_specs = bcore_life_s_push_sr( l, bcore_inst_typed_create_sr( typeof( "via_specs" ) ) );
    {
        bcore_via_nset_sz( via_specs, typeof( "size"   ),  10 );
        bcore_via_nset_u3( via_specs, typeof( "param1" ), 200 );
        bcore_via_nset_s3( via_specs, typeof( "" ), -50 );
    }

    ASSERT( *( sz_t* )bcore_via_nget( via_specs, typeof( "size"   ) ).o ==  10 );
    ASSERT( *( u2_t* )bcore_via_nget( via_specs, typeof( "param1" ) ).o == 200 );
    ASSERT( *( s2_t* )bcore_via_nget( via_specs, typeof( "" ) ).o       == -50 );

    bcore_flect_define_parse_sc( "via_specs_arr = { aware_t _; u3_t flags; via_specs [] arr; }" );
    sr_s via_specs_arr = bcore_life_s_push_sr( l, bcore_inst_typed_create_sr( typeof( "via_specs_arr" ) ) );
    sr_s arr = sr_cp( bcore_life_s_push_sr( l, bcore_via_nget( via_specs_arr, typeof( "arr" ) ) ), TYPEOF_bcore_array_s );
    sz_t arr_size = 100000;

    for( sz_t i = 0; i < arr_size; i++ ) bcore_array_push( arr, via_specs );
    for( sz_t i = 0; i < arr_size; i++ ) bcore_via_nset_sz( bcore_array_get( arr, i ), typeof( "size" ), i );

    sr_s via_specs_arr2 = bcore_life_s_push_sr( l, bcore_inst_typed_create_sr( typeof( "via_specs_arr" ) ) );

    bcore_via_nset( via_specs_arr2, typeof( "arr" ), bcore_via_nget( via_specs_arr, typeof( "arr" ) ) );

    sr_s arr2 = bcore_life_s_push_sr( l, bcore_via_nget( via_specs_arr2, typeof( "arr" ) ) );

    for( sz_t i = 0; i < arr_size; i++ )
    {
        sr_s via_specs_l = bcore_life_s_push_sr( l, bcore_array_get( arr2, i ) );
        ASSERT( i == *( sz_t* )bcore_via_nget( via_specs_l, typeof( "size" ) ).o );
    }

    ASSERT( bcore_array_get_size( arr ) == arr_size );

    bcore_life_s_discard( l );
    return NULL;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_spect_via_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_spect_via" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        via_s_define_trait();
        bcore_flect_define_creator( typeof( "bcore_via_s" ), via_s_create_self );
    }
    else if( signal == typeof( "selftest" ) )
    {
        bcore_string_s_print_d( spect_via_selftest() );
    }

    return NULL;
}

