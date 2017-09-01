/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_spect_closure.h"
#include "bcore_quicktypes.h"
#include "bcore_life.h"
#include "bcore_spect_array.h"
#include "bcore_spect_via.h"
#include "bclos_quicktypes.h"
#include "bclos_env.h"
#include "bclos_proc.h"

/// binding values of 0 indicate no binding
typedef struct bclos_proc_binding_s
{
    tp_t func; // function
    tp_t ret;  // return value;
    union
    {
        bcore_static_array_s args_arr;
        struct
        {
            tp_t* args;
            sz_t size, space;
        };
    };
} bclos_proc_binding_s;

DEFINE_FUNCTION_INIT_SPECT(    bclos_proc_binding_s )
DEFINE_FUNCTION_DOWN_SPECT(    bclos_proc_binding_s )
DEFINE_FUNCTION_COPY_SPECT(    bclos_proc_binding_s )
DEFINE_FUNCTION_CREATE_SPECT(  bclos_proc_binding_s )
DEFINE_FUNCTION_DISCARD_SPECT( bclos_proc_binding_s )
DEFINE_FUNCTION_CLONE_SPECT(   bclos_proc_binding_s )

static bcore_flect_self_s* proc_binding_s_create_self( void )
{
    sc_t def =
    "bclos_proc_binding_s ="
    "{"
        "tp_t func;"
        "tp_t ret;"
        "tp_t [] args;"
    "}";

    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_proc_binding_s ) );
    return self;
}

/**********************************************************************************************************************/

static sc_t bclos_proc_arg_s_def =
"bclos_proc_arg_s = "
"{"
    "tp_t name;"
    "tp_t type;"
    "bl_t w;"
    "bl_t r;"
"}";

typedef struct bclos_proc_arg_s
{
    tp_t name;
    tp_t type;
    bl_t w; // write flag
    bl_t r; // read flag
} bclos_proc_arg_s;

/// procedure closure
static sc_t bclos_proc_s_def =
"bclos_proc_s = "
"{"
    "aware_t _;"
    "private bclos_env_s* external_env;"
    "bclos_proc_arg_s [] args;"
    "tp_t ret_name;"
    "tp_t ret_type;"
    "bclos_proc_binding_s [] bindings;"
"}";

typedef struct bclos_proc_s
{
    aware_t _;
    bclos_env_s* external_env; // not owned by closure

    union
    {
        bcore_static_array_s args_arr;
        struct
        {
            bclos_proc_arg_s* args;
            sz_t args_size, args_space;
        };
    };

    tp_t ret_name;
    tp_t ret_type;

    union
    {
        bcore_static_array_s bindings_arr;
        struct
        {
            bclos_proc_binding_s* bindings;
            sz_t bindings_size, bindings_space;
        };
    };
} bclos_proc_s;

DEFINE_FUNCTION_INIT_SPECT(    bclos_proc_s )
DEFINE_FUNCTION_DOWN_SPECT(    bclos_proc_s )
DEFINE_FUNCTION_COPY_SPECT(    bclos_proc_s )
DEFINE_FUNCTION_CREATE_SPECT(  bclos_proc_s )
DEFINE_FUNCTION_DISCARD_SPECT( bclos_proc_s )
DEFINE_FUNCTION_CLONE_SPECT(   bclos_proc_s )

void bclos_proc_s_push_arg( bclos_proc_s* o, tp_t type, tp_t name, sc_t flags )
{
    sr_s arg = sr_create( typeof( "bclos_proc_arg_s" ) );
    ( ( bclos_proc_arg_s* )arg.o )->type = type;
    ( ( bclos_proc_arg_s* )arg.o )->name = name;
    ( ( bclos_proc_arg_s* )arg.o )->w    = ( flags[ 0 ] == 'w' ) || ( ( flags[ 0 ] != 0 ) && ( flags[ 1 ] == 'w' ) );
    ( ( bclos_proc_arg_s* )arg.o )->r    = ( flags[ 0 ] == 'r' ) || ( ( flags[ 0 ] != 0 ) && ( flags[ 1 ] == 'r' ) );
    bcore_array_push( bcore_via_aware_nget( o, typeof( "args" ) ), arg );
}

void bclos_proc_s_set_ret( bclos_proc_s* o, tp_t type, tp_t name )
{
    o->ret_type = type;
    o->ret_name = name;
}

void bclos_proc_s_push_op( bclos_proc_s* o, tp_t func_name, tp_t ret_name, tp_t* arg_names, sz_t n_args )
{

}

vd_t bclos_proc_s_func( bclos_proc_s* o, const vd_t* args, sz_t n_args )
{
    assert( n_args >= o->bindings_size );

    bcore_life_s* l = bcore_life_s_create();
    bclos_env_s* env = bcore_life_s_push_typed( l, TYPEOF_bclos_env_s, bclos_env_s_create() );
    env->external = o->external_env;
    for( sz_t i = 0; i < o->args_size; i++ )
    {
        const bclos_proc_arg_s* arg = &o->args[ i ];
        sr_s sr = ( arg->w ) ? sr_twd( arg->type, args[ i ] ) : sr_twc( arg->type, args[ i ] );
        bclos_env_s_set( env, arg->name, sr );
    }

    for( sz_t i = 0; i < o->bindings_size; i++ ) // for all functions
    {
        const bclos_proc_binding_s* binding = &o->bindings[ i ];
        sr_s* func = bclos_env_s_get( env, binding->func );
        if( !func ) ERR( "Function '%s' is not defined.", ifnameof( binding->func ) );

        vd_t* f_args = bcore_u_alloc( sizeof( vd_t ), NULL, binding->size, NULL );

        for( sz_t i = 0; i < binding->size; i++ )
        {
            ad_s  arg_ad = bcore_closure_q_d_arg( func, i );
            sr_s* arg_sr = bclos_env_s_get( env, binding->args[ i ] );
            if( !arg_sr )
            {
                ERR( "Function '%s': Argument '%s' is not defined.",
                     ifnameof( binding->func ),
                     ifnameof( binding->args[ i ] ) );
            }
            if( arg_ad.type != sr_s_type( arg_sr ) )
            {
                ERR( "Function '%s', argument '%s':\n"
                     "Type mismatch: passed '%s', expected '%s'.",
                     ifnameof( binding->func ),
                     ifnameof( binding->args[ i ] ),
                     ifnameof( sr_s_type( arg_sr ) ),
                     ifnameof( arg_ad.type ) );
            }
            if( ( arg_ad.flags & AD_F_WR ) && sr_s_is_const( arg_sr ) )
            {
                ERR( "Function '%s', argument '%s':\n"
                     "Function writes to a const argument.",
                     ifnameof( binding->func ),
                     ifnameof( binding->args[ i ] ) );
            }
            f_args[ i ] = arg_sr->o;
        }
        tp_t t_ret = bcore_closure_q_t_ret( func );
        sr_s ret_sr = sr_null();
        vd_t ret_vd = bcore_closure_q_func( func, f_args, binding->size );
        if( t_ret ) ret_sr = sr_tsd( t_ret, ret_vd );
        if( binding->ret )
        {
            if( !ret_sr.p ) ERR( "Function '%s' has no return value.", ifnameof( binding->func ) );
            bclos_env_s_set( env, binding->ret, ret_sr );
        }
        else
        {
            sr_down( ret_sr );
        }

        bcore_un_alloc( sizeof( vd_t ), f_args, binding->size, 0, NULL );
    }

    vd_t ret_vd = NULL;
    if( o->ret_type )
    {
        sr_s* ret_sr = bclos_env_s_get( env, o->ret_name );
        if( !ret_sr ) ERR( "Return variable '%s' is not defined.", ifnameof( o->ret_name ) );
        if( sr_s_type( ret_sr ) != o->ret_type )
        {
            ERR( "Type mismatch of return variable '%s' of type '%s'. Expected type '%s'.",
                 ifnameof( o->ret_name ),
                 ifnameof( sr_s_type( ret_sr ) ),
                 ifnameof( o->ret_type ) );
        }
        if( sr_s_is_strong( ret_sr ) )
        {
            ret_vd = ret_sr->o;
            *ret_sr = sr_null();
        }
        else
        {
            ret_vd = bcore_inst_q_clone( ret_sr );
        }
    }

    bcore_life_s_discard( l );

    return ret_vd;
}

sz_t bclos_proc_s_n_args( const bclos_proc_s* o )
{
    return o->args_size;
}

ad_s bclos_proc_s_d_arg( const bclos_proc_s* o, sz_t index )
{
    assert( index < o->args_size );
    const bclos_proc_arg_s* arg = &o->args[ index ];
    ad_s ad;
    ad.type = arg->type;
    ad.flags = ( arg->w ? AD_F_WR : 0 ) | ( arg->r ? AD_F_RD : 0 );
    return ad;
}

tp_t bclos_proc_s_t_ret( const bclos_proc_s* o )
{
    return o->ret_type;
}

void bclos_proc_s_set_env( bclos_proc_s* o, vd_t env )
{
    if( env )
    {
        assert( *(aware_t*)env == TYPEOF_bclos_env_s );
        o->external_env = env;
    }
}

bcore_flect_self_s* proc_arg_s_create_self( void )
{
    return bcore_flect_self_s_build_parse_sc( bclos_proc_arg_s_def, sizeof( bclos_proc_s ) );
}

bcore_flect_self_s* proc_s_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( bclos_proc_s_def, sizeof( bclos_proc_s ) );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_proc_s_func,    "bcore_closure_fp_func",    "func" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_proc_s_n_args,  "bcore_closure_fp_n_args",  "n_args" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_proc_s_d_arg,   "bcore_closure_fp_d_arg",   "d_arg" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_proc_s_t_ret,   "bcore_closure_fp_t_ret",   "t_ret" );
    bcore_flect_self_s_push_external_func( self, ( fp_t )bclos_proc_s_set_env, "bcore_closure_fp_set_env", "set_env" );
    return self;
}

/**********************************************************************************************************************/

void bclos_proc_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bclos_proc_binding_s" ), proc_binding_s_create_self );
    bcore_flect_define_creator( typeof( "bclos_proc_arg_s"     ), proc_arg_s_create_self );
    bcore_flect_define_creator( typeof( "bclos_proc_s"         ), proc_s_create_self );
}

/**********************************************************************************************************************/

#include "bcore_txt_ml.h"

bcore_string_s* bclos_proc_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    bclos_env_s* env = bcore_life_s_push_aware( l, bclos_env_s_create() );

    bclos_env_s_set( env, entypeof( "*" ), sr_create( typeof( "bclos_f3_mul_f3_f3" ) ) );
    bclos_env_s_set( env, entypeof( "+" ), sr_create( typeof( "bclos_f3_add_f3_f3" ) ) );
    bclos_env_s_set( env, entypeof( "-" ), sr_create( typeof( "bclos_f3_sub_f3_f3" ) ) );
    bclos_env_s_set( env, entypeof( "/" ), sr_create( typeof( "bclos_f3_div_f3_f3" ) ) );

    bcore_txt_ml_to_stdout( sr_awc( env ) );

    bcore_life_s_discard( l );
    return NULL;
}
