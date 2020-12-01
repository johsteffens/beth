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

/** File operations */

#ifndef BCORE_FILE_H
#define BCORE_FILE_H

#include "bcore_control.h"
#include "bcore_signal.h"
#include "bcore.xo.h"
#include "bcore_spect_via_call.h"
#include "bcore_xoila.h"

BCORE_FORWARD_OBJECT( bcore_source );
BCORE_FORWARD_OBJECT( bcore_sink );

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_file
XOILA_DEFINE_GROUP( bcore_file, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

signature sc_t get_sc( const );
signature void set_sc( mutable, sc_t name );

/** File path that is automatically extended if relative.
 *  Use function get_sc to obtain the full path.
 *  This object implements via_call:source, which is called during interpreting a source (e.g. object is read from a file).
 *  via_call:source provides bcore_path_s with a stream source from which a file location is obtained.
 */
stamp : path = aware bcore_inst
{
    st_s name; // absolute or relative path ( e.g. "../obj/myobj.txt" );
    hidden st_s => root; // root folder
    hidden st_s => full; // full path: source_folder / rel
    func bcore_via_call . source;
    func              : . get_sc = { return o->full ? o->full->sc : o->name.sc; };
    func              : . set_sc =
    {
        o.full =< NULL;
        o.name.copy_sc( name );
        if( name[ 0 ] != '/' && o->root != NULL )
        {
            o->full = st_s_create_fa( "#<sc_t>/#<sc_t>", o->root->sc, o->name.sc );
        }
    };

    func( @* create_sc( sc_t sc ) );
    func( @* create_st( const st_s* st ) );
};

/**********************************************************************************************************************/
/// file path manipulations using posix path scheme

/** Creates a string returning 'path' with redundancies removed.
 *  Redundancies are expressions that can be replaced by a shorter expression
 *  without changing the actual path.
 *  Examples:
 *    - repeating slashes: '<expr1>//<expr2>'         -> '<expr1>/<expr2>'
 *    - current directory: '<expr1>/./<expr2>'        -> '<expr1>/<expr2>'
 *    - parent directory:  '<expr1>/<dir>/../<expr2>' -> '<expr1>/<expr2>'
 */
func( st_s* path_minimized( sc_t path ) );

/** Returns extension of path excluding '.'
 *  If path has no extension, returns ""
 */
func( sc_t extension( sc_t path ) );

/** Returns file-name of path, including extension, excluding preceding folders.
 *  If path has no preceding folder, returns path
 */
func( sc_t name( sc_t path ) );

/** Returns string 'path' with extension removed.
 *  If path has no extension, returns path as is.
 *  Example: "mydir/myfile.cpp" -> "mydir/myfile"
 */
func( st_s* strip_extension( sc_t path ) );

/** Creates string with path up to last folder, excluding file-name.
 *  If path has no preceding folder, the function returns ""
 *  Example: "myroot/mydir/myfile.cpp" -> "myroot/mydir"
 *  Example: "myroot/mydir"            -> "myroot"
 */
func( st_s* folder_path( sc_t path ) );

/** Creates string with last folder name, excluding file-name.
 *  If path has no preceding folder, returns ""
 */
func( st_s* folder_name( sc_t path ) );

/**********************************************************************************************************************/

func( bl_t exists( sc_t name ) ); // checks if file exists
func( bl_t touch(  sc_t name ) ); // creates empty file if not existing; returns success (no effect if existing)
func( bl_t delete( sc_t name ) ); // deletes file if existing; returns success
func( bl_t rename( sc_t src_name, sc_t dst_name ) ); // renames file if existing; returns success

/** Searches for a file <folder>/<name>, descending the tree through parent folders of <folder>.
 *  Returns false if not found.
 *  If found, the detected path is stored in <result>.
 *
 *  Example:
 *    bcore_file_find_descend( "/home/john/proj/proj1", "module.cfg", result );
 *     Searches for module.cfg in
 *       /home/john/proj/proj1/module.cfg
 *       /home/john/proj/module.cfg
 *       /home/john/module.cfg
 *       /home/module.cfg
 *       /module.cfg
 *
 *    If any matches, result holds the first matching path (e.g. "/home/module.cfg")
 *    and 'true' is returned. Otherwise 'false' is returned.
 */
func( bl_t find_descend( sc_t folder, sc_t name, st_s* result ) );

/// opens file-source (close it with bcore_source_a_discard)
func( bcore_source* open_source( sc_t name ) );
func( bcore_source* open_source_path( const bcore_file_path_s* path ) );

/// opens file-sink (close it with bcore_sink_a_discard)
func( bcore_sink* open_sink( sc_t name ) );

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif // TYPEOF_bcore_file

/**********************************************************************************************************************/

vd_t bcore_file_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_FILE_H
