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

#include "bcore_life.h"
#include "bcore_control.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

bcore_life_s* __bcore_life = NULL;

BCORE_DEFINE_FUNCTION_INIT_FLAT( bcore_life_item_s )

// ---------------------------------------------------------------------------------------------------------------------

void bcore_life_item_s_down( bcore_life_item_s* o )
{
    if( o->object )
    {
        if( o->type )
        {
            o->typed_down_or_discard( o->type, o->object );
        }
        else if( o->down_or_discard )
        {
            o->down_or_discard( o->object );
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_COPY_FLAT( bcore_life_item_s )
BCORE_DEFINE_FUNCTION_CREATE(    bcore_life_item_s )
BCORE_DEFINE_FUNCTION_DISCARD(   bcore_life_item_s )
BCORE_DEFINE_FUNCTION_CLONE(     bcore_life_item_s )

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

void bcore_life_s_init( struct bcore_life_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
    o->_ = TYPEOF_bcore_life_s;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_life_s_down( struct bcore_life_s* o )
{
    assert( o );
    bcore_release_obj_arr( ( fp_t )bcore_life_item_s_down, o->data, o->size, sizeof( bcore_life_item_s ) );
}

// ---------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_FUNCTION_CREATE(  bcore_life_s )
BCORE_DEFINE_FUNCTION_DISCARD( bcore_life_s )

// ---------------------------------------------------------------------------------------------------------------------

void bcore_life_s_setup( bcore_life_s* o, bcore_life_s* parent )
{
    o->nesting_level = ( parent ) ? parent->nesting_level + 1 : 0;
    o->parent = parent;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_life_s_init_setup( bcore_life_s* o, bcore_life_s* parent )
{
    bcore_life_s_init( o );
    bcore_life_s_setup( o, parent );
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_life_item_s* bcore_life_s_push_level_item( bcore_life_s* o, sz_t nesting_level )
{
    if( o->nesting_level == nesting_level )
    {
        assert( o );
        if( !o->data )
        {
            o->space = 4;
            o->data = bcore_alloc( NULL, o->space * sizeof( bcore_life_item_s ) );
        }
        else if( o->size == o->space )
        {
            o->space = o->space * 2;
            o->data = bcore_alloc( o->data, o->space * sizeof( bcore_life_item_s ) );
        }
        bcore_life_item_s* r = &o->data[ o->size ];
        o->size++;
        bcore_life_item_s_init( r );
        return r;
    }
    else if( o->parent )
    {
        return bcore_life_s_push_level_item( o->parent, nesting_level );
    }
    else
    {
        ERR_fa( "Found no manager at nesting-level #<sz_t>", nesting_level );
    }
    return NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

bcore_life_item_s* bcore_life_s_push_item( bcore_life_s* o )
{
    return bcore_life_s_push_level_item( o, o->nesting_level );
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_life_s_push( bcore_life_s* o, bcore_fp_discard discard, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->down_or_discard = discard;
    item->object  = object;
    return object;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_life_s_spush( bcore_life_s* o, bcore_fp_down down, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->down_or_discard = down;
    item->object  = object;
    return object;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_life_s_push_typed( bcore_life_s* o, tp_t type, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->type   = type;
    item->object = object;
    item->typed_down_or_discard = ( bcore_fp_typed_down_or_discard )bcore_inst_t_discard;
    return object;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_life_s_push_level_typed( bcore_life_s* o, sz_t level, tp_t type, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_level_item( o, level );
    item->type   = type;
    item->object = object;
    item->typed_down_or_discard = ( bcore_fp_typed_down_or_discard )bcore_inst_t_discard;
    return object;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_life_s_spush_typed( bcore_life_s* o, tp_t type, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->type   = type;
    item->object = object;
    item->typed_down_or_discard = ( bcore_fp_typed_down_or_discard )bcore_inst_t_down;
    return object;
}

// ---------------------------------------------------------------------------------------------------------------------

sr_s bcore_life_s_push_sr( bcore_life_s* o, sr_s object )
{
    if( sr_s_is_strong( &object ) )
    {
        return sr_pocs( object.p, bcore_life_s_push_typed( o, sr_s_type( &object ), object.o ), sr_s_is_const( &object ), false );
    }
    else
    {
        return object;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_life_s_push_aware( bcore_life_s* o, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->down_or_discard = ( bcore_fp_down_or_discard )bcore_inst_a_discard;
    item->object  = object;
    return object;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_life_s_push_level_aware( bcore_life_s* o, sz_t level, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_level_item( o, level );
    item->down_or_discard = ( bcore_fp_down_or_discard )bcore_inst_a_discard;
    item->object  = object;
    return object;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_life_s_spush_aware( bcore_life_s* o, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->down_or_discard = ( bcore_fp_down_or_discard )bcore_inst_a_down;
    item->object  = object;
    return object;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_life_s_push_free( bcore_life_s* o, vd_t object )
{
    bcore_life_item_s* item = bcore_life_s_push_item( o );
    item->down_or_discard = ( bcore_fp_down_or_discard )bcore_free;
    item->object  = object;
    return object;
}

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_life_s_typed_create( bcore_life_s* o, tp_t type )
{
    return bcore_life_s_push_typed( o, type, bcore_inst_t_create( type ) );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_life_s_down_zero( bcore_life_s** o )
{
    if( !o || !*o ) return;
    bcore_life_s_down( *o );
    *o = NULL;
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_life_s_down_all( bcore_life_s* o )
{
    if( !o ) return;
    bcore_life_s* parent = o->parent;
    bcore_life_s_down( o );
    bcore_life_s_down_all( parent );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_life_s_down_level( bcore_life_s* o, sz_t nesting_level )
{
    if( !o ) return;
    if( o->nesting_level < nesting_level ) return;
    bcore_life_s* parent = o->parent;
    bcore_life_s_down( o );
    bcore_life_s_down_level( parent, nesting_level );
}

// ---------------------------------------------------------------------------------------------------------------------

void bcore_life_s_discard_all( bcore_life_s* o )
{
    if( !o ) return;
    bcore_life_s* parent = o->parent;
    bcore_life_s_discard( o );
    bcore_life_s_discard_all( parent );
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

static bcore_self_s* life_s_create_self( void )
{
    sc_t sc_def =
    "bcore_life_s = bcore_inst"
    "{"
        "aware_t _;"
        "private vd_t parent;"
        "private sz_t nesting_level;"
        "private vd_t data;"
        "private uz_t size;"
        "private uz_t space;"
    "}";

    bcore_self_s* self = bcore_self_s_build_parse_sc( sc_def, sizeof( bcore_life_s ), alignof( bcore_life_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_life_s_init, "bcore_fp_init",  "init" );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_life_s_down, "bcore_fp_down",  "down" );
    return self;
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

vd_t bcore_life_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_life" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "bcore_life_s"  ), life_s_create_self  );
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

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

