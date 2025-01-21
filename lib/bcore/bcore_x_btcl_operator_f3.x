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

/** BTCL: Exportable operators highly optimized for f3 operations
 *  These operators are used in x_btcl_functor_f3.
 */

include "bcore_x_btcl_operator_f3.h";

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

func void group_signal_init1()
{
    verbatim_C
    {
        BCORE_REGISTER_TYPE( function_pointer, x_btcl_operator_f3_fp_f3_ar0 );
        BCORE_REGISTER_TYPE( function_pointer, x_btcl_operator_f3_fp_f3_ar1 );
        BCORE_REGISTER_TYPE( function_pointer, x_btcl_operator_f3_fp_f3_ar2 );
        BCORE_REGISTER_TYPE( function_pointer, x_btcl_operator_f3_fp_f3_ar3 );
    }
}

type :fp_f3_ar0;
type :fp_f3_ar1;
type :fp_f3_ar2;
type :fp_f3_ar3;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// function-arguments

//----------------------------------------------------------------------------------------------------------------------

stamp :arg_s
{
    f3_t val;
    tp_t name;
}

//----------------------------------------------------------------------------------------------------------------------

stamp :arg_arr_s x_array
{
    :arg_s [];
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

feature er_t setup( m@* o ) = 0;
feature f3_t get( @* o ) = 0;
feature er_t set_args( m@* o, m :arg_arr_s* arg_arr ) = 0;
feature er_t check_consistency( @* o ) = 0;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// nullary (leaf) operator

//----------------------------------------------------------------------------------------------------------------------

stamp :arg_nop_s
{
    sz_t index = -1; // index into argument list
    tp_t name;  // argument name
    private f3_t* val;

    hidden x_source_point_s sp;

    func :.set_args
    {
        if( o.index < 0 ) GERR_fa( "index has not been set." );
        if( o.index >= arg_arr.size ) GERR_fa( "index is out of range." );

        o.name = arg_arr.[ o.index ].name;
        o.val = arg_arr.[ o.index ].val.1;
        = 0;
    }

    func :.get { assert( o.val ); = o.val.0; }

    func :.check_consistency { EM_ASSERT( o.val ); =0; }
}

//----------------------------------------------------------------------------------------------------------------------

stamp :const_nop_s
{
    f3_t val;
    func :.get = o.val;
    func :.check_consistency { =0; }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// unary operator

//----------------------------------------------------------------------------------------------------------------------

stamp :uop_s
{
    tp_t type;
    : => a;
    hidden :spect_s* sa;

    hidden :fp_f3_ar1 fp_get;
    hidden x_source_point_s sp;

    func :.setup
    {
        o.sa = ( :spect_s* ):spect_s_get_aware( o.a );
        switch( o.type )
        {
            case identity~: o.fp_get = f3_ident; break;
            case neg~:      o.fp_get = f3_neg;   break;
            case exp~:      o.fp_get = f3_exp;   break;
            case log~:      o.fp_get = f3_log;   break;
            case log2~:     o.fp_get = f3_log2;  break;
            case log10~:    o.fp_get = f3_log10; break;
            case sin~:      o.fp_get = f3_sin;   break;
            case cos~:      o.fp_get = f3_cos;   break;
            case tan~:      o.fp_get = f3_tan;   break;
            case tanh~:     o.fp_get = f3_tanh;  break;
            case sqrt~:     o.fp_get = f3_sqrt;  break;
            case sign~:     o.fp_get = f3_sign;  break;
            case abs~:      o.fp_get = f3_abs;   break;
            case ceil~:     o.fp_get = f3_ceil;  break;
            case floor~:    o.fp_get = f3_floor; break;
            default: = o.sp.parse_error_fa( "Unhandled operator type: #<sc_t>.", bnameof( o.type ) ); break;
        }
        o.a.setup();
        = 0;
    }

    func :.set_args
    {
        o.a.set_args( arg_arr );
        = 0;
    }

    func :.get { = verbatim_C{ o->fp_get( o->sa->get( o->a ) ) }; }

    func :.check_consistency
    {
        EM_ASSERT( o.a );
        EM_ASSERT( o.sa );
        EM_ASSERT( o.fp_get != NULL );
        o.a.check_consistency();
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// binary operator

//----------------------------------------------------------------------------------------------------------------------

stamp :bop_s
{
    tp_t type;
    : => a;
    : => b;
    hidden :spect_s* sa;
    hidden :spect_s* sb;

    hidden :fp_f3_ar2 fp_get;
    hidden x_source_point_s sp;

    func :.setup
    {
        o.sa = ( :spect_s* ):spect_s_get_aware( o.a );
        o.sb = ( :spect_s* ):spect_s_get_aware( o.b );
        switch( o.type )
        {
            case pow~:           o.fp_get = :pow_get; break;
            case div~:           o.fp_get = :div_get; break;
            case mul~:           o.fp_get = :mul_get; break;
            case sub~:           o.fp_get = :sub_get; break;
            case add~:           o.fp_get = :add_get; break;
            case equal~:         o.fp_get = :equal_get; break;
            case unequal~:       o.fp_get = :unequal_get; break;
            case larger_equal~:  o.fp_get = :larger_equal_get; break;
            case larger~:        o.fp_get = :larger_get; break;
            case smaller_equal~: o.fp_get = :smaller_equal_get; break;
            case smaller~:       o.fp_get = :smaller_get; break;
            case and~:           o.fp_get = :and_get; break;
            case or~:            o.fp_get = :or_get; break;
            default: = o.sp.parse_error_fa( "Unhandled operator type: #<sc_t>.", bnameof( o.type ) ); break;
        }
        o.a.setup();
        o.b.setup();
        = 0;
    }

    func :.set_args
    {
        o.a.set_args( arg_arr );
        o.b.set_args( arg_arr );
        = 0;
    }

    func :.get { = verbatim_C{ o->fp_get( o->sa->get( o->a ), o->sb->get( o->b ) ) }; }

    func :.check_consistency
    {
        EM_ASSERT( o.a );
        EM_ASSERT( o.b );
        EM_ASSERT( o.sa );
        EM_ASSERT( o.sb );
        EM_ASSERT( o.fp_get != NULL );
        o.a.check_consistency();
        o.b.check_consistency();
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
/// ternary operator

//----------------------------------------------------------------------------------------------------------------------

stamp :top_s
{
    tp_t type;

    : => a;
    : => b;
    : => c;
    hidden :spect_s* sa;
    hidden :spect_s* sb;
    hidden :spect_s* sc;

    hidden :fp_f3_ar3 fp_get;
    hidden x_source_point_s sp;

    func :.setup
    {
        o.sa = ( :spect_s* ):spect_s_get_aware( o.a );
        o.sb = ( :spect_s* ):spect_s_get_aware( o.b );
        o.sc = ( :spect_s* ):spect_s_get_aware( o.c );
        switch( o.type )
        {
            case conditional~: o.fp_get = :conditional_get; break;
            default: = o.sp.parse_error_fa( "Unhandled operator type: #<sc_t>.", bnameof( o.type ) ); break;
        }
        o.a.setup();
        o.b.setup();
        o.c.setup();
        = 0;
    }

    func :.set_args
    {
        o.a.set_args( arg_arr );
        o.b.set_args( arg_arr );
        o.c.set_args( arg_arr );
        = 0;
    }

    func :.get { = verbatim_C{ o->fp_get( o->sa->get( o->a ), o->sb->get( o->b ), o->sc->get( o->c ) ) }; }

    func :.check_consistency
    {
        EM_ASSERT( o.a );
        EM_ASSERT( o.b );
        EM_ASSERT( o.c );
        EM_ASSERT( o.sa );
        EM_ASSERT( o.sb );
        EM_ASSERT( o.sc );
        EM_ASSERT( o.fp_get != NULL );
        o.a.check_consistency();
        o.b.check_consistency();
        o.c.check_consistency();
        = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------

/// converts from ::operator
func d :* create_op( sr_s* sr )
{
    if( sr.type() == ::functor_arg_uop_s~ )
    {
        $* src = sr.o.cast( ::functor_arg_uop_s* );
        m $* op = :arg_nop_s!^;
        op.index = src.index;
        = op.fork();
    }

    if( sr.type() == ::operator_uop_s~ )
    {
        $* src = sr.o.cast( ::operator_uop_s* );
        m $* op = :uop_s!^;
        op.type = src.type;
        ASSERT( src.a.o );
        op.a =< :create_op( src.a );
        op.sp.copy( src.sp );
        = op.fork();
    }

    if( sr.type() == ::operator_bop_s~ )
    {
        $* src = sr.o.cast( ::operator_bop_s* );
        m $* op = :bop_s!^;
        op.type = src.type;
        ASSERT( src.a.o );
        ASSERT( src.b.o );
        op.a =< :create_op( src.a );
        op.b =< :create_op( src.b );
        op.sp.copy( src.sp );
        = op.fork();
    }

    if( sr.type() == ::operator_top_s~ )
    {
        $* src = sr.o.cast( ::operator_top_s* );
        m $* op = :top_s!^;
        op.type = src.type;
        ASSERT( src.a.o );
        ASSERT( src.b.o );
        ASSERT( src.c.o );
        op.a =< :create_op( src.a );
        op.b =< :create_op( src.b );
        op.c =< :create_op( src.c );
        op.sp.copy( src.sp );
        = op.fork();
    }

    if( sr.is_numeric() )
    {
        m $* op = :const_nop_s!^;
        op.val = sr.to_f3();
        = op.fork();
    }

    ERR_fa( "Cannot convert from type #<sc_t>", bnameof( sr.type() ) );
    = NULL;
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
