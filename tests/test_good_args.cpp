#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <signal.h>
#include <algorithm>

#include <gtest/gtest.h>
#include "miniargparse.h"

TEST(miniargparse, test_valid_opts_basic) {
    // Mock argv and argc
    char *argv[] = {
        (char*)"<EXECUTABLE>",
        (char*)"-h",
        (char*)"--verbose",
        (char*)"-d",
        (char*)"6",
    };
    int argc = sizeof(argv) / sizeof(char*);

    // Add and parse opts
    MINIARGPARSE_OPT(help, "h", "help", 0, "Print out help and exit");
    MINIARGPARSE_OPT(verbose, "v", "verbose", 0, "Add verbosity");
    MINIARGPARSE_OPT(data, "d", "data", 1, "Test data-value option");
    int unknownOpt = miniargparseParse(argc, argv);
    EXPECT_EQ(unknownOpt, 0);

    // Basic validation of parsed opts
    EXPECT_EQ(help.infoBits.used, 1);
    EXPECT_EQ(verbose.infoBits.used, 1);
    EXPECT_EQ(data.infoBits.used, 1);
}
