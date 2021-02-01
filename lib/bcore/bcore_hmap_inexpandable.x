/** Author and Copyright 2020 Johannes Bernhard Steffens
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

group bcore_hmap = x_inst
{

signature bl_t exists(  c @* o, tp_t key ); // checks if key exists
signature void clear(   m @* o ); // removes all entries and frees memory
signature uz_t keys(    c @* o ); // returns number of registered keys
signature uz_t size(    c @* o ); // returns current size of the hash map (note that this includes empty places)
signature tp_t idx_key( c @* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)

//----------------------------------------------------------------------------------------------------------------------

stamp bcore_hnode_tpuz_s = obliv bcore_inst
{
    tp_t key;
    uz_t val;
};

group bcore_hmap_tpuz = bcore_inst
{
    signature m uz_t* get   ( c @* o, tp_t key ); // returns pointer to value or NULL when key does not exist
    signature m uz_t* fget  ( m @* o, tp_t key, uz_t init_val ); // forced-get: returns pointer to value associated with key; if key does not exist, it is crated and value initialized init_val
    signature m uz_t* set   ( m @* o, tp_t key, uz_t val ); // sets new key; sets/overwrites value and returns pointer to value location
    signature uz_t remove ( m @* o, tp_t key ); // removes key, returns copy of associated value if existing, 0 otherwise.
    signature uz_t idx_val( c @* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

    /** Compute a map from (aware) array where -
     *  - key is the hash from the specified member of each array-element.
     *  - val is the index position of the array-element.
     *  Uses via and hash perspectives.
     *  Elements without 'member_name' are not indexed.
     *  In case of duplicate-key, the first entry for the key is indexed.
     *  Duplicate keys can arise from duplicate member values or hash collisions.
     *  Returns the number of duplicate-keys detected.
     */
    signature sz_t from_array( m @* o, m bcore_array* array, tp_t member_name );

    stamp :s = aware bcore_inst
    {
        private bcore_hnode_tpuz_s* nodes;
        private bl_t* flags;
        uz_t size;
        uz_t depth_limit;
        uz_t size_limit;

        func  : .get;
        func  : .fget;
        func  : .set;
        func  : .remove;
        func :: .exists;
        func :: .clear;
        func :: .keys;
        func :: .size;
        func :: .idx_key;
        func  : .idx_val;
        func  : .from_array;
    };
};

//----------------------------------------------------------------------------------------------------------------------

stamp bcore_hnode_tpfp_s = obliv bcore_inst
{
    tp_t key;
    fp_t val;
};

group bcore_hmap_tpfp = bcore_inst
{
    signature m fp_t* get ( c @* o, tp_t key ); // returns pointer to value or NULL when key does not exist
    signature m fp_t* fget( m @* o, tp_t key, fp_t init_val ); // forced-get: returns pointer to value associated with key; if key does not exist, it is crated and value initialized init_val
    signature m fp_t* set ( m @* o, tp_t key, fp_t val ); // sets new key; sets/overwrites value and returns pointer to value location
    signature fp_t remove(  m @* o, tp_t key ); // removes key, returns copy of associated value if existing, 0 otherwise.
    signature fp_t idx_val( c @* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

    stamp :s = aware bcore_inst
    {
        private bcore_hnode_tpfp_s* nodes;
        private bl_t* flags;
        uz_t size;
        uz_t depth_limit;
        uz_t size_limit;

        func  : .get;
        func  : .fget;
        func  : .set;
        func  : .remove;
        func :: .exists;
        func :: .clear;
        func :: .keys;
        func :: .size;
        func :: .idx_key;
        func  : .idx_val;
    };
};

//----------------------------------------------------------------------------------------------------------------------

stamp bcore_hnode_tptp_s = obliv bcore_inst
{
    tp_t key;
    tp_t val;
};

group bcore_hmap_tptp = bcore_inst
{
    signature m tp_t* get (  c @* o,   tp_t key ); // returns pointer to value or NULL when key does not exist
    signature m tp_t* fget(  m @* o, tp_t key, tp_t init_val ); // forced-get: returns pointer to value associated with key; if key does not exist, it is crated and value initialized init_val
    signature m tp_t* set (  m @* o, tp_t key, tp_t val ); // sets new key; sets/overwrites value and returns pointer to value location
    signature tp_t  remove(  m @* o, tp_t key ); // removes key, returns copy of associated value if existing, 0 otherwise.
    signature tp_t  idx_val( c @* o,   uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

    stamp :s = aware bcore_inst
    {
        private bcore_hnode_tptp_s* nodes;
        private bl_t* flags;
        uz_t size;
        uz_t depth_limit;
        uz_t size_limit;

        func  : .get;
        func  : .fget;
        func  : .set;
        func  : .remove;
        func :: .exists;
        func :: .clear;
        func :: .keys;
        func :: .size;
        func :: .idx_key;
        func  : .idx_val;
    };
};

//----------------------------------------------------------------------------------------------------------------------

stamp bcore_hnode_tpvd_s = obliv bcore_inst
{
    tp_t key;
    vd_t val;
};

group bcore_hmap_tpvd = bcore_inst
{
    signature m vd_t* get (  c @* o, tp_t key ); // returns pointer to value or NULL when key does not exist
    signature m vd_t* fget(  m @* o, tp_t key, vd_t init_val ); // forced-get: returns pointer to value associated with key; if key does not exist, it is crated and value initialized init_val
    signature m vd_t* set (  m @* o, tp_t key, vd_t val ); // sets new key; sets/overwrites value and returns pointer to value location
    signature vd_t  remove(  m @* o, tp_t key ); // removes key, returns copy of associated value if existing, 0 otherwise.
    signature vd_t  idx_val( c @* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

    stamp :s = aware bcore_inst
    {
        private bcore_hnode_tpvd_s* nodes;
        private bl_t* flags;
        uz_t size;
        uz_t depth_limit;
        uz_t size_limit;

        func  : .get;
        func  : .fget;
        func  : .set;
        func  : .remove;
        func :: .exists;
        func :: .clear;
        func :: .keys;
        func :: .size;
        func :: .idx_key;
        func  : .idx_val;
    };
};

//----------------------------------------------------------------------------------------------------------------------

stamp bcore_hnode_tpto_s = obliv bcore_inst
{
    tp_t key;
    vd_t val;
};

group bcore_hmap_tpto = bcore_inst
{
    signature void  set_type( m @* o, tp_t type ); // sets type; resets array in case of type difference
    signature m vd_t* get   ( c @* o, tp_t key ); // returns pointer to value or NULL when key does not exist
    signature m vd_t* fget_d( m @* o, tp_t key, vd_t init_val ); // forced-get: returns pointer to value associated with key; if key does not exist, it is crated and value initialized init_val
    signature m vd_t* fget  ( m @* o, tp_t key ); // forced-get: returns pointer to value associated with key; if key does not exist, it is created and the value created according to type
    signature m vd_t* set_d ( m @* o, tp_t key, vd_t val ); // sets new key; sets/overwrites value and returns pointer to value location
    signature void  remove  ( m @* o, tp_t key ); // removes key, destroys associated object (if present)
    signature vd_t  idx_val ( c @* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

    stamp :s = aware bcore_inst
    {
        tp_t type;
        private bcore_hnode_tpto_s* nodes;
        private bl_t* flags;
        uz_t size;
        uz_t depth_limit;
        uz_t size_limit;

        func  : .set_type;
        func  : .get;
        func  : .fget_d;
        func  : .fget;
        func  : .set_d;
        func  : .remove;
        func :: .exists;
        func :: .clear;
        func :: .keys;
        func :: .size;
        func :: .idx_key;
        func  : .idx_val;
    };
};

//----------------------------------------------------------------------------------------------------------------------

group bcore_hmap_tp = bcore_inst
{
    signature uz_t get(      c @* o, tp_t key ); // returns index of key; if not existing, index is o->size
    signature uz_t set(    m @* o, tp_t key ); // sets new key if not already existing; returns index
    signature uz_t remove( m @* o, tp_t key ); // removes key, returns old index

    stamp :s = aware bcore_inst
    {
        private tp_t* keys;
        private bl_t* flags;
        uz_t size;
        uz_t depth_limit;
        uz_t size_limit;

        func  : .get;
        func  : .set;
        func  : .remove;
        func :: .exists;
        func :: .clear;
        func :: .keys;
        func :: .size;
        func :: .idx_key;
    };
};

//----------------------------------------------------------------------------------------------------------------------

};
