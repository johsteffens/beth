/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bclos_statement.h"
#include "bclos_quicktypes.h"
#include "bclos_spect_closure.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_spect_source.h"
#include "bcore_life.h"
#include "bcore_trait.h"
#include "bclos_quicktypes.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_FLAT(  bclos_statement_s )
DEFINE_FUNCTION_DOWN_SPECT( bclos_statement_s )
DEFINE_FUNCTION_COPY_SPECT( bclos_statement_s )
DEFINE_CDC_FUNCTIONS(       bclos_statement_s )

void bclos_statement_s_parse_from_source( bclos_statement_s* o, sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    source = bcore_life_s_push_sr( l, source );

    bclos_expression_s_parse_from_source( &o->expression, source );
    if( bcore_source_q_parse_bool_f( &source, " #?';'" ) )
    {
        o->def_type = 0;
        o->target.name = 0;
    }
    else if( bcore_source_q_parse_bool_f( &source, " #?'->'" ) )
    {
        if( bcore_source_q_parse_bool_f( &source, " #?';'" ) )
        {
            o->def_type = 0;
            o->target.name = TYPEOF_bclos_return;
        }
        else
        {
            st_s* identifier = st_s_create_l( l );
            bcore_source_q_parse_fa( &source, " #name", identifier );
            if( bcore_source_q_parse_bool_f( &source, " #?';'" ) )
            {
                o->def_type = 0;
                o->target.name = entypeof( identifier->sc );
            }
            else
            {
                o->def_type = entypeof( identifier->sc );
                bcore_source_q_parse_fa( &source, " #name ;", identifier );
                o->target.name = entypeof( identifier->sc );
            }
        }
    }
    else
    {
        bcore_source_q_parse_errf( &source, "Statement syntax error." );
    }

    bcore_life_s_discard( l );
}

sr_s bclos_statement_s_run( const bclos_statement_s* o, bclos_frame_s* frm )
{
    sr_s expr_result = bclos_expression_s_run( &o->expression, frm );
    if( o->target.name == TYPEOF_bclos_return ) return sr_fork( expr_result );

    if( o->target.name )
    {
        if( o->def_type )
        {
            if( !bcore_trait_satisfied_type( o->def_type, sr_s_type( &expr_result ), NULL ) )
            {
                ERR
                (
                    "Expression returned '%s' but target '%s' is of type '%s'.",
                    ifnameof( sr_s_type( &expr_result ) ),
                    ifnameof( o->target.name ),
                    ifnameof( o->def_type )
                );
            }
            bclos_frame_s_set( frm, o->target.name, expr_result );
        }
        else
        {
            sr_s* target_sr = bclos_frame_s_get( frm, o->target.name );
            if( !target_sr )  ERR( "Target '%s' not defined.", ifnameof( o->target.name ) );
            sr_s_set( target_sr, expr_result );
        }
    }
    else
    {
        sr_down( expr_result );
    }
    return sr_null();
}

bclos_statement_s* bclos_statement_s_create_sc( sc_t sc )
{
    bclos_statement_s* s = bclos_statement_s_create();
    bclos_statement_s_parse_from_source( s, sr_asd( st_s_create_weak_sc( sc ) ) );
    return s;
}

static bcore_flect_self_s* statement_s_create_self( void )
{
//    sc_t def = "bclos_statement_s = { tp_t def_type; bclos_address_s target; tp_t operation; tp_t [] args_arr; }";
    sc_t def = "bclos_statement_s = {  bclos_expression_s expression; tp_t def_type; bclos_address_s target; }";

    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_statement_s ) );
    return self;
}

/**********************************************************************************************************************/

vd_t bclos_statement_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_statement" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_statement_s" ), statement_s_create_self );
    }

    return NULL;
}

/**********************************************************************************************************************/

