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

/** Matrix types and operations.
 *  All routines have been entirely designed from scratch and optimized with modern architectures in mind.
 */

#include <stdio.h>

#include "bmath_mf3.h"
#include "bmath_spect_matrix.h"
#include "bmath_fourier.h"
#include "bmath_grt.h"
#include "bmath_mfx_eval.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_mfx_mul_c.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_mfx_qrd_c.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_mfx_evd_c.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_mfx_svd_c.h"

#define BMATH_TEMPLATE_FX_PREC 3
#include "bmath_template_mfx_c.h"

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bmath_matrix, bmath_mf3_s )
"{"
    "aware_t _;"
    "uz_t rows;"
    "uz_t cols;"
    "uz_t stride;"
    "f3_t [] arr;"

    "const tp_t type_vector_assoc = bmath_vf3_s;"

    "func bmath_fp: is_equ;"
    "func bmath_fp: is_zro;"
    "func bmath_fp: is_one;"
    "func bmath_fp: add;"
    "func bmath_fp: zro;"
    "func bmath_fp: neg;"
    "func bmath_fp: sub;"
    "func bmath_fp: cpy;"
    "func bmath_fp: mul;"
    "func bmath_fp: one;"
    "func bmath_fp: inv;"
    "func bmath_fp: div;"
    "func bmath_fp_matrix: mul_vec;"
    "func bmath_fp_matrix: mul_scl;"

"}";

/**********************************************************************************************************************/
/// array of mf3_s

BCORE_DEFINE_ARRAY_DYN_SOLID_STATIC_AUT( bmath_mf3_s, bmath_arr_mf3_s );

/**********************************************************************************************************************/

vd_t bmath_mf3_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_mf3" ) ) )
    {
        case TYPEOF_init1:
        {
            // features
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_mul );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_mul_htp );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_htp_mul );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_htp_mul_htp );

            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_uav );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_ua  );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_av  );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_uau );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_vav );

            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_trd_htp );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_trd );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_evd_htp );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_svd );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_ubd );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_lbd );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_qrd );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_qrd_pmt );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_lqd );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_pmt_lqd );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_cld );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_lud );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_inv );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_pdf_inv );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_piv );
            BCORE_REGISTER_FEATURE( bmath_fp_mf3_s_hsm_piv );

            BCORE_REGISTER_FFUNC( bmath_fp_is_equ,         bmath_mf3_s_is_equ );
            BCORE_REGISTER_FFUNC( bmath_fp_is_zro,         bmath_mf3_s_is_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_is_one,         bmath_mf3_s_is_one );

            BCORE_REGISTER_FFUNC( bmath_fp_add,            bmath_mf3_s_add );
            BCORE_REGISTER_FFUNC( bmath_fp_zro,            bmath_mf3_s_zro );
            BCORE_REGISTER_FFUNC( bmath_fp_neg,            bmath_mf3_s_neg );
            BCORE_REGISTER_FFUNC( bmath_fp_sub,            bmath_mf3_s_sub );
            BCORE_REGISTER_FFUNC( bmath_fp_cpy,            bmath_mf3_s_cpy );
            BCORE_REGISTER_FFUNC( bmath_fp_mul,            bmath_mf3_s_mul );
            BCORE_REGISTER_FFUNC( bmath_fp_one,            bmath_mf3_s_one );
            BCORE_REGISTER_FFUNC( bmath_fp_inv,            bmath_mf3_s_inv );
            BCORE_REGISTER_FFUNC( bmath_fp_div,            bmath_mf3_s_div );
            BCORE_REGISTER_FFUNC( bmath_fp_matrix_mul_vec, bmath_mf3_s_mul_vec );
            BCORE_REGISTER_FFUNC( bmath_fp_matrix_mul_scl, bmath_mf3_s_mul_scl );

            BCORE_REGISTER_OBJECT( bmath_mf3_s );
            BCORE_REGISTER_OBJECT( bmath_arr_mf3_s );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bmath_mf3_s );
            BCORE_REGISTER_QUICKTYPE( bmath_arr_mf3_s );

            // features
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_mul );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_mul_htp );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_htp_mul );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_htp_mul_htp );

            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_uav );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_ua );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_av );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_uau );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_vav );

            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_trd_htp );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_trd );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_evd_htp );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_svd );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_ubd );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_lbd );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_qrd );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_qrd_pmt );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_lqd );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_pmt_lqd );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_cld );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_lud );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_inv );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_pdf_inv );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_piv );
            BCORE_REGISTER_QUICKTYPE( bmath_fp_mf3_s_hsm_piv );
        }
        break;

        case TYPEOF_selftest:
        {
            selftest();
            eval_test();
            return NULL;
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

