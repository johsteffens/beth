# Table of Content
   * [About Beth](#about-beth)
   * [Usage](#usage)
   * [License](#license)
   * [Sub Libraries](#sub-libraries)
      * [bcore - Core Concepts](#bcore)
      * [bmath - Algebraic Subsystem](#bmath)
      * [bclos - Closures (Functional Programming)](#bclos)
      * [badapt - Adaptive Systems (Neural Networks)](#badapt)
      * [bhvm - Holor Virtual Machine](#bhvm)
   * [Core Concepts](#core-concepts)
      * [Reflection](#reflection)
      * [Perspective](#perspective)
      * [Associative Binding](#binding)
      * [RTTI](#RTTI)
      * [Memory Management](#memory-management)
      * [Plant (automatic code)](#plant)
   * [Motivation](#motivation)

# About Beth

*Beth* is a collection of foundation libraries covering basic objects, advanced 
numerical frameworks, a [meta-code framework](#plant) for high level software design
and a [framework for machine learning](#badapt) (in development).

It is a personal *"moonshot"* project in which I'm trying to bring several high-level programming
paradigms (such as OOP, Serialization, Dynamic Binding) together into efficient C code.
Then using this framework to develop useful solutions in numerical computing, 
machine learning and other disciplines.

Some objectives are...

   * Re-inventing polymorphism and reflection-aided generic programming. 
   * Bridging the gap between procedural and object oriented programming.
   * Reducing interdependencies and enhancing flexibility in maintaining and extending code.
   * Providing the framework for reflection-base generic software design.
   * Dynamic type binding.
   * Binding interfaces and objects via associations.
   * Achieving high runtime efficiency.
   * Providing baseline functionality covering ...
       * Containers (Arrays, Trees, Maps)
       * Serialization
       * Metacoding
       * Numerical Mathematics
       * Machine Learning (Neural Networks)

All code inside beth is designed/developed from scratch (no mere adaptation of preexisting code) 
with focus on platform independence and computational efficiency.

# Usage
Beth is intended as foundation library for more specialized projects. 

### Requirements/Dependencies
   * gcc (or similar compiler suite) supporting the C11 standard.
   * Library `pthread` of the POSIX.1c standard.

### Build with a project depending on beth
   * Depending repositories owned by me contain instructions to build everything.
   * Example: [actinon](https://github.com/johsteffens/actinon) - [How to use it](https://github.com/johsteffens/actinon#how-to-use-it).
    
### Build as stand-alone library
   * `git clone https://github.com/johsteffens/beth.git`
   * `cd beth/lib`
   * `make`

This creates the static library `beth.a`.
      
# License
The source code in this repository is licensed under the [Apache 2.0 License](https://github.com/johsteffens/beth/blob/master/LICENSE). 

*Copyright (C) 2017, ..., 2020 Johannes B. Steffens johannes.b.steffens@gmail.com.*

# Sub Libraries
Beth consists of a suite of sub-libraries.

<a name = "bcore"></a>
### bcore - Core Concepts
Library bcore represents the functional essence of Beth. It defines [core concepts](https://github.com/johsteffens/beth#core-concepts) and contains objects and/or abstractions around ...
   * Memory Management
   * Reference Management
   * Garbage Collection
   * Object Definition and Instance Management
   * Object Scope Management
   * Serialization
   * Arrays, Strings, Hash Maps etc.
   * ... and more

<a name = "bmath"></a>
### bmath - Algebraic Subsystem
Library bmath defines a linear algebra subsystem for vectors and matrices and provides a suite of functionality around numerical analysis. The following is offered:

   * Vector and Matrix subsystem with associated basic algebra (Addition, Multiplication, Inversion, ...).
   * Linear Solvers
   * Complex Numbers
   * Fourier Transformation (DFT and FFT)
   * Matrix Decomposition:
      * LU, QR, Cholesky
      * Bi-, Tri-Diagonalization
      * Eigen Values & Vectors (EVD)
      * Singular Values & Vectors (SVD)

All routines have been redesigned/redeveloped from scratch (no adaption of pre-existing code, no *fortranitis*, no dependence on a third-party linear algebra library). Many routines/algorithms are optimized for modern architectures in a cache- and platform-oblivious manner. Some carry new or unconventional ideas.

<a name = "bclos"></a>
### bclos - Closures (Functional Programming)
Library bclos contains a closure framework useful for for functional programming and interpreter design.
It is used by the Interpreter of the [Actinon Programming Language](https://github.com/johsteffens/actinon).

<a name = "badapt"></a>
### badapt - Adaptive Systems (Neural Networks)
This library represents a newly designed generic infrastructure for machine-learning. It utilizes the meta-language [beth-plant](#plant). High computationial efficiency is achieved by using the [bmath](#bmath) linar subsystem.
With this this concept, specific popular feed-forward and recurrent networks have (so far) been implemented:
   * Multi Layer Perceptron (MLP)
   * 1 Dimensional Convolutional Neural Network. (1D-CNN)
   * Elman Recurrent Network (ERN)
   * Long-Short-Term Memory Recurrent Network (LSTM)

<a name = "bhvm"></a>
### bhvm - Holor Virtual Machine (HVM)
A holor is a generic data type. It can be understood as generalization of a Tensor.
This library implements a virtual machine where operands are holors.
A virtual machine is a generalization of a von Neumann architecture.
It can run virtual machine code. 
Code for a HVM is compiled from a computational graph on holors.

[More details ...](https://github.com/johsteffens/beth/blob/master/lib/bhvm/README.md)

# Core Concepts

<a name = "reflection"></a>
### Reflection
Beth employs *Reflection* as basis for genericity. The reflection-framework allows dynamic object-definition using a declarative (string-based) syntax. A corresponding C-syle structure definition is not required. However, for static types both methods can be mixed, taking advantage of the strengths of either method as inspired by the coding context.

The framework is used for object instantiation, initialization, lifetime management, serialization and more. Inside the beth codebase *reflection* is abbreviated `flect`.

<a name = "perspective"></a>
### Perspective
A key-component is the so-called *Perspective*, which is a special abstraction for a generic framework with the character of a polymorphic interface with dynamic binding. *Perspective* and *Object* need not be aware of each other at compile time, yet a *Perspective* may act as generic source of functionality for an object as well as its abstraction. The perspective-framework builds upon the reflection framework. Inside the beth codebase *perspective* is abbreviated `spect`.

<a name = "binding"></a>
### Associative Binding
Concurrent dynamic associations between *types*, *reflections*, and *perspectives* are accomplished by two kinds of hashmaps: The frontend represents a cache using lock-free linear probing. The backend is realized via memory efficient cuckoo-hashing. 

<a name = "RTTI"></a>
### RTTI (self awareness)
Runtime type information is represented as hash value of the (text-based) identifier. A perspective stores this type-hash value about itself and another about the object it describes. Optionally, an object may also store its type-hash as first value (type `aware_t`). Such an object is considered *self-aware*. Advanced abstraction is available allowing type-oblivious coding and advanced runtime error checks. Most perspectives offer functions for self-aware objects (infix `_a_`), where the required perspective need not be known in advance but is retrieved automatically.

<a name = "memory-management"></a>
### Memory Management & Garbage Collection
Beth has its own memory manager with integrated reference manager. 
Both components work closely together providing efficient garbage collection based on reference-counting. 
A special design-feature is the overhead-free (near) O(1) root-address-determination from any pointer
adressing an embedded element of the object. 
This allows garbage collection, even after all direct pointers to the object have been released while
only pointers to its elements remain in use.
Once the last such element-reference is released,
the enveloping instance is automatically determined and destroyed.

A simplified version of the memory-manager (excluding reference management)
was spun-off into a stand-alone solution in project [tbman](https://github.com/johsteffens/tbman).

<a name = "plant"></a>
### Plant
Beth-Plant is a dedicated (mostly declarative) meta language intended to reduce boilerplate coding. It allows expressing key aspects of reflections and perspectives in a well-readable, compact and simple style. It uses a simple hierarchical namespace scheme. Plant-code can also wrap snippets of c-code in which namespaces can be used. The plant-compiler converts plant-code and c-code snippets into pure c-code.

Plant-code is embedded in c-code as non-parseable section, initiated by macro `PLANT_GROUP( <group-name>, <trait-name> )`. Source files containing plant-code are registered to the plant-compiler.

Key components of the plant language are `stamp`, `group` and `feature`.

`stamp` represents an object (or class). It can have member variables and functions. A stamp is defined inside a `group`.
`group` represents a declarative context. Groups can be nested.
`feature` represents a virtual function. It is defined inside a group turning it into a [perspective](#perspective).

# Motivation
Object oriented programming is generally associated with a specific framework which can be seen in 
many programming Languages. C++ is probably one of the most efficient ways of implementing this concept.
I appreciate and value it, but I also see issues.

For example: The static association between objects and their interfaces forces an inhibiting
rigidity into the developer's code-architecture.
The limited means of genericity often promotes boiler plate code.
More recent languages (e.g. Java, Python, Go) provide advanced features alleviating some of
these issues. Still, they may come at a loss of efficiency and/or loss of control over how
code and data is mapped onto the hardware. 

Overcoming these deficiencies is a key motivation. 'C' was preferred over 'C++' 
because most concepts introduced by 'C++' are solved differently here.

Another motivation is providing functionality as might be expected from a general purpose
library with a scientific touch.

Good prior ideas influencing this work are appreciated and cited inside the code.

However, a major part of this codebase is accomplished by actually questioning well
trodden paths and trying to come up with new ideas or at least viable variations
of what has been done before.

For that matter, getting a grasp on *beth* is probably a bit of a challenge for a
seasoned developer. I hope that those those who try anyway feel rewarded, and experience
it like discovering a new world. For me it is like painting a new picture, which has not
been painted before. Watching it unfold and taking shape is beautiful.

But for all bliss, the ultimate goal is *usefulness*.

# History

From April 2017: 

   * Developed beth-specific coding conventions.
   * Developed a dedicated memory manager.
   * Developed my own solution to generic polymorphism.
   * Conceived the *reflection-perspective* approach.
   * Started project [beth](https://github.com/johsteffens/beth) on github.

2018:

   * Developed the project [actinon](https://github.com/johsteffens/actinon), which is a ray-tracer 
     utilizing *beth* and demonstrating its usefulness.
   * Developed a numerical mathematics suite in sub-library [bmath](#bmath) with special focus on accrate,
     stable and efficient matrix factorization.

2019

   * Developed the meta-language [Beth-Plant](#plant).
   * Developed a new machine learning library [badapt](#badapt).
   * Designed a more generic approach to machine learning. 
   * Initially privately in [badapt_dev](https://github.com/johsteffens/badapt_dev): 
     Developed a new script language for defining mathematical graphs (networks)
     with adaptive and cyclic components. This is gradually being ported into *beth*.
   * Conceived the [holor virtual machine](#bhvm).
   * Named the script language "Haptive" and developed an initial compiler for it.
     Further ongoing development of *haptive* is codenamed "lion".
   
2020:

   * Made [badapt_dev](https://github.com/johsteffens/badapt_dev) public.
   * *Haptive/Lion* Language and Compiler: Continued development.
   
------

<sub>&copy; Johannes B. Steffens</sub>

