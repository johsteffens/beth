/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bclos_statement.h"
#include "bclos_quicktypes.h"
#include "bclos_spect_closure.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_spect_source.h"
#include "bcore_life.h"

DEFINE_FUNCTION_INIT_FLAT(  bclos_statement_s )
DEFINE_FUNCTION_DOWN_SPECT( bclos_statement_s )
DEFINE_FUNCTION_COPY_SPECT( bclos_statement_s )
DEFINE_FUNCTION_CREATE(     bclos_statement_s )
DEFINE_FUNCTION_DISCARD(    bclos_statement_s )
DEFINE_FUNCTION_CLONE(      bclos_statement_s )

void bclos_statement_s_push_arg( bclos_statement_s* o, tp_t arg )
{
    if( o->args_size > o->args_space ) bcore_array_typed_make_strong( TYPEOF_bclos_statement_s, o );
    if( o->args_size == o->args_space )
    {
        o->args_data = bcore_un_alloc
        (
            sizeof( sr_s ),
            o->args_data,
            o->args_space,
            o->args_space > 0 ? o->args_space * 2 : 1,
            &o->args_space
        );
    }
    o->args_data[ o->args_size++ ] = arg;
}

bclos_args_s* bclos_statement_s_create_args( const bclos_statement_s* o, bclos_env_s* env )
{
    bclos_args_s* args = bclos_args_s_create();
    for( sz_t i = 0; i < o->args_size; i++ )
    {
        tp_t name = o->args_data[ i ];
        sr_s* sr = bclos_env_s_get( env, name );
        if( !sr ) ERR( "'%s' not found in environment", ifnameof( name ) );
        bclos_args_s_push( args, sr_cw( *sr ) );
    }
    return args;
}

void bclos_statement_s_run( const bclos_statement_s* o, bclos_env_s* env )
{
    if( o->operation )
    {
        sr_s* operation = bclos_env_s_get( env, o->operation );
        if( !operation )  ERR( "Operation '%s' not defined.", ifnameof( o->operation ) );

        const bclos_closure_s* closure_p = ch_spect( operation->p, TYPEOF_bclos_closure_s );

        {
            bclos_args_s* args = bclos_statement_s_create_args( o, env );
            sr_s ret_sr = bclos_closure_spect_call( closure_p, operation->o, env, args );

            if( o->target )
            {
                if( o->define )
                {
                    if( o->define != sr_s_type( &ret_sr ) )
                    {
                        ERR
                        (
                            "Closure '%s' returned '%s' but target '%s' is of type '%s'.",
                            ifnameof( sr_s_type( operation ) ),
                            ifnameof( sr_s_type( &ret_sr ) ),
                            ifnameof( o->target ),
                            ifnameof( o->define )
                        );
                    }
                    bclos_env_s_set( env, o->target, ret_sr );
                }
                else
                {
                    sr_s* target_sr = bclos_env_s_get( env, o->target );
                    if( !target_sr )  ERR( "Target '%s' not defined.", ifnameof( o->target ) );
                    if( sr_s_type( target_sr ) != sr_s_type( &ret_sr ) )
                    {
                        ERR
                        (
                            "Closure '%s' returned '%s' but target '%s' is of type '%s'.",
                            ifnameof( sr_s_type( operation ) ),
                            ifnameof( sr_s_type( &ret_sr ) ),
                            ifnameof( o->target ),
                            ifnameof( sr_s_type( target_sr ) )
                        );
                    }
                    sr_s_set( target_sr, ret_sr );
                }
            }
            else
            {
                sr_down( ret_sr );
            }
            bclos_args_s_discard( args );
        }
    }
}

bclos_statement_s* bclos_statement_s_parse_from_source( sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    source = bcore_life_s_push_sr( l, source );
    bclos_statement_s* s = bclos_statement_s_create();

    bcore_string_s* name = bcore_string_s_create_l( l );
    bcore_source_q_parsef( &source, " #name", name );
    if( bcore_source_q_parse_boolf( &source, " #?'='" ) )
    {
        s->define = 0;
        s->target = bcore_name_enroll( name->sc );
    }
    else
    {
        s->define = bcore_name_enroll( name->sc );
        bcore_source_q_parsef( &source, " #name =", name );
        s->target = bcore_name_enroll( name->sc );
    }

    bcore_source_q_parsef( &source, " #name ( ", name );
    s->operation = bcore_name_enroll( name->sc );
    while( !bcore_source_q_parse_boolf( &source, " #?')'" ) )
    {
        if( s->args_size > 0 ) bcore_source_q_parsef( &source, " ," );
        bcore_source_q_parsef( &source, " #name ", name );
        bclos_statement_s_push_arg( s, bcore_name_enroll( name->sc ) );
    }

    bcore_life_s_discard( l );
    return s;
}

bclos_statement_s* bclos_statement_s_parse_from_sc( sc_t sc )
{
    return bclos_statement_s_parse_from_source( sr_asd( bcore_string_s_create_weak_sc( sc ) ) );
}

static bcore_flect_self_s* statement_s_create_self( void )
{
    sc_t def = "bclos_statement_s = { tp_t define; tp_t target; tp_t operation; tp_t [] args_arr; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_statement_s ) );
    return self;
}

void bclos_statement_define_self_creators( void )
{
    bcore_flect_define_creator( typeof( "bclos_statement_s" ), statement_s_create_self );
}

vd_t bclos_statement_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_statement" ) ) return NULL;

    if( signal == typeof( "init" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_statement_s" ), statement_s_create_self );
    }

    return NULL;
}

/**********************************************************************************************************************/

