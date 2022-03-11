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

/** Complex Vector */

/** Nomenclature
 *  add, zro, neg, sub, mul, mul_vec, sqr, sqr_sub - according to spect_vector
 *  mul_hdm: C = A o B  (hadamard product == elementwise product)
 *  cnj: conjugation
 *  cpy: copy
 *  cat: catenation
 *  sum: sum of elements
 *  avg: average
 *  var: (std-)variance
 *  dev: (std-)deviation
 *  dft: discrete fourier transform  (fft used when size is power of 2)
 *  ift: inverse dft
 *  pmt: permutation
 *  tss: total sum of squares (also vector-norm)
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp bmath_vcf2_s =
{
    bmath_cf2_s [];

    /// checks, deviations

    /** Near-state means: For each vector element the absolute difference
     *  to the specified state is less or equal max_dev.
     *  Hence, max_dev == 0 tests for the exact state (s. below).
     */
    func bl_t is_near_equ( @* o, @* op, f2_t max_dev ); // equality
    func bl_t is_near_zro( @* o, f2_t max_dev ); // zero

    func bl_t is_equ( @* o, @* op );
    func bl_t is_zro( @* o );

    /// Vector is nan if at least one element is nan
    func bl_t is_nan( @* o );

    /// total sum of squares
    func f2_t tss( @* o );

    /// Generalized frobenius norm (sqrt(sum over all components: sqr(difference))
    func f2_t fdev( @* o, @* op );

    /******************************************************************************************************************/

    func void move( m@* o, m@* src );
    func void clear( m@* o );

    func void set_size( m@* o, uz_t size );
    func void push(     m@* o, bmath_cf2_s cfx );
    func void push_ri(  m@* o, f2_t r, f2_t i );

    func void push_r( m@* o, f2_t r );
    func void push_i( m@* o, f2_t i );
    func void set_r(  m@* o, uz_t index, f2_t r );
    func void set_i(  m@* o, uz_t index, f2_t i );

    /// sets real values from vector (only leftbound min( o.size, vec.size) values)
    func void set_vr( m@* o, bmath_vf2_s* vec );

    /// sets imag values from vector (only leftbound min( o.size, vec.size) values)
    func void set_vi( m@* o, bmath_vf2_s* vec );

    func d@* create_size( uz_t size );
    func d@* create_fill( bmath_cf2_s val, uz_t size );

    /** Sets all vector elements to random values.
     *  Random generator:
     *    Parameters density, min, max, p_rval apply to the random generator.
     *      rval: Pointer to running variable of random generator.
     *            If NULL, an internal fixed random seed is used.
     *
     *     density (range [0.0, 1.0]) specifies the rate at which the random generator
     *     creates a non-zero value.
     */
    func void set_random(    m@* o, f2_t density, f2_t min, f2_t max, bcore_prsg* prsg );
    func void set_random_u3( m@* o, f2_t density, f2_t min, f2_t max, u3_t* p_rval );

    func void zro(           m@* o );
    func void neg(     @* o, m@* res );
    func void cnj(     @* o, m@* res );
    func void cpy(     @* o, m@* res );
    func void add(     @* o, @* op, m@* res );
    func void sub(     @* o, @* op, m@* res );
    func void mul_scl( @* o, const bmath_cf2_s*  op, m@* res );
    func void mul_cfx( @* o, const bmath_cf2_s   op, m@* res );
    func void mul_fx(  @* o,              f2_t   op, m@* res );
    func void dft(     @* o, m@* res );
    func void ift(     @* o, m@* res );
    func void mul_vec( @* o, @* op, bmath_cf2_s* res );
    func void sqr(     @* o, bmath_cf2_s* res );
    func void sub_sqr( @* o, @* op, bmath_cf2_s* res );
    func void sum(     @* o, bmath_cf2_s* res );
    func void avg(     @* o, bmath_cf2_s* res );

    func void set_cfx( m@* o, uz_t index, bmath_cf2_s v );

    func bmath_cf2_s get_cfx( @* o, uz_t index );

    /// For easy inspection
    func void to_stdout( @* o );

    /******************************************************************************************************************/
    // type conversion

    func void copy_typed( m@* o, tp_t type, vc_t src );
    func void copy_t( m@* o, tp_t type, vc_t src );
    func void copy_a( m@* o, vc_t src );
    func d@* clone_t( tp_t type, vc_t src );
    func d@* clone_a( vc_t src );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp bmath_vcf3_s =
{
    bmath_cf3_s [];

    /// checks, deviations

    /** Near-state means: For each vector element the absolute difference
     *  to the specified state is less or equal max_dev.
     *  Hence, max_dev == 0 tests for the exact state (s. below).
     */
    func bl_t is_near_equ( @* o, @* op, f3_t max_dev ); // equality
    func bl_t is_near_zro( @* o, f3_t max_dev ); // zero

    func bl_t is_equ( @* o, @* op );
    func bl_t is_zro( @* o );

    /// Vector is nan if at least one element is nan
    func bl_t is_nan( @* o );

    /// total sum of squares
    func f3_t tss( @* o );

    /// Generalized frobenius norm (sqrt(sum over all components: sqr(difference))
    func f3_t fdev( @* o, @* op );

    /******************************************************************************************************************/

    func void move( m@* o, m@* src );
    func void clear( m@* o );

    func void set_size( m@* o, uz_t size );
    func void push(     m@* o, bmath_cf3_s cfx );
    func void push_ri(  m@* o, f3_t r, f3_t i );

    func void push_r( m@* o, f3_t r );
    func void push_i( m@* o, f3_t i );
    func void set_r(  m@* o, uz_t index, f3_t r );
    func void set_i(  m@* o, uz_t index, f3_t i );

    /// sets real values from vector (only leftbound min( o.size, vec.size) values)
    func void set_vr( m@* o, bmath_vf3_s* vec );

    /// sets imag values from vector (only leftbound min( o.size, vec.size) values)
    func void set_vi( m@* o, bmath_vf3_s* vec );

    func d@* create_size( uz_t size );
    func d@* create_fill( bmath_cf3_s val, uz_t size );

    /** Sets all vector elements to random values.
     *  Random generator:
     *    Parameters density, min, max, p_rval apply to the random generator.
     *      rval: Pointer to running variable of random generator.
     *            If NULL, an internal fixed random seed is used.
     *
     *     density (range [0.0, 1.0]) specifies the rate at which the random generator
     *     creates a non-zero value.
     */
    func void set_random(    m@* o, f3_t density, f3_t min, f3_t max, bcore_prsg* prsg );
    func void set_random_u3( m@* o, f3_t density, f3_t min, f3_t max, u3_t* p_rval );

    func void zro(           m@* o );
    func void neg(     @* o, m@* res );
    func void cnj(     @* o, m@* res );
    func void cpy(     @* o, m@* res );
    func void add(     @* o, @* op, m@* res );
    func void sub(     @* o, @* op, m@* res );
    func void mul_scl( @* o, const bmath_cf3_s*  op, m@* res );
    func void mul_cfx( @* o, const bmath_cf3_s   op, m@* res );
    func void mul_fx(  @* o,              f3_t   op, m@* res );
    func void dft(     @* o, m@* res );
    func void ift(     @* o, m@* res );
    func void mul_vec( @* o, @* op, bmath_cf3_s* res );
    func void sqr(     @* o, bmath_cf3_s* res );
    func void sub_sqr( @* o, @* op, bmath_cf3_s* res );
    func void sum(     @* o, bmath_cf3_s* res );
    func void avg(     @* o, bmath_cf3_s* res );

    func void set_cfx( m@* o, uz_t index, bmath_cf3_s v );
    func bmath_cf3_s get_cfx( @* o, uz_t index );

    /// For easy inspection
    func void to_stdout( @* o );

    /******************************************************************************************************************/
    // type conversion

    func void copy_typed( m@* o, tp_t type, vc_t src );
    func void copy_t( m@* o, tp_t type, vc_t src );
    func void copy_a( m@* o, vc_t src );
    func d@* clone_t( tp_t type, vc_t src );
    func d@* clone_a( vc_t src );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

