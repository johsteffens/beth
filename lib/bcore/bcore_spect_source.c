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
#include "bcore_error_manager.h"
#include "bcore.h"

#define NPX( name ) bcore_source_##name

/**********************************************************************************************************************/
/// bcore_source_context_s
/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_source_context_s )
"{"
    "aware_t _; "
    "st_s => file_path;" // file path; NULL if not available
    "s3_t    index;" // current index
    "sz_t    line;"  // line number for text based sources
    "sz_t    col;"   // column number for text based sources
    "st_s => txt_context;" // text context (if available)
"}";

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_context_s_get_msg_fv( const bcore_source_context_s* o, st_s* msg, sc_t format, va_list args )
{
    st_s_clear( msg );
    st_s_push_fa( msg, "#<sc_t>:#<uz_t>:#<uz_t>: ", o->file_path ? o->file_path->sc : "", o->line, o->col );
    st_s_push_fv( msg, format, args );
    if( o->txt_context )
    {
        st_s_push_fa( msg, "\n#<sc_t>", o->txt_context->sc );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_context_s_get_msg_fa( const bcore_source_context_s* o, st_s* msg, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_source_context_s_get_msg_fv( o, msg, format, args );
    va_end( args );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_context_s_get_error_msg_fv( const bcore_source_context_s* o, st_s* msg, sc_t format, va_list args )
{
    st_s_clear( msg );
    st_s_push_fa( msg, "#<sc_t>:#<uz_t>:#<uz_t>: error: ", o->file_path ? o->file_path->sc : "", o->line, o->col );
    st_s_push_fv( msg, format, args );
    if( o->txt_context )
    {
        st_s_push_fa( msg, "\n#<sc_t>", o->txt_context->sc );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_context_s_get_error_msg_fa( const bcore_source_context_s* o, st_s* msg, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    bcore_source_context_s_get_error_msg_fv( o, msg, format, args );
    va_end( args );
}

//----------------------------------------------------------------------------------------------------------------------

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

char bcore_source_default_inspect_char( const bcore_source_s* p, bcore_source* o )
{
    return bcore_source_default_inspect_u0( p, o );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_parse_errvf( const bcore_source_s* p, bcore_source* o, sc_t format, va_list args )
{
    st_s* s0 = st_s_createvf( format, args );
    st_s* s1 = st_s_create();

    bcore_source_a_parse_msg_to_sink_fa( o, ( bcore_sink* )s1, "error: #<sc_t>", s0->sc );
    bcore_sink_a_push_fa( BCORE_STDERR, "\n#<sc_t>", s1->sc );

    st_s_discard( s0 );
    st_s_discard( s1 );
    bcore_down_exit( -1, 1 );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_get_context( const bcore_source_s* p, bcore_source* o, bcore_source_context_s* context )
{
    if( p->get_context )
    {
        p->get_context( o, context );
    }
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_parse_msgvf( const bcore_source_s* p, bcore_source* o, sc_t format, va_list args )
{
    st_s* s0 = st_s_create_fv( format, args );
    bcore_source_context_s* context = bcore_source_context_s_create();
    bcore_source_p_get_context( p, o, context );

    st_s* s1 = st_s_create();
    bcore_source_context_s_get_msg_fa( context, s1, "#<sc_t>", s0->sc );
    bcore_sink_a_push_fa( BCORE_STDOUT, "#<sc_t>\n", s1->sc );

    st_s_discard( s0 );
    st_s_discard( s1 );
    bcore_source_context_s_discard( context );
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
    st_s* s0 = st_s_create_fv( format, args );
    bcore_source_context_s* context = bcore_source_context_s_create();
    bcore_source_p_get_context( p, o, context );

    st_s* s1 = st_s_create();
    bcore_source_context_s_get_msg_fa( context, s1, "#<sc_t>", s0->sc );
    bcore_sink_a_push_fa( sink, "#<sc_t>", s1->sc );
    st_s_discard( s0 );
    st_s_discard( s1 );
    bcore_source_context_s_discard( context );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_source_default_parse_msg_fv( const bcore_source_s* p, bcore_source* o, sc_t format, va_list args )
{
    bcore_source_p_parse_msg_to_sink_fv( p, o, BCORE_STDOUT, format, args );
}

//----------------------------------------------------------------------------------------------------------------------

/// deprecated
bl_t bcore_source_default_parse_bl_fa( const bcore_source_s* p, bcore_source* o, sc_t format )
{
    bl_t flag = false;
    bcore_source_p_parse_fa( p, o, format, &flag );
    return flag;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_source_default_parse_bl( const bcore_source_s* p, bcore_source* o, sc_t format )
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

void bcore_source_default_set_parent( const bcore_source_s* p, bcore_source* o, vd_t parent )
{
    if( p->set_parent ) p->set_parent( o, parent );
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
    o->get_context     = ( bcore_source_fp_get_context     )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_get_context" ), 0 );
    o->parse_fv        = ( bcore_source_fp_parse_fv        )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_parse_fv" ), 0 );
    o->parse_em_fv     = ( bcore_source_fp_parse_em_fv     )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_parse_em_fv" ), 0 );
    o->set_supplier    = ( bcore_source_fp_set_supplier    )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_set_supplier" ), 0 );
    o->set_parent      = ( bcore_source_fp_set_parent      )bcore_self_s_try_external_fp( self, bcore_name_enroll( "bcore_source_fp_set_parent" ), 0 );
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
    bcore_self_s* self = bcore_self_s_create_plain( entypeof( "bcore_source_s" ), typeof( "spect" ), sizeof( bcore_source_s ), alignof( bcore_source_s ) );
    bcore_self_s_push_d( self, bcore_self_item_s_create_plain( BCORE_CAPS_SOLID_STATIC, TYPEOF_bcore_spect_header_s, entypeof( "header" ) ) );
    bcore_self_body_s_set_complete( self->body, false );
    bcore_self_s_push_ns_func( self, ( fp_t )create_from_self,  "bcore_spect_fp_create_from_self",  "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

void NPX(p_parse_fa    )( const NPX(s)* p, bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_fv    )( p, o, f, a ); va_end( a ); }
er_t NPX(p_parse_em_fa )( const NPX(s)* p, bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); er_t r = NPX(p_parse_em_fv )( p, o, f, a ); va_end( a ); return r; }
void NPX(p_parse_errf  )( const NPX(s)* p, bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_errvf )( p, o, f, a ); va_end( a ); }
void NPX(p_parse_msgf  )( const NPX(s)* p, bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_msgvf )( p, o, f, a ); va_end( a ); }
void NPX(p_parse_err_fa)( const NPX(s)* p, bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_err_fv)( p, o, f, a ); va_end( a ); }
void NPX(p_parse_msg_fa)( const NPX(s)* p, bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_msg_fv)( p, o, f, a ); va_end( a ); }
bcore_source* NPX(a_parse_fa )( bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(a_parse_fv    )( o, f, a ); va_end( a ); return o; }
er_t NPX(a_parse_em_fa )( bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); er_t r = NPX(a_parse_em_fv )( o, f, a ); va_end( a ); return r; }
void NPX(a_parse_errf  )( bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(a_parse_errvf )( o, f, a ); va_end( a ); }
void NPX(a_parse_err_fa)( bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(a_parse_err_fv)( o, f, a ); va_end( a ); }
void NPX(a_parse_msg_fa)( bcore_source* o, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(a_parse_msg_fv)( o, f, a ); va_end( a ); }
void NPX(x_parse_fa    )( sr_s o, sc_t f, ...          ) { va_list a; va_start( a, f ); NPX(x_parse_fv    )( o, f, a ); va_end( a ); }
er_t NPX(x_parse_em_fa )( sr_s o, sc_t f, ...          ) { va_list a; va_start( a, f ); er_t r = NPX(x_parse_em_fv )( o, f, a ); va_end( a ); return r; }
void NPX(x_parse_errf  )( sr_s o, sc_t f, ...          ) { va_list a; va_start( a, f ); NPX(x_parse_errvf )( o, f, a ); va_end( a ); }
void NPX(x_parse_err_fa)( sr_s o, sc_t f, ...          ) { va_list a; va_start( a, f ); NPX(x_parse_err_fv)( o, f, a ); va_end( a ); }
void NPX(x_parse_msg_fa)( sr_s o, sc_t f, ...          ) { va_list a; va_start( a, f ); NPX(x_parse_msg_fv)( o, f, a ); va_end( a ); }
void NPX(r_parse_fa    )( const sr_s* o, sc_t f, ...   ) { va_list a; va_start( a, f ); NPX(r_parse_fv    )( o, f, a ); va_end( a ); }
er_t NPX(r_parse_em_fa )( const sr_s* o, sc_t f, ...   ) { va_list a; va_start( a, f ); er_t r = NPX(r_parse_em_fv )( o, f, a ); va_end( a ); return r; }
void NPX(r_parse_errf  )( const sr_s* o, sc_t f, ...   ) { va_list a; va_start( a, f ); NPX(r_parse_errvf )( o, f, a ); va_end( a ); }
void NPX(r_parse_err_fa)( const sr_s* o, sc_t f, ...   ) { va_list a; va_start( a, f ); NPX(r_parse_err_fv)( o, f, a ); va_end( a ); }
void NPX(r_parse_msg_fa)( const sr_s* o, sc_t f, ...   ) { va_list a; va_start( a, f ); NPX(r_parse_msg_fv)( o, f, a ); va_end( a ); }

void NPX(p_parse_msg_to_sink_fa)( const NPX(s)* p, bcore_source* o, bcore_sink* s, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(p_parse_msg_to_sink_fv)( p, o, s, f, a ); va_end( a ); }
void NPX(a_parse_msg_to_sink_fa)(                  bcore_source* o, bcore_sink* s, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(a_parse_msg_to_sink_fv)(    o, s, f, a ); va_end( a ); }
void NPX(x_parse_msg_to_sink_fa)(                          sr_s  o, bcore_sink* s, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(x_parse_msg_to_sink_fv)(    o, s, f, a ); va_end( a ); }
void NPX(r_parse_msg_to_sink_fa)(                    const sr_s* o, bcore_sink* s, sc_t f, ... ) { va_list a; va_start( a, f ); NPX(r_parse_msg_to_sink_fv)(    o, s, f, a ); va_end( a ); }

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_source_a_parse_error_fv( bcore_source* o, sc_t format, va_list args )
{
    st_s* s0 = st_s_create_fv( format, args );
    st_s* s1 = st_s_create();

    bcore_source_a_parse_msg_to_sink_fa( o, ( bcore_sink* )s1, "error: #<sc_t>", s0->sc );
    er_t err_id = bcore_error_push_sc( TYPEOF_parse_error, s1->sc );

    st_s_discard( s0 );
    st_s_discard( s1 );
    return err_id;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_source_a_parse_error_fa( bcore_source* o, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    er_t ret = bcore_source_a_parse_error_fv( o, format, args );
    va_end( args );
    return ret;
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
            BCORE_REGISTER_OBJECT( bcore_source_context_s );

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


