/** Author and Copyright 2019 Johannes Bernhard Steffens
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

#include "bcore_plant_builder.h"
#include "bcore_plant_compiler.h"
#include "bcore_file.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bcore_plant_builder

//----------------------------------------------------------------------------------------------------------------------
// globals

static bcore_arr_st_s* arr_path_g = NULL;
static bcore_mutex_s* mutex_g = NULL;

//----------------------------------------------------------------------------------------------------------------------

static sz_t build_from_file( sc_t path );

sz_t bcore_plant_builder_target_s_build( const bcore_plant_builder_target_s* o )
{
    BLM_INIT();
    bcore_arr_sz_s* dependencies = BLM_CREATE( bcore_arr_sz_s );
    BFOR_EACH( i, &o->dependencies )
    {
        BLM_INIT();
        st_s* file_path = BLM_CREATE( st_s );
        if( o->dependencies.data[ i ]->sc[ 0 ] != '/' )
        {
            if( o->root ) st_s_push_fa( file_path, "#<sc_t>/", o->root->sc );
        }
        st_s_push_fa( file_path, "#<sc_t>", o->dependencies.data[ i ]->sc );
        sz_t target_index = build_from_file( file_path->sc );
        if( target_index >= 0 )
        {
            bcore_arr_sz_s_push( dependencies, target_index );
        }
        BLM_DOWN();
    }

    sz_t target_index = -1;

    if( !o->name && o->sources.size > 0 )
    {
        ERR_fa( "Plant name is missing." );
    }

    BFOR_EACH( i, &o->sources )
    {
        BLM_INIT();

        st_s* file_path = BLM_CREATE( st_s );
        if( o->sources.data[ i ]->sc[ 0 ] != '/' )
        {
            if( o->root ) st_s_push_fa( file_path, "#<sc_t>/", o->root->sc );
        }
        st_s_push_fa( file_path, "#<sc_t>", o->sources.data[ i ]->sc );

        ASSERT( o->name );

        if( st_s_count_sc( o->name, 0, -1, "_planted" ) > 0 )
        {
            ERR_fa( "Plant name is '#<sc_t>'. Substring '_planted' should not be part of the name.", o->name->sc );
        }

        st_s* planted_name = BLM_A_PUSH( st_s_create_fa( "#<sc_t>_planted", o->name->sc ) );

        sz_t index = bcore_plant_compiler_compile( planted_name->sc, file_path->sc );
        target_index = ( target_index == -1 ) ? index : target_index;
        if( index != target_index )
        {
            ERR_fa
            (
                "Building source '#<sc_t>' in plant '#<sc_t>':\n"
                "Target index mismatch.\n"
                "This problem occurs when a target defines sources from different directories.\n",
                o->sources.data[ i ]->sc,
                o->name->sc
            );
        }

        BLM_DOWN();
    }

    if( target_index >= 0 )
    {
        bcore_plant_compiler_set_target_dependencies( target_index, dependencies );
        st_s* signal_handler = BLM_A_PUSH( st_s_create_fa( "#<sc_t>_general_signal_handler", o->name->sc ) );
        if( o->signal_handler ) st_s_copy( signal_handler, o->signal_handler );
        bcore_plant_compiler_set_target_signal_handler_name( target_index, signal_handler->sc );
    }

    BLM_RETURNV( sz_t, target_index );
}

//----------------------------------------------------------------------------------------------------------------------

static sz_t build_from_file( sc_t path )
{
    BLM_INIT();
    st_s* st_path = BLM_A_PUSH( bcore_file_path_minimized( path ) );

    if( !arr_path_g ) arr_path_g = bcore_arr_st_s_create();

    bl_t new_path = true;

    BFOR_EACH( i, arr_path_g )
    {
        if( st_s_equal_st( arr_path_g->data[ i ], st_path ) )
        {
            new_path = false;
            break;
        }
    }

    sz_t target_index = -1;

    if( new_path )
    {
        bcore_arr_st_s_push_st( arr_path_g, st_path );

        if( bcore_plant_compiler_get_verbosity() > 0 )
        {
            bcore_msg_fa( "BETH_PLANT: building #<sc_t>\n", st_path->sc );
        }

        bcore_plant_builder* builder = BLM_A_PUSH( bcore_txt_ml_from_file( st_path->sc ).o );
        target_index = bcore_plant_builder_a_build( builder );
    }

    BLM_RETURNV( sz_t, target_index );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// bcore_plant_builder interface functions

//----------------------------------------------------------------------------------------------------------------------

void bcore_plant_builder_build_from_file( sc_t path )
{
    bcore_mutex_s_lock( mutex_g );
    build_from_file( path );
    bcore_mutex_s_unlock( mutex_g );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_plant_builder_build_from_rel_file( sc_t root_path, sc_t path )
{
    BLM_INIT();
    bcore_plant_builder_build_from_file( st_s_push_fa( BLM_A_PUSH( bcore_file_folder_path( root_path ) ), "/#<sc_t>", path )->sc );
    BLM_DOWN();
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_plant_builder_update_required( void )
{
    bcore_mutex_s_lock( mutex_g );
    bl_t retv = bcore_plant_compiler_update_required();
    bcore_mutex_s_unlock( mutex_g );
    return retv;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_plant_builder_update( void )
{
    bcore_mutex_s_lock( mutex_g );
    bcore_arr_st_s_detach( &arr_path_g );
    bl_t retv = bcore_plant_compiler_update_planted_files();
    bcore_mutex_s_unlock( mutex_g );
    return retv;
}

//----------------------------------------------------------------------------------------------------------------------

#endif // TYPEOF_bcore_plant_builder

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

vd_t bcore_plant_builder_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_plant_builder" ) ) )
    {
        case TYPEOF_init1:
        {
            mutex_g = bcore_mutex_s_create();
        }
        break;

        case TYPEOF_down1:
        {
            bcore_arr_st_s_detach( &arr_path_g );
            bcore_mutex_s_detach( &mutex_g );
        }
        break;

        default: break;
    }

    return NULL;
}

/**********************************************************************************************************************/

