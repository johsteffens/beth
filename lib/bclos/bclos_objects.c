/// Author & Copyright (C) 2017 Johannes Bernhard Steffens. All rights reserved.

#include "bclos_objects.h"
#include "bclos_quicktypes.h"
#include "bcore_trait.h"

/**********************************************************************************************************************/

vd_t bclos_objects_signal( tp_t target, tp_t signal, vd_t object )
{
    if( target != typeof( "all" ) && target != typeof( "bclos_objects" ) ) return NULL;

    if( signal == typeof( "init1" ) )
    {
    }

    return NULL;
}

