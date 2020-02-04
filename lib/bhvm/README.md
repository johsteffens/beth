# Beth Holor Virtual Machine (BHVM)

## Holor
Name and definition was inspired by *Moon & Spencer's work: "Theory of Holors"*. 
A holor can be understood as generalization of a Tensor. Likewise it can be seen
as canonical extension of a Matrix to *n* dimensions. 

Our definition actually represents a specific sub-class of the M&S Holor. We have chosen
different semantics and nomenclature to describe its properties, as it seemed better
suited to the realm of intended use cases.

In the scope of this library, the holor is physically represented as multidimensional array.
(Jagged arrays are excluded.)

### Inductive Definition
A given Holor has an *Order* **n**, with **n** being a non-negative whole number.
We call it **n-Holor**.

The simplest Holor is the **0-Holor**, which is a **Scalar**. 
It can be represented as real (or complex) number.
All possible values are allowed.

The **(n+1)-Holor** is an array of **n-Holors**.
We call the number of elements in the array: **Leading Dimension** of the Holor.

Each element is a **n-Holor** of **the same leading dimension**.
The values of each element are independent.

### Shape
Above definition implies that a n-Holor is a hierarchical composition
of n sub-holors: { n-Holor, (n-1)-Holor, ... , 1-Holor }, each with a specified
leading dimension. 

We call the ordered set (or sequence) of 'n' leading dimensions: **Shape**.
The shape of the 0-holor is the empty set: \{ \}.

It follows from the definition that all elements of a Holor must have the same Shape.

One can mentalize the Holor as n-Orthotoype (or n-Hypercuboid), with the 0-Holor
being an *'Atom'* (or Orthotoype of unit-size).

Constructing the (n+1)-Holor means stacking multiple n-Orthotoype on each other
in a new dimension. Since each sub-Orthotoype has a fixed and finite shape,
its atoms can be generically folded into linear space
(or linear addressable memory) such that all sub-holors also retain a valid
representation. One possibility of implementing this, and the way we adopted as well, 
is like the ABI of multidimensional arrays in the programming language 'C'.

### Volume
We define **Volume** as the product of all elements of **Shape**.
<br>The 0-holor has Volume **1** (*Empty Product*).
<br><sub> Note: *'Volume'* is the related to Moon & Spencer's *'Number of Merates'*</sub>

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
<sub>&copy; 2019, 2020 Johannes B. Steffens</sub>