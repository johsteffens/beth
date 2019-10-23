/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_spect_matrix.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_signal.h"
#include "bcore_quicktypes.h"

#define NPX( name ) bcore_matrix_##name

BCORE_DEFINE_SPECT( bcore_inst, bcore_matrix )
"{"
    "bcore_spect_header_s header;"
    "private bcore_array_s* spect_array;"
    "feature     uz_t rows_fix   =  0 ~> const uz_t rows;"
    "feature     uz_t cols_fix   =  0 ~> const uz_t cols;"
    "feature     uz_t stride_fix =  0 ~> const uz_t stride;"
    "feature offset_t rows_off   = -1 ~>       uz_t rows;"
    "feature offset_t cols_off   = -1 ~>       uz_t cols;"
    "feature offset_t stride_off = -1 ~>       uz_t stride;"
    "func bcore_spect_fp:create_from_self;"
"}";

static bcore_matrix_s* bcore_matrix_s_create_from_self( const bcore_self_s* self )
{
    bcore_matrix_s* o = bcore_spect_create_from_self( bcore_flect_get_self( typeof( "bcore_matrix_s" ) ), self );
    o->spect_array = bcore_spect_get_typed( TYPEOF_bcore_array_s, self->type );

    if( o->cols_fix == 0 && o->cols_off == ( uz_t )-1 )
    {
        ERR_fa( "Feature 'cols' fixed-zero or no specified in '#<sc_t>'.", ifnameof( self->type ) );
    }

    if( o->rows_fix == 0 && o->rows_off == ( uz_t )-1 )
    {
        ERR_fa( "Feature 'rows' fixed-zero or no specified in '#<sc_t>'.", ifnameof( self->type ) );
    }

    return o;
}

/**********************************************************************************************************************/

void bcore_matrix_default_set_size( const bcore_matrix_s* p, bcore_matrix* o, uz_t rows, uz_t cols )
{
    if( p->rows_fix   > 0 && rows != p->rows_fix   ) ERR_fa( "Array has a fixed row-size of #<uz_t>"   , p->rows_fix );
    if( p->cols_fix   > 0 && cols != p->cols_fix   ) ERR_fa( "Array has a fixed column-size of #<uz_t>", p->cols_fix );
    if( p->stride_fix > 0 && cols != p->stride_fix ) ERR_fa( "Array has a fixed stride-size of #<uz_t>", p->stride_fix );
    bcore_array_p_set_size( p->spect_array, (bcore_array*)o, rows * cols );
    if( p->stride_off != ( uz_t )-1 ) *( uz_t* )BCORE_OFFSET( o, p->stride_off ) = cols;
    if( p->cols_off   != ( uz_t )-1 ) *( uz_t* )BCORE_OFFSET( o, p->cols_off   ) = cols;
    if( p->rows_off   != ( uz_t )-1 ) *( uz_t* )BCORE_OFFSET( o, p->rows_off   ) = rows;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_spect_matrix_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_matrix" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FFUNC( bcore_spect_fp_create_from_self, bcore_matrix_s_create_from_self );
            BCORE_REGISTER_SPECT( bcore_matrix );
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}
