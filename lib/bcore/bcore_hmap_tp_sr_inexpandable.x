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

    func m sr_s* get(    c@* o, tp_t key ); // returns pointer to value or sr_null when key does not exist
    func m sr_s* set(    m@* o, tp_t key, sr_s val ); // assumes ownership (fork if necessary); no deep copy
    func sr_s    remove( m@* o, tp_t key ); // removes key, returns value; call sr_down( remove(...) ) to eliminate entry
    func bl_t    exists( c@* o, tp_t key ); // checks if key exists
    func void    clear(  m@* o           ); // removes all entries and frees memory
    func uz_t    keys(   c@* o           ); // returns number of registered keys
    func uz_t    size(   c@* o           ); // returns current size of the hash map (note that this includes empty places)

    func tp_t    idx_key( const bcore_hmap_tp_sr_s* o, uz_t idx ); // returns indexed key (idx indexes the entire table including empty places)
    func c sr_s* idx_val( const bcore_hmap_tp_sr_s* o, uz_t idx ); // returns indexed value (idx indexes the entire table including empty places)
}

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/


