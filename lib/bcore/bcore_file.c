/** Author and Copyright 2018 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/
/// bcore_file_path_s

//----------------------------------------------------------------------------------------------------------------------

void bcore_file_path_s_source( bcore_file_path_s* o, bcore_source* source )
{
    sc_t file = bcore_source_a_get_file( source );
    st_s_detach( &o->full );
    st_s_detach( &o->root );
    if( file )
    {
        o->root = bcore_file_folder_name( file );
        if( o->root && o->name.size > 0 && o->name.sc[ 0 ] != '/' )
        {
            o->full = st_s_create_fa( "#<sc_t>/#<sc_t>", o->root->sc, o->name.sc );
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

bcore_file_path_s* bcore_file_path_s_create_sc( sc_t sc )
{
    bcore_file_path_s* o = bcore_file_path_s_create();
    st_s_copy_sc( &o->name, sc );
    return o;
}

//----------------------------------------------------------------------------------------------------------------------

bcore_file_path_s* bcore_file_path_s_create_st( const st_s* st )
{
    return bcore_file_path_s_create_sc( st->sc );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

sc_t bcore_file_extension( sc_t path )
{
    st_s* s = st_s_create_sc( path );
    sz_t idx = st_s_find_char( s, -1, 0, '.' );
    st_s_discard( s );
    return path + idx;
}

//----------------------------------------------------------------------------------------------------------------------

sc_t bcore_file_name( sc_t path )
{
    st_s* s = st_s_create_sc( path );
    sz_t idx = st_s_find_char( s, -1, 0, '/' );
    st_s_discard( s );
    if( path[ idx ] == '/' ) return path + idx + 1;
    return path;
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_file_strip_extension( sc_t path )
{
    st_s* s = st_s_create_sc( path );
    sz_t idx = st_s_find_char( s, -1, 0, '.' );
    if( s->data[ idx ] == '.' )
    {
        s->data[ idx ] = 0;
        s->size = idx;
    }
    return s;
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_file_folder_path( sc_t path )
{
    st_s* s = st_s_create_sc( path );
    sz_t idx = st_s_find_char( s, -1, 0, '/' );
    if( s->data[ idx ] == '/' )
    {
        s->data[ idx ] = 0;
        s->size = idx;
    }
    else
    {
        s->data[ 0 ] = 0;
        s->size = 0;
    }
    return s;
}

//----------------------------------------------------------------------------------------------------------------------

st_s* bcore_file_folder_name( sc_t path )
{
    st_s* s1 = bcore_file_folder_path( path );
    st_s* s2 = st_s_create_sc( bcore_file_name( s1->sc ) );
    st_s_discard( s1 );
    return s2;

}

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

bcore_source* bcore_file_open_source_path( const bcore_file_path_s* path )
{
    return bcore_file_open_source( bcore_file_path_s_get_sc( path ) );
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

        case TYPEOF_plant:
        {
            bcore_plant_compile( "bcore_planted", __FILE__ );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

