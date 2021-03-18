/** Author and Copyright 2019 Johannes Bernhard Steffens
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

#include "bcore_main.h"
#include "bcore_std.h"

#include <signal.h>

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/// This variable is to be polled to obtained the latest signal state
static bcore_main_frame_s* bcore_main_frame_g = NULL;
static void (*bcore_default_sigint )( int signal ) = NULL;
static void (*bcore_default_sigtstp)( int signal ) = NULL;
static void (*bcore_default_sigterm)( int signal ) = NULL;

static void signal_callback( int signal_received )
{
    /** due to restrictions and inconsistent implementations
     *  of signal processing, we do the absolute minimum that is
     *  deemed supported on all platforms.
     */

    er_t er = 0;
    switch( signal_received )
    {
        case SIGINT:
        {
            bl_t handled = false;
            if( bcore_main_frame_g && bcore_main_frame_g->object_sr.o )
            {
                if     ( ( handled = bcore_main_r_on_interrupt(   &bcore_main_frame_g->object_sr, bcore_main_frame_g ) ) ) {}
                else if( ( handled = bcore_main_r_on_termination( &bcore_main_frame_g->object_sr, bcore_main_frame_g ) ) ) {}
            }
            if( handled )
            {
                signal( SIGINT, signal_callback );
            }
            else
            {
                if( bcore_default_sigint ) bcore_default_sigint( signal_received );
            }
        }
        break;

        case SIGTSTP:
        {
            bl_t handled = false;
            if( bcore_main_frame_g && bcore_main_frame_g->object_sr.o )
            {
                handled = bcore_main_r_on_suspend( &bcore_main_frame_g->object_sr, bcore_main_frame_g );
            }

            if( handled )
            {
                signal( SIGTSTP, signal_callback );
            }
            else
            {
                if( bcore_default_sigtstp ) bcore_default_sigtstp( signal_received );
            }
        }
        break;

        case SIGTERM:
        {
            bl_t handled = false;
            if( bcore_main_frame_g && bcore_main_frame_g->object_sr.o )
            {
                handled = bcore_main_r_on_termination( &bcore_main_frame_g->object_sr, bcore_main_frame_g );
            }
            if( !handled )
            {
                if( bcore_default_sigterm ) bcore_default_sigterm( signal_received );
            }
        }
        break;

        default: break;
    }

    if( er ) bcore_error_pop_to_stderr();
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_main_frame_s_exec( bcore_main_frame_s* o, const bcore_arr_st_s* args )
{
    if( bcore_main_frame_g ) ERR_fa( "bcore_main_frame_s_exec can only be executed once at a time." );
    bcore_main_frame_g = o;

    bcore_arr_st_s_copy( &o->args, args );
    er_t error = 0;
    sc_t file = NULL;
    if( o->first_argument_is_path_to_object && o->args.size > 1 )
    {
        file = o->args.data[ 1 ]->sc;
        if( !bcore_file_exists( file ) ) file = NULL;
    }

    if( !file && o->local_file )
    {
        file = o->local_file;
        if( !bcore_file_exists( file ) ) file = NULL;
    }

    if( !file && o->global_file )
    {
        file = o->global_file;
        if( !bcore_file_exists( file ) ) file = NULL;
    }

    if( file )
    {
        /// redirect signals
        bcore_default_sigint = signal( SIGINT , signal_callback );
        bcore_default_sigterm = signal( SIGTERM, signal_callback );
        bcore_default_sigtstp = signal( SIGTSTP, signal_callback );

        bcore_source* source = bcore_file_open_source( file );

        bcore_interpreter* interpreter = NULL;
        if( o->interpreter )
        {
            interpreter = bcore_fork( o->interpreter );
        }
        else if( bcore_txt_ml_contains_valid_syntax( source ) )
        {
            interpreter = ( bcore_interpreter* )bcore_txt_ml_interpreter_s_create();
        }
        else
        {
            interpreter = ( bcore_interpreter* )bcore_bin_ml_interpreter_s_create();
        }

        o->object_sr = bcore_interpret_x( sr_awd( interpreter ), sr_awd( source ) );
        bcore_inst_a_detach( (bcore_inst** )&interpreter );
        bcore_inst_a_detach( (bcore_inst** )&source );

        if( o->args.size > 2 && o->second_argument_is_path_to_script && bcore_file_exists( o->args.data[ 2 ]->sc ) )
        {
            bcore_source_a_attach( &o->source, bcore_file_open_source( o->args.data[ 2 ]->sc ) );
        }
        else
        {
            bcore_source_a_attach( &o->source, bcore_fork( BCORE_STDIN ) );
        }

        bcore_sink_a_attach( &o->sink, bcore_fork( BCORE_STDOUT ) );

        if( bcore_main_r_defines_main( &o->object_sr ) )
        {
            error = bcore_main_r_main( &o->object_sr, o );
        }
        else
        {
            bcore_error_push_fa
            (
                TYPEOF_general_error, "bcore_main_frame_s: Object '#<sc_t>' does not define callback feature bcore_main.main.",
                bnameof( sr_s_o_type( &o->object_sr ) )
            );
        }

        sr_down( o->object_sr );
        o->object_sr = sr_null();

        signal( SIGINT , bcore_default_sigint );
        signal( SIGTERM, bcore_default_sigterm );
        signal( SIGTSTP, bcore_default_sigtstp );
    }
    else
    {
        bcore_error_push_fa( TYPEOF_general_error, "bcore_main_frame_s: Could not associate a file from arguments or frame configuration." );
        error = TYPEOF_general_error;
    }

    bcore_main_frame_g = NULL;
    return error;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_main_frame_s_main( bcore_main_frame_s* o, sz_t argc, const char** argv )
{
    bcore_arr_st_s* args = bcore_arr_st_s_create();
    for( sz_t i = 0; i < argc; i++ ) bcore_arr_st_s_push_sc( args, argv[ i ] );

    er_t retv = bcore_main_frame_s_exec( o, args );
    bcore_arr_st_s_detach( &args );
    return retv;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

