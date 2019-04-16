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

#include <stdatomic.h>

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
#include "bcore_tp_fastmap.h"

/**********************************************************************************************************************/
// bcore_spect_header_s

BCORE_DEFINE_FUNCTIONS_SELF_OBJECT_FLAT( bcore_spect_header_s, "bcore_spect_header_s = { tp_t p_type; tp_t o_type; }")

/**********************************************************************************************************************/
// hash map

// power of two > 4
#define BCORE_SPECT_CACHE_SIZE 4096
// four subsequent cache spaces
#define BCORE_SPECT_CACHE_MASK ( ( BCORE_SPECT_CACHE_SIZE - 1 ) ^ 3 )

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

    /// cache for spect manager
    bcore_tp_fastmap_s cache;

    /// array of bcore_tp_fastmap_s caches (managed by spect manager)
    bcore_arr_vd_s cache_arr;

} hmap_s;

static void hmap_s_init( hmap_s* o )
{
    bcore_arr_vd_s_init( &o->arr );
    bcore_arr_vd_s_init( &o->cache_arr );
    bcore_hmap_u2vd_s_init( &o->map );
    bcore_mutex_s_init( &o->mutex );
    bcore_tp_fastmap_s_init( &o->cache );
    bcore_arr_vd_s_push( &o->cache_arr, &o->cache ); // o->cache will be managed via hmap_s_g->cache_arr
}

static sz_t hmap_s_size( hmap_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    sz_t size = bcore_hmap_u2vd_s_keys( &o->map );
    bcore_mutex_s_unlock( &o->mutex );
    return size;
}

vc_t hmap_s_cache_get( hmap_s* o, tp_t key )
{
    return bcore_tp_fastmap_s_get( &o->cache, key );
}

void hmap_s_cache_set( hmap_s* o, tp_t key, vc_t val )
{
    bcore_tp_fastmap_s_set( &o->cache, key, val );
}

static void hmap_s_down( hmap_s* o )
{
    bcore_mutex_s_lock( &o->mutex );

    /// clear and lock all caches because shut down procedure can change perspective addresses (this includes o->cache)
    for( uz_t i = 0; i < o->cache_arr.size; i++ )
    {
        bcore_tp_fastmap_s* cache = o->cache_arr.data[ i ];
        bcore_tp_fastmap_s_clear( cache );
        bcore_tp_fastmap_s_set_locked( cache, true );
    }

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
         *    4. Caches are cleared and locked.
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
            bcore_inst_a_discard( spect );
        }
        bcore_mutex_s_lock( &o->mutex );
    }

    /// shut down all caches (this includes o->cache)
    for( uz_t i = 0; i < o->cache_arr.size; i++ )
    {
        bcore_tp_fastmap_s* cache = o->cache_arr.data[ i ];
        bcore_tp_fastmap_s_down( cache );
    }
    bcore_arr_vd_s_down( &o->cache_arr );

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
    if( !self->body                          ) return false;
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
//    now optional; missing 'bcore_spect_fp_create_from_self' falls back to generic function bcore_spect_create_from_self

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

void bcore_spect_setup_cache( bcore_tp_fastmap_s* cache )
{
    bcore_tp_fastmap_s_init( cache );
    bcore_mutex_s_lock( &hmap_s_g->mutex );
    bcore_arr_vd_s_push( &hmap_s_g->cache_arr, cache );
    bcore_mutex_s_unlock( &hmap_s_g->mutex );
}

//----------------------------------------------------------------------------------------------------------------------

vc_t bcore_spect_get_typed( tp_t p_type, tp_t o_type )
{
    assert( hmap_s_g != NULL );

    tp_t sig = bcore_tp_fold_tp( p_type, o_type );
    vc_t val = hmap_s_cache_get( hmap_s_g, sig );
    if( val ) return val;

    bcore_mutex_s_lock( &hmap_s_g->mutex );
    vd_t* vdp = bcore_hmap_u2vd_s_get( &hmap_s_g->map, sig );
    if( vdp )
    {
        hmap_s_cache_set( hmap_s_g, sig, *vdp );
        bcore_mutex_s_unlock( &hmap_s_g->mutex );
        return *vdp;
    }

    /** Recursion counter used to detect recursions in bcore_spect_get_typed.
     *  This allows detecting certain types of cyclic reflections, which the
     *  framework cannot handle.
     */
    static sz_t recursion_count = 0;
    const sz_t recursion_count_limit = 256;
    recursion_count++;

    // Unlock because create_from_self may make use of registry
    bcore_mutex_s_unlock( &hmap_s_g->mutex );

    if( recursion_count > recursion_count_limit )
    {
        ERR_fa
        (
            "Recursion limit '#<sz_t>' reached at object '#<sc_t>'\n"
            "A possible cause of this error is that a reflection is cyclic.\n"
            "Certain types of cycles cannot be resolved by the framework.\n"
            "Note that the problem can be with the reflection of '#<sc_t>' or an object containing it.\n",
            recursion_count_limit,
            ifnameof( o_type ),
            ifnameof( o_type )
        );
    }

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

    recursion_count--;

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

    if( discard_spect ) bcore_inst_a_discard( discard_spect );
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
    bcore_spect_header_s* spect = bcore_inst_t_create( p_self->type );
    const bcore_via_s* p_via = bcore_via_s_get_typed( p_self->type );
    uz_t size = bcore_via_p_get_size( p_via, NULL );
    for( uz_t i = 0; i < size; i++ )
    {
        const bcore_vitem_s* vitem = bcore_via_p_iget_vitem( p_via, NULL, i );
        if( vitem->flags.f_feature && vitem->flags.f_strict )
        {
            if( vitem->flags.f_fp )
            {
                sr_s dst = bcore_via_p_nget( p_via, (bcore_via*)spect, vitem->name );
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

    bcore_inst_a_discard( (bcore_inst*)spect );

    bcore_trait_set( trait, p_self->parent );
}

vd_t bcore_spect_create_from_self( const bcore_self_s* p_self, const bcore_self_s* o_self )
{
    assert( p_self != NULL );
    assert( o_self != NULL );

    const bcore_inst_s* p_inst = bcore_inst_s_get_typed( p_self->type );

    bcore_spect_header_s* o = bcore_inst_t_create( p_self->type );
    o->o_type = o_self->type;

    uz_t p_items_size = bcore_inst_s_get_items_size( p_inst );
    uz_t o_items_size = bcore_self_s_items_size( o_self );

    for( uz_t i = 0; i < p_items_size; i++ )
    {
        const bcore_inst_item_s* p_inst_item = bcore_inst_s_get_item( p_inst, i );
        const bcore_self_item_s* p_self_item = p_inst_item->self_item;
        if( !p_self_item ) continue;
        if( !p_self_item->flags.f_feature ) continue;

        vd_t dst = bcore_inst_item_s_get_obj( p_inst_item, o );
        bl_t found = false;

        if( p_self_item->child_item ) // explicit bindings
        {
            const bcore_self_item_s* p_child_item = p_self_item->child_item;

            for( uz_t i = 0; i < o_items_size; i++ )
            {
                const bcore_self_item_s* o_self_item = bcore_self_s_get_item( o_self, i );
                if( o_self_item->flags.f_private ) continue;
                if( o_self_item->flags.f_shell   ) continue;

                if
                (
                    p_child_item->type == o_self_item->type &&
                    p_child_item->name == o_self_item->name &&
                    p_child_item->caps == o_self_item->caps
                )
                {
                    found = true;
                    if // external functions
                    (
                        p_self_item->flags.f_fp &&
                        o_self_item->caps == BCORE_CAPS_EXTERNAL_FUNC
                    )
                    // then
                    {
                        fp_t src_fp = bcore_function_get( o_self_item->default_tp );
                        ( *(fp_t*)dst ) = src_fp;
                    }
                    else if // constant
                    (
                        o_self_item->flags.f_const &&
                        o_self_item->caps == BCORE_CAPS_SOLID_STATIC &&
                        p_self_item->caps == BCORE_CAPS_SOLID_STATIC &&
                        p_self_item->type == o_self_item->type
                    )
                    // then
                    {
                        switch( p_self_item->type )
                        {
                            case TYPEOF_s0_t: *( s0_t* )dst = o_self_item->default_s3; break;
                            case TYPEOF_s1_t: *( s1_t* )dst = o_self_item->default_s3; break;
                            case TYPEOF_s2_t: *( s2_t* )dst = o_self_item->default_s3; break;
                            case TYPEOF_s3_t: *( s3_t* )dst = o_self_item->default_s3; break;
                            case TYPEOF_u0_t: *( u0_t* )dst = o_self_item->default_u3; break;
                            case TYPEOF_u1_t: *( u1_t* )dst = o_self_item->default_u3; break;
                            case TYPEOF_u2_t: *( u2_t* )dst = o_self_item->default_u3; break;
                            case TYPEOF_u3_t: *( u3_t* )dst = o_self_item->default_u3; break;
                            case TYPEOF_f2_t: *( f2_t* )dst = o_self_item->default_f3; break;
                            case TYPEOF_f3_t: *( f3_t* )dst = o_self_item->default_f3; break;
                            case TYPEOF_sz_t: *( sz_t* )dst = o_self_item->default_smax; break;
                            case TYPEOF_uz_t: *( uz_t* )dst = o_self_item->default_umax; break;
                            case TYPEOF_bl_t: *( bl_t* )dst = o_self_item->default_u3; break;
                            case TYPEOF_tp_t: *( tp_t* )dst = o_self_item->default_tp; break;
                            default: found = false;
                        }
                    }
                    else if // offset
                    (
                        !o_self_item->flags.f_const &&
                        p_self_item->caps == BCORE_CAPS_SOLID_STATIC &&
                        p_self_item->type == TYPEOF_offset_t
                    )
                    // then
                    {
                        const bcore_inst_s* o_inst = bcore_inst_s_get_typed( o_self->type );
                        const bcore_inst_item_s* o_inst_item = bcore_inst_s_get_item_from_self_item( o_inst, o_self_item );
                        *( offset_t* )dst = o_inst_item->offset;
                    }
                    else if // spect ~> const type
                    (
                        bcore_trait_is_of( p_self_item->type, TYPEOF_spect ) &&
                        p_self_item ->caps == BCORE_CAPS_LINK_STATIC &&
                        p_child_item->type == TYPEOF_tp_t &&
                        p_child_item->flags.f_const &&
                        o_self_item ->flags.f_const
                    )
                    // then
                    {
                        found = true;
                        *( vc_t* )dst = bcore_spect_get_typed( p_self_item->type, o_self_item->default_tp );
                    }
                    else
                    {
                        found = false;
                    }

                    if( found ) break;
                }
            }
        }
        else // implicit (canonic) bindings
        {
            if( bcore_trait_is_of( p_self_item->type, TYPEOF_spect ) && p_self_item->caps == BCORE_CAPS_LINK_STATIC )
            {
                found = true;
                *( vc_t* )dst = bcore_spect_get_typed( p_self_item->type, o_self->type );
            }
            else if( bcore_trait_is_of( p_self_item->type, TYPEOF_function_pointer ) && p_self_item->caps == BCORE_CAPS_SOLID_STATIC )
            {
                for( uz_t i = 0; i < o_items_size; i++ )
                {
                    const bcore_self_item_s* o_self_item = bcore_self_s_get_item( o_self, i );
                    if( o_self_item->flags.f_private ) continue;
                    if( o_self_item->flags.f_shell   ) continue;

                    if( o_self_item->type == p_self_item->type && o_self_item->name == p_self_item->name && o_self_item->caps == BCORE_CAPS_EXTERNAL_FUNC )
                    {
                        found = true;
                        fp_t src_fp = bcore_function_get( o_self_item->default_tp );
                        ( *(fp_t*)dst ) = src_fp;
                        break;
                    }
                }
            }
        }

        if( !found && p_self_item->flags.f_strict )
        {
            if( p_self_item->child_item )
            {
                ERR_fa
                (
                    "Creating perspective '#<sc_t>': Feature '#<sc_t> #<sc_t>' not found in '#<sc_t>'.",
                    ifnameof( p_self->type ),
                    ifnameof( p_self_item->child_item->type ),
                    ifnameof( p_self_item->child_item->name ),
                    ifnameof( o_self->type )
                );
            }
            else
            {
                ERR_fa
                (
                    "Creating perspective '#<sc_t>': Feature '#<sc_t> #<sc_t>' not found in '#<sc_t>'.",
                    ifnameof( p_self->type ),
                    ifnameof( p_self_item->type ),
                    ifnameof( p_self_item->name ),
                    ifnameof( o_self->type )
                );
            }
        }
    }

    return o;
}

vd_t bcore_spect_create_from_self_typed( tp_t p_type, tp_t o_type )
{
    const bcore_self_s* p_self = bcore_flect_get_self( p_type );
    const bcore_self_s* o_self = bcore_flect_get_self( o_type );
    return bcore_spect_create_from_self( p_self, o_self );
}

void bcore_spect_define_creator( tp_t type, bcore_flect_create_self_fp creator )
{
    bcore_flect_define_creator( type, creator );
    bcore_self_s* self = creator();
    bcore_spect_define_trait( self );
    bcore_self_s_discard( self );
}

/**********************************************************************************************************************/

sz_t bcore_spect_size()
{
    assert( hmap_s_g != NULL );
    return hmap_s_size( hmap_s_g );
}

st_s* bcore_spect_status()
{
    bcore_life_s* l = bcore_life_s_create();
    assert( hmap_s_g != NULL );
    const bcore_hmap_u2vd_s* map  = &hmap_s_g->map;
    bcore_hmap_tpuz_s* hist = bcore_life_s_push_aware( l, bcore_hmap_tpuz_s_create() );
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

    for( uz_t i = 0; i < map->size; i++ )
    {
        bcore_mutex_s_lock( &hmap_s_g->mutex );
        vd_t val = bcore_hmap_u2vd_s_idx_val( map, i );
        bcore_mutex_s_unlock( &hmap_s_g->mutex );
        if( val ) ( *bcore_hmap_tpuz_s_fget( hist, *( aware_t* )val, 0 ) )++;
    }

    sr_s nc_arr = sr_cl( bcore_inst_t_create_sr( bcore_flect_type_parse_fa( "{ { uz_t count; tp_t type; } [] arr; }" ) ), l );

    for( uz_t i = 0; i < hist->size; i++ )
    {
        tp_t key   = bcore_hmap_tpuz_s_idx_key( hist, i );
        uz_t count = bcore_hmap_tpuz_s_idx_val( hist, i );
        if( key )
        {
            bcore_array_x_push( nc_arr, sr_null() );
            sr_s pair = sr_cl( bcore_array_x_get_last( nc_arr ), l );
            bcore_via_x_nset_tp( pair, typeof( "type" ),  key   );
            bcore_via_x_nset_uz( pair, typeof( "count" ), count );
        }
    }

    bcore_array_x_sort( nc_arr, 0, -1, -1 );

    for( uz_t i = 0; i < bcore_array_x_get_size( nc_arr ); i++ )
    {
        sr_s pair = sr_cl( bcore_array_x_get( nc_arr, i ), l );
        const tp_t* p_type  = sr_cl( bcore_via_x_nget( pair, typeof( "type"  ) ), l ).o;
        const uz_t* p_count = sr_cl( bcore_via_x_nget( pair, typeof( "count" ) ), l ).o;
        st_s* s = st_s_create();
        st_s_pushf( s, "  %s ", ifnameof( *p_type ) );
        st_s_push_char_n( s, '.', 28 - s->size );
        st_s_pushf( s, "% 4zu\n", *p_count );
        st_s_push_st_d( log, s );
    }

    st_s_push_fa( log, "spect mananger cache:\n" );
    st_s_push_fa( log, "   keys: #<uz_t>\n", bcore_tp_fastmap_s_keys( &hmap_s_g->cache ) );
    st_s_push_fa( log, "   size: #<uz_t>\n", bcore_tp_fastmap_s_size( &hmap_s_g->cache ) );

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

void bcore_spect_missing_err( vc_t p, sc_t name )
{
    ERR_fa( "Unmapped function #<sc_t>(#<sc_t>).#<sc_t>.", ifnameof( spect_tp_p( p ) ), ifnameof( spect_tp_o( p ) ), name );
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
            BCORE_REGISTER_OBJECT( bcore_spect_header_s );

            // features
            BCORE_REGISTER_FEATURE( bcore_spect_fp_create_from_self );
        }
        break;

        case TYPEOF_down0:
        {

            if( o->object && ( *( bl_t* )o->object ) )
            {
                uz_t count = bcore_spect_size();
                uz_t space = bcore_tbman_total_granted_space();
                spect_manager_close();
                space -= bcore_tbman_total_granted_space();
                bcore_msg( "  spect mananger ...... % 6zu (by % 4zu perspectives )\n", space, count );
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
