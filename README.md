# About Beth

*Beth* is a personal "moonshot" project.

In it I am re-thinking polymorphism, reflection-aided generic programming and functional programming. One goal is developing a paradigm in which frequently used design patterns can be efficiently yet more generally realized than by the traditional object oriented approach. The ultimate goal is designing a new software framework which optimally leverages efficient application design with respect to scalability, maintainability and genericity.

## Concepts

One pivoting element is the so-called *Perspective*, which is a special abstraction for a generic framework with the character of a polymorphic interface but not following the tradition of object inheritance in OOP. Binding between Perspective and Object is dynamic. Neither Perspective nor Object need be aware of the other's existence at compiletime, yet a Perspective may act as generic source of functionality for an object as well as its abstract interface.

I conceived this approach in May 2017. This project is the attempt of bringing this (and other ideas) into tangible shape. So far the Perspective turned out useful for object-instance management, dynamic arrays, comparison, serialization and other concepts for which one might traditionally fall back to using 'design patterns'.

(More specific documentation is yet to come.)

Some of the objectives are...
   * Bridging the gap between procedural and object oriented programming.
   * Reducing interdependencies and enhancing flexibility in maintaining and extending code.
   * Providing the framework for reflection-base generic software design.
   * Dynamic type binding.
   * Binding interfaces and objects via associations.
   * Achieving high runtime efficiency.

## License Status

This project is under development. Once the code structure has sufficiently settled, I’ll decide about a public license. 
For the time being: If you wish to use/copy/fork/integrate all or parts of it (other than granted by Git Hub), please contact me for a personal license. - Thank you.

Note that this software comes without any explicit or implied warranty for any purpose.

*Copyright (C) 2017 Johannes Steffens johannes.b.steffens@gmail.com. All rights reserved.*

