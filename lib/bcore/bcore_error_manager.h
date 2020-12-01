/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#ifndef BCORE_ERROR_MANAGER_H
#define BCORE_ERROR_MANAGER_H

#include "bcore_xoila.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore.xo.h"
#include "bcore_trait.h"
#include "bcore_spect_array.h"

/** Dynamic error management.
 *  Maintains a global list of for error (-conditions) and
 *  thread safe functions for pushing and removing errors.
 */

#ifdef BETH_EXPAND_GROUP_bcore_error_manager

XOILA_DEFINE_GROUP( bcore_error_manager, bcore_inst )
#ifdef XOILA_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp :error = aware :
{
    er_t id;
    st_s msg; // error message
};

stamp :error_adl = aware bcore_array
{
    :error_s => [];
};

stamp :context = aware :
{
    :error_adl_s adl;
    bcore_mutex_s mutex;
};

/// frequently used error states
name general_error; // general purpose error
name parse_error; // errors from text/string parsing
name plant_error; // errors from beth plant management
name error_stack; // error stack is not zero (usually a subsequent error for functions that require an empty error stack)

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // XOILA_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // BETH_EXPAND_GROUP_bcore_error_manager

//----------------------------------------------------------------------------------------------------------------------
// bcore_error_manager interface functions

/// Returns number of errors on stack
sz_t bcore_error_stack_size( void );

/// Removes all errors from stack
void bcore_error_clear_stack( void );

/// Pushes an error onto stack; returns id
er_t bcore_error_push_sc( er_t id, sc_t msg );
er_t bcore_error_push_fv( er_t id, sc_t format, va_list args );
er_t bcore_error_push_fa( er_t id, sc_t format, ... );

/// Takes last error from stack; returns false if stack was empty
bl_t bcore_error_pop_st( er_t* id, st_s* msg );
bl_t bcore_error_pop_to_sink( bcore_sink* sink );
bl_t bcore_error_pop_to_stderr( void );

/// Takes all errors from stack
void bcore_error_pop_all_to_sink( bcore_sink* sink );
void bcore_error_pop_all_to_stderr( void );

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_error_manager_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_ERROR_MANAGER_H
