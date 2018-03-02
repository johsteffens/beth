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

/**********************************************************************************************************************/

bcore_arr_fp_s* signal_arr_g = NULL;
bcore_mutex_t mutex;

static void init_once()
{
    bcore_signal( typeof( "all" ), typeof( "init0" ), NULL ); // init0 opens critical services
    bcore_signal( typeof( "all" ), typeof( "init1" ), NULL );
    bcore_mutex_init( &mutex );
    signal_arr_g = bcore_arr_fp_s_create();
    bcore_arr_fp_s_push( signal_arr_g, ( fp_t )bcore_signal );
}

static void init()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, init_once );
}

void bcore_init_library( bcore_fp_signal signal )
{
    init();
    bcore_mutex_lock( &mutex );

    if( bcore_arr_fp_s_find( signal_arr_g, 0, signal_arr_g->size, ( fp_t )signal ) == signal_arr_g->size )
    {
        signal( typeof( "all" ), typeof( "init0" ), NULL );
        signal( typeof( "all" ), typeof( "init1" ), NULL );
        bcore_arr_fp_s_push( signal_arr_g, ( fp_t )signal );
    }

    bcore_mutex_unlock( &mutex );
}

void bcore_init()
{
    bcore_init_library( bcore_signal );
}

void bcore_down( bl_t verbose )
{
    if( !signal_arr_g ) return;

    bcore_mutex_lock( &mutex );
    if( !signal_arr_g ) ERR( "Shutdown race condition." );

    // shut down all but first library in reverse order
    for( sz_t i = signal_arr_g->size - 1; i > 0; i-- )
    {
        bcore_fp_signal signal = ( bcore_fp_signal )signal_arr_g->data[ i ];
        signal( typeof( "all" ), typeof( "down1" ), NULL );
        signal( typeof( "all" ), typeof( "down0" ), NULL );
    }
    bcore_arr_fp_s_discard( signal_arr_g );
    signal_arr_g = NULL;

    bcore_mutex_unlock( &mutex );
    bcore_mutex_down( &mutex );

    bcore_signal( typeof( "all" ), typeof( "down1" ), &verbose );

    if( verbose )
    {
        bcore_msg( "\nBeth global system's memory usage (bytes):\n");
        sz_t space = bcore_tbman_granted_space();
        bcore_msg( "Total ................. % 6zu\n", space );
    }

    bcore_signal( typeof( "all" ), typeof( "down0" ), &verbose ); // down 0 shuts down critical services
}

