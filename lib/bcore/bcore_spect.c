/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_spect.h"
#include "bcore_hmap.h"
#include "bcore_threads.h"
#include "bcore_spect_inst.h"
#include "bcore_life.h"
#include "bcore_spect_array.h"
#include "bcore_spect_via.h"
#include "bcore_quicktypes.h"
#include "bcore_trait.h"
#include "bcore_tbman.h"

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

    bcore_mutex_t mutex;
} hmap_s;

static void hmap_s_init( hmap_s* o )
{
    bcore_arr_vd_s_init(    &o->arr );
    bcore_hmap_u2vd_s_init( &o->map );
    bcore_mutex_init( &o->mutex );
}

static void hmap_s_down( hmap_s* o )
{
    bcore_mutex_lock( &o->mutex );

    // shut down in reverse order of creation
    while( o->arr.size > 0 )
    {
        vd_t spect = bcore_arr_vd_s_pop( &o->arr );
        tp_t p_type = ( ( tp_t* )spect )[ 0 ];
        tp_t o_type = ( ( tp_t* )spect )[ 1 ];
        tp_t sig    = bcore_tp_fold_tp( p_type, o_type );
        bcore_hmap_u2vd_s_remove_h( &o->map, sig );

        /** During discard we lift the lock because the spect's destructor
         *  may still need to retrieve (even re-instantiate) other perspectives.
         *
         *  This may actually add perspectives again that have already been destroyed.
         *  This is allowed and can be handled safely at this point.
         *  Newly added perspectives will be part of o->arr and eventually taken down again.
         */
        bcore_mutex_unlock( &o->mutex );
        if( p_type == TYPEOF_bcore_inst_s )
        {
            bcore_inst_s_discard( spect );
        }
        else
        {
            bcore_inst_aware_discard( spect );
        }
        bcore_mutex_lock( &o->mutex );
    }

    bcore_hmap_u2vd_s_down( &o->map );
    bcore_arr_vd_s_down(    &o->arr );
    bcore_mutex_unlock( &o->mutex );
    bcore_mutex_down( &o->mutex );
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
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, create_hmap_s );
}

static void spect_manager_close()
{
    discard_hmap_s();
}

/// tests if the object's self reflection satisfies the requirements of a perspective
static bl_t supports( const bcore_flect_self_s* self, st_s* log )
{
    if( !self->body                                  ) return false;
    if( self->body->size < 2                         ) return false;
    if( self->body->data[ 0 ].type != TYPEOF_aware_t ) return false;
    if( self->body->data[ 1 ].type != TYPEOF_tp_t    ) return false;
    return true;
}

static void spect_define_trait()
{
    tp_t trait = entypeof( "spect" );
    bcore_trait_require_awareness( trait );
    bcore_trait_require_in_ancestry( trait );
//    bcore_trait_require_function(  trait, entypeof( "bcore_fp_init"                   ), 0 );
//    bcore_trait_require_function(  trait, entypeof( "bcore_fp_down"                   ), 0 );
//    bcore_trait_require_function(  trait, entypeof( "bcore_fp_discard"                ), 0 );
    bcore_trait_require_function(  trait, entypeof( "bcore_spect_fp_create_from_self" ), 0 );
    bcore_trait_register_fp_support( trait, supports );
    bcore_trait_set( trait, typeof( "bcore_inst" ) );
}

bl_t bcore_spect_trait_supported( tp_t spect_trait, tp_t o_type )
{
    tp_t p_type = bcore_tp_fold_sc( spect_trait, "_s" );
    tp_t sig = bcore_tp_fold_tp( p_type, o_type );
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    bl_t exists = bcore_hmap_u2vd_s_exists( &hmap_s_g->map, sig );
    bcore_mutex_unlock( &hmap_s_g->mutex );
    if( exists ) return true;
    return bcore_trait_satisfied_type( spect_trait, o_type, NULL );
}

vc_t bcore_spect_get_typed( tp_t p_type, tp_t o_type )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    tp_t sig = bcore_tp_fold_tp( p_type, o_type );
    vd_t* vdp = bcore_hmap_u2vd_s_get( &hmap_s_g->map, sig );
    if( vdp )
    {
        bcore_mutex_unlock( &hmap_s_g->mutex );
        return *vdp;
    }

    // Unlock because create_from_self may make use of registry
    bcore_mutex_unlock( &hmap_s_g->mutex );

    const bcore_flect_self_s* p_self = bcore_flect_get_self( p_type );
    const bcore_flect_self_s* o_self = bcore_flect_get_self( o_type );
    fp_t create_from_self = bcore_flect_self_s_get_external_fp( p_self, bcore_name_enroll( "bcore_spect_fp_create_from_self" ), 0 );
    vd_t spect = ( ( bcore_spect_fp_create_from_self )create_from_self )( o_self );

    // Lock for registering the perspective (if still not registered)
    bcore_mutex_lock( &hmap_s_g->mutex );

    // Due to the unlocked period, the perspective might have
    // been registered by another thread meanwhile.
    // In that case we discard spect and retrieve the registered version.
    if( bcore_hmap_u2vd_s_exists( &hmap_s_g->map, sig ) )
    {
        bcore_inst_aware_discard( spect );
        spect = *bcore_hmap_u2vd_s_get( &hmap_s_g->map, sig );
    }
    else
    {
        bcore_arr_vd_s_push( &hmap_s_g->arr, spect );
        bcore_hmap_u2vd_s_set( &hmap_s_g->map, sig, spect, false );
    }

    bcore_mutex_unlock( &hmap_s_g->mutex );
    return spect;
}

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
        bcore_mutex_lock( &hmap_s_g->mutex );
        st_s_pushf( s, "% 4zu\n", bcore_hmap_u2vd_s_keys( map ) );
        bcore_mutex_unlock( &hmap_s_g->mutex );
        st_s_push_st_d( log, s );
    }

    for( sz_t i = 0; i < map->size; i++ )
    {
        bcore_mutex_lock( &hmap_s_g->mutex );
        vd_t val = bcore_hmap_u2vd_s_idx_val( map, i );
        bcore_mutex_unlock( &hmap_s_g->mutex );
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

vd_t bcore_spect_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_spect" ) ) return NULL;
    if( signal == typeof( "init0" ) )
    {
        spect_manager_open();
    }
    else if( signal == typeof( "init1" ) )
    {
        spect_define_trait();
    }
    else if( signal == typeof( "down0" ) )
    {
        if( object && ( *( bl_t* )object ) )
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
    return NULL;
}
