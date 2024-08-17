# Getting Started

```
$ git clone https://github.com/johsteffens/beth.git
$ cd beth
$ make
```

You can optionally run a battery of tests
to ensure beth works correctly on your platform:

```
$ cd test
$ make run
```

# About Beth

*Beth* is a collection of foundation libraries covering basic objects, advanced 
numerical frameworks, a [dedicated language](#xoila) for high level software design
and a [framework for machine learning](#badapt) (in development).

It is a personal *"moonshot"* project in which I'm trying to bring several high-level programming
paradigms (such as Polymorphism, Encapsulation, Dynamic Binding, Serialization) together into efficient C code.
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

# License
The source code in this repository is licensed under the [Apache 2.0 License](https://github.com/johsteffens/beth/blob/master/LICENSE). 

*Copyright (C) 2017, ..., 2024 Johannes B. Steffens johannes.b.steffens@gmail.com.*

# Core Concepts

### Reflection
Beth employs *Reflection* as basis for genericity. The reflection-framework allows dynamic object-definition using a declarative (string-based) syntax. A corresponding C-syle structure definition is not required. However, for static types both methods can be mixed, taking advantage of the strengths of either method as inspired by the coding context.

The framework is used for object instantiation, initialization, lifetime management, serialization and more. Inside the beth codebase *reflection* is abbreviated `flect`.

<a name = "perspective"></a>

### Perspective
The *Perspective* is the answer to a common requirement: "I want to see (use) my object under a specific (abstract) aspect, e.g. as a mere container of data, as an array, as a set, as sortable, as streamable, etc". Thus, the perspective takes up the notion of an interface. In beth, this is achieved via dynamic binding. There is no need for inheritance or subtyping. *Interface* and *Object* need not be aware of each other at compile time.

The perspective can make active use of the object's reflection to analyze and manipulate it as wanted. For example: There might be infinite ways of designing a list or array, but the array-perspective can handle anyone providing typically needed methods for array manipulation (random access, appending, deleting, sorting, etc).

Inside the beth codebase *perspective* is abbreviated `spect`.

### Associative Binding
Concurrent dynamic associations between *types*, *reflections*, and *perspectives* are accomplished by two kinds of hashmaps: The frontend represents a cache using lock-free linear probing. The backend is realized via memory efficient cuckoo-hashing. 

### RTTI (self awareness)
Runtime type information is represented as hash value of the (text-based) identifier. A perspective stores this type-hash value about itself and another about the object it describes. Optionally, an object may also store its type-hash as first value (type `aware_t`). Such an object is considered *self-aware*. Advanced abstraction is available allowing type-oblivious coding and advanced runtime error checks. Most perspectives offer functions for self-aware objects (infix `_a_`), where the required perspective need not be known in advance but is retrieved automatically.

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

### Xoila
Xoila (abbreviated 'X') is a highly structured object oriented programming language.
It is conceived for software projects using beth. Its syntax bases on C, borrowing
a few familiar syntax elements from higher level languages. 
Beyond that, X has its own syntax, conceived for safe and compact coding and
advanced structuring.

By this time, X has grown into a viable alternative to C or C++. It allows designing highly compact,
yet well readable source code yielding fast executing programs. Considerable portions in Beth 
have been ported to 'X' because the new code is much better maintainable. 

A program written in X also tends to build significantly faster than comparable C++ code.
X provides many high level features such as polymorphism, reflection, object reference management, 
object lifetime management, generic dynamic arrays, dynamic arrays of virtual objects, iteration,
generic serialization (binary and editable text), namespaces, etc.

The language is defined by the compiler [xoico](https://github.com/johsteffens/beth/xoico), 
which is itself 100% coded in xoila. Xoico and Beth form an ecosystem.

Dedicated (pure) X code resides in files with extension *.x.
X-code can also be embedded in C-header-code, 
initiated by macro `XOILA_DEFINE_GROUP( <group-name>, <trait-name> )`.

Key components of the xoila language are `stamp`, `group` and `feature`.

`stamp` represents an object (or class). It can have member variables and functions.
A stamp is defined inside a `group`.
`group` represents a declarative context. Groups can be nested.
`feature` represents a virtual function.

Xoila uses a simple hierarchical namespace-scheme.

The language is still in development. Once it
reaches (sufficient) feature-completeness, it will be better documented.

# Sub Libraries

Beth consists of a suite of sub-libraries.

### bcore
**Purpose: Core Concepts**

Library bcore represents the functional essence of Beth. It defines [core concepts](https://github.com/johsteffens/beth#core-concepts) and contains objects and/or abstractions around ...

 * Memory Management
   * Reference Management
   * Garbage Collection
   * Object Definition and Instance Management
   * Object Scope Management
   * Serialization
   * Arrays, Strings, Hash Maps etc.
   * ... and more

### bmath
**Purpose: Algebraic Subsystem**

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

### bclos
**Purpose: Closures**

Library bclos contains a closure framework useful for for functional programming and interpreter design.
It is used by the Interpreter of the [Actinon Programming Language](https://github.com/johsteffens/actinon).

### badapt
**Purpose: Adaptive Systems**

This library represents a newly designed generic infrastructure for machine-learning.
It utilizes the meta-language [xoila](#xoila).
High computationial efficiency is achieved by using the [bmath](#bmath) linar subsystem.
With this this concept, specific popular feed-forward
and recurrent networks have (so far) been implemented:

   * Multi Layer Perceptron (MLP)
   * 1 Dimensional Convolutional Neural Network. (1D-CNN)
   * Elman Recurrent Network (ERN)
   * Long-Short-Term Memory Recurrent Network (LSTM)

### bhvm
**Purpose: Holor Virtual Machine**

A holor is a generic data type. It can be understood as generalization of a Tensor.
This library implements a virtual machine where operands are holors.
A virtual machine is a generalization of a von Neumann architecture.
It can run virtual machine code. 
Code for a HVM is compiled from a computational graph on holors.

[More details ...](https://github.com/johsteffens/beth/blob/master/lib/bhvm/README.md)

### bhpt
**Purpose: Holor Based Adaptive System**

Redesigned machine learning framework based on holors. This library eventually replaces badapt.

### bmedia
**Purpose: Audio and Video Interfaces**

This library provides interfaces for video capture, audio capture and audio play. The video interface bases on the video for Linux API. The audio interface bases on the ALSA API. 

### bcodec
**Purpose: Audio and Video Codecs**

This library contains a an interfaces for audio and videio codecs.

**audio_codec_waw**

```adio_codec_waw``` is a newly designed specific codec for audio compression/decompression, which is a *reinvention* based on state-of-the-art techniques around modified discrete cosine transform and several of my own ideas. It uses an equal loudness contour (based on Fletcher-Munron-Characteristic (ISO 226)). It mitigates pre-echo by analyzing time-loudness level and modulating the compression rate accordingly. Compression rates and compression quality is configurable. Useful rates are around 56 ... 240 kbps (average on 2-channels at 44.1 kHz). At 240kbps the difference to the original is practically imperceptible.
# Motivation
Object oriented programming is generally associated with a specific
framework which can be seen in many programming Languages.
I'd say that C++ offers one of the most efficient frameworks. 
I've used, appreciated and valued it for decades. (Still do.)
But that language has also issues, which caused some
resentment over time.

For example: 

* The static association between objects and their interfaces forces an inhibiting rigidity onto the developer's code-architecture.
* The limited means of genericity promotes boiler plate coding.
* The large increase of built-in features over the years. Of which only a fraction appears to be truly needed in any given code base.
* Consequently rather slow compilation.

More modern languages (e.g. Java, Python, Go) provide advanced features as well as 
alleviating some of above issues. Still, they may come at a loss of efficiency and/or
loss of control over how code and data is mapped onto the hardware.

Overcoming these deficiencies is a key motivation in project *beth*. 
The language 'C' was preferred over 'C++' because most concepts that
set 'C++' apart from 'C' have been solved differently in *beth*.
This effort culminates in the language [Xoila](#xoila).

Another motivation is providing functionality as might be expected from a general purpose
library with a scientific touch.

The codebase of *beth* is original work.
Specific prior work, which has influenced its development is cited inside the code 
or in related documents where feasible.
However, a major part is accomplished by actually questioning well trodden paths.
I've been trying to come up with new ideas or at least viable variations of prior solutions.

For that matter, getting a grasp on *beth* is probably a bit of a challenge 
even for a seasoned developer. I hope that who tries it, feels rewarded.
For me it is like painting a new picture, which has not been painted before. 
Watching it unfold and taking shape is beautiful.

But for all the bliss, the ultimate goal is *usefulness*.

# History

**From April 2017**

* Conceived beth-specific coding conventions.
* Developed a dedicated memory manager.
* Conceived the *reflection-perspective* approach.
* Started project [beth](https://github.com/johsteffens/beth) on github.

**2018**

* Developed the project [actinon](https://github.com/johsteffens/actinon), which is a ray-tracer 
     utilizing *beth* and demonstrating its usefulness.
* Developed a numerical mathematics suite in sub-library [bmath](#bmath) with special focus on accrate,
     stable and efficient matrix factorization.

**2019**

* Developed the meta-language [Beth-Plant](#xoila).
* Developed a new machine learning library [badapt](#badapt).
* Designed a more generic approach to machine learning. 
* Initially privately in [badapt_dev](https://github.com/johsteffens/badapt_dev): 
     Developed a new script language for defining mathematical graphs (networks)
     with adaptive and cyclic components. This is gradually being ported into *beth*.
* Conceived the [holor virtual machine](#bhvm).
* Named the script language "Haptive" and developed an initial compiler for it.
     Further ongoing development of *haptive* is codenamed "lion".

**2020**

* Made [badapt_dev](https://github.com/johsteffens/badapt_dev) public.
* *Haptive/Lion*: Language and Compiler: Continued development.
* *Haptive/Lion*: Continued development now codenamed *Opal*
* Reimplemented and renamed meta-language beth-plant: [Xoila](#xoila).
* Implemented library bhvm.
* Implemented library bhpt.

**2021**

* Designed interface for audio and video streaming.
* Implemented library bmedia.

**2022**

* Developed an audio compression codec
* Implemented library bcodec.

**2023**

* Began work on improving SVD speed stability and accuray in repository ```bmath_dev```.

**2024**

* Ongoing work on improving SVD.


------

<sub>&copy; Johannes B. Steffens</sub>

