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

#include "bclos_procedure.h"
#include "bclos_frame.h"
#include "bclos_quicktypes.h"
#include "bcore_signal.h"
#include "bclos_spect_closure.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_life.h"
#include "bcore_trait.h"

/**********************************************************************************************************************/

BCORE_DEFINE_FUNCTIONS_OBJ_INST( bclos_procedure_s )

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
            bclos_statement_s_create_sc( statement )
        )
    );
}

void bclos_procedure_s_def( bclos_procedure_s* o, bclos_frame_s* frm )
{
    o->lexical = frm;
}

sr_s bclos_procedure_s_call( bclos_procedure_s* o, bclos_frame_s* frm, const bclos_arguments_s* args )
{
    bclos_frame_s* local = bclos_frame_s_create(); // local frame
    local->external = o->lexical ? o->lexical : frm;

    if( o->sig )
    {
        if( o->sig->size > args->size )
        {
            ERR( "Closure %s: %u arguments required; %u supplied", ifnameof( o->sig->name ), o->sig->size, args->size );
        }
        for( sz_t i = 0; i < o->sig->size; i++ )
        {
            bclos_signature_arg_s sig_arg = o->sig->data[ i ];
            sr_s arg_obj = bclos_arguments_s_get( args, i, frm );
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
            bclos_frame_s_set( local, sig_arg.name, arg_obj );
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

    bclos_frame_s_discard( local );

    return ret;
}

sr_s bclos_procedure_s_sig( const bclos_procedure_s* o )
{
    return sr_twc( TYPEOF_bclos_signature_s, o->sig );
}

static bcore_self_s* procedure_s_create_self( void )
{
    sc_t def = "bclos_procedure_s = bclos_closure { aware_t _; bclos_statement_s * [] arr; bclos_signature_s* sig; private bclos_frame_s * lexcal; }";
    bcore_self_s* self = bcore_self_s_build_parse_sc( def, sizeof( bclos_procedure_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bclos_procedure_s_def,  "bclos_closure_fp_def",  "def"  );
    bcore_self_s_push_ns_func( self, ( fp_t )bclos_procedure_s_call, "bclos_closure_fp_call", "call" );
    bcore_self_s_push_ns_func( self, ( fp_t )bclos_procedure_s_sig,  "bclos_closure_fp_sig",  "sig"  );
    return self;
}

/**********************************************************************************************************************/

#include "bcore_txt_ml.h"

static sr_s test_add( vc_t o, bclos_frame_s* frm, const bclos_arguments_s* args )
{
    assert( args->size >= 2 );
    sr_s a0 = bclos_arguments_s_get( args, 0, frm );
    sr_s a1 = bclos_arguments_s_get( args, 1, frm );
    sr_s ret = sr_null();
    assert( sr_s_type( &a0 ) == sr_s_type( &a1 ) );
    tp_t t = sr_s_type( &a0 );
    switch( t )
    {
        case TYPEOF_s2_t: ret = sr_s2( *( s2_t* )a0.o + *( s2_t* )a1.o ); break;
        case TYPEOF_s3_t: ret = sr_s3( *( s3_t* )a0.o + *( s3_t* )a1.o ); break;
        case TYPEOF_u2_t: ret = sr_u2( *( u2_t* )a0.o + *( u2_t* )a1.o ); break;
        case TYPEOF_u3_t: ret = sr_u3( *( u3_t* )a0.o + *( u3_t* )a1.o ); break;
        case TYPEOF_f2_t: ret = sr_f2( *( f2_t* )a0.o + *( f2_t* )a1.o ); break;
        case TYPEOF_f3_t: ret = sr_f3( *( f3_t* )a0.o + *( f3_t* )a1.o ); break;
        default: ERR( "cannot add type '%s'", ifnameof( t ) );
    }
    sr_down( a0 );
    sr_down( a1 );
    return ret;
}

static sr_s test_mul( vc_t o, bclos_frame_s* frm, const bclos_arguments_s* args )
{
    assert( args->size >= 2 );
    sr_s a0 = bclos_arguments_s_get( args, 0, frm );
    sr_s a1 = bclos_arguments_s_get( args, 1, frm );
    sr_s ret = sr_null();
    assert( sr_s_type( &a0 ) == sr_s_type( &a1 ) );
    tp_t t = sr_s_type( &a0 );
    switch( t )
    {
        case TYPEOF_s2_t: ret = sr_s2( *( s2_t* )a0.o * *( s2_t* )a1.o ); break;
        case TYPEOF_s3_t: ret = sr_s3( *( s3_t* )a0.o * *( s3_t* )a1.o ); break;
        case TYPEOF_u2_t: ret = sr_u2( *( u2_t* )a0.o * *( u2_t* )a1.o ); break;
        case TYPEOF_u3_t: ret = sr_u3( *( u3_t* )a0.o * *( u3_t* )a1.o ); break;
        case TYPEOF_f2_t: ret = sr_f2( *( f2_t* )a0.o * *( f2_t* )a1.o ); break;
        case TYPEOF_f3_t: ret = sr_f3( *( f3_t* )a0.o * *( f3_t* )a1.o ); break;
        default: ERR( "cannot multiply type '%s'", ifnameof( t ) );
    }
    sr_down( a0 );
    sr_down( a1 );
    return ret;
}

static st_s* procedure_selftest( void )
{
    bcore_life_s* l = bcore_life_s_create();
    {
        bcore_self_s* self = bcore_self_s_build_parse_sc( "test_add = bclos_closure {}", 0 );
        bcore_self_s_push_ns_func( self, ( fp_t )test_add, "bclos_closure_fp_call", "call" );
        bcore_flect_type_self_d( self );
    }
    {
        bcore_self_s* self = bcore_self_s_build_parse_sc( "test_mul = bclos_closure {}", 0 );
        bcore_self_s_push_ns_func( self, ( fp_t )test_mul, "bclos_closure_fp_call", "call" );
        bcore_flect_type_self_d( self );
    }

    {
        sr_s op_r = bcore_life_s_push_sr( l, bcore_inst_typed_create_sr( typeof( "test_mul" ) ) );
        sr_s res = bclos_closure_q_call_na( &op_r, NULL, 2, sr_s3( 25 ), sr_s3( -36 ) );
        ASSERT( *( s3_t* )res.o == -900 );
        sr_down( res );
    }

    {
        bclos_frame_s* frm = bcore_life_s_push_aware( l, bclos_frame_s_create() );

//        bclos_frame_s_set( frm, typeof( "return" ), sr_create( typeof( "bclos_completion" ) ) );
        bclos_frame_s_set( frm, typeof( "add" ), sr_create( typeof( "test_add" ) ) );
        bclos_frame_s_set( frm, typeof( "mul" ), sr_create( typeof( "test_mul" ) ) );
        bclos_frame_s_set( frm, typeof( "print" ), sr_create( typeof( "bclos_writeln_s" ) ) );

        sr_s proc_sr = bcore_life_s_push_sr( l, bcore_inst_typed_create_sr( TYPEOF_bclos_procedure_s ) );
        bclos_procedure_s* proc = proc_sr.o;
        proc->sig = bclos_signature_s_parse_from_sc( "s3_t test_operation( const s3_t v1, const s3_t v2, const s3_t v3 )" );
        bclos_procedure_s_push_sc( proc, "add( v1, v2 )     -> s3_t val1;" );
        //bclos_procedure_s_push_sc( proc, "add( v1, v3 )     -> s3_t val2;" );
        bclos_procedure_s_push_sc( proc, "mul( v1, v3 )     ->   s3_t val2;" );
//        bclos_procedure_s_push_sc( proc, "mul( add( v1, v2 ), mul( v1, v3 ) ) ->;" );
//        bclos_procedure_s_push_sc( proc, "mul( val1, val2 ) -> s3_t ret;" );
        bclos_procedure_s_push_sc( proc, "v1 -> s3_t ret;" );
        bclos_procedure_s_push_sc( proc, "bclos_assign_s( mul( val1, val2 ), ret );" );
//        bclos_procedure_s_push_sc( proc, "val2 -> ret;" );
//        bclos_procedure_s_push_sc( proc, "val1 -> val2;" );
//        bclos_procedure_s_push_sc( proc, "print( ret );" );
//        bclos_procedure_s_push_sc( proc, "bclos_writeln_s( ret );" );
//        bclos_procedure_s_push_sc( proc, "print( val1 );" );
//        bclos_procedure_s_push_sc( proc, "print( val2 );" );
        bclos_procedure_s_push_sc( proc, "ret ->;" );

        bclos_closure_q_def( &proc_sr, frm );
        sr_s res = bclos_closure_q_call_na( &proc_sr, NULL, 3, sr_s3( 2 ), sr_s3( 3 ), sr_s3( 4 ) );
        ASSERT( *( s3_t* )res.o == 40 );

        sr_down( res );
    }

    bcore_life_s_discard( l );
    return NULL;
}

/**********************************************************************************************************************/

vd_t bclos_procedure_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bclos_procedure" ) ) )
    {
        case TYPEOF_init1:
        {
            bcore_flect_define_creator( typeof( "bclos_procedure_s" ), procedure_s_create_self );
        }
        break;

        case TYPEOF_selftest:
        {
            return procedure_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}

