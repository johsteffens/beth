/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

#include "bcore_ref.h"
#include "bcore_life.h"

sr_s sr_spect( sr_s o, tp_t spect_type )
{
    o.p = ch_spect( o.p, spect_type );
    return o;
}

sr_s sr_cl( sr_s o, bcore_life_s* l )
{
    return bcore_life_s_push_sr( l, o );
}
