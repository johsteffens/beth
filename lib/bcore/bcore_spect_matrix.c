/** Copyright 2017 Johannes Bernhard Steffens
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

#include "bcore_spect_matrix.h"
#include "bcore_spect.h"
#include "bcore_trait.h"
#include "bcore_signal.h"
#include "bcore_quicktypes.h"

#define NPX( name ) bcore_matrix_##name

static sc_t bcore_matrix_s_def = "bcore_matrix_s = spect"
"{"
    "bcore_spect_header_s header;"
    "private bcore_array_s* spect_array;"
    "feature     sz_t rows_fix   =  0 ~> const sz_t rows;"
    "feature     sz_t cols_fix   =  0 ~> const sz_t cols;"
    "feature     sz_t stride_fix =  0 ~> const sz_t stride;"
    "feature offset_t rows_off   = -1 ~>       sz_t rows;"
    "feature offset_t cols_off   = -1 ~>       sz_t cols;"
    "feature offset_t stride_off = -1 ~>       sz_t stride;"
"}";

static bcore_matrix_s* bcore_matrix_s_create_from_self( const bcore_self_s* self )
{
    bcore_matrix_s* o = bcore_spect_create_from_self( bcore_flect_get_self( typeof( "bcore_matrix_s" ) ), self );
    o->spect_array = bcore_spect_get_typed( TYPEOF_bcore_array_s, self->type );

    if( o->cols_fix == 0 && o->cols_off == ( sz_t )-1 )
    {
        ERR_fa( "Feature 'cols' fixed-zero or no specified in '#<sc_t>'.", ifnameof( self->type ) );
    }

    if( o->rows_fix == 0 && o->rows_off == ( sz_t )-1 )
    {
        ERR_fa( "Feature 'rows' fixed-zero or no specified in '#<sc_t>'.", ifnameof( self->type ) );
    }

    return o;
}

static bcore_self_s* bcore_matrix_s_create_self( void )
{
    bcore_self_s* self = bcore_self_s_build_parse_sc( bcore_matrix_s_def, sizeof( bcore_matrix_s ) );
    bcore_self_s_push_ns_func( self, ( fp_t )bcore_matrix_s_create_from_self, "bcore_spect_fp_create_from_self", "create_from_self" );
    return self;
}

/**********************************************************************************************************************/

void bcore_matrix_spect_set_size( const bcore_matrix_s* p, vd_t o, sz_t rows, sz_t cols )
{
    if( p->rows_fix   > 0 && rows != p->rows_fix   ) ERR_fa( "Array has a fixed row-size of #<sz_t>"   , p->rows_fix );
    if( p->cols_fix   > 0 && cols != p->cols_fix   ) ERR_fa( "Array has a fixed column-size of #<sz_t>", p->cols_fix );
    if( p->stride_fix > 0 && cols != p->stride_fix ) ERR_fa( "Array has a fixed stride-size of #<sz_t>", p->stride_fix );
    bcore_array_spect_set_size( p->spect_array, o, rows * cols );
    if( p->stride_off != ( sz_t )-1 ) *( sz_t* )BCORE_OFFSET( o, p->stride_off ) = cols;
    if( p->cols_off   != ( sz_t )-1 ) *( sz_t* )BCORE_OFFSET( o, p->cols_off   ) = cols;
    if( p->rows_off   != ( sz_t )-1 ) *( sz_t* )BCORE_OFFSET( o, p->rows_off   ) = rows;
}

/**********************************************************************************************************************/

static inline const bcore_matrix_s* atpd( tp_t tp ) { return bcore_matrix_s_get_typed( tp ); }

sz_t NPX(typed_get_rows     )( tp_t t, vc_t o                         ) { return NPX(spect_get_rows      )( atpd( t ), o          ); }
sz_t NPX(typed_get_cols     )( tp_t t, vc_t o                         ) { return NPX(spect_get_cols      )( atpd( t ), o          ); }
sz_t NPX(typed_get_stride   )( tp_t t, vc_t o                         ) { return NPX(spect_get_stride    )( atpd( t ), o          ); }
void NPX(typed_set_size     )( tp_t t, vd_t o, sz_t r, sz_t c         ) {        NPX(spect_set_size      )( atpd( t ), o, r, c    ); }
sz_t NPX(typed_get_row_index)( tp_t t, vc_t o, sz_t i                 ) { return NPX(spect_get_row_index )( atpd( t ), o, i       ); }
sz_t NPX(typed_get_index    )( tp_t t, vc_t o, sz_t i, sz_t j         ) { return NPX(spect_get_index     )( atpd( t ), o, i, j    ); }
sr_s NPX(typed_get_cell     )( tp_t t, vc_t o, sz_t i, sz_t j         ) { return NPX(spect_get_cell      )( atpd( t ), o, i, j    ); }
void NPX(typed_set_cell     )( tp_t t, vd_t o, sz_t i, sz_t j, sr_s s ) {        NPX(spect_set_cell      )( atpd( t ), o, i, j, s ); }

sz_t NPX(aware_get_rows     )( vc_t o                         ) { return NPX(typed_get_rows      )( *(aware_t*)o, o          ); }
sz_t NPX(aware_get_cols     )( vc_t o                         ) { return NPX(typed_get_cols      )( *(aware_t*)o, o          ); }
sz_t NPX(aware_get_stride   )( vc_t o                         ) { return NPX(typed_get_stride    )( *(aware_t*)o, o          ); }
void NPX(aware_set_size     )( vd_t o, sz_t r, sz_t c         ) {        NPX(typed_set_size      )( *(aware_t*)o, o, r, c    ); }
sz_t NPX(aware_get_row_index)( vc_t o, sz_t i                 ) { return NPX(typed_get_row_index )( *(aware_t*)o, o, i       ); }
sz_t NPX(aware_get_index    )( vc_t o, sz_t i, sz_t j         ) { return NPX(typed_get_index     )( *(aware_t*)o, o, i, j    ); }
sr_s NPX(aware_get_cell     )( vc_t o, sz_t i, sz_t j         ) { return NPX(typed_get_cell      )( *(aware_t*)o, o, i, j    ); }
void NPX(aware_set_cell     )( vd_t o, sz_t i, sz_t j, sr_s s ) {        NPX(typed_set_cell      )( *(aware_t*)o, o, i, j, s ); }

inline static vc_t w_spect( sr_s o ) { if( sr_s_is_const( &o ) ) ERR( "Attempt to modify a constant object" ); return ch_spect_p( o.p, TYPEOF_bcore_matrix_s ); }
inline static vc_t r_spect( sr_s o ) { return ch_spect_p( o.p, TYPEOF_bcore_matrix_s ); }
inline static vc_t x_spect( sr_s o ) { return ch_spect_p( o.p, TYPEOF_bcore_matrix_s ); }

sz_t NPX(get_rows     )( sr_s o                         ) { sz_t r = NPX(spect_get_rows      )( r_spect( o ), o.o          ); sr_down( o ); return r; }
sz_t NPX(get_cols     )( sr_s o                         ) { sz_t r = NPX(spect_get_cols      )( r_spect( o ), o.o          ); sr_down( o ); return r; }
sz_t NPX(get_stride   )( sr_s o                         ) { sz_t r = NPX(spect_get_stride    )( r_spect( o ), o.o          ); sr_down( o ); return r; }
void NPX(set_size     )( sr_s o, sz_t r, sz_t c         ) {          NPX(spect_set_size      )( w_spect( o ), o.o, r, c    ); sr_down( o );           }
sz_t NPX(get_row_index)( sr_s o, sz_t i                 ) { sz_t r = NPX(spect_get_row_index )( r_spect( o ), o.o, i       ); sr_down( o ); return r; }
sz_t NPX(get_index    )( sr_s o, sz_t i, sz_t j         ) { sz_t r = NPX(spect_get_index     )( r_spect( o ), o.o, i, j    ); sr_down( o ); return r; }
sr_s NPX(get_cell     )( sr_s o, sz_t i, sz_t j         ) { sr_s r = NPX(spect_get_cell      )( x_spect( o ), o.o, i, j    ); sr_down( o ); return r; }
void NPX(set_cell     )( sr_s o, sz_t i, sz_t j, sr_s s ) {          NPX(spect_set_cell      )( w_spect( o ), o.o, i, j, s ); sr_down( o );           }

sz_t NPX(q_get_rows     )( const sr_s* o                         ) { sz_t r = NPX(spect_get_rows      )( r_spect( *o ), o->o          ); return r; }
sz_t NPX(q_get_cols     )( const sr_s* o                         ) { sz_t r = NPX(spect_get_cols      )( r_spect( *o ), o->o          ); return r; }
sz_t NPX(q_get_stride   )( const sr_s* o                         ) { sz_t r = NPX(spect_get_stride    )( r_spect( *o ), o->o          ); return r; }
void NPX(q_set_size     )( const sr_s* o, sz_t r, sz_t c         ) {          NPX(spect_set_size      )( w_spect( *o ), o->o, r, c    );           }
sz_t NPX(q_get_row_index)( const sr_s* o, sz_t i                 ) { sz_t r = NPX(spect_get_row_index )( r_spect( *o ), o->o, i       ); return r; }
sz_t NPX(q_get_index    )( const sr_s* o, sz_t i, sz_t j         ) { sz_t r = NPX(spect_get_index     )( r_spect( *o ), o->o, i, j    ); return r; }
sr_s NPX(q_get_cell     )( const sr_s* o, sz_t i, sz_t j         ) { sr_s r = NPX(spect_get_cell      )( x_spect( *o ), o->o, i, j    ); return r; }
void NPX(q_set_cell     )( const sr_s* o, sz_t i, sz_t j, sr_s s ) {          NPX(spect_set_cell      )( w_spect( *o ), o->o, i, j, s );           }

/**********************************************************************************************************************/
// signal

vd_t bcore_spect_matrix_signal_handler( const bcore_signal_s* o )
{
    switch( bcore_signal_s_handle_type( o, typeof( "bcore_spect_matrix" ) ) )
    {
        case TYPEOF_init1:
        {
            BCORE_REGISTER_FLECT( bcore_matrix_s );
        }
        break;

        case TYPEOF_selftest:
        {
//            return spect_matrix_selftest();
        }
        break;

        default: break;
    }

    return NULL;
}
