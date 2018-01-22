# About Beth

*Beth* is a personal "moonshot" project.

In it I am re-thinking polymorphism, reflection-aided generic programming and functional programming. One goal is developing a paradigm in which frequently used design patterns can be efficiently yet more generally realized than by the traditional object oriented approach. The ultimate goal is designing a new software framework which optimally leverages efficient application design with respect to scalability, maintainability and genericity.

## Concepts

One pivoting element is the so-called *Perspective*, which is a special abstraction for a generic framework with the character of a polymorphic interface but not following the tradition of object inheritance in OOP. Binding between Perspective and Object is dynamic. Neither Perspective nor Object need be aware of the other's existence at compiletime, yet a Perspective may act as generic source of functionality for an object as well as its abstract interface.

I conceived this approach in May 2017. This project is the attempt of bringing this (and other ideas) into tangible shape. So far the Perspective turned out useful for object-instance management, dynamic arrays, object-ordering, serialization and other concepts for which one might traditionally fall back to using 'design patterns'.

*More specific documentation is yet to come...*

Some of the objectives are...
   * Bridging the gap between procedural and object oriented programming.
   * Reducing interdependencies and enhancing flexibility in maintaining and extending code.
   * Providing the framework for reflection-base generic software design.
   * Dynamic type binding.
   * Binding interfaces and objects via associations.
   * Achieving high runtime efficiency.

## Usage

This project serves as foundation library shared across more specialized projects. If you are interested in applications, you may want to use beth in combination with a dependent project (e.g. [actinon](https://github.com/johsteffens/actinon)). 

   * Download [beth](https://github.com/johsteffens/beth) together with a project builds on it and compile both together.
   * Or: Develop your own application based on beth.

## License

The source code in this project is licensed under the Apache 2.0 License. 

*Copyright (C) 2017 Johannes B. Steffens johannes.b.steffens@gmail.com.*

