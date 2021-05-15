#ifndef MINIARGPARSE_H
#define MINIARGPARSE_H

#include <string.h>
#include <assert.h>

typedef struct MiniargparseInfoBits {
    unsigned int hasValue   : 1;
    unsigned int hasErr     : 1;
    unsigned int used       : 1;
    unsigned int duplicate  : 1;
} miniargparseInfoBits;

typedef struct MiniargparseOpt {
    const char *shortName;
    const char *longName;
    const char *description;
    const char *value;
    const char *errValMsg;
    unsigned int index;
    miniargparseInfoBits infoBits;
    struct MiniargparseOpt *next;
} miniargparseOpt;

typedef struct MiniargparseOptlistItem {
    unsigned int makeHeadNode;
    miniargparseOpt *opt;
} miniargparseOptlistItem;

#define STR_MATCH(str1, str2) (strcmp(str1, str2) == 0)
#define STR_N_MATCH(str1, str2, n) (strncmp(str1, str2, n) == 0)
#define STR_USED(val) (!(val == NULL) && !(STR_MATCH(val, "")))

#define APPEND_OPT 0
#define HEAD_OPT 1

// Global error strings
enum miniargparseErrIndexes {
    MINIARGPARSE_ERR_MALFORMED_OPT_VAL,
    MINIARGPARSE_ERR_OPT_VAL_END_ARGV,
    MINIARGPARSE_ERR_VAL_IS_OPT,
    MINIARGPARSE_ERR_NON_VAL_OPT_VAL
};
static const char *g_miniargparseErrStrs[] = {
    "Malformed --<option>=<value>",
    "Option already at end of argv - expected value",
    "Value has option syntax (i.e. -, --)",
    "Value given on a non-value opt"
};

static miniargparseOpt *miniargparseOptlistController(miniargparseOptlistItem *option) {
    static miniargparseOpt *miniargparse_HEAD = NULL;
    if (option == NULL) {
        return miniargparse_HEAD;
    }
    else {
        if (miniargparse_HEAD == NULL || option->makeHeadNode == HEAD_OPT) {
            miniargparse_HEAD = option->opt;
            return NULL;
        }
        miniargparseOpt *tmp = miniargparse_HEAD;
        while (tmp->next != NULL) tmp = tmp->next;
        tmp->next = option->opt;
        return NULL;
    }
}

#define MINIARGPARSE_OPT(storeVal, sName, lName, hasVal, desc)                                          \
    assert(!STR_MATCH("-", sName) &&                                                                    \
        "[miniargparse]: ERROR - The '-' character is not permitted as a shortName");                   \
    assert(!(strlen(sName) > 1) &&                                                                      \
        "[miniargparse]: ERROR - The shortName string can only have 1 character");                      \
    miniargparseOpt storeVal =                                                                          \
        { "-" sName, "--" lName, desc, "", "", 0, { hasVal, 0, 0 }, NULL };                             \
    if (strcmp(storeVal.shortName, "-") == 0) { storeVal.shortName = ""; }                              \
    if (strcmp(storeVal.longName, "--") == 0) { storeVal.longName = ""; }                               \
    do {                                                                                                \
        miniargparseOptlistItem opt = { APPEND_OPT, &storeVal };                                        \
        miniargparseOptlistController(&opt);                                                            \
    } while(0)

// Parse argv for opts and return argv index on first-found unknown option (returns 0 if no unknown opts)
static int miniargparseParse(int argc, char *argv[]) {
    int firstUnknownOpt = 0;
    for (int i=1; i<argc; ++i) {
        // Check if arg is not a option-type
        if (argv[i][0] != '-') { continue; }

        int isLongOpt = 0;
        int validOpt = 0;
        miniargparseOpt *tmp = miniargparseOptlistController(NULL);

        // Look for opt in opts list
        while (tmp != NULL) {
            if (STR_USED(tmp->shortName) && STR_MATCH(tmp->shortName, argv[i])) {
                if (tmp->infoBits.used) { tmp->infoBits.duplicate = 1; }
                tmp->infoBits.used = 1;
                tmp->index = i;
                validOpt = 1;
            }
            else if (STR_USED(tmp->longName) && !tmp->infoBits.hasValue && STR_MATCH(argv[i], tmp->longName)) {
                if (tmp->infoBits.used) { tmp->infoBits.duplicate = 1; }
                isLongOpt = 1;
                tmp->infoBits.used = 1;
                tmp->index = i;
                validOpt = 1;
            }
            else {
                char *val = strchr(argv[i], '=');
                size_t offset = (int)(val - argv[i]);
                if (STR_USED(tmp->longName) && STR_N_MATCH(argv[i], tmp->longName, offset)) {
                    if (!tmp->infoBits.hasValue) {
                        tmp->infoBits.hasErr = 1;
                        tmp->errValMsg = g_miniargparseErrStrs[MINIARGPARSE_ERR_NON_VAL_OPT_VAL];
                        tmp->value = argv[i];
                    }
                    if (tmp->infoBits.used) { tmp->infoBits.duplicate = 1; }
                    isLongOpt = 1;
                    tmp->infoBits.used = 1;
                    tmp->index = i;
                    validOpt = 1;
                }
            }
            if (tmp->infoBits.used && tmp->infoBits.hasValue && validOpt) {
                if (isLongOpt) {
                    char *val;
                    size_t offset;
                    val = strchr(argv[i], '=');
                    if (val == NULL) {
                        tmp->infoBits.hasErr = 1;
                        tmp->errValMsg = g_miniargparseErrStrs[MINIARGPARSE_ERR_MALFORMED_OPT_VAL];
                        tmp->value = argv[i];
                    }
                    else {
                        offset = (int)(val - argv[i]);
                        tmp->value = &argv[i][offset+1];
                    }
                }
                else {
                    if ((i+1) >= argc) {
                        tmp->infoBits.hasErr = 1;
                        tmp->errValMsg = g_miniargparseErrStrs[MINIARGPARSE_ERR_OPT_VAL_END_ARGV];
                    }
                    else if (argv[i+1][0] == '-') {
                        tmp->infoBits.hasErr = 1;
                        tmp->errValMsg = g_miniargparseErrStrs[MINIARGPARSE_ERR_VAL_IS_OPT];
                        tmp->value = argv[i+1];
                    }
                    else {
                        tmp->value = argv[i+1];
                        ++i;
                    }
                }
            }
            if (tmp->infoBits.used && validOpt) { break; }
            tmp = tmp->next;
        }

        // Track first occurance of unknown opt
        if (!validOpt && firstUnknownOpt == 0) { firstUnknownOpt = i; }
    }
    return firstUnknownOpt;
}

static int miniargparseGetPositionalArg(int argc, char *argv[], size_t argvOffset) {
    size_t i;
    for (i=argvOffset+1; i<(unsigned)argc; ++i) {
        // Skip if opt-type value
        if (argv[i][0] == '-') { continue; }

        // Otherwise check if arg is opt-value type or not
        int isOptValue = 0;
        miniargparseOpt *tmp = miniargparseOptlistController(NULL);
        while (tmp != NULL) {
            if (tmp->infoBits.used && tmp->infoBits.hasValue && tmp->index == i-1) {
                isOptValue = 1;
            }
            tmp = tmp->next;
        }

        // Found positional arg
        if (!isOptValue) { return i; }
    }
    return 0;
}

#endif // MINIARGPARSE_H
