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

#ifndef BCORE_FOLDER_H
#define BCORE_FOLDER_H

#include "bcore_control.h"
#include "bcore_signal.h"
#include "bcore_st.h"
#include "bcore_arr.h"
#include "bcore_flect.h"
#include "bcore_spect_array.h"

/**********************************************************************************************************************/

bl_t bcore_folder_exists( sc_t name ); // checks if folder exists
bl_t bcore_folder_create( sc_t name ); // creates new folder in mode 0700 if not existing; returns true if success or existing
bl_t bcore_folder_delete( sc_t name ); // deletes empty folder if existing; returns true if success or folder did not exist in the first place
bl_t bcore_folder_rename( sc_t src_name, sc_t dst_name ); // renames folder if existing; returns success

/**********************************************************************************************************************/
/** bcore_folder_s represents a full directory tree with files and sub-folders
 *  use bcore_folder_s_parse to setup from a given root-folder
 */
BCORE_DECLARE_OBJECT( bcore_folder_s )
{
    aware_t _;
    st_s name;                                        // full path
    BCORE_ARRAY_DYN_LINK_STATIC_S( bcore_folder_s, ); // sub solders
    bcore_arr_st_s arr_file;                          // files in folder
};

void bcore_folder_s_clear( bcore_folder_s* o );

/** Sets up object by parsing file system from root folder 'path'.
 *  Returns false in case 'path' does not exist or is no folder.
 *  recurse: true: recurse into sub folders.
 */
bl_t bcore_folder_s_parse( bcore_folder_s* o, sc_t path, bl_t recurse );

/**********************************************************************************************************************/

vd_t bcore_folder_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_FOLDER_H
