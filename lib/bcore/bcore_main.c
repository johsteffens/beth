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

//----------------------------------------------------------------------------------------------------------------------

/// This variable is to be polled to obtained the latest signal state
static volatile sig_atomic_t signal_received_g = 0;

static void signal_callabck( int signal_received )
{
    /** due to restrictions and inconsistent implementations
     *  of signal processing, we do the absolute minimum that is
     *  deemed supported on all platforms.
     */
    signal_received_g = signal_received;
}

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_main_frame_s_exit_required( const bcore_main_frame_s* o )
{
    if( !o ) return false;
    /// lock-unlock to trigger memory fence
    bcore_mutex_s_lock( ( bcore_mutex_s* )&o->mutex );
    bl_t exit_required = ( signal_received_g == SIGINT ) || ( signal_received_g == SIGTERM );
    bcore_mutex_s_unlock( ( bcore_mutex_s* )&o->mutex );
    return exit_required;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_main_frame_s_main( bcore_main_frame_s* o, sz_t argc, char** argv )
{
    bcore_arr_st_s_clear( &o->args );
    for( sz_t i = 0; i < argc; i++ ) bcore_arr_st_s_push_sc( &o->args, argv[ i ] );
    er_t error = 0;
    sc_t file = NULL;
    if( o->use_first_argument && o->args.size > 1 )
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
        signal_received_g = 0;
        signal( SIGINT , signal_callabck );
        signal( SIGTERM, signal_callabck );

        sr_s sr_object = bcore_interpret_x( sr_awd( o->interpreter ), sr_asd( bcore_file_open_source( file ) ) );
        error = bcore_main_r_main( &sr_object, o );
        sr_down( sr_object );

        signal( SIGINT , SIG_DFL );
        signal( SIGTERM, SIG_DFL );
    }
    else
    {
        bcore_error_push_fa( TYPEOF_general_error, "bcore_main_frame_s: Could not associate a file from arguments or frame configuration." );
        error = TYPEOF_general_error;
    }

    return error;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

