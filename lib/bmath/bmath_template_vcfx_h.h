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

/**********************************************************************************************************************/

/** Template: Complex Vector */

/**********************************************************************************************************************/

#include "bmath_template_fx_begin.h"

/**********************************************************************************************************************/

void BCATU(bmath_vcfx_s,move)( bmath_vcfx_s* o, bmath_vcfx_s* src );

void BCATU(bmath_vcfx_s,set_size)( bmath_vcfx_s* o, uz_t size );
void BCATU(bmath_vcfx_s,push)(     bmath_vcfx_s* o, bmath_cfx_s cfx );
void BCATU(bmath_vcfx_s,push_ri)(  bmath_vcfx_s* o, fx_t r, fx_t i );

bmath_vcfx_s* BCATU(bmath_vcfx_s,create_size)( uz_t size );
bmath_vcfx_s* BCATU(bmath_vcfx_s,create_fill)( bmath_cfx_s val, uz_t size );

void BCATU(bmath_vcfx_s,zro)(           bmath_vcfx_s* o );
void BCATU(bmath_vcfx_s,neg)(     const bmath_vcfx_s* o, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,cnj)(     const bmath_vcfx_s* o, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,cpy)(     const bmath_vcfx_s* o, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,add)(     const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,sub)(     const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,mul_scl)( const bmath_vcfx_s* o, const bmath_cfx_s*  op, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,mul_cfx)( const bmath_vcfx_s* o, const bmath_cfx_s   op, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,mul_fx)(  const bmath_vcfx_s* o,              fx_t   op, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,dft)(     const bmath_vcfx_s* o, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,ift)(     const bmath_vcfx_s* o, bmath_vcfx_s* res );
void BCATU(bmath_vcfx_s,mul_vec)( const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_cfx_s* res );
void BCATU(bmath_vcfx_s,sqr)(     const bmath_vcfx_s* o, bmath_cfx_s* res );
void BCATU(bmath_vcfx_s,sub_sqr)( const bmath_vcfx_s* o, const bmath_vcfx_s* op, bmath_cfx_s* res );
void BCATU(bmath_vcfx_s,sum)(     const bmath_vcfx_s* o, bmath_cfx_s* res );
void BCATU(bmath_vcfx_s,avg)(     const bmath_vcfx_s* o, bmath_cfx_s* res );

static inline
void BCATU(bmath_vcfx_s,set_cfx)( bmath_vcfx_s* o, uz_t index, bmath_cfx_s v )
{
    assert( index < o->size );
    o->data[ index ] = v;
}

static inline
bmath_cfx_s BCATU(bmath_vcfx_s,get_cfx)( const bmath_vcfx_s* o, uz_t index )
{
    assert( index < o->size );
    return o->data[ index ];
}

/// For easy inspection
void BCATU(bmath_vcfx_s,to_stdout)( const bmath_vcfx_s* o );

/**********************************************************************************************************************/

#include "bmath_template_fx_end.h"

/**********************************************************************************************************************/

