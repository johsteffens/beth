/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect.h"
#include "bcore_hmap.h"
#include "bcore_threads.h"
#include "bcore_spect_inst.h"

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

/**********************************************************************************************************************/

vc_t bcore_spect_get_typed_n( tp_t p_type, sz_t n, const tp_t* arr )
{
    tp_t sig = bcore_signature_fold_hash_arr( p_type, n, arr );
    vc_t spect_p = bcore_spect_try( sig );
    if( !spect_p )
    {
        fp_t create_from_self = bcore_flect_self_s_get_external_fp( bcore_flect_get_self( p_type ), bcore_name_enroll( "bcore_spect_fp_create_from_self" ), 0 );
        vd_t new_spect_p = NULL;
        if( n == 0 )
        {
            new_spect_p = ( ( bcore_spect_fp_create_from_self )create_from_self )( NULL );
        }
        else if( n == 1 )
        {
            new_spect_p = ( ( bcore_spect_fp_create_from_self )create_from_self )( bcore_flect_get_self( arr[ 0 ] ) );
        }
        else if( n > 1 )
        {
            bcore_flect_self_s* self_arr = bcore_un_alloc( sizeof( bcore_flect_self_s ), NULL, 0, n, NULL );
            for( sz_t i = 0; i < n; i++ )
            {
                bcore_flect_self_s_init( &self_arr[ i ] );
                bcore_flect_self_s_copy( &self_arr[ i ], bcore_flect_get_self( arr[ i ] ) );
            }
            new_spect_p = ( ( bcore_spect_fp_create_from_self )create_from_self )( self_arr );
            for( sz_t i = 0; i < n; i++ )
            {
                bcore_flect_self_s_down( &self_arr[ i ] );
            }
            bcore_un_alloc( sizeof( bcore_flect_self_s ), self_arr, n, 0, NULL );
        }
        else
        {
            new_spect_p = ( ( bcore_spect_fp_create_from_self )create_from_self )( bcore_flect_get_self( arr[ 0 ] ) );
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

