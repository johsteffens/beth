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

/** Folder operations */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

group bcore_folder
{
    // checks if folder exists
    func bl_t exists( sc_t path );

    /** creates new folder if not existing; returns true if success or already existing
     *  smode: "rwx"-access flags for user, group, other: [uuugggooo]; e.g.: "rwxr--r--"
     *  recursive: true creates all parent folders if not existing, false: creates only final folder.
     */
    func bl_t create( sc_t path, bl_t recursive, sc_t smode );

    // deletes empty folder if existing; returns true if success or folder did not exist in the first place
    func bl_t delete( sc_t path );

    // renames folder if existing; returns success
    func bl_t rename( sc_t src_path, sc_t dst_path ); // renames folder if existing; returns success

    // obtains current folder string and stores it in o
    func o get_current( m st_s* o );

    //------------------------------------------------------------------------------------------------------------------

    stamp :s x_array
    {
        st_s name; // full path
        bcore_folder_s => []; // sub solders
        bcore_arr_st_s arr_file; // files in this folder

        /// Clears the object's content (does not delete any file or folder)
        func o clear( m@* o );

        /** Sets up this object by parsing file system from root folder 'path'.
         *  In case 'path' does not exist or is no folder o is simply cleared.
         *  if recurse == true: builds the full tree of all sub folders.
         */
        func o setup( m@* o, sc_t path, bl_t recurse );

        /** Sets up this object by parsing file system from root folder 'path'.
         *  Returns false in case 'path' does not exist or is no folder.
         *  if recurse == true: builds the full tree of all sub folders.
         */
        func bl_t parse( m@* o, sc_t path, bl_t recurse );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
