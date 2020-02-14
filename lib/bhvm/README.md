# Beth Holor Virtual Machine (BHVM)

## Holor
Name and definition was inspired by *Moon & Spencer's work: "Theory of Holors"*. 
A holor can be understood as generalization of a Tensor. Likewise it can be seen
as canonical extension of a matrix to *n* dimensions. 

Our definition actually represents a specific sub-class of the M&S holor. We have chosen
different semantics and nomenclature to describe its properties as it seemed better
suited to the realm of intended use cases.

In the scope of this library the holor is physically represented as multidimensional array.
(Jagged arrays are excluded.)

### Inductive Definition
A given holor has an *order* **n**, with **n** being a non-negative whole number.
We use the specifying term **n-holor**.

**A:** The **0-holor**, is a **scalar**.
It can be represented as real (or complex) number.
All possible values are allowed.

**B:** The **(n+1)-holor** is an array of **n-holors**.
We call the array-size: **Leading Dimension**.

Each element is an **n-holor** and each has the the same leading dimension.
Values across elements are independent.

### Shape
Above definition implies that an n-holor is a hierarchical composition
of n sub-holors: { n-holor, (n-1)-holor, ... , 1-holor }, each with a specified
leading dimension. 

We call the ordered set (or sequence) of 'n' leading dimensions **shape**.
The shape of the 0-holor is the empty set: \{ \}.

It follows from the definition that all elements of a holor must have the same shape.

One can mentalize the holor as n-orthotoype (or n-hypercuboid), with the 0-holor
being an *'atom'* (, which is an orthotoype of unit-size).

Constructing the (n+1)-holor means stacking multiple n-orthotoypes on each other
in a new dimension. Since each sub-orthotoype has a fixed and finite shape,
its atoms can be folded into linear space
(e.g. linear addressable memory) such that all sub-holors also retain a valid
representation. 

### Volume
We define **volume** as the product of all elements of **shape**.
<br>The 0-holor has volume **1** (*empty product*).
<br><sub> Note: *'volume'* is the related to Moon & Spencer's *'Number of Merates'*</sub>

### Value
We call the composite of all individual scalars of a given holor **the value** of that holor.
The value is stored as array of a specified data type. Currently the types f2_t (float) and f3_t
(double) are supported.

The holor-value can be seen as vector. Its size is equal to the volume of the holor. For a given shape
all scalars have a well defined order:

We obtain the order uniquely by adding axiom 'C' to a holor's definition:

**C:** The value of an **(n+1)-holor** is the concatenated array of the associated values 
of **n-holors** as specified in axiom 'B'.

### Undetermined Holor
We extend the definition of holor to include a objects with shape but no associated value.
Here the vale-size is zero, a number not possible for any holor defined so far.

**D:** We define an object with shape but no value and call it _undetermined holor_

*Consequently all holors with value are then called _determined_.*

### Examples
|Name|Description|Shape|Volume|
|:---|:---|:---|:---|
|0-Holor|Scalar|\{\}|1|
|1-Holor|Vector of size m|\{m\}|m|
|2-Holor|(n x m)-Matrix|\{n,m\}|n * m|
|3-Holor|Array of k elements; each being a (n x m)-Matrix|\{k,n,m\}|k * n * m|

## Operations
A holor has well defined operations like addition, multiplication, convolution, transposition, etc.
Where suitable operations are expressed as operators, which are dedicated objects.

## Holor Virtual Machine (HVM)
We define the HVM as virtual machine with holors as data elements
holor-operands as principal instructions. A HVM contains at least the following components:

1. A database for storing holors where each holor is addressable by an integer index.

2. A collection of _virtual machine operations_ (vop). Each vop representing
an operation of arity n on holors and containing an array of n+1 indices, where the
first n indices address the input holors and the last one the output holor of the
given operation. Normally, a vop can handle holors of all data types (f2_t or f3_t)
in any combination.

3. An executable array of vop, called _track_. 
Executing a track means executing all vop in the track from first to last in given sequence.

4. An database of tracks, called _library_.

Additional components are allowed but not mandatory.
Typically such components would include operations such as conditional execution, jump, loop, 
branch, call-subroutine, etc (turning a HVM into a Turing Machine).

------
<sub>&copy; 2019, 2020 Johannes B. Steffens</sub>
