/** Copyright 2019 Johannes Bernhard Steffens
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

#include "bcore_precoder_sample.h"

#ifdef TYPEOF_bcore_precoder_sample

//----------------------------------------------------------------------------------------------------------------------

void bcore_precoder_sample_object_s_setup( bcore_precoder_sample_object_s* o, sz_t n )
{
    st_s_copy_sc( &o->name1, "Franz" );
    for( sz_t i = 0; i < n; i++ ) bcore_array_a_push_sz( ( bcore_array* )o, i );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_precoder_sample_object_s_to_stdout( const bcore_precoder_sample_object_s* o )
{
    bcore_txt_ml_a_to_sink( o, BCORE_STDOUT );
}

//----------------------------------------------------------------------------------------------------------------------

sz_t bcore_precoder_sample_object_s_get_size( const bcore_precoder_sample_object_s* o )
{
    return o->arr_size;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_precoder_sample_object2_s_setup( bcore_precoder_sample_object2_s* o, sz_t n )
{
    st_s_copy_sc( &o->name2, "Paul" );
    bcore_precoder_sample_object_s_setup( &o->object, n );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_precoder_sample_object2_s_to_stdout( const bcore_precoder_sample_object2_s* o )
{
    bcore_txt_ml_a_to_sink( o, BCORE_STDOUT );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_precoder_sample_object_s_selftest( void )
{
    BCORE_LIFE_INIT();

    BCORE_LIFE_CREATE( bcore_precoder_sample_object_s, object1 );
    bcore_precoder_sample_features_a_setup( ( bcore_precoder_sample_features* )object1, 5 );
    bcore_precoder_sample_features_a_to_stdout( ( bcore_precoder_sample_features* )object1 );

    BCORE_LIFE_CREATE( bcore_precoder_sample_object2_s, object2 );
    bcore_precoder_sample_features_a_setup( ( bcore_precoder_sample_features* )object2, bcore_precoder_sample_features_a_get_size( ( bcore_precoder_sample_features* )object1 ) );
    bcore_precoder_sample_features_a_to_stdout( ( bcore_precoder_sample_features* )object2 );

    BCORE_LIFE_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bcore_precoder_sample_object_s

/**********************************************************************************************************************/

vd_t bcore_precoder_sample_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_precoder_sample" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
            bcore_precoder_sample_object_s_selftest();
        }
        break;

        case TYPEOF_precoder:
        {
            bcore_precoder_compile( "bcore_precoded", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

