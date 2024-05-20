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

/** Vector Objects
 *
 *  Nomenclature
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
 *  tss: total sum of squares (also vector-norm^2 or inner product of vector with itself)
 */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp bmath_vf2_s =
{
    f2_t [];

    // vector object functions ...
    func void init_weak( m@* o, f2_t* data, sz_t size );
    func void move( m@* o, m@* src );
    func void clear( m@* o );
    func o set_size(  m@* o, uz_t size );
    func o set_space( m@* o, uz_t space );
    func o fill( m@* o, f2_t val );

    /// copies external data into vector
    func void set_data( m@* o, const f2_t* data, sz_t size );

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

    /// swaps vectors by swapping data references (no copying of memory)
    func void swapr( m@* o, m@* v );

    func void push( m@* o, f2_t val );
    func void push_vfx( m@* o, @* vec );

    func bl_t is_near_equ( @* o, @* op, f2_t max_dev );
    func bl_t is_near_zro( @* o,        f2_t max_dev );

    func bl_t is_equ( @* o, @* op );
    func bl_t is_zro( @* o );

    /// sqrt(sum over squared differences)
    func f2_t fdev( @* o, @* op );

    /// vector is nan if at least one element is nan
    func bl_t is_nan( @* o );

    /// total sum of squares
    func f2_t fx_tss( @* o ); // deprecated (use tss())
    func f3_t tss( @* o );

    /** fdev = ||f(o) - x||
     *  '|| ... ||' = Frobenius norm  ( sqrt(sum over squares) )
     */
    func f2_t fdev_equ( @* o, @* b );
    func f2_t fdev_zro( @* o );

    func void zro( m@* o );
    func void neg(  @* o, m@* res );
    func void cpy(  @* o, m@* res );
    func void cat(  @* o, @* b,  m@* r );
    func void add(  @* o, @* op, m@* res );
    func void sub(  @* o, @* op, m@* res );

    /// adds the same scalar to all components
    func void add_scl(    @* o, f2_t* op, m@* res );
    func void sub_scl(    @* o, f2_t* op, m@* res );
    func void add_scl_fx( @* o, f2_t  op, m@* res );
    func void sub_scl_fx( @* o, f2_t  op, m@* res );

    /// hadamard product: a o b [+ c] -> r  (elementwise)
    func void mul_hdm(     @* o, @* b,       m@* r );
    func void mul_hdm_add( @* o, @* b, @* c, m@* r );

    func void pmt_mul(     @* o, bmath_pmt_s* p, m@* res ); // y = P   * x ; y_p[k] = x_k
    func void pmt_htp_mul( @* o, bmath_pmt_s* p, m@* res ); // y = P^T * x ; y_k = x_p[k]

    func void mul_scl_fx(     @* o, f2_t s,       m@* r ); // o * b     --> r
    func void mul_scl_fx_add( @* o, f2_t s, @* a, m@* r ); // o * b + a --> r

    func void mul_scl(    @* o, f2_t* op, m@* res  );
    func void mul_fx(     @* o, f2_t scl2, m@* res ); // same as mul_scl_fx
    func f2_t fx_mul_vec( @* o, @* vec2 );
    func f2_t fx_sqr(     @* o );
    func f2_t fx_sub_sqr( @* o, @* vec2 );
    func f2_t fx_max(     @* o );
    func f2_t fx_max_abs( @* o );
    func f2_t fx_min(     @* o );
    func f2_t fx_min_abs( @* o );
    func f2_t fx_sum(     @* o );
    func f2_t fx_avg(     @* o );
    func f2_t fx_var(     @* o );
    func f2_t fx_dev(     @* o );
    func void mul_vec(    @* o, @* op, m f2_t* res ); // dot product
    func void sqr(        @* o, m f2_t* res  );
    func void sub_sqr(    @* o, @* op, m f2_t* res );
    func void sum(        @* o, m f2_t* res  );
    func void avg(        @* o, m f2_t* res  );
    func void var(        @* o, m f2_t* res  );
    func void dev(        @* o, m f2_t* res  );

    func void set_sqr( m@* o, f2_t val ); // multiplies a factor to all components such that fx_sqr returns val
    func void set_sum( m@* o, f2_t val ); // adds a value to all components such that fx_sum returns val
    func void set_avg( m@* o, f2_t val ); // adds a value to all components such that BCATU(bmath_vfx_s,fx_avg returns val

    func void set_fx( m@* o, uz_t index, f2_t v );

    func f2_t get_fx( @* o, uz_t index );

    func f2_t max(     @* o );
    func f2_t min(     @* o );
    func uz_t idx_max( @* o );
    func uz_t idx_min( @* o );

    // For easy inspection
    func void to_sink(   @* o, m x_sink* sink );
    func void to_string( @* o, m st_s* string );
    func void to_stdout( @* o                 );

    // vector statistics
    func void stat_to_sink(   @* o, m x_sink* sink );
    func void stat_to_string( @* o, m st_s* string );
    func void stat_to_stdout( @* o                 );

    // type conversion
    func er_t copy_typed( m@* o, tp_t type, obliv x_inst* src );
    func void copy_t(     m@* o, tp_t type, obliv x_inst* src );
    func void copy_a(     m@* o,            aware x_inst* src );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

stamp bmath_vf3_s =
{
    f3_t [];

    // vector object functions ...
    func void init_weak( m@* o, f3_t* data, sz_t size );
    func void move( m@* o, m@* src );
    func void clear( m@* o );
    func o set_size(  m@* o, uz_t size );
    func o set_space( m@* o, uz_t space );
    func o fill( m@* o, f3_t val );

    /// copies external data into vector
    func void set_data( m@* o, const f3_t* data, sz_t size );

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

    /// swaps vectors by swapping data references (no copying of memory)
    func void swapr( m@* o, m@* v );

    func void push( m@* o, f3_t val );
    func void push_vfx( m@* o, @* vec ); // TODO: rename to push_vec or push_vf

    func bl_t is_near_equ( @* o, @* op, f3_t max_dev );
    func bl_t is_near_zro( @* o,        f3_t max_dev );

    func bl_t is_equ( @* o, @* op );
    func bl_t is_zro( @* o );

    /// sqrt(sum over squared differences)
    func f3_t fdev( @* o, @* op );

    /// vector is nan if at least one element is nan
    func bl_t is_nan( @* o );

    /// total sum of squares
    func f3_t fx_tss( @* o ); // deprecated (use tss)
    func f3_t tss( @* o );

    /** fdev = ||f(o) - x||
     *  '|| ... ||' = Frobenius norm  ( sqrt(sum over squares) )
     */
    func f3_t fdev_equ( @* o, @* b );
    func f3_t fdev_zro( @* o );

    func void zro( m@* o );
    func void neg(  @* o, m@* res );
    func void cpy(  @* o, m@* res );
    func void cat(  @* o, @* b,  m@* r );
    func void add(  @* o, @* op, m@* res );
    func void sub(  @* o, @* op, m@* res );

    /// adds the same scalar to all components
    func void add_scl(    @* o, f3_t* op, m@* res );
    func void sub_scl(    @* o, f3_t* op, m@* res );
    func void add_scl_fx( @* o, f3_t  op, m@* res );
    func void sub_scl_fx( @* o, f3_t  op, m@* res );

    /// hadamard product: a o b [+ c] -> r  (elementwise)
    func void mul_hdm(     @* o, @* b,       m@* r );
    func void mul_hdm_add( @* o, @* b, @* c, m@* r );

    func void pmt_mul(     @* o, bmath_pmt_s* p, m@* res ); // y = P   * x ; y_p[k] = x_k
    func void pmt_htp_mul( @* o, bmath_pmt_s* p, m@* res ); // y = P^T * x ; y_k = x_p[k]

    func void mul_scl_fx(     @* o, f3_t s,       m@* r ); // o * b     --> r
    func void mul_scl_fx_add( @* o, f3_t s, @* a, m@* r ); // o * b + a --> r

    func void mul_scl(    @* o, f3_t* op, m@* res  );
    func void mul_fx(     @* o, f3_t scl2, m@* res ); // same as mul_scl_fx
    func f3_t fx_mul_vec( @* o, @* vec2 );
    func f3_t fx_sqr(     @* o );
    func f3_t fx_sub_sqr( @* o, @* vec2 );
    func f3_t fx_max(     @* o );
    func f3_t fx_max_abs( @* o );
    func f3_t fx_min(     @* o );
    func f3_t fx_min_abs( @* o );
    func f3_t fx_sum(     @* o );
    func f3_t fx_avg(     @* o );
    func f3_t fx_var(     @* o );
    func f3_t fx_dev(     @* o );
    func void mul_vec(    @* o, @* op, m f3_t* res ); // dot product
    func void sqr(        @* o, m f3_t* res  );
    func void sub_sqr(    @* o, @* op, m f3_t* res );
    func void sum(        @* o, m f3_t* res  );
    func void avg(        @* o, m f3_t* res  );
    func void var(        @* o, m f3_t* res  );
    func void dev(        @* o, m f3_t* res  );

    func void set_sqr( m@* o, f3_t val ); // multiplies a factor to all components such that fx_sqr returns val
    func void set_sum( m@* o, f3_t val ); // adds a value to all components such that fx_sum returns val
    func void set_avg( m@* o, f3_t val ); // adds a value to all components such that BCATU(bmath_vfx_s,fx_avg returns val

    func void set_fx( m@* o, uz_t index, f3_t v );

    func f3_t get_fx( @* o, uz_t index );

    func f3_t max(     @* o );
    func f3_t min(     @* o );
    func uz_t idx_max( @* o );
    func uz_t idx_min( @* o );

    // For easy inspection
    func void to_sink(   @* o, m x_sink* sink );
    func void to_string( @* o, m st_s* string );
    func void to_stdout( @* o                 );

    // vector statistics
    func void stat_to_sink(   @* o, m x_sink* sink );
    func void stat_to_string( @* o, m st_s* string );
    func void stat_to_stdout( @* o                 );

    // type conversion
    func er_t copy_typed( m@* o, tp_t type, obliv x_inst* src );
    func void copy_t(     m@* o, tp_t type, obliv x_inst* src );
    func void copy_a(     m@* o,            aware x_inst* src );
};

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/

