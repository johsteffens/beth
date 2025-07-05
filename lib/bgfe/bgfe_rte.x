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

/** Thread Safe Runtime Environment.
 *  Only one instance should exist for a running program.
 *  Use function get() to obtain the instance.
 */

/** Nomenclature for functions and variables
 *  rtt_*  - only used inside the runtime thread
 *  rts_*  - shared between runtime thread and user thread. (secured via mutex)
 *  other: - not used by runtime thread
 */

include "bgfe_rte.h";
include <gtk/gtk.h>;

/// GTK identifiers and types

identifier GTK_ORIENTATION_VERTICAL, GTK_ORIENTATION_HORIZONTAL, GTK_CONTAINER, G_CALLBACK;
identifier g_signal_connect, gtk_container_add, gtk_widget_show, gtk_widget_destroy, g_signal_handlers_disconnect_by_data;
identifier gtk_widget_set_size_request, gtk_widget_set_hexpand, gtk_widget_set_vexpand, gtk_widget_set_name;
identifier gdk_threads_add_idle;

type gboolean, GtkWidget;

/// other types
type bgfe_rte_fp_rtt;

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func void group_signal_init1()
{
    verbatim_C
    {
        BCORE_REGISTER_TYPE( function_pointer, bgfe_rte_fp_rtt );
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

/** (Optional) Creates the bgfe-runtime environment considering program arguments.
 *  Call this function in 'main' after beth-initialization or first in 'main_c'.
 *  p_argc, p_argv are pointers to program arguments of the main function.
 *  GTK arguments are parsed and the argument list is updated accordingly.
 *  When argument parsing is not desired, set p_argc, p_argv to NULL.
 *  Note: The rte is created but not started at this point. This happens in function 'get'
 *  Explicit closing is not necessary.
 */
func er_t open( m int* p_argc, m char*** p_argv )
{
    :init_gtk( p_argc, p_argv );
    m$* rte = :s!^;
    rte.init_gtk_called = true;
    bcore_global_get_ifnexists_set_d( :s~, rte._, rte.fork() );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/** Obtains the bgfe-runtime environment. If not existing, it is created.
 *  Objects using the RTE should obtain a reference to ensure the RTE is setup correctly.
 *  Explicit closing is not necessary.
 */
func er_t get( m :s** rte )
{
    m :s* g_rte = bcore_global_get_ifnexists_set( :s~, :s~ ).cast( m :s* );
    if( !g_rte.is_running ) g_rte.start(); // start will return in case rte was started by another thread
    if( rte ) rte.1 = g_rte;
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

func er_t init_gtk( m int* p_argc, m char*** p_argv )
{
    verbatim_C { gtk_disable_setlocale() };
    if( !verbatim_C { gtk_init_check( p_argc, p_argv ) } ) = GERR_fa( "'gtk_init_check' failed." );
    = 0;
}

//----------------------------------------------------------------------------------------------------------------------

/** Runs a function in the rte thread.
 *  Example:
 *  stamp :s
 *  {
 *      func er_t f( m@* o, m x_inst* arg ) = 0; //do something with o and arg
 *      func er_t f_in_rtt( m@* o, m x_inst* arg ) = bgfe_rte_run_s!^( o.f.cast( bgfe_rte_fp_rtt ), o, arg );
 *  }
 */
stamp :run_s
{
    x_condition_s condition;
    x_mutex_s     mutex;

    hidden bl_t    busy;
    hidden x_inst* obj;
    hidden x_inst* arg;
    hidden er_t    ret;
    hidden bgfe_rte_fp_rtt fp_func;

    func gboolean rtt_func( m@* o )
    {
        o.mutex.lock();
        {
            o.ret  = verbatim_C{ o->fp_func( o->obj, o->arg ) };
            o.busy = false;
        }
        o.mutex.unlock();

        o.condition.wake_all();
        = 0;
    }

    /** Synchronized call of rtt_func( obj, arg ) inside rte thread. (Compact Function)
     *  Returns return value of rtt_func.
     *  This function can be called from any thread.
     *  While call_rtt_func is waiting, the runtime environment can be used for other purposes from other threads.
     *  obj, arg:
     *    These parameters are declared 'obliv' to allow passing obliv arguments.
     *    However, their final requirements are determined by fp_func
     */
    func er_t _( m@* o, bgfe_rte_fp_rtt fp_func, m obliv x_inst* obj, m obliv x_inst* arg )
    {
        er_t ret = 0;

        o.mutex.lock();
        {
            while( o.fp_func ) o.condition.sleep( o.mutex );
            o.busy = true;
            o.fp_func = fp_func;
            o.obj = obj;
            o.arg = arg;
            //verbatim_C{ g_idle_add( ( gboolean(*)( vd_t ) )bgfe_rte_run_s_rtt_func, o ) };

            // add_idle_full gives control over priority; otherwise no apparent advantage over 'g_idle_add'
            verbatim_C{ gdk_threads_add_idle_full( G_PRIORITY_DEFAULT_IDLE, ( gboolean(*)( vd_t ) )bgfe_rte_run_s_rtt_func, o, NULL ) };
            while( o.busy ) o.condition.sleep( o.mutex );
            ret = o.ret;
            o.obj = NULL;
            o.arg = NULL;
            o.ret = 0;
            o.fp_func = NULL;
        }
        o.mutex.unlock();

        o.condition.wake_all();
        = ret;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

identifier gtk_css_provider_new, g_object_unref, gtk_css_provider_load_from_data, gtk_style_context_add_provider_for_screen, gdk_screen_get_default;
type GtkCssProvider, GError, GTK_STYLE_PROVIDER_PRIORITY_FALLBACK, GTK_STYLE_PROVIDER_PRIORITY_USER, GTK_STYLE_PROVIDER;

stamp :s
{
    sz_t verbosity = 0;
    :run_s run;
    er_t m_thread_func_err; // m_thread function terminated with error

    private GtkCssProvider* css_provider;

    hidden bl_t init_gtk_called;
    hidden bl_t is_running;

    hidden x_mutex_s mutex;
    hidden x_thread_s thread;

    func bcore_inst_call.down_e o.stop();

    func er_t rtt_setup_css( m@* o, vd_t arg )
    {
        o.css_provider = gtk_css_provider_new();
        m GError* p_gerror = NULL;
        m st_s* style_css = :style_css_as_string()^;
        gtk_css_provider_load_from_data( o.css_provider, style_css.sc, -1, &p_gerror );

        if( p_gerror )
        {
            m$* gtk_msg = st_s!^.push_sc( verbatim_C { p_gerror->message } );
            gtk_msg.replace_sc_sc( "<data>", "bgfe_rte_style.css" );
            = GERR_fa( "GTK style parse error:\n#<sc_t>:\n", gtk_msg.sc );
        }

        gtk_style_context_add_provider_for_screen( gdk_screen_get_default(), GTK_STYLE_PROVIDER( o.css_provider ), GTK_STYLE_PROVIDER_PRIORITY_USER );

        = 0;
    }

    func er_t rtt_cleanup_css( m@* o, vd_t arg )
    {
        if( o.css_provider )
        {
            g_object_unref( o.css_provider );
            o.css_provider = NULL;
        }
        = 0;
    }

    func x_thread.m_thread_func
    {

        if( !o.init_gtk_called ) :init_gtk( NULL, NULL );
        o.init_gtk_called = true;
        if( o.verbosity > 0 ) bcore_msg_fa( "#name: gtk_main started\n", o._ );
        verbatim_C { gtk_main(); }
        if( o.verbosity > 0 ) bcore_msg_fa( "#name: gtk_main exited\n", o._ );

        = NULL;
    }

    func er_t start( m@* o )
    {
        o.mutex.create_lock()^;
        if( o.verbosity > 0 ) bcore_msg_fa( "#name: starting\n", o._ );

        o.thread.call_m_thread_func( o );
        if( o.is_running ) = 0;
        o.is_running = true;

        if( o.verbosity > 0 ) bcore_msg_fa( "#name: setting up css\n", o._ );
        o.run( o.rtt_setup_css.cast( bgfe_rte_fp_rtt ), o, NULL );

        = 0;
    }

    func gboolean rtt_gtk_main_quit( m@* o )
    {
        if( o.verbosity > 0 ) bcore_msg_fa( "#name: gtk_main_quit\n", o._ );
        verbatim_C{ gtk_main_quit() };
        = 0;
    }

    func er_t stop( m@* o )
    {
        o.mutex.create_lock()^;
        if( !o.is_running ) = 0;

        if( o.verbosity > 0 ) bcore_msg_fa( "#name: cleaning up css\n", o._ );
        o.run( o.rtt_cleanup_css.cast( bgfe_rte_fp_rtt ), o, NULL );

        if( o.verbosity > 0 ) bcore_msg_fa( "#name: stopping\n", o._ );
        verbatim_C{ g_idle_add( ( gboolean(*)( vd_t ) )bgfe_rte_s_rtt_gtk_main_quit, o ) };
        o.thread.join();
        if( o.verbosity > 0 ) bcore_msg_fa( "#name: joined\n", o._ );
        o.is_running = false;
        = 0;
    }

 }

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

