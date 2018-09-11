/** Copyright 2018 Johannes Bernhard Steffens
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

#include <stdlib.h>

#include "bcore_file.h"
#include "bmath_plot.h"

/**********************************************************************************************************************/
// bmath_plot_s

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_plot_s )
"{"
    "aware_t _;"
    "sr_s data;" // plot data  (vf3, arr_vf3)
    "st_s title;"
    "st_s x_label;"
    "st_s y_label;"
    "uz_t x_index;"
    "bl_t cols_over_x = false;"  // true: plot column values over x; only for arr_vf3
    "bcore_arr_st_s y_data_label;" // a row is a curve in the plot
"}";

//----------------------------------------------------------------------------------------------------------------------

s2_t bmath_plot_s_call_gnuplot( const bmath_plot_s* o, sc_t data_folder )
{
    BCORE_LIFE_INIT();

    if( !data_folder ) data_folder = ".";

    ASSERT( o->data.o );

    BCORE_LIFE_CREATE( st_s, data_file );
    BCORE_LIFE_CREATE( st_s, title );
    st_s_copy( title, &o->title );
    st_s_set_lowercase( title );
    st_s_replace_char_sc( title, ' ', "" );
    st_s_replace_char_sc( title, '\t', "" );
    st_s_replace_char_sc( title, '\n', "" );
    st_s_push_fa( data_file, "#<sc_t>/__plotdata__#<sc_t>.dat", data_folder, title->sc );

    BCORE_LIFE_CREATE( st_s, syscommand );
    st_s_push_fa( syscommand, "gnuplot -p -e \"" );

    if( o->title.size > 0 )
    {
        st_s* st = st_s_create();
        st_s_copy( st, &o->title );
        st_s_replace_char_sc( st, '\'', "\\'" );
        st_s_replace_char_sc( st, '\"', "\\\"" );
        st_s_push_fa( syscommand, "set title '#<st_s*>';", st );
        st_s_discard( st );
    }

    if( o->x_label.size > 0 )
    {
        st_s* st = st_s_create();
        st_s_copy( st, &o->x_label );
        st_s_replace_char_sc( st, '\'', "\\'" );
        st_s_replace_char_sc( st, '\"', "\\\"" );
        st_s_push_fa( syscommand, "set xlabel '#<st_s*>';", st );
        st_s_discard( st );
    }

    if( o->y_label.size > 0 )
    {
        st_s* st = st_s_create();
        st_s_copy( st, &o->y_label );
        st_s_replace_char_sc( st, '\'', "\\'" );
        st_s_replace_char_sc( st, '\"', "\\\"" );
        st_s_push_fa( syscommand, "set ylabel '#<st_s*>';", st );
        st_s_discard( st );
    }

    /// grid
    st_s_push_fa( syscommand, "set grid xtics ytics;" );


    st_s_push_sc( syscommand, "plot " );

    if( bcore_file_exists( data_file->sc ) ) ERR_fa( "File '#<sc_t>' exists. Remove it manually.", data_file->sc );
    bcore_sink* sink = ( bcore_sink* )bcore_sink_create_file( data_file->sc );

    if( sr_s_type( &o->data ) == TYPEOF_bmath_arr_vf3_s )
    {
        const bmath_arr_vf3_s* arr = o->data.o;
        if( arr->size > 0 )
        {
            if( o->cols_over_x )
            {
                bl_t finished = false;
                for( uz_t idx = 0; !finished; idx++ )
                {
                    finished = true;
                    if( o->x_index < arr->size )
                    {
                        if( idx < arr->data[ o->x_index ].size )
                        {
                            bcore_sink_a_push_fa( sink, "#<f3_t>", arr->data[ o->x_index ].data[ idx ] );
                            finished = false;
                        }
                    }
                    else
                    {
                        bcore_sink_a_push_fa( sink, "#<uz_t>", idx );
                    }


                    for( uz_t i = 0; i < arr->size; i++ )
                    {
                        bcore_sink_a_push_fa( sink, "\t" );
                        if( o->x_index != i )
                        {
                            if( idx < arr->data[ i ].size )
                            {
                                bcore_sink_a_push_fa( sink, "#<f3_t>", arr->data[ i ].data[ idx ] );
                                finished = false;
                            }
                        }
                    }
                    bcore_sink_a_push_fa( sink, "\n" );
                    idx++;
                }
                uz_t n = arr->size + ( o->x_index >= arr->size );
                for( uz_t i = 1; i < n; i++ )
                {
                    st_s_push_fa( syscommand, "'#<sc_t>' using 1:#<uz_t> with line", data_file->sc, i + 1 );
                    if( i <= o->y_data_label.size )
                    {
                        st_s_push_fa( syscommand, " title '#<sc_t>'", o->y_data_label.data[ i - 1 ]->sc );
                    }
                    st_s_push_fa( syscommand, "," );
                }
            }
            else
            {
                for( uz_t i = 0; i < arr->size; i++ )
                {
                    const bmath_vf3_s* v = &arr->data[ i ];
                    if( o->x_index < v->size )
                    {
                        bcore_sink_a_push_fa( sink, "#<f3_t>", v->data[ o->x_index ] );
                    }
                    else
                    {
                        bcore_sink_a_push_fa( sink, "#<uz_t>", i );
                    }

                    for( uz_t i = 0; i < v->size; i++ )
                    {
                        if( i != o->x_index )
                        {
                            bcore_sink_a_push_fa( sink, "\t" );
                            bcore_sink_a_push_fa( sink, "#<f3_t>", v->data[ i ] );
                        }
                    }
                    bcore_sink_a_push_fa( sink, "\n" );
                }
                uz_t n = arr->data[ 0 ].size + ( o->x_index >= arr->data[ 0 ].size );
                for( uz_t i = 1; i < n; i++ )
                {
                    st_s_push_fa( syscommand, "'#<sc_t>' using 1:#<uz_t> with line", data_file->sc, i + 1 );
                    if( i <= o->y_data_label.size )
                    {
                        st_s_push_fa( syscommand, " title '#<sc_t>'", o->y_data_label.data[ i - 1 ]->sc );
                    }
                    st_s_push_fa( syscommand, "," );
                }
            }
        }
    }
    else if( sr_s_type( &o->data ) == TYPEOF_bmath_vf3_s )
    {
        const bmath_vf3_s* vec = o->data.o;
        for( uz_t i = 0; i < vec->size; i++ )
        {
            bcore_sink_a_push_fa( sink, "#<uz_t>\t#<f3_t>\n", i, vec->data[ i ] );
        }
        st_s_push_fa( syscommand, "'#<sc_t>' using 1:2 with line", data_file->sc );
        if( o->y_data_label.size > 0 )
        {
            st_s_push_fa( syscommand, " title '#<sc_t>'", o->y_data_label.data[ 0 ]->sc );
        }
        st_s_push_fa( syscommand, "," );
    }
    else
    {
        ERR_fa( "Cannot handle data of type '#<sc_t>'", ifnameof( sr_s_type( &o->data ) ) );
    }
    bcore_inst_a_discard( sink );

    st_s_push_sc( syscommand, ";" );
    st_s_push_sc( syscommand, "\"" );

//    bcore_msg_fa( "#<sc_t>\n", syscommand->sc );

    s2_t ret = system( syscommand->sc );

    bcore_file_delete( data_file->sc );

    BCORE_LIFE_DOWN();

    return ret;
}

/**********************************************************************************************************************/

vd_t bmath_plot_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_plot" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_OBJECT( bmath_plot_s );
        }
        break;

        case TYPEOF_selftest:
        {
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

