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

/// general purpose interactive shell for objects; returns 'true' in case a command was handled, 'false' otherwise
feature 'a' bl_t shell( m @* o, :frame_s* frame, m bcore_source* source, m bcore_sink* sink, m :control* control ) extern shell_default;

/// lists commands to sink (quick help)
feature 'a' void shell_help( @* o, m :helper* helper ) extern shell_help_default;

/// general purpose interactive shell loop for objects; returns 'true' in case a command was handled, 'false' otherwise
signature er_t shell_loop( m @* o, :frame_s* frame, m bcore_source* source, m bcore_sink* sink, m :control* control );

group :control =
{
    /// request to exit loop asap
    feature 'a' void request_exit_loop( m @* o );
    feature 'a' bl_t exit_loop( @* o );
    feature 'a' o reset( m @* o );
    stamp :s =
    {
        bl_t exit_loop;
        func :.reset             = { o.exit_loop = false; return o; };
        func :.request_exit_loop = { o.exit_loop = true; };
        func :.exit_loop         = { return o.exit_loop; };
    };
};

//----------------------------------------------------------------------------------------------------------------------

group :helper =
{
    feature 'a' void push( @* o, sc_t command, sc_t description );
    feature 'a' void to_sink( @* o, mutable bcore_sink* sink );

    stamp :item_s = { st_s command; st_s description; };
    stamp :items_s = x_array { :item_s => []; };
    stamp :s =
    {
        :items_s items;

        func (o setup( m @* o, bcore_main* main )) = { main.shell_help( o ); return o; };

        func :.push =
        {
            :item_s^ item;
            item.command.copy_sc( command );
            item.description.copy_sc( description );
            o.items.push_c( item );
        };

        func :.to_sink =
        {
            sz_t size_cmd = 0; foreach( $*e in o.items ) size_cmd = sz_max( size_cmd, e.command.size );
            foreach( $*e in o.items )
            {
                sink.push_fa( "#pn' '{#<st_s*>}   #<st_s*>\n", size_cmd, e.command.1, e.description.1 );
            }
        };
    };
};

//----------------------------------------------------------------------------------------------------------------------

func shell_loop =
{
    if( !control ) control = :control_s!.scope( control );

    while( !source.eos() )
    {
        sink.push_fa( "\n(#<sc_t>)> ", bnameof( o._ ) ).flush();
        source.parse_fa( " " );

        control.reset();

        if( o.shell( frame, source, sink, control ) )
        {
            if( control.exit_loop() ) break;
        }
        else if( source.parse_bl( "#?w'xo'" ) )
        {
            break;
        }
        else if( source.parse_bl( "#?w'exit'" ) )
        {
            break;
        }
        else if( source.parse_bl( "#?'?'" ) || source.parse_bl( "#?'help'" ) )
        {
            :helper_s!^.setup( o ).to_sink( sink );
        }
        else
        {
            source.parse_fa( "#-until'\n'" );
            sink.push_fa( "Invalid command.\n\n" );
            sink.push_fa( "List of available commands:\n" );
            :helper_s!^.setup( o ).to_sink( sink );
        }
    }

    return 0;
};

//----------------------------------------------------------------------------------------------------------------------

func shell_default =
{
    if( source.parse_bl( "#?w'ls'" ) )
    {
        source.parse_fa( "#skip' '" );

        bl_t extensive = source.parse_bl( "#?'-l'" );

        x_via_path_s^ path.parse( bcore_source_string_s_create_parse_line( source )^ );
        sr_s sr = path.get_sr_in( o );

        if( sr.o )
        {
            if( extensive )
            {
                x_inst_t_to_sink_txt_ml( sr_s_type( sr ), sr.o, sink );
            }
            else
            {
                tp_t t = sr_s_type( sr );
                x_via* v = sr.o;
                sz_t size = x_via_t_size( t );
                for( sz_t i = 0; i < size; i++ )
                {
                    tp_t name = x_via_t_name( t, i );
                    tp_t type = x_via_t_type( t, v, name );
                    sink.push_fa( "#<sc_t>: #<sc_t>\n", bnameof( name ), bnameof( type ) );
                }
            }
        }
        else
        {
            sink.push_fa( "Path not found.\n" );
        }
        return true;
    }
    else if( source.parse_bl( "#?w'eo'" ) )
    {
        source.parse_fa( "#skip' '" );

        x_via_path_s^ path.parse( bcore_source_string_s_create_parse_line( source )^ );
        sr_s sr = path.get_sr_in( o );

        if( sr.o )
        {
            tp_t t = sr_s_type( sr );
            if( !x_via_t_is_aware( t ) )
            {
                sink.push_fa( "Selected object '#<sc_t>' is oblivious.\n", bnameof( t ) );
                return true;
            }

            mutable bcore_main* mo = sr.o;
            mo.shell_loop( frame, source, sink, NULL );
        }
        else
        {
            sink.push_fa( "Path not found.\n" );
        }

        return true;
    }
    return false;
};

//----------------------------------------------------------------------------------------------------------------------

func shell_help_default =
{
    helper.push( "?, help",        "Prints this help message." );
    helper.push( "ls [-l] <path>", "Lists all visible members. -l: txt_ml to stdout" );
    helper.push( "eo <path>",      "(Enter Object): Enters interactive loop in specified object." );
    helper.push( "xo, exit",       st_s!^.push_fa( "(Exit Object):  Leaves interactive loop in '#<sc_t>'.", bnameof( o._ ) ).sc );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

