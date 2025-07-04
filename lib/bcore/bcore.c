/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_control.h"
#include "bcore_threads.h"
#include "bcore_tbman.h"
#include "bcore_name_manager.h"
#include "bcore_error_manager.h"
#include "bcore_signal.h"
#include "bcore_arr.h"
#include "bcore.h"

#include <locale.h>

/**********************************************************************************************************************/

static bcore_arr_fp_s* signal_handler_arr_fp_g = NULL;
static bcore_mutex_s mutex;

//----------------------------------------------------------------------------------------------------------------------

static void init_once()
{
    /** System wide settings
     *  Locale settings can afffect printf and scanf using a decimal point vs a decimal comma.
     *  Within beth a decimal point is preferred at all times.
     *
     *  We should enforce it via setlocale below because scanf is used to scan floating point
     *  data from text sources. E.g. via btml, btcl etc. Variable treatment can make parameter
     *  files unreadable across platforms.
     */
    setlocale( LC_ALL, "C" );

    /// bcore-library initialization
    bcore_signal_s signal_init0 = bcore_signal_init( TYPEOF_all, TYPEOF_init0, NULL );
    bcore_signal_s signal_init1 = bcore_signal_init( TYPEOF_all, TYPEOF_init1, NULL );

    bcore_general_signal_handler( &signal_init0 );
    bcore_general_signal_handler( &signal_init1 );

    bcore_mutex_s_init( &mutex );
    signal_handler_arr_fp_g = bcore_arr_fp_s_create();
    bcore_arr_fp_s_push( signal_handler_arr_fp_g, ( fp_t )bcore_general_signal_handler );
}


//----------------------------------------------------------------------------------------------------------------------

static void init()
{
    static bcore_once_s flag = bcore_once_init;
    bcore_once_s_run( &flag, init_once );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_register_signal_handler( bcore_fp_signal_handler signal_handler )
{
    init();
    if( signal_handler )
    {
        bcore_signal_s signal_init0 = bcore_signal_init( TYPEOF_all, TYPEOF_init0, NULL );
        bcore_signal_s signal_init1 = bcore_signal_init( TYPEOF_all, TYPEOF_init1, NULL );
        bcore_mutex_s_lock( &mutex );
        bl_t not_registered =
            bcore_arr_fp_s_find( signal_handler_arr_fp_g, 0, -1, ( fp_t )signal_handler )
                == signal_handler_arr_fp_g->size;

        if( not_registered )
        {
            signal_handler( &signal_init0 );
            signal_handler( &signal_init1 );
            bcore_arr_fp_s_push( signal_handler_arr_fp_g, ( fp_t )signal_handler );
        }

        bcore_mutex_s_unlock( &mutex );
    }
}

//----------------------------------------------------------------------------------------------------------------------

static void bcore_push_dependencies( bcore_fp_signal_handler signal_handler, bcore_arr_fp_s* arr )
{
    if( !signal_handler ) return;
    bcore_signal_s signal_push_dependencies = bcore_signal_init( TYPEOF_all, TYPEOF_push_dependencies, arr );
    signal_handler( &signal_push_dependencies );
}

//----------------------------------------------------------------------------------------------------------------------

/** Creates a complete and unique list of dependencies of signal_handler
 *  Cycle-safe
 */
static void bcore_push_signal_handler_with_dependencies_unique_recursive
(
    bcore_fp_signal_handler signal_handler,
    bcore_arr_fp_s* arr
)
{
    if( !signal_handler ) return;

    if( bcore_arr_fp_s_find( arr, 0, -1, ( fp_t )signal_handler ) == arr->size ) // if not yet registered
    {
        bcore_arr_fp_s* arr1 = bcore_arr_fp_s_create();
        bcore_push_dependencies( signal_handler, arr1 );
        BFOR_EACH( i, arr1 )
        {
            bcore_push_signal_handler_with_dependencies_unique_recursive( ( bcore_fp_signal_handler )arr1->data[ i ], arr );
        }

        bcore_arr_fp_s_push( arr, ( fp_t )signal_handler );
        bcore_arr_fp_s_discard( arr1 );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/** Registers signal_handler and all deep dependencies (in reverse order).
 */
void bcore_register_signal_handler_with_deep_dependencies( bcore_fp_signal_handler signal_handler )
{
    init();
    if( !signal_handler ) return;
    bcore_arr_fp_s* arr = bcore_arr_fp_s_create();
    bcore_push_signal_handler_with_dependencies_unique_recursive( signal_handler, arr );
    for( sz_t i = 0; i < arr->size; i++ )
    {
        bcore_register_signal_handler( ( bcore_fp_signal_handler )arr->data[ i ] );
    }
    bcore_arr_fp_s_discard( arr );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_init()
{
    bcore_register_signal_handler( NULL );
}

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_global_signal_handler( const bcore_signal_s* signal )
{
    return bcore_signal_s_broadcast
    (
        signal,
        ( bcore_fp_signal_handler* )signal_handler_arr_fp_g->data,
        signal_handler_arr_fp_g->size
    );
}


//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_run_signal_globally( tp_t target, tp_t type, vd_t object )
{
    bcore_signal_s signal = bcore_signal_init( target, type, object );
    return bcore_global_signal_handler( &signal );
}


//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_run_signal_selftest( tp_t target, vd_t object )
{
    return bcore_run_signal_globally( target, TYPEOF_selftest, object );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_down( s2_t verbosity )
{
    if( !signal_handler_arr_fp_g ) return;

    bcore_mutex_s_lock( &mutex );
    if( !signal_handler_arr_fp_g ) ERR( "Shutdown race condition detected: Do not call bcore_down from multiple threads." );

    bcore_signal_s signal_down1 = bcore_signal_init( TYPEOF_all, TYPEOF_down1, &verbosity );
    bcore_signal_s signal_down0 = bcore_signal_init( TYPEOF_all, TYPEOF_down0, &verbosity );

    // shut down all but first library in reverse order
    for( uz_t i = signal_handler_arr_fp_g->size - 1; i > 0; i-- )
    {
        ( ( bcore_fp_signal_handler )signal_handler_arr_fp_g->data[ i ] )( &signal_down1 );
        ( ( bcore_fp_signal_handler )signal_handler_arr_fp_g->data[ i ] )( &signal_down0 );
    }

    bcore_arr_fp_s_discard( signal_handler_arr_fp_g );
    signal_handler_arr_fp_g = NULL;

    bcore_mutex_s_unlock( &mutex );
    bcore_mutex_s_down( &mutex );

    bcore_general_signal_handler( &signal_down1 );

    if( verbosity > 0 )
    {
        bcore_msg( "\nBeth global system's memory usage (bytes):\n");
        uz_t space = bcore_tbman_total_granted_space();
        bcore_msg( "Total ................. % 7zu\n", space );
    }

    bcore_general_signal_handler( &signal_down0 );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_down_exit( s2_t verbosity, s2_t return_value )
{
    bcore_down( verbosity );
    bcore_exit( return_value );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_pop_errors_down_exit( s2_t return_value )
{
    bcore_error_pop_all_to_stderr();
    bcore_down_exit( -1, return_value );
}

//----------------------------------------------------------------------------------------------------------------------


