/** Copyright 2017 Johannes Bernhard Steffens
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

#define NPX( name ) bcore_array_##name

static sc_t bcore_matrix_s_def = "bcore_matrix_s = spect"
"{"
    "bcore_spect_header_s header;"
    "private bcore_array_s* spect_array;"
    "feature sz_t fixed_cols = 0"
    "feature sz_t fixed_rows = 0"
    "feature sz_t * cols"
    "feature sz_t * rows"
"}";

static bcore_matrix_s* bcore_matrix_s_create_from_self( const bcore_self_s* self )
{
    bcore_matrix_s* o = bcore_spect_create_from_self( bcore_flect_get_self( typeof( "bcore_matrix_s" ) ), self );
    o->spect_array = bcore_spect_get_typed( TYPEOF_bcore_array_s, self->type );
    if( o->fixed_cols > 0 )
    {
        o->cols = &o->fixed_cols;
    }
    else if( o->cols == NULL )
    {
        ERR_fa( "Feature 'cols' no specified in '#<sc_t>'.", ifnameof( self->type ) );
    }

    if( o->fixed_rows > 0 )
    {
        o->rows = &o->fixed_rows;
    }
    else if( o->rows == NULL )
    {
        ERR_fa( "Feature 'rows' no specified in '#<sc_t>'.", ifnameof( self->type ) );
    }
    return o;
}

static bcore_self_s* bcore_matrix_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( bcore_matrix_s_def, sizeof( bcore_matrix_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_matrix_s_create_from_self, "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/
// signal

vd_t bcore_spect_matrix_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_matrix" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FLECT( bcore_matrix_s );
        }
        break;

        case TYPEOF_selftest:
        {
//            return spect_matrix_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}
