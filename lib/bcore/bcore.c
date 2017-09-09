/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_control.h"
#include "bcore_threads.h"
#include "bcore_memory_manager.h"
#include "bcore_name_manager.h"
#include "bcore_flect.h"
#include "bcore_flect_center.h"
#include "bcore_string.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_signal.h"
#include "bcore.h"

#include "bclos_flect_center.h"

static void init_once()
{
    // open global services ...
    bcore_memory_manager_open();
    bcore_name_manager_open();
    bcore_signature_manager_open();
    bcore_flect_open();
    bcore_spect_manager_open();
    bcore_trait_manager_open();

    // run some critical quick-tests ...
    bcore_memory_manager_s_quicktest();
    bcore_string_s_quicktest();

    // broadcast init signal
    bcore_signal( typeof( "all" ), typeof( "init" ), NULL );

    bcore_flect_define_self_creators();
    bclos_flect_define_self_creators();
}

void bcore_library_init()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, init_once );
}

void bcore_library_down()
{
    // broadcast down signal
    bcore_signal( typeof( "all" ), typeof( "down" ), NULL );

    bcore_msg( "\nBeth global system's memory usage (bytes):\n");
    sz_t space = bcore_memory_manager_granted_space();
    bcore_msg( "Total ................. % 6zu\n", space );
    bcore_trait_manager_close();
    bcore_msg( "  trait mananger ...... % 6zu\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();
    bcore_spect_manager_close();
    bcore_msg( "  spect mananger ...... % 6zu\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();
    bcore_flect_close();
    bcore_msg( "  reflection mananger . % 6zu\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();
    bcore_signature_manager_close();
    bcore_msg( "  signature mananger .. % 6zu\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();
    bcore_name_manager_close();
    bcore_msg( "  name mananger ....... % 6zu\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();

    if( space > 0 ) ERR( "Leaking memory: %zu bytes", space );

    bcore_memory_manager_close();
}

