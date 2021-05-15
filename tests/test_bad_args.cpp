#include <iostream>

#include <gtest/gtest.h>
#include "miniargparse.h"

static miniargparseOptlistItem g_clearOptlist = {HEAD_OPT, NULL};

TEST(miniargparse, test_duplicate_opts) {
    // Mock argv and argc
    char *argv[] = {
        (char*)"<EXECUTABLE>",
        (char*)"-h",
        (char*)"--data=6",
        (char*)"--verbose",
        (char*)"-d",
        (char*)"7"
    };
    int argc = sizeof(argv) / sizeof(char*);

    // Add and parse opts
    MINIARGPARSE_OPT(help, "h", "help", 0, "Print out help and exit");
    MINIARGPARSE_OPT(verbose, "v", "verbose", 0, "Add verbosity");
    MINIARGPARSE_OPT(data, "d", "data", 1, "Test data-value option");
    int unknownOpt = miniargparseParse(argc, argv);
    EXPECT_EQ(unknownOpt, 0);

    // Check if duplicate was found
    EXPECT_EQ(data.infoBits.duplicate, 1U);

    // Cleanup HEAD node
    miniargparseOptlistController(&g_clearOptlist);
}

TEST(miniargparse, test_val_opt_end_argv) {
    // Mock argv and argc
    char *argv[] = {
        (char*)"<EXECUTABLE>",
        (char*)"-h",
        (char*)"--verbose",
        (char*)"-d"
    };
    int argc = sizeof(argv) / sizeof(char*);

    // Add and parse opts
    MINIARGPARSE_OPT(help, "h", "help", 0, "Print out help and exit");
    MINIARGPARSE_OPT(verbose, "v", "verbose", 0, "Add verbosity");
    MINIARGPARSE_OPT(data, "d", "data", 1, "Test data-value option");
    int unknownOpt = miniargparseParse(argc, argv);
    EXPECT_EQ(unknownOpt, 0);

    // Check if error was caught
    EXPECT_EQ(data.infoBits.hasErr, 1U);
    EXPECT_TRUE(STR_MATCH(data.errValMsg, g_miniargparseErrStrs[ERR_OPT_VAL_END_ARGV]));

    // Cleanup HEAD node
    miniargparseOptlistController(&g_clearOptlist);
}

TEST(miniargparse, test_unknown_opt) {
    // Mock argv and argc
    char *argv[] = {
        (char*)"<EXECUTABLE>",
        (char*)"--problem",
        (char*)"-h",
        (char*)"--verbose",
        (char*)"--data=8"
    };
    int argc = sizeof(argv) / sizeof(char*);

    // Add and parse opts
    MINIARGPARSE_OPT(help, "h", "help", 0, "Print out help and exit");
    MINIARGPARSE_OPT(verbose, "v", "verbose", 0, "Add verbosity");
    MINIARGPARSE_OPT(data, "d", "data", 1, "Test data-value option");
    int unknownOpt = miniargparseParse(argc, argv);
    EXPECT_NE(unknownOpt, 0);
    EXPECT_TRUE(STR_MATCH(argv[unknownOpt], "--problem"));

    // Cleanup HEAD node
    miniargparseOptlistController(&g_clearOptlist);
}

TEST(miniargparse, test_opt_val_err) {
    // Mock argv and argc
    char *argv[] = {
        (char*)"<EXECUTABLE>",
        (char*)"--verbose",
        (char*)"-d",
        (char*)"-h",
        (char*)"--data2=8"
    };
    int argc = sizeof(argv) / sizeof(char*);

    // Add and parse opts
    MINIARGPARSE_OPT(help, "h", "help", 0, "Print out help and exit");
    MINIARGPARSE_OPT(verbose, "v", "verbose", 0, "Add verbosity");
    MINIARGPARSE_OPT(data, "d", "data", 1, "Test data-value option");
    MINIARGPARSE_OPT(data2, "", "data2", 1, "Test data2-value option");
    int unknownOpt = miniargparseParse(argc, argv);
    EXPECT_EQ(unknownOpt, 0);

    // Check if error was caught
    EXPECT_EQ(data.infoBits.hasErr, 1U);
    EXPECT_TRUE(STR_MATCH(data.errValMsg, g_miniargparseErrStrs[ERR_VAL_IS_OPT]));

    // Cleanup HEAD node
    miniargparseOptlistController(&g_clearOptlist);
}

TEST(miniargparse, test_opt_val_malformed_long) {
    // Mock argv and argc
    char *argv[] = {
        (char*)"<EXECUTABLE>",
        (char*)"--verbose",
        (char*)"-d",
        (char*)"9",
        (char*)"-h",
        (char*)"--data2"
    };
    int argc = sizeof(argv) / sizeof(char*);

    // Add and parse opts
    MINIARGPARSE_OPT(help, "h", "help", 0, "Print out help and exit");
    MINIARGPARSE_OPT(verbose, "v", "verbose", 0, "Add verbosity");
    MINIARGPARSE_OPT(data, "d", "data", 1, "Test data-value option");
    MINIARGPARSE_OPT(data2, "", "data2", 1, "Test data2-value option");
    int unknownOpt = miniargparseParse(argc, argv);
    EXPECT_EQ(unknownOpt, 0);

    // Check if error was caught
    EXPECT_EQ(data2.infoBits.hasErr, 1U);
    EXPECT_TRUE(STR_MATCH(data2.errValMsg, g_miniargparseErrStrs[ERR_MALFORMED_OPT_VAL]));

    // Cleanup HEAD node
    miniargparseOptlistController(&g_clearOptlist);
}
