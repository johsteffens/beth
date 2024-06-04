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
name EXP;
name LOG;
name LOG2;
name LOG10;
name SIN;
name COS;
name TAN;
name TANH;
name SIGN;
name SQRT;
name ABS;

// constants
name true;
name false;
name TRUE;
name FALSE;
name PI;

//----------------------------------------------------------------------------------------------------------------------

func (:context_s) set_reserved_funcs
{
    o.hmap_reserved_func.set_sc( "SIZE" );
    o.hmap_reserved_func.set_sc( "EXP" );
    o.hmap_reserved_func.set_sc( "LOG" );
    o.hmap_reserved_func.set_sc( "LOG2" );
    o.hmap_reserved_func.set_sc( "LOG10" );
    o.hmap_reserved_func.set_sc( "SIN" );
    o.hmap_reserved_func.set_sc( "COS" );
    o.hmap_reserved_func.set_sc( "TAN" );
    o.hmap_reserved_func.set_sc( "TANH" );
    o.hmap_reserved_func.set_sc( "SIGN" );
    o.hmap_reserved_func.set_sc( "SQRT" );
    o.hmap_reserved_func.set_sc( "ABS" );
}

//----------------------------------------------------------------------------------------------------------------------

func (:context_s) set_reserved_consts
{
    o.hmap_reserved_const.set_sc( "true" );
    o.hmap_reserved_const.set_sc( "false" );
    o.hmap_reserved_const.set_sc( "TRUE" );
    o.hmap_reserved_const.set_sc( "FALSE" );
    o.hmap_reserved_const.set_sc( "PI" );
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_reserved_const( m@* o, tp_t name, m x_source* source, m sr_s* sr )
{
    switch( name )
    {
        case TYPEOF_true : sr.const_from_bl(  true ); break;
        case TYPEOF_false: sr.const_from_bl( false ); break;
        case TYPEOF_TRUE : sr.const_from_bl(  true ); break;
        case TYPEOF_FALSE: sr.const_from_bl( false ); break;
        case TYPEOF_PI:    sr.const_from_f3( 3.1415926535897932384626434 ); break;

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

        case EXP~:
        {
            if( sb.is_numeric() )
            {
                sr.const_from_f3( exp( sb.to_f3() ) );
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", o.nameof( name ), bnameof( sb.o_type() ) );
            }
        }
        break;

        case LOG~:
        {
            if( sb.is_numeric() )
            {
                sr.const_from_f3( log( sb.to_f3() ) );
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", o.nameof( name ), bnameof( sb.o_type() ) );
            }
        }
        break;

        case LOG2~:
        {
            if( sb.is_numeric() )
            {
                sr.const_from_f3( log2( sb.to_f3() ) );
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", o.nameof( name ), bnameof( sb.o_type() ) );
            }
        }
        break;

        case LOG10~:
        {
            if( sb.is_numeric() )
            {
                sr.const_from_f3( log10( sb.to_f3() ) );
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", o.nameof( name ), bnameof( sb.o_type() ) );
            }
        }
        break;

        case SIN~:
        {
            if( sb.is_numeric() )
            {
                sr.const_from_f3( sin( sb.to_f3() ) );
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", o.nameof( name ), bnameof( sb.o_type() ) );
            }
        }
        break;

        case COS~:
        {
            if( sb.is_numeric() )
            {
                sr.const_from_f3( cos( sb.to_f3() ) );
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", o.nameof( name ), bnameof( sb.o_type() ) );
            }
        }
        break;

        case TAN~:
        {
            if( sb.is_numeric() )
            {
                sr.const_from_f3( tan( sb.to_f3() ) );
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", o.nameof( name ), bnameof( sb.o_type() ) );
            }
        }
        break;

        case TANH~:
        {
            if( sb.is_numeric() )
            {
                sr.const_from_f3( tanh( sb.to_f3() ) );
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", o.nameof( name ), bnameof( sb.o_type() ) );
            }
        }
        break;

        case SIGN~:
        {
            if( sb.is_numeric() )
            {
                sr.const_from_f3( ( sb.to_f3() >= 0 ) ? 1 : -1 );
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", o.nameof( name ), bnameof( sb.o_type() ) );
            }
        }
        break;

        case SQRT~:
        {
            if( sb.is_numeric() )
            {
                sr.const_from_f3( sqrt( sb.to_f3() ) );
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", o.nameof( name ), bnameof( sb.o_type() ) );
            }
        }
        break;

        case ABS~:
        {
            if( sb.is_numeric() )
            {
                if( sb.is_integer() )
                {
                    s3_t x = sb.to_s3();
                    sr.const_from_s3( ( x >= 0 ) ? x : -x );
                }
                else
                {
                    f3_t x = sb.to_f3();
                    sr.const_from_f3( ( x >= 0 ) ? x : -x );
                }
            }
            else
            {
                = source.parse_error_fa( "Function #<sc_t>: Argument '#<sc_t>' is not numeric.\n", o.nameof( name ), bnameof( sb.o_type() ) );
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
