# About Beth

*Beth* is a personal *"moonshot"* project.

In it I am re-thinking polymorphism, reflection-aided generic programming and functional programming. One goal is leveraging efficient application design with respect to scalability, maintainability and genericity.

Some objectives are...
   * Bridging the gap between procedural and object oriented programming.
   * Reducing interdependencies and enhancing flexibility in maintaining and extending code.
   * Providing the framework for reflection-base generic software design.
   * Dynamic type binding.
   * Binding interfaces and objects via associations.
   * Achieving high runtime efficiency.

## Concepts

### Reflection
Beth employs *Reflection* as basis for genericity. The reflection-framework allows dynamic object-definition using a declarative (string-based) syntax. A corresponding C-syle structure definition is not required. However, for static types both methods can be mixed, taking advantage of the strengths of either method as inspired by coding context.

The framework is used for object instantiation, initialization, lifetime management, serialization and more. Inside the beth codebase *reflection* is abbreviated `flect`.

### Perspective
A key-component is the so-called *Perspective*, which is a special abstraction for a generic framework with the character of a polymorphic interface but without requiring inheritance. Binding between *Perspective* and *Object* is dynamic. Neither need be aware of the other's existence at compile time, yet a *Perspective* may act as generic source of functionality for an object as well as its abstract interface. The perspective-framework builds upon the reflection framework. Inside the beth codebase *perspective* is abbreviated `spect`.

### Associative Binding
Dynamic associations between *types*, *reflections*, and *perspectives* are accomplished via hashmaps. For this purpose I experimented with Pagh's & Rodler's cuckoo-hashing and composed a fast & stable monolithic implementation of the algorithm.

### Memory Management & Garbage Collection
Beth has its own memory manager with integrated reference manager. Both components work closely together providing efficient garbage collection based on reference-counting. A special design-feature is the overhead-free (near) O(1) root-address-determination from any pointer adressing an embedded element of the object. This allows garbage collection, even after all direct pointers to the object have been released while only pointers to its elements remain in use. Once the last such element-reference is released, the root-object is automatically determined and destroyed.

The memory-manager, excluding reference manager, was spun-off into a stand-alone solution in project [tbman](https://github.com/johsteffens/tbman).

## Motivation
The classic framework for object oriented programming (such as realized in C++) has been widely cherished for decades. I appreciate OOP and applaud to the efficient way C++ supports it. Yet there are issues. For example: The static association between objects and their interfaces forces an inhibiting rigidity into the developer's code-architecture. The limited means of genericity often promots boiler plate code. More recent languages (e.g. Java, Python, Go) provide advanced features alleviating some of these issues. Still, they may come at a loss of efficiency and/or loss of control over how code and data is mapped onto the hardware.

During May-July 2017, in an attempt to find my own solution to generic polymorphism, I conceived the *reflection-perspective* approach. Project [beth](https://github.com/johsteffens/beth) shall bring this (and other releated ideas) into tangible shape.

In *beth* and dependent projects, the approach turned out useful for object-instance management, dynamic arrays, object-ordering, serialization and others.

## Usage
Beth serves as foundation library shared across more specialized projects. If you are interested in applications, you may want to use it in combination with a dependent repository (e.g. [actinon](https://github.com/johsteffens/actinon)). 

   * Download [beth](https://github.com/johsteffens/beth) together with a project that builds on it and compile both together.
   * Or: Develop your own application based on beth.

## License

The source code in this repository is licensed under the [Apache 2.0 License](https://github.com/johsteffens/beth/blob/master/LICENSE). 

*Copyright (C) 2017, 2018 Johannes B. Steffens johannes.b.steffens@gmail.com.*

