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
We use the specifying term **n-Holor**.

The simplest holor is the **0-holor**, which is a **scalar**. 
It can be represented as real (or complex) number.
All possible values are allowed.

The **(n+1)-holor** is an array of **n-holors**.
We call the array-size: **Leading Dimension**.

Each element is an **n-holor** and each has the the same leading dimension.
The values across elements are independent.

### Shape
Above definition implies that a n-holor is a hierarchical composition
of n sub-holors: { n-Holor, (n-1)-Holor, ... , 1-Holor }, each with a specified
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

There are different ways to encode this. The method we apply is like the way
multidimensional arrays represented by the programming language 'C'.

### Volume
We define **volume** as the product of all elements of **shape**.
<br>The 0-holor has volume **1** (*empty product*).
<br><sub> Note: *'volume'* is the related to Moon & Spencer's *'Number of Merates'*</sub>

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

## Holor Virtual Machine
The holor virtual machine is a virtual machine with holors as data elements
holor-operands as principal instructions. Additional instructions
controlling program flow (loops, branches) can be part of the virtual machine.

------
<sub>&copy; 2019, 2020 Johannes B. Steffens</sub>