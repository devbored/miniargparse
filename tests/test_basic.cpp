#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <signal.h>
#include <algorithm>

#include <gtest/gtest.h>
#include "miniargparse.h"

void printOptItems(miniargparseOpt opt) {
    std::cout << "Opt fields:\n---\n";
    std::cout << "Short name : " << opt.shortName << std::endl;
    std::cout << "Long name : " << opt.longName << std::endl;
    std::cout << "Has value? : " << opt.hasValue << std::endl;
    std::cout << "Description : " << opt.description << std::endl;
    std::cout << "Used? : " << opt.used << std::endl;
    std::cout << "Value : " << opt.value << std::endl;
    std::cout << "Has value error? : " << opt.hasErrVal << std::endl;
    std::cout << "Value error : " << opt.errValMsg << std::endl;
    std::cout << "---\n\n";
}

TEST(miniargparse, test_works) {

    int argc = 4;
    char *argv[] = {0, "-b", "--verbose", "-d"};

    MINIARGPARSE_INIT(argc, argv);
    MINIARGPARSE_OPT(verbose, "v", "verbose", 0, "Enable verbose message.");
    MINIARGPARSE_OPT(help, "h", "help", 0, "Print out help message.");
    MINIARGPARSE_OPT(data, "d", "data", 1, "Test data.");

    // Print out descriptions of each arg
    printOptItems(verbose);
    printOptItems(help);
    printOptItems(data);
}
