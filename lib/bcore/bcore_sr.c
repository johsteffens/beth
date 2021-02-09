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

#include "bcore_sr.h"
#include "bcore_life.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_signal.h"

/**********************************************************************************************************************/
// embedded usage

vd_t bcore_inst_p_create_wrap( const bcore_inst_s* p )
{
    return bcore_inst_p_create( p );
}

vd_t bcore_inst_t_create_wrap( tp_t type )
{
    return bcore_inst_t_create( type );
}

const bcore_inst_s* bcore_inst_s_get_typed_wrap( tp_t type )
{
    return bcore_inst_s_get_typed( type );
}

void bcore_inst_x_discard_wrap( sr_s o )
{
    bcore_inst_x_discard( o );
}

sr_s sr_clone( sr_s o )
{
    if( !o.o ) return sr_null();
    sr_s sr;
    sr.o = bcore_inst_t_clone( sr_s_type( &o ), o.o );
    sr.p = o.p;
    sr.f = STRONG_f;
    sr_down( o );
    return sr;
}

sr_s sr_p_create( vc_t p )
{
    return sr_psd
    (
        p,
        ( ( (tp_t*)p )[ 0 ] == TYPEOF_bcore_inst_s ) ? bcore_inst_p_create( p ) : bcore_inst_t_create( ( (tp_t*)p )[ 1 ] )
    );
}

sr_s sr_spect( sr_s o, tp_t spect_type )
{
    o.p = ch_spect_p( o.p, spect_type );
    return o;
}

sr_s sr_cl( sr_s o, bcore_life_s* l )
{
    return bcore_life_s_push_sr( l, o );
}

sr_s sr_create_strong_typed( tp_t type, vc_t obj )
{
    return bcore_inst_t_clone_sr( type, obj );
}

sr_s sr_s0( s0_t v ) { sr_s r = sr_create( TYPEOF_s0_t ); *( s0_t * )r.o = v; return r; }
sr_s sr_s1( s1_t v ) { sr_s r = sr_create( TYPEOF_s1_t ); *( s1_t * )r.o = v; return r; }
sr_s sr_s2( s2_t v ) { sr_s r = sr_create( TYPEOF_s2_t ); *( s2_t * )r.o = v; return r; }
sr_s sr_s3( s3_t v ) { sr_s r = sr_create( TYPEOF_s3_t ); *( s3_t * )r.o = v; return r; }
sr_s sr_u0( u0_t v ) { sr_s r = sr_create( TYPEOF_u0_t ); *( u0_t * )r.o = v; return r; }
sr_s sr_u1( u1_t v ) { sr_s r = sr_create( TYPEOF_u1_t ); *( u1_t * )r.o = v; return r; }
sr_s sr_u2( u2_t v ) { sr_s r = sr_create( TYPEOF_u2_t ); *( u2_t * )r.o = v; return r; }
sr_s sr_u3( u3_t v ) { sr_s r = sr_create( TYPEOF_u3_t ); *( u3_t * )r.o = v; return r; }
sr_s sr_f2( f2_t v ) { sr_s r = sr_create( TYPEOF_f2_t ); *( f2_t * )r.o = v; return r; }
sr_s sr_f3( f3_t v ) { sr_s r = sr_create( TYPEOF_f3_t ); *( f3_t * )r.o = v; return r; }
sr_s sr_sz( sz_t v ) { sr_s r = sr_create( TYPEOF_sz_t ); *( sz_t * )r.o = v; return r; }
sr_s sr_uz( uz_t v ) { sr_s r = sr_create( TYPEOF_uz_t ); *( uz_t * )r.o = v; return r; }
sr_s sr_tp( tp_t v ) { sr_s r = sr_create( TYPEOF_tp_t ); *( tp_t * )r.o = v; return r; }
sr_s sr_bl( bl_t v ) { sr_s r = sr_create( TYPEOF_bl_t ); *( bl_t * )r.o = v; return r; }

sr_s sr_string_sc( sc_t v )
{
    sr_s r = sr_create( TYPEOF_st_s );
    st_s_copy_sc( r.o, v );
    return r;
}

/**********************************************************************************************************************/
// object usage

void sr_s_init( sr_s* o )
{
    bcore_memzero( o, sizeof( *o ) );
}

void sr_s_down( sr_s* o )
{
    if( o ) sr_down( *o );
}

void sr_s_copy( sr_s* o, const sr_s* src )
{
    sr_s_clear( o );
    if( src->o )
    {
        *o = sr_psd( src->p, bcore_inst_t_clone( spect_tp_o( src->p ), src->o ) );
    }
}

static void sr_s_copy_typed( sr_s* o, tp_t type, vc_t src )
{
    sr_s sr = sr_twc( type, src );
    sr_s_copy( o, &sr );
}

sr_s* sr_s_create()
{
    sr_s* o = bcore_u_alloc( sizeof( sr_s ), NULL, 1, NULL );
    sr_s_init( o );
    return o;
}

sr_s* sr_s_clone( const sr_s* src )
{
    if( !src ) return NULL;
    sr_s* o = sr_s_create();
    sr_s_copy( o, src );
    return o;
}

void sr_s_discard( sr_s* o )
{
    if( !o ) return;
    sr_s_down( o );
}

static sr_s sr_s_get_obj( sr_s* o )
{
    return sr_s_get( o );
}

static void sr_s_set_obj( sr_s* o, sr_s obj )
{
    sr_s_set( o, obj );
}

static bcore_self_s* sr_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( " sr_s = bcore_inst { private vd_t o; private vc_t p; private tp_t f; shell typed => obj; }", sizeof( sr_s ), alignof( sr_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )sr_s_init,    "bcore_fp_init", "init"    );
    bcore_self_s_push_ns_func( self, ( fp_t )sr_s_down,    "bcore_fp_down", "down"    );
    bcore_self_s_push_ns_func( self, ( fp_t )sr_s_copy,    "bcore_fp_copy", "copy"    );
    bcore_self_s_push_ns_func( self, ( fp_t )sr_s_get_obj, "bcore_fp_get",  "get_obj" );
    bcore_self_s_push_ns_func( self, ( fp_t )sr_s_set_obj, "bcore_fp_set",  "set_obj" );

    bcore_self_s_push_ns_func( self, ( fp_t )sr_s_copy_typed, "bcore_fp_copy_typed", "copy_typed"  );
    return self;
}

f3_t sr_s_to_f3( const sr_s* o )
{
    f3_t v = 0;
    switch( sr_s_type( o ) )
    {
        case TYPEOF_u0_t: v = *( u0_t* )o->o; break;
        case TYPEOF_u1_t: v = *( u1_t* )o->o; break;
        case TYPEOF_u2_t: v = *( u2_t* )o->o; break;
        case TYPEOF_u3_t: v = *( u3_t* )o->o; break;
        case TYPEOF_s0_t: v = *( s0_t* )o->o; break;
        case TYPEOF_s1_t: v = *( s1_t* )o->o; break;
        case TYPEOF_s2_t: v = *( s2_t* )o->o; break;
        case TYPEOF_s3_t: v = *( s3_t* )o->o; break;
        case TYPEOF_f2_t: v = *( f2_t* )o->o; break;
        case TYPEOF_f3_t: v = *( f3_t* )o->o; break;
        case TYPEOF_bl_t: v = *( bl_t* )o->o; break;
        case TYPEOF_tp_t: v = *( tp_t* )o->o; break;
        case TYPEOF_aware_t: v = *( aware_t* )o->o; break;
        default: ERR_fa( "Cannot convert '#<sc_t>' to f3_t.", ifnameof( sr_s_type( o ) ) );
    }
    return v;
}

u3_t sr_s_to_u3( const sr_s* o )
{
    u3_t v = 0;
    switch( sr_s_type( o ) )
    {
        case TYPEOF_u0_t: v = *( u0_t* )o->o; break;
        case TYPEOF_u1_t: v = *( u1_t* )o->o; break;
        case TYPEOF_u2_t: v = *( u2_t* )o->o; break;
        case TYPEOF_u3_t: v = *( u3_t* )o->o; break;
        case TYPEOF_s0_t: v = *( s0_t* )o->o; break;
        case TYPEOF_s1_t: v = *( s1_t* )o->o; break;
        case TYPEOF_s2_t: v = *( s2_t* )o->o; break;
        case TYPEOF_s3_t: v = *( s3_t* )o->o; break;
        case TYPEOF_f2_t: v = *( f2_t* )o->o; break;
        case TYPEOF_f3_t: v = *( f3_t* )o->o; break;
        case TYPEOF_bl_t: v = *( bl_t* )o->o; break;
        case TYPEOF_tp_t: v = *( tp_t* )o->o; break;
        case TYPEOF_aware_t: v = *( aware_t* )o->o; break;
        default: ERR_fa( "Cannot convert '#<sc_t>' to u3_t.", ifnameof( sr_s_type( o ) ) );
    }
    return v;
}

s3_t sr_s_to_s3( const sr_s* o )
{
    s3_t v = 0;
    switch( sr_s_type( o ) )
    {
        case TYPEOF_u0_t: v = *( u0_t* )o->o; break;
        case TYPEOF_u1_t: v = *( u1_t* )o->o; break;
        case TYPEOF_u2_t: v = *( u2_t* )o->o; break;
        case TYPEOF_u3_t: v = *( u3_t* )o->o; break;
        case TYPEOF_s0_t: v = *( s0_t* )o->o; break;
        case TYPEOF_s1_t: v = *( s1_t* )o->o; break;
        case TYPEOF_s2_t: v = *( s2_t* )o->o; break;
        case TYPEOF_s3_t: v = *( s3_t* )o->o; break;
        case TYPEOF_f2_t: v = *( f2_t* )o->o; break;
        case TYPEOF_f3_t: v = *( f3_t* )o->o; break;
        case TYPEOF_bl_t: v = *( bl_t* )o->o; break;
        case TYPEOF_tp_t: v = *( tp_t* )o->o; break;
        case TYPEOF_aware_t: v = *( aware_t* )o->o; break;
        default: ERR_fa( "Cannot convert '#<sc_t>' to s3_t.", ifnameof( sr_s_type( o ) ) );
    }
    return v;
}

bl_t sr_s_to_bl( const sr_s* o )
{
    bl_t v = 0;
    switch( sr_s_type( o ) )
    {
        case TYPEOF_u0_t: v = *( u0_t* )o->o; break;
        case TYPEOF_u1_t: v = *( u1_t* )o->o; break;
        case TYPEOF_u2_t: v = *( u2_t* )o->o; break;
        case TYPEOF_u3_t: v = *( u3_t* )o->o; break;
        case TYPEOF_s0_t: v = *( s0_t* )o->o; break;
        case TYPEOF_s1_t: v = *( s1_t* )o->o; break;
        case TYPEOF_s2_t: v = *( s2_t* )o->o; break;
        case TYPEOF_s3_t: v = *( s3_t* )o->o; break;
        case TYPEOF_f2_t: v = *( f2_t* )o->o; break;
        case TYPEOF_f3_t: v = *( f3_t* )o->o; break;
        case TYPEOF_bl_t: v = *( bl_t* )o->o; break;
        case TYPEOF_tp_t: v = *( tp_t* )o->o; break;
        case TYPEOF_aware_t: v = *( aware_t* )o->o; break;
        default: ERR_fa( "Cannot convert '#<sc_t>' to bl_t.", ifnameof( sr_s_type( o ) ) );
    }
    return v;
}

tp_t sr_s_to_tp( const sr_s* o )
{
    tp_t v = 0;
    switch( sr_s_type( o ) )
    {
        case TYPEOF_u0_t: v = *( u0_t* )o->o; break;
        case TYPEOF_u1_t: v = *( u1_t* )o->o; break;
        case TYPEOF_u2_t: v = *( u2_t* )o->o; break;
        case TYPEOF_u3_t: v = *( u3_t* )o->o; break;
        case TYPEOF_s0_t: v = *( s0_t* )o->o; break;
        case TYPEOF_s1_t: v = *( s1_t* )o->o; break;
        case TYPEOF_s2_t: v = *( s2_t* )o->o; break;
        case TYPEOF_s3_t: v = *( s3_t* )o->o; break;
        case TYPEOF_tp_t: v = *( tp_t* )o->o; break;
        case TYPEOF_aware_t: v = *( aware_t* )o->o; break;
        default: ERR_fa( "Cannot convert '#<sc_t>' to tp_t.", ifnameof( sr_s_type( o ) ) );
    }
    return v;
}

/**********************************************************************************************************************/

vd_t bcore_ref_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_ref" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "sr_s"  ), sr_s_create_self  );
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

