/** Copyright 2017 Johannes Bernhard Steffens
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

#include "bclos_arguments.h"
#include "bclos_quicktypes.h"
#include "bclos_spect_closure.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_spect_source.h"
#include "bcore_life.h"
#include "bcore_trait.h"

/**********************************************************************************************************************/

DEFINE_FUNCTIONS_OBJ_INST( bclos_arguments_s )
DEFINE_CREATE_SELF( bclos_arguments_s, "bclos_arguments_s = bcore_inst { sr_s [] arr; }" )

void bclos_arguments_s_clear( bclos_arguments_s* o )
{
    bcore_array_typed_set_size( TYPEOF_bclos_arguments_s, o, 0 );
}

bclos_arguments_s* bclos_arguments_s_create_nv( sz_t n, va_list v_args )
{
    bclos_arguments_s* args = bclos_arguments_s_create();
    for( sz_t i = 0; i < n; i++ )
    {
        bclos_arguments_s_push( args, va_arg( v_args, sr_s ) );
    }
    return args;
}

bclos_arguments_s* bclos_arguments_s_create_na( sz_t n, ... )
{
    va_list args;
    va_start( args, n );
    bclos_arguments_s* ret = bclos_arguments_s_create_nv( n, args );
    va_end( args );
    return ret;
}

void bclos_arguments_s_push( bclos_arguments_s* o, sr_s sr )
{
    if( o->size > o->space ) bcore_array_typed_make_strong( TYPEOF_bclos_arguments_s, o );
    if( o->size == o->space )
    {
        o->data = bcore_un_alloc
        (
            sizeof( sr_s ),
            o->data,
            o->space,
            o->space > 0 ? o->space * 2 : 1,
            &o->space
        );
    }
    o->data[ o->size++ ] = sr;
}

bclos_arguments_s bclos_arguments_s_weak_crop( const bclos_arguments_s* src, sz_t start, sz_t end )
{
    bclos_arguments_s args;
    bclos_arguments_s_init( &args );
    if( end <= start || start >= src->size ) return args;
    args.data = src->data + start;
    args.size = ( end <= src->size ? end : src->size ) - start;
    return args;
}

// Format: ( <expression>, <expression>, ... )
void bclos_arguments_s_parse_from_source( bclos_arguments_s* o, sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    source = bcore_life_s_push_sr( l, source );
    bclos_arguments_s_clear( o );
    bcore_source_q_parse_fa( &source, " ( " );
    while( !bcore_source_q_parse_bl_fa( &source, " #?')'" ) )
    {
        if( o->size > 0 ) bcore_source_q_parse_fa( &source, " ," );
        bclos_expression_s* expr = bclos_expression_s_create();
        bclos_expression_s_parse_from_source( expr, source );
        if( sr_s_type( &expr->closure ) == TYPEOF_bclos_identity_s )
        {
            bclos_arguments_s_push( o, sr_s_fork( &expr->args.data[ 0 ] ) );
            bclos_expression_s_discard( expr );
        }
        else
        {
            bclos_arguments_s_push( o, sr_tsd( TYPEOF_bclos_expression_s, expr ) );
        }
    }
    bcore_life_s_discard( l );
}

/**********************************************************************************************************************/

DEFINE_FUNCTIONS_OBJ_INST( bclos_expression_s )
DEFINE_CREATE_SELF( bclos_expression_s, "bclos_expression_s = { sr_s closure; bclos_arguments_s args; }" )

void bclos_expression_s_clear( bclos_expression_s* o )
{
    sr_down( o->closure );
    o->closure = sr_null();
    bcore_array_typed_set_size( TYPEOF_bclos_arguments_s, &o->args, 0 );
}

void bclos_expression_s_parse_from_source( bclos_expression_s* o, sr_s source )
{
    bcore_life_s* l = bcore_life_s_create();
    source = bcore_life_s_push_sr( l, source );
    bclos_expression_s_clear( o );

    if( bcore_source_q_parse_bl_fa( &source, " #?'('" ) )
    {
        bclos_expression_s_parse_from_source( o, source );
        bcore_source_q_parse_fa( &source, " )" );
    }
    else
    {
        st_s* identifier = st_s_create_l( l );
        bcore_source_q_parse_fa( &source, " #name", identifier );
        if( identifier->size == 0 ) bcore_source_q_parse_errf( &source, "Identifier expected." );
        tp_t identifier_tp = entypeof( identifier->sc );
        sr_s closure = sr_null();

        const bcore_flect_self_s* closure_self = bcore_flect_try_self( identifier_tp );

        if( closure_self && closure_self->trait == typeof( "bclos_language_closure" ) )
        {
            closure = bcore_inst_typed_create_sr( identifier_tp );
        }
        else
        {
            closure = sr_tsd( TYPEOF_bclos_address_s, bclos_address_s_create_tp( identifier_tp ) );
        }

        if( bcore_source_q_parse_bl_fa( &source, " #=?'('" ) )
        {
            o->closure = closure;
            bclos_arguments_s_parse_from_source( &o->args, source );
        }
        else
        {
            o->closure = sr_cp( bcore_inst_typed_create_sr( TYPEOF_bclos_identity_s ), TYPEOF_bclos_closure_s );
            bclos_arguments_s_push( &o->args, closure );
        }
    }

    /// expression so far is closure; true arguments are yet to come
    if( bcore_source_q_parse_bl_fa( &source, " #=?'('" ) )
    {
        sr_s closure = sr_tsd( TYPEOF_bclos_expression_s, bclos_expression_s_clone( o ) );
        bclos_expression_s_clear( o );
        o->closure = closure;
        bclos_arguments_s_parse_from_source( &o->args, source );
    }

    bcore_life_s_discard( l );
}

sr_s bclos_expression_s_run( const bclos_expression_s* o, bclos_frame_s* frm )
{
    sr_s bclos_closure_q_call( const sr_s* o, bclos_frame_s* frm, const bclos_arguments_s* args );

    const sr_s* closure = &o->closure;
    sr_s ret;

    switch( sr_s_type( closure ) )
    {
        case TYPEOF_bclos_address_s:
        {
            ret = bclos_closure_q_call( bclos_frame_s_get( frm, ( ( const bclos_address_s* )closure->o )->name ), frm, &o->args );
        }
        break;

        case TYPEOF_bclos_expression_s:
        {
            sr_s s = bclos_expression_s_run( closure->o, frm );
            ret = bclos_closure_q_call( &s, frm, &o->args );
            sr_down( s );
        }
        break;

        default:
        {
            ret = bclos_closure_q_call( closure, frm, &o->args );
        }
        break;
    }
    return ret;
}

/**********************************************************************************************************************/

vd_t bclos_arguments_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_arguments" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_arguments_s"  ), bclos_arguments_s_create_self );
        bcore_flect_define_creator( typeof( "bclos_expression_s" ), bclos_expression_s_create_self );
    }

    return NULL;
}

/**********************************************************************************************************************/

