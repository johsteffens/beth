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

#include <stdio.h>

#include "bcore_file.h"
#include "bcore_sources.h"
#include "bcore_sinks.h"

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_file_exists( sc_t name )
{
    vd_t handle = fopen( name, "rb" );
    if( !handle ) return false;
    fclose( handle );
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_file_touch( sc_t name )
{
    if( bcore_file_exists( name ) ) return true;
    vd_t handle = fopen( name, "wb" );
    if( !handle ) return false;
    fclose( handle );
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_file_delete( sc_t name )
{
    return remove( name ) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_file_rename( sc_t src_name, sc_t dst_name )
{
    return rename( src_name, dst_name ) == 0;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_source* bcore_file_open_source( sc_t name )
{
    return ( bcore_source* )bcore_source_open_file( name );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_sink* bcore_file_open_sink( sc_t name )
{
    return ( bcore_sink* )bcore_sink_open_file( name );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t bcore_file_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_file" ) ) )
    {
        case TYPEOF_init1:
        {
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

