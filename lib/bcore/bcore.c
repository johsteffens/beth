/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_control.h"
#include "bcore_threads.h"
#include "bcore_memory_manager.h"
#include "bcore_name_manager.h"
#include "bcore_flect.h"
#include "bcore_string.h"
#include "bcore_source_string.h"
#include "bcore_life.h"
#include "bcore_bml.h"
#include "bcore_signature.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_spect_via.h"
#include "bcore_spect_source.h"
#include "bcore_spect_sink.h"
#include "bcore.h"

static void init_library_bcore_once()
{
    // open global services ...
    bcore_memory_manager_open();
    bcore_name_manager_open();
    bcore_signature_manager_open();
    bcore_flect_open();
    bcore_spect_manager_open();

    // define reflections ...
    bcore_flect_define_basics();
    bcore_flect_define_creator( typeof( "bcore_bml_translator_s"  ), bcore_bml_translator_s_create_self  );
    bcore_flect_define_creator( typeof( "bcore_bml_interpreter_s" ), bcore_bml_interpreter_s_create_self );
    bcore_flect_define_creator( typeof( "bcore_source_string_s"   ), bcore_source_string_s_create_self   );
    bcore_flect_define_creator( typeof( "bcore_life_s"            ), bcore_life_s_create_self            );
    bcore_flect_define_creator( typeof( "bcore_signature_s"       ), bcore_signature_s_create_self       );

    bcore_flect_define_creator( typeof( "bcore_inst_s"            ), bcore_inst_s_create_self            );
    bcore_flect_define_creator( typeof( "bcore_array_s"           ), bcore_array_s_create_self           );
    bcore_flect_define_creator( typeof( "bcore_via_s"             ), bcore_via_s_create_self             );
    bcore_flect_define_creator( typeof( "bcore_source_s"          ), bcore_source_s_create_self          );
    bcore_flect_define_creator( typeof( "bcore_sink_s"            ), bcore_sink_s_create_self            );

    // run some critical quick-tests ...
    bcore_memory_manager_s_quicktest();
    bcore_string_s_quicktest();
}

void init_library_bcore()
{
    static bcore_once_t flag = bcore_once_init;
    bcore_once( &flag, init_library_bcore_once );
}

void down_library_bcore()
{
    sz_t space = bcore_memory_manager_granted_space();
    bcore_msg( "\n");
    bcore_spect_manager_close();
    bcore_msg( "spect mananger      : %zu bytes\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();
    bcore_flect_close();
    bcore_msg( "reflection mananger : %zu bytes\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();
    bcore_signature_manager_close();
    bcore_msg( "signature mananger  : %zu bytes\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();
    bcore_name_manager_close();
    bcore_msg( "name mananger       : %zu bytes\n", space - bcore_memory_manager_granted_space() );
    space = bcore_memory_manager_granted_space();

    if( space > 0 ) ERR( "Leaking memory: %zu bytes", space );

    bcore_memory_manager_close();
}

