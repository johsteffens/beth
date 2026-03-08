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

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

#include "bcore_folder.h"
#include "bcore_sources.h"
#include "bcore_sinks.h"
#include "bcore_spect_inst.h"
#include "bcore_life.h"
#include "bcore_sc.h"
#include "bcore_file.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_folder_exists( sc_t path )
{
    if( sc_t_equal( path, ""  ) ) return true;
    if( sc_t_equal( path, "/" ) )  return true;
    if( sc_t_equal( path, "." ) )  return true;

    struct stat file_stat;
    if( stat( path, &file_stat ) != 0 ) return false;
    if( !S_ISDIR( file_stat.st_mode ) ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_folder_create( sc_t path, bl_t recursive, sc_t smode )
{
    if( bcore_folder_exists( path ) ) return true;

    if( recursive )
    {
        st_s* parent_path = bcore_file_folder_path( path );
        bl_t success = bcore_folder_create( parent_path->sc, true, smode );
        st_s_discard( parent_path );
        if( !success ) return false;
    }

    s2_t nmode = 0;
    sc_t smask = "rwxrwxrwx";
    if( smode )
    {
        for( sz_t i = 0; smask[ i ] != 0; i++ )
        {
            char c = smode[ i ];
            if( c != '-' && c != smask[ i ] ) { ERR_fa( "Invalid mode string #<sc_t>\n", smode ); return false; }
            nmode = ( nmode << 1 ) | ( ( c == smask[ i ] ) ? 1 : 0 );
        }
    }
    else
    {
        nmode = 0700;
    }

    if( mkdir( path, nmode ) != 0 ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_folder_delete( sc_t path )
{
    if( !bcore_folder_exists( path ) ) return true;
    if( rmdir( path ) != 0 ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_folder_rename( sc_t src_path, sc_t dst_path )
{
    if( !bcore_folder_exists( src_path ) ) return false;
    if( rename( src_path, dst_path ) != 0 ) return false;
    return true;
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_folder_get_current( st_s* path )
{
    if( !path ) return path;
    char *get_current_dir_name( void ); // prototype not defined in unistd.h
    sd_t sd = get_current_dir_name();
    ASSERT( sd );
    st_s_copy_sc( path, sd );
    free( sd );
    return path;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_folder_s )
"{"
    "aware_t _;"
    "st_s name;"
    "bcore_folder_s => [] arr;" // sub folders
    "bcore_arr_st_s arr_file;"  // files in folder
"}";

//----------------------------------------------------------------------------------------------------------------------

bcore_folder_s* bcore_folder_s_clear( bcore_folder_s* o )
{
    st_s_clear( &o->name );
    bcore_array_a_set_size( ( bcore_array* )o, 0 );
    bcore_arr_st_s_clear( &o->arr_file );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_folder_s_parse( bcore_folder_s* o, sc_t path, bl_t recurse )
{
    BLM_INIT();
    bcore_folder_s_clear( o );

    if( !bcore_folder_exists( path ) ) BLM_RETURNV( bl_t, false );

    struct dirent* entry;
    struct stat file_stat;

    DIR* dirp = opendir( path );
    if( !dirp ) BLM_RETURNV( bl_t, false );

    st_s_copy_sc( &o->name, path );

    st_s* full_path = BLM_CREATE( st_s );

    while( ( entry = readdir( dirp ) ) )
    {
        if( !sc_t_equ( entry->d_name, "." ) && !sc_t_equ( entry->d_name, ".." ) )
        {
            st_s_copy_fa( full_path, "#<sc_t>/#<sc_t>", path, entry->d_name );
            if( stat( full_path->sc, &file_stat ) == 0 )
            {
                if( S_ISREG( file_stat.st_mode ) ) // regular file
                {
                    bcore_arr_st_s_push_sc( &o->arr_file, full_path->sc );
                }
                else if( recurse && S_ISDIR( file_stat.st_mode ) ) // folder
                {
                    bcore_folder_s* sub_folder = bcore_folder_s_create();
                    if( bcore_folder_s_parse( sub_folder, full_path->sc, recurse ) )
                    {
                        bcore_array_a_push( ( bcore_array* )o, sr_asm( sub_folder ) );
                    }
                    else
                    {
                        bcore_folder_s_discard( sub_folder );
                    }
                }
            }
        }
    }

    if( closedir( dirp ) ) ERR_fa( "Error closing path '#<sc_t>'.", path );

    BLM_RETURNV( bl_t, true );
}

//----------------------------------------------------------------------------------------------------------------------

bcore_folder_s* bcore_folder_s_setup( bcore_folder_s* o, sc_t path, bl_t recurse )
{
    bcore_folder_s_parse( o, path, recurse );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_folder_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_folder" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bcore_folder_s );
        }
        break;

        case TYPEOF_get_quicktypes:
        {
            BCORE_REGISTER_QUICKTYPE( bcore_folder_s );
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

