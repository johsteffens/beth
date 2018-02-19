# About Beth

*Beth* is a personal *"moonshot"* project.

In it I am re-thinking polymorphism, reflection-aided generic programming and functional programming. One goal is developing a paradigm in which frequently used design patterns can be efficiently yet more generally realized than by the traditional object oriented approach. Ultimately, this software framework shall leverage efficient application design with respect to scalability, maintainability and genericity.

Some of the objectives are...
   * Bridging the gap between procedural and object oriented programming.
   * Reducing interdependencies and enhancing flexibility in maintaining and extending code.
   * Providing the framework for reflection-base generic software design.
   * Dynamic type binding.
   * Binding interfaces and objects via associations.
   * Achieving high runtime efficiency.

## Concepts

### Reflection
Beth employs *Reflection* as basis for genericity. The reflection-framework allows dynamic object-definition using a declarative (string-based) syntax. It does not require a C-syle structure definition of the type. However, for static types both methods can be mixed taking advantage of either as the coding context might inspire.

The framework is used for object instantiation, initialization, lifetime management, serialization and more. Inside the beth codebase *reflection* is abbreviated *flect*.

### Perspective
A key-component is the so-called *Perspective*, which is a special abstraction for a generic framework with the character of a polymorphic interface but not following the tradition of object inheritance in OOP. Binding between *Perspective* and *Object* is dynamic. Neither need be aware of the other's existence at compile time, yet a *Perspective* may act as generic source of functionality for an object as well as its abstract interface. The perspective-framework builds upon the reflection framework. Inside the beth codebase *perspective* is abbreviated *spect*.

### Associative Binding
Dynamic associations between *types*, *reflections*, and *perspectives* are accomplished via hashmaps. For this purpose I experimented with the well known (Pagh & Rodler) cuckoo-hashing and developed a fast & stable monolithic flavor of the algorithm.

### Memory Management & Garbage Collection
Beth has its own memory manager with integrated reference manager. Both components work closely together providing efficient garbage collection, which is based on reference-counting. A special design-feature is O(1) root-address-determination from any offset-address into an object. It allows tracking of the reference state without additional overhead even when only references to sub-objects, which normally never own their memory, are known. Such might be references to any element of a dynamic array or structure.

The memory-manager was spun-off into a stand-alone solution in project [tbman](https://github.com/johsteffens/tbman).

## Motivation
The classic OOP-concept of classes and inheritance (as realized in C++) has been a cherished companion for decades but it has also flaws inhibiting flexibility in software design. Such are tightly knit associations between objects and interfaces and (certain) design patterns that promote boiler plate code. More recently developed languages introduce new and better concepts. But some come with the penalty of reduced runtime efficiency. Others leave the taste of an 'forced hybrid' between older and newer ideas.

Around May-July 2017, in an attempt to find a useful but significantly different solution for polymorphism and reflection, I conceived the current *reflection-perspective* approach. This project shall bring this (and other ideas) into tangible shape. So far the approach appears to be useful for object-instance management, dynamic arrays, object-ordering, serialization and other methods for which one might traditionally fall back to using 'design patterns'.

## Usage
Beth serves as foundation library shared across more specialized projects. If you are interested in applications, you may want to use it in combination with a dependent project (e.g. [actinon](https://github.com/johsteffens/actinon)). 

   * Download [beth](https://github.com/johsteffens/beth) together with a project builds on it and compile both together.
   * Or: Develop your own application based on beth.

## License

The source code in this project is licensed under the Apache 2.0 License. 

*Copyright (C) 2017, 2018 Johannes B. Steffens johannes.b.steffens@gmail.com.*

