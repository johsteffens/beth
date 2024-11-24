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

stamp bcore_hnode_tp_sr_s = obliv bcore_inst
{
    tp_t key;
    sr_s val;
}

stamp bcore_hmap_tp_sr_s = aware bcore_inst
{
    private bcore_hnode_tp_sr_s* nodes;
    private bl_t* flags;
    uz_t size;
    uz_t depth_limit;

    func m sr_s* get( c@* o, tp_t key ); // returns pointer to value or sr_null when key does not exist
    func m sr_s* set( m@* o, tp_t key, sr_s val ); // assumes ownership (fork if necessary); no deep copy

    func c bcore_inst* get_c_inst( c@* o, tp_t key ); // returns associated instance (NULL when key does not exists)
    func m bcore_inst* get_m_inst( m@* o, tp_t key ); // returns associated instance (NULL when key does not exists)
    func tp_t          get_type  ( c@* o, tp_t key ); // returns associated instance-type (0 when key does not exists)

    func m sr_s* set_t_inst_c( m@* o, tp_t key, tp_t type, c obliv bcore_inst* inst ); // sets new value by copying typed inst
    func m sr_s* set_t_inst_d( m@* o, tp_t key, tp_t type, d obliv bcore_inst* inst ); // sets new value by referencing discardable typed inst
    func m sr_s* set_inst_c  ( m@* o, tp_t key,            c aware bcore_inst* inst ); // sets new value by copying aware inst
    func m sr_s* set_inst_d  ( m@* o, tp_t key,            d aware bcore_inst* inst ); // sets new value by referencing discardable aware inst

    func sr_s    remove( m@* o, tp_t key ); // removes key, returns value; call sr_down( remove(...) ) to eliminate entry
    func bl_t    exists( c@* o, tp_t key ); // checks if key exists
    func void    clear(  m@* o           ); // removes all entries and frees memory
    func uz_t    keys(   c@* o           ); // returns number of registered keys
    func uz_t    size(   c@* o           ); // returns current size of the hash map (note that this includes empty places)

    func tp_t    idx_key( const bcore_hmap_tp_sr_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
    func c sr_s* idx_val( const bcore_hmap_tp_sr_s* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)

    /// sc_-functions below use a sc_t-key via bentypeof, bnameof (all key names get enrolled)
    func m sr_s* sc_get(   c@* o, sc_t key ); // returns pointer to value or sr_null when key does not exist
    func m sr_s* sc_set(   m@* o, sc_t key, sr_s val ); // assumes ownership (fork if necessary); no deep copy
    func sr_s  sc_remove(  m@* o, sc_t key ); // removes key, returns value; call sr_down( remove(...) ) to eliminate entry
    func bl_t  sc_exists(  c@* o, sc_t key ); // checks if key exists
    func sc_t  sc_idx_key( c@* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
    func c bcore_inst* sc_get_c_inst( c@* o, sc_t key ); // returns associated instance (NULL when key does not exists)
    func m bcore_inst* sc_get_m_inst( m@* o, sc_t key ); // returns associated instance (NULL when key does not exists)
    func tp_t sc_get_type( c@* o, sc_t key ); // returns associated instance-type (0 when key does not exists)
    func sr_s* sc_set_t_inst_c( m@* o, sc_t key, tp_t type, c obliv bcore_inst* inst ); // sets new value by copying typed inst
    func sr_s* sc_set_t_inst_d( m@* o, sc_t key, tp_t type, m obliv bcore_inst* inst ); // sets new value by referencing discardable typed inst
    func sr_s* sc_set_inst_c  ( m@* o, sc_t key,            c aware bcore_inst* inst ); // sets new value by copying aware inst
    func sr_s* sc_set_inst_d  ( m@* o, sc_t key,            m aware bcore_inst* inst ); // sets new value by referencing discardable aware inst
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


