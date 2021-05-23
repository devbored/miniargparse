# miniargparse

A simple C/C++ argument parsing utility.

## Features
- Dependency free (excluding libc)
- Implemented as a single C/C++ header file
- Cross platform (Windows, macOS, Linux)
- Retrieval of positional argument(s)
    - (Only after parsing argv first)

## Thread Safety
One important note to keep in mind is that miniargparse utility is **not thread safe**.

## Option Formatting
- Options can have either a short-name, long-name, or both
- `MINIARGPARSE_OPT` macro parameters are the following (in order):
    - storeValue, shortName, longName, hasValue, description
- Options parsed from argv are expected to be prefixed with:
    - `-` for short-name options
    - `--` for long-name options
- The `storeValue` of an option is a struct that contains various info about that parsed option
- Options with `hasValue` set to 1 that are parsed from argv are expected to have the following formatting:
    - For short-name options, value must be next arg (Example: `-n 45`)
    - For long-name options, an `=<value>` should be appended to option (Example: `--number=45`)

## Example Usage
```c
#include <stdio.h>
#include "miniargparse.h"

int main(int argc, char *argv[]) {
    // Define options
    MINIARGPARSE_OPT(help, "h", "help", 0, "Print out help and exit.");
    MINIARGPARSE_OPT(verbose, "", "verbose", 0, "Enable verbose mode.");
    MINIARGPARSE_OPT(myValueOpt1, "", "myValueOpt1", 1, "Example value-option.");
    MINIARGPARSE_OPT(myValueOpt2, "m", "", 1, "Another example value-option.");

    // Parse
    int unknownOption = miniargparseParse(argc, argv);
    if (unknownOption > 0) {
        // Unknown option detected...
        printf("ERROR - Unknown option [ %s ] used.\n", argv[unknownOption]);
        // ...
    }

    // Get HEAD of option list and iterate over all the options to see if any option had an error
    miniargparseOpt *head = miniargparseOptlistController(NULL);
    while (head != NULL) {
        if (head->infoBits.hasErr) {
            // Option had an error when parsing
            printf("ERROR - %s [ Option: %s ]\n", head->errValMsg, argv[head->index]);
            // ...
        }
        head = head->next;
    }

    // Check-out parsed options
    if (help.infoBits.used) printf("Help option was given.\n");
    if (verbose.infoBits.used) printf("Verbose option was given.\n");
    if (myValueOpt1.infoBits.used) printf("myValueOpt1 was used - value is [ %s ].\n", myValueOpt1.value);
    if (myValueOpt2.infoBits.used) printf("myValueOpt2 was used - value is [ %s ].\n", myValueOpt2.value);

    // Get positional arg(s) - if any
    int positionalArgIndex = miniargparseGetPositionalArg(argc, argv, 0);
    while (positionalArgIndex != 0) {
        // Found positional arg at: argv[positionalArgIndex]
        // ...

        positionalArgIndex = miniargparseGetPositionalArg(argc, argv, positionalArgIndex);
    }

    // ...

    return 0;
}
```

## Building unit tests
[GoogleTest](https://github.com/google/googletest) is used as the unit testing framework. There are
a couple of different ways to build the tests:

If GoogleTest libs are already installed in a default system location

    $ cmake . -Bbuild
    $ cmake --build build

If GoogleTest libs are already installed in a non-default system location

    $ cmake -DGTEST_BASEDIR=<path_up_to_gtest_libs> . -Bbuild
    $ cmake --build build

## Building unit tests (with Conan)
If you don't have the gtest libs already installed, [Conan](https://docs.conan.io/en/latest/installation.html) can be
used to pull gtest in, then build with CMake.

Conan is cross-platform and implemented in Python - to install Conan:

    $ pip install conan

Use Conan to pull the gtest package and build with CMake

    $ conan install -if build conanfile.txt
    $ cmake . -Bbuild
    $ cmake --build build
