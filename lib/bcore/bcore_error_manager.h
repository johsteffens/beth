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
XOILA_DEFINE_GROUP( bcore_error, bcore_inst )
#ifdef XOILA_SECTION

include 'c' "bcore_x_array.h";

group :manager =
{
    stamp :error_s = aware :
    {
        er_t id;
        st_s msg; // error message
    };

    stamp :error_adl_s = aware x_array
    {
        :error_s => [];
        func x_array.set_size
        {
            return x_array_set_size( o, size );
        };

        func x_array.clear
        {
            return x_array_clear( o );
        };

        func x_array.push_d
        {
            return x_array_push_d( o, v );
        };
    };

    stamp :context_s = aware :
    {
        :error_adl_s adl;
        bcore_mutex_s mutex;
    };
};


/// frequently used error states
name general_error; // general purpose error
name conversion_error; // error in type conversion (created by 'copy_typed' and related functions)
name parse_error; // errors from text/string parsing
name plant_error; // errors from beth plant management
name error_stack; // error stack is not zero (usually a subsequent error for functions that require an empty error stack)

//----------------------------------------------------------------------------------------------------------------------
/// interface functions

/// Returns number of errors on stack
func sz_t stack_size();

/// Removes all errors from stack
func void clear_stack();

/// Removes last error from stack (if any)
func void remove_last();

/// Pushes an error onto stack; returns id
func er_t push_sc( er_t id, sc_t msg );
func er_t push_fv( er_t id, sc_t format, va_list args );
func er_t push_fa( er_t id, sc_t format, ... );

/// returns last error id on stack; returns 0 in case of no error; does not change error stack
func er_t last();

/// Takes last error from stack; returns false if stack was empty
func bl_t pop_st( m er_t* id, m st_s* msg );
func bl_t pop_to_sink( m bcore_sink* sink );
func bl_t pop_to_stderr();

/// Takes all errors from stack; returns false in case stack was empty
func bl_t pop_all_to_sink( m bcore_sink* sink );
func bl_t pop_all_to_stderr();
func bl_t pop_all();

func st pop_all_to_st( m st_s* st ) { :pop_all_to_sink( st ); = st; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

#endif // XOILA_SECTION

/**********************************************************************************************************************/

vd_t bcore_error_manager_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_ERROR_MANAGER_H
