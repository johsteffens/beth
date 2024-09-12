/** Author and Copyright 2020 Johannes Bernhard Steffens
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

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp :s = aware :
{
    hidden aware xoico_group_s* group;

    // source file represents plain data to be embedded in target
    bcore_arr_u0_s => arr;
    tp_t embed_method; // embed plain data as string function
    tp_t function_name;

    func xoico.parse
    {
        o.arr =< bcore_arr_u0_s!;
        while( !source.eos() ) o.arr.push( source.get_u0() );
        return 0;
    };

    func xoico.get_hash
    {
        tp_t hash = bcore_tp_fold_tp( bcore_tp_init(), o._ );
        hash = bcore_tp_fold_tp( hash, o.embed_method );
        hash = bcore_tp_fold_tp( hash, o.function_name );
        for( sz_t i = 0; i < o.arr.size; i++ ) hash = bcore_tp_fold_tp( hash, o.arr.data[ i ] );
        return hash;
    };

    func xoico.expand_declaration
    {
        if( o.embed_method == TYPEOF_as_string )
        {
//            sink.push_fa( " \\\n#rn{ }", indent );
//            sink.push_fa( "st_s* #<sc_t>( void );", host.nameof( o.function_name ) );
        }
        = 0;
    }

    func xoico.expand_definition
    {
        if( o.embed_method == TYPEOF_as_string )
        {
            sink.push_fa( "#rn{ }st_s* #<sc_t>( void )\n", indent, host.nameof( o.function_name ) );
            sink.push_fa( "#rn{ }{\n"                            , indent );

            sink.push_fa( "#rn{ }    sc_t sc[] =\n", indent );
            sink.push_fa( "#rn{ }    {\n" , indent );
            sink.push_fa( "#rn{ }        \"" , indent );

            sz_t lines = 0;

            for( sz_t i = 0; i < o.arr.size; i++ )
            {
                u0_t c = o.arr.[ i ];
                switch( c )
                {
                    case '\\': sink.push_sc( "\\\\" ); break;
                    case '"' : sink.push_sc( "\\\"" ); break;
                    case '\n':
                    {
                        if( i + 1 < o.arr.size )
                        {
                            sink.push_fa( "\\n\",\n" );
                            sink.push_fa( "#rn{ }        \"" , indent );
                            lines++;
                        }
                    }
                    break;

                    default: sink.push_char( c );  break;
                }
            }
            sink.push_fa( "\\n\"\n" );
            sink.push_fa( "#rn{ }    };\n", indent );
            lines++;

            sink.push_fa( "#rn{ }    st_s* st = st_s_create();\n", indent );
            sink.push_fa( "#rn{ }    for( sz_t i = 0; i < #<sz_t>; i++ )\n", indent, lines );
            sink.push_fa( "#rn{ }    {\n", indent );
            sink.push_fa( "#rn{ }        st_s_push_sc( st, sc[ i ] );\n", indent );
            sink.push_fa( "#rn{ }    }\n", indent );
            sink.push_fa( "#rn{ }    return st;\n", indent );
            sink.push_fa( "#rn{ }}\n", indent );
        }

        = 0;
    };

};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
