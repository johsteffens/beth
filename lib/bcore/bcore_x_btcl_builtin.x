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
name MAX;
name MIN;
name IFE;
name PRINT;
name PRINTLN;
name PRINTX;
name ASSERT;
name MKDIR;
name RMDIR;
name FILE_EXISTS;
name TO_FILE;
name TO_FILE_BTML;
name TO_FILE_BBML;
name TO_FILE_BCML;
name FROM_FILE;
name FROM_FILE_BTML;
name FROM_FILE_BBML;
name FROM_FILE_BCML;

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
    o.hmap_reserved_func.set_sc( "MAX" );
    o.hmap_reserved_func.set_sc( "MIN" );
    o.hmap_reserved_func.set_sc( "IFE" );
    o.hmap_reserved_func.set_sc( "PRINT" );
    o.hmap_reserved_func.set_sc( "PRINTLN" );
    o.hmap_reserved_func.set_sc( "PRINTX" );
    o.hmap_reserved_func.set_sc( "ASSERT" );
    o.hmap_reserved_func.set_sc( "MKDIR" );
    o.hmap_reserved_func.set_sc( "RMDIR" );
    o.hmap_reserved_func.set_sc( "FILE_EXISTS" );
    o.hmap_reserved_func.set_sc( "TO_FILE" );
    o.hmap_reserved_func.set_sc( "TO_FILE_BTML" );
    o.hmap_reserved_func.set_sc( "TO_FILE_BBML" );
    o.hmap_reserved_func.set_sc( "TO_FILE_BCML" );
    o.hmap_reserved_func.set_sc( "FROM_FILE" );
    o.hmap_reserved_func.set_sc( "FROM_FILE_BTML" );
    o.hmap_reserved_func.set_sc( "FROM_FILE_BBML" );
    o.hmap_reserved_func.set_sc( "FROM_FILE_BCML" );
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
            if( dir.size == 0 ) dir.push_fa( "." );
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
        case SIZE~:  :operator_eval_uop_type( o, size~,  0, source, postfix, sr ); break;
        case EXP~:   :operator_eval_uop_type( o, exp~,   0, source, postfix, sr ); break;
        case LOG~:   :operator_eval_uop_type( o, log~,   0, source, postfix, sr ); break;
        case LOG2~:  :operator_eval_uop_type( o, log2~,  0, source, postfix, sr ); break;
        case LOG10~: :operator_eval_uop_type( o, log10~, 0, source, postfix, sr ); break;
        case SIN~:   :operator_eval_uop_type( o, sin~,   0, source, postfix, sr ); break;
        case COS~:   :operator_eval_uop_type( o, cos~,   0, source, postfix, sr ); break;
        case TAN~:   :operator_eval_uop_type( o, tan~,   0, source, postfix, sr ); break;
        case TANH~:  :operator_eval_uop_type( o, tanh~,  0, source, postfix, sr ); break;
        case SIGN~:  :operator_eval_uop_type( o, sign~,  0, source, postfix, sr ); break;
        case SQRT~:  :operator_eval_uop_type( o, sqrt~,  0, source, postfix, sr ); break;
        case ABS~:   :operator_eval_uop_type( o, abs~,   0, source, postfix, sr ); break;
        case CEIL~:  :operator_eval_uop_type( o, ceil~,  0, source, postfix, sr ); break;
        case FLOOR~: :operator_eval_uop_type( o, floor~, 0, source, postfix, sr ); break;

        case MAX~:
        {
            sr_s^ sa;
            if( postfix )
            {
                sa.twc( sr.type(), sr.o );
            }
            else
            {
                o.eval( 0, source, sa );
            }
            source.parse_fa( " , " );
            :operator_eval_bop_type( o, max~, 0, source, sa, sr );
        }
        break;

        case MIN~:
        {
            sr_s^ sa;
            if( postfix )
            {
                sa.twc( sr.type(), sr.o );
            }
            else
            {
                o.eval( 0, source, sa );
            }
            source.parse_fa( " , " );
            :operator_eval_bop_type( o, min~, 0, source, sa, sr );
        }
        break;

        case IFE~:
        {
            sr_s^ sa;
            if( postfix )
            {
                sa.twc( sr.type(), sr.o );
            }
            else
            {
                o.eval( 0, source, sa );
            }
            source.parse_fa( " , " );

            sr_s^ sb;
            o.eval( 0, source, sb );

            source.parse_fa( " , " );

            sr_s^ sc;
            o.eval( 0, source, sc );


            tp_t op_type = conditional~;
            m$* top = :operator_top_s!^( op_type, sa, sb, sc, x_source_point_s!^( source ) );
            bl_t success = false;
            top.solve( o, sr, success );
            if( !success )
            {
                = source.parse_error_fa( "Operator IFE( #<sc_t>, #<sc_t>, #<sc_t> ) is not defined.\n", bnameof( sa.type() ), bnameof( sb.type() ), bnameof( sc.type() ) );
            }
        }
        break;

        case PRINT~:
        case PRINTLN~:
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

            m$* st = st_s!^;
            o.to_sink( ( name == PRINTX~ ), sb, st );
            if( name == PRINTLN~ )
            {
                if( st.size == 0 || st.[ st.size - 1 ] != '\n' ) st.push_char( '\n' );
            }
            st.to_stdout();

            sr.0 = sb.0;
            sb.0 = sr_null();
        }
        break;

        case ASSERT~:
        {
            m$* sb = sr_s!^;
            o.eval( 0, source, sb );

            if( !sb.is_numeric() ) = source.parse_error_fa( "Expression does not represent a condition.\n" );
            if( !sb.to_bl()      ) = source.parse_error_fa( "Assertion failed.\n" );
            sr.from_bl( true );
        }
        break;

        case MKDIR~:
        {
            m$* sb = sr_s!^;
            o.eval( 0, source, sb );
            if( sb.type() != st_s~ ) = source.parse_error_fa( "Expression must represent a string.\n" );
            sr.from_bl( bcore_folder_create( sb.o.cast( st_s* ).sc ) );
        }
        break;

        case RMDIR~:
        {
            m$* sb = sr_s!^;
            o.eval( 0, source, sb );
            if( sb.type() != st_s~ ) = source.parse_error_fa( "Expression must represent a string.\n" );
            sr.from_bl( bcore_folder_delete( sb.o.cast( st_s* ).sc ) );
        }
        break;

        case FILE_EXISTS~:
        {
            m$* sb = sr_s!^;
            o.eval( 0, source, sb );
            if( sb.type() != st_s~ ) = source.parse_error_fa( "Expression must represent a string.\n" );
            sr.from_bl( bcore_file_exists( sb.o.cast( st_s* ).sc ) );
        }
        break;

        case TO_FILE~:
        case TO_FILE_BTML~:
        {
            m$* sa = sr_s!^;
            o.eval( 0, source, sa );
            if( sa.type() != st_s~ ) = source.parse_error_fa( "First argument must represent a string.\n" );

            source.parse_fa( " , " );

            m$* sb = sr_s!^;
            o.eval( 0, source, sb );
            bl_t success = false;

            if( sb.o )
            {
                x_btml_t_to_file( sb.o.cast( x_btml* ), sb.type(), sa.o.cast( st_s* ).sc );
                success = true;
            }

            sr.from_bl( success );
        }
        break;

        case TO_FILE_BBML~:
        {
            m$* sa = sr_s!^;
            o.eval( 0, source, sa );
            if( sa.type() != st_s~ ) = source.parse_error_fa( "First argument must represent a string.\n" );

            source.parse_fa( " , " );

            m$* sb = sr_s!^;
            o.eval( 0, source, sb );
            bl_t success = false;

            if( sb.o )
            {
                x_bbml_t_to_file( sb.o.cast( x_btml* ), sb.type(), sa.o.cast( st_s* ).sc );
                success = true;
            }

            sr.from_bl( success );
        }
        break;

        case TO_FILE_BCML~:
        {
            m$* sa = sr_s!^;
            o.eval( 0, source, sa );
            if( sa.type() != st_s~ ) = source.parse_error_fa( "First argument must represent a string.\n" );

            source.parse_fa( " , " );

            m$* sb = sr_s!^;
            o.eval( 0, source, sb );
            bl_t success = false;

            if( sb.o )
            {
                x_bcml_t_to_file( sb.o.cast( x_btml* ), sb.type(), sa.o.cast( st_s* ).sc );
                success = true;
            }

            sr.from_bl( success );
        }
        break;

        case FROM_FILE~:
        {
            m$* sa = sr_s!^;
            o.eval( 0, source, sa );
            if( sa.type() != st_s~ ) = source.parse_error_fa( "First argument must represent a string.\n" );

            source.parse_fa( " , " );
            o.eval( 0, source, sr );

            m$* file_source = x_source_create_from_file( sa.o.cast( st_s* ).sc )^;

            if( file_source )
            {
                tp_t type = 0;
                m x_inst* obj = x_btcl_create_from_source_t( file_source, type ).cast( d x_inst* )^;
                if( obj ) sr.tsm( type, obj.fork() );
            }
        }
        break;

        case FROM_FILE_BTML~:
        {
            m$* sa = sr_s!^;
            o.eval( 0, source, sa );
            if( sa.type() != st_s~ ) = source.parse_error_fa( "First argument must represent a string.\n" );
            source.parse_fa( " , " );
            o.eval( 0, source, sr );

            m$* file_source = x_source_create_from_file( sa.o.cast( st_s* ).sc )^;

            if( file_source )
            {
                tp_t type = 0;
                m x_inst* obj = x_btml_create_from_source_t( file_source, type ).cast( d x_inst* )^;
                if( obj ) sr.tsm( type, obj.fork() );
            }
        }
        break;

        case FROM_FILE_BBML~:
        {
            m$* sa = sr_s!^;
            o.eval( 0, source, sa );
            if( sa.type() != st_s~ ) = source.parse_error_fa( "First argument must represent a string.\n" );
            source.parse_fa( " , " );
            o.eval( 0, source, sr );

            m$* file_source = x_source_create_from_file( sa.o.cast( st_s* ).sc )^;
            if( file_source )
            {
                tp_t type = 0;
                m x_inst* obj = x_bbml_create_from_source_t( file_source, type ).cast( d x_inst* )^;
                if( obj ) sr.tsm( type, obj.fork() );
            }
        }
        break;

        case FROM_FILE_BCML~:
        {
            m$* sa = sr_s!^;
            o.eval( 0, source, sa );
            if( sa.type() != st_s~ ) = source.parse_error_fa( "First argument must represent a string.\n" );
            source.parse_fa( " , " );
            o.eval( 0, source, sr );

            m$* file_source = x_source_create_from_file( sa.o.cast( st_s* ).sc )^;
            if( file_source )
            {
                tp_t type = 0;
                m x_inst* obj = x_bcml_create_from_source_t( file_source, type ).cast( d x_inst* )^;
                if( obj ) sr.tsm( type, obj.fork() );
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
