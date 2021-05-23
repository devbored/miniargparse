# miniargparse

A simple C/C++ argument parsing utility.

## Features
- Dependency free (excluding libc)
- Implemented as a single C/C++ header file
- Cross platform (Windows, macOS, Linux)

## Thread Safety
One important note to keep in mind is that miniargparse is **not thread safe**.

## Building unit tests
[GoogleTest](https://github.com/google/googletest) is used as the unit testing framework. There are
a couple of different ways to build the tests:

If GoogleTest libs are already installed in a default system location

    $ cmake . -Bbuild
    $ cmake --build build

If GoogleTest libs are already installed in a non-default system location

    $ cmake -DGTEST_BASEDIR=<path_up_to_gtest_libs> . -Bbuild
    $ cmake --build build

## Conan
If you don't have the gtest libs already installed, [Conan](https://docs.conan.io/en/latest/installation.html) can be
used to pull gtest in, then build with CMake.

Conan is cross-platform and implemented in Python - to install Conan:

    $ pip install conan

Use Conan to pull the gtest package and build with CMake

    $ conan install -if build conanfile.txt
    $ cmake . -Bbuild
    $ cmake --build build
