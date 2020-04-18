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

#include "bcore_spect_sink.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_signal.h"
#include "bcore_spect_inst.h"

/**********************************************************************************************************************/
// bcore_sink_s

static void sink_s_init( bcore_sink_s* o )
{
    bcore_memzero( o, sizeof( bcore_sink_s ) );
    o->header.p_type = typeof( "bcore_sink_s" );
}

static bcore_sink_s* sink_s_create()
{
    bcore_sink_s* o = bcore_alloc( NULL, sizeof( bcore_sink_s ) );
    sink_s_init( o );
    return o;
}

/**********************************************************************************************************************/

void bcore_sink_default_pushvf( const bcore_sink_s* p, bcore_sink* o, sc_t format, va_list args )
{
    st_s* s = st_s_createvf( format, args );
    p->push_data( o, s->data, s->size );
    st_s_discard( s );
}

void bcore_sink_default_pushf( const bcore_sink_s* p, bcore_sink* o, sc_t f, ... )
{
    va_list a;
    va_start( a, f );
    bcore_sink_default_pushvf( p, o, f, a );
    va_end( a );
}

void bcore_sink_default_push_fv( const bcore_sink_s* p, bcore_sink* o, sc_t format, va_list args )
{
    st_s* s = st_s_create_fv( format, args );
    p->push_data( o, s->data, s->size );
    st_s_discard( s );
}

void bcore_sink_default_push_fa( const bcore_sink_s* p, bcore_sink* o, sc_t f, ... )
{
    va_list a;
    va_start( a, f );
    bcore_sink_default_push_fv( p, o, f, a );
    va_end( a );
}

void bcore_sink_default_push_char( const bcore_sink_s* p, bcore_sink* o, char c )
{
    p->push_data( o, &c, 1 );
}

void bcore_sink_default_push_sc( const bcore_sink_s* p, bcore_sink* o, sc_t sc )
{
    while( *sc )
    {
        bcore_sink_default_push_char( p, o, *sc );
        sc++;
    }
}

void bcore_sink_default_push_string( const bcore_sink_s* p, bcore_sink* o, const st_s* string )
{
    p->push_data( o, string->data, string->size );
}

void bcore_sink_default_push_string_d( const bcore_sink_s* p, bcore_sink* o, st_s* string )
{
    p->push_data( o, string->data, string->size );
    st_s_discard( string );
}

/**********************************************************************************************************************/

static void sink_s_define_trait()
{
    tp_t trait = entypeof( "bcore_sink" );
    bcore_trait_require_awareness( trait );
    bcore_trait_require_function( trait, entypeof( "bcore_fp_flow_snk" ), 0 );
    bcore_trait_set( trait, entypeof( "bcore_inst" ) );
}

static bcore_sink_s* create_from_self( const bcore_self_s* self )
{
    assert( self != NULL );
    assert( bcore_self_s_is_aware( self ) );

    bcore_sink_s* o = sink_s_create();
    o->header.o_type = self->type;
    o->push_data     = ( bcore_fp_flow_snk          )bcore_self_s_get_external_fp( self, bcore_name_enroll( "bcore_fp_flow_snk" ), 0 );
    o->set_consumer  = ( bcore_sink_fp_set_consumer )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_sink_fp_set_consumer" ), 0 );
    o->flush         = ( bcore_sink_fp_flush        )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_sink_fp_flush" ), 0 );
    return o;
}

static bcore_self_s* sink_s_create_self( void )
{
    sc_t def = "bcore_sink_s = spect { aware_t p_type; tp_t o_type; ... }";
    bcore_self_s* self = BCORE_SELF_S_BUILD_PARSE_SC( def, bcore_sink_s );
    bcore_self_s_push_ns_func( self, ( fp_t )create_from_self,        "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

void bcore_sink_a_pushf   ( bcore_sink* o, sc_t f, ... ) { va_list a; va_start( a, f ); bcore_sink_a_pushvf(  o, f, a ); va_end( a ); }
void bcore_sink_a_push_fa ( bcore_sink* o, sc_t f, ... ) { va_list a; va_start( a, f ); bcore_sink_a_push_fv( o, f, a ); va_end( a ); }
void bcore_sink_x_pushf   ( sr_s o, sc_t f, ... )        { va_list a; va_start( a, f ); bcore_sink_x_pushvf(  o, f, a ); va_end( a ); }
void bcore_sink_x_push_fa ( sr_s o, sc_t f, ... )        { va_list a; va_start( a, f ); bcore_sink_x_push_fv( o, f, a ); va_end( a ); }
void bcore_sink_r_pushf   ( const sr_s* o, sc_t f, ... ) { va_list a; va_start( a, f ); bcore_sink_r_pushvf ( o, f, a ); va_end( a ); }
void bcore_sink_r_push_fa ( const sr_s* o, sc_t f, ... ) { va_list a; va_start( a, f ); bcore_sink_r_push_fv( o, f, a ); va_end( a ); }

bcore_sink* bcore_sink_t_clone( tp_t type )
{
    bcore_trait_assert_satisfied_type( TYPEOF_bcore_sink, type );
    return ( bcore_sink* )bcore_inst_t_create( type );
}

void bcore_sink_a_discard( bcore_sink* o )
{
    bcore_inst_a_discard( ( bcore_inst* )o );
}

void bcore_sink_a_detach( bcore_sink** o )
{
    if( !o ) return;
    bcore_sink_a_discard( *o );
    *o = NULL;
}

void bcore_sink_a_attach( bcore_sink** o, bcore_sink* src )
{
    if( !o ) return;
    bcore_sink_a_discard( *o );
    *o = src;
}

/**********************************************************************************************************************/

BCORE_DEFINE_SPECT_CACHE( bcore_sink_s );

vd_t bcore_spect_sink_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_sink" ) ) )
    {
        case TYPEOF_init1:
        {
            sink_s_define_trait();
            bcore_flect_define_creator( typeof( "bcore_sink_s"  ), sink_s_create_self  );
            bcore_spect_setup_cache( &bcore_sink_s_cache_g );
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

