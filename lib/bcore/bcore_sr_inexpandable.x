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

//----------------------------------------------------------------------------------------------------------------------

group sr = x_inst
{
    stamp :s = obliv x_inst
    {
        private obliv x_inst* o; // object
        private obliv bcore_spect* p;  // perspective
        private tp_t f;          // flags
        shell typed => obj;

        /** Initializers - 3 letter nomenclature:
         *
         *  1st letter: {t|a|q|p}: perspective retrieval:
         *     t : via type
         *     a : via RTTI of object (RTTI: runtime-type-information <=> aware object)
         *     p : explicit (fastest)
         *     q : via RTPI of object (RTPI: runtime-perspective-information <=> first element of object is perspective)
         *
         *  2nd letter: {w|s}: weak | strong
         *
         *  3rd letter: {c|m}: const | mutable
         *
         */
        func o twc( m@* o, tp_t t, vc_t b );
        func o twm( m@* o, tp_t t, vd_t b );
        func o tsc( m@* o, tp_t t, vd_t b );
        func o tsm( m@* o, tp_t t, vd_t b );
        func o awc( m@* o,         vc_t b );
        func o awm( m@* o,         vd_t b );
        func o asc( m@* o,         vd_t b );
        func o asm( m@* o,         vd_t b );
        func o qwc( m@* o,         vc_t b );
        func o qwm( m@* o,         vd_t b );
        func o qsc( m@* o,         vd_t b );
        func o qsm( m@* o,         vd_t b );
        func o pwc( m@* o, vc_t p, vc_t b );
        func o pwm( m@* o, vc_t p, vd_t b );
        func o psc( m@* o, vc_t p, vd_t b );
        func o psm( m@* o, vc_t p, vd_t b );

        /// o references a numeric object
        func bl_t is_numeric ( c@* o );
        func bl_t is_float   ( c@* o );
        func bl_t is_integer ( c@* o );
        func bl_t is_unsigned( c@* o );

        /// converts to a leaf type
        func f3_t to_f3( c@* o );
        func u3_t to_u3( c@* o );
        func s3_t to_s3( c@* o );
        func bl_t to_bl( c@* o );
        func tp_t to_tp( c@* o );

        /// converts from a leaf type
        func o from_f3( m@* o, f3_t v );
        func o from_u3( m@* o, u3_t v );
        func o from_s3( m@* o, s3_t v );
        func o from_bl( m@* o, bl_t v );
        func o from_tp( m@* o, tp_t v );

        /// converts from a leaf type and sets o const
        func o const_from_f3( m@* o, f3_t v );
        func o const_from_u3( m@* o, u3_t v );
        func o const_from_s3( m@* o, s3_t v );
        func o const_from_bl( m@* o, bl_t v );
        func o const_from_tp( m@* o, tp_t v );

        func tp_t p_type( c@* o ); // type of perspective
        func tp_t o_type( c@* o ); // type of object
        func tp_t type(   c@* o ); // type of object (same as o_type)

        func bl_t is_weak(    c@* o );
        func bl_t is_strong(  c@* o );
        func bl_t is_const(   c@* o );
        func bl_t is_mutable( c@* o );

        func void set_strong( m@* o, bl_t flag );
        func void set_const(  m@* o, bl_t flag );

        /// converts o to sr_null()
        func void clear( m@* o );

        /// sets o to a fork of src
        func void set( m@* o, sr_s src );

        /// creates a strong reference by forking from src (without shutting src down); const flag is preserved
        func o fork_from( m@* o, m sr_s* src );

        /// creates a strong reference by cloning from src (without shutting src down); const flag is reset
        func o clone_from( m@* o, c sr_s* src );

        /// retrieves a weak reference to o
        func sr_s get( m@* o );

        /* reference control */

        /** Number of (strong) references to the object.
         *  Return value
         *    0: This reference is null
         *    1: Only one strong reference. If o is strong, it is the only existing.
         *   >1: Multible strong references exist.
         */
        func uz_t references( c@* o );

    };

    func sr_s null(); // returns a null-reference
    func sr_s pocs( vc_t p, m x_inst* o, bl_t const_f, bl_t strong_f );

    func sr_s twc( tp_t t, c obliv x_inst* o ); // typed weak const
    func sr_s twm( tp_t t, m obliv x_inst* o ); // typed weak mutable
    func sr_s tsc( tp_t t, d obliv x_inst* o ); // typed strong const
    func sr_s tsm( tp_t t, d obliv x_inst* o ); // typed strong mutable
    func sr_s awc( c x_inst* o );  // aware weak const
    func sr_s awm( d x_inst* o );  // aware weak mutable
    func sr_s asc( d x_inst* o );  // aware strong const
    func sr_s asm( d x_inst* o );  // aware strong mutable
    func sr_s pwc( c bcore_spect* p, c obliv x_inst* o ); // spect weak const
    func sr_s pwm( c bcore_spect* p, m obliv x_inst* o ); // spect weak mutable
    func sr_s psc( c bcore_spect* p, d obliv x_inst* o ); // spect strong const
    func sr_s psm( c bcore_spect* p, d obliv x_inst* o ); // spect strong mutable

    func sr_s cw( sr_s o ); // turns a reference into a weak one;
    func sr_s cc( sr_s o ); // turns a reference into a const one;
    func sr_s cp( sr_s o, tp_t spect_type ); // changes perspective

    // returns a strong reference; terminates o
    func sr_s fork( sr_s o );

    // deep clone
    func sr_s clone( sr_s o );

    // shut down
    func void down( sr_s o );

    // creates a new instance
    func sr_s create_from_type( tp_t type );
    func sr_s create_from_spect( c bcore_spect* spect );

    /// creates a strong reference of a typed object by cloning obj
    func sr_s create_strong_typed( tp_t type, c obliv x_inst* obj );

    // copies specified leaf objects into a strong reference
    func sr_s s0( s0_t v );
    func sr_s s1( s1_t v );
    func sr_s s2( s2_t v );
    func sr_s s3( s3_t v );
    func sr_s u0( u0_t v );
    func sr_s u1( u1_t v );
    func sr_s u2( u2_t v );
    func sr_s u3( u3_t v );
    func sr_s f2( f2_t v );
    func sr_s f3( f3_t v );
    func sr_s sz( sz_t v );
    func sr_s uz( uz_t v );
    func sr_s tp( tp_t v );
    func sr_s bl( bl_t v );

    func sr_s string_sc( sc_t v ); // converts to st_s

    /// converts to a leaf type
    func f3_t to_f3( sr_s o );
    func u3_t to_u3( sr_s o );
    func s3_t to_s3( sr_s o );
    func bl_t to_bl( sr_s o );
    func tp_t to_tp( sr_s o );


};

/**********************************************************************************************************************/
