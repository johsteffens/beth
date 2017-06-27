/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_control.h"
#include "bcore_threads.h"
#include "bcore_memory_manager.h"
#include "bcore_perspective.h"
#include "bcore_name_manager.h"
#include "bcore_flect.h"
#include "bcore_string.h"
#include "bcore_string_source.h"
#include "bcore_life.h"
#include "bcore_bml.h"
#include "bcore.h"

static void init_library_bcore_once()
{
    // global services ...
    bcore_memory_manager_open();
    bcore_flect_open();

    // reflections ...
    bcore_flect_define_basics();
    bcore_flect_define_self_d( bcore_bml_translator_s_create_self() );
    bcore_flect_define_self_d( bcore_bml_interpreter_s_create_self() );
    bcore_flect_define_self_d( bcore_string_source_s_create_self() );
    bcore_flect_define_self_d( bcore_life_s_create_self() );

    // critical quick-tests ...
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
    sz_t space = bcore_memory_manager_granted_space();
    bcore_perspective_close();
    bcore_msg( "\n");
    bcore_msg( "perspective mananger: %zu bytes\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();
    bcore_flect_close();
    bcore_msg( "reflection mananger : %zu bytes\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();
    bcore_name_manager_close();
    bcore_msg( "name mananger       : %zu bytes\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();

    if( space > 0 ) ERR( "Leaking memory: %zu bytes", space );

    bcore_memory_manager_close();
}

