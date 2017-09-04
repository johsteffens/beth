/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bclos_procedure.h"
#include "bclos_statement.h"
#include "bclos_env.h"
#include "bclos_quicktypes.h"
#include "bclos_sig.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"

/**********************************************************************************************************************/

typedef struct bclos_procedure_s
{
    union
    {
        bcore_static_link_array_s arr;
        struct
        {
            bclos_statement_s** data;
            sz_t size, space;
        };
    };

    bclos_sig_s* sig;
    bclos_env_s* lexical; // (external) lexical environment
} bclos_procedure_s;

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

void bclos_procedure_s_def( bclos_procedure_s* o, bclos_env_s* env )
{
    o->lexical = env;
}

sr_s bclos_procedure_s_call( bclos_procedure_s* o, bclos_env_s* env, const bclos_args_s* args )
{
    bclos_env_s* local = bclos_env_s_create(); // local environment
    local->external = o->lexical ? o->lexical : env;

    if( o->sig )
    {
        if( o->sig->size > args->size )
        {
            ERR( "Closure %s: %u arguments required; %u supplied", ifnameof( o->sig->name ), o->sig->size, args->size );
        }
        for( sz_t i = 0; i < o->sig->size; i++ )
        {
            bclos_arg_sig_s arg_sig = o->sig->data[ i ];
            sr_s arg_obj = sr_cw( args->data[ i ] );
            if( sr_s_is_const( &arg_obj ) && !arg_sig.is_const )
            {
                ERR( "Closure %s: Argument %u (%s %s): const violation.",
                    ifnameof( o->sig->name ),
                    i,
                    ifnameof( arg_sig.type ),
                    ifnameof( arg_sig.name ) );
            }
            if( ( arg_sig.type > 0 ) && ( arg_obj.p != NULL ) && arg_sig.type != sr_s_type( &arg_obj ) )
            {
                ERR( "Closure %s: Argument %u (%s %s): Incorrect type (%s) received.",
                    ifnameof( o->sig->name ),
                    i,
                    ifnameof( arg_sig.type ),
                    ifnameof( arg_sig.name ),
                    ifnameof( sr_s_type( &arg_obj ) ) );
            }
            bclos_env_s_set( local, arg_sig.name, arg_obj );
        }
    }

    for( sz_t i = 0; i < o->size; i++ )
    {
        const bclos_statement_s* statement = o->data[ i ];
        bclos_statement_s_run( statement, local );
    }

    sr_s ret = sr_null();

    if( o->sig->ret )
    {
        if( !bclos_env_s_get( local, o->sig->name ) )
        {
            ERR( "Closure %s: Return object not defined in body", ifnameof( o->sig->name ) );
        }

        ret = bclos_env_s_remove( local, o->sig->name );
        if( ( ret.p != NULL ) && o->sig->ret != sr_s_type( &ret ) )
        {
            ERR( "Closure %s: Return type mismatch (%s required; %s generated).",
                ifnameof( o->sig->name ),
                ifnameof( o->sig->ret ),
                ifnameof( sr_s_type( &ret ) ) );
        }
    }

    bclos_env_s_discard( local );

    return ret;
}

sr_s bclos_procedure_s_sig( const bclos_procedure_s* o )
{
    return sr_twc( TYPEOF_bclos_sig_s, o->sig );
}

static bcore_flect_self_s* procedure_s_create_self( void )
{
    sc_t def = "bclos_procedure_s = { bclos_statement_s * [] arr; bclos_sig_s* sig; private bclos_env_s * lexcal; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_procedure_s ) );
    return self;
}

void bclos_procedure_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bclos_procedure_s" ), procedure_s_create_self );
}

