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

/** File operations */

#ifndef BCORE_FILE_H
#define BCORE_FILE_H

#include "bcore_control.h"
#include "bcore_signal.h"
#include "bcore_plant_compiler.h"
#include "bcore_planted.h"
#include "bcore_spect_via_call.h"

BCORE_FORWARD_OBJECT( bcore_source );
BCORE_FORWARD_OBJECT( bcore_sink );

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_file
PLANT_GROUP( bcore_file, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

signature sc_t get_sc( const );
signature void set_sc( mutable, sc_t name );

/** File path that is automatically extended if relative.
 *  Implements via_call:source to get the root folder during interpreting a source file.
 */
stamp : path = aware bcore_inst
{
    st_s name; // absolute or relative path ( e.g. "../obj/myobj.txt" ); if relative
    hidden st_s => root; // root folder
    hidden st_s => full; // full path: source_folder / rel
    func bcore_via_call : source;
    func              : : get_sc = { return o->full ? o->full->sc : o->name.sc; };
    func              : : set_sc =
    {
        st_s_detach( &o->full );
        st_s_copy_sc( &o->name, name );
        if( name[ 0 ] != '/' && o->root != NULL )
        {
            o->full = st_s_create_fa( "#<sc_t>/#<sc_t>", o->root->sc, o->name.sc );
        }
    };
};

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif // TYPEOF_bcore_file

bcore_file_path_s* bcore_file_path_s_create_sc( sc_t sc );
bcore_file_path_s* bcore_file_path_s_create_st( const st_s* st );

/**********************************************************************************************************************/
/// file path manipulations using posix path scheme

/** Returns extension of path excluding '.'
 *  If path has no extension, returns ""
 */
sc_t  bcore_file_extension( sc_t path );

/** Returns file-name of path, including extension, excluding preceding folders.
 *  If path has no preceding folder, returns path
 */
sc_t  bcore_file_name( sc_t path );

/** Returns string 'path' with extension removed
 *  If path has no extension, returns path as is.
 *  path can also be just a file name
 */
st_s* bcore_file_strip_extension( sc_t path ); // path with extension (if any) removed

/** Creates string with path up to last folder, excluding file-name.
 *  If path has no preceding folder, returns ""
 */
st_s* bcore_file_folder_path( sc_t path ); // creates string with path of folder

/** Creates string with last folder name, excluding file-name.
 *  If path has no preceding folder, returns ""
 */
st_s* bcore_file_folder_name( sc_t path ); // creates string with name of folder

/**********************************************************************************************************************/

bl_t bcore_file_exists( sc_t name ); // checks if file exists
bl_t bcore_file_touch(  sc_t name ); // creates empty file if not existing; returns success (no effect if existing)
bl_t bcore_file_delete( sc_t name ); // deletes file if existing; returns success
bl_t bcore_file_rename( sc_t src_name, sc_t dst_name ); // renames file if existing; returns success

/// opens file-source (close it with bcore_source_a_discard)
bcore_source* bcore_file_open_source( sc_t name );
bcore_source* bcore_file_open_source_path( const bcore_file_path_s* path );

/// opens file-sink   (close it with bcore_sink_a_discard)
bcore_sink*   bcore_file_open_sink(   sc_t name );

/**********************************************************************************************************************/

vd_t bcore_file_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_FILE_H
