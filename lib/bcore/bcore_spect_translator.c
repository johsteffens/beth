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

#include "bcore_spect_translator.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/
// bcore_translator_s

static void translator_s_down( bcore_translator_s* o );

static void translator_s_init( bcore_translator_s* o )
{
    bcore_memzero( o, sizeof( bcore_translator_s ) );
    o->p_type = TYPEOF_bcore_translator_s;
}

static void translator_s_down( bcore_translator_s* o )
{
}

static bcore_translator_s* translator_s_create()
{
    bcore_translator_s* o = bcore_alloc( NULL, sizeof( bcore_translator_s ) );
    translator_s_init( o );
    return o;
}

static void translator_s_discard( bcore_translator_s* o )
{
    if( !o ) return;
    bcore_release_obj( ( fp_t )translator_s_down, o );
}

/**********************************************************************************************************************/

static void translator_s_define_trait()
{
    tp_t trait = entypeof( "bcore_translator" );
    bcore_trait_require_function( trait, entypeof( "bcore_fp_translate" ), 0 );
    bcore_trait_set( trait, entypeof( "bcore_inst" ) );
}

static bcore_translator_s* create_from_self( const bcore_self_s* self )
{
    assert( self != NULL );
    bcore_translator_s* o = translator_s_create();
    o->o_type = self->type;
    o->fp_translate = ( bcore_fp_translate )bcore_self_s_get_external_fp( self, bcore_name_enroll( "bcore_fp_translate" ), 0 );
    return o;
}

static bcore_self_s* translator_s_create_self( void )
{
    sc_t def = "bcore_translator_s = spect { aware_t p_type; tp_t o_type; ... }";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bcore_translator_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )translator_s_init,             "bcore_fp_init",                   "init"         );
    bcore_self_s_push_ns_func( self, ( fp_t )translator_s_down,             "bcore_fp_down",                   "down"         );
    bcore_self_s_push_ns_func( self, ( fp_t )translator_s_create,           "bcore_fp_create",                 "create"       );
    bcore_self_s_push_ns_func( self, ( fp_t )translator_s_discard,          "bcore_fp_discard",                "discard"      );
    bcore_self_s_push_ns_func( self, ( fp_t )create_from_self,              "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

void bcore_translate( sr_s o, sr_s obj, sr_s sink )
{
    const bcore_translator_s* p = ch_spect_p( o.p, TYPEOF_bcore_translator_s );
    p->fp_translate( o.o, obj, sink );
    sr_down( o );
}

void bcore_translate_q( const sr_s* o, sr_s obj, sr_s sink )
{
    const bcore_translator_s* p = ch_spect_p( o->p, TYPEOF_bcore_translator_s );
    p->fp_translate( o->o, obj, sink );
}

/**********************************************************************************************************************/

BCORE_DEFINE_SPECT_CACHE( bcore_translator_s );

vd_t bcore_spect_translator_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_translator" ) ) )
    {
        case TYPEOF_init1:
        {
            translator_s_define_trait();
            bcore_flect_define_creator( typeof( "bcore_translator_s"  ), translator_s_create_self  );
            BCORE_REGISTER_SPECT_CACHE( bcore_translator_s );
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

/**********************************************************************************************************************/

