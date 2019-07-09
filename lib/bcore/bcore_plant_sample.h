/** Copyright 2019 Johannes Bernhard Steffens
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/** Testbed and sample for Plant Framework */

#ifndef BCORE_PLANT_SAMPLE_H
#define BCORE_PLANT_SAMPLE_H

#include "bcore_std.h"
#include "bcore_planted.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_plant_sample_signatures
BETH_PRECODE( bcore_plant_sample_signatures )
#ifdef BETH_PRECODE_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    signature void setup( mutable, sz_t n );
#endif // BETH_PRECODE_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif // optional enclosure

#ifdef TYPEOF_bcore_plant_sample_features
BETH_PRECODE( bcore_plant_sample_features )
#ifdef BETH_PRECODE_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    signature void sig_setup( mutable, sz_t n );

//    feature strict 'ptar' void setup( mutable, sz_t n );

    feature strict 'ptar' bcore_plant_sample_signatures : setup;

    feature        'ptar' void to_stdout( const ) =
    {
        bcore_msg_fa( "Called to_stdout__\n" );
    };
    feature        'ptar' sz_t get_size( const );

    name anyglobalname;
#endif // BETH_PRECODE_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif // optional enclosure

#ifdef TYPEOF_bcore_plant_sample
BETH_PRECODE( bcore_plant_sample )
#ifdef BETH_PRECODE_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    body to_stdout_body = { bcore_txt_ml_a_to_stdout( o ); };

    stamp : object = aware bcore_inst
    {
        st_s name1;
        sz_t => [] arr;
        func bcore_plant_sample_features : setup;
        func bcore_plant_sample_features : to_stdout = : to_stdout_body;
        func bcore_plant_sample_features : get_size;
    };

    stamp bcore_plant_sample_object2 = bcore_inst
    {
        aware_t _;
        bcore_plant_sample_object_s object;
        st_s name2;
        func bcore_plant_sample_features : setup;
        func bcore_plant_sample_features : to_stdout;
    };
#endif // BETH_PRECODE_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // optional enclosure

/**********************************************************************************************************************/

vd_t bcore_plant_sample_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif  // BCORE_PLANT_SAMPLE_H
