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

identifier __func__;
identifier __FILE__;
identifier __LINE__;

//----------------------------------------------------------------------------------------------------------------------
/// interface functions

/// Returns number of errors on stack
func sz_t stack_size();

/// Removes all errors from stack
func void clear_stack();

/// Removes last error from stack (if any)
func void remove_last();

/// Removes last error from stack (if last error exists and is of given id )
func void remove_last_if_of( er_t id );

/// Pushes an error onto stack; returns id
func er_t push_sc( er_t id, sc_t msg );
func er_t push_fv( er_t id, sc_t format, va_list args );
func er_t push_fa( er_t id, sc_t format, ... );

/** (See also macro 'GERR_fa')
 *  'push_fa' with location details in parseable format including function name, file and line.
 *  Call with __func__, __FILE__, __LINE__
 *  Example: bcore_error_push_ffl_fa( general_error~, __func__, __FILE__, __LINE__, "Something wrong here." );
 */
func er_t push_ffl_fv( er_t id, sc_t func, sc_t file, sz_t line, sc_t format, va_list args );
func er_t push_ffl_fa( er_t id, sc_t func, sc_t file, sz_t line, sc_t format, ... );

/// 'push_ffl_fa' using general_error~ (See also macro 'GERR_fa')
func er_t push_gffl_fv( sc_t func, sc_t file, sz_t line, sc_t format, va_list args );
func er_t push_gffl_fa( sc_t func, sc_t file, sz_t line, sc_t format, ... );

/// returns last error id on stack; returns 0 in case of no error; does not change error stack
func er_t last();

/// Takes last error from stack; returns false if stack was empty
func bl_t pop_st( m er_t* id, m st_s* msg );
func bl_t pop_to_sink( m bcore_sink* sink );
func bl_t pop_to_stderr();

/// Takes all errors from stack; returns false if stack was empty
func bl_t pop_all_to_sink( m bcore_sink* sink );
func bl_t pop_all_to_stderr();
func bl_t pop_all();

func st pop_all_to_st( m st_s* st ) { :pop_all_to_sink( st ); = st; }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

group GERR
{
    set inexpandable;

    /// GERR_fa is a makro definition below; like ERR_fa but via bcore_error manager using generic_error
    func er_t fa( sc_t format, ... );
};

#endif // XOILA_SECTION

/**********************************************************************************************************************/
/// Macros

/// generic managed detailed error (like ERR_fa but via bcore_error manager using general_error)
#define GERR_fa( ... ) bcore_error_push_gffl_fa( __func__, __FILE__, __LINE__, __VA_ARGS__ )

/**********************************************************************************************************************/

vd_t bcore_error_manager_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BCORE_ERROR_MANAGER_H
