# bgfe: Beth Graphical Front End

This library represents a generic graphical user interface for beth. It automates the actual framework widgets for any beth object.

Detailed Documentation: [bgfe.md](../../doc/bgfe/bgfe.md)

BGFE uses GTK3 as GUI back-end. (See also: https://www.gtk.org/ )

## Frame
The frame represents the virtual user interface controller. Specific implementations control specific aspects of a user interface, such as text editor, scales, sliders, switches. Other implementations (specifcally `bfge_frame_s, bgfe_window_s`) represents a list of sub-frames. Thus a tree of frames can be constructed analog to the way a regular beth-object represents a tree of sub-objects.

The frame can be seen as server for the user-interactive needs of an object. The object served by a frame is called **Client**.

## Client
The client is an object monitored and served by the graphical front-end. Any beth-object with a reflection can be client. The BGFE uses the reflection for automatic construction and interfacing with the object. In an ideal world the client defines the entire user interface simply by having a refletion. Thus the full frame can be generated from just one function (e.g. `set_client_with_content`). 

In practice, though, the intended use case may vary even in the case of identical reflections, thus calling for some form adaptability. BGFE provides a simple framework for adaptations via features in `bgfe_client.x`,

## Runtime Environment
The runtime environment (`RTE`) is an intermediate framework running in the background in a dedicated thread. It provides safe bridge between the `BGFE` user interface and the `GTK` functionality taking `GTK`'s thread requirements into account.

The `BGFE` user should not directly access the `RTE`. Instead `BGFE` sets up a the `RTE` automatically as needed. All communication between `BGFE` and `RTE` is properly protected such that the `BGFE` user interface can be used in arbitrary threads as described below.

## Thread Safety
A client-frame combination can be used in any thread without additional protection as long as the instance is only used in the thread where it was created. In this manner, multiple instances can be used in multiple threads. This is safe even for GTK because the RTE ensures that all GTK functionality runs in one dedicated single thread.

## Prerequisites
- Install gtk3 packages:
  - `sudo apt install libgtk-3-dev`
- One can use `pkg-config` for release settings in the makefile:
  - `pkg-config --cflags gtk+-3.0`  for compiler flags
    
  - `pkg-config --libs gtk+-3.0` for linker flags

### Issues

#### Code::Blocks: pkg-config
`pgk-config` computes the library list on the fly. It is normally placed under `Other linker options`. These options are executed in order but precede `Link libraries` in the target options. For gcc, if lib `a` depends on lib `b` , `a` must precede `b`. Hence, `libbeth.a` must precede `pgk-config` -libraries. 

#### Remedy
Manually place the path to `libbeth.a` as first element into `Other linker options` instead into the `Link libraries` field.

#### Code::Blocks: Dependency awareness
Above remedy makes Code::Blocks unaware of the targets dependency to `libbeth.a`. 

#### Remedy
Add the dependency `libbeth.a` in the project settings: `Properties/Build tragets/Dependencies/External dependency files`.



---------------

<sub>&copy; 2025 Johannes B. Steffens</sub>
