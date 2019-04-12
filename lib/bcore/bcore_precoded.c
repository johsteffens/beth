/** This file is generated from 'precode' by the beth-precode-compiler.
 *
 *  Copyright and License:
 *      As specified for the repository in which this file resides.
 *      In case of doubt, the specification of the associated precode shall apply. 
 *
 *  Not suitable for manual editing.
 */

#include "bcore_precoded.h"
#include "bcore_spect_inst.h"

#include "bcore_precoder_sample.h"

//KEY 1940901916 1590493120
//KEY 1621576576 2800257400

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_sample_object_s ) "{ aware_t _; st_s name; sz_t => [] arr; } ";
BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_another_sample_object_s ) "{ aware_t _; u2_t val; }";

vd_t bcore_precoded_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_precoded_precoder" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bcore_precoder_sample_object_s );
            BCORE_REGISTER_OBJECT( bcore_precoder_another_sample_object_s );
        }
        break;
        default: break;
    }
    return NULL;
}
