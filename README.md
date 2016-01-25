# osrm-c

C-wrapper to the [Open Source Routing Machine - OSRM](https://github.com/Project-OSRM/osrm-backend).

## Why?

I wanted a fast Python binding for the library (check `example/`).
And I also think it's cool to have julia/rust/go bindings
(and a c-wrapper would just make those real-easy).

## Build

Building osrm-c requires osrm to be installed,
and all dependencies required for building osrm,
plus [scons](http://www.scons.org/) for building.

For how to build/install osrm, check
[osrm's wiki page](https://github.com/Project-OSRM/osrm-backend/wiki/Building-OSRM).

    git clone https://github.com/tdihp/osrm-c.git && cd osrm-c
    scons
    # scons install

Alternatively, you can just grab `osrm_c.cpp` and do whatever convenient.

## Versioning and Compatibility

Each release of osrm-c will be able to compile against at least **ONE RELEASE**
of osrm-backend. See `CHANGELOG.md` for release information.

However `osrm-c.h` shall remain backward-compatible, unless noted otherwise
in `CHANGELOG.md`.

## Vagrant

To aid development and testing of this wrapper, I've provided vagrant config.

However, when using windows host, symlink
[won't work](http://stackoverflow.com/questions/24200333/symbolic-links-and-synced-folders-in-vagrant).

Use the following fix before running scons:

    mkdir -p /vagrant/build
    sudo mount -t tmpfs -o size=1G tmpfs /vagrant/build
    sudo chown vagrant:vagrant /vagrant/build
