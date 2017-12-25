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
#include "bcore.h"

/// external signal handler (e.g. by application)
static bcore_fp_signal ext_signal = NULL;

static void init_once()
{
    bcore_signal( typeof( "all" ), typeof( "init0" ), NULL ); // init0 opens critical services
    bcore_signal( typeof( "all" ), typeof( "init1" ), NULL );
    if( ext_signal )
    {
        ext_signal( typeof( "all" ), typeof( "init0" ), NULL ); // init0 opens critical services
        ext_signal( typeof( "all" ), typeof( "init1" ), NULL );
    }
}

void bcore_library_init( bcore_fp_signal signal )
{
    ext_signal = signal;
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, init_once );
}

void bcore_library_down( bl_t verbose )
{
    if( ext_signal )
    {
        ext_signal( typeof( "all" ), typeof( "down1" ), &verbose );
        ext_signal( typeof( "all" ), typeof( "down0" ), &verbose );
    }

    bcore_signal( typeof( "all" ), typeof( "down1" ), &verbose );

    if( verbose )
    {
        bcore_msg( "\nBeth global system's memory usage (bytes):\n");
        sz_t space = bcore_tbman_granted_space();
        bcore_msg( "Total ................. % 6zu\n", space );
    }

    bcore_signal( typeof( "all" ), typeof( "down0" ), &verbose ); // down 0 shuts down critical services
}

