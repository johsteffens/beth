/** Author and Copyright 2021 Johannes Bernhard Steffens
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

func void help( m x_sink* sink )
{
    sink.push_sc
    (
        "Xoila Compiler: (C) J.B.Steffens\n"
        "Usage:\n"
        "$ xoico [options] xoico <xoico-config-file> [<xoico-config-file> ...]   #normal operation\n"
        "$ xoico --help                                                          #displays this text\n"
        "\n"
        "Options:\n"
        "-d : Dry run. Target files are not modified.\n"
        "-e : Always expand: Expands a target even if its hash value has not changed.\n"
        "-f : Force overwrite target files. Use with care.\n"
        "-O <folder>: Output folder for this target and all dependencies.\n"
    );
};

//----------------------------------------------------------------------------------------------------------------------

func x_inst.main
{
    xoico_builder_main_s^ builder_main;
    er_t er = 0;

    sz_t arg_idx = 1;

    if( args.size > 1 )
    {
        if( args.[ arg_idx ].equal_sc( "--help" ) )
        {
            :help( x_sink_stdout() );
        }
        else
        {
            while( arg_idx < args.size && args.[ arg_idx ].sc[ 0 ] == '-' )
            {
                if( args.[ arg_idx ].equal_sc( "-d" ) )
                {
                    builder_main.set_dry_run( true );
                }
                else if( args.[ arg_idx ].equal_sc( "-e" ) )
                {
                    builder_main.set_always_expand( true );
                }
                else if( args.[ arg_idx ].equal_sc( "-f" ) )
                {
                    builder_main.set_overwrite_unsigned_target_files( true );
                }
                else if( args.[ arg_idx ].equal_sc( "-O" ) )
                {
                    arg_idx++;
                    if( args.size == arg_idx ) ERR_fa( "Output folder expected.\n" );
                    builder_main.output_folder =< args.[ arg_idx ].clone();
                }
                else
                {
                    ERR_fa( "Invalid option: #<sc_t>\n", args.[ arg_idx ].sc );
                }
                arg_idx++;
            }

            if( builder_main.get_dry_run() )
            {
                x_sink_stdout().push_fa( "Dry run ...\n" );
            }

            if( builder_main.get_always_expand() )
            {
                x_sink_stdout().push_fa( "Expanding all ...\n" );
            }

            if( arg_idx >= args.size ) :help( x_sink_stdout() );
            clock_t time = clock();

            for( sz_t i = arg_idx; i < args.size; i++ )
            {
                if( ( er = builder_main.build_from_file( args.[ i ].sc ) ) ) break;
            }

            if( !er && builder_main.update_required() )
            {
                er = builder_main.update();
            }

            if( !er ) er = builder_main.set_build_timestamp();

            if( !er )
            {
                builder_main.set_build_timestamp();
                f3_t time_var = clock() - time;
                time_var /= CLOCKS_PER_SEC;
                x_sink_stdout().push_st_d( st_s_createf( "Finished after %.3f seconds.\n", time_var ) );
            }
        }
    }
    else
    {
        er = 1;
        st_s^ st;
        :help( st );
        bcore_error_push_sc( er, st.sc );
    }

    if( er ) bcore_error_pop_all_to_stderr();

    return ( er > 0 ) ? 1 : 0;
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

