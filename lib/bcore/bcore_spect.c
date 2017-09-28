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
    bcore_hmap_u2vd_s* map;
    bcore_mutex_t mutex;
} hmap_s;

static void hmap_s_init( hmap_s* o )
{
    o->map = bcore_hmap_u2vd_s_create();
    bcore_mutex_init( &o->mutex );
}

static void hmap_s_node_down( vd_t obj, u2_t key, vd_t val )
{
    if( val )
    {
        bcore_fp_discard discard = bcore_flect_self_s_get_external_fp( bcore_flect_get_self( *( aware_t *)val ), typeof( "bcore_fp_discard" ), 0 );
        discard( val );
    }
}

static void hmap_s_down( hmap_s* o )
{
    bcore_mutex_lock( &o->mutex );
    if( o->map )
    {
        bcore_hmap_u2vd_s_run_c( o->map, NULL, hmap_s_node_down );
        bcore_hmap_u2vd_s_discard( o->map );
        o->map = NULL;
    }
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
        hmap_s_down( hmap_s_g );
        hmap_s_g = bcore_free( hmap_s_g );
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
static bl_t supports( const bcore_flect_self_s* self, bcore_string_s* log )
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
    bcore_trait_require_function(  trait, entypeof( "bcore_fp_init"                   ), 0 );
    bcore_trait_require_function(  trait, entypeof( "bcore_fp_down"                   ), 0 );
    bcore_trait_require_function(  trait, entypeof( "bcore_fp_discard"                ), 0 );
    bcore_trait_require_function(  trait, entypeof( "bcore_spect_fp_create_from_self" ), 0 );
    bcore_trait_register_fp_support( trait, supports );
    bcore_trait_set( trait, 0 );
}


bl_t bcore_spect_supported( tp_t p_type, tp_t o_type )
{
    tp_t sig = bcore_signature_get_hash_tp_tp( p_type, o_type );
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    bl_t exists = bcore_hmap_u2vd_s_exists( hmap_s_g->map, sig );
    bcore_mutex_unlock( &hmap_s_g->mutex );
    if( exists ) return true;
    return bcore_trait_satisfied_type( p_type, o_type, NULL );
}

vc_t bcore_spect_get_typed( tp_t p_type, tp_t o_type )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    tp_t sig = bcore_signature_get_hash_tp_tp( p_type, o_type );
    vd_t* vdp = bcore_hmap_u2vd_s_get( hmap_s_g->map, sig );
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
    if( bcore_hmap_u2vd_s_exists( hmap_s_g->map, sig ) )
    {
        bcore_inst_aware_discard( spect );
        spect = *bcore_hmap_u2vd_s_get( hmap_s_g->map, sig );
    }
    else
    {
        bcore_hmap_u2vd_s_set( hmap_s_g->map, sig, spect, false );
    }

    bcore_mutex_unlock( &hmap_s_g->mutex );
    return spect;
}

bcore_string_s* bcore_spect_status()
{
    bcore_life_s* l = bcore_life_s_create();
    assert( hmap_s_g      != NULL );
    bcore_hmap_u2vd_s* map  = hmap_s_g->map;
    bcore_hmap_tpsz_s* hist = bcore_life_s_push_aware( l, bcore_hmap_tpsz_s_create() );
    bcore_string_s* log = bcore_string_s_create();

    {
        bcore_string_s* s = bcore_string_s_create();
        bcore_string_s_push_sc( s, "registered perspectives " );
        bcore_string_s_push_char_n( s, '.', 28 - s->size );
        bcore_mutex_lock( &hmap_s_g->mutex );
        bcore_string_s_pushf( s, "% 4zu\n", bcore_hmap_u2vd_s_keys( map ) );
        bcore_mutex_unlock( &hmap_s_g->mutex );
        bcore_string_s_push_string_d( log, s );
    }

    for( sz_t i = 0; i < map->size; i++ )
    {
        bcore_mutex_lock( &hmap_s_g->mutex );
        vd_t val = bcore_hmap_u2vd_s_idx_val( map, i );
        bcore_mutex_unlock( &hmap_s_g->mutex );
        if( val ) ( *bcore_hmap_tpsz_s_fget( hist, *( aware_t* )val, 0 ) )++;
    }

    sr_s nc_arr = sr_cl( bcore_inst_typed_create_sr( bcore_flect_type_parsef( "{ { sz_t count; tp_t type; } [] arr; }" ) ), l );

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
        bcore_string_s* s = bcore_string_s_create();
        bcore_string_s_pushf( s, "  %s ", ifnameof( *p_type ) );
        bcore_string_s_push_char_n( s, '.', 28 - s->size );
        bcore_string_s_pushf( s, "% 4zu\n", *p_count );
        bcore_string_s_push_string_d( log, s );
    }

    bcore_life_s_discard( l );
    return log;
}

/**********************************************************************************************************************/
// syntactic sugar

vc_t ch_spect( vc_t p, tp_t spect_type )
{
    if( p && ( *(aware_t*)p != spect_type ) )
    {
        return bcore_spect_get_typed( spect_type, ( ( tp_t* )p )[ 1 ] );
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
        sz_t space = bcore_tbman_granted_space();
        spect_manager_close();
        bcore_msg( "  spect mananger ...... % 6zu\n", space - bcore_tbman_granted_space() );
    }
    return NULL;
}
