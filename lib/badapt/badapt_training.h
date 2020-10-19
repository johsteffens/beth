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

#ifndef BADAPT_TRAINING_H
#define BADAPT_TRAINING_H

#include "bmath_std.h"
#include "badapt_adaptive.h"

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

XOILA_DEFINE_GROUP( badapt_supplier, bcore_inst )
#ifdef XOILA_SECTION
    feature strict sz_t get_in_size(  const );
    feature strict sz_t get_out_size( const );

    feature void fetch_sample_tin( mutable, bmath_vf3_s* in );                   // training autonomous
    feature void fetch_sample_tio( mutable, bmath_vf3_s* in, bmath_vf3_s* out ); // training supervised
    feature void fetch_sample_vin( mutable, bmath_vf3_s* in );                   // validation autonomous
    feature void fetch_sample_vio( mutable, bmath_vf3_s* in, bmath_vf3_s* out ); // validation supervised

    // returns preferred loss function
    feature const badapt_loss* preferred_loss( const );

    // sets up builder parameters where suitable
    feature void setup_builder( const, badapt_builder* builder ) = setup_builder_default;
#endif // XOILA_SECTION

//----------------------------------------------------------------------------------------------------------------------

BCORE_FORWARD_OBJECT( badapt_training_state );
XOILA_DEFINE_GROUP( badapt_guide, bcore_inst )
#ifdef XOILA_SECTION
    // callback for training iterations; state can be changed; return value indicates if training shall continue
    feature strict bl_t callback( const, badapt_training_state* state );
#endif // XOILA_SECTION

//----------------------------------------------------------------------------------------------------------------------

BCORE_FORWARD_OBJECT( badapt_progress_s );
XOILA_DEFINE_GROUP( badapt_training_state, bcore_inst )
#ifdef XOILA_SECTION
    feature void set_adaptive( mutable, const badapt_adaptive* adaptive );
    feature badapt_adaptive* get_adaptive( const );

    feature void set_supplier( mutable, const badapt_supplier* supplier );
    feature badapt_supplier* get_supplier( const );

    feature void set_progress( mutable, const badapt_progress_s* progress );
    feature badapt_progress_s* get_progress( const );

    feature void set_guide( mutable, const badapt_guide* guide );
    feature badapt_guide* get_guide( const );

    feature void set_backup_path( mutable, sc_t name );
    feature sc_t get_backup_path( const );

    feature bl_t backup( const ) =
    {
        sc_t path = badapt_training_state_a_get_backup_path( o );
        if( !path[ 0 ] ) return false;
        st_s* tmp = st_s_create_fa( "#<sc_t>.tmp", path );
        bcore_bin_ml_a_to_file( o, tmp->sc );
        bcore_file_rename( tmp->sc, path );
        st_s_discard( tmp );
        return true;
    };

    feature bl_t recover( mutable ) =
    {
        sc_t path = badapt_training_state_a_get_backup_path( o );
        if( !path || !path[ 0 ]        ) return false;
        if( !bcore_file_exists( path ) ) return false;
        bcore_bin_ml_a_from_file( o, path );
        return true;
    };

#endif // XOILA_SECTION

//----------------------------------------------------------------------------------------------------------------------

XOILA_DEFINE_GROUP( badapt_trainer, bcore_inst )
#ifdef XOILA_SECTION
    // sets up builder parameters where suitable
    feature strict void run( const, badapt_training_state* state );
    feature strict badapt_training_state* create_state( const ); // creates initial state for given training
#endif // XOILA_SECTION

//----------------------------------------------------------------------------------------------------------------------

XOILA_DEFINE_GROUP( badapt_training_objects, bcore_inst )
#ifdef XOILA_SECTION

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp badapt_progress = bcore_inst
{
    sz_t iteration = 0;
    f3_t error     = 0;
    f3_t improved  = 0;
    f3_t bias      = 0;
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp badapt_training_state_std = aware badapt_training_state
{
    badapt_progress_s progress;

    // adaptive to be trained
    aware badapt_adaptive => adaptive;

    // supplier implementing training features
    aware badapt_supplier => supplier;

    // training guide; called at each iteration;
    aware badapt_guide => guide = badapt_guide_std_s;

    st_s backup_file_name;

    func badapt_training_state : set_adaptive = { badapt_adaptive_a_replicate( &o->adaptive, adaptive ); };
    func badapt_training_state : get_adaptive = { return o->adaptive; };
    func badapt_training_state : set_supplier = { badapt_supplier_a_replicate( &o->supplier, supplier ); };
    func badapt_training_state : get_supplier = { return o->supplier; };
    func badapt_training_state : set_progress = { badapt_progress_s_copy( &o->progress, progress ); };
    func badapt_training_state : get_progress = { return cast( badapt_progress_s*, &o->progress ); };
    func badapt_training_state : set_guide    = { badapt_guide_a_replicate( &o->guide, guide ); };
    func badapt_training_state : get_guide    = { return cast( badapt_guide*, o->guide ); };
    func badapt_training_state : set_backup_path = { st_s_copy_sc( &o->backup_file_name, name ); };
    func badapt_training_state : get_backup_path = { return o->backup_file_name.sc; };


};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

stamp badapt_guide_std = aware badapt_guide
{
    f3_t annealing_factor = 0.99;

    // logging
    hidden aware bcore_sink -> log;

    // constructor
    func bcore_inst_call : init_x = { o->log = bcore_fork( BCORE_STDOUT ); };

    func badapt_guide    : callback;
};

#endif // XOILA_SECTION

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t badapt_training_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_TRAINING_H

