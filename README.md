# osrm-c

C-binding to the [Open Source Routing Machine - OSRM](https://github.com/Project-OSRM/osrm-backend).

## Why?

I wanted a fast Python binding for the library.
And I also think it's cool to have julia/rust/go bindings.

> It's the OSRM binding I deserves, but not the one I needs.

## Build

building osrm-c requires osrm to be installed,
all dependencies required for building osrm, and scons.

    git clone https://github.com/tdihp/osrm-c.git && cd osrm-c
    scons
    # scons install
