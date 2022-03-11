/** Author and Copyright 2021 Johannes Bernhard Steffens
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

/** Complex Number
 *
 *  Nomenclature
 *  add, zro, neg, sub, mul, one, inv, div - according to spect_ring
 *  cnj - complex conjugate
 *  det - squared - magnitude: a^2 + b^2  (determinat of the canonic rot-scale-matrix)
 *  mag - magnitude: sqrt( mag );
 *  arg - phase atan2( im, re )
 *  urt - kth element of nth-roots of unity: urt(k,n) = e^i*2*pi*k/n
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp bmath_cf2_s = obliv
{
    $ f2_t r;
    $ f2_t i;

    func bl_t is_equ( @* o, @* op );
    func bl_t is_zro( @* o );
    func bl_t is_one( @* o );
    func void add( @* o, @* op, m@* res );
    func void zro( m@* o );
    func    o urt( m@* o, s3_t k, s3_t n ); // urt(k,n) = e^i*2*pi*k/n
    func void neg( @* o, m@* res );
    func void sub( @* o, @* op, m@* res );
    func void mul( @* o, @* op, m@* res );
    func void sqr( @* o, m@* res );
    func f2_t tss( @* o ); // sum of squares
    func f2_t mag( @* o ); // magnitude (srt( tss ))
    func f2_t arg( @* o ); // phase [-pi pi]

    // sets complex number via magnitude and phase
    func o set_mag_arg( m@* o, f2_t mag, f2_t arg );

    // res = ( o - op )^2
    func void sub_sqr( @* o, @* op, m@* res );

    // res = o^2 + op
    func void sqr_add( @* o, @* op, m@* res );

    func void mul_f2( @* o, f2_t op, m@* res );

    // res = o + ( op1 * op2 )
    func void add_prod( @* o, @* op1, @* op2, m@* res );

    // res = o - ( op1 * op2 )
    func void sub_prod( @* o, @* op1, @* op2, m@* res );

    // res = o * ( op1 - op2 )
    func void mul_diff( @* o, @* op1, @* op2, m@* res );

    func void one( m@* o );
    func void inv( @* o, m@* res );
    func void div( @* o, @* op, m@* res );
    func void cnj( @* o, m@* res );
    func void self_cnj( m@* o );

    /// For easy inspection
    func void to_stdout( @* o );

    /// Type conversion ...
    func void copy_typed( m@* o, tp_t type, vc_t src );
    func void copy_t( m@* o, tp_t type, vc_t src );
    func m@* clone_t( tp_t type, vc_t src );
};

identifier bmath_cf2_init;
identifier bmath_cf2_init_mag_arg;
identifier bmath_cf2_init_urt;
identifier bmath_cf2_zro;
identifier bmath_cf2_one;
identifier bmath_cf2_neg;
identifier bmath_cf2_mul;
identifier bmath_cf2_mul_fx;
identifier bmath_cf2_det;
identifier bmath_cf2_tss;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp bmath_cf3_s = obliv
{
    $ f2_t r;
    $ f2_t i;

    func bl_t is_equ( @* o, @* op );
    func bl_t is_zro( @* o );
    func bl_t is_one( @* o );
    func void add( @* o, @* op, m@* res );
    func void zro( m@* o );
    func    o urt( m@* o, s3_t k, s3_t n ); // urt(k,n) = e^i*2*pi*k/n
    func void neg( @* o, m@* res );
    func void sub( @* o, @* op, m@* res );
    func void mul( @* o, @* op, m@* res );
    func void sqr( @* o, m@* res );
    func f3_t tss( @* o ); // sum of squares
    func f3_t mag( @* o ); // magnitude (srt( tss ))
    func f3_t arg( @* o ); // phase [-pi pi]

    // sets complex number via magnitude and phase
    func o set_mag_arg( m@* o, f3_t mag, f3_t arg );

    // res = ( o - op )^2
    func void sub_sqr( @* o, @* op, m@* res );

    // res = o^2 + op
    func void sqr_add( @* o, @* op, m@* res );

    func void mul_f3( @* o, f3_t op, m@* res );

    // res = o + ( op1 * op2 )
    func void add_prod( @* o, @* op1, @* op2, m@* res );

    // res = o - ( op1 * op2 )
    func void sub_prod( @* o, @* op1, @* op2, m@* res );

    // res = o * ( op1 - op2 )
    func void mul_diff( @* o, @* op1, @* op2, m@* res );

    func void one( m@* o );
    func void inv( @* o, m@* res );
    func void div( @* o, @* op, m@* res );
    func void cnj( @* o, m@* res );
    func void self_cnj( m@* o );

    /// For easy inspection
    func void to_stdout( @* o );

    /// Type conversion ...
    func void copy_typed( m@* o, tp_t type, vc_t src );
    func void copy_t( m@* o, tp_t type, vc_t src );
    func m@* clone_t( tp_t type, vc_t src );
};

identifier bmath_cf3_init;
identifier bmath_cf3_init_mag_arg;
identifier bmath_cf3_init_urt;
identifier bmath_cf3_zro;
identifier bmath_cf3_one;
identifier bmath_cf3_neg;
identifier bmath_cf3_mul;
identifier bmath_cf3_mul_fx;
identifier bmath_cf3_det;
identifier bmath_cf3_tss;

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

