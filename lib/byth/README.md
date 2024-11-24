# Embedded Python via Python/C API

This library provides python functionality in beth by using the &#x1F517;[Python/C API](https://docs.python.org/3/c-api/index.html) .

### Prerequisites

- Install python packages:
  - `sudo apt install python3-dev`
  - `sudo apt install python3-matplotlib`

- One can use `python3-config` for release settings in the makefile:
  - `python3-config --embed --cflags`  for compiler flags
    - Additional compiler flags to counteract some undesired `python3-config` settings: `-Wno-sign-compare -Wno-comment`

  - `python3-config --embed --ldflags` for linker flags

- Debug mode: `python3-config` does not seem to offer a dedicated debug mode. Workaround: Dissect the returned flags and adapt them to the desired debug settings. Only compiler flags need modifications. Following additional settings work for the current python3 release (might change in future)
  - `-I/usr/include/python3.10 -fstack-protector-strong`


## Features and Interfaces

[Object Interface](../../doc/byth/object.md): Low level interface for conversions across beth objects and python objects.
[Plotting Interface](../../doc/byth/plot.md): Wrapper of functionality in the `matplotlib.pyplot` package.

------
<sub>&copy; 2024 Johannes B. Steffens</sub>
