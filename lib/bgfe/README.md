# bgfe: Beth Graphical Front End

This library represents a graphical user interface for beth.

The principal idea is displaying and controlling public elements of an arbitrary object via its reflection in a generic way. This widely automates (thus simplifies) the otherwise often tedious part of designing a proper user interface around an object.

Rendering and controller interfacing us done using GTK3. For mode details about GTK3, see: https://docs.gtk.org/gtk3/

### Prerequisites

- Install python packages:
  - `sudo apt install libgtk-3-dev`
- One can use `python3-config` for release settings in the makefile:
  - `pkg-config --cflags gtk+-3.0`  for compiler flags
    
  - `pkg-config --libs gtk+-3.0` for linker flags

## Features and Interfaces

### Runtime Environment

The runtime environment (`RTE`) is an intermediate framework running in the background in a dedicated thread. It provides safe bridge between the `BGFE` user interface and the `GTK` functionality taking `GTK`'s thread requirements into account.

The `BGFE` user should not directly access the `RTE`. Instead `BGFE` sets up a the `RTE` automatically as needed. All communication between `BGFE` and `RTE` is properly protected such that the `BGFE` user interface can be used in arbitrary threads as described below.

### Thread Safety

A `BGFE` instance can be used in any thread without additional protection as long as the instance is only used in the thread where it was created. In this manner, multiple instances (frames, windows, etc) can be safely used in multiple threads without additional protection. 

If a `BGFE` instance shall be is accessible across (user-) threads, a standard protection of using critical sections (e.g. via `mutual exclusions`)  is required and sufficient.



## Issues

### Code::Blocks application: Dependency and link-order

#### Issue 

`pgk-config` computes the library list on the fly. It is normally placed under `Other linker options`. These options are executed in order but precede `Link libraries` in the target options. For gcc, if lib `a` depends on lib `b` , `a` must precede `b`. Hence, `libbeth.a` must precede `pgk-config` -libraries. 

#### Remedy
Manually place the path to `libbeth.a` as first element into `Other linker options` instead into the `Link libraries` field.

#### Issue 
Above remedy makes Code::Blocks unaware of the targets dependency to `libbeth.a`. 

#### Remedy
Add the dependency `libbeth.a` in the project settings: `Properties/Build tragets/Dependencies/External dependency files`.


<sub>&copy; 2025 Johannes B. Steffens</sub>
