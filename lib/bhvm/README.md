# Beth Holor Virtual Machine (BHVM)

## Holor
Our definition of holor was inspired by *Moon & Spencer's: "Theory of Holors"*.

A holor can be understood as generalization of a tensor. In the scope of this library the holor, is physically 
represented as multidimensional array. Unlike Moon & Spencer, jagged arrays are explicitly excluded.

We call the number of array-dimensions the *"order of a holor"*. A holor of order **n**, is abbreviated **n-holor**.

### Inductive Definition
The **0-holor** is a **Scalar**, which can be represented as real or complex number.

The **(n+1)-holor** is an array with specified number of elements. 
We call the number of elements the holor's **leading dimension**.
Each element is a **n-holor** of **same leading dimension**.

### Shape
Above definition implies that a n-holor is a hierarchical composition
of n sub-holors: { n-holor, (n-1)-holor, ... , 1-holor }, each with a specified
leading dimension. 

We call the ordered set (or sequence) of 'n' leading dimensions: **Shape**.

The shape of the 0-holor is the empty set: {}.

### Volume
The product of all elements of the Shape is called **Volume**.
<br>The 0-holor has Volume **1** (*empty product*).
<br><sub> Note: *'Volume'* is the same as Moon & Spencer's *'Number of Merates'*</sub>

### Examples
|Name|Description|Shape|Volume|
|:---|:---|:---|:---|
|0-Holor|Scalar|{}|1|
|1-Holor|Vector of size m|{m}|m|
|2-Holor|(n x m)-Matrix|{n,m}|n*m|
|3-Holor|Array of k elements; each being a (n x m)-Matrix|{k,n,m}|k*n*m|

## Operations
A holor has well defined operations like addition, multiplication, transposition, etc.
Where suitable operations are expressed as operators, 
which are dedicated objects.

## Holor Virtual Machine
The holor virtual machine is a virtual machine with holors as data elements
holor-operands as principal instructions. Accompanied can be instructions
controlling program flow.

------
<sub>(C) Johannes B. Steffens</sub>