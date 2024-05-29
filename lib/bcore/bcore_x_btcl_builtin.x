/** Author and Copyright 2024 Johannes Bernhard Steffens
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

/** BTCL: Beth text constructive language (interpreter) - Binary Operators */

/**********************************************************************************************************************/
/// builtin constants and functions

// functions
name SIZE;

// constants
name TRUE;
name FALSE;

//----------------------------------------------------------------------------------------------------------------------

func (:context_s) set_reserved_funcs
{
    o.hmap_reserved_func.set_sc( "SIZE" );
}

//----------------------------------------------------------------------------------------------------------------------

func (:context_s) set_reserved_consts
{
    o.hmap_reserved_const.set_sc( "TRUE" );
    o.hmap_reserved_const.set_sc( "FALSE" );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_reserved_const( m@* o, tp_t name, m x_source* source, m sr_s* sr )
{
    switch( name )
    {
        case TRUE~ : sr.const_from_bl(  true ); break;
        case FALSE~: sr.const_from_bl( false ); break;

        default:
        {
            = source.parse_error_fa( "Unknown reserved constant.\n", o.nameof( name ) );
        }
        break;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_reserved_func( m@* o, tp_t name, m x_source* source, m sr_s* sr )
{
    m$* sb = sr_s!^;
    sb.0 = sr.0;
    sr.0 = sr_null();

    if( !sb.o ) o.eval( 0, source, sb );

    switch( name )
    {
        case SIZE~:
        {
            if( sb.o_type() == :list_s~ )
            {
                sr.const_from_s3( sb.o.cast( :list_s* ).arr.size );
            }
            else if( x_array_t_is_array( sb.o_type() ) )
            {
                sr.const_from_s3( x_array_t_size( sb.o, sb.o_type() ) );
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: #<sc_t> is no array.\n", o.nameof( name ), bnameof( sb.o_type() ) );
            }
        }
        break;

        default:
        {
            = source.parse_error_fa( "Unknown reserved function.\n", o.nameof( name ) );
        }
        break;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
