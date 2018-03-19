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

#include "bcore_control.h"
#include "bcore_threads.h"
#include "bcore_tbman.h"
#include "bcore_name_manager.h"
#include "bcore_signal.h"
#include "bcore_arr.h"
#include "bcore.h"

#include <locale.h>

/**********************************************************************************************************************/

bcore_arr_fp_s* signal_handler_arr_g = NULL;
bcore_mutex_s mutex;

static void init_once()
{
    /// system wide settings
//    setlocale( LC_ALL, "C" );

    /// bcore-library initialization
    bcore_signal_s signal_init0 = bcore_signal_init( TYPEOF_all, TYPEOF_init0, NULL );
    bcore_signal_s signal_init1 = bcore_signal_init( TYPEOF_all, TYPEOF_init1, NULL );

    bcore_signal_handler( &signal_init0 );
    bcore_signal_handler( &signal_init1 );

    bcore_mutex_s_init( &mutex );
    signal_handler_arr_g = bcore_arr_fp_s_create();
    bcore_arr_fp_s_push( signal_handler_arr_g, ( fp_t )bcore_signal_handler );
}

static void init()
{
    static bcore_once_s flag = bcore_once_init;
    bcore_once_s_run( &flag, init_once );
}

void bcore_register_signal_handler( bcore_fp_signal_handler signal_handler )
{
    init();
    if( signal_handler )
    {
        bcore_signal_s signal_init0 = bcore_signal_init( TYPEOF_all, TYPEOF_init0, NULL );
        bcore_signal_s signal_init1 = bcore_signal_init( TYPEOF_all, TYPEOF_init1, NULL );
        bcore_mutex_s_lock( &mutex );
        bl_t not_registered =
            bcore_arr_fp_s_find( signal_handler_arr_g, 0, -1, ( fp_t )signal_handler )
                == signal_handler_arr_g->size;

        if( not_registered )
        {
            signal_handler( &signal_init0 );
            signal_handler( &signal_init1 );
            bcore_arr_fp_s_push( signal_handler_arr_g, ( fp_t )signal_handler );
        }

        bcore_mutex_s_unlock( &mutex );
    }
}

void bcore_init()
{
    bcore_register_signal_handler( NULL );
}

vd_t bcore_global_signal_handler( const bcore_signal_s* signal )
{
    return bcore_signal_s_broadcast
    (
        signal,
        ( bcore_fp_signal_handler* )signal_handler_arr_g->data,
        signal_handler_arr_g->size
    );
}

vd_t bcore_run_signal_selftest( tp_t target, vd_t object )
{
    bcore_signal_s signal = bcore_signal_init( target, TYPEOF_selftest, object );
    return bcore_global_signal_handler( &signal );
}

void bcore_down( bl_t verbose )
{
    if( !signal_handler_arr_g ) return;

    bcore_mutex_s_lock( &mutex );
    if( !signal_handler_arr_g ) ERR( "Shutdown race condition." );

    bcore_signal_s signal_down1 = bcore_signal_init( TYPEOF_all, TYPEOF_down1, &verbose );
    bcore_signal_s signal_down0 = bcore_signal_init( TYPEOF_all, TYPEOF_down0, &verbose );

    // shut down all but first library in reverse order
    for( sz_t i = signal_handler_arr_g->size - 1; i > 0; i-- )
    {
        ( ( bcore_fp_signal_handler )signal_handler_arr_g->data[ i ] )( &signal_down1 );
        ( ( bcore_fp_signal_handler )signal_handler_arr_g->data[ i ] )( &signal_down0 );
    }

    bcore_arr_fp_s_discard( signal_handler_arr_g );
    signal_handler_arr_g = NULL;

    bcore_mutex_s_unlock( &mutex );
    bcore_mutex_s_down( &mutex );

    bcore_signal_handler( &signal_down1 );

    if( verbose )
    {
        bcore_msg( "\nBeth global system's memory usage (bytes):\n");
        sz_t space = bcore_tbman_granted_space();
        bcore_msg( "Total ................. % 6zu\n", space );
    }

    bcore_signal_handler( &signal_down0 );
}

