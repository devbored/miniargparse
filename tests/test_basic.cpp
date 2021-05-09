#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <signal.h>
#include <algorithm>

#include <gtest/gtest.h>
#include "miniargparse.h"

TEST(miniargparse, test_works) {

    MINIARGPARSE_OPT(verbose, "v", "verbose", 0, "Enable verbose message.");
    MINIARGPARSE_OPT(help, "h", "help", 0, "Print out help message.");

    // Print out descriptions of each arg
    std::cout << verbose.used << std::endl;
}
