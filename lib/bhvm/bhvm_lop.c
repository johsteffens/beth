/** Author and Copyright 2019 Johannes Bernhard Steffens
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

#include "bhvm_lop.h"

#ifdef TYPEOF_bhvm_lop

/**********************************************************************************************************************/
// sub

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_sub_sqrsum_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += f2_sqr(((f2_t*)a)[i] - ((f2_t*)b)[i]); } break;
        case BKNIT_F223: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += f2_sqr(((f2_t*)a)[i] - ((f2_t*)b)[i]); } break;
        case BKNIT_F232: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += f3_sqr(((f2_t*)a)[i] - ((f3_t*)b)[i]); } break;
        case BKNIT_F233: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += f3_sqr(((f2_t*)a)[i] - ((f3_t*)b)[i]); } break;
        case BKNIT_F322: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += f3_sqr(((f3_t*)a)[i] - ((f2_t*)b)[i]); } break;
        case BKNIT_F323: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += f3_sqr(((f3_t*)a)[i] - ((f2_t*)b)[i]); } break;
        case BKNIT_F332: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += f3_sqr(((f3_t*)a)[i] - ((f3_t*)b)[i]); } break;
        case BKNIT_F333: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += f3_sqr(((f3_t*)a)[i] - ((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_sub_l1_s_f( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += f2_abs(((f2_t*)a)[i] - ((f2_t*)b)[i]); } break;
        case BKNIT_F223: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += f2_abs(((f2_t*)a)[i] - ((f2_t*)b)[i]); } break;
        case BKNIT_F232: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += f3_abs(((f2_t*)a)[i] - ((f3_t*)b)[i]); } break;
        case BKNIT_F233: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += f3_abs(((f2_t*)a)[i] - ((f3_t*)b)[i]); } break;
        case BKNIT_F322: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += f3_abs(((f3_t*)a)[i] - ((f2_t*)b)[i]); } break;
        case BKNIT_F323: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += f3_abs(((f3_t*)a)[i] - ((f2_t*)b)[i]); } break;
        case BKNIT_F332: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += f3_abs(((f3_t*)a)[i] - ((f3_t*)b)[i]); } break;
        case BKNIT_F333: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += f3_abs(((f3_t*)a)[i] - ((f3_t*)b)[i]); } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// mul

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_vv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] = ((f2_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] = ((f2_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] = ((f2_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] = ((f2_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] = ((f3_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] = ((f3_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] = ((f3_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] = ((f3_t*)a)[i] * ((f3_t*)b)[i]; } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_vv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] += ((f2_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] += ((f2_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] += ((f2_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] += ((f2_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] += ((f3_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] += ((f3_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] += ((f3_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] += ((f3_t*)a)[i] * ((f3_t*)b)[i]; } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_vs_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] = ((f2_t*)a)[i] * ((f2_t*)b)[0]; } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] = ((f2_t*)a)[i] * ((f2_t*)b)[0]; } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] = ((f2_t*)a)[i] * ((f3_t*)b)[0]; } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] = ((f2_t*)a)[i] * ((f3_t*)b)[0]; } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] = ((f3_t*)a)[i] * ((f2_t*)b)[0]; } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] = ((f3_t*)a)[i] * ((f2_t*)b)[0]; } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] = ((f3_t*)a)[i] * ((f3_t*)b)[0]; } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] = ((f3_t*)a)[i] * ((f3_t*)b)[0]; } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_vs_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] += ((f2_t*)a)[i] * ((f2_t*)b)[0]; } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] += ((f2_t*)a)[i] * ((f2_t*)b)[0]; } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] += ((f2_t*)a)[i] * ((f3_t*)b)[0]; } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] += ((f2_t*)a)[i] * ((f3_t*)b)[0]; } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] += ((f3_t*)a)[i] * ((f2_t*)b)[0]; } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] += ((f3_t*)a)[i] * ((f2_t*)b)[0]; } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[i] += ((f3_t*)a)[i] * ((f3_t*)b)[0]; } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[i] += ((f3_t*)a)[i] * ((f3_t*)b)[0]; } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_vv_as( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += ((f2_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F223: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += ((f2_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F232: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += ((f2_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F233: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += ((f2_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F322: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += ((f3_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F323: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += ((f3_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F332: for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += ((f3_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F333: for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += ((f3_t*)a)[i] * ((f3_t*)b)[i]; } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_vv_cs( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t s )
{
    assert( s == 0 || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += ((f2_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F223: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += ((f2_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F232: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += ((f2_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F233: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += ((f2_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F322: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += ((f3_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F323: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += ((f3_t*)a)[i] * ((f2_t*)b)[i]; } break;
        case BKNIT_F332: ((f2_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f2_t*)r)[0] += ((f3_t*)a)[i] * ((f3_t*)b)[i]; } break;
        case BKNIT_F333: ((f3_t*)r)[0] = 0; for(sz_t i=0; i<s; i++) { ((f3_t*)r)[0] += ((f3_t*)a)[i] * ((f3_t*)b)[i]; } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_mv_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols )
{
    assert( rows * cols == 0  || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i] += ((f2_t*)a)[i*cols+j] * ((f2_t*)b)[j]; } break;
        case BKNIT_F223: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i] += ((f2_t*)a)[i*cols+j] * ((f2_t*)b)[j]; } break;
        case BKNIT_F232: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i] += ((f2_t*)a)[i*cols+j] * ((f3_t*)b)[j]; } break;
        case BKNIT_F233: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i] += ((f2_t*)a)[i*cols+j] * ((f3_t*)b)[j]; } break;
        case BKNIT_F322: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i] += ((f3_t*)a)[i*cols+j] * ((f2_t*)b)[j]; } break;
        case BKNIT_F323: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i] += ((f3_t*)a)[i*cols+j] * ((f2_t*)b)[j]; } break;
        case BKNIT_F332: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i] += ((f3_t*)a)[i*cols+j] * ((f3_t*)b)[j]; } break;
        case BKNIT_F333: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i] += ((f3_t*)a)[i*cols+j] * ((f3_t*)b)[j]; } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_mv_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols )
{
    assert( rows * cols == 0  || ( a && b && r ) );
    switch( BKNIT_FA_L1_FROM_KNIT( tknit ) )
    {
        case BKNIT_F2: for(sz_t i=0; i<rows; i++) ((f2_t*)r)[i] = 0; break;
        case BKNIT_F3: for(sz_t i=0; i<rows; i++) ((f3_t*)r)[i] = 0; break;
        default: break;
    }
    bhvm_lop_ar2_mul_body_s_f_mv_av( tknit, a, b, r, rows, cols );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_vm_av( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols )
{
    assert( rows * cols == 0  || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[j] += ((f2_t*)b)[i*cols+j] * ((f2_t*)a)[i]; } break;
        case BKNIT_F223: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[j] += ((f2_t*)b)[i*cols+j] * ((f2_t*)a)[i]; } break;
        case BKNIT_F232: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[j] += ((f3_t*)b)[i*cols+j] * ((f2_t*)a)[i]; } break;
        case BKNIT_F233: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[j] += ((f3_t*)b)[i*cols+j] * ((f2_t*)a)[i]; } break;
        case BKNIT_F322: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[j] += ((f2_t*)b)[i*cols+j] * ((f3_t*)a)[i]; } break;
        case BKNIT_F323: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[j] += ((f2_t*)b)[i*cols+j] * ((f3_t*)a)[i]; } break;
        case BKNIT_F332: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[j] += ((f3_t*)b)[i*cols+j] * ((f3_t*)a)[i]; } break;
        case BKNIT_F333: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[j] += ((f3_t*)b)[i*cols+j] * ((f3_t*)a)[i]; } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_vm_cv( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols )
{
    assert( rows * cols == 0  || ( a && b && r ) );
    switch( BKNIT_FA_L1_FROM_KNIT( tknit ) )
    {
        case BKNIT_F2: for(sz_t i=0; i<cols; i++) ((f2_t*)r)[i] = 0; break;
        case BKNIT_F3: for(sz_t i=0; i<cols; i++) ((f3_t*)r)[i] = 0; break;
        default: break;
    }
    bhvm_lop_ar2_mul_body_s_f_vm_av( tknit, a, b, r, rows, cols );
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_vv_cm( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols )
{
    assert( rows * cols == 0  || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i*cols+j] = ((f2_t*)a)[i] * ((f2_t*)b)[j]; } break;
        case BKNIT_F223: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i*cols+j] = ((f2_t*)a)[i] * ((f2_t*)b)[j]; } break;
        case BKNIT_F232: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i*cols+j] = ((f2_t*)a)[i] * ((f3_t*)b)[j]; } break;
        case BKNIT_F233: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i*cols+j] = ((f2_t*)a)[i] * ((f3_t*)b)[j]; } break;
        case BKNIT_F322: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i*cols+j] = ((f3_t*)a)[i] * ((f2_t*)b)[j]; } break;
        case BKNIT_F323: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i*cols+j] = ((f3_t*)a)[i] * ((f2_t*)b)[j]; } break;
        case BKNIT_F332: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i*cols+j] = ((f3_t*)a)[i] * ((f3_t*)b)[j]; } break;
        case BKNIT_F333: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i*cols+j] = ((f3_t*)a)[i] * ((f3_t*)b)[j]; } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void bhvm_lop_ar2_mul_body_s_f_vv_am( tp_t tknit, vc_t a, vc_t b, vd_t r, sz_t rows, sz_t cols )
{
    assert( rows * cols == 0  || ( a && b && r ) );
    switch( tknit )
    {
        case BKNIT_F222: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i*cols+j] += ((f2_t*)a)[i] * ((f2_t*)b)[j]; } break;
        case BKNIT_F223: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i*cols+j] += ((f2_t*)a)[i] * ((f2_t*)b)[j]; } break;
        case BKNIT_F232: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i*cols+j] += ((f2_t*)a)[i] * ((f3_t*)b)[j]; } break;
        case BKNIT_F233: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i*cols+j] += ((f2_t*)a)[i] * ((f3_t*)b)[j]; } break;
        case BKNIT_F322: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i*cols+j] += ((f3_t*)a)[i] * ((f2_t*)b)[j]; } break;
        case BKNIT_F323: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i*cols+j] += ((f3_t*)a)[i] * ((f2_t*)b)[j]; } break;
        case BKNIT_F332: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f2_t*)r)[i*cols+j] += ((f3_t*)a)[i] * ((f3_t*)b)[j]; } break;
        case BKNIT_F333: for(sz_t i=0; i<rows; i++) for(sz_t j=0; j<cols; j++) { ((f3_t*)r)[i*cols+j] += ((f3_t*)a)[i] * ((f3_t*)b)[j]; } break;
        default: ERR_fa( "Invalid tknit '#<tp_t>'.", tknit );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bhvm_lop

vd_t bhvm_lop_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bhvm_lop" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_get_quicktypes:
        {
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        case TYPEOF_plant:
        {
            bcore_plant_compile( "bhvm_lop_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

