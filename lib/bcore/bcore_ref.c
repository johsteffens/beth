/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_ref.h"
#include "bcore_life.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_quicktypes.h"

/**********************************************************************************************************************/
// embedded usage

sr_s sr_clone( sr_s o )
{
    if( !o.o ) return sr_null();
    sr_s sr;
    sr.o = bcore_inst_typed_clone( sr_s_type( &o ), o.o );
    sr.p = o.p;
    sr.f = STRONG_f;
    sr_down( o );
    return sr;
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
    return bcore_inst_typed_clone_sr( type, obj );
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
        *o = sr_psd( src->p, bcore_inst_typed_clone( spect_tp_o( src->p ), src->o ) );
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

static bcore_flect_self_s* sr_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( " sr_s = bcore_inst { private vd_t o; private vc_t p; private tp_t f; shell typed * obj; }", sizeof( sr_s ) );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )sr_s_init,    "bcore_fp_init", "init"    );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )sr_s_down,    "bcore_fp_down", "down"    );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )sr_s_copy,    "bcore_fp_copy", "copy"    );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )sr_s_get_obj, "bcore_fp_get",  "get_obj" );
    bcore_flect_self_s_push_ns_func( self, ( fp_t )sr_s_set_obj, "bcore_fp_set",  "set_obj" );

    bcore_flect_self_s_push_ns_func( self, ( fp_t )sr_s_copy_typed, "bcore_fp_copy_typed", "copy_typed"  );
    return self;
}


/**********************************************************************************************************************/

vd_t bcore_ref_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_ref" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "sr_s"  ), sr_s_create_self  );
    }

    return NULL;
}

/**********************************************************************************************************************/

