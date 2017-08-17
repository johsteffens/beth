/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect.h"
#include "bcore_hmap.h"
#include "bcore_threads.h"
#include "bcore_spect_inst.h"
#include "bcore_life.h"
#include "bcore_spect_array.h"
#include "bcore_spect_via.h"
#include "bcore_quicktypes.h"

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

void bcore_spect_manager_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, create_hmap_s );
}

void bcore_spect_manager_close()
{
    discard_hmap_s();
}

vc_t bcore_spect_try( tp_t sig )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    vd_t* vdp = bcore_hmap_u2vd_s_get( hmap_s_g->map, sig );
    bcore_mutex_unlock( &hmap_s_g->mutex );
    return vdp ? *vdp : NULL;
}

vc_t bcore_spect_get( tp_t sig )
{
    vc_t ret = bcore_spect_try( sig );
    if( !ret ) ERR( "perspective of signature %"PRIu32" has not been enrolled", sig );
    return ret;
}

void bcore_spect_enroll_d( vd_t spect )
{
    tp_t type = *( aware_t *)spect;
    bcore_spect_fp_create_signature create_signature = ( bcore_spect_fp_create_signature )bcore_flect_self_s_get_external_fp( bcore_flect_get_self( type ), bcore_name_enroll( "bcore_spect_fp_create_signature" ), 0 );
    tp_t sig = bcore_signature_manager_enroll_d( create_signature( spect ) );

    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    if( bcore_hmap_u2vd_s_get( hmap_s_g->map, type ) )
    {
        ERR( "Perspective with signature '%s' has already been enrolled.", bcore_signature_s_create_string( bcore_signature_manager_get( sig ) )->sc );
    }
    else
    {
        bcore_hmap_u2vd_s_set( hmap_s_g->map, sig, spect, false );
    }
    bcore_mutex_unlock( &hmap_s_g->mutex );
}

void bcore_spect_remove( tp_t sig )
{
    assert( hmap_s_g != NULL );
    bcore_mutex_lock( &hmap_s_g->mutex );
    bcore_inst_aware_discard( bcore_hmap_u2vd_s_remove_h( hmap_s_g->map, sig ) );
    bcore_mutex_unlock( &hmap_s_g->mutex );
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

    tp_t t_nc_arr  = bcore_flect_type_parsef( "{ aware_t _; { aware_t _; sz_t count; tp_t type; } [] arr; }" );
    vd_t nc_arr    = bcore_life_s_push_aware( l, bcore_inst_typed_create( t_nc_arr ) );
    const bcore_array_s* nc_arr_a =  bcore_array_s_get_aware( nc_arr );

    for( sz_t i = 0; i < hist->size; i++ )
    {
        tp_t key   = bcore_hmap_tpsz_s_idx_key( hist, i );
        sz_t count = bcore_hmap_tpsz_s_idx_val( hist, i );
        if( key )
        {
            vd_t pair = bcore_array_spect_push( nc_arr_a, nc_arr, rf_null() ).o;
            bcore_via_aware_nset_tp( pair, typeof( "type" ),  key   );
            bcore_via_aware_nset_sz( pair, typeof( "count" ), count );
        }
    }

    bcore_array_spect_sort( nc_arr_a, nc_arr, 0, -1, -1 );

    for( sz_t i = 0; i < bcore_array_spect_get_size( nc_arr_a, nc_arr ); i++ )
    {
        vc_t pair = bcore_array_spect_get( nc_arr_a, nc_arr, i ).o;
        const tp_t* p_type  = bcore_via_aware_nget_c( pair, typeof( "type"  ) );
        const sz_t* p_count = bcore_via_aware_nget_c( pair, typeof( "count" ) );
        bcore_string_s* s = bcore_string_s_create();
        bcore_string_s_pushf( s, "%s ", ifnameof( *p_type ) );
        bcore_string_s_push_char_n( s, '.', 28 - s->size );
        bcore_string_s_pushf( s, "% 4zu\n", *p_count );
        bcore_string_s_push_string_d( log, s );
    }

    bcore_life_s_discard( l );
    return log;
}

/**********************************************************************************************************************/

vc_t bcore_spect_get_typed_n( tp_t p_type, sz_t n, const tp_t* arr )
{
    tp_t sig = bcore_signature_fold_hash_arr( p_type, n, arr );
    vc_t spect_p = bcore_spect_try( sig );
    if( !spect_p )
    {
        fp_t create_from_self = bcore_flect_self_s_get_external_fp( bcore_flect_get_self( p_type ), bcore_name_enroll( "bcore_spect_fp_create_from_self" ), 0 );
        vd_t new_spect_p = NULL;
        if( n == 1 )
        {
            const bcore_flect_self_s* self = bcore_flect_try_self( arr[ 0 ] );
            new_spect_p = ( ( bcore_spect_fp_create_from_self )create_from_self )( &self );
        }
        else if( n > 1 )
        {
            const bcore_flect_self_s** self_arr = bcore_un_alloc( sizeof( bcore_flect_self_s* ), NULL, 0, n, NULL );
            for( sz_t i = 0; i < n; i++ ) self_arr[ i ] = bcore_flect_try_self( arr[ i ] );
            new_spect_p = ( ( bcore_spect_fp_create_from_self )create_from_self )( self_arr );
            bcore_un_alloc( sizeof( bcore_flect_self_s* ), self_arr, n, 0, NULL );
        }
        else
        {
            new_spect_p = ( ( bcore_spect_fp_create_from_self )create_from_self )( NULL );
        }
        bcore_spect_enroll_d( new_spect_p );
        spect_p = new_spect_p;
    }
    return spect_p;
}

vc_t bcore_spect_get_typed( tp_t p_type, tp_t o_type )
{
    return bcore_spect_get_typed_n( p_type, 1, &o_type );
}

/**********************************************************************************************************************/

