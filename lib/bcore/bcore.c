/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_control.h"
#include "bcore_threads.h"
#include "bcore_memory_manager.h"
#include "bcore_perspective.h"
#include "bcore_name_manager.h"
#include "bcore_memory_manager.h"
#include "bcore_flect.h"
#include "bcore_string.h"
#include "bcore.h"

static void init_library_bcore_once()
{
    bcore_memory_manager_open();
    bcore_flect_define_basics();

    // critical quick-tests
    bcore_string_s_quicktest();
    bcore_memory_manager_s_quicktest();

}

void init_library_bcore()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, init_library_bcore_once );
}

void down_library_bcore()
{
    bcore_flect_close();
    bcore_perspective_close();
    bcore_name_manager_close();

    sz_t granted_space = bcore_memory_manager_granted_space();
    if( granted_space > 0 ) ERR( "Leaking memory: %lu bytes", granted_space );

    bcore_memory_manager_close();
}

