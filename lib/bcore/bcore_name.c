/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bcore_name.h"

/**********************************************************************************************************************/
// signal

vd_t bcore_name_manager_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bcore_name_manager" ) ) return NULL;
    if( signal == typeof( "init1" ) )
    {
    }
    else if( signal == typeof( "selftest" ) )
    {
    }
    else if( signal == typeof( "down0" ) )
    {
    }
    return NULL;
}

