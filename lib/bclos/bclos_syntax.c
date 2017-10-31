/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bclos_syntax.h"
#include "bclos_quicktypes.h"
#include "bcore_spect_inst.h"

/**********************************************************************************************************************/

DEFINE_FUNCTION_INIT_SPECT( bclos_syntax_s )
DEFINE_FUNCTION_DOWN_SPECT( bclos_syntax_s )
DEFINE_FUNCTION_COPY_SPECT( bclos_syntax_s )
DEFINE_FUNCTION_CREATE(     bclos_syntax_s )
DEFINE_FUNCTION_DISCARD(    bclos_syntax_s )
DEFINE_FUNCTION_CLONE(      bclos_syntax_s )

static bcore_flect_self_s* syntax_s_create_self( void )
{
    sc_t def = "bclos_syntax_s = { aware_t _; }";
    bcore_flect_self_s* self = bcore_flect_self_s_build_parse_sc( def, sizeof( bclos_syntax_s ) );
    return self;
}

/**********************************************************************************************************************/

vd_t bclos_syntax_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_syntax" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        bcore_flect_define_creator( typeof( "bclos_syntax_s" ), syntax_s_create_self );
    }

    return NULL;
}

/**********************************************************************************************************************/

