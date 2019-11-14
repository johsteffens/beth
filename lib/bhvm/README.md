# Beth Holor Virtual Machine (BHVM)

## Holor
Name and definition was inspired by *Moon & Spencer's work: "Theory of Holors"*. A holor can be understood as
generalization of a Tensor. We choose to deviate from some of M&S specification and some nomenclature for practical
reasons. E.g. Our definition actually represents a sub-class of the M&S Holor. Certain properties are emphasized and 
renamed to better relate to the intended realm of use cases.

In the scope of this library, the holor is physically represented as multidimensional array.
Unlike Moon & Spencer, we forbid jagged arrays. 

We define the Holor inductively. Meaning, the definition describes the simplest version with a method to construct
more complex versions from simpler ones.

### Inductive Definition
A general Holor is called **n-holor**, with **n** being a non-negative whole number. **n** is called the *Order*
of the Holor.

The simplest Holor is the **0-Holor**, which is a **Scalar**. It can be represented as real (or complex) number.
All possible values are allowed.

The **(n+1)-Holor** is an array with specified number of elements. 
We call the number of elements: **Leading Dimension**.
Each element is a **n-Holor** of **the same leading dimension**. The values of each element have no restriction.
They are are all independent.

### Shape
Above definition implies that a n-holor is a hierarchical composition
of n sub-holors: { n-holor, (n-1)-holor, ... , 1-holor }, each with a specified
leading dimension. 

We call the ordered set (or sequence) of 'n' leading dimensions: **Shape**.
The shape of the 0-holor is the empty set: \{ \}.

One can mentalize the Holor as n-Orthotoype (or n-Hypercuboid). The atom is the
0-holor. It can be seen as as hypercube of unit-size. 
Constructing the (n+1)-Holor means stacking multiple n-Orthotoype on each other in a new dimension.
Since each sub-Orthotoype has a fixed and finite shape, its atoms can be generically folded into linear space 
(or linear addressable memory) such that all sub-holors also retain a valid representation.
One possibility, and the way we adopt, is the ABI of multidimensional arrays in the C-Language.

### Volume
We define **Volume** as the product of all elements of **Shape**.
<br>The 0-holor has Volume **1** (*Empty Product*).
<br><sub> Note: *'Volume'* is the same as Moon & Spencer's *'Number of Merates'*</sub>

### Examples
|Name|Description|Shape|Volume|
|:---|:---|:---|:---|
|0-Holor|Scalar|\{\}|1|
|1-Holor|Vector of size m|\{m\}|m|
|2-Holor|(n x m)-Matrix|\{n,m\}|n * m|
|3-Holor|Array of k elements; each being a (n x m)-Matrix|\{k,n,m\}|k * n * m|

## Operations
A holor has well defined operations like addition, multiplication, convolution, transposition, etc.
Where suitable operations are expressed as operators, 
which are dedicated objects.

## Holor Virtual Machine
The holor virtual machine is a virtual machine with holors as data elements
holor-operands as principal instructions. Additional instructions
controlling program flow (loops, branches) can be part of the virtual machine.

------
<sub>&copy; 2019 Johannes B. Steffens</sub>