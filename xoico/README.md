# Xoila Compiler
(Continued from https://github.com/johsteffens/xoico)

Xoico is a self-hosting compiler for the [xoila programming language](https://github.com/johsteffens/beth#xoila).

It generates c-source code, which is stored in files named `*.xo.{c,h}`.

Configuration files `*_xoico.cfg` specify how xoico is to navigate through the sources.

***More details will follow ...***

## Getting Started

**Building xoico:**

```
$ git clone https://github.com/johsteffens/beth
$ git clone https://github.com/johsteffens/xoico
$ cd xoico
$ make
```

**Using xoico:**

```
$ ./bin/xoico --help
```

The [makefile of beth](https://github.com/johsteffens/beth/blob/master/makefile) shows 
how xoico is used  to build [beth](https://github.com/johsteffens/beth) in conjunction
with a C-Compiler.

## About Xoila

Xoila is a programming-language, which was designed along with
the [beth](https://github.com/johsteffens/beth)-framework.

Xoila-code is can be embedded in c-header files. 
It can also occur in dedicated files, which have the extension `*.x`.

Inside a header file the xoila-section is enclosed by #ifdef XOILA_SECTION ... #endif.
The (pseudo-)macro XOILA_SECTION is to be unknown by the C-compiler,
so it will not try to interpret the enclosed code.
Xoico, on the other hand, uses this keyword to determine the code block to be compiled.

Xoila is used in conjunction with library bcore of [beth](https://github.com/johsteffens/beth).
It provides a convenient programming layer unlocking core concepts of project beth.

(*Xoila* was initially called *Beth-Plant*.)

The language is still in development. Once it reaches maturity it shall be better document.
Until then the code for this compiler can be used as basis to learn more about the language.

------

<sub>&copy; Johannes B. Steffens</sub>

