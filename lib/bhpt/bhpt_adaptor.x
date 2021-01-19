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

 /// Collection of bhpt_adaptor stamps

/**********************************************************************************************************************/

include "bmath_std.h";
include "bhvm_std.h";

/**********************************************************************************************************************/

// ---------------------------------------------------------------------------------------------------------------------

/// Adding gradient*epsilon to adaptive
stamp bhpt_adaptor_epsilon_s = aware bhpt_adaptor
{
    f3_t epsilon;
    func bhpt_adaptor . reset = {};
    func bhpt_adaptor . adapt =
    {
        assert( node.axon.s.is_equal( node.grad.s.1 ) );
        node.grad.v.mul_scl_f3_acc( o.epsilon, node.axon.v.1 );
    };
};

// ---------------------------------------------------------------------------------------------------------------------

/// Applying l2-regularization (weight-decay) to weights: w -= w * lambda;
stamp bhpt_adaptor_reg_l2_s = aware bhpt_adaptor
{
    f3_t lambda;
    func bhpt_adaptor . reset = {};
    func bhpt_adaptor . adapt = { node.axon.v.mul_scl_f3_acc( -o.lambda, node.axon.v.1 ); };
};

// ---------------------------------------------------------------------------------------------------------------------

stamp bhpt_adaptor_zro_grad_s = aware bhpt_adaptor
{
    func bhpt_adaptor . adapt = { node.grad.v.zro(); };
};

// ---------------------------------------------------------------------------------------------------------------------

/// List of adaptors applied in sequence
stamp bhpt_adaptor_list_s = aware x_array
{
    aware bhpt_adaptor => [];
    func bhpt_adaptor.reset = { foreach( m$* e in o ) e.reset(); };
    func bhpt_adaptor.adapt = { foreach( m$* e in o ) e.adapt( node ); };
};

// ---------------------------------------------------------------------------------------------------------------------

/**********************************************************************************************************************/
