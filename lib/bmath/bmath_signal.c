/** Copyright 2018 Johannes Bernhard Steffens
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

#include "bmath_signal.h"
#include "bmath_types.h"
#include "bmath_feature.h"
#include "bmath_spect_algebraic.h"
#include "bmath_spect_vector.h"
#include "bmath_spect_matrix.h"
#include "bmath_leaf.h"
#include "bmath_f3.h"
#include "bmath_complex.h"
#include "bmath_fourier.h"
#include "bmath_vf3.h"
#include "bmath_vcf3.h"
#include "bmath_mf3.h"
#include "bmath_mf3_eval.h"
#include "bmath_mf3_sx.h"
#include "bmath_mf3_sf.h"
#include "bmath_pmt.h"
#include "bmath_grt.h"
#include "bmath_simd.h"
#include "bmath_estimator.h"
#include "bmath_plot.h"
#include "bmath_planted.h"
#include "bmath_hf3.h"
#include "bmath_hf3_vm.h"

vd_t bmath_signal_handler( const bcore_signal_s* o )
{
    vd_t ret = NULL;

    // non-local targets
    if( o->target != TYPEOF_local )
    {
        /// nothing yet
    }

    if( ret ) return ret;

    // local targets
    {
        bcore_fp_signal_handler arr[] =
        {
            bmath_types_signal_handler,
            bmath_feature_signal_handler,
            bmath_spect_algebraic_signal_handler,
            bmath_spect_vector_signal_handler,
            bmath_spect_matrix_signal_handler,
            bmath_leaf_signal_handler,
            bmath_f3_signal_handler,
            bmath_complex_signal_handler,
            bmath_fourier_signal_handler,
            bmath_vf3_signal_handler,
            bmath_vcf3_signal_handler,
            bmath_mf3_signal_handler,
            bmath_mf3_sx_signal_handler,
            bmath_mf3_sf_signal_handler,
            bmath_matrix_eval_signal_handler,
            bmath_pmt_signal_handler,
            bmath_grt_signal_handler,
            bmath_simd_signal_handler,
            bmath_estimator_signal_handler,
            bmath_plot_signal_handler,
            bmath_planted_signal_handler,
            bmath_hf3_signal_handler,
            bmath_hf3_vm_signal_handler,
        };

        ret = bcore_signal_s_broadcast( o, arr, sizeof( arr ) / sizeof( bcore_fp_signal_handler ) );
    }

    return ret;
}

/**********************************************************************************************************************/
