/** Author and Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_error_manager.h"
#include "bcore_sinks.h"
#include "bcore_life.h"

static bcore_error_manager_context_s* context_g = NULL;

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

static void create_context()
{
    if( !context_g )
    {
        context_g = bcore_error_manager_context_s_create();
    }
}

//----------------------------------------------------------------------------------------------------------------------

static bcore_error_manager_context_s* get_context()
{
    static bcore_once_s flag = bcore_once_init;
    bcore_once_s_run( &flag, create_context );
    return context_g;
}

//----------------------------------------------------------------------------------------------------------------------

static sz_t context_s_stack_size( bcore_error_manager_context_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    sz_t size = o->adl.size;
    bcore_mutex_s_unlock( &o->mutex );
    return size;
}

//----------------------------------------------------------------------------------------------------------------------

static void context_s_clear_stack( bcore_error_manager_context_s* o )
{
    bcore_mutex_s_lock( &o->mutex );
    bcore_error_manager_error_adl_s_clear( &o->adl );
    bcore_mutex_s_unlock( &o->mutex );
}

//----------------------------------------------------------------------------------------------------------------------

static void context_s_push_sc( bcore_error_manager_context_s* o, er_t id, sc_t msg )
{
    ASSERT( id != 0 );
    bcore_mutex_s_lock( &o->mutex );
    bcore_error_manager_error_s* error = bcore_error_manager_error_s_create();
    error->id = id;
    st_s_copy_sc( &error->msg, msg );
    bcore_error_manager_error_adl_s_push_d( &o->adl, error );
    bcore_mutex_s_unlock( &o->mutex );
}

//----------------------------------------------------------------------------------------------------------------------

static bl_t context_s_pop_st( bcore_error_manager_context_s* o, er_t* id, st_s* msg )
{
    bcore_mutex_s_lock( &o->mutex );
    bl_t ret = false;

    if( o->adl.size > 0 )
    {
        const bcore_error_manager_error_s* error = o->adl.data[ o->adl.size - 1 ];
        if( id ) *id = error->id;
        if( msg ) st_s_copy( msg, &error->msg );
        bcore_error_manager_error_adl_s_set_size( &o->adl, o->adl.size - 1 );
        ret = true;
    }
    else
    {
        if( id ) *id = 0;
        if( msg ) st_s_clear( msg );
    }

    bcore_mutex_s_unlock( &o->mutex );
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_error_clear_stack( void )
{
    context_s_clear_stack( get_context() );
}

//----------------------------------------------------------------------------------------------------------------------

sz_t bcore_error_stack_size( void )
{
    return context_s_stack_size( get_context() );
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_error_push_sc( er_t id, sc_t msg )
{
    context_s_push_sc( get_context(), id, msg );
    return id;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_error_push_fv( er_t id, sc_t format, va_list args )
{
    st_s* msg = st_s_create_fv( format, args );
    er_t ret = bcore_error_push_sc( id, msg->sc );
    st_s_discard( msg );
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------

er_t bcore_error_push_fa( er_t id, sc_t format, ... )
{
    va_list args;
    va_start( args, format );
    er_t ret = bcore_error_push_fv( id, format, args );
    va_end( args );
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_error_pop_st( er_t* id, st_s* msg )
{
    return context_s_pop_st( get_context(), id, msg );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_error_pop_to_sink( bcore_sink* sink )
{
    BLM_INIT();
    st_s* s = BLM_CREATE( st_s );
    bl_t success = bcore_error_pop_st( NULL, s );
    if( success ) bcore_sink_a_push_fa( sink, "#<sc_t>\n", s->sc );
    BLM_RETURNV( bl_t, success );
}

//----------------------------------------------------------------------------------------------------------------------

bl_t bcore_error_pop_to_stderr()
{
    return bcore_error_pop_to_sink( BCORE_STDERR );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_error_pop_all_to_sink( bcore_sink* sink )
{
    while( bcore_error_pop_to_sink( sink ) ) {};
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_error_pop_all_to_stderr()
{
    bcore_error_pop_all_to_sink( BCORE_STDERR );
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
// signal

vd_t bcore_error_manager_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, btypeof( "bcore_error_manager" ) ) )
    {
        case TYPEOF_init0:
        {
        }
        break;

        case TYPEOF_init1:
        {
        }
        break;

        case TYPEOF_down1:
        {
            if( context_g )
            {
                if( bcore_error_stack_size() > 0 )
                {
                    WRN_fa( "Error Manager: Stack is not empty!\nList of remaining errors:" );
                    st_s* msg = st_s_create();
                    er_t id = 0;
                    while( bcore_error_pop_st( &id, msg ) )
                    {
                        bcore_wrn_fa( "\n    #<sc_t>: #<sc_t>\n", ifnameof( id ), msg->sc );
                    }
                    st_s_discard( msg );
                }
                bcore_error_manager_context_s_detach( &context_g );
            }
        }
        break;

        default: break;
    }

    return NULL;
}
