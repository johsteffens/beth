/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bclos_statement.h"
#include "bclos_quicktypes.h"
#include "bclos_spect_closure.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_spect_source.h"
#include "bcore_life.h"
#include "bcore_trait.h"

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

bclos_arguments_s* bclos_statement_s_create_args( const bclos_statement_s* o, bclos_frame_s* frm )
{
    bclos_arguments_s* args = bclos_arguments_s_create();
    for( sz_t i = 0; i < o->args_size; i++ )
    {
        tp_t name = o->args_data[ i ];
        sr_s* sr = bclos_frame_s_get( frm, name );
        if( !sr ) ERR( "'%s' not found in frame", ifnameof( name ) );
        bclos_arguments_s_push( args, sr_cw( *sr ) );
    }
    return args;
}

sr_s bclos_statement_s_run( const bclos_statement_s* o, bclos_frame_s* frm )
{
    sr_s ret = sr_null();
    if( o->operation )
    {
        sr_s* operation = bclos_frame_s_get( frm, o->operation );
        if( !operation )  ERR( "Operation '%s' not defined.", ifnameof( o->operation ) );

        const bclos_closure_s* closure_p = ch_spect_p( operation->p, TYPEOF_bclos_closure_s );

        {
            bclos_arguments_s* args = bclos_statement_s_create_args( o, frm );
            sr_s ret_sr = bclos_closure_spect_call( closure_p, operation->o, frm, args );

            if( o->target )
            {
                if( o->define )
                {
                    if( !bcore_trait_satisfied_type( o->define, sr_s_type( &ret_sr ), NULL ) )
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
                    bclos_frame_s_set( frm, o->target, ret_sr );
                }
                else
                {
                    sr_s* target_sr = bclos_frame_s_get( frm, o->target );
                    if( !target_sr )  ERR( "Target '%s' not defined.", ifnameof( o->target ) );
                    sr_s_set( target_sr, ret_sr );
                }
            }
            else if( closure_p->o_type == typeof( "bclos_completion" ) )
            {
                ret = sr_fork( ret_sr );
            }
            else
            {
                sr_down( ret_sr );
            }
            bclos_arguments_s_discard( args );
        }
    }
    return ret;
}

bclos_statement_s* bclos_statement_s_parse_from_source( sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    source = bcore_life_s_push_sr( l, source );
    bclos_statement_s* s = bclos_statement_s_create();

    st_s* name = st_s_create_l( l );
    bcore_source_q_parse_fa( &source, " #name", name );
    if( bcore_source_q_parse_bool_f( &source, " #?'='" ) )
    {
        s->target = bcore_name_enroll( name->sc );
        bcore_source_q_parse_fa( &source, " #name", name );
        if( bcore_source_q_parse_bool_f( &source, " #?'('" ) )
        {
            s->operation = bcore_name_enroll( name->sc );
            while( !bcore_source_q_parse_bool_f( &source, " #?')'" ) )
            {
                if( s->args_size > 0 ) bcore_source_q_parse_fa( &source, " ," );
                bcore_source_q_parse_fa( &source, " #name ", name );
                bclos_statement_s_push_arg( s, bcore_name_enroll( name->sc ) );
            }
        }
        else
        {
            bcore_source_q_parse_fa( &source, " #name ", name );
            bclos_statement_s_push_arg( s, bcore_name_enroll( name->sc ) );
        }
    }
    else if( bcore_source_q_parse_bool_f( &source, " #?'('" ) )
    {
        s->operation = bcore_name_enroll( name->sc );
        while( !bcore_source_q_parse_bool_f( &source, " #?')'" ) )
        {
            if( s->args_size > 0 ) bcore_source_q_parse_fa( &source, " ," );
            bcore_source_q_parse_fa( &source, " #name ", name );
            bclos_statement_s_push_arg( s, bcore_name_enroll( name->sc ) );
        }
    }
    else
    {
        s->define = bcore_name_enroll( name->sc );
        bcore_source_q_parse_fa( &source, " #name =", name );
        s->target = bcore_name_enroll( name->sc );
        bcore_source_q_parse_fa( &source, " #name", name );
        if( bcore_source_q_parse_bool_f( &source, " #?'('" ) )
        {
            s->operation = bcore_name_enroll( name->sc );
            while( !bcore_source_q_parse_bool_f( &source, " #?')'" ) )
            {
                if( s->args_size > 0 ) bcore_source_q_parse_fa( &source, " ," );
                bcore_source_q_parse_fa( &source, " #name ", name );
                bclos_statement_s_push_arg( s, bcore_name_enroll( name->sc ) );
            }
        }
        else
        {
            bcore_source_q_parse_fa( &source, " #name ", name );
            bclos_statement_s_push_arg( s, bcore_name_enroll( name->sc ) );
        }
    }

    bcore_life_s_discard( l );
    return s;
}

bclos_statement_s* bclos_statement_s_parse_from_sc( sc_t sc )
{
    return bclos_statement_s_parse_from_source( sr_asd( st_s_create_weak_sc( sc ) ) );
}

static bcore_flect_self_s* statement_s_create_self( void )
{
    sc_t def = "bclos_statement_s = { tp_t define; tp_t target; tp_t operation; tp_t [] args_arr; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_statement_s ) );
    return self;
}

/// function of the return closure
static sr_s return_func( vc_t o, bclos_frame_s* frm, const bclos_arguments_s* args )
{
    ASSERT( args->size >= 1 );
    return args->data[ 0 ];
}

// completion closure (takes one argument which is deemed to be the return value)
// Statement returns the return of a completion (causing the procedure to return)
static bcore_flect_self_s* return_create_self( void )
{
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( "bclos_completion = bclos_closure_s {}", 0 );
    bcore_flect_self_s_push_external_func( self, ( fp_t )return_func, "bclos_closure_fp_call", "call" );
    return self;
}

/**********************************************************************************************************************/

vd_t bclos_statement_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_statement" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_statement_s" ), statement_s_create_self );
        bcore_flect_define_creator( typeof( "bclos_completion"  ), return_create_self );
    }

    return NULL;
}

/**********************************************************************************************************************/

