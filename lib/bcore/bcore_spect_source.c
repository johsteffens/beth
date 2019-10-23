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

#include "bcore_spect_source.h"
#include "bcore_spect_inst.h"
#include "bcore_flect.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_signal.h"
#include "bcore_sinks.h"

#define NPX( name ) bcore_source_##name

/**********************************************************************************************************************/
// bcore_source_s

static void source_s_init( bcore_source_s* o )
{
    bcore_memzero( o, sizeof( bcore_source_s ) );
    o->header.p_type = typeof( "bcore_source_s" );
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_source_s* source_s_create()
{
    bcore_source_s* o = bcore_alloc( NULL, sizeof( bcore_source_s ) );
    source_s_init( o );
    return o;
}

/**********************************************************************************************************************/

uz_t bcore_source_default_inspect_data( const bcore_source_s* p, bcore_source* o, vd_t data, uz_t size )
{
    s3_t index = bcore_source_p_get_index( p, o );
    uz_t copied = bcore_source_p_get_data( p, o, data, size );
    bcore_source_p_set_index( p, o, index );
    return copied;
}

//----------------------------------------------------------------------------------------------------------------------

u0_t bcore_source_default_inspect_u0( const bcore_source_s* p, bcore_source* o )
{
    s3_t index = bcore_source_p_get_index( p, o );
    u0_t v = bcore_source_p_get_u0( p, o );
    bcore_source_p_set_index( p, o, index );
    return v;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_parse_errvf( const bcore_source_s* p, bcore_source* o, sc_t format, va_list args )
{
    bcore_sink_a_push_fa( BCORE_STDERR, "\nParse Error: " );
    bcore_source_p_context_to_sink( p, o, BCORE_STDERR );
    st_s* msg = st_s_createvf( format, args );
    bcore_sink_a_push_fa( BCORE_STDERR, "#<sc_t>\n", msg->sc );
    st_s_discard( msg );
    bcore_exit( 1 );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_context_to_sink( const bcore_source_s* p, bcore_source* o, bcore_sink* sink )
{
    if( p->context_to_sink )
    {
        p->context_to_sink( o, sink );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_parse_msgf( const bcore_source_s* p, bcore_source* o, sc_t format, va_list args )
{
    st_s* msg = st_s_createvf( format, args );
    bcore_msg( "%s\n", msg->sc );
    st_s_discard( msg );
    bcore_source_p_context_to_sink( p, o, BCORE_STDOUT );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_parse_msgvf( const bcore_source_s* p, bcore_source* o, sc_t format, va_list args )
{
    st_s* msg = st_s_createvf( format, args );
    bcore_msg( "%s\n", msg->sc );
    st_s_discard( msg );
    bcore_source_p_context_to_sink( p, o, BCORE_STDOUT );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_parse_err_fv( const bcore_source_s* p, bcore_source* o, sc_t format, va_list args )
{
    st_s* s = st_s_create_fv( format, args );
    bcore_source_p_parse_errf( p, o, "%s", s->sc );
    st_s_discard( s );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_parse_msg_to_sink_fv( const bcore_source_s* p, bcore_source* o, bcore_sink* sink, sc_t format, va_list args )
{
    if( !sink ) return;
    st_s* s = st_s_create_fv( format, args );
    if( s->size > 0 && s->data[ s->size - 1 ] != '\n' ) st_s_push_char( s, '\n' );
    bcore_sink_a_push_fa( sink, "#<sc_t>", s->sc );
    bcore_source_p_context_to_sink( p, o, sink );
    st_s_discard( s );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_parse_msg_fv( const bcore_source_s* p, bcore_source* o, sc_t format, va_list args )
{
    bcore_source_p_parse_msg_to_sink_fv( p, o, BCORE_STDOUT, format, args );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_source_default_parse_bl_fa( const bcore_source_s* p, bcore_source* o, sc_t format )
{
    bl_t flag = false;
    bcore_source_p_parse_fa( p, o, format, &flag );
    return flag;
}

//----------------------------------------------------------------------------------------------------------------------

sc_t bcore_source_default_get_file( const bcore_source_s* p, const bcore_source* o )
{
    if( p->get_file ) return p->get_file( o );
    return "";
}

//----------------------------------------------------------------------------------------------------------------------

s3_t bcore_source_default_get_index( const bcore_source_s* p, const bcore_source* o )
{
    if( p->get_index ) return p->get_index( o );
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_set_index( const bcore_source_s* p, bcore_source* o, s3_t index )
{
    if( !p->set_index ) ERR( "Object '%s' does not support feature 'bcore_source_fp_set_index'.", nameof( p->header.o_type ) );
    p->set_index( o, index );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_move_index( const bcore_source_s* p, bcore_source* o, s3_t delta )
{
    bcore_source_default_set_index( p, o, bcore_source_default_get_index( p, o ) + delta );
}

/**********************************************************************************************************************/

static void source_s_define_trait()
{
    tp_t trait = entypeof( "bcore_source" );
    bcore_trait_require_awareness( trait );
    bcore_trait_require_function( trait, entypeof( "bcore_fp_flow_src" ), 0 );
    bcore_trait_require_function( trait, entypeof( "bcore_source_fp_eos" ), 0 );
    bcore_trait_set( trait, entypeof( "bcore_inst" ) );
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_source_s* create_from_self( const bcore_self_s* self )
{
    assert( self != NULL );
    assert( bcore_self_s_is_aware( self ) );

    bcore_source_s* o = source_s_create();
    o->header.o_type = self->type;

    o->get_data        = ( bcore_fp_flow_src               )bcore_self_s_get_external_fp( self, bcore_name_enroll( "bcore_fp_flow_src" ), 0 );
    o->context_to_sink = ( bcore_source_fp_context_to_sink )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_context_to_sink" ), 0 );
    o->parse_fv        = ( bcore_source_fp_parse_fv        )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_parse_fv" ), 0 );
    o->set_supplier    = ( bcore_source_fp_set_supplier    )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_set_supplier" ), 0 );
    o->eos             = ( bcore_source_fp_eos             )bcore_self_s_get_external_fp( self, bcore_name_enroll( "bcore_source_fp_eos" ), 0 );
    o->get_file        = ( bcore_source_fp_get_file        )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_get_file" ), 0 );
    o->get_index       = ( bcore_source_fp_get_index       )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_get_index" ), 0 );
    o->set_index       = ( bcore_source_fp_set_index       )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_set_index" ), 0 );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_self_s* source_s_create_self( void )
{
//  We need to create this reflection manually because self_s_build_parse uses it.
    bcore_self_s* self = bcore_self_s_create_plain( entypeof( "bcore_source_s" ), typeof( "spect" ), sizeof( bcore_source_s ) );
    bcore_self_s_push_d( self, bcore_self_item_s_create_plain( BCORE_CAPS_SOLID_STATIC, TYPEOF_bcore_spect_header_s, entypeof( "header" ) ) );
    bcore_self_body_s_set_complete( self->body, false );
    bcore_self_s_push_ns_func( self, ( fp_t )create_from_self,  "bcore_spect_fp_create_from_self",  "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

void NPX(p_parse_fa    )( const NPX(s)* p, bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_fv    )( p, o, f, a ); va_end( a ); }
void NPX(p_parse_errf  )( const NPX(s)* p, bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_errvf )( p, o, f, a ); va_end( a ); }
void NPX(p_parse_msgf  )( const NPX(s)* p, bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_msgvf )( p, o, f, a ); va_end( a ); }
void NPX(p_parse_err_fa)( const NPX(s)* p, bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_err_fv)( p, o, f, a ); va_end( a ); }
void NPX(p_parse_msg_fa)( const NPX(s)* p, bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_msg_fv)( p, o, f, a ); va_end( a ); }
void NPX(a_parse_fa    )( bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(a_parse_fv    )( o, f, a ); va_end( a ); }
void NPX(a_parse_errf  )( bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(a_parse_errvf )( o, f, a ); va_end( a ); }
void NPX(a_parse_err_fa)( bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(a_parse_err_fv)( o, f, a ); va_end( a ); }
void NPX(a_parse_msg_fa)( bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(a_parse_msg_fv)( o, f, a ); va_end( a ); }
void NPX(x_parse_fa    )( sr_s o, sc_t f, ...          ) { va_list a; va_start( a, f ); NPX(x_parse_fv    )( o, f, a ); va_end( a ); }
void NPX(x_parse_errf  )( sr_s o, sc_t f, ...          ) { va_list a; va_start( a, f ); NPX(x_parse_errvf )( o, f, a ); va_end( a ); }
void NPX(x_parse_err_fa)( sr_s o, sc_t f, ...          ) { va_list a; va_start( a, f ); NPX(x_parse_err_fv)( o, f, a ); va_end( a ); }
void NPX(x_parse_msg_fa)( sr_s o, sc_t f, ...          ) { va_list a; va_start( a, f ); NPX(x_parse_msg_fv)( o, f, a ); va_end( a ); }
void NPX(r_parse_fa    )( const sr_s* o, sc_t f, ...   ) { va_list a; va_start( a, f ); NPX(r_parse_fv    )( o, f, a ); va_end( a ); }
void NPX(r_parse_errf  )( const sr_s* o, sc_t f, ...   ) { va_list a; va_start( a, f ); NPX(r_parse_errvf )( o, f, a ); va_end( a ); }
void NPX(r_parse_err_fa)( const sr_s* o, sc_t f, ...   ) { va_list a; va_start( a, f ); NPX(r_parse_err_fv)( o, f, a ); va_end( a ); }
void NPX(r_parse_msg_fa)( const sr_s* o, sc_t f, ...   ) { va_list a; va_start( a, f ); NPX(r_parse_msg_fv)( o, f, a ); va_end( a ); }

void NPX(p_parse_msg_to_sink_fa)( const NPX(s)* p, bcore_source* o, bcore_sink* s, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_msg_to_sink_fv)( p, o, s, f, a ); va_end( a ); }
void NPX(a_parse_msg_to_sink_fa)(                  bcore_source* o, bcore_sink* s, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(a_parse_msg_to_sink_fv)(    o, s, f, a ); va_end( a ); }
void NPX(x_parse_msg_to_sink_fa)(                          sr_s  o, bcore_sink* s, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(x_parse_msg_to_sink_fv)(    o, s, f, a ); va_end( a ); }
void NPX(r_parse_msg_to_sink_fa)(                    const sr_s* o, bcore_sink* s, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(r_parse_msg_to_sink_fv)(    o, s, f, a ); va_end( a ); }

//----------------------------------------------------------------------------------------------------------------------

bcore_source* bcore_source_t_clone( tp_t type )
{
    bcore_trait_assert_satisfied_type( TYPEOF_bcore_source, type );
    return ( bcore_source* )bcore_inst_t_create( type );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_a_discard( bcore_source* o )
{
    bcore_inst_a_discard( ( bcore_inst* )o );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_a_detach( bcore_source** o )
{
    if( !o ) return;
    bcore_source_a_discard( *o );
    *o = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_a_attach( bcore_source** o, bcore_source* src )
{
    if( !o ) return;
    bcore_source_a_discard( *o );
    *o = src;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_SPECT_CACHE( bcore_source_s );

vd_t bcore_spect_source_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_source" ) ) )
    {
        case TYPEOF_init1:
        {
            source_s_define_trait();
            bcore_flect_define_creator( typeof( "bcore_source_s"  ), source_s_create_self  );
            bcore_spect_setup_cache( &bcore_source_s_cache_g );
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

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


