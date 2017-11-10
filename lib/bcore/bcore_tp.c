/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_tp.h"
#include "bcore_name_manager.h"

/**********************************************************************************************************************/

vd_t bcore_tp_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_tp" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
        return NULL;
    }
    else if( signal == typeof( "selftest" ) )
    {
        return NULL;
    }

    return NULL;
}

/**********************************************************************************************************************/

