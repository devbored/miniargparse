#ifndef MINIARGPARSE_H
#define MINIARGPARSE_H

#include <string.h>
#include <stdbool.h>

typedef struct MiniargparseOpt {
    const char *shortName;
    const char *longName;
    bool hasValue;
    const char *description;
    bool used;
    const char *value;
    bool hasErrVal;
    const char *errValMsg;
} miniargparseOpt;

#define USED(val) (!(val == NULL) && !(strcmp(val, "") == 0))
#define STR_MATCH(str1, str2) (strcmp(str1, str2) == 0)
#define SUBSTR_MATCH(str1, str2) (strstr(str1, str2) != NULL)

// Global error strings
const char *g_errMalformedOptionVal = "Malformed --<option>=<value> : ";
const char *g_errOptValEndOfArgv = "Option already at end of argv";
const char *g_errValIsOpt = "Value has option syntax (i.e. -, --) : ";

#define MINIARGPARSE_INIT(argc, arvg)   \
    int MINIARGPARSE_##argc = argc;     \
    char **MINIARGPARSE_##argv = argv;  \
    do {} while(0)

#define MINIARGPARSE_OPT(storeValue, shortName, longName, hasVal, description)              \
    miniargparseOpt storeValue = { shortName, longName, hasVal, description, 0, "", 0, "" };\
    do {                                                                                    \
        for (size_t i=1; i<MINIARGPARSE_argc; ++i) {                                        \
            int isLongOpt = 0;                                                              \
            if (USED(shortName) && STR_MATCH("-" shortName, MINIARGPARSE_argv[i])) {        \
                storeValue.used = 1;                                                        \
            }                                                                               \
            else if (USED(longName) && SUBSTR_MATCH(MINIARGPARSE_argv[i], "--" longName)) { \
                isLongOpt = 1;                                                              \
                storeValue.used = 1;                                                        \
            }                                                                               \
            if (storeValue.used && hasVal == 1) {                                           \
                if (isLongOpt) {                                                            \
                    char *val;                                                              \
                    size_t offset;                                                          \
                    val = strchr(MINIARGPARSE_argv[i], '=');                                \
                    if (val == NULL) {                                                      \
                        storeValue.hasErrVal = 1;                                           \
                        storeValue.errValMsg = g_errMalformedOptionVal;                     \
                        storeValue.value = MINIARGPARSE_argv[i];                            \
                    }                                                                       \
                    else {                                                                  \
                        offset = (int)(val - MINIARGPARSE_argv[i]);                         \
                        storeValue.value = &MINIARGPARSE_argv[i][offset+1];                 \
                    }                                                                       \
                }                                                                           \
                else {                                                                      \
                    if ((i+1) >= MINIARGPARSE_argc) {                                       \
                        storeValue.hasErrVal = 1;                                           \
                        storeValue.errValMsg = g_errOptValEndOfArgv;                        \
                    }                                                                       \
                    else if (MINIARGPARSE_argv[i+1][0] == '-') {                            \
                        storeValue.hasErrVal = 1;                                           \
                        storeValue.errValMsg = g_errValIsOpt;                               \
                        storeValue.value = MINIARGPARSE_argv[i+1];                          \
                    }                                                                       \
                    else {                                                                  \
                        storeValue.value = MINIARGPARSE_argv[i+1];                          \
                    }                                                                       \
                    ++i;                                                                    \
                }                                                                           \
                break;                                                                      \
            }                                                                               \
        }                                                                                   \
    } while(0)

#endif // MINIARGPARSE_H
