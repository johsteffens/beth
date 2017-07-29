/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_closure.h"
#include "bcore_quicktypes.h"
#include "bcore_life.h"

#include "bclos_algebra.h"

/**********************************************************************************************************************/
// functions of type: f3 f( f3, f3 )

static sz_t f3_bi_n_args( vc_t o )             { return 2; }
static tp_t f3_bi_t_arg(  vc_t o, sz_t index ) { return ( index < 2 ) ? TYPEOF_f3_t : 0; }
static tp_t f3_bi_t_ret(  vc_t o, sz_t index ) { return TYPEOF_f3_t; }

static bcore_flect_self_s* f3_bi_self( sc_t name, bcore_closure_fp_func func )
{
    bcore_string_s* def = bcore_string_s_createf( "%s = { aware_t _; }", name );
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def->sc, sizeof( aware_t ) );
    bcore_string_s_discard( def );
    bcore_flect_self_s_push_external_func( self, ( fp_t )func,         "bcore_closure_fp_func",  "func"   );
    bcore_flect_self_s_push_external_func( self, ( fp_t )f3_bi_n_args, "bcore_closure_fp_n_args","n_args" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )f3_bi_t_arg,  "bcore_closure_fp_t_arg", "t_arg"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )f3_bi_t_ret,  "bcore_closure_fp_t_ret", "t_ret"  );
    return self;
}

static vd_t f3_bi_mul( vd_t o, const f3_t* const* args, sz_t n_args )
{
    assert( n_args >= 2 );
    f3_t* f3p = bcore_alloc( NULL, sizeof( f3_t ) );
    *f3p = ( *args[ 0 ] ) * ( *args[ 1 ] );
    return f3p;
}

bcore_flect_self_s* f3_bi_mul_create_self( void ) { return f3_bi_self( "bclos_f3_mul", ( bcore_closure_fp_func )f3_bi_mul ); }

static vd_t f3_bi_add( vd_t o, const f3_t* const* args, sz_t n_args )
{
    assert( n_args >= 2 );
    f3_t* f3p = bcore_alloc( NULL, sizeof( f3_t ) );
    *f3p = ( *args[ 0 ] ) + ( *args[ 1 ] );
    return f3p;
}

bcore_flect_self_s* f3_bi_add_create_self( void ) { return f3_bi_self( "bclos_f3_add", ( bcore_closure_fp_func )f3_bi_add ); }

/**********************************************************************************************************************/

void bclos_algebra_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bclos_f3_mul" ), f3_bi_mul_create_self );
    bcore_flect_define_creator( typeof( "bclos_f3_add" ), f3_bi_add_create_self );
}

/**********************************************************************************************************************/

bcore_string_s* bclos_algebra_selftest()
{
    bcore_string_s* log = bcore_string_s_create();
    bcore_life_s* l = bcore_life_s_create();

    vd_t op_add = bcore_life_s_push_aware( l, bcore_inst_typed_create( typeof( "bclos_f3_add" ) ) );
    vd_t op_mul = bcore_life_s_push_aware( l, bcore_inst_typed_create( typeof( "bclos_f3_mul" ) ) );

    f3_t v1 = 5.0;
    f3_t v2 = 6.0;
    const vc_t args[] = { &v1, &v2 };

    ASSERT( v1 + v2 == *( f3_t* )bcore_life_s_push_typed( l, typeof( "f3_t" ), bcore_closure_aware_func( op_add, args, 2 ) ) );
    ASSERT( v1 * v2 == *( f3_t* )bcore_life_s_push_typed( l, typeof( "f3_t" ), bcore_closure_aware_func( op_mul, args, 2 ) ) );

    bcore_life_s_discard( l );
    return log;
}
