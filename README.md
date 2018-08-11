# Table of Content
   * [About Beth](#about-beth)
   * [Usage](#usage)
   * [License](#license)
   * [Sub Libraries](#sub-libraries)
      * [bcore - Core Concepts](#bcore)
      * [bmath - Algebraic Subsystem](#bmath)
      * [bclos - Closures (Functional Programming)](#bclos)
   * [Core Concepts](#core-concepts)
      * [Reflection](#reflection)
      * [Perspective](#perspective)
      * [Associative Binding](#binding)
      * [Memory Management](#memory-management)
   * [Motivation](#motivation)

# About Beth

*Beth* is a personal *"moonshot"* project. 

It brings high-level programming paradigms (such as OOP, Serialization, Dynamic Binding) into efficient C code. It also (in a way) reinvents polymorphism and reflection-aided generic programming. 

Some objectives are...
   * Bridging the gap between procedural and object oriented programming.
   * Reducing interdependencies and enhancing flexibility in maintaining and extending code.
   * Providing the framework for reflection-base generic software design.
   * Dynamic type binding.
   * Binding interfaces and objects via associations.
   * Achieving high runtime efficiency.
   * Providing baseline functionality covering ...
      * Containers (Arrays, Trees, Maps)
      * Serialization
      * Numerical Mathematics

All code inside beth is redesigned/redeveloped from scratch (no mere adaptation of preexisting code) with focus on platform independence and efficiency.

# Usage
Beth is intended as foundation library for more specialized projects. 

### Requirements/Dependencies
   * gcc (or similar compiler suite) supporting the C11 standard.
   * Library `pthread` of the POSIX.1c standard.

### Build with a project depending on beth
   * Depending repositories owned by me contain instructions to build everything.
   * Example: [actinon](https://github.com/johsteffens/actinon) - [How to use it](https://github.com/johsteffens/actinon#how-to-use-it).
    
### Build as stand-alone library
   * Downlowd [beth](https://github.com/johsteffens/beth).
   * In a terminal ...
      * enter folder `beth-master/lib`.
      * run `make`. This creates the static library `beth.a`.
      
# License
The source code in this repository is licensed under the [Apache 2.0 License](https://github.com/johsteffens/beth/blob/master/LICENSE). 

*Copyright (C) 2017, 2018 Johannes B. Steffens johannes.b.steffens@gmail.com.*

# Sub Libraries
Beth consists of a suite of sub-libraries.

<a name = "bcore"></a>
### bcore - Core Concepts
Library bcore represents the functional essence of Beth. It defines [core concepts](https://github.com/johsteffens/beth#core-concepts) and contains objects and/or abstractions around ...
   * Memory Management
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

<a name = "memory-management"></a>
### Memory Management & Garbage Collection
Beth has its own memory manager with integrated reference manager. Both components work closely together providing efficient garbage collection based on reference-counting. A special design-feature is the overhead-free (near) O(1) root-address-determination from any pointer adressing an embedded element of the object. This allows garbage collection, even after all direct pointers to the object have been released while only pointers to its elements remain in use. Once the last such element-reference is released, the enveloping instance is automatically determined and destroyed.

The memory-manager, excluding reference manager, was spun-off into a stand-alone solution in project [tbman](https://github.com/johsteffens/tbman).

# Motivation
The classic framework for object oriented programming (such as realized in C++) has been widely cherished for decades. I appreciate OOP and like the efficient way C++ supports it. Yet there are issues. For example: The static association between objects and their interfaces forces an inhibiting rigidity into the developer's code-architecture. The limited means of genericity often promotes boiler plate code. More recent languages (e.g. Java, Python, Go) provide advanced features alleviating some of these issues. Still, they may come at a loss of efficiency and/or loss of control over how code and data is mapped onto the hardware.

### History
In an attempt to find my own solution to generic polymorphism, I conceived the *reflection-perspective* approach during Summer 2017 and started project [beth](https://github.com/johsteffens/beth) bringing this (and other ideas) into tangible shape. Winter 2017/2018 the project [actinon](https://github.com/johsteffens/actinon) was developed utilizing the framework and demonstrating its usefulness. Spring 2018 I started develping a a numerical mathematics suite with special focus on accrate, stable and efficient matrix factorizations in sub-library [bmath](#bmath).
