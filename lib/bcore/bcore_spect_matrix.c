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

/*
static sc_t bcore_matrix_s_def = "bcore_matrix_s = spect"
"{"
    "bcore_spect_header_s header;"
    "private bcore_array_s* spect_array;"
"}";

static bcore_matrix_s* bcore_matrix_s_create_from_self( const bcore_self_s* self )
{
    assert( self != NULL );
    bcore_matrix_s* o = matrix_s_create();
    o->p_type = bcore_name_enroll( "bcore_matrix_s" );
    o->o_type = self->type;


    return o;
}

static bcore_self_s* bcore_matrix_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( bcore_matrix_s_def, sizeof( bcore_matrix_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_matrix_s_create_from_self, "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

*/
