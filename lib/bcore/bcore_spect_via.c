/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_spect_via.h"
#include "bcore_spect_via_call.h"
#include "bcore_spect_array.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_signal.h"
#include "bcore_signal.h"
#include "bcore_life.h"

#define NPX( name ) bcore_via_##name

/**********************************************************************************************************************/
// bcore_via_s

static void bcore_via_s_down( bcore_via_s* o )
{
    if( o->vitem_arr ) bcore_release( o->vitem_arr );
}

/**********************************************************************************************************************/

sr_s bcore_via_default_iget( const bcore_via_s* p, const bcore_via* o, uz_t index         )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem = &p->vitem_arr[ index ];
    if( vitem->fp_get )  // explicit getter
    {
        return vitem->fp_get( o );
    }
    switch( vitem->caps )
    {
        case BCORE_CAPS_SOLID_STATIC:
        {
            return sr_twd( vitem->type, ( vd_t )( ( u0_t* )o + vitem->offs ) );
        }

        case BCORE_CAPS_LINK_STATIC:
        {
            const bcore_link_static_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            return sr_twd( vitem->type, dst->link );
        }

        case BCORE_CAPS_LINK_TYPED:
        {
            const bcore_link_typed_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            return dst->link ? sr_twd( dst->type, dst->link ) : sr_null();
        }

        case BCORE_CAPS_LINK_AWARE:
        {
            const bcore_link_aware_s* dst = ( vc_t )( ( u0_t* )o + vitem->offs );
            return sr_twd( dst->link ? *(aware_t*)dst->link : 0, dst->link );
        }

        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC:
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:
        {
            return sr_twd( vitem->type, ( vd_t )( ( u0_t* )o + vitem->offs ) );
        }

        default: break;
    }

    return sr_null();
}

tp_t bcore_via_default_iget_type( const bcore_via_s* p, const bcore_via* o, uz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    const bcore_vitem_s* vitem = &p->vitem_arr[ index ];
    switch( vitem->caps )
    {
        case BCORE_CAPS_LINK_TYPED:
        case BCORE_CAPS_LINK_AWARE:
        {
            sr_s sr = bcore_via_default_iget( p, o, index );
            tp_t t = sr_s_type( &sr );
            sr_down( sr );
            return t;
        }

        default: break;
    }
    return vitem->type;
}

void bcore_via_default_iset( const bcore_via_s* p, bcore_via* o, uz_t index, sr_s src )
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
        case BCORE_CAPS_SOLID_STATIC:
        {
            vd_t dst = ( u0_t* )o + vitem->offs;
            const bcore_inst_s* inst_p = vitem->via_p->inst_p;
            if( sr_s_type( &src ) == vitem->type )
            {
                bcore_inst_p_copy( inst_p, dst, src.o );
            }
            else
            {
                bcore_inst_p_copy_typed( inst_p, dst, sr_s_type( &src ), src.o );
            }
        }
        break;

        case BCORE_CAPS_LINK_STATIC:
        {
            bcore_link_static_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            const bcore_inst_s* inst_p = vitem->via_p->inst_p;
            if( dst->link ) bcore_inst_p_discard( inst_p, dst->link );
            if( sr_s_type( &src ) == vitem->type )
            {
                dst->link = sr_s_is_strong( &src ) ? src.o : bcore_inst_p_clone( inst_p, src.o );
                src = sr_cw( src );
            }
            else
            {
                dst->link = bcore_inst_p_create_typed( inst_p, sr_s_type( &src ), src.o );
            }
        }
        break;

        case BCORE_CAPS_LINK_TYPED:
        {
            bcore_link_typed_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );
            if( dst->type && dst->link )
            {
                bcore_inst_t_discard( dst->type, dst->link );
                dst->link = NULL;
            }
            if( sr_s_type( &src ) )
            {
                dst->type = sr_s_type( &src );
                dst->link = sr_s_is_strong( &src ) ? src.o : bcore_inst_t_clone( sr_s_type( &src ), src.o );
                src = sr_cw( src );
            }
        }
        break;

        case BCORE_CAPS_LINK_AWARE:
        {
            bcore_link_aware_s* dst = ( vd_t )( ( u0_t* )o + vitem->offs );

            if( dst->link )
            {
                bcore_inst_a_discard( dst->link );
                dst->link = NULL;
            }

            if( src.o )
            {
                if( sr_s_type( &src ) )
                {
                    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( sr_s_type( &src ) );
                    if( inst_p->aware )
                    {
                        dst->link = sr_s_is_strong( &src ) ? src.o : bcore_inst_p_clone( inst_p, src.o );
                        src = sr_cw( src );
                    }
                    else
                    {
                        ERR( "Cannot convert '%s' to self-aware object", ifnameof( sr_s_type( &src ) ) );
                    }
                }
                else
                {
                    dst->link = sr_s_is_strong( &src ) ? src.o : bcore_inst_a_clone( src.o );
                    src = sr_cw( src );
                }
            }
        }
        break;

        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC:
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:
        {
            vd_t dst = ( ( u0_t* )o + vitem->offs );
            const bcore_inst_s* inst_p = vitem->via_p->inst_p;
            if( sr_s_type( &src ) == vitem->type )
            {
                bcore_inst_p_copy( inst_p, dst, src.o );
            }
            else
            {
                bcore_inst_p_copy_typed( inst_p, dst, sr_s_type( &src ), src.o );
            }
        }
        break;

        default:
            ERR( "Unsupported caps '%s'", bcore_flect_caps_e_sc( vitem->caps ) );
            break;
    }
    sr_down( src );
}

void bcore_via_default_iset_s3( const bcore_via_s* p, bcore_via* o, uz_t index, s3_t val ) { bcore_via_default_iset( p, o, index, sr_twc( TYPEOF_s3_t, &val ) ); }
void bcore_via_default_iset_u3( const bcore_via_s* p, bcore_via* o, uz_t index, u3_t val ) { bcore_via_default_iset( p, o, index, sr_twc( TYPEOF_u3_t, &val ) ); }
void bcore_via_default_iset_f3( const bcore_via_s* p, bcore_via* o, uz_t index, f3_t val ) { bcore_via_default_iset( p, o, index, sr_twc( TYPEOF_f3_t, &val ) ); }
void bcore_via_default_iset_sz( const bcore_via_s* p, bcore_via* o, uz_t index, sz_t val ) { bcore_via_default_iset( p, o, index, sr_twc( TYPEOF_sz_t, &val ) ); }
void bcore_via_default_iset_uz( const bcore_via_s* p, bcore_via* o, uz_t index, uz_t val ) { bcore_via_default_iset( p, o, index, sr_twc( TYPEOF_uz_t, &val ) ); }
void bcore_via_default_iset_sc( const bcore_via_s* p, bcore_via* o, uz_t index, sc_t val ) { bcore_via_default_iset( p, o, index, sr_twc( TYPEOF_sc_t, &val ) ); }
void bcore_via_default_iset_bl( const bcore_via_s* p, bcore_via* o, uz_t index, bl_t val ) { bcore_via_default_iset( p, o, index, sr_twc( TYPEOF_bl_t, &val ) ); }
void bcore_via_default_iset_tp( const bcore_via_s* p, bcore_via* o, uz_t index, tp_t val ) { bcore_via_default_iset( p, o, index, sr_twc( TYPEOF_tp_t, &val ) ); }

const bcore_array_s* bcore_via_default_iget_array( const bcore_via_s* p, const bcore_via* o, uz_t index )
{
    const bcore_vitem_s* vitem = &p->vitem_arr[ index ];
    if( !bcore_flect_caps_is_array( vitem->caps ) )
    {
        ERR( "%s::%s of type '%s' and capsulation '%s' is no array",
             ifnameof( p->header.o_type ),
             ifnameof( bcore_via_default_iget_name( p, o, index ) ),
             ifnameof( vitem->type ),
             bcore_flect_caps_e_sc( vitem->caps )  );
    }
    return bcore_array_s_get_typed( vitem->type );
}

vc_t bcore_via_default_iget_spect( const bcore_via_s* p, const bcore_via* o, uz_t index, tp_t spect_type )
{
    return bcore_spect_get_typed( spect_type, bcore_via_default_iget_type( p, o, index ) );
}

static inline bl_t v_test_idx( const bcore_via_s* p, tp_t name )
{
    for( uz_t i = 0; i < p->size; i++ ) if( p->vitem_arr[ i ].name == name ) return true;
    return false;
}

static inline uz_t vidx( const bcore_via_s* p, tp_t name )
{
    for( uz_t i = 0; i < p->size; i++ ) if( p->vitem_arr[ i ].name == name ) return i;
    ERR( "object '%s' has no element of name '%s'", ifnameof( p->header.o_type ), ifnameof( name ) );
    return 0;
}

bl_t                 NPX(default_nexists      )( const NPX(s)* p, const bcore_via* o, tp_t n           ) { return v_test_idx( p, n ); }
uz_t                 NPX(default_nget_index   )( const NPX(s)* p, const bcore_via* o, tp_t n           ) { return vidx(       p, n ); }
sr_s                 NPX(default_nget         )( const NPX(s)* p, const bcore_via* o, tp_t n           ) { return NPX(default_iget       )( p, o, vidx( p, n )         ); }
void                 NPX(default_nset         )( const NPX(s)* p,       bcore_via* o, tp_t n, sr_s src ) {        NPX(default_iset       )( p, o, vidx( p, n ), src    ); }
void                 NPX(default_nset_s3      )( const NPX(s)* p,       bcore_via* o, tp_t n, s3_t val ) {        NPX(default_iset_s3    )( p, o, vidx( p, n ), val    ); }
void                 NPX(default_nset_u3      )( const NPX(s)* p,       bcore_via* o, tp_t n, u3_t val ) {        NPX(default_iset_u3    )( p, o, vidx( p, n ), val    ); }
void                 NPX(default_nset_f3      )( const NPX(s)* p,       bcore_via* o, tp_t n, f3_t val ) {        NPX(default_iset_f3    )( p, o, vidx( p, n ), val    ); }
void                 NPX(default_nset_sz      )( const NPX(s)* p,       bcore_via* o, tp_t n, sz_t val ) {        NPX(default_iset_sz    )( p, o, vidx( p, n ), val    ); }
void                 NPX(default_nset_uz      )( const NPX(s)* p,       bcore_via* o, tp_t n, uz_t val ) {        NPX(default_iset_uz    )( p, o, vidx( p, n ), val    ); }
void                 NPX(default_nset_sc      )( const NPX(s)* p,       bcore_via* o, tp_t n, sc_t val ) {        NPX(default_iset_sc    )( p, o, vidx( p, n ), val    ); }
void                 NPX(default_nset_bl      )( const NPX(s)* p,       bcore_via* o, tp_t n, bl_t val ) {        NPX(default_iset_bl    )( p, o, vidx( p, n ), val    ); }
void                 NPX(default_nset_tp      )( const NPX(s)* p,       bcore_via* o, tp_t n, tp_t val ) {        NPX(default_iset_tp    )( p, o, vidx( p, n ), val    ); }
const bcore_vitem_s* NPX(default_nget_vitem   )( const NPX(s)* p, const bcore_via* o, tp_t n           ) { return NPX(default_iget_vitem )( p, o, vidx( p, n )         ); }
const NPX(s)*        NPX(default_nget_via     )( const NPX(s)* p, const bcore_via* o, tp_t n           ) { return NPX(default_iget_via   )( p, o, vidx( p, n )         ); }
const bcore_array_s* NPX(default_nget_array   )( const NPX(s)* p, const bcore_via* o, tp_t n           ) { return NPX(default_iget_array )( p, o, vidx( p, n )         ); }
vc_t                 NPX(default_nget_spect   )( const NPX(s)* p, const bcore_via* o, tp_t n, tp_t stp ) { return NPX(default_iget_spect )( p, o, vidx( p, n ), stp    ); }

bl_t bcore_via_default_is_leaf( const bcore_via_s* p, const bcore_via* o )
{
    return p->is_leaf;
}

bl_t bcore_via_default_is_pure_array( const bcore_via_s* p, const bcore_via* o )
{
    return ( ( p->size == 1 ) && bcore_via_default_iis_array( p, o, 0 ) );
}

bl_t bcore_via_default_iis_array( const bcore_via_s* p, const bcore_via* o, uz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    return bcore_flect_caps_is_array( p->vitem_arr[ index ].caps );
}

bl_t bcore_via_default_iis_static( const bcore_via_s* p, const bcore_via* o, uz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    switch( p->vitem_arr[ index ].caps )
    {
        case BCORE_CAPS_SOLID_STATIC:           return true;
        case BCORE_CAPS_LINK_STATIC:            return true;
        case BCORE_CAPS_LINK_TYPED:             return false;
        case BCORE_CAPS_LINK_AWARE:             return false;
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return true;
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return true;
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   return true;
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return true;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   return true;
        default: ERR( "Unhandled encapsulation '%s'", bcore_flect_caps_e_sc( p->vitem_arr[ index ].caps ) );
    }
    return false;
}

bl_t bcore_via_default_iis_link( const bcore_via_s* p, const bcore_via* o, uz_t index )
{
    if( index >= p->size ) ERR( "index (%zu) out of range (%zu)", index, p->size );
    switch( p->vitem_arr[ index ].caps )
    {
        case BCORE_CAPS_SOLID_STATIC:           return false;
        case BCORE_CAPS_LINK_STATIC:            return true;
        case BCORE_CAPS_LINK_TYPED:             return true;
        case BCORE_CAPS_LINK_AWARE:             return true;
        case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC: return false;
        case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:  return false;
        case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:  return false;
        case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:   return false;
        case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:   return false;
        case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC: return false;
        case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:  return false;
        case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:   return false;
        default: ERR( "Unhandled encapsulation '%s'", bcore_flect_caps_e_sc( p->vitem_arr[ index ].caps ) );
    }
    return false;
}

void bcore_via_default_source( const bcore_via_s* p, bcore_via* o, bcore_source* source )
{
    if( p->via_call_p->source ) p->via_call_p->source( ( bcore_via_call* ) o, source );
}

void bcore_via_default_mutated( const bcore_via_s* p, bcore_via* o )
{
    if( p->via_call_p->mutated ) p->via_call_p->mutated( ( bcore_via_call* ) o );
}

void bcore_via_default_shelve( const bcore_via_s* p, bcore_via* o )
{
    if( p->via_call_p->shelve ) p->via_call_p->shelve( ( bcore_via_call* ) o );
}

bl_t bcore_via_default_defines_shelve( const bcore_via_s* p, bcore_via* o )
{
    return ( p->via_call_p->shelve ) ? true : false;
}

/**********************************************************************************************************************/

static bcore_via_s* bcore_via_s_create_from_self( const bcore_self_s* self )
{
    assert( self != NULL );

    bcore_via_s* o = bcore_inst_t_create( entypeof( "bcore_via_s" ) );
    o->header.p_type  = typeof( "bcore_via_s" );
    o->header.o_type  = self->type;
    o->is_leaf = bcore_type_is_leaf( self->type );

    const bcore_inst_s* inst_p = bcore_inst_s_get_typed( self->type );
    o->inst_p = inst_p;
    o->via_call_p = bcore_via_call_s_get_typed( self->type );

    o->vitem_arr = NULL;
    uz_t vitem_arr_size  = 0;
    uz_t vitem_arr_space = 0;

    uz_t items_size = bcore_self_s_items_size( self );
    for( uz_t i = 0; i < items_size; i++ )
    {
        const bcore_self_item_s* self_item = bcore_self_s_get_item( self, i );

        if( self_item->flags.f_hidden                   ) continue; // hidden items are not accessible in via
        if( i == 0 && self_item->type == TYPEOF_aware_t ) continue; // self-aware type is not accessible in via

        const bcore_inst_item_s* inst_item = bcore_inst_s_get_item_from_self_item( inst_p, self_item );

        if( inst_item && inst_item->no_trace ) continue; // instance no-trace items (private, external, ...) are not accessible in via
        if( self_item->flags.f_const ) continue; // constants are not accessible in via

        bcore_vitem_s vitem;
        bcore_memzero( &vitem, sizeof( bcore_vitem_s ) );
        vitem.name = self_item->name;
        vitem.caps = self_item->caps;
        vitem.offs = inst_item ? inst_item->offset : 0;
        vitem.flags = self_item->flags;

        switch( vitem.caps )
        {
            case BCORE_CAPS_SOLID_STATIC:
            case BCORE_CAPS_LINK_STATIC:
            {
                vitem.type = self_item->type;
            }
            break;

            case BCORE_CAPS_LINK_TYPED:
            case BCORE_CAPS_LINK_AWARE:
                vitem.type = 0;
                break;

            case BCORE_CAPS_ARRAY_DYN_SOLID_STATIC:
                vitem.type = bcore_array_dyn_solid_static_type_of( self_item->type );
                break;

            case BCORE_CAPS_ARRAY_DYN_SOLID_TYPED:
                vitem.type = typeof( "bcore_array_dyn_solid_typed_s" );
                break;

            case BCORE_CAPS_ARRAY_DYN_LINK_STATIC:
                vitem.type = bcore_array_dyn_link_static_type_of( self_item->type );
                break;

            case BCORE_CAPS_ARRAY_DYN_LINK_TYPED:
                vitem.type = typeof( "bcore_array_dyn_link_typed_s" );
                break;

            case BCORE_CAPS_ARRAY_DYN_LINK_AWARE:
                vitem.type = typeof( "bcore_array_dyn_link_aware_s" );
                break;

            case BCORE_CAPS_ARRAY_FIX_SOLID_STATIC:
                vitem.type = bcore_array_fix_solid_static_type_of( self_item->type, self_item->array_fix_size );
                break;

            case BCORE_CAPS_ARRAY_FIX_LINK_STATIC:
                vitem.type = bcore_array_fix_link_static_type_of( self_item->type, self_item->array_fix_size );
                break;

            case BCORE_CAPS_ARRAY_FIX_LINK_AWARE:
                vitem.type = bcore_array_fix_link_aware_type_of( self_item->array_fix_size );
                break;

            case BCORE_CAPS_EXTERNAL_FUNC:
                continue; // external items are (currently) not accessible in via
                break;

            default:
                ERR( "Unexpected caps '%s'", bcore_flect_caps_e_sc( vitem.caps ) );
                break;
        }

        if( vitem.type )
        {
            if( vitem.type == o->header.o_type )
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
            vitem.fp_get = ( bcore_fp_get )bcore_self_s_try_external_fp( self, typeof( "bcore_fp_get" ), typeof( st_s_createf_l( l, "get_%s", ifnameof( vitem.name ) )->sc ) );
            vitem.fp_set = ( bcore_fp_set )bcore_self_s_try_external_fp( self, typeof( "bcore_fp_set" ), typeof( st_s_createf_l( l, "set_%s", ifnameof( vitem.name ) )->sc ) );
            if( vitem.flags.f_shell )
            {
                if( !vitem.fp_get ) ERR( "Object '%s' has shell '%s' but no function 'get_%s'.", ifnameof( o->header.o_type ), ifnameof( vitem.name ), ifnameof( vitem.name ) );
                if( !vitem.fp_set ) ERR( "Object '%s' has shell '%s' but no function 'set_%s'.", ifnameof( o->header.o_type ), ifnameof( vitem.name ), ifnameof( vitem.name ) );
            }
            bcore_life_s_discard( l );
        }

        if( vitem_arr_size == vitem_arr_space )
        {
            o->vitem_arr = bcore_un_alloc( sizeof( bcore_vitem_s ), o->vitem_arr, vitem_arr_space, vitem_arr_space > 0 ? vitem_arr_space * 2 : 1, &vitem_arr_space );
        }
        o->vitem_arr[ vitem_arr_size++ ] = vitem;
    }

    o->size = vitem_arr_size;

    return o;
}

BCORE_DEFINE_SPECT( bcore_inst, bcore_via )
"{"
    "bcore_spect_header_s header;"
    "private bcore_inst_s* inst_p;"
    "private bcore_via_call_s* via_call_p;"
    "private uz_t  size;"
    "private bl_t  is_leaf;"
    "private vd_t vitem_arr;"

    "func bcore_spect_fp:create_from_self;"
    "func bcore_fp:down;"
"}";

/**********************************************************************************************************************/

sr_s bcore_spect_via_create_zoo( uz_t size )
{
    bcore_life_s* l = bcore_life_s_create();
                      bcore_flect_type_parse_sc( "vectors = { st_s => [ 2 ] data1; u3_t [ 3 ] data2; }" );
    tp_t t_animal   = bcore_flect_type_parse_sc( "animal = { st_s => type; f3_t weight; st_s => [] features; vectors vdata; }" );
    tp_t t_compound = bcore_flect_type_parse_sc( "compound = { u3_t id; uz_t area; animal => [] animals; }" );
    tp_t t_zoo      = bcore_flect_type_parse_sc( "zoo = { st_s => name; typed => [] compounds; }" );

    sr_s ret = bcore_inst_t_create_sr( t_zoo );
    sr_s zoo = sr_cw( ret );

    bcore_via_x_nset_sc( zoo, typeof( "name" ), "Mesamurial" );
    sr_s compounds = bcore_life_s_push_sr( l, bcore_via_x_nget( zoo, typeof( "compounds" ) ) );
    {
        sr_s compound = sr_cl( bcore_inst_t_create_sr( t_compound ), l );
        bcore_via_x_nset_u3( compound, typeof( "id" ), 254 );
        bcore_via_x_nset_uz( compound, typeof( "area" ), 1000 );
        sr_s animals = bcore_life_s_push_sr( l, bcore_via_x_nget( compound, typeof( "animals" ) ) );
        {
            sr_s bird = sr_cl( bcore_inst_t_create_sr( t_animal ), l );
            bcore_via_x_nset_sc( bird, typeof( "type" ), "Owl" );
            bcore_via_x_nset_f3( bird, typeof( "weight" ), 2.5 );
            sr_s features = sr_cl( bcore_via_x_nget( bird, typeof( "features" ) ), l );
            {
                bcore_array_x_push_sc( features, "Night active" );
                bcore_array_x_push_sc( features, "Can fly" );
            }
            bcore_array_x_push( animals, bird );
        }
        {
            sr_s bird = sr_cl( bcore_inst_t_create_sr( t_animal ), l );
            bcore_via_x_nset_sc( bird, typeof( "type" ), "Pidgin" );
            bcore_via_x_nset_f3( bird, typeof( "weight" ), 0.5 );
            sr_s features = sr_cl( bcore_via_x_nget( bird, typeof( "features" ) ), l );
            {
                bcore_array_x_push_sc( features, "Day active" );
                bcore_array_x_push_sc( features, "Can fly" );
            }
            bcore_array_x_push( animals, bird );
        }
        bcore_array_x_push( compounds, compound );
        bcore_array_x_push( compounds, sr_null() );
        bcore_array_x_push( compounds, compound );
        bcore_array_x_push( compounds, sr_null() );
        bcore_array_x_push( compounds, sr_null() );
        for( uz_t i = 5; i < size; i++ ) bcore_array_x_push( compounds, compound );
    }
    bcore_life_s_discard( l );

    return ret;
}

static st_s* spect_via_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    bcore_flect_define_parse_sc( "via_specs = { uz_t size = 3; u2_t param1 = 1234; s2_t = -5; }" );
    sr_s via_specs = bcore_life_s_push_sr( l, bcore_inst_t_create_sr( typeof( "via_specs" ) ) );

    ASSERT( *( uz_t* )bcore_via_x_nget( via_specs, typeof( "size"   ) ).o ==    3 );
    ASSERT( *( u2_t* )bcore_via_x_nget( via_specs, typeof( "param1" ) ).o == 1234 );
    ASSERT( *( s2_t* )bcore_via_x_nget( via_specs, typeof( "" ) ).o       ==   -5 );

    {
        bcore_via_x_nset_uz( via_specs, typeof( "size"   ),  10 );
        bcore_via_x_nset_u3( via_specs, typeof( "param1" ), 200 );
        bcore_via_x_nset_s3( via_specs, typeof( "" ), -50 );
    }

    ASSERT( *( uz_t* )bcore_via_x_nget( via_specs, typeof( "size"   ) ).o ==  10 );
    ASSERT( *( u2_t* )bcore_via_x_nget( via_specs, typeof( "param1" ) ).o == 200 );
    ASSERT( *( s2_t* )bcore_via_x_nget( via_specs, typeof( "" ) ).o       == -50 );

    bcore_flect_define_parse_sc( "via_specs_arr = { aware_t _; u3_t flags; via_specs [] arr; }" );
    sr_s via_specs_arr = bcore_life_s_push_sr( l, bcore_inst_t_create_sr( typeof( "via_specs_arr" ) ) );
    sr_s arr = sr_cp( bcore_life_s_push_sr( l, bcore_via_x_nget( via_specs_arr, typeof( "arr" ) ) ), TYPEOF_bcore_array_s );
    uz_t arr_size = 100000;

    for( uz_t i = 0; i < arr_size; i++ ) bcore_array_x_push( arr, via_specs );
    for( uz_t i = 0; i < arr_size; i++ ) bcore_via_x_nset_uz( bcore_array_x_get( arr, i ), typeof( "size" ), i );

    sr_s via_specs_arr2 = bcore_life_s_push_sr( l, bcore_inst_t_create_sr( typeof( "via_specs_arr" ) ) );

    bcore_via_x_nset( via_specs_arr2, typeof( "arr" ), bcore_via_x_nget( via_specs_arr, typeof( "arr" ) ) );

    sr_s arr2 = bcore_life_s_push_sr( l, bcore_via_x_nget( via_specs_arr2, typeof( "arr" ) ) );

    for( uz_t i = 0; i < arr_size; i++ )
    {
        sr_s via_specs_l = bcore_life_s_push_sr( l, bcore_array_x_get( arr2, i ) );
        ASSERT( i == *( uz_t* )bcore_via_x_nget( via_specs_l, typeof( "size" ) ).o );
    }

    ASSERT( bcore_array_x_get_size( arr ) == arr_size );

    bcore_life_s_discard( l );
    return NULL;
}

/**********************************************************************************************************************/
// signal

BCORE_DEFINE_SPECT_CACHE( bcore_via_s );

vd_t bcore_spect_via_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_via" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_trait_set( entypeof( "bcore_via" ), entypeof( "bcore_inst" ) );
            bcore_flect_define_creator( typeof( "bcore_via_s" ), bcore_via_s_create_self );
            bcore_spect_setup_cache( &bcore_via_s_cache_g );

            BCORE_REGISTER_FFUNC( bcore_fp_down, bcore_via_s_down );
            BCORE_REGISTER_FFUNC( bcore_spect_fp_create_from_self, bcore_via_s_create_from_self );
        }
        break;

        case TYPEOF_selftest:
        {
            return spect_via_selftest();
        }
        break;

        default: break;
    }
    return NULL;
}
