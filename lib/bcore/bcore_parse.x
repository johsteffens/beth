/** Author and Copyright 2026 Johannes Bernhard Steffens
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

/** Specialized parse functions */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** Parses a number from source considering all integer, hex and float encodings and SI metric symbols as suffix.
 *  Creates f3_t or s3_t depending on syntax; makes sr constant
 *  See also https://github.com/johsteffens/beth/blob/master/doc/bcore/btcl.md#si-metric-symbols-for-literals
 */
func er_t number_literal_to_sr( m x_source* source, m sr_s* sr )
{
    bl_t is_hex = false;
    bl_t is_float = false;
    st_s^ st;

    if( source.parse_bl( "#?'0x'" ) )
    {
        st.push_sc( "0x" );
        is_hex = true;
    }
    else if( source.parse_bl( "#?'0X'" ) )
    {
        st.push_sc( "0X" );
        is_hex = true;
    }

    if( is_hex )
    {
        while( source.parse_bl( "#?(([0]>='0'&&[0]<='9')||([0]>='a'&&[0]<='f')||([0]>='A'&&[0]<='F'))" ) )
        {
            st.push_char( source.get_char() );
        }
    }
    else // decimal
    {
        while( source.parse_bl( "#?([0]>='0'&&[0]<='9')" ) ) st.push_char( source.get_char() );
    }

    if( !is_hex && source.parse_bl( "#?([0]=='.')" ) )
    {
        st.push_char( source.get_char() );
        while( source.parse_bl( "#?([0]>='0'&&[0]<='9')" ) ) st.push_char( source.get_char() );
        is_float = true;
    }

    bl_t exponent = false;

    if( !is_hex && source.parse_bl( "#?([0]=='e'||[0]=='E')" ) )
    {
        st.push_char( source.get_char() );
        exponent = true;
        is_float = true;
    }

    if( exponent )
    {
        if( source.parse_bl( "#?([0]=='+'||[0]=='-')" ) )
        {
            st.push_char( source.get_char() );
        }

        while( source.parse_bl( "#?([0]>='0'&&[0]<='9')" ) )
        {
            st.push_char( source.get_char() );
        }
    }

    f3_t factor = 1.0;
    bl_t use_suffix = true;

    char c = source.inspect_char();
    switch( c )
    {
        case 'd': factor = 1E-1;  break; // deci
        case 'c': factor = 1E-2;  break; // centi
        case 'm': factor = 1E-3;  break; // milli
        case 'u': factor = 1E-6;  break; // micro
        case 'n': factor = 1E-9;  break; // nano
        case 'p': factor = 1E-12; break; // pico
        case 'f': factor = 1E-15; break; // femto
        case 'a': factor = 1E-18; break; // atto
        case 'z': factor = 1E-21; break; // zepto
        case 'y': factor = 1E-24; break; // yocto
        case 'r': factor = 1E-27; break; // ronto
        case 'q': factor = 1E-30; break; // quecto

        case 'D': factor = 1E+1;  break; // deca
        case 'C': factor = 1E+2;  break; // cento
        case 'K': factor = 1E+3;  break; // kilo
        case 'M': factor = 1E+6;  break; // mega
        case 'G': factor = 1E+9;  break; // giga
        case 'T': factor = 1E+12; break; // tera
        case 'P': factor = 1E+15; break; // peta
        case 'X': factor = 1E+18; break; // exa
        case 'Z': factor = 1E+21; break; // zetta
        case 'Y': factor = 1E+24; break; // yotta
        case 'R': factor = 1E+27; break; // ronna
        case 'Q': factor = 1E+30; break; // quetta

        default: use_suffix = false; break;
    }

    if( use_suffix ) source.get_char();

    if( is_float )
    {
        f3_t f3 = 0;
        st.parse_fa( 0, -1, "#<f3_t*>", f3.1 );
        sr.const_from_f3( f3 * factor );
    }
    else
    {
        s3_t s3 = 0;
        st.parse_fa( 0, -1, "#<s3_t*>", s3.1 );
        if( use_suffix )
        {
            sr.const_from_f3( s3 * factor );
        }
        else
        {
            sr.const_from_s3( s3 );
        }
    }

    = 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

