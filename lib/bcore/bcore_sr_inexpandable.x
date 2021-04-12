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
        private x_inst* o;      // object
        private bcore_spect* p; // perspective
        private tp_t f;         // flags
        shell typed => obj;

        func (f3_t to_f3( c@* o ));
        func (u3_t to_u3( c@* o ));
        func (s3_t to_s3( c@* o ));
        func (bl_t to_bl( c@* o ));
        func (tp_t to_tp( c@* o ));

        func (tp_t p_type( c@* o )); // type of perspective
        func (tp_t o_type( c@* o )); // type of object
        func (tp_t type(   c@* o )); // type of object (same as o_type)

        func (bl_t is_weak(   c@* o ));
        func (bl_t is_strong( c@* o ));
        func (bl_t is_const(  c@* o ));

        func (void set_strong( m@* o, bl_t flag ));
        func (void set_const(  m@* o, bl_t flag ));

        /// converts o to sr_null()
        func (void clear( m@* o ));

        /// sets o to a fork of src
        func (void set( m@* o, sr_s src ));

        /// retrieves a weak reference to o
        func (sr_s get( m@* o ));

        /* reference control */

        /** Number of (strong) references to the object.
         *  Return value
         *    0: This reference is null
         *    1: Only one strong reference. If o is strong, it is the only existing.
         *   >1: Multible strong references exist.
         */
        func (uz_t references( c@* o ));

    };

    func (sr_s null()); // returns a null-reference
    func (sr_s pocs( vc_t p, m x_inst* o, bl_t const_f, bl_t strong_f ));

    func (sr_s twc( tp_t t, c obliv x_inst* o )); // typed weak const
    func (sr_s twd( tp_t t, d obliv x_inst* o )); // typed weak discardable
    func (sr_s tsd( tp_t t, d obliv x_inst* o )); // typed strong discardable
    func (sr_s awc( c x_inst* o ));  // aware weak const
    func (sr_s awd( d x_inst* o ));  // aware weak discardable
    func (sr_s asd( d x_inst* o ));  // aware strong discardable
    func (sr_s pwc( c bcore_spect* p, c obliv x_inst* o )); // spect weak const
    func (sr_s pwd( c bcore_spect* p, d obliv x_inst* o )); // spect weak discardable
    func (sr_s psd( c bcore_spect* p, d obliv x_inst* o )); // spect strong discardable

    func (sr_s cw( sr_s o )); // turns a reference into a weak one;
    func (sr_s cc( sr_s o )); // turns a reference into a const one;
    func (sr_s cp( sr_s o, tp_t spect_type )); // changes perspective

    // returns a strong reference; terminates o
    func (sr_s fork( sr_s o ));

    // deep clone
    func (sr_s clone( sr_s o ));

    // shut down
    func (void down( sr_s o ));

    // creates a new instance
    func (sr_s create_from_type( tp_t type ));
    func (sr_s create_from_spect( c bcore_spect* spect ));

    /// creates a strong reference of a typed object by cloning obj
    func (sr_s create_strong_typed( tp_t type, c obliv x_inst* obj ));

    // copies specified leaf objects into a strong reference
    func (sr_s s0( s0_t v ));
    func (sr_s s1( s1_t v ));
    func (sr_s s2( s2_t v ));
    func (sr_s s3( s3_t v ));
    func (sr_s u0( u0_t v ));
    func (sr_s u1( u1_t v ));
    func (sr_s u2( u2_t v ));
    func (sr_s u3( u3_t v ));
    func (sr_s f2( f2_t v ));
    func (sr_s f3( f3_t v ));
    func (sr_s sz( sz_t v ));
    func (sr_s uz( uz_t v ));
    func (sr_s tp( tp_t v ));
    func (sr_s bl( bl_t v ));

    func (sr_s string_sc( sc_t v )); // converts to st_s

    func (f3_t to_f3( sr_s o ));
    func (u3_t to_u3( sr_s o ));
    func (s3_t to_s3( sr_s o ));
    func (bl_t to_bl( sr_s o ));
    func (tp_t to_tp( sr_s o ));
};

/**********************************************************************************************************************/
