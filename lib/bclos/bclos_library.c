/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bclos_library.h"
#include "bclos_quicktypes.h"
#include "bcore_spect_inst.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_INST( bclos_library_s )
DEFINE_FUNCTION_DOWN_INST( bclos_library_s )
DEFINE_FUNCTION_COPY_INST( bclos_library_s )
DEFINE_FUNCTION_CREATE(     bclos_library_s )
DEFINE_FUNCTION_DISCARD(    bclos_library_s )
DEFINE_FUNCTION_CLONE(      bclos_library_s )

static bcore_flect_self_s* library_s_create_self( void )
{
    sc_t def = "bclos_library_s = bcore_inst { aware_t _; bcore_hmap_tp_sr_s map; bclos_library_arr_s* deps; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_library_s ) );
    return self;
}

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_INST( bclos_library_arr_s )
DEFINE_FUNCTION_DOWN_INST( bclos_library_arr_s )
DEFINE_FUNCTION_COPY_INST( bclos_library_arr_s )
DEFINE_FUNCTION_CREATE(     bclos_library_arr_s )
DEFINE_FUNCTION_DISCARD(    bclos_library_arr_s )
DEFINE_FUNCTION_CLONE(      bclos_library_arr_s )

static bcore_flect_self_s* library_arr_s_create_self( void )
{
    sc_t def = "bclos_library_arr_s = bcore_inst_s { aware_t _; library_s* [] arr; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_library_arr_s ) );
    return self;
}

/**********************************************************************************************************************/

vd_t bclos_library_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_library" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_library_s" ), library_s_create_self );
        bcore_flect_define_creator( typeof( "bclos_library_arr_s" ), library_arr_s_create_self );
    }

    return NULL;
}

/**********************************************************************************************************************/


