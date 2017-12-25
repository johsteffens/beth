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

/**********************************************************************************************************************/
// trait system

#include "bcore_hmap.h"
#include "bcore_threads.h"
#include "bcore_tbman.h"
#include "bcore_trait.h"
#include "bcore_quicktypes.h"
#include "bcore_flect.h"
#include "bcore_spect_inst.h"

const char* trait_ft_s_def_g = "bcore_trait_ft_s = bcore_inst { tp_t type; tp_t name; }";
typedef struct bcore_trait_ft_s { tp_t type; tp_t name; } bcore_trait_ft_s;

const char* trait_s_def_g =
"bcore_trait_s = bcore_inst {"
    "bl_t in_ancestry;"
    "bl_t awareness;"
    "bcore_trait_ft_s [] ft_arr;"
    "fp_t [] fp_arr;"
"}";

typedef struct bcore_trait_s
{
    bl_t in_ancestry; // trait must be declared in the ancestry of object's self.trait
    bl_t awareness;

    union
    {
        bcore_static_array_s ft_arr;
        struct
        {
            bcore_trait_ft_s * ft_data;
            sz_t ft_size, ft_space;
        };
    };

    union
    {
        bcore_static_array_s fp_arr;
        struct
        {
            bcore_trait_fp_supports * fp_data;
            sz_t fp_size, fp_space;
        };
    };

} bcore_trait_s;

void bcore_trait_s_init( bcore_trait_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void bcore_trait_s_down( bcore_trait_s* o )
{
    if( o->ft_space ) o->ft_data = bcore_un_alloc( sizeof( bcore_trait_ft_s ), o->ft_data, o->ft_space, 0, &o->ft_space );
    if( o->fp_space ) o->fp_data = bcore_un_alloc( sizeof( fp_t             ), o->fp_data, o->fp_space, 0, &o->fp_space );
}

DEFINE_FUNCTION_CREATE(  bcore_trait_s )
DEFINE_FUNCTION_DISCARD( bcore_trait_s )

static void trait_s_push_function( bcore_trait_s* o, tp_t function, tp_t name )
{
    if( o->ft_size == o->ft_space )
    {
        o->ft_data = bcore_un_alloc( sizeof( bcore_trait_ft_s ), o->ft_data, o->ft_space, o->ft_space > 0 ? o->ft_space * 2 : 1, &o->ft_space );
    }
    o->ft_data[ o->ft_size++ ] = ( bcore_trait_ft_s ) { function, name };
}

static void trait_s_push_fp_support( bcore_trait_s* o, bcore_trait_fp_supports f )
{
    if( o->fp_size == o->fp_space )
    {
        o->fp_data = bcore_un_alloc( sizeof( fp_t ), o->fp_data, o->fp_space, o->fp_space > 0 ? o->fp_space * 2 : 1, &o->fp_space );
    }
    o->fp_data[ o->fp_size++ ] = f;
}

static bcore_flect_self_s* trait_ft_s_create_self( void )
{
    return bcore_flect_self_s_build_parse_sc( trait_ft_s_def_g, sizeof( bcore_trait_ft_s ) );
}

static bcore_flect_self_s* trait_s_create_self( void )
{
    return bcore_flect_self_s_build_parse_sc( trait_s_def_g, sizeof( bcore_trait_s ) );
}

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

static system_s* system_s_create()
{
    system_s* o = bcore_alloc( NULL, sizeof( system_s ) );
    system_s_init( o );
    return o;
}

static void system_s_down( system_s* o )
{
    bcore_mutex_lock( &o->mutex );

    /// we explicitly discard here to avoid implicitly creating an instance perspective
    for( sz_t i = 0; i < o->trait_map.size; i++ )
    {
        tp_t key  = bcore_hmap_tpto_s_idx_key( &o->trait_map, i );
        vd_t* dst = bcore_hmap_tpto_s_get( &o->trait_map, key );
        if( dst )
        {
            bcore_trait_s_discard( *dst );
            *dst = NULL;
        }
    }

    bcore_hmap_tpto_s_down( &o->trait_map );
    bcore_hmap_tptp_s_down( &o->type_map );
    bcore_mutex_unlock( &o->mutex );
    bcore_mutex_down( &o->mutex );
}

static void system_s_discard( system_s* o )
{
    if( !o ) return;
    system_s_down( o );
    bcore_release( o );
}

/**********************************************************************************************************************/

static system_s* system_s_g = NULL;
static void system_s_g_init()   { system_s_g = system_s_create(); }
static void system_s_g_down()   { system_s_discard( system_s_g ); system_s_g = NULL; }
static void system_s_g_lock()   { assert( system_s_g != NULL ); bcore_mutex_lock( &system_s_g->mutex   ); }
static void system_s_g_unlock() { bcore_mutex_unlock( &system_s_g->mutex ); }

static void trait_manager_open()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, system_s_g_init );
}

static void trait_manager_close()
{
    system_s_g_down();
}

static bcore_trait_s* get_trait( tp_t trait )
{
    vd_t* trait_pp = bcore_hmap_tpto_s_get( &system_s_g->trait_map, trait );
    if( trait_pp ) return *trait_pp;
    return *bcore_hmap_tpto_s_set_d( &system_s_g->trait_map, trait, bcore_trait_s_create() );
}

void bcore_trait_require_in_ancestry( tp_t trait )
{
    assert( trait != 0 );
    system_s_g_lock();
    assert( !bcore_hmap_tptp_s_exists( &system_s_g->type_map, trait ) );
    get_trait( trait )->in_ancestry = true;
    system_s_g_unlock();
}

void bcore_trait_require_awareness( tp_t trait )
{
    assert( trait != 0 );
    system_s_g_lock();
    assert( !bcore_hmap_tptp_s_exists( &system_s_g->type_map, trait ) );
    get_trait( trait )->awareness = true;
    system_s_g_unlock();
}

void bcore_trait_require_function( tp_t trait, tp_t function, tp_t name )
{
    assert( trait != 0 );
    system_s_g_lock();
    assert( !bcore_hmap_tptp_s_exists( &system_s_g->type_map, trait ) );
    trait_s_push_function( get_trait( trait ), function, name );
    system_s_g_unlock();
}

void bcore_trait_register_fp_support( tp_t trait, bcore_trait_fp_supports f )
{
    assert( trait != 0 );
    system_s_g_lock();
    assert( !bcore_hmap_tptp_s_exists( &system_s_g->type_map, trait ) );
    trait_s_push_fp_support( get_trait( trait ), f );
    system_s_g_unlock();
}

static bl_t trait_is( tp_t trait, tp_t ancestor )
{
    for(;;)
    {
        if( trait == ancestor ) return true;
        tp_t* parent = bcore_hmap_tptp_s_get( &system_s_g->type_map, trait );
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
        ERR( "Declaring trait '%s' with parent '%s' causes cyclic ancestry.", ifnameof( trait ), ifnameof( parent ) );
    }
    bcore_hmap_tptp_s_set( &system_s_g->type_map, trait, parent );
    system_s_g_unlock();
}

bl_t bcore_trait_exists( tp_t trait )
{
    system_s_g_lock();
    bl_t flag = bcore_hmap_tptp_s_exists( &system_s_g->type_map, trait );
    system_s_g_unlock();
    return flag;
}

tp_t bcore_trait_parent( tp_t trait )
{
    system_s_g_lock();
    tp_t* parent = bcore_hmap_tptp_s_get( &system_s_g->type_map, trait );
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

bl_t bcore_trait_supported( tp_t trait, const bcore_flect_self_s* self, st_s* log )
{
    if( !trait ) return true;
    if( !bcore_trait_exists( trait ) )
    {
        if( log )
        {
            st_s_pushf( log, "Trait '%s' is not defined.", ifnameof( trait ) );
        }
        return false;
    }
    if( !bcore_trait_supported( bcore_trait_parent( trait ), self, log ) ) return false;

    system_s_g_lock();
    vd_t* trait_p = bcore_hmap_tpto_s_get( &system_s_g->trait_map, trait );
    system_s_g_unlock();

    if( !trait_p ) return true; // no restrictions

    bcore_trait_s* trait_o = *trait_p;

    if( trait_o->in_ancestry )
    {
        if( !bcore_trait_is( self->trait, trait ) )
        {
            if( log )
            {
                st_s_pushf( log, "Trait is not in ancestry." );
            }
            return false;
        }
    }

    if( trait_o->awareness )
    {
        if( !bcore_flect_self_s_is_aware( self ) )
        {
            if( log )
            {
                st_s_pushf( log, "Self awareness missing." );
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
                if( ft_o.type && ft_o.name )
                {
                    st_s_pushf( log, "Missing function '%s' of name '%s'.", ifnameof( ft_o.type ), ifnameof( ft_o.name ) );
                }
                else if( ft_o.type )
                {
                    st_s_pushf( log, "Missing function '%s'.", ifnameof( ft_o.type ) );
                }
                else if( ft_o.name )
                {
                    st_s_pushf( log, "Missing function '%s'.", ifnameof( ft_o.name ) );
                }
            }
            return false;
        }
    }

    for( sz_t i = 0; i < trait_o->fp_size; i++ )
    {
        bcore_trait_fp_supports fp_o = trait_o->fp_data[ i ];
        if( fp_o && !fp_o( self, log ) ) return false;
    }

    return true;
}

bl_t bcore_trait_satisfied_self( tp_t trait, const bcore_flect_self_s* self, st_s* log )
{
    if( self->type == trait                       ) return true;
    if( bcore_trait_is( self->trait, trait )      ) return true;
    if( bcore_trait_supported( trait, self, log ) ) return true;
    return false;
}

bl_t bcore_trait_satisfied_type( tp_t trait, tp_t object_type, st_s* log )
{
    if( object_type == trait                      ) return true;
    const bcore_flect_self_s* self = bcore_flect_try_self( object_type );
    if( !self )
    {
        if( log )
        {
            st_s_pushf( log, "Object '%s' has no self reflection.", ifnameof( object_type ) );
        }
        return false;
    }
    if( bcore_trait_satisfied_self( trait, self, log ) ) return true;
    return false;
}

sz_t  bcore_trait_size()
{
    system_s_g_lock();
    sz_t size = bcore_hmap_tptp_s_keys( &system_s_g->type_map );
    system_s_g_unlock();
    return size;
}

st_s* bcore_trait_show()
{
    system_s_g_lock();
    st_s* log = st_s_create();
    sz_t size = bcore_hmap_tptp_s_size( &system_s_g->type_map );
    for( sz_t i = 0; i < size; i++ )
    {
        tp_t key = bcore_hmap_tptp_s_idx_key( &system_s_g->type_map, i );
        if( key )
        {
            tp_t val = bcore_hmap_tptp_s_idx_val( &system_s_g->type_map, i );
            system_s_g_unlock();
            if( bcore_name_exists( key ) )
            {
                st_s_push_fa( log, "#<sc_t>", ifnameof( key ) );
            }
            else
            {
                st_s_push_fa( log, "#<tp_t>", key );
            }

            system_s_g_lock();
            while( val )
            {
                system_s_g_unlock();
                st_s_push_fa( log, "->#<sc_t>", ifnameof( val ) );
                system_s_g_lock();
                val = *bcore_hmap_tptp_s_get( &system_s_g->type_map, val );
            }
            system_s_g_unlock();
            st_s_push_char( log, '\n' );
            system_s_g_lock();
        }
    }
    system_s_g_unlock();
    st_s_push_char( log, '\n' );
    return log;
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
        bcore_flect_define_creator( typeof( "bcore_trait_ft_s" ), trait_ft_s_create_self  );
        bcore_flect_define_creator( typeof( "bcore_trait_s"    ), trait_s_create_self  );
    }
    else if( signal == typeof( "down0" ) )
    {
        if( object && ( *( bl_t* )object ) )
        {
            sz_t space1 = bcore_tbman_granted_space();
            trait_manager_close();
            sz_t space2 = bcore_tbman_granted_space();
            bcore_msg( "  trait manager ....... % 6zu\n", space1 > space2 ? space1 - space2 : ( sz_t )0 );
        }
        else
        {
            trait_manager_close();
        }
    }
    return NULL;
}
