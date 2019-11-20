/** Author and Copyright 2018 Johannes Bernhard Steffens
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
#include "bmath_feature.h"
#include "bmath_spect_algebraic.h"
#include "bmath_spect_vector.h"
#include "bmath_spect_matrix.h"
#include "bmath_leaf.h"
#include "bmath_f3_op.h"
#include "bmath_cf2.h"
#include "bmath_cf3.h"
#include "bmath_fourier_f2.h"
#include "bmath_fourier_f3.h"
#include "bmath_vf2.h"
#include "bmath_vf3.h"
#include "bmath_arr_vf2.h"
#include "bmath_arr_vf3.h"
#include "bmath_vcf2.h"
#include "bmath_vcf3.h"
#include "bmath_mf2.h"
#include "bmath_mf3.h"
#include "bmath_mfx_eval.h"
#include "bmath_xsmf2.h"
#include "bmath_xsmf3.h"
#include "bmath_asmf2.h"
#include "bmath_asmf3.h"
#include "bmath_pmt.h"
#include "bmath_grt_f2.h"
#include "bmath_grt_f3.h"
#include "bmath_estimator.h"
#include "bmath_plot.h"
#include "bmath_planted.h"

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
            bmath_feature_signal_handler,
            bmath_spect_algebraic_signal_handler,
            bmath_spect_vector_signal_handler,
            bmath_spect_matrix_signal_handler,
            bmath_leaf_signal_handler,
            bmath_f3_op_signal_handler,
            bmath_cf2_signal_handler,
            bmath_cf3_signal_handler,
            bmath_fourier_f2_signal_handler,
            bmath_fourier_f3_signal_handler,
            bmath_vf2_signal_handler,
            bmath_vf3_signal_handler,
            bmath_arr_vf2_signal_handler,
            bmath_arr_vf3_signal_handler,
            bmath_vcf2_signal_handler,
            bmath_vcf3_signal_handler,
            bmath_mf2_signal_handler,
            bmath_mf3_signal_handler,
            bmath_xsmf2_signal_handler,
            bmath_xsmf3_signal_handler,
            bmath_asmf2_signal_handler,
            bmath_asmf3_signal_handler,
            bmath_mfx_eval_signal_handler,
            bmath_pmt_signal_handler,
            bmath_grt_f2_signal_handler,
            bmath_grt_f3_signal_handler,
            bmath_estimator_signal_handler,
            bmath_plot_signal_handler,
            bmath_planted_signal_handler,
        };

        ret = bcore_signal_s_broadcast( o, arr, sizeof( arr ) / sizeof( bcore_fp_signal_handler ) );
    }

    return ret;
}

/**********************************************************************************************************************/
