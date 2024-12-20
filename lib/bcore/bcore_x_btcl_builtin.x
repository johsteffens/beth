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

/** BTCL: Beth text constructive language (interpreter) - Builtin Functions */

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
name CEIL;
name FLOOR;
name PRINT;
name PRINTX;

// constants
name true;
name false;
name TRUE;
name FALSE;
name PI;
name PATH;
name DIR;

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
    o.hmap_reserved_func.set_sc( "CEIL" );
    o.hmap_reserved_func.set_sc( "FLOOR" );
    o.hmap_reserved_func.set_sc( "PRINT" );
    o.hmap_reserved_func.set_sc( "PRINTX" );
}

//----------------------------------------------------------------------------------------------------------------------

func (:context_s) set_reserved_consts
{
    o.hmap_reserved_const.set_sc( "true" );
    o.hmap_reserved_const.set_sc( "false" );
    o.hmap_reserved_const.set_sc( "TRUE" );
    o.hmap_reserved_const.set_sc( "FALSE" );
    o.hmap_reserved_const.set_sc( "PI" );
    o.hmap_reserved_const.set_sc( "PATH" );
    o.hmap_reserved_const.set_sc( "DIR" );
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

        case TYPEOF_PATH:
        {
            sc_t path = source.get_file();
            if( !path )
            {
                = source.parse_error_fa( "Cannot retrieve a file path from source.\n", o.nameof( name ) );
            }
            sr.asc( st_s!.copy_sc( path ) );
        }
        break;

        case TYPEOF_DIR:
        {
            m st_s* dir = source.get_d_dir()^;
            if( !dir )
            {
                = source.parse_error_fa( "Cannot retrieve a directory from source.\n", o.nameof( name ) );
            }
            sr.asc( dir.fork() );
        }
        break;

        default:
        {
            = source.parse_error_fa( "Unknown reserved constant.\n", o.nameof( name ) );
        }
        break;
    }

    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func (:frame_s) er_t eval_reserved_func( m@* o, tp_t name, m x_source* source, bl_t postfix, m sr_s* sr )
{
    switch( name )
    {
        case SIZE~:  :export_eval_uop_type( o, size~,  0, source, postfix, sr ); break;
        case EXP~:   :export_eval_uop_type( o, exp~,   0, source, postfix, sr ); break;
        case LOG~:   :export_eval_uop_type( o, log~,   0, source, postfix, sr ); break;
        case LOG2~:  :export_eval_uop_type( o, log2~,  0, source, postfix, sr ); break;
        case LOG10~: :export_eval_uop_type( o, log10~, 0, source, postfix, sr ); break;
        case SIN~:   :export_eval_uop_type( o, sin~,   0, source, postfix, sr ); break;
        case COS~:   :export_eval_uop_type( o, cos~,   0, source, postfix, sr ); break;
        case TAN~:   :export_eval_uop_type( o, tan~,   0, source, postfix, sr ); break;
        case TANH~:  :export_eval_uop_type( o, tanh~,  0, source, postfix, sr ); break;
        case SIGN~:  :export_eval_uop_type( o, sign~,  0, source, postfix, sr ); break;
        case SQRT~:  :export_eval_uop_type( o, sqrt~,  0, source, postfix, sr ); break;
        case ABS~:   :export_eval_uop_type( o, abs~,   0, source, postfix, sr ); break;
        case CEIL~:  :export_eval_uop_type( o, ceil~,  0, source, postfix, sr ); break;
        case FLOOR~: :export_eval_uop_type( o, floor~, 0, source, postfix, sr ); break;

        case PRINT~:
        case PRINTX~:
        {
            m$* sb = sr_s!^;
            if( postfix )
            {
                sb.0 = sr.0;
                sr.0 = sr_null();
            }
            else
            {
                sr.clear();
            }

            o.eval( 0, source, sb );
            o.to_sink( ( name == PRINTX~ ), sb, x_sink_stdout() );
            sr.0 = sb.0;
            sb.0 = sr_null();
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
