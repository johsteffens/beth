/** This file was generated by the beth-precode-compiler. Not suitable for manual editing.
 *
 *  Copyright and License:
 *    If a specification exists for the repository or folder in which the file is located,
 *    that specification shall apply. Otherwise, the Copyright and License of the associated
 *    source code files containing beth-precode shall apply.
 */

#include "bcore_precoded.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
/// source: bcore_precoder_sample
#include "bcore_precoder_sample.h"

//----------------------------------------------------------------------------------------------------------------------
/// group: bcore_precoder_sample_features

BCORE_DEFINE_SPECT( bcore_inst, bcore_precoder_sample_features )
"{"
    "bcore_spect_header_s header;"
    "feature bcore_precoder_sample_features : setup = bcore_precoder_sample_features_setup_default;"
    "feature bcore_precoder_sample_features : to_stdout = bcore_precoder_sample_features_to_stdout_default;"
    "feature bcore_precoder_sample_features : get_size = bcore_precoder_sample_features_get_size_default;"
"}";

static void bcore_precoder_sample_features_setup_default( bcore_precoder_sample_features* o, sz_t n )
{
    ERR_fa( "Feature has no binding." );
}

static void bcore_precoder_sample_features_to_stdout_default( const bcore_precoder_sample_features* o )
{
    ERR_fa( "Feature has no binding." );
}

static sz_t bcore_precoder_sample_features_get_size_default( const bcore_precoder_sample_features* o )
{
    ERR_fa( "Feature has no binding." );
    return *(sz_t*)NULL;
}

//----------------------------------------------------------------------------------------------------------------------
/// group: bcore_precoder_sample

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_sample_object_s )
"{"
    "aware_t _;"
    "st_s name1;"
    "sz_t => [] arr;"
    "func bcore_precoder_sample_features:setup;"
    "func bcore_precoder_sample_features:to_stdout;"
    "func bcore_precoder_sample_features:get_size;"
"}";

BCORE_DEFINE_OBJECT_INST( bcore_inst, bcore_precoder_sample_object2_s )
"{"
    "aware_t _;"
    "bcore_precoder_sample_object_s object;"
    "st_s name2;"
    "func bcore_precoder_sample_features:setup;"
    "func bcore_precoder_sample_features:to_stdout;"
"}";

/**********************************************************************************************************************/

vd_t bcore_precoded_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_precoded" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bcore_precoded_hash" ), sr_tp( 4122593692 ) );
            BCORE_REGISTER_FEATURE( bcore_precoder_sample_features_setup );
            BCORE_REGISTER_FEATURE( bcore_precoder_sample_features_to_stdout );
            BCORE_REGISTER_FEATURE( bcore_precoder_sample_features_get_size );
            BCORE_REGISTER_FFUNC( bcore_precoder_sample_features_setup, bcore_precoder_sample_features_setup_default );
            BCORE_REGISTER_FFUNC( bcore_precoder_sample_features_to_stdout, bcore_precoder_sample_features_to_stdout_default );
            BCORE_REGISTER_FFUNC( bcore_precoder_sample_features_get_size, bcore_precoder_sample_features_get_size_default );
            BCORE_REGISTER_SPECT( bcore_precoder_sample_features );
            BCORE_REGISTER_FFUNC( bcore_precoder_sample_features_setup, bcore_precoder_sample_object_s_setup );
            BCORE_REGISTER_FFUNC( bcore_precoder_sample_features_to_stdout, bcore_precoder_sample_object_s_to_stdout );
            BCORE_REGISTER_FFUNC( bcore_precoder_sample_features_get_size, bcore_precoder_sample_object_s_get_size );
            BCORE_REGISTER_OBJECT( bcore_precoder_sample_object_s );
            BCORE_REGISTER_FFUNC( bcore_precoder_sample_features_setup, bcore_precoder_sample_object2_s_setup );
            BCORE_REGISTER_FFUNC( bcore_precoder_sample_features_to_stdout, bcore_precoder_sample_object2_s_to_stdout );
            BCORE_REGISTER_OBJECT( bcore_precoder_sample_object2_s );
        }
        break;
        default: break;
    }
    return NULL;
}
