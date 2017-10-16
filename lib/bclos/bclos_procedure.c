/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bclos_procedure.h"
#include "bclos_environment.h"
#include "bclos_quicktypes.h"
#include "bcore_quicktypes.h"
#include "bclos_spect_closure.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_life.h"
#include "bcore_trait.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_SPECT( bclos_procedure_s )
DEFINE_FUNCTION_DOWN_SPECT( bclos_procedure_s )
DEFINE_FUNCTION_COPY_SPECT( bclos_procedure_s )
DEFINE_FUNCTION_CREATE(     bclos_procedure_s )
DEFINE_FUNCTION_DISCARD(    bclos_procedure_s )
DEFINE_FUNCTION_CLONE(      bclos_procedure_s )

void bclos_procedure_s_push( bclos_procedure_s* o, sr_s statement )
{
    bcore_array_typed_push( TYPEOF_bclos_procedure_s, o, statement );
}

void bclos_procedure_s_push_sc( bclos_procedure_s* o, sc_t statement )
{
    bclos_procedure_s_push
    (
        o,
        sr_tsd
        (
            TYPEOF_bclos_statement_s,
            bclos_statement_s_parse_from_sc( statement )
        )
    );
}

void bclos_procedure_s_def( bclos_procedure_s* o, bclos_environment_s* env )
{
    o->lexical = env;
}

sr_s bclos_procedure_s_call( bclos_procedure_s* o, bclos_environment_s* env, const bclos_arguments_s* args )
{
    bclos_environment_s* local = bclos_environment_s_create(); // local environment
    local->external = o->lexical ? o->lexical : env;

    if( o->sig )
    {
        if( o->sig->size > args->size )
        {
            ERR( "Closure %s: %u arguments required; %u supplied", ifnameof( o->sig->name ), o->sig->size, args->size );
        }
        for( sz_t i = 0; i < o->sig->size; i++ )
        {
            bclos_signature_arg_s sig_arg = o->sig->data[ i ];
            sr_s arg_obj = sr_cw( args->data[ i ] );
            if( sr_s_is_const( &arg_obj ) && !sig_arg.is_const )
            {
                ERR( "Closure %s: Argument %u (%s %s): const violation.",
                    ifnameof( o->sig->name ),
                    i,
                    ifnameof( sig_arg.type ),
                    ifnameof( sig_arg.name ) );
            }
            if( ( sig_arg.type > 0 ) && ( arg_obj.p != NULL ) && ( sig_arg.type != sr_s_type( &arg_obj ) ) )
            {
                ERR( "Closure %s: Argument %u (%s %s): Incorrect type (%s) received.",
                    ifnameof( o->sig->name ),
                    i,
                    ifnameof( sig_arg.type ),
                    ifnameof( sig_arg.name ),
                    ifnameof( sr_s_type( &arg_obj ) ) );
            }
            bclos_environment_s_set( local, sig_arg.name, arg_obj );
        }
    }

    sr_s ret = sr_null();

    for( sz_t i = 0; i < o->size; i++ )
    {
        const bclos_statement_s* statement = o->data[ i ];
        ret = bclos_statement_s_run( statement, local );
        if( ret.o ) break;
    }


    if( o->sig->ret )
    {
        if( !bcore_trait_satisfied_type( o->sig->ret, sr_s_type( &ret ), NULL ) )
        {
            ERR( "Closure %s: Return type mismatch ('%s' required; '%s' generated).",
                ifnameof( o->sig->name ),
                ifnameof( o->sig->ret ),
                ifnameof( sr_s_type( &ret ) ) );
        }
    }

    bclos_environment_s_discard( local );

    return ret;
}

sr_s bclos_procedure_s_sig( const bclos_procedure_s* o )
{
    return sr_twc( TYPEOF_bclos_signature_s, o->sig );
}

static bcore_flect_self_s* procedure_s_create_self( void )
{
    sc_t def = "bclos_procedure_s = { aware_t _; bclos_statement_s * [] arr; bclos_signature_s* sig; private bclos_environment_s * lexcal; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_procedure_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_procedure_s_def,  "bclos_closure_fp_def",  "def"  );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_procedure_s_call, "bclos_closure_fp_call", "call" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_procedure_s_sig,  "bclos_closure_fp_sig",  "sig"  );
    return self;
}

/**********************************************************************************************************************/

#include "bcore_txt_ml.h"

static sr_s test_add( vc_t o, bclos_environment_s* env, const bclos_arguments_s* args )
{
    assert( args->size >= 2 );
    assert( sr_s_type( &args->data[ 0 ] ) == sr_s_type( &args->data[ 1 ] ) );
    vc_t a0 = args->data[ 0 ].o;
    vc_t a1 = args->data[ 1 ].o;
    tp_t t = sr_s_type( &args->data[ 0 ] );
    switch( t )
    {
        case TYPEOF_s2_t: return sr_s2( *( s2_t* )a0 + *( s2_t* )a1 );
        case TYPEOF_s3_t: return sr_s3( *( s3_t* )a0 + *( s3_t* )a1 );
        case TYPEOF_u2_t: return sr_u2( *( u2_t* )a0 + *( u2_t* )a1 );
        case TYPEOF_u3_t: return sr_u3( *( u3_t* )a0 + *( u3_t* )a1 );
        case TYPEOF_f2_t: return sr_f2( *( f2_t* )a0 + *( f2_t* )a1 );
        case TYPEOF_f3_t: return sr_f3( *( f3_t* )a0 + *( f3_t* )a1 );
        default: ERR( "cannot add type '%s'", ifnameof( t ) );
    }
    return sr_null();
}

static sr_s test_mul( vc_t o, bclos_environment_s* env, const bclos_arguments_s* args )
{
    assert( args->size >= 2 );
    assert( sr_s_type( &args->data[ 0 ] ) == sr_s_type( &args->data[ 1 ] ) );
    vc_t a0 = args->data[ 0 ].o;
    vc_t a1 = args->data[ 1 ].o;
    tp_t t = sr_s_type( &args->data[ 0 ] );
    switch( t )
    {
        case TYPEOF_s2_t: return sr_s2( *( s2_t* )a0 * *( s2_t* )a1 );
        case TYPEOF_s3_t: return sr_s3( *( s3_t* )a0 * *( s3_t* )a1 );
        case TYPEOF_u2_t: return sr_u2( *( u2_t* )a0 * *( u2_t* )a1 );
        case TYPEOF_u3_t: return sr_u3( *( u3_t* )a0 * *( u3_t* )a1 );
        case TYPEOF_f2_t: return sr_f2( *( f2_t* )a0 * *( f2_t* )a1 );
        case TYPEOF_f3_t: return sr_f3( *( f3_t* )a0 * *( f3_t* )a1 );
        default: ERR( "cannot multiply type '%s'", ifnameof( t ) );
    }
    return sr_null();
}

static st_s* procedure_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    {
        bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "test_add = bclos_closure_s {}", 0 );
        bcore_flect_self_s_push_external_func( self, ( fp_t )test_add, "bclos_closure_fp_call", "call" );
        bcore_flect_type_self_d( self );
    }
    {
        bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "test_mul = bclos_closure_s {}", 0 );
        bcore_flect_self_s_push_external_func( self, ( fp_t )test_mul, "bclos_closure_fp_call", "call" );
        bcore_flect_type_self_d( self );
    }

    {
        sr_s op_r = bcore_life_s_push_sr( l, bcore_inst_typed_create_sr( typeof( "test_mul" ) ) );
        sr_s res = bclos_closure_q_call_na( &op_r, NULL, 2, sr_s3( 25 ), sr_s3( -36 ) );
        ASSERT( *( s3_t* )res.o == -900 );
        sr_down( res );
    }

    {
        bclos_environment_s* env = bcore_life_s_push_aware( l, bclos_environment_s_create() );

        bclos_environment_s_set( env, typeof( "return" ), sr_create( typeof( "bclos_completion" ) ) );
        bclos_environment_s_set( env, typeof( "add" ), sr_create( typeof( "test_add" ) ) );
        bclos_environment_s_set( env, typeof( "mul" ), sr_create( typeof( "test_mul" ) ) );

        sr_s proc_sr = bcore_life_s_push_sr( l, bcore_inst_typed_create_sr( TYPEOF_bclos_procedure_s ) );
        bclos_procedure_s* proc = proc_sr.o;
        proc->sig = bclos_signature_s_parse_from_sc( "s3_t test_operation( const s3_t v1, const s3_t v2, const s3_t v3 )" );
        bclos_procedure_s_push_sc( proc, "s3_t val1 = add( v1, v2 )" );
        bclos_procedure_s_push_sc( proc, "s3_t val2 = add( v1, v3 )" );
        bclos_procedure_s_push_sc( proc, "     val2 = mul( v1, v3 )" );
        bclos_procedure_s_push_sc( proc, "s3_t ret  = mul( val1, val2 )" );
        bclos_procedure_s_push_sc( proc, "return( ret )" );

        bclos_closure_q_def( &proc_sr, env );
        sr_s res = bclos_closure_q_call_na( &proc_sr, NULL, 3, sr_s3( 2 ), sr_s3( 3 ), sr_s3( 4 ) );
        ASSERT( *( s3_t* )res.o == 40 );

        sr_down( res );
    }

    bcore_life_s_discard( l );
    return NULL;
}

/**********************************************************************************************************************/

vd_t bclos_procedure_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_procedure" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_procedure_s" ), procedure_s_create_self );
    }
    else if( signal == typeof( "selftest" ) )
    {
        st_s_print_d( procedure_selftest() );
    }

    return NULL;
}

