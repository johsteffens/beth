/** Copyright 2019 Johannes Bernhard Steffens
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

BETH_PRECODE( badapt_supplier )
#ifdef BETH_PRECODE_SECTION
    feature strict 'a' sz_t get_in_size(  const );
    feature strict 'a' sz_t get_out_size( const );

    feature 'a' void fetch_sample_tin( mutable, bmath_vf3_s* in );                   // training autonomous
    feature 'a' void fetch_sample_tio( mutable, bmath_vf3_s* in, bmath_vf3_s* out ); // training supervised
    feature 'a' void fetch_sample_vin( mutable, bmath_vf3_s* in );                   // validation autonomous
    feature 'a' void fetch_sample_vio( mutable, bmath_vf3_s* in, bmath_vf3_s* out ); // validation supervised

    // returns preferred loss function
    feature 'a' const badapt_loss* preferred_loss( const );

    // sets up builder parameters where suitable
    feature 'a' void setup_builder( const, badapt_builder* builder ) = setup_builder_default;
#endif // BETH_PRECODE_SECTION

//----------------------------------------------------------------------------------------------------------------------

BCORE_FORWARD_OBJECT( badapt_training_state );
BETH_PRECODE( badapt_guide )
#ifdef BETH_PRECODE_SECTION
    // callback for training iterations; state can be changed; return value indicates if training shall continue
    feature strict 'a' bl_t callback( const, badapt_training_state* state );
#endif // BETH_PRECODE_SECTION

//----------------------------------------------------------------------------------------------------------------------

BCORE_FORWARD_OBJECT( badapt_progress_s );
BETH_PRECODE( badapt_training_state )
#ifdef BETH_PRECODE_SECTION
    feature 'a' void set_adaptive( mutable, const badapt_adaptive* adaptive );
    feature 'a' badapt_adaptive* get_adaptive( const );

    feature 'a' void set_supplier( mutable, const badapt_supplier* supplier );
    feature 'a' badapt_supplier* get_supplier( const );

    feature 'a' void set_progress( mutable, const badapt_progress_s* progress );
    feature 'a' badapt_progress_s* get_progress( const );

    feature 'a' void set_guide( mutable, badapt_guide* guide );
    feature 'a' badapt_guide* get_guide( const );
#endif // BETH_PRECODE_SECTION

//----------------------------------------------------------------------------------------------------------------------

BETH_PRECODE( badapt_trainer )
#ifdef BETH_PRECODE_SECTION
    // sets up builder parameters where suitable
    feature strict 'a' void run( const, badapt_training_state* state );
    feature strict 'a' badapt_training_state* create_state( const ); // creates initial state for given training
#endif // BETH_PRECODE_SECTION

//----------------------------------------------------------------------------------------------------------------------

BETH_PRECODE( badapt_training_objects )
#ifdef BETH_PRECODE_SECTION

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

    func : set_adaptive = { badapt_adaptive_a_replicate( &o->adaptive, adaptive ); };
    func : get_adaptive = { return o->adaptive; };
    func : set_supplier = { badapt_supplier_a_replicate( &o->supplier, supplier ); };
    func : get_supplier = { return o->supplier; };
    func : set_progress = { badapt_progress_s_copy( &o->progress, progress ); };
    func : get_progress = { return ( badapt_progress_s* )&o->progress; };
    func : set_guide    = { badapt_guide_a_replicate( &o->guide, guide ); };
    func : get_guide    = { return ( badapt_guide* )o->guide; };
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

#endif // BETH_PRECODE_SECTION

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

vd_t badapt_training_signal_handler( const bcore_signal_s* o );

#endif  // BADAPT_TRAINING_H

