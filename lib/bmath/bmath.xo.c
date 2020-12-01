/** This file was generated from xoila source code.
 *  Compiling Agent : xoico_compiler (C) 2020 J.B.Steffens
 *  Last File Update: 2020-12-01T10:47:36Z
 *
 *  Copyright and License of this File:
 *
 *  Generated code inherits the copyright and license of the underlying xoila source code.
 *  Source code defining this file is distributed across following files:
 *
 *  bmath_xsmf2.h
 *  bmath_xsmf3.h
 *  bmath_asmf2.h
 *  bmath_asmf3.h
 *
 */

#include "bmath.xo.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
// source: bmath_xsmf2.h
#include "bmath_xsmf2.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_xsmf2

BCORE_DEFINE_OBJECT_INST_P( bmath_xsmf2_s )
"aware bcore_inst"
"{"
    "sz_t slos;"
    "sz_t xons;"
    "sz_t rows;"
    "sz_t i_stride;"
    "sz_t [] i;"
    "f2_t [] v;"
    "func bcore_fp:copy_typed;"
"}";

XOILA_DEFINE_SPECT( bcore_inst, bmath_xsmf2 )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bmath_xsmf3.h
#include "bmath_xsmf3.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_xsmf3

BCORE_DEFINE_OBJECT_INST_P( bmath_xsmf3_s )
"aware bcore_inst"
"{"
    "sz_t slos;"
    "sz_t xons;"
    "sz_t rows;"
    "sz_t i_stride;"
    "sz_t [] i;"
    "f3_t [] v;"
    "func bcore_fp:copy_typed;"
"}";

XOILA_DEFINE_SPECT( bcore_inst, bmath_xsmf3 )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bmath_asmf2.h
#include "bmath_asmf2.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_asmf2

BCORE_DEFINE_OBJECT_INST_P( bmath_asmf2_s )
"aware bcore_inst"
"{"
    "sz_t cols;"
    "sz_t rows;"
    "sz_t i_stride;"
    "sz_t [] i;"
    "f2_t [] v;"
    "func bcore_fp:copy_typed;"
"}";

XOILA_DEFINE_SPECT( bcore_inst, bmath_asmf2 )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/
// source: bmath_asmf3.h
#include "bmath_asmf3.h"

//----------------------------------------------------------------------------------------------------------------------
// group: bmath_asmf3

BCORE_DEFINE_OBJECT_INST_P( bmath_asmf3_s )
"aware bcore_inst"
"{"
    "sz_t cols;"
    "sz_t rows;"
    "sz_t i_stride;"
    "sz_t [] i;"
    "f3_t [] v;"
    "func bcore_fp:copy_typed;"
"}";

XOILA_DEFINE_SPECT( bcore_inst, bmath_asmf3 )
"{"
    "bcore_spect_header_s header;"
"}";

/**********************************************************************************************************************/

vd_t bcore_general_signal_handler( const bcore_signal_s* o );

vd_t bmath_xo_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_xo" ) ) )
    {
        case TYPEOF_init1:
        {

            // --------------------------------------------------------------------
            // source: bmath_xsmf2.h

            // group: bmath_xsmf2
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bmath_xsmf2_s_copy_typed );
            BCORE_REGISTER_OBJECT( bmath_xsmf2_s );
            BCORE_REGISTER_NAME( bmath_fp_xsmf2_s_mul );
            BCORE_REGISTER_NAME( bmath_fp_xsmf2_s_mul_htp );
            BCORE_REGISTER_NAME( bmath_fp_xsmf2_s_htp_mul );
            BCORE_REGISTER_NAME( bmath_fp_xsmf2_s_htp_mul_htp );
            XOILA_REGISTER_SPECT( bmath_xsmf2 );

            // --------------------------------------------------------------------
            // source: bmath_xsmf3.h

            // group: bmath_xsmf3
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bmath_xsmf3_s_copy_typed );
            BCORE_REGISTER_OBJECT( bmath_xsmf3_s );
            BCORE_REGISTER_NAME( bmath_fp_xsmf3_s_mul );
            BCORE_REGISTER_NAME( bmath_fp_xsmf3_s_mul_htp );
            BCORE_REGISTER_NAME( bmath_fp_xsmf3_s_htp_mul );
            BCORE_REGISTER_NAME( bmath_fp_xsmf3_s_htp_mul_htp );
            XOILA_REGISTER_SPECT( bmath_xsmf3 );

            // --------------------------------------------------------------------
            // source: bmath_asmf2.h

            // group: bmath_asmf2
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bmath_asmf2_s_copy_typed );
            BCORE_REGISTER_OBJECT( bmath_asmf2_s );
            BCORE_REGISTER_NAME( bmath_fp_asmf2_s_mul );
            BCORE_REGISTER_NAME( bmath_fp_asmf2_s_mul_htp );
            BCORE_REGISTER_NAME( bmath_fp_asmf2_s_htp_mul );
            BCORE_REGISTER_NAME( bmath_fp_asmf2_s_htp_mul_htp );
            XOILA_REGISTER_SPECT( bmath_asmf2 );

            // --------------------------------------------------------------------
            // source: bmath_asmf3.h

            // group: bmath_asmf3
            BCORE_REGISTER_FFUNC( bcore_fp_copy_typed, bmath_asmf3_s_copy_typed );
            BCORE_REGISTER_OBJECT( bmath_asmf3_s );
            BCORE_REGISTER_NAME( bmath_fp_asmf3_s_mul );
            BCORE_REGISTER_NAME( bmath_fp_asmf3_s_mul_htp );
            BCORE_REGISTER_NAME( bmath_fp_asmf3_s_htp_mul );
            BCORE_REGISTER_NAME( bmath_fp_asmf3_s_htp_mul_htp );
            XOILA_REGISTER_SPECT( bmath_asmf3 );
        }
        break;
        case TYPEOF_push_dependencies:
        {
            ASSERT( o->object && ( *( aware_t* )o->object ) == TYPEOF_bcore_arr_fp_s );
            bcore_arr_fp_s* arr_fp = o->object;
            bcore_arr_fp_s_push( arr_fp, ( fp_t )bcore_general_signal_handler );
        }
        break;
        default: break;
    }
    return NULL;
}
// XOILA_OUT_SIGNATURE 0xF86E2893F32FD571ull
