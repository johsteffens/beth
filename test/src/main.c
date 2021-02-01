/** Author and Copyright 2020 Johannes Bernhard Steffens
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

#include "bcore_std.h"
#include "bcore_name_manager.h"
#include "bcore_signal.h"
#include "bcore_spect.h"
#include "bcore_x_via.h"
#include "bcore_quicktypes.h"
#include "bclos_quicktypes.h"
#include "bmath_quicktypes.h"
#include "bcore_threads.h"
#include "bcore_txt_ml.h"
#include "bcore_tp.h"
#include "bcore_trait.h"
#include "bclos_signal.h"
#include "bmath_signal.h"
#include "badapt_signal.h"
#include "bhvm_signal.h"

void selftest( const char* name )
{
    st_s_print_d( bcore_run_signal_selftest( typeof( name ), NULL ) );
}

int main( void )
{
    BETH_USE( bclos );
    BETH_USE( bmath );
    BETH_USE( bhvm );
    BETH_USE( bhpt );
    BETH_USE( badapt );

    if( bcore_error_stack_size() > 0  )
    {
        bcore_error_pop_all_to_stderr();
        BETH_CLOSEV( true );
        return 2;
    }

    bcore_flect_parse_all_flects();
    bcore_inst_test_all_types();

//    bcore_quicktypes_to_stdout( NULL ); return 0;
//    bclos_quicktypes_to_stdout( NULL ); return 0;
//    bmath_quicktypes_to_stdout( NULL ); return 0;

    x_via_selftest();

//    selftest( "bcore_spect_inst" );
//
//    selftest( "bcore_hmap" );
//    selftest( "bcore_tp_fastmap" );
//    selftest( "bcore_hmap_tp_sr" );
//
//    selftest( "bcore_st" );
//    selftest( "bcore_prsg" );
//    selftest( "bcore_sources" );
//
//    selftest( "bhvm_holor" );
//
//    selftest( "bmath_matrix" );
//    selftest( "bmath_complex" );
//    selftest( "bmath_spect_algebraic" );
//    selftest( "bmath_spect_vector" );
//
//    selftest( "bmath_mf2" );
//    selftest( "bmath_mf3" );
//    selftest( "bmath_asmf2" );
//    selftest( "bmath_asmf2" );
//    selftest( "bmath_xsmf3" );
//    selftest( "bmath_xsmf3" );
//    selftest( "bmath_vf2" );
//    selftest( "bmath_vf3" );
//    selftest( "bmath_arr_vf2" );
//    selftest( "bmath_arr_vf3" );
//    selftest( "bmath_cf2" );
//    selftest( "bmath_cf3" );
//    selftest( "bmath_fourier_f2" );
//    selftest( "bmath_fourier_f3" );
//    selftest( "bmath_vcf2" );
//    selftest( "bmath_vcf3" );
//
//    selftest( "bcore_cday" );
//    selftest( "bcore_plant_sample" );
//
//    selftest( "bclos_procedure" );
//    selftest( "bclos_machine" );
//
//    selftest( "bcore_folder" );
//    selftest( "bcore_flect" );
//    selftest( "bcore_name_manager" );
//    selftest( "bcore_spect_array" );
//    selftest( "bcore_spect_via" );
//    selftest( "bcore_spect_compare" );
//    selftest( "bcore_txt_ml" );
//    selftest( "bcore_bin_ml" );
//
//    selftest( "bcore_btree" );
//    selftest( "bcore_arr" );
//    selftest( "bcore_img" );
//    selftest( "bcore_spect_hash" );
//    selftest( "bcore_tbman" );

    st_s_print_d( bcore_spect_status() );

    bcore_msg_fa( "\n" );
    bcore_msg_fa( "#r80{=}\n" );
    bcore_msg_fa( "All tests completed without error.\n" );

    BETH_CLOSEV( 1 );
    return 0;
}
