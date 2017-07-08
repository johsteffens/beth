/// Author & Copyright (C) 2017 Johannes Steffens <johannes.b.steffens@gmail.com>. All rights reserved.

/** Name registration framework
 *  This framework links names to hash values and vice versa.
 *  Hash value 0 is reserved. Names resulting is hash 0 are rejected.
 */

#ifndef BCORE_QUICKTYPES_H
#define BCORE_QUICKTYPES_H

#include "bcore_name_manager.h"

/// This function computes and formats defines below to stdout
void bcore_quicktypes_to_stdout( tp_t (*hash)( sc_t name ) );

/** List of predefined types for quick access.
 *  Created via bcore_quicktypes_to_stdout( NULL );
 */

// leaf types
#define TYPEOF_s3_t                     1748356928
#define TYPEOF_s2_t                      219039883
#define TYPEOF_s1_t                     1151894370
#define TYPEOF_s0_t                     3938332973
#define TYPEOF_u3_t                      629390130
#define TYPEOF_u2_t                     3405486141
#define TYPEOF_u1_t                     1184275984
#define TYPEOF_u0_t                     3960371995
#define TYPEOF_f3_t                     3646813157
#define TYPEOF_f2_t                     1418046714
#define TYPEOF_sz_t                     2631659827
#define TYPEOF_sd_t                     4188968313
#define TYPEOF_sc_t                     3542660944
#define TYPEOF_vd_t                     1618651012
#define TYPEOF_vc_t                      283133669
#define TYPEOF_fp_t                     2940508440
#define TYPEOF_tp_t                      908158082
#define TYPEOF_bool                     3365180733
#define TYPEOF_aware_t                  1507719906
#define TYPEOF_bcore_flect_self_s        556415048

// frequently used types
#define TYPEOF_bcore_string_s           2419449576
#define TYPEOF_bcore_source_string_s    1049426656

// attributes
#define TYPEOF_private                  1657474316
#define TYPEOF_cyclic                   1833008818
#define TYPEOF_external                  857568394

#endif // BCORE_QUICKTYPES_H
