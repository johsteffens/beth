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

#ifndef BHVM_HOP_H
#define BHVM_HOP_H

/**********************************************************************************************************************/

/** Collection of Holor-Operators
 *
 */

/**********************************************************************************************************************/

#include "bmath_std.h"
#include "bhvm_planted.h"
#include "bhvm_holor.h"

/**********************************************************************************************************************/

#ifdef TYPEOF_bhvm_hop

PLANT_GROUP( bhvm_hop, bcore_inst )
#ifdef PLANT_SECTION // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


/// binary operators
group :ar2 =
{
    extending stump verbatim :_ = aware : {};

    signature void f( plain, const bhvm_holor_s* a, const bhvm_holor_s* b, bhvm_holor_s* r );

    body body_lop_f =
    {
        assert( a->v.size == r->v.size );
        tp_t tknit = BKNIT_FA3( a->v.type, b->v.type, r->v.type );
        if(      b->v.size == 1         ) bhvm_lop_ar2_$R_s_k_vs_cv( tknit, a->v.data, b->v.data, r->v.data, a->v.size );
        else if( b->v.size == a->v.size ) bhvm_lop_ar2_$R_s_k_vv_cv( tknit, a->v.data, b->v.data, r->v.data, a->v.size );
        else ERR_fa( "Vector size mismatch." );
    };

    stamp :add = { func : :f = :body_lop_f; };
    stamp :sub = { func : :f = :body_lop_f; };
    stamp :mul = { func : :f = :body_lop_f; };
    stamp :div =
    {
        func : :f =
        {
            assert( a->v.size == r->v.size );
            if( b->v.size == 1 )
            {
                tp_t tknit = BKNIT_FA3( a->v.type, TYPEOF_f3_t, r->v.type );
                f3_t inv = f3_inv( b->v.type == TYPEOF_f2_t ? *(f2_t*)b->v.data : *(f3_t*)b->v.data );
                bhvm_lop_ar2_mul_s_k_vs_cv( tknit, a->v.data, &inv, r->v.data, a->v.size );
            }
            else if( b->v.size == a->v.size )
            {
                tp_t tknit = BKNIT_FA3( a->v.type, b->v.type, r->v.type );
                bhvm_lop_ar2_div_s_k_vv_cv( tknit, a->v.data, b->v.data, r->v.data, a->v.size );
            }
            else
            {
                ERR_fa( "Vector size mismatch." );
            }
        };
    };
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // PLANT_SECTION ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif // TYPEOF_bhvm_hop

/**********************************************************************************************************************/

vd_t bhvm_hop_signal_handler( const bcore_signal_s* o );

/**********************************************************************************************************************/

#endif // BHVM_HOP_H
