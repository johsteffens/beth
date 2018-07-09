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

#include "bmath_grt.h"

BCORE_DEFINE_OBJECT_FLAT( bcore_inst, bmath_grt_f3_s ) "{ f3_t c; f3_t s; }";

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_arr_grt_f3_s ) "{ aware_t _; bmath_grt_f3_s [] arr; }";

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_grt_f3_s_set_space( bmath_arr_grt_f3_s* o, sz_t space )
{
    bcore_array_a_set_space( ( bcore_array* )o, space );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_grt_f3_s_set_size(  bmath_arr_grt_f3_s* o, sz_t size )
{
    bcore_array_a_set_size( ( bcore_array* )o, size );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_grt_f3_s_zro( bmath_arr_grt_f3_s* o )
{
    bcore_u_memzero( sizeof( bmath_grt_f3_s ), o->data, o->size );
}

//---------------------------------------------------------------------------------------------------------------------

void bmath_arr_grt_f3_s_push( bmath_arr_grt_f3_s* o, bmath_grt_f3_s grt )
{
    if( o->space <  o->size ) bcore_array_a_make_strong( ( bcore_array *)o );
    if( o->space == o->size ) bmath_arr_grt_f3_s_set_space( o, sz_max( 1, o->size * 2 ) );
    o->data[ o->size++ ] = grt;
}

//---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bmath_grt_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_grt" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_grt_f3_s );
            BCORE_REGISTER_OBJECT( bmath_arr_grt_f3_s );
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

/**********************************************************************************************************************/

