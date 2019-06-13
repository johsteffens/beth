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

#include "bcore_main.h"
#include "bcore_std.h"

//----------------------------------------------------------------------------------------------------------------------

s2_t bcore_main_frame_s_main( bcore_main_frame_s* o, sz_t argc, char** argv )
{
    bcore_arr_st_s* args = bcore_arr_st_s_create();
    for( sz_t i = 0; i < argc; i++ ) bcore_arr_st_s_push_sc( args, argv[ i ] );
    s2_t ret = -1;
    sc_t file = NULL;
    if( o->use_first_argument && args->size > 1 )
    {
        file = args->data[ 1 ]->sc;
        if( !bcore_file_exists( file ) ) file = NULL;
    }

    if( !file && o->local_file )
    {
        file = o->local_file;
        if( !bcore_file_exists( file ) ) file = NULL;
    }

    if( !file && o->global_file )
    {
        file = o->global_file;
        if( !bcore_file_exists( file ) ) file = NULL;
    }

    if( file )
    {
        sr_s sr_object = bcore_interpret_x( sr_awd( o->interpreter ), sr_asd( bcore_file_open_source( file ) ) );
        ret = bcore_main_r_main( &sr_object, args );
        sr_down( sr_object );
    }

    bcore_arr_st_s_discard( args );
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bcore_main_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_main" ) ) )
    {
        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_selftest:
        {
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

