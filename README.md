[![Build Status](https://img.shields.io/travis/netorcai/netorcai-client-cpp/master.svg?maxAge=600)](https://travis-ci.org/netorcai/netorcai-client-cpp)
[![Coverage Status](https://img.shields.io/codecov/c/github/netorcai/netorcai-client-cpp/master.svg?maxAge=600)](https://codecov.io/github/netorcai/netorcai-client-cpp)

netorcai-client-cpp
===================
C++ version of the [netorcai] client library.

Installation
============
The library uses the [Meson] build system (and therefore uses [Ninja]).

``` bash
# Create build directory.
meson build # --prefix=/desired/installation/prefix

# Build the library (and the unit test executables).
cd build
ninja

# Run the unit tests
ninja test

# Install the library
ninja install
```

Usage
=====
Feel free to look at [hello world examples](./examples) to build your own clients.

[netorcai]: https://github.com/netorcai/
[Meson]: https://mesonbuild.com/
[Ninja]: https://ninja-build.org/
