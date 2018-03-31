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

#include "bcore_spect.h"
#include "bcore_hmap.h"
#include "bcore_threads.h"
#include "bcore_spect_inst.h"
#include "bcore_life.h"
#include "bcore_spect_array.h"
#include "bcore_spect_via.h"
#include "bcore_signal.h"
#include "bcore_trait.h"
#include "bcore_tbman.h"

/**********************************************************************************************************************/
// bcore_spect_header_s

BCORE_DEFINE_OBJECT_FLAT( bcore_spect_header_s, "bcore_spect_header_s = { tp_t p_type; tp_t o_type; }")

/**********************************************************************************************************************/
// hash map

typedef struct hmap_s
{
    /** We use the the array to keep instances in the order created.
     *  Shut down happens in the reverse order.
     *  This ensures that references to perspectives created during
     *  initialization are still valid during shutting down.
     *  Cyclic references are not possible because perspectives are stateless.
     */
    bcore_arr_vd_s arr;

    /// The map is used to access instances.
    bcore_hmap_u2vd_s map;

    bcore_mutex_s mutex;
} hmap_s;

static void hmap_s_init( hmap_s* o )
{
    bcore_arr_vd_s_init(    &o->arr );
    bcore_hmap_u2vd_s_init( &o->map );
    bcore_mutex_s_init( &o->mutex );
}

static void hmap_s_down( hmap_s* o )
{
    bcore_mutex_s_lock( &o->mutex );

    // shut down in reverse order of creation
    while( o->arr.size > 0 )
    {
        vd_t spect = bcore_arr_vd_s_pop( &o->arr );
        tp_t p_type = ( ( bcore_spect_header_s* )spect )->p_type;
        tp_t o_type = ( ( bcore_spect_header_s* )spect )->o_type;
        tp_t sig    = bcore_tp_fold_tp( p_type, o_type );
        bcore_hmap_u2vd_s_remove_h( &o->map, sig );

        /** During discard we lift the lock because the spect's destructor
         *  may retrieve (even recreate) perspective instances.
         *
         *  This could re-add perspectives that have already been destroyed, giving those a different address.
         *  Nevertheless, this is safe at this point for the following reasons:
         *    1. Non-perspective objects, possibly referencing perspectives, have already been taken down.
         *    2. Perspective B can only reference perspective A when A was added before B to the registry.
         *       Shutting down in reverse order ensures that A is never destroyed before B.
         *    3. (follows from 2) Cyclic references are not possible.
         *
         *  Temporarily re-added perspectives reside at the tail-end of o->arr
         *  and are taken down again in this loop without cyclic re-instantiation.
         *  (Halting is guaranteed.)
         */
        bcore_mutex_s_unlock( &o->mutex );
        if( p_type == TYPEOF_bcore_inst_s )
        {
            bcore_inst_s_discard( spect );
        }
        else
        {
            bcore_inst_aware_discard( spect );
        }
        bcore_mutex_s_lock( &o->mutex );
    }

    bcore_hmap_u2vd_s_down( &o->map );
    bcore_arr_vd_s_down(    &o->arr );
    bcore_mutex_s_unlock( &o->mutex );
    bcore_mutex_s_down( &o->mutex );
}

/**********************************************************************************************************************/

static hmap_s* hmap_s_g = NULL;

static void create_hmap_s()
{
    if( hmap_s_g == NULL )
    {
        hmap_s_g = bcore_alloc( NULL, sizeof( hmap_s ) );
        hmap_s_init( hmap_s_g );
    }
}

static void discard_hmap_s()
{
    if( hmap_s_g )
    {
        bcore_release_obj( ( fp_t )hmap_s_down, hmap_s_g );
        hmap_s_g = NULL;
    }
}

static void spect_manager_open()
{
    static bcore_once_s flag = bcore_once_init;
    bcore_once_s_run( &flag, create_hmap_s );
}

static void spect_manager_close()
{
    discard_hmap_s();
}

/// tests if the object's self reflection satisfies the requirements of a perspective
static bl_t supports( const bcore_self_s* self, st_s* log )
{
    if( !self->body                                ) return false;
    if( bcore_self_s_items_size( self ) < 2  ) return false;
    if( bcore_self_s_get_item( self, 0 )->type != TYPEOF_bcore_spect_header_s )
    {
        if( bcore_self_s_get_item( self, 0 )->type != TYPEOF_aware_t ) return false;
        if( bcore_self_s_get_item( self, 1 )->type != TYPEOF_tp_t    ) return false;
    }

    return true;
}

static void spect_define_trait()
{
    tp_t trait = entypeof( "spect" );
    bcore_trait_require_awareness( trait );
    bcore_trait_require_in_ancestry( trait );

//    bcore_trait_require_function(  trait, entypeof( "bcore_spect_fp_create_from_self" ), 0 );
//    now optional; missing 'bcore_spect_fp_create_from_self' falls back to genric function bcore_spect_create_from_self

    bcore_trait_register_fp_support( trait, supports );
    bcore_trait_set( trait, typeof( "bcore_inst" ) );
}

bl_t bcore_spect_trait_supported( tp_t spect_trait, tp_t o_type )
{
    tp_t p_type = bcore_tp_fold_sc( spect_trait, "_s" );
    tp_t sig = bcore_tp_fold_tp( p_type, o_type );
    assert( hmap_s_g != NULL );
    bcore_mutex_s_lock( &hmap_s_g->mutex );
    bl_t exists = bcore_hmap_u2vd_s_exists( &hmap_s_g->map, sig );
    bcore_mutex_s_unlock( &hmap_s_g->mutex );
    if( exists ) return true;
    return bcore_trait_satisfied_type( spect_trait, o_type, NULL );
}

vc_t bcore_spect_get_typed( tp_t p_type, tp_t o_type )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_s_lock( &hmap_s_g->mutex );
    tp_t sig = bcore_tp_fold_tp( p_type, o_type );
    vd_t* vdp = bcore_hmap_u2vd_s_get( &hmap_s_g->map, sig );
    if( vdp )
    {
        bcore_mutex_s_unlock( &hmap_s_g->mutex );
        return *vdp;
    }

    // Unlock because create_from_self may make use of registry
    bcore_mutex_s_unlock( &hmap_s_g->mutex );

    const bcore_self_s* p_self = bcore_flect_get_self( p_type );
    const bcore_self_s* o_self = bcore_flect_get_self( o_type );
    vd_t spect = NULL;
    vd_t discard_spect = NULL; // in case multiple threads try to register, redundant creations must be discarded

    fp_t create_from_self = bcore_self_s_try_external_fp( p_self, bcore_name_enroll( "bcore_spect_fp_create_from_self" ), 0 );
    if( create_from_self )
    {
        spect = ( ( bcore_spect_fp_create_from_self )create_from_self )( o_self );
    }
    else // use generic creation
    {
        spect = bcore_spect_create_from_self( p_self, o_self );
    }

    // Lock for registering the perspective (if still not registered)
    bcore_mutex_s_lock( &hmap_s_g->mutex );

    // Due to the unlocked period, the perspective might have
    // been registered by another thread meanwhile.
    // In that case we discard spect and retrieve the registered version.
    if( bcore_hmap_u2vd_s_exists( &hmap_s_g->map, sig ) )
    {
        discard_spect = spect;
        spect = *bcore_hmap_u2vd_s_get( &hmap_s_g->map, sig );
    }
    else
    {
        bcore_arr_vd_s_push( &hmap_s_g->arr, spect );
        bcore_hmap_u2vd_s_set( &hmap_s_g->map, sig, spect, false );
    }

    bcore_mutex_s_unlock( &hmap_s_g->mutex );

    if( discard_spect ) bcore_inst_aware_discard( discard_spect );
    return spect;
}

/**********************************************************************************************************************/

void bcore_spect_define_trait( const bcore_self_s* p_self )
{
    assert( p_self != NULL );

    tp_t trait = 0;
    {
        st_s* p_name = st_s_create_sc( ifnameof( p_self->type ) );
        if( p_name->size < 3 || p_name->sc[ p_name->size - 2 ] != '_' || p_name->sc[ p_name->size - 1 ] != 's' )
        {
            ERR_fa( "Name of perspective '#<sc_t>' should end in '_s'", p_name->sc );
        }
        p_name->data[ p_name->size - 2 ] = 0;
        trait = entypeof( p_name->sc );
        st_s_discard( p_name );
    }

    /// check requirements
    bcore_spect_header_s* spect = bcore_inst_typed_create( p_self->type );
    const bcore_via_s* p_via = bcore_via_s_get_typed( p_self->type );
    sz_t size = bcore_via_spect_get_size( p_via );
    for( sz_t i = 0; i < size; i++ )
    {
        const bcore_vitem_s* vitem = bcore_via_spect_iget_vitem( p_via, i );
        if( vitem->flags.f_feature && vitem->flags.f_strict )
        {
            if( vitem->flags.f_fp )
            {
                sr_s dst = bcore_via_spect_nget( p_via, spect, vitem->name );
                fp_t* dst_fp = ( fp_t* )dst.o;
                if( vitem->flags.f_strict && !*dst_fp )
                {
                    bcore_trait_require_function( trait, vitem->type, vitem->name );
                }
                sr_down( dst );
            }
            else
            {
                WRN_fa
                (
                    "Defining trait of perspective '#<sc_t>': Feature '#<sc_t> #<sc_t>' is not supported.",
                    ifnameof( p_self->type ),
                    vitem->type,
                    vitem->name
                );
            }
        }
    }

    bcore_inst_aware_discard( spect );

    bcore_trait_set( trait, entypeof( "bcore_inst" ) );
}

vd_t bcore_spect_create_from_self( const bcore_self_s* p_self, const bcore_self_s* o_self )
{
    assert( p_self != NULL );
    assert( o_self != NULL );

    const bcore_via_s* p_via = bcore_via_s_get_typed( p_self->type );
    bcore_spect_header_s* o = bcore_inst_typed_create( p_self->type );
    o->o_type = o_self->type;

    sz_t size = bcore_via_spect_get_size( p_via );
    for( sz_t i = 0; i < size; i++ )
    {
        const bcore_vitem_s* vitem = bcore_via_spect_iget_vitem( p_via, i );
        if( vitem->flags.f_feature )
        {
            if( vitem->flags.f_fp )
            {
                sr_s dst = bcore_via_spect_nget( p_via, o, vitem->name );
                if( dst.o )
                {
                    fp_t src_fp = bcore_self_s_try_external_fp( o_self, vitem->type, vitem->name );
                    fp_t* dst_fp = ( fp_t* )dst.o;

                    if( vitem->flags.f_strict && !*dst_fp && !src_fp )
                    {
                        WRN_fa
                        (
                            "Creating perspective '#<sc_t>': Feature '#<sc_t> #<sc_t>' not found in '#<sc_t>'.",
                            ifnameof( p_self->type ),
                            ifnameof( vitem->type ),
                            ifnameof( vitem->name ),
                            ifnameof( o_self->type )
                        );
                    }

                    if( src_fp ) *dst_fp = src_fp;
                }
                else
                {
                    WRN_fa
                    (
                        "Creating perspective '#<sc_t>': Feature '#<sc_t> #<sc_t>' not found in perspective.",
                        ifnameof( p_self->type ),
                        ifnameof( vitem->type ),
                        ifnameof( vitem->name )
                    );
                }

                sr_down( dst );
            }
            else
            {
                WRN_fa
                (
                    "Creating perspective '#<sc_t>': Feature '#<sc_t> #<sc_t>' is not supported.",
                    ifnameof( p_self->type ),
                    vitem->type,
                    vitem->name
                );
            }
        }
    }

    return o;
}

void bcore_spect_define_creator( tp_t type, bcore_flect_create_self_fp creator )
{
    bcore_flect_define_creator( type, creator );
    bcore_self_s* self = creator();
    bcore_spect_define_trait( self );
    bcore_self_s_discard( self );
}

/**********************************************************************************************************************/

st_s* bcore_spect_status()
{
    bcore_life_s* l = bcore_life_s_create();
    assert( hmap_s_g      != NULL );
    const bcore_hmap_u2vd_s* map  = &hmap_s_g->map;
    bcore_hmap_tpsz_s* hist = bcore_life_s_push_aware( l, bcore_hmap_tpsz_s_create() );
    st_s* log = st_s_create();

    {
        st_s* s = st_s_create();
        st_s_push_sc( s, "registered perspectives " );
        st_s_push_char_n( s, '.', 28 - s->size );
        bcore_mutex_s_lock( &hmap_s_g->mutex );
        st_s_pushf( s, "% 4zu\n", bcore_hmap_u2vd_s_keys( map ) );
        bcore_mutex_s_unlock( &hmap_s_g->mutex );
        st_s_push_st_d( log, s );
    }

    for( sz_t i = 0; i < map->size; i++ )
    {
        bcore_mutex_s_lock( &hmap_s_g->mutex );
        vd_t val = bcore_hmap_u2vd_s_idx_val( map, i );
        bcore_mutex_s_unlock( &hmap_s_g->mutex );
        if( val ) ( *bcore_hmap_tpsz_s_fget( hist, *( aware_t* )val, 0 ) )++;
    }

    sr_s nc_arr = sr_cl( bcore_inst_typed_create_sr( bcore_flect_type_parse_fa( "{ { sz_t count; tp_t type; } [] arr; }" ) ), l );

    for( sz_t i = 0; i < hist->size; i++ )
    {
        tp_t key   = bcore_hmap_tpsz_s_idx_key( hist, i );
        sz_t count = bcore_hmap_tpsz_s_idx_val( hist, i );
        if( key )
        {
            bcore_array_push( nc_arr, sr_null() );
            sr_s pair = sr_cl( bcore_array_get_last( nc_arr ), l );
            bcore_via_nset_tp( pair, typeof( "type" ),  key   );
            bcore_via_nset_sz( pair, typeof( "count" ), count );
        }
    }

    bcore_array_sort( nc_arr, 0, -1, -1 );

    for( sz_t i = 0; i < bcore_array_get_size( nc_arr ); i++ )
    {
        sr_s pair = sr_cl( bcore_array_get( nc_arr, i ), l );
        const tp_t* p_type  = sr_cl( bcore_via_nget( pair, typeof( "type"  ) ), l ).o;
        const sz_t* p_count = sr_cl( bcore_via_nget( pair, typeof( "count" ) ), l ).o;
        st_s* s = st_s_create();
        st_s_pushf( s, "  %s ", ifnameof( *p_type ) );
        st_s_push_char_n( s, '.', 28 - s->size );
        st_s_pushf( s, "% 4zu\n", *p_count );
        st_s_push_st_d( log, s );
    }

    bcore_life_s_discard( l );
    return log;
}

/**********************************************************************************************************************/
// syntactic sugar

vc_t ch_spect_p( vc_t p, tp_t spect_type )
{
    if( p && ( *(aware_t*)p != spect_type ) )
    {
        return bcore_spect_get_typed( spect_type, ( ( tp_t* )p )[ 1 ] );
    }
    return p;
}

vc_t ch_spect_o( vc_t p, tp_t obj_type )
{
    if( p && ( ( ( tp_t* )p )[ 1 ] != obj_type ) )
    {
        return bcore_spect_get_typed( *(aware_t*)p, obj_type );
    }
    return p;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_spect_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect" ) ) )
    {
        case TYPEOF_init0:
        {
            spect_manager_open();
        }
        break;

        case TYPEOF_init1:
        {
            spect_define_trait();
            BCORE_REGISTER_FLECT( bcore_spect_header_s );
        }
        break;

        case TYPEOF_down0:
        {
            if( o->object && ( *( bl_t* )o->object ) )
            {
                sz_t space = bcore_tbman_granted_space();
                spect_manager_close();
                bcore_msg( "  spect mananger ...... % 6zu\n", space - bcore_tbman_granted_space() );
            }
            else
            {
                spect_manager_close();
            }
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}
