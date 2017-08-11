/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_flect.h"
#include "bcore_sources.h"
#include "bcore_sinks.h"
#include "bcore_life.h"
#include "bcore_hmap.h"
#include "bcore_hmap_tp_inst.h"
#include "bcore_signature.h"
#include "bcore_bml.h"
#include "bcore_spect_inst.h"
#include "bcore_spect_array.h"
#include "bcore_spect_via.h"
#include "bcore_spect_source.h"
#include "bcore_spect_sink.h"
#include "bcore_spect_translator.h"
#include "bcore_spect_interpreter.h"
#include "bcore_spect_compare.h"
#include "bcore_spect_closure.h"

void bcore_flect_define_self_creators()
{
    bcore_inst_define_self_creators();
    bcore_bml_define_self_creators();
    bcore_sources_define_self_creators();
    bcore_sinks_define_self_creators();
    bcore_closure_define_self_creators();
    bcore_hmap_define_self_creators();
    bcore_hmap_tp_inst_define_self_creators();

    bcore_flect_define_creator( typeof( "bcore_life_s"        ), bcore_life_s_create_self        );
    bcore_flect_define_creator( typeof( "bcore_signature_s"   ), bcore_signature_s_create_self   );
    bcore_flect_define_creator( typeof( "bcore_array_s"       ), bcore_array_s_create_self       );
    bcore_flect_define_creator( typeof( "bcore_via_s"         ), bcore_via_s_create_self         );
    bcore_flect_define_creator( typeof( "bcore_source_s"      ), bcore_source_s_create_self      );
    bcore_flect_define_creator( typeof( "bcore_sink_s"        ), bcore_sink_s_create_self        );
    bcore_flect_define_creator( typeof( "bcore_translator_s"  ), bcore_translator_s_create_self  );
    bcore_flect_define_creator( typeof( "bcore_interpreter_s" ), bcore_interpreter_s_create_self );
    bcore_flect_define_creator( typeof( "bcore_compare_s"     ), bcore_compare_s_create_self     );
}

