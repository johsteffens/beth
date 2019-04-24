/** This file was generated by the beth-precode-compiler. Not suitable for manual editing.
 *
 *  Copyright and License:
 *    If a specification exists for the repository or folder in which the file is located,
 *    that specification shall apply. Otherwise, the Copyright and License of the associated
 *    source code files containing beth-precode shall apply.
 */

#include "bmath_precoded.h"
#include "bcore_spect.h"
#include "bcore_spect_inst.h"
#include "bcore_sr.h"
#include "bcore_const_manager.h"


/**********************************************************************************************************************/
/// source: bmath_mf3_sx
#include "bmath_mf3_sx.h"

//----------------------------------------------------------------------------------------------------------------------
/// group: bmath_mf3_sx

BCORE_DEFINE_OBJECT_INST( bcore_inst, bmath_mf3_sx_s )\
  "{ aware_t _; sz_t slos; sz_t xons; sz_t rows; sz_t i_stride; sz_t [] i; f3_t [] v; }";

/**********************************************************************************************************************/

vd_t bmath_precoded_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bmath_precoded" ) ) )
    {
        case TYPEOF_init1:
        {
            // Comment or remove line below to rebuild this target.
            bcore_const_x_set_d( typeof( "bmath_precoded_hash" ), sr_tp( 1208762499 ) );
            BCORE_REGISTER_OBJECT( bmath_mf3_sx_s );
        }
        break;
        default: break;
    }
    return NULL;
}
