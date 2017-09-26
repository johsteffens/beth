/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_control.h"
#include "bcore_threads.h"
#include "bcore_memory_manager.h"
#include "bcore_name_manager.h"
#include "bcore_signal.h"
#include "bcore.h"

static void init_once()
{
    bcore_signal( typeof( "all" ), typeof( "init0" ), NULL ); // init0 opens critical services
    bcore_signal( typeof( "all" ), typeof( "init1" ), NULL );
}

void bcore_library_init()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, init_once );
}

void bcore_library_down()
{
    bcore_signal( typeof( "all" ), typeof( "down1" ), NULL );

    bcore_msg( "\nBeth global system's memory usage (bytes):\n");
    sz_t space = bcore_memory_manager_granted_space();
    bcore_msg( "Total ................. % 6zu\n", space );

    bcore_signal( typeof( "all" ), typeof( "down0" ), NULL ); // down 0 shuts down critical services
}

