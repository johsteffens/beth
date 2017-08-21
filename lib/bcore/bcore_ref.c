#include "bcore_ref.h"

sr_s sr_spect( sr_s o, tp_t spect_type )
{
    o.p = ch_spect( o.p, spect_type );
    return o;
}
