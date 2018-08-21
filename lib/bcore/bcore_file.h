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

/**********************************************************************************************************************/

bl_t bcore_file_exists( sc_t name ); // checks if file exists
bl_t bcore_file_touch(  sc_t name ); // creates empty file if not existing; returns success (no effect if existing)
bl_t bcore_file_delete( sc_t name ); // deletes file if existing; returns success
bl_t bcore_file_rename( sc_t src_name, sc_t dst_name ); // renames file if existing; returns success

/**********************************************************************************************************************/

vd_t bcore_file_signal_handler( const bcore_signal_s* o );

#endif  // BCORE_FILE_H
