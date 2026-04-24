/** Author and Copyright 2025 Johannes Bernhard Steffens
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

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// External functions via btcl_function interface (see beth/data/bcore/btcl/selftest.btcl)

//----------------------------------------------------------------------------------------------------------------------

name add_a_b;
name add_a;

stamp :adder_s
{
    f3_t additive;
    func x_btcl.btcl_function_arity
    {
        switch( name )
        {
            case TYPEOF_add_a_b: = 2;
            case TYPEOF_add_a  : = 1;
            default: break;
        }
        = -1; // return -1 to indicate that a function of given name does not exist
    }

    func x_btcl.btcl_function
    {
        switch( name )
        {
            case TYPEOF_add_a_b: result.from_f3( o.additive + args.[0].to_f3() + args.[1].to_f3() ); break;
            case TYPEOF_add_a  : result.from_f3( o.additive + args.[0].to_f3() ); break;
            default: = sp.parse_error_fa( "Invalid function name '#<sc_t>'.", bnameof( name ) );
        }
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// Minimal implementation: simply calls a function of name run(), returns default value 0 (<f3_t>)
name run;
stamp :minimal_s
{
    func er_t run( @* o ) { bcore_msg_fa( "BTCL (#name): 'run' called\n", o._ ); = 0; }
    func x_btcl.btcl_function_arity = ( name == run~ ) ? 0 : -1;
    func x_btcl.btcl_function       = ( name == run~ ) ? o.run() : 0;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// External parser

//----------------------------------------------------------------------------------------------------------------------

stamp :parser_s
{
    // simply copy code section into a string
    func x_btcl.btcl_external_parse
    {
        m$* string = st_s!^;
        while( !source.eos() && ( source.inspect_char() != ')' ) )
        {
            string.push_char( source.get_char() );
        }
        result.asm( string.fork() );
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/** External generic functions
 *  An external generic function is defined by adding the keyword 'generic' to the function signature.
 *  Arguments and return values can have indirections 0 or 1.
 *  Return value:
 *     The btcl-return value is normally the same as the generic function's return value.
 */

//----------------------------------------------------------------------------------------------------------------------

stamp :generic_s
{
    f3_t value;

    /// If the return value is void or er_t, the mutable argument btcl_return is treated as return variable

    func f3_t add1( c@* o, f3_t value )              generic = o.value + value;
    func void add2( c@* o, f3_t value, m f3_t* btcl_return ) generic btcl_return.0 = o.value + value; // btcl_return out as last mutable element
    func f3_t mul_add( c@* o, f3_t mul, f3_t* add ) generic = o.value * mul + add.0;
    func f3_t* get_value_ptr( c@* o )               generic = o.value.1;
    func f3_t get_value( c@* o )                    generic = o.value.1;
    func d@* clone_me( c@* o )                      generic = o.clone();
    func d@* fork_me( m@* o )                       generic = o.fork();
    func o   get_me( c@* o )                        generic = o;
    func o   add_to_me1( m@* o, @* src )            generic { o.value += src.value; = o; }

    func void add_to_me2( m@* o, @* src, m@* btcl_return ) generic
    {
        o.value += src.value;
        btcl_return.copy( o );
    }

    func er_t add_to_me3( m@* o, @* src, m@* btcl_return ) generic
    {
        o.value += src.value;
        btcl_return.copy( o );
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

