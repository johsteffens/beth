/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/**********************************************************************************************************************/
// trait system

#include "bcore_hmap.h"
#include "bcore_threads.h"
#include "bcore_memory_manager.h"
#include "bcore_trait.h"
#include "bcore_spect_array.h"
#include "bcore_spect_via.h"
#include "bcore_quicktypes.h"
#include "bcore_flect.h"

const char* trait_ft_s_def_g = "bcore_trait_ft_s = { tp_t type; tp_t name; }";
typedef struct bcore_trait_ft_s { tp_t type; tp_t name; } bcore_trait_ft_s;

const char* trait_s_def_g =
"bcore_trait_s = {"
    "bl_t awareness;"
    "bcore_trait_ft_s [] ft_arr;"
    "fp_t [] fp_arr;"
"}";

typedef struct bcore_trait_s
{
    bl_t awareness;

    union
    {
        bcore_static_array_s ft_arr;
        bcore_trait_ft_s * ft_data;
        sz_t ft_size, ft_space;
    };

    union
    {
        bcore_static_array_s fp_arr;
        bcore_trait_fp_supports * fp_data;
        sz_t fp_size, fp_space;
    };

} bcore_trait_s;

typedef struct system_s
{
    bcore_hmap_tptp_s type_map;
    bcore_hmap_tpto_s trait_map;
    bcore_mutex_t mutex;
} system_s;

static void system_s_init( system_s* o )
{
    bcore_hmap_tptp_s_init( &o->type_map );
    bcore_hmap_tpto_s_init( &o->trait_map );
    bcore_hmap_tpto_s_set_type( &o->trait_map, typeof( "bcore_trait_s" ) );
    bcore_mutex_init( &o->mutex );
}

static void system_s_down( system_s* o )
{
    bcore_mutex_lock( &o->mutex );
    bcore_hmap_tptp_s_down( &o->type_map );
    bcore_hmap_tpto_s_down( &o->trait_map );
    bcore_mutex_unlock( &o->mutex );
    bcore_mutex_down( &o->mutex );
}

/**********************************************************************************************************************/

static system_s system_s_g;
static void system_s_g_init()   { system_s_init( &system_s_g ); }
static void system_s_g_down()   { system_s_down( &system_s_g ); }
static void system_s_g_lock()   { bcore_mutex_lock( &system_s_g.mutex   ); }
static void system_s_g_unlock() { bcore_mutex_unlock( &system_s_g.mutex ); }

static void trait_manager_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, system_s_g_init );
}

static void trait_manager_close()
{
    system_s_g_down();
}

void bcore_trait_require_awareness( tp_t trait )
{
    system_s_g_lock();
    assert( trait != 0 );
    sr_s trait_sr = sr_twd( system_s_g.trait_map.type, bcore_hmap_tpto_s_fget( &system_s_g.trait_map, trait ) );
    bcore_via_nset_bl( trait_sr, typeof( "awareness" ), true );
    system_s_g_unlock();
}

void bcore_trait_require_function( tp_t trait, tp_t function, tp_t name )
{
    system_s_g_lock();
    assert( trait != 0 );
    sr_s trait_sr = sr_twd( system_s_g.trait_map.type, bcore_hmap_tpto_s_fget( &system_s_g.trait_map, trait ) );
    bcore_trait_ft_s node = { function, name };
    bcore_array_push( bcore_via_nget( trait_sr, typeof( "ft_arr" ) ), sr_twc( typeof( "bcore_trait_ft_s" ), &node ) );
    system_s_g_unlock();
}

void bcore_trait_register_fp_support( tp_t trait, bcore_trait_fp_supports f )
{
    system_s_g_lock();
    assert( trait != 0 );
    sr_s trait_sr = sr_twd( system_s_g.trait_map.type, bcore_hmap_tpto_s_fget( &system_s_g.trait_map, trait ) );
    bcore_array_push( bcore_via_nget( trait_sr, typeof( "fp_arr" ) ), sr_twc( TYPEOF_fp_t, &f ) );
    system_s_g_unlock();
}

static bl_t trait_is( tp_t trait, tp_t ancestor )
{
    for(;;)
    {
        if( trait == ancestor ) return true;
        tp_t* parent = bcore_hmap_tptp_s_get( &system_s_g.type_map, trait );
        if( !parent || !(*parent) ) return false;
        trait = *parent;
    }
}

void bcore_trait_set( tp_t trait, tp_t parent )
{
    system_s_g_lock();
    if( trait_is( parent, trait ) )
    {
        system_s_g_unlock();
        ERR( "trait '%s' is already ancestor to parent '%s'", ifnameof( trait ), ifnameof( parent ) );
    }
    bcore_hmap_tptp_s_set( &system_s_g.type_map, trait, parent );
    system_s_g_unlock();
}

bl_t bcore_trait_exists( tp_t trait )
{
    system_s_g_lock();
    bl_t flag = bcore_hmap_tptp_s_exists( &system_s_g.type_map, trait );
    system_s_g_unlock();
    return flag;
}

tp_t bcore_trait_parent( tp_t trait )
{
    system_s_g_lock();
    tp_t* parent = bcore_hmap_tptp_s_get( &system_s_g.type_map, trait );
    system_s_g_unlock();
    return parent ? *parent : 0;
}

bl_t bcore_trait_is( tp_t trait, tp_t ancestor )
{
    system_s_g_lock();
    bl_t flag = trait_is( trait, ancestor );
    system_s_g_unlock();
    return flag;
}

bl_t bcore_trait_supports( tp_t object, tp_t trait, bcore_string_s* log )
{
    if( object == trait ) return true;

    const bcore_flect_self_s* self = bcore_flect_try_self( object );

    if( !self )
    {
        if( log )
        {
            bcore_string_s_pushf( log, "object '%s' (%u) has no self reflection\n", ifnameof( object ), object );
        }
        return false;
    }

    if( self->trait && bcore_trait_is( self->trait, trait ) ) return true;

    system_s_g_lock();
    vd_t* trait_p = bcore_hmap_tpto_s_get( &system_s_g.trait_map, trait );
    system_s_g_unlock();

    if( !trait_p )
    {
        if( log )
        {
            bcore_string_s_pushf
            (
                log,
                "object '%s' (%u) is not of trait '%s' and the trait does not define requirements.\n",
                ifnameof( object ),
                object,
                ifnameof( trait )
            );
        }
        return false;
    }

    bcore_trait_s* trait_o = *trait_p;

    if( trait_o->awareness )
    {
        if( !bcore_flect_self_s_is_aware( self ) )
        {
            if( log )
            {
                bcore_string_s_pushf
                (
                    log,
                    "Object '%s' (%u) is not of of trait '%s'.\n",
                    "Trait requires self awareness.\n",
                    ifnameof( object ),
                    object,
                    ifnameof( trait )
                );
            }
            return false;
        }
    }

    for( sz_t i = 0; i < trait_o->ft_size; i++ )
    {
        bcore_trait_ft_s ft_o = trait_o->ft_data[ i ];
        if( !bcore_flect_self_s_try_external_fp( self, ft_o.type, ft_o.name ) )
        {
            if( log )
            {
                bcore_string_s_pushf
                (
                    log,
                    "Object '%s' (%u) is not of of trait '%s'.\n",
                    "Trait requires function '%s' of name '%s'.\n",
                    ifnameof( object ),
                    object,
                    ifnameof( trait ),
                    ifnameof( ft_o.type ),
                    ifnameof( ft_o.name )
                );
            }
            return false;
        }
    }

    for( sz_t i = 0; i < trait_o->fp_size; i++ )
    {
        bcore_trait_fp_supports fp_o = trait_o->fp_data[ i ];
        if( fp_o && !fp_o( object, trait, log ) ) return false;
    }

    return true;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_trait_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_trait" ) ) return NULL;
    if( signal == typeof( "init0" ) )
    {
        trait_manager_open();
    }
    else if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_parse_sc( trait_ft_s_def_g );
        bcore_flect_define_parse_sc( trait_s_def_g );
    }
    else if( signal == typeof( "down0" ) )
    {
        sz_t space = bcore_memory_manager_granted_space();
        trait_manager_close();
        bcore_msg( "  trait mananger ...... % 6zu\n", space - bcore_memory_manager_granted_space() );
    }
    return NULL;
}
