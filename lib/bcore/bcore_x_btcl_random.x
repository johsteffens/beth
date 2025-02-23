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

/** External object generating a list of random numbers.
 *  BTCL Example:
 *  // list of 10 random numbers within [-1.0, 1.0].
 *  list = <x_btcl_random/>( .seed = 1234, min = -1.0, max = 1.0 ).list( 10 );
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

name list;
stamp :s
{
    bcore_prsg => prsg;
    u3_t seed = 1234;
    f3_t min = 0.0;
    f3_t max = 1.0;

    func o fill_list( @* o, sz_t size, m x_btcl_list_s* list )
    {
        list.set_size( size );
        m bcore_prsg* prsg = o.prsg.clone()^;
        if( !prsg ) prsg = bcore_prsg_lcg_u3_00_s!^^;

        prsg.set_state_u3( o.seed );

        for( sz_t i = 0; i < size; i++ )
        {
            list.arr.[ i ].from_f3( prsg.gen_f3( o.min, o.max ) );
        }
    }

    func x_btcl.btcl_function_arity
    {
        switch( name )
        {
            case TYPEOF_list: = 1;
            default: break;
        }
        = -1; // return -1 to indicate that a function of given name does not exist
    }

    func x_btcl.btcl_function
    {
        switch( name )
        {
            case TYPEOF_list:
            {
                m $* list = x_btcl_list_s!^;
                o.fill_list( sz_max( 0, args.[0].to_f3() ), list );
                result.asm( list.fork() );
                = 0;
            }

            default: = sp.parse_error_fa( "Invalid function name '#<sc_t>'.", bnameof( name ) );
        }
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

