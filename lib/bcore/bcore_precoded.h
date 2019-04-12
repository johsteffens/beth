/** This file is generated from 'precode' by the beth-precode-compiler.
 *
 *  Copyright and License:
 *      As specified for the repository in which this file resides.
 *      In case of doubt, the specification of the associated precode shall apply. 
 *
 *  Not suitable for manual editing.
 */

#ifndef BCORE_PRECODED_H
#define BCORE_PRECODED_H

#include "bcore_control.h"

#define TYPEOF_bcore_precoder_sample_object_s 1940901916
#define EMPLACE_OBJECT_bcore_precoder_sample_object_s BCORE_DECLARE_OBJECT( bcore_precoder_sample_object_s ) { aware_t _; st_s name; BCORE_ARRAY_DYN_LINK_STATIC_S( sz_t, arr_ ); }

#define TYPEOF_bcore_precoder_another_sample_object_s 1621576576
#define EMPLACE_OBJECT_bcore_precoder_another_sample_object_s BCORE_DECLARE_OBJECT( bcore_precoder_another_sample_object_s ) { aware_t _; u2_t val; }

vd_t bcore_precoded_signal_handler( const bcore_signal_s* o );

#endif // BCORE_PRECODED_H
