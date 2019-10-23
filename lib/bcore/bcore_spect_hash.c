/** Author and Copyright 2018 Johannes Bernhard Steffens
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

#include "bcore_spect_hash.h"
#include "bcore_spect_array.h"
#include "bcore_signal.h"


BCORE_DEFINE_SPECT( bcore_inst, bcore_hash )
"{"
    "bcore_spect_header_s header;"
    "strict feature bcore_via_s -> spect_via;"
"}";

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_hash_default_fold_tp( const bcore_hash_s* p, const bcore_hash* o, tp_t hash )
{
    if( o )
    {
        switch( p->header.o_type )
        {
            case TYPEOF_s3_t: return bcore_tp_fold_u3( hash, *( u3_t* )o );
            case TYPEOF_s2_t: return bcore_tp_fold_u2( hash, *( u2_t* )o );
            case TYPEOF_s1_t: return bcore_tp_fold_u1( hash, *( u1_t* )o );
            case TYPEOF_s0_t: return bcore_tp_fold_u0( hash, *( u0_t* )o );

            case TYPEOF_u3_t: return bcore_tp_fold_u3( hash, *( u3_t* )o );
            case TYPEOF_u2_t: return bcore_tp_fold_u2( hash, *( u2_t* )o );
            case TYPEOF_u1_t: return bcore_tp_fold_u1( hash, *( u1_t* )o );
            case TYPEOF_u0_t: return bcore_tp_fold_u0( hash, *( u0_t* )o );

            case TYPEOF_f3_t:
            {
                int exp = 0;
                s3_t man_s3 = frexp( *( f3_t* )o, &exp ) * 0x7FFFFFFFFFFFFFFF;
                s2_t exp_s2 = exp;
                hash = bcore_tp_fold_u3( hash, man_s3 );
                hash = bcore_tp_fold_u2( hash, exp_s2 );
                return hash;
            }

            case TYPEOF_f2_t:
            {
                int exp = 0;
                s3_t man_s3 = frexp( *( f2_t* )o, &exp ) * 0x7FFFFFFFFFFFFFFF;
                s2_t exp_s2 = exp;
                hash = bcore_tp_fold_u3( hash, man_s3 );
                hash = bcore_tp_fold_u2( hash, exp_s2 );
                return hash;
            }

            case TYPEOF_sz_t: return bcore_tp_fold_vc( hash, o, sizeof( sz_t ) );
            case TYPEOF_uz_t: return bcore_tp_fold_vc( hash, o, sizeof( uz_t ) );
            case TYPEOF_sd_t: return bcore_tp_fold_sc( hash, *( sd_t* )o );
            case TYPEOF_sc_t: return bcore_tp_fold_sc( hash, *( sc_t* )o );
            case TYPEOF_st_s: return bcore_tp_fold_sc( hash, ( ( st_s* )o )->sc );
            case TYPEOF_tp_t: return bcore_tp_fold_tp( hash, *( tp_t* )o );
            case TYPEOF_bl_t: return bcore_tp_fold_u0( hash, (*(bl_t*)o) ? 1 : 0 );
            case TYPEOF_aware_t: return bcore_tp_fold_tp( hash, *( tp_t* )o );

            default:
            {
                hash = bcore_tp_fold_tp( hash, p->header.o_type );
                if( bcore_via_p_is_pure_array( p->spect_via, ( bcore_via* )o ) )
                {
                    const bcore_array_s* arr_p = bcore_array_s_get_typed( p->spect_via->header.o_type );
                    uz_t size = bcore_array_p_get_size( arr_p, ( bcore_array* )o );
                    hash = bcore_tp_fold_uz( hash, size );
                    for( uz_t i = 0; i < size; i++ )
                    {
                        sr_s sr = bcore_array_p_get( arr_p, (bcore_array*)o, i );
                        hash = bcore_tp_fold_tp( hash, sr_s_type( &sr ) );
                        if( sr.p ) hash = bcore_hash_r_fold_tp( &sr, hash );
                        sr_down( sr );
                    }
                }
                else
                {
                    const bcore_via_s* via = p->spect_via;
                    uz_t size = via->size;
                    for( uz_t i = 0; i < size; i++ )
                    {
                        sr_s sr = bcore_via_p_iget( via, (bcore_via*)o, i );
                        hash = bcore_tp_fold_tp( hash, sr_s_type( &sr ) );
                        if( sr.p ) hash = bcore_hash_r_fold_tp( &sr, hash );
                        sr_down( sr );
                    }
                }
            }
        }
    }
    else
    {
        hash = bcore_tp_fold_tp( hash, p->header.o_type );
    }
    return hash;
}

//----------------------------------------------------------------------------------------------------------------------

tp_t bcore_hash_default_get_tp( const bcore_hash_s* p, const bcore_hash* o )
{
    return bcore_hash_default_fold_tp( p, o, bcore_tp_init() );
}

//----------------------------------------------------------------------------------------------------------------------

void bcore_spect_hash_selftest()
{
    sr_s sr1 = bcore_spect_via_create_zoo( 5 );
    sr_s sr2 = bcore_spect_via_create_zoo( 5 );
    tp_t hash1 = bcore_hash_r_get_tp( &sr1 );
    tp_t hash2 = bcore_hash_r_get_tp( &sr2 );

    ASSERT( hash1 == hash2 );

    // This ASSERT might fail if bcore_spect_via_create_zoo or the hash algorithm has changed.
    // In that case update the hash value using
    // bcore_msg_fa( "#<tp_t>\n", hash1 );
    ASSERT( hash1 == 3040853191 );

    sr_down( sr1 );
    sr_down( sr2 );
}

/**********************************************************************************************************************/

vd_t bcore_spect_hash_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_hash" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_SPECT( bcore_hash );
        }
        break;

        case TYPEOF_selftest:
        {
            bcore_spect_hash_selftest();
        }
        break;

        default: break;
    }
    return NULL;
}
