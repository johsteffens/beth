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

/** C-Engine Interface */

/**********************************************************************************************************************/

//----------------------------------------------------------------------------------------------------------------------

feature 'a' er_t translate( c @* o, c xoico_host* host, c xoico_body_s* body, c xoico_signature_s* signature, m x_sink* sink );
feature 'a' bl_t is_reserved( c @* o, tp_t tp_identifier ) { return false; };

//----------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
